# From ChatGPT

cmake_minimum_required(VERSION 3.10)
project(school_pos_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

# Detect the operating system
if(UNIX AND NOT APPLE)  # Linux
    message(STATUS "Configuring for Linux")
    add_definitions(-DLINUX_PLATFORM)
elseif(APPLE)  # macOS
    message(STATUS "Configuring for macOS")
    add_definitions(-DMAC_PLATFORM)
elseif(WIN32)  # Windows
    message(STATUS "Configuring for Windows")
    add_definitions(-DWINDOWS_PLATFORM)
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# Define directories
set(SRC_DIR src)
set(TEST_DIR __tests__)
set(BIN_DIR bin)

# Define source files
set(SRCS ${SRC_DIR}/main.cpp)
set(TEST_SRCS ${TEST_DIR}/main_test.cpp)

# Add main executable
add_executable(main ${SRCS})

# Add test executable
add_executable(main_test ${TEST_SRCS})

# Specify compilation flags
target_compile_options(main PRIVATE)
target_compile_options(main_test PRIVATE)

# Specify output directory for binaries
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR})
set_target_properties(main_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR})
