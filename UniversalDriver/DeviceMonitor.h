/**
* class for implementing device monitor.
* purpose: monitor the hardware changes to detect plug and play and
* notify the interesting devices to the applications
* 
*/
#ifndef DEVICE_MONITOR_H
#define DEVICE_MONITOR_H

#include "external/libusb-1.0.24/include/libusb-1.0/libusb.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <set>
#include <fstream>
#include <map>
#include <string>

class DeviceMonitor {
public:
    DeviceMonitor();
    ~DeviceMonitor();
    void start();
    void stop();
    void EnumerateDevices();
    std::string getConfiguration();
    void createDeviceConfiguration(std::string config);

private:
    void MonitorLoop();
    std::set<std::string> GetConnectedDevices();
    std::map<std::string, std::string> deviceConfig;

    std::atomic<bool> is_running;
    std::thread monitor_thread;
    libusb_context* ctx;
};

#endif