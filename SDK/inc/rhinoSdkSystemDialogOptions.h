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

////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoObjectPropertiesTabs
//
//  Description:
//    Control class for writing plug-in that adds commands to Rhino.
//
class RHINO_SDK_CLASS CRhinoDocumentPropertiesTabs
{
public:
  enum document_property_tabs
  {
    render_tab = 0,
    render_mesh_tab,
    units_tab,
    page_units_tab,
    dimension_tab,
    notes_tab,
    summary_tab,
    grid_tab,
    linetype_tab,
    hatch_tab,
    text_tab,
    tab_count // this is not a valid tab index, it is the number of tabs
  };

  CRhinoDocumentPropertiesTabs();
  
  void RemoveTab( document_property_tabs);
  void IncludeTab( document_property_tabs);

  BOOL32 TabIncluded( document_property_tabs);

private:
  void SetTabState( document_property_tabs, int);

  char m_tabs[tab_count];
};

