#ifndef USB_DEVICE_MONITOR_H
#define USB_DEVICE_MONITOR_H

#include <string>
#include <exception>
#include <fstream>
#include <iostream>

/**
* Purpose:
* Device monitor interface to provide platform dependent implementations for device
* monitoring in each platform. Aim is to develop a cross platform supported c++ implementation.
*/
class USBDeviceMonitor
{
public:
	virtual ~USBDeviceMonitor() = default;
	//pure virtual functions for derived classes
	virtual void start() = 0;
	virtual void stop() = 0;
	std::string getConfiguration(std::string fileName);
	void createDeviceConfiguration(std::string fileName, std::string config);
protected:
	//void EnumerateDevices();
	
};

#endif // end of USB_DEVICE_MONITOR_H