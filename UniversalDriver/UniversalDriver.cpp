// UniversalDriver.cpp : Defines the entry point for the application.
//

#include "UniversalDriver.h"
#include "external/libusb-1.0.24/include/libusb-1.0/libusb.h"

//using namespace std;

void appSelfhelpMenu(const std::unique_ptr<USBDeviceMonitor> dMonitor)
{
	int choice = 1;
	while (choice != 0)
	{
		std::cout << "=========================================" << std::endl;
		std::cout << "== Platform Independent USB HID Driver ==" << std::endl;
		std::cout << "=========================================" << std::endl;
		std::cout << "== 1 - Refresh the device list" << std::endl;
		std::cout << "== 2 - Show device information" << std::endl;
		std::cout << "== 0 - Quit...:)." << std::endl;
		std::cout << "== Enter menu ID : ";

		std::cin >> choice;

		switch (choice)
		{
		case 1: {
			std::cout << "menu 1" << std::endl;
			break;
		}
		case 2: {
			std::cout << "menu 2" << std::endl;			
			break;
		}
		case 0:
			std::cout << "Quiting.." << std::endl;
			if (dMonitor)
			{
				std::cout << "Stopping device monitoring" << std::endl;
				dMonitor->stop();
				return;
			}
			break;
		defult:
			break;
		}

		std::cout << "=========================================" << std::endl;
	}
}

void appSelfhelpMenu(const std::unique_ptr<DeviceMonitor> dMonitor = nullptr)
{
	int choice = 1;
	while (choice != 0)
	{
		std::cout << "=========================================" << std::endl;
		std::cout << "== Platform Independent USB HID Driver ==" << std::endl;
		std::cout << "=========================================" << std::endl;
		std::cout << "== 1 - Start device monitoring" << std::endl;
		std::cout << "== 2 - Stop device monitoring" << std::endl;
		std::cout << "== 3 - Add device search criteria" << std::endl;
		std::cout << "== 4 - View device search criteria" << std::endl;
		std::cout << "== 0 - Quit...:)." << std::endl;
		std::cout << "== Enter menu ID : ";

		std::cin >> choice;

		switch (choice)
		{
		case 1: {
			if (dMonitor)
			{
				dMonitor->start();
			}
			else {
				std::cout << "Error: Device monitor is not initialized" << std::endl;
			}
			break;
		}
		case 2: {
			if (dMonitor)
			{
				std::cout << "Stopping device monitoring" << std::endl;
				dMonitor->stop();
			}
			else {
				std::cout << "Error: Device monitor is not initialized" << std::endl;
			}
			break;
		}
		case 3: {
			std::string vid_pid;
			std::cout << "Enter device search criteria (VID and PID in VID:PID format):";			
			std::cin >> vid_pid;
			dMonitor->createDeviceConfiguration(vid_pid);
			break;
		}
		case 4: {
			if (dMonitor) {
				std::cout << "Current device search criteria (VID:PID): " << dMonitor->getConfiguration() << std::endl;
			}
			break;
		}
		case 0:
			std::cout << "Quiting.." << std::endl;
			if (dMonitor)
			{
				std::cout << "Stopping device monitoring" << std::endl;
				dMonitor->stop();
				return;
			}
			else {
				return;
			}
			break;
		defult:
			break;
		}

		std::cout << "=========================================" << std::endl;
	}
}

int main()
{
	// Platform specific approach with cross compilable implementation.
	/*try
	{
		std::unique_ptr<USBDeviceMonitor> deviceMonitor(USBDeviceMonitorFactory::Create());
		if (deviceMonitor) {
			std::cout << "Device monitor created." << std::endl;
			std::cout << "Start monitoring..." << std::endl;
			deviceMonitor->start();
			// wait here for interruption
			appSelfhelpMenu(std::move(deviceMonitor));
		}
		else {
			std::cout << "Unable to start the Monitor" << std::endl;
		}
		//std::unique_ptr<USBDeviceMonitor> deviceMonitor = std::make_unique(USBDeviceMonitorFactory::Create());
	}
	catch (std::exception ex) {
		std::cout << "Exception occurred: " << ex.what() << std::endl;
	}*/
	

	// Verify libusb setup
	/*libusb_context* ctx = nullptr;
	int result = libusb_init(&ctx);

	if (result != 0) {
		std::cerr << "Failed to initialize libusb: " << libusb_error_name(result) << std::endl;
		return 1;
	}

	std::cout << "libusb initialized successfully." << std::endl;

	libusb_exit(ctx);
	*/

	// Platform independent approach with the use of libUSB
	std::unique_ptr<DeviceMonitor> dMonitor = std::make_unique<DeviceMonitor>();
	//dMonitor->start();
	appSelfhelpMenu(std::move(dMonitor));

	return 0;
}
