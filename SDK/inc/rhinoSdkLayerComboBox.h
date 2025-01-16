//
// Copyright (c) 1993-2017 Robert McNeel & Associates. All rights reserved.
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

#if defined (RHINO_SDK_MFC)

/////////////////////////////////////////////////////////////////////////////
// CRhinoLayerComboBox window
class RHINO_SDK_CLASS CRhinoLayerComboBox : public CComboBox
{
public:
	CRhinoLayerComboBox();

// Operations
public:
  /*
  Description:
    Populate combo box with layer names and colors
  Parameters:
    doc [in] - document containing layer table
    set_current [in] - Layer index of item to set as current
  Returns:
    Number of layers added to list
  */
  int LoadLayers( CRhinoDoc* doc, BOOL32 show_referenced = true, int set_current = -1);
  /*
  Description:
    Add layer to combo box, called by LoadLayers
  Parameters:
    layer [in] - layer to add
  Returns:
    List index for new item or LB_ERR if not added
  Remarks:
    If a layer is added to the layer table after LoadLayers is called use
    this method to expand the list
  */
  int AddLayerToList( const CRhinoLayer& layer);
  /*
  Description:
    Remove layer from combo box.
  Parameters:
    layer_index [in] - layer to remove
  Returns:
    TRUE if layer was removed from list
    FALSE if layer was not in list
  */
  BOOL32 RemoveLayerFromList( int layer_index);
  /*
  Description:
    Get layer index associated with a specific combo box item
  Parameters:
    list_index [in] - list item to check
  Returns:
    Layer index or -1 on error
  */
  int LayerIndexFromListIndex( int list_index);
  /*
  Description:
    Get combo box index from layer index
  Parameters:
    layer_index [in] - layer index to scan for
  Returns:
    combo box index or -1 if not found in list
  */
  int ListIndexFromLayerIndex( int layer_index);
  /*
  Description:
    Get layer index associated with current combo box selection
  Returns:
    Layer index or -1 if nothing selected
  */
  int SelectedLayerIndex();

  CRhinoDoc* Document();
  const CRhinoLayer* Layer( int list_index);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRhinoLayerComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRhinoLayerComboBox();

	// Generated message map functions
protected:
	HPALETTE m_hPalette;

	//{{AFX_MSG(CRhinoLayerComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif

