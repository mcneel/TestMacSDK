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

/*-------------------------------------------------------------------
CLASS
  CArgsRhinoGetCurve

OVERVIEW TEXT
  Arguments for RhinoGetControlPointCurve, RhinoGetInterpolatedCurve and RhinoGetInterpolatedCurveOnSurface

KEYWORDS
  RhinoGetControlPointCurve RhinoGetInterpolatedCurve Curve CVCurve CPCurve InterpCrv InterpCrvOnSrf
-------------------------------------------------------------------*/
class RHINO_SDK_CLASS CArgsRhinoGetCurve
{
public:
  CArgsRhinoGetCurve();
  ~CArgsRhinoGetCurve();

  CArgsRhinoGetCurve(const CArgsRhinoGetCurve&);
  CArgsRhinoGetCurve& operator=(const CArgsRhinoGetCurve&);

  // prompt when getting first point
  // Default = "Start of curve".
  void SetFirstPrompt( const wchar_t* );
  const ON_wString& FirstPrompt() const;

  // prompt when getting second point
  // Default = "Next point".
  void SetSecondPrompt( const wchar_t* );
  const ON_wString& SecondPrompt() const;

  // prompt when getting third and following points 
  // Default = "Next point. Press Enter when done".
  void SetThirdPrompt( const wchar_t* );
  const ON_wString& ThirdPrompt() const;

// interpolated curve start tangent, ON_3dVector(0,0,0) if not defined
  void SetStartTangent( const ON_3dVector&);
  void SetUseStartTangent( BOOL32);
  const ON_3dVector& StartTangent() const;
  BOOL32 UseStartTangent() const;

// interpolated curve end tangent, ON_3dVector(0,0,0) if not defined
  void SetEndTangent( const ON_3dVector&);
  void SetUseEndTangent( BOOL32);
  const ON_3dVector& EndTangent() const;
  BOOL32 UseEndTangent() const;

  // degree of curve
  void SetDegree( int);
  int Degree() const;

  // Surface for Interpolate curve on surface
  void SetSurface( const ON_Surface* pS);
  const ON_Surface* Surface() const;

  // Method of Knot Spacing for interpolated curve 
  //   0=uniform, 1=chord, 2=sqrt(chord)
  void SetKnotSpacing( int);
  int KnotSpacing() const;

  // // FALSE = periodic when closed, TRUE = sharp when closed
  void SetSharp( BOOL32);
  BOOL32 Sharp() const;

  // Array of selected points
  ON_3dPointArray& PointArray();

  // Array of selected parameter space points
  ON_2dPointArray& PointArray2d();

  /*
  Returns:
    True if the resulting curve will be a cubic, uniform, nonrational
    and either periodic or have zero 2nd deriviatives at the ends.
  Remarks:
    SubD friendly curves are cubic, nonrational, uniform NURBS that are either periodic or have
    zero 2nd derivatives at the start and end.
    If SubDFriendly() is true, then degree, knot spacing, and end condition parameters are ignored
    in curve creation commands that use CArgsRhinoGetCurve for input.
  */
  bool SubDFriendly() const;
  
  /*
  Parameters:
    bSubDFriendly - [in]
  Returns:
    Resulting value of SubDFriendly().
  Remarks:
    SubD friendly curves are cubic, nonrational, uniform NURBS that are either periodic or have
    zero 2nd derivatives at the start and end.
    If SubDFriendly() is true, then degree, knot spacing, and end condition parameters are ignored
    in curve creation commands that use CArgsRhinoGetCurve for input.
  */
  bool SetSubDFriendly(
    bool bSubDFriendly
  );

  // When true the linetype of the active layer is used for dynamic drawing
  void SetUseActiveLayerLinetype(bool on);
  bool UseActiveLayerLinetype() const;

protected:
  ON_3dPointArray m_PointArray; 
  ON_2dPointArray m_PointArray2d; 

  ON_wString  m_FirstPrompt; 
  ON_wString  m_SecondPrompt; 
  ON_wString  m_ThirdPrompt; 
  
  // If 3 == m_Degree and m_bSubDFriendly is true, then
  // m_StartTangent is ignored.
  ON_3dVector m_StartTangent = ON_3dVector::ZeroVector; 

  // If 3 == m_Degree and m_bSubDFriendly is true, then
  // m_EndTangent is ignored.
  ON_3dVector m_EndTangent = ON_3dVector::ZeroVector; 
  
  const ON_Surface* m_pS = nullptr; 

  int m_Degree = 3; 

  // If 3 == m_Degree and m_bSubDFriendly is true, then
  // m_KnotSpacing is ignored.
  int m_KnotSpacing = 1; //   0=uniform, 1=chord, 2=sqrt(chord)

  BOOL32 m_Sharp = false; 

  // If 3 == m_Degree and m_bSubDFriendly is true, then
  // m_UseStartTangent is ignored.
  BOOL32 m_UseStartTangent = false;

  // If 3 == m_Degree and m_bSubDFriendly is true, then
  // m_UseEndTangent is ignored.
  BOOL32 m_UseEndTangent = false;

private:
  // If 3 != m_Degree and m_bSubDFriendly is true, then
  // m_bSubDFriendly is ignored.
  bool m_bSubDFriendly = false;

private:
  unsigned char m_pen_usage = 0;
  unsigned short m_reserved2 = 0;
};

/*
Description:
  Interactively get an interpolated curve on surface.
Parameters:
  pS - [in] surface on which to draw curve, or NULL to prompt user
  ppCout - [out] curve returned here
  pObjRefOut - [out] optionally the picked face is returned here if pS = NULL
  pPointsOut - [out] optionally the picked 2d points are returned here
Returns:
  CRhinoCommand::success - successfully got curve
  CRhinoCommand::cancel - user canceled
  CRhinoCommand::failure - unable to get valid curve
Remarks:
  The Rhino "InterpCrvOnSrf" command uses RhinoGetInterpolatedCurveOnSurface.
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetInterpolatedCurveOnSurface( 
  const ON_Surface* pS, 
  ON_Curve** ppCout,
  CRhinoObjRef* pObjRefOut = 0, 
  ON_SimpleArray<ON_2dPoint>* pPointsOut = 0
  );

/*
Description:
  Interactively get a control point curve.
Parameters:
  args - [in/out] controls prompts and options available when getting
           the curve.
  curve_out - [out] curve returned here
Returns:
  CRhinoCommand::success - successfully got curve
  CRhinoCommand::cancel - user canceled
  CRhinoCommand::failure - unable to get valid curve
Remarks:
  The Rhino "Curve" command uses RhinoGetControlPointCurve.
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetControlPointCurve(
  CArgsRhinoGetCurve& args, 
  ON_NurbsCurve& curve_out  
  );

/*
Description:
  Interactively get an interpolated curve.
Parameters:
  args - [in/out] controls prompts and options available when getting
           the curve.
  curve_out - [out] curve returned here
Returns:
  CRhinoCommand::success - successfully got curve
  CRhinoCommand::cancel - user canceled
  CRhinoCommand::failure - unable to get valid curve
Remarks:
  The Rhino "InterpCrv" command uses RhinoGetInterpolatedCurve.
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetInterpolatedCurve( 
  CArgsRhinoGetCurve& args, 
  ON_NurbsCurve& curve_out 
  );

