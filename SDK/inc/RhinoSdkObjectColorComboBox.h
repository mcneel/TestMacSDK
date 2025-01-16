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
// CRhinoObjectColorComboBox window
class RHINO_SDK_CLASS CRhinoObjectColorComboBox : public CRhinoUiColorComboBox
{
// Construction
public:
	CRhinoObjectColorComboBox();

// Attributes
public:
  enum object_colors
  {
    by_layer_color  = 0,
    by_parent_color = 1,
    black_color,
    white_color,
    red_color,
    yellow_color,
    green_color,
    cyan_color,
    blue_color,
    magenta_color,
    custom_color,
    color_count
  };
  
  virtual void SetCurrentColor( ON_Color color, object_colors obj_color = by_layer_color);
  virtual int GetCurrentColor( ON_Color& color, object_colors& obj_color);
  virtual BOOL32 OnSelectionChanged();// Call from ON_CBN_SELCHANGE handler

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRhinoObjectColorComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRhinoObjectColorComboBox();

	// Generated message map functions
protected:
  virtual void Initialize();

	//{{AFX_MSG(CRhinoObjectColorComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif

