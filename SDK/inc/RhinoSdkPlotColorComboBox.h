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

/////////////////////////////////////////////////////////////////////////////
// CRhinoPlotColorComboBox window

class RHINO_SDK_CLASS CRhinoPlotColorComboBox : public CRhinoUiColorComboBox
{
// Construction
public:
	CRhinoPlotColorComboBox();
	~CRhinoPlotColorComboBox();

// Attributes
public:
  enum object_colors
  {
    by_display_color =  0,
    by_layer_color,
    by_parent_color,
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
  
  void SetCurrentColor( ON_Color color, object_colors obj_color = by_layer_color);
  int GetCurrentColor( ON_Color& color, object_colors& obj_color);
  BOOL32 OnSelectionChanged();

// Overrides
protected:
	void PreSubclassWindow();
  void Initialize();
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	DECLARE_MESSAGE_MAP()
};
