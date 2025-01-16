
#pragma once // Public SDK header

// Content library file extensions; not localized.
#define RDK_LIBEXT_MAT         L"rmtl"
#define RDK_LIBEXT_ENV         L"renv"
#define RDK_LIBEXT_TEX         L"rtex"

// These strings are not localized because they must never change
// and they are not displayed to the user.

// Content kinds. DEPRECATED. See CRhRdkContent::Kinds enum.
#define RDK_KIND_MATERIAL      RDK_KIND_MATERIAL_is_deprecated___See_CRhRdkContent_Kinds_enum
#define RDK_KIND_ENVIRONMENT   RDK_KIND_ENVIRONMENT_is_deprecated___See_CRhRdkContent_Kinds_enum
#define RDK_KIND_TEXTURE       RDK_KIND_TEXTURE_is_deprecated___See_CRhRdkContent_Kinds_enum
#define RDK_KIND_COUNT         3

// Generic categories.
#define RDK_CAT_GENERAL        L"general"

// Material categories.
#define RDK_CAT_UTILITY        L"utility"

// Environment categories.
#define RDK_CAT_VOLUMETRIC     L"volumetric"

// Texture categories.
#define RDK_CAT_IMAGE_BASED    L"image-based"
#define RDK_CAT_PROCEDURAL_2D  L"procedural-2d"
#define RDK_CAT_PROCEDURAL_3D  L"procedural-3d"

