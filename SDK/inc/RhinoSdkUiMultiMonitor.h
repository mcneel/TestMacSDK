/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

class RHINO_SDK_CLASS CRhinoUiMultiMonitor
{
public:
  //
  // Description:
  //  Get the number of monitors currently active.
  // Returns:
  //  Returns the number of monitors currently active or value less than one
  //  on error.
  static int NumberOfMonitors();
  //
  // Description:
  //  Get client workspace rectangle for monitor which contains the majority
  //  of the specified rectangle.
  // Parameters:
  //   rc [in] Rectangle used to identify monitor.
  // Returns:
  //  If CRect::IsRectEmpty() then there was an error; otherwise a valid client
  //  workspace rectangle is returned;
  //
  static ON_4iRect MonitorRectFromRect( const ON_4iRect&  rc);
  //
  // Description:
  //  Get client workspace rectangle for monitor which contains the specified
  //  screen point.
  // Parameters:
  //   point [in] Screen point used to identify monitor.
  // Returns:
  //  If CRect::IsRectEmpty() then there was an error; otherwise a valid client
  //  workspace rectangle is returned;
  //
  static ON_4iRect MonitorRectFromPoint( const ON_2iPoint& point);
  //
  // Description:
  //  Get monitor indexfor monitor which contains the majority of the specified 
  //  rectangle.
  // Parameters:
  //   rc [in] Rectangle used to identify monitor.
  // Returns:
  //  Returns NULL if rect is not on a monitor otherwise a valid monitor handle.
  //
  static HMONITOR MonitorFromRect( const ON_4iRect& rc);
  //
  // Description:
  //  Get monitor indexfor monitor which contains the point.
  // Parameters:
  //   rc [in] Point used to identify monitor.
  // Returns:
  //  Returns NULL if point is not on a monitor otherwise a valid monitor handle.
  //
  static HMONITOR MonitorFromPoint( const ON_2iPoint& point);
  //
  // Description:
  //  Get client workspace rectangle for monitor which contains the majority
  //  of the specified window.
  // Parameters:
  //   hWnd [in] Window handle used to search for monitor which contains the
  //             majority of the window.
  // Returns:
  //  If CRect::IsRectEmpty() then there was an error; otherwise a valid client
  //  workspace rectangle is returned;
  //
  static ON_4iRect MonitorRectFromWindow( HWND hWnd);
  //
  // Description:
  //  Force specified rectangle to monitor which contains the majority of
  //  the window specified by hWnd.
  // Parameters:
  //   rc [in/out] Rectangle to validate
  //   hWnd [in] Window handle used to search for monitor which contains the
  //             majority of the window.
  // Returns:
  //  true If main monitor was determined and rectangle was forced on monitors
  //  client work area; otherwise false.
  //
  static bool ForceWindowRectOnMonitor( ON_4iRect& rc, HWND hWnd);
  //
  // Description:
  //  Call this method to get a list of screen rectangles associated with the
  //  currently installed monitors
  // Parameters:
  //   mon_rects [out] List of screen rectangles
  // Returns:
  //  Returns the number of rectangles added to mon_rects
  //
  static int GetMultipleMonitorRects( ON_SimpleArray<ON_4iRect>& mon_rects);
};
