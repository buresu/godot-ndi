cmake_minimum_required(VERSION 3.12)

project(godot-ndi VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_PREFIX "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_PREFIX "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM_PREFIX "macos")
else()
    set(PLATFORM_PREFIX "unknown")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/godot-cpp")

find_package(NDI REQUIRED)

file(GLOB_RECURSE INCLUDES "src/*.hpp")
file(GLOB_RECURSE SOURCES "src/*.cpp")

source_group("" FILES ${SOURCES} ${INCLUDES})

add_library(godot-ndi SHARED ${SOURCES} ${INCLUDES})

target_include_directories(godot-ndi PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(godot-ndi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(godot-ndi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(godot-ndi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/gdextension)
target_include_directories(godot-ndi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/include)
target_include_directories(godot-ndi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/gen/include)
target_include_directories(godot-ndi PRIVATE ${NDI_INCLUDE_DIR})

target_link_directories(godot-ndi PRIVATE ${NDI_LIBRARY_DIR})
target_link_libraries(godot-ndi PRIVATE godot::cpp)
target_link_libraries(godot-ndi PRIVATE ${NDI_LIBS})

set_target_properties(godot-ndi PROPERTIES
    OUTPUT_NAME "godot-ndi.${PLATFORM_PREFIX}.$<LOWER_CASE:$<CONFIG>>.x86_64"
)

set_target_properties(godot-ndi PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(
    TARGET godot-ndi
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/res/ndi.gdextension" "${CMAKE_BINARY_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy "${NDI_LIBRARY_DIR}/libndi.so*" "${CMAKE_BINARY_DIR}/bin"
)
