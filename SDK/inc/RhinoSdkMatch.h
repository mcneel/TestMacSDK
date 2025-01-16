// Copyright (c) 1993-2024 Robert McNeel & Associates. All rights reserved.
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

// CArgsRhinoMatch is obsolete
class ON_DEPRECATED RHINO_SDK_CLASS CArgsRhinoMatch
{
public:
  enum eMatchMode
  {
    smPosition = 0,
    smTangency = 1,
    smCurvature = 2,
  };
  enum eMatchJoin
  {
    mjNone = 0,
    mjJoin = 1,
    mjMerge = 2,
  };

  CArgsRhinoMatch();
  ~CArgsRhinoMatch();

  // No operator=  -- curve pointers copied in assignment

  eMatchJoin m_join;              // none, join, merge
  bool m_bAverage;                // change both curves by 1/2 each
  int  m_preserve;                // maintain shape of the edge opposite the one being edited
                                  // 0 : nothing
                                  // 1 : position
                                  // 2 : tangency
                                  // 3 : curvature
                                      
  eMatchMode m_continuity;        // what geometry characteristics to match
                                  // 0:position, 1:tangency, 2:curvature
  ON_NurbsCurve* m_curvesin[2];   // Nurbs form duplicates of the input curves
  ON_NurbsCurve* m_curvesout[2];  // Result curves after the match
  const ON_BrepTrim* m_trim;      // A brep trim for matching to a surface edge
  double m_target_param;          // parameter on the curve to match
  bool m_bPerpToEdge;             // Match curve perpendicular to surface edge
  bool m_bOneCurve;               // true if both ends of one curve are being matched
  bool m_bMatchPickPoint;         // Matching to a point on a curve (m_curvesin[1]) at m_trim_param
  bool m_bFlipDirection;          // Match opposite target direction
};

