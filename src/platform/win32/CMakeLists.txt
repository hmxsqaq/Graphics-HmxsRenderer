add_library(platform win32_wnd.cpp)

target_include_directories(platform PUBLIC
        ${PROJECT_SOURCE_DIR}/include/platform/win32
        ${PROJECT_SOURCE_DIR}/include/core
)