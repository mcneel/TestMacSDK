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


// CRhinoUiExpandableTabPage

class RHINO_SDK_CLASS CRhinoUiExpandableTabPage : public CRhinoUiDialog
{
  DECLARE_DYNCREATE( CRhinoUiExpandableTabPage)
public:
  //   
  // Description:
  //   Constructors
  //
  // Parameters:
  //    lpszTemplateName [in] Contains a null-terminated string that is the name of a dialog-box
  //                          template resource. 
  //    nIDTemplate [in] Contains the identifier of a dialog-box template resource. 
  //    pParentWnd [in] Pointer to the parent or owner window object of type CWnd to which the dialog
  //                    object belongs. If it is NULL, the dialog object parent window is set to the
  //                    main application window. 
  //
  // See Also:
  //   class CRhinoUiDialog
  //   class CRhinoUiExpandableTab
  CRhinoUiExpandableTabPage( LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
  CRhinoUiExpandableTabPage( UINT nIDTemplate, CWnd* pParentWnd = NULL);
  //   
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiExpandableTabPage();
  //   
  // Description:
  //   Get tab control that owns this tab page.
  //
  // Returns:
  //   Tab control that this page is inserted in.
  //
  // See Also:
  //   class CRhinoUiExpandableTabCtrl
  //   
  CRhinoUiExpandableTabCtrl* GetTabCtrl() const;
  //   
  // Description:
  //   Event notification which is called when a tabs hidden state changes.
  //
  // Parameters:
  //   iTabIndex [in] Tab control index for this item
  //   bHide [in] If true then tab is hidden otherwise tab is no longer hidden.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::HideTab()
  //   
  virtual void OnHideTab( int iTabIndex, bool bHide);
  //   
  // Description:
  //   Event notification which is called when a tabs has been expanded or contracted.
  //
  // Parameters:
  //   iTabIndex [in] Tab control index for this item
  //   bExpand [in] If true then tab has been expanded otherwise tab has been contracted.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::ExpandItem()
  //   
  virtual void OnExpandTab( int iTabIndex, bool bExpand);
  //   
  // Description:
  //  Get expandable tab item record associated with this page. This pointer can be
  //  changed and should not be saved.
  //
  // Returns:
  //   Temporary pointer to expandable tab item record that controls this dialog.
  //
  // See Also:
  //   class CRhinoUiExpandableTabCtrlItem
  //   
  const CRhinoUiExpandableTabCtrlItem* GetExpandableTabCtrlItem() const;
  //   
  // Description:
  //   Get expandable tab control item label
  //
  // Returns:
  //   Name displayed on expandable control tab.
  //
  LPCTSTR PageName() const;
  bool SetUseSpecialColors( bool b);
  bool UseSpecialColors() const;

private:
  friend class CRhinoUiExpandableTabCtrl;

  void InitThemeControl();

  bool m_bCreatedByTabCtrl;
  CRhinoUiThemeDraw* m_pThemeDraw;
  bool m_bSpecialColors;
  CRhinoUiExpandableTabCtrl* m_pTC;

protected:
  DECLARE_MESSAGE_MAP()
private:
  virtual void OnOK();
  virtual void OnCancel();
  virtual INT_PTR DoModal();
protected:
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual BOOL32 OnInitDialog();
  afx_msg void OnSysColorChange();
};
