cmake_minimum_required(VERSION 3.1)

project(godot-spout VERSION 1.0.0)

option(BUILD_GDNATIVE "Build GDNative" ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITS "64")
else()
    set(BITS "32")
endif()

#find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories("C:/Program Files/NewTek/NewTek NDI 3.8 SDK/Include")
include_directories(${PROJECT_SOURCE_DIR}/lib/godot-cpp/godot_headers)
include_directories(${PROJECT_SOURCE_DIR}/lib/godot-cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/godot-cpp/include/core)
include_directories(${PROJECT_SOURCE_DIR}/lib/godot-cpp/include/gen)

if (BITS EQUAL 64)
    link_directories("C:/Program Files/NewTek/NewTek NDI 3.8 SDK/Lib/x64")
else()
    link_directories("C:/Program Files/NewTek/NewTek NDI 3.8 SDK/Lib/x86")
endif()
link_directories(${PROJECT_SOURCE_DIR}/lib/godot-cpp/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

#add_library(Spout ${SPOUT_SOURCES})
add_library(NDIPlugin SHARED ${SOURCES})

if (BITS EQUAL 64)
    target_link_libraries(NDIPlugin Processing.NDI.Lib.x64.lib optimized libgodot-cpp.windows.release.${BITS} debug libgodot-cpp.windows.debug.${BITS})
else()
    target_link_libraries(NDIPlugin Processing.NDI.Lib.x86.lib optimized libgodot-cpp.windows.release.${BITS} debug libgodot-cpp.windows.debug.${BITS})
endif()

if (BUILD_GDNATIVE)
    add_custom_command(
        TARGET NDIPlugin
        PRE_BUILD
        COMMAND scons platform=windows target=$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release> bits=${BITS} generate_bindings=yes
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/godot-cpp
    )
endif()

#install(TARGETS NDIPlugin RUNTIME DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/plugin.cfg DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/plugin.gd DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDIPlugin.gdnlib DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDIReceiver.gdns DESTINATION NDIPlugin)
#install(FILES ${PROJECT_SOURCE_DIR}/res/NDISender.gdns DESTINATION NDIPlugin)
