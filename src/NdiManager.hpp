#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/string.hpp>

class NDIlib_source_t;

namespace godot {

class NdiSourceFinder;

class NdiManager : public RefCounted {
    GDCLASS(NdiManager, RefCounted)
private:
    Ref<NdiSourceFinder> _sourceFinder;

public:
    NdiManager();
    ~NdiManager();

    void initialize();
    void finalize();
    TypedArray<String> available_sources() const;
    Variant get_source(const String &name) const;
};

}
