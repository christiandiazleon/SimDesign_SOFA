#include <iostream>
#include "SurgicalProtocol.h"
#include <sofa/core/behavior/BaseController.h>
// typedef sofa::defaulttype::Vec3d Vec3d;

#include <string>

// To Quat datatype
#include <sofa/defaulttype/Quat.h>
using namespace std;
using std::string;

namespace sofa
{

namespace component
{

namespace controller
{

//using sofa::defaulttype::Quat;
typedef defaulttype::Quat Quat;
//typedef sofa::defaulttype::Quatd;




struct probingData
{
  int nContactPoints;
  std::vector<PointNet> vPointsProbing;
  std::vector<PointNet> vNormalsProbing;
  std::vector<int> ID;
  string strCollisionModel1;
  string strCollisionModel2;
  std::vector<int> vIdTriangles1;
  std::vector<int> vIdTriangles2;
  bool blnProbeContact;
};

struct attachClipData1
{
  std::vector<PointNet> vPointsAttach;
  std::vector<int> vIdTriangles;
  string strMechanicalModel;
  string strSurfaceModel;
  int nContactPoints;
  bool blnAttachClip;
  float fDist;
};

struct attachClipData2
{
  sofa::defaulttype::Vector3 positionClip;
  sofa::defaulttype::Quat orientationClip;
  bool blnDataReady;
};

struct cuttingData
{
  int method;
  string strBody1;
  int indexColElement1;
  PointNet pnt1;
  string strBody2;
  int indexColElement2;
  PointNet pnt2;
  string strBodyL;
  int indexColElementL;
  PointNet pntL;
  bool firstCollision;
  bool secondCollision;
  bool cut;
};

struct carvingData
{
  int NroCarvingPrimitives;
  string strBody;
  helper::vector<int> elemtsToRemove;
  bool IsCarve;
};

class SofaTypeMessages
{


public:
  SofaTypeMessages();
  void test();
  //void instrumentDataSend(instrumentData a);
  void attachingData();
  void probingData();
  void attachClipData1();
  void attachClipData2();
  void cuttingData();
  void carvingData();
};


}

}

}