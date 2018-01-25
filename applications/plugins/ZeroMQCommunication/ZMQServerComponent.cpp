#include <sofa/core/ObjectFactory.h>
#include <zmq.hpp>
#include <iostream>
#include <string>
#include "ZMQServerComponent.h"
//#include "SofaTypeMessages.h"

using namespace std;

namespace sofa
{

namespace component
{

namespace controller
{


/* variables para ZMQ Internal Server */
// zmq::context_t context(1);
// zmq::socket_t socket(context, ZMQ_REP);

/* variables para ZMQ Internal Client */
zmq::context_t context(1);
zmq::socket_t socket(context, ZMQ_REQ);



ZMQServerComponent::ZMQServerComponent()
// : myparam(initData(&myparam, 0.42, "myparam", "ZeroMq version plugin. "))
// : myparam(initData(&myparam, (double)(0.42), "myparam", "ZeroMq version plugin. "))
// , d_address(initData(&d_address, (std::string)"127.0.0.1", "address", "Scale for object display. (default=localhost)"))
{
    
}


void ZMQServerComponent::setupConnection()
{
    cout << "Connecting to python zeroMQ server ..." << endl;
    socket.connect("tcp://localhost:5555");
}

void ZMQServerComponent::instrumentDataSend(instrumentData a)
{
    
    a.pos = sofa::defaulttype::Vec3d(1.0f, 1.0f, 1.0f);
    a.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    a.btnState = 5671;
    a.openInst = 1.0f;
    a.blnDataReady = false;
    /// a.name = '';
    
    gettimeofday(&t_before, NULL);
    zmq::message_t request(10);
    cout << "The data are: " << a.blnDataReady;

    memcpy(request.data(), &a.blnDataReady, 10);
    socket.send(request);
}


void ZMQServerComponent::getResponseFromServer()
{
    
    zmq::message_t reply;
    socket.recv(&reply);

    replyMessage = string(static_cast<char *>(reply.data()), reply.size());
    cout << "Getting response from server: " + replyMessage << " " << endl;

    gettimeofday(&t_after, NULL);

    printf("Time in microseconds: %ld microseconds\n",
           ((t_after.tv_sec - t_before.tv_sec) * 1000000L + t_before.tv_usec) - t_before.tv_usec);
}

void ZMQServerComponent::init()
{
    std::cout << "ZeroMQCommunication::init()" << std::endl;
    ZMQServerComponent z;
    z.setupConnection();

    instrumentData itemp;
    z.instrumentDataSend(itemp);
    z.getResponseFromServer();
    z.draw();
    
}

ZMQServerComponent::~ZMQServerComponent()
{
}


void ZMQServerComponent::draw(const core::visual::VisualParams *vparam)
{
    //draw();
    /*De alguna forma este metodo genera como un ciclo o muestra varais
    veces la salida del DRAW */
}

void ZMQServerComponent::draw()
{   
    
    instrumentData itemp;
    itemp.pos = sofa::defaulttype::Vec3d(1.0f, 1.0f, 1.0f);
    itemp.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    itemp.btnState = 5671;
    itemp.openInst = 1.0f;
    itemp.blnDataReady = true;
    
    cout << itemp.btnState;

    // SofaTypeMessages a;
    // a.instrumentDataSend(itemp);
    
    
}


// int ZeroMqComponentClass = sofa::core::RegisterObject("This component does nothing.").add<ZeroMqComponent>();
SOFA_DECL_CLASS(ZMQServerComponent)

int ZMQServerComponentClass = sofa::core::RegisterObject("This component create a Socket.").add< ZMQServerComponent >();
} // namespace controller

} // namespace component

} // namespace sofa
