#include "NdiInputStream.hpp"

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <Processing.NDI.Lib.h>

using namespace godot;

NdiInputStream::NdiInputStream() : Object() { _mutex = memnew(Mutex); }

NdiInputStream::~NdiInputStream() {
  close();
  memdelete(_mutex);
}

void NdiInputStream::_bind_methods() {
  ClassDB::bind_method(D_METHOD("open"), &NdiInputStream::open);
  ClassDB::bind_method(D_METHOD("close"), &NdiInputStream::close);
  ClassDB::bind_method(D_METHOD("reopen"), &NdiInputStream::reopen);
  ClassDB::bind_method(D_METHOD("get_frame_count"),
                       &NdiInputStream::get_frame_count);
  ClassDB::bind_method(D_METHOD("get_frame"), &NdiInputStream::get_frame);
  ClassDB::bind_method(D_METHOD("set_source_name", "name"),
                       &NdiInputStream::set_source_name);
  ClassDB::bind_method(D_METHOD("set_bandwidth", "bandwidth"),
                       &NdiInputStream::set_bandwidth);
  ClassDB::bind_method(D_METHOD("_thread_function"),
                       &NdiInputStream::_thread_function);

  BIND_ENUM_CONSTANT(HIGHEST);
  BIND_ENUM_CONSTANT(LOWEST);
}

void NdiInputStream::open() {
  if (_thread == nullptr) {
    _should_exit = false;
    _thread = memnew(Thread);
    _thread->start(Callable(this, "_thread_function"));
  }
}

void NdiInputStream::close() {
  if (_thread != nullptr) {
    _should_exit = true;
    _thread->wait_to_finish();
    memdelete(_thread);
    _thread = nullptr;
  }
}

void NdiInputStream::reopen() {
  close();
  open();
}

uint64_t NdiInputStream::get_frame_count() const {
  _mutex->lock();
  uint64_t count = _frame_count;
  _mutex->unlock();
  return count;
}

Ref<Image> NdiInputStream::get_frame() const {
  _mutex->lock();
  Ref<Image> frame = _frame;
  _mutex->unlock();
  return frame;
}

void NdiInputStream::set_source_name(const String &name) {
  _source_name = name;
}

void NdiInputStream::set_bandwidth(Bandwidth bandwidth) {
  _bandwidth = bandwidth;
}

void NdiInputStream::_thread_function() {
  while (!_should_exit) {
    NDIlib_source_t source;
    source.p_ndi_name = _source_name.utf8().get_data();

    NDIlib_recv_create_v3_t recv_create_desc;
    recv_create_desc.source_to_connect_to = source;
    recv_create_desc.color_format = NDIlib_recv_color_format_BGRX_BGRA;
    recv_create_desc.bandwidth = (_bandwidth == HIGHEST)
                                     ? NDIlib_recv_bandwidth_highest
                                     : NDIlib_recv_bandwidth_lowest;

    auto _ndi_receiver = NDIlib_recv_create_v3(&recv_create_desc);

    if (_ndi_receiver) {
      NDIlib_metadata_frame_t meta_frame;
      const char *meta = "<ndi_hwaccel enabled=\"true\"/>";
      meta_frame.p_data = const_cast<char *>(meta);
      NDIlib_recv_send_metadata(_ndi_receiver, &meta_frame);

      bool continue_receiving = true;
      while (!_should_exit && continue_receiving) {
        NDIlib_video_frame_v2_t video_frame;
        NDIlib_audio_frame_v3_t audio_frame;

        switch (NDIlib_recv_capture_v3(_ndi_receiver, &video_frame,
                                       &audio_frame, nullptr, 5000)) {
        case NDIlib_frame_type_video: {
          PackedByteArray image_data;
          image_data.resize(video_frame.xres * video_frame.yres * 4);
          memcpy(image_data.ptrw(), video_frame.p_data, image_data.size());

          Ref<Image> new_frame =
              Image::create_from_data(video_frame.xres, video_frame.yres, false,
                                      Image::FORMAT_RGBA8, image_data);

          _mutex->lock();
          _frame = new_frame;
          _frame_count++;
          _mutex->unlock();

          NDIlib_recv_free_video_v2(_ndi_receiver, &video_frame);
          break;
        }
        case NDIlib_frame_type_audio:
          NDIlib_recv_free_audio_v3(_ndi_receiver, &audio_frame);
          break;
        case NDIlib_frame_type_none:
          continue_receiving = false;
          break;
        }

        OS::get_singleton()->delay_usec(5000); // 5ms delay
      }

      NDIlib_recv_destroy(_ndi_receiver);
      _ndi_receiver = nullptr;

      _mutex->lock();
      _frame = Ref<Image>();
      _frame_count++;
      _mutex->unlock();
    }

    OS::get_singleton()->delay_msec(500);
  }
}
