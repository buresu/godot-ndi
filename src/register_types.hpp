#pragma once

#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_ndi_module(ModuleInitializationLevel p_level);
void uninitialize_ndi_module(ModuleInitializationLevel p_level);