#ifndef USB_DEVICE_MONITOR_GENERIC_H
#define USB_DEVICE_MONITOR_GENERIC_H

#include "USBDeviceMonitor.h"
#include "external/libusb-1.0.24/include/libusb-1.0/libusb.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <set>
#include <fstream>
#include <map>
#include <string>

class USBDeviceMonitorGeneric : public USBDeviceMonitor
{
public:
	USBDeviceMonitorGeneric();
	virtual ~USBDeviceMonitorGeneric() override;

	void start() override;
	void stop() override;

private:
	void monitorLoop();
	std::set<std::string> getConnectedDevices();
	
	std::map < std::string, std::string> deviceConfig;
	std::atomic<bool> is_running;	// use atomic to access control by other threads while operation is inprogress
	std::thread monitor_thread;
	libusb_context* ctx;

};

#endif