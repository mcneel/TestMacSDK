
#pragma once

#include "RhRdkDefinitions.h"

class RHRDK_SDK CRhRdkOneColorEvaluator : public CRhRdkTextureEvaluator
{
public:
	CRhRdkOneColorEvaluator(const class CRhRdkOneColorTextureBase& texture, CEvalFlags ef, bool bCreateTexEvals = true);
	virtual ~CRhRdkOneColorEvaluator();

	virtual const CRhRdkColor& Color1(void) const;
	virtual bool TextureOn1(void) const;
	virtual double TextureAmount1(void) const;

	virtual const ON_Xform& LocalMappingTransform(void) const;

public:
	virtual void DeleteThis(void) { delete this; }

	virtual CRhRdkColor OutputColor(int colorNumber, const ON_3dPoint& uvw, const ON_3dVector& duvwdx, const ON_3dVector& duvwdy, void*) const;

	// Implemented to do point sampling.
	virtual bool GetColor(const ON_3dPoint& uvw, const ON_3dVector& duvwdx,
	                      const ON_3dVector& duvwdy, CRhRdkColor& colOut, void* pvData=nullptr) const;

	virtual bool GetColor2(const ON_3dPoint& uvw, const ON_3dVector& duvwdx,
	                       const ON_3dVector& duvwdy, CRhRdkColor& colOut, void* pvData=nullptr) const;

	virtual bool RegisterChildCallback(IChildCallback* pChildCallback, const CRhRdkTexture& texture, void* pvData);

	// Override this to actually evaluate the texture at UVW.
	virtual CRhRdkColor GetColorSample(const ON_3dPoint& uvw, const ON_3dVector& duvwdx, const ON_3dVector& duvwdy, void*) const = 0;

	virtual void* EVF(const wchar_t*, void*);

protected:
	/** \internal For RDK internal use only. */
	class CRhRdkOneColorEvaluatorImpl* m_pImpl1;
};

class RHRDK_SDK CRhRdkTwoColorEvaluator : public CRhRdkOneColorEvaluator
{
public:
	CRhRdkTwoColorEvaluator(const class CRhRdkTwoColorTextureBase& texture, CEvalFlags ef, bool bCreateTexEvals=true);
	virtual ~CRhRdkTwoColorEvaluator();

	virtual const CRhRdkColor& Color2(void) const;
	virtual bool TextureOn2(void) const;
	virtual double TextureAmount2(void) const;

public:
	virtual CRhRdkColor OutputColor(int colorNumber, const ON_3dPoint& uvw, const ON_3dVector& duvwdx, const ON_3dVector& duvwdy, void*) const;

	virtual bool RegisterChildCallback(IChildCallback* pChildCallback, const CRhRdkTexture& texture, void* pvData);

private:
	/** \internal For RDK internal use only. */
	class CRhRdkTwoColorEvaluatorImpl* m_pImpl2;
};

class RHRDK_SDK CRhRdkOneColorTextureBaseCSI : public CRhRdkContent::CChildSlotIterator
{
public:
	CRhRdkOneColorTextureBaseCSI(const CRhRdkContent* pContent, CRhRdkContent::CSIContext context);
	virtual ~CRhRdkOneColorTextureBaseCSI();

	/** If you override this you must call the base class first and return \e true if the base class returns \e true.
		Otherwise call Index() and return your child slots based on the returned index value; remember to call IncIndex()
		before you return \e true. When you have no more child slots to return, return \e false. */
	virtual bool NextChildSlot(ON_wString& sParamNameOut, ON_wString& sChildSlotNameOut, ON_wString& sDisplayNameOut);

protected:
	virtual int Index(void) const;

	/** Increments the iterator index. */
	virtual void IncIndex(void);

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkTwoColorTextureBaseCSI : public CRhRdkOneColorTextureBaseCSI
{
public:
	CRhRdkTwoColorTextureBaseCSI(const CRhRdkContent* pContent, CRhRdkContent::CSIContext context);
	virtual ~CRhRdkTwoColorTextureBaseCSI();

	/** If you override this you must call the base class first and return \e true if the base class returns \e true.
		Otherwise call Index() and return your child slots based on the returned index value; remember to call IncIndex()
		before you return \e true. When you have no more child slots to return, return \e false. */
	virtual bool NextChildSlot(ON_wString& sParamNameOut, ON_wString& sChildSlotNameOut, ON_wString& sDisplayNameOut);
};

class RHRDK_SDK CRhRdkOneColorTextureBase : public CRhRdkTexture
{
	typedef CRhRdkTexture _super;
public:
	CRhRdkOneColorTextureBase();
	CRhRdkOneColorTextureBase(bool bAutoGlobalRegister);
	virtual ~CRhRdkOneColorTextureBase();

	virtual CRhRdkColor Color1(void) const;
	virtual void SetColor1(const CRhRdkColor& col);

	virtual bool TextureOn1(void) const;
	virtual void SetTextureOn1(bool bOn);

	virtual double TextureAmount1(void) const;
	virtual void SetTextureAmount1(double d);

	virtual bool SuperSample(void) const;
	virtual void SetSuperSample(bool bSwap);

	virtual bool Tile(void) const;
	virtual void SetTile(bool bTile);

protected: // Overrides from CRhRdkContent.
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual bool IsImageBased(void) const override { return false; }
	virtual unsigned int ComputeRenderCRC(const CRenderCRCFlags& rcrcFlags) const override;
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;
	virtual void AddAutoParameters(IRhRdkParamBlock& paramBlock, int sectionId) const override;
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int sectionId) override;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& value) override;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual bool IsBuiltIn(void) const override { return true; }
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;
	virtual CChildSlotIterator* NewChildSlotIterator(CSIContext context) const override;
	virtual CRhRdkContent* MakeCopy(CopyMethods m=CopyMethods::Normal) const override;
	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName, CRhRdkVariant& vValueOut) const override;
	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName, const CRhRdkVariant& vValue, SetContext sc) override;

private:
	class CImpl;
	CImpl* m_pImpl1;
};

class RHRDK_SDK CRhRdkTwoColorTextureBase : public CRhRdkOneColorTextureBase
{
	typedef CRhRdkOneColorTextureBase _super;
public:
	CRhRdkTwoColorTextureBase();
	CRhRdkTwoColorTextureBase(bool bAutoGlobalRegister);
	virtual ~CRhRdkTwoColorTextureBase();

	virtual CRhRdkColor Color2(void) const;
	virtual void SetColor2(const CRhRdkColor& col);

	virtual bool TextureOn2(void) const;
	virtual void SetTextureOn2(bool bOn);

	virtual double TextureAmount2(void) const;
	virtual void SetTextureAmount2(double d);

	virtual bool SwapColors(void) const;
	virtual void SetSwapColors(bool bSwap);

protected:
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& value) override;
	virtual CChildSlotIterator* NewChildSlotIterator(CSIContext context) const override;
	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName, CRhRdkVariant& vValueOut) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraReqName, const CRhRdkVariant& vValue, SetContext sc) override;

private:
	class CImpl;
	CImpl* m_pImpl2;
};
