
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"
#include "RhRdkTexture.h"
#include "RhRdkCompoundContentImplementor.h"

class RHRDK_SDK CRhRdkBasicMaterialCCI : public CRhRdkCompoundContentImplementor
{
public:
	CRhRdkBasicMaterialCCI();
	virtual ~CRhRdkBasicMaterialCCI();

	virtual ON_Color Diffuse(void) const;
	virtual void  SetDiffuse(const ON_Color& col);

	virtual ON_Color Ambient(void) const;
	virtual void  SetAmbient(const ON_Color& col);

	virtual ON_Color Specular(void) const;
	virtual void  SetSpecular(const ON_Color& col);

	virtual ON_Color Emission(void) const;
	virtual void  SetEmission(const ON_Color& col);

	virtual double  Shine(void) const;
	virtual void SetShine(double d);

	virtual double  Transparency(void) const;
	virtual void SetTransparency(double d);

	virtual double  Ior(void) const;
	virtual void SetIor(double d);

	virtual bool IsFresnelEnabled(void) const;
	virtual void EnableFresnel(bool b);

	virtual double Polish(void) const;
	virtual void SetPolish(double d);

	virtual double Clarity(void) const;
	virtual void SetClarity(double d);

	virtual double  Reflectivity(void) const;
	virtual void SetReflectivity(double d);

	virtual ON_Color  TransparencyColor(void) const;
	virtual void SetTransparencyColor(const ON_Color& c);

	virtual ON_Color  ReflectivityColor(void) const;
	virtual void SetReflectivityColor(const ON_Color& c);

	virtual const CRhRdkTexture* Texture(CRhRdkMaterial::ChildSlotUsage usage) const;
	virtual void SetTexture(CRhRdkMaterial::ChildSlotUsage usage, const CRhRdkTexture* pTexture);

	virtual bool    Filtered(CRhRdkMaterial::ChildSlotUsage usage) const;
	virtual void SetFiltered(CRhRdkMaterial::ChildSlotUsage usage, bool b);

	virtual double  Amount(CRhRdkMaterial::ChildSlotUsage usage) const;
	virtual void SetAmount(CRhRdkMaterial::ChildSlotUsage usage, double d);

	virtual bool DisableLighting(void) const;
	virtual void SetDisableLighting(bool b);

	virtual bool IsAlphaTransparencyEnabled(void) const;
	virtual void EnableAlphaTransparency(bool b);

	virtual UUID Kind(void) const;

	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

	/** \return Shader for target renderer. */
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const = 0;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual bool IsBuiltIn(void) const { return false; }

private:
	class CImpl;
	CImpl* m_pImpl;
};
