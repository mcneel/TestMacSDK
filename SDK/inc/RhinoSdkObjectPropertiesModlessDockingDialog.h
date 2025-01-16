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


// CRhinoObjectPropertiesModelessDockingDialog

class CRhinoObjectPropertiesModelessDockingDialog : public CRhinoUiModelessDockingDialog
{
	DECLARE_SERIAL(CRhinoObjectPropertiesModelessDockingDialog)

protected:
  CRhinoObjectPropertiesModelessDockingDialog( LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
  CRhinoObjectPropertiesModelessDockingDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL);
  virtual ~CRhinoObjectPropertiesModelessDockingDialog();
  void Construct();

protected:
  //
  // Virtual interfaces
  //
  virtual void InitControls( const CRhinoObject* new_obj = NULL);
  virtual BOOL32 AddPageToControlBar( const CRhinoObject* obj = NULL) const;
  virtual CRhinoCommand::result RunScript( ON_SimpleArray<const CRhinoObject*>& objects);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Methods you may override
  //
  enum page_type
  {
    material_page = 0,  // Replace the material page with this page, only works are the current render plug-in
    light_page,         // Replace light page with this page, only works are the current render plug-in
    custom_page,        // User defined custom page
    object_prop_page,   // For internal use
    dimension_page,     // If page is provided by Rhino and only dimensions are selected activate this page
    leader_page,        // If page is provided by Rhino and leaders are selected activate this page
    text_page,          // If page is provided by Rhino and only text objects are selected activate this page
    hatch_page,         // If page is provided by Rhino and only hatch objects are selected activate this page
    page_count          // reserved, do not use
  };
  //-----------------------------------------------------------------------------------------------------------------------------
  //
  // Description:
  //   This defaults to custom_page.  Override and return the system page you want to replace.  Values of material_page and
  //   light_page are only meaningful if this page was added by a CRhinoRenderPlugIn and the plug-in is the current renderer.
  virtual page_type PageType() const;

  virtual void OnCloseDocument( CRhinoDoc& doc );
  virtual void OnNewDocument( CRhinoDoc& doc );
  virtual void OnBeginOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL32 bMerge, BOOL32 bReference);
  virtual void OnEndOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL32 bMerge, BOOL32 bReference);
  virtual void OnBeginSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL32 bExportSelected);
  virtual void OnEndSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL32 bExportSelected);
  virtual void OnDocumentPropertiesChanged( CRhinoDoc& doc);

  virtual void OnModifyObjectAttributes( CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes);
  virtual void LayerTableEvent( CRhinoEventWatcher::layer_event event, const CRhinoLayerTable& layer_table, 
                                 int layer_index, const ON_Layer* old_settings);
  virtual void LightTableEvent( CRhinoEventWatcher::light_event event, const CRhinoLightTable& light_table,
                                 int light_index, const ON_Light* old_settings);
  virtual void MaterialTableEvent( CRhinoEventWatcher::material_event event, const CRhinoMaterialTable& material_table,
                                    int material_index, const ON_Material* old_settings);
  virtual void GroupTableEvent( CRhinoEventWatcher::group_event event, const CRhinoGroupTable& group_table,
                                 int group_index, const ON_Group* old_settings);
  virtual void DimStyleTableEvent( CRhinoEventWatcher::dimstyle_event event, const CRhinoDimStyleTable& dimstyle_table,
                                    int dimstyle_index, const ON_DimStyle* old_settings);
  virtual void HatchPatternTableEvent( CRhinoEventWatcher::hatchpattern_event event, const CRhinoHatchPatternTable& hatchpattern_table,
                                    int hatchpattern_index, const ON_HatchPattern* old_settings);
  virtual void LinetypeTableEvent( CRhinoEventWatcher::linetype_event event, const CRhinoLinetypeTable& linetype_table,
                                    int linetype_index, const ON_Linetype* old_settings);
  virtual void OnAppSettingsChanged( const CRhinoAppSettings& new_app_settings);
  virtual void OnUpdateObjectMesh( CRhinoDoc& doc, CRhinoObject& object, ON::mesh_type mesh_type);
  virtual void InstanceDefinitionTableEvent( CRhinoEventWatcher::idef_event event, const CRhinoInstanceDefinitionTable& idef_table,
                                              int idef_index, const ON_InstanceDefinition* old_settings);
public:
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Access to selected objects list
  //
  virtual int SelectedObjectCount() const;
  const CRhinoObject* GetSelectedObject( int index) const;

protected:
	DECLARE_MESSAGE_MAP()
};


