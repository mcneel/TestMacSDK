//
// Copyright (c) 1993-2021 Robert McNeel & Associates. All rights reserved.
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

//////////////////////////////////////////////
// OpenGL engine class...
//
class RHINO_SDK_CLASS CRhinoDisplayEngine_OGL : public CRhinoDisplayEngine
{
  ON_OBJECT_DECLARE(CRhinoDisplayEngine_OGL);
public:
  CRhinoDisplayEngine_OGL();
  virtual ~CRhinoDisplayEngine_OGL();

  // Return the available OpenGL level for the current instance of Rhino
  // The level is returned as major*10+minor, so OpenGL 4.5 would return 45
  // Parameters:
  //   coreProfile: is set to true if operating in core profile mode
  static unsigned int AvailableGLLevel(bool* coreProfile);

  // Activate the 'default' context
  static bool MakeDefaultContextCurrent();
public:
  bool  StartEngine() override;
  
  bool  StopEngine() override;

  bool  ResetEngine() override;
  
  void  ShutdownEngine() override;
  
  int   ActiveStereoProjection() const override; // 0 = left, 1 = right...
  
  bool  SetStereoMode(bool) override;

  int   MaxLightsSupported() const override;

  int   MaxObjectMappingsSupported() const override;

  int   MaxDecalsSupported() const override;
  
  int   MaxClipPlanesSupported() const override;
  
  bool  IsAccelerated() const override;

  bool  SupportsShading() const override;
  
  bool  SupportsMultiSampling() const override;
  
  void  GetMultiSampleModes(ON_SimpleArray<int>&) const override;
  
  bool  SetMultiSampleMode(int) override;
  
  int   GetMultiSampleMode() const override;
  
  bool  SupportsStencilBuffer() const;
  
  bool IsPreview(void) const;
  void SetIsPreview(bool);

  ON_UUID PreviewEnvironmentId(void) const;
  void SetPreviewEnvironmentId(ON_UUID);

  void EnableMeshBuffering(bool on) override;
  bool MeshBufferingEnabled() const override;
public:
  void ClearFrameBuffer(const ON_Color&, const ON_Color&, const ON_Color&, const ON_Color&) override;
  void ClearFrameBuffer(const CRhinoDib*) override;
  
  void ClearDepthBuffer() override;

  void AddLight(const ON_Light&) override;
  
  void DrawPoints(int count, const class CRhPointData* points, CRhinoCacheHandle*) override;

  void DrawLines(const class CLinesDescriptor&) override;
  
  void DrawPolylines(const CRhinoClippingVertexCollection* lists, int count) override;

  void DrawMeshes(const ON_Mesh** meshes, int count, bool wires, bool shaded, const CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle) override;

  void DrawMeshFaces(const ON_Mesh&                    mesh,
                     const int* faces, int faceCount,
                     const CDisplayPipelineAttributes* da,
                     CRhinoCacheHandle* cacheHandle) override;

  void DrawHatch(const ON_Hatch& hatch, const ON_HatchPattern* pattern, float strokeWidth, ON_Color hatchColor, ON_Color boundaryColor, CRhinoCacheHandle* cacheHandle) override;

  void DrawHatch(const ON_Hatch& hatch, const class CRhinoDisplayBrush* fillBrush, const class CRhinoDisplayPen* boundaryPen, CRhinoCacheHandle* cacheHandle) override;

  void DrawCurve(const ON_Curve& curve, const class CRhinoDisplayPen& pen, CRhinoCacheHandle* cacheHandle) override;

  void DrawSubD(
    const class CRhinoSubDDisplay& subd_display,
    const class CRhinoSubDDisplayAttributes& subd_display_attributes
    ) override;

  void DrawBrepWires(const ON_Brep* brep, const class CDisplayPipelineAttributes* da, int wireDensity, bool edgeAnalysis, CRhinoCacheHandle* cacheHandle) override;
  void DrawExtrusionWires(const ON_Extrusion& extrusion, const class CDisplayPipelineAttributes* da, int wireDensity, bool edgeAnalysis, CRhinoCacheHandle* cacheHandle) override;

  
  void Draw2dLines(const ON_2fPoint* points, int count,
        bool asPolyline,
        ON_Color strokeColor,
        float strokeWidth,
        RhinoDashStyle style,
        bool xormode) override;

  void DrawRoundedRectangle(ON_2fPoint screenCenter,
        float pixelWidth,
        float pixelHeight,
        float cornerRadius,
        ON_Color strokeColor,
        float strokeWidth,
        ON_Color fillColor
        ) override;

  void DrawDot(const ON_TextDot& dot,
        bool locationInWorld,
        ON_Color fillColor,
        ON_Color textColor,
        ON_Color borderColor,
        CRhinoCacheHandle* cache) override;
  
  void DrawPointCloud(const ON_PointCloud& cloud, ERhinoPointStyle style,
        float pixelSize, bool scaleSizeForDpi, ON_Color colorOverride,
        int indexCount, const unsigned int* indices,
        CRhinoCacheHandle* cache, int degradeCount) override;

  void DrawString(const wchar_t* text, const ON_Xform& xf, const ON_Font* font, const ON_Color& color,
    ON::TextHorizontalAlignment horizontalAlignment, ON::TextVerticalAlignment verticalAlignment,
    CRhinoCacheHandle* cache) override;

  // Description:
  //   Draws text with specified color, font and height
  void DrawString(const wchar_t* text, const ON_Color& textColor, const ON_2dPoint& point,
    bool middle, float fontHeightPixels, const ON_Font* font ) override;

  void DrawPolygon(
        const ON_3dPoint*  points,
              int          count,
              ON_Color     color,
              bool         bFilled
        ) override;

  // Description:
  //   Draws wallpaper image into the back buffer.
  void DrawWallpaper( const class CRhWallpaper& wallpaper ) override;
  
  // Description:
  //   Draws background bitmap image onto the construction plane.
  void DrawTraceImage( const class CRhTraceImage& traceimage ) override;

  // Description:
  //  Draws background environment into the frame buffer.
  void DrawEnvironment() override;

  // Description:
  //  Draws a ground plane into the frame buffer.
  void DrawGroundPlane(const class CDisplayPipelineAttributes& dpa, const class CRhDisplayGroundPlane& groundplane) override;

  void DrawBitmap(const CRhinoDib& dib, int left, int top) override;
  void DrawBitmap(const CRhinoDib& dib, int left, int top, int width, int height);

  bool PostProcessFrameBuffer();

  void ProcessLinearWorkflow(const class CRhOpenGLTexture& image, const float gammaExponent);
  void DrawViewportAlignedQuad(bool bProgramInUse);
  void DrawViewportAlignedImage(const class CRhOpenGLTexture& image, bool bProgramInUse);
  void TransformShadowMatrix(bool bTransform);
  bool DrawCombinedTextures(UINT  nT1, UINT  nT2, int  nCOP, float Scale, ON_Color Color = ON_UNSET_COLOR);

private:
  // From CRhEngineExtension class. These functions should change
  // as we clean up V6
  /////////////////////////////////////////////////////////////////////////
  class CRhGLSceneManager* GetSceneManager() override;
  class CRhGLSceneManager* GetSceneManager(unsigned int docSerialNumber) override;

  bool DrawLines( ON_Color color, int line_count, float thickness, const float* vertices, const ON_Xform* model_xform ) override;

///////////////////////////////////////////////////////////////////////////
// Caching operations...
public:
  void InvalidateCapturedFrame() override;
  bool CaptureFrame(const ON_Viewport& viewport) override;
  const class CRhFrameCapture* GetCapturedFrame() const override;
  bool DrawCapturedFrame(const ON_Viewport& destination) override;
  
  bool CaptureFrameTo(class CRhOpenGLTexture2D&);

///////////////////////////////////////////////////////////////////////////
// Depth-Buffered operations & attributes...
public:
  void EnableDepthTesting(bool=true) override;

  void EnableDepthWriting(bool = true) override;
  
  void EnableColorWriting(bool = true) override;
  
  ECullFaceMode SetCullFaceMode(ECullFaceMode mode) override;
  
  int  SetDepthMode(int) override;
  int  DepthMode();

  int  FrameBufferBitDepth() override;

  void FrameBufferChannelDepth(int&, int&, int&, int&) override;

  int  DepthBufferBitDepth() override;
  
  void ClearTextureCache() override;
  
  void EnableMultiSample(bool bEnable=true) override;

  void EnableShadowMap(bool bEnable=true);
  bool IsShadowMapEnabled();
  
///////////////////////////////////////////////////////////////////////////
// Engine buffers support...
public:
  bool HasDepthBuffer() override;
  
  bool CopyDepthBufferTo(CRhinoDib&, bool=false, bool=false, CRhinoDib* = nullptr) override;
      
  bool GetBackBuffer(CRhinoDib&) override;
  
  bool GetDepthBuffer(float* buffer, 
                      int    left,  int    top, 
                      int    width, int    height) override;

  bool CopyBackBufferTo(CRhinoDib&) override;
  
  bool CopyBackBufferFrom(const CRhinoDib&) override;

  bool GetOffscreenBufferInfo(HDC dc, int&, int&) override;
  
  bool SaveBackBuffer() override;
  
  bool RestoreBackBuffer() override;
  
  bool CanSaveDepthBuffer() override;

  bool SaveDepthBuffer() override;

  bool RestoreDepthBuffer() override;

  bool ShowBackBuffer(HDC dc) override;

  void GetGlWorldToCamera(float m[16], bool includeModelTransform, const ON_Xform* extra) const;
  void GetGlWorldToClip(float m[16], bool includeModelTransform, const ON_Xform* extra) const;
  void GetGlCameraToClip(float m[16], const ON_Xform* extra) const;
  void GetGlClipToScreen(float m[16]) const;
  void GetGlScreenToClip(float m[16]) const;

  void GetGlWorldToCameraInverse(float m[16], bool includeModelTransform, const ON_Xform* extra) const;
  void GetGlWorldToClipInverse(float m[16], bool includeModelTransform, const ON_Xform* extra) const;
  void GetGlCameraToClipInverse(float m[16], const ON_Xform* extra) const;

///////////////////////////////////////////////////////////////////////////
//  Linear workflow support
  void SetLinearWorkflow(const CDisplayPipelineAttributes::LinearWorkflow& lw);
  const CDisplayPipelineAttributes::LinearWorkflow& LinearWorkflow() const;

///////////////////////////////////////////////////////////////////////////
// Specific OpenGL hardware information 
public:
  void UseHardwareModes(bool=true);

  unsigned int ExecutionLevel() const;
  // Description:
  //  returns the OpenGL rendering context that this engine is working with. May
  //  be NULL if the engine has not created the rendering context.
  HGLRC GetRenderingContext() const;
  
  int MaxTextureSize()   const;
  int MaxTextureCoords() const;
  int MaxTextureUnits() const;

  void SetTextureCoords(int nTex, int stride, const void*) const;
  int RenderPass() const;

  ON_DEPRECATED_MSG("Do not use")
  class CRhOpenGLRenderer*  GetFrameBufferObject();
  class CRhDisplayEnginePrivate_OGL* GetEngineGlPrivate();
protected:
  bool NotifyProjectionChanged() override;
  
  bool NotifyFrameSizeChanged(int  nWidth, int  nHeight) override;

  void NotifyClippingPlanesChanged() override;
protected:
  void                  Defaults();
  void                  Destroy();
  void                  Shutdown();
  
  bool                  InitOpenGL();
  bool                  EnableGLContext();
  bool                  DisableGLContext();
  
  // Set up the 'legacy' OpenGL model and projection matrices with values
  // matching the current camera
  void                  SetupGLMatrices(bool model, bool projection);
  
  bool                  SetupPixelFormat();
  
  void                  DisableTextures();
  
  void                  SetupTexture(UINT    nId,
                                     UINT    nTarget,
                                     ON_Texture&);

  void                  EnumMultiSampleModes(int);       
  
  void                  BlendMap(int  nTex, float  fBlend);                                     
  
  void                  SetupMaterial(int cullFaceMode, const CDisplayPipelineAttributes* da);
  void                  SetupMaterial(const CDisplayAttributeMaterial& material);

protected:
  void ValidateFBO();
  int GetClipPlanes(bool clippingCoordinates, ON_SimpleArray<ON_PlaneEquation>& planes) const;

private:
  friend class CRhEngine_GL11;
  friend class CRhEngine_GL21;
  friend class CRhEngine_GL33;
  friend class CRhEngine_GL40;
  friend class CRhOglEngineExtension;
  friend class CRhResourceManager_OGL;
  friend class CRhinoDisplayPipeline;
  friend class CRhDisplayEnginePrivate_OGL;

  //not virtual and kept private on purpose
  void DrawGradientMesh(const ON_Mesh& mesh, const class CRhinoDisplayBrush* fillBrush, CRhinoCacheHandle* cacheHandle);
  void DrawLines(int lineCount, const ON_Line* lines, float strokeWidth, int colorCount, const ON_ColorStop* colorStops,
    const ON_3dPoint& gradientPoint1, const ON_3dPoint& gradientPoint2, ON_GradientType gradientType,
    float repeatFactor);
  void DrawPointCloudHelper(const ON_PointCloud& cloud, ERhinoPointStyle style,
    float size, bool sizeInWorld, bool scaleSizeForDpi, ON_Color colorOverride,
    int indexCount, const unsigned int* indices, CRhinoCacheHandle* cache, int degradeCount);

protected:
  bool                  m_bTransformShadowMatrix;

  UINT                  m_nTextureUnit;

#if defined (RHINO_MAC_APPLICATION)
private:
  UINT                  m_nTextureFramebufferId = 0;
  UINT                  m_nColorTextureId = 0;
  UINT                  m_nDepthTextureId = 0;

  NSOpenGLPixelFormat*  m_OGL_nPixelFormat;
#endif
  
public:
  void FlushAll() override;

protected:
  void FlushMeshBuffer() override;
  void FlushCurveBuffer() override;
  void FlushPointBuffer() override;
private:
  unsigned int BackgroundBmpTextureId() const;
  unsigned int BackgroundBmpSerialNumber() const;
  void SetBackgroundBitmap(unsigned int textureId, unsigned int serialNumber);
  class CRhDisplayEnginePrivate_OGL* m_private_gl = nullptr;
  ON__UINT_PTR m_sdk_reserved = 0;

  class CRhVboMeshData* EnvSphereVbo(void);
};

/////////////////////////////////////////////////////////////////////////
// Texture caching and updating...
//
// Note: The use of these functions must be within the scope of a frame 
//       buffer update. For example: Inside any of the pipeline draw 
//       channels. Using them outside of this scope can/will have 
//       unpredictable results and behavior.

// Description:
//   Returns the handle to a cached texture the engine will use for the given name. 
//
// Parameters:
//   Name - [in] The name of the cached texture. This can be a name in any format, 
//          consisting of any alphanumeric characters. The idea is that you choose 
//          names that can easily be used for unique identification.
//
// Note: Do not use file names as your cached texture names, otherwise you risk the
//       chance of colliding with textures that have already been or may be loaded
//       by other materials used in the scene. There are other factors used by Rhino
//       when caching textures, so the chances are low for collisions...but there 
//       still exists a possibility that textures cached by the use of this method
//       can collide with textures cached by Rhino's material management that contain
//       the same file name(s)...so it's probably best to avoid it by not using file
//       names as your caching name.
//
// Note2: Holding onto the cached value is not recommended since Rhino can decide to
//        free up any OpenGL resources it needs to for any reason at any time.
//        For example: Loading a new file will cause Rhino to destroy all cached OpenGL
//        resources, including all of your cached textures. Which means any handles
//        previously returned by this method will no longer work, or may collide with
//        handles created during the loading of the new file. Therefore it is best to
//        just always call RhinoGetCachedTexture() before using any of the other texture
//        caching methods. 
//
// Returns: 
//    0   - if the engine was unable to locate or create a texture with the given name.
//    > 0 - Value of cached texture image. Note: The only thing you can do with this value
//          is use it as a parameter to the other texture caching methods. Do not assume
//          it can be used for anything else.
//
RHINO_SDK_FUNCTION
unsigned int RhinoGetCachedTexture(const ON_wString& Name);

// Description:
//   Updates the given cached texture's contents with the contents of the passed in Dib.
//   Note: If the resolution of the Dib is not the same as the cached texture's, then
//         a new cached texture will be created in its place, re-using the same cached
//         handle. For performance purposes, it is recommended that you try to manage
//         using matching resolutions when updating cached textures.
//
// Parameters:
//   hTexture - handle to a valid cached texture (obtained by calling GetCachedTexture())
//   Dib      - Image contents used to update the cached texture's contents.
//
// Returns:
//   True  - if/when the updating of the cached texture succeeds.
//   False - if/when something failed during the update process.
RHINO_SDK_FUNCTION
bool RhinoUpdateCachedTextureFromDib(unsigned int hTexture, const CRhinoDib& Dib);

// Description:
//   Updates the given cached texture's contents with the contents of the passed in file
//   name. Obviously the file name must represent one of the many image file formats 
//   currently supported by Rhino.
//
// Note: Calling this method can cause performance hits since it reads in files from disk.
//       Therefore it is important that you keep track of when a cached texture needs
//       updating, and only update it when necessary and not just blindly call the update
//       method for every frame update.
//
// Parameters:
//   hTexture - handle to a valid cached texture (obtained by calling GetCachedTexture())
//   Filename - Name of an existing and supported image file on disk.
//
// Returns:
//   True  - if/when the updating of the cached texture succeeds.
//   False - if/when something failed during the updating process or if the file could
//           not be opened or is an invalid/unsupported image file format.
RHINO_SDK_FUNCTION
bool RhinoUpdateCachedTextureFromFile(unsigned int hTexture, const ON_wString& Filename);

