//
// Copyright (c) 1993-2024 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

// 7 June 2004
/// DO NOT USE //// pragma once
// It is important that this file be included multiple times in order
// for the checking to happen at the right places.  Please do not
// put pragma once in this file.  Ask Dale Lear if you have
// questions.


// the following line must be the first line in this file that is not a comment

#define RHINO_SDK_H_IN_PROGRESS_

//
//
// NEVER INCLUDE FILES FROM CORE RHINO IN ANY SDK/inc/rhinoSDK*.h file
//
//

#if !defined(OPENNURBS_IMPORTS)
#define OPENNURBS_IMPORTS
#endif
#if !defined(OPENNURBS_PLUS)
#define OPENNURBS_PLUS
#endif

#if !defined(ON_RUNTIME_APPLE) && !defined(ON_RUNTIME_WIN) && !defined(ON_RUNTIME_LINUX)
// 3rd party Rhino SDK plug-in developer path to openNURBS tools
#include "../opennurbs/opennurbs_system.h"
#endif

#if defined(ON_RUNTIME_APPLE) && defined(ON_RUNTIME_WIN)
#error Exactly one runtime must be defined.
#endif

#if !defined(ON_RUNTIME_APPLE) && !defined(ON_RUNTIME_WIN) && !defined(ON_RUNTIME_LINUX)
#error Exactly one runtime must be defined.
#endif

#if !defined(RHINO_WIP_INC_)
#if !defined(OPENNURBS_INC_)
// 3rd party Rhino SDK plug-in developer path to rhino_wip.h
#include "../rhino_wip.h"
#else
// internal McNeel build path to rhino_wip.h
#include "../../../rhino_wip.h"
#endif
#endif

#if defined(_M_X64) && defined(WIN32) && defined(WIN64) && !defined(OPENNURBS_INC_)
//   This is a non-system plug-in for a V5 x64 build configuration.
//
//   The afxwin.h includes afx.h which includes afxver_.h 
//   and afxver_.h unconditionally defines WIN32.  This is a bug
//   and the hope is this simple undef will let us continue with
//   exactly one of WIN32 or WIN64 defined.  For most plug-ins, 
//   rhinoSdk.h is the first include after the afx*.h includes in 
//   the plug-in's stdafx.h file.  This undef is a hack to minimize
//   the confusion while we get the x64 version going.
//
//   Note well that all Windows builds (32 bit and 64 bit)
//   define _WIN32.  Only 64 bit builds define _WIN64.
//   Our makefiles and header files should never define or
//   undef the underbar versions of _WIN32 or _WIN64.  Our
//   Windows makefiles should define EXACTLY one of WIN32
//   or WIN64.
//
//   See the MSDN "Predefined Macros" help file for details.
#undef WIN32
#endif

#if defined(WIN32) || defined(WIN64)
#include "rhinoSdkWindowsVersion.h"
#endif

#include "rhinoSdkChecks.h"

#if !defined(RHINO_SDK_H__INCLUDED__)
#define RHINO_SDK_H__INCLUDED__

#if defined (ON_RUNTIME_WIN) || defined(ON_RUNTIME_LINUX)
typedef BOOL BOOL32;
#else
typedef signed char BOOL32;
#endif

// This define may be used in code that needs to know
// which version of Rhino is being built but wants to
// avoid including rhinoSdkVersion.h to limit the
// number of files that are compiled when the build
// system increments the version numbers.

#define RHINO_6_OR_LATER
#define RHINO_7_OR_LATER
#define RHINO_8_OR_LATER
//#define RHINO_9_OR_LATER

// Ideally, RHINO_NOEXCEPT would be the C++11 "noexcept" specifier.
// Less than ideally, Visual Studio 2012 does not support "noexcept"
// and we use the, almost, equivalent "throw()". The key point
// being that it is important to indicate at compile time 
// that a function, especially constructor and operator=, does 
// not throw exceptions.
#define RHINO_NOEXCEPT ON_NOEXCEPT

// OBSOLETE in Rhino 8.x and later // #define RHINO_V8SR 0

#if defined(_DEBUG)
// If RHINO_DEBUG_PLUGIN is defined, then a plug-in
// developer is making a "debug" plug-in that links 
// with release build of rhino.  If in this case
// RHINO_SDK_DEBUG must NOT be defined.
//
// If RHINO_DEBUG_PLUGIN is not defined and a system
// plug-in is being compiled, then RHINO_SDK_DEBUG
// must be defined before the Rhino SDK headers are
// parsed.

#if !defined(RHINO_DEBUG_PLUGIN)
#if !defined(RHINO_SDK_DEBUG)
#define RHINO_SDK_DEBUG
#endif
#endif

#endif

#if defined (RHINO_SDK_MFC)
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#endif

#if defined(RHINO_SDK_CLASS)
// 2017-07-07, BG: Don't use GetStockObject(DEFAULT_GUI_FONT).
// Instead, use ::RhinoUiPaintManager().GetUiFont().
#undef DEFAULT_GUI_FONT
#define DEFAULT_GUI_FONT "Use ::RhinoUiPaintManager().GetUiFont() instead"
#endif

#if defined(RHINO3_CORE_STUFF_H__INCLUDED_)
// SDK\inc\rhino3Sdk.h must be included before rhino3CoreStuff.h
#error You goofed up.  
#endif

// Use MAC_DEPRECATED to decorate functions that should not be used in Rhino for Mac
#if defined (ON_RUNTIME_APPLE)
#define MAC_DEPRECATED  ON_DEPRECATED
#else
#define MAC_DEPRECATED
#endif

#if !defined(RHINO_SDK_CLASS)


// When the core Rhino executable is being compiled, RHINO_SDK_CLASS
// is defined in Rhino's stdafx.h as __declspec(dllexport).  
// If RHINO_SDK_CLASS is not defined, then it means a Rhino plug-in
// is being compiled.
#define RHINO_PLUGIN_COMPILE

// A Rhino plug-in is being compiled - import Rhino SDK symbols

#if !defined(_UNICODE)
#error All Rhino plug-ins must be UNICODE plugins.
#endif

#if defined(_MBCS) ||  defined(MBCS)
#error Rhino plug-ins cannot be compiled with _MBCS or MBCS defined
#endif

#if defined (RHINO_SDK_MFC)

#if defined(_MSC_VER)
#if defined(_DEBUG)
  #pragma message( " --- rhinoSDK.h --- Building Debug Rhino plug-in" )
#else
  #pragma message( " --- rhinoSDK.h --- Building Release Rhino plug-in" )
#endif
#endif

#if defined(_MSC_VER)

#if ( 1900 <= _MSC_VER )
#pragma message( " --- rhinoSDK.h --- Using Visual Studio compiler tools." )
#else
#error Rhino cannot use this version of the compiler.
#endif

#endif

#if defined(_MFC_VER)

#if ( 0x0E00 == _MFC_VER )
#pragma message( " --- rhinoSDK.h --- Using Windows Platform SDK (MFC 14.x)." )
#else
#error Rhino cannot use this Windows Platform SDK.
#endif

#endif

#endif //RHINO SDK_MFC

#if !defined(RHINO_PLUG_IN_DECLARE)
// If you get this error, then include .../SDK/inc/rhinoSdkPlugInDeclare.h
// on the line immediately before the line that has RHINO_PLUG_IN_DECLARE
// - The definition of RHINO_PLUG_IN_DECLARE depends on values defined in
//   rhinoSdkVersion.h that are frequently changed by the build system.
// - Using rhinoSdkPlugInDeclare.h to define RHINO_PLUG_IN_DECLARE dramatically
//   reduces the amount of code developers have to build when this
//   version information is updated.
#define RHINO_PLUG_IN_DECLARE #error include .../SDK/inc/rhinoSdkPlugInDeclare.h in the .cpp file where you are seeing this error.
#define RHINO_PLUG_IN_DECLARE_IS_ERROR
#endif

#if defined(ON_RUNTIME_WIN)
#define RHINO_SDK_EXPORT __declspec(dllexport)
#else
#define RHINO_SDK_EXPORT  __attribute__ ((visibility ("default")))
#endif

#define RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(sDeveloperOrganization) extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperOrganization(void) {return sDeveloperOrganization;}
#define RHINO_PLUG_IN_DEVELOPER_ADDRESS(sDeveloperAddress)           extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperAddress(void)      {return sDeveloperAddress;}
#define RHINO_PLUG_IN_DEVELOPER_COUNTRY(sDeveloperCountry)           extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperCountry(void)      {return sDeveloperCountry;}
#define RHINO_PLUG_IN_DEVELOPER_PHONE(sDeveloperPhone)               extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperPhone(void)        {return sDeveloperPhone;}
#define RHINO_PLUG_IN_DEVELOPER_FAX(sDeveloperFax)                   extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperFax(void)          {return sDeveloperFax;}
#define RHINO_PLUG_IN_DEVELOPER_EMAIL(sDeveloperEmail)               extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperEmail(void)        {return sDeveloperEmail;}
#define RHINO_PLUG_IN_DEVELOPER_WEBSITE(sDeveloperWebsite)           extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDeveloperWebsite(void)      {return sDeveloperWebsite;}
#define RHINO_PLUG_IN_UPDATE_URL(sUpdatePlugInURL)                   extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInUpdateURL(void)             {return sUpdatePlugInURL;}

// If you want to use a .rhi automagical installer file, then your plug-in
// must contain extern "C" const wchar_t* RhinoPlugInName() and a ON_UUID RhinoPlugInId()
// functions. The argument to the RHINO_PLUG_IN_ID() macro is a uuid in standard
// Windows string format; e.g., RHINO_PLUG_IN_ID(L"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX")
#define RHINO_PLUG_IN_NAME(sPlugInName)    extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInName(void) {return sPlugInName;}
#define RHINO_PLUG_IN_ID(sPlugInId)        extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInId(void) {return sPlugInId;}

// Note, this macro returns const char* in order to support the
// standard predefined __DATE__ and __TIME__ macros.
#define RHINO_PLUG_IN_VERSION(sPlugInVersion) extern "C" RHINO_SDK_EXPORT const char* RhinoPlugInVersion(void) {return sPlugInVersion;}

// If your plug-in requires other plug-ins to be loaded before,
// it can be loaded, then include the macro 
// RHINO_PLUG_IN_DEPENDENCIES(L"id1") where id1 is the id of 
// the plug-in that must be loaded first.
// If you require multiple plug-ins to be loaded, use commas
// to delimit the ids RHINO_PLUG_IN_DEPENDENCIES(L"id1,id2").
// The id should have the format be in the standard format, i.e.
// RHINO_PLUG_IN_DEPENDENCIES(L"14D4C6C1-F5FD-4ee4-B2C0-FC7B6CEDF22D")
// In general, using plug-in dependencies is strongly discouraged.
// Use properly designed and installed DLLs instead.
#define RHINO_PLUG_IN_DEPENDENCIES(sIdList) extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDependencies(void) {return sIdList;}

// If you want your plug-in to provide an icon for use in various user interfaces,
// both inside and outside of Rhino, then include the RHINO_PLUG_IN_ICON_RESOURCE_ID
// macro and specify the resource id of your plug-in's icon.
// For example: RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_MY_PLUGIN_ICON)
#define RHINO_PLUG_IN_ICON_RESOURCE_ID(nIconResourceId) extern "C" RHINO_SDK_EXPORT unsigned int RhinoPlugInIconResourceId(void) { return nIconResourceId; }

// If you want your plug-in to provide a friendly description for use in 
// various user interfaces, both inside and outside of Rhino, then include
// the RHINO_PLUG_IN_DESCRIPTION macro and specify the resource id of your 
// plug-in's description.
#define RHINO_PLUG_IN_DESCRIPTION(sPlugInDescription) extern "C" RHINO_SDK_EXPORT const wchar_t* RhinoPlugInDescription(void) { return sPlugInDescription; }

#if defined(ON_RUNTIME_WIN)
#define RHINO_SDK_CLASS __declspec(dllimport)
#define RHINO_SDK_DATA __declspec(dllimport)
#define RHINO_SDK_FUNCTION __declspec(dllimport)
#define RHINO_SDK_TEMPLATE extern
#else
#define RHINO_SDK_CLASS __attribute__ ((visibility ("default")))
#define RHINO_SDK_DATA __attribute__ ((visibility ("default")))
#define RHINO_SDK_FUNCTION __attribute__ ((visibility ("default")))
#define RHINO_SDK_TEMPLATE __attribute__ ((visibility ("default")))
#endif

#if defined (ON_RUNTIME_WIN)
#pragma warning( push )

// Windows support for theme drawing.
#include <uxtheme.h>
#include <vssym32.h>

#if defined (RHINO_SDK_MFC)
#include <afxtempl.h>
#endif

#pragma warning( pop )
#endif

#if !defined(OPENNURBS_INC_)
// 3rd party Rhino SDK plug-in developer path to openNURBS tools
#include "../opennurbs/opennurbs.h"
#endif

#if defined (ON_RUNTIME_APPLE)
#if !defined (ON_RUNTIME_APPLE_IOS)
#define RHINO_MAC_APPLICATION
#endif
#endif

#if defined (ON_RUNTIME_APPLE)
#if defined (RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE)
#include "rhinoSdkMacTypes.h"
#else
#include "../../AfxMac.h"
#endif
#endif


#endif


//
//
// NEVER INCLUDE FILES FROM CORE RHINO IN ANY SDK/inc/rhinoSDK*.h file
//
//

#include "rhinoSdkRhinoVersion.h"
#include "RhinoSdkYesNoResponse.h"
#include "rhinoSdkCoreClasses.h"
#include "rhinoSdkDisplayEngineAttributes.h"
#include "rhinoSdkPick.h"
#include "RhinoSdkSpaceMorph.h"
#include "rhinoSdkBitmap.h"
#include "rhinoSdkMaterial.h"
#include "rhinoSdkTexture.h"
#include "rhinoSdkGroup.h"
#include "rhinoSdkDimStyle.h"
#include "rhinoSdkHatchPattern.h"
#include "rhinoSdkFont.h"
#include "rhinoSdkUiProfileContext.h"
#include "RhinoSdkPersistentSettings.h"
#if defined (ON_RUNTIME_APPLE)
#include "RhinoSdkMacUiProfileContext.h"
#endif
#include "rhinoSdkAppSettings.h"
#include "rhinoSdkCommand.h"
#include "rhinoSdkFileMenu.h"
#include "rhinoSdkCommandOption.h"
#include "rhinoSdkFileOptions.h"
#include "RhinoSdkPages.h"
#include "RhinoSdkPropertiesPages.h"
#include "rhinoSdkPlugIn.h"
#include "rhinoSdkRenderPlugIn.h"
#include "RhinoSdkRender.h"
#include "rhinoSdkApp.h"
#include "rhinoSdkLinetype.h"
#include "rhinoSdkObjectAttributes.h"
#include "rhinoSdkObject.h"
#include "rhinoSdkLight.h"
#include "rhinoSdkPointObject.h"
#include "rhinoSdkCurveObject.h"
#include "rhinoSdkMeshObject.h"
#include "rhinoSdkBrepObject.h"
#include "rhinoSdkExtrusionObject.h"
#include "RhinoSdkSubD.h"
#include "rhinoSdkAnnotationObject.h"
#include "rhinoSdkHatchObject.h"
#include "rhinoSdkGroupObject.h"
#include "rhinoSdkFileMenu.h"
#include "rhinoSdkApp.h"
#include "rhinoSdkLayer.h"
#include "rhinoSdkAppSettings.h"
#include "rhinoSdkGet.h"
#include "rhinoSdkGetPoint.h"
#include "rhinoSdkGetLine.h"
#include "rhinoSdkGetPlane.h"
#include "rhinoSdkGetArc.h"
#include "rhinoSdkGetCircle.h"
#include "rhinoSdkGetCone.h"
#include "rhinoSdkGetCurve.h"
#include "rhinoSdkGetCylinder.h"
#include "rhinoSdkGetSphere.h"
#include "rhinoSdkGetParabola.h"
#include "rhinoSdkGetString.h"
#include "rhinoSdkGetObject.h"
#include "rhinoSdkGetFileDialog.h"
#include "RhinoSdkUiIncludes.h"
#include "rhinoSdkDoc.h"
#include "rhinoSdkMouse.h"
#include "rhinoSdkCommand.h"
#include "rhinoSdkCommandOption.h"
#include "rhinoSdkObjectHistory.h"
#include "rhinoSdkEventWatcher.h"
#include "rhinoSdPropertiesPanelPageEventWatcher.h"
#include "RhinoSdkPages.h"
#include "rhinoSdkGetDistance.h"
#include "rhinoSdkGetDirection.h"
#include "rhinoSdkGetAngle.h"
#include "RhinoSdkDialog.h"
#include "RhinoSdkUiDockbarTab.h"
#include "rhinoSdkLayerComboBox.h"
#include "RhinoSdkObjectColorComboBox.h"
#include "RhinoSdkDimStyleComboBox.h"
#include "RhinoSdkColorButton.h"
//#include "rhinoSdkStackedDialogPage.h"
#include "rhinoSdkGetText.h"
#include "rhinoSdkGetFrame.h"
#include "rhinoSdkGetRevolve.h"
#include "RhinoSdkDib.h"
#include "rhinoSdkLoft.h"
#include "rhinoSdkGetView.h"
#include "rhinoSdkGetTorus.h"
#include "rhinoSdkFair.h"
#include "rhinoSdkGetXform.h"
//#include "rhinoSdkObjectPropertiesDialogPage.h"
//#include "rhinoSdkOptionsDialogPage.h"
#include "rhinoSdkExtrude.h"
#include "rhinoSdkGetRectangle.h"
#include "rhinoSdkFileUtilities.h"
#include "RhinoSdkDirectoryManager.h"
#include "rhinoSdkGetEllipse.h"
#include "rhinoSdkGetSubCurve.h"
#include "rhinoSdkPolyEdge.h"
#include "rhinoSdkGetPolygon.h"
#include "rhinoSdkCageObject.h"
#include "rhinoSdkGetSpiral.h"
#include "rhinoSdkViewport.h"
#include "RhinoSdkDisplay.h"
#include "rhinoSdkView.h"
#include "rhinoSdkGrips.h"
#include "RhinoSdkRenderViewWnd.h"
#include "RhinoSdkRenderFrame.h"
#include "rhinoSdkSweep.h"
#include "rhinoSdkDetailObject.h"
#include "rhinoSdkPrintInfo.h"
#include "RhinoSdkTextOut.h"
#include "RhinoSdkUiLibManagers.h"
#include "rhinoSdkPrintDialog.h"
#include "RhinoSdkPageLayoutView.h"
#include "RhinoSdkContextMenu.h"
#include "RhinoSdkUiXML.h"
#include "rhinoSdkMeshUtilities.h"
#include "rhinoSdkSkinDll.h"
#include "rhinoSdkDigitizerInfo.h"
#include "rhinoSdkClippingPlane.h"
#include "RhinoSdkDropTargetManager.h"
#include "rhinoSdkAnalysisModes.h"
#include "rhinoSdkSnap.h"
#include "RhinoSdkDigitizerManager.h"
#include "RhinoSdkWorkerThread.h"
#include "rhinoSDKOffsetCrvOnSrf.h"
#include "rhinoSdkUtilities.h"
#include "RhinoSdkBlend.h"
#include "rhinoSdkUnrollSrf.h"
#include "RhinoSdkFitPatch.h"
#include "rhinoSdkGumball.h"
#include "rhinoSdkWorkSession.h"
#include "RhinoSdkRichText.h"
#include "RhinoSdkLeader.h"
#include "RhinoSdkMatchCurve.h"
#include "rhinoSdkWidget.h"
#include "RhinoSdkDimension.h"
#include "RhinoSdkDimLinear.h"
#include "RhinoSdkDimAngular.h"
#include "rhinoSdkDimRadius.h"
#include "RhinoSdkDpi.h"
#include "rhinoSdkTPages.h"
#include "rhinoSdkSnapshots.h"

#include "rhinoSdkSectionStyle.h"

#if !defined(ON_RUNTIME_APPLE_IOS) && !defined(ON_RUNTIME_LINUX)
#include "RhinoSdkUiFile.h"
#endif

#if defined (RHINO_MAC_APPLICATION)
#include "RhinoSdkMacRhinoApp.h"
#endif

// Last: Make sure nothing has gone haywire.
#include "rhinoSdkChecks.h"

#if defined(RHINO3_CORE_STUFF_H__INCLUDED_)
// SDK\inc\rhino3Sdk.h must never include core rhino header files.
#error You goofed up.  
#endif

#if !defined(RHINO_V6_READY)
// When updating your plug-in to work with Rhino V6, it is
// imporant to know about new objects and features in Rhino V6.
// Please visit http://wiki.mcneel.com/developer/rhino/6/sdkfeatures for details.
// Then, add the line
//   #define RHINO_V6_READY
// to your plug-in's stdafx.h file.

#error If you get this error, read the comment above this line.

#endif

#endif

///////////////////////////////////////////////////////////
//
// Turn on selected /W4 warnings for core code compiling
//
#if defined(RHINO_COMPILING_CORE_EXE)

#pragma warning(3:4189) // variable not used
//#pragma warning(3:4211) // nonstandard extension used: redefined extern to static
//#pragma warning(3:4238) // nonstandard extension used : class rvalue used as lvalue
//#pragma warning(3:4239) // nonstandard extension used : argument conversion
//#pragma warning(3:4244) // possible loss of data in conversion
//#pragma warning(3:4245) // signed/unsigned mismatch in conversion
#pragma warning(3:4310) // cast truncates constant value
//#pragma warning(3:4366) // result of unary '&' operator may be unaligned
#pragma warning(3:4366) // signed/unsigned mismatch
//#pragma warning(3:4510) // default constructor could not be generated
#pragma warning(3:4516) // access-declarations are deprecated
//#pragma warning(3:4610) // user defined constructor required
#pragma warning(3:4701) // potential uninitialized variable
#pragma warning(3:4702) // unreachable code
//#pragma warning(3:4706) // assignment within conditional expression

#else // warnings we want turned on in plug-ins

#pragma warning(3:4189) // variable not used
#pragma warning(3:4310) // cast truncates constant value
#pragma warning(3:4366) // signed/unsigned mismatch
#pragma warning(3:4516) // access-declarations are deprecated
#pragma warning(3:4701) // potential uninitialized variable
#pragma warning(3:4702) // unreachable code

#endif
//
///////////////////////////////////////////////////////////


// this line must be the last line in this file that is not a comment
#undef RHINO_SDK_H_IN_PROGRESS_
