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

#error Work in progress - do not use



class RHINO_SDK_CLASS CRhinoCCXEvent
{
public:

  /*
  There are two valid curve-curve intersection events
    CCX_POINT - curves intersect in an isolated point
      a[0] = a[1] = first curve parameter
      A[0] = A[1] = intersection point on first curve
      b[0] = b[1] = second curve parameter
      B[0] = B[1] = intersection point on second curve

    CCX_OVERLAP
      (a[0],a[1]) = first curve parameter range
      A[0] = intersection start point on first curve
      A[1] = intersection end point on first curve
      (b[0],b[1]) = second curve parameter range
      B[0] = intersection start point on second curve
      B[1] = intersection end point on second curve
  */
  enum EVENT_TYPE
  { 
    CCX_NO_EVENT =  0,
    CCX_POINT    =  1, // curve-curve intersection point
    CCX_OVERLAP  =  2, // curve-curve intersection overlap
    CCX_EVENT_TYPE__FORCE__INT = 0xFFFFFFFF
  }; 


  EVENT_TYPE m_type;

  // intersection point(s) on first curve
  ON_3dPoint m_A[2]; 

  // intersection point(s) on second curve
  ON_3dPoint m_B[2]; 

  // intersection parameter(s) on first curve
  double m_a[2];

  // intersection parameter(s on second curve
  double m_b[2];
  
  /*
  Description:
    Compare "this" intersection event with "other".
  Parameters:
    other - [in]
  Returns:
    @untitled table
    -1    this < other
     0    this = other
    +1    this > other
  Remarks:
    Compare is used to sort intersection events into canonical
    order.
  */
  int Compare( 
    const CRhinoCCXEvent& other 
    ) const;
};



class RHINO_SDK_CLASS CRhinoCSXEvent
{
public:

  /*
  There are two valid curve-surface intersection events
    CSX_POINT - curves intersect in an isolated point
      a[0] = a[1] = curve parameter
      A[0] = A[1] = intersection point on curve
      b[0] = b[1] = surface parameter
      B[0] = B[1] = intersection point on surface

    CSX_OVERLAP
      (a[0],a[1]) = curve parameter range
      A[0] = intersection start point on first curve
      A[1] = intersection end point on first curve
      b[0] = surface parameter for start point
      b[1] = surface parameter for end point
      B[0] = intersection start point on surface
      B[1] = intersection end point on surface
  */
  enum EVENT_TYPE
  { 
    CSX_NO_EVENT =  0,
    CSX_POINT    =  1, // curve-surface intersection point
    CSX_OVERLAP  =  2, // curve-surface intersection overlap
    CSX_EVENT_TYPE__FORCE__INT = 0xFFFFFFFF
  }; 


  EVENT_TYPE m_type;

  // intersection point(s) on curve
  ON_3dPoint m_A[2]; 

  // intersection point(s) on surface
  ON_3dPoint m_B[2]; 

  // intersection parameter(s) on curve
  double m_a[2];

  // intersection parameter(s) on surface
  ON_2dPoint m_b[2];
  
  /*
  Description:
    Compare "this" intersection event with "other".
  Parameters:
    other - [in]
  Returns:
    @untitled table
    -1    this < other
     0    this = other
    +1    this > other
  Remarks:
    Compare is used to sort intersection events into canonical
    order.
  */
  int Compare( 
    const CRhinoCSXEvent& other 
    ) const;
};

#if defined (ON_COMPILER_MSC)
#if defined(RHINO_SDK_TEMPLATE)
// This stuff is here because of a limitation in the way Microsoft
// handles templates and DLLs.  See Microsoft's knowledge base 
// article ID Q168958 for details.
#pragma warning( push )
#pragma warning( disable : 4231 )
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_SimpleArray<CRhinoCCXEvent>;
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_SimpleArray<CRhinoCSXEvent>;
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_SimpleArray<CRhinoSSXEvent>;
#pragma warning( pop )
#endif
#endif

RHINO_SDK_FUNCTION
int RhinoCompareCCXEvent(const CRhinoCCXEvent*, const CRhinoCCXEvent*);

/*
Description:
  Intersect two curves.
Parameters:
  curveA - [in]
  curveB - [in]
  x - [out] array of intersection events sorted by curveA
            parameters.
Remarks:
  The curves must be 2 or 3 dimensional and have the same dimension.
  If the curves are the same, a single overlap event will be returned.
See Also:
  RhinoCurveSelfIntersect
*/
RHINO_SDK_FUNCTION
int RhinoCurveCurveIntersect(
       const ON_Curve& curveA,
       const ON_Curve& curveB,
       double tolerance,
       ON_SimpleArray<CRhinoCCXEvent>& x,
       const ON_Interval* curveA_domain=NULL,
       const ON_Interval* curveB_domain=NULL
       );

/*
Description:
  Intersect two curves.
Parameters:
  curve - [in]
  tolerance - [in] intesection tolerance
  x - [out] array of intersection events sorted by curveA
            parameters.
Remarks:
  The curves must be 2 or 3 dimensional and have the same dimension.
  If the curves are the same, a single overlap event will be returned.
See Also:
  RhinoCurveSelfIntersect
*/
RHINO_SDK_FUNCTION
int RhinoCurveSelfIntersect(
       const ON_Curve& curve,
       double tolerance,
       ON_SimpleArray<CRhinoCCXEvent>& x,
       const ON_Interval* curveA_domain=NULL,
       const ON_Interval* curveB_domain=NULL
       );




/*
Description:
  Intersect a curve and a surface.
Parameters:
  curve - [in]
  surface - [in]
  distance_tolerance - [in] intersection 3d tolerance
  angle_tolerance_radians - [in] (>=0.0) angle tolerance in radians 
      (used to determine when a curve is tangent to the surface)
  x - [out] intersection event are appended to this array
  curve_domain - [in] if not NULL, the intersection is restricted to
      this subdomain of curve1
  surface_u_domain - [in] if not NULL, the intersection is restricted to
      this subdomain of of the first surface parameter.
  surface_v_domain - [in] if not NULL, the intersection is restricted to
      this subdomain of of the second surface parameter.
Returns:
  Number of intersection events added to x[].  A return value of
  zero indicates no intersections were found.
*/
RHINO_SDK_FUNCTION
int RhinoCurveSurfaceIntersect(
       const ON_Curve& curve,
       const ON_Surface& surface,
       double distance_tolerance,
       double angle_tolerance_radians,
       ON_SimpleArray<CRhinoCSXEvent>& x,
       const ON_Interval* curve_domain = NULL,
       const ON_Interval* surface_u_domain = NULL,
       const ON_Interval* surface_v_domain = NULL
       );

