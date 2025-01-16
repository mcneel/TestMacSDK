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

#if defined(OPENNURBS_PLUS)
#if defined(OPENNURBS_PUBLIC)
#error OPENNURBS_PUBLIC should not be defined for "plus" builds
#endif
#else
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_INC_)
#define OPENNURBS_PLUS_INC_

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class ON_SurfaceTreeBezier;

class ON_CLASS ON_CurveLeafBox
{
public:
  ON_CurveLeafBox();
  // C++ default copy constructor, operator =, and destructor work fine


  /*
  Description:
    Test the leaf box to make sure its settings are valid.
  Parameters:
    text_log - [in] If an error is found and text_log is not null,
      then a short description of the problem is printed to the
      text_log.
  Returns:
    True if the leaf box is valid, and false otherwise.
  */
  bool IsValid( ON_TextLog* text_log ) const;


  /*
  Description:
    Sets the leaf box to contain bezier curve.
  Parameters:
    bez - [in] Valid bezier curve.
  Returns:
    True if input was valid and the leaf box was set.
  */
  bool Set( const ON_BezierCurve* bez );


  /*
  Description:
    Quickly find a lower bound on the distance 
    between the this leaf box and the other object.
  Parameters:
    P - [in] another point
    bbox - [in] another axis aligned bounding box
    crvleafbox - [in] another curve tree node leaf box
    srfleafbox - [in] another surface tree node leaf box
  Returns:
    A distance that is less than or equal to the shortest
    distance from this leaf box to the other object.
    Put another way, if Q is any point on this leaf box
    and P is any point on the other object, then
    then P.DistanceTo(Q) >= MinimumDistanceTo(other).
  */
  double MinimumDistanceTo( ON_3dPoint P ) const;
  double MinimumDistanceTo( const ON_Line& L ) const;
  double MinimumDistanceTo( const ON_Plane& plane ) const;
  double MinimumDistanceTo( const ON_PlaneEquation& plane_equation ) const;
  double MinimumDistanceTo( const ON_BoundingBox& bbox ) const;
  double MinimumDistanceTo( const ON_CurveLeafBox& crvleafbox ) const;
  double MinimumDistanceTo( const ON_SurfaceLeafBox& srfleafbox ) const;

  /*
  Description:
    Quickly find an upper bound on the longest distance 
    between the this leaf box and the other object.
  Parameters:
    P - [in] another point
    line - [in] another line segment
    bbox - [in] another axis aligned bounding box
    crvleafbox - [in]
    crvleafbox - [in]
  Returns:
    A distance that is greater than or equal to the longest
    distance from this leaf box to the other object.
    Put another way, if Q is any point on this leaf box
    and P is any point on the other object, then
    P.DistanceTo(Q) <= MaximumDistanceTo(other).
  */
  double MaximumDistanceTo( ON_3dPoint P ) const;
  double MaximumDistanceTo( const ON_Line& L ) const;
  double MaximumDistanceTo( const ON_Plane& plane ) const;
  double MaximumDistanceTo( const ON_PlaneEquation& plane_equation ) const;
  double MaximumDistanceTo( const ON_BoundingBox& bbox ) const;
  double MaximumDistanceTo( const ON_CurveLeafBox& crvleafbox ) const;
  double MaximumDistanceTo( const ON_SurfaceLeafBox& srfleafbox ) const;

  /*
  Description:
    Quickly determine if the shortest distance between this
    leaf box and the other object is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    P - [in] another point
    bbox - [in] another axis aligned bounding box
    crvleafbox - [in] another curve tree node leaf box
    srfleafbox - [in] another surface tree node leaf box
  Returns:
    True if the shortest distance from this leaf box to the
    other object is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const ON_3dPoint& P ) const;
  bool IsFartherThan( double d, const ON_Line& L ) const;
  bool IsFartherThan( double d, const ON_Plane& plane ) const;
  bool IsFartherThan( double d, const ON_PlaneEquation& plane_equation ) const;
  bool IsFartherThan( double d, const ON_BoundingBox& bbox ) const;
  bool IsFartherThan( double d, const ON_CurveLeafBox& crvleafbox ) const;
  bool IsFartherThan( double d, const ON_SurfaceLeafBox& srfleafbox ) const;



  /*
  Description:
    Quickly estimate curve and surface parameters for finding 
    closest points.
  Parameters:
    P - [in] another point
    crvleafbox - [in]
    srfleafbox - [in]
    t - [out] curve parameter (0 <= t <= 1).
    c - [out] other curve parameter (0 <= c <= 1)
    s0 - [out] other surface parameter (0 <= s0 <= 1)
    s1 - [out] other surface parameter (0 <= s1 <= 1)
  Returns:
    True if the estimates were calculated.
  */
  bool GetClosestPointSeed( 
            const ON_3dPoint& P, 
            double* t 
            ) const;
  bool GetClosestPointSeed( 
            const ON_CurveLeafBox& crvleafbox, 
            double *t, 
            double* c 
            ) const;
  bool GetClosestPointSeed( 
            const ON_SurfaceLeafBox& srfleafbox, 
            double* t, 
            double* s0, double* s1 
            ) const;


  /*
  Description:
    Point on the chord connecting the bezier's endpoints.
  Parameters:
    t - [in] 0.0 < t < 1.0
  Returns:
    m_L.PointAt(t)
  */
  ON_3dPoint PointAt(double t) const;


  /*
  Description:
    "Radius" of the curve leaf box.  A point is in
    the leaf box if the shortest distance from the
    point to the chord m_M is <= "radius".
  Returns:
    Radius of the leaf box.
  */
  double Radius() const;


  /*
  Description:
    Determine if the point P is in this curve leaf box.
  Parameters:
    P - [in]
  Returns:
    True if the point is in the curve leaf box.
  */
  bool IsPointIn( const ON_3dPoint& P ) const;


  // Below, "C" is the portion of the curve contained in this this
  // curve leaf box.
  ON_Line m_L; // chord from start of C to end of C
  ON_Line m_M; // smallest chord that contains m_L and such that
               // every point on C projects orthogonally
               // to m_M.  For monotone chords, m_L = m_M.
  ON_PlaneEquation m_e; // equation of a plane perpendicular to m_L and containing m_L.from
  double m_e0;      // If P is a point on the curve, then m_e0 <= m_e.ValueAt(P) <= m_e1
  double m_e1;
  double m_r;       // The maximum distance from C to m_M.

  bool m_bValid;    // True if the curve leaf box is set
  bool m_bMono;     // True if m_L.Tangent()oC' >= 0 for every point on C
  bool m_bDegen;    // True if C is yukky
  bool m_bRes;      // NEVER set or reference this value
};

class ON_CLASS ON_CurveTreeBezier : public ON_BezierCurve
{
public:
  // C++ defaults for destruction,
  // copy construction, and operator= work fine.


  ON_CurveTreeBezier();

  /*
  Description:
    Copy constructs the bezier_curve and then sets
    the leaf box.
  */
  ON_CurveTreeBezier(const ON_BezierCurve& bezier_curve );

  /*
  Description:
    Quickly find a lower bound on the distance 
    between the two bezier curves.
  Parameters:
    other - [in] another bezier curve
  Returns:
    A distance that is less than or equal to the shortest
    distance from this curve to the other curve.
    Put another way, if Q is any point on this bezier curve
    and P is any point on the other bezier curve, then
    then P.DistanceTo(Q) >= MinimumDistanceTo(other).
  */
  double MinimumDistanceTo( const ON_CurveTreeBezier& other ) const;
  double MinimumDistanceTo( const ON_SurfaceTreeBezier& other ) const;

  /*
  Description:
    Quickly find an upper bound on the distance 
    between the two bezier curves.
  Parameters:
    other - [in] another bezier curve
  Returns:
    A distance that is greater than or equal to the longest
    distance from this curve to the other curve.
    Put another way, if Q is any point on this bezier curve
    and P is any point on the other bezier curve, then
    then P.DistanceTo(Q) <= MaximumDistanceTo(other).
  */
  double MaximumDistanceTo( const ON_CurveTreeBezier& other ) const;
  double MaximumDistanceTo( const ON_SurfaceTreeBezier& other ) const;

  /*
  Description:
    Quickly determine if the shortest distance from
    the bezier surface to this bezier curve is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    other - [in] 
  Returns:
    True if the shortest distance from the other bezier to
    this bezier curve is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const class ON_CurveTreeBezier& other ) const;
  bool IsFartherThan(double d, const class ON_SurfaceTreeBezier& other) const;
  bool IsFartherThan(double d, const class ON_BoundingBox& box) const;


  /*
  Description:
    Sets the bezier curve and its leaf box.
  Parameters:
    bezier_curve - [in]
  Returns:
    True if bezier_curve is valid.
  See Also:
    ON_CurveTreeBezier::SetLeafBox
  */
  bool SetBezier( const ON_BezierCurve& bezier_curve );

  /*
  Description:
    Sets m_leafbox to be the smallest curve leaf box that
    contains this bezier curve.
  Returns:
    True if this bezier is valid and the leaf box is set.
  */
  bool SetLeafBox();
  
  ON_CurveLeafBox m_leafbox;
};

ON_DECL
ON_CurveTreeBezier* ON_DuplicateCurveTreeBezier( 
  const ON_CurveTreeBezier& src
  );

ON_DECL
ON_CurveTreeBezier* ON_NewCurveTreeBezier( 
  int dim, bool is_rat, int order
  );

ON_DECL
ON_CurveTreeBezier* ON_NewCurveTreeBezier( 
  const ON_BezierCurve& bez
  );

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class ON_CLASS ON_SurfaceLeafBox
{
public:
  ON_SurfaceLeafBox();
  // C++ default copy constructor, operator =, and destructor work fine

  /*
  Description:
    Test the leaf box to make sure its settings are valid.
  Parameters:
    text_log - [in] If an error is found and text_log is not null,
      then a short description of the problem is printed to the
      text_log.
  Returns:
    True if the leaf box is valid, and false otherwise.
  */
  bool IsValid( ON_TextLog* text_log ) const;


  /*
  Description:
    Sets the leaf box to contain bezier surface.
  Parameters:
    bez - [in] Valid bezier surface.
  Returns:
    True if input was valid and leaf box is set.
  */
  bool Set( const ON_BezierSurface* bez );


  /*
  Description:
    Quickly find a lower bound on the distance 
    between the this leaf box and the other object.
  Parameters:
    P - [in] another point
    bbox - [in] another axis aligned bounding box
    crvleafbox - [in] another curve tree node leaf box
    srfleafbox - [in] another surface tree node leaf box
  Returns:
    A distance that is less than or equal to the shortest
    distance from this leaf box to the other object.
    Put another way, if Q is any point on this leaf box
    and P is any point on the other object, then
    then P.DistanceTo(Q) >= MinimumDistanceTo(other).
  */
  double MinimumDistanceTo( const ON_3dPoint& P ) const;
  double MinimumDistanceTo( const ON_Line& L ) const;
  double MinimumDistanceTo( const ON_BoundingBox& bbox ) const;
  double MinimumDistanceTo( const ON_CurveLeafBox& crvleafbox ) const;
  double MinimumDistanceTo( const ON_SurfaceLeafBox& srfleafbox ) const;


  /*
  Description:
    Quickly find an upper bound on the shortest distance 
    between the this leaf box and the other object.
  Parameters:
    P - [in] another point
    bbox - [in] another axis aligned bounding box
    crvleafbox - [in] another curve tree node leaf box
    srfleafbox - [in] another surface tree node leaf box
  Returns:
    A distance that is greater than or equal to the longest
    distance from this leaf box to the other object.
    Put another way, if Q is any point on this leaf box
    and P is any point on the other object, then
    then P.DistanceTo(Q) <= MaximumDistanceTo(other).
  */
  //double MaximumDistanceTo( ON_3dPoint P ) const;
  //double MaximumDistanceTo( const ON_BoundingBox& bbox ) const;
  //double MaximumDistanceTo( const ON_CurveLeafBox& crvleafbox ) const;
  //double MaximumDistanceTo( const ON_SurfaceLeafBox& srfleafbox ) const;


  /*
  Description:
    Quickly determine if the shortest distance between this
    leaf box and the other object is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    P - [in] another point
    bbox - [in] another axis aligned bounding box
    crvleafbox - [in] another curve tree node leaf box
    srfleafbox - [in] another surface tree node leaf box
  Returns:
    True if the shortest distance from this leaf box to the
    other object is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const ON_3dPoint& P ) const;
  bool IsFartherThan( double d, const ON_Line& L ) const;
  bool IsFartherThan( double d, const ON_BoundingBox& bbox ) const;
  bool IsFartherThan( double d, const ON_CurveLeafBox& crvleafbox ) const;
  bool IsFartherThan( double d, const ON_SurfaceLeafBox& srfleafbox ) const;

  /*
  Description:
    Quickly estimate curve and surface parameters for finding 
    closest points.
  Parameters:
    P - [in] another point
    crvleafbox - [in]
    srfleafbox - [in]
    t0 - [out] first surface parameter (0 <= t <= 1).
    t1 - [out] second surface parameter (0 <= t <= 1).
  Returns:
    True if the estimates were calculated.
  See Also:
    ON_CurveLeafBox::GetClosestPointSeed
  */
  bool GetClosestPointSeed( 
            const ON_3dPoint& P, 
            double* t0, double* t1 
            ) const;

  /*
  Description:
    Point on the planar quad/triangle that is the
    "center" of the surface leaf box.
  Parameters:
    s - [in] 0.0 <= s <= 1.0
    t - [in] 0.0 <= t <= 1.0
  Returns:
    point
  */
  ON_3dPoint PointAt(double s, double t) const;


  /*
  Description:
    "Height" of the surface leaf box.
  Returns:
    Height of the surface leaf box.
  */
  double Height() const;


  /*
  Description:
    Determine if the point P is in this surface leaf box.
  Parameters:
    P - [in]
  Returns:
    True if the point is in the surface leaf box.
  */
  bool IsPointIn( const ON_3dPoint& P ) const;


  /*
  Description:
    Expert user function to quickly set m_plane an m_bMono.
  Parameters:
    bez - [in] Valid bezier surface.
    maxheight - [in] Ignore this expert user parameter.
  Returns:
    True if input was valid and m_plane and m_bMono are set.
  See Also
    ON_SurfaceLeafBox::Set    
    ON_SurfaceLeafBox::SetEquationsHelper
  */
  bool SetPlaneAndMonoHelper( const ON_BezierSurface* bez, double maxheight = 0.0 );

  /*
  Description:
    Expert user function to set m_e, m_r, and m_q.
    m_plane and m_bMono must be set before this function
    is called.
  Parameters:
    bez - [in] Valid bezier surface.
  See Also
    ON_SurfaceLeafBox::Set    
    ON_SurfaceLeafBox::SetPlaneAndMonoHelper
  */
  void SetEquationsHelper( const ON_BezierSurface* bez );

  /*
  Description:
    Get the equation of the 3d plane that lies on the side
    of the leaf box.
  Parameters:
    side - [in] 
            0: side from m_q[0] to m_q[1]
            1: side from m_q[1] to m_q[2]
            2: side from m_q[2] to m_q[3]
            3: side from m_q[3] to m_q[0]
            4: bottom
            5: top
    e - [out]  The leaf box lies on the negative side of the plane.
  Returns:
    True if successful.
  */
  bool GetSideEquation( int side, ON_PlaneEquation& e ) const;

  ON_Plane m_plane;

  // Height limits.  m_h[0] <= 0.0 <= m_h[1].  If P is in
  // the surface leaf box, then
  // m_h[0] <= m_plane.plane_equation.ValueAt(P) <= m_h[1].
  double m_h[2];

  // m_c[] = plane coordinates of bounding quad/triangle corners.
  // The polygon connecting the m_c[] points is always convex,
  // but it can happen that the polygon is a triangle.
  // A point is in the surface leaf box if its projection is in the
  // large quad and its distance from the plane is in the
  // interval m_[0] to m_h[1].  The m_c[] are used to
  // answer proximity queries.
  ON_2dPoint m_c[4];

  // m_q[] = plane coordinates of small quad with corners
  // located at the projection of the bibezier corners
  // to m_plane.  In general, the m_q are in the interior
  // of the surface leaf box.  In some cases, the polygon
  // connecting the m_q[] is not convex.  m_q[] are used
  // to calculate seed values.
  ON_2dPoint m_q[4];

  // side plane equations 
  //   d = m_e[i].x*s + m_e[i].y*t + m_e[i].z
  //   d is zero at m_c[i] and m_c[(i+1)%4].
  //   d > 0 in the half space outside the surface leaf
  //   d is the 3d distance from the point m_plane
  //   to the bounding side plane.
  ON_3dVector m_e[4];

  bool m_bValid;
  bool m_bMono;     
  bool m_bDegen; // True if the m_q[] are not convex (the m_c[] are always convex)
  bool m_bPlane;
};


class ON_CLASS ON_SurfaceTreeBezier : public ON_BezierSurface
{
public:
  // C++ defaults for destruction,
  // copy construction, and operator= work fine.

  ON_SurfaceTreeBezier();

  ON_SurfaceTreeBezier( const ON_BezierSurface& bezier_surface );

  /*
  Description:
    Quickly find a lower bound on the distance from a 
    point to this bezier surface.
  Parameters:
    P - [in]
  Returns:
    A distance that is less than or equal to the shortest
    distance from P to a point on the bezier surface.
    Put another way, if Q is any point on the bezier surface, 
    then P.DistanceTo(Q) >= MinimumDistanceTo(P);
  */
  double MinimumDistanceTo( ON_3dPoint P ) const;

  /*
  Description:
    Quickly find an upper bound on the distance
    from a point to this bezier surface.
  Parameters:
    pt - [in]
  Returns:
    A distance that is greater than or equal to the longest
    distance from P to a point on the bezier surface.  
    Put another way, if Q is any point on the bezier surface, 
    then P.DistanceTo(Q) <= MaximumDistanceTo(P);
  */
  double MaximumDistanceTo( ON_3dPoint P ) const;

  /*
  Description:
    Quickly find a lower bound on the distance 
    between the bezier surface and a bounding box.
  Parameters:
    bbox - [in]
  Returns:
    A distance that is less than or equal to the shortest
    distance from this surface to the bounding box.
    Put another way, if Q is any point on this bezier surface
    and P is any point in bbox, then
    P.DistanceTo(Q) >= MinimumDistanceTo(bbox).
  */
  double MinimumDistanceTo( ON_BoundingBox bbox ) const;

  /*
  Description:
    Quickly find an upper bound on the distance 
    between the bezier surface and a bounding box.
  Parameters:
    bbox - [in]
  Returns:
    A distance that is greater than or equal to the longest
    distance from this surface to the bounding box.
    Put another way, if Q is any point on this bezier surface
    and P is any point in bbox, then
    P.DistanceTo(Q) <= MaximumDistanceTo(bbox).
  */
  double MaximumDistanceTo( ON_BoundingBox bbox ) const;

  /*
  Description:
    Quickly find a lower bound on the distance 
    between this surface and a bezier curve.
  Parameters:
    other - [in] a bezier curve
  Returns:
    A distance that is less than or equal to the shortest
    distance from this surface to the curve.
    Put another way, if Q is any point on this bezier surface
    and P is any point on the  bezier curve, then
    then P.DistanceTo(Q) >= MinimumDistanceTo(other).
  */
  double MinimumDistanceTo( const ON_CurveTreeBezier& other ) const;

  /*
  Description:
    Quickly find an upper bound on the distance 
    between this surface and a bezier curve.
  Parameters:
    other - [in] a bezier curve
  Returns:
    A distance that is greater than or equal to the longest
    distance from this surface to the curve.
    Put another way, if Q is any point on this bezier surface
    and P is any point on the bezier curve, then
    then P.DistanceTo(Q) <= MaximumDistanceTo(other).
  */
  double MaximumDistanceTo( const ON_CurveTreeBezier& other ) const;

  /*
  Description:
    Quickly find a lower bound on the distance 
    between a pair of bezier surfaces.
  Parameters:
    stbez - [in] other bezier surface
  Returns:
    A distance that is less than or equal to the shortest
    distance from this surface to the other surface.
    Put another way, if Q is any point on this bezier surface
    and P is any point on the  bezier surface, then
    then P.DistanceTo(Q) >= MinimumDistanceTo(other).
  */
	double MinimumDistanceTo( const class ON_SurfaceTreeBezier& stbez ) const;

  /*
  Description:
    Quickly find an upper bound on the distance 
    between a pair of bezier surfaces.
  Parameters:
    stbez - [in] other surface
  Returns:
    A distance that is greater than or equal to the longest
    distance from this surface to the other.
    Put another way, if Q is any point on this bezier surface
    and P is any point on the othr bezier surface, then
    then P.DistanceTo(Q) <= MaximumDistanceTo(other).
  */ 
	double MaximumDistanceTo( const class ON_SurfaceTreeBezier& stbez ) const;

  /*
  Description:
    Quickly determine if the shortest distance from
    the point P to this bezier curve is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    P - [in] 
  Returns:
    True if the shortest distance from the point P to
    this bezier curve is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const ON_3dPoint& P ) const;

  /*
  Description:
    Quickly determine if the shortest distance from
    the bounding box to this bezier curve is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    bbox - [in] 
  Returns:
    True if the shortest distance from the bounding box to
    this bezier curve is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const ON_BoundingBox& bbox ) const;

  /*
  Description:
    Quickly determine if the shortest distance from
    the other bezier to this bezier curve is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    other - [in] 
  Returns:
    True if the shortest distance from the other bezier to
    this bezier curve is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const ON_CurveTreeBezier& other ) const;

  /*
  Description:
    Quickly determine if the shortest distance from
    the other bezier to this bezier curve is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    other - [in] 
  Returns:
    True if the shortest distance from the other bezier to
    this bezier curve is greater than d. It is not the case that
    false  means that the shortest distance is less than 
    or equal to d.
  */
  bool IsFartherThan( double d, const ON_SurfaceTreeBezier& other ) const;


  /*
  Description:
    Copies bezier_surface and sets m_leafbox to be the smallest
    surface leaf box that contains the bezier surface.
  Returns:
    True if bezier_surface is valid and the leaf box is set.
  */
  bool SetBezier( const ON_BezierSurface& bezier_surface );

  /*
  Description:
    Sets m_leafbox to be the smallest surface leaf box that
    contains the bezier surface.
  Returns:
    True if this bezier is valid and the leaf box is set.
  */
  bool SetLeafBox();

  ON_SurfaceLeafBox m_leafbox;
};


ON_DECL
ON_SurfaceTreeBezier* ON_DuplicateSurfaceTreeBezier( 
  const ON_SurfaceTreeBezier& src
  );

ON_DECL
ON_SurfaceTreeBezier* ON_NewSurfaceTreeBezier( 
  int dim, bool is_rat, int order0,  int order1
  );

ON_DECL
ON_SurfaceTreeBezier* ON_NewSurfaceTreeBezier( 
  const ON_BezierSurface& bez
  );

ON_DECL
bool ON_CvxHullFromLeafBox(ON_ConvexHullPoint2& Hull, const ON_SurfaceLeafBox& leaf);


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// A curve cache is a simple binary tree of bezier curves
// and is used to speed up geometric calculations.

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_X_EVENT>;
#endif

class ON_CLASS ON_CurveTreeNode
{
public:
  ON_CurveTreeNode();
  ~ON_CurveTreeNode();


  bool IsValid( 
    ON_TextLog* text_log = 0,
    int level = 0,
    int side = -1,
    const ON_Curve* curve = 0
    ) const;

  /*
  Description:
    Starting at this node, find the leaf node whose domain
    contains that parameter t.
  Parameters:
    t - [in] 
    side [in] If t lies at the start/end of a domain, side
       controls which leaf is returned.  If side >= 0,
       the node with domain (t,...) is returned.  If side < 0,
       the node with domain (...,t) is returned.
  Returns:
    Pointer to the node that contains t.  If t < the start
    of this node's domain, the node for the start
    is returned.  If t > the end of this node's domain,
    the node for the end parameter is returned.
  */
  const ON_CurveTreeNode* FindLeaf( 
    double t,
    int side = 0
    ) const;


  /*
  Description:
    Evaluate the portion of the curve covered by this
    node.
  Parameters:
    t - [in] evaluation parameter with respect to the
             node's domain.
    der_count - [in] (>=0) number of derivatives to evaluate
    v_stride - [in] stride to use for the v[] array
    v - [out] array of length (der_count+1)*v_stride
        curve(t) is returned in (v[0],...,v[m_dim-1]),
        curve'(t) is returned in (v[v_stride],...,v[v_stride+m_dim-1]),
        curve"(t) is returned in (v[2*v_stride],...,v[2*v_stride+m_dim-1]),
        etc.
    side - [in] optional - determines which side to evaluate from
                =0   default (from above)
                <0   to evaluate from below, 
                >0   to evaluate from above
  Returns:
    If successful, returns a pointer to the leaf node that was used
    in the evaluation.  If null is returned, the evaluation
    could not be performed.
  Remarks:
    If the evaluation parameter is outside this node's domain,
    the bezier on the appropriate end is used to ev
  */
  const ON_CurveTreeNode* Evaluate(
         double t,
         int der_count,
         int v_stride,
         double* v,
         int side = 0
         ) const;

  /*
  Description:
    Used to determine if the node's parametization differs from
    the tree's curve parametization.  If the parameterizations
    are different, then use 
    ON_Curve::GetCurveParameterFromNurbFormParameter
    to convert node parameters to curve parameters and
    ON_Curve::GetCurveParameterFromNurbFormParameter
    to convert curve parameters to node parameters.
  Returns:
    True if the node's parametization differs from
    the tree's curve parametization.
  See Also:
    ON_Curve::GetCurveParameterFromNurbFormParameter
    ON_Curve::GetCurveParameterFromNurbFormParameter
  */
  bool AdjustParameter() const;

	// Set from a bezier or from the defining data for a bezier. Sets
	//	m_bbox, m_bez,  m_radius.   Leaves m_domain unchanged.
	bool SetBezier( const ON_BezierCurve& Crv);

	bool SetBezier( int dim, bool is_rat, int order, const double* cv, int stride);

	// m_domain records the portion of the curve's domain that
  // this node defines.  It is always increasing an included
  // in the curve's domain.  The parameters are NURBS form
  // parameters.
  ON_Interval     m_domain;

  // Bounding box of the portion of the curve with domain
  // [m_domain.Min(), m_domain.Max()]
  ON_BoundingBox  m_bbox;

  // All leaf nodes have a bezier. Non-leaf nodes may have a nullptr here.
  // The domain of m_bez is always [0,1] and m_domain records the portion
  // of the curve that this bezier curve corresponds to.
  ON_CurveTreeBezier* m_bez;

  // m_up moves towards the root node.  If m_up is nullptr, the node
  // is the root node.
  ON_CurveTreeNode*  m_up;

  // m_down[] moves towards the leaf nodes.  Either both m_down[0]
  // and m_down[1] are nullptr, or both are not nullptr.  m_down[0]
  // covers the portion of the curve from m_domain.Min() to m_domain.Mid().
  // m_down[1] covers the portion of the curve from m_domain.Mid() 
  // to m_domain.Max().
  ON_CurveTreeNode*  m_down[2];

  // Next() moves to the next leaf interval in the curve tree if this
  // node is a leaf node.
  ON_CurveTreeNode*  NextLeaf() const;

  // Prev() moves to the previous leaf interval in the curve tree if this
  // node is a leaf node.
  ON_CurveTreeNode*  PrevLeaf() const;


  /*
  Description:
    Quickly find a lower bound on the distance from this
    curve node to the other object.
  Parameters:
    P - [in]
    other - [in]
  Returns:
    A distance that is less than or equal to the shortest
    distance from this curve node to the other object.
    Put another way, if Q is any point Q on the portion
    of the curve covered by this node and P is any point
    on the other object, then 
    P.DistanceTo(Q) >= MinimumDistanceTo();
  */
  double MinimumDistanceLowerBound( ON_3dPoint P ) const;
  double MinimumDistanceLowerBound( const ON_CurveTreeNode* other ) const;
  double MinimumDistanceLowerBound( const ON_SurfaceTreeNode* other ) const;

  /*
  Description:
    Quickly find an upper bound on the shortest distance 
    from this curve node to the other object.
  Parameters:
    P - [in]
    other - [in]
  Returns:
    A distance that is greater than or equal to the shortest
    distance from this curve node to the other object.
    Put another way, there exists a point Q on the portion
    of the curve covered by this node and a point P on the
    other object such that
    P.DistanceTo(Q) <= MinimumDistanceUpperBound();
  */
  double MinimumDistanceUpperBound( ON_3dPoint P ) const;
  double MinimumDistanceUpperBound( const ON_CurveTreeNode* other ) const;
  double MinimumDistanceUpperBound( const ON_SurfaceTreeNode* other ) const;

  /*
  Description:
    Quickly find an upper bound on the distance from a 
    P to the portion of the curve covered by this node.
  Parameters:
    P - [in]
  Returns:
    A distance that is greater than or equal to the longest
    distance from P to a point on the curve covered by this
    node. Put another way, if Q is any point on the portion
    of the curve covered by this node, then
    P.DistanceTo(Q) <= MaximumDistanceTo(P);
  */
  double MaximumDistanceUpperBound( ON_3dPoint P ) const;
  double MaximumDistanceUpperBound( const ON_CurveTreeNode* other ) const;
  double MaximumDistanceUpperBound( const ON_SurfaceTreeNode* other ) const;

  /*
  Description:
    Quickly determine if the shortest distance from
    the point P to the portion of the curve covered by
    this node is greater than d.
  Parameters:
    d - [in] distance (> 0.0)
    P - [in] 
    other - [in] 
  Returns:
    True if if the shortest distance from the point P
    to the portion of the curve covered by this node
    is greater than d.
  Remarks:
    This is conceptually the same but faster than calling 
    MinimumDistanceUpperBound(other) > d.
  */
  bool IsFartherThan( double d, const ON_PlaneEquation& e ) const;
  bool IsFartherThan( double d, const ON_3dPoint& P ) const;
  bool IsFartherThan( double d, const ON_CurveTreeNode* other ) const;
  bool IsFartherThan( double d, const ON_SurfaceTreeNode* other ) const;

  bool IsFartherThanAlt(double d, const ON_SurfaceTreeNode* other) const;   // todo is this really the final soln

  // Returns the smallest subtree that contain the given interval.
  // Returns nullptr if range is not contained the domain of this subtree.
	const ON_CurveTreeNode* TreeContaining( ON_Interval range) const;

  // Returns the smallest subtree that contain the given semi-open interval
  // with beginning, (or ending respectively) at t0 when side =0 or 1 ( or side = -1 ).
  // Returns nullptr if range is not contained the domain of this subtree.
  const ON_CurveTreeNode* TreeContaining(double t0, int side) const;

  /*
  Description:
    Expert user tool to split a curve tree node.  Does not modify 
    this node's m_down[] pointers or left and right's m_up pointers.
    Care must be take to guard against memory leaks and double deletes.
  Parameters:
    s - [in] normalized (bezier parameter) 0 < s < 1
             The corresponding surface domain parameter is
             m_domain.ParameterAt(s).
    left_node - [in] node to hold the left half.
    right_node - [in] node to hold the right half.
  Returns:
    True if input is valid and split was performed.
  */
  bool Split(
          double s, 
          ON_CurveTreeNode& left_node, 
          ON_CurveTreeNode& right_node
          ) const;

  const ON_CurveTreeNode* GetClosestPoint( 
          ON_3dPoint P,
          double* t,
          ON_3dPoint* closestpt = 0,
          double maximum_distance = 0.0,
          const ON_Interval* cdomain = 0
          ) const;

  int IntersectCurve( 
          const class ON_CurveTreeNode* cnodeB,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curveA_domain = 0,
          const ON_Interval* curveB_domain = 0
          ) const;

  int IntersectSelf( 
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          const ON_Interval* curve_domain = 0
          ) const;

  int IntersectSurface( 
          const class ON_SurfaceTreeNode* snodeB,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curveA_domain = 0,
          const ON_Interval* surfaceB_udomain = 0,
          const ON_Interval* surfaceB_vdomain = 0
          ) const;

  int IntersectPlane( 
          const class ON_PlaneEquation& plane_equation,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curve_domain = 0
          ) const;

  int IntersectPlane( 
          const class ON_Plane& plane,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curve_domain = 0
          ) const;


  /*
	Description:
    Get tight bounding box of the portion of the curve covered
    by this node.
	Parameters:
		tight_bbox - [in/out] tight bounding box
		bGrowBox -[in]	(default=false)			
      If true and the input tight_bbox is valid, then returned
      tight_bbox is the union of the input tight_bbox and the 
      node's tight bounding box.
		xform -[in] (default=nullptr)
      If not nullptr, the tight bounding box of the transformed
      node is calculated.  The node is not modified.
	Returns:
    True if the returned tight_bbox is set to a valid 
    bounding box.
  */
	bool GetTightBoundingBox( 
			ON_BoundingBox& bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const;


  unsigned int m_treesn;     // serial number of this tree

  unsigned int m_nodesn;     // serial number of this node

  // Memory management accounting information
  unsigned char m_nodetype;  // 8 bits to flag special nodes
                             // 1 = root node
                             // 2 = meta node (no domain)
                             // 4 = reparam
  unsigned char m_nodemem;   // 0 = do not destroy
                             // 1 = destroy with delete
                             // 2 = destroy with free
  unsigned char m_bezmem;    // 0 = do not destroy
                             // 1 = destroy with delete
                             // 2 = destroy with free
  unsigned char m_reserved_ON_CurveTreeNode[5]; // keep class size a multiple of 16

private:
	// Prohibit copy construction and operator=
	ON_CurveTreeNode(const ON_CurveTreeNode& orig); // NO IMPLEMENTATION
	ON_CurveTreeNode& operator=(const ON_CurveTreeNode& orig); // NO IMPLEMENTATION
};

class ON_CLASS ON_CurveTree : public ON_CurveTreeNode
{
public:
  ON_CurveTree();
  ~ON_CurveTree();

  bool IsValid( 
    ON_TextLog* text_log = 0,
    const ON_Curve* curve = 0
    ) const;
  
  // root node of the binary tree
  ON_CurveTreeNode* m_root; // often points to "this"

  // First() returns the first leaf interval in the curve tree.
  ON_CurveTreeNode* FirstLeaf() const;

  // Last() returns the last leaf interval in the curve tree.
  ON_CurveTreeNode* LastLeaf() const;

  /*
  Description:
    Initialize the tree based on a NURBS curve.
  Parameters:
    nurbs_curve - [in]
    max_span_depth - [in] advanced control - use default
    maxar - [in] advanced control - use default
    mp - [in] memory pool
  Returns:
    True if successful
  */
  bool CreateTree( 
          const ON_NurbsCurve& nurbs_curve,
          int max_span_depth = 0,
          double maxar = 0.0
          );

  /*
  Description:
    Initialize the tree based on a bezier curve.
  Parameters:
    bez_curve - [in]
    domain - [in] used to set the node's domain fields.
                  (This does not restrict the portion
                  of the bezier the tree covers.)
    max_span_depth - [in] advanced control - use default
    maxar - [in] advanced control - use default
    mp - [in] memory pool
  Returns:
    True if successful
  */
  bool CreateTree( 
          const ON_BezierCurve& bez_curve,
          const ON_Interval* node_domain = 0,
          int max_span_depth = 0,
          double maxar = 0.0
          );

  /*
  Description:
    Destroy existing information.  Leaves tree in a state where it can
    be reused for another curve.
  */
  void DestroyTree();

  /*
  Description:
    Expert user tool to get a curve tree interior node.  This node
    will be destroyed by this tree's destructor.
  Returns:
    A surface tree node.  Never delete or free the returned pointer.
  */
  ON_CurveTreeNode* NewNode();

  /*
  Description:
    Expert user tool to get a curve tree leaf node.  This node
    will be destroyed by this tree's destructor.
  Returns:
    A surface tree node with m_bez and m_bez->m_cv.  Never
    delete or free the returned pointer, m_bez, or m_bez->m_cv.
  */
  ON_CurveTreeNode* NewLeaf();

  /*
  Description:
    Expert user tool to return a node for reuse that was 
    created by calling this curve tree's NewNode or NewLeaf.
  Parameters:
    node - [in]
  Returns:
    True if the node was successfully returned.  If false,
    then the node has been damaged or was not created by
    calling this curve tree's NewNode() or NewLeaf().
  */
  bool ReturnNode(ON_CurveTreeNode* node);

private:
  void Startup( int leaf_count, int dim, bool is_rat, int order );
  class ON__CurveTreeMem* m__p; 
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// A surface tree is a simple binary tree of bezier surfaces
// and is used to speed up geometric calculations.
#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_SSX_EVENT>;
#endif

class ON_CLASS ON_SurfaceTreeNode
{
public:
  ON_SurfaceTreeNode();
  ~ON_SurfaceTreeNode();


  bool IsValid( 
    ON_TextLog* text_log = 0,
    int level = 0,
    int side = -1,
    const class ON_Surface* surface = 0
    ) const;


  /*
  Description:
    Starting at this node, find the leaf node whose domain
    contains that parameter (s,t).
  Parameters:
    t - [in] 
    quadrant - [in] optional - 1,2,3 or 4
                Determines which quadrant to use
                from when (s,t) is on the side of a leaf node.
                Values < 1 or > 4 are treated as 1
  Returns:
    Pointer to the node that contains (s,t).
    If (s,t) is outside this node's domain, then the
    leaf closest to (s,t) is returned.
  */
  const ON_SurfaceTreeNode* FindLeaf( 
    double s,
    double t,
    int quadrant = 0
    ) const;

  /*
  Description:
    Evaluate the portion of the surface covered by this
    node.
  Parameters:
    s - [in]
    t - [in] (s,t) - evaluation parameters with respect to this
             node's domain.
    der_count - [in] (>=0) number of derivatives to evaluate
    v_stride - [in] stride to use for the v[] array
    v - [out] array of length (der_count+1)*v_stride
        surface(s,t) is returned in (v[0],...,v[m_dim-1]),
        Ds is returned in (v[v_stride],...,v[v_stride+m_dim-1]),
        Dt is returned in (v[2*v_stride],...,v[2*v_stride+m_dim-1]),
        Dss is returned in (v[3*v_stride],...),
        Dst is returned in (v[4*v_stride],...),
        Dtt is returned in (v[5*v_stride],...),
        etc.
    quadrant - [in] optional - 1,2,3 or 4
                Determines which quadrant to evaluate
                from when (s,t) is on the side of a leaf node.
                Values < 1 or > 4 are treated as 1
  Returns:
    If successful, returns a pointer to the leaf node that was used
    in the evaluation.  If null is returned, the evaluation
    could not be performed.
  Remarks:
    If (s,t) is out outside this node's domain,
    the bezier on the appropriate side is evaluated.
  */
  const ON_SurfaceTreeNode* Evaluate(
         double s,
         double t,
         int der_count,
         int v_stride,
         double* v,
         int quadrant = 0
         ) const;


  /*
  Description:
    Used to determine if the node's parametization differs from
    the tree's surface parametization.  If the parameterizations
    are different, then use 
    ON_Surface::GetSurfaceParameterFromNurbFormParameter
    to convert node parameters to surface parameters and
    ON_Surface::GetNurbFormParameterFromSurfaceParameter 
    to convert surface parameters to node parameters.
  Returns:
    True if the node's parametization differs from
    the tree's surface parametization.
  See Also:
    ON_Surface::GetSurfaceParameterFromNurbFormParameter
    ON_Surface::GetNurbFormParameterFromSurfaceParameter
  */
  bool AdjustParameter() const;

  // m_domain records the portion of the surface's domain that
  // this node defines.  It is always increasing.
  ON_Interval m_domain[2];

  // Bounding box of the portion of the surface with domain
  // [m_domain.Min(), m_domain.Max()]
  ON_BoundingBox  m_bbox;

  // All leaf nodes have a bezier. Non-leaf nodes may have a nullptr here.
  // The domain of m_bez is always [0,1]x[0,1] and m_domain[] records 
  // the portion  of the surface that this bezier surface corresponds to.
  ON_SurfaceTreeBezier* m_bez;

  // m_up moves towards the root node.  If m_up is nullptr, the node
  // is the root node.
  ON_SurfaceTreeNode*  m_up;

  // m_down[] moves towards the leaf nodes.  Either both m_down[0]
  // and m_down[1] are nullptr, or both are not nullptr.  If m_split_dir=0,
	// m_domain is partitioned into a left portion m_down[0]->m_domain 
	// and a right portion m_down[1]->m_domain.  If m_split_dir=1
	// m_domain is partitioned into a lower portion m_down[0]->m_domain
	// and an upper portion m_down[1]->m_domain.
  ON_SurfaceTreeNode*  m_down[2];

  // Next() moves to the next leaf interval in the surface tree if this
  // node is a leaf node.
  ON_SurfaceTreeNode*  NextLeaf() const;

  // Prev() moves to the previous leaf interval in the curve tree if this
  // node is a leaf node.
  ON_SurfaceTreeNode*  PrevLeaf() const;

  /*
  Description:
    Find the smallest node that contains the specified parameter range.
  Parameters:
    Urange - [in]
    Vrange - [in]
  Returns:
    A pointer to the smallest child node (possibly this) that contains
    the specified parameter range or nullptr if no such node exists.
  */
	ON_SurfaceTreeNode* TreeContaining( 
          const ON_Interval& Urange,  
          const ON_Interval& Vrange
          ) const;

	// For any SurfaceTreeNode, not just leafs, the NorthNode(), SouthNode(), EastNode()
	// and WestNode() functions returns a pointer to the SurfaceTreeNode of the neighboring
	// region.  For example, for any SurfaceTreeNode N with non-null N.NorthNode() we have 
	// the relations
	//		 N.NorthNode()->Domain[0].Includes( 	N.Domain[0]  ) and 
	//		 N.NorthNode()->Domain[1][0] == N.Domain[1][1]
	// Remark: The periodicity of the surface is not considered.  So for example every surface has a 
	// northern most leaf for which NorthSide() will return nullptr.
	ON_SurfaceTreeNode*  NorthNode() const;
  ON_SurfaceTreeNode*  SouthNode() const;
  ON_SurfaceTreeNode*  EastNode() const;
  ON_SurfaceTreeNode*  WestNode() const;

  /*
  Description:
    Reports the direction this node was split or
    would be split.
  Parameters:
  Returns:
    0: the node was split in the "u" direction
    1: the node was split in the "v" direction
   -1: otherwise
  */
  int SplitDir() const;

  /*
  Description:
    Quickly find a lower bound on the distance from a 
    P to the portion of the curve covered by this node.
  Parameters:
    P - [in]
  Returns:
    A distance that is less than or equal to the shortest
    distance from P to a point on the curve covered by this
    node. Put another way, if Q is any point on the portion
    of the curve covered by this node, then
    P.DistanceTo(Q) >= MinimumDistanceTo(P);
  */
  double MinimumDistanceLowerBound( ON_3dPoint P ) const;
  double MinimumDistanceLowerBound( const ON_CurveTreeNode* other ) const;
  double MinimumDistanceLowerBound( const ON_SurfaceTreeNode* other ) const;

  /*
  Description:
    Quickly find an upper bound on the shortest distance 
    from this curve node to the other object.
  Parameters:
    P - [in]
    other - [in]
  Returns:
    A distance that is greater than or equal to the shortest
    distance from this surface node to the other object.
    Put another way, there exists a point Q on the portion
    of the surface covered by this node and a point P on the
    other object such that
    P.DistanceTo(Q) <= MinimumDistanceUpperBound();
  */
  double MinimumDistanceUpperBound( ON_3dPoint P ) const;
  double MinimumDistanceUpperBound( const ON_CurveTreeNode* other ) const;
  double MinimumDistanceUpperBound( const ON_SurfaceTreeNode* other ) const;


  /*
  Description:
    Quickly find an upper bound on the longest distance this
    surface node to the other object.
  Parameters:
    P - [in]
    other - [in]
  Returns:
    A distance that is greater than or equal to the longest
    distance from this surface node to the other object.
    Put another way, if Q is any point on the portion
    of the surface covered by this node and P is any point
    on the other object, then
    P.DistanceTo(Q) <= MaximumDistanceTo(P);
  */
  double MaximumDistanceUpperBound( ON_3dPoint P ) const;
  double MaximumDistanceUpperBound( const ON_CurveTreeNode* other ) const;
  double MaximumDistanceUpperBound( const ON_SurfaceTreeNode* other ) const;

  /*
  Description:
    Quickly determine if the shortest distance from the portion of the
    surface covered by this node to the other object is greater
    than d.
  Parameters:
    d - [in] distance (> 0.0)
    P - [in] 
    other - [in]
  Returns:
    True if if the shortest distance from this node to the other
    object is greater than d.  Put another way, true if returned
    if for every point Q on the portion of the surface covered 
    by this node and for every point P on the other object
    Q.DistanceTo(P) > d.
  Remarks:
    This is conceptually the same but faster than calling
    MinimumDistanceUpperBound(other) > d.
  */
  bool IsFartherThan( double d, const ON_3dPoint& P ) const;
  bool IsFartherThan( double d, const ON_CurveTreeNode* other ) const;
  bool IsFartherThan( double d, const ON_SurfaceTreeNode* other ) const;

  /*
  Description:
    Expert user tool to split a surface tree node.  Does not modify 
    this node's m_down[] pointers or left and right's m_up pointers.
    Care must be take to gaurd against memory leaks and double deletes.
  Parameters:
    dir - [in] 0 = split first parameter direction
               1 = split second parameter direction
    s - [in] normalized (bezier parameter) 0 < s < 1
             The corresponding surface domain parameter is
             m_domain[dir].ParameterAt(s).
    left_node - [in] node to hold the left half.
    right_node - [in] node to hold the right half.
  Returns:
    True if input is valid and split was performed.
  */
	bool Split( 
          int dir, 
          double s, 
          ON_SurfaceTreeNode& left_node, 
          ON_SurfaceTreeNode& right_node
          ) const;

  const ON_SurfaceTreeNode* GetClosestPoint( 
          ON_3dPoint P,
          double* s,
          double* t,
          ON_3dPoint* closestpt = 0,
          double maximum_distance = 0.0,
          const ON_Interval* sdomain = 0,
          const ON_Interval* tdomain = 0
          ) const;

  int IntersectSurface( 
        const class ON_SurfaceTreeNode* snodeB,
        ON_ClassArray<ON_SSX_EVENT>& x,
        double intersection_tolerance = 0.0,
        double overlap_tolerance = 0.0,
        double fitting_tolerance = 0.0,
        const ON_Interval* surfaceA_udomain = 0,
        const ON_Interval* surfaceA_vdomain = 0,
        const ON_Interval* surfaceB_udomain = 0,
        const ON_Interval* surfaceB_vdomain = 0
        ) const;


  /*
  Description:
    Test normalized span parameters to see if they are near 
    a singularity.
  Parameters:
    s0 - [in] 0 <= s1 <= 1 
    s1 - [in] 0 <= s1 <= 1
		side - [out]  If true south, 1 = east, 2 = north, 3 = west
		tolerance of normalized parameter
  Returns:
    True if the parameters are near a singularity.
  */
  bool IsNearSingularity( double s0, double s1 ) const;
  bool IsNearSingularity( double s0, double s1 , int& side, double tol=1e-5) const;

  unsigned int m_treesn;     // serial number of this tree

  unsigned int m_nodesn;     // serial number of this node

  // Memory management accounting information
  unsigned char m_nodetype;  //  8 bits to flag special nodes
                             //      1 = root node
                             //      2 = meta node (no domain)
                             //      4 = beizer parameterization != surface parameterization
                             //      8 = south side is singular point
                             //     16 = east  side is singular point
                             //     32 = north side is singular point
                             //     64 = west  side is singular point
  unsigned char m_nodemem;   // 0 = do not destroy
                             // 1 = destroy with delete
                             // 2 = destroy with free
  unsigned char m_bezmem;    // 0 = do not destroy
                             // 1 = destroy with delete
                             // 2 = destroy with free
  unsigned char m_reserved_ON_SurfaceTreeNode[5]; // keep sizeof() a multiple of 16

private:
	ON_SurfaceTreeNode* NeighborNodeHelper( int ) const;

private:
	// Prohibit copy construction and operator=
	ON_SurfaceTreeNode(const ON_SurfaceTreeNode& orig); // NO IMPLEMENTATION
	ON_SurfaceTreeNode& operator=(const ON_SurfaceTreeNode& orig);  // NO IMPLEMENTATION
};


class ON_CLASS ON_SurfaceTree : public ON_SurfaceTreeNode
{
public:
  ON_SurfaceTree();
  ~ON_SurfaceTree();
  
  bool IsValid( 
    ON_TextLog* text_log = 0,
    const class ON_Surface* surface = 0
    ) const;

  // root node of the binary tree
  ON_SurfaceTreeNode* m_root; // often points to "this"

  ON_SurfaceTreeNode* Root() const;

  // First() returns the first leaf interval in the curve tree.
  ON_SurfaceTreeNode* FirstLeaf() const;

  // Last() returns the last leaf interval in the curve tree.
  ON_SurfaceTreeNode* LastLeaf() const;


  /*
  Description:
    Initialize the tree based on a NURBS surface.
  Parameters:
    nurbs_surface - [in]
    max_span_depth - [in] advanced control - use default
    maxar - [in] advanced control - use default
    mp - [in] memory pool
  Returns:
    True if successful
  */
  bool CreateTree( 
          const ON_NurbsSurface& nurbs_surface,
          int max_span_depth = 0,
          double maxar = 0.0
          );

  /*
  Description:
    Initialize the tree based on a bezier surface.
  Parameters:
    bez_curve - [in]
    node_sdomain - [in] used to set the node's domain fields.
                  (This does not restrict the portion
                  of the bezier the tree covers.)
    node_tdomain - [in] used to set the node's domain fields.
                  (This does not restrict the portion
                  of the bezier the tree covers.)
    max_span_depth - [in] advanced control - use default
    maxar - [in] advanced control - use default
    mp - [in] memory pool
  Returns:
    True if successful
  */
  bool CreateTree( 
          const ON_BezierSurface& bez_surface,
          const ON_Interval* node_sdomain = 0,
          const ON_Interval* node_tdomain = 0,
          int max_span_depth = 0,
          double maxar = 0.0
          );

  /*
  Description:
    Creates a trunk over a collection of nodes.
    Useful when calculating intersections and closest
    points to a group of surfaces.
  Parameters:
    leaf_count - [in]
    leaves - [in]
      These should be sorted.  Neighbors in the list will
      end up as children of a parent node.
    mp - [in] memory pool
  Returns:
    True if successful.
  */
  bool CreateTreeTrunk(
          int leaf_count,
          const ON_SurfaceTreeNode* const * leaves
          );

  /*
  Description:
    Destroy existing information.  Leaves tree in a state where it can
    be reused for another curve.
  */
  void DestroyTree();

  /*
  Description:
    Expert user tool to get a surface tree interior node.  This node
    will be destroyed by this tree's destructor.
  Returns:
    A surface tree node.  Never delete or free the returned pointer.
  */
  ON_SurfaceTreeNode* NewNode();

  /*
  Description:
    Expert user tool to get a surface tree leaf node.  This node
    will be destroyed by this tree's destructor.
  Returns:
    A surface tree node with m_bez and m_bez->m_cv.  Never
    delete or free the returned pointer, m_bez, or m_bez->m_cv.
  */
  ON_SurfaceTreeNode* NewLeaf();

  /*
  Description:
    Expert user tool to return a node for reuse that was 
    created by calling this curve tree's NewNode or NewLeaf.
  Parameters:
    node - [in]
  Returns:
    True if the node was successfully returned.  If false,
    then the node has been damaged or was not created by
    calling this surface tree's NewNode() or NewLeaf().
  */
  bool ReturnNode(ON_SurfaceTreeNode* node);

private:
  void Startup( int leaf_count, int dim, bool is_rat, int order0, int order1 );
  class ON__SurfaceTreeMem* m__p; 
};

class ON_CLASS ON_RayShooter
{
public:
  ON_RayShooter();
  ON_RayShooter(const ON_RayShooter& src);
  ON_RayShooter& operator=(const ON_RayShooter& src);

  /*
  Description:
    Shoot a ray at a surface or collection of surfaces.
  Parameters:
    P - [in] start point
    D - [in] direction of infinite ray
    surface - [in]
    suface_list - [in]
    snode -[in]
    snode_list - [in]
    hit - [in/out]
      If hit.m_type = ON_X_EVENT::csx_point and 0.0 < hit.m_a[0],
      then a new hit will be returned only if it closer to the 
      start of the ray.
  Example:
    Shoot a ray at a collection of "mirrors" and let it reflect.

          ON_3dPoint P = ...; // ray starting point
          ON_3dVector D = ...; // ray direction
          int max_reflection_count = 100; // number of reflections
          ON_SimpleArray<const ON_Surface*> surface_list = ...;

          // polyline = path of reflected ray
          ON_Polyline polyline(m_max_bounce_count+1);
          polyline.Append(P);

          // shoot ...
          ON_RayShooter ray;
          ON_X_EVENT hit;
          ON_3dPoint Q;
          ON_3dVector V[3], N, T, R;
          double d;
          R = D;
          for ( i = 0; i < max_reflection_count; i++ )
          {
            memset(&hit,0,sizeof(hit));
            T = R;
            if ( !T.Unitize() )
              break;
            if ( !ray.Shoot(Q,T,surface_list,hit) )
              break;
            Q = hit.m_A[0];
            polyline.Append(Q);
            if ( !hit.m_snodeB[0] )
              break;
            hit.m_snodeB[0]->Evaluate(hit.m_b[0],hit.m_b[1],1,3,&V[0].x);
            N = ON_CrossProduct(V[1],V[2]);
            if ( !N.Unitize() )
              break;
            d = N*T;
            R = T + (-2.0*d)*N; // R = reflection direction
          }


  Returns:
    True if the ray hits a surface.
  */
  bool Shoot(
    const ON_3dPoint& P,
    const ON_3dVector& D,
    const ON_Surface* surface, 
    ON_X_EVENT& hit
    );

  bool Shoot(
    const ON_3dPoint& P,
    const ON_3dVector& D,
    const ON_SimpleArray<const ON_Surface*>& surface_list, 
    ON_X_EVENT& hit
    );

  bool Shoot(
    const ON_3dPoint& P,
    const ON_3dVector& D,
    const ON_SurfaceTreeNode* snode, 
    ON_X_EVENT& hit
    );

  bool Shoot(
    const ON_3dPoint& P,
    const ON_3dVector& D,
    const ON_SimpleArray<const ON_SurfaceTreeNode*>& snode_list, 
    ON_X_EVENT& hit
    );

  bool Shoot(
    ON_3dPoint P,
    ON_3dVector D,
    int snode_count,
    const ON_SurfaceTreeNode* const * snode_list, 
    ON_X_EVENT& hit
    );

  ////////////////////////////////////////////////////////////
  //
  // For expert users ...
  //
  void ConstructHelper();

  bool SetRayHelper(
    ON_3dPoint P,
    ON_3dPoint Q
    );

  /*
  Parameters:
    mins - [in] minimum normalized line parameter
    maxs - [in] maximum normalized line parameter
  */
  bool ShootRayHelper(
    const ON_SurfaceTreeNode* stree, 
    double mins, 
    double maxs,
    ON_X_EVENT& hit
    );

  // Minimum 3d distance the ray must travel from "P" 
  // before an intersection is considered a "hit".  
  // The default is zero.
  double m_min_travel_distance;

  // Workspace for holding ray information.
  ON_CurveTree m_curve_tree;
  ON_CurveTreeBezier m_tree_bez;
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

/*
Description:
  Given the first (d-1) CVs and a value for the d-th derivative,
  this sets the d-th CV. If bezier_curve is rational, then all
  the weights must be set.  The weights are not changed.
Parameters:
  end - [in] 0 start of curve 
               (first d-1 CVs are input and the)
             1 end of curve 
               (last d-1 CVs are set)
  d - [in] derivative to set
            (>=0 and < bezier_curve.m_order)
  D - [in] 
        Value of the d-th derivative.
        (D[] has length bezier_curve.m_dim.)
  bezier_curve - [in/out] 
           If bezier_curve is rational, all the weights must be
           set and the weights are not changed.
           When end=0, the input curve must have the first d-1 CVs
           set and the output curve will have the d-th CV set so
           that the d-th derivative at 0.0 is D. 
           When end=1, the input curve must have the lastt d-1 CVs
           set and the output curve will have the (order-1-d)-th
           CV set so that the d-th derivative at 1.0 is D.
Returns:
  True if successful.
*/
ON_DECL
bool ON_SetCVFromDerivative( 
                  int end,
                  int d,
                  const double* D,
                  ON_BezierCurve& bezier_curve
                  );

/*
Description:
  Evaluate the tangent, curvature and jerk from
  the first three curve derivatives.
Parameters:
  D1 - [in] first derivative
  D2 - [in] second derivative
  D3 - [in] third derivative
  T - [out] unit tangent
  K - [out] curvature
  J - [out] jerk
Returns:
  True if D1 is not zero and T,K,J can be properly calculated
  False if D1 is zero.
Remarks:
  K = dT/ds = derivative of unit tangent
              with respect to arc length parameterization
  J = dK/ds = derivative of curvature with 
              with respect to arc length parameterization
*/
ON_DECL
bool ON_EvJerk( 
        ON_3dVector D1,
        ON_3dVector D2,
        ON_3dVector D3,
        ON_3dVector& T,
        ON_3dVector& K,
        ON_3dVector& J
        );


/*
Description:
  Evaluate the tangent, curvature, jerk, and yank from
  the first four curve derivatives.
Parameters:
  D1 - [in] first derivative
  D2 - [in] second derivative
  D3 - [in] third derivative
  D3 - [in] forth derivative
  T - [out] unit tangent
  K - [out] curvature
  J - [out] jerk
  Y - [out] yank
Returns:
  True if D1 is not zero and T,K,J,Y can be properly calculated
  False if D1 is zero.
Remarks:
  K = dT/ds = derivative of unit tangent
              with respect to arc length parameterization
  J = dK/ds = derivative of curvature
              with respect to arc length parameterization
  Y = dJ/ds = derivative of jerk 
              with respect to arc length parameterization
*/
ON_DECL
bool ON_EvYank( 
        ON_3dVector D1,
        ON_3dVector D2,
        ON_3dVector D3,
        ON_3dVector D4,
        ON_3dVector& T,
        ON_3dVector& K,
        ON_3dVector& J,
        ON_3dVector& Y
        );


/*
Description:
  Evaluate the normal tangent, curvature, jerk, and yank 
  to a surface at a given parameter.
Parameters:
  srf - [in] surface
  u - [in] surface parameter
  v - [in] surface parameter
  srfDirection - [in] a nonzero 3d vector that is not perpendicular
                    to srf at (u,v)
  T - [out] unit tangent to surface (in the direction of the projection
            of srfDirection to the tangent plane at (u,v)).
  K - [out] normal curvature in direction of T
  J - [out] normal jerk
  Y - [out] normal yank
Returns:
  1 if T,K,J,Y can be properly calculated.
  0 for failure
  -1 if srf is singular at u,v
*/
ON_DECL
int ON_EvNormalYank( 
        const ON_Surface& srf,
        double u,
        double v,
        ON_3dVector srfDirection,
        ON_3dVector& T,
        ON_3dVector& K,
        ON_3dVector& J,
        ON_3dVector& Y
        );

/*
Description:
  Given the first derivative and a curvature, calculate the 
  shortest possible second derivative such that a curve with
  the input first derivative and calculated second derivative
  would have the specified curvature.
Parameters:
  D1 - [in] first derivative
  K - [in] curvature
Returns:
  2nd derivative.
Remarks:
  You can add any multiple of the tangent to the
  returned second derivative without changing
  the curvature.
*/
ON_DECL
ON_3dVector ON_2ndDerFromCurvature(
            ON_3dVector D1,
            ON_3dVector K
            );

/*
Description:
  Given the first derivative, second derivative, and a jerk,
  calculate the shortest possible third derivative such that
  a curve with the input first derivative, input second
  derivative, and calculated third derivative
  would have the specified jerk.
Parameters:
  D1 - [in] first derivative
  D2 - [in] second derivative
  J - [in] jerk
Returns:
  3rdd derivative.
Remarks:
  You can add any multiple of the tangent to the
  returned third derivative without changing
  the jerk.
*/
ON_DECL
ON_3dVector ON_3rdDerFromJerk( 
            ON_3dVector D1, 
            ON_3dVector D2, 
            ON_3dVector J
            );

/*
Description:
  Given the first derivative, second derivative, third
  derivative, and a yank, calculate the shortest possible
  forth derivative such that a curve with the input 
  first derivative, input second derivative, input
  third derivative and calculated forth derivative
  would have the specified yank
Parameters:
  D1 - [in] first derivative
  D2 - [in] second derivative
  D3 - [in] second derivative
  Y - [in] yank
Returns:
  4th derivative.
Remarks:
  You can add any multiple of the tangent to the
  returned forth derivative without changing
  the yank.
*/
ON_DECL
ON_3dVector ON_4thDerFromYank( 
            ON_3dVector D1, 
            ON_3dVector D2, 
            ON_3dVector D3, 
            ON_3dVector Y
            );

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


class ON_CLASS ON_MMX_Polyline : public ON_SimpleArray<ON_MMX_POINT>
{
public:
  // see ON_SimpleArray class definition comments for constructor documentation
  ON_MMX_Polyline();
  ON_MMX_Polyline(int);
  ON_MMX_Polyline(const ON_SimpleArray<ON_MMX_POINT>&);
  ON_MMX_Polyline& operator=( const ON_MMX_Polyline& );

  // Description:
  //   Removes duplicate points that result in zero length segments.
  // Parameters:
  //   tolerance - [in] tolerance used to check for duplicate points.
  // Returns:
  //   Number of points removed.
  // Remarks:
  //   If the distance between points polyline[i] and polyline[i+1]
  //   is <= tolerance, then the point with index (i+1) is removed.
  int Clean( 
    double tolerance = 0.0 
    );

};

class ON_CLASS ON_CURVE_POINT
{
public:
  ON_CURVE_POINT();

  // C++ default destructor, copy constructor, and operator= work fine.

  const class ON_Curve* m_curve;
  const class ON_CurveTreeNode* m_cnode;

  //If this intersection is as a result of a mesh face, mesh edge intersection then 
  //m_ci.m_type = ON_COMPONENT_INDEX::meshtop_edge and m_index will be an index into
  //the m_mesh->Topology().m_tope array.  m_t will be the parameter on the line
  //from edge.vi[0] to edge.vi[1]
  ON_COMPONENT_INDEX m_ci;  
  const ON_Mesh* m_mesh; //nullptr if m_ci is unset

  double m_t; // curve parameter
  double m_node_t; // bezier parameter in m_cnode
  unsigned int m_sn; //ON_CURVE_POINT serial number
};


//TODO Maybe we need an ON_MMX_EVENT that has a intersection type associated with it.
//ON_MMX_POINT m_M[2] where m_M[0] = m_M[1] if it is a transverse intersection and 
//m_M[0] != m_M[1] if it is an overlap intersection.


class ON_CLASS ON_CMX_EVENT
{
public:
  ON_CMX_EVENT();

  // C++ default destructor, copy constructor, and operator= work fine.

  enum TYPE
  { 
    no_cmx_event =  0,

    // Two valid event types for curve-mesh intersections
    cmx_point   =  1, // curve-mesh transverse intersection point
    cmx_overlap =  2  // curve-mesh intersection overlap
  };

  /*
    The m_type field determines how the values in the other
    fields are interpreted.
     
    cmx_point events:
        C[0] = C[1] intersection point on curve
        M[0] = M[1] = intersection point on mesh

    cmx_overlap events:
      C[0],C[1] = overlap end points on curve, C[0].m_t < C[1].m_t
      M[0],M[1] = overlap endpoints on mesh
  */
  TYPE m_type;

  ON_CURVE_POINT m_C[2]; // intersection points on curve
  ON_MESH_POINT m_M[2];  // corresponding intersection points on mesh
  
  // Each intersection event is assigned a runtime serial number.
  unsigned int m_sn;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ON_CLASS ON_MeshTreeNode
{
public:
  bool IsValid( ON_TextLog* text_log = 0 ) const;

  // See ON_CurveTreeNode for working example.
  bool GetClosestPoint(const ON_3dPoint& P, 
                       ON_MESH_POINT* t, 
                       double maximum_distance = 0.0) const;

  int IntersectLine(ON_Line line,
                    ON_SimpleArray< ON_CMX_EVENT >& x, 
                    double intersection_tolerance = 0.0,
                    double overlap_tolerance = 0.0
                    ) const;

  int IntersectPolyline(
                    const int& count,
                    const ON_3dPoint* points,
                    ON_SimpleArray< ON_CMX_EVENT >& x, 
                    double intersection_tolerance = 0.0,
                    double overlap_tolerance = 0.0
                    ) const;

  int IntersectPolyline(
                    const ON_SimpleArray<ON_3dPoint>& points,
                    ON_SimpleArray< ON_CMX_EVENT >& x, 
                    double intersection_tolerance = 0.0,
                    double overlap_tolerance = 0.0
                    ) const;

  // slow and careful
  int IntersectMesh(const class ON_MeshTreeNode* mnodeB, 
                    ON_ClassArray< ON_SimpleArray< ON_MMX_POINT > >& x, 
                    double intersection_tolerance = 0.0,
                    double overlap_tolerance = 0.0) const;

  // fast and sloppy
  int IntersectMesh(const class ON_MeshTreeNode* mnodeB, 
                    ON_SimpleArray< ON_Line > & lines 
                    ) const;

  /*
  Description:
    This is a debugging function.  It is slow.
    Starting at this node, find the node that contains
    the specified mesh face.
  Parameters:
    mesh_face_index - [in] ON_Mesh m_F[] array index
  Returns:
    Pointer to the node that contains the specified face.
  */
  const ON_MeshTreeNode* FindNode( 
          int mesh_face_index
          ) const;

  ON_MeshTreeNode();

  ON_BoundingBox m_bbox;

  ON_MeshTreeNode* m_up;
  ON_MeshTreeNode* m_down[2];

  const ON_Mesh* m_mesh;

  const int* m_farray; // array of indices of faces in this node
                       // (null if m_fcount is 0)
                        // never free or delete this pointer.

  int m_fcount;   // number of faces in m_farray[] (can be 0)

  unsigned int m_treesn;     // serial number of this tree

  unsigned int m_nodesn;     // serial number of this node

  // Memory management accounting information
  unsigned char m_nodetype;  // 8 bits to flag special nodes
                             // 1 = root node
                             // 2 = meta node (no domain)
                             // 4 = reparam
  unsigned char m_nodemem;   // 0 = do not destroy
                             // 1 = destroy with delete
                             // 2 = destroy with free

  // pad class size to a multiple of 8 bytes so custom 
  // allocator can keeps doubles and pointers properly
  // aligned and safely use memset()/memcpy() on
  // compilers that reuse tail-padding.
#if 8 == ON_SIZEOF_POINTER
  // 8 byte pointers 
  unsigned char m_reserved_ON_MeshTreeNode[2];
#else
  // 4 byte pointers
  unsigned char m_reserved_ON_MeshTreeNode[6];
#endif

private:
  //bool ClosestPtToMeshFace(const int& faceidx, const ON_3dPoint& ptIn, ON_MESH_POINT* ptOut) const;

private:
  // prohibit use - no implementation
	ON_MeshTreeNode(const ON_MeshTreeNode& orig); // no copy constructor
	ON_MeshTreeNode& operator=(const ON_MeshTreeNode& orig); // no operator=
};

class ON_CLASS ON_MeshTree : public ON_MeshTreeNode
{
public:
  ON_MeshTree();
  ~ON_MeshTree();

  bool IsValid( ON_TextLog* text_log = 0 ) const;

  void DestroyTree();

  bool CreateTree(
          const ON_Mesh& mesh, 
          int max_depth = 0, 
          int max_faces_per_leaf = 0
          );

  /*
  Description:
    Expert user tool to get a mesh tree node.  This node
    will be destroyed by this tree's destructor.
  Returns:
    A mesh tree node.  Never delete or free the returned pointer.
  */
  ON_MeshTreeNode* NewNode();

  // root node of the binary tree (currently unused)
  ON_MeshTreeNode* m_root;

  /*
  Returns:
    Number of bytes of heap memory used by this mesh tree.
  */
  size_t SizeOf() const;

private:
  void Startup( int max_depth, int max_faces_per_leaf );
  class ON__MeshTreeMem* m__p;

private:
  // prohibit use - no implementation
  ON_MeshTree(const ON_MeshTree&); // no copy constructor
  ON_MeshTree& operator=(const ON_MeshTree&); // no operator=
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ON_DECL
bool ON_MeshGetClosestPoint(const ON_Mesh* mesh, 
                            ON_3dPoint P, 
                            ON_MESH_POINT* t, 
                            double distance_tolerance = 0.0);

ON_DECL
/*
  Gets a value indicating if mesh booleans and intersections use the new code.
*/
bool ON_MX_GetUseNewCode();

ON_DECL
/*
  Determines if mesh booleans and intersections use the new code.
*/
void ON_MX_SetUseNewCode(bool value);

ON_DECL
/*
  This function intersects two meshes.
  Consider using the more advanced methods of the ON_Mesh class: IntersectArray, GetSelfIntersections, etc...
*/
bool ON_MeshMeshIntersectOldCompliant(const ON_Mesh* meshA,
  const ON_Mesh* meshB,
  ON_ClassArray<ON_Polyline>& plines,
  ON_ClassArray<ON_Polyline>& overlapplines,
  double intersection_tolerance = 0.0,
  double overlap_tolerance = 0.0,
  bool bDebug = false);

ON_DECL
// This function is soon going to be deprecated. It always uses the old code.
bool ON_MeshMeshIntersect(const ON_Mesh* meshA, 
                          const ON_Mesh* meshB, 
                          ON_ClassArray<ON_MMX_Polyline>& plines, 
                          ON_ClassArray<ON_MMX_Polyline>& overlapplines, 
                          double intersection_tolerance = 0.0, 
                          double overlap_tolerance = 0.0,
                          bool bDebug = false);

// Finds intersections between a mesh and a semi-infinite, oriented ray.
// Inputs:
//   mesh [in] - a mesh.
//   ray [in] - the ray.
//   tolerance [in] - a small, positive value that is used to tell overlaps and perforations apart.
//   ray_ts [in] - an array that will be filled with new values
// Outputs:
//   true if successful. There may not be intersections in this case.
ON_DECL
bool ON_IntersectMeshRay(const ON_Mesh* mesh,
  const ON_3dRay& ray,
  double tolerance,
  ON_SimpleArray<double>& ray_ts);

ON_DECL
// This function is soon going to be deprecated. It always uses the old code.
bool ON_FindMeshMeshIntersectionSets(ON_SimpleArray<ON_MMX_POINT>& pArray, double tol);




///////////////////////////////////////////////////////////////////
//
// FPU control and query tools
//

///////////////////////////////////////////////////////////////////
// FPU status word bits
//
#if defined(ON_RUNTIME_WIN)
#define ON_FPU_SW_INEXACT    _SW_INEXACT    // inexact (precision) - ok - happens frequently
#define ON_FPU_SW_UNDERFLOW  _SW_UNDERFLOW  // underflow           - ok - happens on occasion
#define ON_FPU_SW_OVERFLOW   _SW_OVERFLOW   // overflow            - bad - (but .NET's JIT intentionally sets it.)
#define ON_FPU_SW_ZERODIVIDE _SW_ZERODIVIDE // zero divide         - bad 
#define ON_FPU_SW_INVALID    _SW_INVALID    // invalid             - bad - (but .NET's use of WPF intentionally sets it.)
#define ON_FPU_SW_DENORMAL   _SW_DENORMAL   // denormal status bit - ok - happens on occasion
#else
#define ON_FPU_SW_INEXACT    0x00000001     // inexact (precision) - ok - happens frequently
#define ON_FPU_SW_UNDERFLOW  0x00000002     // underflow           - ok - happens on occasion
#define ON_FPU_SW_OVERFLOW   0x00000004     // overflow            - bad
#define ON_FPU_SW_ZERODIVIDE 0x00000008     // zero divide         - bad
#define ON_FPU_SW_INVALID    0x00000010     // invalid             - bad
#define ON_FPU_SW_DENORMAL   0x00080000     // denormal status bit - ok - happens on occasion
#endif

// "Bad" FPU status - results of calculation are suspect

// As of November 2014, .NET makes it impossible to use the invalid status bit
// to detect serious floating point bugs and errors.  See the comment above
// the declaration of ON_FPU_EnableInvalidStatusCheck() for more details.
//#define ON_FPU_SW_BAD (ON_FPU_SW_OVERFLOW|ON_FPU_SW_ZERODIVIDE|ON_FPU_SW_INVALID)
#define ON_FPU_SW_BAD (ON_FPU_SW_OVERFLOW|ON_FPU_SW_ZERODIVIDE)

///////////////////////////////////////////////////////////////////
// FPU exception mask bits
//
#if defined(ON_RUNTIME_WIN)
#define ON_FPU_CW_EX_MASK    _MCW_EM
#define ON_FPU_EX_INEXACT    _EM_INEXACT    // inexact (precision)
#define ON_FPU_EX_UNDERFLOW  _EM_UNDERFLOW  // underflow
#define ON_FPU_EX_OVERFLOW   _EM_OVERFLOW   // overflow
#define ON_FPU_EX_ZERODIVIDE _EM_ZERODIVIDE // zero divide
#define ON_FPU_EX_INVALID    _EM_INVALID    // invalid 
#define ON_FPU_EX_DENORMAL   _EM_DENORMAL   // denormal exception mask
#else
#define ON_FPU_CW_EX_MASK    0x0008001f
#define ON_FPU_EX_INEXACT    0x00000001     // inexact (precision)
#define ON_FPU_EX_UNDERFLOW  0x00000002     // underflow
#define ON_FPU_EX_OVERFLOW   0x00000004     // overflow
#define ON_FPU_EX_ZERODIVIDE 0x00000008     // zero divide
#define ON_FPU_EX_INVALID    0x00000010     // invalid
#define ON_FPU_EX_DENORMAL   0x00080000     // denormal exception mask
#endif

// Use ON_FPU_EX_ALL to mask all FPU exceptions
#define ON_FPU_EX_ALL (ON_FPU_EX_INEXACT|ON_FPU_EX_UNDERFLOW|ON_FPU_EX_OVERFLOW|ON_FPU_EX_ZERODIVIDE|ON_FPU_EX_INVALID|ON_FPU_EX_DENORMAL)

// Minor FPU exceptions that are common and generally benign
// These should be masked unless you are doing something unusual.
#define ON_FPU_EX_MINOR (ON_FPU_EX_INEXACT|ON_FPU_EX_UNDERFLOW|ON_FPU_EX_DENORMAL)

///////////////////////////////////////////////////////////////////
// FPU control word precision control bits
//
#if defined(ON_RUNTIME_WIN)
#if defined(_M_X64)
// x64 does not support precision control changes.
// It always uses 64 bit mantissas on the fpu and
// this is what we want.
#define ON_FPU_CW_PC_MASK    0
#else
#define ON_FPU_CW_PC_MASK    _MCW_PC
#endif

#define ON_FPU_PC_64         _PC_64         // 64 bits
#define ON_FPU_PC_53         _PC_53         // 53 bits
#define ON_FPU_PC_24         _PC_24         // 24 bits
#else
#define ON_FPU_CW_PC_MASK    0x00030000
#define ON_FPU_PC_64         0x00000000     // 64 bits
#define ON_FPU_PC_53         0x00010000     // 53 bits
#define ON_FPU_PC_24         0x00020000     // 24 bits
#endif

///////////////////////////////////////////////////////////////////
// FPU control word rounding control bits
//
#if defined(ON_RUNTIME_WIN)
#define ON_FPU_CW_RC_MASK    _MCW_RC
#define ON_FPU_RC_NEAR       _RC_NEAR       // near
#define ON_FPU_RC_DOWN       _RC_DOWN       // down
#define ON_FPU_RC_UP         _RC_UP         // up
#define ON_FPU_RC_CHOP       _RC_CHOP       // chop
#else
#define ON_FPU_CW_RC_MASK    0x00000300
#define ON_FPU_RC_NEAR       0x00000000     // near
#define ON_FPU_RC_DOWN       0x00000100     // down
#define ON_FPU_RC_UP         0x00000200     // up
#define ON_FPU_RC_CHOP       0x00000300     // chop
#endif

///////////////////////////////////////////////////////////////////
// FPU default control word for Rhino and OpenNURBS
//   - mask all exceptions (Microsoft and Intel default)
//   - 53 bit mantissa precision (Microsoft default - Intel default is 64 bit mantissa precision)
//   - round to nearest (Microsoft and Intel default)
//   - signed infinity  (Microsoft and Intel default)
//   (Intel FPUs 387 and later use signed infinities)
//
#if defined(_M_X64)
// x64 does not permit precision control changes. It uses 53 bit mantissa precision.
#define ON_FPU_CW_DEFAULT      ( ON_FPU_EX_ALL     | ON_FPU_RC_NEAR    )
#else
// 14 July 2009 Dale Lear
//    In order to have our 32-bit and 64-bit executables perform
//    the same way, I'm switching the FPU precision to 53-bit mantissas.
//    This means plug-ins may run differently in V4 and V5, but in the
//    using this setting will make it easier to write conde the behaves
//    consistently.  PC_53 is also Microsoft's default setting.
//#define ON_FPU_CW_DEFAULT      ( ON_FPU_EX_ALL     | ON_FPU_PC_64      | ON_FPU_RC_NEAR    )
#define ON_FPU_CW_DEFAULT      ( ON_FPU_EX_ALL     | ON_FPU_PC_53      | ON_FPU_RC_NEAR    )
#endif
#define ON_FPU_CW_DEFAULT_MASK ( ON_FPU_CW_EX_MASK | ON_FPU_CW_PC_MASK | ON_FPU_CW_RC_MASK )


///////////////////////////////////////////////////////////////////
//
// FPU functions
//

/*
Description:
  Clear FPU exception and busy flags (Intel assembly fnclex).
See Also:
  ON_FPU_ExceptionStatus
*/
ON_DECL
void ON_FPU_ClearExceptionStatus();

/*
Returns
  Current FPU exception flags (Intel assembly fnstsw).  
  Use the ON_FPU_SW_* defines to interpret the returned value.
Example:
          ON_FPU_ClearExceptionStatus();

          ... do some calculations ...

          ON_UINT32 status = ON_FPU_ExceptionStatus();
          if ( 0 != (status & ON_FPU_SW_BAD ) )
          {
             ... The results of the calculations are suspect.
          }
See Also:
  ON_FPU_ClearExceptionStatus
  ON_FPU_BadExceptionStatusMask
*/
ON_DECL
ON__UINT32 ON_FPU_ExceptionStatus();

/*
Returns:
  Control word used by the FPU.  Except when exception
  debugging is enabled, the value will be FPU_CW_DEFAULT.
See Also:
  ON_FPU_DebugExceptions
*/
ON_DECL
ON__UINT32 ON_FPU_InitControlWord(void);

/*
Description:
  Set the FPU control bits to the Microsoft's preferred defaults.
  These are the same as the Intel FPU fninit defaults,
  except for 32-bit executable mantisaa precision.  Microsoft's
  default is 53-bit mantissas and Intel's is 64-bit mantissas.
  Rhino and OpenNURBS code works best with these settings.
    - mask exceptions
    - round to nearest
    - 53 bit fraction precision control
Returns:
  Control word;
See Also:
  ON_FPU_MaskExceptions
*/
ON_DECL
ON__UINT32 ON_FPU_Init(void);

/*
Description:
  Returns the FPU control word.
*/
ON_DECL
ON__UINT32 ON_FPU_ControlWord();

/*
Description:
  A DEBUGGING tool to unmask FPU exception handling.
  Do not call this function in released code.
Parameters:
  unmasked_ex - [in] 
    Use the ON_FPU_EX_* defines to set the bits of the 
    exceptions you want to unmask.
Example:
          #if defined(ON_DEBUG)
          // crash on divide by zero, overflow, and invalid
          ON_FPU_UnmaskExceptions(ON_FPU_EX_OVERFLOW|ON_FPU_EX_ZERODIVIDE|ON_FPU_EX_INVALID);
          #endif

          ... do suspect calculations

          #if defined(ON_DEBUG)
          // Mask all exceptions (default)
          ON_FPU_UnmaskExceptions(0);
          #endif
Remarks:
  By default floating point exceptions are "masked". This
  means nothing serious happens when an exception, like
  dividing by zero, occurs.  When a floating point exception
  is "unmasked" it means the application will crash when
  that floating point exception occurs.  Unmasking
  exceptions is a useful debugging technique.  Never
  unmask exceptions in release code.

Returns:
  True if the input was valid and the call worked.
*/
ON_DECL
bool ON_FPU_UnmaskExceptions(ON__UINT32 unmasked_ex);

/*
Description:
  Sometimes it is impossible to avoid calling code that performs
  invalid floating point operations or rudely changes the FPU
  control settings.  We have found dozens of cases in Windows
  core DLLs,3rd party DLLs, OpenGL drivers, and the .NET JIT
  conmpiler where the FPU CTRL setting is changed or floating
  point exceptions are generated.  When these cases are 
  discovered, we bracket the code that is abusing the
  FPU with 
     ON_FPU_BeforeSloppyCall();
     ... call that abuses the FPU
     ON_FPU_AfterSloppyCall();
  so we don't lose any information about exceptions in our
  own code and we don't get pestered about exceptions we can't
  do anything about.  If you are calling something that may
  run the .NET JIT, then use ON_FPU_AfterDotNetJITUse() instead
  of ON_FPU_AfterSloppyCall().
Returns:
  ON_FPU_BeforeSloppyCall() returns true if the FPU is "clean"
  at the time of the call.  ON_FPU_AfterSloppyCall() returns
  true if the FPU stayed clean since the last call to 
  ON_FPU_BeforeSloppyCall().
Remarks:
  These functions should be used only in extreme situations
  and there should be a verbose comment in the code explaining
  why the programmer resorted to using these functions.
*/
ON_DECL
bool ON_FPU_BeforeSloppyCall(void);

ON_DECL
bool ON_FPU_AfterSloppyCall(void);

/*
Description:
  The .NET JIT (just in-time compiler) has a bug that 
  causes a floating point overflow exception if there 
  is any double constant that whose absolute value is 
  greater than 3.402823466e+38 (=FLT_MAX).
  
  The following .NET C# program demonstrates the bug in the
  Microsoft JIT compiler.

      class Program
      {
        static bool IsBigDouble(double x)
        {
          // Note: 3.402823466e+38 is the maximum value of a 32 bit IEEE float
          //return x == 3.402823466e+38; // JIT does not generate overflow exception
          //return x == 3.5e+38;         // JIT generates overflow exception
          return x == double.MaxValue;   // JIT generates overflow exception
        }
        static void Main(string[] args)
        {
          double x = 1.0;
          // When the JIT compiles IsBigDouble(), it will generate
          // an FPU overflow exception if the constant in IsBigDouble()
          // is larger than the maximum 32 bit IEEE float.
          bool rc = IsBigDouble(x);
          if ( rc )
            System.Console.WriteLine("{0} is a big double.",x);
          else
            System.Console.WriteLine("{0} is not a big double.",x);
        }
      }
  
  If you are getting warnings that code that makes calls into
  .NET code is generating FPU overflow exceptions, then bracket
  the call to code that  may call the .NET JIT with calls to
  ON_FPU_BeforeSloppyCall()/ON_FPU_AfterDotNetJITUse().
Returns:
  ON_FPU_AfterDotNetJITUse() returns true if the FPU stayed 
  clean except for an overflow exception
  since the last call to ON_FPU_BeforeSloppyCall().
Remarks:
  These functions should be used only in extreme situations
  and there should be a verbose comment in the code explaining
  why the programmer resorted to using these functions.

  Microsoft is aware of this bug and claims it is "by design".
  http://connect.microsoft.com:80/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=100718
     ...
     So in summary this behavior is by Design. In the ECMA
     spec for IL it states that IL floating point operations
     never throw exceptions. The way to detect floating point
     overflow is to use the ckfinite IL instruction after each
     floating point operation that could produce an overflow.
     ...
  Microsoft could easily fix this bug by
  1) saving the FPU CTRL state before running their JIT
  2) clearing the FPU STAT register and restoring the CTRL 
     state after running their JIT
  This amounts to 3 lines of C and less than 10 lines of assembly 
  code and is what I would expect them to do.
*/
ON_DECL
bool ON_FPU_AfterDotNetJITUse(void);


/*
Description:
  WPF uses NaNs for .NET properties that are not initialized
  and intentionally passes these NaNs into the FPU at various
  times. This sets the FPU's invalid operation status bit.
  Prior to November 2014, we used to alert developers that
  their code had done something wrong when the FPU's 
  invalid status bit was set.  Because we have almost no
  control over when .NET will set the invalid status bit,
  we can no longer assume that we have a bug in our code 
  when the bit is set.

  By default, we now treat the setting of invalid status bit 
  the same we we treat the inexact bit, namely it is a common
  and expected situation.

  If you are attempting to test or debug scientific computing
  code and you want to be told when something really bad
  happens on the FPU, then call ON_FPU_EnableInvalidStatusCheck(true)
  before you run your tests.

Returns:
  The value of ON_FPU_EnableInvalidStatusCheckEnabled() before
  it was set to bEnableInvalidStatusCheck.
*/
ON_DECL
bool ON_FPU_EnableInvalidStatusCheck(
  bool bEnableInvalidStatusCheck
  );

ON_DECL
bool ON_FPU_InvalidStatusCheckEnabled();

/*
Description:
  Check the FPU status or control.
Parameters:
  bCheckExceptionStatus - [in]
    If true, the FPU exception status is checked
    to see if any exceptions have happened.
  bCheckControl - [in]
    If true, the FPU control settings are checked
    to see if they have the expected values.
  bCall_ON_Error - [in]
    If true and and exception has occurred or
    the control settings have been changed,
    then ON_Error() is called.
Returns:
  true:
    The FPU status indicates no exceptions have occurred and the
    control settings match the expected values.
  false:
    The FPU status indicates exceptions have occurred or the
    FPU control settings have unexpected values.
   

*/
ON_DECL
bool ON_FPU_Check( 
        bool bCheckExceptionStatus,
        bool bCheckControl,
        bool bCall_ON_Error 
        );

// The ON_ClashEvent class is used by ON_MeshClashSearch() 
// and ON_MeshFaceClashSearch() to return the location mesh
// clash events.
class ON_CLASS ON_ClashEvent
{
public:
  // ON_ClashEvent::UnsetEvent is used as the uninitialized clash event value.
  //  m_i[0] = -1
  //  m_i[1] = -1
  //  m_P = ON_3dPoint::UnsetPoint
  static const ON_ClashEvent UnsetEvent;

  // Default value is ON_ClashEvent::UnsetEvent
  ON_ClashEvent();

  // m_i[] values are used to identify clashing elements.
  // The interpretation of the value varies and is described
  // in the ON_...ClashSearch() documentation.
  ON__INT_PTR m_i[2]; 

  // If m_P is valid, then the sphere centered at P of radius
  // distance intersects the clashing elements.
  ON_3dPoint m_P;
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_ClashEvent>;
#endif

/*
Description:
  Search for locations where the distance from a mesh in 
  mesh_listA[] to a mesh in mesh_listB[] is <= distance.
Parameters:
  mesh_listA_count - [in]
    length of mesh_listA[] array
  mesh_listA - [in]
    array of mesh pointers
  mesh_listB_count - [in]
    length of mesh_listB[] array
  mesh_listB - [in]
    array of mesh pointers
  distance - [in] (valid input are >= 0).
  bUseMultipleThreads - [in]
    Pass true if you want the calculation to use
    multiple threads.
  max_event_count - [in]
    If max_event_count > 0, then the search for clashes stops
    after max_event_count elements have been appended to
    clash_events[].
  clash_events - [out]
    clash events are appended to this array.
    ON_ClashEvent.m_index[0] is a mesh_listA[] index and
    ON_ClashEvent.m_index[1] is a mesh_listB[] index.
Returns:
  Number of events appended to clash_events[].
*/
ON_DECL
int ON_MeshClashSearch(
  int mesh_listA_count,
  const ON_Mesh* const * mesh_listA,
  int mesh_listB_count,
  const ON_Mesh* const * mesh_listB,
  double distance,
  bool bUseMultipleThreads,
  int max_event_count,
  ON_SimpleArray< class ON_ClashEvent >& clash_events
  );


/*
Description:
  Search for locations where the distance from a mesh in 
  mesh_listA[] to a mesh in mesh_listB[] is <= distance.
Parameters:
  meshA - [in]
  meshAtree - [in]
    nullptr or an ON_RTree created by calling CreateMeshFaceTree(meshA).
  meshB - [in]
  meshBtree - [in]
    nullptr or an ON_RTree created by calling CreateMeshFaceTree(meshB).
  distance - [in] (valid input are >= 0).
  bUseMultipleThreads - [in]
    Pass true if you want the calculation to use
    multiple threads.
  max_event_count - [in]
    If max_event_count > 0, then the search for clashes stops
    after max_event_count elements have been appended to
    clash_events[].
  clash_events - [out]
    clash events are appended to this array.
    ON_ClashEvent.m_index[0] is a meshA->m_F[] index and
    ON_ClashEvent.m_index[1] is a meshB->m_F[] index.
  meshA_face_indices - [out]
  meshB_face_indices - [out]
Returns:
  Number of events appended to clash_events[].
*/
ON_DECL
int ON_MeshFaceClashSearch(
  const class ON_Mesh* meshA,
  const class ON_RTree* meshAtree,
  const class ON_Mesh* meshB,
  const class ON_RTree* meshBtree,
  double distance,
  bool bUseMultipleThreads,
  int max_event_count,
  ON_SimpleArray< class ON_ClashEvent >& clash_events
  );


/*
Description:
  Search for locations where the distance from a mesh in 
  mesh_listA[] to a mesh in mesh_listB[] is <= distance.
Parameters:
  meshA - [in]
  meshAtree - [in]
    nullptr or an ON_RTree created by calling CreateMeshFaceTree(meshA).
  meshB - [in]
  meshBtree - [in]
    nullptr or an ON_RTree created by calling CreateMeshFaceTree(meshB).
  distance - [in] (valid input are >= 0).
  bUseMultipleThreads - [in]
    Pass true if you want the calculation to use
    multiple threads.
  max_event_count - [in]
    If max_event_count > 0, then the search for clashes stops
    after max_event_count elements have been appended to
    clash_events[].
  meshA_face_indices - [out]
  meshB_face_indices - [out]
Returns:
  Number of events appended to clash_events[].
*/
ON_DECL
void ON_MeshFaceClashSearch(
  const class ON_Mesh* meshA,
  const class ON_RTree* meshAtree,
  const class ON_Mesh* meshB,
  const class ON_RTree* meshBtree,
  double distance,
  bool bUseMultipleThreads,
  int max_event_count,
  ON_SimpleArray<int>& meshA_face_indices,
  ON_SimpleArray<int>& meshB_face_indices
  );

ON_DECL
double ON_ClosestPointAngle(const ON_Line&, const ON_Curve&, ON_Interval,const ON_3dPoint&,ON_3dPoint&, double*, double* );

ON_DECL
double ON_GetFittingTolerance( const class ON_SurfaceTreeNode* snodeA, const class ON_SurfaceTreeNode* snodeB, double intersection_tolerance, double fitting_tolerance );

//#include "opennurbs_plus_validate.h"

#include "opennurbs_plus_massprop.h"
#if defined (ON_RUNTIME_WIN)
#include "opennurbs_plus_registry.h"
#endif
#include "opennurbs_plus_mesh_marker.h"
#include "opennurbs_plus_squish.h"
#include "opennurbs_plus_trimesh.h"
#include "opennurbs_plus_sections.h"
#include "opennurbs_plus_rectpack.h"
#include "opennurbs_plus_hiddenline.h"
#include "opennurbs_plus_implicitfn.h"

enum class ON_LogMessageType : int
{
  unknown = 0,
  information = 1,
  warning = 2,
  error = 3,
  assert = 4
};

ON_DECL void ON_SendLogMessageToCloud(ON_LogMessageType msg_type, const wchar_t* wszClass, const wchar_t* wszEventDesc, const wchar_t* wszMessage);

typedef void(*ON_SSLMTC_PROC)(int msg_type, const ON_wString* pClass, const ON_wString* pEventDesc, const ON_wString* pMessage);
ON_DECL void ON_SetSendLogMessageToCloudProc(ON_SSLMTC_PROC);

//ON_SimpleMinimizer should be used as a local minimizer of positive valued G2 functions.
//It requires first and second derivatives
//This is a local minimizer. If it hits the edge of the domain and 
//the only way down is out of the domain, it will stop and consider itself
//successful.  Failure means that the iteration count limit was hit
//and there was no convergence.  In that case, the result will be no
//larger that that of the seed, and may be of use.

class ON_CLASS ON_SimpleMinimizer

{
public:
  ON_SimpleMinimizer(int dim, 
    const ON_Interval* domain, //dim of them
    int max_it,//stop after this many iterations
    double val_tol,//Stop if the function value is below this.
                   //If <= 0, only convergence_tol will be used.
    const double* convergence_tol//dim of them.
                                 //Stop if the change from an iteration is within these.
                                 //Any that is less than ON_EPSILON^2 
                                 //will be replaced by N_EPSILON^2.
  );

  virtual ~ON_SimpleMinimizer();

  //If bError is true, val and params will not be filled in.
  //If bError is false, val and params will be filled in with the best result found.
  //In this case, a return of false means that the tolerances were not met.
  bool Minimize(const double* seed, //Input.  Dim of them
    double* val, //Output. Minimum function value found.
    double* params, //Output.  Dim of them.  Parameters of minimum.
    bool* bError //Output.  If true, then there was a serious
                 //problem and the results in val and params are garbage.
  );

  int Dimension() const;

  //pure virtual
  virtual bool Ev(const double* params, //dim
    double*  f,    //Function value.
    double*  df,   //First partials. dim of them ort NULL
    double** ddf   //Second partials. dim x dim or NULL. 
                   //If df is NULL, ddf will not be computed.
  ) const;
  int LineEval(double t, double* f, double* df);
private:
  ON_SimpleMinimizer() = delete;//Not implemented
  friend class CSimpleMinBrent;
  bool FillIn();//Evaluate at parameters and set up matrix equation.
  int Test();//This will call FillIn to set up the nextg iteration, and check
             //if f < val_tol. Return is 0 if done, -1 if error, 1 otherwise.
  int NextGuess();//Same return as Test()
  bool LineMin();//Used in NextGuess.  Matrix equation gives a guess. 
                 //Now use Brent to find the best answer between 
                 //it and the previous best guess.
  bool TrimToDomain(double* del);//Also used in NextGuess.  Matrix equation may
                                 //give an answer outside the domain.  Find where the
                                 //segment from the previous guess to that plus del hits the 
                                 //domain boundary, if it crosses
  int m_dim;
  int m_max_it;
  ON_Matrix m_M;//m_dim x m_dim
  double* m_it_values;//m_dim of them
  double* m_prev_params;//dim.  Starts out as seed
  double* m_params; //m_dim of them
  double m_val;
  double m_val_tol;
  double* m_convergence_tol;
  ON_Interval* m_domain;//m_dim of them
  bool m_bOK;//initialization worked
  bool m_bBestSet;
  int* m_on_edge; //-1 if not on edge of domain[i], 
                  //otherwise, m_params[i] = m_domain[i][m_on_edge[i]]
                  //Scratch space for evaluation
  double* m_df;//dim
  double** m_ddf;//dim x dim

  ON_Workspace m_ws;
  double* m_del;//dim
  double m_best_val;
  double* m_best_params;//m_dim of them

  //For use in LineMin
  double* m_line_start;
  double* m_line_end;
  double* m_line_params;//m_dim of them.  Scratch
  double* m_line_dF;//m_dim of them.  Scratch
  void SetLineParameters(const double* start, const double* end);

  //if -1, a is best.  if 1, c is best.  if 0, b is better than a and c.
  int Bracket(double& a, double& b, double& c);
};

//////////
// ON_GetFunctionRoot() searches for a root of a C1 function.
// ON_GetFunctionRoot() returns true if it finds a root and false if
// it fails to find a root.  In all cases, it returns the "best"
// candidate for a root that it can find.  The algorithm is Newton-Raphson
// and works best if the function is a monotone function.  However
// you can use any function as long as it is C1 and f(t0)*f(t1) <= 0.0.
//

ON_DECL bool ON_GetFunctionRoot(
  void (*func)(void*, double, double*, double*),
  void* funcarg,
  double t0,         // domain to search for root = [t0, t1]
  double t1,         // f(t0)*f(t1) <= 0.0
  double* root       // value of root
);

#endif
