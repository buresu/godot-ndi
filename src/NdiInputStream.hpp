#pragma once

#include "NdiTypeDef.hpp"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/thread.hpp>

namespace godot {

class NdiInputStream : public RefCounted {
  GDCLASS(NdiInputStream, RefCounted)
public:
  NdiInputStream();
  virtual ~NdiInputStream();

  void open();
  void close();
  void reopen();

  uint64_t get_frame_count() const;
  Ref<Image> get_frame() const;
  void set_source_name(const String &name);
  void set_bandwidth(NdiBandwidth bandwidth);
  NdiBandwidth get_bandwidth() const;

protected:
  static void _bind_methods();
  void _thread_function();

private:
  Ref<Thread> _thread;
  Ref<Mutex> _mutex;
  Ref<Image> _frame;
  uint64_t _frame_count = 0;
  String _source_name;
  NdiBandwidth _bandwidth = BANDWIDTH_HIGHEST;
  bool _should_exit = false;
};
} // namespace godot
