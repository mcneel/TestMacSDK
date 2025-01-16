
#pragma once // Public SDK header -- deprecated / obsolete classes; not implemented. [SDK_UNFREEZE]

enum RDK_DEPRECATED eRdkDitheringMethod { dm_none, dm_floyd_steinberg, dm_simple_noise };

/** \class IRhRdkDithering

	This interface is deprecated and only kept for backward compatibility.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkDithering
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkDithering& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

#pragma region RH_C_SHARED_ENUM [IRhRdkDithering::Methods] [Rhino.Render.Dithering.Methods] [nested]
	/// <summary>
	/// Dithering algorithm.
	/// </summary>
	enum class Methods : unsigned int
	{
		/// <summary>No dithering</summary>
		None, // OBSOLETE - not used except in old files.
		/// <summary>Floyd Steinberg algorithm</summary>
		FloydSteinberg,
		/// <summary>Simple random noise</summary>
		SimpleNoise,
	};
#pragma endregion

	/** Get the dithering method */
	virtual Methods Method(void) const = 0;

	/** Set the dithering method */
	virtual void SetMethod(Methods m) = 0;

	/** \return \e true if this object is equal to another one. */
	virtual bool IsEqual(const IRhRdkDithering& dit) const = 0;

	/** Copy another dithering object to this one. */
	virtual void CopyFrom(const IRhRdkDithering& dit) = 0;

	/** Emergency virtual function for future expansion. Currently implemented wszFunc:
		"On"    (pvData = null; returns pointer to bool).
		"SetOn" (pvData is pointer to bool). */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

	bool On(void) const;  // [SDK_UNFREEZE]
	void SetOn(bool bOn); // [SDK_UNFREEZE]

	static IRhRdkDithering* New(void);

	virtual ~IRhRdkDithering() { }
};

/** \class IRhRdkToneMapping [SDK_UNFREEZE]

	This interface is deprecated and only kept for backward compatibility.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkToneMapping
{
public:
	enum class Methods : unsigned int
	{
		None,
		BlackWhitePoint,
		Logarithmic,
	};

	/** Get whether or not tone mapping will be applied during rendering. */
	virtual bool ApplyWhileRendering(void) const = 0;

	/** Set whether or not tone mapping will be applied during rendering. */
	virtual void SetApplyWhileRendering(bool b) = 0;

	/** Get tone mapping method. */
	virtual IRhRdkToneMapping::Methods Method(void) const = 0;

	/** Set tone mapping method. */
	virtual void SetMethod(IRhRdkToneMapping::Methods) = 0;

	/** Get tone mapping params. */
	virtual void GetParams(OUT IRhRdk_XMLSection& section) const = 0;

	/** Set tone mapping params. */
	virtual void SetParams(const IRhRdk_XMLSection& section) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkToneMapping() { }
};

/** \class IRhRdkImageAdjust [SDK_UNFREEZE]

	OBSOLETE -- NOT IMPLEMENTED -- DO NOT USE

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkImageAdjust
{
public:
	virtual IRhRdkDithering::Methods Dither(void) const = 0;
	virtual void SetDither(IRhRdkDithering::Methods dm) = 0;
	virtual float Gamma(void) const = 0;
	virtual void SetGamma(float f) = 0;
	virtual IRhRdkToneMapping& ToneMapping(void) const = 0;
	virtual bool IsEqual(const IRhRdkImageAdjust& a) const = 0;
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkImageAdjust() { }
};

/** \class CRhRdkImageAdjust

	This class is obsolete and only kept for SDK backward compatibility. It is not implemented.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkImageAdjust : public CRhRdkObject, public IRhRdkImageAdjust
{
public:
	CRhRdkImageAdjust();
	CRhRdkImageAdjust(const CRhRdkImageAdjust& a);
	virtual ~CRhRdkImageAdjust();
	const CRhRdkImageAdjust& operator = (const CRhRdkImageAdjust& a);
	const CRhRdkImageAdjust& operator = (const IRhRdkImageAdjust& a);
	bool operator == (const IRhRdkImageAdjust& a) const;
	bool operator == (const CRhRdkImageAdjust& a) const;
	bool operator != (const CRhRdkImageAdjust& a) const;
	virtual IRhRdkDithering::Methods Dither(void) const override;
	virtual void SetDither(IRhRdkDithering::Methods dm) override;
	virtual float Gamma(void) const override;
	virtual void SetGamma(float f) override;
	virtual IRhRdkToneMapping& ToneMapping(void) const override;
	virtual bool IsEqual(const IRhRdkImageAdjust& a) const override;
	virtual void* EVF(const wchar_t*, void*) override { return nullptr; }

protected:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkDocRenderedImageAdjust

	This class is obsolete and only kept for SDK backward compatibility. It is not implemented.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkDocRenderedImageAdjust : public CRhRdkImageAdjust
{
public:
	CRhRdkDocRenderedImageAdjust(const CRhinoDoc& doc);
};
