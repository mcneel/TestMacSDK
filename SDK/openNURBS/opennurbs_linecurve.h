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

#if !defined(ON_GEOMETRY_CURVE_LINE_INC_)
#define ON_GEOMETRY_CURVE_LINE_INC_

class ON_LineCurve;
class ON_CLASS ON_LineCurve : public ON_Curve
{
  ON_OBJECT_DECLARE(ON_LineCurve);

public:
  ON_LineCurve() ON_NOEXCEPT;
  virtual ~ON_LineCurve();
  ON_LineCurve(const ON_LineCurve&);
	ON_LineCurve& operator=(const ON_LineCurve&);

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_LineCurve( ON_LineCurve&& ) ON_NOEXCEPT;

  // The rvalue assignment operator calls ON_Object::operator=(ON_Object&&)
  // which could throw exceptions.  See the implementation of
  // ON_Object::operator=(ON_Object&&) for details.
  ON_LineCurve& operator=( ON_LineCurve&& );
#endif

  ON_LineCurve(const ON_2dPoint&,const ON_2dPoint&); // creates a 2d line curve
  ON_LineCurve(const ON_3dPoint&,const ON_3dPoint&); // creates a 3d line curve
  ON_LineCurve(const ON_Line&);
  ON_LineCurve(const ON_Line&,
                double,double    // domain
                );



	ON_LineCurve& operator=(const ON_Line&);
  
  /////////////////////////////////////////////////////////////////
  // ON_Object overrides

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

  /*
      Returns true if !ON_ArePointsCoincident(Start,End) and
      m_t.IsIncreasing().
  */
  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  void Dump( ON_TextLog& ) const override; // for debugging

  bool Write(
         ON_BinaryArchive&  // open binary file
       ) const override;

  bool Read(
         ON_BinaryArchive&  // open binary file
       ) override;

  /////////////////////////////////////////////////////////////////
  // ON_Geometry overrides

  int Dimension() const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  // virtual ON_Geometry GetTightBoundingBox override		
  bool GetTightBoundingBox( class ON_BoundingBox& tight_bbox, bool bGrowBox = false, const class ON_Xform* xform = nullptr ) const override;

  bool Transform( 
         const ON_Xform&
         ) override;

  // virtual ON_Geometry::IsDeformable() override
  bool IsDeformable() const override;

  // virtual ON_Geometry::MakeDeformable() override
  bool MakeDeformable() override;

  bool SwapCoordinates(
        int, int        // indices of coords to swap
        ) override;

#if defined(OPENNURBS_PLUS)
  // virtual ON_Geometry override
  bool Morph( const ON_SpaceMorph& morph ) override;

  // virtual ON_Geometry override
  bool IsMorphable() const override;
#endif

  /////////////////////////////////////////////////////////////////
  // ON_Curve overrides

  ON_Interval Domain() const override;

  // Description:
  //   Set the domain of the curve
  // Parameters:
  //   t0 - [in]
  //   t1 - [in] new domain will be [t0,t1]
  // Returns:
  //   true if successful.
  bool SetDomain( 
        double t0, 
        double t1 
        ) override;

  bool ChangeDimension(
          int desired_dimension
          ) override;

  int SpanCount() const override; // number of smooth spans in curve

  bool GetSpanVector( // span "knots" 
         double* // array of length SpanCount() + 1 
         ) const override; // 

  int Degree( // returns maximum algebraic degree of any span 
                  // ( or a good estimate if curve spans are not algebraic )
    ) const override; 

  bool IsLinear( // true if curve locus is a line segment between
                 // between specified points
        double = ON_ZERO_TOLERANCE // tolerance to use when checking linearity
        ) const override;

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
  //virtual
  int IsPolyline(
        ON_SimpleArray<ON_3dPoint>* pline_points = nullptr,
        ON_SimpleArray<double>* pline_t = nullptr
        ) const override;

  bool IsArc( // ON_Arc.m_angle > 0 if curve locus is an arc between
              // specified points
        const ON_Plane* = nullptr, // if not nullptr, test is performed in this plane
        ON_Arc* = nullptr, // if not nullptr and true is returned, then arc parameters
                         // are filled in
        double = ON_ZERO_TOLERANCE    // tolerance to use when checking
        ) const override;

  bool IsPlanar(
        ON_Plane* = nullptr, // if not nullptr and true is returned, then plane parameters
                           // are filled in
        double = ON_ZERO_TOLERANCE    // tolerance to use when checking
        ) const override;

  bool IsInPlane(
        const ON_Plane&, // plane to test
        double = ON_ZERO_TOLERANCE    // tolerance to use when checking
        ) const override;

  bool IsClosed(  // true if curve is closed (either curve has
        void      // clamped end knots and euclidean location of start
        ) const override;  // CV = euclidean location of end CV, or curve is
                  // periodic.)

  bool IsPeriodic(  // true if curve is a single periodic segment
        void 
        ) const override;
  
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
  See Also:
    ON_Curve::SetEndPoint
    ON_Curve::PointAtStart
    ON_Curve::PointAtEnd
  */
  bool SetStartPoint(
          ON_3dPoint start_point
          ) override;

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
  See Also:
    ON_Curve::SetStartPoint
    ON_Curve::PointAtStart
    ON_Curve::PointAtEnd
  */
  bool SetEndPoint(
          ON_3dPoint end_point
          ) override;

  bool Reverse() override;       // reverse parameterization
                        // Domain changes from [a,b] to [-b,-a]

  bool Evaluate( // returns false if unable to evaluate
         double,         // evaluation parameter
         int,            // number of derivatives (>=0)
         int,            // array stride (>=Dimension())
         double*,        // array of length stride*(ndir+1)
         int = 0,        // optional - determines which side to evaluate from
                         //         0 = default
                         //      <  0 to evaluate from below, 
                         //      >  0 to evaluate from above
         int* = 0        // optional - evaluation hint (int) used to speed
                         //            repeated evaluations
         ) const override;

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
  bool GetClosestPoint( const ON_3dPoint&, // test_point
          double*,       // parameter of local closest point returned here
          double = 0.0,  // maximum_distance
          const ON_Interval* = nullptr // sub_domain
          ) const override;

  //////////
  // Find parameter of the point on a curve that is locally closest to 
  // the test_point.  The search for a local close point starts at 
  // seed_parameter. If the sub_domain parameter is not nullptr, then
  // the search is restricted to the specified portion of the curve.
  //
  // true if returned if the search is successful.  false is returned if
  // the search fails.
  bool GetLocalClosestPoint( const ON_3dPoint&, // test_point
          double,    // seed_parameter
          double*,   // parameter of local closest point returned here
          const ON_Interval* = nullptr // sub_domain
          ) const override;

  // virtual ON_Curve override
  int IntersectSelf( 
          ON_SimpleArray<ON_X_EVENT>& x,
          double intersection_tolerance = 0.0,
          const ON_Interval* curve_domain = 0
          ) const override;

  // Description:
  //   virtual ON_Curve::GetLength override.
  //   Get the length of the line.
  // Parameters:
  //   length - [out] length returned here.
  //   t - [out] parameter such that the length of the curve
  //      from its start to t is arc_length.
  //   fractional_tolerance - [in] desired fractional precision.
  //       fabs(("exact" length from start to t) - arc_length)/arc_length <= fractional_tolerance
  //   sub_domain - [in] If not nullptr, the calculation is performed on
  //       the specified sub-domain of the curve.
  // Returns:
  //   true if returned if the length calculation is successful.
  bool GetLength(
          double* length,
          double fractional_tolerance = 1.0e-8,
          const ON_Interval* sub_domain = nullptr
          ) const override;

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
  bool IsShort(
    double tolerance,
    const ON_Interval* sub_domain = 0,
    double* length_estimate = 0
    ) const override;

  // Description:
  //   virtual ON_Curve::GetNormalizedArcLengthPoint override.
  //   Get the parameter of the point on the line that is a 
  //   prescribed distance from the start of the line
  // Parameters:
  //   s - [in] normalized arc length parameter.  E.g., 0 = start
  //        of curve, 1/2 = midpoint of curve, 1 = end of curve.
  //   t - [out] parameter such that the length of the line
  //      from its start to t is arc_length.
  //   fractional_tolerance - [in] desired fractional precision.
  //       fabs(("exact" length from start to t) - arc_length)/arc_length <= fractional_tolerance
  //   sub_domain - [in] If not nullptr, the calculation is performed on
  //       the specified sub-domain of the curve.
  // Returns:
  //   true if successful
  bool GetNormalizedArcLengthPoint(
          double s,
          double* t,
          double fractional_tolerance = 1.0e-8,
          const ON_Interval* sub_domain = nullptr
          ) const override;

  /*
  Description:
    virtual ON_Curve::GetNormalizedArcLengthPoints override.
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
  bool GetNormalizedArcLengthPoints(
          int count,
          const double* s,
          double* t,
          double absolute_tolerance = 0.0,
          double fractional_tolerance = 1.0e-8,
          const ON_Interval* sub_domain = nullptr
          ) const override;
#endif

  // Description:
  //   virtual ON_Curve::Trim override.
  //   Removes portions of the curve outside the specified interval.
  // Parameters:
  //   domain - [in] interval of the curve to keep.  Portions of the
  //      curve before curve(domain[0]) and after curve(domain[1]) are
  //      removed.
  // Returns:
  //   true if successful.
  bool Trim(
    const ON_Interval& domain
    ) override;

  // Description:
  //   Where possible, analytically extends curve to include domain.
  // Parameters:
  //   domain - [in] if domain is not included in curve domain, 
  //   curve will be extended so that its domain includes domain.  
  //   Original curve is identical
  //   to the restriction of the resulting curve to the original curve domain, 
  // Returns:
  //   true if successful.
  bool Extend(
    const ON_Interval& domain
    ) override;

  // Description:
  //   virtual ON_Curve::Split override.
  //   Divide the curve at the specified parameter.  The parameter
  //   must be in the interior of the curve's domain.  The pointers
  //   passed to Split must either be nullptr or point to an ON_Curve
  //   object of the same of the same type.  If the pointer is nullptr,
  //   then a curve will be created in Split().  You may pass "this"
  //   as one of the pointers to Split().
  // Parameters:
  //   t - [in] parameter in interval Domain().
  //   left_side - [out] left portion of curve
  //   right_side - [out] right portion of curve
  // Example:
  //   For example, if crv were an ON_NurbsCurve, then
  //
  //     ON_NurbsCurve right_side;
  //     crv.Split( crv.Domain().Mid() &crv, &right_side );
  //
  //   would split crv at the parametric midpoint, put the left side
  //   in crv, and return the right side in right_side.
  bool Split(
      double t,    // t = curve parameter to split curve at
      ON_Curve*& left_side, // left portion returned here
      ON_Curve*& right_side // right portion returned here
    ) const override;

  // Description:
  //   virtual ON_Curve::GetNurbForm override.
  //   Get a NURBS curve representation of this curve.
  // Parameters:
  //   nurbs_curve - [out] NURBS representation returned here
  //   tolerance - [in] tolerance to use when creating NURBS
  //       representation.
  //   subdomain - [in] if not nullptr, then the NURBS representation
  //       for this portion of the curve is returned.
  // Returns:
  //   0   unable to create NURBS representation
  //       with desired accuracy.
  //   1   success - returned NURBS parameterization
  //       matches the curve's to the desired accuracy
  //   2   success - returned NURBS point locus matches
  //       the curve's to the desired accuracy but, on
  //       the interior of the curve's domain, the 
  //       curve's parameterization and the NURBS
  //       parameterization may not match to the 
  //       desired accuracy.
  int GetNurbForm(
        ON_NurbsCurve&,
        double = 0.0,
        const ON_Interval* = nullptr
        ) const override;

  // Description:
  //   virtual ON_Curve::HasNurbForm override.
  //   Does a NURBS curve representation of this curve exist.
  // Parameters:
  // Returns:
  //   0   unable to create NURBS representation
  //       with desired accuracy.
  //   1   success - returned NURBS parameterization
  //       matches the curve's to the desired accuracy
  //   2   success - returned NURBS point locus matches
  //       the curve's to the desired accuracy but, on
  //       the interior of the curve's domain, the 
  //       curve's parameterization and the NURBS
  //       parameterization may not match to the 
  //       desired accuracy.
  int HasNurbForm(
        ) const override;

  // Description:
  //   virtual ON_Curve::GetCurveParameterFromNurbFormParameter override.
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
  //virtual
  bool GetCurveParameterFromNurbFormParameter(
        double nurbs_t,
        double* curve_t
        ) const override;

  // Description:
  //   virtual ON_Curve::GetNurbFormParameterFromCurveParameter override.
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
  //virtual
  bool GetNurbFormParameterFromCurveParameter(
        double curve_t,
        double* nurbs_t
        ) const override;

  /////////////////////////////////////////////////////////////////
  // Interface

  ON_Line m_line;
  ON_Interval m_t;  // domain
  int      m_dim;   // 2 or 3 (2 so ON_LineCurve can be uses as a trimming curve)
};


#endif
