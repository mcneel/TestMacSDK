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

#if !defined(OPENNURBS_PLUS) || !defined(OPENNURBS_PLUS_INC_) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_TRIMESH_INC_)
#define OPENNURBS_PLUS_TRIMESH_INC_

class ON_CLASS ON_TrianglePlane
{
public:
  // Equation of plane: a*x + b*y + c*z + d = 0
  double a, b, c, d;
  
  // evaluation tolerance
  double tol;

  /*
  Description:
    Scale a,b,c,d and tol by the same factor so that 
    1 = a^2 + b^2 + c^2.
  Returns:
    True if the scaling was successful.
    False if (a,b,c) is the zero vector or cannot be unitized.
    When false is returned, the values of  a, b, c, d and tol
    are set to 0.0.
  */
  bool Normalize();

  /*
  Description:
    Create a triangle plane equation from three points.
  Parameters:
    A - [in]
    B - [in]
    C - [in]
      corners of the triangle.
    bSinglePrecisionPoints - [in]
      True if the point locations were created from
      single precision information (floats).
      False if the point locations were created from
      double precision information.
  Returns:
    True if the three points are not coplanar and a plane
    equation was created.  False if a plane equation could
    not be created.  When false is returned, a, b, and c
    are set to ON_UNSET_VALUE and d and tol are set to 0.
  */
  bool Create( 
    const ON_3dPoint* A, 
    const ON_3dPoint* B, 
    const ON_3dPoint* C, 
    bool bSinglePrecisionPoints 
    );

  /*
  Description:
    Create a triangle plane equation from three points.
  Parameters:
    A - [in]
    B - [in]
    C - [in]
      corners of the triangle.
    bSinglePrecisionPoints - [in]
      True if the point locations were created from
      single precision information (floats).
      False if the point locations were created from
      double precision information.
  Returns:
    True if the three points are not coplanar and a plane
    equation was created.  False if a plane equation could
    not be created.  When false is returned, a, b, and c
    are set to ON_UNSET_VALUE and d and tol are set to 0.
  */
  bool Create( 
    const double A[3], 
    const double B[3], 
    const double C[3], 
    bool bSinglePrecisionPoints 
    );
  
  /*
  Description:
    Create a triangle plane equation from three points.
  Parameters:
    A - [in]
    B - [in]
    C - [in]
      corners of the triangle.
  Returns:
    True if the three points are not coplanar and a plane
    equation was created.  False if a plane equation could
    not be created.  When false is returned, a, b, and c
    are set to ON_UNSET_VALUE and d and tol are set to 0.
  */
  bool Create( 
    const ON_3fPoint* A, 
    const ON_3fPoint* B, 
    const ON_3fPoint* C
    );

  /*
  Description:
    Create a triangle plane equation from three points.
  Parameters:
    A - [in]
    B - [in]
    C - [in]
      corners of the triangle.
  Returns:
    True if the three points are not coplanar and a plane
    equation was created.  False if a plane equation could
    not be created.  When false is returned, a, b, and c
    are set to ON_UNSET_VALUE and d and tol are set to 0.
  */
  bool Create( 
    const float A[3], 
    const float B[3], 
    const float C[3]
    );

  /*
  Parameters:
    point - [in]
  Returns:
    Signed elevation of point above/below triangle's plane.
  */
  double Elevation( const class ON_3dPoint* point ) const;
  double Elevation( const double point[3] ) const;

  /*
  Parameters:
    point - [in]
  Returns:
    Signed elevation of point above/below triangle's plane.
  */
  double Elevation( const class ON_3fPoint* point ) const;
  double Elevation( const float point[3] ) const;
};


struct ON_TriangleMeshPoint
{
  unsigned int m_id;
  unsigned int m_triangle_index;
  double m_triangle_bc[3]; // barycentric coordinates
};

class ON_CLASS ON_TriangleMesh
{
public:
  ON_TriangleMesh();
  ON_TriangleMesh(const ON_TriangleMesh&);
  ~ON_TriangleMesh();

  ON_TriangleMesh& operator=(const ON_TriangleMesh&);

  unsigned int m_V_count; // number of vertices in m_V[]
  unsigned int m_T_count; // number of triangles in m_T[]

  // vertex locations
  const double (*m_V)[3];

  // optional vertex unit normals
  const double (*m_N)[3];

  /*
  Returns:
    Vertex locations as an array of ON_3dPoints saving
    you the cast operation.
  */
  const ON_3dPoint* VertexPoints() const;

  /*
  Parameters:
    a - [in]
    b - [in]
  Returns:
    < 0:  a < b in dictionary ordering
    = 0:  a = b
    < 0:  a > b in dictionary ordering
  Remarks:
    null pointers are considered less than non-null pointers.
  */
  static int CompareLocation( const double a[3], const double b[3] );

  /*
  Description:
    Get a 
  Returns:
    A permutation of {0,1, ..., m_V_count-1} such that all 
    ON_TriangleMesh::CompareLocation( Vmap[i], Vmap[i+1] ) <= 0.    
  */
  const unsigned int* VertexMap() const;

  // triangles
  //   m_V[m_T[ti][0]],  m_V[m_T[ti][1]],  m_V[m_T[ti][2]] 
  const unsigned int (*m_T)[3];

  // triangle flags
  //  m_T_flags can be nullptr.  If it is not nullptr, then information
  //  about triangle m_T[ti] is encoded in m_T_flags[ti] as follows:
  //
  //  (m_T_flags[ti] & 0x3F):
  //     The low six bits (m_T_flags[ti] & 0x3F) specifies how
  //     to convert triangle barycentric coordinates 
  //     (a,b,c) to ON_MeshFace barycentric coordinates.
  //        q[ 0x03 & T_flags    ] = a
  //        q[(0x0C & T_flags)>>2] = b
  //        q[(0x30 & T_flags)>>4] = c
  //        q[other element]      = 0.0
  //     The static function BarycentricTriToQuad() will
  //     perform the conversion.
  // 
  //  (m_T_flags[ti] & 0x01C0):
  //     The next 3 bits are used to indicate when an
  //     edge of a triangle is not an edge of the original
  //     mesh object.  For example, when a quad is split
  //     into two triangles, the triangles edges corresponding
  //     to the splitting diagonal have their bits set.
  //     0 != (m_T_flags[ti] & 0x0040) means the triangle
  //     edge opposite vertex m_T[ti][0] is not an edge in
  //     the original mesh.
  //     0 != (m_T_flags[ti] & 0x0080) means the triangle
  //     edge opposite vertex m_T[ti][1] is not an edge in
  //     the original mesh.
  //     0 != (m_T_flags[ti] & 0x0100) means the triangle
  //     edge opposite vertex m_T[ti][2] is not an edge in
  //     the original mesh.
  const unsigned int *m_T_flags; // triangle flags
  // m_T[i][4] = "id" value used in various ways
  // m_T[i][4] = "id" value used in various ways

  // triangle "id" values depend on context and can
  // set as needed.
  // * If the triangle mesh was created from an ON_Mesh using
  //   the Create() function, then m_T_id[ti] is the
  //   ON_Mesh::m_F[] array index of the ON_MeshFace that
  //   generated the triangle.
  const unsigned int *m_T_id;

  /*
  Description:
    Deletes any cached bounding box, triangle plane, R-tree
    or Vmap information.
  Remarks:
    If you modify the length or values of the m_V[] of  m_T[] 
    arrays, then any cached bounding box, triangle plane, 
    R-tree or Vmap information may be invalid and should be
    destroyed.
  */
  void DestroyCachedInformation();

  void Destroy();
  /*
  Description:
    Create a double precision triangle mesh from an ON_Mesh.
  Parameters:
    mesh - [in]
      Mesh to use for triangle references.
    bPersistentMeshReference - [in] (When in doubt, pass false.)
      If bPersistentMeshReference is true, then the caller is
      responsible for making sure "mesh" persists unchanged
      for the life of this ON_TriangleMesh class and as
      much mesh information as possible will be referenced.
      If bPersistentMeshReference is false, all information
      will be copied to heap managed by this ON_TriangleMesh
      class.
  Remarks:
    The index of the ON_Mesh::m_F[] facet used to generate the 
    triangle is m_T[][3].    
    If ON_Mesh::m_F[fi] facet is a quad, then two triangles will
    be made using one of the following vertex sets from 
    ON_MeshFace::vi[] array.
    (0,1,2,fi) and (0,2,3,fi), 
      or 
    (1,2,3,fi) and (1,3,0,fi)    
  */
  bool Create( 
    const class ON_Mesh* mesh, 
    bool bPersistentMeshReference 
    );

  const class ON_Mesh* MeshReference() const;

  /*
  Description:
    Get an ON_Mesh that looks like this triangle mesh.
  Parameters:
    mesh - [in]
      null or the mesh to store the result.
  Returns:
    Null if this mesh is not valid, otherwise a pointer to an
    ON_Mesh class that looks like this triangle mesh.
    If the mesh input parameter is null, then the returned mesh
    is on the heap and must be managed by the caller.  
  */
  class ON_Mesh* MeshForm( class ON_Mesh* mesh ) const;

  /*
  Description:
    Find a point on the triangle mesh that is closest
    to test_point.
  Parameters:
    test_point - [in]
    maximum_distance_tolerance - [in]
      If maximum_distance_tolerance >= 0.0, then an answer is
      returned only if the distance from the point on the
      triangle mesh to test_point is <= maximum_distance_tolerance.
      Otherwise the closest point is returned.
    triangle_index - [out]
    triangle_bc - [out]
      If the return value is true, then triangle_index identifies
      a triangle on the mesh that is closest to test_point.  
      The barycentric coordinates of the point on this triangle 
      are returned in triangle_bc[0,1,2].
      If the return value is false, then triangle_index is set 
      to 0xFFFFFFFF and t is set to 
      (ON_UNSET_VALUE,ON_UNSET_VALUE,ON_UNSET_VALUE).
  */
  bool GetClosestPoint( 
    const class ON_3dPoint* test_point,
    double maximum_distance_tolerance,
    unsigned int * triangle_index,
    double triangle_bc[3]
    ) const;


  /*
  Description:
    Find the intersection points of a line segment and the triangle mesh.
  Parameters:
    p0 - [in]
    p1 - [in]
      The line segment begins at p0 and ends at p1.
    tolerance - [in]
      The distance between the returned points on the line segment 
      and the mesh will be <= tolerance.
    resultCallback - [in]
      Each time an intersection point is found, resultCallback is called.
      If resultCallback() returns true, the search for additional
      intersection points continues. If resultCallback() returns false,
      the search is terminated. The "line_t" and "tri"t" parameters 
      provide the location of the intersection point. If the intersection
      is a single point, then line_t[0] = line_t[1] and tri_t[0] = tri_t[1].
      If the line and triangle overlap, then 0 <= line_t[0] < line_t[1] <= 1.
      The capsule parameter may be shrunk as needed to speed searching
      in applications like ray shooting.  See ON_RTree::Search() for more 
      details about capsule shrinking.
    context - [in]
      This pointer is passed as the first argument to resultCallback().
  Returns:
    True if intersection points were found.
  */
  bool IntersectLine(
    const double p0[3],
    const double p1[3],
    double tolerance,
    bool ON_CALLBACK_CDECL resultCallback(void* context, const double line_t[2], const struct ON_TriangleMeshPoint tri_t[2], struct ON_RTreeCapsule* capsule),
    void* context
    ) const;

  /*
  Description:
    Offset a triangle mesh.
  Parameters:
    offset_distance - [in]
    N - [in]
      array of m_V_count vertex unit length normals.
    offset_V - [out]
      Offset vertex locations are returned here.
      Pass const_cast< double (*)[3] > m_V if you want to
      offset this mesh in place.
  Returns:
    True if successful.
    False if input parameters are not valid.
  */
  bool OffsetMesh( 
    double offset_distance, 
    const double (*N)[3],
    double (*offset_V)[3]
    ) const;

  bool OffsetMesh( 
    double offset_distance, 
    const float (*N)[3],
    double (*offset_V)[3]
    ) const;

  bool OffsetMesh( double offset_distance );


  const class ON_TrianglePlane* PlaneEquation( unsigned int triangle_index ) const;

  /*
  Description:
    Get an r-tree made from triangle bounding boxes and their indices
    that is managed by the ON_TriangleMesh class.
  */
  const class ON_RTree* TriangleRTree() const;

  /*
  Description:
    Get an r-tree made from triangle bounding boxes and their indices
    that is managed by the caller.
  */
  bool CreateTriangleRTree( class ON_RTree& rtree ) const;

  unsigned int Status( unsigned int triangle_index ) const;

  void SetStatus( unsigned int triangle_index, unsigned int status );

  /*
  Returns:
    nullptr if SetStatus has not been called.
    Otherwise a pointer to the m_status[] array.  If you
    const_cast<> this array and modify it, then the value
    returned by SetStatus() will not be reliable.
  */
  const unsigned int* StatusArray() const;

  /*
  Returns:
    Number of non-zero status values.
  */
  unsigned int StatusCount() const;

  /*
  Description:
    Evaluate point on the triangle mesh.
  Parameters:
    triangle_index - [in]
      index of the triangle
    tri_bc - [in]
      barycentric coordinates of the point
    a, b, c - [in]
      barycentric coordinates of the point
  */
  bool EvPoint( 
    unsigned int triangle_index, 
    const double tri_bc[3],
    double point[3]
  ) const;

  bool EvPoint(
    unsigned int triangle_index,
    double a,
    double b,
    double c,
    double point[3]
    ) const;
  
  bool EvPoint( 
    const struct ON_TriangleMeshPoint* triangle_point,
    double point[3]
  ) const;

  /*
  Description:
    Evaluate normal on the triangle mesh.
  Parameters:
    triangle_index - [in]
      index of the triangle
    tri_bc - [in]
      barycentric coordinates of the point
    a, b, c - [in]
      barycentric coordinates of the point
  */
  bool EvNormal( 
    unsigned int triangle_index, 
    const double tri_bc[3],
    double normal[3]
  ) const;

  bool EvNormal(
    unsigned int triangle_index,
    double a,
    double b,
    double c,
    double normal[3]
    ) const;
  
  bool EvNormal( 
    const struct ON_TriangleMeshPoint* triangle_point,
    double normal[3]
  ) const;

  /*
  Description:
    Evaluate normal on the triangle mesh.
  Parameters:
    triangle_index - [in]
      index of the triangle
    tri_bc - [in]
      barycentric coordinates of the point
    a, b, c - [in]
      barycentric coordinates of the point
  */
  bool EvPointAndNormal( 
    unsigned int triangle_index, 
    const double tri_bc[3],
    double point[3],
    double normal[3]
  ) const;

  bool EvPointAndNormal(
    unsigned int triangle_index,
    double a,
    double b,
    double c,
    double point[3],
    double normal[3]
    ) const;
  
  bool EvPointAndNormal( 
    const struct ON_TriangleMeshPoint* triangle_point,
    double point[3],
    double normal[3]
  ) const;
  /*
  Description:
    Convert triangle barycentric coordinates to quad
    barycentric coordinates.
  Parameters:
    T_flags - [in]
      The value of (0x1F & Tflags) specifies the correspondence
      between the corners of the triangle and the corners
      of the quad.  A table of the values and correspondences
      is in the description of the m_T_flags member variable
      above.
    tri_bc - [in]
      Three triangle barycentric coordinates
    quad_bc - [out]
      For quad barycentric coordinates. The corner of
      the quad that does not correspond to a triangle
      corner will have value of 0.0.
  Returns:
    True if the value of (0x1F & Tflags) was a valid and quad_bc
    was set from tri_bc.
    False if the value of (0x1F & Tflags) was not valid and quad_bc
    was set to (0.0,0.0,0.0,0.0).
  */
  static bool BarycentricTriToQuad( 
    unsigned int T_flags, 
    const double tri_bc[3], 
    double quad_bc[4] 
    );

  
  /*
  Description:
    Convert triangle barycentric coordinates to quad
    barycentric coordinates.
  Parameters:
    T_flags - [in]
      The value of (0x1F & Tflags) specifies the correspondence
      between the corners of the triangle and the corners
      of the quad.  A table of the values and correspondences
      is in the description of the m_T_flags member variable
      above.
    a - [in]
    b - [in]
    c - [in]
      (a,b,c) are the triangle barycentric coordinates
    quad_bc - [out]
      For quad barycentric coordinates. The corner of
      the quad that does not correspond to a triangle
      corner will have value of 0.0.
  Returns:
    True if the value of (0x1F & Tflags) was a valid and quad_bc
    was set from tri_bc.
    False if the value of (0x1F & Tflags) was not valid and quad_bc
    was set to (0.0,0.0,0.0,0.0).
  */
  static bool BarycentricTriToQuad( 
    unsigned int T_flags, 
    double a, 
    double b, 
    double c,
    double quad_bc[4] 
    );

private:
  void Copy(const ON_TriangleMesh&);

private:
  ON_BoundingBox m_bbox;
  ON_Workspace m_ws;
  ON_FixedSizePool m_plane_eqn_fsp;

  class ON_TrianglePlane** m_plane_eqn;
  
  const class ON_RTree* m_rtree;

  unsigned int* m_Vmap;

  void* m_reserved1;
  void* m_reserved2;
  void* m_reserved3;

  const class ON_Mesh* m_mesh_reference; // persistent source mesh reference
  unsigned int* m_status;
  unsigned int m_status_count;

  // The bits of m_flags are used to store information
  // about the triangle mesh.
  unsigned int m_flags;
  // (0x0003 & m_flags) - source precision flag
  //   == 0:  Unknown - treat as double precision.
  //   == 1:  The vertex locations used to create this triangle mesh were
  //          from an ON_Mesh::m_V[] array.
  //   == 2:  The vertex locations used to create this triangle mesh were
  //          from an ON_Mesh::DoublePrecisionVertices() array.
};


/*
Description:
  Find the point on the triangle that is closest to test_point.
Parameters:
  A - [in]
  B - [in]
  C - [in]
    triangle corners.
  N - [in]
    null pointer or a nonzero unit vector perpendicular to 
    the plane of the triangle. N may have either orientation
    (+N and -N work equally as well).
    This pointer can be the address of the "a" parameter of
    an ON_TrianglePlane calculated from this triangle.
    If a unit length normal is not available, pass null and
    the necessary information will be calculated in this 
    function.
  test_point - [in]
  triangle_bc - [out]
    The barycentric coordinates of the point on the triangle
    that is closest to test_point. Specifically,
    triangle_bc[0] >= 0, triangle_bc[1] >= 0, triangle_bc[2] >= 0
    1 = triangle_bc[0] + triangle_bc[1] + triangle_bc[2], and
    (triangle_bc[0]*A + triangle_bc[1]*B + triangle_bc[2]*C)
    is the point on the triangle that is closest to test_point.
Returns:
  Zero if the triangle is degenerate.  In this case,
  valid barycentric coordinates of a point are returned,
  but there may be multiple answers.
  Non-zero if the triangle is not degenerate and triangle_bc[]
  are the barycentric coordinates of the point on the
  triangle closest to test_point.  The returned value reports
  the region of the triangles plane test_point projects to.
  If test_point projects to the interior of the triangle, 1 is
  returned.  If test point projects to an edge or vertex of the
  triangle, 2 is returned. If test_point projects to a point
  outside the triangle, 3 is returned
Remarks:
  This function is designed to be as precise as possible, 
  robustly handles points that are not on the triangle, and 
  it handles degenerate cases.  If speed is paramount, use ...
*/
ON_DECL
bool ON_GetClosestPointOnTriangle( 
    const double A[3],
    const double B[3],
    const double C[3],
    const double N[3],
    const double test_point[3],
    double triangle_bc[3]
  );

ON_DECL
bool ON_GetClosestPointOnTriangle( 
    const float A[3],
    const float B[3],
    const float C[3],
    const float N[3],
    const double test_point[3],
    double triangle_bc[3]
  );

/*
Description:
  Determine if a projection of a point to a triangle's plane
  is on the boundary of the triangle.
Parameters:
  A - [in]
  B - [in]
  C - [in]
    triangle corners.
  N - [in]
    null pointer or a nonzero unit vector perpendicular to 
    the plane of the triangle. N may have either orientation
    (+N and -N work equally as well).
    This pointer can be the address of the "a" parameter of
    an ON_TrianglePlane calculated from this triangle.
    If a unit length normal is not available, pass null and
    the necessary information will be calculated in this 
    function.
  test_point - [in]
  Returns:
    True if the projection of a point to a triangle's plane
    is on and edge or vertex of the triangle.
*/
ON_DECL
bool ON_IsProjectedPointOnTriangleBoundary( 
    const double A[3],
    const double B[3],
    const double C[3],
    const double N[3],
    const double test_point[3]
  );

ON_DECL
bool ON_IsProjectedPointOnTriangleBoundary( 
    const float A[3],
    const float B[3],
    const float C[3],
    const float N[3],
    const double test_point[3]
  );

/*
Parameters:
  A - [in]
  B - [in]
  C - [in]
    triangle corners.
  N - [in]
    null pointer or a nonzero unit vector perpendicular to 
    the plane of the triangle. N may have either orientation
    (+N and -N work equally as well).
    This pointer can be the address of the "a" parameter of
    an ON_TrianglePlane calculated from this triangle.
    If a unit length normal is not available, pass null and
    the necessary information will be calculated in this 
    function.
  P - [in]
  Q - [in]
    line segment ends.
  line_t - [out]
    The parameters of the intersection point(s) on the line
    segment.
  triangle_bc - [out]
    The barycentric coordinates of the intersection point(s)
    on the triangle.
Returns:
  0: no intersection
  1: single point of intersection
  2: overlap intersection
*/
ON_DECL
unsigned int ON_IntersectCoplanarLineAndTriangle( 
    const double A[3],
    const double B[3],
    const double C[3],
    const double N[3],
    const double P[3],
    const double Q[3],
    double line_t[2],
    double triangle_bc[2][3]
    );

/*
Parameters:
  A - [in]
  B - [in]
  C - [in]
    triangle corners.
  tri_plane - [in]
    null pointer or a valid triangle plane.
  P - [in]
  Q - [in]
    line segment ends.
  tolerance - [in]
    intersection tolerance.  Values > zero are not perfectly
    handled.
  line_t - [out]
    The parameters of the intersection point(s) on the line
    segment.
  triangle_bc - [out]
    The barycentric coordinates of the intersection point(s)
    on the triangle.
Returns:
  0: no intersection
     line_t and triangle_bc values are not set.
  1: single point of intersection
     line_t[0] and triangle_bc[0] values are set.
     line_t[1] and triangle_bc[1] values are not set
  2: overlap intersection
     All line_t and triangle_bc values are set and
     line_t[0] < line_t[1].
*/
ON_DECL
unsigned int ON_IntersectLineAndTriangle( 
    const double A[3],
    const double B[3],
    const double C[3],
    const ON_TrianglePlane* tri_plane,
    const double P[3],
    const double Q[3],
    double tolerance,
    double line_t[2],
    double triangle_bc[2][3]
    );

#endif
