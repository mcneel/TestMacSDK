
#pragma once // Public SDK header

#include "RhRdkContent.h"

class IRhRdkContentEditor;
class IRhRdkContentFactory;
class IRhRdkContentList;
class CRhRdkUuidCollection;

/** \class CRhRdkEventSinkBase

	\deprecated OBSOLETE - \see CRhRdkEventWatcher
*/

class RHRDK_SDK CRhRdkEventSinkBase
{
public:
	CRhRdkEventSinkBase();
	virtual ~CRhRdkEventSinkBase();

	enum
	{
		rendering          = 0x0001,
		safe_frame         = 0x0002,
		document_sun       = 0x0004,
		post_effects       = 0x0008,
		ground_plane       = 0x0010,
		content_filter     = 0x0020,
		custom_render_mesh = 0x0040,
		unspecified        = 0x8000,
		all                = 0xFFFF,
		force32bit_flags1 = 0xFFFFFFFF
	};

	enum rendering_info
	{
		save_support_files  = 1,
		dithering           = 2,
		gamma               = 3,
		use_linear_workflow = 4,
		tone_mapping        = 5,
		tone_mapper_params  = 6,
		reflective_refractive_environment = 7,
		force32bit_flags2 = 0xFFFFFFFF
	};

	virtual void OnLazyTimer(void) { }
	virtual void OnNewRdkDocument(void) { }
	virtual void OnDocumentSettingsChanged(UINT, UINT_PTR) { }
	virtual void OnGlobalSettingsChanged(void) { }
	virtual void OnUpdateAllPreviews(void) { }
	virtual void OnCacheImageChanged(void) { }
	virtual void OnSelectionChanged(CRhinoDoc&) { }
	virtual void OnClientPlugInUnloading(const UUID&) { }
	virtual void OnContentAdded(const CRhRdkContent*) { }
	virtual void OnContentMoved(const CRhRdkContent*, const CRhRdkContent*, const IRhRdkContentEditor*) { }
	virtual void OnContentRenamed(const CRhRdkContent*) { }
	virtual void OnContentSelected(const CRhRdkContent*, const IRhRdkContentEditor&) { }
	virtual void OnContentDeleting(const CRhRdkContent*) { }
	virtual void OnContentDeleted(void) { }
	virtual void OnContentReplacing(const CRhRdkContent*) { }
	virtual void OnContentReplaced(CRhRdkContent*) { }
	virtual void OnContentChanged(const CRhRdkContent*, RhRdkChangeContext) { }
	virtual void OnContentUpdatePreview(const CRhRdkContent*) { }
	virtual void OnContentListClearing(const IRhRdkContentList&) { }
	virtual void OnContentListCleared(const IRhRdkContentList&) { }
	virtual void OnContentListLoaded(const IRhRdkContentList&) { }
	virtual void OnContentUpdateMarkers(const CRhRdkContent*) { }
	virtual void OnContentCurrencyChanged(const wchar_t*, const CRhRdkContent*) { }
	virtual void OnContentEditorExtended(bool) { }
	virtual void OnContentFilterChanged(const CRhRdkUuidCollection&) { }
	virtual void OnContentEditorActivated(const IRhRdkContentEditor&) { }
	virtual void OnFactoryAdded(const IRhRdkContentFactory&) { }
	virtual void OnFactoryDeleting(const IRhRdkContentFactory&) { }
	virtual void OnFactoryDeleted(const wchar_t*) { }
	virtual void OnAllFactoriesAdded(void) { }
	virtual void OnRendererChanged(void) { }
	virtual void OnRenderEngineStateChanged(bool) { }
	virtual void OnAddRhinoObject(CRhinoDoc&, CRhinoObject&) { }
	virtual void OnDeleteRhinoObject(CRhinoDoc&, CRhinoObject&) { }
	virtual void OnReplaceRhinoObject(CRhinoDoc&, CRhinoObject&, CRhinoObject&) { }
	virtual void TextureMappingTableEvent(const ON_TextureMapping&) { }
	virtual void OnModifyObjectTextureMapping(CRhinoDoc&, const CRhinoObject&) { }
	virtual void OnCustomRenderMeshesChanged(void) { }
	virtual void OnCustomEvent(const UUID&, void*) { }
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);
};

#pragma warning(push)
#pragma warning(disable : 4996 ) // Disable warning here; clients will still get a warning if they try to use the class.

/** \class CRhRdkEventSink

	\deprecated OBSOLETE - Please use CRhRdkEventWatcher instead
	\see CRhRdkEventWatcher
*/
class RHRDK_SDK ON_DEPRECATED CRhRdkEventSink : protected CRhRdkEventSinkBase
{
public:
	CRhRdkEventSink();
	virtual ~CRhRdkEventSink();

	virtual void Enable(bool bEnable) const;
	virtual bool IsEnabled(void) const;
};

#pragma warning(pop)

/** \class CRhRdkContentAdded

	\deprecated OBSOLETE
	\see CRhRdkEventWatcher::OnContentAttached()
*/
class RHRDK_SDK CRhRdkContentAdded final
{
public:
	CRhRdkContentAdded(const CRhRdkContent& c, int, bool, void*);

	const CRhRdkContent& Content(void) const { return m_Content; }
	RDK_DEPRECATED int EditingLevel(void) const { return m_iEditingLevel; }
	RDK_DEPRECATED bool CookedUp(void) const { return m_bCookedUp; }

private:
	const CRhRdkContent& m_Content;
	int m_iEditingLevel;
	bool m_bCookedUp;
	void* m_pReserved;
};

/** \class CRhRdkContentAddedByUser

	\deprecated OBSOLETE
	\see CRhRdkEventWatcher::OnContentAddedByUser()
*/
class RHRDK_SDK CRhRdkContentAddedByUser final
{
public:
	CRhRdkContentAddedByUser(const CRhRdkContentArray&, int, bool bNameWasGenerated, void*);

	const CRhRdkContentArray& Contents(void) const { return m_aContent; }
	RDK_DEPRECATED int EditingLevel(void) const { return m_iEditingLevel; }
	bool NameWasGenerated(void) const { return m_bNameWasGenerated; }

private:
	const CRhRdkContentArray& m_aContent;
	int m_iEditingLevel;
	bool m_bNameWasGenerated;
	void* m_pReserved;
};

/** \class CRhRdkContentMoved

	\deprecated OBSOLETE
*/
class RHRDK_SDK CRhRdkContentMoved final
{
public:
	CRhRdkContentMoved(const CRhRdkContentArray& aContentMoved, const CRhRdkContent* pBefore, HWND hWnd, const UUID& e, void* p);

	const CRhRdkContentArray& ContentMoved(void) const { return m_aContentMoved; }
	const CRhRdkContent* ContentBefore(void) const { return m_pContentBefore; }
	UUID EditorId(void) const { return m_uuidEditor; }
	HWND Window(void) const { return m_hWnd; }

private:
	HWND m_hWnd;
	UUID m_uuidEditor;
	const CRhRdkContent* m_pContentBefore;
	const CRhRdkContentArray& m_aContentMoved;
	void* m_pReserved;
};
