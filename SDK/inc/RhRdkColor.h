
#pragma once // Public SDK header.

#include "RhRdkDefinitions.h"

/* \class CRhRdkColor

	A simple floating point color class.

*/
class RHRDK_SDK CRhRdkColor final
{
public:
	static CRhRdkColor black;
	static CRhRdkColor white;

public:
	/** Construct as solid black (0, 0, 0, 1). */
	CRhRdkColor();

	/** Construct from float array. \note There \b must be four floats in the array. */
	CRhRdkColor(const float* d);

	/** Construct from floats (0.0f to MAX_FLOAT). */
	CRhRdkColor(float fRed, float fGreen, float fBlue, float fAlpha=1.0f);

	/** Construct from integers (0 to 255). */
	CRhRdkColor(int iRed, int iGreen, int iBlue, int iAlpha=255);

#if defined ON_RUNTIME_WIN
	/** Construct from lowest 24 bits of a COLORREF. Alpha is set to 1.0f. */
	explicit CRhRdkColor(COLORREF c);
#endif

	/** Construct from ON_Color. */
	explicit CRhRdkColor(const ON_Color& c);

	/** Construct from ON_4fColor. */
	explicit CRhRdkColor(const ON_4fColor& c);

	/** Construct from another instance. */
	CRhRdkColor(const CRhRdkColor& c);

	/** Direct read access to array of four floats.
		\warning If you access the array beyond the 4th element, the function may crash Rhino. */
	const float* FloatArray(void) const;

	/** Direct read/write access to array of four floats.
		\warning If you access the array beyond the 4th element, the function may crash Rhino. */
	float* FloatArray(void);

	/** Get color as COLORREF (ignores alpha). */
	COLORREF ColorRef(void) const;

	/** Get color as COLORREF (with alpha in high byte). */
	COLORREF ColorRefAlpha(void) const;

	/** Get color as ON_Color. */
	ON_Color OnColor(void) const;

	/** Get color as ON_4fColor. */
	ON_4fColor On4fColor(void) const;

	/** Check if colors are \b exactly equal. */
	bool operator == (const CRhRdkColor& c) const;

	/** Add another color to this color with no clamping of alpha. */
	void Add(const CRhRdkColor& c);

	/** Subtract another color from this color with no clamping of alpha. */
	void Subtract(const CRhRdkColor& c);

	/** Multiply this color by a value with optional alpha. */
	void MultiplyBy(float f, bool bAffectAlpha);

	/** Add another color to this color.
		\note Alpha is clamped. */
	const CRhRdkColor& operator += (const CRhRdkColor& c);

	/** Add a value to this color.
		\note Alpha is clamped. */
	const CRhRdkColor& operator += (float d);

	/** Subtract another color from this color.
		\note Alpha is clamped. */
	const CRhRdkColor& operator -= (const CRhRdkColor& c);

	/** Subtract a value from this color.
		\note Alpha is clamped. */
	const CRhRdkColor& operator -= (float d);

	/** Multiply this color by another color.
		\note Alpha IS multiplied. */
	const CRhRdkColor& operator *= (const CRhRdkColor& c);

	/** Multiply this color by a value.
		\note Alpha is NOT multiplied. */
	const CRhRdkColor& operator *= (float d);

	/** Divide this color by another color.
		\note Alpha IS divided. */
	const CRhRdkColor& operator /= (const CRhRdkColor& c);

	/** Divide this color by a value.
		\note Alpha is NOT divided. */
	const CRhRdkColor& operator /= (float d);

	/** Add two colors. */
	friend CRhRdkColor operator + (const CRhRdkColor& c1, const CRhRdkColor& c2) { CRhRdkColor c(c1); c += c2; return c; }

	/** Subtract two colors. */
	friend CRhRdkColor operator - (const CRhRdkColor& c1, const CRhRdkColor& c2) { CRhRdkColor c(c1); c -= c2; return c; }

	/** Multiply two colors. */
	friend CRhRdkColor operator * (const CRhRdkColor& c1, const CRhRdkColor& c2) { CRhRdkColor c(c1); c *= c2; return c; }

	/** Multiply a value by a color. */
	friend CRhRdkColor operator * (float d, const CRhRdkColor& c1) { CRhRdkColor c(c1); c *= d; return c; }

	/** Multiply a color by a value. */
	friend CRhRdkColor operator * (const CRhRdkColor& c1, float d) { CRhRdkColor c(c1); c *= d; return c; }

	/** Divide a color by a color. */
	friend CRhRdkColor operator / (const CRhRdkColor& c1, const CRhRdkColor& c2) { CRhRdkColor c(c1); c /= c2; return c; }

	/** Divide a value by a color. */
	friend CRhRdkColor operator / (float d, const CRhRdkColor& c1);

	/** Divide a color by a value. */
	friend CRhRdkColor operator / (const CRhRdkColor& c1, float d) { CRhRdkColor c(c1); c /= d; return c; }

	/** Check if colors are not \b exactly equal. */
	bool operator != (const CRhRdkColor& c) const;

	/** Copy color from another instance. */
	CRhRdkColor& operator = (const CRhRdkColor& c);

#if defined ON_RUNTIME_WIN
	/** Copy color from lowest 24 bits of a COLORREF. Alpha is set to 1.0f. */
	CRhRdkColor& operator = (COLORREF c);
#endif

	/** Copy color from an ON_Color instance. */
	CRhRdkColor& operator = (const ON_Color& c);

	/** Copy color from an ON_4fColor instance. */
	CRhRdkColor& operator = (const ON_4fColor& c);

	/** Set color from float array. \note There \b must be four floats in the array */
	void Set(const float * d);

	/** Set color as floats (0.0 to MAX_FLOAT). */
	void Set(float fRed, float fGreen, float fBlue, float fAlpha=1.0f);

	/** Set color as doubles (0.0 to MAX_FLOAT). */
	void Set(double dRed, double dGreen, double dBlue, double dAlpha=1.0);

	/** Set color as ints (0 to 255). */
	void Set(int iRed, int iGreen, int iBlue, int iAlpha=255);

	/** Set color from lowest 24 bits of a COLORREF. Alpha is set to 1.0f. */
	void Set(COLORREF c);

	/** Set alpha channel only. */
	void SetAlpha(int iAlpha);

	/** Set alpha channel only. */
	void SetAlpha(float fAlpha);

	/** Get red value as float (0.0 to MAX_FLOAT). */
	float FRed(void) const;

	/** Get green value as float (0.0 to MAX_FLOAT). */
	float FGreen(void) const;

	/** Get blue value as float (0.0 to MAX_FLOAT). */
	float FBlue(void) const;

	/** Get alpha value as float (0.0 to MAX_FLOAT). */
	float FAlpha(void) const;

	/** Get red value as integer (0 to 255). */
	UINT IRed(void) const;

	/** Get green value as integer (0 to 255). Clamped. */
	UINT IGreen(void) const;

	/** Get blue value as integer (0 to 255). Clamped. */
	UINT IBlue(void) const;

	/** Get alpha value as integer (0 to 255). Clamped. */
	UINT IAlpha(void) const;

	/** Clamp each of the RGBA color components to be between 0.0 and 1.0. */
	void Clamp(void);

	/** Get the hue (0 to 360). */
	float Hue(void) const;

	/** Get the saturation (0 to 1). */
	float Saturation(void) const;

	/** Get the luminance (0 to 1). */
	float Luminance(void) const;

	/** Get the hue (0 to 360), saturation (0 to 1) and luminance (0 to 1). */
	void GetHSL(float& h, float& s, float& l) const;

	/** Set the hue (0 to 360), saturation (0 to 1) and luminance (0 to 1). */
	void SetHSL(float h, float s, float l);

	/** Blend this color to col using t from 0.0 to 1.0
	\note Does not clamp 't' or rgb result unless bClampAlpha is true, in which case alpha is clamped. */
	void BlendTo(float t, const CRhRdkColor& col, bool bClampAlpha=false);

	/** \return \e true if this color is equal to another color within the specified tolerance. */
	bool IsEqualWithinTolerance(const CRhRdkColor& col, double dTolerance) const;

	/** This method is deprecated in favor of the other one which is faster. */
	RDK_DEPRECATED bool ApplyLinearWorkflowGamma(const CRhinoDoc* pDoc);

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED bool ApplyLinearWorkflowGamma(const class IRhRdkLinearWorkflow& lw, bool bForTexture);

	/** Adjust the color for linear workflow gamma.
		If linear workflow is enabled, color is adjusted and the function returns \e true. Otherwise the function returns \e false. */
	bool ApplyLinearWorkflowGamma(const ON_LinearWorkflow& lw, bool bForTexture);

	/** Emergency function for future expansion. */
	void* EF(const wchar_t* wszFunc, void* pvData);

private:
	enum { r=0, g=1, b=2, a=3, num=4, force32bit_rgba=0xFFFFFFFF };
	float m_a[num];
};
