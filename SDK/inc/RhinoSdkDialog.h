//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
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

#if defined (RHINO_SDK_MFC)
class RHINO_SDK_FUNCTION CRhinoDialog : public CRhinoUiDialog
{
	DECLARE_DYNAMIC( CRhinoDialog)

  friend class CRhDialogControlBar;

public:
  // Returns true if a CRhinoDialog is running as a
  // semi-modal dialog (EnableDisplayCommands() is true)
  // and Rhino is currently waiting inside of DoModal()
  // waiting for the user to do something.
  static bool InDoSemiModal();

  // Description:
  //  Internal function used to check Rhino's CRhEventManager while a
  //  dialog is in a semi-modal state. Plug-ins should not have to call this
  //  function; it is exposed for the .NET API to do similar actions as
  //  CRhinoDialog's SetEnableDisplayCommands(true);
  // Returns:
  //  0 = nothing to do
  //  1 = enter event
  //  2 = cancel event
  static int CheckSemiModalMessagePump();

	// Modeless construct
	BOOL32 Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr) override;
	BOOL32 Create(UINT nIDTemplate, CWnd* pParentWnd = nullptr) override;

	// Modal construct
	CRhinoDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr);
	CRhinoDialog(UINT nIDTemplate, CWnd* pParentWnd = nullptr);
  virtual ~CRhinoDialog();

  bool EnableDisplayCommands() const;

  // Description:
  //  Call this method to get the document runtime serial number passed to
  //  SetEnableDisplayCommands
  // Returns:
  //  Returns the document runtime serial number passed to SetEnableDisplayCommands
  unsigned int DocumentSerialNumber() const;

  // Description:
  //  Call this method to enable view manipulation while a modal dialog is being
  //  displayed.
  void SetEnableDisplayCommands(bool b, unsigned int documentRuntimeSerialNumber);

  ON_DEPRECATED_MSG("use overload that takes document serial number")
  void SetEnableDisplayCommands(bool b);

  // Dialog Utilities
  void OnRhinoHelpButton(); // Display help for active Rhino command, call this from your
                            // help button.

  void Localize(); // automatically translates all dialog text to current language
                   // call at the end of OnInitDialog()

	// modal processing
  // Return values:
  //    IDOK     = OK button pressed
  //    IDCANCEL = Cancel or escape pressed
  //    IDABORT  = Command calling this dialog should abort.
  virtual INT_PTR DoModal() override;

  // Called by CRhColorButton if button clicked and color changed;
  virtual void OnColorButtonClicked( UINT nBtnID);

  UINT DialogID(){ return m_template_name_id;}
  LPCTSTR DialogStringID(){ return m_lpszTemplateName;}

  bool PushPickButton( bool hide_this_dialog);

  CSize CalcDialogSize( INT nResourceId);

  /*
  Description:
    Enables the size and position of the dialog to be stored in the Windows Registry.
    This allows a dialog's size and position to be remembered between uses.
  Parameters:
    bEnable        - [in] Enables or disables saving
    wRegistryTitle - [in] A name, or title, to be used to uniquely identify this
                          dialog in the registry. If omitted, the dialog's window
                          title is used. This parameter only needs to be specified
                          if the title of the dialog box will change between uses.
  See Also:
    CRhinoDialog::IsSaveDialogPositionAndSizeEnabled
  */
  void EnableSaveDialogPositionAndSize( bool bEnable = true, const wchar_t* wRegistryTitle = nullptr );

  /*
  Description:
    Verifies that the size and position of the dialog to be stored in the Windows Registry.
  Returns:
    TRUE or FALSE
  See Also:
    CRhinoDialog::EnableSaveDialogPositionAndSize
  */
  bool IsSaveDialogPositionAndSizeEnabled() const;

  /*
  Description:
    Call this method to see if a dialog is in a DoModal loop.
  Returns:
    TRUE or FALSE
  */
  bool InDoModal() const;

public:
  class CRhDialogSdkExtension* m__rhino_dialog_sdk_extension;

// Overrides
public:
  BOOL32 PreTranslateMessage(MSG* pMsg) override;
protected:
  void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
  virtual BOOL32 OnCommand(WPARAM wParam, LPARAM lParam);// override; //not an override on OSX
  LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;

// Implementation
protected:
#if defined (ON_RUNTIME_APPLE)
public:
  bool m_inUpdateData;
  bool m_dialogFinished;
  int m_osxrc;
  BOOL32 ShowWindow(int nCmdShow) override;
  BOOL32 DestroyWindow (void) override;
  BOOL32 UpdateData(BOOL32 toVars=TRUE) override;
	virtual void EndDialog(int rc);
  void OnClose(void);
  id DialogController();
#endif

	virtual BOOL32 ContinueModal();
	virtual void OnOK();
	virtual void OnCancel();

protected:
#if defined (ON_RUNTIME_APPLE)
public:
#endif
	// Generated message map functions
	//{{AFX_MSG(CRhinoDialog)
	virtual BOOL32 OnInitDialog() override;
	afx_msg void OnDestroy();
	afx_msg BOOL32 OnHelpInfo(HELPINFO* pHelpInfo);
  //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

#if defined (ON_RUNTIME_WIN)
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL32 bMinimized);
  afx_msg void OnActivateApp(BOOL32 bActive, DWORD dwThreadID);
  afx_msg void OnShowWindow(BOOL32 bShow, UINT nStatus);
#endif

private:
  CRhinoDialog();
  void Construct();
  void CheckRhinoMessagePump();
  void SaveDialogWindowPos();
  bool WindowPosRegKey( ON_wString& w);
  // Call from constructor or in OnInitDialog to restore saved position. If
  // no previous position saved then will center window.
  void RestorePreviousWindowPos();
  void UpdateTheme();

private:
  bool m_bSemiModal;
  bool m_bDoModal;
  bool m_bIDOK;
  HWND m_hWndSemiModalMain;
  bool m_restore_dlg_pos;
  bool m_dlg_pos_restored;
  bool m_enable_display_commands;
  HICON m_hIcon;
  CString m_sConstructName;
  ON_wString m_wRegistryTitle;
  // Pick button window restore list
  ON_ClassArray<HWND> m_wnd_list;
  ON_SimpleArray<int> m_visibility_list;
  unsigned int m_semi_modal_document_serial_number;
  ON__UINT_PTR m_sdk_reserved = 0;
};
#endif

//Interface class for forwarding dockbar events
class RHINO_SDK_CLASS IDockBarEventWatcher
{
public:
	virtual ~IDockBarEventWatcher() {}

	virtual bool OnDockContextStartDrag( bool bStart) = 0;
	virtual void OnToggleDocking( bool bStart) = 0;
	virtual void OnStartTracking( bool bDoneTracking) = 0;
	virtual void OnDockBarPositionChanged( DWORD dwNewLocation) = 0;

#pragma region RH_C_SHARED_ENUM [IDockBarEventWatcher::ShowEventArgs] [Rhino.UI.ShowPanelReason] [int]
	/// <summary>
	/// OnShowDockbar event type
	/// </summary>
	enum class ShowEventArgs : int
	{
		/// <summary>
		/// Dock bar shown or made visible
		/// </summary>
		Show,
		/// <summary>
		/// Dock bar hidden, no longer visible
		/// </summary>
		Hide,
		/// <summary>
		/// Dock bar temporarily hidden because the main Rhino application is no
		/// longer active.
		/// </summary>
		HideOnDeactivate,
		/// <summary>
		/// Dock bar that was temporarily hidden when the main Rhino application
		/// was deactivated is now being shown.
		/// </summary>
		ShowOnDeactivate
	};
#pragma endregion

	virtual void OnShowDockBar(ShowEventArgs args) = 0;

	// This was replaced by OnShowDockBar(CRhinoUiDockBar::ShowEventArgs args)
	// virtual void OnShowDockBar(bool bShowDockBar) = 0;
};

//Extension interface for forwarding tabbed dockbar events
class RHINO_SDK_CLASS ITabbedDockBarEventWatcher : public IDockBarEventWatcher
{
public:
	virtual ~ITabbedDockBarEventWatcher() {}

	virtual void OnShowTab(const class CRhinoUiPanel& panel, bool bShowTab, const ON_UUID& uuidDockBar) = 0;
};

//Derive from this class for dialogs that will appear as a tabbed dock
//bar.  Ensure you call one of the static Register functions during your
//CRhinoPlugIn::OnLoadPlugIn method.
//Note - your dialog must implement MFC dynamic creation using the DECLARE_DYNCREATE(MyDialog) macro
//in the header, and IMPLEMENT_DYNCREATE(MyDialog, CRhinoTabbedDockBarDialog) in the implementation file.
#if defined (RHINO_SDK_MFC)
class RHINO_SDK_CLASS CRhinoTabbedDockBarDialog : public CRhinoDialog,
										          public ITabbedDockBarEventWatcher
{
public:
	CRhinoTabbedDockBarDialog();

	DECLARE_DYNAMIC(CRhinoTabbedDockBarDialog)

	//Register your tab dialog with like this:
	//CRhinoTabbedDockBarDialog::Register(RUNTIME_CLASS(CMyTabDialog), IID, IDI_ICON, AfxGetStaticModuleState());
	static bool Register(CRuntimeClass* pRTC, UINT nIDTemplate, UINT idi, AFX_MODULE_STATE* pms);
	static bool Register(CRuntimeClass* pRTC, LPCTSTR lpszTemplateName, UINT idi, AFX_MODULE_STATE* pms);

	//Returns the dockbar that this tab is currently on.
	CRhinoUiDockBar* DockBar(void) const;

protected:
	virtual BOOL32 OnInitDialog() override;
	virtual BOOL32 PreTranslateMessage(MSG* pMsg) override;

public:
	//Events, forwarded from CRhinoUiDockBar
	bool OnDockContextStartDrag( bool bStart) override;
	void OnToggleDocking( bool bStart) override;
	void OnStartTracking( bool bDoneTracking) override;
	void OnDockBarPositionChanged( DWORD dwNewLocation) override;

  void OnShowDockBar(IDockBarEventWatcher::ShowEventArgs args) override;
  void OnShowTab(const class CRhinoUiPanel& db, bool bShowTab, const ON_UUID& dockBarId) override;

  ON_DEPRECATED_MSG("Replaced by CRhinoTabbedDockBarDialog::OnShowDockBar(IDockBarEventWatcher::ShowEventArgs)")
  virtual void OnShowDockBar( bool bShowDockBar);

public:
	//Required overrides
	virtual const wchar_t* Caption(void) const	= 0;

	//Generate a unique ID for your tab and return it here.
	virtual ON_UUID TabId(void) const			= 0;
	virtual ON_UUID PlugInId(void) const		= 0;

public:
	// Utility functions to manipulate the tabs.
	static void OpenDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidPanelId, bool bSelect = true, const ON_UUID* uuidDockBar = nullptr);
	static void CreateTabQuietly(const CRhinoDoc& doc, const ON_UUID& uuidPanelId);
	ON_DEPRECATED_MSG("Use the version with bMakeSelected") static void ShowDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidPanelId, bool bShow, const ON_UUID* uuidDockBar);
	static void ShowDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidPanelId, bool bShow, bool bMakeSelected, const ON_UUID* uuidDockBar);
	static void CloseDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidPanelId, const ON_UUID* uuidDockBar);

	static bool IsTabVisible(const CRhinoDoc& doc, const ON_UUID& uuidPanelId, bool bIsSelectedTab = false);

	ON_DEPRECATED_MSG("CRhinoTabbedDockBarDialog::DockBarForTab is obsolete. Use multi-doc capable CRhinoTabbedDockBarDialog::DockBarsForTab (s after Bar).")
	static CRhinoUiDockBar* DockBarForTab(const CRhinoDoc& doc, const ON_UUID& uuidPanelId);

	static void DockBarsForTab(const CRhinoDoc& doc, const ON_UUID& uuidPanelId, ON_SimpleArray<CRhinoUiDockBar*>& dockbars);

	static bool OpenTabOnDockBar(CRhinoUiDockBar* pDockbar, const CRhinoDoc& doc, const ON_UUID& uuidPanelId, bool makeSelectedTab = true);	//If nullptr == pDockbar, a new dockbar is created for the tab

	static void GetOpenTabIds(ON_SimpleArray<ON_UUID>& tabIds);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void* EVF(const wchar_t* wszFunc, const void* pvData);

public:
	class CRhTabbedDockBarDialogSdkExtension* m__rhino_dialog_sdk_extension;
	ON_UUID m_dockbar_id = ON_nil_uuid;
};
#else
#define CRhinoDialog CDialog
#define CRhinoUiDialog CDialog
#endif
