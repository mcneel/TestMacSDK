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


// CRhinoUiSliderCtrl
class CRhinoUiEditInt;
class CRhinoUiEdit;

class RHINO_SDK_CLASS CRhinoUiSliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(CRhinoUiSliderCtrl)

public:
	CRhinoUiSliderCtrl();
	virtual ~CRhinoUiSliderCtrl();

public:
  LPCTSTR Text();

  void SetText( const char* lpsText);
  void SetText( const wchar_t* lpsText);

  void LinkTo( CRhinoUiEdit* edit_ctrl, double slider_to_edit_scale = 0.0);
  void LinkTo( CRhinoUiEditInt* edit_ctrl, double slider_to_edit_scale = 0.0);

  void PaintText( CDC& dc);

protected:
  void _SetText( LPCTSTR lpsText);
  void _LinkTo( CEdit* edit_ctrl, double slider_to_edit_scale);
  void _SetLink( CEdit* edit_ctrl, CRhinoUiSliderCtrl* slider_ctrl);
  void _SetMinMax( CEdit* edit_ctrl, int min_val, int max_val);

private:
  friend class CRhinoUiEdit;
  friend class CRhinoUiEditInt;
  friend class CRhinoUiEditReal;

  CString m_sText;
  CEdit* m_linked_to;
  double m_link_scale;

  void SynchLinkedTo();

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg LRESULT OnSetText( WPARAM, LPARAM);
  afx_msg LRESULT OnGetText( WPARAM, LPARAM);
};
