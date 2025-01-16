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

class RHINO_SDK_CLASS CArgsRhinoFair
{
public:
  enum eClamp
  {
    ecNone = 0,
    ecTangency = 1,
    ecCurvature = 2,
  };

  CArgsRhinoFair();
  ~CArgsRhinoFair();
  

  /*
  Description:
    Get or set the input curve to fair
  Parameters:
    [in]  pC - pointer to the curve
  Returns: 
    a new faired version of the curve
  */
  const ON_Curve* InputCurve() const;
  void SetInputCurve( const ON_Curve* pC);


  /*
  Description:
    Get or set the fairing position tolerance
  Parameters:
    [in] tol - the new tolerance
  Returns:
    the fairing tolerance
  Remarks:
    The faired curve can deviate from the input by this much
  */
  double Tolerance() const;
  void SetTolerance( double tol);

  /*
  Description:
    Get or set the angle tolerance
  Parameters:
    [in] tol - the new tolerance
  Returns:
    the angle tolerance
  Remarks:
    Kinks less than this will be removed
  */
  double AngleTolerance() const;
  void SetAngleTolerance( double tol);

  
  /*
  Description:
    Get or set the number of CV's on the start to keep in place
  Parameters:
    [in]  dercount - Number of derivatives to fix
  Returns:
    Number of derivatives fixed
  Remarks:
    0: position, 1: tangency, 2: curvature
  */
  int ClampStart() const;
  void SetClampStart( int dercount);

  /*
  Description:
    Get or set the number of CV's on the end to keep in place
  Parameters:
    [in]  dercount - Number of derivatives to fix
  Returns:
    Number of derivatives fixed
  Remarks:
    0: position, 1: tangency, 2: curvature
  */
  int ClampEnd() const;
  void SetClampEnd( int dercount);
  

  /*
  Description:
    Set or get the number of iteratoins to use in adjusting the curve
  Parameters:
    [in] count - Number of iterations
  Returns:
    Number of iterations
  */
  int IterationCount() const;
  void SetIterationCount( int count);


private:
  const ON_Curve* m_pCurve;// The curve to fair - RhinoFairCurve uses an
                           // algorithm that works best on degree 3 curves
                           // with simple knots.  Results may not be acceptable
                           // on other curves.
  double m_tolerance;      // maximum allowed distance from smoothed nurb
                           // to input nurb 
  double m_angle_tolerance;// (in radians) kinks with angles <= angle_tol 
                           // are smoothed out.
  int  m_clamp_start;      // number of cvs -1 to preserve at start
                           // Ignored when input curve is not degree 3.
                           // 0 preserve start point
                           // 1 preserve start point and 1st derivative
                           // 2 preserve start point, 1st and 2nd derivative
  int  m_clamp_end;        // number of cvs -1 to preserve at end
                           // Ignored when input curve is not degree 3.
                           // 0 preserve start point
                           // 1 preserve start point and 1st derivative
                           // 2 preserve start point, 1st and 2nd derivative
  int  m_iteration_count;  // >= 0 number of fairing iterations
                           // 20 to 100 seems to work well;
};



  /*
  Description:
    Fair a curve according to the settings in args  
    The input curve, args.m_pCurve, is not changed.
  Parameters:
    [in] args - What to fair and how to fair it ( see CArgsRhinoFair)
  Returns:
    The resulting faired curve.  Caller must free this when done with it.
  */
RHINO_SDK_FUNCTION
ON_Curve* RhinoFairCurve( CArgsRhinoFair& args);

