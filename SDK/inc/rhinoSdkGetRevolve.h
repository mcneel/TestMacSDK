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

class RHINO_SDK_CLASS CArgsRhinoRevolve
{
public:
  CArgsRhinoRevolve();
  ~CArgsRhinoRevolve();


  /*
    Description:
      Set or get the angular domain for the revolve
    Parameters:
      domain [in] - new domain to set
    Returns:
      The current domain
  */
  ON_Interval Domain() const;
  void SetDomain(ON_Interval& domain);

  /*
    Description:
      Get or set the start or end angles of the domain
    Parameters:
      startangle, endangle [in] - angle to set
    Returns:
      Current start or end angle
    See Also:
      Domain(), SetDomain()
  */
  double StartAngle() const;
  void SetStartAngle(double startangle);
  double EndAngle() const;
  void SetEndAngle(double endangle);

  /*
    Description:
      Count, add or get profile curves for the revolve
    Parameters:
      pCrv [in] - pointer to a curve to add to the profile array
      index [in] - index in the profile array of the curve to return
    Returns:
      Pointer to the specified profile curve or NULL if the index is invalid
  */
  int ProfileCount();
  void AddProfile(ON_Curve* pCrv);
  ON_Curve* Profile(int index);
  // next 2 functions added Aug 27 2008 - LW
  void RemoveProfile(int i);
  void ReplaceProfile(int i, ON_Curve* pCrv);

  /*
    Description:
      Set or get the revolve axis
    Parameters:
      axis [in] - a line on the axis
    Returns:
      a line on the axis
  */
  void SetAxis(ON_Line& axis);
  ON_Line& Axis();

  /*
    Description:
      Set or get the deformable surface option settings for the revolved surface
    Parameters:
      count [in] - number of cv's to use in deformable surfaces
    Returns:
      @untitled table
      true     Make surface deformable
      false    Make surface exact
      The number of points for deformable surfaces
    Remarks:
      If the number of points is 0, its exact
  */
  bool IsDeformable();
  void SetDeformable(bool b = true);
  int DeformablePointCount();
  void SetDeformablePointCount(int count = 5);

  /*
    Description:
      Set or get whether or not input profiles should be deleted.
    Parameters:
      del [in] - true: delete input, false: don't delete
    Returns:
      @untitled table
      true     Delete input
      False    Don't delete input
  */
  void SetDeleteInput(bool del = true);
  bool DeleteInput() const;

  /*
    Description:
      Add, count, and retrieve pointers to the input curves
    Parameters:
      pCrv [in] - curve pointer to store
      index [in] - index of the curve to retrieve
    Returns:
      Pointer to the specified curve
      or Number of curves stored
  */
  void AddInputCurve(CRhinoObject* pCrv);
  int InputCount() const;
  CRhinoObject* InputCurve(int index) const;

  /*
    Description:
      Get or set if profiles should be trimmed to the revolve axis
    Parameters:
      b [in] - true: trim the profiles, false: don't trim the profiles
    Returns:
      @untitled table
      true     Do trim
      False    Don't trim
  */
  void SetTrimToAxis(bool b = true);
  bool TrimToAxis();

protected:
  ON_Interval m_domain;  // angular domain in radians
  ON_Line m_axis = ON_Line::UnsetLine;
  ON_SimpleArray<ON_Curve*> m_profiles;
  ON_SimpleArray<CRhinoObject*> m_input_curves;

  bool m_bTrimToAxis = true;
  bool m_bDeleteInput = false;
  bool m_bDeformable = false;
  int m_deformable_point_count = 4;

private:
  CArgsRhinoRevolve( const CArgsRhinoRevolve& src);
};


/*
  Description:
    Non-interactive Rotated Surface maker
    Make 1 Revolved Surface from 1 profile, axis, and angle domain
    Parameters:
      pProfile [in] - The curve to revolve
      axis [in] - Revolve around this line
      domain [in] - starting and ending angles for the revolve
    Returns:
      Pointer to the Revolved surface or NULL on error
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoRevolve(
  ON_Curve* pProfile,
  ON_Line& axis,
  ON_Interval& domain,
  bool bDeformable = false,
  int pointcount = 0
);


ON_DEPRECATED_MSG("Obsolete - do not use.")
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetRevolve(
  CArgsRhinoRevolve& args,
  ON_SimpleArray<ON_RevSurface*>& srfs_out
);


// class CArgsRhinoRevolve
//--------------------------------------------------------------------------------
class RHINO_SDK_CLASS CArgsRhinoRailRevolve
{
public:
  CArgsRhinoRailRevolve();
  ~CArgsRhinoRailRevolve();

  // C++ default copy constructor and operator equal work fine
  // CArgsRhinoRailRevolve( const CArgsRhinoRailRevolve& src);
  // CArgsRhinoRailRevolve& operator=( const CArgsRhinoRailRevolve& src);

  /*
    Description:
      Set or Get the prompt for profile curve
    Parameters:
      prompt [in] the new prompt
    Returns:
      The current prompt
  */
  const wchar_t* ProfilePrompt() const;
  void SetProfilePrompt( const wchar_t* prompt);

  /*
    Description:
      Set or Get the prompt for rail curve
    Parameters:
      prompt [in] the new prompt
    Returns:
      The current prompt
  */
  const wchar_t* RailPrompt() const;
  void SetRailPrompt( const wchar_t* prompt);

  /*
    Description:
      Set or Get the prompt for revolve axis
    Parameters:
      prompt [in] the new prompt
    Returns:
      The current prompt
  */
  const wchar_t* AxisStartPrompt() const;
  void SetAxisStartPrompt( const wchar_t* prompt);
  const wchar_t* AxisEndPrompt() const;
  void SetAxisEndPrompt( const wchar_t* prompt);

  /*
  Description:
    Set profile curve
  Parameters:
    profile_or [in] - profile_or.Curve() is used as the profile.
  */
  void SetProfileObject( const CRhinoObjRef& profile_or );

  /*
  Description:
    Set profile curve
  Parameters:
    profile_curve [in] - profile curve. 
  Remarks:
    ~CArgsRhinoRailRevolve does not delete this curve.
  */
  void SetProfileCurve( const ON_Curve* profile_curve );

  /*
  Description:
    Get profile curve
  Returns:
    profile_curve
  */
  const ON_Curve* ProfileCurve() const;

  /*
  Description:
    Get profile curve object
  Returns:
    profile_object.
  */
  const CRhinoObjRef& ProfileObject() const;

  /*
    Description:
      Set or get a pointer to the rail curve
    Parameters:
      rail_curve [in] - rail curve to save
    Returns:
      pointer to the rail curve
  */
  void SetRail( const ON_Curve* pCrv);

  void SetRailCurve( const ON_Curve* rail_curve );
  void SetRailObject( const CRhinoObjRef& rail_object );

  const ON_Curve* RailCurve() const;

  const CRhinoObjRef& RailObject() const;
  
  /*
    Description:
      Set or get the revolve axis
    Parameters:
      axis [in] - a line on the axis
    Returns:
      a line on the axis
  */
  void SetAxis( ON_Line axis);
  ON_Line Axis() const;

  /*
    Description:
      Set or get whether or not hight of input profiles should scaled.
    Parameters:
      scale [in] - true: scale profile, false: don't scale
    Returns:
      @untitled table
      true     Scale profile
      false    Don't scale profile
  */
  void SetScaleHeight( bool scale );
  bool ScaleHeight() const;

protected:
  ON_wString m_profile_prompt;
  ON_wString m_rail_prompt;
  ON_wString m_axis_start_prompt;
  ON_wString m_axis_end_prompt;
  
  CRhinoObjRef m_profile_object;
  CRhinoObjRef m_rail_object;
  ON_Line m_axis;
  const ON_Curve* m_profile_curve;
  const ON_Curve* m_rail_curve;

  bool m_bScaleHeight;
};


/*
Description:
  Interactively get input for RhinoRailRevolve.
Parameters:
  CArgsRhinoRailRevolve& args - [in/out]
Returns:
  CRhinoCommand::success if valid input was specified.
See Also:
  RhinoRailRevolve
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetRailRevolveInput( CArgsRhinoRailRevolve& args );



/*
Description:
  Create a rail revolve surface.
Parameters:
  profile_curve - [in]
  rail_curve - [in]
  axis - [in]
  bScaleHeight - [in]
  srf - [in]
Example:

          CArgsRhinoRailRevolve args;
          CRhinoCommand::result cmdrc = RhinoGetRailRevolveInput(args);
          if ( CRhinoCommand::success == cmdrc )
          {
            ON_NurbsSurface* srf = RhinoRailRevolve( 
                                      args.ProfileCurve(),
                                      args.RailCurve(),
                                      args.Axis(),
                                      args.ScaleHeight()
                                      NULL
                                      );
                                                     
          }

Returns:
  If successful, a pointer to the surface is returned.  If the input srf
  pointer was not NULL, then the returned point = srf.  If the input srf
  point was NULL, then the returned surface was allocated with
  ON_NurbsCurve::New().
See Also:
  RhinoGetRailRevolveInput
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoRailRevolve( 
          const ON_Curve* provile_curve, 
          const ON_Curve* rail_curve, 
          const ON_Line& axis,
          bool bScaleHeight,
          ON_NurbsSurface* srf
          );

