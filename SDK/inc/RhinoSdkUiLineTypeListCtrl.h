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

// CRhinoUiLinetypeListCtrlInfo

class RHINO_SDK_CLASS CRhinoUiLinetypeListCtrlInfo
{
public:
  CRhinoUiLinetypeListCtrlInfo();
  CRhinoUiLinetypeListCtrlInfo( const ON_Linetype& linetype );
  ~CRhinoUiLinetypeListCtrlInfo();
public:
  ON_Linetype m_linetype;
  bool m_added;    // linetype was added or loaded
  bool m_deleted;  // linetype was deleted
  bool m_modified; // linetype was modified
};

// CRhinoUiLinetypeListCtrl

class RHINO_SDK_CLASS CRhinoUiLinetypeListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC( CRhinoUiLinetypeListCtrl )

public:
	CRhinoUiLinetypeListCtrl();
	virtual ~CRhinoUiLinetypeListCtrl();

  enum columns
  {
    name_column = 0,
    appearance_column,
    column_count,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_columns = 0xFFFFFFFF
  };

  /*
  Description:
    Initializes the control.
  Remarks:
    Call this member from CDialog::OnInitDialog().
  */
  void Initialize();

  /*
  Description:
    Verifies that multiple selection is enabled.
  Returns:
    TRUE  - if multiple selection is enabled.
    FALSE - if multiple selection is disabled.
  See Also:
    SetEnableMultipleSelect
  */
  bool EnableMultipleSelect() const;

  /*
  Description:
    Enables or disables multiple selection.
  Parameters:
    bEnable - [in] TRUE to enable multiple select, otherwise FALSE.
  See Also:
    EnableMultipleSelect
  */
  void SetEnableMultipleSelect( bool bEnable = true );

  /*
  Description:
    Verifies that label editing is enabled.
  Returns:
    TRUE  - if label editing is enabled.
    FALSE - if label editing is disabled.
  See Also:
    SetEnableEditLabel
  */
  bool EnableEditLabel() const;

  /*
  Description:
    Enables or disables label editing.
  Parameters:
    bEnable - [in] TRUE to enable label editing, otherwise FALSE.
  See Also:
    EnableEditLabel
  */
  void SetEnableEditLabel( bool bEnable = true );

  /*
  Description:
    Inserts a linetype into the control.
  Parameters:
    item     - [in] index of the item to insert.
    linetype - [in] the linetype to insert.
  Returns:
    >=0 - index of the inserted item.
    -1  - linetype with the same name already exist.
  Remarks:
    Use this member to add linetypes that already
    exist in the document's linetype table.
  */
  int InsertLinetype( int item, const ON_Linetype& linetype );

  /*
  Description:
    Appends a linetype to the end of the control.
  Parameters:
    linetype - [in] the linetype to append.
  Returns:
    >=0 - index of the inserted item.
    -1  - linetype with the same name already exist.
  Remarks:
    Use this member to add linetypes that already
    exist in the document's linetype table.
  */
  int AppendLinetype( const ON_Linetype& linetype );

  /*
  Description:
    Modifies a linetype in the control.
  Parameters:
    item     - [in] index of the item to modify.
    linetype - [in] the modified linetype.
  Returns:
    TRUE  - if successful.
    FALSE - if item is out of range or if the linetype name is invalid.
  Remarks:
    This does not modify linetypes as it exists
    in the document's linetype table.
  */
  bool ModifyLinetype( int item, const ON_Linetype& linetype );
  
  /*
  Description:
    Modifies a linetype in the control.
  Parameters:
    linetype_name - [in] name of the linetype to modify.
    linetype      - [in] the modified linetype.
  Returns:
    TRUE  - if successful.
    FALSE - if item is out of range or if the linetype name is invalid.
  Remarks:
    This does not modify linetypes as it exists
    in the document's linetype table.
  */
  bool ModifyLinetype( const wchar_t* linetype_name, const ON_Linetype& linetype );

  /*
  Description:
    Deletes a linetype from the control.
  Parameters:
    item - [in] index of the item to delete.
  Returns:
    TRUE  - if successful. 
    FALSE - if item is out of range.
  Remarks:
    This does not delete linetypes as it exists
    in the document's linetype table.
  */
  bool DeleteLinetype( int item );

  /*
  Description:
    Deletes a linetype from the control.
  Parameters:
    linetype_name - [in] name of the linetype to delete.
  Returns:
    TRUE  - if successful. 
    FALSE - if item is out of range.
  Remarks:
    This does not delete linetypes as it exists
    in the document's linetype table.
  */
  bool DeleteLinetype( const wchar_t* linetype_name );

  /*
  Description:
    Inserts a new linetype into the control.
  Parameters:
    item     - [in] index of the item to insert.
    linetype - [in] the linetype to insert.
  Returns:
    >=0 - index of the inserted item
    -1  - linetype with the same name already exist
  Remarks:
    This does not add a new linetype to the document's
    linetype table. Use this member to add linetypes that do
    not already exist in the document's linetype table.
  */
  int InsertNewLinetype( int item, const ON_Linetype& linetype );

  /*
  Description:
    Appends a new linetype to the end of the control.
  Parameters:
    linetype - [in] the linetype to insert.
  Returns:
    >=0 - index of the inserted item.
    -1  - linetype with the same name already exist.
  Remarks:
    This does not add a new linetype to the document's
    linetype taable. Use this member to add linetypes that do
    not already exist in the document's linetype table.
  */
  int AppendNewLinetype( const ON_Linetype& linetype );

  /*
  Description:
    Returns the selected linetype.
  Parameters:
    linetype - [out] the linetype to retrieve.
  Returns:
    TRUE  - if an item is selected. 
    FALSE - if an item is not selected.
  Remarks:
    Use this member when the control is set
    for "single selection."
  */
  bool GetSelectedLinetype( ON_Linetype& linetype );

  /*
  Description:
    Returns the selected linetypes.
  Parameters:
    linetypes - [out] the linetypes to retrieve.
  Returns:
    >0 - the number of selected items. 
    0  - if no items are selected.
  Remarks:
    Use this member when the control is set
    for "multiple selection."
  */
  ON_DEPRECATED_MSG("use version with ON_ObjectArray parameter")
  int GetSelectedLinetypes( ON_ClassArray<ON_Linetype>& linetypes );

  int GetSelectedLinetypes( ON_ObjectArray<ON_Linetype>& linetypes );

  /*
  Description:
    Returns a linetype that is displayed in the control.
  Parameters:
    item     - [in] index of the item to retrieve.
    linetype - [out] the linetype to retrieve.
  Returns:
    TRUE  - if successful. 
    FALSE - if item is out of range.
  */
  bool GetLinetype( int item, ON_Linetype& linetype );

  /*
  Description:
    Returns a linetype that is displayed in the control.
  Parameters:
    linetype_name - [in] name of the linetype to retrieve.
    linetype      - [out] the linetypes to retrieve.
  Returns:
    TRUE  - if successful. 
    FALSE - if item is out of range.
  */
  bool GetLinetype( const wchar_t* linetype_name, ON_Linetype& linetype );
  
  /*
  Description:
    Returns a list of the linetype added while the control
    was displayed. These linetype definitions do not exist
    in the document's linetype table. Linetypes that were add
    and then later deleted are ignored.
  Parameters:
    linetypes - [out] the linetypes to retrieve.
  Returns:
    >0 - the number of linetypes returned. 
    0  - if no items are returned.
  */
  ON_DEPRECATED_MSG("use version with ON_ObjectArray parameter")
  int GetAddedLinetypes( ON_ClassArray<ON_Linetype>& linetypes );
  int GetAddedLinetypes( ON_ObjectArray<ON_Linetype>& linetypes );

  /*
  Description:
    Returns of list of the linetypes that were deleted while
    the control was displayed. These linetype definitions exist
    in the document's linetype table. New linetypes that were added
    and then later deleted are ignored.
  Parameters:
    linetypes - [out] the linetype to retrieve.
  Returns:
    >0 - the number of linetypes returned. 
    0  - if no items are returned.
  */
  ON_DEPRECATED_MSG("use version with ON_ObjectArray parameter")
  int GetDeletedLinetypes( ON_ClassArray<ON_Linetype>& linetypes );
  int GetDeletedLinetypes( ON_ObjectArray<ON_Linetype>& linetypes );

  /*
  Description:
    Returns of list of the linetypes that were modified while
    the control was displayed. These linetype definitions exist
    in the document's linetype table. New linetypes that were added
    and then later modified are ignored.
  Parameters:
    linetypes - [out] the linetype to retrieve.
  Returns:
    >0 - the number of linetypes returned. 
    0  - if no items are returned.
  */
  ON_DEPRECATED_MSG("use version with ON_ObjectArray parameter")
  int GetModifiedLinetypes( ON_ClassArray<ON_Linetype>& linetypes );
  int GetModifiedLinetypes( ON_ObjectArray<ON_Linetype>& linetypes );

  /*
  Description:
    Returns of list of all linetypes that are maintained by
    the control, whether they are displayed or not.
  Parameters:
    linetypes - [out] the linetype to retrieve.
  Returns:
    >0 - the number of linetypes returned. 
    0  - if no items are returned.
  */
  ON_DEPRECATED_MSG("use version with ON_ObjectArray parameter")
  int GetAllLinetypes( ON_ClassArray<ON_Linetype>& linetypes );
  int GetAllLinetypes( ON_ObjectArray<ON_Linetype>& linetypes );

  /*
  Description:
    Selects a linetype in the control by its name.
  Parameters:
    linetype - [in] the linetype to select.
  */
  void SelectLinetype( const wchar_t* linetype_name );

  /*
  Description:
    Selects a linetype in the control by its linetype index.
  Parameters:
    linetype_index - [in] the index of the linetype to select.
  */
  void SelectLinetypeIndex( int linetype_index );

  /*
  Description:
    Finds a linetype that is displayed in the control.
  Parameters:
    linetype - [in] the linetype to find.
  Returns:
    >=0 - index of the item.
    -1  - linetype name cannot be found.
  */
  int FindLinetype( const wchar_t* linetype_name );

  /*
  Description:
    Returns the current sort method.
  Returns:
    TRUE  - sort ascending.
    FALSE - sort descending.
  */
  bool SortAscending() const;

  /*
  Description:
    Sets the current sort method.
  Parameters:
    bAscending - [in] TRUE for ascending, FALSE for descending.
  */
  void SetSortAscending( bool bAscending = true );

  /*
  Description:
    Automatically sizes the name column.
  */
  void SizeNameColumn();

  /*
  Description:
    Automatically sizes the appearance column.
  */
  void SizeAppearanceColumn();

public:
  afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL32 OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnHeaderEndTrack(NMHDR* pNMHDR, LRESULT* pResult);

protected:
  virtual void PreSubclassWindow();
  virtual BOOL32 PreCreateWindow( CREATESTRUCT& cs );
	DECLARE_MESSAGE_MAP()

private:
  void DrawLinetype( NMLVCUSTOMDRAW* pLVCD );

private:
  ON_ClassArray<CRhinoUiLinetypeListCtrlInfo> m_infolist;
  bool m_bSortAscending;
};


