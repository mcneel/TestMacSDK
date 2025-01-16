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

#if !defined(OPENNURBS_KNOT_INC_)
#define OPENNURBS_KNOT_INC_

ON_DECL
double ON_DomainTolerance(
          double, // start of domain
          double  // end of domain
          );

ON_DECL
double ON_KnotTolerance(
          int,           // order (>=2)
          int,           // cv count
          const double*, // knot[] array
          int            // knot index
          );

ON_DECL
double ON_SpanTolerance(
          int,           // order (>=2)
          int,           // cv count
          const double*, // knot[] array
          int            // span index
          );

/// <summary>
/// The number of knots in a NURBS knot vector is (cv_count + order - 2).
/// </summary>
/// <param name="order">
/// order &gt;= 2.
/// Note that order = degree + 1.
/// </param>
/// <param name="cv_count">
/// cv_count &gt;=order.
/// Number of control points.
/// </param>
/// <returns>
/// If the input is valid, then the number of knots is the
/// knot vector is returned.
/// Otherwise, 0 is returned.
/// </returns>
ON_DECL
int ON_KnotCount( // returns (order + cv_count - 2)
          int order,
          int cv_count
          );

ON_DECL
int ON_KnotMultiplicity(
          int,   // order (>=2)
          int,   // cv_count (>=order)
          const double*, // knot[]
          int            // knot_index
          );

ON_DECL
int ON_KnotVectorSpanCount(
          int,           // order (>=2)
          int,           // cv count
          const double*  // knot[] array
          );

ON_DECL
bool ON_GetKnotVectorSpanVector(
          int,           // order (>=2)
          int,           // cv count
          const double*, // knot[] array
          double*        // s[] array
          );

/*
Description:
  Given an evaluation parameter t in the domain of a NURBS curve,
  ON_NurbsSpanIndex(order,cv_count,knot,t,0,0) returns the integer
  i such that (knot[i],...,knot[i+2*degree-1]),  and 
  (cv[i],...,cv[i+degree]) are the knots and control points that
  define the span of the NURBS that are used for evaluation at t.
Parameters:
  order - [in] order >= 2
  cv_count - [in] cv_count >= order
  knot - [in] valid knot vector
  t - [in] evaluation parameter
  side - [in] determines which span is used when t is at a knot
              value;  side = 0 for the default (from above),
              side = -1 means from below, and 
              side = +1 means from above.
  hint - [in] Search hint, or 0 if not hint is available.
Returns:
  Returns the index described above.
*/
ON_DECL
int ON_NurbsSpanIndex(
          int order,
          int cv_count,
          const double* knot,
          double t,
          int side,
          int hint
          );

ON_DECL
int ON_NextNurbsSpanIndex(
          // returns  0: input span_index < 0
          //           cv_count-order: input span_index = cv_count-order
          //         -1: input span_index > cv_count-order;
          //     otherwise next span index
          int order,
          int cv_count,
          const double* knot,
          int            // current span_index 
          );

ON_DECL
int ON_GetSpanIndices( // returns span count, which is one less than length of span_indices[]
          int order,
          int cv_count,
          const double* knot,
          int*           // span_indices[cv_count-order+2]. 
                         //Indices of knots at end of group of mult knots 
                         //at start of span, and knot at start of group of mult knots
                         //at end of spline.
          );

ON_DECL
double ON_SuperfluousKnot( 
          int order,
          int cv_count,
          const double* knot,
          int            // 0 = first superfluous knot 
                         // 1 = last superfluous knot
          );

ON_DECL
bool ON_IsKnotVectorPeriodic(
          int order,
          int cv_count,
          const double* knot
          );

ON_DECL
bool ON_IsKnotVectorClamped(
          int order,
          int cv_count,
          const double* knot,
          int = 2        // 0 = check left end, 1 = check right end, 2 = check both
          );

ON_DECL
bool ON_IsKnotVectorUniform(
          int order,
          int cv_count,
          const double* knot 
          );

//////////
// returns true if all knots have multiplicity = degree
ON_DECL
bool ON_KnotVectorHasBezierSpans(
          int order,
          int cv_count,
          const double* knot
          );


ON_DECL
ON::knot_style ON_KnotVectorStyle( 
          int order,
          int cv_count,
          const double* knot
          );

/*
Description:
  Set the domain of a knot vector.
Parameters:
  order - [in] order >= 2
  cv_count - [in] cv_count >= order
  knot - [in/out] input existing knots and returns knots with new domain.
  t0 - [in]
  t1 - [in] New domain will be the interval (t0,t1).
Returns:
  True if input is valid and the returned knot vector 
  has the requested domain.  False if the input is
  invalid, in which case the input knot vector is not
  changed.
*/
ON_DECL
bool ON_SetKnotVectorDomain( 
          int order, 
          int cv_count, 
          double* knot, 
          double t0, 
          double t1 
          );

ON_DECL
bool ON_GetKnotVectorDomain(
          int,           // order (>=2)
          int,           // cv count
          const double*, // knot[] array
          double*, double*
          );

ON_DECL
bool ON_ReverseKnotVector(
          int,           // order (>=2)
          int,           // cv count
          double*        // knot[] array
          );

ON_DECL
int ON_CompareKnotVector( // returns 
                                      // -1: first < second
                                      //  0: first == second
                                      // +1: first > second
          // first knot vector
          int,           // order (>=2)
          int,           // cv count
          const double*, // knot[] array
          // second knot vector
          int,           // order (>=2)
          int,           // cv count
          const double*  // knot[] array
          );

ON_DECL
bool ON_IsValidKnotVector(
          int order,
          int cv_count, 
          const double* knot, 
          ON_TextLog* text_log = 0
          );

ON_DECL
bool ON_ClampKnotVector(
          // Sets initial/final order-2 knots to values in
          // knot[order-2]/knot[cv_count-1].
          int,           // order (>=2)
          int,           // cv count
          double*,       // knot[] array
          int            // 0 = clamp left end, 1 = right end, 2 = clamp both ends
          );

ON_DECL
bool ON_MakeKnotVectorPeriodic(
          // Sets initial and final order-2 knots to values
          // that make the knot vector periodic
          int,           // order (>=2)
          int,           // cv count
          double*        // knot[] array
          );

 /*
 Description:
   Fill in knot values for a clamped uniform knot
   vector.
 Parameters:
   order - [in] (>=2) order (degree+1) of the NURBS
   cv_count - [in] (>=order) total number of control points
       in the NURBS.
   knot - [in/out] Input is an array with room for 
       ON_KnotCount(order,cv_count) doubles.  Output is
       a clamped uniform knot vector with domain
       (0, (1+cv_count-order)*delta).
   delta - [in] (>0, default=1.0) spacing between knots.
 Returns:
   true if successful
 See Also:
   ON_NurbsCurve::MakeClampedUniformKnotVector
*/
ON_DECL
bool ON_MakeClampedUniformKnotVector(
          int order,
          int cv_count,
          double* knot,
          double delta = 1.0
          );

/*
 Description:
   Fill in knot values for a clamped uniform knot
   vector.
 Parameters:
   order - [in] (>=2) order (degree+1) of the NURBS
   cv_count - [in] (>=order) total number of control points
       in the NURBS.
   knot - [in/out] Input is an array with room for 
       ON_KnotCount(order,cv_count) doubles.  Output is
       a periodic uniform knot vector with domain
       (0, (1+cv_count-order)*delta).
   delta - [in] (>0, default=1.0) spacing between knots.
 Returns:
   true if successful
 See Also:
   ON_NurbsCurve::MakePeriodicUniformKnotVector
*/
ON_DECL
bool ON_MakePeriodicUniformKnotVector(
          int order,
          int cv_count,
          double* knot,
          double delta = 1.0
          );

/*
 Description:
   Fill in knot values for a clamped uniform knot
   vector.
 Parameters:
   order - [in] (>=2) order (degree+1) of the NURBS
   cv_count - [in] (>=order) total number of control points
       in the NURBS.
   knot - [in/out] Input is an array with room for
       ON_KnotCount(order,cv_count) doubles.  Output is
       a periodic uniform knot vector with domain
       (0, (1+cv_count-order)*delta).
   delta - [in] (>0, default=1.0) spacing between knots.
 Returns:
   true if successful
 See Also:
   ON_NurbsCurve::MakePeriodicUniformKnotVector
*/
ON_DECL
bool ON_MakeUniformKnotVector(
  int order,
  int cv_count,
  bool bPeriodic,
  double* knot,
  double delta = 1.0
);

ON_DECL
double ON_GrevilleAbcissa( // get Greville abcissae from knots
          int,           // order (>=2)
          const double*  // knot[] array (length = order-1)
          );

/// <summary>
/// A periodic NURBS with N total control points and degree D = (order-1) has (N-D) Greville abcissa.
/// Note that for a periodic NURBS, ControlPoint[i] = ControlPoint[N-D+i] and 
/// (knot[i]-knot[0]) = (knot[N-1+i]-knot[N-1]), when 0 <= lt;= i &lt; D.
/// For 0 <= lt= i &lt; N-D, the i-th periodic Greville abcissa is
/// ON_GrevilleAbcissa(i + offset, periodic knots), where
/// offset = ON_GrevilleAbcissaPeriodicOffset(D+1, periodic knots) is the value returned
/// by this function. Note that for Greville interpolation, 
/// the free control points have indices offset &lt;= i &lt (offset+N-D).
/// </summary>
/// <param name="order">
/// order &gt;= 2</param>
/// <param name="knot">
/// An array of the initial (2*order-3) knots of the periodic knot vector.
/// Typically, the knot[] parameter simply points to the entire periodic knot vector.
/// </param>
/// <returns>
/// The offset into the periodic knot vector to use for calculating first Greville abcissa.
/// 0 &lt;= offset &lt;= (order - 2).
/// </returns>
ON_DECL
int ON_GrevilleAbcissaOffset(
  int order, 
  bool bPeriodic,
  const double* knot
);


/// <summary>
/// Returns the number of Greville abcissae in a NURBS with the specified properties.
/// When a NURBS is not periodic, there are cv_count Greville abcissae.
/// When a NURBS is periodic, there are (cv_count - order + 1) Greville abcissae.
/// </summary>
/// <param name="order">
/// order &gt;= 2. 
/// Note that order = degree+1.
/// </param>
/// <param name="cv_count">
/// cv_count &gt;= ON_MinimumControlPointCount(order, bPeriodic)
/// Note that if a NURBS is not periodic, then cv_count %gt;= order.
/// </param>
/// <param name="bPeriodic">
/// True if the NURBS is periodic.
/// Note that for a periodic NURBS, 
/// ControlPoint[i] = ControlPoint[cv_count-order+1+i] and 
/// (knot[i]-knot[0]) = (knot[cv_count-1+i]-knot[cv_count-1]), 
/// when 0 <= lt;= i &lt; (order-1).
/// </param>
/// <returns>
/// The number of Greville abcissae in a NURBS with the specified properties.
/// </returns>
ON_DECL
int ON_GrevilleAbcissaeCount(
  int order,
  int cv_count,
  bool bPeriodic
);

/// <summary>
/// Get the minimum number of control points required for a NURBS with a specified properties.
/// </summary>
/// <param name="order">
/// order & gt;= 2.
/// Note that order = degree + 1.
/// </param>
/// <param name="bPeriodic">
/// True if the NURBS is periodic.
/// Note that for a periodic NURBS, 
/// ControlPoint[i] = ControlPoint[cv_count-order+1+i] and 
/// (knot[i]-knot[0]) = (knot[cv_count-1+i]-knot[cv_count-1]), 
/// when 0 <= lt;= i &lt; (order-1).
/// </param>
/// <returns>
/// If order &gt;=2, then the minimum number of control points in a NURBS with the specified properties is returned.
/// Otherwise 0 is returned.
/// </returns>
ON_DECL
int ON_MinimumControlPointCount(
  int order,
  bool bPeriodic
);

ON_DECL
bool ON_GetGrevilleAbcissae( // get Greville abcissae from knots
          int,            // order (>=2)
          int,            // cv count
          const double*,  // knot[] array
          bool,           // true for periodic case
          double*         // g[] array has length cv_count in non-periodic case
                          // and cv_count-order+1 in periodic case
          );

ON_DECL
bool ON_GetGrevilleKnotVector( // get knots from Greville abcissa
          int,           // g[] array stride (>=1)
          const double*, // g[] array
                         // if not periodic, length = cv_count
                         // if periodic, length = cv_count-order+2
          bool,          // true for periodic knots
          int,           // order (>=2)
          int,           // cv_count (>=order)
          double*        // knot[cv_count+order-2]
          );

ON_DECL
bool ON_ClampKnotVector(
        int,       // cv_dim ( = dim+1 for rational cvs )
        int,       // order (>=2)
        int,       // cv_count,
        int,       // cv_stride, 
        double*,   // cv[] nullptr or array of order many cvs
        double*,   // knot[] array with room for at least knot_multiplicity new knots
        int        // end  0 = clamp start, 1 = clamp end, 2 = clamp both ends
        );


#if defined OPENNURBS_PLUS

// Description:
//   Unclamp end knots.  
//   Does not modify the curve location or parameterization,
//   but typically modifies knots and control vertices near the ends.
// Parameters:
//   cv_dim - [in] = (dim + is_rat)
//   end - [in] 0 = unclamp start, 1 = unclamp end, 2 = unclamp start and end
// Returns:
//   true if successful
// Remarks:
//   Degree 1 NURBS curves cannot be unclampled.
// See Also:
//   SetUnclampedTagForExperts
ON_DECL
bool ON_UnclampKnotVectorForExperts(
  int cv_dim,
  int order,
  int cv_count,
  int cv_stride,
  double* cv,
  double* knot,
  int end 
);

#endif



/*
Returns:
  Number of knots added.
*/
ON_DECL
int ON_InsertKnot(
        double,    // knot_value,
        int,       // knot_multiplicity, (1 to order-1 including multiplicity of any existing knots)
        int,       // cv_dim ( = dim+1 for rational cvs )
        int,       // order (>=2)
        int,       // cv_count,
        int,       // cv_stride (>=cv_dim)
        double*,   // cv[]  nullptr or cv array with room for at least knot_multiplicity new cvs
        double*,   // knot[] knot array with room for at least knot_multiplicity new knots
        int*       // hint, optional hint about where to search for span to add knots to
                   // pass nullptr if no hint is available
        );

/*
Description:
  Reparameterize a rational Bezier curve.
Parameters:
  c - [in]
    reparameterization constant (generally speaking, c should be > 0).
    The control points are adjusted so that 
    output_bezier(t) = input_bezier(lambda(t)), where
    lambda(t) = c*t/( (c-1)*t + 1 ).
    Note that lambda(0) = 0, lambda(1) = 1, lambda'(t) > 0, 
    lambda'(0) = c and lambda'(1) = 1/c.
  dim - [in]
  order - [in]
  cvstride - [in] (>= dim+1)
  cv - [in/out]  homogeneous rational control points
Returns:
  The cv values are changed so that
  output_bezier(t) = input_bezier(lambda(t)).
*/
ON_DECL
bool ON_ReparameterizeRationalBezierCurve(
          double c,
          int dim,
          int order,
          int cvstride,
          double* cv
          );

/*
Description:
  Use a combination of scaling and reparameterization to set two rational
  Bezier weights to specified values.
Parameters:
  dim - [in] 
  order - [in]
  cvstride - [in] ( >= dim+1)
  cv - [in/out]  homogeneous rational control points
  i0 - [in] 
  w0 - [in]
  i1 - [in]
  w1 - [in]
    The i0-th cv will have weight w0 and the i1-th cv will have weight w1.
    If v0 and v1 are the cv's input weights, then v0, v1, w0 and w1 must
    all be nonzero, and w0*v0 and w1*v1 must have the same sign.
Returns:
  true if successful
Remarks:
  The equations
    s * r^i0 = w0/v0
    s * r^i1 = w1/v1
  determine the scaling and reparameterization necessary to change v0,v1 to
  w0,w1. 

  If the input Bezier has control vertices {B_0, ..., B_d}, then the 
  output Bezier has control vertices {s*B_0, ... s*r^i * B_i, ..., s*r^d * B_d}.
*/
ON_DECL
bool ON_ChangeRationalBezierCurveWeights(
          int dim, int order, int cvstride, double* cv,
          int i0, double w0, 
          int i1, double w1
          );

/*
Description:
  Reparameterize a rational NURBS curve.
Parameters:
  c - [in]
    reparameterization constant (generally speaking, c should be > 0).
    The control points and knots are adjusted so that 
    output_nurbs(t) = input_nurbs(lambda(t)), where
    lambda(t) = c*t/( (c-1)*t + 1 ).
    Note that lambda(0) = 0, lambda(1) = 1, lambda'(t) > 0, 
    lambda'(0) = c and lambda'(1) = 1/c.
  dim - [in]
  order - [in]
  cvstride - [in] (>=dim+1)
  cv - [in/out]  homogeneous rational control points
  knot - [in/out]
    NURBS curve knots
Returns:
  The cv values are changed so that
  output_bezier(t) = input_bezier(lambda(t)).
See Also:
  ON_ChangeRationalNurbsCurveEndWeights
*/
ON_DECL
bool ON_ReparameterizeRationalNurbsCurve(
          double c, 
          int dim, 
          int order, 
          int cv_count,
          int cvstride,
          double* cv,
          double* knot
          );

/*
Description:
  Use a combination of scaling and reparameterization to set the end
  weights to the specified values.  This 
Parameters:
  dim - [in]
  order - [in]
  cvstride - [in] (>=dim+1)
  cv - [in/out] homogeneous rational control points
  knot - [in/out] (output knot vector will be clamped and internal
                   knots may be shifted.)
  w0 - [in]
  w1 - [in]
    The first cv will have weight w0 and the last cv will have weight w1.
    If v0 and v1 are the cv's input weights, then v0, v1, w0 and w1 must
    all be nonzero, and w0*v0 and w1*v1 must have the same sign.
Returns:
  true if successful
See Also:
  ON_ReparameterizeRationalNurbsCurve
*/
ON_DECL
bool ON_ChangeRationalNurbsCurveEndWeights(
          int dim, 
          int order, 
          int cv_count,
          int cvstride, 
          double* cv, 
          double* knot,
          double w0, 
          double w1
          );

#endif
