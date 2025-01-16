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

// File IO helpers

class RHINO_SDK_FUNCTION CRhinoFileUtilities
{
private:
  // Everything here is static, no need to ever construct an instance of this class
  CRhinoFileUtilities() = delete;
public:

  /*
  Description:
    Find a file by looking where it should be and then looking
    in places where a user might have moved it.
  Parameters:
    first_place_to_look - [in]
               1: check the full_path parameter first.
               2: check the relative_path parameter first.
            else: the function decides what is best.
    full_path - [in]
      full path to the file including the file name,
      Pass null if not available.
    relative_path - [in]
      relative path to the file relative to "parent_path" including
      the file name.
      Pass null if not available.
    parent_path - [in]
      full path to the "parent" file, including a file name.
	doc_serial_number - [in]
	  Document serial number to use for relative to model searching.  Pass CRhinoDoc::NullRuntimeSerialNumber to disable searching relative to the model path.
    found_file_path - [out]
      If file exists, its full path name is returned here.
  Returns:
    0: unable to find the file
    1: file was located at the "full_path" location.
    2: file was located at the "relative_path" relative to "parent_path" location
    3: Rhino found a likely candidate in the parent directory,
       model directory, or current working directory, or subdirectory
       of one of these.
  */
  static int FindFile(
	  unsigned int doc_serial_number,
	  int first_place_to_look,
    const wchar_t* full_path,
    const wchar_t* relative_path,
    const wchar_t* parent_path,
    ON_wString& found_file_path
    );

  ON_DEPRECATED_MSG("use overload that takes doc_serial_number parameter")
  static int FindFile(
    int first_place_to_look,
    const wchar_t* full_path,
    const wchar_t* relative_path,
    const wchar_t* parent_path,
    ON_wString& found_file_path
    );

  /*
  Description:
    Get full path to the specified model.  If the model has no name then
    the working folder is returned.
  Parameters:
    doc_serial_number [in] - serial number of the specified model
    result [out] - Buffer to get folder name.
  Returns:
    TRUE if successful otherwise FALSE.
  */
  static bool ModelFolder( unsigned int doc_serial_number, ON_wString& result);

  /*
  Description:
    Get full path to the specified model.  If the model has no name then
    the working folder is returned.
  Parameters:
    doc_serial_number [in] - serial number of the specified model
    result [out] - Buffer to get folder name.
  Returns:
    TRUE if successful otherwise FALSE.
  */
  static bool ModelPath(unsigned int doc_serial_number, ON_wString& result);

  ON_DEPRECATED_MSG("use overload that takes doc_serial_number parameter")
  static bool ModelFolder(ON_wString& result);

  /*
  Description:
    Check for existence of a file.
  Parameters:
    sFileName [in] - File name to validate.
  Returns:
    TRUE if file exists.
  */
  static bool FileExists(const char* sUTF8_FileName);
  static bool FileExists(const wchar_t* sFileName);


  /*
  Description:
    Check to see if a file with the specified name can be written.
  Parameters:
    sFileName [in] - File name to test.
  Returns:
    True if the file can be written.
  Remarks:
    The file may or may not exist.  Use CRhinoFileUtilities::FileExists()
    if you need to know.
  */
  static bool FileIsWritable(const char* sUTF8_FileName);
  static bool FileIsWritable(const wchar_t* sFileName);

  /*
  Description:
    Determine if a file exists and is read-only.
  Parameters:
    sFileName [in]
  Returns:
    True if the file exists and is read-only.
  */
  static bool FileIsReadOnly(const char* sUTF8_FileName);
  static bool FileIsReadOnly(const wchar_t* sFileName);


  /*
  Description:
    Determine if a file exists and is not read-only.
  Parameters:
    sFileName [in]
  Returns:
    True if the file exists and is not read-only.
  */
  static bool FileIsNotReadOnly(const char* sUTF8_FileName);
  static bool FileIsNotReadOnly(const wchar_t* sFileName);

  /*
  Description:
    Determine if a file is a Rhino 3dm file.
  Parameters:
    sFileName [in]
  Returns:
    0: 
      The file is not a Rhino 3dm file.
    >0: 
      The file is a Rhino 3dm file and the returned value is
      the earliest version of Rhino that can read the file.
  */
  static unsigned int FileIsRhino3dm(const char* sUTF8_FileName);
  static unsigned int FileIsRhino3dm(const wchar_t* sFileName);

  /*
  Description:
    Rename a file.
  Parameters:
    sOldFileName - [in]
      Name of an existing file.
    sNewFileName - [in]
      New name for the file. No current file must have this name.
  Returns:
    True if successful.
  */
  static bool RenameFile(
    const char* sUTF8_OldFileName,
    const char* sUTF8_NewFileName
    );
  static bool RenameFile(
    const wchar_t* sOldFileName,
    const wchar_t* sNewFileName
    );

  /*
  Description:
    Move a file.
  Parameters:
    sOldFileName - [in]
      Name of an existing file.
    sNewFileName - [in]
      New name for the file. If a file with this name exists, it is removed.
    bFailIfDestinationExists - [in]
  Returns:
    True if successful.
  */
  static bool CopyFile(
    const char* sUTF8_OldFileName,
    const char* sUTF8_NewFileName,
    bool bFailIfDestinationExists
  );
  static bool CopyFile(
    const wchar_t* sOldFileName,
    const wchar_t* sNewFileName,
    bool bFailIfDestinationExists
  );

  /*
  Description:
    Move a file.
  Parameters:
    sOldFileName - [in]
      Name of an existing file.
    sNewFileName - [in]
      New name for the file. If a file with this name exists, it is removed.
  Returns:
    True if successful.
  */
  static bool MoveFile(
    const char* sUTF8_OldFileName,
    const char* sUTF8_NewFileName
    );
  static bool MoveFile(
    const wchar_t* sOldFileName,
    const wchar_t* sNewFileName
    );

  /*
  Description:
    Replace a file with a temporary file and, optionally, back up the original file.
  Parameters:
    sDestinationFileName - [in]
      The name of the destination file.  This file may or may not exist.
    sTemporaryFileName - [in]
      Name of the temporary file. This file must exist.
    sBackupFileName - [in]
      nullptr for no backup file.
      If sBackupFileName is not empty and a file named sDestinationFileName exists,
      then the original file named sDestinationFileName is renamed sBackupFileName.
  Returns:
   True if successful.
  */
  static bool ReplaceFile(
    const char* sUTF8_DestinationFileName,
    const char* sUTF8_TemporaryFileName,
    const char* sUTF8_BackupFileName
    );

  static bool ReplaceFile(
    const wchar_t* sDestinationFileName,
    const wchar_t* sTemporaryFileName,
    const wchar_t* sBackupFileName
    );


  /*
    Description:
      Delete a file
    Parameters:
      sFileName [in] - File name to validate.
    Returns:
      true if file existed and was deleted.
  */
  static bool DeleteFile( const char* sUTF8_FileName );
  static bool DeleteFile( const wchar_t* sFileName );

  static bool RecycleFile(const wchar_t* sFullPath, bool keepMultipleCopies);

  /*
  Description:
    Compare file names to see if the identify the same file.
  Parameters:
    sFileName1 - [in]
    sFileName2 - [in]
  Returns:
    0: File names identify the same file
    <0: sFileName1 < sFileName2
    >0: sFileName1 > sFileName2
  */
  static int CompareFileNames(
    const char* sUTF8_FileName1,
    const char* sUTF8_FileName2
    );
  static int CompareFileNames(
    const wchar_t* sFileName1,
    const wchar_t* sFileName2
    );

  static bool FileNamesAreEqual(
    const char* sUTF8_FileName1,
    const char* sUTF8_FileName2
    );
  static bool FileNamesAreEqual(
    const wchar_t* sFileName1,
    const wchar_t* sFileName2
    );

  static bool IsValidFileName(
    const char* sUTF8_FileName
    );
  static bool IsValidFileName(
    const wchar_t* sFileName
    );

  /*
  Returns true if path name does not contain the characters listed below
  | : * ? " < >
  */
  static bool IsValidPathName(
    const char* sUTF8_PathName
    );
  static bool IsValidPathName(
    const wchar_t* sPathName
    );

  static bool MakeValidFileName(
    ON_wString& filename
  );

  /*
  Parameters:
    sUTF8FileName - [in]
      filename encoded as a uTF-8 string.
  Returns:
    If sUTF8_FileName has an extension, then a pointer to the initial '.'
    in the extension is returned.
    If sUTF8_FileName does not have an extension, then null_ptr is returned.
  Remarks:
    If sUTF8_FileName ends in .3dm.bak, then ".3dm.bak" is returned.
  See Also:
    on_splitpath()
  */
  static const char* FileExtensionFromFileName(
    const char* sUTF8_FileName
    );

  /*
  Parameters:
    sFileName - [in]
  Returns:
    If sFileName has an extension, then a pointer to the initial '.'
    in the extension is returned.
    If sFileName does not have an extension, then null_ptr is returned.
  Remarks:
    If sFileName ends in .3dm.bak, then ".3dm.bak" is returned.
  See Also:
    on_wsplitpath()
  */
  static const wchar_t* FileExtensionFromFileName(
    const wchar_t* sFileName
    );

  static bool AppendExtensionToFileName(
    const wchar_t* sFileName,
    const wchar_t* sFileExtension,
    ON_wString& sFileNameWithExtension
    );

  /*
  Returns:
    True if sExt is ".3dm", "3dm", ".3dx" or "3dx", ignoring case.
  */
  static bool Is3dmFileExtension(const char* sUTF8_FileExtension);
  static bool Is3dmFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
    True if sExt is ".3dmbak", "3dmbak", ".3dm.bak", "3dm.bak",
    ".3dx.bak" or "3dx.bak", ignoring case.
  */
  static bool Is3dmBackupFileExtension(const char* sUTF8_FileExtension);
  static bool Is3dmBackupFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".rui" or "rui", ignoring case.
  */
  static bool IsRuiFileExtension(const char* sUTF8_FileExtension);
  static bool IsRuiFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".ruibak", "ruibak", ".rui.bak" or "rui.bak",
  ignoring case.
  */
  static bool IsRuiBackupFileExtension(const char* sUTF8_FileExtension);
  static bool IsRuiBackupFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".tb" or "tb", ignoring case.
  */
  static bool IsToolbarFileExtension(const char* sUTF8_FileExtension);
  static bool IsToolbarFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".tbbak", "tbbak", ".tb.bak" or "tb.bak",
  ignoring case.
  */
  static bool IsToolbarBackupFileExtension(const char* sUTF8_FileExtension);
  static bool IsToolbarBackupFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".rws", "rws", ".ws", "ws", ".ws3" or "ws3", ignoring case.
  */
  static bool IsWorkSessionFileExtension(const char* sUTF8_FileExtension);
  static bool IsWorkSessionFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".rwsbak", "rwsbak", ".wsbak", "wsbak", ".ws3bak", "ws3bak",
  ".rws.bak", "rws.bak", ".ws.bak", "ws.bak", ".ws3.bak" or "ws3.bak", ignoring case.

  ignoring case.
  */
  static bool IsWorkSessionBackupFileExtension(const char* sUTF8_FileExtension);
  static bool IsWorkSessionBackupFileExtension(const wchar_t* sFileExtension);

  /*
  Returns:
  True if sExt is ".bak" or "bak", ignoring case.
  */
  static bool IsBakFileExtension(const char* sUTF8_FileExtension);
  static bool IsBakFileExtension(const wchar_t* sFileExtension);


  /*
    Description:
      Check for existence of a directory.
    Parameters:
      lpsFileName [in] - Directory name to validate.
    Returns:
      TRUE if directory exists.
  */
  static bool DirExists(const char* lpsFileName);
  static bool DirExists(const wchar_t* lpsFileName);

#if defined (ON_RUNTIME_APPLE)
  /*
   Description:
   Given a file path, check for existence of the file's containing directory.
   Parameters:
   lpsFileName [in] - File path to validate.
   Returns:
   TRUE if directory exists.
   */
  static BOOL32 DirExistsForFile( const wchar_t* lpsFileName);

  /*
   Description:
   Remove all directory components and leave only the file name.
   Parameters:
   lpsFileName  [in/out] - File name
   */
  static void StripPath (ON_wString& lpsFileName);
  
  static NSString* RhinoFrameworksPath();
  static NSString* RhinoPluginsPath();
  static NSString* RhinoResourcesPath();
#endif

  /*
  Description:
    Find a file from an ON_FileReference.
  Parameters:
    doc_serial_number - [in]
    base_path - [in]
      Typically the location of the file where the current model is saved.
      Pass nullptr or empty string if you don't have a solid candidate
      for a base path. 
      When file_reference.FullPath() does not exists or has changed content,
      base_path is combined with file_reference.RelativePath() to search for
      the most appropriate file.
    bBasePathIncludesFileName - [in]
      True if the base_path parameter includes a file name (that will be ignored)
    file_reference - [in]
    full_path_to_found_file - [out]
      full path to found file or empty.
  Returns:
    True if file was found.    
  */
  static bool FindFile(
    unsigned int doc_serial_number, 
    const wchar_t* base_path,
    bool bBasePathIncludesFileName,
    const ON_FileReference& file_reference,
    ON_wString& full_path_to_found_file
    );

  static bool FindFile(
    unsigned int doc_serial_number, 
    const ON_FileReference& file_reference,
    ON_wString& full_path_to_found_file
    );

  /*
  Description:
    Find a file from an ON_FileReference.
  Parameters:
    doc_serial_number - [in]
    base_path - [in]
      Typically the location of the file where the current model is saved.
      Pass nullptr or empty string if you don't have a solid candidate
      for a base path. 
      When file_reference.FullPath() does not exists or has changed content,
      base_path is combined with file_reference.RelativePath() to search for
      the most appropriate file.
    bBasePathIncludesFileName - [in]
      True if the base_path parameter includes a file name (that will be ignored)
    file_reference - [in/out]
      If the file is found, file_reference.FullPath() is set to the found file.
    bUpdateFileContentHash - [in]
      If a file is found, and bUpdateFileContentHash is true, then
      the file_reference content hash is set from the found file.
  Returns:
    True if file was found.    
  */
  static bool FindFileAndUpdateReference(
    unsigned int doc_serial_number, 
    const wchar_t* base_path,
    bool bBasePathIncludesFileName,
    ON_FileReference& file_reference,
    bool bUpdateFileContentHash
    );

  /*
    Description:
      Search for file using Rhino's search path.  Rhino will look in the
      following places:
        1. Specified model folder
        2. Path specified in options dialog/File tab
        3. Rhino system folders
        4. Rhino executable folder
    Parameters:
	    doc_serial_number - [in]
        Document serial number to use for relative to model searching.  
        Pass CRhinoDoc::NullRuntimeSerialNumber to disable searching relative to the model path.
      lpsFileName - [in]
        File to find
      result - [out]
        Full path to location of file	 
    Returns:
      true if file is found.
  */
  static bool FindFile(
    unsigned int doc_serial_number, 
    const wchar_t* lpsFileName,
    ON_wString& result);
  
  static bool FindFile(
    unsigned int doc_serial_number, 
    const char* lpsFileName, 
    ON_wString& result);

  ON_DEPRECATED_MSG("use overload that takes doc_serial_number parameter")
  static bool FindFile(const wchar_t* lpsFileName, ON_wString& result);
  ON_DEPRECATED_MSG("use overload that takes doc_serial_number parameter")
  static bool FindFile(const char* lpsFileName, ON_wString& result);
  /*
    Description:
      Create a unique temporary file name.
    Parameters:
      result [out] - File name output to this object.
    Returns:
      TRUE if successful.
  */
  static BOOL32 MakeUniqueFilename( ON_wString& result);
  /*
    Description:
      Rename existing file while you attempt replace it.
    Parameters:
      filename [in]  - Name of file to rename.
      templane [out] - Name of temp file created
    Returns:
      TRUE on success.
  */
  static BOOL32 BackupExistingFile( const wchar_t* filename, ON_wString& tempname);
  /*
    Description:
      Rename <tempname> as <filename>.  If <filename> exists then it is deleted and
      <tempname> is renamed.
    Parameters:
      filename [in] - Original file name
      tempname [in] - Temporary backup file to rename
    Returns:
  */
  static BOOL32 RestoreBackupFile( const wchar_t* filename, const wchar_t* tempname);

  /*
    Description:
      Get Special Folder Paths from such as "My Documents", "Application Data", "Network Neighborhood", etc...

      A wrapper around Microsoft's ShFolder.dll implementation of SHGetFolderPath. The ShFolder.dll
      implementation works consistently across all platforms (but the ShFolder.dll implementation does not).
    Parameters:
      result [out] - Path placed here.
      nFolder [in] - CSIDL constant as described in Microsoft's SHGetFolderPath documentation.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static BOOL32 GetWindowsSpecialFolderPath( int nFolder, ON_wString& result );

  /*
    Description:
      Get full path to "My Documents" folder
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static BOOL32 GetMyDocumentsFolder( ON_wString& result);
  
  /*
    Description:
      Get full path to Rhino Application Data folder. This is the folder where
      Rhino stores global data that is shared between all users on the system.
      
      Windows 95, 98, ME: usually someplace like "C:\Program Files\Rhinoceros"
      Windows NT4, 2000, XP: usually someplace like "C:\Documents and Settings\All Users\Application Data\McNeel\Rhinoceros\5.0\"
      Windows Vista, 7: usually someplace like "C:\ProgramData\McNeel\Rhinoceros\5.0\"
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static BOOL32 GetRhinoApplicationDataFolder( ON_wString& result);

  /*
  Description:
    Get full path to Rhino Application Data languagefolder. This is the folder where
    Rhino stores global language data that is shared between all users on the system.
    Usually someplace like "C:\ProgramData\McNeel\Rhinoceros\8.0\Localization\en-US"
  Parameters:
    result [out] - Path placed here.
  Returns:
    If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static BOOL32 GetRhinoApplicationDataLanguageFolder(ON_wString& result);

  /*
    Description:
      Get full path to a Rhino 5.0 specific sub-folder under the per-user Roaming Profile folder. 
      This is the folder where user-specific data is stored.
      
      Windows NT4, 2000, XP: usually someplace like "C:\Documents and Settings\[USERNAME]\Application Data\McNeel\Rhinoceros\5.0\"
      Windows Vista, 7: usually someplace like "C:\Users\[USERNAME]\AppData\Roaming\McNeel\Rhinoceros\5.0\"
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static bool GetRhinoRoamingProfileDataFolder( ON_wString& result);

  /*
    Description:
      Get full path to a Rhino 5.0 specific sub-folder under the per-user Roaming Profile folder. 
      This is the folder where user-specific temporary files are created.

      If a file in this directory is more than 4 days old, it will be deleted when Rhino starts.
      
      Windows NT4, 2000, XP: usually someplace like "C:\Documents and Settings\[USERNAME]\Application Data\McNeel\Rhinoceros\5.0\...\Temp\"
      Windows Vista, 7: usually someplace like "C:\Users\[USERNAME]\AppData\Roaming\McNeel\Rhinoceros\5.0\...\Temp\"
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static bool GetRhinoRoamingProfileTempFolder( ON_wString& result);


  /*
    Description:
      Get full path to the Rhino 8.0 default template folder for a given language.
    Parameters:
      result [out] - Path placed here.
      languageid [in] - Language to retrieve
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static bool GetDefaultTemplateFolder(ON_wString& result, int languageid);

  /*
    Description:
      Get full path to a Rhino 5.0 specific sub-folder under the per-user Roaming Profile folder where
      localized resources are stored. This folder changes for each language.

      Windows NT4, 2000, XP: usually someplace like "C:\Documents and Settings\[USERNAME]\Application Data\McNeel\Rhinoceros\5.0\Localization\en"
      Windows Vista, 7: usually someplace like "C:\Users\[USERNAME]\AppData\Roaming\McNeel\Rhinoceros\5.0\Localization\en"
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static bool GetRhinoRoamingProfileCurrentLanguageFolder( ON_wString &result);

  /*
    Description:
      Get full path to a Rhino 5.0 specific sub-folder under the per-user Local (non-roaming) Profile folder. 
      This is the folder where user-specific data is stored.
      
      Windows NT4, 2000, XP: usually someplace like "C:\Documents and Settings\[USERNAME]\Local Settings\Application Data\McNeel\Rhinoceros\5.0\"
      Windows Vista, 7: usually someplace like "C:\Users\[USERNAME]\AppData\Local\McNeel\Rhinoceros\5.0\"
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static bool GetRhinoLocalProfileDataFolder( ON_wString& result);

  /*
    Description:
      Get full path to a Rhino 5.0 specific sub-folder under the per-user Roaming Profile folder where
      localized resources are stored. This folder changes for each language.

      Windows NT4, 2000, XP: usually someplace like "C:\Documents and Settings\[USERNAME]\Local Settings\Application Data\McNeel\Rhinoceros\5.0\Localization\en"
      Windows Vista, 7: usually someplace like "C:\Users\[USERNAME]\AppData\Local\McNeel\Rhinoceros\5.0\Localization\en"
    Parameters:
      result [out] - Path placed here.
    Returns:
      If Rhino could figure out the path then TRUE is returned otherwise FALSE is.
  */
  static bool GetRhinoLocalProfileCurrentLanguageFolder( ON_wString &result);

  /*
    Description:
      Gets the full path to the default .rui file
    Parameters:
      sValue: the output value that contains the full path.
    Returns:
      true if the default RUI file was found
      false otherwise
  */
  static bool GetDefaultRuiFilename(ON_wString &sValue);
  /*
    Description:
      Compare two path names ignoring case making ignoring the trailing L'\\' if it
      exists on either name.
    Parameters:
      p1 [in] - First path to compare
      p2 [in] - Second path to compare
    Returns:
      If p1  < p2, returns -1
      If p1 == p2, returns  0
      If p2  < p1, returns  1
  */
  static int ComparePaths( const wchar_t* p1, const wchar_t* p2);

  /*
    Description:
      Compare the two existing files at the specified paths to see if they are the same file (not equal or similar - but that the paths resolve to the same location)
      For example, this function can determine if a UNC path and a mapped drive resolve to the same location, or if a UNC path specified with an IP address maps
      to the same location as one with a netbios name.
    Parameters:
      path1 [in] - First path to file
      path2 [in] - Second path to file
    Returns:
      true if the paths resolve to the same location
      false if they don't.
  */
  static bool IsSameFile(const ON_wString& path1, const ON_wString& path2);

  /*
    Description:
      Create full path to file using directory and extension if supplied.  Is smart
      enough to figure out if it needs to add the '\' between directory and file name.
    Parameters:
      result [out] - Complete path placed here
      lpsDirectory [in] - Directory name with or without trailing '\'
      lpsFileName  [in] - File name
      lpsExtension [in] - File extension with or without '.'
    Returns:
      Returns TRUE if lpsDirectory and lpsFile name are not NULL or empty strings
  */
  static BOOL32 MakeFullPath( ON_wString& result, const wchar_t* lpsDirectory, const wchar_t* lpsFileName, const wchar_t* lpsExtension = 0);
  

  /*
    Description:
      Check to see if two directories reference the same folder. This function does not resolve wildly different
      paths, such as: C:\vol1\myfolder and \\myserver\vol1\myfolder
    Parameters:
      dir1, dir2: directories to compare
    Returns:
      true if directories match, false otherwise
  */
  static bool AreDirectoriesTheSame( const wchar_t *dir1, const wchar_t *dir2);

  /*
    Description:
      Prompt user for directory name.  If bShowDialog is TRUE then displays directory dialog
      complete with edit box which allows user to create a new directory.
    Parameters:
      wDir [in]  - Default folder.
           [out] - Folder selected.
      bShowDialog [in] - If true use dialog otherwise use CRhinoGetString
      hWndParent  [in] - If bShowDialog is TRUE then it is used as the parent window
                         for the directory dialog.  If it is NULL then the main Rhino
                         window handle is used.
    Returns:
      CRhinoCommand::failure - If bShowDialod is true then there was an error displaying dialog
                               otherwise user typed in an invalid directory name.
      CRhinoCommand::cancel  - User pressed cancel button in dialog or escape when prompted for string.
      CRhinoCommand::nothing - User pressed enter and wDef did not change.
      CRhinoCommand::success - Valid directory name entered.
  */
  static CRhinoCommand::result GetDirectory( ON_wString& wDir, BOOL32 bShowDialog = TRUE, HWND hWndParent = NULL);
  /*
    Description:
      Call this method to find out if the specified file or directory name is an absolute path and not
      a relative one.
    Parameters:
      lps [in]  - File or directory name to test
    Returns:
      Returns true if lps is absolute which means it is prefixed with a drive letter or "\\" otherwise returns false
  */
  static bool PathIsAbsolute( const wchar_t* lps);

  /*
  Description:
    Call this method to create a directory on the file system.
  Parameters:
    lpFullPath [in]          - Full path to the directory to create.
  Returns:
    true when: 
       directory already exists
       or if directory creation succeeds
    false when:
       lpFullPath is null
       lpFullPath is empty
       directory creation fails
       lpFullPath is a relative path
  */
  static bool CreateDir( const wchar_t* lpFullPath);

  /*
  Description:
    Call this method to remove an empty directory on the file system.
  Parameters:
    lpFullPath [in]          - Full path to the empty directory to remove.
  Returns:
    true when:
       directory already removed
       or if directory removal succeeds
    false when:
       lpFullPath is null
       lpFullPath is empty
       directory removal fails
       lpFullPath is a relative path
  */
  static bool RemoveDir(const wchar_t* lpFullPath);

  /*
    Description:
      Call this method to convert an absolute path to a path relative to another file name or folder.
    Parameters:
      lpsFrom [in]               - File or directory name to convert.
      bFromIsFileName [in]       - If true then lpsFrom is treated as a file name otherwise it is treated as a directory name
      lpsRelativeTo [in]         - File or folder you whish to make lpsFrom relative to
      bRelativeToIsFileName [in] - If true then lpsRelativeTo is treated as a file name otherwise it is treated as a directory name
      wResult [out]              - Reference to ON_wString which will receive the computed relative path
    Returns:
      Returns true if parameters are valid and lpsFrom is indeed relative to lpsRelativeTo otherwise returns false
  */
  static bool PathRelativePathTo( const wchar_t* lpsFrom, bool bFromIsFileName, const wchar_t* lpsRelativeTo, bool bRelativeToIsFileName, ON_wString& wResult);
  /*
    Description:
      Call this method to convert a relative path to an absolute path relative to the specified path.
    Parameters:
      lpsRelativePath [in]         - Relative path to convert to an absolute path
      bRelativePathisFileName [in] - If true then lpsFrom is treated as a file name otherwise it is treated as a directory name
      lpsRelativeTo [in]           - File or folder the path is relative to
      bRelativeToIsFileName [in]   - If true then lpsFrom is treated as a file name otherwise it is treated as a directory name
      wResult [out]                - Reference to ON_wString which will receive the computed absolute path
    Returns:
      Returns true if parameters are valid and lpsRelativePath is indeed relative to lpsRelativeTo otherwise returns false
  */
  static bool PathAbsoluteFromRelativeTo( const wchar_t* lpsRelativePath, bool bRelativePathisFileName,
                                          const wchar_t* lpsRelativeTo, bool bRelativeToIsFileName,
                                          ON_wString& wResult
                                        );


};

