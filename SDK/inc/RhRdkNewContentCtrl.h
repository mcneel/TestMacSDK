
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

/** \class IRhRdkNewContentCtrl -- AKA 'NCC'.

	STARTING WITH RHINO 8, THIS INTERFACE SHOULD NOT BE USED BY 3RD PARTY PLUG-INS.

*/
class RHRDK_SDK IRhRdkNewContentCtrl : public IRhinoUiWithController
{
public:
	enum class Styles : unsigned int
	{
		Material,            // The control is for materials.
		Environment,         // The control is for environments.
		Texture,             // The control is for textures.
		MaterialIPE,         // The control is for materials and is being used next to an in-place editor.
		MaterialAssignByIPE, // The control is for materials, has 'assign by' items in its drop-down list,
		                     // and is being used next to an in-place editor.
	};

	/** Create a new content control.
		\param s specifies the style of the control.
		\param f must be RhRdkUiFrameworks::Eto; the others are not implemented.
		\return An interface to the new content control if successful, else null. */
	static IRhRdkNewContentCtrl* New(Styles s, RhRdkUiFrameworks f);

	/** Returns the ids of the data sources needed to make the control work. */
	static void RequiredDataSources(OUT ON_SimpleArray<ON_UUID>& aId);

	/** \return The content kind that this control displays. */
	virtual CRhRdkContent::Kinds Kind(void) const = 0;

	/** Refreshes the control. */
	virtual void UpdateView(void) = 0;

	/** Set the control to 'varies'. Useful for clients that only support
		single content selections and want the control to display (varies)
		when more than one content is selected. */
	virtual void SetForcedVaries(bool b=true) = 0;

	/** \see SetForcedVaries(). */
	virtual bool GetForcedVaries(void) const = 0;

	/** Get the selected contents.
		\param aContent accepts the contents.
		\return \e true if successful, else \e false. */
	virtual bool GetSelection(OUT CRhRdkContentArray& aContent) const = 0;

	/** Set the selected contents. Supports 'varies'.
		\param aContent is the array of contents.
		\return \e true if successful, else \e false. */
	virtual bool SetSelection(const CRhRdkContentArray& aContent) = 0;

	/** Get the selected contents' instance ids.
		\param aInstance accepts the instance ids.
		\return \e true if successful, else \e false. */
	virtual bool GetSelection(OUT CRhRdkInstanceIdArray& aInstance) const = 0;

	/** Set the selected contents' instance ids. Supports 'varies'.
		\param aInstance is the array of contents.
		\return \e true if successful, else \e false. */
	virtual bool SetSelection(const CRhRdkInstanceIdArray& aInstance) = 0;

	/** Query whether or not a context menu is displayed. */
	virtual bool ShowContextMenu(void) const = 0;

	/** Set whether or not a context menu is displayed. */
	virtual void SetShowContextMenu(bool bShow) = 0;

	/** Query whether or not a 'New' button is displayed in the drop-down list. */
	virtual bool ShowNewButtonInDropDown(void) const = 0;

	/** Set whether or not a 'New' button is displayed in the drop-down list. */
	virtual void SetShowNewButtonInDropDown(bool bShow) = 0;

	/** Query whether or not a 'plug-in' item is displayed in the drop-down list. */
	virtual bool ShowPlugInItemInDropDown(void) const = 0;

	/** Set whether or not a 'plug-in' item is displayed in the drop-down list. */
	virtual void SetShowPlugInItemInDropDown(bool bShow) = 0;

	/**	\return \e true if the drop-down list is open, else \e false. */
	virtual bool IsDropDownOpen(void) const = 0;

	/** Make the control open its drop-down thumbnail list (if closed).
		\return \e true if successful, else \e false. */
	virtual bool OpenDropDown(void) = 0;

	/** Make the control close its drop-down thumbnail list (if open).
		The drop-down list also closes automatically when it loses the focus.
		\return \e true if successful, else \e false. */
	virtual bool CloseDropDown(void) = 0;

	/** Make the control open its context menu.
		\param point is the client coordinates to display the menu at.
		\return \e true if successful, else \e false. */
	virtual bool OpenContextMenu(CPoint point) = 0;

public:
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED static void NewDataSources(OUT ON_SimpleArray<IRhinoUiDataSource*>& aDS, const CRhRdkContentKindList& kinds, const CRhRdkDocument* pRdkDoc);

	/** Creates new data sources for managing a simple selection of document-resident contents.
		\param aDS accepts all the new data sources that the NCC needs in order to function.
		\param rdkDoc specifies the document within which the contents reside.
		\param kinds specifies the allowed content kinds.
		The caller shall delete the returned data sources when they are longer needed. */
	static void NewDataSources(OUT ON_SimpleArray<IRhinoUiDataSource*>& aDS, const CRhRdkContentKindList& kinds, const CRhRdkDocument& rdkDoc);

public: // Support for style MaterialAssignByIPE. This style specializes
        // the control for use in the Object Material UI.

	enum class AssignBys : unsigned int
	{
		Unset,
		Layer,  // Object material(s) assigned by layer.
		Parent, // Object material(s) assigned by object's parent.
		Object, // Object material(s) assigned by object.
		Varies, // Object materials are assigned differently.
		PlugIn, // Object material assign-by is managed by the current render plug-in.
	};

	/** Get the assign-by for the Object Material UI.
		This UI not only allows selection of contents, but also of
		special items. These items are listed in the AssignBys enum. */
	virtual AssignBys GetAssignBy(void) const = 0;

	/** Set the assign-by for the Object Material UI.
		\param a is the assign-by setting.
		\return \e true if successful, else \e false. */
	virtual bool SetAssignBy(AssignBys a) = 0;

	class RHRDK_SDK IAssignBy
	{
	public:
		virtual AssignBys Get(void) const = 0;
		virtual void Set(AssignBys) = 0;
		virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

	protected:
		virtual ~IAssignBy() { }
	};

public: // Debugging support.
	virtual ON_wString DebugText(void) const = 0;
	virtual void SetDebugText(const wchar_t* wsz) = 0;

protected:
	virtual ~IRhRdkNewContentCtrl() { }
};

/** \class CRhRdkNewContentCtrl -- AKA 'NCC'.

	Using this class as a dialog member is no longer supported.

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class CRhRdkNewContentCtrl
{
	// OBSOLETE
};
