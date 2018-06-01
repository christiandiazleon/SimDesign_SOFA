#include <sofa/core/behavior/BaseController.h>
using namespace std;

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

    void setX(float value){X = value;}
    void setY(float value){Y = value;}
    void setZ(float value){Z = value;}

    float getX(){return X;}
    float getY(){return Y;}
    float getZ(){return Z;}

  private:
    float X;
    float Y;
    float Z;
};


class msgPointsGroupV3
{
  private:
    int IDBody;
    int IDTypeMsgDef; // 1 Start message - 2 Middle messages - 3 End message
    int NroPointsC;
    vector<PointNet> PointsC; // Vector que contiene todos los puntos de deformación del organo a ser enviados
    
    vector<int> vNroPointC; 
    /* Vector que contiene el ID de cada punto de deformación del organo en la escena que sera enviado
      Cada punto PointNet tiene un eje x,y,z  y un ID entonces debe construirse un mensaje asi:
      ID1_X1_Y1_Z1-ID2_X2_Y2_Z2 */

    int startPoint;
    int NroTotalPoints;
    int NbPointsMsg; // Número de puntos que se envían por mensaje
  
  public:
    msgPointsGroupV3();
    void test();
    bool SetupReceive();
};

}

}

}