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

//
// Stores information extracted from plug-in, or RHP, file.
//
class RHINO_SDK_CLASS CRhinoPlugInInfo
{
public:
  CRhinoPlugInInfo() = default;
  ~CRhinoPlugInInfo() = default;
  CRhinoPlugInInfo(const CRhinoPlugInInfo& src) = default;
  CRhinoPlugInInfo& operator=(const CRhinoPlugInInfo&) = default;

  CRhinoPlugInInfo(
    const wchar_t* organization,
    const wchar_t* address,
    const wchar_t* country,
    const wchar_t* phone,
    const wchar_t* email,
    const wchar_t* website,
    const wchar_t* update_url,
    unsigned int sdk_version,
    unsigned int sdk_service_release,
    const wchar_t* fax,
    const wchar_t* plugin_path
  );

  // Description:
  //   Function used to validate information extracted from a plug-in.
  // Returns:
  //   Return true if plug-in contains information required to load and register.
  bool HasRequiredFields() const;

  // Description:
  //   The organization which created this plug-in.
  const ON_wString Organization() const;

  // Description:
  //   The address of the organization which created this plug-in.
  const ON_wString Address() const;

  // Description:
  //   The country of the organization which created this plug-in.
  const ON_wString Country() const;

  // Description:
  //   The phone number of the organization which created this plug-in.
  const ON_wString Phone() const;

  // Description:
  //   The fax number of the organization which created this plug-in.
  const ON_wString Fax() const;

  // Description:
  //   The support e-mail address of the organization which created this plug-in.
  const ON_wString EMail() const;

  // Description:
  //   The web address, or URL, of the organization which created this plug-in.
  const ON_wString WebSite() const;

  // Description:
  //   The web address, or URL, that can be used to update the plug-in.
  const ON_wString UpdateURL() const;

  // Description:
  //   The full path to plug-in assembly, DLL, or RHP file.
  const ON_wString PlugInPath() const;

  // Description:
  //   The version of the SDK that the plug-in requires. The
  //   SDK version must exactly match CRhinoApp::SdkVersion()
  //   or the plug-in will not load.
  // Remarks:
  //   Use ON_VersionNumberParse() to get major, minor,
  //   year, month, day of month and branch information from
  //   this version number.
  unsigned int SDKVersion() const;

  // Description:
  //   The minimum service release version the plug-in requires. The
  //   SDK service release must be <= CRhinoApp::SdkServiceRelease()
  //   or the plug-in will not load.
  // Remarks:
  //   Use ON_VersionNumberParse() to get major, minor,
  //   year, month, day of month and branch information from
  //   this version number.
  unsigned int SDKServiceRelease() const;

  // Description:
  //   Plug-ins have an extern "C" function that returns the plug-in id. 
  //   If the plug-in has that function, then GetPlugInId() will return
  //   true and set plugin_id to the value of the id. If the plug-in does
  //   not export this function, then GetPlugInId() will return false and 
  //   set plugin_id to ON_nil_uuid.
  bool GetPlugInId(ON_UUID& plugin_id) const;

  // Description:
  //   Plug-ins have an extern "C" function that returns the plug-in's short name
  //   that is displayed in the user interface. If the plug-in has this function, 
  //   then PlugInShortName() will return that string. If the plug-in does not 
  //   export this function, then PlugInShortName() will return an empty string.
  const ON_wString PlugInShortName() const;

  // Description:
  //   Plug-ins have an extern "C" function that returns the plug-in's version 
  //   that is displayed in the user interface. If the plug-in has this function, 
  //   then PlugInVersion() will return a that string. If the plug-in is does not 
  //   export this function, then PlugInVersion() will return an empty string.
  const ON_wString PlugInVersion() const;

  // Description:
  //   Plug-ins have an extern "C" function that returns the plug-in's description 
  //   that is displayed in the user interface. If the plug-in has this function, 
  //   then PlugInDescription() will return a that string. If the plug-in is does not 
  //   export this function, then PlugInDescription() will return an empty string.
  const ON_wString PlugInDescription() const;

  // Description:
  //   Plug-ins have an extern "C" function that returns the resource id of the plug-in's 
  //   icon that is displayed in the user interface. If the plug-in has this function,
  //   then PlugInIconResourceId() will return that resource id. If the plug-in is does
  //   not export this function, then PlugInIconResourceId() will return 0.
  unsigned int PlugInIconResourceId() const;

  // Description:
  //   .NET plug-ins can have a custom assembly attribute, PlugInDescriptionAttribute.Icon, 
  //   that returns the resource name of the icon. If the plug-in has this attribute,
  //   then PlugInIconResourceName() will return that resource name. If the plug-in is does
  //   not have this attribute, then PlugInIconResourceName() will return an empty string.
  const ON_wString PlugInIconResourceName() const;

  // Description:
  //   Plug-ins can have a user interface, or RUI, file which contains custom
  //   toolbar and menus. If this is the case, then an entry in the plug-in's
  //   registry key will exist and it points to an existing RUI file that will be 
  //   loaded when Rhino starts. If a user closes the file at some point it will
  //   not get loaded unless the user runs the ToolbarReset command.
  const ON_wString RuiFile() const;

  // Description:
  //   Plug-ins can have a user interface, or RUI, file which contains custom
  //   toolbar and menus. If this is the case, then an entry in the plug-in's
  //   registry key may exist that poinst to a file will be loaded as a 
  //   macro library.
  const ON_wString RuiLibraryFile() const;

public:
  // Required fields
  ON_wString m_wOrganization;
  ON_wString m_wAddress;
  ON_wString m_wCountry;
  ON_wString m_wPhone;
  ON_wString m_wEMail;
  ON_wString m_wWebSite;
  ON_wString m_wUpdateURL;
  unsigned int m_nSDKVersion = 0;
  unsigned int m_nSDKServiceRelease = 0;
  // Optional fields
  ON_wString m_wFax;
  ON_wString m_wPlugInPath;

  void SetPlugInDescription(const wchar_t* plugin_description);
  void SetPlugInIconResourceName(const wchar_t* plugin_icon_resource_name);
  void SetPlugInId(ON_UUID plugin_id);

  void SetPlugInShortName(const wchar_t* plugin_short_name);
  void SetPlugInVersion(const wchar_t* plugin_version);

private:
  void SetPlugInIconResourceId(unsigned int plugin_icon_resource_id);
  void SetRuiFile(const wchar_t* rui_file);
  void SetRuiLibraryFile(const wchar_t* rui_library_file);

private:
  ON_UUID m_plugin_id = ON_nil_uuid;
  ON_wString m_plugin_short_name;
  ON_wString m_plugin_version;
  ON_wString m_rui_file;
  ON_wString m_rui_library_file;
  ON_wString m_plugin_description;
  unsigned int m_plugin_icon_resource_id = 0;
  ON_wString m_plugin_icon_resource_name;
  ON__UINT_PTR m_reserved = 0; // Reserved for future use.
  friend class CRhPlugInManager;
};


////////////////////////////////////////////////////////////////////////////////////////////
//
// This class is used to describe commands associated with a unloaded plug-in.  These
// commands are listed in the registry.
//
class RHINO_SDK_CLASS CRhinoPlugInCommandRecord
{
public:
  CRhinoPlugInCommandRecord() = default;
  //
  // Description:
  //   Constructor
  //
  // Parameters:
  //   lpsEnglishCommandName - [in] English command name.
  //   lpsLocalizedCommandName - [in] Localized command name
  //                                 (may be same as English if not localized)
  //   nFlags - [in] See CRhinoCommand::m_cmd_flags for a description of flags.
  //
  // See Also:
  //   CRhinoCommand::m_cmd_flags
  //
  CRhinoPlugInCommandRecord( const wchar_t* lpsEnglishCommandName, const wchar_t* lpsRegistryLocalCommandName, int nFlags);
  CRhinoPlugInCommandRecord( const CRhinoPlugInCommandRecord&);
  const CRhinoPlugInCommandRecord& operator=( const CRhinoPlugInCommandRecord&);

  //
  // Description:
  //   Return English command name.
  const wchar_t* EnglishCommandName() const;
  //
  // Description:
  //   Return localized command name.
  const wchar_t* LocalizedCommandName() const;
  //
  // Description:
  //   Return command flags.
  int Flags() const;
  //
  // Description:
  //   Returns true if a command is "transparent"; i.e., the command
  //   does not modify the contents of the model's geometry in any way.
  //   Examples of transparent commands include commands that change
  //   views and toggle snap states.  Any command that adds or deletes,
  //   a view cannot be transparent.
  bool IsTransparent() const;
  //
  // Description:
  //   Returns true if a command is "repeatable"; i.e., the command
  //   can be repeated by pressing ENTER immediately after the
  //   command finishes.
  bool IsRepeatable() const;
  //
  // Description:
  //   Returns true if a command is a system command.
  //   (Most commands are not and SDK developers can ignore this field.)
  bool IsSystem() const;
  //
  // Description:
  //   Returns true if a command is a "test" command that is
  //   not available in the commercial release.  All commands
  //   derived from CRhinoTestCommand are "test" commands.
  bool IsTestCommand() const;
  //
  // Description:
  //   Returns true if this command's actions can be
  //   undone by the Rhino "Undo" command.
  //See Also:
  //   CRhinoCommand::EnableUndo
  bool IsUndoable() const;

  //////////
  // Returns true if a command is an "alpha" command.
  bool IsAlphaCommand() const;
  //
  // Description:
  // Returns:
  //   True if the command is a script runner command like
  //   "ReadCommandFile" or the RhinoScript plug-in's "ReadScript".
  // Remarks:
  //   In the depths of the Rhino core, "ordinary" commands
  //   and "script running" commands need to be treated a little
  //   differently so that commands run in a script behave exactly
  //   as if they were being run from the command line.
  bool IsScriptRunnerCommand() const;
  //
  // Description:
  //   Set command flags
  //
  // Parameters:
  //   nFlags - [in] See CRhinoCommand::m_cmd_flags for a description of flags.
  //
  // See Also:
  //   CRhinoCommand::m_cmd_flags
  //
  void SetFlags( int nFlags);
private:
  ON_wString m_english_command_name;
  ON_wString m_local_command_name;
  int m_nFlags = 0;
};


//
// This class is used to manage loaded and unloaded plug-ins.
//
class RHINO_SDK_CLASS CRhinoPlugInRecord
{
public:
  CRhinoPlugInRecord();
  CRhinoPlugInRecord( const CRhinoPlugInRecord&);
  const CRhinoPlugInRecord& operator=( const CRhinoPlugInRecord&);
  ~CRhinoPlugInRecord();

  // Description:
  //   If this plug-in has been loaded this will be a pointer to the
  //   one and only CRhinoPlugIn... derived application object.
  // Returns:
  //   Pointer to plug-in application object or NULL if the plug-in
  //   has not yet been loaded.
  // See Also:
  //   CRhinoPlugIn
  CRhinoPlugIn* PlugIn() const;

  // Description:
  //   This method is used to get information about the plug-in.
  // Returns:
  //   Reference to a CRhinoPlugInInfo item which describes this plug-in.
  // See Also:
  //   CRhinoPlugInInfo
  const CRhinoPlugInInfo& PlugInInfo() const;

  // Description:
  //   Instance handle for loaded Rhino RHP plug-in file.
  // Returns:
  //   Instance handle for loaded Rhino RHP plug-in file or NULL if
  //   not loaded.
  HINSTANCE Instance() const;

  // Description:
  //   Complete registry path used to describe where the plug-in load
  //   information may be found.
  // Returns:
  //   Full path to registry entry used to load the plug-in.
  const wchar_t* RegistryPath() const;
  
#if defined (ON_RUNTIME_APPLE)
  //
  // NSBundle that contains plug-in
  //
  id Bundle() const;
#endif
  
  // Description:
  //   If the plug-in is loaded this will return PlugIn()->LocalPlugInName(),
  //   otherwise it returns the plug-in name found in the registry.
  // Returns:
  //   Name used to describe this plug-in.
  // See Also:
  //   CRhinoPlugIn::PlugInName()
  const wchar_t* Name() const;

  // Added by John Morse 17 February 2021
  // Description:
  //   If the plug-in is loaded this will return PlugIn()->PlugInName(),
  //   otherwise it returns the English plug-in name found in the registry.
  // Returns:
  //   Name used to describe this plug-in.
  // See Also:
  //   CRhinoPlugIn::PlugInName()
  const wchar_t* EnglishName() const;

  // Description:
  //   If the plug-in is loaded this will return PlugIn()->PlugInDescription(),
  //   otherwise it returns the plug-in description found in CRhinoPlugInInfo.
  // Returns:
  //   Description of this plug-in.
  // See Also:
  //   CRhinoPlugIn::PlugInDescription()
  const wchar_t* Description() const;

  // Description:
  //   If the plug-in is loaded this will return PlugIn()->PlugInIcon(),
  //   otherwise it create a new icon based on the resource id found in CRhinoPlugInInfo.
  // Returns:
  //   The plug-in's icon.
  // See Also:
  //   CRhinoPlugIn::PlugInIcon()
  HICON Icon(const ON_2iSize& size);

  // Description:
  //   Full path to the plug-in file to load or the loaded file name.
  // Returns:
  //   Full path to plug-in file.
  const wchar_t* FileName() const;

  // Description:
  //   If the plug-in is loaded this will return the PlugIn()->PlugInLoadTime()
  //   otherwise it returns the plug-in load time value found in the registry.
  // Returns:
  //   load_plug-in_when_needed = Plug-in will not be loaded when Rhino starts.  Plug-in
  //                             will be loaded when a plug-in defined command is
  //                             run or when a user selects a plug-in defined file
  //                             import/export type.
  //   load_plug-in_at_startup  = Plug-in is loaded when Rhino is loaded and initialized.
  // See Also:
  //   CRhinoPlugIn::plugin_load_time
  //   CRhinoPlugIn::PlugInLoadTime()
  int PlugInLoadTime() const;

  // Description:
  //   This will check to see if PlugInLoadTime() is set to CRhinoPlugIn::load_plugin_disabled
  // Returns:
  //   True if plug-in is not currently loaded and PlugInLoadTime() == CRhinoPlugIn::load_plugin_disabled
  // See Also:
  //   CRhinoPlugIn::plugin_load_time
  //   CRhinoPlugIn::PlugInLoadTime()
  bool IsLoadDisabled() const;

  ON_DEPRECATED_MSG("Not used in Rhino")
  int PreviousPlugInLoadTime() const;

  // Description:
  //   If the plug-in is loaded this will return the PlugIn()->PlugInType()
  //   otherwise it returns the plug-in type flag found in the registry.
  // Returns:
  //   Returns CRhinoPlugIn::plug-in_type, see CRhinoPlugIn::plug-in_type for details.
  // See Also:
  //   CRhinoPlugIn::plug-in_type
  //   CRhinoPlugIn::PlugInType()
  CRhinoPlugIn::plugin_type PlugInType() const;

  // Description:
  //   Get a localized description for PlugInType() associated with this record.
  // Returns:
  //   Returns localized description for PlugInType() associated with this record.
  // See Also:
  //   CRhinoPlugIn::plug-in_type
  //   CRhinoPlugIn::PlugInType()
  const wchar_t* PlugInTypeString() const;

  // Description:
  //   Get a localized description for specified CRhinoPlugIn::plugin_type.
  // Parameters:
  //   type - [in] Type of plug-in.
  // Returns:
  //   Returns localized description for PlugInType() associated with this record.
  // See Also:
  //   CRhinoPlugIn::plug-in_type
  //   CRhinoPlugIn::PlugInType()
  static const wchar_t* PlugInTypeString( CRhinoPlugIn::plugin_type type);

  // Description:
  //   If the plug-in is loaded this will return the PlugIn()->AddToPlugInHelpMenu()
  //   otherwise it returns the value found in the registry.
  // Returns:
  //   true If plug-in name is added to the Rhino Help/Plug-ins menu.
  // See Also:
  //   CRhinoPlugIn::AddToPlugInHelpMenu()
  bool AddToHelpMenu() const;

  // Description:
  //   If the plug-in is loaded this will return the PlugIn()->PlugInID()
  //   otherwise it returns the registry UUID key name.
  // Returns:
  //   Reference to a unique plug-in UUID.
  // See Also:
  //   CRhinoPlugIn::PlugInID()
  UUID PlugInUUID() const;

  // Description:
  //   Search list of registered command names for specified command name.
  //   This is used when looking for command names associated with a registered
  //   but unloaded plug-in.
  // Parameters:
  //   lpsCmdName - [in] Command name to search for.
  //   bEnglishCommandName - [in] If true then look in the english command
  //                              name list otherwise look in the localized
  //                              command name list.
  // Returns:
  //   true If command name found.
  bool ContainsCommandNamed( const wchar_t* lpsCmdName, bool bEnglishCommandName) const;

  const wchar_t* EnglishCommandName( int nIndex) const;
  const wchar_t* LocalizedCommandName( int) const;

  // Description:
  //   Add English and localized command names to appropriate list.
  // Parameters:
  //   lpsEnglishCommandName - [in] English command name.
  //   lpsLocalizedCommandName - [in] Localized command name
  //                                 (may be same as English if not localized)
  //   nFlags - [in] See CRhinoCommand::m_cmd_flags for a description of flags.
  void AddCommand( const wchar_t* lpsEnglishCommandName, const wchar_t* lpsLocalizedCommandName, int nFlags = 0);

  // Description:
  //   Used to iterate list of registered commands associated with this plug-in.
  // Returns:
  //   Pointer to a CRhinoPlugInCommandRecordif nIndex is valid otherwise NULL.
  // Parameters:
  //   nIndex - [in] Zero based index of command to return.  Value must be 
  //                 >= 0 and less than CommandRecordCount()
  const CRhinoPlugInCommandRecord* GetCommandRecord( int nIndex) const;

  // Description:
  //   Length of command list.
  int CommandRecordCount() const;

  // Description:
  //   Add to list of file types associated with a CRhinoFileImport or CRhinoFileExport plug-in
  //   that is registered but not loaded.  This is used by the Rhino file dialog when displaying
  //   file types.
  // Parameters:
  //   lpsDescr - [in] File type description.
  //   lpsExtensions - [in] File extensions supported by this description.
  void AddFileTypes( const wchar_t* lpsDescr, const wchar_t* lpsExtensions);

  // Description:
  //   Length of file type list associated with file import/export plug-in.
  int FileTypeCount() const;

  // Description:
  //   Used to iterate file type list.  If invalid index is passed then
  //   a bogus, empty CRhinoFileType reference is returned.
  // Returns:
  //   Length of file type list.
  // See Also:
  //   CRhinoPlugInRecord::AddFileTypes()
  //   CRhinoFileType
  //   CRhinoFileImportPlugIn::AddFileType
  //   CRhinoFileExportPlugIn::AddFileType

  const CRhinoFileType& FileType( int index) const;
  // Description:
  //   Check to see if plug-in was successfully loaded.
  // Returns:
  //   true If plug-in is currently loaded.
  bool IsLoaded() const;

  // Description:
  //   Check to see if a previous attempt was made to load this plug-in.
  // Returns:
  //   true If one or more attempts was made to load this plug-in.
  bool LoadAttempted() const;

  // Description:
  //   Number of attempts made to load this plug-in.
  int LoadAttemptedCount() const;

  // Description:
  //   Determine if the plug-in is a .NET plug-in or a C++ plug-in;
  // Returns:
  //   true If plug-in is a .NET plug-in
  bool IsDotNETPlugIn() const;

  // Description:
  //   Lets you set load protection.  This value is saved in the 
  //   HKEY_CURRENT_USER section of the registry.
  //   and determines how the plug-in will load next time.
  //   This is NOT the way SAFEMODE prevents loading of plug-ins.
  // Parameters:
  //  load_protection - [in]
  //       0,1:  silent load
  //       2:  query load - when the plug-in manager attempts to load
  //                        the plug-in, a modal Yes/No message box 
  //                        pops up and asks the user if they want 
  //                        to load the plug-in.
  //       any other value: treated as 2.
  void SetLoadProtection(int load_protection);
  int LoadProtection() const;

  const ON_ClassArray<CRhinoFileType>& FileTypes() const;
  int LoadOrder() const;

  // Is this plug-in one that ships with Rhino
  bool IsShippingPlugIn() const;

  bool IsDirectoryInstall() const;

  bool IsPackageManagerPlugIn() const;
  static bool IsPackageManagerPlugIn(const wchar_t* path);

protected:
  friend class CRhPlugInManager;
  friend class CPlugInManagerDlg;
  friend class CRhInstallPlugIn;

  CRhinoPlugIn*    m_plug_in = nullptr;
  CRhinoPlugInInfo m_plug_in_info;
  HINSTANCE        m_hInstance = nullptr;
#if defined (ON_RUNTIME_WIN)
  ON_wString       m_wPath;
#endif
  int              m_on_classid_mark = -1;
#if defined (ON_RUNTIME_APPLE)
  id               m_bundle = nil;      // Cocoa bundle that contains plug-in
#endif
  //
  // From registered but not loaded class
  //
  ON_wString  m_wRegPath;
  ON_wString  m_wName;
  ON_wString  m_wFileName;
  int         m_nLoadAttempts = 0;
  int         m_nLoadMode = CRhinoPlugIn::load_plugin_when_needed;
  int         m_nPreviousLoadMode = -1;  // initialized to -1, set whenever the load mode value of a plug-in is written to the registry
  bool        m_bAddToHelpMenu = false;
  UUID        m_plug_in_id = ON_nil_uuid;
  CRhinoPlugIn::plugin_type     m_type = CRhinoPlugIn::unknown_plugin_type;
  ON_ClassArray<CRhinoPlugInCommandRecord>m_command_list;
  ON_ClassArray<CRhinoFileType> m_file_types;
  bool m_bIsDotNETPlugIn = false;
  bool m_bIsDotNETAndIsLoaded = false;
  int m_load_order = -1;
  // Added by Dale Lear 25 February 2004
  //
  //   In SDK's with RHINO_SDK_SERVICE_RELEASE >= 200402250.
  //   We need a way for Rhino to prevent loading of certain plug-ins
  //   that are known to cause trouble.  The default load protection
  //   is 0.  If a user decides a certain plug-in is causing trouble,
  //   they can use the plug-in manager to add load protection.
  //   The reason that both the 0 and 1 values mean silent loading
  //   is so that we don't waste time saving the default information
  //   in registry unless we are actually dealing with a plug-in 
  //   where the user has explicitly changed the load protection.
  //   
  //       0 = silently load - (default setting)
  //       1 = silently load - (setting came from registry)
  //       2 = query load (yes/no modal message box)
  //
  //   The PlugInManager command provides UI for changing setting
  //   the load protection to silent load or query load.  
  //
  //   The non-default load permissions are stored in 
  //     HKEY_CURRENT_USER\...\Global Settings\Plug-ins\<plug-in uuid>\LoadProtection
  //   so that ordinary users (non-administrators) can control loading of
  //   buggy plug-ins.
  //
  //   Use CRhinoPlugIn::SaveLoadProtectionToRegistry(plug-in_uuid,m_LoadProtection)
  //
  //   to save this value if it is ever changed.
  //
  //   ALERT - snotty comment follows:
  //     Because the CRhPlugInManager has become such a twisted pile of crap,
  //     this value has to be stored on 2 different classes: 
  //       CRhinoPlugInRecord::m_nLoadProtection
  //       CRhinoInstallPlugIn::m_nLoadProtection
  //     and kept in sync.  In V4 the information on these three similar
  //     classes will be stored in a SINGLE CLASS.  There will be ONE
  //     instance of the class for each UNIQUE plug-in.  The values on
  //     this new class will indicate the status of the plug-in.  The
  //     settings on this class will be stored someplace that ordinary
  //     users have permission to write.  End of rant.
  //
  //   Please discuss changes with Dale Lear.
  int m_nLoadProtection = 0;

private:
  void SetDirectoryInstall(bool on);
  void SetEnglishName(const wchar_t* name);
  class CRhPlugInRecordPrivate* m_private = nullptr;
};

