
#pragma once // Public SDK header

/** \class IRhRdkSkylight

	This interface is deprecated and only kept for backward compatibility.

*/
/*RDK_DEPRECATED*/ class RHRDK_SDK IRhRdkSkylight
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkSkylight& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** \return \e true if the skylight is on, else \e false. */
	virtual bool On(void) const = 0;

	/** \return The skylight shadow intensity. This is unused at present. */
	virtual double ShadowIntensity(void) const = 0;

	/** \return \e true if the skylight custom environment is on, else \e false. */
	virtual bool CustomEnvironmentOn(void) const = 0;

	/** \return The skylight custom environment instance id.
		If none has been set, returns ON_nil_uuid.
		\note If you are a rendering engine, please prefer
		 CRhRdkDocument::CurrentEnvironment().Get(IRhRdkCurrentEnvironment::Usage::Skylighting, IRhRdkCurrentEnvironment::Purpose::Render);
		 This includes all of the logic to check which environment should be used for which purpose,
		 and how the environments should defer to each other when one is not available. */
	virtual UUID CustomEnvironment(void) const = 0;

	/** Set the skylight on or off. */
	virtual void SetOn(bool bOn) = 0;

	/** Set the skylight shadow intensity. This is unused at present. */
	virtual void SetShadowIntensity(double dIntensity) = 0;

	/** Set the skylight custom environment on or off. */
	virtual void SetCustomEnvironmentOn(bool bOn) = 0;

	/** Set the skylight custom environment instance id.
		\param uuidInstance is the instance id of the environment to use. */
	virtual void SetCustomEnvironment(const UUID& uuidInstance) = 0;

	/** Call this before doing multiple 'Set' operations. */
	virtual void BeginNotifyBracket(void) = 0;

	/** Call this after doing multiple 'Set' operations. If one of the operations
		modified the document skylight, the OnSkylightSettingsChanged event will be sent. */
	virtual void EndNotifyBracket(void) = 0;

	/** \return \e true if this skylight is equal to another one. */
	virtual bool IsEqual(const IRhRdkSkylight& sl) const = 0;

	/** Copies another skylight to this one. */
	virtual void CopyFrom(const IRhRdkSkylight& sl) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

	/** Create a new free-floating skylight object that can be used for storage purposes. */
	static IRhRdkSkylight* New(void);

	virtual ~IRhRdkSkylight() { }

	// These are only here to allow duck-typing to work with ON_Skylight.
	bool Enabled(void) const { return On(); }
	void SetEnabled(bool b) { SetOn(b); }
};
