/******************************************************************/
/* serial.h    Andrés Sánchez.    November, 2017          */
/*----------------------------------------------------------------*/
/* Serial communications in SOFA                                 */
/*----------------------------------------------------------------*/
/* GPL LICENSE                                                    */
/******************************************************************/

//Sensable include

#include <sofa/helper/LCPcalc.h>
#include <sofa/defaulttype/SolidTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/Quater.h>

#include <sofa/core/behavior/BaseController.h>
#include <SofaOpenglVisual/OglModel.h>
#include <SofaRigid/RigidMapping.h>
#include <SofaUserInteraction/Controller.h>
#include <sofa/simulation/Node.h>
#include <sofa/simulation/Simulation.h>
#include <SofaBaseMechanics/MechanicalObject.h>

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/visual/VisualParams.h>

//force feedback
#include <SofaHaptics/ForceFeedback.h>
#include <SofaHaptics/MechanicalStateForceFeedback.h>
#include <SofaHaptics/LCPForceFeedback.h>
#include <SofaHaptics/NullForceFeedbackT.h>

#include <SofaSimulationTree/GNode.h>
#include <SofaBaseTopology/TopologyData.h>
#include <SofaBaseVisual/InteractiveCamera.h>

//Serial Communication
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <thread>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>
#include <math.h>

namespace sofa
{
namespace simulation { class Node; }

namespace component
{
namespace visualModel { class OglModel; }

namespace controller
{

using namespace sofa::defaulttype;
using namespace sofa::helper;
using core::objectmodel::Data;


struct DeviceData
{
    int nupdates;
    int m_buttonState;
    Vec3d pos;
    Quat quat;
    bool ready;
    bool stop;
};

struct SerialData
{
    ForceFeedback::SPtr forceFeedback;
    simulation::Node::SPtr *context;

    sofa::defaulttype::SolidTypes<double>::Transform endOmni_H_virtualTool;
    //Transform baseOmni_H_endOmni;
    sofa::defaulttype::SolidTypes<double>::Transform world_H_baseOmni;
    double forceScale;
    double scale;
    bool permanent_feedback;

    // API OMNI //
    DeviceData servoDeviceData;  // for the haptic loop
    DeviceData deviceData;        // for the simulation loop

    double currentForce[3];
};

struct AllSerialData
{
    sofa::helper::vector<SerialData> serialData;
};

class SerialDriver : public sofa::core::objectmodel::BaseObject
{

public:
	SOFA_CLASS(SerialDriver, sofa::core::objectmodel::BaseObject);
    typedef RigidTypes::Coord Coord;
    typedef RigidTypes::VecCoord VecCoord;
    typedef Vec3dTypes::VecCoord VecCoord2;
    typedef component::container::MechanicalObject<sofa::defaulttype::Rigid3dTypes> MMechanicalObject;

    struct VisualComponent
    {
        simulation::Node::SPtr node;
        simulation::Node::SPtr nodeColis;
        sofa::component::visualmodel::OglModel::SPtr visu;
        sofa::component::mapping::RigidMapping< Rigid3dTypes , ExtVec3fTypes  >::SPtr mapping;
        sofa::component::mapping::RigidMapping< Rigid3dTypes , Vec3dTypes  >::SPtr mappingColis;
    };

    Data<double> forceScale;
    Data<double> scale;
    Data<Vec3d> positionBase;
    Data<Quat> orientationBase;
    Data<Vec3d> positionTool;
    Data<Quat> orientationTool;
    Data<bool> permanent;
    Data<bool> omniVisu;
    Data< VecCoord > posDevice;
    Data< VecCoord > posStylus;
    Data< std::string > locDOF;
    Data< std::string > deviceName;
    Data< int > deviceIndex;
    Data<Vec1d> openTool;
    Data<double> maxTool;
    Data<double> minTool;
    Data<double> openSpeedTool;
    Data<double> closeSpeedTool;
    Data<bool> useScheduler;
    Data<bool> setRestShape;
    Data<bool> applyMappings;
    Data<bool> alignOmniWithCamera;

    sofa::component::container::MechanicalObject<sofa::defaulttype::Rigid3dTypes> *DOFs;
    sofa::component::visualmodel::BaseCamera::SPtr camera;

    bool initVisu;

    SerialData data_s;
    AllSerialData allData;


	SerialDriver();
    virtual ~SerialDriver ();

    virtual void init();
    virtual void bwdInit();
    virtual void reset();
    void reinit();

    int initDevice();

    void cleanup();
    virtual void draw(const core::visual::VisualParams*) override;
    virtual void draw();
    int askDevice();

    void setForceFeedback(ForceFeedback* ff);

    void onKeyPressedEvent(core::objectmodel::KeypressedEvent *);
    void onKeyReleasedEvent(core::objectmodel::KeyreleasedEvent *);
    void onAnimateBeginEvent();

    void setDataValue();

    enum
    {
        NVISUALNODE = 1
    };
    VisualComponent visualNode[NVISUALNODE];
    static const char* visualNodeNames[NVISUALNODE];
    static const char* visualNodeFiles[NVISUALNODE];
    simulation::Node::SPtr nodePrincipal;
    typedef sofa::component::container::MechanicalObject<RigidTypes> MechanicalObjectType;
    std::vector<MechanicalObjectType*> objectsMechTemp;
    typedef sofa::component::container::MechanicalObject<Vec3dTypes> MechanicalObjectType2;
    std::vector<MechanicalObjectType2*> collisionRigid;
    MMechanicalObject::SPtr rigidDOF;
    MMechanicalObject::SPtr rigiDOFEST;
    bool changeScale;
    bool firstInit;
    float oldScale;
    bool visuActif;
    bool isInitialized;
    Vec3d positionBase_buf;
    bool modX;
    bool modY;
    bool modZ;
    bool modS;
    bool axesActif;
    //HDfloat angle1[3];
    //HDfloat angle2[3];
    bool firstDevice;
    //vector<NewOmniDriver*> autreOmniDriver;

private:
    float posDOFEST;
    void handleEvent(core::objectmodel::Event *);
    bool noDevice;

    //Serial Functions
    int  serial_open(char *serial_name, speed_t baud);
	void serial_send(int serial_fd, char *data, int size);
	int  serial_read(int serial_fd, char *data, int size, int timeout_usec);
	void serial_close(int fd);
};
}
}
}
