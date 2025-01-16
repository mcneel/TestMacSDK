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

//////////////////////////////////////////////////////////////////////////
//
// CLASS
// CRhinoGetDirection
//
class RHINO_SDK_CLASS CRhinoGetDirection : public CRhinoGetPoint
{
public:
  CRhinoGetDirection();
  CRhinoGetDirection( const CRhinoGet&);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1a: Use SetSecondPrompt() if you want to specify a different 
  //          prompt for the second picked point.
  //
  void SetSecondPrompt( const char* p1);
  void SetSecondPrompt( const wchar_t* p1);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Optional - Set a basepoint to use for mouse input
  //                    If none is set and mouse is used for input,
  //                    the first point picked becomes the base
  //
  void SetBase( ON_3dPoint& base);
  void ClearBase();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Optional - Set a default value to use if user presses ENTER
  //
  void SetDefault( ON_3dVector& d);
  void ClearDefault();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Call GetDirection() to get a Direction.
  //
  CRhinoGet::result GetDirection();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 6: If GetDirection() returned CRhinoGet::Direction, 
  //         Call Direction() to get the Direction value.
  //
  ON_3dVector Direction();


protected:
  int m_havedefault;
  ON_3dVector m_default;
  ON_3dVector m_Direction;
  ON_wString m_SecondPrompt;
};



