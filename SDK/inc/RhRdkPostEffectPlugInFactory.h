
#pragma once

#include "RhRdkExtension.h"

class CRhRdkPostEffectPlugIn;
class IRhRdkPostEffectPlugIn;

/** \class CRhRdkPostEffectPlugInFactory

	This class is deprecated in favor of CRhRdkPostEffectFactory. It is not implemented.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkPostEffectPlugInFactory : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;
	virtual CRhRdkPostEffectPlugIn* NewPlugIn(void) const = 0;
};

/** \class CRhRdkPostEffectFactory

	This class allows you to provide a factory for generating a custom post-effect plug-in (PEP).

*/
class RHRDK_SDK CRhRdkPostEffectFactory : public CRhRdkExtension
{
public:
	/** Create the new PEP instance.
		\return A pointer to the new PEP object. Do not return null. */
	virtual IRhRdkPostEffect* NewPostEffect(void) const = 0;

protected:
	virtual UUID InterfaceId(void) const final override;
};
