/*
 *  RhinoSdkMacRhinoApp.h
 *  MacRhino
 *
 *  Created by Marlin Prowell on 4/24/06.
 *  Copyright 2006 Robert McNeel & Associates. All rights reserved.
 *
 */

#pragma once

class CRhinoPlugIn;
class CThreadManager;

class MRhinoApp: public CRhinoApp
{
public:
  MRhinoApp(class CRhApp&);
  virtual ~MRhinoApp();
  
  LPCTSTR AppName() const override;
  void SetAppName(LPCTSTR) override;
    
  const wchar_t* ProfileName() const override;
  const wchar_t* RegistryKey() const override;
    
  CString RhinoGetProfileString(const wchar_t*, const wchar_t*, const wchar_t*, int);
  BOOL32 RhinoWriteProfileString(const wchar_t*, const wchar_t*, const char*, int);
  BOOL32 RhinoWriteProfileString(const wchar_t*, const wchar_t*, const wchar_t*, int);
  BOOL32 RhinoWriteProfileString(const wchar_t*, const wchar_t*, CString&, int);
  
  void Arrange3dViews(const class CDocument&);
    class CRecentFileList* RecentFileList();
  
  bool GetCommandLineScript(ON_wString&);
  
  class CDocument* OpenDocumentFile(const wchar_t*);
  void* MainHMenu() const;
  bool LoadResourceDLL(ON_wString);
  HCURSOR LoadCursor (unsigned int cursorID) const override;

  void InitializeCustomCursors();
  void BeginWaitCursor() override;
  void EndWaitCursor() override;

  void SetMruMenuString( LPCTSTR str);
  void HideOnDemandToolPalettesForCommand(LPCTSTR str);

  
  // Returns:
  //   Number of open documents.
  int GetOpenDocumentCount() const;
  
  // Description:
  //
  // Parameters:
  //   document_index - zero based array index
  //
  // Returns:
  //   Reference to the requested document.
  CRhinoDoc* GetOpenDocument(
                             int // document_index
                             ) const;

  // Dragmode cursors
  HCURSOR m_world_dragmode_cursor;
  HCURSOR m_uvn_dragmode_cursor;
  HCURSOR m_view_dragmode_cursor;
  HCURSOR m_control_polygon_dragmode_cursor;
  
  // Rhino language identifier
  unsigned int m_language_identifier;
  const char* m_language_abbreviation;

  CThreadManager& ThreadManager(void) const override;

  int Initialize();
  bool IsLicenseManagerInitialized();
  void OnClose();
  void ExitApp();

  const pthread_t MainThread();
  bool IsAnyMainThread(const pthread_t t);
  id AppDefaults(void) const override;

  const char* GetPOSIXLocaleName() const override;
  unsigned int GetPOSIXLocaleIdentifier() const override;
  static unsigned int GetPOSIXLocaleIdentifier(NSString* locale);

  class RhCommandAutocompleter& GetCommandAutocompleter();

  void OnMRApplicationTerminate();
  void OnMRApplicationTerminateCancel();

protected:
  friend CRhinoAppAppearanceSettings::CRhinoAppAppearanceSettings();
  
  BOOL32 m_bInitialized;       // TRUE after initialization is completed
  CThreadManager* m_thread_manager;
  CRhCustomMenuManager* m_custom_menu_manager;
  pthread_t m_main_thread_id;
  id m_appDefaults;     // a NSMutableDictionary of default values
  
  class RhCommandAutocompleter* m_commandAutocompleter;

  APP_STATE m_save_state = APP_STATE::app_state_ctor_zero;

  unsigned int SetPOSIXLocale();
};  
