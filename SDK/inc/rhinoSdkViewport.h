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

class RHINO_SDK_CLASS CRhinoViewport
{
private:
  // Use CRhinoViewport.RuntimeSerialNumber() to get this value.
  // Use CRhinoViewport::FromRuntimeSerialNumber() so search for
  // a view with a specific serial number.
  //
  // The m_rhino_viewport_sn field is private to make it impossible
  // for the value to be accidentally passed as a pointer or 
  // reference, which would defeat the purpose of using 
  // runtime serial numbers to prevent dereferencing deleted objects.
  //
  // m_rhino_viewport_sn must be the first field in CRhinoViewport
  // so it is initialized first in constructors.
  const unsigned int m_rhino_viewport_sn;

public:
  /*
  Parameters:
    rhino_viewport_sn - [out]
      CRhinoViewport runtime serial number
  Returns:
    A pointer to the CRhinoViewport class with the specified runtime serial number
    or null if none exists.
  */
  static CRhinoViewport* FromRuntimeSerialNumber(
    unsigned int rhino_viewport_sn
    );

  static unsigned int RuntimeSerialNumber(
    const CRhinoViewport* viewport
    );

  /*
  Returns:
    The serial number of the oldest viewport that exists
    or 0 if no viewports exist.  This will be the
    smallest serial number of any viewport that 
    currently exits.
  */
  static unsigned int HeadRuntimeSerialNumber();

  /*
  Returns:
    The serial number of the newest viewport that exists
    or 0 if no viewports exist. This will be the
    largest serial number of any viewport that 
    currently exits.
  Remarks:
    Other viewports may have been created and deleted
    after this viewport.
  */
  static unsigned int TailRuntimeSerialNumber();
  
  /*
  Description:
    In most cases, it is easier, faster and more robust to
    use a CRhinoViewportIterator to go through the list of viewports.
    In the rare case when you really do want a list of 
    all existing viewport serial numbers, use this function.
  Parameters:
    rhino_doc_sn - [in]
       0: iteration ignores the document a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with a specific document.
    rhino_view_sn - [in]
       0: iteration ignores the viewport a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with the specified viewport.
    rhino_viewport_sn_list - [out]
      list of rhino viewport serial numbers in increasing order.
  Returns:
    Number of rhino viewport serial numbers in rhino_viewport_sn_list[].
  See Also:
    CRhinoViewportIterator
  */
  static unsigned int GetViewportList(
    unsigned int rhino_doc_sn,
    unsigned int rhino_view_sn,
    ON_SimpleArray<unsigned int>& rhino_viewport_sn_list
    );

  /*
  Description:
    In most cases, it is easier, faster and more robust to
    use a CRhinoViewportIterator to go through the list of viewports.
    In the rare case when you really do want a list of 
    existing viewport serial numbers, use this function.
  Parameters:
    rhino_doc_sn - [in]
       0: iteration ignores the document a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with a specific document.
    rhino_view_sn - [in]
       0: iteration ignores the viewport a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with the specified viewport.
    rhino_viewport_sn0 - [in]
      Serial numbers <= rhino_viewport_sn0 will not be added
      to rhino_viewport_sn_list[].
    rhino_viewport_sn_list_capacity - [in]
      maximum number of elements in rhino_viewport_sn_list[] array.
    rhino_viewport_sn_list - [out]
      list of rhino viewport serial numbers > rhino_viewport_sn0.
  Returns:
    Number of rhino viewports set in rhino_viewport_sn_list[].
  See Also:
    CRhinoViewportIterator
  */
  static unsigned int GetViewportList(
    unsigned int rhino_doc_sn,
    unsigned int rhino_view_sn,
    unsigned int rhino_viewport_sn0,
    size_t rhino_viewport_sn_list_capacity,
    unsigned int* rhino_viewport_sn_list
    );

  /*
  Parameters:
    rhino_doc_sn - [in]
       0: iteration ignores the document a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with a specific document.
    rhino_view_sn - [in]
       0: iteration ignores the viewport a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with the specified viewport.
    rhino_viewport_id - [in]
  */
  static CRhinoViewport* FromViewportId(
    ON_UUID rhino_viewport_id
    );

  static ON::view_type ViewTypeFromActiveSpace(ON::active_space active_space);
  static ON::active_space ActiveSpaceFromViewType(ON::view_type view_type);

  /*
  Returns:
    True if rhino_viewport is null or points to an existing CRhinoViewport.
  */
  static bool IsValidViewportPointer( 
    const void* viewport
    );

  static bool IsValidViewportReference( 
    const CRhinoViewport& viewport
    );

  unsigned int RuntimeSerialNumber() const;

  /*
  Returns:
    m_v.m_vp.ViewportId().    
  */
  ON_UUID ViewportId() const;

public:
  // Override operator new/delete to make sure allocate/deallocate
  // occurs in same memory manager
  // See comments at the top of opennurbs_object.cpp for details.
  // new/delete
  void* operator new(size_t);
  void  operator delete(void*);

  // array new/delete
  void* operator new[] (size_t);
  void  operator delete[] (void*);

  // in place new/delete
  void* operator new(size_t,void*);
  void  operator delete(void*,void*);

public:
  CRhinoViewport();
  ~CRhinoViewport();

  /*
  Description:
    Gets the parent view, if there is one.
  Returns:
    A pointer to the parent view or NULL.
  Remarks:
    Every CRhinoView has an associated CRhinoViewport that
    does all the 3d display work.  Those associated viewports
    return the CRhinoView as their parent view.  However,
    CRhinoViewports are used in other image creating contexts
    that do not have a parent CRhinoView.  If you call
    ParentView, you MUST check for NULL return values.
  */
  CRhinoView* ParentView() const;

  /*
  Returns:
    The serial number of the view's document, if one exists.
    Zero if there is no view or no document associated with
    this viewport.
  */
  unsigned int DocumentRuntimeSerialNumber() const;

  // Description:
  //   The value of change counter is incremented
  //   every time the view projection or construction plane
  //   changes.  The user can the mouse and nestable view 
  //   manipulation commands to change a view at any time.
  //   The value value of change counter can be used to 
  //   detect these changes in code that is sensitive to
  //   the view projection.
  // Returns:
  //   current value of change counter. (m_vp_iteration)
  // Example:
  //   change_counter0 = vp.ChangeCounter().
  //   ...
  //   if ( change_counter0 != vp.ChangeCounter() )
  //   {
  //     view projection changed
  //   }
  unsigned int ChangeCounter() const;

  //////////
  // A viewport's display mode can be determined by using the uuid of
  // one of the predefined or user defined display attributes,
  // (see CDisplayPipelineAttributes)...
  bool SetDisplayMode(const ON_UUID&);

  //////////
  // Determine if this viewport's display attributes support "shading".
  // This assumption here is that if shading is supported, then objects
  // will be shaded.
  bool DisplayModeIsShaded(void) const;

  /*
  Description:
    By default, shaded viewports can either use smooth shading.
    If flat shading is enabled, then mesh objects are flat
    shaded.
  Parameters:
    bFlatShade - [in] If true, all mesh objects in this viewport
       will be flat shaded.  
  See Also:
    CRhinoViewport::SetDisplayMode
    CRhinoViewport::EnableSelectedShade
    CRhinoViewport::FlatShade
  */
  void EnableFlatShade( bool bFlatShade = true );

  /*
  Description:
    By default, all surface and mesh objects are shaded in shaded 
    viewports.  If  bSelectedShade is true, then only selected
    mesh and solid objects are shaded.
  Parameters:
    bSelectedShade - [in] If true, only selected mesh and surface
       objects are shaded in shaded.
  See Also:
    CRhinoViewport::SetDisplayMode
    CRhinoViewport::EnableFlatShade
    CRhinoViewport::SelectedShade
  */
  void EnableSelectedShade( bool bSelectedShade = true );

  /*
  Returns:
    True if all mesh objects in this viewport will be flat shaded.    
  See Also:
    CRhinoViewport::EnableFlatShade
  */
  bool FlatShade() const;

  /*
  Returns:
    True if only selected objects are shaded.
  See Also:
    CRhinoViewport::EnableSelectedShade
  */
  bool SelectedShade() const;

  /*
  Description:
    Retrieves the exact contents of this viewport's entire z-buffer. Meaning,
    what you see is what you get...If you want to change the output contents of the 
    z-buffer, (ie. forced shading) then you should construct your own CRhinoZBuffer  
    object using this viewport, make the appropirate CRhinoZBuffer method calls, and
    then "Capture()" the z-buffer (see CRhinoZBuffer for details).
  Parameters:
    CRhinoZBuffer& : output parameter that will contain all captured z-buffer info.
  Returns:
    true:   If z-buffer was successfully captured...
    false:  If no z-buffer was captured (or some other failure occurred).
  
  Examples:
  
      CRhinoZBuffer   zbuffer;
      
      // Test contents of entire zbuffer...
      if ( pView->Viewport().GetZBuffer( zbuffer ) )
      
        for (int y = 0; y < zbuffer.Height(); y++)
          for (int x = 0; x < zbuffer.Width(); x++)
          {
            float   z = zbuffer.ZValue( x, y );
            
            if ( (z > 0.0) && (z < 1.0) )
            {
              // Then point (x,y) 'hit' something...
            }
          }
        
      // Test contents of a rectangular area within the z-buffer and convert
      // the hit points to 3D world coordinates...
      CRhinoZBuffer               zbuffer;
      CRect                       rect = some rectangular portion of the viewport...
      ON_SimpleArray<ON_3dPoint>  points;
      
      if ( pView->Viewport().GetZBuffer( zbuffer ) )
        for (int y = rect.top; y < rect.bottom; y++)
          for (int x = rect.left; x < rect.right; x++)
          {
            float   z = zbuffer.ZValue( x, y );
            
            if ( (z > 0.0) && (z < 1.0) )
            {
              points.Append( zbuffer.WorldPoint( x, y ) );
            }
          }
  */
  bool GetZBuffer(class CRhinoZBuffer&) const;
  
  //Description:
  // returns true if some portion world coordinate bounding box is
  // potentially visible in the viewing frustum.
  bool IsVisible( const ON_BoundingBox& ) const;

  //Description:
  // returns true if the world coordinate point is visible in the viewing frustum.
  bool IsVisible( const ON_3dPoint& ) const;


  ///////////////////////////////////////////////////////////////////
  //
  // Drawing controls
  //
  void SetScreenSize(int width, int height, bool forceRegen=false);

  // set optimal clipping planes to view objects in a world coordinate 3d bounding box
  void SetClippingPlanes(const ON_BoundingBox&);

  // viewport properties
  bool SetName( const wchar_t* );
  ON_wString Name() const;

  /*
  Description:
    Get viewport projection information.
  Returns:
    viewport projection information as an ON_Viewport.
  See Also:
    CRhinoViewport::SetVP
  */
  const ON_Viewport& VP() const;

  /*
  Description:
    Set viewport camera projection.
  Parameters:
    camera_location - [in] new target location
    bUpdateTargetLocation - [in] if true, the target
        location is changed so that the vector from the camera
        location to the target is parallel to the camera direction
        vector.  
        If false, the target location is not changed.
        See the remarks section of CRhinoViewport::SetTarget
        for important details.
    bAddToViewStack - [in] if true, the new viewport is
        added to view stack so UndoView will restore the previous one.
        If false, the viewport is not added to view stack, which is useful if
        this is called from dynamic view manipulation.
  Remarks:
    See the remarks section of CRhinoViewport::SetTarget for
    important details.
  See Also:
    CRhinoViewport::Target
    CRhinoViewport::SetTarget
    CRhinoViewport::SetTargetAndCameraLocation
    CRhinoViewport::SetCameraLocation
    CRhinoViewport::SetCameraDirection
    CRhinoViewport::SetVP
    CRhinoViewport::SetView
  */
  bool SetVP( const ON_Viewport&, BOOL32 bUpdateTargetLocation, BOOL32 bAddToViewStack = true );

  /*
  Description:
    Sets the wallpaper bitmap.
  Parameters:
    wallpaper - [in]
  Returns:
    True if successful.
  */
  bool SetWallpaperImage( 
         const ON_3dmWallpaperImage& wallpaper
         );

  /*
  Description:
    Sets the background bitmap.
  Parameters:
    traceimage - [in]
  Returns:
    True if successful.
  Remarks:
    THIS FUNCTION IS DEPRECATED AND WILL BE REMOVED IN THE FUTURE. 
    If you require similar functionality, use RhinoCreatePictureFrame().
    See rhinoSdkObject.h for details.
  */
  bool SetTraceImage( 
         const ON_3dmViewTraceImage& traceimage
         );

  /*
  Description:
    Verifies the existence of a wallpaper.
  Parameters:
    None.
  Returns:
    True if a wallpaper exists.
    False otherwise
  */
  bool IsWallpaperImage();

  /*
  Description:
    Verifies the existence of background bitmap.
  Parameters:
    None.
  Returns:
    True if a background bitmap exists.
    False otherwise
  Remarks:
    THIS FUNCTION IS DEPRECATED AND WILL BE REMOVED IN THE FUTURE.
    If you require similar functionality, use RhinoCreatePictureFrame().
    See rhinoSdkObject.h for details.
  */
  bool IsTraceImage();

  /*
  Description:
    Get complete information about the viewport projection, target 
    location, name, display mode, window location in the Rhino
    mainframe, construction plane, background bitmap and wallpaper 
    bitmap.
  Returns:
    view information as an ON_3dmView.
  See Also:
    CRhinoViewport::SetView
  */
  const ON_3dmView& View() const;


  /*
  Description:
    Expert user function to set viewport projection, target 
    location, name, display mode, window location in the Rhino
    mainframe, construction plane, background bitmap and wallpaper 
    bitmap.
  Parameters:
    view - [in]
  Remarks:
    This function is primarily used to save and restore view settings.
    If you want to modify specific viewport settings, use the functions
    listed in the see also section.
  See Also:
    CRhinoViewport::Target
    CRhinoViewport::SetTarget
    CRhinoViewport::SetTargetAndCameraLocation
    CRhinoViewport::SetCameraLocation
    CRhinoViewport::SetCameraDirection
    CRhinoViewport::SetVP
    CRhinoViewport::SetView
  */
  void SetView( const ON_3dmView& view );

  /*
  Description:
    Expert user function to set viewport projection, target 
    location, name, display mode, window location in the Rhino
    mainframe, construction plane and wallpaper bitmap using the same
	code used by the NamedView command - including support for animation
	fixed-aspect ratio and so on.
  Parameters:
    view - [in] The view to set.
	pDoc - [in] The document is only used to retrieve information about other affected objects and data, including clipping planes.  Can be nullptr.
  */
  bool RestoreNamedView(
	  const ON_3dmView& view,
	  CRhinoDoc* pDoc = nullptr,
	  bool bAspectRatio = false,
	  bool bAnimate = false,
	  bool bConstantSpeed = false,
	  int frames = 100,
	  double units_per_frame = 1.0,
	  int delay = 10
	  );

  // viewport target point
  ON_3dPoint Target() const;

  /*
  Description:
    Set viewport target point.  By default the camera location
    is translated so that the camera direction vector is parallel
    to the vector from the camera location to the target location.
  Parameters:
    target_location - [in] new target location
    bUpdateCameraLocation - [in] if true, the camera location
       is translated so that the camera direction vector is parallel
       to the vector from the camera location to the target location.  
       If false, the camera location is not changed.
       See the remarks section for important details.
  Remarks:
    In general, Rhino users expect to have the camera direction
    vector and the vector from the camera location to the target
    location to be parallel.
    If you use the CRhinoViewport functions listed in the see also
    section to set the camera location, camera direction, and
    target point, then the relationship between these three
    points and vectors is automatically maintained.  If you
    directly manipulate the camera by using the CRhinoViewport::SetVP 
    member function, then you should carefully set the target
    by calling SetTarget() with bUpdateCameraLocation=false.
  See Also:
    CRhinoViewport::Target
    CRhinoViewport::SetTarget
    CRhinoViewport::SetTargetAndCameraLocation
    CRhinoViewport::SetCameraLocation
    CRhinoViewport::SetCameraDirection
    CRhinoViewport::SetVP
    CRhinoViewport::SetView
  */
  void SetTarget(
         ON_3dPoint target_location,
         BOOL32 bUpdateCameraLocation = true
         );

  /*
  Description:
    Set viewport camera location and target location.  By default
    the camera direction vector is changed so that it is parallel 
    to the vector from the camera location to the target location.
  Parameters:
    target_location - [in] new target location
    camera_location - [in] new camera location
    bUpdateCameraDirection - [in] if true, the camera direction
        vector is changed so that it is parallel to the vector 
        from the camera location to the target location.
        If false, the camera direction vector is not changed.
        See the remarks section of CRhinoViewport::SetTarget
        for important details.
   bAddToViewStack - [in] if true, the new viewport is
        added to view stack so UndoView will restore the previous one.
        If false, the viewport is not added to view stack, which is useful if
        this is called from dynamic view manipulation.
  Remarks:
    See the remarks section of CRhinoViewport::SetTarget for
    important details.
  See Also:
    CRhinoViewport::Target
    CRhinoViewport::SetTarget
    CRhinoViewport::SetTargetAndCameraLocation
    CRhinoViewport::SetCameraLocation
    CRhinoViewport::SetCameraDirection
    CRhinoViewport::SetVP
    CRhinoViewport::SetView
  */
  void SetTargetAndCameraLocation(
         ON_3dPoint target_location,
         ON_3dPoint camera_location,
         BOOL32 bUpdateCameraDirection = true,
         BOOL32 bAddToViewStack = true
         );

  /*
  Description:
    Set viewport camera location.  By default the target
    location is changed so that the vector from the camera
    location to the target is parallel to the camera direction
    vector.
  Parameters:
    camera_location - [in] new target location
    bUpdateTarget - [in] if true, the target
        location is changed so that the vector from the camera
        location to the target is parallel to the camera direction
        vector.  
        If false, the target location is not changed.
        See the remarks section of CRhinoViewport::SetTarget
        for important details.
  Remarks:
    See the remarks section of CRhinoViewport::SetTarget for
    important details.
  See Also:
    CRhinoViewport::Target
    CRhinoViewport::SetTarget
    CRhinoViewport::SetTargetAndCameraLocation
    CRhinoViewport::SetCameraLocation
    CRhinoViewport::SetCameraDirection
    CRhinoViewport::SetVP
    CRhinoViewport::SetView
  */
  void SetCameraLocation(
         ON_3dPoint camera_location,
         BOOL32 bUpdateTargetLocation = true
         );

  /*
  Description:
    Set viewport camera direction.  By default the target
    location is changed so that the vector from the camera
    location to the target is parallel to the camera direction.
  Parameters:
    camera_direction - [in] new camera direction
    bUpdateTarget - [in] if true, the target
      location is changed so that the vector from the camera
      location to the target is parallel to the camera direction.
      If false, the target location is not changed.
      See the remarks section of CRhinoViewport::SetTarget for
      important details.
  Remarks:
    See the remarks section of CRhinoViewport::SetTarget for
    important details.
  See Also:
    CRhinoViewport::Target
    CRhinoViewport::SetTarget
    CRhinoViewport::SetTargetAndCameraLocation
    CRhinoViewport::SetCameraLocation
    CRhinoViewport::SetCameraDirection
    CRhinoViewport::SetVP
    CRhinoViewport::SetView
  */
  void SetCameraDirection(
         ON_3dVector camera_direction,
         BOOL32 bUpdateTargetLocation = true
         );

  // Returns:
  //   Viewport construction plane
  const ON_3dmConstructionPlane& ConstructionPlane() const;

  // Description:
  //   Sets the construction plane to cplane.
  // Parameters:
  //   cplane - [in]
  // See Also:
  //   CRhinoViewport::PushConstructionPlane, 
  //   CRhinoViewport::PopConstructionPlane
  //   CRhinoViewport::ConstructionPlane
  void SetConstructionPlane( const ON_3dmConstructionPlane& cplane );

  // Description:
  //   Pushes the current construction plane on the viewport's
  //   construction plane stack and sets the construction plane
  //   to cplane.
  // Parameters:
  //   cplane - [in]
  // Returns:
  //   true if a construction plane was popped.
  // See Also:
  //   CRhinoViewport::PopConstructionPlane, 
  //   CRhinoViewport::SetConstructionPlane
  //   CRhinoViewport::ConstructionPlane
  void PushConstructionPlane( const ON_3dmConstructionPlane& cplane );

  // Description:
  //   Sets the construction plane to the plane that was
  //   active before the last call to PushConstructionPlane.
  // Returns:
  //   true if a construction plane was popped.
  // See Also:
  //   CRhinoViewport::PushConstructionPlane, 
  //   CRhinoViewport::SetConstructionPlane
  //   CRhinoViewport::ConstructionPlane
  bool PopConstructionPlane();

  // Description:
  //   Sets the construction plane to the plane that was
  //   active before the last call to PrevConstructionPlane.
  // Returns:
  //   true if a construction plane was popped.
  // See Also:
  //   CRhinoViewport::PushConstructionPlane, 
  //   CRhinoViewport::SetConstructionPlane
  //   CRhinoViewport::ConstructionPlane
  bool NextConstructionPlane();

  // Description:
  //   Sets the construction plane to the plane that was
  //   active before the last call to NextConstructionPlane
  //   or SetConstructionPlane.
  // Returns:
  //   true if a construction plane was popped.
  // See Also:
  //   CRhinoViewport::PushConstructionPlane, 
  //   CRhinoViewport::SetConstructionPlane
  //   CRhinoViewport::ConstructionPlane
  bool PrevConstructionPlane();

  // controls display of construction plane grid
  bool ShowConstructionGrid() const;
  void SetShowConstructionGrid(BOOL32);

  // controls display of construction plane axes
  bool ShowConstructionAxes() const;
  void SetShowConstructionAxes(BOOL32);

  // controls display of world axes icon
  bool ShowWorldAxes() const;
  void SetShowWorldAxes(BOOL32);

  // viewports are either x-up, y-up, or z-up
  bool IsXUp() const;
  bool IsYUp() const;
  bool IsZUp() const;

  // Description:
  //   Calculate elevator mode point
  // Parameters:
  //     world_line - [in] line from near to far clipping plane that
  //         projects to mouse point.
  //     elevator_basepoint - [in]
  //     elevator_axis - [in] unit vector
  //     bGridSnap - [in] true if grid snap is enabled.  When in
  //        doubt, use value of RhinoApp().AppSettings().GridSnap().
  //     grid_snap_spacing - [in] grid snap distance.  When in
  //        doubt, use value of ConstructionPlane().m_snap_spacing
  //     elevator_height -[out] elevator height returned here.
  // Returns:
  //   true if point is successfully calculated.  The 3d location
  //   is elevator_basepoint + elevator_height*elevator_axis.
  bool GetElevatorHeight( 
    const ON_Line& world_line,
    const ON_3dPoint& elevator_basepoint, 
    const ON_3dVector& elevator_axis,
    BOOL32 bGridSnap,
    double grid_snap_spacing,
    double* elevator_height
    ) const;

  //////////
  // Default views
  bool SetToPlanView( 
          const ON_3dPoint&,  // plane origin
          const ON_3dVector&, // plane xaxis
          const ON_3dVector&, // plane yaxis
          BOOL32 = false        // bSetConstructionPlane
          );

  // Description:
  //   Set view to parallel top view projection.
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the view plane.
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToTopView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );

  // Description:
  //   Set view to parallel projection with world Y 
  //   pointing down and world X pointing to the right.
  // Parameters:
  //   sViewName - [in] if not NULL, the view name is set
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the view plane.
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToBottomView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );

  // Description:
  //   Set view to parallel projection with world Z 
  //   pointing up and world Y pointing to the left.
  // Parameters:
  //   sViewName - [in] if not NULL, the view name is set
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the view plane.
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToLeftView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );

  // Description:
  //   Set view to parallel projection with world Z 
  //   pointing up and world Y pointing to the right.
  // Parameters:
  //   sViewName - [in] if not NULL, the view name is set
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the view plane.
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToRightView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );

  // Description:
  //   Set view to parallel projection with world Z 
  //   pointing up and world X pointing to the right.
  // Parameters:
  //   sViewName - [in] if not NULL, the view name is set
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the view plane.
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToFrontView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );

  // Description:
  //   Set view to parallel projection with world Z 
  //   pointing up and world X pointing to the left.
  // Parameters:
  //   sViewName - [in] if not NULL, the view name is set
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the view plane.
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToBackView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );

  // Description:
  //   Set view to bird's eye perspective projection.
  // Parameters:
  //   sViewName - [in] if not NULL, the view name is set
  // Parameters:
  //   sViewName - [in] if not NULL, then view name is also set
  //   bUpdateConstructionPlane - [in] if true, then the
  //       construction plane is set to the world XY plane
  // Returns:
  //   true if successful
  // See Also
  //  CRhinoViewport::SetConstructionPlane, CRhinoViewport::SetName,
  //  CRhinoViewport::IsXUp, CRhinoViewport::IsYUp, CRhinoViewport::IsZUp
  bool SetToPerspectiveView(
    const wchar_t* sViewName = nullptr,
    BOOL32 bUpdateConstructionPlane = true
    );


  /*
  Description:
    Set view to the Rhino default two point perspective projection.
  Parameters:
    sViewName - [in] if not NULL, the view name is set
    bUpdateConstructionPlane - [in]
      Set construction plane to be parallel to ground plane.
  Returns:
    true if successful
  */
  bool SetToTwoPointPerspectiveView(
    const wchar_t* sViewName,
    bool bUpdateConstructionPlane = true
    );

  // Description:
  //   Appends the current view projection and target to the
  //   viewport's view stack.
  void PushViewProjection();

  // Description:
  //   Sets the view projection and target to the settings
  //   at the top of the view stack and removes those settings
  //   from the view stack.
  bool PopViewProjection();

  // Description:
  //   Sets the view projection and target to the settings that
  //   were active before the last call to PrevView.
  // Returns:
  //   true if the view stack was popped.
  bool NextViewProjection();

  // Description:
  //   Sets the view projection and target to the settings that
  //   were active before the last call to NextViewProjection.
  // Returns:
  //   true if the view stack was popped.
  bool PrevViewProjection();


  /*
  Description:
    Clears saved view projections and cplanes.
  Parameters:
    bClearProjections - [in] if true, then saved view projections
                             are cleared.
    bClearCPlanes - [in] if true, then saved construction planes
                         are cleared.
  Remarks:
    This function should be used only in special circumstances,
    like when a new file is read.  Calling this function destroys
    the information needed in commands likeUndoView and 
    CPlaneNext/CPlanePrevious.
  */
  void ClearUndoInformation( 
    bool bClearProjections = true, 
    bool bClearCPlanes = true 
    );

  //////////
  // returns true if construction plane z axis is parallel
  // to camera direction.
  bool IsPlanView() const;

  //////////
  // returns true if the view is very similar to an existing named view in the document
  bool IsNamedView(bool& modified);

  /*
  Description:
    Dolly the camera (change its location) so that the 
    bbox is centered in the viewport and fills the 
    central region of the viewport.  The "target" point
    is updated to be the center of the bounding box.
  Parameters:
    bbox - [in]
    cs - [in] coordinates system of the bounding box.
      NOTE WELL:
        If cs is anything besides ON::world_cs, the
        the coordinates are with respect to the current
        projection returned by this->VP().  Note
        that the call to DollyExtents will change the 
        projection so that any non-world coordinate
        bbox will not be valid after the function 
        returns.  If this is confusing, then restrict
        yourself to using world coordinate bounding boxes
        and cs = ON::world_cs.
  Remarks:
    The Rhino "ZoomExtents" command uses DollyExtents
    to calculate the view projection.  Technically,
    "zoom" would leave the camera location fixed and
    modify the camera angle.
  */
  bool DollyExtents( 
    ON_BoundingBox bbox,
    ON::coordinate_system cs
    );

  // mouse interaction tools
  
  /*
  Description:
    Rotate view around target.
  */
  bool MouseRotateView( const ON_2iPoint& mouse0, const ON_2iPoint& mouse1 );

  /*
  Description:
    Rotate view around camera location.
  */
  bool MouseRotateCamera( const ON_2iPoint& mouse0, const ON_2iPoint& mouse1 );

  bool MouseLateralDolly( const ON_2iPoint& mouse0, const ON_2iVector& mouse1 );
  bool MouseInOutDolly( const ON_2iPoint& mouse0, const ON_2iVector& mouse1 );
  bool MouseMagnify( const ON_2iPoint& mouse0, const ON_2iPoint& mouse1 );
  bool MouseTilt( const ON_2iPoint& mouse0, const ON_2iPoint& mouse1 );
  bool MouseAdjustLensLength( const ON_2iPoint& mouse0, const ON_2iPoint& mouse1, bool bMoveTarget = false);
  bool MouseDollyZoom( const ON_2iPoint& mouse0, const ON_2iPoint& mouse1);

  ON_3dVector MouseTrackballVector( 
    int, int // mousex, mousey
    ) const;

  // keyboard arrow key interaction tools
  bool LeftRightRotate( double ); // left < 0 < right
  bool DownUpRotate( double );    // down < 0 < up
  bool LeftRightDolly( double );  // left < 0 < right
  bool DownUpDolly( double );     // down < 0 < up
  bool InOutDolly( double );     // out < 0 < in
  bool LateralDolly( double x, double y );    // left x < 0 < right x, down y < 0 < up y
  bool Magnify( 
          double, // magnification factor
          int,    // 0 performs a "dolly" magnification by moving the 
                  //   camera towards/away from the target so that the
                  //   amount of the screen subtended by an object
                  //   changes.
                  // 1 performs a "zoom" magnification by adjusting the
                  //   "lense" angle           
           const ON_2iPoint* = nullptr // optional fixed screen point
          );


  /*
  Description:
    Set the model transformation that is applied to geometry before
    it is drawn.
  Parameters:
    model_xform - [in]
  Remarks:
    The default model transformation is the identity.  The
    model transformation is intended to be used for dynamic
    drawing of objects.  The camera and projection transformations
    are handled in the m_v settings.
  See Also:
    CRhinoViewport::SetDisplayXform
    CRhinoViewport::GetModelXform
  */
  void SetModelXform( const ON_Xform& model_xform );

  /*
  Description:
    Get the model transformation that is applied to geometry before
    it is drawn.
  Parameters:
    model_xform - [out]
  Remarks:
    The default model transformation is the identity.  The
    model transformation is intended to be used for dynamic
    drawing of objects.  The camera and projection transformations
    are handled in the m_v settings.
  See Also:
    CRhinoViewport::SetModelXform
  */
  void GetModelXform( ON_Xform& model_xform ) const;

  /*
  Description:
    Set the display transformation that is applied to the projected
    geometry immediately before it is drawn.  
  Parameters:
    display_xform - [in]
  Remarks:
    The default display transformation is the identity.  The
    display transformation is intended to be used in printing
    applications.  The camera and projection transformations
    are handled in the m_v settings.
  See Also:
    CRhinoViewport::SetModelXform
    CRhinoViewport::GetDisplayXform
  */
  ON_DEPRECATED void SetDisplayXform( const ON_Xform& display_xform );

  /*
  Description:
    Get the display transformation that is applied to the projected
    geometry immediately before it is drawn.  
  Parameters:
    display_xform - [out]
  Remarks:
    The default display transformation is the identity.  The
    display transformation is intended to be used in printing
    applications.  The camera and projection transformations
    are handled in the m_v settings.
  See Also:
    CRhinoViewport::SetDisplayXform
  */
  ON_DEPRECATED void GetDisplayXform( ON_Xform& display_xform ) const;

  /*
  Description:
    Set the display transformation that is applied to marked objects.
  Parameters:
    mark_value - [in] if not zero, then objects with this mark value
            are displayed using the marked_object_xform.
    marked_object_xform - [in]
  Remarks:
    The default marked object transformation is the identity.
    The marked object transformation is intended to be used when
    inserting models.
  See Also:
    CRhinoViewport::SetModelXform
    CRhinoViewport::GetMarkedObjectXform
  */
  void SetMarkedObjectXform( 
          int mark_value,
          const ON_Xform& marked_object_xform 
          );

  /*
  Description:
    Get the display transformation that is applied to marked objects.
  Parameters:
    mark_value - [out]
    marked_object_xform - [out]
  See Also:
    CRhinoViewport::SetMarkedObjectXform
  */
  void GetMarkedObjectXform( 
        int* mark_value, 
        ON_Xform& marked_object_xform 
        ) const;

  // picking

  //////////
  // GetPickXform takes a rectangle in screen coordinates and returns 
  // a transformation that maps the 3d frustum defined by the rectangle
  // to a -1/+1 clipping coordinate box.
  bool GetPickXform( 
         int, int, // screen coordinates of a mouse click
         ON_Xform& 
         ) const;
  bool GetPickXform( 
    const ON_4iRect&, // screen coordinates of a rectangle defining the picking frustum
    ON_Xform& 
    ) const;
  bool SetClippingRegionTransformation(
    int, int, // screen coordinates of a mouse click
    class ON_ClippingRegion& clip_region
    ) const;
  bool SetClippingRegionTransformation(
    const ON_4iRect&, // screen coordinates of a rectangle defining the picking frustum
    class ON_ClippingRegion& clip_region
    ) const;

  CRhinoDisplayPipeline*  DisplayPipeline(void) const;
  CRhinoDisplayPipeline*  AttachPipeline(CRhinoDisplayPipeline*);


  //Description:
  //  Convert a point in parent CRhinoView client window coordinates to the ON_Viewport screen port
  //  client coordinates. The screen port of a CRhinoViewport may not match the client area of
  //  the parent CRhinoView. This occurs in cases when the CRhinoViewport is a nested child viewport
  //Parameters:
  //  pt [in/out]: point in client coordinates of parent CRhinoView window as input. This is
  //               converted to the screen port client coordinates of the ON_Viewport as output
  //Returns:
  //  true if the point is inside of the CRhinoViewport's screen port rectangle
  bool ClientToScreenPort(ON_2iPoint& pt) const;

  // Description:
  //  Return true if m_v.m_view_type == ON::model_view_type.
  bool IsModelView() const;

  // Description:
  //   Determines if this viewport is the main viewport for a page view. Page views
  //   should be able to be panned and zoomed, but not rotated or changed to a
  //   different camera view vector. This is used by functions that attempt to set the
  //   viewport's projection. If the projection is not compatible with a page view and
  //   IsPageViewMainViewport() == true, then the setting of the projection is not allowed
  // Return:
  //   true if thie viewport is the main viewport for a CRhinoPageView
  bool IsPageViewMainViewport() const;

  // Description:
  //   Determines if this viewport is should be able to be panned and zoomed, but not rotated or changed to a
  //   different camera view vector. This is used by functions that attempt to set the
  //   viewport's projection. If the projection is not compatible with a page view / UV Editor view and
  //   IsMainViewportTopViewOnly() == true, then the setting of the projection is not allowed
  // Return:
  //   true if thie viewport is the main viewport for a CRhinoPageView
  bool IsMainViewportTopViewOnly() const;

  // Description:
  //   Similar to an operator= except this will not modify the viewport id unless told to.
  //   Items not copied are: m_draw_step, m_vp_iteration, m_xform_iteration, m_dp
  //                         m_cplane_stack, m_cplane_stack_index
  // Parameters:
  //   src [in]: CRhinoViewport to copy information from
  //   copy_id [in]: If true, this viewport's id will be changed to match that of src. This is
  //                 typically NOT something that you want to do. This set to true for operations
  //                 like printing in order to work with a temporary CRhinoViewport that is an
  //                 exact duplicate of the src viewport
  void CopyFrom(const CRhinoViewport& src, bool copy_id = false);
  void CopyFrom(const CRhinoViewport& src, bool copy_id, bool copy_parent_view);

  // Description:
  //   Determines if printing or "print display" is active for this viewport
  // Parameters:
  //   thickness_scale [out] - optional. Scale applied to an object's plot thickness for
  //                           displaying on the screen or printing
  // Returns:
  //   true if printing or print display is active for this viewport
  bool IsThickDisplayActive(double* thickness_scale = nullptr) const;

  const class CRhTraceImage* GetTraceImage(void)     const;
  const class CRhWallpaper*  GetWallpaperImage(void) const;

  int ScreenWidth()  const;
  int ScreenHeight() const;
  int Iteration()    const { return (int)m_vp_iteration;  }

public:
  /*
  Description:
    Update the transform cache.  If m_xform_iteration != m_vp_iteration
    or bForceUpdate is true, the cache is updated.
  Returns:
    true if the transformations are set.
  */
  bool UpdateTransforms( BOOL32 bForceUpdate = false );

public:
  // pretend these are private and you'll be happier.
  ON_3dmView m_v; // viewport settings
  ON_ClassArray<ON_3dmView> m_view_stack;
  int m_view_stack_index = -1; // -1 or index of view in use

private:
  class CRhWallpaper* m_wallpaper = nullptr;
  class CRhTraceImage* m_traceimage = nullptr;

  // special shaded display settings
  bool m_bFlatShade = false;     // applies to mesh objects in shaded mode
  bool m_bSelectedShade = false; // applies to mesh, surface, and polysrf objects in shaded mode

  // if m_marked_object_xform is not the identity, then marked
  // objects will be drawn with this xform applied.  This is 
  // used in commands like "Insert".
  int m_marked_object_mark_value = 0;
  ON_Xform m_marked_object_xform; // default = identity;

  ON_Xform m_model_xform;   // default = identity

  unsigned int m__parent_view_sn = 0;  // parent CRhinoView

  unsigned long m_vp_iteration = 1; // incremented every time the view projection changes (never 0)
  int           m_clip_z = 1;       // true to z-clip wires and points

  ON_ClassArray<ON_3dmConstructionPlane> m_cplane_stack;
  int m_cplane_stack_index = -1; // -1 or index of cplane in use

  // if m_xform_iteration != m_vp_iteration, transforms are not up to date
  unsigned long m_xform_iteration = 0; 

  // world to clip transformation (includes model transformation)
  ON_Xform m_w2c;

  CRhinoDisplayPipeline* m_dp = nullptr;

  mutable class CRhViewportPrivate* m_private = nullptr;
private:
  //// no implementation on purpose ////
  CRhinoViewport& operator=(const CRhinoViewport&) = delete;
  CRhinoViewport(const CRhinoViewport&);

  friend class CRhinoView;
  friend class CRhinoDetailViewObject;
  friend class CRhViewportPrivate;
public:
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::SetObjectColor")
  ON_Color SetDrawColor(COLORREF color);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::ObjectColor")
  ON_Color DrawColor() const;
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::InterruptDrawing")
  bool InterruptDrawing() const;
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawConstructionPlane")
  void DrawConstructionPlane(
    const ON_3dmConstructionPlane& cplane,
    BOOL32 bShowConstructionGrid,
    BOOL32 bShowConstructionAxes,
    BOOL32 bShowConstructionZAxis,
    COLORREF thin_line_color = ON_UNSET_COLOR,
    COLORREF thick_line_color = ON_UNSET_COLOR,
    COLORREF grid_x_axis_color = ON_UNSET_COLOR,
    COLORREF grid_y_axis_color = ON_UNSET_COLOR
  );
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawPoint")
  void DrawPoint(const ON_3dPoint&, const ON_3dVector* = nullptr);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawLine")
  void DrawLine(const ON_3dPoint&, const ON_3dPoint&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawDottedLine")
  void DrawDottedLine(const ON_3dPoint&, const ON_3dPoint&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawArc")
  void DrawArc(const ON_Arc&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawCircle")
  void DrawCircle(const ON_Circle&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawPoints")
  void DrawPointCloud(int point_count, const ON_3dPoint* points, const ON_3dPoint& origin);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawSphere")
  void DrawSphere(const ON_Sphere& sphere);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawTorus")
  void DrawTorus(const ON_Torus& torus);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawCylinder")
  void DrawCylinder(const ON_Cylinder& cylinder);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawCone")
  void DrawCone(const ON_Cone& cone);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawBox")
  void DrawBox(const ON_3dPoint* box_corners);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawLight")
  void DrawLight(const ON_Light& light, COLORREF wireframe_color);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawCurve")
  void DrawCurve(const ON_Curve& curve);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawNurbsCurve")
  void DrawNurbsCurve(const ON_NurbsCurve& nurbs_curve);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawWireframeMesh")
  void DrawWireframeMesh(const ON_Mesh*, BOOL32 bCullCW);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawShadedMesh")
  void DrawShadedMesh(const ON_Mesh*, bool bCullCW, bool bFlatShade, bool bHighlighted);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawShadedMesh")
  void DrawRenderPreviewMesh(const ON_Mesh*, const CRhinoMaterial& render_material, BOOL32 bCullCW, BOOL32 bHighlighted);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawShadedMesh")
  void DrawFalseColorMesh(const ON_Mesh*, bool bCullCW, COLORREF edge_color = ON_UNSET_COLOR);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawPoint")
  void DrawActivePoint(const ON_3dPoint&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawBox")
  void DrawBoundingBox(const ON_BoundingBox&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawBezier")
  void DrawBezier(const ON_BezierCurve&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawSurface")
  void DrawSurface(const ON_Surface&, int display_density = 1);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawNurbsSurface")
  void DrawNurbsSurface(const ON_NurbsSurface&, int display_density = 1);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawBezierSurface")
  void DrawBezierSurface(const ON_BezierSurface&, int display_density = 1);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawNurbsCage")
  void DrawNurbsCage(const ON_NurbsCage&, int display_density = 1);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawBezierCage")
  void DrawBezierCage(const ON_BezierSurface&, int display_density = 1);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawBrep")
  void DrawBrep(const ON_Brep&, int display_density = 1);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawExtrusion")
  void DrawExtrusion(const ON_Extrusion&);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawObject")
  void DrawRhinoObject(const CRhinoObject*, ON_Xform xform, ON_Color color = ON_UNSET_COLOR);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawDot")
  void DrawDot(int screen_x, int screen_y, const wchar_t* text = NULL, COLORREF dot_color = RGB(0, 0, 0), COLORREF text_color = ON_UNSET_COLOR);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawDot")
  void DrawDot(ON_3dPoint point, const wchar_t* text = NULL, COLORREF dot_color = RGB(0, 0, 0), COLORREF text_color = ON_UNSET_COLOR);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawString")
  void DrawString(const wchar_t* string, int slength, const ON_3dPoint& point,
    int bMiddle = false, int rotation = 0, int height = 12, const wchar_t* fontface = L"Arial" );
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawTriangle")
  void DrawTriangle(const ON_3dPoint corners[3]);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawTriangle")
  void DrawTriangle(const ON_3dPoint& p0, const ON_3dPoint& p1, const ON_3dPoint& p2);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawDirectionArrow")
  void DrawDirectionArrow(const ON_3dPoint&, const ON_3dVector&, ON_3dPoint* head_point = NULL);
  ON_DEPRECATED_MSG("use CRhinoDisplayPipeline::DrawTangentBar")
  void DrawTangentBar(const ON_3dPoint&, const ON_3dVector&, ON_3dPoint* start_point = NULL, ON_3dPoint* end_point = NULL);
};

class RHINO_SDK_CLASS CRhinoViewportIterator
{
  /*
  Example:
    Iteratate all viewports:
    unsigned int rhino_doc_sn = 0 or CRhinoDoc.RuntimeSerialNumber();
    unsigned int rhino_view_sn = 0 or CRhinoView.RuntimeSerialNumber();
    CRhinoDocIterator doc_it(rhino_doc_sn,rhino_view_sn);
    for ( CRhinoViewport* rhino_viewport = viewport_it.First();
          0 != rhino_viewport; 
          rhino_viewport = viewport_it.Next() 
        )
    {
      ...
    }
  */
public:

  /*
  Returns:
     0: iteration ignores the document a viewportport is associated with.
    >0: iteration is restricted to viewportports associated with a specific document.
  */
  unsigned int DocRuntimeSerialNumber() const;

  /*
  Returns:
     0: iteration ignores the viewport a viewportport is associated with.
    >0: iteration is restricted to viewportports associated with the specified viewport.
  */
  unsigned int ViewRuntimeSerialNumber() const;


  /*
  Parameters:
    rhino_doc_sn - [in]
       0: iteration ignores the document a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with a specific document.
    rhino_view_sn - [in]
       0: iteration ignores the viewport a viewportport is associated with.
      >0: iteration is restricted to viewportports associated with the specified viewport.
  */
  CRhinoViewportIterator(
    unsigned int rhino_doc_sn,
    unsigned int rhino_view_sn
    );

  /*
  Returns:
    The first viewport that exists.
  Remarks:
    You may call First() multiple times on the same iterator.
  */
  class CRhinoViewport* First();

  /*
  Returns:
    The next viewport that exists.
  Remarks:
    - Once Next() returns null, it will continue to return null
      until First() is called.
    - If First() has not be called, the first call to Next()
      returns the first viewport in Rhino.  This is done
      so that people can write while loops like
      CRhinoViewportIterator it(rhino_doc_sn);
      while ( CRhinoViewport* rhino_viewport = it.Next() )
      {
         ...
      }
  */
  class CRhinoViewport* Next();

  /*
  Returns:
    The current value of the iterator.
  Remarks:
    - If First() or Next() has not been called, then Current()
      will return null.
    - If the viewport returned by First() or Next() has been deleted
      since the call to First() or Next(), then CurrentViewport() will
      return a null pointer.
  */
  class CRhinoViewport* CurrentViewport() const;
  unsigned int CurrentViewportRuntimeSerialNumber() const;

private:
  const unsigned int m_rhino_doc_sn;  // 0 or identifies a specific CRhinoDoc
  const unsigned int m_rhino_view_sn; // 0 or identifies a specific CRhinoView
  unsigned int m_current_viewport_sn;
  // NOTE:
  //  The iterator can handle an arbitrary number of serial numbers.
  //  The buffer is refilled as needed.
  unsigned int m_sn_index;  
  unsigned int m_sn_count;
  unsigned int m_sn_buffer[59];
};
