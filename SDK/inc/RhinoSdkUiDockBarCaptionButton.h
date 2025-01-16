// Copyright (c) 1993-2016 Robert McNeel & Associates. All rights reserved.
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

class RHINO_SDK_CLASS CRhinoUiDockBarCaptionButton
{
public:
  CRhinoUiDockBarCaptionButton(bool closeButton);

  // Description:
  //   Return rectangle for button in parent window units.  This will be an empty rectangle
  //   if the button is hidden.
  CRect GetRect(const class CRhinoUiDockBar* pDockBar) const;

  // Description:
  //   Paint button
  // Parameters:
  //   pDC [in] Device context capable of painting in non client area of window
  //   hWnd [in] Window handle for painting to
  void Paint(CDC* pDC, const class CRhinoUiDockBar* pDockBar);

  // Description:
  //   Button state to check
  enum button_state
  {
    bs_pushed = 0,
    bs_focused,
    bs_hidden,
    bs_disabled,
    bs_count // not a state, this is used to size the state array
  };

  // Description:
  //   Check to see if a current button state is set.
  bool ButtonState( button_state bs) const;

  // Description:
  //   Change button state.
  // Returns:
  //   true if the button state actually changed
  //   false if the current button state already has the same value
  bool SetButtonState( button_state bs, bool b);

  // Description:
  //   Gets the button visibility state
  // Returns:
  //   true if the button is visible otherwise;
  //   false if the button is hidden
  bool IsButtonVisible() const;

  // Description:
  //   Call this method to get the space between caption buttons
  //   as well as the non client button margin.
  // Returns:
  //   Returns the margin * the current DPI scale.
  static int Margin();

  const wchar_t* ToolTipText() const;
  UINT HitTest() const;
private:
  bool m_close_button;
  bool m_button_states[bs_count];
};