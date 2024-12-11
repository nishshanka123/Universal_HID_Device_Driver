// UniversalDriver.cpp : Defines the entry point for the application.
//

#include "UniversalDriver.h"
#include "external/libusb-1.0.27/include/libusb.h"

// USBHidDevice for the application
USBHidDevice* device = nullptr;

// callback function to process the received data
void processReceivedData(const std::vector<uint8_t>& data) {
	std::cout << "Data received: ";
	for (uint8_t element : data) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << int(element);
	}
	std::cout << std::endl;

}

void appSelfhelpMenu(const std::unique_ptr<USBDeviceMonitor> dMonitor = nullptr)
{
	int choice = 1;
	std::string config_file = "device.config";
	while (choice != 0)
	{
		std::cout << std::endl << "=========================================" << std::endl;
		std::cout << "== Platform Independent USB HID Driver ==" << std::endl;
		std::cout << "=========================================" << std::endl;
		std::cout << "== 1 - Start device monitoring          =" << std::endl;
		std::cout << "== 2 - Stop device monitoring           =" << std::endl;
		std::cout << "== 3 - Add device search criteria       =" << std::endl;
		std::cout << "== 4 - View device search criteria      =" << std::endl;
		std::cout << "== 5 - Connect with device              =" << std::endl;
		std::cout << "== 6 - Send commands to device          =" << std::endl;
		std::cout << "== 9 - Main Menu                        =" << std::endl;
		std::cout << "=========================================" << std::endl;
		std::cout << "== 0 - Quit...:).                       =" << std::endl;
		std::cout << "== Enter menu ID : " << std::endl;

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
			
			if (dMonitor) {
				dMonitor->createDeviceConfiguration(config_file, vid_pid);
			}
			break;
		}
		case 4: {
			if (dMonitor) {
				std::cout << "Current device search criteria (VID:PID): " << dMonitor->getConfiguration(config_file) << std::endl;
				std::cout << std::endl;
			}
			break;
		}
		case 5: {
			std::string vid_pid = dMonitor->getConfiguration(config_file);
			std::cout << "vid: " << std::stoi(vid_pid.substr(0, (vid_pid.find(":") + 1))) << "pid: " << std::stoi(vid_pid.substr(vid_pid.find(":") + 1, (vid_pid.length() - vid_pid.find(":") + 1) )) << std::endl;
			uint16_t vid = std::stoi(vid_pid.substr(0, (vid_pid.find(":") + 1)));
			uint16_t pid = std::stoi(vid_pid.substr(vid_pid.find(":") + 1, (vid_pid.length() - vid_pid.find(":") + 1)));

			device = new USBHidDevice (pid, vid);
			//device = hidDevice;
			try {
				if (device->initializeDevice()) {
					std::cout << "Device connected and initialized..." << std::endl;
					device->startRecieving(processReceivedData);
				}
			}
			catch (std::exception& ex) {
				std::cout << "Exception occurred: " << ex.what() << std::endl;
			}
			//hidDevice = new USBHidDevice(pid, vid);
			//delete hidDevice;

			std::cout << "vid: " << /*std::hex <<*/ vid << " pid: " << /*std::hex <<*/ pid << std::endl;
			std::vector<uint8_t> command = {0x00, 0x08, 0x05, 0x00, 0x17, 0x70, 0x58, 0x02};
			device->sendData(command);
			break;
		}
		case 6: {
			int sub_choice = 100;
			//std::string dev_id;
			//std::cout << "Enter device ID: ";
			//std::cin >> dev_id;
			if(device) {
				std::cout << "Connected device " << device->getDeviceInfo() << std::endl;
			} else {
				std::cout << "Please connect with a device first using option 5 in the main menu." << std::endl;
			}

			while (sub_choice != 0) {
				std::cout << "=========================================" << std::endl;
				std::cout << "== Connect with device ==================" << std::endl;
				std::cout << "=========================================" << std::endl;
				std::cout << "== 1 - Beeb the device                  =" << std::endl;
				std::cout << "== 2 - Beeb the device                  =" << std::endl;
				std::cout << "== 3 - Tern LED OFF                     =" << std::endl;
				std::cout << "== 5 - Connect with device              =" << std::endl;
				std::cout << "== 0 - Quit to main menu                =" << std::endl;
				std::cout << "=========================================" << std::endl;
				std::cout << "== Enter menu ID : " << std::endl;

				std::cin >> sub_choice;

				switch (sub_choice)
				{
				case 1:
					{
						std::vector<unsigned char> command = {0x00, 0x08, 0x05, 0x00, 0x17, 0x70, 0x58, 0x02};
						if(device){
							try{
								device->sendData(command);
							} catch (std::exception& ex) {
								std::cout << "Exception occurred: " << ex.what() << std::endl;
							}
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		}
		case 9: {
			std::cout << "=========================================" << std::endl;
			std::cout << "== Platform Independent USB HID Driver ==" << std::endl;
			std::cout << "=========================================" << std::endl;
			std::cout << "== 1 - Start device monitoring          =" << std::endl;
			std::cout << "== 2 - Stop device monitoring           =" << std::endl;
			std::cout << "== 3 - Add device search criteria       =" << std::endl;
			std::cout << "== 4 - View device search criteria      =" << std::endl;
			std::cout << "== 5 - Connect with device              =" << std::endl;
			std::cout << "=========================================" << std::endl;
			std::cout << "== 0 - Quit...:).                       =" << std::endl;
			std::cout << "== Enter menu ID : " << std::endl;
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

		std::cout << std::endl << "=========================================" << std::endl;
	}
}

void appSelfhelpMenu(const std::unique_ptr<DeviceMonitor> dMonitor = nullptr)
{
	int choice = 1;
	while (choice != 0)
	{
		// add menu items for the console self help application.
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
	USBHidDevice* hidDevice = new USBHidDevice();
	// Platform specific approach with cross compilable implementation.
	try
	{
		std::unique_ptr<USBDeviceMonitor> deviceMonitor(USBDeviceMonitorFactory::Create());
		if (deviceMonitor) {
			//deviceMonitor->start();
			//Display the main menu, wait here for interruption
			appSelfhelpMenu(std::move(deviceMonitor));
		}
		else {
			std::cout << "Unable to start the Monitor" << std::endl;
		}
	}
	catch (std::exception ex) {
		std::cout << "Exception occurred: " << ex.what() << std::endl;
	}
	

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
	//std::unique_ptr<DeviceMonitor> dMonitor = std::make_unique<DeviceMonitor>();
	//dMonitor->start();
	//appSelfhelpMenu(std::move(dMonitor));

	if (device) {
		delete device;
		device = nullptr;
	}
	return 0;
}
