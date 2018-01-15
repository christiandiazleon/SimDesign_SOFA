#include "initSimDesignCNVSS.h"

extern "C" {
    void initExternalModule()
    {
        // Here is the place to write initialisation code, that will be executed
        // before any component is created.
    }

    const char* getModuleName()
    {
        return "SimDesignCNVSS";
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
        return "MyPlugin provides nothing for now.";
    }

    const char* getModuleComponentList()
    {
        // Comma-separated list of the components in this plugin, empty for now
        return "";
    }
}

SOFA_LINK_CLASS(CNVSSTesting)
