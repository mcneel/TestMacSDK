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

#if !defined(OPENNURBS_PLUS) || !defined(OPENNURBS_PLUS_INC_) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_SQUISH_INC_)
#define OPENNURBS_PLUS_SQUISH_INC_

class ON_CLASS ON_SquishConstraint
{
public:
  // 0 = none
  // 1 = 2d point: (m_v[0],m_v[1])
  // 2 = 2d line:  0 = m_v[0]*x + m_v[1]*y + m_v[2]
  unsigned int m_constraint_type;

private:
  unsigned int m_reserved;

public:
  double m_v[4];
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_SquishConstraint>;
#endif

class ON_CLASS ON_SquishConstraints
{
public:
  // m_constraints[] is a list of various constraints
  // you want to apply to some of the mesh's vertices.
  ON_SimpleArray<ON_SquishConstraint> m_constraints;

  // m_constrained_vertices[] is a list that specifies
  // mesh vertices and the constraint. The ON_2dex.i value
  // is a mesh.m_V[] index and the ON_2dex.j value
  // m_constraints[] index.
  ON_SimpleArray<ON_2dex> m_constrained_vertices;
};

// 0 = free
// 1 = stretch mostly
// 2 = stretch only (no compression)
// 3 = compress mostly
// 4 = compress only (no stretching)
// 5 = custom
#pragma region RH_C_SHARED_ENUM [ON_SquishDeformation] [Rhino.Geometry.SquishDeformation] [int]
/// <summary>
/// The types of deformation allowed in the squish process
/// </summary>
enum class ON_SquishDeformation : int
{
  /// <summary>
  /// No preference for compression or stretching.
  /// </summary>
  Free = 0,
  /// <summary>
  /// Strong bias for expansion when the pattern is deformed into the 3-D shape.
  /// </summary>
  StretchMostly = 1,
  /// <summary>
  /// Absolutely no compression when the pattern is deformed into the 3-D shape.
  /// </summary>
  StretchOnly = 2,
  /// <summary>
  /// Strong bias for compression when the pattern is deformed into the 3-D shape.
  /// </summary>
  CompressMostly = 3,
  /// <summary>
  /// Absolutely no expansion when the pattern is deformed into the 3-D shape.
  /// </summary>
  CompressOnly = 4,
  /// <summary>
  /// The Custom option lets you set the parameters used by the custom deformations. There are four parameters you can set:
  /// BndStretch, BndCompress, InteriorStretch, InteriorCompress
  /// The default value for these parameters is 1 and they can be set to any positive number. 
  /// A larger value reduces the amount of the specified deformation compared to what happens when all 
  /// four parameters are equal. For example, if you want to severely limit interior expansion, 
  /// you could do something like:
  /// BndStretch = 1
  /// BndCompress = 1
  /// InteriorStretch = 1
  /// InteriorCompress = 100
  /// If you want to preserve boundary lengths you could use the settings:
  /// BndStretch = 10
  /// BndCompress = 10
  /// InteriorStretch = 1
  /// InteriorCompress = 1
  /// </summary>
  Custom=5
};
#pragma endregion

#pragma region RH_C_SHARED_ENUM [ON_SquishFlatteningAlgorithm] [Rhino.Geometry.SquishFlatteningAlgorithm] [int]
/// <summary>
/// Represents the type of flattening to use
/// </summary>
enum class ON_SquishFlatteningAlgorithm : unsigned int
{
  /// <summary>
  /// geometric flattening algorithm: (scale independent)the "spring" constant is
  ///     proportional to 1/L^2 and the result is independent of scale.
  /// </summary>
  Geometric = 0,
  /// <summary>
  /// physical stress flattening algorithm: (scale dependent) the "spring" constant is 
  ///     proportional to 1/L.
  /// </summary>
  PhysicalStress = 1
};
#pragma endregion

class ON_CLASS ON_SquishParameters
{
public:
  ON_SquishParameters();

  void Dump( 
    ON_TextLog& text_log
    ) const;

  void DumpSpringConstants( 
    ON_TextLog& text_log
    ) const;

  void DumpRelativeTolerances( 
    ON_TextLog& text_log
    ) const;

  /*
  Description:
    A simplified interface for setting the m_*_stretch_c 
    and m_*_compress_c constants.
  Parameters:
    boundary_bias - [in]  0.0 to 1.0
      0.0: boundary and interior treated the same
      1.0: strongest bias to preserving boundary lengths at the expense 
           of interior distortion.
    deformation_bias - [in] -1.0 to 1.0
      -1.0: strongest bias in favor of compression.
       0.0: no preference between compression and stretching
       1.0: strongest bias in favor of stretching
  */
  void SetSpringConstants(
    double boundary_bias,
    double deformation_bias
    );

  /*
  Description:
    See if the spring constants could have been set with
    SetSpringConstants().
  Parameters:
    boundary_bias - [out]
    deformation_bias - [out]
  Returns:
    If the spring constants have values that could be set by
    calling SetSpringConstants(), then boundary_bias and
    deformation_bias are set to those values and this function
    returns true.
    Otherwise, boundary_bias and deformation_bias are set to 0.0
    and false is returned.
  */
  bool GetSpringConstants(
    double* boundary_bias,
    double* deformation_bias
    ) const;

  void SetDeformation(ON_SquishDeformation deformation, 
    bool bPreserveBoundary,
    double boundary_stretch_c=0.0, double boundary_comopress=0.0,
    double interior_stretch_c=0.0, double interior_compress_c=0.0);

  bool Write( 
    class ON_BinaryArchive& binary_archive
    ) const;

  bool Read( 
    class ON_BinaryArchive& binary_archive
    );

  // spring constant for compressed boundary edges is
  // m_boundary_compress_c/(rest length)
  double m_boundary_stretch_c;
  double m_boundary_compress_c;
  double m_interior_stretch_c;
  double m_interior_compress_c;
  double m_diagonal_stretch_c;
  double m_diagonal_compress_c;

  // ***** these are unused *****
  // If a m_..._stretch_rel_tol parameter is > 1.0, then the 
  // mesh will be automatically split in areas where the
  // relative stretch exceeds the specified relative tolerance.
  // If a m_..._compress_rel_tol parameter is > 0.0 and < 1.0,
  // then the mesh will be automatically split in areas where
  // the relative compression exceeds the specified relative
  // tolerance.
  double m_boundary_stretch_rel_tol; // unused
  double m_boundary_compress_rel_tol; // unused
  double m_interior_stretch_rel_tol; // unused
  double m_interior_compress_rel_tol; // unused
  double m_diagonal_stretch_rel_tol; // unused
  double m_diagonal_compress_rel_tol; // unused

  /*
  If -1.0 <= m_absolute_limit < 0.0, then then an absolute
  compression limit is applied so that
  (2d length)/(3d length) >= fabs(m_absolute_limit).
  In particular, if m_absolute_limit = -1.0, then no compression
  is permitted (2d length) >= (3d length).

  If 0.0 < m_absolute_limit <= 1.0 then then an absolute
  stretching limit is applied so that
  (2d length)/(3d length) <= 1/fabs(m_absolute_limit).

  Examples:
    m_absolute_limit
     1.0: no stretching, (2d length) <= 1.0*(3d length)
     0.5: cap on stretching, 0.5*(2d length) <= (3d length)
    -0.5: cap on compression, (2d length) >= 0.5*(3d length)
    -1.0: no compression, (2d length) >= 1.0*(3d length)
  */
  double m_absolute_limit;

  // 0 = geometric flattening algorithm
  //     (scale independent)the "spring" constant is
  //     proportional to 1/L^2 and the result is 
  //     independent of scale.
  // 1 = physical stress flattening algorithm
  //     (scale dependent) the "spring" constant is 
  //     proportional to 1/L.
  unsigned int m_material;

  // If the mesh has coincident vertices and m_bPreserveTopology
  // is true, then the flattening is based on the mesh's
  // topology and coincident vertices will remain coincident.
  // Otherwise coincident vertices are free to move apart.
  bool m_bPreserveTopology;

  // If m_bEnableMapBack is true, then ON_SquishMesh()
  // will save extra information on the squished mesh so 
  // 3d points and curves near the input mesh can be mapped
  // to the squished mesh and 2d points and curves on the 
  // squished mesh can be mapped back to the 3d mesh.
  bool m_bSaveMapping;

  /////////////////////////////////////////////////////////
  //
  // These settings determine how the solution is calculated
  // Leave them as is unless you are debugging.
  //

  // Default is false.  Set to true to debug problem cases.
  bool m_bNewtonPreprocess;

  // Set to true if the initial guess has at least 20% of the
  // triangles in the wrong orientation.
  bool m_bAreaPreprocess;

  // Enables the critical optimization step.
  // Default is true.  Setting it to false is
  // used for debugging the results of the setup
  // and preprocessing steps.
  bool m_bOptimize;

  // Set to true to tune up the result.
  bool m_bLengthPostprocess;

  // Set to bias deformation to happen in regions with
  // relatively more Gaussian curvature.  The mesh's m_K[]
  // values must be set for this setting to have an effect.
  bool m_bGaussianBias;

private:
  bool m_reserved0[5];
  double m_reserved1[16];
  void* m_reserved2[2];

public:
  const static ON_SquishParameters DefaultValue;
};

class ON_CLASS ON_SquisherImpl
{
public:
  ON_SquisherImpl() = default;
  virtual ~ON_SquisherImpl() = default;

  static bool Is2dPatternSquished(const ON_Geometry* geometry);

  void Dump(
    ON_TextLog& text_log
  ) const;

  virtual ON_Mesh* SquishMesh(const ON_SquishParameters& squishParams, const ON_Mesh& mesh3d) = 0;
  virtual ON_Mesh* SquishMesh(const ON_SquishParameters& squishParams, const ON_Mesh& mesh3d,
    const ON_SimpleArray<const ON_Geometry*>* marks, ON_SimpleArray<ON_Geometry*>* squished_marks) = 0;
  virtual ON_Brep* SquishSurface(const ON_SquishParameters& squishParams, const ON_Surface& surface3d) = 0;
  virtual ON_Brep* SquishSurface(const ON_SquishParameters& squishParams, const ON_Surface& surface3d,
    const ON_SimpleArray<const ON_Geometry*>* marks, ON_SimpleArray<ON_Geometry*>* squished_marks) = 0;
  virtual bool SquishPoint(
    ON_3dPoint point3d,
    ON_MESH_POINT& point2d
  ) const = 0;
  virtual ON_PolylineCurve* SquishCurve(
    const ON_Curve& curve3d) const = 0;
  virtual ON_TextDot* SquishTextDot(const ON_TextDot& textDot) const = 0;
  virtual ON_PointCloud* SquishPointCloud(
    const ON_PointCloud& pc3) const = 0;
  virtual const ON_Mesh* GetMesh2d() const = 0;
  virtual const ON_Mesh* GetMesh3d() const = 0;
  virtual int GetLengthConstrained3dLines(ON_SimpleArray<ON_Line>* edges) const = 0;
  virtual int GetLengthConstrained2dLines(ON_SimpleArray<ON_Line>* edges) const = 0;
  virtual int GetAreaConstrainedTrianglesIndices(ON_SimpleArray<ON_MeshFace>* faces) const = 0;
  virtual ON_Plane GetPlane() const = 0;
  virtual void DumpEnergyDebugText(ON_TextLog& text_log) const = 0;
  virtual void SaveOriginalObjectIdAndComponentIndex(const ON_UUID obj_id, const ON_COMPONENT_INDEX obj_ci,
    ON_Geometry* squished_geometry) = 0;
  static bool SquishBack2dMarks(const ON_Geometry* squished_geometry,
    const ON_SimpleArray<const ON_Geometry*>* marks,
    ON_SimpleArray<ON_Geometry*>* squished_marks);
  static void DumpSquishInfoText(const ON_Geometry* squished_geometry, ON_TextLog& text_log,
    const wchar_t* objid_locale_str, const wchar_t* meshcount_locale_str);

private:
  ON__UINT_PTR m_reserved = 0;
};

class ON_CLASS ON_Squisher
{
public:
  ON_Squisher();

  virtual ~ON_Squisher();

  static bool Is2dPatternSquished(const ON_Geometry* geometry);

  void Dump(
    ON_TextLog& text_log
  ) const;

  /*
  Description:
    Use to flatten a mesh.
  Remarks:
    The ON_Squisher class is NOT designed
    to support more than one squish operation.
  */
  ON_Mesh* SquishMesh(const ON_SquishParameters& squishParams, const ON_Mesh& mesh3d);

  /*
  Description:
    Use to flatten a mesh and marks.
  Remarks:
    It is the responsibility of the called to delete the geometry returned in the squished_marks array.
    The ON_Squisher class is NOT designed
    to support more than one squish operation.
  */
  ON_Mesh* SquishMesh(const ON_SquishParameters& squishParams, const ON_Mesh& mesh3d,
    const ON_SimpleArray<const ON_Geometry*>* marks, ON_SimpleArray<ON_Geometry*>* squished_marks);

  /*
Description:
  Use to flatten a surface.
Remarks:
  The TL_SquishMesh class is NOT designed for
  to support more than one squish operation.
*/
  ON_Brep* SquishSurface(const ON_SquishParameters& squishParams, const ON_Surface& surface3d);

  /*
  Description:
    Use to flatten a surface and marks.
  Remarks:
    It is the responsibility of the called to delete the geometry returned in the squished_marks array.
    The TL_SquishMesh class is NOT designed for
    to support more than one squish operation.
  */
  ON_Brep* SquishSurface(const ON_SquishParameters& squishParams, const ON_Surface& surface3d,
    const ON_SimpleArray<const ON_Geometry*>* marks, ON_SimpleArray<ON_Geometry*>* squished_marks);

  /*
Description:
  After you have flattened a mesh or surface, you can
  call this function to flatten a 3d curve that is near
  the 3d mesh or 3d surface.
Parameters:
  point3d - [in] 3d point near the 3d mesh or 3d surface.
  point2d - [out 2d point on the squished mesh.
*/
  bool SquishPoint(
    ON_3dPoint point3d,
    ON_MESH_POINT& point2d
  ) const;

  /*
  Description:
    After you have flattened a mesh or surface, you can
    call this function to flatten a 3d curve that is near
    the 3d mesh or 3d surface.
  Parameters:
    curve3d - [in] 3d curve near the 3d mesh or 3d surface.
    curve2d - [in] if not NULL, the 2d result will be stored
                   in this curve.  If NULL, a polyline curve
                   will be created.
  */
  ON_PolylineCurve* SquishCurve(
    const ON_Curve& curve3d) const;

  /*
  Description:
    After you have flattened a mesh or surface, call this function to map a TextDot on or near
    the surface to the resulting 2d surface
  Parameters:
    textDot - [in] the textDot to squish
    squished_dot - [in] if not NULL, the 2d result will be stored
                   in this TextDot.  If NULL, a TextDot
                   will be created.
  */
  ON_TextDot* SquishTextDot(const ON_TextDot& textDot) const;

  /*
  Description:
    After you have flattened a mesh or surface, you can
    call this function to flatten a PointCloud that is near
    the 3d mesh or 3d surface.
  Parameters:
    pc3 - [in] PointCloud near the 3d surface.
    squished_cloud - [in] if not NULL, the 2d result will be stored
                   in this cloud.  If NULL, a new cloud will be returned  */
  ON_PointCloud* SquishPointCloud(
    const ON_PointCloud& pc3) const;

  /* gets a const pointer to the 2d squished mesh, or NULL if it doesn't exist */
  const ON_Mesh* GetMesh2d() const;

  /* gets a const pointer to the 3d mesh used for squishing, or NULL if it isn't stored */
  const ON_Mesh* GetMesh3d() const;

  // Gets lines at the position of the mesh edges and diagonals that were constrained during the squish, in the 3d mesh.
  // The line at any index here corresponds to the same line in GetLengthConstrained2dLines.
  int GetLengthConstrained3dLines(ON_SimpleArray<ON_Line>* edges) const;

  // Gets lines at the position of the mesh edges and diagonals that were constrained during the squish, in the 2d mesh.
  // The line at any index here corresponds to the same line in GetLengthConstrained3dLines.
  int GetLengthConstrained2dLines(ON_SimpleArray<ON_Line>* edges) const;

  ON_DEPRECATED_MSG("This method has been renamed GetLengthContrained3dLines")
  int GetMesh3dEdges(ON_SimpleArray<ON_Line>* edges) const;

  ON_DEPRECATED_MSG("This method has been renamed GetLengthContrained2dLines")
  int GetMesh2dEdges(ON_SimpleArray<ON_Line>* edges) const;

  // Gets mesh vertex indices for the triangular faces that were constrained during the squish.
  // Indices can be used in both the 2d and 3d mesh vertices arrays.
  int GetAreaConstrainedTrianglesIndices(ON_SimpleArray<ON_MeshFace>* faces) const;

  // Gets the plane that was used to flatten the surface, or ON_Plane::UnsetPlane if local estimates were used.
  ON_Plane GetPlane() const;

  // Dumps the text for debugging the energy optimizer
  void DumpEnergyDebugText(ON_TextLog& text_log) const;

  // Saves the original object id and component index in TL_SquishMapBackInfo user data
  void SaveOriginalObjectIdAndComponentIndex(const ON_UUID obj_id, const ON_COMPONENT_INDEX obj_ci,
    ON_Geometry* squished_geometry);

  // Squish back the marks
  static bool SquishBack2dMarks(const ON_Geometry* squished_geometry,
    const ON_SimpleArray<const ON_Geometry*>* marks,
    ON_SimpleArray<ON_Geometry*>* squished_marks);

  // Dumps the text for the SquishInfo command
  static void DumpSquishInfoText(const ON_Geometry* squished_geometry, ON_TextLog& text_log,
    const wchar_t* objid_locale_str, const wchar_t* meshcount_locale_str);

private:
  ON_SquisherImpl* m_impl;
  ON__UINT_PTR m_reserved = 0;
};

/*
  Description:
    Use to flatten a mesh.
  Parameters:
    mesh - [in]
      3d mesh to flatten.
    squish_parameters - [in]
      parameters used to calculate the 2d result.
    squish_constraints - [in]
      UNUSED AT THIS TIME
    squished_mesh - [in]
      optional parameter that can be used to
      specify where the 2d result should be saved.
  Returns:
    If successful, a flattened mesh.
    Null if not successful.
*/
ON_DECL
ON_Mesh* ON_SquishMesh( 
  const ON_Mesh* mesh,
  const ON_SquishParameters* squish_parameters,
  const ON_SquishConstraints* squish_constraints,
  ON_Mesh* squished_mesh
  );
/*
  Description:
    Use to flatten a mesh.
  Parameters:
    mesh - [in]
      3d mesh to flatten.
    squish_parameters - [in]
      parameters used to calculate the 2d result.
    squish_constraints - [in]
      UNUSED AT THIS TIME
    squished_mesh - [in]
      optional parameter that can be used to
      specify where the 2d result should be saved.
    marks - [in] a list of ON_Point, ON_TextDot, ON_PointCloud, and ON_Curve
      objects to be squished
    squished_marks - [out] The results of ma;pping the marks back to the 3d unsquished
      surface. A failure produces a null entry in the array. It is the responsibility
      of the caller to delete the geometry in squished_marks
    squished_mesh - [in]
      optional parameter that can be used to
      specify where the 2d result should be saved.
  Returns:
    If successful, a flattened mesh.
    Null if not successful.
*/
ON_DECL
ON_Mesh* ON_SquishMesh(
  const ON_Mesh* mesh,
  const ON_SquishParameters* squish_parameters,
  const ON_SquishConstraints* squish_constraints,
  const ON_SimpleArray<const ON_Geometry*>* marks,
  ON_SimpleArray<ON_Geometry*>* squished_marks,
  ON_Mesh* squished_mesh
  );

/*
  Description:
    Use to flatten a mesh.
  Parameters:
    surface - [in]
      3d surface to flatten.
    squish_parameters - [in]
      parameters used to calculate the 2d result.
    squished_brep - [in]
      optional parameter that can be used to
      specify where the 2d result should be saved.
  Returns:
    If successful, a flattened brep.
    Null if not successful.
*/
ON_DECL
ON_Brep* ON_SquishSurface(
  const ON_Surface* surface,
  const ON_SquishParameters* squish_parameters,
  ON_Brep* squished_brep
);

/*
  Description:
    Use to flatten a mesh.
  Parameters:
    surface - [in]
      3d surface to flatten.
    squish_parameters - [in]
      parameters used to calculate the 2d result.
    squished_brep - [in]
      optional parameter that can be used to
      specify where the 2d result should be saved.
    marks - [in] a list of ON_Point, ON_TextDot, ON_PointCloud, and ON_Curve
      objects to be squished
    squished_marks - [out] The results of ma;pping the marks back to the 3d unsquished
      surface. A failure produces a null entry in the array. It is the responsibility
      of the caller to delete the geometry in squished_marks
  Returns:
    If successful, a flattened brep.
    Null if not successful.
*/
ON_DECL
ON_Brep* ON_SquishSurface(
  const ON_Surface* surface,
  const ON_SquishParameters* squish_parameters,
  const ON_SimpleArray<const ON_Geometry*>* marks,
  ON_SimpleArray<ON_Geometry*>* squished_marks,
  ON_Brep* squished_brep);


/*
  Description:
    Maps geometry from a squished geometry back to the original mesh or surface.
  Parameters:
    squished_geometry - [in] the geometry resulting from a call to SquishMesh or SquishSurface
      3d surface to flatten.
    squish_parameters - [in]
      parameters used to calculate the 2d result.
    squished_brep - [in]
      optional parameter that can be used to
      specify where the 2d result should be saved.
    marks - [in] a list of ON_Point, ON_TextDot, ON_PointCloud, and ON_Curve
      objects to be squished
    squishedback_marks - [out] The results of mapping the marks back to the 3d unsquished
      surface. A failure produces a null entry in the array. It is the responsibility
      of the caller to delete the geometry in squished_marks
  Returns:
    If successful, a flattened brep.
    Null if not successful.
*/
ON_DECL
bool ON_SquishBack2dMarks(
  const ON_Geometry* squished_geometry,
  const ON_SimpleArray<const ON_Geometry*>* marks,
  ON_SimpleArray<ON_Geometry*>* squished_marks);

ON_DECL
bool ON_Is2dPatternSquished(const ON_Geometry* geometry);

ON_DECL
void ON_DumpSquishInfoText(const ON_Geometry* squished_geometry, ON_TextLog& text_log,
  const wchar_t* objid_locale_str, const wchar_t* meshcount_locale_str);

#endif
