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


// CRhinoUiGridListCtrl

class RHINO_SDK_CLASS CRhinoUiGridListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CRhinoUiGridListCtrl)

public:
	CRhinoUiGridListCtrl();
	virtual ~CRhinoUiGridListCtrl();

public:
  // For extended selection
  virtual void OnLButtonDownWithControlSelectItem( int nRow, int nColumn);
  // A return value of true means continue processing the mouse down event, false means bail
  virtual bool OnLButtonDownOnItem( bool bShiftButtonIsDown, int nRow, int nColumn);
  virtual void OnLButtonUpDragSelect( int nRow, int nColumn);

  void ClearListSelection();
  void SelectItemIndex( int nItem, int nMode = LVIS_SELECTED);
  bool IsMouseTracking(){ return m_bIsTracking;}
  void KillTracking();
  int ColumnFromPoint( CPoint pt);

  int  CurrentEditItem(){ return m_nEdit;}
  bool SubclassHeaderCtrl(){ return m_bSubclassHeader;};
  // Call this if you want to provide your own header control otherwise
  // this classes OnPreSubclass method will connect to it's own CRhinoUiGridListHeaderCtrl object.
  void SubclassHeaderCtrl( bool b){ m_bSubclassHeader = b;}

  CRhinoUiGridListEdit *GetEditCtrl();
	// Calll this function from OnInitDialog prior to creating columns
	void Setup( BOOL32 bFullRowSelect, BOOL32 bEditOnClick = TRUE, BOOL32 bListChange = TRUE);
	// This function is called after editing is completed, if you don't
	// override it then the list item text is updated.
	virtual BOOL32 DoneEditing( long nItem, int nSubItem, LPCTSTR lpszText);
	// This function is called when an item is clicked (if EditOnClick( TRUE))
	// or double clicked.  You should call EditGridItem or ListGridItem as needed.
	virtual BOOL32 ModifyGridItem( long nItem, int nSubItem); 

  bool AllowColumnResizing(){ return m_bAllowColumnResizing;}
  void AllowColumnResizing( bool b){ m_bAllowColumnResizing = b;}
  void DisableColumnResizing( int nInColumn);
  bool IsColumnResizable( int nColumn);

  bool AllowColumnSwapping(){ return m_bAllowColumnSwapping;}
  void AllowColumnSwapping( bool b);
  void DisableColumnSwapping( int nInColumn);
  bool IsColumnSwapable( int nColumn);

  // Set the minimum column width for the specified column
  void MinimumColumnWidth( int col, int cx);
  // Get the minimum column width for the specified column
  int  MinimumColumnWidth( int col);

  void DestroyEditCtrl();

	// Display edit box in row and column specified. DoneEditing is called if 
	// text has changed.
	BOOL32       EditGridItem( long nItem, int nSubItem);
	// Display combo box in row and column specified.  ComboBox will contain
	// lstItems and will default to nSel.  DoneEditing is called when user selects
	// something. (see NotifyOnListChange)
	CComboBox* ListGridItem( long nItem, int nSubItem, CStringList &lstItems, int nSel);

	// Call done editing only if different list item selected
	BOOL32 NotifyOnListChange(){ return m_bListChange; }
	void NotifyOnListChange( BOOL32 b){ m_bListChange = b; }
	// Determines if items will be edited on a single or double click.  Single is
	// default.
	BOOL32 EditOnClick(){ return m_bEditOnClick; }
	void EditOnClick( BOOL32 b){ m_bEditOnClick = b; }
	// return the number of items currently selected in list
	long getSelectedCount();
  // Convert original column index to current index.  Used to keep column indexes
  // in synch when allowing column re-arranging.
  int  RuntimeListColumn( int nOriginalColumn);
  // Make sure nothing is selected
  void ClearAllSelectedItems();
  // Set items selected state and set focus to it.  if !bClearSelection then select item
	void SelectItem( int nItem, bool bClearSelection = false);
  // Get range of contiguous selection, return true if there are items selected and 
  // the items are contiguous otherwise false
  bool SelectionContiguous( int &nFirst, int &nLast);

  bool IsMultiSelect();

private:
  int HitTest( CPoint point);
  void ShiftSelect( int nStart, int nEnd);
  void DragOffList(UINT nFlags, CPoint point);
  void SelectRange( int nStart, int nEnd);

private:
  friend class CRhinoUiGridListEdit;

  enum Timers
  {
    mouse_down_timer = 1,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_timers = 0xFFFFFFFF
  };
  enum ScrollSpeeds
  {
		scroll_speed_off    = 0,
		slow_scroll_speed,
		medium_scroll_speed,
		fast_scroll_speed,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_speeds = 0xFFFFFFFF

  };

	int  m_cx;                 // Item offset
	BOOL32 m_nEdit;              // Column current edit item is in
	BOOL32 m_bEditOnClick;       // Edit on a single click
	BOOL32 m_bListChange;        // Call DoneEditing only if ListBox changes
	                           // default is TRUE
	CRhinoUiGridListEdit m_LVEdit; // Edit control used to SubClass
	                           // default CListCtrl edit control.
  bool       m_bAllowColumnResizing;     // If true then columns may be resized, defaults to true.
  CUIntArray m_DisableColumnSizingList; // List of columns to disable resizing in, call DisableColumnResizing( int nInColumn)
  bool       m_bAllowColumnSwapping;     // If true then columns may be dragged and dropped to rearrange, defaults to false.
  CUIntArray m_DisableColumnSwapping  ; // List of columns to disable swapping in.
  bool       m_bSubclassHeader;
  CUIntArray m_MinColumnWidth;
  CRhinoUiGridListHeaderCtrl m_RhinoUiGridListHeaderCtrl;

  // For extended selection
  ScrollSpeeds m_ScrollSpeed;
  int          m_nStartIndex;
  int          m_nEndIndex;
  int          m_nRoot;
  bool         m_bIsTracking;
  CPoint       m_ptTimer;
  bool         m_bKillTracking;
  bool         m_bEscapeEdit;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
  virtual BOOL32 OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  virtual void PreSubclassWindow();
};
