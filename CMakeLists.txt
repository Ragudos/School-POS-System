# From ChatGPT

cmake_minimum_required(VERSION 3.10)
project(school_pos_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)


option(ENVIRONMENT "Choose build environment (development or production)" "development")

if (ENVIRONMENT STREQUAL "development")
    add_definitions(-DDEV_BUILD)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
elseif (ENVIRONMENT STREQUAL "production")
    add_definitions(-DPROD_BUILD)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|MSVC")
        if(MSVC)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /LTCG")
        else()
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /flto")
        endif()
    endif()
else()
    message(FATAL_ERROR "Invalid environment specified. Choose 'development' or 'production'")
endif()

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
set(INCLUDE_DEFINITION_SRCS
    ${SRC_DIR}/utils.cpp
    ${SRC_DIR}/keyboard.cpp
    ${SRC_DIR}/looplambda.cpp
    ${SRC_DIR}/screen.cpp
    ${SRC_DIR}/ast/node.cpp
    ${SRC_DIR}/renderer.cpp
    ${SRC_DIR}/contrib/menu.cpp
    ${SRC_DIR}/contrib/state.cpp
    ${SRC_DIR}/contrib/utils.cpp
)
set(TEST_SRCS ${TEST_DIR}/main_test.cpp)

# Add main executable
add_executable(main ${SRCS} ${INCLUDE_DEFINITION_SRCS})

# Add test executable
add_executable(main_test ${TEST_SRCS} ${INCLUDE_DEFINITION_SRCS})

# Specify compilation flags
target_compile_options(main PRIVATE)
target_compile_options(main_test PRIVATE)

# Specify output directory for binaries
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR})
set_target_properties(main_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR})
