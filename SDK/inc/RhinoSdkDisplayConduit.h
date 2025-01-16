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

// Specialiazed UINT class that ensures "supported" values only...
//
// The display pipeline calls conduits during specific phases of drawing (channels)
// During the drawing of a single frame the channels are called in the following order
//
// [Begin Drawing of a Frame]
//   SC_CALCBOUNDINGBOX
//   SC_CALCCLIPPINGPLANES
//   SC_SETUPFRUSTUM
//   SC_SETUPLIGHTING
//   SC_INITFRAMEBUFFER
//   SC_DRAWBACKGROUND
//   If this is a layout and detail objects exist the channels are called in the
//   same order for each detail object (drawn as a nested viewport)
//   SC_PREDRAWMIDDLEGROUND
//   SC_PREDRAWOBJECTS
//
//   For Each Visible Non Highlighted Object
//     SC_OBJECTDISPLAYATTRS
//     SC_PREOBJECTDRAW
//     SC_DRAWOBJECT
//     SC_POSTOBJECTDRAW
//   Next Object
//
//   Here Rhino draws highlighted objects
//
//   SC_PREDRAWTRANSPARENTOBJECTS
//
//   Here Rhino draws transparent objects (sorted)
//
//   SC_POSTDRAWOBJECTS
//   SC_DRAWFOREGROUND
//
//   For Each Visible Highlighted Object
//     SC_OBJECTDISPLAYATTRS
//     SC_PREOBJECTDRAW
//     SC_DRAWOBJECT
//     SC_POSTOBJECTDRAW
//   Next Object
//
//   SC_POSTPROCESSFRAMEBUFFER - If a conduit exists that requires this
//   SC_DRAWOVERLAY - If Rhino is in a feedback mode
// [End of Drawing of a Frame]
//
// NOTE: There may be multiple SC_DRAWOBJECT calls for a single object. An example of when this could
//       happen would be with a shaded sphere. The shaded mesh is first drawn and these channels would
//       be processed; then at a later time the isocurves for the sphere would be drawn.
struct CSupportChannels
{
  const static UINT SC_INITFRAMEBUFFER           = 0x00000001;
  const static UINT SC_SETUPFRUSTUM              = 0x00000002;
  const static UINT SC_OBJECTCULLING             = 0x00000004;
  const static UINT SC_CALCBOUNDINGBOX           = 0x00000008;
  const static UINT SC_CALCCLIPPINGPLANES        = 0x00000010;
  const static UINT SC_SETUPLIGHTING             = 0x00000020;
  const static UINT SC_DRAWBACKGROUND            = 0x00000040;
  const static UINT SC_PREDRAWOBJECTS            = 0x00000080;
  const static UINT SC_DRAWOBJECT                = 0x00000100;
  const static UINT SC_POSTDRAWOBJECTS           = 0x00000200;
  const static UINT SC_DRAWFOREGROUND            = 0x00000400;
  const static UINT SC_DRAWOVERLAY               = 0x00000800;
  const static UINT SC_POSTPROCESSFRAMEBUFFER    = 0x00001000;
  const static UINT SC_MESHINGPARAMETERS         = 0x00002000;
  const static UINT SC_OBJECTDISPLAYATTRS        = 0x00004000;
  const static UINT SC_PREOBJECTDRAW             = 0x00008000;
  const static UINT SC_POSTOBJECTDRAW            = 0x00010000;
  const static UINT SC_VIEWEXTENTS               = 0x00020000;
  const static UINT SC_PREDRAWMIDDLEGROUND       = 0x00040000;
  const static UINT SC_PREDRAWTRANSPARENTOBJECTS = 0x00080000;
  const static UINT SC_CUSTOMRENDEROBJECTS       = 0x00100000;
/*
  const static UINT SC_RESERVED_CHANNEL       = 0x00200000;
  const static UINT SC_RESERVED_CHANNEL       = 0x00400000;
  const static UINT SC_RESERVED_CHANNEL       = 0x00800000;
  const static UINT SC_RESERVED_CHANNEL       = 0x01000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x02000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x04000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x08000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x10000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x20000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x40000000;
  const static UINT SC_RESERVED_CHANNEL       = 0x80000000;
*/
  
  // Helper masks that make some things a little easier...
  // Note: These are not channels, but a combination of
  //       different channels...You should always use the
  //       specific channels above when defining or referring
  //       to channel values.
  const static UINT SC_DRAWMIDDLEGROUND       = SC_PREDRAWMIDDLEGROUND|SC_PREDRAWOBJECTS|SC_DRAWOBJECT|SC_POSTDRAWOBJECTS|SC_CUSTOMRENDEROBJECTS;
  const static UINT SC_OBJECT_BASED_CHANNEL   = SC_OBJECTCULLING|SC_DRAWOBJECT|SC_OBJECTDISPLAYATTRS|SC_PREOBJECTDRAW|SC_POSTOBJECTDRAW;
  const static UINT SC_ALL_DRAW_CHANNELS      = SC_PREDRAWOBJECTS|SC_DRAWOBJECT|SC_POSTDRAWOBJECTS|SC_PREOBJECTDRAW|SC_POSTOBJECTDRAW|SC_CUSTOMRENDEROBJECTS;
  
  
  #define CHANNELS_SUPPORTED                    0x001FFFFF

  CSupportChannels(UINT channels=CHANNELS_SUPPORTED) : 
    m_Channels(channels & CHANNELS_SUPPORTED) {}

public:
  bool              Supports(UINT ch) const    { return ((m_Channels & ch) != 0); }
                    operator UINT() const  { return m_Channels; }
  CSupportChannels& operator=(UINT ch)         { m_Channels = (ch & CHANNELS_SUPPORTED); return *this; }
                
private:  
  UINT              m_Channels;
}; 


////////////////////////////////////////////
// Conduit notification values...
enum EConduitNotifiers
{
  CN_PIPELINEADDED      = 1,
  CN_PIPELINEDELETED,
  CN_PIPELINECLONED,
  CN_PIPELINEOPENED,
  CN_PIPELINECLOSED,
  CN_FRAMESIZECHANGED,
  CN_PROJECTIONCHANGED,
  CN_NESTEDPIPELINE_OPENED,
  CN_NESTEDPIPELINE_CLOSED
};


//////////////////////////////////////////////////////////////////////////////////
// Display conduits provide support for adding custom display functionality during
// drawing inside of a display pipeline/viewport.
//
// A subclass of a conduit specifies what channels or "phases" of drawing that it
// wants to be involved in.
// 
// Conduits may be "bound" to specific display pipelines or viewports which means
// that they will only operate with the objects they are bound to. If a binding is
// not defined, the conduit will operate in all display pipelines.
class RHINO_SDK_CLASS CRhinoDisplayConduit
{
  const unsigned int m_display_conduit_sn;

public:
  /*
  Parameters:
    display_conduit_sn - [in]
  Returns:
    If display_conduit_sn identifies an existing CRhinoDisplayConduit, then a pointer
    to that conduit is returned.
    Otherwise, null is returned.
  */
  static CRhinoDisplayConduit* FromRuntimeSerialNumber(
    unsigned int display_conduit_sn
    );

  /*
  Parameters:
    display_conduit - [in]
  Returns:
    True if the display_conduit pointer is null or points to
    an existing CRhinoDisplayConduit.
  */
  static bool IsValidDisplayConduitPointer(
    const void* display_conduit
    );

  /*
  Parameters:
    display_conduit - [in]
  Returns:
    True if display_conduit refers to an existing CRhinoDisplayConduit.
  */
  static bool IsValidDisplayConduitReference(
    const CRhinoDisplayConduit& display_conduit
    );

  // Description:
  //   Get the uniuqe runtime serial number for a given display conduit
  static unsigned int RuntimeSerialNumber(const CRhinoDisplayConduit* conduit);

public:
  // Parameters:
  //   channel_mask: [in] Set of CSupportChannels that this conduit is interested in.
  //   enable: [in] Should the conduit initially be enabled?
  // Remarks:
  //   Do not use SC_POSTPROCESSFRAMEBUFFER unless you absolutely need to. The
  //   display pipeline will check to see if it has any conduits that require this
  //   phase of drawing and will only perform a postprocess if one exists. Post
  //   processing of the frame buffer requires multiple dib copies at the end of
  //   drawing a frame and will cause a *constant* performance hit on display speed.
  //
  //   Since most of the time a conduit needs to only implement 1 to 2 channels at
  //   at time, it is also recommended that you do NOT use SC_ALL_CHANNELS to construct
  //   your conduit...This will make your counduit (and the pipeline) very inefficient.
  
  // Example:
  //   If you had a conduit subclass that wanted to pay attention to object drawing
  //   and foreground drawing, you would set up the constructor as follows
  //
  //   CMyDisplayConduit::CMyDisplayConduit()
  //   : CRhinoDisplayConduit( CSupportChannels::SC_DRAWOBJECT | CSupportChannels::SC_DRAWFOREGROUND )
  //   {
  //     ...set up some other class variables
  //   }
  //ON_DEPRECATED - removing deprecation for the time being so we can find and clean up other outstanding warnings
  CRhinoDisplayConduit(CSupportChannels channel_mask, bool enable);
  CRhinoDisplayConduit(CSupportChannels channel_mask);
  
  virtual ~CRhinoDisplayConduit();

// Attributes...
public:
  // Parameters:
  //   channel: [in] CSupportChannel or a list of CSupportChannels that are or'd together
  // Returns:
  //   True if this conduit supports given channel or list of channels. The supported
  //   channels are the ones that were defined in the conduit's constructor
  bool ChannelSupported(UINT channel) const;

// Operations...
public:
  // Description:
  //   Conduits should/could be thought of as small pieces of code that get "executed" 
  //   inside of Rhino's pipeline...and by specifying which "channel" your conduit 
  //   executes in, allows you to determine where and when your code runs.
  //
  //   This function is called for every CSupportChannel that this conduit has been
  //   constructed with during the process of drawing a single frame. ExecConduit is not
  //   called when the conduit is disabled.
  //
  // Parameters:
  //   pipeline:  [in] the pipeline that is currently executing and calling this conduit.
  //              This is the pipeline you should be calling "Draw" functions on if you
  //              want to perform any custom drawing.
  //   channel:   [in] This function can be called multiple times by a pipeline while
  //              drawing a single frame. The channel parameter specifies what part of
  //              CSupportChannels is currently being executed
  //   terminate: [out] Set this to true if you don't want other conduits to perform
  //              operations in the current channel after this function returns. Typically
  //              you will leave this parameter alone and allow other enabled conduits to
  //              perform their custom drawing code.
  // Returns:
  //   True if the function completed successfully. If this function returns false,
  //   something bad occurred and drawing of the current frame stops.
  virtual bool ExecConduit( CRhinoDisplayPipeline& pipeline,
                            UINT channel,
                            bool& terminate ) = 0;             

  // Description:
  //   Turn this conduit on. When a conduit is enabled, it's ExecConduit function is called
  //   for each viewport that this conduit pays attention to.
  //ON_DEPRECATED - removing deprecation for the time being so we can find and clean up other outstanding warnings
  void Enable();
  void Enable(unsigned int docSerialNumber);

  // Description:
  //   Turn this conduit off. When a conduit is disabled, it will not participate in any
  ///  custom drawing activity.
  void Disable();
  
  bool IsEnabled() const;
  bool IsEnabled(unsigned int docSerialNumber) const;

  /*
  Returns:
    The document that the conduit is working with at the time of ExecConduit.
  Remarks:
    The returned pointer may be null.
  */
  unsigned int DocumentSerialNumber() const;
  class CRhinoDoc* Document() const;

  /*
  Returns:
    The view that the conduit is working with at the time of ExecConduit.
  Remarks:
    The returned pointer may be null.
  */
  unsigned int ViewSerialNumber() const;
  class CRhinoView* View() const;

  /*
  Returns:
    The page view that the conduit is working with at the time of ExecConduit.
  Remarks:
    The returned pointer may be null.
  */
  unsigned int PageViewSerialNumber() const;
  class CRhinoPageView* PageView() const;

  /*
  Returns:
    The viewport that the conduit is working with at the time of ExecConduit.
  Remarks:
    The returned pointer may be null.
  */
  unsigned int ViewportSerialNumber() const;
  class CRhinoViewport* Viewport() const;
  
// Notifications...
public:
  // Description:
  //   Display pipelines will call this function when any of the actions defined in
  //   EConduitNotifiers occurs. This function is typically not overridden, but may
  //   be useful if you need to adjust internal data when different pipeline events
  //   occur.
  // Parameters:
  //   notification_event: [in] the event reported by the display pipeline
  //   pipeline: [in] the display pipeline calling this conduit with the event
  //   notification.
  virtual void NotifyConduit(EConduitNotifiers notification_event,
                             CRhinoDisplayPipeline& pipeline);
  
// Binding methods...
public:
  // Description:
  //   Bind this conduit to a display pipeline/viewport. If this conduit is already
  //   bound to another pipeline or viewport, that binding will not be removed and
  //   this conduit will be bound to multiple pipelines/viewports
  // Returns:
  //   false if this conduit is already bound to the provided pipeline/viewport
  bool Bind(const CRhinoDisplayPipeline& pipeline);

  // See: bool CRhinoDisplayConduit::Bind(const CRhinoDisplayPipeline& pipeline);
  bool Bind(const CRhinoViewport& viewport);
  // See: bool CRhinoDisplayConduit::Bind(const CRhinoDisplayPipeline& pipeline);
  bool Bind(const CRhinoView& view);
  // See: bool CRhinoDisplayConduit::Bind(const CRhinoDisplayPipeline& pipeline);
  bool Bind(const ON_Viewport& viewport);
  
  // Description:
  //   Remove a binding from a display pipeline/viewport.
  // Returns:
  //   false if this conduit was not bound to the provided pipeline/viewport
  bool Unbind(const CRhinoDisplayPipeline& pipeline);

  // See: bool CRhinoDisplayConduit::Unbind(const CRhinoDisplayPipeline& pipeline);
  bool Unbind(const CRhinoViewport& viewport);
  // See: bool CRhinoDisplayConduit::Unbind(const CRhinoDisplayPipeline& pipeline);
  bool Unbind(const CRhinoView& view);
  // See: bool CRhinoDisplayConduit::Unbind(const CRhinoDisplayPipeline& pipeline);
  bool Unbind(const ON_Viewport& viewport);

  // Description:
  //   When a conduit runs inside of a pipeline, it is said to be "bound" to that 
  //   pipeline. By default all conduits are bound to all existing pipelines and any
  //   newly created ones...This is known as "constant binding"...If a conduit is
  //   only supposed to run/execute inside a specific pipeline/viewport (or set of
  //   viewports) then constant binding must be disabled, and the conduit must then
  //   be manually "bound" to a specific pipeline/viewport. See comments on all of
  //   "Bind()" methods.
  //
  //   Note: You can still bind conduits to specific pipelines/viewports, but the
  //         conduit will have to manually check for bindings inside of "ExecConduit()"
  //         using the "IsBound()" methods. By disabling constant binding and manually
  //         binding your conduit, will force your conduit to only run in specific
  //         pipelines/viewports (ie. no checks are necessary, they're handled by the
  //         pipeline).
  //
  // Parameters:
  //   enable: [in] Set to false to disable constant binding, true to enable it.
  void EnableConstantBinding(bool enable = true);

  // Description:
  //  Set this conduit to not be bound to any specific display pipeline or viewport
  void UnbindAll();

  // Description:
  //  Flip binding state for a given pipeline / viewport
  // Returns:
  //  Always returns true
  bool ToggleBinding(const CRhinoDisplayPipeline& pipeline);
  // See: bool CRhinoDisplayConduit::ToggleBinding(const CRhinoDisplayPipeline& pipeline);
  bool ToggleBinding(const CRhinoViewport& viewport);
  // See: bool CRhinoDisplayConduit::ToggleBinding(const CRhinoDisplayPipeline& pipeline);
  bool ToggleBinding(const CRhinoView& view);
  // See: bool CRhinoDisplayConduit::ToggleBinding(const CRhinoDisplayPipeline& pipeline);
  bool ToggleBinding(const ON_Viewport& viewport);

  // Description:
  //  Remove the binding for a pipeline or viewport and add a binding for a new
  //  pipeline or viewport
  // Parameters:
  //  old_pipeline: [in] the pipeline that this conduit is removing a binding from
  //  new_pipeline: [in] the pipeline that this conduit is adding a binding to
  // Returns:
  //  false if this conduit was not already bound to the pipeline or viewport that
  //  it is trying to replace
  bool ReplaceBinding(const CRhinoDisplayPipeline& old_pipeline,
                      const CRhinoDisplayPipeline& new_pipeline);
  // See: bool CRhinoDisplayConduit::ReplaceBinding(const CRhinoDisplayPipeline& old_pipeline, const CRhinoDisplayPipeline& new_pipeline);
  bool ReplaceBinding(const CRhinoViewport& old_viewport,
                      const CRhinoViewport& new_viewport);
  // See: bool CRhinoDisplayConduit::ReplaceBinding(const CRhinoDisplayPipeline& old_pipeline, const CRhinoDisplayPipeline& new_pipeline);
  bool ReplaceBinding(const CRhinoView& old_view,
                      const CRhinoView& new_view);
  // See: bool CRhinoDisplayConduit::ReplaceBinding(const CRhinoDisplayPipeline& old_pipeline, const CRhinoDisplayPipeline& new_pipeline);
  bool ReplaceBinding(const ON_Viewport& old_viewport,
                      const ON_Viewport& new_viewport);

  // Description:
  //  Determines if this conduit is bound to a specific pipeline or viewport
  // Parameters:
  //  pipeline: [in] the pipeline to check binding against
  //  ignore_state: [in] determines if whether or not the "constant binding" state should be ignored.
  //                     (See comments for "EnableConstantBinding()").
  //                     In other words, if this conduit is using constant binding (default), then
  //                     it doesn't really matter if it is bound to anything since constant binding
  //                     means "always bind to everything". However, most of the time it is 
  //                     necessary to pay attention to the state since conduits can be manually bound
  //                     and programs need to know which pipelines this conduit is bound to. There
  //                     are certain conditions where the display pipeline need not pay attention to
  //                     the constant binding state, but most likely all other programs do...It is 
  //                     recommended that if you do not understand exactly what is going on here, that
  //                     you just use the default.
  bool IsBound(const CRhinoDisplayPipeline& pipeline, bool ignore_state = false) const;
  // See: CRhinoDisplayConduit::IsBound( const CRhinoDisplayPipeline& pipeline, bool ignore_state = false) const;
  bool IsBound(const CRhinoViewport& viewport, bool ignore_state = false) const;
  // See: CRhinoDisplayConduit::IsBound( const CRhinoDisplayPipeline& pipeline, bool ignore_state = false) const;
  bool IsBound(const CRhinoView& view, bool ignore_state = false) const;
  // See: CRhinoDisplayConduit::IsBound( const CRhinoDisplayPipeline& pipeline, bool ignore_state = false) const;
  bool IsBound(const ON_Viewport& viewport, bool ignore_state = false) const;

  // Description:
  //
  bool ConstantBinding() const;

  // Description:
  //  Returns true if this conduit is bound to any specific viewport
  bool BindingsExist() const;
  // Description:
  //  Returns number of viewports that this conduit is bound to
  int BindingCount() const;
  
  // Description:
  //    The geometry type filter controls which types of geometry
  //    (points, curves, surfaces, meshes, etc.) will be processed 
  //    in any "object based" channels (see SC_OBJECT_BASED_CHANNEL)
  //
  // Parameters:
  //    geometry_filter - [in] an unsigned int made by or-ing ( | )
  //        values from the ON::object_type enum.  However, If you pass 0,
  //        then this is interpreted as "no geometry filter", which means
  //        ALL geometry will be processed and is equivalent to passing 
  //        the value ON::any_object.
  //
  // Note: The default geometry filter for all conduits is ON::any_object.
  //       If you want your conduit to only process specific types of
  //       objects, then you must set the geometry filter for your conduit
  //       accordingly. Using ON::object_type might seem like overkill for
  //       this purpose (since not all types are technically "drawn" as 
  //       objects within the channels), however, this is more consistent
  //       with how filtering works in other classes.
  //
  // For example: If you have a conduit that executes in the SC_DRAWOBJECT
  //              channel, but you're only interested in processing "Light"
  //              objects, then you would set your conduit's geometry filter 
  //              like so:
  //
  //       myConduit.SetGeometryFilter( ON::light_type );
  //
  //   ...a good place to set this would be either in your conduit's constructor
  //   or just before you enable your conduit, depending on how you've 
  //   decided to use this feature.
  void SetGeometryFilter(unsigned int geometry_filter);

  // Description:
  //    Returns the current geometry filter set for this conduit. Possible
  //    values will be based on ON::object_type enum and the different
  //    combinations that can exist through or-ing ( | ) them together.
  //
  // Note: If you passed 0 to SetGeometryFilter(), then this routine will
  //       return ON::any_object and not 0.
  unsigned int GetGeometryFilter() const;

  // Description:
  //  For the per-object drawing channels, allow a filter to only work with
  //  selected objects. Enabling the selected filter will cause the
  //  InterestedInObject function to return false for unselected objects
  //
  // Parameters:
  //  on - [in] true limits the conduit to selected objects
  //  checkSubObjects - [in] if true and on=true, then the conduit will be
  //    called for objects that have some subset of the object selected, like some
  //    edges of a surface.
  void SetSelectedFilter(bool on, bool checkSubObjects);

  // Description:
  //  Get filter settings for selected objects. See SetSelectedFilter comments.
  // Parameters:
  //  on - [out] true limits the conduit to selected objects
  //  checkSubObjects - [out] if true and on=true, then the conduit will be
  //    called for objects that have some subset of the object selected, like some
  //    edges of a surface.
  void GetSelectedFilter(bool& on, bool& checkSubObjects) const;

  void SetObjectFilter(ON_UUID objectId);
  void SetObjectFilter(const ON_SimpleArray<ON_UUID>& objectIds);
  ON_UuidList GetObjectFilter() const;

  // Description:
  //    Determines if the passed in object's "type" matches the currently
  //    set geometry filter. This is more or less a helper function used 
  //    when processing object-based channels.
  //
  // Returns:
  //    true  - if passed in object's type matches against the current geometry filter.
  //    false - if passed in object's type does NOT match against the currently geometry filter.
  //
  // Note: If the current geometry filter is set to ON::any_object OR 0, this routine will
  //       always return true, and no conditions or comparisons are checked or made.
  bool InterestedInObject(const CRhinoObject*) const;
protected:
  // The channels that this class was constructed with
  const CSupportChannels        m_Channels;
  
  //////////////////////////////////////////////////////////////////
  // Conduit parameters...
  // Note: Each of these are set and assigned accordingly prior to calling
  //       "ExecConduit"...
  // These pointers can change often. Do not hold onto them and use them outside the
  // scope of the ExecConduit function

  // The document that the conduit is working with at the time of ExecConduit
  // Use CRhinoDisplayConduit.Document() to get a CRhinoDoc pointer.
  unsigned int m_rhino_doc_sn = 0;

  // The view that the conduit is working with at the time of ExecConduit
  // Use CRhinoDisplayConduit.View() to get a CRhinoView pointer.
  // Use CRhinoDisplayConduit.PageView() to get a CRhinoPageView pointer.
  unsigned int m_rhino_view_sn = 0;

  // The viewport the conduit is working with at the time of ExecConduit
  // Use CRhinoDisplayConduit.Viewport() to get a CRhinoViewport pointer.
  unsigned int m_rhino_viewport_sn = 0;

  // Channel attributes are set up for each channel or "phase" of drawing. If the conduit
  // modifies these attributes inside of the ExecConduit function, the display for the
  // current channel will be changed based on these attributes. 
  // Note:  See the definition of CChannelAttributes for a "logical grouping" of which
  //        attributes apply to which channels.
  // Note2: Changing channel attributes that are not part of the "current channel" 
  //        context will have unknown and unexpected results.
  class CChannelAttributes*   m_pChannelAttrs = nullptr;

  // Global pipeline display attributes that the conduit can adjust to modify the display
  // Note: Changing display attributes may have no effect based on which channel this 
  //       conduit is currently running in (ie. The pipeline may step on any changes).
  //       A good rule is to only change these attributes inside SC_OBJECTDISPLAYATTRS,
  //       or all OBJECT DRAW channels, anywhere else may produce unexpected results.
  CDisplayPipelineAttributes*   m_pDisplayAttrs = nullptr;
  
  //
  //////////////////////////////////////////////////////////////////
  
private:
  bool m_bRegistered = false; // This gets set by the pipeline when conduits
                              // register/enable, unregister/disable themselves...

  bool m_bConstantBinding = true;

  ON_UuidList m_Bindings;
  
  unsigned int m_EnabledDoc = 0UL;

private:
  UINT m_geometry_filter; // used in all object draw channels...

  /////////////////////////////////////////////////////////////////////////
  // future additions...
  class CRhDisplayConduitPrivate* m_private = nullptr;

  friend class CRhinoDisplayPipeline;
private:
  // prohibit use - no implementation
  CRhinoDisplayConduit() = delete;
  CRhinoDisplayConduit(const CRhinoDisplayConduit&) = delete;
  CRhinoDisplayConduit& operator=(const CRhinoDisplayConduit&) = delete;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Specialized abstract conduit class that provides very high level support 
// for setting up and managing 3D Stereo viewports.
//
// There are basically two types of stereo modes supported in Rhino:
//    1) Hardware shutter glasses
//    2) Some kind of software generated stereo view,
//       (i.e. Anaglyph red/blue glasses)
//
// You cannot support both types in one conduit. When you construct your
// stereo conduit you must specify what type you plan on supporting and
// all the appropriate setup and framework initialization will be done
// for you automatically.
// 
// Each "stereo phase" is broken into 3 primary parts (or channels):
//    1) Frame buffer initialization
//    2) Frustum setup and initialization
//    3) Frame buffer drawing
//
// ...each of those phases is called for each eye and are handled in
// the abstract routines:
//
//        ProcessLeftEye(...)
//        ProcessRightEye(...)
//
// For obvious reasons, you should process all left eye calculations and
// left eye drawing inside "ProcessLeftEye", and all right eye calculations  
// and right eye drawing inside "ProcessRightEye". Doing any other kind of 
// calculations inside either of these routines will have unknown behavior.
//
// Each processing routine is passed an identifying channel ID so that
// you can determine where you are in the pipeline and what needs to be
// done.
//
// For example: The following overridden routine is used to process left eye
//              stereo frames...(you will need to do similar things inside
//              ProcessRightEye as well)
//
//    bool CSomeStereoViewConduit::ProcessLeftEye(CRhinoDisplayPipeline&  dp, 
//                                                StereoDisplayChannel    nChannel)
//    {
//      bool  bProcessed = false;
//    
//      switch ( nChannel )
//      {
//        case SDC_INIT_FRAME:
//        {
//          // clear the left eye frame buffer...
//          bProcessed = true;
//          break;
//        }
//    
//        case SDC_SETUP_FRUSTUM:
//        {
//          // Setup left eye frustum...
//          bProcessed = true;
//          break;
//        }
//    
//        case SDC_DRAW_FRAME:
//        {
//          // Draw left eye frame...
//          bProcessed = true;
//          break;
//        }
//      }
//      return bProcessed;
//    }
// 
// Note: If you process a given channel then you should return true from your routines,
//       if you don't process a given channel, then returning false will cause Rhino to
//       process that channel using its default mechanism.
// 
//       For example: If you don't plan on doing anything for frame buffer initialization,
//                    then returning false will cause Rhino to clear the frame buffer
//                    using the standard default mechanism (which is based on whatever
//                    the current display mode is using).
//
class RHINO_SDK_CLASS CRhinoStereoDisplayConduit : public CRhinoDisplayConduit
{
public:
  // The primary channels acted on for stereo viewing. These get passed to your
  // Process Eye methods so that you can determine which channel Rhino is
  // currently working on.
  enum StereoDisplayChannel
  {
    SDC_INIT_FRAME,
    SDC_SETUP_FRUSTUM,
    SDC_DRAW_BACKGROUND,
    SDC_DRAW_FRAME,
  };

public:
  // If you wish to support hardware 3d stereo modes (i.e. Shutter glasses), then
  // construct your conduit passing true to this base class constructor, otherwise
  // your stereo implementation is entirely left up to you.
  //
  // Note: Constructing a stereo conduit does NOT automatically set the viewport
  //       into hardware stereo mode. You MUST manually configure a Rhino display
  //       mode to use hardware 3D shutter glasses, and then manually set the 
  //       viewport to use that display mode. Due to OS restrictions and limitations,
  //       binding all of this to a conduit construction is not possible within
  //       current architecture. If you construct your conduit and bind it to a
  //       view that is not using a 3d shutter glasses display mode, then none of
  //       your eye processing methods will be called... However, setting a view
  //       to use a 3d shutter glasses mode after condstruction should then cause
  //       your methods to get called without any further actions by your code.
  CRhinoStereoDisplayConduit(bool bRequiresHardwareShutterGlasses = false);
  virtual ~CRhinoStereoDisplayConduit();

  // Description:
  //   Turn this conduit on. When a conduit is enabled, it's ExecConduit function is called
  //   for each viewport that this conduit pays attention to.
  void Enable();

  // Description:
  //   Turn this conduit off. When a conduit is disabled, it will not participate in any
  ///  custom drawing activity.
  void Disable();

  // Attributes
public:
  bool  UsesHardwareShutterGlasses() const { return m_bUsesHardwareShutterGlasses; }

  static bool HardwareShutterGlassesConduitExists();

  // Intefrace
public:
  // You MUST implement to following methods and act on them accordingly based
  // on the passed in channel type... As mentioned above, each one of these is called
  // automatically by the framework once your conduit is constructed and bound
  // to a view. What you do in these routines is entirely up to your implementation.
  //
  // Parameters:
  //  pipeline: [in] the pipeline your conduit is executing in
  //  nChannel: [in] the current stereo phase the pipeline is running
  // Returns:
  //  true:   if you processed the specified channel and you do not want Rhino to process it.
  //  false:  if you did NOT process the specified channel and you want Rhino to process it
  //          using the standard default mechanism.
  virtual bool      ProcessLeftEye(CRhinoDisplayPipeline&  pipeline, StereoDisplayChannel nChannel) = 0;
  virtual bool      ProcessRightEye(CRhinoDisplayPipeline& pipeline, StereoDisplayChannel nChannel) = 0;

  // Since some stereo modes don't require any sort of post processing of the
  // frame buffer, then the standard pipeline mechanism for displaying the
  // final frame buffer will suffice and therefore this is not a required
  // interface for stereo view conduits...Nevertheless, this routine is always
  // called once, when ALL left and right eye processing has completed.
  //
  // Parameters:
  //  pipeline: [in] the pipeline your conduit is executing in
  virtual void      FinalizeFrame(CRhinoDisplayPipeline& pipeline);

  // Frustum helper methods...
protected:
  // All camera and frustum values are ONLY valid when processing left and right
  // eyes within the SDC_SETUP_FRUSTUM channel. Accessing any of these values
  // before, after, or outside that channel will have unknown behavior.
  double            FrustLeft() const   { return vp_frustum_left; }
  double            FrustRight() const  { return vp_frustum_right; }
  double            FrustTop() const    { return vp_frustum_top; }
  double            FrustBottom() const { return vp_frustum_bottom; }
  double            FrustNear() const   { return vp_frustum_near; }
  double            FrustFar() const    { return vp_frustum_far; }

  ON_3dPoint        CameraLocation() const  { return vp_camera_location; }
  ON_3dVector       CameraDirection() const { return vp_camera_dir; }
  ON_3dVector       CameraUp() const        { return vp_camera_up; }

  void              SetFrustLeft(double);
  void              SetFrustRight(double);

private:
  bool ExecConduit(CRhinoDisplayPipeline& pipeline,
                           UINT channel,
                           bool& terminate) override;

private:
  void              SaveFrustum(CRhinoDisplayPipeline& pipeline);
  void              RestoreFrustum(CRhinoDisplayPipeline& pipeline);

  bool  m_bUsesHardwareShutterGlasses;
  bool  m_bDirty;

  double  vp_frustum_left, vp_frustum_right,
          vp_frustum_bottom, vp_frustum_top,
          vp_frustum_near, vp_frustum_far;

  ON_3dPoint  vp_camera_location;
  ON_3dVector vp_camera_up;
  ON_3dVector vp_camera_dir;

  static int m_nHardwareShutterGlassesConduitExists;

private:
  // prohibit use - no implementation
  CRhinoStereoDisplayConduit(const CRhinoDisplayConduit&) = delete;
  CRhinoStereoDisplayConduit& operator=(const CRhinoDisplayConduit&) = delete;

  friend class CRhinoDisplayConduit;

  ON__UINT_PTR m_sdk_reserved = 0;
};

typedef ON_SimpleArray<CRhinoDisplayConduit*>       ConduitList;

