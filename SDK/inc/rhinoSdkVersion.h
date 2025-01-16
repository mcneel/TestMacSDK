//
// Copyright (c) 1993-2017 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#if !defined(RHINO__SDK__VERSION__DEFINITION__)
////////////////////////////////////////////////////////////////
//
// The static member functions on RhinoVersion class provide
// runtime access to Rhinoceros application version information.
// In most cases, you should use those functions when you need
// to know version information about the Rhino application
// this is running.
//
// In some rare case that you actually need the C++ prerpocessor
// defines for this version information.
//
// If you clearly understand why you are in a special case and you
// want to include this thoughtfully and diliberately use the C++
// defines rather than the RhinoVersion functions, then you must 
// define RHINO__SDK__VERSION__DEFINITION__ before including this 
// file.
//
// If you are confused or frustrated, please ask for help.
//
#error Do NOT include rhinoSdkVersion.h in your code. See comment above.
//
////////////////////////////////////////////////////////////////
#endif

#if !defined(RHINO_SDK_VERSION_INC_)
#define RHINO_SDK_VERSION_INC_

//////////////////////////////////////////////////////////////////////
//
// Todo - move the following defines to a file that is rarely included
// to eliminate the need to recompile 1000's of files when the build
// system updates the year, month, day, hour, minute values and
// other build version information that depend on these defined
// values.
//

#define OPENNURBS_VERSION_DEFINITION

#if defined(RHINO_CORE_COMPONENT) || defined(ON_RUNTIME_APPLE)
// Build system path to opennurbs_version.h 
#include "../../../opennurbs/opennurbs_version.h"
#else
// Public SDK path to opennurbs_version.h
#include "../opennurbs/opennurbs_version.h"
#endif

#undef OPENNURBS_VERSION_DEFINITION

// "major" will be 6 for all versions of Rhinoceros 6
#define RHINO_VERSION_MAJOR        OPENNURBS_VERSION_MAJOR

// "minor" corresponds to the Rhinoceros 6 service release number
#define RHINO_VERSION_MINOR        OPENNURBS_VERSION_MINOR

#define RHINO_VERSION_YEAR         OPENNURBS_VERSION_YEAR
#define RHINO_VERSION_MONTH        OPENNURBS_VERSION_MONTH
#define RHINO_VERSION_DAY_OF_MONTH OPENNURBS_VERSION_DAY_OF_MONTH
#define RHINO_VERSION_HOUR         OPENNURBS_VERSION_HOUR
#define RHINO_VERSION_MINUTE       OPENNURBS_VERSION_MINUTE

/*
RHINO_VERSION_BRANCH
  0: developer build
  1: Windows Commercial build
  2: Mac Commercial build
  3: Windows BETA build
  4: Mac Beta build
  5: Windows WIP build
  6: Mac WIP build
*/
#define RHINO_VERSION_BRANCH       OPENNURBS_VERSION_BRANCH

#define RHINO_VERSION_NUMBER ON_VERSION_NUMBER_CTOR( \
          RHINO_VERSION_MAJOR, RHINO_VERSION_MINOR, \
          RHINO_VERSION_YEAR, RHINO_VERSION_MONTH, RHINO_VERSION_DAY_OF_MONTH, \
          RHINO_VERSION_BRANCH )

// Rhinoceros version numbers as strings
//
// Whenever you need a string that has a 6 that will need to be
// changed to a 7 in the Rhino V7 code, use these macros.
// You can use string concatenation for strings that are not
// explicitly defined.
// Example:
//   L"Rhinoceros " RHINO_VERSION_NUMBER_MAJOR_WSTRING L" " RHINO_VERSION_NUMBER_SR_STRING
//     is the same as 
//   L"Rhinoceros 6 SR2"

// char strings
#define RHINOCEROS_APP_NAME_STRING "Rhinoceros " RMA_VERSION_NUMBER_MAJOR_STRING
#define RHINO_VERSION_NUMBER_MAJOR_STRING RMA_VERSION_NUMBER_MAJOR_STRING
#define RHINO_VERSION_NUMBER_SR_STRING    RMA_VERSION_NUMBER_SR_STRING
#define RHINO_VERSION_NUMBER_STRING       RMA_VERSION_WITH_PERIODS_STRING
#define RHINO_REGISTRY_ROOT_STRING "McNeel\\Rhinoceros\\" RMA_VERSION_NUMBER_MAJOR_STRING ".0"

// wchar_t strings
#define RHINOCEROS_APP_NAME_WSTRING L"Rhinoceros " RMA_VERSION_NUMBER_MAJOR_WSTRING
#define RHINO_VERSION_NUMBER_MAJOR_WSTRING RMA_VERSION_NUMBER_MAJOR_WSTRING
#define RHINO_VERSION_NUMBER_SR_WSTRING    RMA_VERSION_NUMBER_SR_WSTRING
#define RHINO_VERSION_NUMBER_WSTRING       RMA_VERSION_WITH_PERIODS_WSTRING
#define RHINO_REGISTRY_ROOT_WSTRING L"McNeel\\Rhinoceros\\" RMA_VERSION_NUMBER_MAJOR_WSTRING ".0"

// .3dm file version
//
// Early in the WIP cycle, Rhino writes files that can be read by the
// current commercial version of Rhino. At some point during the WIP 
// cycle, the WIP product switches to writing files that will
// only be read by the WIP and the next commercial version.
#define RHINO_CURRENT_FILE_VERSION_NUMBER (OPENNURBS_CURRENT_ARCHIVE_VERSION/10)


// Four integers separated by commas - useful for getting
// the version quartet values as unsigned int or int.
#define RHINO_VERSION_NUMBER_WITH_COMMAS VERSION_WITH_COMMAS

// Use RhinoApp().SdkServiceReleaseVersion() to get the value
// of RHINO_SDK_SERVICE_RELEASE.
//
//   RHINO_VERSION_NUMBER is defined in rhinoSdkRhinoVersion.h
//   and is automatically set by the build system. 
//   Do not hard code this value. 
//   RHINO_SDK_SERVICE_RELEASE must increase so SDK addtions 
//   work as expected. Plug-ins that are built
//   with an "updated SDK" will required a host Rhino that is
//   the same or more recent than the "updated SDK".
#define RHINO_SDK_SERVICE_RELEASE RHINO_VERSION_NUMBER

// The value of RHINO_WINDOWS_REGISTRY_SVN_BRANCH is used
// to build registry paths that require the name of the
// source code branch. Runtime code validates that it
// is set correctly.  It must be hard coded because it
// is used in public and internal debug and release 
// builds.
#define RHINO_WINDOWS_REGISTRY_SVN_BRANCH L"trunk"

// Apple OS X SDK version number
// When the Rhino SDK is frozen, this number will be hard
// coded.  Until then, it will increase each time the
// build system increments the build version numbers.
#define RHINO_SDK_OSX_VERSION RHINO_SDK_SERVICE_RELEASE
#define RHINO_SDK_LINUX_VERSION RHINO_SDK_SERVICE_RELEASE

// Windows SDK version number.
// When the Rhino SDK is frozen, this number will be hard
// coded.  Until then, it will increase each time the
// build system increments the build version numbers.

// SDK is frozen when the definition of RHINO_SDK_WINDOWS_VERSION is a hardcoded number.

// The Rhino Windows C++ SDK is currently "frozen" at Rhino 6.0 2017 August 14.
//
// NOTE WELL:
//   It is intentional that the values of RHINO_SDK_WINDOWS_VERSION for Rhino 6, 7 and 8 are identical.
//
//   The Rhino 7 and 8 SDK is an extended version of the Rhino 6 SDK.
//   Rhino 7 and 8 load Rhino 6.* plug-ins.
//   Thus, the Rhino 6, 7, and 8 have identical values for RHINO_SDK_WINDOWS_VERSION.
//   Use RHINO_SDK_SERVICE_RELEASE to determine which extension of the basic SDK
//   is being used.
//
//   There is not a public Apple OSX C++ SDK. Rhino for Mac C++ plug-ins are built 
//   and distributed by McNeel and the version numbers for those plug-ins are handled 
//   in a different manner.
#define RHINO_SDK_WINDOWS_VERSION ON_VERSION_NUMBER_CTOR(6, 0, 2017, 8, 15, 1 )

#if defined(_MSC_VER)
// Microsoft compiler - Windows build
#if ( _MSC_VER < 1700 )
// older compiler
#error Rhino V6 must be compiled with Visual Studio 2012 or later.
#endif

#define RHINO_SDK_VERSION RHINO_SDK_WINDOWS_VERSION

#elif defined(__APPLE__)
// Apple compiler - OS X build

#define RHINO_SDK_VERSION RHINO_SDK_OSX_VERSION

#elif defined(ON_RUNTIME_LINUX)

#define RHINO_SDK_VERSION RHINO_SDK_LINUX_VERSION

#else
// You goofed!
#error UNSUPPORTED COMPILER
#endif

#endif
