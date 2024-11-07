#include "USBDeviceMonitorWindows.h"
#include <iostream>

USBDeviceMonitorWindows::USBDeviceMonitorWindows() :is_running(false), hwnd(nullptr), device_notification_handle(nullptr)  {
	// Initialize HIDAPI
	if (hid_init() != 0) {
		throw std::runtime_error("Failed to initialize the HIDAPI.");
	}
	else {
		std::cout << "HID library initialized..." << std::endl;
	}

	// register the windows class (current windows of the program) for message handling.
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = HWNDWindowProcess;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "USBDeviceMonitorWindowsClass";
	std::cout << "Register the app for message handling..." << std::endl;

	// register the window class instance, throw exception if failed.
	if (!RegisterClass(&wc)) {
		throw std::runtime_error("Failed to register the app window class");
	}

	// then, create the windows, whole purpose of creating this is to receive the windows hardware event.
	hwnd = CreateWindowEx(
		0,                               // Optional window styles
		"USBDeviceMonitorWindowsClass",  // Window class name
		"USB Device Monitor",            // Window title
		0,                               // Window style (use 0 for no border, WS_OVERLAPPEDWINDOW for windowed)
		0,                   // Initial x position
		0,                   // Initial y position
		0,                   // Initial width
		0,                   // Initial height
		NULL,                            // Parent window (NULL for top-level window)
		NULL,                            // Menu (NULL for no menu)
		GetModuleHandle(NULL),           // Instance handle
		this                             // Additional application data
	);

	if (!hwnd) {
		throw std::runtime_error("Failed to create message handling window.");
	}
	//hwnd = hwnd_instance;
	RegisterForDeviceNotifications();

	//SetTimer(hwnd, 1, 1000, NULL);
	SetTimer(NULL, 1, 1000, NULL);

}

void USBDeviceMonitorWindows::RegisterForDeviceNotifications() {
	std::cout << __func__ << ":start" << std::endl;
	DEV_BROADCAST_DEVICEINTERFACE dbdi = { 0 };
	dbdi.dbcc_size = sizeof(dbdi);
	dbdi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	//dbdi.dbcc_classguid = GUID_DEVINTERFACE_HID; // GUID for HID devices
	dbdi.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE; // GUID for HID devices


	device_notification_handle = RegisterDeviceNotification(
		hwnd,
		&dbdi,
		DEVICE_NOTIFY_WINDOW_HANDLE
	);

	if (!device_notification_handle) {
		std::cout << "device notification event registration failed..." << std::endl;
		throw std::runtime_error("Failed to register for device notifications.");
	}
	else {
		std::cout << "device notification event registration success..." << std::endl;
	}
	std::cout << __func__ << ":end" << std::endl;
}

void USBDeviceMonitorWindows::UnregisterDeviceNotifications() {
	if (device_notification_handle) {
		UnregisterDeviceNotification(device_notification_handle);
		device_notification_handle = nullptr;
	}
}

// The destructor 
USBDeviceMonitorWindows::~USBDeviceMonitorWindows() {
	stop();
	if (hwnd) {
		DestroyWindow(hwnd);
	}

	UnregisterDeviceNotifications();
	hid_exit();
}

// implementation of start overriden function
void USBDeviceMonitorWindows::start() {
	is_running = true;
	monitor_thread = std::thread(&USBDeviceMonitorWindows::MessageLoop, this);
}

// implementation of stop overriden function
void USBDeviceMonitorWindows::stop() {
	is_running = false;
	PostMessage(hwnd, WM_QUIT, 0, 0);
	if (monitor_thread.joinable())
	{
		std::cout << "wating to terminate the monitor thread..." << std::endl;
		monitor_thread.join();
	}
	std::cout << "Monitoring stopped..." << std::endl;

}

void USBDeviceMonitorWindows::MessageLoop() {

	MSG msg;
	std::cout << "Waiting for hardware events..." << std::endl;

	// Main message loop
	while (is_running) {
		std::cout << "Running....1" << std::endl;
		if (GetMessage(&msg, NULL, 0, 0)) {
			std::cout << "message recived..." << std::endl;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			break;  // Exit loop if WM_QUIT is received
		}
		std::cout << "Running....2" << std::endl;
	}

	std::cout << "Exited MessageLoop" << std::endl;


}

// implementation of the event callback 
LRESULT CALLBACK USBDeviceMonitorWindows::HWNDWindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	std::cout << __func__ << ":start" << std::endl;
	if (uMsg == WM_DEVICECHANGE) {
		std::cout << "WM_DEVICECHANGE message received.." << std::endl;
		auto* self = reinterpret_cast<USBDeviceMonitorWindows*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (self) {
			self->HandleDeviceChange(wParam, lParam);
		}
	}
	else if (uMsg == WM_CREATE) {
		std::cout << "WM_CREATE message received.." << std::endl;
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		auto* self = reinterpret_cast<USBDeviceMonitorWindows*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
	}
	else if (uMsg == WM_TIMER) {
		std::cout << "Timer event received" << std::endl;
	}
	std::cout << __func__ << ":end" << std::endl;
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// catch the device change and handle next step
void USBDeviceMonitorWindows::HandleDeviceChange(WPARAM wParam, LPARAM) {
	if (wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE) {
		std::cout << "Device change detected on Windows..." << std::endl;
		EnumerateDevices();
	}
}

// member function to enumerate the connected HID devices.
void USBDeviceMonitorWindows::EnumerateDevices() {
	struct hid_device_info* devs = hid_enumerate(0x0, 0x0);
	struct hid_device_info* cur_dev = devs;
	while (cur_dev) {
		std::wcout << L"Device Found: " << cur_dev->manufacturer_string << L" - " << cur_dev->product_string << std::endl;
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
}