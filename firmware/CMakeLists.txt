
# https://github.com/MaJerle/stm32-cube-cmake-vscode?tab=readme-ov-file

cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})


project(drive_firmware)
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
#
set(CPU_PARAMETERS
    -mthumb

    # This depends on the specific STM32 MCU you are using
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Add subdirectories for bootloader and application
add_subdirectory(Bootloader)
add_subdirectory(Core)

# Define paths to the output binaries.
# Assume bootloader produces target "bootloader" and application produces target "application".
# The following variables refer to the ELF outputs (you can then use objcopy to convert them into binary format).
set(BOOTLOADER_ELF $<TARGET_FILE:bootloader>)
set(APPLICATION_ELF $<TARGET_FILE:application>)
# Create custom commands to convert each ELF to a HEX file.
# Using the standard objcopy tool which is typically part of the gcc arm toolchain.
add_custom_command(
  OUTPUT bootloader.hex
  COMMAND arm-none-eabi-objcopy -O ihex ${BOOTLOADER_ELF} bootloader.hex
  DEPENDS bootloader
  COMMENT "Converting bootloader ELF to HEX"
)

add_custom_command(
  OUTPUT application.hex
  COMMAND arm-none-eabi-objcopy -O ihex ${APPLICATION_ELF} application.hex
  DEPENDS application
  COMMENT "Converting application ELF to HEX"
)

# create the binary for field firmware updates
add_custom_command(
  OUTPUT application.bin
  COMMAND arm-none-eabi-objcopy -O binary ${APPLICATION_ELF} application.bin
  DEPENDS application
  COMMENT "Converting application ELF to BIN"
)

add_custom_target(convert_hex ALL DEPENDS bootloader.hex application.hex application.bin)

# Path to the generated header
set(GENERATED_HDR "${PROJ_PATH}/Core/Inc/device_descriptor.h")

# Choose the Python interpreter inside the venv
if(WIN32)
    set(PYTHON_EXE ".venv/Scripts/python")
else()
    set(PYTHON_EXE ".venv/bin/python")
endif()

# Create a custom target that fuses bootloader and application into one final binary.
add_custom_command(
  OUTPUT combined.hex
  COMMAND ${CMAKE_COMMAND} -E echo "Merging binaries with merge_hex.py..."
  COMMAND ${PROJ_PATH}/../python/${PYTHON_EXE} ${PROJ_PATH}/../python/merge_hex.py 
          bootloader.hex application.hex combined.hex
  DEPENDS bootloader.hex application.hex
  COMMENT "Creating combined binary image"
)

add_custom_target(combined ALL DEPENDS combined.hex)

# Generate device_descriptor.h using Python autogen script
add_custom_command(
    OUTPUT  ${GENERATED_HDR}
    COMMAND ${PYTHON_EXE} autogen.py
    DEPENDS ${PROJ_PATH}/../python/device_descriptor.yaml
    WORKING_DIRECTORY ${PROJ_PATH}/../python
    COMMENT "Generating device_descriptor.h"
    VERBATIM
)

# Target that represents the generated file
add_custom_target(generate_device_descriptor_h ALL
                  DEPENDS ${GENERATED_HDR})

# Main target needs the header first
add_dependencies(application generate_device_descriptor_h)