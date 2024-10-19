#include "NdiManager.hpp"
#include "NdiSourceFinder.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include <Processing.NDI.Lib.h>

using namespace godot;

NdiManager::NdiManager() : _sourceFinder(nullptr) {}

NdiManager::~NdiManager() {
    finalize();
}

void NdiManager::initialize() {
    if (!NDIlib_initialize()) {
        UtilityFunctions::printerr("NDIlib is not initialized");
        return;
    }
    _sourceFinder.instantiate();
    _sourceFinder->start();
}

void NdiManager::finalize() {
    if (_sourceFinder.is_valid()) {
        _sourceFinder->stop();
        _sourceFinder.unref();
    }
    NDIlib_destroy();
}

TypedArray<String> NdiManager::available_sources() const {
    TypedArray<String> sources;
    if (_sourceFinder.is_valid()) {
        Dictionary source_map = _sourceFinder->get_source_map();
        Array keys = source_map.keys();
        for (int i = 0; i < keys.size(); i++) {
            sources.push_back(keys[i]);
        }
    }
    return sources;
}

Variant NdiManager::get_source(const String &name) const {
    if (_sourceFinder.is_valid()) {
        Dictionary source_map = _sourceFinder->get_source_map();
        if (source_map.has(name)) {
            return source_map[name];
        }
    }
    return Variant();
}