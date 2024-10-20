#include "NdiReceiver.hpp"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void NdiReceiver::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_source_name", "name"),
                       &NdiReceiver::set_source_name);
  ClassDB::bind_method(D_METHOD("get_source_name"),
                       &NdiReceiver::get_source_name);
  ClassDB::bind_method(D_METHOD("set_bandwidth", "bandwidth"),
                       &NdiReceiver::set_bandwidth);
  ClassDB::bind_method(D_METHOD("get_bandwidth"), &NdiReceiver::get_bandwidth);
  ClassDB::bind_method(D_METHOD("set_target_texture", "texture"),
                       &NdiReceiver::set_target_texture);
  ClassDB::bind_method(D_METHOD("get_target_texture"),
                       &NdiReceiver::get_target_texture);
  ClassDB::bind_method(D_METHOD("start"), &NdiReceiver::start);
  ClassDB::bind_method(D_METHOD("stop"), &NdiReceiver::stop);
  ClassDB::bind_method(D_METHOD("is_running"), &NdiReceiver::is_running);
  ClassDB::bind_method(D_METHOD("_update_texture"),
                       &NdiReceiver::_update_texture);

  ADD_PROPERTY(PropertyInfo(Variant::STRING, "source_name"), "set_source_name",
               "get_source_name");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "bandwidth"), "set_bandwidth",
               "get_bandwidth");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_texture",
                            PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture"),
               "set_target_texture", "get_target_texture");

  BIND_ENUM_CONSTANT(NdiInputStream::HIGHEST);
  BIND_ENUM_CONSTANT(NdiInputStream::LOWEST);
}

NdiReceiver::NdiReceiver()
    : _is_running(true), _bandwidth(NdiInputStream::HIGHEST) {
  _ndi_input_stream.instantiate();
  _ndi_input_stream->set_source_name("test");
  _ndi_input_stream->open();

  // Connect to the frame_post_draw signal
  RenderingServer::get_singleton()->connect("frame_pre_draw",
                                            Callable(this, "_update_texture"));
}

NdiReceiver::~NdiReceiver() {
  stop();

  // Disconnect from the frame_post_draw signal
  RenderingServer::get_singleton()->disconnect(
      "frame_pre_draw", Callable(this, "_update_texture"));
}

void NdiReceiver::set_source_name(const String &name) {
  _source_name = name;
  _ndi_input_stream->set_source_name(name);
  _ndi_input_stream->reopen();
}

String NdiReceiver::get_source_name() const { return _source_name; }

void NdiReceiver::set_bandwidth(NdiInputStream::Bandwidth bandwidth) {
  _bandwidth = bandwidth;
  _ndi_input_stream->set_bandwidth(bandwidth);
  _ndi_input_stream->reopen();
}

NdiInputStream::Bandwidth NdiReceiver::get_bandwidth() const {
  return _bandwidth;
}

void NdiReceiver::set_target_texture(const Ref<ImageTexture> &texture) {
  _target_texture = texture;
}

Ref<ImageTexture> NdiReceiver::get_target_texture() const {
  return _target_texture;
}

void NdiReceiver::start() {
  if (!_is_running) {
    _ndi_input_stream->open();
    _is_running = true;
  }
}

void NdiReceiver::stop() {
  if (_is_running) {
    _ndi_input_stream->close();
    _is_running = false;
  }
}

void NdiReceiver::_update_texture() {
  if (_is_running && _target_texture.is_valid()) {
    Ref<Image> frame = _ndi_input_stream->get_frame();
    if (frame.is_valid()) {
      if (frame->get_size() != _target_texture->get_size()) {
        _target_texture->set_image(frame);
      } else {
        _target_texture->update(frame);
      }
    }
  }
}
