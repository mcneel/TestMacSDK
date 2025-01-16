
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"
#include "RhRdkObject.h"
#include "RhRdkRenderQuality.h"
#include "IAutoUIExtraRequirements.h"
#include "RhRdkExtension.h"
#include "RhRdkColor.h"

/** \class CRhRdkContent

	Abstract base class for all kinds of render content. A 'render content' (AKA 'content') is a
	material, environment or texture used for rendering.

	Contents have a unique type id which is the same for all instances of the same class and an instance id
	which is unique for each instance. They know how to provide a shader for rendering, how to read and write
	their state as XML and how to create their own user interfaces.

	There are two flavors of content in the RDK -- temporary and persistent. It is very important to understand
	the distinction between a temporary content instance and a persistent content instance, and the fact that a
	temporary instance (and all its children) can become persistent. Persistent content is registered with a
	document and is usually (but not always) owned by it.

	Temporary contents get created and deleted very often during the normal operation of the RDK. In fact, just
	about anything the user clicks on might result in a temporary content being created and deleted again. They
	are created by the content browser, the thumbnail rendering, and so on. They are 'free floating' and are
	owned by whomever created them. They do not appear in the modeless UI, they do not get saved in the 3dm
	file, and they can freely be deleted again after use.

	Contrast this with persistent contents which are attached to a document. They are always owned by RDK,
	appear in the modeless UI and get saved in the 3dm file. Pointers to persistent contents should never
	be stored by clients; you should only store their instance ids and look them up again using
	CRhRdkDocument::FindContentInstance(). They can be deleted only after detaching them from the document.

	CRhRdkDocument::CreateContentByType() is the highest-level function for creating a content. It creates it,
	initializes it, adds it to the document and sends many events. It even records undo. You cannot call this
	method from just anywhere. It must only be called by 'UI code'; scripts or buttons on a dialog. It results
	in a persistent (usually top-level) content being attached to the document and appearing in all the RDK UI
	elements that display contents, like the thumbnail and tree views. If you call this method and specify a
	parent and child-slot name, your new content will be attached to the document-resident parent as a child
	and the UI will be updated accordingly.

	If you want to create a complex content that has children from the moment it is created, you can add the
	children in Initialize(). You want to create a temporary child content that is not attached to any document
	and after that you want to attach it to your parent content -- which RDK is about to attach to a document.
	The sequence of operation is:

	1. The user asks to create your content by pressing a button or menu and choosing your content from the browser.

	2. The RDK uses a factory to create it and calls Initialize().

	3. Your override of Initialize() uses the factory to create your child content and attaches it to the parent
	   by calling SetChild().

	4. RDK attaches the root content (parent) to the specified document and sends events to update the UI.

	The important point is that everything is temporary while the content structure is being built. Only
	after the whole structure is complete will the top-level parent be attached to the document making the
	whole structure persistent.

*/
class RHRDK_SDK CRhRdkContent : public CRhRdkObject, public IAutoUIExtraRequirements
{
public:
	/**	This constructor automatically registers the content in the global content registry.
		\note Construction should not call any virtual content methods. If you need to do anything
		 complicated, please override Initialize() and do it there.
		\see CRhRdkContent::Initialize(). */
	CRhRdkContent();

	/**	\param bAutoGlobalRegister specifies if the content should automatically be registered
		 in the global content registry.
		\note Construction should not call any virtual content methods. If you need to do anything
		 complicated, please override Initialize() and do it there.
		\see CRhRdkContent::Initialize(). */
	CRhRdkContent(bool bAutoGlobalRegister);

	virtual ~CRhRdkContent();

	enum class Kinds : unsigned int { Unset, Material, Environment, Texture };

	// Replaced with RhRdkChangeContext
	enum RDK_DEPRECATED eChangeContext { ccUI, ccDrop, ccProgram, ccIgnore, ccTree, ccUndo, ccFieldInit, ccSerialize, ccRealTimeUI, ccForce32bit = 0xFFFFFFFF };

	/** \return \e true if change context is UI based. */
	static bool IsChangeContextUIBased(RhRdkChangeContext cc);

	/** Get the content fields.
		\see CRhRdkContentField.
		\see CRhRdkContentFields.
		\see BitFlags(). */
	class CRhRdkContentFields& Fields(void) const;

	/** Override this to initialize your content. Please be sure to call the base class
		\e before calling any other methods in your override.
		\note If you create a content using 'new' or the low-level factory methods NewContent(),
		 NewMaterial(), NewEnvironment(), or NewTexture(), then you must call this method on your new content.
		 If, however, you create the content by calling the higher-level methods NewContentFromType() or
		 CreateContentByType() then you must not call this method because RDK calls it automatically.
		\see CRhRdkContentFactory::NewContent() \see CRhRdkMaterialFactory::NewMaterial()
		\see CRhRdkEnvironmentFactory::NewEnvironment() \see CRhRdkTextureFactory::NewTexture()
		\see IRhRdkContentFactories::NewContentFromType()
		\see CRhRdkDocument::CreateContentByType()
		\return \e true if successful, else \e false. */
	virtual bool Initialize(void);

	/** Override this to uninitialize your content. Please be sure to call the base class
		\e after calling any other methods in your override.
		\note This method should only be called if Initialize() returned \e true. */
	virtual void Uninitialize(void);

	/** \return The plug-in's render engine id. */
	virtual UUID RenderEngineId(void) const = 0;

	/** \return The plug-in's Rhino plug-in id. */
	virtual UUID PlugInId(void) const = 0;

	/** \return The unique id of the content type. */
	virtual UUID TypeId(void) const = 0;

	/** \return The internal name of the content type.
		This name should never change and should not be localized. */
	virtual ON_wString InternalName(void) const = 0;

	/** \return The localized name of the content type. */
	virtual ON_wString TypeName(void) const = 0;

	/** \return The localized description of the content type. */
	virtual ON_wString TypeDescription(void) const = 0;

	/** Bit-flags returned by BitFlags() */
	enum : unsigned int
	{
		bfTextureSummary      = 0x0001, // Texture UI includes an automatic texture summary section. \see AddAutoParameters().
		bfQuickPreview        = 0x0002, // Editor displays an instant preview before preview cycle begins.
		bfPreviewCache        = 0x0004, // Content's preview imagery can be stored in the preview cache.
		bfLocalTextureMapping = 0x0010, // Texture UI includes an automatic local mapping section for textures. \see AddAutoParameters().
		bfGraphDisplay        = 0x0020, // Texture UI includes a graph section.
		bfAdjustment          = 0x0080, // Texture UI includes an adjustment section.
		bfFields              = 0x0100, // Content uses fields to facilitate data storage and undo support. \see Fields().
		bfNameTypeSection     = 0x0800, // Content UI includes a 'Name and Type' section (or sections).

		// Developer note: Missing bits are from obsolete flags. Do not re-use these bits.
	};

	/** Optionally override this method to modify the content's bit-flags.
		<b>You should first call the base class</b> and then modify the resulting value before finally
		returning it. This will ensure that you get the correct default behavior before modifying it.
		\return The bit-coded flags for this content. */
	virtual unsigned int BitFlags(void) const;

	/** \return The category of the content. */
	virtual const wchar_t* Category(void) const = 0;

	/** Used by DynamicIcon() */
	enum class DynamicIconUsage : unsigned int
	{
		TreeControl,     // Used in a tree control (e.g., CTreeCtrl).
		SubnodeControl,  // Used in a sub-node control (\see CRhRdkSubNodeCtrl)
		ContentControl,  // Used in a content control  (\see CRhRdkContentCtrl)
		General,         // Used in a custom user interface. The background will be plain white.
	};

	/** Creates imagery (AKA an 'icon') to represent the content's appearance.
		This imagery is dynamic, i.e., it changes depending on the content's state
		(parameters). It is similar to a quick preview and may be implemented in a
		similar way to CreatePreview().
		\param size is the desired physical size of the imagery, in pixels.
		\param dibOut is a DIB that accepts the created imagery.
		\param usage specifies how the icon will be used.
		\return \e true if successful, else \e false. */
	virtual bool DynamicIcon(const ON_2iSize& size, CRhinoDib& dibOut, DynamicIconUsage usage) const = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual HICON Icon(void) const;

	/** Creates imagery (AKA an 'icon') to represent the content's type.
		This imagery is static, i.e., it does not change.
		\param size is the desired physical size of the imagery, in pixels.
		\param dibOut is a DIB that accepts the created imagery.
		\return \e true if successful, else \e false. */
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const = 0;

	/** \return The kind of the content. */
	virtual Kinds Kind(void) const = 0;

	/** \return A pointer to a fixed string giving the library file extension of the content. */
	virtual const wchar_t* LibraryFileExtension(void) const = 0;

	/** \return The content's name. */
	virtual ON_wString InstanceName(void) const;

	/** Set the content's name. If the content is attached to a document, all necessary UI
		will be updated. This method also creates an undo record and sends the events
		OnContentRenaming and OnContentRenamed.
		\param wszName is the new name.
		 If you override this method, <b>please be sure to call the base class</b>. */
	virtual void SetInstanceName(const wchar_t* wszName);

	// This will replace the above when we can break the SDK and it will return bool. [SDK_UNFREEZE]
	virtual_su void SetInstanceName(const wchar_t* wszName, bool bRenameEvents, bool bEnsureNameUnique);

	/** \return The content's display name. This is the same as InstanceName() except if the name is empty,
		it returns '(unnamed)'. This is suitable for display in user interfaces. */
	virtual_su ON_wString DisplayName(void) const;

	/** \return The unique instance id of this instance. */
	virtual const UUID& InstanceId(void) const; // [SDK_UNFREEZE] Get rid of the &

	/** Set the unique instance id of this instance. */
	virtual void SetInstanceId(UUID uuidInstance);

	/** Gets the unique instance id of this instance and all its children.
		\param aOut is an array that accepts the instance ids. */
	virtual void GetInstanceIdsRecursive(class CRhRdkInstanceIdArray& aOut) const;

	/** \return The content's notes. */
	virtual ON_wString Notes(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED void SetNotes(const wchar_t* wszNotes, bool bSendEvents);

	/** Set the content's notes.
		\param wszNotes can be any text.
		\note If the notes do not change, the method has no effect.
		\note If the content is attached to a document, an undo record is created. */
	virtual void SetNotes(const wchar_t* wszNotes);

	/** \return The content's tags. */
	virtual ON_wString Tags(void) const;

	/** Set the content's tags.
		\param wszTags is a semicolon-delimited string of tags.
		\param bSendEvents is \e true to send events, else \e false.
		\note If the tags do not change, the method has no effect.
		\note If the content is attached to a document, an undo record is created. */
	virtual void SetTags(const wchar_t* wszTags, bool bSendEvents=true);

	/** Reset selected parameters to default values.
		\param aParams is an array of internal names of parameters that require resetting.
		\note If aParams is empty, \e all parameters will be reset. */
	virtual void ResetParametersToDefaults(const ON_ClassArray<ON_wString>& aParams);

	/** DEPRECATED. Call RhRdkContentFactories().NewContentFromTypeEx() instead. */
	RDK_DEPRECATED static CRhRdkContent* CreateObjectFromType(UUID uuidType);

	/** \return \e true if the content is hidden from the UI. This feature only works for
		top-level contents because it hides the entire hierarchy. Hidden contents are never shown in the
		modeless UI. They can be shown in the Modal UI if deliberately edited (\see Edit()). They also
		appear in the Object (or Layer) Material Properties UI which always shows whatever is assigned to
		the object (or layer). In the Object (or Layer) Material Properties UI, if the user drops
		down the list, hidden contents are not listed. Hidden contents, being part of the document content
		list, will be listed by any scripts or other code that iterates over the document content list. */
	virtual bool IsHidden(void) const;

	/** Contents can be created as 'auto-delete' by certain commands such as 'Picture'.
		These contents are automatically hidden from the user when the associated Rhino object
		is deleted. They are later deleted when the document is saved.
		This feature only works for top-level contents because it works on the entire hierarchy.
		\return \e true if the content is hidden by the auto-delete system. */
	virtual bool IsHiddenByAutoDelete(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED void SetHidden(bool bHidden, eChangeContext cc);

	/** Set this content as hidden or not. If hidden, it will not appear in the modeless UI. This feature only works
		for top-level contents because it hides the entire hierarchy. Hidden contents are never shown in the
		modeless UI. They can be shown in the Modal UI if deliberately edited (\see Edit()). They also
		appear in the Object (or Layer) Material Properties UI which always shows whatever is assigned to
		the object (or layer). In the Object (or Layer) Material Properties UI, if the user drops
		down the list, hidden contents are not listed. Hidden contents, being part of the document content
		list, will be listed by any scripts or other code that iterates over the document content list.
		\note This method is designed to be used when you want to show and hide a content at will.
		 If you are the author of the content and you want it to be permanently hidden, it is recommended
		 that you override IsHidden() and return \e true. Calls to SetHidden() are then ignored. */
	virtual void SetHidden(bool bHidden);

	/** \return \e true if the content was created by loading it from a file, else \e false. */
	virtual bool CreatedByLoading(void) const;

	/** Convert IAutoUIExtraRequirements::SetContext to RhRdkChangeContext. */
	static RhRdkChangeContext ChangeContextFromSetContext(SetContext sc);

	/** \return \e true if the content is locked. Locked contents cannot be deleted or renamed by the UI.
		\note This method returns \e true if this content or any of its ancestors are locked. */
	virtual bool IsLocked(void) const;

	/** Set this content as locked. This method must only be called once, before the content
		is added to a document. */
	virtual void SetLocked(void);

	/** Convert RhRdkChangeContext to IAutoUIExtraRequirements::SetContext. */
	static SetContext SetContextFromChangeContext(RhRdkChangeContext cc);

	/** Begins a change or batch of changes. This returns a non-const reference to this content
		which you should use to make your changes. It may also make a copy of the content state
		allowing EndChange() to send an event with the old and new contents. Calls to this
		method are counted; you must call EndChange() once for every call to BeginChange().
		Please do not use const_cast or any other means of circumventing this requirement.
		\note If Changed() was called between the calls to BeginChange() and EndChange(), the
		 last call to EndChange() may cause the OnContentChanged event to be sent.
		\param cc is the change context. If this is UI, Program, Drop or Tree, the content
		 will be copied. EndChange() will then send the copy as 'old' in the OnContentChanged event.
		\note If you override this method, <b>please be sure to call the base class</b>.
		\see EndChange() \see Changed()
		\return A non-const reference to this content. */
	virtual CRhRdkContent& BeginChange(RhRdkChangeContext cc) const;

	/** Ends a change or batch of changes. Calls to this method are counted; you must call
		EndChange() once for every call to BeginChange().
		\note If BeginChange() was called with UI, Program, Drop or Tree and Changed()
		 was called between the calls to BeginChange() and EndChange(), the last call to
		 EndChange() will send the OnContentChanged event.
		\note If you override this method, <b>please be sure to call the base class</b>.
		\see BeginChange() \see Changed()
		\return \e true if a change has actually occurred, else \e false. */
	virtual bool EndChange(void);

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void Changed(eChangeContext cc);

	/** Called when the state of the content is changed. If your content subclass
		implements a member variable that affects rendering and/or appears in the content's
		user interface, then you should call this method from your Set... method when
		the Set... method detects a change to the value of the member.
		Optionally override this method to perform further actions when the content changes.
		<b>Please be sure to call the base class</b>.
		\note If you are using content fields, setting the field value with SetValue() calls this automatically. */
	virtual void Changed(void);

	/** Gets the change context that was passed to BeginChange().
		\note Unset is never returned. If BeginChange() has not been called, the method returns Ignore.
		\see BeginChange(). */
	virtual RhRdkChangeContext GetChangeContext(void) const;

	/** \class Change
		Helper class for automatically creating BeginChange() / EndChange() bracketed calls. Example usage:
		\code
			{	const CRhRdkContent::Change<CMyTexture> t(*pMyTexture, RhRdkChangeContext::UI); // BeginChange() called here.
				t().SetMyValue(...); // Calls CRhRdkContent::Changed() if value changed.
			}	// EndChange() called here.
		\endcode
		\note Using the default 'Ignore' ensures that the content will not be copied and no events will be sent by EndChange().
	*/
	template <class T> class Change
	{
	public:
		Change(const CRhRdkContent& c, RhRdkChangeContext cc=RhRdkChangeContext::Ignore) : m_c(c.BeginChange(cc)) { }
		~Change() { m_c.EndChange(); }

		T& operator () (void) const { return dynamic_cast<T&>(m_c); }

	private:
		CRhRdkContent& m_c;
	};

	/** Update the thumbnail preview of any thumbnail that is displaying this content.
		Override this to perform extra actions when the preview is updated. <b>please be sure to call the base class</b>.
		\note The preview is only updated if the relevant thumbnail display has its Automatic Update option set.
		 If this option is not set, the preview will not be updated until the user takes action to update it manually. */
	virtual void UpdatePreview(void) const;

	/** \return The delay before the thumbnail preview is updated. Override this to change the
		 time it takes before a new preview is generated after a change to the content. */
	virtual int PreviewUpdateDelay(void) const;

	/** This method is deprecated in favor of the data source event system. It is no longer called. */
	RDK_DEPRECATED virtual void UpdateUI(void) const;

	/** Called by Changed() to update Rhino document tables.
		\return \e true if successful, else \e false. */
	virtual bool UpdateDocumentTables(void) const = 0;

	/** Call this method to open the content in the main content editor. The content must be attached
		to a document or the method will fail. The content will be opened in the editor associated with
		that document (only relevant on Mac).
		If you override this method, <b>please be sure to call the base class</b>.
		\return \e true if successful, else \e false. */
	virtual bool OpenInMainEditor(void) const;

	/** \return This method is deprecated in favor of Edit(). */
	RDK_DEPRECATED virtual bool OpenInModalEditor(UINT uFlags=0);

	/** This method allows a content hierarchy to be edited using a modal (AKA 'pop-up') editor.
		Overriding this method allows the content provider to implement their own editing procedure.
		The default implementation uses the standard RDK modal editor which after successful editing
		will return an edited version of the entire hierarchy. If the original content is in a document,
		it will remain there, and the edited one will be 'free-floating'. Therefore it is the caller's
		responsibility to do any replacement in the document if required. The returned new content
		will be owned by the caller. To edit multiple contents, please see RhRdkModalEditContent().

		Example usage:
		\code
			const auto* pContentNew = pContent->Edit();
			if (nullptr != pContentNew)
			{
				...
			}
		\endcode

		\return An edited version of the content hierarchy if successful, else null. This edited version has
		 the same instance ids as the original and is unregistered. You can either attach it to a document or
		 globally register it for normal non-document use. Note that if you choose to do the latter, you must
		 first make sure that either the instance ids are changed or the original hierarchy is deleted. \see GlobalRegister()

		\note The method always edits the entire hierarchy. It places a copy of the hierarchy in the editor
		 and selects the copied item that corresponds to this one. Therefore, editing a child will return
		 a top-level content, not a child.

		You can also replace a document content with an edited version of it.
		\see RhRdkReplaceContentInDocument().
	*/
	virtual CRhRdkContent* Edit(void) const;

	/** Used by the RDK content editor to create the content's UI. This UI allows editing of the content's persistent state.
		The implementation in CRhRdkCoreContent should suit most clients. \see CRhRdkCoreContent::CreateUI()
		\return \e true if successful, else \e false. */
	virtual class IRhRdkContentUI* CreateUI(class IRhRdkContentEditor& e, const UUID& uuidUI, const class CRhRdkContentArray& aInitialClients, void* pReserved) = 0;

	/** For expert use only. You should never need to override this method. */
	virtual void ReleaseAllUIs(bool bIncludeChildren) const = 0;

	/** For expert use only. You should never need to override this method. */
	virtual void ReleaseAllEditorUIs(const UUID& uuidEditor, bool bIncludeChildren) const = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual class IRhRdkExpandableContentUI* UI(void) const = 0;

	/** Get an array of pointers to UIs that the content is a client of. */
	virtual void GetUIs(ON_SimpleArray<IRhRdkContentUI*>& aOut) const = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool GetDefaultsFromUser(HWND);

	/** Override this to get defaults from the user when the content is created.
		Only used by contents that need special defaults that must be chosen by the user.
		The default implementation is a no-op that returns \e true.
		\return \e true if successful, \e false if user cancelled. */
	virtual bool GetDefaultsFromUser(void);

	enum class Harvested : unsigned int // Return values for HarvestData()
	{
		None = 0,
		Some = 1,
		All  = 2,
	};

	/** Implement this to transfer data from another content to this content during creation.
		\param oldContent is an old content from which the implementation may harvest data. */
	virtual Harvested HarvestData(const CRhRdkContent& oldContent) = 0; // [SDK_UNFREEZE] This needs flags to specify what to harvest (e.g., the name).

	// Serialization of the content. Includes writing of child streams (i.e., to serialize
	// the content, only the top level CRhRdkContent object needs to be serialized).

	enum RDK_DEPRECATED eGetParamsContext { gpc_serialization, gpc_harvesting, gpc_rendering, gpc_force32bit = 0xFFFFFFFF };
	enum RDK_DEPRECATED eSetParamsContext { spc_serialization, spc_harvesting, spc_rendering, spc_force32bit = 0xFFFFFFFF };

	/** This method is deprecated in favor of WriteParametersToSection(). */
	RDK_DEPRECATED virtual bool SetParameters(class IRhRdk_XMLSection& section, eSetParamsContext context) const;

	/** This method is deprecated in favor of ReadParametersFromSection(). */
	RDK_DEPRECATED virtual bool GetParameters(const class IRhRdk_XMLSection& section, eGetParamsContext context);

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual DWORD WriteXMLStream(wchar_t* wszStream, DWORD dwMaxChars, eSetParamsContext context) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual DWORD WriteParameterXMLStream(wchar_t* wszStream, DWORD dwMaxChars, eSetParamsContext context) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool ReadParameterXMLStream(const wchar_t* wszStream, eGetParamsContext context);

	/** Context for WriteParametersToSection(), WriteXMLStream() and WriteParameterXMLStream(). */
	enum class WriteParamsContext : unsigned int
	{
		Serialization  =  0, // Method is being called for serialization; you must write all your parameters.
		Harvesting     =  1, // Method is being called for parameter harvesting; you must write all parameters that might want to be harvested (usually all).
		Rendering      =  2, // Method is being called for rendering; only write parameters that affect rendering.
		DragDrop       =  3, // Method is being called for serialization specifically by drag and drop; you must write all your parameters.
	};

	/** Context for ReadParametersFromSection() and ReadParameterXMLStream(). */
	enum class ReadParamsContext : unsigned int
	{
		Serialization  =  0, // Method is being called for serialization; you must read all your parameters.
		Harvesting     =  1, // Method is being called for parameter harvesting; you must read all parameters that might want to be harvested (usually all).
		Rendering      =  2, // Method is being called for rendering; only read parameters that affect rendering.
	};

	/** Write the entire content XML to a buffer including parameters, simulation and children.
		Only write the text, there is no need to write a zero terminator.
		Only the top level CRhRdkContent object needs to call this method.
		\param wszStream The buffer to be filled with XML text.
		\param maxChars The physical number of characters that the buffer can accomodate.
		\param context gives the reason the method is being called.
		\param pOptions is for future use.
		\return The logical number of characters written.
		 This will be more than maxChars if the buffer was not big enough. */
	virtual size_t WriteXMLStream(wchar_t* wszStream, size_t maxChars, WriteParamsContext context, const CRhinoFileWriteOptions* pOptions) const;

	// **** Parameters ****
	//
	// The parameters are the properties of the content, e.g., color, transparency etc.

	enum class ParamSerialMethod : unsigned int // Return values for ParameterSerializationMethod()
	{
		GetSet,    // Parameters are serialized by ReadParametersFromSection() and WriteParametersToSection() (recommended).
		XmlStream, // Parameters are serialized by ReadParameterXMLStream() and WriteParameterXMLStream() (not recommended).
	};

	/** Implement this method to indicate which serialization method your content uses.
		If you return GetSet then you must implement the ReadParametersFromSection() and
		WriteParametersToSection() methods to serialize the parameters.
		If you return XmlStream then you must implement the low-level ReadParameterXMLStream() and
		WriteParameterXMLStream() methods to serialize the parameters.
		\note Using GetSet is the easiest and preferred way to serialize the parameters, and it is
		 required if you want your content to animate in Bongo 2. */
	virtual ParamSerialMethod ParameterSerializationMethod(void) const = 0;

	/** Implement this method to get the content's parameters into an XML section.
		\param section is the section to write the parameters to.
		\param context gives the reason the method is being called.
		\see ParameterSerializationMethod().
		\return \e true if you successfully get the parameters. */
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const;

	/** Implement this method to set the content's parameters from an XML section.
		\param section is the section to read the parameters from.
    \param context gives the reason the method is being called.
		\see ParameterSerializationMethod().
		\return \e true if you successfully set the parameters. */
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context);

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void AddAutoParameters(class IRhRdkParamBlock& paramBlock, int id);

	/** Optionally override this method to add the content's parameters to a collection.
		You need to override this method if you are using the automatic UI system and
		also if your content supports the texture summary. \see BitFlags().
		The default implementation is a no-op. Even if you are using fields, you still
		need to override this and call Fields().AddValuesToParamBlock().
		\param paramBlock is the collection.
		\param id is a caller-defined identifier (can be zero if not needed).
		 Values of 0x8000 and above are reserved for RDK internal use.
		 \see enum CRhRdkCoreContent::eSectionId for special values used internally. */
	virtual void AddAutoParameters(class IRhRdkParamBlock& paramBlock, int id) const;

	/** Optionally override this method to get the content's parameters from a collection.
		You need to override this method if you are using the automatic UI system and
		also if your content supports the texture summary. \see BitFlags().
		The default implementation is a no-op. Even if you are using fields, you still
		need to override this and call Fields().GetValuesFromParamBlock().
		\param paramBlock is the collection.
		\param id is a caller-defined identifier (can be zero if not needed).
		 Values of 0x8000 and above are reserved for RDK internal use.
		 \see enum CRhRdkCoreContent::eSectionId for special values used internally. */
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int id);

	/** \note It is highly recommended that you use WriteParametersToSection() instead of this method
		because the registerable properties system (used by Bongo 2 for animation) will require it.
		Instead of overriding WriteParametersToSection() you can override this method to directly write
		the content's parameter XML to a buffer. Because RDK has no way of knowing how big a
		buffer to allocate, this function is called twice. The first time, maxChars is zero.
		In this case the buffer does not yet exist and you should only calculate the number of
		characters that would be written to the buffer if it existed. On the second call, the
		buffer will exist and you should write to it.
		\note Your XML must be contained within a section called \verbatim
		 <parameters> \endverbatim otherwise RDK won't be able to recognise it as the parameter
		 block when reading it back in.
		 For example:

		 \verbatim
		 <parameters>
		 	<amount-first type="double">1.0</amount-first>
		 	<amount-second type="double">2.0</amount-second>
		 </parameters>
		 \endverbatim

		 The default implementation is a no-op that returns 0.

		\param wszStream The buffer to be filled with XML text.
			   Only write the text, there is no need to write a zero terminator.
		\param maxChars The physical number of characters that the buffer can accomodate.
		\param context gives the reason the method is being called.
		\see ParameterSerializationMethod().
		\return Logical number of characters written.
		 This will be more than maxChars if the buffer was not big enough. */
	virtual size_t WriteParameterXMLStream(wchar_t* wszStream, size_t maxChars, WriteParamsContext context) const;

	/** Instead of overriding ReadParametersFromSection() you can override this method to directly
		read the content's parameter XML from a buffer.
		The default implementation is a no-op that returns \e false.
		\param context gives the reason the method is being called.
		\see ParameterSerializationMethod().
		\return \e true on success, else \e false. */
	virtual bool ReadParameterXMLStream(const wchar_t* wszStream, ReadParamsContext context);

	// **** Simulation ****
	//
	// The simulation is a set of parameters that Rhino could use to render a mock-up of
	// the content in the absence of your render plug-in.

	/** Optionally override this method to add the content's simulation to an XML section.
		This is needed if you want to avoid having to use WriteSimulationXMLStream()
		for serialization. For this to work you must return \e true.
		The default implementation is a no-op that returns \e false.
		This is the easiest and preferred way to serialize the simulation. */
	virtual bool SetSimulation(IRhRdk_XMLSection& section) const;

	/** Optionally override this method to get the content's simulation from an XML section.
		This is needed if you want to avoid having to use ReadSimulationXMLStream()
		for serialization. For this to work you must return \e true.
		The default implementation is a no-op that returns \e false.
		This is the easiest and preferred way to serialize the simulation. */
	virtual bool GetSimulation(const IRhRdk_XMLSection& section);

	/** Optionally override this method to set a specific parameter in the content.
		You can use this system to change content programatically if this method is
		implemented for that content. If you implement this method you must return \e true.
		Use of content fields causes this method to be automatically implemented.
		The default implementation checks if fields are in use and if so, it sets the field value and
		returns \e true. Otherwise it returns \e false. */
	RDK_DEPRECATED virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue, eChangeContext cc);
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue);

	/** Optionally override this method to get a specific parameter from the content.
		Use of content fields causes this method to be automatically implemented.
		The default implementation checks if fields are in use and if so, it returns the field value. */
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const;

	/** Instead of overriding SetSimulation() you can override this method to directly write
		the content's simulation XML to a buffer. Because RDK has no way of knowing how big a
		buffer to allocate, this function is called twice. The first time, dwMaxChars is zero.
		In this case the buffer does not yet exist and you should only calculate the number of
		characters that would be written to the buffer if it existed. On the second call, the
		buffer will exist and you should write to it.
		\note Your XML must be contained within a section called \verbatim <simulation>
		\endverbatim otherwise RDK won't be able to recognise it as the simulation block when
		 reading it back in.
		 For example:

		 \verbatim
		 <simulation>
		 	<amount-first type="double">1.0</amount-first>
		 	<amount-second type="double">2.0</amount-second>
		 </simulation>
		 \endverbatim

		 The default implementation is a no-op that returns 0.

		\param wszStream The buffer to be filled with XML text.
		 Only write the text, there is no need to write a zero terminator.
		\param maxChars The physical number of characters that the buffer can accomodate.
		\return Logical number of characters written.
		 This will be more than dwMaxChars if the buffer was not big enough. */
	virtual size_t WriteSimulationXMLStream(wchar_t* wszStream, size_t maxChars) const;

	/** Instead of overriding GetSimulation you can override this method to directly
		read the content's simulation XML from a buffer.
		The default implementation is a no-op that returns \e true.
		\return \e true on success, else \e false. */
	virtual bool ReadSimulationXMLStream(wchar_t* wszStream);

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED bool ReadXMLStream(const wchar_t* wszStream);

	/** Set the object's parameters from the supplied stream.
		Also causes the construction of child objects if they exist in the stream.
		\return \e true if successful, otherwise \e false. */
	virtual bool ReadXMLStream(const wchar_t* wszStream, const CRhinoFileReadOptions* pOptions);

	// These methods are deprecated in favor of the one below.
	RDK_DEPRECATED static CRhRdkContent* FromXMLStream(const wchar_t* wszXML);
	RDK_DEPRECATED static CRhRdkContent* FromXMLStream(const wchar_t* wszXML, const CRhinoDoc* pDocAssoc);

	/** Creates a new CRhRdkContent derived object from an XML stream. */
	static CRhRdkContent* FromXMLStream(const wchar_t* wszXML, const CRhinoDoc* pDocAssoc, bool bUseXMLInstanceId);

	/** Creates a new CRhRdkContent derived object from an ON_RenderContent. */
	static CRhRdkContent* FromON_RenderContent(const class ON_RenderContent& rc, const CRhinoDoc* pDocAssoc, bool bRecursive);

	/** Creates a new ON_RenderContent from this CRhRdkContent. */
	ON_RenderContent* ToON_RenderContent(bool bRecursive=true) const;

	enum class CopyMethods : unsigned int
	{
		Normal,    // The copy produces a normal content. This means it is globally registered and its
		           // instance ids are changed to be unique.
		Transient, // The copy produces a transient content. This means it is not registered anywhere.
		           // It is an exact copy of the original including instance ids.
	};

	/** Copies the current content including its children.
		\param m is the method used to create the copy. Most (if not all) clients will use the default,
		 'Normal'. 'Transient' is an advanced feature used for complex internal manipulation of
		 contents; it will rarely, if ever, be used by ordinary clients.
		\note The copy can subsequently be attached to a document if desired.
		\note The copied contents will be found by RhRdkFindContentInstance().
		\return A copy of the content tree. */
	virtual CRhRdkContent* MakeCopy(CopyMethods m=CopyMethods::Normal) const;

	/** Internally this method calls MakeCopy(). \see MakeCopy(). */
	CRhRdkContent* Clone(CopyMethods m=CopyMethods::Normal) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual CRhRdkContent* MakeGroupInstance(void);

	/** Create an 'instance' of the content hierarchy and group the new hierarchy with this hierarchy.
		If the instance is subsequently attached to the same document, the state of all members
		of the group will be kept synchronized. With the exception of the instance ids, all
		state is exactly preserved in the new instance hierarchy.
		\note The grouping will have no effect until the new instance is attached to the same document.
		\return A grouped instance of the content hierarchy. */
	virtual CRhRdkContent* MakeGroupInstance(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool Ungroup(void);

	/** Remove this content from any instance group it may be a member of.
		Does not record undo but does send the OnContentGroupIdChanged event.
		\return \e true if content was ungrouped, \e false if it was not part of a group. */
	virtual bool Ungroup(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool UngroupRecursive(void);

	/** Remove this content and all its children from any instance groups they may be a member of.
		Does not record undo but does send the OnContentGroupIdChanged event.
		\return \e true if a content was ungrouped, \e false if no content or child was part of a group. */
	virtual bool UngroupRecursive(void) const;

	/** Remove this content and all its children from any instance groups they may be a member of.
		If any content in the same document is left alone in the group, that content is also ungrouped.
		Records undo and sends events OnContentChanged and OnContentGroupIdChanged.
		\note This method is designed to be called from a content UI and is intended for RDK internal
		 use but may be used as an expert user override.
		\return \e true if a content was ungrouped, \e false if no content or child was part of a group. */
	virtual bool SmartUngroupRecursive(void);

	/** Please call DuplicateEx(DupOpt) instead. */
	RDK_DEPRECATED virtual const CRhRdkContent* Duplicate(void) const;

	/** Please call DuplicateEx(DupOpt) instead. */
	RDK_DEPRECATED const CRhRdkContent* DuplicateEx(bool reserved) const;

	/** Duplicate the content. The exact behavior depends on the document status of the content and
		  the value of the DupOpt enum passed as parameter 'd'.
		- If the content is not associated with a document, the function merely returns a copy.
		- When 'd' is DupOpt::Attach:
		  - If the content is associated with (or attached to) a document, the function returns a copy
		    after attaching the copy to the same document. In this case, an undo record will be created
		    and all necessary UI will be updated. Even if the source content is a child, the copy will be
		    top-level. If a hidden content is copied via this method, the copy will not be hidden.
		- When 'd' is DupOpt::NoAttach:
		  - The function returns a copy without attaching it to the document. In other words, it behaves
		    exactly as if the original content was not associated with a document.
		\return A pointer to the new content if successful, otherwise null. */
	enum class DupOpt
	{
		Attach,   // Attaches the content to the document.
		NoAttach, // Does not attach the content to the document.
	};
	/*virtual [SDK_UNFREEZE]*/ const CRhRdkContent* DuplicateEx(DupOpt d) const;

	// Enumeration of children and parent.

	/** \return Parent content or null if this is the top level object. */
	virtual const CRhRdkContent* Parent(void) const;

	/** \return First child of this content or null if none. */
	virtual const CRhRdkContent* FirstChild(void) const;

	/** \return First sibling of this content or null if none. */
	virtual const CRhRdkContent* NextSibling(void) const;

	/** \return A pointer to the top level parent of this const content. Never returns null. */
	virtual const CRhRdkContent* TopLevelParent(void) const;

	/** \return A pointer to the top level parent of this non-const content. Never returns null. */
	virtual CRhRdkContent* TopLevelParent(void);

	/** \return The top level parent of this const content. */
	virtual const CRhRdkContent& TopLevel(void) const;

	/** \return The top level parent of this non-const content. */
	virtual CRhRdkContent& TopLevel(void);

	/** \return \e true if this is a top-level content (i.e., has no parent; is not a child). */
	virtual bool IsTopLevel(void) const;

	/** \return \e true if this is a child content (i.e., has a parent; is not top-level). */
	virtual_su bool IsChild(void) const; // [SDK_UNFREEZE]

	/** \return \e true if the content hierarchy is 'complete'. If the content uses textures that have
		 missing image files, for example, it is considered to be incomplete.
		\param sReason is a localized string giving the reason why the content is incomplete (if so).
		 This optional parameter is only filled in if the method returns \e false. */
	virtual_su bool CheckComplete(ON_wString& sReason) const; // [SDK_UNFREEZE]

	/** \return \e true if the content is the 'default instance' for its kind. */
	virtual bool IsDefaultInstance(void) const;

	/** \return \e true if a content can be edited in a content editor. */
	virtual bool CanBeEdited(void) const;

	/** \return The proxy type of the content. The majority of contents you encounter will return 'None'.
		 There are two types of proxies; proxy materials (single and multi) and proxy textures.
		 When a content is a proxy material it represents a material in the document material table that is not associated
		 with an RDK material. These are also known as 'legacy' or 'V4' materials because they originated in old Rhino 4
		 documents. These proxies are not true document contents and they automatically come and go depending on the state of
		 the material table and the objects that use the table.
		 When a content is a proxy texture it is an aggregate of first-level child textures from any material or environment
		 hierarchy in the document. This allows the Texture Palette to display child textures all in one place for ease of
		 editing. */
	enum class ProxyTypes : unsigned int { None, Single, Multi, Texture };
	virtual ProxyTypes ProxyType(void) const;

	/** Set another content as a child of this content. This content may or may not be attached to a document.
		If this content already has a child with the specified child slot name, that child will be deleted.
		If this content is not attached to a document, the child will be added without sending any events.
		If this content is attached to a document, content undo will be recorded and the necessary events
		will be sent to update the UI.
		\param pChild is a pointer to the content to set as a child of this content. This may be null
		 to just delete any existing child with the specified child slot name.
		 If pChild is already attached to a document, the function will fail.
		 If pChild is already a child of this or another content, the function will fail.
		\param wszChildSlotName is the child slot name that will be assigned to this child.
		 The child slot name cannot be an empty string. If it is, the function will fail.
		\note Do not call this method to add children in your constructor. If you want to add default children,
		 you should override Initialize() and add them there. */
	virtual bool SetChild(const CRhRdkContent* pChild, const wchar_t* wszChildSlotName);

	/** This is an advanced override for expert use only. It is only called by RDK when deleting a child.
		You can override it to detect when RDK is deleting a child of this content. */
	virtual bool ChangeChild(CRhRdkContent* pOldChild, CRhRdkContent* pNewChild);

	/** This is an advanced override for expert use only. It is called at the start of the change-child process
		and gives the content a chance to do something just before its child changes.
		\return \e true unless you want to prevent the child from being changed. */
	virtual bool OnChildChanging(const CRhRdkContent* pOldChild, const CRhRdkContent* pNewChild) const;

	/** This method is deprecated in favor of SetChild() */
	RDK_DEPRECATED virtual bool AddChild(CRhRdkContent* pChild, const wchar_t* wszChildSlotName);

	/** Delete a child by its child slot name. If the child is part of a document-resident content
		hierarchy, an undo record will be created, the child will be detached from the hierarchy
		and deleted, and the necessary events will be sent to update the UI. If the child is part
		of a non-document-resident content hierarchy, it will simply be deleted.
		\param wszChildSlotName is the child slot name of the child to delete.
		\return \e true if successful, else \e false. */
	virtual bool DeleteChild(const wchar_t* wszChildSlotName);

	/** Deletes all the children of this content.
		This method sends events and records undo. */
	virtual void DeleteAllChildren(void);

	/** \return The child slot name of this content. */
	virtual ON_wString ChildSlotName(void) const;

	/** Set the child slot name of this content. */
	virtual void SetChildSlotName(const wchar_t* wszChildSlotName);

	/** \return The child slot name corresponding to a parameter name. */
	virtual ON_wString ChildSlotNameFromParamName(const wchar_t* wszParamName) const;

	/** \return The parameter name corresponding to a child slot-name. */
	virtual ON_wString ParamNameFromChildSlotName(const wchar_t* wszChildSlotName) const;

	/** Get the 'on' state of the specified child slot.
		\param wszChildSlotName is the child slot to get the state of.
		\return The 'on' state of the child slot (variant will be type bool or null if function fails or is not supported). */
	virtual CRhRdkVariant ChildSlotOn(const wchar_t* wszChildSlotName) const;

	/** Set the 'on' state of the specified child slot.
		\param wszChildSlotName is the child slot to set the state of.
		\param bOn is the state to set the child slot to.
		\return \e true if successful, else \e false. */
	virtual bool SetChildSlotOn(const wchar_t* wszChildSlotName, bool bOn);

	/** Get the amount of the specified child slot in the range 0 to 100.
		\param wszChildSlotName is the child slot to get the amount for.
		\return The amount of the child slot (variant will be a numeric type or null if function fails or is not supported). */
	virtual CRhRdkVariant ChildSlotAmount(const wchar_t* wszChildSlotName) const;

	/** Set the amount of the specified child slot.
		\param wszChildSlotName is the child slot to set the amount for.
		\param dPercent is the amount to set the child slot to in the range 0 to 100.
		\return \e true if successful, else \e false. */
	virtual bool SetChildSlotAmount(const wchar_t* wszChildSlotName, double dPercent);

	/** Called when a child slot of the content is about to change.
		\param wszChildSlotName is the name of the child slot that is about to change.
		\note The default implementation just returns \e false.
		\return \e true if successful, else \e false. */
	virtual bool OnChildSlotChanging(const wchar_t* wszChildSlotName) const;

	/** Called when a child slot of the content has changed.
		\param wszChildSlotName is the name of the child slot that has changed.
		\note The default implementation just returns \e false.
		\return \e true if successful, else \e false. */
	virtual bool OnChildSlotChanged(const wchar_t* wszChildSlotName) const;

	/** \return The localized display name of the child slot name. */
	virtual_su ON_wString ChildSlotDisplayName(void) const; // [SDK_UNFREEZE]

	/** \return The immediate child that has the specified child slot name or null if none. */
	virtual const CRhRdkContent* FindChild(const wchar_t* wszChildSlotName) const;

	// Used by sub-classes to retrieve data about the texture on a given channel (like diffuse, or background)
	class RHRDK_SDK TextureChannelInfo final
	{
	public:
		TextureChannelInfo(); // Initializes everything to defaults.

	public:
		CRhRdkColor color;       // Default is black.
		UUID textureInstanceId;  // Default is ON_nil_uuid.
		double textureAmount;    // Default is 0.0. Range is 0.0..100.0
		bool textureOn;          // Default is false.
		ON__UINT_PTR m_reserved; // Reserved for future use.
	};

	/** \return \e true only if content type can be accepted as a child in the specified child slot. */
	virtual bool IsContentTypeAcceptableAsChild(const UUID& uuidType, const wchar_t* wszChildSlotName) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool IsFactoryProductAcceptableAsChild(const class CRhRdkContentFactory* pFactory, const wchar_t* wszChildSlotName) const;

	/** \return \e true only if content produced by a certain factory can be accepted as a child in the specified child slot. */
	virtual bool IsFactoryProductAcceptableAsChild(const class CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const;

	/** \return \e true if content's name is derived from a file name. Used to support file-based textures.
		 The default implementation returns \e false. */
	virtual bool ContentNameTakenFromFileName(void) const;

	/** \returns the default content name stem (e.g., "Custom Material"). By default this function returns TypeName().
		 Bitmap-based materials return the filename of the bitmap. */
	virtual ON_wString AutomaticInstanceName(void) const;

	// Child iterator.
	class RHRDK_SDK CChildIterator final
	{
	public:
		CChildIterator(const CRhRdkContent* pParent);

		/** Gets children in the order they were added to the content. */
		const CRhRdkContent* GetNextChild(void);

	protected:
		const CRhRdkContent* m_pCurrent;
		void* m_pReserved;
	};

	/** \return An iterator for accessing children. */
	virtual CChildIterator GetChildIterator(void) const;

	/** Get an array of children sorted by their child-slot name. */
	void GetSortedChildren(OUT CRhRdkContentArray& aChild) const;

	enum class CSIContext : unsigned int
	{
		All            = 0, // For iterating over all child slots.
		TextureSummary = 1, // For iterating over only child slots that will appear in the Texture Summary.
		Private1       = 2, // For private use by clients.
		Private2       = 3, // For private use by clients.
	};

	// Child slot iterator.
	class RHRDK_SDK CChildSlotIterator : public CRhRdkObject
	{
	public:
		CChildSlotIterator(const CRhRdkContent* pContent, CRhRdkContent::CSIContext context);
		virtual ~CChildSlotIterator();

		/** Retrieve information about the next child slot.
			\param sParamNameOut Receives the internal name of the parameter that is
			 associated with the child slot.
			\param sChildSlotNameOut Receives the name of the child slot.
			\param sDisplayNameOut Receives the display name of the child slot/parameter.
			\return \e true if information was retrieved, \e false if no more child slots. */
		virtual bool NextChildSlot(ON_wString& sParamNameOut, ON_wString& sChildSlotNameOut, ON_wString& sDisplayNameOut) = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t* wszFunc, void* pvData);

	protected:
		const CRhRdkContent* m_pContent;
		CRhRdkContent::CSIContext m_Context;
		ON__UINT_PTR m_reserved; // Reserved for future use.
	};

	/** \return An iterator for accessing child slot information or null if not implemented.
		 The caller shall delete the iterator. */
	virtual CChildSlotIterator* NewChildSlotIterator(CSIContext context) const;

	/** \return A pointer to the factory that creates this type of content (or null if the factory
		 could not be found; this can happen if the plug-in containing the factory has been unplugged. */
	virtual const CRhRdkContentFactory* Factory(void) const;

	/** Searches for the content with the specified instance id. Looks at itself and all children recursively.
		\return A const pointer to the content with the required instance id, or null if not found.
		 If you want to modify the content you will need to call BeginChange() to get a non-const reference.
		\see BeginChange() */
	virtual const CRhRdkContent* FindInstance(const UUID& uuidInstance) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual CRhRdkContent* FindInstance(const wchar_t* wszName) const;

	/** Searches the content hierarchy for contents with the specified instance name.
		\param wszName is the name to search for.
		\param bCaseSensitive specifies if the name check is case-sensitive or not.
		\param bRecurse specifies if the content's children should be searched.
		\param aContent accepts all the contents that satisfy the search. */
	virtual void FindInstanceByName(const wchar_t* wszName, bool bCaseSensitive, bool bRecurse, OUT CRhRdkContentArray& aContent) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void DestroyChildContent(const UUID& uuidPlugIn);

	/** Destroys all child content belonging to the specified plug-in. RDK calls this internally.
		Clients should not call this method. This is an advanced override which clients can use to detect
		when all child content of a plug-in is being deleted by RDK. This happens when the plug-in is unloaded. */
	virtual void DestroyChildContent(const UUID& uuidPlugIn) const;

	/** A content is compatible with a render engine when its RenderEngineId() matches
		the id of the render engine.
		\return \e true if the content is compatible with the specified render engine, else \e false. */
	virtual bool IsCompatible(const UUID& uuidRenderEngine) const;

	/** A content is universal if it is meant to be used with any render engine.
		\return \e true if the content is universal, else \e false. */
	virtual bool IsUniversal(void) const;

	/** A content is private if it is not intended to show in any editor. Private contents can omit support for user interfaces.
		Also, the registered property system ignores private contents. By default, contents are not private.
		\return \e true if the content is private, else \e false. */
	virtual bool IsPrivate(void) const;

	/** Implement this method to return a pointer to your renderer's shader object for this content.
		\param uuidRenderEngine is the render engine id of the caller.
		\param pvData is a pointer to any necessary private data, or null if not required.
		\return A pointer to your renderer's shader object or null on failure. */
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const;

	//DO NOT ENCLOSE THESE IN RHINO_SDK_MFC - it will crash at runtime because this is a virtual function
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual HBITMAP CreatePreview(class CRhRdkRenderPlugIn& plugIn, const class CSize& sizeImage, eRhRdkRenderQuality qual, const class IRhRdkPreviewSceneServer* pSceneServer) const;

	//DO NOT ENCLOSE THESE IN RHINO_SDK_MFC - it will crash at runtime because this is a virtual function
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual HBITMAP CreateLibraryPreview(const class CSize& sizeImage, const IRhRdkPreviewSceneServer* pSceneServer) const;

	/** Create a preview of the content for use in the content editor thumbnail display.
		\param plugIn The render plug-in to use to render the preview.
		\param sizeImage The size of the image to render.
		\param qual The quality of the preview.
		\param pSceneServer A scene server that defines the scene for the preview rendering.
		\param dibOut accepts the rendered preview if the method returns \e true.
		\return \e true if successful, else \e false. */
	virtual bool CreatePreview(CRhRdkRenderPlugIn& plugIn, const ON_2iSize& sizeImage,
	                           RhRdkPreviewQuality qual, const IRhRdkPreviewSceneServer* pSceneServer, class IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) const = 0;

	/** Create a preview bitmap for use when saving the content to the content library.
		\param sizeImage The size of the image to render.
		\param pSceneServer A scene server that defines the scene for the preview rendering. This parameter
		 will be null if the content is a texture.
		\param dibOut accepts the rendered preview if the method returns \e true.
		\return \e true if successful, else \e false. */
	virtual bool CreateLibraryPreview(const ON_2iSize& sizeImage, const IRhRdkPreviewSceneServer* pSceneServer, CRhinoDib& dibOut) const;

	/** Load a content from a library file.
		\param sFullPath is the full path to the library file to load.
		\return A pointer to a new content on success, else null. */
	static CRhRdkContent* LoadFromLibraryFile(const wchar_t* sFullPath);

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool SaveToLibraryFile(const ON_wString& sFullPath, const IRhRdkPreviewSceneServer* pSceneServer) const;

	/** Used by SaveToLibraryFile() */
	enum class EmbedFilesChoice : unsigned int { kNeverEmbed, kAlwaysEmbed, kAskUser };

	/** Save the content to a library file.
		\param sFullPath is the full path to the library file to be created.
		\param pSceneServer is a scene server that defines the scene for the preview rendering.
		\param c is a choice for how embedded files are handled.
		\return \e true on success, else \e false. */
	virtual bool SaveToLibraryFile(const ON_wString& sFullPath, const IRhRdkPreviewSceneServer* pSceneServer, EmbedFilesChoice c) const;

	class RHRDK_SDK CRenderCRCFlags final
	{
	public:
		enum : ON__UINT64
		{
			rcrc_Normal                 = 0x00, // Normal render CRC; nothing is excluded.
			rcrc_ExcludeLinearWorkflow  = 0x01, // Linear Workflow color and texture changes are not included.
			rcrc_ExcludeLocalMapping    = 0x02, // Local mapping is excluded (only used by Textures).
			rcrc_ExcludeUnits           = 0x04, // Units are excluded (only used by Textures).
			rcrc_Reserved2              = 0x08,

			// Recommended flags for use when implementing SimulatedMaterial() or SimulateTexture().
			rcrc_ForSimulation = rcrc_ExcludeLinearWorkflow,

			// Flags to exclude effects that are caused by the associated document (Linear Workflow, Units).
			rcrc_ExcludeDocumentEffects = rcrc_ExcludeLinearWorkflow | rcrc_ExcludeUnits | rcrc_Reserved2,

			#ifdef _DEBUG
			rcrc_DisableCache = 0x80000000, // For debugging only. Disables the render crc cache.
			#endif
		};

		CRenderCRCFlags(const ON__UINT64& f=rcrc_Normal);

		bool operator == (const CRenderCRCFlags& flags) const;
		bool operator != (const CRenderCRCFlags& flags) const;

		/** Get a semicolon-delimited string of param names to exclude from the render CRC. */
		const wchar_t* GetExcludedParamNames(void) const;

		/** Set a semicolon-delimited string of param names to exclude from the render CRC. */
		void SetExcludedParamNames(const wchar_t* wsz);

		operator const ON__UINT64&(void) const;
		const ON__UINT64& Flags(void) const;
		void AddFlag(const ON__UINT64& f);
		bool HasFlag(const ON__UINT64& f) const;

		ON_wString DebugString(void) const;

	private:
		ON_wString m_sExclude;
		ON__UINT64 m_Flags;
		ON__UINT_PTR m_reserved; // Reserved for future use.
	};

	/** Get the render CRC of the content hierarchy. This is an advanced override which is rarely overridden.
		It recurses into children and includes a caching mechanism which means the CRC can be retrieved quickly
		if it hasn't changed. The cache is invalidated when Changed() is called.
		To implement your own CRC function you should override ComputeRenderCRC(), not this method.
		\param rcrcFlags specifies optional flags that control the generation of the crc.
		\note If you are not running on the main thread, you \e must exclude linear workflow and you must
		 \e not exclude units, because this would access the document which is only allowed on the main thread.
		\see ComputeRenderCRC()
		\see Changed() 
		\see RenderCRC(const CRenderCRCFlags& rcrcFlags, const ON_LinearWorkflow& lw) */
	virtual unsigned int RenderCRC(const CRenderCRCFlags& rcrcFlags) const;

	/** This function is deprecated in favor of the one below. */
	RDK_DEPRECATED unsigned int RenderCRC(const CRenderCRCFlags& rcrcFlags, const IRhRdkLinearWorkflow& lw) const;

	/** Use this function to get the RenderCRC of a content when linear workflow is excluded, but you have the linear workflow
		information available. Typically, this will be from the change queue or another threaded application.
		LW must be excluded when you are running off the main thread. */
	unsigned int RenderCRC(const CRenderCRCFlags& rcrcFlags, const ON_LinearWorkflow& lw) const;

	/**	By default, RenderCRC() recurses into children when computing the render CRC.
		However, some applications may require children to be excluded from the render CRC calculation.
		Call this method to enable or disable recursing into children. \see RenderCRC(). */
	void SetIsRenderCRCRecursive(bool bRecursive);

	/** This method is deprecated and no longer called. For more information \see ComputeRenderCRC() */
	RDK_DEPRECATED virtual DWORD ChildrenRenderCRC(void) const;

	/** This method is deprecated and no longer called. For more information \see ComputeRenderCRC() */
	RDK_DEPRECATED virtual bool IsCRCCached(void) const;

	/** This method is deprecated and no longer called. For more information \see ComputeRenderCRC() */
	RDK_DEPRECATED virtual void SetCRCCache(DWORD dwCRC) const;

	/** This method is deprecated in favor of IsOwnedByDocument() */
	RDK_DEPRECATED virtual bool IsInDocument(void) const;

	/** Query whether or not the content hierarchy is owned by a document.
		Document-owned contents are considered to be 'attached' to the document
		that owns them.
		\see RdkDocumentOwner().
		\see SetRdkDocument().
		\return \e true if the content (or its top-level parent) is owned by a document, else \e false. */
	virtual bool IsOwnedByDocument(void) const;

	/** Get the id of the RDK document that this content hierarchy is associated with, if any.
		This can be called on top-level or child contents and will identify the document
		that the top-level parent is associated with, if any.
		\note This identifies the document that the content is \e associated with. Just being
		 associated with a document does not mean the content is actually \e attached
		 to that document. You must call IsOwnedByDocument() to find out if the content is
		 attached to and managed by a document. Clones and copies of content are not
		 attached to a document, but they are still associated with the same document as
		 the content they were cloned/copied from.
		\return The unique identifier of the RDK document that the content hierarchy is associated with,
		 or nil if the content hierarchy is not associated with a document. */
	virtual UUID RdkDocumentId(void) const;

	/** Get the document that this content hierarchy is associated with, if any.
		This can be called on top-level or child contents and will return the document
		that the top-level parent is associated with, if any.
		\note This gets the document that the content is \e associated with. Just being
		 associated with a document does not mean the content is actually \e attached
		 to that document. You must call IsOwnedByDocument() to find out if the content is
		 attached to and managed by a document. Clones and copies of content are not
		 attached to a document, but they are still associated with the same document as
		 the content they were cloned/copied from.
		\return A pointer to the Rhino document that the content hierarchy is associated with,
		 or null if the content hierarchy is not associated with a document. */
	virtual const CRhinoDoc* DocumentAssoc(void) const;
	virtual_su    CRhinoDoc* DocumentAssoc(void);

	/** Set the document that this content hierarchy is associated with.
		This allows you to set the document association of a content hierarchy that is not
		currently associated with or attached to a document. This is useful when you want
		certain document-related functions to work properly on non-document contents.
		\note If the content is already associated with a document, the method will fail.
		\return \e true if successful, else \e false. */
	virtual bool SetDocumentAssoc(const CRhinoDoc& doc) const;

	/** Similar to the above function, but uses another content as the source. As this doesn't access documents
		directly, but only copies the document ID, this is safe when used off the main thread. */
	virtual_su bool SetDocumentAssoc(const CRhRdkContent& source) const;

	/** Remove the content hierarchy's RDK document association.
		This allows you to remove the document association of a content hierarchy that is
		not currently attached to a document. This is useful when you want to disassociate
		a copied content from the document it was copied from.
		\note If the content is not associated with a document, or is attached to a document,
		 the method will fail.
		\return \e true if successful, else \e false. */
	virtual bool RemoveRdkDocumentAssoc(void);

	/** Get the RDK document that this content hierarchy is owned by, if any.
		This can be called on top-level or child contents and will return the document
		that the top-level parent is owned by (and therefore attached to), if any.
		\return A pointer to the RDK document that the content hierarchy is owned by,
		 or null if the content hierarchy is not owned by a document. */
	virtual const CRhRdkDocument* RdkDocumentOwner(void) const;

	/** \deprecated This method is obsolete. */
	RDK_DEPRECATED virtual const CRhRdkDocument* RdkDocumentRegistered(void) const;

	/** Attach the content hierarchy to a document so that the document takes ownership.
		Contents can be attached to a document at the top level; the entire hierarchy is
		attached. Content hierarchies that are owned by a document are managed by that document.
		Content hierarchies that are not owned by a document are managed by their creator.
		It is therefore only legal to use 'delete' on a content when it is owned by you.
		\param pRdkDoc is a pointer to the document to attach the content to. If successful
		 and pRdkDoc is not null, the content will be moved from the document it is currently
		 in (if any) to that document. If pRdkDoc is null, the content will be removed from
		 whatever document it is in and ownership will be transferred to the caller.
		\note Document contents are held in a 'persistent content list' and are visible
		 in any UI that is displaying that document's persistent content lists.
		\see CRhRdkContent::IsOwnedByDocument().
		\see CRhRdkContent::RdkDocumentOwner().
		\see CRhRdkDocument::MaterialList().
		\see CRhRdkDocument::EnvironmentList().
		\see CRhRdkDocument::TextureList().
		\return \e true if successful, else \e false. */
	virtual bool SetRdkDocumentOwner(CRhRdkDocument* pRdkDoc);

	/** \internal For RDK internal use only.
		Get the RDK document that this content hierarchy is associated with, if any.
		This can be called on top-level or child contents and will return the document
		that the top-level parent is associated with, if any.
		\note This gets the document that the content is \e associated with. Just being
		 associated with a document does not mean the content is actually \e attached
		 to that document. You must call IsOwnedByDocument() to find out if the content is
		 attached to and managed by a document. Clones and copies of content are not
		 attached to a document, but they are still associated with the same document as
		 the content they were cloned/copied from.
		\return A pointer to the RDK document that the content hierarchy is associated with,
		 or null if the content hierarchy is not associated with a document. */
	virtual const CRhRdkDocument* RdkDocumentAssoc(void) const;

	/** \internal For RDK internal use only.
		Set the RDK document that this content hierarchy is associated with.
		This allows you to set the document association of a content hierarchy that is not
		currently associated with or attached to a document. This is useful when you want
		certain document-related functions to work properly on non-document contents.
		\note If the content is already associated with a document, the method will fail.
		\return \e true if successful, else \e false. */
	virtual bool SetRdkDocumentAssoc(const CRhRdkDocument& rdkDoc);

	/** \deprecated This method is not implemented and returns null.
		Please use one of the other document methods to get the RDK Document. */
	RDK_DEPRECATED virtual CRhinoDoc* Document(void) const;

	/** Query whether or not the content or any of its ancestors is a reference content.
		\return \e true if the content is a reference, else \e false. */
	virtual bool IsReference(void) const;

	/** \return \e true if the kind of this content matches the input. */
	virtual bool IsKind(CRhRdkContent::Kinds kind) const;

	/** \return A semicolon-delimited string of full paths to files used by the content
		 that need to be embedded in .3dm files and library files (.rmtl, .renv, .rtex).
		 The default implementation returns an empty string. Override this to return
		 the file name or file names used by your content. This is typically used by
		 textures that reference files containing the texture imagery. */
	virtual ON_wString EmbeddedFiles(void) const;

	/** \return \e true if this content supports context-sensitive help. */
	virtual bool SupportsContextHelp(void) const;

	/** Displays context-sensitive help. */
	virtual void DisplayContextHelp(HWND hwndParent) const;

	/** Indicates whether or not the content supports a particular command.
		\param uuidCommand is the UUID of the command.
		\return \e true if the command is supported, else \e false. */
	virtual bool SupportsCommand(UUID uuidCommand) const;

	/** Executes a command.
		\param uuidCommand is the UUID of the command.
		\param pvData is a pointer to command-specific data. */
	virtual void ExecuteCommand(UUID uuidCommand, void* pvData) const;

	/** Emergency virtual function for future expansion. If you override this method,
		you \e must call the base class. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

public: // Implementation of IAutoUIExtraRequirements

	/** Optionally override this method to get a parameter for extra UI requirements.
		<b>Please be sure to call the base class</b>.
		\param wszParamName The name of the content parameter for which to get an extra requirement parameter.
		 \see ParamNameFromChildSlotName() which is relevant when addressing child slots.
		\param wszExtraReqName The name of the extra requirement parameter to retrieve.
		\param vValueOut Receives the parameter's value.
		\return \e true if the parameter was retrieved successfuly, else \e false.
		\note This method must set vValueOut to a non-null value if it returns \e true. */
	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName,
	                                          CRhRdkVariant& vValueOut) const;

	/** Optionally override this method to set a parameter for extra UI requirements.
		<b>Please be sure to call the base class</b>.
		\param wszParamName The name of the content parameter for which to set an extra requirement parameter.
		 \see ParamNameFromChildSlotName() which is relevant when addressing child slots.
		\param wszExtraReqName The name of the extra requirement parameter to be set.
		\param vValue Contains the new value for the specified extra req. parameter.
		\param sc The context in which the set operation is being performed.
		\return \e true if the parameter was set successfuly, else \e false. */
	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName,
	                                          const CRhRdkVariant& vValue, SetContext sc);

	/** Optionally override this method to handle an extra UI requirements button click.
		<b>Please be sure to call the base class</b>.
		\param wszParamName The name of the content parameter for which a button was clicked.
		\param wszButtonName The name of the button that was clicked.
		\return \e true if button click was handled successfully, else \e false. */
	virtual bool OnExtraRequirementButtonClicked(const wchar_t* wszParamName,
	                                             const wchar_t* wszButtonName);

	/** This is an advanced method which you should never need to override. */
	virtual IAutoUIExtraRequirements* DuplicateExtraRequirementsInterface(void) const;

public:
	/** This is an advanced method which you can override in exceptional circumstances.
		\param data is private RDK data.
		 For example, you could call ::RhRdkNew3DPreviewSceneServer() from your override
		 to preview your content (texture) in 3D. */
	virtual class IRhRdkPreviewSceneServer* NewPreviewSceneServer(const class CRhRdkSSData& data) const = 0;

	enum ePreviewSceneType
	{
		preview_scene_server = 0,
		texture_2d_image     = 1,

		force32_bit_sceneType = 0xFFFFFFFF
	};

	/** This method is deprecated in favor of AppearanceDefaults() */
	RDK_DEPRECATED virtual const class IRhRdkThumbViewDefaults* ViewDefaults(void) const;

	/** Provides defaults for a preview appearance.
		The default implementation usually returns null, but it can return an object when loading an RMTL file.
		Because of this, your override should call the base class first and if the result is not null, it should
		return the base class object. Otherwise, you may provide different defaults than the saved or hard coded
		defaults for the content kind.
		\see IRhRdkPreviewAppearanceDefaults
		\see CRhRdkPreviewAppearance */
	virtual const class IRhRdkPreviewAppearanceDefaults* AppearanceDefaults(void) const;

	/** This is an advanced method which you can override in exceptional circumstances.
		\return texture_2d_image if you want RDK to produce a texture preview from the texture evaluator
		 or preview_scene_server to cause RDK to call NewPreviewSceneServer(). */
	virtual ePreviewSceneType PreviewSceneType(void) const = 0;

	enum // Bit flags for PreviewGeometryFlags()
	{
		pg_unused      = 0x00000000,
		pg_sphere      = 0x00000001,
		pg_cube        = 0x00000002,
		pg_cone        = 0x00000004,
		pg_pyramid     = 0x00000008,
		pg_torus       = 0x00000010,
		pg_plane       = 0x00000020,
		pg_cone_sphere = 0x00010000,
		pg_scene       = 0x00020000,

		pg_default_mat = pg_sphere | pg_cube | pg_cone | pg_pyramid | pg_torus | pg_plane | pg_scene,
		pg_default_env = pg_cone_sphere,
		pg_default_tex = pg_unused,

		pg_mat_scene = pg_scene, // Deprecated.

		force32bit_pgflags = 0xFFFFFFFF
	};

	/** \return Bit-flags denoting supported preview geometry types. */
	virtual UINT PreviewGeometryFlags(void) const = 0;

	enum // Bit flags for PreviewBackgroundFlags()
	{
		pb_unused  = 0x00000000,
		pb_none    = 0x00000001,
		pb_checker = 0x00000002,
		pb_custom  = 0x00000004,
		pb_scene   = 0x00000008,

		pb_default_mat = pb_none | pb_checker | pb_custom | pb_scene,
		pb_default_env = pb_unused,
		pb_default_tex = pb_unused,

		force32bit_pbflags = 0xFFFFFFFF
	};

	/** \return Bit-flags denoting supported preview background types. */
	virtual UINT PreviewBackgroundFlags(void) const = 0;

	enum // Bit flags for PreviewLightingFlags()
	{
		pl_unused = 0x00000000,
		pl_simple = 0x00000001,
		pl_sky    = 0x00000002,

		pl_default_mat = pl_simple | pl_sky,
		pl_default_env = pl_unused,
		pl_default_tex = pl_unused,

		force32bit_plflags = 0xFFFFFFFF
	};

	/** \return Bit-flags denoting supported preview lighting types. */
	virtual UINT PreviewLightingFlags(void) const = 0;

	class ICustomUserData : public CRhRdkObject
	{
	public:
		virtual ~ICustomUserData() { }

		virtual const UUID& PlugInId() const = 0;
		virtual const UUID& CustomUserDataId() const = 0;
		virtual void Write(CRhRdkVariant& v) const = 0;
		virtual void Read(const CRhRdkVariant& v) = 0;
	};

	class RHRDK_SDK CCustomUserDataFactory : public CRhRdkExtension
	{
	public:
		virtual UUID InterfaceId(void) const final override;
		virtual UUID CustomUserDataId(void) const = 0;
		virtual CRhRdkContent::ICustomUserData* NewCustomUserData(void) const = 0;
	};

	virtual bool AttachCustomUserData(ICustomUserData* data);
	virtual const ICustomUserData* CustomUserData(const UUID& uuidDataId) const;
	virtual bool DetachCustomUserData(ICustomUserData* pUD);

	enum GDS_Flags : UINT_PTR // Flags for GetDebugString().
	{
		gds_Normal                 = 0x00000, // Normal use.
		gds_KindPrefix             = 0x00001, // Kind abbreviation is output first (in parentheses).
		gds_UppercasePropNames     = 0x00002, // Property names are output in uppercase.
		gds_AbbreviatePropNames    = 0x00004, // Property names are abbreviated.
		gds_NameAndIdFirst         = 0x00008, // Name and id are output before other properties.
		gds_ShortUuids             = 0x00010, // UUIDs are truncated to 4 characters. Otherwise they are truncated to 13 characters.
		gds_SuppressDocumentId     = 0x00020, // Supresses output of document id.
		gds_SuppressGroupId        = 0x00040, // Supresses output of group id.
		gds_SuppressKind           = 0x00080, // Supresses output of kind.
		gds_SuppressTags           = 0x00100, // Supresses output of tags.
		gds_SuppressUsage          = 0x00200, // Supresses output of usage info.
		gds_SuppressSerialNumber   = 0x00400, // Supresses output of serial number.
		gds_SuppressCRCs           = 0x00800, // Supresses output of material and render CRCs.
		gds_SuppressChildSlotName  = 0x01000, // Supresses output of child slot name.
		gds_SuppressRegistration   = 0x02000, // Supresses output of registration info.
		gds_SuppressAttributes     = 0x04000, // Supresses output of attributes.
		gds_SuppressCustomUserData = 0x08000, // Supresses output of custom user data.
		gds_SuppressTextureProxy   = 0x10000, // Supresses output of texture proxy info.
	};

	/** Get debugging information as a string.
		\param sep is a separator (e.g., L", " or L"\n").
		\param flags is a flag word (see above). */
	virtual ON_wString GetDebugString(const wchar_t* sep, UINT_PTR flags) const;

	/** Display debugging information in the debug output window. */
	virtual void DisplayDebugString(const wchar_t* wszTitle=L"") const;

	/** \return \e true if the content or any of its children uses per-object properties, else \e false. */
	virtual bool IsPerObject(void) const;

	/** \return \e true if the content is actually a V4 material (AKA 'proxy'), else \e false.
		This is for RDK internal use only and should not be overridden by clients. */
	virtual bool IsV4Material(void) const;

	/** \return \e true if the content hierarchy is auto-deleted when not in use, else \e false. */
	virtual bool AutoDelete(void) const;

	/** Set if the content hierarchy should be auto-deleted when not in use. */
	virtual void SetAutoDelete(bool bAutoDelete);

	/** This is an advanced override that can be used to detect if a top-level content is being
		registered with a document, or a child is being attached to a document-registered parent.
		It is called in the middle of the registration process so your override must not do anything
		that relies on the 'document residence' status of the content. */
	virtual void OnDocumentRegister(void) const;

	/** This is an advanced override that can be used to detect if a top-level content is being
		unregistered from a document, or a child is being detached from a document-registered parent.
		It is called in the middle of the unregistration process so your override must not do anything
		that relies on the 'document residence' status of the content. */
	virtual void OnDocumentUnregister(void) const;

	/** Returns \e true if the content is one of the basic types that ship with Rhino.
		Basic types can be simulated without  any loss of fidelity. */
	virtual bool IsBasicType(void) const;

	/** Register the content in the global content registry. This is an advanced method which
		can be called after modal editing if the content is going to be used without attaching
		to a document. Modally edited contents are unregistered when returned from the editor.
		\param bRegister is \e true to register the content, \e false to unregister it.
		\return \e true if successful, else \e false. */
	virtual bool GlobalRegister(bool bRegister=true);

public:
	/** \internal For RDK internal use only. Do not implement this method in external plug-ins. */
	virtual class CRhRdkPreviewGeometry* NewPreviewGeometry(void) const = 0;

	/** \internal For RDK internal use only. Do not implement this method in external plug-ins. */
	virtual class CRhRdkPreviewBackground* NewPreviewBackground(void) const = 0;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual bool IsBuiltIn(void) const;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual bool IsSurrogate(void) const;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual ON_wString LibraryFileName(void) const;

	/** \return the number of times the content is in use.
		Contents can be used by objects, layers or the Ground Plane. */
	virtual int UseCount(void) const;

	/** Obsolete. Not implemented. */
	RDK_DEPRECATED virtual void ResetUseCount(void) const;

	/** Obsolete. Not implemented. */
	RDK_DEPRECATED virtual void IncrementUseCount(void) const;

	/** Obsolete. Not implemented. */
	RDK_DEPRECATED virtual void DecrementUseCount(void) const;

	/** \internal For RDK internal use only. */
	virtual bool IsRhinoObjectSelected(void) const;

	/** \internal For RDK internal use only. */
	virtual void SetIsRhinoObjectSelected(bool bSelected) const;

	/** \internal For RDK internal use only. */
	virtual const UUID& GroupId(void) const;

	/** \internal For RDK internal use only. */
	virtual void SetGroupId(const UUID& uuidGroup) const;

	/** \internal For RDK internal use only. */
	virtual bool DuplicationInProgress(void) const;

	/** \return A semicolon-delimited string of full paths to files used by the content and its children. 
		 The default implementation recursively calls EmbeddedFiles on this content and its children and 
		 collates the results. */
	virtual ON_wString EmbeddedFilesRecursive(void) const;

	/** \internal For RDK internal use only. */
	virtual bool SaveToLibraryFile_Internal(const wchar_t* wszFullPath, const IRhRdkPreviewSceneServer* pSceneServer, bool bEmbedFiles) const;

	/** Gives the content an opportunity to find files relative to the loaded content file (RMTL etc)
		immediately after loading. Currently implemented through EVF "ResolveRelativePaths".
		There is no need to call Changed() or use Undo when modifying the content since it has
		not yet been processed in any way since loading. Implementation must recurse into children. */
	virtual void ResolveRelativePaths(const wchar_t* wszPathToFile);

	/** \internal for RDK internal use only. */
	virtual void ResolveURLPaths(bool bForceDownload = false);

	/** \internal For RDK internal use only. */
	virtual void ConvertMetersToUnits(void);

	/** \internal For RDK internal use only. */
	virtual void ConvertUnitsToMeters(void);

	/** \internal For RDK internal use only. */
	virtual ON_wString DragText(const UUID& uuidSource) const;

	/** \internal For RDK internal use only. */
	ON_wString DragTextEx(const UUID& uuidSource, int version, void*) const;

	/** Obsolete. Not implemented. */
	RDK_DEPRECATED virtual void SetIsReference(bool b);

	/** \internal For RDK internal use only. */
	virtual void SetInstanceNameImpl(const wchar_t* wszName, bool bRenameEvents);

	/** \internal For RDK internal use only. [SDK_UNFREEZE] */
	virtual_su void SetInstanceNameImpl(const wchar_t* wszName, bool bRenameEvents, bool bEnsureNameUnique);

	/** Faster version of MakeCopy(). You can call this if you are willing to implement copying for your content's state.
		This method takes care of copying base state, any fields, and children. If your content uses fields exclusively
		to store its state, you need to do nothing else after calling it (but see VerifyCopy() below).
		An example use would be to override the MakeCopy() function as follows:
		\verbatim
		const CRhRdkContent* CMyContent::MakeCopy(CopyMethods m) const
		{
			CMyContent* pCopy = static_cast<CMyContent*>(FastMakeCopy(m));
			if (nullptr != pCopy)
			{
				// Do any extra member copying here.

				#ifdef _DEBUG
				VerifyCopy(pCopy); // Asserts that FastMakeCopy() and MakeCopy() give the same render CRC.
				#endif
			}

			// IMPORTANT: Do not call the base class.

			return pCopy;
		}
		\endverbatim
	*/
	virtual CRhRdkContent* FastMakeCopy(CopyMethods m) const;

	/** Call this when your override of MakeCopy() calls FastMakeCopy(). \see FastMakeCopy(). */
	virtual void VerifyCopy(const CRhRdkContent* pCopy) const;

	/** Optionally override this method to find out when a content field changes.
		If you override this method you \e must call the base class.
		\param field is the field that changed.
		\param vOldValue is the old value of the field.
		\param cc is the context of the change.
		\param pReserved is reserved for future use. */
	virtual void OnFieldChanged(const class CRhRdkContentField& field, const CRhRdkVariant& vOldValue, RhRdkChangeContext cc, void* pReserved);

	/** Optionally override this method to return the user-friendly localized name of a particular parameter.
		The default implementation tries to get the name from a field with that name. If it fails, it
		generates a name based on the parameter name and displays a warning in the debugger output window:
		"GetLocalizedParamName() generated...". This warning is an indication that you need to override this
		method and return a localized string for the given parameter name.
		\param wszParamName is the name of the parameter for which the localized string is required. */
	virtual ON_wString GetLocalizedParamName(const wchar_t* wszParamName) const;

	/** Optionally override this method to return the user-friendly English name of a particular parameter.
		The default implementation tries to get the name from a field with that name. If it fails, it
		generates a name based on the parameter name and displays a warning in the debugger output window:
		"GetEnglishParamName() generated...". This warning is an indication that you need to override this
		method and return an English string for the given parameter name.
		\param wszParamName is the name of the parameter for which the localized string is required. */
	virtual ON_wString GetEnglishParamName(const wchar_t* wszParamName) const;

	class RHRDK_SDK DumpContext
	{
	public:
		/** Implement this method to get a separator to output between each property. */
		virtual ON_wString Separator(void) const = 0;

		/** Implement this method to send the text somewhere. */
		virtual void Output(const wchar_t* text) const = 0;
	};

	/** Dump the content state to a 'dump context'.
		\param gds_flags is a flag word (see GetDebugString() for details).
		\param indent is an indent amount for formatting. */
	virtual_su void Dump(const DumpContext& dc, unsigned int gds_flags, int indent=0) const; // [SDK_UNFREEZE]

	/** \internal For RDK internal use only. */
	virtual bool AddToDocumentList(CRhRdkDocument& rdkDoc);

	/** \internal For RDK internal use only. */
	virtual bool RemoveFromDocumentList(CRhRdkDocument& rdkDoc);

	/** \internal For RDK internal use only. */
	virtual void AddUI(IRhRdkContentUI& ui) const = 0;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual void TransferUIsFrom(const CRhRdkContent& c) = 0;

	/** \internal For RDK internal use only. */
	virtual void InternalChanged(RhRdkChangeContext, const CRhRdkContent*);

	/** \internal For RDK internal use only. */
	virtual_su void Promote(void); // [SDK_UNFREEZE]

	/** \internal For RDK internal use only. */
	virtual_su bool OpenInFirstMainModelessEditor(void) const; // [SDK_UNFREEZE]

	/** \internal For RDK internal use only. */
	virtual_su CRhRdkContent* MakeCopyForPreview(void) const; // [SDK_UNFREEZE]

	/** DO NOT CALL THIS FUNCTION OUTSIDE OF THE RDK CORE.
		This function needs to be deprecated ASAP. */
	virtual_su bool GetUnderlyingInstances(OUT CRhRdkContentArray& a) const; // [SDK_UNFREEZE] uses EVF "GetUnderlyingInstances"

	/** \internal For RDK internal use only. */
	void EmergencyUnregister(void) const;

	/** \internal For RDK internal use only. */
	void CommonCtor(bool b);

protected: /* This should not be called from other classes. */

	/** Calculate the render CRC of the state that affects how the content is rendered.
		Does not include children or perform any caching. Render CRCs are now automatically
		cached by the content framework and you do not have to worry about caching. You also
		do not have to worry about recursing into children.
		This method is now only called internally by the RDK. If you have existing code
		that calls it (or CalculateRenderCRC()), please change your code to call RenderCRC() instead.
		RenderCRC() is the method that manages the cache and recursion by calling this method as needed.
		If you override this method and implement your own CRC function, you only have to compute
		the CRC of this content alone. This removes the need for the methods ChildrenRenderCRC(),
		IsCRCCached() and SetCRCCache(). These methods are now deprecated and never called.
		\see RenderCRC() */
	virtual unsigned int ComputeRenderCRC(const CRenderCRCFlags& rcrcFlags) const;

public: // These methods will be removed in the future [SDK_UNFREEZE].
	virtual const CRhRdkContent* ForDisplay(void) const;
	ON_wString SavedCSN(void) const;

public:
	/** \internal For RDK internal use only. */
	class CImpl;

private:
	CImpl* m_pImpl;

public:
	/** \internal For RDK internal use only. */
	CImpl& Imp(void) const;

public:
	// This helper calls Uninitialize() and then deletes pContent.
	static void UninitializeAndDelete(CRhRdkContent* pContent);

public:
	// Instead of std::unique_ptr, you must use CRhRdkContent::unique_ptr
	// because contents generally have to be uninitialized before deletion.
	template <class T> class unique_ptr : public std::unique_ptr<T, void(*)(CRhRdkContent*)>
	{
	public:
		unique_ptr(T* p = nullptr) : std::unique_ptr<T, void(*)(CRhRdkContent*)>(p, UninitializeAndDelete) { }
	};
};

// This prevents use of std::unique_ptr with CRhRdkContent.
template <> class std::unique_ptr<CRhRdkContent> { };

using CRhRdkContentIdArray = ON_SimpleArray<ON_UUID>;

/** \class CRhRdkInstanceIdArray

	A collection of content instance ids.

*/
class RHRDK_SDK CRhRdkInstanceIdArray final : public ON_SimpleArray<UUID>
{
public:
	bool operator == (const CRhRdkInstanceIdArray& a) const;
	bool operator =  (const CRhRdkContentIdArray& a);
};

/** \class CRhRdkContentArray

	Encapsulates a collection of pointers to contents and provides some
	useful functions that can be performed on them.

*/
class RHRDK_SDK CRhRdkContentArray : public ON_SimpleArray<const CRhRdkContent*>
{
public:
	/**	Gets the common kind shared by all contents in the collection.
		\note If the collection is empty, the method will fail.
		\param bTopLevel forces only top-level contents to be considered.
		\param kind accepts the common kind if the method returns \e true.
		\return \e true if all contents in the collection are of the same kind, else \e false.*/
	virtual bool CommonKind(bool bTopLevel, OUT CRhRdkContent::Kinds& kind) const;

	/**	Gets a localized textual description of the content kinds in the collection.
		\note If the collection is empty, the method will fail.
		\param sDescOut accepts the description (e.g., "1 material, 2 textures").
		\return \e true if successful, else \e false. */
	virtual bool ContentKindDescription(OUT ON_wString& sDescOut) const;

	/**	Gets a localized textual list of the content names in the collection.
		\note If the collection is empty, the method will fail.
		\param bAbbreviate is \e true if the list should be abbreviated using
		 text such as "and 2 others", else \e false.
		\param sNames accepts the name list (e.g., "Custom Material, Gem Material").
		\return \e true if successful, else \e false. */
	bool ContentNameList(bool bAbbreviate, OUT ON_wString& sNames) const;

	/**	Makes a localized string that can be used as an undo string.
		\note If the collection is empty, the method will fail.
		\param sIn is the base undo string (e.g., "Delete").
		\param sOut accepts the complete undo string (e.g., "Delete (Custom 001)" or "Delete (5 materials)").
		\return \e true if successful, else \e false. */
	virtual bool MakeUndoString(const ON_wString& sIn, ON_wString& sOut) const;

	/** Counts the number of contents that will be shown in a UI.
		This is usually the same as Count(), but when the array contains
		contents that have been hidden by auto-delete, it will be different. */
	virtual int CountVisible(void) const;

	/** If the array contains aggregate contents (e.g., Texture Proxies), then
		those aggregates are removed from the array and replaced with their members.
		Otherwise the function returns \e false. */
	bool Explode(void);

	/** If the array contains aggregate contents (e.g., Texture Proxies), then
		those aggregates are removed from the array and replaced with the first member of the first proxy.
		Otherwise the function returns \e false. */
	bool ExplodeFirst(void);

	/** \internal For RDK internal use only. */
	void SetIsCombineable(bool);

	/** \return \e true if the arrays contain the same content pointers. */
	bool operator == (const CRhRdkContentArray& a);

	/** \return \e true if the arrays do not contain the same content pointers. */
	bool operator != (const CRhRdkContentArray& a);

	/** Dump this content array to a 'dump context'. */
	void Dump(const CRhRdkContent::DumpContext& dc, unsigned int gds_flags) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);
};

/** \class CRhRdkContentArrayNC

	Non-const version of CRhRdkContentArray. Used for holding temporary
	non-const contents immediately after creation.

*/
class RHRDK_SDK CRhRdkContentArrayNC : public ON_SimpleArray<CRhRdkContent*>
{
public:
	/** Copy content pointers from this array to a const array. */
	void ToArrayOfConst(OUT CRhRdkContentArray& a) const;

	/** Copy content pointers from a const array to this array. */
	void FromArrayOfConst(const CRhRdkContentArray& a);

	/** Dump this content array to a 'dump context'. */
	void Dump(const CRhRdkContent::DumpContext& dc, unsigned int gds_flags) const;
};

/** \class CRhRdkEditableContentArray

	A collection of pointers to contents enhanced for use in content UIs.
	This array automatically populates itself from a data source host
	(which is usually a controller).

	Example usage from a content UI section:

	Reading:
	\code
		const auto con = Controller();
		if (!con)
			return;

		CRhRdkEditableContentArray aContent(*con, false);
		for (int i = 0; i < aContent.Count(); i++)
		{
			const auto pMaterial = dynamic_cast<const CMyMaterial*>(aContent[i]);
			if (nullptr != pMaterial)
			{
				...
			}
		}
	\endcode

	Writing:
	\code
		const auto con = Controller();
		if (!con)
			return;

		CRhRdkEditableContentArray aContent(*con, true);
		for (int i = 0; i < aContent.Count(); i++)
		{
			const auto pMaterial = dynamic_cast<const CMyMaterial*>(aContent[i]);
			if (nullptr != pMaterial)
			{
				const CRhRdkContent::Change<CMyMaterial> m(*pMaterial, RhRdkChangeContext::UI);
				m().SetSomething(value);
			}
		}
	\endcode
*/
class RHRDK_SDK CRhRdkEditableContentArray : public CRhRdkContentArray
{
public:
	CRhRdkEditableContentArray(class IRhinoUiDataSourceHost& host, bool bForWrite, bool bAutoChangeBracket=true);
	CRhRdkEditableContentArray(class CRhRdkCustomContentUI& ccui,  bool bForWrite, bool bAutoChangeBracket=true);
	virtual ~CRhRdkEditableContentArray();

	/** Change the type of all contents in the collection.
		This provides a safe way to change content types from a UI.
		\param uuidType is the new type id. */
	void ChangeContentType(const UUID& uuidType);

	/** Call this method if the user cancels the operation. */
	void Cancel(void);

	/** \internal For RDK internal use only. */
	void Initialize(void);

	/** \internal For RDK internal use only. */
	CRhRdkEditableContentArray(bool bAllowUpdatePreview, IRhinoUiDataSourceHost& host);

	/** \internal For RDK internal use only. */
	CRhRdkEditableContentArray(int, IRhinoUiDataSourceHost& host);

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK IRhRdkFileBasedContent
{
public:
	virtual ~IRhRdkFileBasedContent() { }

public:
	virtual ON_wString Filename(void) const = 0;
	virtual bool SetFilename(const wchar_t* wszFilename) = 0;

	virtual       CRhRdkContent& Content(void) = 0;
	virtual const CRhRdkContent& Content(void) const = 0;
};

class RHRDK_SDK CRhRdkContentFileWatcher
{
public:
	CRhRdkContentFileWatcher(CRhRdkContent& content, const CRhRdkContentField& fieldToWatch);
	virtual ~CRhRdkContentFileWatcher();

public:
	ON_wString FieldInternalName(void) const;

public:
	//Call these functions from your content overrides to implement the functionality.
	//The easiest way to do this is to use the IMPLEMENT_CONTENTFILEWATCHER macros above
	virtual void OnDocumentRegister(void) const;
	virtual void OnDocumentUnregister(void) const;

	//Default implementation handles the watched field and switches the watch to the new file
	//if you override OnFieldChanged in your content, you must call this explicitly before the base class.
	virtual void OnFieldChanged(const CRhRdkContentField& field, const CRhRdkVariant& vOldValue, RhRdkChangeContext cc, void* pReserved);

protected:
	//Default implementation calls Changed on the field when the file changes on the disk.
	void OnFileChanged(const wchar_t* wszFullPath) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
