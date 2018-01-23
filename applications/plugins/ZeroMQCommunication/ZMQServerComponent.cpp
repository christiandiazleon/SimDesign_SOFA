#include <sofa/core/ObjectFactory.h>
#include <zmq.hpp>
#include <iostream>
#include <string>
#include "ZMQServerComponent.h"
#include "SofaTypeMessages.h"

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

void ZMQServerComponent::sendGreetings()
{
    gettimeofday(&t_before, NULL);
    zmq::message_t request(33);
    memcpy(request.data(), "Hello Python Server, how are you?", 33);
    socket.send(request);
}

void ZMQServerComponent::getResponseFromServer()
{
    string replyMessage;
    zmq::message_t reply;
    socket.recv(&reply);

    replyMessage = string(static_cast<char *>(reply.data()), reply.size());
    cout << "Getting response from server: " + replyMessage << " " << endl;

    gettimeofday(&t_after, NULL);

    printf("Time in microseconds: %ld microseconds\n",
           ((t_after.tv_sec - t_before.tv_sec) * 1000000L + t_before.tv_usec) - t_before.tv_usec);
}

/*
void ZMQServerComponent::receiveRequests()
{
    cout << "Creating socket zeroMQ ..." << endl;
    socket.bind("tcp://*:5555");
    while (true)
    {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv(&request);
        string incomingMessage = std::string(static_cast<char *>(request.data()), request.size());
        cout << "Incoming message from client: " << incomingMessage << endl;

        // Do some work
        sleep(1);
        // int x=0.1;
        float f = 0.1;
        string f_str = std::to_string(f);
        cout << "cout: " << f << '\n'
                  << "to_string: " << f_str << "\n\n";

        string message = "Hello Client, my version is: " + f_str;
        zmq::message_t reply(message.length());
        // memcpy (reply.data(), message.c_str(), replys);
        memcpy(reply.data(), message.c_str(), message.length());
        // cout << "myparam value is:" << myparam.getValue() << endl;
        // cout << "The addres value is:" << d_address.getValue() << endl;
        //memcpy (reply.data(), "Hello Client", myparam.getValue());
        socket.send(reply);
    }
}

void ZMQServerComponent::sendReplyToClient() {
    // std::string s=myparam.getValue()
    std::string messageStr="Hello Client, my version is: ";
    zmq::message_t reply (messageStr.length());
    //memcpy (reply.data(), "H", myparam.getValue());
    memcpy (reply.data(), messageStr.c_str(), myparam.getValue());
    cout << "myparam value is:" << myparam.getValue() << endl;
    cout << "The addres value is:" << d_address.getValue() << endl;
    //memcpy (reply.data(), "Hello Client", myparam.getValue());
    socket.send(reply);
}*/

void ZMQServerComponent::init()
{
    std::cout << "ZeroMQCommunication::init()" << std::endl;
    ZMQServerComponent z;
    z.setupConnection();
    z.sendGreetings();
    z.getResponseFromServer();


    //z.receiveRequests();
    //.sendReplyToClient();
    z.draw();
}

ZMQServerComponent::~ZMQServerComponent()
{
}


void ZMQServerComponent::draw(const core::visual::VisualParams *vparam)
{
    draw();
}

void ZMQServerComponent::draw()
{   
    
    instrumentData itemp;
    itemp.pos = sofa::defaulttype::Vec3d(1.0f, 1.0f, 1.0f);
    itemp.quat = defaulttype::Quat(1.0f, 1.0f, 1.0f, 1.0f);
    itemp.btnState = 5671;
    itemp.openInst = 1.0f;
    itemp.blnDataReady = true;

    SofaTypeMessages a = SofaTypeMessages();
    a.instrumentDataSend(itemp);
    
}


// int ZeroMqComponentClass = sofa::core::RegisterObject("This component does nothing.").add<ZeroMqComponent>();
SOFA_DECL_CLASS(ZMQServerComponent)

int ZMQServerComponentClass = sofa::core::RegisterObject("This component create a Socket.").add< ZMQServerComponent >();
} // namespace controller

} // namespace component

} // namespace sofa
