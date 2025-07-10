
# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/Core/core.ld)
set(EXECUTABLE                      application)

#
# List of source files to compile
#
file(GLOB_RECURSE sources_SRCS
    ${PROJ_PATH}/Core/Src/*.cpp
    ${PROJ_PATH}/Core/Src/*.c
    ${PROJ_PATH}/Core/Startup/*.s
)

#
# Include directories
#
set(include_path_DIRS
    ${PROJ_PATH}/Core/Inc
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${PROJ_PATH}/Drivers/CMSIS/Include
)

#
# Symbols definition
#
set(symbols_SYMB
    "DEBUG"
    "STM32F413xx"
    "HSE_VALUE=16000000"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)