/**
* USB device monitor interface implementation for Linux platform.
* this concrete class implement the USB interface chages in Linux platform using the
* platform specific services and libraries.
*/

#ifndef USB_DEVICE_MONITOR_LINUX_H
#define USB_DEVICE_MONITOR_LINUX_H

#include "USBDeviceMonitor.h"
#include <thread>
#include <libudev.h>
#include <atomic>

class USBDeviceMonitorLinux :public USBDeviceMonitor {
public:
	USBDeviceMonitorLinux();
	virtual ~USBDeviceMonitorLinux() override;

	void start() override;
	void stop() override;

private:
	struct udev* udev_udev;
	struct udev_monitor* udev_monitor;
	std::thread monitor_thread;
	std::atomic<bool> is_running;
	
	//void DeviceMonitorLoop();

};

#endif

