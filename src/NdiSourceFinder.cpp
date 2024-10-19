#include "NdiSourceFinder.hpp"

#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>

#include <Processing.NDI.Lib.h>

using namespace godot;

void NdiSourceFinder::_bind_methods() {
  ClassDB::bind_method(D_METHOD("_thread_function"),
                       &NdiSourceFinder::_thread_function);
  ClassDB::bind_method(D_METHOD("start"), &NdiSourceFinder::start);
  ClassDB::bind_method(D_METHOD("stop"), &NdiSourceFinder::stop);
  ClassDB::bind_method(D_METHOD("get_source_map"),
                       &NdiSourceFinder::get_source_map);
}

NdiSourceFinder::NdiSourceFinder() : Object() { _mutex = memnew(Mutex); }

NdiSourceFinder::~NdiSourceFinder() {
  stop();
  memdelete(_mutex);
}

void NdiSourceFinder::start() {
  if (_thread == nullptr) {
    _should_exit = false;
    _thread = memnew(Thread);
    _thread->start(Callable(this, "_thread_function"));
  }
}

void NdiSourceFinder::stop() {
  if (_thread != nullptr) {
    _should_exit = true;
    _thread->wait_to_finish();
    memdelete(_thread);
    _thread = nullptr;
  }
}

Dictionary NdiSourceFinder::get_source_map() const {
  Dictionary result;
  _mutex->lock();
  for (const KeyValue<String, void *> &E : _source_map) {
    result[E.key] = E.value;
  }
  _mutex->unlock();
  return result;
}

void NdiSourceFinder::_thread_function() {

  NDIlib_find_instance_t find = NDIlib_find_create_v2();

  while (find && !_should_exit) {
    uint32_t num_sources = 0;
    const NDIlib_source_t *sources =
        NDIlib_find_get_current_sources(find, &num_sources);

    _mutex->lock();
    _source_map.clear();
    for (uint32_t i = 0; i < num_sources; ++i) {
      _source_map[String(sources[i].p_ndi_name)] =
          const_cast<NDIlib_source_t *>(&sources[i]);
    }
    _mutex->unlock();

    OS::get_singleton()->delay_msec(5000); // 5 seconds delay
  }

  if (find) {
    NDIlib_find_destroy(find);
  }
}
