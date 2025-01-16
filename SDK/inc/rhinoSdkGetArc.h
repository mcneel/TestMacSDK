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


/*-------------------------------------------------------------------
CLASS
  CArgsRhinoGetArc

OVERVIEW TEXT
  Arguments for GetArc... functions

KEYWORDS
  Arc GetArc
-------------------------------------------------------------------*/
class RHINO_SDK_CLASS CArgsRhinoGetArc
{
public:
  
  //////////
  // current arc mode of GetArc functions
  enum eArcMode
  {
    center_start_end,
    center_start_angle,
    start_mid_end,
    start_end_mid,
    start_end_dir,
    start_dir_end,
    start_end_radius,
    tan_tan_tan,
    curve_end,
    center_mid_end,
    center_mid_angle
  };

  CArgsRhinoGetArc();
  ~CArgsRhinoGetArc();

  CArgsRhinoGetArc(const CArgsRhinoGetArc&);
  CArgsRhinoGetArc& operator=(const CArgsRhinoGetArc&);

  void SetStartPrompt( const wchar_t* );
  const ON_wString& StartPrompt() const;

  void SetEndPrompt( const wchar_t* );
  const ON_wString& EndPrompt() const;

  void SetEndAnglePrompt( const wchar_t* );
  const ON_wString& EndAnglePrompt() const;

  void SetCenterPrompt( const wchar_t* );
  const ON_wString& CenterPrompt() const;

  void SetPointOnArcPrompt( const wchar_t* );
  const ON_wString& PointOnArcPrompt() const;

  void SetDirectionPrompt( const wchar_t* );
  const ON_wString& DirectionPrompt() const;

  void SetRadiusPrompt( const wchar_t* );
  const ON_wString& RadiusPrompt() const;

  //////////
  // Set the first point to use instead of asking for one
  void SetPoint1( const ON_3dPoint& );
  const ON_3dPoint& Point1() const;
  BOOL32 UsePoint1() const; 

  //////////
  // Set the second point to use instead of asking for one
  void SetPoint2( const ON_3dPoint& );
  const ON_3dPoint& Point2() const;
  BOOL32 UsePoint2() const; 

  //////////
  // Set the third point to use instead of asking for one
  void SetPoint3( const ON_3dPoint& );
  const ON_3dPoint& Point3() const;
  BOOL32 UsePoint3() const; 
  void SetUsePoint3( BOOL32 = TRUE);

  //////////
  // Set the arc mode
  void SetArcMode( eArcMode = start_end_mid);
  eArcMode ArcMode() const; 

  ///////
  // Set whether the last prompt accepts a number or not. Used in center_start_end
  void SetAcceptNumber( BOOL32 = TRUE);
  BOOL32 AcceptNumber() const; 

  ////////
  // The current third point quadrant in center_start_end
  void SetQuadrant( int);
  int Quadrant() const; 

  ///////
  // The current arc direction in center_start_end
  void SetDir( int);
  int Dir() const; 

  ///////
  // The current arc normal in center_start_end
  void SetNormal( const ON_3dVector&);
  const ON_3dVector& Normal() const; 

  ///////
  // The current arc angle in center_start_angle
  void SetAngle( double);
  const double Angle() const; 

  ///////
  // The default radius in start_end_radius
  void SetDefaultRadius( double);
  const double DefaultRadius() const; 

  void SetAllowDeformable( BOOL32 b = TRUE);
  BOOL32 AllowDeformable() const;
  void SetDeformable( BOOL32 bDeformable = TRUE);
  BOOL32 Deformable() const;
  void SetDegree( int degree);
  int Degree() const;
  void SetPointCount( int count);
  int PointCount() const;

  // When true the linetype of the active layer is used for dynamic drawing
  void SetUseActiveLayerLinetype(bool on);
  bool UseActiveLayerLinetype() const;
protected:
  ON_wString m_sStartPrompt;
  ON_wString m_sEndPrompt;
  ON_wString m_sEndAnglePrompt;
  ON_wString m_sCenterPrompt;
  ON_wString m_sPointOnArcPrompt;
  ON_wString m_sDirectionPrompt;
  ON_wString m_sRadiusPrompt;

  ON_3dPoint  m_point1;            // [i] if m_bHavePoint1 first point specified here
  ON_3dPoint  m_point2;            // [i] if m_bHavePoint2 second point specified here
  ON_3dPoint  m_point3;            // [i] if m_bHavePoint3 third point specified here
  
  BOOL32 m_bUsePoint1;               // [i] use m_point1 instead of asking for point
  BOOL32 m_bUsePoint2;               // [i] use m_point2 instead of asking for point
  BOOL32 m_bUsePoint3;               // [i] use m_point3 instead of asking for point
 
  BOOL32 m_bAcceptNumber;            // [i] if true, the last prompt accept number. Needed for center_start_end/angle
  
  int m_quadrant;                  // [internal] previous quadrant for last point. Needed for center_start_end/angle
  int m_dir;                       // [internal] previous arc direction. Needed for center_start_end/angle
  double m_angle;                  // [internal] the arc angle, used in center_start_angle
  double m_dDefaultRadius;          // [i/o] default radius for Start_End_Radius
  ON_3dVector m_normal;            // [i] the arc normal. Needed for center_start_end/angle

  eArcMode m_mode;                 // [internal]  Current mode while drawing the arc

  BOOL32 m_allow_deformable;
  BOOL32 m_deformable;

  int m_degree;
  int m_point_count;

private:
  unsigned char m_pen_usage = 0;
  unsigned char m_reserved[3] = { 0 };
};


RHINO_SDK_FUNCTION CRhinoCommand::result RhinoGetArc( 
    CArgsRhinoGetArc&,
    ON_Arc&, // output_sphere
    CRhinoHistory* = NULL // optional history recorder
    );

