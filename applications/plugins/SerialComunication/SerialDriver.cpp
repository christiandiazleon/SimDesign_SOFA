/******************************************************************/
/* serial.h    Andrés Sánchez.    November, 2017          */
/*----------------------------------------------------------------*/
/* Serial communications in SOFA                                 */
/*----------------------------------------------------------------*/
/* GPL LICENSE                                                    */
/******************************************************************/
#include "SerialDriver.h"
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/simulation/AnimateEndEvent.h>
#include <sofa/simulation/Node.h>
#include <sofa/simulation/MechanicalVisitor.h>
#include <sofa/simulation/UpdateMappingVisitor.h>
#include <sofa/core/objectmodel/KeypressedEvent.h>
#include <sofa/core/objectmodel/KeyreleasedEvent.h>
#include <sofa/core/objectmodel/MouseEvent.h>

#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
 
#include <sofa/helper/system/thread/CTime.h>
#ifdef SOFA_HAVE_BOOST
#include <boost/thread.hpp>
#endif

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

using namespace std;
using std::cout;
using std::endl;

double prevTime;
bool frame;
bool visuCreation;

//------------------
//-- CONSTANTS
//------------------

//-- ASCII string to send through the serial port
namespace sofa
{

namespace component
{

namespace controller
{
//-- ASCII String
//#define CMD       "0.1"

//-- ASCII String length
#define CMD_LEN   4

//--TIMEOUT in micro-sec (It is set to 1 sec in this example)
#define TIMEOUT 50000

const char* SerialDriver::visualNodeNames[NVISUALNODE] =
{
    "stylus"
};
const char* SerialDriver::visualNodeFiles[NVISUALNODE] =
{
    "/home/asanchez/Universidad/Investigación/Christian/sofa/src/applications/plugins/Sensable/examples/data/mesh/dental_instrument.obj"
};

using namespace sofa::defaulttype;



vector<SerialDriver*> autreOmniDriver;

bool initDeviceBool;
bool frameAvant = false;
bool desktop = false;
int compteur_debug = 0;

static sofa::helper::system::atomic<int> doUpdate;


int serial_fd;           //-- Serial port descriptor
char data[CMD_LEN+1];    //-- The received command
char path[15] = "/dev/ttyUSB0";
float n2 = 0.0;

//Socket Variables
struct sockaddr_in si_me, si_other;
     
int s, i,recv_len;
socklen_t slen = sizeof(si_other);

char buf[BUFLEN];


int SerialDriver::initDevice()
{
	std::cout<<"init Device is called"<<std::endl;
    return 1;
}

SerialDriver::SerialDriver()
	: forceScale(initData(&forceScale, 1.0, "forceScale","Default forceScale applied to the force feedback. "))
    , scale(initData(&scale, 100.0, "scale","Default scale applied to the Phantom Coordinates. "))
    , positionBase(initData(&positionBase, Vec3d(0,0,0), "positionBase","Position of the interface base in the scene world coordinates"))
    , orientationBase(initData(&orientationBase, Quat(0,0,0,1), "orientationBase","Orientation of the interface base in the scene world coordinates"))
    , positionTool(initData(&positionTool, Vec3d(0,0,0), "positionTool","Position of the tool in the omni end effector frame"))
    , orientationTool(initData(&orientationTool, Quat(0,0,0,1), "orientationTool","Orientation of the tool in the omni end effector frame"))
    , permanent(initData(&permanent, false, "permanent" , "Apply the force feedback permanently"))
    , omniVisu(initData(&omniVisu, false, "omniVisu", "Visualize the position of the interface in the virtual scene"))
    , posDevice(initData(&posDevice, "posDevice", "position of the base of the part of the device"))
    , posStylus(initData(&posStylus, "posStylus", "position of the base of the stylus"))
    , locDOF(initData(&locDOF,"locDOF","localisation of the DOFs MechanicalObject"))
    , deviceName(initData(&deviceName,std::string("Default PHANToM"),"deviceName","name of the device"))
    , deviceIndex(initData(&deviceIndex,1,"deviceIndex","index of the device"))
    , openTool(initData(&openTool,"openTool","opening of the tool"))
    , maxTool(initData(&maxTool,1.0,"maxTool","maxTool value"))
    , minTool(initData(&minTool,0.0,"minTool","minTool value"))
    , openSpeedTool(initData(&openSpeedTool,0.1,"openSpeedTool","openSpeedTool value"))
    , closeSpeedTool(initData(&closeSpeedTool,0.1,"closeSpeedTool","closeSpeedTool value"))
    , useScheduler(initData(&useScheduler,false,"useScheduler","Enable use of OpenHaptics Scheduler methods to synchronize haptics thread"))
    , setRestShape(initData(&setRestShape, false, "setRestShape", "True to control the rest position instead of the current position directly"))
    , applyMappings(initData(&applyMappings, true, "applyMappings", "True to enable applying the mappings after setting the position"))
    , alignOmniWithCamera(initData(&alignOmniWithCamera, true, "alignOmniWithCamera", "True to keep the Omni's movements in the same reference frame as the camera"))
{
    this->f_listening.setValue(true);
    data_s.forceFeedback = NULL;
    noDevice = false;
    firstInit=true;
    firstDevice = true;
    addAlias(&omniVisu,"drawDevice");

    /*//-- Check if the serial device name is given
    if (argc<2) {
        printf ("No serial device name is given\n");
        exit(0);
    }*/

}

SerialDriver::~SerialDriver()
{
}

void SerialDriver::setForceFeedback(ForceFeedback* ff)
{
	// the forcefeedback is already set
    if(data_s.forceFeedback == ff)
    {
        return;
    }

    data_s.forceFeedback = ff;
};

void SerialDriver::die(char *s)
{
    perror(s);
    exit(1);
}

void SerialDriver::init(){

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    sofa::simulation::Node::SPtr rootContext = static_cast<simulation::Node*>(this->getContext()->getRootContext());	

    if(alignOmniWithCamera.getValue())
    {
        camera = this->getContext()->get<component::visualmodel::InteractiveCamera>(this->getTags(), sofa::core::objectmodel::BaseContext::SearchRoot);
        if(!camera)
        {
            camera = this->getContext()->get<component::visualmodel::InteractiveCamera>();
        }
        if (!camera)
        {
            sofa::simulation::Node::SPtr groot = dynamic_cast<simulation::Node*>(this->getContext());
            camera = sofa::core::objectmodel::New<component::visualmodel::InteractiveCamera>();
            camera->setName(core::objectmodel::Base::shortName(camera.get()));
            groot->addObject(camera);
            camera->bwdInit();
        }
        if(!camera)
        {
            serr << "Cannot find or create Camera." << sendl;
        }
    }

    nodePrincipal = rootContext->createChild("omniVisu "+deviceName.getValue());
    nodePrincipal->updateContext();

    initVisu=false;
    visuActif=false;

    for(int i=0; i<NVISUALNODE; i++)
    {
        visualNode[i].visu = NULL;
        visualNode[i].mapping = NULL;
    }

    if(!initVisu)
    {
        rigidDOF = NULL;

        if(rigidDOF == NULL){
            rigidDOF = sofa::core::objectmodel::New<MMechanicalObject>();

            nodePrincipal->addObject(rigidDOF);
            rigidDOF->name.setValue("rigidDOF");

            VecCoord& posDOF =*(rigidDOF->x.beginEdit());
                posDOF.resize(NVISUALNODE);
                posDOFEST_X = posDOF[1].getCenter()[0];
                //std::cout << posDOFEST_X << std::endl;
                posDOFEST_Y = posDOF[1].getCenter()[1];
                //std::cout << posDOFEST_Y << std::endl;
                posDOFEST_Z = posDOF[1].getCenter()[2];
                //std::cout << posDOFEST_Z << std::endl;
                std::cout << positionBase.getValue() << std::endl;
                /*
                posROT1 = posDOF[1].getOrigin()[0];
                posROT2 = posDOF[1].getOrigin()[1];
                posROT3 = posDOF[1].getOrigin()[2];
                posROT4 = posDOF[1].getOrigin()[3];
                std::cout << "POS ROT[0]: " << posROT1 << std::endl;
                std::cout << "POS ROT[1]: " << posROT2 << std::endl;
                std::cout << "POS ROT[2]: " << posROT3 << std::endl;
                std::cout << "POS ROT[4]: " << posROT4 << std::endl;*/
            rigidDOF->x.endEdit();

            rigidDOF->init();

            nodePrincipal->updateContext();
        }
    }

    //Looking for MechanicalObjects
    /*std::vector<MMechanicalObject*> listMechanicalObj;
    getContext()->get<MMechanicalObject>(&listMechanicalObj, core::objectmodel::BaseContext::SearchDown);
    std::cout << "Mi nombre es " << listMechanicalObj.size() << std::endl;
    int numberM = listMechanicalObj.size();*/


    getContext()->get<MechanicalObjectType>(&objectsMechTemp, core::objectmodel::BaseContext::SearchDown);

    int numberM = objectsMechTemp.size();
    
    //std::cout << "Number of MechanicalObjects: " << objectsMechTemp[0]->name.getValue() << std::endl;

    getContext()->get<MechanicalObjectType2>(&collisionRigid, core::objectmodel::BaseContext::SearchDown);

    int numberM2 = objectsMechTemp.size();
    
    //std::cout << "Number of ColissModel: " << collisionRigid[0]->name.getValue() << std::endl;

    if(numberM>0){
        //Looking for OglModel Components
        std::vector<sofa::component::visualmodel::OglModel*> list_oglModels;
        getContext()->get<sofa::component::visualmodel::OglModel>(&list_oglModels, core::objectmodel::BaseContext::SearchDown);

        int number = list_oglModels.size();
        
        //std::cout << "Number of visual models: " << number << std::endl;

        if(number>0){


            visualNode[0].visu = list_oglModels[0];

            //Looking for Principal Instrument Visual Node
            visualNode[0].node = (simulation::Node*) list_oglModels[0]->getContext();
            //std::cout << "Name of the visual model identified: "  << visualNode[0].node->name.getValue() << std::endl;

            visualNode[0].mapping = sofa::core::objectmodel::New< sofa::component::mapping::RigidMapping< Rigid3dTypes, ExtVec3fTypes > > ();
            visualNode[0].node->addObject(visualNode[0].mapping);
            visualNode[0].mapping->setModels(objectsMechTemp[0], visualNode[0].visu.get());
            visualNode[0].mapping->name.setValue("RigidMapping");
            visualNode[0].mapping->f_mapConstraints.setValue(false);
            visualNode[0].mapping->f_mapForces.setValue(false);
            visualNode[0].mapping->index.setValue(1);
            visualNode[0].mapping->init();
            
            visualNode[0].node->updateContext();

            sofa::defaulttype::ResizableExtVector< sofa::defaulttype::Vec<3,float> > &scaleMapping = *(visualNode[0].mapping->points.beginEdit());
            for(unsigned int i=0; i<scaleMapping.size(); i++){
                scaleMapping[i] *= (float)(1.0*scale.getValue()/100.0);
            }

            visualNode[0].mapping->points.endEdit();

            visualNode[0].nodeColis = (simulation::Node*) collisionRigid[0]->getContext();
            visualNode[0].mappingColis = sofa::core::objectmodel::New< sofa::component::mapping::RigidMapping< Rigid3dTypes, Vec3dTypes > > ();
            visualNode[0].nodeColis->addObject(visualNode[0].mappingColis);
            visualNode[0].mappingColis->setModels(objectsMechTemp[0], collisionRigid[0]);
            visualNode[0].mappingColis->name.setValue("CollisionMapping");
            visualNode[0].mappingColis->index.setValue(1);
            visualNode[0].mappingColis->f_mapConstraints.setValue(true);
            visualNode[0].mappingColis->f_mapForces.setValue(true);
            visualNode[0].mappingColis->init();
   
            visualNode[0].nodeColis->updateContext();

            initVisu=true;
            visuActif=false;
        }else{
            std::cout << "No hay nodos visuales";
        }    
    }else{
        std::cout << "The scene don't have MechanicalObjects" << std::endl;
    }

    nodePrincipal->updateContext();

    //-- Open the serial port
    //-- The speed is configure at 9600 baud
    //serial_fd=serial_open(path,B9600);
    //int flush = tcflush(serial_fd,TCIOFLUSH);
    //std::thread first (&SerialDriver::serial_read, this, serial_fd, data, CMD_LEN, TIMEOUT);

    //if (serial_fd==-1) {
      //  printf ("Error opening the serial device: %s\n", path);
        //perror("OPEN");
        //exit(0);
    //}
    
}

void SerialDriver::cleanup()
{
    sout << "SerialDriver::cleanup()" << sendl;

    //if(firstDevice)
        //hdScheduleSynchronous(stopCallback, (void*) &autreOmniDriver, HD_MAX_SCHEDULER_PRIORITY);
    //isInitialized = false;
}

void SerialDriver::bwdInit(){

}

void SerialDriver::setDataValue(){
	data_s.scale = scale.getValue();
    data_s.forceScale = forceScale.getValue();

    Quat q = orientationBase.getValue();
    q.normalize();
    orientationBase.setValue(q);
    data_s.world_H_baseOmni.set( positionBase.getValue(), q		);
    q=orientationTool.getValue();
    q.normalize();
    data_s.endOmni_H_virtualTool.set(positionTool.getValue(), q);
    data_s.permanent_feedback = permanent.getValue();
}

void SerialDriver::reset(){
	std::cout<<"SerialDriver::reset() is called" <<std::endl;
    this->reinit();
}

void SerialDriver::reinit(){
	std::cout<<"SerialDriver::reinit() is called" <<std::endl;

	this->cleanup();
	this->bwdInit();
	if(data_s.scale!=oldScale)
		changeScale = true;
	std::cout<<"SerialDriver::reinit() done" <<std::endl;
}

void SerialDriver::draw(const core::visual::VisualParams* vparam)
{
    vparam = NULL;
    draw();
}


void SerialDriver::draw()
{
    HDdouble transform[16];
    // get Position and Rotation from transform => put in servoDeviceData
    Mat3x3d mrot;
    Quat rot;

    if(initVisu)
    {   
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
        
        char * pch;
        //printf ("Splitting string \"%s\" into tokens:\n",buf);
        pch = strtok (buf," ,");
        float ftemp[16];
        int i = 0;

        while (pch != NULL && i<16)
        {
            ftemp[i] = atof(pch);
            std::cout << ftemp[i] << std::endl;
            //printf ("%s\n",pch);
            pch = strtok (NULL, " ,");
            i++;
        }
      
        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        //printf("Data: %s\n" , buf);
        
        for (int u=0; u<3; u++)
                for (int j=0; j<3; j++)
                    mrot[u][j] = ftemp[j*4+u];

        rot.fromMatrix(mrot);
        rot.normalize();
        
        VecCoord& posDOF =*(objectsMechTemp[0]->x.beginEdit());
            //posDOF.resize(NVISUALNODE+1);
            posDOF[1].getCenter()[0] =  posDOFEST_X + ftemp[12]*0.01;
            posDOF[1].getCenter()[1] =  posDOFEST_Y + ftemp[13]*0.01;
            posDOF[1].getCenter()[2] =  posDOFEST_Z + ftemp[14]*0.01;
            posDOF[1].getCenter() =  posDOF[1].getCenter() - positionBase.getValue();
            //std::cout << "Orientation: " << posDOF[1].getOrientation()[1] << std::endl;
            posDOF[1].getOrientation() =  rot;

            posDOF[1].getCenter() =  posDOF[1].getCenter() + positionBase.getValue();
            //std::cout << "PosRigid: " <<posDOF[1].getCenter()[2] << std::endl;
        objectsMechTemp[0]->x.endEdit();

        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
            std::cout << "Error" << std::endl;
        }
        //close(s);
    }
}

void SerialDriver::onKeyReleasedEvent(core::objectmodel::KeyreleasedEvent *kre){
    kre = NULL;
}

void SerialDriver::onKeyPressedEvent(core::objectmodel::KeypressedEvent *kpe)
{
    kpe = NULL;
}

void SerialDriver::onAnimateBeginEvent()
{

}

void SerialDriver::handleEvent(core::objectmodel::Event *event){
	if (dynamic_cast<sofa::simulation::AnimateBeginEvent *>(event))
    {
        onAnimateBeginEvent();
    }
    else if (dynamic_cast<core::objectmodel::KeypressedEvent *>(event))
    {
        core::objectmodel::KeypressedEvent *kpe = dynamic_cast<core::objectmodel::KeypressedEvent *>(event);
        onKeyPressedEvent(kpe);
    }
    else if (dynamic_cast<core::objectmodel::KeyreleasedEvent *>(event))
    {
        core::objectmodel::KeyreleasedEvent *kre = dynamic_cast<core::objectmodel::KeyreleasedEvent *>(event);
        onKeyReleasedEvent(kre);
    }
}

int SerialDriverClass = sofa::core::RegisterObject("This component does nothing.").add<SerialDriver>();

SOFA_DECL_CLASS(SerialDriver);

} // namespace controller

} // namespace component

} // namespace sofa