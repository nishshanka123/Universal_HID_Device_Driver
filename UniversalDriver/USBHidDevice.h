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
	uint16_t vendorId;
	uint16_t productId;
	bool isOpen;
	std::thread dataRecieveThread;
	std::atomic<bool> isInprogress;
	hid_device* hidDeviceHandler;
	std::function<void(const std::vector<uint8_t>&)> dataCallback;

public:
	// C/Dtors
	USBHidDevice();
	USBHidDevice(uint16_t pid, uint16_t vid);
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
	void sendData(std::vector<uint8_t>& data);

	/**
	* Method to receive data from device, call hid read API
	*/
	void recieveData(std::vector<uint8_t>& data);

	/**
	* Start data reading from the device
	*/
	void startRecieving(std::function<void(const std::vector<uint8_t>&)>& callback);
	
	/**
	* Stop data reading
	*/
	void stopRecieving();

	/**
	* write a receive worker function to seperate the reading thread from data processing
	* specially for serperation of consern.
	*/
	void receiveWorker();

};
#endif // !INCLUDE_USB_HID_DEVICE_H

