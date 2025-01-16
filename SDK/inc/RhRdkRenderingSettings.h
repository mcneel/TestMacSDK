
#pragma once

#include "RhRdkDefinitions.h"

// This class is deprecated and not implemented.

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkRenderingSettings
{
public:
	CRhRdkRenderingSettings(const CRhinoDoc* =nullptr) { }
	CRhRdkRenderingSettings(const CRhinoDoc&) { }
	CRhRdkRenderingSettings(CRhRdkDocument&) { }
	virtual ~CRhRdkRenderingSettings() { }

	bool EmbedOn(void) const;
	void SetEmbedOn(bool b);

	// Please call the equivalent methods in ON_3dmRenderSettings.
	bool UseCustomEnvironmentForReflectionAndRefraction(void) const;
	void SetUseCustomEnvironmentForReflectionAndRefraction(bool b);
	UUID CustomEnvironmentForReflectionAndRefraction(void) const;
	void SetCustomEnvironmentForReflectionAndRefraction(const UUID& uuidInstance);

	// Please call the equivalent methods in ON_3dmRenderSettings (Linearworkflow).
	double Gamma(void) const;
	void SetGamma(double d);
	bool UseLinearWorkflow(void) const;
	void SetUseLinearWorkflow(bool b);

	// Please call the equivalent methods in ON_3dmRenderSettings (Dithering).
	ON_wString Dithering(void) const;
	void SetDithering(const wchar_t* wsz);

	// OSBOLETE. Tone mapping is now a post-effect.
	ON_wString ToneMapping(void) const;
	void SetToneMapping(const wchar_t* wsz);
	void GetToneMapperParams(class CRhRdk_XMLSection& section) const;
	void SetToneMapperParams(const class CRhRdk_XMLSection& section);
	bool ApplyToneMappingWhileRendering(void) const;
	void SetApplyToneMappingWhileRendering(bool b);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*);
};
