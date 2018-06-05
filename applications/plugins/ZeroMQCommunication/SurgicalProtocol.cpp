#include "SurgicalProtocol.h"
#include <sofa/core/behavior/BaseController.h>

namespace sofa
{

namespace component
{

namespace controller
{

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


/*--------------------------------------msgPointsGroup----------------------------------  */
msgPointsGroupV3::msgPointsGroupV3()
{
    NroPointsC = 0;
    startPoint = 0;
    NbPointsMsg = 20;
}

void msgPointsGroupV3::test()
{
    std::cout << "Probando la instanciacion de clase" << std::endl;
    NroPointsC = 0;
    startPoint = 0;
    NbPointsMsg = 20;
    PointsC.resize(20);
}

bool msgPointsGroupV3::SetupReceive()
{
    float data[80];
    // startPoint = (int)data[0];

    for(int i=0;i<80;i++)
    {
        data[i] = (float)i;
    }

    for (int i = 0; i<=NbPointsMsg; i+=3)
    {
        // Llenando el vector que contiene todos los puntos del organo
        PointNet pTemp(data[i], data[i+1], data[i+2]);
        // vNroPointC.push_back((int)data[i]);
        PointsC[i] = pTemp;
        std::cout << "Guardando " << i << "punto por " << i << "ésima ocasión" << std::endl;
        std::cout << PointsC[i].getX() << " " << PointsC[i].getY() << " " << PointsC[i].getZ() << std::endl;  
        std::cout << PointsC.size() << std::endl;
        /*
        if (PointsC.size() == 20 )
        {
            std::cout << i << ". # máximo de puntos alcanzado." << std::endl;

        }
        */
    }
    // vector<PointNet> total(20, "empty");
    /**
     * Declaro el vector PointNet y proporciono
     * el numero de elementos que deseo almacenar y
     * el valor string por defecto empty
    */

   /*
    for(int i=0; i<=PointsC.size(); i++)
    {
        // total+=PointsC[i];
        std::cout << "PointsC element is: " << PointsC[i] << std::endl;
    }*/
    return true;
}


}

}

}