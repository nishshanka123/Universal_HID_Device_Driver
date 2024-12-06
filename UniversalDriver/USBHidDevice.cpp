#include "USBHidDevice.h"

USBHidDevice::USBHidDevice() :productId(0), vendorId(0) {
	isInprogress = false;
	isOpen = false;
	hidDeviceHandler = nullptr;
}

USBHidDevice::USBHidDevice(uint16_t pid, uint16_t vid) :productId(pid), vendorId(vid) {
	isInprogress = false;
	isOpen = false;
	hidDeviceHandler = nullptr;
}

USBHidDevice::~USBHidDevice() {
	std::cout << __func__ << "start" << std::endl;
	stopRecieving();
	closeDevice();
	std::cout << __func__ << "end" << std::endl;
}

bool USBHidDevice::initializeDevice() {
	// initialize the hid library for device creation.
	if (hid_init() != 0) {
		throw std::exception("HID library initialization failed.");
	}

	//open the hid device connection
	// hid_open(vendor_id, product_id, serial_no);
	hidDeviceHandler = hid_open(vendorId, productId, nullptr);
	if (!hidDeviceHandler)
	{
		throw std::exception("Failed to open hid device for the device with given vendor ID and Product ID");
	}

	isOpen = true;

	return true;
}

void USBHidDevice::closeDevice() {

	// 1. stop if there are active reading threads for the device.

	// 2. close the device if open
	if (isOpen) {
		if (hidDeviceHandler) {
			hid_close(hidDeviceHandler);
			hidDeviceHandler = nullptr;
			isOpen = false;
			hid_exit();
		}
	}

}

bool USBHidDevice::isDeviceOpen() {
	return isOpen;
}

void USBHidDevice::sendData(std::vector<uint8_t>& data) {
	if (!isOpen) {
		throw std::exception("Device is not open for data writing.");
	}

	// default report ID set to 0, first set all bytes to 0
	std::vector<unsigned char> outputReport(data.size() + 1, 0);
	//std::vector<unsigned char>oReport(data.size() + 1);
	std::copy(data.begin(), data.end(), outputReport.begin() + 1);
	
	size_t size = hid_write(hidDeviceHandler, outputReport.data(), outputReport.size());
	if (size < 0) {
		//throw std::exception("Failed to write to HID device: %s", hid_error(hidDeviceHandler));
		throw std::exception("Failed to write to HID device");
	}
}


size_t USBHidDevice::recieveData(std::vector<uint8_t>& data) {
	if (!isOpen) {
		throw std::exception("HID Device is not open for data reading.");
	}

	// allocate the buffer size for data reading. this is the input report size of the device.
	data.resize(64);
	int readTimeOut = 1000; // read timeout in ms

	// do nonblocking reading
	size_t readSize = hid_read_timeout(hidDeviceHandler, data.data(), data.size(), readTimeOut);
	if (readSize < 0) {
		//throw std::exception("HID data read error %s", hid_error(hidDeviceHandler));
		throw std::exception("HID data read error");
	}
	// print the data.
	return readSize;
}

void USBHidDevice::startRecieving(const std::function<void(const std::vector<uint8_t>&)>& callback) {
	if (isInprogress) {
		throw std::exception("Data reading thread is already started.");
	}

	if (!isOpen) {
		throw std::exception("Unable to start reading, device not opened");
	}

	dataCallback = callback;
	isInprogress = true;

	// start the recieving thread
	try {
		dataRecieveThread = std::thread(&USBHidDevice::receiveWorker, this);
	}
	catch (std::exception& ex) {
		std::cout << "Unable to start reading thread, exception: " << ex.what() << std::endl;
		throw ex;
	}
	
}

void USBHidDevice::receiveWorker() {
	while (isInprogress) {
		std::vector<uint8_t> rData;
		size_t recvSize = 0;
		try {
			recvSize = recieveData(rData);
			if (recvSize > 0) {
				if (dataCallback) {
					dataCallback(rData);
				}
			}
		}
		catch (std::exception& ex) {
			std::cout << "receiveWorker: Exception occurred: " << ex.what() << std::endl;
			throw ex;
		}
		//std::cout << "Reading...1" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//std::cout << "Reading...2" << std::endl;
	}
}

void USBHidDevice::stopRecieving() {
	std::cout << __func__ << ":start" << std::endl;
	if (isInprogress) {
		isInprogress = false;
		if (dataRecieveThread.joinable()) {
			dataRecieveThread.join();
		}
	}
	std::cout << __func__ << ":end" << std::endl;
}