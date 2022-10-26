cmake_minimum_required(VERSION 3.15)
project(BatController-Firmware)
set(PORT /dev/ttyUSB0)
set(DEVICE cm:esp32:batctrl)
set(CMAKE_CXX_STANDARD 17)

if(WIN32 OR CYGWIN)
    set(Arduino15 $ENV{APPDATA}/../Local/Arduino15)
    set(ArduinoLibs $ENV{USERPROFILE}/Documents/Arduino/libraries)

    string(REPLACE "\\" "/" Arduino15 ${Arduino15})
    string(REPLACE "\\" "/" ArduinoLibs ${ArduinoLibs})

    if(${CYGWIN})
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" Arduino15 ${Arduino15})
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" ArduinoLibs ${ArduinoLibs})
    endif()
else()
    set(Arduino15 ~/.arduino15)
    set(ArduinoLibs /mnt/c/Users/emilg/Documents/Arduino/libraries)
endif()

include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/cores/esp32)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/esp32)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/freertos)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/spi_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/nvs_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/log)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/driver)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/tools/sdk/include/driver/driver)

# Include directories
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/Wire/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/FS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/SD/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/SPI/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/WiFi/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/SPIFFS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/LovyanGFX/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/Update/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/Chatter-Library/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/lvgl/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/glm)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/I2C_BM8563/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/AsyncUDP/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.6.1/libraries/AsyncTCP/src)


include_directories(${ArduinoLibs}/CircuitOS/src)
include_directories(${ArduinoLibs}/TJpg_Decoder/src)
include_directories(${ArduinoLibs}/JPEGDecoder/src)
include_directories(${ArduinoLibs}/JPEGDEC/src)
include_directories(${ArduinoLibs}/AsyncTCP/src)

include_directories(${ArduinoLibs}/BatController-Library/src)

file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.c" "src/**.hpp" "src/**.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_definitions(-DDEBUG)
add_definitions(-DESP32)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_PROJECT_NAME}.ino PROPERTIES LANGUAGE CXX)

add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf
        COMMAND arduino-cli compile --fqbn ${DEVICE} --output-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_PROJECT_NAME}.ino
        DEPENDS ${CMAKE_PROJECT_NAME}.ino ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(CMBuild
        SOURCES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf)

add_custom_target(CMUpload ALL
        COMMAND arduino-cli upload --fqbn ${DEVICE} -p ${PORT} --input-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS CMBuild
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME}.bin ${CMAKE_PROJECT_NAME}.ino ${SOURCES})