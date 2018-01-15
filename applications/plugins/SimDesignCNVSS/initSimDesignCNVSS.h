#ifndef INITSIMDESIGNCNVSS_H
#define INITSIMDESIGNCNVSS_H

#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_SIMDESIGNCNVSS
#define SOFA_SimDesignCNVSS_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_SimDesignCNVSS_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

/** mainpage
This is the main page of the doxygen documentation for MyPlugin.
 */

#endif
