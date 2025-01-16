
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class IRhRdkTaskOrigin;

class RHRDK_SDK IRhRdkTaskUpdate
{
public:
	/** This method is implemented as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** Get the task UI's 'enabled' state.
		\note The default enabled state is \e true unless an implementation sets it to \e false. */
	virtual bool IsEnabled(void) const = 0;

	/** Get the task UI's 'checked' state. */
	virtual bool IsChecked(void) const = 0;

	/** Set the task UI's 'enabled' state. */
	virtual void SetIsEnabled(bool) = 0;

	/** Set the task UI's 'checked' state. Useful for menu check items. */
	virtual void SetIsChecked(bool) = 0;

	/** Set the task UI's 'radio' state. Useful for menu radio items. */
	virtual void SetRadio(bool) = 0;

	/** Get the task origin. This depends on the UI the user clicked in to
		invoke the menu or other UI that displays tasks. */
	virtual const IRhRdkTaskOrigin& Origin(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkTaskUpdate() { }
};

/** \class CRhRdkTask

	This class is the base class for all tasks. A task encapsulates the functionality
	of any operation the user can perform by clicking a menu item or pressing a key
	in a content editor.

*/
class RHRDK_SDK CRhRdkTask : public CRhRdkExtension
{
public:
	CRhRdkTask();
	virtual ~CRhRdkTask();

	/** Returns the ids of the data sources needed to make tasks work. */
	static void RequiredDataSources(OUT ON_SimpleArray<ON_UUID>& aId);

	/** \return The task's unique id. */
	virtual UUID Id(void) const = 0;

	/** Returns the unique interface id of this extension. */
	virtual UUID InterfaceId(void) const final override;

	/** Get the render engine id associated with the task. Only tasks associated with
		the current renderer will be shown on menus. If this method returns uuidUniversalRenderEngine
		then the task will always be shown. This is the default. */
	virtual UUID RenderEngineId(void) const;

	/** \return \e true if the task is enabled, else \e false.
		 This is used by content-dependent tasks to disable the task's UI
		 if the wrong number of contents are selected. */
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const = 0;

	/** \return \e true if the task can be used when editing a sandbox document, else \e false. */
	virtual bool SupportsSandbox(void) const;

	/** Query if the task should be displayed to the user in a menu etc.
		The default implementation returns the result of calling origin.SupportsTask().
		\param origin represents the UI that the menu was invoked in.
		\param bOnItem is \e true if the user right-clicked on one or more items (thumbnail, tree node etc).
		 It will be \e false if the menu was not a context menu, nothing was selected or the user right-clicked
		 on a blank area of the UI. For content/sub-node controls, this is always \e true.
		\return \e true if a UI element (e.g., a menu item) should be created for the task, else \e false. */
	virtual bool Query(const IRhRdkTaskOrigin& origin, bool bOnItem) const;

	/** Update the UI element (e.g., a menu) associated with a task. This method should
		rarely if ever, be overridden. It first checks if the task is enabled generally
		by calling IsEnabled(). If the task is enabled, it updates the task's UI by
		calling Update(). The actual update is performed by an override of Update(). */
	virtual void UserUpdate(IRhRdkTaskUpdate& tu) const;

	/** Result codes for Execute() */
	enum class Result : unsigned int { Success, Failure, Cancel };

	/** Execute the task. This method should rarely, if ever, be overridden. As well as
		executing the task by calling Execute(), it does common operations such as asking
		if the user is sure, and displaying an error message on failure. The actual task
		operation is performed by an override of Execute(). */
	virtual Result UserExecute(const IRhRdkTaskOrigin& origin) const;

	/** \return the sort order of the task on any menus it appears on. */
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const = 0;

	/** This enum is for MenuSeparators() */
	enum class Separator : unsigned int
	{
		None,   // No separators will surround this task on the menu.
		Before, // A separator will appear before this task on the menu.
		After,  // A separator will appear after this task on the menu.
		Both,   // A separator will appear before and after this task on the menu.
	};

	/** \return the menu separators that surround the menu item of this task. */
	virtual Separator MenuSeparators(void) const;

	/** \return The menu string for this task.
		\param kind is the kind of top-level content being displayed
		 in the UI that is showing the menu. */
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin& origin, CRhRdkContent::Kinds kind) const = 0;

	/** \return An icon for the 'out' or 'unchecked' state of the menu item or button that initiates this task. */
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, CRhinoDib& dibOut) const;

	/** \return An icon for the 'in' or 'checked' state of the menu item or button that initiates this task.
		 Since many operations do not have a 'checked' state, the default implementation returns IconOut(). */
	virtual bool IconIn(CRhRdkContent::Kinds kind, int width, int height, CRhinoDib& dibOut) const;

	/** Set an error message and severity. This is typically called when a task
		is about to return failure. */
	virtual void SetError(const wchar_t* wszMessage, RhRdkMbSeverity severity) const;

	/** This enum is for SubMenu() */
	enum class SubMenus
	{
		None,       // Task will appear in the top-level menu.
		CreateNew,  // Task will appear in a 'Create New' sub-menu.
		Thumbnails, // Task will appear in a 'Thumbnails' sub-menu.
		PlugIn,     // Custom task will appear in a sub-menu with the name of the plug-in that owns the task.
	};

	/** \return a value from the SubMenus enum which determines if the task can appear
		 on a sub-menu, and which sub-menu it will appear on. The default is 'None'. */
	virtual SubMenus SubMenu(void) const;

	/** \return \e true if the task can only operate on top-level content, else \e false. */
	virtual bool IsForTopLevelContent(void) const;

public:
	/** Implement this method to execute the task. */
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const = 0;

	/** Implement this method to update the UI element associated with the task (e.g., a menu).
		If you do not implement this, the task's UI elements will always appear in an active
		and unchecked state. */
	virtual void Update(IRhRdkTaskUpdate& tu) const;

protected:
	/** Implement this method to provide an 'Are you sure?' message.
		Only override and implement this if you want such a message.
		\param origin is the task origin.
		\param sMessageOut accepts the message that will be displayed to the user. The message
		 box will have 'Yes' and 'No' buttons and a 'Don't show me this again' check box.
		\param sYesVerbOut provides alternative text for the 'Yes' button (only used on the Mac).
		\param sNoVerbOut provides alternative text for the 'No' button (only used on the Mac).
		\param sDontShowAgainIdOut accepts an identifier which is used to remember if the user
		 chose to not be asked this question again.
		\return \e true if such a message is required by this task, else \e false. */
	virtual bool GetSureMessage(const IRhRdkTaskOrigin& origin, ON_wString& sMessageOut, ON_wString& sYesVerbOut, ON_wString& sNoVerbOut, ON_wString& sDontShowAgainIdOut) const;

	/** Implement this method to provide a custom undo string.
		The default is automatically created from the menu string. */
	virtual void GetUndoString(const IRhRdkTaskOrigin& origin, ON_wString& sOut) const;

	/** Set the selection so that the user can view and edit a collection of one or more contents.
		\param origin is the task origin which represents where the selection should be set.
		\param aContent is the contents to set as the current selection. */
	virtual void SetSelection(const IRhRdkTaskOrigin& origin, const CRhRdkContentArray& aContent) const;

public:
	/** Helper function to set error message with severity 'error' and return failure. */
	Result Error(const wchar_t* wszMessage) const;

protected:
	void FullUndoString(const IRhRdkTaskOrigin& origin, ON_wString& sOut) const;

private:
	class CImpl;
	mutable CImpl* m_pImpl;
	CImpl& Impl(void) const;
};

/** \class CRhRdkSimpleTask

	Base class for tasks that are not content-dependent.

*/
class RHRDK_SDK CRhRdkSimpleTask : public CRhRdkTask
{
protected:
	/** Always returns \e true since simple tasks cannot be disabled based on content selection. */
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const final override;
};

/** \class CRhRdkContentTask

	Base class for content-dependent tasks.

*/
class RHRDK_SDK CRhRdkContentTask : public CRhRdkTask
{
};

/** \class CRhRdkSingleContentTask

	Base class for content-dependent tasks that require exactly one content to be selected.

*/
class RHRDK_SDK CRhRdkSingleContentTask : public CRhRdkContentTask
{
protected:
	/** \return \e true if exactly one content is selected, else \e false. */
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const final override;

	/** \return \e true if bOnItem is true, else \e false. */
	virtual bool Query(const IRhRdkTaskOrigin& origin, bool bOnItem) const override;
};

class RHRDK_SDK CRhRdkSingleTopLevelContentTask : public CRhRdkSingleContentTask
{
protected:
	virtual bool IsForTopLevelContent(void) const final override;
};

/** \class CRhRdkMultipleContentTask

	Base class for content-dependent tasks that require at least one content to be selected.

*/
class RHRDK_SDK CRhRdkMultipleContentTask : public CRhRdkContentTask
{
protected:
	/** \return \e true if at least one content is selected, else \e false. */
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const final override;

	/** \return \e true if bOnItem is true, else \e false. */
	virtual bool Query(const IRhRdkTaskOrigin& origin, bool bOnItem) const override;
};

class RHRDK_SDK CRhRdkMultipleTopLevelContentTask : public CRhRdkMultipleContentTask
{
protected:
	virtual bool IsForTopLevelContent(void) const final override;
};

/** \class CRhRdkRequestTask

	A request task is a task that is implemented in the UI plug-in.
	These tasks operate directly on a UI element and therefore only
	the UI plug-in knows how to execute them.

*/
class RHRDK_SDK CRhRdkRequestTask : public CRhRdkSimpleTask
{
public:
	virtual void Update(IRhRdkTaskUpdate& tu) const override;
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const final override;
};

class RHRDK_SDK CRhRdkCustomTask : public CRhRdkTask
{
protected:
	virtual SubMenus SubMenu(void) const final override;
	virtual bool Query(const IRhRdkTaskOrigin& origin, bool bOnItem) const override;
};

RDK_DEPRECATED class RHRDK_SDK CRhRdkCreateNew final // NOT IMPLEMENTED.
{
public:
	CRhRdkCreateNew();
	~CRhRdkCreateNew();

	enum { cnf_ForceAdvanced = 1, cnf_MultiSelect = 2, cnf_SuppressLibraryDialog = 4, cnf_SuppressHarvesting = 8, cnf_HideNewTab = 16, cnf_HideExistingTab = 32 };

	bool Execute(const IRhRdkTaskOrigin&) const;
	bool Execute(const IRhRdkTaskOrigin&, UINT, OUT CRhRdkContentArray&) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkImportFromFile final
{
public:
	CRhRdkImportFromFile();
	~CRhRdkImportFromFile();

	bool Execute(const IRhRdkTaskOrigin& origin, CRhRdkContentArray& aContentOut) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class IRhRdkTaskOrigin

	A task origin represents the place in the UI where the user
	clicked to invoke a menu or other UI that displays tasks.

*/
class RHRDK_SDK IRhRdkTaskOrigin
{
public:
	/** This method is implemented as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** \return the control type that was clicked on. */
	virtual RhRdkUserControl Control(void) const = 0;

	/** \return The controller of the control that the user is interacting with. */
	virtual CRhRdkControllerPtr Controller(void) const = 0;

	/** Gets the number of selected contents. */
	virtual int GetSelectedCount(void) const = 0;

	/** Get the contents that have been selected by the user.
		\param aContent accepts the selected contents.
		\param bForModify indicates if the operation is planning to modify the contents. */
	virtual void GetSelection(OUT CRhRdkContentArray& aContent, bool bForModify=false) const = 0;

	/** \return \e true if the origin supports the execution of a particular task. */
	virtual bool SupportsTask(const CRhRdkTask& t) const = 0;

	/** An origin is 'ambivalent' when it wants to show tasks for both top-level and
		child contents on the same menu. This is a special quirk of the spanner menu. */
	virtual bool IsAmbivalent(void) const = 0;

	/** \return \e true if tasks can replace the selected content. */
	virtual bool ReplacingSelection(void) const = 0;

	/** Gets the kind of top-level content in the UI. Note that this is not necessarily the same kind
		as the editor is displaying. For example, the Textures panel displays textures but the top-level
		kind of a particular texture could be a material. */
	virtual CRhRdkContent::Kinds TopLevelKind(void) const = 0;

	// [SDK_UNFREEZE] virtual CRhRdkContent::Kinds EditorKind(void) const = 0;

	/** Gets the allowed kinds for creating new contents. */
	virtual CRhRdkContentKindList AllowedKinds(void) const = 0;

	/** Gets the contents that have been selected by the user.
		\param aContentOut accepts the selected contents. */
	virtual void GetSelectionImpl(CRhRdkContentArray& aContentOut) const = 0;

	/** Gets the preview appearance if the origin is a thumbnail list. */
	virtual const class CRhRdkPreviewAppearance* PreviewAppearance(void) const = 0;

	/** TODO: Docs */
	virtual int GetState(void) const = 0;

	class IRequestHandler
	{
	public:
		/** This method is implemented as \code delete this; \endcode */
		virtual void DeleteThis(void) = 0;

		/** The UI implements this method to update a particular UI-bound task's
			enable or check state on a menu or other UI that displays tasks.
			\param uuidTask is the identifier of the task being updated.
			\param tu is a task update object which you can use to get and
			 set the check or enable state. */
		virtual void OnTaskRequestUpdate(const UUID& uuidTask, IRhRdkTaskUpdate& tu) = 0;

		/** The UI implements this method to execute a particular UI-bound task.
			\param uuidTask is the identifier of the task being executed.
			 The UI only has to handle tasks that it cares about. */
		virtual bool OnTaskRequestExecute(const UUID& uuidTask, const IRhRdkTaskOrigin& origin) = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;

	protected:
		virtual ~IRequestHandler() { }
	};

	/** Some tasks create new contents. When that happens, this method is
		called with the content array. The task origin temporarily stores
		the pointers so that the originating UI can examine them. */
	virtual void AcceptNewContent(const CRhRdkContentArray& aContent) const = 0; // [SDK_UNFREEZE] AcceptContent, not New

	/** Gets the array of contents stored by AcceptNewContent(). */
	virtual void GetNewContent(OUT CRhRdkContentArray& aContent) const = 0; // [SDK_UNFREEZE] GetContent, not New

	/** Send a task-update request to the UI that created this task origin.
		This enables the UI to update tasks that are UI-bound. */
	virtual void SendTaskRequestUpdate(const UUID& uuidTask, IRhRdkTaskUpdate& tu) const = 0;

	/** Send a task-execute request to the UI that created this task origin.
		This enables the UI to execute tasks that are UI-bound. */
	virtual bool SendTaskRequestExecute(const UUID& uuidTask) const = 0;

public: // Support for tasks that manipulate child slots.

	/** \return the parent. If the task origin is not a sub-node, returns null. */
	virtual_su CRhRdkContent* Parent(void) const /*= 0*/; // [SDKUNFREEZE] Currently uses EVF(L"Parent").

	/** \return the child slot name. If the task origin is not a sub-node, returns null. */
	virtual const wchar_t* ChildSlotName(void) const = 0;

public: // Callbacks.

	/** TODO: Docs */
	virtual void OnBeginAskUserSure(const UUID& uuidTask) const = 0;

	/** TODO: Docs */
	virtual void OnEndAskUserSure(const UUID& uuidTask, RhRdkMbResult) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkTaskOrigin() { }
};

// This function is in the public SDK by mistake. No longer implemented.
RDK_DEPRECATED RHRDK_SDK IRhRdkTaskOrigin* RhRdkNewCreateNewButtonTaskOrigin(CRhRdkControllerPtr con, CRhRdkContent::Kinds kind);

enum class RhRdkMenuIcons { Check, Radio };
RHRDK_SDK bool RhRdkGetMenuIcon(RhRdkMenuIcons mi, const ON_2iSize& size, OUT CRhinoDib& dib);
