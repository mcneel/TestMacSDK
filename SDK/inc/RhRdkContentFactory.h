
#pragma once // Public SDK header

class IRhRdkContentUIAgent;

/** \class CRhRdkContentFactory

	This class is a factory for producing a certain type of content.
	You must create and implement a subclass for each content type that your renderer supports.

	Content factories are RDK extensions. You usually add new content factories to the system when you
	are called on your implementation of CRhRdkPlugIn::RegisterExtensions(). If necessary, you can also
	add new factories later by calling RhRdkAddExtension(). Factories so added can be found again later
	by calling RhRdkContentFactories() which returns an IRhRdkContentFactories interface.

	When you add a factory to the factory collection, a set of tests are performed on the factory
	before it is allowed to be added. The factory is only added if the following succeeds:

	 - CRhRdkContentFactory::Initialize() must return \e true.
	 - CRhRdkContentFactory::NewContent() must return a valid content.
	 - CRhRdkContent::Initialize() must return \e true.

	If these tests succeed, RDK takes ownership of the factory object and sends the
	OnFactoryAdded event. Otherwise, the factory is considered bad and is deleted.
*/
class RHRDK_SDK CRhRdkContentFactory : public CRhRdkExtension
{
public:
	CRhRdkContentFactory();
	virtual ~CRhRdkContentFactory();

	virtual UUID InterfaceId(void) const final override;

	/** Called by RDK to initialize the factory.
		If you override this method, <b>please be sure to call the base class</b>.
		\return \e true if successful, else \e false. */
	virtual bool Initialize(void);

	/** \return \e true if the kind of content produced by this factory matches the input. */
	virtual bool IsKind(CRhRdkContent::Kinds kind) const;

	/** If there are more than 5 types in the drop-down 'Type' list, the types are not
		listed and can only be accessed from the 'More Types...' option. 'Elevated' types
		will always appear in the list, even if there are more than 5. You should only
		elevate a few of your most commonly-used types.
		\see CRhRdkRenderPlugIn::UiContentTypes.
		\return \e true if the content type that this factory produces is 'elevated'. */
	virtual bool IsElevated(void) const;

	/** \return The type id of the content that this factory produces. */
	virtual UUID ContentTypeId(void) const;

	/** \return The internal name of the content that this factory produces. */
	virtual ON_wString ContentInternalName(void) const;

	/** \return The render engine id of the content that this factory produces. */
	virtual UUID RenderEngineId(void) const;

	/** \return The plug-in id of the plug-in that created this factory. */
	virtual UUID PlugInId(void) const override;

	/** Rebuild the factory cache. This forces a refresh of cached data such as the factory name. */
	virtual void RebuildCache(void);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

public:
	/** \return The kind of content that this factory produces. */
	virtual CRhRdkContent::Kinds Kind(void) const = 0;

	/** Create a new content.
		\note This should be implemented simply as \code return new CMyContent(...); \endcode
		 No further initialization or operations should be performed on the new content.
		\return A pointer to a new instance of the content that this factory produces. */
	virtual CRhRdkContent* NewContent(void) const = 0;

	virtual_su CRhRdkContent* NewContent(bool bAutoGlobalRegister) const; // = 0; [SDK_UNFREEZE] Currently implemented by EVF(L"NewContent")

public:
	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual bool IsBuiltIn(void) const { return false; }

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual IRhRdkContentUIAgent* ContentUIAgent(void) const;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual void SetContentUIAgent(IRhRdkContentUIAgent* pAgent);

private:
	class CImpl;
	CImpl* m_pImpl;
};
