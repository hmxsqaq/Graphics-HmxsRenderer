cmake_minimum_required(VERSION 3.28)
project(Graphics_HmxsRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(Graphics_HmxsRenderer main.cpp
        utility/log.h
        utility/frame_timer.h
        utility/frame_timer.cpp
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
        component-gameobject.cpp
        component-gameobject.h
        ishader.cpp
        ishader.h
        scene.cpp
        scene.h
        callbacks.h)

set_target_properties(Graphics_HmxsRenderer PROPERTIES WIN32_EXECUTABLE YES)