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

class Message
{
protected: // para que puedan ser accedidas por los metodos de la clase msgInstrumentPosition que heredan de Message
  int FromIdMachine;
  int ToIdMachine; //-1 if it is for all machines
  // 1->Instrument message ---- 2->Collision message ---- 3->DeformableModel Message ---- 4->Attach Clip Messages ----
  // 5 -> Attaching messages ---- 6 -> Cutting messages ---- 7 ->Carving messages
  int idTypeMessage;
  char *buffer;
  int bufferSize;
  string bufferStr;

public:
  void setIDTypeMessage(int value) { idTypeMessage = value; }
  int getIDTypeMessage() { return idTypeMessage; }
};

/*
class msgInstrumentPosition : public Message
{
  private:
    float instrumentPosition;
  public:
    msgInstrumentPosition();  
    void setInstrumentPosition(float value) { instrumentPosition = value; }
    bool SetupSend();
    bool SetupReceive();
};
*/

class msgPointsGroup
{
  private:
    int IDBody;
    int IDTypeMsgDef; // 1 Start message - 2 Middle messages - 3 End message
    int NroPointsC;
    std::vector<PointNet> PointsC; // Vector que contiene todos los puntos de deformación del organo a ser enviados
    
    vector<int> vNroPointC; 
    /* Vector que contiene el ID de cada punto de deformación del organo en la escena que sera enviado
      Cada punto PointNet tiene un eje x,y,z  y un ID entonces debe construirse un mensaje asi:
      ID1_X1_Y1_Z1-ID2_X2_Y2_Z2 */

    int startPoint;
    int NroTotalPoints;
    int NbPointsMsg; // Número de puntos que se envían por mensaje
  
  public:
    msgPointsGroup();

    void test();
    
    void setPointsC(std::vector<PointNet> value){PointsC = value;}

    bool SetupSend();
    bool SetupReceive();
    void setNroTotalPoints(int value) { NroTotalPoints = value; }
    void setIDTypeMsgDef(int value) { IDTypeMsgDef = value; }
};

}

}

}