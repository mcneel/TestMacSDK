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


// CRhinoUiButton

class RHINO_SDK_CLASS CRhinoUiButton : public CButton
{
	DECLARE_DYNAMIC(CRhinoUiButton)

public:
	CRhinoUiButton();
	virtual ~CRhinoUiButton();

public:
  //
  // Description:
  //    Use the following method to set the tool tip text associated with this button.
  //
  // Parameters:
  //    lpsTip [in] Tool tip text to associate with this button.
  //
  // See Also:
  //    CRhinoUiButton::GetToolTipText()
  //
  void SetToolTipText( const wchar_t* lpsTip);
  //
  // Description:
  //    Use the following method to get the tool tip text associated with this button.
  //
  // Returns:
  //    Returns tool tip text associated with this button.
  //
  // See Also:
  //    CRhinoUiButton::SetToolTipText()
  //    
  const wchar_t* GetToolTipText() const;
  enum mouse_down_button
  {
    mdb_left = 0,
    mdb_right,
    mdb_middle,
  };
  // Use the following macro in your dialogs message map to receive notification when
  // a mouse button goes down over a button.
  //  ON_REGISTERED_MESSAGE( CRhinoUiButton::m_on_mouse_button_down, OnRhinoUiButtonMouseDown)
  // Method prototype:
  //  afx_msg LRESULT OnRhinoUiButtonMouseDown( WPARAM wParam, LPARAM lParam);
  //   wParam
  //      The low-order word contains the button's control identifier. The high-order word specifies
  //      which mouse button went down.  See "enum mouse_down_button" above. 
  //   lParam
  //      Window handle for the button. 
  // Return a value greater than zero to handle the mouse event yourself.
  // NOTE:
  //   You must call CRhinoUiButton::OnCustomMouseButtonDown() if you return a value greater than
  //   zero so the button state can be correctly set.
  static const UINT m_on_mouse_button_down;
  //
  // Description:
  //    This MUST be called if your CRhinoUiButton::m_on_mouse_button_down handler returns a value
  //    greater than zero to clean up the button state.
  //
  // See Also:
  //    CRhinoUiButton::m_on_mouse_button_down
  //    
  void OnCustomMouseButtonDown();
  //
  // Description:
  //    Check to see if owner draw button should draw its frame when not hot or active.
  //
  // Returns:
  //    Returns true if button frame will only be drawn when mouse is over the button.
  //
  // See Also:
  //    CRhinoUiButton::SetSuppressButtonFrameWhenNotHot()
  //
  bool SuppressButtonFrameWhenNotHot() const;
  //
  // Description:
  //    Call this method to make owner draw button draw its frame only when hot or active.
  //
  // Parameters:
  //    b [in] If true then button frame will only draw when the mouse is not over the button.
  //
  // Returns:
  //    Returns previous value.
  //
  // See Also:
  //    CRhinoUiButton::SuppressButtonFrameWhenNotHot()
  //
  bool SetSuppressButtonFrameWhenNotHot( bool b);
  //
  // Description:
  //    Helper function for owner draw buttons. Transparently draw button text (if it has text)
  //    to specified device context.
  //
  // Parameters:
  //    dc [in] Device context to draw to.
  //    rText [in] Bounding rectangle for text in device context coordinate system.
  //
  void DrawButtonText( CDC& dc, CRect rText);
  //
  // Description:
  //    Helper function for owner draw buttons. Transparently draw button bitmap to specified
  //    device context.
  //
  // Parameters:
  //    dc [in] Device context to draw to.
  //    r  [in] Bounding rectangle button image.
  //
  void DrawButtonBitmap( CDC& dc, CRect r);
  //
  // Description:
  //    Helper function for owner draw buttons. Call this method to determine wether or not
  //    the button frame should be drawn.
  //
  // Returns:
  //    Returns true if button frame should be drawn at this time.
  //
  bool DrawButtonFrame() const;
  //
  // Description:
  //    If this returns true and the button is owner draw then the button will act like a menu
  //    bar item.
  //
  // Returns:
  //    Returns true if button should be drawn like a menu
  //
  // See Also:
  //    CRhinoUiButton::SetIsMenuButton()
  //
  bool IsMenuButton() const;
  //
  // Description:
  //    If this returns true and the button is owner draw then the button will act like a menu
  //    bar item.
  //
  // Parameters:
  //    b [in] If true then the button will be drawn like a menu item
  //
  // Returns:
  //    Returns previous state.
  //
  // See Also:
  //    CRhinoUiButton::IsMenuButton()
  //
  bool SetIsMenuButton( bool b);
  //
  // Description:
  //    Registered windows message sent by a IsMenuButton() when pressed
  //
  static const UINT m_on_menu_button_down;
  //---------------------------------------------------------------------------------
  // Color button methods
  //---------------------------------------------------------------------------------
  // Use the following macro in your dialogs message map to receive notification when
  // a color button is clicked and a new color value was specified:
  //  ON_REGISTERED_MESSAGE( CRhinoUiButton::m_on_color_changed_msg_id, OnRhinoUiColorButtonColorChanged)
  // Method prototype:
  //  afx_msg LRESULT OnRhinoUiColorButtonColorChanged( WPARAM wParam, LPARAM lParam);
  // The WPARAM and LPARAM are the same as BN_CLICKED notification.
  static const UINT m_on_color_changed_msg_id;
  //
  // Description:
  //    Setting the button color to anything other than ON_UNSET_COLOR will cause the
  //    button to become a color button.  This is only valid if this is an owner draw
  //    button.
  //
  // Parameters:
  //    cr [in] Color to paint the button face
  //
  // See Also:
  //    CRhinoUiButton::ButtonColor()
  //    CRhinoUiButton::IsColorButton()
  //
  void SetButtonColor( COLORREF cr = ON_UNSET_COLOR);
  //
  // Description:
  //    Call this method to determine the current button face color.
  //
  // Returns:
  //    Returns the current button face color.  If the color is ON_UNSET_COLOR then
  //    this is not a color button.
  //
  // See Also:
  //    CRhinoUiButton::SetButtonColor()
  //    CRhinoUiButton::IsColorButton()
  //
  COLORREF ButtonColor() const;
  //
  // Description:
  //    Call this method to determine if this button is a color button.
  //
  // Returns:
  //    Returns true if SetButtonColor has been called with a color other than
  //    ON_UNSET_COLOR otherwise returns false.
  //
  // See Also:
  //    CRhinoUiButton::ButtonColor()
  //    CRhinoUiButton::SetButtonColor()
  //
  bool IsColorButton() const;
  //
  // Description:
  //    Call this method to determine if text should be displayed on a color button
  //    when it is enabled.  Color buttons will paint using the
  //    ::GetSystemColor( COLOR_BTNFACE) color and display text when disabled.
  //
  // Returns:
  //    Returns true if button text should be displayed on enabled color button.
  //
  // See Also:
  //    CRhinoUiButton::SetColor()
  //    CRhinoUiButton::IsColorButton()
  //
	bool SuppressTextWhenEnabled() const;
  //
  // Description:
  //    Call this method to make a color button display text when enabled.  By default
  //    text is suppressed for enabled buttons.
  //
  // Parameters:
  //    b [in] If true then text is displayed on color buttons when enabled.
  //
  // See Also:
  //    CRhinoUiButton::SetColor()
  //    CRhinoUiButton::IsColorButton()
  //
	void SetSuppressTextWhenEnabled( bool b);
  //
  // Description:
  //    This method is called when a color button is clicked. The default implementation
  //    displays the Rhino color dialog and invalidates the button if a new color is
  //    selected.
  //
  // Returns:
  //    Returns true if color was changed or false if not.
  //
  // See Also:
  //    CRhinoUiButton::SetColor()
  //    CRhinoUiButton::IsColorButton()
  //
  virtual bool OnColorButtonClicked();
  //---------------------------------------------------------------------------------
  // Bitmap button methods
  //---------------------------------------------------------------------------------
  enum image_list
  {
    il_normal = 0,
    il_disabled,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_image_list = 0xFFFFFFFF
  };
  //
  // Description:
  //    This method retrieves the handle to an image list used when drawing button face.
  //
  // Parameters:
  //    il [in] Specifies the type of image list. It can be one of these values:
  //      il_normal - Image list with enabled buttons
  //      il_disabled - Image list with disabled buttons.
  //
  // Returns:
  //    Returns a pointer to the image list used for drawing button face.
  //
  // See Also:
  //    CRhinoUiButton::SetImageList()
  //    CRhinoUiButton::IsBitmapButton()
  //
	CImageList* GetImageList( image_list il) const;
  //
  // Description:
  //    This method retrieves the bitmap size for a single bitmap in the image list.
  //
  // Parameters:
  //    il [in] Specifies the type of image list. It can be one of these values:
  //      il_normal - Image list with enabled buttons
  //      il_disabled - Image list with disabled buttons.
  //
  // Returns:
  //    Returns individual bitmap size.
  //
  // See Also:
  //    CRhinoUiButton::SetImageList()
  //    CRhinoUiButton::GetImageList()
  //
	CSize GetImageListImageSize( image_list il) const;
  //
  // Description:
  //    This method assigns an image list to a button.  You must call this method prior
  //    to creating the button or create the button as owner draw for this to be affective.
  //
  // Parameters:
  //    pImageList [in] Specifies the pointer to the image list to assign.
  //    il [in] Specifies the type of image list. It can be one of these values:
  //      il_normal - Image list with enabled buttons
  //      il_disabled - Image list with disabled buttons.
  // Returns:
  //    Returns a pointer to the previous image list.
  //
  // See Also:
  //    CRhinoUiButton::GetImageList()
  //    CRhinoUiButton::IsBitmapButton()
  //
	CImageList* SetImageList( CImageList* pImageList, image_list il);
  //
  // Description:
  //    This method retrieves the index of an image for the button face for bitmap.
  //
  // Parameters:
  //    il [in] Specifies the type of image list. It can be one of these values:
  //      il_normal - Image list with enabled buttons
  //      il_disabled - Image list with disabled buttons.
  //
  // Returns:
  //    Returns the index of the image to be used when drawing the button face.
  //
  // See Also:
  //    CRhinoUiButton::SetImageListIndex()
  //    CRhinoUiButton::SetImageList()
  //    CRhinoUiButton::GetImageList()
  //    
  int GetImageListIndex( image_list il) const;
  //
  // Description:
  //    This method assigns the index of an image for the button face for bitmap.
  //
  // Parameters:
  //    iIndex [in] Specifies the image index for the button face bitmap.
  //    il [in] Specifies the type of image list. It can be one of these values:
  //      il_normal - Image list with enabled buttons
  //      il_disabled - Image list with disabled buttons.
  //
  // Returns:
  //    Returns the index of the image to be used when drawing the button face.
  //
  // See Also:
  //    CRhinoUiButton::GetImageListIndex()
  //    CRhinoUiButton::SetImageList()
  //    CRhinoUiButton::GetImageList()
  //    
  int SetImageListIndex( int iIndex, image_list il);
  //
  // Description:
  //    This method is used to determine if a button is a bitmap button.  A button
  //    is a bitmap button if SetImageList() has been called assigning at least
  //    a normal image list and SetImageListIndex() has been called.
  //
  // Returns:
  //    Returns true if a button a bitmap will be used when drawing the button face.
  //
  // See Also:
  //    CRhinoUiButton::GetImageListIndex()
  //    CRhinoUiButton::SetImageList()
  //    CRhinoUiButton::GetImageList()
  //    
  bool IsBitmapButton() const;
  //
  // Description:
  //    Call this method to determine if the mouse is currently over this button.
  //
  // Returns:
  //    Returns true if the mouse is currently over this button, otherwise; returns false.
  //
  bool ButtonIsHot() const;

protected:
  //
  // Description:
  //    Call this method to determine if the mouse is currently over this button.
  //
  // Returns:
  //    Returns true if the mouse is currently over this button, otherwise; returns false.
  //
  bool MouseOverThisButton();
  //
  // Description:
  //    This method is called when a button is created to decide whether or not to
  //    force the button to be owner draw.  If a button color is set prior to creating
  //    the button or an image list assigned then the button will become owner draw.
  //
  // Returns:
  //    Returns true if button should be forced to be owner draw.
  //
  // See Also:
  //    CRhinoUiButton::IsColorButton()
  //    CRhinoUiButton::IsBitmapButton()
  //
  bool MakeOwnerDraw();
  enum
  {
    tool_tip_timer         = 100, // Tool tip timer ID
    tool_tip_cleanup_timer = 101, // Tool tip clean up timer ID
    tool_tip_cleanup_delay = 500, // Tool tip clean up delay
    track_mode_no_track       = 0,// Mouse capture mode
    track_mode_no_button_down = 1,// Hot button or tool tip
    track_mode_lbutton_down   = 2,// Left mouse button down
    track_mode_mbutton_down   = 3,// Middle mouse button down
    track_mode_rbutton_down   = 4,// Right mouse button down
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_button = 0xFFFFFFFF
  };
  //
  // Description:
  //    Call this method to display or hide the tool tip associated with this button.
  //
  // Parameters:
  //    bShow [in] If true then the tool tip will be displayed if this button as tool tip text.
  //    lpR [in] Button window rectangle, the tool tip will be displayed below this if possible.
  //
  // Returns:
  //    Returns true if tool tip was displayed or hidden successfully.
  //
  // See Also:
  //    CRhinoUiButton::SetToolTipText()
  //    CRhinoUiButton::EndTrackToolTip()
  //    
  bool TrackToolTip( bool bShow, LPRECT lpR);
  //
  // Description:
  //    Call this method to close the tool tip and reset all tracking tool tip properties.
  //
  // See Also:
  //    CRhinoUiButton::SetToolTipText()
  //    CRhinoUiButton::EndTrackToolTip()
  //    
  void EndTrackToolTip();
  //
  // Description:
  //    Call this method to determine it the tool tip should be displayed.
  //
  // Returns:
  //    Returns true if tool tip should be displayed, otherwise; returns false.
  //
  // See Also:
  //    CRhinoUiButton::SetToolTipText()
  //    CRhinoUiButton::TrackToolTip()
  //    CRhinoUiButton::EndTrackToolTip()
  //    
  bool MouseOkToDisplayToolTip();
  //
  // Description:
  //    Call this method to determine if the button tool tip is currently visible.
  //
  // Returns:
  //    If the tool tip has been created and is visible true is returned otherwise false is.
  //
  // See Also:
  //    CRhinoUiButton::SetToolTipText()
  //    CRhinoUiButton::TrackToolTip()
  //    CRhinoUiButton::EndTrackToolTip()
  //    
  bool ToolTipIsVisible() const;
  //
  // Description:
  //    This method is called by mouse event handlers.
  //
  // Parameters:
  //    mbd [in] Identifies button sending the message.
  //    nFlags [in] Indicates whether various virtual keys are down.  See OnLButtonDown in MSDN for details.
  //    point [in] Specifies the x- and y-coordinate of the cursor. These coordinates are always relative to
  //               the upper-left corner of the window.
  //
  // Returns:
  //    Returns true if this method handled the message indicating the calling method should not call the 
  //    base class.  If false is returned then the calling method should call the base class.
  //
  // See Also:
  //    CWnd::OnLButtonDown()
  //    CRhinoUiButton::OnMouseButtonUp()
  //    
  bool OnMouseButtonDown( mouse_down_button mdb, UINT nFlags, CPoint point);
  //
  // Description:
  //    This method is called by mouse event handlers.
  //
  // Parameters:
  //    mbd [in] Identifies button sending the message.
  //    nFlags [in] Indicates whether various virtual keys are down.  See OnLButtonUp in MSDN for details.
  //    point [in] Specifies the x- and y-coordinate of the cursor. These coordinates are always relative to
  //               the upper-left corner of the window.
  //
  // Returns:
  //    Returns true if this method handled the message indicating the calling method should not call the 
  //    base class.  If false is returned then the calling method should call the base class.
  //
  // See Also:
  //    CWnd::OnLButtonDown()
  //    CRhinoUiButton::OnMouseButtonDown()
  //    
  bool OnMouseButtonUp( mouse_down_button mdb, UINT nFlags, CPoint point);
  //
  // Description:
  //    Call this method to capture the mouse and begin tracking.  The default tracking mode is track_mode_no_button_down
  //
  // Returns:
  //    If mouse is captured true is returned, otherwise false is returned.
  //
  // See Also:
  //    CRhinoUiButton::track_mode_no_button_down
  //    CRhinoUiButton::EndTracking()
  //    
  bool StartTracking();
  //
  // Description:
  //    Call this method to end capture and clear all tracking flags.
  //
  // See Also:
  //    CRhinoUiButton::StartTracking()
  //    
  void EndTracking();

protected:
  bool m_bSuppressButtonFrameWhenNotHot;
  bool m_bIsMenuButton;
  // Color button properties
  COLORREF m_crButton;
	bool m_bSuppressTextWhenEnabled;
  // Bitmap button properties
  CImageList* m_image_list_normal;
  CImageList* m_image_list_disabled;
  int m_image_list_normal_index;
  int m_image_list_disabled_index;
  CSize m_image_size_normal;
  CSize m_image_size_disabled;
  // Tooltip
  int m_track_mode;
  bool m_traking_over_button;
  CRect m_rToolTip;
  ON_wString m_wToolTip;
  static CRhinoUiToolTip m_tooltip;
public:
  // For internal use only!
  // Reserved for future use.
  class CRhUiButtonExtension* m__button_sdk_extension;
protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
  // CWnd overrides
  virtual void PreSubclassWindow();
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual BOOL32 OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
public:
  // Event handlers
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnPaint();
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
