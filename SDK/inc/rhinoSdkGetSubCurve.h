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
  CArgsRhinoGetSubCurve

OVERVIEW TEXT
  Arguments for RhinoGetSubCurve

KEYWORDS
  RhinoGetSubCurve SubCrv SubCurve
-------------------------------------------------------------------*/
class RHINO_SDK_CLASS CArgsRhinoGetSubCurve
{
public:
  CArgsRhinoGetSubCurve();
  ~CArgsRhinoGetSubCurve();

  CArgsRhinoGetSubCurve(const CArgsRhinoGetSubCurve&);
  CArgsRhinoGetSubCurve& operator=(const CArgsRhinoGetSubCurve&);

  //sets
  void SetFirstPrompt( const wchar_t* );
  void SetSecondPrompt( const wchar_t* );
  void SetCurve( const ON_Curve* pC);
  void SetIncludeRange( const ON_Interval& range);
  void ClearIncludeRange();
  void SetReverse( bool bRev);

  // gets
  const ON_wString& FirstPrompt() const;
  const ON_wString& SecondPrompt() const;
  const ON_Curve* Curve() const;
  bool IncludeRange( ON_Interval& range) const;
  bool IncludeRangeValid() const;
  bool Reverse() const;


protected:
  ON_wString  m_FirstPrompt;  // prompt for first point
  ON_wString  m_SecondPrompt; // prompt for following points

  const ON_Curve* m_pC;
  ON_Interval m_include_range;
  bool m_bReverse;   // true if drag direction was opposite natural curve direction
};

CRhinoCommand::result RhinoGetSubCurve(
  CArgsRhinoGetSubCurve& args,  // [in/out] 
  ON_Curve** pC_Out  // [out] curve returned here
  );

CRhinoCommand::result GetSubCurveRange( 
  CArgsRhinoGetSubCurve& args,   // [in/out] 
  ON_Interval& range,            // [out] - parameter range on args.curve
  bool bAdjustOrder = true       // allow ordering of parameters to put smaller one first
                                 // if this is false, return picks in the order they are made
  );

//////////
// GetPoint derived class
// This interactively gets the second subcurve point
class CRhGetSubCurvePoint: public CRhinoGetPoint
{
public:
  CRhGetSubCurvePoint( CArgsRhinoGetSubCurve& );
  ~CRhGetSubCurvePoint();

  bool GetLengthParameters(const double l, const double t0, const int direction, double& t_start, double& t_end);
  void OnMouseMove( CRhinoViewport&, UINT nFlags, const ON_3dPoint&, const ON_2iPoint* ) override;
  void DynamicDraw( CRhinoDisplayPipeline& dp, const ON_3dPoint& ) override;

  ON_Curve* m_pC; // Working copy of the curve
  CArgsRhinoGetSubCurve& m_args; // Operating parameters, settings and defaults
  double m_t0, m_t1;
  ON_3dPoint m_pt0;

  BOOL32 m_bHaveCurve; // Curve is calculated and can be drawn
  BOOL32 m_bLastWasLong;
  BOOL32 m_bReverse;
  BOOL32 m_bIsClosed;

private:
  CRhGetSubCurvePoint();
};

