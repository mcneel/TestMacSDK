#pragma once

#if !defined(RHINO_SDK_PLUG_IND_DECLARE_INC_)
#define RHINO_SDK_PLUG_IND_DECLARE_INC_

// This header file defines RHINO_PLUG_IN_DECLARE for plug-ins.
// Rhino plugins can  include this file in exactly one .cpp 
// rather than in stdafx.h if they want to minimize the amount of
// recompiling that is required when the version numbers are updated.
// - For plug-ins that use an installed Rhino SDK, this isn't an issue.
// - For Rhino plug-ins that compile against the core Rhino source code,
//   this is an issue and using rhinoSdkPlugInDeclare.h substantually
//   reduces the amount of recompiling that is required when a developer
//   updates source code.

#if defined(RHINO_PLUG_IN_DECLARE_IS_ERROR) && defined(RHINO_PLUG_IN_DECLARE)
#undef RHINO_PLUG_IN_DECLARE_IS_ERROR
#undef RHINO_PLUG_IN_DECLARE
#endif

#define RHINO__SDK__VERSION__DEFINITION__
// This include will define RHINO_SDK_VERSION and RHINO_SDK_SERVICE_RELEASE
// that are required for RHINO_PLUG_IN_DECLARE.
#include "rhinoSdkVersion.h"
#undef RHINO__SDK__VERSION__DEFINITION__

#if defined (_MFC_VER)
#define SDKMFCVER _MFC_VER
#else
#define SDKMFCVER 0
#endif

#if defined (RHINO_SDK_MFC)
#define SDKMFCUSAGE true
#else
#define SDKMFCUSAGE false
#endif

#define RHINO_PLUG_IN_DECLARE \
  extern "C" RHINO_SDK_EXPORT unsigned int  RhinoPlugInSdkVersion(void)        {return  RHINO_SDK_VERSION;} \
  extern "C" RHINO_SDK_EXPORT unsigned int  RhinoPlugInSdkServiceRelease(void) { return RHINO_SDK_SERVICE_RELEASE;}
#if defined (ON_RUNTIME_WIN)
  extern "C" RHINO_SDK_EXPORT unsigned int  RhinoPlugInMfcVersion(void)        { return SDKMFCVER;}
  extern "C" RHINO_SDK_EXPORT unsigned int  RhinoPlugInMscVersion(void)        { return _MSC_VER;}
  extern "C" RHINO_SDK_EXPORT bool          RhinoPlugInUsesMfc(void)           { return SDKMFCUSAGE;}
#endif
#endif
