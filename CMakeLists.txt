cmake_minimum_required(VERSION 3.28)
project(HmxsRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(HmxsRenderer main.cpp
        win32wnd.cpp
        win32wnd.h)

set_target_properties(HmxsRenderer PROPERTIES WIN32_EXECUTABLE YES)