#include <sofa/core/ObjectFactory.h>
#include "zhelpers.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include "ZMQClientComponent.h"
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
zmq::socket_t client(context, ZMQ_DEALER);



ZMQClientComponent::ZMQClientComponent()
// : myparam(initData(&myparam, 0.42, "myparam", "ZeroMq version plugin. "))
// : myparam(initData(&myparam, (double)(0.42), "myparam", "ZeroMq version plugin. "))
// , d_address(initData(&d_address, (std::string)"127.0.0.1", "address", "Scale for object display. (default=localhost)"))
{
    
}

void ZMQClientComponent::setupConnection()
{
    const string endpoint = "tcp://localhost:5555";
    cout << "Connecting to ZMQ Network Manager " << endpoint << "..." << endl;
    client.connect(endpoint);
}

void ZMQClientComponent::instrumentDataSend(instrumentData a)
{   
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

    /**
     * Send message from ZMQ_DEALER
     *ZMQ_SNDMORE is multipart message. --- 
    */
    zmq::message_t message;
    client.send(message);
    
    
}


void ZMQClientComponent::attachingDataToSend(attachingData b)
{
    string vIdTrianglesStr0;
    
    b.vIdTriangles = {888, 2, 3, 4};
    int total = 0;
    total = b.vIdTriangles[0] + b.vIdTriangles[1] + b.vIdTriangles[2] + b.vIdTriangles[3];
    cout << "The first element of vIdTriangles is: " << b.vIdTriangles[0] << endl;
    cout << "The total of elements is: " << total << endl;
    vIdTrianglesStr0 = to_string(b.vIdTriangles[0]);

    
    // Sending data with ZMQ 
    
    //zmq::message_t request(vIdTrianglesStr0.size());
    
    // tener en cuenta esto http://zguide.zeromq.org/page:all#A-Minor-Note-on-Strings

    //std::copy_n(vIdTrianglesStr0.c_str(), vIdTrianglesStr0.size() + 1, reinterpret_cast<char *>(request.data()));
    //socket.send(request);
    
    //s_send(client, vIdTrianglesStr0);
    
    /*
    string response = s_recv(requester);
    cout << "Received reply " << request
            << " [" << response << "]" << endl;
    */
    
}

void ZMQClientComponent::init()
{
    std::cout << "ZeroMQCommunication::init()" << std::endl;
    ZMQClientComponent z;
    
    // Connecting to Nerwork Manager
    z.setupConnection();
    
    // Send instrument data structure members
    instrumentData itemp;
    //z.instrumentDataSend(itemp);


    // Send attaching data
    attachingData n;
    z.attachingDataToSend(n);
    //z.getResponseFromServer();
    //z.draw();
    
}

ZMQClientComponent::~ZMQClientComponent()
{
}


void ZMQClientComponent::draw(const core::visual::VisualParams *vparam)
{
    //draw();
    /*De alguna forma este metodo genera como un ciclo o muestra varais
    veces la salida del DRAW */
}

void ZMQClientComponent::draw()
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
SOFA_DECL_CLASS(ZMQClientComponent)

int ZMQClientComponentClass = sofa::core::RegisterObject("This component create a Socket.").add< ZMQClientComponent >();
} // namespace controller

} // namespace component

} // namespace sofa
