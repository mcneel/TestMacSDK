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

class CRhinoObject;

//////////////////////////////////////////////////////////////////////////
//
// CRhinoGroup contains a group definition and is stored in the
// CRhinoDoc group table.
//
class RHINO_SDK_CLASS CRhinoGroup : public ON_Group
{
public:
  /*
  Returns:
    Worksession reference model id.
         0: group is not in a reference model.
         1: group is in an unidentified worksession reference model.
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
      The group is not part of a linked instance definition
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
    True if the group is part of a worksession reference
    model or a linked instance definition model.  If IsReference()
    is true, then the group will not be saved as part
    of this document.
  See Also:
    CRhinoGroup::WorksessionReferenceModelSerialNumber()
    CRhinoGroup::LinkedInstanceDefinitionSerialNumber()
  */
  ON_DEPRECATED_MSG("use CRhinoGroup::IsReferenceComponent()")
  BOOL32 IsReference() const;
    
  /*
    Description:
      Get an array of all of the objects in this group
    Parameters:
      [out] members - an array to append the objects in the group
  */
  void Members( ON_SimpleArray<CRhinoObject*>& members) const;

  // Runtime index used to sort groups
  int m_sort_index = -1;   

  // Runtime index used when remapping groups for import/export
  int m_remap_index = -1;

private:
  friend class CRhinoGroupTable;

  // constructor and destructors are private because only CRhinoGroupTable
  // should be creating CRhinoGroups.
  CRhinoGroup( const ON_Group& );
  ~CRhinoGroup();
  CRhinoGroup() = delete;
  CRhinoGroup( const CRhinoGroup& ) = delete;
  CRhinoGroup& operator=(const CRhinoGroup&) = delete;
};

class RHINO_SDK_CLASS CRhinoGroupTable : private ON_SimpleArray< CRhinoGroup* >
{
public:

  // Description:
  //   Group tables store the list of groups in a
  //   Rhino document.
  //
  //  Returns:
  //    CRhinoDocument that owns this group table.
  CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;

  // Returns:
  //   Number of groups in the group table
  int GroupCount() const;

  // Description:
  //   Conceptually, the group table is an array of GroupCount()
  //   groups.  The operator[] can be used to get individual
  //   groups.  A group is either active or deleted and this
  //   state is reported by CRhinoGroup::IsDeleted().
  //
  // Parameters:
  //   group_index - zero based array index
  //
  // Returns:
  //   Pointer to the group.  If group_index is out of range,
  //   a NULL pointer is returned. Note that this pointer may become
  //   invalid after AddGroup() is called.
  const CRhinoGroup* operator[](
    int group_index
    ) const;

  /*
   Description:
     Finds the group with a given name.
  
   Parameters:
     group_name - [in] 
       Case is ignored.
     not_found_return_value - [in]
  
   Returns:
     >=0
       CRhinoDoc runtime group index of the group with name = group_name
     ON_UNSET_INT_INDEX
       no group has the name
  */
  int FindGroupFromName( 
    const wchar_t* group_name
    ) const;

  /*
   Description:
     Finds the group with a given id.
  
   Parameters:
     group_id - [in] 
     bSearchDeletedGroups - [in]
     not_found_return_value - [in]
  
   Returns:
     >=0
       CRhinoDoc runtime group index of the group with id = group_id
     ON_UNSET_INT_INDEX
       no group has the id
  */
  int FindGroupFromId( 
    ON_UUID group_id, 
    bool bSearchDeletedGroups
    ) const;

  /*
  Description:
    Adds a new group to the group table.
  Returns:
    @untitled table
    >=0     index of new group
    ON_UNSET_INT_INDEX
      group not added because input is not valid.
  */

  /*
  Description:
    Adds a new group to the group table.
  Parameters:
    group - [in]
      The name and id are used as candidates.
      If they are in use, a new name or id is created.
  Returns:
    @untitled table
    >=0     index of new group
    ON_UNSET_INT_INDEX
      group not added because input is not valid.
  */
  int AddGroup();
  int AddGroup( 
     const ON_Group& group
     );

  /*
  Description:
    Adds a new group to the group table.
  Parameters:
    group - [in]
      The name and id are used as candidates.
      If they are in use, a new name or id is created.
    group_members - [in] list of objects to add to the group.
  Returns:
    @untitled table
    >=0     index of new group
    ON_UNSET_INT_INDEX
      group not added because input is not valid.
  */
  int AddGroup( 
     ON_SimpleArray<const CRhinoObject*>& group_members
     );
  int AddGroup( 
     const ON_Group& group,
     ON_SimpleArray<const CRhinoObject*>& group_members
     );


  /*
   Description:
     Basic tool used to create a new group with specified 
     definition to the group table.  The AddGroup() functions
     all use CreateGroup() when a new group needs to be added
     to the group table.
  
   Parameters:
    group - [in]
      The name and id are used as candidates.
      If they are in use, a new name or id is created.
  
    worksession_ref_model_sn - [in]
          0: not from a reference model
          1: not saved in files but not part of a worksession reference file
     2-1000: reserved for future use
      >1000: worksession reference file serial number   

    linked_idef_sn - [in]
          0: not from a liked instance definition
     1-1000: reserved for future use
      >1000: group instance definition serial number      
  
   Returns:
     @untitled table
     >=0     index of new grpup
     ON_UNSET_INT_INDEX group not added
  */
  int CreateGroup(
    const ON_Group& group,
    unsigned int worksession_ref_model_sn,
    unsigned int linked_idef_sn
    );
  int CreateGroup(
    const ON_Group& group,
    ON_SimpleArray<const CRhinoObject*>& group_members,
    unsigned int worksession_ref_model_sn,
    unsigned int linked_idef_sn
    );

  // Description:
  //   Change group settings
  //
  // Parameters:
  //   group - [in] new settings.  This information is copied.
  //   group_index - [in] zero based index of group to set.  This
  //        can be in the range 0 <= group_index <= GroupCount().
  //        If group_index==GroupCount(), a new group is added.
  //   bQuiet - [in] TRUE to disable message boxes when illegal names
  //       are used.
  //
  // Returns:
  //   TRUE if successful. FALSE if group_index is out of range
  //   or the settings attempt
  //   to lock or hide the current group.
  //
  bool ModifyGroup( 
     const ON_Group& group,
     int group_index,
     bool bQuiet = false
     );

  // Description:
  //   Deletes group
  //
  // Parameters:
  //   group_index - [in] zero based index of group to delete.
  //       This must be in the range 0 <= group_index < GroupCount().
  //   bQuiet - [in] If TRUE, no warning message box appears if a group
  //       the group cannot be deleted because it is the current group
  //       or it contains active geometry.
  //
  // Returns:
  //   TRUE if successful. FALSE if group_index is out of range or the
  //   the group cannot be deleted because it is the current group or
  //   because it group contains active geometry.
  bool DeleteGroup(
     int // group_index
     );

  // Description:
  //   Undeletes a group that has been deleted by DeleteGroup().
  //
  // Parameters:
  //   group_index - [in] zero based index of group to undelete.
  //       This must be in the range 0 <= group_index < GroupCount().
  //
  // Returns:
  //   TRUE if successful.
  bool UndeleteGroup(
     int group_index
     );

  // Description:
  //   Use this to set CRhinoGroup::m_sort_index so that
  //   the values of m_sort_index run from 0 to GroupCount()-1
  //   and compare(group_table[i],group_table[j]) < 0 ) implies 
  //   group_table[i].m_sort_index < group_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoGroup* arg1,const CRhinoGroup* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       BOOL32 cull(const CRhinoGroup* arg,void* p).
  //       that returns TRUE if the group should be ignored
  //       when sorting.  Groups that are ignored are not included
  //       in the list returned by GetSortedList().  Pass 0 if
  //       you do not need to cull the list.
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoGroup pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the group table.  They are intended to be used to get
  //   sorted lists of groups for dialogs, etc.
  void Sort( 
    int (*compare)(const CRhinoGroup*,const CRhinoGroup*,void* p),
    int (*cull)(const CRhinoGroup*,void*),
    void* p=0
    );

  void GetUnusedGroupName(
    ON_wString& unused_group_name
    );

  void GetUnusedGroupName(
    const wchar_t* root_name,
    ON_wString& unused_group_name
    );

  /*
  Parameters:
    
    group_to_name - [out]
      Name is set to an unused group name.
  */
  void SetUnusedGroupName(
    ON_Group& group_to_name
    );

  void SetUnusedGroupName(
    const wchar_t* root_name,
    ON_Group& group_to_name
    );

  void SetRemapIndex(
    int, // layer_index
    int  // remap_index
    );

  void ListTable(
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

 /*
  Returns:
    true 
      The group is an element in this group table.
  */
  bool InGroupTable( const ON_Group* group ) const;

  /*
  Returns:
    true 
      The group_index is a valid index of a group
      element in this group table.    
  */
  bool IsValidGroupTableIndex( int group_index ) const;
  

private:
  friend class CRhinoDoc;
  CRhinoGroupTable(CRhinoDoc&);
  ~CRhinoGroupTable();

private:
  CRhinoGroupTable() = delete;
  CRhinoGroupTable(const CRhinoGroupTable&) = delete;
  CRhinoGroupTable& operator=(const CRhinoGroupTable&) = delete;

private:
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;
  ON_ComponentManifest& Internal_Manifest();

private:
  void Destroy( class CRhinoEventWatcher* ew );
  void Destroy();
};

