
#pragma once // Public SDK header

#include "RhRdkExtension.h"

/** \class CRhRdkCompoundContentImplementor

	Renderers implement this interface to provide a means of handling compound content such as blends.
*/
class RHRDK_SDK CRhRdkCompoundContentImplementor : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	/** \return The unique id for the kind of CCI. */
	virtual UUID Kind(void) const = 0;

	/** \return The Render Engine Id of target renderer. */
	virtual UUID TargetRenderEngineId(void) const = 0;

	/** \return A new instance of the same object. */
	virtual CRhRdkCompoundContentImplementor* NewInstance(void) const = 0;

	/* This appears to be an error in the SDK.  Return false unless you are implementing a basic material CCI. */
	virtual bool DisableLighting(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) override = 0;

public:
	/** \internal For RDK internal use only. Do not implement this method in external plug-ins. */
	virtual bool IsBuiltIn(void) const = 0;
};
