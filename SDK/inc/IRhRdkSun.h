
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

#define RDK_SUN_MIN_YEAR 1971 // Minimum allowed year for sun methods.
#define RDK_SUN_MAX_YEAR 2199 // Maximum allowed year for sun methods.

#define RDK_SUN_MIN_TIME 31536000
#define RDK_SUN_MAX_TIME 7226726399

/** \class IRhRdkSunRO

	This interface is deprecated and only kept for backward compatibility.

*/
/*RDK_DEPRECATED*/ class RHRDK_SDK IRhRdkSunRO
{
public:
	/** \return \e true if enabling/disabling the sun is allowed, else \e false. */
	virtual bool EnableAllowed(void) const = 0;

	/** \return \e true if the sun is enabled, else \e false. */
	virtual bool EnableOn(void) const = 0;

	/** \return \e true if the manual control of the sun position is allowed, else \e false. */
	virtual bool ManualControlAllowed(void) const = 0;

	/** \return \e true if manual control of the sun position is in effect, else \e false. */
	virtual bool ManualControlOn(void) const = 0;

	/** \return The world angle corresponding to North in degrees.
		This angle is zero along the x-axis and increases anticlockwise. */
	virtual double North(void) const = 0;

	/** \return The sun's direction vector in world space, taking into account the sun's azimuth &
		altitude and the direction of north. \see Azimuth() \see Altitude() \see North().
		\note This method only converts the sun's current azimuth, altitude and north to a vector.
		It does not recompute the sun's position. If you want to compute and get the vector, please
		call the Light() method and then call Direction() on the returned ON_Light. \see Light() */
	virtual ON_3dVector Vector(void) const = 0;

	/** \return The azimuth of the sun in degrees. The value increases Eastwards with North as zero.
		\note This value is not affected by the direction of north. \see North() */
	virtual double Azimuth(void) const = 0;

	/** \return The altitude of the sun in degrees. */
	virtual double Altitude(void) const = 0;

	/** \return The latitude of the observer. */
	virtual double Latitude(void) const = 0;

	/** \return The longitude of the observer. */
	virtual double Longitude(void) const = 0;

	/** \return The time zone of the observer in hours. */
	virtual double TimeZone(void) const = 0;

	/** \return \e true if daylight saving is on, else \e false. */
	virtual bool DaylightSavingOn(void) const = 0;

	/** \return The daylight saving of the observer in minutes. */
	virtual int DaylightSavingMinutes(void) const = 0;

	/** Retrieves the local date and time of the observer.
		\param iYearOut accepts the year in the range 1800 to 2199.
		\param iMonthOut accepts the month in the range 1 to 12.
		\param iDayOut accepts the day in the range 1 to 31.
		\param dHoursOut accepts the time expressed as decimal hours in the range 0 to 24. */
	virtual void LocalDateTime(int& iYearOut, int& iMonthOut, int& iDayOut, double& dHoursOut) const = 0;

	/** Retrieves the date and time of the observer as Coordinated Universal Time (UTC).
		\param iYearOut accepts the year in the range RDK_SUN_MIN_YEAR to RDK_SUN_MAX_YEAR.
		\param iMonthOut accepts the month in the range 1 to 12.
		\param iDayOut accepts the day in the range 1 to 31.
		\param dHoursOut accepts the time expressed as decimal hours in the range 0 to 24. */
	virtual void UTCDateTime(int& iYearOut, int& iMonthOut, int& iDayOut, double& dHoursOut) const = 0;

	/** Get an ON_Light which represents the sun. If manual control is in effect, no sun calculation
		is performed; the function uses the values last used in calls to SetAzimuth(), SetAltitude()
		or SetVector(). If manual control is not in effect, the observer's position, date, time,
		time zone and daylight saving values are used to calculate the position of the sun.
		\note The sun light can be disabled. Remember to call IsEnabled() on the ON_Light before use. */
	virtual ON_Light Light(void) const = 0;

	/** return the intensity to be used for the sun. This is 1.0 by default. */
	virtual_su double Intensity(void) const; // [SDK_UNFREEZE]

	/** ShadowIntensity is currently unused. */
	virtual double ShadowIntensity(void) const = 0;

	/** \return the CRC of the sun state. */
	virtual DWORD CRC(void) const = 0;

	/** \return \e true if all the sun parameters are valid,
		 or \e false if something is 'nonsensical'. */
	virtual bool IsValid(void) const = 0;

	/** \return \e true if this sun is equal to another one. */
	virtual bool IsEqual(const class IRhRdkSun& other) const = 0;

	/** Saves the sun to a file. */
	virtual bool SaveToFile(const wchar_t* wszFullPath) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkSunRO() { }
};

/** \class IRhRdkSun

	This interface is deprecated and only kept for backward compatibility.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkSun : public IRhRdkSunRO
{
public:
	virtual ~IRhRdkSun() { }

	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkSun& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** Set whether or not enabling/disabling the sun is allowed. */
	virtual void SetEnableAllowed(bool bAllowed) = 0;

	/** Set whether or not the sun is enabled. */
	virtual void SetEnableOn(bool bOn) = 0;

	/** Set whether or not manual control of the sun position is allowed. */
	virtual void SetManualControlAllowed(bool bAllowed) = 0;

	/** Set whether or not manual control of the sun position is in effect. */
	virtual void SetManualControlOn(bool bManual) = 0;

	/** Set the azimuth corresponding to North.
		\param dNorth is the world angle corresponding to North in degrees in the range 0 to 360.
		This angle is zero along the x-axis and increases anticlockwise. */
	virtual void SetNorth(double dNorth) = 0;

	/** Set the sun's direction vector when manual control is in effect. This calculates and sets
		the sun's azimuth & altitude and takes into account the direction of north.
		\see SetAzimuth() \see SetAltitude() \see North(). */
	virtual void SetVector(const ON_3dVector& v) = 0;

	/** Set the azimuth of the sun when manual control is in effect.
		\param dAzimuth is the sun's azimuth in degrees. The value increases Eastwards with North as zero.
		\note This value is not affected by the direction of north. \see North() */
	virtual void SetAzimuth(double dAzimuth) = 0;

	/** Set the sun's altitude when manual control is in effect.
		\param dAltitude is the sun's altitude in degrees in the range -90 to +90. */
	virtual void SetAltitude(double dAltitude) = 0;

	/** Set the latitude of the observer.
		\param dLatitude is the observer's latitude in degrees in the range -90 to +90. */
	virtual void SetLatitude(double dLatitude) = 0;

	/** Set the longitude of the observer.
		\param dLongitude is the observer's longitude in degrees in the range -180 to +180. */
	virtual void SetLongitude(double dLongitude) = 0;

	/** Set the time zone of the observer in hours, in the range -12 to +13. */
	virtual void SetTimeZone(double dHours) = 0;

	/** Set whether or not the observer is using daylight saving time. */
	virtual void SetDaylightSavingOn(bool bOn) = 0;

	/** Set the daylight saving of the observer in minutes, in the range 0 to 120. */
	virtual void SetDaylightSavingMinutes(int iMinutes) = 0;

	/** Set the local date and time of the observer.
		\param iYear is the year which must lie between RDK_SUN_MIN_YEAR and RDK_SUN_MAX_YEAR inclusive.
		\param iMonth is the month in the range 1 to 12.
		\param iDay is the day in the range 1 to 31.
		\param dHours is the time expressed as decimal hours in the range 0 to 24. */
	virtual void SetLocalDateTime(int iYear, int iMonth, int iDay, double dHours) = 0;

	/** Set the date and time of the observer as Coordinated Universal Time (UTC).
		\param iYear is the year which must lie between RDK_SUN_MIN_YEAR and RDK_SUN_MAX_YEAR inclusive.
		\param iMonth is the month in the range 1 to 12.
		\param iDay is the day in the range 1 to 31.
		\param dHours is the time expressed as decimal hours in the range 0 to 24. */
	virtual void SetUTCDateTime(int iYear, int iMonth, int iDay, double dHours) = 0;

	/** ShadowIntensity is currently unused. */
	virtual void SetShadowIntensity(double dIntensity) = 0;

	/** Set the intensity to be used for the sun. This is 1.0 by default. */
	virtual_su void SetIntensity(double dIntensity); // [SDK_UNFREEZE]

	/** Load the sun from a file. */
	virtual bool LoadFromFile(const wchar_t* wszFullPath) = 0;

	/** Copy this sun from another one. */
	virtual void CopyFrom(const IRhRdkSun& other) = 0;

	/** Call this before doing multiple 'Set' operations. */
	virtual void BeginNotifyBracket(void) = 0;

	/** Call this after doing multiple 'Set' operations. */
	virtual void EndNotifyBracket(void) = 0;

	/** Edits the sun in a pop-up editor. */
	virtual bool Edit(void) = 0;

	static IRhRdkSun* New(void);

	virtual bool WriteToArchive(ON_BinaryArchive& archive) const = 0;
	virtual bool ReadFromArchive(ON_BinaryArchive& archive) = 0;

	// OBSOLETE. Not called.
	virtual void _InternalSetAziAlt(double, double) = 0;
};
