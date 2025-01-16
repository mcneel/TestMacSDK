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

class RHINO_SDK_CLASS CArgsRhinoSweep1
{
public:
  enum eSweep1Style
  {
    ess_Freeform = 0,  
    ess_Roadlike = 1,
    ess_SrfEdge = 2,
    ess_EndpointPivot = 3,
  };

  enum eSweep1Simplify
  {
    ess_Nothing = 0,  // Use the shapes as is
    ess_Rebuild = 1,  // Rebuild interpolating equally spaced points
    ess_Refit = 2,    // Refit to a tolerance
  };

  CArgsRhinoSweep1();
  ~CArgsRhinoSweep1();

  bool TestForSimpleSweep(); // See if inputs are perfect enough to do greville interpolate 

  
  // Test function used for RailRevolve sweeps
  bool AllShapesEndAtPoint( ON_3dPoint* pt = NULL);

  bool m_bSimpleSweep;       // Make a simple surface by greville interpolation of shapes

  bool m_bHaveRailPickPoint;      // true if the rail was selected with a point pick
  ON_3dPoint m_RailPickPoint;     // point where the rail was picked
  const CRhinoPolyEdge* m_rail_curve;       // input rail curve
  ON_SimpleArray<ON_Curve*> m_shape_curves; // array of input shapes
  ON_SimpleArray<double> m_rail_params;     // params on the rail that correspond to each shape
  ON_3dPoint m_points[2];         // start and end points
  int m_bUsePoints[2];            // flag for valid start and/or endpoint to use

  ON_ClassArray<CRhinoObjRef> m_shape_objrefs; // Where the shapes came from
  CRhinoObjRef m_point_objrefs[2];              // point objects picked for endpoints

  BOOL32 m_bClosed;                 // Open or closed sweep if the rail is closed
  int m_style;                    // how the frames get rotated around the rail
  ON_3dVector m_planar_up;        // Up direction for roadlike style
  int m_simplify;                 // Simplify method for shape curves
  int m_rebuild_count;            // Sample point count for rebuilding shapes
  double m_refit_tolerance;       // tolerance to refit shape curves
  double m_sweep_tolerance;       // tolerance to pass to TL sweeper as SweepTolerance
  double m_angle_tolerance;       // tolerance to pass to TL sweeper as AngleTolerance

  int m_miter_type;               // 0: don't miter
                                  // 1: intersect surfaces and trim sweeps
                                  // 2: rotate shapes at kinks and don't trim
  int m_miter_attempts;           // number of kink miters attempted
  int m_miter_count;              // number of kink miters successfully complered

  bool m_bUsePivotPoint;
  ON_3dPoint m_pivot_point;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};



class RHINO_SDK_CLASS CArgsRhinoSweep2
{
public:
  enum eSweep2Simplify
  {
    ess_Nothing = 0,  // Use the shapes as is
    ess_Rebuild = 1,  // Rebuild interpolating equally spaced points
    ess_Refit = 2,    // Refit to a tolerance
  };

  CArgsRhinoSweep2();
  ~CArgsRhinoSweep2();
  bool TestForSimpleSweep(); // See if inputs are perfect enough to do greville interpolate 

  const CRhinoPolyEdge* m_rail_curves[2];         // input rail curves
  ON_3dPoint m_rail_pick_points[2];               // first pick points for rails or ON_UNSET_POINT if not point picked
  ON_SimpleArray<const ON_Curve*> m_shape_curves; // array of input shapes
  ON_SimpleArray<ON_3dPoint> m_points;            // start and end points
  ON_SimpleArray<double> m_rail_params[2];        // params on the rail that correspond to each shape
                                                  // these don't include start or end points or invented shapes

  BOOL32 m_bSimpleSweep;       // Make a simple surface by greville interpolation of shapes
  BOOL32 m_bClosed;            // Open or closed sweep if both rails are closed
  int m_simplify;            // Simplify method for shape curves
  int m_rebuild_count;       // Sample point count for rebuilding shapes
  double m_refit_tolerance;  // tolerance to refit shape curves
  double m_sweep_tolerance;  // tolerance to pass to TL sweeper as SweepTolerance
  double m_angle_tolerance;  // tolerance to pass to TL sweeper as AngleTolerance

  BOOL32 m_bSameHeight;        // maintain height between shapes - don't scale it the same as width
  void* m_pUser;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Public Sweep functions 
// Results are returned in brep_array

RHINO_SDK_FUNCTION
BOOL32 RhinoSweep1( CArgsRhinoSweep1& args, ON_SimpleArray<ON_Brep*>& brep_array);

RHINO_SDK_FUNCTION
BOOL32 RhinoSweep2( CArgsRhinoSweep2& args, ON_SimpleArray<ON_Brep*>& brep_array);

/*
Description:
  Creates a surface through profile curves that define the surface shape and one curve
  that defines a surface edge.
Parameters:
  args           - [in] The sweep1 arguments
  shape_blending - [in] Shape blending, where:
                        Local (shape_blending = 0), the sweep stays constant at the 
                          ends and changes more rapidly in the middle.
                        Global (shape_blending = 1), the sweep is linearly blended from
                          one end to the other, creating sweeps that taper from 
                          one cross-section curve to the other.
  brep_array     - [out] The results of the sweep.
                         NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE OBJECTS.
Returns:
  TRUE if successful
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoSweep1Ex( CArgsRhinoSweep1& args, int shape_blending, ON_SimpleArray<ON_Brep*>& brep_array );


/*
Description:
  Sweep1 function that fits a surface through profile curves that define the surface cross-sections
  and one curve that defines a surface edge.
*/
RHINO_SDK_FUNCTION
bool Rhino1RailSweep(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail,                         // rail going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,  // shapes will be sorted on rail
  bool bClosed = false,                          // only matters if rail is closed
  double tolerance = 0.01                        // tolerance for fitting surface and rails
);

/*
Description:
  Sweep1 function that fits a surface through profile curves that define the surface cross-sections
  and one curve that defines a surface edge.
*/
RHINO_SDK_FUNCTION
bool Rhino1RailSweep(
  ON_SimpleArray<ON_Brep*>& output_surfaces,              // Sweep results
  const ON_Curve* pRailCurve,                             // Rail going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,           // Shapes will be sorted on rail
  ON_3dPoint start_point,                                 // The start point, specify ON_3dPoint::UnsetPoint to ignore.
  ON_3dPoint end_point,                                   // The ending point, specify ON_3dPoint::UnsetPoint to ignore.
  int frame_type = 0,                                     // The frame type: 0 = freeform, 1 = roadlike
  ON_3dVector roadlike_normal = ON_3dVector::UnsetVector, // The roadlike normal direction, or ON_3dVector::UnsetVector to ignore.
  bool bClosed = false,                                   // Only matters if rails are closed
  int blend_type = 0,                                     // The blend type: 0 = local, 1 = global.
  int miter_type = 0,                                     // The miter type: 0 = none, 1 = trimmed, 2 = untrimmed miters
  double tolerance = 0.01,                                // tolerance for fitting surface and rails
  int rebuild_mode = 0,                                   // 0 = no rebuild, 1 = rebuild through points, 2 = refit to tolerance
  int rebuild_points = 0,                                 // if 1 == rebuild_mode, the number of points, otherwise set to 0
  double refit_tolerance = 0.0                            // if 2 == rebuild_mode, the refit tolerance, otherwise set to 0.0
);

/*
Description:
  Sweep1 function that fits a surface through profile curves that define the surface cross-sections
  and one curve that defines a surface edge. The Segmented version breaks the rail at curvature kinks
  and sweeps each piece separately, then put the results together into a Brep.
*/
RHINO_SDK_FUNCTION
bool Rhino1RailSweepSegmented(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail,                         // rail going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,  // shapes will be sorted on rail
  bool bClosed = false,                          // only matters if rail is closed
  double tolerance = 0.01                        // tolerance for fitting surface and rails
);

/*
Description:
  Sweep1 function that fits a surface through profile curves that define the surface cross-sections
  and one curve that defines a surface edge. The Segmented version breaks the rail at curvature kinks
  and sweeps each piece separately, then put the results together into a Brep.
*/
RHINO_SDK_FUNCTION
bool Rh1RailSweepSegmented(
  ON_SimpleArray<ON_Brep*>& output_surfaces,              // Sweep results
  const ON_Curve* pRailCurve,                             // Rail going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,           // Shapes will be sorted on rail
  ON_3dPoint start_point,                                 // The start point, specify ON_3dPoint::UnsetPoint to ignore.
  ON_3dPoint end_point,                                   // The ending point, specify ON_3dPoint::UnsetPoint to ignore.
  int frame_type = 0,                                     // The frame type: 0 = freeform, 1 = roadlike
  ON_3dVector roadlike_normal = ON_3dVector::UnsetVector, // The roadlike normal direction, or ON_3dVector::UnsetVector to ignore.
  bool bClosed = false,                                   // Only matters if rails are closed
  int blend_type = 0,                                     // The blend type: 0 = local, 1 = global.
  int miter_type = 0,                                     // The miter type: 0 = none, 1 = trimmed, 2 = untrimmed miters
  double tolerance = 0.01,                                // tolerance for fitting surface and rails
  int rebuild_mode = 0,                                   // 0 = no rebuild, 1 = rebuild through points, 2 = refit to tolerance
  int rebuild_points = 0,                                 // if 1 == rebuild_mode, the number of points, otherwise set to 0
  double refit_tolerance = 0.0                            // if 2 == rebuild_mode, the refit tolerance, otherwise set to 0.0
);

/*
Description:
  Sweep2 function that fits a surface through profile curves that define the surface cross-sections
  and two curves that defines the surface edges.
*/
RHINO_SDK_FUNCTION
bool Rhino2RailSweep(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail[2],                      // 2 rails going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,  // shapes will be sorted on rail
  bool bClosed = false,                          // only matters if rails are closed
  double tolerance = 0.01                        // tolerance for fitting surface and rails
);

/*
Description:
  Sweep2 function that fits a surface through profile curves that define the surface cross-sections
  and two curves that defines the surface edges.
  Use this version if you want to create shapes like Rhino's Sweep2 commaned.
*/
RHINO_SDK_FUNCTION
bool Rhino2RailSweep(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail[2],                      // 2 rails going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,  // shapes will be sorted on rail
  ON_3dPoint start_point,                        // start point, specify ON_UNSET_POINT to ignore
  ON_3dPoint end_point,                          // end point, specify ON_UNSET_POINT to ignore
  bool bClosed = false,                          // only matters if rails are closed
  double tolerance = 0.01,                       // tolerance for fitting surface and rails
  int rebuild_mode = 0,                          // 0 = no rebuild, 1 = rebuild through points, 2 = refit to tolerance
  int rebuild_points = 0,                        // if 1 == rebuild_mode, the number of points, otherwise set to 0
  double refit_tolerance = 0.0,                  // if 2 == rebuild_mode, the refit tolerance, otherwise set to 0.0
  bool bPreserveHeight = false                   // removes the association between the height scaling from the width scaling
);

/*
Description:
  Sweep2 function that fits a surface through profile curves that define the surface cross-sections
  and two curves that defines the surface edges.
  Use this version if you want to create shapes like earlier version of Rhino.
*/
RHINO_SDK_FUNCTION
bool Rhino2RailSweepOld(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail[2],                      // 2 rails going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,  // shapes will be sorted on rail
  ON_3dPoint start_point,                        // start point, specify ON_UNSET_POINT to ignore
  ON_3dPoint end_point,                          // end point, specify ON_UNSET_POINT to ignore
  bool bClosed = false,                          // only matters if rails are closed
  double tolerance = 0.01,                       // tolerance for fitting surface and rails
  int rebuild_mode = 0,                          // 0 = no rebuild, 1 = rebuild through points, 2 = refit to tolerance
  int rebuild_points = 0,                        // if 1 == rebuild_mode, the number of points, otherwise set to 0
  double refit_tolerance = 0.0,                  // if 2 == rebuild_mode, the refit tolerance, otherwise set to 0.0
  bool bPreserveHeight = false                   // removes the association between the height scaling from the width scaling
);

/*
Description:
  Sweep2 function that fits a surface through profile curves that define the surface cross-sections
  and two curves that defines the surface edges.
*/
RHINO_SDK_FUNCTION
bool Rhino2RailSweep(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail[2],                      // 2 rails going the right direction
  const ON_SimpleArray<const ON_Curve*> shapes,  // shapes will be sorted on rail
  int rebuild_mode,                              // 0 = no rebuild, 1 = refit to tolerance, 2 = rebuild to #points
  int rebuild_points,                            // if 2 == rebuild_mode, the number of points
  double refit_tolerance,                        // if 2 == rebuild_mode, the refit tolerance
  bool bPreserveHeight,                          // removes the association between the height scaling from the width scaling
  bool bClosed = false,                          // only matters if rails are closed
  double tolerance = 0.01);

/*
Description:
  Sweep2 function that fits a surface through profile curves that define the surface cross-sections
  and two curves that defines the surface edges. Result are split where parameterization along
  a rail changes abruptly.
*/
RHINO_SDK_FUNCTION
bool Rhino2RailSweepInParts(
  ON_SimpleArray<ON_Brep*>& output_surfaces,     // sweep results
  const ON_Curve* pRail[2],                      // 2 rails going the right direction
  const ON_SimpleArray< const ON_Curve* >& shapes,
  bool bClosed = false,
  double tolerance = 0.01);


/*
Description:
  Fits a SubD surface through a series of profile curves that define the SubD cross-sections and one curve that defines a SubD edge.
Parameters:
  rail_curve   [in] - A SubD-friendly NURBS curve to sweep along.
  shape_curves [in] - Array of SubD-friendly NURBS curves to sweep through.
  bSubDCorners [in] - With open curves, adds creased vertices to the SubD at both ends of the first and last curves.
  bClosedSweep [in] - Creates a SubD that is closed in the rail curve direction.
  bFrameStyle  [in] - Determines how sweep frame rotations are calculated:
                      true: Freeform - Frames are propagated based on a reference direction taken from the rail curve curvature direction.
                      false: Roadlike - Frame rotations are calculated based on a vector supplied in "frameup" and the world coordinate system.
  frame_up     [in] - A 3D vector used to calculate the frame rotations for sweep shapes (not used if bFrameStyle is true).
  result_subd  [in/out] - Pointer to an empty SubD to return the sweep result.
Returns:
  true if successful, false otherwise.
Remarks:
  Shape curves must be in the proper order and orientation.
  Shape curves must have the same point counts and rail curves must have the same point counts.
  Shape curves will relocated to the nearest pair of Greville points on the rails.
  Shape curves will be made at each pair of rail edit points where there isn't an input shape.
*/
RHINO_SDK_FUNCTION
bool RhinoSubDSweep1(
  const ON_NurbsCurve* rail_curve,
  const ON_SimpleArray<const ON_NurbsCurve*>& shape_curves,
  bool bSubDCorners,
  bool bClosedSweep,
  bool bFrameStyle,
  ON_3dVector frame_up,
  ON_SubD* result_subd
);

/*
Description:
  Fits a SubD surface through a series of profile curves that define the SubD cross-sections and two curves that defines SubD edges.
Parameters:
  rail_curves  [in] - Array of 2 SubD-friendly NURBS curve to sweep along.
  shape_curves [in] - Array of SubD-friendly NURBS curves to sweep through.
  bSubDCorners [in] - With open curves, adds creased vertices to the SubD at both ends of the first and last curves.
  bClosedSweep [in] - Creates a SubD that is closed, continuing past the last curve around to the first curve.
  result_subd  [in/out] - Pointer to an empty SubD to return the sweep result.
Returns:
  true if successful, false otherwise.
Remarks:
  Shape curves must be in the proper order and orientation.
  Shape curves must have the same point counts and rail curves must have the same point counts.
  Shape curves will relocated to the nearest pair of Greville points on the rails.
  Shape curves will be made at each pair of rail edit points where there isn't an input shape.
*/
RHINO_SDK_FUNCTION
bool RhinoSubDSweep2(
  const ON_SimpleArray<const ON_NurbsCurve*>& rail_curves,
  const ON_SimpleArray<const ON_NurbsCurve*>& shape_curves,
  bool bSubDCorners,
  bool bClosedSweep,
  ON_SubD* result_subd
);
