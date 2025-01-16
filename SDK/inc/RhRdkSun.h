
#pragma once // Public SDK header

#include "RhRdkObject.h"
#include "IRhRdkSun.h"

/** \class CRhRdkSunDialog

	This class is a wrapper around a modal sun dialog. It is only used internally.
	SDK clients should not attempt to use this as it requires internal RDK classes
	to be used successfully. [SDK_UNFREEZE] Remove this from the public SDK.

*/
class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkSunDialog : public CRhRdkObject
{
public:
	CRhRdkSunDialog(HWND pParent=NULL);
	virtual ~CRhRdkSunDialog();

	/** Set the controller for the dialog to get and set the sun data.
		The controller must have at least one data source that responds
		to uuidData_RdkSun. */
	void SetController(CRhRdkControllerPtr con);

	enum // Flags for uFlags parameter of SetShowingSections(). Return value of ShowingSections().
	{
		ss_sun_options   = 0x00000001, // Sun Options section.
		ss_skylight      = 0x00000002, // Skylight section.
		ss_sun_position  = 0x00000004, // Sun Position section.
		ss_date_and_time = 0x00000008, // Date and Time section (only visible if manual control is off).
		ss_location      = 0x00000010, // Location section (only visible if manual control is off).
		ss_settings      = 0x00000020, // Settings section.
		ss_all           = 0x000000FF, // All sections.
		ss_force_32_bit  = 0xFFFFFFFF, // For future compatibility.
	};
	/** Set which sections of the dialog should be displayed.
		\param uFlags specifies which sections to make visible as a bit-mask.
		The default is all sections are shown (depending on the state of manual control). */
	virtual void SetShowingSections(UINT uFlags);

	/** Get which sections of the dialog will be displayed. */
	virtual UINT ShowingSections(void) const;

	enum // Flags for uFlags parameter of SetSectionOptions(). Return value of SectionOptions().
	{
		so_lean_ui      = 0x00000001, // Lean UI. Currently this only hides the Time Zone control on the Location section.
		so_no_shadow    = 0x00000002, // Suppresses the day/night shadow on the map control.
		so_force_32_bit = 0xFFFFFFFF, // For future compatibility.
	};
	/** Set options for the dialog sections.
		\param uFlags specifies the options as a bit-mask. */
	virtual void SetSectionOptions(UINT uFlags);

	/** Get section options. */
	virtual UINT SectionOptions(void) const;

	/** Set the caption of the dialog. The default caption is "Sun Angle Calculator". */
	virtual void SetCaption(const wchar_t* wszCaption);

	/** \return \e true if all UI is visible and usable even when 'on' is unchecked. */
	virtual bool IgnoreOn(void) const;

	/** Set if all UI is visible and usable even when 'on' is unchecked. */
	virtual void SetIgnoreOn(bool b);

	/** Call this method to display the sun dialog.
		\return \e true if the user clicked OK, or \e false if the user cancelled. */
	virtual bool DoModal(void) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkSun

	This class represents a 'sun'. It can be used as a temporary sun property storage object
	or as a way to get sun XML or a light that represents a sun. However, please note that this
	is NOT the document sun. To use it, create one on the stack. Call Sun() to get an interface
	to the sun. Call the interface's various set methods to configure the sun the way you want it.
	You can copy the document sun to it if you wish. You might then call XML() or Light() to get
	an ON_Light to use as a sun light.

*/
class RHRDK_SDK CRhRdkSun : public CRhRdkObject
{
public:
	// Creates a scratch-pad sun.
	CRhRdkSun();

	// Creates a scratch-pad sun initialized from 'sun'.
	CRhRdkSun(const CRhRdkSun& sun);

	// Creates a scratch-pad sun initialized from 'sun'.
	CRhRdkSun(const ON_Sun& sun);

	virtual ~CRhRdkSun();

	const CRhRdkSun& operator = (const CRhRdkSun& sun);

	bool operator == (const CRhRdkSun& sun) const;
	bool operator != (const CRhRdkSun& sun) const;

	/** \return An interface to a temporary sun object. */
	virtual IRhRdkSun& Sun(void) const;

	operator IRhRdkSun& (void) const { return Sun(); }

	void ToOnSun(ON_Sun& sunOut) const;
	void FromOnSun(const ON_Sun& sun);

	ON_wString XML(void) const;
	void SetXML(const wchar_t* wszXML);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
	friend class IRhRdkSun;

public:
	RDK_DEPRECATED static IRhRdkSun* NewImpl();
};

/** \class CRhRdkDocSun

	This class is obsolete and should not be used. Please use doc.Properties().RenderSettings() instead.
	For example:

		CRhinoDoc& doc = ...

		CRhRdkDocSun sun(doc);
		if (sun.Sun().EnableOn())
			...
		if (sun.SkylightOn())
			...

		// The above is obsolete; change to:

		const auto& rs = doc.Properties().RenderSettings();

		if (rs.Sun().EnableOn())
			...
		if (rs.Skylight().On())
			...

*/
class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkDocSun : public CRhRdkObject
{
public:
	CRhRdkDocSun(CRhinoDoc& doc);
	CRhRdkDocSun(CRhRdkDocument& rdkDoc);
	CRhRdkDocSun(const CRhinoDoc& doc);
	CRhRdkDocSun(const CRhRdkDocument& rdkDoc);
};

/** Call this function to determine if an ON_Light added to the display for the sunlight preview
	is actually the RDK Document Sun.
	\param light is the light to test.
	\return true if the light is one of the lights returned from IRhRdkSun::Light(). */
RHRDK_SDK bool RhRdkIsSunLight(const ON_Light& light);

///////////////////////////////////////////////////////////////////////////////////////////////////
// These two functions shouldn't be in the SDK and are now deprecated. [SDK_UNFREEZE]
RDK_DEPRECATED RHRDK_SDK double SunAltitudeFromParameters(double dLatitude, double dLongitude, double dTimeZoneHours, int iDaylightMinutes, int iYear, int iMonth, int iDay, double dHours, bool bFast);
RDK_DEPRECATED RHRDK_SDK double SunJulianDay(double dTimeZoneHours, int iDaylightMinutes, int iYear, int iMonth, int iDay, double dHours);
///////////////////////////////////////////////////////////////////////////////////////////////////

enum // Flags for showing_sections parameter of RhRdkModalEditSun.
{
	rdk_sun_ss_sun_options   = 0x00000001, // Sun Options section.
	rdk_sun_ss_skylight      = 0x00000002, // Skylight section.
	rdk_sun_ss_sun_position  = 0x00000004, // Sun Position section.
	rdk_sun_ss_date_and_time = 0x00000008, // Date and Time section (only visible if manual control is off).
	rdk_sun_ss_location      = 0x00000010, // Location section (only visible if manual control is off).
	rdk_sun_ss_settings      = 0x00000020, // Settings section.
	rdk_sun_ss_all           = 0x000000FF, // All sections.
	rdk_sun_ss_force_32_bit  = 0xFFFFFFFF, // For future compatibility.
};

enum // Flags for section_options parameter of RhRdkModalEditSun.
{
	rdk_sun_so_normal       = 0x00000000, // Normal UI with no specializations.
	rdk_sun_so_lean_ui      = 0x00000001, // Lean UI. Currently this only hides the Time Zone control on the Location section.
	rdk_sun_so_no_shadow    = 0x00000002, // Suppresses the day/night shadow on the map control.
	rdk_sun_so_force_32_bit = 0xFFFFFFFF, // For future compatibility.
};

RHRDK_SDK bool RhRdkModalEditSun(ON_Sun& sun);
RHRDK_SDK bool RhRdkModalEditSun(ON_Sun& sun, ON__UINT32 showing_sections, ON__UINT32 section_options);
