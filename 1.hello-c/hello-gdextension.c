#include "include/gdextension_interface.h"

typedef struct {
    uint8_t godot_data_dont_touch_this[sizeof(void *)];
} String;

typedef struct {
    uint8_t godot_data_dont_touch_this[sizeof(void *)];
} StringName;

typedef struct {
    uint8_t godot_data_dont_touch_this[24];
} Variant;

// GDExtensions interface pointer
const GDExtensionInterface *interface;
// Godot API function pointers
static GDExtensionPtrConstructor construct_StringName_from_String;
static GDExtensionVariantFromTypeConstructorFunc construct_Variant_from_String;
static GDExtensionPtrDestructor destroy_String;
static GDExtensionPtrDestructor destroy_StringName;

// here is our global "print" StringName
static StringName print_StringName;
// and here is our global "print" function
static GDExtensionPtrUtilityFunction print_function;

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

void print(const char *text) {
    // 1. Construct a String from the C string
    String string;
    interface->string_new_with_utf8_chars(&string, text);

    // 2. Construct a Variant from the String
    Variant arg1;
    construct_Variant_from_String(&arg1, &string);

    // 3. Call the utility function
    // Since it's a void function, use "NULL" for the return pointer
    GDExtensionConstVariantPtr args[1] = { &arg1 };
    print_function(NULL, args, 1);

    // 4. Clean up resources that are no longer needed
    interface->variant_destroy(&arg1);
    destroy_String(&string);
}

void initialize(void *userdata, GDExtensionInitializationLevel p_level) {
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
        return;
    }

    // StringName constructor at index 2 is the one that receives String
    // You can find this information in `extension_api.json` file
    construct_StringName_from_String = interface->variant_get_ptr_constructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME, 2);
    construct_Variant_from_String = interface->get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_STRING);
    destroy_String = interface->variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING);
    destroy_StringName = interface->variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);

    // Initialize "print" StringName
    print_StringName = construct_StringName_from_cstring("print");
    // then fetch the "print" function pointer
    print_function = interface->variant_get_ptr_utility_function(&print_StringName, 2648703342);

    print("Hello GDExtension from C!");
}

void deinitialize(void *userdata, GDExtensionInitializationLevel p_level) {
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
        return;
    }

    print("Goodbye GDExtension from C!");

    // Destroy "print" StringName
    destroy_StringName(&print_StringName);
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
