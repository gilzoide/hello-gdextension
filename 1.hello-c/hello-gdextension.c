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

// GDExtension API function pointers
static GDExtensionInterfaceStringNewWithUtf8Chars string_new_with_utf8_chars;
static GDExtensionInterfaceStringNameNewWithUtf8Chars string_name_new_with_utf8_chars;
static GDExtensionInterfaceGetVariantFromTypeConstructor get_variant_from_type_constructor;
static GDExtensionInterfaceVariantGetPtrDestructor variant_get_ptr_destructor;
static GDExtensionInterfaceVariantGetPtrUtilityFunction variant_get_ptr_utility_function;
static GDExtensionInterfaceVariantDestroy variant_destroy;
// Godot API function pointers
static GDExtensionVariantFromTypeConstructorFunc construct_Variant_from_String;
static GDExtensionPtrDestructor destroy_String;
static GDExtensionPtrDestructor destroy_StringName;

// Here is our global "print" function
static GDExtensionPtrUtilityFunction print_function;

void print(const char *text) {
    // 1. Construct a String from the C string
    String string;
    string_new_with_utf8_chars(&string, text);

    // 2. Construct a Variant from the String
    Variant arg1;
    construct_Variant_from_String(&arg1, &string);

    // 3. Call the utility function
    // Since it's a void function, use "NULL" for the return pointer
    GDExtensionConstVariantPtr args[1] = { &arg1 };
    print_function(NULL, args, 1);

    // 4. Clean up resources that are no longer needed
    variant_destroy(&arg1);
    destroy_String(&string);
}

void initialize(void *userdata, GDExtensionInitializationLevel p_level) {
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
        return;
    }

    construct_Variant_from_String = get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_STRING);
    destroy_String = variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING);
    destroy_StringName = variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);

    // Initialize "print" StringName
    StringName print_StringName;
    string_name_new_with_utf8_chars(&print_StringName, "print");
    // then fetch the "print" function pointer
    print_function = variant_get_ptr_utility_function(&print_StringName, 2648703342);
    // Release the "print" StringName, since we don't need it anymore
    destroy_StringName(&print_StringName);

    print("Hello GDExtension from C!");
}

void deinitialize(void *userdata, GDExtensionInitializationLevel p_level) {
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
        return;
    }

    print("Goodbye GDExtension from C!");
}

GDExtensionBool hello_extension_entry(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization
) {
    r_initialization->initialize = &initialize;
    r_initialization->deinitialize = &deinitialize;
    // save the GDExtension API function pointers globally
    string_new_with_utf8_chars = (GDExtensionInterfaceStringNewWithUtf8Chars) p_get_proc_address("string_new_with_utf8_chars");
    string_name_new_with_utf8_chars = (GDExtensionInterfaceStringNameNewWithUtf8Chars) p_get_proc_address("string_name_new_with_utf8_chars");
    get_variant_from_type_constructor = (GDExtensionInterfaceGetVariantFromTypeConstructor) p_get_proc_address("get_variant_from_type_constructor");
    variant_get_ptr_destructor = (GDExtensionInterfaceVariantGetPtrDestructor) p_get_proc_address("variant_get_ptr_destructor");
    variant_get_ptr_utility_function = (GDExtensionInterfaceVariantGetPtrUtilityFunction) p_get_proc_address("variant_get_ptr_utility_function");
    variant_destroy = (GDExtensionInterfaceVariantDestroy) p_get_proc_address("variant_destroy");
    return 1;
}
