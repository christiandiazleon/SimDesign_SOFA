#include <iostream>

#include <sofa/core/behavior/BaseController.h>
typedef sofa::defaulttype::Vec3d Vec3d;

// To Quat datatype
#include <sofa/defaulttype/Quat.h>
using sofa::defaulttype::Quat;
//typedef defaulttype::Quat Quat;
typedef sofa::defaulttype::Quatd;

class SofaTypeMessages
{


public:
  SofaTypeMessages();
  void instrumentData(Vec3d pos, Quat quat, int btnState, float openInst, bool blnDataReady);
  void attachingData();
  void probingData();
  void attachClipData1();
  void attachClipData2();
  void cuttingData();
  void carvingData();
};