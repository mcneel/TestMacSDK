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

/// <summary>
/// CRhinoAnnotationSettings class
/// Note:
///   Dimscale is now part of the DimStyles class and this class property is obsolete.
///   Also, much of the CRhinoAnnotationSettings class has been replaced with
///   CRhinoAnnotationSettingsEx.
/// </summary>
class RHINO_SDK_CLASS CRhinoAnnotationSettings
{
public:
  CRhinoAnnotationSettings() = default;
  ~CRhinoAnnotationSettings();
  CRhinoAnnotationSettings(const ON_3dmAnnotationSettings& src);

  // the compare operators are used to test for changed settings
  BOOL32 operator ==(const CRhinoAnnotationSettings& other);
  BOOL32 operator !=(const CRhinoAnnotationSettings& other);

  CRhinoAnnotationSettings& operator=(const ON_3dmAnnotationSettings& src);

  ON_DEPRECATED_MSG("Dim scale property moved from document to a per ON_DimStyle property.")
  double DimScale() const;

  ON_DEPRECATED_MSG("Dim scale property moved from document to a per ON_DimStyle property.")
  void SetDimScale(double scale);

  double ModelSpaceTextScale() const;
  void SetModelSpaceTextScale(double scale);

  double ModelSpaceHatchScale() const;
  void SetModelSpaceHatchScale(double scale);

  bool UseDimensionLayer() const;
  void EnableUseDimensionLayer(bool bEnable);

  ON_UUID DimensionLayerId() const;
  void SetDimensionLayerId(const ON_UUID& dimension_layer_id);

private:
  mutable class CRhinoAnnotationSettingsPrivate* m_private = nullptr;
};

class RHINO_SDK_CLASS CRhinoAnnotationSettingsEx
{
public:
  CRhinoAnnotationSettingsEx();
  ~CRhinoAnnotationSettingsEx();

  /*
  Description:
    Gets the scale factor for text in model space when annotation scaling is enabled.
  */
  double ModelSpaceTextScale() const;

  /*
  Description:
    Sets the scale factor for text in model space when annotation scaling is enabled.
  */
  void SetModelSpaceTextScale(double scale);

  /*
  Description:
    Gets the scale factor for hatches in model space when hatch scaling is enabled.
  */
  double ModelSpaceHatchScale() const;

  /*
  Description:
    Sets the scale factor for hatches in model space when hatch scaling is enabled.
  */
  void SetModelSpaceHatchScale(double scale);

  /*
  Description:
    Returns true if model space annotation scaling is enabled.
  */
  bool IsModelSpaceAnnotationScalingEnabled() const;

  /*
  Description:
    Enables or disabled model space annotation scaling.
  */
  void EnableModelSpaceAnnotationScaling(bool bEnable);

  /*
  Description:
    Return true if layout space annotation scaling is enabled.
  */
  bool IsLayoutSpaceAnnotationScalingEnabled() const;

  /*
  Description:
    Enables or disabled layout space annotation scaling.
  */
  void EnableLayoutSpaceAnnotationScaling(bool bEnable);

  /*
  Description: 
    Return true if hatch scaling is enabled.
  */
  bool IsHatchScalingEnabled() const;
 
  /*
  Description:
    Enables or disables hatch scaling.
  */
  void EnableHatchScaling(bool bEnable);

private:
  bool m_bModelSpaceAnnotationScalingEnabled = true;
  bool m_bLayoutSpaceAnnotationScalingEnabled = true;
  bool m_bHatchScalingEnabled = true;
  bool m_breserved[5] = { false };
  double m_text_scale = 1.0;   // This is a remenant from V5. Text scale is included in dimstyles in V6
  double m_hatch_scale = 1.0;
  double reserved[7] = { 0 };
};


class RHINO_SDK_CLASS CRhinoDocProperties  
{
public:
	CRhinoDocProperties();

	virtual ~CRhinoDocProperties();
  
  CRhinoDoc* Doc(){ return m_doc;}

  void Default();

  const wchar_t* ModelName() const;

  /*
  Description:
    Fill in an ON_3dmSettings class with the values of the
    current document settings.
  Parameters:
    settings - [out]
  Remarks:
    To get and set individual settings, use the member functions
    below.
  */
  void GetCurrentSettings( ON_3dmSettings& settings ) const;

  /*
  */
  BOOL32 GetON_3dmAnnotationSettings( ON_3dmAnnotationSettings& settings) const;

  /*
  Remarks:
    These settings fields are ignored because they are specified
    elsewhere.
    settings.m_views                    CRhinoDoc::GetViewList
    settings.m_current_layer_index      CRhinoDoc::m_layer_table
    settings.m_current_material_index   CRhinoDoc::m_material_table
    settings.m_current_material_source  CRhinoDoc::m_material_table
  */
  void SetFromSettings( const ON_3dmSettings& settings );

  const wchar_t* ModelURL();
  void SetModelURL( const wchar_t*);

  ON_3dPoint ModelBasepoint() const;

  void SetModelBasepoint(ON_3dPoint model_basepoint);

  ON_EarthAnchorPoint EarthAnchorPoint() const;

  void SetEarthAnchorPoint( ON_EarthAnchorPoint earth_anchor_point );

  ON_3dmIOSettings IOSettings() const;
  
  void SetIOSettings(ON_3dmIOSettings io_settings );

  //////////
  // model space tolerances and unit system
  const ON_UnitSystem& ModelUnits() const;

  const ON_3dmUnitsAndTolerances& ModelUnitsAndTolerances() const;
  void SetModelUnitsAndTolerances( 
    const ON_3dmUnitsAndTolerances& u_and_t, 
    bool bScaleModelObjects = false
    );

  // OBSOLETE - use ModelUnitsAndTolerances
  const ON_3dmUnitsAndTolerances& UnitsAndTolerances() const; 

  // OBSOLETE - use SetModelUnitsAndTolerances
  void SetUnitsAndTolerances( 
    const ON_3dmUnitsAndTolerances& u_and_t, 
    bool bScaleModelObjects = false
    );

  //////////
  // page space tolerances and unit system
  const ON_UnitSystem& PageUnits() const;

  const ON_3dmUnitsAndTolerances& PageUnitsAndTolerances() const;

  void SetPageUnitsAndTolerances( 
    const ON_3dmUnitsAndTolerances& u_and_t, 
    bool bScalePaperObject = false
    );

  /*
  Description:
    Set the style of render mesh to create when surfaces, breps and
    extrusions need to be meshed for shaded rendering.
  Parameters:
    render_mesh_style - [in]
      ON_MeshParameters::render_mesh_fast    = ON_MeshParameters::FastRenderMesh()
      ON_MeshParameters::render_mesh_quality = ON_MeshParameters::QualityRenderMesh()
      ON_MeshParameters::render_mesh_custom  = CustomRenderMeshSettings().
      all other values are ignored
  */
  void SetRenderMeshStyle( ON_MeshParameters::MESH_STYLE render_mesh_style );

  /*
  Returns:
    Style of render mesh that is created when objects like surface, breps,
    and extrusions need to be meshed for shaded display.
      ON_MeshParameters::render_mesh_fast    = ON_MeshParameters::FastRenderMesh()
      ON_MeshParameters::render_mesh_quality = ON_MeshParameters::QualityRenderMesh()
      ON_MeshParameters::render_mesh_custom  = CustomRenderMeshSettings().
  */
  ON_MeshParameters::MESH_STYLE RenderMeshStyle() const;

  /*
  Description:
    Set meshing parameters to use when the render mesh style
    is ON_MeshParameters::render_mesh_custom.
  */
  void SetCustomRenderMeshParameters( const ON_MeshParameters& customer_render_mesh_parameters );

  /*
  Returns:
    Meshing parameters to use when creating a render mesh.
    = MeshParameters( RenderMeshStyle() );
  */
  const ON_MeshParameters& RenderMeshParameters() const;

  /*
  Parameters:
    mesh_style - [in]
      ON_MeshParameters::MESH_STYLE enum value
  Returns:
    Meshing parameters to use to create the specified style of mesh.
  */
  const ON_MeshParameters& MeshParameters( ON_MeshParameters::MESH_STYLE mesh_style ) const;


  ON_DEPRECATED_MSG("use RenderMeshParameters")
  const ON_MeshParameters& RenderMeshSettings() const;
  
  ON_DEPRECATED_MSG("use SetRenderMeshStyle")
  void SetRenderMeshSettings( const ON_MeshParameters&);

  ON_DEPRECATED_MSG("use MeshParameters(ON_MeshParameters::custom_render_mesh)")
  const ON_MeshParameters& CustomRenderMeshSettings() const;

  ON_DEPRECATED_MSG("use SetCustomRenderMeshParameters")
  void SetCustomRenderMeshSettings( const ON_MeshParameters&);





  void GetCustomRenderSizes( ON_SimpleArray<ON_2iSize>&sizes);

  // settings used for automatically created analysis meshes
  const ON_MeshParameters& AnalysisMeshSettings() const;
  void SetAnalysisMeshSettings( const ON_MeshParameters&);

  // settings used when annotation objects are created
  // Deprecated 27 Aug 2010 - Lowell
  //ON_DEPRECATED const CRhinoAnnotationSettings& AnnotationSettings() const;
  //ON_DEPRECATED void SetAnnotationSettings( const CRhinoAnnotationSettings& as, BOOL32 bUpdateDims = TRUE);

  // Settings having to do with model space / layout space scaling of dimensions and text
  // Added 27 Aug, 2010
  CRhinoAnnotationSettingsEx GetAnnotationSettings() const;
  void SetAnnotationSettings(const CRhinoAnnotationSettingsEx& as);

  //////////
  // The following functions take a zero based index into an array
  // and return NULL if index is out of range.
  const ON_3dmConstructionPlane* NamedConstructionPlane( int) const;
  const ON_3dmView* NamedView( int) const;
  int NamedViewCount() const;
  //const ON_3dmView* View( int) const;

  /*
  Description:
    Find a named construction plane
  Parameters:
    name - [in]
  Returns:
    pointer to named construction plane
  See Also:
    CRhinoDocProperties::NamedConstructionPlaneIndex
    CRhinoDocProperties::AddNamedConstructionPlane
    CRhinoDocProperties::RemoveNamedConstructionPlane
    CRhinoDocProperties::MoveNamedConstructionPlane
  */
  const ON_3dmConstructionPlane* NamedConstructionPlane( const wchar_t* name ) const;

  /*
  Description:
    Get zero based index of named construction plane
  Parameters:
    cplane - [in]
  Returns:
    0 based index of named construction plane or -1 if cplane
    is not one of documents named construction planes.
  See Also:
    CRhinoDocProperties::NamedConstructionPlane
    CRhinoDocProperties::AddNamedConstructionPlane
    CRhinoDocProperties::RemoveNamedConstructionPlane
    CRhinoDocProperties::MoveNamedConstructionPlane
  */
  int NamedConstructionPlaneIndex( const ON_3dmConstructionPlane* cplane ) const;

  /*
  Description:
    Get zero based index of named construction plane
  Parameters:
    cplane_index - [in]
  Returns:
    TRUE if plane was removed.
  See Also:
    CRhinoDocProperties::NamedConstructionPlane
    CRhinoDocProperties::NamedConstructionPlaneIndex
    CRhinoDocProperties::AddNamedConstructionPlane
    CRhinoDocProperties::MoveNamedConstructionPlane
  */
  BOOL32 RemoveNamedConstructionPlane( int cplane_index );

  /*
  Description:
    Move a named construction plane before another one.
  Parameters:
    cplane_index - [in] The index of the named construction plane to move.
    cplane_index_before - [in] The index of the named construction plane to move before. -1 means move to end of list.
  Returns:
    true if construction plane was moved.
  See Also:
    CRhinoDocProperties::NamedConstructionPlane
    CRhinoDocProperties::NamedConstructionPlaneIndex
    CRhinoDocProperties::AddNamedConstructionPlane
    CRhinoDocProperties::RemoveNamedConstructionPlane
  */
  bool MoveNamedConstructionPlane( int cplane_index, int cplane_index_before );

  /*
  Description:
    Find a named view.
  Parameters:
    name - [in]
  Returns:
    pointer to the named view or null if not found.
  See Also:
    CRhinoDocProperties::NamedView
    CRhinoDocProperties::AddNamedView
    CRhinoDocProperties::GetNamedViewSelection
  */
  const ON_3dmView* NamedView( const wchar_t* name ) const;

  /*
  Description:
    Add named construction plane to document.
  Parameters:
    name_cplane - [in]
  Remarks:
    If named_cplane has an empty name, a unique name
    is automatically created.  If there is already a 
    named onstruction plane with the same name, that 
    construction plane is replaced.
  Returns:
    >= 0 : success 0 based index of named construction plane.
      -1 : failure invalid plane
  See Also:
    CRhinoDocProperties::NamedConstructionPlane
  */
  int AddNamedConstructionPlane( const ON_3dmConstructionPlane& named_cplane );

  /*
  Description:
    Add a named view to the document.
  Parameters:
    named_view - [in]
  Remarks:
    If named_view has an empty name, a unique name
    is automatically created.  If there is already a 
    named view with the same name, that view is
    replaced.
  See Also:
    CRhinoDocProperties::NamedView
  Returns:
    >= 0 : success 0 based index of named view.
      -1 : failure invalid plane
  See Also:
    CRhinoDocProperties::NamedView
    CRhinoDocProperties::NamedViewIndex
    CRhinoDocProperties::RemoveNamedView
    CRhinoDocProperties::MoveNamedView
    CRhinoDocProperties::GetNamedViewSelection
  */
  int AddNamedView( const ON_3dmView& named_view );

  /*
  Description:
    Get zero based index of named view
  Parameters:
    view - [in]
  Returns:
    0 based index of named view or -1 if view
    is not one of documents named views.
  See Also:
    CRhinoDocProperties::NamedView
    CRhinoDocProperties::AddNamedView
    CRhinoDocProperties::RemoveNamedView
    CRhinoDocProperties::MoveNamedView
    CRhinoDocProperties::GetNamedViewSelection
  */
  int NamedViewIndex( const ON_3dmView* view ) const;

  /*
  Description:
    Remove a named view from the document.
  Parameters:
    view_index - [in]
  Returns:
    TRUE if view was removed.
  See Also:
    CRhinoDocProperties::NamedView
    CRhinoDocProperties::NamedViewIndex
    CRhinoDocProperties::AddNamedView
    CRhinoDocProperties::MoveNamedView
    CRhinoDocProperties::GetNamedViewSelection
  */
  BOOL32 RemoveNamedView( int view_index );

  /*
  Description:
    Move a named view before another one.
  Parameters:
    view_index - [in] The index of the named view to move.
    view_index_before - [in] The index of the named view to move before. -1 means move to end of list.
  Returns:
    true if view was moved.
  See Also:
    CRhinoDocProperties::NamedView
    CRhinoDocProperties::NamedViewIndex
    CRhinoDocProperties::AddNamedView
    CRhinoDocProperties::RemoveNamedView
    CRhinoDocProperties::GetNamedViewSelection
  */
  bool MoveNamedView( int view_index, int view_index_before );

  /*
  Description:
    Gets the current selection in the Named Views panel. If more than one such panel is open,
    it will return the selection in the most recently opened panel.
  Parameters:
    named_views - [out] The names of the selected named views are appended to this array.
  Returns:
    true if successful, false if there are no named view panels open.
  See Also:
    CRhinoDocProperties::NamedView
    CRhinoDocProperties::NamedViewIndex
    CRhinoDocProperties::AddNamedView
    CRhinoDocProperties::RemoveNamedView
  */
  bool GetNamedViewSelection(ON_ClassArray<ON_wString>& named_views) const;

  /*
  Description:
      Number of saved snapshots.
  Returns:
      Number of saved snapshots in the document.
  */
  unsigned int SnapshotCount(void) const;
  
  /*
  Description:
      Gets the name of a snapshot at a specified index.
  Parameters:
      iIndex - [In] Zero based index of snapshot.
      sName - [Out] Name of the snapshot at given index.
  Returns:
    True or false depending if index available.
  */
  bool SnapshotAt(int iIndex, ON_wString& sName) const;
  
  /*
  Description:
      Array of snapshot names in the document.
  Parameters:
      aNames - [Out] Array of snapshot names
  */
  void SnapshotNames(ON_ClassArray<ON_wString>& aNames) const;
  
  /*
  Description:
      Saves a snapshot with all available clients.
  Parameters:
      wszName - [In] Name of the snapshot.
  Returns:
    True or false depending if the save succeeded.
  */
  bool SaveSnapshot(const wchar_t* wszName);
  
  /*
  Description:
      Saves a snapshot with a selection of clients.
  Parameters:
      wszName - [In] Name of the snapshot.
      aClients - [In] IDs of enabled clients.
  Returns:
    True or false depending if the save succeeded.
  */
  bool SaveSnapshot(const wchar_t* wszName, const ON_SimpleArray<UUID>& aClients);
  
  /*
  Description:
      Restores the specified snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      aClients - [In] Array of clients.
      bSkipAnimation - [In] Flag to enable/disable animation.
      bSkipUnsavedModelWarning - [In] Flag to enable/disable a message box that warns the user about unsaved data in the document.
  Returns:
    True or false depending if the restore succeeded.
  */
  bool RestoreSnapshot(const wchar_t* wszName, const ON_SimpleArray<UUID>& aClients, bool bSkipAnimation, bool bSkipUnsavedModelWarning);
  
  /*
  Description:
      Restores the specified snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      bSkipAnimation - [In] Flag to enable/disable animation.
      bSkipUnsavedModelWarning - [In] Flag to enable/disable a message box that warns the user about unsaved data in the document.
  Returns:
    True or false depending if the restore succeeded.
  */
  bool RestoreSnapshot(const wchar_t* wszName, bool bSkipAnimation, bool bSkipUnsavedModelWarning);
  
  /*
  Description:
      Renames the specified snapshot.
  Parameters:
      wszNameOld - [In] Current name of the snapshot.
      wszNameNew - [In] New name for the snapshot.
  Returns:
    True or false depending if the renaming succeeded.
  */
  bool RenameSnapshot(const wchar_t* wszNameOld, const wchar_t* wszNameNew);
  
  /*
  Description:
      Deletes the specified snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
  Returns:
    True or false depending if the deleting succeeded.
  */
  bool DeleteSnapshot(const wchar_t* wszName);
  
  /*
  Description:
      Snapshot lookup.
  Parameters:
      wszName - [In] Name of the snapshot.
  Returns:
    True or false depending if the snapshot exists in the document.
  */
  bool SnapshotLookUp(const wchar_t* wszName) const;
  

  /*
  Description:
      Gets the thumbnail of a specified snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      size - [In] Size of the thumbnail.
      dib - [Out] Thumbnail of the snapshot.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotGetThumbnail(const wchar_t* wszName, const ON_2iSize& size, CRhinoDib& dib) const;

  /*
  Description:
      Number of registered clients.
  Parameters:
      doc - [In] The active document.
  Returns:
      Number of registered clients.
  */
  unsigned int SnapshotsClientCount(void) const;
  
  /*
  Description:
      Gets the name and ID of a client at a specified index.
  Parameters:
      iIndex - [In] Zero based index of snapshot.
      sId - [Out] ID of the client at given index.
      sName - [Out] Name of the client at given index.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsClientAt(int iIndex, ON_UUID& id, ON_wString& sName) const;
  
  /*
  Description:
      Gets the name of a client.
  Parameters:
      sId - [In] ID of the client.
      sName - [Out] Name of the client.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsClientName(const ON_UUID& id, ON_wString& sName) const;
  
  /*
  Description:
      Gets the category of a client.
  Parameters:
      sId - [In] ID of the client.
      sCategory - [Out] Category of the client.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsClientCategory(const ON_UUID& id, ON_wString& sCategory) const;
  
  /*
  Description:
      Gets the clients of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      aClients - [Out] Array of client.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsClients(const wchar_t* wszName, ON_SimpleArray<UUID>& aClients) const;
  
  /*
  Description:
      Gets the state of a client of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      idClient - [In] ID of the client.
      bEnabled - [Out] Enable/Disable state.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsIsClientEnabled(const wchar_t* wszName, const UUID& idClient, bool& bEnabled) const;
  
  /*
  Description:
      Sets the state of a client of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      idClient - [In] ID of the client.
      bEnabled - [In] Enable/Disable state.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsEnableClient(const wchar_t* wszName, const UUID& idClient, bool bEnable);
  
  /*
  Description:
      Gets the state of a client of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      idClient - [In] ID of the client.
      bEnabled - [Out] Enable/Disable state.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsIsClientEnabledForPartialRestore(const wchar_t* wszName, const UUID& idClient, bool& bEnabled) const;
  
  /*
  Description:
      Sets the state of a client of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      idClient - [In] ID of the client.
      bEnabled - [In] Enable/Disable state.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsEnableClientForPartialRestore(const wchar_t* wszName, const UUID& idClient, bool bEnable);
  
  /*
Description:
    Gets the data of a client of a named snapshot.
Parameters:
    wszName - [In] Name of the snapshot.
    idClient - [In] ID of the client.
    data - [Out] Client data.
Returns:
  True or false depending if function call is successful.
*/
  bool SnapshotsClientData(const wchar_t* wszName, const UUID& idClient, ON_Buffer& data) const;

  /*
  Description:
      Sets the data of a client of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      idClient - [In] ID of the client.
      data - [In] Client data.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsSetClientData(const wchar_t* wszName, const UUID& idClient, const ON_Buffer& data);

  /*
  Description:
      Gets the view of a named snapshot.
  Parameters:
      wszName - [In] Name of the snapshot.
      view - [In] View of a snapshot.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsGetView(const wchar_t* wszName, ON_3dmView& view) const;
  
  /*
  Description:
      Saves a temporary snapshot of the current document state.
  Parameters:
      id - [OUT] id of the snapshot.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsTemporarySave(ON_UUID& id);
  
  /*
  Description:
      Restores a temporary snapshot.
  Parameters:
      id - [In] id of the snapshot.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsTemporaryRestore(const ON_UUID& id);
  
  /*
  Description:
      Deletes a temporary stored snapshot.
  Parameters:
      id - [In] id of the snapshot.
  Returns:
    True or false depending if function call is successful.
  */
  bool SnapshotsDeleteTemporarySave(const ON_UUID& id);

  /*
  Description:
    Attach a user string to a snapshot.
  Parameters:
    wszName - [in] name of the snapshot.
    wszKey - [in] id used to retrieve this string.
    wszValue - [in] If nullptr, the string with this id will be removed.
  Returns:
    True if successful.
  */
  bool SnapshotsSetUserString(const wchar_t* wszName, const wchar_t* wszKey, const wchar_t* wszValue);

  /*
  Description:
    Append entries to the user string list.
  Parameters:
    wszName - [in] name of the snapshot.
    iCount - [in] number of element in pStrings array.
    pStrings - [in] entries to append.
    bReplace - [in] If bReplace is true, then existing entries with the same key are
      updated with the new entry's value.  If bReplace is false, then existing entries are not updated.
  Returns:
    Number of entries added, deleted, or modified.
  */
  int SnapshotsSetUserStrings(const wchar_t* wszName, int iCount, const ON_UserString* pStrings, bool bReplace);

  /*
  Description:
    Get user string from a snapshot.
  Parameters:
    wszName - [in] name of the snapshot.
    wszKey - [in] id used to retrieve the string.
    sValue - [out]
  Returns:
    True if a string with the id was found.
  */
  bool SnapshotsGetUserString(const wchar_t* wszName, const wchar_t* wszKey, ON_wString& sValue) const;

  /*
  Description:
    Get a list of all user strings from a snapshot.
  Parameters:
    wszName - [in] name of the snapshot.
    aStrings - [out] user strings are appended to this list.
  Returns:
    Number of elements appended to the aStrings list.
  */
  int SnapshotsGetUserStrings(const wchar_t* wszName, ON_ClassArray<ON_UserString>& aStrings) const;

  /*
  Description:
    Get a list of all user string keys from a snapshot.
  Parameters:
    wszName - [in] name of the snapshot.
    aKeys - [out] User string keys are appended to this list.
  Returns:
    Number of elements appended to the aKeys list.
  */
  int SnapshotsGetUserStringKeys(const wchar_t* wszName, ON_ClassArray<ON_wString>& aKeys) const;

   /*
  Description:
    Gets user string count from a snapshot.
  Parameters:
    wszName - [in] name of the snapshot.
  Returns:
    Number of user strings on a snapshot.
  */
  int SnapshotsUserStringCount(const wchar_t* wszName) const;

  const ON_3dmAnimationProperties& AnimationProperties();
  void SetAnimationProperties(const ON_3dmAnimationProperties& ap);

  bool AnimatedViewAtFrame(const ON_3dmAnimationProperties& ap, int iFrame, ON_Viewport& vp);
  bool AnimatedSunAtFrame(const ON_3dmAnimationProperties& ap, int iFrame, ON_Light& sun);

  // The following functions set/get values.  The set functions return
  // the previous value on success otherwise -1

// TODO: ?Are these handled in CRhinoDoc?
/*
  int CurrentLayerIndex() const;
  int SetCurrentLayerIndex( int);

  int CurrentMaterialIndex() const;
  int SetCurrentMaterialIndex( int);
*/
  const ON::object_material_source CurrentMaterialSource() const;
  ON::object_material_source SetCurrentMaterialSource( ON::object_material_source);

  ON_Color CurrentColor() const;
  ON_Color SetCurrentColor( ON_Color);

  ON::object_color_source CurrentColorSource() const;
  ON::object_color_source SetCurrentColorSource( ON::object_color_source);

  // line style if ON::line_style_from_object == LineStyleSource()
  // OBSOLETE 17 March 2005 Dale Lear - const ON_LineStyle& CurrentLineStyle() const;
  // OBSOLETE 17 March 2005 Dale Lear - ON_LineStyle SetCurrentLineStyle( const ON_LineStyle&);

  // OBSOLETE 17 March 2005 Dale Lear - const ON::object_line_style_source CurrentLineStyleSource() const;
  // OBSOLETE 17 March 2005 Dale Lear - ON::object_line_style_source SetCurrentLineStyleSource( ON::object_line_style_source);

  int CurrentWireDensity() const;
  int SetCurrentWireDensity( int);

  const ON_3dmRenderSettings& RenderSettings() const;
  void SetRenderSettings( const ON_3dmRenderSettings&);

  // Rendered view display mode settings
  // If from_doc == true then values are always returned from CRhinoDocProperties
  // otherwise CRhinoAppSettings::GetRenderedViewSettingsFromDoc() determines
  // where the values are returned from.
  bool DisplayCurvesAndIsocurvesInRenderedViews( bool from_doc = false) const;
  bool DisplayTextAndDimensionsInRenderedViews( bool from_doc = false) const;
  bool UseHiddenLightsInRenderedViews( bool from_doc = false) const;
  ON_Color AmbientLightColor( bool from_doc = false) const;
  ON_Color BackgroundColor( bool from_doc = false) const;

  //////////
  // default settings for construction plane grids
  const ON_3dmConstructionPlaneGridDefaults& GridDefaults() const;
  void SetGridDefaults( const ON_3dmConstructionPlaneGridDefaults&);

  //////////
  // Interface for accessing ON_3dmProperties
  //---------------------------------------------------------------------------------
  const ON_3dmRevisionHistory& RevisionHistory() const;
  void SetRevisionHistory( const ON_3dmRevisionHistory&);

  const ON_3dmNotes& Notes() const;
  void SetNotes( const ON_3dmNotes& );

  // Interface for AnnotationScaling added 4 Nov, 2010 - Lowell
  // Get current value of AnnotationScaling (CRhinoAnnotationSettingsEx)
  
  //bool IsAnnotationScalingEnabled() const;
  //void EnableAnnotationScaling(bool b = true);

  // [Lowell - 3-28-2017] These are 2 new fields to replace the one previously used
  bool IsModelSpaceAnnotationScalingEnabled() const;
  void EnableModelSpaceAnnotationScaling(bool b = true);
  bool IsLayoutSpaceAnnotationScalingEnabled() const;
  void EnableLayoutSpaceAnnotationScaling(bool b = true);
  
  bool IsHatchScalingEnabled() const;
  void EnableHatchScaling(bool b = true);

  bool UseDimensionLayer() const;
  ON_UUID DimensionLayerId() const;
  void SetUseDimensionLayer(bool bEnable, const ON_UUID& dimension_layer_id);

private:
  CRhinoDocProperties( const CRhinoDocProperties&);
  CRhinoDocProperties& operator=( const CRhinoDocProperties&);

private:
  // these settings are serialized using ON_3dmProperties
  ON_3dmRevisionHistory  m_RevisionHistory;
  ON_3dmNotes            m_Notes;

  ON_SimpleArray<ON_2iSize> m_custom_render_sizes;
private:
  // these settings are serialized using ON_3dmSettings
  //ON_3dmSettings         m_settings;

  ON_wString m_model_URL;

  //////////
  // model tolerances and unit system
  ON_3dmUnitsAndTolerances m_ModelUnitsAndTolerances;

  //////////
  // Printing (paper) tolerances and unit system
  //
  ON_3dmUnitsAndTolerances m_PageUnitsAndTolerances;

  // m_render_mesh_style determines what meshin parameters are
  // used to create rendering meshes
  ON_MeshParameters::MESH_STYLE m_render_mesh_style; // ON_MeshParameters::render_mesh fast/quality/custom
                                    // All other values default to ON_MeshParameters::render_mesh_fast
  unsigned char m_reserved1[3];

  unsigned char m_reserved2[96];

  // Custom render settings used when the render mesh settings are
  // changed from a standard type to custom.  This is only used by
  // the UI when changing from a standard mesh settings to user defined
  // settings.
  ON_MeshParameters        m_CustomRenderMeshSettings;

  // settings used for automatically created analysis meshes
  ON_MeshParameters        m_AnalysisMeshSettings;

  // settings used when annotation objects are created
  CRhinoAnnotationSettings m_AnnotationSettings;

  ON_ClassArray<ON_3dmConstructionPlane> m_named_cplanes;
  ON_ClassArray<ON_3dmView>              m_named_views;

 
  ON_Color m_current_color;
  ON::object_color_source m_current_color_source;

  // OBSOLETE - 17 March 2005 Dale Lear - ON_LineStyle  m_current_line_style;     // line style if ON::line_style_from_object == LineStyleSource()
  // OBSOLETE - 17 March 2005 Dale Lear - ON::object_line_style_source m_current_line_style_source;
 
  // Surface wireframe density
  //
  //   @untitled table
  //   0       boundary + "knot" wires 
  //   1       boundary + "knot" wires + 1 interior wire if no interior "knots"
  //   N>=2    boundary + "knot" wires + (N-1) interior wires
  int m_current_wire_density;

  // Fields added 27 Aug, 2010 - Lowell
  //bool m_bAnnotationScaling;

  // [Lowell - 3-28-2017] These are 2 new fields to replace the one previously used
  bool m_bModelSpaceAnnotationScaling = true;
  bool m_bLayoutSpaceAnnotationScaling = true;

  bool m_bHatchScaling;
private:
  unsigned char m_reserved_2;
  unsigned char m_reserved_3;

  ON_3dmRenderSettings m_RenderSettings;

  // default settings for construction plane grids
  ON_3dmConstructionPlaneGridDefaults m_GridDefaults;

  // When this model is inserted as an instance definition,
  // m_model_basepoint is mapped to the "insert point"
  // in the target model.  The Rhino SetModelBasePoint
  // command sets this point.
  ON_3dPoint m_model_basepoint; 

  // When the model is used in conjunction with GIS data,
  // m_earth_anchor_point records the location of the
  // model on Earth.
  ON_EarthAnchorPoint m_earth_anchor_point;

  ON_3dmIOSettings m_io_settings;

private:
  friend class CRhinoDoc;
  friend class CRhRenderSettingsHelper;
  friend class CRhinoNamedViewCameraIcon;
  CRhinoDoc*             m_doc;
  ON_wString             m_model_name;

public:
  class CRhRuntimeSettings* RuntimeSettings();
private:
  CRhRuntimeSettings* m_RuntimeSettings;
};
