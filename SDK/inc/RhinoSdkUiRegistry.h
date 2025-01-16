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

//////////////////////////////////////////////////////////////
//
// class CRhinoUiRegistry
//
class RHINO_SDK_CLASS CRhinoUiRegistryEnumValue
{
public:
  CRhinoUiRegistryEnumValue();
  CRhinoUiRegistryEnumValue( const CRhinoUiRegistryEnumValue&);
  ~CRhinoUiRegistryEnumValue();
  CRhinoUiRegistryEnumValue& operator=( const CRhinoUiRegistryEnumValue&);

  LPCTSTR ValueName() const;
  // Extractected from MSDN
  //  Platform SDK: Windows System Information 
  // Registry Value Types
  //-------------------------------------------------------------------------------------------------
  // Value                    Meaning 
  //-------------------------------------------------------------------------------------------------
  // REG_BINARY               Binary data in any form. 
  //-------------------------------------------------------------------------------------------------
  // REG_DWORD                A 32-bit number. 
  //-------------------------------------------------------------------------------------------------
  // REG_DWORD_LITTLE_ENDIAN  A 32-bit number in little-endian format. 
  //                          Microsoft® Windows® is designed to run on little-endian computer
  //                          architectures. Therefore, this value is defined as REG_DWORD in
  //                          the Windows header files.
  //-------------------------------------------------------------------------------------------------
  // REG_DWORD_BIG_ENDIAN     A 32-bit number in big-endian format. 
  //                          Some UNIX systems support big-endian architectures.
  //-------------------------------------------------------------------------------------------------
  // REG_EXPAND_SZ            Null-terminated string that contains unexpanded references to
  //                          environment variables (for example, "%PATH%"). It will be a Unicode
  //                          or ANSI string depending on whether you use the Unicode or ANSI
  //                          functions. To expand the environment variable references, use the
  //                          ExpandEnvironmentStrings function. 
  //-------------------------------------------------------------------------------------------------
  // REG_LINK                 Reserved for system use. 
  //-------------------------------------------------------------------------------------------------
  // REG_MULTI_SZ             Array of null-terminated strings, terminated by two null characters. 
  //-------------------------------------------------------------------------------------------------
  // REG_NONE                 No defined value type. 
  //-------------------------------------------------------------------------------------------------
  // REG_QWORD                A 64-bit number. 
  //-------------------------------------------------------------------------------------------------
  // REG_QWORD_LITTLE_ENDIAN  A 64-bit number in little-endian format. 
  //                          Windows is designed to run on little-endian computer architectures.
  //                          Therefore, this value is defined as REG_QWORD in the Windows header
  //                          files.
  //-------------------------------------------------------------------------------------------------
  // REG_SZ                   Null-terminated string. It will be a Unicode or ANSI string, depending
  //                          on whether you use the Unicode or ANSI functions. 
  //-------------------------------------------------------------------------------------------------
  DWORD ValueType() const;
  const LPBYTE Value() const;
  const DWORD ValueLength() const;
  operator DWORD() const;
  operator LPCTSTR() const;
protected:
  friend class CRhinoUiRegistry;
  CString m_sValueName;
  DWORD m_nValueType;
  LPBYTE m_pData;
  DWORD  m_nDataLen;
};

//////////////////////////////////////////////////////////////
//
// class CRhinoUiRegistry
//
class RHINO_SDK_CLASS CRhinoUiRegistry
{
public:
  CRhinoUiRegistry();
  CRhinoUiRegistry( CRhinoUiRegistry&);
  CRhinoUiRegistry( HKEY hKey);

  ~CRhinoUiRegistry();

  CRhinoUiRegistry& operator=( CRhinoUiRegistry&);
  operator HKEY() const;

  HKEY GetHKEY() const;

  // Detach the CRegKey object from its HKEY.  Releases ownership.
  HKEY Detach();
  // Attach the CRegKey object to an existing HKEY.  Takes ownership.
  void Attach( HKEY hKey);

  bool AttachToRhinoRootRegistryKey( bool bWrite = false, CRhinoProfileContext::context_location nProfile = CRhinoProfileContext::cl_scheme_current_user);
  bool AttachToRhinoAppRegistryKey( bool bWrite = false, CRhinoProfileContext::context_location nProfile = CRhinoProfileContext::cl_scheme_current_user);
  bool AttachToRhinoSectionKey( LPCTSTR lpszSection, bool bWrite = false, CRhinoProfileContext::context_location nProfile = CRhinoProfileContext::cl_scheme_current_user);

  bool Create( HKEY hKeyParent, LPCTSTR lpsKeyName, bool* pbIsNewKey = NULL);
  bool Open( HKEY hKeyParent, LPCTSTR lpsKeyName, bool bWrite = false);
  bool Close();
  bool Flush();

  bool QueryValueSize( LPCTSTR lpsValueName, DWORD& dwSize) const;
  bool QueryValueType( LPCTSTR lpsValueName, DWORD& dwType) const;
  bool QueryValue( LPCTSTR lpsValueName, DWORD* pdwType, LPBYTE pData, DWORD& dwData) const;
  bool QueryDWORDValue( LPCTSTR lpsValueName, DWORD& dwValue) const;
  bool QueryBinaryValue( LPCTSTR lpsValueName, void* pValue, ULONG& nBytesOut) const;
  bool QueryStringValue( LPCTSTR lpsValueName, CString& sValue) const;
  bool QueryMultiStringValue( LPCTSTR lpsValueName, ON_ClassArray<CString>& sValues) const;
  bool QueryBoolValue( LPCTSTR lpsValueName, bool& bValue) const;
  bool QueryBoolValue( LPCTSTR lpsValueName, BOOL32& bValue) const;
  bool QueryIntValue( LPCTSTR lpsValueName, int& iValue) const;
  bool QueryDoubleValue( LPCTSTR lpsValueName, double& nValue) const;
  bool QueryUUIDValue( LPCTSTR lpsValueName, UUID& uuid) const;
  bool QueryRGBValue( LPCTSTR lpsValueName, COLORREF& cr) const;
  bool QueryRectValue( LPCTSTR lpsValueName, CRect& rValue) const;
  bool QueryPointValue( LPCTSTR lpsValueName, CPoint& ptValue) const;
  bool Query3dPointValue( LPCTSTR lpsValueName, ON_3dPoint& ptValue) const;
  bool Query2dPointValue( LPCTSTR lpsValueName, ON_2dPoint& ptValue) const;
  bool Query3dVectorValue( LPCTSTR lpsValueName, ON_3dVector& vValue) const;
  bool QuerySizeValue( LPCTSTR lpsValueName, CSize& szValue) const;

  bool SetValue( LPCTSTR lpsValueName, DWORD dwType, const LPBYTE pData, DWORD dwData) const;
  bool SetDWORDValue( LPCTSTR lpsValueName, DWORD dwValue) const;
  bool SetBinaryValue( LPCTSTR lpsValueName, const void* pValue, ULONG nBytes) const;
  bool SetStringValue( LPCTSTR lpsValueName, LPCTSTR lpsValue) const;
  bool SetStringValueW( LPCTSTR lpsValueName, const wchar_t* lpsValue) const;
  bool SetStringValueA( LPCTSTR lpsValueName, const char* lpsValue) const;
  bool SetMultiStringValue( LPCTSTR lpsValueName, const ON_ClassArray<CString>& sValues) const;
  bool SetMultiStringValueW( LPCTSTR lpsValueName, const ON_ClassArray<ON_wString>& sValues) const;
  bool SetMultiStringValueA( LPCTSTR lpsValueName, const ON_ClassArray<ON_String>& sValues) const;
  bool SetBoolValue( LPCTSTR lpsValueName, bool b) const;
  bool SetBoolValue( LPCTSTR lpsValueName, BOOL32 b) const;
  bool SetIntValue( LPCTSTR lpsValueName, int i) const;
  bool SetDoubleValue( LPCTSTR lpsValueName, double n) const;
  bool SetUUIDValue( LPCTSTR lpsValueName, const UUID& uuid) const;
  bool SetRGBValue( LPCTSTR lpsValueName, COLORREF cr) const;
  bool SetRectValue( LPCTSTR lpsValueName, CRect r) const;
  bool SetPointValue( LPCTSTR lpsValueName, CPoint pt) const;
  bool Set3dPointValue( LPCTSTR lpsValueName, ON_3dPoint pt) const;
  bool Set2dPointValue( LPCTSTR lpsValueName, ON_2dPoint pt) const;
  bool Set3dVectorValue( LPCTSTR lpsValueName, ON_3dVector v) const;
  bool SetSizeValue( LPCTSTR lpsValueName, CSize sz) const;

  bool DeleteSubKey( LPCTSTR lpszSubKey) const;
  bool DeleteValue( LPCTSTR lpszValue) const;

  int EnumKey();
  int KeyCount() const;
  LPCTSTR KeyString( int) const;

  int EnumValue();
  int ValueCount() const;
  const CRhinoUiRegistryEnumValue* Value( int) const;
  const CRhinoUiRegistryEnumValue* Value( LPCTSTR) const;

  bool SetSchemeName( LPCTSTR lpsScheme);

protected:
  bool QueryNumberList( LPCTSTR lpsValueName, int* iList, int iCount) const;
  bool QueryNumberList( LPCTSTR lpsValueName, double* iList, int iCount) const;

protected:
  HKEY m_hKey;
  CStringArray m_sKeyList;
  ON_ClassArray<CRhinoUiRegistryEnumValue>m_LabelList;
  CString m_sSchemeName;
};
