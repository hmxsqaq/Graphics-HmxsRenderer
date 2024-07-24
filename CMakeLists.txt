cmake_minimum_required(VERSION 3.28)
project(HmxsRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(HmxsRenderer main.cpp
        maths/vector.h
        maths/maths.h
        maths/matrix.h
        win32_wnd.cpp
        win32_wnd.h
        color.h
        buffer.cpp
        buffer.h
        renderer.cpp
        renderer.h
        model.cpp
        model.h
        tga_handler.cpp
        tga_handler.h
        log.h
        component-gameobject.cpp
        component-gameobject.h
        ishader.cpp
        ishader.h
        scene.cpp
        scene.h)

set_target_properties(HmxsRenderer PROPERTIES WIN32_EXECUTABLE YES)