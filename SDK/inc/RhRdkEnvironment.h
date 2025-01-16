
#pragma once // Public SDK header

#include "RhRdkMaterial.h"
#include "RhRdkContentKinds.h"
#include "Proto_ON_Environment.h"

/** \class CRhRdkEnvironment

	This is an abstract base class for all kinds of environments.
*/
class RHRDK_SDK CRhRdkEnvironment : public CRhRdkCoreContent
{
public:
	CRhRdkEnvironment();
	virtual ~CRhRdkEnvironment();

public: // Overrides from CRhRdkContent.
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;
	virtual bool DynamicIcon(const ON_2iSize& size, CRhinoDib& dibOut, DynamicIconUsage usage) const override;
	virtual Kinds Kind(void) const final override;
	virtual bool UpdateDocumentTables(void) const override;
	virtual const wchar_t* Category(void) const override { return RDK_CAT_GENERAL; }
	virtual IRhRdkPreviewSceneServer* NewPreviewSceneServer(const CRhRdkSSData& data) const override;
	virtual ePreviewSceneType PreviewSceneType(void) const override { return preview_scene_server; }
	virtual const wchar_t* LibraryFileExtension(void) const override;
	virtual Harvested HarvestData(const CRhRdkContent& oldContent) override;

public:
	enum class ChildSlotUsage : unsigned int
	{
		Background  = 0x01, // Specifies the background environment.
		Reflection  = 0x02, // Specifies the custom reflective environment. Also used for refraction.
		Skylighting = 0x04, // Specifies the custom skylighting environment.

		Any = Background | Reflection | Skylighting
	};

	/** This method is deprecated in favor of TextureChildSlotName(). */
	RDK_DEPRECATED ON_wString BackgroundTextureChildSlotName(void) const;

	/** This method is deprecated in favor of TextureChildSlotName(). */
	RDK_DEPRECATED ON_wString SkylightingTextureChildSlotName(void) const;

	/** \return the texture child slot name for a particular usage.
		\param usage specifies the usage for which the child slot name is required. */
	virtual ON_wString TextureChildSlotName(ChildSlotUsage usage) const;

	/** Used by the framework instead of TextureChildSlotName() when simply asking for the texture.
		The default implementation calls TextureChildSlotName(). */
	virtual bool GetTextureChannelInfo(ChildSlotUsage usage, TextureChannelInfo& info) const;

	/** Helper function- implemened using GetTextureChannelInfo. */
	const CRhRdkContent* GetTextureForUsage(ChildSlotUsage) const;

public:
	/** This method is deprecated in favor of the one below.
		bForDataOnly becomes true=CRhRdkTexture::TextureGeneration::Disallow, false=CRhRdkTexture::TextureGeneration::Allow */
	RDK_DEPRECATED virtual void SimulateEnvironment(CRhRdkSimulatedEnvironment& env, bool bForDataOnly) const;

	/** Modify a CRhRdkSimulatedEnvironment to simulate this environment as best you can. */
	virtual void SimulateEnvironment(CRhRdkSimulatedEnvironment& env, CRhRdkTexture::TextureGeneration tg=CRhRdkTexture::TextureGeneration::Allow, int iSimulatedTextureSize = -1) const = 0;

	/** \return \e true if this environment is in a document and is the
		 current environment for a particular usage in that document. */
	virtual bool IsInUse(IRhRdkCurrentEnvironment::Usage usage) const;

public:
	static bool ComputeBoxUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeSphereUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeEmapUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeLightprobeUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeCubemapUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeVerticalCrossCubemapUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeHorizontalCrossCubemapUV(const ON_3dVector& v, float &a, float &b);
	static bool ComputeHemisphericalUV(const ON_3dVector& v, float &a, float &b);

	// Prefer this one.
	static bool ComputeProjectionUV(CRhRdkSimulatedEnvironment::BackgroundProjections, const ON_3dVector& v, float& a, float& b);

protected:
	virtual bool SetSimulation(IRhRdk_XMLSection& section) const override;
	virtual CRhRdkPreviewGeometry* NewPreviewGeometry(void) const override;
	virtual CRhRdkPreviewBackground* NewPreviewBackground(void) const override;
	virtual UINT PreviewGeometryFlags(void) const override;
	virtual UINT PreviewBackgroundFlags(void) const override;
	virtual UINT PreviewLightingFlags(void) const override;

public:
	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

//Prohibit the use of std::unique_ptr for CRhRdkEnvironment - use CRhRdkContent::unique_ptr instead.
template <> class std::unique_ptr<CRhRdkEnvironment> {};

class RHRDK_SDK CRhRdkEnvironmentArray final : public ON_SimpleArray<const CRhRdkEnvironment*>
{
};

// Does a dynamic cast of 'c' to CRhRdkEnvironment*. Used to avoid green squiggly warnings.
RHRDK_SDK const CRhRdkEnvironment* RhRdkContentAsEnvironment(const CRhRdkContent* c);
