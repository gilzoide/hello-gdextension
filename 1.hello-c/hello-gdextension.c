#include "include/gdextension_interface.h"

GDExtensionBool hello_extension_entry(
   const GDExtensionInterface *interface,
   GDExtensionClassLibraryPtr library,
   GDExtensionInitialization *initialization
) {
   // returning 0 means error, returning non-zero means success
   return 1;
}

