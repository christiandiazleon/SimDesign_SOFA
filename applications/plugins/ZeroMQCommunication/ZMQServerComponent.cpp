#include <sofa/core/ObjectFactory.h>
#include <zmq.hpp>
#include <zmq.h>
#include <algorithm>
#include <iterator>
#include <zmqpp.hpp>
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

/* variables para ZMQ Internal Client estas son */
zmq::context_t context(1);
zmq::socket_t socket(context, ZMQ_REQ);

//  initialize the 0MQ context
//zmqpp::context context;

// generate a push socket
//zmqpp::socket_type type = zmqpp::socket_type::push;
//zmqpp::socket socket(context, type);




ZMQServerComponent::ZMQServerComponent()
// : myparam(initData(&myparam, 0.42, "myparam", "ZeroMq version plugin. "))
// : myparam(initData(&myparam, (double)(0.42), "myparam", "ZeroMq version plugin. "))
// , d_address(initData(&d_address, (std::string)"127.0.0.1", "address", "Scale for object display. (default=localhost)"))
{
    
}


void ZMQServerComponent::setupConnection()
{
    const string endpoint = "tcp://localhost:5555";
    cout << "Connecting to python zeroMQ server" << endpoint << "..." << endl;
    socket.connect(endpoint);
}

void ZMQServerComponent::instrumentDataSend(instrumentData a)
{
    
    a.pos = sofa::defaulttype::Vec3d(2.0f, 1.0f, 1.0f);
    a.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    a.btnState = 45;
    a.openInst = 1.0f;
    a.blnDataReady = false;

    // ***************** btnState ***********************
    
    string s, test, result;
    string btnState, allData, posVector0, quatQuat2;
    posVector0 = to_string(a.pos[0]);
    quatQuat2 = to_string(a.quat[2]);
    cout << posVector0;

    btnState = to_string(a.btnState);
    s  = to_string(a.openInst);
    
    test = " is a number";
    result = s + test;
    
    
    //zmqpp::message message;
    //message << "Hello World!" << 42;
    
    // gettimeofday(&t_before, NULL);
    zmq::message_t request(quatQuat2.size() + 1);
    cout << "btnState is : " << a.btnState << endl;
    // cout << "btnState concatenate is: " << result << endl;
    // cout << "btnState concatenate with & is: " << &result << endl;
    
    // ***************** btnState ***********************
    //memcpy(request.data(), posVector0.c_str(), posVector0.size() + 1);
    //memcpy(request.data(), result.c_str(), result.size() + 1);
    //std::copy_n(reinterpret_cast<char *>(request.data()), result.size(), result.c_str());
    std::copy_n(quatQuat2.c_str(), quatQuat2.size() + 1, reinterpret_cast<char *>(request.data()));
    //std::copy_n(result.c_str(), result.size() + 1, request.data());
    socket.send(request);
    //socket.send(message);
    // cout << "String btnState " << result << " sent to ZMQ Server" << endl;
}
/*
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
*/
void ZMQServerComponent::init()
{
    std::cout << "ZeroMQCommunication::init()" << std::endl;
    ZMQServerComponent z;
    z.setupConnection();

    instrumentData itemp;
    z.instrumentDataSend(itemp);
    //z.getResponseFromServer();
    //z.draw();
    
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
