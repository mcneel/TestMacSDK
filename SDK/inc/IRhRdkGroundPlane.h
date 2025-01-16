
#pragma once // Public SDK header

/** \class IRhRdkGroundPlane

	This interface is deprecated and only kept for backward compatibility.

*/
/*RDK_DEPRECATED*/ class RHRDK_SDK IRhRdkGroundPlane
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkGroundPlane& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** \return \e true if the ground plane is enabled, else \e false. */
	virtual bool On(void) const = 0;

	/** \return \e true if ground plane backface is enabled, else \e false. */
	virtual bool ShowUnderside(void) const = 0;

	/** \return The altitude of the ground plane. */
	virtual double Altitude(void) const = 0;

	/** \return \e true if auto-altitude is enabled. */
	virtual bool AutoAltitude(void) const = 0;

	/** \return \e true if the ground plane is set to shadow-only. */
	virtual bool ShadowOnly(void) const = 0;

	/** \return The instance id of the ground plane's material. */
	virtual UUID MaterialInstanceId(void) const = 0;

	/** \return The texture offset of the ground plane in model units. */
	virtual ON_2dVector TextureOffset(void) const = 0;

	/** \return \e true if the texture offset x and y are locked together. */
	virtual bool TextureOffsetLocked(void) const = 0;

	/** \return \e true if the texture repeat x and y are locked together. */
	virtual bool TextureRepeatLocked(void) const = 0;

	/** \return The texture size of the ground plane in model units. */
	virtual ON_2dVector TextureSize(void) const = 0;

	/** \return The texture rotation of the ground plane in degrees. */
	virtual double TextureRotation(void) const = 0;

	/** \return A pointer to the ground plane's material or null if none. */
	virtual const class CRhRdkMaterial* Material(void) const = 0;

	/** Set the ground plane enabled state. */
	virtual void SetOn(bool) = 0;

	/** Set if the ground plane backface is enabled. */
	virtual void SetShowUnderside(bool) = 0;

	/** Set the ground plane's altitude. */
	virtual void SetAltitude(double) = 0;

	/** Set if the ground plane is set to auto-altitude. */
	virtual void SetAutoAltitude(bool) = 0;

	/** Set if the ground plane is set to shadow-only. */
	virtual void SetShadowOnly(bool) = 0;

	/** Set the instance id of the ground plane's material. */
	virtual void SetMaterialInstanceId(const UUID&) = 0;

	/** Set the texture offset of the ground plane in model units. */
	virtual void SetTextureOffset(const ON_2dVector&) = 0;

	/** Set if the texture offset x and y are locked together. */
	virtual void SetTextureOffsetLocked(bool) = 0;

	/** Set if the texture repeat x and y are locked together. */
	virtual void SetTextureRepeatLocked(bool) = 0;

	/** Set the texture size of the ground plane in model units. */
	virtual void SetTextureSize(const ON_2dVector&) = 0;

	/** Set the texture rotation of the ground plane in degrees. */
	virtual void SetTextureRotation(double) = 0;

	/** \return \e true if this ground plane is equal to another one. */
	virtual bool IsEqual(const IRhRdkGroundPlane& gp) const = 0;

	/** Copy this ground plane from another one. */
	virtual void CopyFrom(const IRhRdkGroundPlane& gp) = 0;

	/** Call this before calling multiple 'Set' methods. */
	virtual void BeginNotifyBracket(void) = 0;

	/** Call this after calling multiple 'Set' methods. */
	virtual void EndNotifyBracket(void) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	static IRhRdkGroundPlane* New(void);

	virtual ~IRhRdkGroundPlane() { }

	// These are only here to allow duck-typing to work with ON_GroundPlane.
	bool Enabled(void) const { return On(); }
	void SetEnabled(bool b) { SetOn(b); }
	bool TextureSizeLocked(void) const { return TextureRepeatLocked(); }
	void SetTextureSizeLocked(bool b) { SetTextureRepeatLocked(b); }

private:
	void operator = (const IRhRdkGroundPlane&) = delete;
};
