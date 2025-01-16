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

#pragma once

#if !defined(RHINO_SDK_STDAFX_PREAMBLE_H__)
#define RHINO_SDK_STDAFX_PREAMBLE_H__

// This file should be the first header file included in your 
// plug-in's stdafx.h.


#if defined(__AFX_H__) || defined(_AFX) || defined(DEBUG_NEW)

// If you get this error, be sure that rhinoSdkStdafxPreamble.h
// is the FIRST file included in your plug-in's stdafx.h file.
#error rhinoSdkStdafxPreamble.h MUST be included in your plug-ins stdafx.h file BEFORE any other includes

#endif

#if !defined(_CRT_SECURE_NO_DEPRECATE)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "rhinoSdkWindowsVersion.h"
#include "rhinoSdkChecks.h"

#endif
