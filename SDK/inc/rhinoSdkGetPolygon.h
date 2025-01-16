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


#define POLYGON_MIN_NUMSIDES 3
#define POLYGON_MAX_NUMSIDES 50000

// SDK header stuff
/*-------------------------------------------------------------------
CLASS
  CArgsRhinoGetPolygon

OVERVIEW TEXT
  Arguments for RhinoGetPolygon... functions

KEYWORDS
  Polygon RhinoGetPolygon GetPolygon
-------------------------------------------------------------------*/
class RHINO_SDK_CLASS CArgsRhinoGetPolygon
{
public:
  CArgsRhinoGetPolygon();
  ~CArgsRhinoGetPolygon();

  CArgsRhinoGetPolygon(const CArgsRhinoGetPolygon&);
  CArgsRhinoGetPolygon& operator=(const CArgsRhinoGetPolygon&);


  //sets
  void SetInscribedCenterPrompt( const wchar_t*);
  void SetInscribedAroundCurveCenterPrompt( const wchar_t*);
  void SetCornerPrompt( const wchar_t*);

  void SetCircumscribedCenterPrompt( const wchar_t*);
  void SetCircumscribedAroundCurveCenterPrompt( const wchar_t*);
  void SetEdgeMidpointPrompt( const wchar_t*);

  void SetEdgeStartPrompt( const wchar_t*);
  void SetEdgeEndPrompt( const wchar_t*);

  void SetStarCenterPrompt( const wchar_t*);
  void SetStarAroundCurveCenterPrompt( const wchar_t*);
  void SetStarEndPrompt( const wchar_t*);  
  void SetStarSecondRadiusPrompt( const wchar_t*);

  void SetCircle( const ON_Circle&);

  void SetFirstPoint( const ON_3dPoint&);
  void SetSecondPoint( const ON_3dPoint&);
  void SetNormal( const ON_3dVector&);
  
  void SetNumSides( int );
  void SetStarSecondRadius( double);

  void SetInscribed( BOOL32);
  void SetStarMode( BOOL32);
  void SetEdgeMode( BOOL32);
  void SetUseFirstPoint( BOOL32);
  void SetVertical( BOOL32);
  void SetAroundCurve( BOOL32);
  void SetCurve(const ON_Curve*);
  void SetUseNormal( BOOL32);
  void SetFlip( BOOL32);
  void SetUseStarSecondRadius( BOOL32);
  void SetGettingStarModePolygon( BOOL32);

  void EnableVerticalPolygonOption(bool = true);
  void EnablePyramidOption(bool = true);
  void SetPyramidConstraint(int);
  void SetCap(unsigned int);

  //gets
  const ON_wString& InscribedCenterPrompt() const;
  const ON_wString& InscribedAroundCurveCenterPrompt() const;
  const ON_wString& CornerPrompt() const;

  const ON_wString& CircumscribedCenterPrompt() const;
  const ON_wString& CircumscribedAroundCurveCenterPrompt() const;
  const ON_wString& EdgeMidpointPrompt() const;

  const ON_wString& EdgeStartPrompt() const;
  const ON_wString& EdgeEndPrompt() const;

  const ON_wString& StarCenterPrompt() const;
  const ON_wString& StarAroundCurveCenterPrompt() const;
  const ON_wString& StarEndPrompt() const;  
  const ON_wString& StarSecondRadiusPrompt() const;

  const ON_Circle& Circle() const;

  const ON_3dPoint& FirstPoint() const;
  const ON_3dPoint& SecondPoint() const;
  const ON_3dVector& Normal() const;
  
  int NumSides() const;
  double StarSecondRadius() const;

  const BOOL32 Inscribed() const;
  const BOOL32 StarMode() const;
  const BOOL32 EdgeMode() const;
  const BOOL32 UseFirstPoint() const;
  const BOOL32 Vertical() const;
  const BOOL32 AroundCurve() const;
  const ON_Curve* Curve() const;
  const BOOL32 UseNormal() const;
  const BOOL32 Flip() const;
  const BOOL32 UseStarSecondRadius() const;
  const BOOL32 GettingStarModePolygon() const;

  const bool VerticalPolygonOptionAvailable() const;
  const bool PyramidOptionAvailable() const;
  const int PyramidConstraint() const;
  unsigned int GetCap() const;

  // When true the linetype of the active layer is used for dynamic drawing
  void SetUseActiveLayerLinetype(bool on);
  bool UseActiveLayerLinetype() const;

  // variables
  int m_NumSides;

protected:
  ON_wString  m_InscribedCenterPrompt;
  ON_wString  m_InscribedAroundCurveCenterPrompt;
  ON_wString  m_CornerPrompt;

  ON_wString  m_CircumscribedCenterPrompt;
  ON_wString  m_CircumscribedAroundCurveCenterPrompt;
  ON_wString  m_EdgeMidpointPrompt;

  ON_wString  m_EdgeStartPrompt;
  ON_wString  m_EdgeEndPrompt;

  ON_wString  m_StarCenterPrompt;
  ON_wString  m_StarAroundCurveCenterPrompt;
  ON_wString  m_StarEndPrompt;  
  ON_wString  m_StarSecondRadiusPrompt;

  ON_Circle m_Circle;

  ON_3dPoint m_FirstPoint;
  ON_3dPoint m_SecondPoint;
  ON_3dVector m_Normal;
  
  double m_StarSecondRadius;

  BOOL32 m_Inscribed; // true=inscribed, false=circumscribed
  BOOL32 m_StarMode; // true=star
  BOOL32 m_EdgeMode; // true=polygon by edge
  BOOL32 m_UseFirstPoint;
  BOOL32 m_Vertical;
  BOOL32 m_AroundCurve;
  const ON_Curve* m_Curve;
  BOOL32 m_UseNormal;
  BOOL32 m_Flip;
  BOOL32 m_UseStarSecondRadius;
  BOOL32 m_bGettingStarModePolygon;

  bool m_bVerticalPolygonAvailable;
  bool m_bPyramidAvailable;

private:
  unsigned char m_bCap;        // [io] Used when CRhinoGetPolygon is called by a primitive solid command like pyramid
  unsigned char m_pen_usage = 0;

protected:
  int m_bPyramidConstraint; //0 = None, 1 = Vertical, 2 = AroundCurve

};



class RHINO_SDK_CLASS CArgsRhinoGetPolygonExtra : public CArgsRhinoGetPolygon
{
public:
  CArgsRhinoGetPolygonExtra();
  virtual ~CArgsRhinoGetPolygonExtra();
  //CArgsRhinoGetPolygonExtra(const CArgsRhinoGetPolygon&);
  CArgsRhinoGetPolygonExtra(const CArgsRhinoGetPolygonExtra&);
  CArgsRhinoGetPolygonExtra& operator=(const CArgsRhinoGetPolygonExtra&);

  // November 11, 2024 - Tim
  // Partial fix (pyramid) for https://mcneel.myjetbrains.com/youtrack/issue/RH-63751
  // Add settings for bothsides.
  const bool AddBothSides() const;
  void SetAddBothSides(const bool);
  const CRhinoCommandOptionName& BothSidesOption() const;
  void SetBothSidesOption(const CRhinoCommandOptionName&);
  const bool BothSides() const;
  void SetBothSides(const bool);

protected:
  class CInternalArgsRhinoGetPolygonExtra* m_internal;
};

// interactive Polygon getter declaration
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetPolygon(
  CArgsRhinoGetPolygon& args,  // [in/out] 
  ON_Polyline& polygon_out,  // [out] Polygon returned here
  CRhinoHistory* history = NULL  // [out] optional history returned here
  );

