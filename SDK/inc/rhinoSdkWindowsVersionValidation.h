// Do not put a #pragma once in this header file.
// It is intentionally included multiple times to
// insure key defines are not changed.

// #pragma once

// These macros are used with #pragma message to print
// diagnostic messgages that show the values of #define
// macros. Usage
// #define RHINO_VAR = 42
// ...
// #pragma message( "RHINO_VAR = " RHINO_PREPROC_STRINGIZE(RHINO_VAR) )
// will print "RHINO_VAR = 42" in the compiler's output log.

#define RHINO_PREPROC_STRINGIZE(s) RHINO_PREPROC_STRINGIZE_HELPER(s)
#define RHINO_PREPROC_STRINGIZE_HELPER(s) #s

// Validation
#if !defined(RHINO_SDK_WINVER_H_INCLUDED_)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if !defined(RHINO_WINVER)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if !defined(RHINO_NTDDI_VERSION)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if !defined(RHINO_IE_VERSION)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if !defined(_WIN32_WINNT)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if !defined(NTDDI_VERSION)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if !defined(_WIN32_IE)
#error Include rhinoSdkWindowsVersion.h first.
#endif

#if _WIN32_WINNT < RHINO_WINVER
#pragma message( "RHINO_WINVER = " RHINO_PREPROC_STRINGIZE(RHINO_WINVER) )
#pragma message( "_WIN32_WINNT = " RHINO_PREPROC_STRINGIZE(_WIN32_WINNT) )
#error Rhino code requires _WIN32_WINNT >= RHINO_WINVER
#endif

#if WINVER < RHINO_WINVER
#pragma message( "RHINO_WINVER = " RHINO_PREPROC_STRINGIZE(RHINO_WINVER) )
#pragma message( "WINVER = " RHINO_PREPROC_STRINGIZE(WINVER) )
#error Rhino code requires WINVER >= RHINO_WINVER
#endif

#if _WIN32_WINDOWS < RHINO_WINVER
#pragma message( "RHINO_WINVER = " RHINO_PREPROC_STRINGIZE(RHINO_WINVER) )
#pragma message( "_WIN32_WINDOWS = " RHINO_PREPROC_STRINGIZE(_WIN32_WINDOWS) )
#error Rhino code requires _WIN32_WINDOWS >= RHINO_WINVER
#endif

#if NTDDI_VERSION < RHINO_NTDDI_VERSION
#pragma message( "RHINO_NTDDI_VERSION = " RHINO_PREPROC_STRINGIZE(RHINO_NTDDI_VERSION) )
#pragma message( "NTDDI_VERSION = " RHINO_PREPROC_STRINGIZE(NTDDI_VERSION) )
#error Rhino code requires NTDDI_VERSION >= RHINO_NTDDI_VERSION
#endif

#if _WIN32_IE < RHINO_IE_VERSION
#pragma message( "RHINO_IE_VERSION = " RHINO_PREPROC_STRINGIZE(RHINO_IE_VERSION) )
#pragma message( "_WIN32_IE = " RHINO_PREPROC_STRINGIZE(_WIN32_IE) )
#error Rhino code requires _WIN32_IE >= RHINO_IE_VERSION
#endif

#if !defined(WINVER) || _WIN32_WINNT != WINVER
// WINVER is an "old" name for _WIN32_WINNT.
// Rhino requires WINVER and _WIN32_WINNT to have identical values.
#pragma message( "WINVER = " RHINO_PREPROC_STRINGIZE(WINVER) )
#pragma message( "_WIN32_WINNT = " RHINO_PREPROC_STRINGIZE(_WIN32_WINNT) )
#error Rhino builds require WINVER and _WIN32_WINNT to have the same value.
#endif

#if !defined(_WIN32_WINDOWS) || _WIN32_WINNT != _WIN32_WINDOWS
// _WIN32_WINDOWS is an "old" name for _WIN32_WINNT.
// Rhino requires _WIN32_WINDOWS and _WIN32_WINNT to have identical values.
#pragma message( "_WIN32_WINDOWS = " RHINO_PREPROC_STRINGIZE(_WIN32_WINDOWS) )
#pragma message( "_WIN32_WINNT = " RHINO_PREPROC_STRINGIZE(_WIN32_WINNT) )
#error Rhino builds require _WIN32_WINDOWS and _WIN32_WINNT to have the same value.
#endif

