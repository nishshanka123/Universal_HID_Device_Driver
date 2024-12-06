#include "USBDeviceMonitor.h"

void USBDeviceMonitor::createDeviceConfiguration(std::string fileName, std::string config) {
    try {
        std::fstream configFstream(fileName, std::ios_base::out);
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

std::string USBDeviceMonitor::getConfiguration(std::string fileName) {
    std::string config;
    try {
        std::fstream configFstream(fileName, std::ios_base::in);
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