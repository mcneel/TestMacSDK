/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
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

#pragma once

#define RHINO_UI_COUNT_OF( array )( sizeof(array) / sizeof( array[0]))

RHINO_SDK_FUNCTION int RhinoRoundOff( double x);
RHINO_SDK_FUNCTION HWND RhinoUiMainFrameHWND();
RHINO_SDK_FUNCTION AFX_MODULE_STATE* RhinoUiModuleState();
RHINO_SDK_FUNCTION CFrameWnd* RhinoUiMainFrame();
RHINO_SDK_FUNCTION CRhinoUiManager& RhinoUiManager();
RHINO_SDK_FUNCTION CRhinoUiDockBarManager& RhinoUiDockBarManager();
RHINO_SDK_FUNCTION CRhinoUiPaintManager& RhinoUiPaintManager();
RHINO_SDK_FUNCTION CRhinoUiResourceManager& RhinoUiResourceManager();
RHINO_SDK_FUNCTION CRhinoUiPageDockBarManager& RhinoUiPageDockBarManager();
RHINO_SDK_FUNCTION CRhinoUiModlessDockingDialogManager& RhinoUiModlessDockingDialogManager();
RHINO_SDK_FUNCTION bool RhinoUiColorDialog( CWnd* pWndParent, ON_Color& color, bool include_button_colors = false, const wchar_t* dialog_title = NULL);
RHINO_SDK_FUNCTION bool RhinoUiScaleLocalizedFonts( UINT& nSize);
