#include <sofa/core/ObjectFactory.h>
#include <zmq.hpp>
#include <zmq.h>
#include <algorithm>
#include <iterator>
// #include <zmqpp.hpp>
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

void ZMQServerComponent::setupConnectionAttachingData()
{
    const string endpoint2 = "tcp://localhost:5556";
    cout << "Connecting to python zeroMQ server" << endpoint2 << "..." << endl;
    socket.connect(endpoint2);
}

void ZMQServerComponent::instrumentDataSend(instrumentData a)
{
    
    a.pos = sofa::defaulttype::Vec3d(2.0f, 1.0f, 1.0f);
    a.quat = defaulttype::Quat(1.0f, 1.0f, 4.0f, 1.0f);
    a.btnState = 45;
    a.openInst = 1.0f;
    a.blnDataReady = false;

    // ***************** btnState ***********************
    
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
    btnStateStr = to_string(a.btnState);
    openInstStr = to_string(a.openInst);
    blnDataReadyStr = to_string(a.blnDataReady);

    /* Concatenating all posVector elements */
    allPosVector = posVector0 + " " + posVector1 + " " + posVector2 + " -> posVector3D elements";

    /* Concatenating all quatQuat elements */
    allQuatQuat = quatQuat0 + " " + quatQuat1 + " " + quatQuat2 + " " + quatQuat3 + " -> quatQuat elements";

    /* Grouping instrumentDataStr */
    allInstrumentData = allPosVector + " " + allQuatQuat + " " + btnStateStr + " " + openInstStr + " " + blnDataReadyStr;
    cout << "instrumentData members sent \n"
         << "Variable\tValue\n\n";
    cout << "posVector 0" << "\t" << a.pos[0] <<endl;
    cout << "posVector 1" << "\t" << a.pos[1] <<endl;
    cout << "posVector 2" << "\t" << a.pos[2] <<endl;
    cout << endl;
    cout << "quatQuat 0" << "\t" << a.quat[0] << endl;
    cout << "quatQuat 1" << "\t" << a.quat[1] << endl;
    cout << "quatQuat 2" << "\t" << a.quat[2] << endl;
    cout << "quatQuat 3" << "\t" << a.quat[3] << endl;
    cout << endl;
    cout << "btnState" << "\t" << a.btnState << endl << endl;
    cout << "openInst" << "\t" << a.openInst <<endl << endl;
    cout << "blnDataReady"<< "\t" << a.blnDataReady << endl;

    // gettimeofday(&t_before, NULL);
    
    zmq::message_t request(allInstrumentData.size()+1);
    cout << "btnState is : " << a.btnState << endl;
    cout << "blnDataReady is : " << a.blnDataReady << endl;

    // ***************** btnState ***********************
    //memcpy(request.data(), result.c_str(), result.size() + 1);
    //std::copy_n(reinterpret_cast<char *>(request.data()), result.size(), result.c_str());
    std::copy_n(allInstrumentData.c_str(), allInstrumentData.size()+1, reinterpret_cast<char *>(request.data()));
    //std::copy_n(result.c_str(), result.size() + 1, request.data());
    socket.send(request);
    
}


void ZMQServerComponent::attachingDataToSend(attachingData b)
{
    string vIdTrianglesStr0;
    
    b.vIdTriangles = {1, 2, 3, 4};
    int total = 0;
    total = b.vIdTriangles[0] + b.vIdTriangles[1] + b.vIdTriangles[2] + b.vIdTriangles[3];
    cout << "The first element of vIdTriangles is: " << b.vIdTriangles[3] << endl;
    cout << "The total of elements is: " << total << endl;
    vIdTrianglesStr0 = to_string(b.vIdTriangles[0]);

    

    /* Sending data with ZMQ */
    zmq::message_t request(vIdTrianglesStr0.size());
    // tener en cuenta esto http://zguide.zeromq.org/page:all#A-Minor-Note-on-Strings

    std::copy_n(vIdTrianglesStr0.c_str(), vIdTrianglesStr0.size() + 1, reinterpret_cast<char *>(request.data()));
    socket.send(request);
    
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

    
    //z.setupConnectionAttachingData();
    
    /**
     * Si enviamos datos de una segunda estructura en la misma conexion
     * el cliente crashea dado que es un soxket ZMQ_REQ entonces es sincronico,
     * y debe esperar una confirmacion de lo primero enviado para que envie datos
     * de la segunda estructura. Toca cambiar de modelo de Socket.
     * 
    */
    //attachingData n;
    // z.attachingDataToSend(n);
    
    // Aqui obtenemos la respuesta del servidor mas precisamente el socket ZMQ_REP .. 
    // Es como un ACK de los datos. No lo necesitamos
    //z.getResponseFromServer();

    //Un ejemplo para un evento con objetos SOFA
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
