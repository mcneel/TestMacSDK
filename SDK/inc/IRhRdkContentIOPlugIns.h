
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkContentIOPlugIn;

/** \class IRhRdkContentIOPlugIns

	This interface represents the collection of all content I/O plug-ins that are known to the RDK.
*/
class RHRDK_SDK IRhRdkContentIOPlugIns
{
public:
	virtual ~IRhRdkContentIOPlugIns() { }

	/** Get a pointer to the first content I/O plug-in in the collection.
		\return A pointer to the first content I/O plug-in or NULL if there are none. */
	virtual CRhRdkContentIOPlugIn* First(void) const = 0;

	/** Get a pointer to the next content I/O plug-in in the collection.
		\return A pointer to the next I/O plug-in or NULL if no more. */
	virtual CRhRdkContentIOPlugIn* Next(void) const = 0;

	/** Find a content I/O plug-in by its file extension.
		\return A pointer to the I/O plug-in or NULL if no plug-in supports the extension. */
	virtual CRhRdkContentIOPlugIn* Find(const ON_wString& sExtension) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
