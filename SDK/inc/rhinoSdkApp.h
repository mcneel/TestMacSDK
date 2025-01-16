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

#include "rhinoSdkAppSettings.h"
#include "rhinoSdkCommand.h"
#include "rhinoSdkFileMenu.h"
#include "rhinoSdkCommandOption.h"
#include "rhinoSdkPlugIn.h"
#include "RhinoSdkPlugInManager.h"
#include "RhinoSdkDirectoryManager.h"


/////////////////////////////////////////////////////////////////////////////
// All the fonts necessary to display text in the command prompt,
// command history window, notes, dialog boxes, etc.
#if defined (RHINO_SDK_MFC)
class RHINO_SDK_CLASS CRhinoAppFonts
{
public:
  CRhinoAppFonts() = default;
  // Description:
  //   Sets m_NormalFont, m_ItalicFont, m_BoldFont, and m_BoldItalicFont to 
  //   the same font and size with one function call. Also handles updating
  //   all the appropriate windows in Rhino to use these new fonts.
  void SetCommandWindowFonts ( const wchar_t* fontFaceName, int pointSize );

  CFont  m_NormalFont;
  CFont  m_ItalicFont;
  CFont  m_BoldFont;
  CFont  m_BoldItalicFont;
private:
  ON_wString m_current_facename;
  int m_current_pointsize = 0;
};
#endif

class RHINO_SDK_CLASS CRhinoGripsEnabler
{
  // See CRhinoApp::RegisterCustomGrips()
public:
  CRhinoGripsEnabler();
  virtual ~CRhinoGripsEnabler();
  // If you add custom grips to Rhino and you want those grips
  // to stay on when CRhinoDoc::Replace object is called
  // (for example, somebody translates the object),
  // then you have to register your grips with Rhino.
  virtual
  void TurnOnGrips(CRhinoObject* object) const = 0;

  static 
  int CompareGripsId( CRhinoGripsEnabler* const *, CRhinoGripsEnabler* const * );

  const ON_UUID CustomGripsId() const;

  // This is the id is used to distinguish between different types of
  // custom grips.  Rhino default grips (the ones you get with the
  // PointsOn command) have the id = nil.  Custom grips must
  // have a non-nil id.
  ON_UUID m_grips_id;
};

/*
Parameters:
  runtime_serial_number - [in]    
Returns:
  The type portion of a Rhino runtime serial number.
Remarks:
  Rhino runtime serial numbers on docs, views, and so on, are created
  by setting serial number = (type | ordinal).  The type identifies
  the type (doc, view, ...) and the ordinal identifies which object
  of that type (1,2,...).
  The functions RhinoSerialNumberType() and RhinoSerialNumberOrdinal()
  are used to decompose a runtime serial number into a type and ordinal,
  typically for printing debugging messages.
Example:
  unsigned int rhino_doc_sn = doc->RuntimeSerialNumber();
  RhinoApp().Print("Doc sn type = %u, ordinal = %d\n",
                   RhinoSerialNumberType(rhino_doc_sn),
                   RhinoSerialNumberOrdinal(rhino_doc_sn)
                   );
*/
unsigned int RhinoSerialNumberType(
  unsigned int runtime_serial_number
  );


/*
Parameters:
  runtime_serial_number - [in]    
Returns:
  The ordinal portion of a Rhino runtime serial number.
Remarks:
  Rhino runtime serial numbers on docs, views, and so on, are created
  by setting serial number = (type | ordinal).  The type identifies
  the type (doc, view, ...) and the ordinal identifies which object
  of that type (1,2,...).
  The functions RhinoSerialNumberType() and RhinoSerialNumberOrdinal()
  are used to decompose a runtime serial number into a type and ordinal,
  typically for printing debugging messages.
Example:
  unsigned int rhino_doc_sn = doc->RuntimeSerialNumber();
  RhinoApp().Print("Doc sn type = %u, ordinal = %d\n",
                   RhinoSerialNumberType(rhino_doc_sn),
                   RhinoSerialNumberOrdinal(rhino_doc_sn)
                   );
*/
unsigned int RhinoSerialNumberOrdinal(
  unsigned int runtime_serial_number
  );



class RHINO_SDK_CLASS CRhinoScriptContext
{
public:
  CRhinoScriptContext() = default;
  ~CRhinoScriptContext() = default;
  CRhinoScriptContext(const CRhinoScriptContext&) = default;
  CRhinoScriptContext& operator=(const CRhinoScriptContext&) = default;

  static const CRhinoScriptContext EmptyScript;

  enum class ScriptEchoMode : unsigned int
  {
    Unset = 0,
    Literal = 1,
    Silent = 2
  };

  bool SetDocRuntimeSerialNumber(
    const class CRhinoDoc& doc
  );

  bool SetDocRuntimeSerialNumber(
    const class CRhinoDoc* doc
  );

  bool SetDocRuntimeSerialNumber(
    unsigned int rhino_doc_sn
  );

  unsigned int DocRuntimeSerialNumber() const;
  class CRhinoDoc* Doc() const;

  CRhinoScriptContext::ScriptEchoMode EchoMode() const;
  void SetEchoMode(
    CRhinoScriptContext::ScriptEchoMode echo_mode
  );

  void SetScript(
    const wchar_t* script
  );

  void SetScript(
    const class ON_wString& script
  );

  /*
  Parameters:
    persistent_script - [in]
      A string that will exist for the duration of the application.     
  Example:
    SetPersistentScript( L"! _Circle 0,0,0w, 1meter" );
  */
  void SetPersistentScript(
    const wchar_t* persistent_script
  );

  const ON_wString Script() const;

  const wchar_t* ScriptPointer() const;

  void SetMRUDisplayString(
    const wchar_t* mru_display_string
  );

  const ON_wString MRUDisplayString() const;

  const wchar_t* MRUDisplayStringPointer() const;

private:
  mutable unsigned int m_rhino_doc_sn = 0;
  CRhinoScriptContext::ScriptEchoMode m_echo_mode = CRhinoScriptContext::ScriptEchoMode::Unset;
  const wchar_t* m_script = nullptr;
  ON_wString m_managed_script;
  ON_wString m_mru_display_string;
  ON__UINT_PTR m_reserved = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CRhinoApp:
class CRhinoApp
{
public:
  /*
  CRhinoApp::APP_STATE enum values mark points in the
  sequence of the CRhinoApp being 
    constructed, 
    initializing, 
    closing, 
    exiting, 
    and destructed.
  It is critical that the enum values be listed in chronological order.
  */
  enum APP_STATE : unsigned int
  {
    // app_state_ctor_* - CRhinoApp::CRhinoApp() {} status
    app_state_ctor_zero = 0,
    app_state_ctor_begin,    // entered scope
    app_state_ctor_end,      // last line of scope
    app_state_ctor_exited,   // exited scope

    // app_state_InitInstance_* - CRhinoApp::InitInstance() {} status
    app_state_InitInstance_begin,   // entered scope
    app_state_InitInstance_before_WinAppInit, // before call to CWinApp::InitInstance()
    app_state_InitInstance_after_WinAppInit,  // entering CRhinoApp::InitInstance() scope
    app_state_InitInstance_end,     // last line of scope
    app_state_InitInstance_exited,    // exited scope

    ///////////////////////////////////////////////////////////////////
    //
    // If m_rhino_app_state >= app_state_InitInstance_exited
    // and m_rhino_app_state < app_state_OnClose_begin, then
    // it means CRhinoApp::InitInstance() is finished and
    // no WM_CLOSE message has been received by the main frame.
    // This generally means Rhino is up and running.
    // If more detailed app status information is needed, we
    // can consider adding it. But keep in mind, this is Rhino
    // app state and an app can have many docs and views
    // in many states while it is running. This is not an appropriate
    // place to report doc or view status because as soon as there
    // are multiple docs or multiple views, there can be more
    // than one answer to a doc or view question.
    //
    ///////////////////////////////////////////////////////////////////

    // app_state_OnClose_* - CMainFrame::OnClose() {} status
    app_state_OnClose_begin,           // entered scope
    app_state_OnClose_before_MDIClose, // before call to CMDIFrameWnd::OnClose()
    app_state_OnClose_UnsavedCancel,   // CRhinoApp::SaveAllModified() - cancel requested
    app_state_OnClose_after_MDIClose,  // ordinary close - after call to CMDIFrameWnd::OnClose()
    app_state_OnClose_end,             // ordinary close - last line of scope
    app_state_OnClose_Canceled,        // canceled close - cleanup in progress
    app_state_OnClose_exited,            // exited scope

    // app_state_ExitInstance_* - CRhinoApp::InitInstance() {} status
    app_state_ExitInstance_begin,   // entered scope
    app_state_ExitInstance_before_WinAppExit, // before call to CWinApp::InitInstance()
    app_state_ExitInstance_after_WinAppExit,  // entering CRhinoApp::InitInstance() scope
    app_state_ExitInstance_end,           // last line of scope
    app_state_ExitInstance_exited,          // exited scope

    app_state_dtor_begin,   // entered scope
    app_state_dtor_end,     // last line of scope
    app_state_dtor_exited   // exited scope
  };

private:
  // m_rhino_app_state must be the first member of CRhinoApp.
  CRhinoApp::APP_STATE m_rhino_app_state = CRhinoApp::APP_STATE::app_state_ctor_zero;

#if defined (ON_RUNTIME_APPLE)
protected:
  CRhinoApp::APP_STATE& MacRhinoAppState();
#endif

public:

  RHINO_SDK_FUNCTION
  CRhinoApp::APP_STATE AppState() const;

  /*
  Returns 
    true 
      - The single instance of the CRhinoApp class is being constructed.
      - The code in CRhinoApp::CRhinoApp() is running.
      - CRhinoApp.m_app_status <= CRhinoApp::APP_STATUS::app_status_ctor_end
  */
  RHINO_SDK_FUNCTION
  bool InConstructor() const;

  /*
  Returns 
    true 
      The code in CRhinoApp::InitInstance() is running.
      CRhinoApp.m_app_status >= CRhinoApp::APP_STATUS::app_status_InitInstance_begin
      CRhinoApp.m_app_status <= CRhinoApp::APP_STATUS::app_status_InitInstance_end
  */
  RHINO_SDK_FUNCTION
  bool InInitInstance() const;

  /*
  Returns 
    true
      - The code in CMainFrame::OnClose() is running.
      - CRhinoApp.m_app_status <= CRhinoApp::APP_STATUS::app_status_OnClose_begin
      - CRhinoApp.m_app_status < CRhinoApp::APP_STATUS::app_status_OnClose_exited
  */
  RHINO_SDK_FUNCTION
  bool InMainFrameOnClose() const;

  /*
  Returns 
    true 
      - The code in CRhinoApp::ExitInstance() is running.
      - CRhinoApp.m_app_status >= CRhinoApp::APP_STATUS::app_status_ExitInstance_begin
      - CRhinoApp.m_app_status <= CRhinoApp::APP_STATUS::app_status_ExitInstance_end
  */
  RHINO_SDK_FUNCTION
  bool InExitInstance() const;

  /*
  Returns 
      - The code in CRhinoApp::~CRhinoApp() is running.
      - CRhinoApp.m_app_status >= CRhinoApp::APP_STATUS::app_status_dtor_begin
  */
  RHINO_SDK_FUNCTION
  bool InDestructor() const;

  /*
  Returns 
    true 
      - We have not completed CRhinoApp::InInitInstance().
      - This means the state is someplace between being constructed
        and being initialized.
      - CRhinoApp.m_app_status <= CRhinoApp::APP_STATUS::app_status_ExitInstance_end
  */
  RHINO_SDK_FUNCTION
  bool IsInitializing() const;

  /*
  Returns 
    True if Rhino is processing a close application message or later. 
    Closing can be canceled.  
    - CRhinoApp.m_app_status >= CRhinoApp::APP_STATUS::app_status_OnClose_begin
  See Also:
    CRhinoApp::InMainFrameOnClose()
  */
  RHINO_SDK_FUNCTION
  bool IsClosing() const;
  
  /*
  Returns 
    true 
      - we have entered CRhinoApp::ExitInstance() or later.
      - CRhinoApp.m_app_status >= CRhinoApp::APP_STATUS::app_status_ExitInstance_begin
  */
  RHINO_SDK_FUNCTION
  bool IsExiting() const;

  /*
  Description:
    Base version of the Rhino SDK supported by this Rhino executable. 
    Rhino will only load plug-ins that were build with exactly the
    same version of the SDK.
  Returns:
    Rhino SDK 9 digit SDK version number in the form YYYYMMDDn
  See Also:
    CRhinoApp::SdkServiceRelease 
    CRhinoApp::ExeVersion 
    CRhinoApp::ExeServiceRelease
  */
  RHINO_SDK_FUNCTION
  unsigned int SdkVersion() const;


  /*
  Description:
    Service release of the Rhino SDK supported by this 
    executable. Rhino will only load plug-ins that require a 
    service release of <= this release number.

    For example, SR1 will load all plug-ins made with any SDK released
    up through and including the SR1 SDK.  But, SR1 will not load a 
    plug-in built using the SR2 SDK.  If an "old" Rhino tries to load
    a "new" plug-in, the user is told that they have to get a free
    Rhino.exe update in order for the plug-in to load.  Rhino.exe
    updates are available from http://www.rhino3d.com.
  Returns:
    Rhino SDK 9 digit SDK service release number in the form YYYYMMDDn
  See Also:
    CRhinoApp::SdkVersion 
    CRhinoApp::ExeVersion 
    CRhinoApp::ExeServiceRelease
  */
  RHINO_SDK_FUNCTION
  unsigned int SdkServiceRelease() const;

  /*
  Returns:
    Major version of Rhino executable 4, 5, 6, ...
  See Also:
    CRhinoApp::SdkVersion 
    CRhinoApp::SdkServiceRelease 
    CRhinoApp::ExeServiceRelease
  */
  RHINO_SDK_FUNCTION
  int ExeVersion() const;

  /*
  Returns:
    Service release version of Rhino executable (0, 1, 2, ...)  
    The integer is the service release number of Rhino.  For example,
    this function returns "0" if Rhino V6SR0 is running and returns
    "1" if Rhino V6SR1 is running.
  See Also:
    CRhinoApp::SdkVersion 
    CRhinoApp::SdkServiceRelease 
    CRhinoApp::ExeVersion
  */
  RHINO_SDK_FUNCTION
  int ExeServiceRelease() const;

  /*
  Returns:
    The product serial number, as seen in Rhino's ABOUT dialog box.  
  See Also:
    CRhinoApp::NodeType 
    CRhinoApp::InstallationType 
  */
  RHINO_SDK_FUNCTION
  ON_wString SerialNumber();

  /*
  Returns:
    The product node type, as seen in Rhino's ABOUT dialog box.  
  See Also:
    CRhinoApp::SerialNumber 
    CRhinoApp::InstallationType 
  */
  enum node_type
  {
    node_stand_alone = 0,     // Standalone
    node_network,             // Network (obtains license from Zoo server)
    node_network_checked_out, // Network (has license checked out from Zoo server)
		node_type_force_32_bit = 0xFFFFFFFF,
  };

  RHINO_SDK_FUNCTION
  node_type NodeType();

  /*
  Returns:
    The product installation type, as seen in Rhino's ABOUT dialog box.  
  See Also:
    CRhinoApp::NodeType 
    CRhinoApp::InstallationType 
  */
  enum installation_type
  {
    installation_undefined = 0,
    installation_commercial,
    installation_educational,
    installation_educational_lab,
    installation_not_for_resale,
    installation_not_for_resale_lab,
    installation_beta,
    installation_beta_lab,
    installation_evaluation,
    installation_corporate,
    installation_evaluation_timed,
		installation_force_32_bit = 0xFFFFFFFF,
  };

  RHINO_SDK_FUNCTION
  installation_type InstallationType();

  RHINO_SDK_FUNCTION
  void GetApplicationName( ON_wString& app_name ) const;

  RHINO_SDK_FUNCTION
  void GetApplicationName( ON_3dmApplication& app_name ) const;


  RHINO_SDK_FUNCTION
  void GetSystemInformation(ON_TextLog& log) const;

  ON_DEPRECATED_MSG("use GetRhinoRootRegistryPath, GetRhinoPlugInsRegistryPath, GetRhinoSchemeRegistryPath, or GetRhinoGlobalOptionsRegistryPath")
  RHINO_SDK_FUNCTION BOOL32 GetRhinoRegistryKey( ON_wString& result) const;

  /*
  Parameters:
    result - [out]
      "Software\McNeel\Rhinoceros\<version>"
  Remarks:
    In shipping builds <version> is something like "5.0". In developer
    builds <version> is something like 5.0-Developer-Debug and
    5.0-Developer-Release.  The reason for the difference
    is so developer builds do not damage the shipping product registry
    and so plug-ins used by developer debug and release have clear
    and separate registry areas.
    There are very few cases when a plug-in developer should use
    GetRhinoRootRegistryPath().   In general, you should use
    GetRhinoPlugInsRegistryPath(), GetRhinoSchemeRegistryPath()
    and GetRhinoGlobalOptionsRegistryPath().
  */
  RHINO_SDK_FUNCTION
  bool GetRhinoRootRegistryPath( ON_wString& result) const;

  /*
  Description:
    Get the plug-ins registry key path.
  Parameters:
    bFullPath - [in]
      If true, the result begins with "Software\McNeel\Rhinoceros\<version>\.
    result - [out]
      In release builds:
        If bFullPath is true, then result is "Software\McNeel\Rhinoceros\<version>\Plug-Ins".
        If bFullPath is false, then result is "Plug-Ins".
      In debug builds:
        If bFullPath is true, then result is "Software\McNeel\Rhinoceros\<version>\Plug-Ins_DebugBuild".
        If bFullPath is false, then result is "Plug-Ins_DebugBuild".
  Remarks:
    The reason release and debug builds use different locations is because
    release Rhino will not load debug plug-ins and vice versa.  Using
    different locations for debug and release builds means developers do
    not have to constantly re-register plug-ins when they switch between 
    running debug and release versions.
  */
  RHINO_SDK_FUNCTION
  bool GetRhinoPlugInsRegistryPath( bool bFullPath, ON_wString& result) const;

  /*
  Description:
    Get the global options registry key path.
  Parameters:
    bFullPath - [in]
      If true, the result begins with "Software\McNeel\Rhinoceros\<version>\.
    result - [out]
      If bFullPath is true, then result is "Software\McNeel\Rhinoceros\<version>\Scheme: <name>".
      If bFullPath is false, then result is "Scheme: <name>".
  */
  RHINO_SDK_FUNCTION
  bool GetRhinoSchemeRegistryPath( bool bFullPath, ON_wString& result) const;

  /*
  Description:
    Get the global options registry key path.
  Parameters:
    bFullPath - [in]
      If true, the result begins with "Software\McNeel\Rhinoceros\<version>\.
    result - [out]
      If bFullPath is true, then result is "Software\McNeel\Rhinoceros\<version>\Global Options".
      If bFullPath is false, then result is "Global Options".
  */
  RHINO_SDK_FUNCTION
  bool GetRhinoGlobalOptionsRegistryPath( bool bFullPath, ON_wString& result) const;


  RHINO_SDK_FUNCTION
  ON_UUID Rhino2_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino3_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino4_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino5_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino6_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino7_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino8_UUID() const;

  RHINO_SDK_FUNCTION
  ON_UUID Rhino9_UUID() const;

  // RhinoCurrent_UUID() returns the id of "this" version
  // of Rhino.  For V5 it is the same as Rhino5_UUID()
  // For V6 it will be the new V6 uuid.
  RHINO_SDK_FUNCTION
  ON_UUID RhinoCurrent_UUID() const;

  RHINO_SDK_FUNCTION
  bool IsRhinoUUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino2UUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino3UUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino4UUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino5UUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino6UUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino7UUID(const ON_UUID) const;

  RHINO_SDK_FUNCTION
  bool IsRhino8UUID(const ON_UUID) const;

  /*
  Description:
    Returns true if Rhino was started as a standalone executable application (Rhino.exe).
    Returns false if Rhino was started by another application or process (Rhino.Inside).
  */
  RHINO_SDK_FUNCTION
  bool StartedAsRhinoExe() const;

  //////////
  // All application settings are kept in CRhinoAppSettings.
  // The settings are loaded in CRhinoApp::InitInstance() and saved
  // in CRhinoApp::ExitInstance().
  RHINO_SDK_FUNCTION
  CRhinoAppSettings& AppSettings();

  ////////
  // main application memory pool tools
  RHINO_SDK_FUNCTION
  void* Malloc( size_t );
  
  RHINO_SDK_FUNCTION
  void* Calloc( size_t, size_t );
  
  RHINO_SDK_FUNCTION
  void  Free( void* );
  
  RHINO_SDK_FUNCTION
  void* Realloc( void*, size_t );

  RHINO_SDK_FUNCTION
  void* Memdup( const void*, size_t );

  /*
  Description:
    In general, you should use CRhinoApp::RunScript to run commands
    unless you are trying to modify the Rhino model from a modeless
    dialog.  See the example below for how to handle the modeless
    dialog situation.
    
    This is a low level tool that searches for a command (a instance
    of a class  derived from CRhinoCommand) with a matching command
    name, if it finds a command, runs that command by calling the 
    virtual CRhinoCommand::RunCommand() member function.
    
    The command name can be prefaced with a combination of 
    underscore (_), hyphen (-), or right single quote (').  If an
    underscore is present, then the command name search looks for
    a matching English command name.  If the underscore is not present,
    the command name search looks for a matching localized command
    name.  If the hyphen is present, then the command is run in script
    mode; otherwise the command is run in interactive mode.  The right
    single quote is ignored by ExecuteCommand.  It is used in scripts
    to indicate that the text token must be treated as a command name
    and is important to upstream text token parsing.  

  Example:   

            CMyCommand : pubic CRhinoCommand
            {
            public:
              RunCommand(...);

              type1 m_my_info1;
              type2 m_my_info2;
              bool m_bOkToRunCommand;
            };

            CMyCommand::CMyCommand()
            {
              m_bOkToRunCommand = false;
            }

            CMyCommand theMyCommand;

            CMyCommand::RunCommand(...)
            {
              if (!m_bOkToRunCommand)
              {
                RhinoMessageBox(L"Use the MyDialog button to run this command");
                return CRhinoCommand::failure;
              }

             // the dialog has filled in m_my_info1 and m_my_info2.
             // Use this as the "input" to the command and then
             // do whatever you need to as if the command were
             // an ordinary command.

             ...

             return CRhinoCommand::success;
            }

            ...

            CMyDialog::OnButtonDown(...)
            {
              // fill in the parameters
              theMyCommand.m_my_info1 = ...;
              theMyCommand.m_my_info2 = ...;
              theMyCommand.m_bOkToRunCommand = true;
              RhinoApp().ExectueCommand(theMyCommand.LocalCommandName());       
              theMyCommand.m_bOkToRunCommand = false;
            }

  Returns:
    Command return code indicating if the command worked, failed,
    or was canceled by the user.
  */
  RHINO_SDK_FUNCTION
  CRhinoCommand::result ExecuteCommand(
    unsigned int rhino_doc_sn,
    const wchar_t* cmd_name 
    );

  /*
  Description:
    Every time Rhino does something critical, like running a command,
    Rhino calls CRhinoApp::IncrementRhinoAppTicker() so the code that
    detects when Rhino is "idle" can do a good job of knowing how long
    Rhino has been idle.
  */
  RHINO_SDK_FUNCTION
  void IncrementRhinoAppTicker();

  MAC_DEPRECATED
  RHINO_SDK_FUNCTION
  void RepeatCommand(); // repeats previous command

  /*
  Description:
    This is a low level tool to determine if Rhino is currently running
    a command.  
  Parameters:
    bIgnoreScriptRunnerCommands - [in] if true, script running commands 
        like "ReadCommandFile" and the RhinoScript plug-ins "RunScript"
        are not counted.
  Returns:
    Number of active commands.
  */
  RHINO_SDK_FUNCTION
  int InCommand( bool bIgnoreScriptRunnerCommands = false ) const;

  /*
  Description:
    This is a low level tool to determine if Rhino is currently running
    a script running command like "ReadCommandFile" or the RhinoScript
    plug-in's "RunScript".
  Returns:
    True if a script running command is active.
  */
  RHINO_SDK_FUNCTION
  bool InScriptRunnerCommand() const;

  /*
  Description:
    Run a Rhino command script.
  Parameters:
    rhino_doc_sn - [in]
      0: document unknown - used application default
      >0: document to begin script in (a script can create and destroy many docs)
    script - [in] script to run
    echo_mode - [in] Controls how the script is echoed in the command output window.
       0 = silent - nothing is echoed
       1 = verbatim - the script is echoed literally
  Remarks:
    Rhino acts as if each character in the script string had
    been typed in the command prompt.  When RunScript is called from a 
    "script runner" command, it completely runs the script before returning.
    When RunScript is called outside of a command, it returns and the
    script is run.  This way menus and buttons can use RunScript to
    execute complicated functions.
  */
  RHINO_SDK_FUNCTION
  bool RunScript(
    unsigned int rhino_doc_sn,
    const wchar_t* script,
    int echo_mode = 1
    );

  /*
  Description:
    Run a Rhino command script.
  Parameters:
    rhino_doc_sn - [in]
      0: document unknown - used application default
      >0: document to begin script in (a script can create and destroy many docs)
    script - [in] script to run
    mruDisplayString - [in] If this is a valid string then the macro is added to the MRU command
       list using this string, if it is NULL or an empty string then false is returned.
    echo_mode - [in] Controls how the script is echoed in the command output window.
       0 = silent - nothing is echoed
       1 = verbatim - the script is echoed literally
  Remarks:
    Rhino acts as if each character in the script string had
    been typed in the command prompt.  When RunScript is called from a 
    "script runner" command, it completely runs the script before returning.
    When RunScript is called outside of a command, it returns and the
    script is run.  This way menus and buttons can use RunScript to
    execute complicated functions.
  */
  RHINO_SDK_FUNCTION
  bool RunScriptEx( 
    unsigned int rhino_doc_sn,
    const wchar_t* script,
    const wchar_t* mruDisplayString, 
    int echo_mode = 1
    );

  // Description:
  //   Run a Rhino menu item script.  Will add the selected menu string to the MRU command menu.
  // Parameters:
  //   script - [in] script to run
  // Remarks:
  //   Rhino acts as if each character in the script string had
  //   been typed in the command prompt.
  RHINO_SDK_FUNCTION
  bool RunMenuScript( 
    const wchar_t* script
  );

  RHINO_SDK_FUNCTION
  bool RunMenuScript( 
    unsigned int rhino_doc_sn,
    const wchar_t* script
  );

  RHINO_SDK_FUNCTION
  bool RunScript( 
    const CRhinoScriptContext& script_context 
  );


  /*
  Returns:
    True if echo is enabled.  Scripts that begin with
    "noecho" or "_noecho" turn off echo.
  */
  RHINO_SDK_FUNCTION
  bool Echo() const;

  /*
  Returns:
    True if a command is running and the user has
    pressed the escape key to cancel a command.
  */
  RHINO_SDK_FUNCTION
  bool CommandCanceled() const;

  /*
  Description:
    Find the document with this serial number.
  Returns:
    A pointer to the document if it exists.
    Null if no living document has this serial number.
  Remarks:
    Use CRhinoDoc::FromRuntimeSerialNumber().
  */
  ON_DEPRECATED_MSG("use CRhinoDoc::FromRuntimeSerialNumber()")
  RHINO_SDK_FUNCTION
  CRhinoDoc* GetDocument(UINT_PTR doc_sn) const;


  // The CRhinoApp::ActiveDoc() function will be deleted 
  // as soon as we finish removing what calls we can and
  // replacing the hard cases with calls to CRhinoApp::ObsoleteActiveDoc().
  //
  // In the rare cases, like the layer control on the Rhino
  // app status bar, where modeless user interface needs
  // a document, use RhinoApp().ModelessUserInterfaceDocRuntimeSerialNumber();
  // 
  // In the more common case where you don't have the time 
  // to elegantly replace a call to CRhinoApp::ActiveDoc(),
  // put in a call to CRhinoApp::ObsoleteActiveDoc() and we will
  // deal with it later.
  //
  // Instead use
  //   1) The command or object's doc value
  //   2) CRhinoDoc::ModelessUserInterfaceRuntimeSerialNumber()
  //      for the document modeless ui, like the layer control on
  //      the app status bar, should use.
  //ON_DEPRECATED
  RHINO_SDK_FUNCTION
  CRhinoDoc* ActiveDoc() const;

  /*
  As we work on V6 and developers find "ActiveDoc()" calls
  that cannot be easily removed or reasonably replaced
  with a call to RhinoApp().ModelessUserInterfaceDocRuntimeSerialNumber(),
  that call to ActiveDoc() will be replaced with ObsoleteActiveDoc().
  This makes it easy for people who are going through Rhino
  and removing ActiveDoc() to ignore calls to this function
  that other developers have already worked on.
  */
  //ON_DEPRECATED
  RHINO_SDK_FUNCTION
  CRhinoDoc* ObsoleteActiveDoc() const;
  

  // Description:  
  //   Returns the active view.
  // Returns:
  //   Pointer to active view or NULL if no views have been created.
  // Remarks:
  //   While models are being loaded, this function will return null.
  //   Code that may be run during file
  //   reading needs to test for a NULL pointer.
  RHINO_SDK_FUNCTION
  class CRhinoView* ActiveView() const;

  // Active view (can be NULL if a file is being read and no views have 
  // been created)
  RHINO_SDK_FUNCTION
  void SetActiveView(CRhinoView*) const;

  // Description:
  //   Set the active viewport by id
  //   (may be the viewport id of a detail object on a page)
  // Note:
  //   This can be much slower than the SetActiveView() command. If
  //   you can get away with just calling SetActiveView(), do that.
  RHINO_SDK_FUNCTION
  void SetActiveViewport( ON_UUID viewport_id );

  // Use CRhinoView::FromMainViewportId() to find CRhinoView
  // from its main id or use CRhinoViewport::FromViewportId()
  // to find a CRhinoViewport (model, page, or detail)
  // ON_DEPRECATED
  RHINO_SDK_FUNCTION
  CRhinoView* LookupView( ON_UUID main_viewport_id ) const;

  // Description:
  //   Returns the current working space.
  RHINO_SDK_FUNCTION
  ON::active_space ActiveSpace() const;

  // Description:
  //   Get Rhino main window.
  // Returns:
  //   Rhino application's AfxGetMainWnd().
  // See Also: 
  //   CRhinoApp::MainFrame
  RHINO_SDK_FUNCTION
  HWND MainWnd() const;

  // Description:
  //   Get Rhino HINSTANCE
  // Returns:
  //   Rhino application's HINSTANCE
  RHINO_SDK_FUNCTION
  HINSTANCE RhinoInstanceHandle() const;

  // Description:
  //   The parent of a top-level window is the desktop window.
  //   Some applications like to  re-parent Rhino's main window to a window they provide.
  //   Use this method to determine if Rhino is in such a condition. 
  //   Note, method is only valid on Windows.
  // Returns:
  //   True if Rhino's parent window is the desktop, false otherwise.
  RHINO_SDK_FUNCTION
  bool IsParentWindowDesktop() const;

  // Description:
  //   Get Rhino's installation folder
  // Parameters:
  //   result - [out] - Folder name placed in this object
  // Returns:
  //   TRUE if result updated otherwise FALSE.
  RHINO_SDK_FUNCTION
  bool InstallFolder( ON_wString &result);

  // Description:
  //   Get Rhino's executable folder
  // Parameters:
  //   result - [out] - Folder name placed in this object
  // Returns:
  //   TRUE if result updated otherwise FALSE.
  RHINO_SDK_FUNCTION
  bool ExecutableFolder( ON_wString &result) const;

  /////////////////////////////////////////////////////////////////////////////
  // Print formatted text in the command window

  // See ON_String::Format for a description of string formatting.
  RHINO_SDK_FUNCTION
  void ON_VARGS_FUNC_CDECL Print(const char* format, ...) const;
  
  // See ON_wString::Format for a description of string formatting.
  RHINO_SDK_FUNCTION
  void ON_VARGS_FUNC_CDECL Print(const wchar_t* format, ...) const;

  // Description:
  //   Returns true if command window print statements are being captured
  //   for retrieval later on.
  //   Default capture state is disabled
  RHINO_SDK_FUNCTION bool CaptureCommandWindowPrintEnabled() const;

  // Description:
  //   Enable or disable command window print statements.
  // Returns:
  //   Previous enabled state for command window print capture
  RHINO_SDK_FUNCTION bool CaptureCommandWindowPrint(bool enabled);


  // Description:
  //   Returns true if command window print statements are being sent to
  //   the console. Useful for headless mode
  RHINO_SDK_FUNCTION bool SendCommandWindowPrintToConsoleEnabled() const;

  // Description:
  //   Enable or disable console output for print statements
  // Returns:
  //   Previous enabled state for console printing
  RHINO_SDK_FUNCTION bool SendCommandWindowPrintToConsole(bool enabled);

  // Description:
  //  Get buffer of strings sent to RhinoApp().Print(...)
  //  This will only return a list of strings if CaptureCommandWindowPrintEnabled
  //  is true.
  // Parameters:
  //  strings: list of strings that have been sent to RhinoApp().Print(...)
  //  clearBuffer: set to true to clear the existing buffer of print strings
  RHINO_SDK_FUNCTION void CommandWindowCapturedStrings(ON_ClassArray<ON_wString>& strings, bool clearBuffer);

  /////////////////////////////////////////////////////////////////////////////
  // Plug-in support for startup /stopwatch command line option
  RHINO_SDK_FUNCTION
  void RecordInitInstanceTime( const wchar_t* ) const;

  // Description:
  //   Returns the text from Rhino's command history window
  // Parameters:
  //   wstr - [out] - The command history window text.
  RHINO_SDK_FUNCTION
  void GetCommandHistoryWindowText( ON_wString& wstr ) const;

  // Description:
  //   Clear the text from Rhino's command history window
  RHINO_SDK_FUNCTION
  void ClearCommandHistoryWindowText();

  /////////////////////////////////////////////////////////////////////////////
  // Setting command window prompt

  /*
  Description:
    Set the text that appears in the Rhino command prompt.
  Parameters:
    sMessage - [in] literal text for the command prompt window.
  Remarks:
    In general, you should use the CRhinoApp::SetCommandPrompt
    functions. The create a prompt that is correctly formatted.
    In rare cases, like worker thread messages, the message
    that appears in the prompt has non-standard formatting.
    In these rare cases, SetCommandPromptMessage can be used
    to literally specify the text that appears in the command
    prompt window.
  */
  RHINO_SDK_FUNCTION
  void SetCommandPromptMessage(
        const wchar_t* sMessage
        );
  /*
  Description:
    Set Rhino command prompt.
  Parameters:
    prompt - [in] prompt
    prompt_default - [in] text that appears in angle brackets
        and indicates what will happen if the user pressed ENTER.
  */
  RHINO_SDK_FUNCTION
  void SetCommandPrompt(
        const wchar_t* prompt,
        const wchar_t* prompt_default = nullptr
        );

  /*
  Description:
    Set Rhino command prompt.
  Parameters:
    prompt - [in] prompt
    prompt_default - [in] text that appears in angle brackets
        and indicates what will happen if the user pressed ENTER.
    options - [in] command options
  */
  RHINO_SDK_FUNCTION
  void SetCommandPrompt( 
        const wchar_t* prompt,
        const wchar_t* prompt_default,
        const ON_ClassArray<CRhinoCommandOption>& options
        );

  /*
  Description:
    Get Rhino command prompt string.
  Parameters:
    prompt - [out] Prompt display string.
  Returns:
    Returns true on success.
  */
  RHINO_SDK_FUNCTION
  bool GetCommandPrompt( ON_wString& prompt) const;

  /*
  Description:
    Parse command line arguments passed to Rhino.exe on
    startup.
  Parameters:
    wScript - [out] A string containing a space-delimited series
    of commands and command arguments that are run when Rhino starts.
    These commands are executed immediately after the commands specified
    by the user as "Startup" commands in the General tab of the Options
    dialog box.
  Returns:
    Returns true if wScript is not empty, indicating that there
    are commands to run.
    Returns false if wScript is empty, indicating that there are not
    commands to run.
  */
  RHINO_SDK_FUNCTION
  bool GetCommandLineScript( ON_wString& wScript);

  /*
  Description:
    Determines if Rhino is running as a server. If Rhino.exe is started
    with the command line switch "/server" then Rhino will run in server
    mode. This means that no UI or command Windows task bar icon are 
    displayed.

    It is assumed that the caller is running a script or plug-in that will
    close Rhino when finished. Rhino will not close itself.
  Returns:
    Returns true if Rhino is running invisibly as a server. Otherwise false.
  */
  RHINO_SDK_FUNCTION
  bool RunAsServer();

  /////////////////////////////////////////////////////////////////////////////
  // Setting command window prompt
  RHINO_SDK_FUNCTION
  void SetStatusBarDistancePane(double dist, bool bAutoFormatDistancePane = true);

  RHINO_SDK_FUNCTION
  void SetStatusBarNumberPane(double angle);

  RHINO_SDK_FUNCTION
  void SetStatusBarPointPane(const ON_3dPoint&, const ON_Plane* pPane = nullptr);

  RHINO_SDK_FUNCTION
  void SetStatusBarMessagePane( const wchar_t* ); // pass NULL to clear message pane

  /*
  Description:
    Starts, or shows, Rhino's status bar progress meter.
  Parameters:
    nLower       - [in] The lower limit of the progress meter's range.
    nUpper       - [in] The upper limit of the progress meter's range.
    pszLabel     - [in] The short description of the progress (e.g. "Calculating", "Meshing", etc).
    bEmbedLabel  - [in] If true, then the label will be embedded in the progress meter.
                        If false, then the label will appear to the left of the progress meter.
    bShowPercent - [in] If true, then the percent complete will appear in the progress meter.
  Returns:
     1 - The progress meter was created successfully.
     0 - The progress meter was not created.
    -1 - The progress meter was not created because some other process has already created it.
  See Also:
    CRhinoApp::StatusBarProgressMeterPos
    CRhinoApp::StatusBarProgressMeterEnd
  */
  ON_DEPRECATED_MSG("use CRhinoDoc::StatusBarProgressMeterStart")
  RHINO_SDK_FUNCTION
  int StatusBarProgressMeterStart( int nLower, int nUpper, const wchar_t* pszLabel = nullptr, bool bEmbedLabel = true, bool bShowPercent = true );

  /*
  Description:
    Sets the current position of Rhino's status bar progress meter.
  Parameters:
    nPos      - [in] The position.
    bAbsolute - [in] If true, then the progress meter is moved to nPos (absolute).
                     If false, then the progress meter is moved nPos from the current position (relative).
  Returns:
     The previous position if successful.
  See Also:
    CRhinoApp::StatusBarProgressMeterStart
    CRhinoApp::StatusBarProgressMeterEnd
  */
  ON_DEPRECATED_MSG("use CRhinoDoc::StatusBarProgressMeterPos")
  RHINO_SDK_FUNCTION
  int StatusBarProgressMeterPos( int nPos, bool bAbsolute = true );
  
  /*
  Description:
    Ends, or hides, Rhino's status bar progress meter.
  See Also:
    CRhinoApp::StatusBarProgressMeterStart
    CRhinoApp::StatusBarProgressMeterPos
  */
  ON_DEPRECATED_MSG("use CRhinoDoc::StatusBarProgressMeterEnd")
  RHINO_SDK_FUNCTION
  void StatusBarProgressMeterEnd();

  #define RHINO_READ_REG_MODE 0
  #define RHINO_WRITE_REG_MODE 1
  // Description:
  //   For expert use only. Use profile contexts for reading and writing 
  //   information that should persist between Rhino sessions. The profile
  //   contexts are platform and location independent.
  //    
  //   Appropriate profile contexts are:
  //       RhinoApp().m_pc_scheme_current_user
  //       RhinoApp().m_pc_global_current_user
  //       RhinoApp().m_pc_root_local_machine
  //       RhinoApp().m_pc_plugins_current_user
  //       RhinoApp().m_pc_plugins_local_machine
  //
  //   Get HKEY for \\HKEY_LOCAL_MACHINE\Software\McNeel\Rhinoceros\<Version>\
  // Parameters:
  //   nMode    - [in]
  //                RHINO_READ_REG_MODE = open key for reading only
  //                RHINO_WRITE_REG_MODE = open key for writing
  //   nProfile - [in] the desired profile type for this registry key. See CRhinoProfileContext::context_location enum for details.
  // Returns:
  //   HKEY on success, NULL on error
  // Caller must close returned HKEY with ::RegCloseKey()
  RHINO_SDK_FUNCTION
  HKEY RhinoGetRootRegistryKeyEx( int nMode, int nProfile = CRhinoProfileContext::cl_scheme_current_user);

  // Description:
  //   For expert use only. Use profile contexts for reading and writing 
  //   information that should persist between Rhino sessions. The profile
  //   contexts are platform and location independent.
  //    
  //   Appropriate profile contexts are:
  //       RhinoApp().m_pc_scheme_current_user
  //       RhinoApp().m_pc_global_current_user
  //       RhinoApp().m_pc_root_local_machine
  //       RhinoApp().m_pc_plugins_current_user
  //       RhinoApp().m_pc_plugins_local_machine
  //
  //   Get HKEY for \\HKEY_LOCAL_MACHINE\Software\McNeel\Rhinoceros\<Version>\Options
  // Parameters:
  //   nMode    - [in]
  //                RHINO_READ_REG_MODE = open key for reading only
  //                RHINO_WRITE_REG_MODE = open key for writing
  //   nProfile - [in] the desired profile type for this registry key. See CRhinoProfileContext::context_location enum for details.
  // Returns:
  //   HKEY on success, NULL on error
  // Caller must close returned HKEY with ::RegCloseKey()
  RHINO_SDK_FUNCTION
  HKEY RhinoGetGlobalOptionsRegistryKeyEx( int nMode, int nProfile = CRhinoProfileContext::cl_scheme_current_user);

  // Description:
  //   For expert use only. Use profile contexts for reading and writing 
  //   information that should persist between Rhino sessions. The profile
  //   contexts are platform and location independent.
  //    
  //   Appropriate profile contexts are:
  //       RhinoApp().m_pc_scheme_current_user
  //       RhinoApp().m_pc_global_current_user
  //       RhinoApp().m_pc_root_local_machine
  //       RhinoApp().m_pc_plugins_current_user
  //       RhinoApp().m_pc_plugins_local_machine
  //
  //   Get HKEY for \\HKEY_LOCAL_MACHINE\Software\McNeel\Rhinoceros\<Version>\<BuildDate>\<lpsSectionName>
  // Parameters:
  //   lpszSection - [in] - sub key to create or open
  //   nProfile - [in] the desired profile type for this registry key. See CRhinoProfileContext::context_location enum for details.
  // Returns:
  //   HKEY on success, NULL on error
  // Caller must close returned HKEY with ::RegCloseKey()
  RHINO_SDK_FUNCTION
	HKEY RhinoGetSectionKey( LPCTSTR lpszSection, int nProfile = CRhinoProfileContext::cl_scheme_current_user);
  
  // Description:
  //   For expert use only. Use profile contexts for reading and writing 
  //   information that should persist between Rhino sessions. The profile
  //   contexts are platform and location independent.
  //    
  //   Appropriate profile contexts are:
  //       RhinoApp().m_pc_scheme_current_user
  //       RhinoApp().m_pc_global_current_user
  //       RhinoApp().m_pc_root_local_machine
  //       RhinoApp().m_pc_plugins_current_user
  //       RhinoApp().m_pc_plugins_local_machine
  //
  //   Get HKEY for \\HKEY_LOCAL_MACHINE\Software\McNeel\Rhinoceros\<Version>\<BuildDate>\<lpsSectionName>
  // Parameters:
  //   lpszSection - [in] - sub key to create or open
  //   nMode    - [in]
  //                RHINO_READ_REG_MODE = open key for reading only
  //                RHINO_WRITE_REG_MODE = open key for writing
  //   nProfile - [in] the desired profile type for this registry key. See CRhinoProfileContext::context_location enum for details.
  // Returns:
  //   HKEY on success, NULL on error
  // Caller must close returned HKEY with ::RegCloseKey()
  RHINO_SDK_FUNCTION
	HKEY RhinoGetSectionKeyEx( LPCTSTR lpszSection, int nMode, int nProfile = CRhinoProfileContext::cl_scheme_current_user);

  // Description:
  //   For expert use only. Use profile contexts for reading and writing 
  //   information that should persist between Rhino sessions. The profile
  //   contexts are platform and location independent.
  //    
  //   Appropriate profile contexts are:
  //       RhinoApp().m_pc_scheme_current_user
  //       RhinoApp().m_pc_global_current_user
  //       RhinoApp().m_pc_root_local_machine
  //       RhinoApp().m_pc_plugins_current_user
  //       RhinoApp().m_pc_plugins_local_machine
  //
  //   Get HKEY for \\HKEY_LOCAL_MACHINE\Software\McNeel\Rhinoceros\<Version>\<BuildDate>
  // Parameters:
  //   nMode    - [in]
  //                RHINO_READ_REG_MODE = open key for reading only
  //                RHINO_WRITE_REG_MODE = open key for writing
  //   nProfile - [in] the desired profile type for this registry key. See CRhinoProfileContext::context_location enum for details.
  // Returns:
  //   HKEY on success, NULL on error
  // Caller must close returned HKEY with ::RegCloseKey()
  RHINO_SDK_FUNCTION
	HKEY RhinoGetAppRegistryKeyEx( int nMode, int nProfile = CRhinoProfileContext::cl_scheme_current_user);

#if defined (ON_RUNTIME_WIN)
  HKEY RhinoGetAppRegistryKeyEx(int version, int nMode, CRhinoProfileContext::context_location profile);
  HKEY GetVersionedRootKey(int version, int mode, CRhinoProfileContext::context_location profile);
#endif
  // Description:
  //   For expert use only. Use profile contexts for reading and writing 
  //   information that should persist between Rhino sessions. The profile
  //   contexts are platform and location independent.
  //    
  //   Appropriate profile contexts are:
  //       RhinoApp().m_pc_scheme_current_user
  //       RhinoApp().m_pc_global_current_user
  //       RhinoApp().m_pc_root_local_machine
  //       RhinoApp().m_pc_plugins_current_user
  //       RhinoApp().m_pc_plugins_local_machine
  //
  // Parameters:
  //   location - [in] see CRhinoProfileContext::context_location for options
  // Returns:
  //   CRhinoProfileContext reference to requested profile context.
  RHINO_SDK_FUNCTION
  CRhinoProfileContext& GetRhinoProfileContext( CRhinoProfileContext::context_location location);

  // Description:
  //   For expert use only. Gets the default profile context used by Rhino.
  //   This is currently the same as calling
  //   GetRhinoProfileContext(CRhinoProfileContext::cl_scheme_current_user)
  // Returns:
  //   CRhinoProfileContext reference.
  RHINO_SDK_FUNCTION
  CRhinoProfileContext& ProfileContext();

  // Description:
  //   For expert use only. Gets the profile context used by Rhino when saving
  //   window and dock bar position information.
  // Returns:
  //   CRhinoProfileContext reference.
  RHINO_SDK_FUNCTION
  CRhinoProfileContext& WindowPositionsProfileContext();

  /*
  Description:
    Get build date information from VERSIONINFO block of the EXE
  Parameters:
    wYear  - [out] Gets filled in with four digit year 
    wMonth - [out] Gets filled in with first 3 chars of the month
    wDate  - [out] Gets filled in with the day of the month
  */
  RHINO_SDK_FUNCTION
  void GetBuildDate( ON_wString& wYear, ON_wString& wMonth, ON_wString& wDate) const;
  /*
  Description:
    Get build date information from VERSIONINFO block of the EXE
  Parameters:
    nYear  - [out] Gets filled in with four digit year 
    nMonth - [out] Gets filled in with month
    nDate  - [out] Gets filled in with the day of the month
  */
  RHINO_SDK_FUNCTION
  void GetBuildDate( int& nYear, int& nMonth, int& nDate) const;
  /*
  Description:
    Get build date formatted for use in registry
  Parameters:
    wBuildDate - [out] L"YYYY-MM-DD"
  */
  RHINO_SDK_FUNCTION
  void GetBuildDate( ON_wString& wBuildDate) const;

  /*
  Description:
    Auto save if necessary
  Parameters:
    rhino_doc_sn - [in]
      serial number of document to save
    lpsCommandName - [in] If command name is in the  CRhinoAppFileSettings::m_always_autosave_before_commands
                          list then force auto save.
    auto_save_action - [in] 
      1: Ignore lpsCommandName parameter and force auto save.  
      0: If lpsCommandName is NULL or not in always save list 
         then save it save if enough time has elapsed since
         the last save.
     -1: delete auto-save file
  */
  RHINO_SDK_FUNCTION
  bool AutoSave( 
    unsigned int rhino_doc_sn,
    const wchar_t* lpsCommandName,
    int auto_save_action
    );

  /*
  Description:
    Set default render application
  Parameters:
    uuid - [in] ID of render plug-in.
  Returns:
    true  - If plug-in found and loaded successfully
    false - If ID is invalid or was unable to load plug-in
  */
  RHINO_SDK_FUNCTION
  bool SetDefaultRenderApp(ON_UUID &uuid);
  /*
  Description:
    Get the application ID for the default render plug-in
  */
  RHINO_SDK_FUNCTION
  ON_UUID GetDefaultRenderApp() const;

  //STEVEBAERV6// - Need to chat with John about plan for supporting menu in V6
  RHINO_SDK_FUNCTION
  BOOL32 AddMenuItem( HMENU hTargetMenu, UINT index, MENUITEMINFO* pMi, CRhinoPlugIn* pApp);

  //STEVEBAERV6// - Need to chat with John about plan for supporting menu in V6
  RHINO_SDK_FUNCTION
  BOOL32 AddMenu( HMENU hSubMenu, LPCTSTR title, CRhinoPlugIn* pApp);

  //STEVEBAERV6// - Need to chat with John about plan for supporting menu in V6
  RHINO_SDK_FUNCTION
  UINT GetUniqueMenuItemID( HMENU hMenu);

  //STEVEBAERV6// - Need to chat with John about plan for supporting menu in V6
  RHINO_SDK_FUNCTION
  BOOL32 RemovePlugInAddedMenu( HMENU hmenu, CRhinoPlugIn* plug_in);

  /*
  Description:
     Finds a menu item ( may be a sub-menu) in a menu.
     Fills in the MENUITEMINFO and returns the index or -1 if not found
     MENUITEMINFO must be initialized and have dwTypeData pointing to a buffer
     and cch set to the buffer's length if you want a string back 
     If you don't care about dwTypeData, you don't have to init the structure
  */
  //STEVEBAERV6// - Need to chat with John about plan for supporting menu in V6
  RHINO_SDK_FUNCTION
  int FindMenuItem( HMENU hmenu, LPCTSTR string, MENUITEMINFO* pMi);

  /*
  Description:
    FindMenuItem searches the Rhino menu structure for a certain item. 
    On success the hParentMenuHandle contains the HMENU of the menu 
    containing the sought item and nMenuItemIndex is the index of the 
    item within hParentMenuHandle.

  Parameters:
    sEnglishMenuPath - [in] Double-colon separated full path to menu
                            example: Too&ls::&Commands
                            The & characters (used for keyboard acceleration) must be
                            specified correctly for this call to work on localized versions
                            of Rhino.
    hParentMenuHandle - [out] the handle of the menu that contains the menu item.
    nMenuItemIndex - [out] the index of the menu item in hParentMenuHandle
   
  Returns:
    true on success.
    false on failure.
  */
  //STEVEBAERV6// - Need to chat with John about plan for supporting menu in V6
  RHINO_SDK_FUNCTION
  bool FindRhinoMenuItem( const wchar_t* sEnglishMenuPath, 
                          HMENU& hParentMenuHandle, 
                          int& nMenuItemIndex 
                        );

  /*
  Description:
    Adds a new page to the object properties dialog.
  Parameters:
    pPage   - [in] The page to add to the object properties dialog.
    pPlugIn - [in] The plug-in that is adding the page.
  Returns:
    TRUE on success.
    FALSE on failure.
  See also:
    CRhinoApp::RemovePageFromObjectPropertiesDialog
    CRhinoPlugIn::AddPagesToObjectPropertiesDialog
  */
  ON_DEPRECATED_MSG("Derive a class from IRhinoAddPropertiesPages in your DLL to add pages")
  RHINO_SDK_FUNCTION
  BOOL32 AddPageToObjectPropertiesDialog(
        class IRhinoPropertiesPanelPage* pPage,
        const CRhinoPlugIn* pPlugIn
        );

  /*
  Description:
    Remove an existing page page from the object properties dialog.
  Parameters:
    pPage   - [in] The page to remove from the object properties dialog.
  Returns:
    TRUE on success.
    FALSE on failure.
  Remarks:
    The removed page is destroyed with a call to DestroyWindow, but the page
    is not deleted. The caller is responsible of deleting the page.
  See also:
    CRhinoApp::AddPageToObjectPropertiesDialog
    CRhinoPlugIn::AddPagesToObjectPropertiesDialog
  */
  ON_DEPRECATED_MSG("Derive a class from IRhinoAddPropertiesPages in your DLL to add pages, the destructor will remove them automatically")
  RHINO_SDK_FUNCTION
  BOOL32 RemovePageFromObjectPropertiesDialog(
    class IRhinoPropertiesPanelPage* pPage);

  /*
  Description:
    Show help topic.
  Parameters:
    helpTopicLink - [in] if null or empty string, the help topic for the
       active command is displayed otherwise;  helpTopicLink specifies a
       help topic URL.
  Returns:
    TRUE  - help topic found
    FALSE - help topic not found
  */
  RHINO_SDK_FUNCTION
  bool DoHelp( const wchar_t* helpTopicLink) const;

  /*
  Description:
    Call this method to display the Rhino or RhinoSkin about box.
  Parameters:
    bForceRhinoAbout - [in] This parameter is only used if Rhino is
    running in skin mode, see CRhinoSkin for details.  If Rhino is
    in skin mode and this is true then the Rhino about box will display
    otherwise the skin splash screen will display.
  See Also:
    CRhinoSkin class
  */
  RHINO_SDK_FUNCTION
  void RhinoAbout( bool bForceRhinoAbout = false ) const;

  /*
  Description:
    Gets the module state of the Rhino core executable.
    Using this information is for advanced, skilled, and
    very careful developers.  You are probably best off
    pretending this does not exist.  It is exported just
    in case somebody needs this low level information
    in a plug-in situation that we have yet to discover.
  Returns:
    Rhino core executable module state.
  */
  RHINO_SDK_FUNCTION
  AFX_MODULE_STATE* RhinoModuleState() const;

  /*
  Description:
    Gets the command line used to launch Rhino.
  Returns:
    A string like '"C:\Program Files\Rhino 6\System\Rhino.exe" /nosplash "filename.3dm"'.
  */
  RHINO_SDK_FUNCTION
  LPCTSTR RhinoCommandLine() const;

  RHINO_SDK_FUNCTION
  CRhinoDirectoryManager& RhinoDirectoryManager();

   /*
  Description:
    The command manager lets you find commands from names
    and uuids.
  Returns:
    Reference to the command manager tool.
  */
  RHINO_SDK_FUNCTION
  class CRhinoCommandManager& RhinoCommandManager();
   /*
  Description:
    Call this method to get a sorted list of monitor display rectangles associated
    with the currently installed monitors.  The array will be sorted by the left
    edge.
  Returns:
    Reference to the monitor rectangles.
  */
  RHINO_SDK_FUNCTION
  ON_SimpleArray<ON_4iRect> MonitorRects() const;

  enum cursor_tooltip_pane
  {
    number_pane = 0,
    osnap_pane = 1,
    point_pane = 2,
    relative_point_pane = 3,
    prompt_pane = 4,
    feedback_pane = 5,
    extra_pane = 6,
    cursor_tooltip_pane_count = 7,
  };

  /*
  Description:
    Returns the localized osnap name.
  */
  RHINO_SDK_FUNCTION
  const wchar_t* OsnapName( CRhinoAppSettings::osnap_mode);

  /*
  Description:
    Sets a cursor tool-tip string shown next to the mouse cursor.
    Overrides all cursor tool-tip panes.
  */
  RHINO_SDK_FUNCTION
  void SetCursorToolTip( const ON_wString&);

  /*
  Description:
    Sets tooltip osnap pane.
  */
  RHINO_SDK_FUNCTION
  void SetCursorToolTipOsnapPane( CRhinoAppSettings::osnap_mode);

  /*
  Description:
    Sets tooltip number pane.
  */
  RHINO_SDK_FUNCTION
  void SetCursorToolTipNumberPane( double d, bool bAutoFormatDistance = true);

  /*
  Description:
    Sets tooltip point pane.
  */
  RHINO_SDK_FUNCTION
  void SetCursorToolTipPointPane( ON_3dPoint point, const ON_Plane* plane = nullptr);

  /*
  Description:
    Sets tooltip prompt pane.
  */
  void SetCursorToolTipPromptPane( const ON_wString&);

  /*
  Description:
    Sets tooltip pane. Can be used as an override to set any pane to any string.
  */
  RHINO_SDK_FUNCTION
  void SetCursorToolTipPane( CRhinoApp::cursor_tooltip_pane, const ON_wString&);

  /*
  Description:
    Clears one tooltip pane.
  */
  RHINO_SDK_FUNCTION
  void ClearCursorToolTipPane( CRhinoApp::cursor_tooltip_pane);

  /*
  Description:
    Clears cursor tooltip, including all tooltip panes.
  */
  RHINO_SDK_FUNCTION
  void ClearCursorToolTip();

  /*
  Description:
    Updates cursor tooltip location. Called internally by CRhinoView::OnMouseMove.
  */
  bool UpdateCursorToolTipLocation( const ON_2iPoint& pt);

  /*
  Description:
    Used internally for initializing the cursor tooltip.
  */
  void InitCursorToolTip();

  /*
  Description:
    Rhino UI library, paint and tool/control bar managers.
  */
  RHINO_SDK_FUNCTION
  class CRhinoAppUiManager& RhinoUiManager() const;
  /*
  Description:
    Rhino UI library resource manager
  */
  RHINO_SDK_FUNCTION
  class CRhinoAppUiResourceManager& RhinoUiResourceManager() const;
#if defined (ON_RUNTIME_WIN)
  /*
  Description:
    Rhino UI library dock bar manager
  */
  RHINO_SDK_FUNCTION
  class CRhinoAppUiDockBarManager& RhinoUiDockBarManager() const;
#endif
  /*
  Description:
    Rhino UI library paint manager
  */
  RHINO_SDK_FUNCTION
  class CRhinoUiPaintManager& RhinoUiPaintManager() const;
  /*
  Description:
    Rhino central Texture manager.
  */
  RHINO_SDK_FUNCTION
  class CRhTextureManager& TextureManager();
  /*
  Description:
    String that gets appended to end of build name when opening
    registry keys.
  */
  RHINO_SDK_FUNCTION
  LPCTSTR RegistrySchemeName() const;

  /*
  Description:
    Returns the pointer to an interface object in a plug-in.
  Parameter:
    plugin_uuid [in] - The plug-in to query.
    interface_uuid [in] - The requested interface.
  Return:
    The IUnknown pointer of the requested interface, if successful.
    Return NULL on error or if the interface is not supported.
  Notes:
    It is the responsibility of the application calling this function to
    increment and decrement the object's reference count using AddRef()
    and Release().
  */
  RHINO_SDK_FUNCTION
  LPUNKNOWN GetPlugInObjectInterface(
        const ON_UUID& plugin_uuid,
        const ON_UUID& interface_uuid
        );

  /*
  Description:
    Add a command to Rhino.
  Parameters:
    pCommand - [in] pointer to a command.  This class
                    must NEVER be destroyed.
  Returns:
    True if successful.  False if command was not added because
    the pointer was NULL or it was attempting to replace a
    command with the same UUID, English name, or local name.
  */
  RHINO_SDK_FUNCTION
  BOOL32 RegisterCommand(CRhinoCommand* pCommand);


  /*
  Description:
    Add a custom grips updater to Rhino.
  Parameters:
    pCustomGripsInformation - [in] 
      Pointer to a CRhinoGripsEnabler class.
      The class must NEVER be destroyed.
  Returns:
    True if successful.  False if updater was not added because
    the UUID was NULL or in use.
  */
  RHINO_SDK_FUNCTION
  bool RegisterGripsEnabler( 
    CRhinoGripsEnabler* pCustomGripsInformation
    );

  RHINO_SDK_FUNCTION
  CRhinoGripsEnabler* FindGripsEnabler(
                ON_UUID grips_id
                ) const;

  /*
  Returns:
    Pointer to currently running command.
  */
  MAC_DEPRECATED
  RHINO_SDK_FUNCTION
  const CRhinoCommand* ActiveCommand() const;

  /*
  Returns:
   Style of currently running command: interactive_ui, script_ui, or batch_ui
  */
  MAC_DEPRECATED
  RHINO_SDK_FUNCTION
  const CRhinoCommandContext::COMMAND_STYLE ActiveCommandStyle() const;

  /*
  Description:
    When script running or nestable commands are running,
    there can be more than one command running at a given time.
    The private array m_running_commands_stack[] keeps
    track of all the commands that are running.  The one
    on the end of the array is the "active" command.
    You really should not be using this function, except
    when you are forced to hack around a sticky situation
    that requires you to know when a certain command (like
    a base level script running command) may be active.
  Parameters:
    i - [in] 0 based index.
  Returns:
    Pointer to command.
  */
  MAC_DEPRECATED
  RHINO_SDK_FUNCTION
  const CRhinoCommand* RunningCommand(
          int i
          ) const;


  /*
  Description:
    Expert user tool to remove commands.
  Parameters:
    pCommand - [in] command to purge.
    password - [in] Contact developer support for the value.
          Your request must include a working plug-in that
          demonstrates exactly what the command will do
          and include a well written justification for
          why your command should be allowed to replace
          another command.  In addition, you must 
          demonstrate the ability to provide high quality
          electronic support and rapid electronic updates.
          If any crashes or unusual things happen when
          we test your plug-in, your request will be
          denied.  If your request is denied three times,
          no more requests will be processed.
  Returns:
    True if the command is successfully purged.
  */
  RHINO_SDK_FUNCTION
  bool PurgeCommand(
          CRhinoCommand* pCommand,
          ON_UUID password
          );

  /*
  Description:
    Use a command's UUID to find a command.
  Parameters:
    command_uuid - [in] command UUID.
  Returns:
    Pointer to the command or NULL if no command was found.
  */
  RHINO_SDK_FUNCTION
  CRhinoCommand* FindCommand( 
          ON_UUID command_uuid 
          ) const;

  /*
  Description:
    Use a command's name to find a command.
  Parameters:
    command_name - [in] command name.  Use the "_" prefix
      if you want to lookup the English command name.  If no
      "_" prefix is present, then this will search for
      the local command name..
  Returns:
    Pointer to the command or NULL if no command was found.
  */
  RHINO_SDK_FUNCTION
  CRhinoCommand* FindCommand( 
          const wchar_t* command_name 
          ) const;

  /*
  Description:
    Hides the toolbars based on the passed in flag...
  Parameters:
    nDockState  [in]: 0 = hide ALL toolbars regardless of dock state
                      1 = hide only DOCKED toolbars
                      2 = hide only FLOATING toolbars
    bRedraw     [in]: true = redraw and recalculate the main window and layout
                      false = don't do anything after the toolbars have been hidden.
  Returns:
    true  - if any toolbars with specified state were hidden.
    false - if nothing was hidden.
  See Also:
    CRhinoApp::RestoreToolBars()
  */
  RHINO_SDK_FUNCTION
  bool HideToolBars(int nDockState=0, bool  bRedraw=true, const CRhinoDoc* pDoc = nullptr);
  
  
  /*
  Description:
    Restores the toolbars to their previous state prior to calling
    "HideToolBars"
  Parameters:
    bRedraw     [in]: true = redraw and recalculate the main window and layout
                      false = don't do anything after the toolbars have been hidden.
    pDoc        [in]: Pointer to the document whose toolbars should be hidden.
                      nullptr is used to indicate the current document
  Returns:
    true  - if any toolbars were restored.
    false - if nothing was done.
  See Also:
    CRhinoApp::HideToolBars()
  */
  RHINO_SDK_FUNCTION
  bool RestoreToolBars(bool  bRedraw=true, const CRhinoDoc* pDoc = nullptr);
  
  
  //////////////////////////////////////////////////////
  // Control bar bit masks...
  enum
  {
    HCB_COMMANDLINE_BAR         = 0x00000001,
    HCB_CURVATURE_ANALYSIS_BAR  = 0x00000002,
    HCB_CURVATURE_GRAPH_BAR     = 0x00000004,
    HCB_DRAFT_ANGLE_BAR         = 0x00000008,
    HCB_EDGE_ANALYSIS_BAR       = 0x00000010,
    HCB_EMAP_BAR                = 0x00000020,
    HCB_LAYER_BAR               = 0x00000040,
    HCB_LAYER_MANAGER_BAR       = 0x00000080,
    HCB_MOVE_UVN_BAR            = 0x00000100,
    HCB_NOTES_BAR               = 0x00000200,
    HCB_OSNAP_BAR               = 0x00000400,
    HCB_PROPERTIES_BAR          = 0x00000800,
    HCB_VIEW_MANAGER_BAR        = 0x00001000,
    HCB_ZEBRA_BAR               = 0x00002000,
    HCB_COMMAND_CONTEXT_BAR     = 0x00004000,
    HCB_END_ANALYSIS_BAR        = 0x00008000,
    HCB_DIRECTION_ANALYSIS_BAR  = 0x00010000,
  };

  /*
  Description:
    Hides the control bars based on the passed in bit mask 
    and dock state...See above list that describes the control
    bars that are currently supported and their corresponding
    mask value:
  Parameters:
    nCBMask     [in]: Control bar bit mask indicating which control bars
                      to hide. If the corresponding bit is ON, then that
                      control bar will be hidden (see description above
                      for control bar bit values).
    nDockState  [in]: 0 = hide control bars regardless of dock state
                      1 = hide only DOCKED control bars
                      2 = hide only FLOATING control bars
    bRedraw     [in]: true = redraw and recalculate the main window and layout
                      false = don't do anything after the toolbars have been hidden.
    pDoc        [in]: Pointer to the document whose toolbars should be restored.
                      nullptr is used to indicate the current document
  Returns:
    true  - if any toolbars with specified state were hidden.
    false - if nothing was hidden.
  See Also:
    CRhinoApp::RestoreControlBars()
  */
  RHINO_SDK_FUNCTION
  bool HideControlBars(UINT  nCBMask=0xFFFFFFFF, int nDockState=0, bool bRedraw=true, const CRhinoDoc* pDoc = nullptr);

  /*
  Description:
    Restores the control bars to their previous state prior to calling
    "HideControlBars"
  Parameters:
    bRedraw     [in]: true = redraw and recalculate the main window and layout
                      false = don't do anything after the control bars have been hidden.
    pDoc        [in]: Pointer to the document whose control bars should be hidden.
                      nullptr is used to indicate the current document
  Returns:
    true  - if any control bars were restored.
    false - if nothing was done.
  See Also:
    CRhinoApp::HideControlBars()
  */
  RHINO_SDK_FUNCTION
  bool RestoreControlBars(bool bRedraw=true, const CRhinoDoc* pDoc = nullptr);


  //////////////////////////////////////////////////////
  // Window bar bit masks...
  enum
  {
    HWB_TITLE_BAR     = 0x00000001,
    HWB_MENU_BAR      = 0x00000002,
    HWB_STATUS_BAR    = 0x00000004,
  };
  
  /*
  Description:
    Hides the main window's window bars based on the passed in bit mask.
    See above list that describes the different types of window bars
    that are currently supported and their corresponding mask value.
  Parameters:
    nWBMask     [in]: Window bar bit mask indicating which window bars
                      to hide. If the corresponding bit is ON, then that
                      window bar will be hidden (see description above
                      for window bar bit values).
    bRedraw     [in]: true = redraw and recalculate the main window and layout
                      false = don't do anything after the bars have been hidden.
    pDoc        [in]: Pointer to the document whose control bars should be restored.
                      nullptr is used to indicate the current document
  Returns:
    true  - if any bars specified were hidden.
    false - if nothing was hidden.
  See Also:
    CRhinoApp::RestoreWindowBars()
  */
  RHINO_SDK_FUNCTION
  bool HideWindowBars(UINT  nWBMask=0xFFFFFFFF, bool bRedraw=true, const CRhinoDoc* pDoc = nullptr);

  /*
  Description:
    Restores the window bars to their previous state prior to calling
    "HideWindowBars"
  Parameters:
    bRedraw     [in]: true = redraw and recalculate the main window and layout
                      false = don't do anything after the window bars have been hidden.
  Returns:
    true  - if any window bars were restored.
    false - if nothing was done.
  See Also:
    CRhinoApp::HideWindowBars()
  */
  RHINO_SDK_FUNCTION
  bool RestoreWindowBars(bool bRedraw=true, const CRhinoDoc* pDoc = nullptr);

  /*
  Description:
    Sets Rhino's main window fullscreen mode.
  Parameters:
    nCBMask     [in]: Control bar bit mask indicating which control bars
                      to hide. If the corresponding bit is ON, then that
                      control bar will be hidden (see description above
                      for control bar bit values).
    nWBMask     [in]: Window bar bit mask indicating which window bars
                      to hide. If the corresponding bit is ON, then that
                      window bar will be hidden (see description above
                      for window bar bit values).
    nDockState  [in]: 0 = hide control bars regardless of dock state
                      1 = hide only DOCKED control bars
                      2 = hide only FLOATING control bars
    pDoc        [in]: Pointer to the document to be shown full screen.
                      nullptr is used to indicate the current document
  Returns:
    true  - if fullscreen mode was successfully set.
    false - if fullscreen mode failed to get set.
   
   See Also:
    CRhinoApp::RestoreFullscreen()
    CRhinoApp::HideToolBars()
    CRhinoApp::HideControlBars()
    CRhinoApp::HideWindowBars()
 */
  RHINO_SDK_FUNCTION
  bool SetFullscreen(UINT nCBMask, UINT  nWBMask, int  nDockState, const CRhinoDoc* pDoc = nullptr);

  /*
  Description:
    Resets Rhino's main window from fullscreen mode, which got set
    previously by a call to SetFullscreen()...
  Parameters:
    pDoc        [in]: Pointer to the document to be shown full screen.
                      nullptr is used to indicated the current document
  Returns:
    true  - if successfully restored from fullscreen mode.
    false - if failed to come out of fullscreen mode...or if fullscreen
            wasn't active in the first place...
  See Also:
    CRhinoApp::SetFullscreen()
    CRhinoApp::RestoreToolBars()
    CRhinoApp::RestoreeControlBars()
    CRhinoApp::RestoreWindowBars()
  */  
  RHINO_SDK_FUNCTION
  bool RestoreFullscreen(const CRhinoDoc* pDoc = nullptr);

  RHINO_SDK_FUNCTION
  bool InFullscreen() const;
  
  RHINO_SDK_FUNCTION
  bool FullscreenEscapeDisabled() const;
  
  // Return UUID for the one and only Rhino system render plug-in
  RHINO_SDK_FUNCTION
  UUID RhinoRenderPlugInUUID() const;

  // Description:
  //   Pause for specified time but keep Windows message pump alive so views will update
  //   and windows will repaint
  // Parameters:
  //   dwMilliseconds - [in] - wait time in milliseconds
  RHINO_SDK_FUNCTION
  void Wait( DWORD dwMilliseconds);

  /*
  Returns:
    Current file reading options set by various UI dialogs
    and functions.
  Remarks:
    If you need to save and restore current read options, use
    an instance of a CRhinoSaveAndRestoreAppFileReadOptions 
    class on the stack.

    This is a foolish place to store these file reading
    settings and to specify how files should be read,
    but it cannot be changed in V5 without breaking the
    public SDK.  So, as a plug-in developer, use the
    FileReadOptions() functions and use only references
    (rather than local instances of CRhinoFileReadOptions
    classes) and changes you make will actually be used
    by core Rhino file reading code.
  */
  RHINO_SDK_FUNCTION
  const CRhinoFileReadOptions& FileReadOptions() const;

  RHINO_SDK_FUNCTION
  CRhinoFileReadOptions& FileReadOptions();

  RHINO_SDK_FUNCTION
  class CRhinoDigitizerManager& DigitizerManager();

  RHINO_SDK_FUNCTION
  bool IsAutomated() const;

  RHINO_SDK_FUNCTION
  bool IsHeadless() const;

  RHINO_SDK_FUNCTION
  bool InAppMode() const;

  RHINO_SDK_FUNCTION
  bool RememberCopyOption() const;
 
 RHINO_SDK_FUNCTION
  void SetRememberCopyOption(bool b);

  // Description:
  //   Get a list of all of the recently run macros/commands
  // Parameters:
  //   display_strings, macros: two lists that MRU command information is added to
  //     in parallel
  // Returns:
  //   number of items added to the lists
  RHINO_SDK_FUNCTION
  int GetMRUCommands(ON_ClassArray<ON_wString>& display_strings, ON_ClassArray<ON_wString>& macros) const;

  RHINO_SDK_FUNCTION
  class CRhSkin& RhSkin() const;

  // Returns:
  //   true if the license will expire
  //   false otherwise
  RHINO_SDK_FUNCTION
  bool LicenseExpires();

  // Returns:
  // true if Rhino currently uses the Cloud Zoo lease
  // false otherwise
  RHINO_SDK_FUNCTION
  bool IsCloudZooNode();

  // Returns:
  // 2 if Updates and Statistics are disabled by administrator
  // 1 if Updates and Statistics are disabled by current user
  // 0 if Updates and Statistics are enabled
  RHINO_SDK_FUNCTION
  int UpdatesAndStatisticsStatus();

  // Returns:
  // true if Rhino is allowed to access the Internet, false otherwise
  // Some large corporations require that Rhino be silent, and not try
  // to access the Internet. All Rhino components that try to access
  // the Internet should check this setting before initiating an outbound
  // connection.
  RHINO_SDK_FUNCTION
  bool IsInternetAccessAllowed();

  // Returns:
  // The owner of the license or lease.
  RHINO_SDK_FUNCTION
  ON_wString LicenseUserName();

  // Returns:
  // The organization the user owning the license or lease belongs to.
  RHINO_SDK_FUNCTION
  ON_wString LicenseUserOrganization();

  // Returns:
  // The type of installation (product edition) of the license or lease.
  RHINO_SDK_FUNCTION
  ON_wString InstallationTypeString();

  // Returns 
  //   true if the license is validated
  //   false otherwise
  RHINO_SDK_FUNCTION
  bool IsLicenseValidated();

  RHINO_SDK_FUNCTION
  bool IsPreRelease();
  
  // Returns number of days within which validation must occur. Zero when
  //   validation grace period has expired.
  // Raises InvalidLicenseTypeException if LicenseType is one of:
  //   EvaluationSaveLimited
  //   EvaluationTimeLimited
  //   Viewer
  //   Unknown
  RHINO_SDK_FUNCTION
  int ValidationGracePeriodDaysLeft();

  // Returns number of days until license expires. Zero when
  //   license is expired.
  // Raises InvalidLicenseTypeException if LicenseExpires
  // would return false.
  RHINO_SDK_FUNCTION
  int DaysUntilExpiration();

  // Returns number of saves left in save-limited Evaluation. Zero when
  //   evaluation is expired.
  // Raises InvalidLicenseTypeException if LicenseType != EvaluationSaveLimited
  // Returns false in an expired evaluation version or otherwise save limited state (invalid licence etc).
  RHINO_SDK_FUNCTION
  bool CanSave();

  RHINO_SDK_FUNCTION
  bool AskUserForRhinoLicense(bool bStandalone, HWND parent);

  RHINO_SDK_FUNCTION
  bool RefreshRhinoLicense();

  // Returns true if Rhino is using a custom, user-interface Skin.
  // For more information on Skins, see:
  //   http://developer.rhino3d.com/guides/cpp/creating-a-skin/
  //   http://developer.rhino3d.com/guides/rhinocommon/creating-a-skin/
  RHINO_SDK_FUNCTION
  bool IsSkinned() const;

  /*
  Description:
    Gets the URL passed to Rhino by the protocol handler registered with the system.
    The URL is in the full form protocol://verb/path (rhino7://package/search/<package name>)

  Parameters:
    protocolHandlerUrl - [out]
      the URL
  Returns:
    true if protocolHandlerUrl is set; false otherwise
  */
  RHINO_SDK_FUNCTION
  bool GetProtocolHandlerUrl(ON_wString& protocolHandlerUrl);

public:
  ///////////////////////////////////////////////////////////////////
  //
  // Everything below must NOT be exported to the SDK.
  // Check with Steve or Dale before you change things here 
  // or add more functions.
  //
  // Do NOT add members that require a complete class definition.
  // Use pointers and call new in CRhinoApp::InitInstance();
  //
  CRhinoApp(class CRhApp&);
  virtual ~CRhinoApp();

  /*
  Description:
    Creates an new CRhinoView.
  
  Parameters:
    rhino_doc_sn - [in]
      Runtime serial number of the document being viewed.
    view - [in]
      null or information about the view type and projection.
    pMfcViewRuntimeClass - [in]
      null or an MFC runtime class for a class derived from CRhinoView.
    plugin_id - [in]
      nil 
        Core Rhino is creating the view.
      id of a plug-in
        A plug-in is using CRhinoDoc.CreateRhinoView() to create a view.
        (It is intentional that plug-ins cannot call CRhinoApp.CreateRhinoView().)
    bShowView - [in]
      If false, the view will not be shown.  This is what you
      should do if you need to perform additional initialization
      on the view before it appears.
      If true, the view will be shown and updated.
  Returns:
    Runtime serial number of the new view which will be a class
    derived from CRhinoView.
    CRhinoView::FromRuntimeSerialNumber(...) can be used to get a pointer to this 
    class.
  */
#if defined (RHINO_SDK_MFC)
  unsigned int CreateRhinoView(
    unsigned int rhino_doc_sn,
    const ON_3dmView* view,
    CRuntimeClass* pMfcViewRuntimeClass,
    bool bShowView
    );
#endif

  /*
  Description:
    Calls CRhinoDoc::ArrangeViews(bMakeVisible) for every document.
    This is useful when the main window or main client is resized.
  Parameters:
    bMakeVisible - [in]
      bMakeVisible parameter passed to CRhinoDoc.ArrangeViews().
  Remarks:
    ArrangeViews() does not modify any active document or active
    view settings and that is intentional.
  */
  void ArrangeViews( bool bMakeVisible );
  
  /*
  Returns:
    True if Rhino should act like a single doc application.
  */
  bool IsSingleDocApp() const;

  /*
  Description:
    Checks to see if Rhino is allowed to run.
  Returns:
    True if Rhino can run; false otherwise.
  */
  bool IsRhinoAllowedToRun() const;

  /*
  Returns:
    For the Rhino application there is a single CDocTemplate.
    and this function returns a pointer to that MFC CDocTemplate.
    The CDocTemplate class is needed in low level application 
    code that manages CRhinoDoc objects.  
  Remarks:
    Do not export this function.  If we ever end up having
    two or more document templates, this function will be
    deleted and all code that uses this function will need
    to be upgraded.
  */
#if defined (RHINO_SDK_MFC)
  CDocTemplate* RhinoMfcDocTemplate();
#endif

  BOOL32 ExitRhino();

  // See source code comments for usage.  Do not
  void EmergencyExit( int );

  // Temporary - this should not be on CRhinoApp.
  class CRhWorkSession* WorkSession() const;

  // Description:
  //   Called by CMainFrame or CInPlaceFrame
  // Parameters:
  //   Standard Windows procedure parameters.
  //   bInPlace [in] - True if called from CInPlaceFrame
  // Returns:
  //   TRUE  - Frame window will call default procedure
  //   FALSE - Frame will not call default procedure
  BOOL32 FrameWindowProc( UINT message, WPARAM wParam, LPARAM lParam, BOOL32 bInPlace);

  // Description:
  //   Add MRU command to list, pop old commands off when list
  //   reaches max size.
  // Parameters:
  //   cmd  - [in] command macro to add
  //   display_string - [in] String displayed in MRU command popup menu, if NUL then
  //                         displays macro.
  MAC_DEPRECATED
  void AddMruCommand( const wchar_t* cmd, const wchar_t* display_string = nullptr);
  // Description:
  //   Called by CMruCommandMenu::MruCmd() to run a m_mru_command_list command
  //   macro.  Will move the macro from its current list position to top of
  //   display list.
  // Parameters:
  //   cmd_index  - [in] command macro to run
  MAC_DEPRECATED
  void RunMruCommand( int cmd_index);
  // Description:
  //   Called by CMruCommandMenu::Favorite() to run a CRhinoAppGeneralSettings.m_popup_favorites
  //   macro.  Will make sure command does not show up on the MRU list
  // Parameters:
  //   cmd  - [in] m_mru_command_list index of command to run
  MAC_DEPRECATED
  void RunMruFavoriteCommand( int cmd_index);
  // Description:
  //   MRU command list contains list of recently run macros/commands
  // Returns:
  //   Reference to MRU command list.
  class CRhMruCommandRecord
  {
  public:
    CRhMruCommandRecord( const wchar_t* = nullptr, const wchar_t* = nullptr);
    CRhMruCommandRecord( const wchar_t*, const wchar_t*, const wchar_t*, const UINT);
    const wchar_t* DisplayString() const;

    void GetDisplayStringWithoutShortcut(ON_wString&) const;
    void GetShortcut(ON_wString&) const;
    UINT GetModifierFlags() const;

    void EmptyRecord();

    ON_wString m_display_string;
    ON_wString m_macro;
  };
  const ON_ClassArray<CRhMruCommandRecord>& MruCommandList() const;
  const CRhMruCommandRecord* CommandToRepeat() const;

  bool IsDontRepeatCommand( wchar_t* token);

  void OnInitMenuPopup( WPARAM wparam, LPARAM lparam);

  int DispatchPluginMenuCommand( HMENU hmenu, WPARAM wParam, LPARAM lParam);
  // Description:
  //   Called by various OnKeyDown methods to assign shortcut keys
  // Parameters:
  //   nChar - [in] - key sending the message
  // Returns:
  //   TRUE  - nChar translated to an assigned key and a macro was executed.
  //   FALSE - nChar was not a shortcut key or there was no macro associated
  //           with it.
  BOOL32 AcceleratorKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );


  // Does the "generic" ESCAPE key handling that should happen if
  // nothing specific applies.
  bool OnEscapeKeyDown();

  // Does the "generic" Alt+Fx and F10 key handling
  bool OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

  // Needed for Alt + arrow key input buffer cleanup when nudging
  bool OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

  // Activates flashing caret in command prompt
  bool ActivateCommandWindowCaret() const;
  bool SetAllowActivateCommandWindowCaret( bool b = true);
  bool AllowActivateCommandWindowCaret() const;

  void EchoCommand( const wchar_t*) const;
  void EchoPrompt( const wchar_t*) const;
  void SetCommandWindowText( const wchar_t* = nullptr);

  void EnterCommand( CRhinoDoc& doc, const CRhinoCommand*, unsigned int* undo_record_sn );

  /*
  Parameters:
    rhino_doc_sn0 - [in]
      Document passed to command.
    rhino_doc_sn1 - [in]
      Document command worked on
      (Some command delete the input doc and make a new one.)
  */
  void ExitCommand( 
    unsigned int rhino_doc_sn0, 
    unsigned int rhino_doc_sn1,
    const CRhinoCommand*, 
    unsigned int undo_record_sn 
    );

  void GetCommandNameList( ON_TextLog& ) const;
  
  // Gets a list of command names from Rhino and plug-ins
  // then sorts it and sends it off to be printer.
  void PrintCommandNameList( bool is_interactive) const;

  bool GetCommandPromptEx( 
    ON_wString& prompt, 
    ON_wString& prompt_default, 
    ON_ClassArray<CRhinoCommandOption>& options 
    );

  /////////////////////////////////////////////////////////////////////////////
  // Returns whether not Rhino was launched with the /safemode
  // command line parameter.
  RHINO_SDK_FUNCTION
  bool IsSafeModeEnabled() const; 

  void CreateOsnapCursors( int );


  // do not export this function
  class CRhFileReadOptions& FileReadOptionsEx();
  void SetFileReadOptions( const class CRhFileReadOptions& );
  void ClearFileReadOptions();

  /////////////////////////////////////////////////////////////////////////////
  // low level tool used to send keyboard events to command window
  void SendCharToCommandWnd( UINT nChar, UINT nRepCnt, UINT nFlags) const;
  void SendKeyDownToCommandWnd( UINT nChar, UINT nRepCnt, UINT nFlags) const;


  /////////////////////////////////////////////////////////////////////////////
  // CRhinoView window class name
  LPCTSTR ViewWndClassName() const;


  /////////////////////////////////////////////////////////////////////////////
  // Use to get main frame - can return a NULL when Rhino is embedded in other
  // applications.  
  //
  // If you call this function, then you MUST CAREFULLY TEST your code by
  // making sure it works when Rhino is embedded in
  // Microsoft Word, PowerPoint and Internet Explorer.  That's THREE tests
  // you must perform - NO SLACKING.  Will return NULL when Rhino is embedded
  // in another application.
  //
  // See Also:
  //   CRhinoApp::MainWnd();  or CRhinoApp::MainWindow();
  class CMainFrame* MainFrame() const;

  // Return main window pointer, this pointer may be a CMainFrame or CInPlaceFrame
  // object, will return CInPlaceFrame if Rhino is embedded in another application.
  // Use pointer->IsKindOf( RUNTIME_CLASS( CMainFrame)) if you care or call 
  // CRhinoApp::MainFrame() if you absolutely need a CMainFrame object.
#if defined (RHINO_SDK_MFC)
  CWnd* MainWindow() const;
#endif

  class CRhAutoSave& GetAutoSave() const;

  // Description:
  //   Make contents of Notes dialog match doc's notes
  void UpdateNotesWindow( unsigned int rhino_doc_sn );

  // Description:
  //   Set status bar push button state
  // Parameters:
  //   nBntID - [in] Pane ID
  //   bIn    - [in] TRUE means depress the button otherwise pop it out
  // Returns:
  //   TRUE  - Status bar exist, nID is valid and state changed
  //   FALSE - Status bar window not created or nID is invalid
  BOOL32 PushStatusBarButton( UINT nBntID, BOOL32 bIn);

  /*
  Description:
    Create a new document and read a file into the new document.
  Parameters:
    file_name_to_read - [in]
      Name of file to read.  If null, the file dialog is used to select a file.
    bUseDialogs - [in]
      true to use dialogs
      false to use script interface
    bCreateEmptyDocOnFailure - [in]
      true if an empty document should be created when the file
      reading fails.
    file_name_read - [out]
      If not null, the name of the read file is returned here.
      If the file reading fails, an empty string is returned.
  Returns:
    Serial number of the document that is created.
  */
  unsigned int FileOpen(
    const wchar_t* file_name_to_read, // null to use file dialog
    bool bUseDialogs, 
    bool bCreateEmptyDocOnFailure,
    ON_wString* file_name_read        // [out] returns name of file read
    );

  unsigned int FileNew( 
    bool bUseDialogs 
    );

  /*
  Description:
    Import the contents of a file into the specified document.
  Parameters:
    rhino_doc_sn - [in]
      document to import into
    file_name_to_read - [in]
      name of file to read. 
      If null, the user is asked to select a file.
    bUseDialogs - [in]
      true if user dialogs can be used.  Otherwise a predictable
      scripting interface is used.
    file_name_read - [out]
      If not null, the name of the file that is actually read
      is returned here.
  Returns:
    True if the file was successfully read.
  */
  bool FileImport( 
    unsigned int rhino_doc_sn,
    const wchar_t* file_name_to_read,
    bool bUseDialogs, 
    bool bSelectNewObjects,
    ON_wString* file_name_read
    );

  bool FileReference(ON_wString& FileName, bool bUseDialog = true, HWND hWndParent = nullptr);

  void SetNoCancelOnMainFrameClose( bool b);
  bool NoCancelOnMainFrameClose() const;
  
  HINSTANCE   RhinoResourceHandle(); 
  class CStringTableCollection& StringTables();
  bool StringTablesInitialized() const;
  
  CRhinoRenderPlugIn* CurrentRenderPlugIn();

  bool PushMainMenu();
  bool PopMainMenu();
  HMENU SavedMainMenu() const;
  HMENU  MainHMenu() const;
  
  bool ViewDisableSetActiveView() const;
  bool ViewSetDisableSetActiveView( bool b);

  // DO NOT PUT THIS FUNCTION IN THE PUBLIC SDK 
  bool PostRhinoWindowsMessage( WPARAM wParam, LPARAM lParam );

  // DO NOT PUT THIS FUNCTION IN THE PUBLIC SDK 
  bool RespondToRhinoWindowsMessage( WPARAM wParam, LPARAM lParam );


  /*
  Description:
    29-November 2012
    This flag gets set by starting Rhino with the "/viewer" switch, if this is
    true then Rhino will NOT allow you to save anything, it will act similar to
    an expired evaluation version.
  Returns:
    Returns true if Rhino is running as a viewer only otherwise; returns false.
  */
  /* BG 2012-12-06: Let's not export this yet, as we may get rid of it completely 
  RHINO_SDK_FUNCTION*/ 
  bool ViewerOnlyMode() const { return m_RunAsViewerOnly; }


  bool UseNetCoreRuntime();
  void GetNetCoreRuntimeVersion(ON_wString& wVersion);

protected:
  void SetCommandToRepeat( const CRhinoApp::CRhMruCommandRecord& rec);

  MAC_DEPRECATED
  LPCTSTR MruMenuString() const;
  MAC_DEPRECATED
  void SetMruMenuString( HMENU hmenu, UINT id);

#if defined(ON_RUNTIME_WIN)
#if defined (RHINO_SDK_MFC)
  BOOL32 RhinoProcessShellCommand( CCommandLineInfo& rCmdInfo, int mode);
#endif
#endif

  //Special version of SetDefaultRenderApp called by InitInstance.  Not exported.
  bool SetDefaultRenderApp(ON_UUID& uuid, bool force);

  // This should only be called one time by CRhinoApp::InitInstance. Use this
  // to initialize CRhinoApp properties
  void LoadProfile( CRhinoProfileContext& PC );
  class CCommandControlBar* CommandWindow() const;
public:
  class CRhinoStatusBar* StatusBar() const;
protected:

  void WinAppOnFileNew(); // TODO: remove this, temp hack to access private CWinApp func.

  // Rhino's version of COleObjectFactory::RegisterAll()
  void RegisterRhinoFactories( bool bAutomated ); 

protected:
  // Moving profile context to protected section, these should be accessed using
  // the CRhinoApp::GetRhinoProfileContext(CRhinoProfileContext::context_location)
  // method
  CRhinoProfileContext* m_pc_scheme_current_user = nullptr;
  // Attaches to the CRhAppSettings::WindowPostions() settings
  CRhinoProfileContext* m_pc_window_positions = nullptr;
private:

#if defined(ON_RUNTIME_WIN)
  // https://mcneel.myjetbrains.com/youtrack/issue/RH-44082
  // These are obsolete and are only here to keep the SDK from breaking.  Call
  // GetRhinoProfileContext to get the appropriate profile context.
  //
  // This commit (https://github.com/mcneel/rhino/pull/13104) in branch 
  // 6.x has break the Rhino 6 C++ SDK, as some member variables has been removed
  // from CRhinoApp. There are other public member variables after the removed 
  // ones that now have a different offset, so any C++ plug-in compiled using a
  // SDK before this change is crashing.
  CRhinoRegProfileContext m_obsolete_global_current_user;		
  CRhinoRegProfileContext m_obsolete_root_local_machine;		
  CRhinoRegProfileContext m_obsolete_plugins_local_machine;		
  CRhinoRegProfileContext m_obsolete_plugins_current_user;		
#endif

public:
  // Steve - I would like to eventually make these private and accessible through functions
  HCURSOR m_default_cursor = nullptr;
  HCURSOR m_wait_cursor = nullptr;
  HCURSOR m_cross_hair_cursor = nullptr;
  HCURSOR m_hand_cursor = nullptr;
  HCURSOR m_rotate_cursor = nullptr;
  HCURSOR m_magnify_cursor = nullptr;
  HCURSOR m_hyperlink_cursor = nullptr;
  HCURSOR m_context_menu_cursor = nullptr;
  HCURSOR m_prohibit_cursor = nullptr;    // circle with slash
  HCURSOR m_arrow_copy_cursor = nullptr;  // arrow with "+"
  HCURSOR m_hand_snap_cursor = nullptr;
  HCURSOR m_magnify_snap_cursor = nullptr;
  HCURSOR m_arrow_command_cursor = nullptr;
  HCURSOR m_cross_hair_copy_cursor = nullptr;

  // Current osnap cursors
  HCURSOR m_point_osnap_cursor = nullptr;
  HCURSOR m_near_osnap_cursor = nullptr;
  HCURSOR m_end_osnap_cursor = nullptr;
  HCURSOR m_mid_osnap_cursor = nullptr;
  HCURSOR m_cen_osnap_cursor = nullptr;
  HCURSOR m_knot_osnap_cursor = nullptr;
  HCURSOR m_quad_osnap_cursor = nullptr;
  HCURSOR m_tan_osnap_cursor = nullptr;
  HCURSOR m_int_osnap_cursor = nullptr;
  HCURSOR m_focus_osnap_cursor = nullptr;
  HCURSOR m_perp_osnap_cursor = nullptr;
  HCURSOR m_vertex_osnap_cursor = nullptr;
  HCURSOR m_onsrf_osnap_cursor = nullptr;
  HCURSOR m_oncrv_osnap_cursor = nullptr;
  HCURSOR m_onmesh_osnap_cursor = nullptr;
  HCURSOR m_object_tan_osnap_cursor = nullptr;
  HCURSOR m_object_perp_osnap_cursor = nullptr;

  // Current drag base point osnap cursors (arrow cursor and osnap label)
  HCURSOR m_point_osnap_arrow_cursor = nullptr;
  HCURSOR m_end_osnap_arrow_cursor = nullptr;
  HCURSOR m_mid_osnap_arrow_cursor = nullptr;
  HCURSOR m_cen_osnap_arrow_cursor = nullptr;
  HCURSOR m_knot_osnap_arrow_cursor = nullptr;
  HCURSOR m_quad_osnap_arrow_cursor = nullptr;
  HCURSOR m_vertex_osnap_arrow_cursor = nullptr;
  HCURSOR m_int_osnap_arrow_cursor = nullptr;
  HCURSOR m_near_osnap_arrow_cursor = nullptr;

  HCURSOR m_cursor = nullptr; // current cursor

#if defined (ON_RUNTIME_APPLE)
  HCURSOR m_arrow_cursor = nullptr;
#endif
public:
  class CRhinoAppFonts* m_pAppFonts;

  // list of events (text tokens, view clicks, digitizer points, etc.
  class CRhEventManager* AppEventManager() const;

  class CRhino3Splash* m_pSplash = nullptr;

public:
  BOOL32 InitInstance();
  int ExitInstance();
  RHINO_SDK_FUNCTION BOOL32 OnIdle(LONG lCount);
  BOOL32 SaveAllModified(); // save before exit

#if defined (RHINO_SDK_MFC)
  CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
  CDocument* OpenDocumentFile(LPCTSTR lpszFileName, BOOL32 bAddToMRU);
#endif

  BOOL32 PreTranslateMessage(MSG* pMsg);

  void OnAppAbout(); // Call RhinoAbout() externally
  void OnFileOpen();
  void OnFileClose();
  void OnAppFileNew();
  void OnFileNew();
  BOOL32 OnOpenRecentFile(UINT nID);

private: // All friends should be here
  friend class CMainFrame;
  friend class CRhinoAppFonts;
  friend class CRhino3SplashWnd;
  friend class CRhinoFileMenu;
  friend class CRhAppSettingsUiWatcher;
  friend void RhOnWmDisplayChange();

public:
  // 'Manager' helper classes - do NOT export to SDK
  class CRhCommandManager&     CommandManager();
  class CRhCustomMenuManager&  CustomMenuManager();
  class CRhDocViewManager&     DocViewManager();
  class CRhinoLicenseManager&  LicenseManager();
  class CRhPlugInManager&      PlugInManager();
  class CRhPrintManager&       PrintManager();
  RHINO_SDK_FUNCTION class IRhinoObjectManager&   ObjectManager(void);

protected:
  // WARNING: Do not add public data fields to CRhinoApp below this line.

  // TODO: Steve. work with John M to see if m_WP is really needed at all anymore.
  // It does not appear to be saved between sessions.
  WINDOWPLACEMENT m_WP;

  bool m_bNoCancelOnMainFrameClose = false;
  bool m_bRememberCopyOption = true; // This looks like it should be on app settings

  ON_wString m_sViewWndClass;
  ON_wString m_sCurrentMenuLabel;
  ON_wString m_sParentMenuLabel;
  HMENU m_hParentMenu = nullptr;

  ON_wString m_sSchemeName;

  HMENU m_save_menu = nullptr;

  bool m_disable_set_active_view = false; // used by CRhinoView::OnMouseMove to disable
                                          // setting active view when tracking mouse
  bool m_bSafeModeEnabled = false;
  bool m_bInViewNavigation = false;

  bool m_activate_command_caret = true; // Enable/disable focus shift from view to
  // command rich edit control.  Used mainly
  // by dialogs that allow view manipulation
  // (semi-modal dialogs)

  bool m_bInFullscreen = false;
  bool m_bRestoreFromFullscreen = false;

  CRhinoFileMenu m_FileMenu;// File menu options;

  class CToolTipCtrl* m_pCursorToolTip = nullptr;
  TOOLINFOW m_tiCursorToolTip;
  ON_wString m_wCursorToolTip;
  bool m_bCursorToolTip = false;

  ON_wString m_wCursorToolTipPanes[cursor_tooltip_pane_count];

  bool m_unused = false;

  // Initialized by CRhinoApp::RhinoProcessShellCommand if a RVB file
  // is passed to the Rhino executable at start-up.  This file will get
  // loaded after InitInstance completes successfully by
  // CRhinoApp::RespondToRhinoWindowsMessage()
  ON_wString m_CommandLineLoadScriptFileName;
  ON_wString m_CommandLineLoadGrasshopperFileName;


  class CStringTableCollection*  m_string_tables = nullptr;
  CRhinoDirectoryManager         m_dir_manager;
  class CRhSkin*                 m_pRhinoSkin = nullptr;
  ON_SimpleArray<ON_UUID>        m_PlugInFileImportDisplayList;
  ON_SimpleArray<ON_UUID>        m_PlugInFileExportDisplayList;
  ON_SimpleArray< CRhinoGripsEnabler* > m_custom_grips;
  ON_UUID m_default_render_plug_in;


  // The CRhinoApp::FileReadOptions() functions
  // return references to m_app_fro which is the
  // V5 replacement for CRhinoApp::m_FileReadeOptions.
  // See comments at the beginning of CRhinoApp::CRhinoApp()
  // for more details.
  class CRhFileReadOptions&  m_app_fro;
  unsigned char m_app_fro_buffer[4096];

  // When something happens, like a command is run or lots of other
  // stuff, the value of m_rhino_app_ticker is incremented by calling 
  // CRhinoApp::IncrementRhinoAppTicker().  The value of m_rhino_app_ticker
  // is used to determine when Rhino has been "idle" long enough that
  // we can risk calling "onidle" event watchers.
  ON__UINT64 m_rhino_app_ticker = 0;

  // 29-November 2012 John Morse - If true then run like an expired evaluation version (no saves)
  // This is set with the "/viewer" command line switch and is "false" by default
  bool m_RunAsViewerOnly = false;

  class CRhinoAppUiManager*     m_pRhinoUiManager = nullptr;

  // lazyptr classes are created the first time they are requested
  // more of the above items should be lazily created... work in progress
  CRhinoAppSettings*            m_lazyptr_app_settings = nullptr;
  class CRhAutoSave*            m_lazyptr_autosave = nullptr;
  class CRhCommandManager*      m_command_manager = nullptr;
  class CRhCustomMenuManager*   m_lazyptr_custom_menu_manager = nullptr;
  class CRhinoDigitizerManager* m_lazyptr_digitizer_manager = nullptr;
  class CRhDocViewManager*      m_lazyptr_docview_manager = nullptr;
  class CRhinoLicenseManager*   m_lazyptr_license_manager = nullptr;
  class CRhPaletteManager*      m_lazyptr_palette_manager = nullptr;
  class CRhPlugInManager*       m_lazyptr_plug_in_manager = nullptr;
  class CRhPrintManager*        m_lazyptr_print_manager   = nullptr;
  class CRhTextureManager*      m_lazyptr_texture_manager = nullptr;
  class CRhWorkSession*         m_lazyptr_work_session    = nullptr;
#if defined (ON_RUNTIME_WIN) || defined (ON_RUNTIME_LINUX)
  class CRhEventManager*        m_app_event_manager       = nullptr;
#endif
private:
  class CRhinoObjectManager*    m_lazyptr_object_manager  = nullptr;
  ON__UINT_PTR m_sdk_reserved1 = 0;
  ON__UINT_PTR m_sdk_reserved2 = 0;
  ON__UINT_PTR m_sdk_reserved3 = 0;

private:
  class CRhApp& m_winapp;

#if defined (ON_RUNTIME_WIN)
public:
  LPCTSTR RegistryKey() const;
  LPCTSTR ProfileName() const;
  RHINO_SDK_FUNCTION LPCTSTR AppName() const;
  RHINO_SDK_FUNCTION DWORD ThreadId() const;
  void SetAppName(const wchar_t*);

public:
  HICON LoadIcon(LPCTSTR lpszResourceName) const;
  HICON LoadIcon(UINT nIDResource) const;
  HCURSOR LoadCursor(UINT nIDResource) const;
  RHINO_SDK_FUNCTION void BeginWaitCursor();
  RHINO_SDK_FUNCTION void EndWaitCursor();
  void RestoreWaitCursor();       // call after messagebox
  RHINO_SDK_FUNCTION BOOL32 PumpMessage();

  BOOL32 GetPrinterDeviceDefaults(PRINTDLG* pPrintDlg);
  void SelectPrinter(HANDLE hDevNames, HANDLE hDevMode, BOOL32 bFreeOld = TRUE);
  int CmdShow(void) const;
  UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
  BOOL32 WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
  
#if defined (RHINO_SDK_MFC)
  RHINO_SDK_FUNCTION CWnd* GetMainWnd(void) const;
  INT_PTR DoPrintDialog(CPrintDialog* pPD);
  RHINO_SDK_FUNCTION CRecentFileList* RecentFileList(void) const;
  BOOL32 CreatePrinterDC(CDC& dc);
#endif
    
#else
public:
  virtual LPCTSTR RegistryKey() const = 0;
  virtual LPCTSTR ProfileName() const = 0;
  virtual LPCTSTR AppName() const = 0;
  virtual void    SetAppName(LPCTSTR) = 0;
  virtual id      AppDefaults(void) const = 0;
  virtual void    BeginWaitCursor() = 0;
  virtual void    EndWaitCursor() = 0;
  virtual HCURSOR LoadCursor(UINT nIDResource) const = 0;
  virtual class CThreadManager& ThreadManager(void) const = 0;
  virtual unsigned int GetPOSIXLocaleIdentifier() const = 0;
  virtual const char*  GetPOSIXLocaleName() const = 0;
#endif //ON_RUNTIME_WIN
    
public:
  void CloseAllDocuments(BOOL32 bEndSession);
};

RHINO_SDK_FUNCTION ON_UUID RhinoDefaultRendererId(void);
RHINO_SDK_FUNCTION int RhinoOsnapPickboxRadius(void);

RHINO_SDK_FUNCTION class CRhinoApp& RhinoApp(); // returns theApp

#if defined(ON_RUNTIME_APPLE)
class MRhinoApp& MacRhinoApp();
#endif

#if defined (ON_RUNTIME_APPLE)
#include "RhinoSdkMacRhinoApp.h"
#endif

RHINO_SDK_FUNCTION
bool RhinoIsMainThread(void);


