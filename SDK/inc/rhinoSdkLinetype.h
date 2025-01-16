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


class RHINO_SDK_CLASS CRhinoLinetype : public ON_Linetype
{
public:
  static unsigned int NextRuntimeSerialNumber();

  const class CRhinoLinetypeTable& m_doc_linetype_table;

  // Runtime serial number of this linetype. 
  // This serial number is unique for each linetype in an instance
  // of a Rhino application. 
  // This serial number is not saved in files and it is generally
  // different the next time a file is read.
  const int m_linetype_sn;


  // Description:
  //   Deleted linetypes are kept in the runtime linetype table
  //   so that undo will work with linetypes.  Call IsDeleted()
  //   to determine if a linetype is deleted.
  //
  // Returns:
  //   true if linetype is deleted.
  bool IsDeleted() const;

  // Description:
  //   Rhino provides the ability to undo linetype modifications.
  //
  // Returns:
  //   true if this linetype has been modified by 
  //   CRhinoLinetypeTable::ModifyLinetype()and the modifications can
  //   be undone.
  //
  // See Also: UndoModify.
  bool IsModified() const;

  // Runtime index used to sort linetypes in linetype dialog
  int m_sort_index = -1;   

  // Runtime index used when remapping linetypes for import/export
  int m_remap_index = -1;



#if defined (ON_RUNTIME_APPLE)
  id MacProxy() const;       // RhinoLinetypeProxy object

#endif

  /*
  Description:
    Call this function to determine if the display segs being used
    are current.
  Parameters:
    linetype_pattern_id - [in/out]
      Store this value someplace on the object that cares about
      this linetype's pattern.  If this function returns true,
      the the input and output values are the same.  If it
      returns false, the returned value is different.
  Returns:
    true:
      The linetype's pattern has not changed.
    False: the linetype's pattern has changed an the display of
      the object needs to be updated.
  */
  bool LinetypePatternIsCurrent( ON__UINT64& linetype_pattern_id ) const;

  // When something in changes that should trigger a recalculation
  // of the information in ON_Linetype.m_segments[], increment
  // m_display_segs_sn.
  int m_display_segs_sn;

  // Draw a few repeats of the linetype into a rectangle
  void DrawLinetype(HDC pDC, const ON_4iRect& rect,
                    COLORREF bkColor, COLORREF fgColor) const;

private:
  friend class CRhinoLinetypeTable;
  // constructor and destructors are private because only 
  // CRhinoLinetypeTable should be creating CRhinoLinetypes.
  // Please do not add any more friend classes.
  CRhinoLinetype() = delete;
  CRhinoLinetype(
    const class ON_Linetype& src,
    const class CRhinoLinetypeTable& line_type_table
    );
  ~CRhinoLinetype();
  CRhinoLinetype( const CRhinoLinetype& ) = delete;
  CRhinoLinetype& operator=(const CRhinoLinetype&) = delete;
  
  unsigned int m_reserved = 0U;

  class CRhLinetypeUndo* m_undo = nullptr;
  
  //void ConstructHelper();
  void DestructHelper();
  //void CopyHelper(const CRhinoLinetype& src);
  
#if defined (ON_RUNTIME_APPLE)
  id m_mac_proxy;     // pointer to RhinoLinetypeProxy
  
  void ReleaseMacProxy();
  void InitMacProxy ();
#endif
};


class RHINO_SDK_CLASS CRhinoLinetypeTable : private ON_SimpleArray< CRhinoLinetype* >
{
public:
  // Description:
  //   Linetype tables store the list of linetypes in a
  //   Rhino document.
  //
  //  Returns:
  //    CRhinoDocument that owns this linetype table.
  CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;

  // Returns:
  //   Number of linetypes in the linetype table, including deleted
  //   linetypes.
  int LinetypeCount() const;

  // Description:
  //   Conceptually, the linetype table is an array of LinetypeCount()
  //   linetypes.  The operator[] can be used to get individual
  //   linetypes.  A linetype is either active or deleted and this
  //   state is reported by CRhinoLinetype::IsDeleted().
  //
  // Parameters:
  //   linetype_index - zero based array index
  //
  // Returns:
  //   Reference to the linetype.  If linetype_index is out of range,
  //   the current linetype is returned. Note that this reference
  //   may become invalid after AddLinetype() is called.
  const CRhinoLinetype& operator[](
    int // linetype_index
    ) const;


  // Description:
  //   Returns the source used by an object to determine its current linetype
  //   to be used by new objects.
  //   Default is ON::linetype_from_layer.
  // Returns:
  //   The linetype source for new objects.
  ON::object_linetype_source CurrentLinetypeSource() const;

  // Description:
  //   Set the source to be used by new objects to determine their current linetype
  // Parameter:
  //   source - the new current linetype source
  void SetCurrentLinetypeSource( ON::object_linetype_source source);


  // Description:
  //   At all times, there is a "current" linetype.  Unless otherwise
  //   specified, new objects are assigned to the current linetype.
  //   If the current linetype source is ON::linetype_from_layer, 
  //     the object's layer's linetype is used instead
  //
  //  Returns:
  //    Zero based linetype table index of the current linetype.
  int CurrentLinetypeIndex() const;

  ON_UUID CurrentLinetypeId() const;

  // Description:
  //   At all times, there is a "current" linetype.  Unless otherwise
  //   specified, new objects are assigned to the current linetype.
  //   If the current linetype source is ON::linetype_from_layer, 
  //     the object's layer's linetype is used instead
  //   The current linetype is never deleted.
  //
  // Parameters:
  //   linetype_index - [in] value for new current linetype.
  //       0 <= linetype_index < LinetypeCount().
  //   bQuiet - [in] if true, then no warning message box
  //       pops up if the current linetype request can't
  //       be satisfied.
  // Returns:
  //   true if current linetype index successfully set.
  bool SetCurrentLinetypeIndex(
         int linetype_index,
         bool bQuiet = false
         );


  // Description:
  //   At all times, there is a "current" linetype.  Unless otherwise
  //   specified, new objects are assigned to the current linetype.
  //   If the current linetype source is ON::linetype_from_layer, 
  //     the object's layer's linetype is used instead
  //
  // Returns:
  //   Reference to the current linetype.  Note that this reference
  //   may become invalid after a call to AddLinetype().
  //
  const CRhinoLinetype& CurrentLinetype() const;

  // Use FindLinePatternFromName()
  // ON_DEPRECATED  too many warnings - deal with this later
  int FindLinetype( 
      const wchar_t* line_pattern_name
      ) const;

  /*
  Description:
    Search for line patterns with a matching name.
  Parameters:
    line_pattern_name - [in]
    bSearchSystemLinePatterns - [in]
      If true, the system Continuous, By Layer, and By Parent line patterns
      are included in the search.
    not_found_return_value - [in]
      Value to return if no line pattern has a matching name.
      Depending on what should happen if the name is not found,
      CurrentLinetypeIndex(), ContinyousLinePattern.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
  Returns:
    The index of the matching line pattern is returned.
    If no match is found, then not_found_return_value is returned.
  */
  int FindLinePatternFromName( 
      const wchar_t* line_pattern_name,
      bool bSearchSystemLinePatterns,
      int not_found_return_value
      ) const;

  /*
  Description:
    Search for line patterns with a matching id.
  Parameters:
    line_pattern_id - [in]
    bSearchSystemLinePatterns - [in]
      If true, the system Continuous, By Layer, and By Parent line patterns
      are included in the search.
    bSearchDeletedLinePatterns - [in]
      If true, deleted line patterns are included in the search.
    not_found_return_value - [in]
      Value to return if no line pattern has a matching id.
      Depending on what should happen if the id is not found,
      CurrentLinetypeIndex(), ContinyousLinePattern.Index() and ON_UNSET_INT_INDEX
      are common choices for this value.
  Returns:
    The index of the matching line pattern is returned.
    If no match is found, then not_found_return_value is returned.
  */
  int FindLinePatternFromId(
    ON_UUID line_pattern_id,
    bool bSearchSystemLinePatterns,
    bool bSearchDeletedLinePatterns,
    int not_found_return_value
    ) const;


  // Description:
  //   For display in Rhino viewports, the linetypes are scaled by a
  //     single scale factor for all viewports.
  //   This is not used for printing, where all linetype patterns are
  //     scaled to print in their defined size 1:1 on the paper.
  // Returns:
  //   The scale factor for linetype display
  double LinetypeScale() const;
  //  Parameter:
  //   New scale factor for linetype display
  void SetLinetypeScale( double scale);

  /*
   Description:
     Basic tool used to create a new linetype with specified 
     definition to the linetype table.  The AddLinetype() functions
     all use CreateLinetype() when a new linetype needs to be added
     to the linetype table.
  
   Parameters:
     linetype - [in] definition of new linetype.  The information
         in linetype is copied.  If linetype.LinetypeName() is empty
         the a unique name of the form "Linetype 01" will be
         automatically created.
  
    worksession_ref_model_sn - [in]
          0: linetype is not a reference layer
          1: linetype is a reference linetype but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number   

    linked_idef_sn - [in]
          0: linetype is not from a liked instance definition
     1-1000: reserved for future use
      >1000: linetype instance definition serial number      
  
   Returns:
     @untitled table
     >=0     index of new linetype
     -1      linetype not added because a linetype with that name
             already exists.
  */
  int CreateLinetype( 
     const ON_Linetype& linetype,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );

  // Description:
  //   Adds a new linetype with specified definition to the linetype table.
  //
  // Parameters:
  //   linetype - [in] definition of new linetype.  The information
  //       in linetype is copied.  If linetype.LinetypeName() is empty
  //       the a unique name of the form "Linetype 01" will be
  //       automatically created.
  //
  //   bReference - [in] true if this linetype is a reference linetype.
  //       Reference linetypes are not saved in files.
  //
  // Returns:
  //   @untitled table
  //   >=0     index of new linetype
  //   -1      linetype not added because a linetype with that name
  //           already exists.
  //
  // Remarks:
  //   In some cases, calling AddLinetype() can cause the memory 
  //   references previously returned by operator[] and 
  //   CurrentLinetype() to become invalid.
  //
  int AddLinetype( 
     const ON_Linetype& linetype,
     bool bReference = false
     );

  /*
  Description:
    Adds a new linetype to the linetype table.
   Parameters:
    linetype_name - [in] name for the new linetype
    segments - [in] array of segment lengths.  Positive values are dashes, negative values are gaps
  Returns:
     @untitled table
     >=0     index of new linetype
     -1      linetype not added because a linetype with that name
             already exists or input is not valid.
 */
  int AddLinetype( const wchar_t* linetype_name, ON_SimpleArray<double>& segments);

  // Description:
  //   Modify linetype settings
  //
  // Parameters:
  //   linetype - [in] new settings.  This information is copied.
  //   linetype_index - [in] zero based index of linetype to set.  This
  //        must be in the range 0 <= linetype_index < LinetypeCount().
  //   bQuiet - [in] if true, information message boxes pop up
  //       when illegal changes are attempted.
  //
  // Returns:
  //   true if successful. false if linetype_index is out of range
  //   or the settings attempt
  //   to lock or hide the current linetype.
  //
  bool ModifyLinetype( 
     const ON_Linetype& linetype,
     int linetype_index,
     bool bQuiet = false
     );

  // Description:
  //   If the linetype has been modified and the modification can be undone,
  //   then UndoModify() will restore the linetype to its previous state.
  //
  // Returns:
  //   true if this linetype had been modified and the modifications were
  //   undone.
  //
  // See Also: IsModified.
  bool UndoModifyLinetype(
     int linetype_index,
     unsigned int ur_sn = 0
     );

  // Description:
  //   Deletes linetype
  //
  // Parameters:
  //   linetype_index - [in] zero based index of linetype to delete.
  //       This must be in the range 0 <= linetype_index < LinetypeCount().
  //   bQuiet - [in] If true, no warning message box appears if a linetype
  //       the linetype cannot be deleted because it is the current linetype
  //       or it contains active geometry.
  //
  // Returns:
  //   true if successful. false if linetype_index is out of range or the
  //   the linetype cannot be deleted because it is the current linetype or
  //   because it linetype is referenced by active geometry.
  bool DeleteLinetype(
     int linetype_index,
     bool bQuiet
     );

  // Description:
  //   Deletes multiple linetypes
  //
  // Parameters:
  //   linetype_index_count - [in] length of linetype_index[] array.
  //   linetype_index - [in] array zero based indices of linetypes to delete.
  //       The indices must be in the range 0 <= linetype_index < LinetypeCount().
  //   bQuiet - [in] If true, no warning message box appears if a linetype
  //       the linetype cannot be deleted because it is the current linetype
  //       or it contains active geometry.
  //
  // Returns:
  //   true if successful. false if linetype_index is out of range or the
  //   the linetype cannot be deleted because it is the current linetype or
  //   because it linetype contains active geometry.
  bool DeleteLinetypes(
     int linetype_index_count,
     const int* linetype_index,
     bool bQuiet
     );

  // Description:
  //   Undeletes a linetype that has been deleted by DeleteLinetype().
  //
  // Parameters:
  //   linetype_index - [in] zero based index of linetype to undelete.
  //       This must be in the range 0 <= linetype_index < LinetypeCount().
  //
  // Returns:
  //   true if successful.
  bool UndeleteLinetype(
     int linetype_index
     );

  // Description:
  //   Use this to set CRhinoLinetype::m_sort_index so that
  //   the values of m_sort_index run from 0 to LinetypeCount()-1
  //   and compare(linetype_table[i],linetype_table[j]) < 0 ) implies 
  //   linetype_table[i].m_sort_index < linetype_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoLinetype* arg1,const CRhinoLinetype* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       bool cull(const CRhinoLinetype* arg,void* p).
  //       that returns true if the linetype should be ignored
  //       when sorting.  Linetypes that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull linetypes
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoLinetype pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the linetype table.  They are intended to be used to get
  //   sorted lists of linetypes for dialogs, etc.
  void Sort( 
    int (*compare)(const CRhinoLinetype*,const CRhinoLinetype*,void*),
    int (*cull)(const CRhinoLinetype*,void*),
    void* p=0
    );

  /*
  Description:
    Gets an array of pointers to linetypes whose
    value of CRhinoLinetype.m_sort_index != -1.
    The returned list is sorted by the value of 
    CRhinoLinetype.m_sort_index.
  
  Parameters:
    sorted_list - [out] this array is returned with 
        length <=  LinetypeCount() and is sorted by 
        the values of CRhinoLinetype::m_sort_index.

    bIgnoreDeleted - [in] true means don't include
        deleted linetypes.

    bIgnoreDeleted - [in] true means don't include
        referenced linetypes. A "referenced" linetype
        is one from a worksession reference model or
        a linked instance definition model.
  
  Remarks:
    Use Sort() to set the values of m_sort_index.
  */
  void GetSortedList(
    ON_SimpleArray<const CRhinoLinetype*>& sorted_list,
    bool bIgnoreDeleted,
    bool bIgnoreReferenced
    ) const;

  void GetSortedList(
    ON_SimpleArray<const CRhinoLinetype*>& sorted_list,
    bool bIgnoreDeleted = false
    ) const;

  /*
  Description:
    Gets unused linetype name used as default when creating new linetypes.
   Parameters:
    unused_line_pattern_name - [out] new linetype name
 */
  void GetUnusedLinetypeName( 
    ON_wString& unused_line_pattern_name
    ) const;

  void GetUnusedLinetypeName( 
    const wchar_t* root_name,
    ON_wString& unused_line_pattern_name
    ) const;

  void SetRemapIndex(
    int, // linetype_index
    int  // remap_index
    );

  /* 
  Description:
    Fills in the linetype table with any default linetypes
    not already included
  */
  void InitDefaultLinetypes();
  void InitDefaultLinetypes(bool bIgnoreDeleted);

  void ListTable( 
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

  /*
  Description:
    Returns true if a linetype is use by an object, layer, or instance definition.
  Parameters:
    linetype_index - [in] zero based index of linetype to query.
  Returns:
    true if successful.
  */
  bool InUse(int linetype_index) const;

private:
  friend class CRhinoDoc;

  // Creates a few default linetypes at creation of the table
  //void CreateDefaultLinetypes();

  // Constructs a linetype table with one default linetype.  This is private
  // because only CRhinoDoc is permitted to create or destroy linetype
  // tables.
  CRhinoLinetypeTable(CRhinoDoc&);
  ~CRhinoLinetypeTable();
  
  // pointer to parent doc set when CRhinoDoc creates the linetype table.
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;

  // index of current linetype
  // SetCurrentLinetypeIndex() keeps this value in sync with
  // CRhinoDoc::Properties().Settings().m_current_linetype_index
  int m_current_linetype_index = -1; // -1 is the continuous pattern
private:

  ON_ComponentManifest& Internal_Manifest();

  // source to be used for new objects
  ON::object_linetype_source m_current_linetype_source = ON::linetype_from_layer;

  // Delete all CRhinoLinetype.m_undo information
  void ClearUndoRecords();

  unsigned int PurgeWorkSessionReferenceModel( 
    unsigned int worksession_ref_model_sn
    );

  void Destroy( class CRhinoEventWatcher* ew );
  void Destroy();

private:
  CRhinoLinetypeTable() = delete;
  CRhinoLinetypeTable(const CRhinoLinetypeTable&) = delete;
  CRhinoLinetypeTable& operator=(const CRhinoLinetypeTable&) = delete;

  bool DeleteLinetypeHelper(
     int, // linetype_index
     bool, // bQuiet
     int* delete_linetypes_with_objects, // 0 = no, 1 = yes, 2 = ask
     int* reference_warning_enabled  // 0 = no, 1 = yes, -1 = delete work session and linked idef reference linetypes
     );

  int FindLinetypeHelper(
    const wchar_t* line_pattern_name,
    int not_found_return_value
  );

  bool AddUndoModifyLinetypeRecord(int linetype_index);

  // global for the whole document but not used for printing
  double m_linetype_display_scale = 1.0;
  
private:
  CRhinoLinetype m_ContinuousLinePattern;
  CRhinoLinetype m_ByLayerLinePattern;
  CRhinoLinetype m_ByParentLinePattern;

  friend class CRhinoDetailViewObject;

public:
  const CRhinoLinetype& ContinuousLinePattern; // index = -1;
  const CRhinoLinetype& ByLayerLinePattern;    // index = -2;
  const CRhinoLinetype& ByParentLinePattern;   // index = -3;

  /*
  Description:
    Returns the number of system linetypes, or those that are implied
    members of the linetype table.
  */
  int SystemLinetypeCount() const;
};


/*
Description:
  Reads linetypes from either a Rhino .3dm file or an AutoCAD .lin file.
Parameters:
  filename  [in]  - The name of the file to read.
  linetypes [out] - The linetypes that were read from the file.
Returns:
  The number of linetypes added to output array.
*/
RHINO_SDK_FUNCTION
int RhinoReadLinetypesFromFile(
  const wchar_t* filename,
  ON_ObjectArray<ON_Linetype>& linetypes
);
