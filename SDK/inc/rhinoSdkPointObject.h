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

#include "rhinoSdkObject.h"

class RHINO_SDK_CLASS CRhinoPointObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoPointObject);

public:
  void MemoryRelocate() override;

  // Returns ON::point_object
  ON::object_type ObjectType() const override;

public:
  CRhinoPointObject();                              // assigns a new object UUID
  CRhinoPointObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoPointObject();
  CRhinoPointObject( const CRhinoPointObject& );          // copies source object UUID
  CRhinoPointObject& operator=(const CRhinoPointObject&); // copies source object UUID
  CRhinoPointObject& operator=(const ON_3dPoint&); // leave attributes unchanged.

  // copies point and moves user data from argument to m_point.
  void SetPoint(ON_Point& point );
  void SetPoint(const ON_3dPoint& point);

  const ON_Point& Point() const;

	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  //OBSOLETE - Use IsVisibleInViewport
  //virtual CRhinoObject::IsVisibleInView override
  //bool IsVisibleInView( CRhinoView& ) const;
  virtual bool IsVisibleInViewport( const CRhinoViewport& vp ) const;

  // virtual CRhinoObject::DrawV6 override
  void DrawV6( class CRhinoObjectDrawContext* draw_context ) const RHINO_NOEXCEPT override;

  void Draw(CRhinoDisplayPipeline&) const override;

  bool PickFilter(const CRhinoGetObject&, bool) const override;

  int Pick(const CRhinoPickContext&, CRhinoObjRefArray&) const override;

  bool SnapTo(const CRhinoSnapContext& snapContext, CRhinoSnapEvent& snapEvent) const override;

  // Returns "point" or "points"
  const wchar_t* ShortDescription(bool bPlural) const override;

  /*
  Description:
    virtual CRhinoObject::GetFrame override
  Parameters:
    plane - [out] frame for curve
  Returns:
    true if successful
  */
  bool GetFrame(ON_Plane& plane) const override;

protected:
  ON_Point m_point;
};


class RHINO_SDK_CLASS CRhinoPointCloudObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoPointCloudObject);

public:
  void MemoryRelocate() override;

  // Returns ON::pointset_object
  ON::object_type ObjectType() const override;

public:
  CRhinoPointCloudObject();                              // assigns a new object UUID
  CRhinoPointCloudObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoPointCloudObject();
  
  CRhinoPointCloudObject( const CRhinoPointCloudObject& );          // copies source object UUID
  CRhinoPointCloudObject& operator=(const CRhinoPointCloudObject&); // copies source object UUID

  CRhinoPointCloudObject( const ON_SimpleArray<ON_3dPoint>& );
  CRhinoPointCloudObject& operator=(const ON_SimpleArray<ON_3dPoint>&); // leave attributes unchanged.
  
  CRhinoPointCloudObject( const ON_SimpleArray<ON_3fPoint>& );
  CRhinoPointCloudObject& operator=(const ON_SimpleArray<ON_3fPoint>&); // leave attributes unchanged.

  /*
  Description:
    Copies copies point cloud and moves user data from 
    point_cloud to m_point_cloud.
  Parameters:
    pointCloud - [in] information in point_cloud is
       copied.  Any attached ON_UserData is moved from
       point_cloud to m_point_cloud.
  */
  void SetPointCloud( ON_PointCloud& pointCloud );

  // Description:
  //   Set the point cloud geometry.
  // Parameters:
  //   pointCount - [in] points copied
  //   pointList - [in] array of point_count many points
  void SetPointCloud( int pointCount, const ON_3dPoint* pointList );

  // Description:
  //   Set the point cloud geometry.
  // Parameters:
  //   pointCount - [in] points copied
  //   pointList - [in] array of point_count many points
  void SetPointCloud( int pointCount, const ON_3fPoint* pointList );

  // Description:
  //   Set the point cloud geometry.
  // Parameters:
  //  pointDimension - [in] dimension of input points (2 or 3)
  //  rational - [in] true if points are in homogeneous rational form
  //  pointCount - [in] number of points
  //  pointStride - [in] number of doubles to skip between points
  //  points - [in] array of point coordinates
  void SetPointCloud( int pointDimension,
                      BOOL32 rational,
                      int pointCount,
                      int pointStride,
                      const double* points
                      );

  // Description:
  //   Set the point cloud geometry.
  // Parameters:
  //  pointDimension - [in] dimension of input points (2 or 3)
  //  rational - [in] true if points are in homogeneous rational form
  //  pointCount - [in] number of points
  //  pointStride - [in] number of doubles to skip between points
  //  points - [in] array of point coordinates
  void SetPointCloud( int pointDimension,
                      BOOL32 rational,
                      int pointCount,
                      int pointStride,
                      const float* points
                      );

  const ON_PointCloud& PointCloud() const;

	bool GetTightBoundingBox( ON_BoundingBox& tightBbox, 
                            bool growBox = false,
			                      const ON_Xform* xform = nullptr
                            ) const override;

protected:
  int SubObjectsAreSelected() const override;
  int SubObjectsAreHighlighted() const override;

public:
  bool IsSubObjectSelected(ON_COMPONENT_INDEX componentIndex) const override;
  int GetSelectedSubObjects(ON_SimpleArray<ON_COMPONENT_INDEX>& componentIndices) const override;
  int UnselectAllSubObjects() override;
  int UnhighlightAllSubObjects() override;
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX componentIndex) const override;
  int GetHighlightedSubObjects(ON_SimpleArray<ON_COMPONENT_INDEX>& componentIndices) const override;
  bool HighlightSubObject(ON_COMPONENT_INDEX componentIndex, bool highlight = true) const override;
  int SelectSubObject(ON_COMPONENT_INDEX componentIndex,
                      bool select = true,
                      bool synchHighlight = true,
                      bool persistentSelect = false
                      ) const override;

  void DeleteFromDocNotification() override;

  void EnableGrips(bool gripsOn) override;

  // virtual CRhinoObject::DrawV6 override
  virtual void DrawV6( class CRhinoObjectDrawContext* draw_context ) const RHINO_NOEXCEPT override;
  
  void Draw(CRhinoDisplayPipeline& dp) const override;
  void DrawSubObject(CRhinoDisplayPipeline& dp, ON_COMPONENT_INDEX componentIndex) const override;
  void DrawHighlightedSubObjects(CRhinoDisplayPipeline& dp) const override;

  bool PickFilter(const CRhinoGetObject&, bool) const override;

  int Pick(const CRhinoPickContext&, CRhinoObjRefArray&) const override;
  
  bool SnapTo(const CRhinoSnapContext& snapContext, CRhinoSnapEvent& snapEvent) const override;

  // Returns "point cloud"
  const wchar_t* ShortDescription( bool bPlural ) const override;

  int GetSubObjects(ON_SimpleArray<CRhinoObject*>& subObjects, const CRhinoViewport* pViewport = nullptr) const override;

private:
  /*
  Parameters:
    componentIndex - [in] identifies a point in the point cloud.
  Returns:
    an unsigned int with bits set to indicate the status of the identified component.
    CRhinoPointCloud::select_mask: the point identified by component_index is selected
    CRhinoPointCloud::persistent_select_mask: the point identified by component_index is persistently selected
    CRhinoPointCloud::highlight_mask: the point identified by component_index is highlighted
  */
  unsigned int PointStatus(ON_COMPONENT_INDEX componentIndex) const;


  /*
  Parameters:
    componentIndex - [in] identifies a point in the point cloud.
    status - [in]
      an unsigned int with bits set to indicate the status of the identified component.
      CRhinoPointCloud::select_mask: the point identified by component_index is selected
      CRhinoPointCloud::persistent_select_mask: the point identified by component_index is persistently selected
      CRhinoPointCloud::highlight_mask: the point identified by component_index is highlighted
  */
  void SetPointStatus(ON_COMPONENT_INDEX componentIndex,unsigned int status);

  enum
  {
    select_mask = 1,
    persistent_select_mask = 2,
    highlight_mask = 4
  };

public: // Cache management
  class CRhCacheDataPointCloud* PointCloudCache() const;
  void DestroyRuntimeCache(bool bDelete=true) override;

private:
  friend class CRhPointCloudGrips;

  ON_PointCloud m_point_cloud;
  ON__UINT_PTR m_reserved = 0;
};


/*
Description:
  Returns a random subsample of a point cloud.
Parameters:
  pPointCloud       - [in] The point cloud to subsample.
  points_to_keep    - [in] The number of points to keep.
  pSubsample        - [in] If not nullptr, this point cloud will be used and returned.
  pProgressReporter - [in] Allows the caller to see the progress the computation.
  pTerminator       - [in] Allows the caller to terminate computation.
Returns:
  A new ON_PointCloud which is a subsample of pPointCloud.
  Note: if pSubsample == nullptr, then memory will be allocated for the
  returned point cloud and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_PointCloud* RhinoPointCloudRandomSubsample(
  const ON_PointCloud* pPointCloud,
  const int points_to_keep,
  ON_PointCloud* pSubsample = nullptr,
  ON_ProgressReporter* pProgressReporter = nullptr,
  ON_Terminator* pTerminator = nullptr
);

/*
Description:
  Creates planar curves by intersecting a plane with a point cloud.
Parameters:
  pPointCloud     - [in] The point cloud to intersect.
  plane           - [in] The plane to intersect with.
  tolerance       - [in] The document's model absolute tolerance.
  max_distance    - [in] Maximum distance to plane. The thickness of the "slab" around the plane from
                         which sample points are taken. Those sample points are projected to the section
                         plane and a polyline is found that connects them. This distance depends on the
                         size of the point cloud and the spacing of the points.
  min_distance    - [in] Minimum distance between points. A threshold for the minimum spacing between
                         adjacent sample points. If there are points closer than that, some are not used.
  bOpenCurves     - [in] Create open curves.
  bCreateSpline   - [in] Creates smooth curves. You can create both a curve and a polyline.
  bCreatePolyline - [in] Creates a polyline. You can create both a curve and a polyline.
  fit_tolerance   - [in] The tolerance used to fit the curve through the polyline.
  out_curves      - [out] The output curves.
Returns:
  The number of curves added to out_curves.
Remarks:
  Memory for the output curves is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
int RhinoCreatePointCloudSection(
  const ON_PointCloud* pPointCloud,
  ON_Plane plane,
  double tolerance,
  double max_distance,
  double min_distance,
  bool bOpenCurves,
  bool bCreateSpline,
  bool bCreatePolyline,
  double fit_tolerance,
  ON_SimpleArray<ON_Curve*>& out_curves
);

/*
Description:
  Creates planar curves by intersecting a plane with a point cloud.
Parameters:
  pPointCloud     - [in] The point cloud to intersect.
  start_point     - [in] Start point for vector that is normal to contour plane.
  end_point       - [in] End point for vector that is normal to contour plane.
  interval        - [in] The interval or distance between contours.
  tolerance       - [in] The document's model absolute tolerance.
  max_distance    - [in] Maximum distance to plane. The thickness of the "slab" around the plane from
                         which sample points are taken. Those sample points are projected to the section
                         plane and a polyline is found that connects them. This distance depends on the
                         size of the point cloud and the spacing of the points.
  min_distance    - [in] Minimum distance between points. A threshold for the minimum spacing between
                         adjacent sample points. If there are points closer than that, some are not used.
  bOpenCurves     - [in] Create open curves.
  bCreateSpline   - [in] Creates smooth curves. You can create both a curve and a polyline.
  bCreatePolyline - [in] Creates a polyline. You can create both a curve and a polyline.
  fit_tolerance   - [in] The tolerance used to fit the curve through the polyline.
  out_curves      - [out] The output curves.
Returns:
  The number of curves added to out_curves.
Remarks:
  Memory for the output curves is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
int RhinoCreatePointCloudContours(
  const ON_PointCloud* pPointCloud,
  ON_3dPoint start_point,
  ON_3dPoint end_point,
  double interval,
  double tolerance,
  double max_distance,
  double min_distance,
  bool bOpenCurves,
  bool bCreateSpline,
  bool bCreatePolyline,
  double fit_tolerance,
  ON_SimpleArray<ON_Curve*>& out_curves
);


class RHINO_SDK_CLASS CRhinoGripObject : public CRhinoPointObject
{
  ON_OBJECT_DECLARE(CRhinoGripObject);
public:
  CRhinoGripObject();
  ~CRhinoGripObject();

  operator ON_3dPoint() const;
  operator const ON_3dPoint&() const;

  /*
  Description:
    Sets location of grip point.
  Parameter:
    new_location - [in]
  See Also:
    CRhinoGripObject::MoveGrip
  */
  CRhinoGripObject& operator=(ON_3dPoint newLocation);

  // Returns ON::grip_object
  ON::object_type ObjectType() const override;

  // Returns "grip" or "grips"
  const wchar_t* ShortDescription(bool bPlural) const override;

  bool PickFilter(const CRhinoGetObject&, bool) const override;

  bool IsDeletable() const override;

  /*
  Description:
    virtual CRhinoObject::ModifyAttributes override to
    prevent changing grip attributes.
  Returns:
    false
  */
  bool ModifyAttributes( 
         const ON_3dmObjectAttributes& newAttributes,
         bool enableUndo = true,
         bool quiet = false
         ) override;

  /*
  Description:
    virtual CRhinoObject::Attributes override returns
    parent object's display and layer attributes.
  Returns:
    attributes
  */
  const CRhinoObjectAttributes& Attributes() const override;


  /*
  Description:
    virtual CRhinoObject::SnapTo override pays checks
    for control polygon culling and then calls
    base class SnapTo().
  Returns:
    attributes
  */
  bool SnapTo(const CRhinoSnapContext& snapContext,CRhinoSnapEvent& snapEvent) const override;

  /*
  Returns:
    Current grip location.
  See Also:
    CRhinoGripObject::GripBasePoint
  */
  ON_3dPoint GripLocation() const;

  /*
  Returns:
    Original grip location.
  See Also:
    CRhinoGripObject::GripLocation
  */
  ON_3dPoint GripBasePoint() const;

  /*
  Returns:
    Current transformation applied to the grip.
    If EnableTransformationWatching(true) has not been
    called, the the returned transformation will always
    be the translation from GripBasePoint() to
    GripLocation() and false will be returned.
    If EnableTransformationWatching(true) has been 
    called and a transformation command, like
    Rotate or Scale has moved the grip, then the
    transformation will be returned.
  See Also:
    CRhinoGripObject::GripLocation
    CRhinoGripObject::GripBasePoint
  */
  bool GetGripPointXform(ON_Xform& xform) const;

  /*
  Description:
    If you want the grips to record the entire transformation
    applied by commands like Rotate and Scale, then call
    EnableTransformationWatching(true) in the constructor
    for the grip.  If MoveGrip(xform) is used to move the
    grip, then the transformation will be saved and can
    be retrieved by calling GetGripTransformation().
  */
  void EnableGetGripPointXform(bool enable);

  /*
  Description:
    Sometimes grips have directions.  These directions
    can have any length and do not have to be orthogonal.
  Parameters:
    x,y,z - [out]
  Returns:
    True if the grip has directions.
  */
  bool GetGripDirections(
    ON_3dVector& x,
    ON_3dVector& y,
    ON_3dVector& z
    ) const;

  /*
  Returns:
    True if grip has moved.    
  */
  virtual
  bool GripMoved() const;

  /*
  Returns:
    Object that owns this grip
  */
  CRhinoObject* Owner() const;

  /*
  Description:
    Move the grip to a new location.
  Parameters:
    xform - [in] transformation applied to base point.
    morph - [in] transformation applied to base point.
    delta - [in] translation applied to base point.
    newLocation - [in] new location for grip point
  See Also:
    CRhinoGripObject::operator=
  */
  void MoveGrip( const ON_Xform& xform );
  void MoveGrip( const CRhinoSpaceMorph& morph );
  void MoveGrip( ON_3dVector delta );
  void MoveGrip( ON_3dPoint newLocation );

  /*
  Description:
    Undoes any grip moves made by calling MoveGrip.
  */
  virtual
  void UndoMoveGrip();

  /*
  Description:
    Called when a grip is being dynamically dragged.
  Parameters:
    worldLine - [in] 3d world mouse line
    delta - [in] default translation
  Remarks:
    Default calls MoveGrip(delta)
  */
  virtual
  void DragGrip(CRhinoViewport& vp, ON_Line worldLine, ON_3dVector delta);

  /*
  Description:
    CRhinoDoc::ShowObject and CRhinoDoc::HideObject call this
    function when a grip visibility is changed.  If needed,
    override HideGrip to do any extra work that is required.
    For example, when mesh grips are hidden, the ON_Mesh::m_H[]
    array needs to be updated.
  Parameters:
    hide - [in]
      true mean grips is being hidden,
      false means grip is being shown.
  */
  virtual
  bool HideGrip(bool hide) const;

  /*
  Returns:
    The weight of a NURBS control point grip or ON_UNSET_VALUE if the grip is
    not a NURBS control point. Default implementation returns ON_UNSET_VALUE.
  */
  virtual
  double Weight() const;

  /*
  Description:
    Set the weight of a NURBS control point grip.
  double
    weight - [in] NURBS control point weight (>= 0 )
  Returns:
    True if weight was changed. Base class implementation does nothing
    and simply returns false
  */
  virtual
  bool SetWeight( double weight );

  /*
  Description:
    Used to get a grip's logical neighbors, like NURBS curve, surface, 
    and cage control point grips.
  Parameters:
    dr - [in]
      -1 to go back one grip, +1 to move forward one grip
      For curves, surfaces and cages, this is the first 
      parameter direction.
    ds - [in]
      -1 to go back one grip, +1 to move forward one grip
      For surfaces and cages this is the second 
      parameter direction.
    dt - [in]
      For cages this is the third parameter direction.
  Returns:
    Pointer to the logical neighbor or NULL if
    there is no logical neighbor.
  */
  CRhinoGripObject* NeighborGrip( 
          int dr, 
          int ds, 
          int dt, 
          bool wrap = false 
          ) const;

  int GetNeighborGrips( 
          ON_SimpleArray<CRhinoGripObject*>& neighborGrips,
          bool wrap = false 
          ) const;

  /*
  Parameters:
    cvindex - [out]
      Control point indices are appended to this array.    
  Returns:
    Number of NURBS curve control points managed by this grip.
    If the grip is not a curve control point, zero is returned.
  */
  virtual
  int GetCurveCVIndices(ON_SimpleArray<int>& cvindex) const;

  virtual
  bool GetCurveParameters( double* t ) const;

  /*
  Parameters:
    cvindex - [out]
      Control point indices are appended to this array.    
  Returns:
    Number of NURBS surface control points managed by this grip.
    If the grip is not a surface control point, zero is returned.
  */
  virtual
  int GetSurfaceCVIndices(ON_SimpleArray<ON_2dex>& cvindex) const;

  virtual
  bool GetSurfaceParameters(double* u, double* v) const;

  /*
  Parameters:
    cvindex - [out]
      Control point indices are appended to this array.    
  Returns:
    Number of NURBS cage control points managed by this grip.
    If the grip is not a cage control point, zero is returned.
  */
  virtual
  int GetCageCVIndices(ON_SimpleArray<ON_3dex>& cvndex) const;

  virtual
  bool GetCageParameters(double* u, double* v, double* w) const;

  /*
  Returns:
    If the grip is a mesh vertex grip, the mesh vertex index is returned,
    otherwise ON_UNSET_UINT_INDEX is returned.
  */
  virtual
  unsigned int MeshVertexIndex() const;

  /*
  Returns:
    If the grip is a subd grip, the subd component id is returned,
    otherwise ON_UNSET_UINT_INDEX is returned.
  */
  virtual
  unsigned int SubDComponentId() const;

  virtual
  ON_SubDComponentPtr SubDComponentPtr() const;

  enum GRIP_TYPE : unsigned int
  {
    no_grip           =   0,

    // default grips (at most one per object type)
    // default grips ids are between 1 and 999
    curve_cv_grip    =    1,
    surface_cv_grip  =    2,
    cage_cv_grip     =    3,
    annotation_grip  =    4,
    mesh_vertex_grip =    5,
    point_cloud_grip =    6,
    light_grip       =    7,
    camera_grip      =    8,
    morph_cv_grip    =    9,
    iref_grip        =   10,
    extrusion_grip   =   11,
    hatch_grip       =   12,
    subd_cv_grip     =   13,
    // Don't add types for new annotations without adding those types
    // to the section in CRhinoDoc::SnapTo( ) that checks grip->IsVisibleInViewport()
    // because currently that only looks at annotation_type grips
    // and if you add a type here the grips that use that type can't be filtered 
    // for IsVisibleInViewport()

    // custom grips 
    custom_grip      = 1000, // drag line is automatically drawn
    custom_nodragline_grip = 1001, // no drag line is drawn
  };

  /*
  Returns:
    The value of m_grips_owner->m__grips_type
  */
  GRIP_TYPE GripType() const;

  /*
  Description:
    This id is used to differentiate between different types
    of custom grips.  The default object grips (curve cvs, etc.)
    always have an id of ON_nil_uuid.
  Returns:
    The value of m_grips_owner->m_grips_id.
  */
  ON_UUID GripsId() const;


  ///////////////////////////////////////////////////////////
  //
  // Expert user fields below - don't look, don't touch
  // No support is available for these fields
  //

  // grips object that owns this grip point
  CRhinoObjectGrips* m_grip_owner;

  // index of grip in CRhinoObjectGrips m_grip_list array
  int m_grip_index;

  // starting location of grip point
  ON_3dPoint m_base_point;

  virtual 
  void NewLocation();

  /*
  Description:
    If a grip has "directions" (like a surface control point), this
    function evaluates those directions.
  Parameters:
    gd - [out] 
  Returns:
    True if evaluation sets gd.
  */
  virtual
  bool EvaluateGripDirections(class CRhinoGripDirections& gd) const;

  /*
  Description:
    Unconditionally sets grip directions to values
    in gd.
  Parameters:
    gd - [in]
  */
  void SetGripDirections( CRhinoGripDirections gd );

  CRhinoGripObject* Next() const; // next in CRhinoDoc::m_grip_list list
  CRhinoGripObject* Prev() const; // prev in CRhinoDoc::m_grip_list list


  //Description:
  //When a CRhinoGripObjectEx grip drag is about to begin,
  //the grip has the option of providing its own constraints.
  //Returns:
  //true - custom constraints added.
  //false - use default constraints.
  virtual bool AddDragConstraints(class CRhinoGetPoint& gp) const;

protected:
  void MoveGripHelper(ON_3dPoint, const ON_Xform* );
};

class RHINO_SDK_CLASS CRhinoGripObjectEx : public CRhinoGripObject
{
  ON_OBJECT_DECLARE(CRhinoGripObjectEx);
public:
  CRhinoGripObjectEx();
  ~CRhinoGripObjectEx();

  // V6 virtual function moved to CRhinoGripObject.
  ON_DEPRECATED_MSG("Use CRhinoGripObject in place of CRhinoGripObjectEx")
  bool AddDragConstraints(class CRhinoGetPoint& gp) const override;
};

