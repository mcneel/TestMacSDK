
#pragma once // SDK header

#include "RhRdkTask.h"

class CRhRdkContentArray;
class CRhRdkDocument;
class CRhRdkPreviewAppearance;
class CRhRdkPreviewMetaData;

/** \class CRhRdkTaskOrigin

*/
class RHRDK_UI_SDK CRhRdkTaskOrigin : public CRhRdkObject
{
public:
	CRhRdkTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con);
	virtual ~CRhRdkTaskOrigin();

	/** Get the contents that have been selected by the user.
		\param aContentOut accepts the selected contents.
		\param bForModify indicates if the operation is planning to modify the contents. */
	void GetSelection(CRhRdkContentArray& aContentOut, bool bForModify=false) const;

	CRhRdkControllerPtr Controller(void) const;

	//-----------------------------------------------
	virtual CRhinoDoc* RhinoDoc(void) const;
	virtual CRhRdkDocument* RdkDocument(void) const;
	//-----------------------------------------------

	virtual int GetSelectedCount(void) const;

	virtual bool SupportsTask(const CRhRdkTask& t) const;

	/** A task origin is 'ambivalent' when it wants to show tasks for both top-level and
		child contents on the same menu. This is a special quirk of the spanner menu. */
	virtual bool IsAmbivalent(void) const;

	virtual IRhRdkTaskCollection::Control Control(void) const = 0;

	/** TODO: Docs */
	virtual bool WillReplaceContent(void) const;

	/** TODO: Docs */
	virtual UUID Uuid(void) const = 0;

	/** TODO: Docs */
	virtual const wchar_t* TopLevelKind(void) const = 0;

	/** Implement this to get the contents that have been selected by the user.
		\param aContentOut accepts the selected contents. */
	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const = 0;

	/** TODO: Docs */
	virtual const CRhRdkPreviewAppearance* PreviewAppearance(void) const;

	/** TODO: Docs */
	virtual const CRhRdkPreviewMetaData* PreviewMetaData(void) const;

public: // Callbacks.
	virtual void OnBeginAskUserSure(const UUID& uuidTask) const;
	virtual void OnEndAskUserSure  (const UUID& uuidTask, RhRdkMbResult) const;
	virtual void OnBeginExecuteTask(const UUID& uuidTask) const;
	virtual void OnEndExecuteTask  (const UUID& uuidTask, CRhRdkTask::Result) const;

public:
	/** TODO: Docs INTERNAL */
	virtual int GetState(void) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkContextTaskOrigin

	Used when task origin is context-sensitive.

*/
class RHRDK_UI_SDK CRhRdkContextTaskOrigin : public CRhRdkTaskOrigin
{
	typedef CRhRdkTaskOrigin _super;
public:
	CRhRdkContextTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con);

	// All context-sensitive task origins have the following quirk: if a task is for top-level content,
	// it's only supported if all the selected contents are top-level contents.
	virtual bool SupportsTask(const CRhRdkTask& task) const override;
};

/** \class CRhRdkContextTaskOriginWithCEC

	Used when task origin is context-sensitive and has a CEC.

*/
class RHRDK_UI_SDK CRhRdkContextTaskOriginWithCEC : public CRhRdkContextTaskOrigin
{
	typedef CRhRdkContextTaskOrigin _super;
public:
	CRhRdkContextTaskOriginWithCEC(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const IRhRdkContentEditingContext& cec);
	virtual ~CRhRdkContextTaskOriginWithCEC();

	virtual UUID Uuid(void) const final override;
	virtual const wchar_t* TopLevelKind(void) const final override;
	virtual const CRhRdkPreviewMetaData* PreviewMetaData(void) const final override;
	virtual const CRhRdkPreviewAppearance* PreviewAppearance(void) const final override;

protected:
	const IRhRdkContentEditingContext& CEC(void) const;

protected:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkThumbnailTaskOrigin

	Used when the task origin is a thumbnail list.

*/
class RHRDK_UI_SDK CRhRdkThumbnailTaskOrigin : public CRhRdkContextTaskOriginWithCEC
{
	typedef CRhRdkContextTaskOriginWithCEC _super;
public:
	CRhRdkThumbnailTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const IRhRdkContentEditingContext& cec);

	virtual IRhRdkTaskCollection::Control Control(void) const final override { return IRhRdkTaskCollection::Control::MainThumb; }

protected:
	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const final override;
};

/** \class CRhRdkThumbnailSelTaskOrigin

	Used when the task origin is a thumbnail list and the selection can be replaced by a task.

*/
class RHRDK_UI_SDK CRhRdkThumbnailSelTaskOrigin : public CRhRdkThumbnailTaskOrigin
{
	typedef CRhRdkThumbnailTaskOrigin _super;
public:
	CRhRdkThumbnailSelTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const IRhRdkContentEditingContext& cec);

	virtual bool WillReplaceContent(void) const final override;
};

/** \class CRhRdkContentTreeTaskOrigin

	Used when the task origin is a content tree.

*/
class RHRDK_UI_SDK CRhRdkContentTreeTaskOrigin : public CRhRdkContextTaskOriginWithCEC
{
	typedef CRhRdkContextTaskOriginWithCEC _super;
public:
	CRhRdkContentTreeTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const IRhRdkContentEditingContext& cec)
		: CRhRdkContextTaskOriginWithCEC(rdkDoc, con, cec) { }

	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const final override;
	virtual IRhRdkTaskCollection::Control Control(void) const final override { return IRhRdkTaskCollection::Control::MainTree; }
};

/** \class CRhRdkContentTreeSelTaskOrigin

	Used when the task origin is a content tree and the selection can be replaced by a task.

*/
class RHRDK_UI_SDK CRhRdkContentTreeSelTaskOrigin : public CRhRdkContentTreeTaskOrigin
{
	typedef CRhRdkContentTreeTaskOrigin _super;
public:
	CRhRdkContentTreeSelTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const IRhRdkContentEditingContext& cec)
		: CRhRdkContentTreeTaskOrigin(rdkDoc, con, cec) { }

	virtual bool WillReplaceContent(void) const final override { return true; }
};

/** \class CRhRdkSingleSelectionTaskOrigin

	Used when task origin is something with a single selected content.
	This is typically used by the floating preview and editor preview thumbnails.
	Its special quirk is that it makes menus work as if the content is not part
	of a hierarchy.

*/
class RHRDK_UI_SDK CRhRdkSingleSelectionTaskOrigin : public CRhRdkContextTaskOrigin
{
	typedef CRhRdkContextTaskOrigin _super;
public:
	CRhRdkSingleSelectionTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance, const UUID& uuidEditor);
	virtual ~CRhRdkSingleSelectionTaskOrigin();

	virtual UUID Uuid(void) const final override;
	virtual const wchar_t* TopLevelKind(void) const final override;
	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const final override;
	virtual const CRhRdkPreviewAppearance* PreviewAppearance(void) const final override;
	virtual bool SupportsTask(const CRhRdkTask& task) const override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkContentTypeBrowserTaskOrigin

	Used when the task origin is a content type browser thumbnail.

*/
class RHRDK_UI_SDK CRhRdkContentTypeBrowserTaskOrigin : public CRhRdkSingleSelectionTaskOrigin
{
	typedef CRhRdkSingleSelectionTaskOrigin _super;
public:
	CRhRdkContentTypeBrowserTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con,  const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance);

	virtual bool SupportsTask(const CRhRdkTask& task) const override;
};

class RHRDK_UI_SDK CRhRdkContentTypeBrowserTaskOrigin_New : public CRhRdkContentTypeBrowserTaskOrigin
{
	typedef CRhRdkContentTypeBrowserTaskOrigin _super;
public:
	CRhRdkContentTypeBrowserTaskOrigin_New(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con,  const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance);

	virtual IRhRdkTaskCollection::Control Control(void) const final override;
};

class RHRDK_UI_SDK CRhRdkContentTypeBrowserTaskOrigin_Existing : public CRhRdkContentTypeBrowserTaskOrigin
{
	typedef CRhRdkContentTypeBrowserTaskOrigin _super;
public:
	CRhRdkContentTypeBrowserTaskOrigin_Existing(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con,  const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance);
	virtual IRhRdkTaskCollection::Control Control(void) const final override;
};

class RHRDK_UI_SDK CRhRdkContentInstanceBrowserTaskOrigin : public CRhRdkSingleSelectionTaskOrigin
{
	typedef CRhRdkSingleSelectionTaskOrigin _super;
public:
	CRhRdkContentInstanceBrowserTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con,  const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance);

	virtual bool SupportsTask(const CRhRdkTask& task) const override;
	virtual IRhRdkTaskCollection::Control Control(void) const final override;
};

/** \class CRhRdkFloatingPreviewTaskOrigin

	Used when the task origin is a floating preview thumbnail.

*/
class RHRDK_UI_SDK CRhRdkFloatingPreviewTaskOrigin : public CRhRdkSingleSelectionTaskOrigin
{
	typedef CRhRdkSingleSelectionTaskOrigin _super;
public:
	CRhRdkFloatingPreviewTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance, const UUID& uuidPreview);

	virtual bool SupportsTask(const CRhRdkTask& task) const override;

	virtual IRhRdkTaskCollection::Control Control(void) const final override { return IRhRdkTaskCollection::Control::FloatingPreview; }
};

/** \class CRhRdkEditorPreviewTaskOrigin

	 Used when task origin is the editor preview thumbnail.

*/
class RHRDK_UI_SDK CRhRdkEditorPreviewTaskOrigin : public CRhRdkSingleSelectionTaskOrigin
{
	typedef CRhRdkSingleSelectionTaskOrigin _super;
public:
	CRhRdkEditorPreviewTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const UUID& uuidInstance, const CRhRdkPreviewAppearance* pAppearance, const UUID& uuidEditor);

	virtual IRhRdkTaskCollection::Control Control(void) const final override { return IRhRdkTaskCollection::Control::EditorPreview; }
};

/** \class CRhRdkSubNodeTaskOrigin

	Used when task origin is a sub-node control.

*/
class RHRDK_UI_SDK CRhRdkSubNodeTaskOrigin : public CRhRdkContextTaskOrigin
{
	typedef CRhRdkContextTaskOrigin _super;
public:
	CRhRdkSubNodeTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const CRhRdkInstanceIdArray& aInstance, class CRhRdkChildSlotHelper* pCSH, bool bLocked);
	virtual ~CRhRdkSubNodeTaskOrigin();

	bool IsForChildSlot(const wchar_t* wszChildSlotName) const;

	virtual const wchar_t* TopLevelKind(void) const override;
	virtual bool SupportsTask(const CRhRdkTask& task) const override;
	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const final override;
	virtual IRhRdkTaskCollection::Control Control(void) const override;
	virtual UUID Uuid(void) const final override;

public: // Callback overrides.
	virtual void OnBeginExecuteTask(const UUID& uuidTask) const override;
	virtual void OnEndExecuteTask(const UUID& uuidTask, CRhRdkTask::Result res) const override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkColorButtonTaskOrigin

	Used when task origin is a color button.

*/
class RHRDK_UI_SDK CRhRdkColorButtonTaskOrigin : public CRhRdkSubNodeTaskOrigin
{
	typedef CRhRdkSubNodeTaskOrigin _super;
public:
	CRhRdkColorButtonTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const CRhRdkInstanceIdArray& aInstance, class CRhRdkChildSlotHelper* pCSH);

	virtual IRhRdkTaskCollection::Control Control(void) const override;
};

/** \class CRhRdkBreadcrumbTaskOrigin

	Used when task origin is the breadcrumb control.

*/
class RHRDK_UI_SDK CRhRdkBreadcrumbTaskOrigin : public CRhRdkContextTaskOriginWithCEC
{
	typedef CRhRdkContextTaskOriginWithCEC _super;
public:
	CRhRdkBreadcrumbTaskOrigin(const CRhRdkDocument& rdkDoc, CRhRdkControllerPtr con, const IRhRdkContentEditingContext& cec, const UUID& uuidInstance);
	virtual ~CRhRdkBreadcrumbTaskOrigin();

	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const final override;
	virtual bool SupportsTask(const CRhRdkTask& task) const override;
	virtual IRhRdkTaskCollection::Control Control(void) const final override;

private:
	class CImpl;
	CImpl* m_pImpl;
};
