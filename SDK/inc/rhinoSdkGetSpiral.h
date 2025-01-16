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
  CArgsRhinoGetSpiral

OVERVIEW TEXT
  Arguments for RhinoGetHelix and RhinoGetSpiral

KEYWORDS
  Helix Spiral RhinoGetHelix RhinoGetSpiral
-------------------------------------------------------------------*/
class RHINO_SDK_CLASS CArgsRhinoGetSpiral
{
public:
  CArgsRhinoGetSpiral();
  ~CArgsRhinoGetSpiral();

  CArgsRhinoGetSpiral(const CArgsRhinoGetSpiral&);
  CArgsRhinoGetSpiral& operator=(const CArgsRhinoGetSpiral&);

  // sets
  void SetStartPrompt( const wchar_t*);
  void SetEndPrompt( const wchar_t*);
  void SetRadiusPrompt( const wchar_t*);
  void SetDiameterPrompt( const wchar_t*);
  void SetFirstRadiusPrompt( const wchar_t*);
  void SetFirstDiameterPrompt( const wchar_t*);
  void SetSecondRadiusPrompt( const wchar_t*);
  void SetSecondDiameterPrompt( const wchar_t*);
  void SetCenterPrompt( const wchar_t*);

  void SetStartPoint( const ON_3dPoint& );
  void SetEndPoint( const ON_3dPoint& );
  void SetRadiusPoint( const ON_3dPoint& );
  void SetStartDir( const ON_3dVector& );
  void SetEndDir( const ON_3dVector& );
  
  void SetRail( const ON_Curve*);

  void SetTurns( double);
  void SetPitch( double);
  void SetLength( double);
  void SetFirstRadius( double);
  void SetSecondRadius( double);
  void SetPointsPerTurn( double);
  
  void SetFlat( BOOL32);
  void SetPitchMode( BOOL32);
  void SetHelixMode( BOOL32);
  void SetReverseTwist( BOOL32);
  void SetVertical( BOOL32);
  void SetAroundCurve( BOOL32);
  void SetToleranceWarning( BOOL32);
  void SetDiameterMode( BOOL32);

  // gets
  const ON_wString& StartPrompt() const;
  const ON_wString& EndPrompt() const;
  const ON_wString& RadiusPrompt() const;
  const ON_wString& DiameterPrompt() const;
  const ON_wString& FirstRadiusPrompt() const;
  const ON_wString& FirstDiameterPrompt() const;
  const ON_wString& SecondRadiusPrompt() const;
  const ON_wString& SecondDiameterPrompt() const;
  const ON_wString& CenterPrompt() const;

  const ON_3dPoint& StartPoint() const;
  const ON_3dPoint& EndPoint() const;
  const ON_3dPoint& RadiusPoint() const;
  const ON_3dVector& StartDir() const;
  const ON_3dVector& EndDir() const;
  
  ON_Curve* Rail() const;

  const double Turns() const;
  const double Pitch() const;
  const double Length() const;
  const double FirstRadius() const;
  const double SecondRadius() const;
  const double PointsPerTurn() const;
  
  const BOOL32 Flat() const;
  const BOOL32 PitchMode() const;
  const BOOL32 HelixMode() const;
  const BOOL32 ReverseTwist() const;
  const BOOL32 Vertical() const;
  const BOOL32 AroundCurve() const;
  const BOOL32 ToleranceWarning() const;
  const BOOL32 DiameterMode() const;

  double m_Turns;
  double m_Pitch;
  double m_PointsPerTurn;
  BOOL32 m_PitchMode;
  BOOL32 m_ReverseTwist;

protected:
  ON_wString m_StartPrompt;
  ON_wString m_EndPrompt;
  ON_wString m_RadiusPrompt;
  ON_wString m_DiameterPrompt;
  ON_wString m_FirstRadiusPrompt;
  ON_wString m_FirstDiameterPrompt;
  ON_wString m_SecondRadiusPrompt;
  ON_wString m_SecondDiameterPrompt;
  ON_wString m_CenterPrompt;

  ON_3dPoint m_StartPoint;
  ON_3dPoint m_EndPoint;
  ON_3dPoint m_RadiusPoint;
  ON_3dVector m_StartDir;
  ON_3dVector m_EndDir;
  
  ON_Curve* m_pRail;

  double m_Length;
  double m_FirstRadius;
  double m_SecondRadius;
  
  BOOL32 m_Flat;
  BOOL32 m_HelixMode;
  BOOL32 m_Vertical;
  BOOL32 m_AroundCurve;
  BOOL32 m_ToleranceWarning;
  BOOL32 m_DiameterMode;
};

/*
Description:
  Interactively creates a spiral curve.
Parameters:
  args       - [in/out] The creation arguments. 
  curve_out  - [out]    The resulting spiral curve.
  history    - [out]    Optional history.
Returns:
  CRhinoCommand::success if successful.
See Also:
  RhinoGetHelix
  RhinoCreateSpiral
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetSpiral(
  CArgsRhinoGetSpiral& args,
  ON_NurbsCurve& curve_out,
  CRhinoHistory* history = NULL
  );

/*
Description:
  Interactively creates a helical curve.
Parameters:
  args       - [in/out] The creation arguments. 
  curve_out  - [out]    The resulting helical curve.
  history    - [out]    Optional history.
Returns:
  CRhinoCommand::success if successful.
See Also:
  RhinoGetSpiral
  RhinoCreateSpiral
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetHelix(
  CArgsRhinoGetSpiral& args, 
  ON_NurbsCurve& curve_out,
  CRhinoHistory* history = NULL
  );

/*
Description:
  Calculates a spiral or helical curve based on set of input arguments.
  Note, RhinoGetHelix and RhinoCreateSpiral call this function to perform
  the actual curve calculation.
Parameters:
  args       - [in]  The creation arguments. 
  curve_out  - [out] The resulting spiral or helical curve.
Returns:
  True is successful, false otherwise.
See Also:
  RhinoGetHelix
  RhinoGetSpiral
*/
RHINO_SDK_FUNCTION
bool RhinoCreateSpiral(
  CArgsRhinoGetSpiral& args,
  ON_NurbsCurve& curve_out
  );

/*
Description:
  Low-level function to calculate the C1 cubic NURBS approximation
  of a helix or spiral.
Parameters:
  axis_start   - [in] Helix's axis starting point or center of spiral.
  axis_dir     - [in] Helix's axis vector or normal to spiral's plane.
  radius_point - [in] Point used only to get a vector that is perpedicular to
                      the axis.  In particular, this vector must not
                      be (anti)parallel to the axis vector.
  pitch        - [in] = 0 : Spiral
                      > 0 : Distance between helix's "threads"
  turn_count   - [in] != 0: Number of turns in spiral or helix.
                      > 0 : Right handed or counter-clockwise orientation.
                      < 0 : Left handed or clockwise orientation.
                     Note, for a helix, turn_count * pitch = length of the helix's axis.
  radius0      - [in] Starting radius.      
  radius1      - [in] Ending radius.
  curve_out    - [out] The output NURBS curve.
Returns:
  True if successful, false otherwise.
Remarks:
  For a helix, you may have r0 == r1.
  For a spiral r0 == r1 produces a circle.
  Zero and negative radii are permissible.
*/
RHINO_SDK_FUNCTION
bool RhinoCreateSpiral(
  ON_3dPoint axis_start, 
  ON_3dVector axis_dir, 
  ON_3dPoint radius_point,
  double pitch, 
  double turn_count,
  double radius0, 
  double radius1,
  ON_NurbsCurve& curve_out
  );

/*
Description:
  Low-level function to calculate the C2 non-rational uniform cubic NURBS
  approximation of a swept helix or spiral
Parameters:
  rail            - [in] The rail curve.
  rail_t0         - [in] Starting portion of rail curve's domain to sweep along.
  rail_t1         - [in] Ending portion of rail curve's domain to sweep along.
  radius_point    - [in] Point used only to get a vector that is perpedicular to 
                         rail'(rail_t0). So (start_point-rail(rail_t0)
                         must not be parallel to rail'(rail_t0).
  pitch           - [in] The pitch (see Remarks).
  turn_count      - [in] The turn count (see Remarks).
  radius0         - [in] Starting radius (see Remarks).               
  radius1         - [in] Ending radius (see Remarks).
  points_per_turn - [in] Number of points to interpolate per turn. Value must be
                    >= 4. 12 seems to work well.
  curve_out       - [out] The output NURBS curve
Returns:
  True if successful, false otherwise.
Remarks:
  If turn_count != 0, then the resulting helix will have this many turns.
  If turn_count == 0, then pitch must be != 0 and the approximate distance
  between turn will be set to pitch.  Use positive turn_count/pitch for
  right handed helices and negative turn_count/pitch for left handed helices.
  At least radii one must be nonzero. Negative values are allowed.
*/
RHINO_SDK_FUNCTION
bool RhinoCreateSpiral(
  const ON_Curve& rail,
  double rail_t0, 
  double rail_t1,
  ON_3dPoint radius_point,
  double pitch, 
  double turn_count,
  double radius0, 
  double radius1,
  int points_per_turn,
  ON_NurbsCurve& curve_out
  );

