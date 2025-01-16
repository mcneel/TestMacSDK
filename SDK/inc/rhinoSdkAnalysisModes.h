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


/////////////////////////////////////////////////////////////////
//
// These ids are passed to CRhinoObject::InAnalysisMode() to
// answer queries about what types of analysis modes are active.
//

// This ID is used to check for any shaded analysis mode:
// false color (like draft angle) or texture 
// based (like zebra and emap).
// {2E5FE617-7D66-4ea0-8572-A1C3F8F06B84}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_SHADED_ANALYSIS_MODE_ID = 
{ 0x2e5fe617, 0x7d66, 0x4ea0, { 0x85, 0x72, 0xa1, 0xc3, 0xf8, 0xf0, 0x6b, 0x84 } };

// This ID is used to check for any false color analysis mode
// like draft angle or curvature.
// {5B3A0840-7899-4e22-987D-4921282558C0}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_FALSECOLOR_ANALYSIS_MODE_ID = 
{ 0x5b3a0840, 0x7899, 0x4e22, { 0x98, 0x7d, 0x49, 0x21, 0x28, 0x25, 0x58, 0xc0 } };

// This ID is used to check for any texture based analysis mode
// like zebra or emap.
// {DF0D8626-3BEA-4471-B8D9-1E85A7967985}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_TEXTURED_ANALYSIS_MODE_ID = 
{ 0xdf0d8626, 0x3bea, 0x4471, { 0xb8, 0xd9, 0x1e, 0x85, 0xa7, 0x96, 0x79, 0x85 } };

// {43BB9491-51E9-493a-B838-89536DD00960}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_WIREFRAME_ANALYSIS_MODE_ID = 
{ 0x43bb9491, 0x51e9, 0x493a, { 0xb8, 0x38, 0x89, 0x53, 0x6d, 0xd0, 0x9, 0x60 } };


/////////////////////////////////////////////////////////////////
//
// These ids are passed to CRhinoObject::InAnalysisMode() to
// determine if an object is in a specific core analysis mode.
// Rhino has 7 core in visual analysis modes. Other analysis modes 
// can be added by plug-ins.  Use the generic queries described above
// if you need to know if an object is in a shaded mode, etc.

// In edge analysis mode brep and mesh edges are shown in 
// a selected color.
// {197B765D-CDA3-4411-8A0A-AD8E0891A918}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_EDGE_ANALYSIS_MODE_ID = 
{ 0x197b765d, 0xcda3, 0x4411, { 0x8a, 0xa, 0xad, 0x8e, 0x8, 0x91, 0xa9, 0x18 } };

// In curvature graph analysis mode, curvature hair is shown on
// curves and surfaces.
// {DF59A9CF-E517-4846-9232-D9AE56A9D13D}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_CURVATURE_GRAPH_ANALYSIS_MODE_ID = 
{ 0xdf59a9cf, 0xe517, 0x4846, { 0x92, 0x32, 0xd9, 0xae, 0x56, 0xa9, 0xd1, 0x3d } };

// In zebra stripe analysis mode, zebra stripes are shown 
// on surfaces and meshes.
// {0CCA817C-95D0-4b79-B5D7-CEB5A2975CE0}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_ZEBRA_STRIPE_ANALYSIS_MODE_ID = 
{ 0xcca817c, 0x95d0, 0x4b79, { 0xb5, 0xd7, 0xce, 0xb5, 0xa2, 0x97, 0x5c, 0xe0 } };

// In emap analysis mode, an environment map is shown 
// on surfaces and meshes.
// {DAEF834E-E978-4f7b-9026-A432C678C189}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_EMAP_ANALYSIS_MODE_ID = 
{ 0xdaef834e, 0xe978, 0x4f7b, { 0x90, 0x26, 0xa4, 0x32, 0xc6, 0x78, 0xc1, 0x89 } };

// In curvature color analysis mode, surface curvature 
// is shown using false color mapping.
// {639E9144-1C1A-4bba-8248-D330F50D7B69}
// RHINO_CURVATURE_COLOR_ANALYSIS_MODE_ID and ON_SurfaceCurvatureColorMapping::Id
// are identical ids and must never be changed.
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_CURVATURE_COLOR_ANALYSIS_MODE_ID =
{ 0x639e9144, 0x1c1a, 0x4bba, { 0x82, 0x48, 0xd3, 0x30, 0xf5, 0xd, 0x7b, 0x69 } };

// In draft angle analysis mode, draft angle is 
// displayed using false color mapping.
// {F08463F4-22E2-4cf1-B810-F01925446D71}
// RHINO_DRAFT_ANGLE_ANALYSIS_MODE_ID and ON_SurfaceDraftAngleColorMapping::Id
// are identical ids and must never be changed.
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_DRAFT_ANGLE_ANALYSIS_MODE_ID =
{ 0xf08463f4, 0x22e2, 0x4cf1, { 0xb8, 0x10, 0xf0, 0x19, 0x25, 0x44, 0x6d, 0x71 } };

// {B28E5435-D299-4933-A95D-3783C496FC66}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_THICKNESS_ANALYSIS_MODE_ID = 
{ 0xb28e5435, 0xd299, 0x4933, { 0xa9, 0x5d, 0x37, 0x83, 0xc4, 0x96, 0xfc, 0x66 } };

// {2FB89F74-0996-4C68-BD09-301EA64BF304}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_SECTION_ANALYSIS_MODE_ID = 
{ 0x2fb89f74, 0x996, 0x4c68, { 0xbd, 0x9, 0x30, 0x1e, 0xa6, 0x4b, 0xf3, 0x4 } };

// {F4477F56-BC94-4DD2-B941-7D21F9DC674A}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_END_ANALYSIS_MODE_ID =
{ 0xf4477f56, 0xbc94, 0x4dd2, { 0xb9, 0x41, 0x7d, 0x21, 0xf9, 0xdc, 0x67, 0x4a } };

// {83141AA3-563B-4355-8009-48685119BC02}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_DIRECTION_ANALYSIS_MODE_ID =
{ 0x83141aa3, 0x563b, 0x4355, { 0x80, 0x9, 0x48, 0x68, 0x51, 0x19, 0xbc, 0x2 } };

// In edge continuity analysis mode, curvature hair is shown on
// pairs of surface edges to represent the continuity deviations.
// {A5CC27F6-E169-443A-87ED-C10657FF4BC9}
// NOTE: The static is a mistake - it should not be here, 
// however removing it will break the SDK.
// Because of the static, in each file that includes this header, 
// there is source file scope copy of this id.
static const ON_UUID RHINO_EDGE_CONTINUITY_ANALYSIS_MODE_ID =
{ 0xa5cc27f6, 0xe169, 0x443a, { 0x87, 0xed, 0xc1, 0x6, 0x57, 0xff, 0x4b, 0xc9 } };


// For each visual analysis mode there is a single instance of a class
// derived from CRhinoVisualAnalysisMode.  The base class constructor,
// CRhinoVisualAnalysisMode(ON_UUID mode_id) registers the visual
// analysis mode with Rhino.
class RHINO_SDK_CLASS CRhinoVisualAnalysisMode
{
public:

  /*
  Description:
    Use the visual analysis mode id to lookup a visual analysis mode.
  Parameters:
    am_id - [in]
  Returns:
    A pointer to the visual analysis mode.  This pointer will
    not change during an instance of Rhino.exe.
  */
  static const CRhinoVisualAnalysisMode* Mode(ON_UUID am_id);

  CRhinoVisualAnalysisMode(ON_UUID am_id, int style);
  virtual ~CRhinoVisualAnalysisMode();

  // Identifies the analysis mode.  For built-in analysis modes, 
  // this is one of the ON_UUIDs listed above.  Plug-ins may define
  // there own analysis modes, so the list above is not a complete
  // list.  The value of m_mode_id is set by the constructor.
  const ON_UUID m_am_id;

  // There are three supported styles.  Anything more complex has
  // to create a display pipeline.
  enum
  {
    wireframe_style   = 1, // like curvature hair or naked edge analysis)
    texture_style     = 2, // like emap and zebra
    false_color_style = 4  // like draft angle, curvature, thickness analysis
  };

  const int m_style; // one of the above enum values

  bool IsWireframe() const;
  bool IsTexture() const;     // style = 2
  bool IsFalseColor() const; 
  bool IsShaded() const;      // texture or falsecolor

  /*
  Description:
    Get the name of the analysis mode.  This name is used
    byt the What command and the object properties details
    window to describe the object.
  */
  virtual
  void GetAnalysisModeName( ON_wString& name ) const = 0;
  
  /*
  Description:
    Turn the analysis mode's user interface on and off.  
    For Rhino's built in modes this opens or closes
    the modeless dialog that controls the analysis
    mode's display settings.
  */
  virtual
  bool EnableUserInterface( bool bEnable ) const;

  /*
  Description:
    Generally, a plug-in developer will override this function.
  Parameters:
    object - [in]
  Returns:
    True if this visual analysis mode can be used on object.
  */
  virtual 
  bool ObjectSupportsAnalysisMode( 
          const CRhinoObject* object 
          ) const;

  /*
  Returns:
    True if this visual analysis mode should show isocuves
    on shaded surface objects.  Often a mode's user interface
    will provide a way to change this setting.
  */
  virtual 
  bool ShowIsoCurves() const;

  /*
  Description:
    If an analysis mode needs to modify display attributes,
    this is the place to do it.  In particular,
    if m_style = CRhinoVisualAnalysisMode::texture_style, 
    then this virtual function must be overridden.

    Shaded visual analysis modes that use texture mapping, like
    emap and zebra, override this function to set up the texture.
  Parameters:
    object - [in]
  */
  virtual
  void SetAnalysisModeDisplayAttributes(
          const CRhinoObject* object,
          CDisplayPipelineAttributes& display_attrs
          ) const;

  /*
  Description:
    If m_style = CRhinoVisualAnalysisMode::false_color_style, then
    this virtual function must be overridden.
  Parameters:
    object - [in]
    meshes - [in/out]
      meshes used to draw the object.
  */
  virtual
  void UpdateVertexColors( 
            const CRhinoObject* object, 
            ON_SimpleArray<const ON_Mesh *>& meshes 
            ) const;

  /*
  Description:
    If m_style = wireframe_style, then the default decomposes 
    the curve object into nurbs curve segments and calls 
    the virtual DrawNurbsCurve  for each segment.
  Parameters:
    curve_object - [in]
    dp - [in]    
  */
  virtual
  void DrawCurveObject( 
          const CRhinoCurveObject& curve_object,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawMeshObject( 
          const CRhinoMeshObject& mesh_object,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawBrepObject( 
          const CRhinoBrepObject& brep_object,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawExtrusionObject( 
          const CRhinoExtrusionObject& extrusion_object,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawPointObject( 
          const CRhinoPointObject& point_object,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawPointCloudObject( 
          const CRhinoPointCloudObject& point_object,
          CRhinoDisplayPipeline& dp
          ) const;

  /*
  Description:
    The default does nothing.  This is a good function to
    override for analysis modes like curvature hair display.
  Parameters:
    curve_object - [in]
      Curve object
    nurbs_curve - [in]
      A nurbs representation of portion of the curve
      or a dynamically edited nurbs_curve.
    dp - [in]    
  */
  virtual
  void DrawNurbsCurve( 
          const CRhinoObject& rhino_object,
          const ON_NurbsCurve& nurbs_curve,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawNurbsSurface( 
          const CRhinoObject& rhino_object,
          const ON_NurbsSurface& nurbs_surface,
          CRhinoDisplayPipeline& dp
          ) const;

  virtual
  void DrawMesh( 
          const CRhinoObject& rhino_object,
          const ON_Mesh& mesh,
          CRhinoDisplayPipeline& dp
          ) const;

private:
  // prohibit use - no implementation
  CRhinoVisualAnalysisMode() = delete;
  CRhinoVisualAnalysisMode(const CRhinoVisualAnalysisMode&) = delete;
  CRhinoVisualAnalysisMode& operator=(const CRhinoVisualAnalysisMode&) = delete;
};


/*
Description:
  At runtime, CRhinoObject.m_visual_analysis_modes is a 
  linked list of CRhinoObjectVisualAnalysisMode classes 
  that record which analysis modes are active on that 
  object.
*/
class RHINO_SDK_CLASS CRhinoObjectVisualAnalysisMode
{
public:
  const CRhinoVisualAnalysisMode* m_am;
  const CRhinoObjectVisualAnalysisMode* m_next;
};

/*
Description:
  Copy analysis modes to destination object.
Parameters:
  source_object - [in]
  analysis_mode_list - [in]
  destination_object - [in]
Returns:
  Number of analysis modes enabled on destination_object.
*/
RHINO_SDK_FUNCTION
int RhinoCopyAnalysisModes(
          const CRhinoObject* source_object,
          CRhinoObject* destination_object
          );

RHINO_SDK_FUNCTION
int RhinoCopyAnalysisModes(
          const CRhinoObjectVisualAnalysisMode* analysis_mode_list, 
          CRhinoObject* destination_object
          );

