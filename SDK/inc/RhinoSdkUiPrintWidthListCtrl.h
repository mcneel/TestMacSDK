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

// CRhinoUiPrintWidthListCtrl

class RHINO_SDK_CLASS CRhinoUiPrintWidthListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC( CRhinoUiPrintWidthListCtrl )

public:
	CRhinoUiPrintWidthListCtrl();
	virtual ~CRhinoUiPrintWidthListCtrl();

  enum columns
  {
    name_column = 0,
    appearance_column,
    column_count
  };

  /*
  Description:
    Initializes the control.
  Remarks:
    Call this member from CDialog::OnInitDialog().
  */
  void Initialize();

  /*
  Description:
    Returns the current display unit system.
  Returns:
    The current display unit system.
  See Also:
    SetDisplayUnits
  */

  ON::LengthUnitSystem DisplayUnits() const;

  /*
  Description:
    Sets the current display unit system.
  Parameters:
    display_units - [in] the units system. The default is ON::LengthUnitSystem::None.
  See Also:
    DisplayUnits
  */
  void SetDisplayUnits( ON::LengthUnitSystem display_units );

  /*
  Description:
    Inserts a print width into the control.
  Parameters:
    item        - [in] index of the item to insert.
    print_width - [in] the print width to insert.
  Returns:
    >=0 - index of the inserted item.
    -1  - print_width already exist.
  */
  int InsertPrintWidth( int item, double print_width );

  /*
  Description:
    Appends a print width to the end of the control.
  Parameters:
    print_width - [in] the print width to append.
  Returns:
    >=0 - index of the inserted item.
    -1  - print_width already exist.
  */
  int AppendPrintWidth( double print_width );

  /*
  Description:
    Returns the selected print width.
  Parameters:
    print_width - [out] the print width to retrieve.
  Returns:
    TRUE  - if an item is selected. 
    FALSE - if an item is not selected.
  */
  bool SelectedPrintWidth( double& print_width );

  /*
  Description:
    Selects print width that exists in the control.
  Parameters:
    print_width - [in] the print width to select.
  */
  void SetSelectedPrintWidth( double print_width );
  
  /*
  Description:
    Finds a print width that is displayed in the control.
  Parameters:
    print_width - [in] the print width to find.
  Returns:
    >=0 - index of the item.
    -1  - print width cannot be found.
  */
  int FindPrintWidth( double print_width ) const;

  /*
  Description:
    Automatically sizes the name column.
  */
  void SizeNameColumn();

  /*
  Description:
    Automatically sizes the appearance column.
  */
  void SizeAppearanceColumn();

public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnHeaderEndTrack(NMHDR *pNMHDR, LRESULT *pResult);

protected:
  virtual void PreSubclassWindow();
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()

private:
  void DrawPrintWidth( NMLVCUSTOMDRAW* pLVCD );
  ON_wString FormatDisplayString( double print_width );

private:
  ON_SimpleArray<double> m_print_widths;
  ON::LengthUnitSystem m_display_units;
};


