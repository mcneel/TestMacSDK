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


// Transformation modes for gumballs
enum GUMBALL_MODE
{
  gb_mode_nothing,

  // gumball menu button was picked
  gb_mode_menu,

  // unconstrained translation
  gb_mode_translatefree,

  // translation along a single axis
  gb_mode_translatex,
  gb_mode_translatey,
  gb_mode_translatez,

  // translation in a plane
  gb_mode_translatexy,
  gb_mode_translateyz,
  gb_mode_translatezx,

  // NOTE: CRhinoGumballFrame m_scale_mode
  //       is used to enable multi-axis
  //       scaling.  This enum indicates
  //       which control is being dragged.
  gb_mode_scalex,
  gb_mode_scaley,
  gb_mode_scalez,

  // scaling in a plane
  gb_mode_scalexy,
  gb_mode_scaleyz,
  gb_mode_scalezx,

  gb_mode_rotatex,
  gb_mode_rotatey,
  gb_mode_rotatez,

  // open extrusion along single axis
  gb_mode_extrudex,
  gb_mode_extrudey,
  gb_mode_extrudez,

  gb_mode_cutx,
  gb_mode_cuty,
  gb_mode_cutz,
};

/*
These enum values are used to set bitfields that
control what appears on the gumball pop-up menu
and are used to report what was selected from
the menu.
*/
enum GUMBALL_POPUP_MENU_ITEM
{
  gb_menu_nothing          =   0,

  // Relocate current gumball 
  gb_menu_relocate         =   1,

  // Auto-gumball on/off Rhino app setting
  gb_menu_autogumballon    =   2,
  gb_menu_autogumballoff   =   4,

  // Snappy/Smooth Rhino app setting
  gb_menu_snappyon         =   8,
  gb_menu_snappyoff        =  16,

  // Run Rhino GumballSettings command
  gb_menu_settingscmd      =  32,

  // Reset current gumball transformation
  gb_menu_reset            =  64,

  gb_rotate_view_around_autogumball = 128,

  // Gumball alignment setting
  gb_menu_align_to_object = 256,
  gb_menu_align_to_cplane = 512,
  gb_menu_align_to_world = 1024,

  // gumball drag ratio/scale/strength
  gb_menu_drag_ratio = 2048,
  gb_menu_drag_ratio_popup = 4096,

  // gumball autoreset after each drag
  gb_menu_autoreset = 8192,

  // Show "Enter" on top of the gumball menu
  gb_menu_enter = 16384,

  // Gumball alignment setting
  gb_menu_align_to_view = 32768,

  // Gumball merge faces after extrude
  gb_extrude_merge_faces = 65536,

  gb_menu_everything       = 0xFFFFFFFF
};

class RHINO_SDK_CLASS CRhinoGumballDragSettings
{
public:
  // User interface events can modify the way the gumball
  // dragging is interpreted.  GUMBALL_MODE parameters
  // indicate what atomic control is being changed.
  // This class indicates how the action should be 
  // interpreted.
  CRhinoGumballDragSettings();

  // m_bRelocateGumball = false,
  // m_bSnappingEnabled = false
  void SetToDefaultDragSettings();

  // If the shift key is depressed, then
  // m_bSnappingEnabled is set to true.
  // If the control key is depressed, then
  // m_bRelocateGumball is set to true.
  void CheckShiftAndControlKeysXX();

  bool m_bRelocateGumball;
  // If true, the gumball is being relocated
  // and should not transform objects.

  bool m_bSnappingEnabled; 
  // false = ignore the settings below
  // true = use the settings below

private:
  bool m_reserved1;
  bool m_reserved2;
public:

  // Scale lock:
  // If scaling is occurring, m_scale_mode determines if
  // dragging one scale grip should be applied to other
  // scale directions.
  int m_scale_mode; // 0 free, 1 x=y, 2 y=z, 3 z=x, 4 x=y=z

  // Rotation snapping:
  // m_rotation_snap is an angle in radians.  If the
  // angle is > 0, then any rotation will be a multiple
  // of this angle.
  double m_rotation_snap;

  // Translation snapping:
  // If m_translation_snap > 0.0, then any axis translation
  // distances will be a multiple of this distance.
  double m_translation_snap;

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

//////////////////////////////////////////////////////////////////
//
// CRhinoGumballFrame class - gumball location settings
//

class RHINO_SDK_CLASS CRhinoGumballFrame
{
public:
  /*
  Description:
    Get the transformation determined by two
    gumball locations.
  Parameters:
    f0 - [in] starting gumball frame
    f1 - [in] ending gumball frame
    scale_mode - [in] 0 freem 1 x=y, 2 y=z, 3 z=x, 4 x=y=z
    xform - [out]
  */
  static bool GetXform(
    const CRhinoGumballFrame& f0,
    const CRhinoGumballFrame& f1,
    int scale_mode,
    ON_Xform& xform
    );

  /*
  Description:
    Get Rhino status bar value of translation distance, scale, or
    rotation angle.
  Parameters:
    f0 - [in] starting frame
    f1 - [in] ending frame;
    scale_mode - [in] 0 freem 1 x=y, 2 y=z, 3 z=x, 4 x=y=z
    gumball_mode - [in];
  Returns:
    If gumball mode is one of the gmode_rotate*, the returned
    status value is the rotation in radians.
    If gumball mode is one of the gmode_translate*, the returned
    status value is the translation distance.
    If gumball mode is one of the gmode_scale*, the returned
    status value is the scale multiplier.
    If the status  value cannot be computed, then ON_UNSET_VALUE is returned.
  */
  static double StatusValue(
    const CRhinoGumballFrame& f0,
    const CRhinoGumballFrame& f1,
    int scale_mode,
    GUMBALL_MODE gumball_mode
    );

  CRhinoGumballFrame();
  ~CRhinoGumballFrame();

  bool IsValid() const;
  bool Write(ON_BinaryArchive& binary_archive) const;
  bool Read(ON_BinaryArchive& binary_archive);
  void SetToDefaultFrame();
  bool Transform(const ON_Xform& xform);


  // gumball points

  bool SetFromBoundingBox( ON_BoundingBox bbox );

  /*
  Parameters:
    frame - [in]
    bbox - [in]
      bounding box with respect to frame.
  Returns:
    True if input is valid and gumball is set.
    False if input is not valid.  In this case,
    gumball is set to the default.
  */
  bool SetFromBoundingBox( ON_Plane frame, ON_BoundingBox bbox );
  bool SetFromLine( const ON_Line& line );
  bool SetFromPlane( const ON_Plane& plane );
  bool SetFromCircle( const ON_Circle& circle );
  bool SetFromArc( const ON_Arc& arc );
  bool SetFromEllipse( const ON_Ellipse& ellipse );
  bool SetFromCurve( const ON_Curve& curve );
  bool SetFromLight( const ON_Light& light );
  bool SetFromHatch( const ON_Hatch& hatch );
  bool SetFromExtrusion( const ON_Extrusion& extrusion );
  bool SetFromClippingPlane( const ON_PlaneSurface& clipping_plane);

  //These functions only set the size of the scale handles.  They do not affect the gumball origin or rotation.
  //In all cases, the first parameter is the plane which defines the Gumball origin and rotation (ie - the object frame)
  bool SetScaleFromBoundingBox(const ON_Plane& frame, const ON_BoundingBox& bbox);
  bool SetScaleFromLine(const ON_Plane& frame, const ON_Line& line);
  bool SetScaleFromPlane(const ON_Plane& frame, const ON_Plane& plane);
  bool SetScaleFromCircle(const ON_Plane& frame, const ON_Circle& circle);
  bool SetScaleFromArc(const ON_Plane& frame, const ON_Arc& arc);
  bool SetScaleFromEllipse(const ON_Plane& frame, const ON_Ellipse& ellipse);
  bool SetScaleFromCurve(const ON_Plane& frame, const ON_Curve& curve);
  bool SetScaleFromLight(const ON_Plane& frame, const ON_Light& light);
  bool SetScaleFromHatch(const ON_Plane& frame, const ON_Hatch& hatch);
  bool SetScaleFromExtrusion(const ON_Plane& frame, const ON_Extrusion& extrusion);
  bool SetScaleFromClippingPlane(const ON_Plane& frame, const ON_PlaneSurface& clipping_plane);
  bool SetScaleFromAnnotation(const ON_Plane& frame, const class CRhinoAnnotation& annotation);



  bool Translate( ON_3dVector V );

  bool SetCenter(const ON_3dPoint& center);

  bool SetFromBrepObject(const CRhinoBrepObject& brep, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromExtrusionObject(const CRhinoExtrusionObject& ext, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromMeshObject(const CRhinoMeshObject& mesh, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromMorphControlObject(const CRhinoMorphControl& mc, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromSubDObject(const CRhinoSubDObject& ext, double dTol = ON_ZERO_TOLERANCE);

  bool SetScaleFromBrepObject(const ON_Plane& frame, const CRhinoBrepObject& brep, double dTol = ON_ZERO_TOLERANCE);
  bool SetScaleFromExtrusionObject(const ON_Plane& frame, const CRhinoExtrusionObject& ext, double dTol = ON_ZERO_TOLERANCE);
  bool SetScaleFromMeshObject(const ON_Plane& frame, const CRhinoMeshObject& mesh, double dTol = ON_ZERO_TOLERANCE);
  bool SetScaleFromMorphControlObject(const ON_Plane& frame, const CRhinoMorphControl& mc, double dTol = ON_ZERO_TOLERANCE);
  bool SetScaleFromSubDObject(const ON_Plane& frame, const CRhinoSubDObject& ext, double dTol = ON_ZERO_TOLERANCE);

  /*
  Description:
    Get drag to point from picking information when
    the world_pick_point is not on the gumball.  This
    typically happens when a GetPoint or grip drag
    is not being constrained.  The returned value
    in drag_to_point can be passed to DragTo().
  Parameters:
    gumball_mode - [in]
      indicates which part of the gumball is being dragged.
    world_pick_point - [in]
      world coordinate of pick point
    world_pick_line - [in]
      world coordinate pick line
    drag_to_point - [out]
      Point that can be passed to DragTo()     
  */
  bool GetDragToPoint( 
          GUMBALL_MODE gumball_mode,
          ON_3dPoint world_pick_point, 
          ON_Line world_pick_line, 
          ON_3dPoint& drag_to_point
          ) const;

  /*
  Description:
    Get a new location for a gumball frame based on
    an input drag_to_point.
  Parameters:
    gumball_mode - [in] 
       indicates which part of the gumball is being dragged.
    drag_settings - [in]
      Settings that modify default drag behavior.
    drag_to_point - [in]
    dragged_gumball_frame - [out]
    drag_line - [out]
  */
  bool DragTo( 
          GUMBALL_MODE gumball_mode,
          const CRhinoGumballDragSettings& drag_settings,
          ON_3dPoint start_point,
          ON_3dPoint drag_to_point,
          CRhinoGumballFrame& dragged_gumball_frame,
          ON_Line& drag_line
          ) const;

  /*
  Parameters:
    i - [in] 0 = x, 1 = y, 2 = z
  Returns:
    Location of scale grip point or ON_UNSET_POINT
    if it is not defined.
  */
  ON_3dPoint ScaleGripPoint(int i) const;

  /*
  Parameters:
    i - [in] 0 = x, 1 = y, 2 = z
  Returns:
    0.0 if scale grip is distance is not set
    > 0.0 if scale grip distance is set
  */
  double ScaleGripDistance(int i) const;

  bool SetScaleGripDistance(int i, double d);

  bool SetScaleGripDistance(const ON_3dVector&);

  /*
  Parameters:
    i - [in] 0 = x, 1 = y, 2 = z
  Returns:
    frame axis
  */
  const ON_3dVector& Axis(int i) const;

  ON_Line AxisLine(int i,double length) const;

  ON_3dPoint AxisPoint(int i,double t) const;

  /*
  Parameters:
    plane_index - [in]
      0 = get yz plane
      1 = get zx plane
      2 = get xy plane
  */
  bool GetPlane( int plane_index, ON_Plane& plane ) const;

  const ON_3dPoint& Center() const;

  ON_Plane m_plane;
  // if a coordinate is valid and non-zero, then the
  // world coordinate of the scale grip
  // is m_frame.origin + m_scale_grip_distance.X*m_frame.Xaxis
  // where "X" is x, y or z.
  // Otherwise the scale grip is a "snap-back" view
  // sized grip.
  ON_3dVector m_scale_grip_distance;

  int m_scale_mode; // 0 = independent, 1 = xy, 2 = yz, 3 = zx, 4 = xyz

  //-1 = unset, 0 = x, 1 = y, 2 = z, corresponding to the axis face extension is supported along
  signed char m_iExtendAxis;

  static const double scale_tolerance;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};




//////////////////////////////////////////////////////////////////
//
// CRhinoGumballAppearance class - gumball appearance settings
//

class RHINO_SDK_CLASS CRhinoGumballAppearance
{
public:
  CRhinoGumballAppearance();

  /*
  Description:
    Set all CRhinoGumballAppearance fields to their default values.
    Everything is enabled, colors are set calling 
    m_colors.SetToDefaultColors() and sizes are set by calling 
    m_sizes.SetToDefaultSizes();
  */
  void SetToDefaultAppearance();

  void SetEnableScale( const ON_3dVector& frame_scale_grip_distance );

  /*
  Description:
    Saves the CRhinoGumballAppearance in an ON_BinaryArchive.
    This Write function includes a versioned chunk wrapper.
  Parameters:
    binary_archive - [in/out]
      The CRhinoGumballAppearance information is appended to
      this archive.
  Returns:
    True if the write was successful.
  */
  bool Write(ON_BinaryArchive& binary_archive) const;

  /*
  Description:
    Reads the CRhinoGumballAppearance from an ON_BinaryArchive.
  Parameters:
    binary_archive - [in]
      The CRhinoGumballAppearance information is read from
      the archive.  Reading begins at the current
      location in the archive.
  Returns:
    True if the read was successful.
  */
  bool Read(ON_BinaryArchive& binary_archive);

  /*
  Description:
    Saves the CRhinoGumballAppearance settings to a CRhinoProfileContext.
  Parameters:
    lpszSection - [in]
    pc - [in];
  Returns:
    True if the write was successful.
  */
  bool Write( LPCTSTR lpszSection, CRhinoProfileContext& pc ) const;

  /*
  Description:
    Reads the CRhinoGumballAppearance settings from a CRhinoProfileContext.
    Any settings that are not in the profile context are
    set to the default values.
  Parameters:
    lpszSection - [in]
    pc - [in];
  Returns:
    True if the read was successful.
  */
  bool Read( LPCTSTR lpszSection, CRhinoProfileContext& pc );

  // When m_bEnableRelocation is true, the user can reposition
  // the gumball by tapping the control key while dragging.
  // Once the repostion drag is terminated by releasing the
  // mouse button, ordinary editing resumes.  The default
  // setting is true.
  bool m_bEnableRelocate;

  // When m_bEnableMenu is true, the menu "button" is
  // drawn on the gumball. The default setting is true.
  bool m_bEnableMenu;

  // Used for quickly making a cut out of a brep with a selected curve
  // 0 unsupported on selection
  // m_iCutAxis & 1 supported on X
  // m_iCutAxis & 2 supported on Y
  // m_iCutAxis & 4 supported on Z
  signed char m_iCutAxis;

  //-1 = unset, 0 = x, 1 = y, 2 = z, corresponding to the axis face extension is supported along
  signed char m_iExtendAxis;

  // When m_nEnableFreeTranslate is 1, the center
  // translation control can be dragged in any direction
  // and moves the object the gumball controls.
  // When m_nEnableFreeTranslate is 2, the center
  // translation control can be dragged in any direction
  // and moves the object the gumball itself.
  // The default value is 2.
  int m_nEnableFreeTranslate;

  // When m_bEnableX/Y/ZTranslate is true, the X/Y/Z axis
  // translation control is available. The default setting
  // is true.
  bool m_bEnableXTranslate;
  bool m_bEnableYTranslate;
  bool m_bEnableZTranslate;

  // When m_bEnableXY/YZ/ZXTranslate is true, the XY/YZ/ZX
  // plane translation control is available in appropriate views.
  // The default setting is true.
  bool m_bEnableXYTranslate;
  bool m_bEnableYZTranslate;
  bool m_bEnableZXTranslate;

  // When m_bEnableX/Y/ZRotate is true, the X/Y/Z
  // rotation control is available. The default setting
  // is true.
  bool m_bEnableXRotate;
  bool m_bEnableYRotate;
  bool m_bEnableZRotate;

  // When m_bEnableX/Y/ZScale is true, the X/Y/Z
  // scale control is available. The default setting
  // is true.
  bool m_bEnableXScale;
  bool m_bEnableYScale;
  bool m_bEnableZScale;

  // When m_bEnableX/Y/ZExtrude is true, the X/Y/Z axis
  // extrude control is available. The default setting
  // is false.
  bool m_bEnableXExtrude;
  bool m_bEnableYExtrude;
  bool m_bEnableZExtrude;

  // At the moment, all gumballs use the same app setting
  // CRhinoGumball::default_bSnappy.

  //// Snappy setting for this gumball. Initialized to
  //// CRhinoGumballAppearance::default_bSnappy;
  //bool m_bSnappy; // true = snappy, false = smooth setting 

  CRhinoGumballColors m_colors;
  CRhinoGumballSizes m_sizes;

  /*
    This static value is used by the CRhinoGumballAppearance constructor 
    to set the m_bSnappy value.  It is set by CRhinoApp.AppSettings().SetSnappyGumball().
  */
  static bool default_bSnappy;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};


//////////////////////////////////////////////////////////////////
//
// CRhinoGumballPickResult class 
//   Used to return picking results
//

class RHINO_SDK_CLASS CRhinoGumballPickResult
{
public:
  CRhinoGumballPickResult();

  void SetToDefaultPickResult();

  // If m_gumball_mode is gb_mode_nothing, then
  // the pick missed.
  GUMBALL_MODE m_gumball_mode;
  double m_pick_depth;
  double m_pick_distance;
  // point to use when CRhinoGumball::m_appearance.m_bSnappy is false;
  ON_3dPoint m_gsnap_off_pick_point;
  // point to use when CRhinoGumball::m_appearance.m_bSnappy is true;
  ON_3dPoint m_gsnap_on_pick_point;

  /*
  Parameters:
    bSnappyGumball - [in] 
      Value of gumball.m_appearance_settings.m_bSnappy
  Returns:
    m_gsnap_off_pick_point if bSnappyGumball is false.
    m_gsnap_on_pick_point if bSnappyGumball is true.
  */
  ON_3dPoint PickPoint( bool bSnappyGumball ) const;

  // Dragging constraints
  int m_drag_constraint; // 0 = none, 
                    // 1 = infinite line
                    // 2 = infinite ray
                    // 3 = circle (start = pick point)
                    // 4 = plane
  ON_Line m_line_constraint;
  ON_Plane m_plane_constraint;
  ON_Circle m_gsnap_off_circle_constraint;
  ON_Circle m_gsnap_on_circle_constraint;

  /*
  Returns:
    m_gsnap_off_circle_constraint if bSnappyGumball is false.
    m_gsnap_on_circle_constraint if bSnappyGumball is true.
  */
  ON_Circle CircleConstraint( bool bSnappyGumball ) const;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

//////////////////////////////////////////////////////////////////
//
// CRhinoGumball class - frame and appearance settings
//

class RHINO_SDK_CLASS CRhinoGumball
{
public:
  ON_DEPRECATED_MSG("Gumball should be created with the view type") CRhinoGumball();
  CRhinoGumball(ON::active_space);
  ~CRhinoGumball();

  CRhinoGumball(const CRhinoGumball&);
  const CRhinoGumball& operator=(const CRhinoGumball&);

  CRhinoGumball(CRhinoGumball&&);
  CRhinoGumball& operator=(CRhinoGumball&&);

  void SetToDefaultGumball();

  void SetActiveSpace(ON::active_space);
  ON::active_space ActiveSpace(void) const;

  bool IsValid() const;
  bool Write(ON_BinaryArchive& binary_archive) const;
  bool Read(ON_BinaryArchive& binary_archive);
  bool Transform(const ON_Xform& xform);

  bool SetCenter(const ON_3dPoint& center);

  bool SetFromBoundingBox( ON_BoundingBox bbox );
  bool SetFromBoundingBox( ON_Plane frame, ON_BoundingBox bbox );
  bool SetFromLine( const ON_Line& line );
  bool SetFromPlane( const ON_Plane& plane );
  bool SetFromArc( const ON_Arc& arc );
  bool SetFromCircle( const ON_Circle& circle );
  bool SetFromEllipse( const ON_Ellipse& ellipse );
  bool SetFromLight( const ON_Light& light );
  bool SetFromHatch( const ON_Hatch& hatch );
  bool SetFromCurve( const ON_Curve& curve );
  bool SetFromExtrusion( const ON_Extrusion& extrusion );
  bool SetFromClippingPlane(const ON_PlaneSurface& clipping_plane);
  bool SetFromBrepObject(const CRhinoBrepObject& brep, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromExtrusionObject(const CRhinoExtrusionObject& ext, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromMeshObject(const CRhinoMeshObject& ext, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromMorphControlObject(const CRhinoMorphControl& mc, double dTol = ON_ZERO_TOLERANCE);
  bool SetFromSubDObject(const CRhinoSubDObject& ext, double dTol = ON_ZERO_TOLERANCE);
  

  bool Pick( const CRhinoPickContext& pick_context,
             CRhinoGumballPickResult& pick_result
             ) const;

  /*
  Description:
    Get the location of a gumball menu ball in window coordinates.
    Useful when calling CRhinoGumballPopUpMenu::ShowMenu().
  Parameters:
    vp - [in]
      Typically CRhinoView.Viewport().VP()
    popup_menu_point - [out]
  Returns:
    true if popup_menu_point was set.
  */
  bool GetPopUpMenuPoint( 
          const CRhinoViewport& vp, 
          ON_2iPoint& popup_menu_point
          ) const;

  /*
  Description:
    Draw gumball and dragging decorations that indicate how the
    gumball is moving.
  Parameters:
    dp - [in]
    gumball_mode - [in] gb_mode_nothing will draw complete gumball
                        or pass the mode returned by CRhinoGumball::Pick(...)
                        to draw what matters when a dynamic drag is
                        happening.
    drag_line - [in] line returned by m_frame.Drag(...)
    start_frame - [in] starting point for the gumball
    highlight_mode - [in] indicates which gumball element to draw highlighted
  */
  void DrawDynamicGumball(CRhinoDisplayPipeline& dp, 
                          GUMBALL_MODE gumball_mode, 
                          const ON_Line& drag_line,
                          const CRhinoGumballFrame& start_frame,
                          GUMBALL_MODE highlight_mode = gb_mode_nothing
                          ) const;

  /*
  Description:
    Draw gumball.
  Parameters:
    dp - [in]
    highlight_mode - [in] indicates which gumball element to draw highlighted
  */
  void DrawStaticGumball(CRhinoDisplayPipeline& dp, GUMBALL_MODE highlight_mode = gb_mode_nothing) const;

  /*
  Returns:
    World coordinate bounding box.  The bounding box
    depends on the viewport that is displaying the gumball.
  */
  ON_BoundingBox BoundingBox( const CRhinoViewport& viewport ) const;

public:
  CRhinoGumballFrame m_frame;
  CRhinoGumballAppearance m_appearance;
private:
  std::unique_ptr<class CRhinoGumball_Private> m_private;
};

class RHINO_SDK_CLASS CRhinoGumballPopUpMenu
{
public:
  CRhinoGumballPopUpMenu();

  /*
  Description:
    Show the gumball popup menu.
  Parameters:
    wnd - [in] 
      Parent window.  Typically a status bar or a CRhinoView.
    point - [in]
      window coordinates of the lower left corner of the pop-up
      CRhinoGumball::GetPopUpMenuPoint() can be used to get the
      screen location of a gumball's menu ball.
    menu_ites - [in]
      bitfield that determine what items should appear on the
      pop-up menu. Use GUMBALL_POPUP_MENU_ITEM to set the bits.
  Returns:
    Selected item.  The caller is responsible for actually doing
    something.  The function RespondToPopUpMenuItem() can 
    be used to handle the app settings menu items.
  */
  static
  GUMBALL_POPUP_MENU_ITEM ShowPopUpMenu(
	  HWND wnd,
          const ON_2iPoint& point,
          unsigned int menu_items
          );

  /*
  Description:
    Handle the app settings menu items.
  Parameters:
    gb_menu_item - [in]
      gumball pop-up menu item to handle.
  Returns:
    True if gb_menu_item corresponds to an app settings item. In which
    case, the appropriate action is taken.  False if gb_menu_item is
    a gumball specific item, like gb_menu_reposition, that applies to
    a single gumball.
  */
  static 
  bool RespondToPopUpMenuItem(GUMBALL_POPUP_MENU_ITEM gb_popup_menu_item);

  /*
  Description:
    Call this method to get the command line string to append to the
    top of the context menu.
  Parameters:
    value - [out]
      String to display.
  Returns:
    True if a string should be added and value is not empty.
  */
  static bool GetCommandLineString(ON_wString& value);
};


class RHINO_SDK_CLASS CRhinoGumballDisplayConduit : public CRhinoDisplayConduit
{
public:
  ON_DEPRECATED_MSG("Use the version that takes a CRhinoView::view_type") CRhinoGumballDisplayConduit(CSupportChannels channel_mask = CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY);
  CRhinoGumballDisplayConduit(ON::active_space, CSupportChannels channel_mask=CSupportChannels::SC_CALCBOUNDINGBOX|CSupportChannels::SC_DRAWOVERLAY);
  virtual ~CRhinoGumballDisplayConduit();

  /*
  Description:
    Completely disables this CRhinoGumballDisplayConduit and
    sets every field as if this CRhinoGumballDisplayConduit were
    just constructed.  Use this function when you'd like to
    delete a CRhinoGumballDisplayConduit but you cannot because
    you are inside of ExecConduit or some other function.
  */
  void DisableAndDestroy();
  
  // override of virtual CRhinoDisplayConduit function
  bool ExecConduit( 
          CRhinoDisplayPipeline& pipeline,
          UINT channel,
          bool& terminate 
          );  

  bool SetBaseGumball( const CRhinoGumball& gumball );

  bool GumballDrawIsEnabled() const;
  void EnableGumballDraw(bool bEnable);

  // The pre-transform is a transformation that needs to be applied
  // before the gumball transformation.
  bool SetPreTransform( const ON_Xform& xform0 );
  bool PreTransformIsIdentity() const;
  const ON_Xform& PreTransform() const;

  // The gumball transformation is the transformation 
  // calculated by comparing the current m_gumball to 
  // to the starting m_base_gumball.
  const ON_Xform& GumballTransform() const;

  // The total transformation is GumballTransform()*PreTransformation().
  const ON_Xform& TotalTransform() const;

  const CRhinoGumball& BaseGumball() const;
  const CRhinoGumball& Gumball() const;
  const ON_Line& DragLine() const;

  // Update location of m_gumball using point and world_line.
  // The inputs are often determined my a mouse location.
  // Also updates m_gumball_xform, m_total_xform and then
  // calls the virtual UpdateCallback().
  bool UpdateGumball( ON_3dPoint point, ON_Line world_line );
  bool UpdateGumball( const CRhinoGumballFrame& gbframe );
  
  // updates Rhion status bar based on current gumball location.
  void UpdateRhinoStatusBar() const;

  bool RelocateBaseGumball( const CRhinoGumballFrame& base_gbframe );

  // The initial mouse down event sets m_pick_result
  CRhinoGumballPickResult m_pick_result;

  // gumball relocation and snapping
  CRhinoGumballDragSettings m_drag_settings;

  void CheckShiftAndControlKeys();

  // Command helper functions
  //void DrawDragLine(CRhinoDisplayPipeline& dp) const;

  bool PickGumball(const CRhinoPickContext& pick_context,CRhinoGetPoint* gp);

private:

  //////////////////////////////////////////////////////////////////
  //
  // implementation
  //

  bool m_bDrawGumball;

  // starting location
  CRhinoGumball m_base_gumball; 
  
  // current location information (valid if m_gumball_mode != gb_mode_nothing)
  CRhinoGumball m_gumball;
  ON_Line m_drag_line;

  // transform to apply before gumball transform
  bool m_bHaveXform0;
  ON_Xform m_xform0;

  bool m_bHaveGumballXform;
  ON_Xform m_gumball_xform; // transform from m_base_gumball to m_gumball
  ON_Xform m_total_xform; // = m_gumball_xform*m_xform0

  /*
  Description:
    If m_bDrawGumball is true, then ExecConduitCalcBBoxHelper(...)
    calls m_pChannelAttrs->m_BoundingBox.Union(bbox) where
    bbox contains the objects used to display a gumball in
    the pipeline's view.
  */
  void ExecConduitCalcBBoxHelper( CRhinoDisplayPipeline& pipeline );

  /*
  Description:
    If m_bDrawGumball is true, then ExecConduitDrawOverlayHelper(...)
    draws the appropriate gumball.
  */
  void ExecConduitDrawOverlayHelper( CRhinoDisplayPipeline& pipeline );

  /*
  Description:
    Low level tool that calculates the transformation from 
    m_base_gumball to m_gumball.  Ignores m_xform0.
    In general, instead of calling this function, you should
    use m_total_xform or m_gumball_xform.
  Parameters:
    xform - [out]
  Returns:
    True if successful.
  */
  bool GetGumballXform(ON_Xform& gumball_xform) const;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

class RHINO_SDK_CLASS CRhinoGumballDragger : public CRhinoGetPoint
{
public:
  CRhinoGumballDragger(CRhinoGumballDisplayConduit& dc);

  // lets user drag m_gumball around.
  CRhinoGet::result DragGumball();

  void OnMouseDown( 
          CRhinoViewport& vp,
          UINT nFlags,
          const ON_3dPoint& point,
          const ON_2iPoint* view_wnd_point
          );

  void OnMouseMove( 
          CRhinoViewport& vp,
          UINT nFlags,
          const ON_3dPoint& point,
          const ON_2iPoint* view_wnd_point
          );

  void DynamicDraw( CRhinoDisplayPipeline& dp, const ON_3dPoint& pt );

  // The gumball and support functions are members of m_dc.
  CRhinoGumballDisplayConduit& m_dc;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

