#pragma once

#include "NdiInputStream.hpp"

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace godot {

class NdiReceiver : public Resource {
  GDCLASS(NdiReceiver, Resource)

private:
  NdiInputStream *_ndi_input_stream;
  Ref<ImageTexture> _target_texture;
  bool _is_running;
  String _source_name;
  NdiInputStream::Bandwidth _bandwidth;

protected:
  static void _bind_methods();

public:
  NdiReceiver();
  ~NdiReceiver();

  void set_source_name(const String &name);
  String get_source_name() const;
  void set_bandwidth(NdiInputStream::Bandwidth bandwidth);
  NdiInputStream::Bandwidth get_bandwidth() const;
  void set_target_texture(const Ref<ImageTexture> &texture);
  Ref<ImageTexture> get_target_texture() const;
  void start();
  void stop();
  void _update_texture();

  bool is_running() const { return _is_running; }
};

} // namespace godot
