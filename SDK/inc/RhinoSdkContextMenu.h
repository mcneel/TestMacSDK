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

#if defined (RHINO_SDK_MFC)

// Description:
//   Helper class used to temporarily attached a CMenu to a HMENU then detach
//   when done.
class RHINO_SDK_CLASS CRhinoTempAttachCMenu : public CMenu
{
public:
	// Description:
	//   Class constructor
  // Parameters:
  //   hMenu [in] Menu to attach to
  CRhinoTempAttachCMenu( HMENU hMenu);

  virtual ~CRhinoTempAttachCMenu();

private:
  bool m_bDetachMenu;
  // Not supported
  CRhinoTempAttachCMenu( const CRhinoTempAttachCMenu&) = delete;
  const CRhinoTempAttachCMenu& operator=( const CRhinoTempAttachCMenu&) = delete;
};

//
// Description:
//   Used to display a popup menu at a specified location. Wrapper for TrackPopupMenuEx()
class RHINO_SDK_CLASS CRhinoPopupMenu
{
public:
	// Description:
	//   Class constructor
  // Parameters:
  //   pModuleState [in] Module state for the plug-in that owns the context menu
  //   nResourceID  [in] Resource ID for menu to be loaded, if this is 0 then no menu will be loaded
  //   hInst        [in] Only used if nResourceID is greater than 0, if it is then the menu is loaded using this resource handle
  //   iSubMenu     [in] Sub menu from loaded menu to be displayed
  CRhinoPopupMenu( AFX_MODULE_STATE* pModuleState, UINT nResourceID = 0, HINSTANCE hInst = nullptr, int iSubMenu = 0);

  // Description:
	//   Class constructor
  // Parameters:
  //   pModuleState  [in] Module state for the plug-in that owns the context menu
  //   lpsResourceID [in] Resource ID for menu to be loaded, if this is a NULL string then no menu will be loaded
  //   hInst         [in] Only used if lpsResourceID is greater than 0, if it is then the menu is loaded using this resource handle
  //   iSubMenu      [in] Sub menu from loaded menu to be displayed
  CRhinoPopupMenu( AFX_MODULE_STATE* pModuleState, LPCTSTR lpsResourceID, HINSTANCE hInst = nullptr, int iSubMenu = 0);

  virtual ~CRhinoPopupMenu();


  // Description:
	//   Add a new string or bitmap item to the end of the popup menu.
  // Parameters:
  //   lps [in] String to add
  //   hBitmap [in] Handle to the bitmap to be displayed or standard bitmap.  See hbmpItem in the MENUITEMINFO structure definition
  //                in the MSDN help file for details.  This bitmap will be copied so it is up to you to delete it when you are done.
  //   hBitmapChecked [in] Handle to the bitmap to display next to the item if it is selected. If this member is NULL, a default bitmap
  //                       is used.
  //   hBitmapUnChecked [in] Handle to the bitmap to display next to the item if it is not selected. If this member is NULL, no bitmap
  //                         is used.
  // Returns:
  //   Returns value < 0 on error otherwise item reference index
  int AddItem( const wchar_t* lps, HBITMAP hBitmap = nullptr, HBITMAP hBitmapChecked = nullptr, HBITMAP hBitmapUnChecked = nullptr);

  // Description:
	//   Add a sub menu to the context menu.  This menu will be cloned and uniquie runtime menu IDs used.  You will need to use
  //   RuntimePopupMenuItemID() to get convert original item IDs to runtime IDs.
  // Parameters:
  //   lps [in] String to be displayed for the sub menu
  //   hMenu [in] Handle to a menu which will be cloned and appended to the menu
  // Returns:
  //   Returns value < 0 on error otherwise item reference index
  int AddItem( const wchar_t* lps, HMENU hMenu);

  // Description:
	//   Append a separator to the menu
  void AddSeparator();

  // Description:
	//   Convert original sub menu ID to runtime ID which was used in this menu.  This is useful when initializing the menu at runtime.
  // Parameters:
  //   hMenuOriginal [in] Menu handle passed to AddItem() to add the sub menu
  //   nOriginalID [in] Original menu item ID
  // Returns:
  //   Returns the runtime ID for the menu item or 0 if not found.
  UINT RuntimePopupMenuItemID( HMENU hMenuOriginal, UINT nOriginalID);

  // Description:
	//   Enable or disable item added to this menu
  // Parameters:
  //   nItem [in] Item reference index returned by AddItem()
  //   bEnable [in] If true the menu item will be enabled otherwise it will be disabled and grayed out.  By default all added items
  //                are enabled.
  void EnableItem( int nItem, bool bEnable = true);

  UINT ItemId(int nItem);

  int ItemReferenceIndexFromId(UINT nId);

  // Description:
	//   Check or un-check a menu item
  // Parameters:
  //   nItem [in] Item reference index returned by AddItem()
  //   bCheck [in] If true then the item will be checked otherwise it will be un-checked.
  void CheckItem( int nItem, bool bCheck = true);

  // Description:
	//   Set status bar help text
  // Parameters:
  //   nItem [in] Item reference index returned by AddItem()
  //   lps [in] Text to be displayed if status bar is synchronized
  void SetStatusBarHelpText( int nItem, const wchar_t* lps);

  // Description:
	//   Returns the number of items currently added to the menu, this includes separators and sub menus.
  int ItemCount() const;

  // Description:
	//   Override this method to get notified on WM_MENUSELECT messages
  // Parameters:
  //   hWnd [in] Window the WM_MENUSELECT was sent to
  //   hMenu [in] Handle to the menu which generated the message
  //   index [in] Zero based index to the selected item
  virtual void OnMenuSelect( HWND hWnd, HMENU hMenu, int index);

  // Description:
  //   Called when WM_INITMENU message received for context menu.
  // Parameters:
  //   hWnd [in] Window that received the message
  //   hMenu [in] Context menu
  virtual void OnInitMenu( HWND hWnd, HMENU hMenu);

  // Description:
	//   Display and track the popup menu.
  // Parameters:
  //   hWndParent [in] Handle to the window which owns the menu and the coordinate system for the display points
  //   ptBottom [in] Bottom point.  Top left corner of the popup.  This is used by default if there is enough
  //                 screen space to display the popup menu on the bottom
  //   ptTop [in] Top point.  Bottom left corner of the poupup.  This is used if there is not enough space to display
  //              the menu below the window.
  //   iTransparent [in] 0 = Not transparent, 1 to 100 = percentage of tranparency
  // Returns:
  //   Returns the index for the selected item which was added by calling AddItem or -1 if canceled.  If using a loaded
  //   menu then WM_COMMAND messages are sent to hWndParent and 0 is returned.
  virtual int PopUpEx( HWND hWndParent, // 
                       CPoint ptBottom, // Bottom point.  Top left corner of the popup.  This is used by default if there is enough
                                        // screen space to display the popup menu on the bottom
                       CPoint ptTop,    // Top point.  Bottom left corner of the poupup.  This is used if there is not enough space to display
                                        // the menu below the window.
                       int iTransparent = 0); // 0 = Not transparent, 1 to 100 = percentage of tranparency.

  // Description:
	//   Returns true if the menu was closed as a result of a left mouse button click.
  bool LeftButtonPicked();

  // Description:
	//   Returns true if the menu was closed as a result of a right mouse button click.
  bool RightButtonPicked();

  // Description:
	//   Return NULL if LeftButtonPicked() and RightButtonPicked() are false otherwise returns point where mouse went down. 
  LPPOINT PointPicked();

  // Description:
	//   Return NULL if left or right button picked is not true otherwise return pointer to message structure for message.
  MSG* LastMessage();

  // Description:
	//   This will be used to extend this class without breaking the SDK and is currently not called
	virtual LRESULT MenuProc(UINT message, WPARAM wParam, LPARAM lParam);

  // Description:
	//   Menu handle for the context menu to be displayed
  HMENU GetMenuHandle();

  // Description:
	//   Rectangle to exclude when positioning the menu, in screen coordinates.  This is an empty rectangle by default and
  //   gets ignored.
  // See Also:
  //   TPMPARAMS Structure in MSDN help
  CRect RectToAvoid() const;

  // Description:
	//   Rectangle to exclude when positioning the menu, in screen coordinates.  Set this to NULL or an empty rectangle to 
  //   disable this feature.
  // Parameters:
  //   lpRect [in] If non-null and not an empty rectangle it is passed to TrackPopupMenuEx() otherwise it is not
  // See Also:
  //   TrackPopumMenuEx() or TPMPARAMS structure in MSDN help
  void SetRectToAvoid( LPRECT lpRect);

protected:
  friend class CRhPopupMenu;

  class CRhPopupMenu* m_pHelper = nullptr;

private:
  void* m_reserved = nullptr;
  // Not supported
  CRhinoPopupMenu( const CRhinoPopupMenu&) = delete;
  const CRhinoPopupMenu& operator=( const CRhinoPopupMenu&) = delete;
};


// Description:
//   Context of a context menu. Used by extensions to decide which items to
//   add to a context menu.
class RHINO_SDK_CLASS CRhinoContextMenuContext
{
public:
  CRhinoContextMenuContext();
  ~CRhinoContextMenuContext() = default;

  // Description:
  //   ID for the context menu being displayed, CRhinoContextMenuExtension for standard
  //   Rhino IDs
	ON_UUID m_uuid;

  // Description:
	//   Selected objects affected by the context menu. You must look at m_uuid
  //   to determine which table these IDs reference.
	ON_SimpleArray<UUID>m_uuids;

  // Description:
	//   If not NULL, the document which contains m_objects objects.
	CRhinoDoc* m_doc = nullptr;

  // Description:
	//   Custom data pointer.
	void* m_param = nullptr;

private:
  void* m_reserved = nullptr;
  // Not supported
  CRhinoContextMenuContext( const CRhinoContextMenuContext&) = delete;
  const CRhinoContextMenuContext& operator =( const CRhinoContextMenuContext&) = delete;
};

class RHINO_SDK_CLASS CRhinoContextMenu : public CRhinoPopupMenu
{
public:
	// Description:
	//   Class constructor
  // Parameters:
  //   uuidContextID [in] UUID that identifies this context menu
  //   pModuleState [in] Module state for the plug-in that owns the context menu
  //   nResourceID  [in] Resource ID for menu to be loaded, if this is 0 then no menu will be loaded
  //   hInst        [in] Only used if nResourceID is greater than 0, if it is then the menu is loaded using this resource handle
  //   iSubMenu     [in] Sub menu from loaded menu to be displayed
  // Returns:
  //   Return true if you want the context menu specified by ecmt to add a separator then
  //   call ExtendContextMenu() allowing this object to add items to the context menu.
  // See Also:
  //   See CRhinoPopup for details
  CRhinoContextMenu( UUID uuidContextID, AFX_MODULE_STATE* pModuleState, UINT nResourceID = 0, HINSTANCE hInst = nullptr, int iSubMenu = 0);

  // Description:
	//   Class constructor
  // Parameters:
  //   uuidContextID [in] UUID that identifies this context menu
  //   pModuleState  [in] Module state for the plug-in that owns the context menu
  //   lpsResourceID [in] Resource ID for menu to be loaded, if this is a NULL string then no menu will be loaded
  //   hInst         [in] Only used if lpsResourceID is greater than 0, if it is then the menu is loaded using this resource handle
  //   iSubMenu      [in] Sub menu from loaded menu to be displayed
  // See Also:
  //   See CRhinoPopup for details
  CRhinoContextMenu( UUID uuidContextID, AFX_MODULE_STATE* pModuleState, LPCTSTR lpsResourceID, HINSTANCE hInst = nullptr, int iSubMenu = 0);

  virtual ~CRhinoContextMenu();

  // Description:
	//   Display and track the popup menu.
  // Parameters:
  //   hWndParent [in] Handle to the window which owns the menu and the coordinate system for the display points
  //   ptBottom [in] Bottom point.  Top left corner of the popup.  This is used by default if there is enough
  //                 screen space to display the popup menu on the bottom
  //   ptTop [in] Top point.  Bottom left corner of the poupup.  This is used if there is not enough space to display
  //              the menu below the window.
  //   iTransparent [in] 0 = Not transparent, 1 to 100 = percentage of tranparency
  // Returns:
  //   Returns the index for the selected item which was added by calling AddItem or -1 if canceled.  If using a loaded
  //   menu then WM_COMMAND messages are sent to hWndParent and 0 is returned.
  // See Also:
  //   See CRhinoPopup::PopupEx() for details
  virtual int PopUpContextMenu( CRhinoContextMenuContext& context,
                                HWND hWndParent,
                                CPoint ptBottom,
                                CPoint ptTop,
                                int iTransparent = 0);

  // Description:
	//   Return the context menu UUID passed to the constructor
  UUID ContextMenuUUID() const;

  // Description:
	//   Override this method to insert items just prior to extending the menu
  // Parameters:
  //   context [in] CRhinoContextMenuContext passed to PopUpContextMenu
  virtual void PreAddContextMenuExtensions( CRhinoContextMenuContext& context);

  // Description:
	//   Override this method to add items directly following the context extensions
  // Parameters:
  //   context [in] CRhinoContextMenuContext passed to PopUpContextMenu
  virtual void PostAddContextMenuExtensions( CRhinoContextMenuContext& context);

  // Description:
	//   This will be used to extend this class without breaking the SDK and is currently not called
	LRESULT MenuProc(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
  friend class CRhPopupMenu;

  // Description:
  //   Call this method to have plug-ins to extend the menu
  // Returns:
  //   Number of plug-in provided items added to the menu
  int AddPlugInExtensions();

protected:
  UUID m_uuidContextMenu;
  CRhinoContextMenuContext* m_pContext;

private:
  void* m_reserved = nullptr;
  // Not supported
  CRhinoContextMenu( const CRhinoContextMenu&) = delete;
  const CRhinoContextMenu& operator=( const CRhinoContextMenu&) = delete;
};

class RHINO_SDK_CLASS CRhinoContextMenuExtension
{
public:
	// Description:
	//   Class constructor
  // Parameters:
  //   plug_in [in] Reference to plug-in that owns this extension
  //   ms      [in] Reference to the module state for the plug-in
  //   bEnable [in] If true then the extension will be enabled, see Enable() method for details
  CRhinoContextMenuExtension( CRhinoPlugIn& plug_in, AFX_MODULE_STATE& ms, bool bEnable = true );

  virtual ~CRhinoContextMenuExtension();
  //----------------------------------------------------------------------------------------------
  //
  // Required overrides
  //
  //----------------------------------------------------------------------------------------------

  // Description:
	//   Required virtual override.  You MUST override this method and return true if you
  //   want ExtendContextMenu() to get called.
  // Parameters:
  //   uuidContextMenu [in] Identifies the context menu about to be displayed
  // Returns:
  //   Return true if you want the context menu specified by ecmt to add a separator then
  //   call ExtendContextMenu() allowing this object to add items to the context menu.
  virtual bool ShouldExtendContextMenu( CRhinoContextMenuContext& context) = 0;

  // Description:
	//   Required virtual override, this method will only be called if ShouldExtendContextMenu()
  //   returns true.  This is called by Rhino after a separator has been added to the specified
  //   context menu and is the plug-ins opportunity to add items to the context menu.
  // Parameters:
  //   context [in] Identifies the context menu to extend and what items are currently selected
  //   pop_menu [in] Reference to the context menu object which will display the context menu,
  //                 call the AddItem() methods on this object to extend the menu
  virtual void ExtendContextMenu( CRhinoContextMenuContext& context, CRhinoContextMenu& pop_menu) = 0;

  // Description:
	//   Required virtual override, this method is called when a menu item which was added by this context
  //   is selected.
  // Parameters:
  //   context [in] CRhinoContextMenuContext passed to CRhinoContextMenu::PopupContextMenu()
  //   iAddItemID [in] Will be >= 0 if the selected item was added to the menu as a string and not as a sub menu
  //                   item.
  //   nID [nID] If the item was added as a string this will be the current menu item ID for the item, if the item
  //             was cloned from a sub menu this will be the original menu item ID
  //   context_menu [in] Item which displayed the menu
	virtual	void OnCommand( CRhinoContextMenuContext& context, int iAddItemID, UINT nID, CRhinoContextMenu& context_menu) = 0;

  //----------------------------------------------------------------------------------------------
  //
  // Optional overrides
  //
  //----------------------------------------------------------------------------------------------
	// Description:
	//   This method is called in response to the WM_INITMENU message which is generated when the context
  //   menu is about to be displayed.
  // Parameters:
  //   context [in] Identifies the context menu to extend and what items are currently selected
  //   hWnd [in] Window that owns the menu
  //   hMenuOriginal [in/out] If this is non-NULL then this is a sub menu item that was added
  //   hMenuRuntime [in/out] If this is non-NULL then this is a sub menu item that was added, use
  //                         RuntimePopupMenuItemID() to convert original menu item IDs to runtime
  //                         IDs
  //   context_menu [in] Item which displayed the menu
  virtual void OnInitPopupMenu( CRhinoContextMenuContext& context, HWND hWnd, HMENU hMenuOriginal, HMENU hMenuRuntime, CRhinoContextMenu& context_menu);

  // Description:
	//   This method is called in response to the WM_MENUSELECT message
  // Parameters:
  //   context [in] Identifies the context menu to extend and what items are currently selected
  //   hWnd [in] Window that owns the menu
  //   hMenuOriginal [in/out] If this is non-NULL then this is a sub menu item that was added
  //   hMenuRuntime [in/out] If this is non-NULL then this is a sub menu item that was added, use
  //                         RuntimePopupMenuItemID() to convert original menu item IDs to runtime
  //                         IDs
  //   context_menu [in] Item which displayed the menu
  virtual void OnMenuSelect( CRhinoContextMenuContext& context, HWND hWnd, HMENU hMenuOriginal, HMENU hMenuRuntime, UINT nID, CRhinoContextMenu& context_menu);

  // Description:
	//   This will be used to extend this class without breaking the SDK, currently not called.
	virtual LRESULT MenuProc( CRhinoContextMenuContext& context, UINT message, WPARAM wParam, LPARAM lParam, CRhinoContextMenu& context_menu);

  
  // Description:
	//   Return enabled state for this ojbject
  // Parameters:
  //   bEnable [in] If true ShouldExtendContextMenu() will get called when Rhino
  //                is about to display a context menu otherwise it it will not
  // Returns:
  //   Returns previously enabled state
  bool Enable( bool bEnable = true);

  // Description:
	//   Return enabled state for this ojbject
  bool IsEnabled() const;

  // Description:
	//   Return the plug-in that owns this object
  CRhinoPlugIn& PlugIn();

  // Description:
	//   Return the module state associated with the plug-in that owns this object
  AFX_MODULE_STATE& ModuleState();

  // Description:
  //   ID for viewport caption context menu
  static UUID UUIDViewportCaption();

  // Description:
  //   ID for viewport context menu, did not click on an object or control point
  static UUID UUIDViewport();

  // Description:
  //   Viewport context menu displayed when clicking on one or more ojets
  static UUID UUIDViewportOnObject();

  // Description:
  //   ID for viewport context menu displays when clicking on one or more control points
  static UUID UUIDViewportOnGrip();

  // Description:
  //   ID for command history window (window above the command prompt) context menu
  static UUID UUIDCommandHisory();

  // Description:
  //   ID for command input window
  static UUID UUIDCommandWindow();

  // Description:
  //   ID for command prompt window
  static UUID UUIDCommandPrompt();

  // Description:
  //   ID for most recent command list menu
  static UUID UUIDMRUCommand();

  // Description:
  //   ID for menu displayed by the PopupMenu command
  static UUID UUIDPopupMenu();

  // Description:
  //   ID for menu displayed when the middle mouse goes down on a view
  static UUID UUIDMiddleMouse();

  // Description:
  //   ID for layer list control context menu
  static UUID UUIDLayerList();
protected:
  CRhinoPlugIn& m_plug_in;
  AFX_MODULE_STATE& m_ms;
  bool m_bEnabled;

private:
  void* m_reserved;
  // Not supported
  CRhinoContextMenuExtension( const CRhinoContextMenuExtension&) = delete;
  const CRhinoContextMenuExtension& operator= (const CRhinoContextMenuExtension&) = delete;
};

#endif
