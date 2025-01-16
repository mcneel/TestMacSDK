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

#include "rhinoSdkCoreClasses.h"
#include "RhinoSdkSpaceMorph.h"
#include "rhinoSdkDocProperties.h"
#include "rhinoSdkLinetype.h"
#include "rhinoSdkLayer.h"
#include "rhinoSdkMaterial.h"
#include "rhinoSdkGroup.h"
#include "rhinoSdkDimStyle.h"
#include "rhinoSdkHatchPattern.h"
#include "rhinoSdkFont.h"
#include "RhinoSdkHistoryRecordTable.h"
#include "rhinoSdkObjectAttributes.h"
#include "rhinoSdkObject.h"
#include "rhinoSdkLight.h"
#include "rhinoSdkAppSettings.h"
#include "rhinoSdkInstance.h"
#include "rhinoSdkGetString.h"
#include "rhinoSdkTextStyleManager.h"
#include "rhinoSdkView.h"

#if defined (RHINO_MAC_APPLICATION)
@class MRDocument;
#endif

// SDK classes
#if defined (ON_RUNTIME_APPLE)
class CWorkerThreadInfo;
class CRhObjectDragHelper;

class CRhStatusBarProgressMeter
{
public:
  int value;
  int lowerBound;
  int upperBound;
  ON_wString label;
  bool embedLabel;
  bool showPercent;
};
#endif

extern "C" {
  int RhinoMemoryErrorHandler(int);
}

//Use CRhinoUndoEventHandlerEx instead
class RHINO_SDK_CLASS CRhinoUndoEventHandler
{
public:
  CRhinoUndoEventHandler();
  virtual ~CRhinoUndoEventHandler();

  /*
  Description:
    The Rhino Undo command will call this virtual Undo
    function when the event needs to be undone. You should
    override this function to undo changes in your plug-in
    data,

    NEVER change any setting in the Rhino document or application.
    Rhino handles ALL changes to the application and document
    and you will break the Undo/Redo commands if you make
    any changes to the application or document.
  Parameters:
    cmd - [in]
      Command being undone.  Can be NULL.
    action_description - [in]
      Description of what is being undone.  Can be NULL.
    bCreatedByRedo - [in]
      True if a "Redo" was responsible for creating this record.
    undo_event_sn - [in]
      Runtime serial number Rhino assigned to the event.
  */
  virtual
    void Undo(
      const CRhinoCommand* cmd,
      const wchar_t* action_description,
      bool bCreatedByRedo,
      unsigned int undo_event_sn
    ) = 0;

  // The AuditUndo command prints this description.
  ON_wString m_description;
};

class RHINO_SDK_CLASS CRhinoUndoEventHandlerEx : public CRhinoUndoEventHandler
{
public:
  /*
    Description:
    The Rhino Undo command will call this virtual Undo
    function when the event needs to be undone. You should
    override this function to undo changes in your plug-in
    data,

    NEVER change any setting in the Rhino document or application.
    Rhino handles ALL changes to the application and document
    and you will break the Undo/Redo commands if you make
    any changes to the application or document.
  Parameters:
    doc - [in]
      Document in which this undo is taking place.
    cmd - [in]
      Command being undone.  Can be NULL.
    action_description - [in]
      Description of what is being undone.  Can be NULL.
    bCreatedByRedo - [in]
      True if a "Redo" was responsible for creating this record.
    undo_event_sn - [in]
      Runtime serial number Rhino assigned to the event.
  */
  virtual
    void Undo(
      CRhinoDoc& doc,
      const CRhinoCommand* cmd,
      const wchar_t* action_description,
      bool bCreatedByRedo,
      unsigned int undo_event_sn
    ) = 0;

private:
  virtual void Undo(const CRhinoCommand*, const wchar_t*, bool, unsigned int) override;
};

class RHINO_SDK_CLASS CRhinoDocIterator
{
  /*
  Example:
    Iterate all documents:
    CRhinoDocIterator doc_it;
    for ( CRhinoDoc* doc = doc_it.First();
          0 != doc;
          doc = doc_it.Next()
        )
    {
      ...
    }
  */
public:
  CRhinoDocIterator();

  void IncludeHeadlessDocuments(bool doInclude);
  bool IncludeHeadlessDocuments() const;

  /*
  Returns:
    The first document.
  Remarks:
    You may call First() multiple times on the same iterator.
  */
  class CRhinoDoc* First();

  /*
  Returns:
    The next document.
  Remarks:
    - Once Next() returns null, it will continue to return null
      until First() is called.
    - If First() has not be called, the first call to Next()
      returns the first document in Rhino.  This is done
      so that people can write while loops like
      CRhinoDocIterator it;
      while ( CRhinoDoc* rhino_doc = it.Next() )
      {
         ...
      }
  */
  class CRhinoDoc* Next();

  /*
  Returns:
    The most recent value returned by First() or Next().
  Remarks:
    - If First() or Next() has not been called, then Current()
      will return null.
    - If the document returned by First() or Next() has been deleted
      since the call to First() or Next(), then CurrentDoc() will
      return a null pointer.
  */
  class CRhinoDoc* CurrentDoc() const;
  unsigned int CurrentDocSerialNumber() const;

private:
  unsigned int m_current_doc_sn;
  // NOTE:
  //  The iterator can handle an arbitrary number of serial numbers.
  //  The buffer is refilled as needed.
  unsigned int m_sn_index;
  unsigned int m_sn_count;
  unsigned int m_flags;
  unsigned int m_sn_buffer[12];
};

enum CRhinoDocStatus
{
  /////////////////////////////////////////////////////////
  //
  doc_status_first = 0,

  /////////////////////////////////////////////////////////
  //
  // CRhinoDoc::CreateDocument status
  //
  create_before_construct, // CRhinoDoc member initialization occurring
  create_enter_construct,  // first line in CRhinoDoc::CRhinoDoc() body
  create_exit_construct,   // last line in CRhinoDoc::CRhinoDoc() body

  create_begin, // in CRhinoDoc::CreateDocument()

  create_before_CRhinoDoc_OnNewDocument,   // in CRhinoDoc::CreateDocument()
  create_enter_CRhinoDoc_OnNewDocument,    // in CRhinoDoc::OnNewDocument()

  // CRhinoDoc::OnNewDocument() calls CDocument::OnNewDocument()
  create_before_CDocument_OnNewDocument,
  create_after_CDocument_OnNewDocument,

  // CRhinoDoc::NewDocument() calls OnNewDocument event watchers
  create_before_event_post_OnNewDocument,
  create_enter_event_post_OnNewDocument,
  create_exit_event_post_OnNewDocument,
  create_after_event_post_OnNewDocument,

  create_exit_CRhinoDoc_OnNewDocument,    // in CRhinoDoc::OnNewDocument()
  create_after_CRhinoDoc_OnNewDocument,   // in CRhinoDoc::CreateDocument()

  // CRhinoDoc::CreateDocument() reads template file
  create_before_read_template_file,
  create_enter_read_3dm_template_file,

  create_before_event_post_OnBeginOpenDocument,
  create_enter_event_post_OnBeginOpenDocument,
  create_exit_event_post_OnBeginOpenDocument,
  create_after_event_post_OnBeginOpenDocument,

  create_after_read_3dm_template_file,

  create_before_event_post_OnEndOpenDocument,
  create_enter_event_post_OnEndOpenDocument,
  create_exit_event_post_OnEndOpenDocument,
  create_after_event_post_OnEndOpenDocument,

  create_read_3dm_template_before_views, // before CRhinoDoc::Read3DM() updates template views
  create_read_3dm_template_after_views,  // after CRhinoDoc::Read3DM() updates template views

  create_exit_read_3dm_template_file,

  create_after_read_template_file,

  // CRhinoDoc::CreateDocument() creates default views when
  // no template is used or no views are included in the template.
  create_before_default_views,
  create_after_default_views,

  // CRhinoDoc::CreateDocument() is finished
  create_end,

  /////////////////////////////////////////////////////////
  //
  // CRhinoDoc::OpenDocument status
  //
  open_begin,

  open_before_CRhinoDoc_OnOpenDocument,   // in CRhinoDoc::OpenDocument()
  open_enter_CRhinoDoc_OnOpenDocument,    // in CRhinoDoc::OnOpenDocument()

  // CRhinoDoc::OnOpenDocument() calls CDocument::OnOpenDocument()
  open_before_CDocument_OnOpenDocument,

  // CRhinoDoc::OpenDocument() is using a plug-in to read a model file
  open_before_read_model_file,

  // CRhinoDoc::OpenDocument() is using CRhinoDoc::Read3DM() to read a .3dm model file
  open_before_read_3dm_model_file,
  open_enter_read_3dm_model_file,

  // CRhinoDoc::OnOpenDocument() calls OnOpenDocument event watchers
  open_before_event_post_OnBeginOpenDocument,
  open_enter_event_post_OnBeginOpenDocument,
  open_exit_event_post_OnBeginOpenDocument,
  open_after_event_post_OnBeginOpenDocument,

  // If a plug-in is reading a file to open a document,
  // then these status settings bracket the call to
  // the plug-in reading the file.
  open_enter_read_model_file,
  open_exit_read_model_file,

  open_before_event_post_OnEndOpenDocument,
  open_enter_event_post_OnEndOpenDocument,
  open_exit_event_post_OnEndOpenDocument,
  open_after_event_post_OnEndOpenDocument,

  open_read_3dm_model_before_views, // before CRhinoDoc::Read3DM() updates views
  open_read_3dm_model_after_views,  // after CRhinoDoc::Read3DM() updates views

  open_exit_read_3dm_model_file,
  open_after_read_3dm_model_file,

  // in CRhinoDoc::OnOpenDocument() after call to CDocument::OnOpenDocument()
  open_after_CDocument_OnOpenDocument,

  // CRhinoDoc is finished using a plug-in to read the model file
  open_after_read_model_file,

  open_exit_CRhinoDoc_OnOpenDocument,    // in CRhinoDoc::OnOpenDocument()
  open_after_CRhinoDoc_OnOpenDocument,   // in CRhinoDoc::OpenDocument()

  // CRhinoDoc::OpenDocument() creates views
  open_before_views,
  open_after_views,

  // CRhinoDoc::OpenDocument() is finished
  open_end,

  /////////////////////////////////////////////////////////
  //
  // doc is alive and usable and being used
  //
  available,   // document can be read and modified

  // soon ...
  //read_only,   // document cannot be modified
  //unavailable, // document is being modified

  /////////////////////////////////////////////////////////
  //
  // CRhinoDoc::CloseDocument status
  //
  close_begin,

  close_before_CRhinoDoc_OnCloseDocument,   // in CRhinoDoc::CreateDocument()
  close_enter_CRhinoDoc_OnCloseDocument,    // in CRhinoDoc::NewDocument()

  // CRhinoDoc::OnCLoseDocument() calls OnCloseDocument event watchers
  close_before_event_post_OnCloseDocument,
  close_enter_event_post_OnCloseDocument,
  close_exit_event_post_OnCloseDocument,
  close_after_event_post_OnCloseDocument,

  // CRhinoDoc::OnCloseDocument() calls CDocument::OnCloseDocument()
  close_before_CDocument_OnCloseDocument,
  close_after_CDocument_OnCloseDocument,

  close_exit_CRhinoDoc_OnCloseDocument,    // in CRhinoDoc::OnCloseDocument()
  close_after_CRhinoDoc_OnCloseDocument,   // in CRhinoDoc::CloseDocument()

  close_end,

  close_enter_destruct, // first line in CRhinoDoc::~CRhinoDoc() body
  close_before_destroy, // 
  close_enter_destroy,  // first line of CRhinoDoc::Destroy() body
  close_exit_destroy,   // last line of CRhinoDoc::Destroy() body
  close_after_destroy,  // first line in CRhinoDoc::~CRhinoDoc() body
  close_exit_destruct,  // last line in CRhinoDoc::~CRhinoDoc() body

  /////////////////////////////////////////////////////////
  //
  doc_status_last = 0x7FFFFFFF // force 4 byte size
};

class RHINO_SDK_CLASS CRhinoCreateDocumentOptions
{
  // The CRhinoCreateDocumentOptions class is used to pass options to the 
  // static CRhinoDoc::CreateDocument() function.

public:
  CRhinoCreateDocumentOptions() = default;
  ~CRhinoCreateDocumentOptions() = default;
  CRhinoCreateDocumentOptions(const CRhinoCreateDocumentOptions&) = default;

  // Compilers implicitly delete operator= due to const m_version field
  // CRhinoCreateDocumentOptions& operator=(const CRhinoCreateDocumentOptions&) = default;

  static const CRhinoCreateDocumentOptions Defaults;

  /*
  Returns:
    true
      If no template file is specified or the template file
      does not contain views, and this parameter is true,
      then the default Rhino views are created.
  Remarks:
    Default value is true.
  */
  bool CreateDefaultViews() const;
  void SetCreateDefaultViews(
    bool bCreateDefaultViews
  );

  /*
  Returns:
    true if a "stealth" worksession should be created.
  Remarks:
    Default value is true.
    Ideally, this function will vanish in the near future!
  */
  bool CreateStealthWorksession() const;
  void SetCreateStealthWorksession(
    bool bCreateStealthWorksession
  );

#if defined (RHINO_MAC_APPLICATION)
  /*
   Returns:
     the MRDocument* to be paired with the CRhinoDoc
   Remarks:
     Default value is nil.
   */
  MRDocument* MacDocument() const;
  void SetMacDocument(
    MRDocument* macDocument
  );
#endif

  /*
  Returns:
    true if a "headless" document should be created
  Remarks:
    This function is a work in progress and may change during the development of V7
  */
  bool CreateHeadless() const;
  void SetCreateHeadless(
    bool bCreateHeadless
  );

private:
  // No friends! Implementation will change unpredictably.
  const unsigned int m_version = 0;

  bool m_bCreateDefaultViews = true;

  bool m_bCreateStealthWorksession = true;

  bool m_bCreateHeadless = false;

  bool m_bReserved2 = 0;

  ON__UINT_PTR m_reserved3 = 0;

#if defined (RHINO_MAC_APPLICATION)
  MRDocument* m_macDocument = nil;
#endif
};


class RHINO_SDK_CLASS CRhinoOpenDocumentOptions
{
  // The CRhinoOpenDocumentOptions class is used to pass options to the 
  // static CRhinoDoc::OpenDocument() function.

public:
  CRhinoOpenDocumentOptions();

  // Not needed until implementation changes
  //~CRhinoOpenDocumentOptions();
  //CRhinoOpenDocumentOptions(const CRhinoOpenDocumentOptions& src);
  //CRhinoOpenDocumentOptions& operator=(const CRhinoOpenDocumentOptions& src);

  static const CRhinoOpenDocumentOptions Defaults;

  /*
  Returns:
    non nil
      The id of a file reading plug-in to read the file.
    ON_nil_uuid
      The model file will be read as a native rhino .3dm file.
  Remarks:
    Default value is nil.
  */
  ON_UUID FileReadingPlugInId() const;
  int FileReadingPlugInFileTypeIndex() const;
  void SetFileReadingPlugInId(
    ON_UUID file_read_plug_in_id,
    int file_type_index
  );

  /*
  Returns:
    true if the model file name should be added to the MRU list
    after the file is successfully read.
  Remarks:
    Default value is true.
  */
  bool AddToMRU() const;
  void SetAddToMRU(
    bool bAddToMRU
  );

  /*
  Returns:
    true if the model should be visible after the file is successfully read.
  Remarks:
    Default value is true.
  */
  bool MakeVisible() const;
  void SetMakeVisible(
    bool bMakeVisible
  );


  /*
  Returns:
    true if the document name should be set from the file name.
    false if the model is not named (reading a backup or auto-save file).
  Remarks:
    Default value is true.
  */
  bool NameDocument() const;
  void SetNameDocument(
    bool bNameDocument
  );

  /*
  Returns:
    true if the Rhino application default directory should be updated.
  Remarks:
    Default value is true.
  */
  bool UpdateDefaultDirectory() const;
  void SetUpdateDefaultDirectory(
    bool bUpdateDefaultDirectory
  );

  /*
  Returns:
    true if the user interface queries or dialogs are permitted.
  Remarks:
    Default value is true.
  */
  bool UserQueriesPermitted() const;
  void SetUserQueriesPermitted(
    bool bUserQueriesPermitted
  );

  /*
  Returns:
    true if a "stealth" worksession should be created.
  Remarks:
    Default value is true.
    Ideally, this function will vanish in the near future!
  */
  bool CreateStealthWorksession() const;
  void SetCreateStealthWorksession(
    bool bCreateStealthWorksession
  );

  /*
  Returns:
    true if a "headless" document should be created
  Remarks:
    This function is a work in progress and may change during the development of V7
  */
  bool CreateHeadless() const;
  void SetCreateHeadless(
    bool bCreateHeadless
  );

#if defined (RHINO_MAC_APPLICATION)
  /*
   Returns:
     the MRDocument* to be paired with the CRhinoDoc
   Remarks:
     Default value is nil.
   */
  MRDocument* MacDocument() const;
  void SetMacDocument(
    MRDocument* macDocument
  );
#endif

private:
  // No friends! Implementation will change during 2013.
  const unsigned int m_version;

  bool m_bAddToMRU; // default = true
  bool m_bMakeVisible; // default = true
  bool m_bNameDocument; // default = true
  bool m_bUpdateDefaultDirectory; // default = true
  bool m_bUserQueriesPermitted; // default = true;
  bool m_bCreateStealthWorksession; // default = true
  bool m_bCreateHeadless; // default = false
  bool m_bUnused; // default = false

  ON_UUID m_file_read_plugin_id; // default = nil
  int m_file_read_plugin_file_type_index;

  ON__UINT_PTR m_sdk_reserved = 0;

#if defined (RHINO_MAC_APPLICATION)
  MRDocument* m_macDocument = nil;
#endif
};

class RHINO_SDK_CLASS CRhinoDimStyleContext : public ON_DimStyleContext
{
  // Constructor is private.
  // Use CRhinoDoc.DimStyleContext() to get an instance of a CRhinoDimStyleContext.

private:
  CRhinoDimStyleContext() = default;

public:
  virtual ~CRhinoDimStyleContext();
  CRhinoDimStyleContext(const CRhinoDimStyleContext&) = default;
  CRhinoDimStyleContext& operator=(const CRhinoDimStyleContext&) = default;

public:
  const ON_DimStyle& CurrentDimStyle() const override;

  const ON_DimStyle* DimStyleFromId(
    ON_UUID id,
    const ON_DimStyle* not_found_result = nullptr
  ) const override;

  const ON_DimStyle* DimStyleFromName(
    const ON_NameHash& name_hash,
    const ON_DimStyle* not_found_result = nullptr
  ) const override;

  const ON_DimStyle* DimStyleFromContentHash(
    const ON_SHA1_Hash& content_hash,
    const ON_DimStyle* not_found_result = nullptr
  ) const override;

  const ON_DimStyle* DimStyleFromFont(
    const ON_Font& font,
    double model_space_text_scale,
    double text_height,
    ON::LengthUnitSystem text_height_unit_system,
    bool bReturnClosestMatch,
    const ON_DimStyle* not_found_result = nullptr
  ) const override;

  const ON_DimStyle* FirstDimStyle(
    bool bIncludeSystemDimStyles = false,
    bool bIncludeDeletedDimStyles = false
  ) const override;

  const ON_DimStyle* NextDimStyle(
    ON_UUID id,
    bool bIncludeSystemDimStyles = false,
    bool bIncludeDeletedDimStyles = false
  ) const override;

  const ON_DimStyle* PrevDimStyle(
    ON_UUID id,
    bool bIncludeSystemDimStyles = false,
    bool bIncludeDeletedDimStyles = false
  ) const override;


  virtual ON::LengthUnitSystem ModelUnitSystem() const override;

  const class CRhinoDoc* Document() const;

private:
  friend class CRhinoDoc;
};

enum class TextureReportFilter : unsigned int
{
  MissingOnly,
  AllTextures,
};

enum class TextureReportResult : unsigned int
{
  Abort,
  Continue,
  Success,
  Failure,
};

class RHINO_SDK_CLASS CRhinoDoc final
{
#if defined (ON_RUNTIME_WIN)
protected: // create from serialization only
  CRhinoDoc(class CRhDoc& doc);
#else
public:
  CRhinoDoc(class CRhDoc& doc);
#endif


private:
  // The m_doc_sn field is private because nothing must ever pass 
  // this value by reference. It must be the first member of 
  // CRhinoDoc and is initialized before any other construction
  // code is called in CRhinoDoc::CRhinoDoc().
  //
  // Call CRhinoDoc.RuntimeSerialNumber() when you want this value.
  //
  // Call CRhinoDoc::FromRuntimeSerialNumber() when you need
  // to convert a Rhino document runtime serial number into 
  // a CRhinoDoc pointer.
  const unsigned int m_rhino_doc_sn;

#if defined (ON_RUNTIME_APPLE)
public:
  bool m_bInitialized;
#endif

private:
  // This value is used for reporting current status. This field must be
  // immediately after m_rhino_doc_sn.
  enum CRhinoDocStatus m_rhino_doc_status;

  friend class C_EW_Manager;
  bool SetDocStatus(enum CRhinoDocStatus doc_status);

private:
  // lock status
  unsigned int m_rhino_doc_lock;

private:
  // When a file is being read into the document or written
  // from the document, this serial number can be used to 
  // find file IO information.
  unsigned int m_rhino_doc_file_io_status_sn;

private:
  // Serial number of this document's active view.
  //   CRhinoDoc.ActiveRhinoView() and CRhinoDoc.SetActiveRhinoView()
  //   are the public interface for using and changing the
  //   value of m_rhino_doc_active_view_sn.
  unsigned int m_rhino_doc_active_view_sn;

public:
  /*
  Returns
    true if the CRhinoDoc::CreateDocument() is in the process
    of creating a document.
  Remarks:
    Use (IsInitializing() && !IsOpening()) to determine if
    CRhinoDoc::CreateDocument() is creating a document.
  */
  bool IsCreating() const;

  /*
  Returns
    true if the CRhinoDoc::OpenDocument() has created a document
    and is in the process of reading the model file.
  Remarks:
    Use (IsInitializing() && !IsOpening()) to determine if
    CRhinoDoc::CreateDocument() is creating a document.
  */
  bool IsOpening() const;

  /*
  Returns
    (IsCreating() or IsOpening()).
  */
  bool IsInitializing() const;

  /*
  Returns:
    True if the document is available for general use.
    Is is not being being created, opened, closing, or closed.
  */
  bool IsAvailable() const;

  /*
  Returns:
    True if this document is closing or has been closed.
  */
  bool IsClosing() const;

  bool IsHeadless() const;

  enum CRhinoDocStatus DocStatus() const;

public:
  /*
  Description:
    The CRhinoDoc runtime serial number is a value >= 1 that
    and is unique for each instance of a CRhinoDoc class in a
    given instance of the Rhino.exe application.
    A value of 0 is unset and never identifies a document.
    The first CRhinoDoc created has serial number 1.
    The serial number can be used to identify find and identify
    documents and is a fast and robust way to specify a document
    (as opposed to passing around a CRhinoDoc pointer or reference).
  See Also:
    CRhinoDoc::FromSerialNumber();
  Remarks:
    - The CRhinoDoc runtime serial number value is not saved
      in 3DM archives and is not unique between instances of
      Rhino.exe.
    - If you have a pointer to a doc and you need to check for
      a null before calling this function, then consider using
      the static function
      CRhinoDoc::RuntimeSerialNumber( const CRhinoDoc* rhino_doc ).
  */
  unsigned int RuntimeSerialNumber() const;

  /*
  Description:
    Get a pointer to a CRhinoDoc from a runtime_serial number.
  Parameters:
    doc_runtime_serial_number - [in]
  Returns:
    The document with the matching runtime serial number
    or null if no document exists.
  Remarks:
    The runtime serial number does not persist in 3dm archives
    and is not unique between instance of Rhino.exe.
    It should be used whenever you are tempted to save a
    CRhinoDoc pointer because Rhino documents can be
    deleted unexpectedly.
  See Also:
    CRhinoDoc::RuntimeSerialNumber();
  */
  static CRhinoDoc* FromRuntimeSerialNumber(
    unsigned int doc_runtime_serial_number
  );

  /*
  Returns:
    The runtime serial number of the document modeless user
    interface should reference when displaying information.
    It is often the case the 0 is returned.
  Remarks:
    - It is rare that this function is required and it should
      be called sparingly.  Never use this function when you are
      in a command, instead use the doc passed in the command's
      context parameter.
    - The doc for a command context, the "target" doc, and
      the "modeless user interface" doc can be three different
      documents.
    - The core Rhino application decides what document is
      used here. There is no way for a plug-in to change
      this value and that is intentional.
    - Modeless user interface should use a
      CRhinoEventWatcherEx::event_type_modeless_ui_doc_changed
      event watcher to detect when the modeless user interface
      document has changed.
  */
  static unsigned int ModelessUserInterfaceDocSerialNumber();

  /*
  Returns:
    The runtime serial number of the document a command will
    use if a specific document is not otherwise specified.
  Remarks:
    - It is rare that this function is required and it should
      be called sparingly.  Never use this function when you are
      in a command, instead use the doc passed in the command's
      context parameter.
    - The doc for a command context, the "target" doc, and
      the "modeless user interface" doc can be three different
      documents.
    - The core Rhino application decides what document is
      used here. There is no way for a plug-in to change
      this value and that is intentional.
  */
  static unsigned int TargetDocSerialNumber();

  /*
  Parameters:
    rhino_doc - [in]
      This parameter can be null, in which case zero is returned.
  Returns:
    The runtime serial number of the rhino_doc parameter.
  Remarks:
    This function is provided so that code doesn't have to
    check for a null pointer before calling the member
    function CRhinoDoc.RuntimeSerialNumber().  If the rhino_doc
    pointer is not null, this function returns
    rhino_doc->RuntimeSerialNumber().
  */
  static unsigned int RuntimeSerialNumber(
    const CRhinoDoc* rhino_doc
  );

  /*
  Returns:
    True if the rhino_doc pointer is null or points to
    an existing CRhinoDoc.
  Remarks:
    Calling CRhinoDoc::IsValidDocPointer() is fast and should be
    used before dereferencing a CRhinoDoc pointer that could possibly
    be invalid.  For example, CRhinoView::GetDocument() uses
    CRhinoDoc::IsValidDocPointer() to validate the pointer saved
    by MFC before returning it.
  */
  static bool IsValidDocPointer(
    const void* rhino_doc
  );

  /*
  Returns:
    True if the rhino_doc reference is valid and refers to
    an existing CRhinoDoc.
  Remarks:
    Calling CRhinoDoc::IsValidDocReference() is fast and should be
    used before using a CRhinoDoc reference that could possibly
    be invalid.
  */
  static bool IsValidDocReference(
    const CRhinoDoc& rhino_doc
  );

  /*
  Returns:
    The serial number of the next document to be constructed.
  Example:
    Examine documents created by SomeProcess().

          const unsigned int rhino_doc_sn0 = CRhinoDoc::NextRuntimeSerialNumber();
          SomeProcess();
          const unsigned int rhino_doc_sn1 = CRhinoDoc::NextRuntimeSerialNumber();
          for ( unsigned int rhino_doc_sn = rhino_doc_sn0; rhino_doc_sn < rhino_doc_sn1; rhino_doc_sn++ )
          {
            CRhinoDoc* doc = CRhinoDoc::FromRuntimeSerialNumber(rhino_doc_sn);
            if ( 0 == doc )
              continue;
            ...
          }
  */
  static unsigned int NextRuntimeSerialNumber();


  /*
  Returns:
    The serial number of the oldest document that exists
    or 0 if no documents exist.  This will be the
    smallest serial number of any document that
    currently exits.
  */
  static unsigned int HeadRuntimeSerialNumber();

  /*
  Returns:
    The serial number of the newest document that exists
    or 0 if no documents exist. This will be the
    largest serial number of any document that
    currently exits.
  Remarks:
    Other documents may have been created and deleted
    after this document.
  */
  static unsigned int TailRuntimeSerialNumber();

  /*
  Returns:
    The value that will always return NULL when passed to CRhinoDoc::FromRuntimeSerialNumber.
  Remarks:
    This can be passed to functions that take a document runtime serial number to mean "no document".
  */
  static const unsigned int NullRuntimeSerialNumber;

  /*
  Description:
    In most cases, it is easier, faster and more robust to
    use a CRhinoDocIterator to go through the list of documents.
    In the rare case when you really do want a list of
    all existing document serial numbers, use this function.
  Parameters:
    rhino_doc_sn_list - [out]
      list of rhino document serial numbers in increasing order.
  Returns:
    Number of rhino document serial numbers in rhino_doc_sn_list[].
  See Also:
    CRhinoDocIterator
  */
  static unsigned int GetDocList(
    ON_SimpleArray<unsigned int>& rhino_doc_sn_list
  );

  /*
  Description:
    In most cases, it is easier, faster and more robust to
    use a CRhinoDocIterator to go through the list of documents.
    In the rare case when you really do want a list of
    existing document serial numbers, use this function.
  Parameters:
    rhino_doc_sn0 - [in]
      Serial numbers <= rhino_doc_sn0 will not be added
      to rhino_doc_sn_list[].
    rhino_doc_sn_list_capacity - [in]
      maximum number of elements in rhino_doc_sn_list[] array.
    rhino_doc_sn_list - [out]
      list of rhino document serial numbers > rhino_doc_sn0.
  Returns:
    Number of rhino documents set in rhino_doc_sn_list[].
  See Also:
    CRhinoDocIterator
  */
  static unsigned int GetDocList(
    unsigned int rhino_doc_sn0,
    size_t rhino_doc_sn_list_capacity,
    unsigned int* rhino_doc_sn_list
  );

  /*
  Returns:
    Number of documents.
  */
  static unsigned int DocumentCount();

  /*
  Description:
    Creates an new CRhinoDoc.

  Parameters:
    sModelTemplateFileName - [in]
      - If null or empty, an empty document is created with no views.
        This is what you want to do if the next thing you are going to
        do is to read a file.
      - If not empty, this parameter specifies the name
        of a Rhino model to use as a template to initialize the
        document.  If the template contains views, those views
        are created.
        This is what you want to do if you are creating a new
        empty document and want to start with specified layers,
        tolerance, unit systems, and so on.
      - The function RhinoApp().AppSettings().FileSettings().m_default_template_file
        returns the Rhino application default template file.
    create_document_options - [in]
      Use this parameter to set options if you do not want
      the default behavior.
  Returns:
    Runtime serial number of the new CRhinoDoc.
  */
  static unsigned int CreateDocument(
    const wchar_t* sModelTemplateFileName,
    const CRhinoCreateDocumentOptions* create_document_options
  );

  /*
  Description:
    Creates an new CRhinoDoc.

  Parameters:
    sModelFileName - [in]
      - This parameter specifies the name of a model file to read
        to initialize document.  If the template contains views,
        those views are created.
        This is what you want to do if you are creating a new
        empty document and want to start with specified layers,
        tolerance, unit systems, and so on.
    open_document_options - [in]
      Use this parameter to set options if you do not want the
      default behavior.
  Returns:
    Runtime serial number of the new CRhinoDoc.
  */
  static unsigned int OpenDocument(
    const wchar_t* sModelFileName,
    const CRhinoOpenDocumentOptions* open_document_options
  );

  /*
  Description:
    Silently closes the specified document.
  Parameters:
      rhino_doc_sn - [in]
        Runtime serial number of the CRhinoDoc to close.
  Returns:
    True if the serial number was valid and the doc
    with this serial number was destroyed.
    False if no existing document has this serial number.
  Remarks:
    The modified flag is ignored.
  */
  static bool CloseDocument(
    unsigned int rhino_doc_sn
  );

  /*
  Description:
    Silently closes all documents.
  Returns:
    Number of documents that are closed.
  */
  static unsigned int CloseAllDocuments();

  /*
  Description:
    Calls CRhinoDoc.SaveModified() for every existing document.
  Returns:
    If the user answers "cancel" to any of the queries, then
    false is returned.  Otherwise true is returned.
  */
  static bool SaveAllModifiedDocuments();

  /*
  Description:
    Calls CRhinoDoc.SaveModified() for every existing document.
    If the user answers "cancel" to any of the queries, then
    false is returned.  Otherwise true is returned.
  Parameters:
    bUseBatchMode - [in] Use true to suppress dialog boxes.
  Returns:
    If the user answers "cancel" to any of the queries, then
    false is returned.  Otherwise true is returned.
  */
  static bool SaveAllModifiedDocuments(bool bUseBatchMode);

  /*
  Description:
    Silently closes all documents except this document.
  Returns:
    Number of documents that are closed.
  */
  unsigned int CloseAllOtherDocuments() const;

  /*
  Description:
    Activates flashing caret in command prompt.
  */
  bool ActivateCommandWindowCaret();

  // Attributes
public:
#if defined(ON_RUNTIME_WIN)
  /*
  Description:
    Draw to a device context.
  Parameters:
    it - [in] iterates through objects to be drawn. When in doubt, use
               CRhinoObjectIterator it( *this,
                      CRhinoObjectIterator::normal_or_locked_objects,
                      CRhinoObjectIterator::active_and_reference_objects);
    pDC - [in]
    width - [in]
    height - [in]
    view_info - [in] View projection and decoration information.  When
        in doubt, use RhinoApp().ActiveView()->Viewport().View().
    bIgnoreHighlights - [in]  If true, highlight status is ignored.
    bDrawTitle - [in]
    bDrawConstructionPlane - [in]
    bDrawWorldAxes - [in]
  Remarks:
    This is useful for creating metafiles, printing, etc.
    NOTE: This will DRAW to ANY type of DC, and will ONLY
    draw in Wireframe. Drawing bitmaps to metafiles generates much larger
    than needed .WMF files. To get rendered and/or preview bitmaps,
    see RenderToDC...

  */
  void DrawToDC(
    CRhinoObjectIterator& it,
    HDC pDC,
    int width,
    int height,
    const ON_3dmView& view_info,
    BOOL32 bIgnoreHighlights = true,
    BOOL32 bDrawTitle = false,
    BOOL32 bDrawConstructionPlane = false,
    BOOL32 bDrawWorldAxes = false
  ) const;

  /*
  Description:
    Render view to a device context.
  Parameters:
    it - [in] iterates through objects to be drawn. When in doubt, use
               CRhinoObjectIterator it( *this,
                      CRhinoObjectIterator::normal_or_locked_objects,
                      CRhinoObjectIterator::active_and_reference_objects);
    pDC - [in]
    width - [in]
    height - [in]
    view_info - [in] View projection and decoration information.  When
        in doubt, use RhinoApp().ActiveView()->Viewport().View().
    bIgnoreHighlights - [in]  If true, highlight status is ignored.
    bDrawTitle - [in]
    bDrawConstructionPlane - [in]
    bDrawWorldAxes - [in]
    bGhosted - [in]
  Remarks:
    This is useful for creating preview bitmaps, printing, etc.
    NOTE: Using this method to generate metafiles can/will result in
    very large files, since it renders to a bitmap and then transfers
    the bitmap to the passed in DC, rather than just drawing directly
    to the DC.

  */
  void RenderToDC(
    CRhinoObjectIterator& it,
    HDC pDC,
    int width,
    int height,
    const ON_3dmView& view_info,
    BOOL32 bIgnoreHighlights = true,
    BOOL32 bDrawTitle = false,
    BOOL32 bDrawConstructionPlane = false,
    BOOL32 bDrawWorldAxes = false,
    BOOL32 bGhosted = false
  ) const;
#endif

  enum display_engine_id
  {
    display_engine_none = 0,
    display_engine_opengl = 1,
    display_engine_direct3d = 2,
    display_engine_count  // The value of this enum will change
                          // if new display engines are added.
                          // Do not explicitly set it.
  };
  /*
  Description:
    Return the status of the display engine:
  Parameters:
    engine_id - [in]
      CRhinoDoc::display_engine_opengl or CRhinoDoc::display_engine_direct3d
  Returns:
    0: Engine is not available.
    1: Engine is available, but not initialized.
    2: Engine is available and initialized.
  */
  int DisplayEngineStatus(int engine_id) const;

  /*
  Description:
    Redraw all views of the active document.
  Remarks:
    If you change something in the active document -- like adding
    objects, deleting objects, modifying layer or object display
    attributes, etc., then you need to call CRhinoDoc::Redraw to
    redraw all the views.

    If you change something in a particular view
    like the projection, construction plane, background bitmap,
    etc., then you need  to call CRhinoView::Redraw to redraw that
    particular view.

    Calling CRhinoDoc::Redraw is conceptually like calling
    CDoc::UpdateAllViews() but Redraw is faster and draws the
    active view first.

    NEVER CALL UpdateAllViews() IN RHINO.

  See Also
    CRhinoView::SetRedrawDisplayHint
    CRhinoView::Redraw( unsigned int display_hint )
  */
  void Redraw();
  static void RedrawDoc(unsigned int rhino_doc_sn);

  /*
  Description:
    Calls "Redraw" for all active documents.
  */
  static void RedrawAllDocs();

  /*
  Description:
    Redraw all views of the active document with
    display hint == regenerate_display_hint.
    Good for a complete cleanup of the screen after a
    command that did a bunch of temporary drawing finishes

  Remarks:
    Same as CRhinoDoc::Redraw( CRhinoView::regenerate_display_hint).

    CRhinoDoc::Regen can be MUCH SLOWER than calling
    CRhinoDoc:Redraw() and should be used sparingly.  When
    developing a command, it is best to call Redraw() to
    see if it will do the trick, and then resort to Regen()
    if nothing else works.  Most commands should not need
    to call Regen().
  See Also
    CRhinoView::Redraw()
    CRhinoView::SetRedrawDisplayHint
  */
  void Regen();

  /*
  Description:
    Redraw all views of the active document with an explicit
    display hint.  This version of Redraw is intended for expert
    users doing custom drawing.

  Parameters:
    display_hint - [in] one of
            CRhinoView::regenerate_display_hint
            CRhinoView::mark_display_hint
            CRhinoView::refresh_display_hint
  Remarks:
    Calling CRhinoDoc::Redraw(display_hint) is the same as
    making the two calls
      SetRedrawDisplayHint( display_hint )
      Redraw()
    If you do not know what display hint to use, the just call
    Redraw() and it will automatically use the appropriate
    type of redraw.

    NEVER CALL UpdateAllViews() IN RHINO.
  See Also
    CRhinoView::Redraw()
    CRhinoView::SetRedrawDisplayHint
  */
  void Redraw(unsigned int display_hint);
  static void RedrawAllDocs(unsigned int display_hint);

  /*
  Description:
    Redraw all views of the active document with a regenerate
    display hint.  This version of Redraw will cause all views to
    update, but not in any particular order or any specific time.
    All you can be assured of is that the views will eventually
    update...

  Parameters:

  Remarks:
    The routine basically iterates through all views calling CRhinoView::Redraw()...

  See Also
    CRhinoView::Redraw()
  */
  void DeferredRedraw(void);

  /*
  Description:
    Resets all of the view pipelines to make sure they're all using
    the expected default pipeline (i.e. GetDefaultDisplayPipelineClass()).
    Certain situations arise where the pipeline class gets switched out
    to be something different than the default class...an example would be
    a shaded analysis mode getting set in a view that is currently using
    GDI wireframe...When the analysis mode is canceled, the view needs to
    return back to the GDI wireframe (i.e. the default display class). This
    routine simply ensures that all views are configured to their appropriate
    default display class.

  Parameters:
    none

  Remarks:
    The routine basically iterates through all views calling:
      CRhinoView::SetupDisplayPipeline()

  */
  void ResetViewPipelines(void);

  /*
  Description:
    Pipes calls made to CRhinoDoc->UpdateAllViews() to
    CRhinoDoc::Redraw().  Developers are not supposed
    to use UpdateAllViews, but sometimes MFC die hards
    can't help themselves.
  Parameters:
    pSender - [in] (can be NULL)
    lHint - [in] passed to CRhinoDoc::SetRedrawDisplayHint()
                 before CRhinoDoc::Redraw() is called.
    pHint - [in] ignored
  Remarks:
    UpdateAllViews is not a virtual function on CDoc.
    So, if somebody calls UpdateAllViews from a
    CDocument pointer, then we can' do anything about
    it.  The views will eventually get drawn, they
    just won't get drawn in the right order and the
    snappy Rhino look-n-feel will be missing.
  See Also:
    CRhinoDoc::Redraw
  */
#if defined (RHINO_SDK_MFC)
  void UpdateAllViews(
    CView* pSender,
    LPARAM lHint = 0L,
    CObject* pHint = NULL
  );
#endif

  /*
  Description:
    Low level helper for CRhinoView::Redraw().  You should call
    CRhinoView::Redraw() instead of this function.
  */
  void RedrawViewHelper(CRhinoView* view, int view_redraw_display_hint);


  /*
  Description:
    Cause objects selection state to change momentarily so the object
    appears to flash on the screen
  Parameters:
    obj_list - [in] list of objects to flash
    flash_style - [in]
            0 = invisible - visible flash,
            1 = object color - selected color
  */
  void FlashObjectList(
    ON_SimpleArray<const CRhinoObject*>& obj_list,
    int flash_style = 1
  );

  /////////////////////////////////////////////////////////////////////////
  //
  // Viewport layout
  //

  /*
  Returns:
    The runtime serial number of the CRhinoView that is
    this document's current active view.
  Remarks:
    Whenever a document needs a single view to perform a calculation
    it uses the active view.  Examples include
    - parsing x,y,z into a world coordinate (cplane to world)
    - saving a thumbnail image in file
    - exporting to a file format that can contain only one viewport
    - modeless user interface that reports view information
  */
  unsigned int ActiveRhinoViewSerialNumber() const;

  /*
  Parameters:
    new_active_rhino_view_sn - [in]
      The runtime serial number of a CRhinoView for this document.
  Returns:
    True if the input parameter was valid and the document's active
    view was set.
  */
  bool SetActiveRhinoViewSerialNumber(
    unsigned int new_active_rhino_view_sn
  );

  /*
  Description:
    Insures the document has an active view if at all possible.
  Parameters:
    ineligible_rhino_view_sn - [in]
      Any view with this serial number will not be used.
      This parameter is useful when a view will soon be
      deleted, hidden, or otherwise rendered useless as
      an active view and should not be considered as a
      candidate for the documents active view.
  Returns:
    The serial number of the document's active view.
  */
  unsigned int AutomaticallySetActiveRhinoViewSerialNumber(
    unsigned int ineligible_rhino_view_sn
  );

  /*
  Description:
    Arrange the placements of the document's views.
  Parameters:
    rect - [in]
      Typically, this is the value returned by
      RhinoApp().MainFrameViewClientRect()
    bMakeVisible - [in]
      True to make the views visible.
  Returns:
    Number of views the document has.
  Remarks:
    ArrangeViews() does not modify any active
    view settings and that is intentional.
  */
  int ArrangeViews(
    bool bMakeVisible
  ) const;

  // Description:
  //   Gets an array of all the views.
  // Parameters:
  //   view_list - [out] list of views
  //   include_standard_views - [in] add standard views to the view_list
  //   include_page_views - [in] add page views to the view list
  // Returns:
  //   Number of views.
  ON_DEPRECATED_MSG("Use the version that takes a CRhinoView::ViewTypeFilter.")
  int GetViewList(ON_SimpleArray<CRhinoView*>& view_list,
    bool include_standard_views = true,
    bool include_page_views = true
  );

  // Description:
  //   Gets an array of all the views.
  // Parameters:
  //   view_list - [out] list of views
  //   filter - [in] enum specifying the types of views to include in the list
  // Returns:
  //   Number of views.
  int GetViewList(ON_SimpleArray<CRhinoView*>& view_list, CRhinoView::ViewTypeFilter filter) const;

  const CRhinoView* ViewFromViewportId(
    ON_UUID viewport_id
  ) const;


  void DefaultViewLayout();
  void DefaultViewLayout(ON_BoundingBox model_bbox);
  void FourViewLayout(bool bUseMatchingViews = false);
  void FourViewLayout(ON_BoundingBox model_bbox, bool bUseMatchingViews = false);
  void FourViewLayout(ON_BoundingBox model_bbox, bool bThirdAngleProjection, bool bUseMatchingViews);
  void ThreeViewLayout(bool bUseMatchingViews = false);
  void ThreeViewLayout(ON_BoundingBox model_bbox, bool bUseMatchingViews = false);

  // Description:
  //   Replace the standard rhino views with new rhino views based on a
  //   set of ON_3dmView objects. All other views are left unchanged
  //   (page and detail views are untouched).
  // Parameters:
  //   view_count: [in] number of objects in views array
  //   views: [in] array of ON_3dmViews
  void ReplaceModelViews(int view_count, const ON_3dmView* views);

  // Description:
  //   Destroy all existing views and create a new set of views based
  //   on a  set of ON_3dmView objects. Similar to ReplaceModelViews,
  //   except this function removes all existing views.
  // Parameters:
  //   view_count: [in] number of objects in views array
  //   views: [in] array of ON_3dmViews
  void CreateViewLayout(
    int view_count,
    const ON_3dmView* views
  );

  // Description:
  //   Creates a new view.
  // Parameters:
  //   new_view - [in] the view parameters
  //   bShowView - [in] show the view
  // Returns:
  //   View serial number.
  unsigned int NewView(
    const ON_3dmView& new_view,
    bool bShowView
  );

  // Description:
  //   Creates a new view.
  // Parameters:
  //   new_view - [in] the view parameters
  // Returns:
  //   View serial number.
  ON_DEPRECATED_MSG("use overload that accepts bShowView parameter")
    unsigned int NewView(
      const ON_3dmView& new_view
    );


  // Description:
  //   Find the first view in the view list that has the same viewport title.
  //   Note the the title string is not guaranteed unique
  // Parameters:
  //   title: [in] viewport title to search for
  //   view: [out] if a view is found it is copied into this
  //   include_standard_views: [in] look at standard rhino views when searching
  //   include_page_views: [in] look at page views when searching
  // Return:
  //   true if a view is found with the matching title
  ON_DEPRECATED_MSG("use the overload the takes ViewTypeFilter")
  bool FindView(const wchar_t* title,
    ON_3dmView& view,
    bool include_standard_views = true,
    bool include_page_views = true
  );

  ON_DEPRECATED_MSG("Should have been const")
  bool FindView(const wchar_t* title,
    ON_3dmView& view,
    CRhinoView::ViewTypeFilter view_types);
    
  bool FindView(const wchar_t* title,
      ON_3dmView& view,
      CRhinoView::ViewTypeFilter view_types) const;

  CRhinoView* ActiveView() const;

#if defined (ON_RUNTIME_APPLE)
  void SetActiveView(CRhinoView* newActiveView);
#endif  // ON_RUNTIME_APPLE

  /////////////////////////////////////////////////////////////////////////
  //
  // Model
  //
  void GetDefaultObjectAttributes(ON_3dmObjectAttributes&) const; // attributes to use for new object
  const ON_3dmObjectAttributes DefaultObjectAttributes() const;

  /*
  Description:
    Get a bounding box of all the visible geometry in the model.
    The wireframe icons used to display render lights are NOT included.
    Page space geometry is NOT included
  Returns;
    bounding box
  See Also
    CRhinoLightTable::WireframeIconsBoundingBox( const CRhinoView* )
  */
  ON_DEPRECATED_MSG("Prefer version with three arguments") const ON_BoundingBox& BoundingBox(bool bIncludeCustomRenderMeshes = true) const;

  /*
  Description:
    Get a tight bounding box of all the visible geometry in the model.
    The wireframe icons used to display render lights are NOT included.
    Page space geometry is NOT included
  Returns;
    bounding box
  See Also
    CRhinoLightTable::WireframeIconsBoundingBox( const CRhinoView* )
  CRhinoObject::GetTightBoundingBox
  */
  ON_DEPRECATED_MSG("Prefer BoundingBox function with three arguments") const ON_BoundingBox& TightBoundingBox(bool bIncludeCustomRenderMeshes = true) const;

  /*
  Description:
  Get a bounding box of all the visible geometry in the model.
  The wireframe icons used to display render lights are NOT included.
  Page space geometry is NOT included
  //   bTight: [in] use CRhinoObject::TightBoundingBox in the calculation instead of BoundingBox
  //   bIncludeCustomRenderMeshes: [in] Resolve custom render meshes for all objects and use those
  //   bIncludeCurves: [in] include curves and annotations in the calculation
  Returns;
  bounding box
  See Also
  CRhinoLightTable::WireframeIconsBoundingBox( const CRhinoView* )
  CRhinoObject::GetTightBoundingBox
  */
  const ON_BoundingBox& BoundingBox(bool bTight, bool bIncludeCustomRenderMeshes, bool bIncludeCurves) const;

  /*
  Description:
    Destroys the cached scene bounding box so that it will be regenerated again.
  from scratch next time the view is regenerated
  See Also
    CRhinoDoc::BoundingBox() const
  */
  void InvalidateBoundingBox() const;

  //////////////////////////////////////////////////////////////////////
  //
  // File Writing
  //

public:
  /*
  Returns:
    If the document is being saved by CRhinoDoc::WriteFile(), then
    a pointer to the current file writing options is returned.
    Otherwise, null is returned.
  Remarks:
    Never save the returned pointer.  If you need to reference the
    information later, make a copy.
    File writing plug-ins can use this information to determine
    what should be saved.
  */
  const class CRhinoFileWriteOptions* WriteFileInProgress() const;

  /////*
  ////Description:
  ////  Expert user function to record a file writing error. Used
  ////  by file writing plug-ins.
  ////*/
  ////bool SetWriteFileFailedReason(
  ////  CRhinoFileWriteOptions::WriteFailedReasons file_write_failed_reason
  ////  );

  /*
  Description:
    Ask the user for a file name and additional file writing options.
    The document's default path and current title will be used
    to create a default file name.
  Parameters:
    fwo_in - [in]
      Current options that determine how the query is performed.
      - fwo_in.UseBatchMode()
         true: the query will use the Rhino command line interface.
         false: the query will use a dialog.
      - fwo_in.SelectedObjectFilter()
         true: The export interface will be used.
         false: The save interface will be used.
      - fwo_in.TemplateFileIsDestination()
         true: The template file interface will be used.
         false: The model file interface will be used.
      - fwo_in.GetFileName()
         Default file name.  If not set, document's default path and
         current title will be used to create a default file name.
    minimum_rhino_3dm_version_rescrition - [in]
      0:  Any type of 3dm file or plug-in file format can be specified.
      >0: Only Rhino .3dm files with version >= minimum_rhino_3dm_version_rescrition
          can be specified.
    fwo_out - [out]
      File writing options including file name and other writing options
      the user may have set or changed.
    pWndParent - [in]
      Expert users can set a parent window. In general, leave this
      parameter set to zero and the Rhino mainframe will automatically
      be the parent.
  Example:
            CRhinoFileWriteOption fwo;
            fwo.Set...() // stuff you want to set up before the dialog is shown.
            if ( doc.WriteFilePathAndOptionsDialog(fwo,true or false,fwo) && doc.WriteFile(fwo,&fwo) )
            {
              ON_wString filename;
              fwo.GetFileName(filename);
              Print("Model successfully saved in a file called %s\n",filename);
            }
  Returns:
    True:
      The file name and other file writing options are returned in fwo_out.
    False:
      The user did not select a file name or other failure.
  */
  bool WriteFilePathAndOptionsDialog(
    CRhinoFileWriteOptions fwo_in,
    unsigned int minimum_rhino_3dm_version_rescrition,
    CRhinoFileWriteOptions& fwo_out,
    HWND pWndParent = 0
  ) const;

  /*
  Description:
    Write information in this model to a file.
  Parameters:
    fwo - [in]
      file writing options
    fwo_out - [out]
      When you need to know the actual file writing options
      that were used, then call the version with an fwo_out
      parameter.  The exact details about what options were
      used and the file writing status information is returned
      here.
  Returns:
    True:
      File writing succeeded.
    False:
      File writing failed.
  Example:

            // Get filename and export options.
            CRhinoFileWriteOptions fwo;

            // Get file name:
            doc.WriteFilePathAndOptionsDialog(fwo,false,fwo);

            // Write file
            doc.WriteFile(fwo,fwo);



            // Simple way to write a myFileName.3dm file.
            CRhinoFileWriteOptions fwo;
            fwo.SetFileName("myFileName.3dm");
            doc.WriteFile(fwo);

  Remarks:
    This is the best choice for general file writing.
    It handles making backups using temporary files,
    locking and unlocking, loading file writing plug-ins,
    and many other details.
  */
  bool WriteFile(
    CRhinoFileWriteOptions fwo_in
  );

  bool WriteFile(
    CRhinoFileWriteOptions fwo_in,
    CRhinoFileWriteOptions& fwo_out
  );

  /*
  Description:
    Use CRhinoDoc::GetWriteFileOptonsAndWrite() to get a file name and
    additional file writing options and then write the file.
  Parameters:
    fwo - [in]
      file writing options
    fwo_out - [out]
      When you need to know the actual file writing options
      that were used, then call the version with an fwo_out
      parameter.  The exact details about what options were
      used and the file writing status information is returned
      here.
  Remarks:
    GetWriteFileOptonsAndWrite(fwo_in,fwo_out) calls
      CRhinoDoc::WriteFilePathAndOptionsDialog(fwo_in,false,fwo_out)
    to get the file name and file writing options and then calls
      CRhinoDoc::WriteFile(fwo_out,fwo_out)
    to write the file.
  */
  bool GetWriteFileOptonsAndWrite(
    CRhinoFileWriteOptions fwo_in
  );

  bool GetWriteFileOptonsAndWrite(
    CRhinoFileWriteOptions fwo_in,
    CRhinoFileWriteOptions& fwo_out
  );

  /*
  Description:
    Write active objects in the current document to an Rhino 3dm file.
  Parameters:
    binary_archive - [in] archive to write to
    fwo_in - [in]
      file writing options.
    fwo_out - [out]
      Complete file writing options that were used and additional
      status information.
 Returns:
   True:
     Write succeeded.
     See fwo_out for detailed information about the options that were used.
   False:
     Write failed.  See fwo_out for detailed information about the failure.
  */
  bool Write3dmFile(
    ON_BinaryArchive& binary_archive,
    CRhinoFileWriteOptions fwo_in
  );

  bool Write3dmFile(
    ON_BinaryArchive& binary_archive,
    CRhinoFileWriteOptions fwo_in,
    CRhinoFileWriteOptions& fwo_out
  );

  /*
  Description:
    This is the function that is used to "save" the document.
    If this function does not work for you, then call CRhinoDoc::WriteFile().
  Parameters:
    bUseBatchMode - [in]
  Returns:
    True:
      Success.
    False:
      Failure.
  */
  bool SaveDocument(
    bool bUseBatchMode
  );

#if defined (RHINO_MAC_APPLICATION)
  // For Mac Rhino internal use only
  bool _InternalWriteMacFile(CRhinoFileWriteOptions& fwo, NSSaveOperationType saveOperation, NSError** outError);
#endif

private:
  bool _WriteFile_Non3dmHelper(
    CRhinoFileWriteOptions fwo_in,
    CRhinoFileWriteOptions& fwo_out
  );

public:

  /*
  Description:
    Core Rhino code uses this this function to read 3dm archives.
  Remarks:
    Plug-ins cannot use this function because they cannot
    create CRhFileReadOptions classes. This is intentional.
  */
  ON::ReadFileResult Read3DM(
    ON_BinaryArchive& archive,
    class CRhFileReadOptions& fro3dm
  );

private:
  /*
  Parameters:
    begin_dimstyle_search_at_sn - [in]
      To test all dimstyles, pass 0.
      To test no dimstyles, pass 0xFFFFFFFFU.
      To test all dimstyles created during some event,
      save the value of CRhinoDimStyle::NextRuntimeSerialNumber() before the event
      and then call GetMissingFontLists() after the event with the saved value as the
      this parameter.
    begin_object_search_at_sn - [in]
      To test all objects, pass 0.
      To test no objects, pass 0xFFFFFFFFU.
      To test all objects created during some event,
      save the value of CRhinoObject::NextRuntimeObjectSerialNumber() before the event
      and then call GetMissingFontLists() after the event with the saved value as the
      this parameter.
    missing_fonts - [out]
      If missing_fonts is not nullptr, then
      missing_fonts[] is a list of managed fonts that are missing on the current device.
    dimstyles_with_missing_fonts - [out]
      If dimstyles_with_missing_fonts is not nullptr, then
      dimstyles_with_missing_fonts[] will be the list of dimstyles with missing fonts.
    objects_with_missing_fonts - [out]
      If objects_with_missing_fonts is not nullptr, then
      objects_with_missing_fonts[] will be the list of objects with missing fonts.
  Returns:
    Number of fonts referenced in the searched dimstyles and objects that
    are not installed on the current device.
  Remarks:
    Deleted dimstyles and deleted objects are always ignored.
  */
  unsigned int GetMissingFontList(
    unsigned int begin_dimstyle_search_at_sn,
    unsigned int begin_object_search_at_sn,
    ON_SimpleArray< const ON_Font* >* missing_fonts,
    ON_SimpleArray< const ON_DimStyle* >* dimstyles_with_missing_fonts,
    ON_SimpleArray< const CRhinoObject* >* objects_with_missing_fonts
  ) const;

public:

  /*
  Parameters:
    bInteractive - [in]
      If true and missing fonts are found, a modeless dialog UI is used to inform the
      user and ask what to do about the missing fonts.
    begin_dimstyle_search_at_sn - [in]
      To test all dimstyles, pass 0.
      To test no dimstyles, pass 0xFFFFFFFFU.
      To test all dimstyles created during some event,
      save the value of CRhinoDimStyle::NextRuntimeSerialNumber() before the event
      and then call GetMissingFontLists() after the event with the saved value as the
      this parameter.
    begin_object_search_at_sn - [in]
      To test all objects, pass 0.
      To test no objects, pass 0xFFFFFFFFU.
      To test all objects created during some event,
      save the value of CRhinoObject::NextRuntimeObjectSerialNumber() before the event
      and then call GetMissingFontLists() after the event with the saved value as the
      this parameter.
  Returns:
    Number of fonts referenced in the searched dimstyles and objects that
    are not installed on the current device.
  Remarks:
    Deleted dimstyles and deleted objects are always ignored.
*/
  unsigned int DocumentMissingFontTest(
    bool bInteractive,
    unsigned int begin_dimstyle_search_at_sn,
    unsigned int begin_object_search_at_sn
  );

  /*
  Description:
    Core Rhino code uses this this function to call file
    reading plug-ins to read files.
  Remarks:
    Plug-ins cannot use this function because they cannot
    create CRhFileReadOptions classes. This is intentional.
  */
  bool ReadPlugInFile(
    class CRhinoFileImportPlugIn* import_plug_in,
    int file_extension_index,
    class CRhFileReadOptions& fro
  );

  /*
  Returns:
    The value returned by the most recent call to Read3DM() or ReadPlugInFile().
  */
  ON::ReadFileResult ReadFileResult() const;

  /*
  Description:
    Set the value returned by ReadFileResult() to CRhinoDoc::eReadFileResult::Unset.
    Typically this happens after Rhino has handled the read in whatever way is appropriate.
  */
  void ClearReadFileResult();

  /*
  Description:
    Set the value returned by ReadFileResult() to CRhinoDoc::eReadFileResult::Unset.
    Typically this happens after Rhino has handled the read in whatever way is appropriate.
  */
  void SetReadFileResult(
    ON::ReadFileResult read_file_result
  );

private:
  void Destroy();

public:
  /*
  Returns:
    Model space absolute tolerance.
  Remarks:
    Use DocProperties().PageUnitsAndTolerances() for page space.
  */
  double AbsoluteTolerance() const;

  /*
  Returns:
    Model space angle tolerance.
  Remarks:
    Use DocProperties().PageUnitsAndTolerances() for page space.
  */
  double AngleToleranceRadians() const;

  /*
  Returns:
    Model space angle tolerance.
  Remarks:
    Use DocProperties().PageUnitsAndTolerances() for page space.
  */
  double AngleToleranceDegrees() const;

  /*
  Returns:
    Model space unit system.
  Remarks:
    Use DocProperties().PageUnitsAndTolerances() for page space.
  */
  ON::LengthUnitSystem UnitSystem() const; // OBSOLETE - USE ModelUnits or PageUnits

  const ON_UnitSystem& ModelUnits() const;

  const ON_UnitSystem& PageUnits() const;

  const ON_UnitSystem ActiveSpaceUnits() const;

  ON::active_space ActiveSpace() const;

  //////////
  // Return current document properties
  //
  CRhinoDocProperties& Properties();
  const CRhinoDocProperties& Properties() const;

  void Get3dmSettings(
    class ON_3dmSettings& settings
  ) const;

  void Get3dmAnnotationContext(
    class ON_3dmAnnotationContext& annotation_context
  ) const;

  const class IRhRdkContents& Contents() const;

  /*  ##########################################################################################################

      D E P R E C A T E D   D O C U M E N T   O B J E C T   A C C E S S   M E T H O D S
      ---------------------------------------------------------------------------------
      These methods are deprecated in favor of using the document's render settings. The new way to use this
      is to call CRhinoDoc::Properties().RenderSettings() to get a reference to a const ON_3dmRenderSettings
      object. You can then call one of the following functions depending on what you're trying to do:

      - For access to Sun properties, call ON_3dmRenderSettings::Sun().

      - For access to Ground Plane properties, call ON_3dmRenderSettings::GroundPlane().

      - For access to Safe Frame properties, call ON_3dmRenderSettings::SafeFrame().

      - For access to Skylight properties, call ON_3dmRenderSettings::Skylight().

      - For access to Linear Workflow properties, call ON_3dmRenderSettings::LinearWorkflow().

      - For access to Dithering properties, call ON_3dmRenderSettings::Dithering().

      - For access to Render Channels properties, call ON_3dmRenderSettings::RenderChannels().

      - For access to Post Effect properties, call ON_3dmRenderSettings::PostEffects().

      - For access to Current Environment properties, call one of the following:

        ON_3dmRenderSettings::RenderEnvironmentOverride()
        ON_3dmRenderSettings::SetRenderEnvironmentOverride()
        ON_3dmRenderSettings::RenderEnvironmentId()
        ON_3dmRenderSettings::SetRenderEnvironmentId()

      When reading, you can simply call the desired access method and then one of its const methods, for example:

        const auto& rs = doc->Properties().RenderSettings();
        const auto latitude = rs.Sun().Latitude(); // Call Sun() access method and then const method Latitude().

      When you want to modify a value, you must first copy the render settings, modify the copy, and then set
      the copy back into the document. For example:

        ON_3dmRenderSettings rs_write = rs;
        rs_write.Sun().SetLatitude(51.0); // Non-const method SetLatitude()  must be called on the non-const copy.
        rs_write.Sun().SetLongitude(0.0); // Non-const method SetLongitude() must be called on the non-const copy.
        ...                               // You can change any number of other properties here.
        doc->Properties().SetRenderSettings(rs_write); // Commit the changes to the document.

      For extensive examples of how to use these methods, see the samples repository at:

      https://github.com/mcneel/rhino-developer-samples/blob/8/cpp/SampleCommands/cmdSampleRenderSettings.cpp

  */

  ON_DEPRECATED_MSG("Use Properties().RenderSettings() functions (Sun, Skylight etc. See comments above.")
  const class CRhRdkDocument& RdkDocument() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().Sun() instead. See comments above.")
  const class IRhRdkSun& Sun() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().Skylight() instead. See comments above.")
  const class IRhRdkSkylight& Skylight() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().GroundPlane() instead. See comments above.")
  const class IRhRdkGroundPlane& GroundPlane() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().SafeFrame() instead. See comments above.")
  const class IRhRdkSafeFrame& SafeFrame() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings() Render Environment functions instead. See comments above.")
  const class IRhRdkCurrentEnvironment& CurrentEnvironment() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().LinearWorkflow() instead. See comments above.")
  const class IRhRdkLinearWorkflow& LinearWorkflow() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().Dithering() instead. See comments above.")
  const class IRhRdkDithering& Dithering() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().RenderChannels() instead. See comments above.")
  const class IRhRdkRenderChannels& RenderChannels() const;

  ON_DEPRECATED_MSG("Use Properties().RenderSettings().PostEffects() instead. See comments above.")
  const class IRhRdkPostEffects& PostEffects() const;

  //##########################################################################################################

  /*
  Description:
    Find a content by its instance id. Top-level contents that are attached to the document will be checked.
  Parameters:
    instanceId - [in] the instance id of the content to find.
  Returns:
    A pointer to the const content or null if not found. If you want to modify the content
    you will need to call BeginChange() to get a non-const reference.
  */
  ON_DEPRECATED_MSG("Use Contents().Find() instead")
  const class CRhRdkContent* FindContentInstance(const ON_UUID& instanceId) const;

  virtual TextureReportResult TextureReport(TextureReportFilter initialFilter, bool bAllowAbort, bool bForceDisplayIfEmpty);

  virtual void GetEmbeddedFilesList(TextureReportFilter filter, ON_ClassArray<ON_wString>& sFiles) const;

  //////////
  // Audit the contents of the model.  Returns true if model is valid.
  // If an error is detected and the ON_TextLog pointer is not NULL,
  // then a description of the error is logged.  If bAttemptRepair
  // is true, than Audit attempts to repair any problems it detects.
  bool Audit(
    ON_TextLog* text_log,
    bool bAttemptRepair
  );

private:
  bool Internal_AuditTableManifest(
    ON_ModelComponent::Type component_type,
    ON_TextLog* text_log,
    unsigned int& active_component_count,
    unsigned int& deleted_component_count
  ) const;

  bool Internal_AuditObjectListManifest(
    ON_ModelComponent::Type component_type,
    ON_TextLog* text_log,
    unsigned int& active_component_count,
    unsigned int& deleted_component_count
  ) const;

  bool Internal_AuditRhinoObjectManifestItem(
    ON_ModelComponent::Type component_type,
    int table_index,
    const CRhinoObject* object,
    ON_SimpleArray<ON_UUID>& active_object_id_list,
    ON_SimpleArray<ON_UUID>& deleted_object_id_list,
    ON_TextLog* text_log
  ) const;

public:

  void SetModelName(const char*);
  void SetModelName(const wchar_t*);
  const wchar_t* ModelName() const;

  /*
  Description:
    If you change a view, then call ViewModified.
  Parameters:
    view - [in] view that was modified.
  */
  void ViewModified(CRhinoView*);

  /*
  Description:
    Creates a new Rhino view.
  Parameters:
    view - [in]
      null or settings to use to initialize the view.
    pMfcViewRuntimeClass - [in]
      null or a runtime class derived from CRhinoView.
    bShowView - [in]
      If false, the view will not be shown.  This is what you
      should do if you need to perform additional initialization
      on the view before it appears.
      If true, the view will be shown and updated.
  Returns:
    The runtime serial number of a view or
    zero if the input was not valid.
  Remarks:
    Use CRhinoView::FromRuntimeSerialNumber() to get a pointer to the view.
  */
  unsigned int CreateRhinoView(
    const ON_3dmView* view,
    bool bShowView
  );

#if defined (RHINO_SDK_MFC)
  unsigned int CreateRhinoView(
    const ON_3dmView* view,
    CRuntimeClass* pMfcViewRuntimeClass,
    bool bShowView
  );
#endif

  // Description:
  //   Create a new page view window.
  // Parameters:
  //   title: [in] string that appears in the new page view's title window
  //          If NULL, the name will be generated as "Page #" where this is the
  //          largest page number.
  //Returns:
  //  The runtime serial number of a view or
  //    zero if the input was not valid.
  //    Remarks :
  //    Use CRhinoView::FromRuntimeSerialNumber() to get a pointer to the view.
  unsigned int CreateRhinoPageView(const wchar_t* title = nullptr);


  // Description:
  //  Returns sorted list of all the page views in this model. The sort
  //  order can be considered the page number order
  const ON_SimpleArray<class CRhinoPageView*>& SortedPageViewList();

  // Description:
  //   Append page view window to list of page views for this model.
  // Parameters:
  //   pageView: [in] CRhinoPageView to be added
  void AppendPageView(class CRhinoPageView& pageView);

  // Description:
  //   Remove page view window from list of page views for this model.
  // Parameters:
  //   pageView: [in] CRhinoPageView to be added
  void RemovePageView(class CRhinoPageView& pageView);

  // Description:
  //   Get page number of a page view window for this model.
  // Parameters:
  //   pageView: [in] CRhinoPageView to be found
  // Returns:
  //   page number (starting at 0) of the pageView on success
  //   -1 on failure
  int GetPageViewNumber(const class CRhinoPageView& pageView) const;

  // Description:
  //   Set page number of a page view window for this model.
  // Parameters:
  //   pageView: [in] CRhinoPageView to be changed
  //       page: [in] new page number for pageView
  void SetPageViewNumber(class CRhinoPageView& pageView, int page);

  void PageViewDetailIsActive(BOOL32 isActive);

#if defined (RHINO_MAC_APPLICATION)
public:
  id GetMRDocument() const;       // MRDocument object for this document
  void SetMRDocument(MRDocument* doc);

  void DeleteAllObjectProxies();

protected:
  void DeleteObjectProxies(CRhinoObject* obj);

  // The __weak attribute gets a compiler warning in the Toucan and QuickLook projects.  Do not remove.
  // The attribute does not get a compiler warning in the MacRhino project and is necessary to prevent storage leaks.
  id __weak m_mrDocument;    // Objective C MRDocument pointer
#endif

private:
  // must be before tables
  ON_ComponentManifest m_doc_manifest;

private:
  ON_ComponentManifest& Internal_Manifest();

public:
  /*
  Description:
    When a component is being added to a model, there are cases when the name will be modified.
    If the component is from a reference file like a worksession reference or a linked instance definition with reference layers,
    then a prefix is added to the name.
    If the name is empty or not unique and bMakeUniqueAndValid is true, then a suffix is added to the name.
  Parameters:
    component_type - [in]
      Type of component.
    bMakeUniqueAndValid - [in]
      If true, then empty names and names already in use are modified to be valid and unique.
    candidate_name - [in]
      Candidate for the component name.
  Remarks:
    This function is used internally when block definitions, annotation styles, line patterns,
    and hatch patterns are added to the document. When something like a file reading plug-in
    must know what name will be assigned to the component ahead of time, it can use this function.
  */
  const ON_wString ComponentNameFromCandidateName(
    ON_ModelComponent::Type component_type,
    bool bMakeUniqueAndValid,
    const wchar_t* candidate_name
  ) const;


public:
  /*
  Description:
    A manifest of all model components, including system components.
  Remarks:
    Examples of "system" components are

      CRhinoLayerTable.DefaultLayer

      CRhinoDimStyleTable.DefaultDimensionStyle
      CRhinoDimStyleTable.DefaultInchDecimal
      CRhinoDimStyleTable.DefaultInchFractional
      CRhinoDimStyleTable.DefaultFootInchArchitecture
      CRhinoDimStyleTable.DefaultMillimeterSmall
      CRhinoDimStyleTable.DefaultMillimeterLarge
      CRhinoDimStyleTable.DefaultMillimeterArchitecture
  */
  const ON_ComponentManifest& Manifest() const;

  const CRhinoDocTableReference& Table(
    ON_ModelComponent::Type component_type
  ) const;

  /*
  Description:
    This is a tool to determine if a model_component is in this CRhinoDoc
    and correctly entered in the manifest. It is useful for debugging and
    testing.
  Parameters:
    model_component - [in]
    table_index - [in]
      If table_index != ON_UNSET_INT_INDEX and the component requires a unique index,
      then the index is tested for validity.
    error_description - [out]
      If an error is detected, a description is returned here.
  Returns:
    If the model_component is in this CRhinoDoc and everything is correctly set,
    then true is returned. Otherwise false is returned.
  */
  bool ModelComponentCheck(
    const ON_ModelComponent* model_component,
    int table_index,
    ON_wString* error_description
  ) const;

private:
  // must be before tables

  /////////////////////////////////////////////////////////////
  //
  // Information used to support Undo/Redo
  //
  int m_undo_count = 0; // number of undo records
  size_t m_undo_memory_size = 0;      // number of bytes used by the undo list and
                                  // deleted objects
  class CRhUndoRecord* m_undo_record = nullptr;
  class CRhUndoRecord* m_redo_record = nullptr;
  bool m_bUndoRecordingEnabled = false;   // true if recording will happen when commands run
  bool m_bUndoRecordingActive = false;    // true if a current events are being recorded into
                                  // a new CRhUndoRecord.  Typically, this means a 
                                  // command is currently running and undo recording
                                  // is enabled.
  bool m_bUndoPurgeCheckRequired = false; // true if objects were deleted while undo
                                  // recording was turned off.
  bool m_bUndoIdefCompactRequired = false;// reserved for future use.

private:
  ON__UINT32 m_reserved1 = 0;

private:
  class CRhFile* m_pFileLock = nullptr; // Used to lock open files to prevent multiple users from writing the same file
  class CFileException* m_pFileException; // used when creating m_pFileLock
  bool m_bReadOnly = false; // If true save will not be allowed

private:
  // Use InternalSetReadFileResult() to set m_xReadFileResult.
  // Use CRhinoDoc.ReadFileResult() to get.
  // Use CRhinoDoc.ClearReadFileResult() to set to unset.
  ON::ReadFileResult m_xReadFileResult = ON::ReadFileResult::Unset;
  void Internal_SetReadFileResult(
    ON::ReadFileResult read_file_result
  );

private:
  // SubD Creamy/Crunchy appearance settings.
  // Creamy = surface display
  // Crunchy = control polygon display
  ON_SubDComponentLocation m_subd_appearance = ON_SubDComponentLocation::Surface;
public:
  /*
  Returns:
      ON_SubDComponentLocation::Surface - SubD display will be smooth shaded surfaces.
      ON_SubDComponentLocation::ControlNet - SubD display will be flat shaded control polygons.
  Remarks:
    SubDAppearance() is a runtime property. It is not saved in 3dm files.
  */
  ON_SubDComponentLocation SubDAppearance() const;

  /*
  Parameters:
    subd_appearance - [in]
      ON_SubDComponentLocation::Surface - SubD display will be smooth shaded surfaces.
      ON_SubDComponentLocation::ControlNet - SubD display will be flat shaded control polygons.
  Returns:
    Number of subds whose appearance changed.
  Remarks:
    SubDAppearance() is a runtime property. It is not saved in 3dm files.
  */
  unsigned int SetSubDAppearance(
    ON_SubDComponentLocation subd_appearance
  );

private:
  unsigned char m_reserved2b = 0; // for future use

private:
#if defined (ON_RUNTIME_APPLE)
public:
#endif

  /*
  Description:
    Lock document so no other occurrence of Rhino will be allowed to write over it.
    This will only succeed if the document may be opened in read/write mode.
  Parameters:
    lpsFileName [in] Name of file to lock
  Returns:
    Returns true if the document is locked otherwise false.
  See Also:
    CRhinoDoc::UnLockDocument
    CRhinoDoc::IsDocumentLocked
  */
  bool LockDocument(LPCTSTR lpsFileName);
  // Added to allow for calling a different dialog to query about 
  // opening files read-only and not changing LockDocument()
  // LockDocumentEx() will use the new dialog if called with bOldMode false rr59912
  bool LockDocumentEx(LPCTSTR lpsFileName, bool bOldMode);
  /*
  Description:
    Remove file lock associated with this document.
  Returns:
    Returns previous lock state.
  See Also:
    CRhinoDoc::LockDocument
    CRhinoDoc::IsDocumentLocked
  */
  bool UnlockDocument();
public:
  /*
  Description:
    This method identifies the current read-only mode for this document.
  Returns:
    Returns true if the document is can be viewed but NOT saved.  Returns false
    if document can be viewed and saved.
  */
  bool IsDocumentReadOnly() const;
  /*
  Description:
    Check to see if the file associated with this document is locked.  If it is
    locked then this is the only document that will be able to write the file.  Other
    instances of Rhino will fail to write this document.
  Returns:
    Returns true if the document is locked otherwise false.
  See Also:
    CRhinoDoc::LockDocument
    CRhinoDoc::UnLockDocument
  */
  bool IsDocumentLocked() const;
  /*
  Description:
    Return file version extracted by CRhinoDoc::Read3DM().  Will not return
    values from referenced or merged files.
  Returns:
    Less than zero means no file has bee read otherwise 1 (V1.x), 2( V2.x) or 3
  See Also:
    CRhinoDoc::Read3DM
  */
  int ReadFileVersion() const;

  /////////////////////////////////////////////////////////////////////////
  // bitmaps used in textures, backgrounds, wallpapers, ...
  //
  CRhinoBitmapTable m_bitmap_table;

  /////////////////////////////////////////////////////////////////////////
  // texture mappings
  //
  CRhinoTextureMappingTable m_texture_mapping_table;

  /////////////////////////////////////////////////////////////////////////
  // rendering materials
  //
  CRhinoMaterialTable m_material_table;

  /////////////////////////////////////////////////////////////////////////
  // linetypes
  //
  CRhinoLinetypeTable m_linetype_table;

  /////////////////////////////////////////////////////////////////////////
  // layers
  //
  CRhinoLayerTable m_layer_table;

  /////////////////////////////////////////////////////////////////////////
  //
  // groups 
  //
  CRhinoGroupTable m_group_table;

  /////////////////////////////////////////////////////////////////////////
  //
  // dimension styles used in annotation objects 
  //
  CRhinoDimStyleTable m_dimstyle_table;
  
#if defined(INCLUDE_RHINO_SECTION_STYLE_TABLE)
  class CRhinoSectionStyleTable& SectionStyleTable();
  const class CRhinoSectionStyleTable& SectionStyleTable() const;
#endif

private:
  unsigned int m_reserved4; // for future use

public:

  /////////////////////////////////////////////////////////////////////////
  //
  // rendering lights
  //
  CRhinoLightTable m_light_table;

  /////////////////////////////////////////////////////////////////////////
  //
  // hatch patterns used to fill hatch objects 
  //
  CRhinoHatchPatternTable m_hatchpattern_table;

  /////////////////////////////////////////////////////////////////////////
  //
  // instance definitions (blocks for you AutoCAD types)
  //
  CRhinoInstanceDefinitionTable m_instance_definition_table;

  /////////////////////////////////////////////////////////////////////////
  //
  // instance definitions (blocks for you AutoCAD types)
  //
  CRhinoHistoryRecordTable m_history_record_table;

  /////////////////////////////////////////////////////////////////////////
  //
  // user key-value string pairs on the document
  //

  /*
  Description:
    Attach a user string to the document.
  Parameters:
    key - [in] id used to retrieve this string.
    string_value - [in]
      If NULL, the string with this id will be removed.
  Returns:
    True if successful.
  */
  bool SetUserString(
    const wchar_t* key,
    const wchar_t* string_value
  );

  /*
  Description:
    Get user string from the document.
  Parameters:
    key - [in] id used to retrieve the string.
    string_value - [out]
  Returns:
    True if a string with id was found.
  */
  bool GetUserString(
    const wchar_t* key,
    ON_wString& string_value
  ) const;

  /*
  Description:
    Get a list of all user strings on the document.
  Parameters:
    user_strings - [out]
      user strings are appended to this list.
  Returns:
    Number of elements appended to the user_strings list.
  */
  int GetUserStrings(
    ON_ClassArray<ON_UserString>& user_strings
  ) const;

  /*
  Description:
    Get a list of all user string keys on the document.
  Parameters:
    user_string_keys - [out]
      user string keys are appended to this list.
  Returns:
    Number of elements appended to the user_strings list.
  */
  int GetUserStringKeys(
    ON_ClassArray<ON_wString>& user_string_keys
  ) const;

  /////////////////////////////////////////////////////////////////////////
  // objects (geometry, annotation, etc.)
  //

  // use CRhinoObjectIterator to iterate through CRhinoObject list(s)

  // object search

  /*
  Remarks:
    Obsolete function.  Use these static functions.
    CRhinoObject::FromId()
    CRhinoObject::FromRuntimeSerialNumber()
  */
  const CRhinoObject* LookupObject(
    ON_UUID object_uuid
  ) const;

  /*
  Description:
    Lookup any document object (geometry, layers, materials, fonts, ...)
    by id.
  Returns:
    NULL or a pointer to the document object.
  */
  const ON_Object* LookupDocumentObject(
    ON_UUID id,
    bool bIgnoreDeleted
  ) const;


  /*
  Remarks:
    Obsolete function.  Use these static functions.
    CRhinoObject::FromId()
    CRhinoObject::FromRuntimeSerialNumber()
  */
  const CRhinoObject* LookupObjectByRuntimeSerialNumber(
    unsigned int runtime_serial_number
  ) const;


  /*
  Remarks:
    Obsolete (and slow) function.  Use these static functions.
    CRhinoObject::FromId()
    CRhinoObject::FromRuntimeSerialNumber()
    Then check the result to see if you want to use it.
  */
  const CRhinoObject* LookupObjectByRuntimeSerialNumber(
    unsigned int runtime_serial_number,
    CRhinoObjectIterator& it
  ) const;

  int LookupObject(const CRhinoLayer&, ON_SimpleArray<CRhinoObject*>&) const;
  int LookupObject(const CRhinoLayer&, ON_SimpleArray<const CRhinoObject*>&) const;
  int LookupObject(const CRhinoLinetype&, ON_SimpleArray<CRhinoObject*>&) const;
  int LookupObject(const CRhinoGroup&, ON_SimpleArray<CRhinoObject*>&) const;
  int LookupObject(const CRhinoMaterial&, ON_SimpleArray<CRhinoObject*>&) const;

  /*
  Description:
    Add a point object to Rhino.
  Parameters:
    point - [in] (point is copied)
    pAttributes - [in] (optional) attributes.  If NULL,
        the current default attributes are used.
    pHistory - [in] (optional) history
    bReference - [in] true if the point object is from
        a reference file.  Reference objects do not
        persist in archives.
  Remarks:
    This is an easy to use wrapper that creates
    a new CRhinoPointObject and calls CRhinoDoc::AddObject.
    The input point is copied and should be deleted by the
    caller when its no longer needed.
  See Also:
    CRhinoDoc::AddPointObject
    CRhinoDoc::AddPointCloudObject
    CRhinoDoc::AddCurveObject
    CRhinoDoc::AddSurfaceObject
    CRhinoDoc::AddMeshObject
    CRhinoDoc::AddBrepObject
    CRhinoDoc::ReplaceObject
    CRhinoPointObject::CRhinoPointObject()
    CRhinoDoc::AddObject
  */
  CRhinoPointObject* AddPointObject(
    const ON_3dPoint& point,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a point cloud object to Rhino.
  // Parameters:
  //   point_count - [in] number of points in list
  //   point_list - [in] array of 3d points
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   pHistory - [in] (optional) history
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoPointCloudObject and calls CRhinoDoc::AddObject.
  //   The input point_list is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoPointCloudObjectObject::CRhinoPointCloudObjectObject()
  //   CRhinoDoc::AddObject
  CRhinoPointCloudObject* AddPointCloudObject(
    int point_count,
    const ON_3dPoint* point_list,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a line object to Rhino.
  // Parameters:
  //   line - [in] (line is copied)
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   pHistory - [in] (optional) history
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoCurveObject and calls CRhinoDoc::AddObject.
  //   The input line is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoCurveObject* AddCurveObject(
    const ON_Line& line,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a polyline object to Rhino.
  // Parameters:
  //   polyline - [in]
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   pHistory - [in] (optional) history
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoCurveObject and calls CRhinoDoc::AddObject.
  //   The input polyline is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoCurveObject* AddCurveObject(
    const ON_Polyline& polyline,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add an arc object to Rhino.
  // Parameters:
  //   arc - [in]
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   pHistory - [in] (optional) history
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoCurveObject and calls CRhinoDoc::AddObject.
  //   The input arc is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoCurveObject* AddCurveObject(
    const ON_Arc& arc,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add an circle object to Rhino.
  // Parameters:
  //   circle - [in]
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   pHistory - [in] (optional) history
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoCurveObject and calls CRhinoDoc::AddObject.
  //   The input circle is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoCurveObject* AddCurveObject(
    const ON_Circle& circle,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a Bezier curve object to Rhino.
  // Parameters:
  //   bezier_curve - [in]
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoCurveObject and calls CRhinoDoc::AddObject.
  //   The input bezier_curve is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoCurveObject* AddCurveObject(
    const ON_BezierCurve& bezier_curve,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a curve object to Rhino.
  // Parameters:
  //   curve - [in] A duplicate of this curve is added to
  //      Rhino.
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoCurveObject and calls CRhinoDoc::AddObject.
  //   The input curve is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoCurveObject* AddCurveObject(
    const ON_Curve& curve,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add an extrusion object to Rhino.
  // Parameters:
  //   extrusion - [in] A duplicate of this extrusion is added to
  //      Rhino.
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoExtrusionObject and calls CRhinoDoc::AddObject.
  //   The input extrusion is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoCurveObject::CRhinoCurveObject()
  //   CRhinoDoc::AddObject
  CRhinoExtrusionObject* AddExtrusionObject(
    const ON_Extrusion& extrusion,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    bool bReference = 0
  );

  // Description:
    //   Add a surface object to Rhino.
    // Parameters:
    //   surface - [in] A duplicate of this surface is added to
    //      Rhino.
    //   pAttributes - [in] (optional) attributes.  If NULL,
    //       the current default attributes are used.
    //   bReference - [in] true if the point object is from
    //       a reference file.  Reference objects do not
    //       persist in archives.
    // Remarks:
    //   This is an easy to use wrapper that creates
    //   a new CRhinoSurfaceObject and calls CRhinoDoc::AddObject.
    //   The input surface is copied and should be deleted by the
    //   caller when its no longer needed.
    // See Also:
    //   CRhinoDoc::AddPointObject
    //   CRhinoDoc::AddPointCloudObject
    //   CRhinoDoc::AddCurveObject
    //   CRhinoDoc::AddSurfaceObject
    //   CRhinoDoc::AddMeshObject
    //   CRhinoDoc::AddBrepObject
    //   CRhinoDoc::ReplaceObject
    //   CRhinoSurfaceObject::CRhinoSurfaceObject()
    //   CRhinoDoc::AddObject
  CRhinoSurfaceObject* AddSurfaceObject(
    const ON_Surface& surface,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a mesh object to Rhino.
  // Parameters:
  //   mesh - [in] A duplicate of this mesh is added to
  //      Rhino.
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoMeshObject and calls CRhinoDoc::AddObject.
  //   The input mesh is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoMeshObject::CRhinoMeshObject()
  //   CRhinoDoc::AddObject
  CRhinoMeshObject* AddMeshObject(
    const ON_Mesh& mesh,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a mesh object to Rhino.
  // Parameters:
  //   mesh - [in] A duplicate of this mesh is added to
  //      Rhino.
  //   bRequireValidMesh - [in] If true, the mesh will not be 
  //      added to the Rhino document if it fails a validity check.
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //      the current default attributes are used.
  //   bReference - [in] true if the point object is from
  //      a reference file.  Reference objects do not
  //      persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoMeshObject and calls CRhinoDoc::AddObject.
  //   The input mesh is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoMeshObject::CRhinoMeshObject()
  //   CRhinoDoc::AddObject
  CRhinoMeshObject* AddMeshObject(
    const ON_Mesh& mesh,
    bool bRequireValidMesh,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );

  // Description:
  //   Add a brep object to Rhino.
  // Parameters:
  //   brep - [in] A duplicate of this brep is added to
  //      Rhino.
  //   pAttributes - [in] (optional) attributes.  If NULL,
  //       the current default attributes are used.
  //   bReference - [in] true if the point object is from
  //       a reference file.  Reference objects do not
  //       persist in archives.
  // Remarks:
  //   This is an easy to use wrapper that creates
  //   a new CRhinoBrepObject and calls CRhinoDoc::AddObject.
  //   The input brep is copied and should be deleted by the
  //   caller when its no longer needed.
  // See Also:
  //   CRhinoDoc::AddPointObject
  //   CRhinoDoc::AddPointCloudObject
  //   CRhinoDoc::AddCurveObject
  //   CRhinoDoc::AddSurfaceObject
  //   CRhinoDoc::AddMeshObject
  //   CRhinoDoc::AddBrepObject
  //   CRhinoDoc::ReplaceObject
  //   CRhinoMeshObject::CRhinoMeshObject()
  //   CRhinoDoc::AddObject
  CRhinoBrepObject* AddBrepObject(
    const ON_Brep& brep,
    const ON_3dmObjectAttributes* pAttributes = NULL,
    CRhinoHistory* pHistory = NULL,
    BOOL32 bReference = 0
  );



  /*
  Returns:
    An class derived from ON_DimStyleContext that can be used
    whenever an ON_DimStyleContext& parameter is required.
  */
  const CRhinoDimStyleContext DimStyleContext() const;

  /*
  Description:
    Create a leader object and add it to the document.
  Parameters:
    leader_text - [in]
    plane - [in]
      ON_Plane::World_xy or the current view's construction plane are good candidates.
      The plane origin is ignored
    leader_polyline_point_count - [in]
    leader_polyline_points - [in]
      3d points (will be projected to the plane)
      First point is arrow tip and last point is text location.
    dim_style - [in]
      If nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The ON_DimStyle::CreateFromfProperties() function
      is useful for getting a good value for this parameter.
    attributes - [in]
      If nullptr, current defaults will be used.

  Example:

     const ON_3dPoint leader_points[3] =
     {
       ON_3dPoint(0,0,0), // arrow head
       ON_3dPoint(5,0,0),
       ON_3dPoint(10,5,0) // text location
     };

     // If you want to specify text appearance or other custom properties ...
     ON_DimStyle style = ON_DimStyle::DimStyleFromProperties( doc->DimStyleContext().CurrentDimStyle(), ... );

     doc->AddLeaderObject(
       L"Abra Cadabra",
       ON_Plane::World_xy,
       3,
       leader_points,
       &style,
       nullptr
       );

  Remarks:
    If you need to specify fancier settings, use the version of
    AddLeaderObject that has a ON_Leader parameter.
  */
  const class CRhinoLeader* AddLeaderObject(
    const wchar_t* leader_text,
    const ON_Plane& plane,
    int leader_polyline_point_count,
    const ON_3dPoint* leader_polyline_points,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  );

  /*
  Description:
    Create a leader object. Caller must add it to the document.
  Parameters:
    leader_text - [in]
    plane - [in]
      ON_Plane::World_xy or the current view's construction plane are good candidates.
      The plane origin is ignored
    leader_polyline_point_count - [in]
    leader_polyline_points - [in]
      3d points (will be projected to the plane)
      First point is arrow tip and last point is text location.
    dim_style - [in]
      If nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The ON_DimStyle::CreateFromProperties() function
      is useful for getting a good value for this parameter.
    attributes - [in]
      If nullptr, current defaults will be used.
  Remarks:
    If you need to specify fancier settings, use the version of
    AddLeaderObject that has a ON_Leader parameter.
  */
  class CRhinoLeader* CreateLeaderObject(
    const wchar_t* leader_text,
    const ON_Plane& plane,
    int leader_polyline_point_count,
    const ON_3dPoint* leader_polyline_points,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  ) const;

  /*
  Description:
    Create a leader object. Caller must add it to the document.
  Parameters:
    leader - [in]
    attributes - [in]
      If nullptr, current defaults will be used.
  Remarks:
    If you need a simple leader object with default settings,
    use the version of AddLeaderObject() that takes a string, plane and points.
    The CRhinoDoc.DimStyleFromTextProperties() function
    is useful for initializing dimension style settings.
  */
  class CRhinoLeader* CreateLeaderObject(
    const ON_Leader& leader,
    const ON_3dmObjectAttributes* attributes = nullptr
  ) const;

  /*
  Description:
    Create a text object and add it to the document.
  Parameters:
    text - [in]
    plane - [in]
      location specified by valign and halign is at plane.Origin().
    dim_style - [in]
      If nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The CRhinoDoc.DimStyleFromTextProperties() function
      is useful for initializing dimension style settings.

  Example:

     ON_DimStyle style;
     // If you want to specify text appearance  or other properties ...
     doc->DimStyleFromTextProperties( ON::AnnotationType::Text,...,&style);
     // If you want to use current appearance ...
     style = doc->m_dim_style_table.CurrentDimStyle();
     doc->AddLeaderObject(
       L"Abra Cadabra",
       ON_Plane::World_xy,
       ON_3dPoint(1,2,3),
       &style,
       nullptr
       );

  Remarks:
    If you need to specify fancier settings, use the version of
    AddTextObject that has a ON_Text parameter.
  */
  const class CRhinoText* AddTextObject(
    const wchar_t* text,
    const ON_Plane& plane,
    const ON_3dPoint text_location,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  );

  /*
  Description:
    Create a text object. Caller must add this object to the document.
  Parameters:
    text - [in]
    plane - [in]
      location specified by valign and halign is at plane.Origin().
    dim_style - [in]
      If nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The CRhinoDoc.DimStyleFromTextProperties() function
      is useful for initializing dimension style settings.

  Example:

     ON_DimStyle style;
     // If you want to specify text appearance  or other properties ...
     doc->DimStyleFromTextProperties( ON::AnnotationType::Text,...,&style);
     // If you want to use current appearance ...
     style = doc->m_dim_style_table.CurrentDimStyle();
     doc->AddLeaderObject(
       L"Abra Cadabra",
       ON_Plane::World_xy,
       ON_3dPoint(1,2,3),
       &style,
       nullptr
       );

  Remarks:
    If you need to specify fancier settings, use the version of
    AddTextObject that has a ON_Text parameter.
  */
  class CRhinoText* CreateTextObject(
    const wchar_t* text,
    const ON_Plane& plane,
    const ON_3dPoint text_location,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  ) const;

  /*
  Description:
    Create a text object. Caller must add this object to the document.
  Parameters:
    text - [in]
  Remarks:
    If you need a simple text object with default settings,
    use the version of AddTextObject() that takes a string and plane.
    The CRhinoDoc.DimStyleFromTextProperties() function
    is useful for initializing dimension style settings.
  */
  class CRhinoText* CreateTextObject(
    const ON_Text& text,
    const ON_3dmObjectAttributes* attributes = nullptr
  ) const;

  /*
  Description:
    Add an angular dimension to the document.
  Parameters:
    arc - [in]
    offset - [in]
      See the corresponding version of ON_DimAngular.Create() for a description of these parameters.
    dim_style - [in]
      nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The CRhinoDoc.DimStyleFromProperties() function
      is useful for initializing dimension style settings.
  */
  const class CRhinoDimAngular* AddDimAngularObject(
    ON_Arc arc,
    double offset,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  );

  /*
  Description:
    Add an angular dimension to the document.
  Parameters:
    arc - [in]
    offset - [in]
      See the corresponding version of ON_DimAngular.Create() for a description of these parameters.
    dim_style - [in]
      nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The CRhinoDoc.DimStyleFromProperties() function
      is useful for initializing dimension style settings.
  */
  const class CRhinoDimAngular* AddDimAngularObject(
    ON_Line line1,
    ON_3dPoint point_on_line1,
    ON_Line line2,
    ON_3dPoint point_on_line2,
    ON_3dPoint point_on_angular_dimension_arc,
    bool bSetExtensionPoints,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  );

  /*
  Description:
    Create an angular dimension object. Caller must add this object to the document.
  Parameters:
    dim_angular - [in]
    attributes - [in]
  Returns:
    A pointer of a CRhinoDimAngular that can be modified.
    You must use CRhinoDoc.AddObject() to add this to the document.
  Remarks:
    If you need a simple angular dimension with default settings,
    use one of the AddDimAnglularObject() functions.
  */
  class CRhinoDimAngular* CreateDimAngularObject(
    const ON_DimAngular& dim_angular,
    const ON_3dmObjectAttributes* attributes = nullptr
  ) const;


  /*
  Description:
    Add an aligned linear dimension to the document. The dimension line is
    parallel to the segment connecting the dimension points.
  Parameters:
    extension_point0 - [in]
    extension_point1 - [in]
      locations of one of the points being dimensioned.
      The dimension line will be parallel to the segment
      connecting these points.
    dimension_line_point - [in]
      a point on the linear dimension line.
    plane_normal - [in]
      A vector perpendicular to the line between the extension points
      that defines the orientation of the dimension's plane.
    dim_style - [in]
      nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The CRhinoDoc.DimStyleFromProperties() function
      is useful for initializing dimension style settings.
    attributes - [in]
      nullptr or object attributes.
  */
  const class CRhinoDimLinear* AddDimLinearObject(
    ON_3dPoint extension_point0,
    ON_3dPoint extension_point1,
    ON_3dPoint dimension_line_point,
    ON_3dVector plane_normal,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  );

  /*
  Description:
    Add a rotated linear dimension to the document.
    The dimension line is explicitly specified.
  Parameters:
    extension_point0 - [in]
    extension_point1 - [in]
      locations of one of the points being dimensioned.
      The dimension line will be parallel to the segment
      connecting these points.
    dimension_line - [in]
      the dimension line. This is treated as an infinite
      line and the points are automatically calculated.
    plane_normal - [in]
      A vector perpendicular to the line between the extension points
      that defines the orientation of the dimension's plane.
      The dimension line is projected to this plane.
    dim_style - [in]
      nullptr, document dimension style, or a valid override style.
      If invalid, then the current dimension style will be used.
      The CRhinoDoc.DimStyleFromProperties() function
      is useful for initializing dimension style settings.
    attributes - [in]
      nullptr or object attributes.
  */
  const class CRhinoDimLinear* AddDimLinearObject(
    ON_3dPoint extension_point0,
    ON_3dPoint extension_point1,
    ON_Line dimension_line,
    ON_3dVector plane_normal,
    const ON_DimStyle* dim_style = nullptr,
    const ON_3dmObjectAttributes* attributes = nullptr
  );

  /*
  Description:
    Create an linear dimension object. Caller must add this object to the document.
  Parameters:
    dim_linear - [in]
    attributes - [in]
  Returns:
    A pointer of a CRhinoDimLinear that can be modified.
    You must use CRhinoDoc.AddObject() to add this to the document.
  Remarks:
    If you need a simple linear dimension with default settings,
    use one of the AddDimAnglularObject() functions.
  */
  class CRhinoDimLinear* CreateDimLinearObject(
    const ON_DimLinear& dim_linear,
    const ON_3dmObjectAttributes* attributes = nullptr
  ) const;

private:
  bool Internal_PrepareAnnotationForAddObject(
    ON_Annotation* annotation
  ) const;

private:
  void Internal_GetAnnotationDimstyle(
    const ON_DimStyle* dim_style_candidate,
    ON_DimStyle& dim_style
  ) const;

public:

  /*
  Description:
    Adds an object to the document.  The attributes need to be set
    before you add the object.  Use GetDefaultObjectAttributes() to
    get the current default attributes.
  Parameters:
    object - [in]
    bReferenceObject - [in] if true, the object is added as a
        reference object that will not be saved
    bReferenceObject - [in] if true, the object is added as a
        reference object that will not be saved
    bInstanceDefinition - [in] if true, the object is added to the
        instance definition geometry list.  The instance definition
        geometry objects are never directly displayed.  They are
        are referenced by CRhinoInstanceObjects.

  Remarks:
    The object that is being added must be created by calling new
    to instantiate a class derived from CRhinoObject.  Classes
    like CRhinoPointObject, CRhinoCurveObject, CRhinoSurfaceObject,
    CRhinoMeshObject, and CRhinoBrepObject can be used.

  SeeAlso:
    CRhinoDoc::AddPointObject
    CRhinoDoc::AddPointCloudObject
    CRhinoDoc::AddCurveObject
    CRhinoDoc::AddSurfaceObject
    CRhinoDoc::AddMeshObject
    CRhinoDoc::AddBrepObject
    CRhinoPointObject::CRhinoPointObject()
    CRhinoPointCloudObject::CRhinoPointCloudObject()
    CRhinoCurveObject::CRhinoCurveObject()
    CRhinoSurfaceObject::CRhinoSurfaceObject()
    CRhinoBrepObject::CRhinoBrepObject()
    CRhinoMeshObject::CRhinoMeshObject()
    CRhinoDoc::ReplaceObject
    CRhinoDoc::AddObject
  */
  bool AddObject(
    CRhinoObject* object,
    bool bReferenceObject = false,
    bool bInstanceDefinition = false
  );

  /*
  Description:
    If objref.Object()->IsNormal() is true, then the object will be
    hidden.
  Parameters:
    objref - [in] reference to object to hide
    bIgnoreLayerMode - [in] if true, the object will be hidden even
       if it is on a layer that is locked or off.
  Returns:
    true if the object was successfully hidden.
  See Also:
    CRhinoDoc::ShowObject
    CRhinoDoc::LockObject
  */
  bool HideObject(const CRhinoObjRef& objref, bool bIgnoreLayerMode = false); // hides object in document

  /*
  Description:
    If objref.Object()->IsHidden() is true, then the object will be
    returned to normal (visible and selectable) mode.
  Parameters:
    objref - [in] reference to normal object to show
    bIgnoreLayerMode - [in] if true, the object will be shown even
       if it is on a layer that is locked or off.
  Returns:
    true if the object was successfully shown.
  See Also:
    CRhinoDoc::HideObject
  */
  bool ShowObject(const CRhinoObjRef& objref, bool bIgnoreLayerMode = false); // shows hidden object

  /*
  Description:
    If objref.Object()->IsNormal() is true, then the object will be
    locked.
  Parameters:
    objref - [in] reference to normal object to lock
    bIgnoreLayerMode - [in] if true, the object will be locked even
       if it is on a layer that is locked or off.
  Returns:
    true if the object was successfully hidden.
  See Also:
    CRhinoDoc::UnlockObject
    CRhinoDoc::HideObject
  */
  bool LockObject(const CRhinoObjRef& objref, bool bIgnoreLayerMode = false); // locks object in document

  /*
  Description:
    If objref.Object()->IsLocked() is true, then the object will be
    returned to normal (visible and selectable) mode.
  Parameters:
    objref - [in] reference to locked object to unlock
    bIgnoreLayerMode - [in] if true, the object will be unlocked even
       if it is on a layer that is locked or off.
  Returns:
    true if the object was successfully unlocked.
  See Also:
    CRhinoDoc::HideObject
  */
  bool UnlockObject(const CRhinoObjRef& objref, bool bIgnoreLayerMode = false); // unlocks locked object

  /*
  Description:
    Deletes objref.Object().  The deletion can be undone by calling
    UndeleteObject().
  Parameters:
    objref - [in] objref.Object() will be deleted
    bQuiet = [in] if false, a message box will appear
        when an object cannot be deleted.
    bIgnoreModes - [in] If true, locked and hidden objects
        are transformed.  If false objects that are locked,
        hidden, or on locked or hidden layers are not
        transformed.
  */
  bool DeleteObject(
    const CRhinoObjRef& objref,
    bool bQuiet = true,
    bool bIgnoreModes = false
  );

  // Description:
  //   If object->IsDeleted() is true, then the object is undeleted.
  bool UndeleteObject(const CRhinoObject* object);

  // Description:
  //   Removes object from document and deletes the pointer.
  //
  // Parameters:
  //   object - [in,out] pointer to object to purge.  This object
  //       is deleted and the pointer is set to NULL.
  bool PurgeObject(CRhinoObject*& object);

  /*
  Parameters:
    worksession_ref_model_sn - [in]
      If 0, then all work session reference models are purged.
      If non-zero, then reference objects with a matching
      serial number are purged.
  Returns:
    Number of purged objects
  */
  unsigned int PurgeWorkSessionReferenceModel(
    unsigned int worksession_ref_model_sn
  );

  /*
  Parameters:
    linked_idef_model_sn - [in]
      If 0, then all linked instance definition objects
      are purged.
      If non-zero, then linked instance definition objects
      with a matching serial number are purged.
  Returns:
    Number of purged objects
  */
  unsigned int PurgeLinkedIDefModel(
    unsigned int linked_idef_model_sn
  );

  // Description:
  //   Modifies an object's attributes.  Cannot be used to change
  //   object id.
  //
  // Parameters:
  //   objref - [in] reference to object to modify
  //   new_attributes - [in] new attributes
  //   bQuiet - [in] if true, then warning message boxes are
  //       disabled.
  //
  // Returns:
  //   true if successful.
  //
  bool ModifyObjectAttributes(
    const CRhinoObjRef& objref,
    const CRhinoObjectAttributes& new_attributes,
    BOOL32 bQuiet = false
  );

  /*
  Description:
    Replace one object with another.  Conceptually, this
    function is the same as calling

      Setting new_object attributes = old_object attributes
      DeleteObject(old_object);
      AddObject(old_object);

  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    new_object - [in] new object to be activated - must not be in
                 document.
    bIgnoreModes - [in] If true, locked and hidden objects
        are replaced.  If false objects that are locked,
        hidden, or on locked or hidden layers are not
        replaced.
    bSkipInCommandTest - [in]
      In order for undo, history, event watcher expectations, and other
      important core processes to work correctly, ReplaceObject() must
      be run inside of a command.  If it is not, as is common when
      modeless user interface code calls replace object, a "proxy"
      command is automatically run.  In extremely rare cases, this is
      not desired.  If you are an expert and you are certain that
      you want to run replace object outside of a command, then
      call ReplaceObject with bSkipInCommandTest = true.
  See Also:
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_3dPoint& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Curve& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Surface& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Brep& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Mesh& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, CRhinoObject* );
  Returns:
    true if successful.
  */
  bool ReplaceObject(
    const CRhinoObjRef& objref,
    CRhinoObject* new_object,
    bool bIgnoreModes = false
  );

  bool ReplaceObject(
    const CRhinoObjRef& objref,
    CRhinoObject* new_object,
    bool bIgnoreModes,
    bool bSkipInCommandTest
  );

  /*
  Description:
    Replace one object with new point object.
  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    point - [in] new point to be added.  The point is copied.
  Example:

     ON_NurbsSurface nurbs_surface = ...;
     CRhinoObject* rhino_object = ...; // existing rhino object
     ReplaceObject( CRhinoObjRef( rhino_object ), nurbs_surface );
  See Also:
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, ON_3dPoint );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Curve& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Surface& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Brep& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Mesh& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, CRhinoObject* );
  Returns:
    If successful, a pointer to the new CRhinoPointObject is returned.
  */
  const CRhinoPointObject* ReplaceObject(
    const CRhinoObjRef& objref,
    ON_3dPoint point
  );

  /*
  Description:
    Replace one object with new curve object.
  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    curve - [in] new curve to be added.
      A duplicate of the curve is added to the Rhino model.
      If the curve parameter is on the heap, then the caller
      is responsible for deleting the curve parameter
      after the call.  If this will be inefficient, use
      the version of ReplaceObject that takes a
      CRhinoObject* as the second argument.
  Example:

     ON_NurbsSurface nurbs_surface = ...;
     CRhinoObject* rhino_object = ...; // existing rhino object
     ReplaceObject( CRhinoObjRef( rhino_object ), nurbs_surface );
  See Also:
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, ON_3dPoint );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Curve& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Surface& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Brep& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Mesh& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, CRhinoObject* );
  Returns:
    If successful, a pointer to the new CRhinoCurveObject is returned.
  */
  const CRhinoCurveObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_Curve& curve
  );

  /*
  Description:
    Replace one object with new surface object.
  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    surface - [in] new surface to be added.
      A duplicate of the surface is added to the Rhino model.
      If the surface parameter is on the heap, then the caller
      is responsible for deleting the surface parameter
      after the call.  If this will be inefficient, use
      the version of ReplaceObject that takes a
      CRhinoObject* as the second argument.
  Example:

     ON_NurbsSurface nurbs_surface = ...;
     CRhinoObject* rhino_object = ...; // existing rhino object
     ReplaceObject( CRhinoObjRef( rhino_object ), nurbs_surface );
  See Also:
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_3dPoint& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Curve& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Surface& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Brep& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Mesh& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, CRhinoObject* );
  Returns:
    If successful, a pointer to the new CRhinoSurfaceObject is returned.
  */
  const CRhinoSurfaceObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_Surface& surface
  );

  /*
  Description:
    Replace one object with new brep object.
  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    brep - [in] new brep to be added.
      A duplicate of the brep is added to the Rhino model.
      If the brep parameter is on the heap, then the caller
      is responsible for deleting the brep parameter
      after the call.  If this will be inefficient, use
      the version of ReplaceObject that takes a
      CRhinoObject* as the second argument.
  See Also:
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_3dPoint& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Curve& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Surface& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Brep& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Mesh& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, CRhinoObject* );
  Returns:
    If successful, a pointer to the new CRhinoBrepObject is returned.
  */
  const CRhinoBrepObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_Brep& brep
  );

  /*
  Description:
    Replace one object with new extrusion object.
  Parameters:
    objref - [in] reference to old object to be replaced. The
        object objref.Object() will be deleted.
    brep - [in] new brep to be added.
        A duplicate of the extrusion is added to the Rhino model.
        If the extrusion parameter is on the heap, then the caller
        is responsible for deleting the extrusion parameter
        after the call. If this will be inefficient, use
        the version of ReplaceObject that takes a
        CRhinoObject* as the second argument.
  See Also:
    CRhinoDoc::ReplaceObject(const CRhinoObjRef&, const ON_3dPoint&);
    CRhinoDoc::ReplaceObject(const CRhinoObjRef&, const ON_Curve&);
    CRhinoDoc::ReplaceObject(const CRhinoObjRef&, const ON_Surface&);
    CRhinoDoc::ReplaceObject(const CRhinoObjRef&, const ON_Brep&);
    CRhinoDoc::ReplaceObject(const CRhinoObjRef&, const ON_Mesh&);
    CRhinoDoc::ReplaceObject(const CRhinoObjRef&, CRhinoObject*);
  Returns:
    If successful, a pointer to the new CRhinoExtrusionObject is returned.
  */
  const CRhinoExtrusionObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_Extrusion& extrusion
  );

  /*
  Description:
    Replace one object with new mesh object.
  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    mesh - [in] new mesh to be added.
      A duplicate of the mesh is added to the Rhino model.
      If the mesh parameter is on the heap, then the caller
      is responsible for deleting the mesh parameter
      after the call.  If this will be inefficient, use
      the version of ReplaceObject that takes a
      CRhinoObject* as the second argument.
  See Also:
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_3dPoint& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Curve& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Surface& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Brep& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, const ON_Mesh& );
    CRhinoDoc::ReplaceObject( const CRhinoObjRef&, CRhinoObject* );
  Returns:
    If successful, a pointer to the new CRhinoMeshObject is returned.
  */
  const CRhinoMeshObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_Mesh& mesh
  );

  /*
  Description:
    Replace one object with a new SubD object.
  Parameters:
    objref - [in] reference to old object to be replaced.
    The object objref.Object() will be deleted.
  subd_ref - [in] new SubD to be added.
    The new object will reference the same subd as subd_ref.
  Returns:
    If successful, a pointer to the new CRhinoSubDObject is returned.
  */
  const CRhinoSubDObject* ReplaceObject(
    const CRhinoObjRef& objref,
    ON_SubDRef subd_ref
  );

  /*
  Description:
    Replace one object with a new SubD object.
  Parameters:
    objref - [in] reference to old object to be replaced.
    The object objref.Object() will be deleted.
  subd - [in] new SubD to be added.
    A duplicate of the SubD is added to the Rhino model.
    If the SubD parameter is on the heap, then the caller
    is responsible for deleting the mesh parameter
    after the call.  If this will be inefficient, use
    the version of ReplaceObject that takes an ON_SubDRef
    as the 2nd argument.
  Returns:
    If successful, a pointer to the new CRhinoSubDObject is returned.
  */
  const CRhinoSubDObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_SubD& subd
  );

  /*
  Description:
    Replace the geometry in one object.
  Parameters:
    objref - [in] reference to old object to be replaced.  The
        object objref.Object() will be deleted.
    geometry - [in] new geometry to be added.
      A duplicate of the geometry is added to the Rhino model.
      If the geometry parameter is on the heap, then the caller
      is responsible for deleting the mesh parameter
      after the call. If this will be inefficient, use
      the version of ReplaceObject that takes a
      CRhinoObject* as the second argument.
    bIgnoreModes - [in] If true, locked and hidden objects
        are replaced.  If false objects that are locked,
        hidden, or on locked or hidden layers are not
        replaced.
  Returns:
    If successful, a pointer to the new CRhinoObject is returned.
  */
  const CRhinoObject* ReplaceObject(
    const CRhinoObjRef& objref,
    const ON_Geometry& geometry,
    bool bIgnoreModes
  );

  /*
  Description:
    Creates a new object that is the transformation of the existing
    object and deletes the existing object if bDeleteOriginal is true.

  Parameters:
    objref - [in] reference to object to transform.  The object
        objref.Object() will be deleted if bDeleteOriginal is true.
    xform - [in] transformation to apply
    bDeleteOriginal = [in]
        if true, the original object is deleted
        if false, the original object is not deleted
    bIgnoreModes - [in] If true, locked and hidden objects
        are transformed.  If false objects that are locked,
        hidden, or on locked or hidden layers are not
        transformed.
    bAddTransformHistory - [in]
        If true and history recording is turned on, then
        transformation history is recorded.  This will be
        adequate for simple transformation commands like
        rotate, move, scale, and so on that do not have
        auxiliary input objects.  For fancier commands,
        that have an auxiliary object, like the spine
        curve in ArrayAlongCrv, set bAddTransformHistory
        to false.
  Returns:
    New object that is the transformation of the existing_object.
    The new object has identical attributes.

  Remarks:
    If the object is locked or on a locked layer, then it cannot
    be transformed.
  */
  CRhinoObject* TransformObject(
    CRhinoObjRef& objref,
    const ON_Xform& xform,
    bool bDeleteOriginal = true,
    bool bIgnoreModes = false,
    bool bAddTransformHistory = false
  );

  /*
  Description:
    Creates a new object that is the transformation of the
    existing object and deletes the existing object if
    bDeleteOriginal is true.

  Parameters:
    old_object - [in] reference to object to morph.  The object
        objref.Object() will be deleted if bDeleteOriginal is true.
    xform - [in] transformation to apply
    bAddNewObjectToDoc - [in]
        if true, the new object is added to the document.
        if false, the new object is not added to the document.
    bDeleteOriginal - [in]
        if true, the original object is deleted
        if false, the original object is not deleted
    bAddTransformHistory - [in]
        If true and history recording is turned on, then
        transformation history is recorded.  This will be
        adequate for simple transformation commands like
        rotate, move, scale, and so on that do not have
        auxiliary input objects.  For fancier commands,
        that have an auxiliary object, like the spine
        curve in ArrayAlongCrv, set bAddTransformHistory
        to false.
  Returns:
    New object that is the morph of the existing_object.
    The new object has identical attributes.

  Remarks:
    If the object is locked or on a locked layer, then it cannot
    be transformed.
  */
  CRhinoObject* TransformObject(
    const CRhinoObject* old_object,
    const ON_Xform& xform,
    bool bAddNewObjectToDoc,
    bool bDeleteOriginal,
    bool bAddTransformHistory
  );

  /*
  Description:
    Creates a new object that is the transformation of the
    existing object and deletes the existing object if
    bDeleteOriginal is true.

  Parameters:
    old_object - [in] reference to object to morph.  The object
        objref.Object() will be deleted if bDeleteOriginal is true.
    xform - [in] transformation to apply
    bAddNewObjectToDoc - [in]
        if true, the new object is added to the document.
        if false, the new object is not added to the document.
    bDeleteOriginal - [in]
        if true, the original object is deleted
        if false, the original object is not deleted
    bAddTransformHistory - [in]
        If true and history recording is turned on, then
        transformation history is recorded.  This will be
        adequate for simple transformation commands like
        rotate, move, scale, and so on that do not have
        auxiliary input objects.  For fancier commands,
        that have an auxiliary object, like the spine
        curve in ArrayAlongCrv, set bAddTransformHistory
        to false.
    bTransformReferenceObjects - [in]
        If true reference objects are allowed to be transformed.
    pGroupMap - [in/out]
        Optional group map if copy transforming multiple grouped objects,
        and the new objects should be grouped with other copies
        and not the originals.

  Returns:
    New object that is the morph of the existing_object.
    The new object has identical attributes.

  Remarks:
    If the object is locked or on a locked layer, then it cannot
    be transformed.
  */
  CRhinoObject* TransformObject(
    const CRhinoObject* old_object,
    const ON_Xform& xform,
    bool bAddNewObjectToDoc,
    bool bDeleteOriginal,
    bool bAddTransformHistory,
    bool bTransformReferenceObjects,
    ON_2dexMap* pGroupMap = 0
  );


  /*
  Description:
    Transforms every object in a list.

  Parameters:
    it - [in] iterates through list of objects to transform
    xform - [in] transformation to apply
    bDeleteOriginal = [in]
         if true, the original objects are deleted
         if false, the original objects are not deleted
    bIgnoreModes - [in]  (Parameter is unused and does nothing)
        If true, locked and hidden objects
        are transformed.  If false objects that are locked,
        hidden, or on locked or hidden layers are not
        transformed.
    bAddTransformHistory - [in]
        If true and history recording is turned on, then
        transformation history is recorded.  This will be
        adequate for simple transformation commands like
        rotate, move, scale, and so on that do not have
        auxiliary input objects.  For fancier commands,
        that have an auxiliary object, like the spine
        curve in ArrayAlongCrv, set bAddTransformHistory
        to false.
  Returns:
    Number of objects that were transformed.

  Remarks:
    This is similar to calling TransformObject() for each object
    int the list except that this function will modify locked and
    hidden objects.  It is used for things like scaling the entire
    model when a unit system is changed.
  */

  // As of Sept 14, 2017, This is not used by rhino
  int TransformObjects(
    CRhinoObjectIterator& it,
    const ON_Xform& xform,
    bool bDeleteOriginal = true,
    bool bIgnoreModes = false,
    bool bAddTransformHistory = false
  );

  // As of Sept 14, 2017, This is not used by rhino
  int TransformObjects(
    CRhinoObjectIterator& it,
    const ON_Xform& xform,
    bool bDeleteOriginal,
    bool bIgnoreModes,  // (unused)
    bool bAddTransformHistory,
    bool bTransformReferenceObjects
  );

  /*
  Description:
  Creates a new object that is a scaled version of the input
  object and deletes the existing object if bDeleteOriginal is true.

  Scale all objects in the iterator around world origin

  This is called for unit changes and in addition to scaling geometry, it may change
  some attributes.
  As of Sept 14, 2017, it will scale hatch patterns of hatch objects
  */
  int ScaleObjects(
    CRhinoObjectIterator& it,
    double scale,
    bool bDeleteOriginal,
    bool bAddTransformHistory,
    bool bTransformReferenceObjects
  );

  /*
  Description:
    Creates a new object that is the morph of the existing
    object and deletes the existing object if bDeleteOriginal is true.

  Parameters:
    old_object - [in] reference to object to morph.  The object
        objref.Object() will be deleted if bDeleteOriginal is true.
    morph - [in] space morph to apply
    bAddNewObjectToDoc - [in]
        if true, the new object is added to the document.
        if false, the new object is not added to the document.
    bDeleteOriginal - [in]
        if true, the original object is deleted
        if false, the original object is not deleted

  Returns:
    New object that is the morph of the existing_object.
    The new object has identical attributes.

  Remarks:
    If the object is locked or on a locked layer, then it cannot
    be transformed.
  */
  CRhinoObject* MorphObject(
    const CRhinoObject* old_object,
    const ON_SpaceMorph& morph,
    bool bAddNewObjectToDoc,
    bool bDeleteOriginal
  );

  //////////
  // PickObjects() returns number of objects added to array
  int PickObjects(
    const CRhinoPickContext&,
    CRhinoObjRefArray&     // picked objects added to this list
  ) const;

  //////////
  // PickObjects() returns number of objects added to array
  int PickObjects(
    const CRhinoPickContext&,
    CRhinoObjRefArray&,     // picked objects added to this list
    bool bSilent            // if true selection filter is applied silently, without flashing the selection filter toolbar
  ) const;

  // Description:
  //   Object snapping tool.
  // Parameters:
  //   snap_context - [in]
  //   snap_event - [out]  If SnapTo returns true, then the
  //       type and location of the snap is returned in snap_event.
  //   construction_points - [in] optional array of points that 
  //       will be snapped to independent of current point osnap
  //       settings.
  //   snap_points - [in] optional array of points that will be
  //       snapped to if point osnap is enabled.
  //   int_snap_points - [in] optional array of points that will be
  //       snapped to if int osnap is enabled. Used for snapping
  //       to isocurve intersections when input is constrained to 
  //       a brep or brep face.
  // Returns:
  //   true if a snap occurred.
  BOOL32 SnapTo(
    const CRhinoSnapContext& snap_context,
    CRhinoSnapEvent& snap_event,
    const ON_SimpleArray<ON_3dPoint>* construction_points = 0,
    const ON_SimpleArray<ON_3dPoint>* snap_points = 0,
    const ON_SimpleArray<ON_3dPoint>* int_snap_points = 0
  ) const;

  /*
  Description:
    Unselect objects.
  Parameters:
    bIgnorePersistentSelections - [in] if true, then
    objects that are persistently selected will not be
    unselected.
  Returns:
    Number of object that were unselected.
  See Also:
    CRhinoObject::Select
    CRhinoObject::IsSelected
  */
  int UnselectAll(
    BOOL32 bIgnorePersistentSelections = false
  );

  //////////
  // Set all object marks to mark_value.  See CRhinoObject::Mark() and
  // CRhinoObject::IsMarked().  The mark values are for general runtime
  // use and do not persist through save or copy operations.
  void ClearMarks(
    CRhinoObjectIterator&,
    int = 0 // mark_value
  );

  /*
  Description:
    Expert user tool to set flags used by CRhinoDoc::Redraw().
  Parameters:
    display_hint - [in] one of
           CRhinoView::regenerate_display_hint
           CRhinoView::mark_display_hint
           CRhinoView::refresh_display_hint
    dm - [in] If the display hint only applies to a certain
              display mode, set dm to that mode.  Otherwise
              the display hint will be applied to all views.
  */
  void SetRedrawDisplayHint(
    unsigned int display_hint,
    bool bShadedOnly = false
  ) const;

  //////////
  // Returns non-null if currently in a CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), or CRhinoGetString::GetString()
  CRhinoGet* InGet() const;

  //////////
  // Returns non-null if currently in a CRhinoGetPoint::GetPoint();
  CRhinoGetPoint* InGetPoint() const;

  //////////
  // Returns non-null if currently in a CRhinoGetPoint::GetObject();
  CRhinoGetObject* InGetObject() const;

  //////////
  // Returns non-null if currently in a CRhinoGetString::GetString();
  CRhinoGetString* InGetString() const;

  //////////
  // Returns non-null if currently in a CRhinoGetString::GetString();
  CRhinoGetNumber* InGetNumber() const;

  //////////
  // Returns non-null if currently in a CRhinoGetString::GetString();
  CRhinoGetOption* InGetOption() const;

  //////////
  // Returns non-null if currently in a CRhinoGetString::GetString();
  CRhinoGetColor* InGetColor() const;

  //////////
  // Returns non-null if currently in a CRhinoGetString::GetString();
  CRhinoGetMeshes* InGetMeshes() const;

  //////////
  // For use by CRhinoGetPoint::GetPoint(),
  // CRhinoGetPoint::GetObject(), CRhinoGetString()
  CRhinoGet* EnableGet(CRhinoGet*);


  void AddGrip(CRhinoGripObject& grip);
  void RemoveGrip(CRhinoGripObject& grip);

  /*
  Description:
    Determine if a camera icon is begin shown.
  Parameters:
    view - [in] if NULL, then all views are tested.
                if not NULL, then just view is tested.
  Returns:
    If a camera icon is on, a pointer to the view whose camera
    is begin shown is returned.  If no camera icon is on,
    NULL is returned.
  */
  class CRhCameraIcon* CameraIcon(CRhinoView* view = 0) const;

  /*
  Description:
    Use to turn camera icon on and off.
  Parameters:
    view - [in] If NULL, any camera icon is turned off.
                If not NULL, the camera icon for that
                view is turned on.
  */
  void EnableCameraIcon(CRhinoView* view);
  /*
  Description:
    Close specified Rhino view.
  Parameters:
    pView [i] View to close
  Returns:
    Returns true on success; otherwise false
  */
  bool CloseRhinoView(CRhinoView* pView);

  // Description:
  //   Creates new window handles for every CRhinoView.
  static void RecreateRhinoViewHWnds();

  /*
  Description:
    Call this method to get the name of the template file used to create this document.  This
    is a runtime value only present if the document was newly created.
  Returns:
    Returns full path to template used to create this document on File/New
  */
  const wchar_t* TemplateFileUsed() const;

public:
  void ClearUndoRecords(bool = false);
  void ClearUndoRecords(unsigned int undo_record_sn, bool bPurgeDeletedObjects);
  void ClearRedoRecords();
  void EnableUndoRecording(bool = true);
  bool UndoRecordingIsEnabled() const; // true if permitted
  bool UndoRecordingIsActive() const;  // true if actually happening now

  /*
    OBSOLETE
    Call CRhinoDoc::BeginUndoRecordEx(...)
  */
  bool BeginUndoRecord(const CRhinoCommand*);

  /*
  Description:
    Used to begin recording undo information when a command starts.
  Example:

          unsigned int undo_record_sn = BeginUndoRecordEx(...);
          ...
          doc->EndUndoRecord(undo_record_sn);

  Returns:
    Serial number of record.  Returns 0 if record is not started
    because undo information is already being recorded or
    undo is disabled.
  */
  unsigned int BeginUndoRecordEx(const CRhinoCommand*);

  /*
    OBSOLETE
    Call CRhinoDoc::BeginUndoRecordEx(...)
  */
  bool BeginUndoRecord(const wchar_t* sActionDescription);

  /*
  Description:
    Used to begin recording undo information when the document
    is changed outside of a command.  An example begin changes
    caused by the modeless layer or object properties dialogs
    when commands are not running.
  Example:

          unsigned int undo_record_sn = BeginUndoRecordEx(...);
          ...
          doc->EndUndoRecord(undo_record_sn);

  Returns:
    Serial number of record.  Returns 0 if record is not started
    because undo information is already being recorded or
    undo is disabled.
  */
  unsigned int BeginUndoRecordEx(const wchar_t* sActionDescription);

  /*
  Description:
    If you want to your plug-in to do something when the Rhino
    Undo/Redo command runs, the call AddCustomUndoEvent during
    your command.

    This function is for expert plug-in developers.  If you
    don't do a good job here, you will really break Rhino.
  Parameters:
    undo_event_handler - [in]
      Pointer to a class allocated with a call to new.
      Never delete this class.
      Never pass a pointer to a stack variable.
  Returns:
    If a non zero number is returned, then this is the runtime
    serial number Rhino has assigned to this undo event.
    If zero is returned, then the user has disabled undo
    and undo_event_handler was deleted.
  */
  unsigned int AddCustomUndoEvent(
    CRhinoUndoEventHandler* undo_event_handler
  );

  // CRhinoDoc::EndUndoRecord() is OBSOLETE 
  //  Call EndUndoRecord(undo_record_sn)
  //  where undo_record_sn is the value returned
  //  by BeginUndoRecordEx(...)
  bool EndUndoRecord(); // <- OBSOLETE

  /*
  Description:
    End current undo if its serial number satisfies
    undo_record_sn0 <= sn <= undo_record_sn1.
  */
  bool EndUndoRecord(unsigned int undo_record_sn);

  // End undo if its serial number satisfies
  // undo_record_sn0 <= sn <= undo_record_sn1.
  bool EndUndoRecord(unsigned int undo_record_sn0, unsigned int undo_record_sn1);

  bool Undo(class CRhUndoRecord* = NULL);
  bool Redo();

  /*
  Returns:
    True if Undo() is currently active.
  */
  bool UndoActive() const;

  /*
  Returns:
    True if Redo() is currently active.
  */
  bool RedoActive() const;

  /*
  Returns:
    Record being currently recorded.  If undo recording is disabled
    or nothing is currently being recorded, then NULL is returned.
  */
  CRhUndoRecord* CurrentUndoRecord() const;

  /*
  Returns:
    >0: undo recording is active and being saved on
        the undo record with the specified serial number.
     0: undo recording is not active. (Disabled or nothing is
        being recorded.)
  */
  unsigned int CurrentUndoRecordSerialNumber() const;

  /*
  Returns:
    The serial number that will be assigned to the next
    undo record that is constructed.
  */
  unsigned int NextUndoRecordSerialNumber() const;

  /*
  Returns:
    Number of undo records.
  */
  int GetUndoRecords(ON_SimpleArray<class CRhUndoRecord* >&) const;

  /*
  Returns:
    Number of undo records.
  */
  int UndoRecordCount() const;

  /*
  Returns:
    Number bytes in used by undo records
  */
  size_t UndoRecordMemorySize() const;

  /*
  Description:
    Culls the undo list to release memory.
  Parameters:
    min_step_count - [in]
      minimum number of undo steps to keep.
    max_memory_size_bytes - [in]
      maximum amount of memory, in bytes, for undo list to use.
  Returns:
    Number of culled records.
  Remarks:
    In the version with no arguments, the settings in
    RhinoApp().AppSettings().GeneralSettings() are used.
  */
  int CullUndoRecords();

  int CullUndoRecords(
    int min_step_count,
    size_t max_memory_size_bytes
  );


  /*
  Returns true if document contains undo records.
  */
  bool HasUndoRecords() const;

  /*
  Returns:
    Number of undo records.
  */
  int GetRedoRecords(ON_SimpleArray<class CRhUndoRecord* >&) const;

  // MRU Commands for this document
  const wchar_t* MruMenuString() const;
  void SetMruMenuString(LPCTSTR str);

  bool RunMenuScript(const wchar_t* s);

  /*
  Description:
    Run a Rhino command script.
  Parameters:
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
  bool RunScript(const wchar_t* script,
    int echo_mode = 1
  );

  /*
  Description:
    Run a Rhino command script.
  Parameters:
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
  bool RunScriptEx(const wchar_t* script,
    const wchar_t* mruDisplayString,
    int echo_mode = 1
  );

  bool RunScript(
    const class CRhinoScriptContext& script
  );

  // Description:
  //   Add MRU command to list, pop old commands off when list
  //   reaches max size.
  // Parameters:
  //   cmd  - [in] command macro to add
  //   display_string - [in] String displayed in mru command pop-up menu, if NUL then
  //                         displays macro.
  void AddMruCommand(const wchar_t* cmd, const wchar_t* display_string = NULL);
  // Description:
  //   Add MRU command to list, pop old commands off when list
  //   reaches max size.
  // Parameters:
  //   cmd  - [in] command macro to add
  //   display_string - [in] String displayed in mru command pop-up menu, if NUL then
  //                         displays macro.
  //   shortcut - [in] String displayed as the shortcut
  //   modifier_flags - [in] modifiers for the shortcut (Mac only)
  void AddMruCommand(const wchar_t* cmd, const wchar_t* display_string, const wchar_t* shortcut, const UINT modifier_flags);
  // Description:
  //   Called by CMruCommandMenu::MruCmd() to run a m_mru_command_list command
  //   macro.  Will move the macro from its current list position to top of
  //   display list.
  // Parameters:
  //   cmd_index  - [in] command macro to run
  void RunMruCommand(int cmd_index);
  // Description:
  //   Called by CMruCommandMenu::Favorite() to run a CRhinoAppGeneralSettings.m_popup_favorites
  //   macro.  Will make sure command does not show up on the MRU list
  // Parameters:
  //   cmd  - [in] m_mru_command_list index of command to run
  void RunMruFavoriteCommand(int cmd_index);

  const ON_ClassArray<CRhinoApp::CRhMruCommandRecord>& MruCommandList() const;
  const CRhinoApp::CRhMruCommandRecord* CommandToRepeat() const;
  int GetMRUCommands(ON_ClassArray<ON_wString>& display_strings, ON_ClassArray<ON_wString>& macros) const;

  const ON_SimpleArray<const CRhinoCommand*>& RunningCommandsStack();
  /*
  Returns:
    Pointer to currently running command.
  */
  const CRhinoCommand* ActiveCommand() const;

  /*
  Returns:
    Style of currently running command: interactive_ui, script_ui, or batch_ui
  */
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
  const CRhinoCommand* RunningCommand(
    int i
  ) const;

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
  underbar (_), hyphen (-), or right single quote (').  If an
  underbar is present, then the command name search looks for
  a matching english command name.  If the underbar is not present,
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
  CRhinoCommand::result ExecuteCommand(const wchar_t* cmd_name);

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
  int InCommand(bool bIgnoreScriptRunnerCommands = false) const;

  /*
  Description:
    This is a low level tool to determine if Rhino is currently running
    a script running command like "ReadCommandFile" or the RhinoScript
    plug-in's "RunScript".
  Returns:
    True if a script running command is active.
  */
  bool InScriptRunnerCommand() const;

  void EnterCommand(const CRhinoCommand* cmd, unsigned int* undo_record_sn);
  void ExitCommand(unsigned int rhino_doc_sn0,
    unsigned int rhino_doc_sn1,
    const CRhinoCommand* cmd,
    unsigned int undo_record_sn);

  void SetCommandToRepeat(const CRhinoApp::CRhMruCommandRecord& rec);


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
    CRhinoDoc::StatusBarProgressMeterPos
    CRhinoDoc::StatusBarProgressMeterEnd
  */
  int StatusBarProgressMeterStart(int nLower, int nUpper, const wchar_t* pszLabel = nullptr, bool bEmbedLabel = true, bool bShowPercent = true);

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
    CRhinoDoc::StatusBarProgressMeterStart
    CRhinoDoc::StatusBarProgressMeterEnd
  */
  int StatusBarProgressMeterPos(int nPos, bool bAbsolute = true);

  /*
  Description:
    Sets the current position of Rhino's status bar progress meter.
  Parameters:
    pszLabel  - [in] The short description of the progress (e.g. "Calculating", "Meshing", etc).
    nPos      - [in] The position. If ON_UNSET_INT_INDEX, then only the label will be updated.
    bAbsolute - [in] If true, then the progress meter is moved to nPos (absolute).
                     If false, then the progress meter is moved nPos from the current position (relative).
  Returns:
     The previous position if successful.
  See Also:
    CRhinoDoc::StatusBarProgressMeterStart
    CRhinoDoc::StatusBarProgressMeterEnd
  */
  int StatusBarProgressMeterPos(const wchar_t* pszLabel, int nPos, bool bAbsolute);

  /*
  Description:
    Ends, or hides, Rhino's status bar progress meter.
  See Also:
    CRhinoDoc::StatusBarProgressMeterStart
    CRhinoDoc::StatusBarProgressMeterPos
  */
  void StatusBarProgressMeterEnd();


protected:

  /////////////////////////////////////////////////////////////////////////////
  // If the command name starts with an underscore "_", then the English names
  // are searched, otherwise the aliases and localized names are searched.
  //  CRhCommandManager* m_command_manager;

public:
  class CRhSelSetManager* m_selset_manager;

  void ChangeTitleToUnNamed();

  static void GetUntitledDocName(
    ON_wString& sUntitledDocName
  );

  /*
  Universal construction plane stack operators
  */
  void PushConstructionPlane(const ON_Plane& plane);
  bool ActiveConstructionPlane(ON_Plane& plane);
  bool NextConstructionPlane(ON_Plane& plane);
  bool PrevConstructionPlane(ON_Plane& plane);
  int ConstructionPlaneCount() const;

#if defined (ON_RUNTIME_APPLE)
  bool IsInitialized();

  // LockModel / UnlockModel protect model when making changes.
  // LockModel and UnlockModel may be called recursively but must be called an equal number of times.
  bool LockModel() const;
  bool UnlockModel() const;

  // show message on document windows
  void Print(const wchar_t* fmt, ...) const;
  void Printv(const wchar_t* fmt, va_list arg) const;
#endif

public:
  class CRhDocSdkExtension* m__doc_sdk_extension;

private:
  friend class CRhinoInstanceDefinitionTable;
  friend class CRhinoRead3dmHelper;
  friend class CRhinoObjectIterator;
  friend class CRhinoLayerTable;
  friend class CRhinoFileMenu;
  friend int RhinoMemoryErrorHandler(int);
  friend class CRhinoApp;
  friend void RhSetTemplateFileUsed(CRhinoDoc&, const wchar_t*);

  void SetTemplateFileUsed(const wchar_t* lpsTemplateFile);

  CRhinoDocProperties   m_doc_properties;
  ON_wString m_wTemplateFileUsed; // Runtime place holder for template file used when creating this document.

#if defined (ON_RUNTIME_APPLE)
public:
  friend class CRhOglEngineExtension;
  class CRhGLSceneManager* GetSceneManager() const;
#endif
private:
  // 3d bounding box of visible objects. Beware, not always used for clipping
  // as you would expect since clipping is now done per-view in the DisplayPipeline.
  mutable ON_BoundingBox  m_model_bbox;
  mutable ON_BoundingBox  m_model_bbox_with_crms;

  mutable ON_BoundingBox  m_model_tight_bbox;
  mutable ON_BoundingBox  m_model_tight_bbox_with_crms;

  void DestroyBoundingBoxes(void) const;
  ON_BoundingBox& BoundingBoxCache(bool bTight, bool bIncludeCRMs, bool bIncludeCurves) const;

private:
  // Used to quickly find objects from an id
  // or runtime serial number.  A pointer to
  // a CRhinoObject is in one of these lists:
  //    CRhinoDoc::m_grips_list,
  //    CRhinoDoc::m_active_model_geometry,
  //    CRhinoDoc::m_active_model_idef_geometry,
  //    CRhinoDoc::m_deleted_geometry,
  //    CRhinoDoc::m_reference_model_geometry,
  //    CRhinoDoc::m_reference_model_idef_geometry,
  //    CRhinoDoc::m_light_table[], then that object
  //  if and only if the object can be found by calling 
  //  m_object_finder->LookupSerialNumber().
  //  An object is in any list, except CRhinoDoc::m_deleted_geometry,
  //  if and only if the object can be found by calling
  //  m_object_finder->LookupId(). 
  class CRhObjectFinder* m_object_finder;

public:
  // This function is a debugging tool that validates the
  // CRhinoObject lists and m_object_finder.  If any errors
  // are detected, false is returned.  If no errors is detected,
  // true is returned. Pass in a text_log if you want a 
  // description of the errors.  Set max_error_count to the
  // number of errors you want to allow before checking
  // is halted.
  bool ObjectFinderIsValid(
    ON_TextLog* text_log,
    unsigned int max_error_count,
    size_t* sn_count,
    size_t* id_count
  ) const;

private:
  CRhinoObject* m_active_model_geometry;   // geometry from active model (can be on locked or hidden layers)
  CRhinoObject* m_active_model_idef_geometry; // instance definitions geometry
  CRhinoObject* m_deleted_geometry; // always belongs to active model

  // worksession reference model(s) geometry - never saved
  CRhinoObject* m_reference_model_geometry;   // worksession reference model(s) geometry
  CRhinoObject* m_reference_model_idef_geometry; // worksession reference model(s) instance definition geometry

  CRhinoDocTableReference m_model_geometry_tableref;

  // view camera icon
  class CRhCameraIcon* m_camera_icon;

  // list used to define the CRhinoPageView page order (page1, page2, page3, ...)
  ON_SimpleArray<CRhinoPageView*> m_pageview_page_order;

  class CRhDetailLayerEventWatcher* m_detailLayerEventWatcher;

private:
  friend class CRhPushPopDocStatus;
  // status bits: nonzero = on/yes, zero = off/no
  // (m_invalid_object_status_bits & 0x01): reading 3dm file
  // (m_invalid_object_status_bits & 0x02): plug-in is reading a non-3dm file
  // (m_invalid_object_status_bits & 0x80): AddObject validates new geometry
  unsigned char m_invalid_object_status_bits;

  // When CRhinoDoc::AddObject is validating new geometry, 
  // m_invalid_object_count stores the number of invalid objects 
  // added in the current command. The value of m_invalid_object_count
  // is set to zero when a command starts and is checked when command ends.
  unsigned int m_invalid_object_count;

private:
  /* keep this private */ void IncrementInvalidObjectCount();
  /* keep this private */ void ClearInvalidObjectCount();

public:

  /*
  Returns:
    True if CRhinoDoc::AddObject() is validating each object as it is
    added.  This can be time consuming but is valuable for debugging.
  */
  bool ValidateNewObjects() const;

  /*
  Parameters:
    bEnable - [in]
      true:
        CRhinoDoc::AddObject() is validates each object as it is
        added. If the object is not valid, the count of This can be time consuming but is valuable for debugging.
      false:
        CRhinoDoc::AddObject() dos not validate objects.
  Returns:
    True if CRhinoDoc::AddObject() is validating each object as it is
    added.  This can be time consuming but is valuable for debugging.
  */
  void EnableValidateNewObjects(bool bEnable);

  /*
  Returns:
    If object validation is enabled ( CRhinoDoc::ValidateNewObjects() is true),
    then this function returns the number of invalid objects added in the
    current command.  If the value is >= 255, then at least 255 "bad"
    objects have been added.  The count is set to zero when a new command
    begins.
  */
  unsigned int InvalidObjectCount() const;

  /*
  Returns:
    True if Rhino is actively reading a .3dm file.
  */
  bool RhinoFileReadInProgress() const;

  /*
  Returns:
    True if a file import plug-in is actively reading a file.
  */
  bool ImportPlugInFileReadInProgress() const;


  /*
  Description:
    Searches for and destroys all CRhinoInstanceObject instances in the
    instance definition geometry table that are at the tail of a cyclic
    instance reference. Cyclic instance references crash display code.
  Returns:
    Number of purged objects. Any value > 0 indicates something managed
    to create a cyclic instance reference. If the creation point can be
    found, it should be fixed so it does not create cyclic references.
  */
  unsigned int PurgeCyclicInstanceReferences();

  int PurgeAnonymousUserTable(bool bQuiet);

private:

  // Construction plane stack
  ON_SimpleArray<ON_Plane> m_plane_stack;
  int m_plane_stack_index;

#if defined (ON_RUNTIME_APPLE) && !defined (ON_RUNTIME_APPLE_IOS)
  class CRhGLSceneManager* m_pSceneManager = nullptr;
#endif

  // Helper functions used by CRhinoDoc::DeleteObject,
  // CRhinoDoc::AddObject and CRhinoDoc::ReplaceObject().
  bool IsDeletableHelper(
    const CRhinoObjRef& objref,
    const CRhinoObject* obj,
    const bool bQuiet,
    const bool bIgnoreModes,
    const bool bDeleteRefObject
  ) const;
  bool IsAddableHelper(
    const CRhinoObject* obj
  ) const;

  int AuditHelper(
    CRhinoObject* obj,
    ON::object_mode correct_mode,
    bool bIsReference,
    bool bIsDeleted,
    const CRhinoObject* list_head,
    CRhinoObject* prev_obj,
    ON_TextLog* dump,
    bool bAttemptRepair
  );

  int AuditIDefTable(
    ON_TextLog* dump,
    bool bAttemptRepair
  );

  int AuditLinetypeTable(
    ON_TextLog* dump,
    bool bAttemptRepair
  );

  int AuditLayerTable(
    ON_TextLog* dump,
    bool bAttemptRepair
  );

  int AuditDimstyleTable(
    ON_TextLog* dump,
    bool bAttemptRepair
  );

  // see source code for details
  bool Write3DMHelper(
    ON_BinaryArchive&,
    int, int,
    const ON_3dmObjectAttributes&,
    const CRhinoObject*,
    const ON_Xform*,
    bool bGeometryOnly
  );

  void DeleteObjectList(CRhinoObject*);

  // linked list of runtime editing grips
  // The CRhinoObject that owns the grips, manages them;
  CRhinoGripObject* m_grip_list;

  // runtime settings

  // set if currently in CRhinoGetPoint::GetPoint(), CRhinoGetPoint::GetObject(),
  // or CRhinoGetString::GetString()
  CRhinoGet* m_in_get;

  // user table "goo"
  class CRhAnonymousUserTable* m_pAnonymousUserTable;

  // Display hint used in CRhinoDoc::Redraw(). Set to one of
  // CRhinoView::regenerate_display_hint
  unsigned int m_redraw_display_hint;

public:
  bool SaveModified(bool bUseBatchMode);
    
  class CRhAnonymousUserTable* UserDataTable(void);

public:
  // Overrides
    /*virtual void SetTitle(LPCTSTR lpszTitle) override;
    virtual void SetPathName(LPCTSTR lpszPathName, BOOL32 bAddToMRU = TRUE) override;
    virtual BOOL32 IsModified() override;
    virtual void SetModifiedFlag(BOOL32 bModified = TRUE) override;
    virtual POSITION GetFirstViewPosition() const override;
    virtual CView* GetNextView(POSITION& rPosition) const override;
    virtual void OnChangedViewList() override; // after Add or Remove view
    virtual void DeleteContents() override; // delete doc items etc
    virtual BOOL32 OnOpenDocument(LPCTSTR lpszPathName) override;
    virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL32 bSaving, UINT nIDPDefault) override;
    virtual BOOL32 SaveModified() override; // return TRUE if OK to continue
    virtual HMENU GetDefaultMenu() override; // get menu depending on state
    virtual BOOL32 OnNewDocument() override;
    virtual void Serialize(CArchive& ar) override;
    virtual void OnCloseDocument() override;*/

private:
  // Use CRhinoDoc::WriteFile() to write files.
  /*OBSOLETE*/ //virtual BOOL32 OnSaveDocument(LPCTSTR lpszPathName) override;
  /*OBSOLETE*/ //virtual BOOL32 DoSave(LPCTSTR lpszPathName, BOOL32 bReplace = true) override;
  /*OBSOLETE*/ //BOOL32 DoFileSave() override;

public:
#ifdef SHARED_HANDLERS
  //DALELEARV6// VS 2012 added the SHARED_HANDLERS section
  //virtual void InitializeSearchContent();
  //virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

#if defined(ON_RUNTIME_WIN)
#ifdef RHINO_SDK_DEBUG
  //virtual void AssertValid() const override;
  //virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

private:
  //Can only be deleted by CRhDoc
  virtual ~CRhinoDoc();

public:
  /*
  Description:
    The MFC framework does not provide a good way for options
    to be passed into the CDocument virtual functions like
    Serialize, SaveAs, DoSave, ...
  */
  class CRhDocSerializeStatus* SerializeStatus() const;
  void PushSerializeStatus(class CRhDocSerializeStatus*);
  class CRhDocSerializeStatus* PopSerializeStatus();

private:
  class CRhDocSerializeStatus* m_serialize_status = nullptr;


private:
  // If a file is being actively read by CRhinoDoc::Read3dmFile()
  // or a file reading plug-in, then m_active_fro is set to the options
  // being used.
  // The value of m_active_fro is ALWAYS modified by calling CRhinoDoc::SetReadFileOptions().
  // The value of m_active_fro is ALWAYS retrieved by calling CRhinoDoc::ReadFileOptions().
  // It is critical that you never directly access or modify the value of m_active_fro.
  const class CRhFileReadOptions* m_active_fro = nullptr;

public:
  /*
  Returns:
    If a file is being read, this returns a pointer to a CRhFileReadOptions
    class that contains information about that file.
    If a file is not being read, this returns nullptr.
  Remarks:
    Plug-ins cannot use this function because they cannot
    create CRhFileReadOptions classes. This is intentional.
  */
  const class CRhFileReadOptions* ReadFileOptions() const;

private:
  void SetReadFileOptions(
    const class CRhFileReadOptions* active_fro
  );

private:
  // If a file is being actively written by CRhinoDoc::Write3dmFile()
  // or a file writing plug-in, then m_active_fwo is set to the options
  // being used.
  const class CRhinoFileWriteOptions* m_active_fwo = nullptr;

  // Settings used for the most recent successful save as 
  // a complete .3dm file that set the document title.
  CRhinoFileWriteOptions m_previous_complete_3dm_save_fwo;

  /////////////////////////////////////////////////////////////////////////////
  // list of events (text tokens, view clicks, digitizer points, etc.
  // - exposed for expert users - do not modify
public:
  class CRhEventManager* EventManager(void) const;

#if defined (ON_RUNTIME_APPLE) || defined(ON_RUNTIME_LINUX)
public:
  // Per document DocViewController functions
  class CRhViewBase* CreateRhinoView(const wchar_t* lpsViewTitle, const UUID& plug_in_id);
  class CRhViewBase* CreateLinkedFloatingRhinoView(const wchar_t* lpsViewTitle, const UUID& plug_in_id);
#endif
  
#if defined (ON_RUNTIME_APPLE)
  // On macOS only
public:

  //////////
  // Call SetPointPane() to change the point displayed
  // in the status bar point pane.  Depending on the
  // current application settings, the status bar point
  // pane displays the point in either world or cplane
  // coordinates.  The CRhinoView is used to convert
  // the point to cplane coordinates.  If cplane coordinates
  // are being displayed and no view is provided, then the
  // point pane is blanked.
  void SetPointPane(
    const ON_3dPoint& wcspoint, // point in world coordinates
    const ON_Plane* plane
  );


  bool AutosaveToFile(LPCTSTR lpszPathName);
  bool SaveShouldBeCancelled();
  BOOL32 IsLoaded() const;
  void SetLoaded(BOOL32);

  void SetPreviewImage(ON_3dmProperties& prop);
  void CreatePreviewImage();

  void ClearUndoRecordsPrivate(bool bPurgeDeletedObjects);
  bool UndoPrivate(CRhUndoRecord* pUndoRecord);
  bool RedoPrivate();

  // return current document cursor
  HCURSOR CurrentCursor(void);
  // Set new cursor for this document
  HCURSOR SetCursor(HCURSOR newCursor) const;

  // Set cursor tool tip
  void SetCursorToolTip(LPCTSTR w);
  void SetCursorToolTip(const ON_wString& w);
  void ClearCursorToolTip();

  CWorkerThreadInfo* workerThreadInfo;      // our one and only worker thread
  unsigned int workerCount;                 // number of existing workers

  CRhObjectDragHelper& ObjectDragHelper() const;
  CRhGetPointHelper& GetPointHelper() const;

  CRhStatusBarProgressMeter* GetProgressMeter() const;

  // returns > 0 if we have file locked
  // returns < 0 if someone else has file locked
  // returns 0 if file is not locked
  int FileLockedStatus();

  class CRhinoHistoryManager* m_history_manager = nullptr;
protected:
  CRhEventManager* m_doc_event_manager;

  CRhStatusBarProgressMeter* m_progressMeter;

  // our preview bitmap
  CRhinoDib* m_pPreviewBitmap;

  ON_wString m_wCursorToolTip;
  bool m_bCursorToolTip;
  HCURSOR m_customToolTipCursor;

  bool m_inDelete;     // true when deleting this object

  CRhObjectDragHelper* m_pObjectDragHelper;
  CRhGetPointHelper* m_GetPointHelper;

#endif    // ON_RUNTIME_APPLE

public:
  // runtime information used when updating linked instance definitions
  class CRhIDefUpdate* m__idef_update;

  /*
  Description:
    Determine if a reference model is currently being read and get
    information about what type of reference is being read.  This
    information is sometimes required by file reading plug-ins.

  Parameters:
    worksession_ref_model_sn - [out]
      If a work session reference model is currently being read, then
      the returned value of *worksession_ref_model_sn will be the
      non-zero work session reference model serial number.  Otherwise
      the returned value of *worksession_ref_model_sn will be zero.
      Pass null if you do not want this information.

    idef - [out]
      If an instance definition reference model is currently
      being read, then the returned value of idef will be
      non-null.  When the Insert command is being used to
      create a block definition, this idef can be a static,
      linked and embedded, or linked idef. When an out-of-date
      block definition is being updated, this idef can be
      linked and embedded or linked.
      Pass null if you do not want this information.

    grandparent_layer_id - [out]
      If a reference model is currently being read, then the returned
      value of *grandparent_layer_id will be the id of the grandparent
      layer for that reference model.  Otherwise the returned value of
      *grandparent_layer_id will be nil.
      Pass null if you do not want this information.

  Returns:
    True if a reference file is currently being read.

  Remarks:
    Worksession reference models can contain linked instance
    definitions and linked instance definitions can contain
    embedded instance definitions.
  */
  bool ReadingAncillaryModel(
    unsigned int* worksession_ref_model_sn,
    const CRhinoInstanceDefinition** idef,
    ON_UUID* grandparent_layer_id
  ) const;

#if defined(RHINO_COMPILING_CORE_EXE)
  /* */
  bool CreateTableElementHelper(unsigned int& worksession_sn, unsigned int& linked_idef_sn) const;
#endif

private:
  bool DeleteObjectHelper(const CRhinoObjRef& objref, bool, bool, bool);
  void UpdateRenderingInfoUseCounts(CRhinoObject*, int);
  void UpdateRenderingInfoUseCounts(CRhinoLayer*, int);

protected:

#ifdef SHARED_HANDLERS
  //DALELEARV6// VS 2012 added this SHARED_HANDLERS section
  // Helper function that sets search content for a Search Handler
  void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
  // Returns a translated string to use prepend to a Dimstyle name to
  // mark it as a built-in default dimstyle
  static const wchar_t* DefaultDimstyleNamePrefix();
  // Creates some default dimstyles and stores them in the dimstyles array
  // returns the number of styles made
  int CreateDefaultDimstyles(ON_ClassArray<ON_DimStyle>& dimstyles);

  //  CRhinoDimstyleTable* DimstyleTable();


    //Re-implement CDocument
  ON_wString GetPathName() const;
  void SetModifiedFlag(BOOL32 bModified = TRUE);
  BOOL32 IsModified(void) const;
  BOOL32 SaveModified(void);

  void OnCloseDocument();
  BOOL32 OnNewDocument();

  BOOL32 OnOpenDocument(LPCTSTR lpszPathName);
  void SetPathName(LPCTSTR, BOOL32 bAddToMRU = TRUE);

  void ClearPathName(void);

  void SetTitle(LPCTSTR);
  BOOL32 DoSave(LPCTSTR lpszPathName, BOOL32 bReplace = TRUE);
  BOOL32 OnSaveDocument(LPCTSTR lpszPathName);
  BOOL32 DoFileSave();
  ON_wString GetTitle(void) const;
  void SetAutoDelete(BOOL32 bAutoDelete);
  BOOL32 AutoDelete(void) const;

  void BeginWaitCursor();
  void EndWaitCursor();
  void RestoreWaitCursor();       // call after MessageBox

#if defined (RHINO_SDK_MFC)
  CDocTemplate* GetDocTemplate() const;
  POSITION GetFirstViewPosition() const;
  CView* GetNextView(POSITION& rPosition) const;
#if defined(ON_RUNTIME_WIN)
  void OnDocumentEvent(CDocument::DocumentEvent deEvent);
#endif
  AFX_MODULE_STATE* ModuleState(void) const;
  const CDocument* SafeMFCDoc(void) const;
  CDocument* SafeMFCDoc(void);
#endif

private:
  class CRhDoc& m_windoc;
  friend class CRhDoc;
};


/*
Description:
  The CRhinoObjectIterator class is used to iterate through
  the objects in a CRhinoDoc.

Example:

          // The default CRhinoObjectIterator iterates
          // through all visible objects in the active document.
          CRhinoObjectIterator it();
          CRhino3Object* pObject;
          for ( pObject = it.First(); pObject; pObject = it.Next() )
          {
            ...
          }
*/

class RHINO_SDK_CLASS CRhinoObjectIterator
{
public:

  // Description:
  //   There are 5 mutually exclusive object states: normal, locked, hidden,
  //   instance, and deleted.  The "instance" objects are the objects used
  //   to define the geometry of an instance definition.  The instance
  //   references (instance objects) are in the normal, locked, hidden 
  //   or deleted state.
  //
  enum object_state
  {
    // DO NOT CHANGE THESE ENUM VALUES - IT WILL BREAK THE SDK
    normal_objects = 1, // (not locked and not hidden)
    locked_objects = 2, // (locked objects or on locked layers)
    hidden_objects = 4, // (hidden objects or on hidden layers)
    idef_objects = 8, // objects in instance definitions (not the instance references)
    deleted_objects = 16,
    normal_or_locked_objects = 3, // normal or locked
    undeleted_objects = 7,       // normal, locked, or hidden
    all_objects = 0xFFFFFFFF,     // normal, locked, hidden, or deleted (not idefs)
    undeleted_and_idef_objects = 0x7FFFFFFF  // normal, locked, hidden and idef
  };

  // Description:
  //   There are 2 mutually exclusive object categories: active and reference.
  //   By default objects are active.  If a Rhino file is loaded as a
  //   reference model, it's objects are reference objects.  Reference
  //   objects cannot be modified and are never saved.
  enum object_category
  {
    // DO NOT CHANGE THESE ENUM VALUES - IT WILL BREAK THE SDK
    active_objects = 1,    // objects that are part of current model and saved in file
    reference_objects = 2, // objects that are for reference and not saved in file
    active_and_reference_objects = 3
  };

  // Description:
  //   This constructor iterates through all active documents
  //
  // Parameters:
  //   s - [in] specifies what object states should be iterated
  //   c - [in] specifies what object categories should be iterated
  CRhinoObjectIterator(unsigned int rhino_doc_sn,
    object_state = normal_or_locked_objects,
    object_category = active_objects
  );

  CRhinoObjectIterator(const CRhinoDoc&,
    object_state = normal_or_locked_objects,
    object_category = active_objects
  );

  // obsolete constructor - you must specify the document
  CRhinoObjectIterator(object_state s = normal_or_locked_objects,
    object_category c = active_objects
  );

  // This constructor is useful for iterating though objects t
  // when writing files.
  CRhinoObjectIterator(
    unsigned int rhino_doc_sn,
    const class CRhinoFileWriteOptions& fwo
  );

  ~CRhinoObjectIterator();

  /*
  Description:
    The default object iterator does not iterate through render
    light objects.  If you want the iterator to include lights,
    then call IncludeLights(true).
  Parameters:
    bIncludeLights - [in] true to include lights in the iteration
  Remarks:
    If you just want to look at render lights, it may be easier to
    use the CRhinoDoc::m_light_table[] array.
  */
  void IncludeLights(BOOL32 bIncludeLights = true);

  /*
  Description:
    The default object iterator does not iterate through grip
    objects.  If you want the iterator to include lights,
    then call IncludeGrips(true).
  Parameters:
    bIncludeGrips - [in] true to include grips in the iteration
  */
  void IncludeGrips(bool bIncludeGrips = true);


  /*
  Description:
    The default object iterator does not iterate through phantom
    objects.  If you want the iterator to include phantoms,
    then call IncludePhantoms(true).
  Parameters:
    bIncludePhantoms - [in] true to include phantom objects
                            in the iteration
  */
  void IncludePhantoms(bool bIncludePhantoms = true);

  /*
    Description:
      If the selected filter is enabled, then the iterator will
      limit itself to selected objects.  The default is to
      ignore selection state.

    Parameters:
      b - [in] true limits the iteration to selected objects.
      bCheckSubObjects - [in] if true and b=true, then the iteration
         will include objects that have some subset of the object selected,
         like some edges of a surface. The default is ignore objects
         where the entire object is not selected.

    Example:
      If you want to iterate through the list of selected
      points and curves, then you would do something like:

          CRhinoObjectIterator it();
          it.EnableSelectedFilter(true);
          it.SetObjectFilter( ON::point_object | ON::curve_object );
          CRhino3Object* pObject;
          for ( pObject = it.First(); pObject; pObject = it.Next() )
          {
            ...
          }

    See Also:
      CRhinoObjectIterator::SetObjectFilter
      CRhinoObjectIterator::SetLayerFilter
      CRhinoObjectIterator::EnableSelectedFilter
      CRhinoObjectIterator::EnableVisibleFilter
  */
  void EnableSelectedFilter(BOOL32 b = true, BOOL32 bCheckSubObjects = false);

  /*
    Description:
      If the visible filter is enabled, then the iterator will
      limit itself to visible objects.  The default is to
      ignore visibility.

    Parameters:
      b - [in] true limits the iteration to visible objects.

    Remarks:
      Hidden objects and objects on layers that are turned off
      are not visible.

    See Also:
      CRhinoObjectIterator::SetObjectFilter
      CRhinoObjectIterator::SetLayerFilter
      CRhinoObjectIterator::EnableSelectedFilter
      CRhinoObjectIterator::EnableVisibleFilter
  */
  void EnableVisibleFilter(BOOL32 b = true);

  /*
  Description:
    The object filter can be used to limit the iteration to
    specific types of geometry.

  Parameters:
    object_filter - [in] an unsigned int made by or-ing ( | )
        values from the ON::object_type enum.  If you pass 0,
        then all objects will be iterated.

  Example:
    If you want to iterate through the list of mesh and
    brep objects, then you would do something like:

          CRhinoObjectIterator it();
          it.SetObjectFilter( ON::brep_object | ON::mesh_object );
          CRhino3Object* pObject;
          for ( pObject = it.First(); pObject; pObject = it.Next() )
          {
            ...
          }

  Remarks:
    If object_filter has the ON::light_object bit set, then iterating
    through lights is automatically enabled.

  See Also:
    CRhinoObjectIterator::SetObjectFilter
    CRhinoObjectIterator::SetLayerFilter
    CRhinoObjectIterator::EnableSelectedFilter
    CRhinoObjectIterator::EnableVisibleFilter
  */
  void SetObjectFilter(unsigned int object_filter);


  /*
    Description:
      The layer filter can be used to limit the iteration to
      objects on a specific layer.
    Parameters:
      layer_index - [in] index of layer or -1 do disable layer
         filtering.
    Example:
      If you want to iterate through the list of objects
      that are on mesh and
      brep objects, then you would do something like:

          const CRhinoLayer& layer = ...
          int layer_index = layer.LayerIndex();

               // or

          CRhinoObject& object = ...;
          int layer_index = object.Attributes().m_layer_index;

          CRhinoObjectIterator it();
          it.SetLayerFilter( layer_index );
          CRhino3Object* pObject;
          for ( pObject = it.First(); pObject; pObject = it.Next() )
          {
            ...
          }
    Remarks:
      By default, no layer filter is enabled.
    See Also:
      CRhinoObjectIterator::SetObjectFilter
      CRhinoObjectIterator::SetLayerFilter
      CRhinoObjectIterator::EnableSelectedFilter
      CRhinoObjectIterator::EnableVisibleFilter
  */
  void SetLayerFilter(int layer_index);


  /*
  Description:
    This function is a one-call way to set up the common
    filter that quickly weeds out objects in the wrong
    space or on the wrong page view.

    if ( ON::page_view_type == viewport.m_v.m_view_type )
    {
      SetSpaceFilter(true,ON::page_space,true);
      SetViewportIdFilter(true,viewport.ViewportId(),true);
    }
    else
    {
      SetSpaceFilter(true,ON::model_space,true);
      SetViewportIdFilter(false,ON_nil_uuid,false);
    }

  Parameters:
    viewport - [in]
  */
  void SetViewportFilter(const CRhinoViewport& viewport);


  /*
  Description:
    Filter on value of object->IsActiveInViewport()

  Parameters:
    bEnable - [in] true to enable the filter
    viewport - [in]
      The caller is responsible for making sure
      this class persists until the iterator is
      no longer needed.  Otherwise, you will crash.
    bFilter - [in]
      An object is included in the iteration only if
      bFilter = object->IsActiveInViewport(viewport)
      is active in that view.
  */
  void SetActiveInViewportFilter(
    bool bEnable,
    const CRhinoViewport& viewport,
    bool bFilter
  );

  /*
  Description:
    The object space filter can be used to filter
    on the value of ON_3dmObjectAttributes.m_space.
  Parameters:
    bEnable - [in] true to enable this filter. (default is false).
    space - [in]
    bFilter - [in]
       If bEnable is true, then the object will be included in
       the iteration if and only if
       bFilter == ( space == object_attributes.m_space).
  Example:
    If you want to iterate through the list of objects
    that are in model space

        CRhinoObjectIterator it();
        it.SetObjectSpaceFilter(true,ON::model_space,true);

        CRhino3Object* pObject;
        for ( pObject = it.First(); pObject; pObject = it.Next() )
        {
          ...
        }

    If you want to iterate through the list of objects
    that are active in a specific page view.

        CRhinoObjectIterator it();
        it.SetObjectSpaceFilter(true,ON::page_space,true);
        const CRhinoViewport* page_viewport = ...;
        it.SetViewportIdFilter(
                              true,
                              page_viewport->ViewportId(),
                              true);

        CRhino3Object* pObject;
        for ( pObject = it.First(); pObject; pObject = it.Next() )
        {
          ...
        }

  See Also:
    CRhinoObjectIterator::SetViewportFilter

  Remarks:
    Model space objects and model views are not special.
    Page view objects, like title block annotation, are
    marked as special and page views are marked as special
    views.  This filter is primarily used to limit the
    iteration to objects that are only in a certain page view
    or objects that are in model space.
  */
  void SetSpaceFilter(
    bool bEnable,
    ON::active_space space,
    bool bFilter
  );

  /*
  Description:
    The object space filter can be used to filter
    on the value of ON_3dmObjectAttributes.m_viewport_id.
  Parameters:
    bEnable - [in] true to enable this filter. (default is false).
    viewport_id - [in]
    bFilter - [in]
       If bEnable is true, then the object will be included in
       the iteration if and only if
       bFilter == ( viewport_id == object_attributes.m_viewport_id).

  See Also:
    CRhinoObjectIterator::SetViewportFilter
  */
  void SetViewportIdFilter(
    bool bEnable,
    ON_UUID viewport_id,
    bool bFilter
  );

  /*
  Description:
    Returns the first object in list. The next call to
    CRhinoObjectIterator::Next will return the second
    object in the list.
  Example:

          CRhinoObjectIterator it;
          CRhinoObject* obj;
          for ( obj = it.First(); obj; obj = it.Next() )
          {
            ...
          }

  Returns:
    First object in the list.
  See Also:
    CRhinoObjectIterator::Next
  */
  CRhinoObject* First();

  /*
  Description:
    Returns the next object in list.  Returns NULL when
    no more objects are in the list.
  Example:

          CRhinoObjectIterator it;
          CRhinoObject* obj;
          for ( obj = it.First(); obj; obj = it.Next() )
          {
            ...
          }

  Returns:
    Next object in the list;
  See Also:
    CRhinoObjectIterator::First
  */
  CRhinoObject* Next();

  // document being iterated.
  const unsigned int m_rhino_doc_sn;

  /*
  Returns:
    CRhinoDoc::FromRuntimeSerialNumber(m_rhino_doc_sn)
  Remarks:
    Returned pointer will be null if document
    has been deleted or an invalid document was
    used in the constructor.
  */
  CRhinoDoc* Document() const;

private:
  void Init(object_state, object_category);

  object_state m_os = CRhinoObjectIterator::object_state::normal_or_locked_objects; // state of objects being iterated
  object_category m_oc = CRhinoObjectIterator::object_category::active_objects; // category of objects being iterated

  CRhinoObject* m_current = nullptr;
  CRhinoObject* m_current_next = nullptr; // saves m_current->m_next because user may delete m_current
  int m_current_light_index = 0;

  CRhinoObject* m_list[16];
  int m_list_count = 0;
  int m_current_list = 0;
  bool m_bIncludeLights = false; // true if lights should be included.
  bool m_bIncludeGrips = false; // true if grips should be included.
  bool m_bIncludePhantoms = false; // true if phantoms should include
  bool m_bFilterCheck = false; // true if some filter needs to be checked.
  bool m_bSelected = false; // true if iterator only returns selected objects
  bool m_bCheckSubObjects = false; // true if iterator returns partially selected objects
  bool m_bVisible = false; // true if iterator only returns visible objects
  unsigned int m_object_filter = 0;
  int m_layer_index_filter = -1;

  //// 2 September 2005 Dale Lear
  //bool m_bEnableSpecialViewportFilter; // true if object attributes' m_bSpecialViewport is must
  //                               // match m_bSpecialViewportFilter.
  //bool m_bSpecialViewportFilter;

  //bool m_bEnableDisplayMaterialRefFilter; // true if display material ref filter is enabled.
  //ON_DisplayMaterialRef m_dmr;
  //bool m_bDisplayMaterialRefFilter;

  // 21 September 2006 Dale Lear
  // If m_bEnableSpaceFilter is true, then an only objects
  // that satisfy
  //   m_bSpaceFilter == (m_object_space == object_attrs.m_space)
  // are returned by the iterator.
  bool m_bEnableSpaceFilter = false;
  ON::active_space m_object_space = ON::active_space::no_space;
  bool m_bSpaceFilter = false;

  // If m_bEnableViewportIdFilter is true, then an only objects
  // that satisfy
  //   m_bViewportIdFilter == (m_object_viewport_id == object_attrs.m_vieport_id)
  // are returned by the iterator.
  bool m_bEnableViewportIdFilter = false;
  ON_UUID m_object_viewport_id = ON_nil_uuid;
  bool m_bViewportIdFilter = false;

  // If m_bEnableActiveViewportFilter is true, then an only objects
  // that satisfy
  //   m_bActiveViewportFilter == rhino_object->IsActiveInViewport(*m_active_viewport)
  // are returned by the iterator.
  bool m_bEnableActiveViewportFilter = false;
  const CRhinoViewport* m_active_viewport = nullptr;
  bool m_bActiveViewportFilter = false;

  bool SkipIt() const;
private:
  friend class CRhinoDisplayPipeline;
  void SetBuilingPipelineDrawLists(bool on);
  bool BuildingPipelineDrawLists() const;

  class CRhinoObjectIterator_Private* m_iterator_private = nullptr;
};


/*
Description:
  Tool to go through rhino objects that are on a specified page
*/
class RHINO_SDK_CLASS CRhinoPageViewObjectIterator : public CRhinoObjectIterator
{
public:
  CRhinoPageViewObjectIterator(
    unsigned int rhino_page_view_serial_number,
    object_state = normal_or_locked_objects,
    object_category = active_objects
  );
  ~CRhinoPageViewObjectIterator();

  const unsigned int m_rhino_page_view_sn;
  const ON_UUID m_rhino_page_view_main_viewport_id;
  const bool m_bValidIterator;

private:
  // no copies
  CRhinoPageViewObjectIterator(const CRhinoPageViewObjectIterator&); // no implementation
  CRhinoPageViewObjectIterator& operator=(const CRhinoPageViewObjectIterator&); // no implementation
};
