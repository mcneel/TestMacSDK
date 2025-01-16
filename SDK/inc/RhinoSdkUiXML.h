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

//==========================================================================================
//
// Description:
//   For internal use.  Used by CRuiOnUpdateMenuItems to track registered menu items
//
class RHINO_SDK_CLASS CRuiOnUpdateMenuItem
{
public:
  //
  // Description:
  //   Default constructor
  //
  CRuiOnUpdateMenuItem();
  //
  // Description:
  //   Constructor to allow easy initialization
  //
  CRuiOnUpdateMenuItem(UUID idFile, UUID idMenu, UUID idItem);
  //
  // Description:
  //   Constructor to allow easy initialization
  //
  CRuiOnUpdateMenuItem(const wchar_t* idFile, const wchar_t* idMenu, const wchar_t* idItem);
  //
  // Description:
  //   Check to see if this item is equal to specified IDs
  //
  // Parameters:
  //   idFile [in] The UUID of the file that contains the menu item
  //   idMenu [in] The UUID of the menu that contains the menu item, this may be a sub men ID
  //   idItem [in] The UUID of the menu item to update
  //
  // Returns;
  //   Returns true if m_idFile == idFile and m_idMenu == idMenu and m_idItem == idItem
  //
  bool IsEqualTo(UUID idFile, UUID idMenu, UUID idItem) const;
  //
  // Description:
  //   File ID associated with this item
  //
  // Returns;
  //   File ID associated with this item
  //
  const UUID& FileID() const { return m_idFile; }
  //
  // Description:
  //   Menu ID associated with this item
  //
  // Returns;
  //   Menu ID associated with this item
  //
  const UUID& MenuID() const { return m_idMenu; }
  //
  // Description:
  //   Menu item ID associated with this item
  //
  // Returns;
  //   Menu item ID associated with this item
  //
  const UUID& MenuItemID() const { return m_idItem; }
protected:
  UUID m_idFile; // File ID
  UUID m_idMenu; // Menu ID
  UUID m_idItem; // Menu item ID
public:
  // For internal use only!
  // Reserved for future use.
  class CRhDockBarExtension* m__update_menu_item_extension;
};
//==========================================================================================
//
// Description:
//   This class provides an easy way to update a menu item.  A reference to one of these is
//   sent to CRuiOnUpdateMenuItems::OnUpdateMenuItem on a WM_INITMENUPOPUP event
//
// See Also:
//   MSDN Help for WM_INITMENUPOPUP
//   MSDN help for CCmdUI
//   CRuiOnUpdateMenuItems::OnUpdateMenuItem
//
class RHINO_SDK_CLASS CRuiUpdateUi
{
public:
  //
  // Description:
  //   Constructor
  //
  CRuiUpdateUi(UUID idFile, UUID idMenu, UUID idItem, HMENU hMenu, int nIndex);
  //
  // Description:
  //   Returns true if successfully initialized
  //
  bool IsValid() const;
  //
  // Description:
  //   Call this member function to enable or disable the user-interface item for this command.
  //
  // Parameters:
  //   bOn [in] Set to true to enable the item, false to disable it
  //
  // See Also:
  //   See CCmdUI::Enable in MSDN help file
  //
  void Enable(bool bOn = true);
  //
  // Description:
  //   Call this member function determine if the menu item is enabled
  //
  // Returns:
  //   Returns true if enabled otherwise false
  //
  bool Enabled() const;
  //
  // Description:
  //   Call this member function to set the user-interface item for this command to the appropriate check state.
  //
  // Parameters:
  //   bCheck [in] Set to true to check the item, false to uncheck it
  //
  // See Also:
  //   See CCmdUI::SetCheck in MSDN help file
  //
	void SetCheck(bool bCheck = true);
  //
  // Description:
  //   Call this member function determine if the item is checked
  //
  // Returns:
  //   Returns true if checked otherwise false
  //
	bool Checked() const;
  //
  // Description:
  //   Call this member function to set the user-interface item for this command to the appropriate check state.
  //
  //   This member function operates like SetCheck, except that it operates on user-interface items acting as part
  //   of a radio group. Unchecking the other items in the group is not automatic unless the items themselves maintain
  //   the radio-group behavior.
  //
  // Parameters:
  //   bOn [in] Set to true to check the item, false to uncheck it
  //
  // See Also:
  //   See CCmdUI::SetRadio in MSDN help file
  //
	void SetRadio(bool bOn = false);
  //
  // Description:
  //   Call this member function determine if the radio button is checked
  //
  // Returns:
  //   Returns true if checked otherwise false
  //
	bool RadioChecked() const;
  //
  // Description:
  //   Call this member function to set the text of the menu item.
  //
  // Parameters:
  //   sText [in] If not NULL or empty string the menu item text will be set to this value
  //
  // See Also:
  //   See CCmdUI::SetText in MSDN help file
  //
	void SetText(const wchar_t* sText);
  //
  // Description:
  //   Call this member function to get the text of the menu item.
  //
  // Returns:
  //   Menu item text
  //
	const wchar_t* Text() const;
  //
  // Description:
  //   Call this member function to get the menu handle for the menu that owns this item
  //
  // Returns:
  //   Return menu handle for the menu that owns this item
  //
  HMENU Menu() const;
  //
  // Description:
  //   Call this member function to get the menu item ID
  //
  // Returns:
  //   Returns the menu item ID
  //
  UINT MenuItemID() const;
  //
  // Description:
  //   Call this member function to get the menu item index
  //
  // Returns:
  //   Returns the menu item index
  //
  int MenuItemIndex() const;
  //
  // Description:
  //   Call this member function get the UUID of the RUI file that owns this Windows menu item
  //
  // Returns:
  //   Returns the UUID of the RUI file that owns this Windows menu item
  //
  UUID FileUUID() const;
  //
  // Description:
  //   Call this member function get the UUID of the RUI menu that owns this Windows menu item
  //
  // Returns:
  //   Returns the UUID of the RUI menu that owns this Windows menu item
  //
  UUID MenuUUID() const;
  //
  // Description:
  //   Call this member function get the UUID of the RUI menu item associated with this Windows menu item
  //
  // Returns:
  //   Returns the UUID of the RUI menu item that owns this associated with this Windows menu item
  //
  UUID MenuItemUUID() const;
protected:
  //
  // Description:
  //   Used internally to set the check state for a menu item without changing the check state
  //   bitmap
  //
  // Parameters:
  //   bCheck [in] Set to true to check the item, false to uncheck it
  //
	void SetCheckState(bool bCheck);

protected:
  HMENU m_hMenu; // Handle to the menu containing this menu item
  UINT m_nID;    // Menu item ID
  int m_nIndex;  // Zero based menu item index
  int m_nItemCount; // Number of items in this menu, used to validate index
  ON_wString m_wText;
  UUID m_idFile; // File ID
  UUID m_idMenu; // Menu ID
  UUID m_idItem; // Menu item ID
public:
  // For internal use only!
  // Reserved for future use.
  class CRhDockBarExtension* m__update_ui_extension;
};
//==========================================================================================
//
// Description:
//   This class is used to create menu item update handlers for menus created when a Rhino
//   RUI file is loaded.  To use a plug-in must derive a new class from this one and override
//   the pure virtual OnUpdateMenuItem method.  Following is an example implementation:
//
//  // {AE63E876-3D28-4a97-823F-7436732F36E0}
//  static const UUID idFile = { 0xae63e876, 0x3d28, 0x4a97, { 0x82, 0x3f, 0x74, 0x36, 0x73, 0x2f, 0x36, 0xe0 } };
//  // {5059282D-3954-44f2-8F43-124B9121242A}
//  static const UUID idMainMenu =  { 0x5059282d, 0x3954, 0x44f2, { 0x8f, 0x43, 0x12, 0x4b, 0x91, 0x21, 0x24, 0x2a } };
//  {BA80E8F8-EC51-44a4-BE1D-021AC366B8BF}
//  static const UUID idLine = { 0xba80e8f8, 0xec51, 0x44a4, { 0xbe, 0x1d, 0x2, 0x1a, 0xc3, 0x66, 0xb8, 0xbf } };
//  // {6F531BF1-402E-4d64-B457-6E627FCCFF63}
//  static const UUID idBox = { 0x6f531bf1, 0x402e, 0x4d64, { 0xb4, 0x57, 0x6e, 0x62, 0x7f, 0xcc, 0xff, 0x63 } };
//  class CMyRuiOnUpdateMenuItems : public CRuiOnUpdateMenuItems
//  {
//  public:
//    CMyRuiOnUpdateMenuItems()
//    {
//      this->RegisterMenuItem(idFile, idMainMenu, idLine);
//      this->RegisterMenuItem(L"AE63E876-3D28-4a97-823F-7436732F36E0", L"0F6EAA81-AE18-48fe-8CA1-585CCDC84957", L"6F531BF1-402E-4d64-B457-6E627FCCFF63");
//    }
//    void OnUpdateMenuItem(UUID idFile, UUID idMenu, UUID idItem, CRuiUpdateUi& cmdui)
//    {
//      if (idLine == idItem)
//        cmdui.SetCheck(true);
//      else if (idBox == idItem)
//        cmdui.SetCheck(true);
//    }
//  };
//
// See Also:
//   MSDN Help for WM_INITMENUPOPUP
//
class RHINO_SDK_CLASS CRuiOnUpdateMenuItems
{
public:
  //
  // Description:
  //   Constructor
  //
  CRuiOnUpdateMenuItems();
  //
  // Description:
  //   Destructor
  //
  virtual ~CRuiOnUpdateMenuItems();
  //----------------------------------------------------------------------------------------
  //
  // Step 1:  Register menu item IDs to receive update notification
  //
  // Description:
  //   Register a <RUI File> <RUI Menu> <RUI Menu Item> to receive update notification
  //
  // Parameters:
  //   idFile [in] The UUID of the file that contains the menu item
  //   idMenu [in] The UUID of the menu that contains the menu item, this may be a sub men ID
  //   idItem [in] The UUID of the menu item to update
  //
  // Returns;
  //   Returns true if successfully registered or false if there was an error
  //
  // See Also:
  //   bool RegisterMenuItem(const wchar_t* idFile, const wchar_t* idMenu, const wchar_t* idItem);
  //
  bool RegisterMenuItem(UUID idFile, UUID idMenu, UUID idItem);
  //
  // Description:
  //   Register a <RUI File> <RUI Menu> <RUI Menu Item> to receive update notification
  //
  // Parameters:
  //   idFile [in] The UUID of the file that contains the menu item
  //   idMenu [in] The UUID of the menu that contains the menu item, this may be a sub men ID
  //   idItem [in] The UUID of the menu item to update
  //
  // Returns;
  //   Returns true if successfully registered or false if there was an error
  //
  // See Also:
  //   bool RegisterMenuItem(UUID idFile, UUID idMenu, UUID idItem);
  //
  bool RegisterMenuItem(const wchar_t* idFile, const wchar_t* idMenu, const wchar_t* idItem);
  //----------------------------------------------------------------------------------------
  //
  // Step 2:  You MUST OVERRIDE this method!!
  //
  // Description:
  //   This method is called when Rhino receives the actually WM_INITPOPUPMENU message and forward
  //   to the first occurrence of RegisterMenuItem()
  //
  // Parameters:
  //   idFile [in] The UUID of the file that contains the menu item
  //   idMenu [in] The UUID of the menu that contains the menu item, this may be a sub men ID
  //   idItem [in] The UUID of the menu item to update
  //
  // See Also:
  //   bool RegisterMenuItem(UUID idFile, UUID idMenu, UUID idItem);
  //   bool RegisterMenuItem(const wchar_t* idFile, const wchar_t* idMenu, const wchar_t* idItem);
  //
  virtual void OnUpdateMenuItem(CRuiUpdateUi& cmdui) = 0;
  //
  // Description:
  //   Method to determine if a specific menu item has been registered
  //
  // Parameters:
  //   idFile [in] The UUID of the file that contains the menu item
  //   idMenu [in] The UUID of the menu that contains the menu item, this may be a sub men ID
  //   idItem [in] The UUID of the menu item to update
  //
  // Returns;
  //   Returns a pointer to the CRuiOnUpdateMenuItem if registered otherwise NULL
  //
  // See Also:
  //   bool RegisterMenuItem(UUID idFile, UUID idMenu, UUID idItem);
  //   bool RegisterMenuItem(const wchar_t* idFile, const wchar_t* idMenu, const wchar_t* idItem);
  //
  const CRuiOnUpdateMenuItem* FindUpdateItem(UUID idFile, UUID idMenu, UUID idItem);
protected:
  ON_SimpleArray<CRuiOnUpdateMenuItem>m_MenuItems;
public:
  // For internal use only!
  // Reserved for future use.
  class CRhDockBarExtension* m__update_ui_extension;
};

