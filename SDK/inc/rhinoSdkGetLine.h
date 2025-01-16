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

// Description:
//   CArgsRhinoGetLine is used to pass input to RhinoGetLine().
class RHINO_SDK_CLASS CArgsRhinoGetLine
{
public:

  enum eLineMode
  {
    two_point,
    surface_normal,
    angled,
    vertical,
    four_point,
    bisector,
    perpendicular,
    tangent,
    curve_end,
    cpnormalvector // added 23 Aug 2011 - Lowell - to enable picking 1 point
                   // and pressing enter to use cplane normal direction
  };


  CArgsRhinoGetLine();
  ~CArgsRhinoGetLine();
  CArgsRhinoGetLine(const CArgsRhinoGetLine&);
  CArgsRhinoGetLine& operator=(const CArgsRhinoGetLine&);

  // prompt when getting first point
  const wchar_t* FirstPointPrompt() const;
  void SetFirstPointPrompt( const wchar_t* );

  // prompt when getting midpoint
  const wchar_t* MidPointPrompt() const;
  void SetMidPointPrompt( const wchar_t* );

  // prompt when getting second point
  const wchar_t* SecondPointPrompt() const;
  void SetSecondPointPrompt( const wchar_t* );

  /*
  Description:
    Controls whither or not a zero length line is acceptable.
    The default is to require the user to keep picking the end
    point until we get a point different than the start point.
  See Also:
    CArgsRhinoGetLine::EnableAcceptZeroLengthLine
  */
  BOOL32 AcceptZeroLengthLine() const;

  /*
  Description:
    Controls whither or not a zero length line is acceptable.
    The default is to require the user to keep picking the end
    point until we get a point different than the start point.
  See Also:
    CArgsRhinoGetLine::AcceptZeroLengthLine
  */
  void EnableAcceptZeroLengthLine( BOOL32 = TRUE );

  /*
  Description:
    If set, the feedback color is used to draw the dynamic
    line when the second point is begin picked.  If not set,
    the active layer color is used.
  Returns:
    TRUE if line feedback color has been set.
  See Also:
    CArgsRhinoGetLine::SetFeedbackColor
    CArgsRhinoGetLine::FeedbackColor
  */
  BOOL32 HaveFeedbackColor() const;

  /*
  Description:
    If set, the feedback color is used to draw the dynamic
    line when the second point is begin picked.  If not set,
    the active layer color is used.
  Returns:
    TRUE if line feedback color has been set.
  See Also:
    CArgsRhinoGetLine::HaveFeedbackColor
    CArgsRhinoGetLine::FeedbackColor
  */
  ON_Color FeedbackColor() const;

  /*
  Description:
    If set, the feedback color is used to draw the dynamic
    line when the second point is begin picked.  If not set,
    the active layer color is used.
  Returns:
    TRUE if line feedback color has been set.
  See Also:
    CArgsRhinoGetLine::HaveFeedbackColor
    CArgsRhinoGetLine::FeedbackColor
  */
  void SetFeedbackColor( COLORREF );

  //////////
  // If the line should have a fixed length, set that length here
  void SetLength( double );

  //////////
  // If Length() > 0, the line must have the specified length.
  double Length() const;

  // If TRUE, then the "BothSides" option shows up when the
  // start point is inteactively picked.
  void EnableFromMidPointOption( BOOL32 = TRUE );
  BOOL32 MidPointOption() const;

  //////////
  // If TRUE, then the user picks the midpoint and endpoint
  void EnableFromMidPoint( BOOL32 = TRUE );

  //////////
  // If TRUE, then the user picked the midpoint and endpoint
  BOOL32 FromMidPoint() const;

  //////////
  // If TRUE, then the all line variations are shown if the default line mode is used
  void EnableShowVariations( BOOL32 = FALSE );

  //////////
  // If TRUE, then showing line variations is enabled
  BOOL32 ShowVariations() const;

  //////////
  // Use SetFirstPoint() to specify the line's starting point and skip the
  // start point interactive picking.
  void SetFirstPoint( const ON_3dPoint& );

  //////////
  // If FirstPoint() returns NULL, then the user interactively picks the
  // starting point.  If FirstPoint() is not NULL, then that point is
  // used as the first point.
  const ON_3dPoint* FirstPoint() const;


  //////////
  // Set current line mode
  void SetLineMode( int = CArgsRhinoGetLine::two_point );

  //////////
  // The current line mode
  int LineMode() const;

  // RhinoGetLine() returns GetPoint object references to 
  // picked points here.
  CRhinoObjRef m_objref[2];

  //////////
  // Makes point picker ignore trims on trimmed surface when mode is surface_normal.
  void SetIgnoreTrims( BOOL32 = TRUE);

  //////////
  // The current ignore trims state
  BOOL32 IgnoreTrims();
  
  // When true the linetype of the active layer is used for dynamic drawing
  void SetUseActiveLayerLinetype(bool on);
  bool UseActiveLayerLinetype() const;
protected:

  //////////
  // [in] prompt when getting start point
  ON_wString m_first_point_prompt;

  //////////
  // [in] prompt when getting midpoint
  ON_wString m_midpoint_prompt;

  //////////
  // [in] prompt when getting end point
  ON_wString m_second_point_prompt;

  //////////
  // [in] color to use when drawing dynamic line while getting end point
  ON_Color m_dynamic_line_color;

  //////////
  // [in] If m_bHaveStartPoint is TRUE, then m_start_point is used as
  // the starting point and the 
  ON_3dPoint m_start_point; 
  BOOL32 m_bHaveStartPoint;

  // [in]
  BOOL32 m_bAcceptZeroLengthLine;

  //////////
  // [in/out] If m_bFromMidpoint is TRUE, then the user picked the midpoint
  // and endpoint
  BOOL32 m_bBothSides;

  BOOL32 m_bEnableBothSidesOption;

  BOOL32 m_bShowVariations;

  int m_LineMode;

  double m_fixed_length;

  BOOL32 m_bIgnoreTrims;
private:
  unsigned char m_pen_usage = 0;
  unsigned char m_reserved[3] = { 0 };
};

// Description:
//   CArgsRhinoGetLine is used to pass input to RhinoGetLine().
class RHINO_SDK_CLASS CArgsRhinoGetPolyline : public CArgsRhinoGetLine
{
public:
  CArgsRhinoGetPolyline();
  ~CArgsRhinoGetPolyline();
  CArgsRhinoGetPolyline(const CArgsRhinoGetPolyline&);
  CArgsRhinoGetPolyline& operator=(const CArgsRhinoGetPolyline&);

  // prompt when getting third point
  // Default = "Next point of polyline".
  const wchar_t* ThirdPointPrompt() const;
  void SetThirdPointPrompt( const wchar_t* );

  // prompt when getting forth and subsequent points
  // Default = "Next point of polyline".
  const wchar_t* NextPointPrompt() const;
  void SetNextPointPrompt( const wchar_t* );

  // [in] If m_bPermitCloseOption is TRUE, then after 3 points
  // have been picked, the user can type "Close" and a closed 
  // polyline will be returned. The default is TRUE.
  BOOL32 m_bPermitCloseOption;

  // [in] If m_bPermitCloseSnap is TRUE, then after 3 points
  // have been picked, the user can pick near the start point and
  // a closed polyline will be returned.
  // The default is TRUE.
  BOOL32 m_bPermitCloseSnap;

  // [in] If m_bForceClosed is TRUE, then the returned polyline
  // is always closed. If m_bForceClosed is TRUE, then m_max_point_count
  // must be 0 or >= 4.  The default is FALSE.
  BOOL32 m_bForceClosed;

  // [in] minimum number of points to require.  On closed polylines,
  // the start/end point is counted once.  The default is 2.
  int m_min_point_count; 

  // [in] maximum number of points to require; zero for no limit.
  // On closed polylines, the start/end point is counted twice.
  // The default is 0.
  int m_max_point_count; 

  // [out]  If the polyline was closed by using the "Close" option
  // or snapping to the start point, then m_bClosed is set to TRUE.
  BOOL32 m_bClosed;

private:
  // [in] prompt when getting third point. 
  // Default = "Next point of polyline".
  ON_wString m_third_point_prompt;

  // [in] prompt when points after the first point.
  // Default = "Next point of polyline".
  ON_wString m_next_point_prompt;
};

/*
Description:
  Use RhinoGetLine() to interactively get an ON_Line.  The Rhino "Line"
  command uses RhinoGetLine.  Use the CArgsRhinoGetLine argument to
  specify input options like prompts, fixed starting points, and so on.
  See CArgsRhinoGetLine for complete details.  The line is returned as
  the second argument.  If you are getting a line in a command and want
  to provide history support, then pass a pointer to a CRhinoHistory
  class as the third argument.
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetLine(
       CArgsRhinoGetLine&,     // [in/out]
       ON_Line&,               // [out] line returned here
       CRhinoHistory* = NULL   // [out] optional history returned here
       );


/*
Description:
  Interactively get a polyline.
Parameters:
  args - [in/out] controls prompts and options available when getting
           the polyline.
  output_polyline - [out] polyline returned here
  history - [out] optional history record.
Returns:
  CRhinoCommand::success - successfully go polyline
  CRhinoCommand::cancel - user canceled
  CRhinoCommand::failure - unable to get valid polyline
Remarks:
  The Rhino "Polyline" command uses RhinoGetPolyline.
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetPolyline(
       CArgsRhinoGetPolyline& args,
       ON_Polyline& output_polyline,
       CRhinoHistory* history = NULL
       );


#if defined (ON_RUNTIME_APPLE)
class CRhinoGetPolylinePoint : public CRhinoGetPoint
{
public:
  CRhinoGetPolylinePoint();
  ~CRhinoGetPolylinePoint();
  
  void DynamicDraw( CRhinoDisplayPipeline&,
                    const ON_3dPoint&  // current mouse/digitizer location
                    ) override;
  
  CRhinoGetPolylinePoint* GetPolylinePoint(void) override;
  
  BOOL32 m_bFromMidpoint;
  void TweakLine(ON_Line&);
  ON_Polyline m_pline;
};
#endif

