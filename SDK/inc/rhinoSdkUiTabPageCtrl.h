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


// CRhinoUiTabPageCtrl

class RHINO_SDK_CLASS CRhinoUiTabPageCtrl : public CRhinoUiTabCtrl
{
	DECLARE_DYNAMIC(CRhinoUiTabPageCtrl)

public:
	CRhinoUiTabPageCtrl();
	virtual ~CRhinoUiTabPageCtrl();

public:
  //   
  // Description:
  //    Call this method from the CRhinoUiTabPageCtrl parent dialog when the OK button is pressed.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //
  // Returns:
  //    Returns true if the all modified pages OnApply() and all created pages OnOk() methods
  //    returned true; otherwise, it is false.
  //
  bool OnOK();
  //   
  // Description:
  //    Call this method from the CRhinoUiTabPageCtrl parent dialog when the Apply button is pressed.
  //
  // Returns:
  //    Returns true if the all modified pages OnApply() methods returned true; otherwise, it is false.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //
  bool OnApply();
  //   
  // Description:
  //    This method adds a CRhinoUiTabCtrlPage object to the tab control. Add pages to the tab control in the
  //    left-to-right order you want them to appear.
  //
  // Parameters:
  //    dlg [in] Page to add to the tab control
  //    lpsPageName [in] Tab text, if this is NULL or an empty string the page will not be added.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //
  void AddPage( CRhinoUiTabCtrlPage& dlg, LPCTSTR lpsPageName);
  //   
  // Description:
  //    This method adds a CRhinoUiTabCtrlPage object to the tab control. Add pages to the tab control in the
  //    left-to-right order you want them to appear.
  //
  // Parameters:
  //    pDlg [in] Page to add to the tab control, this pointer will be deleted by the tab control.
  //    lpsPageName [in] Tab text, if this is NULL or an empty string the page will not be added.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //
  void AddPage( CRhinoUiTabCtrlPage* pDlg, LPCTSTR lpsPageName);
  //   
  // Description:
  //    This method changes the active page. For example, use SetActivePage if a user’s action on one page should
  //    cause another page to become the active page.
  //
  // Parameters:
  //    iPage [in] Specifies the index of the page to set. It must be between zero and one less than the number
  //               of pages in the tab control, inclusive.
  //
  // Returns:
  //    Returns true if the page is activated successfully; otherwise, it is false.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetPageCount()
  //    CRhinoUiTabPageCtrl::SetActivePage( CRhinoUiTabCtrlPage* pPage)
  //
  bool SetActivePage( int iPage);
  //   
  // Description:
  //    This method changes the active page. For example, use SetActivePage if a user’s action on one page should
  //    cause another page to become the active page.
  //
  // Parameters:
  //    pPage [in] Points to the page to set in the tab control. It cannot be NULL
  //
  // Returns:
  //    Returns true if the page is activated successfully; otherwise, it is false.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetPageCount()
  //    CRhinoUiTabPageCtrl::SetActivePage( int iPage)
  //
  bool SetActivePage( CRhinoUiTabCtrlPage* pPage);
  //   
  // Description:
  //    This method removes a page from the tab control and destroys the associated window. The CRhinoUiTabCtrlPage
  //    pointer itself is not deleted until the owner of the CRhinoUiTabPageCtrl window is closed.
  //
  // Parameters:
  //    iPage [in] Specifies the index of the page to be removed. Must be between zero and one less than the number of
  //    pages in the tab control, inclusive.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetPageCount()
  //
  void RemovePage( int iPage); 
  //   
  // Description:
  //    This method removes a page from the tab control and destroys the associated window. The CRhinoUiTabCtrlPage
  //    pointer itself is not deleted until the owner of the CRhinoUiTabPageCtrl window is closed.
  //
  // Parameters:
  //    pPage [in] Points to the page to be removed from the tab control. Cannot be NULL. 
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetPageCount()
  //
  void RemovePage( CRhinoUiTabCtrlPage* pPage); 
  //   
  // Description:
  //    This method gets the index number of the active page from the tab control.
  //
  // Returns:
  //    Returns the index number of the active page or -1 if no active page has been identified.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetPageCount()
  //
  int GetActiveIndex() const;
  //   
  // Description:
  //    This method retrieves the active page from the tab control. Use this method to perform some action on
  //    the active page.
  //
  // Returns:
  //    Returns the pointer to the active page or NULL if no active page has been identified.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetActiveIndex()
  //
  CRhinoUiTabCtrlPage* GetActivePage() const;
  //   
  // Description:
  //    This method retrieves the index number of the specified page in the tab control.
  //
  // Parameters:
  //    pPage [in] Points to the page to be removed from the tab control. Cannot be NULL. 
  //
  // Returns:
  //    Returns the index number of a page or -1 if page is not found in the tab control.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //
  int GetPageIndex( CRhinoUiTabCtrlPage* pPage) const;
  //   
  // Description:
  //    This method determines the number of pages currently in the tab control.
  //
  // Returns:
  //    Returns the number of pages in the tab control.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //
  int GetPageCount() const;
  //   
  // Description:
  //    This method retrieves a pointer to the specified page in this tab control.
  //
  // Parameters:
  //    nPage [in] Specifies the index of the desired page, starting at zero. Must be between
  //    zero and one less than the number of pages in the property sheet, inclusive.
  //
  // Returns:
  //    The pointer to the page corresponding to the nPage parameter or NULL if nPage is invalid.
  //
  // See Also:
  //    CRhinoUiTabCtrlPage
  //    CRhinoUiTabPageCtrl::AddPage()
  //    CRhinoUiTabPageCtrl::GetPageCount()
  //
  CRhinoUiTabCtrlPage* GetPage( int nPage) const;

protected:
  bool CreatePage( int iPageIndex);
  void RemoveAll();
  bool CallOnApply();
  bool CallOnOK();

protected:
  int m_active_page;
  ON_SimpleArray<CRhinoUiTabCtrlPage*>m_page_list;
  ON_SimpleArray<CRhinoUiTabCtrlPage*>m_remove_delete_list;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnDestroy();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
  virtual void PreSubclassWindow();
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
};


