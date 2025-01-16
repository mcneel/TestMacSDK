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

class CRhinoUiDialog;
class CRhinoUiDHtmlDialog;

class RHINO_SDK_CLASS CRhinoUiLocalizeDialog
{
public:
  // This class if FOR INTERNAL USE ONLY!!
  CRhinoUiLocalizeDialog( class CRhinoUiDialog*);
  CRhinoUiLocalizeDialog( class CRhinoUiDHtmlDialog*);
  CRhinoUiLocalizeDialog( UINT nDialogID );

  void Localize();
  void Localize(UUID idPlugIn, HWND hWnd, UINT nDialogID);
  bool HasBeenLocalized();

  HWND DialogHWnd() const;
  UUID PlugInUUID() const;
  UINT DialogTemplateID() const;

private:
  CRhinoUiDialog* m_dlg;
  CRhinoUiDHtmlDialog* m_html_dlg;
  bool m_bLocalized;
};
//   
// Description:
//   This class is provide to allow windows or dialogs that have a module state which is
//   is not equal to RhinoApp().RhinoModuleState() to get PreTranslateMessage() notification.
//   If you derive your class from CRhinoUiDialog you will not need to use this class, if
//   you use a dialog from an extension DLL or whish to derive from CWnd and create a top
//   level window you should add an instance of this class to your derived class
//   class CMyWnd : public CWnd
//   {
//     ...
//     CRhinoUiRegisterPreTranslateMessage m_rptm;
//     ...
//   };
//   Class constructor:
//     CMyWnd : m_rptm( ::AfxGetStaticModuleState())
//     {
//       ...
//     }
//   This will allow your window to receive PreTranslateMessage() calls.  Another option is
//   to add the member to your CRhino....PlugIn derived class and initialize in the same manner
//
class RHINO_SDK_CLASS CRhinoUiRegisterPreTranslateMessage
{
public:
  CRhinoUiRegisterPreTranslateMessage( AFX_MODULE_STATE* pMS);
  ~CRhinoUiRegisterPreTranslateMessage();

private:
  AFX_MODULE_STATE* m_pMS;
  class CRhinoUiRegisterPreTranslateMessage__Data* m__pData;
};

// CRhinoUiDialog dialog
class RHINO_SDK_CLASS CRhinoUiDialog : public CDialog
{
	DECLARE_DYNCREATE(CRhinoUiDialog)
public:
  //   
  // Description:
  //   Constructors
  //
  // Parameters:
  //    lpszTemplateName [in] Contains a null-terminated string that is the name of a dialog-box
  //                          template resource. 
  //    nIDTemplate [in] Contains the identifier of a dialog-box template resource. 
  //    pParentWnd [in] Pointer to the parent or owner window object of type CWnd to which the dialog
  //                    object belongs. If it is NULL, the dialog object parent window is set to the
  //                    main application window. 
  // See Also:
  //   CDialog
  //   
  CRhinoUiDialog( LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
	CRhinoUiDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL);
  // Modeless construct
  virtual BOOL32 Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
  virtual BOOL32 Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual BOOL32 CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd = NULL, void* lpDialogInit = NULL);
	virtual BOOL32 CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd = NULL);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiDialog();
  //   
  // Description:
  //   Template name passed to dialog constructor.
  //
  // Returns:
  //   Return template name passed to dialog constructor.
  //
  // See Also:
  //   CRhinoUiDialog::CRhinoUiDialog()
  //   
  LPCTSTR DialogTemplateName() const;
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
  //   Return the module state that was current when this dialog
  //   object was created.
  //
  // Returns:
  //   Return the module state that was current when this dialog
  //   object was created.
  //
  AFX_MODULE_STATE* DialogModuleState();
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
  //   CRhinoUiDialog::SetAllowEscapeAndEnter()
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
  //   CRhinoUiDialog::AllowEscapeAndEnter()
  //   
  bool SetAllowEscapeAndEnter( bool b);
  //   
  // Description:
  //  Override this method to allow the escape and enter keys to get
  //  forwarded to multi-line controls like rich text boxes in hosted
  //  user controls.
  //
  // Parameters:
  //  msg [in] Value passed to CRhinoUiDialog::PreTranslate message.
  //    Look at msg->wParam to see if it is equal to VK_ENTER or
  //    VK_ESCAPE.  msg->hWnd Will contain the window handle of the
  //    target control.
  //
  // Returns:
  //  true
  //    Disregard the AllowEscapeAndEnter() and forward the message.
  //  false
  //    Go ahead and suppress the message.
  //
  // See Also:
  //   CRhinoUiDialog::AllowEscapeAndEnter()
  //   CRhinoUiDialog::SetAllowEscapeAndEnter( bool b)
  virtual bool OverrideAllowEscapeAndEnter(MSG* msg);
  //
  // Description:
  //   If AllowEscapeAndEnter is set to true, the dialog will not close when these
  //   keys are pressed. If there is no control on the dialog that should receive
  //   these characters, they are "eaten" by the dailog and nothing happens.
  //   This function allows you to perform some action when in the case where the
  //   enter and escape would be "eaten".
  //
  // Parameters:
  //   nChar [in] either VK_ESCAPE or VK_RETURN
  //
  // See Also:
  //   CRhinoUiDialog::AllowEscapeAndEnter()
  //   CRhinoUiDialog::SetAllowEscapeAndEnter( bool b)
  virtual void OnUnhandledEscapeAndEnter( UINT nChar );
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
  //   CRhinoUiDialog::ForwardLMouseDblClickMessagesToParent()
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
  //   CRhinoUiDialog::ForwardLMouseDownMessagesToParent()
  //   
  virtual bool ForwardLMouseDblClickMessagesToParent( CPoint point);
  //   
  // Description:
  //   Retrieves a pointer to the specified control or child window in this
  //   dialog box and enables or disables the control window based on the
  //   value of the bEnable argument.
  //
  // Parameters:
  //   nItemID [in] Specifies the identifier of the control to be enabled or disabled 
  //   bEnable [in] Specifies whether to enable or disable the window. If this parameter
  //                is true, the window is enabled. If the parameter is false, the window
  //                is disabled. 
  //
  // Returns:
  //   Returns true if nItemID is a valid control identifier otherwise returns false.
  //
  // See Also:
  //   CRhinoUiDialog::EnableDialogItems()
  //   CRhinoUiDialog::IsDialogItemEnabled()
  //   
  bool EnableDialogItem( UINT nItemID, bool bEnable);
  //   
  // Description:
  //   Retrieves a pointer to the specified controls or child windows in this
  //   dialog box and enables or disables the control windows based on the
  //   value of the bEnable argument.
  //
  // Parameters:
  //   nItemID [in] Specifies the identifiers of the controls to be enabled or disabled.
  //   iArraySize [in] Specifies the size of the nItemID array.
  //   bEnable [in] Specifies whether to enable or disable the windows. If this parameter
  //                is true, the windows are enabled. If the parameter is false, the windows
  //                are disabled. 
  //
  // Returns:
  //   Returns true if nItemID is a valid control identifier otherwise returns false.
  //
  // See Also:
  //   CRhinoUiDialog::EnableDialogItem()
  //   CRhinoUiDialog::IsDialogItemEnabled()
  //   
  int EnableDialogItems( UINT* nItemIDArray, int iArraySize, bool bEnable);
  //
  // Description:
  //   Retrieves a pointer to the specified controls or child windows in this
  //   dialog box and enables or disables the control windows based on the
  //   value of the bEnable argument.
  //
  // Parameters:
  //   nItemID [in] Specifies the identifiers of the controls to be enabled or disabled.
  //   bEnable [in] Specifies whether to enable or disable the windows. If this parameter
  //                is true, the windows are enabled. If the parameter is false, the windows
  //                are disabled. 
  //
  // Returns:
  //   Returns true if nItemID is a valid control identifier otherwise returns false.
  //
  // See Also:
  //   CRhinoUiDialog::EnableDialogItem()
  //   CRhinoUiDialog::IsDialogItemEnabled()
  //
  int EnableDialogItems( CUIntArray& nItemIDArray, bool bEnable);
  //
  // Description:
  //   Retrieves a pointer to the specified control or child window in this
  //   dialog box returns true if it found and is enabled or false if the
  //   control is not found or the enabled.
  //   value of the bEnable argument.
  //
  // Parameters:
  //   nItemID [in] Specifies the identifier of the control to check.
  //
  // Returns:
  //   Returns true if nItemID is a valid control identifier and is enabled
  //   otherwise returns false.
  //
  // See Also:
  //   CRhinoUiDialog::EnableDialogItems()
  //   CRhinoUiDialog::EnableDialogItem()
  //
  bool IsDialogItemEnabled( UINT nItemID) const;
  //
  // Description:
  //   Call this method to temporarily disable every control in this dialog.
  //   Prior to disabling a control the controls enabled state is pushed to
  //   a restore list.  The control enabed state list is used by the
  //   RestoreAllDisabledDialogItems() method to return the controls to there
  //   original state.
  //
  // See Also:
  //   CRhinoUiDialog::RestoreAllDisabledDialogItems()
  //
  void DisableAllDialogItems();
  //
  // Description:
  //   Call this method to return dialog controls to there original enabled state.
  //   This method will only do something if DisableAllDialogItems() was previously
  //   called.
  //
  // See Also:
  //   CRhinoUiDialog::DisableAllDialogItems()
  //
  void RestoreAllDisabledDialogItems();
  //
  // Description:
  //   Call this method to enable or disable every control in this dialog.
  //
  void EnableAllDlgControls( bool bEnable);
  // Reserved for future use
  enum rhino_dialog_flags
  {
    // If you are a dialog that will be embedded in a docking windows,
    // set this flag in our dialog's constructor.
    is_dockbar_dialog = 1,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_rhino_flag = 0xFFFFFFFF
  };
  //
  // Description:
  //   Call this method to get the current flag settings.  This is provided
  //   as a way of adding functionality to this class without breaking the sdk.
  //
  // Returns:
  //   Returns current flags value
  //
  // See Also:
  //   CRhinoUiDialog::SetRhinoFlags()
  //   CRhinoUiDialog::IsRhinoFlagSet()
  //   CRhinoUiDialog::SetRhinoFlag()
  //
  int RhinoFlags() const;
  //
  // Description:
  //   Call this method to set multiple rhino flags.
  //
  // Parameters:
  //   iFlags [in] Specifies the new flags value.
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiDialog::RhinoFlags()
  //   CRhinoUiDialog::IsRhinoFlagSet()
  //   CRhinoUiDialog::SetRhinoFlag()
  //
  int SetRhinoFlags( int iFlags);
  //
  // Description:
  //   Call this method to determine if a flag is currently set.
  //
  // Parameters:
  //   flag [in] Specifies the identifier of the flag to check.
  //
  // Returns:
  //   Returns true if the flag is set otherwise returns false.
  //
  // See Also:
  //   CRhinoUiDialog::SetRhinoFlags()
  //   CRhinoUiDialog::RhinoFlags()
  //   CRhinoUiDialog::SetRhinoFlag()
  //
  bool IsRhinoFlagSet( rhino_dialog_flags flag) const;
  //
  // Description:
  //   Call this method to set a specific flag value.
  //
  // Parameters:
  //   flag [in] Flag to set
  //   b [in] New value for the flag
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiDialog::SetRhinoFlags()
  //   CRhinoUiDialog::IsRhinoFlagSet()
  //   CRhinoUiDialog::RhinoFlags()
  //
  bool SetRhinoFlag( rhino_dialog_flags flag, bool b);
////////////////////////////////////////////////////////////////////////
// Ported from CRhinoUiDialog
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
  friend class CRhinoUiLocalizeDialog;
  friend class CRhinoUiDialogItemResizer;

protected:
  void Construct();
  bool _SetDlgItemToolTip( CWnd* pWndCtrl, LPCTSTR lpsToolTip);
	BOOL32 RhCreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd, HINSTANCE hInst);
  BOOL32 RhCreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd, HINSTANCE hInst);
	BOOL32 RhCreateIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit, HINSTANCE hInst);
	INT_PTR RhDoModal();

public:
	INT_PTR DoModal();

public:
  CRhinoUiDialogItemResizer m_Resize;
  CRhinoUiLocalizeDialog m_localize; // For internal use only
  bool m_allow_auto_resize;

protected:
  CString m_template_name_string;
  UINT m_template_name_id;
  UUID m_plugin_id;
  bool m_allow_escape_and_enter;
	CRect m_rcPage;
  CSize m_sizeGripper;
  CRect m_rcGripper;
  CSize m_sizeMin; // Defaults to 0,0 which means no clamp, if other than 0,0 use this instead
  CSize m_sizeMax; // Defaults to 0,0 which means no clamp, if other than 0,0 use this instead
  bool m_bAddResizer;
  CToolTipCtrl m_tool_tip;
  bool m_activate_tool_tip;
  CWnd* m_tool_tip_owner;
#if defined (ON_RUNTIME_WIN)
  ON_SimpleArray<HWND>m_enable_child_list;
#endif
#if defined (ON_RUNTIME_APPLE)
  // These are Objective-C id objects on the Mac
  ON_ClassArray<HWND>m_enable_child_list;
#endif
  int m_rhino_flags;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
  afx_msg LRESULT OnNcHitTest(CPoint point);
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual BOOL32 OnInitDialog();
protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


// This helper class will add scroll bars to any dialog.
//
// References:
//   MSDN ID 262954: How to create a resizable dialog box with scroll bars in Visual C++. 
//   MSDN ID 152252: How To Get 32-bit Scroll Position During Scroll Messages.
//   "Programming Windows with MFC, 2nd Edition", by Jeff Prosise.
//
// Remarks:
//   Make sure your dialog has the WS_HSCROLL and/or WS_VSCROLL styles.
//   Your dialog should also have a "Resizing" or "None" border.

class RHINO_SDK_CLASS CRhinoUiDialogScrollHelper
{
public:
  CRhinoUiDialogScrollHelper();
  ~CRhinoUiDialogScrollHelper();

  // Call this in your dialog's constructor
  // For example:
  //   m_scroller = new CRhinoUiDialogScrollHelper();
  //   if( m_scroller )
  //     m_scroller->Attach( this );
  //
  void Attach(CWnd* pWnd);
  void Detach();

  // Set the base display size. When the dialog size is smaller than the display size,
  // then scrollbars will appear. Set either the display width or height to zero if you
  // don't want to enable the scroll bar in that direction.
  // Best to call this in your OnInitDialog() member before calling the base class.
  // For example:
  //   if( m_scroller )
  //   {
  //     CRect rect;
  //     GetClientRect( rect );
  //     m_scroller->SetDisplaySize( rect );
  //   }
  //
  void SetDisplaySize(CRect rect);
  void SetDisplaySize(int width, int height);
  CSize DisplaySize() const;

  // Returns the current scroll position. This might be in OnPaint() overrides.
  CSize ScrollPos() const;
  // Returns the current page size.
  CSize PageSize() const;

  // Scroll back to top, left, or top-left corner of the window.
  void ScrollToOrigin(bool bScrollLeft, bool bScrollTop);

  // Enables or disables the scroll bars (without hiding them).
  void EnableScrollBars(bool bEnable = true);
  void EnableScrollBar(int nBar, bool bEnable = true);

  // Your dialog should implement these message handlers, and them forward
  // those messages to these handlers.
  void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  void OnSize(UINT nType, int cx, int cy);

private:
  int Get32BitScrollPos(int nBar, CScrollBar* pScrollBar);
  void UpdateScrollInfo();
  void UpdateScrollBar(int nBar, int window_size, int display_size, LONG& page_size, LONG& scroll_pos, LONG& delta_pos);

private:
  CWnd* m_pWnd;
  CSize m_page_size;
  CSize m_display_size;
  CSize m_scroll_pos;
};
