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

// Description:
//   CArgsRhinoText is used to pass input to RhinoGetText().
class RHINO_SDK_CLASS CArgsRhinoText
{
public:

  CArgsRhinoText();
  ~CArgsRhinoText() = default;
  CArgsRhinoText(const CArgsRhinoText&)  = default;
  CArgsRhinoText& operator=(const CArgsRhinoText&) = default;

  CArgsRhinoText( 
    unsigned int rhino_doc_sn,
    const ON_3dPoint* start_point, 
    const wchar_t* string
    );

  void SetDefaultPrompts();

  // prompt when getting start point
  const wchar_t* StartPointPrompt() const;
  void SetStartPointPrompt( const wchar_t* prompt);

  // prompt when getting the text string
  const wchar_t* StringPrompt() const;
  void SetStringPrompt( const wchar_t* prompt);

  // Text start point
  const ON_3dPoint* StartPoint() const;
  void SetStartPoint( const ON_3dPoint& start_point);
  const wchar_t* String() const;
  void SetString( const wchar_t* string);

  bool HaveStartPoint() const;
  bool HaveString() const;

  // Current text style is used if its not forced here
  /*
  Returns:
    -1: Use document default
  */
  int DimStyleIndex() const;

  int DimStyleIndex(const CRhinoDoc& doc) const;
  void SetDimStyleIndex( const CRhinoDoc& doc, int test_style_index);

  double Height() const;
  void SetHeight( double height);

  // Set or get whether the command is being run interaceively
  bool IsInteractive();
  void SetInteractive( bool interactive);

protected:
  bool m_interactive = true;

  // [in] prompt when getting start point
  ON_wString m_promptstartpoint;

  // [in] prompt when getting the text string
  ON_wString m_promptstring;

  //[in]  font index in the Rhino text style table
  int m_dim_style_index = -1;  // -1 means use doc default

  ON_3dPoint m_startpoint = ON_3dPoint::Origin;
  ON_wString m_string = ON_wString::EmptyString;
  double m_height = 1.0;

  bool m_havepoint = false;
  bool m_havestring = false;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

