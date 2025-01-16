#pragma once

// Please discuss any changes to this file with Dale Lear
#if !defined(RHINO_SDK_WINVER_H_INCLUDED_)
#define RHINO_SDK_WINVER_H_INCLUDED_


// 21 March 2012 Dale Lear:
//   Windows 7 is the minimum Windows OS that Rhino 6 will run on.
// 25 June 2024 S. Baer
//   Windows 10 is the minimum Windows OS that Rhino 9 will run on.

// If you get a compile error or warning in this file, it is 
// a serious problem.  These values MUST be used by any code
// that McNeel ships with Rhino.  This includes all code that
// gets linked into Rhino.exe and ALL code that gets linked
// into any plug-in that McNeel ships with Rhino.

// Minimum Windows platform = Windows 10 with IE 7.0
//   See http://msdn.microsoft.com/en-us/library/aa383745(v=vs.85).aspx#setting_winver_or__win32_winnt
//   Note: The first 4 digits of RHINO_NTDDI_VERSION must match
//   the value of RHINO_WINVER.

#define RHINO_WINVER 0x0A00 
#define RHINO_NTDDI_VERSION 0x0A000000

// Visual Studio 2012 and 2013's afxwin.h forces _WIN32_IE to be 0x0700
// (We cannot require IE 8.0 while using VS 2012 or 2013.)
#define RHINO_IE_VERSION 0x0700

#if defined(RHINO_COMPILING_CORE_EXE) || defined(RHINO_COMPILING_CORE_PLUGINS)

#define _WIN32_WINNT RHINO_WINVER
#define WINVER RHINO_WINVER
#define _WIN32_WINDOWS RHINO_WINVER
#define NTDDI_VERSION RHINO_NTDDI_VERSION
#define _WIN32_IE RHINO_IE_VERSION

#else

// Allow third party plug-ins to require later
// versions of the Windows Platform SDK.

#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT RHINO_WINVER
#endif
#if !defined(WINVER)
#define WINVER _WIN32_WINNT
#endif
#if !defined(_WIN32_WINDOWS)
#define _WIN32_WINDOWS _WIN32_WINNT
#endif

#if !defined(NTDDI_VERSION)
#define NTDDI_VERSION RHINO_NTDDI_VERSION
#endif

#if !defined(_WIN32_IE)
#define _WIN32_IE RHINO_IE_VERSION
#endif

#endif

#include "rhinoSdkWindowsVersionValidation.h"

#endif
