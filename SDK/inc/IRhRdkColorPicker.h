
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkColor;

/**	\class IRhRdkColorPicker

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkColorPicker
{
public:
	/** You must implement this method as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool ShowPicker(CRhRdkColor& colInOut, HWND pParent, class CRhRdkColorButton* pButton);

	/** Display the color picker user interface.
		\param colInOut contains the initial color value and accepts the user's new color choice.
		\param pParent is the parent window of the picker. This should not be the color button itself.
		\note To update the color button during picking, you should call RhRdkGetActiveColorButton(),
		 check for null, and update the returned button if not null. This replaces the
		 color button pointer in the deprecated version because there is a danger that
		 the button can be deleted during the time the picker is displayed.
		\return \e true if a new color was chosen, else \e false. */
	virtual bool ShowPicker(CRhRdkColor& colInOut, HWND pParent) = 0;

	/** Called when the user picks a color. This can happen once when the color picker is dismissed
		or it can be called many times as the user plays with the color picker, depending on how
		the color picker is implemented. */
	virtual void SetButtonColor(const CRhRdkColor& col) const = 0;

	/** Called when a color is about to be applied to a color button. The implementation of this
		method typically does nothing, but you may wish to implement special behaviour to modify
		the color. For example, a grayscale picker would convert the color to grayscale. */
	virtual void NormalizeColor(CRhRdkColor& colInOut) const = 0;

	/** Optionally implement this method to return the menu string describing the picker and return \e true.
		If you just want the default text "Color Picker..." you can simply return \e false. */
	virtual bool MenuString(ON_wString& sOut) const = 0;

	/** Implement this method to return an array of custom named colors to be displayed
		on the left side of the color picker instead of the defaults.
		\param aColorOut accepts the custom colors if you plan to return \e true.
		\return \e true to use your custom colors, \e false if you don't want to use custom colors. */
	virtual bool GetCustomNamedColors(ON_ClassArray<CRhinoNamedColor>& aColorOut) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkColorPicker() { }
};
