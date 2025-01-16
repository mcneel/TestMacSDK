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

// RhinoSdkMatchCurve.h

#pragma once

// Set end geometry of a nurbs curve to match the specified  conditions
// position, tangency and curvature.
// 
// ON_NurbsCurve* curve - Curve to be modified in place
// bool bSetStart  - true: Set start of curve, false: set end of curve
// int continuity - what to set - 0: nothing, 1: position, 2: position & tangency, 3: position, tangency & curvature
// ON_3dPoint point - Point to set as curve endpoint
// ON_3dVector* tangent - Tangent for curve end if tangency is being set, or NULL
// ON_3dVector* curvature - Curvature for curve end if curvature is being set, or NULL
// returns bool true if sesuccessful, or false if not
//
// input curve is modified
// If the curve has too few cvs to set the requested condition, function will do nothing and return false
// Setting an end condition can move the other end of the curve if there are not enough cvs in the curcve
RHINO_SDK_FUNCTION
bool RhinoSetNurbsCurveEndCondition(
  ON_NurbsCurve* curve,
  int end,                   // end to change - 0: change start, 1: change end
  int continuity,            // geometry to be changed - 0: nothing, 1: position, 2: tangency, 3: curvature
  const ON_3dPoint& point,         // point to set
  const ON_3dVector& tangent,      // tangency to set
  const ON_3dVector& curvature     // curvature to set
  );

