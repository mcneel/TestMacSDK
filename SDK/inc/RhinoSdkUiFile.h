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

class RHINO_SDK_CLASS CRhinoUiFile
{
public:
  // File access methods
  static int FileCount();
  static UUID FileID(int index);
  static UUID FileID(const wchar_t* sFileName, bool bIsAlias);
  static ON_wString FileName(UUID id);
  static ON_wString FileAlias(UUID id);
  static UUID FileOpen(const wchar_t* sFileName);
  static bool FileClose(UUID id);
  static bool FileIsOpen(const wchar_t* sName, bool bIsAlias);
  static bool FileSave(UUID id);
  static bool FileSaveAs(UUID id, const wchar_t* sSaveAs);

  // Group access methods
  static int GroupCount(UUID idFile);
  static UUID GroupID(UUID idFile, int index);
  static ON_wString GroupName(UUID idFile, UUID idGroup);
  static ON_wString GroupName(UUID idFile, UUID idGroup, unsigned int locale);
  static UUID GroupDockBarID(UUID idFile, UUID idGroup);
  static bool GroupIsVisible(UUID idFile, UUID idGroup);
  static bool GroupShow(UUID idFile, UUID idGroup, bool bShow);
  static bool GroupIsDocked(UUID idFile, UUID idGroup);
  static bool GroupIsFloating(UUID idFile, UUID idGroup);
  static UUID GroupActiveItem(UUID idFile, UUID idGroup);
  static UUID GroupSetActiveItem(UUID idFile, UUID idGroup, UUID idItem);
  static int GroupItemCount(UUID idFile, UUID idGroup);
  static UUID GroupItemID(UUID idFile, UUID idGroup, int index);
  static UUID GroupItemToolBarID(UUID idFile, UUID idGroup, UUID idItem);

  // Toolbar tab access methods
  static int ToolBarCount(UUID idFile);
  static UUID ToolBarID(UUID idFile, int index);
  static ON_wString ToolBarName(UUID idFile, UUID idToolBar);
  static ON_wString ToolBarName(UUID idFile, UUID idToolBar, unsigned int locale);
  static ON_2iSize TabBitmapSize();
#ifdef RESIABLE_TOOLBAR_AND_PANEL_TABS
  static bool SetTabBitmapSize(const ON_2iSize& size);
#endif
  static ON_2iSize ToolBarBitmapSize();
  static bool SetToolBarBitmapSize(const ON_2iSize& size);

  // Sidebar access methods
  static void ShowSidebar(bool mruSidebar, bool show);
  static bool SidebarIsVisible(bool mruSidebar);

  // Description:
  //  Call this method to get the number of macros in the specified file.
  // Parameters:
  //  idFile[in] File Id to get the macro count from.
  static int MacroCount(UUID idFile);

  // Description:
  //  Call this method when iterating the macro table, it will return the Id
  //  for the specified index.
  // Parameters:
  //  idFile[in] File Id to get the macro Id from.
  //  index[in]  Zero based index of the Id to get.
  // Returns:
  //  Returns ON_nil_uuid on error or the Id for the macro at the specified
  //  index on success.
  static UUID MacroID(UUID idFile, int index);

  // Description:
  //  Call this method to get the Id for the bitmap associated with this macro. 
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  // Returns:
  //  Returns the bitmap Id for the specified macro or ON_nil_uuid on error.
  static UUID MacroBitmapID(UUID idFile, UUID idMacro);

  // Description:
  //  Call this method to get the macro name.
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  //  locale[in] Locale Id of the result string, will default to English if the
  //    locale is not specified in the RUI file.
  // Returns:
  //  Returns the .
  static ON_wString MacroName(UUID idFile, UUID idMacro, unsigned int locale);

  // Description:
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  //  locale[in] Locale Id of the result string, will default to English if the
  //    locale is not specified in the RUI file.
  // Returns:
  //  Returns the .
  static ON_wString MacroHelpText(UUID idFile, UUID idMacro, unsigned int locale);

  // Description:
  //  Call this method to get the status bar help text associated with the specified
  //  macro.
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  //  locale[in] Locale Id of the result string, will default to English if the
  //    locale is not specified in the RUI file.
  // Returns:
  //  Returns the status bar help text associated with the specified macro.
  static ON_wString MacroToolTip(UUID idFile, UUID idMacro, unsigned int locale);

  // Description:
  //  Call this method to get the tool-bar button text associated with the specified
  //  macro.
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  // Returns:
  //  Returns the tool-bar button text associated with the specified.
  static ON_wString MacroButtonText(UUID idFile, UUID idMacro, unsigned int locale);

  // Description:
  //  Call this method to get the menu item text associated with the specified macro.
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  //  locale[in] Locale Id of the result string, will default to English if the
  //    locale is not specified in the RUI file.
  // Returns:
  //  Returns the menu item text associated with the specified macro.
  static ON_wString MacroMenuText(UUID idFile, UUID idMacro, unsigned int locale);

  // Description:
  //  Call this method to get the script associated with the specified macro. 
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  //  fromMenu[in] Set to true if the macro is being run as a result of a menu
  //    item click otherwise; set it to false.
  // Returns:
  //  Returns the script associated with the specified macro.
  static void MacroRunScript(UUID idFile, UUID idMacro, bool fromMenu);

  // Description:
  //  Call this method to get the script associated with the specified macro. 
  // Parameters:
  //  idFile[in] Id of the file containing the macro.
  //  idMacro[in] Id of the specific macro to query.
  // Returns:
  //  Returns the script associated with the specified macro.
  static ON_wString MacroScript(UUID idFile, UUID idMacro);

  // Description:
  //  Call this method to get the number of bitmaps in the specified file.
  // Parameters:
  //  idFile[in] File Id to get the bitmap count from.
  static int BitmapCount(UUID idFile);

  // Description:
  //  Call this method when iterating the bitmap table, it will return the Id
  //  for the specified index.
  // Parameters:
  //  idFile[in] File Id to get the bitmap Id from.
  //  index[in]  Zero based index of the Id to get.
  // Returns:
  //  Returns ON_nil_uuid on error or the Id for the bitmap at the specified
  //  index on success.
  static UUID BitmapID(UUID idFile, int index);

  // Description:
  //  Call this method to get the bitmap macro.
  // Parameters:
  //  idFile[in] File Id
  //  idBitmap[in] Bitmap Id
  //  menuBitmap[in] If true the small bitmap used by custom menu items and
  //    tool bar tabs is returned otherwise; the large bitmap used by buttons
  //    is returned.
  //  bitmap[out] Will contain the extracted bitmap.
  // Returns:
  //  Returns true of the bitmap was found and successfully extracted.
  static bool Bitmap(UUID idFile, UUID idBitmap, bool menuBitmap, CRhinoDib& bitmap);

private:
  CRhinoUiFile() = delete;
  CRhinoUiFile(const CRhinoUiFile&) = delete;
  CRhinoUiFile& operator=(const CRhinoUiFile&) = delete;
};

// CRhinoUiFileEvent Event Id's

// Description:
//  CRhinoUiFileEvent File opened event
//  {5FA9BC7F-D8EF-40FD-91F4-57CFF222CDE2}
RHINO_UILIB_DEFINE_GUID(RHINO_UI_FILE_OPEN_EVENT, 0x5fa9bc7f, 0xd8ef, 0x40fd, 0x91, 0xf4, 0x57, 0xcf, 0xf2, 0x22, 0xcd, 0xe2);

// Description:
//  CRhinoUiFileEvent File closed event
// {2395E5FC-DF89-49BD-A6EE-2EE0001024B5}
RHINO_UILIB_DEFINE_GUID(RHINO_UI_FILE_CLOSED_EVENT, 0x2395e5fc, 0xdf89, 0x49bd, 0xa6, 0xee, 0x2e, 0xe0, 0x0, 0x10, 0x24, 0xb5);

// Description:
//  CRhinoUiFileEvent new macro added event
// {498D441F-0066-48AB-870D-7CCC4B3410A3}
RHINO_UILIB_DEFINE_GUID(RHINO_UI_FILE_MACRO_NEW_EVENT, 0x498d441f, 0x66, 0x48ab, 0x87, 0xd, 0x7c, 0xcc, 0x4b, 0x34, 0x10, 0xa3);

// Description:
//  CRhinoUiFileEvent macro deleted event
// {221ECF6B-0E12-4A3B-AE4B-91E4DE750A5F}
RHINO_UILIB_DEFINE_GUID(RHINO_UI_FILE_MACRO_DELETE_EVENT, 0x221ecf6b, 0xe12, 0x4a3b, 0xae, 0x4b, 0x91, 0xe4, 0xde, 0x75, 0xa, 0x5f);

// Description:
//  CRhinoUiFileEvent existing macro modified event
// {7F30DA3F-54CB-4964-BFFF-0F2879876F85}
RHINO_UILIB_DEFINE_GUID(RHINO_UI_FILE_MACRO_MODIFIED_EVENT, 0x7f30da3f, 0x54cb, 0x4964, 0xbf, 0xff, 0xf, 0x28, 0x79, 0x87, 0x6f, 0x85);
