
#pragma once // Public SDK header

class CRhRdkDocument;
class CRhRdkBasicMaterial;
class CRhRdkUuidCollection;
class CRhRdkPreviewJobSignature;

#include "RhRdkContent.h"
#include "RhRdkRenderQuality.h"

/** \class IRhRdkRenderingFileInfo

	This interface is used by the OnRenderingLoaded, OnRenderingSaved and OnRenderingDeleted events.

*/
class RHRDK_SDK IRhRdkRenderingFileInfo
{
public:
	/** \return The full path of file name. */
	virtual const wchar_t* Filename(void) const = 0;

	/** \return The name of the renderer that created the file. */
	virtual const wchar_t* Renderer(void) const = 0;

	/** \return The render engine id of the renderer that created the file. */
	virtual const UUID& RenderEngineId(void) const = 0;

	/** \return The render session id. This will return nil if the rendering was loaded
		 into a standalone render window. \see IRhRdkRenderWindow::Create()
		\see RhRdkFindRenderSession() \see RhRdkFindRenderWindow() */
	virtual const UUID& RenderSessionId(void) const = 0;

	/** \return The time taken for rendering. Legacy value for backward compatibility with old files.
		If zero, use StartTimeInMilliseconds() and EndTimeInMilliseconds instead. */
	virtual int ElapsedSeconds(void) const = 0;

	virtual_su ON__UINT64 StartTimeInMilliseconds(void) const; // = 0; [SDK_UNFREEZE] Uses EVF "StartTimeInMilliseconds" for now.
	virtual_su ON__UINT64   EndTimeInMilliseconds(void) const; // = 0; [SDK_UNFREEZE] Uses EVF "EndTimeInMilliseconds" for now.

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkRenderingFileInfo() { }
};

/** \class CRhRdkEventWatcher

	Base class for auto-registering 'event watchers'. To allow a class to receive events, derive it from
	CRhRdkEventWatcher and override the methods which you want to be called on. Some of these methods may
	be called with nullptr pointers. Event handlers must check all incoming pointers for nullptr before
	dereferencing them unless otherwise stated.

	The RDK event 'watcher' is designed to be a replacement for the RDK event 'sink' system.
	Currently both systems are used at the same time, although event sinks are deprecated and never called.
	New clients should use event watchers instead of event sinks.
*/
class RHRDK_SDK CRhRdkEventWatcher
{
public:
	CRhRdkEventWatcher(bool bAutoRegister=true);
	virtual ~CRhRdkEventWatcher();

	virtual void Enable(bool bEnable) const;
	virtual bool IsEnabled(void) const;

	/** Called approximately every 300 milliseconds. */
	virtual void OnLazyTimer(void);

	/** Called after a new document has been created or loaded. */
	virtual void OnNewRdkDocument(const CRhRdkDocument&);

	/** Values for OnRenderingSettingsChanged 'info' parameter. */
	enum rendering_info
	{
		ri_save_support_files    = 1,  // OBSOLETE; not used.
		ri_dithering             = 2,  // Dithering method changed.
		ri_gamma                 = 3,  // OBSOLETE; see ri_gamma_linear_workflow.
		ri_use_linear_workflow   = 4,  // OBSOLETE; see ri_gamma_linear_workflow.
		ri_tone_mapping          = 5,  // OBSOLETE; Tone Mapping is now a post effect plug-in.
		ri_tone_mapper_params    = 6,  // OBSOLETE; Tone Mapping is now a post effect plug-in.
		ri_custom_reflective_env = 7,  // Custom reflective environment changed.
		ri_rhino_settings        = 8,  // Rhino settings changed (non-RDK ON_3dmRenderSettings or its user data).
		ri_render_channels       = 9,  // Render Channels settings changed.
		ri_safe_frame            = 10, // Safe Frame settings changed.
		ri_ground_plane          = 11, // Ground Plane settings changed.
		ri_skylight              = 12, // Skylight settings changed.
		ri_sun                   = 13, // Sun settings changed.
		ri_background_env        = 14, // Background environment changed.
		ri_custom_skylight_env   = 15, // Skylight environment changed.
		ri_gamma_linear_workflow = 16, // Any gamma or linear workflow setting changed.
		ri_post_effects          = 17, // Post effects changed.

		ri_force32bit = 0xFFFFFFFF
	};

	/** Called after the document rendering settings have been changed.
		\param rdkDoc is the document whose settings were changed.
		\param info indicates what was changed.
		\param pSender is for internal use only. */
	virtual void OnRenderingSettingsChanged(const CRhRdkDocument& rdkDoc, rendering_info info, const void* pSender);

	/** Called after the document sun settings have been changed.
		\param rdkDoc is the document whose sun was changed.
		\param newSun is an interface which provides the settings after the change.
		\param oldSun is an interface which provides the settings before the change.
		\param pSender is for internal use only. */
	virtual void OnSunSettingsChanged(const CRhRdkDocument& rdkDoc,
	                                  const IRhRdkSun& newSun, const IRhRdkSun& oldSun,
	                                  const void* pSender);

	/** Called after the document skylight settings have been changed.
		\param rdkDoc is the document whose skylight was changed.
		\param newSun is an interface which provides the settings after the change.
		\param oldSun is an interface which provides the settings before the change.
		\param pSender is for internal use only. */
	virtual void OnSkylightSettingsChanged(const CRhRdkDocument& rdkDoc,
	                                       const IRhRdkSkylight& newSun, const IRhRdkSkylight& oldSun,
	                                       const void* pSender);

	/** Called after the document safe frame settings have been changed.
		\param rdkDoc is the document whose safe frame was changed.
		\param newSF is an interface which provides the settings after the change.
		\param oldSF is an interface which provides the settings before the change.
		\param pSender is for internal use only. */
	virtual void OnSafeFrameSettingsChanged(const CRhRdkDocument& rdkDoc,
	                                        const IRhRdkSafeFrame& newSF, const IRhRdkSafeFrame& oldSF,
	                                        const void* pSender);

	/** Called after the document ground plane settings have been changed.
		\param rdkDoc is the document whose ground plane was changed.
		\param newGP is an interface which provides the settings after the change.
		\param oldGP is an interface which provides the settings before the change.
		\param pSender is for internal use only. */
	virtual void OnGroundPlaneSettingsChanged(const CRhRdkDocument& rdkDoc,
	                                          const IRhRdkGroundPlane& newGP, const IRhRdkGroundPlane& oldGP,
	                                          const void* pSender);

	/** Called after the document post effect settings have been changed.
		\param rdkDoc is the document whose post effects were changed.
		\param pSender is for internal use only. */
	virtual void OnPostEffectSettingsChanged(const CRhRdkDocument& rdkDoc, const void* pSender);

	/** Called after undo or redo has occurred for document settings.
		\param rdkDoc is the document that changed.
		\param bRedo is \e false for undo or \e true for redo. */
	virtual void OnUndoRedo(const CRhRdkDocument& rdkDoc, bool bRedo);

	/** Called when a display update bracket begins.
		\see CRhRdkDisplayUpdate */
	virtual void OnDisplayUpdateBegin(void);

	/** Called when a display update bracket has ended.
		\see CRhRdkDisplayUpdate */
	virtual void OnDisplayUpdateEnded(void);

	/** OBSOLETE - not called. */
	virtual void OnMenuChecksDirty(void);

	/** Called when an undo or redo operation begins. */
	virtual void OnUndoRedoBegin(CRhinoDoc& doc);

	/** Called when an undo or redo operation has ended. */
	virtual void OnUndoRedoEnded(CRhinoDoc& doc);

	/** Called after any RDK global settings have been changed. */
	virtual void OnGlobalSettingsChanged(void);

	/** Called after any RDK user data has been transformed
		This can be used to capture decal transformation. */
	virtual void OnUserDataTransformed(const CRhinoObject& obj);

	/** Called when all content previews need to be updated. */
	virtual void OnUpdateAllPreviews(const CRhRdkDocument& rdkDoc);

	/** Called when an image in the thumbnail cache is changed. */
	virtual void OnCacheImageChanged(void);

	/** Called when the number of selected Rhino objects changes. */
	virtual void OnSelectionChanged(const CRhRdkDocument& rdkDoc);

	/** Called when a client plug-in is about to be unloaded.
		\param uuidPlugIn is the plug-in id of the client's RDK plug-in. */
	virtual void OnClientPlugInUnloading(const UUID& uuidPlugIn);

	// Content events.

	/** Called before a content's name is changed. Called for all contents, document or not.
		\param c is the content that is being renamed. */
	virtual void OnContentRenaming(const CRhRdkContent& c);

	/** Called after a content's name has been changed. Called for all contents, document or not.
		\param c is the content that was renamed. */
	virtual void OnContentRenamed(const CRhRdkContent& c);

	/** Called before a content's notes are changed. Called for all contents, document or not.
		\param c is the content whose notes are changing. */
	virtual void OnContentNotesChanging(const CRhRdkContent& c);

	/** Called after a content's notes have been changed. Called for all contents, document or not.
		\param c is the content whose notes were changed. */
	virtual void OnContentNotesChanged(const CRhRdkContent& c);

	/** Called before a content's tags are changed. Called for all contents, document or not.
		\param c is the content whose tags are changing. */
	virtual void OnContentTagsChanging(const CRhRdkContent& c);

	/** Called after a content's tags have been changed. Called for all contents, document or not.
		\param c is the content whose tags were changed. */
	virtual void OnContentTagsChanged(const CRhRdkContent& c);

	/** Called when one or more contents are selected in an editor.
		\param aContentSel is the array of contents being selected.
		\param aContentEdit is the array of contents that can actually be edited. Some contents in the
		 selection may not be editable because of modal editing restrictions or because they are
		 surrogates or reference contents.
		\param uuidEditor is the id of the editor the content is being selected in. */
	virtual void OnContentSelection(const CRhRdkContentArray& aContentSel,
	                                const CRhRdkContentArray& aContentEdit, const UUID& uuidEditor);

	/** Reason content is being attached to a document.
		\see OnContentAttached, \see CRhRdkDocument::AttachContent() */
	enum class AttachReason : unsigned int
	{
		Attach, // Content is being attached by the CRhRdkDocument::AttachContent() or CRhRdkContent::SetChild() methods, or to a decal.
		Change, // Content is being attached while it is replacing another one.
		Undo  , // Content is being attached during undo/redo.
		Open  , // Content is being attached during open document.
		Edit  , // Content is being attached during modal editing.
		DCM   , // Content is being attached by the default content manager.
	};

	/** Called just after a content is attached to a document or a document decal.
		\param rdkDoc is the RDK document the content was attached to, or the document the decal is attached to.
		\param c is the content that was attached.
		\param r is the reason the content was attached. */
	virtual void OnContentAttached(const CRhRdkDocument& rdkDoc, const CRhRdkContent& c, AttachReason r);

	/** Reason content is being detached from a document or a document decal.
		\see OnContentDetaching, \see CRhRdkDocument::DetachContent(), \see CRhRdkContent::Delete() */
	enum class DetachReason : unsigned int
	{
		Detach, // Content is being detached by the CRhRdkDocument::DetachContent() method, or from a decal.
		Change, // Content is being detached while it is being replaced.
		Undo  , // Content is being detached during undo/redo.
		Delete, // Content is being detached during normal deletion.
		Edit  , // Content is being detached during modal editing.
	};

	/** Called just before a content is detached from a document or a document decal.
		\param rdkDoc is the RDK document the content is about to be detached from or the document the decal is attached to.
		\param c is the content that is being detached.
		\param r is the reason the content is being detached. */
	virtual void OnContentDetaching(const CRhRdkDocument& rdkDoc, const CRhRdkContent& c, DetachReason r);

	/** Called just after a content is detached from a document or a document decal.
		\param rdkDoc is the RDK document the content was detached from or the document the decal is attached to.
		\param c is the content that was detached.
		\param r is the reason the content was detached. */
	virtual void OnContentDetached(const CRhRdkDocument& rdkDoc, const CRhRdkContent& c, DetachReason r);

	/** Used by the OnContentAutoDeletion event. */
	enum class AutoDeletionStateChange : unsigned int { Enter, Leave }; 

	/** Called when a content enters or leaves the auto-deletion state.
		\note This only applies to content that is attached to a document.
		\param rdkDoc is the RDK document the content is attached to.
		\param c is the content.
		\param change is the auto-deletion state change.
		 A content enters the auto-deletion state when its auto-delete flag is
		 true and its use count is lowered from 1 to 0. When in this state, the content
		 is ignored by content UI that would usually display a representation of the content.
		 A content leaves the auto-deletion state when its auto-delete flag is
		 true and its use count is raised from 0 to 1. In this state, the content
		 is displayed by content UI in the usual way. */
	virtual void OnContentAutoDeletion(const CRhRdkDocument& rdkDoc, const CRhRdkContent& c, AutoDeletionStateChange change);

	/** Called after content is attached to a document by a user action such as Create New, Paste, Duplicate etc.
		\param rdkDoc is the RDK document the content was attached to.
		\param aContent is an array of content that was added.
		\param bNameWasLoaded is \e true if content's name was loaded from a file, otherwise \e false. */
	virtual void OnContentAddedByUser(const CRhRdkDocument& rdkDoc, const CRhRdkContentArray& aContent, bool bNameWasLoaded);

	/** Called just before a content is replaced in the document. Replacing a content involves several
		operations: 1. Detaching the old content, 2. Deleting the old content, 3. Attaching the new content.
		Together, the OnContentReplacing() and OnContentReplaced() events bracket this operation enabling
		clients to determine when the sequence begins and ends.
		\param oldContent is the content that is about to be replaced. */
	virtual void OnContentReplacing(const CRhRdkDocument& rdkDoc, const CRhRdkContent& oldContent);

	/** Called just after a content is replaced in the document. Replacing a content involves several
		operations: 1. Detaching the old content, 2. Deleting the old content, 3. Attaching the new content.
		Together, the OnContentReplacing() and OnContentReplaced() events bracket this operation enabling
		clients to determine when the sequence begins and ends.
		\param newContent is the content that has replaced the old content. */
	virtual void OnContentReplaced(const CRhRdkDocument& rdkDoc, const CRhRdkContent& newContent);

	/** Called when a content is changed. Called for all content changes, document or not.
		\param c is the content that changed.
		\param old is the old content. This is a temporary object with a different instance id to the original.
		\param cc is the context of the change. */
	virtual void OnContentChanged(const CRhRdkContent& c, const CRhRdkContent& old, RhRdkChangeContext cc);

	/** Called when a content preview needs to be updated.
		\param c is the content whose preview needs to be updated. */
	virtual void OnContentUpdatePreview(const CRhRdkContent& c);

	/** Called just before a persistent content list is cleared.
		\param rdkDoc is the RDK document the list exists in.
		\param list is the list being cleared. */
	virtual void OnContentListClearing(const CRhRdkDocument& rdkDoc, const IRhRdkContentList& list);

	/** Called just after a persistent content list has been cleared.
		\param rdkDoc is the RDK document that the list exists in.
		\param list is the list that was cleared. */
	virtual void OnContentListCleared(const CRhRdkDocument& rdkDoc, const IRhRdkContentList& list);

	/** Called just after a persistent content list has been loaded (e.g., from a .3dm file).
		\param rdkDoc is the RDK document the list exists in.
		\param list is the list that was loaded. */
	virtual void OnContentListLoaded(const CRhRdkDocument& rdkDoc, const IRhRdkContentList& list);

	/** This event is used internally by RDK. Do not override this method. */
	virtual void OnContentUpdateMarkers(const CRhRdkDocument& rdkDoc, CRhRdkContent::Kinds kind);

	/** Called when a 'current environment' is changed. There are three current environment 'usages'
		which are marked in the thumbnail editor with small triangles. This event is sent when a new
		environment is made current or when a current environment is deleted so that there is no current
		environment for that usage.
		\see CRhRdkEnvironment::eUsage.
		\param rdkDoc is the document that the content is attached to.
		\param usage is the environment usage that was changed.
		\param pContent is a pointer to the content. This pointer will be null if no environment is current for the specified usage. */
	virtual void OnCurrentEnvironmentChanged(const CRhRdkDocument& rdkDoc, IRhRdkCurrentEnvironment::Usage usage, const CRhRdkContent* pContent);

	/** Called when the content filter dialog is dismissed with the OK button. */
	virtual void OnContentFilterChanged(const CRhRdkDocument& rdkDoc, const CRhRdkUuidCollection& excludedRenderEngines);

	/** Called when a V4 material is about to become an RDK material.
		\param v4Material is the V4 material. */
	virtual void OnContentBlossomBegin(const CRhRdkBasicMaterial& v4Material);

	/** Called after a V4 material has become an RDK material.
		\param pBlossom is a pointer to the RDK material, or nullptr if failed. */
	virtual void OnContentBlossomEnd(const CRhRdkBasicMaterial* pBlossom);

	/** Called when a content's group id changes. The group id controls 'instancing'.
		Contents with the same group id stay in synch with each other's settings.
		\param c is the content.
		\param uuidGroupNew is the new group id.
		\param uuidGroupOld is the old group id. */
	virtual void OnContentGroupIdChanged(const CRhRdkContent& c, const UUID& uuidGroupNew, const UUID& uuidGroupOld);

	// Factory events.

	/** Called when a content factory is added to the factories collection. */
	virtual void OnFactoryAdded(const CRhRdkContentFactory& factory);

	/** Called after all content factories, CCIs, PEPs, registered properties,
		custom render meshes, content I/O plug-ins and custom plug-ins have been added. */
	virtual void OnAllFactoriesAdded(void);

	/** Called when a content factory is about to be deleted. */
	virtual void OnFactoryDeleting(const CRhRdkContentFactory& factory);

	/** Called when a content factory has been deleted. */
	virtual void OnFactoryDeleted(CRhRdkContent::Kinds kind);

	// Renderer events.

	/** Called when Rhino's current renderer has changed. */
	virtual void OnRendererChanged(void);

	/** Called when certain Rhino settings are changed through an RDK document.
		\see IRhRdkRhinoSettings */
	virtual void OnRhinoSettingsChanged(const CRhRdkDocument& rdkDoc);

	/** This event is deprecated in favor of the one below. THIS EVENT IS NO LONGER SENT. */
	RDK_DEPRECATED virtual void OnRenderingFinished(bool bSuccessful);

	/** Called when rendering finishes.
		\param bSuccess is \e true if rendering completed sucessfully, else \e false. */
	virtual void OnRenderingFinished(const IRhRdkRenderSession& session, bool bSuccess);

	/** Called when a render engine becomes busy or idle during preview updates. */
	virtual void OnRenderEngineStateChanged(bool bBusy);

	// Texture mapping events.
	virtual void TextureMappingTableEvent(const CRhRdkDocument& rdkDoc, const ON_TextureMapping& tm);
	virtual void OnModifyObjectTextureMapping(const CRhRdkDocument& rdkDoc, const CRhinoObject& obj);

	// Custom render mesh events.

	/** Called when a custom render mesh is changed. */

	enum class eCRMChanged
	{
		kSingleObject					= 0, // the CRMs on a single object have been changed.
		kWholeDocument					= 1, // the entire set of CRMs for this document should be considered invalid.
		kDocumentRenderMeshes			= 2, // only the CRMs that are document specific (ie -the object pointer was NULL) have changed.
	};

	virtual void OnCustomRenderMeshesChanged(const CRhRdkDocument& rdkDoc, const CRhinoObject* pObject, eCRMChanged crmc);

	// Assignments.

	/** Called when an object's material assignment has changed.
		\param doc is the document.
		\param obj is the Rhino object in the document.
		\param uuidInstanceNew is the new RDK material assignment. May be nil.
		\param uuidInstanceOld is the old RDK material assignment. May be nil. */
	virtual void OnObjectMaterialAssignmentChanged(const CRhinoDoc& doc, const CRhinoObject& obj,
	                                               const UUID& uuidInstanceNew, const UUID& uuidInstanceOld);

	/** Called when a layer's material assignment has changed.
		\param doc is the document.
		\param layer is the Rhino layer in the document layer table.
		\param uuidInstanceNew is the new RDK material assignment. May be nil.
		\param uuidInstanceOld is the old RDK material assignment. May be nil. */
	virtual void OnLayerMaterialAssignmentChanged(const CRhinoDoc& doc, const CRhinoLayer& layer,
	                                              const UUID& uuidInstanceNew, const UUID& uuidInstanceOld);

	// Decal events.
	virtual void OnDecalAdded(const CRhinoObject& obj, const IRhRdkDecal& decal);
	virtual void OnDecalDeleted(const CRhinoObject& obj, RDK_DECAL_CRC old_decal_crc);
	virtual void OnDecalModified(const CRhinoObject& obj, const IRhRdkDecal& decal, RDK_DECAL_CRC old_decal_crc);

	// Rendering file access.

	/** Called when a rendering has been loaded. These are stored in .rimage files.
		\param info contains information about the file that was loaded. */
	virtual void OnRenderingLoaded(const IRhRdkRenderingFileInfo& info);

	/** Called when a rendering has been saved. These are stored in .rimage files.
		\param info contains information about the file that was saved. */
	virtual void OnRenderingSaved(const IRhRdkRenderingFileInfo& info);

	/** Called when a rendering has been deleted. These are stored in .rimage files.
		\param info contains information about the file that was deleted. */
	virtual void OnRenderingDeleted(const IRhRdkRenderingFileInfo& info);

	/** Called when a render window has been cloned.
		\param uuidRenderSessionOld is the render session id of the original render window.
		\param uuidRenderSessionNew is the render session id of the cloned render window. */
	virtual void OnRenderWindowCloned(const UUID& uuidRenderSessionOld, const UUID& uuidRenderSessionNew);

	/** Called when a preview has been rendered. If you are implementing a UI which is interested
		in previews, you can check the quality and signature and optionally copy the dib for use in your UI.
		\param dib is the rendered preview.
		\param qual is the quality of the rendered preview. 
		\param sig is the job signature of the rendered preview. */
	virtual void OnPreviewRendered(const CRhinoDib& dib, RhRdkPreviewQuality qual, const CRhRdkPreviewJobSignature& sig);

	/** \internal For RDK internal use only. */
	virtual void OnInternalProxyUpdate(const CRhRdkInstanceIdArray&, UINT_PTR) { }

	/** \internal For RDK internal use only. */
	virtual void OnUpdateThumbDropTL(const CRhRdkDocument&) { }

	// Future expansion.

	/** Custom event. */
	virtual void OnCustomEvent(const UUID& uuidCode, void* pvContext);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

	// ################################################################################
	//
	// These obsolete event sink overrides have been retained to provide deprecated warnings.
	// If you get a warning for one of these methods, you need to change your override to
	// match the new function prototype above.
	//
	RDK_DEPRECATED virtual void OnNewRdkDocument(void) { }
	RDK_DEPRECATED virtual void OnUpdateAllPreviews(void) { }
	RDK_DEPRECATED virtual void OnSelectionChanged(CRhinoDoc&) { }
	RDK_DEPRECATED virtual void OnAutomaticParamChanged(HWND, DWORD_PTR) { } // This is obsolete.
	RDK_DEPRECATED virtual void OnContentMoved(const CRhRdkContent*, const CRhRdkContent*, const IRhRdkContentEditor*) { }
	RDK_DEPRECATED virtual void OnContentRenamed(const CRhRdkContent*) { }
	RDK_DEPRECATED virtual void OnContentSelected(const CRhRdkContent*, const IRhRdkContentEditor&) { }
	RDK_DEPRECATED virtual void OnContentReplacing(const CRhRdkContent*) { }
	RDK_DEPRECATED virtual void OnContentReplaced(CRhRdkContent*) { }
	RDK_DEPRECATED virtual void OnContentChanged(const CRhRdkContent*, RhRdkChangeContext) { }
	RDK_DEPRECATED virtual void OnContentUpdatePreview(const CRhRdkContent*) { }
	RDK_DEPRECATED virtual void OnContentListClearing(const IRhRdkContentList&) { }
	RDK_DEPRECATED virtual void OnContentListCleared(const IRhRdkContentList&) { }
	RDK_DEPRECATED virtual void OnContentListLoaded(const IRhRdkContentList&) { }
	RDK_DEPRECATED virtual void OnContentUpdateMarkers(const CRhRdkContent*) { }
	RDK_DEPRECATED virtual void OnContentCurrencyChanged(const wchar_t*, const CRhRdkContent*) { }
	RDK_DEPRECATED virtual void OnContentEditorExtended(bool) { }
	RDK_DEPRECATED virtual void OnContentFilterChanged(const CRhRdkUuidCollection&) { }
	RDK_DEPRECATED virtual void OnContentEditorActivated(const IRhRdkContentEditor&) { }
	RDK_DEPRECATED virtual void OnAddRhinoObject(CRhinoDoc&, CRhinoObject&) { }
	RDK_DEPRECATED virtual void OnDeleteRhinoObject(CRhinoDoc&, CRhinoObject&) { }
	RDK_DEPRECATED virtual void OnReplaceRhinoObject(CRhinoDoc&, CRhinoObject&, CRhinoObject&) { }
	RDK_DEPRECATED virtual void TextureMappingTableEvent(const ON_TextureMapping&) { }
	RDK_DEPRECATED virtual void OnModifyObjectTextureMapping(CRhinoDoc&, const CRhinoObject&) { }
	RDK_DEPRECATED virtual void OnContentBlossomEnd(const CRhRdkBasicMaterial&) { }

	/** If you were watching for OnContentAdded you now probably want to watch for OnContentAttached instead. */
	RDK_DEPRECATED virtual void OnContentAdded(const CRhRdkContent*) { }

	/** If you were watching for OnContentDeleting you now probably want to watch for OnContentDetaching instead. */
	RDK_DEPRECATED virtual void OnContentDeleting(const CRhRdkContent*) { }

	/** If you were watching for OnContentDeleted you now probably want to watch for OnContentDetached instead. */
	RDK_DEPRECATED virtual void OnContentDeleted(void) { }

	/** OnDocumentSettingsChanged has been replaced by individual events:

		OnRenderingSettingsChanged    replaces CRhRdkEventSinkBase::rendering
		OnSunSettingsChanged          replaces CRhRdkEventSinkBase::document_sun
		OnSafeFrameSettingsChanged    replaces CRhRdkEventSinkBase::safe_frame
		OnGroundPlaneSettingsChanged  replaces CRhRdkEventSinkBase::ground_plane
		OnPostEffectSettingsChanged   replaces CRhRdkEventSinkBase::post_effects
		OnContentFilterChanged        replaces CRhRdkEventSinkBase::content_filter
		OnCustomRenderMeshesChanged   replaces CRhRdkEventSinkBase::custom_render_mesh
		OnUndoRedo                    replaces CRhRdkEventSinkBase::all
	*/
	RDK_DEPRECATED virtual void OnDocumentSettingsChanged(UINT, UINT_PTR) { }

	// ################################################################################

private:
	class CImpl;
	CImpl* m_pImpl;
};
