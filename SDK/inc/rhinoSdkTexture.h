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

class RHINO_SDK_CLASS CRhinoTextureMapping : public ON_TextureMapping
{
public:
  static unsigned int NextRuntimeSerialNumber();

  /*
  Returns:
    Worksession reference model id.
         0: linetype is not in a reference model.
         1: linetype is in an unidentified worksession reference model.
         2-1000: reserved for future use
     >1000:
       Serial number of the worksession reference model.       
  Remarks:
    The reference model serial number is a runtime value.
    It is not saved in files and it is generally different the
    next time a file is read.
  */
  unsigned int WorksessionReferenceModelSerialNumber() const;

  /*
  Returns:
    0: 
      The linetype is not part of a linked instance definition
    1-1000: 
      These numbers are reserved for future use.
    >1000:
      CRhinoDoc::m_instance_defintion_table.FindBySerialNumber() can
      be used to find the instance definition.
  Remarks:
    The linked instance definition serial number is a runtime value.
    It is not saved in files and it is generally different the next
    time a file is read.
  */
  unsigned int LinkedInstanceDefinitionSerialNumber() const;
  
  // Runtime index used to sort texture_mappings in texture_mapping dialog
  int m_sort_index = -1;   

  // Runtime index used when remapping texture_mappings for import/export
  int m_remap_index = -1;

  // Runtime serial number of this linetype. 
  // This serial number is unique for each linetype in an instance
  // of a Rhino application. 
  // This serial number is not saved in files and it is generally
  // different the next time a file is read.
  const int m_texturemapping_sn;

  /*
  Returns:
    Number of object using this mapping.
  */
  unsigned int UseCount() const;

  static ON_Xform OcsMappingTransformForObject(const CRhinoObject& object);

#if defined (ON_RUNTIME_APPLE)  
  // table that owns this texture mapping (can be NULL)
  class CRhinoTextureMappingTable* TextureMappingTable() const;
  
  // Document that owns this texture mapping (can be NULL)
  CRhinoDoc* Document() const;

  id MacProxy() const;
#endif
  
private:
  friend class CRhinoTextureMappingTable;

  // constructor and destructors are private because only CRhinoTextureMappingTable
  // should be creating CRhinoTextureMappings. 
  // Please do not add any friend classes.
  CRhinoTextureMapping();
  ~CRhinoTextureMapping();
  CRhinoTextureMapping( const ON_TextureMapping& );

  CRhinoTextureMapping( const CRhinoTextureMapping& ) = delete;
  CRhinoTextureMapping& operator=(const CRhinoTextureMapping&) = delete;

  void ClearUndoRecords();

  unsigned int m_use_count = 0;  // number of objects using this mapping

  class CRhTextureMappingUndo* m_undo = nullptr;
  
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

class RHINO_SDK_CLASS CRhinoTextureMappingTable : private ON_SimpleArray< CRhinoTextureMapping* >
{
public:
  const CRhinoTextureMapping& DefaultTextureMapping;

public:
  /*
  Description:
    TextureMapping tables store the list of texture_mappings in a
    Rhino document.
 
   Returns:
     CRhinoDocument that owns this texture_mapping table.
  */
  CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;


  /*
  Returns:
    Number of texture_mappings in the texture_mapping table, including deleted
    texture_mappings.
  */
  int TextureMappingCount() const;

  /*
  Description:
    Conceptually, the texture_mapping table is an array of TextureMappingCount()
    texture_mappings.  The operator[] can be used to get individual
    texture_mappings.  A texture_mapping is either active or deleted and this
    state is reported by CRhinoTextureMapping::IsDeleted().
 
  Parameters:
    texture_mapping_index - zero based array index or -1 for the
        default rendering texture_mapping.
 
  Returns:
    Reference to the texture_mapping.  If texture_mapping_index is out of range,
    the current texture_mapping is returned. Note that this reference
    may become invalid after AddTextureMapping() is called.
  */
  const CRhinoTextureMapping& operator[]( int texture_mapping_index ) const;

  int FindTextureMapping( 
    const wchar_t* texture_mapping_name,
    bool bIgnoreDeleted = true
    ) const;

  int FindTextureMapping(
    ON_UUID texture_mapping_id,
    bool bIgnoreDeleted = true
    ) const;

  int FindTextureMapping(
    const ON_MappingChannel* mapping_channel,
    bool bIgnoreDeleted = true
    ) const;

  /*
  Description:
    Get a texture mapping from a mapping ref.
  Parameters:
    mapping_channel - [in]
    texture_mapping_id - [in]
    mapping - [out]
  Return
    True if the mapping was found.
  */
  bool GetTextureMapping(
    const ON_MappingChannel* mapping_channel,
    ON_TextureMapping& mapping
    ) const;

  bool GetTextureMapping(
    const ON_UUID& texture_mapping_id,
    ON_TextureMapping& mapping
    ) const;

  /*
  Description:
    Get cached texture coordinates off a mesh.  If the coordinates
    do not exist but can be calculated, they are calculated and
    returned.

  Parameters:
    plugin_id - [in]
    mapping_channel_id - [in]
    object_attributes - [in]
    mc - [in]
    mesh - [in]

  Returns:
    A pointer to the texture coordinates, if they exist.
  */
  const ON_TextureCoordinates* CachedTextureCoordinates(
      const ON_UUID& plugin_id,
      int mapping_channel_id,
      const ON_3dmObjectAttributes& object_attributes,
      const ON_Mesh& mesh
      ) const;

  const ON_TextureCoordinates* CachedTextureCoordinates(
      const ON_MappingChannel* mc,
      const ON_Mesh& mesh
      ) const;

  /*
  Description:
    Adds a new texture_mapping to the texture_mapping table.
 
  Parameters:
    texture_mapping - [in] definition of new texture_mapping.  The information
        in texture_mapping is copied.  If texture_mapping.TextureMappingName() is empty
        the a unique name of the form "TextureMapping 01" will be
        automatically created.
 
    bReference - [in] TRUE if this texture_mapping is a reference texture_mapping.
        Reference texture_mappings are not saved in files.
 
  Returns:
    @untitled table
    >=0     index of new texture_mapping
    -1      texture_mapping not added because a texture_mapping with that name
            already exists.
 
  Remarks:
    In some cases, calling AddTextureMapping() can cause the memory 
    references returned by operator[] and CurrentTextureMapping()
    to become invalid.
 
  */
  int AddTextureMapping( 
     const ON_TextureMapping& texture_mapping,
     bool bReference = false
     );

  /*
   Description:
     Basic tool used to create a new texturemapping with specified 
     definition to the texturemapping table.  The AddTextureMapping() functions
     all use CreateTextureMapping() when a new texturemapping needs to be added
     to the texturemapping table.
  
   Parameters:
     texturemapping - [in] definition of new texturemapping.  The information
         in texturemapping is copied.  If texturemapping.TextureMappingName() is empty
         the a unique name of the form "TextureMapping 01" will be
         automatically created.
  
    worksession_ref_model_sn - [in]
          0: texturemapping is not a reference layer
          1: texturemapping is a reference texturemapping but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number   

    linked_idef_sn - [in]
          0: texturemapping is not from a liked instance definition
     1-1000: reserved for future use
      >1000: texturemapping instance definition serial number      
  
   Returns:
     @untitled table
     >=0     index of new texturemapping
     -1      texturemapping not added because a texturemapping with that name
             already exists.
  */
  int CreateTextureMapping( 
     const ON_TextureMapping& mapping,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );


  /*
  Description:
    Modify texture_mapping settings
 
  Parameters:
    texture_mapping - [in] new settings.  This information is copied.
    texture_mapping_index - [in] zero based index of texture_mapping to set.  This
         must be in the range 0 <= texture_mapping_index < TextureMappingCount().
    bQuiet - [in] if TRUE, then no message boxes pop up to explain
        failures.
 
  Returns:
    TRUE if successful. FALSE if texture_mapping_index is out of range
    or the settings attempt
    to lock or hide the current texture_mapping.
 
  */
  bool ModifyTextureMapping( 
     const ON_TextureMapping& texture_mapping,
     int texture_mapping_index,
     bool bQuiet = false
     );

  bool DeleteTextureMapping( int texture_mapping_index );

  bool UndeleteTextureMapping( int texture_mapping_index );

  /*
  Description:
    If the texture_mapping has been modified and the modification can be undone,
    then UndoModifyTextureMapping() will restore the texture_mapping to its previous state.
 
  Returns:
    True if this texture_mapping had been modified and the modifications were
    undone.
 
  See Also: CRhinoTextureMapping::IsModified.
  */
  bool UndoModifyTextureMapping( 
    int texture_mapping_index, 
    unsigned int ur_sn
    );

  /*
  Description:
    Use this to set CRhinoTextureMapping::m_sort_index so that
    the values of m_sort_index run from 0 to TextureMappingCount()-1
    and compare(texture_mapping_table[i],texture_mapping_table[j]) < 0 ) implies 
    texture_mapping_table[i].m_sort_index < texture_mapping_table[j].m_sort_index.
 
  Parameters:
    compare - [in] compare function with prototype
        int compare(const CRhinoTextureMapping* arg1,const CRhinoTextureMapping* arg2,void* p).
        that returns <0 if arg1<arg2, 0 if arg1=arg2, 
        and >0 if arg1>arg2.
    cull - [in] optional filter function with prototype
        int cull(const CRhinoTextureMapping* arg,void* p).
        that returns TRUE if the texture_mapping should be ignored
        when sorting.  TextureMappings that are ignored are not included
        in the list returned by GetSortedList().  Pass 0 if
        you do not need to cull the list.
    p - [in] pointer passed as last argument to compare() and cull().
 
  Remarks:
    After calling Sort(), you can repeatedly call GetSortedList()
    to get a sorted list of CRhinoTextureMapping pointers.  Sort() and
    GetSortedList() do not modify the order or persistent information
    in the texture_mapping table.  They are intended to be used to get
    sorted lists of texture_mappings for dialogs, etc.
  */
  void Sort( 
    int (*compare)(const CRhinoTextureMapping*,const CRhinoTextureMapping*,void*),
    int (*cull)(const CRhinoTextureMapping*,void*),
    void* p=0
    );

  /*
  Description:
    Gets an array of pointers to texturemappings whose
    value of CRhinoTextureMapping.m_sort_index != -1.
    The returned list is sorted by the value of 
    CRhinoTextureMapping.m_sort_index.
  
  Parameters:
    sorted_list - [out] this array is returned with 
        length <=  TextureMappingCount() and is sorted by 
        the values of CRhinoTextureMapping::m_sort_index.

    bIgnoreDeleted - [in] true means don't include
        deleted texturemappings.

    bIgnoreReferenced - [in] true means don't include
        referenced texturemappings. A "referenced" texturemapping
        is one from a worksession reference model or
        a linked instance definition model.
  
  Remarks:
    Use Sort() to set the values of m_sort_index.
  */
  //void GetSortedList(
  //  ON_SimpleArray< const CRhinoTextureMapping* >& sorted_list,
  //  bool bIgnoreDeleted,
  //  bool bIgnoreReferenced
  //  ) const;

  void SetRemapIndex(
    int, // texture_mapping_index
    int  // remap_index
    );

  void ListTable( 
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

  /*
  Returns:
    true 
      The texture mapping is an element in this texture mapping table.
  */
  bool InTextureMappingTable( const ON_TextureMapping* texture_mapping ) const;

  /*
  Returns:
    true 
      The texture_mapping_index is a valid index of a texture mapping
      element in this texture mapping table.    
  */
  bool IsValidTextureMappingTableIndex( int texture_mapping_index ) const;

private:
  friend class CRhinoDoc;
  // Constructs a texture_mapping table with one default texture_mapping.  
  // The constructor is private because only CRhinoDoc is permitted to 
  // create or destroy texture_mapping tables.
  CRhinoTextureMappingTable(CRhinoDoc&);
  ~CRhinoTextureMappingTable();
  
  void ClearUndoRecords();

  void Destroy( class CRhinoEventWatcher* ew );

  void Destroy();

  unsigned int PurgeWorkSessionReferenceModel( 
    unsigned int worksession_ref_model_sn
    );

  //void GetUnusedTextureMappingName( ON_wString& ) const;

  bool AddUndoModifyTextureMappingRecord(int texture_mapping_index);

  // pointer to parent doc set when CRhinoDoc creates the texture_mapping table.
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

  void SetTextureMappingUseCount( int texture_mapping_index, int use_count );

private:
  CRhinoTextureMappingTable() = delete;
  CRhinoTextureMappingTable(const CRhinoTextureMappingTable&) = delete;
  CRhinoTextureMappingTable& operator=(const CRhinoTextureMappingTable&) = delete;

private:
  ON_ComponentManifest& Internal_Manifest();
  CRhinoTextureMapping m_DefaultTextureMapping;
};

