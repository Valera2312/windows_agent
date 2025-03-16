cmake_minimum_required(VERSION 3.28)
project(windows_agent)

# Set system and compiler
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Set C++ standard without macOS-specific flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Remove macOS-specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add Windows-specific definitions
add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DWIN32_LEAN_AND_MEAN)

add_executable(windows_agent src/main.cpp
        src/metrics/metrics.cpp
        src/metrics/metrics.h
        src/transmission/transmission.cpp
        src/transmission/transmission.h)

# Link Windows-specific libraries
target_link_libraries(windows_agent
        pdh
        ws2_32
)

# Include directories for MinGW headers
target_include_directories(windows_agent PRIVATE
        /opt/homebrew/opt/mingw-w64/include
)

