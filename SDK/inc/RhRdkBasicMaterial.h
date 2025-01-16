
#pragma once // Public SDK header

#include "RhRdkContentKinds.h"
#include "RhRdkMaterial.h"

class CRhRdkBasicMaterialCCI;

/*RDK_DEPRECATED*/ #define RDK_BASIC_MAT_BITMAP_TEXTURE         CS_MAT_BITMAP_TEXTURE
/*RDK_DEPRECATED*/ #define RDK_BASIC_MAT_BUMP_TEXTURE           CS_MAT_BUMP_TEXTURE
/*RDK_DEPRECATED*/ #define RDK_BASIC_MAT_TRANSPARENCY_TEXTURE   CS_MAT_TRANSPARENCY_TEXTURE
/*RDK_DEPRECATED*/ #define RDK_BASIC_MAT_ENVIRONMENT_TEXTURE    CS_MAT_ENVIRONMENT_TEXTURE

/** \class CRhRdkBasicMaterialBase

	This is the base class for any material that has properties based on ON_Material.

*/
class RHRDK_SDK CRhRdkBasicMaterialBase : public CRhRdkMaterial
{
public:
	// Initializes the material from an ON_Material. No events of any kind are sent.
	virtual void FromOnMaterial(const ON_Material& mat);

	/** Deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void FromOnMaterialEx(const ON_Material& mat, eChangeContext cc, bool bRenameEvents, UINT_PTR uReserved=0);

	// Initializes the material from an ON_Material.
	virtual void FromOnMaterialEx(const ON_Material& mat, bool bRenameEvents, bool bChangeName, UINT_PTR uReserved=0) = 0;

	virtual bool IsBasicType(void) const override;

	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual CRhRdkContent* MakeCopy(CopyMethods m) const override;

	static ON_Color DefaultColor();

protected:
	/** Deprecated - use the one with the UUID. */
	RDK_DEPRECATED virtual CRhRdkBasicMaterialCCI* NewCompoundContentImplementer(void) const;

	// Use this one instead.
	virtual CRhRdkBasicMaterialCCI* NewCompoundContentImplementer(const UUID& uuidRenderEngine) const = 0;
};

/** \class CRhRdkBasicMaterial

	This class is the RDK equivalent of ON_Material.

*/
class RHRDK_SDK CRhRdkBasicMaterial : public CRhRdkBasicMaterialBase
{
	class CImpl;
	typedef CRhRdkBasicMaterialBase _super;
public:
	CRhRdkBasicMaterial();
	virtual ~CRhRdkBasicMaterial();

	static int NumTextureTypes(void);

	virtual ON_Color Diffuse(void) const;
	virtual void SetDiffuse(const ON_Color& c);

	virtual ON_Color Gloss(void) const;
	virtual void SetGloss(const ON_Color& c);

	virtual double GlossFinish(void) const;
	virtual void SetGlossFinish(double d);

	virtual double Transparency(void) const;
	virtual void SetTransparency(double d);

	virtual ON_Color TransparencyColor(void) const;
	virtual void SetTransparencyColor(const ON_Color& c);

	virtual double Reflectivity(void) const;
	virtual void SetReflectivity(double d);

	virtual ON_Color ReflectivityColor(void) const;
	virtual void SetReflectivityColor(const ON_Color& c);

	virtual ON_Color Emission(void) const;
	virtual void SetEmission(const ON_Color& c);

	virtual ON_Color Ambient(void) const;
	virtual void SetAmbient(const ON_Color& c);

	virtual double IOR(void) const;
	virtual void SetIOR(double d);

	virtual bool IsFresnelEnabled(void) const;
	virtual void EnableFresnel(bool b);

	virtual double Polish(void) const;
	virtual void SetPolish(double d);

	virtual double Clarity(void) const;
	virtual void SetClarity(double d);

	virtual ON_wString FlamingoLibrary(void) const;
	virtual void SetFlamingoLibrary(const ON_wString& s);

	virtual bool DisableLighting(void) const;
	virtual void SetDisableLighting(bool b);

	virtual bool IsAlphaTransparencyEnabled(void) const;
	virtual void EnableAlphaTransparency(bool b);

	virtual void FromOnMaterialEx(const ON_Material& mat, bool bRenameEvents, bool bChangeName, UINT_PTR uReserved=0) override;

	class RHRDK_SDK CTextureSlot final : public CRhRdkObject
	{
	public:
		// The following members are exposed only to prevent breaking the Rhino 6 SDK.
		// If you are directly accessing these, your code will no longer work correctly.
		// You will need to change these accesses to function calls as specified below.
		// Accesses to these members will cause deprecated warnings.
		//
		RDK_DEPRECATED bool m_bOn;         // Use On() and SetOn() instead.
		RDK_DEPRECATED bool m_bFilterOn;   // Use FilterOn() and SetFilterOn() instead.
		RDK_DEPRECATED double m_dAmount;   // Use Amount() and SetAmount() instead.
		RDK_DEPRECATED ON_wString m_sName; // Use InternalName() instead. You are not allowed to set this member.

	private:
		class CRhRdkTexturedContentField* m_pField = nullptr; // Was UINT_PTR m_uReserved;

		CTextureSlot();
		friend class CRhRdkBasicMaterial::CImpl;

	public:
		CTextureSlot(const CTextureSlot& slot);

		ON_wString InternalName(void) const;

		ON_wString ChildSlotName(void) const;

		bool On(void) const;
		void SetOn(bool b);

		bool FilterOn(void) const;
		void SetFilterOn(bool b);

		double Amount(void) const; // 0..1
		void SetAmount(double d);  // 0..1

		const CTextureSlot& operator = (const CTextureSlot& slot);

		bool operator == (const CTextureSlot& slot);
		bool operator != (const CTextureSlot& slot);

		void WriteParametersToSection(IRhRdk_XMLSection& section, CRhRdkContent::WriteParamsContext context) const;
		void ReadParametersFromSection(const IRhRdk_XMLSection& section, CRhRdkContent::ReadParamsContext context);
		void SetSimulation(IRhRdk_XMLSection& section) const;

		// For RDK internal use only.
		void SetField(CRhRdkTexturedContentField* p);
	};

	virtual const CTextureSlot& TextureSlot(CRhRdkMaterial::ChildSlotUsage type) const;
	virtual void SetTextureSlot(CRhRdkMaterial::ChildSlotUsage type, const CTextureSlot& slot, bool bRecordUndo=false);

	/** Deprecated - use the one with TextureGeneration and pObject. */
	//RDK_DEPRECATED virtual void GetSimulation(const wchar_t* wszChildSlotName, CRhRdkSimulatedTexture& tex) const;

	virtual CRhRdkBasicMaterialCCI* NewCompoundContentImplementer(const UUID& uuidRenderEngine) const override;

public: // Overrides from CRhRdkContent.
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual const wchar_t* Category(void) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual void AddAutoParameters(IRhRdkParamBlock& paramBlock, int sectionId) const override;
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int sectionId) override;
	virtual ParamSerialMethod ParameterSerializationMethod(void) const override;
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;
	virtual bool GetSimulation(const IRhRdk_XMLSection& section) override;
	virtual bool IsBuiltIn(void) const override { return true; }
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;
	virtual CChildSlotIterator* NewChildSlotIterator(CSIContext context) const override;
	virtual ON_wString ChildSlotNameFromParamName(const wchar_t* wszParamName) const override;
	virtual ON_wString ParamNameFromChildSlotName(const wchar_t* wszChildSlotName) const override;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) override;
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const override;
	virtual unsigned int ComputeRenderCRC(const CRenderCRCFlags& rcrcFlags) const override;
	virtual bool SupportsContextHelp(void) const override;
	virtual void DisplayContextHelp(HWND hwndParent) const override;
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;

	virtual bool SetSimulation(IRhRdk_XMLSection& section) const override;

	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName,
	                                          CRhRdkVariant& vValueOut) const override;

	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName,
	                                          const CRhRdkVariant& vValue, SetContext sc) override;

	virtual ON_wString TextureChildSlotName(ChildSlotUsage usage) const override;

	virtual CRhRdkContent* MakeCopy(CopyMethods m) const override;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED bool OnChildSlotChanging(const wchar_t* wszChildSlotName);

	virtual bool OnChildSlotChanging(const wchar_t* wszChildSlotName) const override;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED bool OnChildSlotChanged(const wchar_t* wszChildSlotName);

	virtual bool OnChildSlotChanged(const wchar_t* wszChildSlotName) const override;

	virtual ON_wString GetLocalizedParamName(const wchar_t* wszParamName) const override;
	virtual ON_wString GetEnglishParamName(const wchar_t* wszParamName) const override;

	virtual Harvested HarvestData(const CRhRdkContent& oldContent) override;
	virtual ON_wString AutomaticInstanceName(void) const override;

protected:
	virtual void SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg=CRhRdkTexture::TextureGeneration::Allow, int iSimulatedTextureSize = -1, const CRhinoObject* pObject=nullptr) const override;

private:
	/** \internal For RDK internal use only. */
	void SetupCCIChild(CRhRdkMaterial::ChildSlotUsage, CRhRdkBasicMaterialCCI&) const;

private:
	/** \internal For RDK internal use only. */
	CImpl* m_pImpl;
};
