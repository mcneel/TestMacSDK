
#pragma once // Public SDK header

#include "RhRdkObject.h"

/** \class CRhRdkExtension

	This is the base class for RDK extensions whose interface is defined by a UUID.
	Some RDK extensions have interface files which are supplied with the RDK SDK
	(e.g., content factories) and some do not. The latter case is generally reserved
	for features which are added to the RDK after the initial SDK release and allows
	us to add extensions without breaking the SDK.

	\see CRhRdkPlugIn::RegisterExtensions()
	\see CRhRdkPlugIn::AddExtension()
	\see RhRdkAddExtension()
*/
class RHRDK_SDK CRhRdkExtension : public CRhRdkObject
{
public:
	CRhRdkExtension();
	virtual ~CRhRdkExtension();

	/** Returns the unique interface id of this extension. */
	virtual UUID InterfaceId(void) const = 0;

	/** This must return the UUID of the host's CRhinoPlugIn class. This is critical because it
	    is used to determine if an extension should be deleted when its host plug-in is removed. */
	virtual UUID PlugInId(void) const = 0;

	/** Unregister this extension. If successful, this deletes this extension and returns \e true.
		\return \e true if successful, else \e false. */
	bool Unregister(void);

private:
	class CImpl;
	CImpl* m_pImpl;
};
