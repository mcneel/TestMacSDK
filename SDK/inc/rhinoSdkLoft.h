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

class RHINO_SDK_CLASS CRhinoLoftCurve
{
public:
  CRhinoLoftCurve();
  ~CRhinoLoftCurve();

  int CloserEnd();

  ON_Curve* m_curve;
  const ON_BrepTrim* m_trim;
  ON_3dPoint m_pick_point;
  ON_Plane m_plane;
  double m_pick_t;
  bool m_bClosed;
  bool m_bPlanar;
  bool m_bRev;
};

class CArgsRhinoLoftEx;

class RHINO_SDK_CLASS CArgsRhinoLoft
{
public:
  // 15 April 2015 - Removed ltDevelopable
  enum eLoftType
  {
    ltNormal      = 0,
    ltLoose       = 1,
    ltTight       = 2,
    ltStraight    = 3,
    ltUniform     = 4,
  };
  enum eLoftSimplify
  {
    lsNone     = 0,
    lsRebuild  = 1,
    lsRefit     = 2,
  };
  enum eLoftEnds
  {
    leTight    = 0,
    leStraight = 1,
    leLoose    = 2,
    leNatural  = 3,
    leTangent  = 4,
  };

  CArgsRhinoLoft();
  ~CArgsRhinoLoft();
  //CArgsRhinoLoft( const CArgsRhinoLoftEx& src);
  //CArgsRhinoLoft& operator=( const CArgsRhinoLoftEx& src);

  int CurveCount() const;

  ON_3dPoint StartPoint() const;
  void SetStartPoint( const ON_3dPoint& point);

  ON_3dPoint EndPoint() const;
  void SetEndPoint( const ON_3dPoint& point);

  BOOL32 UseStartPoint() const;
  BOOL32 UseEndPoint() const;

  BOOL32 MatchStartTangent() const;
  void SetMatchStartTangent( BOOL32 match = TRUE);

  BOOL32 MatchEndTangent() const;
  void SetMatchEndTangent( BOOL32 match = TRUE);

  int LoftType() const;
  void SetLoftType( int type);

  BOOL32 Closed() const;
  void SetClosed( BOOL32 closed = TRUE);

  int SimplifyMethod() const;
  void SetSimplifyMethod( int method);

  int RebuildPointCount() const;
  void SetRebuildPointCount( int count);

  double RefitTolerance() const;
  void SetRefitTolerance( double tol);

  double AngleToleranceRadians() const;
  void SetAngleToleranceRadians(double tol);


  ON_3dPoint m_startpoint;
  ON_3dPoint m_endpoint;
  BOOL32 m_bUseStartpoint;
  BOOL32 m_bUseEndpoint;
  BOOL32 m_bAllowStartTangent; // set if the start curve is an edge
  BOOL32 m_bAllowEndTangent;   // set if the end curve is an edge

  ON_SimpleArray<const CRhinoLoftCurve*> m_loftcurves;
  
  int m_loft_type;  // eLoftType
  BOOL32 m_bClosed;
  int m_simplify_method; // eLoftSimplify
  int m_rebuild_point_count;
  double m_refit_tolerance;
  double m_angle_tolerance_radians;
  int m_start_condition;
  int m_end_condition; // eLoftEnds
  ON_SimpleArray<bool> m_bRevCurve; // shape curves reversed from input direction
};

/// @cond NOT_IN_SDK

class /*RHINO_SDK_CLASS*/ CArgsRhinoLoftEx
{
public:
  enum class LoftType : unsigned int
  {
    Normal = 0,
    Loose = 1,
    Tight = 2,
    Straight = 3,
    Uniform = 4,
  };
  enum class LoftSimplify : unsigned int
  {
    None = 0,
    Rebuild = 1,
    Refit = 2,
  };
  enum class LoftEnds : unsigned int
  {
    Tight = 0,
    Straight = 1,
    Loose = 2,
    Natural = 3,
    Tangent = 4,
  };

  CArgsRhinoLoftEx() = default;
  ~CArgsRhinoLoftEx() = default;
  CArgsRhinoLoftEx(const class CArgsRhinoLoft& src);
  CArgsRhinoLoftEx& operator=(const class CArgsRhinoLoft& src);

  ON_3dPoint StartPoint() const;
  void SetStartPoint(const ON_3dPoint& point);

  ON_3dPoint EndPoint() const;
  void SetEndPoint(const ON_3dPoint& point);

  int UseStartPoint() const;
  void SetUseStartPoint(int use);

  int UseEndPoint() const;
  void SetUseEndPoint(int use);

  bool AllowStartTangent() const;
  void SetAllowStartTangent(bool allow);

  bool AllowEndTangent() const;
  void SetAllowEndTangent(bool allow);

  bool MatchStartTangent() const;
  void SetMatchStartTangent(bool match);

  bool MatchEndTangent() const;
  void SetMatchEndTangent(bool match);

  LoftType LoftType() const;
  void SetLoftType(enum LoftType type);

  LoftEnds StartCondition() const;
  void SetStartCondition(LoftEnds condition);

  LoftEnds EndCondition() const;
  void SetEndCondition(LoftEnds condition);

  bool Closed() const;
  void SetClosed(bool closed);

  LoftSimplify SimplifyMethod() const;
  void SetSimplifyMethod(LoftSimplify method);

  int RebuildPointCount() const;
  void SetRebuildPointCount(int count);

  double RefitTolerance() const;
  void SetRefitTolerance(double tol);

  double AngleToleranceRadians() const;
  void SetAngleToleranceRadians(double tol);

  bool SplitAtTangents();
  void SetSplitAtTangents(bool split);
   
private:
  ON_3dPoint m_startpoint = ON_unset_point;
  ON_3dPoint m_endpoint = ON_unset_point;
  int m_bUseStartpoint = 0;
  int m_bUseEndpoint = 0;
  bool m_bAllowStartTangent = false; // set if the start curve is an edge
  bool m_bAllowEndTangent = false;   // set if the end curve is an edge
  bool m_bUseStartTangent = false;
  bool m_bUseEndTangent = false;
  bool m_bClosed = false;
  unsigned char m_reserved1 = 0;
  unsigned short m_reserved2 = 0;
  enum LoftType m_loft_type = LoftType::Normal;
  enum LoftSimplify m_simplify_method = LoftSimplify::None;
  int m_rebuild_point_count = 10;
  int m_reserved3 = 0;
  double m_refit_tolerance = 0.001;
  double m_angle_tolerance_radians = ON_PI / 180.0;
  enum LoftEnds m_start_condition = LoftEnds::Natural;
  enum LoftEnds m_end_condition = LoftEnds::Natural;
  bool m_bSplitAtTangents = true;
  unsigned char m_reserved4 = 0;
  unsigned short m_reserved5 = 0;
  unsigned int m_reserved6 = 0;
  ON__UINT_PTR m_reserved7 = 0;
};

// Function to make a loft surface through curves.
//RHINO_SDK_FUNCTION
bool RhinoSdkLoftSurfaceEx(
  CArgsRhinoLoftEx& args,
  ON_SimpleArray<const CRhinoLoftCurve*>& input_curves,
  ON_SimpleArray<ON_NurbsSurface*>& srfs
);

//RHINO_SDK_FUNCTION
bool RhinoSdkLoftEx(
  CArgsRhinoLoftEx& args,
  ON_SimpleArray<const CRhinoLoftCurve*>& input_curves,
  ON_SimpleArray<ON_Geometry*>& lofts_out
  );

/// @endcond

//-------------------------
// Rhino SDK function to make a loft surface through curves.
RHINO_SDK_FUNCTION
bool RhinoSdkLoftSurface(
  CArgsRhinoLoft& args,
  ON_SimpleArray<ON_NurbsSurface*>& srfs,
  CRhinoHistory* history = nullptr
);

// Rhino SDK function to make a loft surface through curves.
// Results returned as Breps
RHINO_SDK_FUNCTION
bool RhinoSdkLoft(
  CArgsRhinoLoft& args,
  ON_SimpleArray<ON_Brep*>& breps,
  CRhinoHistory* history = nullptr
);

/*
Description:
  Creates a SubD surface lofted through shape curves.
Parameters:
  shape_curves [in] - Array of SubD-friendly NURBS curves to loft through
  bClosedLoft  [in] - Creates a SubD that is closed in the lofting direction. Must have three or more shape curves.
  bSubDCorners [in] - With open curves, adds creased vertices to the SubD at both ends of the first and last curves.
  bSubDCreases [in] - With kinked curves, adds creased edges to the SubD along the kinks.
  nDivisions   [in] - The segment number between adjacent input curves.
  result_subd  [in/out] - Pointer to an empty SubD to return the loft result.
Returns:
  true if successful, false otherwise.
Remarks:
  Curves must be in the proper order and orientation and have point counts for the desired surface.
  Curves must be either all open or all closed.
*/
RHINO_SDK_FUNCTION
bool RhinoSubDLoft(
  const ON_SimpleArray<const ON_NurbsCurve*>& shape_curves,
  bool bClosedLoft,
  bool bSubDCorners,
  bool bSubDCreases,
  int nDivisions,
  ON_SubD* result_subd
);
