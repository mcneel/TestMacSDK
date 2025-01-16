
#pragma once // Public SDK header

class IRhRdkContentEditor;
class CRhRdkCoreContent;
class IRhinoUiHolder;
class IRhinoUiSection;
class IRhinoUiDataSource;

#if defined(_MSC_VER)
#pragma warning( push )
// C4263: Member function does not override any base class virtual member function
// C4264: No override available for virtual member function from base 'class'; function is hidden
#pragma warning( disable : 4263 4264 )
#endif

/** \class IRhRdkContentUI

	This interface represents the user interface (UI) of a content. A content's UI appears inside
	an editor which is represented by IRhRdkContentEditor. The UI is the part of the editor which
	allows the user to view and modify the persistent state of the selected content(s).

	This interface is implemented by RDK internally in the core content class, CRhRdkCoreContent.
	If you do not override CRhRdkCoreContent::CreateUI() you do not have to concern yourself with
	the details. If however, you do want to override CreateUI() to create your own custom UI, then
	you must derive your UI class from CRhRdkCustomContentUI and implement this interface to support
	your own UI design. \note CRhRdkCustomContentUI implements some of this interface for you.

	\see CRhRdkCustomContentUI
	\see CRhRdkContent::CreateUI()
	\see CRhRdkCoreContent::CreateUI()
	\see IRhRdkContentEditor
*/
class RHRDK_SDK IRhRdkContentUI
{
public:
	/** You must implement this method as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** \return the UI's framework. */
	virtual RhRdkUiFrameworks Framework(void) const = 0;

	/** \internal For RDK internal use only. */
	RDK_INTERNAL virtual CRhRdkControllerPtr Controller(void) const = 0;

	/** \internal For RDK internal use only. */
	RDK_INTERNAL virtual void SetController(CRhRdkControllerPtr con) = 0;

	/** Your implementation \e must do any and all clean-up here and \e not in DeleteThis(). */
	virtual void Destroy(void) = 0;

	/** Get the unique identifier of the UI. Implemented by CRhRdkCustomContentUI. */
	virtual UUID Uuid(void) const = 0;

	/** Get the unique identifier of the editor that the UI is being displayed in.
		Implemented by CRhRdkCustomContentUI. */
	virtual UUID EditorUuid(void) const = 0;

	/** Called when the user selects content in the editor.
		\param aContent is the content being selected. Your UI should 'remember'
		the instance ids of the contents so that it can look them up again and transfer
		their state to the controls in DisplayData().
		\see DisplayData(). */
	virtual void SetSelection(const CRhRdkContentArray& aContent) = 0;

	/** Called when the UI should transfer the selected content state to the controls.
		\see SetSelection(). */
	virtual void DisplayData(void) = 0;

	/** Enable or disable controls in the UI. */
	virtual void EnableDisableControls(void) = 0;

	/** Set the position and size of the UI. */
	virtual void SetPositionAndSize(const ON_4iRect& rect) = 0;

	/** \return \e true if the UI is created else \e false. */
	virtual bool IsCreated(void) const = 0;

	/** \return \e true if the UI is being shown, else \e false. */
	virtual bool IsShown(void) const = 0;

	/** Show/Hide the UI. Your implementation must respond to this by showing or hiding the entire UI.
		Please call the base class. */
	virtual void Show(bool bShow) = 0;

	/** Enable/disable the UI. Your implementation must respond to this by enabling or disabling the entire UI. */
	virtual void Enable(bool bEnable) = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED void AddClient(CRhRdkCoreContent& c);

	/** Called when a client is added to the UI. Your implementation should remember
		this client, perhaps in a CRhRdkContentArray. Implemented by CRhRdkCustomContentUI.
		\see ClientCount() */
	virtual void AddClient(const CRhRdkContent& c) = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED void RemClient(CRhRdkCoreContent& c);

	/** Called when a client is removed from the UI. Your implementation should remove the client
		from its client list or array. When the last client is removed, you \e must hide the entire UI.
		Implemented by CRhRdkCustomContentUI.
		\see ClientCount(). */
	virtual void RemClient(const CRhRdkContent& c) = 0;

	/** \return the number of clients using the UI. Implemented by CRhRdkCustomContentUI.
		\see AddClient() \see RemClient() */
	virtual int ClientCount(void) const = 0;

	/** \internal For RDK internal use only. */
	virtual void ReleaseAllClients(void) = 0;

	/** \return UI information for debugging. */
	virtual ON_wString DebugString(void) const = 0;

	/** \return The holder that contains the UI sections, if this is an expandable
		 content UI with sections inside holders. Returns null for custom UIs. */
	virtual IRhinoUiHolder* ContentUIHolder(void) const = 0;

	/** This method is deprecated (not implemented) because IRhRdkContentUI can now be
		implemented without using any 'holder'. This happens in custom UIs. */
	RDK_DEPRECATED virtual class IRhinoUiHolder& Holder(void) const;

	/** This method is deprecated (not implemented) because IRhRdkContentUI can now be implemented
		without using any 'sections'. This happens in custom UIs. You will need to change
		your code to add the section via IRhRdkExpandableContentUI. */
	RDK_DEPRECATED virtual void AddSection(class CRhRdkExpandingSection* pSection, int dummy=0);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

public:
	/** This method is deprecated (not implemented) in favor of RhRdkContentUIManager().FindUI() */
	RDK_DEPRECATED static IRhRdkContentUI* Get(const UUID&) { return 0; }

protected:
	virtual ~IRhRdkContentUI() { }
};

/** \class CRhRdkCustomContentUI

	This is the base class for custom content user interfaces. If you want to override
	CRhRdkCoreContent::CreateUI() to create your own custom UI, you \e must derive your UI
	from this class. This ensures that your custom UI is properly registered with the RDK,
	and it also implements some of the IRhRdkContentUI methods for you.
	Please do not try to directly implement IRhRdkContentUI. This approach will not work.

	Also, please note that as of Rhino 8, your CRhRdkCustomContentUI derived class must also
	be inherited from CWnd. If this is not the case, you will need to update your plug-in.

	\see CRhRdkContent::CreateUI()
	\see CRhRdkCoreContent::CreateUI()
	\see IRhRdkContentUI

*/
class RHRDK_SDK CRhRdkCustomContentUI : public IRhRdkContentUI
{
public:
	CRhRdkCustomContentUI(const UUID& uuidEditor, const UUID& uuidUI);
	virtual ~CRhRdkCustomContentUI();

	/** Get the client at an index.
		\see ClientCount(). */
	const CRhRdkContent* GetClient(int index) const;

	/** Helper function to get the selected content. */
	void GetSelection(CRhRdkContentArray& aOut) const;

public: // Implement IRhRdkContentUI.
	virtual UUID Uuid(void) const override;
	virtual UUID EditorUuid(void) const override;
	virtual int ClientCount(void) const override;
	virtual void AddClient(const CRhRdkContent& c) override;
	virtual void RemClient(const CRhRdkContent& c) override;
	virtual void Show(bool bShow) override;
	virtual ON_wString DebugString(void) const override;

	/** \internal For RDK internal use only. */
	RDK_INTERNAL virtual CRhRdkControllerPtr Controller(void) const override;

	/** \internal For RDK internal use only. */
	RDK_INTERNAL virtual void SetController(CRhRdkControllerPtr con) override;

	virtual void SetSelection(const CRhRdkContentArray& aContent) override;
	virtual IRhinoUiHolder* ContentUIHolder(void) const override; // Returns null because it's a custom UI.
	virtual void ReleaseAllClients(void) override;

	// This is actually useless. See [EVF_OVERSIGHT]. Clients were forced to override
	// EVF() and now this one never gets called because of their forced overrides.
	virtual void* EVF(const wchar_t*, void*) override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class IRhRdkExpandableContentUI

	This interface extends IRhRdkContentUI to represent the default user interface which
	the RDK provides for a content. This is the UI created by the default implementation
	of CRhRdkCoreContent::CreateUI(). The main feature of this implementation is a 'holder'
	containing one or more expandable 'sections' (AKA 'roll-ups').

*/
class RHRDK_SDK IRhRdkExpandableContentUI : public IRhRdkContentUI
{
public:
	/** Add a section to the UI. Called \e before Create(). A generic controller is attached. */
	virtual void AddSection(IRhinoUiSection* pSection) = 0;

	/** \internal For RDK internal use only.
		Add a section to the UI with a custom controller. Called \e before Create(). */
	RDK_INTERNAL virtual void AddSection(IRhinoUiSection* pSection, CRhRdkControllerPtr con) = 0;

	/** \return The holder that contains the UI sections. */
	virtual IRhinoUiHolder* ContentUIHolder(void) const = 0;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif
