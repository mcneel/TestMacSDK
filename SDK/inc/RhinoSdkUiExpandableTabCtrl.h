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

/////////////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiExpandableTabCtrlItem
//
// Description:
//  Control class for tab items.  Used by CRhinoUiExpandableTabCtrl class below.
//
class RHINO_SDK_CLASS CRhinoUiExpandableTabCtrlItem
{
public:
  CRhinoUiExpandableTabCtrlItem();
  virtual ~CRhinoUiExpandableTabCtrlItem() = default;
  CRhinoUiExpandableTabCtrlItem( const CRhinoUiExpandableTabCtrlItem&);
  const CRhinoUiExpandableTabCtrlItem& operator=( const CRhinoUiExpandableTabCtrlItem&);

  // Description:
  //   Tab label
  // Returns:
  //   Text which will left aligned on the tab
  LPCTSTR Name() const;

  // Description:
  //   Index into image list, if provided, which will appear to the left of Name()
  // Returns:
  //   Only meaningful if CRhinoUiExpandableTabCtrl::SetImageList() called.  If so
  //   it is a zero based index of the image to be displayed to the left of the tab
  //   name.
  int ImageIndex() const;

  // Description:
  //   States the tab page may be in.  They are not mutually exclusive.
  enum item_states
  {
    is_expanded = 0, // Tab page is visible
    is_active,       // Mouse is currently over this item and so it is highlighted
    is_hidden,       // If this is set to true then the tab does not appear in the tab control
    is_pressed,
    is_invalid       // For internal use, should always be last item in list
  };
  //   
  // Description:
  //   Check to see if an item state is set.
  //
  // Parameters:
  //   flag [in] Flag to check
  //
  // Returns:
  //   If flag is set then returns true otherwise false.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrlItem::SetItemState()
  //   
  bool ItemState( item_states flag) const;
  //   
  // Description:
  //   Check to see if point is over this tab or tab page.
  //
  // Parameters:
  //   pt [in] Point to test in tab control client window coordinates.
  //   bCheckPageRect [in] If true will check to see if point is over
  //                       client are of tab page.  If false then only
  //                       checks tab label.
  // Returns:
  //   Returns true if point is over the tab.  If bCheckPageRect is true
  //   then true will be returned of point is over the page.
  //
  bool PointOverItem( CPoint pt, bool bCheckPageRect = false) const;
  //   
  // Description:
  //   Get the dialog page associated with this item.
  //
  // Returns:
  //   If the tab has been added and the page has been created then a
  //   valid pointer to the page associated with this item will be returned.
  //   If the page has not been created then NULL is returned.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::AddPage()
  //   CRhinoUiExpandableTabCtrl::InsertPage()
  //   
  CDialog* GetDialogPage() const;
  //   
  // Description:
  //   Get the size of the page associated with this item.
  //
  // Returns:
  //   Return the size of the dialog page associated with this item.
  //
  CSize GetPageSize() const;
  //   
  // Description:
  //   The tab rectangle is the rectangle used to display the tab name
  //   and expand/contract button.
  //
  // Returns:
  //   Tab caption rectangle if item is not hidden.  If item is hidden an
  //   empty rectangle is returned.
  //
  CRect GetTabRect() const;
  //   
  // Description:
  //   Display rectangle associated with this item if it is expanded.  If
  //   the item is hidden or contracted an empty rectangle is returned.
  //
  // Returns:
  //   Display rectangle associated with this item if it is expanded.  If
  //   the item is hidden or contracted an empty rectangle is returned.
  //
  CRect GetPageRect() const;
  //   
  // Description:
  //   Get pointer to tab control this item is inserted in.
  //
  // Returns:
  //   Returns pointer to tab control this item is inserted in or NULL
  //   if not inserted yet.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::AddPage()
  //   CRhinoUiExpandableTabCtrl::InsertPage()
  //   
  class CRhinoUiExpandableTabCtrl* GetTabCtrl() const;
  //   
  // Description:
  //   Test to see if point is over item.
  //
  // Parameters:
  //   pt [in] Point in tab control client window coordinates.
  //
  // Returns:
  //   Returns CRhinoUiExpandableTabCtrl::hit_test value which identifies
  //   which part, if any, the point is over.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::hit_test
  //   
  int HitTest( CPoint pt) const;
  //   
  // Description:
  //   Check to see if hidden state is set.
  //
  // Returns:
  //   Returns true if CRhinoUiExpandableTabCtrlItem::is_hidden flag is set.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrlItem::SetItemState()
  //   CRhinoUiExpandableTabCtrlItem::ItemState()
  //   
  bool IsTabHidden() const;
protected:
  friend class CRhinoUiExpandableTabCtrl;
  friend class CRhinoUiModlessDockingDialogManager;
  //   
  // Description:
  //   If the tab control has a scroll bar visible then adjust the rectangle
  //   to reflect the current scroll bar position.
  //
  // Parameters:
  //   r [in/out] Rectangle to adjust
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ScrollTabClientRect()
  //   
  void ScrollTabClientRect( CRect& r) const;
  //   
  // Description:
  //   If the tab control has a scroll bar visible then adjust the point
  //   to reflect the current scroll bar position.
  //
  // Parameters:
  //   pt [in/out] Point to adjust
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ScrollTabClientPoint()
  //   
  void ScrollTabClientPoint( CPoint& pt) const;
  //   
  // Description:
  //   Set CRhinoUiExpandableTabCtrlItem::item_states flag.
  //
  // Parameters:
  //   flag [in] Item state flag to set
  //   b [in] New value for item state flag
  //
  // Returns:
  //   Returns previous item state flag value.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrlItem::ItemState()
  //   
  bool SetItemState( item_states flag, bool b);
  //   
  // Description:
  //   Get the expand/contract button rectangle.
  //
  // Returns:
  //   Expand/contract button rectangle in tab control client window units.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrlItem::ScrollTabClientRect()
  //   
  CRect GetExpandContractButtonRect() const;
  //   
  // Description:
  //   Draw tab caption
  //
  // Parameters:
  //   dc [in] Tab control device context to be used when drawing.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrlItem::GetTabRect()
  //   CRhinoUiExpandableTabCtrlItem::ItemState()
  //   CRhinoUiExpandableTabCtrlItem::DrawItemArrow()
  //
  virtual void DrawItem( CDC& dc);

  bool UseSpecialColors() const;
private:
  class CRhinoUiExpandableTabCtrl* m_tab_ctrl;
  CString m_sName;
  int m_nImageIndex;
  CRect m_r;
  CRect m_rPage;
  CSize m_szPage;
  BYTE m_item_states[is_invalid];
  CDialog* m_page;
  class CRhinoUiExpandableTabPage* m_dlg_page;
};


/////////////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiExpandableTabCtrl
//
// Description:
//   This is a control that features tabs stacked at the top or left side of the window.  The
//   tabs feature hot tracking.  Hot tracking is automatic highlighting when mouse is over tab.
//   Tab pages drop down from or to the right of tab.  Multiple tabs may be expanded at the
//   same time.
//

// Forward decloration
class CRhinoUiDragBitmapWnd;

class RHINO_SDK_CLASS CRhinoUiExpandableTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CRhinoUiExpandableTabCtrl)
public:
  //   
  // Description:
  //   Constructor
  //
	CRhinoUiExpandableTabCtrl();
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiExpandableTabCtrl();
  //   
  // Description:
  //   Custom event notification messages sent to parent use the following format:
  //     message = CRhinoUiExpandableTabCtrl::m_custom_win_msg_id
  //               (static UINT holding ID of registered Windows message)
  //     WPARAM = LOWORD is notification ID, HIWORD is message dependent
  //     LPARAM = Message dependent
  //
  //   To use these notification codes do add the message map macro to the parent
  //   of this control:
  //     ON_REGISTERED_MESSAGE( CRhinoUiExpandableTabCtrl::m_custom_win_msg_id, OnRegisteredMessage)
  //   Add this to the decloration of the the parent window class:
  //     afx_msg LRESULT OnRegisteredMessage( WPARAM wParam, LPARAM lParam);
  //   Add this method to the class implementation
  //     LRESULT CMyDialog::OnRegisteredMessage( WPARAM wParam, LPARAM lParam)
  //     {
  //       int tab_notification_id = LOWORD( wParam);
  //       int hi_word = HIWORD( wParam);
  //       CDialog* pPage = NULL;
  //       switch( tab_notification_id)
  //       {
  //         case CRhinoUiExpandableTabCtrl::etcn_expand:
  //           pPage = (CDialog*)lParam;
  //           // expanded == hi_word
  //           return true;
  //         case CRhinoUiExpandableTabCtrl::etcn_hide:
  //           pPage = (CDialog*)lParam;
  //           // hidden = hi_word
  //           return true;
  //       }
  //       return 0L;
  //     }
  //
  enum notification_ids
  {
    // message = CRhinoUiExpandableTabCtrl::m_custom_win_msg_id
    // WPARAM LOWORD = etcn_expand
    //        HIWROD = Bool, if true then tab has just been expanded
    //                 otherwise it has just been contracted.
    // LPARAM = Pointer to CRhinoUiExpandableTabCtrlItem item being
    //          expanded or contracted.
    etcn_expand = 0,
    // message = CRhinoUiExpandableTabCtrl::m_custom_win_msg_id
    // WPARAM LOWORD = etcn_hide
    //        HIWROD = Bool, if true then tab has just been hidden
    //                 otherwise it has just become visible.
    // LPARAM = Pointer to CRhinoUiExpandableTabCtrlItem item being
    //          hidden or shown.
    etcn_hide,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_notification_ids = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Style and mode flags associated with tab control.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::IsExpandableTabFlagFlagSet()
  //   CRhinoUiExpandableTabCtrl::SetExpandableTabFlag()
  //   
  enum expandable_tab_flags
  {
    etf_vertical_tabs = 0, // Display tabs on left side of control, default is false
    etf_allow_drag_drop,   // Allow dragging of tabs to rearrange
    etf_user_1, // reserved for future use
    etf_user_2, // reserved for future use
    etf_user_3, // reserved for future use
    etf_user_4, // reserved for future use
    etf_user_5, // reserved for future use
    etf_user_6, // reserved for future use
    etf_user_7, // reserved for future use
    etf_user_8, // reserved for future use
    etf_user_9, // reserved for future use
    etf_user_10, // reserved for future use
    // Invalid should always be last, it is used to size flag byte array
    etf_invalid
  };
  //   
  // Description:
  //   Check to see if mode flag is set.
  //
  // Parameters:
  //   flag [in] Flag value to check.
  //
  // Returns:
  //   Returns true if flag is set, false if it is not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::SetExpandableTabFlag()
  //   
  bool IsExpandableTabFlagFlagSet( expandable_tab_flags flag) const;
  //   
  // Description:
  //   Set mode flag
  //
  // Parameters:
  //   flag [in] Flag value to set.
  //   b [in] New value for flag
  //
  // Returns:
  //   Previous value for flag
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ExpandableTabFlag()
  //   
  bool SetExpandableTabFlag( expandable_tab_flags flag, bool b);
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
  // See Also:
  //   CRhinoUiExpandableTabCtrl::SetExpandableTabFlag()
  //   
  bool CreateExpandableTabCtrl( CWnd* pParent, UINT nID, LPRECT lpRect = NULL);
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
  //   CRhinoUiExpandableTabCtrl::CreateExpandableTabCtrl()
  //   
  bool ReplaceDlgItem( CDialog* pParent, UINT nID);
  //   
  // Description:
  //   Add new page to bottom of tab stack.
  //
  // Parameters:
  //   lpszItem [in] Tab caption
  //   nImage [in] Index of image to display to left of caption
  //   lpszTemplateName [in] The null-terminated string that is the name of a dialog-box
  //                         template resource. 
  //   szHtmlResID [in] The null-terminated string that is the name of an HTML resource. 
  //   nIDTemplate [in] Contains the ID number of a dialog-box template resource. 
  //   nHtmlResID [in] Contains the ID number of an HTML resource. 
  //   pRC [in] Class to create and add to control.  Use RUNTIME_CLASS( CMyClass) macro to format this parameter.
  //            Class decloration must include 	DECLARE_DYNCREATE(CMyClass) macro.  Class implementation must
  //            include IMPLEMENT_DYNCREATE(CMyClass, CRhinoUiDialog) macro.
  //   pMS [in] This must be your DLL application module state.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::InsertItem()
  //   AfxGetStaticModuleState()
  //   
  virtual bool AddPage( LPCTSTR lpszItem, int nImage, LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  virtual bool AddPage( LPCTSTR lpszItem, int nImage, UINT nIDTemplate, UINT nHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  //   
  // Description:
  //   Insert a new page or add new page to bottom of tab stack.
  //
  // Parameters:
  //   nItem [in] Position to insert item.  If nItem is less than zero or greater than or equal to the number
  //              of items in the control, the item is appened to the bottom of the stack.
  //   lpszItem [in] Tab caption
  //   nImage [in] Index of image to display to left of caption
  //   lpszTemplateName [in] The null-terminated string that is the name of a dialog-box
  //                         template resource. 
  //   szHtmlResID [in] The null-terminated string that is the name of an HTML resource. 
  //   nIDTemplate [in] Contains the ID number of a dialog-box template resource. 
  //   nHtmlResID [in] Contains the ID number of an HTML resource. 
  //   pRC [in] Class to create and add to control.  Use RUNTIME_CLASS( CMyClass) macro to format this parameter.
  //            Class decloration must include 	DECLARE_DYNCREATE(CMyClass) macro.  Class implementation must
  //            include IMPLEMENT_DYNCREATE(CMyClass, CRhinoUiDialog) macro.
  //   pMS [in] This must be your DLL application module state.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::AddItem()
  //   AfxGetStaticModuleState()
  //   
  virtual bool InsertItem( int nItem, LPCTSTR lpszItem, int nImage, LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  virtual bool InsertItem( int nItem, LPCTSTR lpszItem, int nImage, UINT nIDTemplate, UINT nHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  //   
  // Description:
  //   Insert a new page or add new page to bottom of tab stack.
  //
  // Parameters:
  //   nItem [in] Position to insert item.  If nItem is less than zero or greater than or equal to the number
  //              of items in the control, the item is appened to the bottom of the stack.
  //   lpszItem [in] Tab caption
  //   nImage [in] Index of image to display to left of caption
  //   lpszTemplateName [in] The null-terminated string that is the name of a dialog-box
  //                         template resource. 
  //   szHtmlResID [in] The null-terminated string that is the name of an HTML resource. 
  //   nIDTemplate [in] Contains the ID number of a dialog-box template resource. 
  //   nHtmlResID [in] Contains the ID number of an HTML resource. 
  //   pRC [in] Class to create and add to control.  Use RUNTIME_CLASS( CMyClass) macro to format this parameter.
  //            Class decloration must include 	DECLARE_DYNCREATE(CMyClass) macro.  Class implementation must
  //            include IMPLEMENT_DYNCREATE(CMyClass, CRhinoUiDialog) macro.
  //   pMS [in] This must be your DLL application module state.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::AddItem()
  //   AfxGetStaticModuleState()
  //   
  virtual bool OnItemDroppedOnOtherWindow( int drag_item_index, CPoint point);
  //   
  // Description:
  //   Remove tab from display but not from internal tab list.  This allows hiding and displaying of tabs
  //   in there original position and state.
  //
  // Parameters:
  //   iIndex [in] Index of tab to hide
  //   bHide [in] If true tab is hidden; otherwise tab is displayed
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::IsTabHidden()
  //   
  bool HideTab( int iIndex, bool bHide = true);
  //   
  // Description:
  //   Check to see if tab is hidden
  //
  // Parameters:
  //   iIndex [in] Index of tab to check
  //
  // Returns:
  //   Returns true if tab is hidden; otherwise false
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::HideTab()
  //   
  bool IsTabHidden( int iIndex) const;
  //   
  // Description:
  //   Remove tab and destroy tab page dialog.
  //
  // Parameters:
  //   iIndex [in] Index of page to delete.
  //
  // Returns:
  //   Returns true if iIndex is valid and deleted, false if not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::DeleteAllItems()
  //   
  bool DeleteItem(int nItem);
  //   
  // Description:
  //   Delete and destroy all tab pages in control.
  //
  // Returns:
  //   Returns true if successful; otherwise false
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::DeleteItem()
  //   
  bool DeleteAllItems();
  //   
  // Description:
  //   The tab gap is the space between the bottom of the tab caption and the next tab when
  //   a tab is contracted.  If the tab is expanded it is the space between the bottom of the
  //   tab page and the next tab.
  //
  //   If the tab is vertically aligned the space is not at the bottom but on the right side
  //   of the tab.
  //
  // Returns:
  //   Returns gap in client window units.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::SetTabGap()
  //   
  int GetTabGap() const;
  //   
  // Description:
  //   The tab gap is the space between the bottom of the tab caption and the next tab when
  //   a tab is contracted.  If the tab is expanded it is the space between the bottom of the
  //   tab page and the next tab.
  //
  //   If the tab is vertically aligned the space is not at the bottom but on the right side
  //   of the tab.
  //
  // Parameters:
  //   nGap [in] The new gap in client window units.
  //
  // Returns:
  //   Returns previous gap
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetTabGap()
  //   
  int SetTabGap( int nGap);
  //   
  // Description:
  //   Determine tab index based on a client window point.
  //
  // Parameters:
  //   pt [in] Point in client window coordinates.
  //   bCheckPageRect [in] Check to see if point is over page as well as tab caption.
  //
  // Returns:
  //   Returns zero based index of tab page if found or -1 if not found.
  //
  int ItemIndex( CPoint pt, bool bCheckPageRect = false) const;
  //   
  // Description:
  //   Determine tab index based on dialog page pointer.
  //
  // Parameters:
  //   pPage [in] Find index of tab associated with this page
  //
  // Returns:
  //   Returns zero based index of tab page if found or -1 if not found.
  //
  int ItemIndex( const CDialog* pPage) const;
  //   
  // Description:
  //   Number of tabs currently added to this control.  Includes hidden tabs.
  //
  // Returns:
  //   Returns number of items in tab control including hidden items.
  //
  int GetItemCount() const;
  //   
  // Description:
  //   Get the tab control item associated with a particular index.
  //
  // Parameters:
  //   nItem [in] Index of item to get.
  //
  // Returns:
  //   Return valid pointer if nItem is valid or NULL if not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetItemCount()
  //   
  const CRhinoUiExpandableTabCtrlItem* GetItem( int nItem) const;
  //   
  // Description:
  //   Get the tab control item associated with a particular page.
  //
  // Parameters:
  //   pPage [in] Page to search for.
  //
  // Returns:
  //   Return valid pointer if the dialog page is found or NULL if not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetItemCount()
  //   
  const CRhinoUiExpandableTabCtrlItem* GetItem( const CDialog* pPage) const;
  //   
  // Description:
  //   Get index of page that currently has focus.
  //
  // Returns:
  //   Returns index of page that currently has focus
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetActiveItem()
  //   CRhinoUiExpandableTabCtrl::SetActiveItem()
  //   
  int GetActiveItemIndex() const;
  //   
  // Description:
  //   Get pointer to item that currently has focus.
  //
  // Returns:
  //   Returns pointer to item that currently has focus or NULL if none found.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetActiveItemIndex()
  //   CRhinoUiExpandableTabCtrl::SetActiveItem()
  //   
  const CRhinoUiExpandableTabCtrlItem* GetActiveItem() const;
  //   
  // Description:
  //   Make sure page is visible and expanded and set focus to page.
  //
  // Parameters:
  //   nItem [in] Index of page to activate.
  //
  // Returns:
  //   Returns true if page found and successfully expanded or false if not
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetActiveItem()
  //   CRhinoUiExpandableTabCtrl::GetActiveItemIndex()
  //   
  bool SetActiveItem( int nItem);
  //   
  // Description:
  //   Expand or contract tab item page.
  //
  // Parameters:
  //   nItem [in] Zero based index of page to expand or contract
  //   bExpand [in] If true then page is expanded; otherwise page is contracted
  //
  // Returns:
  //   Returns true on success; otherwise false
  //
  bool ExpandItem( int nItem, bool bExpand = true);
  //   
  // Description:
  //   Get horizontal or vertical SCROLLINFO used to control scorll bars.
  //
  // Parameters:
  //   bVert [in] If true then vertical scroll bar info is returned otherwise
  //              horizontal scroll bar info is returned.
  //
  // Returns:
  //   const Reference to SCROLLINFO used to control scroll bar.
  //
  const SCROLLINFO& GetTabCtrlScrollInfo( bool bVert) const;
  //   
  // Description:
  //   Valid places a point may land on a tab control or item
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::HitTest()
  //   
  enum hit_test
  {
    ht_caption = 0,            // On a tab item caption
    ht_expand_contract_button, // On a tab item caption expand/contract button
    ht_client,                 // On expanded tab page client
    ht_item_stretch,           // gap between tabs
    ht_invalid, // Hit test failed, not over a tab item.  Should always be last
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_hit_test = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Check to see if a tab control client point is over any part of a tab
  //   control item.
  //
  // Parameters:
  //   pt[in] Point in client window coordinates to test
  //   pItemIndex [out] If non NULL it receives index of item the point is over if
  //                    the point is over an item.
  //
  // Returns:
  //   Returns hit_test value indicating what the point is over.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::hit_test
  //   
  hit_test HitTest( CPoint pt, int* pItemIndex = NULL) const;
  //   
  // Description:
  //   Adjust client window point to reflect current scroll bar position.  Does nothing
  //   if scroll bar is not currently visible.
  //
  // Parameters:
  //   pt [in/out] Point to adjust.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ScrollTabClientRect()
  //   
  void ScrollTabClientPoint( CPoint& pt) const;
  //   
  // Description:
  //   Adjust rectangle, in client window coordinates, to reflect current scroll bar position.
  //   Does nothing if scroll bar is not currently visible.
  //
  // Parameters:
  //   r [in/out] Rectangle to adjust.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ScrollTabClientPoint()
  //   
  void ScrollTabClientRect( CRect& r) const;
  //   
  // Description:
  //   Get client rect positioned relative to current scroll bar position.
  //
  // Parameters:
  //   lpR [out] Pointer to receive rectangle
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ScrollTabClientRect()
  //   
  void GetScrolledTabClientRect( LPRECT lpR) const;
  //   
  // Description:
  //   Check to see if index is greater than or equal to zero and less than item count.  This
  //   validates a zero based index into tab control.
  //
  // Parameters:
  //   iIndex [in] Item index to validate
  //
  // Returns:
  //   Returns true if iIndex is valid; otherwise false
  //
  bool IndexIsValid( int iIndex) const;
  //
  // Description:
  //   Get borders surrounding the pages.
  //
  // Returns:
  //   Returns CSize where CSize::cx is the left and right borders and
  //   CSize::cy is the top and bottom.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::SetBorders
  //
  CSize Borders() const;
  //
  // Description:
  //   Get borders surrounding the pages.
  //
  // Parameters:
  //   cx [in] border to be applied to the left and right sides
  //   cy [in] border to be applied to the top and bottom
  //
  // Returns:
  //   Returns true if cx and cy are greater to or equal to 0
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::Borders
  //
  bool SetBorders( int cx, int cy);
  //
  // Description:
  //   Get CRhinoUiThemeDraw object pointer if theme drawing is supported
  //   for this control.
  //
  // Returns:
  //   Returns valid pointer to a initialized CRhinoUiThemeDraw object if
  //   theme drawing is supported for this object; otherwise return NULL.
  //
  // See Also:
  //   CRhinoUiThemeDraw
  //
  CRhinoUiThemeDraw* ThemeDraw() const;
public:
  //   
  // Description:
  //   See description for CRhinoUiExpandableTabCtrl::notification_ids
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::notification_ids
  //   
  static const UINT m_custom_win_msg_id;
protected:
  //   
  // Description:
  //   Calculate tab caption rectangles used for display and hit testing
  //
  // Returns:
  //   Returns size required necessary to display all tab items.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::RecalcLayout()
  //   
  CSize CalculateButtonRects();
  //   
  // Description:
  //   This is called when tabs are added, removed, expanded, hidden or shown.  This method
  //   calls CalculateButtonRects and decides if scroll bars need to be displayed.
  //
  // Parameters:
  //   bRepaint [in] If true then the client area of the tab control is invalidated.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::CalculateButtonRects()
  //   
  void RecalcLayout( bool bRepaint = true);
  //   
  // Description:
  //   Send event notification to parent window.  See CRhinoUiExpandableTabCtrl::notification_ids
  //   for valid event notification codes.
  //
  // Parameters:
  //   msg [in] CRhinoUiExpandableTabCtrl::notification_ids
  //   hiword [in] Hi word of WPARAM sent to parent, typically a state value
  //   lParam [in] Sent as LPARAM of custom notification message, typically a pointer to the item
  //               sending the notification.
  // See Also:
  //   CRhinoUiExpandableTabCtrl::notification_ids
  //   
  void SendParentNotification( int msg, int hiword, LPARAM lParam);

private:
  int TrackButton( CPoint point);
  bool SetExpandTabMouseCapture();
  bool ReleaseExpandTabMouseCapture();
  void InitThemeControl();
  bool StartDragDrop( int item_index, CPoint point);
  bool EndDragDrop( int lmouse_down_on, int mouse_over_index, CPoint point);
  // If to < 0 then move to end of list
  bool MoveItem( int from, int to, CRhinoUiExpandableTabCtrl* to_ctrl = NULL);
  int ItemFromPoint( CPoint pt, bool if_on_caption = true, bool if_on_page = true, bool if_on_stretch = true);
  void DestroyDragImageWindow();
  bool ShowDragButtonWindow( int item_index, bool bShow, CPoint point);
protected:
  bool TearOffAndDockBar( int lmouse_down_on, CPoint point, CPoint screen_point, CWnd* mouse_over_window);
private:
  CSize m_borders;
  CRhinoUiThemeDraw* m_pThemeDraw;
  int m_cyItemTitle;

  int m_mouse_over_index;
  int m_lmouse_down_on;
  CPoint m_lmouse_down_pt;
  int m_tab_gap;
  int m_active_page;
  bool m_bExpandTabMouseCaptured;
  bool m_bInDragDrop;
  CRhinoUiDragBitmapWnd* m_drag_window;
  static CRhinoDib m_drag_dib;
  UINT m_htmlID;
  SCROLLINFO m_scroll_info_v;
  SCROLLINFO m_scroll_info_h;
  ON_ClassArray<CRhinoUiExpandableTabCtrlItem>m_item_list;
  BYTE m_tab_flags[etf_invalid];

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSysColorChange();
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiExpandableTabCtrlItem_New
//
// Description:
//    This is a drop-in replacement for CRhinoUiExpandableTabCtrlItem when using CRhinoUiExpandableTabCtrl_New.
//
class CRhinoUiExpandableTabCtrl_New;

class RHINO_SDK_CLASS CRhinoUiExpandableTabCtrlItem_New
{
public:
  virtual ~CRhinoUiExpandableTabCtrlItem_New();

  CDialog* GetDialogPage(void) const;
  bool ItemState(CRhinoUiExpandableTabCtrlItem::item_states flag) const;

private:
  int m_iIndex;
  CRhinoUiExpandableTabCtrl_New& m_Owner;
  void* m_pExtension;
 
private:
  CRhinoUiExpandableTabCtrlItem_New(int index, CRhinoUiExpandableTabCtrl_New& c);
  friend class CRhinoUiExpandableTabCtrl_New;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiExpandableTabCtrl_New
//
// Description:
//    This is a drop-in replacement for CRhinoUiExpandableTabCtrl which uses
//    the RDK's holder containing one or more collapsible sections.
//
class RHINO_SDK_CLASS CRhinoUiExpandableTabCtrl_New
{
public:
  CRhinoUiExpandableTabCtrl_New();
  virtual ~CRhinoUiExpandableTabCtrl_New();

  bool ReplaceDlgItem(CDialog* pParentDlg, UINT nID);
  bool SetBorders(int cx, int cy);
  bool ExpandItem(int nItem, bool bExpand = true);
  int GetItemCount(void) const;
  bool HideTab(int iIndex, bool bHide = true);
  const CRhinoUiExpandableTabCtrlItem_New* GetItem(int nItem) const;
  const CRhinoUiExpandableTabCtrlItem_New* GetActiveItem(void) const;
  bool AddPage(LPCTSTR lpszItem, int nImage, UINT nIDTemplate, UINT nHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  bool CreateExpandableTabCtrl(CDialog* pParentDlg, UINT nID, LPRECT lpRect);
  int GetScrollPos(int nBar) const;
  int SetScrollPos(int nBar, int nPos, BOOL32 bRedraw = TRUE);
  bool IsCreated(void) const;
  void Move(const CRect& rect);
  int Width(void) const;

protected:
  CDialog* GetDialogPage(int index) const;
  bool ItemState(int index, CRhinoUiExpandableTabCtrlItem::item_states flag) const;
  friend class CRhinoUiExpandableTabCtrlItem_New;

private:
  CSize m_szBorder;
  int m_iWidth;
  class IRhExpandablePageUI* m_pPageUI;
  ON_SimpleArray<CRhinoUiExpandableTabCtrlItem_New*> m_aItems;
  void* m_pExtension;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoUiExpandableTabPage_New
//
// Description:
//    This is a drop-in replacement for CRhinoUiExpandableTabPage which uses
//    an RDK collapsible section.
//
class RHINO_SDK_CLASS CRhinoUiExpandableTabPage_New : public CRhinoUiDialog
{
  DECLARE_DYNAMIC(CRhinoUiExpandableTabPage_New)
public:
  CRhinoUiExpandableTabPage_New(UINT nIDTemplate, CWnd* pParent);
  virtual ~CRhinoUiExpandableTabPage_New();

  void SetParentForOnCreate(CWnd* p);
  bool SetUseSpecialColors(bool b);

protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg LRESULT OnIsWindowSection(WPARAM, LPARAM);
  DECLARE_MESSAGE_MAP()

private:
  CWnd* m_pParentForOnCreate;
  void* m_pExtension;
};
