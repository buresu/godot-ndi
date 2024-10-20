#pragma once

#include "NdiTypeDef.hpp"

#include <godot_cpp/classes/node.hpp>

namespace godot {

class NdiInputStream;
class ImageTexture;

class NdiReceiver : public Node {
  GDCLASS(NdiReceiver, Node)
private:
  Ref<NdiInputStream> _ndi_input_stream;
  Ref<ImageTexture> _target_texture;
  bool _is_running = true;
  String _source_name;
  NdiBandwidth _bandwidth = BANDWIDTH_HIGHEST;

protected:
  static void _bind_methods();
  void _update_texture();
  void _notification(int p_what);
  void _get_property_list(List<PropertyInfo> *p_list) const;
  bool _set(const StringName &p_name, const Variant &p_value);
  bool _get(const StringName &p_name, Variant &r_ret) const;

public:
  NdiReceiver();
  virtual ~NdiReceiver() override;

  void set_source_name(const String &name);
  String get_source_name() const;
  void set_bandwidth(NdiBandwidth bandwidth);
  NdiBandwidth get_bandwidth() const;
  void set_target_texture(const Ref<ImageTexture> &texture);
  Ref<ImageTexture> get_target_texture() const;
  void start();
  void stop();

  bool is_running() const { return _is_running; }
};

} // namespace godot
