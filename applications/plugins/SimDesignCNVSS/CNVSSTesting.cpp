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
#include "CNVSSTesting.h"

#include <sofa/core/ObjectFactory.h>
//#include <SofaOpenglVisual/OglModel.h>


namespace sofa
{

namespace component
{

namespace behaviormodel
{


CNVSSTesting::CNVSSTesting():
    customUnsignedData(initData(&customUnsignedData, (unsigned)1,"Custom Unsigned Data","Example of unsigned data with custom widget")),
    regularUnsignedData(initData(&regularUnsignedData, (unsigned)1,"Unsigned Data","Example of unsigned data with standard widget"))
{
    customUnsignedData.setWidget("widget_myData");
}


CNVSSTesting::~CNVSSTesting()
{
}

void CNVSSTesting::init()
{
  std::cout << "CNVSSTesting::init()" << std::endl;

  //Looking for OglModel Components
	std::vector<sofa::component::visualmodel::OglModel*> objectsOglModelTemp;
	getContext()->get<sofa::component::visualmodel::OglModel>(&objectsOglModelTemp, core::objectmodel::BaseContext::SearchDown);

  int number = objectsOglModelTemp.size();
	std::cout << "Number of visual models: " << number << std::endl;

  if(number>0){
    std::cout << "Name of the visual model identified " << objectsOglModelTemp[0]->name.getValue() << std::endl;
  }

}

void CNVSSTesting::reinit()
{
}

void CNVSSTesting::updatePosition(double /*dt*/)
{
}

SOFA_DECL_CLASS(CNVSSTesting)

int CNVSSTestingClass = core::RegisterObject("Dummy component with a custom widget.").add< CNVSSTesting >();


} // namespace behaviormodel

} // namespace component

} // namespace sofa
