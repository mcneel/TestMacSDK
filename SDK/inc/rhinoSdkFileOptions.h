//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
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

class RHINO_SDK_CLASS CRhinoFileWriteOptions
{
public:
  CRhinoFileWriteOptions();
  ~CRhinoFileWriteOptions();
  CRhinoFileWriteOptions(const CRhinoFileWriteOptions&);
  CRhinoFileWriteOptions& operator=(const CRhinoFileWriteOptions&);

#if defined(ON_HAS_RVALUEREF)
  // Clone constructor
  CRhinoFileWriteOptions(CRhinoFileWriteOptions&&) ON_NOEXCEPT;

  // Clone assignment
  CRhinoFileWriteOptions& operator=(CRhinoFileWriteOptions&&)ON_NOEXCEPT;
#endif

  static const CRhinoFileWriteOptions Defaults;

  /*
  Description:
    If a model is being saved to the clipboard, then the 
    CRhinoFileWriteOptions parameter must be constructed
    as a copy of ClipboardFileDefaults.
  */
  static const CRhinoFileWriteOptions ClipboardFileDefaults;

  /*
  Description:
    If a model is being saved as a template, then the
    CRhinoFileWriteOptions parameter must be constructed
    as a copy of TemplateFileDefaults.
  */
  static const CRhinoFileWriteOptions TemplateFileDefaults;

  static bool GetFileTypeIdAndIndexFromFileName(
    const wchar_t* sFileName,
    ON_UUID& file_type_id,
    unsigned int& file_type_index
    );

  static bool GetFileTypeIdAndIndexFromFileExtension(
    const wchar_t* sFileExtension,
    ON_UUID& file_type_id,
    unsigned int& file_type_index
    );

  /*
  Returns:
    True if a template file is being saved.
  */
  bool TemplateFileIsDestination() const;
  
  /*
  Returns:
    True if a the selected object are being copied to the clipboard.
  */
  bool ClipboardIsDestination() const;

  /*
  Returns:
    The runtime serial number of the document being written.
  Remarks:
    The default is 0.  Use CRhinoDoc::FromRuntimeSerialNumber() to
    get a pointer to the document.
  */  
  unsigned int DocumentSerialNumber() const;

  bool SetDocumentSerialNumber(
    unsigned int
    );

  /*
  Returns:
    True: (default)
      If a complete, current version, 3dm file is successfully saved,
      then then name of the file will be used to update the document's 
      default file path and title and document will be marked as not
      modified.
    False:
      The document's default file path, title and modified state will
      not be changed under any circumstances.
  Remarks:
    This setting is ignored when selected information is exported,
    earlier 3dm versions or non-3dm formats are written.
  */
  bool UpdateDocumentPath() const;
  bool SetUpdateDocumentPath(
    bool bUpdateDocumentPath
    );

  enum WriteStates : unsigned int
  {
    kNotWriting = 0,
    kWriteInProgress = 1,
    kWriteSucceeded = 2,
    kWriteFailed = 3
  };

 CRhinoFileWriteOptions::WriteStates WriteState() const;

 enum WriteFailedReasons : unsigned int
 {
   kUnsetReason = 0,

   kUserCanceled,
   kEvaluationPeriodExpired,
   kAnotherWriteInProgress,

   kEmptyFileName,
   kInvalidFileName,  
   kFileIsReadOnly,   // 
   kFileIsLocked,     // Another Rhino session is using this file and it is locked
   kUnableToOpenFile,
   kUnableToOpenTemporaryFile,
   kUnableToRenameTemporaryFile,
   kUnableToReplaceFile,

   kUnableToLoadPlugIn, // FileTypeId() does not identify any plug-in that can be loaded
   kFileTypeIdDoesNotIdentifyAFileWritingPlugIn, // FileTypeId() does not identify a file writing plug-in
   kFileWritingPlugInFailed,

   kDeviceFull,
   kDeviceWriteFailed,
   kDeviceSeekFailed,

   kInputValidationFailed, // there is a bug in the code and a validation check found it

   // writing to a section or table failed
   kWriteFailedStartSection,
   kWriteFailedPropertiesTable,
   kWriteFailedSettingsTable,
   kWriteFailedBitmapTable,
   kWriteFailedTextureMappingTable,
   kWriteFailedMaterialTable,
   kWriteFailedLinetypeTable,
   kWriteFailedLayerTable,
   kWriteFailedGroupTable,
   kWriteFailedDimensionStyleTable,
   kWriteFailedLightTable,
   kWriteFailedHatchPatternTable,
   kWriteFailedInstanceDefinitionTable,
   kWriteFailedObjectTable,
   kWriteFailedHistoryRecordTable,
   kWriteFailedUserTable,
   kWriteFailedEndMark
 };

  unsigned int WriteFailedReason() const;

  static const wchar_t* WriteFailedReasonDescription(
    unsigned int write_failed_reason
    );

  /*
  Returns:
    If WriteState() and WriteFailedReason() identify a reason for failure,
    the a description is returned.  Otherwise null is returned.
  */
  const wchar_t* WriteFailedReasonDescription() const;

  /*
  Returns:
    nil:
      File will be a Rhino .3dm file.
    not-nil: 
      ID of the file export plug-in that will write the file
  */
  ON_UUID FileTypeId() const;

  /*
  Returns:
    FileTypeIndex that identifies the extension the file writing plugin will create.
  */
  unsigned int FileTypeIndex() const;

  /*
  Returns:
    True if the file time is some version of a Rhino 3dm file.
  */
  bool FileTypeIs3dm() const;

  /*
  Returns:
    True if the the file type is the current 3dm file version
    and everything in the document will be saved.
  */
  bool FileTypeIsComplete3dm() const;

  /*
  Parameters:
    file_type_id - [in]
      nil, a Rhino application id, or the id of a file writing plug-in.
    file_type_index - [in]
      If file_type_id identifies a file writing plug-in, then file_type_index
      selects the type of file the plug-in will write.
      If file_type_id is nil or a Rhino application id, this parameter is
      ignored and the file type is set to the current 3dm version.
  Returns:
    True if the value was set to application_id.
  Remarks:
    CRhinoFileWriteOptions::GetFileTypeIdAndIndexFromFileName() and
    CRhinoFileWriteOptions::GetFileTypeIdAndIndexFromFileExtension()
    may be used to get the file_type_id and  file_type_index from
    a file name or file extension.
  */
  bool SetFileTypeIdAndIndex(
    ON_UUID file_type_id,
    unsigned int file_type_index
    );

  /*
  Returns:
    0: 
      The file type is non-3dm file identified by FileTypeId() and FileTypeIndex().
    >= 2:
      The file type is a 3dm file and the number specifies the
      earliest version of Rhino that will be able to read the
      3dm file.
  Remarks:
    The default is RhinoVersion::App3dmVersionNumber().
  */
  unsigned int Rhino3dmVersion() const;

  /*
  Description:
    Specify what version of .3dm file should be written.
  Parameters:
    rhino_3dm_version - [in]
      2 <= rhino_3dm_version <= RhinoVersion::App3dmVersionNumber()
      This number specifies the earliest version of Rhino
      that will be able to read the file.
  Returns:
    True if the value was set to rhino_3dm_version.
    False if setting is prohibited or the input is not valid,
    in which case you can call Rhino3dmVersion() to determine
    what happened.
  */
  bool SetRhino3dmVersion(
    unsigned int rhino_3dm_version
    );

  //// Returns RhinoVersion::App3dmVersionNumber();
  //static int Default3dmFileVersion();

  /*
  Returns:
    True:
      The selected object filter will be applied.
      Only selected objects and the additional information in the
      model they rely on, like layers, materials and instance definitions,
      will be saved.
    False:
      The selected object filter will not be applied.
  Remarks:
    The default is false.
  */
  bool SelectedObjectFilter() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetSelectedObjectFilter(
    bool bSelectedObjectFilter
    );

  /*
  Returns:
    True:
      The selected subobject filter will be applied.
      Only selected subobjects and the additional information in the
      model they rely on, like layers, materials and instance definitions,
      will be saved.
    False:
      The selected subobject filter will not be applied.
  Remarks:
    The default is false.
  */
  bool SelectedSubobjectFilter() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetSelectedSubobjectFilter(
    bool bSelectedSubobjectFilter
  );

  /*
  Returns:
    True:
      The geometry only filter will be applied.
      Only geometric objects (curves, surface, breps, meshes, ...) will be saved.
      Annotation, layers, materials, ... will not be saved.
    False:
      The geometry only filter will not be applied.
  Remarks:
    The default is false.
  */
  bool GeometryObjectFilter() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetGeometryObjectFilter(
    bool bGeometryObjectFilter
    );

  /*
  Returns:
    True if a preview image of the model will be saved in the file.
  Remarks:
    The default is true.
  */
  bool IncludePreviewImage() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIncludePreviewImage(
    bool bIncludePreviewImage
    );

  /*
  Returns:
    True if bitmap  images used for textures and other purposes
    should be saved in the file.
  Remarks:
    The default is true.
  */
  bool IncludeBitmaps() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIncludeBitmaps(
    bool bIncludeBitmaps
    );


  /*
  Returns:
    True if texture information should be saved in the file.
  Remarks:
    The default is false.
  */
  bool IncludeTextures() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIncludeTextures(
    bool bIncludeTextures
    );


  /*
  Returns:
    True if object history information should be saved in the file.
  Remarks:
    The default is true.
  */
  bool IncludeObjectHistory() const;

  bool SetIncludeObjectHistory(
    bool bIncludeObjectHistory
    );


  /*
  Returns:
    True if cached render meshes will be saved in the file.
  Remarks:
    The default is true.
  */
  bool IncludeRenderMeshes() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIncludeRenderMeshes(
    bool bIncludeRenderMeshes
    );

  /*
  Returns:
    True if plug-in information will be saved in the file.
  Remarks:
    The default is true.
  */
  bool IncludePlugInInformation() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIncludePlugInInformation(
    bool bIncludePlugInInformation
    );

  /*
  Returns:
    True if information from reference models will be saved in the file.
  Remarks:
    The default is false.
  */
  bool IncludeReferenceModelInformation() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIncludeReferenceModelInformation(
    bool bIncludeReferenceModelInformation
    );

  /*
  Returns:
  True if notes will be saved in the file.
  Remarks:
  The default is true.
  */
  bool IncludeNotes() const;

  /*
  Returns:
  True if the value was set.
  */
  bool SetIncludeNotes(
    bool bIncludeNotes
    );


  /*
  Returns:
    True if the writing operation cannot pause for user input.
  Remarks:
    The default is false.
  */
  bool UseBatchMode() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetUseBatchMode(
    bool bUseBatchMode
    );


  /*
  Returns:
    True if the save operation is an autosave.
  Remarks:
    The default is false.
  */
  bool IsAutosave() const;

  /*
  Returns:
    True if the value was set.
  */
  bool SetIsAutosave(
    bool bIsAutosave
  );

   /*
  Returns:
    True if no transformation will be applied to the saved geometry.
  */
  bool TransformationIsIdentity() const;

  /*
  Returns:
    Transformation that will be applied to the saved geometry.
  */
  ON_Xform Transformation() const;

  /*
  Parameters:
    xform - [in]
      Transformation to apply to saved geometry.
  Returns:
    True if the value was set.
  */
  bool SetTransformation(
    ON_Xform xform
    );


  bool GetFileName(
    ON_String& sUTF8FileName
    ) const;

  bool GetFileName(
    ON_wString& sFileName
    ) const;

  /*
  Parameters:
    sUTF8FileName - [in]
  Returns:
    True if the value was set.
  */
  bool SetFileName(
    const char* sUTF8FileName
    );

  /*
  Parameters:
    sFileName - [in]
  Returns:
    True if the value was set.
  */
  bool SetFileName(
    const wchar_t* sFileName
    );

  bool FileNameIsNotEmpty() const;

  bool FileNameIsEmpty() const;
 
  /*
  Returns:
    True:
      When writing occurs, a temporary file will be written to. 
      If the writing succeeds, the temporary file will be renamed
      to the specified file name.   
  Remarks:
    It is not necessary to specify the name of the temporary file.
    If you want to specify the name of the temporary file, you
    may use SetTemporaryFileName().
  */
  bool UseTemporaryFileAndRename() const;
  bool SetUseTemporaryFileAndRename(
    bool bUseTemporaryFileAndRename
    );

  bool SetTemporaryFileName(
    const char* sUTF8FileName
    );
  bool SetTemporaryFileName(
    const wchar_t* sFileName
    );
  bool GetTemporaryFileName(
    ON_String& sUTF8FileName
    ) const;
  bool GetTemporaryFileName(
    ON_wString& sFileName
    ) const;
  bool TemporaryFileNameIsNotEmpty() const;
  bool TemporaryFileNameIsEmpty() const;

  /*
  Returns:
    True:
      If a 3dm file with the same name exists, it will be renamed by appending "bak" to its name.
    False
      If a 3dm file with the same name exists, it will be deleted and replaced.
  */
  bool CreateBackupFile() const;
  bool SetCreateBackupFile(
    bool bCreateBackupFile
    );

  /*
  Returns:
    True:
      If a non-3dm file with the same name exists, it will be renamed by appending "bak" to its name.
    False
      If a non-3dm file with the same name exists, it will be deleted and replaced.
  */
  bool CreateOtherBackupFile() const;
  bool SetCreateOtherBackupFile(bool);

  /*
    If a file with the same name exists, it will be copied to the path set in BackupFilePath and have "bak" to its name.
    If BackupFilePath() == L"" the file will be renamed by appending "bak" to its name.
  */
  ON_wString BackupFilePath() const;
  bool SetBackupFilePath(ON_wString);

#if defined (ON_RUNTIME_APPLE)
  // The Apple frameworks always write to a temporary file and rename the temporary file when done.
  // Some plug-ins need to know the final destination file name, and it is saved here.
  bool SetDestinationFileName(
    const wchar_t* sFileName
    );
  bool GetDestinationFileName(
    ON_wString& sFileName
    ) const;
  ON_wString Description() const;
#endif

  /*
  Description:
    Additional options relative to the specific file format
    the current write operation is about.
  Returns:
    return a const ON_ArchivableDictionary reference with the options.
  */
  const ON_ArchivableDictionary& OptionsDictionary() const;

  /*
  Description:
    Additional options relative to the specific file format
    the write operation will be about.
  Returns:
    return a ON_ArchivableDictionary reference with the options.
  */
  ON_ArchivableDictionary& OptionsDictionary();

private:
  friend class CRhinoDoc;

  unsigned int m_rhino_doc_sn;  // If writing has started, m_rhino_doc_sn is set
  unsigned int m_reserved1;
  enum CRhinoFileWriteOptions::WriteStates m_write_state;   // a WriteStates enum value.
  enum CRhinoFileWriteOptions::WriteFailedReasons m_write_failed_reason;
  bool m_bUseBatchMode;
  bool m_bClipboardIsDestination;    // cannot be changed (not const because operator= must copy)
  bool m_bTemplateFileIsDestination; // cannot be changed (not const because operator= must copy)
  bool m_bUpdateDocumentPath;
  bool m_bCreateBackupFile;
  bool m_bUseTemporaryFileAndRename;
  bool m_bIsAutosave;
  bool m_reserved8;
  ON_wString m_file_name;
  ON_wString m_temporary_file_name;
#if defined (ON_RUNTIME_APPLE)
  ON_wString m_destination_file_name;
#endif

  friend void RhSetWriteFailed(class CRhinoFileWriteOptions*, CRhinoFileWriteOptions::WriteFailedReasons);

  // Additional settings that control the type and content of the file.
  // These settings are managed as a non-SDK class so they can be modified 
  // without breaking the SDK.
  friend CRhinoFileWriteOptions CRhinoFileWriteOptions_DefaultsCtor(unsigned int);
  friend class CRhFileWriteOptionsData* RhFileWriteOptionsData(class CRhinoFileWriteOptions*);
  friend const class CRhFileWriteOptionsData* RhFileWriteOptionsConstData(const class CRhinoFileWriteOptions*);
  class CRhFileWriteOptionsData* m_data;
};

////class RHINO_SDK_CLASS CRhinoFileReadOptions
////;


class RHINO_SDK_CLASS CRhinoFileReadOptions
{
public:
  // plug-in developers:
  //  Avoid creating local CRhinoFileReadOptions classes
  //  Instead use const CRhinoFileReadOptions& RhinoApp().FileReadOptions()
  //  when you need the current file reading options.
  CRhinoFileReadOptions();
  CRhinoFileReadOptions( 
    bool bImport
    );
  CRhinoFileReadOptions( const CRhinoFileReadOptions&);
  CRhinoFileReadOptions& operator=( const CRhinoFileReadOptions&);

  ~CRhinoFileReadOptions();

  static const CRhinoFileReadOptions Unset;
  static const CRhinoFileReadOptions OpenModelDefaults;
  static const CRhinoFileReadOptions ImportModelDefaults;
  static const CRhinoFileReadOptions ImportReferenceModelDefaults;
  static const CRhinoFileReadOptions OpenTemplateDefaults;
  
public:
  /*  
  Parameters:
    destination_document - [in]
      The CRhinoDoc that is the destination of the information read from the file.
  Returns:
    True if the destination document was set.
    False if the caller attempted to change the destination from one document to another.
  */
  bool SetDestinationDocument(
    const CRhinoDoc* destination_document
  );

  /*  
  Returns:
    The runtime serial number of the CRhinoDoc that is the destination of the information
    read from the file.

  Example:

      CRhinoDoc* doc = CRhinoDoc::FromRuntimeSerialNumber(fro.DestinationDocumentSerialNumber());
      if (nullptr == doc)
        ... no destination document ...
      else
        ...
  */
  unsigned int DestinationDocumentSerialNumber() const;

  /*  
  Returns:
    CRhinoDoc::FromRuntimeSerialNumber(DestinationDocumentSerialNumber());
  Remarks:
    You must check for a nullptr before dereferencing the returned pointer.
  */
  CRhinoDoc* DestinationDocument() const;

public:
  RhinoYesNoResponse LookForMissingFile() const;

  void SetLookForMissingFile(
    RhinoYesNoResponse look_for_missing_file
  );

private:
  /*
  Description:
    Clear all settings.  This is useful when an io class is being
    reused.
  */
  void Internal_SetDefaults();

  void Internal_CtorHelper();

  static CRhinoFileReadOptions Internal_ConstantsInitializer(int);

public:

  /*
  Returns:
    true:
      The information being read is being merged into
      the existing document.  This happens in a number of cases
      including but not limited to reading work session reference
      files, reading instance definitions, anything done by the
      Insert command, harvesting various things like materials,
      page layouts, annotation styles and hatch patterns from
      files.
    false:
      The information being read is being used to create a new
      document.
  Remarks:
    This is identical to the Mode(CRhinoFileReadOptions::ModeFlag::ImportMode) setting.
  */
  bool Merge() const;

  /*
  Returns:
       0: no stuatus
       1: reading in progress
       2: reading initial file successful
       3: updating linked idefs found in this file
      99: reading finished successfully
     100: reading failed.
  */
  int ReadingStatus() const;

  /*
  Returns:
    True if a Rhino model (typically a .3dm file) is being read.
    False otherwise.
  */
  bool ReadingRhinoModel() const;

  /*
  Returns:
    0: Not reading a Rhino file.
    1: Reading a Rhino 1 file.
    2: Reading a Rhino 2 file.
    3: Reading a Rhino 3 file.
    4: Reading a Rhino 4 file.
    5: Reading a 32-bit Rhino 5 file (file size limited to 4GB)
    50: Reading a 64-bit Rhino 5 file
    60: Reading a Rhino 6 file
    ...
  */
  unsigned int RhinoArchiveVersion() const;

  /*
  Returns:
    0: Not reading a Rhino file.
    >0: opernnurbs version
  Remarks:
    The hight bit of the version number is typically set. 
    Do not cast this value as an int.
  */
  unsigned int RhinoArchiveOpenNURBSVersion() const;

  /*
  Returns:
    id that identifies what is doing the reading.
    If RhinoApp().IsRhinoUUID(id) is true, then Rhino is reading
    a Rhino model (typically .3dm file).  
    If id is not nill and RhinoApp().IsRhinoUUID(id) is false, then
    the id identifies the CRhinoPlugIn that is reading the file.
  */
  ON_UUID ReaderId() const;

  /*
  Returns:
    The model space unit system used by the model the file is
    being read into.
  Remarks:
    When the ScaleGeometry mode setting is false, do not apply
    unit system scaling.
  */
  ON_UnitSystem ModelUnitSystem() const;

  /*
  Returns:
  The model space unit system used by the model the file is
  being read out of (in the case of import, insert etc).
  Remarks:
  When the ScaleGeometry mode setting is false, do not apply
  unit system scaling.
  */
  ON_UnitSystem SourceModelUnitSystem() const;

  /*
  Parameters:
    file_unit_system - [in]
      unit system for an object read from the file
  Returns:
    The unit system conversion scale factor to apply to model
    space objects from the file.
  Remarks:
    When the ScaleGeometry mode setting is false, do not apply
    unit system scaling.
  */
  double ModelUnitSystemScale( ON_UnitSystem file_unit_system ) const;
  
  /*
  Parameters:
    file_unit_system - [in]
      unit system for an object read from the file
  Returns:
    The unit system conversion scale factor to apply to model
    space objects from the file.
  Remarks:
    When the ScaleGeometry mode setting is false, do not apply
    unit system scaling.
  */
  double ModelUnitSystemScale( ON::LengthUnitSystem file_unit_system ) const;

  /*
  Returns:
    true:
      You may not ask any questions while the the file is being read.
      Specifically, no dialogs and no "getter" prompts are permitted.
      This happens in a number of cases including situations where
      files are being "batch mode" processed and users will be grumpy
      if they start a long job, go home, and return the next morning
      to discover their job didn't finish because you interrupted
      the reading to ask a question.
    false:
      You may ask the user questions.
  Remarks:
    This is identical to the Mode(CRhinoFileReadOptions::ModeFlag::BatchMode) setting.
  */
  bool NoUserInterfaceQuestions() const;

  /*
  Returns:
    True if a reference model is being read. 
    This is always the case for worksession reference
    models and linked instance definitions.  It may
    happen in other less common situations.
  */
  bool ReadingReferenceModel() const;

  /*
  Returns:
     0: Information being read will be added to the
        active model.
    >0: Serial number of a work session reference model
        based on the contents of the file being read.
  */
  unsigned int WorkSessionReferenceModelSerialNumber() const;

  /*
  Returns:
     0: Reading the active model.
    >0: Serial number of an instance definition
        based on the contents of the file being read.
        When an instance definition is being created, the
        instance definition may be of any type
        (static_def, linked_and_embedded_def, or linked_def).
        When an instance definition is being updated,
        the instance definition can be linked_and_embedded_def,
        or linked_def.
  */
  unsigned int InstanceDefinitionSerialNumber() const;

  /*
  Returns:
     0: Not reading a linked instance definition.
    >0: Serial number of a linked instance definition
        being read or one that is being updated.  For
        example, if a linked and embedded idef is being updated
        and it is nested inside of a linked idef, then this
        function will return the serial number of the linked
        idef.
  */
  unsigned int LinkedInstanceDefinitionSerialNumber() const;

  /*
  Returns:
    0: unset
    1: active 
       Use short name to find a matching active layer.
       If none is found, then create the layer using the
       full path as an active layer (no grandparent layer).
    2: reference
       Put the idef
  Remarks:
    When a linked instance definition is being read in an
    active model, then this returns the value of
    ON_InstanceDefinition.m_idef_layer_style for the idef.
    Note that simple layer names 
  */
  ON_InstanceDefinition::eLinkedComponentAppearance LinkedInstanceDefinitionLayerStyle() const;

  /*
  Description:
    When some sort if import is happening (import,
    paste, insert, ...) this setting controls
    how Rhino looks for maching existing layers.
    The 
  Returns:
    0: Use full path layer name from file.
    1: If there is already an active layer with the 
       same short name, then use it.  For example,
       if the current model has a layer tree that
       looks like
       X
        Y
        Z

      and the file being read has a layer tree that looks like
      P
       Q
       X
      Y
       Z

      then the file object on file layer P::X ends up on 
      model layer X, the file objects on file layer Y end
      up on on model layer X::Y and the file objects on file
      layer Y::Z end up on model layer X::Z.
  Remarks:
    When a worksession reference model is being read, 0 is always returned.
    When a linked instance definition reference model is being read,
    the value of ON_InstanceDefinition::m_idef_layer_style
    determines what value this function returns.
    Otherwise, the value returned by
    RhinoApp().AppSettings().ImportFileLayerMatchingOption()
    determines what value this function returns.
  */
  int ExistingLayerNameMatchingStyle() const;


  /*
  Description:
    Information that ultimately is part of a work session
    reference model or linked instance definition model 
    is being read, all layers are added as sub-layers
    of a grand-parent layer.  When the information
    being read is destined for the active model, no
    grand parent layer is used.
  Returns:
    nil: no grand parent layer is in use.
    non-nil: id of grand parent layer.
  */
  ON_UUID ReferenceModelGrandParentLayerId() const;

  /*
  Parameters:
    full_path - [out]
      Absolute path to the file, including the drive, directory and
      file name with extension.
  Returns:
    True if the full path is available.  False if not.
  */
  bool GetFullPath( ON_wString& full_path ) const;

  /*
  Parameters:
    file_name - [out]
      The file name, including the extension.  Does not
      include drive or directory information.
  Returns:
    True if the value was returned.
  */
  bool GetFileName( ON_wString& file_name ) const;

  /*
  Description:
    The file alias is generally the same as the file name with
    extension, but sometimes includes some  portion of the directory
    path to avoid confusion when ther are several files in play with
    the same name. This string is used in user interface to identify
    the file when the absolute path name is too long to be useful
    and can be found someplace else in the user interface.
  Parameters:
    file_alias - [out]
  Returns:
    True if an alias existed.
  */
  bool GetFileAlias( ON_wString& file_alias ) const;

  /*
  Description:
    When reference information is imported, name of model components
    like annotation styles, hatch patterns, block definitions, ...,
    have a reference prefix prepended to the original name.
  Parameters:
    original_name - [in]
    reference_component_name - [out]
      both parameters can be the same string.
  Example:
    If a block definition named "X" is in a reference file named C.3dm, then
    the reference_component_name will be "C.3dm : X".  If the component
    is being read from a nested linked instance definition, the reference_component_name
    will be something like "A.3dm>B.3dm>C.3dm : X"

  Returns:
    True if reference_component_name name was modified by prepending a prefix.
    False if reference_component_name is identical to original_name.    
  */
  bool GetReferenceModelComponentName( 
    const wchar_t* original_name,
    ON_wString& reference_component_name
    ) const;

  /*
  Parameters:
    bAppendReferenceNameSeparator - [in]
      If true and the reference prefix is not empty, then
      ON_ModelComponent::NameReferenceDelimiter (" : ") is append
      to the returned string.
  Returns:
    If a reference model is being read, the prefix for
    reference model components.
    Otherwise, empty string.
  */
  const ON_wString ReferanceModelComponentNamePrefix(
    bool bAppendReferenceNameSeparator
  ) const;

 public:
   /*
  Description:
    This function is provided for plug-ins to use when
    their CRhinoPlugIn::ReadDocument() function is called
    and they need to convert Rhino document item ids used
    in a .3dm file that was just read into the Rhino document
    item ids used in the runtime model.

    When a file is read, sometimes is is necessary to change the
    id of an item in a file to a new id in the runtime model.
    This often happens when one file is being imported into 
    an existing document and an id found in the file is already 
    in use in the existing document. An "item" can be anything
    in a table, like a layer, material, dimension style, geometry
    object, instance definition, and so on.
  Parameters:
    file_item_id - [in]
      id used for the item in the .3dm file being read.
    model_item_id - [out]
      id used to for the item in the runtime model
  See Also:
    CRhinoFileReadOptions::ArchiveToModelIdentificationMap()
  */
  void GetModelItemId(
    ON_UUID file_item_id, 
    ON_UUID* model_item_id
    ) const;

 public:
   /*
   Description:
     ArchiveToModelIdentificationMap() returns a mapping from archive (file)
     identification (component type, index, id) to model component identification.
   */
   const class ON_ManifestMap& ArchiveToModelIdentificationMap() const;
  
  enum class ModeFlag : unsigned int
  {
    // If you need to make any additions or changes to
    // these enum values, then please read comment in  
    // CRhinoFileReadOptions::CRhinoFileReadOptions().

    // These mode names are confusing and misleading, but
    // they cannot be changed or the SDK will break.
    // Read the comments carefully.  When in doubt,
    // read everything and put it in the model.

    // ImportMode:
    //  true means we are merging whatever is being
    //  read into an existing document. This means
    //  you need to consider things like:
    //  * If the information being read is in a different
    //    unit system, it should be scaled if UseScaleGeometry
    //    is true.
    //  * There can be existing layers, fonts, materials, 
    //    dimension styles, hatch patterns, and so on with the
    //    same name as items being read from the file.
    ImportMode = 0,

    // OpenMode:
    //   true means we are reading the information into an empty
    //   document.  This means you need to consider things like:
    //   * Setting the unit system (if the file has a unit system)
    //   * Creating a default layer if one is not there.
    //   * Setting up appropriate views when you're finished reading.
    OpenMode,

    // NewMode: (default = false)
    //   true means we are reading template information in something
    //   like a OnFileNew event.
    NewMode,

    // InsertMode:
    //   true means we are reading information that will be used to
    //   create an instance definition or some other type of
    //   "inserting" that is supported by Rhino's "Insert" command.
    InsertMode,

    // ImportReferenceMode: (default = false)
    //   true means we are reading information for a work session
    //   reference model or a linked instance definition.
    ImportReferenceMode,

    // BatchMode: (default = false)
    //   true means you cannot ask questions during reading.
    //   (no dialogs, no "getters", etc.)
    BatchMode,

    // UseScaleGeometry: (default = true)
    //   If this parameter is true, then no questions are asked
    //   when unit conversion scaling is optional and the
    //   setting specified by ScaleGeometry is used.
    UseScaleGeometry,

    // ScaleGeometry: (default = true)
    //   true:
    //     If ImportMode is true and the geometry in the file
    //     being read has a unit system different from the 
    //     model's unit system, then apply the unit conversion
    //     scale to the file's geometry before adding it to the
    //     model.
    //
    //   false:
    //     Do not scale. Once case where this happens is when an
    //     instance definition is read from a file and the 
    //     model space instance references have been scaled.
    //     In case the instance definition geometry cannot
    //     be scaled or the net result is that the size of
    //     the instance reference object is scaled by the
    //     square of the scale factor.
    ScaleGeometry,

    // ScriptUpdateEmbededInstanceDefinitions: (default = true)
    //   When an embedded instance definition is out of date, settings
    //   in document properties and on the instance definition control
    //   if the idef is not updated, automatically updated, or the
    //   user is asked if they want to update.  In the case where the
    //   settings indicate the user should be asked, but BatchMode
    //   is true (and we can't ask), we need to have an automatic
    //   way to decide what to do.  In this case (BatchMode = true
    //   and doc/idef settings indicate user should be asked, the
    //   value of ScriptUpdateEmbededInstanceDefinitions mode is
    //   used to decide if the out-of-date instance definitions
    //   should be updated.  If this is confusing, then use the
    //   default (=true) setting.
    ScriptUpdateEmbededInstanceDefinitions,

    // UpdatingExistingInstanceDefinitions: (default = false)
    // Set to true when an existing instance definition is being updated
    // and when instance definition name collisions occur, the existing
    // instance definition should be silently updated.
    UpdatingExistingInstanceDefinitions,

    ModeCount
  };


  /*
  Description:
    Check which modes are enabled for file reading.  
    More than one mode can be enabled for a file read.
  Parameters:
    mode_to_check - [in] 
  Example:
          bool bImportMode = file_options.Mode(CRhinoFileReadOptions::ModeFlag::ImportMode );
          bool bOpenMode = file_options.Mode(CRhinoFileReadOptions::ModeFlag::OpenMode );
          bool bNewMode = file_options.Mode(CRhinoFileReadOptions::ModeFlag::NewMode );
          bool bBatchMode = file_options.Mode(CRhinoFileReadOptions::ModeFlag::BatchMode );
  Returns:
    TRUE if mode_to_check is active.
  See Also:
    ReadingTemplateFile(), ReadingPrimaryFile(), ReadingTemplateFile()
  */
  bool Mode( ModeFlag mode_to_check ) const;

  /*
  Description:
    Set modes for file reading.  
    More than one mode can be enabled for a file read.
  Parameters:
    mode_to_set - [in] 
    bEnabled - [in]
  Example:
          // set batch import mode
          file_options.SetMode(CRhinoFileReadOptions::ModeFlag::ImportMode, TRUE );
          file_options.SetMode(CRhinoFileReadOptions::ModeFlag::BatchMode, TRUE );
  */
  void SetMode( 
    ModeFlag mode_to_set, 
    bool bEnabled 
    );

  /*
  Returns:
    True if a template file is being read during something like a "New"
  Remarks:
    Exactly one of ReadingTemplateFile(), ReadingPrimaryFile(), or ReadingSecondaryFile() is true.
  */
  bool ReadingTemplateFile() const;

  /*
  Returns:
    True means an empty document is being filled with the contents of the file during something like an "Open"
  Remarks:
    Exactly one of ReadingTemplateFile(), ReadingPrimaryFile(), or ReadingSecondaryFile() is true.
  */
  bool ReadingPrimaryFile() const;

  /*
  Returns:
    True means something like an import or insert or worksession reference file is being read
    into an existing document.
  Remarks:
    Exactly one of ReadingTemplateFile(), ReadingPrimaryFile(), or ReadingSecondaryFile() is true.
  */
  bool ReadingSecondaryFile() const;

  ///*
  //// 22 Oct 2014 - Lowell - Added the answer about browsing for missing
  //// linked blocks to FileReadOptions rh-28779
  //Description:
  //  Sets or returns the current answer to the question of
  //  Ignoring missing linked block files asked while
  //  reading a 3dm file.
  //  In particular, if a user has already answered yes to all (rc_yes_to_all) 
  //  no to all (rc_no_to_all), don't ask again.
  //*/
  //int GetMissingLinkedFileQueryAction() const;
  //void SetMissingLinkedFileQueryAction(int action);

  /*
  Description:
    Additional options relative to the specific file format 
    the current read operation is about.
  Returns:
    return a const ON_ArchivableDictionary reference with the options.
  */
  const ON_ArchivableDictionary& OptionsDictionary() const;

  /*
  Description:
    Additional options relative to the specific file format
    the read operation will be about.
  Returns:
    return a ON_ArchivableDictionary reference with the options.
  */
  ON_ArchivableDictionary& OptionsDictionary();

private:
  friend class CRhFileReadOptions;

private:
  // The CRhinoDoc runtime serial number of the destination document.
  unsigned int m_destination_doc_sn = 0;

private:
  // When a referenced file cannot be located (linked instance definition, image, ...),
  // This is the response to the question
  // "File X is missing, would you like to use a file system browser to find it?"
  RhinoYesNoResponse m_look_for_missing_file = RhinoYesNoResponse::Unset;

private:
  unsigned int m_class_rtti = 0;  // 0 = CRhinoFileReadOptions
                                  // 2 = CRhFileReadOptions

private:
  unsigned int m_bool_bits = 0; // room for 32 true/false settings

private:
  class CRhFileReadOptionsData* m_data = nullptr;
};


/*
Description:
  Put a CRhinoSaveAndRestoreAppFileReadOptions on the stack
  when you want to save current CRhinoApp file read options,
  modify them for some use, and then restore them.

  The default constructor saves the current settings at construction
  time.  The destruction will restore any saved settings.

  If you want more control over the time when saving and restoring
  happens, you may use the constrctor with a bool bSave parameter
  to control what happens at construction time and explicitly call
  Save, Restore and Destroy to manage the saved settings.
*/
class RHINO_SDK_CLASS CRhinoSaveAndRestoreAppFileReadOptions
{
public:

  /*
  Description:
    The default constructor saves the current value of
    RhinoApp().FileReadOptions().
  */
  CRhinoSaveAndRestoreAppFileReadOptions();

  /*
  Parameters:
    bSaveWhenConstructed - [in]
      true:
        This constructor will save the current value of
        RhinoApp().FileReadOptions().
      false:
        Constructs this with no saved settings.
  */
  CRhinoSaveAndRestoreAppFileReadOptions( bool bSaveWhenConstructed );

  /*
  Description:
    If there are saved settings, the destructor will use them to
    set the value of RhinoApp().FileReadOptions().
  */
  ~CRhinoSaveAndRestoreAppFileReadOptions();

  /*
  Returns:
    Settings currently saved in this class.
  */
  const class CRhinoFileReadOptions* SavedFileReadOptions() const;

  /*
  Description:
    Saves the current value CRhinoApp().FileReadOptions().
  */
  void Save();

  /*
  Description:
    If there are saved settings, Restore() will use them to set the
    value of RhinoApp().FileReadOptions() and then destroy the saved
    settings.
  Returns:
    True if there were settings to restore.
  */
  void Restore();

  /*
  Description:
    Destroy any currently saved settings without modifying
    the CRhinoApp().FileReadOptions() settings.
  */
  void Destroy();

private:
  // prohibit use of copy construction and operator=
  CRhinoSaveAndRestoreAppFileReadOptions(const CRhinoSaveAndRestoreAppFileReadOptions&);
  CRhinoSaveAndRestoreAppFileReadOptions& operator=(const CRhinoSaveAndRestoreAppFileReadOptions&);
  class CRhFileReadOptions* m_saved_fro;
};


/*
Description:
  SDK tool to read a file using the Rhino file I/O plug-ins.
Parameters:
  lpsFileName - [in] complete name of the file to write, including
                     full path and extension.  The extension is used
                     to determine what type of file is read.
  doc - [in]
  options - [in] file reading options
Returns:
  True if reading succeeded.
  Look at doc.ReadFileResult() if you need to distinguish 
  between partial and complete success.
See Also:
  CRhinoDoc::Read3DM
*/
RHINO_SDK_FUNCTION
bool RhinoReadFile( 
  const wchar_t* lpsFileName, 
  CRhinoDoc& doc, 
  const CRhinoFileReadOptions& options
  );

class RHINO_SDK_CLASS CRhinoComponentNameConflictUI
{
public:
  CRhinoComponentNameConflictUI() = default;
  ~CRhinoComponentNameConflictUI() = default;
  CRhinoComponentNameConflictUI(const CRhinoComponentNameConflictUI&) = default;
  CRhinoComponentNameConflictUI& operator=(const CRhinoComponentNameConflictUI&) = default;


  ///////////////////////////////////////////////////////////////////
  //
  // input information for script and dialog query
  //

  void SetDocument(
    const CRhinoDoc& doc
  );

  void SetDocument(
    const CRhinoDoc* doc
  );

  void SetDocument(
    unsigned int rhino_doc_sn
  );

  CRhinoDoc* Document() const;

  void SetDefaultResolutionMethod(
    ON::ComponentNameConflictResolution default_resolution_method
  );
  ON::ComponentNameConflictResolution DefaultResolutionMethod() const;

  void SetComponentWithNameConflict(
    const ON_ModelComponent& component
  );

  const ON_wString& NameWithConflict() const;
  ON_ModelComponent::Type ComponentType() const;
  ON_UUID ComponentParentId() const;

  bool SetDefaultNewComponentName(
    const wchar_t* default_new_component_name
  );
  const ON_wString& DefaultNewComponentName() const;

  /*
  If a query message is not set, an appropriate one will be automatically generated.
  */
  void SetQueryMessage(
    const wchar_t* query_message
  );
  const ON_wString& QueryMessage() const;

  void SetPermitCancel(
    bool bPermitCancel
  );

  /*
  Returns:
    true: The query user interface should permit canceling.
    false: The user must provide an answer.
  */
  bool PermitCancel() const;

  ///////////////////////////////////////////////////////////////////
  //
  // input for script query
  //
  
  /*
  If a script get option prompt is not set, an appropriate one will be automatically generated.
  */
  void SetScriptQueryGetGetOptionPrompt(
    const wchar_t* script_query_get_option_prompt
  );
  const ON_wString& ScriptQueryGetGetOptionPrompt() const;

  /*
  If a script get name prompt is not set, an appropriate one will be automatically generated.
  */
  void SetScriptQueryGetGetNamePrompt(
    const wchar_t* script_query_get_name_prompt
  );
  const ON_wString& ScriptQueryGetGetNamePrompt() const;

  ///////////////////////////////////////////////////////////////////
  //
  // input for dialog query
  //

  /*
  Description:
    If no title is set, a default title is generated.
  */
  void SetDialogTitle(
    const wchar_t* dialog_title
  );
  const ON_wString& DialogTitle() const;
  
  /*
  Description:
    If no radio button description is set, one is automatically generated.
  */
  void SetUseModelComponentRadioButtonDescription(
    const wchar_t* use_model_component_radio_button_description
  );
  const ON_wString& UseModelComponentRadioButtonDescription() const;

  
  /*
  Description:
    If no radio button description is set, one is automatically generated.
  */
  void SetUpdateModelComponentRadioButtonDescription(
    const wchar_t* update_model_component_radio_button_description
  );
  const ON_wString& UpdateModelComponentRadioButtonDescription() const;

  
  /*
  Description:
    If no radio button description is set, one is automatically generated.
  */
  void SetUseBothComponentsRadioButtonDescription(
    const wchar_t* use_both_components_radio_button_description
  );
  const ON_wString& UseBothComponentsRadioButtonDescription() const;

  /*
  If a script new name edit box description is not set, an appropriate one will be automatically generated.
  */
  void SetNewNameEditBoxDescription(
    const wchar_t* new_name_edit_box_description
  );
  const ON_wString& NewNameEditBoxDescription() const;

  /*
  If a use all check box description is not set, an appropriate one will be automatically generated.
  */
  void SetUseForAllCheckBoxDescription(
    const wchar_t* for_all_check_box_description
  );
  const ON_wString& UseForAllCheckBoxDescription() const;

  ///////////////////////////////////////////////////////////////////
  //
  // tools to determine how to resolve the name conflict
  //

  /*
  Description:
    Tests new_name_candidate to see if it is valid and unique. If it is,
    both components can be used by changing the name of one to new_name_candidate.
  Parameters:
    new_name_candidate - [in]
      component name to test.
  Returns:
    True: new_name can be used to resolve the component name conflict.
  */
  bool NewNameIsValidAndUnique(
    const wchar_t* new_name_candidate
  ) const;

  /*
  Returns:
    The manifest for the current document.
  */
  const ON_ComponentManifest& DocumentManifest() const;

  /*
  Description:
    Use Rhino command script interface to resolve the component name conflict.
  Returns:
    CRhinoCommand::result::success: 
      Query was successful. 
      The results are availailable from
      ResolutionMethod(), UseResolutionMethodForFutureConflicts(), and NewComponentName().
    Otherwise:
      Query was canceled or unable to get an answer.
  */
  CRhinoCommand::result ScriptQuery();

  /*
  Description:
    Script interface to get a new component name.
  */
  CRhinoCommand::result ScriptGetNewNameQuery();

  /*
  Description:
    Use a modal dialog interface to resolve the component name conflict.
  Returns:
    CRhinoCommand::result::success: 
      Query was successful. 
      The results are availailable from
      ResolutionMethod(), UseResolutionMethodForFutureConflicts(), and NewComponentName().
    Otherwise:
      Query was canceled or unable to get an answer.
  */
  CRhinoCommand::result DialogQuery();


  ///////////////////////////////////////////////////////////////////
  //
  // output information
  //
  void SetResolutionMethod(
    ON::ComponentNameConflictResolution resolution_method
  );
  ON::ComponentNameConflictResolution ResolutionMethod() const;
  
  bool SetNewComponentName(
    const wchar_t* new_component_name
  );
  const ON_wString& NewComponentName() const;

  void SetUseResolutionMethodForFutureConflicts(
    bool bUseResolutionMethodForFutureConflicts
  );
  bool UseResolutionMethodForFutureConflicts() const;

  CRhinoCommand::result CommandResult() const;
  
private:
  ///////////////////////////////////////////////////////////////////
  // All types of query input
  //
  unsigned int m_rhino_doc_sn = 0;
  ON_ModelComponent::Type m_component_type = ON_ModelComponent::Type::Unset;
  ON_UUID m_component_parent_id = ON_nil_uuid;
  ON_wString m_name_with_conflict;

  ON::ComponentNameConflictResolution m_default_resolution_method = ON::ComponentNameConflictResolution::Unset;
  bool m_bPermitCancel = false;
  
  mutable ON_wString m_default_new_component_name;
  mutable ON_wString m_query_message;

  ///////////////////////////////////////////////////////////////////
  // Rhino command script query input (automatically generated)
  //
  mutable ON_wString m_script_query_get_option_prompt;
  mutable ON_wString m_script_query_get_name_prompt;

  ///////////////////////////////////////////////////////////////////
  // Modal dialog query input
  //
  mutable ON_wString m_dialog_title;

  mutable ON_wString m_use_model_component_radio_button_description;
  mutable ON_wString m_update_model_component_radio_button_description;
  mutable ON_wString m_use_both_components_radio_button_description;
  mutable ON_wString m_new_name_edit_box_description;
  mutable ON_wString m_use_for_all_check_box_description;

  ///////////////////////////////////////////////////////////////////
  // query output
  //  
  ON::ComponentNameConflictResolution m_resolution_method = ON::ComponentNameConflictResolution::Unset;
  ON_wString m_new_component_name;
  bool m_bUseResolutionMethodForFutureConflicts = false;
  CRhinoCommand::result m_query_result = CRhinoCommand::result::nothing;

private:
  bool Internal_ValidateCompoentName(
    const wchar_t* new_name_candidate,
    ON_wString& new_name
  ) const;

  void InternalBeforeQuery();
};
