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


// CRhinoUiGridListCombo

#ifndef IDC_RHINOUI_GRIDLIST_COMBO
#define IDC_RHINOUI_GRIDLIST_COMBO (WM_USER * 2)
#endif

class RHINO_SDK_CLASS CRhinoUiGridListCombo : public CComboBox
{
	DECLARE_DYNAMIC(CRhinoUiGridListCombo)

public:
	//                   Edit Row    Edit Column        List strings        Default   Notify only if selection changes
	//                       |           |                  |                  |         |
	//                       V           V                  V                  V         V
	CRhinoUiGridListCombo( long iItem, int iSubItem, CStringList *plstItems, int nSel , BOOL32 bChanged);
	virtual ~CRhinoUiGridListCombo();

private:
	long 	m_iItem;       // Edit row
	int 	m_iSubItem;    // Edit column
	int 	m_nSel;        // Selected item
	BOOL32    m_bChanged;    // Notify only if item changed
	BOOL	m_bESC;        // EScape key hit flag
	CString m_strOriginal; // Default string
	CStringList m_lstItems;// List strings

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnNcDestroy();
  afx_msg void OnCbnCloseup();
};


