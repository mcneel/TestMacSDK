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

#if !defined(OPENNURBS_PLUS_VALIDATE_INC_)
#define OPENNURBS_PLUS_VALIDATE_INC_

/*
Find the first span of a curve that satisfies a property given by a check function

INPUT:
  crv
  t0, t1 - Must be increasing.  Search subdomain
  tolerance - Used by the check function
  check - function that takes the above parameters

OUTPUT:
  seg_dom - If Non-nullptr on input, Subdomain of the span that passes check.
  span_index - If Non-nullptr on input, the index of the span.

Returns true if a span is found for which check returns true.
*/
ON_DECL
bool ON_GetNextCurveSpan(const ON_Curve& crv, 
                         double t0, 
                         double t1,
                         double tolerance,
                         //Test function for subdom given by first two doubles.  Third is a tolerance
                         bool (*check)(const ON_Curve&, double, double, double),
                         ON_Interval* seg_dom,
                         int* span_index
                         );

/*
Find short spans of a curve.

INPUT:
  crv
  tolerance - Any span with arclength (approximated) less than tolerance is short

OUTPUT:
 span_indices - If non-nullptr, this will be the list of all short spans. If nullptr, then
                function will return as soon as a short span is hit.

Returns - True if there are short spans.
*/

ON_DECL
bool ON_GetShortSpans(const ON_Curve& crv, 
                         double tolerance, 
                         ON_SimpleArray<int>* span_indices
                         );
/*
Find spans of a curve that have high curvature.

INPUT:
  crv
  tolerance - Look for spans with max curvature greater than tolerance.

OUTPUT:
 span_indices - If non-nullptr, this will be the list of all found spans. If nullptr, then
                function will return as soon as a high curvature span is hit.

Returns - True if there are spans with high curvature.
*/

ON_DECL
bool ON_GetHighCurvatureSpans(const ON_Curve& crv, 
                         double tolerance, 
                         ON_SimpleArray<int>* span_indices
                         );

/* Find a local minimum of a one parameter function using a quadratic approximation algorithm.
 * This algorithm does not use derivative information.
 *
 * INPUT:
 *   ax, bx, cx a bracketed minimum satisfying conditions 1 and 2.
 *      1) ax < bx < cx.
 *      2) f(bx) < f(ax) and f(bx) < f(cx).
 *   farg
 *      pointer passed to function f()
 *   f
 *      evaluation function with prototype
 *              int f(void* farg,double t,double* ft)
 *      f(farg,t,&ft) should compute ft = value of function at t
 *      -1: failure
 *       0: success
 *       1: |f(x)| is small enough -  will return *t_addr = x
 *          and the return code 1.
 *   desired_min - once a function value this small is found, stop. 
 *   stop_tol - once it is determined that a min exists in an interval of this size, stop
 *   max_it ( >= 2)
 *      maximum number of iterations to permit (when in doubt use 100)
 *
 * OUTPUT:
 *   *t_addr abcissa of a local minimum between ax and cx.
 *   ON_SimpleQuadraticMinimize()
 *       0: successful
 *       1: The search for a minimum was stopped because the function f()
 *          returned a 1 indicating that the current value of t was small
 *          enough.
 *       2: After max_iteration_cnt iterations the tolerance restrictions
 *          where not satisfied.  Try increasing max_it, 
 *          or use the value of (*t_addr) with extreme caution.
 *      -1: failure 
 *
 *
 */
ON_DECL
int ON_SimpleQuadraticMinimize(int (*f)(void*, double, double*), void* f_arg, 
                               double ax, double bx, double cx,
                               double desired_min,
                               double stop_tol,
                               int max_it,
                               double *t_addr=0,
                               double *min_f=0
                               );

/*
Class to check the relationship between pairs of curves in a set.
Looks for pairs that overlap.  
  Only consider overlaps that start and end at curve endpoints.
  Check 5 points including endpoints
  Classify overlaps as 
    match - curves seem to be identical to within close_g0_tol
    partial - overlap starts at an endpt of one curve and ends at an endpt of the other.
              parts of each curve are not contained in the overlap
    supercurve, subcurve - one curve is contained within the other.
Looks for endpoints that are close but not close enough, wrt g0, g1, g2;
  If endpt A is within close_gx_tol of endtpt B, but not within good_gx_tol, it is reported.
g0 tol is 3d distance.
g1 tol is angle in radians
g2 tol is a percentage - if fabs(kA - kB) < tol/200*(kA + kB), then within tol
*/

class ON_CLASS ON_CurveSetOverlapCheck

{
public:
  ON_CurveSetOverlapCheck(const ON_SimpleArray<const ON_Curve*>& Crvs,
                          double close_g0_tol, double good_g0_tol,
                          double close_g1_tol, double good_g1_tol,
                          double close_g2_tol, double good_g2_tol
                          );
  ~ON_CurveSetOverlapCheck();


  bool OverlapCheck();
  bool EndCheck();
  bool Check();//Check both
  void GetProblemEvents(ON_SimpleArray<ON_2dex>& overlaps,
                        ON_SimpleArray<ON_Interval>& overlap_subdoms,//of overlaps[n].i
                        //Curve id, end, curve id, end
                        ON_SimpleArray<ON_4dex>& g0Probs,
                        ON_SimpleArray<ON_4dex>& g1Probs,
                        ON_SimpleArray<ON_4dex>& g2Probs
                        );

private:
  enum ovlp_type {
    unknown_ovlp = 0,
    not_ovlp = 1,
    partial_start = 2,
    partial_end = 3,
    partial_interior = 4,
    match = 5,
    subcurve = 6,
    supercurve = 7
  };

  enum end_type {
    unknown_g = 0,
    not_g0 = 1,//ends not close
    bad_g0 = 2,//ends close but not exact
    not_g1 = 3,//ends meet but a definite kink
    bad_g1 = 4,//g0, close to g1 but not exact
    not_g2 = 5,//g1, definite curvature kink
    bad_g2 = 6,//g1, nearly g2 but not quite
    g2 = 7
  };

  struct ovlp_event {
    int other_cid;
    ON_Interval this_dom;
  };

  struct end_event {
    int this_end;
    int cid;
    int other_end;
    end_type type;
  };

  const ON_SimpleArray<const ON_Curve*>* m_Crvs;
  ON_SimpleArray<ovlp_event>* m_OverlapEvents;
  ON_SimpleArray<end_event>* m_EndEvents;

  double m_g0_tol[2];//good, close
  double m_g1_tol[2];//cos(good), cos(close)
  double m_g2_tol[2];//0.01*good, 0.01*close
  ON_3dPointArray m_EndPts[2];//arrays of start and end points, parallel to m_Crvs

  bool OverlapCheck(int i, int j);
  bool EndCheck(int i, end_event& e) const;//returns true if a bad end match was found.

  bool GetPotentialOverlaps(int i, ON_SimpleArray<int>& ids) const;
  bool GetPotentialEndMatches(int i, ON_SimpleArray<end_event>& ids) const;
};

ON_DECL
bool ON_ValidateCurveSet(const ON_SimpleArray<const ON_Curve*>& Crvs,
                         double close_dist_tol, double good_dist_tol,
                         double close_kink_tol, double good_kink_tol,
                         double close_curvature_tol, double good_curvature_tol,
                         ON_SimpleArray<ON_2dex>& overlap_problems,
                         ON_SimpleArray<ON_Interval>& overlap_subdoms,//For Crvs[overlap_problems[n].i]
                         ON_SimpleArray<ON_4dex>& g0_problems,
                         ON_SimpleArray<ON_4dex>& g1_problems,
                         ON_SimpleArray<ON_4dex>& g2_problems
                         );

// If srf.IsSingular(side) == false, check control points along srf's side. 
// If all are within tolerance, but not equal, return false. 
// Otherwise, return true.  
// Note: If side is invalid, or if it does not make sense for srf to have a pole along side,
// this will return true.

ON_DECL 
bool ON_ValidateNurbsSurfacePole(const ON_NurbsSurface& srf, 
                                 int side, //0-3, SENW same as O_Surface::IsSingular()
                                 double tolerance, 
                                 ON_3dPoint* pAvgPole // if not nullptr on input, and return is true,
                                                      // result will have the average location of the cv's along the side.
                                );

ON_DECL 
void ON_FixNurbsSurfacePole(ON_NurbsSurface& srf, int side, const ON_3dPoint* pPole);

ON_DECL 
bool ON_ValidateSurfaceSeam(const ON_Surface& srf, int dir, double dist_tol, double angle_tol, bool& bIsCloseG1);


ON_DECL 
void ON_FixNurbsSurfaceSeam(ON_NurbsSurface& srf, int dir, bool bMakeG1);

// Simple check of the distance between adjacent surface cvs.
// If stack_tol < ON_ZERO_TOLERANCE, ON_ZERO_TOLERANCE will be used.
// If close_tol <= stack_tol, only stacked_ids will be filled in.
// Cvs at poles will not be considered close or stacked to other cvs at the same pole.
// Output lists will be sorted by u then v.
ON_DECL
bool ON_ValidateSurfaceCVSpacing(const ON_NurbsSurface& srf, 
                                 double close_tol, 
                                 double stack_tol,
                                 ON_SimpleArray<ON_2dex>& close_ids,
                                 ON_SimpleArray<ON_2dex>& stacked_ids
                                 );

ON_DECL
bool ON_ValidateCurveCVSpacing(const ON_NurbsCurve& crv, 
                               double close_tol, 
                               double stack_tol,
                               ON_SimpleArray<int>& close_ids,
                               ON_SimpleArray<int>& stacked_ids,
                               double& chord_length //of control polygon
                               );

//Look for self-intersecting boundaries.  Currently limited to 
//slits and improper links between what should be an outer and inner boundary.
//If loop_ids == 0, returns as soon as a bad loop is found.  Otherwise, the ids of all bad loops
//will end up on loop_ids
//True if no bad boundary is found
ON_DECL
bool ON_ValidateBrepLoops(const ON_Brep& B, ON_SimpleArray<int>* loop_ids=0);

//Repair problems found in ON_ValidateBrepLoops().  Returns true if something changed.
//If loop_ids !=0, it is the list of loops to fix (they will not be checked first).   Otherwise, all loops will be checked and repaired
ON_DECL 
bool ON_FixBrepLoops(ON_Brep& B, ON_SimpleArray<int>* loop_ids=0);

//See ON_ValidateBrepLoops
ON_DECL
bool ON_ValidateBrepLoop(const ON_Brep& B, int loop_id);

//Fix problems found in ON_ValidateBrepLoop.  Returns true if something changed.
//If bCompact is true and something has changed, the result will be compacted.
//NOTE: This may add to B.m_L, so any refs or pointers to loops should be reset.
//Any added loops will go to the end of the array, so indices of existing loops, other than loop_id, will still be valid.
ON_DECL 
bool ON_FixBrepLoop(ON_Brep& B, int loop_id, bool bCompact=true);

#endif
