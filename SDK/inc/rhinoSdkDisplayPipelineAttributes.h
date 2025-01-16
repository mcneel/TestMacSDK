//
// Copyright (c) 1993-2023 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#pragma once

#define DECLARE_PIPELINE_ATTRIBUTE_CLASS ON_UUID ClassId() const override;

#define REGISTER_PIPELINE_ATTRIBUTE_CLASS(className, classId) \
static class className##RegistrationHelper\
{	public:	className##RegistrationHelper()\
	{		class Factory : public CDisplayPipelineAttributes::Factory\
		{	virtual ON_UUID ClassId() const { return classId; }\
			virtual CDisplayPipelineAttributes* NewObject() const { return new className; } };\
		CDisplayPipelineAttributes::Factory::Register(std::make_unique<Factory>());}\
} g_the##className##RegistrationHelper;\
UUID className::ClassId() const { return classId; }

class RHINO_SDK_CLASS CDisplayPipelineAttributes
{
	//No longer using MFC bound dynamic creation - use CDisplayPipelineAttributes::Factory instead
	//DECLARE_DYNCREATE( CDisplayPipelineAttributes )
	//Instead, use REGISTER_PIPELINE_ATTRIBUTE_CLASS in the implementation file

public:
	class Factory
	{
	public:
		virtual ~Factory() {}

		virtual ON_UUID ClassId() const = 0;
		virtual CDisplayPipelineAttributes* NewObject() const = 0;

		RHINO_SDK_FUNCTION static void Register(std::unique_ptr<Factory>&& factory);
		RHINO_SDK_FUNCTION static void Remove(const UUID& id);

		static void Cleanup();

		RHINO_SDK_FUNCTION static CDisplayPipelineAttributes* NewFromClassId(const UUID& id);
	};

	//Must match factory Id - CDisplayPipelineAttributes returns ON_nil_uuid 
	virtual ON_UUID ClassId() const;

	CDisplayPipelineAttributes* NewFromThisClass() const;

public:
                      CDisplayPipelineAttributes();
                      CDisplayPipelineAttributes(const CDisplayPipelineAttributes&);
  virtual            ~CDisplayPipelineAttributes();
  CDisplayPipelineAttributes&   operator=(const CDisplayPipelineAttributes&);
  
  bool                operator==(const CDisplayPipelineAttributes&) const;
  
  void GetCurveAttributes( CRhCurveAttributes& attr, const CRhinoDisplayPipeline& dp ) const;
  void GetGridAttributes(CRhinoAppGridSettings& grattr) const;
  void SetGridAttributes(const CRhinoAppGridSettings& grattr);
  /*
  Description:
    Every now and then you want to draw lots of different objects
    using the same color.  This function sets all object drawing
    colors to color.  Examples include the "grey" shadows drawn
    in dynamic transformation commands and printing everything
    black.
  Parameters:
    color - [in]      
  */
  void SetAllObjectDrawingColors( COLORREF color );
  
/////////////////////////////////////
// General display overrides...
public:
  bool                m_bXrayAllObjects;
  bool                m_bIgnoreHighlights;
  bool                m_bDisableConduits;
  bool                m_bDisableTransparency;
  const CRhinoObjectIterator* m_pIteratorOverride;

  ON_UUID             m_CurrentObjectId;
  
  BYTE                m_BBoxMode;
  
/////////////////////////////////////
// General dynamic/runtime object drawing attributes...
public:  
  bool                m_bIsSurface;
  int                 m_IsHighlighted;
  COLORREF            m_ObjectColor;
  bool                m_bShowGrips;
  
  int                 m_nLineThickness;
  UINT                m_nLinePattern;
  bool                m_bBakeTextures;
  bool                m_bShowDecals;

  // Added in V8
  void SetFromDisplayObjectAttrs(const class CDisplayObjectAttrs* attr, const CRhinoObject* obj);
  // Added in V8
  ON_Color BaseObjectColor() const;

  // Added in V8
  void CopyImplementationAttrs(const class CDisplayPipelineAttributes* attrs, const CRhinoObject* obj);

  // Runtime material used by all mesh drawing (shaded and/or rendered)
  CDisplayPipelineMaterial*  m_pMaterial;
/////////////////////////////////////
// View specific attributes...
public:
  // The engine will typically use the current monitor dpi scaling unless
  // m_dpi_scale_override is set to a value > 0
  float m_dpi_scale_override = -1;
public:  
  bool                  m_bUseDocumentGrid;
  bool                  m_bDrawGrid;
  bool                  m_bDrawAxes;
  bool                  m_bDrawZAxis;
  bool                  m_bDrawWorldAxes;
  bool                  m_bShowGridOnTop;
  bool                  m_bShowTransGrid;
  bool                  m_bBlendGrid;
  int                   m_nGridTrans;
  bool                  m_bDrawTransGridPlane;
  int                   m_nGridPlaneTrans;
  int                   m_nAxesPercentage;
  bool                  m_bPlaneUsesGridColor;
  COLORREF              m_GridPlaneColor;
  int                   m_nPlaneVisibility;
  int                   m_nWorldAxesColor;
  COLORREF              m_WxColor;
  COLORREF              m_WyColor;
  COLORREF              m_WzColor;

  enum class GroundPlaneUsages
  {
    ByDocument=0,
    Custom=1,
  } m_groundPlaneUsage;

  struct 
  {
    bool on = true;
    bool shadowOnly = true;
    double altitude = 0.0;
    bool automaticAltitude = true;
    //ON_UUID materialId = ON_nil_uuid;
  } 
  m_customGroundPlaneSettings;

  bool IsGroundPlaneOff() const;

  enum class LinearWorkflowUsages
  {
    ByDocument=0,
    Custom=1,
  } m_linearWorkflowUsage;

  //Copy of IRhRdkLinearWorkflow - eventually these classes need to be combined.
  class RHINO_SDK_CLASS LinearWorkflow
  {
  public:
    /* DEPRECATED*/ void SetPreProcessColors(bool);
    /* DEPRECATED*/ bool PreProcessColors() const;

    void SetPreProcessColorsOn(bool);
    bool PreProcessColorsOn() const;

    /* DEPRECATED*/ void SetPreProcessTextures(bool);
    /* DEPRECATED*/ bool PreProcessTextures() const;

    void SetPreProcessTexturesOn(bool);
    bool PreProcessTexturesOn() const;

    /* DEPRECATED*/ void SetPostProcessFrameBuffer(bool);
    /* DEPRECATED*/ bool PostProcessFrameBuffer() const;

    void SetPostProcessFrameBufferOn(bool);
    bool PostProcessFrameBufferOn() const;

    void SetPreProcessGamma(float);
    float PreProcessGamma() const;

    void SetPostProcessGamma(float);
    float PostProcessGamma() const;

    bool PostProcessGammaOn(void) const;
    void SetPostProcessGammaOn(bool bOn);

    //Use this function to determine the gamma correction factor for input non-linear colors.
    //Will return 1.f if PreProcessColors returns false
    float GammaAdjustmentForPreProcessingColors() const;

    //Use this function to determine the gamma correction factor for input non-linear textures.
    //Will return 1.f if PreProcessTextures returns false
    float GammaAdjustmentForPreProcessingTextures() const;

    //Use this function to determine the gamma correction factor for rendered output pixels.
    //This will return the reciprocal value - not the UI value.
    //Use this value in the pow function directly.
    //Will return 1.f if PostProcessFrameBuffer returns false
    float GammaAdjustmentForPostProcessingFrameBuffer() const;

  public:
    ON__UINT32 ComputeCRC() const;
    bool IsEqual(const LinearWorkflow&) const;

  private:
    bool  bPreProcessColors = false;
    bool  bPreProcessTextures = false;
    bool  bPostProcessFrameBuffer = false;
    float fPreProcessGamma = 1.f;
    float fPostProcessGamma = 1.f;
    float fPostProcessGammaReciprocal = 1.f;
    ON__UINT32 bPostProcessGammaOn = 0;
    ON__UINT32 reserved2 = 0;
    ON__UINT32 reserved3 = 0;
  }
  m_customLinearWorkflowSettings;

  /////////////////////////////////////
  // Frame background settings...
  EFrameBufferFillMode  m_eFillMode;
  ON_Color              m_SolidColor;

private:
  ON_wString            m_background_bitmap_name;
  mutable std::shared_ptr<CRhinoDib> m_background_bitmap_dib;
public:
  void SetBackgroundBitmap(const wchar_t* name);
  void SetBackgroundBitmap(std::shared_ptr<CRhinoDib>& dib);
  const wchar_t* BackgroundBitmapName() const;
  const std::shared_ptr<CRhinoDib>& BackgroundBitmap() const;

  ON_Color              m_GradTopLeft;
  ON_Color              m_GradBotLeft;
  ON_Color              m_GradTopRight;
  ON_Color              m_GradBotRight;

  /////////////////////////////////////
  // Stereo mode settings...
  int                   m_nStereoModeEnabled;
  float                 m_fStereoSeparation;
  float                 m_fStereoParallax;
  int                   m_nAGColorMode;
  int                   m_nAGViewingMode;
  bool                  m_bFlipGlasses;
  
  double                m_dHorzScale;
  double                m_dVertScale;
 
  /////////////////////////////////////
  // Custom clippling plane attributes
  bool                  m_bShowClippingPlanes;
  bool                  m_bClippingShowXSurface;
  bool                  m_bClippingShowXEdges;
  bool                  m_bClippingClipSelected;
  bool                  m_bClippingShowCP;       // Draw a transparent fill for selected clipping planes
  BYTE                  m_nClippingSurfaceUsage; // 0 = object color settings,  1 = display mode backface settings, 2 = clipping plane's color, 3 = solid color
  BYTE                  m_nClippingEdgesUsage;   // 0 = clipping plane's color setting, 1 = solid color, 2 = object color setting
  BYTE                  m_nClippingCPUsage;      // 0 = clipping plane's color, 1 = clipping plane's material, 2 = solid color
  BYTE                  m_nClippingCPTrans;      // 0 = opaque ... 100 = transparent
  BYTE                  m_nClippingEdgeThickness;
  COLORREF              m_ClippingSurfaceColor;
  COLORREF              m_ClippingEdgeColor;
  COLORREF              m_ClippingCPColor;
  BYTE ClipSectionUsage() const; // 0 = use section styles, 1 = use attributes defined above
  void SetClipSectionUsage(BYTE usage);

/////////////////////////////////////
// Curves specific attributes...
public:
  bool                m_bShowCurves;
  int                 m_nCurveThickness;
  int                 m_nCurveTrans;
  UINT                m_nCurvePattern;
  bool                m_bCurveKappaHair;
  bool                m_bSingleCurveColor;
  COLORREF            m_CurveColor;
  RhinoLineCapStyle   m_eLineEndCapStyle;
  RhinoLineJoinStyle  m_eLineJoinStyle;
  BYTE CurveThicknessUsage() const; // 0 = by object, 1 = pixel thickness
  void SetCurveThicknessUsage(BYTE usage);
  float CurveThicknessScale() const;
  void SetCurveThicknessScale(float scale);

/////////////////////////////////////
// Both surface and mesh specific attributes...
public:
  bool                m_bShadeSurface;
  
  bool                m_bUseObjectMaterial;
  bool                m_bSingleWireColor;
  COLORREF            m_WireColor;

  bool                m_bUseDefaultBackface;  
  bool                m_bUseObjectBFMaterial;
  bool                m_bCullBackfaces;
  bool                m_bShadeVertexColors;
  bool SurfaceColorWriting() const;
  void EnableSurfaceColorWriting(bool on);

  std::shared_ptr<const CRhinoIsoDrawEffect> IsoDrawEffectSettings() const;
  void SetIsoDrawEffectSettings(const CRhinoIsoDrawEffect* settings);
  void SetIsoDrawEffectSettings(std::shared_ptr<const CRhinoIsoDrawEffect>& settings);

  /////////////////////////////////////
  // Technical mode(s) settings...
  UINT                m_nTechMask;
  UINT                m_nTechUMask;
  int                 m_nTHThickness;  // Hidden lines
  int                 m_nTEThickness;  // Edge lines
  int                 m_nTSiThickness; // Silhouette lines
  int                 m_nTCThickness;  // Crease lines
  int                 m_nTSThickness;  // Seam lines
  int                 m_nTIThickness;  // Intersection lines
  
  COLORREF            m_nTHColor;  // Hidden lines
  COLORREF            m_nTEColor;  // Edge lines
  COLORREF            m_nTSiColor; // Silhouette lines
  COLORREF            m_nTCColor;  // Crease lines
  COLORREF            m_nTSColor;  // Seam lines
  COLORREF            m_nTIColor;  // Intersection lines

  /////////////////////////////////////
  // Specialized shading effects...
  BYTE                m_nShadingEffectsUsage;
  BYTE                m_SEPL_Width;      // ShadedEffectParallelLine-Width
  BYTE                m_SEPL_Separation; // ShadedEffectParallelLine-Separation
  int                 m_SEPL_Rotation;   // ShadedEffectParallelLine-Rotation

/////////////////////////////////////
// Surface specific attributes...
public:
  bool                m_bSurfaceKappaHair;
  bool                m_bHighlightSurfaces;
  
  // iso's...
  bool                m_bShowIsocurves;
  
  bool                m_bIsoThicknessUsed;
  int                 m_nIsocurveThickness; // not used
  int                 m_nIsoUThickness;
  int                 m_nIsoVThickness;
  int                 m_nIsoWThickness;

  bool                m_bSingleIsoColor;
  COLORREF            m_IsoColor;
  bool                m_bIsoColorsUsed;
  COLORREF            m_IsoUColor;
  COLORREF            m_IsoVColor;
  COLORREF            m_IsoWColor;
  
  bool                m_bIsoPatternUsed;
  UINT                m_nIsocurvePattern;
  UINT                m_nIsoUPattern;
  UINT                m_nIsoVPattern;
  UINT                m_nIsoWPattern;
  
  // brep edge info (not used for mesh or SubD objects)
  bool                m_bShowEdges;
  bool                m_bShowNakedEdges;
  bool                m_bShowEdgeEndpoints;
  bool ShowTangentEdges() const;
  void SetShowTangentEdges(bool show);
  bool ShowTangentSeams() const;
  void SetShowTangentSeams(bool show);
  unsigned char SurfaceThicknessUsage() const;
  void SetSurfaceThicknessUsage(unsigned char usage);
  float SurfaceEdgeThicknessScale() const;
  void SetSurfaceEdgeThicknessScale(float scale);
  bool SurfaceEdgeApplyPattern() const;
  void SetSurfaceEdgeApplyPattern(bool apply);

  void SetSurfaceNakedEdgeUseNormalEdgeThickness(bool on);
  bool SurfaceNakedEdgeUseNormalEdgeThickness() const;
  float SurfaceNakedEdgeThicknessScale() const;
  void SetSurfaceNakedEdgeThicknessScale(float scale);
  bool SurfaceNakedEdgeApplyPattern() const;
  void SetSurfaceNakedEdgeApplyPattern(bool apply);

  void SurfaceIsoThicknessScale(float* u, float* v, float* w) const;
  void SetSurfaceIsoThicknessScale(float u, float v, float w);
  void SurfaceIsoApplyPattern(bool* u, bool* v, bool* w) const;
  void SetSurfaceIsoApplyPattern(bool u, bool v, bool w);

  bool ShowFlatSurfaceIsos() const;
  void SetShowFlatSurfaceIsos(bool show);

  int                 m_nEdgeThickness;
  int                 m_nEdgeColorUsage;
  int                 m_nNakedEdgeOverride; // not used
  int                 m_nNakedEdgeColorUsage;
  int                 m_nNakedEdgeThickness;
  int                 m_nEdgeColorReduction;
  int                 m_nNakedEdgeColorReduction;
  
  // brep edge info (not used for mesh or SubD objects)
  COLORREF            m_EdgeColor;
  COLORREF            m_NakedEdgeColor;

  UINT                m_nEdgePattern;
  UINT                m_nNakedEdgePattern;

  // brep edge non-manifold specific  (not used for mesh or SubD objects)
  UINT                m_nNonmanifoldEdgePattern;
  COLORREF            m_NonmanifoldEdgeColor;
  bool                m_bShowNonmanifoldEdges;
  
/////////////////////////////////////
// SubD specific attributes...
public:
  enum class SubDEdgeType : int
  {
    SmoothInterior = 0,
    CreaseInterior = 1,
    NonManifold = 2,
    Boundary =3
  };
  // [0] == smooth interior
  // [1] == crease interior
  // [2] == non-manifold
  // [3] == boundary
  static CDisplayPipelineAttributes::SubDEdgeType SubDEdgeEdgeType(const ON_SubDEdgePtr*);
  static CDisplayPipelineAttributes::SubDEdgeType SubDEdgeEdgeType(const ON_SubDEdge*);
  void GetSubDWireAttributes(ON_Color colors[4], float thicknesses[4], bool applyXforms = true) const;
  void SetSubDEdgeVisibility(bool on);
  void SetSubDEdgeVisibility(SubDEdgeType edgeType, bool on);
  bool GetSubDEdgeVisibility(SubDEdgeType edgeType) const;
  ON_Color SubDEdgeColor(SubDEdgeType edgeType) const;
  void SetSubDEdgeColor(SubDEdgeType edgeType, const ON_Color& color);
  void SetSubDEdgeThickness(SubDEdgeType edgeType, float thickness);
  float SubDEdgeThickness(SubDEdgeType edgeType) const;
  void GetSubDUsageIndexes(int[4]) const;
  void SetSubDUsageIndexes(int[4]);
  void SetSubDEdgeUsage(SubDEdgeType edgeType, int usage);
  int SubDEdgeUsage(SubDEdgeType edgeType) const;
  void GetSubDColorReductions(int[4]) const;
  int SubDEdgeColorReduction(SubDEdgeType edgeType) const;
  void SetSubDEdgeColorReduction(SubDEdgeType edgeType, int reduction);

  unsigned char SubDThicknessUsage() const;
  void SetSubDThicknessUsage(unsigned char usage);
  float SubDThicknessScale(SubDEdgeType edgeType) const;
  void SetSubDThicknessScale(SubDEdgeType edgeType, float scale);
  bool SubdApplyPattern(SubDEdgeType edgeType) const;
  void SetSubDApplyPattern(SubDEdgeType edgeType, bool apply);
  bool SubDApplyPattern(SubDEdgeType edgeType) const;

  // SubD Reflection plane previewing attributes...
  void ShowSubDReflectionPlanePreview(bool show);
  bool SubDReflectionPlanePreviewOn(void) const;
  void ShowSubDReflectionPlaneAxisLine(bool show);
  bool SubDReflectionPlaneAxisLineOn(void) const;
  void SetSubDReflectionPlaneColorUsage(int);
  int  SubDReflectionPlaneColorUsage(void) const;
  void SetSubDReflectionPlaneColor(const ON_Color&);
  ON_Color SubDReflectionPlaneColor(void) const;
  void SetSubDReflectionColorPercentage(int);
  int  SubDReflectionColorPercentage(void) const;
  void SetSubDReflectionAxisLineColor(const ON_Color&);
  ON_Color SubDReflectionAxisLineColor(void) const;
  void SetSubDReflectionAxisLineThickness(float);
  float SubDReflectionAxisLineThickness(void) const;
  void SetSubDReflectionPlane(const ON_PlaneEquation* pe);
  bool SubDReflectionPlane(ON_PlaneEquation& pe) const;

public:
  void ResetReflectionPlaneSettings(void);    // turns off reflected plane preview drawing (for runtime evaluation)...
  void UseSubDReflectionPlaneSettings(void);  // sets up reflected plane preview drawing based on the SubD's settings (this may turn it on or off at runtime)

  // Get the "current" reflected plane preview drawing settings...
  bool ReflectionPlaneExists(void) const;
  bool ReflectionPlanePreviewOn(void) const;
  bool ReflectionPlaneAxisLineOn(void) const;
  int  ReflectionPlaneColorUsage(void) const;
  ON_Color ReflectionPlaneColor(const ON_Color&) const;
  int  ReflectionColorPercentage(void) const;
  ON_Color ReflectionAxisLineColor(void) const;
  float ReflectionAxisLineThickness(void) const;
  ON_PlaneEquation ReflectionPlane(void) const;

/////////////////////////////////////
// Mesh specific attributes...
public:
  // [0] == non-manifold wires (3 or more faces attach to the edge)
  // [1] == crease wires (2 faces attach to a crease or break edge)
  // [2] == manifold wires (2 faces attach to a smooth edge)
  // [3] == naked edge wires (boundary edge - 1 face)
  void GetMeshWireAttributes(ON_Color colors[4], float thicknesses[4], unsigned int patterns[4]) const;
  bool                m_bHighlightMeshes;
  bool                m_bSingleMeshWireColor;
  COLORREF            m_MeshWireColor;
  int                 m_nMeshWireThickness;
  UINT                m_nMeshWirePattern;
  bool                m_bShowMeshWires;
  bool                m_bShowMeshVertices;
  int                 m_nMeshVertexSize;

  // polygon mesh and SubD edge info
  bool                m_bShowMeshEdges;            // here "Edge" means "break" or "crease" edges
  bool                m_bShowMeshNakedEdges;       // "Naked" means boundary edges
  bool                m_bShowMeshNonmanifoldEdges; // "Nonmanifold means 3 or more faces meet at the edge
  
  int                 m_nMeshEdgeThickness;        // here "Edge" means "break" or "crease" edges

  int                 m_nMeshNakedEdgeThickness;
  int                 m_nMeshEdgeColorReduction;   // here "Edge" means "break" or "crease" edges
  int                 m_nMeshNakedEdgeColorReduction; 
  COLORREF            m_MeshEdgeColor;             // here "Edge" means "break" or "crease" edges
  COLORREF            m_MeshNakedEdgeColor;

  // polygon mesh and SubD non-manifold specific 
  int                 m_nMeshNonmanifoldEdgeThickness;
  int                 m_nMeshNonmanifoldEdgeColorReduction;
  COLORREF            m_MeshNonmanifoldEdgeColor;

/////////////////////////////////////
// Object locking attributes....
public:
  int                 m_nLockedUsage;
  bool                m_bGhostLockedObjects;
  int                 m_nLockedTrans;
  COLORREF            m_LockedColor;
  bool                m_bLockedObjectsBehind;
  bool                m_bLayersFollowLockUsage;
  
/////////////////////////////////////
// Dimensions & Text specific attributes...
public:
  bool                m_bShowText;
  bool                m_bShowAnnotations;
  COLORREF            m_DotTextColor;
  COLORREF            m_DotBorderColor;

/////////////////////////////////////
// Lights & Lighting specific attributes...
public:
  bool                m_bShowLights;
  bool                m_bUseHiddenLights;
  bool                m_bUseLightColor;

  ELightingScheme     m_eLightingScheme;
  
  int                 m_nLuminosity;
  COLORREF            m_AmbientColor;

  ON_ObjectArray<ON_Light>  m_Lights;
  
  int                 m_eShadowMapType;
  int                 m_nShadowMapSize;
  int                 m_nNumSamples;
  COLORREF            m_ShadowColor;
  ON_3dVector         m_ShadowBias;
  double              m_fShadowBlur;
  bool                m_bCastShadows;
  BYTE                m_nShadowBitDepth;
  BYTE                m_nTransparencyTolerance;
  bool                m_bPerPixelLighting;
  BYTE                m_bShadowClippingUsage;    // First nibble (0..15) = types of clipping, Second nibble (0x10 .. 0x80) = mask for when to clip.
  float               m_fShadowClippingRadius;
  
/////////////////////////////////////
// Points specific attributes...
public:
  bool                m_bShowPoints;
  int                 m_nPointSize;
  ERhinoPointStyle    m_ePointStyle;
  
/////////////////////////////////////
// Control Polygon specific attributes...
public:
  bool                m_bCPSolidLines;
  bool                m_bCPSingleColor;
  bool                m_bCPHidePoints;
  bool                m_bCPHideSurface;
  bool                m_bCPHighlight;
  bool                m_bCPHidden;
  int                 m_nCPWireThickness;

  int                 m_nCVSize;
  ERhinoPointStyle    m_eCVStyle;

  ON_Color            m_CPColor;
  
/////////////////////////////////////
// PointClouds specific attributes...
public:
  bool                m_bShowPointClouds;
  int                 m_nPCSize;
  ERhinoPointStyle    m_ePCStyle;
  int                 m_nPCGripSize;
  ERhinoPointStyle    m_ePCGripStyle;

//////////////////////////////////////
// Extra information for DrawToDC calls...
public:
  enum class ContextsForDrawToDC : unsigned int
  {
	  kUnset = 0,			//CRhinoDisplayPipeline::DrawToDC will ASSERT if called with this value.  Proceed your call with one of the values below.
	  kFilePreview = 1,		//called from GetPreviewBitmap
	  kViewCapture = 2,		//ViewCaptureToFile or ViewCaptureToClipboard
	  kPrinting = 3,		//Printing
	  kUIPreview = 4,		//Used by various dialogs to draw the document into a bitmap.
	  kMask = 5,
	  kRenderOverlays = 6,
	  kOther = 0xFFFFFFFF	//3rd party call
  } m_contextDrawToDC;
  
/////////////////////////////////// 
// general class object attributes...
public:
  ON_UUID               Id() const;
  
  ON_wString            Name() const;
  
  virtual
  const wchar_t*        EnglishName() const;
  
  virtual
  const wchar_t*        LocalName() const;
  
  const ON_ClassId*     Pipeline() const;
  ON_UUID               PipelineId() const;
  
  void                  SetUuid(ON_UUID);
  void                  SetName(const ON_wString&);
  void                  SetPipeline(const ON_ClassId*);

  ON_UUID               RealtimeDisplayId() const;
  void                  SetRealtimeDisplayId(const ON_UUID& id);

  int                   RealtimeRenderPasses(void) const;
  void                  SetRealtimeRenderPasses(int);

  bool                  ShowRealtimeRenderProgressBar(void) const;
  void                  SetShowRealtimeRenderProgressBar(bool);

  int                   BackgroundStyleOverride() const;  //-1 means do not override style.  Otherwise the value from ON_3dmRenderSettings::m_background_style is used.
  void                  SetBackgroundStyleOverride(int);

  int                   BackgroundStyleHelper(const CRhinoDoc& doc) const;

  bool                  SupportsCustomRenderMeshes(void) const;
  void                  SetSupportsCustomRenderMeshes(bool bSupports);

  int                   ShadowIntensity(void) const;
  void                  SetShadowIntensity(int);

  enum class StereoRenderContext : unsigned int
  {
    NotApplicable = 0,
    RenderingLeftEye = 1,
    RenderingRightEye = 2,
    RenderingBothEyes = RenderingLeftEye | RenderingRightEye
  };

  StereoRenderContext GetStereoRenderContext() const;
  void SetStereoRenderContext(StereoRenderContext stereo_render_context);

  int GetSkylightShadowQuality() const;
  void SetSkylightShadowQuality(int iSkylightShadowQuality) const;

  bool ShadowsIgnoreClippingPlanes(void) const;
  
  int  GetShadingMethod(void) const;
  void SetShadingMethod(int);

  CRhinoDisplayPipeline*  NewDisplayPipeline() const;
  
  bool SaveProfile(const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadProfile(const wchar_t* lpsSection, CRhinoProfileContext& );
  void LoadProfile(const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);

  // Description:
  //  Set a color fade effect to make objects fade a given amount towards a given color.
  // Parameters:
  //  fade_color: [in] the color to fade towards.
  //  fade_amount: [in] the amount of fade towards the given color (0..1).
  void SetColorFadeEffect(const ON_Color& fade_color, const float fade_amount);

  // Description:
  //  Returns TRUE if there is a color fade effect enabled with a color fade effect amount
  //  larger than 0.0, FALSE otherwise.
  bool HasColorFadeEffect() const;

  // Description:
  //  Get the current color fade effect data.
  // Parameters:
  //  fade_color: [out] the current fade color.
  //  fade_amount: [out] the current fade amount.
  void GetColorFadeEffect(ON_Color& fade_color, float& fade_amount) const;

  // Description:
  //  Set a dither transparency effect to make objects render with a given amount of
  //  transparency using a dither effect.
  // Parameters:
  //  transparency_amount: [in] the amount of transparency (0..1).
  void SetDitherTransparencyEffect(const float transparency_amount);

  // Description:
  //  Returns TRUE if there is a dither transparency effect enabled with a transparency
  //  amount larger than 0.0, FALSE otherwise.
  bool HasDitherTransparencyEffect() const;

  // Description:
  //  Get the current dither transparency amount.
  // Returns:
  //  The transparency amount.
  float GetDitherTransparencyEffectAmount() const;

  // Description:
  //  Set a diagonal hatch effect to make objects render with diagonal hatch with
  //  a given strength and width in pixels. The effect works by brightening and 
  //  darkening pixels in a diagonal pattern.
  // Parameters:
  //  hatch_strength: [in] the strength of the hatch effect (0..1).
  //  hatch_width: [in] the width of the diagonal hatch in pixels (>= 0).
  void SetDiagonalHatchEffect(const float hatch_strength, const float hatch_width);

  // Description:
  //  Returns TRUE if there is a diagonal hatch effect enabled with a hatch strength
  //  larger than 0.0, FALSE otherwise.
  bool HasDiagonalHatchEffect() const;

  // Description:
  //  Get the current diagonal hatch strength and width in pixels.
  // Parameters:
  //  hatch_strength: [out] the strength of the hatch effect.
  //  hatch_width: [out] the diagonal hatch width in pixels.
  void GetDiagonalHatchEffect(float& hatch_strength, float& hatch_width) const;

protected:
  virtual void Defaults();
  
public:
  ON_SHA1_Hash CurrentMaterialHash() const;
  void ResetMaterialHash();
  void ComputeMaterialHash();
  void SetActiveCacheHandle(CRhinoCacheHandle* cache);
  const CRhinoCacheHandle* ActiveCacheHandle() const;
private:
  ON_UUID               m_uuid;
  ON_wString            m_sName;
  ON_UUID               m_PipelineId;
  ON_UUID               m_RealtimeDisplayId;
  const ON_ClassId*     m_pPipeline;

  bool SaveViewAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadViewAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);

  bool SaveShadingAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadShadingAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);

  bool SaveLightingAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadLightingAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);

  bool SaveObjectAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadObjectAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
    
  bool SaveSurfaceAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadSurfaceAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
    
  bool SaveMeshAttrs(const wchar_t* lpsSection, CRhinoProfileContext&) const;
  void LoadMeshAttrs(const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);

  bool SaveSubDAttrs(const wchar_t* lpsSection, CRhinoProfileContext&) const;
  void LoadSubDAttrs(const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);

  bool SaveReflectedPlaneAttrs(const wchar_t* lpsSection, CRhinoProfileContext&, class CRhReflectionPlaneAttrs&) const;
  void LoadReflectedPlaneAttrs(const wchar_t* lpsSection, CRhinoProfileContext&, class CRhReflectionPlaneAttrs&, const class CRhReflectionPlaneAttrs& Defaults);

  bool SaveCurveAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadCurveAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
  
  bool SavePointAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadPointAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
    
  bool SaveAnnotationAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadAnnotationAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
    
  bool SaveMaterialAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadMaterialAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
  
  bool SaveTechnicalAttrs( const wchar_t* lpsSection, CRhinoProfileContext& ) const;
  void LoadTechnicalAttrs( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayPipelineAttributes& Defaults);
  
   
  bool SaveMaterial( const wchar_t* lpsSection, CRhinoProfileContext&, const CDisplayAttributeMaterial& ) const;
  void LoadMaterial( const wchar_t* lpsSection, CRhinoProfileContext&, CDisplayAttributeMaterial&, const CDisplayAttributeMaterial& defaults );
    
  class CRhDisplayPipelineAttributesPrivate* m_private = nullptr;
  
  friend class CRhinoDisplayPipeline;
  friend class CDisplayAttrsPage;
  friend class CRhinoDisplayAttrsMgr;
  friend class DisplayAttrsMgrList;
  friend class DisplayAttrsMgrListDesc;
  friend class CRhDisplayPipelineAttributesPrivate;
  friend class CShadowsAttributesPage;
};
