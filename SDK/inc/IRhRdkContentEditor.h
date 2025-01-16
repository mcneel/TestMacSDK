
#pragma once // Public SDK header

#include "RhRdkContent.h"

using CRhRdkControllerPtr = std::shared_ptr<class IRhinoUiController>;

/** \class IRhRdkEditor

	This interface represents an editor.

*/
class RHRDK_SDK IRhRdkEditor
{
public:
	/** \return The unique identifier of the editor. */
	virtual UUID Id(void) const = 0;

	enum class Types : unsigned int
	{
		Unset,    // Unspecified.
		Main,     // One of the main modeless content editors.
		Modal,    // A pop-up modal content editor.
		Floating, // A pop-up modeless content editor.
		InPlace,  // An in-place content editor.
	};

	/** \return The editor type. */
	virtual Types Type(void) const = 0;

	/** \return The UI framework. */
	virtual RhRdkUiFrameworks Framework(void) const = 0;

	/** \return the modality of the editor. */
	virtual RhRdkUiModalities Modality(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	/** Find an editor by its id. */
	RDK_DEPRECATED static IRhRdkEditor* Find(const UUID& uuidEditor);

protected:
	virtual ~IRhRdkEditor() { }
};

/** \class IRhRdkContentEditor

	This interface represents a content editor. Currently two types of content editor are implemented:

	1. The Thumbnail Editor, of which there are 3 kinds, Material Editor, Environment Editor
	   and Texture Palette.

	2. The In-place Editor which is found in the Object Properties material page,
	   the Layer Material dialog and the Ground Plane dialog.

	A content editor acts as a host for a content UI. The UiHostWindow() method
	returns a pointer to the window that is the parent of the main UI window.
	The main UI window is usually a content UI holder containing sections
	(AKA roll-ups), but if you override CRhRdkCoreContent::CreateUI() and provide
	a custom UI, it can be anything you like.

	A content editor must know the document it is for. This is not strictly needed on Windows,
	but on the Mac there can be many editors for a content kind but each for a different
	document. Even modal editors which are capable of editing free-floating contents
	still have a document, but this document is a 'sandbox', not a 'live' document.

*/
class RHRDK_SDK IRhRdkContentEditor : public IRhRdkEditor
{
public:
	virtual ~IRhRdkContentEditor() { }

	enum class EditMethods : unsigned int // Methods by which contents can be edited.
	{
		Local,    // Edit in the same editor.
		Modal,    // Edit in a pop-up modal editor.
		Floating, // Edit in a pop-up modeless editor.
	};

	/** \return The kind of top-level content being displayed in the editor. */
	virtual CRhRdkContent::Kinds TopLevelKind(void) const = 0;

	/** \internal For RDK internal use only. */
	RDK_INTERNAL virtual CRhRdkControllerPtr Controller(void) const = 0;

	/** Called just after a content UI has been created. */
	virtual void OnContentUICreated(class IRhRdkContentUI& ui) = 0;

	/** OBSOLETE. There is no longer a host 'window' since the framework uses Eto (which is C#).
	// If you were calling this, just use NULL for the host or parent. */
	RDK_DEPRECATED virtual HWND UiHostWindow(void) const = 0;

	virtual_su UUID RdkDocumentId(void) const; // [SDK_UNFREEZE]
	virtual_su void SetRdkDocumentId(const UUID& uuidRdkDoc); // [SDK_UNFREEZE]

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	/** Find a content editor by its id. */
	static IRhRdkContentEditor* Find(const UUID& uuidEditor);
};
