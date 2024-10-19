#include "register_types.hpp"

#include "NdiInputStream.hpp"
#include "NdiManager.hpp"
#include "NdiReceiver.hpp"
#include "NdiSourceFinder.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

static NdiManager *ndi_manager_singleton = nullptr;

void initialize_ndi_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  ClassDB::register_class<NdiSourceFinder>();
  ClassDB::register_class<NdiManager>();
  ClassDB::register_class<NdiInputStream>();
  ClassDB::register_class<NdiReceiver>();

  ndi_manager_singleton = memnew(NdiManager);
  Engine::get_singleton()->register_singleton("NdiManager",
                                              ndi_manager_singleton);

  ndi_manager_singleton->initialize();
}

void uninitialize_ndi_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  ndi_manager_singleton->finalize();

  if (ndi_manager_singleton) {
    Engine::get_singleton()->unregister_singleton("NdiManager");
    memdelete(ndi_manager_singleton);
  }
}

extern "C" {
GDExtensionBool GDE_EXPORT
ndi_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                 const GDExtensionClassLibraryPtr p_library,
                 GDExtensionInitialization *r_initialization) {
  GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library,
                                          r_initialization);

  init_obj.register_initializer(initialize_ndi_module);
  init_obj.register_terminator(uninitialize_ndi_module);
  init_obj.set_minimum_library_initialization_level(
      MODULE_INITIALIZATION_LEVEL_SCENE);

  return init_obj.init();
}
}
