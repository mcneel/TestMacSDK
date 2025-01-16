
#pragma once // SDK header

#include "RhRdkDefinitions.h"

class IRhRdkParamBlock;

/** \class IRhRdkParamTransport

	This interface makes the connection between RDK's automatic user interface
	and your RDK plug-in. You must implement this interface if you want to
	use RDK's automatic user interface services.
*/
class RHRDK_SDK IRhRdkParamTransport
{
public:
	virtual ~IRhRdkParamTransport() { }

	/** Implement this method to return a caption string for the ui section. */
	virtual ON_wString Caption(void) const = 0;

	/** Implement this method to initially add your parameters to the param block. */
	virtual void AddParamsToParamBlock(IRhRdkParamBlock& paramBlock) const = 0;

	/** Implement this method to retrieve the updated parameters after the
		user has made a change to one of them. */
	virtual void GetParamsFromParamBlock(const IRhRdkParamBlock& paramBlock) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
