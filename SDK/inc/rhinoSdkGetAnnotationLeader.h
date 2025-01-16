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
//   CArgsRhinoAnnotationLeader is used to pass input to GetAnnotationLeader().
class RHINO_SDK_CLASS CArgsRhinoAnnotationLeader
{
public:
  CArgsRhinoAnnotationLeader();
  ~CArgsRhinoAnnotationLeader();

  // prompt when getting first dimension point
  const wchar_t* FirstPrompt() const;
  void SetFirstPrompt( const wchar_t* prompt);

  // prompt when getting second dimension
  const wchar_t* PointPrompt() const;
  void SetPointPrompt( const wchar_t* prompt);

  // color used to draw feedback line
  ON_Color FeedbackColor() const;
  void SetFeedbackColor( COLORREF color);

  // Set or get whether the command is being run interaceively
  bool IsInteractive();
  void SetInteractive( bool interactive);

protected:
  bool m_interactive;

  // [in] prompt when getting first point
  ON_wString m_first_prompt;

  // [in] prompt when getting other points
  ON_wString m_point_prompt;

  // [in] color to use when drawing dynamic line while getting end point
  ON_Color m_dynamic_line_color;

private:
  CArgsRhinoAnnotationLeader(const CArgsRhinoAnnotationLeader& src);
  CArgsRhinoAnnotationLeader& operator=(const CArgsRhinoAnnotationLeader& src);

};

//--------------------------------------------------------
// Interactive leader drawing

CRhinoCommand::result GetAnnotationLeader(
       const CRhinoDoc& doc,
       CArgsRhinoAnnotationLeader& dimargs,
       CRhinoAnnotationLeader*& output_leader,
       CRhinoHistory* history
       );

