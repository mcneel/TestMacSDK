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

#include "RhinoSdkDisplayConduit.h"

class RHINO_SDK_CLASS CRhinoView
{
public:
#if defined (RHINO_SDK_MFC)
  CView* SafeMFCView(void);
  const CView* SafeMFCView(void) const;

  ON_DEPRECATED_MSG("Do not call - use CRhViewBase::FromRhinoView")
  class CRhViewBase& MFCViewEx(void);

  ON_DEPRECATED_MSG("Do not call - use CRhViewBase::FromRhinoView")
  const class CRhViewBase& MFCViewEx(void) const;

  struct CRuntimeClass* GetRuntimeClass() const;
  static CRhinoView* FromMFCView(CView*);
#endif
    
#if defined (ON_RUNTIME_APPLE)
  id    OSXView(void) const;
  void  SetOSXView(id osxView);
  bool  SetScreenPoint(const ON_2iPoint& screen_point);
  const ON_2iPoint GetScreenPoint(void) const;
#endif

  //Reimplement commonly used MFC functions
  HWND SetFocus();
  void GetWindowRect(ON_4iRect&) const;
  void GetClientRect(ON_4iRect&) const;
#if defined (ON_RUNTIME_WIN)
  HWND GetParentFrame() const;
#endif
  HWND GetParent() const;
  void ClientToScreen(ON_2iPoint&) const;
  void ClientToScreen(ON_4iRect&) const;
  void ScreenToClient(ON_2iPoint&) const;
  void ScreenToClient(ON_4iRect&) const;
  HWND SetCapture();
  HDC GetDC();
  int ReleaseDC(HDC);
  void UpdateWindow();
  bool RedrawWindow(const ON_4iRect* lpRectUpdate = nullptr, HRGN prgnUpdate = 0, UINT flags = 0x105);
  void InvalidateRect(const ON_4iRect& lpRect, bool bErase = true);
  void InvalidateRect(bool bErase = true);	//Same as passing a null pointer to MFC InvalidateRect
  void Invalidate(bool bErase = true);
  HWND SetActiveWindow();
  bool IsWindowVisible() const;
  bool IsWindowEnabled() const;
  bool ShowWindow(int nCmdShow);
  bool EnableWindow(bool bEnable = true);
  void MoveWindow(const ON_4iRect& rect, bool bRepaint = true);
  HWND GetSafeHwnd() const;

public:
	//These functions pass through to the handlers in the implementation view.
	//Expert use only.
  void OnDraw(HDC hDC);
  void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  void OnLButtonDown(UINT nFlags, const ON_2iPoint& point);
  void OnLButtonUp(UINT nFlags, const ON_2iPoint& point);
  void OnLButtonDblClk(UINT nFlags, const ON_2iPoint& point);
  void OnMButtonDown(UINT nFlags, const ON_2iPoint& point);
  void OnMButtonUp(UINT nFlags, const ON_2iPoint& point);
  void OnMButtonDblClk(UINT nFlags, const ON_2iPoint& point);
  void OnRButtonDown(UINT nFlags, const ON_2iPoint& point);
  void OnRButtonUp(UINT nFlags, const ON_2iPoint& point);
  void OnRButtonDblClk(UINT nFlags, const ON_2iPoint& point);
  void OnMouseMove(UINT nFlags, const ON_2iPoint& point);
  bool OnMouseWheel(UINT nFlags, short zDelta, const ON_2iPoint& pt);
#if defined (ON_RUNTIME_WIN)
  int  OnMouseActivate(HWND pWnd, UINT nHitTest, UINT message);
#endif
  bool OnSetCursor(HWND pWnd, UINT nHitTest, UINT message);

private:
  // Use CRhinoView.RuntimeSerialNumber() to get this value.
  // Use CRhinoView::FromRuntimeSerialNumber() so search for
  // a view with a specific serial number.
  //
  // The m_rhino_view_sn field is private to make it impossible
  // for the value to be accidentally passed as a pointer or 
  // reference, which would defeat the purpose of using 
  // runtime serial numbers to prevent dereferencing deleted objects.
  //
  // m_rhino_view_sn must be the first field in CRhinoView
  // so it is initialized first in constructors.
  const unsigned int m_rhino_view_sn;

protected:
  CRhinoView(class CRhViewBase& view);
  CRhinoView(class CRhViewBase* view);
  virtual ~CRhinoView();
private:
  void ConstructHelper(class CRhViewBase* view);
public:
  // Return true if this view does not have an attached window
  bool IsHeadless() const;

  virtual bool IsPageView(void) const;

  //Calls delete on the underlying CRhView (derived from CView)
  //results in this object also being destroyed.  Expert usage only.
  void Delete(void);

  /*
  Description:
    Create a new CRhinoView.
  Parameters:
    rhino_doc_sn - [in]
      Runtime serial number of the document being viewed.
    view - [in]
      null or information about the view type and projection.
    bShowView - [in]
      If false, the view will not be shown.  This is what you
      should do if you need to perform additional initialization
      on the view before it appears.
      If true, the view will be shown and updated.
  Returns:
    Runtime serial number of the new view.
    CRhinoView::FromRuntimeSerialNumber(...) can be used to get a pointer to this 
    class.
  */
  static unsigned int CreateView(
    unsigned int rhino_doc_sn,
    const ON_3dmView* view,
    bool bShowView
    );

  /*
  Description:
    Get a view from a runtime serial number
  Parameters:
    rhino_view_sn - [in]
      View runtime serial number.
  Return:
    A pointer to the view or null if there is not
    a view with the specified serial number.
  */
  static CRhinoView* FromRuntimeSerialNumber(
    unsigned int rhino_view_sn
    );

  /*
  Description:
    Get a view from a main viewport id.
  Parameters:
    main_viewport_id - [in]
      Model views have a single viewport which
      is both the main viewport and
      Page views, this is the id of the large
      view that may contain multiple details.

      main_viewport id      
  Return:
    A pointer to the view or null if there is not
    a view with the specified serial number.
  */
  static CRhinoView* FromMainViewportId(
    ON_UUID main_viewport_id
    );

  /*
  Parameters:
    view - [in]
      pointer value
  Returns:
    A pointer to a CRhinoView if view points to a CRhinoView class.
  */
  static CRhinoView* FromPointer(
    void* view
    );


  /*
  Description:
    Get a view from a frame window.
  Parameters:
    pFrameWnd - [in]
      A pointer to a window that contains the CRhinoView.
      In the common case of a Rhino V6 view in the client
      area of the main frame, this will be a CMDIChildWnd.
    bSetChildWndActiveView - [in]
      If 
        bSetChildWndActiveView is true
      and 
        pFrameWind points to a CMDIChildWnd, 
      and 
        ((CMDIChildWnd()pFrameWnd)->GetActiveView() returns null
      and 
        pFrameWnd->GetDescendantWindow( AFX_IDW_PANE_FIRST, true)
        returns a pointer to a CRhinoView
      then 
        ((CMDIChildWnd()pFrameWnd)->SetActiveView( CRhinoView pointer, false )
        is called toset the CRhinoView as the active view so that 
        future MFC calls to GetActiveView() will work.
  Return:
    A pointer to the view or null if the window is not a frame for
    a CRhinoView.
  */
  static CRhinoView* FromViewFrameWnd(
    class CWnd* pFrameWnd,
    bool bSetChildWndActiveView
    );

  /*
  Returns:
    >0: runtime serial number of view that exists.
     0: view is null
  */
  static unsigned int RuntimeSerialNumber(
    const CRhinoView* rhino_view
    );

  /*
  Returns:
    True if rhino_view is null or a valid view.
    False if the pointer is not valid.
  */
  static bool IsValidViewPointer(
    const void* rhino_view
    );

  static bool IsValidViewReference(
    const CRhinoView& rhino_view
    );

  unsigned int RuntimeSerialNumber() const;

  /*
  Returns:
    The id of the main viewport. Model views have single
    viewport.  For page views, the "main" viewport is the
    large one that represents the page.  A page may
    contain zero or more details, each of which has
    a viewport id as well.
  */
  ON_UUID MainViewportId() const;

  /*
  Returns:
    The serial number of the next view to be constructed.
  */
  static unsigned int NextRuntimeSerialNumber();

  /*
  Description:
    In most cases, it is easier, faster and more robust to
    use a CRhinoViewIterator to go through the list of views.
    In the rare case when you really do want a list of 
    all existing view serial numbers, use this function.
  Parameters:
    rhino_doc_sn - [in]
       0: get all view serial numbers.
      >0: get all view serial numbers for the specified document.
    rhino_view_sn_list - [out]
      list of rhino view serial numbers in increasing order.
  Returns:
    Number of rhino view serial numbers in rhino_view_sn_list[].
  See Also:
    CRhinoViewIterator
  */
  static unsigned int GetViewList(
    unsigned int rhino_doc_sn,
    ON_SimpleArray<unsigned int>& rhino_view_sn_list
    );

#pragma region RH_C_SHARED_ENUM [CRhinoView::ViewTypeFilter] [Rhino.Display.ViewTypeFilter] [int]
  /// <summary>
  /// Filter for view types to list
  /// </summary>
  enum class ViewTypeFilter : int
  {
    /// <summary>
    /// No views
    /// </summary>
    None = 0,
    /// <summary>
    /// Standard Rhino model views
    /// </summary>
    Model = 1,
    /// <summary>
    /// Page views
    /// </summary>
    Page = 2,
    /// <summary>
    /// All view styles except Page.
    /// </summary>
    ModelStyleViews = 0xFFFFFFF ^ 2,
    /// <summary>
    /// UV Editor views
    /// </summary>
    UVEditor = 4,
    /// <summary>
    /// Block Editor views
    /// </summary>
    BlockEditor = 8,
    /// <summary>
    /// All views
    /// </summary>
    All = 0xFFFFFFF
  };
#pragma endregion

  /*
  Description:
    In most cases, it is easier, faster and more robust to
    use a CRhinoViewIterator to go through the list of views.
    In the rare case when you really do want a list of 
    all existing view serial numbers, use this function.
  Parameters:
    rhino_doc_sn - [in]
       0: get all views with serial numbers > rhino_view_sn0.
      >0: get all views of a specified document with view serial
          numbers > rhino_view_sn0.
    rhino_view_sn0 - [in]  
      Serial numbers <= rhino_view_sn0 will not be added
      to rhino_view_sn_list[].
    rhino_view_sn_list_capacity - [in]
      maximum number of elements in rhino_view_sn_list[] array.
    rhino_view_sn_list - [out]
      list of rhino view serial numbers in increasing order.
  Returns:
    Number of rhino view serial numbers set in rhino_view_sn_list[].
  See Also:
    CRhinoViewIterator
  */
  static unsigned int GetViewList(
    unsigned int rhino_doc_sn,
    unsigned int rhino_view_sn0,
    size_t rhino_view_sn_list_capacity,
    unsigned int* rhino_view_sn_list
    );

  /*
  Parameters:
    rhino_doc_sn - [in]
       0: all views.
      >0: restricte search to views of the specified document
  Returns:
    Number of active views.
  */
  static unsigned int ViewCount(
    unsigned int rhino_doc_sn
    );


public:
  /*
  Description:
    Redraw this view.
  Parameters:
    view_redraw_display_hint - [in] a CRhinoView::DISPLAY_HINT value
  Remarks:
    If you change something in "this" view like the projection,
    construction plane, background bitmap, etc., then you need 
    to call CRhinoView::Redraw to redraw "this" view.  The other
    views will not be changed.  If you change something in the
    document (like adding new geometry to the model), then you
    need to call CRhinoDoc::Redraw to redraw all the views.
    
    CRhinoView::Redraw updates some internal flags and then
    calls CView::InvalidateRect(NULL,false) to get a WM_PAINT
    message posted.

    NEVER CALL CView::InvalidateRect.
  See Also
    CRhinoDoc::Redraw
    */
  void Redraw( unsigned int view_redraw_display_hint = CRhinoView::regenerate_display_hint );

  /*
  Description:
    Expert user tool to set flags used by CRhinoView::OnDraw()
  Parameters:
    view_redraw_display_hint - [in] a CRhinoView::DISPLAY_HINT value
  Remarks:
    You probably want to use CRhinoDoc::SetRedrawDisplayHint()
    instead of  CRhinoView::SetRedrawDisplayHint()
  See Also:
    CRhinoDoc::SetRedrawDisplayHint
  */
  void SetRedrawDisplayHint( unsigned int display_hint ) const;

  /*
  Description:
    Enables drawing.  By default, drawing is enabled.  There are
    some rare situations where scipts want to disable drawing
    for a while.
  Parameters:
    bEnableDrawing - [in] true to enable drawing.
  Returns:
    Previous drawing state.
  See Also:
    CRhinoView::IsDrawingEnabled
  */
  static bool EnableDrawing( bool bEnableDrawing = true );

  /*
  Description:
    Check to see if drawing is enabled.  By default, drawing is enabled.
    There are some rare situations where scipts want to disable drawing
    for a while.
  Returns:
    Returns true if drawing is enabled; otherwise false.
  See Also:
    CRhinoView::EnableDrawing
  */
  static bool IsDrawingEnabled();

  // Description:
  //   Copy current screen view to a bitmap.
  // Parameters:
  //   dib - [out] 24 Bit color DIB containing screen capture of this view
  //   bIncludeCursor  - [in] If (bIncludeCursor == true) and cursor is over
  //                          this view then "dib" will include cursor.
  //   bClientAreaOnly - [in] do not capture the parent frame
  // Returns:
  //   true if successful.
  bool ScreenCaptureToBitmap( CRhinoDib& dib, BOOL32 bIncludeCursor = true, BOOL32 bClientAreaOnly = false);

  CRhinoDoc* GetDocument() const;

  unsigned int DocumentRuntimeSerialNumber() const;
  
  // m_pSelection holds the selection to the current CJunkCntrItem.
  // For many applications, such a member variable isn't adequate to
  //  represent a selection, such as a multiple selection or a selection
  //  of objects that are not CJunkCntrItem objects.  This selection
  //  mechanism is provided just to help you get started.

  //DALELEARV6// no OLE container support in V6 // // TODO: replace this selection mechanism with one appropriate to your app.
  //DALELEARV6// no OLE container support in V6 // CRhCntrItem* m_pSelection;

  // Return true if this view is maximized.
  bool IsMaximized() const;

  // Return true if this view is hidden.
  bool IsHidden() const;

  enum DISPLAY_HINT
  {
    // completely regenerate back buffer image
    regenerate_display_hint=0, 

    // For depth buffered viewports, completely regenerate back
    // buffer image. For non-depth buffered viewports,
    // objects with CRhinoObject::m_display_hint=true are drawn
    // in the current back buffer image.
    regenerate_depthbuffered_display=1,
    
    // For shaded viewports, completely regenerate back
    // buffer image. For wireframe viewports, objects 
    // with CRhinoObject::m_display_hint=true are drawn
    // in the current back buffer image.
    regenerate_shaded_display=2,
    
    // Objects with CRhinoObject::m_display_hint=true
    // are drawn in the current back buffer image.
    mark_display_hint=3,         

    // Quickly refresh display by showing back buffer.
    // (gets rid of custom GDI drawing, repairs views
    // covered by portions of other windows, etc.)
    refresh_display_hint=4
  };

  // Description:
  //   all 3d projection information and drawing tools are provided by m_vp
  // Parameters:
  //   view- [in] the view information to set
  //   bMainViewport= [in] if true, view is set on the MainViewport()
  //                       if false, view is set on the ActiveViewport()
  void SetProjection( const ON_3dmView& view, bool bMainViewport);


  //Description:
  // A CRhinoView contains a "main viewport" that fills the entire view client window.
  // CRhinoPageViews may also contain nested child CRhinoViewports for implementing
  // detail viewports.
  // The MainViewport will always return this CRhinoView's m_vp
  CRhinoViewport& MainViewport() const;

  //Description:
  // The ActiveViewport is the same as the MainViewport for standard CRhinoViews. In
  // a CRhinoPageView, the active viewport may be the CRhinoViewport of a child detail object.
  // Most of the time, you will use ActiveViewport unless you explicitly need to work with
  // the main viewport.
  virtual CRhinoViewport& ActiveViewport() const;

  //Description:
  // An alias for ActiveViewport() so that pre-existing code continues to compile
  // and work. It is recommended that you use ActiveViewport() in any new and future code.
  CRhinoViewport& Viewport() const;

  //Description:
  // Returns viewport ID for the active viewport. Faster than ActiveViewport function when
  // working with page views.
  virtual ON_UUID ActiveViewportID() const;

  //Description:
  // returns relative position in main frame client rect
  const ON_3dmViewPosition& Position() const; 

  enum drag_plane
  {
    world_plane,
    construction_plane,
    view_plane,
    uvn_plane,
    control_polygon
  };

  /*
  Returns:
    Plane used for dynamic mouse dragging.    
  See Also:
    CRhinoView::SetDragPlane
  */
  static drag_plane DragPlane();

  /*
  Description:
    Set the plane used for dynamic mouse dragging.    
  See Also:
    CRhinoView::DragPlane
  */
  static void SetDragPlane( drag_plane );

  // call when view gets/loses focus
  void UpdateTitle(bool isActiveView);

  void ShowTitle(bool visible);

  bool IsTitleVisible() const;

  // call to toggle views maximized/restore state.
  void MaximizeRestoreView();

  /*
  Description:
    Get list of objects that are visible in the view.
  Parameters:
    pick_rect - [in] if non-empty, then this defines a view
                window coordinate rectangle to use.  If
                empty, then the entire view is searched.
    visible_objects - [out] visible objects are appended to this list.
  Returns:
    Number of objects appended to visible_object] array.
  */
  int GetVisibleObjects( const ON_4iRect& pick_rect, ON_SimpleArray<const CRhinoObject*>& visible_objects );

  enum view_type
  {
    rhino_view_type = 0,  // Standard CRhinoView
    page_view_type,       // CRhinoPageView = Plot layout view
    custom_view_type,     // Some unknown derived class
    invalid_view_type,
    uveditor_view_type,   // CRhinoUVEditorView
    blockeditor_view_type // Block Editor view
  };
  /*
  Description:
    Identifies what type of CRhinoView or CRhinoView derived class this
    is.
  Returns:
    Returns a CRhinoView::view_type indicating what kind of view
    this is.
  See Also:
    class CRhinoPageView
  */
  virtual int RhinoViewType() const;

  // Description:
  //   Get the runtime class for the CRhinoDisplayPipeline that this view should use.
  //   Subclass views may use custom display pipelines and should override this function
  //   to retrieve the runtime class for the custom display pipeline.
  //   The base class version of this function makes decisions based on view mode and
  //   application settings to return either a GDI or OpenGL based display pipeline
  //   (CRhinoDisplayPipeline_GDI or CRhinoDisplayPipeline_OGL)
  // Return:
  //   Runtime class pointer for the CRhinoDisplayPipeline derived class to use for
  //   this view.
  virtual const ON_ClassId* GetDefaultDisplayPipelineClass() const;

  // Description:
  //  Sets up the default display pipeline based on current
  //  display settings (eg. GDI vs. OpenGL), and attaches it
  //  to "this" view's viewport object.
  bool SetupDisplayPipeline(void);
  
  ///////////////////////////////
  // Returns the active display pipeline attached
  // to "this" view's viewport.
  CRhinoDisplayPipeline*  DisplayPipeline(void) const;
  
  ///////////////////////////////
  // Returns the display mode attributes assigned to this view.
  CDisplayPipelineAttributes* DisplayAttributes(void) const;

  void ShowToast(const wchar_t* message);
public:
  // OnMouseDown helpers
  void OnMouseDownSelectObject( 
          int button,
          BOOL32 bCtrl,  // true means Ctrl key is down
          BOOL32 bShift, // true means Shift key is down
          BOOL32 bAlt, // true means Alt key is down
          const ON_2iPoint& point, // mouse point
          CRhinoViewport& active_vp
          );

  // helpers for handling key events important to dragging UI
  void OnAltKey( bool bDown );
  void OnShiftKey( bool bDown );
  void OnControlKey( bool bDown );
  void OnTabKey( bool bDown );
  void FakeMouseMove(UINT nFlags, const ON_2iPoint& point);
  bool OnSysKey( bool bDown, UINT nChar, UINT nRepCnt, UINT nFlags);

  void OnContextMenu(const ON_2iPoint& point, ON_SimpleArray<CRhinoObject*>& objects);

  // mouse status
  enum
  {
    no_mouse_action     = 0,
    mouse_rotate        = 1, // rotate view about target point
    mouse_dolly         = 2, // dolly target plane sideways
    mouse_magnify       = 3, // increase/decrease size
    mouse_zoom_window   = 4,
    mouse_pick_window   = 5,
    mouse_getpoint_onup = 6, // CRhinoGetPoint::Get.. getting the point on mouse up
    mouse_tilt          = 7, // tilt camera
    mouse_rotate_camera = 8, // rotate camera in place
    mouse_adjust_lenslength = 9, // adjust perspective camera lens length
    mouse_inout_dolly   = 10  // dolly camera and target towards/away from scene
  };

  // 6-20-03 lw added to bail out of window or crossing drag on escape key
  void CancelMousePickWindow();

public:
  //Description:
  //  Creates a new window handle for the CRhinoView. This is needed for
  //  setting up window pixel formats (which can only be performed once
  //  on a HWND). All child windows are reparented to the new HWND.
  //  Subclasses that wish to override this function should call this base
  //  class version of the function first.
  //Returns:
  //  true on success
  virtual bool RecreateHWND();
  //Description:
  //  Used by Rhino main frame and doc/view manager to determine if this view is
  //  in a floating frame or a child of the MDIClient window associated with the
  //  Rhino main frame window.
  //Returns:
  //  true If this view is embedded in a free floating frame window.
  bool IsFloatingRhinoView() const;
  //Description:
  //  This is a test method.  It may not be available in the final SDK release.
  //  Call this method to set the window placement and show mode when turning a 
  //  standard Rhino view into a floating Rhino view.
  //Parameters:
  //  wp [in] Floating view frame position, size and state.
  void SetFloatingRhinoViewPlacement( const WINDOWPLACEMENT& wp);
  //Description:
  //  This is a test method.  It may not be available in the final SDK release.
  //  Call this method to get the window placement information from the last time
  //  this view was floated.
  //Returns:
  //  Returns a WINDOWPLACEMENT pointer if this view has been floated before or NULL
  //  if it has not.
  WINDOWPLACEMENT* FloatingRhinoViewPlacement() const;
  //Description:
  //  Call this method to change parent container for this view.
  //
  //Parameters:
  //  bFloat [in] If true then the view will be embedded in a floating frame window otherwise
  //              the view will be embedded in a CMDIChildWnd.
  //
  //Returns:
  //  Returns a handle to the views parent container on success otherwise NULL.
  HWND FloatRhinoView(bool bFloat);

  // Description:
  //   Returns whether or not the mouse is captured in this view.
  // Parameters:
  //    bIncludeMovement - [in] If captured, test if the mouse has moved between mouse down and mouse up.
  // Returns:
  //  true or false
  bool IsMouseCaptured(bool bIncludeMovement) const;

  // Description:
  //   Post digitizer clicked point event.
  //
  // Parameters:
  //   point - [in] 3d world coordinates of point
  //   nFlags - [in] set bits using Windows MK_LBUTTON, MK_RBUTTON, MK_MBUTTON,
  //   MK_SHIFT, and MK_CONTROL defines.
  //
  // Remarks:
  //   Use the nFlags parameter to mimic mouse actions from the digitizer.
  //   For example, if a just a point is being sent, then nFlags = 0.
  //   If the user has clicked a button on the digitizer that is equivalent
  //   to clicking the left mouse button, then nFlags = MK_LBUTTON.
  //   the digitizer.
  void PostDigitizerPointEvent(const ON_3dRay& dig_ray, LPARAM nFlags) const;

public:
  static view_type ViewTypeFromActiveSpace(ON::active_space active_space);

  ON::active_space ActiveSpace(void) const;

protected:
  void  DetermineRegenState(void);

  friend class CRhinoApp; // for access to m_bPendingPartialDrawDocument
  friend class CRhinoDoc; // for access to m_bPendingPartialDrawDocument
  friend class CRhDynamicViewGetPoint; // for access to m_bInDynamicViewChange
  friend class CRhDynamicViewCommand;  // for access to m_bInDynamicViewChange
  friend class CRhinoDisplayPipeline;
  friend class CMainFrame;
  friend class CRhViewBase;
  friend class CRhNamedViewLinkedView;
  friend class CRhView;
  friend class CRhPageView;

protected:
  /// <summary>
  /// Called by OnGestureZoom() and OnMouseWheel() methods to do zooming.</summary>
  bool MouseOrGestureViewZoom(
    CRhinoDoc& doc,
    CRhinoViewport& viewport,
    double magnification_factor,
    const ON_2iPoint& zoom_center_point,
    bool& bInDynamicViewChange,
    bool endZoom);

protected:
  CRhinoViewport* m_vp = nullptr;
  unsigned long m_view_iteration; // if m_view_iteration != m_vp->m_vp_iteration, then regen is needed
  bool m_bNeedRegen = true;
  int m_mouse_captured; // one of above enum values

  // m_bInDynamicViewChange = true if the view is being dynamically
  // changed by mouse moves, arrow keys, trackballs, etc.
  //
  // m_bInterruptDynamicViewChange = true if drawing of the current
  // view should cease ASAP.
public:
  bool m_bInDynamicViewChange = false;
protected:
  bool m_bInterruptDynamicViewChange = false;

private:
  bool m__bInOnDraw = false;

  static drag_plane m_drag_plane;


  // Every Rhino View has its own set of display attributes based on which
  // display "mode" has been assigned to its viewport. These attributes are
  // always setup and assigned the values Rhino currently uses for that mode
  // prior to drawing, so any modifications before, during or after a frame 
  // buffer update will have ZERO effect on the view's display attributes.
  CDisplayPipelineAttributes*   m_pDisplayAttrs = nullptr;

  int m_mouse_action; // one of above enum values

  HCURSOR m_mouse_down_cursor = nullptr;
  ON_Viewport m_mouse_down_vp;      //active viewport ON_Viewport
  ON_3dPoint  m_mouse_down_target;  //active viewport target
  ON_Viewport m_mouse_down_main_vp; //main viewport ON_Viewport
  ON_3dPoint  m_mouse_down_main_target; //main viewport target
  ON_2iPoint m_mouse_down_point;
  ON_2iPoint m_mouse_move_point;
  bool   m_mouse_moved = false; // true if mouse moves between mouse down and mouse up

  // Display hint used in CRhinoView::OnDraw()
  // see also CRhinoDoc::m_redraw_display_hint
  // value is one of DISPLAY_HINT enum values
  int m_view_redraw_display_hint = regenerate_display_hint;

  class CRhViewportTitleWnd* m_viewport_title_wnd = nullptr;

  unsigned char m__redraw_flags = 0;  // (m__redraw_flags & 1) = 1 if Rhino event triggered redraw
  //                         0 if Windows event triggered redraw 
  //                         Reset to zero after OnDraw.
  // (other 7 bits are reserved for future use.)

  unsigned char m_tracking_mouse = 0; // Used with the CRhMouseTracker to
                                      // tell windows that it should send WM_MOUSEHOVER and
                                      // WM_MOUSELEAVE events to this CRhinoView
  WINDOWPLACEMENT* m_pFloatingPlacement = nullptr;

  class CRhViewSdkExtension* m__view_sdk_extension = nullptr;
  ON__UINT_PTR m_sdk_reserved = 0;

private:
  class CRhViewBase* m_platform_window = nullptr;
  friend class CRhViewBase;
};


// Description:
//  For expert use only. 
//  CRhinoView::InterruptDrawing() hook for spacemouse-type devices.
RHINO_SDK_FUNCTION
void RhinoDynamicViewChange(CRhinoView* view);


class RHINO_SDK_CLASS CRhinoViewIterator
{
  /*
  Example:
    Iteratate all views:
    unsigned int rhino_doc_sn = 0 or doc->RuntimeSerialNumber();
    CRhinoDocIterator doc_it(rhino_doc_sn);
    for ( CRhinoView* rhino_view = view_it.First();
          0 != rhino_view; 
          rhino_view = view_it.Next() 
        )
    {
      ...
    }
  */
public:
  unsigned int DocRuntimeSerialNumber() const;


  /*
  Parameters:
    rhino_doc_sn - [in]
      0:  iterate all views of all documents.
      >0: iteration all views of the specified doc.
  */
  CRhinoViewIterator(
    unsigned int rhino_doc_sn
    );

  /*
  Returns:
    The first view that exists.
  Remarks:
    You may call First() multiple times on the same iterator.
  Example:
        for ( CRhinoView* view = view_it.First(); 0 != view; view = view_it.Next() )
        {
          ...
        }
  */
  class CRhinoView* First();

  /*
  Returns:
    The next view that exists.
  Remarks:
    - Once Next() returns null, it will continue to return null
      until First() is called.
    - If Initialize() is called and First() is not called, then
      the first call to Next() returns the first view.
    - If First() or Initialize() have not been called,
      the first call to Next() returns the first view.
  Example:
    See CRhinoViewIterator::First() and CRhinoViewIterator::Initialize().
  */
  class CRhinoView* Next();

  /*
  Returns:
    The current value of the iterator.
  Remarks:
    - If First() or Next() has not been called, then Current()
      will return null.
    - If the view returned by First() or Next() has been deleted
      since the call to First() or Next(), then CurrentView() will
      return a null pointer.
  */
  class CRhinoView* CurrentView() const;
  unsigned int CurrentViewRuntimeSerialNumber() const;

  /*
  Description:
    Sets the iterator so the next call to Next()
    will return the first view that exists.
    This is useful when it is not practical to call
    First() to begin the iteration through the views.
  Remarks:
    You may call Initialize() multiple times on the same iterator.
  Example:
        view_it.Initialize();
        while ( CRhinoView* view = view_it.Next() )
        {
          ...
        }
  */
  void Initialize();

private:
  const unsigned int m_rhino_doc_sn;
  unsigned int m_current_view_sn;
  // NOTE:
  //  The iterator can handle an arbitrary number of serial numbers.
  //  The buffer is refilled as needed.
  unsigned int m_sn_index;  
  unsigned int m_sn_count;
  unsigned int m_sn_buffer[28];
};

class RHINO_SDK_CLASS CRhinoPageViewIterator
{
  /*
  Example:
    Iteratate all page views:
    unsigned int rhino_doc_sn = 0 or doc->RuntimeSerialNumber();
    CRhinoDocIterator doc_it(rhino_doc_sn);
    for ( CRhinoPageView* rhino_page_view = view_it.First();
          0 != rhino_page_view; 
          rhino_page_view = view_it.Next() 
        )
    {
      ...
    }
  */
public:

  unsigned int DocRuntimeSerialNumber() const;

  /*
  Parameters:
    rhino_doc_sn - [in]
      0:  iterate all views of all documents.
      >0: iteration all views of the specified doc.
  */
  CRhinoPageViewIterator(
    unsigned int rhino_doc_sn
    );

  /*
  Returns:
    The first page view that exists.
  Remarks:
    You may call First() multiple times on the same iterator.
  */
  class CRhinoPageView* First();

  /*
  Returns:
    The next page view that exists.
  Remarks:
    - Once Next() returns null, it will continue to return null
      until First() is called.
    - If First() has not be called, the first call to Next()
      returns the first view in Rhino.  This is done
      so that people can write while loops like
      CRhinoViewIterator it(rhino_doc_sn);
      while ( CRhinoView* rhino_view = it.Next() )
      {
         ...
      }
  */
  class CRhinoPageView* Next();

  /*
  Returns:
    The current value of the iterator.
  Remarks:
    - If First() or Next() has not been called, then Current()
      will return null.
    - If the view returned by First() or Next() has been deleted
      since the call to First() or Next(), then CurrentView() will
      return a null pointer.
  */
  class CRhinoPageView* CurrentView() const;
  unsigned int CurrentViewRuntimeSerialNumber() const;

private:
  CRhinoViewIterator m_view_it;
};


class RHINO_SDK_CLASS CRhinoModelViewIterator
{
  /*
  Example:
    Iteratate all model views:
    unsigned int rhino_doc_sn = 0 or doc->RuntimeSerialNumber();
    CRhinoDocIterator doc_it(rhino_doc_sn);
    for ( CRhinoView* rhino_model_view = view_it.First();
          0 != rhino_model_view; 
          rhino_model_view = view_it.Next() 
        )
    {
      ...
    }
  */
public:

  unsigned int DocRuntimeSerialNumber() const;

  /*
  Parameters:
    rhino_doc_sn - [in]
      0:  iterate all model views of all documents.
      >0: iteration all model views of the specified doc.
  */
  CRhinoModelViewIterator(
    unsigned int rhino_doc_sn
    );

  /*
  Returns:
    The first model view that exists.
  Remarks:
    You may call First() multiple times on the same iterator.
  */
  class CRhinoView* First();

  /*
  Returns:
    The next model view that exists.
  Remarks:
    - Once Next() returns null, it will continue to return null
      until First() is called.
    - If First() has not be called, the first call to Next()
      returns the first view in Rhino.  This is done
      so that people can write while loops like
      CRhinoViewIterator it(rhino_doc_sn);
      while ( CRhinoView* rhino_view = it.Next() )
      {
         ...
      }
  */
  class CRhinoView* Next();

  /*
  Returns:
    The current value of the iterator.
  Remarks:
    - If First() or Next() has not been called, then Current()
      will return null.
    - If the view returned by First() or Next() has been deleted
      since the call to First() or Next(), then CurrentView() will
      return a null pointer.
  */
  class CRhinoView* CurrentView() const;
  unsigned int CurrentViewRuntimeSerialNumber() const;

private:
  CRhinoViewIterator m_view_it;
};

/*
Description:
  If you need to add custom drawing code to Rhino, derive a class from
  CRhinoDrawCallback and override the virtual function where you want
  to draw something.

  To enable, call:
    CRhinoDrawCallback::Enable();
  When you are done, call:
    CRhinoDrawCallback::Disable();

Remarks:
  This is a refactored version of the legacy CRhinoDrawCallback class found
  in previous versions of Rhino. As this temporary helper class will go away
  in future releases, you should replace your usage of this with your own
  class that inherits from CRhinoDisplayConduit.
*/
class RHINO_SDK_CLASS CRhinoDrawCallback : public CRhinoDisplayConduit
{
public:
  CRhinoDrawCallback();
  virtual ~CRhinoDrawCallback(void);

  /*
  Description:
    Called when a view is regenerated and can be used to specify a box you want
    to make sure is included in the view frustum. This is a good way to make
    sure temporary geometry drawn by DrawBackground, DrawMiddleground, and
    DrawForeground is inside of the near and far clipping planes.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  Returns:
    A bounding box
  */
  ON_DEPRECATED_MSG("use a custom CRhinoDisplayConduit")
  virtual ON_BoundingBox BoundingBox(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called when a view is regenerated and should be used for things like
    drawing background images into the "back" buffer.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  */
  ON_DEPRECATED_MSG("use a custom CRhinoDisplayConduit")
  virtual void DrawBackground(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called when a view is regenerated and should be used for things like drawing
    temporary geometry into the "back" buffer. Objects drawn in DrawMiddleground
    are depth clipped and depth buffered. If your objects are getting clipped
    and you don't want them to be clipped, the draw them in DrawForeground.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  Remarks:
    DrawMiddleground is a good place to draw shaded objects. If you want to draw
    wireframe objects, then you should do it in a DrawForeground override.
  */
  ON_DEPRECATED_MSG("use a custom CRhinoDisplayConduit")
  virtual void DrawMiddleground(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called when a view is regenerated and should be used for drawing "decorations"
    like the world axis icon into the "back" buffer. Wireframe objects drawn in
    DrawForeground are not depth clipped or depth buffered.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  Remarks:
    DrawMiddleground is a good place to draw shaded objects. If you want to draw
    wireframe objects, then you should do it in a DrawForeground override.
  */
  ON_DEPRECATED_MSG("use a custom CRhinoDisplayConduit")
  virtual void DrawForeground(CRhinoViewport& vp, CRhinoDoc& doc);

  /*
  Description:
    Called right after the "back" buffer is blitted to the screen and should be
    used for drawing things like tracking lines or rubber bands.
  Parameters:
    vp - [in] The active viewport
    doc - [in] - The active document
  */
#if defined (RHINO_SDK_MFC)
  ON_DEPRECATED_MSG("use a custom CRhinoDisplayConduit")
  virtual void DrawDecorations(CRhinoView* pVP, CDC* pDC, CRhinoViewport& vp, CRhinoDoc& doc);
#endif

  // Obsolete
#if defined (RHINO_SDK_MFC)
  ON_DEPRECATED_MSG("use a custom CRhinoDisplayConduit")
  virtual bool DrawEverything(CRhinoView& view, ON_BoundingBox bbox, CDC* pDC, CRhinoViewport& vp, CRhinoDoc& doc);
#endif

private:
  // CRhinoDisplayConduit::ExecConduit override
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate) override;

};
