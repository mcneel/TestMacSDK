
#pragma once // Public SDK header

/** \class IRhRdkCurrentEnvironment

	This is the interface to the current environments for background, skylighting and
	reflection / refraction.

	Ideally this interface would be deprecated and replaced by ON_3dmRenderSettings
	but we can't do that as the latter has no current environment interface.

	The Usage and Purpose enums have the same meaning as those in ON_3dmRenderSettings::EnvironmentUsage
	but the values are different so be careful about the possible need to convert between them.

*/
class RHRDK_SDK IRhRdkCurrentEnvironment
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkCurrentEnvironment& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	enum class Usage : unsigned int
	{
		Background  = 0x01, // Specifies the background environment.
		Reflection  = 0x02, // Specifies the custom reflective environment. Also used for refraction.
		Skylighting = 0x04, // Specifies the custom skylighting environment.

		Any = Background | Reflection | Skylighting // Only used in special cases. Not valid for calls to On(), Get(), etc.
	};

	enum class Purpose : unsigned int
	{
		Render = 0, // Used by renderers to get the logic for deferring to other environments when one is not available.
		Simple = 1, // Used to directly get and set the environment instance id.
	};

	/** Get if the current environment is 'on' for a particular usage. */
	virtual bool On(Usage usage) const = 0;

	/** Set if the current environment is 'on' for a particular usage. */
	virtual void SetOn(Usage usage, bool bOn) = 0;

	/** Get the current environment instance id. If there is no current environment,
		the function returns the default environment's instance id. */
	virtual UUID Get(Usage usage, Purpose p) const = 0;

	/** Set the current environment instance id for a specific usage. Note that passing ON_nil_uuid
		for a usage will remove that specialization. If a usage is ON_nil_uuid, that usage will revert
		to the same as the background usage. */
	virtual bool Set(Usage usage, const UUID& uuidInstance) = 0;

	/** Get the current environment. If there is no current environment,
		the function returns the default environment. */
	virtual const class CRhRdkEnvironment* GetEnv(Usage usage, Purpose p) const = 0;

	/** Copy this current environment from another one. */
	virtual void CopyFrom(const IRhRdkCurrentEnvironment& ce) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkCurrentEnvironment() { }
};
