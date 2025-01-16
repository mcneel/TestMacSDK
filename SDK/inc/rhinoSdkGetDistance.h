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
// CRhinoGetDistance
class RHINO_SDK_CLASS CRhinoGetDistance : public CRhinoGetPoint
{
public:
  CRhinoGetDistance();
  CRhinoGetDistance( const CRhinoGet&);

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
  void SetDefault( double d);
  void ClearDefault();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Optional - Set this option if you want the user to
  //         be able to enter a negative distance value from the 
  //         keyboard. The default is setting is not to allow
  //         negative input from the keyboard.
  //
  void AllowNegativeDistance( bool b = true );

  //////////////////////////////////////////////////////////////////
  //
  // STEP 6: Call GetDistance() to get a distance.
  //
  CRhinoGet::result GetDistance();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 7: If GetDistance() returned CRhinoGet::distance, 
  //         Call Distance() to get the distance value.
  //
  double Distance();


protected:
  int m_havedefault;
  double m_default;
  double m_distance;
  ON_wString m_SecondPrompt;
  bool m_allow_negative_distance;
};

