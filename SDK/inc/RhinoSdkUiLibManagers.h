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

#if defined (RHINO_SDK_MFC)
class RHINO_SDK_CLASS CRhinoAppUiResourceManager : public CRhinoUiResourceManager
{
public:
  CRhinoAppUiResourceManager() = default;
  virtual ~CRhinoAppUiResourceManager();

public:
  HCURSOR GetCursor( cursors) override;
  bool GetImageList( images, CRhinoUiImageList&) override;
  COLORREF GetImageMaskingColor() override;
  bool GetBitmap( bitmaps, CRhinoDib&) override;
  void CreateTextCursors();

private:
  HCURSOR m_cursor_tb_shift = 0;
  HCURSOR m_cursor_tb_ctrl  = 0;
  HCURSOR m_cursor_tb_shift_ctrl = 0;
  HCURSOR m_cursor_tb_move = 0;
  HCURSOR m_cursor_tb_link = 0;
  HCURSOR m_cursor_tb_copy = 0;
  HCURSOR m_cursor_tb_delete = 0;
private:
  // no implementation - prohibit use
  CRhinoAppUiResourceManager(const CRhinoAppUiResourceManager&);
  CRhinoAppUiResourceManager& operator=(const CRhinoAppUiResourceManager&);
};
#endif

RHINO_SDK_FUNCTION bool RhinoOnToolBarButtonClicked(const wchar_t* lpsMacro, const wchar_t* lpsTooltip);

#if defined (ON_RUNTIME_WIN)
#define RHINO_UILIB_DEFINE_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)\
  EXTERN_C const GUID DECLSPEC_SELECTANY name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif

// The above macro does not use legal C++.  You cannot both declare and define a variable in a header file that is included everywhere.
// The following lines only declare the global variables and are duplicated in MacHelpers.mm to define the variables once.

#if !defined (ON_RUNTIME_WIN)
#if defined RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE
#define RHINO_UILIB_DEFINE_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)  //Omit these from Mac plug-ins.
#else
#define RHINO_UILIB_DEFINE_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)\
EXTERN_C const GUID DECLSPEC_SELECTANY name;
#endif
#endif

RHINO_UILIB_DEFINE_GUID( RHINO_COMMAND_DOCK_BAR_ID,                  0x1D3D1785, 0x2332, 0x428B, 0xA8, 0x38, 0xB2, 0xFE, 0x39, 0xEC, 0x50, 0xF4);
RHINO_UILIB_DEFINE_GUID( RHINO_OSNAP_DOCK_BAR_ID,                    0xd3c4a392, 0x88de, 0x4c4f, 0x88, 0xa4, 0xba, 0x56, 0x36, 0xef, 0x7f, 0x38);
RHINO_UILIB_DEFINE_GUID( RHINO_VIEW_MANAGER_DOCK_BAR_ID,             0x844b3278, 0xe78e, 0x4844, 0x86, 0x8d, 0x63, 0x9b, 0x9b, 0x65, 0x46, 0x50);
RHINO_UILIB_DEFINE_GUID( RHINO_SELECTION_FILTER_DOCK_BAR_ID,         0x918191ca, 0x1105, 0x43f9, 0xa3, 0x4a, 0xdd, 0xa4, 0x27, 0x68, 0x83, 0xc1);
RHINO_UILIB_DEFINE_GUID( RHINO_CLASH_DOCK_BAR_ID,                    0x770ff06a, 0xd775, 0x41e4, 0xbb, 0xc7, 0x9f, 0x32, 0xed, 0x18, 0x9b, 0x5e);
RHINO_UILIB_DEFINE_GUID( RHINO_EDGE_CONTINUITY_ETO_DOCK_BAR_ID,      0xf7cda9c5, 0xa66c, 0x4ae6, 0x87, 0xb5, 0x89, 0xc2, 0x42, 0x54, 0xe3, 0xb3);
RHINO_UILIB_DEFINE_GUID( RHINO_DRAFT_ANGLE_ETO_DOCK_BAR_ID,          0xFE0CBB9E, 0x336B, 0x4482, 0x9A, 0xBE, 0xB9, 0x4B, 0x35, 0xB0, 0x08, 0xFD);
RHINO_UILIB_DEFINE_GUID( RHINO_CONTOUR_ANALYSIS_DOCK_BAR_ID,         0xa0f238df, 0x1898, 0x47cf, 0x91, 0x9d, 0xf8, 0x4d, 0xe9, 0x5a, 0x0b, 0xb9);
RHINO_UILIB_DEFINE_GUID( RHINO_CURVATURE_GRAPH_ETO_DOCK_BAR_ID,      0xF7F2E719, 0xBC07, 0x42EF, 0xBD, 0xCA, 0x5F, 0xE1, 0xE4, 0xAE, 0xE1, 0x6D);
RHINO_UILIB_DEFINE_GUID( RHINO_ZEBRA_ETO_DOCK_BAR_ID,                0x2B8EE38D, 0xEB9A, 0x4615, 0xB0, 0xB3, 0xE4, 0x33, 0x45, 0x76, 0x38, 0xA7);
RHINO_UILIB_DEFINE_GUID( RHINO_EDGE_ANALYSIS_ETO_DOCK_BAR_ID,        0x62411278, 0xA9B8, 0x4FFF, 0xAA, 0xF6, 0x7A, 0xA3, 0x80, 0x22, 0x7E, 0x0D);
RHINO_UILIB_DEFINE_GUID( RHINO_THICKNESS_ANALYSIS_ETO_DOCK_BAR_ID,   0x1CC988B1, 0xE50D, 0x43C0, 0x8E, 0x21, 0x3F, 0x80, 0xB2, 0xCB, 0x89, 0x80);
RHINO_UILIB_DEFINE_GUID( RHINO_CURVATURE_ANALYSIS_ETO_DOCK_BAR_ID,   0xD41FEA92, 0x55E3, 0x403D, 0x8F, 0x35, 0x62, 0xCF, 0x3D, 0xB3, 0x45, 0xCB);
RHINO_UILIB_DEFINE_GUID( RHINO_DIRECTION_ANALYSIS_ETO_DOCK_BAR_ID,   0xC62FB4FD, 0xCE99, 0x418F, 0xBD, 0x51, 0x3B, 0x46, 0xB1, 0xCC, 0x7E, 0x02);
RHINO_UILIB_DEFINE_GUID( RHINO_END_ANALYSIS_ETO_DOCK_BAR_ID,         0x1A5E7C40, 0x0274, 0x4124, 0xB6, 0x8C, 0x7B, 0x3D, 0xBE, 0xAC, 0x9F, 0x38);
RHINO_UILIB_DEFINE_GUID( RHINO_EMAP_ANALYSIS_ETO_DOCK_BAR_ID,        0xDDECA215, 0x5B08, 0x4EEF, 0xBC, 0xB9, 0xD8, 0xF5, 0xDE, 0x48, 0xC4, 0x51);


//Removed - these control bars are now included in the docking tab system.
//RHINO_UILIB_DEFINE_GUID( RHINO_LAYER_DOCK_BAR_ID,              0x16B641D3, 0x75AF, 0x4D2C, 0x86, 0x1E, 0xB9, 0x6E, 0x38, 0x7B, 0x6F, 0x80);
//RHINO_UILIB_DEFINE_GUID( RHINO_LAYER_MANAGER_DOCK_BAR_ID,      0x2363ed67, 0x616d, 0x4861, 0x9e, 0x87, 0xe2, 0xfe, 0x8f, 0xe7, 0x88, 0xa5);
//RHINO_UILIB_DEFINE_GUID( RHINO_PROPERTIES_DOCK_BAR_ID,         0x20A1348C, 0xD529, 0x44F6, 0x8B, 0xD3, 0xEC, 0x8C, 0x00, 0x1B, 0x2B, 0xB7);
//RHINO_UILIB_DEFINE_GUID( RHINO_NOTES_DOCK_BAR_ID,              0x6641B35A, 0xA21E, 0x4350, 0xA5, 0xF7, 0xCC, 0xCA, 0x3E, 0xC6, 0xAC, 0x72);
//RHINO_UILIB_DEFINE_GUID( RHINO_COMMAND_CONTEXT_BAR_ID,         0x7dd6a049, 0x72ca, 0x457d, 0xbf, 0x12, 0x37, 0x10, 0x0d, 0x7e, 0xe9, 0x67);

RHINO_UILIB_DEFINE_GUID( RHINO_DOCKBARTAB_LAYER,			   0x3610bf83, 0x047d, 0x4f7f, 0x93, 0xfd, 0x16, 0x3e, 0xa3, 0x05, 0xb4, 0x93);
RHINO_UILIB_DEFINE_GUID( RHINO_DOCKBARTAB_PROPERTIES,		   0x34ffb674, 0xc504, 0x49d9, 0x9f, 0xcd, 0x99, 0xcc, 0x81, 0x1d, 0xcd, 0xa2);                     

RHINO_UILIB_DEFINE_GUID(RHINO_DRAG_STRENGTH_DOCK_BAR_ID, 0x50eb6c7b, 0xbe0, 0x4166, 0xad, 0x17, 0xea, 0x69, 0x4c, 0x97, 0xa6, 0xf4);
//
// End of lines copied to MacHelpers.mm
//

#if defined (RHINO_SDK_MFC)
class RHINO_SDK_CLASS CRhinoAppUiDockBarManager : public CRhinoUiDockBarManager
{
public:
  CRhinoAppUiDockBarManager() = default;
  virtual ~CRhinoAppUiDockBarManager();

  class CRhinoStatusBar* GetRhinoStatusBar() const;
  class CRhOsnapToolBar* GetRhinoOsnapToolBar() const;
  class CRhSelectionFilterToolBar* GetRhinoSelectionFilterToolBar() const;

protected:
  bool OnCReateDefaultDockBars() override;
  bool CreateStatusBar( CFrameWnd* pMainFrame) override;
  void OnSetDockStateCompleted() override;

private:
  class CRhinoStatusBar* m_status_bar = nullptr;

  class CRhinoDropTarget* m_pNamedViewDropTarget = 0;
  class CRhinoDropTarget* m_pNamedCPlaneDropTarget = 0;
};

class RHINO_SDK_CLASS CRhinoAppUiManager : public CRhinoUiManager
{
public:
  CRhinoAppUiManager() = default;
  virtual ~CRhinoAppUiManager();

  CRhinoUiResourceManager& ResourceManager() override;
  CRhinoUiDockBarManager& DockBarManager() override;
  CRhinoUiPaintManager& PaintManager() override;
  bool IsPlugInLoaded(const UUID& plug_in_id) const override;
  bool LoadPlugIn(const UUID& plug_in_id, bool fail_quietly = false, bool ignore_load_failed = false) override;
  CRuntimeClass* RuntimeClassFromName(const UUID& plug_in_id, const wchar_t* lpsClassName) override;
  void DisplayDockLocationContextDialog(CDockBar& dock_bar, CPoint pt) override;
  bool ColorDialog(CWnd* pWndParent, ON_Color& color, bool include_button_colors = false, const wchar_t* dialog_title = NULL) override;

protected:
  CRhinoAppUiResourceManager m_test_resource_manager;
};

#endif
