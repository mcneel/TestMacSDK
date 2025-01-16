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

// CRhinoDimStyle class
class RHINO_SDK_CLASS CRhinoDimStyle : public ON_DimStyle
{
public:
  static unsigned int NextRuntimeSerialNumber();


  const class CRhinoDimStyleTable& m_doc_dimension_style_table;

  enum
  {
    min_precision = 0,
    max_precision = 7,
  };

  bool operator==(const CRhinoDimStyle&) const;

  /*
  Returns:
    True if the dimstyle is part of a worksession reference
    model or a linked instance definition model.  If IsReference()
    is true, then the dimstyle will not be saved as part
    of this document.
  See Also:
    CRhinoDimStyle::WorksessionReferenceModelSerialNumber()
    CRhinoDimStyle::LinkedInstanceDefinitionSerialNumber
  */
  bool IsReference() const;

  /*
  Returns:
    True if the dimstyle is one of the system dimstyles:

    m_doc_dimension_style_table.DefaultDimensionStyle
    m_doc_dimension_style_table.DefaultInchDecimal
    m_doc_dimension_style_table.DefaultInchFractional
    m_doc_dimension_style_table.DefaultFootInchArchitecture
    m_doc_dimension_style_table.DefaultMillimeterSmall
    m_doc_dimension_style_table.DefaultMillimeterLarge
    m_doc_dimension_style_table.DefaultMillimeterArchitecture

  */
  bool IsSystemDimStyle() const;

  /*
  Returns:
    Worksession reference model id.
         0: dimstyle is not in a reference model.
         1: dimstyle is in an unidentified worksession reference model.
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
      The dimstyle is not part of a linked instance definition
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

  // Used for font index remapping during I/O operations. 
  // It is a runtime variable and should be ignored by 
  // everything except CRhinoDoc::Read3DM() and CRhinoDoc::Write3DM().
  int m_remap_index = -1;

  // Runtime index used to sort dimstyles in dialogs
  int  m_sort_index = -1;

  // Runtime serial number of this dimstyle. 
  // This serial number is unique for each dimstyle in an instance
  // of a Rhino application. 
  // This serial number is not saved in files and it is generally
  // different the next time a file is read.
  const unsigned int m_dimstyle_sn;

  // These functions are like the ON_DimStyle ones but with the 
  // values multiplied by DimScale()
  double DSxExtExtension() const;
  double DSxExtOffset() const;
  double DSxArrowSize() const;
  double DSxLeaderArrowSize() const;
  double DSxCenterMark() const;
  double DSxTextGap() const;
  double DSxTextHeight() const;
  double DSxDimExtension() const;

#if defined (RHINO_SDK_MFC)
  static void InitPrecisionList(CComboBox& combo_box, // combobox control
    int def_precision,    // default precision
    UINT current_format); // decimal, fractional, feet/inches

  static void InitToleranceStyleList(CComboBox& combo_box, UINT current = 0);

  static void InitPrecisionList(  // (Obsolete function)
    CComboBox* combo_boxes[2], // length and angle combo box controls
    int defaults[2],           // default length and angle precision
    UINT& current_format,      // last/next radio button index (input/output)
    BOOL32 bLinearOnly);
#endif

#if defined (ON_RUNTIME_APPLE)
  // table that owns this dimstyle (can be NULL)
  class CRhinoDimStyleTable* DimStyleTable() const;

  id MacProxy() const;
#endif

private:
  friend class CRhinoDimStyleTable;
  // constructor and destructors are private because only 
  // CRhinoLinetypeTable should be creating CRhinoLinetypes.
  // Please do not add any more friend classes.

  void ClearUndoRecords();

  // Runtime undo information stored on CRhinoDimStyles
  // that are in the CRhinoDoc::m_dim_style_table.
  class CRhDimStyleUndo* m_undo = nullptr;

private:
  // constructor and destructors are private because only CRhinoDimStyleTable
  // should be creating CRhinoDimStyles.
  // Please do not add any more friend classes.

  CRhinoDimStyle(
    const class CRhinoDimStyleTable&
  );
  ~CRhinoDimStyle();

  CRhinoDimStyle(
    const class CRhinoDimStyleTable&,
    const ON_DimStyle&
  );

private:
  CRhinoDimStyle() = delete;
  CRhinoDimStyle(const CRhinoDimStyle&) = delete;
  CRhinoDimStyle& operator=(const CRhinoDimStyle&) = delete;

#if defined (ON_RUNTIME_APPLE)
  id m_mac_proxy = NULL;     // pointer to MRDimStyle

  void ReleaseMacProxy();
  void InitMacProxy();
#endif

public:
  static const ON_DimStyle* ParentDimstyle(const CRhinoDoc* doc, ON_UUID id);

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};


//////////
// CRhinoDimStyleTable class - A list of DimensionStyle settings groups
// The base class is private because this dynamic array must be accessed and
// modified in a controlled way to insure the document is valid.
class RHINO_SDK_CLASS CRhinoDimStyleTable : private ON_SimpleArray< CRhinoDimStyle* >
{
public:
  CRhinoDimStyleTable(CRhinoDoc&);
  ~CRhinoDimStyleTable();

  CRhinoDoc& Document() const;
  /*
  Returns:
    The current document model space scale setting or ON_UNSET_VALUE if it is not in use or set.
  */
  double DocumentModelViewTextScale() const;

  const class CRhinoDocTableReference& TableReference() const;


  // Description:
  //   Initializes an CRhinoDimStyle with the current default dimension style properties.
  //
  // Parameters:
  //   style - [out] properties returned here
  //
  // Remarks:
  //   The default properties are:
  void GetDefaultDimStyleProperties(ON_DimStyle& style) const;

  // Returns:
  //   Number of styles in the dim style table, including deleted
  //   styles.
  int DimStyleCount() const;

  // Description:
  //   Conceptually, the dim style table is an array of DimStyleCount()
  //   dim styles.  The operator[] can be used to get individual
  //   dim styles.  A dim style is either active or deleted and this
  //   state is reported by CRhinoDimStyle::IsDeleted().
  //
  // Parameters:
  //   style_index - zero based array index
  //
  // Returns:
  //   Reference to the style.  If style_index is out of range,
  //   the current style is returned. Note that this reference
  //   may become invalid after AddDimStyle() is called.
  const CRhinoDimStyle& operator[](int style_index) const;

  const CRhinoDimStyle* DimStyle(
    const ON_ComponentManifestItem& item
  ) const;

  // Description:
  //   At all times, there is a "current" dim style.  Unless otherwise
  //   specified, new dim objects are assigned to the current dim style.
  //   The current dim style is never deleted.
  //
  // Parameters:
  //   style_index - [in] value for new current dim style.
  //       0 <= style_index < DimStyleCount().
  //   bQuiet - [in] if true, then no warning message box
  //       pops up if the current style request can't
  //       be satisfied.
  // Returns:
  //   true if current dim style index successfully set.
  bool SetCurrentDimStyleIndex(int style_index, bool bQuiet = false);

  // Description:
  //   At all times, there is a "current" dim style.  Unless otherwise
  //   specified, new dimension objects are assigned the current dim style.
  //   The current dim style is never deleted.
  //
  // Returns:
  //   Reference to the current dim style.  Note that this reference
  //   may become invalid after a call to AddDimStyle().
  //
  const CRhinoDimStyle& CurrentDimStyle() const;

  // Description:
  //   At all times, there is a "current" dim style.  Unless otherwise
  //   specified, new dimension objects are assigned to the current dim style.
  //   The current dim style is never deleted.
  //
  //  Returns:
  //    Zero based dim style index of the current dim style.
  int CurrentDimStyleIndex() const;

  ON_UUID CurrentDimStyleId() const;

  /*
  Description:
    Search for a dimension style with a matching name.
  Parameters:
    dim_style_name - [in]
    bSearchSystemDimStyles - [in]
      If true, the system default dimension styles (DefaultDimensionStyle, DefaultInchDecimal,
      ..., DefaultMillimeterArchitecture) are included in the search.
      If you are searching for a dimension style to use as it exists,
      bSearchDefaultDimStyles should be true.
      If you are searching for a dimension style to modify, bSearchDefaultDimStyles
      should be false because the default dimension styles cannot be modified.
    not_found_return_value - [in]
      Value to return if no dimension style has a matching name.
      Depending on what should happen if the name is not found,
      CurrentDimStyleIndex(), DefaultDimensionStyle.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
  Returns:
    The index of the matching dimension style is returned.
    If no match is found, then not_found_return_value is returned.
  */
  int FindDimStyleFromName(
    const wchar_t* dim_style_name,
    bool bSearchSystemDimStyles,
    int not_found_return_value
  ) const;

  int FindDimStyleFromName(
    const ON_NameHash& dim_style_name_hash,
    bool bSearchSystemDimStyles,
    int not_found_return_value
  ) const;

  /*
  Description:
    Search for a dimension style with a matching id.
  Parameters:
    dim_style_id - [in]
    bSearchDefaultDimStyles - [in]
      If true, the system default dimension styles (DefaultDimensionStyle, DefaultInchDecimal,
      ..., DefaultMillimeterArchitecture) are included in the search.
      If you are searching for a dimension style to use as it exists,
      bSearchDefaultDimStyles should be true.
      If you are searching for a dimension style to modify, bSearchDefaultDimStyles
      should be false because the default dimension styles cannot be modified.
    bSearchDeletedDeletedDimStyles - [in]
      If true, deleted dimension styles are included in the search.
    not_found_return_value - [in]
      Value to return if no dimension style has a matching id.
      Depending on what should happen if the id is not found,
      CurrentDimStyleIndex(), DefaultDimensionStyle.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
  Returns:
    The index of the matching dimension style is returned.
    If no match is found, then not_found_return_value is returned.
  */
  int FindDimStyleFromId(
    ON_UUID dim_style_id,
    bool bSearchDefaultDimStyles,
    bool bSearchDeletedDimStyles,
    int not_found_return_value
  ) const;

  /*
  Description:
    Find the most appropriate dimension style with the specified content hash.
  Parameters:
    dim_style_content_hash - [in]
      ON_DimStyle.ContentHash() value.
  */
  int FindDimStyleFromContentHash(
    ON_SHA1_Hash dim_style_content_hash,
    bool bSearchDefaultDimStyles,
    bool bSearchDeletedDimStyles,
    int not_found_return_value
  ) const;

  /*
  Parameters:
    font - [in]
    model_space_text_scale - [in]
      If model_space_text_scale is not valid or not positive,
      then CRhinoDimStyleTable.DocumentModelViewTextScale() is used.
      The DimScale() property must be equal to model_space_text_scale.
  */
  int FindDimStyleFromFont(
    const ON_Font& font,
    double model_space_text_scale
  ) const;

  /*
  Description:
    Look for a dimstyle using the specified font. If one does not exist, create a dimstyle.
  Parameters:
    managed_font_serial_number - [in]
      The value returned by ON_Font.ManagedFontSerialNumber().
    model_space_text_scale - [in]
      If model_space_text_scale is not valid or not positive,
      then CRhinoDimStyleTable.DocumentModelViewTextScale() is used.
      The DimScale() property must be equal to model_space_text_scale.
  */
  int FindDimStyleFromManagedFontSerialNumber(
    unsigned int managed_font_serial_number,
    double model_space_text_scale
  ) const;

  /*
  Description:
    Look for a dimstyle using the specified font. If one does not exist, create a dimstyle.
  Parameters:
    font - [in]
    model_space_text_scale - [in]
      If model_space_text_scale is not valid or not positive,
      then CRhinoDimStyleTable.DocumentModelViewTextScale() is used.
      The DimScale() property must be equal to model_space_text_scale.
    bCreateIfNotFound - [in]
      If a dimstyle using the font is not found, a new dimstyle is created by copying
      the default dimstyle and setting the font.
  */
  int FindDimStyleFromFont(
    const ON_Font* font,
    double model_space_text_scale,
    bool bCreateIfNotFound
  );

  /*
  Description:
    Look for a dimstyle using the specified font. If one does not exist, create a dimstyle.
  Parameters:
    font - [in]
    model_space_text_scale - [in]
      If model_space_text_scale is not valid or not positive,
      then CRhinoDimStyleTable.DocumentModelViewTextScale() is used.
      The DimScale() property must be equal to model_space_text_scale.
    bCreateIfNotFound - [in]
      If a dimstyle using the font is not found, a new dimstyle is created by copying
      the default dimstyle and setting the font.
  */

  int FindDimStyleFromFont(
    const ON_Font& font,
    double model_space_text_scale,
    bool bCreateIfNotFound
  );

  /*
  Description:
    Look for a dimstyle using the specified font. If one does not exist, create a dimstyle.
  Parameters:
    font - [in]
    model_space_text_scale - [in]
      If model_space_text_scale is not valid or not positive,
      then CRhinoDimStyleTable.DocumentModelViewTextScale() is used.
      The DimScale() property must be equal to model_space_text_scale.
    template_style - [in]
      If template_style is non-null, the style it points to will be used as the
      starting point if a new style is made, otherwise, the default style is used.
    bCreateIfNotFound - [in]
      If a dimstyle using the font is not found, a new dimstyle is created by copying
      the template_style, if it is non-null, or the default style, and setting the font.
  */
  int FindDimStyleFromFont(
    const ON_Font& font,
    double model_space_text_scale,
    const ON_DimStyle* template_style,
    bool bCreateIfNotFound
  );

  // Description:
  //   Gets unsed layer name used as default when creating new layers
  //
  // Parameters:
  //   result - [out] this is the wString which receives new name
  //
  void GetUnusedDimStyleName(
    ON_wString& result
  ) const;

  void GetUnusedDimStyleName(
    const wchar_t* root_name,
    ON_wString& result
  ) const;

  /*
  Description:
    Sets the name on dim_style to a name that is not currently
    used in the this dimstyle table.
  Parameters:
    dim_style - [in/out]
      If dim_style has no name or an empty name, then a unique name is assigned.

      If dim_style has a non-empty name, it is tested to see if is there is
      an existing dimension style in the table that has the
      same name. If the name is not used by an existing dimension style,
      then dim_style is not changed. If the name is used by an existing
      dimension style, then a suffix is appended to the dim_style name
      to make it unique.
  */
  void SetUnusedDimStyleName(
    ON_DimStyle& dim_style
  ) const;


  // Description:
  //   Adds a new dim style with default definition to the dim style table.
  //
  // Parameters:
  //   bReference - [in] true if this dim style is a reference style.
  //       Reference dim styles are not saved in files.
  //
  // Returns:
  //   index of new dim style
  //
  // Remarks:
  //   In some cases, calling AddDimStyle() can cause the memory 
  //   references previously returned by operator[] and 
  //   CurrentDimStyle() to become invalid.
  //
  int AddDimStyle(bool bReference);

  // Description:
  //   Adds a new dim style with specified definition to the dim style table.
  //
  // Parameters:
  //   style - [in] definition of new dim style.  The information
  //       in style is copied.  If style.Name() is empty
  //       the a unique name of the form "DimStyle 01" will be
  //       automatically created.
  //
  //   bReference - [in] true if this layer is a reference dim style.
  //       Reference dim styles are not saved in files.
  //
  // Returns:
  //   @untitled table
  //   >=0     index of new dim style
  //   -1      dim style not added because a dim style with that name
  //           already exists.
  //
  // Remarks:
  //   In some cases, calling AddDimStyle() can cause the memory 
  //   references previously returned by operator[] and 
  //   CurrentDimStyle() to become invalid.
  //
  int AddDimStyle(const ON_DimStyle& style, bool bReference);

  /*
   Description:
     Basic tool used to create a new dimstyle with specified
     definition to the dimstyle table.  The AddDimStyle() functions
     all use CreateDimStyle() when a new dimstyle needs to be added
     to the dimstyle table.

   Parameters:
     dimstyle - [in] definition of new dimstyle.  The information
         in dimstyle is copied.  If dimstyle.DimStyleName() is empty
         the a unique name of the form "DimStyle 01" will be
         automatically created.

    worksession_ref_model_sn - [in]
          0: dimstyle is not a reference layer
          1: dimstyle is a reference dimstyle but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number

    linked_idef_sn - [in]
          0: dimstyle is not from a liked instance definition
     1-1000: reserved for future use
      >1000: dimstyle instance definition serial number

   Returns:
     @untitled table
     >=0     index of new dimstyle
     -1      dimstyle not added because a dimstyle with that name
             already exists.
  */
  int CreateDimStyle(
    const ON_DimStyle& dimstyle,
    unsigned int worksession_ref_model_sn,
    unsigned int linked_idef_sn
  );

  // Description:
  //   Override fields in an existing dimstyle
  //
  // Parameters:
  //   new_style - [in] new settings - This dimstyle must have at least
  //        one field overridden, with the field override flag set and the 
  //        override count set correctly.
  //        Use ON_DimStyle::SetFieldOverride() to set the flag
  //   dimstyle_index - [in] zero based index of style to override.  This
  //        must be in the range 0 <= style_index < DimStyleCount().
  //
  // Comments:
  //   The dimstyle in the table at dimstyle_index becomes the parent.
  //   new_dimstyle is copied, its name is set to a derivative of the name
  //   of the parent and the parent id is set to the parent's id
  //   The fields in the copied style are set an it is added to the table
  // Returns: 
  //   index (>=0) of the newly added style if successful. -1 if something goes wrong.
  //
  int OverrideDimStyle(const ON_DimStyle& new_dimstyle, int dimstyle_index);

  // Description:
  //   Modify dim style settings
  //
  // Parameters:
  //   style - [in] new settings.  This information is copied.
  //   style_index - [in] zero based index of style to set.  This
  //        must be in the range 0 <= style_index < DimStyleCount().
  //   bQuiet - [in] if true, information message boxes pop up
  //       when illegal changes are attempted.
  //
  // Returns:
  //   true if successful. false if style_index is out of range.
  //
  bool ModifyDimStyle(const ON_DimStyle& style, int style_index, bool bQuiet = false);

  /*
  Description:
    If the dimsytle has been modified and the modification can be undone,
    then UndoModifyDimStyle() will restore the dimstyle to its previous state.

  Returns:
    True if this dimstyle had been modified and the modifications were
    undone.

  See Also: CRhinoDimStyle::IsModified.
  */
  bool UndoModifyDimStyle(
    int dimstyle_index,
    unsigned int ur_sn = 0
  );

  // Description:
  //   Deletes dim style
  //
  // Parameters:
  //   style_index - [in] zero based index of style to delete.
  //       This must be in the range 0 <= style_index < DimStyleCount().
  //   bQuiet - [in] If true, no warning message box appears if a layer
  //       the dim style cannot be deleted because it is the current dim style
  //       or it contains active geometry.
  //
  // Returns:
  //   true if successful. false if style_index is out of range or the
  //   the dim style cannot be deleted because it is the current dim style or
  //   because it dim style contains active geometry.
  bool DeleteDimStyle(int style_index, bool bQuiet);

  // Description:
  //   Deletes multiple dim styles
  //
  // Parameters:
  //   style_index_count - [in] length of style_index[] array.
  //   style_index - [in] array zero based indices of dim styles to delete.
  //       The indices must be in the range 0 <= style_index < DimStyleCount().
  //   bQuiet - [in] If true, no warning message box appears if a dim style
  //       cannot be deleted because it is the current dim style or it 
  //       contains active geometry.
  //
  // Returns:
  //   true if successful. false if style_index is out of range or the
  //   the dim style cannot be deleted because it is the current style or
  //   because there is geometry referencing this dim style.
  bool DeleteDimStyles(int style_index_count, const int* style_index, bool bQuiet);

  // Description:
  //   Undeletes a dim style that has been deleted by DeleteDimStyle().
  //
  // Parameters:
  //   style_index - [in] zero based index of tyle to undelete.
  //       This must be in the range 0 <= style_index < DimStyleCount().
  //
  // Returns:
  //   true if successful.
  bool UndeleteDimStyle(int layer_index);

  // Description:
  //   Use this to set CRhinoDimStyle::m_sort_index so that
  //   the values of m_sort_index run from 0 to DimStyleCount()-1
  //   and compare(dimstyle_table[i],dimstyle_table[j]) < 0 ) implies 
  //   dimstyle_table[i].m_sort_index < dimstyle_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoDimStyle* arg1,const CRhinoDimStyle* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       bool cull(const CRhinoDimStyle* arg,void* p).
  //       that returns true if the dim style should be ignored
  //       when sorting.  Dim styles that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull layers
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoDimStyle pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the layer table.  They are intended to be used to get
  //   sorted lists of dim styles for dialogs, etc.
  void Sort(int (*compar)(const CRhinoDimStyle*, const CRhinoDimStyle*, void*),
    int (*cull)(const CRhinoDimStyle*, void*),
    void* p);

  /*
  Description:
    Gets an array of pointers to dimstyles whose
    value of CRhinoDimStyle.m_sort_index != -1.
    The returned list is sorted by the value of
    CRhinoDimStyle.m_sort_index.

  Parameters:
    sorted_list - [out] this array is returned with
        length <=  DimStyleCount() and is sorted by
        the values of CRhinoDimStyle::m_sort_index.

    bIgnoreDeleted - [in] true means don't include
        deleted dimstyles.

    bIgnoreDeleted - [in] true means don't include
        referenced dimstyles. A "referenced" dimstyle
        is one from a worksession reference model or
        a linked instance definition model.

  Remarks:
    Use Sort() to set the values of m_sort_index.
  */
  void GetSortedList(
    ON_SimpleArray<const CRhinoDimStyle*>& sorted_list,
    bool bIgnoreDeleted,
    bool bIgnoreReference
  ) const;

  void GetSortedList(
    ON_SimpleArray<const CRhinoDimStyle*>& sorted_list,
    bool bIgnoreDeleted = false
  ) const;

  void SetRemapIndex(int style_index, int remap_index);

  // Description:
  //   Finds a dimstyle's parent if there is one
  //
  // Parameters:
  //   childindex - [in] Index of the child dimstyle for which to find the parent
  //
  // Returns:
  //   Index of the parent or -1 if there isn't one
  //
  // Remarks:
  //   Won't return the index of a deleted dimstyle
  int FindParent(int childindex) const;
  int FindParent(int childindex); // bogs del with missing const

  void ListTable(
    ON_TextLog& text_log,
    unsigned int level_of_detail
  ) const;

private:
  friend class CRhinoDoc;
  friend class CRhinoRead3dmHelper;

  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

  ON_ComponentManifest& Internal_Manifest();

  // array index of current dimstyle
  int m_current = -1;

  void ClearUndoRecords();

  void Destroy(class CRhinoEventWatcher* ew);

  void Destroy();

  bool AddUndoModifyDimStyleRecord(int dimstyle_index);

  unsigned int PurgeWorkSessionReferenceModel(
    unsigned int worksession_ref_model_sn
  );

private:
  /*
  Description:
    This function determines what actual value to use for the model_space_text_scale
    when the model_space_text_scale parameter is unset or not valid.
    This function is used by the various forms of CRhinoDimStyleTable::FindDimStyleFromFont().
  */
  double Internal_ModelSpaceTextScale(
    double model_space_text_scale
  ) const;

private:
  CRhinoDimStyleTable() = delete;
  CRhinoDimStyleTable(const CRhinoDimStyleTable&) = delete;
  CRhinoDimStyleTable& operator=(const CRhinoDimStyleTable&) = delete;

private:
  CRhinoDimStyle* m_DefaultDimensionStyle = nullptr;
  CRhinoDimStyle* m_DefaultInchDecimal = nullptr;
  CRhinoDimStyle* m_DefaultInchFractional = nullptr;
  CRhinoDimStyle* m_DefaultFootInchArchitecture = nullptr;
  CRhinoDimStyle* m_DefaultMillimeterSmall = nullptr;
  CRhinoDimStyle* m_DefaultMillimeterLarge = nullptr;
  CRhinoDimStyle* m_DefaultMillimeterArchitecture = nullptr;
  CRhinoDimStyle* m_DefaultFeetDecimal = nullptr;
  CRhinoDimStyle* m_DefaultModelUnitsDecimal = nullptr;
  CRhinoDimStyle* m_DefaultFeetEngrave = nullptr;
  CRhinoDimStyle* m_DefaultMillimeterEngrave = nullptr;
  CRhinoDimStyle* m_DefaultModelUnitsEngrave = nullptr;

  // To add additional built-in dimstyles in the future:
  // 1. Add a new pointer at the end of the list above
  // 2. Add a new ref before the first public ref below
  // 3. Reduce the size of the m_reserved_ptr[] array by 2
  // 4. Update the CRhinoDimStyleTable ctor.
  // In all cases sizeof(CRhinoDimStyleTable) and the offsets
  // of every existing public data member cannot change.

private:
  ON__UINT_PTR m_reserved_ptr[984] = {};

public:
  // Rhino 7 new default dimension styles
  const CRhinoDimStyle& DefaultFeetDecimal;            // index = -8;
  const CRhinoDimStyle& DefaultModelUnitsDecimal;      // index = -9;
  const CRhinoDimStyle& DefaultFeetEngrave;            // index = -10;
  const CRhinoDimStyle& DefaultMillimeterEngrave;      // index = -11;
  const CRhinoDimStyle& DefaultModelUnitsEngrave;      // index = -12;

public:
  // Rhino 6 default dimension styles
  const CRhinoDimStyle& DefaultDimensionStyle;         // index = -1;
  const CRhinoDimStyle& DefaultInchDecimal;            // index = -2;
  const CRhinoDimStyle& DefaultInchFractional;         // index = -3;
  const CRhinoDimStyle& DefaultFootInchArchitecture;   // index = -4;
  const CRhinoDimStyle& DefaultMillimeterSmall;        // index = -5;
  const CRhinoDimStyle& DefaultMillimeterLarge;        // index = -6;
  const CRhinoDimStyle& DefaultMillimeterArchitecture; // index = -7;
  enum : unsigned int
  {
    DefaultDimStyleCount = 12
  };

  /*
  Description:
    Get a list of the default dimension styles.
  Parameters:
    default_dimension_style_list_capacity - [in]
      Capacity of the default_dimension_style_list[] array.
    default_dimension_style_list - [out]
      List of default dimension styles.
  Returns:
    Number of dimension styles added to the list.
  Remarks:
    The dimension style table has CRhinoDimStyleTable::DefaultDimStyleCount
    default dimension styles.
  */
  unsigned int GetDefaultDimensionStyles(
    size_t default_dimension_style_list_capacity,
    const CRhinoDimStyle** default_dimension_style_list
  ) const;

  /*
  Description:
    Get a list of the default dimension styles.
  Returns:
    Number of dimension styles added to the list.
  Remarks:
    The dimension style table has CRhinoDimStyleTable::DefaultDimStyleCount
    default dimension styles.
  */
  unsigned int GetDefaultDimensionStyles(
    ON_SimpleArray<const CRhinoDimStyle*>& default_dimension_style_list
  ) const;

  /*
  Description:
    Get one of the default dimension styles from an index value.
  Parameters:
    dimension_style_index - [in]
  Returns:
    A pointer to the default dimension style with Index() = dimension_style_index
    or nullptr if dimension_style_index does not identify a default dimstyle.
  */
  const class CRhinoDimStyle* DefaultDimStyleFromIndex(
    int dimension_style_index
  ) const;

  /*
  Description:
    Get one of the default dimension styles from an id value.
  Parameters:
    dimension_style_id - [in]
  Returns:
    A pointer to the default dimension style with Id() = dimension_style_id
    or nullptr if dimension_style_id does not identify a default dimstyle.
  */
  const class CRhinoDimStyle* DefaultDimStyleFromId(
    ON_UUID dimension_style_id
  ) const;
};

/*
  Description
    Ask user to select an existing DimStyle
  Parameters:
    [in] interactive - if true, a dialog is used, otherwise, the commandline is used
  Returns:
    integer index of selected style or ON_UNSET_INT_INDEX for none.
*/
// Added 11/10/2009 to eliminate ActiveDoc() call
RHINO_SDK_FUNCTION
int RhinoSelectDimstyle(CRhinoDoc& doc, bool interactive);

// Old version replaced 11/09/2009 SDKbreak
ON_DEPRECATED_MSG("use overload that takes a CRhinoDoc parameter")
RHINO_SDK_FUNCTION
int RhinoSelectDimstyle(bool interactive);

