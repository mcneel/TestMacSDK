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


// CRhinoUiPointEdit

class RHINO_SDK_CLASS CRhinoUiPointEdit : public CWnd
{
	DECLARE_DYNCREATE(CRhinoUiPointEdit)

public:
	CRhinoUiPointEdit();
	virtual ~CRhinoUiPointEdit();
  enum edit_field_styles
  {
    efs_borders = 1, // Adds client edge border to edit control
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_edit_field_styles = 0xFFFFFFFF
  };
  enum edit_fields
  {
    x_field = 0,
    y_field,
    z_field,
    edit_wnd_cnt = 3,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_edit_fields = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Create a point edit control.
  //
  // Parameters:
  //   pParent [in] The parent window. 
  //   dwStyle [in] Specifies the window style attributes.
  //   dwStyleEx [in] Specifies extended windows style attributes.
  //   r [in] The size and position of the window, in client coordinates of pParentWnd. 
  //   nID [in] The ID of the child window. 
  //
  // Returns:
  //   Returns true if control is created or false if it was not.
  //
  // See Also:
  //   CWnd::Create
  //   
  bool CreatePointEdit( CWnd* pParent, DWORD dwStyle, DWORD dwStyleEx, const CRect& r, UINT nID = 0);
  //   
  // Description:
  //   Replace a dialog item with this control 
  //
  // Parameters:
  //   pParent [in] Dialog to embed point edit control inside of.
  //   nID [in] The ID of the dialog control to replace.  The new
  //            point edit control will be created using this ID.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiPointEdit::CreatePointEdit()
  //   
  bool ReplaceDlgItem( CDialog* pParent, UINT nID);
  //   
  // Description:
  //   Current item label background color.
  //
  // Returns:
  //   Returns color used to fill edit label backgrounds.  If SetLabelBkGrndColor( CRhinoUiPaintManager::unset_color)
  //   is called then the current window color will be used.  To change call SetLabelBkGrndColor( RGB( r, g, b))
  //   with new background color.  By default the label background color is CRhinoUiPaintManager::unset_color which
  //   causes the window background color to be returned.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //
  COLORREF LabelBkGrndColor() const;
  //   
  // Description:
  //   This method is used to identify the edit label text color.  The default value is CRhinoUiPaintManager::unset_color
  //   which causes the default window text color to be used.
  //
  // Returns:
  //   Returns color to be used to to draw edit label text.  If color is CRhinoUiPaintManager::unset_color
  //   then default color is assumed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::LabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::OnDrawItem()
  //   
  COLORREF LabelTextColor() const;
  //   
  // Description:
  //    Used to override default label back ground color for unselected items.  Default
  //    value is CRhinoUiPaintManager::unset_color which causes the dialog background
  //    color to be used.  Set to valid RGB value to change label background color.
  //
  // Parameters:
  //   cr [in] New unselected label background color or CRhinoUiPaintManager::unset_color
  //           for default color.
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::LabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   CRhinoUiOptionsListCtrl::GridLineColor()
  //   CRhinoUiOptionsListCtrl::SetGridLineColor()
  //   
  COLORREF SetLabelBkGrndColor( COLORREF cr);
  //   
  // Description:
  //    Used to override default label text color for unselected items.  Default
  //    value is CRhinoUiPaintManager::unset_color which causes the dialog text
  //    color to be used.  Set to valid RGB value to change label text color.
  //
  // Parameters:
  //   cr [in] New label text color CRhinoUiPaintManager::unset_color for default color.
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::LabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::GridLineColor()
  //   CRhinoUiOptionsListCtrl::SetGridLineColor()
  //
  COLORREF SetLabelTextColor( COLORREF cr);
  //   
  // Description:
  //    Set X, Y and Z edit control values
  //
  // Parameters:
  //   pt [in] X, Y and Z values used to initialize edit controls
  //
  void SetPoint( ON_3dPoint pt);
  //   
  // Description:
  //    Set X, Y and Z edit control values
  //
  // Parameters:
  //   x [in] X value used to initialize edit control
  //   y [in] Y value used to initialize edit control
  //   z [in] Z value used to initialize edit control
  //
  void SetPoint( double x, double y, double z);
  //   
  // Description:
  //    Set value for X, Y or Z edit control.
  //
  // Parameters:
  //   ef [in] Field to set, see edit_fields enum for field IDs
  //   d [in] Value to place in edit control
  //
  void SetNumber( int ef, double d);
  //   
  // Description:
  //    Get current value of X, Y and Z edit controls
  //
  // Returns:
  //   Returns 3d point containing current edit control values
  //
  ON_3dPoint GetPoint() const;
  //   
  // Description:
  //    Get current value of X, Y and Z edit controls
  //
  // Parameters:
  //   x [out] Variable to hold the X edit control value
  //   y [out] Variable to hold the Y edit control value
  //   z [out] Variable to hold the Z edit control value
  //
  // Returns:
  //   Returns 
  //
  void GetPoint( double& x, double& y, double& z) const;
  //   
  // Description:
  //    Get current X, Y and Z edit control strings
  //
  // Parameters:
  //   x [out] Variable to hold the X edit control string value
  //   y [out] Variable to hold the Y edit control string value
  //   z [out] Variable to hold the Z edit control string value
  //
  void GetPoint( CString& x, CString& y, CString& z) const;
  //   
  // Description:
  //    Modify edit_field_styles associated with this control
  //
  // Parameters:
  //   remove [in] edit_field_styles Style flags to remove from control
  //   add [in] edit_field_styles Style flags to add to control
  //
  void ModifyEditFieldsStyle( int remove, int add);
  //   
  // Description:
  //    Get label associated with specified X, Y or Z edit control
  //
  // Parameters:
  //   ef [in] Edit field to get label for, see edit_field_styles for possible values
  //
  // Returns:
  //   Returns label associated with ef or NULL if ef is invalid
  //
  const wchar_t* EditFieldLabelText( int ef) const;
  //   
  // Description:
  //    Padding between labels and edit controls
  //
  // Returns:
  //   Returns padding between labels and edit controls
  //
  int TextPadding() const;
protected:
  //   
  // Description:
  //    Initialize m_redit and m_rtext arrays
  //
  // Parameters:
  //   dc [in] Device context used to compute text sizes
  //   rCtrl [in] Bounding rectangle, in dc coordinate system, for this control
  //
  void ComputeControlRects( CDC& dc, CRect rCtrl);
  //   
  // Description:
  //    Draw item labels to specified DC
  //
  // Parameters:
  //   dc [in] Device context to draw into
  //   rClient [in] Bounding rectangle for control elements in dc coordinate system
  //
  void DrawItemLabels( CDC& dc, CRect rClient) const;

protected:
  CRhinoUiEdit m_xyz[edit_wnd_cnt];
  CRect m_redit[edit_wnd_cnt];
  CRect m_rtext[edit_wnd_cnt];
  COLORREF m_crLabelBkGrndColor;
  COLORREF m_crLabelTextColor;
  int m_iEditFieldStyles;

public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);

  // CWnd overrides
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual BOOL32 OnCommand(WPARAM wParam, LPARAM lParam);
  virtual BOOL32 OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	DECLARE_MESSAGE_MAP()
};


