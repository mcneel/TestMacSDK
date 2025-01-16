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

class RHINO_SDK_CLASS CRhinoGripDirections
{
public:
  void Unset();
  bool IsSet() const;

  ON_3dVector m_X;
  ON_3dVector m_Y;
  ON_3dVector m_Z;
};

class RHINO_SDK_CLASS CRhinoGripStatus
{
public:
  CRhinoGripStatus();
  ~CRhinoGripStatus();
  bool m_bVisible;
  bool m_bHighlighted;
  bool m_bLocked;
  bool m_bMoved;
  bool m_bCulled; // volitile and view dependent
  bool IsVisible() const;
  CRhinoGripDirections m_directions;
};

#if defined (ON_COMPILER_MSC)
// This stuff is here because of a limitation in the way Microsoft
// handles templates and DLLs.  See Microsoft's knowledge base 
// article ID Q168958 for details.
#pragma warning( push )
#pragma warning( disable : 4231 )
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_SimpleArray<CRhinoGripDirections>;
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_SimpleArray<CRhinoGripStatus>;
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_SimpleArray<CRhinoGripObject*>;
#pragma warning( pop )
#endif


class RHINO_SDK_CLASS CRhinoDrawGripsSettings
{
public:
  CRhinoDrawGripsSettings( CRhinoDisplayPipeline& dp );

  CRhinoDisplayPipeline&  m_dp;

  ///////////////////////////////////////////////////////////////
  // If true, then draw stuff that does not move when grips are
  // dragged, like the control polygon of the "original" curve.
  bool m_bDrawStaticStuff;

  ///////////////////////////////////////////////////////////////
  // If true, then draw stuff that moves when grips are dragged,
  // like the curve being bent by a dragged control point.
  bool m_bDrawDynamicStuff;

  ///////////////////////////////////////////////////////////////
  // m_cp_grip_style determines what kind of line is used to 
  // display things like control polygons.
  //   0 = no control points
  //   1 = default grip style
  int m_cp_grip_style;

  ///////////////////////////////////////////////////////////////
  // m_cp_line_style determines what kind of line is used to 
  // display things like control polygons.
  //   0 = no control polygon
  //   1 = solid control polygon
  //   2 = dotted control polygon
  int m_cp_line_style;

  ON_Color m_grip_color;
  ON_Color m_locked_grip_color;
  ON_Color m_selected_grip_color;

  ON_SimpleArray<CRhinoGripStatus> m_grip_status;

  /*
  Description:
    A helper function for drawing lines in control polygons.
  Parameters:
    L - [in]
      Line between two grips
    gs0 - [in]
      Grip status at start of line.
    gs1 - [in]
      Grip status at start of line.
    adjustDepthAndBiasModes - [in] optional
      Highlighted lines adjust the depth and bias modes to "always in front"
      and "neutral" and then restore the mode after the line is drawn. This
      will cause a flush of the line buffer which in most cases is fine. Set
      to false only if you want to explicitly set these modes outside of this
      function.
  */
  void DrawControlPolygonLine(
                const ON_Line& L,
                const class CRhinoGripStatus& gs0,
                const class CRhinoGripStatus& gs1,
                bool adjustDepthAndBiasModes = true
                );
  /*
  Description:
    Restores viewport settings to the values in the
    m_saved_vp fields.
  */
  void RestoreViewportSettings();

  // vp settings when CRhinoDrawGripsSettings was created.
  const ON_Color m_saved_vp_color;
  const EDepthMode  m_saved_vp_wire_depth;
  const int m_saved_vp_wire_zbias;
};

class RHINO_SDK_CLASS CRhinoObjectGrips
{
public:
  CRhinoObjectGrips( CRhinoGripObject::GRIP_TYPE grips_type );
  virtual ~CRhinoObjectGrips();

  ///////////////////////////////////////////////////////////////////////
  //
  // Begin custom grips interface
  //
#pragma region Custom grips interface

  // NOTE:
  //  Custom grips ids do not have to appear here. 
  //  They can be hidden in implementation files.
  //
  //  The ids here are for core Rhino grips, like edit points,
  //  and are provided so it is easy for 3rd party plug-ins
  //  to determine what type of core Rhino grips are enabled.

  // CustomCurveEditPointGripsId makes a grip at every curve Greville point.
  // The EditPtOn command applied to a SubD enables these grips.
  static const ON_UUID CustomCurveEditPointGripsId;

  // CustomSubDEditPointGripsId makes a grip for every subd surface point.
  // The EditPtOn command applied to a SubD enables these grips.
  static const ON_UUID CustomSubDEditPointGripsId;

  /*
  Parameters:
    grips - [in]
    custom_grips_id - [in]      
  Returns:
    True if grips are custom grips with the specified custom grips id.
  */
  static bool IsCustom(const class CRhinoObjectGrips* grips, ON_UUID custom_grips_id);

  /*
  Parameters:
    custom_grips_id - [in]
  Returns:
    True if this are custom grips with the specified custom grips id.
  */
  bool IsCustom( ON_UUID custom_grips_id) const;

#pragma endregion
  //
  // End custom grips interface
  //
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  //
  // Begin all grips virtual function interface
  // All types of grips (built-in and custom) override the following functions.
  //
#pragma region All grips virtual function interface

  /*
  Description:
    Resets location of all grips to original spots and cleans
    up stuff that was created by dynamic dragging.  This is
    required when dragging is canceled or in the Copy command
    when grips are "copied".  This function should not destroy
    the current grips and make new ones because the grips may
    be in a list someplace (like in the copy command).

    The default CRhinoObjectGrips::Reset() takes care of the 
    grip location issues.  The override should clean up dynamic
    workspace stuff and can call CRhinoObjectGrips::Reset() 
    to handle resetting grip locations.
  */
  virtual
  void Reset();

  /*
  Description:
    Just before Rhino turns off object grips, it calls this function.
    If you have modified any object display information, you must override
    this function and restore the display information to its original state.

  Remarks:
    This function is poorly named. It harkens back to a simpler time in the late 1990s.
  */
  virtual
  void ResetMeshes();

  /*
  Description:
    Just before Rhino draws an object with grips on, it calls UpdateMesh() 
    to update display information that will be used in the near future. 
    
    If "m_bNewLocation" is true, update your dynamic display information
    and then set m_bNewLocation to false. If "m_bNewLocation" is false, do nothing.

  Parameters:
    mesh_type_hint - [in]
      Ideally, design your override so it can ignore this parameter. 
      At best, treat the parameter as a hint for what will occur in the near future.

      ON::mesh_type::analysis_mesh
        The grips parent object is currently in some sort of visual analsis mode (false color curvature, draft, edges, ...)
        and at some point in the near future the grips parent object and any dynamic display information will
        be shown.
      ON::mesh_type::render_mesh
        At some point in the near future the grips parent object and any dynamic display information will
        be used in a "shaded" viewport setting. It's common for there to be several viewports in both
        shaded and wireframe contexts.
      ON::mesh_type::any_type
        At some point in the near future the grips parent object and any dynamic display information will
        be used in a "wireframe" viewport setting. It's common for there to be several viewports in both
        shaded and wireframe contexts.

  Remarks:
    This function is poorly named and the parameter is not all that useful.
    Both harken back to a simpler time in the late 1990s.
  */
  virtual
  void UpdateMesh( ON::mesh_type mesh_type_hint);

  /*
  Description:
    If the grips control just one object, then override
    NewObject().  When NewObject() is called, return an
    new object calculated from the current grip locations.  
    This happens  once at the end of a grip drag.
  Returns:
    NULL or a new object whose shape is calculated from the current
    grip locations.  This object should not be added to the document.
  Remarks:
    This function is not used for lights.
  */
  virtual
  CRhinoObject* NewObject();

  /*
  Description:
    Rhino calls DeleteGrips() when some of the object's grips
    are deleted.  If the object supports grip deletion, like
    meshes, then DeleteGrips() should create a new object
    by deleting the indicated grips.
  Returns:
    Pointer to the new object if successful.  Rhino will
    take care of adding this object to the document.
  */
  virtual
  CRhinoObject* DeleteGrips( 
       ON_SimpleArray<int>& deleted_grips 
       );

  /*
  Description:
    Rhino calls NewLightObject() when it wants a new light that
    is calculated from the current grip locations.  This happens 
    once at the end of a grip drag.
  Parameters:
    bCopy - [in]
      If true, the light object the grips are on is left in place
      and a new light is added to the light table.  If false, the
      light object is replaced.
  Returns:
    Pointer to the new light if successful.  The new light is
    in the light table.
  */
  virtual
  CRhinoLight* NewLightObject( bool bCopy );

  /*
  Description:
  Updates the widget geometry based on e.g. grip dragging.
  Notes:
  Does not do drawing.
  See also:
  CRhinoObjectGrips::Draw()
  */
  virtual
  void UpdateWidgetGeometry();

  /*
  Description:
    The default draws the grips.  Override if you need to
    draw dynamic stuff and then call CRhinoObjectGrips::Draw()
    to draw the grips themselves.
  Parameters:
    dgs - [in]
  */
  virtual
  void Draw( CRhinoDrawGripsSettings& dgs );

  /*
  Description:
    If an object has grips turned on, SelectGrips() can be
    used to change the selection state of the object's grips.
  Parameters:
    bSelect - [in] true to select, false to unselect
    bSynchHighlight - [in] if true, highlight state is 
             synchronized with selection state.
    bPersistentSelect - [in] if true, grips are persitently selected.
  See Also:
    CRhinoObject::HasGripsSelected
  */
  virtual 
  void SelectGrips(        
         bool bSelect = true,
         bool bSynchHighlight = true,
         bool bPersistentSelect = true
         );

  /*
  Description:
    Query the selection state of an object's grips.
  Parameters:
    bCheckAllGrips - [in] If false, HasGripsSelected() will
       return true if any grips are selected.
       If true, HasGripsSelected() will
       return true if all grips are selected.    
  Returns:
    True if all grips are selected or at least one grip
    is selected and bCheckAllGrips is false.
  */
  virtual
  bool HasGripsSelected( bool bCheckAllGrips = false ) const;


  /*
  Description:
    Get neighbors.
  Parameters:
    grip_index - [in] index of grip where the search begins
    dr - [in
       1 = next grip in the first parameter direction
      -1 = prev grip in the first parameter direction
    ds - [in]
       1 = next grip in the second parameter direction
      -1 = prev grip in the second parameter direction
    dt - [in]
       1 = next grip in the third parameter direction
      -1 = prev grip in the third parameter direction
    bWrap - [in]
      If true and object is "closed", the search will wrap.      
  Returns:
    Pointer to the desired neighbor or NULL if there is no neighbor
  */
  virtual
  CRhinoGripObject* NeighborGrip(
          int grip_index, 
          int dr, 
          int ds, 
          int dt,
          bool bWrap 
          ) const;

  /*
  Description:
    Get all adjacent grips.
  Parameters:
    grip_index - [in] index of grip where the search begins
    neighbor_grips - [out] neighbors are appended to this array
    bWrap - [in]
      If true and object is "closed", the search will wrap.      
  Returns:
    number of grips appended to neighbor_grips[] array.
  */
  virtual
  int GetNeighborGrips(
        int grip_index,
        ON_SimpleArray<CRhinoGripObject*>& neighbor_grips,
        bool bWrap
        ) const;


  /*
  Description:
    If the grips are control points of a NURBS curve,
    then this gets the index of the grip that controls
    the i-th cv.
  Parameters:
    cv_i - [in]
  Returns:
    A grip controlling a NURBS curve CV or NULL.
  */
  virtual
  CRhinoGripObject* NurbsCurveGrip( int cv_i ) const;

  /*
  Description:
    If the grips control a NURBS curve, this returns
    a pointer to that curve.  You can look at but you
    must NEVER change this curve.
  Returns:
    A pointer to a NURBS curve or NULL.
  */
  virtual
  const ON_NurbsCurve* NurbsCurve() const;

  /*
  Description:
    If the grips are control points of a NURBS surface,
    then this gets the index of the grip that controls
    the (i,j)-th cv.
  Parameters:
    cv_i - [in]
    cv_j - [in]
  Returns:
    A grip controlling a NURBS surface CV or NULL.
  */
  virtual
  CRhinoGripObject* NurbsSurfaceGrip( int cv_i, int cv_j ) const;

  /*
  Description:
    If the grips control a NURBS surface, this returns
    a pointer to that surface.  You can look at but you
    must NEVER change this surface.
  Returns:
    A pointer to a NURBS surface or NULL.
  */
  virtual
  const ON_NurbsSurface* NurbsSurface() const;


  /*
  Description:
    If the grips are control points of a NURBS cage,
    then this gets the index of the grip that controls
    the (i,j,k)-th cv.
  Parameters:
    cv_i - [in]
    cv_j - [in]
    cv_k - [in]
  Returns:
    A grip controlling a NURBS cage CV or NULL.
  */
  virtual
  CRhinoGripObject* NurbsCageGrip( int cv_i, int cv_j, int cv_k ) const;

  /*
  Description:
    If the grips control a NURBS cage, this returns
    a pointer to that cage.  You can look at but you
    must NEVER change this cage.
  Returns:
    A pointer to a NURBS cage or NULL.
  */
  virtual
  const ON_NurbsCage* NurbsCage() const;

  /*
  Description:
    If the grips control a morph control, this returns
    a pointer to that morph controle.  You can look at but you
    must NEVER change this cage.
  Returns:
    A pointer to a morph control or NULL.
  */
  virtual
  const ON_MorphControl* MorphControl() const;

  /*
  Description:
    Appends the status of the grips to the grip_status[] array.
  Parameters:
    grip_status - [out]
  Returns:
    Number of visible grips.
  */
  virtual
  int GetGripStatus( 
          ON_SimpleArray<CRhinoGripStatus>& grip_status 
          ) const;

#pragma endregion 
  //
  // End all grips virtual function interface
  //
  ///////////////////////////////////////////////////////////////////////


  /*
  Description:
    Set grip status.  This is useful in commands that
    replace and copy objects.
  Parameters:
    grip_status - [in]
  Returns:
    True if status was restored.
  */
  bool SetGripStatus(
          const ON_SimpleArray<CRhinoGripStatus>& grip_status 
          );

  /*
  Description:
    Uses the m_captive_object_id[] list to get pointers to
    captive objects that are in the document.
  Parameters:
    captives -[out]
      Captives are appened to this array.
    bWithGripsOn - [in]
      If true, only captives with grips on are appended
  Returns:
    Number of objects appended to captives[].
  */
  int GetCaptives( 
          ON_SimpleArray<CRhinoObject*>& captives,
          bool bWithGripsOn = true
          ) const;


  // List of all the grips
  ON_SimpleArray<CRhinoGripObject*> m_grip_list;

  // Optional list of grip directions
  ON_SimpleArray<CRhinoGripDirections> m_grip_directions;

  // Optional list of captive object ids.
  // Use when these object grips deform more than
  // just the owner of these gripss.
  ON_UuidList m_captive_object_id;

  /*
  Returns:
    True if grips are currently begin dragged.
  */
#if defined (ON_RUNTIME_WIN) || defined(ON_RUNTIME_LINUX)
  static bool Dragging();
#endif
#if defined (ON_RUNTIME_APPLE)
  static bool Dragging(CRhinoDoc* pDoc);
#endif

  // true if some of the grips have been moved.
  // CRhinoGripObject::NewLocation() sets m_bNewLocation=true 
  // Derived classes can set m_bNewLocation to false after 
  // updating temporary display information.
  bool m_bNewLocation;

  // If m_bGripsMoved is true if some of the grips have 
  // ever been moved CRhinoGripObject::NewLocation() sets 
  // m_bGripsMoved=true  Nothting else should ever change
  // the value of m_bGripsMoved.
  bool m_bGripsMoved;

  // CRhinoObject owner of the grips
  CRhinoObject* m_owner_object;

  // Type of grips
  const CRhinoGripObject::GRIP_TYPE m__grips_type;

  // This id is for distinguishing between different types
  // of custom grips.
  ON_UUID m_grips_id;

private:
  // prohibit use
  CRhinoObjectGrips();
  CRhinoObjectGrips(const CRhinoObjectGrips&);
  CRhinoObjectGrips& operator=(const CRhinoObjectGrips& src);
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

/*
Description:
  This class is useful when grip information needs to be duplicated
  on objects are copied or replaced.
*/
class RHINO_SDK_CLASS CRhinoCopyGripsHelper
{
public:
  CRhinoCopyGripsHelper();

  /*
  Description:
    Call this function to save the current grip information.
  Parameters:
    object - [in]
      Object currently in the document with its grips on.
  */
  void SaveGripStatus( 
    const CRhinoObject* object
    );

  /*
  Description:
    Call this function turn on the grips on the copy or
    replacement object.
  Parameters:
    object - [in]
      This object should be in the document and not have
      its grips on.
  */
  void RestoreGripStatus(
    CRhinoObject* object
    );

  /////////////////////////////////////////////////////////////////
  //
  // Implementation
  //

  // This pointer is for expert user's to keep track of objects.
  // Neither SaveGripStatus() nor RestoreGripStatus() sets or
  // references this pointer.
  CRhinoObject* m_saved_object;

  CRhinoGripObject::GRIP_TYPE      m_grip_type;
  const CRhinoGripsEnabler*        m_grips_enabler;
  int                              m_grip_count;
  ON_SimpleArray<CRhinoGripStatus> m_grip_status;

  // If the old and new object are NURBS curves/surface/cages with 
  // the same cv counts, m_cv_grip_index[] is used to transmit
  // grips status when there are fewer grips than control points.
  int m_cv_count[3]; // number of cvs
  // The grip status for cv (i,j,k) is in m_grip_status[n], where
  // n = m_cv_map[ i + m_cv_count[0]*(j+m_cv_count[1]*k)]
  ON_SimpleArray<int> m_cv_grip_index; 

  ON_UuidList m_captive_object_id;

  void GetNurbsCurveMap( const CRhinoObject* object );
  void GetNurbsSurfaceMap( const CRhinoObject* object );
  void GetNurbsCageMap( const CRhinoObject* object );
  void RestoreNurbsCurveGripStatus( CRhinoObject* object );
  void RestoreNurbsSurfaceGripStatus( CRhinoObject* object );
  void RestoreNurbsCageGripStatus( CRhinoObject* object );
};

/*
Description:
  The grips positions are used to calculate updated objects and
  these objects are added to the Rhino document.
Parameters:
  old_object - [in]
    Object in document with grips currently on.
  bCopyObject - [in]
    If false, the updated object(s) replace the originals in the
    document.  Otherwise, the new objects are simply added to
    the document
  group_remap - [in]
    If bCopyObject is true and group_remap is not NULL,
    the copies are put in new groups.  If bCopyObject is
    false, then group_remap is ignored.
Returns:
  A pointer to the new object if successful.
*/
RHINO_SDK_FUNCTION
CRhinoObject* RhinoUpdateGripOwner( 
          CRhinoObject* old_object, 
          bool bCopyObject,
          ON_2dexMap* group_remap
          );

/*
Description:
  In some situations, a bunch of objects are copied, new
  new groups need to be created.  This function handles the
  details.
Parameters:
  new_object - [in/out]
    New object with group attributes of the old object.
  group_map - [in/out]
    A map from old group indices to new group indices.
*/
RHINO_SDK_FUNCTION
void RhinoUpdateObjectGroups( 
          CRhinoObject* new_object, 
          ON_2dexMap& group_map 
          );

/*
Description:
  In some situations, a bunch of objects are copied, new
  new groups need to be created.  This function handles the
  details.
Parameters:
  attributes - [in/out]
    Attributes of the new object with group indices copied from the old object.
  group_map - [in/out]
    A map from old group indices to new group indices.
*/
RHINO_SDK_FUNCTION
void RhinoUpdateObjectGroups(
  CRhinoObjectAttributes& attributes,
  ON_2dexMap& group_map
  );
