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

// implementation of start overriden function
void USBDeviceMonitorWindows::start() {
	is_running = true;
	monitor_thread = std::thread(&USBDeviceMonitorWindows::MessageLoop, this);
}

// implementation of stop overriden function
void USBDeviceMonitorWindows::stop() {
	is_running = false;
	if (monitor_thread.joinable()) monitor_thread.join();
}

void USBDeviceMonitorWindows::MessageLoop() {
	MSG msg;
	while (is_running && GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// implementation of the event callback 
LRESULT CALLBACK USBDeviceMonitorWindows::HWNDWindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DEVICECHANGE) {
		auto* self = reinterpret_cast<USBDeviceMonitorWindows*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (self) self->HandleDeviceChange(wParam, lParam);
	}
	else if (uMsg == WM_CREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		auto* self = reinterpret_cast<USBDeviceMonitorWindows*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// catch the device change and handle next step
void USBDeviceMonitorWindows::HandleDeviceChange(WPARAM wParam, LPARAM) {
	if (wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE) {
		std::cout << "Device change detected on Windows!" << std::endl;
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