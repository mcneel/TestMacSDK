
#pragma once // Public SDK header

//Obsolete for Rhino 6+
//50 is the first version for Rhino 5.0.
//#define RDK_SDK_VERSION L"4.50.0"
//#define RDK_MAJOR_VERSION 4
//#define RDK_MINOR_VERSION 50		
//#define RDK_LAST_BREAKING_CHANGE_DATE 20080418

// Note if RDK_BETA_RELEASE is 0, then this release is a public release.  Any other number means this is not
// a shippable product.
#define RDK_BETA_RELEASE  1

#include "RhRdkDefinitions.h"
#include "RhRdkObject.h"
#include "RhRdkContent.h"
#include "RhRdkDocument.h"
#include "RhRdkRenderQuality.h"
#include "RhRdkVariant.h"

class CRhRdkColor;
class CRhRdkContent;
class CRhRdkBasicMaterial;
class CRhRdkEnvironment;
class CRhRdkTexture;
class CRhRdkSimulatedEnvironment;
class CRhRdkSimulatedTexture;
class CRhRdkContentKindList;
class CRhRdkSSData;
class IRhRdkSun;
class IRhRdkMenu;
class IRhRdkActions;
class IRhRdkToolbar;
class IRhRdkContentEditor;
class IRhRdkContentFactories;
class IRhRdkCustomPlugIn;
class IRhRdkPreviewSceneServer;
class IRhRdkAutomaticUI;
class IRhRdkCustomRenderMeshManager;
class IRhRdkCompoundContentImplementors;
class IRhRdkContentList;
class IRhRdkPostEffectPlugInFactories;
class IRhRdkContentIOPlugIns;
class CRhRdkExtension;
class IRhRdkDecalSupport;
class IRhRdk_XMLSection;
class IRhRdkRegisteredPropertyManager;

/** \class IRhRdkContentChoosingInfo

	A pointer to this interface is returned from CRhRdkRenderPlugIn::GetContentChoosingInfo().
	You can use it to get extra information about the context of content choosing that is in
	progress when CRhRdkRenderPlugIn::AllowChooseContent() is called.
*/
class RHRDK_SDK IRhRdkContentChoosingInfo
{
public:
	virtual ~IRhRdkContentChoosingInfo() { }

	virtual bool IsChoosingChild(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

/** \class IRhRdkPlugInUsedContentNotifier

	This interface is used in calls to CRhRdkPlugIn::ReportUsedContent() and CRhRdkPlugIn::ChangeContentInstanceId()

	When importing a 3dm file or pasting, incoming contents are checked for usage and are only imported if
	they are being used by objects, layers or plug-ins. RDK will query your plug-in to ask if contents are
	being used by calling the ReportUsedContent() method. If your plug-in wants to make sure that contents
	are imported and not discarded, it should override this method to report each used content instance id.
	This is done by calling ReportUsedInstanceId() for each used content. During the content import, RDK
	may need to change some of the content instance ids. If it does it will notify all plug-ins. Therefore
	your plug-in should also override the ChangeContentInstanceId() method to change the recorded instance
	id of used content. Call GetInstanceIdChange() and update your record of the old instance id to be the
	new one.
*/
class RHRDK_SDK IRhRdkPlugInUsedContentNotifier
{
public:
	virtual ~IRhRdkPlugInUsedContentNotifier() { }

	/** Call this method from ReportUsedContent to get the Rhino objects for which you should check if contents are used. */
	virtual const ON_SimpleArray<const CRhinoObject*>& GetRhinoObjects(void) const = 0;

	/** Call this method from ReportUsedContent() to add each content you are using.
		\param uuidInstance is the instance id of the content. */
	virtual void ReportUsedInstanceId(const UUID& uuidInstance) = 0;

	/** Call this method from ChangeContentInstanceId() to get the old and new content instance id.
		You should then change your storage of the old id to be the new id. */
	virtual void GetInstanceIdChange(UUID& uuidOldOut, UUID& uuidNewOut) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

/** \class CRhRdkFileType

	Represents a file type. A file type can have multiple file extensions.
*/
class RHRDK_SDK CRhRdkFileType final
{
public:
	CRhRdkFileType(const wchar_t* wszType=nullptr, const wchar_t* wszDescription=nullptr);

	/** Get the file type. This is a string that uniquely identifies the type (e.g., "MYTYPE"). */
	const ON_wString& FileType(void) const;

	/** Get the localized description of the file type. e.g., Description for type "MYTYPE" could be "My file type". */
	const ON_wString& Description(void) const;

	/** Add a file extension to the type. Do not include the period. e.g., "mytype" */
	void AddFileExtension(const wchar_t* wszExtension);

	/** \return \e true if the file type is associated with a certain file extension, else \e false.
		\param wszExtension is the extension to check. */
	bool HasFileExtension(const wchar_t* wszExtension) const;

	/** \return A 'pattern' string made from the type's file extensions. e.g., "*.myt;*.mytype"
		This pattern is used in file open and save dialogs. */
	ON_wString MakeFileExtensionPattern(void) const;

private:
	ON_wString m_sFileType;
	ON_wString m_sDescription;
	ON_ClassArray<ON_wString> m_aFileExtension;
	void* m_pReserved;
};

class IRhRdkPreviewCallbacks
{
public:
	virtual ~IRhRdkPreviewCallbacks() { }

	virtual void NotifyIntermediateUpdate(const CRhinoDib& dib) const = 0;

	class ICancel
	{
	public:
		virtual ~ICancel() { }
		virtual void CancelPreview() const = 0;
		virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
	};

	// Set your cancellation callback from within CreatePreview.
	// Ensure that you set it back to null before the function exits.
	virtual void SetCancelCallback(const ICancel* cancel) = 0;
	virtual const ICancel* CancelCallback(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
};

/** \deprecated in favor of CRhRdkFileType
	\see CRhRdkFileType
*/
/*RDK_DEPRECATED*/ typedef CRhRdkFileType CRhRdkCustomRenderSaveFileType;

/** \class CRhRdkPlugIn

	Use this base class if your RDK plug-in is a Rhino plug-in but \e not a Rhino render plug-in.
	This is the main connection between your plug-in and RDK.
	You must create a subclass and implement the pure virtual functions so that
	RDK can communicate with your plug-in.

	RDK_SDK_VERSION define:

	Note, this define concerns the actual version of the SDK headers, not the version of the
	executable, which will be updated with service release even if the SDK headers do not change.

	The first number is the major version (which corresponds to the Rhino version the RDK is designed
	to run in.  If you compile with (say) version 4.0.0 of the SDK, it will
	work with all executable RDKs with the same version number.  The minor version (the second number)
	indicates minor non-breaking updates to the SDK.  If you use any of the features in the minor update, however
	it will not link to lesser versions of the RDK.

	To ensure that your plug-in works with all versions of the RDK for the platform in question, please 
	ensure that the SDK you are using is the x.0.0 version.

	The final number indicates beta/experimental version numbers. You should never use an SDK which has anything other
	than 0 in the final slot, except for beta releases of your product.

	Use the CRhRdkPlugIn::RdkVersion function to determine the actual executable version and date of the 
	RDK plug-in. This will correspond to the RDK_SDK_VERSION it was compiled with. You can also find the compile
	date of the RDK plug-in using CRhRdkPlugIn::RdkBuildDate().
*/
class RHRDK_SDK CRhRdkPlugIn : public CRhRdkObject
{
public:
	virtual ~CRhRdkPlugIn();

public:
	/** Plug-in types. */
	enum ePlugInType
	{
		rdkPlugIn       = 0,
		rdkRenderPlugIn = 1,
		force32bit_pit = 0xFFFFFFFF
	};

	/** \return plug-in type. */
	virtual ePlugInType PlugInType(void) const { return rdkPlugIn; }

	/** You must implement this method to return the identifier of your RDK plug-in.
		\return The UUID of the RDK client plug-in. */
	virtual UUID PlugInId(void) const = 0;

	/** Optionally override this method to return an icon for the plug-in. */
	virtual bool Icon(CRhinoDib& dibOut) const;

	/** You must implement this method to fill in the version numbers of the RDK SDK the plug-in was compiled with.
		This function should be implemented as follows:
		\code
		void MyRdkPlugIn::PlugInRdkVersion(int& iMajorVersion, int& iMinorVersion, int& iBetaRelease) const
		{
			iMajorVersion = RDK_MAJOR_VERSION;
			iMinorVersion = RDK_MINOR_VERSION;
			iBetaRelease  = RDK_BETA_RELEASE;
		}
		\endcode
		*/
	RDK_DEPRECATED virtual void PlugInRdkVersion(int& iMajorVersion, int& iMinorVersion, int& iBetaRelease) const;

	/** You must implement this method to return a reference to your CRhinoPlugIn derived class.
		\return A reference to your CRhinoPlugIn derived class. */
	virtual CRhinoPlugIn& RhinoPlugIn(void) const = 0;

	/** This method is obsolete and is never called. */
	RDK_DEPRECATED virtual void EnableNonModalWindows(bool) const;

	/** \return \e true if you support this custom plug-in type */
	virtual bool SupportsCustomPlugIn(const UUID &uuidInterfaceId) const;

	/** Optionally override this method to set a specific parameter in the plug-in.
		You can use this system to change parameters programatically if these methods are implemented
		for that plug-in. If you implement this method you must return \e true.
		The default implementation is a no-op that returns \e false. */
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue);

	/** Optionally override this method to get a specific parameter in the plug-in. */
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const;

	/** Initializes the plug-in. Override this method to perform additional initialization.
		<b>please be sure</b> to call the base class. 
		\return \e true if the plug-in initialized correctly, \e false if not - this will usually
		happen if the RDK the plug-in was built against is older than the RDK that is running and
		there have been breaking SDK changes in between. */
	virtual bool Initialize(void);

	/** Uninitializes the plug-in to perform clean-up when the plug-in is unloaded.
		Override this method to perform additional clean-up tasks.
		<b>please be sure</b> to call the base class. */
	virtual void Uninitialize(void);

	/** Called in response to clicking the Browse button on Rhino's Properties /
		Material / Plug-in page or the Layer Material dialog.
		The default implementation returns \e false. Override this method to provide an assignment procedure. */
	virtual bool OnAssignMaterial(HWND hwndParent, ON_Material& material);

	/** Called in response to clicking the Edit button on Rhino's Properties /
		Material / Plug-in page or the Layer Material dialog.
		The default implementation returns \e false. Override this method to provide an assignment procedure. */
	virtual bool OnEditMaterial(HWND hwndParent, ON_Material& material);

	/** This method is deprecated in favor of RegisterExtensions(). \see RegisterExtensions() */
	RDK_DEPRECATED virtual void RegisterContent(IRhRdkContentFactories&) const;

	/** This method is deprecated in favor of RegisterExtensions(). \see RegisterExtensions() */
	RDK_DEPRECATED virtual void RegisterCompoundContentImplementors(IRhRdkCompoundContentImplementors& ccis) const;

	/** This method is deprecated in favor of RegisterExtensions(). \see RegisterExtensions() */
	RDK_DEPRECATED virtual void RegisterPostEffectPlugIns(IRhRdkPostEffectPlugInFactories& pepf) const;

	/** This method is deprecated in favor of RegisterExtensions(). \see RegisterExtensions() */
	RDK_DEPRECATED virtual void RegisterCustomRenderMeshProviders(IRhRdkCustomRenderMeshManager& crmm) const;

	/** This method is deprecated in favor of RegisterExtensions(). \see RegisterExtensions() */
	RDK_DEPRECATED virtual void RegisterContentIOPlugIns(IRhRdkContentIOPlugIns& ciop) const;

	// These methods are deprecated in favor of the ones below.
	RDK_DEPRECATED virtual void RegisterCustomPlugIns(void) const;                  // Override RegisterExtensions() instead.
	RDK_DEPRECATED virtual bool AddCustomPlugIn(IRhRdkCustomPlugIn* pPlugIn) const; // Override AddExtension() instead.

	/** Optionally override this method to register extensions whose interface is defined by
		a UUID. Some of these extension types have interface files which are supplied with the
		RDK SDK (e.g., content factories) and some do not. The latter case is generally reserved
		for features which are added to the RDK after the initial SDK release and allows us to add extensions
		without breaking the SDK. To add these extensions your implementation should call AddExtension().
		\see CRhRdkExtension
		\see AddExtension()
		\see RhRdkAddExtension */
	virtual void RegisterExtensions(void) const;

	/** Adds an extension to the RDK. If you override this method, please be sure to call the base class.
		The base class implementation calls RhRdkAddExtension().
		The RDK takes ownership of the extension. If you are planning to unregister the extension
		before your plug-in is unloaded, you can store the pointer to make it possible to call
		CRhRdkExtension::Unregister(), but you should not use the pointer for any other purpose.
		\see CRhRdkExtension
		\see RegisterExtensions()
		\see RhRdkAddExtension */
	virtual bool AddExtension(CRhRdkExtension* pExtension) const;

	virtual_su bool RemoveExtension(CRhRdkExtension* pExtension) const;

	/** Optionally implement this method to satisfy license requests from RDK service providers,
		for example custom render mesh providers. */
	virtual UUID GetLicenseCode(const UUID& uuidRequestCode) const;

public:
	/** This method is obsolete. Actions have been replaced with custom tasks. \see CRhRdkCustomTask. */
	RDK_DEPRECATED virtual void AddCustomEditorActions(IRhRdkActions&, const IRhRdkContentEditor&) const;

	/** This method is obsolete. Actions have been replaced with custom tasks. \see CRhRdkCustomTask. */
	RDK_DEPRECATED virtual void UpdateCustomEditorActions(IRhRdkActions&, const IRhRdkContentEditor&) const;

	/** This method is obsolete. Actions have been replaced with custom tasks. \see CRhRdkCustomTask. */
	RDK_DEPRECATED virtual void AddCustomEditorMenu(IRhRdkMenu&, const IRhRdkContentEditor&) const;

	/** This method is obsolete. */
	RDK_DEPRECATED virtual void AddCustomEditorToolButtons(IRhRdkToolbar&, const IRhRdkContentEditor&) const;

	/** Override this function to tell RDK not to fiddle with the display when displaying
		the RDK sun. Generally you will do this if you are developing a display conduit and
		want to deal with the RDK sun yourself. 
		\return \e true if you intend to deal with the sun, else \e false (default) to get the RDK to do it. */
	virtual bool OverrideSunViewportDisplay(void) const;

	/** This is called when merging content into a document during import. If your plug-in is using content, you should
		override this method to report the content being used. \see IRhRdkPlugInUsedContentNotifier for details. */
	virtual void ReportUsedContent(IRhRdkPlugInUsedContentNotifier& ucn);

	/** This is called when merging content into a document during import. If your plug-in is using content, you should
		override this method to change your record of an instance id. \see IRhRdkPlugInUsedContentNotifier for details. */
	virtual void ChangeContentInstanceId(IRhRdkPlugInUsedContentNotifier& ucn);

	virtual_su void AddCustomUISections(IRhRdkExpandableContentUI& ui) const; // = 0; [SDK_UNFREEZE] Currently implemented using EVF(L"AddCustomUISections, (void*)&ui")

	/** If your UI has a control that doesn't care about where the focus is, have the control
		handle this message and return 1. RDK will then periodically move the focus from your
		control to Rhino's command line. Example usage:
		\code
		const UINT uQueryFocusMsg = CRhRdkPlugIn::QueryFocusMessageNumber();
		...
		BEGIN_MESSAGE_MAP(CMyButton, CButton)
			ON_REGISTERED_MESSAGE(uQueryFocusMsg, OnQueryFocus)
		END_MESSAGE_MAP()
		...
		LRESULT CMyButton::OnQueryFocus(WPARAM, LPARAM)
		{
			return 1; // Don't care about focus.
		}
		\endcode
	*/
	static UINT QueryFocusMessageNumber(void);

public:
	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual void EnsureDefaultContent(CRhinoDoc* pDoc=RhinoApp().ObsoleteActiveDoc()) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

protected:
	CRhRdkPlugIn();

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkRenderPlugIn

	Use this base class if your RDK plug-in is a Rhino render plug-in.
	This is the main connection between your renderer and RDK.
	You must create a subclass and implement the pure virtual functions so that
	RDK can communicate with your renderer.

*/
class RHRDK_SDK CRhRdkRenderPlugIn : public CRhRdkPlugIn
{
public:
	virtual ~CRhRdkRenderPlugIn();

	/** \return plug-in type. */
	virtual ePlugInType PlugInType(void) const override { return rdkRenderPlugIn; }

	/** \return \e true if you support this feature as part of RDK.
		\see The uuidFeature... definitions in RhRdkUuids.h */
	virtual bool SupportsFeature(const UUID& uuidFeature) const;

	/** \return \e true if you would like RDK to quickly create a basic content in response
		to 'Create New' commands. Return \e false if you would prefer RDK to display the
		content chooser dialog. */
	virtual bool PreferBasicContent(void) const;

	/** Override this method to provide the UUIDs of all content types that should be presented
		to the user in the types combo box or the [+] button types menu when this is the current
		renderer. The default implementation adds only RDK's built-in types. RDK automatically
		adds types in the most efficient way to minimize list length. If you override this method,
		you may call the base class first to add the built-in types, then add a separator followed
		by your own types. Or you may omit the base class call and add only chosen types yourself,
		followed by a separator and your own types. A 'More Types...' item is automatically added
		when needed by the RDK. Specify a separator by adding uuidUiContentType_Separator.
		\note You should add \e all types you would want to appear in any context, regardless of
		 their content kind. The RDK ensures that only types that make sense will actually be
		 presented to the user in a given context.
		\param aTypes accepts the types (optionally including uuidUiContentType_Separator).
		\see CRhRdkContentFactory::IsElevated */
	virtual void UiContentTypes(OUT ON_SimpleArray<UUID>& aTypes) const;

	/** Override this method to provide the UUIDs of all sections that should be displayed in
		the Render Settings tab when this is the current renderer. The default implementation
		adds all the RDK's built-in Render Settings sections. These UUIDs start with the prefix
		'uuidRenderSettingsSection'. They can be found in RhRdkUuids.h
		\param aSections accepts the UUIDs of the desired render settings sections.
		\note uuidRenderSettingsSection_CurrentRenderer is added by the RDK and cannot be removed. */
	virtual void RenderSettingsSections(OUT ON_SimpleArray<UUID>& aSections) const;

	/** Implement this method to specify other tabs that you would like to \e remove from view.
		You can remove any of the three built-in post effect tabs. If you don't want to remove any tabs,
		just implement this method as a no-op stub. If you want to remove \e all other built-in tabs,
		add ON_nil_uuid to the array.
		\see uuidTabPostEffect_Early, uuidTabPostEffect_ToneMapping, uuidTabPostEffect_Late */
	virtual_su void RenderWindowTabsToRemove(ON_SimpleArray<ON_UUID>& a) const; // [SDK_UNFREEZE]. Implemented by EVF("RenderWindowTabsToRemove").

	/** Initializes the plug-in. Override this method to perform additional initialization.
		<b>please be sure</b> to call the base class.
		\return \e true if the plug-in initialized correctly, \e false if not - this will usually
		happen if the RDK the plug-in was built against is older than the RDK that is running and
		there have been breaking SDK changes in between. */
	virtual bool Initialize(void) override;

	class RHRDK_SDK CRhRdkRenderOutputType final
	{
	public:
		CRhRdkRenderOutputType();
		CRhRdkRenderOutputType(const ON_wString sExtension, const ON_wString& sDescription);

	public:
		/** The image format specified as a file extension without the period.
			\note for JPEG you must specify 'jpg', for TIFF you must specify 'tif'.
			RDK currently supports the following types: bmp, jpg, png, tif, tga, hdr, exr, rimage. */
		ON_wString m_sFileExtension;

		/** Description of the type. e.g., L"Windows Bitmap" */
		ON_wString m_sTypeDescription;

	private:
		void* m_pReserved;
	};

	/** Implement this function to return a list of output types which your renderer can write.
		The default implementation returns the list supported by CRhinoDib - bmp, jpg, png, tif, tga.
		\see CRhRdkRenderOutputType */
	virtual void OutputTypes(ON_ClassArray<CRhRdkRenderOutputType>& outputTypes) const;

	/** You must implement this function to abort rendering (if possible). */
	virtual void AbortRender(void) = 0;

	/** Preview renderer type. \see documentation for PreviewRenderType() below. */
	enum class PreviewRenderTypes : int
	{
		None                = 0,
		ThreeSeparateImages = 1,
		SingleImage         = 2,
		Progressive         = 3,
	};

	/** Override this method to specify a different type of preview rendering. The default is
		ThreeSeparateImages, and this will work for most cases, including no preview support if you
		follow the comments for CreatePreview() below. This function is really meant to give a hint
		to the framework, and may be used to facilitate different preview systems later on. If you don't
		know what to return here, just don't override the method. */
	virtual PreviewRenderTypes PreviewRenderType(void) const;

	/** Implement this method to create the preview image that will appear
		in the content editor's thumbnail display when previewing materials and environments.
		\note This preview is the 'renderer preview' and is called 3 times with varying levels
		 of quality. If you don't want to implement this kind of preview, and are satisfied with the
		 'quick preview' generated by CreateQuickPreview(), just return \e false. If you don't support
		progressive refinement, return \e false from the first two quality levels.
		\param pNotify points to an interface for updating the preview midway through a progressive rendering.
		 The implementer gets to decide how many intermediate images to deliver. The final image should still
		 be returned when the function returns. This parameter is only valid when PreviewRenderType()
		 returns 'Progressive'. Otherwise it will be null.
		\param sizeImage is the size of the image to create, in pixels.
		\param qual is the required preview quality. \note The 'Quick' option is not supported by this method.
		\param pSceneServer is a pointer to an object describing the scene to show in the preview.
		 Note that this pointer might be null in exceptional circumstances and you must check for null before using it.
		\param dibOut accepts the preview image if the function returns \e true.
		\return \e true if successful, else \e false. */
	virtual bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality qual,
	                           const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut);

    //DO NOT ENCLOSE THESE IN RHINO_SDK_MFC - it will crash at runtime because this is a virtual function
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual HBITMAP CreatePreview(const class CSize& sizeImage, eRhRdkRenderQuality quality, const IRhRdkPreviewSceneServer* pSceneServer);

    //DO NOT ENCLOSE THESE IN RHINO_SDK_MFC - it will crash at runtime because this is a virtual function
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual HBITMAP CreatePreview(const class CSize& sizeImage, const CRhRdkTexture& texture);

	/** Implement this method to create the preview bitmap that will appear
		in the content editor's thumbnail display when previewing textures.
		\param dibOut accepts the new bitmap if the function returns \e true.
		\return \e true if successful, else \e false. */
	virtual bool CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut);

	/** Used by CreateQuickPreview to specify the reason for the call. */
	enum class QuickPreviewReason : unsigned int
	{
		ContentChanged = 0,
		ViewChanged    = 1,
		RefreshDisplay = 2,
		UpdateBitmap   = 3,
		Other          = 99
	};

	/** Optionally implement this method to change the way quick content previews are generated.
		By default, this is handled by the internal RDK OpenGL renderer and is based on the
		simulation of the content. If you want to implement an instant render based on the
		actual content parameters, or if you just think you can do a better job, override this method.
		\param size is the size of the required preview, in pixels.
		\param ss is the scene server that defines the preview scene.
		\param reason is the reason for the call. \see QuickPreviewReason
		\param dibOut accepts the preview imagery if the method returns \e true.
		\return \e true if successful, else \e false. */
	virtual bool CreateQuickPreview(const ON_2iSize& size, const IRhRdkPreviewSceneServer& ss, QuickPreviewReason reason, CRhinoDib& dibOut);

	/** Override this method and return \e true to prevent RDK from automatically updating thumbnail previews. */
	virtual bool DisableAutoUpdatePreview(void) const;

	/** Override this function and return \e false if you don't want to allow a certain content type
		to be picked from the content browser by the user during, for example, 'Create New...', etc.
		while your render engine is current.
		\return \e true to allow picking, else \e false. */
	virtual bool AllowChooseContent(const CRhRdkContent& content) const;

	/** Get extra information about content choosing. Can be called from AllowChooseContent() to
		determine, for example, if content is being chosen as top-level or as a child.
		\return a pointer to IRhRdkContentChoosingInfo if successful or nullptr if content
		choosing info is not available. Currently, content choosing info is only available
		during execution of AllowChooseContent(). */
	const IRhRdkContentChoosingInfo* GetContentChoosingInfo(UINT_PTR uReserved=0) const;

	/** You can optionally override this method to return a pointer to an implementation
		of IRhRdkDecalSupport. The default returns nullptr (not implemented). */
	virtual const IRhRdkDecalSupport* DecalSupport(void) const;

	/** You can optionally override this method to add custom sections to the Render Settings dialog.
		You should add new instances of your sections to the array as follows:
		\code AFX_MANAGE_STATE(AfxGetStaticModuleState());
		      aSections.Append(new CMySection(m)); \endcode
		\note The Render Settings dialog can be shown in a modeless or modal context.
		\param m is the modality of the Render Settings dialog. See note above.
		\param aSections is the array to append your custom dialogs to.
		\note You \e must call the base class \e after adding your sections. */
	virtual void AddCustomRenderSettingsSections(RhRdkUiModalities m, ON_SimpleArray<IRhinoUiSection*>& aSections) const;

	/** You can optionally override this method to add custom sections to the Sun dialog.
		You should add new instances of your sections to the array as follows:
		\code AFX_MANAGE_STATE(AfxGetStaticModuleState());
		      aSections.Append(new CMySection(m)); \endcode
		\note The Sun dialog can be shown in a modeless, semi-modal or modal context.
		\param m is the modality of the Sun dialog. See note above.
		\param aSections is the array to append your custom dialogs to.
		\note You \e must call the base class \e after adding your sections. */
	virtual void AddCustomSunSections(RhRdkUiModalities m, ON_SimpleArray<IRhinoUiSection*>& aSections) const;

	/** You can optionally override this method to add custom 'render save' file types to an array.
		These file types are used by the render window when the user chooses Save As. They will then
		be provided as additional file types after the default types such as BMP and JPEG.
		\param aFileType is the array to which you should add your custom file types.
		\see SaveCustomRenderFile() */
	virtual void GetCustomRenderSaveFileTypes(ON_ClassArray<CRhRdkFileType>& aFileType) const;

	/** You can optionally override this method to save your custom 'render save' file types.
		This method is called when the user does Save As from a render window and chooses one
		of the custom file types that was added by GetCustomRenderSaveFileTypes().
		\param wszFilename is the full path to the file to save to.
		\param type is the file type of the file being saved.
		\param uuidRenderSession is the render session id of the render window.
		\param bSaveAlpha is \e true if the 'Save alpha channel' check box is checked on the save file dialog.
		\see GetCustomRenderSaveFileTypes() */
	virtual bool SaveCustomRenderFile(const wchar_t* wszFilename, CRhRdkFileType& type, const UUID& uuidRenderSession, bool bSaveAlpha) const;

	/** \return \e true if this render plug-in displays 'universal' content, else \e false. */
	virtual bool DisplaysUniversalContent(void) const;

	class RENDER_ARCTIC_OPTIONS
	{
	public:
		bool bAffectMaterials = true;
		bool bAffectLights = true;
		bool bAffectGroundPlane = true;
		bool bAffectBackground = true;
		bool bReserved0 = false;
		bool bReserved1 = false;
		bool bReserved2 = false;
		bool bReserved3 = false;
	};

	/** Override this function to implement a custom version of RenderArctic for your engine. \return a valid command result. 
	    Default implementation forces render settings to all-white and calls RunScript(L"_Render"); */
	virtual CRhinoCommand::result RenderArctic(const CRhinoCommandContext& rrc, const RENDER_ARCTIC_OPTIONS&) const;

	/** Helper function. Calls EVF(L"SupportedChannels"). Override EVF() to implement this.
		Return a list of channels your renderer supports. Custom channels are allowed. */
	virtual_su void SupportedChannels(OUT ON_SimpleArray<UUID>& aChan) const; // [SDK_UNFREEZE]

	/** Helper function. Calls EVF(L"CustomChannelName"). Override EVF() to implement this.
		Return the localized name of your custom channel. */
	virtual_su ON_wString CustomChannelName(const UUID& uuidChan) const; // [SDK_UNFREEZE]

	/** Helper function. Calls EVF(L"InitialChannelToDisplay"). Override EVF() to implement this.
		Return the initial channel to display in the render window.  By default, IRhRdkRenderWíndow::chanRGBA. (pvData = UUID*) */
	virtual_su ON_UUID InitialChannelToDisplay(void) const; // [SDK_UNFREEZE]

	/** Helper function. Calls EVF(L"IsTextureSupported"). Override EVF() to implement this.
		Returns true if this renderer can render the texture natively without needing it to be baked into a bitmap, false otherwise.
		By default, returns false for all textures. */
	virtual_su bool IsTextureSupported(const CRhRdkTexture& texture) const; // [SDK_UNFREEZE]

	/* EVF example:
	\code
	static const UUID MyCustomChannel = { ... };

	void* CMyRdkPlugIn::EVF(const wchar_t* wszFunc, void* pvData)
	{
		if (0 == _wcsicmp(L"SupportedChannels", wszFunc))
		{
			auto& a = *static_cast<ON_SimpleArray<UUID>*>(pvData);
			a.Append(IRhRdkRenderWindow::chanRGBA);
			a.Append(MyCustomChannel);
			return this;
		}
		else
		if (0 == _wcsicmp(L"CustomChannelName", wszFunc))
		{
			const auto& uuid = *static_cast<UUID*>(pvData);
			if (MyCustomChannel == uuid)
				return L"My custom channel";
			return L"";
		}
		else
		if (0 == _wcsicmp(L"InitialChannelToDisplay", wszFunc))
		{
			auto& uuid = *(UUID*)pvData;
			uuid = IRhRdkRenderWindow::chanRGB;
			return this;
		}

		return __super::EVF(wszFunc, pvData);
	}
	\endcode */

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

	/** \internal For RDK internal use only. */
	bool CreatePreviewImplemented(void) const;

	/** \internal For RDK internal use only. */
	void SetCreatePreviewImplemented(bool b);

protected:
	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual void EnsureDefaultContent(CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc()) const override;

	/** You can optionally override this method to create extra default content for your renderer
		in addition to any content in the default content folder. If you override this method,
		<b>please be sure</b> to call the base class. */
	virtual void CreateDefaultContent(CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc()) const;

protected:
	CRhRdkRenderPlugIn();

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

/** Global helper functions. */

/** RDK may fail to load if something has gone wrong during installation. You can call this
	method if you want to be certain that RDK is actually available before using it.
	\return \e true if RDK is available and working properly, else \e false. */
RHRDK_SDK bool RhRdkIsAvailable(void);

/** Returns the RDK_SDK_VERSION this RDK was built with. */
//RHRDK_SDK ON_wString RhRdkVersion(void);

/** Returns the build date of this RDK - implemented as return __DATE__; */
RHRDK_SDK ON_wString RhRdkBuildDate(void);

/** Returns the RDK_MAJOR_VERSION this RDK was built with. */
//RHRDK_SDK int RhRdkMajorVersion(void);

/** Returns the RDK_MINOR_VERSION this RDK was built with. */
//RHRDK_SDK int RhRdkMinorVersion(void);

/** Returns the RDK_BETA_RELEASE this RDK was built with. */
//RHRDK_SDK int RhRdkBetaRelease(void);

/** Get a material shader for an object. */
RHRDK_SDK void* RhRdkObjectMaterialShader(const CRhinoDoc& doc, const UUID& uuidObject, const UUID& uuidRenderEngine, void* pvData=nullptr);

/** Deprecated version of the above - not document-aware. */
RDK_DEPRECATED RHRDK_SDK void* RhRdkObjectMaterialShader(const UUID& uuidObject, const UUID& uuidRenderEngine, void* pvData=nullptr);

/** Get a material shader for an object. */
RHRDK_SDK void* RhRdkObjectMaterialShader(const CRhinoObject* pObject, const UUID& uuidRenderEngine, void* pvData=nullptr);

///** If 'prefer native renderer' is checked and the native renderer is available, // TODO:
///	return that render plug-in. Otherwise return the current render plug-in. */
///RHRDK_SDK CRhRdkRenderPlugIn* RhRdkFindSuitableRenderPlugIn(const UUID& uuidNativeRenderEngine);

/** Get the RDK current render engine id. */
//Removed - use RhinoApp().GetDefaultRenderApp()
//RHRDK_SDK UUID RhRdkCurrentRenderEngineId(void);

RHRDK_SDK CRhRdkRenderPlugIn* RhRdkFindCurrentRenderPlugIn(void);

/** This function is deprecated in favor of the one below. */
RDK_DEPRECATED RHRDK_SDK CRhRdkContent* RhRdkFindContentInstance(const UUID& uuidInstance);

/** Find a content by its instance id. Top-level contents that are attached to the
	specified document will be checked as will their children and any temporary non-document
	contents that happen to be on the stack or the heap. Cloned contents will not be found.
	\param pDoc identifies the document to look in. If not nullptr, this function is the same as
	 calling CRhRdkDocument::FindContentInstance(). If nullptr, the document check will be skipped
	 but the function will still check for non-document contents.
	\param uuidInstance is the instance id of the content to find.
	\return A pointer to the content or nullptr if not found. */
RHRDK_SDK const CRhRdkContent* RhRdkFindContentInstance(const CRhinoDoc* pDoc, const UUID& uuidInstance);

/** Call this method to open one or more contents in a modal (AKA 'pop-up') editor.
	\param aContentIn is an array of pointers to contents.
	\param aContentOut accepts the edited versions of the contents if the function returns \e true.
	 These contents will be owned by the caller.
	\note If there is only one content in the array, the editing is done by calling CRhRdkContent::Edit().
	 This gives the content a chance to override the editing procedure. If there is more than one content
	 in the array, the editing is done entirely by the RDK multiple-selection modal editing system. If the
	 original contents are in a document, they will remain there, and the edited ones will be 'free-floating'.
	 Therefore it is the caller's responsibility to do any replacement in the document if required.
	\return \e true if the user clicked OK, \e false if cancelled or failed. */
RHRDK_SDK bool RhRdkModalEditContent(const CRhRdkContentArray& aContentIn, OUT CRhRdkContentArrayNC& aContentOut);

/** This is the same as the function above but it takes non-const contents. */
RHRDK_SDK bool RhRdkModalEditContent(const CRhRdkContentArrayNC& aContentIn, OUT CRhRdkContentArrayNC& aContentOut);

/** Display the standard modal color picker dialog.
	\param colInOut is the initial color to set the picker to and also accepts the user's choice.
	\param pParent is the parent window of the color picker.
	\param bUseAlpha specifies if the color picker should allow changes to the alpha channel or not.
	\return \e true if a color was picked, \e false if the user cancelled the picker dialog. */
RHRDK_SDK bool RhRdkShowColorPicker(CRhRdkColor& colInOut, HWND pParent, bool bUseAlpha);

/** Display the standard modal color picker dialog with support for custom colors.
	\param colInOut is the initial color to set the picker to and also accepts the user's choice.
	\param hWndParent is the parent window of the color picker.
	\param bUseAlpha specifies if the color picker should allow changes to the alpha channel or not.
	\param hWndMessage is the window the picker sends messages to (usually a color button).
	\param pCustomNamedColors is a pointer to an array of custom colors.
	\return \e true if a color was picked, \e false if the user cancelled the picker dialog. */
RHRDK_SDK bool RhRdkShowColorPickerEx(CRhRdkColor& colInOut, HWND hWndParent, bool bUseAlpha, HWND hWndMessage=NULL, const ON_ClassArray<CRhinoNamedColor>* pCustomNamedColors=nullptr);

/** \return A reference to RDK's content factory collection. */
RDK_DEPRECATED RHRDK_SDK IRhRdkContentFactories& RhRdkContentFactories(void);
RHRDK_SDK IRhRdkContentFactoriesEx& RhRdkContentFactoriesEx(void);

RDK_DEPRECATED RHRDK_SDK bool RhRdkShowIncompatibleContent(CRhRdkContent::Kinds kind);
RDK_DEPRECATED RHRDK_SDK void RhRdkSetShowIncompatibleContent(CRhRdkContent::Kinds kind, bool bShow);

/** Deprecated; \see CRhRdkDocument::ShowContentInstanceBrowser(). */
RDK_DEPRECATED RHRDK_SDK bool RhRdkChooseContentEx(UUID& uuidInstance, const CRhRdkContentKindList& kinds, UINT uFlags, CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Deprecated; \see CRhRdkDocument::ShowContentInstanceBrowser(). */
RDK_DEPRECATED RHRDK_SDK bool RhRdkChooseContent(UUID& uuidInstance, const CRhRdkContentKindList& kinds, CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** \return A reference to RDK's custom render mesh manager. */
RDK_DEPRECATED RHRDK_SDK IRhRdkCustomRenderMeshManager& RhRdkCustomRenderMeshManager(void);

/// <summary>
/// Returns a reference to the new Rhino 8 onwards custom render mesh manager.  Replaces the deprecated RhRdkCustomRenderMeshManager function.
/// </summary>
/// <returns>Returns a reference to the new Rhino 8 onwards custom render mesh manager.</returns>
RHRDK_SDK RhRdk::CustomRenderMeshes::IManager& RhRdkCustomRenderMeshManager2(void);

/** Deprecated; \see CRhRdkEnvironment::CurrentEnvironment(CRhRdkEnvironment::eUsage) */
RDK_DEPRECATED RHRDK_SDK UUID RhRdkCurrentDocumentContentInstanceId(CRhRdkContent::Kinds kind, CRhinoDoc* pDoc=RhinoApp().ObsoleteActiveDoc());

/** Deprecated; \see CRhRdkDocument::SetCurrentEnvironment(CRhRdkEnvironment::eUsage). */
RDK_DEPRECATED RHRDK_SDK bool RhRdkSetCurrentDocumentContentInstanceId(CRhRdkContent::Kinds kind, const UUID& uuidInstance, CRhinoDoc* pDoc=RhinoApp().ObsoleteActiveDoc());

/** Gets the RDK instance Id of a material \see ON_Material::RdkMaterialInstanceId. Does additional checking though - 
    returns the default material if applicable, and also checks to see if the material is in the document, returning ON_nil_uuid if not. */
RHRDK_SDK UUID RhRdkMaterialInstanceId(const CRhinoDoc& doc, const ON_Material* pMaterial);

/** Deprecated; not document-aware. */
RDK_DEPRECATED RHRDK_SDK UUID RhRdkMaterialInstanceId(const ON_Material* pMaterial);

/** Deprecated; \see ON_Material::SetRdkMaterialInstanceId. */
RDK_DEPRECATED RHRDK_SDK void RhRdkSetMaterialInstanceId(const UUID& uuidInstance, int iMaterialIndex, CRhinoDoc& doc);

/** Deprecated version of the above. */
RDK_DEPRECATED RHRDK_SDK void RhRdkSetMaterialInstanceId(const UUID& uuidInstanceId, int iMaterialIndex, CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Add an extension to the RDK.
	\param pExtension is a pointer to a new instance of your extension.
	 The RDK takes ownership of the extension. If you are planning to unregister the extension
	 before your plug-in is unloaded, you can store the pointer to make it possible to call
	 CRhRdkExtension::Unregister(), but you should not use the pointer for any other purpose.
	\see CRhRdkExtension.
	\see CRhRdkPlugIn::RegisterExtensions().
	\see CRhRdkPlugIn::AddExtension(). */
RHRDK_SDK bool RhRdkAddExtension(CRhRdkExtension* pExtension);
RHRDK_SDK bool RhRdkRemoveExtension(CRhRdkExtension* pExtension);

/** Remove all extensions registered by given client UUID.
	\param uuidClient is the UUID for the client whose registered extensions should be unregistered. */
RHRDK_SDK void RhRdkUnregisterPlugInExtensions(const UUID& uuidClient);

/** Queries whether or not the specified thumbnail editor is visible.
	\return \e true if visible, else \e false. */
RHRDK_SDK bool RhRdkIsThumbnailEditorVisible(const CRhinoDoc& doc, CRhRdkContent::Kinds kind);

/** NOT IMPLEMENTED */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsContentBrowserDockBarVisible(void);

/** NOT IMPLEMENTED */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsSunDockBarVisible(void);

/** NOT IMPLEMENTED */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsViewDockBarVisible(void);

/** Queries whether or not the Safe Frame is visible for a certain document.
	Deprecated in favor of doc.Properties().RenderSettings().SafeFrame().On().
	\return \e true if visible, else \e false. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsSafeFrameVisible(CRhinoDoc& doc);

/** Deprecated version of the above. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsSafeFrameVisible(CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Queries whether or not the Ground Plane is visible for a certain document.
	Deprecated in favor of doc.Properties().RenderSettings().GroundPlane().On().
	\return \e true if visible, else \e false. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsGroundPlaneVisible(CRhinoDoc& doc);

/** Deprecated version of the above. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsGroundPlaneVisible(CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Prefer CRhRdkMaterial::FromOnMaterial instead - support for physically based
    Create a new basic material from an ON_Material.
	\param mat is the material to create the basic material from.
	\param pDocAssoc is a document to associate the basic material with.
	\note This does not attach the material to the document.
	\return A pointer to the basic material. Never null. */
RHRDK_SDK CRhRdkBasicMaterial* RhRdkNewBasicMaterial(const ON_Material& mat, const CRhinoDoc* pDocAssoc);

RDK_DEPRECATED RHRDK_SDK CRhRdkBasicMaterial* RhRdkNewBasicMaterial(const ON_Material& mat);

/** Create a new basic environment.
	\return A pointer to the basic environment. Never null. */
RHRDK_SDK CRhRdkEnvironment* RhRdkNewBasicEnvironment(const CRhinoDoc* pDocAssoc);

RDK_DEPRECATED RHRDK_SDK CRhRdkEnvironment* RhRdkNewBasicEnvironment(void);

/** Create a new basic environment from a CRhRdkSimulatedEnvironment.
	\param env is the environment to create the basic environment from.
	\return A pointer to the basic environment. Never null. */
RHRDK_SDK CRhRdkEnvironment* RhRdkNewBasicEnvironment(const CRhRdkSimulatedEnvironment& env, const CRhinoDoc* pDocAssoc);

RDK_DEPRECATED RHRDK_SDK CRhRdkEnvironment* RhRdkNewBasicEnvironment(const CRhRdkSimulatedEnvironment& env);

/** Deprecated in favor of RhRdkNewBitmapTexture() */
RDK_DEPRECATED RHRDK_SDK CRhRdkTexture* RhRdkNewBasicTexture(const CRhRdkSimulatedTexture& tex);

/** Create a new bitmap texture from a CRhRdkSimulatedTexture.
	\param tex is the texture to create the bitmap texture from.
	\return A pointer to the bitmap texture. Never null. */
RHRDK_SDK CRhRdkTexture* RhRdkNewBitmapTexture(const CRhRdkSimulatedTexture& tex, const CRhinoDoc* pDocAssoc, bool bSimpleBitmap = false);

RDK_DEPRECATED RHRDK_SDK CRhRdkTexture* RhRdkNewBitmapTexture(const CRhRdkSimulatedTexture& tex, bool bSimpleBitmap=false);

/** Create a new texture from a dib.
	\param pDib is a pointer to a dib to create the texture from.
	\param bShared determines whether or not ownership of the dib is passed to RDK. If bShared is \e false,
	 you must delete pDib at some convenient future time. If bShared is \e true, RDK will delete the dib
	 when the texture is deleted. You can use this parameter to share bitmaps between textures.
	\param bAllowSimulation determines whether simulation of the texture into a temporary bitmap is allowed.
	\return A pointer to the texture. Never null. */
RHRDK_SDK CRhRdkTexture* RhRdkNewDibTexture(CRhinoDib* pDib, const CRhinoDoc* pDocAssoc, bool bShared = false, bool bAllowSimulation = true);

RHRDK_SDK CRhRdkTexture* RhRdkNewDibTexture(std::shared_ptr<CRhinoDib> pDib, const CRhinoDoc* pDocAssoc, bool bAllowSimulation = true);

/** Deprecated in favor of the above with the document association. NOT IMPLEMENTED. */
RDK_DEPRECATED RHRDK_SDK CRhRdkTexture* RhRdkNewDibTexture(CRhinoDib* pDib, bool bShared=false, bool bAllowSimulation=true);

RHRDK_SDK CRhRdkBasicMaterial* RhRdkNewDisplayAttributeMaterial(const ON_Material& mat, const CRhinoDoc* pDocAssoc);

/** Creates a new basic material and attaches it to the document.
	\param doc is the document.
	\param mat is the material whose properties are used to create the basic material. This
	 is also set as an RDK material by setting its RDK material instance id and plug-in id.
	\param bReference is \e true if the material is a reference material, else \e false.
	\note This function is provided mainly for use by import plug-ins. It allows such plug-ins
	 to add RDK materials with the addition of a single line of code. */
RHRDK_SDK CRhRdkMaterial* RhRdkCreateImportedMaterial(CRhinoDoc& doc, ON_Material& mat, bool bReference);

/** Get an interface to an automatic UI. The caller shall delete the interface
	when it is no longer required.
	\param f is the UI framework to use.
	\param parent is the parent window which <b>must not be nullptr</b>.
	\param style specifies the visual style of the UI.
	\return Interface to automatic UI. This will be nullptr only if RDK or plug-in is
	 not correctly initialized. */
RHRDK_SDK IRhRdkAutomaticUI* RhRdkNewAutomaticUI(RhRdkUiFrameworks f, HWND parent, IRhRdkAutomaticUI::Styles style);

#define rdksccflags_hide_new_tab      CRhRdkDocument::ccbu_hide_new_tab
#define rdksccflags_hide_existing_tab CRhRdkDocument::ccbu_hide_existing_tab

/** Deprecated; \see CRhRdkDocument::CreateContentByType(). */
RDK_DEPRECATED RHRDK_SDK CRhRdkContent* RhRdkCreateContentByType(const UUID& uuidType, CRhRdkContent* pParent=nullptr, const wchar_t* wszChildSlotName=nullptr,
                                                                 UINT uFlags=0, CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Deprecated; \see CRhRdkDocument::CreateContentByUser(). */
RDK_DEPRECATED RHRDK_SDK CRhRdkContent* RhRdkCreateContentByUser(const UUID& uuidDefaultType, const UUID& uuidDefaultInstance,
                                                                 const CRhRdkContentKindList& allowedKinds,
                                                                 UINT ccbu_flags=0, CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());
#define rdkccc_AllowNone     CRhRdkDocument::ccc_AllowNone
#define rdkccc_AutoEdit		 CRhRdkDocument::ccc_AutoEdit
#define rdkccc_Preserve		 CRhRdkDocument::ccc_Preserve
#define rdkccc_HarvestMask	 CRhRdkDocument::ccc_HarvestMask
#define rdkccc_HarvestUseOpt CRhRdkDocument::ccc_HarvestUseOpt
#define rdkccc_HarvestAlways CRhRdkDocument::ccc_HarvestAlways
#define rdkccc_HarvestNever	 CRhRdkDocument::ccc_HarvestNever

/** Deprecated; \see CRhRdkDocument::ChangeChildContent(). */
RDK_DEPRECATED RHRDK_SDK bool RhRdkChangeChildContent(CRhRdkContent& parentContent, const wchar_t* wszChildSlotName,
                                                      const CRhRdkContentKindList& allowedKinds,
                                                      UINT uFlags=rdkccc_AllowNone|rdkccc_AutoEdit);

/** Change the type of a content. This deletes the content and creates a replacement
	of the specified type using the Renderer Support option to decide about harvesting.
	\param pOldContent is the old content which is deleted; this pointer is then set to nullptr.
	This must be an RDK-owned content that is attached to a document (either top-level or child).
	\param uuidNewType is the type of content to replace pOldContent with.
	\return A pointer to new content if successful, else nullptr. */
RHRDK_SDK const CRhRdkContent* RhRdkChangeContentType(const CRhRdkContent*& pOldContent, const UUID& uuidNewType);

/** Change the type of a content. This deletes the content and creates a replacement
	of the specified type allowing the caller to decide about harvesting.
	\param pOldContent is the old content which is deleted; this pointer is then set to nullptr.
	This must be an RDK-owned content that is attached to a document (either top-level or child).
	\param uuidNewType is the type of content to replace pOldContent with.
	\param bHarvest determines whether or not parameter harvesting will be performed.
	\return A pointer to new content if successful, else nullptr. */
RHRDK_SDK const CRhRdkContent* RhRdkChangeContentTypeEx(const CRhRdkContent*& pOldContent, const UUID& uuidNewType, bool bHarvest);

/** Delete a child content.
	\param parent is the content whose child is to be deleted. This must be an
	 RDK-owned content that is attached to a document (either top-level or child).
	\param wszChildSlotName is the child-slot name of the child to be deleted.
	\note If parent is not attached to a document, the method will fail.
	\return \e true if successful, else \e false. */
RHRDK_SDK bool RhRdkDeleteChildContent(CRhRdkContent& parent, const wchar_t* wszChildSlotName);

/** \return A preview scene server for previewing 3D textures.
	This is typically called by an advanced texture's override of CRhRdkContent::NewPreviewSceneServer() */
RHRDK_SDK IRhRdkPreviewSceneServer* RhRdkNew3DPreviewSceneServer(const CRhRdkTexture* pTexture, const CRhRdkSSData& data);

/** Deprecated; \see CRhRdkDocument::MaterialList() */
RDK_DEPRECATED RHRDK_SDK const IRhRdkContentList& RhRdkMaterialList(const CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Deprecated; \see CRhRdkDocument::EnvironmentList() */
RDK_DEPRECATED RHRDK_SDK const IRhRdkContentList& RhRdkEnvironmentList(const CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** Deprecated; \see CRhRdkDocument::TextureList() */
RDK_DEPRECATED RHRDK_SDK const IRhRdkContentList& RhRdkTextureList(const CRhinoDoc* pDoc= RhinoApp().ObsoleteActiveDoc());

/** \return A pointer to the content list containing a particular kind of content or nullptr if kind is 'Unset'.
	This pointer is to const because you can only read the list.
	If you want to modify it, you must use one of the other RhRdk... global functions. */
RDK_DEPRECATED RHRDK_SDK const IRhRdkContentList* RhRdkPersistentContentList(CRhRdkContent::Kinds kind, const CRhinoDoc* pDoc=RhinoApp().ObsoleteActiveDoc());

/** Call this function after changing the preview cache using IRhRdkPreviewSceneServer::SetCacheImage()
	to invalidate the thumbnail lists. */
RHRDK_SDK void RhRdkSetThumbnailListsDirty(void);

/** \return The collection of content IO plug-ins. */
RHRDK_SDK const IRhRdkContentIOPlugIns& RhRdkContentIOPlugIns(void);

enum // For RhRdkPromptForSaveImageFileParams uFlags param
{
	rdk_psi_IncludeRimage = 0x01,
	rdk_psi_IncludeHDR    = 0x02,
	rdk_psi_IncludeEXR    = 0x04,
};

/** Prompt the user for a save file name and the width, height and depth of an image to be saved.
	\param sFilenameInOut specifies the default file name and accepts the file name specified by the user.
	\param widthOut accepts the image width specified by the user.
	\param heightOut accepts the image height specified by the user.
	\param depthOut accepts the image depth specified by the user.
	\return \e true if successful or \e false if the user cancels the operation. */
RHRDK_SDK bool RhRdkPromptForSaveImageFileParams(ON_wString& sFilenameInOut, int& widthOut, int& heightOut, int& depthOut, unsigned int uFlags=0);

/** Copy similar parameters from an old to a new content.
	This is a default implementation for CRhRdkContent::HarvestData()
	although you may choose to change this method or supplement it with content-specific code.
	\param newContent is the content that has just been created.
	\param oldContent is the pre-changed content.
	\return How much data was harvested. */
RHRDK_SDK CRhRdkContent::Harvested RhRdkHarvestContentDataEx(CRhRdkContent& newContent, const CRhRdkContent& oldContent);

/** This function is deprecated in favor of RhRdkHarvestContentDataEx(). */
RHRDK_SDK void RhRdkHarvestContentData(CRhRdkContent& newContent, const CRhRdkContent& oldContent);

/** Call this function to get an object which implements IRhRdk_XMLSection for use in, for example,
	CRhRdkContent::ReadParametersFromSection() / WriteParametersToSection().
	\return a pointer to an object which implements IRhRdk_XMLSection. The client shall delete this pointer. */
RHRDK_SDK IRhRdk_XMLSection* RhRdkNewXMLSection(void);

/** This function is deprecated in favor of the one below. */
RDK_DEPRECATED RHRDK_SDK CRhRdkContent* RhRdkLoadPersistentContentFromFile(const wchar_t* wszFullPath);

/** Loads a content from a library file and attaches it to a document, hence the returned
	pointer is to a const content. Do not const-cast or attempt to delete the returned pointer.
	To make changes to the content, call its BeginChange() method.
	\see CRhRdkContent::BeginChange()
	\param doc is the document to attach the loaded content to.
	\param wszFullPath is the full path to the file to be loaded.
	\return a pointer to the loaded content or nullptr if an error occurred. */
RHRDK_SDK const CRhRdkContent* RhRdkLoadPersistentContentFromFile(const CRhinoDoc& doc, const wchar_t* wszFullPath, bool bForceDownloadTextures = false);

/** Loads a content from a library file but does not attach it to a document, hence the returned
	pointer is to a non-const content. The caller is responsible for deleting the content. If the
	file contains more than one content, the first content is returned.
	\param wszFullPath is the full path to the file to be loaded.
	\return a pointer to the loaded content or nullptr if an error occurred. */
RHRDK_SDK CRhRdkContent* RhRdkLoadContentFromFile(const wchar_t* wszFullPath, bool bForceDownloadTextures = false);

/** This function is deprecated in favor of the one below. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkAddPersistentContent(CRhRdkContent* pContent);

/** Attach a content to a document. Document contents are called 'persistent' contents.
	After successfully adding the content, any relevant UIs will be updated.
	\note This method does not record undo. If you want undo, please use CRhRdkContentUndo. \see CRhRdkContentUndo
	\return \e true if added successfully, else \e false. */
RHRDK_SDK bool RhRdkAddPersistentContent(const CRhinoDoc& doc, CRhRdkContent& c);

#define eRhRdkSccResult UINT
#define rdkscc_none     CRhRdkDocument::SctbResult::kNone
#define rdkscc_new      CRhRdkDocument::SctbResult::kNew
#define rdkscc_copy     CRhRdkDocument::SctbResult::kCopy
#define rdkscc_instance CRhRdkDocument::SctbResult::kInstance

/** Deprecated; \see CRhRdkDocument::ShowContentNewExistingBrowser() */
RDK_DEPRECATED RHRDK_SDK UINT RhRdkShowContentChooser(const UUID& uuidDefaultType, const UUID& uuidDefaultInstance, const CRhRdkContentKindList& allowedKinds,
                                                      UUID& uuidOut, UINT uFlags=0, const CRhinoDoc* pDoc = nullptr);

/** This method is deprecated. */
RDK_DEPRECATED RHRDK_SDK CRhRdkContent* RhRdkGetSelectedContent(const wchar_t* wszKind);

RDK_DEPRECATED RHRDK_SDK bool RhRdkGetVisibleThumbnailEditorKind(ON_wString& sKindOut); // OBSOLETE. Not implemented.

/** Replacement for CRhinoFileUtilities::FindFile() which also handles network shares. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkFindFile(const wchar_t* wszFullPath, ON_wString& sFoundFileOut); // Deprecated in favor of the one below.
RHRDK_SDK bool RhRdkFindFile(unsigned int doc_serial_number, const wchar_t* wszFullPath, ON_wString& sFoundFileOut);

/** Replacement for CRhinoFileUtilities::FindFile() which also handles network shares and automatic file unpacking from the bitmap table. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkFindFile(const wchar_t* wszFullPath, bool bAutoUnpackBitmaps, ON_wString& sFoundFileOut); // Deprecated in favor of the one below.
RHRDK_SDK bool RhRdkFindFile(unsigned int doc_serial_number, const wchar_t* wszFullPath, bool bAutoUnpackBitmaps, ON_wString& sFoundFileOut);

/** Helper for use in OnEraseBkgnd() to avoid flicker. */
RHRDK_SDK void RhRdkClipChildren(HWND wnd, HDC dc);

/** Determine if any texture in any open document is using the specified file for caching.
	\param wszTextureFile is the file name to check for. The extension is ignored.
	\param uFlags is for future use only.
	\return \e true if the cache file is in use, else \e false. */
RHRDK_SDK bool RhRdkIsCachedTextureFileInUse(const wchar_t* wszTextureFile, UINT uFlags=0);

RHRDK_SDK IRhRdkRegisteredPropertyManager& RhRdkRegisteredPropertiesManager(void);

RHRDK_SDK CRhRdkRenderPlugIn* FindCurrentRenderPlugIn(void);
RHRDK_SDK CRhRdkPlugIn* RhRdkPlugInFromRhinoPlugIn(CRhinoPlugIn* pRhinoPlugIn);

/** Deprecated; use IRhRdkSun::CopyFrom() */
RDK_DEPRECATED RHRDK_SDK void RhRdkCopySun(IRhRdkSun& dest, const IRhRdkSun& srce);

/** Deprecated; use IRhRdkSun::IsEqual() */
RDK_DEPRECATED RHRDK_SDK bool RhRdkIsSunEqual(const IRhRdkSun& sun1, const IRhRdkSun& sun2);

RDK_DEPRECATED RHRDK_SDK int RhRdkSetSimulatedTextureSize(int iSize);	//Returns the old texture size (deprecated - clients should not need to call this)
RHRDK_SDK int RhRdkGetSimulatedTextureSize(void);		//Returns the simulated texture size

RHRDK_SDK ON_BoundingBox RhRdkGetCRMBoundingBox(const class IRhRdkCustomRenderMeshes& meshes);

/** Get the 16 by 16 icon representing a particular content kind.
	\param kind is the content kind.
	\return A handle to the icon or nullptr if kind is invalid. */
RDK_DEPRECATED RHRDK_SDK HICON RhRdkGetContentIcon_16x16(CRhRdkContent::Kinds kind);

RHRDK_SDK void RhRdkFillOutputTypesFromSDS(ON_ClassArray<CRhRdkRenderPlugIn::CRhRdkRenderOutputType>& ots, const wchar_t* wszSDS_Ext, const wchar_t* wszSDS_Desc);

/** Create a new render session. The returned pointer should not be stored.
	If you want to store the session for later use, you should store the session's id
	and call RhRdkFindRenderSession() to look it up again later. */
RHRDK_SDK IRhRdkRenderSession* RhRdkNewRenderSession(const CRhinoDoc& doc, const UUID& uuidRenderEngine);

/** Get a pointer to a render session by render session id (or null if not found).
	The returned pointer should not be stored. */
RHRDK_SDK IRhRdkRenderSession* RhRdkFindRenderSession(const UUID& uuidRenderSession);

enum // frs_Flags for RhRdkFindRenderSession() below.
{
	frs_reusable        = 0x01, // OBSOLETE. Do not use.
	frs_most_recent     = 0x02, // The most recent suitable session will be returned.
	frs_oldest          = 0x04, // The oldest suitable session will be returned.
	frs_quiescent_ok    = 0x40, // Usually quiescent sessions are not considered; this flag overrides that (not recommended).
	frs_initializing_ok = 0x80, // Usually initializing sessions are not considered; this flag overrides that (not recommended).
};

/** Get a pointer to a render session by render engine id (or null if not found). The returned pointer should not be stored. */
RHRDK_SDK IRhRdkRenderSession* RhRdkFindRenderSession(const UUID& uuidRenderEngine, unsigned int frs_Flags);

/** Get a pointer to a render window by render session id (or null if not found). The returned pointer should not be stored. */
RHRDK_SDK IRhRdkRenderWindow* RhRdkFindRenderWindow(const UUID& uuidRenderSession);

RHRDK_SDK bool RhRdkConvertIconToDib(HICON icon, CRhinoDib& dibOut);

RHRDK_SDK HICON RhRdkConvertDibToIcon(const CRhinoDib& dib);

#if defined ON_RUNTIME_WIN
/** Convert horizontal dialog units to pixels.
	\param du is the number of dialog units.
	\param pFont specifies a font to use. If nullptr, the default GUI font is used.
	\return The number of pixels corresponding to one horizontal dialog unit using the specified font. */
RHRDK_SDK int RhRdkDialogUnitsToPixelsX(int du, HFONT font=nullptr);

/** Convert vertical dialog units to pixels.
	\param du is the number of dialog units.
	\param pFont specifies a font to use. If nullptr, the default GUI font is used.
	\return The number of pixels corresponding to one vertical dialog unit using the specified font. */
RHRDK_SDK int RhRdkDialogUnitsToPixelsY(int du, HFONT font=nullptr);
#endif

enum eRdkEditorOp // Obsolete.
{
	// Thumbnail size. Radio buttons.
	rdk_eo_Small = 1, rdk_eo_Medium = 2, rdk_eo_Large = 3, rdk_eo_Tiny = 4, 

	// Thumbnail preview geometry. Radio buttons.
	rdk_eo_Sphere  = 10, rdk_eo_Cube  = 11, rdk_eo_Cone  = 12,
	rdk_eo_Pyramid = 13, rdk_eo_Torus = 14, rdk_eo_Plane = 15,
	rdk_eo_Scene   = 16, /* Deprecated */   rdk_eo_MaterialScene = rdk_eo_Scene,

	// Thumbnail preview.
	rdk_eo_ShowLabels        = 100, // bool -- show labels toggle/on/off.
	rdk_eo_ShowUnits         = 101, // bool -- show units toggle/on/off.
	rdk_eo_UpdatePreview     = 102, // Updates selected content previews.
	rdk_eo_UpdateAllPreviews = 103, // Updates all previews.
	rdk_eo_Properties        = 104, // Opens thumbnail properties dialog.
};

/** This function is deprecated and not implemented. */
RDK_DEPRECATED RHRDK_SDK bool RhRdkExecuteEditorOperation(HWND pWnd, eRdkEditorOp op, int state=-1, UINT_PTR reserved=0);

/** \return the RDK's state serial number. This number is incremented whenever any RDK state changes. */
RHRDK_SDK unsigned int RhRdkStateSerialNumber(void);

//---------------------------------------------------------------------------------------------------------------

class RHRDK_SDK CRhRdkEnterModeBase
{
public:
	CRhRdkEnterModeBase();
	virtual ~CRhRdkEnterModeBase() = 0;

protected:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkEnterCommandLineMode : public CRhRdkEnterModeBase
{
public:
	CRhRdkEnterCommandLineMode();
	virtual ~CRhRdkEnterCommandLineMode();
};

class RHRDK_SDK CRhRdkEnterGUIMode : public CRhRdkEnterModeBase
{
public:
	CRhRdkEnterGUIMode();
	virtual ~CRhRdkEnterGUIMode();
};

class RHRDK_SDK CRhRdkEnterQuietMode : public CRhRdkEnterModeBase
{
public:
	CRhRdkEnterQuietMode();
	virtual ~CRhRdkEnterQuietMode();
};

class RHRDK_SDK CRhRdkEnterMode : public CRhRdkEnterModeBase
{
public:
	enum eMode
	{
		GUI         = 0,
		CommandLine = 1,
		Quiet       = 2,
		force32bit_mode	= 0xFFFFFFFF
	};

	CRhRdkEnterMode(eMode);
	virtual ~CRhRdkEnterMode();
};

/** \class CRhRdkDisplayUpdate

	This class is used to optimize display updates during batch operations.
	If you perform a repeated sequence of content operations such as attaching or
	detaching, deleting, etc., you should put one of these on the stack at the top
	of your function. While one or more of these objects is in scope, various
	UI elements in the RDK will not update. Instead, they will update when the
	object goes out of scope. This prevents repeated UI updates and can drastically
	improve performance when operating on a lot of contents.

	\see RhRdkIsDisplayUpdateEnabled()

*/
class RHRDK_SDK CRhRdkDisplayUpdate final
{
public:
	CRhRdkDisplayUpdate(int iNumItems=-1, bool bAllowWaitCursor=true);
	~CRhRdkDisplayUpdate();

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkDisableFileFinder final
{
public:
	CRhRdkDisableFileFinder();
	~CRhRdkDisableFileFinder();

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkSdkContentSelector

	Provides access to the selected contents in the main content editor for a particular document and content kind.

*/
class RHRDK_SDK CRhRdkSdkContentSelector
{
public:
	/** This constructor is deprecated and not implemented. Please use the other constructor. */
	RDK_DEPRECATED CRhRdkSdkContentSelector(const UUID& uuidTarget, const CRhinoDoc& doc, UINT_PTR uReserved=0);

	/**	\param doc is the document that contains the contents to be accessed.
		\param kind is the kind of contents to access.
		\param uReserved is reserved for future use and should be set to zero. */
	CRhRdkSdkContentSelector(const CRhinoDoc& doc, CRhRdkContent::Kinds kind, UINT_PTR uReserved=0);

	virtual ~CRhRdkSdkContentSelector();

	/** Unselects all the contents in the editor.
		\return \e true if successful, else \e false. */
	virtual bool UnselectAllContent(void) const;

	/** Gets the contents currently selected in the editor.
		\param aContent accepts the content instance ids.
		\return \e true if successful, else \e false. */
	virtual bool GetSelection(CRhRdkInstanceIdArray& aContent) const;

	/** Sets the content selection in the editor.
		\param aContent specifies the instance ids of the contents to select.
		\return \e true if successful, else \e false. */
	virtual bool SetSelection(const CRhRdkInstanceIdArray& aContent) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*);

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** Slow UI elements may want to call this from event handlers such as OnContentAttached, OnContentDetached.
	If it returns false, the UI should not update. Instead, it should update in response to the event
	CRhRdkEventWatcher::OnDisplayUpdateEnded()

	\see CRhRdkDisplayUpdate
*/
RHRDK_SDK bool RhRdkIsDisplayUpdateEnabled(void);

RDK_DEPRECATED RHRDK_SDK bool RhRdkIsFastTextureSimulationEnabled(void); // OBSOLETE.

RDK_DEPRECATED RHRDK_SDK bool RhRdkNeedsTextureBaking(const CRhinoObject* pObject, bool& bDoubleSided, const CRhRdkObjectAncestry* pOptionalAncestry = nullptr, const CRhRdkMaterial* pOptionalMaterial = nullptr);

enum eRhRdkBakingFunctions : ON__UINT32	//Bitfields, so you will need to cast them to ON__UINT32 to combine them
{
	kNone						= 0x00,
	kDecals						= 0x01,
	kProceduralTextures			= 0x02,
	kCustomObjectMappings		= 0x04,
	kWCSBasedMappings			= 0x08,
	kMultipleMappingChannels	= 0x10,
	kNoRepeatTextures			= 0x20,
	kAll						= 0xFFFFFFFF
};

RHRDK_SDK bool RhRdkNeedsTextureBaking(const CRhinoObject* pObject, eRhRdkBakingFunctions functions, bool& bDoubleSided, const CRhRdkObjectAncestry* pOptionalAncestry = nullptr, const CRhRdkMaterial* pOptionalMaterial = nullptr);
RHRDK_SDK bool RhRdkNeedsTextureBaking(const CRhinoObject* pObject, eRhRdkBakingFunctions functions, bool& bDoubleSided, const ON_UUID& rendererId, const CRhRdkObjectAncestry* pOptionalAncestry = nullptr, const CRhRdkMaterial* pOptionalMaterial = nullptr);
RHRDK_SDK bool RhRdkBakeTexture(const CRhinoObject* pObject, const ON_Xform& blockTransform, ON_Texture::TYPE textureChannel, bool bFront,
								const ON_2iSize & bitmapSize, ON_wString & strFileNameOut, const CRhRdkObjectAncestry* pOptionalAncestry = nullptr, const CRhRdkMaterial* pOptionalMaterial = nullptr);

RHRDK_SDK bool RhRdkBakeTextureEx(const CRhinoObject* pObject, 
	                              const ON_Xform& blockTransform, 
	                              ON_Texture::TYPE textureChannel, 
	                              bool bFront,
	                              const ON_2iSize& bitmapSize, 
	                              ON_wString& strFileNameOut,
	                              bool bAsync,
	                              const CRhRdkObjectAncestry* pOptionalAncestry = nullptr, 
	                              const CRhRdkMaterial* pOptionalMaterial = nullptr);

RHRDK_SDK bool RhRdkBakeTextureCancellable(const CRhinoObject* pObject,
	const ON_Xform& blockTransform,
	ON_Texture::TYPE textureChannel,
	bool bFront,
	const ON_2iSize& bitmapSize,
	ON_wString& strFileNameOut,
	bool bAsync,
	bool bAllowCancel,
	bool &bCancelledOut,
	const CRhRdkObjectAncestry* pOptionalAncestry = nullptr,
	const CRhRdkMaterial* pOptionalMaterial = nullptr);

RDK_DEPRECATED RHRDK_SDK bool RhRdkGetUnpackTargetFolder(ON_wString& sTargetFolderOut);

RHRDK_SDK bool RhRdkGetUnpackTargetFolder2(const CRhinoDoc* pDoc, ON_wString& sTargetFolderOut);

/** Deprecated; not implemented. Use CRhRdkDocument::TextureReport() instead. */
RDK_DEPRECATED RHRDK_SDK void RhRdkReportMissingEmbeddedFiles(CRhinoDoc& doc, bool& bAbort);

// Thumbnail SDK.

#define RDK_SDK_TNHANDLE          ON_UUID
#define RDK_SDK_INVALID_TNHANDLE  ON_nil_uuid

typedef ON_SimpleArray<RDK_SDK_TNHANDLE> CRhRdkThumbArray;

/** \class IRhRdkSelChange. A pointer to this interface is passed as the LPARAM to the
	IRhRdkThumbnails::SelChangingMessageNumber() and IRhRdkThumbnails::SelChangedMessageNumber() messages.
*/
class RHRDK_SDK IRhRdkSelChange
{
public:
	/** Get the old set of selected thumbnails. Used by both SelChanging and SelChanged messages. */
	virtual const CRhRdkThumbArray& Old(void) const = 0;

	/** Get the new set of selected thumbnails. Used by only the SelChanged message. */
	virtual const CRhRdkThumbArray& New(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkSelChange() { }
};

class RHRDK_SDK IRhRdkThumbnails
{
public:
	/** You must call this to delete the control after use. */
	virtual void Delete(void) = 0;

	/** \return The unique id of the thumbnail control. */
	virtual UUID Uuid(void) const = 0;

	/** Optionally set a document for the thumbnails to use. This is not required for modeless thumbnails
		because they find the document automatically. If you are writing a modal dialog, you may need to
		call this method and pass a special document to override the automatic default. */
	virtual void SetRdkDocument(const CRhRdkDocument& rdkDoc) = 0;

	/** Set the position and size of the control.
		\param rect is the rectangle to move to, expressed in client coordinates of the parent. */
	virtual void MoveWindow(const ON_4iRect& rect) = 0;

	/** Get the set of selected thumbnails.
		\param aThumbOut accepts the thumbnail handles of the selected thumbnails.
		\return \e true if successful, else \e false. */
	virtual bool GetSelection(CRhRdkThumbArray& aThumbOut) = 0;

	/** Select a set of thumbnails.
		\param aThumb specifies the thumbnails to select.
		\return \e true if successful, else \e false. */
	virtual bool SetSelection(const CRhRdkThumbArray& aThumb) = 0;

	/** Get the contents associated with a set of thumbnails. If the thumbnails are not displaying
		contents, the function will fail.
		\param aThumb is the array of thumbnail handles to return the contents for.
		\param aContentOut accepts the associated content instance ids.
		\return \e true if successful, else \e false. */
	virtual bool GetContentArray(const CRhRdkThumbArray& aThumb, CRhRdkInstanceIdArray& aContentOut) = 0;

	/** Specifies the metrics of a thumbnail. \see GetThumbMetrics(). */
	class RHRDK_SDK CThumbnailMetrics final
	{
	public:
		CThumbnailMetrics();

		ON_2iSize m_szThumb;    // The dimensions of the thumbnail body.
		ON_2iSize m_szBorder;   // The dimensions of the border around a thumbnail.
		ON_2iSize m_szGap;      // The dimensions of the gap between thumbnails.
		void* m_pReserved;
	};

	/** Get the metrics of a thumbnail. */
	virtual bool GetThumbnailMetrics(CThumbnailMetrics& m) const = 0;

	/** \return \e true if scroll bar is visible, else \e false. */
	virtual bool IsScrollBarVisible(void) const = 0;

	/** Orientation of the thumbnails. This determines if the thumbnails have a vertical or horizontal scroll bar. */
	enum orientation { or_vertical, or_horizontal };

	/** Get the orientation the thumbnails have when in 'grid' mode. The default is or_vertical. \see orientation. */
	virtual orientation GridModeOrientation(void) const = 0;

	/** Set the orientation the thumbnails should have when in 'grid' mode. \see orientation. */
	virtual void SetGridModeOrientation(orientation o) = 0;

	/** \return The message that the control sends to the parent window when the selection is about to change. */
	virtual UINT SelChangingMessageNumber(void) const = 0;

	/** \return The message that the control sends to the parent window when the selection has changed. */
	virtual UINT SelChangedMessageNumber(void) const = 0;

	/** \return The message that the control sends to the parent window when a thumbnail is double-clicked. */
	virtual UINT DoubleClickedMessageNumber(void) const = 0;

	/** \return The message that the control sends to the parent window when the thumbnail size is changed. */
	virtual UINT ThumbSizeChangedMessageNumber(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkThumbnails() { }
};

enum rdkct_type /** Type for RhRdkCreateThumbnails() */
{
	rdkct_persistent_content = 0, // Control displays the persistent content of the kind specified by parameter 'wsz'.
	rdkct_custom             = 9, // Control displays custom thumbnails (still to be designed).
	rdkct_force32 = 0xFFFFFFFF
};

#if defined (RHINO_SDK_MFC)
/** These functions are deprecated and not implemented. */
RDK_DEPRECATED RHRDK_SDK IRhRdkThumbnails* RhRdkCreateThumbnails(const UUID& uuid, DWORD dwStyle, const CRect& rect, HWND pParent, UINT uId,
                                           rdkct_type type, UINT uFlags, const wchar_t* wsz, const CRhinoDoc& doc, UINT_PTR uReserved=0);

RDK_DEPRECATED RHRDK_SDK IRhRdkThumbnails* RhRdkCreateThumbnails(const UUID& uuid, DWORD dwStyle, const CRect& rect, HWND pParent, UINT uId,
                                           rdkct_type type, UINT uFlags, const wchar_t* wsz, UINT_PTR uReserved=0);
#endif

template <class T> RDK_DEPRECATED bool RhRdkSupportsVariesHelper(const ON_SimpleArray<UUID>& aContentInst) { return false; }

/** \return \e true if all contents in the list are derived from class T. */
template <class T> bool RhRdkSupportsVariesHelperEx(const CRhRdkContentArray& aContent)
{
	for (int i = 0; i < aContent.Count(); i++)
	{
		if (nullptr == dynamic_cast<const T*>(aContent[i]))
			return false;
	}

	return true;
}

RHRDK_SDK void RhRdkMoveWindow(HWND hwnd, int x, int y, int cx, int cy, bool bRepaint=false, bool bRepaintNC=false);

RHRDK_SDK ON_wString RhRdkStringFromKind(CRhRdkContent::Kinds kind);

RHRDK_SDK CRhRdkContent::Kinds RhRdkKindFromString(const wchar_t* wszKind);

RHRDK_SDK bool RhRdkIsDoubleSidedMaterial(const CRhinoDoc* pDoc, const ON_Material& material);
RHRDK_SDK const ON_Material& RhRdkDoubleSidedBackMaterial(const CRhinoDoc* pDoc, const ON_Material& material);

/** Assign a material to an object via an object reference. This can assign to whole objects
	or to sub-faces of objects, depending on what the objRef refers to.
	\param doc is the document containing the material.
	\param aObject is an array of references to objects or sub-faces to assign the material to.
	\param uuidInstance is the instance id of the material.
	\note This is the simple version which might ask the user questions using message boxes.
	 For more control over this, see the other version */
RHRDK_SDK void RhRdkAssignMaterialToObjects(CRhinoDoc& doc, const ON_ClassArray<CRhinoObjRef>& aObject, const UUID& uuidInstance);

/** Assign a material to a collection of objects via object references. This can assign to whole objects
	or to sub-faces of objects, depending on what the objRef refers to.
	\param doc is the document containing the material.
	\param aObject is an array of references to objects or sub-faces to assign the material to.
	\param uuidInstance is the instance id of the material.
	\param sfc specifies how to handle existing sub-face assignments.
	\param bc specifies how to handle blocks (if present in the aObjRef array).
	\param bInteractive specifies how to ask the user questions.
	 If \e true, a message box is used, otherwise the command line is used. */
RHRDK_SDK void RhRdkAssignMaterialToObjects(CRhinoDoc& doc, const ON_ClassArray<CRhinoObjRef>& aObject, const UUID& uuidInstance,
               RhRdkAssignToSubFaceChoices sfc, RhRdkAssignToBlockChoices bc, bool bInteractive);

/** Assign a material to a collection of layers via layer indices.
	\param doc is the document containing the material and the layers.
	\param layer_indices is an array of layer indices identifying the layers to assign the material to.
	\param uuidInstance is the instance id of the material.
	\param bInteractive specifies how to ask the user questions.
	 If \e true, a message box is used, otherwise the command line is used.
	 This is for future use as no questions are currently asked. */
RHRDK_SDK bool RhRdkAssignMaterialToLayers(CRhinoDoc& doc, const ON_SimpleArray<int>& layer_indices,
                                           const UUID& uuidInstance, bool bInteractive);

/** \internal For internal use only. Please do not use this class. It will soon be deleted. */
class RHRDK_SDK CRhRdkInternalSnapshotWorkaround final
{
public:
	CRhRdkInternalSnapshotWorkaround(CRhinoDoc& doc);
	~CRhRdkInternalSnapshotWorkaround();

private:
	void* m_p;
};

RHRDK_SDK ON_Mesh* RhinoCreateMeshVertexColorsFromTexture(CRhinoDoc& doc, const CRhinoMeshObject* pMeshObj, ON_Texture::TYPE textureType);
RHRDK_SDK bool RhinoCreateMeshVertexColorsFromDib(const CRhinoDoc& doc, ON_Mesh& mesh, const ON_TextureMapping& mapping, const ON_Xform& localMappingXform, const CRhinoDib& dib);

/** \class CRhRdkPreviewSceneElement

	This class represents a scene element which provides information about an element in
	the scene necessary to render a preview of a material or an environment.
	These elements are typically backgrounds, lighting, or simple objects like sphere, cube etc.

*/
class RHRDK_SDK CRhRdkPreviewSceneElement : public CRhRdkObject
{
public:
	CRhRdkPreviewSceneElement();
	virtual ~CRhRdkPreviewSceneElement();

	virtual UUID TypeId(void) const = 0;
	virtual const wchar_t* ElementKind(void) const = 0;
	virtual const wchar_t* XmlTag(void) const = 0;

	virtual CRhRdkPreviewSceneElement* Duplicate(void) const = 0;
};

class RHRDK_SDK CRhRdkPreviewGeometry : public CRhRdkPreviewSceneElement
{
public:
	virtual const wchar_t* ElementKind(void) const final override;

public:
	virtual void SetUpPreview(class IRhRdkPreviewSceneServerEx& ss, const CRhRdkContent& c, bool bCopy) const = 0;
};

class RHRDK_SDK CRhRdkPreviewBackground : public CRhRdkPreviewSceneElement
{
public:
	/** If the background uses an environment, return the instance id of the environment.
		The default implementation returns ON_nil_uuid. */
	virtual UUID EnvironmentInstanceId(void) const;

	/** If the background uses an environment, set the instance id of the environment. */
	virtual void SetEnvironmentInstanceId(const UUID& uuidInstance);

public:
	virtual const wchar_t* ElementKind(void) const final override;

public:
	virtual void SetUpPreview(class IRhRdkPreviewSceneServerEx& ss, const UUID uuidRdkDoc) const = 0;
};

class RHRDK_SDK CRhRdkPreviewLighting : public CRhRdkPreviewSceneElement
{
public:
	virtual const wchar_t* ElementKind(void) const final override;

public:
	virtual void SetUpPreview(class IRhRdkPreviewSceneServerEx& ss) const = 0;
};

// Preview geometry strings.
#define SS_PG_SPHERE       L"sphere"
#define SS_PG_CUBE         L"cube"
#define SS_PG_CONE         L"cone"
#define SS_PG_PYRAMID      L"pyramid"
#define SS_PG_TORUS        L"torus"
#define SS_PG_PLANE        L"plane"
#define SS_PG_CONE_SPHERE  L"cone-sphere"
#define SS_PG_SCENE        L"material-scene"

RHRDK_SDK CRhRdkPreviewGeometry* RhRdkNewPreviewGeometry(const wchar_t* wszPG);

// Preview background strings.
#define SS_PB_UNUSED     L"unused"
#define SS_PB_NONE       L"none"
#define SS_PB_CHECKERED  L"checkered"
#define SS_PB_CUSTOM     L"custom"
#define SS_PB_SCENE      L"material-scene"

RHRDK_SDK CRhRdkPreviewBackground* RhRdkNewPreviewBackground(const wchar_t* wszPB);

// Preview lighting strings.
#define SS_PL_SIMPLE  L"simple-lighting"
#define SS_PL_SKY     L"skylighting"

RHRDK_SDK CRhRdkPreviewLighting* RhRdkNewPreviewLighting(const wchar_t* wszPL);

/** \class CRhRdkSSData

	This class contains data about the preview scene server.

*/
class RHRDK_SDK CRhRdkSSData : public CRhRdkObject
{
public:
	enum class Usage : unsigned int
	{
		Synchronous,
		Asynchronous,
	};

	CRhRdkSSData(const CRhRdkPreviewGeometry* pGeo, const CRhRdkPreviewBackground* pBack, const CRhRdkPreviewLighting* pLight, Usage u);
	virtual ~CRhRdkSSData();

	Usage GetUsage(void) const;

	const CRhRdkPreviewGeometry*   Geometry(void) const;
	const CRhRdkPreviewBackground* Background(void) const;
	const CRhRdkPreviewLighting*   Lighting(void) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

// CRC a double. Helps in CRC calculation when floating point accuracy can be a problem.
RHRDK_SDK ON__UINT32 RhRdkDoubleCRC(ON__UINT32& crc, double d);

/*! \mainpage RDK SDK Documentation
 *
 * \section intro_sec Introduction
 *
 * This is the documentation for the "Renderer Development Kit" (RDK) SDK - a special SDK that adds renderer
 * developer specific support to the basic Rhino SDK. The RDK includes support for a fully extensible
 * Material, Environment and Texture editor, procedural textures, along with other useful utilities for renderer
 * developers like a Sun Angle Calculator, some UI widgets and a few other bits and pieces.
 *
 *
 * \section install_sec Installation
 *
 * Download the RDK plug-in installer from the Wiki and install.  
 * Your renderer plug-in installer should run the RDK plug-in installer as part of the installation
 * process and include the "\silent" switch on the command line.
 *
 * \section example_sec Sample Code
 *
 * Please see https://github.com/mcneel/rhino-developer-samples/tree/7/cpp/SampleRdkMarmalade
 * This is a complete implementation of a rendering plug-in including automatic UI materials and frame buffer support.
 */
