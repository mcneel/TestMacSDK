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

#include "rhinoSdkFileOptions.h"
#include "rhinoSdkSystemDialogOptions.h"

////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoPlugIn
//
//	Description:
//    This is the base class for every Rhino plug-in type.  Never
//    derive directly from this class use one of the following:
//      CRhinoUtilityPlugIn
//      CRhinoFileImportPlugIn
//      CRhinoFileExportPlugIn
//      CRhinoDigitizerPlugIn
//      CRhinoRenderPlugIn
//      CRhinoDisplayPlugIn
//
//  Remarks:
//    A plug-in is a DLL and MUST be dynamically linked with MFC
//    DLLs.
//
//    Because a plug-in DLL is dynamically linked against the MFC
//    DLLs, any functions exported from this DLL which
//    call into MFC must have the AFX_MANAGE_STATE macro
//    added at the very beginning of the function.
//
//  Example:
//
//    extern "C" BOOL32 PASCAL EXPORT ExportedFunction()
//    {
//       AFX_MANAGE_STATE(AfxGetStaticModuleState());
//       // normal function body here
//    }
//
//    It is very important that this macro appear in each
//    function, prior to any calls into MFC.  This means that
//    it must appear as the first statement within the 
//    function, even before any object variable declarations
//    as their constructors may generate calls into the MFC
//    DLL.
//
//    Please see MFC Technical Notes 33 and 58 for additional
//    details.
//
class RHINO_SDK_CLASS CRhinoPlugIn : public ON_Object
{
  ON_OBJECT_DECLARE(CRhinoPlugIn);

public:

  /*
  Description:
    This function will return true when Rhino is loading a plug-in
    and a single CRhinoPlugIn() class is permitted to be constructed.
    If CRhinoPlugIn() is called and CRhinoPlugIn::PlugInConstructionPermitted()
    is false, it means a plug-in developer is incorrectly constructing
    a class derived from CRhinoPlugIn().  As soon as CRhinoPlugIn::CRhinoPlugIn()
    is called, this function will return false.  This means that in the
    constructor of a class derived from CRhinoPlugIn(), this function
    will always return false.
  Returns:
    true if Rhino is loading a plug-in and a single class derived from
    CRhinoPlugIn is permitted to be constructed.
  */
  static bool PlugInConstructionPermitted();

  CRhinoPlugIn();
  virtual ~CRhinoPlugIn();

#if defined (RHINO_SDK_MFC)
  AFX_MODULE_STATE* PlugInModuleState() const;
#endif

  //////////////////////////////////////////////////////////////////////////////////
  //
  // Description:  The following virtual functions must be overridden
  //

  // Description:
  //    Plug-in unique identifier.
  // Returns:
  //    Reference to a unique plug-in GUID.
  // Remarks:
  //    Use Window's GUIDGEN.EXE to create a GUID.
  virtual GUID PlugInID() const = 0;

  // Description:
  //    Plug-in name display string. This name is displayed by Rhino when loading 
  //    the plug-in, in the plug-in help menu, and in the Rhino 
  //    interface for managing plug-ins.
  // Returns:
  //    String to display.
  virtual const wchar_t* PlugInName() const = 0;
  virtual const wchar_t* LocalPlugInName() const;

  // Description:
  //    Plug-in version display string.  This name is displayed by Rhino 
  //    when loading the plug-in and in the Rhino interface for managing plug-ins.
  // Returns:
  //    String to display.
  virtual const wchar_t* PlugInVersion() const = 0;

  // Description:
  //    Identifies the plug-in type.  The plug-in loader will attempt to cast the
  //    one and only plug-in object depending on this type code.
  //
  // Remarks:
  //    These codes map to the following classes:
  //       Code:                    Class Name:
  //      unknown_plugin_type      Reserved for use by constructor, never use.
  //      render_plugin            CRhinoRenderPlugIn
  //      file_export_plugin       CRhinoFileExportPlugIn
  //      file_import_plugin       CRhinoFileImportPlugIn
  //      digitizer_plugin         CRhinoDigitizerPlugIn
  //      utility_plugin           CRhinoUtilityPlugIn
  //      display_pipeline_plugin  CRhinoDisplayPipelinePlugIn
  //      display_engine_plugin    CRhinoDisplayEnginePlugIn
  enum plugin_type : unsigned int
  {
    unknown_plugin_type = 0,
    render_plugin = 0x01,
    file_export_plugin = 0x02,
    file_import_plugin = 0x04,
    digitizer_plugin = 0x08,
    utility_plugin = 0x10,
    display_pipeline_plugin = 0x20,
    display_engine_plugin = 0x40
  };

  // Description:
  //    Identifies the type of plug-in.
  // Returns:
  //    Codes described above.
  // Remarks:
  //    Rhino will attempt to cast the one and only plug-in derived object when loading 
  //    a plug-in.  If it does not cast correctly then the plug-in will not be allowed to
  //    load.
  virtual plugin_type PlugInType() const = 0;

  //////////////////////////////////////////////////////////////////////////////////
  //
  // Description: The Following are USUALLY overridden, it is not necessary to call
  //              the base class version of these functions.
  //

  // Description:
  //   Plug-in description string. This description is used in various user interfaces.
  // Returns:
  //   String to display.
  virtual const wchar_t* PlugInDescription() const;
  virtual const wchar_t* LocalPlugInDescription() const;

  // Description:
  //   Plug-in icon. This image is used in various user interfaces.
  // Parameters:
  //   size [in] The size of the requested icon.
  // Example:
  //
  //   HICON CTestPlugIn::PlugInIcon(const ON_2iSize& size)
  //   {
  //     unsigned int icon_id = RhinoPlugInIconResourceId();
  //     return CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), icon_id, size.cx, size.cy);
  //   }
  //
  // Returns:
  //   Handle to icon to display.
  virtual HICON PlugInIcon(const ON_2iSize& size);

  // Description:
  //    Called one time when a plug-in is loaded.
  //
  // Returns:
  //    1  = Initialization succeeded, leave the plug-in loaded.
  //    0  = Unable to initialize, unload the plug-in and display load error dialog.
  //    -1 = Unable to initialize, unload the plug-in and do not display load error dialog.
  //            Note: OnUnloadPlugIn will not be called.
  //
  // Remarks:
  //    Plug-ins are not loaded until after Rhino is started and a default document
  //    is created.  Because the default document already exists 
  //    CRhinoEventWatcher::On????Document() functions are not called for the default
  //    document.  If you need to do any document initialization/synchronization then
  //    override this function and do it here.  It is not necessary to call 
  //    CPlugIn::OnLoadPlugIn() from your derived class.
  //
  virtual int OnLoadPlugIn();

  // Description:
  //    If OnLoadPlugIn() returns 0 the Rhino plug-in manager will call this method.
  //    If this method sets message to a valid string and returns true then the
  //    plug-in on load error dialog will display your custom message.
  //
  // Parameters:
  //   msg [out] Error message to display
  //
  // Returns:
  //    true = Display custom message
  //    false = Display system message
  //
  // See Also:
  //    CRhinoPlugIn::OnLoadPlugIn()
  //
  virtual bool OnLoadPlugInErrorString(ON_wString& msg);

  // Description:
  //    The Rhino plug-in manager will call this method when Rhino is preparing
  //    to close. At this point, the Rhino SDK should still be completely
  //    accessible.
  //
  // See Also:
  //    CWinApp::SaveAllSettings
  //    CRhinoPlugIn::OnLoadPlugIn()
  virtual void OnSaveAllSettings();

  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active 
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.
  virtual void OnUnloadPlugIn();

  // Description:
  //    Call this method to get access to persistent plug-in settings
  CRhinoSettings& Settings();

  // Description:
  //    Write settings to disk which will raise a CRhinoPlugInSettingsChanged
  //    event.
  void SaveSettings();

  // Description:
  //   Loads persistent plug-in settings from the Rhino profile.
  // Parameters:
  //   lpszSection - [in] profile section to use for this plug-in
  //   pc - [in] profile context to use to get at
  //             value saved in the Rhino profile.
  // Remarks:
  //   Rhino will call LoadProfile after the plug-in is loaded and before
  //   the plug-in is used.  The default does nothing.
  // See Also:
  //   CRhinoPlugIn::SaveProfile
  virtual void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

  // Description:
  //   Save persistent plug-in settings to the Rhino profile.
  // Parameters:
  //   lpszSection - [in] profile section to use for this command
  //   pc - [in] profile context to use to get at
  //             value saved in the Rhino profile.
  // Remarks:
  //   Rhino will call SaveProfile before unloading your plug-in
  //   and exiting.  Override SaveProfile to save plug-in settings
  //   that need to persist between Rhino sessions.
  //   The default does nothing.
  // See Also:
  //   CRhinoPlugIn::LoadProfile
  virtual void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

  BOOL32 GetPlugInRegistryKey(ON_wString& result) const;

  // Description: The Following are SOMETIMES overridden, it is not necessary 
  //              to call the base class version of these functions.
  //

  // Description:
  //    Called when plug-in self registration begins and ends.
  //
  // Remarks:
  //    When a plug-in is loaded successfully Rhino registers it so it may be loaded
  //    automatically in the future.  This method is called at the beginning and end of
  //    the registration process.
  //
  virtual void OnRegisterPlugIn(bool bDone);

  // Description:
  //    Identifies when Rhino should load this plug-n.  This value is written/read
  //    from/to the \\HEKY_LOCAL_MACHINE\Software\McNeel\Rhinoceros\<rhino version>\Plug-ins\<plug-in GUID>
  //    registry keys "LoadMode" entry.
  //
  enum plugin_load_time
  {
    load_plugin_disabled = 0,     // never load plug-in
    load_plugin_when_needed = 2, // (default) Load the first time a plug-in command used
    load_plugin_at_startup = 1, // Load when Rhino starts
    load_plugin_when_needed_ignore_docked = 6, // Load the first time a plug-in command used NOT when restoring docking control bars
    load_plugin_when_needed_or_optionsdlg = 10, // same as "when needed", plus when the options dialog is shown
    load_plugin_when_needed_or_tabbed_dockbar = 18,	// same as "when needed", plus when a tabbed dockbar is loaded.
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    plugin_load_time_force_32bit = 0xFFFFFFFF
  };

  // Description:
  //    Called by Rhino when writing plug-in information to the registry.  This
  //    information will be read the next time Rhino starts to identify properly
  //    installed plug-ins.
  //
  // Returns:
  //    load_plugin_when_needed = Plug-in will not be loaded when Rhino starts.  Plug-in
  //                              will be loaded when a plug-in defined command is
  //                              run or when a user selects a plug-in defined file
  //                              import/export type.  Plug-in will be loaded if it created
  //                              control bars and one or more control bar was visible when
  //                              Rhino last closed.
  //    load_plugin_when_needed_ignore_docked = Same as load_plugin_when_needed with the exception
  //                              that plug-in will NOT be loaded if it created control bars and
  //                              one or more control bar was visible when Rhino last closed.
  //    load_plugin_at_startup  = Plug-in is loaded when Rhino is loaded and initialized.
  //
  // Remarks:
  //    Override this function and return load_plugin_at_startup to get your plug-in to
  //    load when Rhino does.
  virtual plugin_load_time PlugInLoadTime();

  // Description:
  //    Called by Rhino to determine if the plug-in name should be added to the Rhino
  //    Help/Plug-ins menu.
  //
  // Returns:
  //    true  = Plug-in name is added to menu.
  //    false = Plug-in name is not added to menu.
  //
  // Remarks:
  //    Default is false, to automatically add your plug-in to the Rhino help menu override
  //    and return true.
  virtual BOOL32 AddToPlugInHelpMenu() const;

  // Description:
  //    Called by Rhino if AddToPlugInHelpMenu() returns true and menu item associated
  //    with this plug-in is selected.
  //
  // Parameters:
  //    HWND hWnd = Window handle of the active Rhino interface.
  //
  // Returns:
  //    true  = Help displayed successfully.
  //    false = Error displaying help
  //
  // Remarks:
  //    If AddToPlugInHelpMenu() was overridden and returns true then override this 
  //    function and call your plug-in's help file.
  virtual BOOL32 OnDisplayPlugInHelp(HWND hWnd) const;

  // Description:
  //    Called by Rhino when a plug-in adds a menu or menu item to the Rhino menu and
  //    a associate WM_INITMENUPOPUP message is received.
  //
  // Parameters:
  //    Same as Windows WM_INITMENUPOPUP message
  //
  // Remarks:
  //    If plug-in adds menu items and wants to check/uncheck or gray them out it
  //    should override this function.
  virtual void OnInitPlugInMenuPopups(WPARAM wparam, LPARAM lparam);

  // Description:
  //    Called by Rhino when a plug-in added menu or menu item is activated and the
  //    main Rhino windows CFrameWnd::GetMessageString() method is called.  The string
  //    placed in wMessage will be displayed in the status bar at the bottom of the
  //    main Rhino window.
  //
  // Parameters:
  //    hMenu [in] Menu which is requesting the help string
  //    nID [in] Menu item ID to get help string for
  //    wMessage [out] String to display on the status bar
  //
  // Remarks:
  //    If plug-in adds menu items and wants to check/uncheck or gray them out it
  //    should override this function.
  virtual void GetMenuMessageString(HMENU hMenu, UINT nID, ON_wString& wMessage) const;

  // Description:
  //    Called when a plug-in added menu item is executed.
  //
  // Parameters:
  //    WPARAM wparam = WM_COMMAND WPARAM Passed to Rhino when menu item selected.
  //
  // Remarks:
  //    Override and take action if plug-in adds commands to the Rhino menu.
  virtual BOOL32 OnPlugInMenuCommand(WPARAM wparam);

  /*
  Description:
    Called whenever a Rhino is about to save a version 4 .3dm file.
  Returns:
    True if the plug-in wants to save document user data in the
    version 4 .3dm file.  The default returns false.
  Remarks:
    If you want to save plug-in document data when a model is
    saved in a version 4 .3dm file, then you must override this
    function to return true and you must override WriteDocument().
  */
  virtual BOOL32 CallWriteDocument(const CRhinoFileWriteOptions& options);

  /*
  Description:
    Called whenever a Rhino is about to save a version 3 .3dm file.
  Returns:
    True if the plug-in wants to save document or object user data
    in the version 3 .3dm file.  The default returns false.
  Remarks:
    If you want to save plug-in document data when a model is
    saved in a version 3 .3dm file, then you must override this
    function to return true and you must override WriteDocument().
  */
  virtual bool SaveAsV3(const CRhinoFileWriteOptions& options);

  /*
  Description:
    Called when Rhino is saving a .3dm file to allow the plug-in
    to save document user data.

  Parameters:
    doc - [in]
    archive - [in]
      OpenNURBS file archive object Rhino is using to write the file.
      Use ON_BinaryArchive::Write*() functions to write plug-in data.
      NOTE WELL:
        If your plug-in supports writing version 3 and version 4
        documents, then you must check archive.Archive3dmVersion()
        to determine which version of .3dm file is being saved.
    options - [in]
  Returns:
    If any ON_BinaryArchive::Write*() functions return false,
    then you should immediately return false.  Otherwise return true
    if all data was written successfully.
    Returning false will cause Rhino to stop saving the file.
  */
  virtual BOOL32 WriteDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileWriteOptions& options);

  // Description:
  //     Called whenever a Rhino document is being loaded and plug-in user data was
  //     encountered written by a plug-in with this plug-in's GUID.
  //
  // Parameters:
  //     CRhinoDoc&             = Document being loaded
  //     ON_BinaryArchive&       = OpenNURBS file archive object Rhino is using to
  //                               read this file.  Use ON_BinaryArchive::Read*() functions
  //                               to read plug-in data.
  //     CRhinoFileWriteOptions = Describes what is being written.
  //
  // Returns:
  //     If any ON_BinaryArchive::Read*() functions return false than you should 
  //     immediately return false otherwise return true when all data was read.
  virtual BOOL32 ReadDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileReadOptions& options);

  // Description:
  //   DEPRECATED method, use IRhinoAddPropertiesPages to add object properties panels instead.
  ON_DEPRECATED_MSG("Use IRhinoAddPropertiesPages to add object properties panels instead.")
  virtual void AddPagesToObjectPropertiesDialog(CRhinoPropertiesPanelPageCollection& collection);
  // Description:
  //   Override this function if you want to extend the DocumentProperties dialog
  // Parameters:
  //   collection     [out] - Add IRhinoOptionsPage objects to this collection.
  // Note:
  //   Make sure you include the following macro prior to calling new:
  //    AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //	 Please see MFC Technical Notes 33 and 58 for additional details.
  virtual void AddPagesToDocumentPropertiesDialog(CRhinoOptionsPageCollection& collection);

  // Description:
  //   Override this method if you want to suppress the display of specific document
  //   property dialog tabs.
  // Parameters:
  //   hwndParent  [in] - HWND of parent dialog.
  //   tab_manager [in] - See class CRhinoDocumentPropertiesTabs for details
  virtual void RemoveDocumentPropertiesDialogTabs(CRhinoDocumentPropertiesTabs& tab_manager);

  // Description:
  //   Override this function if you want to extend the Options dialog
  // Parameters:
  //   collection     [out] - Add IRhinoOptionsPage objects to this collection.
  // Note:
  //   Make sure you include the following macro prior to calling new:
  //    AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //	 Please see MFC Technical Notes 33 and 58 for additional details.
  virtual void AddPagesToOptionsDialog(CRhinoOptionsPageCollection& collection);

  /*
  Description:
    Override this function if you want to return an interface pointer for
    a COM object in your plug-in. Note, if you want your plug-in to support
    scripting with RhinoScript, you must return the interface pointer of an
    IDispatch-derived object (IID_IDispatch).
  Parameter:
    iid [in] - The requested interface.
  Return:
    The IUnknown pointer of the requested interface, if successful.
    Return NULL on error or if the interface is not supported.
  Notes:
    It is the responsibility of the application calling this function to
    increment and decrement the object's reference count using AddRef() and Release().
    If the plug-in object being request is a stack variable, then you will
    also want to increment the reference counter yourself so the object will not
    be destroyed when released by the calling application.
  */
  virtual LPUNKNOWN GetPlugInObjectInterface(
    const ON_UUID& iid
  );

  /*
  Description:
  Override this function if your plug-in has "do not show this dialog again"
  dialogs and you want on place to reset any/all settings to show that those dialogs.
  */
  virtual void ResetMessageBoxes();

  bool DisplayPlugInErrorDialog(const wchar_t* message) const;

  // Description:
  //     Helper function to get the HINSTANCE for Rhino received when loading the
  //     plug-in.
  //
  // Returns:
  //     HINSTANCE = Module handle.
  HINSTANCE PlugInInstance() const;

  // Description:
  //     Used to retrieve full path used to load the plug-in.
  //
  // Parameters:
  //     ON_wString& result = Reference to buffer to receive path
  //
  // Returns:
  //     true  = Buffer filled in.
  //     false = Unable to fill in buffer.
  BOOL32 GetPlugInFileName(ON_wString& result) const;
  const wchar_t* PlugInFileName() const;

  // Description:
  //     Get folder plug-in was loaded from.
  //
  // Parameters:
  //     ON_wString& result = Reference to buffer to receive folder name.
  //
  // Returns:
  //     true  = Buffer filled in
  //     false = Could not determining source folder.
  BOOL32 GetPlugInFolder(ON_wString& result) const;

  // Description:
  //     A Plug-in can call this function to add its menu to the Rhino menu bar
  //     This version gets a submenu from a main menu by index and uses the title in
  //     the main menu.
  BOOL32 InsertPlugInMenuToRhinoMenu(HMENU hmenu, int index);

  // Description:
  //     This version takes a submenu and a specific title
  // Returns:
  //     true  = Menu item added
  //     false = Not added.
  BOOL32 InsertPlugInMenuToRhinoMenu(HMENU hmenu, LPCTSTR title);

  // Description:
  //     Insert an item anywhere in rhino menus
  // Returns:
  //     true  = Added
  //     false = Not added
  BOOL32 InsertPlugInItemToRhinoMenu(HMENU hTargetMenu, UINT index, MENUITEMINFO* pMi);

  // Description:
  //     Remove menu added by this plug-in
  // Parameters:
  //     HMENU hTargetMenu = menu to unload
  // Returns:
  //     true  = Removed
  //     false = menu not found or not added by this plug-in
  BOOL32 RemovePlugInMenuFromRhino(HMENU hTargetMenu);
  //--------------------------------------------------------------------------------
  // Description:
  //    Return plug-in type code as descriptive UNICODE string
  //
  // Parameters:
  //    ON_wString& wResult = Object to receive description.
  void PlugInTypeString(ON_wString& wResult) const;

  // Description:
  //    Return descriptive plug-in type string
  // Parameters:
  //    int type = CRhinoPlugIn::plugin_type
  static const wchar_t* PlugInTypeString(int type);

  // Description:
  //   Call this method to determine if the plug-in is a C++ or .NET Rhino Common
  //   runtime plug-in.
  bool IsRhinoCommonPlugIn() const;

  // Description:
  //   Controls the buttons that will appear on the license notification window
  //   that is displayed if a license for the requesting product is not found.
  //   Note, the Close button will always be displayed.
  enum license_capabilities
  {
    no_capabilities = 0x0,       // Only the "Close" button will be displayed<
    can_be_purchased = 0x1,      // Shows "Buy a License" button
    can_be_specified = 0x2,      // Shows "Enter a License" and "Use a Zoo" buttons
    can_be_evaluated = 0x4,      // Shows "Evaluate" button
    evaluation_is_expired = 0x8, // Shows "Evaluate" button disabled
    supports_rhino_accounts = 0x10,  // Enables UI and interaction with Cloud Zoo for the product requesting a license
    supports_standalone = 0x20,       // Enables UI and support for standalone licenses for the product requesting a license
    supports_zoo_per_user = 0x40,     // Enables UI and interaction with Zoo using one license per user for the product requesting a license
    supports_zoo_per_core = 0x80,     // Enables UI and interaction with Zoo using one license per core for the product requesting a license
    supports_license_discovery = 0x100,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    license_dialog_buttons_force_32bit = 0xFFFFFFFF
  };

protected:
  //--------------------------------------------------------------------------------
  // Description:
  //   Verifies that there is a valid product license for your plug-in, using the
  //   Rhino licensing system. If the plug-in is installed as a standalone node,
  //   the locally installed license will be validated. If the plug-in is installed
  //   as a network node, a loaner license will be requested by the system's assigned
  //   Zoo server. If the Zoo server finds and returns a license, then this license
  //   will be validated. If no license is found, then the user will be prompted
  //   to provide a license key, which will be validated. Validation is provided by
  //   your plug-in's one and only CRhinoLicenseValidator-derived class object.
  // Returns:
  //     True if a valid license was found. False otherwise.
  bool GetLicense();

  //--------------------------------------------------------------------------------
  // Description:
  //   Sets internal plug-in variables that control how Rhino displays user interface
  //   when asking the user for license keys for your plug-in. You must call
  //   SetLicenseCapabilities in OnLoadPlugin for Rhino to properly set up the user 
  //   interface when you call GetLicense, or when the user clicks "Change my license..."
  //   in Tools > Options > Licenses
  // Parameters:
  //    capabilities [in] - an unsigned int made by or-ing ( | ) values from
  //                        the CRhinoPlugIn::license_capabilities enum.
  //                        In the event that a license was not found, or if the
  //                        user wants to change the way your plug-in is licenses,
  //                        then provide what capabilities your license has by using
  //                        this enumeration flag.
  //    text_mask    [in] - In the event that the user needs to be asked for a license,
  //                        then you can provide a text mask, which helps the user to
  //                        distinguish between proper and improper user input of your
  //                        license code. Note, if you do not want to use a text mask,
  //                        then pass in a null value for this parameter. 
  //                        For more information on text masks, search MSDN for the
  //                        System.Windows.Forms.MaskedTextBox class.
  //    licenseId    [in] - The UUID used by your plug-in to request a license. This
  //                        may be your plug-ins ID (this->PlugInID()) or another UUID
  //                        of your choosing. As you release new versions of your plug-
  //                        ins, and want them to require a new license, you will need
  //                        to change this licenseId. You'll also need to provide an 
  //                        updated plug-in for the Zoo server, and contact McNeel
  //                        to support this ID in the Rhino accounts licensing system.
  void SetLicenseCapabilities(const wchar_t* format_string, CRhinoPlugIn::license_capabilities capabilities, ON_UUID licenseId);

  //--------------------------------------------------------------------------------
  // Description:
  //   Verifies that there is a valid product license for your plug-in, using the
  //   Rhino licensing system. If the plug-in is installed as a standalone node,
  //   the locally installed license will be validated. If the plug-in is installed
  //   as a network node, a loaner license will be requested by the system's assigned
  //   Zoo server. If the Zoo server finds and returns a license, then this license
  //   will be validated. If no license is found, then the user will be prompted
  //   to provide a license key, which will be validated. Validation is provided by
  //   your plug-in's one and only CRhinoLicenseValidator-derived class object.
  // Parameters:
  //    capabilities [in] - an unsigned int made by or-ing ( | ) values from
  //                        the CRhinoPlugIn::license_capabilities enum.
  //                        In the event that a license was not found, or if the
  //                        user wants to change the way your plug-in is licenses,
  //                        then provide what capabilities your license has by using
  //                        this enumeration flag.
  //    text_mask    [in] - In the event that the user needs to be asked for a license,
  //                        then you can provide a text mask, which helps the user to
  //                        distinguish between proper and improper user input of your
  //                        license code. Note, if you do not want to use a text mask,
  //                        then pass in a null value for this parameter. 
  //                        For more information on text masks, search MSDN for the
  //                        System.Windows.Forms.MaskedTextBox class.
  // Returns:
  //     True if a valid license was found. False otherwise.
  ON_DEPRECATED_MSG("Call SetLicenseCapabilities, then GetLicense(void) instead.")
  bool GetLicense(unsigned int capabilities, const wchar_t* text_mask);

  //--------------------------------------------------------------------------------
  // Description:
  //   Returns, or releases, a product license that was obtained from the Rhino
  //   licensing system. Note, most plug-ins DO NOT need to call this as the Rhino
  //   licensing system will return all licenses when Rhino shuts down. If, for example
  //   your plug-in is a rendering plug-in and you want to release your license when
  //   your rendering plug-ins is no longer the active renderer, then you can use
  //   this function to do this.
  bool ReturnLicense();

  //--------------------------------------------------------------------------------
  // Description:
  //   Get the customer name and organization used when entering the product
  //   license. 
  bool GetLicenseOwner(ON_wString& registeredOwner, ON_wString& registeredOrganization);

public:
  enum plug_in_proc_message_id
  {
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    plug_in_force_32bit = 0xFFFFFFFF
  };

  // This will be used to extend this class without breaking the SDK
  virtual LRESULT PlugInProc(UINT message, WPARAM wParam, LPARAM lParam);

#if defined (ON_RUNTIME_APPLE)
  //--------------------------------------------------------------------------------
  // Description:
  //    Return an Objective-C helper object for this plug-in.
  //        Will return NULL if none implemented.
  //
  virtual
    id PlugInProxy();
  void AddCommand(CRhinoCommand*);
#endif

  const ON_SimpleArray<CRhinoCommand*>& CommandList() const;
protected:

  bool IsValid(ON_TextLog* text_log = nullptr) const override;

  // This is is should only be set by Rhino Common when creating a runtime
  // plug-in, DO NOT set this.
  bool m_is_rhino_common_plug_in;

public:
  // Description:
  //   Saves the plug-in's load protection setting.
  // Parameters:
  //   plug-in_uuid - [in]  If 0, then false is returned and
  //                       *load_protection is set to 2.
  //   load_protection - [out]
  //     0,1: load silently
  //     2: query load
  // Returns:
  //   true if the value came from the registry.
  //   false if the plug-in_uuid = 0 (*load_protection=2)
  //         or no registry entry existed (*load_protection=1)
  static bool SaveLoadProtectionToRegistry(ON_UUID plugin_uuid, int load_protection);

  // Description:
  //   Load registered plug-in .
  //
  // Parameters:
  //   uuid - [in] Plug-in ID of registered plug-in to load.
  //   bQuietly - [in] If true then error messages are not displayed
  //   ignore_load_failed - [in] Load plug-in even if previous attempt to load has failed
  //
  // Returns
  //   values less than zero mean an error occurred
  //   values 0 and greater mean success
  static int LoadPlugIn(const ON_UUID& uuid, bool bQuietly, bool ignore_load_failed);

  // Description:
  //   Returns number of registered and loaded plug-ins.
  static int PlugInCount();

  // Description:
  //   Used to enumerate plug-in record list.
  //
  // Parameters:
  //   nIndex - [in] Zero based index of record to get.  If >= CRhinoPlugIn::PlugInCount()
  //                 then NULL is returned.
  //
  // Returns:
  //   Will return NULL if nIndex is invalid or the list is empty otherwise see CRhinoPlugInRecord
  //   for description.
  static class CRhinoPlugInRecord* GetPlugIn(int nIndex);

  // Description:
  //   Scan plug-in record list for a plug-in with matching file name.
  //
  // Parameters:
  //   lpsFileName - [in] File name to match
  //
  // Returns:
  //   See CRhinoPlugInRecord for description.  Will return NULL if nIndex
  //   is invalid or list is empty.
  static class CRhinoPlugInRecord* GetPlugIn(const wchar_t* lpsFileName);

  // Description:
  //   Scan plug-in record list for a plug-in with matching UUID.
  //
  // Parameters:
  //   uuid - [in] UUID to match
  //
  // Returns:
  //   See CRhinoPlugInRecord for description.  Will return NULL if nIndex
  //   is invalid or list is empty.
  static class CRhinoPlugInRecord* GetPlugIn(ON_UUID uuid);

  // Description:
  //   Scan plug-in record list for a loaded plug-in with matching UUID.
  //
  // Parameters:
  //   uuid - [in] UUID to match
  //
  // Returns:
  //   See CRhinoPlugInRecord for description.  Will return NULL if nIndex
  //   is invalid or list is empty or the plug-in is not currently loaded.
  static class CRhinoPlugInRecord* GetLoadedPlugIn(ON_UUID uuid);

  // Description:
  //   Scan plug-in record list for plug-in's of type specified.
  //
  // Parameters:
  //   result - [out] Plug-in of nPlugInTypes type.
  //   nPlugInTypes - [in] Plug-in types to look for, see CRhinoPlugIn::plug-in_type
  //                       for options.
  //
  // Returns:
  //   true If any plug-ins are found otherwise false.
  static bool GetPlugInList(class ON_ClassArray<CRhinoPlugInRecord>& result, int nPlugInTypes = 0xFFFFFFFF);

  // Returns the ID to be used by the ZooClient when retrieving and storing licenses
  // for this plug-in. If SetLicenseId() is never called, GetLicenseId() returns
  // PluginId()
  const ON_UUID GetLicenseId();

public:
  // Get extended data from plug-in
  class CRhPlugInPrivate* GetPrivateData();

private:
  friend class CRhinoCommand;
  friend class CRhPlugInManager;
  friend RHINO_SDK_FUNCTION void RhinoGetPlugInCommandList(const CRhinoPlugIn* plug_in, ON_SimpleArray<const CRhinoCommand*>& cmd_list);
  friend class CRhMiniDumpHelper;

  ON_UUID m_PlugInUuid; // value returned by PlugInID() when plug-in is registered.
                        // saved here so we get a consistent answer to the question.
  HINSTANCE  m_hPlugInInstance;                   // plug-in DLL application instance
  ON_wString m_wFileName;                         // file name buffer
  ON_SimpleArray<CRhinoCommand*> m_command_list; // commands to be added when done loading
  class AFX_MODULE_STATE* m_pModuleState;
  class CRhPlugInSettings* m_plug_in_settings;
  class CRhinoSettings* m_settings;
  class CRhPlugInPrivate* m_private = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////
//
//  Used by CRhinoFileImport and CRhinoFileExport file plug-ins to add file
//  extensions to the standard Rhino file dialog box.
class RHINO_SDK_CLASS CRhinoFileType
{
public:
  CRhinoFileType();
  CRhinoFileType(const CRhinoFileType& src);

  // Description: constructor
  // Parameters:
  //    GUID guid = Plug-in ID of the CRhinoFileImportPlugIn/CRhinoFileExportPlugIn
  //    const wchar_t* lpsDescription = Description of the file displayed in the open file dialog
  CRhinoFileType(GUID guid, const wchar_t* lpsDescription);

  // Description: constructor
  // Parameters:
  //    GUID guid = Plug-in ID of the CRhinoFileImportPlugIn/CRhinoFileExportPlugIn
  //    const wchar_t* lpsDescription = Description of the file displayed in the open file dialog
  //    const wchar_t* lpsExt         = file extension (without the "*.")
  //    includeOptionsButtonInFileDialog = If true the file open/save dialog will include a Options
  //       button when this file type is selected and will call CRhinoFileImportPlugIn::DisplayOptionsDialog
  //       or CRhinoFileExportPlugIn::DisplayOptionsDialog as appropriate when the button is clicked on.
  CRhinoFileType(GUID guid, const wchar_t* lpsDescription, const wchar_t* lpsExt, bool includeOptionsButtonInFileDialog = false);

  // Description: constructor
  // Parameters:
  //    GUID guid = Plug-in ID of the CRhinoFileImportPlugIn/CRhinoFileExportPlugIn
  //    const wchar_t* lpsDescription= Description of the file displayed in the open file dialog
  //    const wchar_t* lpsExt1 = file extension (without the "*.")
  //    const wchar_t* lpsExt2 = another file extension associated with this file type
  //    includeOptionsButtonInFileDialog = If true the file open/save dialog will include a Options
  //       button when this file type is selected and will call CRhinoFileImportPlugIn::DisplayOptionsDialog
  //       or CRhinoFileExportPlugIn::DisplayOptionsDialog as appropriate when the button is clicked on.
  CRhinoFileType(GUID guid, const wchar_t* lpsDescription, const wchar_t* lpsExt1,
    const wchar_t* lpsExt2, bool includeOptionsButtonInFileDialog = false);
  // Description: constructor
  // Parameters:
  //    GUID guid = Plug-in ID of the CRhinoFileImportPlugIn/CRhinoFileExportPlugIn
  //    const wchar_t* lpsDescription= Description of the file displayed in the open file dialog
  //    const wchar_t* lpsExt1 = file extension (without the "*.")
  //    const wchar_t* lpsExt2 = alternate file extension associated with this file type
  //    const wchar_t* lpsExt3 = another file extension associated with this file type
  //    includeOptionsButtonInFileDialog = If true the file open/save dialog will include a Options
  //       button when this file type is selected and will call CRhinoFileImportPlugIn::DisplayOptionsDialog
  //       or CRhinoFileExportPlugIn::DisplayOptionsDialog as appropriate when the button is clicked on.
  CRhinoFileType(GUID guid, const wchar_t* lpsDescription, const wchar_t* lpsExt1,
    const wchar_t* lpsExt2, const wchar_t* lpsExt3, bool includeOptionsButtonInFileDialog = false);

  ~CRhinoFileType();
  CRhinoFileType& operator=(const CRhinoFileType& src);

  // Description:
  //    Set ID of the plug-in that reads these files (or you can set it in the constructor)
  // Parameter:
  //    GUID guid = ID of plug-in that reads this file type
  void SetFileTypePlugInID(GUID guid);

  // Description:
  //    Add file extension or alternate if called more than once.
  void AddFileTypeExtension(const wchar_t* lpsExt);

  // Description:
  //    Set file description (or you can set it in the constructor)
  void FileTypeDescription(const wchar_t* lpsDescription);

  // Description:
  //    Get description associated with this file extensions
  const wchar_t *FileTypeDescription() const;

  // Description:
  //    Get file extension (returns NULL if index is out of range)
  const wchar_t *FileTypeExtension(int) const;

  // Description:
  //    ID of the plug-in that reads these files (or you can set it in the constructor)
  // Returns:
  //    GUID = Plug-in ID that reads/writes this file type
  GUID FileTypePlugInID() const;

  // Description:
  //    If true then the file open/save dialog will include an options button and
  //    call the associated plug-in's DisplayOptionsDialog when the button is clicked.
  // Returns:
  //    Returns true then the file open/save dialog will include an options button and
  //    call the associated plug-in's DisplayOptionsDialog when the button is clicked.
  bool DisplayOptionsDialog() const;

  // Description:
  //    Call this method to tell Rhino if the file type plug-in supports the options
  //    feature.
  void SetDisplayOptionsDialog(bool value);

private:
  // Variables
  GUID                      m_plugin_id;        // Plug-in ID
  ON_wString                m_file_description; // File description
  ON_ClassArray<ON_wString> m_file_extensions;  // List of acceptable file extensions that
                                                // match this description.
  // 17 Jan 2012  - Lowell - Changed this pointer to private since I'm going to define
  // a CRhFileTypeSdkExtension class and use it to 
  // store the order of file type registration by each plug-in
  //public:
  class CRhFileTypeSdkExtension* m__file_type_sdk_extension;
public:
  // Description:
  //   Get index value saved with SetFileTypeIndex.  This is used for figuring out the
  //   index of a file type that a plug-in originally defined because the filter list
  //   on the file open/save dialogs re-sort indices based what we think is the best
  //   way to sort on a given day.  Currently we are sorting alphabetically by file
  //   description.
  // Parameters:
  //   value_if_not_stored = value to return if SetFileTypeIndex was never called for
  //   this class instance
  int GetStoredFileTypeIndex(int value_if_not_stored) const;
  void SetFileTypeIndex(int index);
};


////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoFileImportPlugIn
//
//	Description:
//      Derive plug-in control object from this class to create a file import plug-in.
//      This class provides interfaces for extending the Rhino file open dialog, 
//      drawing custom preview images and reading files.
class RHINO_SDK_CLASS CRhinoFileImportPlugIn : public CRhinoPlugIn
{
  ON_OBJECT_DECLARE(CRhinoFileImportPlugIn);

public:
  CRhinoFileImportPlugIn();
  ~CRhinoFileImportPlugIn();

  plugin_type PlugInType() const override { return file_import_plugin; }

  //--------------------------------------------------------------------------------
  // Description:
  //    When Rhino gets ready to display the open file dialog, it calls AddFileType()
  //    once for each loaded CRhinoFileImportPlugIn.
  //
  // Parameters:
  //    ON_ClassArray<CRhinoFileType>& ext_list = Append your supported file types
  //                                               to this list
  //    const CRhinoFileReadOptions& opts       = File read options,
  //
  // Example:
  //    If your file import plug-in reads "My Geometry Files" that have  a ".mygeo"
  //    extension, then your AddToFileType(....) would look like
  //
  //   CMyGeometryImportPlugIn::AddToFileType( ON_ClassArray<CRhinoFileType>& ft_list,
  //                                             const CRhinoFileReadOptions& opts)
  //   {
  //      CRhinoFileType ft( PlugInID(), L"My Geometry Files (*.mygeo)", L"mygeo");
  //      ft_list.Append( ft);
  //   }
  //     
  virtual
    void AddFileType(ON_ClassArray<CRhinoFileType>& ext_list, const CRhinoFileReadOptions& opts) = 0;
  //--------------------------------------------------------------------------------
  // Description:
  //   OBSOLETE - See GetFileDialogPreviewImageAndNotes
  //
  //    When a user single clicks on a file in the file open dialog, Rhino displays a
  //    preview image of the file's contents.  If you want to paint a preview image
  //    then override OnPaintFileDialogPreviewImage() and return true.  If 
  //    OnPaintFileDialogPreviewImage() returns false, then Rhino will clear the 
  //    preview image.  This method is only called if calling
  //    OnPaintFileDialogPreviewImageAndNotes() returns false.
  //
  // Returns:
  //    true  = This plug-in painted preview.
  //    false = This plug-in did not paint, let file dialog paint.
  //
  // Parameters:
  //    const wchar_t* lpsFileName = File name (full path)
  //    HWND hWnd                  = Handle to preview window.
  //ON_DEPRECATED virtual
  //BOOL32 OnPaintFileDialogPreviewImage( const wchar_t* lpsFileName, HWND hWnd);
  //--------------------------------------------------------------------------------
  // Description:
  //    When a user single clicks on a file in the file open dialog, Rhino displays a
  //    preview image of the file's contents and notes associated with the file.
  //    If you want to paint a preview image or modify the notes control then override
  //    this method and return true.  If this method returns false Rhino will clear the
  //    preview image.
  //
  // Returns:
  //    true  = This plug-in painted preview and/or updated the notes
  //    false = This plug-in did not paint, let file dialog paint.
  //
  // Parameters:
  //    lpsFileName = File name (full path)
  //    dib         = 24 bit DIB which is the size of the preview image window.
  //    ctrlNotes   = Read-only rich edit control to display file information in
  virtual
    BOOL32 GetFileDialogPreviewImageAndNotes(const wchar_t* lpsFileName, class CRhinoDib& dib, class CRhinoUiRichEditCtrl& ctrlNotes);
  //--------------------------------------------------------------------------------
  // Description:
  //    Rhino calls ReadFile() to create document geometry.
  //
  // Parameters:
  //    const wchar_t* lpsFileName = Name of file to read
  //    int nFileTypeIndex         = Index of file extension added to list in 
  //                                 AddToFileType() callback.
  //    CRhinoDoc& doc            = Empty document containing nothing but a Default
  //                                 layer.
  //    const CRhinoFileReadOptions& options = File read options
  //
  // Remarks:
  //    The plug-in is responsible for opening lpsFile, reading it and creating document
  //    geometry.
  virtual
    BOOL32 ReadFile(const wchar_t* lpsFileName, int index, CRhinoDoc& doc,
      const CRhinoFileReadOptions& options) = 0;
  //--------------------------------------------------------------------------------
  // Description:
  //    Called when AddFileType was called, a CRhinoFileType was added passing a true
  //    as the includeOptionsButtonInFileDialog constructor parameter and the file 
  //    type is selected in the file open dialog.
  //
  // Parameters:
  //    parent                     = File dialog box handle to be used as the parent
  //        for the plug-in provided options dialog box.
  //    const wchar_t* lpsFileName = Name of file
  //    int nFileTypeIndex         = Index of file extension added to list in 
  //                                 AddToFileType() callback.
  //    const CRhinoFileReadOptions& options = File read options
  //
  // Remarks:
  //    The plug-in is responsible for opening lpsFile, reading it and creating document
  //    geometry.
  virtual void DisplayOptionsDialog(HWND parent, const CRhinoFileType& fileType);
  //--------------------------------------------------------------------------------
  // Description:
  //    ReadFile should use this function to decorate table object names (such as
  //    layer names) with the file name when referencing a file.
  //
  // Returns:
  //    true  = ReadFile is being called by the Worksession commands Attach option
  //            and new_name now contains modified name.
  //    false = Not called at the correct time or lpsName is NULL string.
  // Parameters:
  //    lpsName [in]   = name to prefix
  //    new_name [out] = buffer to receive new name.
  //
  // Remarks:
  //    This will only work when ReadFile is being called by the Worksession/Attach
  //    command.
  BOOL32 MakeReferenceTableName(const wchar_t* lpsName, ON_wString& new_name) const;

public:
  enum file_import_plug_in_proc_message_id
  {
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    file_import_plug_in_force_32bit = 0xFFFFFFFF
  };

  // This will be used to extend this class without breaking the SDK
  virtual LRESULT FileImportPlugInProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoFileExportPlugIn
//
//	Description:
//      Derive plug-in control object from this class to create a file export plug-in.
//      This class provides interfaces for extending the Rhino file save dialog, 
//      drawing custom preview images and writing files.
class RHINO_SDK_CLASS CRhinoFileExportPlugIn : public CRhinoPlugIn
{
  ON_OBJECT_DECLARE(CRhinoFileExportPlugIn);
public:
  CRhinoFileExportPlugIn();
  ~CRhinoFileExportPlugIn();

  plugin_type PlugInType() const override { return file_export_plugin; }

  //--------------------------------------------------------------------------------
  // Description:
  //    When Rhino gets ready to display the save file dialog, it calls AddFileType()
  //    once for each loaded CMyGeometryExportPlugIn.
  //
  // Parameters:
  //    ON_ClassArray<CRhinoFileType>& ext_list = Append your supported file types
  //                                               to this list
  //    const CRhinoFileWriteOptions& opts      = File write options,
  //
  //
  // Example:
  //    If your file import plug-in writes "My Geometry Files" that have  a ".mygeo"
  //    extension, then your AddToFileType(....) would look like
  //
  //   CMyGeometryExportPlugIn::AddToFileType( ON_ClassArray<CRhinoFileType>&  ft_list,
  //                                             const CRhinoFileWriteOptions& opts)
  //   {
  //      CRhinoFileType ft( PlugInID(), L"My Geometry Files (*.mygeo)", L"mygeo");
  //      ft_list.Append( ft);
  //   }
  //
  virtual
    void AddFileType(ON_ClassArray<CRhinoFileType>& ext_list,
      const CRhinoFileWriteOptions& options) = 0;
  //--------------------------------------------------------------------------------
  // Description:
  //   OBSOLETE - See GetFileDialogPreviewImageAndNotes
  //
  //    When a user single clicks on a file in the file open dialog, Rhino displays a
  //    preview image of the file's contents.  If you want to paint a preview image
  //    then override OnPaintFileDialogPreviewImage() and return true.  If 
  //    OnPaintFileDialogPreviewImage() returns false, then Rhino will clear the 
  //    preview image.  This method is only called if calling
  //    OnPaintFileDialogPreviewImageAndNotes() returns false.
  //
  // Returns:
  //    true  = This plug-in painted preview.
  //    false = This plug-in did not paint, let file dialog paint.
  //
  // Parameters:
  //    const wchar_t* lpsFileName = File name (full path)
  //    HWND hWnd                  = Handle to preview window.
  ON_DEPRECATED_MSG("see GetFileDialogPreviewImageAndNotes")
    virtual BOOL32 OnPaintFileDialogPreviewImage(const wchar_t* lpsFileName, HWND hWnd);
  //--------------------------------------------------------------------------------
  // Description:
  //    When a user single clicks on a file in the file open dialog, Rhino displays a
  //    preview image of the file's contents and notes associated with the file.
  //    If you want to paint a preview image or modify the notes control then override
  //    this method and return true.  If this method returns false Rhino will clear the
  //    preview image.
  //
  // Returns:
  //    true  = This plug-in painted preview and/or updated the notes
  //    false = This plug-in did not paint, let file dialog paint.
  //
  // Parameters:
  //    lpsFileName = File name (full path)
  //    dib         = 24 bit DIB which is the size of the preview image window.
  //    ctrlNotes   = Read-only rich edit control to display file information in
  virtual
    BOOL32 GetFileDialogPreviewImageAndNotes(const wchar_t* lpsFileName, class CRhinoDib& dib, class CRhinoUiRichEditCtrl& ctrlNotes);
  //--------------------------------------------------------------------------------
  // Description:
  //    Rhino calls WriteFile() to create document geometry.
  //
  // Parameters:
  //    const wchar_t* lpsFileName = Name of file to write
  //    int nFileTypeIndex         = Index of file extension added to list in 
  //                                 AddToFileType() callback.
  //    CRhinoDoc& doc            = Empty document containing nothing but a Default
  //                                 layer.
  //    const CRhinoFileWriteOptions& options = File read options
  //
  // Returns:
  //    > 0 = File was successfully written
  //      0 = Not able to write file for some reason
  //    < 0 = File writing canceled
  //
  // Remarks:
  //    The plug-in is responsible for opening lpsFile and writing document data to it.
  virtual
    int WriteFile(const wchar_t* lpsFileName, int index, CRhinoDoc& doc,
      const CRhinoFileWriteOptions& options) = 0;

  //--------------------------------------------------------------------------------
  // Description:
  //    Called when AddFileType was called, a CRhinoFileType was added passing a true
  //    as the includeOptionsButtonInFileDialog constructor parameter and the file 
  //    type is selected in the file save dialog.
  //
  // Parameters:
  //    parent                     = File dialog box handle to be used as the parent
  //        for the plug-in provided options dialog box.
  //    const wchar_t* lpsFileName = Name of file
  //    int nFileTypeIndex         = Index of file extension added to list in 
  //                                 AddToFileType() callback.
  //    const CRhinoFileReadOptions& options = File read options
  //
  // Remarks:
  //    The plug-in is responsible for opening lpsFile, reading it and creating document
  //    geometry.
  virtual void DisplayOptionsDialog(HWND parent, const CRhinoFileType& fileType);
public:
  enum file_export_plug_in_proc_message_id
  {
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    file_export_plug_in_force_32bit = 0xFFFFFFFF
  };

  // This will be used to extend this class without breaking the SDK
  virtual LRESULT FileExportPlugInProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoDigitizerPlugIn
//
//  Description:
//    Control class used to create digitizer plug-ins.
//
class RHINO_SDK_CLASS CRhinoDigitizerPlugIn : public CRhinoPlugIn
{
  ON_OBJECT_DECLARE(CRhinoDigitizerPlugIn);
public:
  CRhinoDigitizerPlugIn();
  ~CRhinoDigitizerPlugIn();

  plugin_type PlugInType() const override { return digitizer_plugin; }

  ////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Digitizer plug-in specific
  //

  /*
  Description:
    Called by Rhino to enable/disable input from the digitizer.
    If bEnable is true and EnableDigitizer() returns false,
    then Rhino  will not calibrate the digitizer.
  Parameters:
    bEnable - [in] If true, enable the digitizer.  If false,
                   disable the digitizer.
  Remarks:
    This is pure virtual function that every digitizer plug-in
    must override.
  */
  virtual
    bool EnableDigitizer(
      bool bEnable
    ) = 0;

  /*
  Description:
    Unit system digitizer is using.  Rhino uses this value when
    it calibrates a digitizer.
  Parameters:
    bEnable - [in] If true, enable the digitizer.  If false,
                   disable the digitizer.
  Returns:
    Unit system of the points the digitizer passes to
    SendPoint().  This unit system must be not change.
  Remarks:
    This is pure virtual function that every digitizer plug-in
    must override.
  */
  virtual
    ON::LengthUnitSystem UnitSystem() const = 0;

  /*
  Description:
    The point tolerance is the distance the digitizer must move
    (in digitizer coordinates) for a new point to be considered
    real rather than noise.  Small desktop digitizer arms
    have values like 0.001 inches and 0.01 millimeters.  This value
    should never be smaller than the accuracy of the digitizing
    device.
  Returns:
    The point tolerance of digitizer device.
  Remarks:
    This is pure virtual function that every digitizer plug-in
    must override.
  */
  virtual
    double PointTolerance() const = 0;

  /*
  Description:
    If the digitizer is enabled, call this function to send a point
    to Rhino.  Call this function as much as you like.  The digitizers
    that Rhino currently supports send a point every 15 milliseconds
    or so.  This function should be called when users press or release
    any digitizer button.
  Parameters:
    point - [in] 3d point in digitizer coordinates
    nFlags - [in] Like the nFlags parameter of Windows OnMouseMove.
       Set the MK_LBUTTON | MK_RBUTTON | MK_MBUTTON bits if the
       corresponding digitizer button is down.
  */
  void SendPoint(
    ON_3dPoint point,
    UINT nFlags
  );

  void SendRay(
    ON_3dRay ray,
    UINT nFlags
  );

public:
  enum digitizer_plug_in_proc_message_id
  {
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    digitizer_plug_in_force_32bit = 0xFFFFFFFF
  };

  // This will be used to extend this class without breaking the SDK
  virtual LRESULT DigitizerPlugInProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoUtilityPlugIn
//
//  Description:
//    Control class for writing plug-in that adds commands to Rhino.
//
class RHINO_SDK_CLASS CRhinoUtilityPlugIn : public CRhinoPlugIn
{
  ON_OBJECT_DECLARE(CRhinoUtilityPlugIn);
public:
  CRhinoUtilityPlugIn() = default;
  virtual ~CRhinoUtilityPlugIn() = default;

  plugin_type PlugInType() const override { return utility_plugin; }

  // Used to extend class without breaking the SDK
  virtual LRESULT UtilityPlugInProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};


/////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoLicenseLease
//  Description:
//		This class represents a lease from the Cloud Zoo for a particular
//		product.
//
class RHINO_SDK_CLASS CRhinoLicenseLease
{
public:
  CRhinoLicenseLease(
    const wchar_t* product_id,
    const wchar_t* group_name,
    const wchar_t* group_id,
    const wchar_t* user_name,
    const wchar_t* user_id,
    const wchar_t* product_title,
    const wchar_t* product_version,
    const wchar_t* product_edition,
    const wchar_t* lease_id,
    time_t iat,
    time_t exp);

  CRhinoLicenseLease(
    const wchar_t* product_id,
    const wchar_t* group_name,
    const wchar_t* group_id,
    const wchar_t* user_name,
    const wchar_t* user_id,
    const wchar_t* product_title,
    const wchar_t* product_version,
    const wchar_t* product_edition,
    const wchar_t* lease_id,
    time_t iat,
    time_t exp,
    time_t renewable_until);

  CRhinoLicenseLease(const CRhinoLicenseLease& other);

  CRhinoLicenseLease& operator = (const CRhinoLicenseLease &other);

  ~CRhinoLicenseLease();

  //The product id of the lease.
  ON_wString ProductId() const;
  //The display name of the group whom the lease belongs to.
  //If it is an empty string, then the lease does not belong to a group.
  ON_wString GroupName() const;
  //The id of the group whom the lease belongs to.
  //If it is an empty string, then the lease does not belong to a group.
  ON_wString GroupId() const;
  //The display name of the user whom the lease belongs to.
  ON_wString UserName() const;
  //The id of the user whom the lease belongs to.
  ON_wString UserId() const;
  //The title of the product whom the lease represents. I.E. 'Rhinoceros'
  ON_wString ProductTitle() const;
  //The version of the product whom the lease represents. I.E. '6.0'
  ON_wString ProductVersion() const;
  //The edition of the product whom the lease represents. I.E. 'Commercial'
  ON_wString ProductEdition() const;
  //The unique id of the lease.
  ON_wString LeaseId() const;
  //The time the lease was issued, represented as the number of seconds
  //that have passed since midnight, 1st January 1970 UTC.
  time_t Iat() const;
  //The time the lease expires, represented as the number of seconds
  //that have passed since midnight, 1st January 1970 UTC.
  time_t Exp() const;
  //True if the lease is expired. Otherwise false.
  bool IsExpired() const;
  // If the license backing this lease expires, its expiration date
  // will be set here. Otherwise, RenewableUntil will return a COleDateTime 
  // object with .invalid set to true.
  time_t RenewableUntil() const;

private:
  ON_wString m_lease_id;
  ON_wString m_product_id;
  ON_wString m_group_name;
  ON_wString m_group_id;
  ON_wString m_user_name;
  ON_wString m_user_id;
  ON_wString m_product_title;
  ON_wString m_product_version;
  ON_wString m_product_edition;
  time_t m_iat;
  time_t m_exp;
};


/////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoLeaseChangedEventArgs
//
class RHINO_SDK_CLASS CRhinoLeaseChangedEventArgs
{
private:
  CRhinoLeaseChangedEventArgs() = default;

public:
  ~CRhinoLeaseChangedEventArgs();
  CRhinoLicenseLease* GetLease() const;
  static CRhinoLeaseChangedEventArgs *Create(const CRhinoLicenseLease* lease);

private:
  CRhinoLicenseLease* m_lease = nullptr;
};


/////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoLicenseValidator
//
//  Description:
//    Class for validating licenses for the Rhino license manager.
//    For each plug-in, there can only be a single CRhinoLicenseValidator
//    derived class and a single static instance that exists in the .CPP file
//    where the class is defined.  No other declarations or instances of a class
//    derived from CRhinoLicenseValidator should ever be defined or created.
//
//  For details on how the licensing system in Rhino works, see
//    http://developer.rhino3d.com/guides/cpp/creating-zoo-plugins/
//
class RHINO_SDK_CLASS CRhinoLicenseValidator
{
public:
  CRhinoLicenseValidator();
  virtual ~CRhinoLicenseValidator();

  enum product_build_type
  {
    unspecified_build = 0,               // Unspecified
    release_build = 100,                 // Release license (e.g. commercial, educational, etc.)
    evaluation_build = 200,              // Evaluation license
    beta_build = 300,                    // Beta or work-in-progress license
    build_type_force_32bit = 0xFFFFFFFF, // For internal use only
  };

  /*
  Description:
    This member is called by Rhino, from CRhinoPlugIn::GetLicense(), when it needs to know what
    the build type of your plug-in is. This value is used when looking for licenses for your product.
  */
  virtual
    product_build_type ProductBuildType() = 0;

  enum result
  {
    error_hide_message = -1,         // There was an error validating the product key or license,
                                     // do not allow the license manager show an error message -
                                     // you will display your own error message.
    error_show_message = 0,          // There was an error validating the product key or license,
                                     // allow the license manager show an error message.
    success = 1,                     // The product key or license is validated successfully.
    result_force_32bit = 0xFFFFFFFF, // For internal use only
  };

  /*
  Description:
    Oops. This pure virtual should have been removed from the Rhino SDK. It won't ever be called
    because VerifyLicenseKey will be called instead.

    Please override this function in your class and return CRhinoLicenseValidator::result::error_show_message.
  */
  virtual
    CRhinoLicenseValidator::result ValidateProductKey(const wchar_t* product_key) = 0;

  /*
  Description:
    This member is called by Rhino, from CRhinoPlugIn::GetLicense(), when it needs your plug-in to
    validate your product key or license. If your plug-in determines that product_key represents a
    valid product key or license, then the function should fill out the class members (see below),
    which describe the license, before returning success.

    See http://developer.rhino3d.com/guides/cpp/creating-zoo-plugins/ for details
    Parameters:
      licenseKey - [in] The product key or license as saved in a license file or returned by a Zoo server.
      validationCode - [in] A validation code returned by your validation server
      validationCodeInstalledDate - [in] The date your validation code was associated with this license key.
      gracePeriodExpired - [in] True if the license validation grace period is expired
    Returns:
      success
      error_hide_message
      error_show_message
    */
  virtual
    CRhinoLicenseValidator::result VerifyLicenseKey(const wchar_t* licenseKey, const wchar_t* validationCode, const double validationCodeInstalledDate, bool gracePeriodExpired) = 0;

  /*
  Description:
    Called by Rhino, from CRhinoPlugIn::GetLicense(), after a call to VerifyLicenseKey that
    sets m_requires_previous_version_license_verification to true.
  Parameters:
    license - [in] your license key
    previousVersionLicenseKey - [in] a license key from a previous version of your product.
    This is used for upgrade eligibility.
  Returns:
    true when previousVersionLicense is a valid previous version license for license.
    false otherwise
  */
  virtual
    bool VerifyPreviousVersionLicense(const wchar_t* license, const wchar_t* previousVersionLicense) = 0;

  /*
  Description:
    When Rhino Accounts gets a new lease, this function is called.
    If the Rhino Accounts server determines that this lease is no longer valid, it will
    signal Rhino, and OnLeaseChanged will be called with a null CRhinoLeaseChangedEventArgs.Lease
  */
  virtual
    void OnLeaseChanged(CRhinoLeaseChangedEventArgs&) = 0;

  void SetLease(const CRhinoLicenseLease*);

  /*
  Description:
    Returns a pointer to the plug-in that this validator is associated with.
  */
  CRhinoPlugIn* PlugIn();

public:
  // This value will never be display in any user interface.
  // When your plug-in's ValidateProductKey member is called, it is
  // passed a a product, or CD, key that was entered into the Zoo
  // administrator console. Your ValidateProductKey will validate
  // the product key and decode it into a product license. This is
  // where you can store this license. This value will be passed
  // to your application at runtime when it requests a license.
  ON_wString m_product_license;

  // This value will display in user interface items, such as in
  // the Zoo console and in About dialog boxes. Also, this value
  // is used to uniquely identify this license. Thus, it is
  // critical that this value be unique per product key, entered
  // by the administrator. No other license of this product, as
  // validated by this plug-in, should return this value.
  ON_wString m_serial_number;

  // This value will display in user interface items, such as in
  // the Zoo console and in About dialog boxes.
  // (e.g. "Rhinoceros 5.0", "Rhinoceros 5.0 Commercial", etc.)
  ON_wString m_license_title;

  // The build of the product that this license work with.
  // When your product requests a license from Rhino, it
  // will specify one of these build types.
  product_build_type m_build_type;

  // Licenses can be used by more than one instance of any application.
  // For example, a single Rhino Education Lab license can be used by up
  // to 30 systems simultaneously. If your license supports multiple instance,
  // then specify the number of supported instances here. Otherwise just
  // specify a value of 1 for single instance use.
  int m_license_count;

  // Licenses can be made to expire. If your licensing scheme
  // is sophisticated enough to support this, then specify the
  // expiration date here. Note, this value must be specified in
  // Coordinated Universal Time (UTC). If your license does not expire,
  // then just this value to null.
  DATE m_date_to_expire;

  // This icon will displayed in the "Licenses" page in the Options dialog.
  // Note, Rhino will make a copy of this icon..
  HICON m_product_icon;

  // The lease set by the Cloud Zoo. Data on this lease may change
  // at any time from a different thread. OnLeaseChanged() will be called
  // when this data changes. If OnLeaseChanged() is called and m_lease == nullptr,
  // then your product no longer has a valid lease from the CloudZoo. Disable 
  // your product's functionality in a reasonable way.
  CRhinoLicenseLease* m_lease = nullptr;

  // Set to true if the licenseKey passed to VerifyLicenseKey requires online validation. 
  bool m_requires_online_validation = false;

  // Set to true if the licenseKey passed to VerifyLicenseKey requires verification.
  // A separate call will be made, if necessary, to VerifyPreviousVersionLicense will be
  // made with the previous version license.
  bool m_requires_previous_version_license_verification = false;

  // If VerifyLicenseKey returns error_show_message and m_error_message is set, 
  // Rhino will show this error message. Otherwise, it will show a generic error message.
  ON_wString m_error_message;

private:
  // No copy constructor
  CRhinoLicenseValidator(const CRhinoLicenseValidator&);
  // No operator=
  CRhinoLicenseValidator& operator=(const CRhinoLicenseValidator&);

private:
  friend class CRhPlugInLicenseManager;
  ON_UUID m_plugin_id;
  ON__UINT_PTR m_sdk_reserved = 0;
};
