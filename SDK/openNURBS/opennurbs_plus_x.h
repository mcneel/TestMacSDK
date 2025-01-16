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

#if !defined(OPENNURBS_PLUS_X_INC_)
#define OPENNURBS_PLUS_X_INC_

#if !defined(OPENNURBS_PLUS) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

// The ON_X_EVENT class is used to report curve-curve and curve-surface
// intersection events.
class ON_CLASS ON_X_EVENT
{
  // NO VIRTUAL FUNCTIONS IN THIS CLASS
public:
  
  // Default construction sets everything to zero.
  ON_X_EVENT();

  // This class must use default copy constructor, operator=, and destructor.

  /*
  Description:
    Compares intersection events and sorts them in the 
    canonical order.
  Returns:
    @untitled table
    -1    this  < other
     0    this == other
    +1    this  > other
  Remarks:
    ON_CX_EVENT::Compare is used to sort intersection events into canonical
    order.
  */
  static
  int Compare( const ON_X_EVENT* a, const ON_X_EVENT* b );

  /*
   Description:
     Compare intersection events
   Parameters:
      XA, XB are a pair of events to compare for equivalence
      RelativePointTol - 3d points are compared by || Pa - Pb || < ( 1 + ||Pa||) * RelativePointTol
      log [in] if not null and false is returned then a description of the error is appended to log.
   Returns:
     @untitled table
     true if the XA and XB represent the same intersections
   Remarks:
      use RelativePointTol=ON_UNSET_VALUE to use the default value of ON_SQRT_EPSILON
   */
  static
    bool CompareEquivalent(const ON_X_EVENT& XA, const ON_X_EVENT& XB, double RelativePointTol, ON_TextLog* log);


  void Dump(ON_TextLog& text_log) const;

  /*
  Description:
    Check intersection event values to make sure they are valid.
  Parameters:
    text_log - [in] If not null and an error is found, then a description
                    of the error is printed to text_log.
    intersection_tolerance - [in] 
         0.0 or value used in intersection calculation.
    overlap_tolerance - [in] 
         0.0 or value used in intersection calculation.
    curveA - [in] 
         nullptr or curveA passed to intersection calculation.
    curveA_domain - [in] 
         nullptr or curveA domain used in intersection calculation.
    curveB - [in]
         nullptr or curveB passed to intersection calculation.
    curveB_domain - [in]
         nullptr or curveB domain used in intersection calculation.
    surfaceB - [in]
         nullptr or surfaceB passed to intersection calculation.
    surfaceB_domain0 - [in]
         nullptr or surfaceB "u" domain used in intersection calculation.
    surfaceB_domain1 - [in]
         nullptr or surfaceB "v" domain used in intersection calculation.
  Returns:
    True if event is valid.
  */
  bool IsValid(
          ON_TextLog* text_log,
          double intersection_tolerance,
          double overlap_tolerance,
          const class ON_Curve* curveA,
          const class ON_Interval* curveA_domain,
          const class ON_Curve* curveB,
          const class ON_Interval* curveB_domain,
          const class ON_Surface* surfaceB,
          const class ON_Interval* surfaceB_domain0,
          const class ON_Interval* surfaceB_domain1
          ) const;


  /*
  Description:
    Expert user tool to copy portions of the intersection
    event information from one event to another.  
    If src.m_type is ON_X_EVENT::csx_*, then the
    m_b[] and m_nodeB_t[] values are treated as
    surface parameters, otherwise the values are
    treated as curve parameters.  The m_type field
    is not copied.
  Parameters:
    src - [in]
    src_end - [in] 0 or 1 (m_A[] source index)
    dst - [out]
    dst_end - [in] 0 or 1 (m_A[] destination index)
  */
  static 
  void CopyEventPart(
        const ON_X_EVENT& src, 
        int src_end,
        ON_X_EVENT& dst, 
        int dst_end 
        );

  /*
  Description:
    Expert user tool to cleanup a list of intersection 
    events.
  Parameters:
    event_tolerance - [in] If the distance between
      events is <= event_tolerance, they will
      be considered to be the same event.  Typically,
      event_tolerance is "small" and
      event_tolerance <= intersection_tolerance.

    overlap_tolerance - [in] minimum length for an
      intersection overlap segment. Typically,
      overlap_tolerance is "large" and
      overlap_tolerance > intersection_tolerance

    xevent_count - [in]
    xevent - [in/out]
    domain - [in] domain of the intersection problem.
                  domain[0] is curve domain and either
                  domain[1] is domain of second curve or
                  domain[1] x domain[2] is surface domain
                  domain = nullptr means no special handling of 
                  domain boundaries
  Returns:
    Length of cleaned list.
  */
  static
    int CleanList(
      double event_tolerance,
      double overlap_tolerance,
      int xevent_count,
      ON_X_EVENT* xevent,
      ON_Interval* domain = nullptr
          );

  /*
  Description:
    Expert user tool to check a list of intersection events.
  Parameters:
    xevent_count - [in]
    xevent - [in]
    text_log - [in] nullptr or place to describe errors.
    intersection_tolerance - [in]
    overlap_tolerance - [in]
    curveA - [in]
    curveA_domain - [in] (can be nullptr)
    curveB - [in] (nullptr for curve-surface intersection events)
    curveB_domain - [in] (can be nullptr)
    surfaceB - [in] (nullptr for curve-curve intersection events)
    surfaceB_domain0 - [in] (can be nullptr)
    surfaceB_domain1 - [in] (can be nullptr)
  Returns:
    True if list is valid.
  */
  static
  bool IsValidList(
          int xevent_count,
          const ON_X_EVENT* xevent,
          ON_TextLog* text_log,
          double intersection_tolerance,
          double overlap_tolerance,
          const class ON_Curve* curveA,
          const class ON_Interval* curveA_domain,
          const class ON_Curve* curveB,
          const class ON_Interval* curveB_domain,
          const class ON_Surface* surfaceB,
          const class ON_Interval* surfaceB_domain0,
          const class ON_Interval* surfaceB_domain1
          );

  /*
  Description:
    Do a quick and simple test to see if this curve
    lies on some portion of curveB.
  Parameters:
    curveA_domain - [in] non empty interval
    sample_count - [in] minimum number of test points
    overlap_tolerance - [in]
    cnodeA - [in]
    cnodeB - [in]
    curveB_domain - [in] optional domain restriction
  Returns:
    True if sections overlap.
  */
  static
  bool IsValidCurveCurveOverlap( 
        ON_Interval curveA_domain,
        int sample_count,
        double overlap_tolerance,
        const class ON_CurveTreeNode* cnodeA, 
        const class ON_CurveTreeNode* cnodeB,
        const ON_Interval* curveB_domain = 0
        );

  /*
  Description:
    Do a quick and simple test to see if this curve
    lies on the plane.
  Parameters:
    curveA_domain - [in]  non empty interval
    sample_count - [in] minimum number of test points
    endpont_tolerance - [in] tolerance to use when checking ends
    overlap_tolerance - [in] tolerance to use when checking interior
    cnodeA - [in]
    plane_equation - [in]
  Returns:
    True if curve lies on the plane
  */
  static 
  bool IsValidCurvePlaneOverlap( 
        ON_Interval curveA_domain,
        int sample_count,
        double endpoint_tolerance,
        double overlap_tolerance,
        const class ON_CurveTreeNode* cnodeA,
        const ON_PlaneEquation* plane_equation
        );

  /*
  Description:
    Do a quick and simple test to see if this curve
    lies on the surface.
  Parameters:
    curveA_domain - [in]  non empty interval
    sample_count - [in] minimum number of test points
    overlap_tolerance - [in]
    cnodeA - [in]
    snodeB - [in]
    surfaceB_udomain - [in] optional domain restriction
    surfaceB_vdomain - [in] optional domain restriction
  Returns:
    True if sections overlap.
  */
  static 
  bool IsValidCurveSurfaceOverlap( 
        ON_Interval curveA_domain,
        int sample_count,
        double overlap_tolerance,
        const class ON_CurveTreeNode* cnodeA, 
        const class ON_SurfaceTreeNode* snodeB,
        const ON_Interval* surfaceB_udomain = 0,
        const ON_Interval* surfaceB_vdomain = 0
        );

  /*
  Description:
    Convert input intersection tolerance to value used
    in calculations.
  Parameters:
    intersection_tolerance - [in]
  Returns:
    Value use in intersection calculations.
  */
  static
  double IntersectionTolerance( double intersection_tolerance );

  /*
  Description:
    Convert input intersection tolerance to value used
    in calculations.
  Parameters:
    intersection_tolerance - [in]
  Returns:
    Value use in intersection calculations.
  */
  static
  double OverlapTolerance( double intersection_tolerance, double overlap_tolerance );

  /*
  Returns:
    True if m_type is ccx_point or csx_point.
  */
  bool IsPointEvent() const;

  /*
  Returns:
    True if m_type is ccx_overlap or csx_overlap.
  */
  bool IsOverlapEvent() const;

  /*
  Returns:
    True if m_type is ccx_point or ccx_overlap
  */
  bool IsCCXEvent() const;

  /*
  Returns:
    True if m_type is csx_point or csx_overlap
  */
  bool IsCSXEvent() const;


  enum TYPE
  { 
    no_x_event  =  0,

    // Two valid event types for curve-curve intersections
    ccx_point   =  1, // curve-curve transverse intersection point
    ccx_overlap =  2, // curve-curve intersection overlap

    // Two valid event types for curve-surface intersections
    csx_point   =  3, // curve-surface transverse intersection point
    csx_overlap =  4  // curve-surface intersection overlap
  };

  // Event directions for use in the m_dirA[] and m_dirB[]
  // fields. The "x_from_*" values are used to report the 
  // behavior of the curve as it enters the intersection 
  // event. The "x_to_*" values are used to report the 
  // behavior of the curve as it leaves the intersection
  // event.
  enum DIRECTION
  { 
    no_x_dir       = 0, 

    at_end_dir     = 1, // event is at the start/end/side of object's
                        // parameter space

    from_above_dir = 2, // curve enters x-event from above surface/other curve
    from_below_dir = 3, // curve enters x-event from below surface/other curve
    from_on_dir    = 4, // curve enters x-event tangent surface/other curve

    to_above_dir   = 5, // curve leaves x-event above surface/other curve
    to_below_dir   = 6, // curve leaves x-event below surface/other curve
    to_on_dir      = 7  // curve leaves x-event tangent to surface/other curve
  };

  // This field is a scratch field for users.
  // The constructor sets it to zero and the 
  // intersectors never use it.
  ON_U m_user;

  // The m_type field determines how the values in the other
  // fields are interpreted.  See the detailed comment below
  // for complete information
  TYPE m_type;

  private:
  // (When needed, code will be added to set overlap type and
  //  m_overlap_type will be public.)
  // When the event is ccx_overlap or csx_overlap, m_overlap_type
  // specifies the behavior of distance between curveA and
  // curveB/surfaceB in the overlap region of the the event.
  // 0: unset
  // 1: The distance from curveA to curveB/surfaceB is constant
  //    from the starting point to the ending point of the 
  //    overlap event.
  // 2: curveA is constantly getting closer to curveB/surfaceB
  //    from the starting ON_X_EVENT::point to the ending point of the 
  //    overlap event.
  // 3: curveA is constantly getting farther from curveB/surfaceB
  //    from the starting point to the ending point of the 
  //    overlap event.
  // 4: The distance from curveA to curveB/surfaceB varies in
  //    a non-monotone manner from the starting point to the
  //    ending point of the overlap event.
  unsigned char m_overlap_type;

  public:

  // An overlap start or end can be a local minimum of
  // the function dist( C(t), S(u,v) ) for a curve
  // surface intersection 
  // Local_Min(i) is true if (m_a[i], m_b[2*i], m_b[2*i+1]) is
  // a local minimum of the function  dist( C(t), S(u,v) ).
  bool Local_Min(int ei) const;
  void Set_Local_Min(int ei, bool newvalue);

  private:
  char m_local_min;

  unsigned char m_reserved[2];
  public:

  ON_3dPoint m_A[2]; // intersection points on first curve
  ON_3dPoint m_B[2]; // intersection points on second curve or surface
  double m_a[2];     // intersection parameters on first curve
  double m_b[4];     // intersection parameters on second curve or surface
  
  // There are cases when it is valuable to have direction
  // flags on intersection events.  The m_dirA[] and m_dirB[]
  // fields provide a place to store these flags.  Because this
  // information is rarely used, it is not computed by the
  // intersection routines.  You can use
  //   ON_SetCurveCurveIntersectionDir 
  // or
  //   ON_SetCurveSurfaceIntersectionDir
  // to fill in these fields.
  DIRECTION m_dirA[2];
  DIRECTION m_dirB[2];

  // tree nodes where the intersection events occurred.
  const class ON_CurveTreeNode* m_cnodeA[2];
  double m_nodeA_t[2]; // nodeA bezier parameters corresponding to a[] values 
  const class ON_CurveTreeNode* m_cnodeB[2];
  const class ON_SurfaceTreeNode* m_snodeB[2];
  double m_nodeB_t[4]; // nodeB bezier parameters corresponding to b[] values.

  // Each intersection event is assigned a runtime serial number.
  unsigned int m_x_eventsn;

  private:
  // (When needed, code will be added to set parameter info
  //  and m_parameterA/B will be public.)
  // m_parameterA/B 
  //   0: unset
  //   1: at a span interior parameter (C-infinity continuous)
  //   2: at a span "knot" of undetermined continuity
  //   3: at a "smooth" span "knot"
  //   4: at a "kink" span "knot" 
  unsigned char m_parameterA[2];
  unsigned char m_parameterB[2];
  public:

  /*
  The m_type field determines how the values in the other
  fields are interpreted.

    ccx_point events:
      a[0] = a[1] = first curve parameter
      A[0] = A[1] = intersection point on first curve
      b[0] = b[1] = second curve parameter
      B[0] = B[1] = intersection point on second curve
      b[2] = b[3] = not used
      cnodeA[0] = cnodeA[1] = pointer to first curve's tree node
      cnodeB[0] = cnodeB[1] = pointer to second curve's tree node
      snodeB[0] = snodeB[1] = 0

    ccx_overlap events:
      (a[0],a[1]) = first curve parameter range (a[0] < a[1])
      A[0] = intersection start point on first curve
      A[1] = intersection end point on first curve
      (b[0],b[1]) = second curve parameter range (b[0] != b[1])
      B[0] = intersection start point on second curve
      B[1] = intersection end point on second curve
      b[2] = b[3] = not used
      cnodeA[0] = pointer to first curve's tree node for start point
      cnodeA[1] = pointer to first curve's tree node for end point
      cnodeB[0] = pointer to second curve's tree node for start point
      cnodeB[1] = pointer to second curve's tree node for end point
      snodeB[0] = snodeB[1] = 0
   
   csx_point events:
      a[0] = a[1] = curve parameter
      A[0] = A[1] = intersection point on curve
      (b[0],b[1]) = (b[2],b[3]) = surface parameter
      B[0] = B[1] intersection point on surface
      cnodeA[0] = cnodeA[1] = pointer to curve's tree node
      cnodeB[0] = cnodeB[1] = 0;
      snodeB[0] = snodeB[1] = pointer to surface's tree node

    csx_overlap events:
      (a[0],a[1]) = curve parameter range (a[0] < a[1])
      A[0] = intersection start point on first curve
      A[1] = intersection end point on first curve
      (b[0],b[1]) = surface parameter for curve(a[0]).
      B[0] = intersection start point on surface
      (b[2],b[3]) = surface parameter for curve(a[1]).
      B[1] = intersection end point on surface
      cnodeA[0] = pointer to curve's tree node for start point
      cnodeA[1] = pointer to curve's tree node for end point
      snodeB[0] = pointer to surface's tree node for start point
      snodeB[1] = pointer to surface's tree node for end point
  */
};


/*
Check First Order Necessary Conditions for CSX result
  Curve surface intersection solves an optimization problem.  This code verifies that the solution
  point, either a csx_point or the end of a csx_overlap, 
  satisfies what are called first order necessary conditions for the point being a local minimum 
  relative to the specified problem domain
Parameters:
  text_log - (in)optional text log.If false is returned the problem description is logged.
  xevent - (in)CSX event result must be a csx_point or csx_overlap event type
  end_index - (in)If xevent is an overlap then end_index should be 0 or 1

These are the parameters that we used to call Curve::IntersectSurface()
  curve_domain(in)
  surface_domain - (in)
Return :
  true if the point satisfies the first order necessary conditions for this point being a local minimum
*/

ON_DECL
bool  ON_CSXFirstOrderNecessaryConditions(
  ON_TextLog* text_log,
  const ON_X_EVENT& xevent,
  int end_index,
  const ON_Interval* curve_domain,
  const ON_Interval* surface_domain0,
  const ON_Interval* surface_domain1);

class ON_CLASS ON_MESH_POINT
{
public:
  ON_MESH_POINT();
  /*
  Description:
    Set the topological component of the this mesh point.
  Parameters:
    edge_parameter - [out] optional - can be nullptr
       If the component is an edge, then *edge_parameter
       is set (0 < *edge_parameter < 1).  If the component
       is not an edge, then *edge_parameter is set to
       ON_UNSET_VALUE.
  Returns:
    Component index of the topological component.
  */
  ON_COMPONENT_INDEX SetTopologyComponentIndex(double* edge_parameter);

  bool IsValid( ON_TextLog* text_log ) const;

  /*
  Description:
    Sets the m_mesh, m_et, m_ci, m_edge_index, m_face_index,
    m_t, and m_P fields.
  Parameters:
    m_mesh - [in]
    fi - [in] index of the face
    t - [in] barycentric coordinates of the point.  All t[i]
        must be >= 0.0, <= 1.0, and t[0]+t[1]+t[2]+t[3] must
        be within ON_ZERO_TOLERANCE of 1.0.
  Returns:
    True if the input is valid and the fields are set.
  Remarks:
    Calling SetFacePoint with 1 non-zero barycentric coordinate will
    set ON_MESH_POINT::m_ci.m_type to ON_COMPONENT_INDEX::meshtop_vertex
    and ON_MESH_POINT::m_ci.m_index to the mesh topology vertex index
    of the vertex at the referenced corner of the face.
    And will set m_edge_index to the mesh topology index of the edge 
    preceding that vertex and will set m_et to 0.0 or 1.0
  */
  bool SetFacePoint( const class ON_Mesh* mesh, int fi, const double t[4] );

  /*
  Description:
    Sets the m_mesh, m_et, m_ci, m_edge_index, m_face_index,
    m_t, and m_P fields.
  Parameters:
    m_mesh - [in]
    fi - [in] index of the face the topology's m_topf[] array
              (which is parallel to the mesh's m_F[] array)
    ei - [in] index of edge in the topology's m_tope[] array
    et - [in] edge parameter 0.0 <= et <= 1.0
  Returns:
    True if the input is valid and the fields are set.
    Calling SetEdgePoint with ei == 0 or ei == 1.0 will
    set ON_MESH_POINT::m_ci.m_type to ON_COMPONENT_INDEX::meshtop_vertex
    and ON_MESH_POINT::m_ci.m_index to the mesh topology vertex index
    of the vertex at the start or end of the edge.
  */
  bool SetEdgePoint( const ON_Mesh* mesh, int fi, int ei, double et );

  const ON_Mesh* m_mesh;
  const class ON_MeshTreeNode* m_mnode; // can be null

  // m_et = edge parameter when m_ci refers to an edge.
  double m_et; 
  ON_COMPONENT_INDEX m_ci;  

  // The constructor sets m_edge_index and m_face_index 
  // to -1, however when ON_MESH_POINT is used in an 
  // ON_SimpleArray<>, unused points will have a "default"
  // values of 0.
  //
  // When set, m_edge_index is an index of an edge in
  // m_mesh->Topology().m_tope[] and m_face_index is an
  // index of a face in m_mesh->Topology().m_topf[].
  // The the m_mesh->Topology().m_topf[] and m_mesh->m_F[]
  // arrays are parallel, m_face_index is also the index
  // of the face in m_mesh->m_F[].
  //
  // The m_face_index field must always be set.  When
  // m_ci refers to a vertex, any face that uses the vertex
  // may appear as m_face_index.  When m_ci refers to an
  // edge or m_edge_index is set, then any face that uses
  // that edge may appear as m_face_index.
  int m_edge_index;
  int m_face_index;

  // Gets the mesh face indices of the triangle where the
  // intersection is on the face takes into consideration
  // the way the quad was split during the intersection
  bool GetTriangle(int& idxA, int& idxB, int& idxC) const; 
  bool GetTriangle(ON_3dPoint& ptA, ON_3dPoint& ptB, ON_3dPoint& ptC) const; 

  // Face triangle where the intersection takes place;
  // 0 is unset
  // A is 0,1,2
  // B is 0,2,3
  // C is 0,1,3
  // D is 1,2,3
  char m_Triangle;

  // Barycentric quad coordinates for the point m_P on the mesh
  // face m_mesh->m_F[m_face_index]
  // If the face is a triangle disregard m_t[3] (it should be 
  // set to 0.0).
  //
  // If the face is a quad and is split between vertices 0 and 2,
  // then m_t[3] will be 0.0 when m_P is on the triangle defined
  // by vi[0], vi[1], vi[2], and m_t[1] will be 0.0 when m_P is
  // on the triangle defined by vi[0], vi[2], vi[3].
  //
  // If the face is a quad and is split between vertices 1 and 3,
  // then m_t[2] will be 0.0 when m_P is on the triangle defined
  // by vi[0], vi[1], vi[3], and m_t[0] will be 0.0 when m_P is
  // on the triangle defined by vi[1], vi[2], vi[3].
  double m_t[4]; 
  ON_3dPoint m_P;    // 3d location
  unsigned int m_sn; // ON_MESH_POINT serial number
};

class ON_CLASS ON_MMX_POINT
{
public:
  ON_MMX_POINT();

  bool IsValid( ON_TextLog* text_log, double intersection_tolerance ) const;

  // Nothing that needs a constructor can be in this class.
  // In particular, NO simple arrays.  I want to be able
  // to safely use memset((ptr,0,sizeof(ON_MESH_X_EVENT))
  // to create one of these.

  // Should be something really simple along the lines of ...
  // This is just a suggestion

  ON_MESH_POINT m_A;
  ON_MESH_POINT m_B;

  enum TYPE
  { 
    no_mx_event =  0,

    // Two valid event types for curve-mesh intersections
    mmx_point   =  1, // mesh-mesh transverse intersection point
    mmx_overlap =  2  // mesh-mesh intersection overlap
  };

  /*
    The m_type field determines how the values in the other
    fields are interpreted.
     
    mmx_point  = intersection point on mesh
    mmx_overlap  = overlap endpoints on mesh:
  */
  TYPE m_type;

  unsigned int m_sn; //ON_MMX_POINT serial number

  //ON_MMX_POINT overlap serial number. -1 if the ON_MMX_POINT does not have mmx_overlap as it's type.  
  //No more than 2 ON_MMX_POINT should have the same m_overlap_sn.
  int m_overlap_sn; 

  int m_MMX_Set_ID; //ID for a set of points that theoretically ought to match
  ON_3fPoint m_MMX_Set_PT; //the point chosen to be the intersection for the entire set.  Chosen based on a logical best answer (point that is on an edge or vertex)
  int m_MeshVertex_ID; //ID for a set of points that theoretically ought to match
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MMX_POINT>;
#endif

/*
Description:
  Sets ON_X_EVENT m_dirA[] and m_dirB[] flags for in intersection
  of coplanar curves.  For each  m_dirA[]/m_dirB[] flag that is 
  set to ON_X_EVENT, the curve geometry at the intersection is
  examined to set the flags.
Parameters:
  N [in] normal to the plane
  xcount - [in] number of intersection events
  xevent - [in] array of xcount intersection events
  a0 - [in]
  a1 - [in] (a0,a1) = curveA intersection domain
  b0 - [in]
  b1 - [in] (b0,b1) = curveB intersection domain
See Also:
  ON_Curve::IntersectCurve
  ON_SetCurveSurfaceIntersectionDir
*/
ON_DECL
bool ON_SetCurveCurveIntersectionDir(
            ON_3dVector N,
            int xcount,
            ON_X_EVENT* xevent,
            double a0,
            double a1,
            double b0,
            double b1
            );


/*
Description:
  Sets ON_X_EVENT m_dirA[] and m_dirB[] flags for a curve surface
  intersection.  For each  m_dirA[]/m_dirB[] flag that is 
  set to ON_X_EVENT, the curve and surface geometry at the 
  intersection is examined to set the flags.
Parameters:
  xcount - [in] number of intersection events
  xevent - [in] array of xcount intersection events
  t0 - [in]
  t1 - [in] (t0,t1) = curveA intersection domain
  u0 - [in]
  u1 - [in] (u0,u1) = surfaceB u intersection domain
  v0 - [in]
  v1 - [in] (v0,v1) = surfaceB v intersection domain
Returns:
  << TODO: Add return codes here >>
See Also:
  ON_Curve::IntersectSurface
  ON_SetCurveCurveIntersectionDir
*/
ON_DECL
bool ON_SetCurveSurfaceIntersectionDir(
            int xcount,
            ON_X_EVENT* xevent,
            double t0,
            double t1,
            double u0,
            double u1,
            double v0,
            double v1
            );

// The ON_SSX_EVENT class is used to report surface-surface
// intersection events.
class ON_CLASS ON_SSX_EVENT
{
  // NO VIRTUAL FUNCTIONS IN THIS CLASS
public:
  ON_SSX_EVENT() ON_NOEXCEPT;
  ~ON_SSX_EVENT(); // deletes m_curveA, m_curveB, m_curve3d

  // copies  m_curveA, m_curveB, m_curve3d
  ON_SSX_EVENT(const ON_SSX_EVENT&);

  // copies  m_curveA, m_curveB, m_curve3d
  ON_SSX_EVENT& operator=(const ON_SSX_EVENT& src);

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_SSX_EVENT(ON_SSX_EVENT&&) ON_NOEXCEPT;

  // The ON_SSX_EVENT rvalue assignment operator potentially deletes 
  // existing curves on this which could throw exceptions.
  ON_SSX_EVENT& operator=(ON_SSX_EVENT&&);
#endif

  void Dump(ON_TextLog& text_log) const;

  /*
  Description:
    Check intersection event values to make sure they are valid.
  Parameters:
    text_log - [in] If not null and an error is found, then a description
                    of the error is printed to text_log.
    intersection_tolerance - [in] 
         0.0 or value used in intersection calculation.
    overlap_tolerance - [in] 
         0.0 or value used in intersection calculation.
    fitting_tolerance - [in] 
         0.0 or value used in intersection calculation.
    surfaceA - [in] 
         nullptr or surfaceA passed to intersection calculation.
    surfaceA_domain0 - [in]
         nullptr or surfaceA "u" domain used in intersection calculation.
    surfaceA_domain1 - [in]
         nullptr or surfaceA "v" domain used in intersection calculation.
    surfaceB - [in]
         nullptr or surfaceB passed to intersection calculation.
    surfaceB_domain0 - [in]
         nullptr or surfaceB "u" domain used in intersection calculation.
    surfaceB_domain1 - [in]
         nullptr or surfaceB "v" domain used in intersection calculation.
  Returns:
    True if event is valid.
  */
  bool IsValid(
          ON_TextLog* text_log,
          double intersection_tolerance,
          double overlap_tolerance,
          double fitting_tolerance,
          const class ON_Surface* surfaceA,
          const class ON_Interval* surfaceA_domain0,
          const class ON_Interval* surfaceA_domain1,
          const class ON_Surface* surfaceB,
          const class ON_Interval* surfaceB_domain0,
          const class ON_Interval* surfaceB_domain1
          ) const;

  /*
  Returns:
    True if m_type is ssx_transverse_point or ssx_tangent_point.
  See Also:
    ON_SSX_EVENT::IsCurveEvent
    ON_SSX_EVENT::IsTinyEvent
  */
  bool IsPointEvent() const;

  /*
  Returns:
    True if m_type is ssx_transverse or ssx_tangent.
  See Also:
    ON_SSX_EVENT::IsPointEvent
    ON_SSX_EVENT::IsTinyEvent
  */
  bool IsCurveEvent() const;

  /*
  Description:
    This function can be used to detect intersection events
    that are "nearly" points.
  Parameters:
    tiny_tolerance - [in]
  Returns:
    True if 
    m_type is ssx_transverse_point or ssx_tangent_point,
    or,
    m_type is ssx_transverse, ssx_tangent or ssx_overlap
    and length of the longest side of m_curve3d's
    bounding box is <= tiny_tolerance.
  See Also:
    ON_SSX_EVENT::IsPointEvent
    ON_SSX_EVENT::IsCurveEvent
  */
  bool IsTinyEvent(double tiny_tolerance) const;

  /*
  Returns:
    True if m_type is ssx_tangent,  or ssx_tangent_point.
  */
  bool IsTangentEvent() const;

  /*
  Returns:
    True if m_type is ssx_overlap.
  */
  bool IsOverlapEvent() const;

  // This field is a scratch field for users.
  // The constructor sets it to zero and the 
  // intersectors never use it.
  ON_U m_user;

  enum class TYPE : unsigned int
  {
    unset            = 0,
    transverse       = 1,   // transverse surface-surface intersection curve
    tangent          = 2,   // tangent surface-surface intersection curve
    overlap          = 3,   // overlap surface-surface intersection curve
    transverse_point = 4,   // transverse surface-surface intersection point
    tangent_point    = 5,   // tangent surface-surface intersection point
    unknown          = 0xE, // an intersection exists, but is of unknown type
    none             = 0xF  // no intersection exits
  };

  TYPE m_type;

  // If m_type = ssx_transverse, ssx_tangent, or ssx_overlap,
  // then the intersection curves are returned here.  
  // In all cases the 3 curves are compatibly oriented.
  // For ssx_transverse events, the 3d curve direction
  // agrees with SurfaceNormalB x SurfaceNormalA
  // For ssx_tangent events, the orientation is random.
  // For ssx_overlap events, the overlap is to the left of
  // m_curveA.  These curves are deleted by ~ON_SSX_EVENT().
  // If you want to harvest a curve for long term use, set 
  // the pointer to nullptr.
  ON_Curve* m_curveA;  // 2d surface A parameter space curve
  ON_Curve* m_curveB;  // 2d surface B parameter space curve
  ON_Curve* m_curve3d; // 3d surface B parameter space curve

  // If m_type = ssx_transverse_point or ssx_tangent_point,
  // the the points are returned here
  ON_3dPoint m_pointA;  // 2d surfaceA parameter space point with z = 0
  ON_3dPoint m_pointB;  // 2d surfaceB parameter space point with z = 0
  ON_3dPoint m_point3d; // 3d intersection point
};

#endif
