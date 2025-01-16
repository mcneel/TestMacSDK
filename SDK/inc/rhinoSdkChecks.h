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

// DO NOT USE define TO PROTECT THIS FILE AGAINST MULTIPLE INCLUDES.
// DO NOT PUT A #pragma once IN THIS FILE.

// This file checks that compile time defines are correctly set.
// If something incorrect is defined, it will call #error.
#if !defined(ON_RUNTIME_LINUX) && defined(__linux__)
#define ON_RUNTIME_LINUX
#endif

#if !defined(ON_RUNTIME_APPLE) && !defined(ON_RUNTIME_LINUX)
// These checks are very specific to Windows Rhino
#if defined(ON_CMAKE_BUILD)
#if defined(ON_64BIT_RUNTIME)
#define WIN64
#undef WIN32
#endif
#if defined(ON_32BIT_RUNTIME)
#define WIN32
#undef WIN64
#endif
#endif

#if defined(WIN32) && defined(WIN64)
//#error WIN32 and WIN64 defined.  This is wrong!
#error Rhino for Windows is a 64-bit application. WIN64 should be defined. WIN32 should not be defined.
#endif

#if !defined(WIN32) && !defined(WIN64)
//#error Exactly one of WIN32 or WIN64 should be defined.
#error Rhino for Windows is a 64-bit application. WIN64 should be defined. WIN32 should not be defined.
#endif

#if defined(WIN64)

#if !defined(_M_X64)
// This should be automatically defined by the compiler
#error _M_X64 should be defined for Rhino 64-bit executables.
#endif

#if defined(_M_IX86) || defined(_M_IA64)
//#error Incorrect _M_IX... setting for Rhino WIN64 build
#error Rhino for Windows is a 64-bit application. Neither _M_IX86 nor _M_IA64 should be defined.
#endif

#endif


#if defined(WIN32) && !defined(ON_CMAKE_BUILD)

#if defined(RHINO_WIN32_SPECIAL_CASE)
#if defined(_M_IA64) || defined(_M_X64)
#error Incorrect _M_IX... setting for Rhino WIN32 build
#endif

#if !defined(_M_IX86)
// This should be automatically defined by the compiler
#error _M_IX86 should be defined for Rhino 32-bit executables
#endif
#else
#error Rhino for Windows is a 64-bit application. WIN64 should be defined. WIN32 should not be defined.
#endif

#endif


#if !defined(RHINO_SDK_STDAFX_PREAMBLE_H__)
#error rhinoSdkStdafxPreamble.h MUST be included in your plug-in stdafx.h file BEFORE any other includes.
#endif


#if !defined(_MSC_VER)
// If you are getting this error, then you are using the wrong brand
// of C++ compiler.  Rhino application developers muse use
// use Microsoft Visual Studio 2012.
#error Rhino 6 Windows application developers must use Microsoft Visual Studio 2012.
#endif

#if _MSC_VER < 1700
// If you are getting this error, then you are using the wrong version of
// the Microsoft C++ compiler.  Rhino 6 application developers muse use
// use Microsoft Visual Studio 2012 (Microsoft Visual Studio 11.0).
#error Rhino 6 Windows application developers must use Microsoft Visual Studio 2012.
#endif

///////////////////////////////////////////////////////////////
//
// Validate Windows version defines
//
#include "rhinoSdkWindowsVersionValidation.h"

////////////////////////////////////////////////////////
//
// MBCS/UNICODE define house keeping for plug-in developers
//
#if defined (RHINO_SDK_MFC)
#if !defined(_AFXDLL)
#error Rhino and Rhino plug-ins must dynamically link MFC.
#endif
#endif

#if defined(_MBCS) || defined(MBCS)
#error Rhino plug-ins must be UNICODE applications.  Do not define _MBCS or MBCS.
#endif

#if !defined(_UNICODE) || !defined(UNICODE)
#error Rhino is a UNICODE application. Define _UNICODE in your makefile.
#endif

#if defined(new) || defined(delete)

// If you get this error, new or delete is being defined to be something
// besides the default C++ new or delete and this will cause crashes
// in Rhino plug-ins.
#error Never define new or delete when building Rhino plug-ins.

#endif

//////////////////////////////////////////////////////
//
//

#if !defined(RHINO_SDK_CHECKS__LINKER__INSTRUCTIONS__)
#define RHINO_SDK_CHECKS__LINKER__INSTRUCTIONS__

// The disallowlib lines prevent embedded linking instructions
// in 3rd party object code from bringing in the wrong C runtime
// or MFC libraries.  The linking instructions are commonly found
// in statically linked .lib  files that were compiled with something
// besides Visual C++ 8 (Visual Studio 2005) or were compiled with the
// wrong "Code Generation setting".
//
// For more details, see the McNeel developer CRunTimeTrauma Wiki page.

// Release Rhino and all 3rd party plug-ins use
//
//   Code Generation flag /MD
//   msvcr80.dll (Multithreaded CRT)
//   mfc80u.dll  (Unicode MFC)
//
// Debug Rhino and system debug plug-ins use
//   Code Generation flag /MDd
//   msvcr80d.dll (Multithreaded debug CRT)
//   mfc80ud.dll  (Unicode debug MFC)

#if defined(_DEBUG)

// debug build
// We cannot disallow msvcrt.lib - it breaks plug-in compiles
// that link with static libs.
//#pragma comment(linker, "/disallowlib:msvcrt.lib")
#pragma comment(linker, "/disallowlib:mfc80u.lib")
#else
// release build 

#if defined(RHINO_SDK_DEBUG)
#error Only define RHINO_SDK_DEBUG in debug builds.
#endif

#if defined(RHINO_CORE_DEBUG)
#error Only define RHINO_CORE_DEBUG in the Debug configuration of rhino4.vcproj
#endif

#pragma comment(linker, "/disallowlib:msvcrtd.lib")
#pragma comment(linker, "/disallowlib:mfc80ud.lib")
#endif

// No static CRT libs
#pragma comment(linker, "/disallowlib:libc.lib")
#pragma comment(linker, "/disallowlib:libcd.lib")
#pragma comment(linker, "/disallowlib:libcmt.lib")
#pragma comment(linker, "/disallowlib:libcmtd.lib")

// NO STATIC MFC LIBRARES
#pragma comment(linker, "/disallowlib:nafxcw.lib")
#pragma comment(linker, "/disallowlib:nafxcwd.lib")
#pragma comment(linker, "/disallowlib:nafxis.lib")
#pragma comment(linker, "/disallowlib:nafxisd.lib")
#pragma comment(linker, "/disallowlib:uafxcw.lib")
#pragma comment(linker, "/disallowlib:uafxcwd.lib")

// NO MFC 7.1 (Visual Studio 7 / 2003 libraries )
#pragma comment(linker, "/disallowlib:mfc71.lib")
#pragma comment(linker, "/disallowlib:mfc71d.lib")
#pragma comment(linker, "/disallowlib:mfc71u.lib")
#pragma comment(linker, "/disallowlib:mfc71ud.lib")
#pragma comment(linker, "/disallowlib:mfcs71.lib")
#pragma comment(linker, "/disallowlib:mfcs71d.lib")
#pragma comment(linker, "/disallowlib:mfcs71u.lib")
#pragma comment(linker, "/disallowlib:mfcs71ud.lib")

// No MFC 4.2 (Visual Studio 4,5,6 libraries )
#pragma comment(linker, "/disallowlib:mfc42.lib")
#pragma comment(linker, "/disallowlib:mfc42d.lib")
#pragma comment(linker, "/disallowlib:mfc42u.lib")
#pragma comment(linker, "/disallowlib:mfc42ud.lib")
#pragma comment(linker, "/disallowlib:mfcd42ud.lib")
#pragma comment(linker, "/disallowlib:mfcn42d.lib")
#pragma comment(linker, "/disallowlib:mfcn42ud.lib")
#pragma comment(linker, "/disallowlib:mfco42d.lib")
#pragma comment(linker, "/disallowlib:mfco42ud.lib")
#pragma comment(linker, "/disallowlib:mfcs42.lib")
#pragma comment(linker, "/disallowlib:mfcs42d.lib")
#pragma comment(linker, "/disallowlib:mfcs42u.lib")
#pragma comment(linker, "/disallowlib:mfcs42ud.lib")


//// Prohibit any future inclusion of CRTDBG.H by 
//// defining _INC_CRTDBG.
//#if defined(_INC_CRTDBG)
//#error Never include CRTDBG.H in Rhino or Rhino plug-ins.
//#endif
//#define _INC_CRTDBG


#endif

//
//
//////////////////////////////////////////////////////


////////////////////////////////////////////////////////
//
// Force DEBUG_NEW to be the real "new".  If you get a compile
// error here, it's because somewhere DEBUG_NEW is defined to
// be something besides "new".  You need to find out why and
// change your compile flags, includes, etc., so that DEBUG_NEW
// is not defined or defined to be "new".
//
#if defined DEBUG_NEW
// undo anything afx.h has done.
#undef DEBUG_NEW
#define DEBUG_NEW new
#endif

#if !defined(OPENNURBS_IMPORTS)
#define OPENNURBS_IMPORTS
#endif

#if !defined(TL_IMPORTS)
#define TL_IMPORTS
#endif

#if defined(OPENNURBS_EXPORTS)
#error Never define OPENNURBS_EXPORTS when compiling Rhino or plug-ins.
#endif

#if defined(TL_EXPORTS)
#error Never define TL_EXPORTS when compiling Rhino or plug-ins.
#endif

#if defined(RHINO_DEBUG_PLUGIN)

// A "debug" plug-in that links with a release build of Rhino
// is being compiled.  In this case RHINO_SDK_DEBUG must
// NEVER be defined.

#if defined(RHINO_SDK_DEBUG)
#error Never define RHINO_SDK_DEBUG for plug-ins that run with release builds of Rhino.
#endif

#endif

#endif
