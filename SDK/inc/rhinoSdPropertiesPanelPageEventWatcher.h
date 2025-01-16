#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPropertiesPanelPageEventWatcher
//
// Description:
//  Use this interface when deriving from IRhinoPropertiesPanelPage to receive
//  event notifications from the properties panel when your page is the active
//  page.
//o
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoPropertiesPanelPageEventWatcher
{
public:
  /*
  WARNING:
  Never modify the Rhino document in an OnCloseDocument() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnCloseDocument() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnCloseDocument(CRhinoDoc& doc);
  /*
  WARNING:
  Never modify the Rhino document in an OnNewDocument() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnNewDocument() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnNewDocument(CRhinoDoc& doc);
  /*
  WARNING:
  Never modify the Rhino document in an OnBeginOpenDocument() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnBeginOpenDocument() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL32 bMerge, BOOL32 bReference);
  /*
  WARNING:
  Never modify the Rhino document in an OnEndOpenDocument() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnEndOpenDocument() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL32 bMerge, BOOL32 bReference);
  /*
  WARNING:
  Never modify the Rhino document in an OnBeginSaveDocument() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnBeginSaveDocument() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnBeginSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL32 bExportSelected);
  /*
  WARNING:
  Never modify the Rhino document in an OnEndSaveDocument() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnEndSaveDocument() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnEndSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL32 bExportSelected);
  /*
  WARNING:
  Never modify the Rhino document in an OnDocumentPropertiesChanged() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnDocumentPropertiesChanged() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnDocumentPropertiesChanged(CRhinoDoc& doc);
  /*
  // Description:
  //   Called if an object's attributes change.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object whose attributes were changed.  The new
  //       attributes are object.Attributes().
  //   old_attributes - [in] attributes before the change.
  WARNING:
  Never modify the Rhino document in a OnModifyObjectAttributes() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnModifyObjectAttributes() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes);
  /*
  // Description:
  //   LayerTableEvent() is called whenever the layer table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   layer_table - [in] layer table
  //   layer_index - [in] index of layer
  //     If the event is layer_sorted, then layer_index is 0.
  //   old_settings - [in] If the event is layer_modified,
  //       then the old layer definition is
  //       passed here. Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a LayerTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your LayerTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void LayerTableEvent(
    CRhinoEventWatcher::layer_event event,
    const CRhinoLayerTable& layer_table,
    int layer_index,
    const ON_Layer* old_settings);
  /*
  // Description:
  //   LightTableEvent() is called whenever the light table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   light_table - [in] light table
  //   light_index - [in] light of light
  //     If the event is light_sorted, then light_index is 0.
  //   old_settings - [in] If the event is light_modified,
  //       then the old light definition is
  //       passed here. Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a LightTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your LightTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void LightTableEvent(
    CRhinoEventWatcher::light_event event,
    const CRhinoLightTable& light_table,
    int light_index,
    const ON_Light* old_settings);
  /*
  // Description:
  //   MaterialTableEvent() is called whenever the material table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   material_table - [in] material table
  //   material_index - [in] index of material
  //     If the event is material_sorted, then material_index is 0.
  //   old_settings - [in] If the event is material_modified,
  //       then the old material definition is
  //       passed here. Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a MaterialTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your MaterialTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void MaterialTableEvent(CRhinoEventWatcher::material_event event,
    const CRhinoMaterialTable& material_table,
    int material_index,
    const ON_Material* old_settings);
  /*
  // Description:
  //   GroupTableEvent() is called whenever the group table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   group_table - [in] group table
  //   group_index - [in] index of group
  //     If the event is group_sorted, then group_index is 0.
  //   old_settings - [in] If the event is group_modified,
  //       then the old group definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a GroupTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your GroupTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void GroupTableEvent(
    CRhinoEventWatcher::group_event event,
    const CRhinoGroupTable& group_table,
    int group_index,
    const ON_Group* old_settings);
  /*
  // Description:
  //   DimStyleTableEvent() is called whenever the dimstyle table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   dimstyle_table - [in] dimstyle table
  //   dimstyle_index - [in] index of dimstyle
  //     If the event is dimstyle_sorted, then dimstyle_index is 0.
  //   old_settings - [in] If the event is dimstyle_modified,
  //       then the old dimstyle definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a DimStyleTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your DimStyleTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void DimStyleTableEvent(
    CRhinoEventWatcher::dimstyle_event event,
    const CRhinoDimStyleTable& dimstyle_table,
    int dimstyle_index,
    const ON_DimStyle* old_settings);
  /*
  // Description:
  //   HatchPatternTableEvent() is called whenever the hatchpattern table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   hatchpattern_table - [in] hatch pattern table
  //   hatchpattern_index - [in] index of hatchpattern
  //     If the event is hatchpattern_sorted, then hatchpattern_index is 0.
  //   old_settings - [in] If the event is hatchpattern_modified,
  //       then the old hatchpattern definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a HatchPatternTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your HatchPatternTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void HatchPatternTableEvent(
    CRhinoEventWatcher::hatchpattern_event event,
    const CRhinoHatchPatternTable& hatchpattern_table,
    int hatchpattern_index,
    const ON_HatchPattern* old_settings);
  /*
  // Description:
  //   LinetypeTableEvent() is called whenever the linetype table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   linetype_table - [in] linetype table
  //   linetype_index - [in] index of linetype
  //     If the event is linetype_sorted, then linetype_index is 0.
  //   old_settings - [in] If the event is linetype_modified,
  //       then the old linetype definition is
  //       passed here. Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a LinetypeTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your LinetypeTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void LinetypeTableEvent(
    CRhinoEventWatcher::linetype_event event,
    const CRhinoLinetypeTable& linetype_table,
    int linetype_index,
    const ON_Linetype* old_settings);
  /*
  Description:
  Called after a value in CRhinoApp::AppSettings() changes.
  WARNING:
  Never modify the Rhino document in a OnAppSettingsChanged() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnAppSettingsChanged() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnAppSettingsChanged(const CRhinoAppSettings& new_app_settings);
  /*
  Description:
  Called if the object's render mesh or analysis mesh is updated.
  Parameters:
  doc - [in] object's document
  object - [in] object whose mesh was updated.  The new
  mesh is attached to the object.
  mesh_type - [in] type of mesh (ON::render_mesh or ON::analysis_mesh)

  WARNING:
  Never modify the Rhino document in a OnUpdateObjectMesh() override.
  If you need to change the Rhino document or post update messages
  to controls, then your OnUpdateObjectMesh() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnUpdateObjectMesh(CRhinoDoc& doc, CRhinoObject& object, ON::mesh_type mesh_type);
  /*
  // Description:
  //   InstanceDefinitionTableEvent() is called whenever the idef table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   idef_table - [in] instance definition table
  //   idef_index - [in] index of idef
  //     If the event is idef_sorted, then idef_index is 0.
  //   old_settings - [in] If the event is idef_modified,
  //       then the old idef definition is
  //       passed here. Otherwise, this parameter is NULL.
  //
  WARNING:
  Never modify the Rhino document in a InstanceDefinitionTableEvent() override.
  If you need to change the Rhino document or post update messages
  to controls, then your InstanceDefinitionTableEvent() override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void InstanceDefinitionTableEvent(
    CRhinoEventWatcher::idef_event event,
    const CRhinoInstanceDefinitionTable& idef_table,
    int idef_index,
    const ON_InstanceDefinition* old_settings);
  /*
  WARNING:
  Never modify the Rhino document in a view event override.
  If you need to change the Rhino document or post update messages
  to controls, then your view event override
  should record what happened in an efficient way and then make
  any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnCreateView(CRhinoView* rhino_view);
  virtual void OnDestroyView(CRhinoView* rhino_view);
  virtual void OnSetActiveView(CRhinoView* rhino_view);
  virtual void OnRenameView(CRhinoView* rhino_view);
};
