#include "DeviceMonitor.h"

DeviceMonitor::DeviceMonitor() : is_running(false), ctx(nullptr) {
    if (libusb_init(&ctx) != 0) {
        throw std::runtime_error("Failed to initialize libusb.");
    }
    std::cout << "libusb initialized..." << std::endl;
}

DeviceMonitor::~DeviceMonitor() {
    stop();
    libusb_exit(ctx);
}

/**
* View configurations from the config file
*/
std::string DeviceMonitor::getConfiguration() {
    std::string config;
    try {
        std::fstream configFstream("device.config", std::ios_base::in);
        if (configFstream.is_open())
        {
            while (configFstream.good()) {
                configFstream >> config;
            }
            configFstream.close();
        }
    }
    catch (std::exception& ex) {
        std::cout << "Exception occurred: " << ex.what() << std::endl;
    }

    return config;
}

/**
* Create configuration for device search criteria for device detection
*/
void DeviceMonitor::createDeviceConfiguration(std::string config) {
    try {
        std::fstream configFstream("device.config", std::ios_base::out);
        if (configFstream.is_open())
        {
            configFstream << config;
            configFstream.close();
        }
        else {
            std::cout << "Failed to save configs: Config file is not opened." << std::endl;
        }

    }
    catch (std::exception& ex) {
        std::cout << "Exception occurred : " << ex.what() << std::endl;
    }
}


void DeviceMonitor::start() {
    is_running = true;
    monitor_thread = std::thread(&DeviceMonitor::MonitorLoop, this);
}

void DeviceMonitor::stop() {
    is_running = false;
    if (monitor_thread.joinable()) {
        monitor_thread.join();
    }
    std::cout << "Monitoring stopped..." << std::endl;
}

void DeviceMonitor::MonitorLoop() {
    std::set<std::string> previous_devices;

    while (is_running) {
        std::set<std::string> current_devices = GetConnectedDevices();

        // Check for new devices
        for (const auto& device : current_devices) {
            if (previous_devices.find(device) == previous_devices.end()) {
                std::cout << "New device connected: " << device << std::endl;
            }
        }

        // Check for removed devices
        for (const auto& device : previous_devices) {
            if (current_devices.find(device) == current_devices.end()) {
                std::cout << "Device disconnected: " << device << std::endl;
            }
        }

        previous_devices = std::move(current_devices);

        // Sleep for a short period to avoid constant polling
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

/**
* Discover the connected devices by iterating the device list.
* for the libusb APIs : https://libusb.sourceforge.io/api-1.0/libusb_api.html
*/
std::set<std::string> DeviceMonitor::GetConnectedDevices() {
    std::set<std::string> device_set;
    libusb_device** list;
    ssize_t count = libusb_get_device_list(ctx, &list);

    for (ssize_t i = 0; i < count; ++i) {
        libusb_device* device = list[i];
        libusb_device_descriptor desc;

        if (libusb_get_device_descriptor(device, &desc) == 0) {
            // Assign an ID using the combination of Vendor ID and Product ID for the device.
            std::string device_id = std::to_string(desc.idVendor) + ":" + std::to_string(desc.idProduct);
            device_set.insert(device_id);

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
        }
    }

    libusb_free_device_list(list, 1);
    return device_set;
}