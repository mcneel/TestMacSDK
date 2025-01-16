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

/*
Description:
  This is a list of the actual geometry that CRhinoInstanceObjects
  reference.
*/
class RHINO_SDK_CLASS CRhinoInstanceDefinition : public ON_InstanceDefinition
{
public:
  void DestroyRuntimeCache( bool bDelete = true ) override;

  
private:
  /*
  Description:
    Use CRhinoDoc::PurgeCyclicInstanceReferences() to find and destroy
    cyclic instance references.

    This private helper is called by 
    CRhinoInstanceDefinition::Internal_PurgeCyclicInstanceReferences().
    
  Returns:
    Number of cyclic references that were purged.
    Any value > 0 indicates something added a cyclic reference.
  */
  unsigned int Internal_PurgeCyclicInstanceReferences(
    ON_SimpleArray<ON__UINT64>& idef_sn_stack
  );

public:

  /*
  Returns:
    SizeOf this and the geometry definition objects used
    by this CRhinoInstanceDefinition.
  */
  unsigned int SizeOf() const override;

  void Dump(
    ON_TextLog& text_log
    ) const override;

  class CRhinoInstanceDefinitionTable& m_idef_table;

  /*
  Returns:
    Number of objects this definition uses.
  Remarks:
    This counts the objects that are used to define the geometry.
    This does NOT count the number of references to this instance
    definition.
  See Also:
    CRhinoInstanceDefinition::GetReferences
    CRhinoInstanceDefinition::GetObjects
  */
  int ObjectCount() const;

  /*
  Parameters:
    index - [in] 0 <= index < ObjectCount()
  Returns:
    Pointer to an object used as part of this definition.
  Remarks:
    This returns an object that is used to define the geometry.
    This does NOT return an object that references this definition.count the number of references to this instance
  See Also:
    CRhinoInstanceDefinition::GetReferences
    CRhinoInstanceDefinition::GetObjects
  */
  const CRhinoObject* Object(int index) const;

  /*
  Description:
    Get a list of the objects that belong to this instance
    definition.  These objects are stored in the 
    CRhinoDoc::m_instance_geometry list.
  Parameters:
    objects - [out]
  Returns:
    number of objects in list.
  See Also:
    CRhinoInstanceDefinition::GetReferences
  */
  int GetObjects( 
         ON_SimpleArray<const CRhinoObject*>& objects
         ) const;

  /*
  Description:
    Get a list of the CRhinoInstanceObjects (inserts)
    that contains a reference this instance definition.
  Parameters:
    iref_object_list - [out]
    where_to_look - [in] 
        0 = get top level references in active document
            The irefs returned in iref_object_list[]
            will have ->Attributes().Model() = ON::normal_object,
            ON::hidden_object, or ON::locked_object.
        1 = get top level and nested references in active document
            The objects returned in iref_object_list[]
            will have ->Attributes().Model() = ON::normal_object,
            ON::hidden_object, or ON::locked_object.
        2 = check for references from other instance definitions
            The objects returned in iref_object_list[]
            will have ->Attributes().Model() = ON::idef_object.
  Returns:
    number of objects added to the list.
  See Also:
    CRhinoInstanceDefinition::GetObjects
  Remarks:
    There are two separate lists of geometry objects, the model space 
  */
  int GetReferences( 
         ON_SimpleArray<const CRhinoInstanceObject*>& iref_object_list,
         int where_to_look = 0
         ) const;

  /*
  Description:
    Get a list of all the CRhinoInstanceDefinitions 
    that contain a reference this instance definition.
  Parameters:
    idef_list - [out]
  Returns:
    number of objects added to the list.
  See Also:
    CRhinoInstanceDefinition::GetObjects
  */
  int GetContainers( 
         ON_SimpleArray<const CRhinoInstanceDefinition*>& idef_list
         ) const;

  /*
  Description:
    Determine if this instance definition contains a reference
    to another instance definition.
  Parameters:
    other_idef_index - [in] index of another instance definition
  Returns:
    @untitled table
      0      no
      1      other_idef_index is the index of this instance definition
     >1      This CRhinoInstanceDefinition uses the instance definition
             and the returned value is the nesting depth.
  */
  int UsesDefinition( int other_idef_index ) const;

  /*
  Parameters:
    idef_index - [in] index of another instance definition
  Returns:
    Number of nested irefs that reference the "other" instance definition.
  Remarks:
    DefinitionUseCount( this->Index() ) returns 0.
  */
  int NestedDefinitionUseCount( int other_idef_index ) const;
  
  /*
  Parameters:
    where_to_look - [in] 
        0 = check for top level references in active document
        1 = check for top level and nested references in active document
        2 = check for references in other instance definitions
  Returns:
    True if the instance definition is referenced.
  See Also:
    CRhinoInstanceDefinition::GetReferences
  */
  bool InUse( int where_to_look ) const;

  // Runtime index used to sort these in the dialog
  int m_sort_index = ON_UNSET_INT_INDEX;   

  // Runtime index used when remapping for import/export
  int m_remap_index = ON_UNSET_INT_INDEX;

  /*
  Returns
    True: 
      This block definition is a sub-block in a linked block.
  */
  bool IsInLinkedBlockReferenceModel() const;

  /*
  Returns
    True:
      This block definition is from a worksession reference
      model.
  */
  bool IsInWorksessionReferenceModel() const;

  bool IsTenuous() const;

  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const;

  /*
  Description:
    Returns true if some object in the instance definition
    is on the layer with index layer_index.
  Parameters:
    layer_index - [in] index of layer to search for
  Returns:
    True if there is an object in the instance definition
    that is on the layer.
  */
  bool UsesLayer( 
        int layer_index 
        ) const;

  /*
  Description:
    Returns true if some object in the instance definition
    references the linetype with index linetype_index.
  Parameters:
    linetype_index - [in] index of linetype to search for
  Returns:
    True if there is an object in the instance definition
    that references that linetype.
  */
  bool UsesLinetype( int linetype_index) const;

  /*
  Description:
    Change all of the objects in the block to use continuous linetype
    instead of the one in the linetype table at linetype_index
    Does not change layers referenced by the block even if they 
    reference the linetype.
    Doesn't remove references to table[0] (continuous)
  Parameters:
    linetype_index - [in] index of linetype to search for
  Returns:
    true: success
    false: failure
  */
  bool RemoveLinetypeReference( int linetype_index);

#if defined (ON_RUNTIME_APPLE)
  id MacProxy() const;
#endif

public:
  // runtime serial number
  const unsigned int m_idef_runtime_sn;
  
  static unsigned int NextRuntimeSerialNumber();
private:
  friend class CRhinoInstanceDefinitionTable;
  friend class CRhinoInstanceObject;
  friend class CRhinoDoc;
  friend class CRhinoRead3dmHelper;
  friend class CRhInstanceDefinitionPrivate;

  bool m_bIsTenuous = false;
                       // true if this idef is from a linked file.
                       // It will be saved only if it is referenced
                       // by an iref that is in the active geometry
                       // list and is being saved.  Note that there
                       // can be multiple linked idefs with references
                       // to tenuous idefs.

  // Runtime list of pointers to CRhinoObjects in either the
  // CRhinoDoc::m_active_model_idef_geometry list or the
  // CRhinoDoc::m_reference_model_idef_geometry list.
  // The objects are listed in the same order as the 
  // m_instance_def.m_object_uuid[] array.  Never directly
  // append a pointer to this array or remove a pointer from
  // this array.  Use AddIdefGeometryObjectHelper() and
  // RemoveIdefGeometryObjectHelper() so reference counting
  // is properly handled.
  ON_SimpleArray<const CRhinoObject*> m_object_pointer;

  // Verifies the object is idef geometry, and makes
  // appropriate additions to the m_object_uuid[]
  // and m_object_pointer[] arrays.
  bool AddIdefGeometryObjectHelper(const CRhinoObject*);
  bool RemoveIdefGeometryObjectHelper(int i);

  class CRhInstanceDefinitionUndo* m_undo = nullptr;

private:
  // Returns 1 or 2 if undo information for the specified event
  // was found and deleted.  Returns 2 if deleting the undo information
  // may leave objects in the m_active_model_idef_geometry list
  // with no references.
  int CullUndoEventHelper(unsigned int undo_event_sn); 

private:
  CRhinoInstanceDefinition(class CRhinoInstanceDefinitionTable&,
                           ON_UUID idef_uuid,
                           int idef_index
                           );
  ~CRhinoInstanceDefinition();

#if defined (ON_RUNTIME_APPLE)
  id m_mac_proxy;     // pointer to MRInstanceDefinition
  
  void ReleaseMacProxy();
  void InitMacProxy();
#endif
  
private:
  // no implementation
  CRhinoInstanceDefinition() = delete;
  CRhinoInstanceDefinition(const CRhinoInstanceDefinition&) = delete;
  CRhinoInstanceDefinition& operator=(const CRhinoInstanceDefinition&) = delete;

private:
  mutable CRhinoCacheHandle m_sdk_reserved_cache;
  class CRhInstanceDefinitionPrivate* m_private = nullptr;
  ON__UINT_PTR m_sdk_reserved = 0;
};

class RHINO_SDK_CLASS CRhinoInstanceDefinitionTable : private ON_SimpleArray<CRhinoInstanceDefinition*>
{
public:
  CRhinoInstanceDefinitionTable( CRhinoDoc& );

  CRhinoDoc& Document() const;
  const class CRhinoDocTableReference& TableReference() const;


private:
  /*
  Description:
    Use CRhinoDoc::PurgeCyclicInstanceReferences() to find and destroy
    cyclic instance references.

    This private helper is Called by CRhioDoc::PurgeCyclicInstanceReferences().

  Returns:
    Number of cyclic references that were purged.
    Any value > 0 indicates something added a cyclic reference.
  */
  unsigned int Internal_PurgeCyclicInstanceReferences();

private:
  /*
  Description:
    Layer tables store the list of layers in a
    Rhino document.
 
   Returns:
     CRhinoDocument that owns this layer table.
  */
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

public:
  /*
  Returns:
    Number of items in the instance definitions table, 
    including deleted layers.
  */
  int InstanceDefinitionCount() const;

  int Count() const; // OBSOLETE - USE InstanceDefinitionCount.

  /*
  Description:
    Conceptually, the instance definition table is an array
    of InstanceDefinitionCount() elements.
    The operator[] can be used to get individual
    instance definitions. 
  Parameters:
    idef_index - [in]
      zero based array index
  Returns:
    Pointer to the instance definition.  This pointer may be null.
  */
  const CRhinoInstanceDefinition* operator[]( int idef_index ) const;

  /*
  Description:
    Find the instance definition that has the specified
    serial number.
  Parameters:
    sn - [in]
      runtime serial number
  Returns:
    Pointer to the instance definition.  This pointer may be null.
  */
  const CRhinoInstanceDefinition* InstanceDefinition( unsigned int sn ) const;

  /*
  Description:
    Finds the instance definition with a given name.
 
  Parameters:
    idef_name - [in] name of instance definition to search for.
        The search ignores case.

  Returns:
    >=0     index of the instance definition with the given name
    -1      no instance definition has the given name
  */
  int FindInstanceDefinition( 
        const wchar_t* idef_name
        ) const;

  /*
  Description:
    Searches for an instance definition the is in some way linked
    to a file.
 
  Parameters:
    index0 - [in]
      Searching the idef table begins at this index.
      Pass 0 to search the entire table.  This is used
      to continue searching after finding the first idef
      to be linked to a file.
    file_name - [in] 
      name of a file. The search ignores case.
    bRelativePath - [in]
      true if file_name is relative to the current document's location
     bIgnoreDeletedInstanceDefs - [in] true means don't search
        deleted instance definitions.
   Returns:
    >=0     index of the instance definition linked to a file.
    -1      no instance definition has the given name
  */
  int FindLinkedInstanceDefinition(
        int index0,
        const wchar_t* file_name,
        bool bRelativePath,
        bool bIgnoreDeletedInstanceDefs
        ) const;


  /*
  Description:
    Finds the instance definition with a given name.
 
  Parameters:
    idef_uuid - [in] Unique id of the instance definition to
        search for. The search ignores case.
 
    bIgnoreDeletedInstanceDefs - [in] true means don't search
        deleted instance definitions.
 
  Returns:
    >=0     index of the instance definition with the given name
    -1      no instance definition has the given name
  */
  int FindInstanceDefinition( 
        ON_UUID idef_uuid,
        bool bIgnoreDeletedInstanceDefs = true
        ) const;

  /*
  Description:
    Finds the instance definition with a given name.
 
  Parameters:
    idef_runtime_sn - [in] 
        value of CRhinoInstanceDefinition::m_idef_runtime_sn
 
    bIgnoreDeletedInstanceDefs - [in] true means don't search
        deleted instance definitions.
 
  Returns:
    >=0     index of the instance definition with the given name
    -1      no instance definition has the given name
  */
  int FindInstanceDefinition( 
        unsigned int idef_runtime_sn,
        bool bIgnoreDeletedInstanceDefs = true
        ) const;

  /*
  Description:
    Add an instance definition to the instance definition table.

  Parameters:
    idef - [in] the UUID in idef is used to find the objects
    bReference - [in] true if this is from a reference file
    bQuiet - [in] true if errors should be silent

  Returns:
     -1   failure
    >=0   index of instance definition in the instance definition table.
  */
  int AddInstanceDefinition( 
          const ON_InstanceDefinition& idef, 
          bool bReference = false,
          bool bQuiet = false
          );
  /*
  Description:
    Add an instance definition to the instance definition table.

  Parameters:
    idef - [in] the UUID in idef is ignored.
    object - [in] will be moved to instance geometry list
    bReference - [in] true if this is from a reference file
    bQuiet - [in] true if errors should be silent

  Returns:
     -1   failure
    >=0   index of instance definition in the instance definition table.
  */
  int AddInstanceDefinition( 
          const ON_InstanceDefinition& idef, 
          const CRhinoObject* object,
          bool bReference = false,
          bool bQuiet = false
          );

  /*
  Description:
    Add an instance definition to the instance definition table.

  Parameters:
    idef - [in] The UUID in idef is ignored.
    objects - [in] will be moved to instance geometry list
    bReference - [in] true if this is from a reference file
    bQuiet - [in] true if errors should be silent

  Returns:
     -1   failure
    >=0   index of instance definition in the instance definition table.
  */
  int AddInstanceDefinition( 
          const ON_InstanceDefinition& idef, 
          const ON_SimpleArray<const CRhinoObject*>& objects,
          bool bReference = false,
          bool bQuiet = false
          );
  int AddInstanceDefinition( 
          const ON_InstanceDefinition& idef, 
          const ON_SimpleArray<const CRhinoObject*>& objects,
          unsigned int worksession_ref_model_sn,
          unsigned int linked_idef_sn,
          bool bQuiet
          );

  /*
  Parameters:
    source_idef_index - [in]
      index of a block definition with 
      ON_InstanceDefinition::IDEF_UPDATE_TYPE::Linked == ON_InstanceDefinition.InstanceDefinitionType()
      ON_InstanceDefinition::eLinkedComponentAppearance::Reference == ON_InstanceDefinition.LinkedComponentAppearance()
    idef_name - [in]
      If idef_name is not empty, it must be a valid unused name, 
      and the new instance definition will be given this name.
      If idef_name is nullptr or empty, a new name will be automatically assigned.
    bUpdateNewLinkedBlock - [in]
      true if the new linked block should be updated by reading the reference file.
      false if the caller will take responsibility for updating the new linked block.
  Returns:
    -1: copy failed because source_idef_index or idef_name was invalid.
  */
  int CopyLinkedReferenceStyleBlockDefinition(
    int source_idef_index,
    const wchar_t* idef_name,
    bool bUpdateNewLinkedBlock
  );

  CRhinoInstanceObject* CreateInstanceObject( 
    int instance_definition_index,
    ON_Xform instance_xform,
    const ON_3dmObjectAttributes* instance_attributes = NULL,
    CRhinoHistory* pHistory = NULL,
    bool bReference = false,
    bool bInstanceDefinitionObject = false,
    bool bAddToDoc = true
    );

  CRhinoInstanceObject* AddInstanceObject(  // OBSOLETE - USE Create...
    int instance_definition_index,
    ON_Xform instance_xform,
    const ON_3dmObjectAttributes* instance_attributes = NULL,
    CRhinoHistory* pHistory = NULL,
    bool bReference = false 
    );

  /*
  Description:
    Modify instance definition settings (name, url, description).
    Does not change instance definition uuid or geometry.
 
  Parameters:
    idef_settings - [in] new settings.  (The uuid list is ignored.)
    idef_index - [in] zero based index of layer to set.  This
         must be in the range 0 <= layer_index < LayerCount().
    idef_settings_mask - [in]
      Bitfield with values from ON_InstanceDefinition::idef_*_settings
      enum.  If a bit is set, then the corresponding values in
      idef_settings are used to modify the instance definition.
    bQuiet - [in] if TRUE, information message boxes pop up
        when illegal changes are attempted.
 
  Returns:
    TRUE if successful. FALSE if layer_index is out of range
    or the settings attempt
    to lock or hide the current layer.

  Example:    
          // Modify instance definition name and url

          CRhinoInstanceDefinitionTable& idef_table = doc.m_instance_definition_table;

          int idef_index = ...;

          ON_InstanceDefinition idef_settings;
          idef_settings.m_name    = L"Barcelona Map";
          idef_settings.m_url     = L"http://en.wikipedia.org/wiki/Barcelona";
          idef_settings.m_url_tag = L"Barcelona";

          idef_mask = ON_InstanceDefinition::idef_name_setting
                   | ON_InstanceDefinition::idef_url_setting;

          idef_table.ModifyInstanceDefiition(idef_settings,idef_index,idef_mask);

          
          // Modify all settings
          CRhinoInstanceDefinition* idef = idef_table[idef_index];
          ON_InstanceDefinition idef_settings = *idef; // copy existing settings
          idef_settings.m_... = ...; // make necessary changes
          idef_table.ModifyInstanceDefiition( idef_settings,
                                              idef_index,
                                              ON_InstanceDefinition::all_idef_settings);



  See Also:
    CRhinoInstanceDefinitionTable::ModifyInstanceDefinitionGeometry
  */
  bool ModifyInstanceDefinition( 
     const ON_InstanceDefinition& idef_settings,
     int idef_index,
     unsigned int modify_field_mask,
     bool bQuiet
     );

private:
  bool ModifyInstanceDefinitionHelper( 
     const ON_InstanceDefinition& idef_settings,
     int idef_index,
     unsigned int modify_field_mask,
     bool bUpdatePending,
     bool bQuiet
     );

private:
  ON_ComponentManifest& Internal_Manifest();


public:

  /*
  Description:
    If the instance definition has been modified and the modification
    can be undone, then UndoModifyInstanceDefinition() will restore
    the instance definition to its previous state.
  Returns:
    TRUE if this layer had been modified and the modifications were
    undone.
  */
  bool UndoModifyInstanceDefinition(
     int idef_index,
     unsigned int ur_sn = 0
     );

  /*
  Description:
    Modify instance definition geometry and replace all references
    to the current definition with references to the new definition.
 
  Parameters:
    idef_index - [in] zero based index of the instance definition
         to change.
    new_objects - [in] new geometry for the instance definition.
    bQuiet - [in] if TRUE, information message boxes pop up
        when illegal changes are attempted.
 
  Returns:
    index of the new definition or -1 if it fails

  See Also:
    CRhinoInstanceDefinitionTable::ModifyInstanceDefinitionGeometry
  */
  bool ModifyInstanceDefinitionGeometry( 
     int idef_index,
     const ON_SimpleArray<const CRhinoObject*>& new_objects,
     bool bQuiet = false
     );

  /*
  Description:
    Deletes instance definition. 
  Parameters:
    idef_index - [in] zero based index of instance definition to delete.
        This must be in the range 
        0 <= idef_index < InstanceDefinitionCount().
    bDeleteReferences - [in] true to delete all references to this
        definition.  False to delete definition only if there are
        no references.
    bQuiet - [in] If TRUE, no warning message box appears if an
        instance definition cannot be deleted because it is the current layer
        or it contains active geometry.
  Returns:
    TRUE if successful. false if the instance definition has active
    references and bDeleteReferences is false.
  */
  bool DeleteInstanceDefinition(
     int idef_index,
     bool bDeleteReferences,
     bool bQuiet = false
     );

  /*
  Description:
    Purges an instance definition and its definition geometry.
  Parameters:
    idef_index - [in] zero based index of instance definition to delete.
        This must be in the range 
        0 <= idef_index < InstanceDefinitionCount().
  Returns:
    True if successful. False if the instance definition cannot be purged
    because it is in use by reference objects or undo information.
  */
  bool PurgeInstanceDefinition( int idef_index );

  /*
  Description:
    Purge deleted instance definition information that is not
    in use.  This function is time consuming and should be used
    in a thoughtful manner.    
  Parameters:
    bIgnoreUndoReferences:
      If false, then deleted instance definition information
      that could possibly be undeleted by the Undo command
      will not be deleted.
      If true, then all deleted instance definition information
      is deleted.
  */
  void Compact( bool bIgnoreUndoReferences );

  /*
  Description:
    Undeletes an instance definition that has been deleted by DeleteLayer().
  Parameters:
    idef_index - [in] zero based index of an instance definition
        to undelete. This must be in the range
        0 <= idef_index < InstanceDefinitionCount().
  Returns:
    TRUE if successful.
  */
  bool UndeleteInstanceDefinition(
     int idef_index
     );

  /*
  Description:
    Read the objects from a file and use them as the instance's
    definition geometry.
  Parameters:
    idef_index - [in]
      instance definition index
    filename - [in]
      name of file (can be any type of file that Rhino or a plug-in can read).
    bUpdateNestedLinks - [in]
      If true and the instance definition referes to a linked instance definition,
      that needs to be updated, then the nested definition is also updated.
      If false, nested updates are skipped.
  Returns:
    True if successful.
  */
  bool UpdateLinkedInstanceDefinition(
          int idef_index,
          const wchar_t* filename,
          bool bUpdateNestedLinks
          );

  bool UpdateLinkedInstanceDefinition(
    int idef_index,
    const wchar_t* filename,
    bool bUpdateNestedLinks,
    bool bQuiet
    );

  // 24 Oct 2014 - Lowell - Added a function to pass through the file read options
  // to updates of nested linked blocks rh-28779
  bool UpdateLinkedInstanceDefinition(
    int idef_index,
    const wchar_t* filename,
    bool bUpdateNestedLinks,
    CRhFileReadOptions& fro
    );

  /*
  Description:
    Use this to set CRhinoInstanceDefinition::m_sort_index so that
    the values of m_sort_index run from 0 to 
    InstanceDefinitionCount()-1 and 
    compare( idef_table[i], idef_table[j]) < 0 ) implies 
    idef_table[i].m_sort_index < idef_table[j].m_sort_index.
 
  Parameters:
    compare - [in] compare function with prototype
        that returns <0 if arg1<arg2, 0 if arg1=arg2, 
        and >0 if arg1>arg2.
    cull - [in] optional filter function that returns TRUE
        if the instance definition should be ignored
        when sorting.  Instance definitions that are ignored
        are not included in the list returned by GetSortedList().
        Pass 0 if you do not need to cull instance definitions.
    p - [in] pointer passed as last argument to compare() and cull().
 
  Remarks:
    After calling Sort(), you can repeatedly call GetSortedList()
    to get a sorted list of CRhinoInstanceDefinition pointers.  
    Sort() and GetSortedList() do not modify the order or persistent
    information in the layer table.  They are intended to be used to
    get sorted lists of instance definitions for dialogs, etc.
  */
  void Sort( 
    int (*compare)(const CRhinoInstanceDefinition*,const CRhinoInstanceDefinition*,void*),
    int (*cull)(const CRhinoInstanceDefinition*,void*),
    void* p=0
    );

  /*
  Description:
    Gets an array of pointers to layers that is sorted by
    the values of CRhinoInstanceDefinition::m_sort_index.
 
  Parameters:
    sorted_list - [out] this array is filled in with
        CRhinoInstanceDefinition pointers sorted by
        the values of CRhinoInstanceDefinition::m_sort_index.
    bIgnoreDeleted - [in] if TRUE then deleted layers are filtered out.
 
  Remarks:
    Use Sort() to set the values of m_sort_index.
  */
  void GetSortedList(
    ON_SimpleArray<const CRhinoInstanceDefinition*>& sorted_list,
    bool bIgnoreDeleted = false
    ) const;

  /*
  Description:
    Gets unused instance definition name. 
    Used as default when creating new instance definitions.
   Parameters:
    root_name - [in]
      The returned name is "root_name nn"  If root_name is empty,
      then "Block" (localized) is used.
    def_suffix - [in/out]
      Unique names are created by appending a decimal number to the
      localized term for "Block" as in "Block 01", "Block 02",
      and so on.  When def_suffix is not null, the search for an unused
      name begins at "Block suffix".  When suffix is null the search
      begins at "Block 01".  The output value of def_suffix
      is the value of the appended number used to create the unused
      instance definition name.
    def_name - [out]
      A valid instance definition name that is not in use.
  */
  void GetUnusedInstanceDefinitionName( ON_wString& result) const;
  
  void GetUnusedInstanceDefinitionName(
    const wchar_t* root_name, 
    ON_wString& def_name
    ) const;


  // OBSOLETE - USE ModifyInstanceDefinition
  bool SetURL( int index, const wchar_t* url );
  bool SetName( int index, const wchar_t* name );
  bool SetDescription( int index, const wchar_t* description );

  void SetRemapIndex(
      int idef_index,
      int remap_index
      );

  /*
  Description:
    If an instance definition is present because it was created
    while reading a linked idef, it is not saved in the file.
    Call PromoteTenuousInstanceDefinition will modify the idef
    so it is a "real" idef in the file.
  */
  bool PromoteTenuousInstanceDefinition( int idef_index, const wchar_t* name );

  void ListTable(
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

private:
  friend class CRhinoDoc;
  friend class CRhinoRead3dmHelper;

  /*
  Parameters:
    idef - [in]
    idef_final_name - [in]
      If idef_final_name is empty, then idef->Name() is used
      and a reference prefix is prepended as needed.
      If idef_final_name is not empty, then it must be perfectly formatted
      and include the reference decorations.  
  */
  int Internal_AddInstanceDefinition( 
          const ON_InstanceDefinition& idef, 
          const ON_wString idef_final_name,
          unsigned int worksession_reference_model_sn,
          unsigned int linked_idef_sn
          );

  /*
  Parameters:
    idef - [in]
    idef_final_name - [in]
      If idef_final_name is empty, then idef->Name() is used
      and a reference prefix is prepended as needed.
      If idef_final_name is not empty, then it must be perfectly formatted
      and include the reference decorations.  
  */
  int Internal_AddEmptyInstanceDefinition( 
          const ON_InstanceDefinition& idef, 
          const ON_wString idef_final_name,
          unsigned int worksession_reference_model_sn,
          unsigned int linked_idef_sn
          );

  int AddInstanceDefinitionObjects(
          const ON_SimpleArray<const CRhinoObject*>& objects,
          ON_SimpleArray<const CRhinoObject*>& object_list,
          unsigned int worksession_reference_model_sn,
          bool bQuiet
          );

  void Destroy();

  bool DeleteInstanceDefinitionHelper(
     int idef_index,
     bool bDeleteInstanceReferences,
     bool bDeleteReferenceModel,
     bool bQuiet
     );


private:
  // no implementation
  CRhinoInstanceDefinitionTable() = delete;
  CRhinoInstanceDefinitionTable(const CRhinoInstanceDefinitionTable&) = delete;
  CRhinoInstanceDefinitionTable& operator=(const CRhinoInstanceDefinitionTable&) = delete;
};


/*
Description:
  CRhinoInstanceObject::Explode uses CRhinoInstanceObjectPiece
  to return the pieces.
*/
class RHINO_SDK_CLASS CRhinoInstanceObjectPiece
{
public:
  CRhinoInstanceObjectPiece();
  // use default destructor, copy constructor, and operator=

  ON_Xform m_xform;              // transformation to apply
  ON_3dmObjectAttributes m_atts; // attributes to apply
  const CRhinoObject* m_object;  // part of document - do NOT modify
};

#if defined (ON_COMPILER_MSC)
// This stuff is here because of a limitation in the way Microsoft
// handles templates and DLLs.  See Microsoft's knowledge base 
// article ID Q168958 for details.
#pragma warning( push )
#pragma warning( disable : 4231 )
RHINO_SDK_TEMPLATE template class RHINO_SDK_CLASS ON_ClassArray<CRhinoInstanceObjectPiece>;
#pragma warning( pop )
#endif


class RHINO_SDK_CLASS CRhinoInstanceObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoInstanceObject);

public:
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Object::MemoryRelocate override
  void MemoryRelocate() override;

public:
  CRhinoInstanceObject();
  CRhinoInstanceObject( const CRhinoInstanceObject& );
  CRhinoInstanceObject& operator=(const CRhinoInstanceObject& );
  ~CRhinoInstanceObject();

  ///////////////////////////////////////////////////////
  //
  // interface
  //
  const ON_Xform& InstanceXform() const;
  const ON_InstanceRef& IRef() const;
  const CRhinoInstanceDefinition* InstanceDefinition() const;

  /*
  Description:
    Determine if this reference uses an instance definition.
  Parameters:
    idef_index - [in]
  Returns:
    @untitled table
      0      no
    >=1      This CRhinoInstanceObject uses the instance definition
             and the returned value is the nesting depth.
  */
  int UsesDefinition( int idef_index ) const;

  /*
  Description:
    Explodes the instance reference into pieces and add
    these pieces to piece_list.
  Parameters:
    piece_list - [out] pieces of the instance reference are appended
      to this list.
    bExplodeNestedReferences - [in] If true, then nested instance
      references are recursively exploded into until actual geometry
      is found. If false, an CRhinoInstanceReference is added to 
      piece_list when this CRhinoInstanceObject has nested references.
  Returns:
    Number of pieces appended to piece_list.
  */
  int Explode( 
       ON_ClassArray<CRhinoInstanceObjectPiece>& piece_list,
       bool bExplodeNestedReferences = true
       ) const;

  /*
  Description:
    Overload of Explode that allows for hidden pieces to be completely
    skipped and not added to the piece_list.
  Parameters:
    piece_list - [out] pieces of the instance reference are appended
      to this list.
    bSkipHiddenPieces - [in] 
    If true, pieces that are not visible will not be appended to the piece_list
    viewport_id - [in]
      If bSkipHiddenPieces is false, then viewport_id is ignored.
      If bSkipHiddenPieces is true and and viewport_id is not nil,
      then layer and object per viewport visibility settings are used.
      If bSkipHiddenPieces is true and and viewport_id is nil,
      then layer and object global visibility settings are used.
    bExplodeNestedReferences - [in] If true, then nested instance
      references are recursively exploded into until actual geometry
      is found. If false, an CRhinoInstanceReference is added to 
      piece_list when this CRhinoInstanceObject has nested references.
  Returns:
    Number of pieces appended to piece_list.
  */
  int Explode(
       ON_ClassArray<CRhinoInstanceObjectPiece>& piece_list,
       bool bSkipHiddenPieces,
       bool bExplodeNestedReferences
       ) const;

  int Explode(
       ON_ClassArray<CRhinoInstanceObjectPiece>& piece_list,
       bool bSkipHiddenPieces,
       ON_UUID viewport_id,
       bool bExplodeNestedReferences
       ) const;

  /*
  Description:
    Returns true if this object or any "piece" of this instance is hidden.
    An Instance object can be visible, but have hidden pieces because they
    are on hidden layers.
  */
  bool ContainsHiddenObjects() const;

  bool SetInstanceDefinition( const CRhinoInstanceDefinition* idef );

  /*
  Description:
    Sets the instance transform to xform.  Do
    not use this if he instance object has already
    been added to the document.
  Parameters:
    xform - [in]
  */
  bool SetInstanceTransform( const ON_Xform& xform );

  /*
  Description:
    Applies the transformtion to the instance.  Do
    not use this if he instance object has already
    been added to the document.
  Parameters:
    xform - [in]
  */
  bool TransformInstance( ON_Xform xform );

  class CRhInstanceDisplay* InstanceDisplay() const;
public:
  bool IsValid( ON_TextLog* text_log = nullptr ) const override;
  ON::object_type ObjectType() const override;
  const wchar_t* ShortDescription( bool bPlural ) const override;
  bool IsSubObjectSelected( ON_COMPONENT_INDEX component_index ) const override;
  int GetSelectedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const override;
  int SelectSubObject(
        ON_COMPONENT_INDEX component_index,
        bool bSelect = true,
        bool bSynchHighlight = true,
        bool bPersistentSelect = false
        ) const override;
  int UnselectAllSubObjects() override; // returns number of unselected subobjects
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX component_index) const override;   // true if specified component is highlighted
  int GetHighlightedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const override;
  bool HighlightSubObject(ON_COMPONENT_INDEX component_index,bool bHighlight=true) const override;
  int UnhighlightAllSubObjects() override; // returns number of changed subobjects
  bool HighlightRequiresRedraw() const override;
  void Draw(CRhinoDisplayPipeline& dp) const override;
  void DrawHighlightedSubObjects(CRhinoDisplayPipeline&) const override;
  void DrawSubObject(CRhinoDisplayPipeline&,ON_COMPONENT_INDEX) const override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;
  
  bool PickFilter( const CRhinoGetObject&, bool ) const override;
  int Pick( const CRhinoPickContext&, CRhinoObjRefArray& ) const override;
  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const override;
  ON_wString OnDoubleClick(const CRhinoDoubleClickContext&) const override;

  ON_BoundingBox BoundingBox() const override;

	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  bool GetFrame( ON_Plane& plane ) const override;
  int GetWireframeCurves( ON_SimpleArray<ON_Curve*>& curve_list ) const override;
  bool IsMeshable( ON::mesh_type) const override;
  int MeshCount( ON::mesh_type mesh_type, const ON_MeshParameters* mp = nullptr) const override;
  int CreateMeshes( ON::mesh_type, const ON_MeshParameters&, bool bIgnoreCustom=false ) override;
  void DestroyMeshes( ON::mesh_type mesh_type, bool bDeleteMeshes = true ) override;
  bool PromotePreviewMeshes( ON::mesh_type ) override;
  bool IsSolid() const override;

  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const CRhinoViewport* pViewport = nullptr
          ) const override;

  bool SetTextureCoordinatesFromPlugIn(
    const ON_UUID& plugin_id,
    int mapping_channel_id
    ) const override; 

  // Grips support
  void EnableGrips( bool bGripsOn) override;

  const CRhinoObject* SubObjectFromComponentIndex(const ON_COMPONENT_INDEX& ci) const;
protected:
  int SubObjectsAreSelected() const override; // returns number of selected subobjects
  int SubObjectsAreHighlighted() const override; // returns number of highlighted subobjects
private:
  CRhinoInstanceObject(const ON_3dmObjectAttributes&);
  friend class CRhInstanceDisplay;
  friend class CRhinoInstanceDefinitionTable;
  friend class CRhinoInstanceDefinition;
  friend class CRhinoDoc;
  friend class CRhinoRead3dmHelper;
  friend class CRhInstanceGrip;

  ON_InstanceRef m_ref;
  ON_Xform m_inv_xform = ON_Xform::IdentityTransformation; // inverse of m_ref.m_xform

  // serial number of the idef referenced by this iref
  // Use CRhinoInstanceDefinitionTable::FindInstanceDefinition((unsigned int)m_idef_sn,...)
  // to find the idef itself.
  ON__UINT_PTR m_idef_sn = 0;

  class CRhInstanceDisplay* m_instance_display = nullptr;
};

/// <summary>
/// CRhinoInstanceObjectIterator is used to iterate all instance definition geometry referenced by a CRhinoInstanceObject.
/// Any nested instance definition geometry that is referenced multiple times is returned only once.
/// It safely iterates nested instance definitions in a way that protects against infinite looping when damaged models
/// contain circular nested references. 
/// It iterates in a way so that it will not crash if the document is modified or deleted between calls to NextObject().
/// </summary>
class RHINO_SDK_CLASS CRhinoInstanceObjectIterator
{
public:
  CRhinoInstanceObjectIterator() = default;
  ~CRhinoInstanceObjectIterator() = default;
  CRhinoInstanceObjectIterator(const CRhinoInstanceObjectIterator&) = default;
  CRhinoInstanceObjectIterator& operator=(const CRhinoInstanceObjectIterator&) = default;

  /// <summary>
  /// Empty iterator.
  /// </summary>
  static const CRhinoInstanceObjectIterator Empty;

  /// <summary>
  /// Create an iteratator to iterate the objects referenced by iref.
  /// </summary>
  /// <returns></returns>
  CRhinoInstanceObjectIterator(const CRhinoInstanceObject* iref);

  /// <summary>
  /// Initialize this to iterate the objects referenced by iref.
  /// </summary>
  bool Initialize(const CRhinoInstanceObject* iref);

  /// <summary>
  /// CRhinoObject runtime serial number of the instance reference being iterated.
  /// </summary>
  unsigned InstanceObjectSerialNumber() const;

  /// <summary>
  /// The instance reference being iterated.
  /// </summary>
  const CRhinoInstanceObject* InstanceObject() const;

  /// <summary>
  /// The serial number of the CRhinoDoc containing the instance reference being iterated.
  /// </summary>
  unsigned DocumentRuntimeSerialNumber() const;

  /// <summary>
  /// The CRhinoDoc containing the instance reference being iterated.
  /// </summary>
  const CRhinoDoc* Document() const;

  /// <summary>
  /// The instance definition serial number of the current instance definition being iteratated.
  /// </summary>
  unsigned CurrentInstanceDefinitionSerialNumber() const;

  /// <summary>
  /// The current instance definition being iteratated.
  /// </summary>
  const CRhinoInstanceDefinition* CurrentInstanceDefinition() const;

  /// <summary>
  /// The CRhinoObject runtime serial number of CRhinoObject most recently returned by
  /// this iterator.
  /// </summary>
  unsigned CurrentObjectSerialNumber() const;

  /// <summary>
  /// The CRhinoObject most recently returned by
  /// this iterator.
  /// </summary>
  const CRhinoObject* CurrentObject() const;

  /// <summary>
  /// The first object in the set of referenced objects.
  /// This object is always an instance definition object.
  /// When bSkipInstanceObjects = true (generally what you want)
  /// nested instance reference objects are not returned 
  /// (the objects they reference will be returned).
  /// Any instance definition geometry that is referenced multiple times is returned only once.
  /// </summary>
  const CRhinoObject* FirstObject(bool bSkipInstanceObjects);

  /// <summary>
  /// The next object in the set of referenced objects.
  /// This object is always an instance definition object.
  /// When bSkipInstanceObjects = true (generally what you want)
  /// nested instance reference objects are not returned 
  /// (the objects they reference will be returned).
  /// Any instance definition geometry that is referenced multiple times is returned only once.
  /// </summary>
  const CRhinoObject* NextObject(bool bSkipInstanceObjects);

private:
  // Serial numbers are used to prevent crashes when people
  // use an iterator on a document that is modified during iteration.
  unsigned m_doc_sn = 0;
  unsigned m_iref0_sn = 0;

  // Serial numbers of CRhinoInstanceObjects that have already been iterated.
  ON_SimpleArray< unsigned > m_iref_sns;

  // CRhinoInstanceDefinition serial numbers.
  ON_SimpleArray<unsigned> m_idef_sns;

  // If m_current_idef_index >= m_idef_sns.Count(), the iteration has terminated.
  // m_idef_sns[m_current_idef_index] is the CRhinoInstanceDefinition serial number
  // of the CRhinoInstanceDefinition currently being iterated.
  // If i < m_current_idef_index, the corresponding CRhinoInstanceDefinition has
  // already been iterated and will skipped if it is encountered again.
  // If i > m_current_idef_index, the corresponding CRhinoInstanceDefinition 
  // will be iterated in a future call.
  unsigned m_current_idef_index = 0;

  // CRhinoInstanceDefinition.Object(m_current_idef_object_index) is the current object.
  unsigned m_current_idef_object_index = 0;

  // CRhinoObject serial number of the current object.
  unsigned m_current_object_sn = 0;

private:
  unsigned m_reserved0 = 0;
private:
  ON__INT_PTR m_reserved1 = 0;
private:
  ON__INT_PTR m_reserved2 = 0;
};

