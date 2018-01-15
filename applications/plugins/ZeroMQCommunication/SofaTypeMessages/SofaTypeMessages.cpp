#include "SofaTypeMessages.h"

//#include <zmq.hpp>
using std::cout;
using std::endl;

SofaTypeMessages::SofaTypeMessages()
{
}

void SofaTypeMessages::instrumentData(int btnState, float openInst, bool blnDataReady)
{
    cout << "The data are: " << btnState;
    //instrumentData g;
    //a= g.pos
}