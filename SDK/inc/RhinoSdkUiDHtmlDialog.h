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


// CRhinoUiDHtmlDialog dialog
#ifdef DECLARE_DHTML_EVENT_MAP
class RHINO_SDK_CLASS CRhinoUiDHtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CRhinoUiDHtmlDialog)

public:
  //   
  // Description:
  //   Constructors
  //
  // Parameters:
  //   lpszTemplateName [in] The null-terminated string that is the name of a dialog-box
  //                         template resource. 
  //   szHtmlResID [in] The null-terminated string that is the name of an HTML resource. 
  //   pParentWnd [in] A pointer to the parent or owner window object (of type CWnd) to which
  //                   the dialog object belongs. If it is NULL, the dialog object's parent
  //                   window is set to the main application window.
  //   nIDTemplate [in] Contains the ID number of a dialog-box template resource. 
  //   nHtmlResID [in] Contains the ID number of an HTML resource. 
  //
  // See Also:
  //   CDHtmlDialog
  //   
	CRhinoUiDHtmlDialog(UINT nIDTemplate = 0, UINT nHtmlResID = 0, CWnd *pParentWnd = NULL);
	CRhinoUiDHtmlDialog(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd *pParentWnd = NULL);
  // Modeless construct
  virtual BOOL32 Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
  virtual BOOL32 Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiDHtmlDialog();
  //   
  // Description:
  //   Template ID passed to dialog constructor.
  //
  // Returns:
  //   Return template ID passed to dialog constructor.
  //
  // See Also:
  //   CRhinoUiDialog::CRhinoUiDialog()
  //   
  UINT DialogTemplateID() const;
  //   
  // Description:
  //   When the left mouse button goes down over client area of the dialog
  //   but not down on a control the dialog can forward the message to its
  //   parent window.  You would want to do this if you dialog was modless
  //   and embedded in a container that handles mouse down events.
  //
  // Parameters:
  //   point [in] Specifies the x- and y-coordinate of the cursor. These 
  //              coordinates are always relative to the upper-left corner
  //              of the window. 
  //
  // Returns:
  //   The default behavior is to return false.  To change this you will
  //   need to derive a new class from this one and override this method.
  //
  // See Also:
  //   CRhinoUiDHtmlDialog::ForwardLMouseDblClickMessagesToParent()
  //   
  virtual bool ForwardLMouseDownMessagesToParent( CPoint point);
  //   
  // Description:
  //   When the left mouse button is double clicked on the client area
  //   but not down on a control the dialog can forward the message to its
  //   parent window.  You would want to do this if you dialog was modless
  //   and embedded in a container that handles mouse down events.
  //
  // Parameters:
  //   point [in] Specifies the x- and y-coordinate of the cursor. These 
  //              coordinates are always relative to the upper-left corner
  //              of the window. 
  //
  // Returns:
  //   The default behavior is to return false.  To change this you will
  //   need to derive a new class from this one and override this method.
  //
  // See Also:
  //   CRhinoUiDHtmlDialog::ForwardLMouseDownMessagesToParent()
  //   
  virtual bool ForwardLMouseDblClickMessagesToParent( CPoint point);
  //   
  // Description:
  //   If true allow escape and enter keys to close dialog box.  The default
  //   value allows escape and enter keys to close the dialog.  Use 
  //   SetAllowEscapeAndEnter( false) to change the default.
  //
  // Returns:
  //   If true escape and enter keys will close the dialog
  //
  // See Also:
  //   CRhinoUiDHtmlDialog::SetAllowEscapeAndEnter()
  //   
  bool AllowEscapeAndEnter() const;
  //   
  // Description:
  //   Set this to true if you do not want the escape and enter
  //   keys to close this dialog.  You will want to do this when
  //   embedding a modless dialog in a control bar.
  //
  // Parameters:
  //   b [in] If true then escape and enter keys will close the dialog
  //          box otherwise nothing will happen when these keys are pressed.
  //
  // Returns:
  //   Previous value for AllowEscapeAndEnter()
  //
  // See Also:
  //   CRhinoUiDHtmlDialog::AllowEscapeAndEnter()
  //   
  bool SetAllowEscapeAndEnter( bool b);

////////////////////////////////////////////////////////////////////////
// Ported from CRhinoUiDHtmlDialog
public:
  void SetToolTipOwner( CWnd* pWnd);
  bool SetDlgItemToolTip( UINT nID, const char* lpsToolTip);
  bool SetDlgItemToolTip( UINT nID, const wchar_t* lpsToolTip);
  bool SetDlgItemToolTip( CWnd* pWndCtrl, const char* lpsToolTip);
  bool SetDlgItemToolTip( CWnd* pWndCtrl, const wchar_t* lpsToolTip);
  bool AddResizeControl( bool b);
  bool IsResizable();
  bool SetMinSizeToCurrentSize();
  // If Windows supports gradient window captions and they are enabled then 
  // gradient fill the specified rectangle otherwise solid fill it.  If lpsCaption
  // points to a string then paint it left justified and vertically center 
  // justified.
  void PaintGradientTitleRect( CDC* pDC, LPRECT lpRect, const char* lpsCaption = 0, bool bActiveWindow = TRUE);
  void PaintGradientTitleRect( CDC* pDC, LPRECT lpRect, const wchar_t* lpsCaption = 0, bool bActiveWindow = TRUE);
  void CallOnUpdateUi();
  bool GetDlgItemRect( UINT nID, CRect& rc);

protected:
  void Construct();
  bool _SetDlgItemToolTip( CWnd* pWndCtrl, LPCTSTR lpsToolTip);

protected:
  friend class CRhinoUiLocalizeDialog;

public:
  CRhinoUiDialogItemResizer m_Resize;
  CRhinoUiLocalizeDialog m_localize; // For internal use only
protected:
  bool m_allow_escape_and_enter;
  UINT m_template_name_id;
  UUID m_plugin_id;
	CRect m_rcPage;
  CSize m_sizeGripper;
  CRect m_rcGripper;
  CSize m_sizeMin; // Defaults to 0,0 which means no clamp, if other than 0,0 use this instead
  CSize m_sizeMax; // Defaults to 0,0 which means no clamp, if other than 0,0 use this instead
  bool m_bAddResizer;
  CToolTipCtrl m_tool_tip;
  bool m_activate_tool_tip;
  CWnd* m_tool_tip_owner;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL32 OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
  afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
};
#endif
