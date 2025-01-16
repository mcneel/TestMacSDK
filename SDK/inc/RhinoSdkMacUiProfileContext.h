//
//
// NEVER INCLUDE FILES FROM CORE RHINO IN ANY SDK/inc/rhinoSDK*.h file
//
//
#pragma once

#include "RhinoSdkUiProfileContextIterator.h"

@class MRPropertyList;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoPropListProfileContextIterator
//
class RHINO_SDK_CLASS CRhinoPropListProfileContextIterator : public CRhinoProfileContextIterator
{
public:
  CRhinoPropListProfileContextIterator( const wchar_t* section, CRhinoProfileContext& pc);
  virtual ~CRhinoPropListProfileContextIterator();
  
  const wchar_t* FirstSection();
  const wchar_t* NextSection();
  const wchar_t* FirstValue();
  const wchar_t* NextValue();

protected:
  ON_wString FormatSectionString( const ON_wString& section) const;
  bool LoadIniFile( CRhinoIniProfileContext& pc, bool bForceReload);
  int KeyIndex( const ON_wString& w) const;
  ON_wString ExtractSubKey( const ON_wString& section, const ON_wString& key) const;
  
  //class CIniFile* m_pIniFile;
  INT_PTR m_iIniOffset;
  INT_PTR m_iKeyIndex;
  ON_ClassArray<ON_wString>m_key_list;
  ON_ClassArray<ON_wString>m_value_list;
  
};


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoPropListProfileContext
//
class RHINO_SDK_CLASS CRhinoPropListProfileContext : public CRhinoProfileContext
{
public:
  CRhinoPropListProfileContext( CRhinoProfileContext::context_location ct);
  virtual ~CRhinoPropListProfileContext();

  //-----------------------------------------------------------------------------------------------
  //
  // Required CRhinoProfileContext overrides
  //
  CRhinoProfileContextIterator* GetNewIterator( const wchar_t* section);
  bool SaveProfileString(const wchar_t* lpszSection, LPCTSTR lpszEntry, const char*) const;
  bool SaveProfileString(const wchar_t* lpszSection, LPCTSTR lpszEntry, const wchar_t*) const;

  bool LoadProfileString(const wchar_t* lpszSection, LPCTSTR lpszEntry, ON_wString& ) const;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_wString& value, const wchar_t* defaultValue) const;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_String& value, const char* defaultValue) const;

  bool LoadProfileStringDictionary(const wchar_t* lpszSection, const wchar_t* lpszEntry, CRhinoStringDictionary& value) const;
  bool LoadProfileStringDictionary(const wchar_t* lpszSection, const wchar_t* lpszEntry, CRhinoStringDictionary& value, const CRhinoStringDictionary& defaultValue) const;

  bool LoadProfileUuid(const wchar_t* lpszSection, const wchar_t* lpszEntry, UUID* value, const UUID& defaultValue) const;

  void SaveProfileColor( LPCTSTR lpszSection, LPCTSTR lpszEntry, ON_Color ) const;
  bool LoadProfileColor( LPCTSTR lpszSection, LPCTSTR lpszEntry, ON_Color* ) const;
  bool LoadProfileColor(const wchar_t* lpszSection, const wchar_t* lpszEntry, COLORREF* value, COLORREF defaultValue) const;

  //The LoadProfileColor/SaveProfileColor functions ignore alpha. If you want to save a
  //color with alpha information, use the following RGBA functions
  void SaveProfileRGBA(const wchar_t* lpszSection, LPCTSTR lpszEntry, const ON_Color&) const;
  bool LoadProfileRGBA(const wchar_t* lpszSection, LPCTSTR lpszEntry, ON_Color& ) const;
  bool LoadProfileRGBA(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Color& value, const ON_Color& defaultValue) const;

  void SaveProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, bool) const;
  bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, bool*) const;
  bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, bool* value, bool defaultValue) const;

  void SaveProfileBool(const wchar_t* lpszSection, LPCTSTR lpszEntry, BOOL32 ) const;
  bool LoadProfileBool(const wchar_t* lpszSection, LPCTSTR lpszEntry, BOOL32* ) const;
  bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, BOOL32* value, BOOL32 defaultValue) const;

  void SaveProfileInt(const wchar_t* lpszSection, LPCTSTR lpszEntry, int ) const;
  bool LoadProfileInt(const wchar_t* lpszSection, LPCTSTR lpszEntry, int* ) const;
  bool LoadProfileInt(const wchar_t* lpszSection, const wchar_t* lpszEntry, int* value, int defaultValue) const;

  bool LoadProfileInt(const wchar_t* lpszSection, LPCTSTR lpszEntry, BOOL32* ) const;
  
  void SaveProfileDouble(const wchar_t* lpszSection, LPCTSTR lpszEntry, double ) const;
  bool LoadProfileDouble(const wchar_t* lpszSection, LPCTSTR lpszEntry, double* ) const;
  bool LoadProfileDouble(const wchar_t* lpszSection, const wchar_t* lpszEntry, double* value, double defaultValue) const;


  //-----------------------------------------------------------------------------------------------
  //
  // Local public methods
  //
  void SetPropListFileName( const wchar_t*);
  id ProfileKeyFor(LPCTSTR lpszSection, LPCTSTR lpszEntry=NULL) const;

  virtual MRPropertyList* PropertyList() const;
protected:
    
    friend class CRhinoPropListProfileContextIterator;
  //-----------------------------------------------------------------------------------------------
  //
  // Local protected methods
  //
  bool RegWriteProfileString(const wchar_t* lpszSection, LPCTSTR lpszEntry, const class CString& sValue, CRhinoProfileContext::context_location nProfile) const;
protected:
  ON_wString m_wFileName;
  MRPropertyList* m_pPropertyList;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoPropListProfileContext
//
// Note: You must call SetPropListFileName() with a valid property list file name or all other methods will fail.
//
class RHINO_SDK_CLASS CRhinoPropListFileProfileContext : public CRhinoPropListProfileContext
{
public:
  CRhinoPropListFileProfileContext( CRhinoProfileContext::context_location ct);
  MRPropertyList* PropertyList() const;
  void SetPropertyList (MRPropertyList*);
};
