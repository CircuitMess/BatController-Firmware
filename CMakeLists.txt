cmake_minimum_required(VERSION 3.21)
project(BatController-Firmware)
set(PORT /dev/ttyUSB0)
set(DEVICE cm:esp8266:nibble)
set(CMAKE_CXX_STANDARD 14)

set(ROOT ${CMAKE_SOURCE_DIR})
if (WIN32 OR CYGWIN)
    set(Arduino15 $ENV{APPDATA}/../Local/Arduino15)
    set(ArduinoLibs $ENV{USERPROFILE}/OneDrive/Documents/Arduino/libraries)

    string(REPLACE "\\" "/" Arduino15 ${Arduino15})
    string(REPLACE "\\" "/" ArduinoLibs ${ArduinoLibs})

    if (CYGWIN)
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" Arduino15 ${Arduino15})
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" ArduinoLibs ${ArduinoLibs})
        string(REGEX REPLACE "^/cygdrive/([A-Za-z])" "\\1:/" ROOT ${ROOT})
    endif ()
else ()
    set(Arduino15 ~/.arduino15)
    set(ArduinoLibs ~/Arduino/libraries)
endif ()

include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/cores/esp8266)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/esp8266)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/freertos)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/spi_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/nvs_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/log)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/driver)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/tools/sdk/include/driver/driver)

# Include directories
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/Wire/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/FS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/SD/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/SPI/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/WiFi/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/SPIFFS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/LovyanGFX/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/CircuitOS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/Nibble/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/Update/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/CircuitOS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/glm)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/Hash/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/ESP8266WiFi/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/ESP8266WebServer/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/ESPAsyncTCP/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/ESPAsyncWebServer/src)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/TFT_eSPI)
include_directories(${Arduino15}/packages/cm/hardware/esp8266/1.1.0/libraries/TFT_eSPI/Extensions)

include_directories(${ArduinoLibs}/BatController-Library/src)

# Source directories
file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.c" "src/**.hpp" "src/**.h")

# Defines
add_definitions(-DDEBUG)
add_definitions(-DESP8266)
add_definitions(-DCIRCUITOS_FREERTOS)
add_definitions(-DCIRCUITOS_NVS)
add_definitions(-DCIRCUITOS_LOVYANGFX)
add_definitions(-DCIRCUITOS_PIEZO_ESP8266)
add_definitions(-DCIRCUITOS_PIEZO_ESP8266_PWM_CHANNEL=0)
add_definitions(-DRADIOLIB_BUILD_ARDUINO)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(BUILD ${ROOT}/build)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_PROJECT_NAME}.ino PROPERTIES LANGUAGE CXX)

add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf
        COMMAND arduino-cli compile --fqbn ${DEVICE} --output-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_PROJECT_NAME}.ino
        DEPENDS ${CMAKE_PROJECT_NAME}.ino ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(CMBuild
        SOURCES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf)

add_custom_target(CMUpload ALL
        COMMAND arduino-cli upload  --fqbn ${DEVICE} -p ${PORT} --input-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS CMBuild
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME}.bin ${CMAKE_PROJECT_NAME}.ino ${SOURCES})

