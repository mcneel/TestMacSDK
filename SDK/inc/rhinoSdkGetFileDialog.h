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

#include "rhinoSdkCoreClasses.h"
#include "rhinoSdkFileOptions.h"

/*
Description:
  Find the extension of the filename passed in as sPath.
  This function is required to properly handle the name.ext.bak
  files Rhino 4 and early beta versions of Rhino 5 created.
Parameters:
  sPath - [in]
    A null terminated string that contains the path to search.
Returns
  * If the sPath parameter is null, then a null is returned.
  * If no extension is found, then a null is returned (this
    is different than Windows' PathFindExtension() which
    returns a pointer to the null terminator in sPath.
  * If the sPath parameter is not a null and a valid extension
    exists, then the address of the "." that precedes the extension
    is returned.
Remarks:
  The RhinoPathFindFileExtension() function works like the
  as Windows' PathFindExtension(), with three exceptions: 
  1) RhinoPathFindFileExtension() will return ".ext.bak" when
     sPath is "filename.ext.back" while Windows' PathFindExtension()
     will return ".bak".
  2) RhinoPathFindFileExtension() returns null in all other
     cases.
  3) RhinoPathFindFileExtension() dow not limit path names
     to MAX_PATH characters.

  Note: Valid Windows extensions cannot contain spaces.
*/
RHINO_SDK_FUNCTION
const wchar_t* RhinoPathFindFileExtension( const wchar_t* sPath );

/*
Parameters:
  sPath - [in] name of file.
Returns:
  True if the file name ends in ".3dmbak" or ".3dm.bak".
*/
RHINO_SDK_FUNCTION
bool RhinoIs3dmBackupFileName( const wchar_t* sPath );

/*
Parameters:
  sExtension - [in] file extension beginning with a "."
Returns:
  True if sExtension is ".3dmbak" or ".3dm.bak".
*/
RHINO_SDK_FUNCTION
bool RhinoIs3dmBackupFileExtension( const wchar_t* sExtension );

/*
Description:
  Determine if core Rhino or a file import plug-in
  can write files with the specified extension.
Parameters:
  sFileName - [in]
    Full file path or file name.
  file_type_id - [out]
    nil: 
      Nothing in Rhino can read files with this extension.
    RhinoApp().RhinoCurrent_UUID(): 
      The file extension is .3dm, .3dmbak or .3dm.bak.
    Otherwise: 
      Id of a file import plug-in that reads files with
      the specified extension.
  file_type_index - [out]
    -1:
      Nothing in Rhino can read files with this extension.
    >=0:
      File type index for this file.
*/
RHINO_SDK_FUNCTION
bool RhinoGetFileReaderIdFromFileName(
  const wchar_t* sFileName,
  ON_UUID& file_type_id,
  int& file_type_index
  );

/*
Description:
  Determine if core Rhino or a file import plug-in
  can write files with the specified extension.
Parameters:
  sFileNameExtension - [in]
    FIle extension (leading "." is optional).
  file_type_id - [out]
    nil: 
      Nothing in Rhino can read files with this extension.
    RhinoApp().RhinoCurrent_UUID(): 
      The file extension is .3dm, .3dmbak or .3dm.bak.
    Otherwise: 
      Id of a file import plug-in that reads files with
      the specified extension.
  file_type_index - [out]
    -1:
      Nothing in Rhino can read files with this extension.
    >=0:
      File type index for this file.
*/
RHINO_SDK_FUNCTION
bool RhinoGetFileReaderIdFromFileNameExtension(
  const wchar_t* sFileNameExtension,
  ON_UUID& file_type_id,
  int& file_type_index
  );

/*
Description:
  Determine if core Rhino or a file import plug-in
  can write files with the specified extension.
Parameters:
  sFileName - [in]
    Full file path or file name.
  file_type_id - [out]
    nil: 
      Nothing in Rhino can read files with this extension.
    RhinoApp().RhinoCurrent_UUID(): 
      The file extension is .3dm, .3dmbak or .3dm.bak.
    Otherwise: 
      Id of a file import plug-in that reads files with
      the specified extension.
  file_type_index - [out]
    -1:
      Nothing in Rhino can read files with this extension.
    >=0:
      File type index for this file.
*/
RHINO_SDK_FUNCTION
bool RhinoGetFileWriterIdFromFileName(
  const wchar_t* sFileName,
  ON_UUID& file_type_id,
  int& file_type_index
  );RHINO_SDK_FUNCTION

/*
Description:
  Determine if core Rhino or a file import plug-in
  can write files with the specified extension.
Parameters:
  sFileNameExtension - [in]
    FIle extension (leading "." is optional).
  file_type_id - [out]
    nil: 
      Nothing in Rhino can read files with this extension.
    RhinoApp().RhinoCurrent_UUID(): 
      The file extension is .3dm, .3dmbak or .3dm.bak.
    Otherwise: 
      Id of a file import plug-in that reads files with
      the specified extension.
  file_type_index - [out]
    -1:
      Nothing in Rhino can read files with this extension.
    >=0:
      File type index for this file.
*/
bool RhinoGetFileWriterIdFromFileNameExtension(
  const wchar_t* sFileNameExtension,
  ON_UUID& file_type_id,
  int& file_type_index
  );

class RHINO_SDK_CLASS CRhinoGetFileDialog  
{
public:
	CRhinoGetFileDialog();
	virtual ~CRhinoGetFileDialog();

  enum file_dialog_type
  {
    open_dialog = 0,
    open_template_dialog,
    open_bitmap_dialog,
    open_rhino_3dm_file_dialog, // name of a rhino .3dm file 
    open_toolbar_collection_dialog,
    open_text_file_dialog,
    open_worksession_dialog,
    import_dialog,
    attach_dialog,
    load_plug_in_dialog,
    save_dialog,           // Rhino model Save command
    save_small_dialog,
    save_template_dialog,
    save_bitmap_dialog,
    export_dialog,
    save_rhino_3dm_file_dialog, // name of a rhino .3dm file (current version)
    save_toolbar_collection_dialog,
    save_text_file_dialog,
    save_worksession_dialog,
    save_current_toolbar_collection_only_dialog,
#if defined (ON_RUNTIME_APPLE)
    open_python_file_dialog, // 05 Mar 2010 S. Baer - added to allow for filtering on python script files
#endif
    open_hatchpattern_dialog, //RH-33812 - 28 Apr, 2016 - Lowell
    open_linetype_dialog, // RH - 33812 - 28 Apr, 2016 - Lowell
    save_pdf,
    save_xps,
    custom, // RH-50534 - 28 Jan 2019, John Croudy.
    save_rendering_dialog,
    save_csv_dialog,
    // This value is for internal use only. It will change if more
    // items are added. Never use this value in your plug-in.
    file_dialog_type_internal_numof,

    file_dialog_type_count = custom, // For backward compatibility only. DO NOT USE. Please change your code to use 'custom'.
  };

  enum bitmap_file_types
  {
    bmp = 1,
    jpg = 2,
    pcx = 4,
    png = 8,
    tif = 16,
    tga = 32,
#if defined (ON_RUNTIME_WIN) || defined(ON_RUNTIME_LINUX)
    all_types = 0xFFFFFFFF,
#endif
#if defined (ON_RUNTIME_APPLE)
    all_types = bmp | jpg | png | tif,
#endif
  };

  /*
  Description:
    This function is will show just about any kind of Rhino related file
    dialog.  It's a poor design because the context of the dialogs varies
    widely and different dialogs need different information to work well.
  Parameters:
    fileType - [in]
    defaultName - [in]
    pWndParent - [in]
  */
  bool DisplayFileDialog( 
    file_dialog_type fileType,
    const wchar_t* defaultName = nullptr, 
    HWND parentWnd = nullptr
    );

  const wchar_t* FileName() const;

  /*
  Returns:
    True if the file is a current Rhino version 3dm file.
  */
  bool FileTypeIsCurrentRhino3dmVersion() const;

  /*
  Returns:
    True if the file is a previous Rhino version 3dm file.
  */
  bool FileTypeIsEarlierRhino3dmVersion() const;

  /*
  Returns:
    The version of the 3dm file or 0 if it's not a 3dm file
      0: not a Rhino 3dm file
      2 for Rhino V2 3dm file,
      3 for Rhino V3 3dm file,
      4 for Rhino V4 3dm file,
      5 for Rhino V5 3dm file,
      6 for Rhino V6 3dm file,
      ...
  */
  unsigned int FileTypeRhino3dmVersion() const;

  BOOL32 ShowPreview() const;
  BOOL32 SetShowPreview( BOOL32);

  int BitmapTypes() const;
  int SetBitmapTypes( int);

  BOOL32 AddPlugInFileTypes();
  BOOL32 SetAddPlugInFileTypes( BOOL32 b);

  // Returns ON_max_uuid if unset.
  // If it returns ON_nil_uuid, then the file type is a Rhino 3dm file.
  UUID PlugIn() const;

  int PlugInExtensionIndex() const;
 
  BOOL32 UseAsDeafultTemplate() const;
  void SetUseAsDefaultTemplate( BOOL32 b);

  CRhinoFileReadOptions FileReadOptions() const;
  CRhinoFileWriteOptions FileWriteOptions() const;
  void SetFileWriteOptions(CRhinoFileWriteOptions&);

  void SetScriptMode( BOOL32 bInScriptMode);

  static const wchar_t* GetDefaultDirectory( file_dialog_type ft);
  static BOOL32 SetDefaultDirectory( file_dialog_type ft, const wchar_t* lps);

  const wchar_t* GetDefaultDirectory() const;
  void SetDefaultDirectory( const wchar_t* lpsDir);

  void SetDialogTitle( const wchar_t* lpTitle );
  /*
  Description:
    This is only meaningful when scripting the file open command
    and the document setting in the file being read indicates
    a interactive "prompt" should be used to determine if a
    linked and embedded instance definition should be updated.
  Parameters:
    b - [in] true means a linked and embedded instance definitions
             will update when file open is done and the document
             settings indicate a "prompt" should be used to
             get the answer to the question.
  See Also:
    CRhFileDialog::UpdateEmbededInstanceDefinitions
  Remarks:
    Added 3 March 2006 by John Morse so we could add command
    line option that would open files and let the user specify
    what to do with embedded instance definitions.
  */
  void EnableUpdateEmbededInstanceDefinitions( bool b );

  /*
  Description:
    If the update linked and embedded instance definitions
    setting is set to "prompt" and a file is read in
    "script" mode with forbids asking questions, then the
    the value returned by this function is used to determine
    if a linked and embedded instance definition should be
    updated.
  Returns:
    true if definitions are to be updated in scripting mode.
  See Also:
    CRhFileDialog::EnableUpdateEmbededInstanceDefinitions
  Remarks:
    Added 3 March 2006 by John Morse so we could add command
    line option that would open files and let the user specify
    what to do with embedded instance definitions.
  */
  bool UpdateEmbededInstanceDefinitions() const;

  /*
  Description:
    The "Transparent background" check box will be added to the
    File dialog if this value is true.
  Returns:
    true if the "Transparent background" check box will appear in
    the file dialog.
  See Also:
    CRhFileDialog::SetAddTransparentBackgroundControl
  Remarks:
    Added 16 October 2012 by John Morse so we could add the option
    when saving a view to a file.
  */
  bool AddTransparentBackgroundControl() const;
  /*
  Description:
    The "Transparent background" check box will be added to the
    File dialog if this value is true.
  Returns:
    Returns previous value of this flag.
  See Also:
    CRhFileDialog::AddTransparentBackgroundControl
  Remarks:
    Added 16 October 2012 by John Morse so we could add the option
    when saving a view to a file.
  */
  bool SetAddTransparentBackgroundControl(bool value);
  /*
  Description:
    Check to see if the "Transparent background" check box was checked
    when the dialog closed.
  Returns:
    Returns true if the "Transparent background" check box was checked.
  See Also:
    CRhFileDialog::SetTransparentBitampBackgroundChecked
    CRhFileDialog::AddTransparentBackgroundControl
  Remarks:
    Added 16 October 2012 by John Morse so we could add the option
    when saving a view to a file.
  */
  bool TransparentBitampBackgroundChecked() const;
  /*
  Description:
    Sets the default value for the "Transparent background" check box
    if it is displayed.
  Returns:
    Returns previous value of this flag.
  See Also:
    CRhFileDialog::AddTransparentBackgroundControl
    CRhFileDialog::AddTransparentBackgroundControl
  Remarks:
    Added 16 October 2012 by John Morse so we could add the option
    when saving a view to a file.
  */
  bool SetTransparentBitampBackgroundChecked(bool value);

  void SetMinimumRhino3dmVersion(
    unsigned int min_rhino_3dm_version
    );

  unsigned int MinimumRhino3dmVersion() const;

  static void ClearFileTypeDefaultExtension(
    CRhinoGetFileDialog::file_dialog_type ft
    );

#if defined (ON_RUNTIME_APPLE)
  ON_wString MacMessageString;
#endif
  
protected:
  BOOL32 BuildExtensionList( file_dialog_type);
  int  GetDefaultFilterIndex( file_dialog_type) const;
  const wchar_t* GetDefaultFileExtension( file_dialog_type) const;
  void SetDefaultFileExtensionFromFileName( file_dialog_type, const wchar_t* file_name);
  void GetPlugInExtensionIndex( class CRhFileDialog*);
  bool ScriptMode() const;

  INT_PTR DoModal(CRhinoGetFileDialog::file_dialog_type ft, const wchar_t* lpsDefName, class CRhFileDialog& dlg);

  ON_wString                   m_wFileName;


  // If m_min_rhino_3dm_version > 0, then only a RHino 3dm file of that version
  // or greater can be saved on plug-in formats are not supported.
  unsigned int m_min_rhino_3dm_version; // default is 0 

  // If RhinoApp().IsRhinoUUID(m_PlugIn) is true,
  //   then (RHINO_CURRENT_FILE_VERSION_NUMBER-m_nExtensionIndex)
  //   identifies the version of the Rhino .3dm file.
  // If RhinoApp().IsRhinoUUID(m_PlugIn) is false,
  //   then a plug-in is reading/writing the file type and
  //   m_nExtensionIndex is interpreted by the plug-in.
  UUID                         m_PlugIn;
  int                          m_nExtensionIndex;


  BOOL32                         m_bShowPreview;    // Defaults to true
  BOOL32                         m_bAddPlugInTypes; // Defaults to true
  BOOL32                         m_bUseAsDefault;   // Use as default template
  int                          m_types;           // Defaults to all_types
  CRhinoFileReadOptions        m_fro;
  CRhinoFileWriteOptions       m_fwo;
  CRhinoFileWriteOptions       m_fwo_in;
  BOOL32                         m_bUseFileDialogs;
  // 16 October 2012 by John Morse changed this from a BOOL32 to a ON__UINT32
  // and changed it to a flags field, this was done to add additional options
  // without breaking the SDK.  Call the ScriptMode() of function to get the
  // determine if in scripting mode or call SetScriptMode() set set the value.
  //BOOL32                         m_bScriptMode;
  ON__UINT32                   m_Flags;
  ON_ClassArray<CRhinoFileType>m_extensions;
  ON_wString                   m_default_dir;
  ON_wString                   m_dialog_title;
  bool                         m_bUpdateEmbededInstanceDefinitions;
protected:
  static ON_ClassArray<ON_wString>m_def_extensions;
private:
  ON__UINT_PTR m_sdk_reserved;
};

