cmake_minimum_required(VERSION 3.12)

project(godot-ndi VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

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

if(WIN32)
    set_target_properties(godot-ndi PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set_target_properties(godot-ndi PROPERTIES OUTPUT_NAME "godot-ndi.windows.$<LOWER_CASE:$<CONFIG>>.x86_64")
elseif(APPLE)
    set_target_properties(godot-ndi PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set_target_properties(godot-ndi PROPERTIES OUTPUT_NAME "godot-ndi.macos.$<LOWER_CASE:$<CONFIG>>")
elseif(UNIX)
    set_target_properties(godot-ndi PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set_target_properties(godot-ndi PROPERTIES OUTPUT_NAME "godot-ndi.linux.$<LOWER_CASE:$<CONFIG>>.x86_64")
endif()

set_target_properties(godot-ndi PROPERTIES SKIP_RPATH TRUE)
set_target_properties(godot-ndi PROPERTIES BUILD_RPATH_USE_ORIGIN TRUE)
set_target_properties(godot-ndi PROPERTIES SKIP_BUILD_RPATH FALSE)
set_target_properties(godot-ndi PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(godot-ndi PROPERTIES INSTALL_RPATH_USE_LINK_PATH FALSE)
if(APPLE)
  set_target_properties(godot-ndi PROPERTIES INSTALL_RPATH "@loader_path")
elseif(UNIX)
  set_target_properties(godot-ndi PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

if(WIN32)
    add_custom_command(
        TARGET godot-ndi
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${NDI_LICENSE_DIR}/Processing.NDI.Lib.Licenses.txt" "${PROJECT_SOURCE_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E copy "${NDI_BINARY_DIR}/Processing.NDI.Lib.x64.dll" "${PROJECT_SOURCE_DIR}/bin"
    )
elseif(APPLE)
    add_custom_command(
        TARGET godot-ndi
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${NDI_LICENSE_DIR}/*.txt" "${PROJECT_SOURCE_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E copy "${NDI_LIBRARY_DIR}/libndi.dylib" "${PROJECT_SOURCE_DIR}/bin"
    )
elseif(UNIX)
    add_custom_command(
        TARGET godot-ndi
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${NDI_LICENSE_DIR}/*.txt" "${PROJECT_SOURCE_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E copy "${NDI_LIBRARY_DIR}/libndi.so*" "${PROJECT_SOURCE_DIR}/bin"
    )
endif()
