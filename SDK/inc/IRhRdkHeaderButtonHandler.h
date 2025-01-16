
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

/** \class IRhRdkHeaderButtonHandler

	This interface represents a handler for putting custom buttons on the header of IRhinoUiSection.

	\see IRhinoUiSection::HeaderButtonHandler().

*/
class RHRDK_SDK IRhRdkHeaderButtonHandler
{
public:
	virtual ~IRhRdkHeaderButtonHandler() { }

	/** You must implement this method as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** Implement this method to describe custom buttons on the section header.
		RDK calls this method continually increasing iIndex until it returns \e false.
		\param index is the button index starting at zero and increasing with each call.
		\param iconOut accepts the button's imagery, or 'icon'. If an icon is not required, do not set this parameter.
		\param sToolTipOut accepts the button's tool-tip. If a tool-tip is not required, do not set this parameter.
		\return \e true if button is required, \e false to stop. */
	virtual bool ButtonDetails(int index, CRhinoDib& iconOut, ON_wString& sToolTipOut) const = 0;

	/** Implement this method to handle a button click.
		\param index is the index of the button that was clicked.
		\return \e true if handled successfully, else \e false. */
	virtual bool OnButtonClicked(int index) = 0;

	/** Implement this method to get the rectangle of a button on the header window.
		\param index is the index of the button.
		\param rectHeader is the usable area of the header where the buttons can be placed.
		\return The rectangle of the button. */
	virtual ON_4iRect ButtonRect(int index, const ON_4iRect& rectHeader) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
