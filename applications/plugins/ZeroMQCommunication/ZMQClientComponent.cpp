#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/Node.h>
#include <zmq.hpp>
#include <zmq.h>
#include "zhelpers.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <stdint.h>
#include <random>
#include "ZMQClientComponent.h"
//#include "SofaTypeMessages.h"

using namespace std;

namespace sofa
{

namespace component
{

namespace controller
{

// Create a context to use in all sockets creation
zmq::context_t context(1);

/** Socket to subscribe this SOFA client to Network Manager server.
 * Although in first instance this SOFA client will be have a sender behavior
 * and not will receive messages
 **/
zmq::socket_t subscriber(context, ZMQ_SUB);


/**
 * Socket to send message, which will be taked by Network Manager and delivered to
 * all peer clients connected
 **/
zmq::socket_t sender(context, ZMQ_PUSH);

ZMQClientComponent::ZMQClientComponent()
// : myparam(initData(&myparam, 0.42, "myparam", "ZeroMq version plugin. "))
// : myparam(initData(&myparam, (double)(0.42), "myparam", "ZeroMq version plugin. "))
// , d_address(initData(&d_address, (std::string)"127.0.0.1", "address", "Scale for object display. (default=localhost)"))
{
    
}

void ZMQClientComponent::setupConnection()
{
    /**publisherEndpoint is the endpoint through the clients will be connect to
     * Network Manager and will receive data driven by SOFA events
     */
    const string publisherEndpoint = "tcp://localhost:5557";

    /**pushEndpoint is the enpoint through clients send SOFA data events to
     * Network Manager and this in turn redirects them to all connected clients
     * via publisherEndpoint socket  
    */
    const string pushEndpoint = "tcp://localhost:5558";

    /**
     * Connecting to publisherEndpoint and pushEndpoint
    */
    cout << "Connecting to ZMQ Network Manager   " << publisherEndpoint << "..." << endl;
    subscriber.connect("tcp://localhost:5557");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    
    /**
     * Connecting to publisherEndpoint and pushEndpoint
    */
    sender.connect("tcp://localhost:5558");

    // Set the identity http://api.zeromq.org/4-2:zmq-setsockopt
    //client.setsockopt(ZMQ_IDENTITY, "PEER1", 5);    
    
}

void ZMQClientComponent::instrumentDataSend(instrumentData a)
{   
    std::cout << "enviando datos de instrument: " << endl;

    a.pos = sofa::defaulttype::Vec3d(2.0f, 1.0f, 1.0f);
    a.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    a.btnState = 45;
    a.openInst = 1.0f;
    a.blnDataReady = false;

    /* Strings to store instrumentData members */
    string posVector0, posVector1, posVector2, allPosVector, 
        quatQuat0, quatQuat1, quatQuat2, quatQuat3, allQuatQuat,
        btnStateStr, openInstStr, blnDataReadyStr, allInstrumentData;

    // ******* Turn to strings Vec3d pos contents*********
    posVector0 = to_string(a.pos[0]);
    posVector1 = to_string(a.pos[1]);
    posVector2 = to_string(a.pos[2]);

    // ******* Turn to strings Quat quat contents*********
    quatQuat0 = to_string(a.quat[0]);
    quatQuat1 = to_string(a.quat[1]);
    quatQuat2 = to_string(a.quat[2]);
    quatQuat3 = to_string(a.quat[3]);

    /* Other instrumentData */
    btnStateStr = " btsState value : " + to_string(a.btnState);
    openInstStr = " openInst value : " + to_string(a.openInst);
    blnDataReadyStr = " blnDataReady value : " +  to_string(a.blnDataReady);

    /* Concatenating all posVector elements */
    allPosVector = " -> posVector3D elements: " + posVector0 + " " + posVector1 + " " + posVector2 + " | ";

    /* Concatenating all quatQuat elements */
    allQuatQuat = " -> quatQuat elements: " + quatQuat0 + " " + quatQuat1 + " " + quatQuat2 + " " + quatQuat3 + " | ";

    /* Grouping instrumentDataStr */
    allInstrumentData = allPosVector + " " + allQuatQuat + " " + btnStateStr + " " + openInstStr + " " 
    + blnDataReadyStr;

    // sleep(1);
    s_send(sender, allInstrumentData);

    /**
     * Send message from ZMQ_DEALER
     *ZMQ_SNDMORE is multipart message. --- 
    */
    //zmq::message_t message;
    //client.send(message);
    //s_sendmore(client, "");
    
    //  Wait for next request from client
    
    /*
    while(1){
        // std::string string = s_recv(subscriber);

        // std::cout << "Received request: " << string << std::endl;

        // Do some 'work'
        sleep(1);
        s_send(sender, allInstrumentData);
    }
    */
    
}


void ZMQClientComponent::attachingDataToSend(attachingData b)
{

    string vIdTrianglesStr0;

    std::cout << "enviando datos de attaching: " << endl;

    b.vIdTriangles = {888, 2, 3, 4};
    int total = 0;
    total = b.vIdTriangles[0] + b.vIdTriangles[1] + b.vIdTriangles[2] + b.vIdTriangles[3];
    // cout << "The first element of vIdTriangles is: " << b.vIdTriangles[0] << endl;
    // cout << "The total of elements is: " << total << endl;
    vIdTrianglesStr0 = to_string(b.vIdTriangles[0]);

    
    // Sending data with ZMQ 
    
    //zmq::message_t request(vIdTrianglesStr0.size());
    
    // tener en cuenta esto http://zguide.zeromq.org/page:all#A-Minor-Note-on-Strings

    //std::copy_n(vIdTrianglesStr0.c_str(), vIdTrianglesStr0.size() + 1, reinterpret_cast<char *>(request.data()));
    //socket.send(request);
    
    s_send(sender, vIdTrianglesStr0);
    
    
}

void ZMQClientComponent::init()
{

    /* We get the rootContext */
    sofa::simulation::Node::SPtr rootContext = static_cast<simulation::Node *>(this->getContext()->getRootContext());
    cout << "rootContext: " << rootContext << endl;

    //este era
    getContext()->get<SerialDriverType>(&objectsSerialDriver, core::objectmodel::BaseContext::SearchRoot);

    //rootContext->getTreeObjects<SerialDriverType>(&objectsSerialDriver);

    std::cout << "Hola este es el tamaño de mi vector Serial Driver:" << objectsSerialDriver.size() << " \n\n"
              << endl;
    std::cout << "Este es el elemento 0 de mi vector Serial Driver: " << objectsSerialDriver[0]->name.getValue() << " \n\n"
              << endl;

    //SerialDriver* s = new SerialDriver();

    std::cout << "ZeroMQCommunication::init()" << std::endl;
    ZMQClientComponent z;
    
    // Connecting to Nerwork Manager
    z.setupConnection();

    // float a = s->askDevice();
    // std::cout << "datos del hapkit" << a << std::endl;
    
    

    // Creating instrument data object
    // instrumentData itemp;
    
    // Creating attaching data object
    // attachingData n;
    
    

    /*
    ***** Envio de datos de instrumento y de attaching usando las estructuras***
    while (1)
    {
        // Sending data objects structure members
        z.instrumentDataSend(itemp);
        z.attachingDataToSend(n);
        // z.getResponseFromServer();
        // z.draw();
    }
    */
}

/*ZMQClientComponent::~ZMQClientComponent()
{
}*/



void ZMQClientComponent::draw(const core::visual::VisualParams *vparam)
{
    std::cout
        << "Draw del ZMQ" << std::endl;

    float a = s->askDevice();
    std::cout << "datos del hapkit" << a << std::endl;
}

void ZMQClientComponent::draw()
{
    std::cout
        << "Draw del ZMQ" << std::endl;

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
SOFA_DECL_CLASS(ZMQClientComponent)

int ZMQClientComponentClass = sofa::core::RegisterObject("This component connect to ZMQ Socket.").add< ZMQClientComponent >();
} // namespace controller

} // namespace component

} // namespace sofa
