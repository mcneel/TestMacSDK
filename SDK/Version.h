#pragma once

// This file is included by BOTH RC files and C++ files
// This means you have a very limited set of preprocessor instructions
// at your disposal.

// To update version numbers, edit ..\build\build_dates.msbuild
#define RMA_VERSION_MAJOR 9
#define RMA_VERSION_MINOR 0

////////////////////////////////////////////////////////////////
//
// These are set automatically by the build system as the
// first step in each build.
//
#define RMA_VERSION_YEAR   2025
#define RMA_VERSION_MONTH  1
#define RMA_VERSION_DATE   16
#define RMA_VERSION_HOUR   10
#define RMA_VERSION_MINUTE 31

////////////////////////////////////////////////////////////////
//
// branch = 0 to 3
//   This number identifies the branch used in the build.
//
//   The build system automatically sets the value to
//   1, 2 or 3 before compiling any code.
//
//   The file checked into the source code repository
//   always has branch set to 0.
//     0 = developer build
//     1 = build system trunk build
//     2 = build system release candidate build
//     3 = build system release build
#define RMA_VERSION_BRANCH 5

#define VERSION_WITH_COMMAS  9,0,25016,10315
#define VERSION_WITH_PERIODS 9.0.25016.10315
#define COPYRIGHT "Copyright (C) 1993-2025, Robert McNeel & Associates. All Rights Reserved."
#define SPECIAL_BUILD_DESCRIPTION "Rhino WIP"

#define RMA_VERSION_NUMBER_MAJOR_STRING   "9"
#define RMA_VERSION_NUMBER_MAJOR_WSTRING L"9"
#define RMA_PREVIOUS_VERSION_NUMBER_MAJOR_WSTRING L"8"

#define RMA_VERSION_NUMBER_SR_STRING   "SR0"
#define RMA_VERSION_NUMBER_SR_WSTRING L"SR0"

#define RMA_VERSION_WITH_PERIODS_STRING   "9.0.25016.10315"
#define RMA_VERSION_WITH_PERIODS_WSTRING L"9.0.25016.10315"

// BEGIN - OBSOLETE SVN INFORMATION ////////////////////////////
////////////////////////////////////////////////////////////////
//
// As of Noveber 10, 2016 the McNeel build process changes the
// svn source revison string from "0" to a string containing
// the git revision SHA-1 hash in hexadecimal format.

// OBSOLETE - current set to git hash
#define RMA_SRC_SVN_REVISION "77231544fea8a7e7e312a3ac40ab954ac5d5d551"

// OBSOLETE - never changed
#define RMA_SRC_SVN_BRANCH ""

// OBSOLETE - never changed
#define RMA_DOC_SVN_REVISION "0"

// OBSOLETE - never changed
#define RMA_DOC_SVN_BRANCH ""

//
// END - OBSOLETE SVN INFORMATION //////////////////////////////

// git revision SHA-1 hash as char hexadecimal string
#define RMA_GIT_REVISION_HASH_STRING RMA_SRC_SVN_REVISION
#define RMA_GIT_REVISION_HASH_WSTRING L""

// git branch name as char string
#define RMA_GIT_BRANCH_NAME_STRING ""
#define RMA_GIT_BRANCH_NAME_WSTRING L""

#define GOOGLE_ANALYTICS_TRACKING_ID L"UA-512742-12"

// RHINO_FILE_FLAGS_MASK can be one or more of the following:
// VS_FF_DEBUG        - File contains debugging information or is compiled with debugging features enabled.
// VS_FF_PATCHED      - File has been modified and is not identical to the original shipping file of the same version number.
// VS_FF_PRERELEASE   - File is a development version, not a commercially released product.
// VS_FF_PRIVATEBUILD - File was not built using standard release procedures. If this value is given, the StringFileInfo block must contain a PrivateBuild string.
// VS_FF_SPECIALBUILD - File was built by the original company using standard release procedures but is a variation of the standard file of the same version number. If this value is given, the StringFileInfo block block must contain a SpecialBuild string.
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(WINDOWS) || defined(_WINDOWS_) || defined(__WINDOWS__)
#if !defined(VS_FF_PRERELEASE)
// At this time, verrsrc.h does not have protection against multiple includes.
// Testing for VS_FF_PRERELEASE seems to prevent double incudes and the
// redef errors it generates.
#include "verrsrc.h"
#endif
#endif

#define RHINO_FILE_FLAGS_MASK VS_FF_PRERELEASE

// BEGIN - LICENSE GUID SECTION ////////////////////////////////
////////////////////////////////////////////////////////////////
//

#if !defined(ON_UUID_DEFINED)
  // This code also exists in opennurbs_uuid.h, but needs to be here
  // for include purposes
  #define ON_UUID_DEFINED
  // ON_UUID is a 16 byte universally unique identifier
  #if defined(UUID_DEFINED)
  typedef UUID ON_UUID;
  #elif defined(GUID_DEFINED)
  typedef GUID ON_UUID;
  #else
  typedef struct ON_UUID_struct
  {
    unsigned int   Data1;    // 32 bit unsigned integer
    unsigned short Data2;    // 16 bit unsigned integer
    unsigned short Data3;    // 16 bit unsigned integer
    unsigned char  Data4[8];
  } ON_UUID;
  #endif
#endif

#if RMA_VERSION_MAJOR == 8
// {59FF75C9-9C71-4EF8-A290-6B590F3FC63A}
static const ON_UUID LICENSE_GUID_V7_COMMERCIAL =
{ 0x59ff75c9, 0x9c71, 0x4ef8, { 0xa2, 0x90, 0x6b, 0x59, 0xf, 0x3f, 0xc6, 0x3a } };

// {868C63F5-3760-4A45-8600-5399CC57B47C}
static const ON_UUID LICENSE_GUID_V8_COMMERCIAL =
{ 0x868c63f5, 0x3760, 0x4a45, { 0x86, 0x0, 0x53, 0x99, 0xcc, 0x57, 0xb4, 0x7c } };

#define LICENSE_GUID_PREV_VERSION LICENSE_GUID_V7_COMMERCIAL
#define LICENSE_GUID_THIS_VERSION LICENSE_GUID_V8_COMMERCIAL

#elif RMA_VERSION_MAJOR == 9
// {868C63F5-3760-4A45-8600-5399CC57B47C}
static const ON_UUID LICENSE_GUID_V8_COMMERCIAL =
{ 0x868c63f5, 0x3760, 0x4a45, { 0x86, 0x0, 0x53, 0x99, 0xcc, 0x57, 0xb4, 0x7c } };

// {953A29B7-895D-4389-9374-F5106F10D17D}
static const ON_UUID LICENSE_GUID_V9_COMMERCIAL =
{ 0x953a29b7, 0x895d, 0x4389, { 0x93, 0x74, 0xf5, 0x10, 0x6f, 0x10, 0xd1, 0x7d } };

#define LICENSE_GUID_PREV_VERSION LICENSE_GUID_V8_COMMERCIAL
#define LICENSE_GUID_THIS_VERSION LICENSE_GUID_V9_COMMERCIAL

#else
#error Generate new UUIDs for license files.
// When updating this file for Rhino 9,
// also be sure to add the new GUID to IsCoreRhino() in
// src4\Zoo\ZooClient\ClientManager.cs
#endif

//
// END - LICENSE GUID SECTION //////////////////////////////////
