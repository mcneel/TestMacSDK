
#pragma once // Public SDK header

#include "RhRdkSimulatedTexture.h"

class RHRDK_SDK CRhRdkSimulatedEnvironment  
{
public:
	CRhRdkSimulatedEnvironment();
	CRhRdkSimulatedEnvironment(const CRhRdkSimulatedEnvironment& src);
	virtual ~CRhRdkSimulatedEnvironment();

	const CRhRdkSimulatedEnvironment& operator = (const CRhRdkSimulatedEnvironment& src);

	bool operator == (const CRhRdkSimulatedEnvironment& src) const;

	RDK_DEPRECATED enum eBackgroundProjection { planar, spherical, emap, box, automatic, lightprobe, cubemap, vertical_cross_cubemap, horizontal_cross_cubemap, hemispherical, force32 = 0xFFFFFFFF };

	enum class BackgroundProjections : unsigned int
	{
		Planar      = 0,
		Spherical   = 1, // Equirectangular projection.
		Emap        = 2, // Mirror ball.
		Box         = 3,
		Automatic   = 4,
		Lightprobe  = 5,
		Cubemap     = 6,
		VerticalCrossCubemap = 7,
		HorizontalCrossCubemap = 8,
		Hemispherical = 9,
	};

	ON_Color BackgroundColor(void) const;
	void SetBackgroundColor(ON_Color color);

	const CRhRdkSimulatedTexture& BackgroundImage(void) const;
	void SetBackgroundImage(const CRhRdkSimulatedTexture& tex);

	BackgroundProjections BackgroundProjection(void) const;
	void SetBackgroundProjection(BackgroundProjections proj);

	static BackgroundProjections ProjectionFromString(const wchar_t* wsz);
	static const wchar_t* StringFromProjection(BackgroundProjections proj);

	static BackgroundProjections AutomaticProjectionFromChildTexture(const CRhRdkTexture* pChildTexture);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
