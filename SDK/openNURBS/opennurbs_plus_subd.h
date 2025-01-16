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

#if !defined(OPENNURBS_PLUS) || defined(OPENNURBS_PUBLIC)
#error This file opennurbs_plus_subd.h should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_SUBD_PLUS_INC_)
#define OPENNURBS_SUBD_PLUS_INC_

class ON_CLASS ON_SubDSurfaceInterpolator
{
public:
  ON_SubDSurfaceInterpolator() = default;
  ~ON_SubDSurfaceInterpolator() = default;
  ON_SubDSurfaceInterpolator(const ON_SubDSurfaceInterpolator&) = default;
  ON_SubDSurfaceInterpolator& operator=(const ON_SubDSurfaceInterpolator&) = default;

public:

#pragma region RH_C_SHARED_ENUM [ON_SubDSurfaceInterpolator::MaximumCounts] [Rhino.Geometry.SubDSurfaceInterpolator.MaximumCounts] [nested:uint:clsfalse]
  /// <summary>
  /// Stores maximum count values for the solver to work in reasonable time.
  /// </summary>
  enum class MaximumCounts : unsigned int
  {
    /// <summary>
    /// Interpolation requires building a solver.
    /// If the number of interpolated vertices is greater than MaximumInterpolatedVertexCount,
    /// the solver construction time can be too long for some users' comfort.
    /// Given sufficient time, memory, and CPU resources, the code will work with any value
    /// </summary>
    MaximumRecommendedInterpolatedVertexCount = 1000U
  };
#pragma endregion

  /*
  Parameters:
    subd - [in]
      Every vertex in subd will be a free vertex in the linear system.
  */
  unsigned CreateFromSubD(ON_SubD& subd);

  /*
  Parameters:
    bFreeVertexMark - [in]
      value of ON_SubDVertex.Mark() that free vertices have.
  */
  unsigned CreateFromMarkedVertices(ON_SubD& subd, bool bInterplatedVertexRuntimeMark);
  unsigned CreateFromSelectedVertices(ON_SubD& subd);
  unsigned CreateFromVertexList(ON_SubD& subd, const ON_SimpleArray<ON_COMPONENT_INDEX>& interpolated_vertices);
  unsigned CreateFromVertexList(ON_SubD& subd, const ON_SimpleArray<unsigned>& interpolated_vertices);
  unsigned CreateFromVertexList(ON_SubD& subd, const ON_SimpleArray<ON_SubDComponentPtr>& interpolated_vertices);
  unsigned CreateFromVertexList(ON_SubD& subd, const ON_SimpleArray<ON_SubDVertexPtr>& interpolated_vertices);
  unsigned CreateFromVertexList(ON_SubD& subd, const ON_SimpleArray<const ON_SubDVertex*>& interpolated_vertices);
  unsigned CreateFromVertexList(const ON_SubDComponentList& interpolated_vertices);

  /*
  Description:
    Destroys the information need to solve the interpolation.
  */
  void Clear();

  /*
  Returns:
    Number of vertices with interpolated surface points.
  */
  unsigned InterpolatedVertexCount() const;

  /*
  Returns:
    Number of vertices with fixed control net points.
  */
  unsigned FixedVertexCount() const;

  /*
  Returns:
    True if the vertex surface point is being interpolated.
  */
  bool IsInterpolatedVertex(
    const ON_SubDVertex* vertex
  ) const;

  /*
  Returns:
    True if the vertex surface point is being interpolated.
  */
  bool IsInterpolatedVertex(
    unsigned vertex_id
  ) const;

  /*
  Parameters:
    surface_points[in]
      surface_points[] is an array of InterpolatedVertexCount() points and surface_points[i] is the
      desired surface point location for the vertex VertexList()[i].Vertex().
    Updates the subd referenced by VertexList() so the corresponding surface points are surface_point[]
  */
  bool Solve(
    const ON_SimpleArray<ON_3dPoint>& surface_points
  );

  /*
  Parameters:
    surface_points [in]
      surface_points[] is an array of InterpolatedVertexCount() points and surface_points[i] is the 
      desired surface point location for the vertex VertexList()[i].Vertex().
    Updates the subd referenced by VertexList() so the corresponding surface points are surface_point[]
  */
  bool Solve(
    const ON_3dPoint* surface_points
  );

  /*
  Returns:
    If vertex is an interpolated vertex, then InterpolatedVertex() is the index of the
    vertex in the array returned by VertexList() and UniqueVertexList().
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned InterpolatedVertexIndex(const ON_SubDVertex* vertex) const;

  /*
  Returns:
    If vertex is an interpolated vertex, then InterpolatedVertex() is the index of the
    vertex in the array returned by VertexList() and UniqueVertexList().
    Otherwise ON_UNSET_UINT_INDEX is returned.
  */
  unsigned InterpolatedVertexIndex(unsigned vertex_id) const;


  /*
  Returns:
    The context assigned id. This id is provided for applications using ON_SubDSurfaceInterpolator.
    It is not inspected or used in any part of the interpolation setup or calculations.
  Remarks:
    In Rhino, when an interpolator is being used to modify a CRhinoSubDObject, 
    this id is often the Rhino object id.
  */
  const ON_UUID ContextId() const;

  /*
  Description:
    Set context assigned id. This id is provided for applications using ON_SubDSurfaceInterpolator.
    It is not inspected or used in any part of the interpolation setup or calculations.
  */
  void SetContextId(ON_UUID);

  /*
  Returns:
    List of vertices the with interpolated surface points. Vertices that are not in this 
    vertex list have fixed control net points.
    This list might be shared with other instances of ON_SubDSurfaceInterpolator.
  */
  const ON_SubDComponentList& VertexList() const;

  /*
  Returns:
    List of vertices the with interpolated surface points. Vertices that are not in this
    vertex list have fixed control net points. 
    This list is unique to this ON_SubDSurfaceInterpolator instance.
  */
  ON_SubDComponentList& UniqueVertexList();

  void Transform(const ON_Xform& xform);

private:
  // The context assigned id.This id is provided for applications using ON_SubDSurfaceInterpolator.
  // It is not inspected or used in any part of the interpolation setup or calculations.
  mutable ON_UUID m_context_id = ON_nil_uuid;

private:
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4251 )
  // C4251: ... needs to have dll-interface to be used by clients of class ...
  // This warning is not correct. 
  // m_solver_sp and m_vertex_list_sp are private and all code that manages them is explicitly implemented in the DLL.

  // m_solver_sp information needed by Solve() to calculate free vertex control net points from
  // desired surface points. This information does not depend on the locations of free vertices.
  std::shared_ptr<class ON_SubDSurfaceInterpolatorSolver> m_solver_sp;

  // m_vertex_list_sp is the list of free vertices the solver will modify. 
  std::shared_ptr<class ON_SubDComponentList> m_vertex_list_sp;
#pragma ON_PRAGMA_WARNING_POP

private:
  unsigned Internal_Create(ON_SubD& subd, bool bFreeVertexMark, unsigned marked_vertex_count);
};

class ON_CLASS Internal_SubDToNurbsExtraordinaryPoint {
 public:
  /*
  Remove double knots if possible without changing the surface location, in a
  surface of degree 4 in direction dir. Internal use only in
  opennurbs_plus_brep_facegroups.cpp and opennurbs_plus_subd_mesh.cpp for dealing
  with SubD to NURBS conversions.
  */
  ON_INTERNAL_SDK
  static const bool Internal_TryRemoveDoubleKnotsDegFour(const int dir,
                                                         ON_NurbsSurface* srf);
};

#endif

