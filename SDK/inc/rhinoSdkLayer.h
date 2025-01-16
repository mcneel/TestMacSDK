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

class RHINO_SDK_CLASS CRhinoLayerNode
{
public:
  CRhinoLayerNode();
  ~CRhinoLayerNode();
  CRhinoLayerNode( const CRhinoLayerNode& src );
  CRhinoLayerNode& operator=( const CRhinoLayerNode& src );

  /*
  Description:
    Puts the layer tree information in the m_layer_index, 
    m_parent_layer_index, and m_child_layer_index[] array fields.
  Parameters:
    layer_index - [in]
      index of "this" layer
    list_parents - [in]
      0 = no,
      1 = immediate parent
            If this layer has a parent, then m_parent_count=1 and
            the returned m_parent_list[] array has one element.
      2 = immediate parent, their parent, and so on.
            If this layer has a parent, then m_parent_count>=1 and
            the returned m_parent_list[] is sorted by generation.
            m_parent_list[0] = immediate parent,
            m_parent_list[1] = grandparent, 
            m_parent_list[2] = great grandparent,
            and so on.
    list_children - [in]
      0 = no,
      1 = immediate children
            If the layer has children, then m_child_count is
            the number of immediate children the returned 
            m_child_list[] array list is sorted by layer index.
      2 = immediate children, their children, and so on.
            If the layer has children, then m_child_count is
            the number of children, grandchildren, and so on, 
            and the returned m_child_list[] array list is 
            sorted by layer index.
    bIgnoreDeletedLayers - [in]
      If true (the default), deleted layers are not added to the lists.
      If false, deleted layers are included.
  Returns:
    True if input and layer table are valid and false if an index is out
    of range or an error was detected in the layer tree information.
  */
  bool Create( 
    class CRhinoLayerTable& layer_table,
    int layer_index,
    int list_parents,
    int list_children,
    bool bIgnoreDeletedLayers = true
    );

  void Destroy();

  /*
  Description:
    After Create() has been called, IsChildLayer can be used
    to quickly see if child_layer_index is the index of a
    child of "this" layer.
  Parameters:
    child_layer_index - [in]
  Returns:
    True if child_layer_index is the index of a child layer.
  See Also:
    CRhinoLayerNode::Create
    CRhinoLayerNode::IsParentLayer
  */
  bool IsChildLayer( int child_layer_index ) const;

  /*
  Description:
    After Create() has been called, IsParentLayer can be used
    to quickly see if parent_layer_index is the index of a 
    parent of "this" layer.

  Parameters:
    parent_layer_index - [in]
  Returns:
    True if parent_layer_index is the index of a parent layer.
  See Also:
    CRhinoLayerNode::Create
    CRhinoLayerNode::IsChildLayer
  */
  bool IsParentLayer( int parent_layer_index ) const;

  // index of "this" layer
  int m_layer_index;

  // number of child layers
  int m_child_count;
  int* m_child_list; // indices of child layers

  // number of parent layers
  int m_parent_count;
  int* m_parent_list; // indices of parent layers
};

class RHINO_SDK_CLASS CRhinoLayer : public ON_Layer
{
public:
  // Description:
  //   Layers are always in the Document().m_layer_table.
  //  Returns:
  //   CRhinoDoc that owns this layer.
  CRhinoDoc& Document() const;

  const class CRhinoLayerTable& m_doc_layer_table;

  /*
  Description:
    Rhino allows files to be used as the definitions for
    linked block instances and to be uses as reference
    models in worksessions. When a linked instance definition
    or worksession reference model is present, additional
    parent layers are added to the layer tree to contain the
    layer tables of the referenced file.  If a layer is
    any of these types of layers, then IsReference()
    returns true.  If IsReference() returns true, then
    the layer is never saved in a file.
  Returns:
    true if the layer is a "reference" layer as described above.
  Remarks:
    If you need to know what type of reference the layer is
    used for, use the WorksessionReferenceModelSerialNumber(),
    LinkedInstanceDefinitionSerialNumber() and 
    IsReferenceParentLayer() functions.
  See Also:
    CRhinoLayer::WorksessionReferenceModelSerialNumber
    CRhinoLayer::LinkedInstanceDefinitionSerialNumber
    CRhinoLayer::IsReferenceParentLayer
    CRhinoLayer::IsWorkSessionReferenceParentLayer
    CRhinoLayer::IsLinkedInstanceDefinitionParentLayer
  */
  bool IsReference() const;

  /*
  Returns:
    Worksession reference model id.
         0: layer is not in a reference model.
         1: layer is in an unidentified worksession reference model.
         2-1000: reserved for future use
     >1000:
       Serial number of the worksession reference model.       
  Remarks:
    The reference model serial number is a runtime value.
    It is not saved in files and it is generally different the
    next time a file is read.
  See Also:
    CRhinoLayer::LinkedInstanceDefinitionSerialNumber
    CRhinoLayer::IsReferenceParentLayer
    CRhinoLayer::IsWorkSessionReferenceParentLayer
    CRhinoLayer::IsLinkedInstanceDefinitionParentLayer
  */
  unsigned int WorksessionReferenceModelSerialNumber() const;

  /*
  Returns:
    0: 
      The layer is not part of a linked instance definition
    1-1000: 
      These numbers are reserved for future use.
    >1000:
      CRhinoDoc::m_instance_defintion_table.FindBySerialNumber() can
      be used to find the instance definition.
  Remarks:
    The linked instance definition serial number is a runtime value.
    It is not saved in files and it is generally different the next
    time a file is read.
  See Also:
    CRhinoLayer::WorksessionReferenceModelSerialNumber
    CRhinoLayer::IsReferenceParentLayer
    CRhinoLayer::IsWorkSessionReferenceParentLayer
    CRhinoLayer::IsLinkedInstanceDefinitionParentLayer
  */
  unsigned int LinkedInstanceDefinitionSerialNumber() const;

  /*
  Returns:
    True if the layer is a parent layer of the layer tree
    from a linked instance definition or the layer tree from
    a worksession reference model.
  See Also:
    CRhinoLayer::WorksessionReferenceModelSerialNumber
    CRhinoLayer::LinkedInstanceDefinitionSerialNumber
    CRhinoLayer::IsReference
    CRhinoLayer::IsWorkSessionReferenceParentLayer
    CRhinoLayer::IsLinkedInstanceDefinitionParentLayer
  */
  bool IsReferenceParentLayer() const;

  /*
  Returns:
    True if the layer is a parent layer of the layer tree
    from a worksession reference model.
  See Also:
    CRhinoLayer::WorksessionReferenceModelSerialNumber
    CRhinoLayer::LinkedInstanceDefinitionSerialNumber
    CRhinoLayer::IsReference
    CRhinoLayer::IsReferenceParentLayer
    CRhinoLayer::IsLinkedInstanceDefinitionParentLayer
  */
  bool IsWorkSessionReferenceParentLayer() const;
  
  /*
  Returns:
    True if the layer is a parent layer of the layer tree
    from a linked instance definition model.
  See Also:
    CRhinoLayer::WorksessionReferenceModelSerialNumber
    CRhinoLayer::LinkedInstanceDefinitionSerialNumber
    CRhinoLayer::IsReference
    CRhinoLayer::IsReferenceParentLayer
    CRhinoLayer::IsWorkSessionReferenceParentLayer
  */
  bool IsLinkedInstanceDefinitionParentLayer() const;

  // Description:
  //   Rhino provides the ability to undo layer modifications.
  //
  // Returns:
  //   true if this layer has been modified by 
  //   CRhinoLayerTable::ModifyLayer()and the modifications can
  //   be undone.
  //
  // See Also: UndoModify.
  bool IsModified() const;

  // Description:
  //   Gets the rendering material for this layer.  The
  //   default layer material is 
  //   RhinoApp().AppSettings().DefaultMaterial()
  //
  // Returns:
  //   The rendering material associated with this layer.
  const CRhinoMaterial& LayerMaterial() const;

  // Description:
  //   Gets the RDK rendering material for this layer.
  //
  // Returns:
  //   The rendering material associated with this layer.
  const CRhRdkMaterial* LayerRdkMaterial() const;

  // Description:
  //    Gets the linetype for this layer.
  //
  // Returns:
  //    The linetype associated with this layer.
  const CRhinoLinetype& LayerLinetype() const;

  /*
  Description:
    Returns parent of a layer.
  Parameters:
    bRootLevelParent - [in]
      If true, the root level parent is returned. The root level
      parent never has a parent.
      If false, the immediate parent is returned. The immediate
      parent may have a parent.
  Returns:
    A pointer to the parent or NULL if this layer does not have a parent.
    Calls to CRhinoLayerTable::AddLayer() may make this pointer invalid.
  */
  const CRhinoLayer* ParentLayer( bool bRootLevelParent ) const;

  // Runtime index used to sort layers in layer dialog
  int m_sort_index = -1;

private:
  // Runtime index used when remapping layers for import/export
  int m_remap_index = -1;

public:
  int RemapIndex() const;

public:
  /*
  Returns:
    True if the layer is visible in the layer manager dialog
    and other Rhino user interfaces the display layers.
    Otherwise the layer is filtered.
  */
  bool IsVisibleInRhinoUserInterface() const;
  void SetVisibleInRhinoUserInterface(bool bSet);

  // If you have code that sets these values, delete it.
  // If you have code that uses these values, call
  // ON_Layer::IsVisible() or ON_Layer::IsLocked() instead.
  /* never use or set this value */ bool m_sublayer_bVisible;
  /* never use or set this value */ bool m_sublayer_bLocked;
  
#if defined (ON_RUNTIME_APPLE)
  id MacProxy() const;
#endif

  // Runtime serial number of this layer.
  // This serial number is unique for each layer in an instance
  // of a Rhino application. 
  // This serial number is not saved in files and it is generally
  // different the next time a file is read.
  const unsigned int m_layer_sn;

  /*
  Description:
    Save the current layer settings.
  */
  void SaveSettings();

  /*
  Description:
    Get the settings that were saved by the last call
    to SaveSettings().
  Returns:
    true if settings had been saved.
  */
  bool GetSavedSettings(ON_Layer& saved_settings);

  /*
  Description:
    Delete any saved settings.
  */
  void DeleteSavedSettings();

  static unsigned int NextRuntimeSerialNumber();

private:
  friend class CRhinoLayerTable;
  friend class CRhinoLayerNode;

  // constructor and destructors are private because only
  // CRhinoMaterialTable should be creating CRhinoMaterials.
  // Please do not add any more friend classes.
  CRhinoLayer( CRhinoLayerTable& layer_table );
  CRhinoLayer( CRhinoLayerTable& layer_table, const ON_Layer& );
  ~CRhinoLayer();

  CRhinoLayer() = delete;
  CRhinoLayer& operator=(const CRhinoLayer&) = delete;

  // m_bIsVisibleInRhinoUserInterface will be true if the layer
  // is currently visible in the Rhino user interface. When
  // things like layer manager filters are applied, this field
  // is set to indicate that the layer passes the filer.
  bool m_bIsVisibleInRhinoUserInterface = true;


  // If 0 != m_ref_layer, then the layer is a parent layer
  // added to the layer table by a worksession or linked
  // instance definition to help keep it organized and prevent
  // layer name collisions of sub-layers.
  // The value of  m_ref_flags identifies the function of the layer.
  //   2:  This is a linked idef parent layer containing the layer
  //       table of the linked idef.  The layer typically has the 
  //       same name as file referenced by the linked idef and
  //       ON_ModelComponent.InstanceDefinitionModelSerialNumber()
  //       identifies the lined instance definition.
  //   4:  This is a worksession reference model parent layer 
  //       containing the layer table of the reference model.
  //       The layer typically has the same name as the file
  //       containing the reference model and the 
  //       value of ON_ModelComponent.ReferenceModelSerialNumber()
  //       identifies the worksession reference file.
  unsigned char m_internal_reference_parent_layer_type = 0;
  
  class CRhLayerUndo* m_undo = nullptr;

  void ClearUndoRecords();

  //void CopyHelper(const CRhinoLayer& src);

#if defined (ON_RUNTIME_APPLE)
  void InitMacProxy();
  void ReleaseMacProxy();

  id m_mac_proxy = NULL;       // RhinoLayerProxy object
#endif

};

class RHINO_SDK_CLASS CRhinoLayerTable : private ON_SimpleArray<CRhinoLayer*>
{
public:
  // Description:
  //   Layer tables store the list of layers in a
  //   Rhino document.
  //
  //  Returns:
  //    CRhinoDocument that owns this layer table.
  CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;


  // Description:
  //   Initializes an ON_Layer with the current default layer properties.
  //
  // Parameters:
  //   layer - [out] properties returned here
  //
  // Remarks:
  //   The default layer properties are:
  //
  //   @untitled table
  //   color              RhinoApp().AppSettings().AppearanceSettings.m_default_layer_color
  //   line style         RhinoApp().AppSettings().AppearanceSettings.m_default_layer_line_style
  //   material index     -1
  //   iges level         -1
  //   mode               ON::normal_layer
  //   name               empty
  //   layer index        0 (ignored by AddLayer)
  void GetDefaultLayerProperties( ON_Layer& layer ) const;

  // Returns:
  //   Number of layers in the layer table, including deleted
  //   layers.
  int LayerCount() const;

  // Description:
  //   Conceptually, the layer table is an array of LayerCount()
  //   layers.  The operator[] can be used to get individual
  //   layers.  A layer is either active or deleted and this
  //   state is reported by CRhinoLayer::IsDeleted().
  //
  // Parameters:
  //   layer_index - zero based array index
  //
  // Returns:
  //   Reference to the layer.  If layer_index is out of range,
  //   the current layer is returned. Note that this reference
  //   may become invalid after AddLayer() is called.
  const CRhinoLayer& operator[](
    int // layer_index
    ) const;

  const CRhinoLayer* Layer(
    const ON_ComponentManifestItem& item
    ) const;

  // Description:
  //   At all times, there is a "current" layer.  Unless otherwise
  //   specified, new objects are assigned to the current layer.
  //   The current layer is never locked, hidden, or deleted.
  //
  //  Returns:
  //    Zero based layer table index of the current layer.
  int CurrentLayerIndex() const;

  ON_UUID CurrentLayerId() const;

  // Description:
  //   At all times, there is a "current" layer.  Unless otherwise
  //   specified, new objects are assigned to the current layer.
  //   The current layer is never locked, hidden, or deleted.
  //
  // Parameters:
  //   layer_index - [in] value for new current layer.
  //       0 <= layer_index < LayerCount().  The layer's mode
  //       is automatically set to ON::normal_mode.
  //   bQuiet - [in] if true, then no warning message box
  //       pops up if the current layer request can't
  //       be satisfied.
  // Returns:
  //   true if current layer index successfully set.
  bool SetCurrentLayerIndex(
         int layer_index,
         bool bQuiet = false
         );

  // Description:
  //   At all times, there is a "current" layer.  Unless otherwise
  //   specified, new objects are assigned to the current layer.
  //   The current layer is never locked, hidden, or deleted.
  //
  // Returns:
  //   Reference to the current layer.  Note that this reference
  //   may become invalid after a call to AddLayer().
  //
  const CRhinoLayer& CurrentLayer() const;

  /*
  Description:
    Get the layer path name.
  Parameters:
    layer_index - [in]
    layer_path_name - [out]
  Returns:
    true: layer_index was valid and layer_path_name was set.
    false: layer_index was not valid.
  Remarks:
    If a layer does not have a parent, then its layer path name is
    identical to its layer name.  If a layer has a parent, then its layer
    path name is made by prepending names of parent layers separated
    by the string returned from ON_Layer::LayerNamePathDelimiter().
  */
  bool GetLayerPathName( 
    int layer_index,
    ON_wString& layer_path_name 
    ) const;

  bool GetLayerPathName( 
    int layer_index,
    const wchar_t* sLayerNamePathDelimeter,
    ON_wString& layer_path_name 
    ) const;

  /*
  Description:
    Find the layer with a specified parent and name.
  Parameters:
    parent_layer_id - [in]
      ON_nil_uuid for a root level layer (no parent)
    layer_name - [in]
      layer name
    not_found_return_value - [in]
      If no layer is found this value is returned.
  Returns:
    != not_found_return_value: index of the layer with the specified parent and name
    == not_found_return_value: no layer has the specified parent and name
  Remarks:
    Layers with different parents may have identical names.
  */
  int FindLayerFromParentAndName(
      ON_UUID parent_layer_id,
      const wchar_t* layer_name,
      int not_found_return_value
      ) const;

  /*
  Description:
    Finds a layer with the given name.
  Parameters:
    layer_index - [in]
      index of the first layer tested.
      ON_UNSET_INT_INDEX:
        The search with default layers
      0:
        The search will begin the first custom layer.
      >0:
        layer_table[i] is the first layer tested.
    layer_name - [in]
      name to search for
    not_found_return_value - [in]
      If no layer is found this value is returned.
  Returns:
    >=0     smallest index > layer_index of a layer the given name
    -1      no layer with index > layer_index has the given name
  */
  int FindNextLayerFromName(
        int layer_index,
        const wchar_t* layer_name,
        int not_found_return_value
        ) const;

  /*
  Description:
    Finds the layer from a layer path name.
  Parameters:
    layer_full_path_name - [in]
      name of layer to search for in the form
        "reference : alpha::beta::gamma"

      The "reference" portion of the name is present when a layer
      is from a reference model in a worksession. The delimiter 
      between the reference and the rest of the name is the string
      returned by 
        ON_Layer::LayerNameReferenceDelimiter() 
      
      The delimiter between the layer names is a names in the path
      is the string returned by
        ON_Layer::LayerNamePathDelimiter()

    not_found_return_value - [in]
      If no layer is found this value is returned.

  Returns:
    index of the layer with a matching name or not_found_return_value is no layer is found.
  */
  int FindLayerFromFullPathName(
    const wchar_t* layer_full_path_name,
    int not_found_return_value
    ) const;

  /*
  Description:
    Find a layer from a name that uniquely identifies a layer.
  Parameters:
    layer_name - [in] name of layer to find
    not_found_return_value - [in] value to return if no layer found
    multiple_found_return_value - [in] value to return if multiple layers found
  Returns:
    multiple_found_return_value:  two or more layers with the same name
    not_found_return_value:  no layer with specified name
    >=0:  index of the layer with the given name
  Example:
    If there are layers with indices and names:

      0: "alpha"
      1: "beta"
      2: "beta::alpha"
      3: "beta::gamma"
      4: "beta::gamma::pi"
      5: "beta::gamma::omega"
      5: "beta::gamma::omega::pi"

    Then 
      FindLayerFromUniqueName( "alpha" ) returns 0.
      FindLayerFromUniqueName( "beta::alpha" ) returns 2.
      FindLayerFromUniqueName( "gamma" ) returns 3.
      FindLayerFromUniqueName( "pi" ) returns -2. (multiple possible answers)
      FindLayerFromUniqueName( "beta::gamma::pi" ) returns 4.
      FindLayerFromUniqueName( "beta::gamma::omega::pi" ) returns 5.
      FindLayerFromUniqueName( "sigma" ) returns -1. (no matches)
      FindLayerFromUniqueName( "" ) returns -1. (no matches)
      FindLayerFromUniqueName( null ) returns -1. (no matches)
  */
  int FindLayerFromUniqueName(
    const wchar_t* layer_name,
    int not_found_return_value,
    int multiple_found_return_value
    ) const;

  /*
  Description:
    Find a layer from a name that uniquely identifies a layer.
  Parameters:
    layer_name - [in] - name of layer to find
    bIgnoreDeletedLayers - [in] - parameter is unused
  Returns:
    -2:  two or more layers with the same name
    -1:  no layer with specified name
    >=0:  index of the layer with the given name
  Example:
    If there are layers with indices and names:

      0: "alpha"
      1: "beta"
      2: "beta::alpha"
      3: "beta::gamma"
      4: "beta::gamma::pi"
      5: "beta::gamma::omega"
      5: "beta::gamma::omega::pi"

    Then
      FindLayerFromUniqueName( "alpha" ) returns 0.
      FindLayerFromUniqueName( "beta::alpha" ) returns 2.
      FindLayerFromUniqueName( "gamma" ) returns 3.
      FindLayerFromUniqueName( "pi" ) returns -2. (multiple possible answers)
      FindLayerFromUniqueName( "beta::gamma::pi" ) returns 4.
      FindLayerFromUniqueName( "beta::gamma::omega::pi" ) returns 5.
      FindLayerFromUniqueName( "sigma" ) returns -1. (no matches)
      FindLayerFromUniqueName( "" ) returns -1. (no matches)
      FindLayerFromUniqueName( null ) returns -1. (no matches)
  */
  ON_DEPRECATED_MSG("use overload that accepts not_found_return_value and multiple_found_return_value parameters")
  int FindLayerFromUniqueName(
    const wchar_t* short_name,
    bool bIgnoreDeletedLayers = true
    ) const;


  /*
  Description:
    Search for a layer with a matching name.
  Parameters:
    layer_name - [in]
    bSearchDefaultLayers - [in]
      If true, the system default layers (DefaultLayer) are included in the search.
      If you are searching for a layer to use as it exists,
      bSearchDefaultLayers should be true.
      If you are searching for a layer to modify, bSearchDefaultLayers
      should be false because the default layers cannot be modified.
    bSearchFullPathNames - [in]
      true:
        search for the full path name parent_name::...:leaf_name that uniquely identify a layer.
      false:
        search layer names (when the layer tree contains child nodes, there may be
        multiple layers with the same name.
    not_found_return_value - [in]
      Value to return if no layer has a matching name.
      Depending on what should happen if the name is not found,
      CurrentLayerIndex(), DefaultLayer.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
    multiple_found_return_value - [in]
      Value to return if bSearchFullPathNames is false and two or more layers have a matching name.
      Depending on what should happen if the name is not found,
      CurrentLayerIndex(), DefaultLayer.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
  Returns:
    The index of the matching layer is returned.
    If no match is found, then not_found_return_value is returned.
    If multiple matches are found, then multiple_match_return_value is returned.
  Example:
    If bSearchFullPathNames is falues and there are layers with indices and names:

      0: "alpha"
      1: "beta"
      2: "beta::alpha"
      3: "beta::gamma"
      4: "beta::gamma::pi"
      5: "beta::gamma::omega"
      5: "beta::gamma::omega::pi"

    Then
      FindLayerFromUniqueName( "alpha" ) returns 0.
      FindLayerFromUniqueName( "beta::alpha" ) returns 2.
      FindLayerFromUniqueName( "gamma" ) returns 3.
      FindLayerFromUniqueName( "pi" ) returns multiple_found_return_value. (multiple possible answers)
      FindLayerFromUniqueName( "beta::gamma::pi" ) returns 4.
      FindLayerFromUniqueName( "beta::gamma::omega::pi" ) returns 5.
      FindLayerFromUniqueName( "sigma" ) returns not_found_return_value. (no matches)
      FindLayerFromUniqueName( "" ) returns not_found_return_value. (no matches)
      FindLayerFromUniqueName( null ) returns not_found_return_value. (no matches)

  */
  int FindLayerFromName(
    const wchar_t* layer_name,
    bool bSearchDefaultLayers,
    bool bSearchFullPathNames,
    int not_found_return_value,
    int multiple_match_return_value
    ) const;

  /*
  Description:
    When user interface code is using a GetString to find a layer
    name, set up the getter and then call FindLayer(CRhinoGetString&).
  Parameters:
    gs - [in]
      gs.GetString() is called to get the layer name.  Set up options,
      prompts, and so on before calling FindLayer(gs,...)
    nothing_layer_index - [in]
      If >= 0, then this index is used if gs returns CRhinoGet::nothing.
    bVerbose - [in]
      If true and the layer is not found or two or more layers have the
      same name, then RhinoApp().Print() is used to print a message
      in the command feedback window.
  Returns:
    -5:  gs.CommandResult() != CRhinoCommand::success
    -4:  gs.Result() != CRhinoGet::string
    -3:  empty string entered
    -2:  two or more layers with the same name
    -1:  no layer with specified name
   >=0:  index of the layer with the given name
  */
  int FindLayerFromGetString(
    class CRhinoGetString& gs,
    int nothing_layer_index,
    bool bVerbose
    ) const;

  /*
  Description:
    Search for a layer with a matching id.
  Parameters:
    layer_id - [in]
    bSearchDefaultLayers - [in]
      If true, the system default layers (DefaultLayer) are included in the search.
      If you are searching for a layer to use as it exists,
      bSearchDefaultLayers should be true.
      If you are searching for a layer to modify, bSearchDefaultLayers
      should be false because the default layers cannot be modified.
    bSearchDeletedDeletedLayers - [in]
      If true, deleted layers are included in the search.
    not_found_return_value - [in]
      Value to return if no layer has a matching id.
      Depending on what should happen if the id is not found,
      CurrentLayerIndex(), DefaultLayer.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
  Returns:
    The index of the matching layer is returned.
    If no match is found, then not_found_return_value is returned.
  */
  int FindLayerFromId(
    ON_UUID layer_id,
    bool bSearchDefaultLayers,
    bool bSearchDeletedLayers,
    int not_found_return_value
    ) const;



  // Description:
  //   Adds a new layer with specified definition to the layer table.
  //
  // Parameters:
  //   layer - [in] definition of new layer.  The information
  //       in layer is copied.  If layer.LayerName() is empty
  //       the a unique name of the form "Layer 01" will be
  //       automatically created.
  //
  //   bReference - [in] true if this layer is a reference layer.
  //       Reference layers are not saved in files.
  //
  // Returns:
  //   @untitled table
  //   >=0     index of new layer
  //   -1      layer not added because a layer with that name
  //           already exists.
  //
  // Remarks:
  //   In some cases, calling AddLayer() can cause the memory 
  //   references previously returned by operator[] and 
  //   CurrentLayer() to become invalid.
  //
  int AddLayer( 
     const ON_Layer& layer,  // layer
     bool bReference = false // bReference
     );

  /*
  Description:
    Basic tool used by the add layer functions.
  Parameters:
    layer - [in]
      settings for new layer.  These are copied to the layer table entry
      and any user data on layer is moved to the layer table entry.
    layer_type - [in]
      0: ordinary layer
      2: linked instance definition "filename.3dm" parent layer of 
         the instance defintion's layer tree
      4: worksession reference model "filename.3dm" parent layer of
         the reference model's layer tree
    worksession_ref_model_sn - [in]
          0: layer is not a reference layer
          1: layer is a reference layer but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number      
    linked_idef_sn - [in]
          0: layer is not from a liked instance definition
     1-1000: reserved for future use
      >1000: linked instance definition serial number      
  */
  int CreateLayer( 
     const ON_Layer& layer,
     unsigned int layer_type,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );

  // Description:
  //   Adds a new layer with default definition to the layer table.
  //
  // Parameters:
  //   bReference - [in] true if this layer is a reference layer.
  //       Reference layers are not saved in files.
  //
  // Returns:
  //   index of new layer
  //
  // Remarks:
  //   In some cases, calling AddLayer() can cause the memory 
  //   references previously returned by operator[] and 
  //   CurrentLayer() to become invalid.
  //
  int AddLayer( 
     bool bReference = false
     );

  /*
  Description:
    Create a new layer with a specified name and color.
  Parameters:
    layer_name - [in]
      If the name is nullptr or an empty string, an unused layer name
      is automatically used.
    layer_color - [in]
      If layer_color is ON_Color::UnsetColor, the default layer color is used
    bReturnExistingLayer - [in]
      If there is a layer named layer_name and bReturnExistingLayer is true,
      then the index of the existing layer is returned.
      If there is a layer named layer_name and bReturnExistingLayer is false,
      then the ON_UNSET_INT_INDEX is returned.
  Returns:
    >= 0: index of the layer
    ON_UNSET_INT_INDEX: 
      layer could not be added because layer_name was invalid or
      a layer named layer_name and bReturnExistingLayer was false.
  */
  int AddLayer(
    const wchar_t* layer_name,
    ON_Color layer_color,
    bool bReturnExistingLayer
  );


  // Description:
  //   Modify layer settings
  //
  // Parameters:
  //   layer - [in] new settings.  This information is copied.
  //   layer_index - [in] zero based index of layer to set.  This
  //        must be in the range 0 <= layer_index < LayerCount().
  //   bQuiet - [in] if true, information message boxes pop up
  //       when illegal changes are attempted.
  //
  // Returns:
  //   true if successful. false if layer_index is out of range
  //   or the settings attempt
  //   to lock or hide the current layer.
  //
  bool ModifyLayer( 
     const ON_Layer& layer,
     int layer_index,
     bool bQuiet = false
     );

  // Description:
  //   If the layer has been modified and the modification can be undone,
  //   then UndoModify() will restore the layer to its previous state.
  //
  // Returns:
  //   true if this layer had been modified and the modifications were
  //   undone.
  //
  // See Also: IsModified.
  bool UndoModifyLayer(
     int layer_index,
     unsigned int ur_sn = 0
     );

  // Description:
  //   Deletes layer
  //
  // Parameters:
  //   layer_index - [in] zero based index of layer to delete.
  //       This must be in the range 0 <= layer_index < LayerCount().
  //   bQuiet - [in] If true, no warning message box appears if a layer
  //       the layer cannot be deleted because it is the current layer
  //       or it contains active geometry.
  //
  // Returns:
  //   true if successful. false if layer_index is out of range or the
  //   the layer cannot be deleted because it is the current layer or
  //   because it layer contains active geometry.
  bool DeleteLayer(
     int layer_index,
     bool bQuiet
     );

  /*
  Description:
    Deletes multiple layers.
  Parameters:
    layer_index_count - [in] length of layer_index[] array.
    layer_index_list  - [in] array zero based indices of layers to delete.
                             The indices must be in the range 0 <= layer_index < LayerCount().
    bQuiet            - [in] If true, no warning message box appears if a layer
                             cannot be deleted because it is the current layer,
                             contains active geometry, and so on.
  Returns:
    Number of layers that were deleted.  If this is < layer_index_count,
    you can dig through the layer table and check which ones were
    not deleted.
  */
  int DeleteLayers(
     int layer_index_count,
     const int* layer_index_list,
     bool bQuiet
     );

  /*
  Description:
    Deletes multiple layers.
  Parameters:
    layer_index_count      - [in] length of layer_index_list[] array.
    layer_index_list       - [in] array zero based indices of layers to delete.
                             The indices must be in the range 0 <= layer_index < LayerCount().
    bQuiet                 - [in] If true, no warning message box appears if a layer
                             cannot be deleted because it is the current layer,
                             contains active geometry, and so on.
    delete_objects_warning - [in] Action to take when multiple delete warnings occur, where:
                             0 == No to all, 1 == Yes to all, 2 == Ask the user.
  Returns:
    Number of layers that were deleted.  If this is < layer_index_count,
    you can dig through the layer table and check which ones were
    not deleted.
  */
  int DeleteLayers(
    int layer_index_count,
    const int* layer_index_list,
    bool bQuiet,
    int delete_objects_warning
  );

  // Description:
  //   Undeletes a layer that has been deleted by DeleteLayer().
  //
  // Parameters:
  //   layer_index - [in] zero based index of layer to undelete.
  //       This must be in the range 0 <= layer_index < LayerCount().
  //
  // Returns:
  //   true if successful.
  bool UndeleteLayer(
     int layer_index
     );

  // Description:
  //   Use this to set CRhinoLayer::m_sort_index so that
  //   the values of m_sort_index run from 0 to LayerCount()-1
  //   and compare(layer_table[i],layer_table[j]) < 0 ) implies 
  //   layer_table[i].m_sort_index < layer_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoLayer* arg1,const CRhinoLayer* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //       If you want to preserve the existing sort order, pass null
  //       for the compare function.
  //   cull - [in] optional filter function with prototype
  //       bool cull(const CRhinoLayer* arg,void* p).
  //       that returns true if the layer should be ignored
  //       when sorting.  Layers that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull layers
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoLayer pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the layer table.  They are intended to be used to get
  //   sorted lists of layers for dialogs, etc.
  void Sort( 
    int (*compare)(const CRhinoLayer*,const CRhinoLayer*,void*),
    int (*cull)(const CRhinoLayer*,void*),
    void* p=0
    );

  // Description:
  //   Gets an array of pointers to layers that is sorted by
  //   the values of CRhinoLayer::m_sort_index.
  //
  // Parameters:
  //   sorted_list - [out] this array is returned with length
  //       LayerCount() and is sorted by the values of
  //       CRhinoLayer::m_sort_index.
  //   bIgnoreDeleted - [in] true means don't include
  //       deleted layers.
  //
  // Remarks:
  //   Use Sort() to set the values of m_sort_index.
  void GetSortedList(
    ON_SimpleArray<const CRhinoLayer*>&, // sorted_list
    bool = false // bIgnoreDeleted
    ) const;

  void GetSortedList(
    bool bIgnoreDeleted,  // Ignore deleted layers
    bool bIgnoreFiltered, // Ignore filtered layers
    ON_SimpleArray<const CRhinoLayer*>& sorted_list
  ) const;

  /*
  Description:
    Sort layers, in the layer table, by their name in either ascending or descending order.
    This function sorts layers in the same manner as Rhino's Layers panel.
  Parameters:
    bAscending - [in] Sort in ascending (true) or descending (false) order.
  Remarks:
    This function changes the value of CRhinoLayer::m_sort_index.
    It does not change the index order of layers in the layer table.
  */
  void SortByLayerName(bool bAscending);

  /*
  Description:
    Gets unused layer name used as default when creating new layers.
   Parameters:
    root_name - [in]
      The returned name is "root_name nn"  If root_name is empty,
      then "Layer" (localized) is used.
    layer_name - [out]
      A valid layer name that is not in use.
    bIgnoreDeleted - [in] 
      When bIgnoreDeleted is true, then the returned name may
      be used by a deleted layer.
 */
  void GetUnusedLayerName(
    ON_wString& unused_layer_name
    ) const;

  void GetUnusedLayerName( 
    const wchar_t* root_name,
    ON_wString& unused_layer_name
    ) const;

  bool SetUnusedLayerName(
    const wchar_t* root_name,
    ON_Layer& layer
    ) const;

  void SetRemapIndex(
    int layer_index,
    int remap_index
    );

  int RemapIndex(
    int layer_index
    ) const;

  void ListTable(
    ON_TextLog& text_log,
    unsigned int level_of_detail
    ) const;

  /*
  Returns:
    true 
      The layer is an element in this layer table.
  */
  bool InLayerTable( const ON_Layer* layer ) const;

  /*
  Returns:
    true 
      The layer_index is a valid index of a layer
      element in this layer table.    
  */
  bool IsValidLayerTableIndex( int layer_index ) const;

  void ChangeLayerSortOrder(const ON_SimpleArray<ON_2dex>& sort_order);

private:
  friend class CRhinoDoc;
  friend class CRhinoRead3dmHelper; // for access to CRhinoLayerTable::FindReferenceParentLayer()

  // Constructs a layer table with one default layer.  This is private
  // because only CRhinoDoc is permitted to create or destroy layer
  // tables.
  CRhinoLayerTable(CRhinoDoc&);
  ~CRhinoLayerTable();
  
  // pointer to parent doc set when CRhinoDoc creates the layer table.
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

  ON_ComponentManifest& Internal_Manifest();


  // index of current layer
  // SetCurrentLayerIndex() keeps this value in sync with
  // CRhinoDoc::Properties().Settings().m_current_layer_index
  int m_current_layer_index = -1;

private:
  friend bool RhGetGrandParentLayerId( 
          CRhinoDoc&,
          unsigned int,
          unsigned int,
          unsigned int,
          const wchar_t*,
          const wchar_t*,
          ON_UUID*
          );
  /*
  Description:
    Low level layer table helper function.  
    Do not make this function public because it will change
    from time to time.
  Parameters:
    layer_type - [in]
      2: parent of linked xref file's layer table
         The linked_idef_sn parameter must be a valid linked idef
         serial number.  If the linked idef is in a parent linked
         idef, then parent_linked_idef_sn must be specified.
         If the linked idef is in a worksession reference model, 
         then worksession_reference_model_sn must be specified.
      4: parent of worksession reference model file's layer table
         The worksession_reference_model_sn parameter must be a valid
         worksession reference model serial number.  The 
         parent_linked_idef_sn and linked_idef_sn values must be zero.
    worksession_reference_model_sn - [in]
      zero or a valid worksession reference model serial number.  When
      set, it indicates the layer is associated with the specified
      worksession reference model.
    parent_linked_idef_sn - [in]
      zero or a valid linked idef serial number.  When set, it 
      indicates you are finding/creating a layer for a linked
      idef that is nested in the idef identified by parent_linked_idef_sn.
    linked_idef_sn - [in]
      zero or a valid linked idef serial number.  When set, it 
      indicates you are finding/creating a layer for a linked
      idef. If this linked idef is in a worksession reference model,
      then worksession_reference_model_sn must be specified.
      If this linked idef is nested in a parent linked idef,
      then parent_linked_idef_sn must be specified.
    bCreateIfNotFound - [in]
      If true, the layer will be created if it cannot be found.
    sLayerName - [in]
      root name to use if the layer must be created.  This parameter
      is ignored when searching for existing layers.  If the layer
      name is in use, it will be decorated with a suffix to make
      a unique layer name.
    layer_settings - [in]
      layer settings (color, parent, visibility, ...) to use
      if the layer must be created.
  Returns:
    -1: layer not found (and could not be created)
   >=0: layer table index.
  */
  int FindReferenceParentLayer( 
        unsigned int layer_type,
        unsigned int worksession_reference_model_sn,
        unsigned int parent_linked_idef_sn,
        unsigned int linked_idef_sn,
        bool bCreateIfNotFound,
        const wchar_t* sLayerName,
        const ON_Layer* layer_settings
        );

private:
  // Delete all CRhinoLayer.m_undo information
  void ClearUndoRecords();

  void Destroy( class CRhinoEventWatcher* ew );
  void Destroy();

  unsigned int PurgeWorkSessionReferenceModel( 
    unsigned int worksession_ref_model_sn
    );

private:
  CRhinoLayer m_DefaultLayer;

public:
  /*
  Description:
    CRhinoLayer::DefaultLayer is returned when an invalid index is
    passed to operator[] and in other emergencies when a valid reference
    to a layer is needed and there is there is not a layer in the table that
    can be used.

    This is not the layer with the English name "Default".
    CRhinoLayer::DefaultLayer is never saved in files.

      CRhinoLayer::DefaultLayer.Index() = -1
      CRhinoLayer::DefaultLayer.Id() = nil,
      CRhinoLayer::DefaultLayer.Name() = ""
  */
  const CRhinoLayer& DefaultLayer;

  bool UnsortLayer(int layer_index, int sort_index);

private:
  CRhinoLayerTable() = delete;
  CRhinoLayerTable(const CRhinoLayerTable&) = delete;
  CRhinoLayerTable& operator=(const CRhinoLayerTable&) = delete;

private:
  bool AddUndoModifyLayerRecord(int layer_index);

#if defined (ON_RUNTIME_APPLE)
public:
  // create new layer after specified sibling, return index of new layer
  int AddLayerAfterSibling (int sibling_index);
  // create new sublayer for parent, return index of new layer
  int AddSublayer (int parent_index);
#endif
};


/*
Description:
  Duplicates one or more layer and, optionally, all of the objects on the layer.
Parameters:
  doc                 [in] - The document.
  in_layers           [in] - The indices of one or more layers to duplicate.
  bDuplicateObjects   [in] - If true, then layer objects will also be duplicated.
  bDuplicateSublayers [in] - If true, then a layer's sublayers will also be duplicated.
  out_layers          [out] - The indices of the newly added layers.
Returns:
  The number of layer indices added to out_layers.
*/
RHINO_SDK_FUNCTION
int RhinoDuplicateLayers(
  CRhinoDoc& doc,
  const ON_SimpleArray<int>& in_layers,
  bool bDuplicateObjects,
  bool bDuplicateSublayers,
  ON_SimpleArray<int>& out_layers
);


ON_DEPRECATED_MSG("use overload that takes a CRhinoDoc parameter")
RHINO_SDK_FUNCTION
void RhinoRedrawLayerManagerWindow();

/*
Description:
  Redraws the layer manager window, if visible.
Parameters:
  doc - [in] The Rhino document.
Returns:
  Nothing.
Remarks:
  When adding lots of layers to the layer table, if the layer manager window is open,
  the process can be time consuming. Each addition to the layer table triggers a
  CRhinoEventWatcher::layer_added event, which in turn causes the layer manager window
  to repaint. To add bulk layers expeditiously, you will want to:
    1.) Turn off viewport redrawing by calling CRhinoView::EnableDrawing(false).
    2.) Add your bulk layers.
    3.) Turn viewport redrawing back on by calling CRhinoView::EnableDrawing(true).
    4.) Redraw the layer manager window by calling RhinoRedrawLayerManagerWindow(doc).
*/
RHINO_SDK_FUNCTION
void RhinoRedrawLayerManagerWindow(CRhinoDoc& doc);


ON_DEPRECATED_MSG("use overload that takes a CRhinoDoc parameter")
RHINO_SDK_FUNCTION
int RhinoLayerManagerSelectedLayers(ON_SimpleArray<int>& layers);

/*
Description:
  If the layer manager window is visible, then the function returns the
  layer indices of the selected layer items in the list.
Parameters:
  doc    - [in] The Rhino document.
  layers - [out] The layer indices of the selected layer items.
Returns:
  >= 0 - The number of indices added to the output array.
  -1 - If the layer manager window is not visible, or on error.
*/
RHINO_SDK_FUNCTION
int RhinoLayerManagerSelectedLayers(CRhinoDoc& doc, ON_SimpleArray<int>& layers);


ON_DEPRECATED_MSG("use overload that takes a CRhinoDoc parameter")
RHINO_SDK_FUNCTION
int RhinoLayerManagerSelectLayers(ON_SimpleArray<int> layers, bool bDeselect);

/*
Description:
  If the layer manager window is visible, then the function selects
  layer items, specified by the layer indices, in the list.
Parameters:
  doc       - [in] The Rhino document.
  layers    - [in] The indices of the layers to select.
  bDeselect - [in] If true, then any previously selected layers will be unselected.
Returns:
  >= 0 - The number layer items that were selected.
  -1 - If the layer manager window is not visible, or on error.
*/
RHINO_SDK_FUNCTION
int RhinoLayerManagerSelectLayers(CRhinoDoc& doc, const ON_SimpleArray<int>& layers, bool bDeselect);


/*
Description:
  Provides access to the document's saved layer states.
*/ 
class RHINO_SDK_CLASS CRhinoLayerState
{
public:
  enum class LayerProperty : unsigned int
  {
    None = 0x0,
    Current = 0x1,
    Visible = 0x2,
    Locked = 0x4,
    Color = 0x8,
    Linetype = 0x10,
    PrintColor = 0x20,
    PrintWidth = 0x40,
    ViewportVisible = 0x80,
    ViewportColor = 0x100,
    ViewportPrintColor = 0x200,
    ViewportPrintWidth = 0x400,
    RenderMaterial = 0x800,
    Unused = 0x1000,
    All = 0xFFFFFFFF
  };

  /*
  Description:
    Returns the number of layer states saved in the document.
  Parameters:
    doc - [in] The document.
  Returns:
    The number of layer states.
  */
  static int Count(CRhinoDoc& doc);

  /*
  Description:
    Gets the names of the layer states saved in the document.
  Parameters:
    doc - [in] The document.
    state_names - [out] The names of the layer states.
  Returns:
    true if successful, false otherwise.
  */
  static bool Names(CRhinoDoc& doc, ON_ClassArray<ON_wString>& state_names);

  /*
  Description:
    Saves a new layer state to the document. 
  Parameters:
    doc - [in] The document.
    state_name - [in] The name of the layer state to save.
    viewport_id - [in] If saving the layer state of a layout or a details, then
                       the id of the layout or detail view's viewport. Otherwise,
                       just use ON_nil_uuid;
  Returns:
    The index of the layer state, -1 on failure.
  */
  static int Save(CRhinoDoc& doc, const wchar_t* state_name, const ON_UUID& viewport_id);

  /*
  Description:
    Restores a layer state saved in the document.
  Parameters:
    doc - [in] The document.
    state_name - [in] The name of the layer state to restore.
    properties - [in] The layer properties to restore.
    viewport_id - [in] If restoring the layer state to a layout or a details, then
                       the id of the layout or detail view's viewport. Otherwise,
                       just use ON_nil_uuid;
  Returns:
    true if successful, false otherwise.
  */
  static bool Restore(CRhinoDoc& doc, const wchar_t* state_name, CRhinoLayerState::LayerProperty properties, const ON_UUID& viewport_id);
  
  /*
  Description:
    Renames a layer state saved in the document.
  Parameters:
    doc - [in] The document.
    state_name - [in] The name of the layer state to rename.
    new_name - [in] The new name.
  Returns:
    true if successful, false otherwise.
  */
  static bool Rename(CRhinoDoc& doc, const wchar_t* state_name, const wchar_t* new_name);

  /*
  Description:
    Deletes a layer state saved in the document.
  Parameters:
    doc - [in] The document.
    state_name - [in] The name of the layer state to delete.
  Returns:
    true if successful, false otherwise.
  */
  static bool Delete(CRhinoDoc& doc, const wchar_t* state_name);

  /*
  Description:
    Finds a layer state saved in the document.
  Parameters:
    doc - [in] The document.
    state_name - [in] The name of the layer state to find.
  Returns:
    The index of the layer state, -1 on failure.
  */
  static int Find(CRhinoDoc& doc, const wchar_t* state_name);
  
  /*
  Description:
    Imports layer states from another .3dm file.
  Parameters:
    doc - [in] The document.
    filename - [in] The name of the .3dm file.
  Returns:
    The number of layer states imported from the file, -1 on failure.
  */
  static int Import(CRhinoDoc& doc, const wchar_t* filename);
};
