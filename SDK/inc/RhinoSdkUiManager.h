//
// Copyright (c) 1993-2015 Robert McNeel & Associates. All rights reserved.
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

class RHINO_SDK_CLASS CRhinoUiManager
{
public:
  CRhinoUiManager();
  virtual ~CRhinoUiManager();

  virtual CRhinoUiDockBarManager& DockBarManager();
  virtual CRhinoUiPaintManager& PaintManager();
  virtual CRhinoUiPageDockBarManager& PageDockBarManager();
  virtual CRhinoUiModlessDockingDialogManager& ModlessDockingDialogManager();

  virtual CRhinoUiResourceManager& ResourceManager() = 0;

  virtual bool IsPlugInLoaded( const UUID& plug_in_id) const = 0;
  virtual bool LoadPlugIn( const UUID& plug_in_id, bool fail_quietly = false, bool ignore_load_failed = false) = 0;
  virtual CRuntimeClass* RuntimeClassFromName( const UUID& plug_in_id, const wchar_t* lpsClassName) = 0;
  virtual void DisplayDockLocationContextDialog( CDockBar& dock_bar, CPoint pt) = 0;
  virtual bool ColorDialog( CWnd* pWndParent, ON_Color& color, bool include_button_colors = false, const wchar_t* dialog_title = NULL);
protected:
  friend void RhOnDestroyMainFrame();
  void OnDestroyMainFrame();

  CRhinoUiDockBarManager* m_dock_bar_manager = nullptr;
  CRhinoUiPaintManager* m_paint_manager = nullptr;
  CRhinoUiPageDockBarManager* m_page_dock_bar_manager = nullptr;
  CRhinoUiModlessDockingDialogManager* m_modless_docking_dialog_manager = nullptr;
};
