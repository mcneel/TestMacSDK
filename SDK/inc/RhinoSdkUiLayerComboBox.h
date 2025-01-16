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

// CRhinoUiLayerComboBox

class RHINO_SDK_CLASS CRhinoUiLayerComboBox : public CComboBox
{
  DECLARE_DYNAMIC(CRhinoUiLayerComboBox)

public:
  CRhinoUiLayerComboBox();
  virtual ~CRhinoUiLayerComboBox();

  /*
  Description:
    Populate combo box with layer names and colors
  Parameters:
    doc [in] - document containing layer table
    bShowReference [in] - show reference layers
    set_current [in] - layer index of item to set as current
  Returns:
    Number of layers added to list
  */
  int LoadLayers(CRhinoDoc* doc, bool bShowReference = true, int set_current = -1);

  /*
  Description:
    Get layer index associated with a specific combo box item
  Parameters:
    list_index [in] - list item to check
  Returns:
    Layer index or -1 on error
  */
  int LayerIndexFromListIndex(int list_index);

  /*
  Description:
    Get combo box index from layer index
  Parameters:
    layer_index [in] - layer index to scan for
  Returns:
    combo box index or -1 if not found in list
  */
  int ListIndexFromLayerIndex(int layer_index);

  /*
  Description:
    Get layer index associated with current combo box selection
  Returns:
    Layer index or -1 if nothing selected
  */
  int SelectedLayerIndex();

  CRhinoDoc* Document();
  const CRhinoLayer* Layer(int list_index);
  int AddLayerToList(const CRhinoLayer& layer);
  BOOL32 RemoveLayerFromList(int layer_index);

public:
  virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
  void ShowPopDownWnd(HWND hWnd, bool bShow);
  void DestroyPopDownWnd();

  virtual void PreSubclassWindow();
  DECLARE_MESSAGE_MAP()

protected:
  CRhinoUiPopDownLayerWnd* m_pWndPopDown;
  bool m_bShowReference;

protected:
  HPALETTE m_hPalette;
};


