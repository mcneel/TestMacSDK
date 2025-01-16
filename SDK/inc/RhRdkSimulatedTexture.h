
#pragma once // Public SDK header

#include "RhRdkTexture.h"

class RHRDK_SDK CRhRdkSimulatedTexture : public CRhRdkObject
{
public:
	RDK_DEPRECATED CRhRdkSimulatedTexture();
	virtual ~CRhRdkSimulatedTexture();

	CRhRdkSimulatedTexture(const CRhinoDoc* pDocAssoc);

	RDK_DEPRECATED CRhRdkSimulatedTexture(const ON_Texture& src);
	CRhRdkSimulatedTexture(const CRhinoDoc* pDocAssoc, const ON_Texture& src);

	CRhRdkSimulatedTexture(const CRhRdkSimulatedTexture& src);
	const CRhRdkSimulatedTexture& operator=(const CRhRdkSimulatedTexture& src);

	bool operator == (const CRhRdkSimulatedTexture& src) const;

	virtual ON_Xform LocalMappingTransform(void) const;

public:
	virtual ON_wString Filename(void) const;
	
	RDK_DEPRECATED virtual void SetFilename(const ON_wString& sFilename, bool bAllowSearch = true);

	virtual_su void SetFilename(const ON_wString& sFilename, const CRhinoDoc* doc, bool bAllowSearch = true);

	virtual ON_wString OriginalFilename(void) const;

	/** \note Initialized to (1.0, 1.0) - texture repeat (ie - 1.0 / scale). */
	virtual ON_2dVector Repeat(void) const;

	virtual void SetRepeat(const ON_2dVector& repeat);
	virtual void SetRepeat(double s, double t);

	/** \note Initialized to (0.0, 0.0) - offset in texture UV space. */
	virtual ON_2dVector Offset(void) const;

	virtual void SetOffset(const ON_2dVector& offset);
	virtual void SetOffset(double s, double t);

	/** \note Initialized to 0.0 - radians, anti-clockwise. */
	virtual double Rotation(void) const;
	virtual void SetRotation(double dRotation);

	/** \note Initialized to CRhRdkTexture::repeating. */
	virtual CRhRdkTexture::WrapTypes WrapType(void) const;
	virtual void SetWrapType(CRhRdkTexture::WrapTypes wt);

	virtual void SetMappingAndProjection(CRhRdkTexture::ProjectionModes pm, int iMappingChannel, CRhRdkTexture::EnvironmentMappingModes e);

	/** \note Initialized to 1. */
	virtual int MappingChannel(void) const;
	RDK_DEPRECATED virtual void SetMappingChannel(int iChannel);

	/** \note Initialized to mapping_channel. */
	virtual CRhRdkTexture::ProjectionModes ProjectionMode(void) const;
	RDK_DEPRECATED virtual void SetProjectionMode(CRhRdkTexture::ProjectionModes);

	virtual bool Filtered(void) const;
	virtual void SetFiltered(bool);

	/** Calling GetColorAdjuster() can only be done once - it will return NULL the second time.
	The call transfers ownership of the IColorAdjuster object to the caller. */
	virtual CRhRdkTexture::IColorAdjuster* GetColorAdjuster(void) const;

	/** The call transfers ownership of the IColorAdjuster object to this object. */
	virtual void SetColorAdjuster(CRhRdkTexture::IColorAdjuster*);

	RDK_DEPRECATED double UnitsToMeters(double dUnits) const;
	RDK_DEPRECATED double MetersToUnits(double dMeters) const;

	virtual double UnitsToMeters(const CRhinoDoc& doc, double dUnits) const;
	virtual double MetersToUnits(const CRhinoDoc& doc, double dMeters) const;

	// WARNING: The returned reference will be invalid if another CRhRdkSimulatedTexture is
	// copied over this one. The underlying ON_Texture gets completely replaced in that case.
	virtual ON_Texture& OnTexture(void);
	virtual const ON_Texture& OnTexture(void) const;

	/** OBSOLETE; returns false. */
	virtual bool HasTransparentColor(void) const;

	/** OBSOLETE; not implemented. */
	virtual void SetHasTransparentColor(bool);

	/** OBSOLETE; returns black. */
	virtual CRhRdkColor TransparentColor(void) const;

	/** OBSOLETE; not implemented. */
	virtual void SetTransparentColor(const CRhRdkColor&);

	/** OBSOLETE; returns 0.0 */
	virtual double TransparentColorSensitivity() const;

	/** OBSOLETE; not implemented. */
	virtual void SetTransparentColorSensitivity(double);

	// Used by SimulateTexture and NewBitmapTexture to leave document information
	// on the simulated texture to determine whether the WCS scale needs to change
	// during a simulation->desimulation cycle.
	const CRhinoDoc* DocumentAssoc(void) const;
	void SetDocumentAssoc(const CRhinoDoc*);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
