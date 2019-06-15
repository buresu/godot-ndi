#include <Godot.hpp>
#include <Processing.NDI.Lib.h>

#include "NDIReceiver.hpp"
//#include "SpoutViewport.hpp"

using namespace godot;

// GDNative Singleton
extern "C" void GDN_EXPORT godot_gdnative_singleton() {}

// GDNative Initialize
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
  Godot::gdnative_init(o);
  NDIlib_initialize();
}

// GDNative Terminate
extern "C" void GDN_EXPORT
godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
  Godot::gdnative_terminate(o);
  NDIlib_destroy();
}

// NativeScript Initialize
extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
  Godot::nativescript_init(handle);
  register_class<NDIReceiver>();
  //register_tool_class<SpoutViewport>();
}

// NativeScript Terminate
extern "C" void GDN_EXPORT godot_nativescript_terminate(void *handle) {
  Godot::nativescript_terminate(handle);
}
