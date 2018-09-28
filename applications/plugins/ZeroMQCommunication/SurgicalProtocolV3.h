//
//  SurgicalProtocol.h
//  NetComponent
//
//  Created by Christian Diaz on 11/29/11.
//  Copyright (c) 2011 Stanford University. All rights reserved.
//

#ifndef SOFA_HELPER_NETWORKING_SURGICALPROTOCOLV3_H
#define SOFA_HELPER_NETWORKING_SURGICALPROTOCOLV3_H

#include <iostream>
#include <vector>
#include <sofa/helper/helper.h>
#include <sofa/defaulttype/SolidTypes.h>
#include <string>
#include <sstream>

// #include <sofa/helper/networking/SurgicalProtocol.h>
#include <iterator>
#include <algorithm>

using namespace std;

namespace sofa
{

namespace helper
{

namespace networking
{

using namespace sofa::defaulttype;

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

}	//networking

}	//helper

}	//sofa

#endif  //SOFA_HELPER_NETWORKING_SURGICALPROTOCOL_V3_H