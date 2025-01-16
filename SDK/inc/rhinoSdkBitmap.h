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

class RHINO_SDK_CLASS CRhinoDocTableReference
{
public:
  static const CRhinoDocTableReference Unset;

  CRhinoDocTableReference() = delete;
  ~CRhinoDocTableReference() = default;
  CRhinoDocTableReference(const CRhinoDocTableReference&);
  CRhinoDocTableReference& operator=(const CRhinoDocTableReference&) = delete;
  
  unsigned int DocumentSerialNumber() const;

  const class CRhinoDoc* Document() const;

  ON_ModelComponent::Type ComponentType() const;

  unsigned int ComponentCount() const;

  /*
  Remarks:
    Every document component can be found by id.
  */
  const class ON_ModelComponent* FromId(
    ON_UUID id
  ) const;

  /*
  Returns:
    True if the components in this table have unique names and
    can be found by name or name hash.
  Remarks:
    All model components have names, but some, like model geometry and rendering materials, 
    are not required to have unique names.
  */
  bool UniquelyNamedComponents() const;

  const class ON_ModelComponent* FromName(
    const wchar_t* name,
    ON_UUID parent_id
  ) const;

  const class ON_ModelComponent* FromNameHash(
    const ON_NameHash& name_hash
  ) const;

  /*
  Returns:
    True if the components in this table can be found by index.
  */
  bool IndexedComponents() const;

  /*
  Remarks:
    If IndexedComponents() is true, then the table components
    can be found by index.
  */
  const class ON_ModelComponent* operator[](size_t i) const;


  CRhinoDocTableReference
  (
    unsigned int doc_sn,
    ON_ModelComponent::Type component_type,
    const ON_SimpleArray<const class ON_ModelComponent*> * table,
    const wchar_t* default_component_root_name
  );
  
  const ON_wString UnusedComponentName() const;
  
  const ON_wString UnusedComponentName(
    const wchar_t* root_name
  ) const;

  const ON_wString& DefaultComponentRootName() const;
  
private:
  const unsigned int m_doc_sn;
  ON_ModelComponent::Type m_component_type = ON_ModelComponent::Type::Unset;
  const ON_SimpleArray<const ON_ModelComponent*>* m_doc_table = nullptr;
  const ON_wString m_default_component_root_name;
  mutable unsigned int m_unused_name_suffix = 0;
  ON__UINT64 reserved1 = 0;
  ON__UINT64 reserved2 = 0;
  ON__UINT64 reserved3 = 0;
  ON__UINT64 reserved4 = 0;
};


class RHINO_SDK_CLASS CRhinoBitmap
{
public:
  /*
  Returns:
    Bitmap name.  
    When the bitmap is an embedded image file, the
    full path to the image file is returned.
  */
  const ON_FileReference& FileReference() const;

  /*
  Description:
    Gets a CRhinoDib of the bitmap
  Parameters:
    dib - [in] if not NULL, the dib is put into this bitmap.
  Returns:
    If successful, a pointer to a CRhinoDib.  The caller
    is responsible for deleting this CRhinoDib.
  */
  class CRhinoDib* Dib( 
    class CRhinoDib* dib = nullptr
    ) const;

  /*
  Description:
    Get array index of the bitmap in the bitmap table.
  Returns:
     -1:  The bitmap is not in the bitmap table
    >=0:  Index of the bitmap in the bitmap table.
  */
  int Index() const;

  ON_UUID Id() const;

  bool IsDeleted() const;

  // Runtime index used to sort bitmaps in bitmap dialog
  int m_sort_index = -1;   

  // Runtime index used when remapping bitmaps for import/export
  int m_remap_index = -1;

  const ON_Bitmap* Bitmap() const;
  const ON_EmbeddedBitmap* EmbeddedBitmap() const;
  const ON_WindowsBitmapEx* WindowsBitmap() const;

  /*
  Description:
    Write the bitmap to a file.
  Parameters:
    fullpath - [in] full path, including file name and extension,
    name of the file to write.
  Returns:
    True if successful.
  See Also:
    CRhinoBitmapTable::ExportToFiles
  */
  bool ExportToFile( 
    const wchar_t* fullpath
    ) const;

  /*
  Description:
    Rhino allows files to be used as the definitions for
    linked block instances definitions and to be used as 
    reference models in worksessions. If an entry in the bitmap
    table is from a linked instance definition or worksession
    reference model, then IsReference() returns true. 
    If IsReference() returns true, then the bitmap is not
    saved in a file.
  Returns:
    true if the bitmap is part of a linked instance definition
    or a worksession reference model.
  Remarks:
    If you need to know what type of reference the bitmap is
    used for, use the WorksessionReferenceModelSerialNumber(),
    and LinkedInstanceDefinitionSerialNumber() functions.
  */
  bool IsReference() const;

  /*
  Returns:
    Worksession reference model id.
         0: bitmap is not in a reference model.
         1: bitmap is in an unidentified worksession reference model.
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
      The bitmap is not part of a linked instance definition
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


private:
  friend class CRhinoBitmapTable;
  // constructor and destructors are private because only CRhinoBitmapTable
  // should be creating CRhinoBitmaps.
  CRhinoBitmap() = default;
  ~CRhinoBitmap();
  CRhinoBitmap( const CRhinoBitmap& ) = delete;
  CRhinoBitmap& operator=(const CRhinoBitmap&) = delete;

private:
  ON_Bitmap* m_bitmap = nullptr;
};


class RHINO_SDK_CLASS CRhinoBitmapTable : private ON_SimpleArray<const CRhinoBitmap*>
{
public:
  // Description:
  //   Bitmap tables store the list of bitmaps in a
  //   Rhino document.
  //
  //  Returns:
  //    CRhinoDocument that owns this bitmap table.
  class CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;

  /*
  Returns:
    Number of bitmaps in the table.
  */
  int BitmapCount() const;

  /*
  Parameters:
    i - [in] 0 <= i < BitmapCount()
  Returns:
    Pointer to bitmap or NULL.
  */
  const CRhinoBitmap* operator[](int i) const;

  const CRhinoBitmap* FindBitmap( 
        ON_UUID bitmap_id,
        bool bIgnoreDeleted = true
        ) const;

  /*
  Description:
    Get a bitmap.
  Parameters:
    bitmap_filename - [in] name of bitmap to search for. The search ignores case.  
    bLoadFromFile   - [in] if true, and the bitmap is on disk but not in the current table, 
                           then the bitmap is added to the table.     
  Returns:
    NULL if the bitmap could not be found, otherwise a pointer to the bitmap is returned.
  */
  const CRhinoBitmap* FindEmbeddedFile( 
    const wchar_t* bitmap_filename,
    bool bLoadFromFile = false
    ) const;  
  
  const CRhinoBitmap* FindEmbeddedFile( 
    const ON_FileReference& file_reference,
    bool bLoadFromFile=false
    ) const;

  /*
  Description:
    Adds a new bitmap with specified name to the bitmap table.
 
  Parameters:
    bmi - [in] definition of new bitmap. (Copied into the table.)  

    bits - [in] pointer to bitmap bits.  If NULL, then it will
           be assumed the bitmap bits are after the palette
           in bmi.

    bitmap_filename - [in] If NULL or empty, then a unique name
        of the form "Bitmap 01" will be automatically created.

    bReplaceExisting - [in] If true and the there is already a bitmap
        using the specified name, then that bitmap is replace.
        If false and there is already a bitmap using the specified
        name, then NULL is returned.
 
  Returns:
    A pointer to the added bitmap or NULL if the bitmap name
    is in use and bReplaceExisting = false.

  See Also:
    CRhinoBitmapTable::Bitmap
  */
  const CRhinoBitmap* AddBitmap( 
     const BITMAPINFO* bmi,
     const void* bits,
     const wchar_t* bitmap_filename = nullptr,
     bool bReplaceExisting = false
     );

  /*
  Description:
    Adds an embedded file to the bitmap table.
  Parameters:
    file_reference - [in]
  Returns:
    NULL if the file cannot be located.
  */
  const CRhinoBitmap* AddEmbeddedFile( 
     const wchar_t* file_path,
     bool bReplaceExisting,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );

  const CRhinoBitmap* AddEmbeddedFile( 
     const ON_FileReference& file_reference,
     bool bReplaceExisting,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );

  /*
  Description:
    Adds a new bitmap with specified name to the bitmap table.
 
  Parameters:
    pBitmap - [in] an bitmap created with either 
       new ON_WindowsBitmapEx() or new ON_EmbeddedBitmap().
       If pBitmap->m_name is not set, a name will be provided.

    bReplaceExisting - [in] If true and the there is already a bitmap
        using the specified name, then that bitmap is replace.
        If false and there is already a bitmap using the specified
        name, then NULL is returned.
 
    worksession_ref_model_sn - [in]
          0: bitmap is not a reference layer
          1: bitmap is a reference layer but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number      
    linked_idef_sn - [in]
          0: bitmap is not from a liked instance definition
     1-1000: reserved for future use
      >1000: bitmap instance definition serial number
      
  Returns:
    NULL if the bitmap cannot be added or the name is in use.
 
  See Also:
    CRhinoFileUtilities::FindFile
    CRhinoBitmapTable::Bitmap
  */
  const CRhinoBitmap* AddBitmap( 
      ON_Bitmap* pBitmap,
      bool bReplaceExisting,
      unsigned int worksession_ref_model_sn,
      unsigned int linked_idef_sn
      );

  // Description:
  //   Deletes bitmap
  //
  // Parameters:
  //   bitmap_index - [in] zero based index of bitmap to delete.
  //       This must be in the range 0 <= bitmap_index < BitmapCount().
  //   bQuiet - [in] If TRUE, no warning message box appears if a bitmap
  //       the bitmap cannot be deleted because it is the current bitmap
  //       or it contains active geometry.
  //
  // Returns:
  //   TRUE if successful. FALSE if bitmap_index is out of range or the
  //   the bitmap cannot be deleted because it is the current bitmap or
  //   because it bitmap contains active geometry.
  bool DeleteBitmap(
     const wchar_t* bitmap_filename
     );  
  
  bool DeleteEmbeddedFile(
     const wchar_t* file_path
     );  

  bool DeleteEmbeddedFile(
     const ON_FileReference& file_reference
     );  

  bool DeleteEmbeddedFile(
     ON_UUID bitmap_id
     );

  // Description:
  //   Use this to set CRhinoBitmap::m_sort_index so that
  //   the values of m_sort_index run from 0 to BitmapCount()-1
  //   and compare(bitmap_table[i],bitmap_table[j]) < 0 ) implies 
  //   bitmap_table[i].m_sort_index < bitmap_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoBitmap* arg1,const CRhinoBitmap* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       BOOL32 cull(const CRhinoBitmap* arg,void* p).
  //       that returns TRUE if the bitmap should be ignored
  //       when sorting.  Bitmaps that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull bitmaps
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoBitmap pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the bitmap table.  They are intended to be used to get
  //   sorted lists of bitmaps for dialogs, etc.
  void Sort( 
    int (*compare)(const CRhinoBitmap * const *,const CRhinoBitmap * const *,void*),
    int (*cull)(const CRhinoBitmap*,void*),
    void* p=nullptr
    );

  // Description:
  //   Gets an array of pointers to bitmaps that is sorted by
  //   the values of CRhinoBitmap::m_sort_index.
  //
  // Parameters:
  //   sorted_list - [out] this array is returned with length
  //       BitmapCount() and is sorted by the values of
  //       CRhinoBitmap::m_sort_index.
  //   bIgnoreDeleted - [in] TRUE means don't include
  //       deleted bitmaps.
  //
  // Remarks:
  //   Use Sort() to set the values of m_sort_index.
  void GetSortedList(
    ON_SimpleArray<const CRhinoBitmap*>& sorted_list
    ) const;

  /*
  Description:
    Export all the bitmaps in the table to files.
  Parameters:
    dirpath - [in] full path to the directory where the
    bitmaps should be saved.  If NULL, a dialog is used
    to interactively get the directory name.

    overwrite - [in] 0 = no, 1 = yes, 2 = ask
  Returns:
    Number of bitmaps written.
  See Also:
    CRhinoBitmap::ExportToFile
  */
  int ExportToFiles( 
    const wchar_t* dirpath,
    int overwrite = 0
    ) const;

  void SetRemapIndex(
    int, // bitmap_index
    int  // remap_index
    );

  void ListTable( 
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

  /*
  Returns:
    true 
      The bitmap is an element in this bitmap table.
  */
  bool InBitmapTable( const ON_Bitmap* bitmap ) const;

  /*
  Returns:
    true 
      The bitmap_index is a valid index of a bitmap
      element in this bitmap table.    
  */
  bool IsValidBitmapTableIndex( int bitmap_index ) const;

  /*
  Description:
    Get a bitmap.
  Parameters:
    bitmap_name - [in] name of bitmap to search for.  The search ignores case.
    bLoadFromFile - [in] if true, and the bitmap is on disk but not in the current table, 
                         then the bitmap is added to the table.
  Returns:
    NULL if the bitmap could not be found, otherwise a pointer to the bitmap is returned.
  */
  ON_DEPRECATED_MSG("use CRhinoBitmapTable::FindEmbeddedFile()")
  const CRhinoBitmap* Bitmap(
    const wchar_t* bitmap_filename,
    bool bLoadFromFile = false
    ) const;

private:
  friend class CRhinoDoc;
  friend class CRhDocSdkExtension;

  void Destroy();

  // Constructs a layer table with one default layer.  This is private
  // because only CRhinoDoc is permitted to create or destroy layer
  // tables.
  CRhinoBitmapTable(CRhinoDoc&);
  ~CRhinoBitmapTable();

private:
  CRhinoBitmapTable() = delete;
  CRhinoBitmapTable(const CRhinoBitmapTable&) = delete;
  CRhinoBitmapTable& operator=(const CRhinoBitmapTable&) = delete;

private:
  ON_ComponentManifest& Internal_Manifest();
  
  // pointer to parent doc set when CRhinoDoc creates the layer table.
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

  unsigned int PurgeWorkSessionReferenceModel( 
    unsigned int worksession_ref_model_sn
    );

};
