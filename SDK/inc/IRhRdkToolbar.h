
#pragma once // Public SDK header

/** \class IRhRdkToolbar

	This interface facilitates possible future inclusion of a tool bar in the
	RDK content editor dialog. This feature is currently <b>not implemented</b>
	and is included only for future compatibility.
*/

#include "RhRdkDefinitions.h"

class RHRDK_SDK IRhRdkToolbar
{
public:
	virtual ~IRhRdkToolbar() { }

	/** Add a button to the toolbar.
		\param uuidAction is the identifier of the action to associate with the button.
		\return \e true if successful, else \e false. */
	virtual bool AddButton(const UUID& uuidAction) = 0;

	/** Add a separator to the toolbar. */
	virtual void AddSeparator(void) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
