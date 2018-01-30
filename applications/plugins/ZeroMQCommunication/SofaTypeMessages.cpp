#include <stdio.h>
#include <stdlib.h>
#include "SofaTypeMessages.h"
#include <string.h>
#include <cstring>

#include <zmq.hpp>
using std::cout;
using std::endl;

namespace sofa
{

namespace component
{

namespace controller
{

SofaTypeMessages::SofaTypeMessages()
{
}

//void SofaTypeMessages::instrumentData(Vec3d pos, Quat quat, int btnState, float openInst, bool blnDataReady)
/*
void SofaTypeMessages::instrumentDataSend(instrumentData a)
{
    // zmq::context_t context(1);
    // zmq::socket_t socket(context, ZMQ_REQ);
    cout << "The data are: " << a.btnState;
    cout << "The data are: " << a.openInst;
    cout << "The data are: " << a.quat;
    //static const void* d = (void*)a.btnState;
    // zmq::message_t request(33);

    //itoa(a.btnState);
    
    //string btnState = std::to_string(a.btnState);
    // const int size = 10;
    //string name = a.nameInstrument;

    // memcpy(request.data(), &a.btnState, 33);
    // socket.send(request);

    //a.btn
    //instrumentData g;
    //a= g.pos
}
*/

}

}

}