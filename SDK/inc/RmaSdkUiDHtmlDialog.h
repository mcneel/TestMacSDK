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

#ifdef DECLARE_DHTML_EVENT_MAP
class RHINO_SDK_CLASS CRmaUiDHtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CRmaUiDHtmlDialog)
private:
  void Construct();
public:
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
	CRmaUiDHtmlDialog();
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
	CRmaUiDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID, CWnd *pParentWnd = NULL);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
	CRmaUiDHtmlDialog(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd *pParentWnd = NULL);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  virtual ~CRmaUiDHtmlDialog();
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  // If Windows supports gradient window captions and they are enabled then 
  // gradient fill the specified rectangle otherwise solid fill it.  If lpsCaption
  // points to a string then paint it left justified and vertically center 
  // justified.
  void PaintGradientTitleRect( CDC* pDC, LPRECT lpRect, LPCTSTR lpsCaption = 0, BOOL32 bActiveWindow = TRUE);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  COLORREF Dither( COLORREF crFrom, COLORREF crTo, int nStep, int nSteps);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  // If dialog has a resizable frame use this interface to add controls
  // to the resize list.  This will automatically resize specified controls.
  // Example: lock the bottom right corner of the IDC_BUTTON control
  //     m_Resize.Add( IDC_BUTTON, CRmaUiDialogItemResizer::resize_lockbottomright);
  CRmaUiDialogItemResizer m_Resize;
protected:
	CRect        m_rcPage;
  CSize        m_sizeGripper;
  CRect        m_rcGripper;
  CSize        m_sizeMin; // Defaults to 0,0 which means clamp to initial size, if other than 0,0 use this instead
  BOOL32         m_bAddResizer;
  CToolTipCtrl m_tool_tip;
  BOOL32         m_activate_tool_tip;
  CWnd*        m_tool_tip_owner;
//Methods
public:
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  // Returns true if dialog has a sizing frame otherwise false
  BOOL32 AddResizeControl( BOOL32 b);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  BOOL32 IsResizable();
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  BOOL32 GetDlgItemRect( UINT nID, CRect& rc);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  void SetToolTipOwner( CWnd* pWnd);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  BOOL32 SetDlgItemToolTip( UINT nID, LPCTSTR lpsToolTip);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  BOOL32 SetDlgItemToolTip( CWnd* pWndCtrl, LPCTSTR lpsToolTip);
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  BOOL32 SetMinSizeToCurrentSize();
  //   
  // Description:
  //   
  //
  // Parameters:
  //   
  //
  // Returns:
  //   
  //
  // See Also:
  //   
  //   
  void CallOnUpdateUi();
  
// Overrides

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL32 OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg UINT OnNcHitTest(CPoint point);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
#endif
