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


// CRhinoUiPopUpListWnd frame

class CRhinoUiPopuUpListBox;

class RHINO_SDK_CLASS CRhinoUiPopUpListWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CRhinoUiPopUpListWnd)

public:
	CRhinoUiPopUpListWnd();           // protected constructor used by dynamic creation
	virtual ~CRhinoUiPopUpListWnd();

public:
  // Step 1: Create list box
  virtual BOOL32 CreatePopUpListBox( const RECT& rect, CWnd* pParentWnd, UINT nListBoxStyles = LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT);
  // Step 2: Add list box items
  // Step 3: Adjust list box size and display
  bool DisplayPopUpListBox(int nCmdShow = SW_SHOW);
  // Delete this pointer when windows is destroyed
  bool AutoDelete();
  void SetAutoDelete( bool b = true);
  // Destroy window when list box looses focus.  If this returns
  // TRUE then HideWindowOnKillFocus() never gets called
  bool DestroyOnKillFocus();
  void SetDestroyOnKillFocus( bool b = true);
  // Hide window when list box looses focus.  Only gets called
  // if DestroyOnKillFocus() returns false
  bool HideOnKillFocus();
  void SetHideOnKillFocus( bool b = true);
  // Lock top of window and adust bottom to fit list item if TRUE
  // otherwise lock bottom and adjust top.
  bool LockWindowTop();
  void SetLockWindowTop( bool lock = true);
  // Adjust list to fit widest item.
  bool AdjustWidth();
  void SetAdjustWidth( bool adjust = true); // default value is true
  // Destroy window when on list item box click.
  bool DestroyOnClickOnListItem();
  void SetDestroyOnClickOnListItem( bool b = true);
  // Hide window when list box looses focus.
  bool HideOnClickOnListItem();
  void SetHideOnClickOnListItem( bool b = true);
  CRhinoUiPopuUpListBox& ListBox();
  // Maximum client area for list box
  void SetMaximumClientArea(CRect& rc);
  
  void SizeToContent();

  // Description:
  //  Called by list box OnLButtonUp() method if mouse comes up on
  //  the same item it went down on and in the same column.  See 
  //  OnLBOnLButtonUpInColumn( ...)
  // Parameters:
  //  item_index [in] - list index for item clicked on
  //
  virtual void ListItemClickedOn( int item_index, int x_col);
  // Description:
  //  Called by list box OnKeyDown() method.
  // Parameters:
  //  item_index [in] - currently selected list item
  //  nChar [in] see CWnd::OnKeyDown(...)
  //  nRepCnt [in] see CWnd::OnKeyDown(...)
  //  nFlags [in] see CWnd::OnKeyDown(...)
  //
  virtual void OnLBKeyDown(int item_index, UINT nChar, UINT nRepCnt, UINT nFlags);
  // Description:
  //  Called by list box OnLButtonUp() method if mouse comes up on
  //  the same item it went down on.  The base class assumes there
  //  is only one column and returns true.  Override this function 
  //  if you are owner drawing this control and want to do column
  //  validation.  See ListItemClickedOn()
  // Parameters:
  //  x_down [in] - x point coordinate where mouse went down
  //  x_up [in] - x point coordinage where mouse came up
  //
  virtual BOOL32 OnLBOnLButtonUpInColumn( int item, int x_down, int x_up);
  // Description:
  //  Called by list box OnChar( ...) method
  // Parameters:
  //  See CWnd::OnChar()
  virtual void OnLBChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  // Description:
  //  Override if you want to provide your onwn owner draw list
  //  box or want to do your own custom messaging and return
  //  your derived object
  virtual CRhinoUiPopuUpListBox* NewPopUpListBox();
  // Description:
  //  ONLY override if you overrider NewPopUpListBox().  This method will
  //  be called to destroy your list box objcet.  The pointer returned by
  //  NewPopUpListBox() is stored in CRhinoUiPopUpListWnd::mp_list_box.
  virtual void DeletePopUpListBox();

private:
  friend class CRhinoUiPopuUpListBox;

  void OnLBKillFocus(CWnd* pNewWnd);
  void CloseWindow( int item_index = -1, int x_col = -1);
  bool InCloseWindow();

  char m_lock_wnd_top;
  UINT m_list_box_style;
  char m_auto_delete;
  char m_destroy_on_kill_focus;
  char m_hide_on_kill_focus;
  char m_destroy_on_click_on_list_item;
  char m_hide_on_click_on_list_item;
  char m_in_close_window;
  CRect m_rcMaxClientRect;
  bool m_bUseMaxClientRect;
protected:
  CRhinoUiPopuUpListBox* mp_list_box;

protected:
	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnPaint();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
