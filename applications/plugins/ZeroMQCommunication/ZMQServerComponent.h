#include <sofa/core/behavior/BaseController.h>

#include <zmq.hpp>

// #include <sofa/core/objectmodel/Data.h>
// using sofa::core::objectmodel::Data;

#include <sofa/defaulttype/VecTypes.h>
#include "SofaTypeMessages.h"

// To Quat datatype
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
    // Data<double> myparam;
    Data<std::string> d_address;
    typedef sofa::defaulttype::Vec3d Vec3d;
    typedef defaulttype::Quat Quat;

    ZMQServerComponent();
    virtual ~ZMQServerComponent();
    void receiveRequests();
    void sendReplyToClient();
    void init();

    virtual void draw();
    virtual void draw(const core::visual::VisualParams *) override;
};


}
}
}
