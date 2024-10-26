#ifndef USB_DEVICE_MONITOR_WINDOWS_H
#define USB_DEVICE_MONITOR_WINDOWS_H
/**
* Platform specific implementation for windows to monitor the USB interface
* changes for device changes.
*/
#include "USBDeviceMonitor.h"
#include <windows.h>
#include <thread>
#include <atomic>
#include "hidapi/hidapi.h"


class USBDeviceMonitorWindows :public USBDeviceMonitor
{
public:
	USBDeviceMonitorWindows();
	virtual ~USBDeviceMonitorWindows() override;
	void start() override;
	void stop() override;

private:
	/*Use a windows handle to receive the system messages. 
	* Windows is broatcating events like WM_DEVICECHANGE 
	* to all application and HWND instance can be use to 
	* catch those by creating a hidden window.
	*/
	HWND hwnd;

	// Thread instance for monitoring, use std::thread
	std::thread monitor_thread;
	std::atomic<bool> is_running;

	// Register callback for the wondow instance to handle HWND events
	static LRESULT CALLBACK HWNDWindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Additional functions for handling the windows messages and enumerate the USB devices
	void MessageLoop();
	void HandleDeviceChange(WPARAM wParam, LPARAM lParam);
	void EnumerateDevices();

};


#endif