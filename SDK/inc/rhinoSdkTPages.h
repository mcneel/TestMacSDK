#pragma once

#if defined(ON_RUNTIME_WIN) && defined(_MFC_VER)
#include "rhinoSdkTMfcPages.h"
#endif

#if defined(ON_RUNTIME_APPLE)
#if !defined RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE
#include "rhinoSdkTMacPages.h"
#endif
#endif
