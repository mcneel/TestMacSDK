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

#if !defined(RHINO_SDK_STDAFX_PREAMBLE_H__)
#error rhinoSdkStdafxPreamble.h MUST be included in your plug-in stdafx.h file BEFORE any other includes.
#endif

#if !defined(RHINO_SDK_CORE_CLASSES_H__INCLUDED__)
#define RHINO_SDK_CORE_CLASSES_H__INCLUDED__

#pragma once

#if defined(_UNICODE) && defined(_MBCS)
#error _UNICODE and _MBCS are both defined - choose exactly one.
#endif

#if !defined(_UNICODE) && !defined(_MBCS)
#error Neither _UNICODE nor _MBCS is defined - choose exactly one.
#endif

// The classes listed below are defined in core Rhino and are not
// part of the Rhino SDK.
//
// Core Rhino Developers:
//
//   All core Rhino forward class declarations go here.
//   List in alpha order.
//   Do NOT put core Rhino forwared class declarations in rhinoSdk*.h files.
//

class CRhinoSnapContext;
class CRhinoSnapEvent;
class CRhinoObjRef;

#endif
