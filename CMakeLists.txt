cmake_minimum_required(VERSION 3.28)
project(windows_agent)

set(CMAKE_CXX_STANDARD 23)

add_executable(windows_agent src/main.cpp
        src/metrics/metrics.cpp
        src/metrics/metrics.h)
# Link necessary libraries
target_link_libraries(windows_agent Pdh)


