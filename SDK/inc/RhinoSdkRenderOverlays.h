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

// Post rendering features that make up all possible post renderables...
enum ERenderOverlayFeatures
{ 
  ROF_POINTS            = 0x00000001,
  ROF_LIGHTS            = 0x00000002,
  ROF_MESHWIRES         = 0x00000004,
  ROF_ISOS              = 0x00000008,
  ROF_EDGES             = 0x00000010,
  ROF_TEXT              = 0x00000020, 
  ROF_CURVES            = 0x00000040, 
  ROF_ANNOTATIONS       = 0x00000080,
  ROF_SUBDWIRES         = 0x00000100,
  ROF_SECTION_FILL      = 0x00000200,
  ROF_SECTION_BOUNDARY  = 0x00000400,
};

// Rendering overlay class that creates "maskable" renderings (known as Overlays), 
// of all of the "non-mesh based" objects in the scene. These overlays can then
// be added or combined with other renderings providing a general solution for
// those renderers that do not support some or all of the non-mesh based objects.
class RHINO_SDK_CLASS CRhinoRenderOverlays
{
public:
   ON_DEPRECATED_MSG("use CRhinoRenderOverlays::CRhinoRenderOverlays(const CRhinoDoc&)")
   CRhinoRenderOverlays();

   CRhinoRenderOverlays(const CRhinoDoc&);
   CRhinoRenderOverlays(int DocumentSerialNumber);
   ~CRhinoRenderOverlays();


  // Description:
  //   Creates all of the requested overlays in the given resolution using the
  //   given frustum and display attributes. These overlays are stored internally
  //   and can only be accessed via the "GetOverlayXXX" methods.
  //
  // Note: This is the starting point for using overlays. Basically you use this
  //       routine to specify ALL of the types of overlays you want to support,
  //       and then you use GetOverlayXXXX() to gather/comibine an overlay or set
  //       of overlays into one maskable entity. An overlay is made up of one or
  //       more "features", and a feature is described/defined by ERenderOverlayFeatures.
  //
  // Parameters:
  //   Frustum  - [in]
  //      viewing frustum that specifies camera and projection from which to render
  //      the overlays. This most likely will come from one of Rhino's viewports, but
  //      doesn't necessarily have to.
  //   Attrs  - [in]
  //      Display attributes that will be used when rendering the overlays. These will
  //      only define "how" things will get rendered, and not "what" gets rendered, since
  //      that is what is determined by this routine in general. For example: Isocurve thickness
  //      and color can be controlled by these attributes.
  //   nWidth  - [in]
  //      The physical width in pixels that you want all overlay resolutions to be.
  //   nHeight - [in]
  //      The physical height in pixels that you want all overlay resolutions to be.
  //   nFeatures - [in]
  //      A bitwise mask (using ERenderOverlayFeatures constants) that specifies which
  //      overlays you're going/wanting to support.
  //   bCombine - [in]
  //      If "true", then all requested features will be rendered into a single ovleray.
  //      If "false", then all requested features will be rendered and  maintained in 
  //      their own individual overlay.
  //
  // Returns:
  //   TRUE if all requested features were rendered successfully into overlays. 
  //   FALSE if something bad happened during the creation process.
  //
  // Example:
  //
  //    unsigned int    nSupportedFeatures = ROF_CURVES|ROF_ISOS|ROF_EDGES|ROF_POINTS;
  //
  //    CRhinoRenderOverlays   PostRender;
  //
  //    if ( PostRender.CreateOverlays( view, attrs, 1024, 768, nSupportedFeatures, false ) )
  //    {
  //      unsigned int    nUsedFeatures = 0;
  //    
  //      if ( bShowCurves )
  //        nUsedFeatures |= ROF_CURVES;
  //      if ( bShowIsos )
  //        nUsedFeatures |= ROF_ISOS;
  //      if ( bShowEdges )
  //        nUsedFeatures |= ROF_EDGES;
  //      if ( bShowPoints )
  //        nUsedFeatures |= ROF_POINTS;
  //
  //      CRhinoDib     overlay;
  //
  //      if ( PostRender.GetOverlay( overlay, nUsedFeatures ) )
  //      {
  //        PostRender.CombineOverlay( overlay, SOME_FINAL_RENDERING_IMAGE );
  //      }
  //    }
  //
  bool CreateOverlays(const ON_3dmView&           Frustum, 
                      CDisplayPipelineAttributes  Attrs, 
                      int                         nWidth, 
                      int                         nHeight, 
                      unsigned int                nFeatures,
                      bool                        bCombine=true);

  
  // Description:
  //   Creates the requested overlay based on the specified features. 
  //
  // Parameters:
  //   Overlay - [out]
  //      The created overlay containing all of the requested features. This will be
  //      a 32bit DIB containing all requested features with alpha channel. The alpha channel
  //      will include anti-aliasing values used when the overlays were initially created.
  //
  //   nFeatures - [in]
  //      The bitwise mask made up of ERenderOverlayFeatures constants that defines
  //      which features should be rendered into the Overlay. (Note: This mask
  //      must be equal to or a subset of the mask used when calling "CreateOverlays").
  //
  // Returns:
  //   TRUE if the overlay with the requested features was successfully created.
  //   FALSE if one or more the requested features could not be successfully rendered
  //   into the overlay. This can happen if you specify a feature that was not also
  //   specified in the call to "CreateOverlays" prior to calling this method.
  //
  // Note: A successful call to "CreateOverlays" must be made prior to calling this method.
  //
  bool GetOverlay(CRhinoDib&  Overlay, unsigned int  nFeatures) const;

  
  // Description:
  //    Creates the requested overlay based on the specified features in a grayscale format.
  //
  // Parameters:
  //   Overlay - [out]
  //      The created overlay containing all of the requested features. This will be
  //      a 24bit RGB DIB containing all requested features, but will be in a grayscale
  //      format (i.e. R = G = B ) regardless of what attribute settings were used 
  //      when creating the overlays initially.
  //
  //   nFeatures - [in]
  //      The bitwise mask made up of ERenderOverlayFeatures constants that defines
  //      which features should be rendered into the Overlay. (Note: This mask
  //      must be equal to or a subset of the mask used when calling "CreateOverlays").
  //
  // Returns:
  //   TRUE if the overlay with the requested features was successfully created.
  //   FALSE if one or more the requested features could not be successfully rendered
  //   into the overlay. This can happen if you specify a feature that was not also
  //   specified in the call to "CreateOverlays" prior to calling this method.
  //
  // Note: A successful call to "CreateOverlays" must be made prior to calling this method.
  //
  bool GetOverlayAsAlpha(CRhinoDib&  Overlay, unsigned int  nFeatures) const;

  // Description:
  //    Method used to determine whether or not certain overlays can be created using the
  //    specified features.
  //
  // Parameters:
  //   nFeatures - [in]
  //      The bitwise mask made up of ERenderOverlayFeatures constants that defines
  //      which features should be considered when performaing the query. (Note: This mask
  //      must be equal to or a subset of the mask used when calling "CreateOverlays").
  //
  // Returns:
  //   TRUE if the specified features would result in a successful overlay creation.
  //   FALSE if the specified features would not create a valid overlay.
  //
  // Note: A successful call to "CreateOverlays" must be made prior to calling this method.
  //
  bool HasOverlay(unsigned int nFeatures) const;

  // Description:
  //    Combines two like overlays into one.
  //
  // Parameters:
  //    Source - [in]
  //       Contains the primary source information of one of the overlays to be combined.
  //    Destination - [in out]
  //       Contains the secondary source information of the other overlay to be combined, 
  //       but also will have its contents modified/updated to resulting combination.
  //
  // Returns:
  //    TRUE if the combination of the two overlays was successful, and the results are
  //    placed in Destination.
  //    FALSE if the combination failed. This might be due to a mismatch in resolution or
  //    color depth between the two overlays.
  //
  // Note: Both Source and Destination resolutions and color depths MUST be exactly the same.
  //
  bool CombineOverlays(const CRhinoDib&   Source, CRhinoDib&  Destination);

public:
  int DocumentRuntimeSerialNumber(void) const;
  CRhinoDoc* GetDocument(void) const;

protected:
  void SetFeatureAttrs(unsigned int  nFeatures, CDisplayPipelineAttributes& Attrs, bool bState);

  bool CreateFeatures(CRhinoDisplayPipeline_OGL&, CDisplayPipelineAttributes&, unsigned int nFeatures);
  bool CreateFeatures(CRhinoDisplayPipeline&, CDisplayPipelineAttributes&, unsigned int nFeatures);

  void SetFillOrBoundaryRequest(bool);
  bool FillOrBoundaryRequested(void) const;

private:
  class COverlay
  {
  public:
    COverlay() = default;
    ~COverlay() { if ( m_pDepth != nullptr ) delete[] m_pDepth; }

    unsigned int m_nFeatures = 0;
    CRhinoDib    m_Overlay;
    float*       m_pDepth = nullptr;
  };


  ON_SimpleArray<const COverlay*>   m_Overlays;

  void Destroy(void);

private:
  void* m_impl = nullptr;
};

