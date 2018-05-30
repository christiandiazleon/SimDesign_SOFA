#include "PointNet.h"
#include <sofa/core/behavior/BaseController.h>

namespace sofa
{

namespace component
{

namespace controller
{

  //------------------------------------Point---------------------------------------

  PointNet::PointNet(float valX, float valY, float valZ)
  {
      X = valX;
      Y = valY;
      Z = valZ;
  }

  PointNet::PointNet()
  {
      X = 0.0;
      Y = 0.0;
      Z = 0.0;
  }

}

}

}
