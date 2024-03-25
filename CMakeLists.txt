# Set min required version
cmake_minimum_required(VERSION 3.10)

# Set project name
project(JChess VERSION 2.0)

# Check for bluez development modules
# FindPkgConfig(BLUEZ REQUIRED bluez-5.72)

# Set Debug
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

# Build bluetooth serial library
add_library(bluetooth_serial src/bluetooth_serial/bluetooth_serial.c)
target_link_libraries(bluetooth_serial ${BLUEZ_LIBRARY_DIRS})

# Build bluetooth trellis library
add_library(blue_trellis src/blue_trellis/blue_trellis.c)
target_link_libraries(blue_trellis)

# Build game executable
add_executable(trellis_game src/trellis_game/trellis_game.c)
target_link_libraries(trellis_game blue_trellis)

# Force CMake to output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
