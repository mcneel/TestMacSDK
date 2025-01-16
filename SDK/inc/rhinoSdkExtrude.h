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

/*
  Description
    Extrude a curve along a vector direction
  Parameters:
    [in] pCurve - curve to extrude
    [in] direction - the extrude vector
    [in] scale - optional scalar for the extrude vector
  Returns:
    The extruded surface if successful
    NULL if not
  Remarks:
    The direction vector is used for both distance and direction input
    The direction vector length is multiplied by scale
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoExtrudeCurveStraight( 
                      const ON_Curve* pCurve, 
                      ON_3dVector direction, 
                      double scale = 1.0);



/*
  Description
    Extrude a curve to a point making a coned surface
  Parameters:
    [in] pCurve - the curve to extrude
    [in] apex - the point for the tip of the surface
  Returns:
    The extruded surface if successful
    NULL if not
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoExtrudeCurveToPoint( 
                      const ON_Curve* pCurve, 
                      const ON_3dPoint& apex);


/*
  Description
    Extrude a curve to a taper making a brep (potentially more than 1)
  Parameters:
    [in] input_curve - the curve to extrude
    [in] distance - the distance to extrude
    [in] direction - the direction of the extrusion
    [in] base - the basepoint of the extrusion
    [in] draft_angle - angle of the extrusion
    [in] cornertype - corner type of the extrusion. valid options are no_corner = 0, sharp = 1, round = 2, smooth = 3, and chamfer = 4
    [in] tol - tolerance to be used in calculation, if you don't have a tolerance in mind consider using the absolute tolerance from the document
    [in] ang_tol - angle tolerance to be used in calculation, if you don't have a tolerance in mind consider using the absolute tolerance from the document
    [out] output_breps - array of breps
  Returns:
    True if successful, false if not.
*/
RHINO_SDK_FUNCTION
bool RhinoCreateTaperedExtrude( const ON_Curve* input_curve, 
                                double distance,
                                ON_3dVector direction,
                                ON_3dPoint base,
                                double draft_angle,
                                int cornertype,
                                double tol,
                                double ang_tol,
                                ON_SimpleArray<ON_Brep*>& output_breps );

