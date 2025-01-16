
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"
#include "RhRdkColor.h"
#include "RhRdkCompoundContentImplementor.h"
#include "Proto_ON_Environment.h"

class CRhRdkTexture;

class RHRDK_SDK CRhRdkBasicEnvironmentCCI : public CRhRdkCompoundContentImplementor
{
public:
	CRhRdkBasicEnvironmentCCI();
	virtual ~CRhRdkBasicEnvironmentCCI();

	/** \return Shader for target renderer. */
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const = 0;

	/** Modify an Proto_ON_Environment to simulate this environment as best you can. */
	virtual bool GetSimulation(CRhRdkSimulatedEnvironment&) const { return false; }

	virtual CRhRdkColor BackgroundColor(void) const;
	virtual void SetBackgroundColor(const CRhRdkColor& col);

	virtual CRhRdkSimulatedEnvironment::BackgroundProjections Projection(void) const;
	virtual void SetProjection(CRhRdkSimulatedEnvironment::BackgroundProjections proj);

	virtual const CRhRdkTexture* Texture(void) const;
	virtual void SetTexture(const CRhRdkTexture* pTexture);

	virtual bool DisableLighting(void) const;

public: // Implement CRhRdkCompoundContentImplementor.
	virtual UUID Kind(void) const;
	virtual bool IsBuiltIn(void) const { return false; }
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

private:
	class CImpl;
	CImpl* m_pImpl;
};
