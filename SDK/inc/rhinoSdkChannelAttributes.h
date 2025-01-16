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
// Flags and attributes need to be provided to pass back and forth to the
// conduits based on all of the default stuff the pipeline draws in 
// its channels.
//
// Reason: Conduits may want to override the default behavior (eg. DrawGrid),
//         so we need a way for conduits to tell the pipeline what is being
//         done, as well as let other conduits know what has already been 
//         done.  
//
// Note: These are all based on what Rhino's pipeline does...there is no
//       way to second guess what other pipelines will or won't do, so all
//       we can do for now is tell conduits as much as we can about Rhino's
//       pipeline attributes...

enum EFrameBufferFillMode : int
{
  FBFM_DEFAULT_COLOR = 1,
  FBFM_SOLID_COLOR,
  FBFM_GRADIENT_2_COLOR,
  FBFM_GRADIENT_4_COLOR,
  FBFM_BITMAP,
  FBFM_RENDERER,
  FBFM_TRANSPARENT
};

enum ELightingScheme : int
{
  LS_NONE = 0,
  LS_DEFAULT_LIGHTING = 1,
  LS_SCENE_LIGHTING = 2,
  LS_CUSTOM_LIGHTING = 3,
  LS_AMBIENT_OCCLUSION = 4,
};

class RHINO_SDK_CLASS CChannelAttributes
{
public:
  CChannelAttributes();
  
  bool                  InitAttributes();
  
////////////////////////////////////////////////////////////////
// Attributes are broken down and organized into their 
// relevant pipeline channels. 
//
// Note: accessing and/or setting attributes outside their
//       required channel is an unknown behavior or operation.
//       Besides, the pipeline initializes these attributes on 
//       a per-channel basis, so for example, setting an attribute
//       designated for DrawMiddleground channels from within a
//       DrawBackground channel has no affect.
public:
  UINT                  m_nActiveChannel;
  UINT                  m_nRenderPass;
  
  //////////////////////////////////////////////////
  // Init framebuffer conduit channel attributes...
  bool                  m_bFramebufferCleared;

  EFrameBufferFillMode  m_eFrameBufferFillMode;

  ON_Color              m_SolidColor;

  ON_Color              m_GradTopLeft;
  ON_Color              m_GradBotLeft;
  ON_Color              m_GradTopRight;
  ON_Color              m_GradBotRight;
  std::shared_ptr<const CRhinoDib> m_pBgBitmap;
  
  //////////////////////////////////////////////////
  // Calc BoundingBox conduit channel attributes...
  bool                  m_bUseExclusiveBBox;
  ON_BoundingBox        m_BoundingBox;
  
  //////////////////////////////////////////////////
  // Calc clipping planes conduit channel attributes...
  // The m_dNear and m_dFar values often correspond to the
  // CRhinoDisplayPipeline.m_Clipping.m_bbox_near and
  // CRhinoDisplayPipeline.m_Clipping.m_bbox_far
  // values. In particular, it is common for m_dNear and
  // m_dFar to be different from the view frustum's near/far
  // values. This unfortunate situation cannot be 
  // fixed until we ship Rhino 6.0.
  double                m_dNear;
  double                m_dFar;
  ON_SimpleArray<ON_ClippingPlaneInfo> m_ClippingPlanes;
    
  //////////////////////////////////////////////////
  // Setup Frustum conduit channel attributes...
  // m_dLeft, ..., m_dBottom are typically the view frustum's 
  // left,right,top,bottom values, but since you don't
  // reliably know the view frustum's near and far values,
  // m_dLeft, ..., m_dBottom are useless in perspective views.
  double                m_dLeft;
  double                m_dRight;
  double                m_dTop;
  double                m_dBottom;
    
  //////////////////////////////////////////////////
  // Draw background conduit channel attributes...
  bool                  m_bDrawConstructionGridOnTop;
  bool                  m_bDrawConstructionGrid;
  bool                  m_bDrawZAxis;
  bool                  m_bGridDrawn;
  
  bool                  m_bDrawConstructionAxes;
  bool                  m_bAxesDrawn;
  
  bool                  m_bDrawWallpaper;
  bool                  m_bWallpaperDrawn;
  bool                  m_bDrawBackgroundBitmap;
  bool                  m_bBgBitmapDrawn;
  
  void                  EnableBackgroundAttrs(bool=true);
  void                  EnableGridAndAxesAttrs(bool=true);
  
  //////////////////////////////////////////////////
  //  Setup Lighting conduit channel attributes...
  ELightingScheme           m_eLightingScheme;
  ON_ObjectArray<ON_Light>  m_Lights;
  
  //////////////////////////////////////////////////
  // Draw middleground conduit channel attributes...
  bool                  m_bSortByDistance;
  bool                  m_bHiddenLineRemoval;
  
  //////////////////////////////////////////////////
  // Draw object AND Object Culling conduit channel attributes...
  bool                  m_bDrawObject;
  bool                  m_bObjectReplaced;
  bool                  m_bObjectColorOverridden;
  int                   m_nObjectIndex;
  const CRhinoObject*   m_pObject;
  
  //////////////////////////////////////////////////
  // Draw foreground conduit channel attributes...
  bool                  m_bWorldAxesDrawn;
  bool                  m_bDrawWorldAxes;
  
  //////////////////////////////////////////////////
  // Meshing Parameters conduit channel attributes...
  bool                  m_bUseMeshingParms;
  ON_MeshParameters     m_MeshingParms;

  //////////////////////////////////////////////////
  // Post Processing Frame Buffer conduit channel attributes...
  bool                  m_bFrameBufferModified;
  CRhinoDib*          m_pFrameBuffer;
  
public:
  bool                  PopulateChannelAttrs(CChannelAttributes&, UINT);

};

