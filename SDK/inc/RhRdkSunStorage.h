
#pragma once // Public SDK header

#include "RhRdkVariant.h"

/** \class CRhRdkSunStorage

	This class is deprecated and only here to avoid breaking the SDK.

	None of the methods are implemented since it's almost certain that no 3rd-party was using this class.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkSunStorage : public CRhRdkObject, public IRhRdkSun
{
public:
	void ToSun(class IRhRdkSun& sun) const;

public: // Sun interface.
	virtual bool EnableAllowed(void) const override;
	virtual void SetEnableAllowed(bool bOn) override;
	virtual bool EnableOn(void) const override;
	virtual void SetEnableOn(bool bOn) override;
	virtual bool ManualControlOn(void) const override;
	virtual void SetManualControlOn(bool bOn) override;
	virtual bool ManualControlAllowed(void) const override;
	virtual void SetManualControlAllowed(bool bEnabled) override;
	virtual double North(void) const override;
	virtual void SetNorth(double dNorth) override;
	virtual double Azimuth(void) const override;
	virtual void SetAzimuth(double dAzimuth) override;
	virtual double Altitude(void) const override;
	virtual void SetAltitude(double dAltitude) override;
	virtual double Latitude(void) const override;
	virtual void SetLatitude(double dLatitude) override;
	virtual double Longitude(void) const override;
	virtual void SetLongitude(double dLongitude) override;
	virtual double TimeZone(void) const override;
	virtual void SetTimeZone(double dHours) override;
	virtual int DaylightSavingMinutes(void) const override;
	virtual void SetDaylightSavingMinutes(int iMinutes) override;
	virtual bool DaylightSavingOn(void) const override;
	virtual void SetDaylightSavingOn(bool bOn) override;
	virtual double ShadowIntensity(void) const override;
	virtual void SetShadowIntensity(double dIntensity) override;
	virtual void LocalDateTime(int& iYearOut, int& iMonthOut, int& iDayOut, double& dHoursOut) const override;
	virtual void SetLocalDateTime(int iYear, int iMonth, int iDay, double dHours) override;
	virtual void UTCDateTime(int& iYearOut, int& iMonthOut, int& iDayOut, double& dHoursOut) const override;
	virtual void SetUTCDateTime(int iYear, int iMonth, int iDay, double dHours) override;
	virtual void _InternalSetAziAlt(double, double) override;
	virtual void* EVF(const wchar_t*, void*) override;

public:
	virtual void SetTimeZone(double dHours, bool bAutoAdjustTime);

public: // Notification.
	virtual void HandleNotify(void);
	virtual void BeginNotifyBracket(void) override;
	virtual void EndNotifyBracket(void) override;

public: // Abstract interface.
	virtual void CopyFromStorage(const CRhRdkSunStorage& s) = 0;
	virtual bool LoadFromFile(const wchar_t* wszFullPath) override = 0;
	virtual bool SaveToFile(const wchar_t* wszFullPath) const override = 0;
	virtual CRhRdkVariant Value(const wchar_t* wszName, const CRhRdkVariant& vDefault) const = 0;
	virtual bool SetValue(const wchar_t* wszName, const CRhRdkVariant& v) = 0;
	virtual void SetAziAlt(double dAzi, double dAlt) const = 0;
};
