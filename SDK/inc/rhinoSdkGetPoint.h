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

#include "rhinoSdkGet.h"
#include "rhinoSdkSnap.h"


/*
Description:
  CRhinoGetPoint is the Rhino SDK user interface tool used to get points.
  It is derived from CRhinoGet which is used to provide the command prompt
  and option setting tools that are common to all the Rhino SDK getting
  tools.

  Step 1:
    Use the CRhinoGet member functions to specify command prompt, 
    command options, and what types of non-point input are acceptable.
    For example, if you want the user to be able to either pick a point
    or type a number, then you would call CRhinoGet::AcceptNumber(true).
    The default command prompt is a localized "Pick point" prompt.

  Step 2:
    Use the CRhinoGetPoint member functions to customize point
    picking behavior.

  Step 3:
    Call CRhinoGetPoint::GetPoint();

  Step 4:
    Examine the value returned by CRhinoGet::Result() and react
    accordingly.

Example:
    CRhinoCommand::result my_center_radius_getter(
                                ON_3dPoint& my_center, 
                                double* my_radius 
                                )
    {
      double radius = 1.0;
      ON_3dPoint center(0,0,0);

      CRhinoGetPoint gp;
      gp.SetCommandPrompt( L"Center point" );
      gp.SetCommandPromptDefault( L"(0,0,0)" );
      gp.AddCommandOptionNumber( L"radius", &radius, "New radius",
                                  false, ON_ZERO_TOLERANCE);
      gp.AcceptNothing(true);
      for (;;)
      {
        gp.GetPoint();
        switch (gp.Result())
        {
        case CRhinoGet::cancel:
          return CRhinoCommand::cancel; // user pressed ESCAPE
          break;

        case CRhinoGet::nothing:
          // user pressed enter - use default center value
          break;

        case CRhinoGet::option:
          // user changed radius value - radius automatically updated
          continue;
          break;

        case CRhinoGet::point:
          center = gp.Point();
          break;

        default:
          continue;
          break;
        }
        break;
      }
      my_center = center;
      *my_radius = radius;
      return CRhinoCommand::success;
    }
  
See Also:
  CRhinoGetObject, CRhinoGetString, CRhinoGetInteger, CRhinoGetNumber, CRhinoGetOption
*/
class RHINO_SDK_CLASS CRhinoGetPoint : public CRhinoGet
{
public:
  CRhinoGetPoint();
  virtual ~CRhinoGetPoint();
  CRhinoGetPoint(const CRhinoGet& src ); // prompt and options copied

  // no implementation
  CRhinoGetPoint(const CRhinoGetPoint &) = delete;
  CRhinoGetPoint& operator=(const CRhinoGetPoint &) = delete;

  /*
  Description:
    Set base point used by ortho snap, from snap, planar snap, etc.
  Parameters:
    basePoint - [in]
    showDistanceInStatusBar - [in] if true, then the distance from
       base_point to the current point will be in the
       status bar distance pane.
  */
  void SetBasePoint( 
    ON_3dPoint basePoint, 
    bool showDistanceInStatusBar = false
    );

  // Description:
  //   Location of base point used by ortho snap, from snap,
  //   etc., and DrawLineFromPoint().
  // Parameters:
  //   base_point - [in] pass ON_unset_point to clear the base point.
  // Returns:
  //   Location of base point.  If value is ON_unset_point, then
  //   the base point is not set.
  // Remarks:
  //   Default base_point is world (0,0,0)
  // See Also:
  //   SetBasePoint
  ON_3dPoint BasePoint() const;

  // Description:
  //   Returns true if SetBasePonint has been called
  // Returns:
  //   true  - SetBasePoint has been called.
  //   false - SetBasePoint has not been called.
  // See Also:
  //   SetBasePoint
  BOOL32 HasBasePoint() const;

  /*
  Description:
    Set distance constraint from base point
  Parameters:
    distance_from_basepoint - [in] pass ON_UNSET_VALUE to clear
        this constraint.  Pass 0.0 to disable the ability to set this
        constraint by typing a number during GetPoint.
  Remarks:
    If the base point is set and the distance from base point constraint
    is > 0, then the picked point is constrained to be this distance
    from the base point.
  See Also:
    CRhinoGetPoint::BasePoint
    CRhinoGetPoint::PermitFromOption
  */
  void ConstrainDistanceFromBasePoint( 
    double distance_from_basepoint
    );

  /*
  Description:
    Check if point is constrained to be on a plane.
  Parameters:
    vp - [in] NULL or viewport to use for construction plane.
    plane - [out] If GetPlanarConstraint() returns true, then
                  the planar constraint is returned here.
  Example:
          CRhinoView* view = ...;
          ON_Plane plane;
          if ( GetPlanarConstraint( &view->Viewport(), plane ) )
          {
            ...
          }
  Returns:
    true if there is an active planar constraint.
  */
  BOOL32 GetPlanarConstraint( 
    const CRhinoViewport* vp,
    ON_Plane& plane 
    ) const;

  /*
  Description:
    Set the color used by CRhinoGetPoint::DynamicDraw
    to draw the current point and the line from the 
    base point to the current point.
  Remarks:
    The default is RhinoApp().AppSettings().ActiveLayerColor().
  See Also:
    DynamicDrawColor, EnableDrawLineFromPoint
  */
  void SetDynamicDrawColor( COLORREF color );

  /*
  Description:
    The color used by CRhinoGetPoint::DynamicDraw
    to draw the current point and the line from the 
    base point to the current point.
  Returns:
    Current dynamic draw color.
  See Also:
    CRhinoGetPoint::SetDynamicDrawColor
  */
  ON_Color DynamicDrawColor() const;

  // Description:
  //   Use DrawLineFromPoint() if you want a dynamic line
  //   drawn from a point to the point being picked.
  // Parameters:
  //   start_point - [in] The line is drawn from start_point
  //       to the point being picked.  If the base point has
  //       not been set, then it is set to start_point.
  //   bShowDistanceInStatusBar - [in] if true, the distance
  //       from the base_point to the point begin picked is
  //       shown in the status bar.
  // Remarks:
  //   Calling DrawLineFromPoint automatically enables drawing
  //   the line.  Use EnableDrawLineFromPoint() to toggle the
  //   line drawing state.
  // See Also:
  //   EnableDrawLineFromPoint, SetBasePoint.
  void DrawLineFromPoint( 
         ON_3dPoint start_point,
         BOOL32 bShowDistanceInStatusBar
         );

  // Description:
  //   Controls drawing of dynamic a line from the start point.
  // Parameters
  //   bDrawLineFromPoint - [in] if true, a dynamic line
  //       is drawn from the DrawLineFromPoint start_point
  //       to the point being picked.
  // Remarks:
  //   Use DrawLineFromPoint to specify the start point, color,
  //   and status bar behavior.
  // See Also:
  //   DrawLineFromPoint, SetBasePoint.
  void EnableDrawLineFromPoint(BOOL32 bDrawLineFromPoint = true);

  // Description:
  //   Controls availability of ortho snap.  Default is true.
  // Parameters:
  //   bPermitOrthoSnap - [in] if true, then GetPoint pays attention
  //       to the Rhino "ortho snap" and "planar snap" settings
  //       reported by RhinoApp().AppSettings().Ortho() and
  //       RhinoApp().AppSettings().Planar().
  // Remarks:
  //   The base point must be set and ortho snapping must be permitted
  //   and RhinoApp().AppSettings().Ortho() must be true for
  //   ortho snapping to work.
  // See Also:
  //   SetBasePoint, PermitFromOption.
  void PermitOrthoSnap(BOOL32 bPermitOrthoSnap = true);

  // Description:
  //   Returns the ortho permitter state set with PermitOrthoSnap. By default returns true.
  // See Also:
  //   PermitOrthoSnap
  bool OrthoSnap() const;
  
  // Description:
  //   Returns true if m_bEnableOsnap is false and vice versa. It also sets the mode to m_osnap_override;
  bool OsnapOverideEnabled(CRhinoAppSettings::osnap_mode& mode) const;

  /*
  Description:
    Control the availability of the built-in "From" option.
    By default, the "From" option is enabled.
  Parameters:
    bPermitFromOption - [in] if true, then the "From" option
    is automatically available in GetPoint.
  Remarks:
    The GetPoint "From" option is never visible on the 
    command line and the user must type the complete 
    option name to activate the "From" option.  
    When the GetPoint "From" snap is enabled, the user 
    set/change the base point during GetPoint by typing
    "From" and picking a point.

    A related option is the builit-in distance from base point
    constraint that is can be set before GetPoint is called by
    passing a value to 
    CRhinoGetPoint::ConstrainDistanceFromBasePoint 
    or during GetPoint by entering a number.
  See Also:
    CRhinoGetPoint::SetBasePoint
    CRhinoGetPoint::PermitOrthoSnap
    CRhinoGetPoint::ConstrainDistanceFromBasePoint
    CRhinoGetPoint::PermitConstraintOptions
  */
  void PermitFromOption(BOOL32 bPermitFromOption = true);

  /*
  Description:
    Control the availability of the built-in linear, planar, curve,
    and surface constraint options like "Along", "AlongPerp", 
    "AlongTan", "AlongParallel", "Between", "OnCrv", "OnSrf", 
    ".x", ".y", ".z", ".xy", etc.
  Parameters:
    bPermitConstraintOptions - [in] if true, then the built-in
    constraint options are automatically available in GetPoint.
  Remarks:
    By default, these built-in constraint options are available 
    unless an explicit constraint is added by calling one of the
    CRhinoGetPoint::Constrain functions.  
    Calling CRhinoGetPoint::ClearConstraints automatically enables
    the built-in constraint options.
    The built-in constraint options are never visible on the 
    command line and the user must type the complete 
    option name to activate these options.
  See Also:
    CRhinoGetPoint::SetBasePoint
    CRhinoGetPoint::PermitOrthoSnap
    CRhinoGetPoint::PermitFromOption
  */
  void PermitConstraintOptions(BOOL32 bPermitConstraintOptions = true);

  /*
  Description:
    Permit use of the tab key to define a line constraint.
  Parameters:
    bPermitTabMode - [in] if true, then the built-in
    tab key mode is available.
  Remarks:
    By default, use of the tab key is supported.
  */
  void PermitTabMode(
    bool bPermitTabMode = true
    );

  /*
  Description:
    Returns the current tab mode setting.
  Parameters:
    tab_mode      - [out] The current tab mode, 0 = off, 1 = on.
    tab_mode_line - [out] The current line constraint.
  Returns:
    True of tab mode is permitted, false otherwise.
  See Also:
    CRhinoGetPoint::PermitTabMode
  */
  bool GetTabModeSettings( int* tab_mode = 0, ON_Line* tab_mode_line = 0 );

  /*
  Description:
    Permit use of the control key to define a line constraint.
  Parameters:
    permit_elevator_mode - [in] 
       0: no elevator modes are permitted
       1: fixed plane elevator mode (like the Line command)
       2: cplane elevator mode (like object dragging)
  Remarks:
    By default, mode 1 is enabled.
  */
  void PermitElevatorMode(
    int permit_elevator_mode = 1
    );

  /*
  Description:
    By default, object snaps like "end", "near", etc. are
    controlled by the user.  If you want to disable this
    ability, then call PermitObjectSnap(false).
  Parameters:
    bPermitObjectSnap - [in] true to permit snapping to objects.
  */
  void PermitObjectSnap(
    BOOL32 bPermitObjectSnap=true
    );

  /*
  Description:
    Used to prevent snapping to a specific object.
  Parameters:
    object - [in] object that should not be snapped to.    
  */
  void ProhibitObjectSnap( const CRhinoObject* object );

  /*
  Description:
    Add a point to the list of osnap points
  Parameters:
    snap_point - [in]
  Returns:
    Total number of snap points
  Remarks:
    When point osnap is enabled, GetPoint will snap to points in
    the Rhino model.  If you want the user to be able to snap to
    additional points, then use CRhinoGetPoint::AddSnapPoints to 
    specify the locations of these additional points.
  See Also:
    CRhinoGetPoint::AddSnapPoints
    CRhinoGetPoint::ClearSnapPoints
    CRhinoGetPoint::GetSnapPoints
    CRhinoGetPoint::AddSnapPoint
    CRhinoGetPoint::AddConstructionPoint
  */
  int AddSnapPoint( 
    const ON_3dPoint& snap_point
    );

  /*
  Description:
    Add points to the list of snap points.
  Parameters:
    point_count - [in]
    snap_points - [in]
  Returns:
    Total number of snap points
  Remarks:
    When point osnap is enabled, GetPoint will snap to points in
    the Rhino model.  If you want the user to be able to snap to
    additional points, then use CRhinoGetPoint::AddSnapPoints to 
    specify the locations of these additional points.
  See Also:
    CRhinoGetPoint::AddSnapPoint
    CRhinoGetPoint::ClearSnapPoints
    CRhinoGetPoint::GetSnapPoints
    CRhinoGetPoint::AddSnapPoint
  */
  int AddSnapPoints( 
    int point_count,
    const ON_3dPoint* snap_points
    );

  /*
  Description:
    Add points to the list of snap points.
  Parameters:
    bRationalPoints - [in] true if points are 4d rational homogeneos points
                           false if points are 3d eudlidean points
    point_count - [in] number of points
    point_stride - [in] number of doubles to skip between points
                   ( >= bRationalPoints ? 4 : 3 )
  Returns:
    Total number of snap points.
  Remarks:
    When point osnap is enabled, GetPoint will snap to points in
    the Rhino model.  If you want the user to be able to snap to
    additional points, then use CRhinoGetPoint::AddSnapPoints to 
    specify the locations of these additional points.
  See Also:
    CRhinoGetPoint::AddSnapPoint
    CRhinoGetPoint::ClearSnapPoints
    CRhinoGetPoint::GetSnapPoints
    CRhinoGetPoint::AddSnapPoint
  */
  int AddSnapPoints( 
    BOOL32 bRationalPoints,
    int point_count,
    int point_stride,
    const double* points
    );

  /*
  Description:
    Remove all snap points.
  Remarks:
    When point osnap is enabled, GetPoint will snap to points in
    the Rhino model.  If you want the user to be able to snap to
    additional points, then use CRhinoGetPoint::AddSnapPoints to 
    specify the locations of these additional points.
  See Also:
    CRhinoGetPoint::AddSnapPoint
    CRhinoGetPoint::AddSnapPoints
    CRhinoGetPoint::GetSnapPoints
  */
  void ClearSnapPoints();

  /*
  Description:
    Get current snap points.
  Parameters:
    snap_points - [out]
  Returns:
    number of snap points
  Remarks:
  See Also:
    CRhinoGetPoint::AddSnapPoint
    CRhinoGetPoint::AddSnapPoints
    CRhinoGetPoint::ClearSnapPoints
  */
  int GetSnapPoints( 
    ON_SimpleArray<ON_3dPoint>& snap_points 
    ) const;

  /*
  Description:
    Add a point to the list of construction points
  Parameters:
    construction_point - [in]
  Returns:
    Total number of construction points
  Remarks:
    Construction points are like snap points except that they
    get snapped to even when point osnap is off.  Typically, there
    are only a few construction points while there can be many snap
    points.  For example, when polylines are drawn the start point
    is a construction point and the other points are snap points.
  See Also:
    CRhinoGetPoint::ClearConstructionPoint
    CRhinoGetPoint::GetConstructionPoint
    CRhinoGetPoint::AddSnapPoint
  */
  int AddConstructionPoint( 
    const ON_3dPoint& construction_point
    );

  /*
  Description:
    Remove all construction points.
  Remarks:
    Construction points are like snap points except that they
    get snapped to even when point osnap is off.  Typically, there
    are only a few construction points while there can be many snap
    points.  For example, when polylines are drawn the start point
    is a construction point and the other points are snap points.
  See Also:
    CRhinoGetPoint::AddConstructionPoint
    CRhinoGetPoint::GetConstructionPoint
    CRhinoGetPoint::AddSnapPoint
  */
  void ClearConstructionPoints();

  /*
  Description:
    Get current construction points.
  Parameters:
    construction_points - [out]
  Returns:
    number of construction points
  Remarks:
    Construction points are like snap points except that they
    get snapped to even when point osnap is off.  Typically, there
    are only a few construction points while there can be many snap
    points.  For example, when polylines are drawn the start point
    is a construction point and the other points are snap points.
  See Also:
    CRhinoGetPoint::AddConstructionPoint
    CRhinoGetPoint::ClearConstructionPoint
    CRhinoGetPoint::AddSnapPoint
  */
  int GetConstructionPoints( 
    ON_SimpleArray<ON_3dPoint>& construction_points 
    ) const;

  /*
  Description:
    Controls display of the curve snap tangent bar icon.
  Parameters:
    bDrawTangentBarAtSnapPoint - [in] true to draw a tangent bar icon
        whenever GetPoint snaps to a curve.
    bDrawEndPoints - [in] true to draw points at the end of the
        tangent bar.
  Remarks:
    The tangent bar is drawn by CRhinoGetPoint::DynamicDraw.
    If you override CRhinoGetPoint::DynamicDraw,
    then you must call the base class function.
  See Also:
    CRhinoGetPoint::EnableCurveSnapArrow
    CRhinoGetPoint::EnableSnapToCurves
    CRhinoGetPoint::Constrain( const ON_Curve& )
  */
  void EnableCurveSnapTangentBar( 
          BOOL32 bDrawTangentBarAtSnapPoint = true,
          BOOL32 bDrawEndPoints = false
          );

  /*
  Description:
    Controls display of the curve snap perpendicular bar icon.
  Parameters:
    bDrawPerpBarAtSnapPoint - [in] true to draw a tangent bar icon
        whenever GetPoint snaps to a curve.
    bDrawEndPoints - [in] true to draw points at the end of the
        tangent bar.
  Remarks:
    The tangent bar is drawn by CRhinoGetPoint::DynamicDraw.
    If you override CRhinoGetPoint::DynamicDraw,
    then you must call the base class function.
  See Also:
    CRhinoGetPoint::EnableCurveSnapArrow
    CRhinoGetPoint::EnableSnapToCurves
    CRhinoGetPoint::Constrain( const ON_Curve& )
  */
  void EnableCurveSnapPerpBar( 
          BOOL32 bDrawPerpBarAtSnapPoint = true,
          BOOL32 bDrawEndPoints = false
          );

  /*
  Description:
    Controls display of the curve snap arrow icon.
  Parameters:
    bDrawDirectionArrowAtSnapPoint - [in] true to draw arrow icon
        whenever GetPoint snaps to a curve.
    bReverseArrow - [in] true if arrow icon direction should be the
        reverse of the first derivative direction.
  Remarks:
    The tangent bar is drawn by CRhinoGetPoint::DynamicDraw.
    If you override CRhinoGetPoint::DynamicDraw,
    then you must call the base class function.
  See Also:
    CRhinoGetPoint::EnableCurveSnapTangentBar
    CRhinoGetPoint::EnableSnapToCurves
    CRhinoGetPoint::Constrain( const ON_Curve& )
  */
  void EnableCurveSnapArrow( 
    BOOL32 bDrawDirectionArrowAtSnapPoint = true,
    BOOL32 bReverseArrow = false
    );


  // If you want GetPoint() to try to snap to curves when the mouse is near
  // a curve (like the center point in the Circle command when the
  // AroundCurve option is on), then enable the snap to curves option.
  void EnableSnapToCurves(BOOL32=true);

  /*
  Description:
    By default, hidden one-shot object snaps, like typing "e"
    or "end" and getting a one-shot end snap, are enabled.  
    You may disable them by calling 
    EnableOneShotOsnapHiddenOptions(false).
  */
  void EnableOneShotOsnapHiddenOptions(bool bEnable);


  // You may use one the following to constrain the picked point to lie
  // on a line, plane, curve, surface, ...
  BOOL32 Constrain( ON_3dPoint, ON_3dPoint ); // constrain to a line
  BOOL32 Constrain( const ON_Line&, BOOL32 bTrackFromStart = true );        // constrain to a line
  BOOL32 Constrain( const ON_Arc& );        // constrain point to lie on an arc
  BOOL32 Constrain( const ON_Circle& );     // constrain point to lie on a circle
  BOOL32 Constrain( const ON_Plane&, bool bAllowElevator = false );      // constrain point to lie on a plane
  BOOL32 Constrain( const ON_Sphere& );     // constrain point to lie on a sphere
  BOOL32 Constrain( const ON_Cylinder& );   // constrain point to lie on a cylinder
  
  // bAllowPickingPointOffObject defines whether the point pick is allowed to 
  // happen off object. When false, a "no no" cursor is shown when the cursor is not
  // on the object. When true, a normal point picking cursor is used and the marker
  // is visible also when the cursor is not on the object.

  BOOL32 Constrain( const ON_Curve&, bool bAllowPickingPointOffObject = false );
  BOOL32 Constrain( const ON_Surface&, bool bAllowPickingPointOffObject = false );

  // When face_index <0, constrain to whole brep. When face_index >=0, constrain to individual face.
  // When wire_density<0, isocurve intersection snapping is turned off, when wire_density>=0, the value
  // defines the isocurve density used for isocurve intersection snapping. 
  BOOL32 Constrain( const ON_Brep&, int wire_density = -1, int face_index = -1, bool bAllowPickingPointOffObject = false );
  BOOL32 Constrain( const ON_BrepFace&, int wire_density = -1, bool bAllowPickingPointOffObject = false );

  BOOL32 Constrain(const ON_Mesh&, bool bAllowPickingPointOffObject = false);
  // Expert version of mesh constraint. Has an optional offset option, and the passed in mesh is used without
  // making a copy which means it must not be deleted while CRhinoGetPoint is in scope.
  BOOL32 ConstrainEx(const ON_Mesh&, bool bAllowPickingPointOffObject = false, double dOffset = 0.0);
  // Expert version of SubD constraint. Has an optional offset option, and the passed in SubD is used without
  // making a copy which means it must not be deleted while CRhinoGetPoint is in scope.
  BOOL32 ConstrainEx(const ON_SubD&, bool bAllowPickingPointOffObject = false, double dOffset = 0.0);

  // Description:
  //   If enabled, the picked point is constrained to be on the 
  //   active construction plane.  If the base point is set, then the 
  //   point is constrained to be on the plane that contains the base 
  //   point and is parallel to the active construction plane.  
  //   By default this constraint is enabled.
  // Parameters:
  //   bThroughBasePoint - [in] if true and the base point is set,
  //       then the point is constrained to be on the plane parallel
  //       to the construction plane that passes through the base point,
  //       even when planar mode is off. If bThroughBasePoint is false, 
  //       then the base point shift only happens if planar mode is on.
  //   bProjectOsnaps - [in] if true, then osnaps are projected to the current
  //       constraint plane. It's identical to turning on projecting osnaps
  //       in the osnap dialog, and also planar mode if bThroughBasePoint is true.
  // Remarks:
  //   "Planar" mode is on if RhinoApp().AppSettings().Planar() is true.
  // See Also:
  //   SetBasePoint.
  BOOL32 ConstrainToConstructionPlane( BOOL32 bThroughBasePoint = true, BOOL32 bProjectOsnaps = false );

  // Constrain point to lie on a plane that is parallel to the
  // viewing plane and passes through the view's target point.
  BOOL32 ConstrainToTargetPlane(); 
  
  // Description:
  //   If enabled, the picked point is constrained to be on the 
  //   intersection of the plane and the virtual CPlane going through
  //   the plane origin.
  //   If the planes are parallel, the constraint works just like planar constraint.
  // Parameters:
  //   ON_Plane& - The plane used for the plane - virtual CPlane intersection.
  BOOL32 ConstrainToVirtualCPlaneIntersection( const ON_Plane& );

  // Description:
  //   Remove any explicit constraints added by calls to
  //   CRhinoGetPoint::Constraint() and enable the built-in
  //   constraint options.
  // See Also:
  //   CRhinoGetPoint::PermitConstraintOptions
  void ClearConstraints();

  void ClearLineConstraint();

  bool Constrained() const;

  //////////
  //
  // If you have lengthy computations in OnMouseMove() 
  // and/or DymanicPaint() overrides, then periodically call
  // InterruptMouseMove() to see if you should interrupt
  // your work because the mouse has moved again.
  //
  BOOL32 InterruptMouseMove() const; // tool for use in 

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Optional - override CRhinoGetPoint::OnMouseMove()
  //

  /*
  Description:
    If you want to be called every time the mouse moves, override
    this function.
  Parameters:
    vp - [in] active view
    nFlags - [in] standard windows OnMouseMove event flags
    point - [in] 3d world location ( May be uninitialized)
    view_wnd_point - [in] If the point is coming from a "mouse"
       pointing device, view_wnd_point is the CRhinoView window
       coordinates of mouse location.  If the point is coming
       from a 3d digitizer pointing device, view_wnd_point is
       NULL.
  Remarks:
    CRhinoGetPoint::OnMouseMove is called once per mouse move
    and is called BEFORE any calls to CRhinoGetPoint::OnDynamicDraw.
    If you are doing anything that takes a long time, periodically
    call InterruptMouseMove() to see if you should stop.
    If the view is such that the 2d screen point can't be mapped to 
    a 3d point, the 'point' argument will be ON_unset_point
  */
  virtual 
  void OnMouseMove( 
          CRhinoViewport& vp,
          UINT nFlags,
          const ON_3dPoint& point,
          const ON_2iPoint* view_wnd_point
          );

  /*
  Description:
    This virtual function CRhinoGetPoint::OnMouseDown is called 
    during CRhinoGetPoint::Get2dRectangle, RhinoGetPoint::Get2dLine,
    and CRhinoGetPoint::GetPoint(..,true) when the mouse down event
    for the initial point occurs. The default does nothing.  This
    function is not called during ordinary point getting because
    the mouse down event terminates an ordinary point get and 
    returns a CRhinoGet::point result.
  Parameters:
    vp - [in] active view
    nFlags - [in] standard windows OnMouseMove event flags
    point - [in] 3d world location
    view_wnd_point - [in] If the point is coming from a "mouse"
       pointing device, view_wnd_point is the CRhinoView window
       coordinates of mouse location.  If the point is coming
       from a 3d digitizer pointing device, view_wnd_point is
       NULL.
  */
  virtual 
  void OnMouseDown( 
          CRhinoViewport& vp,
          UINT nFlags,
          const ON_3dPoint& point,
          const ON_2iPoint* view_wnd_point
          );


  //////////////////////////////////////////////////////////////////
  //
  // STEP 5a: Optional - override CRhinoGetPoint::DynamicDraw()
  //                     if you want to dynamically draw things
  //                     as the mouse/digitizer moves.  Every
  //                     time the mouse moves, DynamicDraw()
  //                     will be called once per viewport.
  //                     The calls to DynamicDraw() happen AFTER
  //                     the call to OnMouseMove().
  //
  //   Use the CRhinoView::Draw* tools to draw things.
  //   Use Rhino::LayerColor(), etc., to get Rhino system colors.
  //
  //   If you are drawing anything that takes a long time, periodically
  //   call InterruptMouseMove() to see if you should stop.
  //
  //   If you override both OnMouseMove() and DynamicDraw(), then every
  //   time the mouse moves you will first get a single call to
  //   OnMouseMove() and then get a call to DynamicDraw() for each
  //   viewport.
  //
  //   If you override dynamic draw, you should call 
  //   CRhinoGetPoint::DynamicDraw() at the end of your override.
  //
  virtual 
  void DynamicDraw( 
          CRhinoDisplayPipeline&,
          const ON_3dPoint&  // current mouse/digitizer location
          );

  ON_DEPRECATED_MSG("override DynamicDraw(CRhinoDisplayPipeline&, const ON_3dPoint&) instead")
  virtual void DynamicDraw( HDC, CRhinoViewport&, const ON_3dPoint& );
  
  /*
  Description:
    Sets cursor that will be used when GetPoint() is called and
    a snap is not happening.
  Parameters:
    getpoint_cursor - [in]
  Returns:
    Previous get point cursor.
  Remarks:
    When CRhinoGetPoint::GetPoint is called, the cursor is changed
    to getpoint_cursor.  
    The default is RhinoApp().m_cross_hair_cursor.
  See Also:
    CRhinoGetPoint::CurrentCursor
    CRhinoGetPoint::GetPointCursor
    RhinoSetCursor 
  */
  HCURSOR SetGetPointCursor( HCURSOR getpoint_cursor );

  /*
  Description:
    Gets cursor that will be used when GetPoint() is called. This
    is either the get point cursor or a snap cursor.
  Returns:
    The current GetPoint cursor (may be a snap cursor or
    the get point cursor).
  Remarks:
    When CRhinoGetPoint::GetPoint is called, the cursor is changed
    to the cursor specified in CRhinoGetPoint::SetGetPointCursor.  
    If a snap happens during GetPoint, the cursor is changed to the
    appropriate snap cursor while the snap is active.
    The default get point cursor is RhinoApp().m_cross_hair_cursor.
  See Also:
    CRhinoGetPoint::SetGetPointCursor
    CRhinoGetPoint::GetPointCursor
    RhinoSetCursor
  */
  HCURSOR CurrentCursor() const;

  /*
  Description:
    Gets the default get point cursor.
  Returns:
    The default GetPoint cursor.
  Remarks:
    When CRhinoGetPoint::GetPoint is called, the cursor is changed
    to the cursor specified in CRhinoGetPoint::SetGetPointCursor.  
    If a snap happens during GetPoint, the cursor is changed to the
    appropriate snap cursor while the snap is active.
    The default get point cursor is RhinoApp().m_cross_hair_cursor.
  See Also:
    CRhinoGetPoint::SetGetPointCursor
    CRhinoGetPoint::CurrentCursor
    RhinoSetCursor
  */
  HCURSOR GetPointCursor() const;

  /*
  Description:
    By default, object snap cursors are enabled.
  Parameters:
    bEnableObjectSnapCursors - [in]
  Returns:
    True if object snap cursors (plus sign with "near", "end", etc.)
    are used when the point snaps to a object.  
  */
  void EnableObjectSnapCursors(bool bEnableObjectSnapCursors=true);

  /*
  Description:
    By default, OnCrv/OnSrf/OnMesh cursor is shown if osnap cursors are enabled and input is constrained to a curve, 
    surface, brep or a mesh, and the cursor is over the constrained object.
  Parameters:
  bUseOnObjectCursors - [in]
  */
  void EnableOnObjectCursors(bool bEnableOnObjectCursors = true);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 6: Call GetPoint() or Get2dPoint() to get a point.
  //
  //

#if defined(ON_RUNTIME_APPLE_IOS)
  // Make two parameter version explicit on mobile. On mobile the default behavior
  // it to complete a get point operation when the finger comes off the screen in
  // cases when the CRhinoGetPoint subclass is overriding DynamicDraw
  CRhinoGet::result GetPoint(CRhinoHistory* history = nullptr);
  CRhinoGet::result GetPoint(CRhinoHistory* history, bool bOnMouseUp);
#else
  /*
  Description:
    After setting up options and so on, call CRhinoGetPoint::GetPoint
    to get a 3d point.
  Parameters:
    history - [in] if not NULL, a record of the options and point
       getting is stored here.
    bOnMouseUp - [in] if false, the point is returned when the
                      left mouse button goes down.  If true,
                      the point is returned when the left mouse
                      button goes up.  If true, the virtual function

  Returns:
    The type of input that was specified.  If CRhinoGet::point is
    returned, then use CRhinoGetPoint::Point to get the value
    of the 3d point.
  */
  CRhinoGet::result GetPoint( 
           CRhinoHistory* history = nullptr,
           bool bOnMouseUp = false
           );
#endif
  /*
  Description:
    After setting up options and so on, call CRhinoGetPoint::Get2dPoint
    to get a 2d point in a view window. 
  Parameters:
    pView - [in] if pView is not NULL, then the 2d point will be in
                 this view.  Otherwise, the 2d point can be in any view.
    bOnMouseUp - [in] if true, the point will be returned on the mouse 
                      up event.
  Returns:
    The type of input that was specified.  If CRhinoGet::point2d is
    returned, then use CRhinoGetPoint::Point2d to get the value
    of the 2d point and CRhinoGetPoint::View() to get the view
    that contains the 2d point.
  */
  CRhinoGet::result Get2dPoint(
           CRhinoView* pView = NULL,
           bool bOnMouseUp = false
           );

  /*
  Description:
    After setting up options and so on, call CRhinoGetPoint::Get2dRectangle
    to get a 2d rectanble in a view window. 
  Parameters:
    pView - [in] if pView is not NULL, then the 2d rectangle will be in
                 this view.  Otherwise, the 2d rectangle can be in any view.
    corner - [in] if pView is not NULL and corner is not NULL, then
                 corner will be the location of the first corner.
    bCentered - [in] if true, the first point will be the center of the
                 rectangle.
    pen_style - [in] pen style used to draw dynamic rectangle
    second_prompt - [in] default automatically comes up with the
                         right string.  If you are very hard to
                         satisfy, you can supply your own.
  Returns:
    The type of input that was specified.  If CRhinoGet::rect2d is
    returned, then use CRhinoGetPoint::Rectangle2d to get the value
    of the 2d point and CRhinoGetPoint::View() to get the view
    that contains the 2d point.
  */
  CRhinoGet::result Get2dRectangle( 
              CRhinoView* pView = NULL, 
              const POINT* corner = NULL,
              BOOL32 bCentered = false,
              int pen_style = PS_SOLID,
              bool bClampToView = true,
              const wchar_t* second_prompt=NULL
              );


  /*
  Description:
    After setting up options and so on, call CRhinoGetPoint::Get2dLine
    to get a 2d line in a view window.
  Parameters:
    pView - [in] if pView is not NULL, then the 2d line will be in
                 this view.  Otherwise, the 2d line can be in any view.
    corner - [in] if pView is not NULL and corner is not NULL, then
                 corner will be the location of the start point
    bCentered - [in] if true, the first point will be the center of the
                 line.
    pen_style - [in] pen style used to draw dynamic line. If PS_NULL,
                     then no dynamic line is drawn
    bClampToView - [in] if true, then the endpoints are constrained
                        to be in the view.  If false, then the
                        endpoints can be outside of the view.
  Returns:
    The type of input that was specified.  If CRhinoGet::rect2d is
    returned, then use CRhinoGetPoint::Rectangle2d to get the value
    of the 2d point and CRhinoGetPoint::View() to get the view
    that contains the 2d point.
  */
  CRhinoGet::result Get2dLine(
              CRhinoView* pView = NULL, 
              const POINT* corner = NULL,
              BOOL32 bCentered = false,
              int pen_style = PS_SOLID,
              bool bClampToView = true
              );

  //////////////////////////////////////////////////////////////////
  //
  // STEP 7: Use result code returned by GetPoint() to determine what
  //         function to call to get the input.
  //
  //         result               function
  //           CRhinoGet::option    CRhinoGet::Option()
  //           CRhinoGet::number    CRhinoGet::Number()
  //           CRhinoGet::point     CRhinoPoint::Point()
  //
  ON_3dPoint Point()  const; // returns 3d point in world coordinates

  // Call this function to see if the point was on an object.  If
  // PointOnObject() returns true, then the object is returned
  // in the CRhinoObjRef parameter.  Use CRhinoObjRef::CurveParameter() or
  // CRhinoObjRef::SurfaceParameter() to get the parameter values for
  // the point.
  BOOL32 PointOnObject(CRhinoObjRef&) const;

  // Description:
  //   Use to determine if point was on a curve.
  //
  // Parameters:
  //   t - [out] if the point was on a curve, then the *t is
  //        the curve parameter for the point.  The point
  //        returned by CRhinoGetPoint::Point() is the same as curve->PointAt(*t).
  //
  // Returns:
  //   pointer to a curve or NULL if the point was not on a curve.
  const ON_Curve* PointOnCurve(double* t) const;

  // Description:
  //   Use to determine if point was on an edge curve of a brep.
  // Parameters:
  //   edge_parameter - [out] edge parameter of pick point
  //   pTrim - [out] the associated trim
  // Remarks:
  //   An edge can be a boundary or interior edge.  Boundary edges have a
  //   single trim.  Interior edges have multiple trims.  If you need
  //   information about the surface, then examine the trim.
  // Example:
  //   CRhinoGetPoint gp;
  //   gp.GetPoint();
  //   if ( gp.Result() == CRhinoGet::point )
  //   {
  //     double edge_t;
  //     double trim_t;
  //     const ON_BrepTrim* pTrim = NULL;
  //     const ON_BrepEdge* pEdge = gp.PointOnEdge( &edge_t, pTrim );
  //     if ( pEdge )
  //     {
  //       const ON_Surface* pSurface pEdge->Brep()->SurfaceOf(*pTrim);
  //     }
  //   }
  // Remarks:
  //   If you do not need trim or surface information, then use
  //   CRhinoGetPoint::PointOnCurve.
  // Returns:
  //   NULL or edge the point was on.
  const ON_BrepEdge* PointOnEdge(
            double* edge_parameter, 
            const ON_BrepTrim*& pTrim 
            ) const;

  // Description:
  //   Use to determine if point was on a surface.
  //
  // Parameters:
  //   u - [out]
  //   v - [out] if the point was on a surface, then the (*u,*v) are
  //        the surface parameters for the point.  The point
  //        returned by Point() is the same as surface->PointAt(*u,*v).
  //
  // Returns:
  //   pointer to a surface or NULL if the point was not on a surface.
  const ON_Surface* PointOnSurface(double* u, double* v) const;


  // Description:
  //   Use to determine if point was on a brep (face).
  //
  // Parameters:
  //   u - [out]
  //   v - [out] if the point was on a face, then the (*u,*v) are
  //        the face parameters for the point.
  //
  // Returns:
  //   pointer to a brep or NULL if the point was not on a brep.
  const ON_BrepFace* PointOnBrep( double* u, double* v) const;


  /*
  Returns:
    True if mouse moved during the call to GetPoint();
  */
  bool MouseMoved() const;

  /*
  Description:
    Used internally. Do not call. No support is available.
  */
  bool GetView3dPoint( UINT nPointingDevice,
                       CRhinoView& view, 
                       UINT_PTR nFlags, 
                       ON_3dPoint device_point,
                       const ON_Line& world_line,
                       ON_3dPoint& world_point
                       );

  /*
  Description:
    This setting turns off automatic redraw at the end of GetPoint.
    May be needed in some commands for flicker free feedback.
    When true the caller is responsible for cleaning up the screen
    after GetPoint. The default is false.
  Parameters:
    bNoRedrawOnExit - [in]
  */
  void EnableNoRedrawOnExit( bool bNoRedrawOnExit = true);


#if defined (ON_RUNTIME_APPLE)
  virtual class CRhinoGetPolylinePoint* GetPolylinePoint(void);
#endif

  /*
  Description:
    Returns the snap event if object snap was active when picking the point.
  */
  const CRhinoSnapEvent& SnapEvent() const;

  /*
  Description:
    Sets the base objref needed for object-object tan and perp osnaps.
  */
  void SetBaseObjRef( const CRhinoObjRef& ref);

  /*
  Description:
    Returns the base objref used for object-object tan and perp osnaps.
  */
  const CRhinoObjRef& BaseObjRef() const;

  bool DrawsShadedMeshes() const;
  void SetDrawsShadedMeshes(bool on);
  void SetDynamicDrawBounds(const ON_BoundingBox& bbox);
  const ON_BoundingBox& DynamicDrawBounds() const;

  /*
  Description:
    Returns true and optionally the number if m_bAcceptNumber is true and user is typing a number, but
    hasn't pressed Enter.
    Necessary for making OnMouseMove override pay attention to what is being typed at the command 
    prompt in real time, and also after call to GetPoint() if you want the typed number to affect the result
    even if the user picked a point.
  */
  bool NumberPreview(double* number = nullptr) const;

  /*
  Description:
    Sets the number preview. Pass ON_UNSET_VALUE to clear the preview.
  */
  void SetNumberPreview(double number);

protected:
  /*
  Parameters:
    elevator - [out] if the getter is in elevator mode, this
                     line defines the base and top of the elevator.
  Returns:
    True if point getter is currently in elevator mode.
  */
  bool InElevatorMode( ON_Line& elevator ) const;

protected:
  ON_Plane GetCS( bool bWorldCoordinates, bool bRelativeCoordinates ) override;

protected:
  // when a snap or constraint requires tracking line(s),
  // m_tracking_line_count >= 0.  If you override DynamicDraw,
  // then draw these lines using RhinoApp().AppSettings().TrackingColor()
  // The default tracking color is white.
  ON_SimpleArray<ON_Line> m_tracking_lines;

  // when a snap or constraint requires feedback line(s),
  // m_feedback_line_count >= 0.  If you override DynamicDraw,
  // then draw these lines using RhinoApp().AppSettings().FeedbackColor()
  // The default feedback color is black.
  ON_SimpleArray<ON_Line> m_feedback_lines;

  ON_3dPointArray m_construction_points;
  ON_3dPointArray m_point_osnap_points; 
  ON_3dPointArray m_int_osnap_points; 

private:
  HCURSOR m_getpoint_cursor; // cursor to use when a snap is not happening
  HCURSOR m_current_cursor; // current getpoint cursor (may be a snap cursor or m_getpoint_cursor)
  bool m_bUseOSnapCursors = true;
  bool m_bUseOnObjectCursors = true;

  bool m_bDrawsShadedMeshes = false;
  ON_BoundingBox m_dynamicdraw_bounds;

  // color used by default CRhinoGetPoint::DynamicDraw()
  // a value of 0x01020304 means use the current value of
  // RhinoApp().AppSettings().ActiveLayerColor()
  ON_Color m_dynamic_draw_color;

  // used by CRhinoGetPoint::DynamicDraw() to draw a line from 
  // the base point to the current point
  BOOL32        m_bDrawLine;
  BOOL32        m_bDrawLineStatusPane;
  ON_3dPoint  m_draw_line_point; // usually same as base point unless a From

  // m_base_point is set by SetBasePoint().
  // The location m_base_point is 
  // used as the reference point for ortho, from, and planar constraints.
  ON_3dPoint m_base_point;

  // if true, the distance from the base point to the
  // current point is printed on the status bar
  bool m_bShowBasePointDistanceInStatusPane = false;

  // used to constrain point to on a plane
  BOOL32 m_bOnPlane;
  ON_Plane m_plane;

  // used to constrain point to a plane and virtual CPlane intersection
  BOOL32 m_bOnPlaneVirtualCPlaneIntersection;

  // used to constrain point to on a line
  BOOL32 m_bOnLine;
  ON_Line m_line;

  // used to constrain point to on an arc
  BOOL32 m_bOnArc;
  ON_Arc m_arc;

  // used to constrain point to on a circle
  BOOL32 m_bOnCircle;
  ON_Circle m_circle;

  // used to constrain point to on a sphere
  BOOL32 m_bOnSphere;
  ON_Sphere m_sphere;

  // used to constrain point to on a cylinder
  BOOL32 m_bOnCylinder;
  ON_Cylinder m_cylinder;

  // used to constrain point to on a curve
  BOOL32 m_bOnCurve;
  ON_Curve* m_curve;
  class CRhCurveDisplay* m_curve_display;

  // used to constrain point to on a surface
  BOOL32 m_bOnSurface;
  ON_Surface* m_surface;
  class TL_NurbsSurface* m_nurbs_surface; // NURBS form of m_surface

  // used to constrain point to a brep
  BOOL32 m_bOnBrep;
  ON_Brep* m_pBrep;
  int m_brep_face_index; // When <0, constrain to whole brep. When >=0, constrain to individual face.

  // When input is constrained to a curve or a surface, 
  // setting this flag to true allows the picker to return points
  // that are not on the object the input is constrained to.
  // The flag is set in the calls to Constrain( ON_Curve&, bool) and 
  // Constrain( ON_Surface&, bool)
  bool m_bAllowPickingPointOffObject;

  // -- "From" snap --
  // If m_bPermitFromOption is true, the user may use the built-in
  // "From" option to set m_base_point during GetPoint
  bool m_bPermitFromOption; // default = true

  // If m_bPermitConstraintOptions is true, the user may use the built-in
  // linear, planar, curve, and surface constraint options.
  bool m_bPermitConstraintOptions; // default = true

  // used to constrain point to a mesh
  bool m_bOnMesh;

  // Uused to constrain point to be on the active construction
  // plane or a plane parallel to the construction plane that
  // passes through m_base_point (if m_base_point != ON_unset_point).
  // If 1 and RhinoApp().AppSettings().Planar() is false, then the 
  // point must be on the construction plane.  If 1 and 
  // RhinoApp().AppSettings().Planar() is true, then the 
  // base point shifted plane is used.  If 2, the base point shifted
  // plane is always used.
  // Modes 3 and 4 duplicate 1 and 2, but imply projecting osnaps to the
  // constraint plane.
  int m_OnConstructionPlane;

  // used to constrain point to be on the plane that passes
  // through the view target point and is parallel
  // to the view projection plane.
  BOOL32 m_bOnTargetPlane;

  // Modifies the input point to conform to any enabled constraints to geometry
  // such as OnLine, OnPlane, ...  Or to a special line constraint (m_tab_mode_line)
  // if "tab mode" is on. Tab mode means the direction to track is locked to a line
  // because the user pressed TAB.  Or to a fixed distance from the basepoint if 
  // m_distance_from_basepoint is set (because the user typed a number in GetPoint).
  bool ApplyConstraints( CRhinoView* view, ON_3dPoint& point );

  // -- "Ortho" snap --
  // When m_bPermitOrtho is true and RhinoApp().AppSettings().Ortho()
  // is true, the the point is constrained to be on a line that passes
  // throught m_base_point and is parallel to m_plane.xaxis or m_plane.yaxis.
  // (m_plane is set to be the active construction plane unless there
  // is another plane that has been set.)
  BOOL32 m_bPermitOrtho; // default = true

  // if m_bPermitOrtho is true and m_ortho_angle != ON_UNSET_VALUE,
  // then the user typed <a to start a run-time ortho-constraint.
  double m_ortho_angle;

  // used to constrain point to a mesh
  ON_Mesh* m_pMesh;

  // If m_distance_from_basepoint > 0, then the 
  // then the point is constrained to be m_distance_from_basepoint
  // from m_base_point.  If m_distance_from_basepoint==0.0, the
  // user is not permitted to set the value of m_distance_from_basepoint
  // by typing a number during GetPoint.
  double m_distance_from_basepoint; // default = ON_UNSET_VALUE

  // used to have getpoint try to snap to curves
  BOOL32 m_bSnapToCurves;

  // used to have getpoint decorate point on a curve
  int m_draw_perp_bar; // 0 = no, 1 = yes, 2 = with grips
  int m_draw_tangent_bar; // 0 = no, 1 = yes, 2 = with grips
  int m_draw_direction_arrow; // 0 = no, 1 = yes, -1 = reversed

  // elevator mode stuff
  int m_permit_elevator_mode; //0=no,1=from m_elevator_plane, 2=from cplane, 3=from m_plane
  int m_elevator_mode; //0=off,1=from m_elevator_plane, 2=from cplane
  int m_elevator_exit; //0=no, 1=exit if click happens in same view
  ON_3dPoint m_elevator_basepoint;
  ON_Plane m_elevator_plane;
  double m_elevator_height;

  // tab mode stuff  
  bool m_bPermitTabMode; // true if tabmode is available
  int m_tab_mode; // 0=off,1=on line, 2=on circle
  ON_Line m_tabmode_line;
  ON_Circle m_tabmode_circle;

  // redraw control
  bool m_bNoRedrawOnExit; // true if caller wants to take care of cleanup

  // mouse moving accounting
  bool m_gp_mouse_moved;
  bool m_gp_snap_check;
  POINT m_gp_screen_point0; // ::GetCursorPos() display x,y

  ////////////////////////////////////////////////////////////////////
  //
  // set when CRhinoGetPoint::GetPoint() returns CRhinoGet::point
  //
  // CRhinoGet::m_point = world point
  // CRhinoGet::m_view = view where point was picked
  ON_Line m_gp_line; // world line from near plane to far plane
  POINT m_gp_screen_point; // if source == 1, screen point (window coords)
  char m_gp_point_source; // 1 if point came from point pick,
                          // 2 if point came from digitizer,
                          // 3 if point came from other source
  BOOL32 m_bHaveObjRef; // true if point was on an object
  CRhinoObjRef m_objref; // object point was on
  CRhinoSnapEvent m_snap_event; // snap event if point was osnapped
  CRhinoObjRef m_base_objref; // base objref for object-object tan and perp osnap

private:
  ON_3dPoint m_point_preview; // 3d point currently being typed
  double m_number_preview; // number value currently being typed
  const CRhinoObject* m_do_not_snap_to_me;
  bool m_bPermitOsnap; // default = true; 
  bool m_bEnableOsnap; // default = true; for internal use, set and reset by one shot osnaps
  bool m_bEnableOneShotOsnapHiddenOptions; // default = true; 
  bool m_baseDynamicDrawCalled = false;
  unsigned char m_reserved1[5];

  CRhinoAppSettings::osnap_mode m_osnap_override;
  bool m_bOneTimeConstraint;
  
  // If m_complex_point_input is true, the getpoint operation used some kind of point input not
  // well supported by History and PointOnObject, like .x, .y, between, Intersection osnap, etc.
  // This is added so that putting this kind of thing in history records can be avoided
public:
  bool ComplexPointInput() const;
  void SetComplexPointInput(bool b);
private:
  bool m_complex_point_input = false;

  unsigned char m_reserved2[2];
  void AddBuiltInOptions( void* );
  void ProcessBuiltInOptions( void* );

public:
  // these functions are used by CRhinoView::OnMouseDown/Move/Up - pretend they don't exist
  int OnViewMouseDown( CRhinoView&, int button, UINT nFlags, const ON_2iPoint& point, CRhinoViewport& vp );
  int OnViewMouseMove( CRhinoView&, int button, UINT nFlags, const ON_2iPoint& point, CRhinoViewport& vp );
  int OnViewMouseUp( CRhinoView&, int button, UINT nFlags, const ON_2iPoint& point, CRhinoViewport& vp );
  bool GetPointOnMouseUp() const; // true if getting point on mouse up event

#if defined (ON_RUNTIME_APPLE)
public:
  // these functions are used to determine the state GetPoint constraints
  bool HasPersistentOnCurveConstraint();
  bool HasPersistentOnSurfaceConstraint();
  bool HasPersistentOnBrepConstraint();
  bool HasPersistentOnMeshConstraint();
  bool InOneShotConstraintCommand(CRhinoAppSettings::osnap_command);
  bool HasOneShotConstraint(CRhinoAppSettings::osnap_mode);
public:
  CRhinoAppSettings::osnap_command m_oneshot_command;
#endif
private:
  friend class CRhinoGetPointPrivate;
  class CRhinoGetPointPrivate* m_private = nullptr;


private:
  //These sealed versions of the virtual functions with the old declarations are here specifically to break
  //older plug-ins to ensure they don't miss a virtual function not being overridden.

  // OBSOLETE - use the nFlags version
  virtual void OnMouseMove(CRhinoViewport& vp,const ON_3dPoint& point,const class CPoint* view_wnd_point) final;

  // OBSOLETE - use the ON_2iPoint version
  virtual void OnMouseMove(CRhinoViewport& vp,UINT nFlags,const ON_3dPoint& point,const class CPoint* view_wnd_point) final;

  // OBSOLETE - use the ON_2iPoint version
  virtual void OnMouseDown(CRhinoViewport& vp,UINT nFlags,const ON_3dPoint& point, const class CPoint* view_wnd_point) final;
};

