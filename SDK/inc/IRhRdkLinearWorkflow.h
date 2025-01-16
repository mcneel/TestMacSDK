
#pragma once // Public SDK header

/** \class IRhRdkLinearWorkflow

	This interface is deprecated and only kept for backward compatibility.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkLinearWorkflow
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkLinearWorkflow& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** \return linear workflow active state for textures (also check IsLinear()). */
	virtual bool PreProcessTextures(void) const = 0;

	/** \return linear workflow active state for individual colors. */
	virtual bool PreProcessColors(void) const = 0;

	/** \return whether the frame buffer should be gamma corrected. */
	virtual bool PostProcessFrameBuffer(void) const = 0;

	/** Set linear workflow (pre-process) active state for textures. */
	virtual void SetPreProcessTextures(bool b) = 0;

	/** Set linear workflow (pre-process) active state for individual colors. */
	virtual void SetPreProcessColors(bool b) = 0;

	/** Set linear workflow (pre-process) active state for individual colors. */
	virtual void SetPostProcessFrameBuffer(bool b) = 0;

	/** \return pre-process gamma for input textures and colors. */
	virtual float PreProcessGamma(void) const = 0;

	/** \return post-process gamma for frame buffer. This is not the value applied, this is the value that
		 appears in the UI. \see PostProcessGammaReciprocal(). */
	virtual float PostProcessGamma(void) const = 0;

	/** \return gamma reciprocal for frame buffer. This is the value that is applied to each color channel
		 for post-process and is supplied here as an optimization. */
	virtual float PostProcessGammaReciprocal(void) const = 0;

	/** Set pre-process gamma. This will generally be >= 1.0 (usually 2.2).
		This is the actual value applied in pre-process. */
	virtual void SetPreProcessGamma(float f) = 0;

	/** Set post-process gamma. This will generally be >= 1.0 (usually 2.2).
		The actual value to be applied in post-process is the reciprocal. */
	virtual void SetPostProcessGamma(float f) = 0;

	/** \return CRC of gamma and linear workflow active state. */
	virtual ON__UINT32 ComputeCRC(void) const = 0;

	/** Applies pre-process gamma correction to a color if linear workflow is active. 
		\param bForTexture is \e true if the color is part of a texture.
		See notes below for checking CRhRdkTexture::IsLinear(). */
	virtual void ApplyPreProcessGamma(CRhRdkColor& col, bool bForTexture) const = 0;

	/** Applies pre-process gamma correction to a dib (i.e., an LDR texture) if PreProcessTextures is true.
		\note If this texture comes from a CRhRdkTexture, you should first check CRhRdkTexture::IsLinear().
		 If the texture is linear, it will require no further processing. */
	virtual void ApplyPreProcessGamma(CRhinoDib& dibInOut) const = 0;

	/** Modifies a DIB by applying gamma correction using the gamma reciprocal.
		\note This method replaces the NewGammaCorrectedBitmap() method.
		\param dibInOut is the DIB which is altered if gamma is not equal to 1. */
	virtual void ApplyPostProcessGamma(CRhinoDib& dibInOut) const = 0;

	/** Copy another linear workflow object to this one. */
	virtual void CopyFrom(const IRhRdkLinearWorkflow& lw) = 0;

	/** \return \e true if this object is equal to another one. */
	virtual bool IsEqual(const IRhRdkLinearWorkflow& lw) const = 0;

	/** Emergency virtual function for future expansion. Currently implemented wszFunc:
		"PostProcessGammaOn"    (pvData = null; returns pointer to bool).
		"SetPostProcessGammaOn" (pvData is pointer to bool). */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

	bool PostProcessGammaOn(void) const; // [SDK_UNFREEZE]
	void SetPostProcessGammaOn(bool);    // [SDK_UNFREEZE]

	static IRhRdkLinearWorkflow* New(void);

	virtual ~IRhRdkLinearWorkflow() { }

	// These are only here to allow duck-typing to work with ON_LinearWorkflow.
	bool PreProcessTexturesOn(void)     const { return PreProcessTextures(); }
	void SetPreProcessTexturesOn(bool b)      { SetPreProcessTextures(b); }
	bool PreProcessColorsOn(void)       const { return PreProcessColors(); }
	void SetPreProcessColorsOn(bool b)        { SetPreProcessColors(b); }
	bool PostProcessFrameBufferOn(void) const { return PostProcessFrameBuffer(); }
	void SetPostProcessFrameBufferOn(bool b)  { SetPostProcessFrameBuffer(b); }
};
