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
//   Definition of surface proxy object
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_SURFACEPROXY_INC_)
#define OPENNURBS_SURFACEPROXY_INC_

class ON_CLASS ON_SurfaceProxy : public ON_Surface
{
public:
  // virtual ON_Object::DestroyRuntimeCache override
  void DestroyRuntimeCache( bool bDelete = true ) override;

public:
  ON_SurfaceProxy();
  ON_SurfaceProxy(const ON_Surface*);
  ON_SurfaceProxy(const ON_SurfaceProxy&);
  ON_SurfaceProxy& operator=(const ON_SurfaceProxy&);

  virtual ~ON_SurfaceProxy();

  void SetProxySurface( const ON_Surface* proxy_surface );
  const ON_Surface* ProxySurface() const;
  bool ProxySurfaceIsTransposed() const;

private:
  const ON_Surface* m_surface;
  bool m_bTransposed; // proxy surface parameterization is transpose of m_surface

public:
  /*
  Description:
    Get a duplicate of the surface.
  Returns:
    A duplicate of the surface.  
  Remarks:
    The caller must delete the returned surface.
    For non-ON_SurfaceProxy objects, this simply duplicates the surface using
    ON_Object::Duplicate.
    For ON_SurfaceProxy objects, this duplicates the actual proxy surface 
    geometry and, if necessary, transposes the result to that
    the returned surfaces's parameterization and locus match the proxy surface's.
  */
  ON_Surface* DuplicateSurface() const override;

  /////////////////////////////////////////////////////////////////
  // ON_Object overrides

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

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

  bool Transform( 
         const ON_Xform&
         ) override;

  /////////////////////////////////////////////////////////////////
  // ON_Surface overrides
#if defined(OPENNURBS_PLUS)
  ON_Mesh* CreateMesh( 
             const ON_MeshParameters& mp,
             ON_Mesh* mesh = nullptr
             ) const override;
#endif

  ON_Interval Domain(
    int // 0 gets first parameter's domain, 1 gets second parameter's domain
    ) const override;


  /*
  Description:
    Get an estimate of the size of the rectangle that would
    be created if the 3d surface where flattened into a rectangle.
  Parameters:
    width - [out]  (corresponds to the first surface parameter)
    height - [out] (corresponds to the first surface parameter)
  Remarks:
    overrides virtual ON_Surface::GetSurfaceSize
  Returns:
    true if successful.
  */
  bool GetSurfaceSize( 
      double* width, 
      double* height 
      ) const override;

  int SpanCount(
    int // 0 gets first parameter's domain, 1 gets second parameter's domain
    ) const override; // number of smooth spans in curve

  bool GetSpanVector( // span "knots" 
    int, // 0 gets first parameter's domain, 1 gets second parameter's domain
    double* // array of length SpanCount() + 1 
    ) const override; // 

  int Degree( // returns maximum algebraic degree of any span 
                  // ( or a good estimate if curve spans are not algebraic )
    int // 0 gets first parameter's domain, 1 gets second parameter's domain
    ) const override; 


  bool GetParameterTolerance( // returns tminus < tplus: parameters tminus <= s <= tplus
         int,     // 0 gets first parameter, 1 gets second parameter
         double,  // t = parameter in domain
         double*, // tminus
         double*  // tplus
         ) const override;

  // override virtual ON_Surface::IsIsoparametric
  ON_Surface::ISO IsIsoparametric(
        const ON_Curve& curve,
        const ON_Interval* curve_domain = nullptr
        ) const override;

  // override virtual ON_Surface::IsIsoparametric
  ON_Surface::ISO IsIsoparametric(
        const ON_BoundingBox& bbox
        ) const override;

  /*
  Description:
    Test a surface to see if it is planar.
  Parameters:
    plane - [out] if not nullptr and true is returned,
                  the plane parameters are filled in.
    tolerance - [in] tolerance to use when checking
  Returns:
    true if there is a plane such that the maximum distance from
    the surface to the plane is <= tolerance.
  Remarks:
    Overrides virtual ON_Surface::IsPlanar.
  */
  bool IsPlanar(
        ON_Plane* plane = nullptr,
        double tolerance = ON_ZERO_TOLERANCE
        ) const override;

  bool IsClosed(   // true if surface is closed in direction
        int        // dir  0 = "s", 1 = "t"
        ) const override;

  bool IsPeriodic( // true if surface is periodic in direction
        int        // dir  0 = "s", 1 = "t"
        ) const override;

  bool IsSingular( // true if surface side is collapsed to a point
        int        // side of parameter space to test
                   // 0 = south, 1 = east, 2 = north, 3 = west
        ) const override;
  
  /*
  Description:
    Search for a derivative, tangent, or curvature
    discontinuity.
  Parameters:
    dir - [in] If 0, then "u" parameter is checked.  If 1, then
               the "v" parameter is checked.
    c - [in] type of continuity to test for.
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
  bool GetNextDiscontinuity( 
                  int dir,
                  ON::continuity c,
                  double t0,
                  double t1,
                  double* t,
                  int* hint=nullptr,
                  int* dtype=nullptr,
                  double cos_angle_tolerance=ON_DEFAULT_ANGLE_TOLERANCE_COSINE,
                  double curvature_tolerance=ON_SQRT_EPSILON
                  ) const override;

  /*
  Description:
    Test continuity at a surface parameter value.
  Parameters:
    c - [in] continuity to test for
    s - [in] surface parameter to test
    t - [in] surface parameter to test
    hint - [in] evaluation hint
    point_tolerance - [in] if the distance between two points is
        greater than point_tolerance, then the surface is not C0.
    d1_tolerance - [in] if the difference between two first derivatives is
        greater than d1_tolerance, then the surface is not C1.
    d2_tolerance - [in] if the difference between two second derivatives is
        greater than d2_tolerance, then the surface is not C2.
    cos_angle_tolerance - [in] default = cos(1 degree) Used only when
        c is ON::continuity::G1_continuous or ON::continuity::G2_continuous.  If the cosine
        of the angle between two normal vectors 
        is <= cos_angle_tolerance, then a G1 discontinuity is reported.
    curvature_tolerance - [in] (default = ON_SQRT_EPSILON) Used only when
        c is ON::continuity::G2_continuous.  If K0 and K1 are curvatures evaluated
        from above and below and |K0 - K1| > curvature_tolerance,
        then a curvature discontinuity is reported.
  Returns:
    true if the surface has at least the c type continuity at the parameter t.
  Remarks:
    Overrides virtual ON_Surface::IsContinuous
  */
  bool IsContinuous(
    ON::continuity c,
    double s, 
    double t, 
    int* hint = nullptr,
    double point_tolerance=ON_ZERO_TOLERANCE,
    double d1_tolerance=ON_ZERO_TOLERANCE,
    double d2_tolerance=ON_ZERO_TOLERANCE,
    double cos_angle_tolerance=ON_DEFAULT_ANGLE_TOLERANCE_COSINE,
    double curvature_tolerance=ON_SQRT_EPSILON
    ) const override;

  bool Reverse(  // reverse parameterization, Domain changes from [a,b] to [-b,-a]
    int // dir  0 = "s", 1 = "t"
    ) override;

  bool Transpose() override; // transpose surface parameterization (swap "s" and "t")

  // work horse evaluator
  bool Evaluate( // returns false if unable to evaluate
         double, double, // evaluation parameters
         int,            // number of derivatives (>=0)
         int,            // array stride (>=Dimension())
         double*,        // array of length stride*(ndir+1)*(ndir+2)/2
         int = 0,        // optional - determines which quadrant to evaluate from
                         //         0 = default
                         //         1 from NE quadrant
                         //         2 from NW quadrant
                         //         3 from SW quadrant
                         //         4 from SE quadrant
         int* = 0        // optional - evaluation hint (int[2]) used to speed
                         //            repeated evaluations
         ) const override;


  ON_Curve* IsoCurve(
         int dir,
         double c
         ) const override;

#if defined(OPENNURBS_PLUS)
  ON_Curve* Pushup( const ON_Curve& curve_2d,
                    double tolerance,
                    const ON_Interval* curve_2d_subdomain = nullptr,
                    ON_FitResult* rc=0
                    ) const override;

  ON_Curve* Pullback( const ON_Curve& curve_3d,
                    double tolerance,
                    const ON_Interval* curve_3d_subdomain = nullptr,
                    ON_3dPoint start_uv = ON_3dPoint::UnsetPoint,
                    ON_3dPoint end_uv = ON_3dPoint::UnsetPoint,
                    ON_FitResult* rc=0
                    ) const override;


  /*
  Description:
    Get the parameters of the point on the surface that is closest to P.
  Parameters:
    P - [in] 
            test point
    s - [out]
    t - [out] 
            (*s,*t) = parameters of the surface point that 
            is closest to P.
    maximum_distance = 0.0 - [in] 
            optional upper bound on the distance from P to 
            the surface.  If you are only interested in 
            finding a point Q on the surface when 
            P.DistanceTo(Q) < maximum_distance, then set
            maximum_distance to that value.
    sdomain = 0 - [in] optional domain restriction
    tdomain = 0 - [in] optional domain restriction
  Returns:
    True if successful.  If false, the values of *s and *t
    are undefined.
  See Also:
    ON_Surface::GetLocalClosestPoint.
  */
  bool GetClosestPoint( 
          const ON_3dPoint& P,
          double* s,
          double* t,
          double maximum_distance = 0.0,
          const ON_Interval* sdomain = 0,
          const ON_Interval* tdomain = 0
          ) const override;

  //////////
  // Find parameters of the point on a surface that is locally closest to 
  // the test_point.  The search for a local close point starts at 
  // seed parameters. If a sub_domain parameter is not nullptr, then
  // the search is restricted to the specified portion of the surface.
  //
  // true if returned if the search is successful.  false is returned if
  // the search fails.
  bool GetLocalClosestPoint( const ON_3dPoint&, // test_point
          double,double,     // seed_parameters
          double*,double*,   // parameters of local closest point returned here
          const ON_Interval* = nullptr, // first parameter sub_domain
          const ON_Interval* = nullptr  // second parameter sub_domain
          ) const override;

  /*
  Description:
    Offset surface.
  Parameters:
    offset_distance - [in] offset distance
    tolerance - [in] Some surfaces do not have an exact offset that
      can be represented using the same class of surface definition.
      In that case, the tolerance specifies the desired accuracy.
    max_deviation - [out] If this parameter is not nullptr, the maximum
      deviation from the returned offset to the true offset is returned
      here.  This deviation is zero except for cases where an exact
      offset cannot be computed using the same class of surface definition.
  Remarks:
    Overrides virtual ON_Surface::Offset.
  Returns:
    Offset surface.
  */
  ON_Surface* Offset(
        double offset_distance, 
        double tolerance, 
        double* max_deviation = nullptr
        ) const override;
#endif

  int GetNurbForm( // returns 0: unable to create NURBS representation
                   //            with desired accuracy.
                   //         1: success - returned NURBS parameterization
                   //            matches the surface's to the desired accuracy
                   //         2: success - returned NURBS point locus matches
                   //            the surfaces's to the desired accuracy but, on
                   //            the interior of the surface's domain, the 
                   //            surface's parameterization and the NURBS
                   //            parameterization may not match to the 
                   //            desired accuracy.
        ON_NurbsSurface&,
        double = 0.0
        ) const override;

  int HasNurbForm( // returns 0: unable to create NURBS representation
                   //            with desired accuracy.
                   //         1: success - returned NURBS parameterization
                   //            matches the surface's to the desired accuracy
                   //         2: success - returned NURBS point locus matches
                   //            the surfaces's to the desired accuracy but, on
                   //            the interior of the surface's domain, the 
                   //            surface's parameterization and the NURBS
                   //            parameterization may not match to the 
                   //            desired accuracy.
        ) const override;

  bool GetSurfaceParameterFromNurbFormParameter(
        double nurbs_s, double nurbs_t,
        double* surface_s, double* surface_t
        ) const override;

  bool GetNurbFormParameterFromSurfaceParameter(
        double surface_s, double surface_t,
        double* nurbs_s,  double* nurbs_t
        ) const override;

private:

  ON_OBJECT_DECLARE(ON_SurfaceProxy);
};

#endif
