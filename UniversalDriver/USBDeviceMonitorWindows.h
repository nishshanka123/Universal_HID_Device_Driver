#ifndef USB_DEVICE_MONITOR_WINDOWS_H
#define USB_DEVICE_MONITOR_WINDOWS_H
/**
* Platform specific implementation for windows to monitor the USB interface
* changes for device changes.
*/
#include "USBDeviceMonitor.h"
#include <windows.h>
#include <initguid.h>
#include <Dbt.h>
#include <ntddstor.h>
#include <thread>
#include <atomic>
#include "external/hidapi/include/hidapi.h"

// Define GUID_DEVINTERFACE_HID if it is not defined
DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4D1E55B2, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30);
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);


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

	// device notification handler to register with the application window. This is needed as the application 
	// is launch as a console app and there is no parent window. 
	HDEVNOTIFY device_notification_handle;

	// Register callback for the wondow instance to handle HWND events
	static LRESULT CALLBACK HWNDWindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Additional functions for handling the windows messages and enumerate the USB devices
	void MessageLoop();
	void HandleDeviceChange(WPARAM wParam, LPARAM lParam);
	void EnumerateDevices();
	void RegisterForDeviceNotifications();
	void UnregisterDeviceNotifications();

};


#endif