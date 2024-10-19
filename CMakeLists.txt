cmake_minimum_required(VERSION 3.12)

project(godot-ndi VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /DTYPED_METHOD_BIND")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT /DTYPED_METHOD_BIND")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITS "64")
else()
    set(BITS "32")
endif()

#add_subdirectory("${PROJECT_SOURCE_DIR}/lib/godot-cpp")

find_package(NDI REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_library(GodotNdi SHARED ${SOURCES})

set_target_properties(GodotNdi PROPERTIES DEBUG_POSTFIX d)

target_include_directories(GodotNdi PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(GodotNdi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GodotNdi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(GodotNdi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/gdextension)
target_include_directories(GodotNdi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/include)
target_include_directories(GodotNdi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/gen/include)
target_include_directories(GodotNdi PRIVATE ${NDI_INCLUDE_DIR})

target_link_directories(GodotNdi PRIVATE ${PROJECT_SOURCE_DIR}/lib/godot-cpp/bin)
target_link_directories(GodotNdi PRIVATE ${NDI_LIBRARY_DIR})
target_link_libraries(GodotNdi PRIVATE optimized godot-cpp.linux.template_release.x86_${BITS} debug godot-cpp.linux.template_debug.x86_${BITS})
target_link_libraries(GodotNdi PRIVATE ${NDI_LIBS})

add_custom_target(
    BuildGodotCpp
    COMMAND scons platform=linux target=$<$<CONFIG:Debug>:template_debug>$<$<CONFIG:Release>:template_release> bits=${BITS}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/godot-cpp
)

add_dependencies(GodotNdi BuildGodotCpp)

install(TARGETS GodotNdi RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
install(FILES ${PROJECT_SOURCE_DIR}/res/ndi.gdextension DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
