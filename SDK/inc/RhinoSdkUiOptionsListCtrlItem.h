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

class CRhinoUiOptionsListCtrl;

class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlItem
{
public:
  //   
  // Description:
  //   Valid CRhinoUiOptionsListCtrlItem types
  //
  enum control_type
  {
    // implemented
    ct_static_text = 0,  // CRhinoUiOptionsListCtrlStaticText - Static text. Drawn same as label text.
    ct_edit_box,         // CRhinoUiOptionsListCtrlEditBox - Edit box which may be used to modify strings or numbers
    ct_check_box,        // CRhinoUiOptionsListCtrlCheckBox - Normal or tri-state check box.
    ct_combo_box,        // CRhinoUiOptionsListCtrlComboBox - Combo box, use the OnPopulateComboBox message to inialize
    ct_push_button,      // CRhinoUiOptionsListCtrlPushButton - Push button that generates nc_on_item_button_clicked notification event
    ct_ip_address,       // CRhinoUiOptionsListCtrlIPAddress - IP edit box
    ct_radio_buttons,    // CRhinoUiOptionsListCtrlRadioButtonColumn - Column of one or more radio buttons.
    ct_color_combo_box,  // CRhinoUiOptionsListCtrlComboBox - Combo box containing list of standard colors and/or custom color option
    ct_color_button,     // CRhinoUiOptionsListCtrlButton - Color button that displays standard color dialog when pressed
    ct_file,             // CRhinoUiOptionsListCtrlFileNameEditBox - Edit box with "..." butto which displays file open dialog box.
    ct_font_combo,       // CRhinoUiOptionsListCtrlFontComboBox - Combo box containing list of fonts installed on computer
    ct_layer_combo_box,  // CRhinoUiOptionsListCtrlLayerComboBox - Combo box containing list of layers in current document.
    ct_linetype_combo_box,
    ct_print_width_combo_box,
    ct_separator, // An item which spans both columns and simply displays a left justified label
    // not implemented yet
    ct_folder,
    ct_hyperlink,
    ct_custom_control,
    ct_invalid,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_control_type = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Constructors
  //
  // Parameters:
  //   ct [in] Type of control this is
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlItem( control_type ct = ct_invalid, LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Copy constructor
  //
  CRhinoUiOptionsListCtrlItem( const CRhinoUiOptionsListCtrlItem&);
  //   
  // Description:
  //   Operator equal
  //
  const CRhinoUiOptionsListCtrlItem& operator=( const CRhinoUiOptionsListCtrlItem&);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlItem();
  //   
  // Description:
  //   This method returns the current state of the auto delete variable.  If this returns true, which is the default,
  //   then this object is deleted automatically when the options list is destroyed.  Use the SetAutoDelete() method to
  //   change this value.
  //
  // Returns:
  //   Returns true to allow options list to delete this item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::SetAutoDelete()
  //   
  bool AutoDelete() const;
  //   
  // Description:
  //   This method is used to set the current state of the auto delete variable.  If this is set to true then this object
  //   will be deleted automatically when the options list is destroyed.
  //
  // Parameters:
  //   b [in] New value for auto delete variable
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::AutoDelete()
  //   
  bool SetAutoDelete( bool b);
  //   
  // Description:
  //   This method returns the current label for this options list control item.
  //
  // Returns:
  //   The current label for this item or NULL if no label.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::SetLabel()
  //   
  LPCTSTR Label() const;
  //   
  // Description:
  //   This method is used to set the current label for this options list control item.
  //
  // Parameters:
  //   lpsLabel [in] New label, not allowed to be NULL.
  //
  // Returns:
  //   Returns true if label is set, false if not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::Label()
  //   
  bool SetLabel( LPCTSTR lpsLabel);
  //   
  // Description:
  //   This method identifies what type of control this is.  This may be used to decide
  //   valid upcast for this item.
  //
  // Returns:
  //   Returns control type
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::control_type
  //   CRhinoUiOptionsListCtrlItem::SetControlType()
  //   
  control_type ControlType() const;
  //   
  // Description:
  //   This is the default method called to display options list control item text.
  //
  // Parameters:
  //   dc[in] Device context used to draw text.  This device context will already have the correct font
  //          selected and text color set.
  //   s[in]  Item text to display, if this is empty nothing is drawn.
  //   rItem [in] List control item text bounding rectangle
  //   rDeflate [in] Rectangle used to deflate rItem before drawing text.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  void DrawItemText( CDC& dc, const CString& s, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   This method returns the options list control this item has been added to.
  //
  // Returns:
  //   Returns the options list control this item has been added to.
  //
  // See Also:
  //   class CRhinoUiOptionsListCtrl
  //   CRhinoUiOptionsListCtrl::AddItem()
  //   CRhinoUiOptionsListCtrl::InsertItem()
  //   
  CRhinoUiOptionsListCtrl* GetListCtrl() const;
  //   
  // Description:
  //   This method is used to return text value for this item.  This value is displayed in the
  //   right column of the options list control.
  //
  // Returns:
  //   Return text value for this item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetText()
  //   CRhinoUiOptionsListCtrl::DrawItemText()
  //   
  virtual LPCTSTR Text() const;
  //   
  // Description:
  //   This method is used to set text value for this item.  This value is displayed in the
  //   right column of the options list control.
  //
  // Parameters:
  //   lps [in] New display text for this item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::Text()
  //   CRhinoUiOptionsListCtrl::DrawItemText()
  //   
  virtual void SetText( LPCTSTR lps);
  //   
  // Description:
  //   This method is used to retrieve the run-time ID for this options list control item.  Each options
  //   list control item will have an ID unique to its options list control.
  //
  // Returns:
  //   Returns unique runtime ID for this item.
  //
  unsigned int ItemID() const;
  //   
  // Description:
  //   This method is used to determine if this item is enabled or not.  BeginEditItem() is only called if
  //   this item is enabled.
  //
  // Returns:
  //   Returns true if this item is enabled; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetIsEnabled()
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  bool IsEnabled() const;
  //   
  // Description:
  //   This method is used to set the enabled state for this item.  BeginEditItem() is only called if
  //   this item is enabled.
  //
  // Parameters:
  //   b [in] New value for this item.
  //
  // Returns:
  //   Previous value for this item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsEnabled()
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  bool SetIsEnabled( bool b);
  //   
  // Description:
  //   This method is used to determine the edit state of an item.  The edit control associated with this
  //   item will be created as long as IsEnabled() is true.  If this value is true the user will be allowed
  //   to modify the control value, if false they will not.
  //
  // Returns:
  //   Returns true if item control value may be changed; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetIsReadOnly()
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  bool IsReadOnly() const;
  //   
  // Description:
  //   This method is used to set the edit state of an item.  The edit control associated with this
  //   item will be created as long as IsEnabled() is true.  If this value is true the user will be allowed
  //   to modify the control value, if false they will not.
  //
  // Parameters:
  //   b [in] New value.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsReadOnly()
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  bool SetIsReadOnly( bool b);
  //   
  // Description:
  //   This method is used to identify the unselected item label color.  The default value is CRhinoUiPaintManager::unset_color
  //   which causes the default color to be used.  The default color depends on the IsEnabled() state of the control item.  This
  //   color is only used when drawing un-selected items.
  //
  // Returns:
  //   Returns color to be used to fill the background of non-selected item values.  If color is CRhinoUiPaintManager::unset_color
  //   then default color is assumed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetLabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   CRhinoUiOptionsListCtrl::OnDrawItem()
  //   
  COLORREF LabelBackGroundColor() const;
  //   
  // Description:
  //   This method is used to set the unselected item label color.  The default value is CRhinoUiPaintManager::unset_color
  //   which causes the default color to be used.  The default color depends on the IsEnabled() state of the control item.  Set
  //   this value to change the background color on an item by item bases.
  //
  // Parameters:
  //   cr [in] New background color or CRhinoUiPaintManager::unset_color for default
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::LabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   CRhinoUiOptionsListCtrl::OnDrawItem()
  //   
  COLORREF SetLabelBackGroundColor( COLORREF cr);
  //   
  // Description:
  //   This method is used to identify the unselected item label text color.  The default value is CRhinoUiPaintManager::unset_color
  //   which causes the default color to be used.  The default color depends on the IsEnabled() state of the control item.  This
  //   color is only used when drawing un-selected items.
  //
  // Returns:
  //   Returns color to be used to to draw text of non-selected item values.  If color is CRhinoUiPaintManager::unset_color
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
  //   This method is used to set the unselected item label text color.  The default value is CRhinoUiPaintManager::unset_color
  //   which causes the default color to be used.  The default color depends on the IsEnabled() state of the control item.  Set
  //   this value to change the text color on an item by item bases.
  //
  // Parameters:
  //   cr [in] New text color or CRhinoUiPaintManager::unset_color for default
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::LabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::OnDrawItem()
  //   
  COLORREF SetLabelTextColor( COLORREF cr);
  // Description:
  //   Call this method to determine the indent level of this items label.  This is only used when
  //   CRhinoUiOptionsListCtrl::IndentItems() is true.  If item indenting is enabled then this value
  //   is multiplied by CRhinoUiOptionsListCtrl::IndentSpacing() when indenting item labels.  The
  //   default value is zero.
  //
  // Returns:
  //   Returns number of indents to preceed this item label.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IndentItems()
  //   CRhinoUiOptionsListCtrl::IndentSpacing()
  //   CRhinoUiOptionsListCtrlItem::SetIndentLevel()
  //   
  int IndentLevel() const;
  // Description:
  //   Call this method to set the indent level of this items label.  This is only used when 
  //   CRhinoUiOptionsListCtrl::IndentItems() is true.  If item indenting is enabled then this value
  //   is multiplied by CRhinoUiOptionsListCtrl::IndentSpacing() when indenting item labels.  The
  //   default value is zero.
  //
  // Parameters:
  //   iIndentLevel [in] Number of indents to preceed this item label.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IndentItems()
  //   CRhinoUiOptionsListCtrl::IndentSpacing()
  //   CRhinoUiOptionsListCtrlItem::IndentLevel()
  //   
  int SetIndentLevel( int iIndentLevel);
  //   
  // Description:
  //   Call this method to see if this item should suppress the item label and display the control and control
  ///  text in the entire row.
  //
  // Returns:
  //   Returns true if label will be suppressed and item value will span the entire row otherwise returns false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsFullRowItem()
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  bool IsFullRowItem() const;
  //   
  // Description:
  //   Call this method to tell the item if it should suppress the item label and display the control and control
  ///  text in the entire row.
  //
  // Parameters:
  //   b [in] New value.  If b is true then the item label will be suppressed and the control will san the entire
  //          row.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::IsFullRowItem()
  //   CRhinoUiOptionsListCtrl::BeginEditItem()
  //   
  bool SetIsFullRowItem( bool b);
  //   
  // Description:
  //   This method is called when painting the value portion of the item.  If this method returns true then the
  //   background and text colors used for the label portion will be used when drawing the value.
  //
  // Returns:
  //   Returns true if label colors will be used when drawing values otherwise returns false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetUseLabelColors()
  //   CRhinoUiOptionsListCtrl::LabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   
  bool UseLabelColors() const;
  //   
  // Description:
  //   Call this method to tell the control to use the background and text colors used to draw the label portion
  //   of the item row when drawing the value.
  //
  // Parameters:
  //   b [in] New value.  If b is true then the item label colors will be used when drawing values portion of
  //          the item row.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetUseLabelColors()
  //   CRhinoUiOptionsListCtrl::LabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::SetLabelBackGroundColor()
  //   CRhinoUiOptionsListCtrl::LabelTextColor()
  //   CRhinoUiOptionsListCtrl::SetLabelTextColor()
  //   
  bool SetUseLabelColors( bool b);
  //   
  // Description:
  //   Call this method find out if a item will be included in the list control display.
  //
  // Returns:
  //   Returns true if item is hidden otherwise returns false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::HideItem()
  //
  bool IsItemHidden( ) const;
  int ParentIndex() const;
  bool SetParentIndex( int iItem);
  enum item_state
  {
    expanded_state     =      1,
    hidden_state       =      2,
    enabled_state      =      4,
    readonly_state     =      8,
    hidden_group_state =   0x10,
    state0             =   0x20, // For future use
    state1             =   0x40,
    state2             =   0x80,
    state3             =  0x100,
    state4             =  0x200,
    state5             =  0x400,
    state6             =  0x800,
    state7             = 0x1000,
    state8             = 0x2000,
    state9             = 0x4000,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_item_state = 0xFFFFFFFF
  };
  bool GetItemState( item_state istate) const;
  bool SetItemState( item_state istate, bool bEnabled);
  enum item_property
  {
    full_row_item_property       =      1,
    use_label_colors_property    =      2,
    is_group_combo_item_property =      4, // DO NOT set this property, it will be set by the 
                                           // CRhinoUiOptionsListCtrlGroupComboBox constructor
                                           // and identifies this item as a group combo box so
                                           // it may be safely cast as such.
    property0                    =      8, //for future use
    property1                    =   0x10,
    property2                    =   0x20,
    property3                    =   0x40,
    property4                    =   0x80,
    property5                    =  0x100,
    property6                    =  0x200,
    property7                    =  0x400,
    property8                    =  0x800,
    property9                    = 0x1000,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_item_property = 0xFFFFFFFF
  };
  bool GetItemProperty( item_property iprop) const;
  bool SetItemProperty( item_property iprop, bool bEnabled);
  bool IsItemExpandable() const;
protected:
  //   
  // Description:
  //   This method is used to set the control type of this options list control item. See 
  //   CRhinoUiOptionsListCtrlItem::control_type for valid options.  This method should be called
  //   one time by the constructor, you should not change a control item type on the fly.
  //
  // Parameters:
  //   ct [in] Type ID for this control item
  //
  // Returns:
  //   Returns true if control type is set.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::control_type
  //   CRhinoUiOptionsListCtrlItem::ControlType()
  //   
  bool SetControlType( control_type ct);
  //---------------------------------------------------------------------------------------------------------------
  // Virtual methods
  //---------------------------------------------------------------------------------------------------------------
  enum begin_edit_item_reason
  {
    eir_label_left_clicked = 0, // Mouse click over the left (label) column
    eir_value_left_clicked,     // Mouse click over item in the right (value) column
    eir_f2_pressed,             // F2 function key was pressed to begin edit
    eir_navigation_key,         // Up or down arrow or page up or down keys pressed
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_begin_edit_item_reason = 0xFFFFFFFF
  };
  //   
  // Description:
  //   This method is called when a options list control item is selected.  The method should create the
  //   control window used to change the items value.
  //
  // Parameters:
  //   list_ctrl [in] The options list control calling this method.
  //   rEdit [in] The options list control item value bounding rectangle.
  //
  // Returns:
  //   Should return true if the control has been successfully created; otherwise false.
  //
  virtual bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir) = 0;
  //   
  // Description:
  //   This method is called when a column or options list control has been resized while a item control is
  //   visible.
  //
  // Parameters:
  //   list_ctrl [in] The list control this item is inserted in.
  //   rEdit [in] The options list control item value bounding rectangle.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //   
  virtual void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit) = 0;
  //   
  // Description:
  //   This method is called by the options list control to draw the options list control item value.
  //   The background will already be filled with the appropriate color and the device context will
  //   have the correct font and font color set.
  //
  // Parameters:
  //   [dc] Device context used for drawing
  //   rItem [in] The options list control item value bounding rectangle.
  //   rDeflate [in] Standard margins used to deflate rITem rectangle prior to drawing
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::SetText()
  //   CRhinoUiOptionsListCtrlItem::Text()
  //   
  virtual void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   This method is called to determine height for this options list control item.  Override this method
  //   if you want to increase the height of this item.
  //
  // Returns:
  //   Returns height for this item.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::LabelItemHeight()
  //   
public:
  virtual int ItemHeight() const;
protected:
  //   
  // Description:
  //   This method is may be called to determine if the options list control associated with this item
  //   can be determined and if it points to a window which is currently created.
  //
  // Parameters:
  //   wnd [in] Pointer to control item edit window
  //   nFlags [in] Indicates whether various virtual keys are down. This parameter can be any combination of the following values: 
  //      MK_CONTROL   Set if the CTRL key is down. 
  //      MK_LBUTTON   Set if the left mouse button is down. 
  //      MK_MBUTTON   Set if the middle mouse button is down. 
  //      MK_RBUTTON   Set if the right mouse button is down. 
  //      MK_SHIFT   Set if the SHIFT key is down. 
  //   zDelta [in] Indicates distance rotated. The zDelta value is expressed in multiples or divisions of WHEEL_DELTA, which is 120.
  //               A value less than zero indicates rotating back (toward the user) while a value greater than zero indicates rotating
  //               forward (away from the user). The user can reverse this response by changing the Wheel setting in the mouse software.
  //               See the Remarks for more information about this parameter. 
  //   pt [in] Specifies the x- and y-coordinate of the cursor. These coordinates are always relative to the upper-left corner of the screen. 
  //
  // Returns:
  //   Returns true if WM_MOUSEWHEEL message would be forwarded to options list control
  //
  // See Also:
  //   CWnd::OnMouseWheel()
  //   
  bool ForwardOnMouseWheel( CWnd& wnd, UINT nFlags, short zDelta, CPoint pt);
private:
  friend class CRhinoUiOptionsListCtrl;

private:
  CRhinoUiOptionsListCtrl* m_pListCtrl;
  CString m_sLabel;
  control_type m_type;
  bool m_bAutoDelete;
  CWnd* m_pCtrl;
  CString m_sText;
  unsigned int m_iItemID;
  COLORREF m_crLabelBkGrnd;
  COLORREF m_crLabelText;
  int m_indent_level;
  int m_iParent;
  int m_iState;
  int m_iProperty;
  int m_iExpands;
protected:
  CPoint m_ptEndTrackSelect;
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlSeparator
//
// Description:
//    Static text options list control item
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlSeparator : public CRhinoUiOptionsListCtrlItem
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::CRhinoUiOptionsListCtrlItem()
  //   
  CRhinoUiOptionsListCtrlSeparator( LPCTSTR lpsLabel = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlSeparator();
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  //   
  // Description:
  //   This method is overridden to display separator line if enabled
  //
  // Parameters:
  //   See: CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  virtual void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  void SetText( LPCTSTR lps);
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
  int ItemHeight() const;
  bool DrawSeparatorLine() const;
  bool SetDrawSeparatorLine( bool b);
protected:
  bool m_bDrawSeparatorLine;
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlStaticText
//
// Description:
//    Static text options list control item
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlStaticText : public CRhinoUiOptionsListCtrlItem
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::CRhinoUiOptionsListCtrlItem()
  //   
  CRhinoUiOptionsListCtrlStaticText( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlStaticText();
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlEditBox
//
// Description:
//    Edit box control.  This control can be used to edit string, double or integer values.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlEditBox : public CRhinoUiEdit, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlEditBox)
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem constructor
  //   
	CRhinoUiOptionsListCtrlEditBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiOptionsListCtrlEditBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //
  //   Create CRhinoUiEdit control for editing the Text() value associated with this item.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if edit control was created; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //   
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  // Description:
  //   CRhinoUiOptionsListCtrlItem override
  void SetText( LPCTSTR lps);
  // Description:
  //   CRhinoUiOptionsListCtrlItem override
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //   
  //   See CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //   
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
  //   
  // Description:
  //   This method is called when this edit control looses focus to validate the
  //   new control value prior to saving.
  //
  // Parameters:
  //   lps [in] New text value
  //
  // Returns:
  //   Returns true if lps is valid and should be saved; otherwise false.
  //
  virtual bool OnEndEdit( LPCTSTR lps) const;
  //   
  // Description:
  //   Convert double to string and put string in edit control.
  //
  // Parameters:
  //   d [in] Value to place in edit control
  //
  void SetNumber( double d);
  //   
  // Description:
  //   Convert int to string and put string in edit control.
  //
  // Parameters:
  //   i [in] Value to place in edit control
  //   
  void SetNumber( int i);
private:
  //   
  // Description:
  //   This method is called to validate control value prior to saving.  This method will call
  //   OnEndEdit() prior to returning.
  //
  // Returns:
  //   Returns true if value should be updated; otherwise false
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlEditBox::OnEndEdit()
  //
  bool GetTextFromEditControl();

protected:
  bool m_bCancel;
  mutable int m_edit_state = 0;

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  afx_msg void OnEnKillfocus();
  afx_msg void OnEnChange();
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlPointEditBox
//
// Description:
//    Edit box for point input.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlPointEditBox : public CRhinoUiPointEdit, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlPointEditBox)
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem constructor
  //   
	CRhinoUiOptionsListCtrlPointEditBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiOptionsListCtrlPointEditBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //
  //   Create CRhinoUiEdit control for editing the Text() value associated with this item.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if edit control was created; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //   
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  // Description:
  //   CRhinoUiOptionsListCtrlItem override
  void SetText( LPCTSTR lps);
  // Description:
  //   CRhinoUiOptionsListCtrlItem override
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //   
  //   See CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //   
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
  //   
  // Description:
  //   This method is called when this edit control looses focus to validate the
  //   new control value prior to saving.
  //
  // Parameters:
  //   lps [in] New text value
  //
  // Returns:
  //   Returns true if lps is valid and should be saved; otherwise false.
  //
  virtual bool OnEndEdit( LPCTSTR lps) const;
  //   
  // Description:
  //   Convert double to string and put string in edit control.
  //
  // Parameters:
  //   d [in] Value to place in edit control
  //
  void SetNumber( double d);
  //   
  // Description:
  //   Convert int to string and put string in edit control.
  //
  // Parameters:
  //   i [in] Value to place in edit control
  //   
  void SetNumber( int i);

protected:
  bool GetTextFromEditControl();

  bool m_bCancel;
  bool m_bInOnBeginEdit;

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual BOOL32 OnCommand(WPARAM wParam, LPARAM lParam);
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlFileNameEditBox
//
// Description:
//   Display a read only edit box containing file name and a "..." button used to browse for
//   file name.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlFileNameEditBox : public CRhinoUiOptionsListCtrlEditBox
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlFileNameEditBox)
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlFileNameEditBox(  LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlFileNameEditBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //
  //   Create read only edit control and browse button.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if edit control and browse button are created; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlEditBox::BeginEditItem()
  //   
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  // Description:
  //   CRhinoUiOptionsListCtrlItem override
  void SetText( LPCTSTR lps);
  // Description:
  //   CRhinoUiOptionsListCtrlItem override
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   Call this method to get the control object for the file button.  When the file button is pressed it will
  //   use this object to display a standard Rhino file dialog.  This will only get used if SetFileDialogCtrlType()
  //   is set to custom.
  //
  // Returns:
  //   Rhino file dialog control object used by the file button associated with this item.
  //
  // See Also:
  //   CRhinoGetFileDialog
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialogCtrlType()
  //   
  CRhinoGetFileDialog& FileDialogCtrl();
  //   
  // Description:
  //   Call this method to get Rhino file dialog mode which will be used when the file button is pressed.  If this
  //   returns CRhinoGetFileDialog::custom FileDialog() will be used instead.
  //
  // Returns:
  //   Rhino file dialog control type to be used by the FileDialogCtrl()
  //
  // See Also:
  //   CRhinoGetFileDialog
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialogCtrlType()
  //   
  CRhinoGetFileDialog::file_dialog_type FileDialogCtrlType() const;
  //   
  // Description:
  //   Call this method to set the Rhino file dialog mode which will be used when the file button is pressed.  If this
  //   returns CRhinoGetFileDialog::custom FileDialog() will be used instead.
  //
  // Parameters:
  //   type [in] New Rhino file dialog mode.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoGetFileDialog
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialogCtrlType()
  //   
  CRhinoGetFileDialog::file_dialog_type SetFileDialogCtrlType( CRhinoGetFileDialog::file_dialog_type type);
  //   
  // Description:
  //   Call this method to set the CFileDialog object which will be used by the file button if the FileDialogCtrlType()
  //   is equal to CRhinoGetFileDialog::custom
  //
  // Parameters:
  //   pFD [in] Pointer to CFileDialog control object to be used by the file button.
  //
  // Returns:
  //   Returns the CFileDialog object which will be used by the file button if the FileDialogCtrlType()
  //   is equal to CRhinoGetFileDialog::custom
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrl()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialog()
  //   
  void SetFileDialog( CFileDialog* pFD);
  //   
  // Description:
  //   Call this method to get the CFileDialog object which will be used by the file button if the FileDialogCtrlType()
  //   is equal to CRhinoGetFileDialog::custom
  //
  // Returns:
  //   Returns the CFileDialog object which will be used by the file button if the FileDialogCtrlType()
  //   is equal to CRhinoGetFileDialog::custom
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrl()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::FileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialogCtrlType()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetFileDialog()
  //   
  CFileDialog* FileDialog();
  //   
  // Description:
  //   Call this method to determine if a warning message should be displayed when the delete key is pressed
  //   which will clear the file name.  The warning message will include Yes and No buttons to give the user
  //   the opportunity to change there mind.  The default value is false.
  //
  // Returns:
  //   Returns true if a warning will be displayed otherwise false
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetWarnOnDeleteKey()
  //   
  bool WarnOnDeleteKey() const; // Defaults to false
  //   
  // Description:
  //   Call this method to tell the control to display or suppress a warning message when the delete key is pressed
  //   which will clear the file name.  The warning message will include Yes and No buttons to give the user
  //   the opportunity to change there mind.
  //
  // Parameters:
  //   b [in] New value for this mode.
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFileNameEditBox::WarnOnDeleteKey()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::ClearTextOnDeleteKey()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetClearTextOnDeleteKey()
  //   
  bool SetWarnOnDeleteKey( bool b);
  //   
  // Description:
  //   Call this method to determine if file name should be cleared when the delete key is pressed.
  //   The default value is true.
  //
  // Returns:
  //   Returns true if file name will be cleared when the delete key is pressed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetWarnOnDeleteKey()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetWarnOnDeleteKey()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::ClearTextOnDeleteKey()
  //   
  bool ClearTextOnDeleteKey() const; // Defaults to true
  //   
  // Description:
  //   Call this method to tell the control if the file name should be cleared when the delete key is
  //   pressed.  The default value is true.
  //
  // Parameters:
  //   b [in] New value for this mode.
  //
  // Returns:
  //   Returns previous value
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFileNameEditBox::WarnOnDeleteKey()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::SetWarnOnDeleteKey()
  //   CRhinoUiOptionsListCtrlFileNameEditBox::ClearTextOnDeleteKey()
  //   
  bool SetClearTextOnDeleteKey( bool b); // Defaults to true
  //   
  // Description:
  //   Call this method to display the file dialog in the same manner as when the
  //   "..." file button is clicked.  This will only work if the file window has
  //   been created.
  //
  // Returns:
  //   Returns true if the control windows exists and button click was simulated
  //   otherwise; returns false.
  //
  bool DisplayFileDialogNow();

private:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);

  friend class CRhUiOptionsListCtrlFileNameEditBoxButton;
  class CRhUiOptionsListCtrlFileNameEditBoxButton* m_file_button;
  bool m_file_button_pushed;
  CRhinoGetFileDialog m_get_file_dialog;
  CFileDialog* m_file_dialog;
  bool m_delete_file_dialog;
  bool m_warn_on_delete_key;
  bool m_clear_on_delete_key;
  CRhinoGetFileDialog::file_dialog_type m_file_dialog_type; // defaults to CRhinoGetFileDialog::open_bitmap_dialog
                                                            // Setting to CRhinoGetFileDialog::custom will cause
                                                            // standard CFile dialog to appear
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnDestroy();
  afx_msg void OnEnKillfocus();
};

//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlIPAddress
//
// Description:
//   IP edit box.  See "IP Address Controls" topic in MSDN
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlIPAddress : public CWnd, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlIPAddress)
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
	CRhinoUiOptionsListCtrlIPAddress( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiOptionsListCtrlIPAddress();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //
  //   IP edit box.  See "IP Address Controls" topic in MSDN
  //
  //   Microsoft® Internet Explorer Version 4.0 introduces the IP address control, a new control similar to an edit control
  //   that allows the user to enter a numeric address in Internet protocol (IP) format. This format consists of four three-digit
  //   fields. Each field is treated individually; the field numbers are zero-based and proceed from left to right as shown in
  //   this figure. 
  //       Field 0
  //          |    Field 1
  //          |       |    Field 2
  //          |       |       |    Field 3
  //          |       |       |       |
  //         255    .255    .255     .0
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if IP edit box was successfully created; otherwise false.
  //
  // See Also:
  //   "IP Address Controls" topic in MSDN
  //   CRhinoUiOptionsListCtrlEditBox::BeginEditItem()
  //   
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem override.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem::OnEndEdit() override.
  //
  //   Adjust the IP controls size and position as necessary if the options list controls size is
  //   changed while the control is visible.
  //
  // Parameters:
  //   See See CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //   
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem::OnEndEdit() override.
  //
  // Parameters:
  //   See  CRhinoUiOptionsListCtrlItem::OnEndEdit()
  //
  // Returns:
  //   Returns true since the IP control validates input as it is made.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnEndEdit()
  //   
  virtual bool OnEndEdit( LPCTSTR lps) const;
  //   
  // Description:
  //   Get the last valid IP address input.
  //
  // Returns:
  //   Returns a long value which identifies the IP address.
  //     The field 3 value will be contained in bits 0 through 7.
  //     The field 2 value will be contained in bits 8 through 15.
  //     The field 1 value will be contained in bits 16 through 23.
  //     The field 0 value will be contained in bits 24 through 31.
  //   The FIRST_IPADDRESS, SECOND_IPADDRESS, THIRD_IPADDRESS, and FOURTH_IPADDRESS macros can also be used to extract the 
  //   address information. Zero will be returned as the address for any blank fields. 
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnEndEdit()
  //   CRhinoUiOptionsListCtrlIPAddress::SetIPAddress()
  //   FIRST_IPADDRESS
  //   SECOND_IPADDRESS
  //   THIRD_IPADDRESS
  //   FOURTH_IPADDRESS
  // 
  DWORD IPAddress() const;
  //   
  // Description:
  //   Sets the address values for all four fields in the IP address control.
  //
  // Parameters:
  //   ip [in] Value that contains the new address. The field 3 value is contained in bits 0 through 7. The field 2 value is contained
  //           in bits 8 through 15. The field 1 value is contained in bits 16 through 23. The field 0 value is contained in bits 24
  //           through 31. The MAKEIPADDRESS macro can also be used to create the address information. 
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlIPAddress::IPAddress()
  //   IPM_SETADDRESS window message
  //   MAKEIPADDRESS macro
  //   
  DWORD SetIPAddress(DWORD ip);
private:
  bool GetTextFromEditControl();

protected:
  bool m_bCancel;
  DWORD m_ip_address;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnKillfocus();
  afx_msg void OnEnChange();
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlComboBox
//
// Description:
//   This control will display a simple combo box containing a list of options.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlComboBox : public CRhinoUiComboBox, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlComboBox)
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //   ct [in] Type of control this is
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::CRhinoUiOptionsListCtrlItem()
  //   
  CRhinoUiOptionsListCtrlComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0, CRhinoUiComboBoxItemData::class_type ct = CRhinoUiComboBoxItemData::ct_strings);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem override.
  //
  //   Create simple, drop down combo box.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if comob box was created; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //   
  virtual bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem override.
  //
  //   Move and/or resize combo box when options list control is resized and the combo
  //   box exists.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::MoveItemWindow()
  //   
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
  //   
  // Description:
  //   This method is called by the control when the combo box has been created and needs
  //   to be populated with data.  If this method returns false the combo box will destroyed.
  //
  //   The default implementation of this method sends a CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   custom windows message with a CRhinoUiOptionsListCtrl::nc_populate_combo_box notification code
  //   to the parent of the options list control this item is inserted in.  You can either override
  //   this method prior to adding a item to the options list control or respond the the nc_populate_combo_box
  //   in the window that owns the options list control.  See CRhinoUiOptionsListCtrl::notification_codes
  //   for details.
  //
  // Returns:
  //   Returns true if the combo box has been initialized and populated and should be displayed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   
  virtual bool PopulateComboBox();
  //   
  // Description:
  //   This method is called after PopulateComboBox() to set default value or modify the contents of 
  //   the combo box.
  //
  //   The default implementation of this method sends a CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   custom windows message with a CRhinoUiOptionsListCtrl::nc_on_populate_combo_box notification code
  //   to the parent of the options list control this item is inserted in.  You can either override
  //   this method prior to adding a item to the options list control or respond the the nc_populate_combo_box
  //   in the window that owns the options list control.  See CRhinoUiOptionsListCtrl::notification_codes
  //   for details.
  //
  // Returns:
  //   Returns true if combo box should be displayed; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   
  virtual bool OnComboBoxPopulated();
  //   
  // Description:
  //   This method is called when the current selection of the combo box has changed.
  //
  //   The default implementation of this method sends a CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   custom windows message with a CRhinoUiOptionsListCtrl::nc_on_combo_box_sel_changed notification code
  //   to the parent of the options list control this item is inserted in.  You can either override
  //   this method prior to adding a item to the options list control or respond the the nc_populate_combo_box
  //   in the window that owns the options list control.  See CRhinoUiOptionsListCtrl::notification_codes
  //   for details.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   
  virtual void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   This method is called by BeginEditItem() after the combo box has been created,
  //   populated and initialized.
  //
  // Returns:
  //   If this method returns true then the the list box of the combo box that has will
  //   be shown; otherwise it will be hidden.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetShowDropDownOnBeginEdit()
  //   CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //   
  bool ShowDropDownOnBeginEdit() const;
  //   
  // Description:
  //   Call this method to cause the list box of the combo box to be shown when the item
  //   is selected for editing.
  //
  // Parameters:
  //   b [in] If truen then the list box is shown automatically; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::ShowDropDownOnBeginEdit()
  //   CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //   
  bool SetShowDropDownOnBeginEdit( bool b);
  //   
  // Description:
  //   This method is called by OnDrawItem() to determine if the drop down arrow
  //   should be drawn.
  //
  // Returns:
  //   If this method returns true then the drop down arrow is drawn by OnDrawItem();
  //   otherwise it is not.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetDrawDownArrow()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::OnDrawItem()
  //   
  bool DrawDownArrow() const;
  //   
  // Description:
  //   Call this method to specify if a combo box drop down list arrow should be
  //   drawn by OnDrawItem().
  //
  // Parameters:
  //   b [in] If truen then the drop down list arrow is drawn; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::DrawDownArrow()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::OnDrawItem()
  //   
  bool SetDrawDownArrow( bool b);
  //   
  // Description:
  //   This method is overridden to display the combo box drop down list arrow when the
  //   list item is not active for editing.
  //
  // Parameters:
  //   See: CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  virtual void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   Call this method to draw the drop down arrow for a combo box item.
  //
  // Parameters:
  //   dc [in] Device context to draw into
  //   rItem [in/out] Rectangle of area available for drawing.  The right side of this
  //                  rect will be adjusted to reflect the area drawn by the arrow.
  //   rDeflate [in] Amount to deflate the draw rectangle to account for item borders.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  virtual void OnDrawDownArrow( CDC& dc, CRect& rItem, const CRect& rDeflate) const;
// CRhinoUiOptionsListCtrlComboBox
protected:
  // CRhinoUiComboBox override
  void ShowLayerPopDownWnd( HWND hWnd, bool bShow);
protected:
  bool m_bOwnerDraw;
  bool m_bShowDropDownOnBeginEdit;
  bool m_bDrawDownArrow;
  bool m_bIncludeEditCtrl;
  bool m_bSort;
  bool m_bShowingDropDown;

protected:
  DECLARE_MESSAGE_MAP()
  afx_msg void OnCbnSelchange();
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnEditKillFocus();

public:
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlGroupComboBox
//
// Description:
//   This control will display a simple combo box containing a list of options.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlGroupComboBox : public CRhinoUiOptionsListCtrlComboBox
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //   ct [in] Type of control this is
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::CRhinoUiOptionsListCtrlItem()
  //   CRhinoUiOptionsListCtrlItem::CRhinoUiOptionsListCtrlComboBox()
  //   
  CRhinoUiOptionsListCtrlGroupComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0, CRhinoUiComboBoxItemData::class_type ct = CRhinoUiComboBoxItemData::ct_strings);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlGroupComboBox();
  //   
  // Description:
  //   Call this method to add an item and identify items associated with this group
  //
  // Parameters:
  //   lpsComboText [in] String to display in the comobo box
  //   group_items [in] List of options list control items associated with this combo box item
  //
  // Returns:
  //   Returns zero based combo box index for new item or -1 on error
  //   
  int AddGroupItem( const wchar_t* lpsComboText, ON_SimpleArray<CRhinoUiOptionsListCtrlItem*>& group_items);
  //   
  // Description:
  //   Call this method to add an item and identify items associated with this group
  //
  // Parameters:
  //   lpsComboText [in] String to display in the comobo box
  //   group_items [in] List of options list control items associated with this combo box item
  //   count [in] Number of items in group_items
  //
  // Returns:
  //   Returns zero based combo box index for new item or -1 on error
  //   
  int AddGroupItem( const wchar_t* lpsComboText, CRhinoUiOptionsListCtrlItem** group_items, const int count);
  //   
  // Description:
  //   This method is called by the control when the combo box has been created and needs
  //   to be populated with data.  If this method returns false the combo box will destroyed.
  //
  //   The default implementation of this method sends a CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   custom windows message with a CRhinoUiOptionsListCtrl::nc_populate_combo_box notification code
  //   to the parent of the options list control this item is inserted in.  You can either override
  //   this method prior to adding a item to the options list control or respond the the nc_populate_combo_box
  //   in the window that owns the options list control.  See CRhinoUiOptionsListCtrl::notification_codes
  //   for details.
  //
  // Returns:
  //   Returns true if the combo box has been initialized and populated and should be displayed.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   
  virtual bool PopulateComboBox();
  //   
  // Description:
  //   This method is called after PopulateComboBox() to set default value or modify the contents of 
  //   the combo box.
  //
  //   The default implementation of this method sends a CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   custom windows message with a CRhinoUiOptionsListCtrl::nc_on_populate_combo_box notification code
  //   to the parent of the options list control this item is inserted in.  You can either override
  //   this method prior to adding a item to the options list control or respond the the nc_populate_combo_box
  //   in the window that owns the options list control.  See CRhinoUiOptionsListCtrl::notification_codes
  //   for details.
  //
  // Returns:
  //   Returns true if combo box should be displayed; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::BeginEditItem()
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   
  virtual bool OnComboBoxPopulated();
  //   
  // Description:
  //   This method is called when the current selection of the combo box has changed.
  //
  //   The default implementation of this method sends a CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   custom windows message with a CRhinoUiOptionsListCtrl::nc_on_combo_box_sel_changed notification code
  //   to the parent of the options list control this item is inserted in.  You can either override
  //   this method prior to adding a item to the options list control or respond the the nc_populate_combo_box
  //   in the window that owns the options list control.  See CRhinoUiOptionsListCtrl::notification_codes
  //   for details.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrl::m_custom_win_msg_id
  //   CRhinoUiOptionsListCtrl::notification_codes
  //   
  virtual void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   Call this method to set the current combo box selection.  Setting the current selection will show all
  //   items associated with that selection and hide all items associated with the previous selection
  //
  // Parameters:
  //   iCur [in] Zero based index of item to make current.
  //
  // Returns:
  //   Returns previous current item or -1 if there was none
  //   
  // See Also:
  //   CRhinoUiOptionsListCtrl::CurGroupSel()
  //   
  int SetCurGroupSel( int iCur);
  //   
  // Description:
  //   Call this method to get the current combo box selection.
  //
  // Returns:
  //   Returns zero based index of current selection
  //   
  // See Also:
  //   CRhinoUiOptionsListCtrl::SetCurGroupSel()
  //   
  int CurGroupSel() const;

protected:
  friend class CGroupComboItem;

  DECLARE_MESSAGE_MAP()

  int m_iCurGroupSel;
  class CGroupComboData* m_pData;
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlLinetypeComboBox
//
// Description:
//   This control will display a simple combo box containing a list loaded line types.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlLinetypeComboBox : public CRhinoUiOptionsListCtrlComboBox
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlLinetypeComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0, CRhinoUiComboBoxItemData::class_type ct = CRhinoUiComboBoxItemData::ct_linetype);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlLinetypeComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox virtual override
  //
  //   Overridden to fill combo box with loaded linetypes
  //
  // Returns:
  //   Returns true if combo box is filled with one or more of the loaded linetypes; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetIncludeByLayer()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeByLayer()
  //   
  bool PopulateComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override, calls base class.
  //
  //   Overridden to save currently selected linetype
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::OnItemComboBoxSelChanged()
  //   
  void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetype options to combo box.
  //
  // Returns:
  //   Returns true if "By Layer" option should be added to linetype list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetIncludeByLayer()
  //   
  bool IncludeByLayer() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of "By Layer" linetype option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then "By Layer" option is added to combo box; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeByLayer()
  //   
  bool SetIncludeByLayer( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetypes to combo box.
  //
  // Returns:
  //   Returns true if "By Parent" option should be added to linetype list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetIncludeByParent()
  //   
  bool IncludeByParent() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of "By Parent" linetype option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then "By Parent" option is added to combo box; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeByLayer()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeByParent()
  //   
  bool SetIncludeByParent( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetype options to combo box.
  //
  // Returns:
  //   Returns true if default linetypes option should be added to linetype list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetIncludeDefaults()
  //   
  bool IncludeDefaults() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of default linetypes option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then default linetypes are added to combo box; otherwise they are not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeDefaults()
  //   
  bool SetIncludeDefaults( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetypes to the combo box.
  //
  // Returns:
  //   Returns true if deleted linetype names should be added to the combo box; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetIncludeDeletedLinetypes()
  //   
  bool IncludeDeletedLinetypes() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of deleted linetypes to the combo box.
  //
  // Parameters:
  //   b [in] If truen then deleted linetype names are added to combo box; otherwise they are not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeDeletedLinetypes()
  //   
  bool SetIncludeDeletedLinetypes( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetypes to the combo box.
  //
  // Returns:
  //   Returns true if reference linetype names should be added to the combo box; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::SetIncludeReferenceLinetypes()
  //   
  bool IncludeReferenceLinetypes() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of reference linetypes to the combo box.
  //
  // Parameters:
  //   b [in] If truen then reference linetype names are added to combo box; otherwise they are not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeReferenceLinetypes()
  //   
  bool SetIncludeReferenceLinetypes( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetypes to the combo box.  If
  //   this is set to true then CRhinoLinetypeTable::GetSortedList is called otherwise all linetypes
  //   are included.
  //
  // Returns:
  //   Returns true if sorted linetype list should be used; otherwise complete linetype list will be
  //   used.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeDeletedLinetypes()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeReferenceLinetypes()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   
  bool UseSortedList() const;
  //   
  // Description:
  //   Call this method to control what linetypes get added to the combo box.  If this is set
  //   to true then the sorted and culled list displayed in the linetype dialog is used.  If this
  //   set set to false then the entire linetype list is used.  SetIncludeReferenceLinetypes() and
  //   SetIncludeDeletedLinetypes() are used in either case to filter the display of those linetypes.
  //
  // Parameters:
  //   b [in] If true then the linetype dialog list is used.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeDeletedLinetypes()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::IncludeReferenceLinetypes()
  //   CRhinoUiOptionsListCtrlLinetypeComboBox::PopulateComboBox()
  //   
  bool SetUseSortedList( bool b);
private:
  bool m_include_by_layer;
  bool m_include_by_parent;
  bool m_include_defaults;
  bool m_include_reference_linetypes;
  bool m_include_deleted_linetypes;
  bool m_use_sorted_list;
protected:
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchange();
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlPrintWidthComboBox
//
// Description:
//   This control will display a simple combo box containing a list loaded line types.
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlPrintWidthComboBox : public CRhinoUiOptionsListCtrlComboBox
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlPrintWidthComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0, CRhinoUiComboBoxItemData::class_type ct = CRhinoUiComboBoxItemData::ct_linetype);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlPrintWidthComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox virtual override
  //
  //   Overridden to fill combo box with loaded linetypes
  //
  // Returns:
  //   Returns true if combo box is filled with one or more of the loaded linetypes; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::SetIncludeByLayer()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeByLayer()
  //   
  bool PopulateComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override, calls base class.
  //
  //   Overridden to save currently selected plot width option
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::OnItemComboBoxSelChanged()
  //   
  void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding width options to combo box.
  //
  // Returns:
  //   Returns true if "By Layer" option should be added to width list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::SetIncludeByLayer()
  //   
  bool IncludeByLayer() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of "By Layer" width option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then "By Layer" option is added to combo box; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeByLayer()
  //   
  bool SetIncludeByLayer( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding linetypes to combo box.
  //
  // Returns:
  //   Returns true if "By Parent" option should be added to width list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::SetIncludeByParent()
  //   
  bool IncludeByParent() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of "By Parent" width option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then "By Parent" option is added to combo box; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeByLayer()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeByParent()
  //   
  bool SetIncludeByParent( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding plot width options to combo box.
  //
  // Returns:
  //   Returns true if default plot width option should be added to the width list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::SetIncludeDefault()
  //   
  bool IncludeDefault() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of the default option to the width list
  //
  // Parameters:
  //   b [in] If true then default option is added to the width list; otherwise is is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeDefault()
  //   
  bool SetIncludeDefault( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding plot width options to combo box.
  //
  // Returns:
  //   Returns true if "No Print" option should be added to the end of the width list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::SetIncludeNoPrint()
  //   
  bool IncludeNoPrint() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of the "No Print" option to the end of the width list
  //
  // Parameters:
  //   b [in] If true then "No Print" option is added to the end of the width list; otherwise is is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeNoPrint()
  //   
  bool SetIncludeNoPrint( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding plot width options to combo box.
  //
  // Returns:
  //   Returns true if "Hairline" option should be added to the width list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::SetIncludeNoPrint()
  //   
  bool IncludeHairline() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of the "Hairline" option to the width list
  //
  // Parameters:
  //   b [in] If true then "Hairline" option is added to the width list; otherwise is is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlPrintWidthComboBox::IncludeNoPrint()
  //   
  bool SetIncludeHairline( bool b);
private:
  bool m_include_by_layer;
  bool m_include_default;
  bool m_include_by_parent;
  bool m_include_no_print;
  bool m_include_hairline;
protected:
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchange();
  afx_msg void OnEditKillFocus();
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlFontComboBox
//
//   Display a combo box containing a list of currently installed fonts.  This list may be filtered
//   to only display TrueType  and/or non-TrueType fonts.  See CRhinoUiComboBox for details.
//
// See Also:
//   class CRhinoUiComboBox()
//   class CRhinoUiOptionsListCtrlComboBox()
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlFontComboBox : public CRhinoUiOptionsListCtrlComboBox
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  // See Also:
  //   class CRhinoUiComboBox()
  //   class CRhinoUiOptionsListCtrlComboBox()
  //
  CRhinoUiOptionsListCtrlFontComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
  // See Also:
  //   class CRhinoUiComboBox()
  //   class CRhinoUiOptionsListCtrlComboBox()
  //
  virtual ~CRhinoUiOptionsListCtrlFontComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override to display font name and sample.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override
  //
  //   Overridden to fill combo box with list of fonts installed on this computer.
  //
  // Returns:
  //   See CRhinoUiOptionsListCtrlComboBox::PopulateComboBox()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::PopulateComboBox()
  //   CRhinoUiComboBox::FillUsingInstalledFonts()
  //   
  bool PopulateComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override, calls base class.
  //
  //   Overridden to save currently selected LOGFONT
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::OnItemComboBoxSelChanged()
  //   
  void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   Get description of the last selected font.
  //
  // Parameters:
  //   lf[out] LOGFONT structure to receive font information.
  //
  // Returns:
  //   Returns true if lf is filled in.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFontComboBox::SetLogFont()
  //   CRhinoUiOptionsListCtrlFontComboBox::OnItemComboBoxSelChanged()
  //   
  bool GetLogfont( LOGFONT& lf) const;
  //   
  // Description:
  //   Set default font description.  This will be displayed when the item is
  //   not selected and become the default value for the font combo box the first
  //   time is is displayed.
  //
  // Parameters:
  //   lf[in] Description of default font
  //
  // Returns:
  //   Returns true if font descriptions was successfully saved.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlFontComboBox::GetLogFont()
  //   CRhinoUiOptionsListCtrlFontComboBox::OnItemComboBoxSelChanged()
  //   
  bool SetLogfont( const LOGFONT& lf);
private:
  LOGFONT m_logfont;

protected:
  DECLARE_MESSAGE_MAP()
public:
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlColorComboBox
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlColorComboBox : public CRhinoUiOptionsListCtrlComboBox
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlColorComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0, CRhinoUiComboBoxItemData::class_type ct = CRhinoUiComboBoxItemData::ct_color);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlColorComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox virtual override
  //
  //   Create CRhinoUiOptionsListCtrlComboBox combo box for selecting colors.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if combo box was created; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::BeginEditItem()
  //   
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlItem virtual override
  //
  //   This method draws color swatch and color name in value column of options list control.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlItem::OnDrawItem()
  //   
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox virtual override
  //
  //   Overridden to fill combo box with standard color options including "By Layer" if the
  //   IncludeByLayer() flag is set.
  //
  // Returns:
  //   Returns true if combo box is filled with one or more standard colors; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeByLayer()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeByLayer()
  //   
  bool PopulateComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override, calls base class.
  //
  //   Overridden to save currently selected color
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::OnItemComboBoxSelChanged()
  //   
  void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding color options to combo box.
  //
  // Returns:
  //   Returns true if "By Layer" option should be added to color list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeByLayer()
  //   
  bool IncludeByLayer() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of "By Layer" color option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then "By Layer" option is added to combo box; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeByLayer()
  //   
  bool SetIncludeByLayer( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding color options to combo box.
  //
  // Returns:
  //   Returns true if "By Parent" option should be added to color list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeByParent()
  //   
  bool IncludeByParent() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of "By Parent" color option to combo
  //   box.
  //
  // Parameters:
  //   b [in] If truen then "By Parent" option is added to combo box; otherwise it is not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeByLayer()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeByParent()
  //   
  bool SetIncludeByParent( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding color options to combo box.
  //
  // Returns:
  //   Returns true if "By Display" option should be added to color list; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeByDisplay()
  //   
  bool IncludeByDisplay() const;

  // Description:
  //   Include a "By Display" color option in the combo box.
  // Parameters:
  //   include [in] If true then "By Display" option is added to combo box;
  //           otherwise it is not.
  // Returns:
  //   Returns the previous value.
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeByDisplay()
  bool SetIncludeByDisplay(bool include);

  // Description:
  //   By default, the combo box will not include alpha settings when showing
  //   the custom color dialog. This function allows the alpha controls to be
  //   shown.
  // Parameters:
  //   on [in] turn the feature on
  // Returns:
  //  previous value for using alpha
  bool SetUseAlpha(bool on);

  // Description:
  //  Allow for alpha controls when showing the color dialog for selecting a
  //  custom color
  bool UseAlpha() const;
private:
  unsigned char m_include_flags = 0;
  bool m_use_alpha = false;
  bool m_reserved;
  bool m_delay_destroy = false;
protected:
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchange();
  afx_msg void OnKillFocus(CWnd* pNewWnd);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlLayerComboBox
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlLayerComboBox : public CRhinoUiOptionsListCtrlColorComboBox
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlLayerComboBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlLayerComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox virtual override
  //
  //   Call base class to reate CRhinoUiOptionsListCtrlComboBox combo box for layer and
  //   set default to current layer.
  //
  // Parameters:
  //   See CRhinoUiOptionsListCtrlItem::BeginEditItem()
  //
  // Returns:
  //   Returns true if combo box was created; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::BeginEditItem()
  //   
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox virtual override
  //
  //   Overridden to fill combo box with current Rhino document layer list.
  //
  // Returns:
  //   Returns true if combo box is filled with one or more layer name; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeByLayer()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeByLayer()
  //   
  bool PopulateComboBox();
  //   
  // Description:
  //   CRhinoUiOptionsListCtrlComboBox override, calls base class.
  //
  //   Overridden to save currently selected layer index.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlComboBox::OnItemComboBoxSelChanged()
  //   
  void OnItemComboBoxSelChanged();
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding layers to the combo box.
  //
  // Returns:
  //   Returns true if deleted layer names should be added to the combo box; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeDeletedLayers()
  //   
  bool IncludeDeletedLayers() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of deleted layers to the combo box.
  //
  // Parameters:
  //   b [in] If truen then deleted layer names are added to combo box; otherwise they are not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeDeletedLayers()
  //   
  bool SetIncludeDeletedLayers( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding layers to the combo box.
  //
  // Returns:
  //   Returns true if reference layer names should be added to the combo box; otherwise false.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::SetIncludeReferenceLayers()
  //   
  bool IncludeReferenceLayers() const;
  //   
  // Description:
  //   Call this method to allow or disallow adding of reference layers to the combo box.
  //
  // Parameters:
  //   b [in] If truen then reference layer names are added to combo box; otherwise they are not.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeReferenceLayers()
  //   
  bool SetIncludeReferenceLayers( bool b);
  //   
  // Description:
  //   This method is called by PopulateComboBox() when adding layers to the combo box.  If
  //   this is set to true then CRhinoLayerTable::GetSortedList is called otherwise all layers
  //   are included.
  //
  // Returns:
  //   Returns true if sorted layer list should be used; otherwise complete layer list will be
  //   used.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeDeletedLayers()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeReferenceLayers()
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   
  bool UseSortedList() const;
  //   
  // Description:
  //   Call this method to control what layerers get added to the combo box.  If this is set
  //   to true then the sorted and culled list displayed in the layer dialog is used.  If this
  //   set set to false then the entire layer list is used.  SetIncludeReferenceLayers() and
  //   SetIncludeDeletedLayers() are used in either case to filter the display of those layer
  //   types.
  //
  // Parameters:
  //   b [in] If true then the layer dialog list is used.
  //
  // Returns:
  //   Returns the previous value.
  //
  // See Also:
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeDeletedLayers()
  //   CRhinoUiOptionsListCtrlColorComboBox::IncludeReferenceLayers()
  //   CRhinoUiOptionsListCtrlColorComboBox::PopulateComboBox()
  //   
  bool SetUseSortedList( bool b);
private:
  bool m_use_sorted_list;
  bool m_include_deleted_layers;
  bool m_include_reference_layers;
protected:
  DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlRadioButtonColumn
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlRadioButtonColumnBtn
{
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlRadioButtonColumnBtn( class CRhinoUiOptionsListCtrlRadioButtonColumn* = NULL, LPCTSTR = NULL);
  CRhinoUiOptionsListCtrlRadioButtonColumnBtn( const CRhinoUiOptionsListCtrlRadioButtonColumnBtn&);
  //   
  // Description:
  //   operator= Called by ON_ClassArray when growing array
  //
  const CRhinoUiOptionsListCtrlRadioButtonColumnBtn& operator=( const CRhinoUiOptionsListCtrlRadioButtonColumnBtn&);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlRadioButtonColumnBtn();
  //   
  // Description:
  //   Radio button label which will appear in the value column.
  //
  // Returns:
  //   Returns the text to display to the right of this button.
  //
  LPCTSTR ButtonLabel() const;
  //   
  // Description:
  //   Return CRhinoUiOptionsListCtrlRadioButtonColumn zero based index for this button item.
  //
  // Returns:
  //   Return CRhinoUiOptionsListCtrlRadioButtonColumn zero based index for this button item.
  //
  // See Also:
  //   class CRhinoUiOptionsListCtrlRadioButtonColumn
  //   CRhinoUiOptionsListCtrlRadioButtonColumn::GetButton()
  //   
  int ButtonIndex() const;
  //   
  // Description:
  //   Display rectangle for this button item.  Used when creating or drawing button.
  //
  // Returns:
  //   Returns display rectangle for this button item.
  //
  // See Also:
  //   class CRhinoUiOptionsListCtrlRadioButtonColumn
  //   CRhinoUiOptionsListCtrlRadioButtonColumn::AddRadioButton()
  //   
  CRect ButtonRect() const;
  //   
  // Description:
  //   Check button state to see if it is currently pressed.
  //
  // Returns:
  //   Returns true if mouse is currently down over this item.  Used when drawing item
  //   during selection process.
  //
  bool ButtonIsPressed() const;
  //   
  // Description:
  //   This method is used to get the CRhinoUiOptionsListCtrlRadioButtonColumn this button
  //   belongs to.
  //
  // Returns:
  //   Returns the CRhinoUiOptionsListCtrlRadioButtonColumn this button belongs to or NULL
  //   if the button has not yet been added to a column
  //
  // See Also:
  //   class CRhinoUiOptionsListCtrlRadioButtonColumn
  //   CRhinoUiOptionsListCtrlRadioButtonColumn::AddRadioButton()
  //   
  CRhinoUiOptionsListCtrlRadioButtonColumn* ButtonColumn() const;
private:
  friend class CRhinoUiOptionsListCtrlRadioButtonColumn;
  CString m_sLabel;
  int m_iIndex;
  CRect m_r;
  bool m_button_pressed;
  class CRhinoUiOptionsListCtrlRadioButtonColumn* m_button_column;
};

class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlRadioButtonColumn : public CWnd, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlRadioButtonColumn)
public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsLabel [in] Label for this item, appears in left column of options list
  //   lpsText [in] Text associated with this item, appears in right column of options list
  //
  CRhinoUiOptionsListCtrlRadioButtonColumn( LPCTSTR lpsLabel = NULL, LPCTSTR lpsText = NULL, int iIndentLevel = 0);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiOptionsListCtrlRadioButtonColumn();
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
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
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
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
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
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
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
  int ItemHeight() const;
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
  int AddRadioButton( LPCTSTR lps);
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
  virtual void OnRadioButtonSelect();
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
  int SelectedButton() const;
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
  int SetSelectedButton( int iButtonIndex);
  int ButtonCount() const;
  const CRhinoUiOptionsListCtrlRadioButtonColumnBtn* GetButton( int iButtonIndex) const;
  const CRhinoUiOptionsListCtrlRadioButtonColumnBtn* GetSelectedButton() const;
private:
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
  void DrawButton( CRhinoUiOptionsListCtrl* pListCtrl, CDC& dc, const CString& s, CRect rButton, bool hot, bool checked, bool pressed) const; 

  enum
  {
    first_button_id = 200
  };
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
  int ButtonIndexFromPoint( CPoint point) const;

  bool SetRadioButtonColMouseCapture();
  bool ReleaseRadioButtonColMouseCapture();

  bool m_bRadioButtonColumnMouseCaptured;
  int m_iSelectedIndex;
  int m_iHotIndex;
  int m_iMouseDownIndex;
  ON_ClassArray<CRhinoUiOptionsListCtrlRadioButtonColumnBtn>m_button_list;
  CRect m_rDeflate;
public:
  DECLARE_MESSAGE_MAP()
  afx_msg void OnDestroy();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlPushButton
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlPushButton : public CRhinoUiButton, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlPushButton)
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
  CRhinoUiOptionsListCtrlPushButton( LPCTSTR lpsLabel = NULL, LPCTSTR lpsButtonCaption = NULL, int iIndentLevel = 0);
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
  virtual ~CRhinoUiOptionsListCtrlPushButton();
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
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
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
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
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
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
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
  virtual bool OnItemButtonClicked();
private:
  friend class CRhinoUiButton;
  bool m_bOnItemButtonClicked;

public:
  DECLARE_MESSAGE_MAP()
  afx_msg void OnBnClicked();
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlColorButton
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlColorButton : public CRhinoUiOptionsListCtrlPushButton
{
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
  CRhinoUiOptionsListCtrlColorButton( LPCTSTR lpsLabel = NULL, COLORREF cr = CRhinoUiPaintManager::unset_color, int iIndentLevel = 0);
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
  virtual ~CRhinoUiOptionsListCtrlColorButton();
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
  virtual bool OnItemButtonClicked();

  COLORREF Color() const;
  COLORREF SetColor( COLORREF cr);

public:
  DECLARE_MESSAGE_MAP()

protected:
  COLORREF m_item_color;
};
//-------------------------------------------------------------------------------------------------
//
// CRhinoUiOptionsListCtrlCheckBox
//
class RHINO_SDK_CLASS CRhinoUiOptionsListCtrlCheckBox : public CWnd, public CRhinoUiOptionsListCtrlItem
{
	DECLARE_DYNCREATE(CRhinoUiOptionsListCtrlCheckBox)
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
  CRhinoUiOptionsListCtrlCheckBox( LPCTSTR lpsLabel = NULL, LPCTSTR lpsButtonCaption = NULL, int iIndentLevel = 0);
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
  virtual ~CRhinoUiOptionsListCtrlCheckBox();
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
  void OnDrawItem( CDC& dc, const CRect& rItem, const CRect& rDeflate) const;
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
  bool BeginEditItem( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit, begin_edit_item_reason eir);
  void SetText( LPCTSTR lps);
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
  void MoveItemWindow( CRhinoUiOptionsListCtrl& list_ctrl, const CRect rEdit);
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
  virtual bool OnItemButtonClicked();
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
  //
  // BST_UNCHECKED
  // BST_CHECKED
  // BST_INDETERMINATE
  //
  int SetCheck( int bst_state);
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
  int GetCheck() const;
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
  bool SetDrawCheckBox( bool b);
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
  bool DrawCheckBox() const;
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
  bool SetCreateTriStateCeckBox( bool b);
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
  bool CreateTriStateCeckBox() const;
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
  void SetCheckText( LPCTSTR lps);
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
  LPCTSTR GetCheckText() const;
private:
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
  void Redraw( CDC* pDC = NULL);

  bool SetCheckBoxMouseCapture();
  bool ReleaseCheckBoxMouseCapture();
private:

  bool m_bCheckBoxMouseCaptured;
  int m_iCheckState;
  bool m_bLButtonDown;
  bool m_bHot;
  bool m_bOnItemButtonClicked;
  bool m_bCreateTriStateCeckBox;
  bool m_bDrawCheckBox;
  CString m_sCheckText;
  CRect m_rDeflate;

public:
  DECLARE_MESSAGE_MAP()
  afx_msg void OnBnClicked();
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnDestroy();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
