//
// Copyright (c) 1993-2021 Robert McNeel & Associates. All rights reserved.
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

struct CRhinoPickContext_MeshEdgeInfo
{
  // edge list
  //   The i-th edge connects mesh vertices m_V[vi0] and m_V[vi1], where
  //   vi0 = m_edges[i*m_edge_stride] and vi1 = m_edges[i*m_edge_stride+1].
  int m_edge_count;
  int m_edge_stride;
  const int* m_edges;

  // edge component index map
  //   If m_ci_map is null, then m_edges[] is in component index order.
  //   If m_ci_map is not null, then the meshtop_edge component index for
  //   the i-th edge is m_ci_map[i*m_ci_map_stride].
  int m_edge_ci_map_stride;
  const int* m_edge_ci_map;

  // vertext component index map
  //   m_vertex_ci_map[i*m_vertex_ci_map_stride] is the meshtop_vertex 
  //   component index for the vertex ON_Mesh.m_V[i].
  int m_vertex_ci_map_stride;
  const int* m_vertex_ci_map;
};

class RHINO_SDK_CLASS CRhinoPickContext
{
public:
  CRhinoPickContext();

  // This m_view can be a model view or a page view. When m_view 
  // is a page view, then you need to distinguish between the viewports
  // m_view->MainViewport() and  m_view->ActiveViewport().  When
  // m_view is a model view, both m_view->MainViewport() and  
  // m_view->ActiveViewport() return the world view's viewport.
  class CRhinoView* m_view = nullptr;

  // pick chord starts on near clipping plane and ends on far clipping plane
  ON_Line m_pick_line; 

  // m_pick_region.m_xform maps world 3d picking frustum 
  // into -1<=x,y,z<=+1 clipping coordinate box
  ON_ClippingRegion m_pick_region;

  // If m_bSetClippingPlanes is true, then CRhPickContext 
  // will use lazy evaluation to automatically set 
  // m_pick_region.m_clip_planes[] when m_view is set.
  // If m_bSetClippingPlanes is false, then you have to set
  // m_pick_region.m_clip_planes[] if they are required.
  bool m_bSetClippingPlanes = true;

  // Sometimes a component of m_pick_xform is due to a model 
  // transformation.  For example, this happens when component 
  // geometry in an instance reference is being picked.  The model 
  // transformation information is recoreded below.  Most objects
  // can ignore this information.  However, objects like annotation
  // where arrow heads and text sizes are independent of model
  // transformations need this information.
  ON_Xform m_model_xform;
  bool m_bModelXformIsIdentity = true; // True if the m_model_xform is the identity.
  double m_model_xform_det = 1.0;      // determinant of m_model_xform

  /*
  Description:
    Updates the clipping plane information in m_pick_region.
    The m_bSetClippingPlanes and m_view fields must be 
    before calling UpdateClippingPlanes().
  */
  void UpdateClippingPlanes();

  /*
  Description:
    Updates the clipping plane information in m_pick_region.
    The m_bSetClippingPlanes and m_view fields must be
    before calling UpdateClippingPlanes().
  Parameters:
    filter - [in] optional list of UUIDs to pay attention to when
      updating clipping planes. Only planes with ids in the filter
      list will be used if filter is not null and contains entries
  */
  void UpdateClippingPlanes(const ON_UuidList* filter);


  // three styles of picking
  enum
  {
    no_pick       = 0,
    point_pick    = 1,
    window_pick   = 2,
    crossing_pick = 3
  } m_pick_style = point_pick;

  /*
  Returns:
    m_pick_style as an ON_PickType.
  */
  ON_PickType PickType() const;

  // picking can happen in wireframe or shaded display mode
  enum
  {
    wireframe_pick = 1,
    shaded_pick    = 2
  } m_pick_mode = wireframe_pick;

  // these flags are used in pre and post pick
  bool m_bSkipSelected = true;
  bool m_bSkipUnselected = false;

  // in pre-pick, m_go is NULL
  const class CRhinoGetObject* m_go = nullptr; // set if picking is in CRhinoGetObject::GetObject()

  // true if CRhinoGroupObjects should be added to pick list
  bool m_bPickGroups = false;

  // true if the user activated subobject selection
  bool m_bSubSelect = false; 

  /*
  Description:
    Tests an object to see if it is an acceptable candidate for being
    selected.
    This test is fast and completely answers all status questions. 
    If IsAcceptableCandidate() returns true, then the only thing
    left to test is intersecting the pick frustrum with the object.
  Parameters:
    object - [on] object to test
    bIgnoreSelectionState - [in] if true, current selection state is ignored.
          If false (default), then selected objects are NOT acceptable.
    bIgnoreLocking - [in] 
          If true, locked objects and objects on locked layers are acceptable.
          If false (default), then locked objects and objects on locked layers 
          are not acceptable.
    bIgnoreVisibility - [in] 
          If true, hidden objects and objects on hidden layers are acceptable.
          If false (default), then hidden objects and objects on hidden layers
          are not acceptable.
    
  Returns:
    Returns true if object is an acceptable candidate for being selected
  */
  bool IsAcceptableCandidate( 
            const class CRhinoObject* object, 
            bool bIgnoreSelectionState = false,
            bool bIgnoreLocking = false,
            bool bIgnoreVisibility = false
            ) const;

  /*
  Description:
    Fast test to check if a bounding box intersects a pick frustum.
  Parameters:
    bbox - [in] bounding box
    bBoxInsidePickFrustum - [out] 
       if not NULL, then *bBoxInsidePickFrustum is set to true 
       if the box is completely contained in the pick frustum.  
       When doing a window or crossing pick, you can immediately 
       return a hit if the object's bounding box is completely
       inside of the pick frustum.
  Returns:
    False if bbox is invalid or box does not intersect the 
    pick frustum
  */
  bool PickBox( 
    const ON_BoundingBox& bbox, 
    bool* bBoxInsidePickFrustum = 0
    ) const;

  /*
  Description:
    Utility for picking 3d point
  Parameters:
    point - [in]
    depth - [out] depth returned here for point picks
                  LARGER values are NEARER to the camera.
                  SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
                  SMALLER values are CLOSER to the pick point
  Returns:
    true if there is a hit.
  */
  bool PickPoint(
        const ON_3dPoint& point, 
        double*  depth,
        double*  dist
        ) const;

  /*
  Description:
    Utility for picking 3d point clouds
  Parameters:
    point_count - [in]
    points - [in]
    point_index [out] zero based index returned here for point picks
    depth - [out] depth returned here for point picks
                  LARGER values are NEARER to the camera.
                  SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
                  SMALLER values are CLOSER to the pick point
  Returns:
    true if there is a hit.
  */
  bool PickPointCloud(
        int point_count,
        const ON_3dPoint* points, 
        int* point_index,
        double* depth,
        double* dist
        ) const;

  /*
  Description:
    Utility for picking 3d line
  Parameters:
    line - [in]
    t - [out] line parameter returned here for point picks
    depth - [out] depth returned here for point picks
                  LARGER values are NEARER to the camera.
                  SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
                  SMALLER values are CLOSER to the pick point
  Returns:
    true if there is a hit.
  */
  bool PickLine(
        const ON_Line& line, 
        double* t,
        double* depth, 
        double* dist
        ) const;

  /*
  Description:
    Utility for picking bezier curves
  Parameters:
    bezier_curve - [in]
    t - [out] line parameter returned here for point picks
    depth - [out] depth returned here for point picks
                  LARGER values are NEARER to the camera.
                  SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
                  SMALLER values are CLOSER to the pick point
  Returns:
    true if there is a hit.
  */
  bool PickBezier(
        const ON_BezierCurve& bezier_curve, 
        double* t,
        double* depth,
        double* dist
        ) const;

  /*
  Description:
    Utility for picking NURBS curves
  Parameters:
    nurbs_curve - [in]
    t - [out] line parameter returned here for point picks
    depth - [out] depth returned here for point picks
                  LARGER values are NEARER to the camera.
                  SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
                  SMALLER values are CLOSER to the pick point
  Returns:
    true if there is a hit.
  */
  bool PickNurbsCurve(
        const ON_NurbsCurve& nurbs_curve, 
        double* t,
        double* depth,
        double* dist
        ) const;

  /*
  Description:
    Utility for picking meshes
  Parameters:
    mesh - [in]
    edge_info - [in] (can be null)
      If not null, edge_info for wireframe picking.
      This parameter is not used when pick_style != 0
    pick_style - [in] 
          0 = wireframe picking 
              (checks for vertex and edge hits)  If the mesh edge list
              from ON_Mesh::GetMeshEdgeList() is available, then pass
              it in.
          1 = shaded mode picking
              (checks for face hits)
          2 = vertex only picking
              (returns false if no vertices are hit)

    mesh_edge_count - [in]
      Number of edges in mesh_edge_list[].
    mesh_edge_stride - [in]
      The mesh.m_V[] indices of the i-th mesh edge are
      vi0 = mesh_edge_list[i*mesh_edge_stride] and
      vi1 = mesh_edge_list[i*mesh_edge_stride+1] and
    mesh_edge_list - [in]
      optional list of mesh edges from ON_Mesh::GetEdgeList().
      If this information is available, pass it in.

    hit_point - [out] location returned here for point picks
    hit_srf_uv - [out] if not NULL and the mesh has surface parameters, hit_uv
          is set to the surface parameters of the hit point.
    hit_tex_st - [out] if not NULL and the mesh has texture coordinates, 
          hit_st is set to the texture coordinate of the hit 
          point.  Note that the texture coordinates can be set
          in many different ways and this information is
          useless unless you know how the texture coordinates
          are set on this particular mesh.

    depth - [out] depth returned here for point picks
          LARGER values are NEARER to the camera.
          SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
          SMALLER values are CLOSER to the pick point

    vef_flag - [out] (for point picks) 0 = vertex hit, 1 = edge hit, 2 = face hit
    vef_index - [out] (for point picks) index of vertex/edge/face that was hit
  Returns:
    true if there is a hit.
  */
  bool PickMesh(
        const ON_Mesh* mesh,
        int pick_style,
        ON_3dPoint* hit_point,
        ON_2dPoint* hit_srf_uv,
        ON_2dPoint* hit_tex_st,
        double* depth,
        double* dist,
        int* vef_flag,
        int* vef_index
        ) const;

  bool PickMesh(
        const ON_Mesh* mesh,
        const struct CRhinoPickContext_MeshEdgeInfo* edge_info,
        int pick_style,
        ON_3dPoint* hit_point,
        ON_2dPoint* hit_srf_uv,
        ON_2dPoint* hit_tex_st,
        double* depth,
        double* dist,
        int* vef_flag,
        int* vef_index
        ) const;

  /*
  Description:
    Pick a triangle.

  Parameters:
    points - [in]
      corners of the triangle.

    clip_points - [in]
      The clip_points[] and clipflags[] parameters can be nullptr, in which case the 
      information will be locally calculated in the function. When a large number of 
      triangles that share corner points are being tested, it is more efficient to 
      calculate the clip_pionts[] and clip_flags[] information once per point 
      location, provided storage and access is well managed. 
      Use CRhinoPickContext.m_pick_region.TrasnformPoint() to calculate the correct
      values for clip_points[] and clip_flags[].

      Both versions of PickSegment() return the same true/false result and calculate the 
      same values for pick_point.m_point, pick_point.m_depth and pick_point.m_distance.
      If the pick projection is from a parallel (orthographic) view, then both versions
      calculate the same value for pick_point.m_t[0] parameter.  If the pick projection
      is from a perspective view, then the version of the function that takes 
      ON_4dPoints will calculated the pick_point.m_t[0] parameter more accurately, 
      assuming the ON_4dPoints are properly calculated from the projection matrix.

    clip_flags - [in]
      The clip_points[] and clipflags[] parameters can be nullptr, in which case the 
      information will be locally calculated in the function. When a large number of 
      triangles that share corner points are being tested, it is more efficient to 
      calculate the clip_pionts[] and clip_flags[] information once per point 
      location, provided storage and access is well managed. 
      Use CRhinoPickContext.m_pick_region.TrasnformPoint() to calculate the correct
      values for clip_points[] and clip_flags[].

    pick_point - [out]

  Returns:
    True if the triangle is hit.
  Remarks:
    The value of CRhinoPickContext.m_pick_mode is ignored. The hit point calculation
    considers the entire triangle.
  */
  bool PickTriangle(
    const ON_3dPoint points[3],
    const ON_3dPoint clip_points[3],
    const unsigned int clip_flags[3],
    ON_PickPoint& pick_point
    ) const;

  bool PickTriangle(
    const ON_3dPoint points[3],
    const ON_4dPoint clip_points[3],
    const unsigned int clip_flags[3],
    ON_PickPoint& pick_point
    ) const;

  /*
  Description:
    Pick a segment.

  Parameters:
    points - [in]
      ends of the segment.

    clip_points - [in]
      The clip_points[] and clipflags[] parameters can be nullptr, in which case the 
      information will be locally calculated in the function. When a large number of 
      segments that share end points are being tested, it is more efficient to 
      calculate the clip_pionts[] and clip_flags[] information once per point 
      location, provided storage and access is well managed. 
      Use CRhinoPickContext.m_pick_region.TrasnformPoint() to calculate the correct
      values for clip_points[] and clip_flags[].

      Both versions of PickSegment() return the same true/false result and calculate the 
      same values for pick_point.m_point, pick_point.m_depth and pick_point.m_distance.
      If the pick projection is from a parallel (orthographic) view, then both versions
      calculate the same value for pick_point.m_t[0] parameter.  If the pick projection
      is from a perspective view, then the version of the function that takes 
      ON_4dPoints will calculated the pick_point.m_t[0] parameter more accurately, 
      assuming the ON_4dPoints are properly calculated from the projection matrix.

    clip_flags - [in]
      The clip_points[] and clipflags[] parameters can be nullptr, in which case the 
      information will be locally calculated in the function. When a large number of 
      segments that share end points are being tested, it is more efficient to 
      calculate the clip_pionts[] and clip_flags[] information once per point 
      location, provided storage and access is well managed. 
      Use CRhinoPickContext.m_pick_region.TrasnformPoint() to calculate the correct
      values for clip_points[] and clip_flags[].

    pick_point - [out]
  Returns:
    True if the triangle is hit.
  */
  bool PickSegment(
    const ON_3dPoint points[2],
    const ON_3dPoint clip_points[2],
    const unsigned int clip_flags[2],
    ON_PickPoint& pick_point
    ) const;

  bool PickSegment(
    const ON_3dPoint points[2],
    const ON_4dPoint clip_points[2],
    const unsigned int clip_flags[2],
    ON_PickPoint& pick_point
    ) const;

  /*
  Description:
    Pick a triangle.
  Parameters:
    subd_appearance - [in]
      surface or control net
    subd_surface_mesh_fragment - [in]
      A surface mesh fragment used to display some part of the face.
    component_points - [out]
      The array must have a capacity of 9 components.
      If component_points[].m_component_ptr.Direction() is not zero, then a window
      pick involving a partial fragment added an edge/face and additional testing is
      required to determine if the entire edge/face is inside the widow.
  Returns:
    Number of components that were hit.  Information about these
    components is returned in the component_points[] array.
    If bReturnSubObjects is false and the pick style is a window or crossing,
    then 1 is returned if the pick hits the fragment and component_points[0]
    is set, but the contents are meaningless.
  */
  unsigned int PickSubDMeshFragment(
    ON_SubDComponentLocation subd_appearance,
    const class ON_SubDMeshFragment& subd_surface_mesh_fragment,
    const class CRhinoSubDPickSettings& subd_pick_settings,
    ON_SubDComponentPoint component_points[9]
  ) const;

  /*
  Description:
    Low level tool to determine perform additional checking to 
    insure a SubD face is inside a window pick.
  Parameters:
    subd_appearance - [in]
      surface or control net
    bWireframePick - [in]
      True for wireframe pick.
      False for shaded surface pick.
    face - [in]
      SubD face to test.
    subd_surface_mesh_fragment - [in]
      A surface mesh fragment used to display some part of the face.
  Returns:
    True when input information is valid and the entire subd face is inside the window pick.
    False otherwise.
  */
  bool SubDFaceIsInsideWindowPick(
    ON_SubDComponentLocation subd_appearance,
    bool bWireframePick,
    const class ON_SubDFace* face,
    const class ON_SubDMeshFragment& subd_surface_mesh_fragment
  ) const;

  /*
  Description:
    Low level tool to determine perform additional checking to
    insure a SubD edge is inside a window pick.
  Parameters:
    edge - [in]
      SubD edge to test.
   subd_surface_mesh_fragment - [in]
      A surface mesh fragment used to display some part of the face.
   Returns:
    True when input information is valid and the entire subd face is inside the window pick.
    False otherwise.
  */
  bool SubDEdgeIsInsideWindowPick(
    ON_SubDComponentLocation subd_appearance,
    const class ON_SubDEdge* edge,
    const class ON_SubDMeshFragment& subd_surface_mesh_fragment
  ) const;

  /*
  Description:
    Find the best points for a subd selection. 
    The value of subd_frit.SubDAppearance() determines if the surface 
    or control net is inspected.
  Parameters:
    subd_frit - [in]  
      A subd mesh fragment iterator.
      The value of subd_frit.SubDAppearance() determines if the surface
      or control net is inspected.
    subd_pick_settings - [in]
    component_points - [out]
      If bReturnSubObjects is false and a point pick is done,
      then the best pick (vertex, edge or face) is added to component_points[].

      If bReturnSubObjects is false and a window or crossing pick
      is done, then nothing is added to component_points[].

      If bReturnSubObjects is true, every subobject that qualifies is
      added to component_points[].
  Returns:
    0: nothing selected.
    
    If bReturnSubObjects is false and something was selected, 1 is returned.
    
    If bReturnSubObjects is true, the number of components added to component_points[]
    is returned.
  */
  unsigned int PickSubD(
    ON_SubDMeshFragmentIterator subd_frit,
    const class CRhinoSubDPickSettings& subd_pick_settings,
    ON_SimpleArray< ON_SubDComponentPoint >& component_points
    ) const;

  /*
  Description:
    Find the best points for a subd control net selection.
    Up to 3 points can be returned, a face point, an edge point
    and a vertex point.  The caller can apply various criteria to
    determing which point is the best one.
  Parameters:
    subd_limit_mesh - [in]
    level_index - [in]
    component_point - [out]
      component_point[0] 
        A point on a vertex or ON_SubDComponentPoint::Unset.
      component_point[1] 
        A point on an edge or ON_SubDComponentPoint::Unset.
      component_point[2] 
        A point on a face or ON_SubDComponentPoint::Unset.
  Returns:
    true if at least one point is returned.
  */
  unsigned int PickSubDControlNet(
    const class ON_SubDRef subd_ref,
    const class CRhinoSubDPickSettings& subd_pick_settings,
    ON_SimpleArray< ON_SubDComponentPoint >& component_points
    ) const;

  /*
  Description:
    Utility for picking meshes
  Parameters:
    light - [in]
    hit_point - [out] location returned here for point picks
    depth - [out] depth returned here for point picks
                  LARGER values are NEARER to the camera.
                  SMALLER values are FARTHER from the camera.
    dist - [out]  planar distance returned here for point picks.
                  SMALLER values are CLOSER to the pick point
  Returns:
    true if there is a hit.
  */
  bool PickLight(
        const ON_Light& light, 
        ON_3dPoint& hit_point,
        double* depth, 
        double* dist
        ) const;
};


class RHINO_SDK_CLASS CRhinoPickFilter
{
public:
  CRhinoPickFilter() = default;
	virtual ~CRhinoPickFilter();

	void Register();
	void UnRegister();

	virtual bool IsAcceptableCandidate(const CRhinoPickContext& pickContext,
									   const class CRhinoObject* object, 
									   bool bIgnoreSelectionState,
									   bool bIgnoreLayerLocking,
									   bool bIgnoreLayerVisibility);
};

class RHINO_SDK_CLASS CRhinoSubDPickSettings
{
public:
  static const CRhinoSubDPickSettings Defaults;

  CRhinoSubDPickSettings() = default;
  ~CRhinoSubDPickSettings() = default;
  CRhinoSubDPickSettings(const CRhinoSubDPickSettings&) = default;
  CRhinoSubDPickSettings& operator=(const CRhinoSubDPickSettings&) = default;

  static CRhinoSubDPickSettings  CreateFromObjectAndPickContext(
    const class CRhinoSubDObject& subd_object,
    const class CRhinoPickContext& pick_context
    );

  static CRhinoSubDPickSettings  CreateFromObjectAndGetObjectContext(
    const class CRhinoSubDObject& subd_object,
    const class CRhinoGetObject& go,
    bool bCheckSubObjects
    );

  /*
  Parameters:
    subd - [in]
    bExcludeComponents - [in]
      if true, vertices, edges, and faces (either as subd components or proxy brep components)
      cannot be selected.
    bExcludeProxyBrep - [in]
      if true, a proxy brep or its components cannot be used.
      if false, go_geometry_filter and go_attribute_filter determine when a proxy brep might be allowed.
  */
  static CRhinoSubDPickSettings CreateFromObjectAndGetObjectFilters(
    const class ON_SubD& subd,
    bool bExcludeComponents,
    bool bExcludeProxyBrep,
    unsigned int go_geometry_filter,
    unsigned int go_attribute_filter
    );

public:
  /*
  Returns:
    True if the pick must eventually return references to the subd or its components.
  Remarks:
    At most one of SubDResults() and ProxyBrepResults() will be true;
  */
  bool SubDResults() const;

  /*
  Returns:
    True if the entire subd is eligible to be selected as a subd object.
  Remarks:
    At most one of SubDResults() and ProxyBrepResults() will be true;
  */
  bool SubDIsEligible() const;

  /*
  Returns:
    true if one or more individual subd components are eligbile to be selected as subd components.
  Remarks:
    At most one of SubDComponentsAreEligible() and ProxyBrepComponentsAreEligible() will be true;
  */
  bool SubDComponentsAreEligible() const;

  /*
  Returns:
    True if the pick must eventually return references to the proxy brep or its components.
  Remarks:
    At most one of SubDResults() and ProxyBrepResults() will be true;
  */
  bool ProxyBrepResults() const;

  /*
  Returns:
    True if the entire proxy brep is eligible to be selected as a brep object.
  Remarks:
    At most one of SubDResults() and ProxyBrepResults() will be true;
  */
  bool ProxyBrepIsEligible() const;

  /*
  Returns:
    true if one or more individual proxy brep components are eligbile to be selected as brep components.
  Remarks:
    At most one of SubDComponentsAreEligible() and ProxyBrepComponentsAreEligible() will be true;
  */
  bool ProxyBrepComponentsAreEligible() const;

  /*
  Returns:
    True if the SubD or proxy brep as an entire object is eligible to be selected.
  */
  bool EntireObjectIsEligible() const;


  /*
  Returns:
    True if SubD or proxy brep vertices are eligible to be selected.
  */
  bool VerticesAreEligible() const;

  /*
  Returns:
    True if SubD or proxy brep edges are eligible to be selected.
  */
  bool EdgesAreEligible() const;

  /*
  Returns:
    True if SubD or proxy brep faces are eligible to be selected.
  */
  bool FacesAreEligible() const;
  
  /*
  Returns:
    True if SubD or proxy brep vertices, edges, or faces are eligible to be selected as subobjects.
  */
  bool ComponentsAreEligible() const;

  /*
  Returns:
    True if nothing is eligible to be selected.
  */
  bool NothingIsEligible() const;

public:

  /*
  Description:
    Determine if the subd_face attributes make it eligible to be selected.
  Parameters:
    subd_face - [in]
  Returns:
    true if the subd_face is eligible for selection.
    false if subd_face is not eligible for selection.
  Remarks:
    This function inspects the values of CRhinoSubDPickSettings.m_bExcludeFaces, 
    subd_face->m_status.IsHidden(), and subd_face->ComponentAttributes() 
    to determine if the face is eligible for selection.
    Any additional tests, like the frustum tests required for a 
    point/window/crossing selection, must be checked as well.
  */
  bool FaceIsEligible(
    const ON_SubDFace* subd_face
    ) const;


  /*
  Description:
    Determine if the subd_face attributes make it eligible to be selected
    in a wireframe pick that hits subd_edge
  Parameters:
    subd_face - [in]
      Face to test
    subd_edge - [in]
      Edge to pick in wireframe
  Returns:
    true if the subd_face is eligible for selection.
    false if subd_face is not eligible for selection.
  Remarks:
    This function inspects the values of CRhinoSubDPickSettings.m_bExcludeFaces, 
    subd_face->m_status.IsHidden(), and subd_face->ComponentAttributes() 
    to determine if the face is eligible for selection.
    Any additional tests, like the frustum tests required for a 
    point/window/crossing selection, must be checked as well.
  */
  bool FaceFromWireframeEdgeIsEligible(
    const ON_SubDFace* subd_face,
    const ON_SubDEdge* subd_edge
    ) const;

  /*
  Description:
    Determine if the subd_edge is eligible to be selected as an edge subobject.
  
  Parameters:
    subd_edge - [in]
  
  Returns:
    true if the subd_edge is eligible for selection.
    false if subd_edge is not eligible for selection.
  
  Remarks:
    Do not use this function to determine if attached faces or attached 
    vertices are eligible.

    This function inspects the values of CRhinoSubDPickSettings.m_bExcludeEdges, 
    subd_edge->m_status.IsHidden(), and subd_edge->ComponentAttributes() 
    to determine if the edge is eligible for selection.
    Any additional tests, like the frustum tests required for a 
    point/window/crossing selection, must be checked as well.
  */
  bool EdgeIsEligible(
    const ON_SubDEdge* subd_edge
    ) const;

  /*
  Description:
    Determine if the subd_vertex attributes make it eligible to be selected.
  Parameters:
    subd_vertex - [in]
  Returns:
    true if the subd_vertex is eligible for selection.
    false if subd_vertex is not eligible for selection.
  Remarks:
    This function inspects the values of CRhinoSubDPickSettings.m_bExcludeVertices, 
    subd_vertex->m_status.IsHidden(), and subd_vertex->ComponentAttributes() 
    to determine if the vertex is eligible for selection.
    Any additional tests, like the frustum tests required for a 
    point/window/crossing selection, must be checked as well.
  */
  bool VertexIsEligible(
    const ON_SubDVertex* subd_vertex
    ) const;

  /*
  Description:
    Determine if the component_ptr attributes make it eligible to be selected.
  Parameters:
    component_ptr - [in]
  Returns:
    true if the subd component is eligible for selection.
    false if subd component is not eligible for selection.
  Remarks:
    This function inspects the component attributes information and
    values of CRhinoSubDPickSettings.m_bExclude* to determine if the
    component is eligible for selection.
    Any additional tests, like the frustum tests required for a 
    point/window/crossing selection, must be checked as well.
  */
  bool ComponentIsEligible(
    ON_SubDComponentPtr component_ptr
    ) const;

  /*
  Returns:
    True if the selection can only "see" edges and vertices.
  */
  bool WireFrameSelection() const;

  /*
  Returns:
    True if the object is "see through" shaded, like xray or ghosted, 
    and should behave like a wireframe object for edge and vertex picks.
  */
  bool Ghosted() const;
  void SetGhosted(bool bSet = true);

  ///////////////////////////////////////////////////////////////////////////
  //
  // The values of m_bExcludeSubD/m_bExcludeVertices/m_bExcludeEdges/m_bExcludeFaces
  // control what parts of an ON_SubD are eligible to be selected.
  // The m_bExcludeSubD/m_bExcludeVertices/m_bExcludeEdges/m_bExcludeFaces values
  // are not used to determine what display information is inspected during 
  // mouse point/window/crossing selection.
  //
  //
  // If all four of the m_bExclude* values are true, every possibility is excluded 
  // and nothing can be selected.
  //

  // Identifies the Rhino document being queried.
  // Use CRhinoDoc::FromRuntimeSerialNumber(m_rhino_doc_sn) to get a pointer if one is needed.
  unsigned int m_rhino_doc_sn = 0;

  // If not zero, the top level Rhino object that is being queried. Note that this
  // may be a CRhinoSubDObject, an CRhinoInstanceObject, or something else in 
  // complex situations. 
  // Use CRhinoObject::FromRuntimeSerialNumber(m_rhino_doc_sn,m_rhino_object_sn) to 
  // get a pointer if one is needed.
  unsigned int m_rhino_object_sn = 0;

private:
  // If m_bExcludeSubD is true, the a subd as an entire object is not eligible
  // for selection as a subd. See also m_bIncludeProxyBrep.
  bool m_bExcludeSubD = false;     

  // If m_bExcludeSubDVertices is true, the an individual vertex is not eligible
  // for selection as a subd vertex. See also m_bIncludeProxyBrepVertices.
  bool m_bExcludeSubDVertices = false;

  // If m_bExcludeEdges is true, the an individual edge is not eligible
  // for selection as a subd edge. See also m_bIncludeProxyBrepEdges.
  bool m_bExcludeSubDEdges = false;

  // If m_bExcludeFaces is true, the an individual face is not eligible
  // for selection as a subd face. See also m_bIncludeProxyBrepFaces.
  bool m_bExcludeSubDFaces = false;

private:
  ///////////////////////////////////////////////////////////////////////////
  //
  // The values of m_bIncludeProxyBrep, m_bIncludeProxyBrepVertices,
  // m_bIncludeProxyBrepEdges and m_bIncludeProxyBrepFaces
  // control what parts of a proxy brep are eligible to be selected.
  //
  // If all four of the m_bInclude* values are false, every proxy brep
  // possibility is excluded and nothing from a proxy brep is eligible for selection.
  //
  // Proxy breps are used only in cases when the subd or its components cannot
  // be selected, but a brep or brep components can be selected.
  //
  bool m_bIncludeProxyBrep = false;
  bool m_bIncludeProxyBrepVertices = false;
  bool m_bIncludeProxyBrepEdges = false;
  bool m_bIncludeProxyBrepFaces = false;

private:     
  ///////////////////////////////////////////////////////////////////////////
  //
  // If m_bExcludeMeshes is true, then the quad/triangle meshes used to create
  // a "shaded" image of the subd surface are not tested. Put another way,
  // the selection is a "wire frame" selection that can only "see" edges and vertices.
  //
  bool m_bExcludeMeshes = false;  

public:
  ///////////////////////////////////////////////////////////////////////////
  //
  // If m_bReturnMultipleSubobjects is true, then every eligible prt of the
  // ON_SubD that is hit will be returned.
  // If m_bReturnMultipleSubobjects is false, then the a single best
  // best pick is returned.
  bool m_bReturnMultipleSubobjects = false;


private:
  bool m_bGhosted = false;
  bool m_reserved2 = false;
  unsigned int m_reserved3 = 0;

private:
  unsigned int m_reserved4 = 0; // perhaps for m_vertex_component_attribute_filter = 0;

public:

  ///////////////////////////////////////////////////////////////////////////
  //
  // Component attribute filters
  // 
  //   If a filter is not zero and the corresponding component is not excluded, 
  //   then the filter restricts properties a component must have to be selected.
  //   Set the bits of properties you will accept.  The filters are bit fields
  //   set using values from ON_ComponentAttributes::EdgeAttributes enums.
  //   
  // If 0 != m_edge_topology_attribute_filter and m_bExcludeEdges is false, then
  // and edge is eligible only if 
  // 0 != (m_edge_component_attribute_filter & edge->EdgeAttributes()).
  unsigned int m_edge_component_attribute_filter = 0;

private:
  unsigned int m_reserved5 = 0; // perhaps for m_face_component_attribute_filter = 0;
};

class RHINO_SDK_CLASS CRhinoDoubleClickContext
{
public:
  const CRhinoPickContext* PickContext() const;

private:
  // Only CRhinoView and currently construct an instance of this class
  // We want to be able to add members to this class in the future, so
  // we do not want to allow a plug-in to create one of these on the stack.
  CRhinoDoubleClickContext() = default;
  CRhinoDoubleClickContext(const CRhinoDoubleClickContext&) = default;
  friend class CRhViewBase;

  const CRhinoPickContext* m_pick_context = nullptr;
  ON__UINT_PTR m_sdk_reserved = 0;
};
