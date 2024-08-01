add_library(core
        buffer.cpp
        component-gameobject.cpp
        ishader.cpp
        model.cpp
        renderer.cpp
        scene.cpp
        tga_handler.cpp
)

target_include_directories(core PUBLIC
        ${PROJECT_SOURCE_DIR}/include/core
)