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
#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_SERIALPLUGIN
#define SOFA_SERIALPLUGIN_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_SERIALPLUGIN_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

extern "C" {
    void initExternalModule()
    {
        // Here is the place to write initialisation code, that will be executed
        // before any component is created.
    }

    const char* getModuleName()
    {
        return "Serial Driver";
    }

    const char* getModuleVersion()
    {
        return "0.1";
    }

    const char* getModuleLicense()
    {
        return "LGPL";
    }

    const char* getModuleDescription()
    {
        return "Serial Driver provides interfaces for comunication with serial ports in the SOFA Framework.";
    }

    const char* getModuleComponentList()
    {
        // Comma-separated list of the components in this plugin, empty for now
        return "SerialPort Controllers";
    }
}

SOFA_LINK_CLASS(SerialDriver)