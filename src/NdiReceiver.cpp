#include "NdiReceiver.hpp"
#include "NdiManager.hpp"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/core/class_db.hpp>

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

  BIND_ENUM_CONSTANT(BANDWIDTH_HIGHEST);
  BIND_ENUM_CONSTANT(BANDWIDTH_LOWEST);
}

NdiReceiver::NdiReceiver() : _is_running(true) {

  _ndi_input_stream.instantiate();
  _ndi_input_stream->set_bandwidth(_bandwidth);
  _ndi_input_stream->set_source_name(_source_name);
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

void NdiReceiver::_get_property_list(List<PropertyInfo> *p_list) const {

  auto ndi_manager = NdiManager::get_singleton();

  p_list->push_back(
      PropertyInfo(Variant::STRING, "source_name", PROPERTY_HINT_ENUM,
                   String(",").join(ndi_manager->get_available_sources())));

  p_list->push_back(PropertyInfo(Variant::INT, "bandwidth", PROPERTY_HINT_ENUM,
                                 "Highest,Lowest"));

  p_list->push_back(PropertyInfo(Variant::OBJECT, "target_texture",
                                 PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture"));
}

bool NdiReceiver::_set(const StringName &p_name, const Variant &p_value) {
  if (p_name == StringName("source_name")) {
    set_source_name(p_value);
    return true;
  } else if (p_name == StringName("bandwidth")) {
    set_bandwidth(static_cast<NdiBandwidth>(static_cast<uint32_t>(p_value)));
    return true;
  } else if (p_name == StringName("target_texture")) {
    set_target_texture(p_value);
    return true;
  }
  return false;
}

bool NdiReceiver::_get(const StringName &p_name, Variant &r_ret) const {
  if (p_name == StringName("source_name")) {
    r_ret = get_source_name();
    return true;
  } else if (p_name == StringName("bandwidth")) {
    r_ret = get_bandwidth();
    return true;
  } else if (p_name == StringName("target_texture")) {
    r_ret = get_target_texture();
    return true;
  }
  return false;
}

void NdiReceiver::set_source_name(const String &name) {
  _source_name = name;
  if (_ndi_input_stream->get_source_name() != name) {
    _ndi_input_stream->set_source_name(name);
    _ndi_input_stream->call_deferred("reopen");
  }
}

String NdiReceiver::get_source_name() const { return _source_name; }

void NdiReceiver::set_bandwidth(NdiBandwidth bandwidth) {
  _bandwidth = bandwidth;
  if (_ndi_input_stream->get_bandwidth() != bandwidth) {
    _ndi_input_stream->set_bandwidth(bandwidth);
    _ndi_input_stream->call_deferred("reopen");
  }
}

NdiBandwidth NdiReceiver::get_bandwidth() const { return _bandwidth; }

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
