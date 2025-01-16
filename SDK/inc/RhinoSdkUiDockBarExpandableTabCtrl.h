//
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

// CRhinoUiDockBarClientExpandableTabPage

class RHINO_SDK_CLASS CRhinoUiDockBarClientExpandableTabPage : public CRhinoUiExpandableTabPage
{
	DECLARE_DYNAMIC(CRhinoUiDockBarClientExpandableTabPage)

public:
  // Description:
  //   Constructors
  // Parameters:
  //    lpszTemplateName [in] Contains a null-terminated string that is the name of a dialog-box
  //                          template resource. 
  //    nIDTemplate [in] Contains the identifier of a dialog-box template resource. 
  //    pParentWnd [in] Pointer to the parent or owner window object of type CWnd to which the dialog
  //                    object belongs. If it is NULL, the dialog object parent window is set to the
  //                    main application window. 
	CRhinoUiDockBarClientExpandableTabPage( LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
	CRhinoUiDockBarClientExpandableTabPage( UINT nIDTemplate, CWnd* pParentWnd = NULL);

  virtual ~CRhinoUiDockBarClientExpandableTabPage();

  // Description:
  //   This method is overridden to allow users to drag dock bar by clicking on client area
  //   of the dialog.
  //
  //   See CRhinoUiDialog::ForwardLMouseDownMessagesToParent() for additional details.
  // See Also:
  //   CRhinoUiDialog::ForwardLMouseDownMessagesToParent()
  bool ForwardLMouseDownMessagesToParent( CPoint point) override;

  // Description:
  //   This method is overridden to allow users to toggle dock bar docked state by double
  //   clicking on the client area of the dialog.
  //
  //   See CRhinoUiDialog::ForwardLMouseDblClickMessagesToParent() for additional details.
  // See Also:
  //   CRhinoUiDialog::ForwardLMouseDblClickMessagesToParent()
  bool ForwardLMouseDblClickMessagesToParent( CPoint point) override;
protected:
	DECLARE_MESSAGE_MAP()
};


class RHINO_SDK_CLASS CRhinoUiDockBarClientExpandableTabCtrl : public CRhinoUiExpandableTabCtrl
{
	DECLARE_DYNAMIC(CRhinoUiDockBarClientExpandableTabCtrl)
public:
	CRhinoUiDockBarClientExpandableTabCtrl() = default;
	virtual ~CRhinoUiDockBarClientExpandableTabCtrl() = default;

  // Description:
  //   Create expandable tab control which may be embedded in a window or dialog which
  //   is in turn embedded in a dock bar.
  // Parameters:
  //   pCB [in] Dock bar this control or its parent is embedded in.
  //   pParent [in] Window to embed this control in.
  // Returns:
  //   Returns true if the tab control is successfully created or false on error.
  bool CreateExpandableTabCtrl( CRhinoUiDockBar* pCB, CWnd* pParent);

  // Description:
  //   Get dock bar this control is embedded in.
  CRhinoUiDockBar* GetRhinoDockBar();
protected:
  CRhinoUiDockBar* m_pCB = nullptr;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

// CRhinoUiDockBarExpandableTabCtrl

class RHINO_SDK_CLASS CRhinoUiDockBarExpandableTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CRhinoUiDockBarExpandableTabCtrl)
public:
	CRhinoUiDockBarExpandableTabCtrl();
	virtual ~CRhinoUiDockBarExpandableTabCtrl();

  // Description:
  //   Create window to hold the tab control.  This window will size itself to fit
  //   the entire client area of the dock bar it is embedded in.  If window is created
  //   successfully then a tab control will be created and embedded in this window.
  // Parameters:
  //   pParent [in] Dock bar to embed window in.
  // Returns:
  //   Returns true if this window and the tab control associated with it are created
  //   successfully.
  bool CreateExpandableTabCtrl( CRhinoUiDockBar* pParent);

  // Description:
  //   Get tab control embedded in this window
  CRhinoUiDockBarClientExpandableTabCtrl& GetExpandableTabCtrl();

  // Description:
  //   Margins on left, right, top and bottom of this child tab control.
  //      Size::cx is left and right margins
  //      Size::cy is top and bottom margins
  // Returns:
  //   Margin values
  CSize GetMargins() const;

  // Description:
  //   Set margins on left, right, top and bottom of this child tab control.
  //      Size::cx is left and right margins
  //      Size::cy is top and bottom margins
  // Parameters:
  //   size [in] New margin values
  void SetMargins( CSize size);

  // Description:
  //   Get pointer to dock bar this window is embedded in.
  CRhinoUiDockBar* GetRhinoDockBar();
private:
  CRhinoUiDockBarClientExpandableTabCtrl m_tab_ctrl;
protected:
  CRhinoUiDockBar* m_pCB = nullptr;
  CSize m_margins;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
