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
#ifndef PLUGINEXAMPLE_NETWORKCONTROLLERCNVSS_H
#define PLUGINEXAMPLE_NETWORKCONTROLLERCNVSS_H

#include "initSimDesignCNVSS.h"
#include <sofa/core/behavior/BaseController.h>
#include <SofaUserInteraction/Controller.h>
#include <SofaOpenglVisual/OglModel.h>

namespace sofa
{

namespace component
{

namespace controller
{


/**
 * This CNVSSTesting does nothing but contain a custom data widget.
 */
class NetworkControllerCNVSS : public Controller
{

public:
    SOFA_CLASS(NetworkControllerCNVSS, Controller);

protected:
    NetworkControllerCNVSS();
    ~NetworkControllerCNVSS();

public:
    virtual void init();
    virtual void reinit();
    virtual void updatePosition(double dt);

private:
  void handleEvent(core::objectmodel::Event *);
  bool initInteractionObjects();
  bool initConnection();

protected:
    Data<unsigned> customUnsignedData;
    Data<unsigned> regularUnsignedData;
};


} // namespace controller

} // namespace component

} // namespace sofa


#endif // NETWORKCONTROLLERCNVSS__H
