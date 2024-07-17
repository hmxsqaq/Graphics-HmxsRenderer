cmake_minimum_required(VERSION 3.28)
project(HmxsRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(HmxsRenderer main.cpp
        display/win32_wnd.cpp
        display/win32_wnd.h
        core/color.h
        core/buffer.cpp
        core/buffer.h
        core/renderer.cpp
        core/renderer.h
        maths/vector.h
        maths/maths.h
        maths/matrix.h)

set_target_properties(HmxsRenderer PROPERTIES WIN32_EXECUTABLE YES)