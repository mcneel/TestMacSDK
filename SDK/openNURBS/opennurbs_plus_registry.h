//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_PLUS) || !defined(OPENNURBS_PLUS_INC_) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_REGISTRY_INC_)
#define OPENNURBS_PLUS_REGISTRY_INC_


class ON_RegKey;

/*
Description:
  ON_RegKey provides simplified access to the Windows Registry.
  All functions return true to indicate success, false for failure.
  If a function returns false, check the ErrorCode property to see extended 
  error codes returned by the underlying Windows API functions.

  Performance Note: The underlying Windows HKEY is not cached; it is opened 
  and closed for each operation.
  No testing has been done to determine if this causes much overhead for 
  lots of reads and writes.
  The reason for this is to simplify the underlying implementation as copy 
  constructors and operator = do not need to 
  check for open keys, close them when necessary, or worry about other 
  copies closing the handle.
*/
class ON_CLASS ON_RegKey 
{
public:
  /*
  Description:
    Default constructor.
  Parameters:
    None
  Returns:
    None
  */
  ON_RegKey();

  ~ON_RegKey();

  /*
  Description:
    Copy constructor. After initialization, the assignment operator is called.
  Parameters:
    src - [in] the ON_RegKey to copy.
  See Also:
    ON_RegKey::operator=(const ON_RegKey& src);
  */
  ON_RegKey(const ON_RegKey& src);

  /*
  Description:
    Assignment operator. Properties from src are copied to this ON_RegKey, but the underlying Windows HKEY is not.
    The root hive, path to the key, and whether the key should be created or not is copied.

    It is safe to make one or more copies of an ON_RegKey; each will open a unique connection to the Windows
    Registry to prevent a copy from inadvertently closing the source HKEY.
  Parameters:
    src - [in] the ON_RegKey to copy.
  See Also:
    ON_RegKey(const ON_RegKey& src);
  */
  ON_RegKey& operator =(const ON_RegKey& src);

  /*
  Description:
    OpenRead opens the specified registry key for reading. It optionally creates the key if it does not exist, then opens it for reading.
  Parameters:
    KeyName - [in] the full path to the registry key, including the hive. The hive can be the full name or 
      abbreviation: HKEY_LOCAL_MACHINE or HKLM, HKEY_CURRENT_USER or HKCU, HKEY_CLASSES_ROOT or HKCR, 
      HKEY_CURRENT_CONFIG or HKCC, HKEY_USERS or HKU.
      See examples for details.
    Create - [in, optional] if true, KeyName will be created if it doesn't already exist. Default is false.
  Returns:
    true when SubKey could be created or opened.
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx or RegCreateKeyEx
  Examples:
    // Open HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct if it exists
    ON_RegKey reg;
    reg.Open(L"HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct");

    // Create HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct
    ON_RegKey reg(L"HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct", true);

    // Open HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct
    ON_RegKey reg(L"HKLM\\Software\\MyCompany\\MyProduct");

    // Open HKEY_CURRENT_USER\\Software\\MyCompany\\MyProduct
    ON_RegKey reg(L"HKCU\\Software\\MyCompany\\MyProduct");
  See Also:
    ON_RegKey::OpenWrite
    Windows Platform SDK documentation for RegOpenKeyEx
    Windows Platform SDK documentation for RegCreateKeyEx
  */
  bool OpenRead(const wchar_t* KeyName);

  /*
  Description:
    OpenSubKeyRead opens the specified registry key for reading.
  Parameters:
    ParentKey - [in] reference to parent key under SubKey should be opened.
    SubKey - [in] name of the key to create. If SubKey contains "\" 
    characters, nested sub keys are created and the bottom key is returned.
  Returns:
    true when SubKey could be created or opened.
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx
  See Also:
    ON_RegKey::OpenSubKeyWrite
    ON_RegKey::CreateKey
    ON_RegKey::DeleteSubKey
    Windows Platform SDK documentation for RegOpenKeyEx
  */
  bool OpenRead(const ON_RegKey &Parent, const wchar_t *SubKey);

  /*
  Description:
    OpenWrite opens the specified registry key for reading and writing. It optionally creates the key if it does not exist, then opens it for reading and writing.
  Parameters:
    KeyName - [in] the full path to the registry key, including the hive. The hive can be the full name or 
      abbreviation: HKEY_LOCAL_MACHINE or HKLM, HKEY_CURRENT_USER or HKCU, HKEY_CLASSES_ROOT or HKCR, 
      HKEY_CURRENT_CONFIG or HKCC, HKEY_USERS or HKU.
      See examples for details.
    Create - [in, optional] if true, KeyName will be created if it doesn't already exist. Default is false.
  Returns:
    true when SubKey could be created or opened.
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx or RegCreateKeyEx
  Examples:
    // Open HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct if it exists
    ON_RegKey reg;
    reg.Open(L"HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct");

    // Create HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct
    ON_RegKey reg(L"HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct", true);

    // Open HKEY_LOCAL_MACHINE\\Software\\MyCompany\\MyProduct
    ON_RegKey reg(L"HKLM\\Software\\MyCompany\\MyProduct");

    // Open HKEY_CURRENT_USER\\Software\\MyCompany\\MyProduct
    ON_RegKey reg(L"HKCU\\Software\\MyCompany\\MyProduct");
  See Also:
    ON_RegKey::OpenRead
    Windows Platform SDK documentation for RegOpenKeyEx
    Windows Platform SDK documentation for RegCreateKeyEx
  */
  bool OpenWrite(const wchar_t* KeyName);

  /*
  Description:
    OpenSubKeyWrite opens the specified registry key for writing.
  Parameters:
    Parent - [in] reference to parent key under SubKey should be opened.
    SubKey - [in] name of the key to create. If SubKey contains "\" 
    characters, nested sub keys are created and the bottom key is returned.
  Returns:
    true when SubKey could be created or opened.
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx
  See Also:
    ON_RegKey::OpenSubKeyRead
    ON_RegKey::CreateKey
    ON_RegKey::DeleteSubKey
    Windows Platform SDK documentation for RegOpenKeyEx
  */
  bool OpenWrite(const ON_RegKey &Parent, const wchar_t *SubKey);

  /*
  Description:
    CreateKey creates the specified registry key and opens it for reading and writing.
    If the key already exists, the function opens it.
  Parameters:
    KeyPath - [in] Path to the key to create. If SubKey contains "\" 
    characters, nested sub keys are created and the bottom key is returned.
  Returns:
    true when KeyPath could be created or opened.
    false otherwise
    ErrorCode contains the return value of the underlying call to RegCreateKeyEx
  Remarks:
  See Also:
    ON_RegKey::OpenSubKeyRead
    ON_RegKey::OpenSubKeyWrite
    ON_RegKey::DeleteSubKey
    Windows Platform SDK documentation for RegCreateKeyEx
  */
  bool CreateKey(const wchar_t* KeyPath);

  /*
  Description:
    CreateKey creates the specified registry key and opens it for reading and writing.
    If the key already exists, the function opens it.
  Parameters:
    Parent - [in] reference to parent key under SubKey should be created.
    SubKey - [in] name of the key to create. If SubKey contains "\" 
    characters, nested sub keys are created and the bottom key is returned.
  Returns:
    true when SubKey could be created or opened.
    false otherwise
    ErrorCode contains the return value of the underlying call to RegCreateKeyEx
  Remarks:
  See Also:
    ON_RegKey::OpenSubKeyRead
    ON_RegKey::OpenSubKeyWrite
    ON_RegKey::DeleteSubKey
    Windows Platform SDK documentation for RegCreateKeyEx
  */
  bool CreateKey(const ON_RegKey &Parent, const wchar_t *SubKey);

  /*
  Description:
    QueryValue retrieves the specified string value from the registry. 
    If the value is not stored as a REG_SZ value, QueryValue fails.
  Parameters:
    ValueName - [in] the name of the value to query. Can be nullptr or 
    empty. If ValueName is nullptr or empty, the data is retrieved from 
    the default value for this key.
    data - [in, out] a reference to the ON_wString that will contain 
    the string value after a successful query.
  Returns:
    true when QueryValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegQueryValueEx
  See Also:
    ON_RegKey::QueryValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegQueryValueEx
  */
  bool QueryValue(const wchar_t *ValueName, ON_wString &data);

  /*
  Description:
    QueryValue retrieves the specified DWORD value from the registry. 
    If the value is not stored as a REG_DWORD value, QueryValue fails.
  Parameters:
    ValueName - [in] the name of the value to query. Can be nullptr or empty. 
    If ValueName is nullptr or empty, the data is retrieved from the default 
    value for this key.
    data - [in, out] a reference to the DWORD that will contain the string 
    value after a successful query.
  Returns:
    true when QueryValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegQueryValueEx
  See Also:
    ON_RegKey::QueryValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegQueryValueEx
  */
  bool QueryValue(const wchar_t *ValueName, DWORD &data);

  /*
  Description:
    QueryValue retrieves the specified MultiString value from the 
    registry. If the value is not stored as a REG_MULTI_SZ value, 
    QueryValue fails.
  Parameters:
    ValueName - [in] the name of the value to query. Can be nullptr or 
    empty. If ValueName is nullptr or empty, the data is retrieved from 
    the default value for this key.
    data - [in, out] a reference to the ON_ClassArray<ON_wString> that 
    will contain the string value after a successful query.
  Returns:
    true when QueryValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegQueryValueEx
  See Also:
    ON_RegKey::QueryValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegQueryValueEx
  */
  bool QueryValue(const wchar_t *ValueName, ON_ClassArray<ON_wString> &data);

  bool QueryValue(const wchar_t *ValueName, ON_SimpleArray<unsigned char> &data);

  /*
  Description:
    SetValue saves the specified string value as a REG_SZ value in the registry.
  Parameters:
    ValueName - [in] the name of the value to save.
    data - [in] the string to save as data in the value named ValueName.
  Returns:
    true when SetValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegSetValueEx
  See Also:
    ON_RegKey::SetValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegSetValueEx
  */
  bool SetValue(const wchar_t *ValueName, const wchar_t* data);

  /*
  Description:
    SetValue saves the specified DWORD value as a REG_DWORD value in the registry.
  Parameters:
    ValueName - [in] the name of the value to save. Can be nullptr or 
    empty. If ValueName is nullptr or empty, the data is saved in the 
    default value for this key.
    data - [in] the DWORD to save as data in the value named ValueName.
  Returns:
    true when SetValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegSetValueEx
  See Also:
    ON_RegKey::SetValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegSetValueEx
  */
  bool SetValue(const wchar_t *ValueName, DWORD data);

  /*
  Description:
    SetValue saves the specified array of ON_wStrings as a REG_MULTI_SZ in the registry.
  Parameters:
    ValueName - [in] the name of the value to save. Can be nullptr or 
    empty. If ValueName is nullptr or empty, the data is saved in the 
    default value for this key.
    data - [in] the ON_wString array to save as data in the value 
    named ValueName. If data is nullptr, or if any of the ON_wString 
    objects in the array are empty, SetValue fails and ErrorCode 
    is set to ERROR_INVALID_DATA
    count - [in] the number of items in the data array. If count == 0, 
    SetValue fails and ErrorCode is set to ERROR_INVALID_DATA.
  Returns:
    true when SetValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegSetValueEx
  See Also:
    ON_RegKey::SetValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegSetValueEx
  */
  bool SetValue(const wchar_t *ValueName, const ON_wString* data, int count);

  /*
  Description:
    SetValue saves the specified array of ON_wStrings as a REG_MULTI_SZ 
    in the registry.
  Parameters:
    ValueName - [in] the name of the value to save. Can be nullptr or 
    empty. If ValueName is nullptr or empty, the data is saved in the 
    default value for this key.
    data - [in] the ON_wString array to save as data in the value 
    named ValueName. If data is empty, or if any of the ON_wString 
    objects in the array are empty, SetValue fails and ErrorCode is 
    set to ERROR_INVALID_DATA
  Returns:
    true when SetValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegSetValueEx
  See Also:
    ON_RegKey::SetValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegSetValueEx
  */
  bool SetValue(const wchar_t *ValueName, const ON_ClassArray<ON_wString> &data);

  bool SetValue(const wchar_t *ValueName, const unsigned char* pData, const int cData);
  bool SetValue(const wchar_t *ValueName, const ON_SimpleArray<unsigned char> &data);


  /*
  Description:
    GetSubKeys returns an array of ON_RegKeys. There is one ON_RegKey 
    for each key that is an immediate child of this key.
  Parameters:
    SubKeys - [in, out] reference to an ON_RegKeyArray that will contain the list of sub keys. 
    SubKeys is emptied prior to being populated with new keys.
  Returns:
    true when GetSubKeys succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegSetValueEx
  See Also:
    ON_RegKey::QueryValue
    ON_RegKey::GetValueNames
    ON_RegKey::SetValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegEnumKeyEx
  */
  //bool GetSubKeys(ON_RegKeyArray &SubKeys);
  bool GetKeyNames(ON_ClassArray<ON_wString> &SubKeyNames);

    /*
  Description:
    GetValueNames returns an ON_ClassArray<ON_wString> containing the names of each value in the key. This array does not include values in subkeys.
  Parameters:
    ValueNames - [in] reference to an ON_ClassArray<ON_wString> that will contain the list of value names. 
    ValueNames is emptied prior to being populated with new keys.
  Returns:
    true when GetValueNames succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegEnumValue
  See Also:
    ON_RegKey::QueryValue
    ON_RegKey::SetValue
    ON_RegKey::DeleteValue
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegEnumValue
  */
  bool GetValueNames(ON_ClassArray<ON_wString> &ValueNames);

  /*
  Description:
    DeleteValue deletes the specified value from the registry.
  Parameters:
    ValueName - [in] the value to delete from the registry. Can be nullptr or 
    empty. If ValueName is nullptr or empty, data deleted from the default 
    value for this key.
  Returns:
    true when DeleteValue succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegSetValueEx
  See Also:
    ON_RegKey::QueryValue
    ON_RegKey::SetValue
    Windows Platform SDK documentation for RegDeleteValue
  */
  bool DeleteValue(const wchar_t *ValueName);

  /*
  Description:
    DeleteSubKey deletes the specified key and all values it contains from the 
    registry. The subkey to be deleted must not have subkeys. To delete a key 
    and all its subkeys, you need to enumerate recursively the subkeys and delete 
    them individually. To delete keys recursively, use the DeleteSubKeyRecursive 
    function.
  Parameters:
    KeyName - [in] the name of the key to delete. Must not be nullptr or empty.
  Returns:
    true when DeleteSubKey succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegDeleteKey
  See Also:
    ON_RegKey::DeleteSubKeyRecursive
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for RegDeleteKey
  */
  bool DeleteSubKey(const wchar_t *KeyName);

  /*
  Description:
    DeleteSubKeyRecursive deletes the specified key, all values it contains, and 
    all sub keys from the registry.
  Parameters:
    KeyName - [in] the name of the key to delete. Must not be nullptr or empty.
  Returns:
    true when DeleteSubKeyRecursive succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to SHDeleteKey
  See Also:
    ON_RegKey::DeleteSubKey
    ON_RegKey::CreateKey
    Windows Platform SDK documentation for SHDeleteKey
  */
  bool DeleteSubKeyRecursive(const wchar_t *KeyName);

  /*
  Description:
    CopyKey copies all values from source to destination. Existing values in destination are not removed.
    If CopyKey fails in the middle of the copy operation, any successfully copied keys will remain.
  Parameters:
    source - [in] the key that contains the values to copy.
    destination - [in] the key into which values will be copied.
  Returns:
    ERROR_SUCCESS on success
    Any other Windows error code on failure. Failures can come from opening source for read, opening destination for write, reading source values, or writing destination values.
  See Also:
    ON_RegKey::CopyKeyRecursive
    ON_RegKey::CopyValue
    ON_RegKey::DeleteSubKey
    ON_RegKey::CreateKey
  */
  static LONG CopyKey(ON_RegKey &source, ON_RegKey &destination);
  static LONG CopyKeyRecursive(ON_RegKey &source, ON_RegKey &destination);
  static LONG CopyValue(ON_RegKey &source, ON_RegKey &destination, const wchar_t *ValueName);

  /*
  Description:
    ErrorCode contains the result from underlying calls to Windows registry functions.
    When a call succeeds, all ON_RegKey functions return true, and ErrorCode == ERROR_SUCCESS.
  See Also:
    Windows Platform SDK documentation for Registry Functions.
  */
  LONG m_ErrorCode;

  /*
  Description:
    Returns an ON_wString containing the name of this key.
  Example:
    If the current key is "HKEY_LOCAL_MACHINE\\Software\\McNeel"
    then Name() return "McNeel"
  */
  ON_wString Name() const;

  /*
  Description:
    Returns an ON_wString containing the name of the registry hive that is the root of this key.
  Example:
    If the current key is "HKEY_LOCAL_MACHINE\\Software\\McNeel"
    then Hive() return "HKEY_LOCAL_MACHINE"
  */
  const wchar_t* Hive() const;

  /*
  Description:
    Returns an ON_wString containing the full path of this registry key.
  Example:
    If the current key is "HKEY_LOCAL_MACHINE\\Software\\McNeel"
    then Path() return "HKEY_LOCAL_MACHINE\\Software\\McNeel"
  */
  ON_wString Path() const;

  /*
  Description:
    Returns an ON_wString containing the full path of this registry key.
  Example:
    If the current key is "HKEY_LOCAL_MACHINE\\Software\\McNeel"
    then Parent() return "HKEY_LOCAL_MACHINE\\Software"
  */
  ON_wString Parent() const;


  /*
  Description:
    CloseKey closes the specified key.
  Parameters:
    key - [in] the HKEY to be closed.
  See Also:
    ON_RegKey::OpenKeyRead
    ON_RegKey::OpenKeyWrite
    Windows Platform SDK documentation for RegOpenKeyEx
  */
  void CloseKey();

// Private Members:

protected:
  // Shared initialization used by constructors and operator=
  void Init();

private:
  /*
  Description:
    ParseKeyPath is a helper function that sets m_root and m_key_path based on KeyPath.
    KeyPath accepts both full and abbreviated hive names:
    HKEY_LOCAL_MACHINE or HKLM, HKEY_CURRENT_USER or HKCU, HKEY_CLASSES_ROOT or HKCR, 
    HKEY_CURRENT_CONFIG or HKCC, HKEY_USERS or HKU.
  Returns:
    true when ParseKeyPath succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx
  Example:
    if KeyPath is "HKLM\Software\McNeel\Rhinoceros" then
    m_key is set to HKEY_LOCAL_MACHINE and 
    m_key_path is set to "Software\McNeel\Rhinoceros".
  */
  bool ParseKeyPath(const wchar_t* KeyPath);

  /*
  Description:
    OpenKeyRead opens the key indicated in the hive specified by m_root, with the path 
    specified by m_key_path. The resulting HKEY has KEY_READ privileges, but cannot write 
    values or create sub-keys. For write access, use ON_RegKey::OpenKeyWrite.
  Returns:
    true when OpenKeyRead succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx
  See Also:
    ON_RegKey::OpenKeyWrite
    ON_RegKey::CloseKey
    Windows Platform SDK documentation for RegOpenKeyEx
  */
  bool OpenKeyRead();

  /*
  Description:
    OpenKeyRead opens the key indicated in the hive specified by m_root, with the path 
    specified by m_key_path. The resulting HKEY has KEY_WRITE privileges, meaning it 
    can read, write, create, and delete keys and values. For read-only access, use 
    ON_RegKey::OpenKeyRead.
  Returns:
    true when OpenKeyWrite succeeds
    false otherwise
    ErrorCode contains the return value of the underlying call to RegOpenKeyEx
  See Also:
    ON_RegKey::OpenKeyWrite
    ON_RegKey::CloseKey
    Windows Platform SDK documentation for RegOpenKeyEx
  */
  bool OpenKeyWrite();

  /*
  Description:
    The full path, relative to the current hive (see m_root), of this registry key. This string 
    is one of two pieces of persistent information about this key that is saved between calls 
    to the member functions of ON_RegKey. The other is m_root.
  See Also:
    m_root
  */
  ON_wString m_key_path;

  /*
  Description:
    The root HKEY for this ON_RegKey object. This should be one of the Windows registry hives 
    (HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER, etc...). This is one of two pieces of information 
    saved about this registry key between calls to ON_RegKey member functions. The other is m_root.
  See Also:
    m_key_path
  */
  HKEY m_root;

  /*
  Description:
    The handle to the Windows HKEY used by all calls to Windows Platform Registry functions.
    DO NOT MAKE THIS PUBLIC. Having external users close this key while this ON_RegKey has it open
    will cause difficult to find failures.
  See Also:
    m_key_path
  */
  HKEY m_key;

  /*
  Description:
    enum to define the current state of this registry key.
  See Also:
    m_state
  */
  enum RegOpenState
  {
    Reg_Closed = 0,
    Reg_OpenRead,
    Reg_OpenWrite,
  };

  /*
  Description:
    if m_create is true, the key specified by m_key and m_key_path will be created
    when it is opened. If the key doesn't exist in the registry, calls to OpenRead
    and OpenWrite will fail.
  */
  bool m_create;

  RegOpenState m_state;

#ifdef WIN64
public:
  /*
  Description:
    m_bWow64 sets behavior of all calls to RegOpenKeyEx and RegCreateKeyEx.
    if m_bWow64 is true, this ON_RegKey will access the 32-bit registry nodes
    on 64-bit machines.
  See Also:
    ON_RegKeyWow64 class
  */
  bool m_bWow64;
#endif
};

#endif
