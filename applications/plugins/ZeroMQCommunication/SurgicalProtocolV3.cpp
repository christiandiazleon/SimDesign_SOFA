//
//  SurgicalProtocol.cpp
//  NetComponent
//
//  Created by Christian Diaz on 11/29/11.
//  Copyright (c) 2011 Stanford University. All rights reserved.
//

#include <iostream>
#include <sofa/helper/networking/SurgicalProtocolV3.h>

namespace sofaZMQClientComponent
{

namespace helper
{

namespace networking
{

msgPointsGroupV3::msgPointsGroupV3()
{
    NroPointsC = 0;
    startPoint = 0;
}

void msgPointsGroupV3::test()
{
    std::cout << "Probando la instanciacion de clase" << std::endl;
}

void msgPointsGroupV3::FreeMemory()
{
    free(buffer);
}

bool msgPointsGroupV3::SetupSend()
{
    buffer = (char *)malloc(sizeof(char) * bufferSize);
    char mystringIDC[10];
    char mystringIDObj[10];
    char mystringIDTMDef[10];
    char mystringNroTotalPoints[10];
    char mystringNroPoints[10];
    char mystringPoint[10];
    char mystringX[30];
    char mystringY[30];
    char mystringZ[30];
    int NroPtoTemp;

    sprintf(buffer, "%d", idTypeMessage);
    sprintf(mystringIDC, "%d", FromIdMachine);
    sprintf(mystringIDObj, "%d", IDBody);
    sprintf(mystringIDTMDef, "%d", IDTypeMsgDef);
    sprintf(mystringNroTotalPoints, "%d", NroTotalPoints);
    sprintf(mystringNroPoints, "%d", NroPointsC);

    strcat(buffer, " ");
    strcat(mystringIDC, " ");
    strcat(mystringIDObj, " ");
    strcat(mystringIDTMDef, " ");
    strcat(mystringNroPoints, " ");
    strcat(mystringNroTotalPoints, " ");

    strcat(buffer, mystringIDC);
    strcat(buffer, mystringIDObj);
    strcat(buffer, mystringIDTMDef);
    strcat(buffer, mystringNroTotalPoints);
    strcat(buffer, mystringNroPoints);

    for (int i = 0; i < NbPointsMsg; i++)
    {

        if (NroPointsC > i)
        {
            NroPtoTemp = startPoint + i;
            sprintf(mystringPoint, "%d", NroPtoTemp);
            sprintf(mystringX, "%f", PointsC[i].getX());
            sprintf(mystringY, "%f", PointsC[i].getY());
            sprintf(mystringZ, "%f", PointsC[i].getZ());
        }
        else
        {
            NroPtoTemp = -1;
            sprintf(mystringPoint, "%d", -1);
            sprintf(mystringX, "%f", -1.0);
            sprintf(mystringY, "%f", -1.0);
            sprintf(mystringZ, "%f", -1.0);
        }

        strcat(mystringPoint, " ");
        strcat(mystringX, " ");
        strcat(mystringY, " ");
        strcat(mystringZ, " ");

        strcat(buffer, mystringPoint);
        strcat(buffer, mystringX);
        strcat(buffer, mystringY);
        strcat(buffer, mystringZ);
    }

    return true;
}

bool msgPointsGroupV3::SetupReceive()
{
    float data[80];
    string strTemp(buffer);

    // construct a stream from the string
    std::stringstream strstr(strTemp);

    // use stream iterators to copy the stream to the vector as whitespace separated strings
    std::istream_iterator<std::string> it(strstr);
    std::istream_iterator<std::string> end;
    std::vector<std::string> results(it, end);

    if (results.size() == ((NbPointsMsg * 4) + 6))
    {
        for (int i = 0; i < results.size(); i++)
        {
            if (i == 0)
            {
                idTypeMessage = atoi(results[0].c_str());
            }
            else if (i == 1)
            {
                FromIdMachine = atoi(results[1].c_str());
            }
            else if (i == 2)
            {
                IDBody = atoi(results[2].c_str());
            }
            else if (i == 3)
            {
                IDTypeMsgDef = atoi(results[3].c_str());
            }
            else if (i == 4)
            {
                NroTotalPoints = atoi(results[4].c_str());
            }
            else if (i == 5)
            {
                NroPointsC = atoi(results[5].c_str());
            }
            else
            {
                data[i - 6] = atof(results[i].c_str());
            }
        }

        startPoint = (int)data[0];

        for (unsigned int i = 0; i < (NbPointsMsg * 4); i += 4)
        {
            vNroPointC.push_back((int)data[i]);
            PointNet pTemp(data[i + 1], data[i + 2], data[i + 3]);
            PointsC.push_back(pTemp);
        }

        return true;
    }
    else
    {
        return false;
    }
}

void msgPointsGroupV3::CompAmPointsByMsg()
{
}

void msgPointsGroupV3::ResetData()
{
    FromIdMachine = 0;
    idTypeMessage = 0;
    IDBody = 0;
    NroPointsC = 0;
    PointsC.clear();
    vNroPointC.clear();
    startPoint = 0;
}

void msgPointsGroupV3::PrintMsg()
{
    /*std::cout << "IdTypeMsg: " << idTypeMessage << " IDMachine: " << FromIdMachine << " IDBody: " << IDBody << " IDTypeDefMsg: " << IDTypeMsgDef << "Nro Total: " << NroTotalPoints << " NroPoints " << NroPointsC << std::endl;
    
    for (int i = 0; i<NroPointsC; i++) {
        
        std::cout << "NroPoint: " << startPoint + i << " Point: " << PointsC[i].getX() << " " << PointsC[i].getY() << " " << PointsC[i].getZ() << std::endl; 
    
    }*/

    std::cout << idTypeMessage << " " << FromIdMachine << " " << IDBody << " " << IDTypeMsgDef << " " << NroTotalPoints << " " << NroPointsC << " ";

    for (int i = 0; i < NroPointsC; i++)
    {

        std::cout << startPoint + i << " " << PointsC[i].getX() << " " << PointsC[i].getY() << " " << PointsC[i].getZ() << " ";
    }

    std::cout << std::endl;
}

} // namespace networking

} // namespace helper

} // namespace sofa
