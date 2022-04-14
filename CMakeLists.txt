cmake_minimum_required(VERSION 3.7)

project(godot-ndi VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/godot-cpp")

find_package(NDI REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_library(NDIPlugin SHARED ${SOURCES})

target_include_directories(NDIPlugin PRIVATE ${NDI_INCLUDE_DIR})
target_link_directories(NDIPlugin PRIVATE ${NDI_LIBRARY_DIR})
target_link_libraries(NDIPlugin PRIVATE godot-cpp ${NDI_LIBS})

#install(TARGETS NDIPlugin RUNTIME DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/plugin.cfg DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/plugin.gd DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDIPlugin.gdnlib DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDIReceiver.gdns DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDISender.gdns DESTINATION NDIPlugin)
