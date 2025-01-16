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


// CRhinoUiDockBarDialog dialog

class RHINO_SDK_CLASS CRhinoUiDockBarDialog : public CRhinoUiDialog
{
	DECLARE_DYNCREATE(CRhinoUiDockBarDialog)

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
  // Returns:
  //   
  //
  // See Also:
  //   CRhinoUiDialog
  //   CDialog
  //   CRhinoUiDockBarHtmlDialog
  //   
	CRhinoUiDockBarDialog( LPCTSTR lpszTemplateName = NULL, CWnd* pParent = NULL);
	CRhinoUiDockBarDialog( UINT nIDTemplate, CWnd* pParent = NULL);
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiDockBarDialog();
  //   
  // Description:
  //   Get Rhino dock bar that this dialog is embedded in.
  //
  // Returns:
  //   Rhino dock bar that this dialog is embedded in.
  //
  // See Also:
  //   CRhinoUiDockBarHtmlDialog
  //   
  CRhinoUiDockBar* GetRhinoDockBar() const;
  //
  // CRhinoUiDialog overrides
  //   
  // Description:
  //   See CRhinoUiDialog::ForwardLMouseDownMessagesToParent() for details
  //
  // See Also:
  //   CRhinoUiDialog::ForwardLMouseDownMessagesToParent()
  //   
  bool ForwardLMouseDownMessagesToParent( CPoint point);
  //   
  // Description:
  //   See CRhinoUiDialog::ForwardLMouseDblClickMessagesToParent() for details
  //
  // See Also:
  //   CRhinoUiDialog::ForwardLMouseDblClickMessagesToParent()
  //   
 bool ForwardLMouseDblClickMessagesToParent( CPoint point);
private:
  friend class CRhinoUiDockBar;
  CRhinoUiDockBar* m_pCB;
  bool m_bCreatedByRhinoDockBar;

  //virtual
  INT_PTR DoModal();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
  virtual void OnOK();
  virtual void OnCancel();
public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
