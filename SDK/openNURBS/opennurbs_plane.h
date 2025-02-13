//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////

#if !defined(ON_PLANE_INC_)
#define ON_PLANE_INC_

class ON_CLASS ON_Plane
{
public:

  /*
  Description:
    The default constructor creates a plane
    with orgin=(0,0,0), xaxis=(1,0,0), yaxis=(0,1,0)
    zaxis=(0,0,1), and equation=(0,0,1,0).
  */
  ON_Plane();

  /*
  Description:
    Construct a plane from a point and normal vector.
  Parameters:
    origin - [in] point on the plane
    normal - [in] non-zero normal to the plane
  Remarks:
    origin = point, zaxis = unitized normal, xaxis
    xaxis set with xaxis.PerpendicularTo(zaxis).
  See Also:
    ON_Plane::CreateFromNormal
  */
  ON_Plane(
    const ON_3dPoint& origin,
    const ON_3dVector& normal
    );



  /*
  Description:
    Construct a plane from a point, and two vectors in
    the plane.
  Parameters:
    origin - [in] point on the plane
    x_dir - [in] non-zero vector in the plane that
        determines the xaxis direction.
    y_dir - [in] non-zero vector not parallel to x_dir
        that is used to determine the yaxis direction.
        y_dir does not have to be perpendicular to x_dir.
  */
  ON_Plane(
    const ON_3dPoint& origin,
    const ON_3dVector& x_dir,
    const ON_3dVector& y_dir
    );

  /*
  Description:
    Construct a plane from three non-collinear points.
  Parameters:
    origin - [in] point on the plane
    x_point - [in] second point in the plane.
        The xaxis will be parallel to x_point-origin.
    y_point - [in] third point on the plane that is
        not collinear with the first two points.
        yaxis*(y_point-origin) will be > 0.
  */
  ON_Plane(
    const ON_3dPoint& origin,
    const ON_3dPoint& x_point,
    const ON_3dPoint& y_point
    );

  /*
  Description:
    Construct a plane from an equation.
  Parameters:
    equation - [in] an array of 4 doubles with
       one of equation[0], equation[1], or equation[2]
       being non-zero.
  */
  ON_Plane(
    const double equation[4]
    );

  ON_Plane(
    const ON_PlaneEquation& plane_equation
    );

  ~ON_Plane();

  bool operator==(const ON_Plane&) const;
  bool operator!=(const ON_Plane&) const;

  /*
  Description:
    Create a plane from a point and normal vector.
  Parameters:
    origin - [in] point on the plane
    normal - [in] non-zero normal to the plane
  Remarks:
    origin = point, zaxis = unitized normal, xaxis
    xaxis set with xaxis.PerpendicularTo(zaxis).
  Returns:
    true if valid plane is created.
  See Also:
    ON_Plane::CreateFromNormalYup
  */
  bool CreateFromNormal(
    const ON_3dPoint& origin,
    const ON_3dVector& normal
    );

  /*
Description:
  Construct a plane from a point and normal vector and a vector that
  projects to the positive y-axis.
Parameters:
  origin - [in] point on the plane
  normal - [in] non-zero normal to the plane
  y-up - [in] vector linearly independent from normal that projects
              to the positive y-axis of the plane
Remarks:
  origin = point,
  zaxis = unitized normal,
  xaxis = unitized ( y-up X normal )
  yaxis = zaxis X xaxis
See Also:
  ON_Plane::CreateFromNormal
*/
 bool CreateFromNormalYup(
    const ON_3dPoint& origin,
    const ON_3dVector& normal,
    const ON_3dVector& y_up
  );


  /*
  Description:
    Construct a plane from a point, and two vectors in
    the plane.
  Parameters:
    origin - [in] point on the plane
    x_dir - [in] non-zero vector in the plane that
        determines the xaxis direction.
    y_dir - [in] non-zero vector not parallel to x_dir
        that is used to determine the yaxis direction.
        y_dir does not have to be perpendicular to x_dir.
  Returns:
    true if valid plane is created.
  */
  bool CreateFromFrame(
    const ON_3dPoint& origin,
    const ON_3dVector& x_dir,
    const ON_3dVector& y_dir
    );

  /*
  Description:
    Construct a plane from three non-collinear points.
  Parameters:
    origin - [in] point on the plane
    point_on_x - [in] second point in the plane.
        The xaxis will be parallel to x_point-origin.
    point_on - [in] third point on the plane that is
        not collinear with the first two points.
        yaxis*(y_point-origin) will be > 0.
  Returns:
    true if valid plane is created.
  */
  bool CreateFromPoints(
    const ON_3dPoint& origin,
    const ON_3dPoint& point_on_x,
    const ON_3dPoint& point_on
    );

  /*
  Description:
    Construct a plane from an equation.
  Parameters:
    equation - [in] an array of 4 doubles with
       one of equation[0], equation[1], or equation[2]
       being non-zero.
  Remarks:
    points on the plane will satisfy 
    x*equation[0] +y*equation[1] + z*equation[2] + equation[3] = 0
  Returns:
    true if valid plane is created.
  */
  bool CreateFromEquation(
    const double equation[4]
    );

  bool CreateFromEquation(
    const class ON_PlaneEquation& plane_equation
    );

  /*
  Returns:
    ON_Plane::UnsetPlane if input is not valid.
  */
  static ON_Plane FromPointList(
    size_t point_list_count,
    const ON_3dPoint* point_list
    );

  /*
  Returns:
    ON_Plane::UnsetPlane if input is not valid.
  */
  static ON_Plane FromPointList(
    const ON_SimpleArray< ON_3dPoint >& point_list
    );


  /*
  Returns:
    ON_Plane::UnsetPlane if input is not valid.
  */
  static ON_Plane FromPointList(
    size_t point_list_count,
    const ON_3fPoint* point_list
    );

  /*
  Returns:
    ON_Plane::UnsetPlane if input is not valid.
  */
  static ON_Plane FromPointList(
    const ON_SimpleArray< ON_3fPoint >& point_list
    );

  /*
  Returns:
    ON_Plane::UnsetPlane if input is not valid.
  */
  static ON_Plane FromPointList(
    const class ON_3dPointListRef& point_list
    );

  /*
  Returns:
    ON_Plane::UnsetPlane if input is not valid.
  */
  static ON_Plane FromPointList(
    size_t point_index_count,
    const unsigned int* point_index_list,
    const class ON_3dPointListRef& point_list
    );

  static ON_Plane FromPointList(
    size_t point_index_count,
    size_t point_index_stride,
    const unsigned int* point_index_list,
    const class ON_3dPointListRef& point_list
    );

  /*
  Description:
    Test plane to see if it is valid.
  Returns:
    true if all fields contain reasonable
    information and equation jibes with point and zaxis.
  */
  bool IsValid() const;

  void Dump(class ON_TextLog&) const;

  /*
  Returns:
    Plane origin.
  */
  const ON_3dPoint& Origin() const;

  /*
  Returns:
    Plane unit x-axis.
  */
  const ON_3dVector& Xaxis() const;

  /*
  Returns:
    Plane unit y-axis.
  */
  const ON_3dVector& Yaxis() const;

  /*
  Returns:
    Plane unit normal.
  */
  const ON_3dVector& Normal() const;


  /*
  Description:
    Set the origin and update the plane equation
  Parameters:
    origin - [in] the new origin
  */
  void SetOrigin( const ON_3dPoint& origin );
  
  /*
  Description:
    Evaluate a point on the plane
  Parameters:
    u - [in]
    v - [in] evaluation parameters
  Returns:
    plane.origin + u*plane.xaxis + v*plane.yaxis
  */
  ON_3dPoint PointAt(
    double u,
    double v
    ) const;

  /*
  Description:
    Evaluate a point on the plane
  Parameters:
    u - [in]
    v - [in] evaluation parameters
    w - [in] elevation parameter
  Returns:
    plane.origin + u*plane.xaxis + v*plane.yaxis + z*plane.zaxis
  */
  ON_3dPoint PointAt(
    double u,
    double v,
    double w
    ) const;

  /*
  Description:
    Get an isoparameteric line on the plane.
  Parameters:
    dir - [in] direction of iso-parametric line
        0: first parameter varies and second parameter is constant
           e.g., line(t) = plane(t,c)
        1: first parameter is constant and second parameter varies
           e.g., line(t) = plane(c,t)
    c - [in] value of constant parameter 
  Returns:
    iso-parametric line
  */
  ON_Line IsoLine(
         int dir,
         double c
         ) const;

  /*
  Description:
    Get signed distance from the plane to a point.
  Parameters:
    point - [in]
  Returns:
    Signed distance from a point to a plane.
  Remarks:
    If the point is on the plane, the distance is 0.
    If the point is above the plane, the distance is > 0.
    If the point is below the plane the distance is < 0.
    The zaxis determines the plane's orientation.
  */
  double DistanceTo( 
        const ON_3dPoint& point
        ) const;


  bool GetDistanceToBoundingBox(
           //returns false if plane has zero length normal
				   const ON_BoundingBox&, // Box

           //output
				   double* min,    // min signed dist from plane to box 
           double* max     //max signed dist from plane to box
           ) const;

  /*
  Description:
    Update the plane equation based on the current values
    of the origin and zaxis.
  Returns:
    true if successful.  false if zaxis is zero.
  Remarks:
    If you modify a plane's origin or zaxis, call UpdateEquation()
    to set equation[]. 
  */
  bool UpdateEquation();

  /*
  Description:
    Get point on plane that is closest to a given point.
  Parameters:
    world_point - [in] 3d point
    u - [out] 
    v - [out] The point ON_Plane::PointAt(*u,*v) is the point
              on the plane that is closest to world_point.
  Returns:
    true if successful.
  */
  bool ClosestPointTo( 
         ON_3dPoint world_point,
         double* u,
         double* v
         ) const;

  /*
  Description:
    Get point on plane that is closest to a given point.
  Parameters:
    point - [in]
  Returns:
    A 3d point on the plane that is closest to world_point.
  */
  ON_3dPoint ClosestPointTo( 
         ON_3dPoint point
         ) const;

  // For intersections see ON_Intersect();

  /*
  Description:
    Transform plane.
  Parameters:
    xform - [in] transformation to apply to plane
  Returns:
    true if successful
  */
  bool Transform( 
        const ON_Xform& xform
        );

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Morph plane.
  Parameters:
    morph - [in] morph to apply to plane
  Returns:
    true if successful
  Remarks:
    The resulting plane still has an orthonormal frame
  */
  bool Morph( const ON_SpaceMorph& morph );
#endif

  /*
  Description:
    Transform a plane by swapping coordinates.
  Parameters:
    i - [in]
    j - [in] indices of coordinates to swap.
        0 = x coordinate, 1 = y coordinate, 2 = z coordinate.
  Returns:
    true if successful.
  */
  bool SwapCoordinates(
        int i,
        int j
        );

  /*
  Description:
    Rotate a plane about its origin.
  Parameters:
    sin_angle - [in] sine of rotation angle
    cos_angle - [in] cosine of rotation angle
    axis - [in] axis of rotation
  Returns:
    true if successful
  */
  bool Rotate(
        double sin_angle,
        double cos_angle,
        const ON_3dVector& axis
        );

  /*
  Description:
    Rotate a plane about its origin.
  Parameters:
    angle - [in] rotation angle in radians
    axis - [in] axis of rotation
  Returns:
    true if successful
  */
  bool Rotate(
        double angle,
        const ON_3dVector& axis
        );

  /*
  Description:
    Rotate a plane about a point.
  Parameters:
    sin_angle - [in] sine of rotation angle
    cos_angle - [in] cosine of rotation angle
    axis - [in] axis of rotation
    center - [in] center of rotation
  Returns:
    true if successful
  */
  bool Rotate(
        double sin_angle,
        double cos_angle,
        const ON_3dVector& axis,
        const ON_3dPoint&  center
        );

  /*
  Description:
    Rotate a plane about a point.
  Parameters:
    angle - [in] rotation angle in radians
    axis - [in] axis of rotation
    center - [in] center of rotation
  Returns:
    true if successful
  */
  bool Rotate(
        double angle,
        const ON_3dVector& axis,
        const ON_3dPoint& center
        );

  /*
  Description:
    Translate a plane.
  Parameters:
    delta - [in] translation vector
  Returns:
    true if successful
  */
  bool Translate(
        const ON_3dVector&  delta
        );

  /*
  Description:
    Flip plane orientation by swapping x and y axes,
    reversing the zaxis, and updating the equation.
  Returns:
    true if successful
  */
  bool Flip();

  // world coordinate system ON_Plane(ON_3dPoint::Origin, ON_3dVector::XAxis, ON_3dVector::YAxis); 
  const static ON_Plane World_xy;

  // world coordinate system ON_Plane(ON_3dPoint::Origin, ON_3dVector::YAxis, ON_3dVector::ZAxis); 
  const static ON_Plane World_yz;

  // world coordinate system ON_Plane(ON_3dPoint::Origin, ON_3dVector::ZAxis, ON_3dVector::XAxis); 
  const static ON_Plane World_zx;

  // All values are ON_UNSET_VALUE.
	const static
	ON_Plane UnsetPlane;

  // All values are ON_DBL_QNAN.
	const static ON_Plane NanPlane;

public:
  // origin of plane
  ON_3dPoint  origin;

  // unit X axis of plane
  ON_3dVector xaxis;

  // unit Y axis of plane
  ON_3dVector yaxis;

  // unit Z axis of plane
  ON_3dVector zaxis;

  // equation of plane
  ON_PlaneEquation plane_equation;
  //double equation[4];
};

class ON_CLASS ON_ClippingPlaneInfo
{
public:
  // C++ defaults for construction, destruction, copy construction
  // and operator= work fine.

  // A point is visible if m_plane_equation.ValueAt(point) <= 0.
  // (This is the opposite convention from what OpenGL uses.)
  ON_PlaneEquation m_plane_equation;
  ON_UUID m_plane_id = ON_nil_uuid;
  bool m_bEnabled = false;

  // A distance where the clipping plane does not clip geometry.
  // A positive value is equivalent to placing another clipping plane at a
  // distance from this clipping plane along it's normal and then flipping it
  //
  // The depth must also be enabled to be effective
  double Depth() const;

  // Negative depth values are currently not allowed. If a negative depth value
  // is passed to this function, it will not the the internal depth value
  void SetDepth(double depth);

  // Default is false
  bool DepthEnabled() const;
  void SetDepthEnabled(bool on);

  void Default();
  bool Write( ON_BinaryArchive& ) const;
  bool Read( ON_BinaryArchive& );

private:
  bool m_depth_enabled = false;
  char m_reserved[2] = {};

  // This should be a double, but is a float in order to not change
  // the class size. When the Rhino SDK can break, this data type should change.
  float m_depth = 0;
};

// The only reason this class exists is to maintain extra information on ON_ClippingPlane
// without changing the size of the ON_ClippingPlane class (to not break the SDK). This
// class helps maintain a list of additional information associated with an ON_ClippingPlane.
// Pretend it does not exist
class ON_CLASS ON_ClippingPlaneDataStore
{
public:
  ON_ClippingPlaneDataStore();
  ON_ClippingPlaneDataStore(const ON_ClippingPlaneDataStore& src);
  ~ON_ClippingPlaneDataStore();
  ON_ClippingPlaneDataStore& operator=(const ON_ClippingPlaneDataStore& src);
  
  unsigned int m_sn=0;
};

class ON_CLASS ON_ClippingPlane
{
public:
  ON_ClippingPlane();
  ~ON_ClippingPlane();

  void Default();

  ON_Plane m_plane;
  ON_UuidList m_viewport_ids; //ids of viewports that this clipping plane "clips"
  ON_UUID m_plane_id;
  bool m_bEnabled; // true if this clipping plane is active

  ON_ClippingPlaneInfo ClippingPlaneInfo() const;

  // A distance where the clipping plane does not clip geometry.
  // A positive value is equivalent to placing another clipping plane at a
  // distance from this clipping plane along it's normal and then flipping it
  //
  // The depth must also be enabled to be effective
  double Depth() const;

  // Negative depth values are currently not allowed. If a negative depth value
  // is passed to this function, it will not the the internal depth value
  void SetDepth(double depth);

  // Default is false
  bool DepthEnabled() const;
  void SetDepthEnabled(bool on);

  // Default is false
  bool ParticipationListsEnabled() const;
  void SetParticipationListsEnabled(bool on);

  void SetParticipationLists(
    const ON_SimpleArray<ON_UUID>* objectIds,
    const ON_SimpleArray<int>* layerIndices,
    bool isExclusionList);

  const ON_UuidList* ObjectClipParticipationList() const;
  const ON_SimpleArray<int>* LayerClipParticipationList() const;
  bool ClipParticipationListsAreExclusionLists() const;

  bool Read( class ON_BinaryArchive& );
  bool Write( class ON_BinaryArchive& ) const;

private:
  bool m_depth_enabled = false;
  bool m_participation_lists_enabled = false;
  char m_reserved;
  
  ON_ClippingPlaneDataStore m_data_store;
};


#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_Plane>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_ClippingPlane>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_ClippingPlaneInfo>;
#endif

extern ON_EXTERN_DECL const ON_Plane ON_xy_plane;
extern ON_EXTERN_DECL const ON_Plane ON_yz_plane;
extern ON_EXTERN_DECL const ON_Plane ON_zx_plane;

/*
Description:
  Get a convex hull of a set of 3d points.
Parameters:
  points - [in]
    List of points.  This function can handle tens of points
    but is too slow for hundreds of points.
  hull -[out]
    Equations of the sides of the convex hull are appended to
    this list.  
    A point P is inside the hull if hull[i].ValueAt(P) <= 0 for
    every plane equation.
Returns:
  Number of equations appended to hull[] array.
  If 0, then the points are coincident or collinear.
  If 2, then the points are coplanar and the returned
  planes are parallel.
  If >= 4, then the points are in a 3d convex hull.
*/
ON_DECL
int ON_Get3dConvexHull( 
          const ON_SimpleArray<ON_3dPoint> & points, 
          ON_SimpleArray<ON_PlaneEquation> & hull 
          );

#endif
