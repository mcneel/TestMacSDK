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

// CRhinoUiOptionsListCtrl

class CRhinoUiOptionsListCtrlItem;

typedef struct tagRhinoUiOptionsListCtrlNM
{
  NMHDR m_NMHDR;
  CRhinoUiOptionsListCtrlItem* m_item;
} RhinoUiOptionsListCtrlNM, *LPRhinoUiOptionsListCtrlNM;

class RHINO_SDK_CLASS CRhinoUiOptionsListCtrl : public CWnd
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrl)
public:
	CRhinoUiOptionsListCtrl();
	virtual ~CRhinoUiOptionsListCtrl();

  // Description:
  //   Create a options list control.
  // Parameters:
  //   pParent [in] The parent window. 
  //   dwStyle [in] Specifies the window style attributes.
  //   r [in] The size and position of the window, in client coordinates of pParentWnd. 
  //   nID [in] The ID of the child window. 
  // Returns:
  //   Returns true if control is created or false if it was not.
  // See Also:
  //   CWnd::Create
  bool CreateOptionsList( CWnd* pParent, DWORD dwStyle, const CRect& r, UINT nID = 0);

  // Description:
  //   Replace a dialog item with this control 
  // Parameters:
  //   pParent [in] Dialog to embed tab control inside of.
  //   nID [in] The ID of the dialog control to replace.  The new
  //            tab control will be created using this ID.
  // Returns:
  //   Returns true if successful; otherwise false
  // See Also:
  //   CRhinoUiOptionsListCtrl::CreateOptionsList()
  bool ReplaceDlgItem( CDialog* pParent, UINT nID);

  // Description:
  //   Resize the window so there is no extra space at the bottom
  //   and ensure that no items are partially displayed.
  // Parameters:
  //   bGrow [in] Allow window to grow to fit the bottom visible item.
  void SizeToContent( bool bGrow);

  // Description:
  //   Insert new item into options list control.  If the location is less than zero or greater than
  //   or equal to list item count the item is added to the end of the list.
  // Parameters:
  //   iItemIndex [in] Index to insert item into.  If the less than zero or greater than or equal to
  //                   list item count the item is added to the end of the list.
  //   pItem [in] Item to insert into list.  If CRhinoUiOptionsListCtrlItem::AutoDelete() is true
  //              then the control will delete the pointer when the list or item is destroyed.  If
  //              pItem points to a stack variable it must remain in scope for the life of the list
  //              and you must call CRhinoUiOptionsListCtrlItem::SetAutoDelete( false) to keep the
  //              item from self deleting.
  // Returns:
  //   Returns true if item is successfully inserted into options list control or false if was not.
  // See Also:
  //   CRhinoUiOptionsListCtrl::AddItem()
  //   CRhinoUiOptionsListCtrlItem::AutoDelete()
  //   CRhinoUiOptionsListCtrlItem::SetAutoDelete()
  bool InsertItem( int iItemIndex, CRhinoUiOptionsListCtrlItem* pItem);

  // Description:
  //   Add a new item to the end of the options item list.
  // Parameters:
  //   pItem [in] Item to add list.  If CRhinoUiOptionsListCtrlItem::AutoDelete() is true
  //              then the control will delete the pointer when the list or item is destroyed.  If
  //              pItem points to a stack variable it must remain in scope for the life of the list
  //              and you must call CRhinoUiOptionsListCtrlItem::SetAutoDelete( false) to keep the
  //              item from self deleting.
  // Returns:
  //   Returns number greater than or equal to zero which represent the index of the item or -1 on
  //   error.
  // See Also:
  //   CRhinoUiOptionsListCtrl::InsertItem()
  //   CRhinoUiOptionsListCtrlItem::AutoDelete()
  //   CRhinoUiOptionsListCtrlItem::SetAutoDelete()
  int AddItem( CRhinoUiOptionsListCtrlItem* pItem);
  //   
  // Description:
  //   Call this method to control wether or not an item is displayed in the list control.
  //
  // Parameters:
  //   pItem [in] Item to display or hide
  //   bHide [in] If bHide is true then the item will be removed from the list display but not
  //              from the list, the item will still have the same item index.
  //
  // Returns:
  //   Returns true if the items hide state was successfully changed or false if not.
  //
  bool HideItem( CRhinoUiOptionsListCtrlItem* pItem, bool bHide = true);
  //   
  // Description:
  //   Call this method to control wether or not an item is displayed in the list control.
  //
  // Parameters:
  //   iItem [in] Index of item to display or hide.
  //   bHide [in] If bHide is true then the item will be removed from the list display but not
  //              from the list, the item will still have the same item index.
  //
  // Returns:
  //   Returns true if the items hide state was successfully changed or false if not.
  //
  bool HideItem( int iItem, bool bHide = true);
  //   
  // Description:
  //   Size of borders surrounding the list items.  The client rectangle is shrunk by these amounts
  //   prior to displaying option items.
  //
  // Parameters:
  //   iLeft   [in] Space on left side of list items
  //   iTop    [in] Space above first item in list
  //   iRight  [in] Space on right side of list items
  //   iBottom [in] Space below last visible item in list
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::Borders()
  //   
  void SetBorders( int iLeft, int iTop, int iRight, int iBottom);
  //   
  // Description:
  //   Size of borders surrounding the list items.  The client rectangle is shrunk by these amounts
  //   prior to displaying option items.
  //
  // Parameters:
  //   r [in] Size of borders surrounding the list items.
  //     r.left   = Space on left side of list items
  //     r.top    = Space above first item in list
  //     r.right  = Space on right side of list items
  //     r.bottom = Space below last visible item in list
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::Borders()
  //   
  void SetBorders( const CRect& r);
  //   
  // Description:
  //   Size of borders surrounding the list items.  The client rectangle is shrunk by these amounts
  //   prior to displaying option items.
  //
  // Returns:
  //   Size of borders surrounding the list items.
  //     CRect.left   = Space on left side of list items
  //     CRect.top    = Space above first item in list
  //     CRect.right  = Space on right side of list items
  //     CRect.bottom = Space below last visible item in list
  //   
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetBorders()
  //   
  CRect Borders() const;
  //   
  // Description:
  //   Current item label background color.
  //
  // Returns:
  //   Returns color used to fill item label background for unselected items.  If 
  //   SetLabelBkGrndColor( CRhinoUiPaintManager::unset_color) is called then the current
  //   dialog background color will be used.  To change call SetLabelBkGrndColor( RGB( r, g, b))
  //   with new background color.  By default the label background color is
  //   CRhinoUiPaintManager::unset_color which causes the dialog background color to be returned.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //
  COLORREF LabelBkGrndColor() const;
  //   
  // Description:
  //   Current item label text color color.
  //
  // Returns:
  //   Returns color used to for label text in unselected items.  If 
  //   SetLabelBkGrndColor( CRhinoUiPaintManager::unset_color) is called then the current
  //   dialog text color will be used.  To change call SetLabelBkGrndColor( RGB( r, g, b))
  //   with new background color.  By default the label background color is
  //   CRhinoUiPaintManager::unset_color which causes the dialog background text color to
  //   be returned.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   CRhinoUiOptionsListCtrl::LabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //
  COLORREF LabelTextColor() const;
  //   
  // Description:
  //   Current separator item label background color.
  //
  // Returns:
  //   Returns color used to fill item label background for unselected items.  If 
  //   SetLabelBkGrndColor( CRhinoUiPaintManager::unset_color) is called then the current
  //   dialog background color will be used.  To change call SetLabelBkGrndColor( RGB( r, g, b))
  //   with new background color.  By default the label background color is
  //   CRhinoUiPaintManager::unset_color which causes the dialog background color to be returned.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //
  COLORREF SeparatorLabelBkGrndColor() const;
  //   
  // Description:
  //   Current separator item label text color color.
  //
  // Returns:
  //   Returns color used to for label text in unselected items.  If 
  //   SetLabelBkGrndColor( CRhinoUiPaintManager::unset_color) is called then the current
  //   dialog text color will be used.  To change call SetLabelBkGrndColor( RGB( r, g, b))
  //   with new background color.  By default the label background color is
  //   CRhinoUiPaintManager::unset_color which causes the dialog background text color to
  //   be returned.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   CRhinoUiOptionsListCtrl::LabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //
  COLORREF SeparatorLabelTextColor() const;
  //   
  // Description:
  //   Current color used to for drawing horizontal and vertical grid lines separating
  //   list items.
  //
  // Returns:
  //   Returns color used to for drawing horizontal and vertical grid lines separating
  //   list items. If  SetLabelBkGrndColor( CRhinoUiPaintManager::unset_color) is called 
  //   then the default color is used.  To change call SetGridLineColor( RGB( r, g, b))
  //   with new grid line color.  By default the grid line color is
  //   CRhinoUiPaintManager::unset_color which causes the default grind line color to be
  //   used.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetGridLineColor()
  //
  COLORREF GridLineColor() const;
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
  COLORREF SetSeparatorLabelBkGrndColor( COLORREF cr);
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
  COLORREF SetSeparatorLabelTextColor( COLORREF cr);
  //   
  // Description:
  //    Used to override default grid line color.  Default value is CRhinoUiPaintManager::unset_color
  //    which causes the default color to be used.  Set to valid RGB value to change label text color.
  //
  // Parameters:
  //   cr [in] New grid line color CRhinoUiPaintManager::unset_color for default color.
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GridLineColor()
  //   CRhinoUiOptionsListCtrl::LabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBkGrndColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //
  COLORREF SetGridLineColor( COLORREF cr);
  //   
  // Description:
  //   Get current width for label column.
  //
  // Returns:
  //   Current width of label column in client window units.
  //
  int LabelColumnWidth() const;
  //   
  // Description:
  //   Get options list item index from client window point.
  //
  // Parameters:
  //   point [in] Point to test in client window coordinates.
  //
  // Returns:
  //   Returns zero or greater if point is over item or -1 if not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetItem()
  //   
  int ItemIndex( CPoint point) const;
  //   
  // Description:
  //   Get options list item index from list item object.
  //
  // Parameters:
  //   item [in] Search list for this item
  //
  // Returns:
  //   Returns zero or greater if item is found or -1 if not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetItem()
  //   
  int ItemIndex( const CRhinoUiOptionsListCtrlItem& item) const;
  enum
  {
    min_label_width = 5,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_options_list_ctrl = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Valid hit test return codes
  //
  enum hit_test
  {
    ht_item_label = 100, // An item label in the left column
    ht_item_expand,      // Expand/contract "+/-" image to left of label text     
    ht_item_value,       // An item value in the right column
    ht_label_size,       // The size bar which vertically separates the left and right columns
    ht_invalid,          // Did not land on an item or separator
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_hit_test = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Test point to see if it is over a item or size control.
  //
  // Parameters:
  //   point [in] Point is client window coordinates to test
  //   pItem [out] If not NULL it will receive the item index if the point is over a valid item.
  //
  // Returns:
  //   Returns CRhinoUiOptionsListCtrl::hit_test value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::hit_test
  //   
  int HitTest( CPoint point, int* pItem = NULL) const;
  //   
  // Description:
  //   Codes which represent the reason for the current mouse capture.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsTracking()
  //   CRhinoUiOptionsListCtrl::TrackingMode()
  //   
  enum tracking_mode
  {
    track_label_width = 0, // Mouse captured to track label column resize event
    track_select_item,     // Mouse captured on a mouse down to track a select event
    not_tracking,          // Mouse is not captured or not being tracked
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_tracking_mode = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Used to determine if mouse tracking is active.
  //
  // Returns:
  //   Returns true if the mouse is captured and being tracked; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::TrackingMode()
  //   
  bool IsTracking() const;
  //   
  // Description:
  //   Returns code which describes why the mouse is captured or not_tracking it not captured.
  //
  // Returns:
  //   Returns CRhinoUiOptionsListCtrl::tracking_mode value indicating what is being tracked or
  //   CRhinoUiOptionsListCtrl::not_tracking if not tracking.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsTracking()
  //   
  int TrackingMode() const;
  //   
  // Description:
  //   Default label item height.
  //
  // Returns:
  //   Returns default label item height.
  //
  int LabelItemHeight() const;
  //   
  // Description:
  //   This method retrieves the index of the topmost visible item when in options list.
  //
  // Returns:
  //   Returns the index of the topmost visible item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetCountPerPage()
  //   
  int GetTopIndex() const;
  //   
  // Description:
  //   This method retrieves the zero-based index of the currently selected item, if any, in
  //   options list box.
  //
  // Returns:
  //   The zero-based index of the currently selected item. It is less than zero if no item is
  //   currently selected.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetCurSel()
  //   
  int GetCurSel() const;
  //   
  // Description:
  //   This method selects a item and scrolls it into view, if necessary. When the new item is selected,
  //   the list box removes the highlight from the previously selected item.
  //
  // Parameters:
  //   Specifies the zero-based index of the item to be selected. If nSelect is –1, the options list box
  //   is set to have no selection.
  //
  // Returns:
  //   Previously selected item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetCurSel()
  //   
	int SetCurSel(int iItem);
  //   
  // Description:
  //   Used by GetItemRect() to identify which part of an item rectangle to get.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetItemRect()
  //   
  enum item_rect_part
  {
    irp_bounds = 0,
    irp_label,
    irp_value,
    irp_invalid,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_item_rect_part = 0xFFFFFFFF
  };
  //   
  // Description:
  //   This method retrieves the bounding rectangle for all or part of an item in the control.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index of the item whose position is to be retrieved. 
  //   lpRect [out] Address of a RECT structure that receives the bounding rectangle.
  //   nCode [in] Specifies the portion of the item for which to retrieve the bounding rectangle.
  //              It can be one of these values: 
  //                  irp_bounds  Returns the bounding rectangle of the entire item, including the
  //                              label and value. 
  //                  irp_label   Returns the bounding rectangle of the item label column
  //                  irp_value   Returns the bounding rectangle of the item value column
  //
  // Returns:
  //   Nonzero if it is successful; otherwise, it is zero.
  //
  bool GetItemRect(int iItem, LPRECT lpRect, item_rect_part nCode) const;
  //   
  // Description:
  //   This method retrieves the number of items in a options list control.
  //
  // Returns:
  //   The number of items in the options list control.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetItem()
  //   
  int GetItemCount() const;
  //   
  // Description:
  //   This method retrieves options item attributes in a options list control.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index of the item whose attributes
  //              are to be retrieved.
  //
  // Returns:
  //   The options item attributes associate with the item; otherwise NULL.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetItemCount()
  //   
  CRhinoUiOptionsListCtrlItem* GetItem( int iItem) const;
  //   
  // Description:
  //   This method forces a list view control to repaint a specific.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index of the item to redraw.
  //
  void RedrawItem( int iItem) const;
  //   
  // Description:
  //   This method forces a list view control to repaint a specific.
  //
  // Parameters:
  //   item [in] Will look up this item and redraw if found.
  //
  void RedrawItem( const CRhinoUiOptionsListCtrlItem& item) const;
  //   
  // Description:
  //   This method calculates the number of items that can fit vertically in the visible
  //   area of a options list control.
  //
  // Parameters:
  //   bPartialOK [in] Specifies whether partially  visible items are counted. 
  //
  // Returns:
  //   The number of items that can fit vertically in the visible area of a options list
  //   control.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::GetTopIndex()
  //   CRhinoUiOptionsListCtrl::GetItemCount()
  //
  int GetCountPerPage( bool bPartialOK = false) const;
  //   
  // Description:
  //   This method ensures that a options list item is at least partially visible. The options
  //   list control is scrolled if necessary. If the bPartialOK parameter is true, no scrolling
  //   occurs if the item is partially visible.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index of the options list item that is to be visible. 
  //   bPartialOK [in] Specifies whether partial visibility is acceptable.
  //
  // Returns:
  //   Returns true if it is successful; otherwise, it is false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsItemVisible()
  //   
  bool EnsureVisible( int iItem, bool bPartialOK);
  //   
  // Description:
  //   This method will check to see if a options list item is at is at least partially visible.
  //   If the bPartialOK parameter is true then partially visible items return false.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index of the options list item that to check. 
  //   bPartialOK [in] Specifies whether partially  visible items okay. 
  //
  // Returns:
  //   Returns true if item is visible; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::EnsureVisible()
  //   
  bool IsItemVisible( int iItem, bool bPartialOK) const;
  //   
  // Description:
  //   This method will validate a zero-base index into the options list control.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index to validate.
  //
  // Returns:
  //   Returns true if index is valid; otherwise false.
  //
  // See Also:
  //   
  //   
  bool ItemIndexIsValid( int iItem) const;
  //   
  // Description:
  //   This method returns the rectangle for the options list control client area.  This rectangle
  //   is the client rectangle defeated by the border offsets.
  //
  // Returns:
  //   Returns rectangle used to display options list control items.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetBorders()
  //   CRhinoUiOptionsListCtrl::Borders()
  //   
  CRect GetListRect() const;
  //   
  // Description:
  //   This method is called to create edit control window when a item value is clicked on.
  //
  // Parameters:
  //   iItem [in] Specifies the zero-based index to validate.
  //   eir [in] What caused this item to be edited
  //
  // Returns:
  //   Return true if the item control window has been created or false if it has not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   CRhinoUiOptionsListCtrl::OnItemButtonClicked()
  //   CRhinoUiOptionsListCtrlItem::begin_edit_item_reason
  //
  bool BeginEditItem( int iItem, CRhinoUiOptionsListCtrlItem::begin_edit_item_reason eir);
  //   
  // Description:
  //   This method is called when a item control window has been created and is about to
  //   be displayed.  The default behavior is to send a registered windows message to the
  //   parent of the list control to notify it of this event.
  //
  // Parameters:
  //   item [in] Specifies the item control window has just been created so the value may
  //   be modified.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual void OnBeginEditItem( CRhinoUiOptionsListCtrlItem& item);
  //   
  // Description:
  //   This method is called when a item value has been changed.  The default behavior is to
  //   send a registered windows message to the parent of the list control to notify it of
  //   this event.
  //
  // Parameters:
  //   item [in] Specifies the item whos value has been modified.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  virtual void OnEndEditItem( CRhinoUiOptionsListCtrlItem& item);
  //   
  // Description:
  //   This method is called as a result of an EN_CHANGE notification.  The default behavior is
  //   to send a registered windows message to the parent of the list control to notify it of
  //   this event.
  //
  // Parameters:
  //   item [in] Specifies the item whos value has been modified.
  //
  virtual void OnEnChangeItem( CRhinoUiOptionsListCtrlItem& item);
  //   
  // Description:
  //   This method is called when push button has been clicked on.  The default  behavior is
  //   to send a registered windows message to the parent of the list control to notify it
  //   of this event.
  //
  // Parameters:
  //   item [in] Specifies the item which was clicked on.
  //
  // Returns:
  //   Returns true if item value should be updated or false if it should not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::OnItemButtonClicked()
  //   CRhinoUiOptionsListCtrl::OnItemComboBoxSelChanged()
  //   
  virtual bool OnItemButtonClicked( CRhinoUiOptionsListCtrlPushButton& item);
  //   
  // Description:
  //   This method is called when check box has been clicked on.  The default  behavior is
  //   to send a registered windows message to the parent of the list control to notify it
  //   of this event.
  //
  // Parameters:
  //   item [in] Specifies the item which was clicked on.
  //
  // Returns:
  //   Returns true if item value should be updated or false if it should not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual bool OnItemButtonClicked( CRhinoUiOptionsListCtrlCheckBox& item);
  //   
  // Description:
  //   This method is called when control want to simulate clicking on a button.  The message
  //   is used by the file control to notify the list control the file button was pressed.
  //   The default  behavior is to send a registered windows message to the parent of the list
  //   control to notify it of this event.
  //
  // Parameters:
  //   item [in] Specifies the item which was clicked on.
  //
  // Returns:
  //   Returns true if item value should be updated or false if it should not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual bool OnItemButtonClicked( CRhinoUiOptionsListCtrlItem& item);
  //   
  // Description:
  //   This method is called by CRhinoUiOptionsListCtrlComboBox and derived controls after
  //   the combo box has been created but before it has been displayed.  The method should
  //   be used to add items to the combo box.  If not items are added and/or this method
  //   returns true then the combo box is destroyed and no further action is taken in response
  //   to BeginEditItem().
  //
  //   The default behavior is to send a registered windows message to the parent of the list
  //   control to notify it of this event.
  //
  // Parameters:
  //   item [in] Specifies comobo box to add items to.
  //
  // Returns:
  //   Return true to force comobo box to display, false to abort edit operation.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::OnItemComboBoxSelChanged()
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual bool PopulateItemComboBox( CRhinoUiOptionsListCtrlComboBox& item);
  //   
  // Description:
  //   This method is called upon a successful call to PopulateItemComboBox() to notify the
  //   list control that it has been populated and provide it with and opportunity set set
  //   the current selection.
  //
  // Parameters:
  //   item [in] Specifies comobo box to set current selection.
  //
  // Returns:
  //   Return true if edit action should continue; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::PopulateItemComboBox()
  //   CRhinoUiOptionsListCtrl::OnItemComboBoxSelChanged()
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual bool OnComboBoxPopulated( CRhinoUiOptionsListCtrlComboBox& item);
  //   
  // Description:
  //   This method is called when when the user changes the current selection in the list box
  //   of a combo box.  The default behavior is to send a registered windows message to the 
  //   parent of the list control to notify it of this event. 
  //
  // Parameters:
  //   item [in] Specifies comobo box which selection has changed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::PopulateItemComboBox()
  //   CRhinoUiOptionsListCtrl::OnComboBoxPopulated()
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual void OnItemComboBoxSelChanged( CRhinoUiOptionsListCtrlComboBox& item);
  //   
  // Description:
  //   This method is called when when the user clicks a radio button in a radio button column
  //   list control item.  The default behavior is to send a registered windows message to the 
  //   parent of the list control to notify it of this event. 
  //
  // Parameters:
  //   item [in] Specifies radio button column which selection has changed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::OnEndEditItem()
  //   
  virtual void OnItemRadioButtonChecked( CRhinoUiOptionsListCtrlRadioButtonColumn& item);
  //   
  // Description:
  //   See description for CRhinoUiOptionsListCtrl::notification_codes
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   CRhinoUiOptionsListCtrl::SendParentNotification()
  //   
  static const UINT m_custom_win_msg_id;
  //
  // Description:
  //   Custom event notification messages sent to parent use the following format:
  //     message = CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //               (static UINT holding ID of registered Windows message)
  //     WPARAM = Pointer to a RhinoUiOptionsListCtrlNM structure that provides information
  //              about the item sending the message including the 
  //              CRhinoUiOptionsListCtrl::notification_codes that caused the message to be
  //              sent.
  //                typedef struct tagRhinoUiOptionsListCtrlNM
  //                {
  //                  NMHDR m_NMHDR;
  //                            |- hwndFrom = This window handle
  //                            |- idFrom   = Zero-based index of the item causing this message to be sent;
  //                            |- code     = CRhinoUiOptionsListCtrl::notification_codes
  //                  CRhinoUiOptionsListCtrlItem* m_item = Pointer to item causing this message to be sent.
  //                                                        Use the m_NMHDR.code to determine valid cast options
  //                                                        for this pointer.  For example, if the
  //                                                        code == nc_on_item_button_clicked then you can cast
  //                                                        m_item as follows:
  //                                                           static_cast<CRhinoUiOptionsListCtrlPushButton*>( m_item)
  //                                                        See notification_codes for valid options.
  //                } RhinoUiOptionsListCtrlNM, *LPRhinoUiOptionsListCtrlNM;
  //     LPARAM = This is a pointer to a LRESULT.  This is how you respond to the notification_codes.  Setting this value
  //              to true generally means you have handled the message so default processing can stop.  See notification_codes
  //              for description of this parameter.
  //
  //   To use these notification codes do add the message map macro to the parent of this control:
  //     ON_REGISTERED_MESSAGE( CRhinoUiOptionsListCtrl::m_custom_win_msg_id, OnRegisteredMessage)
  //   Add this to the decloration of the the parent window class:
  //     afx_msg LRESULT OnRegisteredMessage( WPARAM wParam, LPARAM lParam);
  //   Add this method to the class implementation
  //     LRESULT CTestOptionsListDlg::OnRegisteredMessage( WPARAM wParam, LPARAM lParam)
  //     {
  //       LPRhinoUiOptionsListCtrlNM pNM = (LPRhinoUiOptionsListCtrlNM)wParam;
  //       LRESULT* pResult = (LRESULT*)lParam;
  //       *pResult = false;
  //       CRhinoUiOptionsListCtrlItem* pItem = pNM->m_item;
  //       if( NULL == pItem)
  //         return 0L;
  //       int iItemIndex = pNM->m_NMHDR.idFrom;
  //       UINT nNotificationCode = pNM->m_NMHDR.code;
  //       switch( nNotificationCode)
  //       {
  //         case CRhinoUiOptionsListCtrl::nc_on_item_button_clicked:
  //            ...
  //            break;
  //       }
  //     }
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::SendParentNotification()
  //   
  enum notification_codes
  {
    nc_on_item_button_clicked = 0, // RhinoUiOptionsListCtrlNM::m_item points to a CRhinoUiOptionsListCtrlPushButton* item
                                   // LRESULT is not used
    nc_on_item_check_box_clicked,  // RhinoUiOptionsListCtrlNM::m_item points to a CRhinoUiOptionsListCtrlCheckBox* item
                                   // LRESULT is not used
    nc_populate_combo_box,         // RhinoUiOptionsListCtrlNM::m_item points to a CRhinoUiOptionsListCtrlComboBox* item
                                   // LRESULT Combo box will only be displayed if this is equal to true.
    nc_combo_box_populated,        // RhinoUiOptionsListCtrlNM::m_item points to a CRhinoUiOptionsListCtrlComboBox* item
                                   // LRESULT Combo box will only be displayed if this is equal to true
    nc_on_combo_box_sel_changed,   // RhinoUiOptionsListCtrlNM::m_item points to a CRhinoUiOptionsListCtrlComboBox* item
                                   // LRESULT is not used
    nc_on_radio_button_checked,    // RhinoUiOptionsListCtrlNM::m_item points to a CRhinoUiOptionsListCtrlRadioButtonColumn* item
                                   // LRESULT is not used
    nc_on_en_change_item ,         // Will point to an edit box or list box edit control.  This will be sent as the result
                                   // of a EN_CHANGE notification.  This will also be sent by numeric versions of the control
                                   // when a scroll button is clicked.  When being sent as a result of a scroll button the LRESULT
                                   // will be 1L otherwise it will be 0L.
    nc_on_begin_edit_item,         // Can point to any valid derived type.  This will be sent right after the control window
                                   // has been created.
    nc_on_end_edit_item,           // Can point to any valid derived type.  You will have to check the m_item->ItemID() to decide.
                                   // Will be sent by a CRhinoUiOptionsListCtrlEditBox with a numeric style when a scroll button
                                   // is released signifying the end of a scroll event, if this is the case LRESULT will be 1L.
    nc_expand_state_changed,       // Sent when an items expanded state changes.  LRESULT is ignored.
	  nc_on_separator_clicked,       // A separator control has been clicked

    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future

    force_32bit_notification_codes = 0xFFFFFFFF
  };
  enum options_list_ctrl_styles
  {
    mouse_over_column_sizing_style       =     1, // on by default, if this is set then lock_item_value_column_width and lock_item_label_column_width are ignored.
    lock_item_value_column_width         =     2, // off by default, only meaningful if lock_item_label_column_width is not set
    lock_item_label_column_width         =     4, // off by default, will override lock_item_value_column_width
    no_row_highlight                     =     8, // off by default, if this is set then rows will always paint the same, no highlight background on labels or vales.
    use_button_face_color_for_labels     =  0x10, // off by default, if this is set then item labels are painted using button face colors otherwise they use window colors.
    use_button_face_color_for_separators =  0x20, // on by default, if this is set then separators are painted using button face colors otherwise they use window colors.
    use_button_face_color_for_indent     =  0x40, // on by default, if this is set then indent spacing is painted using button face colors otherwise they use the item label color.
    draw_separator_lines                 =  0x80, // off by default, if this is set then separators will draw a horizontal line to the right of the separator text.
    dont_scale_fonts_when_localized      = 0x100, // off by default, if this is not set then the font used to draw this control will be scaled when localized if necessary
    rhino7_style                         = 0x200, // off by default. When this is set, the list control will draw based on styles defined for Rhino7
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_options_list_ctrl_styles = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Call this method to get options list control style flags.
  //
  // Returns:
  //   Returns bitwise integer which represents which styles are set.  See options_list_ctrl_styles for options.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::options_list_ctrl_styles
  //   
  int ListStyle() const;
  //   
  // Description:
  //   Call this method to change style flags.
  //
  // Parameters:
  //   styles [in] New styles.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::options_list_ctrl_styles
  //   CRhinoUiOptionsListCtrl::ListStyle()
  //
  void SetListStyle( int styles);
  //   
  // Description:
  //   Call this method modify specific style flags.
  //
  // Parameters:
  //   remove [in] Style flags to remove.
  //   add    [in] Style flags to add.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::options_list_ctrl_styles
  //   CRhinoUiOptionsListCtrl::ListStyle()
  //
  void ModifyListStyle( int remove, int add);
  //   
  // Description:
  //   Call this method to get fixed width of a column.  If the lock_item_label_column_width
  //   style is set then the width applies to the item label, if the lock_item_value_column_width
  //   value is set then it applies to the value column; otherwise it is ignored.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::options_list_ctrl_styles
  //   CRhinoUiOptionsListCtrl::ListStyle()
  //   CRhinoUiOptionsListCtrl::SetListStyle()
  //   CRhinoUiOptionsListCtrl::SetFixedColumnWidth()
  //
  int FixedColumnWidth() const;
  //   
  // Description:
  //   Call this method to set the fixed width of a column.  If the lock_item_label_column_width
  //   style is set then the width applies to the item label, if the lock_item_value_column_width
  //   value is set then it applies to the value column; otherwise it is ignored.
  //
  // Parameters:
  //   iWidth [in] Specifies width in pixels.
  //
  // Returns:
  //   Returns previous width
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::options_list_ctrl_styles
  //   CRhinoUiOptionsListCtrl::ListStyle()
  //   CRhinoUiOptionsListCtrl::SetListStyle()
  //   CRhinoUiOptionsListCtrl::FixedColumnWidth()
  //
  int SetFixedColumnWidth( int iWidth);
  //   
  // Description:
  //   Call this method to determine if items indent level will be used.
  //
  // Parameters:
  //   iWidth [in] Specifies width in pixels.
  //
  // Returns:
  //   If this returns true then CRhinoUiOptionsListCtrlItem::IndentLevel() is used
  //   when drawing item labels; otherwise it is ignored.  The default value is off.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetIndentItems()
  //   CRhinoUiOptionsListCtrlItem::IndentLevel()
  //
  bool IndentItems() const;
  //   
  // Description:
  //   Call this method to enable or disable item label indenting.
  //
  // Parameters:
  //   b [in] If true then CRhinoUiOptionsListCtrlItem::IndentLevel() is used
  //          when drwing item labels.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetIndentItems()
  //   CRhinoUiOptionsListCtrlItem::IndentLevel()
  //
  bool SetIndentItems( bool b);
  //   
  // Description:
  //   Call this method to determine the number of spaces used to indent an item.
  //   label when item indenting is enabled.  The default value is 3 spaces.
  //
  // Returns:
  //   Returns the number of spaces which will be multiplied by the item spacing when
  //   indenting item labels.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetIndentSpacing()
  //   CRhinoUiOptionsListCtrl::IndentItems()
  //   CRhinoUiOptionsListCtrl::SetIndentItems()
  //   CRhinoUiOptionsListCtrlItem::IndentLevel()
  //
  int IndentSpacing() const;
  //   
  // Description:
  //   Call this method to set the number of spaces which will be used for an indent level.
  //
  // Parameters:
  //   iNumberOfSpaces [in] Specifies the number of spaces which will be multiplied by the
  //                        item indent level when calculating item label indents.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IndentSpacing()
  //   CRhinoUiOptionsListCtrl::IndentItems()
  //   CRhinoUiOptionsListCtrl::SetIndentItems()
  //   CRhinoUiOptionsListCtrlItem::IndentLevel()
  //
  int SetIndentSpacing( int iNumberOfSpaces);
  //   
  // Description:
  //   Call this method to adjust list control contents and decide if the scroll bar should be
  //   displayed or not.  This is called by OnSize() and should be called when done showing or 
  //   hiding items.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::HideItem()
  //
  void RecalcLayout();

  bool ExpandItem( int iItem, bool bExpand = true);

  bool ExpandItem( CRhinoUiOptionsListCtrlItem* pItem, bool bExpand = true);

  // Description:
  //   Send m_custom_win_msg_id registered windows message to the parent of this options list control.
  //
  // Parameters:
  //   item [in] Item associated with this message
  //   nNotificationCode [in] CRhinoUiOptionsListCtrl::notification_codes, reason for sending this message
  //   lpResult [out] Response to message.  See CRhinoUiOptionsListCtrl::notification_codes for description.
  //   bPostMessage [in] Some messages and controls may be destroyed or in the process of being destroyed when
  //                     this message is being sent.  If this is the case set this to true so messages waiting for
  //                     control get processed prior to sending this message.
  // Returns:
  //   Returns the result of the call to ParentWindow->SendMessage() or ParentWindow->PostMessage()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   class CRhinoUiOptionsListCtrlItem
  //   
  LRESULT SendParentNotification( CRhinoUiOptionsListCtrlItem& item, UINT nNotificationCode, LRESULT* lpResult, bool bPostMessage = false);
  //   
  // Description:
  //   This method is called when a visual aspect of an visible options list item changes.
  //
  // Parameters:
  //   dc [in] Identifies the device context to be used when performing drawing operations on the options list control
  //   iItem [in] Identifies the zero-based list index of the item being drawn.
  //   rItem [in] A rectangle in the device context specified by the hDC member that defines the boundaries of the item to be drawn.
  //   item[in] Item being drawn.
  virtual void DrawItem( CDC& dc, int iItem, CRect rItem, const CRhinoUiOptionsListCtrlItem& item) const;
protected:
  void SetTrackingCursor( tracking_mode tm);

  // Description:
  //   Used by GetOptionsListCursor() to describe which cursor to return
  enum options_list_cursor
  {
    olc_label_width = 0,
    olc_invalid,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_list_cursor = 0xFFFFFFFF
  };

  HCURSOR GetOptionsListCursor( options_list_cursor cur);
  CRhinoUiOptionsListCtrlItem* GetItemX( int iItem) const;

private:
  // Description:
  //   Used to resize column width.  This method will capture the mouse and set
  //   flags necessary for OnMouseMove to track the label column resize event.
  // Parameters:
  //   point [in] Current mouse position to start tracking from.  This becomes
  //     the anchor for the resize event.  The new width is based on the
  //     difference of current mouse position X and the anchor point X.
  // See Also:
  //   CRhinoUiOptionsListCtrl::TrackLabelWidth()
  //   CRhinoUiOptionsListCtrl::EndTrackLabelWidth()
  void StartTrackLabelWidth( CPoint point);

  // Description:
  //   Called by OnMouseMove to dynamically adjust the label column width while
  //   the mouse is moving and the left button remains down.  This will resize
  //   the column assuring that is does not get too narrow.
  // Parameters:
  //   point [in] Current mouse position
  // See Also:
  //   CRhinoUiOptionsListCtrl::StartTrackLabelWidth()
  //   CRhinoUiOptionsListCtrl::EndTrackLabelWidth()
  void TrackLabelWidth( CPoint point);

  // Description:
  //   This method is called when the left mouse button is released during a label column resize event.  It will
  //   finish the label column adjustment and release the mouse capture.
  //
  // Parameters:
  //   point [in] Current mouse position
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::StartTrackLabelWidth()
  //   CRhinoUiOptionsListCtrl::TrackLabelWidth()
  //   
  void EndTrackLabelWidth( CPoint point);
  //   
  // Description:
  //   This method draws the tracking line while adjusting the label column width.
  //
  // Parameters:
  //   point [in] Current mouse position
  //   mode [in] This parameter identifies the tracking method calling this method.  If this method is called
  //             when starting tracking it is not necessary to remove the previous tracking line.  If called
  //             when the tracking event is complete it is only necessary to remove the drag line.  If called
  //             during drag the old line must be removed and the new one drawn.
  //
  // See Also:
  //   point [in] Current mouse position.  Used to calculate current drag line position.
  //   mode [in] Current tracking mode; start tracking, tracking, or done tracking.
  //   
  void DrawTrackLabelWidth( CPoint point, int mode);
  //   
  // Description:
  //   This method is called when the mouse goes down over an item to begin the select item process.
  //
  // Parameters:
  //   point [in] Current cursor position.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::TrackSelect()
  //   CRhinoUiOptionsListCtrl::EndTrackSelect()
  //   
  void StartTrackSelect( CPoint point);

  // Description:
  //   Called while the left mouse button is down while over an item.
  // Parameters:
  //   point [in] Current mouse position.
  // See Also:
  //   CRhinoUiOptionsListCtrl::StartTrackSelect()
  //   CRhinoUiOptionsListCtrl::EndTrackSelect()
  void TrackSelect( CPoint point);

  // Description:
  //   Called when the left mouse button is released.
  // Parameters:
  //   point [in] Mouse position when the left button comes up.
  // See Also:
  //   CRhinoUiOptionsListCtrl::StartTrackSelect()
  //   CRhinoUiOptionsListCtrl::TrackSelect()
  void EndTrackSelect( CPoint point);
  bool SetOptionsListMouseCapture();
  bool ReleaseOptionsListMouseCapture();

private:
  CRect m_margins;
  CRect m_rList;
  CRect m_rListPaint;
  CPoint m_ptStartTracking;
  CRect m_rLastTrack;
  double m_cxLabel;
  int m_nMyNcHitTest;
  tracking_mode m_track_mode;
  HCURSOR m_hCursorTracking;
  ON_SimpleArray<CRhinoUiOptionsListCtrlItem*>m_item_list;
  int m_iCurSel;
  int m_iTopIndex;
  unsigned int m_iNextItemID;
  SCROLLINFO m_scroll_info_v;
  bool m_bDisplayHideScroll;
  COLORREF m_crLabelBkGrnd;
  COLORREF m_crLabelText;
  COLORREF m_crSeparatorLabelBkGrnd;
  COLORREF m_crSeparatorLabelText;
  COLORREF m_crGridLine;
  RhinoUiOptionsListCtrlNM m_item_nm;
  bool m_bListCtrlMouseCaptured;
  unsigned char m_padding1[3];
  int m_list_styles;
  int m_fixed_width;
  bool m_bIndent;
  unsigned char m_padding2[3];
  int m_indent_spacing;
  int m_indet_width;
  CRhinoUiImageList m_image_list;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL32 bMinimized);
  afx_msg void OnActivateApp(BOOL32 bActive, DWORD dwThreadID);
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL32 OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg LRESULT OnNcHitTest(CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnSysColorChange();
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual BOOL32 OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual void PostNcDestroy();
  virtual BOOL32 OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


