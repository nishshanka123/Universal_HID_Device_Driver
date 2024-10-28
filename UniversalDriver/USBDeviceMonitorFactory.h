#ifndef USB_DEVICE_MONITOR_FACTORY_H
#define USB_DEVICE_MONITOR_FACTORY_H

#include "USBDeviceMonitor.h"

class USBDeviceMonitorFactory {
public:
    // Static method to create a platform-specific USB device monitor based no the code is built on
    static USBDeviceMonitor* Create();
};

#endif // USB_DEVICE_MONITOR_FACTORY_H