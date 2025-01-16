/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

class RHINO_SDK_CLASS CRhinoProfileContextIterator
{
public:
  CRhinoProfileContextIterator(const wchar_t* section, CRhinoProfileContext& pc);
  virtual ~CRhinoProfileContextIterator();

  // Must override
  virtual const wchar_t* FirstSection() = 0;
  virtual const wchar_t* NextSection() = 0;
  virtual const wchar_t* FirstValue() = 0;
  virtual const wchar_t* NextValue() = 0;

  int IterateAllValues(bool(*cull)(ON_wString& value_name, ON_wString& value) = NULL);

  const wchar_t* Value() const;
  bool Value(ON_wString& result) const;
  bool Value(ON_Color& result) const;
  bool Value(bool& result) const;
  bool Value(int& result) const;
  bool Value(double & result) const;
  bool Value(RECT& result) const;
  bool Value(POINT& result) const;
  bool Value(ON_3dPoint& result) const;
  bool Value(ON_3dVector& result) const;

  int ValueCount() const;
  const ON_ClassArray<ON_wString>& ValueNames() const;
  const ON_ClassArray<ON_wString>& Values() const;

  const wchar_t* ValueName(int index) const;

  bool Value(int index, ON_wString& result) const;
  bool Value(int index, ON_Color& result) const;
  bool Value(int index, bool& result) const;
  bool Value(int index, int& result) const;
  bool Value(int index, double & result) const;
  bool Value(int index, RECT& result) const;
  bool Value(int index, POINT& result) const;
  bool Value(int index, ON_3dPoint& result) const;
  bool Value(int index, ON_3dVector& result) const;

  int IterateAllSections(bool(*cull)(const wchar_t*) = NULL);

  const ON_ClassArray<ON_wString>& Sections() const;
  const wchar_t* Section(int index) const;
  int SectionCount() const { return m_sections.Count(); }

protected:
  CRhinoProfileContext& m_pc;
  ON_wString m_section;
  ON_wString m_w;
  ON_wString m_value_name;
  ON_wString m_value;
  int m_item;

  ON_ClassArray<ON_wString>m_value_names;
  ON_ClassArray<ON_wString>m_values;
  ON_ClassArray<ON_wString>m_sections;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoRegProfileContextIterator
//
class RHINO_SDK_CLASS CRhinoRegProfileContextIterator : public CRhinoProfileContextIterator
{
public:
  CRhinoRegProfileContextIterator(const wchar_t* section, CRhinoProfileContext& pc);
  virtual ~CRhinoRegProfileContextIterator();

  const wchar_t* FirstSection();
  const wchar_t* NextSection();
  const wchar_t* FirstValue();
  const wchar_t* NextValue();

protected:
  const wchar_t* Close();

protected:
  HKEY m_hKey;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoIniProfileContextIterator
//
class RHINO_SDK_CLASS CRhinoIniProfileContextIterator : public CRhinoProfileContextIterator
{
public:
  CRhinoIniProfileContextIterator(const wchar_t* section, CRhinoProfileContext& pc);
  virtual ~CRhinoIniProfileContextIterator();

  const wchar_t* FirstSection();
  const wchar_t* NextSection();
  const wchar_t* FirstValue();
  const wchar_t* NextValue();

protected:
  bool LoadIniFile(CRhinoIniProfileContext& pc, bool bForceReload);
  ON_wString FormatSectionString(const ON_wString& section) const;
  int KeyIndex(const ON_wString& w) const;
  ON_wString ExtractSubKey(const ON_wString& section, const ON_wString& key) const;

  class CRhinoIniFile* m_pIniFile;
  int m_iIniOffset;
  int m_iKeyIndex;
  ON_ClassArray<ON_wString>m_key_list;
};
