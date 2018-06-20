/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, v17.06                  *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef PLUGINEXAMPLE_POINTNET_H
#define PLUGINEXAMPLE_POINTNET_H

#include "initSimDesignCNVSS.h"


namespace sofa
{

namespace component
{

namespace forcefield
{


/**
 * This class is a container of 3D Points
 */
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


} //namespace forcefield

} // namespace componente

}  // namespace sofa


#endif // PLUGINEXAMPLE_POINTNET_H
