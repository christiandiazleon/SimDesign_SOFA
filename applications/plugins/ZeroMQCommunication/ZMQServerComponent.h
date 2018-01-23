#include <sofa/core/behavior/BaseController.h>
#include <zmq.hpp>
#include <sys/time.h>

// #include <sofa/core/objectmodel/Data.h>
// using sofa::core::objectmodel::Data;

#include <sofa/defaulttype/VecTypes.h>
// #include "SofaTypeMessages.h"

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
    
    /* Conect to ZMQ external Server  */
    void setupConnection();

    /* Send some data to ZMQ external Server  */
    void sendGreetings();

    /* Get some response from ZMQ external Server */
    void getResponseFromServer();

    /* Initialize ZMQ Internal Server and receive some data */
    // void receiveRequests();
    // void sendReplyToClient();
    void init();

    virtual void draw();
    virtual void draw(const core::visual::VisualParams *) override;

  private:
    struct timeval t_before, t_after;
    string replyMessage;
};


}
}
}
