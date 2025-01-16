/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2014 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

// rhinoSdkPlugInLinkingPragmas.h : defines linking pragmas for the SDK plug-ins.
//

#pragma once

#if !defined(RHINO_LIB_DIR)
#error RHINO_LIB_DIR is not defined.
#endif

#pragma message (" --- Rhino C++ component links with libs in " OPENNURBS_PP2STR(RHINO_LIB_DIR) )
#pragma comment(lib, RHINO_LIB_DIR "/RhinoCore.lib")
#pragma comment(lib, RHINO_LIB_DIR "/opennurbs.lib")
#pragma comment(lib, RHINO_LIB_DIR "/rdk.lib")
