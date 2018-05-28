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
#include "NetworkControllerCNVSS.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/Simulation.h>
//#include <SofaOpenglVisual/OglModel.h>


namespace sofa
{

namespace component
{

namespace controller
{


NetworkControllerCNVSS::NetworkControllerCNVSS():
    customUnsignedData(initData(&customUnsignedData, (unsigned)1,"Custom Unsigned Data","Example of unsigned data with custom widget")),
    regularUnsignedData(initData(&regularUnsignedData, (unsigned)1,"Unsigned Data","Example of unsigned data with standard widget"))
{
    customUnsignedData.setWidget("widget_myData");
}


NetworkControllerCNVSS::~NetworkControllerCNVSS()
{
}

void NetworkControllerCNVSS::init()
{
  std::cout << "NetworkControllerCNVSS::init()" << std::endl;

  //Looking for OglModel Components
	std::vector<sofa::component::visualmodel::OglModel*> objectsOglModelTemp;
	getContext()->get<sofa::component::visualmodel::OglModel>(&objectsOglModelTemp, core::objectmodel::BaseContext::SearchDown);

  simulation::Node *tempNode =(simulation::Node *) objectsOglModelTemp[0]->getContext();

  std::cout << "Node identified " << tempNode->name.getValue() << std::endl;

  int number = objectsOglModelTemp.size();
	std::cout << "Number of visual models: " << number << std::endl;

  if(number>0){
    std::cout << "Name of the visual model identified " << objectsOglModelTemp[0]->name.getValue() << std::endl;
  }

}

bool NetworkControllerCNVSS::initInteractionObjects()
{

}

bool NetworkControllerCNVSS::initConnection()
{

}

void NetworkControllerCNVSS::reinit()
{
}

void NetworkControllerCNVSS::updatePosition(double /*dt*/)
{
}

void NetworkControllerCNVSS::handleEvent(core::objectmodel::Event *event)
{

}

SOFA_DECL_CLASS(NetworkControllerCNVSS)

int NetworkControllerCNVSSClass = core::RegisterObject("Dummy component for interface SOFA with the network").add< NetworkControllerCNVSS >();


} // namespace controller

} // namespace component

} // namespace sofa
