cmake_minimum_required(VERSION 3.28)
project(HmxsRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(HmxsRenderer main.cpp
        win32_wnd.cpp
        win32_wnd.h
        color.h)

set_target_properties(HmxsRenderer PROPERTIES WIN32_EXECUTABLE YES)