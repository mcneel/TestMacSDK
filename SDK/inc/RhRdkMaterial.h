
#pragma once // Public SDK header

#include "RhRdkFieldStrings.h"

#define RDK_MAT_TRANSPARENT  ON_MATERIAL_SIM_TRANSPARENT_COLOR

/** \class CRhRdkMaterial

	This is an abstract base class for all kinds of materials.

*/
class RHRDK_SDK CRhRdkMaterial : public CRhRdkCoreContent
{
public:
	CRhRdkMaterial();
	virtual ~CRhRdkMaterial();

public:
	/** Assign the material to an object via an object reference. This can assign to
		whole objects or to sub-faces of objects, depending on what the objRef refers to.
		\param objRef is a reference to an object or sub-face to assign the material to.
		\note This is the simple version which might ask the user questions using message boxes.
		 For more control over this, see the other version.
		\return \e true if successful, else \e false. */
	bool AssignTo(const CRhinoObjRef& objRef) const;

	/** Assign the material to a collection of objects via object references. This can assign to
		whole objects or to sub-faces of objects, depending on what the objRef refers to.
		\param aObjRef is an array of references to objects or sub-faces to assign the material to.
		\param sfc specifies how to handle existing sub-face assignments.
		\param bc specifies how to handle blocks (if present in the aObjRef array).
		\param bInteractive specifies how to ask the user questions.
		 If \e true, a message box is used, otherwise the command line is used.
		\return \e true if successful, else \e false. */
	bool AssignTo(const ON_ClassArray<CRhinoObjRef>& aObjRef, RhRdkAssignToSubFaceChoices sfc, RhRdkAssignToBlockChoices bc, bool bInteractive) const;

public: // Overrides from CRhRdkContent.
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;
	virtual bool DynamicIcon(const ON_2iSize& size, CRhinoDib& dibOut, DynamicIconUsage usage) const override;
	virtual Kinds Kind(void) const final override;
	virtual bool UpdateDocumentTables(void) const override;
	virtual IRhRdkPreviewSceneServer* NewPreviewSceneServer(const CRhRdkSSData& data) const override;
	virtual ePreviewSceneType PreviewSceneType(void) const override { return preview_scene_server; }
	virtual bool SetSimulation(IRhRdk_XMLSection& section) const override;
	virtual const wchar_t* Category(void) const override { return RDK_CAT_GENERAL; }
	virtual UINT PreviewGeometryFlags(void) const override;
	virtual UINT PreviewBackgroundFlags(void) const override;
	virtual UINT PreviewLightingFlags(void) const override;
	virtual const wchar_t* LibraryFileExtension(void) const override;
	virtual Harvested HarvestData(const CRhRdkContent& oldContent) override;
	virtual void SetInstanceName(const wchar_t* wszName) override;
	virtual CRhRdkContent* MakeCopy(CopyMethods m = CopyMethods::Normal) const override;
	virtual CRhRdkContent* FastMakeCopy(CopyMethods m) const override;

public:
	enum class ChildSlotUsage : unsigned int
	{
		None                             = 0x0000000,
		Diffuse                          = 0x0000001,
		Transparency                     = 0x0000002,
		Opacity                          = 0x0000002, // Deprecated.
		Bump                             = 0x0000004,
		Environment                      = 0x0000008,
		PBR_base_color                   = Diffuse,
		PBR_opacity                      = Transparency,
		PBR_bump                         = Bump,
		PBR_subsurface                   = 0x0000020,
		PBR_subsurface_scattering        = 0x0000040, // Deprecated.
		PBR_subsurface_scattering_color  = 0x0000040,
		PBR_subsurface_scattering_radius = 0x0000080,
		PBR_metallic                     = 0x0000100,
		PBR_specular                     = 0x0000200,
		PBR_specular_tint                = 0x0000400,
		PBR_roughness                    = 0x0000800,
		PBR_anisotropic                  = 0x0001000,
		PBR_anisotropic_rotation         = 0x0002000,
		PBR_sheen                        = 0x0004000,
		PBR_sheen_tint                   = 0x0008000,
		PBR_clearcoat                    = 0x0010000,
		PBR_clearcoat_roughness          = 0x0020000,
		PBR_opacity_ior                  = 0x0040000,
		PBR_opacity_roughness            = 0x0080000,
		PBR_emission                     = 0x0100000,
		PBR_ambient_occlusion            = 0x0200000,
		//PBR_smudge                     = 0x0400000,
		PBR_displacement                 = 0x0800000,
		PBR_clearcoat_bump               = 0x1000000,
		PBR_alpha                        = 0x2000000,

		Any = 0xFFFFFFFF
	};

	static ON_Texture::TYPE TextureTypeFromUsage(ChildSlotUsage u);
	static ChildSlotUsage UsageFromTextureType(ON_Texture::TYPE type);

	class RHRDK_SDK PhysicallyBased
	{
	public:
		class RHRDK_SDK ParameterNames
		{
		public:
			static ON_wString BaseColor(void);
			static ON_wString BRDF(void);
			static ON_wString Subsurface(void);
			static ON_wString SubsurfaceScatteringColor(void);
			static ON_wString SubsurfaceScatteringRadius(void);
			static ON_wString Specular(void);
			static ON_wString SpecularTint(void);
			static ON_wString Metallic(void);
			static ON_wString Roughness(void);
			static ON_wString Anisotropic(void);
			static ON_wString AnisotropicRotation(void);
			static ON_wString Sheen(void);
			static ON_wString SheenTint(void);
			static ON_wString Clearcoat(void);
			static ON_wString ClearcoatRoughness(void);
			static ON_wString OpacityIor(void);
			static ON_wString Opacity(void);
			static ON_wString OpacityRoughness(void);
			static ON_wString Emission(void);
			static ON_wString Alpha(void);
		};

		class RHRDK_SDK ChildSlotNames
		{
		public:
			static ON_wString BaseColor(void);
			static ON_wString Subsurface(void);
			static ON_wString SubsurfaceScatteringColor(void);
			static ON_wString SubsurfaceScatteringRadius(void);
			static ON_wString Specular(void);
			static ON_wString SpecularTint(void);
			static ON_wString Metallic(void);
			static ON_wString Roughness(void);
			static ON_wString Anisotropic(void);
			static ON_wString AnisotropicRotation(void);
			static ON_wString Sheen(void);
			static ON_wString SheenTint(void);
			static ON_wString Clearcoat(void);
			static ON_wString ClearcoatRoughness(void);
			static ON_wString OpacityIor(void);
			static ON_wString Opacity(void);
			static ON_wString OpacityRoughness(void);
			static ON_wString Emission(void);
			static ON_wString AmbientOcclusion(void);
			static ON_wString Displacement(void);
			static ON_wString Bump(void);
			static ON_wString ClearcoatBump(void);
			static ON_wString Alpha(void);
			static ON_wString FromUsage(ChildSlotUsage usage);
			static ChildSlotUsage ToUsage(const wchar_t* wszChildSlotName);
		};
	};

	/** Used by the framework instead of TextureChildSlotName() when simply asking for the texture.
		Transparency color should vary from white (fully transparent) to black (opaque) if supported.
		Bump and environment default colors are black. Default implementation calls TextureChildSlotName(). */
	virtual bool GetTextureChannelInfo(ChildSlotUsage usage, TextureChannelInfo& info) const;

	/** Do not use to determine the texture for a usage - use GetChannelTextureInfo instead.
		Only use this function for specifically determining the child slot name for use in SetChild, SetChildSlotOn etc. */
	ON_wString GetTextureChildSlotName(ChildSlotUsage usage) const;

	/** Helper function - implemented using GetTextureChannelInfo. */
	const CRhRdkContent* GetTextureForUsage(ChildSlotUsage usage) const;

	/** Helper function. */
	ChildSlotUsage TextureUsage(const wchar_t* wszChildSlotName) const;

protected:
	//** Do not call - this method is for specialization only */
	virtual ON_wString TextureChildSlotName(ChildSlotUsage usage) const;

	/** This method is deprecated in favor of TextureChildSlotName(). */
	RDK_DEPRECATED virtual ON_wString DiffuseTextureChildSlotName(void) const;

	/** This method is deprecated in favor of TextureChildSlotName(). */
	RDK_DEPRECATED virtual ON_wString TransparencyTextureChildSlotName(void) const;

	/** This method is deprecated in favor of TextureChildSlotName(). */
	RDK_DEPRECATED virtual ON_wString BumpTextureChildSlotName(void) const;

	/** This method is deprecated in favor of TextureChildSlotName(). */
	RDK_DEPRECATED virtual ON_wString EnvironmentTextureChildSlotName(void) const;

public:
	/** The default implementation returns \e false if the wszChildSlotName is the same as is returned by BumpTextureChildSlotName, otherwise \e true.  
		See CRhRdkTexture::IsLinear for more information. */
	virtual bool TextureRequiresLinearWorkflowAdjustment(const wchar_t* wszChildSlotName) const;

	/** \return \e true if the material is associated with 'mat'.
		The default behavior is to check if the material's instance id is assigned to the ON_Material. */
	virtual bool IsAssociatedWith(const ON_Material& mat) const;

	virtual CRhRdkPreviewGeometry* NewPreviewGeometry(void) const override;
	virtual CRhRdkPreviewBackground* NewPreviewBackground(void) const override;

	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

	/** Get the material as an ON_Material. Calls SimulateMaterial() if something relevant
		has changed since the last call. Deprecated in favor of ToOnMaterial() - exactly the same functionity. */
	ON_DEPRECATED_MSG("Prefer ToOnMaterial for clarity") virtual const ON_Material& SimulatedMaterial(CRhRdkTexture::TextureGeneration tg=CRhRdkTexture::TextureGeneration::Allow, int iSimulatedTextureSize = -1, const CRhinoObject* pObject=nullptr) const;

	virtual_su const ON_Material& ToOnMaterial(CRhRdkTexture::TextureGeneration tg = CRhRdkTexture::TextureGeneration::Allow, int iSimulatedTextureSize = -1, const CRhinoObject* pObject = nullptr) const;

	static CRhRdkMaterial* FromOnMaterial(const ON_Material& material, const CRhinoDoc* pDocAssoc);

	static std::shared_ptr<CRhRdkMaterial> Default(void);

protected:
	/** This method is deprecated in favor of the one below.
		bForDataOnly becomes true=CRhRdkTexture::TextureGeneration::Disallow, false=CRhRdkTexture::TextureGeneration::Allow */
	RDK_DEPRECATED virtual void SimulateMaterial(ON_Material& matOut, bool bForDataOnly, const CRhinoObject* pObject=nullptr) const;

	/** Implement this method to modify an ON_Material to simulate this material as best you can.
		Callers should call ToOnMaterial() instead which is optimized to only do the
		simulation if something has changed. 
		IMPORTANT NOTE:	Since V6, this function is both pure and implemented, so you should call it from your implementation.
		The base class version (this one) implements support for setting up the standard texture slots. */
	virtual void SimulateMaterial(ON_Material& matOut, CRhRdkTexture::TextureGeneration tg=CRhRdkTexture::TextureGeneration::Allow, int iSimulatedTextureSize = -1, const CRhinoObject* pObject=nullptr) const = 0;

public:
	// Helper functions to help figure out if a basic material is functionally equivalent to one of the built in basic types.
	bool SmellsLike(const UUID& typeId, bool bIgnoreTextures = false) const;
	bool SmellsLikePlaster(void) const;
	bool SmellsLikePaint(void) const;
	bool SmellsLikeMetal(void) const;
	bool SmellsLikePlastic(void) const;
	bool SmellsLikeGem(void) const;
	bool SmellsLikeGlass(void) const;

	bool SmellsLikeTexturedPlaster(void) const;
	bool SmellsLikeTexturedPaint(void) const;
	bool SmellsLikeTexturedMetal(void) const;
	bool SmellsLikeTexturedPlastic(void) const;
	bool SmellsLikeTexturedGem(void) const;
	bool SmellsLikeTexturedGlass(void) const;

public:
	// Return a white plaster material which is always available as a backstop.
	static const CRhRdkMaterial* WorstPossibleMaterial(void);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;

public:
	/** \internal For RDK internal use only. */
	CImpl& Impl(void) const;
};

//Prohibit the use of std::unique_ptr for CRhRdkMaterial - use CRhRdkContent::unique_ptr instead.
template <> class std::unique_ptr<CRhRdkMaterial> { };

class RHRDK_SDK CRhRdkMaterialArray final : public ON_SimpleArray<const CRhRdkMaterial*>
{
};

// Does a dynamic cast of 'c' to CRhRdkMaterial*. Used to avoid green squiggly warnings.
RHRDK_SDK const CRhRdkMaterial* RhRdkContentAsMaterial(const CRhRdkContent* c);
