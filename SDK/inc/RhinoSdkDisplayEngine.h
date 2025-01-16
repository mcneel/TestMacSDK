//
// Copyright (c) 1993-2017 Robert McNeel & Associates. All rights reserved.
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

/////////////////////////////////////////////////////////////////////////////
// Each display pipeline contains a display engine. The engine performs the
// actual API specific calls to draw (GDI, OpenGL, DirectX). This is the generic
// base class that performs common engine operations. Subclasses will draw
// using specific display technologies (CRhinoDisplayEngine_OGL, CRhinoDisplayEngine_GDI)
class RHINO_SDK_CLASS CRhinoDisplayEngine : public ON_Object
{
  ON_OBJECT_DECLARE(CRhinoDisplayEngine);
public:
  CRhinoDisplayEngine();
  virtual ~CRhinoDisplayEngine();

///////////////////////////////////////////////////////////////////////////
// Generic engine attributes...
public:
  // Description:
  //  Get the viewport that this engine draws the contents of
  const ON_Viewport& VP() const;
  
  //  Return the document that the pipeline uses to get geometry information from when drawing.
  const CRhinoDoc* GeometrySource() const;

  // Description:
  //  Is this engine drawing using hardware acceleration?
  // Returns:
  //  true if the engine is using hardware accelerated OpenGL or Direct3D
  virtual bool IsAccelerated() const;

  // Returns:
  //  The maximum number of lights supported for shading. OpenGL and Direct3D
  //  will support different numbers of lights based on the installed graphics card.
  virtual int MaxLightsSupported() const;

  // Returns:
  //  The maximum number of object mappings supported for shading. OpenGL and Direct3D
  //  will support different numbers of object mappings based on the installed graphics card.
  virtual int MaxObjectMappingsSupported() const;

  // Returns:
  //  The maximum number of decals supported for shading. OpenGL and Direct3D
  //  will support different numbers of decals based on the installed graphics card.
  virtual int MaxDecalsSupported() const;

  // Returns:
  //  The maximum number of procedural commands supported for shading. OpenGL and Direct3D
  //  will support different numbers of procedural commands based on the installed graphics card.
  int MaxProceduralCommandsSupported() const;

  // Returns:
  //  The maximum number of procedural transforms supported for shading. OpenGL and Direct3D
  //  will support different numbers of procedural transforms based on the installed graphics card.
  int MaxProceduralTransformsSupported() const;

  // Returns:
  //  The maximum number of procedurals supported for shading. OpenGL and Direct3D
  //  will support different numbers of procedurals based on the installed graphics card.
  int MaxProceduralsSupported() const;

  // Returns:
  //  The maximum number of user defined clipping planes supported. This does not
  //  include the standard view frustum clipping planes. OpenGL and Direct3D will
  //  support different numbers of clipping planes based on the installed graphics card.
  virtual int MaxClipPlanesSupported() const;

  // Returns:
  //  True if the display engine support drawing triangles with lighting effects.
  //  Basically this is false for GDI and true for OpenGL and DirectX
  virtual bool SupportsShading() const;

  // Returns:
  //  True if the underlying engine technology support multisampling modes. This
  //  is used for anti-aliasing purposes and depends on the installed graphics card.
  virtual bool SupportsMultiSampling() const;
  
  // Description:
  //  If this engine supports multisampling, this function retrieves a list of the
  //  multisampling modes available
  // Parameters:
  //  modes: [out] list of multi-sample modes available
  // Note:
  //  the values returned are engine specific, and only the underlying 
  //  engine knows how to really interpret their meaning. The routine
  //  SetMultiSampleMode( N ) is used in conjunction with these return
  //  values, and again, only the engine knows what they mean and how
  //  they're interpreted when actually setting up multi-sampling.
  virtual void GetMultiSampleModes(ON_SimpleArray<int>& modes) const;

  // Description:
  //  Set the current multisampling mode for this engine
  // Parameters:
  //  mode: [in] one of the modes retrieved from GetMultiSampleModes function
  // Returns:
  //  true on success. If the engine does not support multisampling the return value
  //  will always be false
  virtual bool SetMultiSampleMode(int mode);
  
  // Returns:
  //  The current multi-sample mode for this engine or -1 if multisampling is not supported
  virtual int GetMultiSampleMode() const;
    
  // Enables / disables multisampling
  virtual void EnableMultiSample(bool bEnable=true);
  
///////////////////////////////////////////////////////////////////////////
// Engine initialization and cleanup operations...
public:
  // Description:
  //  At the very beginning of drawing each frame the pipeline will call StartEngine
  //  to allow the engine to perform any required initializaion or context enabling
  //
  //  All subclasses that override this function should call this base class version
  //  of the function.
  // Returns:
  //  True if the engine was successfully "started". False if something went wrong.
  //  If the function returns false, the pipeline will not be able to draw a frame.
  virtual bool StartEngine();
  
  // Description:
  //  Near the end of drawing a frame the display pipeline will call StopEngine to tell
  //  the engine that it should flush any remaining buffers and perform and other task
  //  that must occur just prior to displaying the frame.
  // Returns:
  //  true if the engine was successfully stopped. This is pretty much always true
  virtual bool StopEngine();
  
  // Description:
  //  Should reset the engine back to some initial state...but at the same time
  //  try to maintain current state (ie. opened or closed).
  //  Typically this means saving the current state, shutting down the engine and then
  //  restarting the engine with the saved state
  //
  //  This is typically only used for things like changing the multisampling modes for an engine
  // Returns:
  //  true if the reset operation was successful
  virtual bool ResetEngine();

  // Description:
  //  Should completely shutdown and destroy everything associated with this engine...
  //  This includes any runtime buffers, dibs, or rendering contexts
  virtual void ShutdownEngine();


  virtual bool InStereoMode() const;

  virtual double GetStereoSeparation() const;

  virtual double GetStereoParallax() const;
  
  // Description:
  //  0 = left, 1 = right...
  virtual int ActiveStereoProjection() const;

  virtual bool SetStereoMode(bool bStereo);

  virtual void SetStereoSeparation(float fSep);

  virtual void SetStereoParallax(float);

///////////////////////////////////////////////////////////////////////////
// Model transform operations...
public:
  // Returns:
  //  The model transformation that is currently being applied to geometry
  //  for drawing.
  const ON_Xform& ModelTransform() const;

  // Returns:
  //  The instance definition transformation. The same as ModelTransform() excluding
  //  the dynamic transform.
  const ON_Xform& InstanceDefinitionTransform() const;

  // Description:
  //  Set a model transformation that will be applied to all future drawing
  //  operations. All line and point buffers are always flushed in ths function
  //  before the transformation is set.
  // Parameters:
  //  mt: [in] the transform that the engine's model transform will be set to.
  void SetModelTransform(const ON_Xform& mt);

  // Description:
  //  Set the instance definition transformation. This is the same as
  //  the model transformation excluding the dynamic transform.
  // Parameters:
  //  idt: [in] the instance definition transform.
  void SetInstanceDefinitionTransform(const ON_Xform& idt);
  
  bool ModelTransformIsIdentity() const;

  // Description:
  //  Save the current model transform on a stack and set a new model transform
  // Remark:
  //    This function does not multiply mt by the existing model transform. If you
  //    want to compound transformations multiply ModelTransform() by your xform
  //    before calling this function.
  // Parameters:
  //  mt: [in] the transform that the engine's model transform will be set to.
  void PushModelTransform(const ON_Xform& mt);

  // Description:
  //  Restore a model transform that was pushed on to a stack of transforms using
  //  the PushModelTransform function
  void PopModelTransform();

  const ON_Xform& WorldToCamera(bool includeModelXform = true) const;
  const ON_Xform& WorldToCameraInverse(bool includeModelXform = true) const;

  //world to screen transform = Viewport(world to clip)*m_ModelTransform
  const ON_Xform& WorldToClip(bool includeModelXform = true) const;
  const ON_Xform& WorldToClipInverse(bool includeModelXform = true) const;

  //world to screen transform = Viewport(world to screen)*m_ModelTransform
  const ON_Xform& WorldToScreen(bool includeModelXform = true) const;
  const ON_Xform& WorldToScreenInverse(bool includeModelXform = true) const;

  const ON_Xform& CameraToClip() const;
  const ON_Xform& CameraToClipInverse() const;

  const ON_Xform& ScreenToClip() const;
  const ON_Xform& ScreenToClipInverse() const;
///////////////////////////////////////////////////////////////////////////
// Standard engine routines for drawing high-level (non-primitive) items...
//
// Note: These routines eventually end up calling the low-level primitive
//       drawing routines, which, for the most part, are usually overridden
//       by custom engines. Therefore, it's usually not necessary to 
//       override any of these routines, unless you want to completely 
//       change the way they look or are drawn.
//
///////////////////////////////////////////////////////////////////////////
public:
  
  // Description:
  //  Draws world axes icon in lower left corner of viewport.
  //
  // Parameters:
  //  xaxis_color - [in] X axis color
  //  yaxis_color - [in] Y axis color
  //  zaxis_color - [in] Z axis color
  //  axes_length - [in] length in pixels (25 works well)
  //  label_offset- [in] distance in pixels labels are placed from axes
  //  letter_size - [in] label font size (in points, 14 works well)
  virtual void DrawWorldAxes( const ON_Color& xaxis_color,
                              const ON_Color& yaxis_color,
                              const ON_Color& zaxis_color,
                              int axes_length,
                              int label_offset,
                              int letter_size
                              );

  // Description:
  //  Draws construction plane lines and axes for the given cplane object.
  //
  // Parameters:
  //  bShowGrid: [in] true to draw cplane grid
  //  bShowGridAxes: [in] true to draw cplane grid axes
  virtual void DrawConstructionPlane( const ON_3dmConstructionPlane& plane,
                                      bool bShowGrid,
                                      bool bShowGridAxes,
                                      bool bShowZAxis,
                                      const ON_Color& grid_thin_color,
                                      const ON_Color& grid_thick_color,
                                      const ON_Color& grid_xaxis_color,
                                      const ON_Color& grid_yaxis_color,
                                      const ON_Color& grid_zaxis_color,
                                      int grid_thick_line_size,
                                      int grid_axis_line_size,
                                      int axes_percentage=100,
                                      bool bForceBackground=false
                                      );

  // Description:
  //  Draws wallpaper image into the frame buffer.
  virtual void DrawWallpaper( const class CRhWallpaper& wallpaper );

  // Description:
  //  Draws background bitmap image into the frame buffer.
  virtual void DrawTraceImage( const class CRhTraceImage& traceimage );

  // Description:
  //  Draws background environment into the frame buffer.
  virtual void DrawEnvironment();

  // Description:
  //  Draws a ground plane into the frame buffer.
  virtual void DrawGroundPlane( const class CDisplayPipelineAttributes& dpa, const class CRhDisplayGroundPlane& groundplane );

  // Description:
  //  Draw a mesh with a given set of display attributes
  // Parameters:
  //  mesh: [in] Mesh to draw
  //  da: [in] Display attributes to use for drawing. This is required. Passing
  //      NULL will not draw anything
  void DrawMesh(const ON_Mesh& mesh, bool wires, bool shaded, const class CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle);
  virtual void DrawMeshes(const ON_Mesh** meshes, int count, bool wires, bool shaded, const class CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle);

  // Description:
  //  Draw a single face from a mesh
  // Parameters:
  //  mesh: [in] the mesh that contains the face to draw
  //  faceIndex: [in] the index into the mesh's m_F array for the face
  //  da: [in] Display attributes to use for drawing. This is required. Passing
  //      NULL will not draw anything
  void DrawMeshFace( const ON_Mesh& mesh, int faceIndex, const class CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle);

  // Description:
  //  Draw a set of faces from a mesh
  // Parameters:
  //  mesh: [in] the mesh that contains the faces to draw
  //  faces: [in] array of indices into the mesh's m_F array for the faces
  //  faceCount: [in] number of elements in the faces array
  //  da: [in] Display attributes to use for drawing. This is required. Passing
  //      NULL will not draw anything
  virtual void DrawMeshFaces(const ON_Mesh& mesh, const int* faces, int faceCount, const class CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle);
  void DrawMeshFaces(const ON_Mesh& mesh, const ON_SimpleArray<int>& faces, const class CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle);

  virtual void DrawHatch(const ON_Hatch& hatch, const ON_HatchPattern* pattern, float strokeWidth, ON_Color hatchColor, ON_Color boundaryColor, CRhinoCacheHandle* cacheHandle);
  virtual void DrawHatch(const ON_Hatch& hatch, const class CRhinoDisplayBrush* hatchBrush, const class CRhinoDisplayPen* boundaryPen, CRhinoCacheHandle* cacheHandle);
  void DrawHatch(const ON_Hatch& hatch, const class CRhinoDisplayBrush* fillBrush, const class CRhinoDisplayBrush* hatchBrush, const class CRhinoDisplayPen* boundaryPen, CRhinoCacheHandle* cacheHandle);

  virtual void DrawCurve(const ON_Curve& curve, const class CRhinoDisplayPen& pen, CRhinoCacheHandle* cacheHandle);

  virtual void DrawSubD(const class CRhinoSubDDisplay& subd_display, const class CRhinoSubDDisplayAttributes& subd_display_attributes );
  // Do not call - placeholder virtual function in order to improve SubD display without breaking SDK.
  virtual void DrawSubD(const class ON_SubD& subd, const class CRhSubDDisplayAttributes* attrs, CRhinoCacheHandle* cacheHandle);
  // Do not call - placeholder virtual function in order to improve SubD display without breaking SDK.
  virtual void DrawSubDComponents(const class ON_SubD& subd, const class ON_SubDComponentPtr* components, size_t componentCount, const class CRhSubDDisplayAttributes* attrs, CRhinoCacheHandle* cacheHandle);
  void DrawSubDWires(const CRhinoSubDDisplay& subd_display,
                     const class CRhinoDisplayPen* boundaryPen,
                     const class CRhinoDisplayPen* smoothInteriorPen,
                     const class CRhinoDisplayPen* creaseInteriorPen,
                     const class CRhinoDisplayPen* nonmanifoldPen);
  
  virtual void DrawBrepWires(const ON_Brep* brep, const class CDisplayPipelineAttributes* da, int wireDensity, bool edgeAnalysis, CRhinoCacheHandle* cacheHandle);
  void DrawBrepWires(const ON_Brep* brep, const class CDisplayPipelineAttributes* da, int wireDensity, bool edgeAnalysis, const class CRhDisplayPenPrivate* pen, CRhinoCacheHandle* cacheHandle);

  virtual void DrawShadedBrep(const ON_Brep* brep, const class CDisplayPipelineAttributes* da, CRhinoCacheHandle* cacheHandle);
  virtual void DrawExtrusionWires(const ON_Extrusion& extrusion, const class CDisplayPipelineAttributes* da, int wireDensity, bool edgeAnalysis, CRhinoCacheHandle* cacheHandle);
  void DrawExtrusionWires(const ON_Extrusion& extrusion, const class CDisplayPipelineAttributes* da, int wireDensity, bool edgeAnalysis, const class CRhDisplayPenPrivate* pen, CRhinoCacheHandle* cacheHandle);
  // Description:
  //  Add a light to the list of lights that the engine should use when displaying
  //  shaded geometry.
  // Parameters:
  //  light: [in] the light information the engine should use to set up an graphics
  //         technology specific light
  virtual void AddLight(const ON_Light& light);

  // Returns:
  //  Number of lights this engine is currently using for drawing shaded geometry
  virtual int LightCount() const;

  virtual void ClearLights();

  virtual const ON_Light* GetLight(int index) const;

  // Description:
  //  Remove all the user defined clipping planes that may have been added through
  //  the AddClipPlane function
  virtual void ClearClipPlanes();

  bool UseClipPlanes() const;

  virtual int SetClippingPlanes(const ON_SimpleArray<ON_ClippingPlaneInfo>& clipplanes);
  const ON_SimpleArray<ON_ClippingPlaneInfo>& ClippingPlanes() const;

  // Returns:
  //  Number of user defined clipping planes currently set up for this engine.
  //  This does not include the standard view frustum clipping planes
  int ClipPlaneCount() const;

  ON_SimpleArray<ON_PlaneEquation> GetEnabledClipPlanes(const ON_Xform* xf) const;

///////////////////////////////////////////////////////////////////////////
// Generic engine routines for drawing low-level primitives...
//
// Note: These routines are usually (and probably should be) always
//       overridden by custom engines since their default implementations
//       do nothing. By not implementing one, some, or all of these routines,
//       you are in a sense disabling the drawing for those respective features
//       or items and all other items that use them. 
//       
//       For example: If you do not override "DrawPoints()" in your
//                    engine, and don't implement some form of point rendering, 
//                    then points and all routines that use/need points
//                    will no longer show up in the final frame buffer.
//
///////////////////////////////////////////////////////////////////////////
public:
  // Description:
  //  Fill the frame buffer with a single color
  //  This function also clears the depth buffer for engines that support
  //  depth buffered drawing.
  // Parameters:
  //  color: [in] the color to fill the frame buffer with
  void ClearFrameBuffer(const ON_Color& color);
  
  // Description:
  //  Fill the frame buffer with a vertical gradient between two colors
  //  This function also clears the depth buffer for engines that support
  //  depth buffered drawing.
  // Parameters:
  //  colorTop: [in] the color on the top line of the frame buffer
  //  colorBottom: [in] the color on the bottom line of the frame buffer
  void ClearFrameBuffer(const ON_Color& colorTop, const ON_Color& colorBottom);
  
  // Description:
  //  Fill the frame buffer with a gradient between four corner colors
  //  This function also clears the depth buffer for engines that support
  //  depth buffered drawing.
  // Parameters:
  //  colorTopLeft: [in] the color in the top-left corner of the frame buffer
  //  colorBottomLeft: [in] the color in the bottom-left corner of the frame buffer
  //  colorTopRight: [in] the color in the top-right corner of the frame buffer
  //  colorBottomRight: [in] the color in the bottom-right corner of the frame buffer
  virtual void ClearFrameBuffer( const ON_Color& colorTopLeft,  const ON_Color& colorBottomLeft,
                                 const ON_Color& colorTopRight, const ON_Color& colorBottomRight) = 0;

  // Description:
  //  File the frame buffer with a bitmap image
  //  This function also clears the depth buffer for engines that support
  //  depth buffered drawing.
  // Parameters:
  //  dib: [in] the image to draw into the frame buffer
  virtual void ClearFrameBuffer(const CRhinoDib* dib);

  // Description:
  //  Clear the depth buffer for this engine. If the engine does not support depth buffered
  //  drawing, then nothing is done.
  virtual void ClearDepthBuffer();

  // Description:
  //  Draw a bezier curve
  // Parameters:
  //  dim: [in]
  //  bIsRational: [in]
  //  order: [in]
  //  cv_stride: [in] number of doubles between CVs (>=4)
  //  cv4d: [in] 4d homogeneous world coordinate CVs
  //  attrs: [in] attributes for the curve
  virtual void DrawBezier( int            dim,
                           BOOL32           bIsRational,
                           int            order,
                           int            cv_stride,
                           const double*  cv4d,
                           const CRhCurveAttributes& attrs);
  
  // Description:
  //  Draws a point at specified location, using specified radius, style
  //  and color.
  //
  // Parameters:
  //  point: [in] the location to draw the point
  //  radius: [in] point size in screen pixels
  //  style: [in] style used for how the point should be drawn
  //  color: [in] color used to draw the point
  virtual void DrawPoints(int count, const ON_3dPoint* points, ERhinoPointStyle style,
    const ON_Color& strokeColor, const ON_Color& fillColor,
    float diameter, float strokeWidth, float innerDiameter,
    float rotationRadians, bool diameterInPixels, bool autoScaleDpi, CRhinoCacheHandle* cache);

  virtual void DrawPoints(int count, const ON_2dPoint* screenPoints, ERhinoPointStyle style,
    const ON_Color& strokeColor, const ON_Color& fillColor,
    float diameterPixels, float strokeWidthPixels, float innerDiameterPixels,
    float rotationRadians, bool autoScaleDpi, CRhinoCacheHandle* cache);

  virtual void DrawPoints(int count, const class CRhPointData* points, CRhinoCacheHandle* cache) = 0;
  void DrawPoints(int count, const class CRhPointData* points, CRhinoCacheHandle* cache, const ON_Xform* ffoTransform, bool precomputedTransform);
  void DrawPoints(int count, const class CRhPointData* points, const class CRhPointData* forcedOverride,
    CRhinoCacheHandle* cache, const ON_Xform* ffoTransform, bool precomputedTransform);

  virtual void DrawPolylines(const class CRhinoClippingVertexCollection* lists, int count) = 0;

  // Description:
  //  Draws a line at specified locations, using specified thickness,
  //  color and pattern.
  // Parameters:
  //  from: [in] start point of line
  //  to: [in] end point of line
  //  color: [in] color to draw line
  //  thickness: [in] thickness in screen pixels
  //  pattern: [in] bit pattern to use for drawing. All bits set to 1 means draw a solid line
  virtual void DrawLine(const ON_3dPoint& from, const ON_3dPoint& to, const ON_Color& color, int thickness, UINT pattern=0xFFFFFFFF);
  
  // Description:
  //  Draws a line at specified locations, using specified thickness and color.
  //  This version of line drawing will draw the segments of the line that
  //  extend beyond the near and far planes of the view frustum with depths on
  //  those planes
  // Parameters:
  //  from: [in] start point of line
  //  to: [in] end point of line
  //  color: [in] color to draw line
  //  thickness: [in] thickness in screen pixels
  void DrawLineNoClip(const ON_3dPoint& from, const ON_3dPoint& to, const ON_Color& color, float thickness);

  void DrawLinesNoClip(int count, const ON_Line* lines, const ON_Color& color, float thickness);

  // Description:
  //  Draw a set of lines
  // Parameters:
  //  ld: [in] the line locations and attributes to draw
  virtual void DrawLines(const class CLinesDescriptor& ld);

  // Description:
  //  Draw a polyline using information stored in a CLinesDescriptor.
  //  Note - If not implemented by deriving engine class, this routine will
  //         just call "DrawLine" method in a loop
  // Parameters:
  //  ld: [in] the polyline locations and attributes to draw
  virtual void DrawPolylines(const class CLinesDescriptor& ld);
  

  // Description:
  //  Draw polyline defined in screen coordinates
  //  This is the low level 2d drawing routine used for drawing lines, rectangles,...
  virtual void Draw2dLines(const ON_2fPoint* points, int count,
                           bool asPolyline, // if false, each point pair is treated as independent lines
                           ON_Color strokeColor,
                           float strokeWidth,
                           RhinoDashStyle style,
                           bool xormode) = 0;

  virtual void DrawRoundedRectangle( ON_2fPoint screenCenter,
                                     float pixelWidth,
                                     float pixelHeight,
                                     float cornerRadius,
                                     ON_Color strokeColor,
                                     float strokeWidth,
                                     ON_Color fillColor
                                     ) = 0;

  virtual void DrawDot( const ON_TextDot& dot,
                        bool locationInWorld,
                        ON_Color fillColor,
                        ON_Color textColor,
                        ON_Color borderColor,
                        class CRhinoCacheHandle* cache) = 0;

  virtual void DrawPointCloud(const ON_PointCloud& cloud, ERhinoPointStyle style,
                              float pixelSize, bool scaleSizeForDpi, ON_Color colorOverride,
                              int indexCount, const unsigned int* indices,
                              CRhinoCacheHandle* cache, int degradeCount);

  virtual void DrawString(const wchar_t* text, const ON_Xform& xf, const ON_Font* font, const ON_Color& color,
    ON::TextHorizontalAlignment horizontalAlignment, ON::TextVerticalAlignment verticalAlignment,
    CRhinoCacheHandle* cache) = 0;

  // Description:
  //  Draws text with specified color, font and height
  // Parameters:
  //  text: [in] string to draw
  //  color: [in] text color
  //  point: [in] definition point, either lower-left or middle based on value in middle
  //  middle: [in] TRUE: middle justified, FALSE: lower-left justified
  //  fontface: [in] face name to use for font selection
  virtual void DrawString( const wchar_t* text,
                           const ON_Color& textColor,
                           const ON_2dPoint& point,
                           bool middle,
                           float fontHeightPixels,
                           const ON_Font* font
                           ) = 0;
  
  virtual void DrawString(const ON_TextContent& text, const ON_Xform& textXform, ON_Color color, CRhinoCacheHandle* cache);
  virtual void DrawTextRun(const ON_TextRun& run, const ON_Font* font, const ON_Xform& xform, ON_Color color, CRhinoCacheHandle* cache);

  // Description:
  //  Draw a polygon using a set of points for the polygon border
  // Parameters:
  //  points: [in] array of points in world coordinates to use for polygon border
  //  count: [in] number of elements in points array
  //  color: [in] draw color
  //  bFilled: [in] true = draw the polygon as a filled shape
  //                false = just draw the polygon border
  virtual void DrawPolygon( const ON_3dPoint* points, int count, ON_Color color, bool bFilled );          

  // Description:
  //  Draw a bitmap
  // Parameters:
  //  dib: [in] the bitmap to draw
  //  nX: [in] screen coordinate left location to draw top-left corner of dib
  //  nY: [in] screen coordinate top location to draw top-left corner of dib
  //  mask: [in] if set to a color, the dib is drawn using a color mask for transparency
  virtual void DrawBitmap(const CRhinoDib& dib, int nX, int nY);
  
  // Description:
  //  Flushes any and all buffers used by the engine.
  virtual void FlushAll();

  void DrawTechnicalWires(const CRhinoObject& rhinoObject, const ON_Color& color, float borderThickness, float silhouetteThickness, float creaseThickness);
protected:
  virtual void FlushMeshBuffer();
  virtual void FlushCurveBuffer();
  virtual void FlushPointBuffer();
  ///////////////////////////////////////////////////////////////////////////
// "State" operations & attributes...
public:
  virtual void EnableMeshBuffering(bool on);
  virtual bool MeshBufferingEnabled() const;

  // Description:
  //  Turn of/off depth testing for engines that support depth buffers
  virtual void EnableDepthTesting(bool bEnable=true);

  // Description
  //  Turn on/off writing of information to the depth buffer for engines
  //  that support depth buffers
  virtual void EnableDepthWriting(bool bEnable=true);
  
  // Description:
  //  Turn on/off writing of information to the color buffer
  virtual void EnableColorWriting(bool bEnable=true);

  // Description:
  //  Save the current state of depth testing on a stack and turn on/off depth testing
  virtual void PushDepthTesting(bool bEnable);

  // Description:
  //  Restore state of depth testing that was saved by calling PushDepthTesting
  // Returns:
  //  previous state of depth testing
  virtual void PopDepthTesting();
  
  // Description:
  //  Save the current state of depth writing on a stack and turn on/off depth writing
  virtual void PushDepthWriting(bool bEnable);

  // Description:
  //  Restore state of depth writing that was saved by calling PushDepthWriting
  // Returns:
  //  previous state of depth writing
  virtual void PopDepthWriting();
  
  // Description:
  //  Save the current state of color writing on a stack and turn on/off color writing
  virtual void PushColorWriting(bool bEnable);

  // Description:
  //  Restore state of color writing that was saved by calling PushColorWriting
  // Returns:
  //  previous state of color writing
  virtual bool PopColorWriting();
  
  virtual ECullFaceMode SetCullFaceMode(ECullFaceMode mode);
  ECullFaceMode CullFaceMode() const;

  // Description:
  //  Sets the current depth-testing mode used when drawing curves and lines. This basically
  //  determines whether lines are either always drawn in front, in back, or pays attention
  //  to the current depth value in the depth buffer.
  // Parameters:
  //  nDepth: [in]  0 = Perform a depth-buffer test draw - this the *normal* mode of operation
  //               -1 = Always draw in back of everything else (i.e. on the far clipping plane)
  //                1 = Always draw in front of everything else (i.e. on the near clipping plane)
  // Note: All parameter values are currently clamped to [-1 .. 1]
  // Returns:
  //   int : the previously used depth mode setting.
  virtual int SetDepthMode(int nDepth);

  virtual int ZBiasLevel() const;
  // Description:
  //  Sets a bias to apply to drawn geometry.
  //  Typical values are
  //     -1 for pushing geometry further away
  //      0 for no biasing
  //     +1 for pulling geometry a little closer
  // Parameters:
  //  zbias: [in] new bias level to apply
  // Returns:
  //  previous zbias mode value
  virtual int SetZBiasLevel(int zbias);

  // Returns:
  //  Number of bits per pixel for the frame buffer color values
  virtual int FrameBufferBitDepth();

  // Description:
  //  Gets the number of bits used to define each color channel for an individual pixel
  // Parameters:
  //  red_bits: [out] number of bits dedicated to describing red level in pixel
  //  green_bits: [out] number of bits dedicated to describing green level in pixel
  //  blue_bits: [out] number of bits dedicated to describing blue level in pixel
  //  alpha_bits: [out] number of bits dedicated to describing alpha level in pixel
  virtual void FrameBufferChannelDepth(int& red_bits, int& green_bits, int& blue_bits, int& alpha_bits);

  // Returns:
  //  Number of bits per pixel used to describe the depth value
  virtual int DepthBufferBitDepth();

  // Description:
  //  Enable/Disable user defined clipping planes
  // Parameters:
  //  plane: [in] index for plane to enable/disable. If -1, all planes
  //              will be enabled or disabled
  virtual bool EnableClipPlanes(bool bEnable, int plane = -1);

  // Description:
  //  Remove all cached texture information that may be stored locally or on the graphics card
  virtual void ClearTextureCache();

  // Description:
  //  Set the camera's frustum that this engine uses to define it's view projection information
  // Note:
  //  This information is copied into the engines local camera settings, so making any changes
  //  to the passed in viewport object after calling this routine will have no effect. You must
  //  always use SetCamera( ...) to change and/or set the engine's frustum.
  void SetCamera(const ON_Viewport&);

  ON_3dPoint  FrustumCenter() const;
  void        GetNearPlane(ON_Plane&) const;
  void        GetFarPlane(ON_Plane&) const;
  void        GetNearFarPlane(ON_Plane& N, ON_Plane& F) const;

  // Description:
  //  Clip region defines an area on the framebuffer where drawing can occur
  //  Note - This function has not been implemented for V4 and serves as a virtual function
  //         placeholder for future versions of Rhino
  // Parameters:
  //  boundary: [in] a closed 2d curve used to define area where drawing can occur
  // Returns:
  //  id for an engine defined clip region. -1 if a clip region could not be created
  virtual int CreateClipRegion( const ON_Curve& boundary );

  // Description:
  //  Clip region defines an area on the framebuffer where drawing can occur
  //  Note - This function has not been implemented for V4 and serves as a virtual function
  //         placeholder for future versions of Rhino
  // Parameters:
  //  region_id: [in] id returned from CreateClipRegion function
  virtual void DestroyClipRegion( int region_id );

  // Description:
  //  Clip region defines an area on the framebuffer where drawing can occur
  //  Note - This function has not been implemented for V4 and serves as a virtual function
  //         placeholder for future versions of Rhino
  // Parameters:
  //  region_id: [in] id returned from CreateClipRegion function
  virtual int SetClipRegion( int region_id );

  // Description:
  //  Get the current curve attributes that are being used by the engine
  // Parameters:
  //  attributes: [out] the current curve attributes
  void GetCurveAttributes(CRhCurveAttributes& attributes) const;

  // Description:
  //  Set the current curve attributes that are being used by the engine
  // Parameters:
  //  attributes: [in] the engine's current curve attributes are set to these values
  void SetCurveAttributes(const CRhCurveAttributes& attributes);
    
  double ScreenLength(double world_length) const;

  void SetDpiScale(float scale);
  float DpiScale() const;
  float WireDpiScale() const;

///////////////////////////////////////////////////////////////////////////
// Caching operations...
public:
  virtual void InvalidateCapturedFrame();
  virtual bool CaptureFrame(const ON_Viewport& viewport);
  virtual const class CRhFrameCapture* GetCapturedFrame() const;
  virtual bool DrawCapturedFrame(const ON_Viewport& destination);
  
public: 
  // Description:
  //  Convert a point in 3d world space coordinates to 2d screen space coordinates
  // Parameters:
  //  point: [in] the location in world coordinates to convert
  // Returns:
  //  the screen space coordinate where the world coordinate would be drawn
  ON_2dPoint WorldToScreenPoint(const ON_3dPoint& point) const;

  // Description:
  //  Convert a point in 4d clipping coordinates to 2d screen space coordinates
  // Parameters:
  //  point: [in] the location in clipping coordinates to convert
  // Returns:
  //  the screen space coordinate where the clipping coordinate would be drawn
  ON_2dPoint ClippingToScreenPoint(const ON_4dPoint& point) const;

  // Description:
  //  Convert a point in 3d world space coordinates to 4d clipping coordinates
  // Parameters:
  //  point: [in] the location in world coordinates to convert
  // Returns:
  //  the equivalent clipping coordinates to the world coordinate with the current viewport projection
  ON_4dPoint WorldToClippingPoint(const ON_3dPoint& point) const;

  // Returns:
  //  true if the supplied bounding box is entirely inside the clip box.x
  bool IsEntireBoxVisible( const ON_BoundingBox& bbox ) const;

///////////////////////////////////////////////////////////////////////////
// Notifier routines called by the pipeline framework...
public:
  
  /////////////////////////////////////////////////////////////////////////
  // Description:
  //  This routine is normally called by the pipeline framework because the
  //  current "display's" viewport projection has changed...This means that
  //  all of the commonly used engine attributes need to be updated.
  //
  //  Note: this routine should never be called haphazardly since it can be
  //        somewhat expensive performance-wise. However, if you or your
  //        display pipeline ever change the projection other than by through
  //        the standard pipeline mechanisms, then you must ensure that this
  //        routine gets called AFTER the changes, or update these attributes
  //        manually on an as-needed basis. Also, if your engine overrides 
  //        this routine, then it is best that your implementation also call
  //        the derived class's implementation.
  // Returns:
  //  true on success
  virtual bool NotifyProjectionChanged();
  
  // Description:
  //  Save the current viewport on a stack and set the current viewport to a given viewport
  // Parameters:
  //  viewport: [in] the viewport to make the current viewport for the engine.
  void PushProjection(const ON_Viewport& viewport);

  // Description:
  //  Restore the viewport that was pushed on to a stack with a call to PushProjection.
  void PopProjection();

  // Description:
  //  Tell the engine that the size of the rectangle that it draws to has changed.
  virtual bool NotifyFrameSizeChanged(int nWidth, int nHeight);
  
  virtual void NotifyClippingPlanesChanged();
public:
  // Returns:
  //  true if this engine can use a depth buffer (z-buffer)
  virtual bool HasDepthBuffer();
  
  // Description:
  //  Copy the depth buffer information to a bitmap
  // Parameters:
  //  dib: [out] the bitmap that is created with depth buffer information
  //  bConvertToImage: [in] if true, the depth information is converted into a grey scale image
  //                        if false, each byte in the dib contains a depth value
  //  bInvertDepthValues: [in] if true, the values in the dib are flipped where a byte used to be 0 will
  //                           now be 255 and a byte that used to be 255 will now be 0
  //  pBackground: [in] If not NULL, this image will be place in the background of dib where no depth
  //                    information was written to
  // Returns:
  //  true if successful
  virtual bool CopyDepthBufferTo( CRhinoDib& dib,
                                  bool bConvertToImage=false,
                                  bool bInvertDepthValues=false,
                                  CRhinoDib* pBackground=nullptr);
  
  // Description:
  //  Copy depth information into color values in the back buffer
  // Returns:
  //  true if successful
  virtual bool CopyDepthToBackBuffer();

  // Description:
  //  copy the color information in the back buffer to a bitmap
  // Parameters:
  //  dib: [out] the bitmap that is created and holds the back buffer's values
  // Returns:
  //  true if successful
  virtual bool GetBackBuffer(CRhinoDib& dib);
  
  // Description:
  //  Read depth values into an array
  // Parameters:
  //  buffer: [out] array large enough to hold width * height elements
  //                This array will hold the depth values
  //  left: [in] defines rectangle on viewport to get information from
  //  top: [in] defines rectangle on viewport to get information from
  //  width: [in] defines rectangle on viewport to get information from
  //  height: [in] defines rectangle on viewport to get information from
  // Returns:
  //  true if successful
  virtual bool GetDepthBuffer(float* buffer, int left, int top, int width, int height);

  // Description:
  //  Copy color information in back buffer to a device context
  // Parameters:
  //  pDC: [in] device context that will get color information blitted into it
  // Returns:
  //  true if successful
  virtual bool CopyBackBufferTo(HDC hDC);
  
  // Description:
  //  Copy color information from a device context into the back buffer
  // Parameters:
  //  pDC: [in] device context that contains the color information
  // Returns:
  //  true if successful
  virtual bool CopyBackBufferFrom(HDC pDC);
  
  // Description:
  //  Copy color information in back buffer to a bitmap
  // Parameters:
  //  dib: [out] bitmap that is created with color information from back buffer
  // Returns:
  //  true if successful
  virtual bool CopyBackBufferTo(CRhinoDib& dib);
  
  // Description:
  //  Copy color information from a bitmap into the back buffer
  // Parameters:
  //  dib: [in] bitmap that contains the color information
  // Returns:
  //  true if successful
  virtual bool CopyBackBufferFrom(const CRhinoDib& dib);

  // Description:
  //  Display the contents of the back buffer on to the front buffer
  // Parameters:
  //  pDC: [in] device context to draw the back buffer colors to
  // Returns:
  //  true if successful
  virtual bool ShowBackBuffer(HDC hDC); 

  // Description:
  //
  virtual bool GetOffscreenBufferInfo(HDC hDC, int& nColorDepth, int& nZDepth);

  // Returns:
  //  true if this engine successfully saved back buffer color information to a location
  //  that can be retrieved at a later time
  virtual bool SaveBackBuffer();

  // Returns:
  //  true if a saved back buffer was successfully restored to the back buffer
  virtual bool RestoreBackBuffer();

  // Returns:
  //  true if this engine can save depth buffer information
  virtual bool CanSaveDepthBuffer();

  // Returns:
  //  true is this engine successfully saved the depth buffer information to a location
  //  that can be retrieved at a later time
  virtual bool SaveDepthBuffer();

  // Returns:
  //  true if a saved depth buffer was successfully restored to the engine depth buffer
  virtual bool RestoreDepthBuffer();

protected:
  // Description:
  //  Set the window that this engine draws on
  virtual void SetRenderTarget(HWND hWnd);
  virtual void SetRenderTarget(HDC pDC, bool bShutDown);

  void ComputeZBiasValues(int bias, double& a, double& b, double& c, double& d, bool& use_d) const;

public:
  // From CRhEngineExtension class. These functions should change
  // as we clean up V6
  virtual class CRhGLSceneManager* GetSceneManager();
  class CRhGLSceneManager* GetSceneManager(const CRhinoDoc* doc);
  virtual class CRhGLSceneManager*  GetSceneManager(unsigned int docSerialNumber);

  void DisableCoplanarClippingPlanes(const CRhinoObject* pRhinoObject, ON_SimpleArray<int>& planes_disabled);
  void DisableCoplanarClippingPlanes(const ON_Plane& plane, ON_SimpleArray<int>& planes_disabled);

  // Used to track the contents of the OpenGL model view matrix
  // We should always use this matrix to compute new model transform
  // matrices on the CPU in double precision because the OpenGL
  // matrix stack is in floats and precision goes down the toilet
  // when you are far from the origin and call glmultmatrix.
  // We should always glloadmatrix with our double precision calculated
  // xform.
  void SetModelViewMatrix(const ON_Xform& xf);
  void PushModelViewMatrix(const ON_Xform& xf);
  void PushModelViewMatrix();
  void PopModelViewMatrix();
  ON_Xform GetModelViewMatrix() const;

  bool UseMaterials();
  void EnableMaterials(bool on);

  virtual bool DrawLines( ON_Color color, int line_count, float thickness, const float* vertices, const ON_Xform* model_xform );
protected:
  void DrawBezier(
    int,               // order (>=2)
    const ON_4fPoint*, // homogeneous points in clipping coordinates
    const CRhCurveAttributes&,
    int = 1,           // 0 if bezier is completely inside clipping box
    int = 0            // level
    );

  void MoveTo(const ON_4fPoint&); // clipping coordinate point
  void LineTo(const ON_4fPoint&); // clipping coordinate point
  void MoveToLineTo(const ON_4fPoint& start, const ON_4fPoint& end);
  void Line(const ON_4fPoint& start, const ON_4fPoint& end);

  void SetScreenDrawTolerance(
    double, // horizontal screen width tolerance (default = 1/2 pixel)
    double, // vertical screen width tolerance (default = 1/2 pixel)
    double  // screen depth tolerance (default = 1/2 pixel)
    );

  /*
  Description:
  Quickly calculates a zbias to apply in applications
  like drawing curves on surfaces.  In parallel projections
  the zbias is constant.  In perpsective projections the
  zbias depends on the depth where the bias is being applied.
  Parameters:
  z, w - [in] clipping depth (homogeneous coordinates) ( -1 <= z <= 1 )
  Returns:
  homogeneous zbias at z
  */
  double ZBiasDistance(double z, double w) const;

  void DrawConstructionPlaneFarLines(
    ON_SimpleArray<ON_Line>& far_lines
    );

  void DrawConstructionPlaneLine(
    const ON_Plane& near_plane,
    const ON_3dPoint& p, const ON_3dPoint& q,
    ON_SimpleArray<ON_Line>& far_lines
    );

  // Description:
  //   This should always be called instead of the virtual ShutDownEngine function. This function
  //   does eventually call the virtual ShutDownEngine
  void iShutdownEngine();

  HDC GetRenderTargetDC() const;

public:
  const CDisplayPipelineAttributes* DisplayAttrs() const;

private:
  friend class CRhinoDisplayPipeline;
  friend class CRhinoPipelineRef;
  friend class CRhDisplayEnginePrivate;

protected:
  // New for V8 [begin]
  bool SupportsVboBuffers() const;
  void BufferShadedMesh(CRhinoCacheHandle& cache, const ON_Xform* xform);
  void BufferShadedMesh(CRhinoCacheHandle& cache, ECullFaceMode cfm,
    const ON_Color& frontAmbient, const ON_Color& frontDiffuse,
    const std::shared_ptr<CDisplayPipelineMaterial>& material, const ON_SHA1_Hash& materialHash, const ON_Xform* xform);
  void BufferShadedMeshes(class CRhVboMeshCache* buffer);
  void BufferMeshWires(CRhinoCacheHandle& cache, const CDisplayPipelineAttributes& da, const ON_Xform* xform, bool isSubD, const CRhinoSubDDisplayAttributes* subdAttrs);

public:
	void SetLinearWorkflow(const CDisplayPipelineAttributes::LinearWorkflow& lw);
  const CDisplayPipelineAttributes::LinearWorkflow& LinearWorkflow() const;
	bool IsPreview(void) const;
	void SetIsPreview(bool);
	ON_UUID PreviewEnvironmentId(void) const;
	void SetPreviewEnvironmentId(ON_UUID id);
  // New for V8 [end]

public:
  void EnableShadows(bool);
  bool ShadowsEnabled(void) const;

protected:
  ////////////////////////////////////////////////////
  // Common attributes "probably" used by most engine implementations.
  // Note: These get set appropriately by the framework when necessary so
  //       there's no guess-work and no duplication needed by other engines.
  //       see description for "NotifyProjectionChanged"

  // Viewport size attributes...
  int                   m_nDisplayWidth = 0;
  int                   m_nDisplayHeight = 0;

  // Stereo viewing operations...
  bool                  m_bInStereoMode = false;
  float                 m_fStereoSeparation = 1.0f;
  float                 m_fStereoParallax = 1.0f;

  ///////////////////////////////////////////////////////////////////////////
  // Engine display and drawing attributes...
  HWND                  m_hWnd = nullptr; // Window handle for window based render targets
  HDC                   m_hDC = nullptr;  // Device Context handle render target

  ON_Viewport           m_Camera;

  int                   m_wire_depth = 0;

  bool                  m__zbias_use_d = true; // true to return constance bias
  double                m__zbias_d = 0; // parallel proj bias or 0 for no zbias
  double                m__zbias_a = 0; // (near+far)
  double                m__zbias_b = 0; // (far-near)
  double                m__zbias_c = 0; // m__zbias_d*/(4*near*far)


  CRhinoDisplayPipeline*    m_dp = nullptr;

  float                 m_dpi_scale = 1.0f; // scaling for high resolution displays

  ON_SimpleArray<ON_Xform> m_modelview_matrix_stack;
  ON_SimpleArray<bool> m_depth_writing_stack;
  ON_SimpleArray<bool> m_depth_testing_stack;

  bool m_bUseMaterials = true;

  ON_SimpleArray<ON_Xform>         m_ModelXformStack;
  ON_ClassArray<ON_Viewport>       m_CameraStack;
  
  
  // If m_bInfiniteDepth = true, then wireframe drawing does 
  // not clip at the near/far clipping planes.  This is useful
  // when code like curvature hair display or control
  // polygon editing is drawing curves and points that are
  // often outside the view frustum and changing the near/far
  // clipping planes to include the wireframe is either
  // computationally expensive or leads to numerically
  // unstable view frustums.  The default setting of
  // m_bInfiniteDepth is false.  The setting is controlled
  // by CRhinoDisplayPipeline::SetInfinteDepth() and
  // reported by CRhinoDisplayPipeline::InfiniteDepth().
  // It is a "state" setting and any code that changes
  // m_bInfiniteDepth is responsible for restoring its
  // value to the previous setting.
  bool                  m_bInfiniteDepth = false;

  CRhCurveAttributes    m_curve_attrs;
  
  float                 m_stol_x = 0.75f;
  float                 m_stol_y = 0.75f;
  float                 m_stol_z = 1.00f; // screen buffer drawing tolerances
  
  float                 m_cliptol_x = 0.01f;
  float                 m_cliptol_y = 0.01f;
  float                 m_cliptol_z = 0.01f; // tolerance in clipping coordinates
  
  int                   m_draw_speed = 0; // 0 = accurate, 1 = fast, 2 = really

  mutable class CRhDisplayEnginePrivate* m_private = nullptr;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};
