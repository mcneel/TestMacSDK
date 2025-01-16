
#pragma once // Public SDK header

#include "RhRdkEnvironment.h"
#include "RhRdkEventWatcher.h"
#include "IRhRdkCurrentEnvironment.h"
#include "IRhRdkSkylight.h"

/** This enum is obsolete. */
enum RDK_DEPRECATED eRdkToneMappingMethod { tmm_none, tmm_black_white_point, tmm_logarithmic, tmm_reinhard_2001, tmm_reinhard_2005 };

class RHRDK_SDK CRhRdkContentFilterInfo final
{
public:
	CRhRdkContentFilterInfo();
	~CRhRdkContentFilterInfo();

	ON_wString m_sExcluded;
	ON_wString m_sNameFilter;
	bool m_bNameFilterInvert;
	bool m_bShowUnassigned;
	bool m_bShowV4;
	bool m_bShowHidden;
	bool m_bShowReference;
	void* m_pReserved;
};

class RHRDK_SDK IRhRdkContentIterator
{
public:
	virtual ~IRhRdkContentIterator() { }

	/** \return A pointer to the next content in the owned collection or null if no more exist. */
	virtual const CRhRdkContent* Next(void) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RHRDK_SDK CRhRdkDocument : public CRhRdkObject
{
public:
	/** This method is implemented as \code delete this; \endcode */
	virtual void DeleteThis(void);

	/** Begins a change or batch of changes. This returns a non-const reference to this document
		which you should use to make your changes. Calls to this method are counted; you must
		call EndChange() once for every call to BeginChange(). Please do not use const_cast or
		any other means of circumventing this requirement.
		\param cc is the change context.
		\note If you override this method, <b>please be sure to call the base class</b>.
		\see EndChange()
		\return A non-const reference to this document. */
	virtual CRhRdkDocument& BeginChange(RhRdkChangeContext cc) const;

	/** Ends a change or batch of changes. Calls to this method are counted; you must call
		EndChange() once for every call to BeginChange().
		\note If you override this method, <b>please be sure to call the base class</b>.
		\see BeginChange()
		\return \e true if a change has actually occurred, else \e false. */
	virtual bool EndChange(void);

	/** \return the unique identifier of this RDK document. */
	virtual UUID Id(void) const;

	/** \return the serial number of the Rhino document associated with this RDK document.
	This will return zero if the RDK document is not associated with a Rhino document. */
	virtual unsigned int RhinoDocSerial(void) const;

	/** \return a pointer to the Rhino document associated with this RDK document.
	This will return null if the RDK document is not associated with a Rhino document. */
	virtual CRhinoDoc* RhinoDoc(void) const;

	/** Calls UpdateDocumentTables() on all the contents in the RDK document. */
	virtual void UpdateDocumentTables(void);

	/** A 'sandbox' document is a temporary document that is not associated with a Rhino document.
		Sandboxes are used to hold and manipulate temporary contents in, for example, the Modal Editor.
		\return \e true if the document is a sandbox document, else \e false. */
	virtual bool IsSandbox(void) const;

public: // Document matching.
	/** Checks if two document are the same (by id). */
	static bool Match(const CRhRdkDocument* pRdkDoc1, const CRhRdkDocument* pRdkDoc2);

	/** Checks if two document are the same (by id). */
	static bool Match(const CRhRdkDocument* pRdkDoc, const CRhRdkDocument& other);

	/** Checks if this document is the same as another document (by id). */
	bool Match(const CRhRdkDocument& other) const;

	/** Checks if this document is the same as another document (by id). */
	bool Match(const CRhRdkDocument* pOther) const;

public: // Content.

	/** \return \e true if content is filtered out according to document filter settings. */
	virtual bool IsContentFilteredOut(const CRhRdkContent& c) const;

	enum : unsigned int // Bit-flags for it_Flags parameter of NewContentIterator().
	{
		it_Normal             = 0x0000, // Normal; no flags.
		it_IncludeDefault     = 0x0001, // Includes the Default Material or Environment in the iteration if applicable.
		it_IncludeProxies     = 0x0002, // Includes legacy (V4) materials (material proxies) and Texture Proxies in the iteration.
		it_IncludeAutoDeleted = 0x0004, // Includes auto-deleted contents in the iteration.
		it_ForDisplay         = 0x0008, // Indicates that the items will be used in a content-list display. This can include Texture Proxies.
	};

	/** Get a new content iterator. The caller shall delete the iterator after use.
		\param kinds specifies the content kinds to return.
		\param it_Flags are flags that control the iterator. See above. */
	virtual IRhRdkContentIterator* NewContentIterator(const CRhRdkContentKindList& kinds, unsigned int it_Flags) const;

	/** Attach a content to the document by transferring ownership of it to the document.
		\param c is the content to attach.
		\param reason is the reason for the operation. If the reason is 'Attach' and content undo was not
		 recorded by the caller, then content undo will be recorded automatically.
		\note If successful, ownership of the content will be transferred to the document. If not, ownership
		 will remain with the caller. Failure to attach usually indicates that there is already a content with
		 the same instance id in the document. This situation is not allowed and the new content will be rejected.
		\note If the content is a child or already attached to this or another document, the method will fail.
		\return \e true if successful, else \e false. */
	virtual bool AttachContent(CRhRdkContent& c, CRhRdkEventWatcher::AttachReason reason=CRhRdkEventWatcher::AttachReason::Attach);

	/** Detach a content from the document. Ownership of the content will be transferred to either the caller
		or the undo system, depending on the 'reason' parameter.
		\note From Rhino 9 onwards, this method records undo if the reason is 'Delete'.
		\param c is the content to detach. If successful, ownership of the content will be transferred to the
		 caller if the reason is anything other than 'Delete', or to the undo system if the reason is 'Delete'.
		\param reason is the reason for the operation.
		\return A pointer to the non-const content if the reason is not 'Delete' and the operation succeeds.
		 If the reason is 'Delete', the method returns null because the content has been transferred to the
		 undo system and is no longer available for clients to use. If the method does return a content, then
		 it is safe to delete that content.
		\note If the content is a child or not attached to this document, the method will fail. */
	virtual CRhRdkContent* DetachContent(const CRhRdkContent& c, CRhRdkEventWatcher::DetachReason reason=CRhRdkEventWatcher::DetachReason::Detach);

	/** Replace a content in the document. Deprecated in favor of ReplaceContent(). */
	RDK_DEPRECATED virtual bool ChangeContent(const CRhRdkContent& oldContent, CRhRdkContent& newContent, bool bPreserve=true);

	/** Replace a content in the document.
		\param oldContent is the content to detach and delete.
		\param newContent is the content to attach in place of oldContent.
		 If successful, the old content will be deleted and ownership of the new content will
		 be transferred to the document.
		\param bPreserve determines whether or not the instance id and notes are preserved. If \e true,
		 the instance id of the old content is preserved in the new content. This means that any objects
		 or layers that were referencing the old content will reference the new content. The same applies to
		 the ground plane. The content's notes are also preserved. TODO: The name preserving is currently being reconsidered. [ANDY]
		\note The old content can be top-level or a child.
		\note If the old content is not attached to this document, the method will fail.
		\note If the new content is a child already attached to this or another document, the method will fail.
		\return \e true if successful, else \e false. */
	virtual_su bool ReplaceContent(const CRhRdkContent& oldContent, CRhRdkContent& newContent, bool bPreserve=true); // [SDK_UNFREEZE]

	/** Find a content by its instance id. All document-registered contents (including children) will be checked.
		\param bCheckGlobal determines if global contents should also be checked. Global contents are non-document
		 contents that happen to be on the stack or the heap. If this is \e true, global contents will be checked if the
		 content was not found in the document. If \e false, the search is restricted to document contents only.
		\note Prior to May 1st 2018, the bCheckGlobal parameter defaulted to \e true. This default has been removed
		 because it is important to think carefully about whether or not you need the global check. If you are
		 reading this because you suddenly got a compiler error, you can add ", true" to your call if you want to
		 keep the same behavior (which by default was checking globally if not found in the document).
		\note Cloned contents are never found as they are not registered in documents or globally.
		\param uuidInstance is the instance id of the content to find.
		\return A const pointer to the content or null if not found. If you want to modify the content
		 you will need to call BeginChange() to get a non-const reference. \see BeginChange() */
	virtual const CRhRdkContent* FindContentInstance(const UUID& uuidInstance, bool bCheckGlobal) const;

	/** Searches for contents with a particular instance name.
		\param kinds specifies which top-level content kinds should be searched.
		\param wszName is the name to search for.
		\param bCaseSensitive specifies if the name check is case-sensitive or not.
		\param bRecurse specifies if children of top-level contents should be searched.
		\param aContentOut accepts all the contents that satisfy the search.
		 If you want to modify these contents you will need to call BeginChange() to
		 get a non-const reference. \see BeginChange() */
	virtual void FindContentInstanceByName(const CRhRdkContentKindList& kinds,
	                                       const wchar_t* wszName, bool bCaseSensitive, bool bRecurse, CRhRdkContentArray& aContentOut) const;

	/** Searches for a content with the same name and appearance as another content.
		\note The name check is case-insensitive. The 'appearance' is the render CRC of the content.
		\param c is the content to match against.
		\return A pointer to a similar content if found, else null. */
	virtual const CRhRdkContent* FindContentWithSameNameAndAppearance(const CRhRdkContent& c) const;

	// This method is deprecated in favor of ShowContentInstanceBrowser().
	RDK_DEPRECATED virtual bool ChooseContent(UUID& uuidInstanceInOut, const CRhRdkContentKindList& kinds, UINT rdkcce_Flags) const;

	/** Allows the user to choose a content by displaying the Content Instance Browser dialog.
		\param instance_id sets the initially selected content and receives the instance id of the chosen content.
		\param kinds specifies the kind(s) of content that should be displayed in the browser.
		\param rdkcce_flags specifies flags controlling the browser. See enum in RhRdkDefinitions.h
		\return \e true if the user chooses a content, \e false if the dialog is cancelled. */
	virtual_su bool ShowContentInstanceBrowser(IN OUT UUID& instance_id, const CRhRdkContentKindList& kinds, UINT rdkcce_flags) const;

	/** Creates a new content of the specified type and attaches it to the document.
		\note This method calls Initialize() on the new content. If this fails, the content is deleted
		 and this method returns null.
		\note This function cannot be used to create temporary contents that you delete after use.
		Contents created by this function are owned by RDK and appear in the content editor.
		To create a temporary content which is owned by you, call RhRdkContentFactories().NewContentFromType().
		\see IRhRdkContentFactories::NewContentFromType()
		\param uuidNewType is the type of the content to create.
		\param pParent is the parent content. If not null, this must be an RDK-owned content that is attached to
		 the document (either top-level or child). The new content then becomes its child.
		 If null, the new content is added to the document's top-level content list instead.
		\param wszChildSlotName is the unique child identifier to use for the new content when creating it as a child
		 of pParent (i.e., when pParent is not null).
		\param uFlags is for future use only.
		\return A const pointer to the new content if successful, else null. If you want to modify the content
		 you will need to call BeginChange() to get a non-const reference. \see BeginChange() */
	virtual const CRhRdkContent* CreateContentByType(const UUID& uuidNewType, const CRhRdkContent* pParent=nullptr,
	                                                 const wchar_t* wszChildSlotName=L"", UINT uFlags=0);

	/** Bit-flags for CreateContentByUser() and ShowContentNewExistingBrowser() uFlags. */
	enum
	{
		ccbu_hide_new_tab          = 0x0001,
		ccbu_hide_existing_tab     = 0x0002,
		ccbu_multiple_selection    = 0x0004,
		ccbu_allow_none            = 0x0008,
		ccbu_disable_import_button = 0x0010,
		ccbu_hide_copy_or_share    = 0x0020, // Only used by ShowContentNewExistingBrowser().

		ccbu_force32bit = 0xFFFFFFFF
	};

	/** Create one or more new content(s) chosen by the user and attach them to the document.
		\note This function cannot be used to create temporary content that you delete after use.
		 Contents created by this function are owned by RDK and appear in the content editor.
		\param contents accepts the new content(s). If you want to modify the contents
		 you will need to call BeginChange() to get a non-const reference. \see BeginChange()
		\param uuidDefaultType is the default content type.
		\param uuidDefaultInstance is the default selected content instance.
		\param allowedKinds determines which content kinds are allowed to be chosen from the dialog.
		\param ccbu_flags is from the enum above with prefix ccbu_ -- NOTE: ccbu_hide_copy_or_share is ignored.
		\return \e true if successful, else \e false. */
	virtual bool CreateContentByUser(OUT CRhRdkContentArray& contents,
	                                 const UUID& uuidDefaultType, const UUID& uuidDefaultInstance,
	                                 const CRhRdkContentKindList& allowedKinds, UINT ccbu_flags=0);

	/** Bit-flags for ChangeChildContent() uFlags. */
	enum
	{
		ccc_AllowNone       = 0x0001, // Allow (none) item to be displayed in dialog.
		ccc_AutoEdit        = 0x0002, // OBSOLETE. The new content is opened in the relevant modeless thumbnail editor unless the user started from an existing content.
		ccc_Preserve        = 0x0004, // Specifies that the user should be given the opportunity to preserve the old child as a child of the new child.
		ccc_AutoEditBM      = 0x0008, // OBSOLETE. Bitmap textures are no longer given special treatment.
		ccc_NoGroupChange   = 0x0010, // If this flag is set, grouped contents will not be changed.
		ccc_HideNewTab      = 0x0020, // If this flag is set, then the new tab will be hidden.
		ccc_HideExistingTab = 0x0040, // If this flag is set, then the existing tab will be hidden.

		ccc_HarvestMask     = 0xF000,
		ccc_HarvestUseOpt   = 0x0000, // Use Renderer Support option to decide about harvesting.
		ccc_HarvestAlways   = 0x1000, // Always copy similar parameters from old child.
		ccc_HarvestNever    = 0x2000, // Never copy similar parameters from old child.

		ccc_force32bit = 0xFFFFFFFF
	};
	/** Change a content's child by allowing the user to choose the new content type from a
		content browser dialog. The child is created if it does not exist, otherwise the old
		child is deleted and replaced by the new child (unless ccc_Preserve is specified in
		which case the old child is optionally preserved as a child of the new child).
		\param parent is the content whose child is to be manipulated.
		\param wszChildSlotName is the child-slot name of the child to be manipulated.
		\param allowedKinds determines which content kinds are allowed to be chosen from the content browser dialog.
		\param ccc_Flags is a set of flags for controlling the operation of the function (see ccc_ enum above).
		\note If parent is not attached to this document (either top-level or child), the function will fail.
		\return A pointer to the new content if successful, null if it fails or if the user cancels.
		 If you want to modify the content you will need to call BeginChange() to get a non-const reference.
		\see BeginChange() */
	virtual const CRhRdkContent* ChangeChildContent(CRhRdkContent& parent, const wchar_t* wszChildSlotName,
                                 const CRhRdkContentKindList& allowedKinds, UINT ccc_Flags=ccc_AllowNone|ccc_AutoEdit);

	/** Result of ShowContentNewExistingBrowser() */
	enum class SctbResult : unsigned int
	{
		kNone,      // No choice (user cancelled).
		kNew,       // User chose from 'New' tab. contents accepts the type ids(s).
		kCopy,      // User chose from 'Existing' tab with 'copy' radio button checked. contents accepts the instance ids(s).
		kInstance,  // User chose from 'Existing' tab with 'instance' radio button checked. contents accepts the instance id(s).
		kImport,    // User clicked the Import button.
		kReference, // User chose from 'Existing' tab when the radio buttons are hidden. contents accepts the instance id(s).
	};

	/** Parameters for ShowContentNewExistingBrowser() */
	class RHRDK_SDK CTypeBrowserParams final // Should be called ContentNewExistingBrowserParams but awkward to change. [SDK_UNFREEZE]
	{
	public:
		CTypeBrowserParams();
		~CTypeBrowserParams();

		// The content type that will be initially selected in the 'New' tab.
		ON_UUID m_uuidDefaultType;

		// The content instance that will be initially selected in the 'Existing' tab.
		ON_UUID m_uuidDefaultInstance;

		// Specifies which content kinds will be displayed.
		CRhRdkContentKindList m_AllowedKinds;

		// Specifies the category to preset in the drop-down list on the 'New' tab.
		// If this string is empty, the preset category will be 'All'.
		ON_wString m_sPresetCategory;

		// Specifies categories to display. Contents that do not belong to one of these categories will not be listed.
		// If this array is empty, all contents from all categories will be listed.
		ON_ClassArray<ON_wString> m_aListCategories;

		// Specifies content types to display. Contents that are not one of these types will not be listed.
		// If this array is empty, all content types will be listed.
		ON_SimpleArray<ON_UUID> m_aListTypes;

		// Flags from the enum above with the prefix ccbu_
		unsigned int m_ccbuFlags;

		// For RDK internal use only.
		void* m_pReserved;
	};

	using ContentNewExistingBrowserParams = CTypeBrowserParams; // [SDK_UNFREEZE] Change CTypeBrowserParams to ContentNewExistingBrowserParams.

	/** This method is deprecated in favor of ShowContentNewExistingBrowser(). */
	RDK_DEPRECATED virtual SctbResult ShowContentTypeBrowser(const UUID&, const UUID&, const CRhRdkContentKindList&, CRhRdkInstanceIdArray&, UINT=0) const;

	/** This method is deprecated in favor of ShowContentNewExistingBrowser(). */
	RDK_DEPRECATED virtual_su SctbResult ShowContentTypeBrowser(const CTypeBrowserParams&, CRhRdkInstanceIdArray&) const; // [SDK_UNFREEZE]

	/** Shows the Content New / Existing Browser to allow the user to choose one or more new or existing contents.
		\param params specifies various parameters that control the browser.
		\param contents accepts the UUID(s) of the chosen item(s). Depending on SctbResult, this can be
		 type ids of content types or instance ids of existing contents.
		\return A value from the SctbResult enum giving the result of the operation. */
	virtual_su SctbResult ShowContentNewExistingBrowser(const ContentNewExistingBrowserParams& params,
	                                                    OUT CRhRdkContentIdArray& ids) const; // [SDK_UNFREEZE]

	virtual bool IsRenderEngineExcluded(const UUID& uuidRenderEngine) const;

public: // Embedded support files.

	// These two functions are deprecated because this information is not supposed to be stored per-document.
	RDK_DEPRECATED virtual bool EmbedSupportFilesOn(void) const;
	RDK_DEPRECATED virtual void SetEmbedSupportFilesOn(bool b);

	virtual TextureReportResult TextureReport(TextureReportFilter initialFilter, bool bAllowAbort, bool bForceDisplayIfEmpty) const;

	virtual void GetEmbeddedFilesList(TextureReportFilter filter, ON_ClassArray<ON_wString>& sFiles) const;

public: // Tone mapping.

	/** \return \e true if tone mapping is to be applied when rendering, else \e false. */
	RDK_DEPRECATED virtual bool ApplyToneMappingWhileRendering(void) const;

	/** Set whether or not tone mapping is to be applied when rendering. */
	RDK_DEPRECATED virtual void SetApplyToneMappingWhileRendering(bool b);

	/** This method is deprecated in favor of CRhinoDoc::Properties().RenderSettings().PostEffects(). */
	RDK_DEPRECATED virtual IRhRdkToneMapping::Methods ToneMappingMethod(void) const;

	/** This method is obsolete. */
	RDK_DEPRECATED virtual void SetToneMappingMethod(eRdkToneMappingMethod tmm);

	/** This method is deprecated in favor of CRhinoDoc::Properties().RenderSettings().PostEffects(). */
	RDK_DEPRECATED virtual void SetToneMappingMethod(IRhRdkToneMapping::Methods tmm);

	/** This method is obsolete. */
	RDK_DEPRECATED virtual void GetToneMapperParams(IRhRdk_XMLSection& section) const;

	/** This method is obsolete. */
	RDK_DEPRECATED virtual void SetToneMapperParams(const IRhRdk_XMLSection& section);

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().PostEffects().
	RDK_DEPRECATED UUID PostEffectSelection(const wchar_t*) const;
	RDK_DEPRECATED void SetPostEffectSelection(const wchar_t*, const UUID&);

public: // Content access.
	      IRhRdkContents& Contents(void);
	const IRhRdkContents& Contents(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().GroundPlane().
	RDK_DEPRECATED       IRhRdkGroundPlane& GroundPlane(void);
	RDK_DEPRECATED const IRhRdkGroundPlane& GroundPlane(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().Dithering().
	RDK_DEPRECATED       IRhRdkDithering& Dithering(void);
	RDK_DEPRECATED const IRhRdkDithering& Dithering(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().SafeFrame().
	RDK_DEPRECATED       IRhRdkSafeFrame& SafeFrame(void);
	RDK_DEPRECATED const IRhRdkSafeFrame& SafeFrame(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().Skylight().
	RDK_DEPRECATED       IRhRdkSkylight& Skylight(void);
	RDK_DEPRECATED const IRhRdkSkylight& Skylight(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().LinearWorkflow().
	RDK_DEPRECATED       IRhRdkLinearWorkflow& LinearWorkflow(void);
	RDK_DEPRECATED const IRhRdkLinearWorkflow& LinearWorkflow(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().RenderChannels().
	RDK_DEPRECATED       IRhRdkRenderChannels& RenderChannels(void);
	RDK_DEPRECATED const IRhRdkRenderChannels& RenderChannels(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings().Sun().
	RDK_DEPRECATED       IRhRdkSun& Sun(void);
	RDK_DEPRECATED const IRhRdkSun& Sun(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings() functions.
	RDK_DEPRECATED       IRhRdkPostEffects& PostEffects(void);
	RDK_DEPRECATED const IRhRdkPostEffects& PostEffects(void) const;

public: // Deprecated in favor of CRhinoDoc::Properties().RenderSettings() functions.
	RDK_DEPRECATED       IRhRdkCurrentEnvironment& CurrentEnvironment(void);
	RDK_DEPRECATED const IRhRdkCurrentEnvironment& CurrentEnvironment(void) const;

public: // Settings.

	/** Get a document setting by its name. */
	virtual CRhRdkVariant Setting(const wchar_t* wszName) const;

	/** Set a document setting by its name. */
	virtual void SetSetting(const wchar_t* wszName, const CRhRdkVariant& vValue);

public: // Static methods.

	/** Get the RDK document which is associated with the given Rhino document.
		\param doc is the Rhino document.
		\return A reference to the RDK document associated with the given Rhino document. */
	static const CRhRdkDocument& Get(const CRhinoDoc& doc);

	/** Get an RDK document by its unique identifier.
		\param uuidRdkDoc is the identifier of the RDK document.
		\return A pointer to the RDK document identified by uuidRdkDoc, or null if not found. */
	static const CRhRdkDocument* Get(const UUID& uuidRdkDoc);

	/** Get the RDK document which is associated with an array of content.
		\note If all the content is not attached to the same document, the method will fail.
		\param aContent is the array of content.
		\return A pointer to the common RDK document or null if none. */
	static const CRhRdkDocument* Get(const CRhRdkContentArray& aContent);

public: // Persistent list access.

	/** \return A reference to the material list.
		\note This reference is to const because you can only read the list. */
	virtual const IRhRdkContentList& MaterialList(void) const;

	/** \return A reference to the environment list.
		\note This reference is to const because you can only read the list. */
	virtual const IRhRdkContentList& EnvironmentList(void) const;

	/** \return A reference to the texture list.
		\note This reference is to const because you can only read the list. */
	virtual const IRhRdkContentList& TextureList(void) const;

	/** \return A pointer to the content list for a particular content kind.
		\param kind is the content kind for which to find the list.
		\note This pointer is to const because you can only read the list.
		\note If kind is 'Unset', the method will return null.
		\note If the document is being closed, the method will return null.
		\return A pointer to the content list for the given kind or null on failure. */
	virtual const IRhRdkContentList* ContentList(CRhRdkContent::Kinds kind) const;

public: // Content filters.
	virtual void GetContentFilterInfo(CRhRdkContentFilterInfo& infoOut) const;
	virtual void SetContentFilterInfo(const CRhRdkContentFilterInfo& infoNew, const CRhRdkContentFilterInfo& infoOld);

public://
	ON_UnitSystem UnitSystem(void) const;

public: // Debugging.
	RDK_DEPRECATED ON_wString DebugString(void) const;
	ON_wString DebugString(int uuid_len) const;

protected: // For RDK internal use only.
	explicit CRhRdkDocument(bool);
	explicit CRhRdkDocument(unsigned int);
	virtual ~CRhRdkDocument();
	friend class CRhRdkDocuments;

private: // Not implemented.
	CRhRdkDocument(const CRhRdkDocument& rdkDoc);
	const CRhRdkDocument& operator = (const CRhRdkDocument& rdkDoc);

private:
	class CImpl;
	CImpl* m_pImpl;
	friend class IRhRdkContents;

public:
	/** \internal For RDK internal use only. */
	CImpl& Impl(void) const;
};

class RHRDK_SDK CRhRdkImportAllowAutoInstanceName final
{
public:
	CRhRdkImportAllowAutoInstanceName();
	~CRhRdkImportAllowAutoInstanceName();
};

/** \class CRhRdkUndoableEvent

	The original version of the undoable event was intended for use with a document.
	This was before the data source system which was supposed to abstract the document away.
	Since 3rd-party plug-ins cannot actually use the data source system, this might still be
	useful for them.

*/
class RHRDK_SDK CRhRdkUndoableEvent : public CRhRdkObject
{
public:
	/** Construct using a Rhino document. */
	CRhRdkUndoableEvent(CRhinoDoc& doc, const wchar_t* wszDesc);

	/** Construct using a Rhino document pointer. If the pointer is null, the class does nothing. */
	CRhRdkUndoableEvent(CRhinoDoc* pDoc, const wchar_t* wszDesc);

	/** Construct using an RDK document pointer. If the pointer is null, the class does nothing. */
	CRhRdkUndoableEvent(const CRhRdkDocument* pRdkDoc, const wchar_t* wszDesc);

	virtual ~CRhRdkUndoableEvent();

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkNewUndoableEvent

	The new version of the undoable event was intended for use with the data source system
	which in the end did not belong in the public SDK. We still use it internally, and the
	use by the custom content UI could still be useful for 3rd-party plug-ins.

	Note for RDK developers: this is not actually used by the RDK Eto UI.
	Instead, the UI uses the underlying data source directly via IRhRdkUndo.

*/
class RHRDK_SDK CRhRdkNewUndoableEvent : public CRhRdkObject
{
public:
	/** \internal For RDK internal use only. Construct using a data source. */
	CRhRdkNewUndoableEvent(const IRhinoUiDataSource& h, const wchar_t* undo_string);

	/** \internal For RDK internal use only. Construct using a data source host. */
	CRhRdkNewUndoableEvent(const IRhinoUiDataSourceHost& h, const wchar_t* undo_string);

	/** Construct using a custom content UI. */
	CRhRdkNewUndoableEvent(const CRhRdkCustomContentUI& ccui, const wchar_t* undo_string);

	/** \internal For RDK internal use only. Construct using a data source host.
		This constructor allows a null host and just does nothing.
		Convenient when you want to put one on the stack without any hassle. */
	CRhRdkNewUndoableEvent(const IRhinoUiDataSourceHost* pHost, const wchar_t* undo_string);

	virtual ~CRhRdkNewUndoableEvent();

private:
	class CImpl;
	CImpl* m_pImpl;
};

// These functions can be used to create the undo strings used above.
RDK_INTERNAL RHRDK_SDK ON_wString RhRdkSmartUndoString(CRhRdkControllerPtr con,  const wchar_t* wszField, const wchar_t* wszNoField=nullptr);
RDK_INTERNAL RHRDK_SDK ON_wString RhRdkSmartUndoString(const IRhinoUiSection& s, const wchar_t* wszField, const wchar_t* wszNoField=nullptr);
