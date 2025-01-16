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

// Forward decloration
class CRhinoProfileContextIterator;

/*
Description:
  A collection of key value string pairs, each key name must be unique, key
  names are case sensitive.
*/
class RHINO_SDK_CLASS CRhinoStringDictionary
{
public:
  /*
  Description:
    Constructor
  */
  CRhinoStringDictionary();
  /*
  Description:
    Call this method to get the number of items in the dictionary.
  Returns:
    Returns the number of items in the dictionary
  */
  int Count() const;
  /*
  Description:
    Call this method to clear the dictionary and set its count to 0.
  */
  void Empty();
  /*
  Description:
    Call this method to add or replace a key in the dictionary.  The key name
    is case sensitive.  If there is currently a matching key then the value
    for that key is replaced with the new value.
  Parameters:
    key - [in] Case sensitive key name
    value - [in] Value associated with the specified key
  Returns:
    Returns the index of the item added or modified if successful or -1 on
    error.
  */
  int Add(const wchar_t* key, const wchar_t* value);
  /*
  Description:
    Call this method to delete a specific key, the key name is case sensitive.
  Parameters:
    key - [in] Case sensitive key name
  Returns:
    Returns true if the key was found in the dictionary and successfully deleted
    or false on error.
  */
  bool Delete(const wchar_t* key);
  /*
  Description:
    Call this method to delete the key value pair at the specified index.
  Parameters:
    index - [in] Zero based index
  Returns:
    Returns true if the index was valid and the key value pair was successfully
    deleted otherwise; returns false on error.
  */
  bool Delete(int index);
  /*
  Description:
    Call this method to modify the value associated with the specified key,
    the key is case sensitive.
  Parameters:
    key - [in] Case sensitive key name
    value - [in] New value
  Returns:
    Returns true if the key was found and the value was successfully set.
  */
  bool Set(const wchar_t* key, const wchar_t* value);
  /*
  Description:
    Call this method to get the 0 based index of the specified key.
  Parameters:
    key - [in] Case sensitive key name
  Returns:
    Returns a value greater than 0 and less than Count() if successful
    otherwise; returns -1.
  */
  int IndexOf(const wchar_t* key) const;
  /*
  Description:
    Array operator that allows access to key values
  Parameters:
    key - [in] Case sensitive key name
  Returns:
    Returns the value if found or NULL on error.
  */
  const wchar_t* operator[](const wchar_t* key);
  /*
  Description:
    Call this method to get a key name at a specific index.
  Parameters:
    index - [in] Zero based index
  Returns:
    Returns the key name string if the index is greater than 0 and less than Count().
  */
  const wchar_t* Key(int index) const;
  /*
  Description:
    Call this method to get a value at a specific index.
  Parameters:
    index - [in] Zero based index
  Returns:
    Returns the value string if the index is greater than 0 and less than Count().
  */
  const wchar_t* Value(int index) const;
  /*
  Description:
    Call this method to get the value for the specified key name, the key name
    string is case sensitive.
  Parameters:
    key - [in] Case sensitive key name
  Returns:
    Returns the value string if the key name is found in the dictionary otherwise;
    returns NULL.
  */
  const wchar_t* Value(const wchar_t* key) const;
  /*
  Description:
    Call this method to gain direct access to the list of keys in the dictionary.
  Returns:
    Returns a const reference to the key list.
  */
  const ON_ClassArray<ON_wString>& Keys() const;
  /*
  Description:
    Call this method to gain direct access to the list of values in the dictionary.
  Returns:
    Returns a const reference to the value list.
  */
  const ON_ClassArray<ON_wString>& Values() const;

private:
  friend class CRhSettingsHooks;

  ON_ClassArray<ON_wString>m_keys;
  ON_ClassArray<ON_wString>m_values;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoProfileContext
//
class RHINO_SDK_CLASS CRhinoProfileContext
{
public:
  enum context_location : unsigned int
  {
    cl_global_current_user = 0,
    cl_scheme_current_user,

    // "build date" is used to prevent breaking the SDK.
    // These map to the root of the Rhino registry 
    // ( "Software\McNeel\Rhinoceros\<version>\ )
    cl_root_local_machine,

    // plug-ins profile is used for storing plug-in information
    cl_plugins_current_user,
    cl_plugins_local_machine
  };
  CRhinoProfileContext(context_location ct);
  virtual ~CRhinoProfileContext();

  //
  // This method must be overridden to provide a CRhinoProfileContextIterator which is
  // responsible for iterating section names and values associated with this profile
  // context.  The overridden method need to derive a class from CRhinoProfileContextIterator,
  // and return a new pointer to the derived class.  It is the responsibility of the calling
  // function to delete the returned pointer.
  //
  virtual CRhinoProfileContextIterator* GetNewIterator(const wchar_t* section) = 0;

  virtual bool IsSynchronizing() const;
  /////////////////////////////////////////////////////////////////////////////
  // The SaveProfile*() functions write to strings to the desired output location.
  // You must derive a class from CRhinoProfileContext and provide this method.
  // The LoadProfile*() functions return TRUE if the requested entry is found
  // and is valid.
  //
  // NOTE:  Never apply localization to the lpszSection and lpszEntry
  //        parameters.  The lpszSection and lpszEntry parameters are
  //        one of the few situations in the Rhino core
  //        where we must use const wchar_t* types instead of wchar_t*.
  //
  //        All lpszSection and lpszEntry parameters in core Rhino used
  //        symbols from the ASCII character set.  Plug-in developers 
  //        who want to avoid localization and UNICODE vs MBCS
  //        issues are encouraged to adopt the same ASCII approach.
  //
  //////////
  // All the SaveProfile* functions eventually call one of 
  // these SaveProfileString() members.  You MUST override the first two methods.
  // When you override the SaveProfileString(...) above the compiler can not find the other
  // overloaded methods so at a minimum you must override them and call the base class
  virtual bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const char*) const = 0;
  virtual bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const wchar_t*) const = 0;
  // The default implementation of these do nothing.  CRhinoRegProfileContext is the only class that current
  // implements these as registry multi-strings.
  virtual bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_ClassArray<ON_String>&) const;
  virtual bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_ClassArray<ON_wString>&) const;

  // All the LoadProfile* functions eventually call 
  // this LoadProfileString(...,CString&) member.  You MUST override the following method.
  // When you override the SaveProfileString(...) above the compiler can not find the other
  // overloaded methods so at a minimum you must override them and call the base class
  virtual bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_wString&) const = 0;
  virtual bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_wString& value, const wchar_t* defaultValue) const;

  // The following version of LoadProfileString() all call the above version and convert the CString
  virtual bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, char*, int) const;
  virtual bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, wchar_t*, int) const;
  virtual bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_String&) const;
  virtual bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_String& value, const char* defaultValue) const;
  // The default implementation does nothing, CRhinoRegProfileContext is the only class that current
  // implements these.
  virtual bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_String>&) const;
  virtual bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_String>& value, const ON_ClassArray<ON_String>& defaultValue) const;
  virtual bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_wString>& value) const;
  virtual bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_wString>& value, const ON_ClassArray<ON_wString>& defaultValue) const;

  virtual bool SaveProfileStringDictioary(const wchar_t* lpszSection, const wchar_t* lpszEntry, const CRhinoStringDictionary& value) const;
  virtual bool LoadProfileStringDictionary(const wchar_t* lpszSection, const wchar_t* lpszEntry, CRhinoStringDictionary& value) const;
  virtual bool LoadProfileStringDictionary(const wchar_t* lpszSection, const wchar_t* lpszEntry, CRhinoStringDictionary& value, const CRhinoStringDictionary& defaultValue) const;

  virtual bool SaveProfileUuid(const wchar_t* lpszSection, const wchar_t* lpszEntry, const UUID& uuid) const;
  virtual bool LoadProfileUuid(const wchar_t* lpszSection, const wchar_t* lpszEntry, UUID*) const;
  virtual bool LoadProfileUuid(const wchar_t* lpszSection, const wchar_t* lpszEntry, UUID* value, const UUID& defaultValue) const;

  virtual void SaveProfileColor(const wchar_t* lpszSection, const wchar_t* lpszEntry, COLORREF) const;
  virtual bool LoadProfileColor(const wchar_t* lpszSection, const wchar_t* lpszEntry, COLORREF*) const;
  virtual bool LoadProfileColor(const wchar_t* lpszSection, const wchar_t* lpszEntry, COLORREF* value, COLORREF defaultValue) const;

#if defined (ON_RUNTIME_WIN)
  // ON OS X, COLORREF and ON_Color are the same type, so this is a duplicate declaration
  virtual bool LoadProfileColor(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Color*) const;
  virtual bool LoadProfileColor(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Color* value, const ON_Color& defaultValue) const;
#endif

  //The LoadProfileColor/SaveProfileColor functions ignore alpha. If you want to save a
  //color with alpha information, use the following RGBA functions
  virtual void SaveProfileRGBA(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_Color&) const;
  virtual bool LoadProfileRGBA(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Color&) const;
  virtual bool LoadProfileRGBA(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Color& value, const ON_Color& defaultValue) const;

  virtual void SaveProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, bool) const;
  virtual bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, bool*) const;
  virtual bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, bool* value, bool defaultValue) const;

  virtual void SaveProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, BOOL32) const;
  virtual bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, BOOL32*) const;
  virtual bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, BOOL32* value, BOOL32 defaultValue) const;


#if defined (ON_RUNTIME_APPLE) || defined(ON_RUNTIME_LINUX)
  virtual void SaveProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, int) const;
  virtual bool LoadProfileBool(const wchar_t* lpszSection, const wchar_t* lpszEntry, int*) const;
#endif
  virtual void SaveProfileInt(const wchar_t* lpszSection, const wchar_t* lpszEntry, int) const;
  virtual bool LoadProfileInt(const wchar_t* lpszSection, const wchar_t* lpszEntry, int*) const;
  virtual bool LoadProfileInt(const wchar_t* lpszSection, const wchar_t* lpszEntry, int* value, int defaultValue) const;

#if defined (ON_RUNTIME_APPLE)
  virtual bool LoadProfileInt(const wchar_t* lpszSection, const wchar_t* lpszEntry, BOOL32*) const;
#endif

  virtual void SaveProfileDouble(const wchar_t* lpszSection, const wchar_t* lpszEntry, double) const;
  virtual bool LoadProfileDouble(const wchar_t* lpszSection, const wchar_t* lpszEntry, double*) const;
  virtual bool LoadProfileDouble(const wchar_t* lpszSection, const wchar_t* lpszEntry, double* value, double defaultValue) const;

  virtual void SaveProfileRect(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_4iRect&) const;
  virtual void SaveProfileRect(const wchar_t* lpszSection, const wchar_t* lpszEntry, const RECT&) const;
  virtual bool LoadProfileRect(const wchar_t* lpszSection, const wchar_t* lpszEntry, RECT*) const;
  virtual bool LoadProfileRect(const wchar_t* lpszSection, const wchar_t* lpszEntry, RECT* value, const RECT& defaultValue) const;

  virtual void SaveProfilePoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_2iPoint&) const;
  virtual void SaveProfilePoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, const POINT&) const;
  virtual bool LoadProfilePoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, POINT*) const;
  virtual bool LoadProfilePoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, POINT* value, const POINT& defaultValue) const;

  virtual void SaveProfile3dPoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_3dPoint&) const;
  virtual bool LoadProfile3dPoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_3dPoint*) const;
  virtual bool LoadProfile3dPoint(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_3dPoint* value, const ON_3dPoint& defaultValue) const;

  virtual void SaveProfileXform(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_Xform&) const;
  virtual bool LoadProfileXform(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Xform*) const;
  virtual bool LoadProfileXform(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_Xform* value, const ON_Xform& defaultValue) const;

  virtual void SaveProfile3dVector(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_3dVector&) const;
  virtual bool LoadProfile3dVector(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_3dVector*) const;
  virtual bool LoadProfile3dVector(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_3dVector* value, const ON_3dVector& defaultValue) const;

  // Used to save export UDO, RAW, KML, GTS, CD, 3DS plug-in mesh parameters in legacy format.
  // Other applications should use SaveProfileMeshParameters(). 
  void SaveProfileLegacyPlugInMeshParameters(
    const wchar_t* lpszSection,
    const wchar_t* plugin_prefix,
    const ON_MeshParameters& mp,
    int mesh_ui_style
  ) const;

  // Used to read export UDO, RAW, KML, GTS, CD, 3DS plug-in mesh parameters in legacy format.
  // Other applications should use LoadProfileMeshParameters(). 
  bool LoadProfileLegacyPlugMeshParameters(
    const wchar_t* lpszSection,
    const wchar_t* plugin_prefix,
    ON_MeshParameters* mp,
    int* mesh_ui_style
  ) const;


  virtual void SaveProfileMeshParameters(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_MeshParameters&) const;
  virtual bool LoadProfileMeshParameters(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_MeshParameters*) const;
  virtual bool LoadProfileMeshParameters(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_MeshParameters* value, const ON_MeshParameters& defaultValue) const;

  // The default implementation of the *ProfileBinary(...) methods simply return false, CRhinoRegProfileContext
  // is currently the only class that implements these
  virtual bool SaveProfileBinary(const wchar_t* lpszSection, const wchar_t* lpszEntry, const void* pValue, ULONG nBytes) const;
  virtual bool LoadProfileBinary(const wchar_t* lpszSection, const wchar_t* lpszEntry, void* pValue, ULONG& nBytesOut) const;

  context_location ProfileContextLocation() const;

private:
  context_location m_context_location;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CGetRhinoProfileContextIterator
//
// This is a simple utility class for getting and deleting a CRhinoProfileContextIterator
// associated with a CRhinoProfileContext;
//
class RHINO_SDK_CLASS CGetRhinoProfileContextIterator
{
public:
  CGetRhinoProfileContextIterator(const wchar_t* section, CRhinoProfileContext& pc);
  virtual ~CGetRhinoProfileContextIterator();

  CRhinoProfileContextIterator* Iterator() const;

protected:
  CRhinoProfileContextIterator* m_pIterator;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoRegProfileContext
//
#if defined(_MSC_VER)
#pragma warning( push )
// C4263: 'function' : member function does not override any base class virtual member function
// C4264: 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
// C4266: 'function' : no override available for virtual member function from base 'type'; function is hidden
#pragma warning( disable : 4263 4264 4266 )
#endif
class RHINO_SDK_CLASS CRhinoRegProfileContext : public CRhinoProfileContext
{
public:
  CRhinoRegProfileContext(CRhinoProfileContext::context_location ct);
  virtual ~CRhinoRegProfileContext();

  //-----------------------------------------------------------------------------------------------
  //
  // Required CRhinoProfileContext overrides
  //
  CRhinoProfileContextIterator* GetNewIterator(const wchar_t* section) override;
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const char*) const override;
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const wchar_t*) const override;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_wString&) const override;
  // When you override the LoadProfileString(...) above the compiler can not find the other
  // overloaded methods so you must override them and call the base class
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, char*, int) const override;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, wchar_t*, int) const override;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_String&) const override;

  // When you override the SaveProfileString(...) above the compiler can not find the other
  // overloaded methods so you must override them, Reg overrides them anyhow to use the registry
  // multi-string writing stuff
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_ClassArray<ON_String>&) const override;
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_ClassArray<ON_wString>&) const override;
  //-----------------------------------------------------------------------------------------------
  //
  // Optional CRhinoProfileContext overrides
  //
  bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_String>&) const override;
  bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_wString>&) const override;
  bool SaveProfileBinary(const wchar_t* lpszSection, const wchar_t* lpszEntry, const void* pValue, ULONG nBytes) const override;
  bool LoadProfileBinary(const wchar_t* lpszSection, const wchar_t* lpszEntry, void* pValue, ULONG& nBytesOut) const override;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// class CRhinoIniProfileContext
//
// Note: You must call SetIniFileName() with a valid INI file name or all other methods will fail.
//
#if defined(_MSC_VER)
#pragma warning( push )
// C4263: 'function' : member function does not override any base class virtual member function
// C4264: 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
// C4266: 'function' : no override available for virtual member function from base 'type'; function is hidden
#pragma warning( disable : 4263 4264 4266 )
#endif
class RHINO_SDK_CLASS CRhinoIniProfileContext : public CRhinoProfileContext
{
public:
  CRhinoIniProfileContext(CRhinoProfileContext::context_location ct);
  virtual ~CRhinoIniProfileContext();

  //-----------------------------------------------------------------------------------------------
  //
  // Required CRhinoProfileContext overrides
  //
  CRhinoProfileContextIterator* GetNewIterator(const wchar_t* section) override;
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const char*) const override;
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const wchar_t*) const override;
  // When you override the SaveProfileString(...) above the compiler can not find the other
  // overloaded methods so you must override them, INI overrides them anyhow and uses a number
  // decoration scheme to save lists of strings
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_ClassArray<ON_String>&) const override;
  bool SaveProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_ClassArray<ON_wString>&) const override;

  // When you override the LoadProfileString(...) above the compiler can not find the other
  // overloaded methods so you must override them and call the base class
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, char*, int) const override;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, wchar_t*, int) const override;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_String&) const override;
  bool LoadProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_wString&) const override;
  //-----------------------------------------------------------------------------------------------
  //
  // Optional CRhinoProfileContext overrides
  //
  bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_String>&) const override;
  bool LoadProfileMultiString(const wchar_t* lpszSection, const wchar_t* lpszEntry, ON_ClassArray<ON_wString>&) const override;
  //-----------------------------------------------------------------------------------------------
  //
  // Local public methods
  //
  const wchar_t* IniFileName() const;
  void SetIniFileName(const wchar_t*);
protected:
  //-----------------------------------------------------------------------------------------------
  //
  // Local protected methods
  //
  virtual bool IniWriteProfileString(const wchar_t* lpszSection, const wchar_t* lpszEntry, const ON_wString& sValue, CRhinoProfileContext::context_location nProfile) const;
protected:
  ON_wString m_wFileName;
#if defined(ON_RUNTIME_APPLE) || defined(ON_RUNTIME_LINUX)
  class CRhIniProfileContextPrivate* m_private;
#endif
};
#if defined(_MSC_VER)
#pragma warning( pop )
#endif
