#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class Thread;
class Mutex;

class NdiSourceFinder : public Object {
  GDCLASS(NdiSourceFinder, Object)
private:
  Thread *_thread = nullptr;
  Mutex *_mutex = nullptr;
  bool _should_exit = false;
  HashMap<String, void *> _source_map;

protected:
  void _thread_function();
  static void _bind_methods();

public:
  NdiSourceFinder();
  virtual ~NdiSourceFinder() override;

  void start();
  void stop();

  Dictionary get_source_map() const;
};

} // namespace godot
