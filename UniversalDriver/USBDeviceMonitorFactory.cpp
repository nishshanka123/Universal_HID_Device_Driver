#include "USBDeviceMonitorFactory.h"
#include "USBDeviceMonitorGeneric.h"

// Include the relavant header file based on the platform.
#ifdef _WIN32
    #include "USBDeviceMonitorWindows.h"
#elif defined(__linux__)
    #include "USBDeviceMonitorLinux.h"
#elif defined(__unix__)
    #include "USBDeviceMonitorLinux.h"
#else
    // curretly support only Windows and Linux platforms. This can be extend based ont he platform.
    #error "Unsupported platform"
#endif

// Implementation of Create the platform object for the device monitor class.
USBDeviceMonitor* USBDeviceMonitorFactory::Create() {
// return the platform object accordingly.
#ifdef _WIN32
    //return new USBDeviceMonitorWindows();
    return new USBDeviceMonitorGeneric();
#elif defined(__linux__)
    //return new USBDeviceMonitorLinux();
    return new USBDeviceMonitorGeneric();
#elif defined(__unix__)
    //return new USBDeviceMonitorLinux();
    return new USBDeviceMonitorGeneric();
#else
    return nullptr; // Unsupported platform
#endif

}