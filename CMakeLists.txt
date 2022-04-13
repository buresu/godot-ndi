cmake_minimum_required(VERSION 3.7)

project(godot-ndi VERSION 1.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#include_directories("C:/Program Files/NewTek/NewTek NDI 3.8 SDK/Include")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_library(NDIPlugin SHARED ${SOURCES})

target_link_libraries(NDIPlugin godot-cpp)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/godot-cpp")

#install(TARGETS NDIPlugin RUNTIME DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/plugin.cfg DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/plugin.gd DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDIPlugin.gdnlib DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDIReceiver.gdns DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDISender.gdns DESTINATION NDIPlugin)
