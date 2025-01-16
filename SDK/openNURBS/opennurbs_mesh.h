//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_MESH_INC_)
#define OPENNURBS_MESH_INC_

#include <vector>

#pragma region RH_C_SHARED_ENUM [ON_SubDComponentLocation] [Rhino.Geometry.SubDComponentLocation] [byte]
  /// <summary>
  /// The ON_SubDComponentLocation enum is used when an ON_SubD component
  /// is referenced and it is important to distinguish between the
  /// component's location in the SubD control net and its location
  /// in the SubD limit surface.
  /// </summary>
enum class ON_SubDComponentLocation : unsigned char
{
  ///<summary>
  /// Not a valid component location and used to indicate the value is not initialized.
  /// Note well: This value is saved in 3dm archives and cannot be changed.
  ///</summary>
  Unset = 0,

  ///<summary>
  /// The component's location in the SubD control net.
  /// Note well: This value is saved in 3dm archives and cannot be changed.
  ///</summary>
  ControlNet = 1,

  ///<summary>
  /// The component's location in the SubD limit surface.
  /// Note well: This value is saved in 3dm archives and cannot be changed.
  ///</summary>
  Surface = 2
};
#pragma endregion

ON_SubDComponentLocation ON_SubDComponentLocationFromUnsigned(
  unsigned int loc_as_unsigned
);

//////////////////////////////////////////////////////////////////////////
//
// ON_SubDDisplayParameters
//
//  A collection of parameters that are passed to functions that
//  calculate a various representations of ON_SubD objects.
//

class ON_CLASS ON_SubDDisplayParameters
{
public:
  ON_SubDDisplayParameters() = default;
  ~ON_SubDDisplayParameters() = default;
  ON_SubDDisplayParameters(const ON_SubDDisplayParameters&) = default;
  ON_SubDDisplayParameters& operator=(const ON_SubDDisplayParameters&) = default;

public:
  void Dump(class ON_TextLog&) const;

public:
  enum : unsigned int
  {
    ///<summary>
    /// Indicates the SubD display mesh density has not be set.
    ///</summary>
    UnsetDensity = 0,

    ///<summary>
    /// The minimum SubD display density that can be se in Rhino user interface is ExtraCoarseDensity (1).
    ///</summary>
    MinimumUserInterfaceDensity = 1,

    ///<summary>
    /// The maximum SubD display density that can be se in Rhino user interface is ExtraFineDensity (5).
    ///</summary>
    MaximumUserInterfaceDensity = 5,

    ///<summary>
    /// SubD display density values <= MinimumAdaptiveDensity will never be adaptively reduced.
    /// SubD display density values > MinimumAdaptiveDensity may be adaptively reduced to a value >= MinimumAdaptiveDensity.
    ///</summary>
    MinimumAdaptiveDensity = 1,

    ///<summary>
    /// Each SubD quad will generate 1 display mesh quads in a 1x1 grid.
    /// This density can only be used with SubDs where every face is a quad.
    /// User interface code never returns this density. 
    ///</summary>
    MinimumDensity = 0,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 4 display mesh quads in a 2x2 grid and each SubD N-gon will generate N display mesh quads.
    /// Adaptive reductions do not apply to this density.
    /// This is the minimum SubD display density.
    ///</summary>
    ExtraCoarseDensity = 1,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 16 display mesh quads in a 4x4 grid and each SubD N-gon will generate N*4 display mesh quads.
    /// Adaptive reductions do not apply to this density.
    ///</summary>
    CoarseDensity = 2,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 64 display mesh quads in an 8x8 grid and each SubD N-gon will generate N*8 display mesh quads.
    /// When a SubD has more than 8000 faces, adaptive MediumDensity is reduced to CoarseDensity.
    ///</summary>
    MediumDensity = 3,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 256 display mesh quads in a 16x16 grid and each SubD N-gon will generate N*16 display mesh quads.
    /// When a SubD has more than 2000 faces, adaptive FineDensity is reduced to adaptive MediumDensity.
    ///</summary>
    FineDensity = 4,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 256 display mesh quads in a 16x16 grid and each SubD N-gon will generate N*16 display mesh quads.
    /// When a SubD has more than 2000 faces, adaptive DefaultDensity is reduced to adaptive MediumDensity.
    /// This is the default value for creating mesh approximations of SubD surface.  
    /// When treadted as an adaptive setting, it produces acceptable results for most SubDs.
    ///</summary>
    DefaultDensity = 4,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 1024 display mesh quads in a 32x32 grid and each SubD N-gon will generate N*32 display mesh quads.
    /// When a SubD has more than 500 faces, adaptive ExtraFineDensity is reduced to adaptive FineDensity.
    ///</summary>
    ExtraFineDensity = 5,

    ///<summary>
    /// When interpreted as an absolute SubD display density, each SubD quad will generate 
    /// 4096 display mesh quads in a 64x64 grid and each SubD N-gon will generate N*64 display mesh quads.
    /// ON_SubDDisplayParameters.AdaptiveDensity() determines if the subd display density is 
    /// treated adaptively or absolutely.
    /// This value creates ridiculously dense display meshes and should generally be avoided. 
    /// No Rhino user interface will create this value.
    ///</summary>
    MaximumDensity = 6,
  };

  enum : unsigned int
  {
    /// <summary>
    /// When the SubD display density is adaptive (default), AdaptiveMeshQuadMaximum
    /// specifies the approximate number of display mesh quads to permit before
    /// reducing the SubD display mesh density. 
    /// Approximate display mesh quad count = subd.FaceCount()*(4^subd_display_density).
    /// This enum value may change from release to release as rendering technology improves.
    /// Make sure your code works for values between 1024 and 134217728. 
    /// </summary>
    AdaptiveDisplayMeshQuadMaximum = 512000
  };

public:
  static const ON_SubDDisplayParameters Empty;

  // Parameters for a course limit surface display mesh.
  // SubD display density = adaptive ON_SubDDisplayParameters::ExtraCoarseDensity
  static const ON_SubDDisplayParameters ExtraCoarse;

  // Parameters for a course limit surface display mesh.
  // SubD display density = adaptive ON_SubDDisplayParameters::CoarseDensity
  static const ON_SubDDisplayParameters Coarse;

  // Parameters for a medium limit surface display mesh.
  // SubD display density = adaptive ON_SubDDisplayParameters::MediumDensity
  // Too crude for a quality rendering of the SubD Rhino logo.
  static const ON_SubDDisplayParameters Medium;

  // Parameters for the default limit surface display mesh.
  // Produces and acceptable rendering of the SubD Rhino logo.
  // SubD display density = adaptive ON_SubDDisplayParameters::FineDensity (default)
  static const ON_SubDDisplayParameters Fine;

  // Parameters for an extra fine limit surface display mesh.
  // SubD display density = adaptive ON_SubDDisplayParameters::ExtraFineDensity
  static const ON_SubDDisplayParameters ExtraFine;

  // Parameters for the default limit surface display mesh.
  // SubD display density = adaptive ON_SubDDisplayParameters::DefaultDensity
  static const ON_SubDDisplayParameters Default;

  /*
  Parameters:
    adaptive_subd_display_density - [in]
      A value <= ON_SubDDisplayParameters::MaximumDensity.
      When in doubt, pass ON_SubDDisplayParameters::DefaultDensity.
      Invalid input values are treated as ON_SubDDisplayParameters::DefaultDensity.
    subd_face_count - [in]
      Number of SubD faces. 
      When subd_face_count = 0, adaptive_subd_display_density is returned.
  Returns:
    The absolute SubD display density for SubD with subd_face_count faces.
    The absolute SubD display density is <= adaptive_subd_display_density and <= ON_SubDDisplayParameters::MaximumDensity.
  */
  static unsigned int AbsoluteDisplayDensityFromSubDFaceCount(
    unsigned adaptive_subd_display_density,
    unsigned subd_face_count
  );

  /*
  Parameters:
    adaptive_subd_display_density - [in]
      A value <= ON_SubDDisplayParameters::MaximumDensity.
      When in doubt, pass ON_SubDDisplayParameters::DefaultDensity.
      Invalid input values are treated as ON_SubDDisplayParameters::DefaultDensity.
    subd - [in]
      In the cases when the subd in question is not available, like user interface code that applies in general 
      and to unknown SubDs, pass ON_SubD::Empty.
  Returns:
    The absolute SubD display density for subd.
    The absolute SubD display density is <= adaptive_subd_display_density and <= ON_SubDDisplayParameters::MaximumDensity.
  */
  static unsigned int AbsoluteDisplayDensityFromSubD(
    unsigned adaptive_subd_display_density,
    const class ON_SubD& subd
  );

  /*
  Description:
    In most applications, the caller sets the mesh_density
    and leaves the other parameters set to the default
    values.
  Parameters:
    adaptive_subd_display_density - [in]
      A value <= ON_SubDDisplayParameters::MaximumDensity.
      When in doubt, pass ON_SubDDisplayParameters::DefaultDensity.
      Values < ON_SubDDisplayParameters::MinimumAdaptiveDensity are treated as N_SubDDisplayParameters::MinimumAdaptiveDensity.
      All other invalid input values are treated as ON_SubDDisplayParameters::DefaultDensity.
  Returns:
    A ON_SubDDisplayParameters with adaptive SubD display density.
  */
  static const ON_SubDDisplayParameters CreateFromDisplayDensity(
    unsigned int adaptive_subd_display_density
  );

  /*
  Description:
    Use of absolute display density is strongly discouraged. 
    SubDs can have a single face or millions of faces.
    Adaptive display meshing produces more desirable results in almost all cases.
  Parameters:
    absolute_subd_display_density - [in]
      A value <= ON_SubDDisplayParameters::MaximumDensity.
      When in doubt, pass ON_SubDDisplayParameters::DefaultDensity.
  Returns:
    A ON_SubDDisplayParameters that treats the display density value as a constant for all SubDs.
  */
  static const ON_SubDDisplayParameters CreateFromAbsoluteDisplayDensity(
    unsigned int absolute_subd_display_density
  );

  /*
  Description:
    This function creates ON_SubDDisplayParameters from a user interface
    "slider" like Rhino's simple mesh controls.
  Parameters:
    normalized_mesh_density - [in]
      A double between 0.0 and 1.0

      The table below shows the correspondence between normalized_density and subd display density.

      Mesh density percentage / normalized_mesh_density / subd display density
      0% ->  [0.0, ON_ZERO_TOLERANCE] -> 1 = adaptive MinimumUserInterfaceDensity
      0% to 19% -> (ON_ZERO_TOLERANCE, 0.20) -> 2 = adaptive CoarseDensity
      20% to 34% -> [0.20, 0.35) -> 3 = adaptive MediumDensity
      35% to 75% ->   [0.35, 0.75] -> 4 = adaptive FineDensity
      76% to 99% -> (0.75, 1 - ON_ZERO_TOLERANCE) -> 5 = adaptive ExtraFineDensity
      100% -> [1 - ON_ZERO_TOLERANCE, 1.0] -> 5 = adaptive MaximumUserInterfaceDensity
      
      Invalid input -> adaptive DefaultDensity;

   Returns:
     A valid ON_SubDDisplayParameters with the specified subd display density.
  */
  static const ON_SubDDisplayParameters CreateFromMeshDensity(
    double normalized_mesh_density
  );

public:

  ON_DEPRECATED_MSG("Use DisplayDensity(subd)")
  unsigned int DisplayDensity() const;

  /*
  Returns:
    True if the SubD display density setting is adaptive and approximate display
    mesh quad count is capped at ON_SubDDisplayParameters::AdaptiveDisplayMeshQuadMaximum.
  Remarks:
    this->DensityIsAdaptive() and this->DensityIsAbsolute() always return opposite bool values.
    Use the one that makes your code easiest to read and understand.
  */
  bool DisplayDensityIsAdaptive() const;

  /*
  Returns:
    True if the SubD display density setting is absolute.
  Remarks:
    this->DensityIsAdaptive() and this->DensityIsAbsolute() always return opposite bool values.
    Use the one that makes your code easiest to read and understand.
  */
  bool DisplayDensityIsAbsolute() const;

  /*
  Parameters:
    subd - [in]
      Used when the display density is adaptive and subd.FaceCount() > 0. 
      Ignored when the display density is absolute or subd.FaceCount() = 0.

  Returns:
    The absolute display density to use when creating display meshes for subd.
    When adaptive reduction is enabled, subd.FaceCount() is used to determine
    the appropriate display density.

  Remarks:
    The chart below shows the relationship between the returned value and the 
    number of display mesh quads a generated by 1 SubD quad.

    return    display mesh
    value     quads
    0             1 =  1x1
    1             4 =  2x2
    2            16 =  4x4
    3            64 =  8x8            
    4           128 = 16x16
    5         1,024 = 32x32
    6         4,096 = 64x64
  */
  unsigned int DisplayDensity(
    const class ON_SubD& subd
  ) const;

  /*
  Returns:
    The raw value of m_display_density.
  Remarks:
    This function is only intended to get
    public raw access to that value (for example when comparing meshing parameters)
    and should not be used in any place where the returned value will be used
    to compute a SubD display density.

    Use DisplayDensity(subd) instead!
  */
  const unsigned char GetRawDisplayDensityForExperts() const;

  ON_DEPRECATED_MSG("Use SetAdaptiveDisplayDensity()")
  void SetDisplayDensity(
    unsigned int adaptive_display_density
  );

  /*
  Description:
    Set an adaptive SubD display density that caps display mesh quad count at ON_SubDDisplayParameters::AdaptiveDisplayMeshQuadMaximum.
  Parameters:
    adaptive_display_density - [in]
      adaptive_display_density <= ON_SubDDisplayParameters::MaximumDensity
      Values <= ON_SubDDisplayParameters::MinimumAdaptiveDensity will never be adaptively reduced during display mesh creation.
  Remarks:
    The use of this setting
  */
  void SetAdaptiveDisplayDensity(
    unsigned int adaptive_display_density
  );

  /*
  Description:
    In almost all cases, you are better off using SetAdaptiveDisplayDensity().
  Parameters:
    absolute_display_density - [in]
      absolute_display_density <= ON_SubDDisplayParameters::MaximumDensity
  Remarks:
    The use of this setting 
  */
  void SetAbsoluteDisplayDensity(
    unsigned int absolute_display_density
  );

public:


  /*
  Description:
    The MeshLocation() property determines if the mesh is
    on the SubD's control net or the SubD's surface.
  */
  ON_SubDComponentLocation MeshLocation() const;

  /*
  Description:
    The MeshLocation() property determines if the mesh is
    on the SubD's control net or the SubD's surface.
  Parameters:
    mesh_location - [in]
  */
  void SetMeshLocation(ON_SubDComponentLocation mesh_location);

  /*
  Description:
    The ComputeCurvature() property determines if the mesh has curvature values.
  */
  bool ComputeCurvature() const;

  /*
  Description:
    The ComputeCurvature() property determines if the mesh has curvature values.
  Parameters:
    compute_curvature - [in]
  */
  void SetComputeCurvature(bool compute_curvature);

  unsigned char EncodeAsUnsignedChar() const;
  static const ON_SubDDisplayParameters DecodeFromUnsignedChar(
    unsigned char encoded_parameters
  );

private:
  enum : unsigned char
  {
    // do not change these values - they control how m_subd_mesh_parameters is set
    // and the value of m_subd_mesh_parameters is saved in 3dm archives.
    subd_mesh_density_mask = 0x07,
    subd_mesh_location_bit = 0x08,
    subd_mesh_absolute_density_bit = 0x10,
    subd_mesh_compute_curvature_bit = 0x20,

    // If this bit set, then the settings are not current defaults.
    subd_mesh_nondefault_bit = 0x80
  };

private:
  // If n = absolute_display_density, then each SubD quad face will have 
  // a grid of 2^n x 2^n mesh quads for a total of 4^) mesh quads.
  // n   grid size    total number of mesh faces per SubD quad
  // 0     1 x 1             1
  // 1     2 x 2             4
  // 2     4 x 4            16
  // 3     8 x 8            64
  // 4    16 x 16          256
  // 5    32 x 32        1,024
  // 6    64 x 64        4,096

  // m_bAbsoluteDisplayDensity determines if m_display_density is adaptive or absolute.
  bool m_bDisplayDensityIsAbsolute = false; // default must be false so 7.0 to 7.1 transition works correctly
  unsigned char m_display_density = 0; // SubD display density (0,1,2,3,4,5,6)

  unsigned short m_reserved = 0;

  // If m_bControlNetMesh is false, a mesh of the limit surface is produced.
  // If m_bControlNetMesh is true, a mesh of the subdivided control net is produced.
  bool m_bControlNetMesh = false;

public:
  enum class Context : unsigned char
  {
    ///<summary>
    /// Unknown, unspecified, or unset context. This is typical.
    ///</summary>
    Unknown = 0,

    ///<summary>
    /// These parameters are being used to generate a quad mesh approximations of an ON_SubD.
    /// Low level meshing code copies input parameters and specifies this context when appropriate.
    ///</summary>
    SubDToMesh = 1,

    ///<summary>
    /// These parameters are being used to generate NURBS surface approximations of an ON_SubD.
    /// Low level conversion to NURB code copies input parameters and specifies this context when appropriate.
    ///</summary>
    SubDToNURBS = 2
  };


  /*
  Description:
    Low level mesh creation and ON_SubD to NURBS conversion code
    occasional looks at the context. Typically it is set in a local
    copy and no user of the top level ON_SubD SDK needs to be concerned
    about the context setting. 
    This setting is not saved in 3dm archives and is ignored by all compare functions.
  */
  ON_SubDDisplayParameters::Context ContextForExperts() const;

  void SetContextForExperts(ON_SubDDisplayParameters::Context context);

private:
  ON_SubDDisplayParameters::Context m_context = ON_SubDDisplayParameters::Context::Unknown;

  bool m_bComputeCurvature = false;
  unsigned char m_reserved3 = 0;
  unsigned int m_reserved4 = 0;
  unsigned int m_reserved5 = 0;
  ON__UINT_PTR m_reserved6 = 0;
  double m_reserved7 = 0.0;

public:
  // TODO - split this class into two - what's above and one derived from that with what's below.
  ON_Terminator* Terminator() const;
  void SetTerminator(
    ON_Terminator* terminator
  );

  ON_ProgressReporter* ProgressReporter() const;  
  const ON_Interval ProgressReporterInterval() const;
  void SetProgressReporter(
    ON_ProgressReporter* progress_reporter,
    ON_Interval progress_reporter_interval
  );

public:
  bool Write(class ON_BinaryArchive& archive) const;
  bool Read(class ON_BinaryArchive& archive);
   
private:
  ON_Terminator* m_terminator = nullptr;

private:
  // optional progress reporting
  ON_ProgressReporter* m_progress_reporter = nullptr;
  ON_Interval m_progress_reporter_interval = ON_Interval::ZeroToOne;
};

///////////////////////////////////////////////////////////////////////////////
//
// Class  ON_Mesh
//
class ON_CLASS ON_MeshParameters
{
  // surface meshing parameters
public:

  // The Rhino legacy mesher is the mesher used in Rhino 1, 2, 3, 4, 5, 6.
  // {F15F67AA-4AF9-4B25-A3B8-517CEDDAB134}
  static const ON_UUID RhinoLegacyMesherId;

  // {EB6F6F3F-F975-4546-9D1C-64E9423BEB7F}
  static const ON_UUID PangolinMesherId;

  enum class MESH_STYLE : unsigned char
  {
    // All of these enum values must be in the range 0-255 because
    // unsigned chars are use for storage in some locations.
    unset_mesh_style      =   0,
    render_mesh_fast      =   1, // Use ON_MeshParameters::FastRenderMesh
    render_mesh_quality   =   2, // Use ON_MeshParameters::QualityRenderMesh
    // 3 - 8 reserved for future predefined render mesh styles
    render_mesh_custom    =   9,// Use ON_3dmSettings::m_CustomRenderMeshSettings
    render_mesh_per_object = 10 // Use ON_Object::GetMeshParameters().
  };

  static ON_MeshParameters::MESH_STYLE MeshStyleFromUnsigned(
    unsigned int mesh_style_as_unsigned
  );


  //////////////////////////////////////////////////////////////
  //
  // The MESH_PARAMETER_ID enum values are used to identify
  // mesh creation parameters.
  //
  enum class MESH_PARAMETER_ID : unsigned int
  {
    unspecified_mesh_parameter_id = 0,
    
    ////////////////////////////////////////////////////////
    // BEGIN Legacy parameters.
    //

    bComputeCurvature_parameter_id = 1,
    bSimplePlanes_parameter_id = 2,
    bRefine_parameter_id = 3,
    bJaggedSeams_parameter_id = 4,
    bDoublePrecision_parameter_id = 5,
    mesher_parameter_id = 6,
    texture_range_parameter_id = 7,
    tolerance_parameter_id = 8,
    relative_tolerance_parameter_id = 9,
    min_tolerance_parameter_id = 10,
    min_edge_length_parameter_id = 11,
    max_edge_length_parameter_id = 12,
    grid_aspect_ratio_parameter_id = 13,
    grid_min_count_parameter_id = 14,
    grid_max_count_parameter_id = 15,
    grid_angle_parameter_id = 16,
    grid_amplification_parameter_id = 17,
    refine_angle_parameter_id = 18,
    face_type_parameter_id = 19,
    srf_domain_parameter_id = 20,
    bClosedObjectPostProcess_id = 21,

    //
    // END Legacy parameters.
    ////////////////////////////////////////////////////////

    // UUID parameter identifying what mesher code created the mesh.
    mesher_id = 22,

    ////////////////////////////////////////////////////////
    // BEGIN Pangolin parameters
    //

    crv_tess_min_num_segments_parameter_id = 23,
    crv_tess_angle_tol_in_degrees_parameter_id = 24,
    crv_tess_max_dist_between_points_parameter_id = 25, // Not same as 'max_edge_length_parameter_id' since
                                                        // 'curve_tess_max_dist_between_points' is only for
                                                        // curves, not surfaces.
    crv_tess_min_parametric_ratio_parameter_id = 26,
    bEvaluatorBasedTessellation_parameter_id = 27,
    srf_tess_chord_height_parameter_id = 28, // Not same as 'tolerance_parameter_id' since
                                             // 'surface_tess_chord_height' is only for
                                             // surfaces, not curves.
    srf_tess_angle_tol_in_degrees_parameter_id = 29,
    srf_tess_max_edge_length_parameter_id = 30,
    srf_tess_min_edge_length_parameter_id = 31,
    srf_tess_min_edge_length_ratio_uv_parameter_id = 32,
    srf_tess_max_aspect_ratio_parameter_id = 33,
    smoothing_passes_parameter_id = 34,

    //
    // END Pangolin parameters
    ////////////////////////////////////////////////////////

    max_mesh_parameter_id
  };

  static ON_MeshParameters::MESH_PARAMETER_ID MeshParameterIdFromUnsigned(
    unsigned int mesh_parameter_id_as_unsigned
  );

  /// <summary>
  /// ON_MeshParameter::Type identifies the type of mesh creation settings.
  /// </summary>
  enum class Type : unsigned char
  {
    /// <summary>
    /// Not set.
    /// </summary>
    Unset = 0,

    /// <summary>
    /// Default mesh creation settings from ON_MeshParameters::DefaultMesh.
    /// </summary>
    Default = 1,

    /// <summary>
    /// Fast render mesh creation settings from ON_MeshParameters::FastRenderMesh.
    /// </summary>
    FastRender = 2,

    /// <summary>
    /// Quality render mesh creation settings from ON_MeshParameters::QualityRenderMesh.
    /// </summary>
    QualityRender = 3,

    /// <summary>
    /// Default analysis mesh creation settings from ON_MeshParameters::DefaultAnalysisMesh
    /// </summary>
    DefaultAnalysis = 4,

    /// <summary>
    /// Mesh density settings from ON_MeshParameters::CreateFromMeshDensity(normalized_mesh_density). 
    /// The value of normalized_mesh_density is returned by the MeshDensity() property.
    /// </summary>
    FromMeshDensity = 5,

    /// <summary>
    /// Mesh creation settings are set and are not from one of the cases listed above.
    /// </summary>
    Custom = 15
  };

  /*
  Returns:
    The type of geometry settings taking SubD parameters into account.
  Remarks:
    This function will never return ON_MeshParameters::Type::Unset.
    In particular, if the return value is not ON_MeshParameters::Type::Custom,
    then the settings come from one of the built-in mesh creation settings.
  */
  ON_MeshParameters::Type GeometrySettingsType() const;

  /*
  Returns:
    The type of geometry settings.
  Parameters:
    bIgnoreSubDParameters - [in]
      If true, SubD meshing parameters are ignored in determining the type.
  Remarks:
    This function will never return ON_MeshParameters::Type::Unset.
    In particular, if the return value is not ON_MeshParameters::Type::Custom,
    then the settings come from one of the built-in mesh creation settings.
  */
  ON_MeshParameters::Type GeometrySettingsType(
    bool bIgnoreSubDParameters
  ) const;

  /*
  Description:
    Mesh creation parameters to create the default render mesh.
  */
  static 
  const ON_MeshParameters DefaultMesh;

  /*
  Description:
    Mesh creation parameters to create the a render mesh
    when meshing speed is preferred over mesh quality.
  */
  static 
  const ON_MeshParameters FastRenderMesh;

  /*
  Description:
    Mesh creation parameters to create the a render mesh
    when mesh quality is preferred over meshing speed.
  */
  static 
  const ON_MeshParameters QualityRenderMesh;

  /*
  Description:
    Mesh creation parameters to create the default analysis mesh.
  */
  static 
  const ON_MeshParameters DefaultAnalysisMesh;

  /*
  Returns:
    "Fast" if this and ON_MeshParameters::FastRenderMesh have the same geometry settings.
    "Quality" if this and ON_MeshParameters::QualityRenderMesh have the same geometry settings.
    "Density(p%)" if this and ON_MeshParameters::CreateFromMeshDensity(p/100.0) have the same geometry settings.
    "Default" if this and ON_MeshParameters::DefaultMesh have the same geometry settings.
    "DefaultAnalysis" if this and ON_MeshParameters::DefaultAnalysis have the same geometry settings.
    Otherwise, "Custom(SHA1)" where SHA1 = this->GeometryHash();
  */
  const ON_wString Description() const;


  /*
  Description:
    This function creates ON_MeshParameters from a user interface
    "slider" like Rhino's simple mesh controls.
  Parameters:
    normalized_mesh_density - [in]
      A double between 0.0 and 1.0.
      0.0 creates meshes with fewer faces than 1.0.

      Invalid input is treated as 0.5.
   Returns:
     A valid ON_MeshParameters with the specified subd display density.
  */
  static const ON_MeshParameters CreateFromMeshDensity(
    double normalized_mesh_density
  );

  /*
  Returns:
    If these mesh parameters, including the SubD meshing parameters, were created from ON_MeshParameters::CreateFromMeshDensity(normalized_mesh_density),
    then normalized_mesh_density is returned.
    Otherwise, ON_DBL_QNAN is returned.
  Remarks:
    The values of m_bDoublePrecision, m_bClosedObjectPostProcess, and m_texture_range can be arbitrary
    because they do not determine geometry of the resulting mesh and are typically ignored. 
    You must compare these properties if they matter in your particular context.
  */
  double MeshDensity() const;

  /*
  Parameters:
    bIgnoreSubDParameters - [in]
      If true, SubD meshing parameters are ignored.
  Returns:
    If these mesh parameters were created from ON_MeshParameters::CreateFromMeshDensity(normalized_mesh_density),
    then normalized_mesh_density is returned.
    Otherwise, ON_DBL_QNAN is returned.
  Remarks:
    The values of m_bDoublePrecision, m_bClosedObjectPostProcess, and m_texture_range can be arbitrary
    because they do not determine geometry of the resulting mesh and are typically ignored.
    You must compare these properties if they matter in your particular context.
  */
  double MeshDensity(bool bIgnoreSubDParameters ) const;

  /*
  Description:
    Convert a mesh density value to a percentage with finite precision fuzz removed.
  Parameters:
    normalized_mesh_density - [in]
      valid input is 0.0 <= normalized_mesh_density <= 1.0
  Returns:
    If normalized_density is valid, 100*normalized_density with fuzz cleaned up is returned.
    Otherwise ON_DBL_QNAN is returned.
  */
  static double MeshDensityAsPercentage(double normalized_mesh_density);

  /*
  Description:
    This function cleans up normalized_density used in 
    ON_MeshParameters::CreateFromMeshDensity() 
    and ON_SubDDisplayParameters CreateFromMeshDensity().
  Parameters:
    normalized_mesh_density - [in]
      should be close to being between 0 and 1.
   Returns:
      if normalized_density is between 0.0 and 1.0, that value is returned.
      If normalized_density is a hair smaller than 0.0, then 0.0 is returned.
      If normalized_density is a hair bigger than 1.0, then 1.0 is returned.
      Otherwise 0.5 is returned.
  */
  static double ClampMeshDensityValue(double normalized_mesh_density);


  /*
  Description:
    Get a value to use for tolerance based on the relative_tolerance
    and actual size.
  Parameters:
    relative_tolerance - [in] 
      See m_relative_tolerance field
    actual_size - [in]
      Length of object's bounding box diagonal or some similar 
      measure of the object's 3d size.
  Returns:
    A value that can be used for m_tolerance if no
    user specified value is available.
  */
  static
  double ToleranceFromObjectSize( double relative_tolerance, double actual_size );

  /*
  Description:
    Get a value to use for minimum edge length base on max_edge_length
    and tolerance settings.
  Parameters:
    max_edge_length - [in] 
      3d maximum edge length used to create mesh.
    tolerance - [in]
      3d distance tolerance used to create mesh.
  Returns:
    A value that can be used for m_min_edge_length if no
    user specified value is available.
  */
  static
  double MinimumEdgeLengthFromTolerance( double max_edge_length, double tolerance );

  ON_MeshParameters() = default;
  ~ON_MeshParameters() = default;
  ON_MeshParameters(const ON_MeshParameters&) = default;
  ON_MeshParameters& operator=(const ON_MeshParameters&) = default;

  /*
  Description:
    Tool for providing a simple "slider" interface.
  Parameters:
    normalized_mesh_density - [in] 0.0 <= normalized_mesh_density <= 1.0
      0 quickly creates extremely coarse meshes.
      1 slowly creates extremely dense meshes.
    min_edge_length - [in]
      > 0.0 custom value
      ON_UNSET_VALUE: for default (0.0001)
  Remarks:
    If you are using a user interface "slider" to set mesh parameters, 
    then you are strongly encouraged to call ON_MeshParameters::CreateFromMeshDensity()
    instead of using this constructor. 
    ON_MeshParameters::CreateFromMeshDensity() handles out of bounds input
    in a predictable way and is easier to search for when examining code.
  */
  ON_MeshParameters(
    double normalized_mesh_density,
    double min_edge_length = ON_UNSET_VALUE
    );

  // C++ default works fine // ON_MeshParameters(const ON_MeshParameters& );
  // C++ default works fine // ON_MeshParameters& operator=(const ON_MeshParameters&);
 
  void Dump( ON_TextLog& test_log ) const;


  /*
  */
  static int Compare( 
    const ON_MeshParameters& a,
    const ON_MeshParameters& b
    );

  /*
  Description:
    Compares all meshing parameters that control mesh geometry.
    Does not compare m_bCustomSettings, CustomSettingsEnabled(),
    m_bComputeCurvature, m_bDoublePrecision, MinimumTolerance(),
    m_texture_range, m_srf_domain0 and m_srf_domain1; and 
    from the SubD meshing parameters in m_subd_mesh_parameters_as_char,
    does not compare m_context, m_bComputeCurvature, m_terminator,
    m_progress_reporter, and m_progress_reporter_interval.
  */
  static int CompareGeometrySettings(
    const ON_MeshParameters& a,
    const ON_MeshParameters& b
    );

  /*
  Description:
    Compares all meshing parameters that control mesh geometry.
    Does not compare m_bCustomSettings, CustomSettingsEnabled(),
    m_bComputeCurvature, m_bDoublePrecision, MinimumTolerance(),
    m_texture_range, m_srf_domain0 and m_srf_domain1; and 
    from the SubD meshing parameters in m_subd_mesh_parameters_as_char,
    does not compare m_context, m_bComputeCurvature, m_terminator,
    m_progress_reporter, and m_progress_reporter_interval.
    If bIgnoreSubDParameters is true, ignores all settings in
    m_subd_mesh_parameters_as_char.
  */
  static int CompareGeometrySettings(
    const ON_MeshParameters& a,
    const ON_MeshParameters& b,
    const bool bIgnoreSubDParameters
    );

  /*
  Returns:
    A hash of every ON_MeshParameters setting.
  Remarks:
    The hash intentionally ignores m_bCustomSettingsEnabled or m_bDoublePrecision.
  */
  ON_SHA1_Hash ContentHash() const;

  /*
  Returns:
    A hash of values that control mesh geometry.
  Remarks:
    This has intentionally ignored
    m_bCustomSettings, m_bCustomSettingsEnabled, m_bComputeCurvature, 
    m_bDoublePrecision, m_bClosedObjectPostProcess, m_texture_range.
    If you need to include those values, call ContentHash().
  */
  ON_SHA1_Hash GeometrySettingsHash() const;

  /*
  Parameters:
    bIgnoreSubDParameters - [in]
      If true, the SubD meshing parameters are not hashed.
  Returns:
    A hash of values that control mesh geometry.
  Remarks:
    This has intentionally ignored
    m_bCustomSettings, m_bCustomSettingsEnabled, m_bComputeCurvature,
    m_bDoublePrecision, m_bClosedObjectPostProcess, m_texture_range.
    If you need to include those values, call ContentHash().
  */
  ON_SHA1_Hash GeometrySettingsHash(
    bool bIgnoreSubDParameters
  ) const;

  ON_UUID MesherId() const;
  void SetMesherId(
    ON_UUID
  );

  /*
  Returns:
    ON_MeshParameters::render_mesh_fast
      ON_MeshParameters::FastRenderMesh and this have the same geometry settings
    ON_MeshParameters::render_mesh_quality
      ON_MeshParameters::QualityRenderMesh and this have the same geometry settings
    ON_MeshParameters::render_mesh_custom
      custom_mp is not null and has the same geometry settings
    no_match_found_result
      otherwise
  */
  const ON_MeshParameters::MESH_STYLE GeometrySettingsRenderMeshStyle(
    const ON_MeshParameters* custom_mp,
    ON_MeshParameters::MESH_STYLE no_match_found_result
    ) const;

  /*
  Returns:
    n in the range 0 to 100, inclusive, when 
      (0 == ON_MeshParameters::CompareGeometrySettings(*this,ON_MeshParameters(n/100.0))
    no_match_found_result:
      otherwise
  Remarks:
    This is a legacy function with roots dating back to Rhino 1.0. 
    High quality code should use ON_MeshParameters::MeshDensity() or
    ON_MeshParameters::MeshDensityAsPercentage(ON_MeshParameters::MeshDensity()).
  */
  const int GeometrySettingsDensityPercentage(
    int no_match_found_result
    ) const;

  bool Write( ON_BinaryArchive& ) const;
  bool Read( ON_BinaryArchive& );

  ON__UINT32 DataCRC(ON__UINT32) const;
    
  //////////////////////////////////////////////////////////////
  //
  // The CustomSettings() parameter applies when these mesh 
  // creation parameters specify how an object's mesh should
  // be created and these parameters should override the
  // the model or application default mesh creation parameters.
  //
  // When CustomSettings() is true, it indicates these mesh 
  // creation parameters are explicitly set for the object
  // and context in question and should override the model
  // or application defaults.
  //
  // When CustomSettings() is false, it indicates these mesh
  // creation parameters were inherited from from model or 
  // application defaults and any mesh created with these
  // parameters should be updated when these parameters
  // differ from the current model or application defaults.
  //
  const bool CustomSettings() const;
  void SetCustomSettings(
    bool bCustomSettings
  );

  //////////////////////////////////////////////////////////////
  //
  // The CustomSettingsEnabled() value applies to mesh creation
  // parameters that are on ON_3dmObjectAttributes and have
  // CustomSettings() = true.  In this situation:
  //
  //   If CustomSettingsEnabled() is true, then the use of 
  //   these mesh creation parameters is enabled.
  //
  //   If CustomSettingsEnabled() is false, then these mesh
  //   creation parameters should be ignored.
  //
  const bool CustomSettingsEnabled() const;
  void SetCustomSettingsEnabled(
    bool bCustomSettingsEnabled
  );


  //////////////////////////////////////////////////////////////
  //
  // Meshing happens in two stages.  The first stage creates a
  // rectangular grid.  The second stage refines the grid until
  // the mesh meets all meshing requirements.  The third stage
  // combines coincident vertices if the resulting mesh is a composite.
  // 


  // false - (default) - ON_Mesh::m_K[] not computed
  // true  - ON_Mesh::m_K[] computed  bool ComputeCurvature() const;
  const bool ComputeCurvature() const;
  void SetComputeCurvature(
    bool bComputeCurvature
  );

  // false - (default) planar surfaces are meshed
  //          using the controls below.
  // true   - planar surfaces are meshed using
  //          minimal number of triangles and
  //          aspect/edge controls are ignored.
  const bool SimplePlanes() const;
  void SetSimplePlanes(
    bool bSimplePlanes
  );

public:
  void SetSubDDisplayParameters(
    const class ON_SubDDisplayParameters& subd_mesh_parameters
    );

  const ON_SubDDisplayParameters SubDDisplayParameters() const;
  

public:
  // false - skip stage 2 mesh refinement step
  // true  - (default) do stage 2 mesh refinement step
  const bool Refine() const;
  void SetRefine(
    bool bRefine
  );

public:
  // false - (default) edges of meshes of joined 
  //          b-rep faces match with no gaps or
  //          "T" joints.
  // true   - faces in b-reps are meshed independently.
  //          This is faster but results in gaps and
  //          "T" joints along seams between faces.
  const bool JaggedSeams() const;
  void SetJaggedSeams(
    bool bJaggedSeams
  );

public:
  // false - (default) the mesh vertices will be 
  //         float precision values in the m_V[] array.
  // true -  The mesh vertices will be double precision
  //         values in the DoublePrecisionVertices()
  //         array.  Float precision values will also
  //         be returned in the m_V[] array.
  const bool DoublePrecision() const;
  void SetDoublePrecision(
    bool bDoublePrecision
  );

  // 0 = slow mesher, 1 = fast mesher
  const unsigned int Mesher() const;
  void SetMesher(
    unsigned int mesher
  );
    
  // 1: unpacked, unscaled, normalized
  //   each face has a normalized texture range [0,1]x[0,1].
  //   The normalized coordinate is calculated using the
  //   entire surface domain.  For meshes of trimmed
  //   surfaces when the active area is a small subset of
  //   the entire surface, there will be large regions of
  //   unused texture space in [0,1]x[0,1].  When the 3d region
  //   being meshed is far from being square-ish, there will be
  //   a substantial amount of distortion mapping [0,1]x[0,1]
  //   texture space to the 3d mesh.
  //   
  // 2: packed, scaled, normalized (default)
  //   each face is assigned a texture range that is a 
  //   subrectangle of [0,1]x[0,1].  The subrectangles are 
  //   mutually disjoint and packed into into [0,1]x[0,1]
  //   in a way that minimizes distortion and maximizes the
  //   coverage of [0,1]x[0,1].
  //   When the surface or surfaces being meshed are trimmed,
  //   this option takes into account only the region of the
  //   base surface the mesh covers and uses as much of 
  //   [0,1]x[0,1] as possible.  unsigned int TextureRange() const;
  const unsigned int TextureRange() const;
  void SetTextureRange(
    unsigned int texture_range
  );
  const bool TextureRangeIsValid() const;
  void SetTextureRangePictureFrameHack();

  // If the object being meshed is closed, m_bClosedObjectPostProcess is true, 
  // m_bJaggedSeams = false, and the resulting mesh is not closed, then a post meshing process
  // is applied to find and close gaps in the mesh. Typically the resulting mesh
  // is not closed because the input object has a geometric flaw like loops in
  // trimming curves.
  const bool ClosedObjectPostProcess() const;
  void SetClosedObjectPostProcess(
    bool bClosedObjectPostProcess
  );

  // These controls are used in both stages

  // approximate maximum distance from center of edge to surface
  const double Tolerance() const;
  void SetTolerance(
    double tolerance
  );

  /*
    If 0.0 < RelativeTolerance() < 1.0, 
    then the maximum distance from the
    center of an edge to the surface will
    be <= T, where T is the larger of
    (MinimumTolerance(),d*RelativeTolerance()), 
    where d is an estimate of the size of the
    object being meshed.
  */
  const double RelativeTolerance() const;
  void SetRelativeTolerance(
    double relative_tolerance
  );

  const double MinimumTolerance() const;
  void SetMinimumTolerance(
    double minimum_tolerance
  );

  // edges shorter than MinimumEdgeLength() will
  // not be split even if the do not meet other
  // meshing requirements
  const double MinimumEdgeLength() const;
  void SetMinimumEdgeLength(
    double minimum_edge_length
  );

public:
  // edges longer than MaximumEdgeLength() will
  // be split even when they meet all other
  // meshing requirements
  const double MaximumEdgeLength() const;
  void SetMaximumEdgeLength(
    double maximum_edge_length
  );

  ////////////////////////////////////////////////////////////////////////////////////
  //
  // These controls are used during stage 1 to generate the grid
  //


  // desired aspect ratio of quads in grid
  // 0.0 = any aspect ratio is acceptable
  // values >0 and < sqrt(2) are treated as sqrt(2)
  const double GridAspectRatio() const;
  void SetGridAspectRatio(
    double grid_aspect_ratio
  );

  // minimum number of quads in initial grid
  const int GridMinCount() const;
  void SetGridMinCount(
    int grid_min_count
  );

  // desired maximum number of quads in initial grid
  const int GridMaxCount() const;
  void SetGridMaxCount(
    int grid_max_count
  );

  // maximum angle (radians) between surface
  // normal evaluated at adjacent vertices.
  // 0.0 is treated as pi.
  const double GridAngleRadians() const;
  void SetGridAngleRadians(
    double grid_angle_radians
  );

  // maximum angle (degrees) between surface
  // normal evaluated at adjacent vertices.
  // 0.0 is treated as 180.0.
  const double GridAngleDegrees() const;
  void SetGridAngleDegrees(
    double grid_angle_degrees
  );

  // The parameters above generate a grid.
  // If you want fewer quads, set m_grid_amplification
  // to a value < 1.  If you want more quads,
  // set m_grid_amplification to a value > 1.
  // default = 1 and values <= 0 are treated as 1.
  const double GridAmplification() const;
  void SetGridAmplification(
    double grid_amplification
  );

  ////////////////////////////////////////////////////////////////////////////
  //
  // These controls are used during stage 2 to refine the grid
  //


  // (in radians) maximum angle in radians between
  // surface normal evaluated at adjacent vertices.
  const double RefineAngleRadians() const;
  void SetRefineAngleRadians(
    double refine_angle_radians
  );
  const double RefineAngleDegrees() const;
  void SetRefineAngleDegrees(
    double refine_angle_degrees
  );

  ////////////////////////////////////////////////////////////////////////////
  //
  // These controls are used during stage 3
  //

  // 0 = mixed triangle and quads
  // 1 = all triangles
  // 2 = all quads
  const unsigned int FaceType() const;
  void SetFaceType(
    unsigned int face_type
  );


private:
  void Internal_SetBoolHelper(bool b, bool* dest);
  void Internal_SetCharHelper(unsigned int u, unsigned char minc, unsigned char maxc, unsigned char*);
  void Internal_SetDoubleHelper(double x, double minx, double maxx, double* dest);
  void Internal_SetIntHelper(int i, int mini, int maxi, int* dest);

private:
  //////////////////////////////////////////////////////////
  //
  // BEGIN Rhino Legacy parameters
  //
  bool m_bCustomSettings = false;
  bool m_bCustomSettingsEnabled = true;
  bool m_bComputeCurvature = false;
  bool m_bSimplePlanes = false; 
  
  bool m_bRefine = true;
  bool m_bJaggedSeams = false;
  bool m_bDoublePrecision = false;
  bool m_bClosedObjectPostProcess = false;

  ON_UUID m_mesher_id = ON_nil_uuid;
  
  unsigned char m_mesher = 0;
  unsigned char m_texture_range = 2;
  unsigned char m_face_type = 0;

  // Uses ON_SubDDisplayParameters::EncodeAsUnsignedChar() / ON_SubDDisplayParameters::DecodeFromUnsignedChar() 
  // to save ON_SubDDisplayParameters settings in this class.
  // (Done this way to avoid breaking the version 6.0 C++ public SDK because ON_SubDDisplayParameters is to big to add to this class.)
  unsigned char m_subd_mesh_parameters_as_char = 0;

  int m_grid_min_count = 0;
  int m_grid_max_count = 0;     
  mutable ON_SHA1_Hash m_geometry_settings_hash = ON_SHA1_Hash::ZeroDigest; // the cached hash includes SubD meshing parameters

  ON__UINT32 m_reserved2 = 0;

  double m_tolerance = 0.0;
  double m_relative_tolerance = 0.0;
  double m_min_tolerance = 0.0;
  double m_min_edge_length = 0.0001;
  double m_max_edge_length = 0.0;
  double m_grid_aspect_ratio = 6.0;
  double m_grid_angle_radians = 20.0*ON_PI/180.0;
  double m_grid_amplification = 1.0;
  double m_refine_angle_radians = 20.0*ON_PI/180.0;
  //
  // BEGIN Rhino Legacy parameters
  //
  //////////////////////////////////////////////////////////

private:
  bool m_reserved3 = false;
  bool m_reserved4 = false;

private:
  //////////////////////////////////////////////////////////
  //
  // BEGIN Pangolin parameters
  //

  bool m_bEvaluatorBasedTessellation = false;
  int m_curve_tess_min_num_segments = 0;
  double m_curve_tess_angle_tol_in_degrees = 20.0;
  double m_curve_tess_max_dist_between_points = 0.0;
  double m_curve_tess_min_parametric_ratio = 0.00001;
  double m_surface_tess_angle_tol_in_degrees = 20.0;
  double m_surface_tess_max_edge_length = 0.0;
  double m_surface_tess_min_edge_length = 0.0;
  double m_surface_tess_min_edge_length_ratio_uv = 0.0001;
  double m_surface_tess_max_aspect_ratio = 0.0;
  int m_smoothing_passes = 0;

private:
  void Internal_AccumulatePangolinParameters(
    const ON_MeshParameters& pangolin_defaults,
    class ON_SHA1& sha1
  ) const;

  //
  // END Pangolin parameters
  //
  //////////////////////////////////////////////////////////
private:
  // This value is permanently reserved for use by ON_SubD core code
  // that is part of public opennurbs.
  unsigned int m_subd_stuff_reserved5 = 0;

private:
  // NOTE WELL: This value cannot become a managed pointer.
  // Lots of legacy code manages ON_MeshParameter values in ways
  // that are incompatible with this class having a managed pointer.
  ON__UINT64 m_reserved6 = 0;
};

ON_DECL
bool operator!=(const ON_MeshParameters& a, const ON_MeshParameters& b);

ON_DECL
bool operator==(const ON_MeshParameters& a, const ON_MeshParameters& b);

class ON_CLASS ON_MeshCurvatureStats
{
public:
  ON_MeshCurvatureStats();
  ~ON_MeshCurvatureStats();
  ON_MeshCurvatureStats(const ON_MeshCurvatureStats& );
  ON_MeshCurvatureStats& operator=(const ON_MeshCurvatureStats&);

  void Destroy();
  void EmergencyDestroy();
  
  bool Set( ON::curvature_style,
            int,           // Kcount,
            const ON_SurfaceCurvature*, // K[]
            const ON_3fVector*, // N[] surface normals needed for normal sectional curvatures
            double = 0.0   // if > 0, value is used for "infinity"
            );

  bool Write( ON_BinaryArchive& ) const;
  bool Read( ON_BinaryArchive& );

  ON::curvature_style m_style;
  
  double m_infinity; // curvature values >= this are considered infinite
                     // and not used to compute the m_average or m_adev
  int    m_count_infinite; // number of "infinite" values
  int    m_count;    // count of "finite" values
  double m_mode;     // mode of "finite" values
  double m_average;  // average of "finite" values
  double m_adev;     // average deviation of "finite" values

  ON_Interval m_range;
};

///////////////////////////////////////////////////////////////////////////////
//
// Class  ON_MeshTopology
//

struct ON_MeshTopologyVertex
{
  // m_tope_count = number of topological edges that begin or 
  // end at this topological vertex.
  int m_tope_count;

  // m_topei[] is an array of length m_tope_count with the indices 
  // of the topological edges that begin or end at this topological
  // vertex.  Generally, these edges are listed in no particular
  // order.  If you want the edges listed "radially", then call
  // ON_MeshTopology::SortVertexEdges.
  const int* m_topei;

  // m_v_count = number of ON_Mesh vertices that correspond to 
  // this topological vertex.
  int m_v_count;

  // m_vi[] is an array of length m_v_count with the indices of the
  // ON_Mesh vertices that correspond to this topological vertex.
  const int* m_vi;
};

struct ON_MeshTopologyEdge
{
  // m_topvi[] = indices of the topological vertices where the edge begins and ends.
  int m_topvi[2];

  // m_topf_count = number of topological faces that share this topological edge.
  int m_topf_count;

  // m_topfi[] is an array of length m_topf_count with the indices of the
  // topological faces that share this topological edge.
  const int* m_topfi;
};

struct ON_CLASS ON_MeshTopologyFace
{
  /*
    m_topei[] = indices of the topological edges that bound the face.
    If m_topei[2] = m_topei[3], then the face is a triangle, otherwise
    the face is a quad.
 
    NOTE WELL:
      The topological edge with index m_topei[k] ENDS at the
      vertex corresponding to ON_MeshFace.vi[k]. So, ...

      If the face is a quad, (ON_MeshFace.vi[2]!=ON_MeshFace.vi[3]),
      the topological edge with index m_topei[0] STARTS at
      ON_MeshFace.vi[3] and ENDS at ON_MeshFace.vi[0],
      the topological edge with index m_topei[1] STARTS at
      ON_MeshFace.vi[0] and ENDS at ON_MeshFace.vi[1],
      the topological edge with index m_topei[2] STARTS at
      ON_MeshFace.vi[1] and ENDS at ON_MeshFace.vi[2], and
      the topological edge with index m_topei[3] STARTS at
      ON_MeshFace.vi[2] and ENDS at ON_MeshFace.vi[3],
      
      If the face is a triangle, (ON_MeshFace.vi[2]==ON_MeshFace.vi[3]),
      the topological edge with index m_topei[0] STARTS at
      ON_MeshFace.vi[2] and ENDS at ON_MeshFace.vi[0],
      the topological edge with index m_topei[1] STARTS at
      ON_MeshFace.vi[0] and ENDS at ON_MeshFace.vi[1],
      the topological edge with index m_topei[2] STARTS at
      ON_MeshFace.vi[1] and ENDS at ON_MeshFace.vi[2].
  */
  int m_topei[4];

  /*
    If m_reve[i] is 0, then the orientation of the edge matches the
    orientation of the face.  If m_reve[i] is 1, then the orientation
    of the edge is opposite that of the face.
  */
  char m_reve[4];

  /*
  Description:
    A topological mesh face is a valid triangle if m_topei[0], 
    m_topei[1], m_topei[2] are distinct edges and 
    m_topei[3]=m_topei[2].
  Returns:
    True if face is a triangle.
  */
  bool IsTriangle() const;

  /*
  Description:
    A topological mesh face is a valid quad if m_topei[0], 
    m_topei[1], m_topei[2], and m_topei[3] are distinct edges.
  Returns:
    True if face is a quad.
  */
  bool IsQuad() const;

  /*
  Description:
    A topological mesh face is valid if m_topei[0], m_topei[1], 
    and m_topei[2] are mutually distinct, and m_topei[3] is 
    either equal to m_topei[2] or mutually distinct from the
    first three indices.
  Returns:
    True if face is valid.
  */
  bool IsValid( ) const;
};

class ON_CLASS ON_MeshFace
{
public:
  static const ON_MeshFace UnsetMeshFace; // all vi[] values are -1.


  int vi[4]; // vertex index - vi[2]==vi[3] for triangles

  /*
  Returns:
    True if vi[2] == vi[3];
  Remarks:
    Assumes the face is valid.
  */
  bool IsTriangle() const;

  /*
  Returns:
    True if vi[2] != vi[3];
  Remarks:
    Assumes the face is valid.
  */
  bool IsQuad() const;

  /*
  Description:
    Determine if a face is valid by checking that the vertices
    are distinct.
  Parameters:
    mesh_vertex_count - [in]
      number of vertices in the mesh
    V - [in]
      optional array of mesh_vertex_count vertex locations.
  Returns:
    true
      The face is valid.
    false
      The face is not valid. It may be possible to repair the
      face by calling ON_MeshFace::Repair().
  */
  bool IsValid(
    int mesh_vertex_count
    ) const;
  bool IsValid(
    unsigned int mesh_vertex_count
    ) const;
  bool IsValid(
        int mesh_vertex_count,
        const ON_3fPoint* V
        ) const;
  bool IsValid(
        int mesh_vertex_count,
        const ON_3dPoint* V
        ) const;

  /*
  Description:
    Reverses the order of the vertices in v[].
    vi[0] is not changed.
  */
  void Flip();

  /*
  Description:
    If IsValid() returns false, then you can use Repair()
    to attempt to create a valid triangle. 
  Parameters:
    mesh_vertex_count - [in]
      number of vertices in the mesh
    V - [in]
      optional array of mesh_vertex_count vertex locations.
  Returns:
    true
     repair was successful and v[0], v[1], vi[2] have distinct valid
     values and v[2] == v[3].
    false
     this face's vi[] values cannot be repaired    
  */
  bool Repair(
        int mesh_vertex_count
        );
  bool Repair(
        int mesh_vertex_count,
        const ON_3fPoint* V
        );
  bool Repair(
        int mesh_vertex_count,
        const ON_3dPoint* V
        );

  /*
  Description:
    Compute the face normal
  Parameters:
    dV - [in] double precision vertex array for the mesh
    fV - [in] float precision vertex array for the mesh
    FN - [out] face normal
  Returns:
    true if FN is valid.
  */
  bool ComputeFaceNormal( const ON_3dPoint* dV, ON_3dVector& FN ) const;
  bool ComputeFaceNormal( const ON_3fPoint* fV, ON_3dVector& FN ) const;
  bool ComputeFaceNormal( const class ON_3dPointListRef& vertex_list, ON_3dVector& FN ) const;

  /*
  Parameters:
    planar_tolerance - [in] 
      If planar_tolerance >= 0 and
      (maximum plane equation value - minimum plane equation value) > planar_tolerance,
      then false is returned.
    angle_tolerance_radians - [in]
      If angle_tolerance_radians >= 0.0 and the angle between opposite
      corner normals is > angle_tolerance_radians, then false is returned.
      A corner normal is the normal to the triangle formed by two
      adjacent edges and the diagonal connecting their endpoints.
      A quad has four corner normals.
      Passing in ON_PI/2 is a good way will result in false being
      returned for non-convex quads
    face_plane_equation - [out]
      If not null, the equation used to test planarity is returned here.
  Returns:
    True if the face is planar
  */
  bool IsPlanar( 
    double planar_tolerance,
    double angle_tolerance_radians,
    const class ON_3dPointListRef& vertex_list,
    ON_PlaneEquation* face_plane_equation
    ) const;

  /*
  Description:
    Get corner normals.
  Parameters:
    vertex_list - [in]
    corner_normals[4] - [out]
      For a triangle, all values are identical.
      If a corner normal cannot be calculated, ON_3dVector::UnsetVector
      is returned.
  Returns:
    Number of corner normals that are valid.
  */
  unsigned int GetCornerNormals(
    const class ON_3dPointListRef& vertex_list,
    ON_3dVector corner_normals[4]
    ) const;

  bool GetPlaneEquation(
    const class ON_3dPointListRef& vertex_list,
    ON_PlaneEquation& face_plane_equation
    ) const;
};


class ON_CLASS ON_MeshTriangle
{
public:
  static const ON_MeshTriangle UnsetMeshTriangle; // all vi[] values are ON_UNSET_UINT_INDEX.

  unsigned int m_vi[3]; // vertex index list

  /*
  Description:
    Determine if a triangle is valid by checking that the vertices
    are distinct.
  Parameters:
    mesh_vertex_count - [in]
      number of vertices in the mesh
    vertex_list - [in]
      optional list of mesh vertex locations.
  Returns:
    true
      The triangle is valid.
    false
      The triangle is not valid.
  */
  bool IsValid( 
    size_t mesh_vertex_count
    ) const;

  bool IsValid(
    size_t mesh_vertex_count,
    const class ON_3fPoint* vertex_list
    ) const;

  bool IsValid(
    size_t mesh_vertex_count,
    const class ON_3dPoint* vertex_list
    ) const;  

  bool IsValid(
    const class ON_3dPointListRef& vertex_list
    ) const;

  /*
  Description:
    Swaps the values of m_vi[1] and m_vi[2]. m_vi[0] is not changed.
  */
  void Flip();


  /*
  Description:
    Compute the triangle normal
  Parameters:
    dV - [in] double precision vertex array for the mesh
    fV - [in] float precision vertex array for the mesh
    vertex_list - [in] vertex locations
    triangle_normal - [out] triangle normal
  Returns:
    true if triangle_normal is valid.
  */
  bool GetTriangleNormal(
    const class ON_3dPoint* dV, 
    class ON_3dVector& triangle_normal 
    ) const;

  bool GetTriangleNormal(
    const class ON_3fPoint* fV, 
    class ON_3dVector& triangle_normal 
    ) const;

  bool GetTriangleNormal(
    const class ON_3dPointListRef& vertex_list, 
    class ON_3dVector& triangle_normal
    ) const;

 static bool GetTriangleNormal(
   ON_3dPoint point0,
   ON_3dPoint point1,
   ON_3dPoint point2,
   class ON_3dVector& triangle_normal
   );

};

class ON_CLASS ON_MeshFaceList
{
public:
  ON_MeshFaceList()
    : m_bQuadFaces(false)
    , m_face_count(0)
    , m_face_stride(0)
    , m_faces(0)
  {}

  ON_MeshFaceList(
    const ON_Mesh* mesh
    );

  static const ON_MeshFaceList EmptyFaceList;

  unsigned int SetFromTriangleList(
    size_t triangle_count,
    size_t triangle_stride,
    const unsigned int* triangles
    );
  
  unsigned int SetFromQuadList(
    size_t quad_count,
    size_t quad_stride,
    const unsigned int* quads
    );

  unsigned int SetFromMesh(
    const ON_Mesh* mesh
    );

  inline const unsigned int* Fvi(unsigned int face_index) const
  {
    return (face_index < m_face_count) ? (m_faces + (face_index*m_face_stride)) : nullptr;
  }

  inline unsigned int* QuadFvi(unsigned int face_index, unsigned int buffer[4]) const
  {
    if ( face_index < m_face_count )
    {
      const unsigned int* p = m_faces + (face_index*m_face_stride);
      buffer[0] = *p;
      buffer[1] = *(++p);
      buffer[2] = *(++p);
      buffer[3] = m_bQuadFaces ? *(++p) : buffer[2];
    }
    else
    {
      buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0;
    }
    return buffer;
  }


  inline bool IsQuad(unsigned int face_index) const
  {
    if ( m_bQuadFaces && face_index < m_face_count )
    {
      const unsigned int* p = m_faces + (face_index*m_face_stride);
      return p[2] != p[3];
    }
    return false;
  }

  inline unsigned int FaceCount() const
  {
    return m_face_count;
  }

  inline unsigned int FaceVertexCount() const
  {
    return m_bQuadFaces?4:3;
  }

  size_t FaceStride() const
  {
    return m_face_stride;
  }

  /*
  Description:
    Get the minimum and maximum vertex indices referenced by a face in the list.
  Parameters:
  minimum_valid_vertex_index - [in]
    Any face with a vertex index < minimum_valid_vertex_index will be ignored.
  maximum_valid_vertex_index - [in]
    Any face with a vertex index > maximum_valid_vertex_index will be ignored.
  minimum_vertex_index - [out]
  maximum_vertex_index - [out]
    If there are no valid faces, then both output values are 0.
  Returns:
    Number of valid faces.
  */
  unsigned int GetVertexIndexInterval(
    unsigned int minimum_valid_vertex_index,
    unsigned int maximum_valid_vertex_index,
    unsigned int* minimum_vertex_index,
    unsigned int* maximum_vertex_index
    ) const;
  
private:
  bool m_bQuadFaces;
  unsigned int m_face_count;
  unsigned int m_face_stride;
  const unsigned int* m_faces;
};

class ON_CLASS ON_MeshVertexFaceMap
{
public:
  ON_MeshVertexFaceMap() ON_NOEXCEPT;
  ~ON_MeshVertexFaceMap();
  ON_MeshVertexFaceMap(const ON_MeshVertexFaceMap&);
  ON_MeshVertexFaceMap& operator=(const ON_MeshVertexFaceMap&);

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_MeshVertexFaceMap( ON_MeshVertexFaceMap&& ) ON_NOEXCEPT;
  ON_MeshVertexFaceMap& operator=( ON_MeshVertexFaceMap&& ) ON_NOEXCEPT;
#endif

  bool SetFromMesh(
    const ON_Mesh* mesh,
    bool bMapInvalidFaces
    );

  bool SetFromFaceList(
    unsigned int vertex_count,
    const class ON_MeshFaceList& face_list,
    bool bMapInvalidFaces
    );

  void Destroy();

  /*
  Returns:
    Number a vertices.
  */
  unsigned int VertexCount() const;

  /*
  Returns:
    Number a faces.
  */
  unsigned int FaceCount() const;

  /*
  Parameters:
    vertex_index - [in]
      Index of a vertex.
  Returns:
    The number of faces that reference the vertex.
  Remarks:
    If vertex_index is out of range, then zero is returned.
  */
  unsigned int VertexFaceCount(
    unsigned int vertex_index
    ) const;
  
  /*
  Parameters:
    vertex_index - [in]
      Index of a vertex.
  Returns:
    An array of indices of faces that reference the vertex.
    The array has length VertexFaceCount(vertex_index).
    When the value of VertexFaceCount(vertex_index) is zero,
    this function returns a null pointer.
  Remarks:
    If vertex_index is out of range, then null is returned.
  */
  const unsigned int* VertexFaceList(
    unsigned int vertex_index
    ) const;

  /*
  Description:
    Expert user function for situations where rapid access to the
    vertex face list information is required.
  Returns:
    An array of VertexCount() unsigned int arrays that list the
    indices of faces that reference each vertex.
    VertexFaceMap()[vertex_index] is null if zero faces reference the
    indicated vertex. Otherwise, VertexFaceMap()[vertex_index][0] is
    the number of faces that reference the vertex and 
    VertexFaceMap()[vertex_index][1,...,n] are the indices of those faces,
    where "n" is the value of VertexFaceMap()[vertex_index][0].
  */
  const unsigned int *const* VertexFaceMap() const;

private:
  unsigned int m_vertex_count;
  unsigned int m_face_count;
  const unsigned int *const* m_vertex_face_map;
  void* m_p;
  void m_copy(const ON_MeshVertexFaceMap&);
  void* m_alloc(size_t);
};

#if defined(OPENNURBS_PLUS)

/// <summary>
/// Provides a mechanism for lazily evaluating mesh data. The implementation is private and subject to change.
/// </summary>
class ON_CLASS ON_MeshIntersectionCache
{
public:
  ON_MeshIntersectionCache();
  ~ON_MeshIntersectionCache();
  class ON_MeshIntersectionCacheImplementation;
  ON_MeshIntersectionCacheImplementation* m_impl;

  // if you request a boundingbox in the cache, then [meshes] must be exactly the same ones used later or earlier in the intersection code
  ON_BoundingBox GetBoundingBox(const ON_SimpleArray<const ON_Mesh*>& meshes);
private:
  ON_MeshIntersectionCache(const ON_MeshIntersectionCache&) = delete;
  ON_MeshIntersectionCache& operator=(const ON_MeshIntersectionCache&) = delete;
};

/// <summary>
/// Provides a mechanism for lazily evaluating mesh data.
/// </summary>
class ON_CLASS ON_MeshIntersectionOptions
{
public:
  /// <summary>
  /// Defines the desired accuracy of the intersection calculation.
  /// </summary>
  enum class Accuracy
  {
    ForVisualization, // Fastest, lower quality, no overlap detection
    Standard // Default
  };

  ON_MeshIntersectionOptions();
  ~ON_MeshIntersectionOptions();
  class ON_MeshIntersectionOptionsImplementation;
  ON_MeshIntersectionOptionsImplementation* m_impl;

  void SetRequestedAccuracy(Accuracy accuracy);
  Accuracy RequestedAccuracy() const; // Defaults to Accuracy::Standard

  void SetMultithreadingEnabled(bool enabled);
  bool MultithreadingEnabled() const; // Defaults to true

  void SetTextLog(ON_TextLog* log);
  ON_TextLog* TextLog() const; // Defaults to nullptr

  void SetTerminator(ON_Terminator* terminator);
  ON_Terminator* Terminator() const; // Defaults to nullptr

  ON_ProgressReporter* ProgressReporter() const;  // Defaults to nullptr
  void SetProgressReporter(ON_ProgressReporter* reporter);

  ON_MeshIntersectionCache* MeshIntersectionCache() const; // Defaults to nullptr
  void SetMeshIntersectionCache(ON_MeshIntersectionCache* cache); 

  double Tolerance() const; // Defaults to ON_ZERO_TOLERANCE
  void SetTolerance(double tolerance);

private:
  ON_MeshIntersectionOptions(const ON_MeshIntersectionOptions&) = delete;
  ON_MeshIntersectionOptions& operator=(const ON_MeshIntersectionOptions&) = delete;
};

#endif

class ON_CLASS ON_MeshNgonBuffer
{
  // An ON_MeshNgonBuffer provides memory for 
  // creating an ON_MeshNgon that is a triangle or quad.
public:
  ON_MeshNgonBuffer();
  const class ON_MeshNgon* Ngon() const;
  
  const class ON_MeshNgon* CreateFromMeshFaceIndex(
    const class ON_Mesh* mesh,
    unsigned int face_index
    );
  
  const class ON_MeshNgon* CreateFromMeshFace(
    const class ON_MeshFace* mesh_face,
    unsigned int face_index
    );
  
  const class ON_MeshNgon* CreateFromTriangle(
    const unsigned int triangle_vertex_indices[3],
    unsigned int face_index
    );

  const class ON_MeshNgon* CreateFromQuad(
    const unsigned int quad_vertex_indices[4],
    unsigned int face_index
    );

public:
  ON__UINT_PTR m_ngon_buffer[10];
};

class ON_CLASS ON_MeshNgon
{
public:
  // Number of N-gon corners (N >= 3)
  unsigned int m_Vcount; // number of vertices and sides (the "n" in n-gon)
  unsigned int m_Fcount; // number of faces

  // N-gon vertex indices
  // An array of m_Vcount indices into the mesh's m_V[] vertex array.
  // Unset elements have the value ON_UNSET_UINT_INDEX.  If the ngon
  // in managed by an ON_NgonAllocator, then the memory for m_vi[]
  // is also managed by that ON_NgonAllocator.
  unsigned int* m_vi;

  // N-gon face indices
  // An array of m_Fcount indices into the mesh's m_F[] face array.
  // Unset elements have the value ON_UNSET_UINT_INDEX.  If the ngon
  // in managed by an ON_NgonAllocator, then the memory for m_fi[]
  // is also managed by that ON_NgonAllocator.
  unsigned int* m_fi;

  /*
  Returns:
    0:   This n-gon is not managed by an ON_MeshNgonAllocator.
    >=0: The maximum capcity (maximum m_Vcount+m_Fcount) for this N-gon
  */
  unsigned int Capacity() const;

  static int Compare(
    const ON_MeshNgon* A,
    const ON_MeshNgon* B
    );

  /*
  Returns:
    32-bit cyclic redundancy check that can be used as a hash code.
  */
  ON__UINT32 CRC32() const;

  /*
  Returns:
    A SHA-1 has of the vertex and face indices.
  */
  ON_SHA1_Hash ContentHash() const;

  /*
  Parameters:
    mesh_face_list - [in]
      faces referenced by this n-gon.
  Returns:
    Total number of boundary edges, including interior edges.
  */
  unsigned int BoundaryEdgeCount(
    const ON_MeshFaceList& mesh_face_list
  ) const;

  /*
  Parameters:
    mesh - [in]
      mesh referenced by this n-gon.
  Returns:
    Total number of boundary edges, including interior edges.
  */
  unsigned int BoundaryEdgeCount(
    const ON_Mesh* mesh
  ) const;

  /*
  Returns:
    Total number of outer boundary edges.
  */
  unsigned int OuterBoundaryEdgeCount() const;

  /*
  Parameters:
    mesh_face_list - [in]
      ON_Mesh face list.
    bPermitHoles - [in]
      true if the ngon is permitted to have interior holes
      false otherwise.
  Description:
    Determine if the ngon's boundary orientation matches that of the set of faces it is made from.
  Returns:
     1: The ngon does not have holes, the ngon's faces are compatibly oriented, 
        and the ngon's outer boundary orientation matches the faces' orientation.
    -1: The ngon does not have holes, the ngon's faces are compatibly oriented,
        and the ngon's outer boundary orientation is opposite the faces' orientation.
     0: Otherwise. The ngon may be invalid, have holes, the ngon's faces may not be compatibly oriented, 
        the ngons edges may not have a consistent orientation with respect to the faces, or some other issue.
  */
  int Orientation(
    const ON_MeshFaceList& mesh_face_list,
    bool bPermitHoles
  ) const;

  /*
  Parameters:
    mesh_face_list - [in]
      ON_Mesh face list.
    bPermitHoles - [in]
      true if the ngon is permitted to have interior holes
      false otherwise.
  Description:
    Determine if the ngon's boundary orientation matches that of the set of faces it is made from.
  Returns:
     1: The ngon does not have holes, the ngon's faces are compatibly oriented, 
        and the ngon's outer boundary orientation matches the faces' orientation.
    -1: The ngon does not have holes, the ngon's faces are compatibly oriented,
        and the ngon's outer boundary orientation is opposite the faces' orientation.
     0: Otherwise. The ngon may be invalid, have holes, the ngon's faces may not be compatibly oriented, 
        the ngons edges may not have a consistent orientation with respect to the faces, or some other issue.
  */
  int Orientation(
    const ON_Mesh* mesh,
    bool bPermitHoles
  ) const;

  /*
  Description:
    Reverse the order of the m_vi[] array.
  */
  void ReverseOuterBoundary();

  /*
  Description:
    Use the ngon m_vi[] array to get a list of 3d points from
    mesh_vertex_list.
  Parameters:
    mesh_vertex_list - [in]
    bAppendStartPoint - [in]
      If true, the initial point in the boundary will be added
      as the first point of ngon_boundary_points[] and then
      added again as the last point of ngon_boundary_points[].
      This is useful when you need a closed polyline.
    ngon_boundary_points - [out]
      An array of ngon->m_Vcount + (bAppendStartPoint ? 1 : 0)
      points.
  Returns:
    Number of points added to ngon_boundary_points[] or 0 if invalid
    input is encountered.
  */
  unsigned int GetOuterBoundaryPoints(
    const class ON_3dPointListRef& mesh_vertex_list,
    bool bAppendStartPoint,
    ON_SimpleArray<ON_3dPoint>& ngon_boundary_points
    ) const;

  /*
  Description:
    Use the ngon m_vi[] array to get a list of 3d points from
    mesh_vertex_list.
  Parameters:
    mesh_vertex_list - [in]
    bAppendStartPoint - [in]
      If true, the initial point in the boundary will be added
      as the first point of ngon_boundary_points[] and then
      added again as the last point of ngon_boundary_points[].
      This is useful when you need a closed polyline.
    ngon_boundary_points - [out]
      An array of ngon->m_Vcount + (bAppendStartPoint ? 1 : 0) points
      is returned in ngon_boundary_points[].  The caller must insure
      that ngon_boundary_points[] has room for this many elements.
  Returns:
    Number of points added to ngon_boundary_points[] or 0 if invalid
    input is encountered.
  */
  unsigned int GetOuterBoundaryPoints(
    const class ON_3dPointListRef& mesh_vertex_list,
    bool bAppendStartPoint,
    ON_3dPoint* ngon_boundary_points
    ) const;


  /*
  Description:
    Use the ngon m_fi[] array to get a list of ngon boundary sides.
  Parameters:
    mesh_face_list - [in]
    ngon_boundary_sides - [out]
      ngon_boundary_sides[i]/8 = ON_MeshNon.m_fi[] array index
      ngon_boundary_sides[i]%4 = side index
        side index 0 identifies the side that runs from the first face
        vertex to the second face vertex.
      ngon_boundary_sides[i]&4 != 0 means the face side is reversed
        when used as an ngon boundary segment.
  Returns:
    Number of elements added to ngon_boundary_sides[] or 0 if invalid
    input is encountered.
  */
  unsigned int GetBoundarySides(
    const class ON_MeshFaceList& mesh_face_list,
    ON_SimpleArray<unsigned int>& ngon_boundary_sides
    ) const;

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Trianglulate an ngon.
  Parameters:
    point_index_count - [in]
    point_index_stride - [in]
    point_index_list - [in]
      point_index_list[] is a list of indices into point_list[].
      If point_index_count >= 3, 0 == point_index_stride,
      and null = point_index_list, then 3d points 
      (point_list[0], point_list[1], ... point_list[point_index_count-1])
      are used. Otherwise, for "i" from 0 to point_index_count-1, the 3d points 
      point_list[point_index_list[i*point_index_stride] are used.
    point_list - [in]
      3d point locations.
    projection_plane - [in]
      When in doubt, pass ON_Plane::UnsetPlane and a proper orientation
      of the plane 
          ON_Plane::FromPointList(
              point_index_count,
              point_index_stride,
              point_index_list,
              point_list)
      will be used.
      This plane is used in the triangluation calculation to convert
      the 3d point list into a 2d point list.
    planar_tolerance - [in]
      When in doubt, pass ON_UNSET_VALUE.
      This tolerance is used to determine when the boundary points should
      be considered coplanar enough for convex polygon special case
      triangulation.
    triangle_index_type - [in]
      0: The returned ON_MeshTriangle.m_vi[] values are in the 
         in the range from 0 to point_index_count-1 and index
         the point_index_list[] array.
      1: The returned ON_MeshTriangle.m_vi[] values are in the 
         in the range from 0 to point_list.PointCount()-1 and
         index the point_list[] array.
    triangle_list - [out]
      Triangles are appended to this list
    ngon_plane - [out]
      If ngon_plane is not null, then the plane use to convert
      the 3d points into 2d points is returned here.  If you pass
      in a projection_plane, the returned ngon_plane will be
      flipped if the outer boundary was oriented clockwise
      with respect to the input plane.
    points2d_list - [out]
      If points2d_list is not null, then the 2d points used to
      calculate the triangulation are returned here. points2d_list[i]
      corresponds to the i-th boundary point.
      This information useful if you want to assign texture
      coordinates.
  Returns:
    Number of triangles appended to triangle_list[].
  */
  static unsigned int TriangulateNgon(
    size_t point_index_count,
    size_t point_index_stride,
    const unsigned int* point_index_list,
    const class ON_3dPointListRef& point_list,
    ON_Plane projection_plane,
    double planar_tolerance,
    unsigned int triangle_index_type,
    ON_SimpleArray< ON_MeshTriangle >& triangle_list,
    ON_Plane* ngon_plane,
    ON_SimpleArray<ON_2dPoint>* points2d_list
    );

  /*
  Description:
    Trianglulate an ngon.
  Parameters:
    boundary_vertex_count - [in]
    boundary_vertex_list - [in]
      boundary_vertex_list[] should be an array of boundary_vertex_count points
      that forms a simple closed polygon.
    triangle_list - [out]
      Triangles are appended to this list
  Returns:
    Number of triangles appended to triangle_list[].
  */
  static unsigned int TriangulateNgon(
    size_t boundary_vertex_count,
    const class ON_3dPoint* boundary_vertex_list,
    ON_SimpleArray< ON_MeshTriangle >& triangle_list
    );
#endif

  //////////////////////////////////////////////////////////////
  //
  // Tools for finding a making n-gons
  //
  static unsigned int FindPlanarNgons(
    const class ON_3dPointListRef& vertex_list,
    const class ON_MeshFaceList& face_list,
    const unsigned int *const* vertex_face_map,
    double planar_tolerance,
    unsigned int minimum_ngon_vertex_count,
    unsigned int minimum_ngon_face_count,
    bool bAllowHoles,
    class ON_MeshNgonAllocator& NgonAllocator,
    ON_SimpleArray<unsigned int>& NgonMap,
    ON_SimpleArray<ON_MeshNgon*>& Ngons
    );

  /*
  Description:
    Get a list of vertices that form the boundary of a set of faces.
  Parameters:
    mesh_vertex_list - [in]
    mesh_face_list - [in]
    vertex_face_map - [in]
      null or a vertex map made from the information in
      mesh_vertex_list and mesh_face_list.
    ngon_fi_count - [in]
      length of ngon_fi[] array
    ngon_fi - [in]
      An array of length ngon_fi_count that contains the indices 
      of the faces that form the ngon.
    ngon_vi - [out]
      An array of vertex indices that make the ngon boundary.        
  Returns:
    Number of vertices in the ngon outer boundary or 0 if the input is
    not valid.
  */
  static unsigned int FindNgonOuterBoundary(
    const class ON_3dPointListRef& mesh_vertex_list,
    const class ON_MeshFaceList& mesh_face_list,
    const unsigned int *const* vertex_face_map,
    size_t ngon_fi_count,
    const unsigned int* ngon_fi,
    ON_SimpleArray<unsigned int>& ngon_vi
    );

    /*
  Description:
    Get a list of vertices that form the boundary of a set of faces.
  Parameters:
    mesh_vertex_list - [in]
    mesh_face_list - [in]
    vertex_face_map - [in]
      null or a vertex map made from the information in
      mesh_vertex_list and mesh_face_list.
    ngon_fi_count - [in]
      length of ngon_fi[] array
    ngon_fi - [in]
      An array of length ngon_fi_count that contains the indices 
      of the faces that form the ngon.
    ngon_vi - [out]
      An array of vertex indices that make the ngon boundary.        
  Returns:
    Number of vertices in the ngon outer boundary or 0 if the input is
    not valid.
  */
  static unsigned int FindNgonOuterBoundary(
    const class ON_3dPointListRef& mesh_vertex_list,
    const class ON_MeshFaceList& mesh_face_list,
    ON_MeshVertexFaceMap* vertex_face_map,
    size_t ngon_fi_count,
    const unsigned int* ngon_fi,
    ON_SimpleArray<unsigned int>& ngon_vi
    );

/*
Description:
  Get a list of vertices that form any boundary of a set of faces.
  This includes inner boundaries.
Parameters:
  mesh_vertex_list - [in]
  mesh_face_list - [in]
  vertex_face_map - [in]
    null or a vertex map made from the information in
    mesh_vertex_list and mesh_face_list.
  ngon_fi_count - [in]
    length of ngon_fi[] array
  ngon_fi - [in]
    An array of length ngon_fi_count that contains the indices
    of the faces that form the ngon.
  ngon_vi - [out]
    An array of vertex indices that make the ngon boundary.
  ngon_boundary_markers - [out] indexes into ngon_boundary_points to differentiate (inner) boundaries,
    if empty there is only an outer boundary.
Returns:
  Number of vertices in the ngon outer boundary or 0 if the input is
  not valid.
*/
  static unsigned int FindNgonBoundaries(
    const class ON_3dPointListRef& mesh_vertex_list,
    const class ON_MeshFaceList& mesh_face_list,
    ON_MeshVertexFaceMap* vertex_face_map,
    size_t ngon_fi_count,
    const unsigned int* ngon_fi,
    ON_SimpleArray<unsigned int>& ngon_vi, 
    ON_SimpleArray<unsigned int>& ngon_vi_markers
  );

/*
Description:
  Get a list of vertices that form any boundary of a set of faces.
  This includes inner boundaries.
Parameters:
  mesh_vertex_list - [in]
  mesh_face_list - [in]
  vertex_face_map - [in]
    null or a vertex map made from the information in
    mesh_vertex_list and mesh_face_list.
  ngon_fi_count - [in]
    length of ngon_fi[] array
  ngon_fi - [in]
    An array of length ngon_fi_count that contains the indices
    of the faces that form the ngon.
  ngon_vi - [out]
    An array of vertex indices that make the ngon boundary.
Returns:
  Number of vertices in the ngon outer boundary or 0 if the input is
  not valid.
*/
  static unsigned int FindNgonBoundary(
    const class ON_3dPointListRef& mesh_vertex_list,
    const class ON_MeshFaceList& mesh_face_list,
    const unsigned int *const* vertex_face_map,
    size_t ngon_fi_count,
    const unsigned int* ngon_fi,
    ON_SimpleArray<unsigned int>& ngon_vi
  );

    /*
  Description:
    Get a list of vertices that form any boundary of a set of faces.
    This includes inner boundaries.
  Parameters:
    mesh_vertex_list - [in]
    mesh_face_list - [in]
    vertex_face_map - [in]
      null or a vertex map made from the information in
      mesh_vertex_list and mesh_face_list.
    ngon_fi_count - [in]
      length of ngon_fi[] array
    ngon_fi - [in]
      An array of length ngon_fi_count that contains the indices
      of the faces that form the ngon.
    ngon_vi - [out]
      An array of vertex indices that make the ngon boundary.
  Returns:
    Number of vertices in the ngon outer boundary or 0 if the input is
    not valid.
  */
  static unsigned int FindNgonBoundary(
    const class ON_3dPointListRef& mesh_vertex_list,
    const class ON_MeshFaceList& mesh_face_list,
    ON_MeshVertexFaceMap* vertex_face_map,
    size_t ngon_fi_count,
    const unsigned int* ngon_fi,
    ON_SimpleArray<unsigned int>& ngon_vi
  );

  /*
  Description:
    Create an ngon pointer that contains a triangle (3-gon) 
    or quad (4-gon) from a mesh face.  
    This is handy when your code needs to handle both
    ngons and faces because it lets you convert a face to its
    ngon format and the rest of the code can work exclusively with
    ngons.
  Parameters:
    buffer - [in]
      an array with a capacity for at least 9 ON__UINT_PTR elements.
      The returned ngon information will be stored in this memory.
    mesh_face_index - [in]
    fvi - [in]
      mesh face vertex indices.
      If "f" is an ON_MeshFace, then pass (const unsigned int*)f.vi.
  Returns:
    If the input is valid, the returned ngon pointer is is the 
    face's triangle or quad.  All returned information is in the
    buffer[].
    null - invalid input.
  See Also:
    ON_Mesh::NgonFromComponentIndex()
  */
  static class ON_MeshNgon* NgonFromMeshFace(
    class ON_MeshNgonBuffer& ngon_buffer,
    unsigned int mesh_face_index,
    const unsigned int* fvi
    );

  /*
  Description:
    Create an array of a single ngon pointer that contains
    a triangle (3-gon) or quad (4-gon) from a mesh face.  
    This is handy when your code needs to handle both
    ngons and faces because it lets you convert a face to its
    ngon format and the rest of the code can work exclusively with
    ngons.
  Parameters:
    ngon_buffer - [in]
      memory used to create ngon class array with a capacity for at least 10 ON__UINT_PTR elements.
    mesh_face_index - [in]
    fvi - [in]
      mesh face vertex indices.
      If "f" is an ON_MeshFace, then pass (const unsigned int*)f.vi.
  Returns:
    If the input is valid, the returned pointer is an array of a single ngon 
    that is the face's triangle or quad.  All returned information is in the
    buffer[].
    null - invalid input.
  */
  static class ON_MeshNgon** NgonListFromMeshFace(
    class ON_MeshNgonBuffer& ngon_buffer,
    unsigned int mesh_face_index,
    const unsigned int* fvi
    );

  /*
  Description:
    If a component index identifies a face or ngon, Get an array Create an array of a single ngon pointer that contains
    a triangle (3-gon) or quad (4-gon) from a mesh face.  
    This is handy when your code needs to handle both
    ngons and faces because it lets you convert a face to its
    ngon format and the rest of the code can work exclusively with
    ngons.
  Parameters:
    ngon_buffer - [in]
      memory used to create ON_MeshNgon class
    ci - [in]
    mesh - [in]
    ngon_list - [out]
      An array of ngon pointers.  Some pointers may be null.
  Returns:
    Number of ngon pointers in ngon_list.
  */
  static  unsigned int NgonListFromMeshFaceOrNgonComponent(
    class ON_MeshNgonBuffer& ngon_buffer,
    ON_COMPONENT_INDEX ci,
    const class ON_Mesh* mesh,
    const class ON_MeshNgon* const *& ngon_list
    );

  ////////////////////////////////////////////////////////////////////
  //
  // Tools for text output
  //
  ON_String ToString() const;

  ON_wString ToWideString() const;

  void Dump( 
    class ON_TextLog& text_log 
    )const;

  void AppendToString( 
    class ON_String& s
    )const;

  void AppendToString( 
    class ON_wString& s
    )const;


  ////////////////////////////////////////////////////////////////////
  //
  // Tools for validation output
  //

  /*
  Description:
    Test ngon to see if the vertex and face references are valid and
    pass partial boundary validity checks,
  Parameters:
    ngon - [in]
      ngon to test
    ngon_index - [in]
      This index is used in messages sent to text_log
    text_log - [in]
      nullptr or a place to send information about problems.
    mesh_vertex_count - [in]
      Number of vertices in the mesh
    mesh_face_count - [in]
      Number of face in the mesh
    mesh_F - [in]
      nullptr of mesh faces - required for boundary checks
    workspace_buffer - [in]
      If you are passing in mesh_F and you are testing testing multiple 
      ngons, then consider providing a workspace_buffer that will be automatically
      reused for successive ngons.
  Returns:
    0: ngon is not valid
    >0: number of boundary edges.  
        If this number is > ngon->m_V_count, then the ngon has inner boundaries
        or duplicate vertices.       
  */
  static unsigned int IsValid(
    const ON_MeshNgon* ngon,
    unsigned int ngon_index,
    ON_TextLog* text_log,
    unsigned int mesh_vertex_count,
    unsigned int mesh_face_count,
    const ON_MeshFace* mesh_F
    );

  static unsigned int IsValid(
    const ON_MeshNgon* ngon,
    unsigned int ngon_index,
    ON_TextLog* text_log,
    unsigned int mesh_vertex_count,
    unsigned int mesh_face_count,
    const ON_MeshFace* mesh_F,
    ON_SimpleArray< unsigned int >& workspace_buffer
    );
};

class ON_CLASS ON_MeshNgonAllocator
{
public:
  ON_MeshNgonAllocator() ON_NOEXCEPT;
  ~ON_MeshNgonAllocator();

  /*
  Parameters:
    Vcount - [in] >= 3
    Fcount - [in] >= 0
  */
  ON_MeshNgon* AllocateNgon(
    unsigned int Vcount,
    unsigned int Fcount
    );

  /*
  Parameters:
    Vcount - [in] >= 3
    Fcount - [in] >= 0
  */
  ON_MeshNgon* ReallocateNgon(
    ON_MeshNgon* ngon,
    unsigned int Vcount,
    unsigned int Fcount
    );

  /*
  Parameters:
    ngon - in]
      An ngon pointer value that was previously returned by
      this allocator's AllocateNgon() or CopyNgon() function.
  */
  bool DeallocateNgon(
    ON_MeshNgon* ngon
    );

  /*
  Description:
    Returns a copy of ngon.
  Parameters:
    ngon - [in]
  Returns:
    If 
  */
  ON_MeshNgon* CopyNgon(
    const ON_MeshNgon* ngon
    );

  /*
  Description:
    Deallocate every n-gon managed by this allocator.
  */
  void DeallocateAllNgons();

#if defined(ON_HAS_RVALUEREF)
  ON_MeshNgonAllocator(ON_MeshNgonAllocator&&);
  ON_MeshNgonAllocator& operator=(ON_MeshNgonAllocator&&);
#endif

private:

  ON_FixedSizePool m_7;  // Vcount+Fcount <= 7
  ON_FixedSizePool m_15; // Vcount+Fcount <= 15
  void* m_31; // available for Vcount+Fcount <= 31
  void* m_63; // available for Vcount+Fcount <= 63
  void* m_active;   // active Vcount+Fcount >= 16

private:
  // prohibit copy construction. No implementation.
  ON_MeshNgonAllocator(const ON_MeshNgonAllocator&) = delete;

  // prohibit operator=. No implementation.
  ON_MeshNgonAllocator& operator=(const ON_MeshNgonAllocator&) = delete;
};

class ON_MeshFaceSide
{
public:
  unsigned int   m_vi[2]; // vertex indices or ids (equal values indicate unset)
  unsigned int   m_fi;    // face index or id
  unsigned char  m_side;  // triangles use 0,1,3, quads use 0,1,2,3
                          // m_side 0 connect face vertex 0 to face vertex 1.
  unsigned char  m_dir;   // 0 = counterclockwise, 1 = clockwise (reversed)
  unsigned short m_value; // Use depends on context.
  unsigned int   m_id;    // Use depends on context - typically identifies and edge or ngon

  static const ON_MeshFaceSide Unset; // all values are zero

  /*
  Description:
    Compare a and b in dictionary order comparing the field values in the order
      m_fi
      m_vi[0]
      m_vi[1]
      m_side
      m_dir
  Parameters:
    a - [in]
    b - [in]
  Returns:
    -1: *a < *b
     0: *a < *b
     1: *a > *b
  Remarks:
    The function is thread safe.
  */
  static int CompareFaceIndex(
    const ON_MeshFaceSide* a,
    const ON_MeshFaceSide* b
    );

  /*
  Description:
    Compare a and b in dictionary order comparing the field values in the order
      m_vi[0]
      m_vi[1]
      m_fi
      m_side
      m_dir
  Parameters:
    a - [in]
    b - [in]
  Returns:
    -1: *a < *b
     0: *a < *b
     1: *a > *b
  Remarks:
    The function is thread safe.
  */
  static int CompareVertexIndex(
    const ON_MeshFaceSide* a,
    const ON_MeshFaceSide* b
    );

  /*
  Description:
    Sort the face_sides[] using the compare function 
    ON_MeshFaceSide::CompareVertexIndex().
  Parameters:
    face_sides - [in/out]
      array to sort
    face_sides_count - [in]
      number of elements in the face_sides[] array.
  Remarks:
    The function is thread safe.
  */
  static void SortByVertexIndex(
    ON_MeshFaceSide* face_sides,
    size_t face_sides_count
    );

  /*
  Description:
    Sort the face_sides[] using the compare function 
    ON_MeshFaceSide::CompareFaceIndex().
  Parameters:
    face_sides - [in/out]
      array to sort
    face_sides_count - [in]
      number of elements in the face_sides[] array.
  Remarks:
    The function is thread safe.
  */
  static void SortByFaceIndex(
    ON_MeshFaceSide* face_sides,
    size_t face_sides_count
    );

  /*
  Description:
    Get a list of mesh face sides.
  Parameters:
    mesh_vertex_count - [in]
      Number of vertices in the mesh.
      This value is used to validate vertex index values in mesh_face_list.
    mesh_face_list - [in]
      Mesh faces
    fi_list - [in]
      - If fi_list null, then sides for every face in mesh_face_list will 
        be added and the ON_MeshFaceSide.m_fi values will be the 
        mesh_face_list[] index.
      - If fi_list is not null, then fi_list[] is an array of mesh_face_list[]
        indices and the ON_MeshFaceSide.m_fi values will be fi_list[] array 
        indices. For example, you may pass ON_MeshNon.m_fi as this parameter
        when you want a list of sides of faces in an ngon.
    fi_list_count - [in]
      - If fi_list is not null, then fi_list_count is the number of elements
        in the fi_list[] array.
      - If fi_list is null, then fi_list_count is ignored.
    vertex_id_map - [in] (can be null)
      - If vertex_id_map is null, then the ON_MeshFaceSide::m_vi[] values
        are the vertex index values from mesh_face_list[].
      - If vertex_id_map is not null, then vertex_id_map[] is an array
        with the mesh_vertex_count elements and ON_MeshFaceSide::m_vi[] values
        are vertex_id_map[mesh_face_list[] vertex indices]. A vertex_id_map[]
        is commonly used when coincident vertices need to be treated as
        a single topological entity.
    face_side_list - [out]
      - If the input value of face_side_list is not null, then face_side_list[] 
        must be long enough to hold the returned face_side_list list.  
        The maximum possible length is 4*mesh_face_list.FaceCount().
      - If the input falue of face_side_list is null, memory will be allocated
        using onmalloc() and the caller is responsible for calling onfree() at
        an appropriate time.
      The returned is face_side_list[] is dictionary sorted by ON_MeshFaceSide.m_fi
      and then ON_MeshFaceSide.m_si. The vertex ids satisfy
      ON_MeshFaceSide.m_vi[0] < ON_MeshFaceSide.m_vi[1].  ON_MeshFaceSide.m_dir
      is 0 if the face vertex order is the same and 1 if the face vertex order
      is opposite. The static sorting functions on ON_MeshFaceSide can be used
      to change this ordering.
  Returns:
    Number of elements set in face_side_list[].
  Remarks:
    Faces in mesh_face_list with vertex indices that are >= mesh_vertex_count
    are ignored.  Degenerate faces are processed, but degenerate sides 
    (equal vertex ids) are not added to face_side_list[].
  */
  static unsigned int GetFaceSideList(
    size_t mesh_vertex_count,
    const class ON_MeshFaceList& mesh_face_list,
    const unsigned int* fi_list,
    size_t fi_list_count,
    const unsigned int* vertex_id_map,
    ON_MeshFaceSide*& face_side_list
    );
};


struct ON_MeshPart
{
  // ON_Mesh faces with indices fi[0] <= i < fi[1] reference
  // vertices with indices vi[0] <= j < vi[1].
  int vi[2]; // subinterval of mesh m_V[] array
  int fi[2]; // subinterval of mesh m_F[] array
  int vertex_count;   // = vi[1] - vi[0];
  int triangle_count; // tris + 2*quads >= fi[1] - fi[0]
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MeshFace>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MeshNgon*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MeshTopologyVertex>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MeshTopologyEdge>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MeshTopologyFace>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<struct ON_MeshPart>;
#endif

class ON_CLASS ON_MeshTopology
{
  // A mesh topology class is always associated with an ON_Mesh
  // and can be retrieved by calling ON_Mesh::Topology()
public:
  ON_MeshTopology();
  ~ON_MeshTopology();

  bool IsValid() const;

  void Dump( ON_TextLog& ) const;

  //////////
  // The parent ON_Mesh geometry used to compute this mesh topology.
  const ON_Mesh* m_mesh;

  //////////
  // number of topological vertices (<= m_mesh.VertexCount())
  int TopVertexCount() const;

  //////////
  // number of topological edges
  int TopEdgeCount() const;
  
  //////////
  // number of topological faces (same as m_mesh.FaceCount())
  int TopFaceCount() const;

  class ON_MeshComponentRef MeshComponentRef(
    ON_COMPONENT_INDEX ci
    ) const;

  /*
  Parameters:
    ci - [in]
      A component index with type of 
      ON_COMPONENT_INDEX::TYPE::mesh_vertex or 
      ON_COMPONENT_INDEX::TYPE::meshtop_vertex.
  Return:
    If ci correctly identifies a mesh topology vertex, then
    component index with type of ON_COMPONENT_INDEX::TYPE::meshtop_vertex is returned.
    Otherwise ON_COMPONENT_INDEX::UnsetComponentIndex is returned.
  */
  const ON_COMPONENT_INDEX TopVertexComponentIndex(
    ON_COMPONENT_INDEX ci
  ) const;

  /*
  Description:
    Get the 3d point location of a vertex.
  Parameters:
    topv_index - [in];
  Returns:
    Location of vertex.
  */
  ON_3dPoint TopVertexPoint(
    int topv_index
    ) const;

  /*
  Description:
    Get the 3d line along an edge.
  Parameters:
    tope_index - [in];
  Returns:
    Line along edge.  If input is not valid,
    the line.from and to are ON_3dPoint::UnsetPoint
  */
  ON_Line TopEdgeLine(
    int tope_index
    ) const;

  ////////
  // returns index of edge that connects topological vertices
  // returns -1 if no edge is found.
  int TopEdge(
    int vtopi0,
    int vtopi1 // ON_MeshTopology vertex topology indices
    ) const;

  ////////
  // returns ON_MeshTopology vertex topology index of a face
  // corner.  The face is triangle iv TopFaceVertex(2) = TopFaceVertex(3)
  bool GetTopFaceVertices(
    int topfi,    // ON_MeshTopology face topology index (= ON_Mesh face index)
    int topvi[4]  // ON_MeshTopology vertex indices returned here
    ) const;

  /*
  Parameters:
    topvi - [in]
      Topology vertex index
    mesh_facedex_to_ngondex_map - [in]
      If null, Mesh().NgonMap() will be used.  In cases where Mesh().NgonMap()
      does not exist and cannot be created, an expert user may pass in
      a local map that converts a face index into an ngon index.
  Returns:
    If the vertex is interior to a single ngon, then the index of the
    ngon is returned.  Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int NgonIndexFromTopologyVertexIndex(
    unsigned int topvi,
    const unsigned int* mesh_facedex_to_ngondex_map
    ) const;

  /*
  Parameters:
    topei - [in]
      Topology edge index
    mesh_facedex_to_ngondex_map - [in]
      If null, Mesh().NgonMap() will be used.  In cases where Mesh().NgonMap()
      does not exist and cannot be created, an expert user may pass in
      a local map that converts a face index into an ngon index.
  Returns:
    If the vertex is interior to a single ngon, then the index of the
    ngon is returned.  Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int NgonIndexFromTopologyEdgeIndex(
    unsigned int topei,
    const unsigned int* mesh_facedex_to_ngondex_map
    ) const;

  /*
  Description:
    Sort the m_topei[] list of a mesh topology vertex so that
    the edges are in radial order.  The "const" is a white
    lie to make this function easier to call.
  Parameter:
    topvi - [in] index of vertex in m_topv[] array.
  Remarks:
    A nonmanifold edge is treated as a boundary edge with respect
    to sorting.  If any boundary or nonmanifold edges end at the
    vertex, then the first edge will be a boundary or nonmanifold
    edge.
  */
  bool SortVertexEdges( int topvi ) const;

  /*
  Description:
    Sort the m_topei[] list of every mesh topology vertex so 
    that the edges are in radial order.  The "const" is a white
    lie to make this function easier to call.
  Remarks:
    Same as
    for ( int topvi = 0; topvi < m_topv.Count(); topvi++ )
      SortVertexEdges(topvi);
  */
  bool SortVertexEdges() const;

  /*
  Description:
    Returns true if the topological vertex is hidden. 
  Parameters:
    topvi - [in] mesh topology vertex index.
  Returns:
    True if mesh topology vertex is hidden.
  Remarks:
    The mesh topology vertex is hidden if and only if
    all the ON_Mesh vertices it represents is hidden.
  */
  bool TopVertexIsHidden( int topvi ) const;

  /*
  Description:
    Returns true if the topological edge is hidden. 
  Parameters:
    topei - [in] mesh topology edge index.
  Returns:
    True if mesh topology edge is hidden.
  Remarks:
    The mesh topology edge is hidden if and only if
    either of its mesh topology vertices is hidden.
  */
  bool TopEdgeIsHidden( int topei ) const;

  /*
  Description:
    Returns true if the topological face is hidden. 
  Parameters:
    topfi - [in] mesh topology face index.
  Returns:
    True if mesh topology face is hidden.
  Remarks:
    The mesh topology face is hidden if and only if
    any of its mesh topology edges are hidden.
  */
  bool TopFaceIsHidden( int topfi ) const;

  /*
  Parameters:
    topei - [in]
      m_tope[] index
  Returns:
    true if the edge 
    has 2 distinct vertices,
    2 or more attached faces,
    and all attached faces reference the same ON_Mesh vertices along this edge.
  */
  bool IsWeldedEdge( int topei ) const;

  //////////
  // m_topv_map[] has length m_mesh.VertexCount() and 
  // m_topv[m_topv_map[vi]] is the topological mesh vertex that is associated
  // the with the mesh vertex m_mesh.m_V[vi].
  ON_SimpleArray<int> m_topv_map;

  ////////////
  // Array of topological mesh vertices.  See the comments in the definition
  // of ON_MeshTopologyVertex for details.
  ON_SimpleArray<ON_MeshTopologyVertex> m_topv;

  ////////////
  // Array of topological mesh edges.  See the comments in the definition
  // of ON_MeshTopologyEdge for details.
  ON_SimpleArray<ON_MeshTopologyEdge> m_tope;

  ////////////
  // Array of topological mesh faces.  The topological face
  // m_topf[fi] corresponds to the mesh face ON_Mesh.m_F[fi].
  // See the comments in the definition of ON_MeshTopologyFace
  // for details. To get the indices of the mesh topology 
  // vertices at the face corners use 
  // topvi = m_topv_map[m_mesh.m_F[fi].vi[n]]
  ON_SimpleArray<ON_MeshTopologyFace> m_topf;

  /*
  Description:
    Expert user function for efficiently getting the
    integer arrays used by the ON_MeshTopologyVertex
    and ON_MeshTopologyEdge classes.
  Parameters:
    count - [in] number of integers in array
  Returns:
    pointer to integer array.  The array memory
    will be freed by ~ON_MeshTopology()
  */
  int* GetIntArray(int count);

private:
  friend class ON_Mesh;

  bool Create();
  void Destroy();
  void EmergencyDestroy();

  // efficient workspaces for
  struct memchunk
  {
    struct memchunk* next;
  } *m_memchunk;

  // NOTE: this field is a bool with valid values of 0 and 1.
  volatile int m_b32IsValid; // sizeof(m_bIsValid) must be 4 - it is used in sleep locks.
                    //    0: Not Valid
                    //    1: Valid
                    //   -1: Sleep locked - ON_Mesh::Topology() calculation is in progress
  int WaitUntilReady(int sleep_value) const; // waits until m_b32IsValid >= 0

private:
  // no implementation
  ON_MeshTopology(const ON_MeshTopology&);
  ON_MeshTopology& operator=(const ON_MeshTopology&);
};



class ON_CLASS ON_MeshPartition
{
public:
  ON_MeshPartition();
  ~ON_MeshPartition();

  // maximum number of vertices in a partition
  int m_partition_max_vertex_count;
  // maximum number of triangles in a partition (quads count as 2 triangles)
  int m_partition_max_triangle_count;

  // Partition i uses 
  // vertices m_V[j] where 
  //
  //   m_part[i].vi[0] <= j < m_part[i].vi[1] 
  //
  // and uses faces m_F[k] where
  //
  //    m_part[i].fi[0] <= k < m_part[i].fi[1]
  ON_SimpleArray<struct ON_MeshPart> m_part;
};

class ON_CLASS ON_MappingTag
{
public:
  ON_MappingTag();
  ON_MappingTag(const ON_TextureMapping& mapping,const ON_Xform* xform);

  static const ON_MappingTag Unset;

  /// <summary>
  /// id = ON_TextureMapping::SurfaceParameterTextureMappingId
  /// type = ON_TextureMapping::TYPE::srfp_mapping
  /// </summary>
  static const ON_MappingTag SurfaceParameterMapping;

  void Default();
  bool Write(ON_BinaryArchive&) const;
  bool Read(ON_BinaryArchive&);
  void Dump( ON_TextLog& ) const;
  void Transform( const ON_Xform& xform );
  void Set(const ON_TextureMapping& mapping);

  /*
  Description:
    Sets the tag to the value the meshes have that
    come out of ON_Brep::CreateMesh().
  */
  void SetDefaultSurfaceParameterMappingTag();

  int Compare( const ON_MappingTag& other,
               bool bCompareId = true,
               bool bCompareCRC = true,
               bool bCompareXform = true
               ) const;

  static int CompareAll(const ON_MappingTag& lhs, const ON_MappingTag& rhs);
  static int CompareAllFromPointer(const ON_MappingTag* lhs, const ON_MappingTag* rhs);

  /*
  Returns:
    True if the mapping tag is set.
  */
  bool IsSet() const;

  /*
  Returns:
    True if the mapping tag is for a mapping with
    type ON_TextureMapping::srfp_mapping with
    m_uvw = identity.
  */
  bool IsDefaultSurfaceParameterMapping() const;

  // m_mapping_id identifies the mapping used to create
  // the texture coordinates and/or false colors.
  ON_UUID  m_mapping_id = ON_nil_uuid;
  
  ON_TextureMapping::TYPE m_mapping_type = ON_TextureMapping::TYPE::no_mapping;

  // The m_mapping_crc is a CRC of a SHA1 hash of the parameters used in 
  // the calculation to set the current texture coordinates and/or vertex colors. 
  // This CRC is used to detect when the the texture coordinates and/or false colors need to be updated.
  // (Saving the SHA1 hash itself would be better, but changing m_mapping_crc to a SHA1 hash would break the SDK.)
  // 
  // When m_mapping_id = ON_nil_uuid and m_mapping_type = ON_TextureMapping::TYPE::no_mapping, 
  // m_mapping_crc has no meaning and is ignored by operator== and operator!=.
  // 
  // When m_mapping_id = ON_MappingTag::SurfaceParameterMapping.m_mapping_id and m_mapping_type = ON_TextureMapping::TYPE::srfp_mapping, 
  // m_mapping_crc can be set from a uvw texture coordinate transformation.
  ON__UINT32 m_mapping_crc = 0;

  // It and records transformations applied 
  // to the mesh after the texture coordinates were
  // calculated.  If the texture mapping does not
  // change when the mesh is transformed, then set 
  // m_mesh_xform to zero so that compares will work right.
  // 
  // When m_mapping_id = ON_nil_uuid and m_mapping_type = ON_TextureMapping::TYPE::no_mapping, 
  // m_mesh_xform has no meaning and is ignored by operator== and operator!=.
  // 
  // When m_mapping_id = ON_MappingTag::SurfaceParameterMapping.m_mapping_id and m_mapping_type = ON_TextureMapping::TYPE::srfp_mapping, 
  // m_mesh_xform has no meaning and is ignored by operator== and operator!=.
  ON_Xform m_mesh_xform = ON_Xform::IdentityTransformation;

  /*
  Returns:
    World space transformation to apply to the object when using this mapping.
  */
  const ON_Xform Transform() const;

  /*
  Returns:
    True if Transform() will return the identity transformation.
  */
  bool TransformIsIdentity() const;


  /*
  Returns:
    True if ON_MappingTag will consider xform to be the identity transformation.
  */
  static bool TransformTreatedIsIdentity(const ON_Xform* xform);

  /*
  Returns:
    A sha1 hash of the m_mapping_id, m_mapping_type, m_mapping_crc and m_mesh_xform
    the uniquely identifies the mapping tag but can be used where hashes are more
    convenient.
  */
  const ON_SHA1_Hash Hash() const;
};

ON_DECL
bool operator==(const ON_MappingTag& lhs, const ON_MappingTag& rhs);

ON_DECL
bool operator!=(const ON_MappingTag& lhs, const ON_MappingTag& rhs);

class ON_CLASS ON_TextureCoordinates
{
public:
  ON_TextureCoordinates();

  ON_MappingTag   m_tag;
  int                        m_dim; // 1, 2, or 3
  ON_SimpleArray<ON_3fPoint> m_T;   // texture coordinates
};


#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_MappingTag>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_TextureCoordinates>;
#endif

class ON_CLASS ON_SurfaceCurvatureColorMapping
{
public:
  /// <summary>
  /// {639E9144-1C1A-4bba-8248-D330F50D7B69}
  /// The id ON_SurfaceCurvatureColorMapping::Id identifies
  /// the surface curvature color analysis used by the Rhino CurvatureAnalysis command
  /// and is used as the id for mapping tags that indicate per vertex colors
  /// were set from surface principal curvatures.
  /// </summary>
  static const ON_UUID Id;

  /// <summary>
  /// The default surface curvature color analysis hue range used
  /// by the Rhino CurvatureAnalysis command is
  /// 0 to 4pi/3 (red to blue)
  /// </summary>
  static const ON_Interval DefaultHueRangeRadians;

  static const ON_SurfaceCurvatureColorMapping Unset;

public:
  ON_SurfaceCurvatureColorMapping() = default;
  ~ON_SurfaceCurvatureColorMapping() = default;
  ON_SurfaceCurvatureColorMapping(const ON_SurfaceCurvatureColorMapping&) = default;
  ON_SurfaceCurvatureColorMapping& operator=(const ON_SurfaceCurvatureColorMapping&) = default;

  /// <summary>
  /// Construct a ON_SurfaceCurvatureColorMapping for converting a specified
  /// range of curvatures to a color.
  /// </summary>
  /// <param name="kappa_style">
  /// Specifies what curvature value to used.
  /// </param>
  /// <param name="kappa_range">
  /// A range that is used to convert kappa values to colors. 
  /// This interval may be increasing, decreasing, or a singleton.
  /// A kappa value equal to kappa_range[0] is mapped to 
  /// ON_SurfaceCurvatureColorMapping::DefaultHueRangeRadians[0].
  /// A kappa value equal to kappa_range[1] is mapped to 
  /// ON_SurfaceCurvatureColorMapping::DefaultHueRangeRadians[1].
  /// A kappa value k in between kappa_range[0] and kappa_range[1]
  /// is mapped to the saturated color with 
  /// hue = ON_SurfaceCurvatureColorMapping::DefaultHueRangeRadians.ParameterAt(kappa_range.NormalizedParameterAt(k)).
  /// Kappa values outside the kappa_range interval are mapped to the hue
  /// assigned to the nearest end of the kappa_range interval.
  /// </param>
  ON_SurfaceCurvatureColorMapping(
    ON::curvature_style m_kappa_style,
    ON_Interval kappa_range
  );

  /// <summary>
  /// Construct a ON_SurfaceCurvatureColorMapping for converting a specified
  /// range of curvatures to a color.
  /// </summary>
  /// <param name="kappa_style">
  /// Specifies what curvature value to used.
  /// </param>
  /// <param name="kappa_range">
  /// A range that is used to convert kappa values to colors. 
  /// This interval may be increasing, decreasing, or a singleton.
  /// A kappa value equal to kappa_range[0] is mapped to hue_range_in_radians[0].
  /// A kappa value equal to kappa_range[1] is mapped to hue_range_in_radians[1].
  /// A kappa value k in between kappa_range[0] and kappa_range[1]
  /// is mapped to the saturated color with 
  /// hue = hue_range_in_radians.ParameterAt(kappa_range.NormalizedParameterAt(k)).
  /// Kappa values outside the kappa_range interval are mapped to the hue
  /// assigned to the nearest end of the kappa_range interval.
  /// </param>
  /// <param name="hue_range_in_radians">
  /// When in doubt, pass ON_SurfaceCurvatureColorMapping::DefaultHueRangeRadians.
  /// This parameter specifies the range of color hues (in radians) assigned
  /// to the kappa_range interval.
  /// This interval may be increasing or decreasing.
  /// </param>
  ON_SurfaceCurvatureColorMapping(
    ON::curvature_style kappa_style,
    ON_Interval kappa_range,
    ON_Interval hue_range_in_radians
  );

  /// <returns>
  /// If the curvature type, range, and hure range are set and valid, then true is returned.
  /// Otherwise false is returned.
  /// </returns>
  bool IsSet() const;

  /// <returns>
  /// If one or more of the  curvature type, range, and hure range are not set or are not valid,
  /// valid, then true is returned.
  /// Otherwise false is returned.
  /// </returns>
  bool IsUnset() const;

  /// <summary>
  /// Get the color the the settings in this ON_SurfaceCurvatureColorMapping assign
  /// to a pair of principal surface curvatures.
  /// </summary>
  /// <param name="K">
  /// principal surface curvatures
  /// </param>
  /// <returns>
  /// If K and this->IsSet() is true, then the color assigned to the principal curvatures K is returned.
  /// Otherwise ON_Color::UnsetColor is returned.
  /// </returns>
  const ON_Color Color(
    ON_SurfaceCurvature K
  ) const;

  /// <summary>
  /// Get a SHA1 hash that uniquely identifies the 
  /// settings in this ON_SurfaceCurvatureColorMapping. 
  /// This hash is ideal for use as a SubD fragment colors hash.
  /// </summary>
  /// <returns>
  /// If this->IsSet() is true, a SHA1 hash that uniquely identifies the 
  /// settings in this ON_SurfaceCurvatureColorMapping is returned.
  /// Otherwise ON_SHA1_Hash::EmptyContentHash() is returned.
  /// </returns>
  const ON_SHA1_Hash Hash() const;

  /// <summary>
  /// Get a 32 bit CRC hash that identifies the 
  /// settings in this ON_SurfaceCurvatureColorMapping. 
  /// </summary>
  /// <returns>
  /// If this->IsSet() is true, this->Hash()CRC32(0) is returned.
  /// Otherwise 0 is returned.
  /// </returns>
  ON__UINT32 CRC32() const;

  /// <summary>
  /// The returned mapping tag is ideal for setting the ON_SubD fragment and ON_Mesh
  /// per vertex color tag when the colors are set by calling this->Color()
  /// on the per vertex principal surface curvatures.
  /// </summary>
  /// <returns>
  /// If this->ISSet() is true, a mapping tag with 
  /// type = ON_TextureMapping::TYPE::false_colors,
  /// id = ON_SurfaceCurvatureColorMapping::Id,
  /// crc = this->CRC32()
  /// xform = identity
  /// is returned. Otherwise ON_MappingTag::Unset is returned.
  /// </returns>
  const ON_MappingTag ColorMappingTag() const;

  ON::curvature_style KappaStyle() const;

  const ON_Interval KappaRange() const;

  const ON_Interval HueRangeInRadians() const;

private:
  mutable unsigned char m_is_set = 0; // 0 = not set, 1 = this is valid and set, 2 = unknown
  ON__UINT8 m_reserved1 = 0;
  ON__UINT16 m_reserved2 = 0;
  ON::curvature_style m_kappa_style = ON::curvature_style::unknown_curvature_style;
  ON_Interval m_kappa_range = ON_Interval::Nan;
  ON_Interval m_hue_range_in_radians = ON_SurfaceCurvatureColorMapping::DefaultHueRangeRadians;
};

/// <summary>
/// Returns true if all properties of lhs and rhs are identical and no double is a nan.
/// </summary>
ON_DECL
bool operator==(const ON_SurfaceCurvatureColorMapping& lhs, const ON_SurfaceCurvatureColorMapping& rhs);

/// <summary>
/// Returns true if some property of lhs and rhs is different and no double is a nan.
/// </summary>
ON_DECL
bool operator!=(const ON_SurfaceCurvatureColorMapping& lhs, const ON_SurfaceCurvatureColorMapping& rhs);


class ON_CLASS ON_SurfaceDraftAngleColorMapping
{
public:
  /// <summary>
  ///  // {F08463F4-22E2-4cf1-B810-F01925446D71}
  /// The id ON_SurfaceDraftAngleColorMapping::Id identifies
  /// the surface draft angle color analysis used by the Rhino DraftAngleAnalysis command
  /// and is used as the id for mapping tags that indicate per vertex colors
  /// were set from surface normal draft angles.
  /// </summary>
  static const ON_UUID Id;

  /// <summary>
  /// The default surface draft angle color analysis hue range used
  /// by the Rhino DraftAngleAnalysis command is
  /// 0 to 4pi/3 (red to blue)
  /// </summary>
  static const ON_Interval DefaultHueRangeRadians;

  static const ON_SurfaceDraftAngleColorMapping Unset;

public:
  ON_SurfaceDraftAngleColorMapping() = default;
  ~ON_SurfaceDraftAngleColorMapping() = default;
  ON_SurfaceDraftAngleColorMapping(const ON_SurfaceDraftAngleColorMapping&) = default;
  ON_SurfaceDraftAngleColorMapping& operator=(const ON_SurfaceDraftAngleColorMapping&) = default;

  /// <summary>
  /// Construct a ON_SurfaceDraftAngleColorMapping for converting a specified
  /// range of draft angles to a color.
  /// </summary>
  /// <param name="up">
  /// Specify the up direction used to calculate the draft angle.
  /// The draft angle of a surface normal N is 0 if N is perpendicular to up,
  /// pi/2 (90 degrees) if N an up point the same direction,
  /// and -pi/2 (-90 degrees) if N and up point opposite directions.
  /// </param>
  /// <param name="angle_range_in_radians">
  /// A range that is used to convert draft angles to colors. 
  /// This interval may be increasing, decreasing, or a singleton.
  /// A draft angle equal to angle_range_in_radians[0] is mapped to 
  /// ON_SurfaceDraftAngleColorMapping::DefaultHueRangeRadians[0].
  /// A draft angle equal to angle_range_in_radians[1] is mapped to 
  /// ON_SurfaceDraftAngleColorMapping::DefaultHueRangeRadians[1].
  /// A draft angle a in between angle_range_in_radians[0] and angle_range_in_radians[1]
  /// is mapped to the saturated color with 
  /// hue = ON_SurfaceDraftAngleColorMapping::DefaultHueRangeRadians.ParameterAt(angle_range_in_radians.NormalizedParameterAt(a)).
  /// Draft angles outside the angle_range_in_radians interval are mapped to the hue 
  /// assigned to the nearest end of the angle_range_in_radians interval.
  /// </param>
  ON_SurfaceDraftAngleColorMapping(
    ON_3dVector up,
    ON_Interval angle_range_in_radians
  );

  /// <summary>
  /// Construct a ON_SurfaceDraftAngleColorMapping for converting a specified
  /// range of draft angles to a color.
  /// </summary>
  /// <param name="up">
  /// Specify the up direction used to calculate the draft angle.
  /// The draft angle of a surface normal N is 0 if N is perpendicular to up,
  /// pi/2 (90 degrees) if N an up point the same direction,
  /// and -pi/2 (-90 degrees) if N and up point opposite directions.
  /// </param>
  /// <param name="angle_range_in_radians">
  /// A range that is used to convert draft angles to colors. 
  /// This interval may be increasing, decreasing, or a singleton.
  /// A draft angle equal to angle_range_in_radians[0] is mapped to 
  /// hue_range_in_radians[0].
  /// A draft angle equal to angle_range_in_radians[1] is mapped to 
  /// hue_range_in_radians[1].
  /// A draft angle a in between angle_range_in_radians[0] and angle_range_in_radians[1]
  /// is mapped to the saturated color with 
  /// hue = hue_range_in_radians.ParameterAt(angle_range_in_radians.NormalizedParameterAt(a)).
  /// Draft angles outside the angle_range_in_radians interval are mapped to the hue 
  /// assigned to the nearest end of the angle_range_in_radians interval.
  /// </param>
  /// <param name="hue_range_in_radians">
  /// When in doubt, pass ON_SurfaceDraftAngleColorMapping::DefaultHueRangeRadians.
  /// This parameter specifies the range of color hues (in radians) assigned
  /// to the angle_range_in_radians interval.
  /// The hue interval may be increasing or decreasing.
  /// </param>
  ON_SurfaceDraftAngleColorMapping(
    ON_3dVector up,
    ON_Interval angle_range_in_radians,
    ON_Interval hue_range_in_radians
  );

  /// <returns>
  /// If the draft angle up, angle range, and hue range are set and valid, then true is returned.
  /// Otherwise false is returned.
  /// </returns>
  bool IsSet() const;

  /// <returns>
  /// If one or more of the up vector, angle range, and hue range are not set or are not valid,
  /// valid, then true is returned.
  /// Otherwise false is returned.
  /// </returns>
  bool IsUnset() const;

  /// <summary>
  /// Get the color the the settings in this ON_SurfaceDraftAngleColorMapping assign
  /// to a surface normal.
  /// </summary>
  /// <param name="surface_normal">
  /// Unit length normal to the surface.
  /// </param>
  /// <returns>
  /// If surface_normal is set and this->IsSet() is true, 
  /// then the color assigned to the resulting draft angle is returned.
  /// Otherwise ON_Color::UnsetColor is returned.
  /// </returns>
  const ON_Color Color(
    ON_3dVector surface_normal
  ) const;

  /// <summary>
  /// Get a SHA1 hash that uniquely identifies the 
  /// settings in this ON_SurfaceDraftAngleColorMapping. 
  /// This hash is ideal for use as a SubD fragment colors hash.
  /// </summary>
  /// <returns>
  /// If this->IsSet() is true, a SHA1 hash that uniquely identifies the 
  /// settings in this ON_SurfaceDraftAngleColorMapping is returned.
  /// Otherwise ON_SHA1_Hash::EmptyContentHash() is returned.
  /// </returns>
  const ON_SHA1_Hash Hash() const;

  /// <summary>
  /// Get a 32 bit CRC hash that identifies the 
  /// settings in this ON_SurfaceDraftAngleColorMapping. 
  /// </summary>
  /// <returns>
  /// If this->IsSet() is true, this->Hash()CRC32(0) is returned.
  /// Otherwise 0 is returned.
  /// </returns>
  ON__UINT32 CRC32() const;

  /// <summary>
  /// The returned mapping tag is ideal for setting the ON_SubD fragment and ON_Mesh
  /// per vertex color tag when the colors are set by calling this->Color()
  /// on the per vertex principal surface curvatures.
  /// </summary>
  /// <returns>
  /// If this->IsSet() is true, a mapping tag with 
  /// type = ON_TextureMapping::TYPE::false_colors,
  /// id = ON_SurfaceDraftAngleColorMapping::Id,
  /// crc = this->CRC32()
  /// xform = identity
  /// is returned. Otherwise ON_MappingTag::Unset is returned.
  /// </returns>
  const ON_MappingTag ColorMappingTag() const;

  const ON_3dVector Up() const;

  const ON_Interval AngleRangeInRadians() const;

  const ON_Interval HueRangeInRadians() const;

private:
  mutable unsigned char m_is_set = 0; // 0 = not set, 1 = this is valid and set, 2 = unknown
  ON__UINT8 m_reserved1 = 0;
  ON__UINT16 m_reserved2 = 0;
  ON__UINT32 m_reserved3 = 0;
  ON_3dVector m_up = ON_3dVector::NanVector;
  ON_Interval m_angle_range_in_radians = ON_Interval::Nan;
  ON_Interval m_hue_range_in_radians = ON_SurfaceDraftAngleColorMapping::DefaultHueRangeRadians;
};

/// <summary>
/// Returns true if all properties of lhs and rhs are identical and no double is a nan.
/// </summary>
ON_DECL
bool operator==(const ON_SurfaceDraftAngleColorMapping& lhs, const ON_SurfaceDraftAngleColorMapping& rhs);

/// <summary>
/// Returns true if some property of lhs and rhs is different and no double is a nan.
/// </summary>
ON_DECL
bool operator!=(const ON_SurfaceDraftAngleColorMapping& lhs, const ON_SurfaceDraftAngleColorMapping& rhs);


class ON_CLASS ON_Mesh : public ON_Geometry
{
  ON_OBJECT_DECLARE(ON_Mesh);

public:
  ON_Mesh();
  ON_Mesh(
    int   initial_face_array_capacity,   // initial face array capacity
    int   initial_vertex_array_capacity, // initial vertex array capacity
    bool  has_vertex_normals,            // true if mesh has vertex normals
    bool  has_texture_coordinates        // true if mesh has texture coordinates
    );
  ON_Mesh( const ON_Mesh& );
  ON_Mesh& operator=( const ON_Mesh& );
  ~ON_Mesh();

public:
  static const ON_Mesh Empty;


#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Create a mesh that fills in a 3d polygon.
  Parameters:
    polygon_points_count - [in]
      >= 3
    polygon_points - [in]
      An array of distinct 3d points that forms a polygon. 
      The points do not have to be coplanar, but they should be
      close to coplanar if you want a decent result.
    planar_tolerance - [in]
      If planar_tolerance >= 0.0 and polygon_points >= 4, then
      planar_tolerance is a 3d distance tolerance use to determine
      if the polygon is planar.  When a polygon is planar, all 
      vertex normals are identical. When a polygon is not planar, 
      vertex normals may vary.
      If planar_tolerance is not >= 0.0, then all vertex normals
      are identical in all cases.
    destintation_mesh - [in]
      null or a pointer to the mesh to be used.
  Returns:
    A pointer to a mesh that fills the 3d polygon or
    null if the input is not valid.
  */
  static ON_Mesh* From3dPolygon( 
    size_t polygon_points_count,
    const ON_3dPoint* polygon_points,
    double planar_tolerance,
    ON_Mesh* destintation_mesh
    );

#if defined(OPENNURBS_PLUS)
  /// <summary>
  /// Create a triangle mesh with vertices at the input points and which includes the specified edges.
  /// Some subset of the edges must form a closed outer boundary. If you want holes inside the mesh,
  /// orient the edges that form the inner boundaries and pass the appropriate information to indicate
  /// which side of these edges you want the mesh to be on.
  /// 
  /// This is the most advanced version of CreateFrom2dPointsAndEdges(...). If your point and edtge
  /// input is in simple arrays, there are easier to call overloads with the same name and
  /// simple array parameters.
  /// </summary>
  /// <param name="point_count">
  /// Number of 2d points. There must be at least 3 points.
  /// </param>
  /// <param name="point_stride">
  /// Double offset for 2d points. 
  /// i-th 2d point = (points[j],points[j+1]), where j = i*point_stride.
  /// </param>
  /// <param name="points">
  /// 2d point locations.  
  /// Duplicate points are not permitted.
  /// It is ok to include points that are inside the outer boundary 
  /// but are not at the end of any input edge.
  /// For 0 %lt;= i &lt point_count, the returned mesh will have mesh->Vertex(i) = points[i].
  /// </param>
  /// <param name="edge_count">
  /// Number of edges in the edges[] array.
  /// If edge_count = 0, then the convex hull of points[] will be the boundary.
  /// If edge_count = point_count, edge_stride = 0, edges = nullptr, and edge_side_stride = 0
  /// then the input list of points must from a closed polyline and will be the 
  /// outer boundary of the region to be meshed. Note this polyline does not have to be convex.
  /// </param>
  /// <param name="edge_stride">
  /// i-th edge connects points (edge[j],edge[j+1]) where j = i*edge_stride.
  /// </param>
  /// <param name="edges">
  /// The edges[] array contains pairs of points[] array indices
  /// the specify line segments that should be sides of triangles.
  /// The edges do not have to be sorted into connected components.
  /// It is not required to specify an outer boundary. If some subset of edges forms
  /// a closed polyline that contains all other edges, then that polyline will automatically
  /// become the outer boundary, even if there are points outside of this polyline.
  /// If any edges intersect, then the intersection points are added to the mesh. 
  /// Any added mesh vertices will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="edge_orientation_stride">
  /// If edge_orientations is not nullptr, then the edge orientation for the i-th edge is 
  /// edge_orientations[i*edge_orientation_stride]. 
  /// If all edges have a constant orientation value, then you may set edge_orientation_stride = 0 
  /// and pass the address of an unsigned char whose value is the constant orientation.
  /// </param>
  /// <param name="edge_orientations">
  /// If not nullptr, then edge_orientations[i*edge_orientation_stride] determines which side(s) 
  /// of the i-th edge should be part of a triangle.
  /// 0 = Automatically determined edge orientation.
  ///     This is the best choice for all edges that are not inner boundary edges.
  /// 1 = left side only - boundary edge with a triangle on the left side.
  ///     This is a good choice for clockwise inner boundary edges.
  /// 2 = right side only - boundary edge with a triangle on the right side.
  ///     This is a good choice for counter-clockwise inner boundary edges.
  /// If edge_orientations is nullptr, then 0 is used for all edges.
  /// If all edges have the same orientation, then setting edge_orientation_stride=0
  /// and passing the address of an unsigned char with the constant orientation value
  /// saves having to create and manage an array where every element has the same value.
  /// </param>
  /// <param name="bOuterBoundaryIsConvexHull">
  /// If edges[] contains one or more outer boundaries, then pass false.
  /// Otherwise pass true; the outer boundary of the returned mesh will be the convex
  /// hull of points[] and any edges that are not part of the convex hull will be inside
  /// the mesh.
  /// </param>
  /// <param name="bPermitVertexAdditions">
  /// If true, then additional interior points may be added to improve the
  /// quality of the mesh. These additional points will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="bPermitEdgeSplitting">
  /// If true, then edges may be split to improve mesh quality.
  /// If any edges are split, the additional points will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="destination_mesh">
  /// If destination_mesh is not nullptr, then the mesh will be created in this
  /// mesh. If destination_mesh is nullptr, then a new mesh created by calling new ON_Mesh() will
  /// be returned and the caller is responsible for managing the mesh and calling delete 
  /// when it is no longer needed.
  /// </param>
  /// <returns>
  /// A triangle mesh of the 2d region specified by the input points and edges.
  /// For 0 &lt;= i %lt; point_count, mesh->Vertex(i) is the i-th input point.
  /// </returns>
  static ON_Mesh* CreateFrom2dPointsAndEdges(
    size_t point_count,
    size_t point_stride,
    const double* points,
    size_t edge_count,
    size_t edge_stride,
    const unsigned int* edges,
    size_t edge_orientation_stride,
    const unsigned char* edge_orientations,
    bool bOuterBoundaryIsConvexHull,
    bool bPermitVertexAdditions,
    bool bPermitEdgeSplitting,
    ON_Mesh* destination_mesh
  );

  /// <summary>
  /// Create a triangle mesh with vertices at the input points and which includes the specified edges.
  /// If no edges are specified, then the convex hull of points[] will be the mesh boundary.
  /// If you want holes inside the mesh, orient the edges that form the inner boundaries of the holes
  /// and pass the appropriate information to indicate which side of these edges you want the mesh to be on.
  /// 
  /// This overload of ON_Mesh::CreateFrom2dPointsAndEdges() is designed for case where your edges are
  /// in a single simple array and you don't need to specify edge orientations,
  /// or a single orientation applies to all edges, or you have detailed customized per edge orientations.
  /// If you find managing customized per edge orientations challenging or the source
  /// of your edges is already partitioned into distinct simple arrays, there is an overload of
  /// of ON_Mesh::CreateFrom2dPointsAndEdges() that lets you pas in multiple arrays of edges that have
  /// similar orientations.
  /// </summary>
  /// <param name="points">
  /// 2d point locations.  
  /// Duplicate points are not permitted.
  /// It is ok to include points that are inside the outer boundary 
  /// but are not at the end of any input edge.
  /// For 0 %lt;= i &lt point_count, the returned mesh will have mesh->Vertex(i) = points[i].
  /// </param>
  /// <param name="edges">
  /// If edges[] is empty, then the convex hull of points[] will be the mesh boundary.
  /// The edges[] array contains pairs of points[] array indices
  /// the specify line segments that should be sides of triangles.
  /// The edges do not have to be sorted into connected components.
  /// It is not required to specify an outer boundary. If some subset of edges forms
  /// a closed polyline that contains all other edges, then that polyline will automatically
  /// become the outer boundary, even if there are points outside of this polyline.
  /// If any edges intersect, then the intersection points are added to the mesh. 
  /// Any added mesh vertices will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="edge_orientations">
  /// If edge_orientations[] is empty, the all interior edges have a triangle on both sides.
  /// If edge_orientations[] has one element, then that orientation is used for all edges and
  /// the edges will have one triangle on the specified side.
  /// Otherwise, edge_orientations[] must have edges.Count() elements, and edge_orientations[i]
  /// specifies which side(s) or edges[i] should have a triangle.
  /// 0 = Automatically determined edge orientation.
  ///     This is the best choice for all edges that are not inner boundary edges.
  /// 1 = left side only - boundary edge with a triangle on the left side.
  ///     This is a good choice for clockwise inner boundary edges.
  /// 2 = right side only - boundary edge with a triangle on the right side.
  ///     This is a good choice for counter-clockwise inner boundary edges.
  /// </param>
  /// <param name="bOuterBoundaryIsConvexHull">
  /// If edges[] contains one or more outer boundaries, then pass false.
  /// Otherwise pass true; the outer boundary of the returned mesh will be the convex
  /// hull of points[] and any edges that are not part of the convex hull will be inside
  /// the mesh.
  /// </param>
  /// <param name="bPermitVertexAdditions">
  /// If true, then additional interior points may be added to improve the quality of the mesh. 
  /// These additional points will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="bPermitEdgeSplitting">
  /// If true, then input edges may be split to improve mesh quality.
  /// If any edges are split, the additional points will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="destination_mesh">
  /// If destination_mesh is not nullptr, then the mesh will be created in destination_mesh
  /// and destination_mesh will be returned.
  /// Otherwise the returned mesh created by calling new ON_Mesh() will
  /// be returned and the caller is responsible for managing the mesh and calling delete 
  /// when it is no longer needed.
  /// </param>
  /// <returns>
  /// If successful, a pointer to a triangle mesh of the 2d region specified by points[] and edges[].
  /// For 0 &lt;= i %lt; point_count, mesh->Vertex(i) is the i-th input point.
  /// </returns>
  static ON_Mesh* CreateFrom2dPointsAndEdges(
    const ON_SimpleArray<ON_2dPoint>& points,
    const ON_SimpleArray<ON_2udex>& edges,
    const ON_SimpleArray<unsigned char>& edge_orientations,
    bool bOuterBoundaryIsConvexHull,
    bool bPermitVertexAdditions,
    bool bPermitEdgeSplitting,
    ON_Mesh* destination_mesh
  );


  /// <summary>
  /// Create a triangle mesh with vertices at the input points and which includes the specified edges.
  /// If no edges are specified, then the convex hull of points[] will be the mesh boundary.
  /// The resulting mesh can have no outer boundary specified; in this case the outer boundary will
  /// be the convex hull of the points. Or edges specifying one or more outer boundaries can be passed in.
  /// The resulting mesh may have zero or more inner boundaries (holes in the mesh). Additionally
  /// interior edges (having triangles on both sides) can be specified.
  /// 
  /// Edges specifying outer boundaries do not need to be properly oriented if they are passed in the
  /// automatic_edges[] list. There is no need to sort outer boundary edges into loops.
  /// 
  /// Edges specifying inner boundaries must be oriented so they form clockwise loops
  /// and must be passed in the oriented_edges[] list. There is no need to sort inner boundary edges
  /// into loops.
  /// 
  /// You may also specify interior edges that should have a triangle on both sides of the interior edge.
  /// Interior edges can have any orientation and must be passed in the automatic_edges[] list.
  /// There is no need to sort interior edges into connected sets.
  /// </summary>
  /// <param name="points">
  /// 2d point locations.  
  /// Duplicate points are not permitted.
  /// It is ok to include points that are inside the outer boundary 
  /// but are not at the end of any input edge.
  /// For 0 %lt;= i &lt point_count, the returned mesh will have mesh->Vertex(i) = points[i].
  /// </param>
  /// <param name="automatic_edges">
  /// Duplicate edges are not permitted. 
  /// The edges in automatic_edges[] can be in any order and have any orientation. 
  /// In particular they do not have to be sorted into connected sets.
  /// This is a good choice for almost all edges that are not inner boundary edges.
  /// </param>
  /// <param name="oriented_boundary_edges">
  /// Duplicate edges are not permitted. 
  /// The edges in oriented_boundary_edges[] can be in any order. 
  /// In particular they do not have to be sorted into connected sets.
  /// The edges in oriented_edges[] should be part of some boundary loop (inner or outer).
  /// This is the only place to specify inner boundaries (holes in the mesh). 
  /// If there are outer boundaries and the edges forming the outer boundaries
  /// are oriented so the mesh is on the left sde of the edge, then they may be included 
  /// in either oriented_edges[] or automatic_edges[], but not both.
  /// </param>
  /// <param name="bOuterBoundaryIsConvexHull">
  /// If edges[] contains one or more outer boundaries, then pass false.
  /// Otherwise pass true; the outer boundary of the returned mesh will be the convex
  /// hull of points[] and any edges that are not part of the convex hull will be inside
  /// the mesh.
  /// </param>
  /// <param name="bPermitVertexAdditions">
  /// When in doubt, pass true.
  /// If true, then additional interior points may be added to improve the quality of the mesh. 
  /// These additional points will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="bPermitEdgeSplitting">
  /// When in doubt, pass true.
  /// If true, then input edges may be split to improve mesh quality.
  /// If any edges are split, the additional points will have mesh vertex indices &gt;= point_count.
  /// </param>
  /// <param name="destination_mesh">
  /// If destination_mesh is not nullptr, then the mesh will be created in destination_mesh
  /// and destination_mesh will be returned.
  /// Otherwise the returned mesh created by calling new ON_Mesh() will
  /// be returned and the caller is responsible for managing the mesh and calling delete 
  /// when it is no longer needed.
  /// </param>
  /// <returns>
  /// If successful, a pointer to a triangle mesh of the 2d region specified by points[] and edges[].
  /// For 0 &lt;= i %lt; point_count, mesh->Vertex(i) is the i-th input point.
  /// When the returned mesh has mesh->VertexCount() &gt; point_count, it is because
  /// inter edge intersections were added or the a true value of
  /// bPermitVertexAdditions or bPermitEdgeSplitting resulted in vertices being added
  /// to improve triangle quality.
  /// </returns>
  static ON_Mesh* CreateFrom2dPointsAndEdges(
    const ON_SimpleArray<ON_2dPoint>& points,
    const ON_SimpleArray<ON_2udex>& automatic_edges,
    const ON_SimpleArray<ON_2udex>& oriented_boundary_edges,
    bool bOuterBoundaryIsConvexHull,
    bool bPermitVertexAdditions,
    bool bPermitEdgeSplitting,
    ON_Mesh* destination_mesh
  );


#endif

  /*
Description:
  Create a mesh from a filtered mesh list.
Parameters:
  original - [in] a mesh input
  pattern - [in]
    An array of boolean values.
  patternLength - [in]
    Length of pattern.
Returns:
  A pointer to a mesh that is the result, or nullptr.
*/
  static ON_Mesh* FromFilteredFaceList(
    const ON_Mesh& original,
    bool* pattern,
    unsigned patternLength
  );

  static ON_Mesh* From3dPolygon( 
    const ON_SimpleArray< ON_3dPoint >& polygon_points,
    double planar_tolerance,
    ON_Mesh* destintation_mesh
    );

  /*
  Description:
    Intersects a group of meshes. This method uses the new code.
  Parameters:
    meshes - [in] The mesh input list. nullptr entries are tolerated.
    tolerance - [in]
      A 3d-distance tolerance value. If tolerance is:
       - negative: the positive value will be used.
       - 0.0: no tolerance will be used. This is rarely useful, and only in cases where perforations intersections happen inside
          faces, and/or there is coplanarity parallel to one of the three fundamental axes.
       - ON_DBL_QNAN, ON_UNSET_VALUE and ON_UNSET_POSITIVE_VALUE: the function picks a suitable, or default, tolerance value.
      WARNING! Generally, good tolerance values for objects drawn near the origin are in the magnitude of 10^-7, or
      ON_SQRT_EPSILON * 10. Often, the document absolute tolerance is not a good tolerance value. You might use a fractional
      coefficient to scale the document tolerance to a suitable value, while accepting a user-provided value.
      This function expects that each mesh conforms to the tolerance that is provided, and that means that features in the mesh
      are always larger than the mentioned tolerance. Coplanar gaps between meshes that are smaller than tolerance are overlaps.
    perforatingResults - [out]
      If not nullptr, this array will be filled with the polylines from perforating intersections.
    overlapResults - [out]
      If not nullptr, this array will be filled with the polylines from overlaps.
      This can be (but does not need to be) the same object provided to intersectionResults.
    overlapMeshResult - [out]
      If not nullptr, this mesh will have new faces added that represent the coplar unions of inputs.
      If this output is not required, it's best to leave this as nullptr.
    log - [out]
      If not nullptr, intersection warnings and errors are printed here. If the intersection runs smoothly, nothing will
      generally be printed.
    cancel - [out]
      If not nullptr, a trigger to request termination. When cancellation is requested, the return value will often be false.
      You are responsible to delete new objects that were returned, even if you requested cancellation.
    reporter - [out]
      If not nullptr, an indicative progress is reported.
  Returns:
    true if the computation finished successfully, false otherwise.
  */
  static bool IntersectArray(
    const ON_SimpleArray<const ON_Mesh*>& meshes,
    double tolerance,
    ON_SimpleArray<ON_Polyline*>* perforatingResults,
    ON_SimpleArray<ON_Polyline*>* overlapResults,
    ON_Mesh* overlapMeshResult,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter);

  /*
  Description:
    Return information on intersections and overlaps for a group of meshes. This method uses the new code.

    anyTypeOfIntersection - [in] if provided, it will be set to true if any mesh perforates another or overlaps - or nullptr.
    pairs - [in] if provided, a list of pairs of mesh indices that intersect or overlap. Each pair appears once only - or nullptr.

    See the parameter description in the function above.
  */
  static bool IntersectArrayPredicate(
    const ON_SimpleArray<const ON_Mesh*>& meshesA,
    const ON_SimpleArray<const ON_Mesh*>* meshesB,
    ON_MeshIntersectionCache* cacheForMeshesB,
    double tolerance,
    bool* anyTypeOfIntersection,
    ON_SimpleArray<ON_2dex>* pairs,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter);

  /*
  Get intersections with the current mesh. Intersections between other meshes are ignored.
  See the parameter description in the function above.
  */
  bool GetIntersections(
    const ON_SimpleArray<const ON_Mesh*>& withTheseOtherMeshes,
    double tolerance,
    ON_SimpleArray<ON_Polyline*>* perforatingResults,
    ON_SimpleArray<ON_Polyline*>* overlapResults,
    ON_Mesh* overlapMeshResult,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter) const;

  /*
  Get intersections with the current mesh. Intersections between other meshes are ignored.
  See the parameter description in the function above.

  cacheForOtherMeshes [in/out] - Users of this class can provide nullptr, or call this method with a default-initialized
  instance of ON_MeshIntersectionCache. In this case, the cache can be used with other calls of this function that include exactly withTheseOtherMeshes.
*/
  bool GetIntersections(
    const ON_SimpleArray<const ON_Mesh*>& withTheseOtherMeshes,
    ON_MeshIntersectionCache* cacheForOtherMeshes,
    double tolerance,
    ON_SimpleArray<ON_Polyline*>* perforatingResults,
    ON_SimpleArray<ON_Polyline*>* overlapResults,
    ON_Mesh* overlapMeshResult,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter) const;

/*
Get intersections with the current mesh. Intersections between other meshes are ignored.
See the parameter description in the function above.

ON_MeshIntersectionOptions [in] - Users of this class can provide nullptr, or call this method with a default-initialized
instance of ON_MeshIntersectionOptions.
*/
  bool GetIntersections(
    const ON_SimpleArray<const ON_Mesh*>& withTheseOtherMeshes,
    ON_MeshIntersectionOptions* options,
    ON_SimpleArray<ON_Polyline*>* perforatingResults,
    ON_SimpleArray<ON_Polyline*>* overlapResults,
    ON_Mesh* overlapMeshResult) const;
  

  /*
    Get self intersections on the current mesh.
    See the parameter description in the function above.
  */
  bool GetSelfIntersections(
    double tolerance,
    ON_SimpleArray<ON_Polyline*>* perforatingResults,
    ON_SimpleArray<ON_Polyline*>* overlapResults,
    ON_Mesh* overlapMeshResult,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter) const;

  /*
    Get self intersections on the current mesh.
    See the parameter description in the function above.
  */
  bool GetSelfIntersections(
    ON_MeshIntersectionOptions* options,
    ON_SimpleArray<ON_Polyline*>* perforatingResults,
    ON_SimpleArray<ON_Polyline*>* overlapResults,
    ON_Mesh* overlapMeshResult) const;

  /*
  Description:
    Splits the current mesh and returns the split result.
    If nothing happened, the current mesh is not copied. Instead, somethingHappened is set to false.
  Parameters:
    meshesThatSplit - [in] The mesh splitters. nullptr entries are tolerated.
    tolerance - [in]
      A 3d-distance tolerance value. If tolerance is:
       - negative: the positive value will be used.
       - 0.0: no tolerance will be used. This is rarely useful, and only in cases where perforations intersections happen inside
          faces, and/or there is coplanarity parallel to one of the three fundamental axes.
       - ON_DBL_QNAN, ON_UNSET_VALUE and ON_UNSET_POSITIVE_VALUE: the function picks a suitable, or default, tolerance value.
      WARNING! Generally, good tolerance values for objects drawn near the origin are in the magnitude of 10^-7, or
      ON_SQRT_EPSILON * 10. Often, the document absolute tolerance is not a good tolerance value. You might use a fractional
      coefficient to scale the document tolerance to a suitable value, while accepting a user-provided value.
      This function expects that each mesh conforms to the tolerance that is provided, and that means that features in the mesh
      are always larger than the mentioned tolerance. Coplanar gaps between meshes that are smaller than tolerance are overlaps.
    somethingHappened - [out] If not nullptr, informs the caller if the mesh was actually hit by anything.
    results - [out]
      This array will be filled with the split result. If nothing happened, the current mesh is not copied.
    log - [out]
      If not nullptr, intersection warnings and errors are printed here. If the intersection runs smoothly, nothing will be printed.
    cancel - [out]
      If not nullptr, a trigger to request termination. When cancellation is requested, the return value will often be false.
      You are responsible to delete new objects that were returned, even if you requested cancellation.
    reporter - [out]
      If not nullptr, an indicative progress is reported.
  Returns:
    true if the computation finished successfully, false otherwise.
  */
  bool Split(
    const ON_SimpleArray<const ON_Mesh*>& meshesThatSplit,
    double tolerance,
    bool splitAtCoplanar,
    bool createNgons,
    bool* somethingHappened,
    ON_SimpleArray<ON_Mesh*>& results,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter) const;

  /*
  Splits a list of meshes.
  See the parameter description two methods above.
  Additional parameter:
    createNgons - If true, ngons are created along the splitting ridge to preserve the boundary look.
  */
  static bool SplitArray(
    const ON_SimpleArray<const ON_Mesh*>& meshesToSplit,
    const ON_SimpleArray<const ON_Mesh*>& meshesThatSplit,
    double tolerance,
    bool splitAtCoplanar,
    bool createNgons,
    bool* somethingHappened,
    ON_SimpleArray<ON_Mesh*>& results,
    ON_TextLog* log,
    ON_Terminator* cancel,
    ON_ProgressReporter* reporter);

  /*
  Description:
    Finds the naked border of a mesh.
  Parameters:
    polylines - [in] An array to which objects will be added.
    joinOverUnweldedVertices - [in]
      If true, unwelded vertices will stop the polyline.
  Returns:
    true if the computation finished successfully, false otherwise.
  */
  bool DuplicateBorder(
    ON_SimpleArray<ON_Polyline*>& polylines,
    bool joinOverUnweldedVertices) const;

  /*
  Description:
    Determines if meshes require an iterative cleanup based on a tolerance value.
    All available cleanup steps are used. Currently available cleanup steps are:

    - mending of single precision coincidence even though double precision vertices differ.
    - union of nearly identical vertices, irrespectively of their origin.
  Parameters:
    meshes - [in] A group of meshes that are worked on together. A single mesh is also OK.
    tolerance - [in] A 3d distance. This is usually a value of about 10e-7 magnitude.
  Returns:
    true if the step is required for mesh booleans, intersections, etc, false otherwise.
  */
  static bool RequireIterativeCleanup(
    const ON_SimpleArray<const ON_Mesh*>& meshes,
    double tolerance);

  /*
  Description:
    Performs the maximum currently-available iterative cleanup based on a tolerance value 
    on a group of meshes. This is an iterative process.
  Parameters:
    meshes - [in] A group of meshes that are worked on together. A single mesh is also OK.
    tolerance - [in] A 3d distance. This is usually a value of about 10e-7 magnitude.
    results - [out] A list that will be filled the exactly mesh.Count() items.
      If a mesh did not require cleanup, or cleanup did not yield any change,
      a nullptr is added to this list.
  Returns:
    true if the operation was successful, false otherwise. Also returning all nullptr
      is considered success.
  */
  static bool IterativeCleanup(
    const ON_SimpleArray<const ON_Mesh*>& meshes,
    double tolerance,
    ON_SimpleArray<ON_Mesh*>& results);

#endif

  // Override of virtual ON_Object::MemoryRelocate
  void MemoryRelocate() override;

  // virtual ON_Object::DestroyRuntimeCache override
  void DestroyRuntimeCache( bool bDelete = true ) override;

  void Destroy();
  void EmergencyDestroy(); // Call only when memory used by this class's
                           // members will soon become invalid for reasons 
                           // beyond your control. EmergencyDestroy() zeros
                           // anything that could possibly cause
                           // ~ON_Mesh() to crash.  Calling
                           // EmergencyDestroy() under normal conditions 
                           // will result in ~ON_Mesh() leaking
                           // memory.

#if defined(OPENNURBS_PLUS)
  /*
  Parameters:
    bCreateIfNoneExists - [in]
      If a mesh tree is not cached on this mesh and
      bCreateIfNoneExists is true, then a new one will be
      created and cached.
      If a mesh tree is not cached on this mesh and
      bCreateIfNoneExists is false, then null is returned.
  Returns:
    An pointer ON_MeshTree for this mesh. The pointer may
    be null and the caller must check it before dereferencing.
  */
  ON_DEPRECATED_MSG("Call MeshTree(true)")
  const class ON_MeshTree* MeshTree() const;

  const class ON_MeshTree* MeshTree( bool bCreateIfNoneExists  ) const;

  /*
  Parameters:
    bCreateIfNoneExists - [in]
      If a mesh face rtree is not cached on this mesh and
      bCreateIfNoneExists is true, then a new one will be
      created and cached.
      If a mesh face rtree is not cached on this mesh and
      bCreateIfNoneExists is false, then null is returned.
  Returns:
    An ON_Rtree with 3d node boxes and mesh face indices 
    as the element id values. The pointer may be null and
    the caller must check it before dereferencing.
  */
  const class ON_RTree* MeshFaceTree( bool bCreateIfNoneExists ) const;
#endif

  void DestroyTree( bool bDeleteTree = true );

  /*
  Description:
    Check for corrupt data values that are likely to cause crashes.
  Parameters:
    bRepair - [in]
      If true, const_cast<> will be used to change the corrupt data
      so that crashes are less likely.
    bSilentError - [in]
      If true, ON_ERROR will not be called when corruption is detected.
    text_log - [out]
      If text_log is not null, then a description of corruption 
      is printed using text_log.
  Remarks:
    Ideally, IsCorrupt() would be a virtual function on ON_Object,
    but doing that at this point would break the public SDK.
  */
  bool IsCorrupt(
    bool bRepair,
    bool bSilentError,
    class ON_TextLog* text_log
  ) const;

  /////////////////////////////////////////////////////////////////
  // ON_Object overrides

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  void Dump( ON_TextLog& ) const override; // for debugging

  bool Write( ON_BinaryArchive& ) const override;

  bool Read( ON_BinaryArchive& ) override;

  ON::object_type ObjectType() const override;

  /////////////////////////////////////////////////////////////////
  // ON_Geometry overrides

  int Dimension() const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  // virtual ON_Geometry GetTightBoundingBox override		
  bool GetTightBoundingBox( class ON_BoundingBox& tight_bbox, bool bGrowBox = false, const class ON_Xform* xform = nullptr ) const override;

	bool GetTightBoundingBox(
		ON_BoundingBox& tight_bbox,
		bool bGrowBox ,
		const ON_SimpleArray<ON_PlaneEquation>& clipping_planes,		
		const ON_Xform* xform = nullptr
		) const ;

  bool Transform( 
         const ON_Xform&
         ) override;

  // virtual ON_Geometry::IsDeformable() override
  bool IsDeformable() const override;

  // virtual ON_Geometry::MakeDeformable() override
  bool MakeDeformable() override;

  bool SwapCoordinates(
        int, int        // indices of coords to swap
        ) override;

#if defined(OPENNURBS_PLUS)
  // virtual ON_Geometry override
  bool Morph( const ON_SpaceMorph& morph ) override;

  // virtual ON_Geometry override
  bool IsMorphable() const override;
#endif

  // virtual ON_Geometry override
  bool EvaluatePoint( const class ON_ObjRef& objref, ON_3dPoint& P ) const override;


  /////////////////////////////////////////////////////////////////
  // Interface
  // 

  /*
  Returns
    true if there are zero vertices or zero faces.
  */
  bool IsEmpty() const;


  /*
  Returns
    true if there are vertices and faces.
  */

  bool IsNotEmpty() const;

  // creation
  bool SetVertex(
         int,              // vertex index
         const ON_3dPoint& // vertex location
         );
  bool SetVertex(
         int,              // vertex index
         const ON_3fPoint& // vertex location
         );
  bool SetVertexNormal(
         int,               // vertex index
         const ON_3dVector& // unit normal
         );
  bool SetVertexNormal(
         int,               // vertex index
         const ON_3fVector& // unit normal
         );
  bool SetTextureCoord(
         int,               // vertex index
         double, double     // texture coordinates
         );
  bool SetTriangle(
         int, // face index
         int,int,int // vertex indices
         );
  bool SetQuad(
         int, // face index
         int,int,int,int // vertex indices
         );

  /*
  Description:
    Use this function to append a duplicate of an existing vertex.
  Parameters:
    vertex_index - [in]
      index of the existing vertex
  Returns:
    If vertex_index is valid, the index of the duplicate is returned.
    Otherwise, ON_UNSET_UINT_INDEX is returned.
  Remarks:
    This function duplicates all information associated with
    the input vertex and is a good way to insure that
    optional vertex information like color, texture, surface
    parameters, curvatures, vertex normals, and so on get
    duplicated as well.
  */
  unsigned int AppendDuplicateVertex(
    unsigned int vertex_index
    );

  /*
  Description:
    Increases the capacity of arrays used to hold vertex information.
  Parameters:
    new_vertex_capacity - [in]
      desired capacity
  Returns:
    true if successful.
  Remarks:
    This function is useful if you are getting ready to add a known number
    of vertices and want to increase the dynamic array capacities before
    you begin adding vertices.
  */  
  bool ReserveVertexCapacity(
    size_t new_vertex_capacity
    );

  /*
  Parameters:
    ci - [in]
      component index to test
  Returns:
    True if ci identifies a component (vertex, edge, face, ngon) that exists in this mesh.
  */
  bool IsValidMeshComponentIndex(
    ON_COMPONENT_INDEX ci
    ) const;

  class ON_MeshComponentRef MeshComponentRef(
    ON_COMPONENT_INDEX ci
    ) const;

  /*
  Parameters:
   ci - [in] a component index with type mesh_vertex, meshtop_vertex,
             meshtop_edge, or mesh_face.
  Returns:
    A pointer to an ON_MeshComponentRef
    The caller must delete the returned object when it is no longer
    needed.
  */
  class ON_MeshComponentRef* MeshComponent( 
      ON_COMPONENT_INDEX ci
      ) const;

  /*
  Description:
    Delete the portions of the mesh identified in ci_list[].
  Parameters:
    ci_list - [in]
      List of components to delete.
    ci_list_count - [in]
      Number of elements in the ci_list[] array.
      Can be zero if you are using this function to remove
      unused vertices or empty ngons.
    bIgnoreInvalidComponents - [in]
      If true, invalid elements in ci_list[] are ignored.
      If false and ci_list[] contains an invalid element,
      then no changes are made and false is returned.
    bRemoveDegenerateFaces - [in]
      If true, remove degenerate faces.
    bCullUnusedVertices - [in]
      Remove vertices that are not referenced by a face.
      Pass true unless you have a good reason for keeping
      unreferenced vertices.
    bRemoveEmptyNgons - [in]
      Remove ngons that are empty.
      Pass true unless you have a good reason for keeping
      empty ngons.
  Returns:
    True: successful
    False: failure - no changes.
  */
  bool DeleteComponents(
    const ON_COMPONENT_INDEX* ci_list,
    size_t ci_count,
    bool bIgnoreInvalidComponents,
    bool bRemoveDegenerateFaces,
    bool bRemoveUnusedVertices,
    bool bRemoveEmptyNgons
    );

  /*
Description:
  Delete the portions of the mesh identified in ci_list[].
Parameters:
  ci_list - [in]
    List of components to delete.
  ci_list_count - [in]
    Number of elements in the ci_list[] array.
    Can be zero if you are using this function to remove
    unused vertices or empty ngons.
  bIgnoreInvalidComponents - [in]
    If true, invalid elements in ci_list[] are ignored.
    If false and ci_list[] contains an invalid element,
    then no changes are made and false is returned.
  bRemoveDegenerateFaces - [in]
    If true, remove degenerate faces.
  bCullUnusedVertices - [in]
    Remove vertices that are not referenced by a face.
    Pass true unless you have a good reason for keeping
    unreferenced vertices.
  bRemoveEmptyNgons - [in]
    Remove ngons that are empty.
    Pass true unless you have a good reason for keeping
    empty ngons.
  faceMap - [i]
    If anything other than nullptr is passed in, then
    faceMap[fi] is the index of the new face after the
    removal of vertices, faces, etc.
    This needs to be allocated to be at least m_F.Count() long.
Returns:
  True: successful
  False: failure - no changes.
*/
  bool DeleteComponents(
    const ON_COMPONENT_INDEX* ci_list,
    size_t ci_count,
    bool bIgnoreInvalidComponents,
    bool bRemoveDegenerateFaces,
    bool bRemoveUnusedVertices,
    bool bRemoveEmptyNgons,
    unsigned int* faceMap
  );

  /*
  Description:
    Calls the detailed version of DeleteComponents() with
    bool bIgnoreInvalidComponents = true;
    bool bRemoveDegenerateFaces = false;
    bool bRemoveUnusedVertices = true;
    bool bRemoveEmptyNgons = true;
  */
  bool DeleteComponents(
    const ON_COMPONENT_INDEX* ci_list,
    size_t ci_count
    ) override;

  /*
  Description:
    Calls the detailed version of DeleteComponents() with
    bool bIgnoreInvalidComponents = true;
    bool bRemoveDegenerateFaces = false;
    bool bRemoveUnusedVertices = true;
    bool bRemoveEmptyNgons = true;
  */
  bool DeleteComponents(
    const ON_SimpleArray<ON_COMPONENT_INDEX>& ci_list
    );

  /*
  Description:
    Calls the detailed version of DeleteComponents() with
    bool bIgnoreInvalidComponents = true;
    bool bRemoveDegenerateFaces = false;
    bool bRemoveUnusedVertices = true;
    bool bRemoveEmptyNgons = true;
  */
  bool DeleteComponent(
    ON_COMPONENT_INDEX ci
    );

  /*
  Description:
    Merge faces like ON_SubD::MergeFaces() does.
  Parameters:
    ci_list - [in]
      vertices, edges, and faces that trigger merging.
  Returns:
    If ngons were added, then the index of the first added ngon is returned.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int MergeFaceSets(
    const ON_SimpleArray<ON_COMPONENT_INDEX>& ci_list
  );

  /*
  Description:
    Merge faces like ON_SubD::MergeFaces() does.
  Parameters:
    ci_list - [in]
      vertices, edges, and faces that trigger merging.
  Returns:
    If ngons were added, then the index of the first added ngon is returned.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int DissolveOrDelete(
    const ON_SimpleArray<ON_COMPONENT_INDEX>& ci_list
  );


  /*
  Description:
    Copy the subset of the mesh identified in the component list.
   Parameters:
     ci_list - [in]
     ci_count - [in]
       ci_list[] is an array of ci_count components that identify the
       parts of the mesh to copy.  If a face or ngon is specified, then
       any vertices or faces needed for a valid copy are automatically
       copied as well.
     destination_mesh - [in]
       If null, a new mesh is allocated for the copy.
       If not null, the copy is put in this mesh.
  Return:
    null - invalid input - no copy created
    not null - a pointer to the copy.
  */
  ON_Mesh* CopyComponents(
    const ON_COMPONENT_INDEX* ci_list,
    size_t ci_count,
    class ON_Mesh* destination_mesh
    ) const;

  /*
  Description:
    Copy the subset of the mesh identified in the component list.
   Parameters:
     ci_list - [in]
       ci_list[] is an array of ci_count components that identify the
       parts of the mesh to copy.  If a face or ngon is specified, then
       any vertices or faces needed for a valid copy are automatically
       copied as well.
     destination_mesh - [in]
       If null, a new mesh is allocated for the copy.
       If not null, the copy is put in this mesh.
  Return:
    null - invalid input - no copy created
    not null - a pointer to the copy.
  */  
  ON_Mesh* CopyComponents(
    const ON_SimpleArray<ON_COMPONENT_INDEX>& ci_list,
    class ON_Mesh* destination_mesh
    ) const;

  // query
  int VertexCount() const;
  unsigned int VertexUnsignedCount() const;
  int FaceCount() const;
  unsigned int FaceUnsignedCount() const;
  int QuadCount() const; // number of faces that are quads
  int TriangleCount() const; // number of faces that are triangles
  int InvalidFaceCount() const; // number of face that have invalid m_vi[] values.
  bool HasVertexNormals() const; // normals at vertices
  bool HasFaceNormals() const;
  bool HasTextureCoordinates() const;
  bool HasSurfaceParameters() const;
  bool HasPrincipalCurvatures() const;

  /// <returns>
  /// If this mesh has per vertex colors set in the m_C[] array, then true is returned.
  /// Otherwise false is returned.
  ///</returns>
  bool HasVertexColors() const;

  /// <param name="color_tag"></param>
  /// <returns>
  /// If this mesh has per vertex colors set in the m_C[] array and
  /// color_tag = m_Ctag, then true is returned.
  /// Otherwise false is returned.
  /// </returns>
  bool HasVertexColors(
    ON_MappingTag color_tag
    ) const;

  void ClearVertexColors();

  /// <summary>
  /// 
  /// </summary>
  /// <param name="bLazy"></param>
  /// <param name="draft_angle_colors"></param>
  /// <returns></returns>
  bool SetDraftAngleColorAnalysisColors(
    bool bLazy,
    ON_SurfaceDraftAngleColorMapping draft_angle_colors
  );

  
  /// <summary>
  /// Set the mesh's per vertex colors in m_C[]
  /// from the surface principal curvatures in m_K[]
  /// </summary>
  /// <param name="bLazy">
  /// If bLazy is true, the mesh has curvatures and per vertex colors,
  /// and m_Ctag == kappa_colors.ColorMappingTag(), then m_C[] is assumed to
  /// be set correctly and no calculations are performed.
  /// </param>
  /// <param name="kappa_colors">
  /// m_C[i] = kappa_colors.Color(m_K[i])
  /// </param>
  /// <returns></returns>
  bool SetCurvatureColorAnalysisColors(
    bool bLazy,
    ON_SurfaceCurvatureColorMapping kappa_colors
  );

  /*
  Returns:
    True if the mesh has ngons.
  */
  bool HasNgons() const;

  /*
  Returns:
    Number of vertices that are hidden.
  */
  int HiddenVertexCount() const;

  bool GetCurvatureStats( 
         ON::curvature_style, 
         ON_MeshCurvatureStats& 
         ) const;

  void InvalidateVertexBoundingBox(); // Call if defining geometry is changed by 
                             // directly manipulating the m_V[] array.
  void InvalidateVertexNormalBoundingBox(); // Call if defining geometry is changed by 
                             // directly manipulating the m_N[] array.
  void InvalidateTextureCoordinateBoundingBox(); // Call if defining geometry is changed by 
                             // directly manipulating the m_T[] array.
  void InvalidateCurvatureStats(); // Call if defining geometry is changed by 
                             // directly manipulating the m_T[] array.
  void InvalidateBoundingBoxes(); // Invalidates all cached bounding box information.


  void Flip(); // reverses face orientations and flips vertex and face normals

  void FlipVertexNormals(); // reverses vertex normals
  void FlipFaceNormals(); // reverses face normals
  void FlipFaceOrientation(); // reverses face orientation (does nothing to normals)
  void FlipNgonOrientation(); // reverses ngon boundary direction

  void SetMeshParameters( const ON_MeshParameters& );
  const ON_MeshParameters* MeshParameters() const;
  void DeleteMeshParameters();

  
  bool UnitizeVertexNormals();
  bool UnitizeFaceNormals();
  bool CountQuads();

  /*
  Description:
    Splits all quads along the short diagonal.
  */
  bool ConvertQuadsToTriangles();

  /*
  Description:
    Splits non-planer quads into two triangles.

  Parameters:
    planar_tolerance - [in]
      If planar_tolerance >= 0, then a quad is split if its vertices
      are not coplaner.  

      If both planar_tolerance = ON_UNSET_VALUE and angle_tolerance_radians >= 0.0,
      then the planarity test is skipped.

      If both planar_tolerance = ON_UNSET_VALUE and angle_tolerance_radians = ON_UNSET_VALUE,
      then all quads are split.

  angle_tolerance_radians - [in]
      If angle_tolerance_radians >= 0.0, then a quad is split if the
      angle between opposite corner normals is > angle_tolerance_radians.
      The corner normal is the normal to the triangle  formed by two
      adjacent edges and the diagonal connecting their endpoints.
      A quad has for corner normals.

      If both angle_tolerance_radians = ON_UNSET_VALUE and planar_tolerance >= 0.0,
      then the corner normal angle test is skipped.

      If both planar_tolerance = ON_UNSET_VALUE and angle_tolerance_radians = ON_UNSET_VALUE,
      then all quads are split.

    split_method - [in]
      0 default 
         Currently divides along the short diagonal. This may be
         changed as better methods are found or preferences change.
         By passing zero, you let the developers of this code
         decide what's best for you over time.
      1 divide along the short diagonal
      2 divide along the long diagonal
      3 minimize resulting area
      4 maximize resulting area
      5 minimize angle between triangle normals
      6 maximize angle between triangle normals

  bDeleteNgonsContainingSplitQuads - [in]
    If true, ngons that contain a split quad are deleted.

  Returns:
    Number of quads that were converted to triangles.
  */
  unsigned int ConvertNonPlanarQuadsToTriangles(
    double planar_tolerance,
    double angle_tolerance_radians,
    unsigned int split_method
    );

  unsigned int ConvertNonPlanarQuadsToTriangles(
    double planar_tolerance,
    double angle_tolerance_radians,
    unsigned int split_method,
    bool bDeleteNgonsContainingSplitQuads
    );

  /*
  Description:
    Joins adjacent triangles into quads if the resulting quad
    is nice.
  Parameters:
    angle_tol_radians - [in] Used to compare adjacent
      triangles' face normals.  For two triangles to be considered,
      the angle between their face normals has to be <= angle_tol_radians.
      When in doubt use ON_PI/90.0 (2 degrees).
    min_diagonal_length_ratio - [in] ( <= 1.0) For two triangles to be
       considered the ratio of the resulting quad's diagonals
       (length of the shortest diagonal)/(length of longest diagonal).
       has to be >= min_diagonal_length_ratio.
       When in doubt us .875.
  */
  bool ConvertTrianglesToQuads(
    double angle_tol_radians,
    double min_diagonal_length_ratio
    );

  bool ComputeFaceNormals();   // compute face normals for all faces
  bool ComputeFaceNormal(int); // computes face normal of indexed face

  /*
  Description:
    Get a list of pairs of faces that clash.
  Parameters:
    max_pair_count - [in]
      If max_pair_count > 0, then at most this many pairs
      will be appended to the clashing_pairs[] array.
      If max_pair_count <= 0, then all clashing pairs
      will be appended to the clashing_pairs[] array.
    clashing_pairs - [out]
      The faces indices of clashing pairs are appended
      to this array. 
  Returns:
    Number of pairs appended to clashing_pairs[].
  */
  int GetClashingFacePairs( 
    int max_pair_count,
    ON_SimpleArray< ON_2dex >& clashing_pairs
    ) const;

  /*
  Description:
    Cull clashing faces from the mesh.
  Parameters:
    what_to_cull - [in]
      0: when a pair of faces clash, cull both faces
      1: when a pair of faces clash, leave the face with the
         longest edge.
      2: when a pair of faces clash, cull the face with the
         longest edge.
      3: when a pair of faces clash, leave the face with
         the largest area.
      4: when a pair of faces clash, cull the face with
         the largest area.
  Returns:
    Number of faces culled from the mesh.
  Remarks:
    If a large face clashes with many small faces, the large
    face and one small face will be removed.  When a degenerate
    face is encountered, it is also culled.
  */
  int CullClashingFaces( int what_to_cull );

  unsigned int CullDegenerateFaces(); // returns number of degenerate faces

  int CullUnusedVertices(); // returns number of culled vertices

  /*
  Description:
    Removes degenerate and unused mesh components.
  Returns:
    Number of removed components;
  */
 unsigned int CullDegenerates();

  // Description:
  //   Removes any unreferenced objects from arrays, reindexes as needed,
  //   and shrinks arrays to minimum required size.
  bool Compact();

  /*
  Description:
    Removes and unsets all possible cached information and 
    then calls Compact().
  Parameters:
    bRemoveNgons - [in]
      If true, all n-gon information is removed.
    bRemoveDegenerateFaces - [in]
      If true, CullDegenerateFaces() is used to remove degenerate faces.
    bCompact - [in]
      If true, Compact() is called after removing cached information.
  */
  void Cleanup(
    bool bRemoveNgons,
    bool bRemoveDegenerateFaces,
    bool bCompact
    );

  /*
  Description:
    Calls the latest version of the detailed cleanup command passing the value for bRemoveNgons
    and setting all other parameters to true.
  Parameters:
    bRemoveNgons - [in]
      If true, all n-gon information is removed.
  */
  void Cleanup(
    bool bRemoveNgons
    );

  bool ComputeVertexNormals();    // uses face normals to cook up a vertex normal

#if 0
  // Will be added April/may of 2024

#pragma region NOT_YET_RH_C_SHARED_ENUM [ON_Mesh::CurvatureSettingMethod] [Rhino.Geometry.MeshCurvatureSettingMethod] [byte]
  /// <summary>
  /// Enumberates the methods available to set the vertex principal curvature
  /// values saved in the ON_Mesh m_K[] array.
  /// </summary>
  enum class CurvatureSettingMethod : unsigned char
  {
    /// <summary>
    /// The most appropriate method for current context will be used.
    /// </summary>
    Unset = 0,

    /// <summary>
    /// The mesh is being created from a surface and the mesh's principal curvatures
    /// will be set from the surface's first and second partial deriatives.
    /// </summary>
    SurfaceEvaluation = 1,

    /// <summary>
    /// The mesh's principal curvatures will be set using 
    /// ON_SurfaceCurvature::CreateFromGaussianAndMeanCurvatures()
    /// with input values from
    /// ON_Mesh::ComputeDiscreteGaussianMeshCurvature() and
    /// ON_Mesh::ComputeDiscreteLaplacianMeanMeshCurvatures().
    /// </summary>
    DiscreteDefault = 2,

    /// <summary>
    /// The mesh's principal curvatures will be set using 
    /// ON_SurfaceCurvature::CreateFromGaussianAndMeanCurvatures()
    /// with input values from
    /// ON_Mesh::ComputeDiscreteGaussianMeshCurvature() and
    /// ON_Mesh::ComputeDiscreteLaplacianMeanMeshCurvatures().
    /// </summary>
    DiscreteLaplacianMean = 3,

    /// <summary>
    /// The mesh's principal curvatures will be set using 
    /// ON_SurfaceCurvature::CreateFromGaussianAndMeanCurvatures()
    /// with input values from
    /// ON_Mesh::ComputeDiscreteGaussianMeshCurvature() and
    /// ON_Mesh::ComputeDiscreteDihedralMeanMeshCurvature().
    /// </summary>
    DiscreteDihedralMean = 4
  };
#pragma endregion

  /// <summary>
  /// Use discrete curvature estimates to set the principal curvature values
  /// in the mesh's m_K[] array.
  /// </summary>
  /// <param name="method">
  /// Specifies the method to use.
  /// One of the discrete methods must be specified. 
  /// When in doubt, pass ON_Mesh::CurvatureSettingMethod::DiscreteDefault.
  /// </param>
  /// <param name="bLazy">
  /// If bLazy=true, only unset m_K[] values will be set.
  /// If bLazy=false, only unset m_K[] values will be set.
  /// </param>
  /// <returns></returns>
  bool ComputeVertexPrincipalCurvatures(
    ON_Mesh::CurvatureSettingMethod method,
    bool bLazy
  );

#if defined(OPENNURBS_PLUS)
  /* Description:
  Function that will apply the (pure, not area-weighted) cotangent-weighted Laplacian operator to a scalar field, specified
  at the topology vertices of the mesh.
  Parameters:
  mesh - [in] Input mesh
  scalarValues  - [in, out] the scalar values to be Laplaced on input, and the result on output.
  Their count must be the number of the mesh's topology vertices.
  Returns:
    True or false depending on the success of the operation.
  */

  /// <summary>
  ///  Function that will apply the (pure, not area-weighted) cotangent-weighted Laplacian operator 
  /// to a scalar field, specified at the topology vertices of the mesh.
  /// </summary>
  /// <param name="input_scalarValues">
  /// The input scalarValues[] must have count = MeshTopology().VertexCount()
  /// and be the scaler values to be Laplaced.
  /// You may pass the same array as input_scalarValues[] and input_scalarValues[].
  /// </param>
  /// <param name="laplaced_scalarValues">
  /// The laplaced scalar values are returned.
  /// You may pass the same array as input_scalarValues[] and input_scalarValues[].
  /// </param>
  /// <returns></returns>
  bool ComputeLaplacianToScalar(
    const ON_SimpleArray<double>& input_scalarValues,
    ON_SimpleArray<double>& laplaced_scalarValues
  ) const;

  /// <summary>
  /// Function that computes the discrete Gaussian curvatures, by taking the angle defectes and dividing them
  /// by the face areas.
  /// That method has the property that on closed meshes its integral gives ON_2PI*(Euler characteristic),
  /// Thus, it satisfies the Gauss - Bonnet theorem.
  /// Note that this function will triangulate internally along the short diagonal, but leave the mesh untouched.
  /// The function takes care of mesh topology and quads.
  /// </summary>
  /// <param name="gaussian_curvatures">
  /// The mean gaussian curvature estimates at each vertex are returned in gaussian_curvatures[].
  /// gaussian_curvatures.Length() = mesh.VertexCount().
  /// </param>
  /// <param name="globalAngleDefect">
  /// If not nullptr, then the returned value is the sum of all angle defects before dividing by the vertex areas.
  /// Pass nullptr if you do not want this output value.
  /// </param>
  /// <returns>
  /// True if the calculation was successful and gaussian_curvatures[] values are set.
  /// Otherwise false is returned and gaussian_curvatures.Count() will be zero.
  /// </returns>
  bool ComputeDiscreteGaussianCurvature(
    ON_SimpleArray<double>& gaussian_curvatures,
    double* globalAngleDefect
  ) const;

  /// <summary>
  /// Function that computes the discrete mean curvatures, by taking the dihedral angles of the topology 
  /// edges and dividing them by the edge lengths.
  /// The function takes care of mesh topology and quads by internal triangulation.
  /// If this mesh is nonmanifold, then the results for nonmanifold meshes are meaningless.
  /// </summary>
  /// <param name="mean_curvatures">
  /// The mean curvature estimates at each vertex are returned in mean_curvatures[].
  /// mean_curvatures.Length() = mesh.VertexCount().
  /// </param>
  /// <returns>
  /// True if the calculation was successful and mean_curvatures[] values are set.
  /// Otherwise false is returned and mean_curvatures.Count() will be zero.
  /// </returns>
  bool ComputeDiscreteDihedralMeanCurvature(
    ON_SimpleArray<double>& mean_curvatures
  ) const;

  /// <summary>
  /// Function that computes the discrete mean curvatures, by using the cotangent-weighted Laplacian
  /// of the coordianate functions.  The function takes care of mesh topology and quads by internal
  /// triangulation. The results for nonmanifold meshes are meaningless.
  /// </summary>
  /// <param name="mean_curvatures">
  /// The mean curvature estimates at each vertex are returned in mean_curvatures[].
  /// mean_curvatures.Length() = mesh.VertexCount().
  /// </param>
  /// <returns>
  /// True if the calculation was successful and mean_curvatures[] values are set.
  /// Otherwise false is returned and mean_curvatures.Count() will be zero.
  /// </returns>
  bool ComputeDiscreteLaplacianMeanCurvatures(
    ON_SimpleArray<double>& mean_curvatures
  ) const;
#endif
#endif

  //////////
  // Scales textures so the texture domains are [0,1] and
  // eliminates any texture rotations.
  bool NormalizeTextureCoordinates();

	/////////
	// Description:
	//		Transposes the texture coordinates
	//  Returns
	//			true  -  success
	bool TransposeTextureCoordinates();
	bool TransposeSurfaceParameters();
 
	/////////
	// Description:
	//		Reverse one coordinate direction of the texture coordinates, within texture domain m_tex_domain
	//	Parameters:
	//		dir  -[in]	-   dir=0  first texture coordinate is reversed
	//									  dir=1 second texture coordinate is reversed
	//  Returns
	//			true  -  success
	bool ReverseTextureCoordinates( int dir );
	bool ReverseSurfaceParameters( int dir );
 


  /*
  Description:
    Use a texture mapping function to set the m_T[] values.
  Parameters:
    mapping - [in]
    mesh_xform - [in]
      If not nullptr, the mapping calculation is performed as
      if the mesh were transformed by mesh_xform; the
      location of the mesh is not changed.
    bLazy - [in]
      If true and the m_T[] values were set using the same
      mapping parameters, then no calculation is performed.
  Returns:
    True if successful.
  See Also:
    ON_TextureMapping::GetTextureCoordinates
  */
  bool SetTextureCoordinates( 
          const class ON_TextureMapping& mapping,
          const class ON_Xform* mesh_xform = 0,
          bool bLazy = true
          );

  /*
  Description:
    Use a texture mapping function to set the m_T[] values.
  Parameters:
    mapping - [in]
    mesh_xform - [in]
      If not nullptr, the mapping calculation is performed as
      if the mesh were transformed by mesh_xform; the
      location of the mesh is not changed.
    bLazy - [in]
      If true and the m_T[] values were set using the same
      mapping parameters, then no calculation is performed.
    bSeamCheck - [in]
      If true then some mesh edges might be unwelded to better
      represent UV discontinuities in the texture mapping.
      This only happens for the following mappings:
      Box, Sphere, Cylinder.
  Returns:
    True if successful.
  See Also:
    ON_TextureMapping::GetTextureCoordinates
  */
  bool SetTextureCoordinatesEx(
    const class ON_TextureMapping& mapping,
    const class ON_Xform* mesh_xform = 0,
    bool bLazy = true,
    bool bSeamCheck = true
  );

  /*
  Description:
    Returns true if the mesh has at least one set of valid
    cached texture coordinates in the m_TC array.
  Returns:
    True if the mesh contains cached texture coordinates.
  See Also:
   ON_Mesh::SetCachedTextureCoordinatesFromMaterial
   ON_Mesh::CachedTextureCoordinates
   ON_Mesh::SetCachedTextureCoordinatesEx
  */
  bool HasCachedTextureCoordinates() const;
  
  /*
  Description:
    Prepares the cached texture coordinates by filling the
    m_TC array with the relevant texture coordinates needed
    to render the provided material using the provided
    mapping ref and ONX model.
   Call this function first if you are planning on storing
   the results from repeated calls to any of the following
   methods:
    * CachedTextureCoordinates
    * SetCachedTextureCoordinates
    * SetCachedTextureCoordinatesEx
    * GetCachedTextureCoordinates
  Parameters:
    onx_model - [in]
      The ONX model that contains the texture mappings.
    material - [in]
      The material which contains the textures we want to
      calculate the texture coordinates for.
    mapping_ref - [in]
      The texture mapping ref to use to get at the texture mapping.
  Returns:
    True if successful.
  See Also:
   ON_Mesh::CachedTextureCoordinates
   ON_Mesh::SetCachedTextureCoordinatesEx
  */
  bool SetCachedTextureCoordinatesFromMaterial(const class ONX_Model& onx_model, const ON_Material& material, const ON_MappingRef* mapping_ref) const;
  
  /*
  Description:
    Returns the cached texture coordinates corresponding to
    the provided texture if they exist. If they don't
    exist, they can be created using the following methods:
    * SetCachedTextureCoordinatesFromMaterial
    * SetCachedTextureCoordinates
    * SetCachedTextureCoordinatesEx

    NOTE: If you store the pointers of the results from this
    function, you need to first call
    SetCachedTextureCoordinatesFromMaterial to make sure the
    pointers don't get invalidated by subsequent calls to
    functions like SetCachedTextureCoordinates and
    SetCachedTextureCoordinatesEx.
  Parameters:
    onx_model - [in]
      The ONX model that contains the texture mappings.
     texture - [in]
       The texture to calculate the texture coordinates for.
     mapping_ref - [in]
       The texture mapping ref to use to get at the texture mapping.
  Returns:
    A pointer to the matching cached texture coordinates,
    nullptr if none exist.
  See Also:
    ON_Mesh::SetCachedTextureCoordinatesFromMaterial
    ON_Mesh::SetCachedTextureCoordinatesEx
  */
  const ON_TextureCoordinates* GetCachedTextureCoordinates(const class ONX_Model& onx_model, const ON_Texture& texture, const ON_MappingRef* mapping_ref) const;

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Prepares the cached texture coordinates by filling the
    m_TC array with the relevant texture coordinates needed
    to render the provided material using the provided
    mapping ref and Rhino document.
    Call this function first if you are planning on storing
    the results from repeated calls to any of the following
    methods:
     * CachedTextureCoordinates
     * SetCachedTextureCoordinates
     * SetCachedTextureCoordinatesEx
     * GetCachedTextureCoordinates
  Parameters:
    rhino_doc - [in]
      The Rhino document that contains the texture mapping table.
    material - [in]
      The material which contains the textures we want to
      calculate the texture coordinates for.
    mapping_ref - [in]
      The texture mapping ref to use to get at the texture mapping.
  Returns:
    True if successful.
  See Also:
    ON_Mesh::CachedTextureCoordinates
    ON_Mesh::SetCachedTextureCoordinatesEx
    ON_Mesh::GetCachedTextureCoordinates
  */
  bool SetCachedTextureCoordinatesFromMaterial(const class CRhinoDoc& rhino_doc, const ON_Material& material, const ON_MappingRef* mapping_ref) const;

  /*
 Description:
   Returns the cached texture coordinates corresponding to
   the provided texture if they exist. If they don't
   exist, they can be created using the following methods:
   * SetCachedTextureCoordinatesFromMaterial
   * SetCachedTextureCoordinates
   * SetCachedTextureCoordinatesEx

   NOTE: If you store the pointers of the results from this
   function, you need to first call
   SetCachedTextureCoordinatesFromMaterial to make sure the
   pointers don't get invalidated by subsequent calls to
   functions like SetCachedTextureCoordinates and
   SetCachedTextureCoordinatesEx.
 Parameters:
    rhino_doc - [in]
      The Rhino document that contains the texture mapping table.
    texture - [in]
      The texture to calculate the texture coordinates for.
    mapping_ref - [in]
      The texture mapping ref to use to get at the texture mapping.
 Returns:
   A pointer to the matching cached texture coordinates,
   nullptr if none exist.
 See Also:
   ON_Mesh::SetCachedTextureCoordinatesFromMaterial
   ON_Mesh::SetCachedTextureCoordinatesEx
 */
  const ON_TextureCoordinates* GetCachedTextureCoordinates(const class CRhinoDoc& rhino_doc, const ON_Texture& texture, const ON_MappingRef* mapping_ref) const;

#endif

  /*
  Description:
    Returns the cached texture coordinates corresponding to
    the provided mapping id if they exist. If they don't
    exist, they can be created using the following methods:
    * SetCachedTextureCoordinatesFromMaterial
    * SetCachedTextureCoordinates
    * SetCachedTextureCoordinatesEx
  
    NOTE: If you store the pointers of the results from this
    function, you need to first call
    SetCachedTextureCoordinatesFromMaterial to make sure the
    pointers don't get invalidated by subsequent calls to
    functions like SetCachedTextureCoordinates and
    SetCachedTextureCoordinatesEx.
  Parameters:
    mapping_id - [in]
      The texture mapping to use for the calculation.
  Returns:
    A pointer to the matching cached texture coordinates,
    nullptr if none exist.
  See Also:
    ON_Mesh::SetCachedTextureCoordinatesFromMaterial
    ON_Mesh::SetCachedTextureCoordinatesEx
  */
  const ON_TextureCoordinates* CachedTextureCoordinates( 
          const ON_UUID& mapping_id 
          ) const;

  // Use SetCachedTextureCoordinatesEx instead
  const ON_TextureCoordinates* SetCachedTextureCoordinates( 
          const class ON_TextureMapping& mapping,
          const class ON_Xform* mesh_xform = 0,
          bool bLazy = true
          );

  /*
  Description:
    Returns the cached texture coordinates corresponding to
    the provided mapping and other parameters. If they don't
    exist, this function will attempt to create them first.
    If they do exist and bLazy is true, then no calculation
    is performed.

    NOTE: Subsequent calls to this function with different
    parameters can invalidate previously returned
    ON_TextureCoordinates pointers. If you want store the
    pointers of previous results, you need to first call
    SetCachedTextureCoordinatesFromMaterial.
  Parameters:
    mapping - [in]
      The texture mapping to use for the calculation.
    mesh_xform  - [in]
      If not nullptr, the mapping calculation is performed as
      if the mesh were transformed by mesh_xform
    bLazy - [in]
      If true and the m_TC[] values were set using the same
      mapping parameters, then no calculation is performed.
    bSeamCheck - [in]
      If true then some mesh edges might be unwelded to better
      represent UV discontinuities in the texture mapping.
      This only happens for the following mappings:
      Box, Sphere, Cylinder.
  Returns:
    A pointer to the cached texture coordinates, nullptr if
    the function failed to calculate the texture coordinates.
  See Also:
    ON_Mesh::SetCachedTextureCoordinatesFromMaterial
    ON_Mesh::CachedTextureCoordinates
  */
  const ON_TextureCoordinates* SetCachedTextureCoordinatesEx(
          const class ON_TextureMapping& mapping,
          const class ON_Xform* mesh_xform = 0,
          bool bLazy = true,
          bool bSeamCheck = true
          );

  /*
  Description:
    Invalidates all cached texture coordinates. Call this
    function when you have made changes that will affect
    the texture coordinates on the mesh.
  Parameters:
    bOnlyInvalidateCachedSurfaceParameterMapping - [in]
      If true then only cached surface parameter mapping
      texture coordinates will be invalidated. Use this
      after making changes to the m_S array.
  */
  void InvalidateCachedTextureCoordinates(bool bOnlyInvalidateCachedSurfaceParameterMapping = false);

  bool EvaluateMeshGeometry( const ON_Surface& ); // evaluate surface at tcoords
                                                  // to set mesh geometry

  // finds all coincident vertices and merges them if break angle is small enough
  bool CombineCoincidentVertices( 
          ON_3fVector, // coordinate tols for considering vertices
                       // to be coincident
          double  // cosine normal angle tolerance in radians
                  // if vertices are coincident, then they are combined
                  // if NormalA o NormalB >= this value
          );

  /*
  Description:
    Combines identical vertices.
  Parameters:
    bIgnoreVertexNormals - [in] If true, then vertex normals
      are ignored when comparing vertices.
    bIgnoreTextureCoordinates - [in] If true, then vertex
      texture coordinates, colors, and principal curvatures
      are ignored when comparing vertices.
  Returns:
    True if the mesh is changed, in which case the returned
    mesh will have fewer vertices than the input mesh.
  */
  bool CombineIdenticalVertices(
          bool bIgnoreVertexNormals = false,
          bool bIgnoreTextureCoordinates = false
          );

  unsigned int RemoveAllCreases();

  void Append( const ON_Mesh& ); // appends a copy of mesh to this and updates
                                 // indices of appended mesh parts

  /*
  Description:
    Append a list of meshes. This function is much more efficient
    than making repeated calls to ON_Mesh::Append(const ON_Mesh&)
    when lots of meshes are being joined into a single large mesh.
  Parameters:
    count - [in]
      length of meshes[] array.
    meshes - [in]
      array of meshes to append.
  */
  void Append( int count, const ON_Mesh* const* meshes );

  /*
  Description:
    Append a vector of meshes. This function is much more efficient
    than making repeated calls to ON_Mesh::Append(const ON_Mesh&)
    when lots of meshes are being joined into a single large mesh.
  Parameters:
    meshes - [in]
      vector of meshes to append.
  */
  void Append(std::vector<std::shared_ptr<const ON_Mesh>>);
  
  /*
  Description:
    Expert user function to set m_is_closed member.  
    Setting this value correctly after a mesh is constructed 
    can save time when IsClosed() is called.
    This function sets the private member variable m_is_closed.
  Parameters:
    closed - [in]
      0: The mesh is not closed.  There is at least one face with an 
         edge that is geometrically distinct (as an unoriented line segment)
         from all other edges.
      1: The mesh is closed.  Every geometrically distinct edge is used
         by two or more faces.
  */
  void SetClosed(int closed);

  /*
  Returns:
    True if every mesh "edge" has two or more faces.
  */
  bool IsClosed() const;

  /*
  Returns:
    True if every mesh "edge" has at most two faces.
  */
  bool IsManifold() const;

  /*
  Returns:
    True if the mesh is manifold and every pair of faces
    that share an "edge" have compatible orientations.
  */
  bool IsOriented() const;

  /*
  Description:
    Determine if the mesh is a manifold.
  Parameters:
    bTopologicalTest - [in]
      If true, the query treats coincident vertices as
      the same.
    pbIsOriented - [out]
      If the input pointer is not nullptr, then the returned
      value of *pbIsOriented will be true if the mesh
      is a manifold and adjacent faces have compatible
      face normals.
    pbHasBoundary - [out]
      If the input pointer is not nullptr, then the returned
      value of *pbHasBoundary will be true if the mesh
      is a manifold and there is at least one "edge"
      with no adjacent faces have compatible
      face normals.
  Returns:
    True if every mesh "edge" has at most two adjacent faces.
  */
  bool IsManifold(
    bool bTopologicalTest,
    bool* pbIsOriented = nullptr,
    bool* pbHasBoundary = nullptr
    ) const;

  /*
  Description:
    Expert user function to set m_is_solid member.  
    Setting this value correctly after a mesh is constructed 
    can save time when IsSolid() is called.
    This function sets the private member variable m_is_solid.
    If solid is nonzero, it will set m_is_closed to 1.
  Parameters:
    solid - [in]
      0: The mesh is not an oriented manifold solid mesh. Either
         the mesh is not closed, not manifold, or the faces are
         not oriented compatibly.
      1: The mesh is an oriented manifold solid whose face normals
         point outwards.
     -1: The mesh is an oriented manifold solid whose face normals
         point inwards.
  */
  void SetSolidOrientation(int solid_orientation);

  /*
  Description:
    Determine orientation of a mesh.
  Returns:
    +1     mesh is a solid with outward facing normals
    -1     mesh is a solid with inward facing normals
     0     mesh is not a solid
  See Also:
    ON_Mesh::IsSolid
  */
  int SolidOrientation() const;

  /*
  Description:
    Test mesh to see if it is a solid.  (A "solid" is
    a closed oriented manifold.)
  Returns:
    true       mesh is a solid
    fals       mesh is not a solid
  See Also:
    ON_Mesh::SolidOrientation
    ON_Mesh::IsManifold
  */
  bool IsSolid() const;

  /*
  Description:
    Determine if a point is inside a solid brep.
  Parameters:
    test_point - [in]
    tolerance - [in] >= 0.0
      3d distance tolerance used for ray-mesh intersection
      and determining strict inclusion.
    bStrictlyInside - [in] 
      If bStrictlyInside is true, then test_point must be inside mesh
      by at least tolerance in order for this function to return
      true. If bStrictlyInside is false, then this function will return
      true if test_point is inside or the distance from test_point to
      a mesh face is <= tolerance.
  Returns:
    True if test_point is inside the solid mesh.
  Remarks:
    The caller is responsible for making certing the mesh is
    solid before calling this function. If the mesh is not
    solid, the behavior is unpredictable.
  See Also:
    ON_Mesh::IsSolid()
  */
  bool IsPointInside(
          ON_3dPoint test_point, 
          double tolerance,
          bool bStrictlyInside
          ) const;

  /*
  Description:
    Appends a list of mesh edges that begin or end at the specified
    vertices to the edges[] array.
  Parameters:
    vcount - [in]
      number of vertices
    vertex_index - [in]
      array of vertex indices
    bNoDuplicates - [in]
      If true, then only one edges[] is added for each edge,
      the first vertex index will always be less than the
      second, and the returned elements are sorted in dictionary
      order.
      If false and an edge is shared by multiple faces, then
      there will be an edges[] element added for each face and the
      order of the vertex indices will indicate the orientation
      of the edge with respect to the face.  No sorting is performed
      in this case.
    edges - [out]
      Edges that begin or end at one of the specified vertices are
      appended to this array.  Each ON_2dex records the start and
      end vertex index.
  Returns:
    Number of ON_2dex values appended to the edges[] array.
  */
  int GetVertexEdges( 
    int vcount,
    const int* vertex_index, 
    bool bNoDuplicates,
    ON_SimpleArray<ON_2dex>& edges
    ) const;


  /*
  Description:
    Appends a list of mesh edges to the edges[] array.
  Parameters:
    edges - [out]
      Each edges[] element is a pair of vertex indices.  There
      is at least one face in the mesh with an edge running between
      the indices.
  Returns:
    Number of ON_2dex values appended to the edges[] array.
  */
  int GetMeshEdges( 
    ON_SimpleArray<ON_2dex>& edges
    ) const;

  /*
  Description:
    Assign a unique id to each vertex location.  Coincident vertices
    get the same id.
  Parameters:
    first_vid - [in]
      Initial vertex id.  Typically 1 or 0.
    Vid - [out]
      If not null, then Vid[] should be an array of length VertexCount().
      and the vertex ids will be stored in this array.  If null,
      the array will be allocated by calling onmalloc().  The returned
      array Vid[i] is the id of the vertex m_V[i].  If m_V[i] and
      m_V[j] are the same 3d point, then Vid[i] and Vid[j] will have
      the same value.
    Vindex - [out] (can be null)
      If Vindex is not null, then it must have length at least m_V.Count()
      and the returned array will be a permutation of (0,1,...,m_V.Count()-1)
      such (Vid[Vindex[0]], Vid[Vindex[1]], ..., Vid[Vindex[m_V.Count()-1]])
      is an increasing list of value.
  Returns:
    null if the mesh has no vertices.
    An array of length VertexCount(). If vertices m_V[i] and m_V[j]
    are coincident, then Vid[i] = Vid[j].  The id values begin at first_vid.
    The maximum vertex id is Vid[Vindex[m_V.Count()-1]].  The number of
    unique vertex locations is (Vid[Vindex[m_V.Count()-1]] - first_vid + 1).
  */
  unsigned int* GetVertexLocationIds( 
    unsigned int first_vid,
    unsigned int* Vid,
    unsigned int* Vindex
    ) const;

  /*
  Description:
    Get a list of the sides of every face.
  Parameters:
    Vid - [in] (can be null)
      If Vid is null, then the mesh m_V[] index values are used to set
      the ON_MeshFaceSide::vi[] values.
      If Vid is not null, then it must be an array of length VertexCount().
      The value Vid[mesh m_V[] index] will be used to set the
      ON_MeshFaceSide::vi[] values.
    sides - [out]
      If the input value of sides is not null, then sides[] must be long 
      enough to hold the returned side list.  The maximum possible length
      is 4*FaceCount() for a mesh contining FaceCount() nondegenerate quads.
      If the input value of sides is null, memory will be allocated using
      onmalloc() and the caller is responsible for calling onfree() at an
      appropriate time.  This function fills in the sides[] array
      with face side information.  The returned list is sorted by sides[].fi
      and the sides[].side and each element has vi[0] <= vi[1].  
      The function ON_SortMeshFaceSidesByVertexIndex() can be used to sort the 
      list by the sides[].vi[] values.
  Returns:
    Number of elements added to sides[].
  Remarks:
    Faces with out of range ON_MeshFace.vi[] values are skipped. 
    Degenerate faces are processed, but degenerate sides (equal vertex indices)
    are not added to the list.
  */
  unsigned int GetMeshFaceSideList( 
      const unsigned int* Vid,
      class ON_MeshFaceSide*& sides
      ) const;

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Get a list of the geometrically unique edges in a mesh.
  Parameters:
    edge_list - [out]
      The edge list for this mesh is appended to edge_list[].  
      The ON_2dex i and j values are mesh->m_V[] array indices.
      There is exactly one element in edge_list[] for each
      unoriented 3d line segment in the mesh. The edges are 
      oriented the same way the corresponding ON_MeshTopology
      edge is oriented.
    bLookForNgonInteriorEdges - [in]
      true if ngon interior edges should be separated from manifold edges.
    bOmitHiddenEdges - [in]
      true if edges that have at least one hidden vertex should be omitted from
      the returned lists.
    ci_meshtop_edge_map - [out]
      If you call the version of GetMeshEdgeList() with the ci_meshtop_edge_map[],
      parameter, then the edge in edge_list[i] corresponds to the edge
      in ON_MeshTopology.m_tope[ci_meshtop_edge_map[i]]. The value
      ci_meshtop_edge_map[i] is useful if you need to convert an edge_list[]
      index into an ON_COMPONENT_INDEX with type meshtop_edge.
    ci_meshtop_vertex_map - [out]
      If you call the version of GetMeshEdgeList() with the ci_meshtop_vertex_map[],
      parameter, then the vertex m_V[i] corresponds to the vertex
      in ON_MeshTopology.m_topv[ci_meshtop_vertex_map[i]]. The value
      ci_meshtop_vertex_map[i] is useful if you need to convert an m_V[]
      index into an ON_COMPONENT_INDEX with type meshtop_vertex.
    edge_list_partition - [out] (can be null)
      The edge_list[] is always ordered so that edge_types
      are partitioned into contiguous regions. The edge_list_partition[5]
      values report the edge type regions.
      * If edge_type_partition[0] <= ei < edge_type_partition[1], then
        edge_list[ei] is an edge of exactly two faces and the vertices
        used by the faces are identical.  These are also called
        "manifold edges".
      * If edge_type_partition[1] <= ei < edge_type_partition[2], then
        edge_list[ei] is an edge of exactly two faces, but at least
        one of the vertices is duplicated.  These are also called
        "crease edges".
      * If edge_type_partition[2] <= ei < edge_type_partition[3], then
        edge_list[ei] is an edge of 3 or more faces. These are also called
        "nonmanifold edges".
      * If edge_type_partition[3] <= ei < edge_type_partition[4], 
        then edge_list[ei] is a boundary edge of exactly one mesh face.
        These are also called "naked edges".
      * If edge_type_partition[4] <= ei < edge_type_partition[5], 
        then edge_list[ei] is an edge of exactly two faces that
        belong to the same ngon.
        These are also called "ngon interior edges".
    faceSidesList - [out] (can be null) fills the array with sides,
      ordered by vertex index. This can be thought as a reverse face-vertex
      map. This is required by any internal calculations.
  Returns:
    Number of edges added to edge_list[].
  Remarks:
    This calculation also sets m_closed.  If you modify the mesh's
    m_V or m_F information after calling this function, be sure to
    clear m_is_closed.
  */
  unsigned int GetMeshEdgeList( 
      ON_SimpleArray<ON_2dex>& edge_list, 
      bool bLookForNgonInteriorEdges,
      unsigned int edge_type_partition[6] 
      ) const;

  unsigned int GetMeshEdgeList( 
      ON_SimpleArray<ON_2dex>& edge_list, 
      bool bLookForNgonInteriorEdges,
      ON_SimpleArray<int>& ci_meshtop_edge_map,
      unsigned int edge_type_partition[6] 
      ) const;

  unsigned int GetMeshEdgeList( 
      ON_SimpleArray<ON_2dex>& edge_list, 
      bool bLookForNgonInteriorEdges,
      ON_SimpleArray<int>& ci_meshtop_edge_map,
      ON_SimpleArray<int>& ci_meshtop_vertex_map,
      unsigned int edge_type_partition[6] 
      ) const;

  unsigned int GetMeshEdgeList( 
      ON_SimpleArray<ON_2dex>& edge_list, 
      bool bLookForNgonInteriorEdges,
      bool bOmitHiddenEdges,
      unsigned int edge_type_partition[6] 
      ) const;

  unsigned int GetMeshEdgeList( 
      ON_SimpleArray<ON_2dex>& edge_list, 
      bool bLookForNgonInteriorEdges,
      bool bOmitHiddenEdges,
      ON_SimpleArray<int>& ci_meshtop_edge_map,
      unsigned int edge_type_partition[6] 
      ) const;

  unsigned int GetMeshEdgeList( 
      ON_SimpleArray<ON_2dex>& edge_list, 
      bool bLookForNgonInteriorEdges,
      bool bOmitHiddenEdges,
      ON_SimpleArray<int>& ci_meshtop_edge_map,
      ON_SimpleArray<int>& ci_meshtop_vertex_map,
      unsigned int edge_type_partition[6] 
      ) const;

  unsigned int GetMeshNakedEdgeList(
    ON_SimpleArray<ON_2dex>& naked_edge_list,
    ON_SimpleArray<int>* ci_meshtop_vertex_map = nullptr,
    bool bLookForNgonInteriorEdges = false
  ) const;

  unsigned int GetMeshNakedEdgeListEx(
    ON_SimpleArray<ON_2dex>& naked_edge_list,
    ON_SimpleArray<int>* ci_meshtop_vertex_map = nullptr,
    bool bLookForNgonInteriorEdges = false,
    ON_SimpleArray<ON_MeshFaceSide>* faceSides = nullptr
  ) const;

  /*
  Description:
    Get the point on the mesh that is closest to P.
  Parameters:
    P - [in] test point
    Q - [out] point on the mesh
    maximum_distance = 0.0 - [in] 
            optional upper bound on the distance 
            from P to the mesh.  If you are only 
            interested in finding a point Q on the 
            mesh when P.DistanceTo(Q) < maximum_distance, 
            then set maximum_distance to that value.
  Returns:
    True if successful.  If false, the value of Q
    is undefined.
  */
  bool GetClosestPoint(
          const ON_3dPoint& P,
          class ON_MESH_POINT* Q,
          double maximum_distance = 0.0
          ) const;

  /*
  Description:
    Quickly intersect this mesh with meshB.  Ignore overlaps
    and near misses.
  Parameters:
    meshB - [in]
    lines - [out] Intersection lines are appended to
                  this list.
  Returns:
    number of lines appended to lines[] array.
  Remarks:
    The InstersectMesh function will will create a meshtree, a mesh topology 
    and face normals of this mesh and meshB. Note: if you create these in 
    multiple memory pools you run the risk of crashing or leaking memory if
    you are not careful.
  */
  int IntersectMesh( 
          const ON_Mesh& meshB,
          ON_SimpleArray<ON_Line>& lines
          ) const;

  /*
  Description:
    Slowly intersect this mesh with meshB. This method uses the old code.
  Parameters:
    meshB - [in]
    x - [out] Each element of x is a polyline of ON_MMX_POINTs.
    intersection_tolerance - [in]
    overlap_tolerance - [in]
  Returns:
    number of polylines appended to x[] array.
  Remarks:
    The IntersectMesh function will will create a meshtree, a mesh topology
    and face normals of this mesh and meshB. Note: if you create these in 
    multiple memory pools you run the risk of crashing or leaking memory if
    you are not careful.
  */
  int IntersectMesh( 
          const ON_Mesh& meshB,
          ON_ClassArray< ON_SimpleArray< ON_MMX_POINT > >& x, 
          double intersection_tolerance = 0.0,
          double overlap_tolerance = 0.0
          ) const;


  /*
  Description:
    Intersect this mesh with an infinite plane.
    This uses the new codebase as of Rhino 7.16.
  Parameters:
    plane_equation - [in]
    tolerance - [in]- see remarks at GetIntersections() function.
                Use a suitably small value.
    perforations - [out] intersection polylines are appended if not nullptr.
    overlaps - [out] intersection polylines are appended if not nullptr.
    cache_for_this_mesh - [in-out]
                  if a constructed instance of a cache is passed
                  then it can be used in subsequent calls.
  Returns:
    number of lines appended to perforations[] and overlaps[] arrays.
  */
  int IntersectPlane( 
          ON_PlaneEquation plane_equation,
          double tolerance,
          ON_SimpleArray<ON_Polyline*>* perforations,
          ON_SimpleArray<ON_Polyline*>* overlaps,
          ON_MeshIntersectionCache* cache = nullptr
          ) const;

  /*
  Description:
    Intersect this mesh with an infinite plane.
    This uses the new codebase as of Rhino 7.16.
  Parameters:
    plane_equation - [in]
    lines - [out] Intersection lines are appended to
                  this list.
  Returns:
    number of lines appended to lines[] array.
  */
  int IntersectPlane( 
          ON_PlaneEquation plane_equation,
          ON_SimpleArray<ON_Line>& lines
          ) const;

  /*
  Description:
    Compute area of the mesh.
  Parameters:
    error_estimate - [out]  if not nullptr, an upper bound on the error
        in the area calculation is returned.
  Example:

            ON_Mesh mesh = ...;
            double area, error_estimate;
            area = mesh.Area(&error_estimate);
            printf("mesh area = %g (+/- %g)\n",area,error_estimate);

  Returns:
    Area of the mesh.
  */
  double Area( double* error_estimate = nullptr ) const;

  /*
  Description:
    Compute area centroid of the mesh.
  Parameters:
    area - [out] it not nullptr, area of the mesh
  Returns:
    Location of area centroid.
  See Also:
    ON_Mesh::AreaMassProperties
  */
  ON_3dPoint AreaCentroid( 
          double* area = nullptr
          ) const;

  /*
  Description:
    Calculate area mass properties of the mesh.
  Parameters:
    mp - [out] 
    bArea - [in] true to calculate area
    bFirstMoments - [in] true to calculate area first moments,
                         area and area centroid.
    bSecondMoments - [in] true to calculate area second moments.
    bProductMoments - [in] true to calculate area product moments.
  Returns:
    True if successful.
  */
  bool AreaMassProperties(
    class ON_MassProperties& mp,
    bool bArea = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true
    ) const;


  /*
  Description:
    Compute volume of the mesh.
  Parameters:
    base_point - [in] optional base point When computing the volume of
      solid defined by several meshes, pass the same base_point to each call
      to volume.  When computing the volume of a solid defined by a single
      mesh, the center of the bounding box is a good choice for base_point.
    error_estimate - [out]  if not nullptr, an upper bound on the error
        in the volume calculation is returned.
  Returns:
    volume of the mesh.
  Example:

            // Assume a solid is enclosed by 3 meshes, mesh1, mesh2, and mesh3.
            // The volume of the solid can be computed as follows.
            ON_Mesh mesh1=..., mesh2=..., mesh3=...;
            // use the center of the solid's bounding box as a common base point.
            ON_BoundingBox bbox = mesh1.BoundingBox();
            mesh2.GetBoundingBox(bbox,true);
            mesh3.GetBoundingBox(bbox,true);
            ON_3dPoint base_point = bbox.Center()
            double vol1_err, vol2_err, vol3_err;
            double vol1 = mesh1.Volume(base_point,&vol1_err);
            double vol2 = mesh2.Volume(base_point,&vol2_err);
            double vol3 = mesh3.Volume(base_point,&vol3_err);
            double volume = vol1 + vol2 + vol3;
            double error_estimate = vol1_err + vol2_err + vol3_err;
            printf("mesh volume = %g (+/- %g)\n",volume,error_estimate);
  */
  double Volume( 
              ON_3dPoint base_point = ON_3dPoint::Origin, 
              double* error_estimate = nullptr 
              ) const;


  /*
  Description:
    Compute volume centroid of the mesh.
  Parameters:
    base_point - [in] When computing the centroid of a solid
      volume defined by several meshes, pass the same base_point 
      to each call to GetVolumeCentroid() and add the answers.
      When computing the centroid of a solid defined by a single
      mesh, the center of the bounding box is a good choice for base_point.
    volume - [out] it not nullptr, Volume of the mesh
  Returns:
    Location of the volume centroid.
  */
  ON_3dPoint VolumeCentroid( 
          ON_3dPoint base_point = ON_3dPoint::Origin,
          double* volume = nullptr
          ) const;

  /*
  Description:
    Calculate volume mass properties of the mesh.
  Parameters:
    base_point - [in] When computing the volume mass properties
      of a solid volume defined by several meshes, pass the same
      base_point to each call to VolumeMassProperties() and add 
      the answers.  When computing the volume mass properties of
      a solid defined by a single mesh, the center of the 
      bounding box is a good choice for base_point. If the mesh
      is closed, you can pass ON_3dPoint::UnsetPoint and the
      center of the bounding box will be used.
    mp - [out] 
    bVolume - [in] true to calculate volume
    bFirstMoments - [in] true to calculate volume first moments,
                         volume, and volume centroid.
    bSecondMoments - [in] true to calculate volume second moments.
    bProductMoments - [in] true to calculate volume product moments.
    base_point - [in] 
      If the surface is closed, then pass ON_UNSET_VALUE.

      This parameter is for expert users who are computing a
      volume whose boundary is defined by several non-closed
      breps, surfaces, and meshes.

      When computing the volume, volume centroid, or volume
      first moments of a volume whose boundary is defined by 
      several breps, surfaces, and meshes, pass the same 
      base_point to each call to VolumeMassProperties.  

      When computing the volume second moments or volume product
      moments of a volume whose boundary is defined by several
      breps, surfaces, and meshes, you MUST pass the entire 
      volume's centroid as the base_point and the input mp 
      parameter must contain the results of a previous call
      to VolumeMassProperties(mp,true,true,false,false,base_point).
      In particular, in this case, you need to make two sets of
      calls; use first set to calculate the volume centroid and
      the second set calculate the second moments and product 
      moments.
  Returns:
    True if successful.
  */
  bool VolumeMassProperties(
    class ON_MassProperties& mp, 
    bool bVolume = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    ON_3dPoint base_point = ON_3dPoint::UnsetPoint
    ) const;

  bool GetSilhouette(
    const ON_SilhouetteParameters parameters,
    const ON_PlaneEquation* clipping_planes,
    size_t clipping_plane_count,
    ON_ClassArray<ON_SIL_EVENT>& silhouettes,
    ON_ProgressReporter* progress,
    ON_Terminator* terminator
    ) const;


#endif

  ///////////////////////////////////////////////////////////////////////
  //
  // mesh editing
  //

  /*
  Description:
    Replace a mesh edge with a vertex at its center and update
    adjacent faces as needed.
  Parameters:
    topei - [in] index of edge in MeshTopology().m_tope[] array
  Returns:
    true if successful.
  */
  bool CollapseEdge( int topei );

  /*
  Description:
    Tests a mesh edge to see if it is valid as input to
    ON_Mesh::SwapMeshEdge.
  Parameters:
    topei - [in] index of edge in MeshTopology().m_tope[] array
  Returns:
    true if edge can be swapped by ON_Mesh::SwapMeshEdge.
  See Also:
    ON_Mesh::SwapEdge
  */
  bool IsSwappableEdge( int topei );


  /*
  Description:
    If the edge is shared by two triangular face, then
    the edge is "swapped".
  Parameters:
    topei - [in] index of edge in MeshTopology().m_tope[] array
  Returns:
    true if successful
  See Also:
    ON_Mesh::IsSwappableEdge
  */
  bool SwapEdge( int topei );

  /*
  Description:
    Removes a face from a mesh and does not alter the
    geometry of the remaining mesh.
  Parameters:
    meshfi - [in] index of face in ON_Mesh.m_F[] array
  Remarks:
    This function calls DestroyTopology() and DestroyPartition().
    The caller is responsible for calling Compact() if that step
    is required.
  Returns:
    true if successful
  */
  bool DeleteFace( int meshfi );

  /*
  Description:
    Destroys the m_H[] array and sets m_hidden_count=0.
  */
  void DestroyHiddenVertexArray();

  /*
  Returns:
    If the mesh has some hidden vertices, then an array
    of length VertexCount() is returned and the i-th
    element is true if the i-th vertex is hidden.
    If no vertices are hidden, nullptr is returned.
  */
  const bool* HiddenVertexArray() const;

  /*
  Description:
    Set the runtime vertex hidden flag.
  Parameters:
    meshvi - [in] mesh vertex index
    bHidden - [in] true to hide vertex
  */
  void SetVertexHiddenFlag( int meshvi, bool bHidden );

  /*
  Description:
    Returns true if the mesh vertex is hidden.  This is a runtime
    setting that is not saved in 3dm files.
  Parameters:
    meshvi - [in] mesh vertex index.
  Returns:
    True if mesh vertex is hidden.
  */
  bool VertexIsHidden( int meshvi ) const;

  /*
  Description:
    Returns true if the mesh face is hidden.  This is a runtime
    setting that is not saved in 3dm files.
  Parameters:
    meshfi - [in] mesh face index.
  Returns:
    True if mesh face is hidden.
  Remarks:
    A face is hidden if, and only if, at least one of its
    vertices is hidden.
  */
  bool FaceIsHidden( int meshvi ) const;


  ///////////////////////////////////////////////////////////////////////
  //
  // mesh topology
  //
  // In order to keep the mesh facet definition simple and make the mesh
  // definition easily used in common rendering application, if two facets
  // share a vertex location but have different normals, curvatures, 
  // textures, etc., at that common vertex location, then the vertex is
  // duplicated.  When the topology of the mesh needs to be known,
  // use Topology() to get a class that provides complete topological
  // information about the mesh.
  const ON_MeshTopology& Topology() const;

  ///////////////////////////////////////////////////////////////////////
  // If you modify the mesh in any way that may change its topology,
  // then call DestroyTopology().  Specifically if you add or remove
  // vertices or face, change vertex locations, or change the face m_vi[]
  // values, then you must call DestroyTopology().
  void DestroyTopology();

  /*
  Returns:
    This is an expert user function that returns true if the topology
    information is already calculated and cached.  It can be used to
    to avoid calling the Topology() function when the expensive creation
    step will be performed.
  */
  /* obsolete - used HasMeshTopology() */ bool TopologyExists() const;
  bool HasMeshTopology() const;

  ///////////////////////////////////////////////////////////////////////
  //
  // mesh partitions
  //
  // In ancient times, some rendering engines were only able to process
  // small batches of triangles and th CreatePartition() function was
  // provided to partition the mesh into subsets of vertices and faces
  // that those renering engines could handle.
  //
  const ON_MeshPartition* CreatePartition( 
                int, // maximum number of vertices in a partition
                int  // maximum number of triangles in a partition
                );
  const ON_MeshPartition* Partition() const;
  void DestroyPartition();

  /*
  Description:
    Extract the portion of this mesh defined by mesh_part.
  Parameters:
    mesh_part - [in]
      defines portion of the mesh to extract.
    mesh - [in] (can be null, cannot be = "this).
      If mesh is no null, the extracted mesh will be put into
      this mesh.  If mesh is null, the extracted mesh will
      be created in a mesh allocated on the heap using the
      new operator.
  Returns:
    A pointer to the submesh.  If the input mesh parameter is null,
    then the caller must delete this mesh when it is no longer needed.
    If the input is invalid, then null is returned.
  */
  ON_Mesh* MeshPart( 
    const ON_MeshPart& mesh_part,
    ON_Mesh* mesh 
    ) const;

  /*
  Description:
    Create a mesh that is a single face of this mesh.
  Parameters:
  Returns:
    A pointer to the submesh.  If the input mesh parameter is null,
    then the caller must delete this mesh when it is no longer needed.
    If the input is invalid, then null is returned.
  */
  ON_Mesh* DuplicateFace( 
    int face_index,
    ON_Mesh* mesh 
    ) const;

  ///////////////////////////////////////////////////////////////////////
  //
  // mesh N-gon lists.  
  //   ON_Mesh objects support faces that are triangle or quads.
  //   When a mesh is created from a format that supports N-gons
  //   for N larger than 4, an optional N-gon list can be added 
  //   that specifies the vertices and faces that make up the N-gon.
  //

  /*
  Description:
    If the mesh has an N-gon list, return a pointer to it.
  Returns:
    A pointer to the current N-gon list or nullptr.
  */
  const class ON_V4V5_MeshNgonList* V4V5_NgonList() const;

  /*
  Description:
    If an N-gon list exists, it is returned and can be modified.
    If no N-gon list exists, a new empty list is returned and
    it can be modified.
  Returns:
    A pointer to the N-gon list that can be modified.
  */
  class ON_V4V5_MeshNgonList* V4V5_ModifyNgonList();

  /*
  Description:
    Destroy any existing N-gon list.
  */
  void V4V5_DestroyNgonList();

  ///////////////////////////////////////////////////////////////////////
  //
  // mesh components
  //   ON_Mesh objects can consist of sets of faces that are isolated
  //   from any other sets of faces.  The following 2 functions will
  //   dissect a mesh into these sets, called components.  Not to be 
  //   confused with ON_COMPONENT_INDEX.

  /*
    Description:
      Calculates the components of a mesh and sets a label for each face in
      the facet_component_labels array.
    Parameters:
      bUseVertexConnections- [in]
        If this parameter is true, then facets that share a common vertex
        are considered connected.
        If this parameter is false, then facets must share an edge to
        be considered connected.
      bUseTopologicalConnections - [in]
        If this parameter is true, then geometric location is used
        to determine if facets are connected. 
        If this parameter is false, then facets must share the same vertex 
        or vertices to be considered connected.
      facet_component_labels- [out]
        facet_component_labels[] will be an array with the same size
        as ON_Mesh.m_F.Count() and facet_component_labels[i]
        is the component id m_F[i] belongs to.  The component id
        will be 1 to the number of components.
    Returns:
      Number of components on success, 0 on failure 
  */

  int GetConnectedComponents( bool bUseVertexConnections, 
                              bool bTopologicalConnections, 
                              ON_SimpleArray<int>& facet_component_labels
                            ) const;

  /*
    Description:
      Calculates the components of a mesh and sets a label for each face in
      the facet_component_labels array.
    Parameters:
      bUseVertexConnections- [in]
        If this parameter is true, then facets that share a common vertex
        are considered connected.
        If this parameter is false, then facets must share an edge to
        be considered connected.
      bUseTopologicalConnections - [in]
        If this parameter is true, then geometric location is used
        to determine if facets are connected. 
        If this parameter is false, then facets must share the same vertex 
        or vertices to be considered connected.
      components   - [out]
        New components are appended to this array
        if this parameter is null, then the components are just counted.
    Returns:
      Number of components on success, 0 on failure 
  */

  int GetConnectedComponents( bool bUseVertexConnections, 
                              bool bTopologicalConnections, 
                              ON_SimpleArray<ON_Mesh*>* components
                            ) const;

  /////////////////////////////////////////////////////////////////
  // 
  // Mesh offset
  // 

  /*
    Description:
      Offsets a mesh by the input distance
      
      distance - [in]
        Distance to offset
      direction - [in]
        If this parameter is ON_3dVector::UnsetVector, offset each vertex in the normal direction
        otherwise, offset every vertex in the input direction
    Returns:
      New mesh that is an offset of a duplicate of this mesh
      Or nullptr if the input was invalid or the mesh could not be duplicated or offset
      Caller manages memory of new mesh
  */
  ON_Mesh* OffsetMesh(const double distance, const ON_3dVector& direction) const;


  /////////////////////////////////////////////////////////////////
  // 
  // Double precision vertex support
  // 

  /*
  Returns:
    True if the mesh vertex count is > 0, the mesh has single and double 
    precision vertices, and the values of the locations are synchronized.
  */
  bool HasSynchronizedDoubleAndSinglePrecisionVertices() const;

  /*
  Returns:
    True if the mesh has double precision vertices (m_dV.Count() > 0).
  Remarks:
    Use ON_Mesh::UpdateDoublePrecisionVertices()
    or ON_Mesh::UpdateSinglePrecisionVertices() to synchronize
    values of single and double precision vertices.
  */
  bool HasDoublePrecisionVertices() const;

   bool HasSinglePrecisionVertices() const;


  /*
  Description:
    If you modify the values of double precision vertices,
    then you must call UpdateSinglePrecisionVertices().
  Remarks:
    If double precision vertices are not present, this function
    does nothing.
  */
  void UpdateSinglePrecisionVertices();

  /*
  Description:
    If you modify the values of the single precision vertices
    in m_V[], then you must call UpdateDoublePrecisionVertices().
  Remarks:
    If double precision vertices are not present, this function
    creates them.
  */
  void UpdateDoublePrecisionVertices();

  /*
  Description:
    The function removes all double precision vertex information.
  */
  void DestroyDoublePrecisionVertices();


  /////////////////////////////////////////////////////////////////
  // Implementation - mesh geometry

  // Vertex locations
  //   In a case where adjacent facets share a vertex
  //   location but have distinct normals or texture
  //   coordinates at that location, the vertex must
  //   be duplicated.

  /*
  Description:
    Get double precision vertices.  If they do not exist,
    they will be created and match the existing single
    precision vertices.
  Returns:
    Array of double precision vertices.  If you modify the
    values in this array, you must make the same modifications
    to the single precision vertices, or call 
    UpdateSinglePrecisionVertices().
  Example:

          // add a bunch of double precision information
          ON_3dPointArray& dv = mesh.DoublePrecisionVertices();
          for ( i = 0; i < lots; i++ )
          {
            dv[i] = ...
          }
          // This call updates the single precision values
          // in m_V[] and sets all the counts and CRCs that
          // are used in validity checking.
          mesh.UpdateSinglePrecisionVertices();
    
  Remarks:
    Avoid multiple calls to DoublePrecisionVertices().
    It is most efficient to make one call, save a local 
    reference, and use the local reference as needed.
  */
  ON_3dPointArray& DoublePrecisionVertices();
  const ON_3dPointArray& DoublePrecisionVertices() const;

  /*
  Description:
    m_dV[] double precision vertices.
    m_V[] single precision vertices.
    
    If m_dV[] is not empty, then m_V and m_dV should have the same length
    and HasSynchronizedDoubleAndSinglePrecisionVertices() should be true.

    Otherwise a bug incorrectly modified vertex location information.

    If m_dV[] and m_V[] are in use and you modify vertex locations or count,
    then your calculation should insure both are properly updated.
  */
  ON_3dPointArray m_dV;
  ON_3fPointArray m_V;

  /*
  Returns:
    Location of the vertex.  If double precision vertices
    are present, the double precision vertex location is
    returned.  If vertex_index is out of range,
    ON_UNSET_VALUE is returned.
  */
  ON_3dPoint Vertex(int vertex_index) const;

  // m_F[] facets (triangles or quads)
  ON_SimpleArray<ON_MeshFace> m_F;

  ////////////////////////////////////////////////////////////////////////
  //
  // N-gon
  //
  //   An n-gon is a collection of faces that are grouped together.
  //   The outer boundary of the face collection must be a closed
  //   polyline.
  //

  ////////////////////////////////////////////////////////////////////////
  //
  // N-gon interface
  //

  /*
  Number of n-gons in this mesh.
  */
  int NgonCount() const;

  /*
  Number of n-gons in this mesh.
  */
  unsigned int NgonUnsignedCount() const;

  /*
  Returns:
    null - This mesh does ot have n-gon information.
    not null - a pointer to an array of ON_MeshNgon pointers.
      The array has length ON_Mesh::NgonCount().
  Remarks:
    If ON_Mesh::RemoveNgon has been called, then the array
    can contain null pointers.
  */
  const ON_MeshNgon* const * Ngons() const;

  /*
  Parameters:
    ngon_index - [in]
      Index of an ngon.
  Returns:
    A pointer to the indexed n-gon or null if the 
    indexed ngon is null or ngon_index is out of range.
  Remarks:
    If ON_Mesh::RemoveNgon has been called, then a null
    pointer can be returned even when ngon_index >= 0
    and ngon_index < ON_Mesh.NgonCount().
  */
  const ON_MeshNgon* Ngon(
    unsigned int ngon_index
    ) const;

  /*
  Parameters:
    ngon_index - [in]
      Index of an ngon.
  Returns:
    Total number of boundary edges, including interior edges
  */
  unsigned int NgonBoundaryEdgeCount(
    unsigned int ngon_index
  ) const;

  const ON_MeshNgon* NgonFromComponentIndex(
    class ON_MeshNgonBuffer& ngon_buffer,
    ON_COMPONENT_INDEX ci
    ) const;

  unsigned int AddNgons(
    const ON_SimpleArray<ON_COMPONENT_INDEX>& ci_list
  );

  /*
  Description:
    Add a new ngon to the mesh.
    Does not allow the creation of inner boundaries.
  Parameters:
    ngon_fi[]
      An array of distinct ON_Mesh.m_F[] face indices referencing
      a set of connected faces.
  Returns:
    index of the new n-gon.
    -1: If input information is not valid.
  */
  int AddNgon(
    const ON_SimpleArray<unsigned int>& ngon_fi
    );

    /*
  Description:
    Add a new ngon to the mesh.
  Parameters:
    ngon_fi[]
      An array of distinct ON_Mesh.m_F[] face indices referencing
      a set of connected faces.
    bPermitHoles
      If true, also ngons that contain inner boundaries are allowed.
  Returns:
    index of the new n-gon.
    -1: If input information is not valid.
  */
  int AddNgon(
    const ON_SimpleArray<unsigned int>& ngon_fi,
    bool bPermitHoles
    );

  /*
  Description:
    Add a new ngon to the mesh.
    Does not allow the creation of inner boundaries.
  Parameters:
    Fcount - [in]
      Number of face that make up the ngon.
    ngon_fi[]
      An array of N distinct ON_Mesh.m_F[] face indices referencing
      a set of connected faces.
  Returns:
    index of the new n-gon.
    -1: If input information is not valid.
  */
  int AddNgon(
    unsigned int Fcount, 
    const unsigned int* ngon_fi
    );

  /*
  Description:
    Add a new ngon to the mesh.
  Parameters:
    Fcount - [in]
      Number of face that make up the ngon.
    ngon_fi[]
      An array of N distinct ON_Mesh.m_F[] face indices referencing
      a set of connected faces.
    bPermitHoles
      If true, also ngons that contain inner boundaries are allowed.
  Returns:
    index of the new n-gon.
    -1: If input information is not valid.
*/
  int AddNgon(
    unsigned int Fcount,
    const unsigned int* ngon_fi,
    bool bPermitHoles
  );

  /*
  Description:
    Add a new ngon to the mesh.
    WARNING! The usage of this particular overload is discouraged unless
    its usage is critical for performance. If vertexFaceMap is set to something
    other than nullptr, then the value will be filled and can be re-fed to the
    function.
  Parameters:
    Fcount - [in]
      Number of face that make up the ngon.
    ngon_fi[]
      An array of N distinct ON_Mesh.m_F[] face indices referencing
      a set of connected faces.
    bPermitHoles
      If true, also ngons that contain inner boundaries are allowed.
    faceVertexMap
      If nullptr, this will be set to a new faceVertexMap. Must be freed with onfree().
      It is responsibility of the user to call onfree() on the created object.
  Returns:
    index of the new n-gon.
    -1: If input information is not valid.
*/
  int AddNgon_Expert(
    unsigned int Fcount,
    const unsigned int* ngon_fi,
    bool bPermitHoles,
    ON_MeshVertexFaceMap* vertexFaceMap
  );


  /*
  Description:
    Add a new ngon to the mesh.
  Parameters:
    Vcount - number of vertices and number of sides in the n-gon
    ngon_vi[] - in
      An array of N distinct ON_Mesh.m_V[] vertex indices
    Fcount - [in]
      Number of face that make up the ngon.
    ngon_fi[]
      An array of N distinct ON_Mesh.m_F[] face indices
      The outer boundary of this group of faces should
      be the list of vertices passes as ngon_vi[]
  Returns:
    index of the new n-gon.
    -1: If input information is not valid.
  */
  int AddNgon(
    unsigned int Vcount, 
    const unsigned int* ngon_vi,
    unsigned int Fcount, 
    const unsigned int* ngon_fi
    );

  bool ModifyNgon(
    unsigned int ngon_index,
    unsigned int Vcount, 
    const unsigned int* ngon_vi,
    unsigned int Fcount, 
    const unsigned int* ngon_fi
    );

  bool ModifyNgon(
    unsigned int ngon_index,
    const ON_MeshNgon* ngon
    );

  /*
  Description:
    Insert an n-gon in the ngon list.  This is generally
    slow.  Use AddNgon or ModifyNgon.
  */
  bool InsertNgon(
    unsigned int ngon_index,
    const ON_MeshNgon* ngon
    );

  /*
  Returns:
    Average of the n-gon vertex locations.
  */
  ON_3dPoint NgonCenter(
    unsigned int ngon_index
    ) const;

  /*
  Returns:
    Average of the n-gon vertex locations.
  */
  ON_3dPoint NgonCenter(
    const ON_MeshNgon* ngon
    ) const;

  /*
  Returns:
    Bounding box of the n-gon vertex locations.
  */
  ON_BoundingBox NgonBoundaryBoundingBox(
    unsigned int ngon_index
    ) const;

  /*
  Returns:
    Bounding box of the n-gon vertex locations.
  */
  ON_BoundingBox NgonBoundaryBoundingBox(
    const ON_MeshNgon* ngon
    ) const;

  /*
  Parameters:
    ngon - [in]
    bAppendStartPoint - [in]
      If true, the initial point in the boundary will be added
      as the first point of ngon_boundary_points[] and then
      added again as the last point of ngon_boundary_points[].
      This is useful when you need a closed polyline.
    ngon_boundary_points - [out]
      An array of ngon->m_Vcount + (bAppendStartPoint ? 1 : 0)
      points.
  Returns:
    Number of points added to ngon_boundary_points[] or 0 if invalid
    input is encountered.
  */
  unsigned int GetNgonBoundaryPoints(
    const ON_MeshNgon* ngon,
    bool bAppendStartPoint,
    ON_SimpleArray<ON_3dPoint>& ngon_boundary_points
    ) const;

  /*
  Parameters:
    ngon - [in]
    bAppendStartPoint - [in]
      If true, the initial point in the boundary will be added
      as the first point of ngon_boundary_points[] and then
      added again as the last point of ngon_boundary_points[].
      This is useful when you need a closed polyline.
    ngon_boundary_points - [out]
      An array of ngon->m_Vcount + (bAppendStartPoint ? 1 : 0) points
      is returned in ngon_boundary_points[].  The caller must insure
      that ngon_boundary_points[] has room for this many elements.
  Returns:
    Number of points added to ngon_boundary_points[] or 0 if invalid
    input is encountered.
  */
  unsigned int GetNgonBoundaryPoints(
    const ON_MeshNgon* ngon,
    bool bAppendStartPoint,
    ON_3dPoint* ngon_boundary_points
    ) const;

  /*
  Description:
    If the mesh has ngons with ON_MeshNgon.Orientation() = -1, 
    the reverse the ngon's boundary orientation.
  Parameters:
    bPermitHoles - [in]
      ngons may contain holes.
  Returns:
    True if all non-empty ngons have ON_MeshNgon.Orientation()=1 after the call.
  */
  bool OrientNgons(
    bool bPermitHoles
  );

  /*
  Description:
    Remove an n-gon.
  Parameters:
    ngon_index - [in]
  Returns:
    True if ngon_index was valid and the corresponding n-gon was removed.
  Remarks:
    The mesh triangles that make up the n-gon are not deleted.
  */
  bool RemoveNgon(
    unsigned int ngon_index
    );

  unsigned int RemoveNgons(
    unsigned int ngon_index_count,
    const unsigned int* ngon_index_list
    );

  /*
  Description:
  Remove null and empty entries from the ON_Mesh n-gon list.
  */
  void RemoveEmptyNgons();

  /*
  Description:
    Remove all entries from the ON_Mesh n-gon list.
  Remarks:
    Same as SetNgonCount(0)
  */
  void RemoveAllNgons();

  /*
  Description:
    Set the n-gon count. Null n-gons are be appended
    when ngon_count > current count.  Existing n-gons are
    removed when ngon_count < current count.
  Parameters:
    ngon_count - [in]
      Number of n-gons to have.
      0: removes all ngons.
  Remarks:
    The mesh triangles that make up any removed n-gons are not deleted.
  */
  void SetNgonCount(
    unsigned int ngon_count
    );

  /*
  Parameters:
    face_index - [in]
      Mesh face ON_Mesh.m_F[] index.
  Returns:
    ON_UNSET_UINT_INDEX:
       The face is not part of an n-gon.
    Otherwise:
      Index of the n-gon the face is part of.
  */
  unsigned int NgonIndexFromFaceIndex(
    unsigned int face_index
    ) const;

  /*
  Returns:
    null: 
      The ngonMap does not exist.
    an array of length m_F.Count():
      The value of the i-th element is either the index of the n-gon
      the mesh face m_F[i] belongs to or ON_UNSET_UINT_INDEX when
      m_F[i] does not belong to an n-gon.
  */
  const unsigned int* NgonMap() const;

  const unsigned int* NgonMap(
    bool bCreateIfMissing
    );

  /*
  Returns:
    true if the n-gon information is valid for adding an n-gon to this mesh.
  Parameters:
    Vcount - [in]
       Number of vertices and sides in the n-gon.
    ngon_vi - [in]
  */
  bool IsValidNewNgonInformation(
    unsigned int Vcount, 
    const unsigned int* ngon_vi,
    unsigned int Fcount, 
    const unsigned int* ngon_fi
    ) const;

  /*
  Description:
    For each set of coplanar connected faces in the mesh that
    qualifies as an n-gon, an new ON_MeshNgon will be appended 
    to the Ngons[] array.  Faces belonging to existing ngons are 
    ignored.
  Parameters:
    vertex_face_map - [in]
      - Pass null if you don't have one.
      - See ON_MeshVertexFaceMap for details about making one.
        The only reason to pass one in is because you
        need it for other reasons or you already have one.
    planar_tolerance - [in] 
      For faces to be coplanar, all the points in the
      n-gon must be within planar_tolerance of the plane
      defined by the first face in the n-gon.
    minimum_ngon_vertex_count - [in]
      n-gons must have at least this many sides in order
      to be added.
    minimum_ngon_face_count - [in]
      n-gons must have at least this many faces in order to
      be added.
    bAllowHoles - [in]
      If true, then the added ngons are permitted to have holes.
    bSeparateNgons - [in]
      If true, any face belonging to a new ngon, will not
      share vertices with a face that does not belong to that
      ngon and the vertex normals for all vertices in an ngon will
      be set to the plane's normal.
    bSetNgonVertexNormals - [in]
      If bSeparateNgons and bSetNgonVertexNormals are both true,
      then all vertex normals vertices in a new ngon will be
      set to the ngon's plane normal.
    bRemoveNgonInteriorPoints - [in]
      If true, the new ngons will not have interior vertices.
      This will result in the ngon being retriangluated
      when connected coplanar faces 
  Returns:
    The number of new n-gons appended to m_Ngons[]
  */
  unsigned int AddPlanarNgons(
    const unsigned int *const* vertex_face_map,
    double planar_tolerance,
    unsigned int minimum_ngon_vertex_count,
    unsigned int minimum_ngon_face_count,
    bool bAllowHoles
    );

  /*
  Description:
    For each ngon with index in the specified range,
    duplicate vertices as needed so that the ngon
    does not share any vertices with faces that do not
    belong to the ngon.
  Parameters:
    vertex_face_map - [in]
      - Pass null if you don't have one.
      - See ON_MeshVertexFaceMap for details about making one.
        The only reason to pass one in is because you
        need it for other reasons or you already have one.
      - Note that if true is returned, then the information
        in this vertex_face_map will be changed and no
        information will be added for the new vertices.
     ngon_index0 - [in]
     ngon_index1 - [in]
       ngons with indices ni satisfying 
       ngon_index0 <= ni < ngon_index1 will be separated.
       To separate every ngon in a mesh, pass
       ngon_index0 = 0 and ngon_index1 = mesh->NgonCount().    
  Returns:
    true 
      one or more vertices were duplicated to separate an ngon
      from it's neighboring faces. This changes the mesh's 
      vertex and face information and invalidates any input
      vertex_face_map.
    false
      The mesh was not modified.
  */
  bool SeparateNgons(
    unsigned int** vertex_face_map,
    unsigned int ngon_index0,
    unsigned int ngon_index1
    );

  /*
  Description:
    For each ngon with index in the specified range,
    all vertices in the ngon will have their vertex normal
    set to the normal of the first face in the ngon.
  Parameters:
     ngon_index0 - [in]
     ngon_index1 - [in]
       ngons with indices ni satisfying 
       ngon_index0 <= ni < ngon_index1 will be separated.
       To separate every ngon in a mesh, pass
       ngon_index0 = 0 and ngon_index1 = mesh->NgonCount().    
  Returns:
    true 
      one or more vertices were duplicated to separate an ngon
      from it's neighboring faces. This changes the mesh's 
      vertex and face information and invalidates any input
      vertex_face_map.
    false
      The mesh was not modified.
  */
  bool SetNgonVertexNormals(
    unsigned int ngon_index0,
    unsigned int ngon_index1
    );

  /*
  Description:
    For each ngon with index in the specified range that has
    interior vertices, remove the interior vertices and
    triangulate the ngon.
  Parameters:
    vertex_face_map - [in]
      - Pass null if you don't have one.
      - See ON_MeshVertexFaceMap for details about making one.
        The only reason to pass one in is because you
        need it for other reasons or you already have one.
      - If true is returned, then the information
        in this vertex_face_map will be invalid because
        vertices will be removed.
     ngon_index0 - [in]
     ngon_index1 - [in]
       ngons with indices ni satisfying 
       ngon_index0 <= ni < ngon_index1 will be separated.
       To separate every ngon in a mesh, pass
       ngon_index0 = 0 and ngon_index1 = mesh->NgonCount().    
  Returns:
    true 
      one or more vertices were removed and one or more ngons
      were triangulated. This changes the mesh's vertex and face
      information and invalidates any input vertex_face_map.
    false
      The mesh was not modified.
  Remarks:
    If true is returned and you are finished modify the mesh, then
    call ON_Mesh::Compact() or ON_Mesh::CullUnusedVertices() to remove
    the unreferenced interior vertices.
  */
  bool RemoveNgonInteriorVertices(
    const unsigned int *const* vertex_face_map,
    unsigned int ngon_index0,
    unsigned int ngon_index1
    );

  /*
  Description:
    Given a group of connected coplanar faces,
    find the n-gon boundary.
  ngon_fi_count - [in]
    number of indices in ngon_fi[]
  ngon_fi - [in]
    Indices of faces in the ON_Mesh.m_F[] array.
  ngon_vi - [out]
    An ordered list of indices of vertices in the ON_Mesh.m_V[] 
    array that for the outer boundary of the n-gon.  The natural
    counter-clockwise orientation of the first face with a 
    boundary edge determines the order of the ngon_vi[] list.
  */
  unsigned int GetNgonOuterBoundary(
    unsigned int ngon_fi_count,
    const unsigned int* ngon_fi,
    ON_SimpleArray<unsigned int>& ngon_vi
    ) const;
  
  /*
  Description:
    An expert user function that allocates an ngon from heap
    memory managed by this ON_Mesh.
  Parameters:
    N - [in] (>= 3)
    Fcount - [in]
  Returns:
    A pointer to an uninitialized ngon.
    Use the ON_Mesh::AddNgon(ngon) to add this ngon to the mesh
    or use DeallocateNgon(ngon) to deallocate the ngon.
  */
  ON_MeshNgon* AllocateNgon(
    unsigned int Vcount, 
    unsigned int Fcount
    );

  /*
  Description:
    An expert user function that deallocates an ngon
    that was created by AllocateNgon().
  Parameters:
    ngon - [in]
  */
  bool DeallocateNgon(
    ON_MeshNgon* ngon
    );

  /*
  Description:
    An expert user function that unconditionally appends the ngon
    pointer to ON_Mesh.m_Ngon[].
  Parameters:
    ngon - [in]
  Returns:
      ON_UNSET_UINT_INDEX: invalid input
    < ON_UNSET_UINT_INDEX: index of the new n-gon.
  */
  unsigned int AddNgon(
    ON_MeshNgon* ngon
    );

  /*
  Description:
    Expert user function to update n-gon map after the expert user
    does something to make the current one invalid.
  Returns:
    null: 
      The mesh does not have ngon-information.
    an array of length m_F.Count() ngon_map[]
      - If ngon_map[fi] >= 0, then ON_MeshFace.m_F[fi] belongs
        to ON_Mesh.Ngon(ngon_map[fi]).
      - Otherwise, ngon_map[fi] = -1.
  */
  const unsigned int* CreateNgonMap();

  /*
  Description:
    Expert user function to construct n-gon map even on const objects
    after the expert user did something that made the current one invalid.
  map [in-out]:
      The map must have at least m_F elements
      It is modified to match the new ngon map, if true is returned.
  Returns:
  A value indicating if the mesh has ngon information.
  The map is an array of length m_F.Count(), ngon_map[]
      - If ngon_map[fi] >= 0, then ON_MeshFace.m_F[fi] belongs
        to ON_Mesh.Ngon(ngon_map[fi]).
      - Otherwise, ngon_map[fi] = -1.
  */
  bool CreateNgonMap(unsigned int* ngon_map) const;


  /*
Description:
  Expert user function to construct n-gon map even on const objects
  after the expert user did something that made the current one invalid.
map [out]:
    The map is modified to match the new ngon map, if true is returned.
Returns:
A value indicating if the mesh has ngon information.
The map is an array of length m_F.Count(), ngon_map[]
    - If ngon_map[fi] >= 0, then ON_MeshFace.m_F[fi] belongs
      to ON_Mesh.Ngon(ngon_map[fi]).
    - Otherwise, ngon_map[fi] = -1.
*/
  bool CreateNgonMap(ON_SimpleArray<unsigned int>& map) const;

  /*
  Description:
    Expert user function to delete n-gon map information
    but leave n-gon definition information unchanged.
  Description:
    Removes any existing n-gon map.
    Does not remove other n-gon information.
  */
  void RemoveNgonMap();

  ////////////////////////////////////////////////////////////////////////
  //
  // N-gon implementation
  //

  // If ON_Mesh::HasNgons() is true, then the mesh has n-gons.
  // When a mesh has ngons, m_NgonMap[] is used to determine when
  // a face belongs to an n-gon.
  // If m_NgonMap[fi] < m_Ngon.UnsignedCount(), then it is the index of the N-gon in m_Ngon[] 
  // that m_F[]  belongs to.  Otherwise, m_NgonMap[fi] is ON_UNSET_UINT_INDEX.
  ON_SimpleArray<unsigned int> m_NgonMap; // invalid if m_NgonMap.Count() != m_F.Count()
  ON_SimpleArray<ON_MeshNgon*> m_Ngon;
  ON_MeshNgonAllocator m_NgonAllocator; // use this to allocate elements added to m_Ngon;

  ////////////////////////////////////////////////////////////////////////
  //
  // Vertex and Face normal implementation
  //

  // m_N[] OPTIONAL vertex unit normals
  // If m_N[] is empty or m_N.Count() != m_V.Count(), 
  // Either m_N[] has zero count or it m_N[j] is the
  // the unit vertex normal at m_V[j].
  ON_3fVectorArray m_N;

  // m_FN[] OPTIONAL face unit normals
  // If m_FN[] is empty or m_FN.Count() != m_F.Count(), 
  // then m_FN is ignored.  Otherwise m_FN[j] is the
  // unit normal for the facet m_F[j].
  ON_3fVectorArray m_FN;

  /////////////////////////////////////////////////////////////////
  // Implementation - texture coordinates
  //
  // OPTIONAL texture coordinates for each vertex

  // The m_Ttag member has been DEPRECATED. Please don't use it.
  ON_MappingTag m_Ttag; // DEPRECATED

  // The m_T array has been DEPRECATED. Please don't use it.
  // Instead, use the m_S array to store per-vertex texture coordinates.
  ON_2fPointArray m_T;  // DEPRECATED

  // RUNTIME ONLY
  //   This array is used to cache texture coordinates used by
  //   rendering applications that require 1d, 2d or 3d texture
  //   coordinates, or multiple sets of texture coordinates
  //   (e.g. blended textures with different mappings).
  //   Users are responsible for verifying 
  //   m_TC[i].m_T.Count() = m_V.Count()
  ON_ClassArray<ON_TextureCoordinates> m_TC;  

  // If m_T.Count() == m_V.Count(), then the mesh has texture coordinates
  // and m_T[j] is the texture coordinate for vertex m_V[j].
  //
  // When opennurbs or Rhino meshes an ON_Surface or ON_Brep, the texture
  // coordinates have a "canonical" linear relationship with the surface 
  // parameters that is described in the next section.  However, various 
  // mappings, spherical, planar, cylindrical, etc., can be applied that 
  // change the values of the texture coordinates.
  //
  // If a texture mapping function was used to set the m_T[] values, 
  // then the id and serial number of the mapping function is saved
  // in m_mapping_id and m_mapping_sn. The intended use of these fields
  // is to make it easy to avoid unnecessary recalculation.  
  // If a mesh is modified, then m_mapping_id should be set to nil 
  // and m_mapping_crc should be set to 0.
  //
  /////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////
  // Texture coordinates / surface parameters
  //
  // If m_S.Count() == m_V.Count(), then the mesh has texture
  // coordinates, or it is a tessellation of a parametric surface
  // and m_S[j] is the texture coordinate / surface parameter at
  // m_V[j].
  // If the values in m_S are changed, then you need to call
  // InvalidateCachedTextureCoordinates().
  // Storing values in m_S[] is OPTIONAL.
  ON_2dPointArray m_S;

  // Packed texture information
  ON_Interval m_srf_domain[2]; // surface evaluation domain.

  // If m_srf_scale[] has positive values, then they represent
  // the world coordinate size of a rectangle that would 
  // minimize texture distortion if it were mapped to the
  // mesh using normalized surface evaluation parameters.
  // This information is used to calculate high quality 
  // packed texture coordinates.  
  double m_srf_scale[2];


  // Packed texture information.
  //
  // If either of the m_packed_tex_domain[] intervals is a 
  // proper subinterval of (0,1), then a texture packing 
  // calculation assigned this subrectangle to this mesh.
  ON_Interval m_packed_tex_domain[2];

  // The m_packed_tex_rotate setting is valid only when
  // m_S, m_srf_domain, m_packed_scale[] and 
  // m_packed_tex_domain[] are all valid and the texture
  // coordinates are based on surface evaluation parameters.
  // In this special situation, this boolean records the 
  // correspondence between the the surface parameters, (u,v),
  // and the packed texture coordinates, (s,t),
  //
  //   m_packed_tex_rotate = false:
  //     a = m_srf_domain[0].NormalizedParameterAt(u);
  //     b = m_srf_domain[1].NormalizedParameterAt(v);
  //     s = m_packed_tex_domain[0].ParameterAt(a);
  //     t = m_packed_tex_domain[1].ParameterAt(b);
  //
  //     x = m_packed_tex_domain[0].NormalizedParameterAt(s);
  //     y = m_packed_tex_domain[1].NormalizedParameterAt(t);
  //     u = m_srf_domain[0].ParameterAt(x);
  //     v = m_srf_domain[1].ParameterAt(y);
  //
  //   m_packed_tex_rotate = true:
  //     a = m_srf_domain[0].NormalizedParameterAt(u);
  //     b = m_srf_domain[1].NormalizedParameterAt(v);
  //     s = m_packed_tex_domain[0].ParameterAt(a);
  //     t = m_packed_tex_domain[1].ParameterAt(1.0-b);
  //
  //     x = m_packed_tex_domain[0].NormalizedParameterAt(s);
  //     y = m_packed_tex_domain[1].NormalizedParameterAt(t);
  //     u = m_srf_domain[0].ParameterAt(y);
  //     v = m_srf_domain[1].ParameterAt(1.0 - x);
  bool m_packed_tex_rotate;

  /*
  Returns:
    True if the m_srf_scale[] values are positive and
    the m_packed_tex_domain[] intervals are set to values
    that describe a proper subrectangle of (0,1)x(0,1).
    True does not necessarily mean the current values in
    m_T[] are packed texture coordinates.
  */
  bool HasPackedTextureRegion() const;

  /*
  Description:
    If the mesh does not have surface evaluation parameters,
    has texture coordinates, and the surface parameters can
    be set in a way so the existing texture coordinates can
    be computed from the surface parameters, then this function
    sets the surface parameters.  This is useful when meshes
    that have texture coordinates and do not have surface 
    parameters want ot set the surface parameters in a way
    so that the texture mapping
    ON_TextureMapping::SurfaceParameterTextureMapping
    will restore the texture coordinates.
  Returns:
    true - successful
    false - failure - no changes made to the mesh.
  */
  bool SetSurfaceParamtersFromTextureCoodinates();


  /////////////////////////////////////////////////////////////////
  // Implementation - curvature

  ON_SimpleArray<ON_SurfaceCurvature> m_K;  // OPTIONAL surface curvatures
                                            // Either m_K[] has zero count or it has the same
                                            // count as m_V[], in which case m_K[j] reports
                                            // the surface curvatures at m_V[j].

  /////////////////////////////////////////////////////////////////
  // Implementation - false color
  ON_MappingTag m_Ctag; // OPTIONAL tag for values in m_C[]
  ON_SimpleArray<ON_Color> m_C;  // OPTIONAL vertex color
                                 // Either m_C[] has zero count or it has the same
                                 // count as m_V[], in which case m_C[j] reports
                                 // the color assigned to m_V[j].

  /////////////////////////////////////////////////////////////////
  // Implementation - runtime vertex visibility - not saved in 3dm files.
  ON_SimpleArray<bool> m_H; // OPTIONAL vertex visibility.
                            // If m_H.Count() = m_V.Count(), then
                            // m_H[vi] is true if the vertex m_V[vi] 
                            // is hidden.  Otherwise, all vertices are visible.
  int m_hidden_count;       // number of vertices that are hidden
                            // = number of true values in m_H[] array.

  /////////////////////////////////////////////////////////////////
  // Implementation - runtime UI information
  const ON_Object* m_parent; // runtime parent geometry (use ...::Cast() to get it)

protected:

  /////////////////////////////////////////////////////////////////
  // Implementation - mesh topology
  ON_MeshTopology m_top;

  ON_MeshParameters* m_mesh_parameters; // If mesh was created from a parametric surface,
                                        // these parameters were used to create the mesh.
  int                         m_invalid_count;
  int                         m_quad_count;
  int                         m_triangle_count;

private:
  char m_mesh_is_closed;   // 0 = unset, 1 = all edges have 2 or more faces, 2 = at least one boundary edge 
  char m_mesh_is_manifold; // 0 = unset, 1 = all edges have 1 or 2 faces, 2 = not manifold
  char m_mesh_is_oriented; // 0 = unset, 1 = faces normals agree across all edges that have 2 faces, 2 = not oriented
  char m_mesh_is_solid;    // 0 = unset, 1 = solid with outward face normals, 2 = solid with inward face normals, 3 = not solid

private:
  mutable ON_BoundingBox m_vertex_bbox = ON_BoundingBox::UnsetBoundingBox;

protected:
  float m_nbox[2][3]; // 3d bounding box of all referenced unit normals 
                      // (for estimation of Gauss map bounds)
  float m_tbox[2][2]; // 2d bounding box of all referenced texture coordinates

private:
  // m_vertex_bbox = bounding box of vertex locations

  // cache of recently used tight bounding boxes
  mutable ON_BoundingBoxCache m_tight_bbox_cache;

protected:

  ON_MeshCurvatureStats* m_kstat[4]; // gaussian,mean,min,max,sectionx,sectiony,sectionz

  // sub-mesh information rendering large meshes
  ON_MeshPartition* m_partition;

#if defined(OPENNURBS_PLUS)
private:
  class ON_MeshTreeCache* m_mesh_cache;
  class ON_MeshTreeCache* MeshTreeCache( bool ) const;
  void DestroyMeshTreeCache(bool);
#endif

private:
  bool Write_1( ON_BinaryArchive& ) const; // uncompressed 1.x format
  bool Write_2( int, ON_BinaryArchive& ) const; // compressed 2.x format
  bool Read_1( ON_BinaryArchive& );
  bool Read_2( int, ON_BinaryArchive& );
  bool WriteFaceArray( int, int, ON_BinaryArchive& ) const;
  bool ReadFaceArray( int, int, ON_BinaryArchive& );
  bool SwapEdge_Helper( int, bool );

public:
#if defined(OPENNURBS_PLUS)
  bool SetMappingMeshInfo(const ON_MappingMeshInfo& info);
#endif
  const ON_MappingMeshInfo* GetMappingMeshInfo() const;
#if defined(OPENNURBS_PLUS)
  void RemoveMappingMeshInfo();
  void TransformMappingMeshInfo(const ON_Xform& xform);
  bool SetRenderMeshInfo(const ON_RenderMeshInfo& info);
#endif
  const ON_RenderMeshInfo* GetRenderMeshInfo() const;
};

//////////////////////////////////////////////////////////////////////////
//
// ON_MeshRef
//
class ON_CLASS ON_MeshRef
{
public:
  static const ON_MeshRef Empty;

  ON_MeshRef() ON_NOEXCEPT;
  ~ON_MeshRef();
  ON_MeshRef(const ON_MeshRef& src) ON_NOEXCEPT;
  ON_MeshRef& operator=(const ON_MeshRef& src);

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_MeshRef( ON_MeshRef&& ) ON_NOEXCEPT;
  // rvalue assignment operator
  ON_MeshRef& operator=( ON_MeshRef&& );
#endif

  /*
  Returns:
    True if no ON_Mesh is being managed by this ON_MeshRef.
  Remarks:
    It is always the case that exactly one of IsEmpty() and IsNotEmpty() is true.
    Both are provided so code using ON_MeshRef can be clean and easily read.
  */
  bool IsEmpty() const;

  /*
  Returns:
    True if an ON_Mesh is being managed by this ON_MeshRef.
  Remarks:
    It is always the case that exactly one of IsEmpty() and IsNotEmpty() is true.
    Both are provided so code using ON_MeshRef can be clean and easily read.
  */
  bool IsNotEmpty() const;

  /*
  Returns:
    The mesh being managed by this ON_MeshRef. 
    If this ON_MeshRef is not managing an ON_Mesh, then ON_Mesh::Empty is returned.
  */
  const class ON_Mesh& Mesh() const;

  /*
  Returns:
    Number of references to the managed ON_Mesh, including the one by this ON_MeshRef.
  */
  unsigned int ReferenceCount() const;

  /*
  Description:
    Allocates a new empty ON_Mesh and has this ON_MeshRef reference it.
  */
  class ON_Mesh& NewMesh();

  /*
  Description:
    Allocates a new ON_Mesh and has this ON_MeshRef reference it.
  Parameters:
    src - [in]
      The new ON_Mesh managed by this ON_MeshRef will be a copy of src.Mesh().
  Returns:
    A reference to the new ON_Mesh managed by this ON_MeshRef.
  */
  class ON_Mesh& CopyMesh(
    const ON_MeshRef& src
    );

  /*
  Description:
    Allocates a new ON_Mesh and has this ON_MeshRef reference it.
  Parameters:
    src - [in]
      The new ON_Mesh managed by this ON_MeshRef will be a copy of src.
  Returns:
    A reference to the new ON_Mesh managed by this ON_MeshRef.
  */
  class ON_Mesh& CopyMesh(
    const ON_Mesh& src
    );

  /*
  Description:
    If ReferenceCount() > 1, then have this ON_MeshRef reference a 
    new copy. Otherwise do nothing. The result being that this will
    be the unique reference to the ON_Mesh managed by this ON_MeshRef.
  Returns:
    A reference to the ON_Mesh uniquely managed by this ON_MeshRef.
  */
  class ON_Mesh& UniqueMesh();

  /*
  Returns:
    The mesh being managed by this ON_MeshRef.
    If this ON_MeshRef is not managing an ON_Mesh, then ON_Mesh::Empty is returned.
  */
  const std::shared_ptr<class ON_Mesh>& SharedMesh() const;
  
  /*
  Description:
    Remove this reference to the managed ON_Mesh. 
    If this is the last reference, then the managed ON_Mesh is deleted.
  */
  void Clear();

public:
  /*
  Description:
    Expert user function to have this ON_MeshRef manage mesh.
  Parameters:
    mesh - [in/out]
      mesh must point to an ON_Mesh that was constructed on the heap using
      an operator new call with a public ON_Mesh constructor.
  Returns:
    a pointer to the managed mesh
  Example:
    ON_Mesh* mesh = new ON_Mesh(...);
    ON_MeshRef mesh_ref;
    ON_Mesh* managed_mesh = mesh_ref.SetMesh(mesh);
    // mesh = nullptr
    // managed_mesh = pointer you can use
  */
  class ON_Mesh* SetMeshForExperts(
    class ON_Mesh*& mesh
    );
  
private:
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4251 ) 
  // C4251: ... needs to have dll-interface to be used by clients of class ...
  // m_mesh_sp is private and all code that manages m_mesh_sp is explicitly implemented in the DLL.
private:
  std::shared_ptr<class ON_Mesh> m_mesh_sp;
#pragma ON_PRAGMA_WARNING_POP
};

//////////////////////////////////////////////////////////////////////////
//
// ON_MeshCache
//
class ON_CLASS ON_MeshCache
{
public:
  static const ON_MeshCache Empty;

  static const ON_UUID RenderMeshId;
  static const ON_UUID AnalysisMeshId;
  static const ON_UUID PreviewMeshId;
  static const ON_UUID AnyMeshId;

  // Cached mesh with the fewest faces
  static const ON_UUID CoarseMeshId;

  // Cached mesh with the most faces
  static const ON_UUID FineMeshId;

  /*
  Returns:
    The id that corresponds to the obsolete ON::mesh_type enum value.
  Remarks:
    Ids are used to allow custom meshes to be cached.
  */
  static ON_UUID MeshIdFromMeshType(
    ON::mesh_type mesh_type
    );

public:
  ON_MeshCache() = default;
  ~ON_MeshCache();
  ON_MeshCache( const ON_MeshCache& src );
  ON_MeshCache& operator=( const ON_MeshCache& src );

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_MeshCache( ON_MeshCache&& ) ON_NOEXCEPT;

  ON_MeshCache& operator=( ON_MeshCache&& );
#endif

public:

  /*
  Parameters:
    mesh_id - [in]
      mesh_id cannot be nil or ON_MeshCache::AnyMeshId.
  */  
  void SetMesh(
    ON_UUID mesh_id,
    const std::shared_ptr<ON_Mesh>& mesh_sp
    );
  void SetMesh(
    ON::mesh_type mesh_type,
    const std::shared_ptr<ON_Mesh>& mesh_sp
    );
    
  /*
  Parameters:
    mesh_id - [in]
      If mesh_id is ON_MeshCache::AnyMeshId, then every cached mesh 
      will be deleted.
  */
  void ClearMesh(
    ON_UUID mesh_id
    );
  void ClearMesh(
    ON::mesh_type mesh_type
    );
  
  void ClearAllMeshes();

  /*
  Parameters:
    bDeleteMesh - [in]
      true 
        ON_Mesh will be deleted.
      false 
        ON_Mesh will not be deleted. This is typically done when the
        mesh was in the process of being created in a separate thread
        and memory pool, both of which were killed and the pointer
        to the mesh is no longer valid.
  */
  void ClearMesh(
    ON_UUID mesh_id,
    bool bDeleteMesh
    );

  /*
  Parameters:
    bDeleteMesh - [in]
      true 
        ON_Mesh will be deleted.
      false 
        ON_Mesh will not be deleted. This is typically done when the
        mesh was in the process of being created in a separate thread
        and memory pool, both of which were killed and the pointer
        to the mesh is no longer valid.
  */
  void ClearMesh(
    ON::mesh_type mesh_type,
    bool bDeleteMesh
    );
  

  /*
  Parameters:
    bDeleteMeshes - [in]
      true 
        ON_Mesh will be deleted.
      false 
        ON_Mesh will not be deleted. This is typically done when the
        mesh was in the process of being created in a separate thread
        and memory pool, both of which were killed and the pointer
        to the mesh is no longer valid.
  */
  void ClearAllMeshes(
    bool bDeleteMeshes
  );

  /*
  Parameters:
    mesh_id - [in]
      If mesh_id is ON_MeshCache::AnyMeshId, then the most recently cached mesh is returned.
  */
  const ON_Mesh* Mesh(
    ON_UUID mesh_id
    ) const;
  const ON_Mesh* Mesh(
    ON::mesh_type mesh_type
    ) const;


  /*
  Parameters:
    mesh_id - [in]
      If mesh_id is ON_MeshCache::AnyMeshId, then the most recently cached mesh is returned.
  */
  std::shared_ptr<ON_Mesh> MeshSharedPtr(
    ON_UUID mesh_id
    ) const;

  std::shared_ptr<ON_Mesh> MeshSharedPtr(
    ON::mesh_type mesh_type
    ) const;

  unsigned int MeshCount() const;

  bool Write(
    ON_BinaryArchive& archive
    ) const;

  bool Read(
    ON_BinaryArchive& archive
    );

  void Dump(
    ON_TextLog& text_log
    ) const;

  bool Transform(
    const ON_Xform& xform
    );

private:
  void Internal_CopyHelper(
    const class ON_MeshCacheItem* src_item_list
    );

  class ON_MeshCacheItem* Internal_FindHelper(
    ON_UUID mesh_type
    ) const;

  class ON_MeshCacheItem* Internal_CreateItem();
  class ON_MeshCacheItem* Internal_CopyItem(const class ON_MeshCacheItem& src_item);

  void Internal_DeleteItem(class ON_MeshCacheItem*,bool bDeleteMesh);

  class ON_MeshCacheItem* m_impl = nullptr;
};

class ON_CLASS ON_MeshNgonIterator
{
public:

  static const ON_MeshNgonIterator EmptyMeshNgonIterator;

  ON_MeshNgonIterator() = default;
  ~ON_MeshNgonIterator() = default;
  
  ON_MeshNgonIterator(
    const ON_MeshNgonIterator& src
    );

  ON_MeshNgonIterator& operator=(
    const ON_MeshNgonIterator& src
    );


  /*
  Parameters:
    mesh - [in]
      If the mesh has explicit ngons, then mesh->NgonMap() must
      return true;
  */
  ON_MeshNgonIterator(
    const class ON_Mesh* mesh
    );

  /*
  Parameters:
    mesh - [in]
      If the mesh has explicit ngons, 
    meshfdex_to_meshngondex_map - [in]
      It's generally best to pass the value of mesh->NgonMap(true).
      Expert users can specify a custom map if required.
  */
  void SetMesh(
    const class ON_Mesh* mesh,
    const unsigned int* meshfdex_to_meshngondex_map
    );

  /*
  Returns:
    The mesh being iterated.
  */
  const ON_Mesh* Mesh() const;

  /*
  Description:
    Returns the first ngon.
  Returns:
    The first ngon when iterating through the mesh 
    triangles, quads and explicitly defined ngons.
  Remarks:
    If CurrentNgonIsMeshFace() is true after calling FirstNgon().
    the the returned ngon references a triangle or
    quad that is not part of an explicitly defined
    ngon in the mesh. If you need the information 
    to persist after any subsequent calls to the iterator
    or after the destruction of the iterator, then
    you must make and manage a copy of the ngon.
  */
  const class ON_MeshNgon* FirstNgon();

  /*
  Description:
    Increments the iterator and returns the next ngon.
  Returns:
    The next ngon when iterating through the mesh 
    triangles, quads and explicitly defined ngons.
  Remarks:
    If CurrentNgonIsMeshFace() is true after calling NextNgon().
    the the returned ngon references a triangle or
    quad that is not part of an explicitly defined
    ngon in the mesh. If you need the information 
    to persist after any subsequent calls to the iterator
    or after the destruction of the iterator, then
    you must make and manage a copy of the ngon.
  */
  const class ON_MeshNgon* NextNgon();
  
  /*
  Description:
    Get the ngon most recently returned by FirstNgon()
    or NextNgon().
  Returns:
    Returns the ngon most recently returned by FirstNgon()
    or NextNgon().
  Remarks:
    If CurrentNgonIsMeshFace() is true after calling CurrentNgon().
    the the returned ngon references a triangle or
    quad that is not part of an explicitly defined
    ngon in the mesh. If you need the information 
    to persist after any subsequent calls to the iterator
    or after the destruction of the iterator, then
    you must make and manage a copy of the ngon.
  */
  const class ON_MeshNgon* CurrentNgon();

  ON_COMPONENT_INDEX CurrentNgonComponentIndex() const;
  /*
  Returns:
    If the current iterator ngon references an ON_MeshFace
    that is in m_mesh->m_F[] but is not explicitly referenced
    by an ON_MeshNgon in ON_Mesh.m_Ngon[], then true is returned.
    In this case, the ngon's m_fi[] array
    has length 1 and contains the face's index, and the ngon's
    m_vi[] array is a copy of the faces's vi[] array.
    Otherwise false is returned.
  */
  bool CurrentNgonIsMeshFace() const;

  /*
  Returns:
    If the current iterator ngon references an ON_MeshNgon
    that is in m_mesh->m_Ngon[], then true is returned.
    Otherwise false is returned.
  */
  bool CurrentNgonIsMeshNgon() const;
  
  /*
  Description:
    Sets the state of the iterator to the initial state that
    exists after construction.  This is useful if the iterator
    has been used the get one or more elements and then
    the referenced mesh is modified or code wants
    to begin iteration again a used a call to NextNgon()
    to return the first element.
  */
  void Reset();

  /*
  Returns:
    Number of ngons that will be iterated over.
  Remarks:
    The count = explicit ngons + faces that are not in an ngon.
  */
  unsigned int Count() const;

private:
  const class ON_Mesh* m_mesh = nullptr;
  const unsigned int* m_facedex_to_ngondex_map = nullptr;
  ON__UINT_PTR m_current_ngon = 0;
  ON_MeshNgonBuffer m_ngon_buffer;
  ON_COMPONENT_INDEX m_current_ngon_ci = ON_COMPONENT_INDEX::UnsetComponentIndex;
  unsigned int m_mesh_face_count = 0;
  unsigned int m_mesh_ngon_count = 0;
  unsigned int m_iterator_index = 0;
};

class ON_CLASS ON_MeshComponentRef : public ON_Geometry
{
  ON_OBJECT_DECLARE(ON_MeshComponentRef);
public:
  static const ON_MeshComponentRef Unset;

  ON_MeshComponentRef();
  ON_MeshComponentRef(
    const class ON_Mesh* mesh,
    ON_COMPONENT_INDEX ci 
    );
  ~ON_MeshComponentRef();
  ON_MeshComponentRef& operator=(const ON_MeshComponentRef&);

  /*
  Description:
    Dictionary compare:
      1) Compare m_mesh pointer values as unsigned values
      2) ON_COMPONENT_INDEX::Compare() m_mesh_ci values
  */
  static int Compare(const ON_MeshComponentRef* lhs, const ON_MeshComponentRef* rhs);

  /*
  Description:
    Dictionary compare:
      1) Compare m_mesh pointer values as unsigned values
      2) ON_COMPONENT_INDEX::Compare() m_mesh_ci values
  */
  static int Compare2(const ON_MeshComponentRef* const * lhs, const ON_MeshComponentRef *const* rhs);

  /*
  Description:
    Compare m_mesh pointer values as unsigned values.
    Ignore m_mesh_ci values.
  */
  static int CompareMeshPointer(const ON_MeshComponentRef* lhs, const ON_MeshComponentRef* rhs);

private:
  // referenced mesh
  const class ON_Mesh* m_mesh;

  // component
  ON_COMPONENT_INDEX m_mesh_ci;

public:
  void Set(
    const class ON_Mesh* mesh,
    ON_COMPONENT_INDEX ci 
    );

  /*
  Returns:
    The referenced mesh.
  */
  const class ON_Mesh* Mesh() const;

  /*
  Description:
    Override of the virtual ON_Geometry::ComponentIndex().
  Returns:
    A mesh component index for the face.  The type is
    ON_COMPONENT_INDEX::mesh_face and the index is the
    index into the ON_Mesh.m_F[] array.
  */
  ON_COMPONENT_INDEX ComponentIndex() const override;

  /*
  Returns:
    If the mesh topology exists or the component references
    a mesh topology component, then this returns a pointer
    to the mesh topology.
    Otherwise null is returned.
  */
  const class ON_MeshTopology* MeshTopology() const;

  /*
  Returns:
    If the component is a vertex, this returns the vertex index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int VertexIndex() const;

  /*
  Returns:
    If the component is a mesh vertex or mesh topology vertex,
    then this returns the vertex location.
    Otherwise ON_3dPoint::UnsetPoint is returned.
  */
  ON_3dPoint VertexPoint() const;

  /*
  Parameters:
    point - [out]
      location of the vertex
  Returns:
    If the component is a vertex, this returns the vertex index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int GetVertexPoint(
    class ON_3dPoint& point
    ) const;

  /*
  Returns:
    If the component is a vertex and mesh topology exists or
    the component is a mesh topology vertex, then this returns
    a pointer to the mesh topology vertex index.
    Otherwise null is returned.
  */
  const struct ON_MeshTopologyVertex* MeshTopologyVertex() const;

  /*
  Returns:
    If the component is a vertex and mesh topology exists or
    the component is a mesh topology vertex, then this returns
    the mesh topology vertex index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int MeshTopologyVertexIndex() const;

  /*
  Returns:
    If the component is a vertex and mesh topology exists or
    the component is a mesh topology vertex, then this returns
    the mesh topology vertex index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int GetMeshTopologyVertexPoint(
    class ON_3dPoint& point
    ) const;

  /*
  Returns:
    If the component is a vertex and mesh topology exists or
    the component is a mesh topology vertex, then this returns
    the mesh topology vertex index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int GetMeshTopologyVertex(
    const struct ON_MeshTopologyVertex*& topv
    ) const;

  /*
  Returns:
    If the component is a vertex and mesh topology exists or
    the component is a mesh topology vertex, then this returns
    the mesh topology vertex index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int GetMeshTopologyVertexAndPoint(
    const struct ON_MeshTopologyVertex*& topv, 
    class ON_3dPoint& point
    ) const;

  /*
  Returns:
    If the component is a mesh topology edge, this returns
    the mesh topology edge index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned int MeshTopologyEdgeIndex() const;

  /*
  Returns:
    If the component is an edge, this returns the edge.
    Otherwise null is returned.
  */
  const struct ON_MeshTopologyEdge* MeshTopologyEdge() const;
  
  unsigned int GetMeshTopologyEdge(
    const struct ON_MeshTopologyEdge*& tope
    ) const;
  
  unsigned int GetMeshTopologyEdgeLine(
    class ON_Line& line
    ) const;

  /*
  Parameters:
    line - [out]
      If the component is an edge, the 3d line is returned here.
  Returns:
    If the component is an edge, this returns the edge.
    Otherwise null is returned.
  */
  unsigned int GetMeshTopologyEdgeAndLine(
    const struct ON_MeshTopologyEdge*& tope,
    ON_Line& line 
    ) const;

  /*
  Returns:
    - If the component references to an ON_MeshNgon in the mesh, then a pointer
      to this ngon is returned.
    - If the component references an ON_MeshFace triangle or quad, then
      then a single face ON_MeshNgon is created in the memory in
      ngon_buffer. 
    - Otherwise, null is returned.
  */
  const class ON_MeshNgon* MeshNgon(
    class ON_MeshNgonBuffer& ngon_buffer
    ) const;

  /*
  Returns:
    If the component is a face or an ngon containing a single face,
    this returns the face index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  Remarks:
    The best way to write code that works with triangle, quad
    and ngons is to use the ON_MeshComponent::MeshNgon(ngon_buffer).
  */
  unsigned int MeshFaceIndex() const;

  /*
  Returns:
    If the component is a face or an ngon made of a single face,
    this returns the face.
    Otherwise null is returned.
  Remarks:
    The best way to write code that works with triangle, quad
    and ngons is to use the ON_MeshComponent::MeshNgon(ngon_buffer).
  */
  const class ON_MeshFace* MeshFace() const;

  /*
  Returns:
    If the component is a face or an ngon made of a single face,
    this returns the face.
    Otherwise null is returned.
  Remarks:
    The best way to write code that works with triangle, quad
    and ngons is to use the ON_MeshComponent::MeshNgon(ngon_buffer).
  */
  unsigned int GetMeshFace(
    const class ON_MeshFace*& mesh_face
    ) const;


  /*
  Returns:
    If the component is an ngon or a face in an ngon,
    this returns the ngon index.
    Otherwise ON_UNSET_UINT_INDEX is returned.
  Remarks:
    The best way to write code that works with triangle, quad
    and ngons is to use the ON_MeshComponent::MeshNgon(ngon_buffer).
  */
  unsigned int MeshNgonIndex() const;


  /*
  Returns:
    If the component is an ngon or a face in an ngon, this returns the ngon.
    Otherwise null is returned.
  Remarks:
    The best way to write code that works with triangle, quad
    and ngons is to use the ON_MeshComponent::MeshNgon(ngon_buffer).
  */
  const class ON_MeshNgon* MeshNgon() const;


  // overrides of virtual ON_Object functions
  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;
  void Dump( ON_TextLog& ) const override;
  unsigned int SizeOf() const override;
  ON::object_type ObjectType() const override;

  // overrides of virtual ON_Geometry functions
  int Dimension() const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  bool Transform( 
         const ON_Xform& xform
         ) override;
};

#if defined(OPENNURBS_PLUS)
/*
Description:
  Calculate a quick and dirty polygon mesh approximation
  of a surface.
Parameters:
  surface - [in]
  mesh_density - [in] If <= 10, this number controls
        the relative polygon count.  If > 10, this number
        specifies a target number of polygons.
  mesh - [in] if not nullptr, the polygon mesh will be put
              on this mesh.
Returns:
  A polygon mesh approximation of the surface or nullptr
  if the surface could not be meshed.
*/
ON_DECL
ON_Mesh* ON_MeshSurface( 
            const ON_Surface& surface, 
            int mesh_density = 0,
            ON_Mesh* mesh = 0
            );

/*
Description:
  Calculate a quick and dirty polygon mesh approximation
  of a surface.
Parameters:
  surface - [in]
  u_count - [in] >= 2 Number of "u" parameters in u[] array.
  u       - [in] u parameters
  v_count - [in] >= 2 Number of "v" parameters in v[] array.
  v       - [in] v parameters
  mesh - [in] if not nullptr, the polygon mesh will be put
              on this mesh.
Returns:
  A polygon mesh approximation of the surface or nullptr
  if the surface could not be meshed.
*/
ON_DECL
ON_Mesh* ON_MeshSurface( 
            const ON_Surface& surface, 
            int u_count,
            const double* u,
            int v_count,
            const double* v,
            ON_Mesh* mesh = 0
            );

ON_DECL
ON_Mesh* ON_MeshSurface( 
            const ON_Surface& surface, 
            int u_count,
            const double* u,
            int v_count,
            const double* v,
            bool bDoublePrecisionMesh,
            ON_Mesh* mesh
            );

/*
Description:
  Finds the barycentric coordinates of the point on a 
  triangle that is closest to P.
Parameters:
  A - [in] triangle corner
  B - [in] triangle corner
  C - [in] triangle corner
  P - [in] point to test
  a - [out] barycentric coordinate
  b - [out] barycentric coordinate
  c - [out] barycentric coordinate
        If ON_ClosestPointToTriangle() returns true, then
        (*a)*A + (*b)*B + (*c)*C is the point on the 
        triangle's plane that is closest to P.  It is 
        always the case that *a + *b + *c = 1, but this
        function will return negative barycentric 
        coordinate if the point on the plane is not
        inside the triangle.
Returns:
  True if the triangle is not degenerate.  False if the
  triangle is degenerate; in this case the returned
  closest point is the input point that is closest to P.
*/
ON_DECL
bool ON_ClosestPointToTriangle( 
        ON_3dPoint A, ON_3dPoint B, ON_3dPoint C,
        ON_3dPoint P,
        double* a, double* b, double* c
        );

/*
Description:
  Finds the barycentric coordinates of the point on a 
  triangle that is closest to P.
Parameters:
  A - [in] triangle corner
  B - [in] triangle corner
  C - [in] triangle corner
  P - [in] point to test
  a - [out] barycentric coordinate
  b - [out] barycentric coordinate
  c - [out] barycentric coordinate
        If ON_ClosestPointToTriangle() returns true, then
        (*a)*A + (*b)*B + (*c)*C is the point on the 
        triangle's plane that is closest to P.  It is 
        always the case that *a + *b + *c = 1, but this
        function will return negative barycentric 
        coordinate if the point on the plane is not
        inside the triangle.
Returns:
  True if the triangle is not degenerate.  False if the
  triangle is degenerate; in this case the returned
  closest point is the input point that is closest to P.
*/
ON_DECL
bool ON_ClosestPointToTriangleFast( 
          const ON_3dPoint& A, 
          const ON_3dPoint& B, 
          const ON_3dPoint& C, 
          ON_3dPoint P,
          double* a, double* b, double* c
          );

/*
Description:
  Finds the barycentric coordinates of a pair of points on two
  triangles that are as close as any other pair.
Parameters:
  A - [in] first triangle corners
  B - [in] second triangle corners
  a - [out] barycentric coordinates for triangle A
  b - [out] barycentric coordinates for triangle B

        If ON_ClosestPointBetweenTriangles() returns true, then
        (a[0])*A[0] + (a[1])*A[1] + (a[2])*A[2] is the point on  
        triangle A and  (b[0])*B[0] + (b[1])*B[1] + (b[2])*B[2] is
        the point on triangle B.  It is 
        always the case that a[0]+a[1]+a[2] = 1, a[0]>=0, a[1]>0, a[2]>0,
        b[0]+b[1]+b[2] = 1, b[0]>=0, b[1]>0, b[2]>0
Returns:
  True if successful.  
*/
ON_DECL
bool ON_ClosestPointBetweenTriangles(const ON_3dPoint A[3],
                                     const ON_3dPoint B[3],
                                     double a[3],
                                     double b[3]
                                     );

/*
Description:
  Finds the barycentric coordinates of a pair of points on two
  triangles that are as close as any other pair.
Parameters:
  Tri - [in] triangle corners
  Quad - [in] quad corners in order around the quad
  t - [out] barycentric coordinates for Tri
  q - [out] barycentric coordinates for Quad

        If ON_ClosestPointBetweenTriangleAndQuad() returns true, then
        (t[0])*Tri[0] + (t[1])*Tri[1] + (t[2])*Tri[2] is the point on  
        Tri and  (q[0])*Quad[0] + (q[1])*Quad[1] + (q[2])*Quad[2] + (q[3])*Quad[3] is
        the point on Quad.  It is 
        always the case that t[0]+t[1]+t[2] = 1, t[0]>=0, t[1]>0, t[2]>0,
        q[0]+q[1]+q[2]+q[3] = 1, q[0]>=0, q[1]>0, q[2]>0, q[3].
        The surface of Quad is defined to be the two triangle surfaces, 
        <Quad[0], Quad[1], Quad[2]> and <Quad[2], Quad[3], Quad[0]>.  It will always be
        the case that either q[1]=0 or q[3]=0.
Returns:
  True if successful.  
*/
ON_DECL
bool ON_ClosestPointBetweenTriangleAndQuad(const ON_3dPoint Tri[3],
                                           const ON_3dPoint Quad[4],
                                           double t[3],
                                           double q[4]
                                           );

/*
Description:
  Finds the barycentric coordinates of a pair of points on two
  triangles that are as close as any other pair.
Parameters:
  A - [in] first quad corners, in order around the quad
  B - [in] second quad corners, in order around the quad
  a - [out] barycentric coordinates for quad A
  b - [out] barycentric coordinates for quad B

        If ON_ClosestPointBetweenQuads() returns true, then
        (a[0])*A[0] + (a[1])*A[1] + (a[2])*A[2] + (a[3])*A[3] is the point on  
        quad A and  (b[0])*B[0] + (b[1])*B[1] + (b[2])*B[2] + (b[3])*B[3] is
        the point on quad B.  It is 
        always the case that a[0]+a[1]+a[2]+a[3] = 1, a[0]>=0, a[1]>0, a[2]>0, a[3]>0.
        b[0]+b[1]+b[2]+b[3] = 1, b[0]>=0, b[1]>0, b[2]>0, b[3]>0
        The surface of Quad is defined to be the two triangle surfaces, 
        <Quad[0], Quad[1], Quad[2]> and <Quad[2], Quad[3], Quad[0]>.  It will always be
        the case that either q[1]=0 or q[3]=0.
Returns:
  True if successful.  
*/
ON_DECL
bool ON_ClosestPointBetweenQuads(const ON_3dPoint A[4],
                                 const ON_3dPoint B[4],
                                 double a[4],
                                 double b[4]
                                 );

/*
Description:
  Finds a the closest points on a pair of meshes.
Parameters:
  MeshA    - [in]  First mesh
  MeshB    - [in]  Second mesh
  max_dist - [in]  Do not search for point pairs further apart than max_dist
  bUseMultipleThreads - [in]
    Pass true if you want the calculation to use
    multiple threads.
  fidA     - [out] index of closest face of MeshA
  a        - [out] barycentric coordinates of closest point on MeshA.m_F[*fidA]
  fidB     - [out] index of closest face of MeshB
  b        - [out] barycentric coordinates of closest point on MeshB.m_F[*fidB]
Returns:
  true if points A and B could be found with distance from A to B no greater than max_dist
  and as close together as any other pair. A is the sum of a[i]*VA[i], B is the sum of b[i]*VB[i],
  where VA and VB are the vertices of MeshA.m_F[*fidA] and MeshB.m_F[*fidB].  Note that if the face is a triangle,
  the 4th barycentric coordinate will be 0, and 3 is a valid index for the mesh face vertex array.
*/
ON_DECL
bool ON_GetMeshMeshClosestPoint(const ON_Mesh& MeshA, const ON_Mesh& MeshB, 
                                double max_dist,
                                bool bUseMultipleThreads,
                                int* fidA, double a[4],
                                int* fidB, double b[4]);
#endif

/*
Description:
  Calculate a mesh representation of the NURBS surface's control polygon.
Parameters:
  nurbs_surface - [in]
  bCleanMesh - [in] If true, then degenerate quads are cleaned
                    up to be triangles. Surfaces with singular
                    sides are a common source of degenerate qauds.
  input_mesh - [in] If nullptr, then the returned mesh is created
       by a class to new ON_Mesh().  If not null, then this 
       mesh will be used to store the control polygon.
Returns:
  If successful, a pointer to a mesh.
*/
ON_DECL
ON_Mesh* ON_ControlPolygonMesh( 
          const ON_NurbsSurface& nurbs_surface, 
          bool bCleanMesh,
          ON_Mesh* input_mesh = nullptr
          );

#if defined(OPENNURBS_PLUS)
/*
Description:
  Finds the intersection between a line segment an a triangle.
Parameters:
  A - [in] triangle corner
  B - [in] triangle corner
  C - [in] triangle corner
  P - [in] start of line segment
  Q - [in] end of line segment
  abc - [out] 
     barycentric coordinates of intersection point(s)
  t - [out] line coordinate of intersection point(s)
Returns:
  0 - no intersection
  1 - one intersection point
  2 - intersection segment
*/
ON_DECL
int ON_LineTriangleIntersect(
        const ON_3dPoint& A,
        const ON_3dPoint& B,
        const ON_3dPoint& C,
        const ON_3dPoint& P,
        const ON_3dPoint& Q,
        double abc[2][3], 
        double t[2],
        double tol
        );
#endif

/*
Description:
  Finds the unit normal to the triangle
Parameters:
  A - [in] triangle corner
  B - [in] triangle corner
  C - [in] triangle corner
Returns:
  Unit normal
*/
ON_DECL
ON_3dVector ON_TriangleNormal(
        const ON_3dPoint& A,
        const ON_3dPoint& B,
        const ON_3dPoint& C
        );


/*
Description:
  Finds the unit normal to the triangle
Parameters:
  A - [in] triangle corner
  B - [in] triangle corner
  C - [in] triangle corner
  a - [out] must not be null
  b - [out] must not be null
  c - [out] must not be null
  d - [out] must not be null
    The equation of the plane is a*x + b*y + c*z + d = 0
  ev_tol - [out]
    If ev_tol is not null, then it is the maximum absolute
    value of the plane equation evaluated at A,B,C.  Mathematically,
    ev_tol is zero.  Since these computations are performed with
    finite precision doubles, ev_tol is generally not zero.
Returns:
  Unit normal
*/
ON_DECL
bool ON_GetTrianglePlaneEquation(
        const ON_3dPoint& A,
        const ON_3dPoint& B,
        const ON_3dPoint& C,
        double* a,
        double* b,
        double* c,
        double* d,
        double* evaluation_tol
        );

#if defined(OPENNURBS_PLUS)
/*
Description:
  Triangulate a 2D simple closed polygon.
Parameters:
  point_count - [in] number of points in polygon ( >= 3 )
  point_stride - [in]
  P - [in] 
    i-th point = (P[i*point_stride], P[i*point_stride+1])
  tri_stride - [in]
  triangle - [out]
    array of (point_count-2)*tri_stride integers
Returns:
  True if successful.  In this case, the polygon is trianglulated into 
  point_count-2 triangles.  The indices of the 3 points that are the 
  corner of the i-th (0<= i < point_count-2) triangle are
    (triangle[i*tri_stride], triangle[i*tri_stride+1], triangle[i*tri_stride+2]).
Remarks:
  Do NOT duplicate the start/end point; i.e., a triangle will have
  a point count of 3 and P will specify 3 distinct non-collinear points.
*/
ON_DECL
bool ON_Mesh2dPolygon( 
          int point_count,
          int point_stride,
          const double* P,
          int tri_stride,
          int* triangle 
          );

/// <summary>
/// Fill in a 2d region with triangles.
/// </summary>
/// <param name="point_count">
/// Number of 2d points. There must be at least 3 points.
/// </param>
/// <param name="point_stride">
/// Double offset for 2d points. 
/// i-th 2d point = (points[j],points[j+1]), where j = i*point_stride.
/// </param>
/// <param name="points">
/// 2d point locations.  
/// It is ok to include points that are inside the region but not at the end of any input edge.
/// Duplicate points are not permitted.
/// </param>
/// <param name="edge_count">
/// Number of edges in the edges[] array.
/// If edge_count = 0, then the convex hull of points[] will be the boundary.
/// If edge_count = point_count, edge_stride = 0, edges = nullptr, and edge_side_stride = 0
/// then the input list of points must from a closed polyline and will be the 
/// outer boundary of the region to be meshed. Note this polyline does not have to be convex.
/// </param>
/// <param name="edge_stride">
/// i-th edge connects points (edges[j],edges[j+1]) where j = i*edge_stride.
/// </param>
/// <param name="edges">
/// indices of edge ends. If any edges intersect, then the intersection points are
/// returned in new_points[].
/// </param>
/// <param name="edge_side">
/// If not nullptr, then edge_side[i] determines which side(s) of the edge should be meshed.
/// 0 = Automatically determined edge orientation.
///     This is the best choice for all edges that are not inner boundary edges.
/// 1 = left side only - boundary edge with a triangle on the left side.
///     This is a good choice for clockwise inner boundary edges.
/// 2 = right side only - boundary edge with a triangle on the right side.
///     This is a good choice for counter-clockwise inner boundary edges.
/// If edge_side = nullptr, the 0 is used for all edges.
/// If all edges have the same edge_side value, then 
/// setting edge_side_stride = 0 and passing the address of char with that value
/// saves having to create an array where every element has the same value.
/// </param>
/// <param name="triangles">
/// triangles are appended to this list. 
/// If you are reusing triangles[] for a collection of DISTINCT meshes, the
/// call triangles.SetCount(0) before each call to ON_Mesh2dRegion().
/// If you are using multiple calls to ON_Mesh2dRegion() to
/// create a single mesh, say your tesselatiing a sequence of glyphs into a single mesh,
/// then you typically need to adjust the indices in the appended elements accordingly.
/// The (i,j,k) values are vertex indices.
/// If a triangle vertex index is 0 &gt;= i &lt; point_count, then the corresponding 2d point 
/// is (points[i*point_stride], points[i*point_stride+1]).
/// If a triangle vertex index is i &gt;= point_count, then the corresponding 2d point 
/// is new_points[i-point_count].
/// </param>
/// <returns>
/// The number of triangles appended to triangles[] array.
/// </returns>
ON_DECL
int ON_Mesh2dRegion(
          int point_count,
          int point_stride,
          const double* points,
          int edge_count,
          int edge_stride,
          const int* edges,
          const int* edge_side,
          ON_SimpleArray<ON_3dex>& triangles
          );

/// <summary>
/// Fill in a 2d region with triangles.
/// </summary>
/// <param name="point_count">
/// Number of 2d points. There must be at least 3 points.
/// </param>
/// <param name="point_stride">
/// Double offset for 2d points. 
/// i-th 2d point = (points[j],points[j+1]), where j = i*point_stride.
/// </param>
/// <param name="points">
/// 2d point locations.  
/// It is ok to include points that are inside the region but not at the end of any input edge.
/// Duplicate points are not permitted.
/// </param>
/// <param name="edge_count">
/// Number of edges in the edges[] array.
/// If edge_count = 0, then the convex hull of points[] will be the boundary.
/// If edge_count = point_count, edge_stride = 0, edges = nullptr, and edge_side_stride = 0
/// then the input list of points must from a closed polyline and will be the 
/// outer boundary of the region to be meshed. Note this polyline does not have to be convex.
/// </param>
/// <param name="edge_stride">
/// i-th edge connects points (edges[j],edges[j+1]) where j = i*edge_stride.
/// </param>
/// <param name="edges">
/// indices of edge ends. If any edges intersect, then the intersection points are
/// returned in new_points[].
/// </param>
/// <param name="edge_side_stride">
/// If edge_side is not nullptr, then the edge_side for the i-th edge is edge_side[i*edge_side_stride].
/// If you're calling from code where you have edge_side[] values stored in ints, you may pass a pointer
/// to the least significant bit of the int and set edge_side_stride = sizeof(int).
/// </param>
/// <param name="edge_side">
/// If not nullptr, then edge_side[i] determines which side(s) of the edge should be meshed.
/// 0 = Automatically determined edge orientation.
///     This is the best choice for all edges that are not inner boundary edges.
/// 1 = left side only - boundary edge with a triangle on the left side.
///     This is a good choice for clockwise inner boundary edges.
/// 2 = right side only - boundary edge with a triangle on the right side.
///     This is a good choice for counter-clockwise inner boundary edges.
/// If edge_side = nullptr, the 0 is used for all edges.
/// If all edges have the same edge_side value, then 
/// setting edge_side_stride = 0 and passing the address of char with that value
/// saves having to create an array where every element has the same value.
/// </param>
/// <param name="triangles">
/// triangles are appended to this list. 
/// If you are reusing triangles[] for a collection of DISTINCT meshes, the
/// call triangles.SetCount(0) before each call to ON_Mesh2dRegion().
/// If you are using multiple calls to ON_Mesh2dRegion() to
/// create a single mesh, say your tesselatiing a sequence of glyphs into a single mesh,
/// then you typically need to adjust the indices in the appended elements accordingly.
/// The (i,j,k) values are vertex indices.
/// If a triangle vertex index is 0 &gt;= i &lt; point_count, then the corresponding 2d point 
/// is (points[i*point_stride], points[i*point_stride+1]).
/// If a triangle vertex index is i &gt;= point_count, then the corresponding 2d point 
/// is new_points[i-point_count].
/// </param>
/// <param name="new_points">
/// If the edges intersect, then the intersection points are added to new_points[]./// 
/// </param>
/// <returns>
/// The number of triangles appended to triangles[] array.
/// </returns>
ON_DECL
int ON_Mesh2dRegion(
          unsigned int point_count,
          unsigned int point_stride,
          const double* points,
          unsigned int edge_count,
          unsigned int edge_stride,
          const unsigned int* edges,
          unsigned int edge_side_stride,
          const unsigned char* edge_side,
          ON_SimpleArray<ON_3dex>& triangles,
          ON_SimpleArray<ON_2dPoint>& new_points
          );

/// <summary>
/// Fill in a 2d region with triangles.
/// </summary>
/// <param name="point_count">
/// Number of 2d points. There must be at least 3 points.
/// </param>
/// <param name="point_stride">
/// Double offset for 2d points. 
/// i-th 2d point = (points[j],points[j+1]), where j = i*point_stride.
/// </param>
/// <param name="points">
/// 2d point locations.  
/// It is ok to include points that are inside the region but not at the end of any input edge.
/// Duplicate points are not permitted.
/// </param>
/// <param name="edge_count">
/// Number of edges in the edges[] array.
/// If edge_count = 0, then the convex hull of points[] will be the boundary.
/// If edge_count = point_count, edge_stride = 0, edges = nullptr, and edge_side_stride = 0
/// then the input list of points must from a closed polyline and will be the 
/// outer boundary of the region to be meshed. Note this polyline does not have to be convex.
/// </param>
/// <param name="edge_stride">
/// i-th edge connects points (edges[j],edges[j+1]) where j = i*edge_stride.
/// </param>
/// <param name="edges">
/// indices of edge ends. If any edges intersect, then the intersection points are
/// returned in new_points[].
/// </param>
/// <param name="edge_side_stride">
/// If edge_side is not nullptr, then the edge_side for the i-th edge is edge_side[i*edge_side_stride].
/// If you're calling from code where you have edge_side[] values stored in ints, you may pass a pointer
/// to the least significant bit of the int and set edge_side_stride = sizeof(int).
/// </param>
/// <param name="edge_side">
/// If not nullptr, then edge_side[i] determines which side(s) of the edge should be meshed.
/// 0 = Automatically determined edge orientation.
///     This is the best choice for all edges that are not inner boundary edges.
/// 1 = left side only - boundary edge with a triangle on the left side.
///     This is a good choice for clockwise inner boundary edges.
/// 2 = right side only - boundary edge with a triangle on the right side.
///     This is a good choice for counter-clockwise inner boundary edges.
/// If edge_side = nullptr, the 0 is used for all edges.
/// If all edges have the same edge_side value, then 
/// setting edge_side_stride = 0 and passing the address of char with that value
/// saves having to create an array where every element has the same value.
/// </param>
/// <param name="bPermitVertexAdditions">
/// If true, then additional interior points may be added to improve the
/// quality of the mesh. The additional points are returned in new_points[].
/// </param>
/// <param name="bPermitEdgeSplitting">
/// If true, then intersection edges are split and other edges may be
/// split edges may be split to improve mesh quality.
/// The additional points are returned in new_points[].
/// </param>
/// <param name="triangles">
/// triangles are appended to this list. 
/// If you are reusing triangles[] for a collection of DISTINCT meshes, the
/// call triangles.SetCount(0) before each call to ON_Mesh2dRegion().
/// If you are using multiple calls to ON_Mesh2dRegion() to
/// create a single mesh, say your tesselatiing a sequence of glyphs into a single mesh,
/// then you typically need to adjust the indices in the appended elements accordingly.
/// The (i,j,k) values are vertex indices.
/// If a triangle vertex index is 0 &gt;= i &lt; point_count, then the corresponding 2d point 
/// is (points[i*point_stride], points[i*point_stride+1]).
/// If a triangle vertex index is i &gt;= point_count, then the corresponding 2d point 
/// is new_points[i-point_count].
/// </param>
/// <param name="new_points">
/// If the returned new_points[] has count = 0, then no points were added.
/// If the returned new_points[] has count &gt; 0, then new points were added.
/// If the edges intersect, then the intersection points are added to new_points[].
/// If bPermitVertexAdditions is true, then any those additions are added to new_points[].
/// </param>
/// <returns>
/// The number of triangles appended to triangles[] array.
/// </returns>
ON_DECL
int ON_Mesh2dRegion(
  unsigned int point_count,
  unsigned int point_stride,
  const double* points,
  unsigned int edge_count,
  unsigned int edge_stride,
  const unsigned int* edges,
  unsigned int edge_side_stride,
  const unsigned char* edge_side,
  bool bPermitVertexAdditions,
  bool bPermitEdgeSplitting,
  ON_SimpleArray<ON_3dex>& triangles,
  ON_SimpleArray<ON_2dPoint>& new_points
);


/// <summary>
/// Fill in a 2d region with triangles.
/// </summary>
/// <param name="point_count">
/// Number of 2d points. There must be at least 3 points.
/// </param>
/// <param name="point_stride">
/// Double offset for 2d points. 
/// i-th 2d point = (points[j],points[j+1]), where j = i*point_stride.
/// </param>
/// <param name="points">
/// 2d point locations.  
/// It is ok to include points that are inside the region but not at the end of any input edge.
/// Duplicate points are not permitted.
/// </param>
/// <param name="edge_count">
/// Number of edges in the edges[] array.
/// If edge_count = 0, then the convex hull of points[] will be the boundary.
/// If edge_count = point_count, edge_stride = 0, edges = nullptr, and edge_side_stride = 0
/// then the input list of points must from a closed polyline and will be the 
/// outer boundary of the region to be meshed. Note this polyline does not have to be convex.
/// </param>
/// <param name="edge_stride">
/// i-th edge connects points (edges[j],edges[j+1]) where j = i*edge_stride.
/// </param>
/// <param name="edges">
/// indices of edge ends. If any edges intersect, then the intersection points are
/// returned in new_points[].
/// </param>
/// <param name="edge_side_stride">
/// If edge_side is not nullptr, then the edge_side for the i-th edge is edge_side[i*edge_side_stride].
/// If you're calling from code where you have edge_side[] values stored in ints, you may pass a pointer
/// to the least significant bit of the int and set edge_side_stride = sizeof(int).
/// </param>
/// <param name="edge_side">
/// If not nullptr, then edge_side[i] determines which side(s) of the edge should be meshed.
/// 0 = Automatically determined edge orientation.
///     This is the best choice for all edges that are not inner boundary edges.
/// 1 = left side only - boundary edge with a triangle on the left side.
///     This is a good choice for clockwise inner boundary edges.
/// 2 = right side only - boundary edge with a triangle on the right side.
///     This is a good choice for counter-clockwise inner boundary edges.
/// If edge_side = nullptr, the 0 is used for all edges.
/// If all edges have the same edge_side value, then 
/// setting edge_side_stride = 0 and passing the address of char with that value
/// saves having to create an array where every element has the same value.
/// </param>
/// <param name="bOuterBoundaryIsConvexHull">
/// If edges[] contains one or more outer boundaries, then pass false.
/// Otherwise pass true; the outer boundary of the returned mesh will be the convex
/// hull of points[] and any edges that are not part of the convex hull will be inside
/// the mesh.
/// </param>
/// <param name="bPermitVertexAdditions">
/// If true, then additional interior points may be added to improve the
/// quality of the mesh. The additional points are returned in new_points[].
/// </param>
/// <param name="bPermitEdgeSplitting">
/// If true, then intersection edges are split and other edges may be
/// split edges may be split to improve mesh quality.
/// The additional points are returned in new_points[].
/// </param>
/// <param name="triangles">
/// triangles are appended to this list. 
/// If you are reusing triangles[] for a collection of DISTINCT meshes, the
/// call triangles.SetCount(0) before each call to ON_Mesh2dRegion().
/// If you are using multiple calls to ON_Mesh2dRegion() to
/// create a single mesh, say your tesselatiing a sequence of glyphs into a single mesh,
/// then you typically need to adjust the indices in the appended elements accordingly.
/// The (i,j,k) values are vertex indices.
/// If a triangle vertex index is 0 &gt;= i &lt; point_count, then the corresponding 2d point 
/// is (points[i*point_stride], points[i*point_stride+1]).
/// If a triangle vertex index is i &gt;= point_count, then the corresponding 2d point 
/// is new_points[i-point_count].
/// </param>
/// <param name="new_points">
/// If the returned new_points[] has count = 0, then no points were added.
/// If the returned new_points[] has count &gt; 0, then new points were added.
/// If the edges intersect, then the intersection points are added to new_points[].
/// If bPermitVertexAdditions is true, then any those additions are added to new_points[].
/// </param>
/// <returns>
/// The number of triangles appended to triangles[] array.
/// </returns>
ON_DECL
int ON_Mesh2dRegion(
  unsigned int point_count,
  unsigned int point_stride,
  const double* points,
  unsigned int edge_count,
  unsigned int edge_stride,
  const unsigned int* edges,
  unsigned int edge_side_stride,
  const unsigned char* edge_side,
  bool bOuterBoundaryIsConvexHull,
  bool bPermitVertexAdditions,
  bool bPermitEdgeSplitting,
  ON_SimpleArray<ON_3dex>& triangles,
  ON_SimpleArray<ON_2dPoint>& new_points
);


class ON_CLASS ON_MeshXPoint
{
public:
  // this class must not have a vtable.
  ON_MeshXPoint();

  /*
  Description:
    Dictionary compares the coordinates of m_S.
  */
  static int Compare2dLocation(
    const ON_MeshXPoint* p, 
    const ON_MeshXPoint* q
    );

  /*
  Description:
    Dictionary compares the coordinates of m_point.
  */
  static int Compare3dLocation(
    const ON_MeshXPoint* p, 
    const ON_MeshXPoint* q
    );

  /*
  Description:
    Dictionary the coordinates of m_point, then m_mesh_id.
  */
  static int Compare3dLocationAndId(
    const ON_MeshXPoint* p, 
    const ON_MeshXPoint* q
    );

  /*
  Description:
    Dictionary the coordinates of m_point, then m_mesh_id, 
    then the coordinates of m_S.
  */
  static int CompareLocationsAndId(
    const ON_MeshXPoint* p, 
    const ON_MeshXPoint* q
    );

  /*
  Description:
    Compares the next and previous pointers in m_p[].
    (null,null) < (not null,null) < (null,not null) < (not null,not null)
  */
  static int CompareNextPrev(
    const ON_MeshXPoint* p, 
    const ON_MeshXPoint* q
    );

  /*
  Returns:
     0: not a facet corner
     1: face.vi[0] corner
     2: face.vi[1] corner
     4: face.vi[2] corner
     8: face.vi[3] corner
  */
  int IsFacetCorner() const;

  /*
  Parameters:
    p - [in]
      edge starting point
    q - [in]
      edge ending point      
  Returns:
     0: (p,q) are not the ends of a mesh facet edge
     +3: (p,q) is the edge from face.vi[0] to face.vi[1].
     -3: (p,q) is the edge from face.vi[1] to face.vi[0].
     +5: (p,q) is the edge from face.vi[2] to face.vi[0].
     -5: (p,q) is the edge from face.vi[0] to face.vi[2].
     +9: (p,q) is the edge from face.vi[3] to face.vi[0].
     -9: (p,q) is the edge from face.vi[0] to face.vi[3].
     +6: (p,q) is the edge from face.vi[1] to face.vi[2].
     -6: (p,q) is the edge from face.vi[2] to face.vi[1].
    +10: (p,q) is the edge from face.vi[3] to face.vi[1].
    -10: (p,q) is the edge from face.vi[1] to face.vi[3].
    +12: (p,q) is the edge from face.vi[2] to face.vi[3].
    -12: (p,q) is the edge from face.vi[3] to face.vi[2].
  */
  static int IsFacetEdge(
    const ON_MeshXPoint* p, 
    const ON_MeshXPoint* q
    );

  // m_point must be the first data member in this class.
  ON_3dPoint m_point;

  // m_event_sn must immediately follow m_point.
  ON__UINT32 m_event_sn;
  
  // The bits of (m_status_corner_mask & m_point_status) are set to
  // identify the corner or edge of the facet the mesh-plane
  // intersection point lies on. Below "Cn" identifies a corner of
  // the facet: Cn = mesh.m_V[face.vi[n]], n = 0, 1, 2 or 3.
  //
  //   (1 & m_point_status): set if C0 is an end point
  //   (2 & m_point_status): set if C1 is an end point
  //   (4 & m_point_status): set if C2 is an end point
  //   (8 & m_point_status): set if C3 is an end point
  //   (1 | 2): 
  //     point is interior to the facet edge connecting C0 and C1
  //   (2 | 4): 
  //     point is interior to the facet edge connecting C1 and C2
  //   (4 | 8): 
  //     point is interior to the quad edge connecting C2 and C3
  //   (1 | 8): 
  //     point is interior to the quad edge connecting C3 and C0
  //   (1 | 4): 
  //     point is interior to the triangle edge connecting C0 and C2
  //
  //   (0x10 & m_point_status):
  //     set if the point is a corner point (not interior).
  //
  // The bits of (m_status_olap_mask & m_point_status) reports overlap conditions.
  //
  //   (m_status_olap_facet & m_point_status): 
  //     set if the point is the corner of a facet that is in the plane.
  //   (m_status_edge_01 & m_point_status): 
  //     set if the facet edge connecting C0 and C1 is in the plane
  //     and the point is at the start or end of this facet edge.
  //   (m_status_edge_12 & m_point_status): 
  //     set if the facet edge connecting C1 and C2 is in the plane
  //     and the point is at the start or end of this facet edge.
  //   (m_status_edge_23 & m_point_status): 
  //     set if the quad facet edge connecting C2 and C3 is in the 
  //     plane and the point is at the start or end of this edge.
  //   (m_status_edge_30 & m_point_status): 
  //     set if the quad facet edge connecting C3 and C0 is in the
  //     plane and the point is at the start or end of this edge.
  //   (m_status_edge_20 & m_point_status): 
  //     set if the triangle facet edge connecting C2 and C0 is in the
  //     plane and the point is at the start or end of this edge.
  ON__UINT16 m_point_status;

private:
  friend class ON_MeshXPointPool;
  unsigned char m_private_tag; // used internally

private:
  unsigned char m_reserved1;

public:

  // The "m_tag" field is set to zero when an ON_MeshXPoint
  // is created and when it is destroyed.  It is copied when
  // an ON_MeshXPoint class is copied. Nothing else in opennurbs
  // modifies or uses this value.
  ON__INT_PTR m_tag;

  ON__UINT_PTR m_mesh_id;



  unsigned int m_mesh_facet_index;
  // If (0x0F & m_status_bits) = 1, 2, 4 or 8, then
  // m_mesh_vertex_index = index of the mesh's vertex.
  unsigned int m_mesh_vertex_index;

  // Surface parameters
  //  If no surface parameters are on the mesh, m_S = (0.0,0.0).
  ON_2dPoint m_S;

  // The values of m_bc[] are the mesh facet's barycentric coordinates
  // for the point.
  double m_bc[4];

  // The m_p[] pointers are used as previous and next pointers when
  // connecting events in an intersection component. 
  // See ON_MeshXPoint::JoinPoints() for more details.
  class ON_MeshXPoint* m_p[2]; // (previous,next)


  ////////////////////////////////////////////////////////////////
  //
  //
  // The interpretation of m_other_id depends on what created
  // the ON_MeshXPoint.
  //   ON_MeshXPlane::IntersectLine(): m_other_id = line_id
  //   ON_MeshXPlane::IntersectPlane(): m_other_id = plane_id

  double m_other_parameter;
  ON__UINT_PTR m_other_id;

private:
  ON__UINT_PTR  m_reserved2;

public:

  ////////////////////////////////////////////////////////////////
  //
  //

  static const ON__UINT16 m_status_corner_value[4];   // = {0x0001,0x0002,0x0004,0x0008}
  static const ON__UINT16 m_status_corner_value_mask; // = 0x000F

  static const ON__UINT16 m_status_at_corner;         // = 0x0010
  static const ON__UINT16 m_status_corner_mask;       // = 0x001F

  static const ON__UINT16 m_status_olap_edge_start;   // = 0x0020
  static const ON__UINT16 m_status_olap_edge_end;     // = 0x0040
  static const ON__UINT16 m_status_olap_edge_mask;    // = 0x0060

  static const ON__UINT16 m_status_olap_facet_corner; // = 0x0080

  static const ON__UINT16 m_status_degenerate;        // = 0x0100
};

class ON_CLASS ON_MeshXPointPair
{
public:
  class ON_MeshXPoint* m_mxpoint[2];
};


class ON_CLASS ON_MeshXPointPoolIterator : private ON_FixedSizePoolIterator
{
public:
  ON_MeshXPointPoolIterator( const class ON_MeshXPointPool& mxpointpool );

  const class ON_MeshXPointPool& m_mxpointpool;

  class ON_MeshXPoint* FirstPoint();
  class ON_MeshXPoint* NextPoint();

private:
  // no implementation
  ON_MeshXPointPoolIterator& operator=(const ON_MeshXPointPoolIterator&);
};

class ON_CLASS ON_MeshXPointPool : private ON_FixedSizePool
{
  friend class ON_MeshXPointPoolIterator;

public:
  ON_MeshXPointPool();

  /*
  Parameters:
    mesh_point_event_sn0 - [in]
      event serial numbers will begin at mesh_point_event_sn0+1
  */
  ON_MeshXPointPool(unsigned int mesh_point_event_sn0);

  unsigned int PointCount() const;

  /*
  Returns:
    All event serial numbers are <= MaximumEventSerialNumber().
  */
  unsigned int MaximumEventSerialNumber() const;

  ON_MeshXPoint* AllocPoint();

  void ReturnPoint(ON_MeshXPoint*);

  void ReturnAllPoints();

  /*
  Description:
    Set every point's m_tag value to tag.
  Parameters:
    tag - [in]
  Remarks:
    This
  */
  void SetAllTags( ON__INT_PTR tag );

  /*
  Returns:
    True if the ON_MeshXPoint.m_p[] values are correctly
    set.
  */
  bool ValidatePrevNext() const;

  /*
  Description:
    If ValidatePrevNext() returns false, then RepairPrevNext()
    can be used to alter the m_p[] values so they make sense.
  Returns:
    Number of ON_MeshXPoint m_p[0] values that were set
    to zero.
  */
  unsigned int RepairPrevNext();


  /*
  Description:
    Join the points into connected intersection components
    by setting the ON_MeshXPoint::m_p[] pointers to
    the previous and next points in the intersection component.
    The first point of an intersection component has zero m_p[0]
    and nonzero m_p[1] values. The last point of an intersection
    component has nonzero m_p[0] and zero m_p[1] values.
  Parameters:
    starts - [out]
      (pass null if you do not want this information)
      For each intersection component, the first point in the
      component is appended to starts[].  
    pairs - [out]  
      (pass null if you do not want this information)
      For each intersection component, a pair with the first and
      last points in intersection component is appended to pairs[].
    compare_points_func - [in]
      If compare_points_func() is null, then the function
      ON_MeshXPoint::CompareLocationsAndId() is used
      to decide if points are coincident. If compare_points_func()
      is not null, it must be a function that can be used
      to quick sort an array of ON_MeshXPoint elements.
  Remarks:
    The linked list of ON_MeshXPoint will contain duplicate
    locations where the intersection component crosses a shared edge.
    The JoinPoints() function requires the input values of m_p[] 
    to be valid. Call RepairPrevNext() before calling JoinPoints()
    if you are running code outside of opennurbs that may have bugs
    that incorrectly set m_p[] values.
  */
  void JoinPoints( 
    ON_SimpleArray< ON_MeshXPoint* >* starts,
    ON_SimpleArray< ON_MeshXPointPair >* pairs ,
    int (*compare_points_func)(const ON_MeshXPoint*, const ON_MeshXPoint*)
    );

private:
  // used to generate serial numbers for ON_MeshXPoint.m_event_sn
  unsigned int m_mesh_point_event_sn;
};

class ON_CLASS ON_MeshX
{
public:

  /*
  Parameters:
    mesh - [in]
      mesh.MeshFaceTree(true) is used to get a mesh face rtree.
    mesh_id - [in]
      Value of that is assigned to ON_MeshXPoint.m_mesh_id.
  */
  ON_MeshX(    
    const ON_Mesh& mesh,
    ON__UINT_PTR mesh_id
    );

  ON_MeshX(    
    const ON_Mesh& mesh,
    const ON_RTree& mesh_face_rtree,
    ON__UINT_PTR mesh_id
    );

  ON_MeshX(    
    unsigned int vertex_count,
    const class ON_3dPoint* mesh_dV,
    const class ON_3fPoint* mesh_fV,
    const class ON_2dPoint* mesh_S,
    unsigned int face_count,
    const class ON_MeshFace* mesh_F,
    const ON_RTree& mesh_face_rtree,
    ON__UINT_PTR mesh_id
    );

  ~ON_MeshX();

  double BarycentricCoordinateTolerance() const;

  /*
  Parameters:
    bc_tolerance - [in] 
      If a barycentric coordinate is < bc_tolerance,
      then the barycentric coordinate is set to zero.
      Any value that is invalid or < ON_EPSILON or >= 0.25
      is ignored.  
  Returns:
    true if bc_tolerance was a valid value and will be used
    as the barycentric coordinate tolerance.
  Remarks:
    The default is ON_EPSILON.  Barycentric coordinates are
    calculated from plane equation evaluations of mesh vertex
    locations. All internal calculations use double precision
    arithmetic.
  */
  bool SetBarycentricCoordinateTolerance(double bc_tolerance);

  /*
  Returns:
    Transformation that is conceptually applied to the
    mesh before it is intersected with the plane.
  */
  ON_Xform MeshTransform() const;

  /*
  Returns:
    Inverse of the transformation that is conceptually applied
    to the mesh before it is intersected with the plane.
  */
  ON_Xform MeshInverseTransform() const;

  /*
  Description:
    If you want to calculate the instersection of a plane
    and xform*mesh, then you can use SetMeshTransform() to
    specify the transformation and avoid having to transform
    the mesh and calculate a new mesh face rtree.
  Parameters:
    xform - [in]
      transformation that is conceptually applied to the
      mesh before an intesection calculation.
      This transformation must have an inverse.
  */
  bool SetMeshTransform(ON_Xform xform);

  /*
  Description:
    When a mesh approximates a surface with singularities and
    an intersection event occurs on a facet with a corner at
    a singularity, surface property information is needed
    to correctly set the ON_MeshXPoint.m_S[] values.
  */
  void SetSurfaceProperties( const class ON_Surface* surface );
  void SetSurfaceProperties( const class ON_SurfaceProperties* surface_properties );
  const class ON_SurfaceProperties& SurfaceProperties() const;

  // When the ON_MeshX() constructor that takes an ON_Mesh
  // reference is used, this pointer is set to &mesh for the
  // convenience of the caller.  The ON_MeshX class never
  // uses this pointer
  const ON_Mesh* m_mesh;

  const ON_RTree* m_mesh_face_rtree;

  // These m_mesh_* fields save the information need to calculate
  // mesh plane intersections.  The values of these fields are
  // set in the ON_MeshX constructors.
  const unsigned int m_mesh_Vcnt;
  const unsigned int m_mesh_Fcnt;
  
  const class ON_3dPoint*  m_mesh_dV;
  const class ON_3fPoint*  m_mesh_fV;
  const class ON_2dPoint*  m_mesh_S;
  const class ON_MeshFace* m_mesh_F;

  ON_BoundingBox m_mesh_bbox;

  // The value of the mesh_id parameter passed to the
  // ON_MeshX() constructor is saved in m_mesh_id
  // and the value is assigned to ON_MeshX.m_mesh_id
  // when intersection events are added.
  const ON__UINT_PTR m_mesh_id;

  // The ON_MeshX constructor sets m_tag to zero.
  // Nothing in opennurbs modifies or uses this value.
  ON__UINT_PTR m_tag;

protected:
  unsigned char m_reserved1[7];

  unsigned char m_bHaveXform;
  ON_Xform m_xform;
  ON_Xform m_inverse_xform;

  double m_bc_tolerance; // default is ON_EPSILON
  ON_SurfaceProperties m_surface_properties;

private:
  double m_reserved2[4];

  // no implementation
  ON_MeshX(const ON_MeshX&) = delete;
  ON_MeshX& operator=(const ON_MeshX&) = delete;
};

class ON_CLASS ON_MeshXPlane : public ON_MeshX
{
public:

  /*
  Parameters:
    mesh - [in]
      mesh.MeshFaceTree(true) is used to get a mesh face rtree.
    mesh_id - [in]
      Value of that is assigned to ON_MeshXPoint.m_mesh_id.
  */
  ON_MeshXPlane(    
    const ON_Mesh& mesh,
    ON__UINT_PTR mesh_id
    );

  ON_MeshXPlane(    
    const ON_Mesh& mesh,
    const ON_RTree& mesh_face_rtree,
    ON__UINT_PTR mesh_id
    );

  ON_MeshXPlane(    
    unsigned int vertex_count,
    const class ON_3dPoint* mesh_dV,
    const class ON_3fPoint* mesh_fV,
    const class ON_2dPoint* mesh_S,
    unsigned int face_count,
    const class ON_MeshFace* mesh_F,
    const ON_RTree& mesh_face_rtree,
    ON__UINT_PTR mesh_id
    );

  ~ON_MeshXPlane();

  /*
  Returns:
    True if an events should be added when an entire mesh
    facet is in the plane.
  Remarks:
    The constructor default is false.
  */
  bool IncludeOverlapFacets() const;
  void SetIncludeOverlapFacets( bool bIncludeOverlapFacets );

  /*
  Returns:
    True if an event should be added when the intersection
    of the plane and a mesh facet is an isolated vertex point.
  Remarks:
    The constructor default is false.
  */
  bool IncludeVertexPoints() const;
  void SetIncludeVertexPoints( bool bIncludeVertexPoints );

  /*
  Returns:
    True if an event should be added when the intersection
    of the plane and a mesh facet is a vertex point.
  Remarks:
    The constructor default is false.
  */
  bool IncludeDegenerateEdgePoints() const;
  void SetIncludeDegenerateEdgePoints( bool bIncludeDegenerateEdgePoints );

  /*
  Parameters:
    plane_eqn - [in]
    plane_eqn_zero_tolerance - [in] (use ON_UNSET_VALUE when in doubt)
      If fabs(plane_eqn.ValueAt(P)) <= plane_eqn_zero_tolerance,
      then P is considered to be on the plane.  If the input value of
      plane_eqn_zero_tolerance is < 0.0 or not valid, then
      plane_eqn.ZeroTolerance() is used.  If the input value is 0.0,
      then no tolerance is used.
    bc_tolerance - [in] (use 0.0 when in doubt)
      if a barycentric coordinate is < max(bc_tolerance,ON_EPSILON),
      then the barycentric coordinate is set to zero.
    plane_id - [in]
      Value of that is assigned to ON_MeshXPoint.m_other_id.
  Remarks:
    This function is not thread safe.  If you want to use multiple threads
    to intersect multiple planes with the same mesh, then create a local
    ON_MeshXPlane class for each thread.
  */
  unsigned int IntersectPlane( 
    ON_PlaneEquation plane_eqn,
    double plane_eqn_zero_tolerance,
    unsigned int plane_id,
    ON_MeshXPointPool& mxpointpool
    );

private:
  ON__UINT_PTR m_reserved;

  friend class ON_MeshXPlaneCache;
  class ON_MeshXPlaneCache* m_mxp_cache;
  class ON_MeshXPlaneCache* MeshXPlaneCache() const;

  // no implementation
  ON_MeshXPlane(const ON_MeshXPlane&);
  ON_MeshXPlane& operator=(const ON_MeshXPlane&);
};


class ON_CLASS ON_MeshXLine : public ON_MeshX
{
public:

  /*
  Parameters:
    mesh - [in]
      mesh.MeshFaceTree(true) is used to get a mesh face rtree.
    mesh_id - [in]
      Value of that is assigned to ON_MeshXPoint.m_mesh_id.
  */
  ON_MeshXLine(    
    const ON_Mesh& mesh,
    ON__UINT_PTR mesh_id
    );

  ON_MeshXLine(    
    const ON_Mesh& mesh,
    const ON_RTree& mesh_face_rtree,
    ON__UINT_PTR mesh_id
    );

  ON_MeshXLine(    
    unsigned int vertex_count,
    const class ON_3dPoint* mesh_dV,
    const class ON_3fPoint* mesh_fV,
    const class ON_2dPoint* mesh_S,
    unsigned int face_count,
    const class ON_MeshFace* mesh_F,
    const ON_RTree& mesh_face_rtree,
    ON__UINT_PTR mesh_id
    );

  ~ON_MeshXLine();

  /*
  Parameters:
    line - [in]
    tolerance - [in]
      If the input value of tolerance is < 0.0 or not valid, then
      0.0 is used.
    line_id - [in]
      Value of that is assigned to ON_MeshXPoint.m_other_id.
  Remarks:
    This function is not thread safe.  If you want to use multiple threads
    to intersect multiple lines with the same mesh, then create a local
    ON_MeshXLine class for each thread.
  */
  unsigned int IntersectLine( 
    const ON_Line& L,
    double tolerance,
    unsigned int line_id,
    ON_MeshXPointPool& mxpointpool
    );

  unsigned int IntersectLine( 
    ON_3dPoint A,
    ON_3dPoint B,
    double tolerance,
    unsigned int line_id,
    ON_MeshXPointPool& mxpointpool
    );

private:
  ON__UINT_PTR m_reserved;

  friend class ON_MeshXPlaneCache;
  class ON_MeshXLineCache* m_mxl_cache;
  class ON_MeshXLineCache* MeshXLineCache() const;

  // no implementation
  ON_MeshXLine(const ON_MeshXLine&);
  ON_MeshXLine& operator=(const ON_MeshXLine&);
};

class ON_CLASS ON_MeshThicknessAnalysisPoint
{
public:
  ON_MeshThicknessAnalysisPoint() = default;
  ~ON_MeshThicknessAnalysisPoint() = default;
  ON_MeshThicknessAnalysisPoint(const ON_MeshThicknessAnalysisPoint&) = default;
  ON_MeshThicknessAnalysisPoint& operator=(const ON_MeshThicknessAnalysisPoint&) = default;

  static const ON_MeshThicknessAnalysisPoint UnsetPoint;

public:
  // m_mesh_context = value of the ON_MeshThicknessAnalysis::AddMesh() mesh_context parameter.
  ON__INT_PTR m_mesh_context = 0;

  // m_mesh_id = value returned by ON_MeshThicknessAnalysis::AddMesh().
  unsigned int m_mesh_id = ON_UNSET_UINT_INDEX;

  // m_mesh_vertex_index = ON_Mesh vertex index.
  unsigned int m_mesh_vertex_index = ON_UNSET_UINT_INDEX;
  
  // If the distance from the vertex to the other side was greater than the max_distance
  // parameter in the CalculateVertexDistance call, then 0 <= m_distance = ON_UNSET_POSITIVE_VALUE
  double m_distance = ON_UNSET_POSITIVE_VALUE;

  // Location of the mesh vertex
  ON_3dPoint m_vertex_point = ON_3dPoint::UnsetPoint;

  // Location of the point on the other side
  ON_3dPoint m_closest_point = ON_3dPoint::UnsetPoint;
};

class ON_CLASS ON_MeshThicknessAnalysisVertexIterator
{
public:
  ON_MeshThicknessAnalysisVertexIterator() = default;
  ~ON_MeshThicknessAnalysisVertexIterator() = default;
  ON_MeshThicknessAnalysisVertexIterator(const ON_MeshThicknessAnalysisVertexIterator&) = default;
  ON_MeshThicknessAnalysisVertexIterator& operator=(const ON_MeshThicknessAnalysisVertexIterator&) = default;

  ON_MeshThicknessAnalysisVertexIterator(
    const class ON_MeshThicknessAnalysis& mta
    );

  static ON_MeshThicknessAnalysisVertexIterator Create(
    const class ON_MeshThicknessAnalysis& mta
    );

public:
  /*
  Description:
    Resets the iterator so the next call to GetNextPoint() will return the first point.
  Returns:
    Number of analysis points in ON_MeshThicknessAnalysis.
  */
  unsigned int Reset();

  /*
  Description:
    Initializes the iterator and gets the first analysis point (first vertex in first mesh).
  Parameters:
    point - [out]
      Analysis point returned here.
  Returns:
    true:
      point was returned.
    false:
      no analysis points are in the ON_MeshThicknessAnalysisVertexIterator.
      point = ON_MeshThicknessAnalysisPoint::UnsetPoint.
  */
  bool GetFirstPoint(
    ON_MeshThicknessAnalysisPoint& point
    );

  /*
  Description:
    Increments the iterator to the next point and gets that point.
  Parameters:
    point - [out]
      Analysis point returned here.
  Returns:
    true:
      point was returned.
    false:
      no more analysis points are available.
      point = ON_MeshThicknessAnalysisPoint::UnsetPoint.
  */
  bool GetNextPoint(
    ON_MeshThicknessAnalysisPoint& point
    );

  /*
  Description:
    Does not change the iterator state.
  Parameters:
    point - [out]
      Analysis point returned here.
  Returns:
    true:
      The current point was returned.
    false:
      No point is available.  Either the ON_MeshThicknessAnalysis has no points
      or the iterator has run through all of them.
      point = ON_MeshThicknessAnalysisPoint::UnsetPoint.
  */
  bool GetCurrentPoint(
    ON_MeshThicknessAnalysisPoint& point
    ) const;

  /*
  Description:
    Initializes the iterator and returns the first analysis point (first vertex in first mesh).
  Returns:
    First analysis point.  If none are available, then ON_MeshThicknessAnalysisPoint::UnsetPoint
    is returned.
  */
  ON_MeshThicknessAnalysisPoint FirstPoint();

  /*
  Description:
    Increments the iterator to the next point and returns that point.
  Returns:
    Next analysis point.  If none are available, then ON_MeshThicknessAnalysisPoint::UnsetPoint
    is returned.
  */
  ON_MeshThicknessAnalysisPoint NextPoint();

  /*
  Returns:
    Current analysis point.  If none are available, then ON_MeshThicknessAnalysisPoint::UnsetPoint
    is returned.
  */
  ON_MeshThicknessAnalysisPoint CurrentPoint() const;

  /*
  Returns:
    Number of meshes in the ON_MeshThicknessAnalysis.
  */
  unsigned int MeshCount() const;

  /*
  Returns:
    Total number of points in the ON_MeshThicknessAnalysis.  
    This is the sum of the vertex counts from each mesh.
  */
  unsigned int PointCount() const;

  /*
  Returns:
    Zero based index of the current point. ON_UNSET_UINT_INDEX is returned if no point is available.
  */
  unsigned int CurrentPointIndex() const;

#if defined(ON_HAS_RVALUEREF)
  const ON_MeshThicknessAnalysis MeshThicknessAnalysis() const;
#endif

private:
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4251 ) 
  // C4251: 'ON_MeshThicknessAnalysisVertexIterator::m_sp' : class 'std::shared_ptr<ON_MeshThicknessAnalysisImpl>' 
  //        needs to have dll-interface to be used by clients ...
  // m_sp is private and all code that manages m_sp is explicitly implemented in the DLL.
private:
  std::shared_ptr< class ON_MeshThicknessAnalysisImpl > m_sp;
#pragma ON_PRAGMA_WARNING_POP

private:
  ON_FixedSizePoolIterator m_it;

  // m_current_mesh_index = ON_MeshThicknessAnalysis input mesh index
  unsigned int m_current_mta_mesh_index = ON_UNSET_UINT_INDEX;

  void* m_current_point = nullptr;
  ON__UINT_PTR m_current_mesh_context = ON_MeshThicknessAnalysisPoint::UnsetPoint.m_mesh_context;
  unsigned int m_current_mesh_id = ON_MeshThicknessAnalysisPoint::UnsetPoint.m_mesh_id;
  unsigned int m_current_mesh_vertex_index = ON_MeshThicknessAnalysisPoint::UnsetPoint.m_mesh_vertex_index;
};

class ON_CLASS ON_MeshThicknessAnalysis
{
public:
  ON_MeshThicknessAnalysis() = default;

  ~ON_MeshThicknessAnalysis() = default;

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_MeshThicknessAnalysis( ON_MeshThicknessAnalysis&& ) ON_NOEXCEPT;
  // rvalue assignment operator
  ON_MeshThicknessAnalysis& operator=( ON_MeshThicknessAnalysis&& );
#endif

  /*
  Description:
    Adds a mesh that will be included in the thickness calculation.
    Multiple meshes can be added. The final calculation will treat
    the collection of mesh faces and vertices as a single mesh.
  Parameters:
    mesh - [in]
      mesh to add.
    mesh_context - [in]
      A "context" value passed back on ON_MeshThicknessAnalysisPoint.m_mesh_context
      when an ON_MeshThicknessAnalysisVertexIterator is use to iterate through
      the results.  If you are certain mesh will persist until you are finished
      iterating over the results, (ON__UINT_PTR)mesh can be passed to provide
      quick access to mesh during iteration.
    terminator - [in] (can be nullptr)
      optional terminator to check.
      This process is generally fast. 
      If your mesh has millions of vertices and faces, you may want to use a terminator.
  Returns:
    >0:
      The mesh id value for this mesh in this ON_MeshThicknessAnalysis
      The first mesh has id = 1, the second id = 2, and so on.
    0: 
      Invalid input nothing added.    
  */
  unsigned int AddMesh(
    const ON_Mesh* mesh,
    ON__UINT_PTR mesh_context,
    ON_Terminator* terminator
    );

  /*
  Returns:
    Total number of mesh in this ON_MeshThicknessAnalysis.
  */
  unsigned int MeshCount() const;

  /*
  Returns:
    Total number of vertices in this ON_MeshThicknessAnalysis.
  */
  unsigned int PointCount() const;

  /*
  Returns:
    Value of maximum distance used to set vertex distances
  */
  double MaximumDistance() const;

  /*
  Returns:
    The sharp angle used to set vertex distances
  */
  double SharpAngleRadians() const;

  /*
  Returns
    true if vertex distance information is up to date. 
    (Set by calling CalculateVertexDistances().)
  */
  bool VertexDistancesSet() const;


  /*
  Description:
    Computes the shortest distance from each vertex to a facet on the
    "other side".  When that distance is <=  max_distance, the information
    is saved.
  Parameters:
    mesh - [in]
      mesh to add.
    max_distance - [in]
      Distances larger than this value will be ignored.  Setting this parameter to a value
      a bit larger than any distance you care about can dramatically reduce the amount
      of time spent in CalculateVertexDistances().
    sharp_angle_radians - [in]
      If sharp_angle_radians >= 0.0 and < 0.5*ON_PI, it is used as the sharp angle.
      Otherwise, 89 degrees (89.0/180.0*ON_PI radians) is used as the sharp angle.
      When the interior angle between triangles sharing a common vertex is <= sharp angle,
      the vertex is assigned a thickness of 0.  
      Smaller values of sharp_angle_radians reduce the number of adjacent vertices 
      that are assigned a thickness of 0.
    progress_reporter - [in] (can be nullptr)
      optional progress reporter.
    terminator - [in] (can be nullptr)
      optional terminator.
  Returns:
    true:
      Calculation finished.
    false:
      Calculation failed or was terminated.
  */
  bool CalculateVertexDistances(
    double max_distance,
    double sharp_angle_radians,
    ON_ProgressReporter* progress_reporter,
    ON_Terminator* terminator
    );

  ON_MeshThicknessAnalysisVertexIterator Iterator() const;

private:
  // Prohibit copy construction
  ON_MeshThicknessAnalysis(const ON_MeshThicknessAnalysis&) = delete;

private:
  // Prohibit operator=
  ON_MeshThicknessAnalysis& operator=(const ON_MeshThicknessAnalysis&) = delete;

private:
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4251 ) 
  // C4251: 'ON_MeshThicknessAnalysisVertexIterator::m_sp' : class 'std::shared_ptr<ON_MeshThicknessAnalysisImpl>' 
  //        needs to have dll-interface to be used by clients ...
  // m_sp is private and all code that manages m_sp is explicitly implemented in the DLL.
  friend class ON_MeshThicknessAnalysisVertexIterator;
  std::shared_ptr< class ON_MeshThicknessAnalysisImpl > m_sp;
  ON_MeshThicknessAnalysis( std::shared_ptr< class ON_MeshThicknessAnalysisImpl > & );

#pragma ON_PRAGMA_WARNING_POP
};

#endif

#endif
