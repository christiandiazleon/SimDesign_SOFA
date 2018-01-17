#include "SofaTypeMessages.h"

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
void SofaTypeMessages::instrumentDataSend(instrumentData a)
{
    cout << "The data are: " << a.btnState;

    //a.btn
    //instrumentData g;
    //a= g.pos
}

}

}

}