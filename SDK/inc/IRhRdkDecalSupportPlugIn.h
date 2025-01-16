
#pragma once // Public SDK header

class IRhRdk_XMLSection;

class RHRDK_SDK IRhRdkDecalSupport
{
public:
	virtual ~IRhRdkDecalSupport() { }

	/** \return \e true if you support editing of custom decal properties. */
	virtual bool SupportsEditProperties(void) const = 0;

	/** Called when there is no data on an object and custom parameter values need to be set
		to their initial values.
		\param section is the parameter section to be initialized.
		\return \e true if initialized successfully, else \e false. */
	virtual bool Initialize(IRhRdk_XMLSection& section) const = 0;

	/*	DEPRECATED; NOT CALLED - see IRhRdkDecalSupportEx
		Your implementation should simply return false. */
	virtual bool OnEditProperties(IRhRdk_XMLSection& section) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RHRDK_SDK IRhRdkDecalSupportEx : public IRhRdkDecalSupport
{
public:
	/** Called when the user clicks the Properties button on the Decal Object Property page.
		\param section provides your custom parameters and allows you to set them back
		 after editing by the user.
		\param pDoc is the document containing the objects that have the decals.
		\return \e true if editing completed successfully, else \e false. */
	virtual bool OnEditPropertiesEx(IRhRdk_XMLSection& section, CRhinoDoc* pDoc) const = 0;
};
