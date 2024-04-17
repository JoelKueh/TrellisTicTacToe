# Set min required version
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# Set project name
project(JChess VERSION 2.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(STDIO "Set ON if bluetooth commands should be routed to stdout" OFF)


# Set Debug
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

# Build bluetooth serial library
# add_library(bluetooth_serial src/bluetooth_serial/bluetooth_serial.cpp)
# target_link_libraries(bluetooth_serial ${BLUEZ_LIBRARY_DIRS})

# Build bluetooth-serial-port subdirectory
add_subdirectory(src/bluetooth-serial-port)

# Build bluetooth trellis library
add_library(blue_trellis src/blue_trellis/blue_trellis.cpp)
target_link_libraries(blue_trellis bluetoothserialport)
if (STDIO)
	target_compile_definitions(blue_trellis PRIVATE BLUE_STDIO)
endif (STDIO)

# Build game executable
add_executable(trellis_game src/trellis_game/trellis_main.cpp
	src/trellis_game/scene.cpp
	src/trellis_game/dummy_animation.cpp
	src/trellis_game/tictactoe.cpp
	src/trellis_game/mole_whacker.cpp
	src/trellis_game/calculator.cpp)
target_link_libraries(trellis_game blue_trellis)

# Build test executable
add_executable(trellis_test src/trellis_game/trellis_test.cpp)
target_link_libraries(trellis_test blue_trellis)

# Force CMake to output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)

