#ifndef INITZEROMQCOMMUNICATION_H
#define INITZEROMQCOMMUNICATION_H

#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_ZEROMQCOMMUNICATION
#define SOFA_ZeroMQCommunication_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_ZeroMQCommunication_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

/** mainpage
This is the main page of the doxygen documentation for MyPlugin.
 */

#endif
