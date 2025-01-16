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

// CRhinoHatchPattern class
class RHINO_SDK_CLASS CRhinoHatchPattern : public ON_HatchPattern
{
public:
  static unsigned int NextRuntimeSerialNumber();

  /*
  Returns:
    Worksession reference model id.
         0: hatch pattern is not in a reference model.
         1: hatch pattern is in an unidentified worksession reference model.
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
      The hatch pattern is not part of a linked instance definition
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


  // Draw a swatch of the pattern into a rectangle
  void DrawPattern( HDC pDC, const ON_4iRect& rect, double angle,
                    COLORREF bkColor, COLORREF fgColor) const;

  // Used for font index remapping during I/O operations. 
  // It is a runtime variable and should be ignored by 
  // everything except CRhinoDoc::Read3DM() and CRhinoDoc::Write3DM().
  int m_remap_index = -1;

  // Runtime index used to sort layers in layer dialog
  int  m_sort_index = -1;

//  bool Create( const ON_Plane& plane, const ON_3dPoint& p0, const ON_3dPoint& p1, ON_SimpleArray<ON_Line> lines);

  // Description:
  //    Creates the display graphics for a Hatch from the Hatch and
  //    HatchPattern definitions.
  // Parameters:
  //    pattern [in] - the hatch pattern to draw
  //    hatch [in]   - The hatch contains
  //                     3d plane for the display objects
  //                     scale to apply to the pattern definition
  //                     rotation to apply to the pattern definition
  //                     2d boundary loops for the hatched area 
  //                       in hatch plane coordinates
  //    bounds [out] - 3d boundary curves for the hatch
  //                     The first curve in the array is the outer boundary
  //                       and the rest are inners
  //    lines [out]  - 3d line segments representing the applied pattern
  //                     There may be no lines if the hatch uses a different fill type
  //    pBrep [out]  - Trimmed planar brep used for solid fill hatch display
  //                     Will be NULL if the hatch isn't solid filled
  // Returns:
  //    true for success, false for failure
  static bool CreateDisplay( const ON_HatchPattern* pattern, 
                             const ON_Hatch* hatch,   
                             ON_SimpleArray<ON_NurbsCurve*>& bounds,
                             ON_SimpleArray<ON_Line>& lines,
                             ON_Brep*& pBrep);
 
  // 11 Nov, 2010 - Lowell - Added to support paper space scaling of hatches
  // Same as above, but the pattern_scale argument is a multipier for the 
  // pattern scale stored on the hatch.
  static bool CreateDisplay( const ON_HatchPattern* pattern, 
                             const ON_Hatch* hatch,
                             double pattern_scale,
                             ON_3dVector xdir,
                             ON_3dVector ydir,
                             ON_SimpleArray<ON_NurbsCurve*>& bounds,
                             ON_SimpleArray<ON_Line>& lines,
                             ON_Brep*& pBrep);
 
  // Runtime serial number of this hatch pattern. 
  // This serial number is unique for each hatch pattern in an instance
  // of a Rhino application. 
  // This serial number is not saved in files and it is generally
  // different the next time a file is read.
  const unsigned int m_hatchpattern_sn;

private:
  friend class CRhinoHatchPatternTable;
  // constructor and destructors are private because only 
  // CRhinoHatchPatternTable should be creating CRhinoHatchPattern
  // classes. Please do not add any more friend classes.
  CRhinoHatchPattern(const ON_HatchPattern&);
  ~CRhinoHatchPattern();
  CRhinoHatchPattern() = delete;
  CRhinoHatchPattern( const CRhinoHatchPattern& ) = delete;
  CRhinoHatchPattern& operator=( const CRhinoHatchPattern& ) = delete;

  // Delete all CRhinoHatchPattern.m_undo information
  void ClearUndoRecords();

  class CRhHatchPatternUndo* m_undo = nullptr;
  ON__UINT_PTR m_sdk_reserved = 0;
};


//////////
// CRhinoHatchPatternTable class - A list of Hatch Pattern definitions
class RHINO_SDK_CLASS CRhinoHatchPatternTable : private ON_SimpleArray< CRhinoHatchPattern* >
{
public:
  CRhinoHatchPatternTable( CRhinoDoc&);
  ~CRhinoHatchPatternTable();

  CRhinoDoc& Document() const;
  const class CRhinoDocTableReference& TableReference() const;

  // Returns:
  //   Number of patterns in the hatch pattern table, including deleted
  //   patterns.
  int HatchPatternCount() const;

  // Description:
  //   Conceptually, the hatch pattern table is an array of PatternCount()
  //   hatch patterns.  The operator[] can be used to get individual
  //   hatch patterns.  A hatch pattern is either active or deleted and this
  //   state is reported by CRhinoHatchPattern::IsDeleted().
  //
  // Parameters:
  //   pattern_index - zero based array index
  //
  // Returns:
  //   Reference to the pattern.  If pattern_index is out of range,
  //   the current pattern is returned. Note that this reference
  //   may become invalid after AddHatchPattern() is called.
  const CRhinoHatchPattern& operator[]( int pattern_index) const;

  const CRhinoHatchPattern* HatchPattern(
    const ON_ComponentManifestItem& hatch_pattern_manifest_item
    ) const;

  const CRhinoHatchPattern* HatchPattern(
    const ON_ComponentManifestItem& hatch_pattern_manifest_item,
    const CRhinoHatchPattern* not_found
    ) const;

  // Description:
  //   At all times, there is a "current" hatch pattern.  Unless otherwise
  //   specified, new hatch objects are assigned to the current hatch pattern.
  //   The current hatch pattern is never deleted.
  //
  // Parameters:
  //   pattern_index - [in] value for new current hatch pattern.
  //       0 <= pattern_index < HatchPatternCount().
  //   bQuiet - [in] if true, then no warning message box
  //       pops up if the current pattern request can't
  //       be satisfied.
  // Returns:
  //   true if current hatch pattern index successfully set.
  bool SetCurrentHatchPatternIndex( 
    int pattern_index, 
    bool bQuiet = false
    );
  bool SetCurrentHatchPattern(
    const ON_ComponentManifestItem& hatch_pattern_manifest_item,
    bool bQuiet = false
    );

  // Description:
  //   At all times, there is a "current" hatch pattern.  Unless otherwise
  //   specified, new hatch objects are assigned the current hatch pattern.
  //   The current hatch pattern is never deleted.
  //
  // Returns:
  //   Reference to the current hatch pattern.  Note that this reference
  //   may become invalid after a call to AddHatchPattern().
  //
  const CRhinoHatchPattern& CurrentHatchPattern() const;

  /*
  Description:
    Finds the hatch pattern with a given name.
  
  Parameters:
    name - [in]  
    bSearchDefaultHatchPatterns - [in]
  Returns:
    id of hatch pattern or ON_nil_uuid if no hatch pattern has the name
  */
  const ON_ComponentManifestItem& HatchPatternManifestItemFromName(
    const wchar_t* name, 
    bool bSearchDefaultHatchPatterns
    ) const;

  const CRhinoHatchPattern* HatchPatternFromName(
    const wchar_t* name, 
    bool bSearchDefaultHatchPatterns
    ) const;

  const ON_ComponentManifestItem& HatchPatternManifestItemFromId(
    ON_UUID hatchpattern_id,
    bool bSearchDefaultHatchPatterns,
    bool bSearchDeletedHatchPatterns
    ) const;

  const CRhinoHatchPattern* HatchPatternFromId(
    ON_UUID hatchpattern_id,
    bool bSearchDefaultHatchPatterns,
    bool bSearchDeletedHatchPatterns
    ) const;

  const ON_ComponentManifestItem& HatchPatternManifestItemFromIndex(
    int hatchpattern_index,
    bool bSearchDefaultHatchPatterns,
    bool bSearchDeletedHatchPatterns
    ) const;

  const CRhinoHatchPattern* HatchPatternFromIndex(
    int hatchpattern_index,
    bool bSearchDefaultHatchPatterns,
    bool bSearchDeletedHatchPatterns
    ) const;

  // Description:
  //   Gets unsed layer name used as default when creating new layers
  //
  // Parameters:
  //   result - [out] this is the wString which receives new name
  //
  void GetUnusedHatchPatternName( 
    ON_wString& unused_name
    ) const;

  void GetUnusedHatchPatternName( 
    const wchar_t* root_name,
    ON_wString& unused_name
    ) const;
  /*
   Description:
     Basic tool used to create a new hatch pattern with specified 
     definition to the hatch pattern table.  The AddHatchPattern() functions
     all use CreateLinetype() when a new linetype needs to be added
     to the linetype table.
  
   Parameters:
     hatch_pattern - [in] definition of new hatch pattern.  
         The information in hatch_pattern is copied.  
         If hatch_pattern.HatchPatternName() is empty, then a unique name
         of the form "Hatch Pattern 01" will be automatically created.
  
    worksession_ref_model_sn - [in]
          0: hatch_pattern is not a reference layer
          1: hatch_pattern is a reference linetype but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number   

    linked_idef_sn - [in]
          0: hatch_pattern is not from a liked instance definition
     1-1000: reserved for future use
      >1000: instance definition serial number      
  
   Returns:
     @untitled table
     >=0     index of new hatch pattern
     -1      hatch pattern not added because a hatch pattern with that name
             already exists.
  */
  const ON_ComponentManifestItem& CreateHatchPattern( 
     const ON_HatchPattern& hatch_pattern,
     unsigned int worksession_ref_model_sn,
     unsigned int linked_idef_sn
     );

  // Description:
  //   Adds a new hatch pattern with default definition to the hatch pattern table.
  //
  // Parameters:
  //   bReference - [in] true if this hatch pattern is a reference pattern.
  //       Reference hatch patterns are not saved in files.
  //
  // Returns:
  //   index of new hatch pattern
  //
  // Remarks:
  //   In some cases, calling AddHatchPattern() can cause the memory 
  //   references previously returned by operator[] and 
  //   CurrentHatchPattern() to become invalid.
  //
  const ON_ComponentManifestItem& AddHatchPattern( bool bReference);

  // Description:
  //   Adds a new hatch pattern with specified definition to the hatch pattern table.
  //
  // Parameters:
  //   pattern - [in] definition of new hatch pattern.  The information
  //       in pattern is copied.  If pattern.Name() is empty
  //       the a unique name of the form "HatchPattern 01" will be
  //       automatically created.
  //
  //   bReference - [in] true if this layer is a reference hatch pattern.
  //       Reference hatch patterns are not saved in files.
  //
  //   pIdx - [out]  May be NULL.  If the hatch pattern is added, the index assigned it 
  //                 is returned here.
  //                 If the pattern is not added because of a name collision, the index
  //                 of the colliding record is returned here
  //                 If the pattern was not added because of some other problem, ON_UNSET_INT_INDEX is returned.
  //
  // Returns:
  //   @untitled table
  //   true    hatch pattern was added
  //   false   hatch pattern not added
  const ON_ComponentManifestItem& AddHatchPattern( 
    const ON_HatchPattern& pattern,
    bool bReference, 
    int* pIdx = NULL
    );

  // Description:
  //   Modify hatch pattern settings
  //
  // Parameters:
  //   pattern - [in] new definition.  This information is copied.
  //   pattern_index - [in] zero based index of pattern to set.  This
  //        must be in the range 0 <= pattern_index < HatchPatternCount().
  //   bQuiet - [in] if true, information message boxes pop up
  //       when illegal changes are attempted.
  //
  // Returns:
  //   true if successful. false if pattern_index is out of range.
  //
  bool ModifyHatchPattern( const ON_HatchPattern& pattern, int pattern_index, bool bQuiet = false);
  bool ModifyHatchPattern( const ON_HatchPattern& pattern, const ON_ComponentManifestItem& item, bool bQuiet = false);

  /*
  Description:
    If the hatch pattern has been modified and the modification can be
    undone, then UndoModifyHatchPattern() will restore the hatch 
    pattern to its previous state.
 
  Returns:
    True if the hatch pattern had been modified and the modifications were
    undone.
 
  See Also: CRhinoHatchPattern::IsModified.
  */
  bool UndoModifyHatchPattern( 
    int hatchpattern_index, 
    unsigned int ur_sn = 0 
    );

  // Description:
  //   Deletes hatch pattern
  //
  // Parameters:
  //   pattern_index - [in] zero based index of pattern to delete.
  //       This must be in the range 0 <= pattern_index < HatchPatternCount().
  //   bQuiet - [in] If true, no warning message box appears if a layer
  //       the hatch pattern cannot be deleted because it is the current hatch pattern
  //       or it contains active geometry.
  //
  // Returns:
  //   true if successful. false if pattern_index is out of range or the
  //   the hatch pattern cannot be deleted because it is the current hatch pattern or
  //   because it hatch pattern contains active geometry.
  bool DeleteHatchPattern( int pattern_index, bool bQuiet);
  bool DeleteHatchPattern( const ON_ComponentManifestItem& item, bool bQuiet);

  // Description:
  //   Deletes multiple hatch patterns
  //
  // Parameters:
  //   pattern_index_count - [in] length of pattern_index[] array.
  //   pattern_index - [in] array zero based indices of hatch patterns to delete.
  //       The indices must be in the range 0 <= pattern_index < HatchPatternCount().
  //   bQuiet - [in] If true, no warning message box appears if a hatch pattern
  //       cannot be deleted because it is the current hatch pattern or it 
  //       contains active geometry.
  //
  // Returns:
  //   true if successful. false if pattern_index is out of range or the
  //   the hatch pattern cannot be deleted because it is the current pattern or
  //   because there is geometry referencing this hatch pattern.
  bool DeleteHatchPatterns( int pattern_index_count, const int* pattern_index, bool bQuiet);

  // Description:
  //   Undeletes a hatch pattern that has been deleted by DeleteHatchPattern().
  //
  // Parameters:
  //   pattern_index - [in] zero based index of tyle to undelete.
  //       This must be in the range 0 <= pattern_index < HatchPatternCount().
  //
  // Returns:
  //   true if successful.
  bool UndeleteHatchPattern( int pattern_index);
  bool UndeleteHatchPattern( const ON_ComponentManifestItem& item );

  // Description:
  //   Use this to set CRhinoHatchPattern::m_sort_index so that
  //   the values of m_sort_index run from 0 to HatchPatternCount()-1
  //   and compare(HatchPattern_table[i],HatchPattern_table[j]) < 0 ) implies 
  //   HatchPattern_table[i].m_sort_index < HatchPattern_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoHatchPattern* arg1,const CRhinoHatchPattern* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       bool cull(const CRhinoHatchPattern* arg,void* p).
  //       that returns true if the hatch pattern should be ignored
  //       when sorting.  Hatch patterns that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull layers
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoHatchPattern pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the layer table.  They are intended to be used to get
  //   sorted lists of hatch patterns for dialogs, etc.
  void Sort( int (*compar)(const CRhinoHatchPattern*,const CRhinoHatchPattern*,void*),
             int (*cull)(const CRhinoHatchPattern*,void*),
             void* p);

  // Description:
  //   Gets an array of pointers to hatch patterns that is sorted by
  //   the values of CRhinoHatchPattern::m_sort_index.
  //
  // Parameters:
  //   sorted_list - [out] this array is returned with length
  //       HatchPatternCount() and is sorted by the values of
  //       CRhinoLayer::m_sort_index.
  //   bIgnoreDeleted - [in] true means don't include
  //       deleted hatch patterns.
  //
  // Remarks:
  //   Use Sort() to set the values of m_sort_index.
  void GetSortedList( 
    ON_SimpleArray<const CRhinoHatchPattern*>& sorted_list,
    bool bIgnoreDeleted = false
    ) const;

  void GetSortedList( 
    ON_SimpleArray<const CRhinoHatchPattern*>& sorted_list,
    bool bIgnoreDeleted,
    bool bIgnoreReference
    ) const;


  /*
  Description:
    Draw hatch pattern to a display context. 
    Use this function to create preview images of hatch patterns.
  Parameters:
    pattern_index [in] - Index of the hatch pattern, in this table, to draw.
    hdc [in] - The display context to draw into
    rect [in] - The size of hatch pattern rectangle to draw.
    angle [in] - The rotation angle of the pattern display in radians.
    bkColor [in] - The background color.
    fgColor [in] - The foreground color.
  */
  void DrawPattern(
    int pattern_index,
#if defined ON_RUNTIME_WIN
    HDC hdc,
#else
    CRhinoDib&,
#endif
    const ON_4iRect& rect,
    double angle,
    COLORREF bkColor = RGB(255, 255, 255),
    COLORREF fgColor = RGB(0, 0, 0)
  ) const;

  /*
  Description:
    Draw hatch pattern to a display context.
    Use this function to create preview images of hatch patterns.
  Parameters:
    pattern [in] - The hatch pattern to draw.
    hdc [in] - The display context to draw into
    rect [in] - The size of hatch pattern rectangle to draw.
    angle [in] - The rotation angle of the pattern display in radians.
    bkColor [in] - The background color.
    fgColor [in] - The foreground color.
  */
  void DrawPattern(
    const ON_HatchPattern& pattern,
    HDC pDC,
    const ON_4iRect& rect,
    double angle,
    COLORREF bkColor = RGB(255, 255, 255),
    COLORREF fgColor = RGB(0, 0, 0)
  ) const;


  void SetRemapIndex( int pattern_index, int remap_index);

  void ListTable(
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

  /*
  Returns:
  true
  The hatch_pattern is an element in this hatch pattern table.
  */
  bool InHatchPatternTable(const ON_HatchPattern* hatch_pattern) const;

  /*
  Returns:
  true
  The hatch_pattern is a default hatch pattern.
  */
  bool IsBuiltinHatchPattern(const ON_HatchPattern* hatch_pattern) const;

  /*
  Returns:
    true 
      The hatch_pattern_index is a valid index of a hatch pattern
      element in this hatch pattern table.    
  */
  bool IsValidHatchPatternTableIndex( int hatch_pattern_index ) const;

  /*
  Description:
    Finds the hatch pattern with a given name.
  Parameters:
    name - [in] name of hatch pattern to search for. The search ignores case.
    bIgnoreDeletedStyles - [in] parameter is unused.
  Returns:
    index  = index of the hatch pattern with the given name.
    -(DefaultHatchPatternCount + 1) : no hatch pattern has the given name.
  */
  ON_DEPRECATED_MSG("use CRhinoHatchPatternTable::HatchPatternFromName()")
  int FindHatchPattern(const wchar_t* name, bool bIgnoreDeleted = true) const;

  /*
  Description:
    Returns true if a hatch pattern is use by an object, layer, or instance definition.
  Parameters:
    hatch_pattern_index - [in] index of hatch pattern to query.
  Returns:
    true if successful.
  */
  bool InUse(int hatch_pattern_index) const;

private:
  friend class CRhinoDoc;
  CRhinoDoc& m_doc;

private:
  ON_ComponentManifest& Internal_Manifest();
  CRhinoDocTableReference m_table_reference;

private:
  mutable unsigned int m_unused_hatch_pattern_name_suffix = 0;

  bool Internal_DeleteHatchPatternHelper(
    int pattern_index, 
    bool bQuiet,
    bool bDeleteReferenceHatches,
    bool bPurge
    );

  unsigned int Internal_PurgeWorkSessionReferenceModel( 
    unsigned int worksession_ref_model_sn
    );

  CRhinoHatchPatternTable() = delete;
  CRhinoHatchPatternTable(const CRhinoHatchPatternTable&) = delete;
  CRhinoHatchPatternTable& operator=(const CRhinoHatchPatternTable&) = delete;

  bool Internal_AddUndoModifyHatchPatternRecord(int hatchpattern_index);

  // Delete all CRhinoHatchPattern.m_undo information
  void Internal_ClearUndoRecords();

  void Internal_Destroy( class CRhinoEventWatcher* ew );
  void Internal_Destroy();

private:
  // These must be private.
  CRhinoHatchPattern m_Solid;
  CRhinoHatchPattern m_Hatch1;
  CRhinoHatchPattern m_Hatch2;
  CRhinoHatchPattern m_Hatch3;
  CRhinoHatchPattern m_HatchDash;
  CRhinoHatchPattern m_Grid;
  CRhinoHatchPattern m_Grid60;
  CRhinoHatchPattern m_Plus;
  CRhinoHatchPattern m_Squares;

private:
  mutable const CRhinoHatchPattern* m_current_hatch_pattern = nullptr;

public:
  // Built-in hatch patterns.
  // Localized names, id set, unique, and persistent
  const CRhinoHatchPattern& Solid;     // index = -1, id set, unique, and persistent
  const CRhinoHatchPattern& Hatch1;    // index = -2, id set, unique, and persistent
  const CRhinoHatchPattern& Hatch2;    // index = -3, id set, unique, and persistent
  const CRhinoHatchPattern& Hatch3;    // index = -4, id set, unique, and persistent
  const CRhinoHatchPattern& HatchDash; // index = -5, id set, unique, and persistent
  const CRhinoHatchPattern& Grid;      // index = -6, id set, unique, and persistent
  const CRhinoHatchPattern& Grid60;    // index = -7, id set, unique, and persistent
  const CRhinoHatchPattern& Plus;      // index = -8, id set, unique, and persistent
  const CRhinoHatchPattern& Squares;   // index = -9, id set, unique, and persistent

  enum : unsigned int
  {
    DefaultHatchPatternCount = 9
  };

  /*
  Description:
    Get a list of the default hatch patterns.
  Parameters:
    default_hatch_pattern_list_capacity - [in]
      Capacity of the default_hatch_pattern_list[] array.
    default_hatch_pattern_list - [out]
      List of default hatch patterns.
  Returns:
    Number of hatch patterns added to the list.
  Remarks:
    The hatch pattern table has CRhinoHatchPatternTable::DefaultHatchPatternCount 
    default hatch patterns.
  */
  unsigned int GetDefaultHatchPatterns(
    size_t default_hatch_pattern_list_capacity,
    const CRhinoHatchPattern** default_hatch_pattern_list
    ) const;

  /*
  Description:
    Get a list of the default hatch patterns
  Returns:
    Number of hatch patterns added to the list.
  Remarks:
    The hatch pattern table has CRhinoHatchPatternTable::DefaultHatchPatternCount 
    default hatch patterns.
  */
  unsigned int GetDefaultHatchPatterns(
    ON_SimpleArray<const CRhinoHatchPattern*>& default_hatch_pattern_list
    ) const;

  /*
  Description:
    Get the default hatch pattern identified by hatch_pattern_id.
  Parameters:
    hatch_pattern_id - [in]
  Returns:
    A pointer to the default hatch pattern with Id() = dimension_style_id
    or nullptr if hatch_pattern_id does not identify a default hatch pattern.
  */
  const class CRhinoHatchPattern* DefaultHatchPatternFromId(
    ON_UUID hatch_pattern_id
    ) const;

  /*
  Description:
    Get the default hatch pattern identified by hatch_pattern_manifest_item.
  Parameters:
    hatch_pattern_manifest_item - [in]
  Returns:
    A pointer to the default hatch pattern with Id() = dimension_style_id
    or nullptr if hatch_pattern_id does not identify a default hatch pattern.
  */
  const class CRhinoHatchPattern* DefaultHatchPattern(
    const ON_ComponentManifestItem& hatch_pattern_manifest_item
    ) const;

  /*
  Description:
    Get the default hatch pattern to use when no identification is available.
  */
  const class CRhinoHatchPattern& DefaultHatchPattern() const;
};


/*
  Description
    Ask user to select an existing HatchPattern
  Parameters:
    rhino_doc_sn - [in]
      Required to identify and existing document.
    [in] interactive - if true, a dialog is used, otherwise, the commandline is used
  Returns:
    integer index of selected pattern or ON_UNSET_INT_INDEX for none.
*/
RHINO_SDK_FUNCTION
int RhinoSelectHatchPattern(
  unsigned int rhino_doc_sn,
  bool interactive
  );

