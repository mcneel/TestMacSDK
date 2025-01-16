
#pragma once // Public SDK header

#include "RhRdkContent.h"

class CRhRdkCoreContentUI;
class IRhRdkParamBlock;
class IRhRdkExpandableContentUI;
class IRhRdkCustomCurveGraphSectionCallback;

#pragma warning(push)
#pragma warning(disable : 4996) // xxxx was declared deprecated.

/** \class CRhRdkCoreContent

	This class provides a default implementation of user interface creation
	and destruction. To use this, derive your content from CRhRdkCoreContent
	instead of CRhRdkContent and override AddUISections() to add each of your
	user interface sections (derived from CRhRdkContentUISection_MFC).
*/
class RHRDK_SDK CRhRdkCoreContent : public CRhRdkContent
{
public:
	CRhRdkCoreContent();
	CRhRdkCoreContent(bool bAutoGlobalRegister);
	virtual ~CRhRdkCoreContent();

	/** Used by the RDK editor to create the content's UI. This UI allows editing of the content's
		persistent state. In order to reduce the number of Window handles used by content UIs,
		RDK supports Content UI Sharing. This causes your content UI to be shared with other
		contents of the same type id. Your UI code cannot store the pointer, reference or
		instance id of the associated content because that content will change when the
		selection changes. The default implementation creates a UI using a 'holder' and
		'sections' (AKA 'roll-ups'). You can override this method to completely replace
		the RDK's default content UI system for this particular content type. This is an
		expert override which will break the RDK content editor if implemented incorrectly.
		\see IRhRdkContentUI \see IRhinoUiSection
		\param e is the editor the UI will appear in.
		\param uuidUI is the unique identifier of the UI being created.
		\param aInitialClients is an array of contents that are requesting the UI to be created.
		\param pReserved is reserved for future use.
		\return \e true if successful, else \e false. */
	virtual IRhRdkContentUI* CreateUI(IRhRdkContentEditor& e, const UUID& uuidUI, const CRhRdkContentArray& aInitialClients, void* pReserved) override;

	/** For expert use only. You should never need to override this method. */
	virtual void ReleaseAllUIs(bool bIncludeChildren) const override;

	/** For expert use only. You should never need to override this method. */
	virtual void ReleaseAllEditorUIs(const UUID& uuidEditor, bool bIncludeChildren) const override;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool AddAutomaticUISection(const wchar_t* wszCaption, int id=0);

	/** Call this method from AddUISections() in order to add an automatic section.
		You will need to override AddAutoParameters() to add the parameters
		to a collection that will be used by this system.
		\param ui is the UI to add the automatic section to.
		\param wszLocalCaption is the localized caption to display in the section header.
		\param wszEnglishCaption is the english caption.
		\param id is a caller-defined identifier (can be zero if not needed).
		 Values of 0x8000 and above are reserved for RDK internal use.
		 \see enum eSectionId for special values used internally.
		\note This method must \e only be called from AddUISections().
		\see CRhRdkContent::AddAutoParameters()
		\see CRhRdkContent::GetAutoParameters()
		\return \e true if successful, else \e false. */
	virtual bool AddAutomaticUISection(IRhRdkExpandableContentUI& ui, const wchar_t* wszLocalCaption, const wchar_t* wszEnglishCaption, int id=0);

	/** \see CRhRdkCoreContent::AddAutomaticUISection()
		\see CRhRdkContent::AddAutoParameters()
		\see CRhRdkContent::GetAutoParameters() */
	enum SectionId : int
	{
		/** This id value is used when creating the Texture Summary section.
			AddAutoParameters() and GetAutoParameters() will be called with
			this id value if that section is included in your UI. */
		TextureSummary = 0x8000,

		/** This id value is used to indicate that the section id parameter is not used. */
		NotUsed = 0x8001
	};

	/** This method is obsolete because the Texture Summary panel now automatically refreshes as necessary. */
	RDK_DEPRECATED virtual bool RecreateTextureSummary(void);

	virtual ParamSerialMethod ParameterSerializationMethod(void) const override;

	/** <b>Please be sure to call the base class</b> if you override this function. */
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;

	/** <b>Please be sure to call the base class</b> if you override this function. */
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual IRhRdkExpandableContentUI* UI(void) const override;

	/** Get an array of pointers to UIs that the content is a client of. */
	virtual void GetUIs(ON_SimpleArray<IRhRdkContentUI*>& aOut) const override;

	/** Create a preview of the content for use in the content editor thumbnail display.
		\param plugIn The render plug-in to use to render the preview.
		\param sizeImage The size of the image to render.
		\param qual The quality of the preview.
		\param pSceneServer A pointer to a scene server that defines the scene for the preview rendering.
		 This can be null in which case a default scene is used.
		\param dibOut A dib containing the rendered image if the method returns \e true.
		\return \e true if successful, else \e false. */
	virtual bool CreatePreview(CRhRdkRenderPlugIn& plugIn, const ON_2iSize& sizeImage,
	                           RhRdkPreviewQuality qual, const IRhRdkPreviewSceneServer* pSceneServer,
	                           IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) const override;

	/** Optionally implement this method to have RDK add a custom curve section to your content's UI.
		The custom curve section will be identified by the UUID uuidCustomCurveSection in RhRdkUuids.h
		\see uuidCustomCurveSection
		\see IRhRdkCustomCurveGraphSectionSharedCallback
		\return A pointer to a new instance of your implementation of IRhRdkCustomCurveGraphSectionSharedCallback.
		 The callback object will be owned by the UI section. The default implementation returns null; no custom curve section required.

		[SDK_UNFREEZE] Prefix this function name with 'New', make it const, and return IRhRdkCustomCurveGraphSectionSharedCallback.

		The return type of this method is wrong; it is the obsolete IRhRdkCustomCurveGraphSectionCallback interface.
		Until we can fix this properly you must use reinterpret_cast to return a new instance of
		your implementation of IRhRdkCustomCurveGraphSectionSharedCallback as IRhRdkCustomCurveGraphSectionCallback.

		IRhRdkCustomCurveGraphSectionCallback* CMyMaterial::CustomCurveGraphSectionCallback(void)
		{
			return reinterpret_cast<IRhRdkCustomCurveGraphSectionCallback*>(new CMyCurveCallback);
		}
	*/
	virtual IRhRdkCustomCurveGraphSectionCallback/*SharedCallback*/ * /*New*/CustomCurveGraphSectionCallback(void); // const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void AddUISections(void);

	/** This is called by the default implementation of CreateUI. You should create
		each of your UI sections using 'new' and then call ui.AddSection() on them.
		Please <b>be sure</b> to call the base class after adding your sections. */
	virtual void AddUISections(IRhRdkExpandableContentUI& ui);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

public:
	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual void AddUI(IRhRdkContentUI& ui) const override;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual void TransferUIsFrom(const CRhRdkContent& c) override;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual CRhRdkPreviewGeometry* NewPreviewGeometry(void) const override;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual CRhRdkPreviewBackground* NewPreviewBackground(void) const override;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

#pragma warning(pop)
