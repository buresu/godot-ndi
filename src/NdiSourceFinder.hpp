#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class Thread;
class Mutex;

class NdiSourceFinder  : public RefCounted  {
    GDCLASS(NdiSourceFinder, RefCounted)
private:
    Thread* _thread;
    Mutex* _mutex;
    bool _should_exit;
    HashMap<String, void*> _source_map;

    void _thread_function();

public:
    NdiSourceFinder();
    ~NdiSourceFinder();

    void start();
    void stop();
    Dictionary get_source_map() const;
};

}
