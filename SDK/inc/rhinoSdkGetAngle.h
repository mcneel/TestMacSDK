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

//////////////////////////////////////////////////////////////////
//CRhinoGetAngle
class RHINO_SDK_CLASS CRhinoGetAngle : public CRhinoGetPoint
{
public:
  CRhinoGetAngle();
  CRhinoGetAngle( const CRhinoGet&);


  /////  All programmatic input and output to CRhinoGetAngle is in radians
  //     But the user input if a number is typed is in the 
  //     current angle mode ( which for now is always degrees)


  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Set up the frame for the angle 
  //         and optional starting angle from frame x axis
  //
  void SetReference( double refangle); // Angle in radians
  void SetFrame( const ON_Plane& frame);
  void ClearFrame();
  void SetBase( const ON_3dPoint& pt);
  void ClearBase();
  // if a reference point is set, the reference angle is calculated 
  // from it and a reference lijne is drawn from the base point to it
  // This will override any reference angle set with SetReference()
  // the point will be projected to the plane or cplane
  // being used by GetAngle()
  void SetReferencePoint( const ON_3dPoint& pt);
  void ClearReferencePoint();


  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Optional - Set a default value to use if user presses ENTER
  //
  void SetDefault( double d); // Angle in radians
  void ClearDefault();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Optional - override CRhinoGetAngle::OnMouseMove()
  //                    if you want to be called every time the
  //                    mouse moves.  OnMouseMove() is called once
  //                    per mouse move and is called BEFORE any
  //                    calls to DynamicDraw().
  // If you overrid this function, call CRhinoGetAngle::OnMouseMove()
  // from your version
  //
  // Also see CRhinoGetPoint::DynamicDraw()
  //
  void OnMouseMove( CRhinoViewport& vp, UINT nFlags, const ON_3dPoint& pt, const ON_2iPoint* p2);
  
  //////////////////////////////////////////////////////////////////
  //
  // STEP 4a: Optional - override CRhinoGetPoint::DynamicDraw()
  //                     if you want to dynamically draw things
  //                     as the mouse/digitizer moves.  Every
  //                     time the mouse moves, DynamicDraw()
  //                     will be called once per viewport.
  //                     The calls to DynamicDraw() happen AFTER
  //                     the call to OnMouseMove().
  //
  //   Use the CRhinoView::Draw* tools to draw things.
  //   Use Rhino::LayerColor(), etc., to get Rhino system colors.
  //
  //   If you are drawing anything that takes a long time, periodically
  //   call InterruptMouseMove() to see if you should stop.
  //
  //   If you override both OnMouseMove() and DynamicDraw(), then every
  //   time the mouse moves you will first get a single call to
  //   OnMouseMove() and then get a call to DynamicDraw() for each
  //   viewport.
  //
  //   If you override dynamic draw, you should call 
  //   CRhinoGetPoint::DynamicDraw() at the end of your override.
  //
  void DynamicDraw( CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);



  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Call GetAngle() to get an angle.
  //
  CRhinoGet::result GetAngle();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 6: If GetAngle() returned CRhinoGet::angle, 
  //         Call Angle() to get the angle value.
  //
  double Angle();

  // Get the plane the angle was selected in - New Nov2, 2009 - Lowell
  const ON_Plane& ResultPlane();

protected:
  double CalcPointAngle( CRhinoViewport& vp, const ON_3dPoint& pt);

  int m_havedefault;
  double m_default;
  ON_Plane m_frame;
  double m_angle;
  double m_refangle;
  BOOL32 m_havebase;   // otherwise ask
  BOOL32 m_fixframe;   // force angle to specified frame
  BOOL32 m_have_refpoint;
  ON_3dPoint m_refpoint;
  ON_Plane m_lastplane;

private:  // not implemented
  CRhinoGetAngle( const CRhinoGetAngle&);
  CRhinoGetAngle& operator=( const CRhinoGetAngle& src);

};


