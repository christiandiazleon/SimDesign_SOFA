#include "initZeroMQCommunication.h"
#include <sofa/helper/system/config.h>

#ifndef WIN32
#define SOFA_EXPORT_DYNAMIC_LIBRARY
#define SOFA_IMPORT_DYNAMIC_LIBRARY
#define SOFA_ZEROMQCOMMUNICATIONPLUGIN_API
#else
#ifdef SOFA_BUILD_ZEROMQCOMMUNICATIONPLUGIN
#define SOFA_ZEROMQCOMMUNICATIONPLUGIN_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_ZEROMQCOMMUNICATION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif
#endif

/*
#ifdef WIN32
// BUGFIX(Jeremie A. 02-05-2009): put OpenHaptics libs here instead of the project file to work around a bug in qmake when there is a space in an environment variable used to look-up a library
#pragma comment(lib,"hl.lib")
#pragma comment(lib,"hd.lib")
#pragma comment(lib,"hdu.lib")
#endif
*/

/*
namespace sofa
{

namespace component
{
*/

//Here are just several convenient functions to help user to know what contains the plugin

extern "C" {
SOFA_ZEROMQCOMMUNICATIONPLUGIN_API void initExternalModule();
SOFA_ZEROMQCOMMUNICATIONPLUGIN_API const char *getModuleName();
SOFA_ZEROMQCOMMUNICATIONPLUGIN_API const char *getModuleVersion();
SOFA_ZEROMQCOMMUNICATIONPLUGIN_API const char *getModuleLicense();
SOFA_ZEROMQCOMMUNICATIONPLUGIN_API const char *getModuleDescription();
SOFA_ZEROMQCOMMUNICATIONPLUGIN_API const char *getModuleComponentList();
}

void initExternalModule()
{
    // Here is the place to write initialisation code, that will be executed
    // before any component is created.
}

const char *getModuleName()
{
    return "Plugin zeroMQ Async Client/Server communication";
}

const char *getModuleVersion()
{
    return "0.1";
}

const char *getModuleLicense()
{
    return "LGPL";
}

const char *getModuleDescription()
{
    return "Network communication from SOFA and external enities via zeroMQ sockets";
}

const char *getModuleComponentList()
{
    // Comma-separated list of the components in this plugin, empty for now
    return "";
}

//}

//}

SOFA_LINK_CLASS(ZMQClientComponent)
