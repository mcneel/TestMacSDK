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


// CRhinoUiTabCtrlPage dialog

class RHINO_SDK_CLASS CRhinoUiTabCtrlPage : public CRhinoUiDialog
{
	DECLARE_DYNAMIC(CRhinoUiTabCtrlPage)

public:
  CRhinoUiTabCtrlPage( LPCTSTR lpszTemplateName = NULL);
	CRhinoUiTabCtrlPage( UINT nIDTemplate);

  virtual ~CRhinoUiTabCtrlPage();

public:
  //   
  // Description:
  //    This method is called by the CRhinoUiTabPageCtrl when the user chooses 
  //    either the OK or Apply Now button, immediately after CRhinoUiTabPageCtrl
  //    calls OnKillActive. Override this method to implement additional 
  //    behavior specific to the currently active page when the user dismisses 
  //    the CRhinoUiTabPageCtrl.
  //
  //    The default implementation of this method marks the page as clean 
  //    to reflect that the data was updated in the OnKillActive method.
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //    CRhinoUiTabCtrlPage::OnKillActive()
  //    CRhinoUiTabCtrlPage::OnApply()
  //
  virtual void OnOK();
  //   
  // Description:
  //    This method is called by CRhinoUiTabPageCtrl when the user chooses the 
  //    OK or the Apply Now button. When CRhinoUiTabPageCtrl calls this method, 
  //    changes made on all tab control pages in the CRhinoUiTabPageCtrl are 
  //    accepted, the tab control page retains focus, and OnApply returns true.
  //    Before OnApply can be called by the framework, you must have called 
  //    CRhinoUiTabCtrlPage::SetModified and set its parameter to true. 
  //    This will activate the Apply Now button as soon as the user makes a 
  //    change on the tab control page.
  //
  //    Override this method to specify what action your program takes when 
  //    the user clicks the Apply Now button. When overriding, the method 
  //    should return true to accept changes and false to prevent changes 
  //    from taking effect.
  //
  //    The default implementation of OnApply calls CRhinoUiTabCtrlPage::OnOK().
  //
  // Returns:
  //    Return true if the changes are accepted; otherwise, returns false. 
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //    CRhinoUiTabCtrlPage::OnOK()
  //
  virtual bool OnApply();
  //   
  // Description:
  //    This method is called by the CRhinoUiTabPageCtrl when the page is chosen 
  //    by the user and becomes the active page. Override this method to 
  //    perform tasks when a page is activated. Your override of this method 
  //    should call the default version before any other processing is done.
  //
  // Returns:
  //    Return true if the page was successfully set active; 
  //    otherwise, returns false. 
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //
  virtual bool OnSetActive();
  //   
  // Description:
  //    This method is called by CRhinoUiTabPageCtrl when the page is no longer 
  //    the active page. Override this method to perform special data 
  //    validation tasks.  The default implementation of this method copies 
  //    settings from the controls in the property page to the member variables
  //    of the property page. If the data was not updated successfully due to 
  //    a dialog data validation (DDV) error, the page retains focus.
  //
  // Returns:
  //    Returns true if data was updated successfully; otherwise, returns false.
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //
  virtual bool OnKillActive();

public:
  //   
  // Description:
  //    Call this method to get the CRhinoUiTabPageCtrl that this page belongs to.  
  //    This method will do nothing until the page has been added to the tab 
  //    control by calling CRhinoUiTabPageCtrl::AddPage.
  //
  // Returns:
  //    Returns pointer to CRhinoUiTabPageCtrl that owns this page.  This will 
  //    return NULL until CRhinoUiTabPageCtrl::AddPage() has been called for 
  //    this page.
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //    CRhinoUiTabPageCtrl::AddPage()
  //
  class CRhinoUiTabPageCtrl* GetTabCtrl() const;
  //   
  // Description:
  //    Call this method to enable or disable the Apply Now button, based 
  //    on whether the settings in the tab control page should be applied 
  //    to the appropriate external object.  The framework keeps track of 
  //    which pages are dirty; that is, tab control pages for which you have 
  //    called SetModified( true). The Apply Now button is always enabled if 
  //    you call SetModified( true) for one of the pages. The Apply Now button
  //    is disabled when you call SetModified( false) for one of the pages, 
  //    but only if none of the other pages is dirty.
  //
  // Parameters:
  //    bChanged [in] If this is true then page is dirty otherwise it is clean.
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //    CRhinoUiTabCtrlPage::IsModified()
  //
  void SetModified( bool bChanged = true); 
  //   
  // Description:
  //    Current "dirty" state of this page, see SetModified() for details.
  //
  //
  // Returns:
  //    Returns true if this page has been modified; otherwise, returns false.
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //    CRhinoUiTabCtrlPage::SetModified()
  //
  bool IsModified() const;
  //   
  // Description:
  //    Call this method to forward a message to each page in the property sheet.
  //    If a page returns a nonzero value, the property sheet does not send 
  //    the message to subsequent pages. 
  //
  // Returns:
  //    Returns the nonzero value from a page in the tab control, or zero if 
  //    all pages return a value of zero.
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //    CRhinoUiTabPageCtrl::AddPage()
  //
  LRESULT QuerySiblings( WPARAM wParam, LPARAM lParam); 

protected:
  void InitializeBackgroundBrush();

private:
  friend class CRhinoUiTabCtrl;
  friend class CRhinoUiTabPageCtrl;
  bool m_bCreatedByTabCtrl;
protected:
  bool m_bIsModified;
  bool m_bFirstSetActive;
  bool m_bAutoDelete;
  CBrush m_bkGrndBrush;
  COLORREF m_crBkGrndBrush;
  CRhinoUiTabPageCtrl* m_pTC;

protected:
  void EndDialog( int nEndID); // called for error scenarios
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
  //   
  // Description:
  //    This method is called by the framework when the Cancel button is 
  //    selected. Override this method to perform Cancel button actions. 
  //    The default negates any changes that have been made.  DO NOT call
  //    EndDialog( IDCANCEL) or base class OnCancel().
  //
  // See Also:
  //    CRhinoUiTabPageCtrl
  //
  virtual void OnCancel();
private:
  virtual INT_PTR DoModal();
protected:
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
protected:
  virtual void PreSubclassWindow();
};
