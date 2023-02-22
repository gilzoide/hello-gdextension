#include <stdio.h>
#include "include/gdextension_interface.h"

typedef GDExtensionStringPtr String;
typedef GDExtensionStringNamePtr StringName;

// GDExtensions interface pointer
const GDExtensionInterface *interface;
// Godot API function pointers
static GDExtensionPtrConstructor construct_StringName_from_String;
static GDExtensionPtrDestructor destroy_String;
static GDExtensionPtrDestructor destroy_StringName;

// here is our global "print" StringName
static StringName print_StringName;

StringName construct_StringName_from_cstring(const char *text) {
    // 1. Construct a String from the C string
    String string;
    interface->string_new_with_latin1_chars(&string, text);

    // 2. Construct a StringName from the String
    StringName string_name;
    GDExtensionConstTypePtr constructor_arguments[1] = { &string };
    construct_StringName_from_String(&string_name, constructor_arguments);

    // 3. Destroy the String, since it's not needed anymore
    destroy_String(&string);

    return string_name;
}

void initialize(void *userdata, GDExtensionInitializationLevel p_level) {
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
        return;
    }

    // StringName constructor at index 2 is the one that receives String
    // You can find this information in `extension_api.json` file
    construct_StringName_from_String = interface->variant_get_ptr_constructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME, 2);
    destroy_String = interface->variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING);
    destroy_StringName = interface->variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);

    // Initialize "print" StringName
    print_StringName = construct_StringName_from_cstring("print");

    printf("initialize at level %d\n", p_level);
}

void deinitialize(void *userdata, GDExtensionInitializationLevel p_level) {
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
        return;
    }

    // Destroy "print" StringName
    destroy_StringName(&print_StringName);

    printf("deinitialize at level %d\n", p_level);
}

GDExtensionBool hello_extension_entry(
    const GDExtensionInterface *p_interface,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization
) {
    r_initialization->initialize = &initialize;
    r_initialization->deinitialize = &deinitialize;
    // save the GDExtensionInterface globally
    interface = p_interface;
    return 1;
}
