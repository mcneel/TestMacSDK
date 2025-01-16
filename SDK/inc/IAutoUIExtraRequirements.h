
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

/** \class IAutoUIExtraRequirements

	Extra requirements are a way of specifying extra functionality on parameters in the automatic UI.
	All objects that support such extra UI requirements must implement this interface.
	
	RDK currently supports the following extra requirements:
	
	- RDK_TEXTURING               Provides texturing controls; an 'on' checkbox, an 'amount' edit box, and a 'subnode' control.
	- RDK_EXCLUDE_LIST            Provides support for exclude lists (lists of things to be excluded from some operation).
	- RDK_ENABLE_DISABLE          Provides an 'on' checkbox for enable/disable of the control set.
	- RDK_FIXED_DECIMAL_PLACES    Allows customization of number of decimal places displayed in a numeric edit box.
	- RDK_NUMBER_EDIT_TWEAKS      Allows tweaking of numeric edit boxes.
	- RDK_COLOR_BUTTON_TWEAKS     Allows tweaking of color buttons.
	- RDK_IOR_MENU                Requests a context menu containing the indices of refraction for all kinds of substances.
	- RDK_COMBOBOX
	- RDK_DATETIME                Requests a callback for providing date/time information to the date-time or location controls.
	- RDK_LOCATION                Requests a callback for providing location information to the date-time or location controls.
	- RDK_CONTROLSET_ENABLE       Requests a general enable/disable callback.

*/

// Extra requirements for texturing. See also RhRdkGetTexturingExtraRequirement(), RhRdkSetTexturingExtraRequirement().
#define RDK_TEXTURING                   L"texturing"                   // Specifies 'texturing' extra requirement.
#define RDK_TEXTURE_ON                  L"texture-on"                  // ---- value for check box (true or false).
#define RDK_TEXTURE_AMOUNT              L"texture-amount"              // ---- value for edit box (0 to 100).
#define RDK_TEXTURE_ON_SHOWN            L"texture-on-shown"            // ---- show or hide the check box (default is shown).
#define RDK_TEXTURE_AMOUNT_SHOWN        L"texture-amount-shown"        // ---- show or hide the edit  box (default is shown).
#define RDK_TEXTURE_PARENT              L"texture-parent"              // ---- for getting parent content instance id.
#define RDK_TEXTURE_LOWER_BOUND         L"texture-lower-bound"         // ---- optional lower bound amount.
#define RDK_TEXTURE_UPPER_BOUND         L"texture-upper-bound"         // ---- optional upper bound amount.
#define RDK_TEXTURE_CHANNEL_MODE        L"channel-mode"
#define RDK_TEXTURE_CHANNEL_CUSTOM      L"channel-string"              // ---- This was previously RDK_TEXTURE_CHANNEL_STRING
#define RDK_TEXTURE_CHANNEL_STRINGS     L"channel-strings"             // ---- A semicolon delimited list of possible custom strings.

// Extra requirements for exclude list.
#define RDK_EXCLUDE_LIST                L"exclude-list"                // Specifies 'exclude list' extra requirement.
#define RDK_EXCLUDE_LIST_VALUE          L"exclude-list-value"          // ---- name of value string.
#define RDK_EXCLUDE_LIST_BUTTON         L"exclude-list-button"         // ---- name of button.
#define RDK_EXCLUDE_LIST_DIALOG_CAPTION L"exclude-list-dialog-caption" // ---- value of dialog caption.
#define RDK_EXCLUDE_LIST_BUTTON_CAPTION L"exclude-list-button-caption" // ---- value of button caption.

// Extra requirements for local mapping.
#define RDK_LOCAL_MAPPING               L"local-mapping"               // Specifies 'local-mapping' extra requirement.

// Extra requirements for enable/disable.
#define RDK_ENABLE_DISABLE              L"enable-disable"              // Specifies 'enable/disable' extra requirement.
#define RDK_ENABLE_ON                   L"enable-on"                   // ---- state of check box.

// The original decimal places system is DEPRECATED and NOT IMPLEMENTED due to issues in Brazil.
#define RDK_DECIMAL_PLACES              L"decimal-places"
#define RDK_DECIMAL_PLACES_AMOUNT       L"decimal-places-amount"

// Extra requirements for number edit decimal places.
#define RDK_FIXED_DECIMAL_PLACES        L"fixed-decimal-places"        // Specifies 'fixed decimal places' extra requirement.
#define RDK_FIXED_DECIMAL_PLACES_AMOUNT L"fixed-decimal-places-amount" // ---- Number of decimal places (double).

// Extra requirements for number edit tweaks.
#define RDK_NUMBER_EDIT_TWEAKS          L"number-edit-tweaks"  // Specifies 'number edit tweaks' extra requirement.
#define RDK_FIXED_SPINNER_DELTA         L"fixed-spinner-delta" // --- Tweak for fixed spinner delta (double).
#define RDK_LINEAR_ROLLING_DELTA        L"linear-rolling-delta"// --- Tweak for linear rolling delta (double).
#define RDK_SHIFT_KEY_SCALING           L"shift-key-scaling"   // --- Tweak for shift key scaling (double).
#define RDK_CONTROL_KEY_SCALING         L"control-key-scaling" // --- Tweak for control key scaling (double).
// Implemented in Rhino 5 SR4
#define RDK_PERCENTILE                  L"percentile"          // --- Tweak for edit boxes that turn 0.0..1.0 into 0..100% (bool).
#define RDK_SUFFIX_STRING               L"suffix-string"       // ----Tweak for suffix (i.e., degree symbol) (string)
// Implemented in Rhino 5 SR8
#define RDK_SIMPLE_ARITHMETIC           L"simple-arithmetic"   // --- Tweak for simple arithmetic (bool).

// Extra requirements for color button tweaks.
#define RDK_COLOR_BUTTON_TWEAKS         L"color-button-tweaks" // Specifies 'color button tweaks' extra requirement.
#define RDK_COLOR_BUTTON_ALPHA          L"color-button-alpha"  // --- Tweak to make color button support alpha (bool).

// Extra requirements for edit box IOR menu.
#define RDK_IOR_MENU                    L"ior-menu"            // Specifies 'IOR menu' extra requirement.
#define RDK_IOR_MENU_ON                 L"ior-menu-on"         // ---- Is menu to be displayed? (bool).

// Extra requirements for combo box.
#define RDK_COMBOBOX                    L"combo-box"           // Specifies 'combo box' extra requirement.
#define RDK_COMBOBOX_CONTENTS           L"combo-box-contents"  // ---- Semicolon-delimited contents (string).

// Content Control Channel modes.
#define RDK_CC_MODE_DEFAULT             L"default"
#define RDK_CC_MODE_CUSTOM              L"custom"

// These two are deprecated and not used due to being confusing and inaccurate.
#define RDK_DATETIME_LOCATION           L"datetime-location"
#define RDK_LOCATION_DATETIME           L"location-datetime"

// Extra requirement for date/time.
#define RDK_DATETIME                    L"datetime"            // Specifies 'datetime' extra requirement. Implied return value must be vtTime.

// Extra requirement for location.
#define RDK_LOCATION                    L"location"            // Specifies 'location datetime' extra requirement. Implied return value must be vtVector2d (x = latitude, y = longitude).

// Extra requirement to determine whether the control should be enabled or disabled.
#define RDK_CONTROLSET_ENABLE           L"control-set-enable"  // Specifies 'control set enable' extra requirement. Implied return value must be vtBool.

#define RDK_UI_HINT                     L"ui-hint"             // Gets the Ui hint for a specific parameter.  This may result in specific extra requirements being added.

// Deprecated string names.
#define RDK_DATETIME_SUN_LOCATION   DEPRECATED L"Please use RDK_LOCATION"
#define RDK_LOCATION_SUN_DATETIME   DEPRECATED L"Please use RDK_DATETIME"
#define RDK_COLOR_CONTROLSET_ALPHA  DEPRECATED L"Please use RDK_COLOR_BUTTON_TWEAKS with RDK_COLOR_BUTTON_ALPHA"

class CRhRdkVariant;

class RHRDK_SDK IAutoUIExtraRequirements
{
public:
	virtual ~IAutoUIExtraRequirements() { }

	enum RDK_DEPRECATED eSetContext { scUI, scDrop, scProgram, ccForce32bit=0xFFFFFFFF };

	enum class SetContext : unsigned int
	{
		UI,      // Setting extra requirement as a result of user activity.
		Drop,    // Setting extra requirement as a result of drag and drop.
		Program, // Setting extra requirement as a result of other (non-user) program activity.
	};

	/** Implement this method to get a parameter for extra UI requirements.
		\param wszParamName is the name of the content parameter for which to get an extra req. parameter.
		\param wszExtraReqName is the name of the extra requirement parameter to retrieve.
		\param vValueOut receives the parameter's value.
			For the enabled-on checkbox \e Checked property, 0 means \e false, 1 means \e true, 2 means \e null (indeterminate).
		\return \e true if the parameter was retrieved successfuly, else \e false. */
	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName,
	                                          CRhRdkVariant& vValueOut) const = 0;

	/** Implement this method to set a parameter for extra UI requirements.
		\param wszParamName is the name of the content parameter for which to set an extra req. parameter.
		\param wszExtraReqName is the name of the extra requirement parameter to be set.
		\param vValue contains the new value for the specified extra req. parameter.
			For the enabled-on checkbox \e Checked property, 0 means \e false, 1 means \e true, 2 means \e null (indeterminate).
		\param sc is the context in which the set operation is being performed.
		\return \e true if the parameter was set successfuly, else \e false. */
	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName,
	                                          const CRhRdkVariant& vValue, SetContext sc) = 0;

	/** Implement this method to handle an extra UI requirements button click.
		\param wszParamName is the name of the content parameter for which a button was clicked.
		\param wszButtonName is the name of the button that was clicked.
		\return \e true if button click was handled successfully, else \e false. */
	virtual bool OnExtraRequirementButtonClicked(const wchar_t* wszParamName,
	                                             const wchar_t* wszButtonName) = 0;

	/** \return A duplicate of the object that implements the interface. */
	virtual IAutoUIExtraRequirements* DuplicateExtraRequirementsInterface(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
