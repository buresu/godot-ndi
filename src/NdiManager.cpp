#include "NdiManager.hpp"
#include "NdiSourceFinder.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Processing.NDI.Lib.h>

using namespace godot;

NdiManager *NdiManager::singleton = nullptr;

void NdiManager::_bind_methods() {
  ClassDB::bind_method(D_METHOD("initialize"), &NdiManager::initialize);
  ClassDB::bind_method(D_METHOD("finalize"), &NdiManager::finalize);
  ClassDB::bind_method(D_METHOD("available_sources"),
                       &NdiManager::available_sources);
  ClassDB::bind_method(D_METHOD("get_source", "name"), &NdiManager::get_source);
}

NdiManager::NdiManager() : Object() { singleton = this; }

NdiManager::~NdiManager() {
  finalize();
  singleton = nullptr;
}

NdiManager *NdiManager::get_singleton() { return singleton; }

void NdiManager::initialize() {
  if (!NDIlib_initialize()) {
    UtilityFunctions::printerr("NDIlib is not initialized");
    return;
  }
  _sourceFinder.instantiate();
  _sourceFinder->start();
}

void NdiManager::finalize() {
  if (_sourceFinder != nullptr) {
    _sourceFinder->stop();
    _sourceFinder.unref();
  }
  NDIlib_destroy();
}

TypedArray<String> NdiManager::available_sources() const {
  TypedArray<String> sources;
  if (_sourceFinder != nullptr) {
    Dictionary source_map = _sourceFinder->get_source_map();
    Array keys = source_map.keys();
    for (int i = 0; i < keys.size(); i++) {
      sources.push_back(keys[i]);
    }
  }
  return sources;
}

Variant NdiManager::get_source(const String &name) const {
  if (_sourceFinder != nullptr) {
    Dictionary source_map = _sourceFinder->get_source_map();
    if (source_map.has(name)) {
      return source_map[name];
    }
  }
  return Variant();
}
