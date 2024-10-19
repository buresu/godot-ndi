#pragma once

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/thread.hpp>

namespace godot {

class NdiInputStream : public RefCounted {
  GDCLASS(NdiInputStream, RefCounted)
public:
  enum Bandwidth { HIGHEST, LOWEST };

  NdiInputStream();
  virtual ~NdiInputStream();

  void open();
  void close();
  void reopen();
  uint64_t get_frame_count() const;
  Ref<Image> get_frame() const;
  void set_source_name(const String &name);
  void set_bandwidth(Bandwidth bandwidth);
  void _thread_function();

protected:
  static void _bind_methods();

private:
  Ref<Thread> _thread;
  Ref<Mutex> _mutex;
  Ref<Image> _frame;
  uint64_t _frame_count = 0;
  String _source_name;
  Bandwidth _bandwidth = HIGHEST;
  bool _should_exit = false;
};
} // namespace godot

VARIANT_ENUM_CAST(godot::NdiInputStream::Bandwidth);
