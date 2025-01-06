# Universal_HID_Device_Driver
platform independent device driver for HID devices to connect with a computer. This is a proof of concept to prove it is possible to develop platform independent and cross platform supported device driver for Humand Interface Devices, hence no rich GUI interfaces are used and console interface is used to provide userfriendly operations on the driver. Driver is consist of 2 modules.
1. Device monitoring and detection
2. Data transfer
device detection is done with the use of platform independent machanisam and design is extendable to implement platform specific solutions as well. Build tool used for the project is CMake and it can be build in Windows and Linux without making any change.

# Build and Run the project in Windows.
1. Download the GIT source project from https://github.com/nishshanka123/Universal_HID_Device_Driver to your windows PC
2. Open the project using visual studio 2022 or any available version greate than 2017
3. Click on Build->BuildAll from menu, Project will build now.
4. Click on Debug->StartWithoutDebugging to run the driver

# Build and Run the project in Linux
1. Download the GIT source project from https://github.com/nishshanka123/Universal_HID_Device_Driver to your Linux PC
2. Open your terminal and change the directory to project home directory
3. Change directory to Universal_HID_Device_Driver\UniversalDriver directory.
4. Create a directory call "build" if it is not aready there inside UniversalDriver directory
5. Change the directory to "build"
6. Run "cmake .." to generate the build Make Files 
7. Run "cmake --build ." to build the project
8. Now executable will be available inside the "build" directory. Run the executable with supper user mode as driver shoud transfer the data.

# Dependencies 
1. For windows platform, dependent libraries are part of the source project itself. So no additional libraries to be installed.
2. For Linux libusb, libudev should be available in the system

# Application menu
=========================================
== Platform Independent USB HID Driver ==
=========================================
== 1 - Start device monitoring          =
== 2 - Stop device monitoring           =
== 3 - Add device search criteria       =
== 4 - View device search criteria      =
== 5 - Connect with device              =
== 6 - Send commands to device          =
== 9 - Main Menu                        =
=========================================
== 0 - Quit...:).                       =
== Enter menu ID :

user can enter the menu item to do driver operations. Self help menu will nevigate to sub menus based on the operation.

