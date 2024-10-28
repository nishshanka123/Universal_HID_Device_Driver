# CMakeList.txt : CMake project for UniversalDriver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("UniversalDriver")

# Set the path to the HIDAPI include directory and library file
set(HIDAPI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/hidapi/include")
#set(HIDAPI_LIBRARY "${CMAKE_SOURCE_DIR}/hidapi/x64/hidapi.lib")

if (WIN32)
    set(HIDAPI_LIBRARY "${CMAKE_SOURCE_DIR}/external/hidapi/x64/hidapi.lib")
elseif (UNIX)
    set(HIDAPI_LIBRARY "${CMAKE_SOURCE_DIR}/external/hidapi/lib/libhidapi.so")
endif()

# Add source to this project's executable.
if (WIN32)
    add_executable (UniversalDriver "UniversalDriver.cpp" "UniversalDriver.h" "DeviceMonitor.h"  "USBDeviceMonitorWindows.cpp" "USBDeviceMonitor.cpp" "USBDeviceMonitor.h" "USBDeviceMonitorWindows.h" "USBDeviceMonitorFactory.h" "USBDeviceMonitorFactory.cpp")
elseif (UNIX)
    add_executable (UniversalDriver "UniversalDriver.cpp" "UniversalDriver.h" "DeviceMonitor.h"  "USBDeviceMonitorWindows.cpp" "USBDeviceMonitor.cpp" "USBDeviceMonitor.h" "USBDeviceMonitorWindows.h" "USBDeviceMonitorLinux.h" "USBDeviceMonitorLinux.cpp")
endif()


# Link HIDAPI library
target_link_libraries(UniversalDriver PRIVATE ${HIDAPI_LIBRARY})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET UniversalDriver PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
