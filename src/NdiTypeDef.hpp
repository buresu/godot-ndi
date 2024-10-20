#pragma once

#include <godot_cpp/core/binder_common.hpp>

namespace godot {

enum NdiBandwidth { BANDWIDTH_HIGHEST, BANDWIDTH_LOWEST };

} // namespace godot

VARIANT_ENUM_CAST(godot::NdiBandwidth);
