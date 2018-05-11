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
#include <stdio.h>
#include <stdlib.h>
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
#define CMD_LEN 4

//--TIMEOUT in micro-sec (It is set to 1 sec in this example)
#define TIMEOUT 50000

const char *SerialDriver::visualNodeNames[NVISUALNODE] =
    {
        "stylus"};
const char *SerialDriver::visualNodeFiles[NVISUALNODE] =
    {
        // Caution, update this path ...
        "/home/bgarcial/workspace/sofa/src/applications/plugins/Sensable/examples/data/mesh/dental_instrument.obj"};

using namespace sofa::defaulttype;

vector<SerialDriver *> autreOmniDriver;

bool initDeviceBool;
bool frameAvant = false;
bool desktop = false;
int compteur_debug = 0;

static sofa::helper::system::atomic<int> doUpdate;

int serial_fd;          //-- Serial port descriptor
char data[CMD_LEN + 1]; //-- The received command
//string data;

    char path[15] = "/dev/ttyUSB0";
float n2 = 0.0;

int SerialDriver::initDevice()
{
    std::cout << "init Device is called" << std::endl;
    return 1;
}

SerialDriver::SerialDriver()
    : forceScale(initData(&forceScale, 1.0, "forceScale", "Default forceScale applied to the force feedback. ")), scale(initData(&scale, 100.0, "scale", "Default scale applied to the Phantom Coordinates. ")), positionBase(initData(&positionBase, Vec3d(0, 0, 0), "positionBase", "Position of the interface base in the scene world coordinates")), orientationBase(initData(&orientationBase, Quat(0, 0, 0, 1), "orientationBase", "Orientation of the interface base in the scene world coordinates")), positionTool(initData(&positionTool, Vec3d(0, 0, 0), "positionTool", "Position of the tool in the omni end effector frame")), orientationTool(initData(&orientationTool, Quat(0, 0, 0, 1), "orientationTool", "Orientation of the tool in the omni end effector frame")), permanent(initData(&permanent, false, "permanent", "Apply the force feedback permanently")), omniVisu(initData(&omniVisu, false, "omniVisu", "Visualize the position of the interface in the virtual scene")), posDevice(initData(&posDevice, "posDevice", "position of the base of the part of the device")), posStylus(initData(&posStylus, "posStylus", "position of the base of the stylus")), locDOF(initData(&locDOF, "locDOF", "localisation of the DOFs MechanicalObject")), deviceName(initData(&deviceName, std::string("Default PHANToM"), "deviceName", "name of the device")), deviceIndex(initData(&deviceIndex, 1, "deviceIndex", "index of the device")), openTool(initData(&openTool, "openTool", "opening of the tool")), maxTool(initData(&maxTool, 1.0, "maxTool", "maxTool value")), minTool(initData(&minTool, 0.0, "minTool", "minTool value")), openSpeedTool(initData(&openSpeedTool, 0.1, "openSpeedTool", "openSpeedTool value")), closeSpeedTool(initData(&closeSpeedTool, 0.1, "closeSpeedTool", "closeSpeedTool value")), useScheduler(initData(&useScheduler, false, "useScheduler", "Enable use of OpenHaptics Scheduler methods to synchronize haptics thread")), setRestShape(initData(&setRestShape, false, "setRestShape", "True to control the rest position instead of the current position directly")), applyMappings(initData(&applyMappings, true, "applyMappings", "True to enable applying the mappings after setting the position")), alignOmniWithCamera(initData(&alignOmniWithCamera, true, "alignOmniWithCamera", "True to keep the Omni's movements in the same reference frame as the camera"))
{
    this->f_listening.setValue(true);
    data_s.forceFeedback = NULL;
    noDevice = false;
    firstInit = true;
    firstDevice = true;
    addAlias(&omniVisu, "drawDevice");

    /*//-- Check if the serial device name is given
    if (argc<2) {
        printf ("No serial device name is given\n");
        exit(0);
    }*/
}

SerialDriver::~SerialDriver()
{
}

void SerialDriver::setForceFeedback(ForceFeedback *ff)
{
    // the forcefeedback is already set
    if (data_s.forceFeedback == ff)
    {
        return;
    }

    data_s.forceFeedback = ff;
};

void SerialDriver::init()
{
    std::cout << "Entrando al metodo init del Serial Driver.cpp" << std::endl;
    
    n1 = 0.0f;

    sofa::simulation::Node::SPtr rootContext = static_cast<simulation::Node *>(this->getContext()->getRootContext());

    if (alignOmniWithCamera.getValue())
    {
        camera = this->getContext()->get<component::visualmodel::InteractiveCamera>(this->getTags(), sofa::core::objectmodel::BaseContext::SearchRoot);
        if (!camera)
        {
            camera = this->getContext()->get<component::visualmodel::InteractiveCamera>();
        }
        if (!camera)
        {
            sofa::simulation::Node::SPtr groot = dynamic_cast<simulation::Node *>(this->getContext());
            camera = sofa::core::objectmodel::New<component::visualmodel::InteractiveCamera>();
            camera->setName(core::objectmodel::Base::shortName(camera.get()));
            groot->addObject(camera);
            camera->bwdInit();
        }
        if (!camera)
        {
            serr << "Cannot find or create Camera." << sendl;
        }
    }

    nodePrincipal = rootContext->createChild("omniVisu " + deviceName.getValue());
    nodePrincipal->updateContext();
    initVisu = false;
    visuActif = false;

    for (int i = 0; i < NVISUALNODE; i++)
    {
        visualNode[i].visu = NULL;
        visualNode[i].mapping = NULL;
    }

    if (!initVisu)
    {
        rigidDOF = NULL;

        if (rigidDOF == NULL)
        {
            rigidDOF = sofa::core::objectmodel::New<MMechanicalObject>();

            nodePrincipal->addObject(rigidDOF);
            rigidDOF->name.setValue("rigidDOF");

            VecCoord &posDOF = *(rigidDOF->x.beginEdit());
            posDOF.resize(NVISUALNODE);
            posDOFEST = posDOF[0].getCenter()[0];
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

    /** 
     * We get a 
    */
    getContext()->get<MechanicalObjectType>(&objectsMechTemp, core::objectmodel::BaseContext::SearchDown);

    int numberM = objectsMechTemp.size();

    //std::cout << "Number of MechanicalObjects: " << objectsMechTemp[0]->name.getValue() << std::endl;

    getContext()->get<MechanicalObjectType2>(&collisionRigid, core::objectmodel::BaseContext::SearchDown);

    int numberM2 = objectsMechTemp.size();

    //std::cout << "Number of ColissModel: " << collisionRigid[0]->name.getValue() << std::endl;

    if (numberM > 0)
    {
        //Looking for OglModel Components
        std::vector<sofa::component::visualmodel::OglModel *> list_oglModels;
        getContext()->get<sofa::component::visualmodel::OglModel>(&list_oglModels, core::objectmodel::BaseContext::SearchDown);

        int number = list_oglModels.size();

        //std::cout << "Number of visual models: " << number << std::endl;

        if (number > 0)
        {

            visualNode[0].visu = list_oglModels[0];

            //Looking for Principal Instrument Visual Node
            visualNode[0].node = (simulation::Node *)list_oglModels[0]->getContext();
            //std::cout << "Name of the visual model identified: "  << visualNode[0].node->name.getValue() << std::endl;

            visualNode[0].mapping = sofa::core::objectmodel::New<sofa::component::mapping::RigidMapping<Rigid3dTypes, ExtVec3fTypes>>();
            visualNode[0].node->addObject(visualNode[0].mapping);
            visualNode[0].mapping->setModels(objectsMechTemp[0], visualNode[0].visu.get());
            visualNode[0].mapping->name.setValue("RigidMapping");
            visualNode[0].mapping->f_mapConstraints.setValue(false);
            visualNode[0].mapping->f_mapForces.setValue(false);
            visualNode[0].mapping->index.setValue(1);
            visualNode[0].mapping->init();

            visualNode[0].node->updateContext();

            sofa::defaulttype::ResizableExtVector<sofa::defaulttype::Vec<3, float>> &scaleMapping = *(visualNode[0].mapping->points.beginEdit());
            for (unsigned int i = 0; i < scaleMapping.size(); i++)
            {
                scaleMapping[i] *= (float)(1.0 * scale.getValue() / 100.0);
            }

            visualNode[0].mapping->points.endEdit();

            visualNode[0].nodeColis = (simulation::Node *)collisionRigid[0]->getContext();
            visualNode[0].mappingColis = sofa::core::objectmodel::New<sofa::component::mapping::RigidMapping<Rigid3dTypes, Vec3dTypes>>();
            visualNode[0].nodeColis->addObject(visualNode[0].mappingColis);
            visualNode[0].mappingColis->setModels(objectsMechTemp[0], collisionRigid[0]);
            visualNode[0].mappingColis->name.setValue("CollisionMapping");
            visualNode[0].mappingColis->index.setValue(1);
            visualNode[0].mappingColis->f_mapConstraints.setValue(true);
            visualNode[0].mappingColis->f_mapForces.setValue(true);
            visualNode[0].mappingColis->init();

            visualNode[0].nodeColis->updateContext();

            initVisu = true;
            visuActif = false;
        }
        else
        {
            std::cout << "No hay nodos visuales";
        }
    }
    else
    {
        std::cout << "The scene don't have MechanicalObjects" << std::endl;
    }

    nodePrincipal->updateContext();

    //-- Open the serial port
    //-- The speed is configure at 9600 baud
    serial_fd = serial_open(path, B9600);
    int flush = tcflush(serial_fd, TCIOFLUSH);
    //std::thread first (&SerialDriver::serial_read, this, serial_fd, data, CMD_LEN, TIMEOUT);

    if (serial_fd == -1)
    {
        printf("Error opening the serial device: %s\n", "/dev/usbnoseque");
        perror("OPEN");
        exit(0);
    }
}

void SerialDriver::cleanup()
{
    sout << "SerialDriver::cleanup()" << sendl;

    //if(firstDevice)
    //hdScheduleSynchronous(stopCallback, (void*) &autreOmniDriver, HD_MAX_SCHEDULER_PRIORITY);
    //isInitialized = false;
}

void SerialDriver::bwdInit()
{
}

void SerialDriver::setDataValue()
{
    data_s.scale = scale.getValue();
    data_s.forceScale = forceScale.getValue();

    Quat q = orientationBase.getValue();
    q.normalize();
    orientationBase.setValue(q);
    data_s.world_H_baseOmni.set(positionBase.getValue(), q);
    q = orientationTool.getValue();
    q.normalize();
    data_s.endOmni_H_virtualTool.set(positionTool.getValue(), q);
    data_s.permanent_feedback = permanent.getValue();
}

void SerialDriver::reset()
{
    std::cout << "SerialDriver::reset() is called" << std::endl;
    this->reinit();
}

void SerialDriver::reinit()
{
    std::cout << "SerialDriver::reinit() is called" << std::endl;

    this->cleanup();
    this->bwdInit();
    if (data_s.scale != oldScale)
        changeScale = true;
    std::cout << "SerialDriver::reinit() done" << std::endl;
}

void SerialDriver::draw(const core::visual::VisualParams *vparam)
{
    std::cout << "Entrando al metodo draw del SerialDriver.cpp con parametros" << std::endl;
    vparam = NULL;
    draw();
}

void SerialDriver::draw()
{
    std::cout << "Entrando al metodo draw solo del SerialDriver.cpp " << std::endl;
    if (initVisu)
    {
        //VecCoord& posD =(*posDevice.beginEdit());
        if (serial_fd != -1)
        {
            float n;
            float n4;
            int flush = tcflush(serial_fd, TCIOFLUSH);
            n = serial_read(serial_fd, data, CMD_LEN, TIMEOUT);
            
            // este parametro data es de la escena, son como las coordenadas o posicion del instrumento
            // cambia cuando muevo la escena con el mouse en sofa

            std::cout << "Dato traido desde el serial_read " << data << std::endl;
            std::cout << "tipo de dato de data" << typeid(data).name() << std::endl;
            flush = tcflush(serial_fd, TCIOFLUSH);
            //n = n*0.01f;
            n4 = atof(data);
            cout << "Este es n4 " << n4 << std::endl;
            n1 = atof(data) * 0.5f;
            // se multiplica por 0.5 para escalar el dato que se lee y mejorar la precision
            // con respecto a lo que se lee y se mueve en el hapkit y la escena
            cout << "Este es n1 que tiene a data turn it " << n1 << std::endl;

            VecCoord &posDOF = *(objectsMechTemp[0]->x.beginEdit());
            posDOF.resize(NVISUALNODE + 1);

            // en el draw se convierte ese data positon instrumen a  float 
            positionInstrument = atof(data);
            //positionInstrument = sscanf(data, "%f", n1);
            posDOF[1].getCenter()[2] = posDOFEST + n1;
            //std::cout << "PosRigid: " <<posDOF[1].getCenter()[2] << std::endl;
            objectsMechTemp[0]->x.endEdit();
        }
        //std::cout << posDOF[1].getCenter()[2] << std::endl;
        //for(int i=0;i<NVISUALNODE;i++)
        //{+ 0.01f
        //  if(omniVisu.getValue() || i>6)
        //  {
        //      visualNode[i].visu->drawVisual();
        //      visualNode[i].mapping->draw();
        //  }
        //}
        //rigidDOF->x.endEdit();
    }

    std::cout << "Serial Driver draw n1: " << n1 << " " << positionInstrument << std::endl;
    printf("%.3f", positionInstrument);
    //std::cout<<"SerialDriver::draw() is called" <<std::endl;
}

float SerialDriver::askDevice()
{
    /*std::cout << "Entrando al metodo askDevice " << std::endl;
    // float n1 = -1;
    if (serial_fd != -1)
    {
        // float n1;
        float n;
        int flush = tcflush(serial_fd, TCIOFLUSH);
        n = serial_read(serial_fd, data, CMD_LEN, TIMEOUT);

        // este parametro data es de la escena

        std::cout << data << "aaj" << std::endl;
        flush = tcflush(serial_fd, TCIOFLUSH);
        //n = n*0.01f;

        n1 = atof(data) * 0.5;
        cout << "este es n1 en donde estan los datos leidos del hapkit" << n1 << std::endl;
        return n1;
    }
    return n1;*/
    return 0.0f;
}

void SerialDriver::onKeyReleasedEvent(core::objectmodel::KeyreleasedEvent *kre)
{
    kre = NULL;
}

void SerialDriver::onKeyPressedEvent(core::objectmodel::KeypressedEvent *kpe)
{
    kpe = NULL;
}

void SerialDriver::onAnimateBeginEvent()
{
}

void SerialDriver::handleEvent(core::objectmodel::Event *event)
{
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

//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************

int SerialDriver::serial_open(char *serial_name, speed_t baud)
{
    struct termios newtermios;
    int fd;

    // Open the serial port
    fd = open(serial_name, O_RDWR | O_NOCTTY);

    // Configure the serial port attributes:
    //   -- No parity
    //   -- 8 data bits
    //   -- other things...
    newtermios.c_cflag = CBAUD | CS8 | CLOCAL | CREAD;
    newtermios.c_iflag = IGNPAR;
    `serial_read` newtermios.c_oflag = 0;
    newtermios.c_lflag = 0;
    newtermios.c_cc[VMIN] = 1;
    newtermios.c_cc[VTIME] = 0;

    // Set the speed
    cfsetospeed(&newtermios, baud);
    cfsetispeed(&newtermios, baud);

    // flush the input buffer
    if (tcflush(fd, TCIFLUSH) == -1)
    {
        return -1;
    }

    // flush the output buffer
    if (tcflush(fd, TCOFLUSH) == -1)
    {
        return -1;
    }

    //-- Configure the serial port now!!
    if (tcsetattr(fd, TCSANOW, &newtermios) == -1)
    {
        return -1;
    }

    //-- Return the file descriptor
    return fd;
}

void SerialDriver::serial_send(int serial_fd, char *data, int size)
{
    std::cout << "Ha entrado a serial_send " << std::endl;
    serial_fd = 0;
    data = NULL;
    size = 0;
}

int SerialDriver::serial_read(int serial_fd, char *data, int size, int timeout_usec)
{
    std::cout << "Entrando al metodo serial_read " << std::endl;
    fd_set fds;
    struct timeval timeout;
    bool band = false;
    int count = 0;
    int ret;
    int n;

    std::cout << "Han sido definidas las variables en serial_read " << std::endl;

    //-- Wait for the data. A block of size bytes is expected to arrive
    //-- within the timeout_usec time. This block can be received as
    //-- smaller blocks.
    do
    {
        //-- Set the fds variable to wait for the serial descriptor
        FD_ZERO(&fds);
        FD_SET(serial_fd, &fds);

        //-- Set the timeout in usec.
        timeout.tv_sec = 0;
        timeout.tv_usec = timeout_usec;
        std::cout << "timeouts establecidos " << std::endl;

        //-- Wait for the data
        ret = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
        //-- If there are data waiting: read it
        if (ret == 1)
        {
            //-- Read the data (n bytes)
            n = read(serial_fd, &data[count], 1);
            // read viene de sofa/src/applications/plugins/SofaPML/argumentParser.h
            std::cout << "Entrando al metodo read para leer el serial " << std::endl;

            if (band)
            {
                if (data[count] != ' ')
                {
                    if (data[count] == '\n')
                    {
                        data[count] = '\0';
                        return count;
                    }
                    //-- The number of bytes receives is increased in n
                    count += n;
                }
                std::cout << "Numero de bytes recibidos " << count << std::endl;
            }
            if (!band && data[count] == '\n')
            {
                band = true;
            }
            //-- The last byte is always a 0 (for printing the string data)
            data[count] = 0;
        }
        std::cout << "Valores de band " << band << " y data " << data << std::endl;

        //-- Repeat the loop until a data block of size bytes is received or
        //-- a timeout occurs
    } while (count < size && ret == 1);

    //-- Return the number of bytes reads. 0 If a timeout has occurred.
    std::cout << "Saliendo del metodo serial_read " << std::endl;
    std::cout << "Este es count " << count << std::endl;
    return count;
    
}

void serial_close(int fd);

int SerialDriverClass = sofa::core::RegisterObject("This component does nothing.").add<SerialDriver>();

SOFA_DECL_CLASS(SerialDriver);

} // namespace controller

} // namespace component

} // namespace sofa