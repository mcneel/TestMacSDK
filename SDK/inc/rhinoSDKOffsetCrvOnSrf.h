/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

//Description: 
//  Offset a curve on a surface.  
//  The source curve must lie on the surface.
//Parameters:
//  p3dCurve      [in]  - the curve to offset
//  pBrep         [in]  - brep on which to offset the curve
//  fid           [in]  - index of the face in the brep
//  distance      [in]  - distance to offset +: left, -: right
//  tol           [in]  - fitting tolerance
//  result_curves [out] - results of the offset
//
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoOffsetCurveOnSrf( const ON_Curve* p3dCrv, 
                                             const ON_Brep* pBrep, 
                                             int fid, 
                                             double distance, 
                                             double tol, 
                                             ON_SimpleArray<ON_Curve*>& result_curves);

//Description: 
//  Offset a curve on a surface.  
//  The source curve must lie on the surface.
//Parameters:
//  p3dCurve      [in]  - the curve to offset
//  pBrep         [in]  - brep on which to offset the curve
//  fid           [in]  - index of the face in the brep
//  through_point [in]  - 2d point on the brep face to offset through
//  tol           [in]  - fitting tolerance
//  result_curves [out] - results of the offset
//
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoOffsetCurveOnSrf( const ON_Curve* p3dCrv, 
                                             const ON_Brep* pBrep, 
                                             int fid, 
                                             ON_2dPoint through_point, 
                                             double tol, 
                                             ON_SimpleArray<ON_Curve*>& result_curves);
//Description: 
//  Offset a curve on a surface.  
//  The source curve must lie on the surface.
//Parameters:
//  p3dCurve      [in]  - the curve to offset
//  pBrep         [in]  - brep on which to offset the curve
//  fid           [in]  - index of the face in the brep
//  params        [in]  - curve parameters corresponding to the offset distances
//  dists         [in]  - distances to offset  +: left, -: right
//  tol           [in]  - fitting tolerance
//  result_curves [out] - results of the offset
//
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoOffsetCurveOnSrf( const ON_Curve* p3dCrv, 
                                             const ON_Brep* pBrep, 
                                             int fid, 
                                             ON_SimpleArray<double>& params,
                                             ON_SimpleArray<double>& dists,
                                             double tol, 
                                             ON_SimpleArray<ON_Curve*>& result_curves);

RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoOffsetCurveOnSrfOld(
  const ON_Curve* p3dCrv,
  const ON_Brep* pBrep,
  int fid,
  ON_SimpleArray<double>& params,
  ON_SimpleArray<double>& dists,
  double tol,
  ON_SimpleArray<ON_Curve*>& result_curves
);
