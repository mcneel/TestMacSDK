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

class RHINO_SDK_CLASS CRhinoMaterial : public ON_Material
{
public:
  static unsigned int NextRuntimeSerialNumber();

  /*
  Returns:
    Worksession reference model id.
         0: material is not in a reference model.
         1: material is in an unidentified worksession reference model.
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
      The material is not part of a linked instance definition
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

  /*
  Returns:
    True if the material has been modified and the modification
    can be undone.
  See Also:
    CRhinoMaterialTable::UndoModifyMaterial
  */
  bool IsModified() const;

  // Description:
  //   By default Rhino layers and objects are assigned the
  //   default rendering material.
  //
  // Returns:
  //   TRUE if this material is the default rendering material.
  //
  // Remarks:
  //   The default material is returned by
  //   RhinoApp().AppSettings().DefaultMaterial().
  bool IsDefaultMaterial() const;

  // Runtime index used to sort materials in material dialog
  int m_sort_index = -1;   

  // Runtime index used when remapping materials for import/export
  int m_remap_index = -1;

  // Runtime serial number of this material. 
  // This serial number is unique for each material in an instance
  // of a Rhino application. 
  // This serial number is not saved in files and it is generally
  // different the next time a file is read.
  const unsigned int m_material_sn;

  /*
  Returns:
    texture map bitmap or NULL
  */
  const ON_Bitmap* TextureBitmap() const;
  
  /*
  Returns:
    bump map bitmap or NULL
  */
  const ON_Bitmap* BumpBitmap() const;

  /*
  Returns:
    Environment map bitmap or NULL
  */
  const ON_Bitmap* EmapBitmap() const;

  /*
  Returns:
    The material for the requested channel.
  */
  const CRhinoMaterial& MaterialChannel( 
    int material_channel_index 
    ) const;

  /*
  Returns:
    Number of objects and layers that use this material.
  */
  int InUse() const;

  
#if defined (ON_RUNTIME_APPLE)
  id MacProxy () const;       // MRDocONMaterial object
#endif

private:
  friend class CRhinoMaterialTable;
  // constructor and destructors are private because only 
  // CRhinoMaterialTable should be creating CRhinoMaterial
  // classes. Please do not add any more friend classes.
  CRhinoMaterial();
  ~CRhinoMaterial();
  CRhinoMaterial( const CRhinoMaterial& ) = delete;
  CRhinoMaterial& operator=(const CRhinoMaterial&) = delete;

  int m_use_count = 0; // number of objects and layers that use this material.

  class CRhMaterialUndo* m_undo = nullptr;

#if defined (ON_RUNTIME_APPLE)
  void InitMacProxy (class CRhinoMaterialTable* material_table);
  void ReleaseMacProxy ();
  
  id m_mac_proxy;       // MRDocONMaterial object
#endif
};

class RHINO_SDK_CLASS CRhinoMaterialTable : private ON_SimpleArray<CRhinoMaterial*>
{
public:
  // default material for unlocked objects.
  const CRhinoMaterial& DefaultMaterial;

  // default material for locked objects.
  const CRhinoMaterial& DefaultLockedMaterial;

public:
  // Description:
  //   Material tables store the list of materials in a
  //   Rhino document.
  //
  //  Returns:
  //    CRhinoDocument that owns this material table.
  CRhinoDoc& Document() const;

  const CRhinoDocTableReference& TableReference() const;

  // Returns:
  //   Number of materials in the material table, including deleted
  //   materials.
  int MaterialCount() const;

  // Description:
  //   Conceptually, the material table is an array of MaterialCount()
  //   materials.  The operator[] can be used to get individual
  //   materials.  A material is either active or deleted and this
  //   state is reported by CRhinoMaterial::IsDeleted().
  //
  // Parameters:
  //   material_index - zero based array index or -1 for the
  //       default rendering material.
  //
  // Returns:
  //   Reference to the material.  If material_index is out of range,
  //   the current material is returned. Note that this reference
  //   may become invalid after AddMaterial() is called.
  const CRhinoMaterial& operator[]( int material_index ) const;

  const CRhinoMaterial* Material(
    const ON_ComponentManifestItem& item
    ) const;


  int FindMaterial( 
    const ON_Material&, // ignores name, material index, and material_id
    bool bIgnoreDeleted = true
    ) const;

  // Remarks:
  //   For duplicate materials with the same name, the first
  //   material match in the table will be returned. This function
  //   will not work for finding unnamed materials.
  int FindMaterial( 
    const wchar_t* material_name,
    bool bIgnoreDeleted = true
    ) const;

  int FindMaterial(
    ON_UUID material_id,
    bool bIgnoreDeleted = true
    ) const;

  /*
  Parameters:
    plugin_id - [in] rendering plugin.
      (RhinoApp().GetDefaultRenderApp() returns the active rendering id)

    layer - [in]
    object - [in] NULL is handled without crashing
    object_attributes - [in]
      object attributes
    rendering_attributes - [in]
      object or layer rendering attributes
    material_ref - [in]
      material reference - can be NULL

    front_material - [out]
      Pointer to front face material (never NULL)
      NOTE WELL: This pointer will be invalid if the
      material table is changed.  NEVER use this as 
      a long term reference to a material.
    back_material - [out]
      Pointer to back face material (never NULL)
      NOTE WELL: This pointer will be invalid if the
      material table is changed.  NEVER use this as 
      a long term reference to a material.
  Returns:
    Material for object's front faces.
  */
  void GetMaterials( 
    const ON_UUID& plugin_id, 
    const ON_Layer& layer,
    const CRhinoMaterial*& front_material,
    const CRhinoMaterial*& back_material
    ) const;

  void GetMaterials( 
    const ON_UUID& plugin_id, 
    const ON_3dmObjectAttributes& object_attributes,
    const CRhinoMaterial*& front_material,
    const CRhinoMaterial*& back_material
    ) const;

  void GetMaterials( 
    const ON_UUID& plugin_id, 
    const ON_RenderingAttributes& rendering_attributes,
    const CRhinoMaterial*& front_material,
    const CRhinoMaterial*& back_material
    ) const;

  void GetMaterials( 
    const ON_MaterialRef* material_ref,
    const CRhinoMaterial*& front_material,
    const CRhinoMaterial*& back_material
    ) const;


  // Description:
  //   At all times, there is a "current" material.  Unless otherwise
  //   specified, new objects are assigned to the current layer.
  //   The current layer is never locked, hidden, or deleted.
  //
  //  Returns:
  //    Zero based material table index of the current material.
  int CurrentMaterialIndex() const;

  ON_UUID CurrentMaterialId() const;

  // Description:
  //   At all times, there is a "current" material.  Unless otherwise
  //   specified, new objects are assigned to the current material.
  //   The current material is never locked, hidden, or deleted.
  //
  // Parameters:
  //   material_index - [in] value for new current material.
  //       -1 <= material_index < MaterialCount().
  //   bQuiet - [in] if TRUE, then no warning message box
  //       pops up if the current material request can't
  //       be satisfied.
  // Returns:
  //   TRUE if current material index successfully set.
  bool SetCurrentMaterialIndex(
         int material_index,
         bool bQuiet = FALSE
         );

  ON::object_material_source CurrentMaterialSource() const;

  bool SetCurrentMaterialSource( 
    ON::object_material_source current_material_source 
    );

  /*
   Description:
     Adds a new material to the material table.
  
   Parameters:
     material - [in] definition of new material.  The information
         in material is copied.  It is fine to have empty material
         names
  
    worksession_ref_model_sn - [in]
          0: material is not a reference layer
          1: material is a reference linetype but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number   

    linked_idef_sn - [in]
          0: material is not from a liked instance definition
     1-1000: reserved for future use
      >1000: material instance definition serial number      
  
   Returns:
     @untitled table
     >=0     index of new material
     -1      material not added because a material with that name
             already exists.
  
   Remarks:
     In some cases, calling AddMaterial() can cause the memory 
     references returned by operator[] and CurrentMaterial()
     to become invalid.
  
  */
  int CreateMaterial( 
     const ON_Material& material,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );

  // Description:
  //   Adds a new material to the material table.
  //
  // Parameters:
  //   material - [in] definition of new material.  The information
  //       in material is copied.  It is fine to have empty material
  //       names
  //
  //   bReference - [in] TRUE if this material is a reference material.
  //       Reference materials are not saved in files.
  //
  // Returns:
  //   @untitled table
  //   >=0     index of new material
  //   -1      material not added because a material with that name
  //           already exists.
  //
  // Remarks:
  //   In some cases, calling AddMaterial() can cause the memory 
  //   references returned by operator[] and CurrentMaterial()
  //   to become invalid.
  //
  int AddMaterial( 
     const ON_Material&, // material
     bool=FALSE       // bReference
     );

  /*
  Description:
    Adds a copy of "DefaultMaterial" to the material table.
    Typically this gets modified a little later.
  Returns:
    index of material
  */
  int AddCopyOfDefaultMaterial();

  /*
  Description:
    Reset material to default settings.
  Parameters:
    material_index - [in] 
      zero based index of material to reset.
  */
  bool ResetMaterial(
    int material_index
    );

  // Description:
  //   Modify material settings
  //
  // Parameters:
  //   material - [in] new settings.  This information is copied.
  //   material_index - [in] zero based index of material to set.  This
  //        must be in the range 0 <= material_index < MaterialCount().
  //   bQuiet - [in] if TRUE, then no message boxes pop up to explain
  //       failures.
  //
  // Returns:
  //   TRUE if successful. FALSE if material_index is out of range
  //   or the settings attempt
  //   to lock or hide the current material.
  //
  bool ModifyMaterial( 
     const ON_Material& material,
     int material_index,
     bool bQuiet = false
     );

  /*
  Description:
    If the material has been modified and the modification can be undone,
    then UndoModifyMaterial() will restore the material to its previous state.
 
  Returns:
    True if this material had been modified and the modifications were
    undone.
 
  See Also: CRhinoMaterial::IsModified.
  */
  bool UndoModifyMaterial( 
    int material_index, 
    unsigned int ur_sn = 0 
    );

  // Description:
  //   If the material has been modified and the modification can be undone,
  //   then UndoModify() will restore the material to its previous state.
  //
  // Returns:
  //   TRUE if this material had been modified and the modifications were
  //   undone.
  //
  // See Also: IsModified.
  bool UndoModifyMaterial(
     int // material_index
     );

  // Description:
  //   Deletes material
  //
  // Parameters:
  //   material_index - [in] zero based index of material to delete.
  //       This must be in the range 0 <= material_index < MaterialCount().
  //   bQuiet - [in] If TRUE, no warning message box appears if a material
  //       the material cannot be deleted because it is the current material
  //       or it contains active geometry.
  //
  // Returns:
  //   TRUE if successful. FALSE if material_index is out of range or the
  //   the material cannot be deleted because it is the current material or
  //   because it material contains active geometry.
  bool DeleteMaterial(
     int // material_index
     );

  // Description:
  //   Undeletes a material that has been deleted by DeleteMaterial().
  //
  // Parameters:
  //   material_index - [in] zero based index of material to undelete.
  //       This must be in the range 0 <= material_index < MaterialCount().
  //
  // Returns:
  //   TRUE if successful.
  bool UndeleteMaterial(
     int material_index
     );

  /*
  Description:
    Use this to set CRhinoMaterial::m_sort_index so that
    the values of m_sort_index run from 0 to MaterialCount()-1
    and compare(material_table[i],material_table[j]) < 0 ) implies 
    material_table[i].m_sort_index < material_table[j].m_sort_index.
  
  Parameters:
    compare - [in] compare function with prototype
        int compare(const CRhinoMaterial* arg1,const CRhinoMaterial* arg2,void* p).
        that returns <0 if arg1<arg2, 0 if arg1=arg2, 
        and >0 if arg1>arg2.
    cull - [in] optional filter function with prototype
        int cull(const CRhinoMaterial* arg,void* p).
        Pass null if you do not want to cull the list.
        The cull() should return a non-zero value if the material
        should be ignored when sorting. Materials that are culled
        get m_sort_index set to -1.
    p - [in] pointer passed as last argument to compare() and cull().
  
  Remarks:
    After calling Sort(), you can repeatedly call GetSortedList()
    to get a sorted list of CRhinoMaterial pointers.  Sort() and
    GetSortedList() do not modify the order or persistent information
    in the material table.  They are intended to be used to get
    sorted lists of materials for dialogs, etc.
  */
  void Sort( 
    int (*compare)(const CRhinoMaterial*,const CRhinoMaterial*,void*),
    int (*cull)(const CRhinoMaterial*,void*),
    void* p=0
    );

  /*
  Description:
    Gets an array of pointers to materials whose
    value of CRhinoMaterial.m_sort_index != -1.
    The returned list is sorted by the value of 
    CRhinoMaterial.m_sort_index.
  
  Parameters:
    sorted_list - [out] this array is returned with 
        length <=  MaterialCount() and is sorted by 
        the values of CRhinoMaterial::m_sort_index.

    bIgnoreDeleted - [in] true means don't include
        deleted materials.

    bIgnoreDeleted - [in] true means don't include
        referenced materials. A "referenced" material
        is one from a worksession reference model or
        a linked instance definition model.
  
  Remarks:
    Use Sort() to set the values of m_sort_index.
  */
  void GetSortedList(
    ON_SimpleArray<const CRhinoMaterial*>& sorted_list,
    bool bIgnoreDeleted,
    bool bIgnoreReference
    ) const;

  void SetRemapIndex(
    int, // material_index
    int  // remap_index
    );

  void ListTable(
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

  /*
  Returns:
    true 
      The material is an element in this texture mapping table.
  */
  bool InMaterialTable( const ON_Material* material ) const;

  /*
  Returns:
    true 
      The material_index is a valid index of a material
      element in this material table.    
  */
  bool IsValidMaterialTableIndex( int material_index ) const;


private:
  friend class CRhinoDoc;
  // Constructs a material table with one default material.  This is private
  // because only CRhinoDoc is permitted to create or destroy material
  // tables.
  CRhinoMaterialTable(CRhinoDoc&);
  ~CRhinoMaterialTable();
  
  // pointer to parent doc set when CRhinoDoc creates the material table.
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

  // index of current material
  int m_current_material_index;

  ON::object_material_source m_current_material_source;

  /*
  Description:
    CRhinoDoc::ClearUndoRecords uses this to
    delete all CRhinoMaterial.m_undo information
  */
  void ClearUndoRecords();

  /*
  Description:
    CRhinoDoc::UpdateRenderingInfoUseCounts() uses this
    to adjust CRHinoMaterial::m_use_count.
  Parameters:
    material_index - [in]
    delta - [in]
      must be -1 or +1.
  Returns:
    true if usage count was changed.

    false if usage count could not be changed because input
    parameters are not valid or the use count would be negative
    after the change.
  */
  bool UpdateMaterialUseCounter( int material_index, int delta );

  void Destroy( class CRhinoEventWatcher* ew );
  void Destroy();

  unsigned int PurgeWorkSessionReferenceModel( 
    unsigned int worksession_ref_model_sn
    );

private:
  CRhinoMaterialTable() = delete;
  CRhinoMaterialTable(const CRhinoMaterialTable&) = delete;
  CRhinoMaterialTable& operator=(const CRhinoMaterialTable&) = delete;

  bool AddUndoModifyMaterialRecord(int material_index);

  friend bool RhFastDeleteMaterialHelper(CRhinoMaterialTable&, int);

private:
  ON_ComponentManifest& Internal_Manifest();
  CRhinoMaterial m_DefaultMaterial;
  CRhinoMaterial m_DefaultLockedMaterial;

public:
  // The "WorstPossibleMaterial" is used when no document
  // is available but poorly designed code requires a material.
  // This code can't possibly do anything useful because there
  // is no context in which to use a material.
  static const CRhinoMaterial& WorstPossibleMaterial();
};

