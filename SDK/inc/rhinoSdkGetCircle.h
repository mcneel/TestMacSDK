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

#include "rhinoSdkGetPoint.h"
#include <functional>

// Input to the RhinoGetCircle()
class RHINO_SDK_CLASS CArgsRhinoGetCircle
{
public:
  // Results of main Circle Command function
  enum eCircleResult
  {
    nothing, // no result
    center,  // Center/size - m_centerpoint is valid
    twopt,   // 2point
    threept, // 3point
    ttr,     // TTR
    ttt,     // TTT
    fitpoints, // least squares fit points
    around,  // Around curve
    cancel,  // User canceled
    error,   // Something went wrong
  };

  enum
  {
    circle_type,
    cylinder_type,
    cone_type,
    sphere_type,
    torus_type,
    sphere_type_ex,
    cylinder_type_ex,
    arc_type,
    mesh_sphere_type,
    cone_type_ex
  };

  CArgsRhinoGetCircle();
  ~CArgsRhinoGetCircle();

  CArgsRhinoGetCircle( const CArgsRhinoGetCircle& );
  CArgsRhinoGetCircle& operator=( const CArgsRhinoGetCircle& );

  //////////
  // Set a custom prompt to use for getting the first (center) point
  void SetPrompt0( const wchar_t* );
  const ON_wString& Prompt0() const;

  //////////
  // Set a custom prompt to use for getting the second point
  void SetPrompt1( const wchar_t* );
  const ON_wString& Prompt1() const;

  //////////
  // Set a custom prompt to use for getting third point
  void SetPrompt2( const wchar_t* );
  const ON_wString& Prompt2() const;

  //////////
  // Set a custom prompt to use for getting the radius
  void SetRadiusPrompt( const wchar_t* );
  const ON_wString& RadiusPrompt() const;

  //////////
  // Set a custom prompt to use for getting the diameter
  void SetDiameterPrompt( const wchar_t* );
  const ON_wString& DiameterPrompt() const;

  //////////
  // Set the first point to use instead of asking for one
  void SetPoint0( const ON_3dPoint& );
  const ON_3dPoint& Point0() const;

  //////////
  // Set the second point to use instead of asking for one
  void SetPoint1( const ON_3dPoint& );
  const ON_3dPoint& Point1() const;

  //////////
  // Set the third point to use instead of asking for one
  void SetPoint2( const ON_3dPoint& point);
  const ON_3dPoint& Point2() const;

  //////////
  // Set a plane
  void SetPlane( const ON_Plane& );
  const ON_Plane& Plane() const;

  //////////
  // Set the default size to use - diameter or radius depending on m_bUseDiameter
  void SetDefaultSize( double );
  double DefaultSize() const;

  //////////
  // Use the default size when getting diameter/radius
  void SetUseDefaultSize( BOOL32 = TRUE);
  BOOL32 UseDefaultSize() const;

  //////////
  // Use the first point and don't ask for one
  void SetUsePoint0( BOOL32 = TRUE);
  BOOL32 UsePoint0() const;

  //////////
  // Use the second point and don't ask for one
  void SetUsePoint1( BOOL32 = TRUE);
  BOOL32 UsePoint1() const;

  //////////
  // Constrain the circle to a plane - Center/radius, 2point, 3point only
  void SetUsePlane( BOOL32 = TRUE);
  BOOL32 UsePlane() const;

  //////////
  // Use Diameter mode (instead of radius mode) for getting the size
  // Also qualifies the meaning of m_size
  void SetUseDiameterMode( BOOL32 = TRUE);
  BOOL32 UseDiameterMode() const;

  //////////
  // Make the circle vertical to the current cplane with the axis in the cplane
  //

  // Description:
  //   Returns the state of the vertical circle option.
  //   When the vertical circle option is selected, the
  //   circle is made vertical to the current cplane with
  //   the axis in the cplane.  By default the vertical
  //   circle option is available but is not selected.
  // Returns:
  //   TRUE if the vertical circle option is selected.
  // See Also:
  //   SetVerticalCircle, EnableVerticalCircleOption
  //   VerticalConeCyl, EnableVerticalConeCylOption
  BOOL32 VerticalCircle() const;

  // Description:
  //   Set the state of the vertical circle option.
  // Parameters:
  //   bSelected - [in] TRUE to select this option.
  // Remarks:
  //   Use EnableVerticalCircleOption to control if
  //   the "Vertical" applies to circles and
  //   appears at the initial point pick.
  // See Also:
  //   VerticalCircle, EnableVerticalCircleOption
  //   VerticalConeCyl, EnableVerticalConeCylOption
  void SetVerticalCircle( BOOL32 bSelected = TRUE);

  // Description:
  //   If the vertical circle option is available, then
  //   the "Vertical" option applies to circles and
  //   appears at the initial point pick.
  // Parameters:
  //   bAvailable - [in] TRUE to make the "Vertical" option
  //       available on the first pick and have it apply
  //       as a vertical circle option.
  // See Also:
  //   VerticalCircle, EnableVerticalCircleOption
  //   VerticalConeCyl, EnableVerticalConeCylOption
  void EnableVerticalCircleOption(BOOL32 bAvailable = TRUE);
  BOOL32 VerticalCircleOptionAvailable() const;

  // Description:
  //   Returns the state of the cone/cyl constraint option.
  //   When the cone/cyl option is selected, the
  //   circle is being made as a base for a cone/cyl.
  //   By default the vertical cone/cyl option not
  //   available but is not selected.  By default
  //   the "Vertical" option applies to VerticalCircle.
  // Returns:
  //   0 for None, 1 for Vertical and 2 for AroundCurve
  // See Also:
  //   SetVerticalConeCyl, EnableVerticalConeCylOption
  //   VerticalCircle, EnableVerticalCircleOption
  int ConeCylConstraint() const;

  // Description:
  //   Set the state of the vertical cone/cyl option.
  // Parameters:
  //   bSelected - [in] TRUE to select this option.
  // Remarks:
  //   Use EnableVerticalConeCylOption to control if
  //   the "Vertical" option applies to a cone/cyl
  //   base circle and appears at the initial point pick.
  // See Also:
  //   VerticalConeCyl, EnableVerticalConeCylOption
  //   VerticalCircle, EnableVerticalCircleOption
  void SetConeCylConstraint( int i = 0);

  // Description:
  //   If the vertical cone/cyl option is available, then
  //   the "Vertical" option applies to a cone/cyl base
  //   circle and appears at the initial point pick.
  // Parameters:
  //   bAvailable - [in] TRUE to make the "Vertical" option
  //       available on the first pick and have it apply
  //       as a cone/cyl option.
  // See Also:
  //   SetVerticalConeCyl, VerticalConeCyl
  //   VerticalCircle, EnableVerticalCircleOption
  void EnableConeCylOption(bool bAvailable = true);
  bool ConeCylOptionAvailable() const;

  // Description:
  //   Sets and gets a bool to determine whether
  //   solid primitive commands like Cylinder or Cone
  //   should "Cap" the output.
  // Parameters:
  //   bCap - [in] value of the cap bool
  void SetCap(bool bCap = true);
  bool GetCap() const;


  //////////
  // Use a fit curve instead or a rational ot primitive one
//  void SetDeformable( BOOL32 set = TRUE);
//  BOOL32 Deformable() const;


  // Description:
  //   If TRUE, then during the interactive part of
  //   RhinoGetCircle, a sphere is drawn.
  BOOL32 DynamicDrawSphere() const;

  // Description:
  //   Set DynamicDrawSphere state.
  // Parameters:
  //   bDrawSphere - [in] TRUE to draw a sphere instead of a circle
  void EnableDynamicDrawSphere( BOOL32 bDrawSphere = TRUE );

  // Description:
  //   Set/Get mode of circle creation.
  // Parameters:
  //   mode - [in] CArgsRhinoGetCircle::eCircleResult
  void SetCircleType(eCircleResult mode);
  eCircleResult GetCircleType();

  // Description:
  //   Set/Get state of allowing deformable options to be processed.
  // Parameters:
  //   b - [in] whether or not to display a Deformable prompt
  void SetAllowDeformable( BOOL32 b = TRUE);
  BOOL32 AllowDeformable() const;


  // Description:
  //   Set/Get deformable flag
  // Parameters:
  //   bDeformable - [in] whether or not circle is to be mad deformable
  void SetDeformable( BOOL32 bDeformable = TRUE);
  BOOL32 Deformable() const;

  // Description:
  //   Set/Get degree of nurbs curve used for deformable circle
  // Parameters:
  //   degree - [in] curve degree
  void SetDegree( int degree);
  int Degree() const;

  // Description:
  //   Set/Get number of CVs in nurbs curve used for deformable circle
  // Parameters:
  //   count - [in] CV count
  void SetPointCount( int count);
  int PointCount() const;

  /*
  Description:
  Set callback function for doing custom dynamic drawing
  Parameters:
  callback [in] Callback function for dynamic drawing
  Remarks:
  Good for custom drawing when calling ::RhinoGetSphere
  */
  void SetDynamicCircleCallback(std::function<void(const ON_Circle&)> callback);

  /*
  Description:
  Get callback function for doing custom dynamic drawing
  Returns:
  Callback function
  Remarks:
  Good for custom drawing when calling ::RhinoGetSphere
  */
  std::function<void(const ON_Circle&)> DynamicCircleCallback() const;

  //Interface to allow primitive mesh creation to use 
  //RhinoGetPlane, RhinoGetBox, RhinoGetCircle etc.
  void SetPromptForMeshDensity( bool bMeshDensity = true);
  bool PromptForMeshDensity() const;

  // prompt for number of faces in the vertical direction
  const CRhinoCommandOptionName& VerticalMeshDensityDirectionPrompt() const;
  void SetVerticalMeshDensityDirectionPrompt(const CRhinoCommandOptionName& prompt);
  // number of faces in the vertical direction
  int VerticalDirectionCount() const;
  void SetVerticalDirectionCount( int count);
  int VerticalDirectionMinCount() const;
  void SetVerticalDirectionMinCount( int count);

  // prompt for number of faces in the around direction
  const CRhinoCommandOptionName& AroundMeshDensityDirectionPrompt() const;
  void SetAroundMeshDensityDirectionPrompt(const CRhinoCommandOptionName& prompt);
  // number of faces in the around direction
  int AroundDirectionCount() const;
  void SetAroundDirectionCount( int count);
  int AroundDirectionMinCount() const;
  void SetAroundDirectionMinCount( int count);

  //return type of args class circle, cone, cylinder, sphere
  int ArgsType() const;

  // When true the linetype of the active layer is used for dynamic drawing
  void SetUseActiveLayerLinetype(bool on);
  bool UseActiveLayerLinetype() const;
protected:
  ON_wString  m_prompt0;           // [i]  Prompt for first or center point
  ON_wString  m_prompt1;           // [i]  Prompt for second point
  ON_wString  m_prompt2;           // [i]  Prompt for third point
  ON_wString  m_promptradius;      // [i]  Prompt when getting size in radius mode
  ON_wString  m_promptdiameter;    // [i]  Prompt when getting size in diameter mode
  ON_3dPoint  m_point0;            // [i]  if( m_bHavePoint0) first point specified here (center/radius, 2pt, 3pt only)
  ON_3dPoint  m_point1;            // [i]  if( m_bHavePoint1) second point specified here (3pt only)
  ON_3dPoint  m_point2;            // [i]  if( m_bHavePoint2) second point specified here (3pt only)
  ON_Plane    m_plane;             // [i]  if( m_bUsePlane) constrain to this plane
  double      m_defaultsize;       // [i]  if( m_bUseDefaultSize) radius or diameter (center/radius, ttr, AroundCurve only)

  BOOL32 m_bUsePoint0;               // [i]  Use first (center) instead of asking for point
  BOOL32 m_bUsePoint1;               // [i]  Use second instead of asking for point
  BOOL32 m_bUsePlane;                // [i]  Constrain the circle to a plane
  BOOL32 m_bVerticalCircleAvailable; // [i]  TRUE if "Vertical" option is available and applies to VerticalCircle
  BOOL32 m_bVerticalCircle;          // [io]  Make the circle axis parallel to the current construction plane
  bool m_bConeCylAvailable;        // [i]  TRUE if "Vertical" option is available and applies to VerticalConeCyl
  bool m_bCap = false;               // [io] Used when RhinoGetCircle is called by a primitive solid command like cylinder or cone
private:
  bool m_pen_usage = false; // change to unsigned char in the future if we need more modes
  bool m_bReserved2 = false;
protected:
  int m_ConeCylConstraint;          // [io]  0 = none, 1 = vertical, 2 = aroundcurve
  BOOL32 m_bUseDefaultSize;          // [i]  Defaultsize is valid
  BOOL32 m_bUseDiameterMode;         // [io] Get size in diameter mode instead of radius
  BOOL32 m_bDynamicDrawSphere;       // [i]
  eCircleResult m_result;         // [o]  What mode was used

  BOOL32 m_allow_deformable;         // display deformable prompt or not
  BOOL32 m_deformable;               // make circle deformable or not

  int m_degree;                    // deformable degree
  int m_point_count;               // deformable cv count

  std::function<void(const ON_Circle&)> m_dynamicCircleCallback;

  //Interface to allow primitive mesh creation to use 
  //RhinoGetCylinder, RhinoGetCone, etc.
  bool m_PromptForMeshDensity;
  CRhinoCommandOptionName m_mesh_density_vertical_direction_prompt;
  int m_mesh_density_VerticalDirection;
  int m_mesh_density_MinVerticalDirection;
  CRhinoCommandOptionName m_mesh_density_around_direction_prompt;
  int m_mesh_density_AroundDirection;
  int m_mesh_density_MinAroundDirection;

  //This variable is to distinguish whether this class is really
  //a CArgsRhinoGetCircle or the derived class CArgsRhinoGetCylinder, CArgsRhinoGetCone.  It
  //is set to plane_type in the constructor for the plane, which is called first,
  //and box_type in the constructor for the box.  ArgsType() returns the value of 
  //m_ArgsType
  int m_ArgsType;
};


////////////
// Global Utility function to interactively get a Circle
// Starts in Center/Radius mode.
// If an option is chosen for another construction method, switches to that method
// Used by the circle command
RHINO_SDK_FUNCTION
  CRhinoCommand::result RhinoGetCircle(
  CArgsRhinoGetCircle& args,
  ON_Circle& output_circle
  );

RHINO_SDK_FUNCTION
  CRhinoCommand::result RhinoGetCircleCenterRadius(
  CArgsRhinoGetCircle& args,
  ON_Circle& output_circle
  );

RHINO_SDK_FUNCTION
  CRhinoCommand::result RhinoGetCircle3Point(
  CArgsRhinoGetCircle& args,
  ON_Circle& output_circle
  );

RHINO_SDK_FUNCTION
  CRhinoCommand::result RhinoGetCircle2Point(
  CArgsRhinoGetCircle& args,
  ON_Circle& output_circle
  );

//RHINO_SDK_FUNCTION
//CRhinoCommand::result RhinoGetCircleTTR(
//    CArgsRhinoGetCircle& args,
//    ON_Circle& output_circle,
//    CRhinoHistory* history = NULL
//    );

RHINO_SDK_FUNCTION
  CRhinoCommand::result RhinoGetCircleTTT(
  CArgsRhinoGetCircle& args,
  ON_Circle& output_circle
  );

RHINO_SDK_FUNCTION
  CRhinoCommand::result RhinoGetCircleFitPoints(
  CArgsRhinoGetCircle& args,
  ON_Circle& output_circle
  );


/*
Description:
  Calculates a circle that is tangent to 3 curves.
Parameters:
  c1     [in]  - The first curve.
  c2     [in]  - The second curve.
  c3     [in]  - The third curve.
  seed1  [in]  - A seed parameter on the first curve.
  seed2  [in]  - A seed parameter on the second curve.
  seed3  [in]  - A seed parameter on the third curve.
  result [out] - The results of the calculation.
Return:
  true or false indicating success or failure.
*/
RHINO_SDK_FUNCTION
  bool RhinoCalculateCircleTanTanTan(
  const ON_Curve* c1,
  const ON_Curve* c2,
  const ON_Curve* c3,
  double seed1,
  double seed2,
  double seed3,
  ON_Circle& result
  );

/*
Description:
  Calculates a circle that is tangent to 2 curves.
Parameters:
  c1     [in]  - The first curve.
  c2     [in]  - The second curve.
  seed1  [in]  - A seed parameter on the first curve.
  seed2  [in]  - A seed parameter on the second curve.
  result [out] - The results of the calculation.
Return:
  true or false indicating success or failure.
*/
RHINO_SDK_FUNCTION
  bool RhinoCalculateCircleTanTan(
  const ON_Curve* c1,
  const ON_Curve* c2,
  double seed1,
  double seed2,
  ON_Circle& result
  );
