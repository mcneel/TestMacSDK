//
// Copyright (c) 1993-2024 Robert McNeel & Associates. All rights reserved.
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

#if defined(INCLUDE_RHINO_SECTION_STYLE_TABLE)

//////////////////////////////////////////////////////////////////////////
//
// CRhinoSectionStyle contains a named section style definition and
// is stored in the CRhinoDoc section style table.
//
class RHINO_SDK_CLASS CRhinoSectionStyle : public ON_SectionStyle
{
public:
  // Runtime index used to sort section styles
  int m_sort_index = -1;

  // Runtime index used when remapping section styles for import/export
  int m_remap_index = -1;
private:
  friend class CRhinoSectionStyleTable;

  // constructor and destructors are private because only CRhinoSectionStyleTable
  // should be creating CRhinoSectionStyles.
  CRhinoSectionStyle(const ON_SectionStyle&);
  ~CRhinoSectionStyle();
  CRhinoSectionStyle() = delete;
  CRhinoSectionStyle( const CRhinoSectionStyle& ) = delete;
  CRhinoSectionStyle& operator=(const CRhinoSectionStyle&) = delete;
};

class RHINO_SDK_CLASS CRhinoSectionStyleTable : private ON_SimpleArray<CRhinoSectionStyle*>
{
public:
  // Description:
  //   SectionStyle tables store the list of named section styles in a Rhino document.
  //  Returns:
  //   CRhinoDocument that owns this section style table.
  CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;

  // Returns:
  //   Number of section styles in the table
  int SectionStyleCount() const;

  // Description:
  //   Conceptually, the section style table is an array of SectionStyleCount()
  //   section styles.  The operator[] can be used to get individual section
  //   styles.  A section style is either active or deleted and this state is
  //   reported by CRhinoSectionStyle::IsDeleted().
  // Parameters:
  //   index - zero based array index
  // Returns:
  //   Pointer to the section style.  If index is out of range, nullptr is
  //   returned. Note that this pointer may become invalid after
  //   AddSectionStyle() is called.
  const CRhinoSectionStyle* operator[](int index) const;

  // Description:
  //   Finds the section style with a given name.
  // Parameters:
  //   section_style_name - [in] Case is ignored.
  // Returns:
  //   index for the section style on success
  //   ON_UNSET_INT_INDEX if no section style has the name
  int FindSectionStyleFromName(const wchar_t* section_style_name) const;

  // Description:
  //   Finds the section style with a given id.
  // Parameters:
  //   section_style_id - [in]
  //   bSearchDeletedSectionStyles - [in]
  // Returns:
  //   index for the section style on success
  //   ON_UNSET_INT_INDEX if no section style has the id
  int FindSectionStyleFromId(ON_UUID section_style_id, bool bSearchDeletedSectionStyles) const;

  // Description:
  //   Adds a new section style to the table.
  // Returns:
  //   index for the section style on success
  int AddSectionStyle();

  // Description:
  //   Adds a new section style to the table.
  // Parameters:
  //   section_style - [in]
  //    The name and id are used as candidates.
  //    If they are in use, a new name or id is created.
  // Returns:
  //   index for the section style on success
  //   ON_UNSET_INT_INDEX if input is not valid
  int AddSectionStyle(const ON_SectionStyle& section_style, bool reference);

  // Description:
  //   Basic tool used to create a new section style with specified definition
  //   to the section style table.  The AddSectionStyle() functions all use
  //   CreateSectionStyle() when a new section style needs to be added to the
  //   section style table.
  // Parameters:
  //   section_style - [in]
  //     The name and id are used as candidates. If they are in use, a new name
  //     or id is created.
  //   worksession_ref_model_sn - [in]
  //        0: not from a reference model
  //        1: not saved in files but not part of a worksession reference file
  //   2-1000: reserved for future use
  //    >1000: worksession reference file serial number
  //  linked_idef_sn - [in]
  //        0: not from a liked instance definition
  //   1-1000: reserved for future use
  //    >1000: group instance definition serial number
  // Returns:
  //   index of new section style on success.
  //   ON_UNSET_INT_INDEX on failuer
  int CreateSectionStyle(
    const ON_SectionStyle& section_style,
    unsigned int worksession_ref_model_sn,
    unsigned int linked_idef_sn);

  // Description:
  //   Change section style settings
  // Parameters:
  //   section_style - [in] new settings.  This information is copied.
  //   index - [in] zero based index of section_style to set. This can be in
  //        the range 0 <= index <= SectionStyleCount().
  //        If index==SectionStyleCount(), a new section style is added.
  //   quiet - [in] true to disable message boxes when illegal names are used.
  // Returns:
  //   true if successful. false if index is out of range.
  bool ModifySectionStyle(
    const ON_SectionStyle& section_style,
    int index,
    bool quiet = false);

  // Description:
  //   Delete a section style
  // Parameters:
  //   index - [in] zero based index of section style to delete.
  //       This must be in the range 0 <= index < SectionStyleCount().
  //   quiet - [in] true to disable message boxes when illegal names are used.
  // Returns:
  //   true if successful. false if index is out of range or the section style
  //   cannot be deleted because it is the current section style or because it
  //   contains active geometry.
  bool DeleteSectionStyle(int index, bool quiet);
  
  bool UndeleteSectionStyle(int index);

  void GetUnusedSectionStyleName(ON_wString& unused_section_style_name) const;
  void GetUnusedSectionStyleName(const wchar_t* root_name, ON_wString& unused_section_style_name) const;

  // Returns:
  //   true if the section style is an element in this table
  bool InSectionStyleTable(const ON_SectionStyle* section_style) const;

  // Returns:
  //   true if the section_style_index is a valid index of a section style
  //   in this table
  bool IsValidSectionStyleTableIndex(int index) const;

private:
  friend class CRhDoc;
  CRhinoSectionStyleTable(CRhinoDoc&);
  ~CRhinoSectionStyleTable();

  bool DeleteSectionStyleHelper(
    int section_style_index,
    bool bQuiet,
    int* delete_referenced_section_styles,
    int* ref_file_warning_enabled);

private:
  CRhinoSectionStyleTable() = delete;
  CRhinoSectionStyleTable(const CRhinoSectionStyleTable&) = delete;
  CRhinoSectionStyleTable& operator=(const CRhinoSectionStyleTable&) = delete;

private:
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;
  ON_ComponentManifest& Internal_Manifest();
};

#endif
