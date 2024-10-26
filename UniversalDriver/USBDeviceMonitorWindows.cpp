#include "USBDeviceMonitorWindows.h"
#include <iostream>

USBDeviceMonitorWindows::USBDeviceMonitorWindows() :is_running(false), hwnd(nullptr) {
	// Initialize HIDAPI
	if (hid_init() != 0) {
		throw std::runtime_error("Failed to initialize the HIDAPI.");
	}

	// register the windows class (current windows of the program) for message handling.
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = HWNDWindowProcess;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "USBDeviceMonitorWindowClass";

	// register the window class instance, throw exception if failed.
	if (!RegisterClass(&wc)) {
		throw std::runtime_error("Failed to register the app window class");
	}

	// then, create the windows, whole purpose of creating this is to receive the windows hardware event.
	HWND hwnd = CreateWindowEx(
		0,                               // Optional window styles
		"USBDeviceMonitorWindowClass",   // Window class name
		"USB Device Monitor",            // Window title
		0,                               // Window style (use 0 for no border, WS_OVERLAPPEDWINDOW for windowed)
		CW_USEDEFAULT,                   // Initial x position
		CW_USEDEFAULT,                   // Initial y position
		CW_USEDEFAULT,                   // Initial width
		CW_USEDEFAULT,                   // Initial height
		NULL,                            // Parent window (NULL for top-level window)
		NULL,                            // Menu (NULL for no menu)
		GetModuleHandle(NULL),           // Instance handle
		this                             // Additional application data
	);

	if (!hwnd) {
		throw std::runtime_error("Failed to create message handling window.");
	}
}

// The destructor 
USBDeviceMonitorWindows::~USBDeviceMonitorWindows() {
	stop();
	if (hwnd) DestroyWindow(hwnd);
	hid_exit();
}

void USBDeviceMonitorWindows::start() {
	is_running = true;
	monitor_thread = std::thread(&USBDeviceMonitorWindows::MessageLoop, this);
}

void USBDeviceMonitorWindows::stop() {
	is_running = false;
	if (monitor_thread.joinable()) monitor_thread.join();
}
