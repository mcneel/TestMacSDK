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

class RHINO_SDK_CLASS CRhinoDirectoryManager
{
  // See CRhinoApp for the one and only CRhinoDirectoryManager object
  CRhinoDirectoryManager(); // only CRhinoApp can create this class
public:

  enum FileTypes
  {
    ftOpen = 0,
    ftSave = ftOpen,
    ftImport,
    ftExport,
    ftTemplate,
    ftBitmap,
    ftRender,
    ftScreenCapture = ftRender,
    ftSaveDisplay = ftRender,
    ftTextures,
    ftBump,
    ftToolbarCollection,
    ftToolbarBitmapExport,
    ftToolbarBitmapImport = ftToolbarBitmapExport,
    ftCommandFile,
    ftRun,
    ftReadFile,
    ftCommandAlias,
    ftPlugIn,
    ftRhino3dmFile,
    ftWorksession,
    ftAttachReferenceFile,
    ftCount, // Leave this at the end, it specifies array length
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_file_types = 0xFFFFFFFF
  };
  /*
    Description:
      Get default directory for various file I/O operations
    Parameters:
      ft [in] - File operation
      result [out] - Receives default directory name
    Returns:
      Returns TRUE if ft is valid otherwise returns FALSE.
  */
  BOOL32 GetDefaultDirectory( FileTypes ft, ON_wString& result);
  /*
    Description:
      Set default directory for various file I/O operations
    Parameters:
      ft [in] - File operation
      lpsDirectory [in] - New default directory for specified file operations
    Returns:
      Returns TRUE ft is valid and something placed in result.
  */
  BOOL32 SetDefaultDirectory( FileTypes ft, const wchar_t* lpsDirectory);
  /*
    Description:
      Set default directory for various file I/O operations.
    Parameters:
      ft [in] - File operation
      lpsFileName [in] - MUST be a fully qualified path.  Strips file name from end
                         and calls SetDefaultDirectory
    Returns:
      Returns TRUE ft is valid and lpsFileName is not NULL or an empty string.
  */
  BOOL32 SetDefaultDirectoryFromFileName( FileTypes ft, const wchar_t* lpsFileName);
  /*
    Description:
      Extract directory name from complete file name.
    Parameters:
      lpsFileName [in] - File name to parse.
      dir [out] - Will contain directory name.
    Returns:
      Returns TRUE lpsFileName is not NULL, empty and is a full path.
  */
  BOOL32 DirectoryFromFileName( const wchar_t* lpsFileName, ON_wString& dir);
  /*
    Description:
      Sets all file operations to the same default folder
    Parameters:
      lpsDirectory [in] - New default directory for all file operations
  */
  void SetWorkingDirectory( const wchar_t* lpsDirectory);
  /*
    Description:
      Number of user directories CRhinoFileUtilities::FindFile will look in.
    Returns:
      Returns number of user folders CRhinoFileUtilities::FindFile will search when
      looking for files.
  */
  int SearchPathCount() const;
  /*
    Description:
      Iterate user directories CRhinoFileUtilities::FindFile will look in.
    Parameters:
      i [in] - Directory index
    Returns:
      NULL if index is out of range otherwise full path to folder
  */
  const wchar_t* SearchPath( int i) const;
  /*
    Description:
      Remove user directory from CRhinoFileUtilities::FindFile
    Parameters:
      i [in] - Index of directory to delete.
    Returns:
  */
  void DeleteSearchPath( int i);
  /*
    Description:
      Add directory to end of search path.
    Parameters:
      lps [in] - Directory to add.
    Returns:
      Index of new directory or -1 if an error occurred.
  */
  int AppendSearchPath( const wchar_t* lps);
  /*
    Description:
      Insert directory to into search path
    Parameters:
      i [in] - Position in list to insert new directory.
      lps [in] - Directory to add.
    Returns:
      TRUE if index in range and lps points to a valid directory.
  */
  BOOL32 InsertSearchPath( int i, const wchar_t* lps);

  // Helper function for internal use
  FileTypes ConvertRhinoGetFileDialogDialogType( int ft);

  /*
    Description:
      Returns key which can be used with RhinoApp().m_PC reading and
      writing functions.
    Returns:
      Key which can be used with RhinoApp().m_PC reading and writing functions.
  */
  const wchar_t* SearchPathKey() const;
  /*
    Description:
      Returns entry which can be used with RhinoApp().m_PC reading and
      writing functions.
    Returns:
      Entry which can be used with RhinoApp().m_PC reading and writing functions to
      get the number of custom search paths saved.
  */
  const wchar_t* SearchPathCountEntry() const;
  /*
    Description:
      Returns entry which can be used with RhinoApp().m_PC reading and
      writing functions.
    Returns:
      Entry which can be used with RhinoApp().m_PC reading and writing functions to
      get enumerate custom search path registry entries.  The first entry will be 
      SearchPathEntryPrefix() + L"00" up to count - 1;
  */
  const wchar_t* SearchPathEntryPrefix() const;

  void LoadProfile( CRhinoProfileContext& PC );
  void SaveProfile( CRhinoProfileContext& PC );

protected:
#if defined (ON_RUNTIME_APPLE)
public:
#endif
  friend class CRhinoApp;
  void Initialize();
  void Save();       // Should only be called once by CRhinoApp::ExitInstance()
  const wchar_t* Key();
  const wchar_t* Entry( FileTypes);

protected:
  ON_wString                m_dir_list[ftCount];
  ON_ClassArray<ON_wString> m_search_path;

private: // do not change private to any other word!
  // Parameters:
  //   filename = Command line name of a file used for opening Rhino. Can be empty
  void Init(const wchar_t* filename); // Should only be called once by CRhinoApp::InitInstance()

};
