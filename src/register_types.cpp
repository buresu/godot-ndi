#include "register_types.hpp"

#include <godot_cpp/core/class_db.hpp>

//#include "GDSpoutSender.hpp"
// #include "GDSpoutTexture.hpp"

void initialize_ndi_module(ModuleInitializationLevel p_level) {

  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  //ClassDB::register_class<GDSpoutSender>();
  // ClassDB::register_class<GDSpoutTexture>();
}

void uninitialize_ndi_module(ModuleInitializationLevel p_level) {

  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }
}

extern "C" {

GDExtensionBool GDE_EXPORT
ndi_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                   const GDExtensionClassLibraryPtr p_library,
                   GDExtensionInitialization *r_initialization) {

  godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library,
                                                 r_initialization);

  init_obj.register_initializer(initialize_ndi_module);
  init_obj.register_terminator(uninitialize_ndi_module);
  init_obj.set_minimum_library_initialization_level(
      MODULE_INITIALIZATION_LEVEL_SCENE);

  return init_obj.init();
}
}
