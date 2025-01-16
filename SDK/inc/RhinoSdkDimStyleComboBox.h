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
// CRhinoDimStyleComboBox window
class RHINO_SDK_CLASS CRhinoDimStyleComboBox : public CComboBox
{
public:
	CRhinoDimStyleComboBox();

public:
  /*
  Description:
    Populate combo box with dimension styles
  Parameters:
    doc [in] - document containing dimension style table
    set_current [in] - Style index of item to set as current
  Returns:
    Number of styles added to list
  */
  int LoadStyles( CRhinoDoc* doc, BOOL32 show_referenced = true, int set_current = -1);
  /*
  Description:
    Add style to combo box, called by LoadStyles
  Parameters:
    style [in] - style to add
  Returns:
    List index for new item or LB_ERR if not added
  Remarks:
    If a style is added to the style table after LoadStyles is called use
    this method to expand the list
  */
  int AddStyleToList( const CRhinoDimStyle& style);
  /*
  Description:
    Remove style from combo box.
  Parameters:
    style_index [in] - style to remove
  Returns:
    TRUE if style was removed from list
    FALSE if style was not in list
  */
  BOOL32 RemoveStyleFromList( int style_index);
  /*
  Description:
    Get style index associated with a specific combo box item
  Parameters:
    list_index [in] - list item to check
  Returns:
    Style index or -1 on error
  */
  int StyleIndexFromListIndex( int list_index);
  /*
  Description:
    Get combo box index from style index
  Parameters:
    style_index [in] - style index to scan for
  Returns:
    combo box index or -1 if not found in list
  */
  int ListIndexFromStyleIndex( int style_index);
  /*
  Description:
    Get style index associated with current combo box selection
  Returns:
    Style index or -1 if nothing selected
  */
  int SelectedStyleIndex();

  CRhinoDoc* Document();
  const CRhinoDimStyle* DimStyle( int list_index);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRhinoDimStyleComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRhinoDimStyleComboBox();

#if defined (ON_RUNTIME_APPLE)
  int m_selIndex;     // Used on Mac to fake selection index
#endif
  
protected:
  CRhinoDoc* m_doc;

	// Generated message map functions
protected:
	//{{AFX_MSG(CRhinoDimStyleComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif

