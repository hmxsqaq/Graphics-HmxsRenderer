cmake_minimum_required(VERSION 3.28)
project(Graphics_HmxsRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Graphics_HmxsRenderer WIN32 src/main.cpp)

target_link_libraries(Graphics_HmxsRenderer PRIVATE core platform)

set(ASSETS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(ASSETS_DEST_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

add_custom_command(TARGET Graphics_HmxsRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
        COMMENT "Copying assets directory to binary directory"
)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Copying assets directory"
        DEPENDS Graphics_HmxsRenderer
)

target_compile_definitions(Graphics_HmxsRenderer PRIVATE
        ASSETS_PATH="${ASSETS_DEST_DIR}"
)
