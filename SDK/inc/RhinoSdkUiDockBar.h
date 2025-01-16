// Copyright (c) 1993-2016 Robert McNeel & Associates. All rights reserved.
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

#include "RhinoSdkDialog.h"

class RHINO_SDK_CLASS CRhinoUiDockBar : public CControlBar
{
	DECLARE_DYNCREATE(CRhinoUiDockBar)

protected:
	CRhinoUiDockBar();
  const CRhinoUiDockBar& operator=( const CRhinoUiDockBar&);
  virtual ~CRhinoUiDockBar();

public:
  // Returns:
  //   Unique id which identifies this dock bar.  This should be persistent
  //   as this id will be used to serialize size, state and position information
  //   relative to this dock bar when Rhino is shutdown and start up.
  virtual ON_UUID DockBarID() const;

  // Description:
  //   There is no requirement for dock bars to have unique names.  Dock bars
  //   must have a name so that the dock bar manager GUI can reference this
  //   control dock bar.
  // Parameters:
  //   languageId [in] Localized language version control_bar_name is using
  // Returns:
  //   Display name for this dock bar
  virtual const wchar_t* DockBarName(int languageId = 1033) const;

  // Description:
  //   Called when dock bar is being created.  This is where controls which
  //   are to be embedded in this dock bar should be created.
  // Returns:
  //   true If controls are created successfully.  false If they are not.  Returning
  //   false will cause CRhinoUiDockBar::CreateRhinoDockBar() to fail.
  virtual bool CreateDockBarControls();

  // Description:
  //   Called when dock bar position is being read.
  // Parameters:
  //   lpszProfileName [in] Profile section being read
  virtual void LoadState(LPCTSTR lpszProfileName = NULL);

  // Description:
  //   Called when dock bar position is being saved.
  // Parameters:
  //   lpszProfileName [in] Profile section being written
  virtual void SaveState(LPCTSTR lpszProfileName = NULL);


  // Description:
  //   Called when time to draw non client caption for docked dock bars.
  // Parameters:
  //   pDC [in/out] Non client device context to draw into.
  //   rect [in] Caption rectangle in pDC coordinates.
  virtual void DrawDockedCaption(CDC* pDC, const CRect& rect);

  // Description:
  //   Called prior to closing the dock bar
  // Returns:
  //   true (default) If dock bar is allowed to close.
  virtual bool CanClose();

  // Description:
  //   Called when beginning to drag a control bar
  // Parameters:
  //   dragActionStarting [in] true then drag action is starting;
  //                      otherwise drag action is finishing.
  // Returns:
  //   If dragActionStarting is true return true to allow drag action or false to stop.
  //   If dragActionStarting is false then the return value is ignored.
  virtual bool OnDockContextStartDrag(bool dragActionStarting);

  // Description:
  //   Called just prior to and just after tool bars docking state is being
  ///  toggled.
  // Parameters:
  //   priorToStateChange [in] If true then it is being called prior to state changing;
  //               otherwise state has changed.
  virtual void OnToggleDocking(bool priorToStateChange);
 
  // Description:
  //   Called just prior to and just after dock bar is stretched while docked.
  // Parameters:
  //   bDoneTracking [in] If true then it stretching is complete; otherwise just
  //                      just getting started.
  virtual void OnStartTracking( bool bDoneTracking);

  // Description:
  //   Called when a dock bars docked location has changed.
  // Parameters:
  //   newLocation [in] New dock location, can be one of the following:
  //                       AFX_IDW_DOCKBAR_TOP
  //                       AFX_IDW_DOCKBAR_BOTTOM
  //                       AFX_IDW_DOCKBAR_LEFT
  //                       AFX_IDW_DOCKBAR_RIGHT
  //                       AFX_IDW_DOCKBAR_FLOAT
  virtual void OnDockBarPositionChanged(DWORD newLocation);

  // Description:
  //   Called when a dock bar has been shown or hidden.
  // Parameters:
  //   show [in] If dock bar is to be shown this will be true otherwise
  //             it will be false.
  ON_DEPRECATED_MSG("Replaced by CRhinoUiDockBar::OnShowDockBar(ShowEventArgs)")
  virtual void OnShowDockBar(bool show);

  // Description:
  //   Check to see if ShowEventArgs is a show event of some sort
  static bool IsShowing(IDockBarEventWatcher::ShowEventArgs args);

  // Description:
  //   Check to see if ShowEventArgs is a hide event of some sort
  static bool IsHiding(IDockBarEventWatcher::ShowEventArgs args);

  // Description:
  //   Called when a dock bar has been shown or hidden.
  // Parameters:
  //   show [in] If dock bar is to be shown this will be true otherwise
  //             it will be false.
  virtual void OnShowDockBar(IDockBarEventWatcher::ShowEventArgs args);

  // Description:
  //   Called by CRhinoUiDockBar::CalcFixedLayout just prior to committing size.
  //   Override if you want the size to snap at specific increments.
  // Parameters:
  //   size [in] New calculated size.
  //   bStretch [in] See CRhinoUiDockBar::CalcFixedLayout[bStretch]
  //   bHorz [in] See CRhinoUiDockBar::CalcFixedLayout[bHorz]
  // Returns;
  //   Return size to commit.
  virtual CSize OnCalcFixedLayout(CSize size, BOOL32 bStretch, BOOL32 bHorz);

  // Description:
  //   Called by CRhinoUiDockBar::CalcDynamicLayout just prior to committing size.
  //   Override if you want the size to snap at specific increments.
  // Parameters:
  //   size [in] New calculated size.
  //   nLength [in] See CRhinoUiDockBar::CalcDynamicLayout[nLength]
  //   nMode [in] See CRhinoUiDockBar::CalcDynamicLayout[nMode]
  // Returns;
  //   Return size to commit.
  virtual CSize OnCalcDynamicLayout(CSize size, int nLength, DWORD nMode);

  // Description:
  //   Call this method to get position and state information about the dock bar.
  virtual void RhinoGetBarInfo( CControlBarInfo& cbi);

  bool CreateDockBarClientDialog( LPCTSTR lpszTemplateName, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  bool CreateDockBarClientDialog( UINT nIDTemplate, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  void SetDockBarClientWnd( CWnd* pWnd);
  CWnd* DockBarClientWnd() const;

  // Description:
  //   Set initial dock bar size when floating.  Should be called prior to calling
  //   CRhinoUiDockBar::CreateRhinoDockBar().
  // Parameters:
  //   size [in] Initial size.
  void SetInitialSizeFloating( CSize size);
  CSize InitialSizeFloating() const;

  // Description:
  //   Set initial dock bar size when docked on right or left side of Rhino main
  //   frame window.  Should be called prior to calling
  //   CRhinoUiDockBar::CreateRhinoDockBar().
  // Parameters:
  //   size [in] Initial size.
  void SetInitialSizeDockedVert( CSize size);
  CSize InitialSizeDockedVert() const;

  // Description:
  //   Set initial dock bar size when docked on top or bottom of Rhino main
  //   frame window.  Should be called prior to calling
  //   CRhinoUiDockBar::CreateRhinoDockBar().
  // Parameters:
  //   size [in] Initial size.
  void SetInitialSizeDockedHorz( CSize size);
  CSize InitialSizeDockedHorz() const;

  // Description:
  //   Call to assure that the entire dock bar is visible on the monitor which
  //   contains the greatest amount of the control bar.  If the control bar 
  //   is outside of a monitor then it is pushed onto the monitor containing
  //   the main Rhino window.  This is used by CRhinoUiDockBarManager::ShowDockBar()
  //   right after showing a control bar.
  void ForceOnScreen();

  // Description:
  //   Calculate the combined left+right and top+bottom non client borders at
  //   specified location.  This is may be useful when setting minimum, maximum
  //   and initial sizes.
  // Parameters:
  //   dock_location [in] Valid docking locations are:
  //                        AFX_IDW_DOCKBAR_TOP
  //                        AFX_IDW_DOCKBAR_BOTTOM
  //                        AFX_IDW_DOCKBAR_LEFT
  //                        AFX_IDW_DOCKBAR_RIGHT
  //                        AFX_IDW_DOCKBAR_FLOAT
  //                      If dock_location is not one of the above AFX_IDW_DOCKBAR_FLOAT
  //                      is assumed.
  //   bResizable [in] If true then allow for resize edge
  // Returns:
  //   Size.cx represents the sum of the left and right non client edges.
  //   Size.cy represents the sum of the top and bottom non client edges.
  CSize CalculateNCBorderSize( int dock_location, bool bResizable = true) const;

  // Description:
  //   Calculate the left, right, top, and bottom non client borders at
  //   specified location.  This is may be useful when setting minimum, maximum
  //   and initial sizes and is used by CRhinoUiDockBar::OnNcCalcSize()
  // Parameters:
  //   dock_location [in] Valid docking locations are:
  //                        AFX_IDW_DOCKBAR_TOP
  //                        AFX_IDW_DOCKBAR_BOTTOM
  //                        AFX_IDW_DOCKBAR_LEFT
  //                        AFX_IDW_DOCKBAR_RIGHT
  //                        AFX_IDW_DOCKBAR_FLOAT
  //                      If dock_location is not one of the above AFX_IDW_DOCKBAR_FLOAT
  //                      is assumed.
  //   bResizable [in] If true then allow for resize edge
  // Returns:
  //   Returns a CRect which contains the border sizes as follows:
  //     CRect.left   = left non client edge
  //     CRect.right  = right non client edge
  //     CRect.top    = top non client edge
  //     CRect.bottom = bottom non client edge
  CRect CalculateNCBorders( int dock_location, bool bResizable = true) const;

  // Description:m
  //   Get current gripper size value.
  // Returns:
  //   Current gripper size.
  int GripperSize() const;

  // Description:
  //   The dock bar manager is a collection of CRhinoUiDockBars.  The dock bar
  //   manager includes methods for sorting and culling this list for display in Rhino
  //   GUI.  This index is a runtime index used to get this sorted list.
  // Returns:
  //   If less than 0 then this dock bar is not visible in the Rhino GUI ortherwise
  //   it identifies the display index of the dock bar in the GUI.
  int SortIndex() const;

  // Description:
  //   Get the current docked location of this dock bar.
  // Returns:
  //   The current docked location of this dock bar.
  //     AFX_IDW_DOCKBAR_RIGHT
  //     AFX_IDW_DOCKBAR_LEFT
  //     AFX_IDW_DOCKBAR_TOP
  //     AFX_IDW_DOCKBAR_BOTTOM
  //     AFX_IDW_DOCKBAR_FLOAT
  int DockLocation() const;

  // Description:
  //   Check to see if this dock bar is docked.
  // Returns:
  //   true If the dock bar is docked at the top, bottom, left or right.
  bool IsDocked() const;

  // Description:
  //   Check to see if this dock bar is docked at the top of the Rhino main
  //   frame window.
  // Returns:
  //   true If the dock bar is docked at the top.
  bool IsDockedTop() const;

  // Description:
  //   Check to see if this dock bar is docked at the bottom of the Rhino main
  //   frame window.
  // Returns:
  //   true If the dock bar is docked at the bottom.
  bool IsDockedBottom() const;

  // Description:
  //   Check to see if this dock bar is docked at the top or bottom of the Rhino
  //   main frame window.
  // Returns:
  //   true If the dock bar is docked at the top or bottom.
  bool IsDockedHoriz() const;

  // Description:
  //   Check to see if this dock bar is docked on the right sied of the Rhino
  //   main frame window.
  // Returns:
  //   true If the dock bar is docked on the right side.
  bool IsDockedRight() const;

  // Description:
  //   Check to see if this dock bar is docked on the right sied of the Rhino
  //   main frame window.
  // Returns:
  //   true If the dock bar is docked on the left side.
  bool IsDockedLeft() const;

  // Description:
  //   Check to see if this dock bar is docked at the right or left side of the
  //   Rhino main frame window.
  // Returns:
  //   true If the dock bar is docked on the right or left side.
  bool IsDockedVert() const;

  // Description:
  //   Flags that describe how the dock bar will be drawn or act.
  enum control_bar_flags : int
  {
    cbf_show_name_when_docked_vert = 0,
    cbf_draw_gripper_when_docked_horz,
    cbf_draw_gripper_when_docked_vert,
    cbf_forward_mouse_wheel_to_cursor_wnd, // forward WM_MOUSEWHEEL messages to the CWnd under the mouse cursor
    // Reserved for future use
    cbf_unused_1,
    cbf_unused_2,
    cbf_unused_3,
    cbf_unused_4,
    // Invalid should always be last, it is used to size flag byte array
    cbf_invalid
  };

  // Description:
  //   Check to see if style flag is set.
  // Parameters:
  //   flag [in] Flag to check
  // Returns:
  //   true If flag is set otherwise false.
  bool IsDockBarFlagSet( control_bar_flags flag) const;

  // Description:
  //   Set dock bar style flag.
  // Parameters:
  //   flag [in] Flag to set
  //   b [in] New value for flag
  // Returns:
  //   Previous flag value
  bool SetDockBarFlag( control_bar_flags flag, bool b);

  bool ShowCloseButton(bool show);
  bool ShowGearButton(bool show);

  bool IsCloseButtonVisible() const;
  bool IsGearButtonVisible() const;

  // Used by OnNcHitTest to return custom caption button hit test ID's
  enum caption_button_hit_test : int
  {
    cbht_close = HTCLOSE,
    cbht_expand = HTMAXBUTTON,
  };

  // Description:
  //   Toggle dock bars docked state.  If docked then float otherwise if floating then
  //   return to most recent docked position.
  void ToggleDocking();

  // Description:
  //   If this dock bar was created by a plug-in this will return the plug-ins UUID otherwise
  //   it returns the Rhino UUID
  // Returns:
  //   If this dock bar was created by a plug-in this will return the plug-ins UUID otherwise
  //   it returns the Rhino UUID
  ON_UUID PlugInID() const;

private:
  // Description:
  //   Call to create and display dock bar in default location.
  //
  // Parameters:
  //   show_control_bar [in] Display dock bar if created successfully.
  //   default_dock_location [in] Default display location of dock bar.
  //     Valid values are:
  //        AFX_IDW_DOCKBAR_FLOAT - float the dock bar
  //        AFX_IDW_DOCKBAR_TOP - dock the dock bar at the top of the Rhino main frame
  //        AFX_IDW_DOCKBAR_BOTTOM - dock the dock bar at the bottom of the Rhino main frame
  //        AFX_IDW_DOCKBAR_LEFT - dock the dock bar at the left side of the Rhino main frame
  //        AFX_IDW_DOCKBAR_RIGHT - dock the dock bar at the right side of the Rhino main frame
  //   dock_style [in] - Areas this dock bar is allowed to be docked.
  //   float_point [in] - Only used if default_dock_location==AFX_IDW_DOCKBAR_FLOAT.  If floating
  //     dock bar then this is the top left cornner of the dock bar.  If this is NULL then
  //     dock bar is cascaded.
  //   styles [in]
  //     Dock bar styles used when creating this dock bar.
  bool CreateRhinoDockBar( ON_UUID plug_in_id
                           , bool show_control_bar = true
                           , DWORD default_dock_location = AFX_IDW_DOCKBAR_FLOAT
                           , DWORD dock_style = CBRS_ALIGN_ANY
                           , LPPOINT float_point = NULL
                           , DWORD styles = CBRS_TOOLTIPS|CBRS_SIZE_DYNAMIC|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_LEFT
                         );
protected:
  bool IsDelayFloat() const;
  bool DelayFloat();
  void PaintCaptionButtons( HWND hWnd);
  bool GetMinMaxSizes( LPSIZE szMin, LPSIZE szMax, int at_location) const;
  bool IsSizable( int at_location = -1) const;

  // Description:
  //   Get CRhinoUiDockBar objects which share the docking location of this
  //   dock bar.  Called when a dock bar is docked and a list of other
  //   dock bars at the same docking location is needed.
  // Parameters:
  //   bar_list [out] List to populate with dock bars.
  // Returns:
  //   Number of dock bars found including this one.  Zero if none found
  //   or on error.
  int GetDockSiteBars( ON_SimpleArray<CRhinoUiDockBar*>& bar_list) const;

  void GetDockSiteInfo( int& first_bar, int& last_bar, int& this_bar) const;

  // Tracking modes when sizing a docked dock bar.
  enum tracking : int
  {
    not_tracking    = HTERROR,
    tracking_left   = HTLEFT,
    tracking_right  = HTRIGHT,
    tracking_top    = HTTOP,
    tracking_bottom = HTBOTTOM,
  };

  // Description:
  //  Current tracking mode when sizing a docked dock bar.
  tracking TrackingLocation() const;

  // Description:
  //  Check to see if dock bar is being sized by dragging the size control
  //  seporating two dock bars docked at the same location.
  // Returns:
  //  true If dragging the size control seporating two dock bars docked at
  //  the same location.
  bool IsSideTracking() const;

  // Description:
  //  Check to see if a dock bar is docked and being sized.
  // Returns:
  //  true If this dock bar is docked and sized by dragging an edge.
  bool IsTracking() const;

  bool NegociateSpace( int length_available, bool is_horz);
  void AlignDockBars();
  bool IsEdgeVisible ( UINT hit_test) const;
  bool IsAlmostRight() const;
  bool GetEdgeRect( CRect rcWnd, UINT nHitTest, CRect& rcEdge) const;
  enum edge_hit_test_side : int
  {
    edge_ht_left = 0,
    edge_ht_top,
    edge_ht_right,
    edge_ht_bottom,
    edge_ht_invalid,
  };
  static UINT GetEdgeHTCode( edge_hit_test_side nEdge);
  void RedrawButton( const CRhinoUiDockBarCaptionButton& btn);
  void StartTracking( UINT nHitTest);
  void OnTrackInvertTracker();
  void TrackButtons( CPoint ptScreen);
  void StopTracking();
  void OnTrackUpdateSize( CPoint& point);
  void Expand ( bool bToggle);
  bool CanStretch( UINT nHitTest) const;

private:
  void HookFloatingDockBar();

public:
  enum RHINO_CONTROL_BAR_STYLES
  {
    RHCBS_EDGELEFT   = 0x00000001,
    RHCBS_EDGERIGHT  = 0x00000002,
    RHCBS_EDGETOP    = 0x00000004,
    RHCBS_EDGEBOTTOM = 0x00000008,
    RHCBS_EDGEALL    = 0x0000000F,
    RHCBS_SHOWEDGES  = 0x00000010,
    RHCBS_GRIPPER    = 0x00000020,
  };

private:
  friend LRESULT CALLBACK RhDockBarFloatingFrameWndProc( HWND, UINT, WPARAM, LPARAM);
  friend LRESULT CALLBACK RhDockBarFloatingFrameWndProcB( HWND, UINT, WPARAM, LPARAM);
  friend class CRhinoUiDockBarManager;
  friend class CRhUiDockContext;
  friend class CRhControlBarInfo;
  friend class CRhUiDockBarManagerWnd;
  friend class CRhDockState;
  friend class CRhUiDockBar;
  friend class CRhinoUiPageDockBarManager;
  friend class CRhinoUiPageDockBar;
  friend void RhUiDockStatePlugInLoaded( UUID plug_in_id);

  bool PreviousStateLoaded() const;
  void PreviousStateLoaded(bool value);

  bool WasVisibleAtLeastOnce() const;
  void WasVisibleAtLeastOnce(bool value);

protected:
  virtual LRESULT CALLBACK DockBarFloatingFrameWndProc( HWND, UINT, WPARAM, LPARAM);
  LRESULT CALLBACK DockBarFloatingFrameWndProcB( HWND, UINT, WPARAM, LPARAM);
  bool OnNcPaintFloat( HWND hwnd, WPARAM wParam, LPARAM lParam);
  bool SetDockBarMouseCapture();
  void ReleaseDockBarMouseCapture();
  void SetDelayFloatInfo( class CRhControlBarInfo*);
public:
  const class CRhControlBarInfo* DelayFloatInfo() const;


private:
  CWnd* m_pWndAutoDelete = nullptr;
  bool  m_bSubclassFloatFrame = true;
  class CRhControlBarInfo* m_pDelayFloatInfo = nullptr;
  CRhinoUiDockBarCaptionButton m_close_button;
  CRhinoUiDockBarCaptionButton m_gear_button;

protected:
  CSize m_szHorz;
  CSize m_szVert;
  CSize m_szFloat;
  CSize m_szMinFloat;
  CSize m_szMaxFloat;
  CSize m_szMinHorz;
  CSize m_szMaxHorz;
  CSize m_szMinVert;
  CSize m_szMaxVert;
  DWORD m_rhino_cb_style;
  int m_border_size;
  int m_dock_location;
  int m_tracking;
  bool m_is_parent_sizing;
  bool m_is_single_in_row;
  bool m_is_maximized;
  bool m_keep_size;
  int m_cx_edge;
  int m_sort_index;
  int m_pushed_button;
  int m_over_button;
  CRect m_redraw_rect;
  CSize m_save_size;
  CSize m_track_min_size;
  CSize m_track_max_size;
  CSize m_max_tracking_size;
  CSize m_min_tracking_size;
  CPoint m_save_point;
  bool m_bActiveApp;
  bool m_bActiveFloating;
  bool m_bDockBarMouseCaptured;
  bool m_bReplaceDockContext;
  bool m_bAutoDeleteRhinoDockBar;
  BYTE m_rhino_flags[cbf_invalid];
  CWnd* m_client_window;
  WNDPROC m_pOldFloatFrameWndProc;
  UUID m_plug_in_id;
  static UUID m_bogus_rhino_control_bar_uuid;
  static ON_wString m_bogus_rhino_control_bar_name;
public:
  class CRhDockBarExtension* m__control_bar_sdk_extension = nullptr;
public:  
  // CControlBar overrides
  void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL32 bDisableIfNoHndler) override;
  // sizing
  CSize CalcFixedLayout(BOOL32 bStretch, BOOL32 bHorz) override;
  CSize CalcDynamicLayout(int nLength, DWORD nMode) override;
  // painting
  void DrawGripper(CDC* pDC, const CRect& rect) override;
  void DelayShow(BOOL32 bShow) override;

  BOOL32 IsVisible() const override;
  DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout) override;
  BOOL32 IsDockBar() const override;
  BOOL32 DestroyWindow() override;
  void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle) override;
  BOOL32 PreTranslateMessage(MSG* pMsg) override;
  BOOL32 PreCreateWindow(CREATESTRUCT& cs) override;
  void PostNcDestroy() override;
  LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam) override;
  BOOL32 SetStatusText(INT_PTR nHit) override;

  void EnableDocking(DWORD dwDockStyle);

private:
  CRhinoUiDockBar(const CRhinoUiDockBar&) = delete;

protected:
  DECLARE_MESSAGE_MAP()
protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnNcPaint();
  afx_msg void OnNcCalcSize(BOOL32 bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
  afx_msg LRESULT OnNcHitTest(CPoint point);
  afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
  afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
  afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
  afx_msg void OnCaptureChanged(CWnd *pWnd);
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
  afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg BOOL32 OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnActivateApp(BOOL32 bActive, DWORD dwThreadID);
#if !defined (ON_RUNTIME_WIN)
  afx_msg void OnMouseMove(UINT nFlags, CPoint point) override;
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point) override;
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point) override;
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point) override;
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point) override;
  afx_msg void OnTimer( UINT_PTR nIDEvent ) override;
#else
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnTimer( UINT_PTR nIDEvent );
#endif
};



