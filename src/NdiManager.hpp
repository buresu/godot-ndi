#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class NdiSourceFinder;

class NdiManager : public Object {
  GDCLASS(NdiManager, Object)
private:
  static NdiManager *singleton;
  Ref<NdiSourceFinder> _sourceFinder;

protected:
  static void _bind_methods();

public:
  static NdiManager *get_singleton();

  NdiManager();
  virtual ~NdiManager() override;

  void initialize();
  void finalize();

  TypedArray<String> get_available_sources() const;
  Variant get_source(const String &name) const;
};

} // namespace godot
