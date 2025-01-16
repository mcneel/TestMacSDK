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

class CRhinoUiTabbedSheetCtrlGDI;

///////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiTabbedSheetCtrlItem
//
class RHINO_SDK_CLASS CRhinoUiTabbedSheetCtrlItem
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsName [in] Text displayed on the tab when visible.
  //   pWndItem [in] Window associated with this tab.  When the tab becomes active
  //                 this window will be shown otherwise it will be hidden.
  //   
  CRhinoUiTabbedSheetCtrlItem( const wchar_t* lpsName = NULL, CWnd* pWndItem = NULL);
  //   
  // Description:
  //   Destructor
  //
  ~CRhinoUiTabbedSheetCtrlItem();
  //   
  // Description:
  //   Copy constructor
  //
  CRhinoUiTabbedSheetCtrlItem( const CRhinoUiTabbedSheetCtrlItem& src);
  //   
  // Description:
  //   Operator equal
  //
  const CRhinoUiTabbedSheetCtrlItem& operator=( const CRhinoUiTabbedSheetCtrlItem& src);
public:
  //   
  // Description:
  //   Call this method to get the tab text.
  //
  // Returns:
  //   Returns the text associated with this named tab or NULL if there is no name.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::SetName()
  //   
  const wchar_t* Name() const;
  //   
  // Description:
  //   Call this method to set the text which will be displayed on the tab.
  //
  // Parameters:
  //   lps [in] Text to display on tab
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::Name()
  //   
  void SetName( const wchar_t* lps);
  //   
  // Description:
  //   Call this method to get the window which will be displayed when this tab is current.
  //
  // Returns:
  //   Window associated with this tab.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::SetIemWindow()
  //   
  CWnd* ItemWindow() const;
  //   
  // Description:
  //   Call this method to set the window which will be displayed when this tab is current.
  //
  // Parameters:
  //   pWnd [in] Window to display.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::IemWindow()
  //   
  void SetItemWindow( CWnd* pWnd);
  //   
  // Description:
  //   Call this method to find out if the tab will be hidden.
  //
  // Returns:
  //   Returns true if the tab will be hidden or false if it will be visible.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::IsVisible()
  //   CRhinoUiTabbedSheetCtrl::HideItem()
  //   
  bool IsHidden() const;
  //   
  // Description:
  //   Call this method to find out if the tab will be displayed.
  //
  // Returns:
  //   Returns true if the tab will be displayed or false if it will not.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::IsHidden()
  //   CRhinoUiTabbedSheetCtrl::HideItem()
  //   
  bool IsVisible() const;
  enum item_state
  {
    normal_state = 0x0000,      // Tab is in normal state, not depressed or highlighted
    pressed_state = 0x0001,     // Tab button is depressed
    highlighted_state = 0x0002, // Tab button is highlighted
  };
  //   
  // Description:
  //   Retrieves the state of this tab control item.
  //
  // Returns:
  //   Returns item_state enum describing the current state of this tab control item.
  //   Can be one of the following:
  //     normal_state        Tab is in normal state, not depressed or highlighted.
  //     pressed_state       The tab control item is selected.
  //     highlighted_state   The tab control item is highlighted.
  //
  DWORD ItemState() const;
  //   
  // Description:
  //   This method retrieves the application-specific value associated with this tab control item.
  //   This value is the m_dwItemData member of this class.
  //
  // Returns:
  //   A application-specific value associated with this tab control item.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetDataPtr()
  //   CRhinoUiTabbedSheetCtrlItem::SetDataPtr()
  //   
  DWORD_PTR GetData() const;
  //   
  // Description:
  //   This method sets the application-specific value associated with this tab control item. This
  //   value is the m_dwItemData member of this class.
  //
  // Parameters:
  //   dwItemData [in] Specifies a application-specific value to be associated with this item.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetDataPtr()
  //   CRhinoUiTabbedSheetCtrlItem::SetDataPtr()
  //   
  void SetData( DWORD_PTR dwItemData);
  //   
  // Description:
  //   This method retrieves the application-specific value associated with this tab control item.
  //   This value is the m_pData member of this class.
  //
  // Returns:
  //   A application-specific pointer associated with this tab control item or NULL if none.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   CRhinoUiTabbedSheetCtrlItem::SetDataPtr()
  //   
  void* GetDataPtr() const;
  //   
  // Description:
  //   This method sets the application-specific value associated with this tab control item. This
  //   value is the m_pData member of this class.
  //
  // Parameters:
  //   pData [in] Specifies a application-specific value to be associated with this item.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetDataPtr()
  //   
  void SetDataPtr( void* pData);

protected:
  friend class CRhinoUiTabbedSheetCtrl;
  //   
  // Description:
  //   Call this method to psition the ItemWindow() associated with this tab control item.
  //
  // Parameters:
  //   r [in] New position for window
  //   pWndZOrderAfter [in] Window in ZOrder to position after, ignored if NULL
  //
  void MovePage( CRect r, CWnd* pWndZOrderAfter) const;
  //   
  // Description:
  //   Call this method to show or hide the ItemWindow() associated with this tab control item.
  //
  // Parameters:
  //   bShow [in] If true then the window will be shown otherwise; it will be hidden.
  //   pWndZOrderAfter [in] Window in ZOrder to position after, ignored if NULL
  //   lpR [in] If this points to a RECT structure then the window is moved here.
  //
  // Returns:
  //   Returns true if successful otherwise false.
  //
  bool ShowItemWindow( bool bShow = true, CWnd* pWndZOrderAfter = NULL, LPRECT lpR = NULL) const;
  //   
  // Description:
  //   Call this method to initialize the m_rLabel, m_rgnLabel and m_label_pts members for this
  //   tab control item.
  //
  // Parameters:
  //   r [in] m_rLabel Will be set to this.  Bounding rectangle for the tab label.
  //   orientate [in] Current tab display location.  This is used when calculating m_rgnLabel
  //                  and m_label_pts
  //   
  void SetLabelRect( const CRect& r, int orientate);
  //   
  // Description:
  //   Call this method to draw this tab control item using the specified device context.
  //
  // Parameters:
  //   dc [in] Device context to draw to
  //   bCurrent [in] Should be true if this is the active tab control item
  //   orientation [in] Current tab display location
  //   gdi [in] Draw helper, see CRhinoUiTabbedSheetCtrlGDI for details
  //
  // See Also:
  //   class CRhinoUiTabbedSheetCtrlGDI
  //
  void DrawLabel( CDC& dc, bool bCurrent, int orientation, CRhinoUiTabbedSheetCtrlGDI& gdi);
  //   
  // Description:
  //   Call this method to draw text label for this tab control item.
  //
  // Parameters:
  //   dc [in] Device context to draw to
  //   orientation [in] Current display position for tabs
  //
  // Returns:
  //   Returns true if there was text to be drawn otherwise false
  //
  bool DrawItemText( CDC& dc, int orientation);

protected:
  CRect m_rLabel;         // Bounding rectangle for tab control label
  CRgn m_rgnLabel;        // Fill and hit-test region for tab control item
  ON_SimpleArray<CPoint>m_label_pts; // Points used to create m_rgnLable
  bool m_bShow;           // If true then tab is displayed otherwise it is not
  ON_wString m_wName;     // Tab label
  CWnd* m_pWndItem;       // Window to show when tab becomes active
  DWORD m_dwItemState;    // Current item state
  DWORD_PTR m_dwItemData; // Application specified data associated with this item
  void* m_pData;          // Application specified data associated with this item
};

//   
// Description:
//   Registered Windows message sent to controls parent then and then the control itself if the
//   parent does not return a non zero value.  This message is sent when calling DeleteItem()
//   or DeleteAllItems() and uses the following format:
//     message = CRhinoUiTabbedSheetCtrl::m_delete_item_custom_win_msg
//               (static UINT holding ID of registered Windows message)
//     WPARAM = HWND of the control sending the message
//     LPARAM = Zero based index of item being deleted.
//
//   To use this notification message do add the message map macro to the parent
//   of this control:
//     ON_REGISTERED_MESSAGE( CRhinoUiTabbedSheetCtrlMsg::m_custom_msg_id, OnTabbedSheetCtrlMsg)
//   Add this to the decloration of the the parent window class:
//     afx_msg LRESULT OnTabbedSheetCtrlMsg( WPARAM wParam, LPARAM lParam);
//   Add this method to the class implementation
//     LRESULT CMyDialog::OnTabbedSheetCtrlMsg( WPARAM wParam, LPARAM lParam)
//     {
//       CRhinoUiTabbedSheetCtrlMsg::LPCUSTOMINFO pInfo = (CRhinoUiTabbedSheetCtrlMsg::LPCUSTOMINFO)lParam;
//       if( NULL == pInfo)
//         return 0;
//       
//       if( m_tabbed_sheet_ctrl.m_hWnd != pInfo->m_hWnd)
//         return 0;
//
//       switch( pInfo->m_nMsg)
//       {
//         case CRhinoUiTabbedSheetCtrlMsg::mid_delete_item:
//           // do something like deleting item data
//           return 1;
//         case CRhinoUiTabbedSheetCtrlMsg::mid_begin_label_edit:
//           // allow label edit
//           return 1;
//         case CRhinoUiTabbedSheetCtrlMsg::mid_end_label_edit:
//           // Allow label update as long as text is not empty
//           return( pInfo->m_lpsText && pInfo->m_lpsText[0]);
//       }
//       return 0L;
//     }
//
// See Also:
//   RegisterWindowMessage() in the Platform SDK
//   ON_REGISTERED_MESSAGE in the Platform SDK
//
class RHINO_SDK_CLASS CRhinoUiTabbedSheetCtrlMsg
{
public:
  enum message_id
  {
    mid_sel_changing = 1, // Message sent when the current tab is about to change
    mid_sel_changed,      // Message sent when the current tab has been changed
    mid_delete_item,      // Message sent just prior to deleting an item
    mid_begin_label_edit, // Message sent prior to editing an item label, return 1 to allow
                          // Editing to continue or 0 to disable editing for this item
    mid_end_label_edit,   // Message sent when in-place edit control is hidden and text has
                          // changed.  Return 1 to commit change or 0 to abort.
    mid_right_mouse_down_on_tab, // The right mouse button has gone down over a tab.  The tab
                                 // will be made current prior to this message being sent.
    mid_begin_tab_drag,   // Message sent just prior to starting to drag a tab.  If the m_bool property
                          // is not set to true then the drag action is aborted.  By default m_bool is
                          // true.
    mid_tab_dropped,      // Message sent when a tab is dragged and dropped someplace other
                          // than on another tab.  m_pt Will contain screen point for drop action.
    mid_tab_drag_moved,   // Message sent when a tab is dragged to a new position in the control.
                          // m_iItem Will be equal to new position, m_nPrevSel will be equal to
                          // previous position.
    left_mouse_down_on_add_button, // The left mouse went down on the add button, the "+" after the last tab
    add_button_click,              // The left mouse went down and up on the add button, the "+" after the last tab
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_message_id = 0xFFFFFFFF
  };

  typedef struct tagCUSTOMINFO
  {
    HWND m_hWnd;  // Event source
    UINT m_nMsg;      // Event (selection change, item deleted, etc )
    int m_iItem;      // Zero based index for tab generating the message
    int m_nPrevSel;   // Zero based index for previous current tab, only valid for
                      // mid_sel_changing and mid_sel_changed messages.
    LPCTSTR m_lpsText;// Pointer to string used by mid_begin_label_edit and
                      // mid_end_label_edit messages.
    bool m_bool;      // Message dependent:
                      //   mid_begin_tab_drag = If true then drag operation will continue as normal
                      //                        otherwise it will be aborted.
    POINT m_pt;       // Mouse event point
  } CUSTOMINFO, *LPCUSTOMINFO;

  static void InitCustomInfo(   CUSTOMINFO& info
                              , HWND hWnd
                              , message_id message
                              , int iItem1
                              , int iItem2 = -1
                              , LPCTSTR lps = NULL
                              , LPPOINT lpPt = NULL
                              , bool b = true
                            );

  static LPCTSTR MsgString();
  static UINT const m_custom_msg_id;

};

class RHINO_SDK_CLASS CRhinoUiTabbedSheetCtrl : public CWnd
{
	DECLARE_DYNAMIC(CRhinoUiTabbedSheetCtrl)

public:
  //   
  // Description:
  //   Constructor
  //
	CRhinoUiTabbedSheetCtrl();
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiTabbedSheetCtrl();

  enum orientation
  {
    orient_top = 0, // Display tabs at top of control with pages below
    orient_left,    // Display tabs on left side of control with pages to the right
    orient_right,   // Display tabs on right side of control with pages to the left
    orient_bottom   // Default orientation, display tabs at bottom of control with pages above
  };
  //   
  // Description:
  //   Call this method to get the current display orientation for tab control items in this control.
  //
  // Returns:
  //   Returns the current display orientation for tab control items in this control.  Possible
  //   values:
  //      orient_top         Display tabs at top of control with pages below
  //      orient_left        Display tabs on left side of control with pages to the right
  //      orient_right       Display tabs on right side of control with pages to the left
  //      orient_bottom      Display tabs at bottom of control with pages above
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetOrientation()
  //   CRhinoUiTabbedSheetCtrl::orientation
  //   
  orientation Orientation() const;
  //   
  // Description:
  //   Set the display orientation for tab control items in this control.
  //
  // Parameters:
  //   orient [in] Display orientation for tab control items in this control.  Possible values:
  //                  orient_top         Display tabs at top of control with pages below
  //                  orient_left        Display tabs on left side of control with pages to the right
  //                  orient_right       Display tabs on right side of control with pages to the left
  //                  orient_bottom      Display tabs at bottom of control with pages above
  //
  // Returns:
  //   Returns previous orientation
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::Orientation()
  //   CRhinoUiTabbedSheetCtrl::orientation
  //   
  orientation SetOrientation( orientation orient);
  //   
  // Description:
  //   Call this method to find out if tab label names will in-place edit when double clicked.
  //
  // Returns:
  //   Returns true if the label will in-place edit on double click otherwise returns false.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetEditLabelOnDoubleClick()
  //   CRhinoUiTabbedSheetCtrl::EditLabel()
  //   CRhinoUiTabbedSheetCtrlMsg::mid_begin_label_edit
  //   CRhinoUiTabbedSheetCtrlMsg::mid_end_label_edit
  //   
  bool EditLabelOnDoubleClick() const;
  //   
  // Description:
  //   Call this method to enable or disable tab label name in-place editing when a tab label is
  //   double clicked.
  //
  // Parameters:
  //   b [in] If this is true then labels will in-place edit when double clicked if
  //          CRhinoUiTabbedSheetCtrlMsg::mid_begin_label_edit message returns 1.
  //
  // Returns:
  //   Returns previous state.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetEditLabelOnDoubleClick()
  //   CRhinoUiTabbedSheetCtrl::EditLabel()
  //   CRhinoUiTabbedSheetCtrlMsg::mid_begin_label_edit
  //   CRhinoUiTabbedSheetCtrlMsg::mid_end_label_edit
  //   
  bool SetEditLabelOnDoubleClick( bool b);
  //   
  // Description:
  //   Call this method to cause the add tab button (the "+" after the last tab) to be added to the control.
  //   This method will only work if called after the control has been created, the default value is false.
  //
  // Parameters:
  //   b [in] If this is true then the add button is visible otherwise; the add button is not visible
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::IncludeAddButton()
  //   
  void SetIncludeAddButton(bool b);
  //   
  // Description:
  //   Call this method to determine if the tab button will be displayed.   This method will only work if called
  //   after the control has been created, the default value is false.
  //
  // Returns:
  //   Returns true if the add tab button is part of the control
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetIncludeAddButton()
  //   
  bool IncludeAddButton();
  //   
  // Description:
  //   Call this method to find out if "First" and "Last" scroll buttons will be displayed.  The "First"
  //   button will make sure the first visible tab control item is displayed, the "Last" button will 
  //   make sure the last visible tab control item is displayed.
  //
  // Returns:
  //   Returns true if the "First" and "Last" scroll buttons will be displayed otherwise returns false.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetIncludeFirstAndLastButtons()
  //   
  bool IncludeFirstAndLastButtons() const;
  //   
  // Description:
  //   Call this method to enable or disable the display of the "First" and "Last" scroll buttons. The
  //   "First" button will make sure the first visible tab control item is displayed, the "Last" button
  //   will  make sure the last visible tab control item is displayed.
  //
  // Parameters:
  //   b [in] If this is true then the first and last buttons will be displayed otherwise they will not.
  //
  // Returns:
  //   Returns previous state.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::IncludeFirstAndLastButtons()
  //   
  bool SetIncludeFirstAndLastButtons( bool b);
  //   
  // Description:
  //   Call this method to find out if "Previous" and "Next" scroll buttons will be displayed.  The
  //   "Previous" button will make sure the item, prior to the current first visible item, is displayed,
  //   the "Next" button will make sure the item after the last displayed is visible.
  //
  // Returns:
  //   Returns true if "Next" and "Previous" buttons are visible otherwise false.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::IncludeFirstAndLastButtons()
  //   
  bool IncludeNextAndPreviousButtons() const;
  //   
  // Description:
  //   Call this method to enable display of the "Previous" and "Next" scroll buttons. The "Previous"
  //   button will make sure the item, prior to the current first visible item, is displayed, the "Next"
  //   button will make sure the item after the last displayed is visible.
  //
  // Parameters:
  //   b [in]  If true then the "Next" and "Previous" buttons will be displayed otherwise they will not.
  //
  // Returns:
  //   Returns previous state.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetIncludeFirstAndLastButtons()
  //   
  bool SetIncludeNextAndPreviousButtons( bool b);
  //   
  // Description:
  //   Adds a new last item to existing tab control.
  //
  // Parameters:
  //   item [in] Reference to a CRhinoUiTabbedSheetCtrlItem object that specifies the attributes of the tab.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::InsertItem()
  //   
  //   
  int AddItem( const CRhinoUiTabbedSheetCtrlItem& item);
  //   
  // Description:
  //   Adds a new last item to existing tab control.
  //
  // Parameters:
  //   lpsItem [in] Address of a null-terminated string that contains the text of the tab.
  //   pWndItem [in] Pointer to window which should be displayed when this tab is active.  If this is NULL
  //                 then no window synchronization occurs.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::InsertItem()
  //   
  int AddItem( const wchar_t* lpsItem, CWnd* pWndItem = NULL);
  //   
  // Description:
  //   Inserts a new tab in an existing tab control.
  //
  // Parameters:
  //   iIndex [in] Zero-based index of the new tab.
  //   lpsItem [in] Address of a null-terminated string that contains the text of the tab.
  //   pWndItem [in] Pointer to window which should be displayed when this tab is active.  If this is NULL
  //                 then no window synchronization occurs.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::AddItem()
  //   
  int InsertItem( int iIndex, const wchar_t* lpsItem, CWnd* pWndItem = NULL);
  //   
  // Description:
  //   Inserts a new tab in an existing tab control.
  //
  // Parameters:
  //   iIndex [in] Zero-based index of the new tab.
  //   item [in] Reference to a CRhinoUiTabbedSheetCtrlItem object that specifies the attributes of the tab.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::AddItem()
  //   
  int InsertItem( int iIndex, const CRhinoUiTabbedSheetCtrlItem& item);
  //   
  // Description:
  //   Inserts a new tab in an existing tab control.
  //
  // Parameters:
  //   iIndex [in] Zero-based index of the new tab.
  //   pTabCtrlItem [in] Pointer to a TCITEM structure that specifies the attributes of the tab. 
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::AddItem()
  //   TCITEM in the Platform SDK
  //   CTabCtrl::InsertItem in the Platform SDK
  //   
  int InsertItem(int iIndex, TCITEM* pTabCtrlItem);
  //   
  // Description:
  //   Inserts a new tab in an existing tab control.
  //
  // Parameters:
  //   iIndex [in] Zero-based index of the new tab.
  //   lpsItem [in] Address of a null-terminated string that contains the text of the tab.
  //   iImage [in] The zero-based index of an image to insert from an image list.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::AddItem()
  //   TCITEM in the Platform SDK
  //   CTabCtrl::InsertItem in the Platform SDK
  //   
  int InsertItem(int iIndex, LPCTSTR lpsItem, int iImage);
  //   
  // Description:
  //   Inserts a new tab in an existing tab control.
  //
  // Parameters:
  //   nMask [in] Specifies which TCITEM structure attributes to set. Can be zero or a combination of the
  //              following values: 
  //                  TCIF_TEXT   The pszText member is valid. 
  //                  TCIF_IMAGE   The iImage member is valid. 
  //                  TCIF_PARAM   The lParam member is valid. 
  //                  TCIF_RTLREADING   The text of pszText is displayed using right-to-left reading order
  //                                    on Hebrew or Arabic systems. 
  //                  TCIF_STATE   The dwState member is valid. 
  //   iIndex [in] Zero-based index of the new tab.
  //   lpsItem [in] Address of a null-terminated string that contains the text of the tab.
  //   iImage [in] The zero-based index of an image to insert from an image list.
  //   lParam [in] Application-defined data associated with the tab.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::AddItem()
  //   CRhinoUiTabbedSheetCtrl::GetItemData()
  //   TCITEM in the Platform SDK
  //   CTabCtrl::InsertItem in the Platform SDK
  //
  int InsertItem(UINT nMask, int iIndex, LPCTSTR lpsItem, int iImage, LPARAM lParam);
  //   
  // Description:
  //   Inserts a new tab in an existing tab control.
  //
  // Parameters:
  //   nMask [in] Specifies which TCITEM structure attributes to set. Can be zero or a combination of the
  //              following values: 
  //                  TCIF_TEXT   The pszText member is valid. 
  //                  TCIF_IMAGE   The iImage member is valid. 
  //                  TCIF_PARAM   The lParam member is valid. 
  //                  TCIF_RTLREADING   The text of pszText is displayed using right-to-left reading order
  //                                    on Hebrew or Arabic systems. 
  //                  TCIF_STATE   The dwState member is valid. 
  //   iIndex [in] Zero-based index of the new tab.
  //   lpsItem [in] Address of a null-terminated string that contains the text of the tab.
  //   iImage [in] The zero-based index of an image to insert from an image list.
  //   lParam [in] Application-defined data associated with the tab.
  //   dwStateMask [in] Specifies which states are to be set. For more information, see TCITEM in the Platform SDK.
  //
  // Returns:
  //   Zero-based index of the new tab if successful; otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::AddItem()
  //   TCITEM in the Platform SDK
  //   CTabCtrl::InsertItem in the Platform SDK
  //   
  int InsertItem(UINT nMask, int iIndex, LPCTSTR lpsItem, int iImage, LPARAM lParam, DWORD dwState, DWORD dwStateMask);
  //   
  // Description:
  //   Retrieves the zero based index for the currently selected tab in a tab control.
  //
  // Returns:
  //   Zero-based index of the selected tab if successful or – 1 if no tab is selected.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetCurSel()
  //   CRhinoUiTabbedSheetCtrl::GetCurItem()
  //   
  int GetCurSel() const;
  //   
  // Description:
  //   Selects a tab in a tab control.
  //
  // Parameters:
  //   iItem [in] The zero-based index of the item to be selected.
  //
  // Returns:
  //   Zero-based index of the previously selected tab if successful, otherwise – 1.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetCurSel()
  //   CRhinoUiTabbedSheetCtrl::GetCurItem()
  //   
  int SetCurSel( int iItem);
  //   
  // Description:
  //   Retrieves a pointer to a CRhinoUiTabbedSheetCtrlItem for the currently selected
  //   tab in a tab control.
  //
  // Returns:
  //   Pointer to a CRhinoUiTabbedSheetCtrlItem for the currently selected tab if successful,
  //   otherwise NULL
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetCurSel()
  //   CRhinoUiTabbedSheetCtrl::SetCurSel()
  //   
  const CRhinoUiTabbedSheetCtrlItem* GetCurItem() const;
  //   
  // Description:
  //   Retrieves information about a tab in a tab control.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Returns:
  //   Pointer to a CRhinoUiTabbedSheetCtrlItem which contains information about the tab.
  //
  // See Also:
  //   class CRhinoUiTabbedSheetCtrlItem
  //   
  const CRhinoUiTabbedSheetCtrlItem* GetItem( int iItem) const;
  //   
  // Description:
  //   Retrieves zero based index of the first tab found with the specified name.
  //
  // Parameters:
  //   lps [in] Name of tab to look for.
  //   bCaseSensitiveMatch [in] If true then will only find tab where name and case match
  //                            otherwise; will find tab where name matches regardless of case.
  //
  // Returns:
  //   Returns zero based index of first tab found with matching name or -1 if no match found.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetItem()
  //   
  int GetItemIndex( const wchar_t* lps, bool bCaseSensitiveMatch = false) const;
  //   
  // Description:
  //   Retrieves the number of tabs in the tab control.  This includes tabs that are hidden.
  //
  // Returns:
  //   Number of items, including hidden items, in the tab control.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetItem()
  //   CRhinoUiTabbedSheetCtrl::SetItem()
  //   
  int GetItemCount() const;
  //   
  // Description:
  //   Retrieves the bounding rectangle for the specified tab in a tab control.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //   lpRect [out] Pointer to a RECT structure that receives the bounding rectangle of the tab.
  //               These coordinates use the viewport's current mapping mode.
  // Returns:
  //   true If successful; otherwise false.
  //
  bool GetItemRect( int iItem, LPRECT lpRect) const;
  //   
  // Description:
  //   Retrieves the state of the tab control item identified by iItem.
  //
  // Parameters:
  //   iItem [in] The zero-based index number of the item for which to retrieve state information.
  //
  // Returns:
  //   Returns CRhinoUiTabbedSheetCtrlItem::item_state enum describing the current state of this
  //   tab control item.  Can be one of the following:
  //     normal_state        Tab is in normal state, not depressed or highlighted.
  //     pressed_state       The tab control item is selected.
  //     highlighted_state   The tab control item is highlighted.
  //   
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::item_state
  //   
  DWORD GetItemState( int iItem) const;
  //   
  // Description:
  //   Changes an items properties including display window and tab label.
  //
  // Parameters:
  //   iItem [in] The zero-based index number of the item for which to retrieve state information.
  //   item [in] Reference to a CRhinoUiTabbedSheetCtrlItem object that specifies the new attributes
  //             for the tab.
  // Returns:
  //   Returns true if item attributes have been successfully updated.
  //
  // See Also:
  //   class CRhinoUiTabbedSheetCtrlItem
  //   
  bool SetItem( int iItem, const CRhinoUiTabbedSheetCtrlItem& item);
  //   
  // Description:
  //   Removes all items from a tab control.
  //
  // Returns:
  //   Returns true if successful; otherwise false.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::DeleteItem()
  //   
  bool DeleteAllItems();
  //   
  // Description:
  //   Removes the specified item from a tab control.
  //
  // Parameters:
  //   iItem [in] The zero-based index number of the item to delete.
  //
  // Returns:
  //   Return true if successful; otherwise false.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::DeleteAllItems()
  //   
  bool DeleteItem( int iItem);
  //   
  // Description:
  //   Retrieves information about a tab in a tab control.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //   pTabCtrlItem [out] Pointer to a TCITEM structure, used to specify the information
  //                      to retrieve. Also used to receive information about the tab.
  //                      This structure is used with the InsertItem, GetItem, and SetItem
  //                      member functions.
  //
  // Returns:
  //    Returns true if successful; false otherwise.
  //
  // See Also:
  //   CTabCtrl::GetItem in the Platform SDK
  //   
  bool GetItem( int iItem, TCITEM* pTabCtrlItem) const;
  //   
  // Description:
  //   Sets some or all of a tab's attributes.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //   pTabCtrlItem [in] Pointer to a TCITEM structure that contains the new item attributes.
  //                     The mask member specifies which attributes to set. If the mask member
  //                     specifies the TCIF_TEXT value, the pszText member is the address of a
  //                     null-terminated string and the cchTextMax member is ignored. 
  // Returns:
  //   Return true if successful; otherwise false.
  //
  // See Also:
  //   CTabCtrl::GetItem in the Platform SDK
  //   
  bool SetItem( int iItem, TCITEM* pTabCtrlItem);
  //   
  // Description:
  //   This method retrieves the application-specific value associated with item specified by
  //   iItem.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Returns:
  //   A application-specific value associated with item specified by iItem.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   
  DWORD_PTR GetItemData(int iItem) const;
  //   
  // Description:
  //   This method sets the application-specific value associated with the item specified by iItem.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //   dwItemData [in] Specifies a application-specific value to be associated with this item.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetDataPtr()
  //   CRhinoUiTabbedSheetCtrlItem::SetDataPtr()
  //   
  int SetItemData(int iItem, DWORD_PTR dwItemData);
  //   
  // Description:
  //   This method retrieves the application-specific value associated with the item specified
  //   by iItem.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Returns:
  //   A application-specific pointer associated with iItem or NULL if none.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   CRhinoUiTabbedSheetCtrlItem::SetDataPtr()
  //   CRhinoUiTabbedSheetCtrlItem::GetDataPtr()
  //   
  void* GetItemDataPtr(int iItem) const;
  //   
  // Description:
  //   This method sets the application-specific value associated with the item specified
  //   by iItem.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Parameters:
  //   pData [in] Specifies a application-specific value to be associated iItem.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetDataPtr()
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetData()
  //   CRhinoUiTabbedSheetCtrlItem::SetData()
  //   CRhinoUiTabbedSheetCtrlItem::GetDataPtr()
  //   
  int SetItemDataPtr(int iItem, void* pData);
  //   
  // Description:
  //   Remove item from display but not from tab control.  This is useful to temporarily
  //   hide a tab.  Hidding the current tab will result in the current selection being set
  //   to none.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab to hide.
  //
  // Returns:
  //   Returns true if tab was hidden successfully.
  //
  bool HideItem( int iItem, bool bHide = true);
  //   
  // Description:
  //   Call this helper method to determine if a tab index value is valid.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Returns:
  //   Returns true if iItem is greater than zero and less than GetItemCount()
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetItemCount()
  //   
  bool ItemIndexIsValid( int iItem) const;
  //   
  // Description:
  //   Call this method to have tab display and hit test elements update.  This is called when
  //   adding, deleting, hiding or scrolling.
  //
  void RecalcLayout();
  //   
  // Description:
  //   Determines which tab, if any, is at the specified screen position. 
  //
  // Parameters:
  //   pt [in] Screen point
  //
  // Returns:
  //   Returns the zero-based index of the tab or – 1 if no tab is at the
  //   specified position.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::ButtonFromPoint()
  //   
  int ItemIndexFromPoint( CPoint pt) const;
  enum scroll_button
  {
    sb_invalid = 0,
    sb_first,
    sb_last,
    sb_next,
    sb_previous,
    sb_add,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_scroll_button = 0xFFFFFFFF,
  };
  //   
  // Description:
  //   Determines which scroll button, if any, is at the specified screen position. 
  //
  // Parameters:
  //   pt [in] Screen point
  //
  // Returns:
  //   scroll_button Which identifies the scroll button or sb_invalid if no button is
  //   at the specified position.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::ItemIndexFromPoint()
  //   
  scroll_button ButtonFromPoint( CPoint pt) const;
  //   
  // Description:
  //   Call this method to force the tab control to redraw itself.
  //
  void Redraw();
  //   
  // Description:
  //   Create tab control window.
  //
  // Parameters:
  //   pParent [in] Parent window for this tab control.
  //   nID [in] The ID of the child window.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  bool CreateTabbedSheetCtrl( CWnd* pParent, UINT nID, LPRECT lpRect = NULL);
  //   
  // Description:
  //   Replace a dialog item with this control 
  //
  // Parameters:
  //   pParent [in] Dialog to embed tab control inside of.
  //   nID [in] The ID of the dialog control to replace.  The new
  //            tab control will be created using this ID.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::CreateTabbedSheetCtrl()
  //   
  bool ReplaceDlgItem( CDialog* pParent, UINT nID);
  //   
  // Description:
  //   Call this method to find out if the tabs are aligned horizontally on the top
  //   or bottom of the control.
  //
  // Returns:
  //   Returns true if tabs are aligned at the top or bottom of the control otherwise,
  //   returns false.
  //   
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::IsVertical()
  //   CRhinoUiTabbedSheetCtrl::Orientation()
  //   CRhinoUiTabbedSheetCtrl::orientation enum
  //   
  bool IsHorizontal() const;
  //   
  // Description:
  //   Call this method to find out if the tabs are aligned vertically on the right
  //   or left side of the control.
  //
  // Returns:
  //   Returns true if tabs are aligned at the right or bottom of the control otherwise,
  //   returns false.
  //   
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::IsHorizontal()
  //   CRhinoUiTabbedSheetCtrl::Orientation()
  //   CRhinoUiTabbedSheetCtrl::orientation enum
  //   
  bool IsVertical() const;
  //   
  // Description:
  //   Call this method to find out if layout is recalculated when calling AddItem() or
  //   InsertItem() methods.  You may want to temporarily disable this if you are adding
  //   multiple items.
  //
  // Returns:
  //   Returns true if tab layout will be recalculated each time AddItem() or InsertItem()
  //   methods are called otherwise; returns false.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetRecalcOnAddOrInsert()
  //   CRhinoUiTabbedSheetCtrl::RecalcLayout()
  //   
  bool RecalcOnAddOrInsert() const;
  //   
  // Description:
  //   Call this method to control whether or not AddItem() and InsertItem() methods will
  //   call RecalcLayout() after adding a new tab to the control.  You may want to temporarily
  //   disable this functionality when adding many items to a control.  If you disable this
  //   functionality you are responsible for calling RecalcLayout() to get new tabs to appear.
  //
  // Parameters:
  //   b [in] If true then RecalcLayout() is not called when adding new items otherwise it is.
  //
  // Returns:
  //   Previous value.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::RecalcLayout()
  //   
  bool SetRecalcOnAddOrInsert( bool b);
  //   
  // Description:
  //   By default active tabs are drawing using ::GetSysColor( COLOR_WINDOW) and inactive tabs
  //   are drawn using ::GetSysColor( COLOR_BTNFACE).  You may override this behavior by setting
  //   a color to a value other than ON_UNSET_COLOR.  Calling this method without any arguments
  //   will reset the colors to there default values.
  //
  // Parameters:
  //   crActiveTab [in] Color to use when drawing the active tab background.
  //   crActiveText [in] Color to use when drawing the active tab caption text.
  //   crNormalTab [in] Color to use when drawing an inactive tab background.
  //   crNormalText [in] Color to use when drawing an inactive tab caption text.
  //
  // Returns:
  //   Returns true if colors are changed.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetControlColors()
  //   
  bool SetControlColors( COLORREF crActiveTab = ON_UNSET_COLOR,
                         COLORREF crActiveText = ON_UNSET_COLOR,
                         COLORREF crNormalTab = ON_UNSET_COLOR,
                         COLORREF crNormalText = ON_UNSET_COLOR
                       );
  //
  // Description:
  //   By default active tabs are drawing using ::GetSysColor( COLOR_WINDOW) and inactive tabs
  //   are drawn using ::GetSysColor( COLOR_BTNFACE).  You may override this behavior by setting
  //   a color to a value other than ON_UNSET_COLOR.  Cal this method to determine the current
  //   draw colors.
  //
  // Parameters:
  //   crActiveTab [out] Color to use when drawing the active tab background.
  //   crActiveText [out] Color to use when drawing the active tab caption text.
  //   crNormalTab [out] Color to use when drawing an inactive tab background.
  //   crNormalText [out] Color to use when drawing an inactive tab caption text.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::SetControlColors()
  //   
  void GetControlColors( COLORREF& crActiveTab,
                         COLORREF& crActiveText,
                         COLORREF& crNormalTab,
                         COLORREF& crNormalText
                       ) const;
  //   
  // Description:
  //   Call this method if to determine if you can currently see the tab specified by iItem
  //   on the screen.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Returns:
  //   Returns true if the entier tab is currently visible on the screen.
  //
  bool CanSeeItem( int iItem) const;
  //   
  // Description:
  //   Use this method to begin in-place editing of the specified text in the tab.
  //
  // Parameters:
  //   iItem [in] Zero-based index of the tab.
  //
  // Returns:
  //   Returns a pointer to the in-place edit control if successful otherwise returns NULL.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::GetEditControl()
  //   CRhinoUiTabbedSheetCtrl::CancelEditLabel()
  //
  CEdit* EditLabel( int iItem);
  //   
  // Description:
  //   This method retrieves a pointer to the edit control used to edit text in a tab.
  //
  // Returns:
  //   If successful, a pointer to the CEdit object that is used to edit the item text; otherwise,
  //   it is NULL.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::EditLabel()
  //   CRhinoUiTabbedSheetCtrl::CancelEditLabel()
  //
  CEdit* GetEditControl() const;
  //   
  // Description:
  //   Cancels tab item text editing operation.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::EditLabel()
  //   CRhinoUiTabbedSheetCtrl::GetEditControl()
  //
  void CancelEditLabel();
protected:
  friend class CRhinoUiTabbedSheetCtrlGDI;
  //   
  // Description:
  //   Clean up m_item_list pointers.
  //
  void HoseItemList();
  //   
  // Description:
  //   Create in-place edit control used by EditLable() method
  //
  // Returns:
  //   Returns true if edit control was created successfully or already existed or false if
  //   unable to create the control.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::EditLabel()
  //
  bool CreateEdit();
  //   
  // Description:
  //   Send message to parent to decide whether or not to commit new text to the tab.  If parent
  //   says it is okay then update the text and invalidate the control.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::CreateEdit()
  //   CRhinoUiTabbedSheetCtrl::EditLabel()
  //
  void EndEdit();
  bool ShowDragWindow( int iItem, bool bShow, CPoint point);
  void DestroyDragWindow();
  //   
  // Description:
  //   Send specified message to the parent of this control, if the message returns zero then
  //   send the message to this window also.  The message will only be sent to the parent if
  //   bToParent is true.
  //
  // Parameters:
  //   bToParent [in] If bToParent is true then message is sent to the parent window.  If the parent
  //                  window returns non-zero then nothing else happens.  If the parent window
  //                  returns zero then the message is also sent to this control.
  //                  If bToParent is false then the message is sent directly to this control.
  //   info [in/out] Message ID and parameters to send to window(s)
  //
  // Returns:
  //   Returns value received by CWnd::SendMessage().
  //
  // See Also:
  //   CWnd::SendMessage()
  //   class CRhinoUiTabbedSheetCtrlMsg
  //   CRhinoUiTabbedSheetCtrlMsg::InitCustomInfo()
  //   
  LRESULT SendTabbedSheetCustomMessage( bool bToParent, CRhinoUiTabbedSheetCtrlMsg::CUSTOMINFO& info);
  //   
  // Description:
  //   Call this method to get the font used for drawing tab label text.
  //
  // Parameters:
  //   bBold [in] If true then the bold version of the font is returned otherwise the normal
  //              font is returned.
  //   bHorizontal [in] If true then a normal, horizontal font is returned otherwise a vertical
  //                    font is returned.  The vertical font that is returned is controlled by
  //                    the bottom_up argument.  If bottom up is true then a font with lfEscapement
  //                    and lfOrientation of 900 (90 degrees) is returned otherwise; a font with
  //                    lfEscapement and lfOrientation of 2700 (270 degrees) is returned.
  //   bottom_up [in] Only valid if bHorizontal is false.  If bottom up is true then a font with
  //                  lfEscapement and lfOrientation of 900 (90 degrees) is returned otherwise; a
  //                  font with lfEscapement and lfOrientation of 2700 (270 degrees) is returned.
  //
  // Returns:
  //   Pointer to the specified font if successful or NULL if not.
  //
  // See Also:
  //   CTabSheetDlgTest::GetDrawFont( bool bBold)
  //   
  CFont* GetDrawFont( bool bBold, bool bHorizontal, bool bottom_up) const;
  //   
  // Description:
  //   Call this method to get the font used for drawing tab label text.
  //
  // Parameters:
  //   bBold [in] If true then the bold version of the font is returned otherwise the normal
  //              font is returned.  The font angle is determined by the current orientation.
  //              For orient_top and orient_bottom positions a horizontal font is returned.
  //              For the orient_left position a font with an angle of 90 is returned.  For
  //              the orient_right position a font with an angle of 270 is returned.
  //
  // Returns:
  //   Pointer to the specified font if successful or NULL if not.
  //
  // See Also:
  //   CTabSheetDlgTest::GetDrawFont( bool bBold, bool bHorizontal, bool bottom_up)
  //   CTabSheetDlgTest::Orientation()
  //   
  CFont* GetDrawFont( bool bBold) const;
  //   
  // Description:
  //   Call this method to make sure the scroll button rectangles and bitmap match the current
  //   tab orientation.  This will only update if ResetScrollButtons() has been called first.
  //
  // Parameters:
  //   rItem [in/out] Current area available to position the scroll buttons.  This rect will be
  //                  adjusted removing the scroll buttons and reflecting what area is left for
  //                  the tabs.
  //
  // See Also:
  //   CTabSheetDlgTest::ResetScrollButtons()
  //   
  void MakeButtonDibs( CRect& rItem, int cy);
  //   
  // Description:
  //   Call this method to draw the scroll buttons if any are visible.  This will draw the buttons
  //   in there current state.
  //
  // Parameters:
  //   dc [in] Device context to draw to.
  //   sb [in] Button to draw.
  //
  // Returns:
  //   Returns true if a button was drawn or false if it was not.
  //
  bool DrawScrollButton( CDC& dc, scroll_button sb);
  //   
  // Description:
  //   Method called by the scroll button click event handler or the scroll timer.
  //
  // Parameters:
  //   sb [in] Scroll button action.
  //
  // Returns:
  //   Returns true if a scroll action has occurred.
  //
  // See Also:
  //   
  //   
  bool DoScrollButton( scroll_button sb);
  //   
  // Description:
  //   Call this method to clear the scroll timer flags.
  //
  void KillScrollTimer();
  //   
  // Description:
  //   Call this method to clear the scroll button tool tip timer flags.
  //
  void KillTooltipTimer();
  //   
  // Description:
  //   Call this method to kill the timer and mouse capture associated with a 
  //   scroll button mouse over active.
  //
  void KillScrollMouseHover();
  //   
  // Description:
  //   Call this method to force scroll buttons to redraw.
  //
  void RedrawButtons();
  //   
  // Description:
  //   Call this method when the current selection has been changed to force TCN_SELCHANGE
  //   and TCN_SELCHANGING messages to be sent to this controls parent window.  Also sends
  //   CRhinoUiTabbedSheetCtrl::m_sel_changed_custom_win_msg registered windows message.
  //
  // Parameters:
  //   bChanged [in] If true then the current selection has been changed otherwise it is
  //                 about to change.
  //
  // See Also:
  //   CRhinoUiTabbedSheetCtrl::m_sel_changed_custom_win_msg
  //   
  void SendSelChangedMessage( bool bChanged, int iPrevious, int iCurrent);
  //   
  // Description:
  //   Call this methods to force RecalcLayout() to recalculate scroll button rectangles and
  //   button bitmaps.
  //
  void ResetScrollButtons();
  enum timers
  {
    scroll_timer = 100,
    tool_tip_timer,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_timers = 0xFFFFFFFF,
  };
  enum mouse_capture
  {
    no_mouse_capture = 0,
    scroll_mouse_capture,
    scroll_hover_mouse_capture,
    drag_tab_capture,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_mouse_capture = 0xFFFFFFFF,
  };

protected:
  orientation m_orientation;
  CRhinoUiTabbedSheetCtrlGDI* m_gdi;
  int m_iCurSel;
  int m_iFirstVisible;
  int m_iLastVisible;
  int m_iFirstVisibleIndex;
  int m_iLastVisibleIndex;
  int m_iVisible;
  ON_SimpleArray<CRhinoUiTabbedSheetCtrlItem*>m_item_list;
  CRect m_rPage;
  CRect m_rCtrlClient;
  CRect m_rPageBorders;
  bool m_bIncludeFirstAndLastButtons;
  bool m_bIncludeNextAndPreviousButtons;
  bool m_bEditLabelOnDoubleClick;
  CRect m_rTabs;
  CRect m_rButtons;
  CRect m_rFirst;
  CRect m_rLast;
  CRect m_rPrev;
  CRect m_rNext;
  CRhinoDib m_button_dib;
  CRhinoDib m_button_dib_disabled;
  static COLORREF m_crMask;
  scroll_button m_scroll_timer_button;
  scroll_button m_hot_scroll_button;
  scroll_button m_mouse_down_on_scroll_button;
  scroll_button m_pressed_scroll_button;
  mouse_capture m_mouse_capture;
  bool m_bRecalcOnAddOrInsert;
  COLORREF m_crActiveTab;
  COLORREF m_crActiveText;
  COLORREF m_crNormalTab;
  COLORREF m_crNormalText;
  int m_iEdit;
  class CRhinoUiInPlaceEdit* m_pEdit;
  CPoint m_lmouse_down_pt;
  CRhinoUiDragBitmapWnd* m_drag_window;
  static CRhinoDib m_drag_dib;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnNcPaint();
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
  afx_msg LRESULT OnEndEditMessage(WPARAM wParam, LPARAM lParam);
  virtual void PreSubclassWindow();
public:
  afx_msg BOOL32 OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  class CInternalRhUiTabbedSheetCtrl* m_internal_tabbed_sheet_ctrl = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiTabbedSheetCtrlGDI
//
class RHINO_SDK_CLASS CRhinoUiTabbedSheetCtrlGDI
{
public:
  CRhinoUiTabbedSheetCtrlGDI( CRhinoUiTabbedSheetCtrl& tabctrl);
  void Init();
  CRhinoUiTabbedSheetCtrl& m_tabctrl;
  CBrush m_brush_active;
  CBrush m_brush_inactive;
  CPen m_pen_edge;
  CPen m_pen_white;
  CPen m_pen_black;
  COLORREF m_crActiveTab;
  COLORREF m_crActiveText;
  COLORREF m_crNormalTab;
  COLORREF m_crNormalText;
};

