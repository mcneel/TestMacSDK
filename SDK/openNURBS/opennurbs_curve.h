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

////////////////////////////////////////////////////////////////
//
//   Definition of virtual parametric curve
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_CURVE_INC_)
#define OPENNURBS_CURVE_INC_

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class ON_CLASS ON_MeshCurveParameters
{
public:
  ON_MeshCurveParameters();

  // If main_seg_count <= 0, then both these parameters are ignored.
  // If main_seg_count > 0, then sub_seg_count must be >= 1.  In this
  // case the curve will be broken into main_seg_count equally spaced
  // chords. If needed, each of these chords can be split into as many
  // sub_seg_count sub-parts if the subdivision is necessary for the
  // mesh to meet the other meshing constraints.  In particular, if
  // sub_seg_count = 0, then the curve is broken into main_seg_count
  // pieces and no further testing is performed.
  int m_main_seg_count; 
  int m_sub_seg_count;

  int m_reserved1;
  int m_reserved2;

  // Maximum angle (in radians) between unit tangents at adjacent
  // vertices.
  double m_max_ang_radians;

  // Maximum permitted value of 
  // distance chord midpoint to curve) / (length of chord)
  double m_max_chr;

  // If max_aspect < 1.0, the parameter is ignored. 
  // If 1 <= max_aspect < sqrt(2), it is treated as if 
  // max_aspect = sqrt(2).
  // This parameter controls the maximum permitted value of
  // (length of longest chord) / (length of shortest chord)
  double m_max_aspect;

  // If tolerance = 0, the parameter is ignored.
  // This parameter controls the maximum permitted value of the
  // distance from the curve to the mesh.  
  double m_tolerance;

  // If m_min_edge_length = 0, the parameter is ignored.
  // This parameter controls the minimum permitted edge length.
  double m_min_edge_length;
  
  // If max_edge_length = 0, the parameter is ignored.
  // This parameter controls the maximum permitted edge length.
  double m_max_edge_length;

  double m_reserved3;
  double m_reserved4;
};

/*
Description:
  ON_Curve is a pure virtual class for curve objects
  - Any class derived from ON_Curve should have a
      ON_OBJECT_DECLARE(ON_...);
    at the beginning of its class definition and a
      ON_OBJECT_IMPLEMENT( ON_..., ON_Curve );
    in a .cpp file.
Example:
  - See the definition of ON_NurbsCurve for an example.
*/
class ON_CLASS ON_Curve : public ON_Geometry
{
  ON_OBJECT_DECLARE(ON_Curve);

public:
  ON_Curve() ON_NOEXCEPT;
  virtual ~ON_Curve();
  ON_Curve(const ON_Curve&);
  ON_Curve& operator=(const ON_Curve&);

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_Curve( ON_Curve&& ) ON_NOEXCEPT;

  // The rvalue assignment operator calls ON_Object::operator=(ON_Object&&)
  // which could throw exceptions.  See the implementation of
  // ON_Object::operator=(ON_Object&&) for details.
  ON_Curve& operator=( ON_Curve&& );
#endif

public:
  // virtual ON_Object::DestroyRuntimeCache override
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Geometry override
  bool EvaluatePoint( const class ON_ObjRef& objref, ON_3dPoint& P ) const override;

  /*
  Description:
    Get a duplicate of the curve.
  Returns:
    A duplicate of the curve.  
  Remarks:
    The caller must delete the returned curve.
    For non-ON_CurveProxy objects, this simply duplicates the curve using
    ON_Object::Duplicate.
    For ON_CurveProxy objects, this duplicates the actual proxy curve 
    geometry and, if necessary, trims and reverse the result to that
    the returned curve's parameterization and locus match the proxy curve's.
  */
  virtual
  ON_Curve* DuplicateCurve() const;

  // Description:
  //   overrides virtual ON_Object::ObjectType.
  // Returns:
  //   ON::curve_object
  ON::object_type ObjectType() const override;

  // virtual ON_Geometry GetTightBoundingBox override		
  bool GetTightBoundingBox( class ON_BoundingBox& tight_bbox, bool bGrowBox = false, const class ON_Xform* xform = nullptr ) const override;

  /*
  Description:
    overrides virtual ON_Geometry::Transform().
    ON_Curve::Transform() calls ON_Geometry::Transform(xform),
    which calls ON_Object::TransformUserData(xform), and then
    calls this->DestroyCurveTree().
  Parameters:
    xform - [in] transformation to apply to object.
  Remarks:
    Classes derived from ON_Curve should call
    ON_Curve::Transform() to handle user data
    transformations and curve tree destruction
    and then transform their definition.
  */
  bool Transform( 
    const ON_Xform& xform
    ) override;


  ////////////////////////////////////////////////////////////////////
  // curve interface

  // Description:
  //   Gets domain of the curve
  // Parameters:
  //   t0 - [out]
  //   t1 - [out] domain is [*t0, *t1]
  // Returns:
  //   true if successful.
  bool GetDomain( double* t0, double* t1 ) const;

  // Returns:
  //   domain of the curve.
  virtual 
  ON_Interval Domain() const = 0;

  /*
  Description:
    Set the domain of the curve.
  Parameters:
    domain - [in] increasing interval
  Returns:
    true if successful.
  */
  bool SetDomain( ON_Interval domain );

  // Description:
  //   Set the domain of the curve
  // Parameters:
  //   t0 - [in]
  //   t1 - [in] new domain will be [t0,t1]
  // Returns:
  //   true if successful.
  virtual
  bool SetDomain( 
        double t0, 
        double t1 
        );


  /*
  Description:
    If this curve is closed, then modify it so that
    the start/end point is at curve parameter t.
  Parameters:
    t - [in] curve parameter of new start/end point.  The
             returned curves domain will start at t.
    min_dist - [in] Do not change if Crv(t) is within min_dist of the original seam
  Returns:
    true if successful, and seam was moved.
  */

  bool ChangeClosedCurveSeam( 
            double t, 
            double min_dist 
            );

  /*
  Description:
    If this curve is closed, then modify it so that
    the start/end point is at curve parameter t.
  Parameters:
    t - [in] curve parameter of new start/end point.  The
             returned curves domain will start at t.
  Returns:
    true if successful.
  */
  virtual 
  bool ChangeClosedCurveSeam( 
            double t 
            );

  /*
  Description:
    Change the dimension of a curve.
  Parameters:
    desired_dimension - [in]
  Returns:
    true if the curve's dimension was already desired_dimension
    or if the curve's dimension was successfully changed to
    desired_dimension.
  */
  virtual
  bool ChangeDimension(
          int desired_dimension
          );


  // Description:
  //   Get number of nonempty smooth (c-infinity) spans in curve
  // Returns:
  //   Number of nonempty smooth (c-infinity) spans.
  virtual 
  int SpanCount() const = 0;

  // Description:
  //   Get number of parameters of "knots".
  // Parameters:
  //   span_parameters - [out] an array of length SpanCount()+1 is filled in
  //       with the parameters where the curve is not smooth (C-infinity).
  // Returns:
  //   true if successful
  virtual
  bool GetSpanVector(
        double* span_parameters
        ) const = 0; // 

  //////////
  // If t is in the domain of the curve, GetSpanVectorIndex() returns the 
  // span vector index "i" such that span_vector[i] <= t <= span_vector[i+1].
  // The "side" parameter determines which span is selected when t is at the
  // end of a span.
  virtual
  bool GetSpanVectorIndex(
        double t ,               // [IN] t = evaluation parameter
        int side,                // [IN] side 0 = default, -1 = from below, +1 = from above
        int* span_vector_index,  // [OUT] span vector index
        ON_Interval* span_domain // [OUT] domain of the span containing "t"
        ) const;

  /// <summary>
  /// The curve's span vector is a stricltly monotone increasing list of doubles
  /// that are the intervals on which the curve is C-infinity.
  /// </summary>
  /// <returns>
  /// The curve's span vector.
  /// </returns>
  const ON_SimpleArray<double> SpanVector() const;

  // Description:
  //   Returns maximum algebraic degree of any span
  //   or a good estimate if curve spans are not algebraic.
  // Returns:
  //   degree
  virtual 
  int Degree() const = 0; 

  // Description:
  //   Returns maximum algebraic degree of any span
  //   or a good estimate if curve spans are not algebraic.
  // Returns:
  //   degree
  virtual 
  bool GetParameterTolerance( // returns tminus < tplus: parameters tminus <= s <= tplus
         double t,       // [IN] t = parameter in domain
         double* tminus, // [OUT] tminus
         double* tplus   // [OUT] tplus
         ) const;

  // Description:
  //   Test a curve to see if the locus if its points is a line segment.
  // Parameters:
  //   tolerance - [in] // tolerance to use when checking linearity
  // Returns:
  //   true if the ends of the curve are farther than tolerance apart
  //   and the maximum distance from any point on the curve to
  //   the line segment connecting the curve's ends is <= tolerance.
  virtual
  bool IsLinear(
        double tolerance = ON_ZERO_TOLERANCE 
        ) const;

  /*
  Description:
    Several types of ON_Curve can have the form of a polyline including
    a degree 1 ON_NurbsCurve, an ON_PolylineCurve, and an ON_PolyCurve
    all of whose segments are some form of polyline.  IsPolyline tests
    a curve to see if it can be represented as a polyline.
  Parameters:
    pline_points - [out] if not nullptr and true is returned, then the
        points of the polyline form are returned here.
    t - [out] if not nullptr and true is returned, then the parameters of
        the polyline points are returned here.
  Returns:
    @untitled table
    0        curve is not some form of a polyline
    >=2      number of points in polyline form
  */
  virtual
  int IsPolyline(
        ON_SimpleArray<ON_3dPoint>* pline_points = nullptr,
        ON_SimpleArray<double>* pline_t = nullptr
        ) const;

  // Description:
  //   Test a curve to see if the locus if its points is an arc or circle.
  // Parameters:
  //   plane - [in] if not nullptr, test is performed in this plane
  //   arc - [out] if not nullptr and true is returned, then arc parameters
  //               are filled in
  //   tolerance - [in] tolerance to use when checking
  // Returns:
  //   ON_Arc.m_angle > 0 if curve locus is an arc between
  //   specified points.  If ON_Arc.m_angle is 2.0*ON_PI, then the curve
  //   is a circle.
  virtual
  bool IsArc(
        const ON_Plane* plane = nullptr,
        ON_Arc* arc = nullptr,
        double tolerance = ON_ZERO_TOLERANCE
        ) const;

  /*
  Description:
  Parameters:
    t - [in] curve parameter
    plane - [in]
      if not nullptr, test is performed in this plane
    arc - [out]
      if not nullptr and true is returned, then arc parameters
       are filled in
    tolerance - [in]
      tolerance to use when checking
    t0 - [out]
      if not nullptr, and then *t0 is set to the parameter
      at the start of the G2 curve segment that was
      tested.
    t1 - [out]
      if not nullptr, and then *t0 is set to the parameter
      at the start of the G2 curve segment that was
      tested.
  Returns:
    True if the parameter t is on a arc segment of the curve.
  */
  bool IsArcAt( 
    double t, 
    const ON_Plane* plane = 0,
    ON_Arc* arc = 0,
    double tolerance = ON_ZERO_TOLERANCE,
    double* t0 = 0, 
    double* t1 = 0
    ) const;

  virtual
  bool IsEllipse(
      const ON_Plane* plane = nullptr,
      ON_Ellipse* ellipse = nullptr,
      double tolerance = ON_ZERO_TOLERANCE
      ) const;

  // Description:
  //   Test a curve to see if it is planar.
  // Parameters:
  //   plane - [out] if not nullptr and true is returned,
  //                 the plane parameters are filled in.
  //   tolerance - [in] tolerance to use when checkin
  // Note:
  //   If the curve is a simple planar closed curve the plane 
  //   orientation agrees with the curve orientation.
  // Returns:
  //   true if there is a plane such that the maximum distance from
  //   the curve to the plane is <= tolerance.
  virtual
  bool IsPlanar(
        ON_Plane* plane = nullptr,
        double tolerance = ON_ZERO_TOLERANCE
        ) const;

  // Description:
  //   Test a curve to see if it lies in a specific plane.
  // Parameters:
  //   test_plane - [in]
  //   tolerance - [in] tolerance to use when checking
  // Returns:
  //   true if the maximum distance from the curve to the
  //   test_plane is <= tolerance.
  virtual
  bool IsInPlane(
        const ON_Plane& test_plane,
        double tolerance = ON_ZERO_TOLERANCE
        ) const = 0;

  /*
  Description:
    Decide if it makes sense to close off this curve by moving 
    the endpoint to the start based on start-end gap size and length
    of curve as approximated by chord defined by 6 points.
  Parameters:
    tolerance - [in] maximum allowable distance between start and end.
                     if start - end gap is greater than tolerance, returns false
    min_abs_size - [in] if greater than 0.0 and none of the interior sampled
                     points are at least min_abs_size from start, returns false.
    min_rel_size - [in] if greater than 1.0 and chord length is less than 
                     min_rel_size*gap, returns false.
  Returns:
    true if start and end points are close enough based on above conditions.
  */

  bool IsClosable(
        double tolerance,
        double min_abs_size = 0.0,
        double min_rel_size = 10.0
        ) const;

  // Description:
  //   Test a curve to see if it is closed.
  // Returns:
  //   true if the curve is closed.
  virtual 
  bool IsClosed() const;

  // Description:
  //   Test a curve to see if it is periodic.
  // Returns:
  //   true if the curve is closed and at least C2 at the start/end.
  virtual 
  bool IsPeriodic() const;

  /*
  Description:
    Search for a derivative, tangent, or curvature
    discontinuity.
  Parameters:
    c - [in] type of continity to test for.
    t0 - [in] Search begins at t0. If there is a discontinuity
              at t0, it will be ignored.  This makes it 
              possible to repeatedly call GetNextDiscontinuity
              and step through the discontinuities.
    t1 - [in] (t0 != t1)  If there is a discontinuity at t1 is 
              will be ignored unless c is a locus discontinuity
              type and t1 is at the start or end of the curve.
    t - [out] if a discontinuity is found, then *t reports the
          parameter at the discontinuity.
    hint - [in/out] if GetNextDiscontinuity will be called 
       repeatedly, passing a "hint" with initial value *hint=0
       will increase the speed of the search.       
    dtype - [out] if not nullptr, *dtype reports the kind of 
        discontinuity found at *t.  A value of 1 means the first 
        derivative or unit tangent was discontinuous.  A value 
        of 2 means the second derivative or curvature was 
        discontinuous.  A value of 0 means the curve is not
        closed, a locus discontinuity test was applied, and
        t1 is at the start of end of the curve.
        If 'c', the type of continuity to test for 
        is ON::continuity::Gsmooth_continuous and the curvature changes 
        from curved to 0 or 0 to curved and there is no 
        tangency kink dtype is returns 3
    cos_angle_tolerance - [in] default = cos(1 degree) Used only
        when c is ON::continuity::G1_continuous or ON::continuity::G2_continuous.  If the
        cosine of the angle between two tangent vectors is 
        <= cos_angle_tolerance, then a G1 discontinuity is reported.
    curvature_tolerance - [in] (default = ON_SQRT_EPSILON) Used 
        only when c is ON::continuity::G2_continuous.  If K0 and K1 are 
        curvatures evaluated from above and below and 
        |K0 - K1| > curvature_tolerance, then a curvature 
        discontinuity is reported.
  Returns:
    Parametric continuity tests c = (C0_continuous, ..., G2_continuous):

      true if a parametric discontinuity was found strictly 
      between t0 and t1. Note well that all curves are 
      parametrically continuous at the ends of their domains.

    Locus continuity tests c = (C0_locus_continuous, ...,G2_locus_continuous):

      true if a locus discontinuity was found strictly between
      t0 and t1 or at t1 is the at the end of a curve.
      Note well that all open curves (IsClosed()=false) are locus
      discontinuous at the ends of their domains.  All closed 
      curves (IsClosed()=true) are at least C0_locus_continuous at 
      the ends of their domains.
  */
  virtual
  bool GetNextDiscontinuity( 
                  ON::continuity c,
                  double t0,
                  double t1,
                  double* t,
                  int* hint=nullptr,
                  int* dtype=nullptr,
                  double cos_angle_tolerance=ON_DEFAULT_ANGLE_TOLERANCE_COSINE,
                  double curvature_tolerance=ON_SQRT_EPSILON
                  ) const;

#if defined(OPENNURBS_PLUS)

  /// <summary>
  /// Find the next curve kink in the search domain.
  /// </summary>
  /// <param name="search_domain">
  /// An increasing interval that is contained in this->Domain().
  /// </param>
  /// <param name="kink_definition">
  /// </param>
  /// <returns>
  /// If a kink is found, then a parameter strictly inside search_domain
  /// is returned where the first kink was found.
  /// If no kink exists in the domain, the search_domain.Max() is returned.
  /// If the input is invalid, then ON_DBL_QNAN is returned.
  /// </returns>
  double NextCurveKink(
    ON_Interval search_domain,
    const class ON_CurveKinkDefinition& kink_definition
  ) const;
#endif

  /*
  Description:
    Test continuity at a curve parameter value.
  Parameters:
    c - [in] type of continuity to test for. Read ON::continuity
             comments for details.
    t - [in] parameter to test
    hint - [in] evaluation hint
    point_tolerance - [in] if the distance between two points is
        greater than point_tolerance, then the curve is not C0.
    d1_tolerance - [in] if the difference between two first derivatives is
        greater than d1_tolerance, then the curve is not C1.
    d2_tolerance - [in] if the difference between two second derivatives is
        greater than d2_tolerance, then the curve is not C2.
    cos_angle_tolerance - [in] default = cos(1 degree) Used only when
        c is ON::continuity::G1_continuous or ON::continuity::G2_continuous.  If the cosine
        of the angle between two tangent vectors 
        is <= cos_angle_tolerance, then a G1 discontinuity is reported.
    curvature_tolerance - [in] (default = ON_SQRT_EPSILON) Used only when
        c is ON::continuity::G2_continuous or ON::continuity::Gsmooth_continuous.  
        ON::continuity::G2_continuous:
          If K0 and K1 are curvatures evaluated
          from above and below and |K0 - K1| > curvature_tolerance,
          then a curvature discontinuity is reported.
        ON::continuity::Gsmooth_continuous:
          If K0 and K1 are curvatures evaluated from above and below
          and the angle between K0 and K1 is at least twice angle tolerance
          or ||K0| - |K1|| > (max(|K0|,|K1|) > curvature_tolerance,
          then a curvature discontinuity is reported.
  Returns:
    true if the curve has at least the c type continuity at 
    the parameter t.
  */
  virtual
  bool IsContinuous(
    ON::continuity c,
    double t, 
    int* hint = nullptr,
    double point_tolerance=ON_ZERO_TOLERANCE,
    double d1_tolerance=ON_ZERO_TOLERANCE,
    double d2_tolerance=ON_ZERO_TOLERANCE,
    double cos_angle_tolerance=ON_DEFAULT_ANGLE_TOLERANCE_COSINE,
    double curvature_tolerance=ON_SQRT_EPSILON
    ) const;


  // Description:
  //   Reverse the direction of the curve.
  // Returns:
  //   true if curve was reversed.
  // Remarks:
  //   If reversed, the domain changes from [a,b] to [-b,-a]
  virtual 
  bool Reverse()=0;


  /*
  Description:
    Force the curve to start at a specified point.
  Parameters:
    start_point - [in]
  Returns:
    true if successful.
  Remarks:
    Some end points cannot be moved.  Be sure to check return
    code. 
    ON_Curve::SetStartPoint() returns true if start_point is the same as the start of the curve,
    false otherwise. 
  See Also:
    ON_Curve::SetEndPoint
    ON_Curve::PointAtStart
    ON_Curve::PointAtEnd
  */
  virtual
  bool SetStartPoint(
          ON_3dPoint start_point
          );

  /*
  Description:
    Force the curve to end at a specified point.
  Parameters:
    end_point - [in]
  Returns:
    true if successful.
  Remarks:
    Some end points cannot be moved.  Be sure to check return
    code.
    ON_Curve::SetEndPoint() returns true if end_point is the same as the end of the curve,
    false otherwise. 
  See Also:
    ON_Curve::SetStartPoint
    ON_Curve::PointAtStart
    ON_Curve::PointAtEnd
  */
  virtual
  bool SetEndPoint(
          ON_3dPoint end_point
          );
  
  // Description:
  //   Evaluate point at a parameter.
  // Parameters:
  //   t - [in] evaluation parameter
  // Returns:
  //   Point (location of curve at the parameter t).
  // Remarks:
  //   No error handling.
  // See Also:
  //   ON_Curve::EvPoint
  //   ON_Curve::PointAtStart
  //   ON_Curve::PointAtEnd
  ON_3dPoint  PointAt( 
                double t 
                ) const;

  // Description:
  //   Evaluate point at the start of the curve.
  // Parameters:
  //   t - [in] evaluation parameter
  // Returns:
  //   Point (location of the start of the curve.)
  // Remarks:
  //   No error handling.
  // See Also:
  //   ON_Curve::PointAt
  ON_3dPoint  PointAtStart() const;

  // Description:
  //   Evaluate point at the end of the curve.
  // Parameters:
  //   t - [in] evaluation parameter
  // Returns:
  //   Point (location of the end of the curve.)
  // Remarks:
  //   No error handling.
  // See Also:
  //   ON_Curve::PointAt
  ON_3dPoint  PointAtEnd() const;

  // Description:
  //   Evaluate first derivative at a parameter.
  // Parameters:
  //   t - [in] evaluation parameter
  // Returns:
  //   First derivative of the curve at the parameter t.
  // Remarks:
  //   No error handling.
  // See Also:
  //   ON_Curve::Ev1Der
  ON_3dVector DerivativeAt(
                double t 
                ) const;

  // Description:
  //   Evaluate unit tangent vector at a parameter.
  // Parameters:
  //   t - [in] evaluation parameter
  // Returns:
  //   Unit tangent vector of the curve at the parameter t.
  // Remarks:
  //   No error handling.
  // See Also:
  //   ON_Curve::EvTangent
  ON_3dVector TangentAt(
                double t 
                ) const;

  // Description:
  //   Evaluate the curvature vector at a parameter.
  // Parameters:
  //   t - [in] evaluation parameter
  // Returns:
  //   curvature vector of the curve at the parameter t.
  // Remarks:
  //   No error handling.
  // See Also:
  //   ON_Curve::EvCurvature
  ON_3dVector CurvatureAt(
                double t
                ) const;

  // Description:
 //   Evaluate the signed curvature of a planar curve at a parameter.
 // Parameters:
 //   t - [in] evaluation parameter
 //   plane_normal - [in] oriented plane unit normal, 
 //                   defaults to ON_3dVector(0,0,1) for curve in xy-plane
 // Returns:
 //   signed curvature of a planar curve at the parameter t.
 // Remarks:
 //   No error handling.
 // See Also:
 //   ON_Curve::EvSignedCurvature
  double SignedCurvatureAt(
    double t,
    const ON_3dVector* plane_normal = nullptr
  ) const;

  // Description:
  //   Return a 3d frame at a parameter.
  // Parameters:
  //   t - [in] evaluation parameter
  //   plane - [out] the frame is returned here
  // Returns:
  //   true if successful
  // See Also:
  //   ON_Curve::PointAt, ON_Curve::TangentAt,
  //   ON_Curve::Ev1Der, Ev2Der
  bool FrameAt( double t, ON_Plane& plane) const;

  // Description:
  //   Evaluate point at a parameter with error checking.
  // Parameters:
  //   t - [in] evaluation parameter
  //   point - [out] value of curve at t
  //   side - [in] optional - determines which side to evaluate from
  //               =0   default
  //               <0   to evaluate from below, 
  //               >0   to evaluate from above
  //   hint - [in/out] optional evaluation hint used to speed repeated evaluations
  // Returns:
  //   false if unable to evaluate.
  // See Also:
  //   ON_Curve::PointAt
  //   ON_Curve::EvTangent
  //   ON_Curve::Evaluate
  bool EvPoint(
         double t,
         ON_3dPoint& point, 
         int side = 0,
         int* hint = 0
         ) const;

  // Description:
  //   Evaluate first derivative at a parameter with error checking.
  // Parameters:
  //   t - [in] evaluation parameter
  //   point - [out] value of curve at t
  //   first_derivative - [out] value of first derivative at t
  //   side - [in] optional - determines which side to evaluate from
  //               =0   default
  //               <0   to evaluate from below, 
  //               >0   to evaluate from above
  //   hint - [in/out] optional evaluation hint used to speed repeated evaluations
  // Returns:
  //   false if unable to evaluate.
  // See Also:
  //   ON_Curve::EvPoint
  //   ON_Curve::Ev2Der
  //   ON_Curve::EvTangent
  //   ON_Curve::Evaluate
  bool Ev1Der(
         double t,
         ON_3dPoint& point,
         ON_3dVector& first_derivative,
         int side = 0,
         int* hint = 0
         ) const;

  // Description:
  //   Evaluate second derivative at a parameter with error checking.
  // Parameters:
  //   t - [in] evaluation parameter
  //   point - [out] value of curve at t
  //   first_derivative - [out] value of first derivative at t
  //   second_derivative - [out] value of second derivative at t
  //   side - [in] optional - determines which side to evaluate from
  //               =0   default
  //               <0   to evaluate from below, 
  //               >0   to evaluate from above
  //   hint - [in/out] optional evaluation hint used to speed repeated evaluations
  // Returns:
  //   false if unable to evaluate.
  // See Also:
  //   ON_Curve::Ev1Der
  //   ON_Curve::EvCurvature
  //   ON_Curve::Evaluate
  bool Ev2Der(
         double t,
         ON_3dPoint& point,
         ON_3dVector& first_derivative,
         ON_3dVector& second_derivative,
         int side = 0,
         int* hint = 0
         ) const;

  /*
  Description:
    Evaluate unit tangent at a parameter with error checking.
  Parameters:
    t - [in] evaluation parameter
    point - [out] value of curve at t
    tangent - [out] value of unit tangent
    side - [in] optional - determines which side to evaluate from
                =0   default
                <0   to evaluate from below, 
                >0   to evaluate from above
    hint - [in/out] optional evaluation hint used to speed repeated evaluations
  Returns:
    false if unable to evaluate.
  See Also:
    ON_Curve::TangentAt
    ON_Curve::Ev1Der
  */
  bool EvTangent(
         double t,
         ON_3dPoint& point,
         ON_3dVector& tangent,
         int side = 0,
         int* hint = 0
         ) const;

  /*
  Description:
    Evaluate unit tangent and curvature at a parameter with error checking.
  Parameters:
    t - [in] evaluation parameter
    point - [out] value of curve at t
    tangent - [out] value of unit tangent
    kappa - [out] value of curvature vector
    side - [in] optional - determines which side to evaluate from
                =0   default
                <0   to evaluate from below, 
                >0   to evaluate from above
    hint - [in/out] optional evaluation hint used to speed repeated evaluations
  Returns:
    false if unable to evaluate.
  See Also:
    ON_Curve::CurvatureAt
    ON_Curve::Ev2Der
    ON_EvCurvature
  */
  bool EvCurvature(
         double t,
         ON_3dPoint& point,
         ON_3dVector& tangent,
         ON_3dVector& kappa,
         int side = 0,
         int* hint = 0
         ) const;

  /*
    Description:
      Evaluate unit tangent and signed curvature (also called oriented curvature)  of a planar 
      curve at a parameter with error checking.
    Parameters:
      t - [in] evaluation parameter
      point - [out] value of curve at t
      tangent - [out] value of unit tangent
      kappa - [out] value of signed curvature 
      normal - [in] oriented unit normal of the plane containing the curve.
                    default of nullptr is interpreted as ON_3dVector(0,0,1)
      side - [in] optional - determines which side to evaluate from
                  =0   default
                  <0   to evaluate from below,
                  >0   to evaluate from above
      hint - [in/out] optional evaluation hint used to speed repeated evaluations
    Returns:
      false if unable to evaluate.
    Notes:
      Computes the Triple product T o ( K X N)
      where T is the unit tangent, K is the curvature vector
      and N is the plane unit normal.  If the curve is planar this is the signed curvature for the given 
      plane orientation.  The normal defaults to (0,0,1)  for curves in the x-y plane.
    See Also:
      ON_Curve::CurvatureAt
      ON_Curve::Ev2Der
      ON_EvCurvature
    */
  bool EvSignedCurvature(
    double t,
    ON_3dPoint& point,
    ON_3dVector& tangent,
    double& kappa,
    const ON_3dVector* normal = nullptr,
    int side = 0,
    int* hint = 0
  ) const;


  /*
  Description:
    This evaluator actually does all the work.  The other ON_Curve
    evaluation tools call this virtual function.
  Parameters:
    t - [in] evaluation parameter ( usually in Domain() ).
    der_count - [in] (>=0) number of derivatives to evaluate
    v_stride - [in] (>=Dimension()) stride to use for the v[] array
    v - [out] array of length (der_count+1)*v_stride
        curve(t) is returned in (v[0],...,v[m_dim-1]),
        curve'(t) is returned in (v[v_stride],...,v[v_stride+m_dim-1]),
        curve"(t) is returned in (v[2*v_stride],...,v[2*v_stride+m_dim-1]),
        etc.
    side - [in] optional - determines which side to evaluate from
                =0   default
                <0   to evaluate from below, 
                >0   to evaluate from above
    hint - [in/out] optional evaluation hint used to speed repeated evaluations
  Returns:
    false if unable to evaluate.
  See Also:
    ON_Curve::EvPoint
    ON_Curve::Ev1Der
    ON_Curve::Ev2Der
  */
  virtual 
  bool Evaluate(
         double t,
         int der_count,
         int v_stride,
         double* v,
         int side = 0,
         int* hint = 0
         ) const = 0;

  
#if defined(OPENNURBS_PLUS)
  //////////
  // Find parameter of the point on a curve that is closest to test_point.
  // If the maximum_distance parameter is > 0, then only points whose distance
  // to the given point is <= maximum_distance will be returned.  Using a 
  // positive value of maximum_distance can substantially speed up the search.
  // If the sub_domain parameter is not nullptr, then the search is restricted
  // to the specified portion of the curve.
  //
  // true if returned if the search is successful.  false is returned if
  // the search fails.
  virtual
  bool GetClosestPoint( 
          const ON_3dPoint& test_point,
          double* t,       // parameter of local closest point returned here
          double maximum_distance = 0.0,  // maximum_distance
          const ON_Interval* sub_domain = nullptr // sub_domain
          ) const;

  //////////
  // Find parameter of the point on a curve that is locally closest to 
  // the test_point.  The search for a local close point starts at 
  // seed_parameter. If the sub_domain parameter is not nullptr, then
  // the search is restricted to the specified portion of the curve.
  //
  // true if returned if the search is successful.  false is returned if
  // the search fails.
  virtual
  bool GetLocalClosestPoint( 
            const ON_3dPoint& test_point,
            double seed_parameter,
            double* t,
            const ON_Interval* sub_domain = 0
            ) const;

  /*
  Description:
    Find curve's self intersection points.
  Parameters:
    x - [out] 
       Intersection events are appended to this array.
       Only intersection points are reported.
       Overlaps are not reported.
    intersection_tolerance - [in]
    curve_domain - [in] optional restriction
  Returns:
    Number of intersection events appended to x.
  Remarks:
    Overlaps were are not reported. 
    Overlaps are now reported as of Rhino 8.0
  */
  virtual
  int IntersectSelf( 
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          const ON_Interval* curve_domain = 0
          ) const;

  /*
  Description:
    Intersect this curve with curveB.
  Parameters:
    curveB - [in]
    x - [out] Intersection events are appended to this array.
    intersection_tolerance - [in]  If the distance from a point
      on this curve to curveB is <= intersection tolerance,
      then the point will be part of an intersection event.
      If the input intersection_tolerance <= 0.0, then 0.001 is used.
    overlap_tolerance - [in] If t1 and t2 are parameters of this 
      curve's intersection events and the distance from curve(t) to 
      curveB is <= overlap_tolerance for every t1 <= t <= t2,
      then the event will be returned as an overlap event.
      If the input overlap_tolerance <= 0.0, then 
      intersection_tolerance*2.0 is used,  Otherwise, overlap
      tolerance must be >= intersection_tolerance.
    curveA_domain - [in] optional restriction on this curve's domain
    curveB_domain - [in] optional restriction on curveB domain
  Returns:
    Number of intersection events appended to x.
  */
  int IntersectCurve( 
          const ON_Curve* curveB,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curveA_domain = 0,
          const ON_Interval* curveB_domain = 0
          ) const;

  /*
  Description:
    Intersect this curve with surfaceB.

  Parameters:
    surfaceB - [in]

    x - [out] 
      Intersection events are appended to this array.
    intersection_tolerance - [in]  
      If the distance from a point on this curve to the surface 
      is <= intersection tolerance, then the point will be part 
      of an intersection event, or there is an intersection event
      the point leads to. If the input intersection_tolerance <= 0.0,
      then 0.001 is used.

    overlap_tolerance - [in] 
      If the input overlap_tolerance <= 0.0, then 
      2.0*intersection_tolerance is used.  Otherwise, overlap
      tolerance must be >= intersection_tolerance.
      In all cases, the intersection calculation is performed 
      with an overlap_tolerance that is >= intersection_tolerance.
      If t1 and t2 are curve parameters of intersection events 
      and the distance from curve(t) to the surface 
      is <= overlap_tolerance for every t1 <= t <= t2, then the 
      event will be returned as an overlap event.
       
    curveA_domain - [in] 
      optional restriction on this curve's domain

    surfaceB_udomain - [in]
      optional restriction on surfaceB u domain

    surfaceB_vdomain - [in]
      optional restriction on surfaceB v domain

  Returns:
    Number of intersection events appended to x.
  */
  int IntersectSurface( 
          const ON_Surface* surfaceB,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curveA_domain = 0,
          const ON_Interval* surfaceB_udomain = 0,
          const ON_Interval* surfaceB_vdomain = 0
          ) const;


  /*
  Description:
    Intersect this curve with an infinite plane.

  Parameters:
    plane_equation - [in]

    x - [out] 
      Intersection events are appended to this array.
    intersection_tolerance - [in]  
      If the distance from a point on this curve to the surface 
      is <= intersection tolerance, then the point will be part 
      of an intersection event, or there is an intersection event
      the point leads to. If the input intersection_tolerance <= 0.0,
      then 0.001 is used.

    overlap_tolerance - [in] 
      If the input overlap_tolerance <= 0.0, then 
      2.0*intersection_tolerance is used.  Otherwise, overlap
      tolerance must be >= intersection_tolerance.
      In all cases, the intersection calculation is performed 
      with an overlap_tolerance that is >= intersection_tolerance.
      If t1 and t2 are curve parameters of intersection events 
      and the distance from curve(t) to the surface 
      is <= overlap_tolerance for every t1 <= t <= t2, then the 
      event will be returned as an overlap event.
       
    curve_domain - [in] 
      optional restriction on this curve's domain

  Returns:
    Number of intersection events appended to x.

  NOTE: 
    The surface parameters, m_b, of x are not useful.  If you got the plane equation
    from an ON_Plane, the m_b will most likely not be correct with respect to the original plane.
  */
  int IntersectPlane( 
          ON_PlaneEquation plane_equation,
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0,
          const ON_Interval* curve_domain = 0
          ) const;



  /*
  Description:
    Get the length of the curve.
  Parameters:
    length - [out] length returned here.
    fractional_tolerance - [in] desired fractional precision.
        fabs(("exact" length from start to t) - arc_length)/arc_length <= fractional_tolerance
    sub_domain - [in] If not nullptr, the calculation is performed on
        the specified sub-domain of the curve (must be non-decreasing)
  Returns:
    true if returned if the length calculation is successful.
    false is returned if the length is not calculated.
  Remarks:
    The arc length will be computed so that
    (returned length - real length)/(real length) <= fractional_tolerance
    More simply, if you want N significant figures in the answer, set the
    fractional_tolerance to 1.0e-N.  For "nice" curves, 1.0e-8 works
    fine.  For very high degree NURBS and NURBS with bad parameterizations,
    use larger values of fractional_tolerance.
  */
  virtual
  bool GetLength(
          double* length,
          double fractional_tolerance = 1.0e-8,
          const ON_Interval* sub_domain = nullptr
          ) const;
#endif

  /*
  Parameters:
    min_length -[in]
      minimum length of a linear span
    tolerance -[in]
      distance tolerance to use when checking linearity.
  Returns 
    true if the span is a non-degenerate line.  This means:
    - dimension = 2 or 3
    - The length of the the line segment from the span's initial 
      point to the span's control point is >= min_length.
    - The maximum distance from the line segment to the span
    is <= tolerance and the span increases monotonically
    in the direction of the line segment.
  */
  bool FirstSpanIsLinear( 
    double min_length,
    double tolerance
    ) const;

  bool LastSpanIsLinear( 
    double min_length,
    double tolerance
    ) const;

  bool FirstSpanIsLinear( 
    double min_length,
    double tolerance,
    ON_Line* span_line
    ) const;

  bool LastSpanIsLinear( 
    double min_length,
    double tolerance,
    ON_Line* span_line
    ) const;

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Used to quickly find short curves.
  Parameters:
    tolerance - [in] (>=0)
    sub_domain - [in] If not nullptr, the test is performed
      on the interval that is the intersection of 
      sub_domain with Domain().
  Returns:
    True if the length of the curve is <= tolerance.
  Remarks:
    Faster than calling Length() and testing the
    result.
  */
  virtual
  bool IsShort(
    double tolerance,
    const ON_Interval* sub_domain = 0,
    double* length_estimate = 0
    ) const;

  /*
  Description:
    Looks for segments that are shorter than tolerance
    that can be removed. If bRemoveShortSegments is true,
    then the short segments are removed. Does not change the 
    domain, but it will change the relative parameterization.
  Parameters:
    tolerance - [in]
    bRemoveShortSegments - [in] If true, then short segments
                                are removed.
  Returns:
    True if removable short segments can were found.
    False if no removable short segments can were found.
  */
  virtual
  bool RemoveShortSegments(
    double tolerance,
    bool bRemoveShortSegments = true
    );

  /*
  Description:
    Get the parameter of the point on the curve that is a 
    prescribed arc length from the start of the curve.
  Parameters:
    s - [in] normalized arc length parameter.  E.g., 0 = start
         of curve, 1/2 = midpoint of curve, 1 = end of curve.
    t - [out] parameter such that the length of the curve
       from its start to t is arc_length.
    fractional_tolerance - [in] desired fractional precision.
        fabs(("exact" length from start to t) - arc_length)/arc_length <= fractional_tolerance
    sub_domain - [in] If not nullptr, the calculation is performed on
        the specified sub-domain of the curve.
  Returns:
    true if successful
  */
  virtual
  bool GetNormalizedArcLengthPoint(
          double s,
          double* t,
          double fractional_tolerance = 1.0e-8,
          const ON_Interval* sub_domain = nullptr
          ) const;

  /*
  Description:
    Get the parameter of the point on the curve that is a 
    prescribed arc length from the start of the curve.
  Parameters:
    count - [in] number of parameters in s.
    s - [in] array of normalized arc length parameters. E.g., 0 = start
         of curve, 1/2 = midpoint of curve, 1 = end of curve.
    t - [out] array of curve parameters such that the length of the 
       curve from its start to t[i] is s[i]*curve_length.
    absolute_tolerance - [in] if absolute_tolerance > 0, then the difference
        between (s[i+1]-s[i])*curve_length and the length of the curve
        segment from t[i] to t[i+1] will be <= absolute_tolerance.
    fractional_tolerance - [in] desired fractional precision for each segment.
        fabs("true" length - actual length)/(actual length) <= fractional_tolerance
    sub_domain - [in] If not nullptr, the calculation is performed on
        the specified sub-domain of the curve.  A 0.0 s value corresponds to
        sub_domain->Min() and a 1.0 s value corresponds to sub_domain->Max().
  Returns:
    true if successful
  */
  virtual
  bool GetNormalizedArcLengthPoints(
          int count,
          const double* s,
          double* t,
          double absolute_tolerance = 0.0,
          double fractional_tolerance = 1.0e-8,
          const ON_Interval* sub_domain = nullptr
          ) const;
#endif

  // Description:
  //   Removes portions of the curve outside the specified interval.
  // Parameters:
  //   domain - [in] interval of the curve to keep.  Portions of the
  //      curve before curve(domain[0]) and after curve(domain[1]) are
  //      removed.
  // Returns:
  //   true if successful.
  virtual
  bool Trim(
    const ON_Interval& domain
    );

  // Description:
  //   Pure virtual function. Default returns false.
  //   Where possible, analytically extends curve to include domain.
  // Parameters:
  //   domain - [in] if domain is not included in curve domain, 
  //   curve will be extended so that its domain includes domain.  
  //   Will not work if curve is closed. Original curve is identical
  //   to the restriction of the resulting curve to the original curve domain, 
  // Returns:
  //   true if successful.
  virtual
  bool Extend(
    const ON_Interval& domain
    );

  /*
  Description:
    Splits (divides) the curve at the specified parameter.  
    The parameter must be in the interior of the curve's domain.
    The pointers passed to Split must either be nullptr or point to
    an ON_Curve object of the same type.  If the pointer is nullptr,
    then a curve will be created in Split().  You may pass "this"
    as left_side or right_side.
  Parameters:
    t - [in] parameter to split the curve at in the
             interval returned by Domain().
    left_side - [out] left portion of curve returned here
    right_side - [out] right portion of curve returned here
	Returns:
		true	- The curve was split into two pieces.  
		false - The curve could not be split.  For example if the parameter is
						too close to an endpoint.

  Example:
    For example, if crv were an ON_NurbsCurve, then

          ON_NurbsCurve right_side;
          crv.Split( crv.Domain().Mid() &crv, &right_side );

    would split crv at the parametric midpoint, put the left side
    in crv, and return the right side in right_side.
  */
  virtual
  bool Split(
      double t,
      ON_Curve*& left_side,
      ON_Curve*& right_side
    ) const;

  /*
  Description:
    Get a NURBS curve representation of this curve.
  Parameters:
    nurbs_curve - [out] NURBS representation returned here
    tolerance - [in] tolerance to use when creating NURBS
        representation.
    subdomain - [in] if not nullptr, then the NURBS representation
        for this portion of the curve is returned.
  Returns:
    0   unable to create NURBS representation
        with desired accuracy.
    1   success - returned NURBS parameterization
        matches the curve's to wthe desired accuracy
    2   success - returned NURBS point locus matches
        the curve's to the desired accuracy and the
        domain of the NURBS curve is correct.  On
        However, This curve's parameterization and
        the NURBS curve parameterization may not 
        match to the desired accuracy.  This situation
        happens when getting NURBS representations of
        curves that have a transcendental parameterization
        like circles
  Remarks:
    This is a low-level virtual function.  If you do not need
    the parameterization information provided by the return code,
    then ON_Curve::NurbsCurve may be easier to use.
  See Also:
    ON_Curve::NurbsCurve
  */
  virtual
  int GetNurbForm(
        ON_NurbsCurve& nurbs_curve,
        double tolerance = 0.0,
        const ON_Interval* subdomain = nullptr
        ) const;
  /*
  Description:
    Does a NURBS curve representation of this curve.
  Parameters:
  Returns:
    0   unable to create NURBS representation
        with desired accuracy.
    1   success - NURBS parameterization
        matches the curve's to wthe desired accuracy
    2   success - NURBS point locus matches
        the curve's and the
        domain of the NURBS curve is correct.  
        However, This curve's parameterization and
        the NURBS curve parameterization may not 
        match.  This situation
        happens when getting NURBS representations of
        curves that have a transcendental parameterization
        like circles
  Remarks:
    This is a low-level virtual function.  
  See Also:
    ON_Curve::GetNurbForm
    ON_Curve::NurbsCurve
  */
  virtual
  int HasNurbForm() const;

  /*
  Description:
    Get a NURBS curve representation of this curve.
  Parameters:
    pNurbsCurve - [in/out] if not nullptr, this ON_NurbsCurve
    will be used to store the NURBS representation
    of the curve will be returned.
    tolerance - [in] tolerance to use when creating NURBS
        representation.
    subdomain - [in] if not nullptr, then the NURBS representation
        for this portion of the curve is returned.
  Returns:
    nullptr or a NURBS representation of the curve.
  Remarks:
    See ON_Surface::GetNurbForm for important details about
    the NURBS surface parameterization.
  See Also:
    ON_Curve::GetNurbForm
  */
  ON_NurbsCurve* NurbsCurve(
        ON_NurbsCurve* pNurbsCurve = nullptr,
        double tolerance = 0.0,
        const ON_Interval* subdomain = nullptr
        ) const;

  // Description:
  //   Convert a NURBS curve parameter to a curve parameter
  //
  // Parameters:
  //   nurbs_t - [in] nurbs form parameter
  //   curve_t - [out] curve parameter
  //
  // Remarks:
  //   If GetNurbForm returns 2, this function converts the curve
  //   parameter to the NURBS curve parameter.
  //
  // See Also:
  //   ON_Curve::GetNurbForm, ON_Curve::GetNurbFormParameterFromCurveParameter
  virtual
  bool GetCurveParameterFromNurbFormParameter(
        double nurbs_t,
        double* curve_t
        ) const;

  // Description:
  //   Convert a curve parameter to a NURBS curve parameter.
  //
  // Parameters:
  //   curve_t - [in] curve parameter
  //   nurbs_t - [out] nurbs form parameter
  //
  // Remarks:
  //   If GetNurbForm returns 2, this function converts the curve
  //   parameter to the NURBS curve parameter.
  //
  // See Also:
  //   ON_Curve::GetNurbForm, ON_Curve::GetCurveParameterFromNurbFormParameter
  virtual
  bool GetNurbFormParameterFromCurveParameter(
        double curve_t,
        double* nurbs_t
        ) const;


  // Description:
  //   Destroys the runtime curve tree used to speed closest
  //   point and intersection calculations.
  // Remarks:
  //   If the geometry of the curve is modified in any way,
  //   then call DestroyCurveTree();  The curve tree is 
  //   created as needed.
  void DestroyCurveTree();

#if defined(OPENNURBS_PLUS)
  // Description:
  //   Get the runtime curve tree used to speed closest point
  //   and intersection calculations.
  // Returns:
  //   Pointer to the curve tree.
  const class ON_CurveTree* CurveTree() const;

  virtual
  class ON_CurveTree* CreateCurveTree() const;

  /*
  Description:
    Calculate length mass properties of the curve.
  Parameters:
    mp - [out] 
    bLength - [in] true to calculate length
    bFirstMoments - [in] true to calculate volume first moments,
                         length, and length centroid.
    bSecondMoments - [in] true to calculate length second moments.
    bProductMoments - [in] true to calculate length product moments.
  Returns:
    True if successful.
  */
  bool LengthMassProperties(
    class ON_MassProperties& mp, 
    bool bLength = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    double rel_tol = 1.0e-6,
    double abs_tol = 1.0e-6
    ) const;

  /*
  Description:
    Calculate area mass properties of a curve.  The curve should
    be planar.
  Parameters:
    base_point - [in] 
      A point on the plane that contains the curve.  To get
      the best results, the point should be in the near the
      curve's centroid.
      
      When computing the area, area centroid, or area first
      moments of a planar area whose boundary is defined by 
      several curves, pass the same base_point and plane_normal
      to each call to AreaMassProperties.  The base_point must 
      be in the plane of the curves.  
      
      When computing the area second moments or area product
      moments of a planar area whose boundary is defined by several
      curves, you MUST pass the entire area's centroid as the
      base_point and the input mp parameter must contain the
      results of a previous call to 
      AreaMassProperties(mp,true,true,false,false,base_point).
      In particular, in this case, you need to make two sets of
      calls; use first set to calculate the area centroid and
      the second set calculate the second moments and product 
      moments.
    plane_normal - [in]
      nonzero unit normal to the plane of integration.  If a closed
      curve has counter clock-wise orientation with respect to
      this normal, the area will be positive.  If the a closed curve
      has clock-wise orientation with respect to this normal, the
      area will be negative.
    mp - [out] 
    bArea - [in] true to calculate volume
    bFirstMoments - [in] true to calculate area first moments,
                         area, and area centroid.
    bSecondMoments - [in] true to calculate area second moments.
    bProductMoments - [in] true to calculate area product moments.
  Notes:
		Here is an example of using a curve to compute the mass properties of the 
		planar area bounded by the curve.

	  ON_Curve& crv = ...;
		ON_Plane plane;
		double tol = .001;
		ON_MassProperties mp;
		if( crv.IsClosed() && crv.IsPlanar( plane, tol)) // ensure curve is closed and planar
		{
			if( ON_ClosedCurveOrientation( crv, plane)<0)	// get correct orientation of plane
				plane.Flip();
			ON_BoundingBox bbox = crv.BoundingBox();			// choose a reasonable base_point
			ON_3dPoint base_point = plane.ClosestPointTo(bbox.Center());
		}
		if( crv.AreaMassProperties( base_point, plane.Normal(), mp))
		{
			// mp contains area mass properties of planar region bounded crv.
		}
  Returns:
    True if successful.
  */
  bool AreaMassProperties(
    ON_3dPoint base_point,
    ON_3dVector plane_normal,
    ON_MassProperties& mp, 
    bool bArea = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    double rel_tol = 1.0e-6,
    double abs_tol = 1.0e-6
    ) const;

  /*
  Description:
    Mesh a curve into line segments.
  Parameters:
    mp - [in] 
      Parameters that determine how the curve will be
      approximated by a polyline.
    polyline - [in]
      If not nullptr, the polyline approximation will be appended
      to this polyline.
    bSkipFirstPoint - [in]
      If true, the starting point of the approximation
      will not be added to the returned polyline.  This
      parameter is useful when getting a polyline approximation
      of a sequence of contiguous curves.
    domain - [in]
      If not nullptr, the polyline approximation will be restricted
      to this domain.
  Returns:
    A pointer to the polyline approximation.
  */
  class ON_PolylineCurve* MeshCurve( 
    ON_MeshCurveParameters& mp,
    ON_PolylineCurve* polyline,
    bool bSkipFirstPoint,
    const ON_Interval* domain
    ) const;

  /*
  Description:
    Create a cubic nurbs surface that interpolates a list of curves.    

  Parameters:
    point_count - [in] >= 2
      number of points
    point_dim - [in] >= 1
      dimension of points
    point_stride - [in] >= point_dim
      The first coordinate of the i-th point is point_list[i*point_stride].
    point_list - [in]
      array of point coordinates

    k - [in] (0.0 <= k) or k = ON_UNSET_VALUE
      k determines how the curve's m_knot[0] values
      are calculated. Most frequently, 0.0, 0.5, or 1.0 should be used.
        0.0: uniform
        0.5: sqrt(chord length)
        1.0: chord length
          In general, when k >= 0.0, then spacing is pow(d,k), where d is the
          average distance between the curves defining the span.
        ON_UNSET_VALUE: the interpolation knot vector is explicitly specified.
          The knots in the interpolated direction are specified.  You must
          understand the mathematics of NURBS surfaces to use this option.
          To specify an explicit knot vector for the interpolation, the 
          nurbs_curve parameter must be non-null, nurbs_curve->m_order 
          must be 4.  The value of nurbs_curve->m_cv_count must be set as
          describe in the is_closed parameter section.
          The array nurbs_curve->m_knot[0][] must have length 
          nurbs_curve->m_cv_count[0]+2, and the values in
          nurbs_curve->m_knot[0][2, ..., nurbs_curve->m_cv_count[0]-1]
          must be strictly increasing.
    is_closed - [in]
      0: open
         curve_count must be at least 2.
         The resulting nurbs_curve will have m_cv_count = point_count+2.
      1: closed
         curve_count must be at least 3. Do not include a duplicate of the 
         start point as the last point in the list.
         The resulting nurbs_curve will have m_cv_count = point_count+3.
      2: periodic
         curve_count must be at least 3.
         The resulting nurbs_curve will have m_cv_count = point_count+3.

    start_shape - [in]
    end_shape - [in]
      The start_shape and end_shape parameters determine the
      starting and ending shape of the lofted surface.
      
      Simple shapes:
        The simple end conditions calculate the rows of free
        control points based on the locations of the input
        curves and do not require additional input information.
          ON::cubic_loft_ec_quadratic: quadratic
          ON::cubic_loft_ec_linear: linear
          ON::cubic_loft_ec_cubic: cubic
          ON::cubic_loft_ec_natural: natural (zero 2nd derivative)

      Explicit shapes:
        ON::cubic_loft_ec_unit_tangent: unit tangent is specified
        ON::cubic_loft_ec_1st_derivative: first derivative is specified
        ON::cubic_loft_ec_2nd_derivative: second derivative is specified
        ON::cubic_loft_ec_free_cv: free control vertex is specified

        In order to specify explicit end conditions, point_count must
        be at least 3, is_closed must be 0 or 1, the nurbs_curve
        parameter must be non-null, the nurbs_curve control
        points must be allocated, and nurbs_curve->m_cv_count
        must set as described in the is_closed parameter section.
        If the start_shape is explicit, then nurbs_curve->CV(1)
        must be set to the desired value.  If the end_shape is explicit,
        then nurbs_curve->CV(nurbs_curve->m_cv_count-2) must be
        set to the desired value.
        A good way to specify explicit shapes is to call CreateCubicLoft()
        with ON::cubic_loft_ec_quadratic as the condition parameters, modify 
        the returned curves's end condition CVs as desired, and then call 
        CreateCubicLoft() with the explicit end condition option. This way 
        you will be sure to have a properly initialized nurbs_curve.

    nurbs_curve - [in]
      If not null, the result will returned in this ON_NurbsCurve.
      Typically, this parameter is used when you want to store the
      result in an ON_NurbsCurve that is on the stack.  This
      parameter is also used when you want to specify the interpolation
      knots or end conditions.

  Returns:
    If successful, a pointer to the surface is returned. If the input
    nurbs_surface parameter was null, then this surface is on the heap
    and will need to be deleted to avoid memory leaks.  If the input
    is not valid, null is returned, even when nurbs_surface is not
    null.

  Example:
    
    // EXAMPLE: Loft a curve through a list of points
      ON_SimpleArray< ON_3dPoint > point_list = ....;
      ON_NurbsCurve* srf = ON_Curve::CreateCubicLoft(
                     point_list.Count(), // number of points
                     3, // dimension
                     3, // stride
                     &point_list[0].x,
                     0.5 // sqrt(chord length) spacing
                     );
  */
  static 
  class ON_NurbsCurve* CreateCubicLoft(
    int point_count,
    int point_dim,
    int point_stride,
    const double* point_list,
    double k,
    int is_closed = 0,
    ON::cubic_loft_end_condition start_shape = ON::cubic_loft_ec_quadratic,
    ON::cubic_loft_end_condition end_shape   = ON::cubic_loft_ec_quadratic,
    class ON_NurbsCurve* nurbs_curve = 0
    );

#endif

  /*
	Description:
		Lookup a parameter in the m_t array, optionally using a built in snap tolerance to 
		snap a parameter value to an element of m_t.
		This function is used by some types derived from ON_Curve to snap parameter values
	Parameters:
		t			- [in]	parameter
		index -[out]	index into m_t such that
					  			if function returns false then
								   
									 @table  
									 value                  condition
						  			-1									 t<m_t[0] or m_t is empty				
										0<=i<=m_t.Count()-2		m_t[i] < t < m_t[i+1]			
										m_t.Count()-1					t>m_t[ m_t.Count()-1]			 

									if the function returns true then t is equal to, or is closest to and 
									within  tolerance of m_t[index]. 
									
		bEnableSnap-[in] enable snapping 
		m_t				-[in]	Array of parameter values to snap to
		RelTol		-[in] tolerance used in snapping
	
	Returns:		
		true if the t is exactly equal to (bEnableSnap==false), or within tolerance of
		(bEnableSnap==true) m_t[index]. 
  */
protected:
  bool ParameterSearch( double t, int& index, bool bEnableSnap, const ON_SimpleArray<double>& m_t, 
															double RelTol=ON_SQRT_EPSILON) const;

private:
#if defined(OPENNURBS_PLUS)
  // Runtime only - ignored by Read()/Write()
  volatile class ON_CurveTree* m_ctree;
#endif
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_Curve*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<const ON_Curve*>;
#endif

class ON_CLASS ON_CurveArray : public ON_SimpleArray<ON_Curve*>
{
public:
  ON_CurveArray( int = 0 );
  ~ON_CurveArray(); // deletes any non-nullptr curves

  bool Write( ON_BinaryArchive& ) const;
  bool Read( ON_BinaryArchive& );

  void Destroy(); // deletes curves, sets pointers to nullptr, sets count to zero

  bool Duplicate( ON_CurveArray& ) const; // operator= copies the pointer values
                                          // duplicate copies the curves themselves

  /*
	Description:
    Get tight bounding box of the bezier.
	Parameters:
		tight_bbox - [in/out] tight bounding box
		bGrowBox -[in]	(default=false)			
      If true and the input tight_bbox is valid, then returned
      tight_bbox is the union of the input tight_bbox and the 
      tight bounding box of the bezier curve.
		xform -[in] (default=nullptr)
      If not nullptr, the tight bounding box of the transformed
      bezier is calculated.  The bezier curve is not modified.
	Returns:
    True if the returned tight_bbox is set to a valid 
    bounding box.
  */
	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const;
};

/*
Description:
  Trim a curve.
Parameters:
  curve - [in] curve to trim (not modified)
  trim_parameters - [in] trimming parameters
    If curve is open, then  trim_parameters must be an increasing 
    interval.If curve is closed, and trim_parameters ins a 
    decreasing interval, then the portion of the curve across the
    start/end is returned.
Returns:
  trimmed curve or nullptr if input is invalid.
*/
ON_DECL
ON_Curve* ON_TrimCurve( 
            const ON_Curve& curve,
            ON_Interval trim_parameters
            );

/*
Description:
  Move ends of curves to a common point. Neither curve can be closed or an ON_CurveProxy.
  If one is an arc or polycurve with arc at end to change, and the other is not, 
  then the arc is left unchanged and the other curve is moved to the arc endpoint. 
  Otherwise, both are moved to the midpoint of the segment between the ends.
Parameters:
  Crv0 - [in] first curve to modify.
         [out] with one endpoint possibly changed.
  end0 - [in] if 0, change start of Crv0.  Otherwise change end.
  Crv1 - [in] second curve to modify.
         [out] with one endpoint possibly changed.
  end1 - [in] if 0, change start of Crv1.  Otherwise change end.
Returns:
  true if the endpoints match. False otherwise,
*/
ON_DECL
bool ON_ForceMatchCurveEnds(
                            ON_Curve& Crv0, 
                            int end0, 
                            ON_Curve& Crv1, 
                            int end1
                            );

/*
OBSOLETE. Use int ON_JoinCurves(const ON_SimpleArray<const ON_Curve*>& InCurves,
                                ON_SimpleArray<ON_Curve*>& OutCurves,
                                double join_tol,
                                double kink_tol,
                                bool bPreserveDirection = false,
                                ON_SimpleArray<int>* key = 0
                               );

Description:
  Join all contiguous curves of an array of ON_Curves.
Parameters:
  InCurves - [in] Array of curves to be joined (not modified)
  OutCurves - [out] Resulting joined curves and copies of curves that were not joined to anything
                    are appended.
  join_tol - [in] Distance tolerance used to decide if endpoints are close enough
  bPreserveDirection - [in] If true, curve endpoints will be compared to curve startpoints.
                            If false, all start and endpoints will be compared, and copies of input 
                            curves may be reversed in output.
  key     -  [out] if key is not null, InCurves[i] was joined into OutCurves[key[i]].
Returns:
  Number of curves added to Outcurves
Remarks:
  Closed curves are copied to OutCurves. 
  Curves that cannot be joined to others are copied to OutCurves.  When curves are joined, the results
  are ON_PolyCurves. All members of InCurves must have same dimension, at most 3.
  */
ON_DECL
int ON_JoinCurves(const ON_SimpleArray<const ON_Curve*>& InCurves,
                  ON_SimpleArray<ON_Curve*>& OutCurves,
                  double join_tol,
                  bool bPreserveDirection = false,
                  ON_SimpleArray<int>* key = 0
                 );

/*
Description:
  Join all contiguous curves of an array of ON_Curves.
Parameters:
  InCurves - [in] Array of curves to be joined (not modified)
  OutCurves - [out] Resulting joined curves and copies of curves that were not joined to anything
                    are appended.
  join_tol - [in] Distance tolerance used to decide if endpoints are close enough
  kink_tol - [in] Angle in radians.  If > 0.0, then curves within join_tol will only be joined if the angle between them
                  is less than kink_tol. If <= 0, then the angle will be ignored and only join_tol will be used.
  bUseTanAngle - [in] If true, choose the best match using angle between tangents.  
                      If false, best match is the closest. This is used whether or not kink_tol is positive.
  bPreserveDirection - [in] If true, curve endpoints will be compared to curve startpoints.
                            If false, all start and endpoints will be compared, and copies of input 
                            curves may be reversed in output.
  key     -  [out] if key is not null, InCurves[i] was joined into OutCurves[key[i]].
Returns:
  Number of curves added to Outcurves
Remarks:
  Closed curves are copied to OutCurves. 
  Curves that cannot be joined to others are copied to OutCurves.  When curves are joined, the results
  are ON_PolyCurves. All members of InCurves must have same dimension, at most 3.
  */
ON_DECL
int ON_JoinCurves(const ON_SimpleArray<const ON_Curve*>& InCurves,
                  ON_SimpleArray<ON_Curve*>& OutCurves,
                  double join_tol,
                  double kink_tol,
                  bool bUseTanAngle,
                  bool bPreserveDirection = false,
                  ON_SimpleArray<int>* key = 0
                 );


struct ON_CLASS CurveJoinSeg
{
  int id;
  bool bRev;
};

/*
Description:
  Sorts curve ends - this is the first step of joining curves.
Parameters:
  InCurves - [in] Array of curves to be sorted. These curves should be open (this is NOT checked) and not null.
  join_tol - [in] Distance tolerance used to decide if endpoints are close enough
  kink_tol - [in] Angle in radians.  If > 0.0, then curves within join_tol will only be joined if the angle between them
                  is less than kink_tol. If <= 0, then the angle will be ignored and only join_tol will be used.
  bUseTanAngle - [in] If true, choose the best match using angle between tangents.
                      If false, best match is the closest. This is used whether or not kink_tol is positive.
  bPreserveDirection - [in] If true, curve endpoints will be compared to curve startpoints.
                            If false, all start and endpoints will be compared, and copies of input
                            curves may be reversed in output.
  SegsArray - [out] The array of curve join segments. This array is sorted in the way that the curves are oriented 
                    head-to-tail. Which end is head and which is tail is determined by the bRev parameter of each segment
  Singles -   [out] An array of unjoinable curves. These do not overlap with any other curve within the given tolerance.
Returns:
  True on success, false when the input is not valid.
*/

ON_DECL
bool ON_SortCurveEnds(const ON_SimpleArray<const ON_Curve*>& InCurves,
  double join_tol, double kink_tol,
  bool bUseTanAngle,
  bool bPreserveDirection,
  ON_ClassArray<ON_SimpleArray<CurveJoinSeg> >& SegsArray,
  ON_SimpleArray<int>& Singles
);


/*
Description:
Join all contiguous curves of an array of ON_Curves using an older sort algorithm.
Unless this older version is necessary, use ON_JoinCurves instead.
Parameters:
  InCurves - [in] Array of curves to be joined (not modified)
  OutCurves - [out] Resulting joined curves and copies of curves that were not joined to anything
are appended.
  join_tol - [in] Distance tolerance used to decide if endpoints are close enough
  bPreserveDirection - [in] If true, curve endpoints will be compared to curve startpoints.
    If false, all start and endpoints will be compared, and copies of input 
    curves may be reversed in output.
  key     -  [out] if key is not null, InCurves[i] was joined into OutCurves[key[i]].
Returns:
  Number of curves added to Outcurves
Remarks:
  Closed curves are copied to OutCurves. 
  Curves that cannot be joined to others are copied to OutCurves.  When curves are joined, the results
  are ON_PolyCurves. All members of InCurves must have same dimension, at most 3.
*/
ON_DECL
int ON_JoinCurvesOld(const ON_SimpleArray<const ON_Curve*>& InCurves,
  ON_SimpleArray<ON_Curve*>& OutCurves,
  double join_tol,
  bool bPreserveDirection, // = false
  ON_SimpleArray<int>* key //=0
);



/*
Description:
  Sort a list of lines so they are geometrically continuous.
Parameters:
  line_count - [in] number of lines
  line_list  - [in] array of lines
  index       - [out] The input index[] is an array of line_count unused integers.
                      The returned index[] is a permutation of {0,1,...,line_count-1}
                      so that the list of lines is in end-to-end order.
  bReverse    - [out] The input bReverse[] is an array of line_count unused bools.
                      If the returned value of bReverse[j] is true, then
                      line_list[index[j]] needs to be reversed.
Returns:
  True if successful, false if not.
*/
ON_DECL
bool ON_SortLines( 
        int line_count, 
        const ON_Line* line_list, 
        int* index, 
        bool* bReverse 
        );

/*
Description:
  Sort a list of lines so they are geometrically continuous.
Parameters:
  line_list  - [in] array of lines
  index       - [out] The input index[] is an array of line_count unused integers.
                      The returned index[] is a permutation of {0,1,...,line_count-1}
                      so that the list of lines is in end-to-end order.
  bReverse    - [out] The input bReverse[] is an array of line_count unused bools.
                      If the returned value of bReverse[j] is true, then
                      line_list[index[j]] needs to be reversed.
Returns:
  True if successful, false if not.
*/
ON_DECL
bool ON_SortLines( 
        const ON_SimpleArray<ON_Line>& line_list,
        int* index, 
        bool* bReverse 
        );

/*
Description:
  Sort a list of open curves so end of a curve matches the start of the next curve.
Parameters:
  curve_count - [in] number of curves
  curve_list  - [in] array of curve pointers
  index       - [out] The input index[] is an array of curve_count unused integers.
                      The returned index[] is a permutation of {0,1,...,curve_count-1}
                      so that the list of curves is in end-to-end order.
  bReverse    - [out] The input bReverse[] is an array of curve_count unused bools.
                      If the returned value of bReverse[j] is true, then
                      curve_list[index[j]] needs to be reversed.
Returns:
  True if successful, false if not.
*/
ON_DECL
bool ON_SortCurves(
          int curve_count,
          const ON_Curve* const* curve_list, 
          int* index,
          bool* bReverse
          );

/*
Description:
  Sort a list of curves so end of a curve matches the start of the next curve.
Parameters:
  curve       - [in] array of curves to sort.  The curves themselves are not modified.
  index       - [out] The input index[] is an array of curve_count unused integers.
                      The returned index[] is a permutation of {0,1,...,curve_count-1}
                      so that the list of curves is in end-to-end order.
  bReverse    - [out] The input bReverse[] is an array of curve_count unused bools.
                      If the returned value of bReverse[j] is true, then
                      curve[index[j]] needs to be reversed.
Returns:
  True if successful, false if not.
*/
ON_DECL
bool ON_SortCurves( 
                   const ON_SimpleArray<const ON_Curve*>& curves, 
                   ON_SimpleArray<int>& index, 
                   ON_SimpleArray<bool>& bReverse 
                   );

/*
Description:
  Sort a list of curves so end of a curve matches the start of the next curve.
Parameters:
  curve_count - [in] number of curves
  curve       - [in] array of curve pointers
  index       - [out] The input index[] is an array of curve_count unused integers.
                      The returned index[] is a permutation of {0,1,...,curve_count-1}
                      so that the list of curves is in end-to-end order.
  bReverse    - [out] The input bReverse[] is an array of curve_count unused bools.
                      If the returned value of bReverse[j] is true, then
                      curve[index[j]] needs to be reversed.
Returns:
  True if successful, false if not.
*/
ON_DECL
bool ON_SortCurves( 
          const ON_SimpleArray<ON_Curve*>& curves, 
          ON_SimpleArray<int>& index, 
          ON_SimpleArray<bool>& bReverse 
          );

/*
Description:
  Determine the orientation (counterclockwise or clockwise) of a closed
  planar curve.
Parameters:
  curve - [in] simple (no self intersections) closed planar curve
  xform - [in] Transformation to map the curve to the xy plane. If the
               curve is parallel to the xy plane, you may pass nullptr.
	plane - [in] If curve is on plane then determine the orientation in relation to 
								plane's orientation.
Returns:
  +1: The curve's orientation is counter clockwise in the xy plane.
  -1: The curve's orientation is clockwise in the xy plane.
   0: Unable to compute the curve's orientation.
*/
ON_DECL
int ON_ClosedCurveOrientation(const ON_Curve& curve, const ON_Xform* xform);
ON_DECL
int ON_ClosedCurveOrientation(const ON_Curve& curve, const ON_Plane& plane);


/*
Description:
  Get a crude approximation of the signed area of the region in the 
  x-y plane traced out by the curve.  This is useful for calculating
  the orientation of projections of loops to planes when you have
  more than one curve.
Parameters:
  curve - [in] 
  domain - [in]
    optional sub-domain. (null if entire curve should be used).
  xform - [in] Transformation to map the curve to the xy plane. If the
               curve is parallel to the xy plane, you may pass nullptr.
  bReverseCurve - [in]
Returns:
  1/2 the sum of (p[i].x-p[i+1].x)*(p[i].y+p[i+1].y), where p[i] 
  is a series of sampled points on the curve.
*/
ON_DECL
double ON_CurveOrientationArea( 
  const ON_Curve* curve,
  const ON_Interval* domain,
  const ON_Xform* xform,
  bool bReverseCurve
  );

#if defined(OPENNURBS_PLUS)
/*
Description:
Looks for segments that are shorter than tolerance
that can be combined. For NURBS of degree greater than 1,
spans are combined by removing knots. Similarly for NURBS segments of polycurves.
Otherwise, crv.RemoveShortSegments() is called.
Does not change the 
domain, but it will change the relative parameterization.
Parameters:
tolerance - [in]
Returns:
True if short segments were combined or removed.
False otherwise.
*/

ON_DECL
bool ON_CombineShortSegments(ON_Curve& crv, double tolerance);
#endif

#endif
