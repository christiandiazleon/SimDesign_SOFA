#include <sofa/core/behavior/BaseController.h>

namespace sofa
{

namespace component
{

namespace controller
{

class PointNet
{
  public:
    PointNet(float valX, float valY, float valZ);
    PointNet();
    /*
    void setX(float value);
    void setY(float value);
    void setZ(float value);

    float getX();
    float getY();
    float getZ();
    */
    void setX(float value) { X = value; }
    void setY(float value) { Y = value; }
    void setZ(float value) { Z = value; }
    
    float getX() { return X; }
    float getY() { return Y; }
    float getZ() { return Z; }

  private:
    float X;
    float Y;
    float Z;
};

}

}

}