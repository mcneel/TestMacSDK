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
//
// CRhinoUiAutocompleteEdit
//
// Description:
//    Edit control with a pop-up autocomplete list.
//
class RHINO_SDK_CLASS CRhinoUiAutocompleteEdit : public CRhinoUiEdit
{
  DECLARE_DYNAMIC(CRhinoUiAutocompleteEdit)
public:
  CRhinoUiAutocompleteEdit();
  virtual ~CRhinoUiAutocompleteEdit();

  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  void CreateAutocomplete();
  void DestroyAutocomplete();

  virtual const ON_ClassArray<ON_wString>* GetSortedStringList() = 0;
protected:
  void DoBackSpace( UINT nRepCnt, UINT nFlags);
  virtual void OnVkReturn();
  virtual void OnVkTab();
  virtual void OnVkEscape();
  virtual void DoDataExchange(CDataExchange* pDX);

  DECLARE_MESSAGE_MAP()
protected:
  afx_msg void OnDestroy();
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);


private:
  void AutocompleteWndSetFocus();
  friend class CRhUiAutocompleteWnd;
  class CRhUiAutocompleteWnd* m_pAutoCompleteWnd;
};
