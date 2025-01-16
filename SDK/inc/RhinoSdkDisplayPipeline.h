//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
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


struct ClippingInfo
{
  // CRhinoDisplayPipeline::iCalcBoundingBox sets
  // bbox.  Often it is too big, but generally contains
  // everything core Rhino knows needs to be drawn.
  ON_BoundingBox    bbox;

  // CRhinoDisplayPipeline::iCalcClippingPlanes sets 
  // min_near_dist and min_near_over_far based on 
  // video hardware capabilities.   If you want high 
  // quality display, make sure your code sets near 
  // and far so that
  //    bbox_near >= min_near_dist
  //    bbox_near >= min_near_over_far*bbox_far
  double            min_near_dist;
  double            min_near_over_far;

  // Rhino sets this to be the distance from the camera
  // to the target.  It can generally be ignored.
  // In situations where bbox_near and bbox_far are too
  // far apart, target_dist can be used as a hint about
  // what area of the frustum is the most important to
  // show.
  double            target_dist;

  // You can override the virtual function
  // CRhinoDisplayPipeline::CalcClippingPlanes() 
  // and adjust the values of
  //   m_Clipping.bbox_near
  //   m_Clipping.bbox_far
  // If you set them incorrectly, Rhino will ignore 
  // your request.
  //    bbox_near >  0
  //    bbox_far  >  bbox_near
  //
  // NOTE WELL: 
  //   These are INPUT values used in the calculations
  //   that set the view frustum's near and far values. 
  //   Use the visible portion of objects you want to
  //   set to set bbox_near and bbox_far.  Specifically,
  //   do not set bbox_near = frustum_near, or you will
  //   cause "near creep" when relative depth bias is taken
  //   into account when near clipping planes are set.
  double            bbox_near;
  double            bbox_far;
  
  // These fields report the view frustum that was set by 
  // Rhino.  Unless you are doing something pretty fancy,
  // you should ignore these values and query the actual
  // view frustum if you want to know where it is.
  // These values get set when core Rhino view code
  // calculates the view frustum. None of these values are
  // used as input to any core Rhino calculation that sets
  // a view frustum.
  double            frustum_left,frustum_right;
  double            frustum_top,frustum_bottom;
  double            frustum_near,frustum_far;

  ON_3dPoint        camera_location;
  ON_3dVector       camera_Y; // unit up vector
  ON_3dVector       camera_Z; // unit vector (negative unitized direction)

private:
  // Reserved. 
  //   If you reference the memory in the __padding_01[] field
  //   in any way, then your code will cause Rhino to crash in
  //   the future.
  ON__UINT_PTR      reserved_1 = 0;
  ON__UINT_PTR      reserved_2 = 0;
};


//////////////////////////////////////////////////
// Object management constructs...
typedef ON_SimpleArray<int>       DisplayBucket;
typedef ON_SimpleArray<const CRhinoObject*> ObjectArray;

//
//////////////////////////////////////////////////

enum EDepthMode : unsigned int
{
  DM_NEUTRAL = 0,
  DM_ALWAYS_IN_FRONT,
  DM_ALWAYS_IN_BACK,
};

enum EZBiasMode : int
{
  ZBM_AWAY_FROM_CAMERA = -1,
  ZBM_NEUTRAL = 0,
  ZBM_TOWARDS_CAMERA = 1
};



/////////////////////////////////////////////////////////////////////  

typedef bool(*PFDRAWOVERRIDEPROC)(CRhinoDisplayPipeline&);

/////////////////////////////////////////////////////////////////////  
#if defined(ON_COMPILER_CLANG)

#define DECLARE_DYNAMIC_PIPELINE(class_name) \
private:\
  ON_OBJECT_DECLARE( class_name );\
public: \
  static bool     m_bRegistered##class_name; \
  static ON_UUID  m_Uuid##class_name;\
  static ON_UUID  Uuid(void);\
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"") \
  virtual bool    IsRegistered(void); \
_Pragma("clang diagnostic pop") \

#else

#define DECLARE_DYNAMIC_PIPELINE(class_name) \
private:\
  ON_OBJECT_DECLARE( class_name );\
public: \
  static bool     m_bRegistered##class_name; \
  static ON_UUID  m_Uuid##class_name;\
  static ON_UUID  Uuid(void);\
  virtual bool    IsRegistered(void);

#endif
  
#define IMPLEMENT_DYNAMIC_PIPELINE(class_name, base_name, description, uuid, bwire, bshade) \
  ON_OBJECT_IMPLEMENT_NO_COPY( class_name, base_name, uuid );\
  ON_UUID   class_name::m_Uuid##class_name = ::ON_UuidFromString( uuid );\
  bool      class_name::m_bRegistered##class_name = ::RegisterDisplayPipeline(&ON_CLASS_RTTI(class_name),\
                                                                              class_name::m_Uuid##class_name,\
                                                                              description,\
                                                                              bwire, bshade); \
  ON_UUID   class_name::Uuid(void) { return class_name::m_Uuid##class_name; }\
  bool      class_name::IsRegistered(void) { return class_name::m_bRegistered##class_name; }
  

//Forward declaration for CRhinoDisplayPipeline::RealtimeDisplayMode
namespace RhRdk
{
	namespace Realtime
	{
		class DisplayMode;
	}
}

///////////////////////////////////////////////////////////////////////
// Drawing in Rhino is performed by a display pipeline / display engine
// pair. The pipeline feeds geometry for drawing to an engine and also
// controls the order to drawing operations.
class RHINO_SDK_CLASS CRhinoDisplayPipeline : public ON_Object
{
private:
  // m_display_pipeline_sn must be the first member of CRhinoDisplayPipeline
  // to insure it can be used in any other code that appears in the
  // CRhinoDisplayPipeline constructors.
  const unsigned int m_display_pipeline_sn;
  unsigned int m_cloned_from_sn;
  unsigned int m_crhinoviewport_sn;

private:
  DECLARE_DYNAMIC_PIPELINE(CRhinoDisplayPipeline)

public:
  // Description:
  //  Constructs a display pipeline that will render to an off-screen DIB (aka, backbuffer bitmap).
  //  The pipeline does not have any viewport information with this constructor.
  //
  //  IMPORTANT: You MUST call SetProjection with a valid CRhinoViewport before calling any
  //             functions on this class.
  CRhinoDisplayPipeline(); 
  CRhinoDisplayPipeline(CRhinoDoc* pDoc);
  
  // Description:
  //  Constructs a display pipeline that will render to an off-screen DIB (aka, backbuffer bitmap). 
  // Parameters:
  //   vp: [in] viewport that supplies projection information. This pipeline is only valid for the
  //       lifetime of vp or you call SetProjection with a different CRhinoViewport
  CRhinoDisplayPipeline(CRhinoViewport& vp); 
             
  virtual ~CRhinoDisplayPipeline();

  unsigned int RuntimeSerialNumber() const { return m_display_pipeline_sn; }

  static CRhinoDisplayPipeline* FromRuntimeSerialNumber(unsigned int sn);
  
  static CRhinoDisplayPipeline* CreateDefaultPipeline();

  // Description:
  //  Returns true if the pipeline pointer is null or points to an existing
  //  CRhinoDisplayPipline.
  // Remarks:
  //  Calling CRhinoDisplayPipelin::IsValidPipelinePointer() is fast and should be
  //  used before dereferencing a CRhinoDisplayPipelin pointer that could possibly
  //  be invalid.
  static bool IsValidPipelinePointer( const void* pipeline );

  // Description:
  //  Post-constructor initialisation, used when a new pipeline is constructed through the
  //  ON_ClassId mechanism.
  //  Note that this function will go away when pipelines are no longer built on top of
  //  MFC.
  // Parameters:
  //   name:  [in] string that can be used to further specialise the freshly created instance
  virtual void PostConstructInitialize(
    const char* name
    );

  enum class DisplayTechnologies : int
  {
    None = 0,
    OpenGL = 1,
    Metal = 2,
    DirectX = 3,
    Software = 4,
    Vulkan = 5
  };

/////////////////////////////////////////////////////////////////////  
// STATIC Pipeline routines...
public:
  // Description:
  static void EnableFrameBufferCapture(bool bEnable=true);
  // Returns:
  static bool FrameBufferCaptureEnabled();
  
  // Description:
  static void EnableFeedbackProcessing(bool bEnable=true);
  // Returns:
  static bool FeedbackProcessingEnabled();
  
  // Description:
  //  Turn on/off automatic mesh generation for surfaces in shaded display modes.
  static void EnableMeshGeneration(bool bEnable=true);
  // Returns:
  //  static state determining if pipelines should automatically generate meshes
  //  for surfaces when needed in shaded display modes
  static bool MeshGenerationEnabled();
  
  // Description:
  //  Unbinds all registered conduits from a given viewport.
  //  See CRhinoDisplayConduit::Bind and Unbind
  // Parameters:
  //  pViewport: [in] the viewport to remove conduit bindings from
  static void ClearAllBindings(const CRhinoViewport* pViewport);

private:
  // Description:
  //  Display conduits are maintained in lists that are accessed by pipelines and
  //  iterated through at the appropriate time during the drawing of a frame.
  //  This function adds a given display conduit to these lists. Typically you would
  //  call CRhinoDisplayConduit::Enable to perform this operation instead of calling
  //  this static function (Enable just calls RegisterConduit(this)).
  // Parameters:
  //  pConduit: [in] the conduit to register
  static void RegisterConduit(CRhinoDisplayConduit* pConduit);
  // Description:
  //  Display conduits are maintained in lists that are accessed by pipelines and
  //  iterated through at the appropriate time during the drawing of a frame.
  //  This function removes a given display conduit from these lists. Typically you would
  //  call CRhinoDisplayConduit::Disable to perform this operation instead of calling
  //  this static function (Disable just calls UnregisterConduit(this)).
  // Parameters:
  //  pConduit: [in] the conduit to unregister
  static void UnregisterConduit(CRhinoDisplayConduit* pConduit);

public:
  // Returns:
  //  The window that this pipeline is associated with.
  HWND GetWnd() const;

  // Returns:
  //  The viewport that this pipeline uses to define projections and what gets drawn. It is
  //  assumed that a pipeline always has an associated viewport.
  CRhinoViewport* GetRhinoVP() const;
  CRhinoView*     GetRhinoView() const;

  // Description:
  //  Every pipeline contains an instance of a display engine. The engine is used to perform
  //  the low level drawing operations for the pipeline.
  // Parameters:
  //  bCreate: [in] If the an engine hasn't been created for this pipeline one will be created
  //           If an associated engine already exists, this parameter is ignored
  // Returns:
  //  the engine associated to this pipeline
  CRhinoDisplayEngine* Engine(bool bCreate=false) const;

  std::shared_ptr<RhRdk::Realtime::DisplayMode> RealtimeDisplayMode() const;

  // Returns:
  //  The size of the framebuffer that this pipeline is drawing to
  const ON_2iSize GetFrameSize() const;

  // Returns:
  //  The average amount of time required to draw a single frame
  UINT GetSingleFrameRate() const;

  // Returns:
  //  The desired amount of time to stay under for drawing a frame
  //  The pipeline makes decisions on level of detail when the frame rate
  //  exceeds this desired value.
  UINT GetDesiredSingleFrameRate() const;

  // Returns:
  //  The average frames per second interval for drawing with the current
  //  set of geometry
  UINT GetFramesPerSecond() const;
  
  double GetStereoSeparation() const;
  double GetStereoParallax() const;

  // Returns:
  // List of display conduits bound to this pipeline
  ConduitList GetBoundConduits();
  
  // Returns:
  //  True if the pipeline is currently in overlay mode. The overlay stage of drawing
  //  occurs after everything else has been drawn in a frame. It is used to draw things
  //  like highlighted geometry.
  bool InOverlayMode() const;

  // Returns:
  //  True if the pipeline is currently drawing a feedback type display.
  //  Feedback mode is defined as being in a GetPoint or in some kind of
  //  overlay conduit... but not in a dynamic view command
  //  (Pan, Rotate, Zoom, RotateCamera)
  bool InFeedbackMode() const;
  bool InFeedbackMode(bool bIgnoreOverlay) const;
  
  // Returns:
  //  True if this pipeline is using an engine that takes advantage of hardware acceleration
  bool IsAccelerated() const;

  // Returns:
  //  True if this pipeline is currently drawing for printing purposes
  bool IsPrinting() const;

  // Returns:
  //  True if this pipeline is currently using an engine that is performing stereo style drawing
  //  Stereo drawing is for providing an "enhanced 3-D" effect through stereo viewing devices
  bool InStereoMode() const;

  // Returns:
  //  True if this pipeline is currently in a View Capture scenario...either to clipboard or file.
  //  False otherwise.
  bool InViewCapture() const;

  // Returns:
  //  The current stereo projection if stereo mode is on
  //  0 = left, 1 = right
  //  If stereo mode is not enable, this just returns 0
  int ActiveStereoProjection() const;
  
  // Returns:
  //  The viewport that this pipeline uses to define projection information when drawing
  ON_Viewport& VP() const;

  // Returns:
  //  The current model transformation that is applied to vertices when drawing.
  const ON_Xform& ModelTransform() const;

  // Returns:
  //  The current instance definition transformation. The same as ModelTransform() excluding
  //  the dynamic transform.
  const ON_Xform& InstanceDefinitionTransform() const;

  // Returns:
  //  True if a given 3d world coordinate point is inside the view frustum under the current
  //  viewport and model transformation settings
  // Parameters:
  //  world_coordinate: [in] the point to test for visibility
  bool IsVisible(const ON_3dPoint& world_coordinate) const;

  // Returns:
  //  True if this object is inside the view frustum under the current viewport and model
  //  transformation settings. This function calls a virtual IsVisibleFinal function that
  //  subclassed pipelines can add extra tests to. In the base class, this test only tests
  //  visibility based on the objects world coordinates location and does not pay attention
  //  to the object's attributes
  //
  //  NOTE: Use CRhinoDisplayPipeline::IsVisible() to perform "visibility"
  //        tests based on location (is some part of the object in the view frustum).
  //        Use CRhinoDisplayPipeline::IsActive() to perform "visibility"
  //        tests based on object type.
  bool IsVisible(const CRhinoObject* pObject) const;

  // Returns:
  //  True if some portion of a 3d world coordinate bounding box is inside the view frustum under the current
  //  viewport and model transformation settings
  // Parameters:
  //  bbox: [in] the box to test for visibility
  bool IsVisible(const ON_BoundingBox& bbox) const;

  // Description:
  //  Determines if an object can be visible in this viewport based on it's object type and display attributes.
  //  This test does not check for visibility based on location of the object.
  //  NOTE: Use CRhinoDisplayPipeline::IsVisible() to perform "visibility"
  //        tests based on location (is some part of the object in the view frustum).
  //        Use CRhinoDisplayPipeline::IsActive() to perform "visibility"
  //        tests based on object type.
  // Returns:
  //  true if this object can be drawn in the pipeline's viewport based on it's object type and display attributes
  bool IsActive(const CRhinoObject* pObject) const;

  // Description:
  //  Tests to see if the pipeline should stop drawing more geometry and just show what it has so far.
  //  If a drawing operation is taking a long time, this function will return true and tell Rhino it should just
  //  finish up and show the frame buffer. This is used in dynamic drawing operations.
  // Returns:
  //  true if the pipeline should stop attempting to draw more geometry and just show the frame buffer
  bool InterruptDrawing();

  // Description:
  //  Dynamic display is the state a viewport is in when it is rapidly redrawing because of an operation like panning or rotating.
  //  The pipeline will drop some level of detail while inside a dynamic display state to keep the frame rate as high as possible.
  // Returns:
  //  true if the current frame drawing is for a dynamic display viewport state.
  bool ViewInDynamicDisplay();

  // Enable degradation of the display to make a slow display render faster.
  // Note: This will work correctly if a call to DrawFrameBuffer is followed
  // by a call to ShowFrameBuffer.
  void EnableDynamicDisplayDowngrade(bool bEnable);

  // Returns 'true' if display degradation is enabled, 'false' otherwise.
  bool DynamicDisplayDowngradeEnabled() const;
  
  // Description:
  //  Just calls the standard C++ clock function which retrieves the number of clock ticks that have elapsed
  //  since Rhino was launched
  UINT GetFrameTick() const      { return (UINT)::clock(); }

  // Description:
  //  Retrieve the estimated time that this frame drawing operation will complete. This is based off of previous frame drawing.
  UINT GetFrameETA()  const      { return m_nFrameTimeEstimate; }

  // Description:
  //  Retrieve the amount of time that has elapsed since the current frame draw operation has begun
  UINT GetFrameTickDelta() const { return (UINT)::clock() - m_nFrameStartTime; }

  // Description:
  //  Retrieve the number of CRhinoObjects that this pipeline iterates through and draws
  int GetObjectCount() const;

  // Description:
  //  The pipeline maintains a list of objects that it draws during a single frame. This function retrieves an object
  //  from this list.
  // Parameters:
  //  index: [in] index in object list >= 0 and less than GetObjectCount()
  const CRhinoObject* GetObject(int index) const;

  int ActiveObjectIndex() const;

  const CRhinoObject* ActiveTopLevelObject() const;

  // Description:
  //  The pipeline allows to draw objects as part of other objects like Instance Reference does.
  //  This function retrieves the nesting stack currently active.
  // Parameters:
  //  stack: [out] - Optional. active object nesting stack is returned in outermost-innermost order.
  // Returns:
  //  -1 if no active object.
  //   0 if active object is a root object.
  //  >0 nesting level of active object and length of "stack"
  int ActiveObjectNestingLevel(ON_SimpleArray<const CRhinoObject*>* stack = nullptr) const;

  // Description:
  //  Returns true if the object at index has a model space transformation associated with it
  // Parameters:
  //  index: [in] index in list for the object in question. See GetObject(int index)
  bool  ObjectHasTransform(int index) const;

  // Description:
  //  If the object at index has a model space transformation associated with it, return that transformation.
  //  If no transformation is associated, then NULL is returned
  // Parameters:
  //  index: [in] index in list for the object in question. See GetObject(int index)
  const ON_Xform* GetObjectTransform(int index) const;
  
  // Description:
  //  Determines if the given object will cast shadows based on its current state and the state of this pipeline's
  //  shadow casting attributes.
  // Parameters:
  //  pObject: [in] object to test against shadow casting attributes
  //  ReferencePoint: [in] point value used as the "center" of reference when calculating the shadow bubble.
  bool ObjectCastsShadows(const CRhinoObject* pObject, const ON_3dPoint& ReferencePoint, ON_BoundingBox* = nullptr) const;
  
  // Description:
  //  Returns the display pipeline drawing attributes that will be used to draw a given rhino object in this pipeline
  // Parameters:
  //  pObject: [in] the object to look up display pipeline attributes for
  //  pAttributes: [in] if you want to use something other than the object's current attributes, pass in a different
  //               object attributes.
  const CDisplayPipelineAttributes* GetObjectDisplayAttrs(const CRhinoObject* pObject,
                                                          const CRhinoObjectAttributes* pAttributes=nullptr)  const;

  // Returns:
  //  The current display pipeline attributes used by the pipeline to define how objects are drawn
  const CDisplayPipelineAttributes* DisplayAttrs() const;
  
  // Returns:
  //  This pipeline's channel attributes currently being used by the "active" channel.
  const CChannelAttributes* ChannelAttrs() const;

  // Returns:
  //  Display mode ID for a display mode that needs to be set after drawing completes...
  const ON_UUID&  DeferredDisplayMode() const;

  // Description:
  //  Returns true if this pipeline has set a flag that tells it that it needs to redraw itself.
  //  During the drawing of a frame, the pipeline may decide that it needs to run through the frame buffer
  //  drawing code again and does this by setting the redraw flag in the pipeline.
  //
  //  This DOES NOT set the redraw flag. For this use SetRedraw.
  //  If you want to tell a pipeline to redraw outside the scope of drawing a frame, call CRhinoView::Redraw
  //  or CRhinoDoc::Redraw
  bool Redraw() const;

  // Description:
  //  Returns the current pass that the pipeline is in for drawing a frame. Typically drawing a frame requires
  //  a single pass through the DrawFrameBuffer function, but some special display effects can be achived through
  //  drawing with multiple passes.
  int RenderPass() const;
  
  // Description:
  //  Returns the current nested viewport drawing level. This is used to know if you are currently inside the drawing
  //  of a nested viewport (detail object in Rhino).
  //  Nest level = 0 Drawing is occurring in a standard Rhino viewport or on the page viewport
  //  Nest level = 1 Drawing is occurring inside a detail view object
  int NestLevel() const;

  // Description:
  //  When Rhino is drawing geometry inside of a detail view (NestLevel()==1)
  //  this function returns the CRhinoDetailObject that the geometry is being
  //  drawn into
  const class CRhinoDetailViewObject* InDetailViewDraw() const;
  
  // Returns:
  //  true if the pipeline is currently in a curve drawing operation
  bool DrawingWires() const;
  // Returns:
  //  true if the pipeline is currently in a grip drawing operation
  bool DrawingGrips() const;
  // Returns:
  //  true if the pipeline is currently in a surface drawing operation
  bool DrawingSurfaces() const;
  // Returns:
  //  true if the pipeline is currently inside of QuickDrawSurface().
  bool InQuickDrawSurface() const;
  bool ObjectsShouldDrawWires() const;
  bool ObjectsShouldDrawShadedMeshes() const;
  bool ObjectsShouldDrawSilhouetteHightlights() const;
  bool InHiddenLinePass(void) const;
  const ON_Linetype* HiddenLineType(void) const;
  bool SilhouetteAttrIsOn(void) const;

  void PreDrawSetup();
  void PreDrawSetup(const DisplayBucket& aDrawBucket);

  // Description:
  //  Adds(multiplies) a model space transformation on an object in the display list at index
  //  If the frame is currently in a dynamic display mode, then no transformation will be set
  // Returns:
  //  true on success
  ON_DEPRECATED bool AddObjectTransform(int index, const ON_Xform& xform);

  // Description:
  //  Adds(multiplies) a model space transformation on an object in the display list at index
  //  If the frame is currently in a dynamic display mode, then no transformation will be set
  // Parameters:
  //   index - [in]
  //   xform - [in]
  //   side - [in]
  //     -1:  pipeline object xform <- xform*(pipeline object xform)
  //     +1:  pipeline object xform <- (pipeline object xform)*xform
  // Remarks:
  //   If ViewInDynamicDisplay() is true, then you probably do not want to call
  //   MultObjectTransform(), but we'll let you decide.  Be sure to test your code in
  //   many situations including while using the mouse to pan, rotate and zoom
  //   the view.
  // Returns:
  //  true on success
  bool MultiplyObjectTransform(int index, const ON_Xform& xform, int side );

  // Description:
  //  Sets a model space transformation on an object in the display list at index
  // Returns:
  //  true on success
  bool SetObjectTransform(int index, const ON_Xform& xform);

  float DpiScale() const;
  void SetDpiScale(float scale);
public:
  // Description:
  //  Set a view/window/DC as the target-destination for rendering.
  // Returns:
  //  Always returns true
  bool SetRenderTarget(CRhinoView&);
  bool SetRenderTarget(HDC pDC);
 
#if defined (ON_RUNTIME_WIN)
   bool SetRenderTarget(HWND hWnd);
#endif

  // Description:
  //  Sets a flag in the pipeline to tell it that it needs to redraw itself.
  //  During the drawing of a frame, the pipeline may decide that it needs to run through the frame buffer
  //  drawing code again and does this by setting the redraw flag in the pipeline.
  //
  //  If you want to tell a pipeline to redraw outside the scope of drawing a frame, call CRhinoView::Redraw
  //  or CRhinoDoc::Redraw
  void SetRedraw(bool enable);
  
  //  Return the document that the pipeline uses to get geometry information from when drawing.
  const CRhinoDoc* GeometrySource() const;

  // Description:
  //  Sets a document that the pipeline uses to get geometry information from when drawing. Typically
  //  the pipeline will use RhinoApp().ObsoleteActiveDoc()
  CRhinoDoc* SetGeometrySource(CRhinoDoc*  pDoc);

  // Description:
  //  Used to draw a set of objects other than the standard objects in a RhinoDocument.
  //  If a custom draw list exists in this pipeline, only the objects in this list will be drawn
  //
  //  If you want to remove the custom draw list from this pipeline, pass an empty list to this function
  void SetCustomDrawList(const ObjectArray& custom_list);

  // Description:
  //  Set the viewport size for this pipeline. This function does affect the CRhinoViewport that this
  //  pipeline uses.
  void SetFrameSize(int nWidth, int nHeight);
  
  // Description:
  //  Set the model transformation that is applied to geometry being drawn.
  //  This includes both the dynamic transform and a potential instance definition transform.
  void SetModelTransform(const ON_Xform& xform);

  // Description:
  //  Set the instance definition transformation that is applied to blocks being drawn.
  void SetInstanceDefinitionTransform(const ON_Xform& xform);

  // Description:
  //  The "ShadingRequired" flag gets set inside the pipeline when a request is
  //  made to draw a shaded mesh but the current render engine doesn't support shaded
  //  mesh drawing...at this point the redraw mechanism will make sure everything will
  //  work the next time around.
  void SetShadingRequired(bool bRequired=true);

  // Description:
  //  If the pipeline is currently not using a render engine that supports shading and during
  //  the drawing of a frame it is determined that something needs to be shaded, this flag is set
  //  to inform the redraw mechanism that it should redraw and attempt to switch to a pipeline/engine
  //  that supports shading
  bool ShadingRequired() const;

  // Description:
  // If the pipeline should render larger-than-screen images by tiling, then this function will return true.
  // the current logic looks to see if the display attributes RealtimeDisplayId is ON_nil_uuid.  Other implementations
  // are possible.
  bool SupportsTiling() const;

  // Returns true if this pipeline is currently being used to create a selection map (typically for Drag and Drop operations)
  bool SelectionMap() const;

  // Returns true if this pipeline is currently being used to create a z buffer map
  bool ZBufferCapture() const;
  
  // Description:
  //  Tells the pipeline that it needs to copy the frame buffer contents to the pipeline managed frame buffer DIB
  //  at the end of drawing the current frame
  void SetCapture(bool bEnable=true);

  // Description:
  //  Set this to true to inform the pipeline that the purpose of drawing future frames is
  //  for printing.
  void SetPrintingMode(bool bPrinting);

  // Description:
  //   Gets the accumulated bounding box from all conduits that implement the SC_VIEWEXTENTS channel
  bool GetConduitViewExtents(ON_BoundingBox&);

  // Description:
  //  Turn on/off stereo mode for the pipeline
  // Returns:
  //  true on success
  //  false if you are attempting to turn on stereo mode for an engine that does not support stereo drawing
  bool SetStereoMode(bool bStereo);
  void SetStereoSeparation(float fSep);
  void SetStereoParallax(float fPar);
  
  PFDRAWOVERRIDEPROC SetDrawChannelsOverride(PFDRAWOVERRIDEPROC);
  
  // Description:
  //  Save the current model transform on a stack and set a new model transform
  // Remark:
  //    This function does not multiply mt by the existing model transform. If you
  //    want to compound transformations multiply ModelTransform() by your xform
  //    before calling this function.
  // Parameters:
  //  mt: [in] the transform that the engine's model transform will be set to.
  void  PushModelTransform(const ON_Xform& xform);

  // Description:
  //  Remove the currently pushed model transform from the stack and restore the
  //  previously pushed transform (if any).
  void  PopModelTransform();

  void      PushObjectColor(const ON_Color& color) const;
  void      PopObjectColor() const;
  
  ON_Color  ObjectColor() const;
  ON_Color  SetObjectColor(const ON_Color& color);
  UINT      SetLinePattern(UINT);
  int       SetCurveThickness(int);

  /*
  Parameters:
    bInfiniteDepth - [in]
      desire setting for m_pDisplayAttrs->m_bInfiniteDepth
  Returns:
    Previous value of m_pDisplayAttrs->m_bInfiniteDepth
  */
  bool  SetInfiniteDepth(bool bInfiniteDepth);

  /*
  Returns:
    Current value of m_pDisplayAttrs->m_bInfiniteDepth.
  */
  bool  InfiniteDepth() const;
  
  void  EnableDepthTesting(bool=true);
  void  EnableDepthWriting(bool=true);
  
  void  EnableColorWriting(bool=true);
  
  bool  EnableClippingPlanes(bool enable = true);
  void  DisableClippingPlanes();
  
  void  PushDepthTesting(bool);
  void  PopDepthTesting();

  void  PushDepthWriting(bool);
  void  PopDepthWriting();

  void  PushProjection(const ON_Viewport&);
  void  PopProjection();

  void        SetDepthMode(EDepthMode);
  EDepthMode  DepthMode() const;
  
  /*
  Returns:
    The relative depth bias used in depth bias calculations.
    The returned value is independent of the current "ZBiasMode".
    The returned value is >= 0.0.
  Example:

        // Adjust clipping coordinate depth
        double bias_sign = +1.0 or -1.0;
        const double relative_depth_bias = CRhinoDisplayPipeline::RelativeDepthBias();
        clip_z += pipeline->VP().ClipCoordDepthBias( bias_sign*relative_depth_bias, clip_z, clip_w );

        // Get clipbias transformation to adjust clipping coordinate depth
        double bias_sign = +1.0 or -1.0;
        const double relative_depth_bias = CRhinoDisplayPipeline::RelativeDepthBias();
        ON_Xform clipbias;
        pipeline->VP().GetClipCoordDepthBiasXform( bias_sign*relative_depth_bias, clipbias );
        ON_4dPoint clipping_coordinate_point = ...;
        ON_4dPoint biased_clipping_coordinate_point = clipbias*clipping_coordinate_point;

  See Also:
    CRhinoDisplayEngine::CRhinoDisplayEngine::ZBiasDistance
  */
  static double RelativeDepthBias();

  void SetZBiasMode(int);
  int  ZBiasMode() const;
  
  void  EnableDrawGrayScale(bool=true);
  
  void  EnableChannel(UINT);
  void  DisableChannel(UINT);
  
  bool  SetDisplayMode(const ON_UUID&);
  
  bool  FreezeDrawing(bool);
  
  bool  CreateSelectionMap(ObjectArray* = nullptr,
                           CDisplayPipelineAttributes* = nullptr,
                           HRGN = nullptr);
  bool  GeneratingSelectionMap(void) const;

  CRhinoDib*  GetSelectionMap();
                                             
  const CRhinoObject* GetObjectAt(int X, int Y);
  const CRhinoObject* GetObjectAt(int X, int Y, ON_COMPONENT_INDEX& ci);
  
  //////////////////////////
  // Description:
  bool  SetupDisplayMaterial(CDisplayPipelineMaterial&,
                             COLORREF) const;

  bool  SetupDisplayMaterial(CDisplayPipelineMaterial&,
                             const CRhinoObject*,
                             const CRhinoObjectAttributes* = nullptr,
                             const ON_Xform* pOptionalBlockXform = nullptr) const;

  bool  SetupDisplayMaterial(CDisplayPipelineMaterial&,
                             const CRhinoDoc*,
                             const CRhinoObject*,
                             const CRhinoObjectAttributes* = nullptr,
                             const ON_Xform* pOptionalBlockXform = nullptr) const;
  
  //////////////////////////
  // Description:
  //   Converts the passed in material to a single color material, no textures, no PBR settings,
  //   and no specific lighting characteristics (i.e. specular, reflective, etc...), just a plain
  //   specified single color will be used, everything else will be stripped and/or deleted.
  // Note: This also does not apply any kind of color adjustments to White or Black values.
  bool  ConvertToSingleColorMaterial(CDisplayPipelineMaterial&,
                                     ON_Color,
                                     bool noLigthing=false) const;

  // Description:
  //   Modifies the passed in display material object so that it draws
  //   blended with the passed in color...
  void  BlendMaterialWithColor(CDisplayPipelineMaterial&, 
                               COLORREF,
                               double dBlend=0.5) const;
  
  void  IgnoreLockedState(bool);
  
  //////////////////////////
  // Shader specifics...
  bool  EnableShaders(bool=true) const;
  bool  EnableEnvironment(bool=true);

  //////////////////////////
  // Description:
  //   Returns a dib containing all wires (based on display attributes) rendered 
  //   using "MaskColor" against a background color "Background"...
  bool  GetWiresAlphaMask(CRhinoDib&  dib, 
                          int           nWidth,
                          int           nHeight,
                          const CDisplayPipelineAttributes&,
                          int           nScale=1,
                          COLORREF      MaskColorbg=RGB(255,255,255), 
                          COLORREF      Background=RGB(0,0,0));
                                          
  //////////////////////////
  // Description:
  //   Returns a dib containing all Objects (based on display attributes) rendered 
  //   using "MaskColor" (note: No shading), against a background color "Background"...
  bool  GetObjectsAlphaMask(CRhinoDib&  dib,
                            int           nWidth,
                            int           nHeight,
                            const CDisplayPipelineAttributes&,
                            COLORREF      MaskColorbg=RGB(255,255,255), 
                            COLORREF      Background=RGB(0,0,0));
  
  //////////////////////////
  // Description:
  //   Returns a dib containing all wires (based on display attributes) rendered 
  //   using their respective colors against a background color "Background"...
  bool  GetWiresMask(CRhinoDib&  dib,
                     int           nWidth,
                     int           nHeight,
                     const CDisplayPipelineAttributes&,
                     int           nScale=1,
                     COLORREF      Background=RGB(255,255,255));
  
  // Description:
  //   Returns a dib containing all objects (based on display attributes) rendered 
  //   using their respective colors and shading against a background color "Background"...
  bool  GetSpecularMask(CRhinoDib&  dib,
                        int           nWidth,
                        int           nHeight,
                        const CDisplayPipelineAttributes&,
                        bool          bIgnoreTransparency);

/////////////////////////////////////////////////////////////////////
///////////////// D R A W I N G   R O U T I N E S ///////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////  
// Pipeline OBJECTS drawing operations...   
public:
  bool DrawWallpaper();

  bool DrawTraceImage();

  void DrawWireframeMesh(const ON_Mesh& mesh, const ON_Color& color, CRhinoCacheHandle* cache = nullptr);
  void DrawWireframeMeshes(const ON_Mesh** meshes, int count, const ON_Color& color, CRhinoCacheHandle* cache = nullptr);

  void DrawMeshVertices(const ON_Mesh&, const ON_Color& color);

  void DrawShadedMesh(const ON_Mesh&  mesh, const CDisplayPipelineMaterial* = nullptr);
  void DrawShadedMeshes(const ON_Mesh* const* meshes, int count, const CDisplayPipelineMaterial* material, CRhinoCacheHandle* const * caches);
  void DrawShadedMeshes(const ON_Mesh* const* meshes, int count, const CDisplayPipelineMaterial* material, CRhinoCacheHandle* const* caches, const ON_Xform* const* transforms);

  void DrawFace(const CRhinoObject*               pObject,
                      int                         face, 
                const CDisplayPipelineAttributes* = nullptr);
  
  void DrawFaces(const CRhinoObject*               pObject,
                 const ON_SimpleArray<int>&        faces, 
                 const CDisplayPipelineAttributes* = nullptr);
  
  void DrawRenderedMesh(const ON_Mesh* mesh,
                        const CRhinoMaterial& render_material,
                        bool bHighlighted);

  // Description:
  //   Draws a wireframe brep.
  // Parameters:
  //   brep - [in]
  //   display_density - [in] ON_3dmObjectAttributes.m_wire_density value
  // Remarks:
  //   This function can be slow and should not be used in drawing code
  //   that is called every time the mouse moves. Use the other version of DrawBrep
  void DrawBrep(const ON_Brep& brep, const ON_Color& wireColor = ON_Color::UnsetColor, int wireDensity=1);

  // Description:
  //  Draw a wireframe brep
  // Parameters:
  //  brep [in] - the brep to draw
  //  wireColor [in] - if ON_Color::UnsetColor, current display attribute colors are used.
  //  wireDensity [in] - IsoCurve density
  //  edgeAnalysis [in] - set to true if brep should be drawn as if it is an edge analysis mode
  //  cache [in/out] - optional. If not NULL, the display may use this to
  //                   attempt to draw the brep faster. Make sure to ALWAYS
  //                   pass the same cache for a given brep. If you pass the
  //                   same cache for different brep, performance of the display
  //                   code will not be as good.
  void DrawBrep(const ON_Brep* brep, const ON_Color& wireColor, int wireDensity, bool edgeAnalysis, CRhinoCacheHandle* cache);

  void DrawShadedBrep(const ON_Brep* brep, const CDisplayPipelineMaterial* material, CRhinoCacheHandle* cache);

  void DrawShadedBreps(const ON_SimpleArray<ON_Brep*>& breps, const CDisplayPipelineMaterial* = nullptr);

  void DrawClippingPlane(const ON_ClippingPlaneSurface* clippingplane, const ON_Color& wireColor);

  /*
  Description:
    Draws a shaded Brep with Zebra stripe preview.
  Parameters:
    brep  [in]     - Brep to draw.
    zas   [in]     - Zebra analysis settings.
    mp    [in]     - Analysis meshing parameters.
    color [in]     - Object color.
    cache [in/out] - Optional. If not nullptr, the display may use this to
                     attempt to draw the brep faster. Make sure to ALWAYS
                     pass the same cache for a given Brep. If you pass the
                     same cache for different Brep, performance of the display
                     code will not be as good.
  */
  void DrawZebraPreview(
    const ON_Brep* brep,
    const CRhinoZebraAnalysisSettings& zas,
    const ON_MeshParameters& mp,
    const ON_Color& color,
    CRhinoCacheHandle* cache
  );

  /*
  Description:
    Draws a shaded Mesh with Zebra stripe preview.
  Parameters:
    mesh  [in]     - Mesh to draw.
    zas   [in]     - Zebra analysis settings.
    color [in]     - Object color.
    cache [in/out] - Optional. If not nullptr, the display may use this to
                     attempt to draw the brep faster. Make sure to ALWAYS
                     pass the same cache for a given Brep. If you pass the
                     same cache for different Brep, performance of the display
                     code will not be as good.
  */
  void DrawZebraPreview(
    const ON_Mesh* mesh,
    const CRhinoZebraAnalysisSettings& zas,
    const ON_Color& color,
    CRhinoCacheHandle* cache
  );


  void DrawNurbsCurve(const ON_NurbsCurve& nurbs_curve);

  void DrawBezier(const ON_BezierCurve& bezier_curve);

  /*
  Description:
    Draw a subD.

  Parameters:
    subd_display - [in]

      If you have a CRhinoSubDObject, see the remarks below.

      If you have a CRhinoSubDObject and you want a temporary customized display, 
      call DrawSubD(subd_object.SubDDisplay(), customized_subd_display_attributes ).

      If you have an ON_SubD, then call
      DrawSubD( CRhinoSubDDisplay::Create(subd), subd_display_attributes )

      If you have an ON_SubDRef, then call
      DrawSubD( CRhinoSubDDisplay::Create(subd_ref), subd_display_attributes )


    subd_display_attributes - [in]

  Remarks:
    This is a tool for drawing subd objects that are not in a CRhinoSubDObject.
    Use CRhinoSubDObject::Draw() to draw CRhinoSubDObjects    
  */
  void DrawSubD( 
    const class CRhinoSubDDisplay& subd_display,
    const class CRhinoSubDDisplayAttributes& subd_display_attributes
    );

  void DrawBezier(int order, const ON_3dPoint* cv); // 3d world coordinate CVs 

  void DrawBezier(int            dim,
                  BOOL32           bRational,
                  int            order,
                  int            cv_stride,  // number of doubles between CVs (>=3)
                  const double*  cv4d        // 4d homogeneous world coordinate CVs 
                  );

  void DrawActivePoint( const ON_3dPoint& );


  
  // Description:
  //   Draws a direction arrow decoration.
  // Parameters:
  //   tail_point - [in] start of the arrow.
  //   direction_vector - [in] direction of arrow.
  //      A vector with a length of 1 unit draws the standard
  //      arrow dectoration.
  //   head_point - [out] if not NULL, the location of the
  //       arrow tip is returned here.
  // Remarks:
  //   These are the arrows used to indicate curve
  //   direction and surface normals.
  void DrawDirectionArrow(const ON_3dPoint& tail_point, const ON_3dVector& direction_vector, ON_3dPoint* head_point = nullptr);

  
  // Description:
  //   Draws a tangent line segment decoration.
  // Parameters:
  //   mid_point - [in] midpoint of line segment
  //   direction_vector - [in] unit vector direction of line segment.
  //   start_point -[out] if not NULL, the start of the tangent bar
  //      is returned here.
  //   end_point -[out] if not NULL, the end of the tangent bar
  //      is returned here.
  // Remarks:
  //   These are the line segments used to indicate curve
  //   tangents.  Generally, direction_vector should have length = 1.
  //   For special situations when a shorter/longer tangent bar
  //   is desired, 
  void DrawTangentBar(
    const ON_3dPoint& mid_point,
    const ON_3dVector& direction_vector,
    ON_3dPoint* start_point = nullptr,
    ON_3dPoint* end_point = nullptr
    );

  // Feedback drawing routines...Note: These methods should really only be used
  // for drawing objects while inside drawing operations...calling them from
  // outside the pipeline will have unknown results.
  void DrawObject(const CRhinoObject*, const ON_Xform* =nullptr, ON_Color color = ON_UNSET_COLOR);

  void DrawObject(const CRhinoInstanceDefinition*, const ON_Xform* =nullptr);

  void DrawSubObject(const CRhinoObject*, ON_COMPONENT_INDEX, const ON_Xform* =nullptr);

  void DrawObjects(const ON_SimpleArray<CRhinoObject*>&,
                   const ON_Xform* =nullptr);

  void DrawObjects(const ObjectArray&,
                   const ON_Xform* =nullptr);

public:
  bool DrawSilhouette(const CRhinoObject*, float thickness = -1.0f, ON_Color = ON_UNSET_COLOR, UINT pattern = 0xFFFFFFFF);

  // Description:
  //   Draws a wireframe extrusion.
  // Parameters:
  //   extrusion - [in]
  // Remarks:
  //   This function is intended to draw dynamic display in commands that
  //   create and edit extrusions.
  // 
  void DrawExtrusion(const ON_Extrusion& extrusion);

  // Description:
  //  Draw a wireframe extrusion
  // Parameters:
  //  extrusion [in] - the extrusion to draw
  //  wireColor [in] - if ON_Color::UnsetColor, current display attribute colors are used.
  //  wireDensity [in] - IsoCurve density
  //  edgeAnalysis [in] - set to true if extrusion should be drawn as if it is an edge analysis mode
  //  cache [in/out] - optional. If not NULL, the display may use this to
  //                   attempt to draw the brep faster. Make sure to ALWAYS
  //                   pass the same cache for a given extrusion. If you pass the
  //                   same cache for different extrusion, performance of the display
  //                   code will not be as good.
  void DrawExtrusion(const ON_Extrusion& extrusion, const ON_Color& wireColor, int wireDensity, bool edgeAnalysis, CRhinoCacheHandle* cache);
  void DrawExtrusion(const ON_Extrusion& extrusion, const ON_Color& wireColor, int wireDensity, bool edgeAnalysis, const class CRhDisplayPenPrivate* pen, CRhinoCacheHandle* cache);


  // Specific conduit channel drawing routines...These routines are primarily used
  // to draw the "current" object inside of a conduit's SC_DRAWOBJECT channel, exactly
  // the way the pipeline would draw it...in other words, if your conduit doesn't call
  // one these, then the pipeline will. See the "Objects access" section above for
  // more information you can obtain for the current object..
  
  ///////////////////////////////////////////////////////////////////
  // Description: 
  //   Draws the current object's wires. Note: for things that can't
  //   be shaded (like a curve), this is the only routine you need
  //   to call.
  //
  // Parameters:
  //   int        : [in] The index into the pipeline's object draw
  //                     list indicating which object you want drawn.
  // 
  //                Note: This should only be obtained while inside
  //                      the SC_DRAWOBJECT channel within a display
  //                      conduit by using the channel attribute:
  //                      "m_nObjectIndex". Using any other value at
  //                      any other time will have undetermined 
  //                      results.
  //
  //                For example:
  //                  dp.DrawObject( m_pChannelAttrs->m_nObjectIndex);
  //               
  void DrawObject(int index, bool wires, bool surfaces);
  
  ///////////////////////////////////////////////////////////////////
  // Description: 
  //   Draws the current 'surface' object using the current display 
  //   attributes to define the way it will be shaded.
  //   Note: If the object is not a surface, or one that can't be shaded, 
  //         this routine does nothing.
  //
  // Parameters:
  //   int        : [in] The index into the pipeline's object draw
  //                     list indicating which object you want shaded.
  // 
  //                Note: This should only be obtained while inside
  //                      the SC_DRAWOBJECT channel within a display
  //                      conduit by using the channel attribute:
  //                      "m_nObjectIndex". Using any other value at
  //                      any other time will have undetermined 
  //                      results.
  void DrawSurface(int);

  // Description: 
  //   Draws the object's "edges" using the passed in display 
  //   attributes to define the way it will be drawn. 
  
  //   Note: If this object is not a BRep or extrusion based object, 
  //         or one that doesn't have "edges", then this routine does
  //         nothing.
  void  DrawEdges(const CRhinoObject&, int  nThickness, UINT  nPattern, bool bShowSeams);

/////////////////////////////////////////////////////////////////////  
// Pipeline PRIMITIVES drawing operations...   
public:
  // Description:
  //   Draw a point using the current display attributes point size, point style
  //   and object color (m_nPointSize, m_ePointStyle, m_ObjectColor)
  // Parameters:
  //   point: [in] location of point in world coordinates
  void DrawPoint(const ON_3dPoint& point);
  
  // Description:
  //   Draw a point with a given radius, style and color
  // Parameters:
  //   point:  [in] location of point in world coordinates
  //   radius: [in] point size in pixels
  //   style:  [in] how the point is drawn
  //   color:  [in] color of point. If style is RPS_CONTROL_POINT, this will be
  //                the border color
  void DrawPoint(const ON_3dPoint& point, int radius, ERhinoPointStyle style, const ON_Color& color);

  void DrawPoints(int count, const ON_3dPoint* points, ERhinoPointStyle style,
    const ON_Color& strokeColor, const ON_Color& fillColor,
    float diameter, float strokeWidth, float innerDiameter,
    float rotationRadians, bool diameterIsInPixels, bool autoScaleDpi);

  void DrawPoint(const ON_3dPoint& point, ERhinoPointStyle style,
    const ON_Color& strokeColor, const ON_Color& fillColor,
    float diameter, float strokeWidth, float innerDiameter,
    float rotationRadians, bool diameterIsInPixels, bool autoScaleDpi);

  // Description:
  //   Draw inference point used in gesture based snapping
  // Parameters:
  //   P     : [in] world 3d
  //   color : [in] if = ON_UNSET_COLOR, then the system inference color
  //                is used.
  void DrawInferencePoint(const ON_3dPoint& P, const ON_Color& color = ON_Color::UnsetColor);
  
  void DrawDottedLine(const ON_3dPoint&,const ON_3dPoint&);

  // Description:
  //   Draw a line with a given color, thickness, and pattern
  // Parameters:
  //   from:      [in] start of line in world coordinates
  //   to:        [in] end of line in world coordinates
  //   color:     [in] color of line
  //   thickness: [in] thickness in pixels
  //   pattern:   [in] bit pattern for line. 0xFFFFFFFF is a solid line
  //                   see glLineStipple documentation online for a description of pattern
  void DrawLine(const ON_3dPoint& from, const ON_3dPoint& to, const ON_Color& color = ON_Color::UnsetColor, int thickness=0, UINT pattern=0xFFFFFFFF);

  // Description:
  //   Draw a polyline with a given color, thickness, and pattern
  // Parameters:
  //   polyline:  [in] polyline to draw in world coordinates
  //   color:     [in] color of polyline
  //   thickness: [in] thickness in pixels
  //   pattern:   [in] bit pattern for polyline. 0xFFFFFFFF is a solid line
  //                   see glLineStipple documentation online for a description of pattern
  void DrawPolyline(const ON_Polyline& polyline, const ON_Color& color, int thickness, UINT pattern=0xFFFFFFFF);
  
  // Description:
  //   Draw inference line used in gesture based snapping
  // Parameters:
  //   P     : [in] world 3d start
  //   Q     : [in] world 3d end
  //   color : [in] if = ON_UNSET_COLOR, then the system inference color is used.
  //   type  : [in] 
  //             0 = chord, 
  //             1 = ray from P through Q
  //             2 = infinite line
  void DrawInferenceLine( const ON_3dPoint& P, const ON_3dPoint& Q, ON_Color color=ON_Color::UnsetColor, int type=0 );

  // Description:
  //   Draw a closed shape.
  // Parameters:
  //   points: [in] array of world coordinate points that are connected by lines to form a closed shape
  //   count:  [in] number of items in the points array. Must be > 2
  //   color:  [in] draw color used for the polygon
  //   filled:[in] true if the closed area should be filled with color. false if you want to draw
  //                just the border of the closed shape
  void DrawPolygon( const ON_3dPoint* points, int count, const ON_Color& color, bool filled );

  void DrawArc(const ON_Arc&);
  void DrawArc(const ON_Arc&, ON_Color Color, int nThickness=1, UINT nPattern=0xFFFFFFFF);
  
  void DrawCircle(const ON_Circle&);
  void DrawCircle(const ON_Circle&, ON_Color Color, int nThickness=1, UINT nPattern=0xFFFFFFFF);

  // Description:
  //   Draw a wireframe sphere.
  // Parameters:
  //   sphere - [in]
  void DrawSphere(const ON_Sphere& sphere);
  void DrawSphere(const ON_Sphere& sphere, ON_Color Color, int nThickness=1, UINT nPattern=0xFFFFFFFF);

  // Description:
  //   Draw a wireframe torus.
  // Parameters:
  //   sphere - [in]
  void DrawTorus(const ON_Torus& torus);
  void DrawTorus(const ON_Torus& torus, ON_Color Color, int nThickness=1, UINT nPattern=0xFFFFFFFF);

  // Description:
  //   Draw a wireframe cylinder.
  // Parameters:
  //   cylinder - [in]
  void DrawCylinder(const ON_Cylinder& cylinder);
  void DrawCylinder(const ON_Cylinder& cylinder, ON_Color Color, int nThickness=1, UINT nPattern=0xFFFFFFFF);

  // Description:
  //   Draw a wireframe cone.
  // Parameters:
  //   cone - [in]
  void DrawCone(const ON_Cone& cone);
  void DrawCone(const ON_Cone& cone, ON_Color Color, int nThickness=1, UINT nPattern=0xFFFFFFFF);
  
  // Description:
  //   Draw a curve
  // Parameters:
  //   color: stroke color for curve. If unset, the display attributes m_ObjectColor is used
  //   thickness: stroke width for curve. If < 1, the display attributes m_nCurveThickness is used
  //   pattern: dash pattern to used. If 0, the display attributes m_nCurvePattern is used
  void DrawCurve(const ON_Curve& curve, const ON_Color& color=ON_Color::UnsetColor, int nThickness=0, UINT nPattern=0);

  // Description:
  //  Draw a solid curve
  // Parameters:
  //  curve [in] - the curve to draw
  //  color [in] - if ON_Color::UnsetColor, current display attribute colors are used.
  //  thickness [in] - stroke width for curve
  //  cache [in/out] - optional. If not NULL, the display may use this to
  //                   attempt to draw the brep faster. Make sure to ALWAYS
  //                   pass the same cache for a given brep. If you pass the
  //                   same cache for different brep, performance of the display
  //                   code will not be as good.
  void DrawCurve(const ON_Curve& curve, const ON_Color& color, float thickness, CRhinoCacheHandle* cache);

  void DrawTriangle( const ON_3dPoint corners[3]);
  void DrawTriangle( const ON_3dPoint corners[3], ON_Color color);
  void DrawTriangle( const ON_3dPoint& p0, const ON_3dPoint& p1, const ON_3dPoint& p2);
  void DrawTriangle( const ON_3dPoint& p0, const ON_3dPoint& p1, const ON_3dPoint& p2, ON_Color    color);
  
  /*
  Description:
    Draws a wireframe surface.
  Parameters:
    surface - [in]
    display_density - [in] ON_3dmObjectAttributes.m_wire_density value
  Remarks:
    This function can be slow and should not be used in drawing code
    that is called every time the mouse moves.
  See Also:
    DrawNurbsSurface
    DrawBezierSurface
    DrawSurface
    DrawBrep
  */
  void DrawSurface( 
        const ON_Surface& surface, 
        int display_density = 1
        );

  /*
  Description:
    Draws a wireframe NURBS surface.
  Parameters:
    nurbs_surface - [in]
    display_density - [in] ON_3dmObjectAttributes.m_wire_density value
  */
  void DrawNurbsSurface( 
        const ON_NurbsSurface& nurbs_surface, 
        int display_density = 1
        );

  /*
  Description:
    Draws a wireframe bezier surface.
  Parameters:
    bezier_surface - [in]
    display_density - [in] ON_3dmObjectAttributes.m_wire_density value
  */
  void DrawBezierSurface( 
        const ON_BezierSurface& bezier_surface,
        int display_density = 1
        );


  /*
  Description:
    Draws a wireframe NURBS cage.
  Parameters:
    nurbs_cage - [in]
    display_density - [in] ON_3dmObjectAttributes.m_wire_density value
  */
  void DrawNurbsCage( const ON_NurbsCage& nurbs_cage, int display_density = 1 );

  /*
  Description:
    Draws a wireframe bezier cage.
  Parameters:
    bezier_cage - [in]
    display_density - [in] ON_3dmObjectAttributes.m_wire_density value
  */
  void DrawBezierCage( const ON_BezierSurface& bezier_cage, int display_density = 1 );

#pragma region V6 display
public:

  // Description:
  //  Draw/fill a rounded rectangle in screen coordinates
  // Parameters:
  //  screenCenter - screen coordinate center of rectangle
  //  pixelWidth, pixelHeight - size of rectangle in pixels
  //  cornerRadius - radius in pixels
  //  strokeColor  - border color of rectangle
  //  strokeWidth  - width of border curve in pixels
  //  fillColor    - if ON_Color::UnsetColor, then no fill will be applied
  void DrawRoundedRectangle(ON_2fPoint screenCenter, float pixelWidth, float pixelHeight, float cornerRadius,
    ON_Color strokeColor, float strokeWidth, ON_Color fillColor = ON_Color::UnsetColor);

  // Description:
  //  Draw a text dot
  // Parameters:
  //  dot [in] - the text dot to draw
  //  fillColor [in] - background color for the dot
  //  textColor [in] - if ON_Color::UnsetColor, text color is white or black
  //                   based on the value of the fillColor
  //  borderColor [in] - if ON_Color::UnsetColor, will be the same as fillColor
  //  cache [in/out] - optional. If not NULL, the display may use this to
  //                   attempt to draw the dot faster. Make sure to ALWAYS
  //                   pass the same cache for a given dot. If you pass the
  //                   same cache for different dots, you will get unpredicitable
  //                   results.
  void DrawDot(const ON_TextDot& dot, ON_Color fillColor, ON_Color textColor, ON_Color borderColor, CRhinoCacheHandle* cache);

  void DrawDot(float screenX, float screenY, const wchar_t* text, ON_Color fillColor = ON_Color(0, 0, 0), ON_Color textColor = ON_Color::UnsetColor);
  void DrawDot(const ON_3dPoint& point, const wchar_t* text, ON_Color fillColor = ON_Color(0, 0, 0), ON_Color textColor = ON_Color::UnsetColor);
  void DrawDot(const ON_3dPoint& point, const wchar_t* text, const wchar_t* fontFace, int fontHeight, ON_Color fillColor = ON_Color(0, 0, 0), ON_Color textColor = ON_Color::UnsetColor);

  // Description:
  //  Draw a point cloud
  // Parameters:
  //  cloud [in] - the point cloud to draw
  //  overrideColor [in] - color to use for points. If the cloud has it's color
  //                       array filled in and overrideColor contains some alpha
  //                       the colors are blended.
  void DrawPointCloud(const ON_PointCloud& cloud, float pixelSize, ERhinoPointStyle pointStyle, ON_Color colorOverride,
    CRhinoCacheHandle* cache);
  void DrawPointCloud(const ON_PointCloud& cloud, float pixelSize, ERhinoPointStyle pointStyle, ON_Color colorOverride,
                      int indexCount, const unsigned int* indices,
                      CRhinoCacheHandle* cache);
  void DrawPointCloud(const ON_PointCloud& cloud, float size, bool sizeInWorld, ERhinoPointStyle pointStyle,
    ON_Color colorOverride, int indexCount, const unsigned int* indices, CRhinoCacheHandle* cache);

  void DrawPointCloud(int pointCount, const ON_3dPoint* points, ON_Color color = ON_Color::UnsetColor);

  //void DrawString(const ON_TextContent& text, ON_Color color, bool drawforward, CRhinoCacheHandle* cache);
  void DrawString(const ON_TextContent& text, double textScale, ON_Color color, bool drawforward, CRhinoCacheHandle* cache);
  void DrawString(const ON_TextContent& text, const ON_Xform& textXform, ON_Color color, CRhinoCacheHandle* cache);

  void DrawString(const wchar_t* text, ON_Color color, const ON_Plane& plane, double height,
    const ON_Font* font, ON::TextHorizontalAlignment horizontalAlignment, ON::TextVerticalAlignment verticalAlignment);

  void DrawAnnotation(
    ON_Annotation& annotation,
    ON_Color color,
    void* updater,
    CRhinoCacheHandle* cache);

  void DrawAnnotation(
    ON_Annotation& annotation,
    double dimscale,
    ON_DimStyle& dimstyle,
    ON_Color color,
    void* updater,
    CRhinoCacheHandle* cache);

  void DrawAnnotationArrowhead(
    const ON_Arrowhead& arrowhead,
    ON_Xform xform,
    ON_Color color,
    CRhinoCacheHandle* cache);

  // Draw a mesh based on the current state of display attributes
  void DrawMesh(const ON_Mesh& mesh, bool wires, bool shaded, CRhinoCacheHandle* cache);
  void DrawMeshes(const ON_Mesh** meshes, int count, bool wires, bool shaded, CRhinoCacheHandle* cache);

  void DrawHatch(const ON_Hatch& hatch, float strokeWidth, const ON_Color& hatchColor, ON_Color boundaryColor, CRhinoCacheHandle* cache);

  // Continue to do this for all geometry types
  //void DrawMesh(const ON_Mesh& mesh, const class CRhMeshWireAttributes* wireSettings, const CDisplayPipelineMaterial* shadeSettings, CRhinoCacheHandle* cache);
  //void DrawBrep(const ON_Brep& brep, const class CRhBrepWireAttributes* wireSettings, const CDisplayPipelinMaterial* shadeSettings, CRhinoCacheHandle* cache);
#pragma endregion

#pragma region V7 WIP display
  void DrawHatch(const ON_Hatch& hatch, float strokeWidth,
    const ON_Color& gradientColor1, const ON_Color& gradientColor2,
    const ON_3dPoint& gradientPoint1, const ON_3dPoint& gradientPoint2, bool linearGradient,
    const ON_Color& boundaryColor, CRhinoCacheHandle* cache);

  void DrawHatch(const ON_Hatch& hatch, float strokeWidth,
    int colorCount, const ON_ColorStop* colorStops,
    const ON_3dPoint& gradientPoint1, const ON_3dPoint& gradientPoint2, ON_GradientType gradientType,
    float repeatFactor, const ON_Color& boundaryColor, CRhinoCacheHandle* cache);

  void DrawMesh(const ON_Mesh& mesh, int colorCount, const ON_ColorStop* colorStops,
    const ON_3dPoint& gradientPoint1, const ON_3dPoint& gradientPoint2, ON_GradientType gradientType,
    float repeatFactor, CRhinoCacheHandle* cache);

  void DrawLines(int lineCount, const ON_Line* lines, float strokeWidth, int colorCount, const ON_ColorStop* colorStops,
    const ON_3dPoint& gradientPoint1, const ON_3dPoint& gradientPoint2, ON_GradientType gradientType,
    float repeatFactor, CRhinoCacheHandle* cache);

#pragma endregion


public:
  // Description:
  //   Draws text flat to the screen at a screen coordinates location with specified color, font and height in pixels
  void DrawString( 
    const wchar_t*    string,            // string to draw, can return a modified string
    ON_Color          color,             // draw color
    ON_2dPoint        point,             // definition point, either lower-left or middle
    bool              bMiddle  = false,  // TRUE: middle justified, FALSE: lower-left justified
    int               height   = 12,     // height in pixels
    const wchar_t*    fontface = L"Arial"
    );


  ////   Draws text flat to the screen at a 3d model space location with specified color, font and height in pixels
  void DrawString( 
    const wchar_t* string,    // string to draw, can return a modified string
    const ON_3dPoint& point,  // definition point, either lower-left or middle
    bool bMiddle = false,      // true: middle justified, false: lower-left justified
    int height = 12,          // height in pixels
    const wchar_t* fontface = L"Arial"
    );


  //Description:
  // Determine screen bounds that would be drawn to using the DrawString(..) function
  // with the same parameters.
  //Returns:
  // true on success, false on failure
  static bool MeasureString(ON_4iRect& rect, const wchar_t* text, ON_2dPoint point, bool middle = false, int heightPixels = 12, const wchar_t* fontface = L"Arial");
  static bool MeasureString(ON_2dPoint& boundsMin, ON_2dPoint& boundMax, const wchar_t* text, ON_2dPoint point, bool middle = false, double heightPixels = 12, const wchar_t* fontface = L"Arial");
  static bool MeasureString(ON_2dPoint& boundsMin, ON_2dPoint& boundMax, const wchar_t* text, ON_2dPoint point, bool middle, double heightPixels, const ON_Font* font);

  // Description:
  //   Draw a wireframe box from a ON_BoundingBox
  // Parameters:
  //   box       : [in] bounding box to draw
  //   color     : [in] color of wireframe
  //   thickness : [in] thickness of wireframe
  //
  void DrawBox( const ON_BoundingBox& box, const ON_Color& color, int thickness );
        
  // Description:
  //   Draw a wireframe box given its 8 corner points.
  // Parameters:
  //   box_corners - [in] array of eight box corners
  //
  //            7______________6
  //            |\             |\
  //            | \            | \
  //            |  \ _____________\
  //            |   4          |   5
  //            |   |          |   |
  //            |   |          |   |
  //            3---|----------2   |
  //            \   |          \   |
  //             \  |           \  |
  //              \ |            \ |
  //               \0_____________\1
  //
  void DrawBox(const ON_3dPoint* box_corners, const ON_Color& colot, int thickness);

  // Description:
  //   Draws the curve's curvature hair graph in a viewport window.
  // Parameters:
  //   curve         : [in]
  //   hair_settings : [in]  If NULL, the settings from RhinoApp().AppSettings().CurvatureGraphSettings() 
  //                         are used.
  // Remarks:
  //   This function can be slow and should not be used in drawing code
  //   that is called every time the mouse moves.
  // See Also:
  //   CRhinoDisplayPipeline::DrawCurve
  void DrawCurvatureGraph( 
        const ON_Curve& curve, 
        const CRhinoCurvatureGraphSettings* hair_settings = nullptr 
        );

  // Description:
  //   Draw the surface's normal curvature along surface iso parametric curves.
  // Parameters:
  //   surface      : [in]
  //   wire_density : [in] Density of curvature combs. If < -1, hair_settings.WireDensity()
  //                       is used.
  //   hair_settings: [in] If NULL, RhinoApp().AppSettings().CurvatureGraphSettings()
  //                       are used.
  void DrawNormalCurvatureGraph( 
        const ON_Surface& surface, 
        int wire_density,
        const CRhinoCurvatureGraphSettings* hair_settings = nullptr 
        );

  void DrawLight( const ON_Light& light, const ON_Color& wireframe_color );

  void DrawConstructionPlane( bool depthBuffered, int transparencyLevel );

  void DrawConstructionPlane( 
          const ON_3dmConstructionPlane& cplane,
          bool bShowConstructionGrid,
          bool bShowConstructionAxes,
          bool bShowConstructionZAxis,
          ON_Color thin_line_color = ON_Color::UnsetColor,
          ON_Color thick_line_color = ON_Color::UnsetColor,
          ON_Color grid_x_axis_color = ON_Color::UnsetColor,
          ON_Color grid_y_axis_color = ON_Color::UnsetColor,
          ON_Color grid_z_axis_color = ON_Color::UnsetColor
          );
          
  // Description:
  //  Draw cross hairs across the entire viewport that
  //  run along the plane's x and y axes.
  // Parameters:
  //  plane - [in] the cross hairs run along the plane's
  //               x and y axes.
  //  color - [in] color to use crosshairs
  void DrawCrossHairs( const ON_Plane& plane, const ON_Color& color );
  // Description:
  //  Draw cross hairs across the entire viewport that
  //  run along the plane's x and y axes.
  // Parameters:
  //  plane - [in] the cross hairs run along the plane's
  //               x and y axes.
  //  color - [in] color to use crosshairs
  //  strokeWidth - [in] width of crosshair line
  //  screenLength - [in] pixel length of crosshair
  void DrawCrossHairs( const ON_Plane& plane, const ON_Color& color, float strokeWidth, float screenLength);

  /////////////////////////////////////////////////////////////////////  
  // 2D Pipeline Operations: 
  // Note: These routines draw directly to the framebuffer so that
  //       they can be used during frame generation and not as a post
  //       process.
  void Draw2dRectangle(int x0, int y0, int x1, int y1, ON_Color strokeColor, float strokeWidth, RhinoDashStyle style, bool xormode);
  void Draw2dRectangle(float x0, float y0, float x1, float y1, ON_Color strokeColor, float strokeWidth, RhinoDashStyle style, bool xormode);

  ON_DEPRECATED_MSG("use alternate overloads of Draw2dRectangle")
  void Draw2dRectangle(const ON_4iRect&  rect, HPEN, bool = true);
  ON_DEPRECATED_MSG("use alternate overloads of Draw2dLine")
  void Draw2dLine(const ON_2iPoint&, const ON_2iPoint&, HPEN, bool = true);

  void Draw2dLine(int x0, int y0, int x1, int y1, ON_Color strokeColor, float strokeWidth, RhinoDashStyle style, bool xormode);
  void Draw2dLine(float x0, float y0, float x1, float y1, ON_Color strokeColor, float strokeWidth, RhinoDashStyle style, bool xormode);


  void DrawBitmap(const CRhinoDib&, int  nX, int  nY);
  
  // Description:
  //   Draw a filled 2D rectangle. The rectangle is in screen coordinates of the active viewport and may
  //   be drawn with a level of transparency.
  // Parameters:
  //   left, top:     [in] 2D coordinate of the left/top corner of the rectangle. This point is is "Windows"
  //                       coordinates where the top is is number of pixels from the top of the viewport
  //   width, height: [in] distances from left, top that define the rectangle
  //   color:         [in] Fill color for rectangle
  //   transparency:  [in] 0=opaque,  255=completely transparent
  void FillSolidRect(int left, int top, int width, int height, COLORREF color, int transparency=0);

/////////////////////////////////////////////////////////////////////  
// Fixed pipeline operations...   
public:
  bool OpenPipeline();
  bool PipelineOpened() const;
  
  bool ClosePipeline();
  
  bool CopyFrameBufferTo(HDC);
  
  bool CopyFrameBufferTo(CRhinoDib&);

  bool CopyBackBufferTo(HDC);
  
  bool CopyBackBufferTo(CRhinoDib&);

  bool CopyToBackBuffer(const CRhinoDib&);

  bool CopyDepthBufferTo(CRhinoDib&, 
                         bool          bConvertToImage=false, 
                         bool          bInvertDepthValues=false,
                         CRhinoDib*  pBackgroundImage=nullptr);


  bool GetDepthBuffer(float* zbuffer, 
                      int    left,       int    top, 
                      int    width,      int    height,
                      float* fmin=nullptr,  float* fmax=nullptr);

  bool CopyToFrameBuffer(const CRhinoDib&);

  bool CopyDepthToBackBuffer();


  ////////////////////////////////////
  //  Description:
  //    Saves the current backbuffer to something that can later be retrieved
  //    with the RestoreBackBuffer function.
  //  Remarks:
  //    This does not necessarily save the buffer to m_pFrameBuffer. The OpenGL
  //    implementation may attempt to save the buffer to a texture on the card.
  //
  //  Returns:
  //    true or false indicating success/failure
  bool SaveBackBuffer();

  bool RestoreBackBuffer();

  bool CanSaveDepthBuffer();

  bool SaveDepthBuffer();

  bool RestoreDepthBuffer();

  void FlushPipeline();
  
  bool SupportsShading() const;

  // Description:
  //  Returns true if the pipline is in the middle of drawing a CRhinoInstanceObject and that
  //  instance is completely inside the view frustum
  bool IsEntireActiveInstanceVisible() const;

/////////////////////////////////////////////////////////////////////  
// Pipeline overridable phases...
public:
  bool DrawFrameBuffer(const CDisplayPipelineAttributes& attribs,
                       const ON_Viewport&,
                       bool  updateContents,
                       bool  drawFeedback=true,
                       UINT  enabledConduits=CSupportChannels());

  bool DrawNestedFrame(CDisplayPipelineAttributes* da,
                       CChannelAttributes*         ca,
                       CSupportChannels            channels,
                       int                         nest_level,
                       bool activeViewport);


  virtual
  bool InitFrameBuffer();          
  

  virtual
  bool CalcBoundingBox(ON_BoundingBox&);          
  
  ////////////////////////////////////
  //  Description:
  //    Method to calculate near and far clipping plane values. These
  //    values are very sensitive and can really mess up the display
  //    if set incorrectly...If you're not sure what this is, or you're
  //    not sure what one or more of the values are inside "ClippingInfo",
  //    then it is probably best to just leave this up to Rhino.
  //  
  //  Returns:
  virtual
  bool   CalcClippingPlanes(ClippingInfo&);       
  
  virtual
  bool   SetupFrustum(const ClippingInfo&);          
  
  virtual
  bool   SetupLighting();
  
  virtual
  bool   DrawBackground();           
  
  virtual
  bool   DrawMiddleground();            
  
  virtual
  bool   DrawForeground();           
  
  virtual
  void   DrawOverlays();

  bool   DrawDynamicOverlays();

  virtual
  bool   PostProcessFrameBuffer();
  
  virtual
  bool   ShowFrameBuffer(HDC hDC);
  
  bool   CaptureFrameBuffer(bool bForce=false);
 

  bool   SetDesiredFrameRate(double);
  
  
  ////////////////////////////////////
  //  Description:
  //    Called by the pipeline framework when ever the "current" single
  //    frame rate is greater than the "desired" frame rate.
  //    Note: An internal counter is maintained and incremented every
  //          time current rate > desired rate...then passed into this
  //          routine...
  bool  SetLevelOfDetail(UINT  nLOD);
  
  bool  ResetLevelOfDetail();


  ////////////////////////////////////
  //  Description:
  //    Cleans up any allocated resources used by the pipeline as well
  //    as any display engine that might be attached.
  //
  //  Note: This routine should be able to be called from anywhere except
  //        while "inside" the pipeline. If called inside the pipeline,
  //        this rotuine will do nothing.
  //  
  //  Returns:
  //    false - if resources were not deallocated.
  //    true  - if resources were successfully deallocated.
  bool FreeResources();
  
  bool DrawToDib(CRhinoDib& dib, int width, int height, const CDisplayPipelineAttributes& attrib);

#if defined ON_RUNTIME_WIN
  bool DrawToDC(HDC pDC, int width, int height, const CDisplayPipelineAttributes& attrib);
#endif
   
  ////////////////////////////////////
  //  Description:
  //    This routine will create an exact copy of 'this' pipeline,
  //    and then bind all conduits, that have previously been
  //    bound to this pipeline, to the copied pipeline.
  //
  //  Note:
  //    It probably is not a good idea to use a projection (CRhinoViewport)
  //    that is currently in use by another pipeline since no actions
  //    are taken to preserve the incoming projection.
  //
  //  Returns:
  //    CRhinoDisplayPipeline*  : A pointer to the copied pipeline object.
  //                              It is the caller's responsibility to free
  //                              up the memory allocated by this routine.
  CRhinoDisplayPipeline* ClonePipeline(CRhinoViewport&);
  CRhinoDisplayPipeline* ClonePipeline(CRhinoViewport&, HDC);

  
  void  CopyBindingsFrom(const CRhinoDisplayPipeline&);

  virtual void  CloneFrom(const CRhinoDisplayPipeline&);

  //This function will return 0 if the pipeline was not cloned, otherwise, the original
  //pipeline serial number will be returned here.
  unsigned int ClonedFrom() const { return m_cloned_from_sn; }
  bool IsCloned() const { return 0 != m_cloned_from_sn; }

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Notifier routines used to maintain synchronization between view, viewport, 
  // pipeline, and engine...Note: CRhinoDisplayEngine has equivalent versions
  // of these routines...
  virtual
  bool                  NotifyFrameSizeChanged(int  nWidth, int  nHeight);
  
  virtual
  bool                  NotifyProjectionChanged();
  
  void UpdateClippingFrustum();
  
  ////////////////////////////////////
  //  Description:
  //    This routine will force a complete frame buffer update, ignoring
  //    any and all InDynamicView update optimizations.
  //
  //  Note: A call to this routine will only be valid up until the next
  //        frame buffer update...After that, additional calls will need
  //        to be made for any other forced updates required (ie. This only
  //        works on a per-frame basis).
  void ForceFrameBufferUpdate();
  
  // Added Oct 30, 07 - LW
  // Returns the state of m_bModelXfromIsIdentity
  bool ModelTransformIsIdentity() const;

  void GetTiledFrameInfo(ON_2iSize& fullSize, ON_4iRect& currentTile) const;
  void SetTiledFrameInfo(const ON_2iSize& fullSize, const ON_4iRect& currentTile);

  void DrawDocumentCustomRenderMeshes();
protected:
  // Description:
  //  Sets the camera frustum that this pipeline uses.
  // Parameters:
  //  ON_Viewport: [in] the viewport settings that this pipeline will use as its camera
  // Returns:
  //  true if a valid viewport was successfully assigned to this pipeline.
  bool SetCamera(const ON_Viewport&);

  typedef int (*ObjectSortFunc)(const int*, const int*);
  
  const CRhinoAppSettings&            AppSettings;
  const CRhinoAppAppearanceSettings&  AppearanceSettings;

protected:

  bool                  AttachEngine(CRhinoDisplayEngine*, bool bDeleteExisting = true);
  CRhinoDisplayEngine*  DetachEngine();
  
  virtual
  const ON_ClassId*     EngineClass();
  
  virtual
  bool                  InitializePipeline(); // Called every time the pipeline is opened
                                                  // so that derived pipelines can setup/change
                                                  // things before drawing starts...

  bool                  CreateOnScreenBuffer(int  nW, int  nH);
  
  bool                  InitTolerancesAndDetail();
  
  bool                  PopulateObjectsList();
  void                  InitializeObjectLists();
  
  ON_Color              GetObjectWireColor(const CRhinoObject*, const CRhinoObjectAttributes&) const;
  
  ///////////////////////////////////
  // High level object drawing routines...
  void  DrawGrips();
  void  DrawTransparentObjects();

  void  DrawOrderedObjects(bool back);
  
  ///////////////////////////////////
  // Mid level object drawing routines...
  void  DrawLockedSurfaces();
  void  DrawLockedObjects();
  void  DrawLockedWires();
  void  DrawLockedTextDots();  

  void  DrawNormalSurfaces();
  void  DrawNormalObjects();
  void  DrawNormalWires();
  void  DrawNormalTextDots();  
  
  void  DrawHighlightedSurfaces();
  void  DrawHighlightedPointClouds();
  void  DrawHighlightedObjects();
  void  DrawHighlightedWires();
  void  DrawHighlightedSubWires();
  void  DrawHighlightedTextDots();  

  void  DrawMappingWidgets();

  void  DrawDetailObjects(bool drawActiveDetail, bool drawInactiveDetails);
  
  void  DrawSurfaceGrips();
  void  DrawCurveGrips();
  void  DrawPointCloudGrips();
  
  bool DrawSilhouette(const CRhinoObject*, ON_Color, float thickness, UINT pattern, class CRhMeshEdgeCache* cache);

  //Description:
  //  Creates a plane object that represents the ground plane currently being
  //  used/drawn.
  //Returns:
  //    false   - if there is no ground plane OR the ground plane is turned OFF
  //    true    - if a valid ground plane exists AND it is currently turned ON
  bool  GetGroundPlane(ON_Plane&);

  bool  EnableGroundPlane(bool);
  bool  DrawGroundPlane(void) const;


  ///////////////////////////////////
  // Low level object drawing routines...
  void  DrawFrontTrans(int, DisplayBucket* = nullptr);
  void  DrawBackTrans(int, DisplayBucket* = nullptr);
  
  void  DrawObjectsAsTransparent(DisplayBucket&  objects, DisplayBucket* hilights = nullptr);

  //////////////////////////
  // Description:
  //  Last chance object drawing override...Override this routine if you
  //  want to override the way objects get drawn (if at all)...Note: This
  //  is different than any of the routines above because it is the very
  //  last step before an object actually gets drawn...It doesn't actually
  //  draw anything, unless it's been overridden by some deriving pipeline.  
  //
  // Returns:
  //   FALSE: If the object wasn't drawn and Rhino should continue drawing
  //          the object.
  //   TRUE:  If the object was drawn by some other means and Rhino should
  //          NOT continue drawing the object...OR...if you just don't
  //          want the object drawn.
  //
  // See also: 
  //    IsVisibleFinal()
  virtual
  bool  DrawObjectFinal(const CRhinoObject&) const;
  
  virtual
  void  PreAttributesPass(const CRhinoObject&,
                          CDisplayPipelineAttributes&) const;

  virtual
  void  PostAttributesPass(const CRhinoObject&,
                           CDisplayPipelineAttributes&) const;
  

  ///////////////////////////////////
  // object sorting...
  void  SortLockedSurfaces(ObjectSortFunc = nullptr);
  void  SortLockedObjects(ObjectSortFunc = nullptr);
  void  SortLockedTextDots(ObjectSortFunc = nullptr);

  void  SortNormalSurfaces(ObjectSortFunc = nullptr);
  void  SortNormalObjects(ObjectSortFunc = nullptr);
  void  SortNormalTextDots(ObjectSortFunc = nullptr);
  
  void  SortHighlightedSurfaces(ObjectSortFunc = nullptr);
  void  SortHighlightedObjects(ObjectSortFunc = nullptr);
  void  SortHighlightedSubWires(ObjectSortFunc = nullptr);
  void  SortHighlightedTextDots(ObjectSortFunc = nullptr);

  void  SortTransparentObjects(ObjectSortFunc = nullptr);

  void  PopDisplayAttrs() const;
  void  QuickSort(int a[], int l, int r, ObjectSortFunc  fpSort);
  void  InsertionSort(int a[], int lo0, int hi0, ObjectSortFunc  fpSort);
  
  ///////////////////////////////////////////////////////////////////////////
  // The following "quick" draw routines bypass as much of the display 
  // attribute juggling as possible. Their goal is to simply render a mesh
  // to the backbuffer as fast as possible with total disregard of material
  // or display mode.
  void  QuickDrawSurface(int nObjIdx);
  void  QuickDrawSurfaceBucket(const DisplayBucket&  bucket); 
  void  QuickDrawRenderMeshes();
  void  QuickDrawSolids();

  friend CRhinoObject;

protected:
  void  AddObject(const CRhinoObject*,
                  bool  bColorOverride = false,
                  ON_COMPONENT_INDEX=ON_COMPONENT_INDEX());
                     
  void  DrawObjectBucket(const DisplayBucket& bucket, bool drawWires=true, bool drawSurfaces=false);                    
  void  DrawSurfaceBucket(const DisplayBucket& bucket);
  void  SortBucket(DisplayBucket&, ObjectSortFunc);    
  void  UpdateFrameBufferObjects();               
  void  UpdateOverlayBufferObject(const CRhinoObject*,
                                        bool, int);   
  int   IsTransparentMaterial(const CDisplayPipelineMaterial&) const;                                                                    
  bool  CheckForSubobjectTransparency(const CRhinoObject&,
                                      const CDisplayPipelineAttributes&,
                                      BYTE&);

  void  PushDrawInterrupt();
  void  PopDrawInterrupt();
  void  PopulateRenderedAttrs(CDisplayPipelineAttributes* pAttrs) const;
  //
  //////////////////////////////////////////////////////////////

  void  Construct(CRhinoDoc* pDoc);
   
  bool  InitializeEngine();
  
  //////////////////////////////////////////////////////////////////////////
  // Wrapper functions for creating engines with specified projection and
  // render target.
  CRhinoDisplayEngine* CreateEngine();
  CRhinoDisplayEngine* CreateEngine(HWND);
#if !defined RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE
  CRhinoDisplayEngine* CreateEngine(HDC);
#endif

  //////////////////////////////////////////////////////////////////////////
  // The following routines are "wrapper" routines around the virtual
  // ones above. These versions call into their overridden counterparts,
  // but then also process any/all registered conduits. This was done
  // so that the overriding routines don't need to worry about how or when
  // to do conduit processing...
  
  bool  iInitFrameBuffer();          
  
  bool  iSetupLighting();            
  
  bool  iObjectCulling();            
  
  bool  iMeshObjects();
  
  bool  iCalcBoundingBox();          
  
  bool  iCalcClippingPlanes();       
  
  bool  iSetupFrustum();             
    
  bool  iDrawBackground();       
  
  bool  iDrawMiddleground();         
  
  bool  iDrawForeground();           
  
  bool  iDrawFeedback(bool bFrameBufferUpdated);
  
  bool  iDrawOverlays();
  
  bool  iPostProcessFrameBuffer();   
  
  //////////////////////////////////////////////////////////////////////////
  
  // Frame buffer creation and manipulation routines...
  bool  CreateFrameBuffer();   // called inside of OpenPipeline()...

  bool  UpdateObjectLists();
    
  
  // Helper function used to iterate through and process certain types
  // of conduits...  
  bool  ProcessConduits(ConduitList& list, UINT active_channel, bool* terminate = nullptr) const;
  void  NotifyConduits(EConduitNotifiers);
  
  bool  DoPreDrawMiddleground();
  bool  DoPreDrawObjects();
  bool  DoPreDrawTransparentObjects();
  bool  DoPostDrawObjects();

  //////////////////////////////////////////////////////////////////////////
  // dynamic clipping section display...
  void  ResetClippingSections();
  void  UpdateClippingSections(const CRhinoObject*);
  bool  UpdateClippingSections(int);
  void  DrawClippingSections();

  //////////////////////////////////////////////////////////////////////////
  // sorting and comparing routines...
  static CRhinoDisplayPipeline*  m_pSortThis;
  
  static int compareDrawOrder(const int* nA, const int* nB);
  static int compareBackToFrontOrder(const int* nA, const int* nB);
  static int compareFrontToBackOrder(const int* nA, const int* nB);
  static int compareSurfaceOrder(const int* nA, const int* nB);

private:
  // added in V8
  void DrawClippingSections(int objectIndex, bool fill, bool wires);
  bool PrepareShadows();
protected:
  // Description:
  //  Called before an object is drawn. This function is responsible for determining if the object
  //  is visible and sets up m_pDisplayAttrs with attributes for this object
  // Returns:
  //  true if the display should continue attempting to draw the object this frame
  //  false if an error occurred or the object is determined to not be visible
  bool PreObjectDrawing( int dpo_index, bool skipAttributeSetup=false );
  void PostObjectDrawing( int dpo_index );
  bool ProcessConduits(ConduitList& list, UINT active_channel, int dpo_index) const;

  void InitializeEnvironments();

protected:
  // This is an expensive function call and should be rarely called during a frame draw
  void PushPipelineObjectAttributes( int dpo_index ) const;

  bool IsVisible(const class CDisplayPipelineObject& dpo) const;

  void AddObject( int dpo_index,
                  bool bColorOverride, // (default should be false) Override object's color using the display attrs...
                  bool bForcedDeposit);// (default should be false) Force the object to get drawn idependent of state...
                                  
  void AddToBuckets( int dpo_index,
                     int   IsHighlighted,
                     bool  bColorOverride,
                     bool  bForceDeposit);

  void ProcessShadedAnalysisMode( const CRhinoObject* pObject );

  void RepurposeAlphaValues(CRhinoDib& dib, const CDisplayPipelineAttributes*);

  void SetCachedAttributesValid(bool valid);
  bool CachedAttributesValid() const;
  CDisplayPipelineAttributes& CachedAttributes();

  void EnableMaterialCaching(bool bEnable);
  bool MaterialCachingEnabled(void) const;

public:
  //The layout information is used when the pipeline is being sent to
  //a high-resolution image (for printing or file save)
  int m_nLayoutWidth = -1;   //Width of high-res display (-1 if not in high-res operation)
  int m_nLayoutHeight = -1;  //Height of high-res display (-1 if not in high-res operation)
  ON_4iRect m_layout_tile;  //Tiled position on high-res display

  bool                  m_bPipelineOpened;
  bool                  m_bInOverlay;
  bool                  m_bCaptureFrameBuffer;
  bool                  m_bDrawGrayScale;

  bool                  m_bInterrupted;

  int                   m_nOverlaySortIndex;

  CRhinoDib*          m_pFrameBuffer;    // Buffer containing final rendered image...
  CRhinoDib*          m_pOffscreenBuffer;// Buffer used to render frames offscreen...

  CRhinoDoc*            m_pDoc;         // Always set = to RhinoApp().ObsoleteActiveDoc() 
                                        // when pipeline is opened...

  ON_Viewport           m_Camera;       // Set by DrawFramebuffer or SetCamera...

  mutable CChannelAttributes*           m_pChannelAttrs;
  mutable CDisplayPipelineAttributes*   m_pDisplayAttrs;

  /////////////////////////////////////////////////////////////////
  //
  // Bounding box of visible objects and near and far 
  // clipping planes
  //
  // At the beginning of each frame, m_Clipping contains
  // no information.
  //
  // Bounding box of visible objects:
  //   In CRhinoDisplayPipeline::iCalcBoundingBox(), the
  //   active channel is set to SC_CALCBOUNDINGBOX and
  //   CRhinoDisplayPipeline::CalcBoundingBox() examines the
  //   document and sets m_Clipping.bbox.  Then the conduits
  //   for the SC_CALCBOUNDINGBOX channel are called.have the 
  //   opportunity to set ChannelAttributes::m_BoundingBox and 
  //   ChannelAttributes::m_bUseExclusiveBBox. If conduits set 
  //   ChannelAttributes::m_BoundingBox to a  valid box, then
  //   it is  unioned with m_Clipping.bbox (if m_bUseExclusiveBBox
  //   is false) or replaces m_Clipping.bbox (if m_bUseExclusiveBBox
  //   is true).
  //
  // Clipping planes:
  //  After the m_Clipping.bbox is set, the near and far clipping
  //  planes are calculated in CRhinoDisplayPipeline::iCalcClippingPlanes().
  //  
  //  
  // the value of ChannelAttributes::m_bUseExclusiveBBox
  // In CRhinoDisplayPipeline::iCalcClippingPlanes, m_Clipping 
  // is initialized and then the virtual function
  // CRhinoDisplayPipeline::CalcClippingPlanes() is called.
  // During the call, the near and far clipping planes can 
  // be adjusted as needed.
  ClippingInfo          m_Clipping;
  //
  // m_SavedClipping is used internally.  Never modify the
  // contents of m_SavedClipping.
  ClippingInfo          m_SavedClipping;

  int                   m_nDisplayWidth;
  int                   m_nDisplayHeight;

  UINT                  m_nLOD;         // Level-Of-Detail...
  UINT                  m_nDegradeLevel;
  double                m_dDesiredFrameRate;
  UINT                  m_nFrameStartTime;
  UINT                  m_nFrameTimeEstimate;

  bool                  m_bShadingRequired;

  bool                  m_bForceCompleteFrameUpdate; // Single pass, single frame flag
                                                     // indicating that all dynamic view
                                                     // update optimizations should be
                                                     // ignored...

  bool                  m_bEntireActiveInstanceVisible; // should only be true when inside instance drawing
                                                        // and the parent instance is entirely inside the view frustum
  int                   m_nInstanceNestLevel;
  bool                  m_bForcedVisibility;
  bool                  m_bSkipOrderedObjects;

protected:
  /////////////////////////////////////////////////////////////////////
  // Pipeline attributes...
  bool    m_bRedraw;
  bool    m_bNeedsCapture;
  bool    m_bBuildObjectLists;
  bool    m_bDrawingWires;
  bool    m_bDrawingGrips;
  bool    m_bDrawingSurfaces;

  bool    m_bFreezeDrawing;

  bool    m_bIsOnScreenBuffer;
  bool    m_bIsPrinting;

  class CWnd* m_pWnd; // Different types of render targets...
  HDC     m_pDC;            // These are mutually exclusive...

  struct DrawInterruptItem
  {
    UINT   m_nFrameStartTime;
    UINT   m_nFrameTimeEstimate;
  };

  UINT    m_nSingleFrameRate;
  UINT    m_nPrevFrameRate;
  UINT    m_nDesiredSingleFrameRate;
  UINT    m_nAverageFrameRate;
  UINT    m_nFrameCount;

  ON_SimpleArray<DrawInterruptItem> m_InterruptStack;

  int                   m_nRenderPass;

  EDepthMode            m_eDepthMode;
  EZBiasMode            m_eZBiasMode;

  CSupportChannels      m_EnabledChannels;

  int                   m_nest_level;

  CRhinoDisplayEngine*  m_pEngine;

  //////////////////////////////////////////////////////////////
  //
  class CDisplayPipelineDrawList* m_pDrawList;   // points to m_pFBDrawList or m_pOBDrawList
  class CDisplayPipelineDrawList* m_pFBDrawList; // "frame buffer" draw list (main list all objects are in)
  class CDisplayPipelineDrawList* m_pOBDrawList; // "overlay buffer" draw list (populated iDrawOverlays)

  mutable ON_SimpleArray<ON_Color>                    m_ObjectColorStack;
  mutable ON_ClassArray<CDisplayPipelineAttributes>   m_DisplayAttrsStack;
  mutable ON_ClassArray<ON_Viewport>                  m_ProjectionStack;

  ON_SimpleArray<class CDisplayMeshingObject>*      m_pRenderMeshes;

  CRhinoDib*  m_pSelectionMap;
  ObjectArray   m_ObjectMapList;
  ObjectArray   m_CustomDrawList;


  ON_UUID   m_DeferredDisplayMode;

  bool      m_bInSelectionMask;
public:
  bool      m_bInZBufferCapture;
protected:
  bool      m_bEnvironmentEnabled;
  bool      m_bIgnoreLockedState;

  bool m_bInQuickDrawSurface = false;
  PFDRAWOVERRIDEPROC  m_pfDrawChannelsOverride;
  friend bool  DrawChannelsOverride(CRhinoDisplayPipeline& dp);

  const class CRhinoDetailViewObject*  m_pCurrentDetailObject = nullptr;

public:
  bool UseImageBasedLighting() const;
  void SetUseImageBasedLighting(bool b);

  bool DrawingHighlightedObjects(void) const;
  bool DrawingLockedObjects(void) const;

public:
  bool TechPreProcessingObjects(void) const;
  bool TechPreProcessingVisibleMeshes(void) const;
  bool TechDrawingHiddenLines(void) const;

  const ON_Mesh* GetGroundPlaneMesh(void) const;
  
protected:
  bool m_bUseImageBasedLighting = false;

  friend class CRhPerFrameCache;
  mutable class CRhPerFrameCache* m_pPerFrameCache = nullptr;
  friend class CRhDisplayPipelinePrivate;
  mutable class CRhDisplayPipelinePrivate* m_private = nullptr;

private:
  void CleanPreviousRenderTarget();
  bool InternalSetRenderTarget(class CWnd*);

  void PreShadedMeshSetup(void) const;
  void PostShadedMeshSetup(void) const;

public:
  //////////////////////////
  // Description:
  //  Checks whether the calculated pixel offset error is greater than the
  //  given 'pixel_offset'. 'pixel_offset' is expressed in pixel units,
  //  where a value of 1.0 represents the width of a pixel.
  //
  // Returns:
  //   False: If the display precision error is smaller than given value.
  //   True:  If the display precision error is larger than the given value.
  bool IsDisplayPrecisionErrorGreaterThan(double pixel_offset) const;


  friend class CRhinoViewport;
  friend class CPipelineMeshingThread;
  friend class CRhinoDetailViewObject;
  friend class CRhinoInstanceObject;
  friend class CDisplayPipelineDrawList;
  friend class CRhinoDisplayPipelineRef;
  friend class CRhinoDisplayConduit;
  friend class CRhinoDisplayEngine;
  friend class CRhGLSLRenderer;
  friend class CRhDisplayEnginePrivate;
  friend class CRhDisplayEnginePrivate_Metal;
  friend class CRhDisplayEnginePrivate_OGL;
};


/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  
struct RegisteredPipeline
{
public:
  bool              m_bSupportsWireframe;
  bool              m_bSupportsShading;
  bool              m_bDotNetPipeline;
  ON_UUID           m_uuid;
  const ON_ClassId*    m_pClass;
  ON_wString        m_sDescription;
};

typedef ON_ClassArray<RegisteredPipeline>   RegisteredPipelineList;

struct RealtimeDisplayMode
{
public:
  ON_UUID           m_uuid;
  ON_wString        m_sDescription;
};
typedef ON_ClassArray<RealtimeDisplayMode>   RealtimeDisplayModeList;

RHINO_SDK_FUNCTION 
bool RegisterDisplayPipeline(const ON_ClassId*, const ON_UUID&, const ON_wString&, bool=true, bool=false, bool=false);

RHINO_SDK_FUNCTION 
bool UnregisterDisplayPipeline(const ON_ClassId*);

RHINO_SDK_FUNCTION 
bool FindRegisteredPipeline(const ON_ClassId*, RegisteredPipeline&);

RHINO_SDK_FUNCTION 
bool FindRegisteredPipeline(const ON_UUID&, RegisteredPipeline&);

RHINO_SDK_FUNCTION 
int  GetRegisteredPipelines(RegisteredPipelineList&);

RHINO_SDK_FUNCTION 
int  GetRegisteredWireframePipelines(RegisteredPipelineList&);

RHINO_SDK_FUNCTION 
int  GetRegisteredShadingPipelines(RegisteredPipelineList&);

//RHINO_SDK_FUNCTION 
//bool RegisterDefaultWireframePipeline(const ON_ClassId*);
//
//RHINO_SDK_FUNCTION 
//bool RegisterDefaultShadingPipeline(const ON_ClassId*);
//
//RHINO_SDK_FUNCTION 
//bool RegisterDefaultPageViewPipeline(const ON_ClassId*);

RHINO_SDK_FUNCTION 
const ON_ClassId* GetDefaultWireframePipeline();

RHINO_SDK_FUNCTION 
const ON_ClassId* GetDefaultShadingPipeline();

RHINO_SDK_FUNCTION 
const ON_ClassId* GetDefaultPageViewPipeline();

RHINO_SDK_FUNCTION 
CRhinoDisplayPipeline* CreateDisplayPipeline(const ON_ClassId*);

/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////  

class CColorVec : public ON_4fPoint
{
public:
  CColorVec(ON_Color c) :
    ON_4fPoint((float)c.FractionRed(), (float)c.FractionGreen(), (float)c.FractionBlue(), (float)(1.0-c.FractionAlpha()))
  {}

  CColorVec(ON_Color c, float a) :
    ON_4fPoint((float)c.FractionRed(), (float)c.FractionGreen(), (float)c.FractionBlue(), a)
  {}

#if defined (ON_RUNTIME_WIN)
  CColorVec(COLORREF c, float a=1.0f) :
    ON_4fPoint( GetRValue(c)/255.0f, GetGValue(c)/255.0f, GetBValue(c)/255.0f, a )
  {}
#endif

  CColorVec(RGBQUAD c) :
    ON_4fPoint( c.rgbRed/255.0f, c.rgbGreen/255.0f, c.rgbBlue/255.0f, c.rgbReserved/255.0f )
  {}

  CColorVec(int  R, int  G, int  B, int  A=255) :
    ON_4fPoint( R/255.0f, G/255.0f, B/255.0f, A/255.0f )
  {}
    
  CColorVec(float  R, float  G, float  B, float  A=1.0f) :
    ON_4fPoint( R, G, B, A )
  {}

  CColorVec(float  c) :
    ON_4fPoint( c, c, c, 1.0 )
  {}
  
  CColorVec(const ON_4fPoint& pt) :
    ON_4fPoint( pt )
  {
  }
  
  int       Red()   const     { return (int)(x*255); }
  int       Green() const     { return (int)(y*255); }
  int       Blue()  const     { return (int)(z*255); }
  int       Alpha() const     { return (int)(w*255); }
  
  float     Redf()   const    { return x; }
  float     Greenf() const    { return y; }
  float     Bluef()  const    { return z; }
  float     Alphaf() const    { return w; }

#if defined(ON_DEBUG)
  // deprecate only in debug while we work on replacing calls
  ON_DEPRECATED_MSG("Use ToON_Color() function. This function does not properly convert alpha")
#endif
  operator  ON_Color() const
  {
    return ON_Color( (int)(x*255), (int)(y*255), (int)(z*255) );
  }
  
  ON_Color ToON_Color() const
  {
    float alpha = 1.0f-w;
    return ON_Color( (int)(x*255), (int)(y*255), (int)(z*255), (int)(alpha*255) );
  }

#if defined (ON_RUNTIME_WIN)
  operator COLORREF() const
  {
    return RGB(x*255,y*255,z*255);
  }
#endif
  
  CColorVec&  operator=(float c)
  {
    x = y = z = c;
    w = 1.0f;
    return *this;
  }
  
  CColorVec&  operator=(BYTE c)
  {
    x = y = z = c;
    w = 1.0f;
    return *this;
  }
  
  CColorVec&  operator=(UINT c)
  {
    x = (c&0xff) / 255.0f;
    y = ((c>>8)&0xff) / 255.0f;
    z = ((c>>16)&0xff) / 255.0f;
    w = ((c>>24)&0xff) / 255.0f;
    return *this;
  }
  
  CColorVec&  operator=(const ON_Color& c)
  {
    x = (float)c.FractionRed();
    y = (float)c.FractionGreen();
    z = (float)c.FractionBlue();
    w = (float)c.FractionAlpha();
    return *this;
  }
  
  #if defined (ON_RUNTIME_WIN)
  CColorVec&  operator=(COLORREF c)
  {
    x = (c&0xff) / 255.0f;
    y = ((c>>8)&0xff) / 255.0f;
    z = ((c>>16)&0xff) / 255.0f;
    w = ((c>>24)&0xff) / 255.0f;
    return *this;
  }
  #endif
  
  CColorVec&  operator=(const ON_4fPoint& p)
  {
    ON_4fPoint::operator =( p );
    return *this;
  }
  
  CColorVec  operator+(const CColorVec& p) const
  {
    CColorVec q( x, y, z );
    
    q.x += p.x;
    q.y += p.y;
    q.z += p.z;
    
    return q;
  }
  
  CColorVec  operator-(const CColorVec& p) const
  {
    CColorVec q( x, y, z );
    
    q.x -= p.x;
    q.y -= p.y;
    q.z -= p.z;
    
    return q;
  }
  
  CColorVec  operator*(const CColorVec& p) const
  {
    CColorVec q( x, y, z );
    
    q.x *= p.x;
    q.y *= p.y;
    q.z *= p.z;
    
    return q;
  }

  CColorVec  operator*(float s) const
  {
    CColorVec q( x, y, z );
    
    q.x *= s;
    q.y *= s;
    q.z *= s;
    
    return q;
  }
  
  CColorVec  operator+(const CColorVec& p)
  {
    CColorVec q( x, y, z );
    
    q.x += p.x;
    q.y += p.y;
    q.z += p.z;
    
    return q;
  }
  
  CColorVec  operator-(const CColorVec& p)
  {
    CColorVec q( x, y, z );
    
    q.x -= p.x;
    q.y -= p.y;
    q.z -= p.z;
    
    return q;
  }
  
  CColorVec  operator*(const CColorVec& p)
  {
    CColorVec q( x, y, z );
    
    q.x *= p.x;
    q.y *= p.y;
    q.z *= p.z;
    
    return q;
  }

  CColorVec  operator*(float s)
  {
    CColorVec q( x, y, z );
    
    q.x *= s;
    q.y *= s;
    q.z *= s;
    
    return q;
  }
  
};

