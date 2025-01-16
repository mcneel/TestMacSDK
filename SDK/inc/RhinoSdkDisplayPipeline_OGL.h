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

// Texture combiner constants used in DrawCombinedTextures() method...
enum : unsigned int
{
  COMBINE_ADD        = 1,
  COMBINE_MULTIPLY   = 2,
  COMBINE_BLEND      = 3,
  COMBINE_HATCHED    = 4, 
  COMBINE_MULTIPLY_WITH_COLOR = 5,
};


RHINO_SDK_FUNCTION const ON_ClassId*  RegisterOpenGLEngine(const ON_ClassId*);

class RHINO_SDK_CLASS CRhinoDisplayPipeline_OGL : public CRhinoDisplayPipeline
{
  DECLARE_DYNAMIC_PIPELINE(CRhinoDisplayPipeline_OGL)
public:
  CRhinoDisplayPipeline_OGL();
  CRhinoDisplayPipeline_OGL(CRhinoDoc*);
  CRhinoDisplayPipeline_OGL(CRhinoViewport&,CRhinoDoc*);

#if defined (ON_RUNTIME_WIN)
  CRhinoDisplayPipeline_OGL(HWND hwnd, CRhinoViewport&);
#endif

  CRhinoDisplayPipeline_OGL(CRhinoView& view, CRhinoViewport&);

  ON_DEPRECATED_MSG("Use CRhinoDisplayPipeline_OGL(CRhinoViewport&) instead")
  CRhinoDisplayPipeline_OGL(HDC,  CRhinoViewport&);

  CRhinoDisplayPipeline_OGL(CRhinoViewport&);
  void     Construct();
  virtual ~CRhinoDisplayPipeline_OGL();


public:  
  bool InitializePipeline() override;

  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool CalcClippingPlanes(ClippingInfo&) override;
  
  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool SetupFrustum(const ClippingInfo&) override;
          
  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool SetupLighting() override;

  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool InitFrameBuffer() override;

  //Description:
  //  Draws the viewport as seen from the left and the right eye viewports
  //  and returns the result as OpenGL texture handles.
  //
  //  dpa: The display pipeline attributes to use for drawing.
  //  vp_eye_left: The viewport representing the left eye location and look direction.
  //  vp_eye_right: The viewport representing the right eye location and look direction.
  //
  //Returns:
  //  handle_eye_left:  Will contain the OpenGL texture handle which references the left output color buffer.
  //  handle_eye_right: Will contain the OpenGL texture handle which references the right output color buffer.
  //
  //  true:  Drawing succeeded - 'handle_eye_left' and 'handle_eye_right' contain valid handles.
  //  false: Drawing failed    - 'handle_eye_left' and 'handle_eye_right' are both set to 0.
  bool DrawStereoFrameBuffer(
    CDisplayPipelineAttributes& dpa,
    const ON_Viewport& vp_eye_left,
    const ON_Viewport& vp_eye_right,
    unsigned long long& handle_eye_left,
    unsigned long long& handle_eye_right);

  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool DrawBackground() override;

  void DrawDocumentBasedCustomRenderMeshes();
  bool DocumentCustomRenderMeshesExist(void);

  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool DrawMiddleground() override;

  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool DrawForeground() override;

  //Returns:
  //  false - if you want to halt the processing of this routine's conduits.
  //  true  - if you want to continue processing this routine's conduits.
  bool PostProcessFrameBuffer() override;

  bool ShowFrameBuffer(HDC hDC) override;

  //Description:
  //  Disables and enables multi-sampling of drawn objects (ie. Anti-aliasing)
  //
  //  Note: Some video cards may not support multi sample modes, therefore,
  //        calling these routines on some systems may have no effect.
  void EnableMultiSample(bool enable);
  
  //Description:
  //  Creates a plane object that represents the ground plane currently being
  //  used/drawn.
  //Returns:
  //    false   - if there is no ground plane OR the ground plane is turned OFF
  //    true    - if a valid ground plane exists AND it is currently turned ON
  bool GetGroundPlane(ON_Plane&);
  
  const ON_ClassId* EngineClass() override;

  class CRhinoDisplayEngine_OGL*  GetEngine(bool create=false);
  class CRhGLMultiPassRender*     GetMultiPassRenderer(void);

  void DrawClippingSlices();

  // Draws the semi-transparent quad for a selected clipping plane object
  void DrawClippingSelection();

  void DrawSlice(int nObjIndex, int  CPlaneIndex, const ON_Mesh* pSlicer);

  enum : unsigned int
  {
    de_Normal                  = 0x00,
    de_NoRealtimeRenderer      = 0x01,
    de_NoShadows               = 0x02,
    de_NoShading               = 0x04,
    de_BoundingBoxesOnly       = 0x08
  };
  
  //Return the enum above with de_ prefix to determine how this frame should be drawn.
  unsigned int DegradationEffects() const;
  void DeterminePerformanceState(void);

protected:
  bool SinglePassLighting();
  
  bool SinglePassInitFrameBuffer();
  bool MultiPassInitFrameBuffer();
  
  bool SinglePassDrawBackground();
  bool MultiPassDrawBackground();
  
  bool SinglePassDrawMiddleground();
  bool MultiPassDrawMiddleground();
  
  UINT GetShadowMapTexture();
  bool DrawCombinedTextures(UINT  nT1, UINT  nT2, int  nCOP, float Scale, ON_Color Color = ON_UNSET_COLOR);
  
  bool EnableGroundPlane(bool);
  bool DrawGroundPlane();
  
private:
  void DrawSlice(int nObjIndex, int  CPlaneIndex, const ON_Mesh* pSlicer, CRhinoCacheHandle* cacheHandle);

  static const ON_ClassId* m_pEngineClass;
  
  friend RHINO_SDK_FUNCTION
  const ON_ClassId* RegisterOpenGLEngine(const ON_ClassId*  pEngine);

private:
  friend class CRhOglPipelineExtension;
  friend class CRhDisplayPipelinePrivate;
  class CRhOglPipelineExtension* m_glpipe_private = nullptr;
  ON__UINT_PTR m_sdk_reserved = 0;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoZBuffer
{
public:
            CRhinoZBuffer();                       // Assumes active viewport projection...
            CRhinoZBuffer(const CRhinoViewport&);  // Uses passed in viewport projection...
            
public:
           ~CRhinoZBuffer();

public:
  void      Reset();
  
  bool      Capture();
  bool      Capture(const ON_4iRect&);
  
  void      SetCaptureRect(const ON_4iRect&);          // Defines the capture area...
  
  void      SetProjection(const CRhinoViewport&);  // Resets zbuffer with viewport projection...
  
  void      SetDisplayMode(const ON_UUID&  dmid);  // Sets display mode used in creating zbuffer...
                                                   // This also resets the display attributes to
                                                   // the ones found in the display mode...
  
  void      ShowIsocurves(bool);
  void      ShowMeshWires(bool);
  void      ShowCurves(bool);
  void      ShowPoints(bool);
  void      ShowText(bool);
  void      ShowAnnotations(bool);
  void      ShowLights(bool);
  void      ShowClippingPlanes(bool);
  void      ShowGrips(bool);

  void      EnableShading(bool);
  void      EnableGroundPlane(bool);
  
public:
  int       Width();                           // Returns the width of the entire ZBuffer...
                                                   // Note: This may not be the same as the capture
                                                   //       area width...
  int       Height();                          // Returns the height of the entire ZBuffer...
                                                   // Note: This may not be the same as the capture
                                                   //       area height...
  
  const ON_4iRect&     CaptureRect();                     // Returns the current capture rect (area)...
  
  int       HitCount();                        // Returns the number of points within the capture
                                                   // area that contain z values that don't fall on
                                                   // the near or far clipping planes...
  
  float     MaxZ();                            // Returns maximum Z value that's < 1.0 within the
                                                   // capture area...
                                                   
  float     MinZ();                            // Returns minimum Z value that's > 0.0 within the
                                                   // capture area...
                                                  
  float ZValue(int x, int y);        // Retrieves the raw zbuffer value at (x,y)...
  float ZValueMax(int x, int y, int r); // Retrieves the largest (i.e. most distant) Z value within
                                        // a square of radius 'r' centered at (x, y).
  float ZValueMin(int x, int y, int r); // Retrieves the smallest (i.e. closest) Z value within
                                        // a square of radius 'r' centered at (x, y).
  void ZValueMinMax(int x, int y, int r, float& min_z, float& max_z);
                                        // Retrieves the smallest and largest (i.e. closest and most distant) Z value within
                                        // a square of radius 'r' centered at (x, y).
  const ON_3dPoint&   WorldPoint(int x, int y);    // Retrieves zbuffer value at (x,y) in world coordinates...
  const CRhinoDib&  GrayscaleDib();          // Retrieves the zbuffer in the form of a grayscale
                                                   // dib (bitmap) with increasing Z values converging to
                                                   // black (0)...
  
  const CRhinoViewport* RhinoVP(); 
  const ON_Viewport&    VP(); 
  const float*          Array();
  int                   Size();
            
private:
  bool                        m_bCaptured;
  bool                        m_bEnableGroundPlane;
  bool                        m_ownsDP;
  ON_4iRect                   m_ZBRect;
  ON_4iRect                   m_capRect;
  int                         m_nHitCount;
  float                       m_fMin;
  float                       m_fMax;
  
  CRhinoView*                 m_pView;
  CRhinoViewport              m_vp;
#if defined(ON_RUNTIME_WIN)
  CRhinoDisplayPipeline_OGL   m_unused_reserved;
#endif
  CRhinoDisplayPipeline*      m_dp;
  CDisplayPipelineAttributes  m_da;

  float*                      m_pZ;
  ON_SimpleArray<ON_3dPoint>  m_W;
  CRhinoDib                 m_GrayscaleDib;

  bool      CreateWorldPoints();

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};
