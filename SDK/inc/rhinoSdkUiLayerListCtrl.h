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

// CRhinoUiLayerListCtrlItem

class RHINO_SDK_CLASS CRhinoUiLayerListCtrlItem
{
public:
  CRhinoUiLayerListCtrlItem();
  CRhinoUiLayerListCtrlItem(const CRhinoUiLayerListCtrlItem& src);
  virtual CRhinoUiLayerListCtrlItem& operator=(const CRhinoUiLayerListCtrlItem& src);
  virtual ~CRhinoUiLayerListCtrlItem();

public:
  int m_layer_index;
  ON_wString m_layer_name;
  int m_indent;
  bool m_bChildren;
  bool m_bExpanded;
};


// CRhinoUiLayerListCtrl
class RHINO_SDK_CLASS CRhinoUiLayerListCtrl : public CListCtrl
{
  DECLARE_DYNAMIC(CRhinoUiLayerListCtrl)

public:
  CRhinoUiLayerListCtrl();
  virtual ~CRhinoUiLayerListCtrl();

  enum columns
  {
    name_column = 0,
    color_column,
    current_column,
    visible_column,
    locked_column,
    column_count
  };

  enum state_images
  {
    no_state_image = 0,
    collapsed_image,
    expanded_image
  };

  enum layer_images
  {
    no_layer_image = 0,
    current_image,
    visible_image,
    invisible_image,
    locked_image,
    unlocked_image,
    cached_visible_image,
    cached_locked_image,
    image_count
  };

  /*
  Description:
    Verifies that layer editing is enabled.
  See Also:
    SetLayerEditingEnabled
  */
  bool LayerEditingEnabled() const;

  /*
  Description:
    Enables or disables layer editing.
  Parameters:
    bEnable [in] - enable layer editing. The default behavior
                   of the control is not to allow layer editing.
  Remarks:
    If this member is not called or is called with a value of false,
    then the control will display a color and name column only. This
    mode is useful if only want the user to select a layer. If this
    member is called with a value of true, then this control will display
    visible, locked, color, current, and name columns. When items in
    these columns are clicked, the layer in which they represent will
    be manipulate accordingly.
  See Also:
    Initialize
    LoadLayers
    LayerEditingEnabled
  */
  void SetLayerEditingEnabled(bool bEnable = true);

  /*
  Description:
    Initializes the control.
  Remarks:
    Call this member from CDialog::OnInitDialog() after any call to
    SetLayerEditingEnabled and before calling LoadLayers().
  See Also:
    LoadLayers
  */
  void Initialize();

  /*
  Description:
    Load the layer table of the document into the list control
  Parameters:
    doc [in] - document containing the layer table
    show_referenced [in] - show reference layers
    set_current [in] - layer index of item to set as current
  Returns:
    Number of layers added to list
  See Also:
    Initialize
  */
  int LoadLayers(CRhinoDoc* doc, bool bShowReference = true, int set_current = -1);

  /*
  Description:
    Verifies that multiple selection is enabled.
  Returns:
    TRUE  - if multiple selection is enabled.
    FALSE - if multiple selection is disabled.
  See Also:
    SetEnableMultipleSelect
  */
  bool EnableMultipleSelect() const;

  /*
  Description:
    Enables or disables multiple selection.
  Parameters:
    bEnable - [in] TRUE to enable multiple select, otherwise FALSE.
  See Also:
    EnableMultipleSelect
  */
  void SetEnableMultipleSelect(bool bEnable);

  /*
  Description:
    Returns the selected layer.
  Returns:
    >= 0  The layer index, as found in CRhinoDoc::m_layer_table,
          of the selected layer.
    -1    If no item is selected
  See Also:
    SetSelectedLayer
  */
  int SelectedLayer() const;

  /*
  Description:
    Sets the selected layer. Note, if the specified layer index
    is a child of a layer who is not expanded, the layer will
    not be selected.
  Parameters:
    layer_index - [in] The layer index, as found in CRhinoDoc::m_layer_table,
                       of the layer to select.
  See Also:
    SelectedLayer
  */
  void SetSelectedLayer(int layer_index);

  /*
  Description:
    Returns a list of selected layers.
  Parameters:
    layer_list [out] The array to hold the list of selected layers,
    as as found in CRhinoDoc::m_layer_table.
  Returns:
    > 0  The number of items added to layer_list.
    0    If no items were selected
  See Also:
    SetSelectedLayers
  */
  int SelectedLayers(ON_SimpleArray<int>& layer_list) const;

  /*
  Description:
    Sets the selected layers. Note, if any of the specified layer indices
    is a child of a layer who is not expanded, the layer will not be selected.
  Parameters:
    layer_list [in] The array layer indices, as as found in CRhinoDoc::m_layer_table.
                    to select.
  See Also:
    SelectedLayers
  */
  void SetSelectedLayers(const ON_SimpleArray<int>& layer_list);

  /*
  Description:
    Deletes all items in the list control, and purges local item data.
  */
  BOOL32 DeleteAllItems();

  /*
  Description:
    Ensures that column headers are sized appropriately.
    Call from CDialog::OnSize().
  */
  void SizeColumns();

  /*
  Description
    Override this virtual function if you want to be notified
    when an item was expanded.
  */
  virtual void OnItemExpanded(int nItem);

  /*
  Description
    Override this virtual function if you want to be notified
    when an item was collapsed.
  */
  virtual void OnItemCollapsed(int nItem);

  /*
  Description
    Returns the size, in pixels, of the long layer name
    in the control.
  */
  int GetMaxStringPixels();

  /*
  Description
    Returns the size, in pixels, of the deepest layer indent
    in the control.
  */
  int GetMaxIndentPixels();

public:
  afx_msg void OnPaint();
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

protected:
  virtual void PreSubclassWindow();
  DECLARE_MESSAGE_MAP()

  void AppendLayer(CRhinoDoc* doc, const CRhinoLayer* layer, bool bExpanded);
  void AppendItem(CRhinoDoc* doc, const CRhinoLayer* layer, bool bExpanded);
  int InsertItem(int nItem, int nPos, const CRhinoUiLayerListCtrlItem& item);

  const CRhinoUiLayerListCtrlItem* LayerFromItem(int nItem) const;
  int GetItemIndent(int nItem) const;

  int Expand(int nPos, int nItem, bool bRecurse = false);
  int DoExpand(int nItem, bool bRecursive, int& nCounterInsert);
  void Collapse(int nPos, int nItem);

  void OnClickNameColumn(int nItem);
  void OnClickCurrentColumn(int nItem);
  void OnClickVisibleColumn(int nItem);
  void OnClickLockedColumn(int nItem);
  void OnClickColorColumn(int nItem);
  int LayerIndexFromItem(int nItem) const;

protected:
  unsigned int m_rhino_doc_sn;
  CRhinoUiImageList m_ilTree;
  CRhinoUiImageList m_ilEmpty;
  CRhinoUiImageList m_ilImage;
  ON_ClassArray<CRhinoUiLayerListCtrlItem> m_layers;
  bool m_bShowReference;
  bool m_bDrawHorzizontalGridLines;
  bool m_bLayerEditingEnabled;
  bool m_bKeepFocus;
};


// CRhinoUiPopDownLayerListCtrl

class RHINO_SDK_CLASS CRhinoUiPopDownLayerListCtrl : public CRhinoUiLayerListCtrl
{
  DECLARE_DYNAMIC(CRhinoUiPopDownLayerListCtrl)

public:
  CRhinoUiPopDownLayerListCtrl(CComboBox& cb);
  virtual ~CRhinoUiPopDownLayerListCtrl();

  CComboBox& ComboBox() { return m_ComboBox; }

  void OnItemExpanded(int nItem);
  void OnItemCollapsed(int nItem);

  bool LayerHasChildren(int nItem);

  bool CalcSizeAndPosition(CRect& rect, bool& bPopDown);
  class CRhinoUiComboBox* RhinoUiComboBox();

protected:
  void HidePopUpWindow();

public:
  afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

  // virtual overrides
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
  friend class CRhinoUiPopDownLayerWnd;
  CComboBox& m_ComboBox;
  class CRhinoUiPopDownLayerWnd* m_PopDownLayerWnd;

protected:
  void ComboBoxSelectLayer(int layer_index);
  DECLARE_MESSAGE_MAP()
};


// CRhinoUiPopDownLayerWnd

class RHINO_SDK_CLASS CRhinoUiPopDownLayerWnd : public CWnd
{
  DECLARE_DYNAMIC(CRhinoUiPopDownLayerWnd)

public:
  CRhinoUiPopDownLayerWnd(CComboBox& cb);
  virtual ~CRhinoUiPopDownLayerWnd();

  bool CreatePopDownWnd();
  bool ShowPopDownWnd(HWND hWnd, bool bShow, bool bShowReference);

  CRhinoUiPopDownLayerListCtrl& PopDownList() { return m_PopDownList; }
  CComboBox& ComboBox() { return m_ComboBox; }

  void HidePopUpWindow();
  class CRhinoUiComboBox* RhinoUiComboBox();

public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnShowWindow(BOOL32 bShow, UINT nStatus);

protected:
  friend class CRhinoUiPopDownLayerListCtrl;
  void SynchPopDownListWithCombo(bool bShow, bool bShowReference);

  DECLARE_MESSAGE_MAP()

protected:
  static CString m_sWndClass;
  CRhinoUiPopDownLayerListCtrl m_PopDownList;
  CComboBox& m_ComboBox;
};

UINT RhinoUiLayerListCtrlColumnClickMessageId();
