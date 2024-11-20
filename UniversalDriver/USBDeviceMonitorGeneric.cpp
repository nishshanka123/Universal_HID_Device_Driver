#include "USBDeviceMonitorGeneric.h"

USBDeviceMonitorGeneric::USBDeviceMonitorGeneric() :is_running(false), ctx(nullptr) {
	std::cout << "Generic device monitor..." << std::endl;
	if (libusb_init(&ctx) != 0)
	{
		throw std::runtime_error("Exeception : failed to initilize libusb context");
	}
}

USBDeviceMonitorGeneric::~USBDeviceMonitorGeneric() {
	stop();
	libusb_exit(ctx);
}


void USBDeviceMonitorGeneric::start() {
	is_running = true;
	std::cout << std::endl << "Start device monitoring..." << std::endl;
	try
	{
		monitor_thread = std::thread(&USBDeviceMonitorGeneric::monitorLoop, this);
	}
	catch (std::exception& ex) {
		std::cout << "Exception occurred: " << ex.what() << std::endl;
	}	
}

void USBDeviceMonitorGeneric::stop() {
	is_running = false;
	std::cout << std::endl << "Stop device monitoring" << std::endl;
	try {
		if (monitor_thread.joinable()) {
			monitor_thread.join();
		}
	}
	catch (std::exception& ex) {
		std::cout << "Exception occurred: " << ex.what() << std::endl;
	}
}

/**
* Monitor the device changes in a continuous loop
*/
void USBDeviceMonitorGeneric::monitorLoop() {
	std::set<std::string> previous_devices;
	std::cout << "Start device monitoring..." << std::endl;
	while (is_running)
	{
		std::set<std::string> current_devices = getConnectedDevices();
		// check for new devices connections
		for (auto device : current_devices) {
			if (previous_devices.find(device) == previous_devices.end())
			{
				std::cout << "Device connected(VID:PID) : " << device << std::endl;
			}
		}

		// check for device removals
		for (auto device : previous_devices) {
			if (current_devices.find(device) == current_devices.end()) {
				std::cout << "Device removed(VID:PID): " << device << std::endl;
			}
		}
		previous_devices = std::move(current_devices);

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

/**
* Get the connected devices at any time
*/
std::set<std::string> USBDeviceMonitorGeneric::getConnectedDevices() {
	//std::cout << __func__ << ":start" << std::endl;
	std::set<std::string> device_set;
	libusb_device** libusb_device_list; // get the unique device list.
	ssize_t device_count = libusb_get_device_list(ctx, &libusb_device_list);

	//std::cout << "Device count: " << device_count << std::endl;

	for (int x = 0; x < device_count; x++) {
		libusb_device* device = libusb_device_list[x];
		libusb_device_descriptor desc;
		if (libusb_get_device_descriptor(device, &desc) == 0)
		{
			// Print the device descriptor info for debug purpose, 
			/*std::cout << "Device Descriptor:" << std::endl;
			std::cout << "  bLength: " << static_cast<int>(desc.bLength) << std::endl;
			std::cout << "  bDescriptorType: " << static_cast<int>(desc.bDescriptorType) << std::endl;
			std::cout << "  bcdUSB: " << desc.bcdUSB << std::endl;
			std::cout << "  bDeviceClass: " << static_cast<int>(desc.bDeviceClass) << std::endl;
			std::cout << "  bDeviceSubClass: " << static_cast<int>(desc.bDeviceSubClass) << std::endl;
			std::cout << "  bDeviceProtocol: " << static_cast<int>(desc.bDeviceProtocol) << std::endl;
			std::cout << "  bMaxPacketSize0: " << static_cast<int>(desc.bMaxPacketSize0) << std::endl;
			std::cout << "  idVendor: 0x" << std::hex << desc.idVendor << std::dec << std::endl;
			std::cout << "  idProduct: 0x" << std::hex << desc.idProduct << std::dec << std::endl;
			std::cout << "  bcdDevice: " << desc.bcdDevice << std::endl;
			std::cout << "  iManufacturer: " << static_cast<int>(desc.iManufacturer) << std::endl;
			std::cout << "  iProduct: " << static_cast<int>(desc.iProduct) << std::endl;
			std::cout << "  iSerialNumber: " << static_cast<int>(desc.iSerialNumber) << std::endl;
			std::cout << "  bNumConfigurations: " << static_cast<int>(desc.bNumConfigurations) << std::endl;*/

			std::string device_id = /*std::to_string(x + 1) + ":" + */std::to_string(desc.idVendor) + ":" + std::to_string(desc.idProduct);
			device_set.insert(device_id);			
		}
		else {
			std::cout << "Unable to get the device descriptor" << std::endl;
		}
	}

	libusb_free_device_list(libusb_device_list, 1);
	//std::cout << __func__ << ":end" << std::endl;
	return device_set;
}
