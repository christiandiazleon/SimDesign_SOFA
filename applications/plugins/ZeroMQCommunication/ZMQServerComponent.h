#include <sofa/core/behavior/BaseController.h>
#include "PointNet.h"
#include <zmq.hpp>

#include <sofa/core/objectmodel/Data.h>
using sofa::core::objectmodel::Data;

#include <sofa/defaulttype/VecTypes.h>
// using sofa::component::engine::DistanceZoneImageToolBoxNoTemplated::Vec3d

#include <sofa/defaulttype/Quat.h>
using sofa::defaulttype::Quat;
using std::string;

namespace sofa
{

namespace component
{

namespace controller
{

class ZMQServerComponent : public sofa::core::behavior::BaseController
{

  public:
    SOFA_CLASS(ZMQServerComponent, sofa::core::behavior::BaseController);
    Data<double> myparam;
    Data<std::string> d_address;
    typedef sofa::defaulttype::Vec3d Vec3d;
    typedef defaulttype::Quat Quat;

    ZMQServerComponent();
    virtual ~ZMQServerComponent();
    void receiveRequests();
    void sendReplyToClient();
    void init();
};

struct instrumentData
{
    typedef sofa::defaulttype::Vec3d Vec3d;
    Vec3d pos;
    Quat quat;
    int btnState;
    float openInst;
    bool blnDataReady;
};

struct attachingData
{
    std::vector<PointNet> vPointsAttach;
    std::vector<int> vIdTriangles;
    std::vector<int> vIdInstTriangles;
    string strMechanicalModel;
    string strSurfaceModel;
    int nContactPoints;
    bool blnAttachContact;
    bool blnDeAtacchContact;
    float fDist;
};

struct probingData
{
    int nContactPoints;
    std::vector<PointNet> vPointsProbing;
    std::vector<PointNet> vNormalsProbing;
    std::vector<int> ID;
    string strCollisionModel1;
    string strCollisionModel2;
    std::vector<int> vIdTriangles1;
    std::vector<int> vIdTriangles2;
    bool blnProbeContact;
};

struct attachClipData1
{
    std::vector<PointNet> vPointsAttach;
    std::vector<int> vIdTriangles;
    string strMechanicalModel;
    string strSurfaceModel;
    int nContactPoints;
    bool blnAttachClip;
    float fDist;
};

struct attachClipData2
{
    sofa::defaulttype::Vector3 positionClip;
    sofa::defaulttype::Quat orientationClip;
    bool blnDataReady;
};

struct cuttingData
{
    int method;
    string strBody1;
    int indexColElement1;
    PointNet pnt1;
    string strBody2;
    int indexColElement2;
    PointNet pnt2;
    string strBodyL;
    int indexColElementL;
    PointNet pntL;
    bool firstCollision;
    bool secondCollision;
    bool cut;
};

struct carvingData
{
    int NroCarvingPrimitives;
    string strBody;
    helper::vector<int> elemtsToRemove;
    bool IsCarve;
};
}
}
}
