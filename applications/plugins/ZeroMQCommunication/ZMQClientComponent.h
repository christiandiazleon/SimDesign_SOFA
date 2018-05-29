#include <sofa/core/behavior/BaseController.h>
#include <zmq.hpp>
#include <sys/time.h>
#include "PointNet.h"

// #include <sofa/core/objectmodel/Data.h>
// using sofa::core::objectmodel::Data;

#include <sofa/defaulttype/VecTypes.h>
// #include "SofaTypeMessages.h"

// To Quat datatype
#include <sofa/defaulttype/Quat.h>
using sofa::defaulttype::Quat;

//#include <SerialComunication/SerialDriver.h>
#include "SerialDriver.h"
/// #include <SofaBaseMechanics/MechanicalObject.h>

using std::string;

namespace sofa
{

namespace component
{

namespace controller
{

struct instrumentData
{
  typedef sofa::defaulttype::Vec3d Vec3d;
  Vec3d pos;
  Quat quat;
  int btnState;
  float openInst;
  bool blnDataReady;
};

struct attachingData
{
  std::vector<PointNet> vPointsAttach;
  std::vector<int> vIdTriangles;
  std::vector<int> vIdInstTriangles;
  string strMechanicalModel;
  string strSurfaceModel;
  int nContactPoints;
  bool blnAttachContact;
  bool blnDeAtacchContact;
  float fDist;
};

class ZMQClientComponent : public sofa::core::behavior::BaseController
{
  public:
    SOFA_CLASS(ZMQClientComponent, sofa::core::behavior::BaseController);
    // Data<double> myparam;
    // Data<std::string> d_address;
    // typedef sofa::defaulttype::Vec3d Vec3d;
    // typedef defaulttype::Quat Quat;0


    ZMQClientComponent();
    virtual ~ZMQClientComponent(){};

    /* Conect to ZMQ external Server  */
    void setupConnection();
    // void setupConnectionAttachingData();

    /* Send some data to ZMQ external Server  */
    void instrumentDataSend(instrumentData a);
    void hapkitDataSend();
    void attachingDataToSend(attachingData b);


    /* Get some response from ZMQ external Server */
    //void getResponseFromServer();

    /* Initialize ZMQ Internal Server and receive some data */
    // void receiveRequests();
    // void sendReplyToClient();
    void init();

    //virtual void draw(){};
    virtual void draw();
    //virtual void draw(const core::visual::VisualParams *) override;
    //virtual void draw(const core::visual::VisualParams *){};
    virtual void draw(const core::visual::VisualParams *);

    /** We declare an alias to SerialDriver attribute, which should be a vector
     * to reference it on ZMQClientComponent.cpp to save inside the SerilDriver objects
      */
    //typedef sofa::component::controller::SerialDriver g;
    typedef SerialDriver SerialDriverType;
    // typedef sofa::component::controller::SerialDriver d;
    std::vector<SerialDriverType *> objectsSerialDriver;

  private:
    struct timeval t_before, t_after;
    string replyMessage;
    SerialDriver *s = new SerialDriver();
    string hapkitValue;
};

class msgPointsGroupV3
{
private:
	int IDBody;
  int IDTypeMsgDef; // 1 Start message - 2 Middle messages - 3 End message
  int NroPointsC;
  vector<PointNet> PointsC; // Vector que contiene todos los puntos de deformación del organo a ser enviados
  vector<int> vNroPointC; // Vector que contiene el ID de cada punto de deformación del organo en la escena que sera enviado
                            // Cada punto PointNet tiene un eje x,y,z  y un ID entonces debe construirse un mensaje asi:
                            // ID1_X1_Y1_Z1-ID2_X2_Y2_Z2

  int startPoint;
  int NroTotalPoints;
	int NbPointsMsg; // Número de puntos que se envían por mensaje

public:
	msgPointsGroupV3();

  void test();

	void setIDBody(int value){IDBody = value;}
  void setNroPointsC(int value){NroPointsC = value;}
  void setStartPoint(int value){startPoint = value;}
  void setPointsC(vector<PointNet> value){PointsC = value;}
  void setIDTypeMsgDef(int value){IDTypeMsgDef = value;}
  void setNroTotalPoints(int value){NroTotalPoints = value;}
	void setNbPointsMsg(int value){NbPointsMsg = value;}

	int getIDBody(){return IDBody;}
  int getNroPointsC(){return NroPointsC;}
  int getStartPoint(){return startPoint;}
  int getIDTypeMsgDef(){return IDTypeMsgDef;}
  int getNroTotalPoints(){return NroTotalPoints;}
	int getNbPointsMsg(){return NbPointsMsg;}
	vector<PointNet> getPointsC(){return PointsC;}
  vector<int> getVecNroPointsC(){return vNroPointC;}

	bool SetupSend();
	bool SetupReceive();
  void CompAmPointsByMsg();
  void ResetData();
  void PrintMsg();
	void FreeMemory();
};


}

}

}
