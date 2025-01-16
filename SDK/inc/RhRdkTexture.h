
#pragma once // Public SDK header

#include "RhRdkCoreContent.h"

class CRhRdkColor;
class CRhRdkTextureGraphInfo;
class CRhRdkSimulatedTexture;
class IRhRdkTextureEvaluator;

#pragma warning(push)
#pragma warning(disable : 4996) // xxxx was declared deprecated.

/** \class CRhRdkTexture

	This is an abstract base class for all kinds of textures.
*/
class RHRDK_SDK CRhRdkTexture : public CRhRdkCoreContent
{
public:
	CRhRdkTexture();
	CRhRdkTexture(bool bAutoGlobalRegister);
	virtual ~CRhRdkTexture();

	//Replaced by CRhRdkMaterial::ChildSlotUsage
	/*enum eType
	{
		bitmap       = 0,
		bump         = 1,
		transparency = 2,
		environment  = 3,
		force32bit_type = 0xFFFFFFFF 
	};*/

	/** Save the texture as an image file.
		\param wszFilename is the file name to save to.
		\param width is the pixel width to save the image at.
		\param height is the pixel height to save the image at.
		\param depth is the bit-depth to save the image at.
		\return \e true if successful, else \e false. */
	bool SaveAsImage(const wchar_t* wszFilename, int width, int height, int depth) const;

public: // Overrides from CRhRdkContent.
	virtual bool Initialize(void) override;
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;
	virtual bool DynamicIcon(const ON_2iSize& size, CRhinoDib& dibOut, DynamicIconUsage usage) const override;
	virtual Kinds Kind(void) const final override;
	virtual const wchar_t* LibraryFileExtension(void) const override;
	virtual bool UpdateDocumentTables(void) const override;
	virtual IRhRdkPreviewSceneServer* NewPreviewSceneServer(const CRhRdkSSData& data) const override;
	virtual bool SetSimulation(IRhRdk_XMLSection& section) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual int	PreviewUpdateDelay(void) const override;
	virtual bool CreatePreview(CRhRdkRenderPlugIn& plugIn, const ON_2iSize& sizeImage, RhRdkPreviewQuality qual, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) const override;
	virtual bool CreateLibraryPreview(const ON_2iSize& sizeImage, const IRhRdkPreviewSceneServer* pSceneServer, CRhinoDib& dibOut) const override;
	virtual ParamSerialMethod ParameterSerializationMethod(void) const override;

	/** <b>Please be sure to call the base class</b> if you override this function. */
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;

	/** <b>Please be sure to call the base class</b> if you override this function. */
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;

	virtual ePreviewSceneType PreviewSceneType(void) const override;
	virtual Harvested HarvestData(const CRhRdkContent& oldContent) override;

	RDK_DEPRECATED virtual void AddLocalMappingSection(void); // OBSOLETE.
	RDK_DEPRECATED virtual void AddGraphSection(void);        // OBSOLETE.

public: // Overrides from CRhRdkCoreContent.

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void AddUISections(void) override;

	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void AddTwoColorSection(void);

	/** If your texture needs a two-color UI section, it should usually be
		derived from CRhRdkTwoColorTextureBase. Then a two-color section
		will automatically be provided by the RDK. However, if you are willing
		to provide your own fields to support the two-color UI, you can derive
		your texture directly from CRhRdkTexture and call this method from your
		override of AddUISections(). */
	virtual void AddTwoColorSection(IRhRdkExpandableContentUI& ui);

public: // Local mapping support.

	/** This method is deprecated - obsolete. */
	RDK_DEPRECATED virtual bool RecreateLocalMapping(void);

	/** Get the transformation that can be applied to the UVW vector to convert it from 
		normalized texture space into locally mapped space (ie - with repeat, offset and rotation applied.) */
	virtual ON_Xform LocalMappingTransform(void) const;

	/** Get repeat value across UVW space. If the projection type is WCS or other type specified in model
		units, then this is the repeat across 1 meter of the model. */
	virtual ON_3dVector	Repeat(void) const;

	/** Set repeat value across UVW space. If the projection type is WCS or other type specified in model
		units, then this is the repeat across 1 meter of the model. */
	RDK_DEPRECATED virtual void SetRepeat(const ON_3dVector& vecRepeat, eChangeContext cc);
	virtual void SetRepeat(const ON_3dVector& vecRepeat);

	/** Get offset value across UVW space. If the projection type is WCS or other type specified in model
		units, then this is the offset in meters. */
	virtual ON_3dVector	Offset(void) const;

	/** Set offset value across UVW space. If the projection type is WCS or other type specified in model
		units, then this is the offset in meters. */
	RDK_DEPRECATED virtual void SetOffset(const ON_3dVector& vecOffset, eChangeContext cc);
	virtual void SetOffset(const ON_3dVector& vecOffset);

	virtual bool RepeatLocked(void) const;
	RDK_DEPRECATED virtual void SetRepeatLocked(bool bLocked, eChangeContext cc);
	virtual void SetRepeatLocked(bool bLocked);

	virtual bool OffsetLocked(void) const;
	RDK_DEPRECATED virtual void SetOffsetLocked(bool bLocked, eChangeContext cc);
	virtual void SetOffsetLocked(bool bLocked);

	virtual ON_3dVector	Rotation(void) const;
	RDK_DEPRECATED virtual void SetRotation(const ON_3dVector& vecRotation, eChangeContext cc);
	virtual void SetRotation(const ON_3dVector& vecRotation);

	virtual int MappingChannel(void) const;
	RDK_DEPRECATED virtual void SetMappingChannel(int iMappingChannel, eChangeContext cc);
	virtual void SetMappingChannel(int iMappingChannel);

	RDK_DEPRECATED enum eWrapType { clamped=0, repeating=1, force32bit_wt = 0xFFFFFFFF };
	RDK_DEPRECATED virtual void SetWrapType(eWrapType wt, eChangeContext cc);

	enum class WrapTypes : unsigned int
	{
		Clamped,
		Repeating,
	};
	virtual WrapTypes WrapType(void) const;
	virtual void SetWrapType(WrapTypes wt);

	RDK_DEPRECATED enum eProjectionMode { pm_unset=-1, mapping_channel=0, wcs= 2, emap=3, wcs_box= 4, screen=5, force32=0xFFFFFFFF };
	RDK_DEPRECATED virtual void SetProjectionMode(eProjectionMode pm, eChangeContext cc);

	enum class ProjectionModes : int
	{
		Unset          =-1,
		MappingChannel = 0,
		//View         = 1,	// View is no longer supported - use emap with "emm_spherical" instead.
		Wcs            = 2,
		Emap           = 3,	// Now means "environment mapped" - call "EnvironmentMappingMode" to get specific projection for this texture.
		WcsBox         = 4,
		Screen         = 5,
	};
	virtual ProjectionModes ProjectionMode(void) const;
	virtual void SetProjectionMode(ProjectionModes pm);

	enum RDK_DEPRECATED eEnvironmentMappingMode { emm_automatic=0, emm_spherical=1, emm_emap=2, emm_box=3, emm_lightprobe=5, emm_cubemap=6, emm_vertical_cross_cubemap=7, emm_horizontal_cross_cubemap=8, emm_hemispherical=9, emm_force_32bit_bkgd=0xFFFFFFFF };

	enum class EnvironmentMappingModes : unsigned int
	{
		Automatic  = 0,
		Spherical  = 1, // Equirectangularprojection.
		Emap       = 2, // Mirror ball.
		Box        = 3,
		Lightprobe = 5,
		Cubemap    = 6,
		VerticalCrossCubemap = 7,
		HorizontalCrossCubemap = 8,
		Hemispherical = 9,
	};
	virtual EnvironmentMappingModes InternalEnvironmentMappingMode(void) const;
	RDK_DEPRECATED virtual void SetEnvironmentMappingMode(eEnvironmentMappingMode e, eChangeContext cc);
	virtual void SetEnvironmentMappingMode(EnvironmentMappingModes emm);
	virtual EnvironmentMappingModes EnvironmentMappingMode(void) const;
	virtual EnvironmentMappingModes AutomaticEnvironmentMappingMode(void) const;

	virtual bool PreviewIn3D(void) const;
	RDK_DEPRECATED virtual void SetPreviewIn3D(bool b, eChangeContext cc);
	virtual void SetPreviewIn3D(bool b);

	virtual bool PreviewLocalMapping(void) const;
	RDK_DEPRECATED virtual void SetPreviewLocalMapping(bool b, eChangeContext cc);
	virtual void SetPreviewLocalMapping(bool b);

	virtual bool DisplayInViewport(void) const;
	RDK_DEPRECATED virtual void SetDisplayInViewport(bool b, eChangeContext cc);
	virtual void SetDisplayInViewport(bool b);

	static ON_3dPoint WcsBoxMapping(const ON_3dPoint& world_xyz, const ON_3dVector& normal);

	static bool EnvironmentMappingProjection(EnvironmentMappingModes emm, const ON_3dVector& reflection_vector, float& u, float& v);

public: // Adjustment support - added 9th July 2008 ALB
	class IColorAdjuster
	{
	public:
		virtual void DeleteThis(void) = 0;
		virtual bool AdjustColor(CRhRdkColor& color) const = 0;
		virtual bool On(void) const = 0;

		virtual bool operator !=(const IColorAdjuster* ica) const { return !operator==(ica); }
		virtual bool operator ==(const IColorAdjuster*) const = 0;

		RDK_DEPRECATED static DWORD CRC(IColorAdjuster* pca);

		static DWORD CRC(const IColorAdjuster& pca);

	protected:
		virtual ~IColorAdjuster() {}
	};

	/** \return a color adjuster object that can be used to modify evaluated colors. */
	virtual IColorAdjuster* NewColorAdjuster(void) const;

	virtual bool AdjustmentGrayscale(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentGrayscale(bool, eChangeContext cc);
	virtual void SetAdjustmentGrayscale(bool);

	virtual bool AdjustmentInvert(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentInvert(bool, eChangeContext cc);
	virtual void SetAdjustmentInvert(bool);

	virtual bool AdjustmentClamp(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentClamp(bool, eChangeContext cc);
	virtual void SetAdjustmentClamp(bool);

	virtual bool AdjustmentScaleToClamp(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentScaleToClamp(bool, eChangeContext cc);
	virtual void SetAdjustmentScaleToClamp(bool);

	virtual double AdjustmentMultiplier(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentMultiplier(double, eChangeContext cc);
	virtual void SetAdjustmentMultiplier(double);

	virtual double AdjustmentClampMin(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentClampMin(double, eChangeContext cc);
	virtual void SetAdjustmentClampMin(double);

	virtual double AdjustmentClampMax(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentClampMax(double, eChangeContext cc);
	virtual void SetAdjustmentClampMax(double);

	virtual double AdjustmentGain(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentGain(double, eChangeContext cc);
	virtual void SetAdjustmentGain(double);

	virtual double AdjustmentGamma(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentGamma(double, eChangeContext cc);
	virtual void SetAdjustmentGamma(double);

	virtual double AdjustmentSaturation(void) const;
	RDK_DEPRECATED virtual void SetAdjustmentSaturation(double, eChangeContext cc);
	virtual void SetAdjustmentSaturation(double);

	/** \return Adjustment hue shift in radians. */
	virtual double AdjustmentHueShift(void) const;

	/** Set adjustment hue shift.
		\param dShift is the hue shift in radians.
		\param cc is the context of the change. */
	RDK_DEPRECATED virtual void SetAdjustmentHueShift(double dShift, eChangeContext cc);
	virtual void SetAdjustmentHueShift(double dShift);

public:
	/** \enum TextureGeneration is used by CRhRdkMaterial::SimulateMaterial(), CRhRdkTexture::SimulateTexture() and CRhRdkEnvironment::SimulateEnvironment() */
	enum class TextureGeneration : int
	{
		Allow    = 1, // Texture generation is allowed during simulation.
		Disallow = 2, // Texture generation is not allowed; simulation should be fast.
		Skip     = 3, // Texture information should not be supplied at all.
	};

	/** This method is deprecated in favor of the one below.
		bForDataOnly becomes true=TextureGeneration::Disallow, false=TextureGeneration::Allow */
	RDK_DEPRECATED virtual void SimulateTexture(CRhRdkSimulatedTexture& texOut, bool bForDataOnly) const;

	/** Modify an CRhRdkSimulatedTexture to simulate this texture as best you can.
		The default implementation attempts to create a temporary bitmap from the texture evaluator.
		\param texOut accepts the simulation.
		\param tg determines the extent of texture generation performed by the default implementation. \see TextureGeneration
		\param pObject is an optional object pointer (usually CRhinoObject or CRhinoLayer).
		\note If the default implementation is overridden, no guarantee can be made that the override will respect
		 the TextureGeneration requirements, but clients should attempt to do so. */
	virtual void SimulateTexture(CRhRdkSimulatedTexture& texOut, TextureGeneration tg=TextureGeneration::Allow, int iSimulatedTextureSize = -1, const CRhinoObject* pObject=NULL) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual IRhRdkTextureEvaluator* NewTextureEvaluator(void) const;

	/** This enum is deprecated in favor of IRhRdkTextureEvaluator::CEvalFlags */
	enum RDK_DEPRECATED { efNormal = 0, efDisableFiltering = 1, efDisableLocalMapping = 2, efDisableAdjustment = 4, efDisableProjectionChange = 8 };

	/** Create a texture evaluator. This is an independent lightweight object capable
		of evaluating texture color throughout uvw space. May be called from within
		a rendering shade pipeline. The caller shall delete the texture evaluator by calling DeleteThis().
		The default implementation returns null.
		\param ef can optionally be used to pass flags that control creation of the evaluator. */
	virtual IRhRdkTextureEvaluator* NewTextureEvaluator(IRhRdkTextureEvaluator::CEvalFlags ef) const;

	/** Create a texture evaluator that mimics the simulated version of this texture.
		In most cases, this will simply be a pixelated version of the texture, but in the case of high
		dynamic range textures, the resultant evaluator will be tone-mapped. The function is implemented
		to cache the tone-mapped results to a bitmap, so the actual evaluator you will be returned will
		be a bitmap texture evaluator.
		\param iBaseSize is the simulation size - generally square, but for projection specific images this may
		 be modified to a larger size by the framework. Input -1 means "use the pixel-height parameter
		 if available, otherwise use the simulation size". */
	virtual IRhRdkTextureEvaluator* NewSimulatedTextureEvaluator(int iBaseSize = -1) const;

	/** Optionally override this method to generate a texture simulation in a way that
		is different to the default implementation.
		\param dibOut is a DIB that receives the simulated texture pixels.
		\param ef can optionally be used to pass flags that control creation of a texture evaluator.
		\return \e true if successful, else \e false. */
	virtual bool GenerateTextureSimulation(CRhinoDib& dibOut, IRhRdkTextureEvaluator::CEvalFlags ef) const;

	enum class eBitFormat : int
	{
		k32_bpc_RGB,	// 3 float values per pixel.
		k32_bpc_BGR,
		k32_bpc_RGBA,	// 4 float values per pixel.
		k32_bpc_BGRA,
		k8_bpc_RGB,		// 3 BYTE values per pixel.
		k8_bpc_BGR,	
		k8_bpc_RGBA,	// 4 BYTE values per pixel.
		k8_bpc_BGRA
	};

	/** Optionally override this method to generate a texture buffer.  This should be used to provide pixel
		data for bitmap types very quickly.  Do not override if your texture does not have a discrete representation or
		if the values differ outside of the 0.0-1.0 UV range.  This function is not implemented by default.
		\param sizeOut is the size of the image in the buffer.
		\param bfOut is the bit format in the buffer as given above.
		\param vOut should be of type vt_Buffer, and will contain the raw data.  There will be sizeOut.cx * sizeOut.cy pixels in the form x0y0,x1y0...xMaxXyMaxy
		\return \e true if successful, else \e false. */
	virtual bool GenerateRawBuffer(ON_2iSize& sizeOut, eBitFormat& bfOut, CRhRdkVariant& vOut) const;

	/** Call this method to determine whether the texture is, at heart, an image.  Generally
		it will be more efficient to call SimulateTexture in this case than using a texture
		evaluator, since all that really happens inside the evaluator is that the image is
		loaded and sampled. */
	virtual bool IsImageBased(void) const = 0;

	/** Call this method to determine whether the texture is capable of providing a
		high dynamic range image. The default implementation returns \e false. */
	virtual bool IsHDRCapable(void) const;

	/** Call this method to determine whether the texture should be treated as gamma packed (usually to 2.2)
		The default implementation returns \e false. Bitmap textures will return \e true as long as they are
		not in the child slot designated as requiring linear workflow adjustment by the host material.
		See CRhRdkMaterial::TextureRequiresLinearWorkflowAdjustment(). */
	virtual bool IsLinear(void) const;

	/** Call this method to determine if the texture appears to be a normal map. */
	virtual_su bool IsNormalMap(void) const; // [SDK_UNFREEZE]

	/* Call this method to determine whether Advanced Texture Preview will do a better job of displaying this texture than standard mapping. 
	   Implemented using EVF(L"NeedsBaking", nullptr) */
	virtual_su bool NeedsBaking(void) const; // [SDK_UNFREEZE]

	/** Call this method to determine whether Advanced Texture Preview will do a better job of displaying this texture than standard mapping.
		Further, the function will use the rendererId parameter to determine whether the renderer is capable of rendering the texture by
		itself. If that is the case, this function will return false.
		Implemented using EVF(L"NeedsBaking", &rendererId) */
	virtual_su bool NeedsBaking(const ON_UUID& rendererId) const; // [SDK_UNFREEZE]

	/** Call this method to determine whether the provided renderer supports rendering this texture natively without the need to first bake this texture into a bitmap.
		Implemented using EVF(L"SupportedByRenderer", &rendererId) */
	virtual_su bool SupportedByRenderer(const ON_UUID& rendererId) const; // [SDK_UNFREEZE]

	/** Call this method to determine the dimensions of the image in pixels. 
		\param widthOut receives the width (along U) of the texture in pixels if supported.
		\param heightOut receives the height (along V) of the texture in pixels if supported.
		\param depthOut receives the depth (along W) of the texture in pixels if supported.
		\return \e true if the texture is image-based (i.e., a bitmap of some sort), else \e false.
		 When returning \e false, the output values do not have to be filled in.
		\note When implementing a class derived from CRhRdkTexture, if your texture is image-based
		 you must override this method to return the dimensions of your texture. Please make sure you
		 do \e not call the base class as the only thing it does is flag an error to say that the method
		 has not been correctly overridden. \see IsImageBased(). */
	virtual bool PixelSize(int& widthOut, int& heightOut, int& depthOut) const;

	virtual UINT PreviewGeometryFlags(void) const override;
	virtual UINT PreviewBackgroundFlags(void) const override;
	virtual UINT PreviewLightingFlags(void) const override;

	virtual double UnitsToMeters(double dUnits) const;
	virtual double MetersToUnits(double dMeters) const;

	virtual void ConvertMetersToUnits(void) override;
	virtual void ConvertUnitsToMeters(void) override;

	virtual bool SetParameter(const wchar_t*, const CRhRdkVariant&) override;
	virtual CRhRdkVariant GetParameter(const wchar_t*) const override;

	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

public:
	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual CRhRdkPreviewGeometry* NewPreviewGeometry(void) const override;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual CRhRdkPreviewBackground* NewPreviewBackground(void) const override;

	/** \internal For RDK internal use only. */
	virtual void GraphInfo(CRhRdkTextureGraphInfo& infoOut) const;

	/** \internal For RDK internal use only. */
	virtual void SetGraphInfo(const CRhRdkTextureGraphInfo& info);

	/** \internal For RDK internal use only. */
	virtual bool IsCachedTextureFileInUse(const wchar_t* wszTextureFile) const;

	/** \internal For RDK internal use only. Do not override this method in external plug-ins. */
	virtual bool GetFilenameUI(ON_wString& sFilenameOut) const;

	/** \internal For RDK internal use only. */
	enum eLocalMappingType { lmt_none, lmt_2D, lmt_3D, lmt_force32bit = 0xFFFFFFFF };
	virtual eLocalMappingType LocalMappingType(void) const;

	/** \internal For RDK internal use only. */
	static void ClearIconCache(void);

	/** \internal setter for working around non-virtual IsNormalMap problem. */
	void SetIsNormalMap(bool bIsNormalMap);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

#pragma warning(pop)

//Prohibit the use of std::unique_ptr for CRhRdkTexture - use CRhRdkContent::unique_ptr instead.
template <> class std::unique_ptr<CRhRdkTexture> {};

class RHRDK_SDK CRhRdkTextureArray final : public ON_SimpleArray<const CRhRdkTexture*>
{
public:
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED void SetFilename(IRhinoUiController&, const wchar_t*, const wchar_t* csn=nullptr);

	/** Call this method to update all of the textures with a new filename for file based textures. 
		\param file_name is the new filename.
		\param child_slot_name is the child slot name of the filename field.
		 If this is null, FS_TEX_FILENAME will be used. */
	void SetFilename(const wchar_t* file_name, const wchar_t* child_slot_name=nullptr);

	/** Populate this texture array from all textures in a content array. */
	void PopulateFrom(const CRhRdkContentArray& a);

	/** Dump this texture array to a 'dump context'. */
	void Dump(const CRhRdkContent::DumpContext& dc, unsigned int gds_flags) const;
};

RHRDK_SDK int* RhinoGetProceduralPerlinNoiseArray();
RHRDK_SDK unsigned int RhinoGetProceduralPerlinNoiseArraySize();

RHRDK_SDK float* RhinoGetProceduralImpulseNoiseArray();
RHRDK_SDK unsigned int RhinoGetProceduralImpulseNoiseArraySize();

RHRDK_SDK float* RhinoGetProceduralVcNoiseArray();
RHRDK_SDK unsigned int RhinoGetProceduralVcNoiseArraySize();

RHRDK_SDK const int* RhinoGetProceduralAaltonenNoiseArray();
RHRDK_SDK unsigned int RhinoGetProceduralAaltonenNoiseArraySize();

// Does a dynamic cast of 'c' to CRhRdkTexture*. Used to avoid green squiggly warnings.
RHRDK_SDK const CRhRdkTexture* RhRdkContentAsTexture(const CRhRdkContent* c);
