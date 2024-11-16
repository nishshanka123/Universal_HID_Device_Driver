#ifndef INCLUDE_USB_HID_DEVICE_H
#define INCLUDE_USB_HID_DEVICE_H

#include "external/hidapi/include/hidapi.h"
#include <map>
#include <exception>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <functional>

class USBHidDevice
{
private:
	int vendorId;
	int productId;
	bool is_open;
public:
	// C/Dtors
	USBHidDevice();
	USBHidDevice(int pid, int vid);
	virtual ~USBHidDevice();

	// member functions
	/**
	* Initialize the device HID connectivity
	*/
	bool initializeDevice();

	/**
	* Deinitialize the device HID connectivity
	*/
	void closeDevice();

	/**
	* Get device connectivity status, this is an internal flag
	*/
	bool isDeviceOpen();

	/**
	* Send data to device, wrapping for hid write call.
	*/
	bool sendData();

	/**
	* Method to receive data from device, call hid read API
	*/
	bool recieveData();

	/**
	* Start data reading from the device
	*/
	void startRecieving();
	
	/**
	* Stop data reading
	*/
	void stopRecieving();

};
#endif // !INCLUDE_USB_HID_DEVICE_H

