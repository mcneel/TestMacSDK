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

////////////////////////////////////////////////////////////////
//
//   Definition of b-rep and its parts
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_BREP_INC_)
#define OPENNURBS_BREP_INC_

class ON_BrepTrim;
class ON_BrepEdge;
class ON_BrepLoop;
class ON_BrepFace;


// TEMPORARY DEFINES SO I DON'T BREAK THE BUILD
//#define m_vertex_user_i m_vertex_user.i
//#define m_trim_user_i m_trim_user.i
//#define m_edge_user_i m_edge_user.i
//#define m_loop_user_i m_loop_user.i
//#define m_face_user_i m_face_user.i

// Description:
//   Brep vertex information is stored in ON_BrepVertex classes.
//   ON_Brep.m_V[] is an array of all the vertices in the brep.
//
//   If a vertex is a point on a face, then brep.m_E[m_ei]
//   will be an edge with no 3d curve.  This edge will have
//   a single trim with type ON_BrepTrim::ptonsrf.  There
//   will be a loop containing this single trim.
//   Use ON_Brep::NewPointOnFace() to create vertices that are
//   points on faces. 
class ON_CLASS ON_BrepVertex : public ON_Point
{
  ON_OBJECT_DECLARE(ON_BrepVertex);

public:
  // Union available for application use.
  // The constructor zeros m_vertex_user.
  // The value is of m_vertex_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_vertex_user; 

public:
  mutable ON_ComponentStatus m_status = ON_ComponentStatus::NoneSet;

private:
  ON__UINT16 m_reserved1 = 0U;

public:
  // index of the vertex in the ON_Brep.m_V[] array
  int m_vertex_index = -1;

  /////////////////////////////////////////////////////////////////
  // Construction
  //
  // In general, you should not directly create ON_BrepVertex classes.
  // Use ON_Brep::NewVertex instead.
  ON_BrepVertex();
  ON_BrepVertex(
    int // vertex index
    );
  ON_BrepVertex& operator=(const ON_BrepVertex&);

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  // virtual ON_Object::Dump() override
  void Dump( ON_TextLog& ) const override; // for debugging

  // virtual ON_Object::Write() override
  bool Write( ON_BinaryArchive& ) const override;

  // virtual ON_Object::Read() override
  bool Read( ON_BinaryArchive& ) override;

  // virtual ON_Geometry::ComponentIndex() override
  ON_COMPONENT_INDEX ComponentIndex() const override;

  /////////////////////////////////////////////////////////////////
  // Interface

  // Description:
  //   Set vertex location.
  // Parameters:
  //   point - [in] 3d vertex location
  bool SetPoint( 
          const ON_3dPoint& // point
          );

  // Returns:
  //   Vertex location.
  ON_3dPoint Point() const;

  // Returns:
  //   value of ON_BrepVertex::m_tolerance
  // Remarks:
  //   Use ON_Brep::SetVertexTolerance( ON_BrepVertex& ) to set tolerances.
  double Tolerance() const;

  // Returns:
  //   number of edges that begin or end at this vertex.
  int EdgeCount() const;

#if defined(OPENNURBS_PLUS)
  /*
  Returns:
    If this vertex is part of a brep created by ON_SubD::ProxyBrep(), then
    the subd component id of the corresponding SubD vertex is returned. Otherwise 0 is returned.
  */
  unsigned int ProxyBrepSubDVertexId() const;
#endif

  /////////////////////////////////////////////////////////////////
  // Implementation

  // indices of edges starting/ending at this vertex
  //
  // For closed edges, edge.m_vi[0] = edge.m_vi[1] and 
  // edge.m_edge_index appears twice in the m_ei[] array.
  // The first occurrence of edge.m_edge_index in m_ei[]
  // is for the closed edge starting the vertex.
  // The second occurrence of edge,m_edge_index in m_ei[]
  // is for the closed edge ending at the vertex.
  // C.f. ON_Brep::Next/PrevEdge().
  ON_SimpleArray<int> m_ei;

  // accuracy of vertex point (>=0.0 or ON_UNSET_VALUE)
  //
  // A value of ON_UNSET_VALUE indicates that the
  // tolerance should be computed.
  //
  // A value of 0.0 indicates that the distance
  // from the vertex to any applicable edge or trim
  // end is <=  ON_ZERO_TOLERANCE
  //
  // If an edge begins or ends at this vertex,
  // then the distance from the vertex's 
  // 3d point to the appropriate end of the
  // edge's 3d curve must be <= this tolerance.
  //
  // If a trim begins or ends at this vertex,
  // then the distance from the vertex's 3d point
  // to the 3d point on the surface obtained by
  // evaluating the surface at the appropriate
  // end of the trimming curve must be <= this
  // tolerance.
  double m_tolerance = ON_UNSET_VALUE;

private:
  ON_BrepVertex( const ON_BrepVertex& ) = delete;
};

/*
Description:
  Brep edge information is stored in ON_BrepEdge classes.
  ON_Brep.m_E[] is an array of all the edges in the brep.

  An ON_BrepEdge is derived from ON_CurveProxy so the the
  edge can supply easy to use evaluation tools via 
  the ON_Curve virtual member functions.

  Note well that the domains and orientations of the curve
  m_C3[edge.m_c3i] and the edge as a curve may not
  agree.
*/

// April 24, 2017 Dale Lear
// ON_Curve::Trim(const ON_Interval&) is a virtual function and ON_BrepEdge derives 
// from ON_CurveProxy which derives from ON_Curve. The ON_Brep::Trim(int) function was 
// added and mistakenly named "Trim". This all happened about twenty years ago.
//
// Both the virtual ON_Curve::Trim(const ON_Interval&) and ON_BrepEdge::Trim(int) functions
// are widely used. At some point the functionON_Brep::Trim() will be deprecated and the 4263
// warning ON_Brep::Trim(int) generates will be disabled for the definition of class ON_BrepEdge.
// In version 7, ON_Brep::Trim(int) will be deleted and the warning will no longer be disabled.
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC(4263)
#pragma ON_PRAGMA_WARNING_DISABLE_MSC(4264)

class ON_CLASS ON_BrepEdge : public  ON_CurveProxy
{
  ON_OBJECT_DECLARE(ON_BrepEdge);

public:

  // Union available for application use.
  // The constructor zeros m_edge_user.
  // The value is of m_edge_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_edge_user;

public:
  mutable ON_ComponentStatus m_status = ON_ComponentStatus::NoneSet;

private:
  ON__UINT16 m_reserved1 = 0U;

public:
  // index of edge in ON_Brep.m_E[] array
  int m_edge_index = -1;    


  // virtual ON_Curve::IsClosed override
  bool IsClosed() const override;

  /////////////////////////////////////////////////////////////////
  // Construction
  //
  // In general, you should not directly create ON_BrepEdge classes.
  // Use ON_Brep::NewVertex instead.
  ON_BrepEdge();
  ON_BrepEdge(int); // edge index
  ON_BrepEdge& operator=(const ON_BrepEdge&);

  // virtual ON_Object function
  // The ON_BrepEdge override returns ON::curve_object.
  ON::object_type ObjectType() const override;

  /*
  Returns:
    Brep this edge belongs to.
  */
  ON_Brep* Brep() const;


  /*
  Parameters:
    eti - [in] index into the edge's m_ti[] array.
  Returns:
    The trim brep.m_T[edge.m_ti[eti]];
  Remarks:
    This version of "Trim" hides the virtual function ON_CurveProxy::Trim(const ON_Interval&),
    which is a good thing. Special care must be taken when changing the geometry
    of a brep edge to insure vertex, trim, and edge information remains valid.
  */
  ON_BrepTrim* Trim( int eti ) const;

  /*
  Returns:
    Number of trims attached to this edge.
  */
  int TrimCount() const;

  /*
  Parameters:
    evi - [in] 0 or 1
  Returns:
    Brep vertex at specified end of the edge.
  */
  ON_BrepVertex* Vertex(int evi) const;

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  // virtual ON_Object::Dump() override
  void Dump( ON_TextLog& ) const override; // for debugging

  // virtual ON_Object::Write() override
  bool Write( ON_BinaryArchive& ) const override;

  // virtual ON_Object::Read() override
  bool Read( ON_BinaryArchive& ) override;

  // virtual ON_Geometry::ComponentIndex() override
  ON_COMPONENT_INDEX ComponentIndex() const override;

  // virtual ON_Curve::Reverse override
  bool Reverse() override;

  /* Not necessary.  Base class does the right thing.  ON_CurveProxy does not have an override.
  // virtual ON_Curve::SetStartPoint override
  bool SetStartPoint(
          ON_3dPoint start_point
          );

  // virtual ON_Curve::SetEndPoint override
  bool SetEndPoint(
          ON_3dPoint end_point
          );
          */

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    For a manifold, non-boundary edge, decides whether or not the two surfaces on either side
    meet smoothly.
  Parameters:
    angle_tolerance - [in] used to decide if surface normals on either side are parallel.
  Returns:
    true if edge is manifold, has exactly 2 trims, and surface normals on either
    side agree to within angle_tolerance.
  */
  bool IsSmoothManifoldEdge(double angle_tolerance = ON_DEFAULT_ANGLE_TOLERANCE) const;
#endif

  /////////////////////////////////////////////////////////////////
  // Implementation

  /*
  Returns:
    brep.m_C3[] index of the 3d curve geometry used by this edge 
    or -1.
  */
  int EdgeCurveIndexOf() const;

  /*
  Returns:
    3d curve geometry used by this edge or nullptr.
  */
  const ON_Curve* EdgeCurveOf() const;

  /*
  Description:
    Expert user tool that replaces the 3d curve geometry
    of an edge
  Parameters;
    c3i - [in] brep 3d curve index of new curve
  Returns:
    True if successful.
  Example:

            ON_Curve* pCurve = ...;
            int c3i = brep.AddEdgeCurve(pCurve);
            edge.ChangeEdgeCurve(c3i);

  Remarks:
    Sets m_c3i, calls SetProxyCurve, cleans runtime caches.
  */
  bool ChangeEdgeCurve(
    int c3i 
    );

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    For each trim at this edge, find the 3d unit vector at the edge parameter in the trim's surface tan plane,
    perpendicular to the edge, which points to the active side of the trim
  Parameters:
    edge_t - [in] parameter of this edge.
    V      - [out] 3d vectors pointing into the faces.  One for each trim, in order of m_ti.
    N      - [out] Surface normals at edge_t.  Parallel to V.
  Returns:
    True if vectors can be found. False if edge tangent or surface normals can't be found,
    or if input is bad.
  */
  bool Get3dVectorsIntoFaces(double edge_t, ON_SimpleArray<ON_3dVector>& V, ON_SimpleArray<ON_3dVector>& N) const;
#endif
  /*
  Description:
    When an edge is modified, the m_pline[].e values need
    to be set to ON_UNSET_VALUE by calling UnsetPlineEdgeParameters().
  */
  void UnsetPlineEdgeParameters();

#if defined(OPENNURBS_PLUS)
  /*
  Returns:
    If this edge is part of a brep created by ON_SubD::ProxyBrep(), then
    the subd component id of the corresponding SubD edge is returned. Otherwise 0 is returned.
  */
  unsigned int ProxyBrepSubDEdgeId() const;
#endif

  // index of 3d curve in m_C3[] array
  // (edge.m_curve also points to m_C3[m_c3i])
  int m_c3i = -1;

  // indices of starting/ending vertex
  //
  // For closed edges, m_vi[0] = m_vi[1] and m_edge_index
  // appears twice in the m_V[m_vi[0]].m_ei[] array.
  // The first occurrence of m_edge_index in m_V[m_vi[0]].m_ei[]
  // is for the closed edge starting the vertex.  The second
  // occurrence of m_edge_index in m_V[m_vi[0]].m_ei[]
  // is for the closed edge edge ending at the vertex.
  // C.f. ON_Brep::Next/PrevEdge().
  int m_vi[2];

  // indices of Trims that use this edge
  ON_SimpleArray<int> m_ti;

  // accuracy of edge curve (>=0.0 or ON_UNSET_VALUE)
  //
  // A value of ON_UNSET_VALUE indicates that the
  // tolerance should be computed.
  //
  // The maximum distance from the edge's 3d curve
  // to any surface of a face that has this edge as
  // a portion of its boundary must be <= this
  // tolerance.
  double m_tolerance = ON_UNSET_VALUE;

private:
  friend class ON_Brep;
  ON_Brep* m_brep = nullptr; // so isolated edge class edge can get at it's 3d curve
  ON_BrepEdge( const ON_BrepEdge& ) = delete;
};

#pragma ON_PRAGMA_WARNING_POP

struct ON_BrepTrimPoint
{
  ON_2dPoint p; // 2d surface parameter space point
  double t;     // corresponding trim curve parameter
  double e;     // corresponding edge curve parameter (ON_UNSET_VALUE if unknown)
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_BrepTrimPoint>;
#endif


/*
Description:
  Brep trim information is stored in ON_BrepTrim classes.
  ON_Brep.m_T[] is an array of all the trim in the brep.

  An ON_BrepTrim is derived from ON_CurveProxy so the the
  trim can supply easy to use evaluation tools via 
  the ON_Curve virtual member functions.

  Note well that the domains and orientations of the curve
  m_C2[trim.m_c2i] and the trim as a curve may not
  agree.
*/
class ON_CLASS ON_BrepTrim : public  ON_CurveProxy
{
  ON_OBJECT_DECLARE(ON_BrepTrim);

public:
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // Union available for application use.
  // The constructor zeros m_trim_user.
  // The value is of m_trim_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_trim_user;

public:
  mutable ON_ComponentStatus m_status = ON_ComponentStatus::NoneSet;

private:
  ON__UINT16 m_reserved1 = 0U;

public:
  int m_trim_index = -1;  // index of trim in ON_Brep.m_T[] array

  // types of trim - access through m_type member.  Also see m_iso and ON_Surface::ISO
  enum TYPE 
  {
    unknown  = 0,
    boundary = 1,       // trim is connected to an edge, is part of an outer, 
                        // inner or slit loop, and is the only trim connected
                        // to the edge.
    mated    = 2,       // trim is connected to an edge, is part of an outer,
                        // inner or slit loop, no other trim from the same 
                        // loop is connected to the edge, and at least one 
                        // trim from a different loop is connected to the edge.
    seam     = 3,       // trim is connected to an edge, is part of an outer loop,
                        // and exactly one other trim from the same loop is 
                        // also connected to the edge.  The trims are domain side
                        // iso's on opposite sides of the surface (e.g E_iso and W_iso) 
                        // (There can be other mated trims that are also
                        // connected to the edge.  For example, the non-manifold
                        // edge that results when a surface edge lies along
                        // the seam of another surface.) 
    singular = 4,       // trim is part of an outer loop, the trim's 2d curve
                        // runs along the singular side of a surface, and the
                        // trim is NOT connected to an edge. (There is no 3d
                        // edge because the surface side is singular.)
    crvonsrf = 5,       // trim is connected to an edge, is the only trim in
                        // a crfonsrf loop, and is the only trim connected to
                        // the edge.
    ptonsrf  = 6,       // trim is a point on a surface, trim.m_pbox is records
                        // surface parameters, and is the only trim
                        // in a ptonsrf loop.  This trim is not connected
                        // to an edge and has no 2d curve.
    slit     = 7,       // 17 Nov 2006 - reserved for future use
                        //   currently an invalid value
    trim_type_count = 8,
    force_32_bit_trim_type = 0xFFFFFFFF
  };

  /////////////////////////////////////////////////////////////////
  // Construction
  //
  // In general, you should not directly create ON_BrepTrim classes.
  // Use ON_Brep::NewTrim instead.
  ON_BrepTrim();
  ON_BrepTrim(int); // trim index
  ON_BrepTrim& operator=(const ON_BrepTrim&);

  /*
  Returns:
    Brep that this trim belongs to.
  */
  ON_Brep* Brep() const;

  /*
  Returns:
    Brep loop that this trim belongs to.
  */
  ON_BrepLoop* Loop() const;

  /*
  Returns:
    Brep face this trim belongs to.
  */
  ON_BrepFace* Face() const;

  /*
  Returns:
    Brep edge this trim uses or belongs to.  This will
    be nullptr for singular trims.
  */
  ON_BrepEdge* Edge() const;

  /*
  Parameters:
    tvi - [in] 0 or 1
  Returns:
    Brep vertex at specified end of the trim.
  */
  ON_BrepVertex* Vertex(int tvi) const;

  /////////////////////////////////////////////////////////////////
  // ON_Object overrides
  //
  // (Trims are purely topological - geometry queries should be 
  //  directed at the trim's 2d curve or the trim's edge's 3d curve.)

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  void Dump( ON_TextLog& ) const override; // for debugging

  bool Write( ON_BinaryArchive& ) const override;

  bool Read( ON_BinaryArchive& ) override;

  // virtual ON_Geometry::ComponentIndex() override
  ON_COMPONENT_INDEX ComponentIndex() const override;

  // virtual ON_Curve::Reverse override
  // Reverses curve - caller must make sure trim's m_bRev3d
  // flags are properly updated.  Use
  // ON_Brep::FlipTrim to reverse and trim and update all
  // m_bRev3d information.
  bool Reverse() override;

  /* Not necessary Base class does the same.
  // virtual ON_Curve::SetStartPoint override
  bool SetStartPoint(
          ON_3dPoint start_point
          ) override;

  // virtual ON_Curve::SetEndPoint override
  bool SetEndPoint(
          ON_3dPoint end_point
          ) override;
*/
  /////////////////////////////////////////////////////////////////
  // Interface
  
  /*
  Description:
    Expert user tool that replaces the 2d curve geometry
    of a trim
  Parameters;
    c2i - [in] brep 2d curve index of new curve
  Returns:
    True if successful.
  Example:

            ON_Curve* pCurve = ...;
            int c2i = brep.AddTrimCurve(pCurve);
            trim.ChangeTrimCurve(c2i);

  Remarks:
    Sets m_c2i, calls SetProxyCurve, cleans runtime caches,
    and updates m_pbox.
  */
  bool ChangeTrimCurve( int c2i );

  /*
  Description:
    Destroy parameter space information.
    Currently, this involves destroying m_pline
    and m_pbox. Parameter space information should
    be destroyed when the location of a trim
    curve is changed.
  */
  void DestroyPspaceInformation();
  
  /*
  Description:
    Expert user function.
    Removes a trim from an edge.
  Parameters:
    bRemoveFromStartVertex - [in] if true, the trim
      is removed from its start vertex by setting
      m_vi[0] to -1.
    bRemoveFromEndVertex - [in] if true, the trim
      is removed from its start vertex by setting
      m_vi[1] to -1.
  Remarks:
    If the trim is attached to an edge (m_ei>=0), then
    the trim is removed from the edge and the edge's
    m_ti[] list.  The trim's m_bRev3d and tolerance values
    are not changed.
  */
  bool RemoveFromEdge( 
        bool bRemoveFromStartVertex,
        bool bRemoveFromEndVertex
        );

  /*
  Description:
    Expert user function.
    Attaches a trim to an edge.
  Parameters:
    edge_index - [in] index of an edge.
    bRev3d - [in] value for trim's m_bRev3d field.
  Remarks:
    If the trim is attached to an edge (m_ei>=0), then
    the trim is removed from the edge and the edge's
    m_ti[] list.  The trim's tolerance values are not
    changed.
  */
  bool AttachToEdge(
        int edge_index,
        bool bRev3d
        );

  /*
  Returns:
    2d curve geometry used by this trim or nullptr
  */
  const ON_Curve* TrimCurveOf() const;

  /*
  Returns:
    3d curve geometry used by this trim or nullptr.
  */
  const ON_Curve* EdgeCurveOf() const;

  /*
  Returns:
    3d surface geometry used by this trim or nullptr
  */
  const ON_Surface* SurfaceOf() const;

  /*
  Returns:
    brep.m_C2[] 2d curve index of the 2d curve geometry used by 
    this trim or -1.
  */
  int TrimCurveIndexOf() const;

  /*
  Returns:
    brep.m_C3[] 3d curve index of the 3d curve geometry used by 
    this trim or -1.
  */
  int EdgeCurveIndexOf() const;

  /*
  Returns:
    brep.m_S[] surface index of the 3d surface geometry used by 
    this trim or -1.
  */
  int SurfaceIndexOf() const;

  /*
  Returns:
    brep.m_F[] face index of the face used by this trim or -1.
  */
  int FaceIndexOf() const;

  /*
  Returns:
    True if the trim satisfies these four criteria.
      1) is part of a loop
      2) is connected to a 3d edge
      3) one other trim from the same loop is connected to the edge
      4) The 2d trim curve for the other trim is the reverse
         of the 2d trim curve for this trim.
  Remarks:
    In order for IsSlit() to work correctly, the m_type and m_iso
    fields must be set correctly.  In V4 SR1, this function will
    be removed and ON_BrepTrim::slit will be added as a type.
  */
  bool IsSlit() const;

  /*
  Returns:
    True if the trim satisfies these four criteria.
      1) is part of a loop
      2) is connected to a 3d edge
      3) one other trim from the same loop is connected to the edge
      4) the 2d trim curve for this trim lies along the side of 
         the face's parameter space and the 2d curve for the other
         trim lies on the opposite side of the face's parameter
         space.
  Remarks:
    In order for IsSeam() to work correctly, the m_type and m_iso
    fields must be set correctly.  In V4 SR1, this function will
    be removed and ON_BrepTrim::slit will be added as a type.
  */
  bool IsSeam() const;

  /*
  Description:
    Expert user tool that transforms all the parameter space (2d)
    trimming curves in this loop.  Only 2d curve geometry is
    changed.  The caller is responsible for reversing loops,
    toggle m_bRev, flags, etc.
  Parameters:
    xform - [in] Transformation applied to 2d curve geometry.
  Returns
    True if successful.  If false is returned, the brep
    may be invalid.
  */
  bool TransformTrim( const ON_Xform& xform );

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    At trim parameter trim_t, get the 3d unit vector in the surface tangent plane,
    perpendicular to the edge, which points into the active side of the trim.
  Parameters:
    trim_t - [in] parameter to trim at which to find the vector
    V      - [out] the 3d vector
    N      - [out] Surface normal at trim_t
  Returns:
    true if the vector can be found. 
    False if edge tangent can't be computed or input is bad.
  */
  bool Get3dVectorIntoFace(double trim_t, ON_3dVector& V, ON_3dVector& N) const;
#endif
  // index of the 2d parameter space trimming curve
  int m_c2i = -1;

  // index of 3d edge (-1 if ON_BrepTrim is singular)
  int m_ei = -1;

  // Indices of start/end vertices.  Trims along singular
  // sides and trims that correspond to closed 3d edges
  // have m_vi[0] = m_vi[1].  Note that singular trims
  // and trims on the closed edge of a closed surface can
  // have an open 2d trimming curve and still have 
  // m_vi[0] = m_vi[1].
  int m_vi[2];
        
  // true if the 2d trim and 3d edge have opposite orientations.
  bool   m_bRev3d = false;

  TYPE   m_type = ON_BrepTrim::unknown;
  ON_Surface::ISO  m_iso = ON_Surface::not_iso;

  // index of loop that uses this trim
  int    m_li = -1;

  // The values in m_tolerance[] record the accuracy of
  // the parameter space trimming curves.
  //
  // Remarks:
  //   m_tolerance[0] = accuracy of parameter space curve
  //   in first ( "u" ) parameter
  //
  //   m_tolerance[1] = accuracy of parameter space curve
  //   in second ( "v" ) parameter
  //
  //   A value of ON_UNSET_VALUE indicates that the 
  //   tolerance should be computed. If the value >= 0.0, 
  //   then the tolerance is set.  If the value is 
  //   ON_UNSET_VALUE, then the tolerance needs to be
  //   computed.
  //
  //   If the trim is not singular, then the trim must
  //   have an edge.  If P is a 3d point on the edge's
  //   curve and surface(u,v) = Q is the point on the 
  //   surface that is closest to P, then there must
  //   be a parameter t in the interval [m_t[0], m_t[1]]
  //   such that
  //
  //   |u - curve2d(t)[0]| <= m_tolerance[0]
  //
  //   and 
  //
  //   |v - curve2d(t)[1]| <= m_tolerance[1]
  //
  //   If P is the 3d point for the vertex brep.m_V[m_vi[k]]
  //   and (uk,vk) is the corresponding end of the trim's
  //   parameter space curve, then there must be a surface
  //   parameter (u,v) such that:
  //
  //   *  the distance from the 3d point surface(u,v) to P
  //      is <= brep.m_V[m_vi[k]].m_tolerance,
  //   *  |u-uk| <= m_tolerance[0].
  //   *  |v-vk| <= m_tolerance[1].
  double m_tolerance[2]; 

  // Runtime polyline approximation of trimming curve.
  // This information is not saved in 3DM archives.
  ON_SimpleArray<ON_BrepTrimPoint> m_pline;

  /*
  Description:
    When an edge is modified, the m_pline[].e values need
    to be set to ON_UNSET_VALUE by calling UnsetPlineEdgeParameters().
  */
  void UnsetPlineEdgeParameters();

  // Runtime parameter space trimming curve bounding box.
  // This information is not saved in 3DM archives.
  ON_BoundingBox m_pbox;

public:
  // values stored in legacy file formats - ignore

  void m__legacy_flags_Set(int,int);   // used internally - ignore
  bool m__legacy_flags_Get(int*,int*) const; // used internally - ignore
  double m__legacy_2d_tol = ON_UNSET_VALUE; // used internally - ignore
  double m__legacy_3d_tol = ON_UNSET_VALUE; // used internally - ignore
  int    m__legacy_flags = 0;  // used internally - ignore

private:
  friend class ON_Brep;
  ON_Brep* m_brep = nullptr; // so isolated edge class edge can get at it's 3d curve
  ON_BrepTrim( const ON_BrepTrim& ) = delete;
};

class ON_CLASS ON_BrepLoop : public  ON_Geometry
{
  ON_OBJECT_DECLARE(ON_BrepLoop);

public:
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Geometry overrides
  // A loop is derived from ON_Geometry so that is can 
  // be passed around to things that expect ON_Geometry
  // pointers.  It is not a very useful stand-alone object.

  /*
  Description:
    virtual ON_Geometry::Dimension() override.
  Returns:
    2
  */
  int Dimension() const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  // virtual ON_Geometry::Transform() override.
  bool Transform( 
         const ON_Xform& xform
         ) override;
public:
  /*
  Returns:
   Brep that the loop belongs to.
  */
  ON_Brep* Brep() const;

  /*
  Returns:
    Brep face this loop belongs to.
  */
  ON_BrepFace* Face() const;

  /*
  Parameters:
    lti - [in] index into the loop's m_ti[] array.
  Returns:
    The trim brep.m_T[loop.m_ti[lti]];
  */
  ON_BrepTrim* Trim( int lti ) const;

  /*
  Returns:
    Number of trims in this loop.
  */
  int TrimCount() const;

  // Union available for application use.
  // The constructor zeros m_loop_user.
  // The value is of m_loop_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_loop_user;

public:
  mutable ON_ComponentStatus m_status = ON_ComponentStatus::NoneSet;

private:
  ON__UINT16 m_reserved1 = 0U;

public:
  int m_loop_index = -1;  // index of loop in ON_Brep.m_L[] array

  enum TYPE {
    unknown  = 0,
    outer    = 1,  // 2d loop curves form a simple closed curve with a counterclockwise orientation
    inner    = 2,  // 2d loop curves form a simple closed curve with a clockwise orientation
    slit     = 3,  // always closed - used internally during splitting operations
    crvonsrf = 4,  // "loop" is a curveonsrf made from a single 
                   // (open or closed) trim that is has type ON_BrepTrim::crvonsrf.
    ptonsrf = 5,   // "loop" is a ptonsrf made from a single 
                   // trim that is has type ON_BrepTrim::ptonsrf.
    type_count = 6
  };

  ON_BrepLoop();
  ON_BrepLoop(int); // loop index
  ON_BrepLoop& operator=(const ON_BrepLoop&);

  /////////////////////////////////////////////////////////////////
  // ON_Object overrides
  //
  // (Loops and trims are purely topological - geometry queries should be 
  // directed at the trim's 2d curve or the trim's edge's 3d curve.)

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  void Dump( ON_TextLog& ) const override; // for debugging

  bool Write( ON_BinaryArchive& ) const override;

  bool Read( ON_BinaryArchive& ) override;

  // virtual ON_Geometry::ComponentIndex() override
  ON_COMPONENT_INDEX ComponentIndex() const override;

  /////////////////////////////////////////////////////////////////
  // Interface

  //////////
  // Returns the index i such that loop.m_ti[i] = trim.m_trim_index.
  // Returns -1 if the trim is not in this loop
  int IndexOfTrim( const ON_BrepTrim& ) const;

  /*
  Returns:
    brep.m_S[] surface index of the 3d surface geometry used by 
    this loop or -1.
  */
  int SurfaceIndexOf() const;

  /*
  Returns:
    Pointer to the surface geometry used by the loop.   
  */
  const ON_Surface* SurfaceOf() const;

  /*
  Description:
    Expert user tool that transforms all the parameter space (2d)
    trimming curves in this loop.  Only 2d curve geometry is
    changed.  The caller is responsible for reversing loops,
    toggle m_bRev, flags, etc.
  Parameters:
    xform - [in] Transformation applied to 2d curve geometry.
  Returns
    True if successful.  If false is returned, the brep
    may be invalid.
  */
  bool TransformTrim( const ON_Xform& xform );

  ON_SimpleArray<int> m_ti;   // trim indices
  TYPE         m_type = ON_BrepLoop::unknown;
  int          m_fi = -1;   // index of face that uses this loop

  //////////
  // parameter space trimming loop bounding box
  // runtime information - not saved
  ON_BoundingBox m_pbox;
private:
  friend class ON_Brep;
  ON_Brep* m_brep = nullptr;
  ON_BrepLoop(const ON_BrepLoop&) = delete;
};

#if defined(OPENNURBS_PLUS)

struct ON_BrepTrimParameter
{
  int m_trim_index;
  unsigned int m_reserved; // m_reserved is for future use.  Do not reference or set.
  double m_trim_parameter;
  double m_surface_parameters[2];
};

struct ON_BrepFaceIsoInterval
{
  struct ON_BrepTrimParameter m_t[2];
};

#endif

class ON_CLASS ON_BrepFace : public ON_SurfaceProxy
{
  ON_OBJECT_DECLARE(ON_BrepFace);

public:
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // Union available for application use.
  // The constructor zeros m_face_user.
  // The value is of m_face_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_face_user;

public:
  mutable ON_ComponentStatus m_status = ON_ComponentStatus::NoneSet;

private:
  // the 4 byte pack id is stored as 2 ON__UINT16 values to prevent breaking the C++ SDK.
  ON__UINT16 m_pack_id_low = 0; // PackId() = 0x10000*m_pack_id_high + m_pack_id_low;

public:
  int m_face_index = -1;  // index of face in ON_Brep.m_F[] array

  ON_BrepFace();
  ~ON_BrepFace();
  ON_BrepFace(int);
  ON_BrepFace& operator=(const ON_BrepFace&);

  /*
  Returns:
   Brep that the face belongs to.
  */
  ON_Brep* Brep() const;

  /*
  Parameters:
    fli - [in] index into the face's m_li[] array.
  Returns:
    The loop brep.m_L[face.m_li[fli]];
  */
  ON_BrepLoop* Loop( int fli ) const;

  /*
  Returns:
    Number of loops in this face.
  */
  int LoopCount() const;

  /*
  Returns:
    Outer boundary loop for this face.
  */
  ON_BrepLoop* OuterLoop() const;

  /*
  Parameters:
    dir
       1: side with underlying surface normal
         pointing into the topology region
      -1: side with underlying surface normal
          pointing out of the topology region
  Returns:
    Brep region topology face side.  If the region
    topology has not be created by calling
    ON_Brep::RegionToplogy(), then nullptr is returned.
  */
  class ON_BrepFaceSide* FaceSide(int dir) const;


  /////////////////////////////////////////////////////////////////
  // ON_Object overrides
  //
  // (Faces are purely topological - geometry queries should be 
  //  directed at the face's 3d surface.)

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  void Dump( ON_TextLog& ) const override; // for debugging

  bool Write( ON_BinaryArchive& ) const override;

  bool Read( ON_BinaryArchive& ) override;

  // virtual ON_Geometry::ComponentIndex() override
  ON_COMPONENT_INDEX ComponentIndex() const override;

  // virtual ON_Geometry::ClearBoundingBox() override
  void ClearBoundingBox() override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

#if defined(OPENNURBS_PLUS)
  ON_Mesh* CreateMesh( 
             const ON_MeshParameters& mp,
             ON_Mesh* mesh = nullptr
             ) const override;
#endif

  /*
  Description:
    This is an override of the virtual ON_Surface::Reverse
    function.  It toggles the face's m_bRev flag so the abstract
    orientation of the face does not change.
  Parameters:
    dir - [in] 0 = reverse "s" parameter, 1 = reverse "t" parameter
         The domain changes from [a,b] to [-a,-b]
  Returns:
    True if successful.
  Remarks:
    The range of the face's trimming curves and the orientation direction
    of then loops are changed so that the resulting face is still valid.
  */
  bool Reverse(
    int dir
    ) override;

  /*
  Description:
    This is an override of the virtual ON_Surface::Transpose
    function.  It toggles the face's m_bRev flag so the abstract
    orientation of the face does not change.
  Returns:
    True if successful.
  Remarks:
    The range of the face's trimming curves and the orientation direction
    of then loops are changed so that the resulting face is still valid.
  */
  bool Transpose() override;

  /*
  Description:
    This is an override of the virtual ON_Surface::SetDomain
    function.
  Parameters:
    dir - [in] 0 = set "u" domain, 1 = set "v" domain.
    t0 - [in]
    t1 - [in] t0 < t1  The new domain is the interval (t0,t1)
  Returns:
    True if successful.
  */
  bool SetDomain(
    int dir,
    double t0,
    double t1
    ) override;

  /*
  //////////
  // Change the domain of a face
  // This changes the parameterization of the face's surface and transforms
  // the "u" and "v" coordinates of all the face's parameter space trimming
  // curves.  The locus of the face is not changed.
  */
  bool SetDomain(
         ON_Interval udom,
         ON_Interval vdom
         );

  /////////////////////////////////////////////////////////////////
  // Rendering Interface
  //int MaterialIndex() const; // if -1, use parent's material definition
  //void SetMaterialIndex(int);

  // If true is returned, then ~ON_BrepFace will delete mesh.
  bool SetMesh( ON::mesh_type, ON_Mesh* mesh );

  //Internal storage is now a std::shared_ptr to const ON_Mesh, so allow it to be set and accessed as such
  bool SetMesh(ON::mesh_type, const std::shared_ptr<const ON_Mesh>& mesh);

  const ON_Mesh* Mesh( ON::mesh_type mesh_type ) const;

  //Internal storage is now a std::shared_ptr to const ON_Mesh, so allow it to be set and accessed as such
  const std::shared_ptr<const ON_Mesh>& SharedMesh(ON::mesh_type mesh_type) const;

  //Note that while this function returns a std::shared_ptr<const ON_Mesh>, the ON_Mesh is guaranteed unique and
  //can be modified.
  const std::shared_ptr<const ON_Mesh>& UniqueMesh(ON::mesh_type mesh_type);

  /*
  Description:
    Destroy meshes used to render and analyze surface and polysurface objects.
  Parameters:
    mesh_type - [in] type of mesh to destroy
    bDeleteMesh - [in] if true, cached mesh is deleted.
      If false, pointer to cached mesh is just set to nullptr.
  See Also:
    CRhinoObject::GetMeshes
    CRhinoObject::MeshCount
    CRhinoObject::IsMeshable
  */
  ON_DEPRECATED_MSG("Support for bDeleteMesh no longer supported")
  void DestroyMesh( ON::mesh_type mesh_type, bool bDeleteMesh);

  /*
  Description:
    Destroy meshes used to render and analyze surface and polysurface objects.
  Parameters:
    mesh_type - [in] type of mesh to destroy
  See Also:
    CRhinoObject::GetMeshes
    CRhinoObject::MeshCount
    CRhinoObject::IsMeshable
  */
  void DestroyMesh(ON::mesh_type mesh_type);

  /////////////////////////////////////////////////////////////////
  // "Expert" Interface

  /*
  Description:
    Expert user tool that transforms all the parameter space (2d)
    trimming curves on this face.  Only 2d curve geometry is
    changed.  The caller is responsible for reversing loops,
    toggle m_bRev, flags, etc.
  Parameters:
    xform - [in] Transformation applied to 2d curve geometry.
  Returns
    True if successful.  If false is returned, the brep
    may be invalid.
  */
  bool TransformTrim( const ON_Xform& xform );

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Expert user tool that replaces the 3d surface geometry
    use by the face.
  Parameters;
    si - [in] brep surface index of new surface
    bTransformTrimCurves - [in]
      If unsure, then pass true.
      If the surface's domain has not changed and you are certain
      its parameterization still jibes with the trim curve
      locations, then pass false.
  Returns:
    True if successful.
  Example:

            ON_Surface* pSurface = ...;
            int si = brep.AddSurface(pSurface);
            face.ChangeSurface(si);

  Remarks:
    If the face had a surface and new surface has a different
    shape, then you probably want to call something like
    ON_Brep::RebuildEdges() to move the 3d edge curves so they
    will lie on the new surface. This doesn't delete the old 
    surface; call ON_Brep::CullUnusedSurfaces() or ON_Brep::Compact
    to remove unused surfaces.
    If the old surface has transcendental parameterization and the
    new surface does not, call RebuildTrimsForV2 with the new surface
    prior to calling ChangeSurface to get the trims in the
    correct parameterization.

  See Also:
    ON_Brep::RebuildEdges
    ON_Brep::CullUnusedSurfaces
    ON_Surface::GetNurbForm - use return value == 2 to get
       transcendental parameterization mismatch.
    ON_Brep::RebuildTrimsForV2
  */
  bool ChangeSurface(
    int si
    );
  bool ChangeSurface(
    int si,
    bool bTransformTrimCurves
    );
#endif
  
  /*
  Returns:
    brep.m_S[] surface index of the 3d surface geometry used by 
    this face or -1.
  */
  int SurfaceIndexOf() const;

  /*
  Returns:
    Pointer to the surface geometry used by the face.   
  */
  const ON_Surface* SurfaceOf() const;

#if defined(OPENNURBS_PLUS)

  /*
  Description:
    Get intervals where isocurve exists.
  Parameters:
    iso_dir - [in] 
      0: intervals are "u" intervals, c = constant "v" value
      1: intervals are "v" intervals, c = constant "u" value
    iso_constant - [in]
      constant "c" above
    intervals - [out]
      intervals are append to this list
      The isocurve(s) exist on the domain of the intervals.
  Returns:
    true if reliable information is returned.
    false if the input parameters or the brep is corrupt
    and the calculation could not be performed.
  */
  bool GetIsoIntervals(
    int iso_dir,
    double iso_constant,
    ON_SimpleArray<ON_Interval>& intervals
  ) const;

  bool GetIsoIntervals(
    int iso_dir,
    double iso_constant,
    ON_SimpleArray<struct ON_BrepFaceIsoInterval>& intervals
  ) const;

  /*
  Description:
    Get isocurves
  Parameters:
    iso_dir - [in] 
      0: intervals are "u" intervals, c = constant "v" value
      1: intervals are "v" intervals, c = constant "u" value
    iso_constant - [in]
      constant "c" above
    isocurves - [out]
      isocurves are appended to this list.
  Returns:
    number of curves appended to iso_curves[].
  Remarks:
    The returned curves are on the heap and the caller is 
    responsible for deleting them when they are no longer
    needed.
  */
  int GetIsoCurves(
    int iso_dir,
    double iso_constant,
    ON_SimpleArray<ON_Curve*>& iso_curves
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

#if defined(OPENNURBS_PLUS)
  /*
  Returns:
    If this face is part of a brep created by ON_SubD::ProxyBrep(), then
    the subd component id of the corresponding SubD face is returned. Otherwise 0 is returned.
  */
  unsigned int ProxyBrepSubDFaceId() const;
#endif

  ON_SimpleArray<int> m_li; // loop indices (outer loop is m_li[0])
  int m_si = -1;            // index of surface in b-rep m_S[] array
  bool m_bRev = false;         // true if face orientation is opposite
                       //      of natural surface orientation


  /*
  Returns:
    0: unset pack id.
    > 0: set pack id.
  Remarks:
    PackId values assigned to brep faces are inheritied from the PackId values
    assigned to subd faces when a subd is converted into a brep.
    These faces are "trivially trimmed" which means the boundary of the face
    is identical to the boundary of the underlying surface.
    There are two types of face packs in a subd, quad grid packs and singleton packs.
    A subd quad grid pack is a set of subd quads that form a rectangular grid.
    A subd singleton pack is a single face, quad or n-gon, that is not part of
    a quad grid pack. 
    There are three types of face packs in a brep created from a subd, 
    grid packs, star packs and singleton packs.
    A brep "grid pack" comes from a rectangular grid of subd quads. A grid pack of brep faces can
    be converted into a single larger trivially trimmed brep face.
    A brep "star pack" of brep faces comes from a singel subd n-gon (n = 3, 5 or more). The star pack
    will have n faces with a star center vertex and shared edges radiating from  the star center.
    A brep "singleton" pack comes from a single subd quad that could not be grouped into a larger
    subd quad grid pack.
  */
  unsigned int PackId() const;

  /*
  Description:
    Sets PackId() to zero.
  */
  void ClearPackId();

  /*
  Description:
    Used by ON_SubD functions that create breps to transmit the subd face ON_SubDFace.PackId() value 
    to the brep face or faces generated from the subd face.
    Unless you are an expert and doing something very carefully and very fancy, to not call this function.
  Remarks:
    PackId values assigned to brep faces are inheritied from the PackId values
    assigned to subd faces when a subd is converted into a brep.
    These faces are "trivially trimmed" which means the boundary of the face
    is identical to the boundary of the underlying surface.
    There are two types of face packs in a subd, quad grid packs and singleton packs.
    A subd quad grid pack is a set of subd quads that form a rectangular grid.
    A subd singleton pack is a single face, quad or n-gon, that is not part of
    a quad grid pack.
    There are three types of face packs in a brep created from a subd,
    grid packs, star packs and singleton packs.
    A brep "grid pack" comes from a rectangular grid of subd quads. A grid pack of brep faces can
    be converted into a single larger trivially trimmed brep face.
    A brep "star pack" of brep faces comes from a singel subd n-gon (n = 3, 5 or more). The star pack
    will have n faces with a star center vertex and shared edges radiating from  the star center.
    A brep "singleton" pack comes from a single subd quad that could not be grouped into a larger
    subd quad grid pack.
  */
  void SetPackIdForExperts(
    unsigned int pack_id
  );

private:
  ON__UINT8 m_reserved2 = 0;

private:
  // the 4 byte pack id is stored as 2 ON__UINT16 values to prevent breaking the C++ SDK.
  ON__UINT16 m_pack_id_high = 0; // PackId() = 0x10000*m_pack_id_high + m_pack_id_low;

public:
  // The application specifies a base ON_Material used to render the brep this face belongs to.
  // If m_material_channel_index > 0 AND face_material_id = base.MaterialChannelIdFromIndex(m_material_channel_index)
  // is not nil, then face_material_id identifies an override rendering material for this face.
  // Otherwise base will be used to render this face.
  int m_face_material_channel = 0;

public:
  /*
  Description:
    Set this face's rendering material channel index.

  Parameters:
    material_channel_index - [in]
      A value between 0 and ON_Material::MaximumMaterialChannelIndex, inclusive.
      This value is typically 0 or the value returned from ON_Material::MaterialChannelIndexFromId().

  Remarks:
    If base_material is the ON_Material assigned to render this brep and
    ON_UUID face_material_id = base_material.MaterialChannelIdFromIndex( material_channel_index )
    is not nil, then face_material_id identifies an override rendering material for this face.
    Otherwise base_material is used to render this face.
  */
  void SetMaterialChannelIndex(int material_channel_index) const;

  /*
  Description:
    Remove per face rendering material channel index setting. The face will use the material assigned to the brep object.
  */
  void ClearMaterialChannelIndex() const;

  /*
  Returns:
    This face's rendering material channel index.

    Remarks:
    If base_material is the ON_Material assigned to render this subd, MaterialChannelIndex() > 0,
    and ON_UUID face_material_id = base_material.MaterialChannelIdFromIndex( face.MaterialChannelIndex() )
    is not nil, then face_material_id identifies an override rendering material for this face.
    Otherwise base_material is used to render this face.
  */
  int MaterialChannelIndex() const;

  /*
  Description:
    Set per face color.

  Parameters:
    color - [in]
  */
  void SetPerFaceColor(
    ON_Color color
    ) const;

  /*
  Description:
    Remove per face color setting. The face will use the color assigned to the brep object.
  */
  void ClearPerFaceColor() const;

  /*
  Returns:
    Per face color. A value of ON_Color::UnsetColor indicates the face uses the color assigned to the brep object.
  */
  const ON_Color PerFaceColor() const;

public:

  // Persistent id for this face.  Default is ON_nil_uuid.
  ON_UUID m_face_uuid = ON_nil_uuid;

private:
  // RH-37306
  // 30 Mar 2020: Space reserved for future implementation.
  mutable ON_Color m_per_face_color = ON_Color::UnsetColor;

private:
  ON_BoundingBox m_bbox;      // 3d bounding box (should be declared mutable and const_cast<> is used to make it fake mutable)
  ON_Interval    m_domain[2]; // rectangular bounds of 2d curves
  //ON_Mesh* m_render_mesh = nullptr;
  //ON_Mesh* m_analysis_mesh = nullptr;
  //ON_Mesh* m_preview_mesh = nullptr;

  //New impl declaration to replace the old mesh pointers.
  class Impl;
  Impl* m_pImpl;
  friend Impl;

  //Pad out the size to match the old.
  void* m_reserved0 = nullptr;
  void* m_reserved1 = nullptr;

  //int m_material_index; // if 0 (default), ON_Brep's object attributes
  //                      // determine material.
private:
  friend class ON_Brep;
  ON_Brep* m_brep = nullptr;
  ON_BrepFace( const ON_BrepFace& ) = delete;

private:
  /*
  Parameters:
    bLazy - [in]
      If true and if ON_BrepFace.m_bbox is not empty, then ON_BrepFace.m_bbox is returned.
      In all other cases the bbox is calculated from scratch.
    bUpdateCachedBBox - [in]
      If true and the bounding box is calculated, then the value is saved in ON_BrepFace.m_bbox
      so future lazy evaluations can use the value.
  */
  const ON_BoundingBox InternalFaceBoundingBox(bool bLazy, bool bUpdateCachedBBox) const;
};

class ON_CLASS ON_BrepFaceSide : public ON_Object
{
  ON_OBJECT_DECLARE(ON_BrepFaceSide);
public:
  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  // Union available for application use.
  // The constructor zeros m_faceside_user.
  // The value is of m_faceside_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_faceside_user;

  // index of face side in ON_BrepRegionTopology.m_FS[] array
  int m_faceside_index;  

  ON_BrepFaceSide();
  ~ON_BrepFaceSide();
  ON_BrepFaceSide& operator=(const ON_BrepFaceSide&);

  bool Write(ON_BinaryArchive& binary_archive) const override;
  bool Read(ON_BinaryArchive& binary_archive) override;


  /*
  Returns:
   Brep this face side belongs to.
  */
  const class ON_Brep* Brep() const;

  /*
  Returns:
    Region topology this face side belongs to.
  */
  const class ON_BrepRegionTopology* RegionTopology() const;

  /*
  Returns:
   Region the face side belongs to.
  */
  const class ON_BrepRegion* Region() const;

  /*
  Returns:
   Face this side belongs to.
  */
  const class ON_BrepFace* Face() const;

  /*
  Returns:
   +1: underlying geometric surface normal points
       into region.
   -1: underlying geometric surface normal points
       out of region.
  */
  int SurfaceNormalDirection() const;

public:
  int m_ri; // region index 
            // m_ri = -1 indicates this face side overlaps
            // another face side. Generally this is a flaw
            // in an ON_Brep.
  int m_fi; // face index
  int m_srf_dir; //  1 ON_BrepFace's surface normal points into region
                 // -1 ON_BrepFace's surface normal points out of region

private:
  friend class ON_Brep;
  friend class ON_BrepRegionTopology;
  ON_BrepRegionTopology* m_rtop;
  ON_BrepFaceSide( const ON_BrepFaceSide& );
};

class ON_CLASS ON_BrepRegion : public ON_Object
{
  ON_OBJECT_DECLARE(ON_BrepRegion);
public:
  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  // Union available for application use.
  // The constructor zeros m_region_user.
  // The value is of m_region_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_region_user;

  // index of region in ON_BrepRegionTopology.m_R[] array
  int m_region_index;

  ON_BrepRegion();
  ~ON_BrepRegion();
  ON_BrepRegion& operator=(const ON_BrepRegion&);

  bool Write(ON_BinaryArchive& binary_archive) const override;
  bool Read(ON_BinaryArchive& binary_archive) override;

  /*
  Returns:
   Brep this region belongs to.
  */
  const ON_Brep* Brep() const;

  /*
  Returns:
    Region topology this region belongs to.
  */
  class ON_BrepRegionTopology* RegionTopology() const;

  /*
  Parameter:
    rfsi - [in] index into the region's m_fsi[] array.
  Returns:
    The face side in rtop.m_FS[m_fsi[rsi]], where
    rtop is the ON_BrepRegionTopology class this
    region belongs to.
  */
  ON_BrepFaceSide* FaceSide(int rfsi) const;

  /*
  Returns:
    True if the region is finite.
  */
  bool IsFinite() const;

  /*
  Returns:
   Region bounding box.
  */
  const ON_BoundingBox& BoundingBox() const;

  ON_SimpleArray<int> m_fsi; // indices of face sides
  int m_type; // 0 = infinite, 1 = bounded
  ON_BoundingBox m_bbox;

  /*
  Description:
    Get the boundary of a region as a brep object.  
    If the region is finite, the boundary will be a closed
    manifold brep.  The boundary may have more than one
    connected component.
  Parameters:
    brep - [in] if not nullptr, the brep form is put into
                this brep.
  Returns: the region boundary as a brep or nullptr if the
           calculation fails.
  */
  ON_Brep* RegionBoundaryBrep( ON_Brep* brep = nullptr ) const;

#if defined(OPENNURBS_PLUS)

  bool AreaMassProperties(
    ON_MassProperties& mp,
    bool bArea = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    double rel_tol = 1.0e-6,
    double abs_tol = 1.0e-6
    ) const;

  bool VolumeMassProperties(
    ON_MassProperties& mp, 
    bool bVolume = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    ON_3dPoint base_point = ON_3dPoint::UnsetPoint,
    double rel_tol = 1.0e-6,
    double abs_tol = 1.0e-6
    ) const;

  bool GetSilhouette(
    const ON_SilhouetteParameters parameters,
    size_t clipping_plane_count,
    const ON_PlaneEquation* clipping_planes,
    ON_ClassArray<ON_SIL_EVENT>& silhouettes,
    ON_ProgressReporter* progress,
    ON_Terminator* terminator
    ) const;

  bool IsPointInside(
          ON_3dPoint P, 
          double tolerance,
          bool bStrictlyInside
          ) const;
#endif

private:
  friend class ON_Brep;
  friend class ON_BrepRegionTopology;
  ON_BrepRegionTopology* m_rtop;
  ON_BrepRegion( const ON_BrepRegion& );
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_BrepVertex>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepVertex>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_BrepEdge>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepEdge>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_BrepTrim>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepTrim>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_BrepLoop>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepLoop>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_BrepFace>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepFace>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepFaceSide>;
ON_DLL_TEMPLATE template class ON_CLASS ON_ObjectArray<ON_BrepRegion>;
#endif

class ON_CLASS ON_BrepVertexArray : public ON_ObjectArray<ON_BrepVertex>
{
public:
  ON_BrepVertexArray();
  ~ON_BrepVertexArray();

  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;
};

class ON_CLASS ON_BrepEdgeArray   : public  ON_ObjectArray<ON_BrepEdge>
{
public:
  ON_BrepEdgeArray();
  ~ON_BrepEdgeArray();
  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;
};

class ON_CLASS ON_BrepTrimArray   : public  ON_ObjectArray<ON_BrepTrim>
{
public:
  ON_BrepTrimArray();
  ~ON_BrepTrimArray();
  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;
};

class ON_CLASS ON_BrepLoopArray   : public  ON_ObjectArray<ON_BrepLoop>
{
public:
  ON_BrepLoopArray();
  ~ON_BrepLoopArray();
  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;
};

class ON_CLASS ON_BrepFaceArray   : public  ON_ObjectArray<ON_BrepFace>
{
public:
  ON_BrepFaceArray();
  ~ON_BrepFaceArray();
  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;
};

class ON_CLASS ON_BrepFaceSideArray : public ON_ObjectArray<ON_BrepFaceSide>
{
public:
  ON_BrepFaceSideArray();
  ~ON_BrepFaceSideArray();

  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;

private:
  bool Internal_ReadV5( ON_BinaryArchive& );
  bool Internal_ReadV6( ON_BinaryArchive& );

  bool Internal_WriteV5( ON_BinaryArchive& ) const;
  bool Internal_WriteV6( ON_BinaryArchive& ) const;
};

class ON_CLASS ON_BrepRegionArray : public ON_ObjectArray<ON_BrepRegion>
{
public:
  ON_BrepRegionArray();
  ~ON_BrepRegionArray();

  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;

private:
  bool Internal_ReadV5( ON_BinaryArchive& );
  bool Internal_ReadV6( ON_BinaryArchive& );

  bool Internal_WriteV5( ON_BinaryArchive& ) const;
  bool Internal_WriteV6( ON_BinaryArchive& ) const;
};

class ON_CLASS ON_BrepRegionTopology
{
public:
  ON_BrepRegionTopology();
  ON_BrepRegionTopology(const ON_BrepRegionTopology& src);
  ~ON_BrepRegionTopology();
  ON_BrepRegionTopology& operator=(const ON_BrepRegionTopology&);

  ON_BrepFaceSideArray m_FS;
  ON_BrepRegionArray m_R;

#if defined(OPENNURBS_PLUS)
  bool Create(const ON_Brep* brep);
#endif

  const ON_Brep* Brep() const;
  bool IsValid( ON_TextLog* text_log = 0 ) const;
  bool Read( ON_BinaryArchive& );
  bool Write( ON_BinaryArchive& ) const;

  unsigned int SizeOf() const;

  bool Transform(
    const ON_Xform& xform
  );


private:
  friend class ON_V5_BrepRegionTopologyUserData;
  friend class ON_Brep;
  const ON_Brep* m_brep = nullptr;
};

class ON_CLASS ON_Brep : public ON_Geometry 
{
  ON_OBJECT_DECLARE(ON_Brep);

public:

  /////////////////////////////////////////////////////////////////
  //
  // Component status interface
  //
  //

  //virtual
  unsigned int ClearComponentStates(
    ON_ComponentStatus states_to_clear
    ) const override;

  //virtual
  unsigned int GetComponentsWithSetStates(
    ON_ComponentStatus states_filter,
    bool bAllEqualStates,
    ON_SimpleArray< ON_COMPONENT_INDEX >& components
    ) const override;
  
  //virtual
  unsigned int SetComponentStates(
    ON_COMPONENT_INDEX component_index,
    ON_ComponentStatus states_to_set
    ) const override;

  //virtual
  unsigned int ClearComponentStates(
    ON_COMPONENT_INDEX component_index,
    ON_ComponentStatus states_to_clear
    ) const override;
  
  //virtual
  unsigned int SetComponentStatus(
    ON_COMPONENT_INDEX component_index,
    ON_ComponentStatus status_to_copy
    ) const override;

  //virtual
 ON_AggregateComponentStatus AggregateComponentStatus() const override;

  //virtual
  void MarkAggregateComponentStatusAsNotCurrent() const override;

  // virtual ON_Object::DestroyRuntimeCache override
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Object::SizeOf override
  unsigned int SizeOf() const override;

  // virtual ON_Object::DataCRC override
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const override;

  // virtual ON_Geometry override
  bool EvaluatePoint( const class ON_ObjRef& objref, ON_3dPoint& P ) const override;

public:


  /*
  Description:
    Use ON_Brep::New() instead of new ON_Brep() when writing
    Rhino plug-ins (or when openNURBS is used as a Microsoft 
    DLL and you need to create a new ON_Brep in a different 
    .EXE or .DLL).
  Example:

              // bad - ON_Brep* pBrep = new ON_Brep();
              ON_Brep* pBrep = ON_Brep::New(); // good
              ...
              delete pBrep;
              pBrep = nullptr;

  Returns:
    Pointer to an ON_Brep.  Destroy by calling delete.
  Remarks:
    When openNURBS is used as a Microsoft DLL, the CL.EXE
    compiler uses local vtables for classes that are new-ed
    in other executables but uses the ordinary vtable for
    for classes that are allocated in functions like
    ON_BrepCylinder(), ON_NurbsSurfaceQuadrilateral(),
    ON_Cylinder::RevSurfaceForm(nullptr), etc.
    Using static New() functions like ON_Brep::New() insures
    that identical classes has the same vtable and makes
    all code run identically.
  */
  static ON_Brep* New();

  /*
  Description:
    Use ON_Brep::New(const ON_Brep& src) instead 
    of new ON_Brep(const ON_Brep& src).
  Returns:
    Pointer to an ON_Brep.  Destroy by calling delete.
  Remarks:
    See static ON_Brep* ON_Brep::New() for details.
  */
  static ON_Brep* New(const ON_Brep&);

	// Construction
  ON_Brep();
	~ON_Brep();		
  ON_Brep(const ON_Brep&);
  ON_Brep& operator=(const ON_Brep&);

  // Override of virtual ON_Object::MemoryRelocate
  void MemoryRelocate() override;


  /*
  Description:
    Does nothing. Will be deleted in next version.
  */
  ON_DEPRECATED_MSG("Does nothing. Delete call.") 
  bool IsDuplicate( 
    const ON_Brep& other, 
    double tolerance = ON_ZERO_TOLERANCE 
    ) const;

  /////////////////////////////////////////////////////////////////
  // construction/destruction helpers

  // returns Brep to state it has after default construction
  void Destroy(); 

  // call if memory pool used by b-rep members becomes invalid
  void EmergencyDestroy(); 

  /*
  Description:
    Calculates polygon mesh approximation of the brep
    and appends one mesh for each face to the mesh_list[]
    array.
  Parameters:
    mp - [in] meshing parameters
    mesh_list - [out] meshes are appended to this array.
  Returns:
    Number of meshes appended to mesh_list[] array.
  Note:
    This function is not thread safe.  
  */
  int CreateMesh( 
    const ON_MeshParameters& mp,
    ON_SimpleArray<ON_Mesh*>& mesh_list
    ) const;

  /*
  Description:
    Destroy meshes used to render and analyze brep.
  Parameters:
    mesh_type - [in] type of mesh to destroy
    bDeleteMesh - [in] if true, cached meshes are deleted.
      If false, pointers to cached meshes are just set to nullptr.
  See Also:
    ON_Brep::GetMesh
    ON_BrepFace::DestroyMesh
    ON_BrepFace::Mesh
    ON_BrepFace::SetMesh
  */
  ON_DEPRECATED_MSG("bDeleteMesh=false is no longer supported")
  void DestroyMesh( ON::mesh_type mesh_type, bool bDeleteMesh);

  /*
  Description:
    Destroy meshes used to render and analyze brep.
  Parameters:
    mesh_type - [in] type of mesh to destroy
  See Also:
    ON_Brep::GetMesh
    ON_BrepFace::DestroyMesh
    ON_BrepFace::Mesh
    ON_BrepFace::SetMesh
  */
  void DestroyMesh(ON::mesh_type mesh_type);

  /*
  Description:
    Get cached meshes used to render and analyze brep.
  Parameters:
    mesh_type - [in] type of mesh to get
    meshes - [out] meshes are appended to this array.  The ON_Brep
      owns these meshes so they cannot be modified.
  Returns:
    Number of meshes added to array. (Same as m_F.Count())
  See Also:
    ON_Brep::DestroyMesh
    ON_BrepFace::DestroyMesh
    ON_BrepFace::Mesh
    ON_BrepFace::SetMesh
  */
  int GetMesh( ON::mesh_type mesh_type, ON_SimpleArray< const ON_Mesh* >& meshes ) const;


#if defined(OPENNURBS_PLUS)
  /// <summary>
  /// Sets the vertex colors of the brep's faces' analysis meshes from the principal surface curvatures.
  /// 
  /// The color ON_MappingTag::CurvatureAnalysisColor(kappa_style,kappa_range)
  /// is assigned to the fragments's vertex color setting.
  /// 
  /// The SHA1 hash ON_MappingTag::CurvatureColorAnalysisParametersHash(kappa_style,kappa_range)
  /// is used to identify this method of setting the fragments' vertex colors when 
  /// SHA1 hashes are used.
  ///
  /// The mapping tag ON_MappingTag::CurvatureAnalysisMappingTag(kappa_style,kappa_range)
  /// is used to identify this method of setting the fragments' vertex colors when 
  /// mapping tags are used.
  /// 
  /// See ON_SurfaceCurvature::KappaValue()
  /// </summary>
  /// <param name="bLazySet"></param>
  /// If bLazySet and a fragment has a matching tag, hash, and set vertex colors, then
  /// the existing colors are assumed to be correctly set. When in doubt, pass true.
  /// <param name="kappa_colors">
  /// kappa_colors.Color(K) is the color assigned to the surface principal curvatures K.
  /// </param>
  /// <returns></returns>
  bool SetCurvatureColorAnalysisColors(
    bool bLazySet,
    ON_SurfaceCurvatureColorMapping kappa_colors
  ) const;
#endif

#if defined(OPENNURBS_PLUS)
  /// <summary>
  /// Sets the vertex colors of the brep's faces' analysis meshes from the draft angle of the surface normals.  /// 
  /// </summary>
  /// <param name="bLazySet">
  /// If bLazySet and a fragment has a matching tag, hash, and set vertex colors, then
  /// the existing colors are assumed to be correctly set. When in doubt, pass true.
  /// </param>
  /// <param name="draft_angle_colors"></param>
  /// <returns></returns>
  bool SetDraftAngleColorAnalysisColors(
    bool bLazySet,
    ON_SurfaceDraftAngleColorMapping draft_angle_colors
  ) const;
#endif

#if defined(OPENNURBS_PLUS)
  public:
  /*
  Description:
    Convert brep face grid packs into single faces.
  Parameters:
    from_subd - [in] true if the Brep to pack comes from a SubD to NURBS conversion and has PackIds
                     set on all its faces.
  Returns:
    Number of brep face grid packs converted to single faces.
  Remarks:
    PackId values assigned to brep faces are inheritied from the PackId values
    assigned to subd faces when a subd is converted into a brep.
    These faces are "trivially trimmed" which means the boundary of the face
    is identical to the boundary of the underlying surface.
    There are two types of face packs in a subd, quad grid packs and singleton packs.
    A subd quad grid pack is a set of subd quads that form a rectangular grid.
    A subd singleton pack is a single face, quad or n-gon, that is not part of
    a quad grid pack.
    There are three types of face packs in a brep created from a subd,
    grid packs, star packs and singleton packs.
    A brep "grid pack" comes from a rectangular grid of subd quads. A grid pack of brep faces can
    be converted into a single larger trivially trimmed brep face.
    A brep "star pack" of brep faces comes from a singel subd n-gon (n = 3, 5 or more). The star pack
    will have n faces with a star center vertex and shared edges radiating from  the star center.
    A brep "singleton" pack comes from a single subd quad that could not be grouped into a larger
    subd quad grid pack.
    */
  unsigned int PackFaces(bool from_subd = true);
#endif

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Calculate area mass properties of the brep.
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
    ON_MassProperties& mp,
    bool bArea = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    double rel_tol = 1.0e-6,
    double abs_tol = 1.0e-6
    ) const;

  /*
  Description:
    Calculate volume mass properties of the brep.
  Parameters:
    mp - [out] 
    bVolume - [in] true to calculate volume
    bFirstMoments - [in] true to calculate volume first moments,
                         volume, and volume centroid.
    bSecondMoments - [in] true to calculate volume second moments.
    bProductMoments - [in] true to calculate volume product moments.
    base_point - [in] 
      If the brep is closed, then pass ON_UNSET_VALUE.

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
    ON_MassProperties& mp, 
    bool bVolume = true,
    bool bFirstMoments = true,
    bool bSecondMoments = true,
    bool bProductMoments = true,
    ON_3dPoint base_point = ON_3dPoint::UnsetPoint,
    double rel_tol = 1.0e-6,
    double abs_tol = 1.0e-6
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

  /*
  Description:
    Create a brep from a surface.  The resulting surface has an outer
    boundary made from four trims.  The trims are ordered so that
    they run along the south, east, north, and then west side of the
    surface's parameter space.
  Parameters:
    pSurface - [in] pointer to a surface.  The brep will manage this
       pointer and delete it in ~ON_Brep.
  Returns:
    @untitled table
    true     successful
      When true is returned, the pSurface pointer is added to the
      brep's m_S[] array and it will be deleted by the brep's
      destructor.
    false
      brep cannot be created from this surface.
      When false is returned, then the caller is responsible
      for deleting pSurface unless it was previously added
      to the brep's m_S[] array.     
  Remarks:
    The surface class must be created with new so that the
    delete in ~ON_Brep will not cause a crash.
  */
  bool Create( 
          ON_Surface*& pSurface
          );

  bool Create( 
          ON_NurbsSurface*& pNurbsSurface
          );

  bool Create( 
          ON_PlaneSurface*& pPlaneSurface
          );

  bool Create( 
          ON_RevSurface*& pRevSurface
          );

  bool Create( 
          ON_SumSurface*& pSumSurface
          );

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

  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;

  /*
  Description:
    Tests the brep to see if its topology information is
    valid.
  Parameters:
    text_log - [in] if the brep topology is not valid and 
        text_log is not nullptr, then a brief English 
        description of the problem is appended to the log.
        The information appended to text_log is suitable for 
        low-level debugging purposes by programmers and is 
        not intended to be useful as a high level user 
        interface tool.
  Returns:
    @untitled table
    true     brep topology is valid
    false    brep topology is not valid
  Remarks:
    ON_Brep::IsValidTopology can be called at any time.
  See Also:
    ON_Brep::IsValid
    ON_Brep::IsValidGeometry
    ON_Brep::IsValidTolerancesAndFlags
  */
  bool IsValidTopology( ON_TextLog* text_log = nullptr ) const;


  /*
  Description:
    Expert user function that tests the brep to see if its
    geometry information is valid.  The value of 
    brep.IsValidTopology() must be true before 
    brep.IsValidGeometry() can be safely called.
  Parameters:
    text_log - [in] if the brep geometry is not valid and 
        text_log is not nullptr, then a brief English 
        description of the problem is appended to the log.
        The information appended to text_log is suitable for 
        low-level debugging purposes by programmers and is 
        not intended to be useful as a high level user 
        interface tool.
  Returns:
    @untitled table
    true     brep geometry is valid
    false    brep geometry is not valid
  Remarks:
    ON_Brep::IsValidTopology must be true before you can
    safely call ON_Brep::IsValidGeometry.
  See Also:
    ON_Brep::IsValid
    ON_Brep::IsValidTopology
    ON_Brep::IsValidTolerancesAndFlags
  */
  bool IsValidGeometry( ON_TextLog* text_log = nullptr ) const;

  /*
  Description:
    Expert user function that tests the brep to see if its
    tolerances and flags are valid.  The values of 
    brep.IsValidTopology() and brep.IsValidGeometry() must
    be true before brep.IsValidTolerancesAndFlags() can 
    be safely called.
  Parameters:
    text_log - [in] if the brep tolerance or flags are not
        valid and text_log is not nullptr, then a brief English 
        description of the problem is appended to the log.
        The information appended to text_log is suitable for 
        low-level debugging purposes by programmers and is 
        not intended to be useful as a high level user 
        interface tool.
  Returns:
    @untitled table
    true     brep tolerance and flags are valid
    false    brep tolerance and flags are not valid
  Remarks:
    ON_Brep::IsValidTopology and ON_Brep::IsValidGeometry
    must be true before you can safely call
    ON_Brep::IsValidTolerancesAndFlags.
  See Also:
    ON_Brep::IsValid
    ON_Brep::IsValidTopology
    ON_Brep::IsValidGeometry
  */
  bool IsValidTolerancesAndFlags( ON_TextLog* text_log = nullptr ) const;

  // Description:
  //   Tests brep to see if it is valid for 
  //   saving in V2 3DM archives.
  // Returns:
  //   true if brep is valid for V2 3DM archives.
  // Remarks:
  //   V2 breps could not have dangling curves.
  bool IsValidForV2() const;
  bool IsValidForV2( const ON_BrepTrim& ) const;
  bool IsValidForV2( const ON_BrepEdge& ) const;

  #if defined(OPENNURBS_PLUS)

  // Description:
  //   Change brep so it is valid for saving
  //   in V2 3DM archives.
  bool MakeValidForV2();


/*
  Description:
    No support is available for this function.
    Expert user function used by MakeValidForV2 to
    convert trim curves from one surface to its
    NURBS form.  After calling this function, you
    need to change the face's surface to nurbs_surface.
  Parameters:
    face - [in] face whose underlying surface has
                a parameterization that is different
                from its NURBS form.
    nurbs_surface - [in] NURBS form of the face's
                underlying surface
  Remarks:
    Don't call this function unless you know exactly
    what you are doing.  No support is available.
  */
  void RebuildTrimsForV2(
          ON_BrepFace& face, 
          const ON_NurbsSurface& nurbs_surface
          );

/*
  Description:
    No support is available for this function.
    Expert user function used by MakeValidForV2 to
    convert trim curves from one surface to its
    NURBS form.  After calling this function, you
    need to change the face's surface to nurbs_surface.
  Parameters:
    face - [in] face whose underlying surface has
                a parameterization that is different
                from its NURBS form.
    nurbs_surface - [in] NURBS form of the face's
                underlying surface
    min_tol - [in] 3D edges are pulled back using the max of edge tolerance and this 
  Remarks:
    Don't call this function unless you know exactly
    what you are doing.  No support is available.
  */
  void RebuildTrimsForV2(
          ON_BrepFace& face, 
          const ON_NurbsSurface& nurbs_surface,
          double min_tol
          );

  /*
  Description:
    Split any faces with creases into G1 pieces.
  Parameters:
    kink_tol_radians - [in] kink tolerance
    bCompactIfNeeded - [in]
      If true and splitting is performed, ON_Brep::Compact()
      will be called to clean up the unused parts.
  Returns:
    True if successful.
  Remarks:
    If you need to detect when splitting occurred, compare the
    before and after values of m_F.Count().
  */
  bool SplitKinkyFaces( 
          double kink_tol_radians = ON_DEFAULT_ANGLE_TOLERANCE,
          bool bCompactIfNeeded = true
          );

  /*
  Description:
    Split the face into G1 pieces.
  Parameters:
    face_index - [in] Index of the face to test and split.
    kink_tol_radians - [in] kink tolerance
  Returns:
    True if successful.
  Remarks:
    This function leaves deleted stuff in the brep.  
    Call ON_Brep::Compact() to remove deleted stuff.
  */
  virtual
  bool SplitKinkyFace( 
    int face_index, 
    double kink_tol_radians = ON_DEFAULT_ANGLE_TOLERANCE // ON_PI/180.0
    );

  /*
  Description:
    Split the edge into G1 pieces.
  Parameters:
    edge_index - [in] Index of the edge to test and split.
    kink_tol_radians - [in] kink tolerance
  Returns:
    True if successful.
  Remarks:
    This function leaves deleted stuff in the brep.  
    Call ON_Brep::Compact() to remove deleted stuff.
  */
  virtual
  bool SplitKinkyEdge( 
    int edge_index, 
    double kink_tol_radians = ON_DEFAULT_ANGLE_TOLERANCE //ON_PI/180.0
    );

  /*
  Description:
    Split an edge at the specified parameters.
  Parameters:
    edge_index - [in] Index of the edge to split.
    edge_t_count - [in]
      number of edge parameters
    edge_t - [in]
      edge parameters
  Returns:
    Number of splits applied to the edge.
  Remarks:
    This function leaves deleted stuff in the brep.  
    Call ON_Brep::Compact() to remove deleted stuff.
  */
  int SplitEdgeAtParameters(
    int edge_index,
    int edge_t_count,
    const double* edge_t
    );
  #endif

  // virtual ON_Objet::Dump() override
  void Dump( ON_TextLog& ) const override; // for debugging

  // virtual ON_Objet::Write() override
  bool Write( ON_BinaryArchive& ) const override;

  // virtual ON_Objet::Read() override
  bool Read( ON_BinaryArchive& ) override;

  // virtual ON_Objet::ObjectType() override
  ON::object_type ObjectType() const override;

  // virtual ON_Geometry::Dimension() override
  int Dimension() const override;

  // virtual ON_Geometry::ClearBoundingBox() override
  void ClearBoundingBox() override;

  // virtual ON_Geometry GetTightBoundingBox override		
  bool GetTightBoundingBox( class ON_BoundingBox& tight_bbox, bool bGrowBox = false, const class ON_Xform* xform = nullptr ) const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  // virtual ON_Geometry::Transform() override
  bool Transform( 
         const ON_Xform&
         ) override;

#if defined(OPENNURBS_PLUS)
  // virtual ON_Geometry::IsDeformable() override
  bool IsDeformable() const override;

  // virtual ON_Geometry::MakeDeformable() override
  bool MakeDeformable() override;
#endif

  // virtual ON_Geometry::SwapCoordinates() override
  bool SwapCoordinates(
        int, int        // indices of coordinates to swap
        ) override;

#if defined(OPENNURBS_PLUS)
  // virtual ON_Geometry override
  bool Morph( const ON_SpaceMorph& morph ) override;

  // virtual ON_Geometry override
  bool IsMorphable() const override;
#endif

  // virtual ON_Geometry::HasBrepForm() override
  bool HasBrepForm() const override; // returns true

  /*
  Description:
    If possible, BrepForm() creates a brep form of the
    ON_Geometry. 
  Parameters:
    brep - [in] if not nullptr, brep is used to store the brep
        form of the geometry.
  Result:
    If brep is not nullptr, then brep = this, otherwise
    a duplicate of this is returned.
  Remarks:
    Override of virtual ON_Geometry::BrepForm
  */
  ON_Brep* BrepForm( ON_Brep* brep = nullptr ) const override;

  /////////////////////////////////////////////////////////////////
  // Creation Interface

  // These add a new geometry piece to the b-rep and return the
  // index that should be used to reference the geometry.
  // -1 is returned if the input is not acceptable.
  // ~ON_Brep() will delete the geometry.
  int AddTrimCurve( ON_Curve* ); // 2d curve used by ON_BrepTrim
  int AddEdgeCurve( ON_Curve* ); // 3d curve used by ON_BrepEdge
  int AddSurface( ON_Surface* ); // 3d surface used by ON_BrepFace

  // Description:
  //   Set 3d curve geometry used by a b-rep edge.
  // Parameters:
  //   edge - [in]
  //   c3_index - [in] index of 3d curve in m_C3[] array
  //   sub_domain - [in] if not nullptr, sub_domain is an increasing
  //      sub interval of m_C3[c3_index]->Domain().
  // Returns:
  //   true if successful.
  bool SetEdgeCurve( 
    ON_BrepEdge& edge,
    int c3_index,
    const ON_Interval* sub_domain = nullptr
    );

  // Description:
  //   Set 2d curve geometry used by a b-rep trim.
  // Parameters:
  //   trim - [in]
  //   c2_index - [in] index of 2d curve in m_C2[] array
  //   sub_domain - [in] if not nullptr, sub_domain is an increasing
  //      sub interval of m_C2[c2_index]->Domain().
  // Returns:
  //   true if successful.
  bool SetTrimCurve( 
    ON_BrepTrim& trim,
    int c2_index,
    const ON_Interval* sub_domain = nullptr
    );

  // These add a new topology piece to the b-rep and return a 
  // reference that is intended to be used for initialization.
  ON_BrepVertex& NewVertex();
  ON_BrepVertex& NewVertex( 
    ON_3dPoint vertex_point,
    double vertex_tolerance = ON_UNSET_VALUE
    );

  ON_BrepEdge& NewEdge(
                  int = -1              // 3d curve index
                  );
  ON_BrepEdge& NewEdge( 
                  ON_BrepVertex&, // start vertex
                  ON_BrepVertex&, // end vertex
                  int = -1,       // 3d curve index
                  const ON_Interval* = nullptr, // sub_domain
                  double edge_tolerance = ON_UNSET_VALUE
                  );

  /*
  Description:
    Add a new face to a brep.  An incomplete face is added.
    The caller must create and fill in the loops used by
    the face.
  Parameters:
    si - [in] index of surface in brep's m_S[] array
  Returns:
    Reference to new face.
  Remarks:
    Adding a new face may grow the dynamic m_F array.  When
    this happens pointers and references to memory in the
    previous m_F[] array may become invalid.  Use face indices
    if this is an issue.
  Example:
    See ON_BrepBox and ON_BrepSphere source code.
  See Also:
    ON_Brep::AddSurface
  */
  ON_BrepFace& NewFace(
                  int si = -1
                  );

  /*
  Description:
    Add a new face to a brep.  This creates a complete face with
    new vertices at the surface corners, new edges along the surface
    boundary, etc.  The loop of the returned face has four trims that
    correspond to the south, east, north, and west side of the 
    surface in that order.  If you use this version of NewFace to
    add an exiting brep, then you are responsible for using a tool
    like ON_Brep::JoinEdges() to hook the new face to its
    neighbors.
  Parameters:
    surface - [in] surface is copied.
  Returns:
    Pointer to new face.
  Remarks:
    Adding a new face may grow the dynamic arrays used to store
    vertices, edges, faces, loops, and trims.  When these dynamic
    arrays are grown, any pointers and references to memory in
    the previous arrays may become invalid.  Use indices
    if this is an issue.
  See Also:
    ON_Brep::JoinEdges
    ON_Brep::AddSurface
  */
  ON_BrepFace* NewFace( 
    const ON_Surface& surface 
    );

  /*
  Description:
    Add a new face to brep.  This version is for expert users.
  Parameters:
    pSurface - [in] the returned face will have an outer loop
                    that goes around the edges of the surface.
    vid - [in/out] four vertex indices that specify the vertices at
                   the (sw,se,nw,ne) corners.  If the input value
                   of a vertex index is -1, then the vertex will be 
                   created.
    eid - [in/out] four edge indices that specify the edges for
                   the (south,east,north,west) sides.  If the input value
                   of an edge index is -1, then the edge will be created.
    bRev3d - [in/out] four values of the trim m_bRev3d flags of
                   the (south,east,north,west) sides.
  Returns:
    Pointer to the new face or nullptr if input is not valid.
    If null is returned, then the caller must delete pSurace
    unless it was previously added to the brep's m_S[] array.
  Remarks:
    Adding a new face may grow the dynamic m_F array.  When
    this happens pointers and references to memory in the
    previous m_F[] array may become invalid.  Use face indices
    if this is an issue.
  Example:
    See ON_BrepBox and ON_BrepSphere source code.
  See Also:
    ON_Brep::AddSurface
    ON_Brep::AddFace( int si )
    ON_Brep::Create( ON_Surface*& )
  */
  ON_BrepFace* NewFace(
       ON_Surface* pSurface,
       int vid[4],
       int eid[4],
       bool bRev3d[4]
       );

  /*
  Description:
    Add a new face to the brep whose surface geometry is a 
    ruled surface between two edges.
  Parameters:
    edgeA - [in] The south side of the face's surface will
          run along edgeA.
    bRevEdgeA - [in] true if the new face's outer boundary
          orientation along edgeA is opposite the orientation
          of edgeA.
    edgeB - [in] The north side of the face's surface will
          run along edgeA.
    bRevEdgeB - [in] true if the new face's outer boundary
          orientation along edgeB is opposite the orientation
          of edgeB.
  Returns:
    A pointer to the new face or a nullptr if the new face could
    not be created.
  */
  ON_BrepFace* NewRuledFace(
        const ON_BrepEdge& edgeA,
        bool bRevEdgeA,
        const ON_BrepEdge& edgeB, 
        bool bRevEdgeB
        );

  /*
  Description:
    Add a new face to the brep whose surface geometry is a 
    ruled cone with the edge as the base and the vertex as
    the apex point.
  Parameters:
    vertex - [in] The apex of the cone will be at this vertex.
                   The north side of the surface's parameter
                   space will be a singular point at the vertex.
    edge - [in] The south side of the face's surface will
          run along this edge.
    bRevEdge - [in] true if the new face's outer boundary
          orientation along the edge is opposite the 
          orientation of edge.
  Returns:
    A pointer to the new face or a nullptr if the new face could
    not be created.
  */
  ON_BrepFace* NewConeFace(
        const ON_BrepVertex& vertex,
        const ON_BrepEdge& edge,
        bool bRevEdge
        );

  /*
  Description:
    Create a new empty boundary loop. The new loop will not be part of a face and 
    will not include any trim curves.
  Returns:
    New boundary loop.
  */
  ON_BrepLoop& NewLoop( ON_BrepLoop::TYPE );

  /*
  Description:
    Create a new boundary loop on a face.  After you get this
    ON_BrepLoop, you still need to create the vertices, edges, 
    and trims that define the loop.
  Returns:
    New loop that needs to be filled in.
  */
  ON_BrepLoop& NewLoop( ON_BrepLoop::TYPE loop_type, ON_BrepFace& face );

  /*
  Description:
    Create a new outer boundary loop that runs along the sides
    of the face's surface.  All the necessary trims, edges,
    and vertices are created and added to the brep.
  Parameters:
    face_index - [in] index of face that needs an outer boundary
                      that runs along the sides of its surface.
  Returns:
    New outer boundary loop that is complete.
  */
  ON_BrepLoop* NewOuterLoop( int face_index );

  /*
  Description:
    Add a new face to brep.  This version is for expert users.
  Parameters:
    face_index - [in] index of face that will get a new outer
                   loop running around the sides of the face's
                   underlying surface.
    vid - [in/out] four vertex indices that specify the vertices at
                   the (sw,se,nw,ne) corners.  If the input value
                   of a vertex index is -1, then the vertex will be 
                   created.
    eid - [in/out] four edge indices that specify the edges for
                   the (south,east,north,west) sides.  If the input value
                   of an edge index is -1, then the edge will be created.
    bRev3d - [in/out] four values of the trim m_bRev3d flags of
                   the (south,east,north,west) sides.
  Returns:
    Pointer to the new loop or nullptr if input is not valid.
  Remarks:
    Adding a new loop may grow the dynamic m_L array.  When
    this happens pointers and references to memory in the
    previous m_L[] array may become invalid.  Use face indices
    if this is an issue.
  See Also:
    ON_Brep::NewFace
  */
  ON_BrepLoop* NewOuterLoop(
         int face_index,
         int vid[4],
         int eid[4],
         bool bRev3d[4]
         );

  /*
  Description:
    Add a planar trimming loop to a planar face.
  Parameters:
    face_index - [in] index of planar face.  The underlying
        surface must be an ON_PlaneSurface.
    loop_type - [in] type of loop to add.  If loop_type is
        ON_BrepLoop::unknown, then the loop direction is tested
        and the the new loops type will be set to 
        ON_BrepLoop::outer or ON_BrepLoop::inner.  If the loop_type
        is ON_BrepLoop::outer, then the direction of the new loop
        is tested and flipped if it is clockwise. If the loop_type
        is ON_BrepLoop::inner, then the direction of the new loop
        is tested and flipped if it is counter-clockwise.
    boundary - [in] a list of 3d curves that form a simple (no self
        intersections) closed curve.  These curves define the 3d
        edge geometry and should be near the planar surface.
    bDuplicateCurves - [in] If true, then duplicates of the curves 
        in the boundary array are added to the brep.  If false, the
        curves in the boundary array are added to the brep and will
        be deleted by ON_Brep::~ON_Brep.
    Returns:
      true if successful.  The new loop will be brep.m_L.Last().
  */
  bool NewPlanarFaceLoop(
        int face_index,
        ON_BrepLoop::TYPE loop_type,
        ON_SimpleArray<ON_Curve*>& boundary,
        bool bDuplicateCurves = true
        );


  /*
  Description:
    Add a new trim that will be part of an inner, outer, or slit loop
    to the brep.
  Parameters:
    c2i - [in] index of 2d trimming curve
  Returns:
    new trim
  Example:
    int c2i = brep->AddTrimCurve( p2dCurve );
    ON_BrepTrim& trim = NewTrim( edge, bRev3d, loop, c2i );
    trim.m_ei = ...;
    trim.m_li = ...;
    trim.m_tolerance[0] = ...;
    trim.m_tolerance[1] = ...;
    trim.m_type = ...;
    trim.m_iso = ...;
  Remarks:
    You should set the trim's ON_BrepTrim::m_tolerance, ON_BrepTrim::m_type,
    ON_BrepTrim::m_iso, ON_BrepTrim::m_li, and ON_BrepTrim::m_ei values.
        In general, you should try to use the
    ON_BrepTrim::NewTrim( edge, bRev3d, loop, c2i ) version of NewTrim.
    If you want to add a singular trim, use ON_Brep::NewSingularTrim.
    If you want to add a crvonsrf trim, use ON_Brep::NewCurveOnFace.
    If you want to add a ptonsrf trim, use ON_Brep::NewPointOnFace.
  See Also:
    ON_Brep::SetTrimTypeFlags
    ON_Brep::SetTrimIsoFlags
    ON_Brep::NewSingularTrim
    ON_Brep::NewPointOnFace
    ON_Brep::NewCurveOnFace
  */
  ON_BrepTrim& NewTrim(
                  int c2i = -1
                  );

  /*
  Description:
    Add a new trim that will be part of an inner, outer, or slit loop
    to the brep.
  Parameters:
    bRev3d - [in] ON_BrepTrim::m_bRev3d value.  true if the
       edge and trim have opposite directions.
    loop - [in] trim is appended to this loop
    c2i - [in] index of 2d trimming curve
  Returns:
    new trim
  Example:
    int c2i = brep->AddTrimCurve( p2dCurve );
    ON_BrepTrim& trim = NewTrim( edge, bRev3d, loop, c2i );
    trim.m_ei = ...;
    trim.m_tolerance[0] = ...;
    trim.m_tolerance[1] = ...;
    trim.m_type = ...;
    trim.m_iso = ...;
  Remarks:
    You should set the trim's ON_BrepTrim::m_tolerance, ON_BrepTrim::m_type,
    ON_BrepTrim::m_iso, and ON_BrepTrim::m_ei values.
        In general, you should try to use the
    ON_BrepTrim::NewTrim( edge, bRev3d, loop, c2i ) version of NewTrim.
    If you want to add a singular trim, use ON_Brep::NewSingularTrim.
    If you want to add a crvonsrf trim, use ON_Brep::NewCurveOnFace.
    If you want to add a ptonsrf trim, use ON_Brep::NewPointOnFace.
  See Also:
    ON_Brep::SetTrimTypeFlags
    ON_Brep::SetTrimIsoFlags
    ON_Brep::NewSingularTrim
    ON_Brep::NewPointOnFace
    ON_Brep::NewCurveOnFace
  */
  ON_BrepTrim& NewTrim(
                  bool bRev3d,
                  ON_BrepLoop& loop,
                  int c2i = -1
                  );

  /*
  Description:
    Add a new trim that will be part of an inner, outer, or slit loop
    to the brep.
  Parameters:
    edge - [in] 3d edge associated with this trim
    bRev3d - [in] ON_BrepTrim::m_bRev3d value.  true if the
       edge and trim have opposite directions.
    c2i - [in] index of 2d trimming curve
  Returns:
    new trim
  Example:
    int c2i = brep->AddTrimCurve( p2dCurve );
    ON_BrepTrim& trim = NewTrim( edge, bRev3d, c2i );
    trim.m_li = ...;
    trim.m_tolerance[0] = ...;
    trim.m_tolerance[1] = ...;
    trim.m_type = ...;
    trim.m_iso = ...;
  Remarks:
    You should set the trim's ON_BrepTrim::m_tolerance, 
    ON_BrepTrim::m_type, ON_BrepTrim::m_iso, 
    and ON_BrepTrim::m_li values.
        In general, you should try to use the
    ON_BrepTrim::NewTrim( edge, bRev3d, loop, c2i ) version of NewTrim.
    If you want to add a singular trim, use ON_Brep::NewSingularTrim.
    If you want to add a crvonsrf trim, use ON_Brep::NewCurveOnFace.
    If you want to add a ptonsrf trim, use ON_Brep::NewPointOnFace.
  See Also:
    ON_Brep::SetTrimTypeFlags
    ON_Brep::SetTrimIsoFlags
    ON_Brep::NewSingularTrim
    ON_Brep::NewPointOnFace
    ON_Brep::NewCurveOnFace
  */
  ON_BrepTrim& NewTrim(
                  ON_BrepEdge& edge,
                  bool bRev3d,
                  int c2i = -1
                  );

  /*
  Description:
    Add a new trim that will be part of an inner, outer, or slit loop
    to the brep.
  Parameters:
    edge - [in] 3d edge associated with this trim
    bRev3d - [in] ON_BrepTrim::m_bRev3d value.  true if the
       edge and trim have opposite directions.
    loop - [in] trim is appended to this loop
    c2i - [in] index of 2d trimming curve
  Returns:
    new trim
  Example:
    int c2i = brep->AddTrimCurve( p2dCurve );
    ON_BrepTrim& trim = brep->NewTrim( edge, bRev3d, loop, c2i );
    trim.m_tolerance[0] = ...;
    trim.m_tolerance[1] = ...;
  Remarks:
    You should set the trim's ON_BrepTrim::m_tolerance values.
    If c2i is -1, you must set the trim's ON_BrepTrim::m_iso values.
    This version of NewTrim sets the trim.m_type value.  If the
    input edge or loop are not currently valid, then you may
    need to adjust the trim.m_type value.
    If you want to add a singular trim, use ON_Brep::NewSingularTrim.
    If you want to add a crvonsrf trim, use ON_Brep::NewCurveOnFace.
    If you want to add a ptonsrf trim, use ON_Brep::NewPointOnFace.
  See Also:
    ON_Brep::SetTrimTypeFlags
    ON_Brep::SetTrimIsoFlags
    ON_Brep::NewSingularTrim
    ON_Brep::NewPointOnFace
    ON_Brep::NewCurveOnFace
  */
  ON_BrepTrim& NewTrim(
                  ON_BrepEdge& edge,
                  bool bRev3d,
                  ON_BrepLoop& loop,
                  int c2i = -1
                  );

  /*
  Description:
    Add a new singular trim to the brep.
  Parameters:
    vertex - [in] vertex along collapsed surface edge
    loop - [in] trim is appended to this loop
    iso - [in] one of ON_Surface::S_iso, ON_Surface::E_iso, 
               ON_Surface::N_iso, or ON_Surface::W_iso.
    c2i - [in] index of 2d trimming curve
  Returns:
    new trim
  See Also:
    ON_Brep::NewTrim
  */
  ON_BrepTrim& NewSingularTrim(
                  const ON_BrepVertex& vertex,
                  ON_BrepLoop& loop,
                  ON_Surface::ISO iso,
                  int c2i = -1
                  );

  /*
  Description:
    Adds a new point on face to the brep.
  Parameters:
    face - [in] face that vertex lies on
    s,t - [in] surface parameters
  Returns:
    new vertex that represents the point on face.
  Remarks:
    If a vertex is a point on a face, then brep.m_E[m_ei]
    will be an edge with no 3d curve.  This edge will have
    a single trim with type ON_BrepTrim::ptonsrf.  There
    will be a loop containing this single trim.
  */
  ON_BrepVertex& NewPointOnFace( 
    ON_BrepFace& face,
    double s,
    double t
    );

  /*
  Description:
    Add a new curve on face to the brep.
  Parameters:
    face - [in] face that curve lies on
    edge - [in] 3d edge associated with this curve on surface
    bRev3d - [in] true if the 3d edge and the 2d parameter space 
                  curve have opposite directions.
    c2i - [in] index of 2d curve in face's parameter space
  Returns:
    new trim that represents the curve on surface
  Remarks:
    You should set the trim's ON_BrepTrim::m_tolerance and
    ON_BrepTrim::m_iso values.
  */
  ON_BrepTrim& NewCurveOnFace(
                  ON_BrepFace& face,
                  ON_BrepEdge& edge,
                  bool bRev3d = false,
                  int c2i = -1
                  );

  // appends a copy of brep to this and updates
  // indices of appended brep parts.  Duplicates are not removed.
  void Append( 
    const ON_Brep& // brep
    ); 

  // This function can be used to compute vertex information for a
  // b-rep when everything but the m_V array is properly filled in.
  // It is intended to be used when creating a ON_Brep from a 
  // definition that does not include explicit vertex information.
  void SetVertices(void);

  // This function can be used to set the ON_BrepTrim::m_iso
  // flag. It is intended to be used when creating a ON_Brep from
  // a definition that does not include compatible parameter space
  // type information.
  // See Also: ON_BrepSetFlagsAndTolerances
  bool SetTrimIsoFlags();    // sets all trim iso flags
  bool SetTrimIsoFlags( ON_BrepFace& );
  bool SetTrimIsoFlags( ON_BrepLoop& );
  bool SetTrimIsoFlags( ON_BrepTrim& );


  /*
  Description:
    Calculate the type (singular, mated, boundary, etc.) of
    an ON_BrepTrim object.
  Parameters:
    trim - [in]
    bLazy - [in] if true and trim.m_type is set to something other
       than ON_BrepTrim::unknown, then no calculation is
       performed and the value of trim.m_type is returned.
       If false, the value of trim.m_type is ignored and is calculated.
  Returns:
    Type of trim.
  Remarks:
    The trim must be connected to a valid loop.
  See Also:
    ON_Brep::SetTrimTypeFlags
  */
  ON_BrepTrim::TYPE TrimType( 
    const ON_BrepTrim& trim, 
    bool bLazy = true
    ) const;

  // This function can be used to set the ON_BrepTrim::m_type
  // flag.  If the optional bLazy argument is true, then only
  // trims with m_type = unknown are set.
  // See Also: ON_BrepSetFlagsAndTolerances
  bool SetTrimTypeFlags( bool bLazy = false );    // sets all trim iso flags
  bool SetTrimTypeFlags( ON_BrepFace&, bool bLazy = false );
  bool SetTrimTypeFlags( ON_BrepLoop&, bool bLazy = false );
  bool SetTrimTypeFlags( ON_BrepTrim&, bool bLazy = false );

  // GetTrim2dStart() evaluates the start of the
  // parameter space (2d) trim curve.
  bool GetTrim2dStart( 
          int trim_index,         // index of ON_BrepTrim in m_T[] array
          ON_2dPoint& 
          ) const;

  // GetTrim2dEnd() evaluates end of the
  // parameter space (2d) trim curve.
  bool GetTrim2dEnd(
          int,         // index of ON_BrepTrim in m_T[] array
          ON_2dPoint& 
          ) const;

  // GetTrim3dStart() evaluates the 3d surface at the start of the
  // parameter space (2d) trim curve.
  bool GetTrim3dStart( 
          int,         // index of ON_BrepTrim in m_T[] array
          ON_3dPoint& 
          ) const;

  // GetTrim3dEnd() evaluates the 3d surface at the end of the
  // parameter space (2d) trim curve.
  bool GetTrim3dEnd(
          int,         // index of ON_BrepTrim in m_T[] array
          ON_3dPoint& 
          ) const;

  // This function examines the 2d parameter space curves and returns
  // the loop's type based on their orientation.  Use this function for
  // debugging loop orientation problems.
  ON_BrepLoop::TYPE ComputeLoopType( const ON_BrepLoop& ) const;

  // These set the various tolerances.  The optional bool argument
  // is called bLazy.  If bLazy is false, the tolerance is recomputed
  // from its definition.  If bLazy is true, the tolerance is computed
  // only if its current value is negative.
  bool SetVertexTolerance( ON_BrepVertex& vertex, bool bLazy = false ) const;
  virtual
  bool SetTrimTolerance( ON_BrepTrim& trim, bool bLazy = false ) const;
  virtual
  bool SetEdgeTolerance( ON_BrepEdge& edge, bool bLazy = false ) const;

  /*
  Description:
    Set the brep's vertex tolerances.
  Parameters:
    bLazy - [in] if true, only vertex tolerances with the value
       ON_UNSET_VALUE will be set.  If false, the vertex tolerance
       is recomputed from the geometry in the brep.
  Returns:
    true if successful.
  See Also:
    ON_Brep::SetVertexTolerance
    ON_Brep::SetTrimTolerance
    ON_Brep::SetEdgeTolerance
    ON_Brep::SetVertexTolerances
    ON_Brep::SetTrimTolerances
    ON_Brep::SetEdgeTolerances
    ON_Brep::SetTolerancesAndFlags
  */
  bool SetVertexTolerances( bool bLazy = false );

  /*
  Description:
    Set the brep's trim tolerances.
  Parameters:
    bLazy - [in] if true, only trim tolerances with the value
       ON_UNSET_VALUE will be set.  If false, the trim tolerance
       is recomputed from the geometry in the brep.
  Returns:
    true if successful.
  See Also:
    ON_Brep::SetVertexTolerance
    ON_Brep::SetTrimTolerance
    ON_Brep::SetEdgeTolerance
    ON_Brep::SetVertexTolerances
    ON_Brep::SetTrimTolerances
    ON_Brep::SetEdgeTolerances
    ON_Brep::SetTolerancesAndFlags
  */
  bool SetTrimTolerances( bool bLazy = false );

  /*
  Description:
    Set the brep's edge tolerances.
  Parameters:
    bLazy - [in] if true, only edge tolerances with the value
       ON_UNSET_VALUE will be set.  If false, the edge tolerance
       is recomputed from the geometry in the brep.
  Returns:
    true if successful.
  See Also:
    ON_Brep::SetVertexTolerance
    ON_Brep::SetTrimTolerance
    ON_Brep::SetEdgeTolerance
    ON_Brep::SetVertexTolerances
    ON_Brep::SetTrimTolerances
    ON_Brep::SetEdgeTolerances
    ON_Brep::SetTolerancesAndFlags
  */
  bool SetEdgeTolerances( bool bLazy = false );


  /*
  Description:
    Set the trim parameter space bounding box (trim.m_pbox).
  Parameters:
    trim - [in]
    bLazy - [in] if true and trim.m_pbox is valid, then
       the box is not set.
  Returns:
    true if trim ends up with a valid bounding box.
  */
  virtual
  bool SetTrimBoundingBox( ON_BrepTrim& trim, bool bLazy=false );

  /*
  Description:
    Set the loop parameter space bounding box (loop.m_pbox).
  Parameters:
    loop - [in]
    bLazy - [in] if true and loop trim trim.m_pbox is valid, 
       then that trim.m_pbox is not recalculated.
  Returns:
    true if loop ends up with a valid bounding box.
  */
  virtual
  bool SetTrimBoundingBoxes( ON_BrepLoop& loop, bool bLazy=false );


  /*
  Description:
    Set the loop and trim parameter space bounding boxes
    for every loop and trim in the face 
  Parameters:
    face - [in]
    bLazy - [in] if true and trim trim.m_pbox is valid, 
       then that trim.m_pbox is not recalculated.
  Returns:
    true if all the face's loop and trim parameter space bounding 
    boxes are valid.
  */
  virtual
  bool SetTrimBoundingBoxes( ON_BrepFace& face, bool bLazy=false );

  /*
  Description:
    Set the loop and trim parameter space bounding boxes
    for every loop and trim in the brep.
  Parameters:
    bLazy - [in] if true and trim trim.m_pbox is valid, 
       then that trim.m_pbox is not recalculated.
  Returns:
    true if all the loop and trim parameter space bounding boxes
    are valid.
  */
  virtual
  bool SetTrimBoundingBoxes( bool bLazy=false );

  /*
  Description:
    Set tolerances and flags in a brep
  Parameters:
    bLazy - [in] if true, only flags and tolerances that are not
       set will be calculated.
    bSetVertexTolerances - [in] true to compute vertex.m_tolerance values
    bSetEdgeTolerances - [in] true to compute edge.m_tolerance values
    bSetTrimTolerances - [in] true to compute trim.m_tolerance[0,1] values
    bSetTrimIsoFlags - [in] true to compute trim.m_iso values
    bSetTrimTypeFlags - [in] true to compute trim.m_type values
    bSetLoopTypeFlags - [in] true to compute loop.m_type values
    bSetTrimBoxes - [in] true to compute trim.m_pbox values
  See Also:
    ON_Brep::SetVertexTolerance
    ON_Brep::SetEdgeTolerance
    ON_Brep::SetTrimTolerance
    ON_Brep::SetTrimTypeFlags
    ON_Brep::SetTrimIsoFlags
    ON_Brep::ComputeLoopType
    ON_Brep::SetTrimBoundingBox
    ON_Brep::SetTrimBoundingBoxes
  */
  void SetTolerancesBoxesAndFlags(
       bool bLazy = false,
       bool bSetVertexTolerances = true,
       bool bSetEdgeTolerances = true,
       bool bSetTrimTolerances = true,
       bool bSetTrimIsoFlags = true,
       bool bSetTrimTypeFlags = true,
       bool bSetLoopTypeFlags = true,
       bool bSetTrimBoxes = true
       );


  /////////////////////////////////////////////////////////////////
  // Query Interface

  /*
  Description:
    Determine how many brep faces reference m_S[surface_index].
  Parameters:
    surface_index - [in] index of the surface in m_S[] array
    max_count - [in] counting stops if max_count > 0 and
                     at least max_count faces use the surface.
  Returns:
    Number of brep faces that reference the surface.
  */
  int SurfaceUseCount( 
              int surface_index,
              int max_count=0 ) 
              const;
  /*
  Description:
    Determine how many brep edges reference m_C3[c3_index].
  Parameters:
    c3_index - [in] index of the 3d curve in m_C3[] array
    max_count - [in] counting stops if max_count > 0 and
                     at least max_count edges use the 3d curve.
  Returns:
    Number of brep edges that reference the 3d curve.
  */
  int EdgeCurveUseCount( 
              int c3_index,
              int max_count=0 ) 
              const;

  /*
  Description:
    Determine how many brep trims reference m_C2[c2_index].
  Parameters:
    c2_index - [in] index of the 2d curve in m_C2[] array
    max_count - [in] counting stops if max_count > 0 and
                     at least max_count trims use the 2d curve.
  Returns:
    Number of brep trims that reference the 2d curve.
  */
  int TrimCurveUseCount( 
              int c2_index,
              int max_count=0 ) 
              const;

  /*
  Description:
    Get a single 3d curve that traces the entire loop
  Parameters:
    loop - [in] loop whose 3d curve should be duplicated
    bRevCurveIfFaceRevIsTrue - [in] If false, the returned
       3d curve has an orientation compatible with the
       2d curve returned by Loop2dCurve().
       If true and the m_bRev flag of the loop's face
       is true, then the returned curve is reversed.
  Returns:
    A pointer to a 3d ON_Curve.  The caller must delete
    this curve.
  */
  ON_Curve* Loop3dCurve( 
    const ON_BrepLoop& loop,
    bool bRevCurveIfFaceRevIsTrue = false
    ) const;

  /*
  Description:
    Get a list of 3d curves that trace the non-seam edge
    portions of an entire loop
  Parameters:
    loop - [in] loop whose 3d curve should be duplicated
    curve_list - [out] 3d curves are appended to this list
    bRevCurveIfFaceRevIsTrue - [in] If false, the returned
       3d curves have an orientation compatible with the
       2d curve returned by Loop2dCurve().
       If true and the m_bRev flag of the loop's face
       is true, then the returned curves are reversed.
  Returns:
    Number of curves appended to curve_list.
  */
  int Loop3dCurve( 
    const ON_BrepLoop& loop,
    ON_SimpleArray<ON_Curve*>& curve_list,
    bool bRevCurveIfFaceRevIsTrue = false
    ) const;


  /*
  Description:
    Get a 3d curve that traces the entire loop
  Parameters:
    loop - [in] loop whose 2d curve should be duplicated
  Returns:
    A pointer to a 2d ON_Curve.  The caller must delete
    this curve.
  */
  ON_Curve* Loop2dCurve( const ON_BrepLoop& loop ) const;

  /*
  Description:
    Determine orientation of a brep.
  Returns:
    @untitle table
    +2     brep is a solid but orientation cannot be computed
    +1     brep is a solid with outward facing normals
    -1     brep is a solid with inward facing normals
     0     brep is not a solid
  Remarks:
    The base class implementation returns 2 or 0.  This
    function is overridden in the Rhino SDK and returns
    +1, -1, or 0.
  See Also:
    ON_Brep::IsSolid
  */
  virtual
  int SolidOrientation() const;

  /*
  Description:
    Test brep to see if it is a solid.  (A "solid" is
    a closed oriented manifold.)
  Returns:
    If the brep is a solid, true is returned. Otherwise false is returned.
  See Also:
    ON_Brep::SolidOrientation
    ON_Brep::IsManifold
  */
  bool IsSolid() const;
  
  /*
  Description:
    Test brep to see if it is an oriented manifold.
  Parameters:
    pbIsOriented - [in]  if not null, *pbIsOriented is set
        to true if b-rep is an oriented manifold and false
        if brep is not an oriented manifold.
    pbHasBoundary - [in]  if not null, *pbHasBoundary is set
        to true if b-rep has a boundary edge and false if
        brep does not have a boundary edge.
  Returns:
    If the brep is a manifold, true is returned. Otherwise false is returned.
  See Also:
    ON_Brep::IsSolid
  */
  bool IsManifold( // returns true if b-rep is an oriented manifold
    bool* pbIsOriented = nullptr,
    bool* pbHasBoundary = nullptr
    ) const;


  /*
  Description:
    When an expert is 100% certain of a brep's solid orientation, this function
    can be used to set the SolidOrientation() property.
  Parameters:
    solid_orientation - [in]
      0: not solid,
      1: oriented manifold solid (no boundary) with outward facing normals.
     -1: oriented manifold solid (no boundary) with inward facing normals.
  */
  void SetSolidOrientationForExperts(
    int solid_orientation
  );


  /*
  Description: 
    Determine if P is inside Brep.  This question only makes sense
    when the brep is a closed manifold.  This function does not
    not check for closed or manifold, so result is not valid in
    those cases.  Intersects a line through P with brep, finds
    the intersection point Q closest to P, and looks at face 
    normal at Q.  If the point Q is on an edge or the intersection
    is not transverse at Q, then another line is used.
  Parameters:
    P - [in] 3d point
    tolerance - [in] 3d distance tolerance used for intersection
      and determining strict inclusion.
    bStrictlInside - [in] If bStrictlInside is true, then this
      function will return false if the distance from P is within
      tolerance of a brep face.
  Returns:
    True if P is in, false if not. See parameter bStrictlyIn.
  */
  bool IsPointInside(
          ON_3dPoint P, 
          double tolerance,
          bool bStrictlyInside
          ) const;


  bool IsSurface() const;      // returns true if the b-rep has a single face
                               // and that face is geometrically the same
                               // as the underlying surface.  I.e., the face
                               // has trivial trimming.  In this case, the
                               // surface is m_S[0].
                               // The flag m_F[0].m_bRev records
                               // the correspondence between the surface's
                               // natural parametric orientation and the
                               // orientation of the b-rep.


  bool FaceIsSurface(          // returns true if the face has a single
         int // index of face  // outer boundary and that boundary runs
         ) const;              // along the edges of the underlying surface.
                               // In this case the geometry of the surface
                               // is the same as the geometry of the face.
                               // If FaceIsSurface() is true, then
                               // m_S[m_F[face_index].m_si] is the surface.
                               // The flag m_F[face_index].m_bRev records
                               // the correspondence between the surface's
                               // natural parametric orientation and the
                               // orientation of face in the b-rep.

  bool LoopIsSurfaceBoundary(  // returns true if the loop's trims all run
         int // index of loop  // along the edge's of the underlying surface's
         ) const;              // parameter space.

  /////////////////////////////////////////////////////////////////
  // Modification Interface

  //////////
  // Clears all ON_BrepFace.m_bRev flags by ON_BrepFace::Transpose
  // on each face with a true m_bRev.
  bool FlipReversedSurfaces();

  //////////
  // Change the domain of a trim's 2d curve.  This changes only the
  // parameterization of the 2d trimming curve; the locus of the 
  // 2d trimming curve is not changed.
  bool SetTrimDomain(
         int, // index of trim in m_T[] array
         const ON_Interval&
         );

  //////////
  // Change the domain of an edge.  This changes only the
  // parameterization of the 3d edge curve; the locus of the 
  // 3d edge curve is not changed.
  bool SetEdgeDomain(
         int, // index of edge in m_E[] array
         const ON_Interval&
         );

  // Reverses entire brep orientation of all faces by toggling 
  // value of all face's ON_BrepFace::m_bRev flag.
  void Flip();

  // reverses orientation of a face by toggling ON_BrepFace::m_bRev
  void FlipFace(ON_BrepFace&);

  // Reverses orientation of trimming loop. 
  // This function is intended to be used by brep experts and does
  // does NOT modify ON_BrepLoop::m_type.  You should make sure 
  // ON_BrepLoop::m_type jibes with the loop's direction.  (Outer loops
  // should be counter-clockwise and inner loops should be clockwise.)
  // You can use ON_Brep::LoopDirection() to determine the direction of
  // a loop.
  void FlipLoop(ON_BrepLoop&); // reverses orientation of trimming loop

  // LoopDirection() examines the 2d trimming curve geometry that defines
  // the loop and returns
  //
  //   @untitled table
  //   +1    the loop is a counter-clockwise loop.
  //   -1    the loop is a clockwise loop.
  //    0    the loop is not a continuous closed loop.
  //
  // Since LoopDirection() calculates its result based on the 2d trimming
  // curve geometry, it can be use to set ON_BrepLoop::m_type to outer/inner
  // when translating from data definition where this distinction is murky.
  int LoopDirection( const ON_BrepLoop& ) const;


  /*
  Description:
    Sort the face.m_li[] array by loop type 
    (outer, inner, slit, crvonsrf, ptonsrf)
  Parameters:
    face - [in/out] face whose m_li[] array should be sorted.
  Returns:
    @untitled table
    true      success
    false     failure - no loops or loops with unset loop.m_type
  See Also:
    ON_Brep::ComputeLoopType
    ON_Brep::LoopDirection
  */
  bool SortFaceLoops( ON_BrepFace& face ) const;

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Rebuild the edges used by a face so they lie
    on the surface.
  Parameters:
    face - [in] face whose surface should be changed
    tolerance - [in] tolerance for fitting 3d edge curves
    bRebuildSharedEdges - [in] if false and and edge is
      used by this face and a neighbor, then the edge will
      be skipped.
    bRebuildVertices - [in] if true, vertex locations are
      updated to lie on the surface.
  See Also:
    ON_Brep::RebuildEdges
  */
  bool RebuildEdges( ON_BrepFace& face, 
                     double tolerance,
                     bool bRebuildSharedEdges,
                     bool bRebuildVertices
                     );

  /*
  Description:
    Join coincident edges.
  Parameters:
    edge - [in] this edge will survive the joining process
      and the vertices at its ends will survive the joining process.
    other_edge - [in] this edge and the vertices at its ends will
      be removed.
    join_tolerance - [in] The distances between the ends
      of edge and other_edge must be at most join_tolerance
      in order for the edges to be joined.  The caller is
      responsible for insuring that the 3d location of 
      other_edge is within join_tolerance of edge.
    bCheckFaceOrientaion - [in]
      If true and edge and other_edge are boundary edges,
      then the orientation of the face using other_edge
      is adjusted to match the orientation of the face using
      edge.
  Returns:
    true if join is successful
  Example:

    // extrude an edge of a brep to make a new face
    // NOTE WELL:
    //   THIS IS A SIMPLE EXAMPLE THAT IS NOT VERY EFFICIENT
    //   Use ON_BrepExtrudeEdge if you really want to extrude an edge.
    ON_Brep brep = ...;
    // edge = some valid edge in brep
    const ON_BrepEdge& edge = brep.m_E[...];

    // extrude edge to make a surface
    ON_3dVector v = ...;
    ON_SumSurface* new_surface = new ON_SumSurface();
    new_surface->Create( edge, v );

    // 
    ON_Brep new_brep;
    new_brep.AddFace( Create( new_surface );
    brep.

  See Also:
    ON_Brep:CullUnusedEdges
    ON_Brep:CullUnusedVertices
    ON_Brep:CullUnused3dCurves
  */
  bool JoinEdges( 
    ON_BrepEdge& edge, 
    ON_BrepEdge& other_edge,
    double join_tolerance,
    bool bCheckFaceOrientaion = true
    );
  #endif


  /*
  Description:
    Expert user function. 
    Turn an edge into a series of naked or seam edges.
    One for each trim at the original edge that comes from a unique face.
    These edges will share the 3d curve of the original edge. The original edge will
    still be valid and will have m_ti[0] unchanged.
    */
  bool DisconnectEdgeFaces(int eid);

  /*
  Description:
    Expert user function.
  See Also:
    ON_Brep::JoinEdges
  */
  bool CombineCoincidentVertices(ON_BrepVertex&, ON_BrepVertex&); // moves information to first vertex and deletes second

  /*
  Description:
    Expert user function.
  See Also:
    ON_Brep::JoinEdges
  */
  bool CombineCoincidentEdges(ON_BrepEdge&, ON_BrepEdge&); // moves information to first edge and deletes second

  /*
  Description:
    Expert user function.
    Combines contiguous edges into a single edge.  The edges
    must share a common vertex, then angle between the edge
    tangents are the common vertex must be less than or
    equal to angle_tolerance_radians, and any associated
    trims must be contiguous in there respective boundaries.
  Parameters;
    edge_index0 - [in]
    edge_index1 - [in]
    angle_tolerance_radians - [in]
  Returns:
    Pointer to the new edge or nullptr if the edges cannot
    be combined into a single edge.
  Remarks:
    The input edges are deleted but are still in the
    brep's m_E[] arrays.  Use ON_Brep::Compact to remove 
    the unused edges.
  */
  ON_BrepEdge* CombineContiguousEdges( 
    int edge_index0, 
    int edge_iindex1, 
    double angle_tolerance_radians = ON_PI/180.0
    );

#if defined(OPENNURBS_PLUS)
  /*
  Description:
    Given a trim and parameter on the corresponding 3d edge,
    get the corresponding parameter on the 2d trim curve.
  Parameters:
    trim_index - [in] index of trim in m_T array
    edge_t - [in] parameter on 3d edge
    trim_t - [out] parameter on 2d trim curve
    bOkToBuildTrimPline - [in] 
       if true and m_T[trim_index].m_pline[] does not
       have its edge parameters set, then they are filled
       in.  This is slow the first time, but greatly
       increases the speed of GetTrimParameter
       and GetEdgeParameter on subsequent calls.
  Returns:
    @untitled table
    true        successful
    false       failure - trim_t not set
  See Also:
    ON_Brep::GetEdgeParameter
  */
  bool GetTrimParameter(
          int trim_index,
          double edge_t,
          double* trim_t,
          bool bOkToBuildTrimPline = true
          ) const;

  /*
  Description:
    Given a trim and parameter on the 2d trim curve,
    get the corresponding parameter on the 3d edge curve.
  Parameters:
    trim_index - [in] index of trim in m_T array
    trim_t - [in] parameter on 2d trim curve
    edge_t - [out] parameter on 3d edge
    bOkToBuildTrimPline - [in] 
       if true and m_T[trim_index].m_pline[] does not
       have its edge parameters set, then they are filled
       in.  This is slow the first time, but greatly
       increases the speed of GetTrimParameter
       and GetEdgeParameter on subsequent calls.
  Returns:
    @untitled table
    true        successful
    false       failure - edge_t not set
  See Also:
    ON_Brep::GetEdgeParameter
  */
  bool GetEdgeParameter(
          int trim_index,
          double trim_t,
          double* edge_t,
          bool bOkToBuildTrimPline = true
          ) const;
  /*
  Description:
    Expert user function.
    Splits an edge into two edges.  The input edge
    becomes the left portion and a new edge is created
    for the right portion.
  Parameters:
    edge_index - [in] index of edge in brep.m_E[]
    edge_t - [in] 3d edge splitting parameter
    trim_t - [in] array of trim splitting parameters.
             trim_t[eti] is the parameter for splitting the
             trim brep.m_T[edge.m_ti[eti]].
    vertex_index - [in] if not -1, then this vertex will be
             used for the new vertex.  Otherwise a new
             vertex is created.
    bSetTrimBoxesAndFlags - [in] if true, trim boxes and flags
             are set.  If false, the user is responsible for
             doing this.  Set to true if you are unsure
             what to use.  If you pass false, then need to
             call SetTrimBoundingBoxes(..,bLazy=true)
             so that the trim iso flags and bounding info
             is correctly updated.  If you pass true, then
             the trim flags and bounding boxes get set
             inside of SplitEdge.
  Returns:
    True if successful.
  */
  bool SplitEdge(
        int edge_index,
        double edge_t,
        const ON_SimpleArray<double>& trim_t,
        int vertex_index = -1,
        bool bSetTrimBoxesAndFlags = true
        );

  /*
  Description:
    Splits closed surfaces so they are not closed.
  Parameters:
    min_degree - [in]
      If the degree of the surface < min_degree,
      the surface is not split.  In some cases,
      min_degree = 2 is useful to preserve
      piecewise linear surfaces.
    bShrinkInSplitDirection - [in]
      If true, the face's surface is shrunk so
      the surface's edge coincides with the split.
  Returns:
    True if successful.
  Remarks:
    The version of the function without the
    bShrinkInSplitDirection parameter
    does not split the surface.
  */
  bool SplitClosedFaces( 
    int min_degree, 
    bool bShrinkInSplitDirection 
    );

  bool SplitClosedFaces(
    int min_degree=0 
    );


  /*
  Description:
    Splits surfaces with two singularities, like spheres,
    so the results have at most one singularity.
  Parameters:
    bShrinkInSplitDirection - [in]
      If true, the face's surface is shrunk so
      the surface's edge coincides with the split.
  Returns:
    True if successful.
  Remarks:
    The version of the function without the
    bShrinkInSplitDirection parameter
    does not split the surface.
  */
  bool SplitBipolarFaces(
     bool bShrinkInSplitDirection
     );

  bool SplitBipolarFaces();

#endif

  // These remove a topology piece from a b-rep but do not
  // rearrange the arrays that hold the brep objects.  The
  // deleted objects have their indices set to -1.  Deleting
  // an object that is connected to other objects will 
  // modify those objects.
  void DeleteVertex(ON_BrepVertex& vertex);
  void DeleteEdge(ON_BrepEdge& edge, bool bDeleteEdgeVertices); // pass true to delete vertices used only by edge
  void DeleteTrim(ON_BrepTrim& trim, bool bDeleteTrimEdges); // pass true to delete edges and vertices used only by trim
  void DeleteLoop(ON_BrepLoop& loop, bool bDeleteLoopEdges); // pass true to delete edges and vertices used only by trim
  void DeleteFace(ON_BrepFace& face, bool bDeleteFaceEdges); // pass true to delete edges and vertices used only by face
  void DeleteSurface(int s_index);
  void Delete2dCurve(int c2_index);
  void Delete3dCurve(int c3_index);

  // Description:
  //   Set m_vertex_user.i, m_edge_user.i, m_face_user.i, m_loop_user.i,
  //   and m_trim_user.i values of faces of component including 
  //   m_F[face_index] to label. Numbering starts at 1.
  // Parameters:
  //   face_index - [in] index of face in component
  //   label - [in] value for m_*_user.i
  // Returns:
  // Remarks:
  //   Chases through trim lists of face edges to find adjacent faces.
  //   Does NOT check for vertex-vertex connections
  void LabelConnectedComponent(
    int face_index,
    int label
    ) const;

  /*
  Description:
    Set  m_vertex_user.i, m_edge_user.i, m_face_user.i, m_loop_user.i,
    and m_trim_user.i values values to distinguish connected components.
  Parameters:
  Returns:
    number of connected components
  Remarks:
    For each face in the i-th component, sets m_face_user.i to i>0.
    Chases through trim lists of face edges to find adjacent faces.
	  Numbering starts at 1. Does NOT check for vertex-vertex connections.
  See Also:
    ON_Brep::GetConnectedComponents
  */
  int LabelConnectedComponents() const;

  /*
  Description:
    If this brep has two or more connected components, 
    then duplicates of the connected components are appended
    to the components[] array.
  Parameters:
    components - [out] connected components are appended to this array.
    bDuplicateMeshes - [in] if true, any meshes on this brep are copied
         to the output breps.
  Returns:
    Number of connected components appended to components[] or zero
    if this brep has only one connected component.
  See Also:
    ON_Brep::GetConnectedComponents
  */
  int GetConnectedComponents( 
          ON_SimpleArray< ON_Brep* >& components,
          bool bDuplicateMeshes
          ) const;


#if defined(OPENNURBS_PLUS)
  // Description:
  //   Set m_vertex_user.i, m_edge_user.i, m_face_user.i, m_loop_user.i,
  //   and m_trim_user.i values of faces of component including 
  //   m_F[face_index] to label. Numbering starts at 1.
  // Parameters:
  //   face_index - [in] index of face in component
  //   angle_tol - [in] to determine if an edge is tangent.  In radians.
  //   label - [in] value for m_*_user.i
  // Returns:
  // Remarks:
  //   Chases through trim lists of face edges to find tangent adjacent faces.
  //   Does NOT check for vertex-vertex connections
  void LabelTangentConnectedComponent(
    int face_index,
    double atol,
    int label
    ) const;

  /*
  Description:
    Set  m_vertex_user.i, m_edge_user.i, m_face_user.i, m_loop_user.i,
    and m_trim_user.i values values to distinguish connected components.
  Parameters:
    angle_tol - [in] to determine if an edge is tangent.  In radians.
  Returns:
    number of connected components
  Remarks:
    For each face in the i-th component, sets m_face_user.i to i>0.
    Chases through trim lists of face edges to find adjacent faces.
	  Numbering starts at 1. Does NOT check for vertex-vertex connections.
  See Also:
    ON_Brep::GetConnectedComponents
  */
  int LabelTangentConnectedComponents(double angle_tol) const;

  /*
  Description:
    If this brep has two or more components connected by tangent edges, 
    then duplicates of the connected components are appended
    to the components[] array.
  Parameters:
    components - [out] connected components are appended to this array.
    angle_tol - [in] to determine if an edge is tangent.  In radians.
    bDuplicateMeshes - [in] if true, any meshes on this brep are copied
         to the output breps.
  Returns:
    Number of connected components appended to components[] or zero
    if this brep has only one connected component.
  See Also:
    ON_Brep::GetTangentConnectedComponents
  */
  int GetTangentConnectedComponents( 
          ON_SimpleArray< ON_Brep* >& components,
          double angle_tol,
          bool bDuplicateMeshes
          ) const;
#endif
  /*
  Description:
    Copy a subset of this brep.
  Parameters:
    subfi_count - [in] length of sub_fi[] array.
    sub_fi - [in] array of face indices in this
      brep to copy. (If any values inf sub_fi[]
      are out of range or if sub_fi[] contains
      duplicates, this function will return null.)
    sub_brep - [in] if this pointer is not null,
      then the sub-brep will be created in this
      class.
  Returns:
    If the input is valid, a pointer to the
    sub-brep is returned.  If the input is not
    valid, null is returned.  The faces in
    in the sub-brep's m_F array are in the same
    order as they were specified in sub_fi[].
  */
  ON_Brep* SubBrep( 
    int subfi_count, 
    const int* sub_fi, 
    ON_Brep* sub_brep = 0 
    ) const;

  ///////////////////////////////////////////////////////////////////////
  //
  // region topology
  //
  bool HasRegionTopology() const;

  /*
  Description:
    Get region topology information:
    In order to keep the ON_Brep class efficient, rarely used
    region topology information is not maintained.  If you 
    require this information, call RegionTopology().
  */
  const ON_BrepRegionTopology& RegionTopology() const;

  /*
  Description:
    Destroy region topology information.
  */
  void DestroyRegionTopology();

  // Description:
  //   Duplicate a single brep face.
  // Parameters:
  //   face_index - [in] index of face to duplicate
  //   bDuplicateMeshes - [in] if true, any attached meshes are duplicated
  // Returns:
  //   Single face brep.
  // Remarks:
  //   The m_vertex_user.i, m_edge_user.i, m_face_user.i, m_loop_user.i,
  //   and m_trim_user.i values of the returned brep are are set to the 
  //   indices of the objects they duplicate.
  // See Also:
  //   ON_Brep::DeleteFace, ON_Brep::ExtractFace
  ON_Brep* DuplicateFace(
    int face_index,
    bool bDuplicateMeshes
    ) const;

  // Description:
  //   Duplicate a a subset of a brep
  // Parameters:
  //   face_count - [in] length of face_index[] array
  //   face_index - [in] array of face indices
  //   bDuplicateMeshes - [in] if true, any attached meshes are duplicated
  // Returns:
  //   A brep made by duplicating the faces listed in the face_index[] array.
  // Remarks:
  //   The m_vertex_user.i, m_edge_user.i, m_face_user.i, m_loop_user.i,
  //   and m_trim_user.i values of the returned brep are are set to the 
  //   indices of the objects they duplicate.
  // See Also:
  //   ON_Brep::DuplicateFace
  ON_Brep* DuplicateFaces(
    int face_count,
    const int* face_index,
    bool bDuplicateMeshes
    ) const;

  // Description:
  //   Extract a face from a brep.
  // Parameters:
  //   face_index - [in] index of face to extract
  // Returns:
  //   Single face brep.
  // See Also:
  //   ON_Brep::DeleteFace, ON_Brep::DuplicateFace
  ON_Brep* ExtractFace(
    int face_index
    );


  /*
  Description:
    Standardizes the relationship between an ON_BrepEdge
    and the 3d curve it uses.  When done, the edge will
    be the only edge that references its 3d curve, the 
    domains of the edge and 3d curve will be the same, 
    and the edge will use the entire locus of the 3d curve.
  Parameters:
    edge_index - [in] index of edge to standardize.
    bAdjustEnds - [in] if true, move edge curve endpoints to vertices
  See Also:
    ON_Brep::StandardizeEdgeCurves
    ON_Brep::Standardize
  */
  bool StandardizeEdgeCurve( int edge_index, bool bAdjustEnds );


  /*
  Description:
    Expert user only.  Same as above, but to be used when the edge
    curve use count is known for the edge.
    Standardizes the relationship between an ON_BrepEdge
    and the 3d curve it uses.  When done, the edge will
    be the only edge that references its 3d curve, the 
    domains of the edge and 3d curve will be the same, 
    and the edge will use the entire locus of the 3d curve.
  Parameters:
    edge_index - [in] index of edge to standardize.
    bAdjustEnds - [in] if true, move edge curve endpoints to vertices
    EdgeCurveUse - [in] if > 1, then the edge curve for this edge is used by more than one
        edge.  if 1, then the edge curve is used only for this edge. 
        If <= 0, then use count is unknown.
  See Also:
    ON_Brep::StandardizeEdgeCurves
    ON_Brep::Standardize
  */
  bool StandardizeEdgeCurve( int edge_index, bool bAdjustEnds, int EdgeCurveUse );


  /*
  Description:
    Standardize all edges in the brep.
  Parameters:
    bAdjustEnds - [in] if true, move edge curve endpoints to vertices
  See Also:
    ON_Brep::StandardizeEdgeCurve
    ON_Brep::Standardize
  */
  void StandardizeEdgeCurves( bool bAdjustEnds );

  /*
  Description:
    Standardizes the relationship between an ON_BrepTrim
    and the 2d curve it uses.  When done, the trim will
    be the only trim that references its 2d curve, the 
    domains of the trim and 2d curve will be the same, 
    and the trim will use the entire locus of the 2d curve.
  Parameters:
    trim_index - [in] index of trim to standardize.
  See Also:
    ON_Brep::StandardizeTrimCurves
    ON_Brep::Standardize
  */
  bool StandardizeTrimCurve( int trim_index );

  /*
  Description:
    Standardize all trims in the brep.
  See Also:
    ON_Brep::StandardizeTrimCurve
    ON_Brep::Standardize
  */
  void StandardizeTrimCurves();

  /*
  Description:
    Standardizes the relationship between an ON_BrepFace
    and the 3d surface it uses.  When done, the face will
    be the only face that references its 3d surface, and
    the orientations of the face and 3d surface will be 
    the same. 
  Parameters:
    face_index - [in] index of face to standardize.
  See Also:
    ON_Brep::StardardizeFaceSurfaces
    ON_Brep::Standardize
  */
  bool StandardizeFaceSurface( int face_index );

  /*
  Description:
    Standardize all faces in the brep.
  See Also:
    ON_Brep::StandardizeFaceSurface
    ON_Brep::Standardize
  */
  void StandardizeFaceSurfaces();

  /*
  Description:
    Standardize all trims, edges, and faces in the brep.
  Remarks:
    After standardizing, there may be unused curves and surfaces
    in the brep.  Call ON_Brep::Compact to remove these unused
    curves and surfaces.
  See Also:
    ON_Brep::StandardizeTrimCurves
    ON_Brep::StandardizeEdgeCurves
    ON_Brep::StandardizeFaceSurface
    ON_Brep::Compact
  */
  void Standardize();
  

  /*
  Description:
    Sometimes the ON_Surface used by a face extends far
    beyond the face's outer boundary.  ShrinkSurface uses
    ON_Surface::Trim to remove portions of the surface that
    extend beyond the face's outer boundary loop.
  Parameters:
    face - [in] face to test and whose surface should be shrunk.
    DisableSide - [in] This is a bit field.  A set bit indicates not to shrink
                the surface on a given side.  The default of 0 enables shrinking 
                on all four sides.
      @table  
      value       meaning
      0x0001     Don't shrink on the west side of domain.
      0x0002     Don't shrink on the south side of domain.
      0x0004     Don't shrink on the east side of domain.
      0x0008     Don't shrink on the north side of domain.
  Returns:
    @untitled table
    true        successful
    false       failure
  Remarks:
    If a surface needs to be shrunk it is copied.  After shrinking,
    you may want to call ON_Brep::CullUnusedSurfaces to remove
    any unused surfaces.
  See Also:
    ON_Brep::ShrinkSurfaces
    ON_Brep::CullUnusedSurfaces
  */
  bool ShrinkSurface( ON_BrepFace& face, int DisableSide=0 );

  /*
  Description:
    Sometimes the ON_Surface used by a face extends far
    beyond the face's outer boundary.  ShrinkSurfaces calls
    ON_Shrink::ShrinkSurface on each face to remove portions
    of surfaces that extend beyond their face's outer boundary
    loop.
  Returns:
    @untitled table
    true        successful
    false       failure
  Remarks:
    If a surface needs to be shrunk it is copied.  After shrinking,
    you may want to call ON_Brep::CullUnusedSurfaces to remove
    any unused surfaces.
  See Also:
    ON_Brep::ShrinkSurface
    ON_Brep::CullUnusedSurfaces
  */
  bool ShrinkSurfaces();

  /*
  Description:
    Uses the CullUnused*() members to delete any unreferenced
    objects from arrays, reindexes as needed, and shrinks
    arrays to minimum required size.
  See Also:
    ON_Brep::CullUnusedFaces
    ON_Brep::CullUnusedLoops
    ON_Brep::CullUnusedTrims
    ON_Brep::CullUnusedEdges
    ON_Brep::CullUnusedVertices
    ON_Brep::CullUnused3dCurves
    ON_Brep::CullUnused2dCurves
    ON_Brep::CullUnusedSurfaces
  */
  bool Compact();

  bool CullUnusedFaces(); // culls faces with m_face_index == -1
  bool CullUnusedLoops(); // culls loops with m_loop_index == -1
  bool CullUnusedTrims(); // culls trims with m_trim_index == -1
  bool CullUnusedEdges(); // culls edges with m_edge_index == -1
  bool CullUnusedVertices(); // culls vertices with m_vertex_index == -1
  bool CullUnused3dCurves(); // culls 2d curves not referenced by a trim
  bool CullUnused2dCurves(); // culls 3d curves not referenced by an edge
  bool CullUnusedSurfaces(); // culls surfaces not referenced by a face

  /////////////////////////////////////////////////////////////////
  // Navigation Interface

  // for moving around loops - returns trim index of previous/next trim in loop
  int PrevTrim(
        int // index of current trim (m_trim_index)
        ) const;
  int NextTrim(
        int // index of current trim (m_trim_index)
        ) const;

  //Same as NextTrim and PrevTrim, but skips over trims with type singular
  int PrevNonsingularTrim(
        int // index of current trim (m_trim_index)
        ) const;
  int NextNonsingularTrim(
        int // index of current trim (m_trim_index)
        ) const;

  /*
  Description:
    This is a simple tool for getting running through the edges
    that begin and end at a vertex.
  Parameters:
    current_edge_index - [in]
    endi - [in] 0 = use the edge start vertex, 1 = use the edge end vertex
    prev_endi - [out] 0 if previous edge begins at the vertex, 
                      1 if previous edge ends at the vertex
  Returns:
    edge index of the previous edge or -1 if there is only one edge
    that begins or ends at the vertex.
  Remarks:
    This is a tool that simplifies searching through the
    ON_BrepVertex.m_ei[] array.
    The edges are in no particular order.
  See Also:
    ON_Brep::NextEdge
  */
  int PrevEdge(
        int current_edge_index,
        int endi,
        int* prev_endi = nullptr
        ) const;

  /*
  Description:
    This is a simple tool for getting running through the edges
    that begin and end at a vertex.
  Parameters:
    current_edge_index - [in]
    endi - [in] 0 = use the edge start vertex, 1 = use the edge end vertex
    next_endi - [out] 0 if next edge begins at the vertex, 
                      1 if next edge ends at the vertex
  Returns:
    edge index of the next edge or -1 if there is only one edge
    that begins or ends at the vertex.
  Remarks:
    This is a tool that simplifies searching through the
    ON_BrepVertex.m_ei[] array.  
    The edges are in no particular order.
  See Also:
    ON_Brep::NextEdge
  */
  int NextEdge(
        int current_edge_index,
        int endi,
        int* next_endi = nullptr
        ) const;

  /*
  Description:
    Get a brep component from its index.
  Parameters:
    component_index - [in] 
  Returns:
    A const pointer to the component.  Do not delete
    the returned object.  It points to an object managed
    by this brep.
  See Also:
    ON_Brep::Face
    ON_Brep::Edge
    ON_Brep::Loop
    ON_Brep::Trim
    ON_Brep::Vertex
  */
  const ON_Geometry* BrepComponent( 
    ON_COMPONENT_INDEX ci
    ) const;

  /*
  Description:
    Get vertex from trim index or component index.
  Parameters:
    vertex_index - [in] either an index into m_V[] or a component index
                      of type brep_vertex.
  Returns:
    If the index is a valid vertex index or a valid vertex component
    index, then a pointer to the ON_BrepVertex is returned.  Otherwise
    nullptr is returned.
  See Also
    ON_Brep::Component( const ON_BrepVertex& )
  */
  ON_BrepVertex* Vertex( int vertex_index ) const;
  ON_BrepVertex* Vertex( ON_COMPONENT_INDEX vertex_index ) const;

  /*
  Description:
    Get edge from edge index or component index.
  Parameters:
    edge_index - [in] either an index into m_E[] or a component index
                      of type brep_edge.
  Returns:
    If the index is a valid edge index or a valid edge component
    index, then a pointer to the ON_BrepEdge is returned.  Otherwise
    nullptr is returned.
  See Also
    ON_Brep::Component( const ON_BrepEdge& )
  */
  ON_BrepEdge* Edge( int edge_index ) const;
  ON_BrepEdge* Edge( ON_COMPONENT_INDEX edge_index ) const;

  /*
  Description:
    Get trim from trim index or component index.
  Parameters:
    trim_index - [in] either an index into m_T[] or a component index
                      of type brep_trim.
  Returns:
    If the index is a valid trim index or a valid trim component
    index, then a pointer to the ON_BrepTrim is returned.  Otherwise
    nullptr is returned.
  See Also
    ON_Brep::Component( const ON_BrepTrim& )
  */
  ON_BrepTrim* Trim( int trim_index ) const;
  ON_BrepTrim* Trim( ON_COMPONENT_INDEX trim_index ) const;

  /*
  Description:
    Get loop from loop index or component index.
  Parameters:
    loop_index - [in] either an index into m_L[] or a component index
                      of type brep_loop.
  Returns:
    If the index is a valid loop index or a valid loop component
    index, then a pointer to the ON_BrepLoop is returned.  Otherwise
    nullptr is returned.
  See Also
    ON_Brep::Component( const ON_BrepLoop& )
  */
  ON_BrepLoop* Loop( int loop_index ) const;
  ON_BrepLoop* Loop( ON_COMPONENT_INDEX loop_index ) const;

  /*
  Description:
    Get face from face index or component index.
  Parameters:
    face_index - [in] either an index into m_F[] or a component index
                      of type brep_face.
  Returns:
    If the index is a valid face index or a valid face component
    index, then a pointer to the ON_BrepFace is returned.  Otherwise
    nullptr is returned.
  See Also
    ON_Brep::Component( const ON_BrepFace& )
  */
  ON_BrepFace* Face( int face_index ) const;
  ON_BrepFace* Face( ON_COMPONENT_INDEX face_index ) const;

  //Match endpoints of adjacent trims. If a trim needs to be adjusted, copy the 2d curve if necessary,
  //convert to NURBS form, yank cvs.  Compact() should be called afterwards. Returns false if error in
  //computation,  Trims must be from same face and meet at a common vertex.
  //These are expert user functions. When in doubt use MatchTrimEnds() on the entire Brep.
  
  /*
  Description:
    Match the end of a trim to the start of the next trim.
  Parameters:
    T0 - [in] brep trim
    T1 - [in] brep trim that comes immediately after T0 in the same loop
  Returns:
    true if either trim's 2d curve is changed
    */
  bool MatchTrimEnds(ON_BrepTrim& T0,
                           ON_BrepTrim& T1 
                           );

  /*
  Description:
    Match the endpoints of a trim to the next and previous trim
  Parameters:
    trim_index - [in] index into m_T
  Returns:
    true if any trim's 2d curve is changed
    */
  bool MatchTrimEnds(int trim_index);

  /*
  Description:
    Match the endpoints of all trims in a loop
  Parameters:
    Loop - [in] brep loop
  Returns:
    true if any trim's 2d curve is changed
    */
  bool MatchTrimEnds(ON_BrepLoop& Loop);

  /*
  Description:
    Match the endpoints of all trims in a brep
  Parameters:
  Returns:
    true if any trim's 2d curve is changed
    */
  bool MatchTrimEnds();

  /*
  Description:
    Convert the 2d curve of a trim to an ON_NurbsCurve
  Parameters:
    T - [in] brep trim
  Returns:
    Pointer to m_C2[T.m_c2i]
  NOTE: After calling this, m_C2[T.m_c2i] will be a NURBS curve only referenced by 
    T, with domain = T.m_t.  Caller should not delete the returned curve since its memory is owned
    by the brep (this).
  */
  ON_NurbsCurve* MakeTrimCurveNurb(ON_BrepTrim& T);

  /*
  Description:
    Check for slit trims and slit boundaries in each face.
  Returns:
    true if any slits were found
  */
  bool HasSlits() const;

  /*
  Description:
    Check for slit trims and slit boundaries in a face.
  Returns:
    true if any slits were found
  */
  bool HasSlits(const ON_BrepFace& F) const;

  /*
  Description:
    Check for slit trims in a loop.
  Returns:
    true if any slits were found
  */
  bool HasSlits(const ON_BrepLoop& L) const;

  /*
  Description:
    remove slit trims and slit boundaries from each face.
  Returns:
    true if any slits were removed
  Remarks:
    Caller should call Compact() afterwards.
  */
  bool RemoveSlits();

  /*
  Description:
    remove slit trims and slit boundaries from a face.
  Parameters:
    F - [in] brep face
  Returns:
    true if any slits were removed
  Remarks:
    Caller should call Compact() when done.
  */
  bool RemoveSlits(ON_BrepFace& F);

  /*
  Description:
    remove slit trims from a loop.
  Parameters:
    L - [in] brep loop
  Returns:
    true if any slits were removed
  Remarks:
    Caller should call Compact() when done.
    If all trims are removed, the loop will be marked as deleted.
  */
  bool RemoveSlits(ON_BrepLoop& L);

  /*
  Description:
    If fid0 != fid1 and m_F[fid0] and m_F[fid1] have the same surface (m_si is identical),
    and they are joined along a set of edges that do not have any other faces, then this will
    combine the two faces into one.
  Parameters:
    fid0, fid1 - [in] indices into m_F of faces to be merged.
  Returns:
    id of merged face if faces were successfully merged. -1 if not merged.
  Remarks:
    Caller should call Compact() when done.
  */
  int MergeFaces(int fid0, int fid1);

  /*
  Description:
    Merge all possible faces that have the same m_si
  Returns:
    true if any faces were successfully merged.
  Remarks:
    Caller should call Compact() when done.
  */
  bool MergeFaces();

  /*
  Description:
    Will turn a linear polycurve into a single line if possible
  Parameters:
    edge_index - [in] index of the edge to simplify.
    tolerance - [in] used in call to ON_Curve::IsLinear;
  Returns:
    true on success.
  */
  bool SimplifyEdge(int edge_index, double tolerance);

  /*
  Description:
    Removes nested polycurves from the m_C2[] and m_C3[] arrays.
  Parameters:
    bExtractSingleSegments - [in] if true, polycurves with a
      single segment are replaced with the segment curve.
    bEdges - [in] if true, the m_C3[] array is processed
    bTrimCurves - [in] if true, the m_C2[] array is processed.  
  Returns:
    True if any nesting was removed and false if no nesting
    was removed.
  */
  bool RemoveNesting(
          bool bExtractSingleSegments,
          bool bEdges = true, 
          bool bTrimCurves = true
          );

  /*
  Description:
    Expert user tool to collapse a "short" edge to a vertex.
    The edge is removed and the topology is repaired
    so that everything that used to connect to the edge
    connects the specified vertex.
  Parameters:
    edge_index - [in] index of edge to remove
    bCloseTrimGap - [in] if true and the removal of the
       edge creates a gap in the parameter space trimming
       loop, then the 2d trim curves will be adjusted to
       close the gap.
    vertex_index - [in] if >= 0, this the edge is collapsed
       to this vertex.  Otherwise a vertex is automatically
       selected or created.
  Returns:
    True if edge was successfully collapsed.
  Remarks:
    After you finish cleaning up the brep, you need
    to call ON_Brep::Compact() to remove unused edge,
    trim, and vertex information from the brep's m_E[], 
    m_V[], m_T[], m_C2[], and m_C3[] arrays.
  */
  bool CollapseEdge(
    int edge_index,
    bool bCloseTrimGap = true,
    int vertex_index = -1
    );

  /*
  Description:
    Expert user tool to move trims and edges from
    one vertex to another.
  Parameters:
    old_vi - [in] index of old vertex
    new_vi - [in] index of new vertex
    bClearTolerances - [in] if true, then tolerances of
       edges and trims that are connected to the old
       vertex are set to ON_UNSET_VALUE.
    vertex_index - [in] if >= 0, this the edge is collapsed
       to this vertex.  Otherwise a vertex is automatically
       selected or created.
  Returns:
    True if successful.
  Remarks:
    After you finish cleaning up the brep, you need
    to call ON_Brep::Compact() to remove unused edge,
    trim, and vertex information from the brep's m_E[], 
    m_V[], m_T[], m_C2[], and m_C3[] arrays.
  */
  bool ChangeVertex( 
    int old_vi, 
    int new_vi, 
    bool bClearTolerances 
    );

  /*
  Description:
    Expert user tool to remove any gap between adjacent trims.
  Parameters:
    trim0 - [in]
    trim1 - [in]
  Returns:
    True if successful.
  Remarks:
    The trims must be in the same trimming loop.  The vertex
    at the end of trim0 must be the same as the vertex at
    the start of trim1.  The trim's m_iso and m_type flags
    need to be correctly set.
  */
  bool CloseTrimGap( 
    ON_BrepTrim& trim0, 
    ON_BrepTrim& trim1 
    );

  /*
  Description:
    Remove edges that are not connected to a face.
  Parameters:
    bDeleteVertices - [in] if true, then the vertices
      at the ends of the wire edges are deleted if 
      they are not connected to face trimming edges.
  Returns:
    Number of edges that were removed.
  Remarks:
    After you finish cleaning up the brep, you need
    to call ON_Brep::Compact() to remove unused edge,
    trim, and vertex information from the brep's m_E[], 
    m_V[], m_T[], m_C2[], and m_C3[] arrays.

    After you finish cleaning up the brep, you need
    to call ON_Brep::Compact() to remove deleted vertices
    from the m_V[] array.
  See Also:
    ON_Brep::RemoveWireVertices
  */
  int RemoveWireEdges( bool bDeleteVertices = true );

  /*
  Description:
    Remove vertices that are not connected to an edge.
  Returns:
    Number of vertices that were deleted.
  Remarks:
    After you finish cleaning up the brep, you need
    to call ON_Brep::Compact() to remove deleted 
    vertices from the m_V[] array.
  See Also:
    ON_Brep::RemoveWireEdges
  */
  int RemoveWireVertices();


public:
  /*
  Description:
    Removes all per face material channel index overrides.
  Returns:
    Number of changed faces.
  Remarks:
    Per face material channel indices are a mutable property on ON_BrepFace and are set with ON_BrepFace.SetMaterialChannelIndex().
  */
  unsigned int ClearPerFaceMaterialChannelIndices();

  /*
  Returns:
    True if one or more faces have per face material channel index overrides.
  Remarks:
    Per face material channel indices are a mutable property on ON_BrepFace and are set with ON_BrepFace.SetMaterialChannelIndex().
  */
  bool HasPerFaceMaterialChannelIndices() const;

  /*
  Description:
    Removes all per face color overrides.
  Returns:
    Number of changed faces.
  Remarks:
    Per face colors are a mutable property on ON_BrepFace and are set with ON_BrepFace.SetPerFaceColor().
  */
  unsigned int ClearPerFaceColors() const;

  /*
  Returns:
    True if one or more faces have per face color overrides.
  Remarks:
    Per face colors are a mutable property on ON_BrepFace and are set with ON_BrepFace.SetPerFaceColor().
  */
  bool HasPerFaceColors() const;

#if defined(OPENNURBS_PLUS)
  /*
  Returns:
    If this brep was created by ON_SubD::ProxyBrep(), then the value of
    ON_SubD.RuntimeSerialNumber() is returned.
  */
  ON__UINT64 ProxyBrepSubDRuntimeSerialNumber() const;
#endif
  /////////////////////////////////////////////////////////////////
  // "Expert" Interface

  void Set_user(ON_U u) const; // set every brep m_*_user value to u
  void Clear_vertex_user_i() const; // zero all brep's m_vertex_user values
  void Clear_edge_user_i(int) const;   // zero all brep's m_edge_user values
  void Clear_edge_user_i() const;   // zero all brep's m_edge_user values
  void Clear_trim_user_i() const;   // zero all brep's m_trim_user values
  void Clear_loop_user_i() const;   // zero all brep's m_loop_user values
  void Clear_face_user_i() const;   // zero all brep's m_face_user values
  void Clear_user_i() const;        // zero all brep's m_*_user values

  // Union available for application use.
  // The constructor zeros m_brep_user.
  // The value is of m_brep_user is not saved in 3DM
  // archives and may be changed by some computations.
  mutable ON_U m_brep_user; 

  // geometry 
  // (all geometry is deleted by ~ON_Brep().  Pointers can be nullptr
  // or not referenced.  Use Compact() to remove unreferenced geometry.
  ON_CurveArray   m_C2;  // Pointers to parameter space trimming curves
                         // (used by trims).
  ON_CurveArray   m_C3;  // Pointers to 3d curves (used by edges).
  ON_SurfaceArray m_S;   // Pointers to parametric surfaces (used by faces)

  // topology
  // (all topology is deleted by ~ON_Brep().  Objects can be unreferenced.
  // Use Compact() to to remove unreferenced geometry.
  ON_BrepVertexArray  m_V;   // vertices
  ON_BrepEdgeArray    m_E;   // edges
  ON_BrepTrimArray    m_T;   // trims
  ON_BrepLoopArray    m_L;   // loops
  ON_BrepFaceArray    m_F;   // faces

protected:	
  friend class ON_BrepFace;
  friend class ON_BrepRegion;
  friend class ON_BrepFaceSide;
  friend class ON_V5_BrepRegionTopologyUserData;
  ON_BoundingBox m_bbox;
  mutable class ON_BrepRegionTopology* m_region_topology = nullptr;
  static class ON_BrepRegionTopology* Internal_RegionTopologyPointer(
    const ON_Brep* brep,
    bool bValidateFaceCount 
    );
  void Internal_AttachV5RegionTopologyAsUserData(
    ON_BinaryArchive& archive
  ) const;


  // m_aggregate_status "should" be an ON_AggregateComponentStatusEx,
  // but that change requires breaking the C++ SDK.
  mutable ON_AggregateComponentStatus m_aggregate_status;

  // Never directly set m_is_solid, use calls to IsSolid() and/or 
  // SolidOrientation() when you need to know the answer to this
  // question.
  // 0 = unset
  // 1 = solid with normals pointing out
  // 2 = solid with normals pointing in
  // 3 = not solid
  int m_is_solid = 0;

public:
  // Not ideal - used in debugging and testing
  //bool GetLock();
  //bool GetLockOrReturnFalse();
  //bool ReturnLock();

private:
  // In calculations where multiple threads are using a brep and calling functions
  // that may modify content, the calling code can use use ON_SleepLockGuard guard(Mutex)
  // or similar techniques to make the calculations thread safe.
  // Because Mutex is a public resource, it must be used with great care to
  // prevent lock contention.
  friend class ON_SleepLockGuard;
  mutable ON_SleepLock m_sleep_lock;
  
protected:
  // These are friends so legacy tol values stored in v1 3dm files
  // can be used to set brep edge and trimming tolerances with a call
  // to ON_Brep::SetTolsFromLegacyValues().
  friend bool ON_BinaryArchive::ReadV1_TCODE_LEGACY_FAC(ON_Object**,ON_3dmObjectAttributes*);
  friend bool ON_BinaryArchive::ReadV1_TCODE_LEGACY_SHL(ON_Object**,ON_3dmObjectAttributes*);
  void Initialize();

  // helpers to set ON_BrepTrim::m_iso flag
  void SetTrimIsoFlag(int,double[6]);
  void SetTrimIsoFlag(int);

  // helpers to create and set vertices
  bool SetEdgeVertex(const int, const int, const int );
  bool HopAcrossEdge( int&, int& ) const;
  bool SetTrimStartVertex( const int, const int);
  void SetLoopVertices(const int);
  void ClearTrimVertices();
  void ClearEdgeVertices();

  // helpers for SwapFaceParameters()
  bool SwapLoopParameters(
        int // index of loop
        );
  bool SwapTrimParameters(
        int // index of trim
        );

  // helpers for validation checking
  bool IsValidTrim(int trim_index,ON_TextLog* text_log) const;
  bool IsValidTrimTopology(int trim_index,ON_TextLog* text_log) const;
  bool IsValidTrimGeometry(int trim_index,ON_TextLog* text_log) const;
  bool IsValidTrimTolerancesAndFlags(int trim_index,ON_TextLog* text_log) const;

  bool IsValidLoop(int loop_index,ON_TextLog* text_log) const;
  bool IsValidLoopTopology(int loop_index,ON_TextLog* text_log) const;
  bool IsValidLoopGeometry(int loop_index,ON_TextLog* text_log) const;
  bool IsValidLoopTolerancesAndFlags(int loop_index,ON_TextLog* text_log) const;

  bool IsValidFace(int face_index,ON_TextLog* text_log) const;
  bool IsValidFaceTopology(int face_index,ON_TextLog* text_log) const;
  bool IsValidFaceGeometry(int face_index,ON_TextLog* text_log) const;
  bool IsValidFaceTolerancesAndFlags(int face_index,ON_TextLog* text_log) const;
  
  bool IsValidEdge(int edge_index,ON_TextLog* text_log) const;
  bool IsValidEdgeTopology(int edge_index,ON_TextLog* text_log) const;
  bool IsValidEdgeGeometry(int edge_index,ON_TextLog* text_log) const;
  bool IsValidEdgeTolerancesAndFlags(int edge_index,ON_TextLog* text_log) const;

  bool IsValidVertex(int vertex_index,ON_TextLog* text_log) const;
  bool IsValidVertexTopology(int vertex_index,ON_TextLog* text_log) const;
  bool IsValidVertexGeometry(int vertex_index,ON_TextLog* text_log) const;
  bool IsValidVertexTolerancesAndFlags(int vertex_index,ON_TextLog* text_log) const;

  void SetTolsFromLegacyValues();

  // read helpers to support various versions
  bool ReadOld100( ON_BinaryArchive& ); // reads legacy old RhinoIO toolkit b-rep
  bool ReadOld101( ON_BinaryArchive& ); // reads legacy Rhino 1.1 b-rep
  bool ReadOld200( ON_BinaryArchive&, int ); // reads legacy trimmed surface
  ON_Curve* Read100_BrepCurve( ON_BinaryArchive& ) const;
  ON_Surface* Read100_BrepSurface( ON_BinaryArchive& ) const;

  // helpers for reading legacy v1 trimmed surfaces and breps
  bool ReadV1_LegacyTrimStuff( ON_BinaryArchive&, ON_BrepFace&, ON_BrepLoop& );
  bool ReadV1_LegacyTrim( ON_BinaryArchive&, ON_BrepFace&, ON_BrepLoop& );
  bool ReadV1_LegacyLoopStuff( ON_BinaryArchive&, ON_BrepFace& );
  bool ReadV1_LegacyLoop( ON_BinaryArchive&, ON_BrepFace& );
  bool ReadV1_LegacyFaceStuff( ON_BinaryArchive& );
  bool ReadV1_LegacyShellStuff( ON_BinaryArchive& );

  

public:
  // The ON_Brep code increments ON_Brep::ErrorCount every time something
  // unexpected happens. This is useful for debugging.
  static unsigned int ErrorCount;

private:
  /*
  Parameters:
    bLazy - [in]
      If true and if ON_BrepFace.m_bbox is not empty, then ON_BrepFace.m_bbox is returned.
      In all other cases the bbox is calculated from scratch.
    bUpdateCachedBBox - [in]
      If true and the bounding box is calculated, then the value is saved in ON_BrepFace.m_bbox
      so future lazy evaluations can use the value.
  */
  const ON_BoundingBox InternalBrepBoundingBox(bool bLazy, bool bUpdateCachedBBox) const;
};

///////////////////////////////////////////////////////////////////////////////
//
// brep construction tools
// 

/*
Description:
  Create a brep representation of a mesh.
Parameters:
  mesh_topology - [in]
  bTrimmedTriangles - [in] if true, triangles in the mesh
     will be represented by trimmed planes in the brep.
     If false, triangles in the mesh will be represented by
     untrimmed singular bilinear NURBS surfaces in the brep.
  pBrep - [in] If not nullptr, this the mesh representation will
     be put into this brep.
Example:

          ON_Mesh mesh = ...;
          ON_Brep* pBrep = ON_BrepFromMesh( mesh.Topology() );
          ...
          delete pBrep;

See Also
  ON_BrepFromMesh( const ON_Mesh& mesh, ... );
*/
ON_DECL
ON_Brep* ON_BrepFromMesh( 
          const ON_MeshTopology& mesh_topology, 
          bool bTrimmedTriangles = true,
          ON_Brep* pBrep = nullptr 
          );

/*
Description:
  Create a brep representation of a mesh.
Parameters:
  mesh_topology - [in]
  bTrimmedTriangles - [in] if true, triangles in the mesh
     will be represented by trimmed planes in the brep.
     If false, triangles in the mesh will be represented by
     untrimmed singular bilinear NURBS surfaces in the brep.
  bUseNgonsIfTheyExist - [in] if the mesh has ngons and bUseNgonsIfTheyExist is true
     then trimmed planes will be created for the ngons regardless of bTrimmedTriangles.
  tolerance - [in] used to determine if an ngon is planar.
  pBrep - [in] If not nullptr, this the mesh representation will
     be put into this brep.
Example:

          ON_Mesh mesh = ...;
          ON_Brep* pBrep = ON_BrepFromMesh( mesh.Topology() );
          ...
          delete pBrep;

See Also
  ON_BrepFromMesh( const ON_Mesh& mesh, ... );
*/
ON_DECL
ON_Brep* ON_BrepFromMeshWithNgons(
  const ON_MeshTopology& mesh_topology,
  bool bTrimmedTriangles,
  bool bUseNgonsIfTheyExist,
  double tolerance,
  ON_Brep * pBrep = nullptr
  );

/*
Description:
  Get an ON_Brep definition of a box.
Parameters:
  box_corners - [in] 8 points defining the box corners
     arranged as the vN labels indicate.

          v7_______e6_____v6
           |\             |\
           | e7           | e5
           |  \ ______e4_____\ 
          e11  v4         |   v5
           |   |        e10   |
           |   |          |   |
          v3---|---e2----v2   e9
           \   e8         \   |
            e3 |           e1 |
             \ |            \ |
              \v0_____e0_____\v1

  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the box with topology

   edge              vertices
    m_E[ 0]           m_V[0], m_V[1]
    m_E[ 1]           m_V[1], m_V[2]
    m_E[ 2]           m_V[2], m_V[3]
    m_E[ 3]           m_V[3], m_V[0]
    m_E[ 4]           m_V[4], m_V[5]
    m_E[ 5]           m_V[5], m_V[6]
    m_E[ 6]           m_V[6], m_V[7]
    m_E[ 7]           m_V[7], m_V[4]
    m_E[ 8]           m_V[0], m_V[4]
    m_E[ 9]           m_V[1], m_V[5]
    m_E[10]           m_V[2], m_V[6]
    m_E[11]           m_V[3], m_V[7]

   face              boundary edges
    m_F[0]            +m_E[0] +m_E[9]  -m_E[4] -m_E[8]
    m_F[1]            +m_E[1] +m_E[10] -m_E[5] -m_E[9]
    m_F[2]            +m_E[2] +m_E[11] -m_E[6] -m_E[10]
    m_F[3]            +m_E[3] +m_E[8]  -m_E[7] -m_E[11]
    m_F[4]            -m_E[3] -m_E[2]  -m_E[1] -m_E[0]
//     m_F[5]            +m_E[4] +m_E[5]  +m_E[6] +m_E[7]
*/
ON_DECL
ON_Brep* ON_BrepBox( const ON_3dPoint* box_corners, ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a wedge.
Parameters:
  corners - [in] 6 points defining the box corners
     arranged as the vN labels indicate.

                     /v5    
                    /|\       
                   / | \     
                  e5 |  e4   
                 /   e8  \     
                /__e3_____\  
              v3|    |    |v4     
                |    |    |       
                |    /v2  |   
                e6  / \   e7   
                |  /   \  |   
                | e2    e1|   
                |/       \|     
                /____e0___\  
              v0           v1

  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the wedge with topology

  edge              vertices
    m_E[ 0]           m_V[0], m_V[1]
    m_E[ 1]           m_V[1], m_V[2]
    m_E[ 2]           m_V[2], m_V[0]
    m_E[ 3]           m_V[3], m_V[4]
    m_E[ 4]           m_V[4], m_V[5]
    m_E[ 5]           m_V[5], m_V[0]
    m_E[ 6]           m_V[0], m_V[3]
    m_E[ 7]           m_V[1], m_V[4]
    m_E[ 8]           m_V[2], m_V[5]

  face              boundary edges
    m_F[0]            +m_E[0] +m_E[7]  -m_E[3] -m_E[6]
    m_F[1]            +m_E[1] +m_E[8]  -m_E[4] -m_E[7]
    m_F[2]            +m_E[2] +m_E[6]  -m_E[5] -m_E[8]
    m_F[3]            +m_E[3] +m_E[8]  -m_E[7] -m_E[11]
    m_F[4]            -m_E[2]  -m_E[1] -m_E[0]
    m_F[5]            +m_E[3] +m_E[4]  +m_E[5]
*/
ON_DECL
ON_Brep* ON_BrepWedge( const ON_3dPoint* corners, ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a sphere.
Parameters:
  sphere - [in]
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the sphere with a single face,
  a single edge along the seam, and vertices at the north
  and south poles.
*/
ON_DECL
ON_Brep* ON_BrepSphere( const ON_Sphere& sphere, ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a sphere.
Parameters:
  Center - [in] Center of sphere
  radius - [int] Radius of sphere
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the sphere with six similar faces,
  each an untrimmed rational quadratic surface
*/
ON_DECL
ON_Brep* ON_BrepQuadSphere( const ON_3dPoint& Center, double radius, ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a torus.
Parameters:
  torus - [in]
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the torus with a single face
  a two edges along the seams.
*/
ON_DECL
ON_Brep* ON_BrepTorus( const ON_Torus& torus, ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a cylinder.
Parameters:
  cylinder - [in] cylinder.IsFinite() must be true
  bCapBottom - [in] if true end at cylinder.m_height[0] should be capped
  bCapTop - [in] if true end at cylinder.m_height[1] should be capped
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the cylinder with a single
  face for the cylinder, an edge along the cylinder seam, 
  and vertices at the bottom and top ends of this seam edge.
  The optional bottom/top caps are single faces with one
  circular edge starting and ending at the bottom/top vertex.
*/
ON_DECL
ON_Brep* ON_BrepCylinder( const ON_Cylinder& cylinder, 
                          bool bCapBottom,
                          bool bCapTop,
                          ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a cone.
Parameters:
  cylinder - [in] cylinder.IsFinite() must be true
  bCapBase - [in] if true the base of the cone should be capped.
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the cone with a single
  face for the cone, an edge along the cone seam, 
  and vertices at the base and apex ends of this seam edge.
  The optional cap is a single face with one circular edge 
  starting and ending at the base vertex.
*/
ON_DECL
ON_Brep* ON_BrepCone( 
          const ON_Cone& cone, 
          bool bCapBottom,
          ON_Brep* pBrep = nullptr 
          );

/*
Description:
  Get an ON_Brep form of a surface of revolution.
Parameters:
  pRevSurface - [in] pointer to a surface of revolution.
     The brep will manage this pointer and delete it in ~ON_Brep.
  bCapStart - [in] if true, the start of the revolute is
     not on the axis of revolution, and the surface of revolution
     is closed, then a circular cap will be added to close
     of the hole at the start of the revolute.
  bCapEnd - [in] if true, the end of the revolute is
     not on the axis of revolution, and the surface of revolution
     is closed, then a circular cap will be added to close
     of the hole at the end of the revolute.
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  @untitled table
  true     successful
  false    brep cannot be created from this surface.
Remarks:
  The surface class must be created with new because
  it will be destroyed with the delete operator
  in ~ON_Brep.
*/
ON_DECL
ON_Brep* ON_BrepRevSurface( 
          ON_RevSurface*& pRevSurface,
          bool bCapStart,
          bool bCapEnd,
          ON_Brep* pBrep = nullptr 
          );


          
/*
Description:
  Create an ON_Brep trimmed plane.
Parameters:
  plane - [in] plane that will be trimmed.
  boundary - [in] a simple (no self intersections) closed
      curve that defines the outer boundary of the trimmed
      plane.  This curve is copied for use in the brep.
  pBrep - [in] if not nullptr, this brep will be used and returned.
Returns:
  An ON_Brep representation of the trimmed plane with a single face.
See Also:
  ON_Brep::NewPlanarFaceLoop()
*/
ON_DECL
ON_Brep* ON_BrepTrimmedPlane( 
            const ON_Plane& plane, 
            const ON_Curve& boundary,
            ON_Brep* pBrep = nullptr );

/*
Description:
  Get an ON_Brep definition of a trimmed plane.
Parameters:
  plane - [in] plane that will be trimmed.
  boundary - [in] a list of 3d curves that form a simple 
      (no self intersections) closed curve that defines the
      outer boundary of the trimmed plane.
  bDuplicateCurves - [in] if true, duplicates of the
       curves in the boundary array are used in the brep.  If false
       the curves in the boundary array are used in the brep
       and the brep's destructor will delete the curves.
  pBrep - [in] if not nullptr, this brep will be used and
               returned.
Returns:
  An ON_Brep representation of the trimmed plane with a single face.
See Also:
  ON_Brep::NewPlanarFaceLoop()
*/
ON_DECL
ON_Brep* ON_BrepTrimmedPlane( 
            const ON_Plane& plane, 
            ON_SimpleArray<ON_Curve*>& boundary,
            bool bDuplicateCurves = true,
            ON_Brep* pBrep = nullptr );


/*
Description:
  Extrude a brep
Parameters:
  brep - [in/out]
  path_curve - [in] path to extrude along.
  bCap - [in] if true, the extrusion is capped with a translation
              of the input brep.
Returns:
  True if successful.
See Also:
  ON_BrepExtrudeFace
  ON_BrepExtrudeLoop
  ON_BrepExtrudeEdge
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new faces are appended to brep.m_F[]. It is the caller's
  responsibility to insure the result does not self intersect.
*/
ON_DECL
bool ON_BrepExtrude( 
          ON_Brep& brep,
          const ON_Curve& path_curve,
          bool bCap = true
          );

/*
Description:
  Extrude a face in a brep.
Parameters:
  brep - [in/out]
  face_index - [in] index of face to extrude.
  path_curve - [in] path to extrude along.
  bCap - [in] if true, the extrusion is capped with a translation
              of the face being extruded.
Example:
  Extrude a face along a vector.

          ON_Brep brep = ...;
          int face_index = ...;
          ON_3dVector v = ...;
          ON_LineCurve line_curve( ON_Line( ON_3dPoint::Origin, vector ) );
          ON_BrepExtrudeFace( brep, face_index, line_curve, true );

Returns:
  @untitled table
  0    failure
  1    successful - no cap added
  2    successful - cap added as last face
See Also:
  ON_BrepExtrude
  ON_BrepExtrudeLoop
  ON_BrepExtrudeEdge
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new faces are appended to brep.m_F[].  If a cap is requested
  it is the last face in the returned brep.m_F[]
*/
ON_DECL
int ON_BrepExtrudeFace( 
          ON_Brep& brep,
          int face_index,
          const ON_Curve& path_curve,
          bool bCap = true
          );

/*
Description:
  Extrude a loop in a brep.
Parameters:
  brep - [in/out]
  loop_index - [in] index of face to extrude.
  path_curve - [in] path to extrude along.
  bCap - [in] if true and the loop is closed, the extrusion
              is capped.
Returns:
  @untitled table
  0    failure
  1    successful - no cap added
  2    successful - cap added as last face
See Also:
  ON_BrepExtrude
  ON_BrepExtrudeFace
  ON_BrepExtrudeEdge
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new faces are appended to brep.m_F[].  If a cap is requested
  it is the last face in the returned brep.m_F[]
*/
ON_DECL
int ON_BrepExtrudeLoop( 
          ON_Brep& brep,
          int loop_index,
          const ON_Curve& path_curve,
          bool bCap = true
          );

/*
Description:
  Extrude an edge in a brep.
Parameters:
  brep - [in/out]
  edge_index - [in] index of face to extrude.
  path_curve - [in] path to extrude along.
Returns:
  @untitled table
  0    failure
  1    successful
See Also:
  ON_BrepExtrude
  ON_BrepExtrudeFace
  ON_BrepExtrudeLoop
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new face is appended to brep.m_F[].
*/
ON_DECL
int ON_BrepExtrudeEdge( 
          ON_Brep& brep,
          int edge_index,
          const ON_Curve& path_curve
          );


/*
Description:
  Extrude a vertex in a brep.
Parameters:
  brep - [in/out]
  vertex_index - [in] index of vertex to extrude.
  path_curve - [in] path to extrude along.
Returns:
  @untitled table
  0    failure
  1    successful
See Also:
  ON_BrepExtrude
  ON_BrepExtrudeFace
  ON_BrepExtrudeLoop
  ON_BrepExtrudeEdge
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new vertex is appended to brep.m_V[] and
  the new edge is appended to brep.m_E[].
*/
ON_DECL
int ON_BrepExtrudeVertex( 
          ON_Brep& brep,
          int vertex_index,
          const ON_Curve& path_curve
          );


/*
Description:
  Cone a face in a brep.
Parameters:
  brep - [in/out]
  face_index - [in] index of face to extrude.
  apex_point - [in] apex of cone.
Returns:
  @untitled table
  0    failure
  1    successful
See Also:
  ON_BrepExtrudeFace
  ON_BrepExtrudeLoop
  ON_BrepExtrudeEdge
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new faces are appended to brep.m_F[].
*/
ON_DECL
int ON_BrepConeFace( 
          ON_Brep& brep,
          int face_index,
          ON_3dPoint apex_point
          );

/*
Description:
  Cone a loop in a brep.
Parameters:
  brep - [in/out]
  loop_index - [in] index of face to extrude.
  apex_point - [in] apex of cone.
Returns:
  @untitled table
  0    failure
  1    successful
See Also:
  ON_BrepExtrudeFace
  ON_BrepExtrudeLoop
  ON_BrepExtrudeEdge
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new faces are appended to brep.m_F[].
*/
ON_DECL
bool ON_BrepConeLoop( 
          ON_Brep& brep,
          int loop_index,
          ON_3dPoint apex_point
          );

/*
Description:
  Cone an edge in a brep.
Parameters:
  brep - [in/out]
  edge_index - [in] index of face to extrude.
  apex_point - [in] apex of cone.
Returns:
  @untitled table
  0    failure
  1    successful
See Also:
  ON_BrepExtrudeFace
  ON_BrepExtrudeLoop
  ON_BrepExtrudeEdge
  ON_BrepExtrudeVertex
  ON_BrepConeFace
  ON_BrepConeLoop
  ON_BrepConeEdge
Remarks:
  The new face is appended to brep.m_F[].
*/
ON_DECL
int ON_BrepConeEdge( 
          ON_Brep& brep,
          int edge_index,
          ON_3dPoint apex_point
          );

//These merge adjacent faces that have the same underlying surface.
ON_DECL
int ON_BrepMergeFaces(ON_Brep& B, int fid0, int fid1);

ON_DECL
bool ON_BrepMergeFaces(ON_Brep& B);

//This removes all slit trims  from F that are not joined to another face.
//Unlike ON_Brep::RemoveSlits(), this will remove slit pairs from a loop in cases 
//that will result in the creation of more loops. Caller is responsible for calling 
//ON_Brep::Compact() to get rid of deleted trims and loops.

ON_DECL
bool ON_BrepRemoveSlits(ON_BrepFace& F);

//Merges all possible edges
ON_DECL
void ON_BrepMergeAllEdges(ON_Brep& B);

/*
Description:
  Merges two breps into a single brep.  The
  result may be non-manifold or have multiple
  connected components.
Parameters:
  brep0 - [in]
  brep1 - [in]
  tolerance - [in]
Returns:
  Merged brep or nullptr if calculation failed.
*/
ON_DECL
ON_Brep* ON_MergeBreps(
          const ON_Brep& brep0,
          const ON_Brep& brep1,
          double tolerance
          );

/*
Description:
  Very low level utility.  Order edges around a vertex.
Parameters:
  B - [in]
  vid - [in]
  trim_ends - [out] trim_ends[a].i is a trim index, trim_ends[a].j is 0 for start or 1 for end.
                    The nth is B.m_T[trim_ends[n].i].Edge(). If bClosed is false, then the first and last edges will be naked.
  bClosed - [out] If true, then all edges at the vertex have exactly two trims
Returns:
  True if the order can be found.  If any edge at the vertex is non-manifold, or if more than two are naked, then false.
*/
ON_DECL
bool ON_OrderEdgesAroundVertex(const ON_Brep& B, int vid,
                               ON_SimpleArray<ON_2dex>& trim_ends, 
                               bool& bClosed);


/*
Description:
Very low level utility.  Order edges around a vertex.
Parameters:
B - [in]
vid - [in]
trim_ends - [out] trim_ends[a].i is a trim index, trim_ends[a].j is 0 for start or 1 for end.
The nth is B.m_T[trim_ends[n].i].Edge(). If bClosed is false, then the first and last edges will be naked.
Must have at least as many ON2dex as the vertex has edges.
bClosed - [out] If true, then all edges at the vertex have exactly two trims
Returns:
True if the order can be found.  If any edge at the vertex is non-manifold, or if more than two are naked, then false.
NOTE: If you don't know how many edges are at the vertex, call the version that takes an ON_SimpleArray.
*/
ON_DECL
bool ON_OrderEdgesAroundVertex(const ON_Brep& B, int vid,
  ON_2dex* trim_ends,//Must be at as big as the edge count at the vertex
  bool& bClosed);

#if defined(OPENNURBS_PLUS)

/*
Description:
  Does extensive tests of an edge and all of its trims to
  insure the edge and trims define the same 3d curve to
  a tolerance.
Parameters:
  brep - [in]
  edge_index - [in]
  text_log - [in]
  bad_tp - [out]
    If not null and false is returned, then bad_tp->t and 
    bad_tp->e will identify trim and edge parameters near the
    area where the two have parameterization problems.
  bad_eti - [out]
    If not null and false is returned, then *bad_eti will
    be the edge.m_ti[] index of the trim whose correspondence
    with the edge has parameterization problems.
  deviation - [out]
    If not null, then deviation[] should have length edge.m_ti.Count().
    If true is returned, then the value of deviation[i] will be the 
    maximum detected 3d deviation between the trim with index 
    edge.m_ti[i] and the edge.
Returns:
  True if the edge and trims have is valid parameterizations.
*/
ON_DECL
bool ON_ValidateEdgeTrims(
        const ON_Brep& brep,
        int edge_index,
        ON_TextLog* text_log,
        ON_BrepTrimPoint* bad_tp,
        int* bad_eti,
        double* deviation
        );

class ON_CLASS ON_BendFaceConstraint
{
public:
  ON_BendFaceConstraint();

  /*
  Description:
    Compare function for sorting and searching 
    arrays of ON_BendFaceEdgeInformation elements.
  Parameters:
    a - [in]
      Must not be null.
    b - [in]
      Must not be null.
  Returns:
    -1: a < b
    +1: a > b
     0: a == b
  */
  static int CompareComponentIndex(
    const class ON_BendFaceConstraint* a,
    const class ON_BendFaceConstraint* b
    );
    
  /*
  Description:
    Set a constraint based on information in an ON_BrepTrim.
  Parameters:
    trim - [in]
    curve_constraint - [in]
    tolerance - [in]
      Desired fitting tolerance
  Returns:
    True if the constraint was set.
  */
  bool SetCurveConstraint(
    const ON_BrepTrim* trim,
    const ON_Curve* curve_constraint,
    double tolerance
    );

  /*
  Description:
    Set a constraint based on information in an ON_BrepTrim.
  Parameters:
    trim - [in]
    surface_constraint - [in]
    tolerance - [in]
      Desired fitting tolerance
  Returns:
    True if the constraint was set.
  */
  bool SetSurfaceConstraint(
    const ON_BrepTrim* trim,
    const ON_Surface* surface_constraint,
    double tolerance
    );

  /*
  Description:
    Set a constraint so the specified side of the surface returned
    by ON_BendBrepFace will match curve_constraint.
  Parameters:
    trim - [in]
    curve_constraint - [in]
      3d curve
    tolerance - [in]
      Desired fitting tolerance
  Returns:
    True if the constraint was set.
  */
  bool SetSurfaceSideConstraint(
    ON_Surface::ISO surface_side,
    const ON_Curve* curve_constraint,
    double tolerance
    );

  /*
  Description:
    Set a constraint so the specified side of the surface returned
    by ON_BendBrepFace will match the same side surface_constraint.
  Parameters:
    trim - [in]
    surface_constraint - [in]
      3d surface whose side should be matched.
    tolerance - [in]
      Desired fitting tolerance
  Returns:
    True if the constraint was set.
  */
  bool SetSurfaceSideConstraint(
    ON_Surface::ISO surface_side,
    const ON_Surface* surface_constraint,
    double tolerance
    );

  /*
  Description:
    Set a constraint so the specified seam of a closed surface
    will be set by attenuating the changes applied to its ends
    by other constraints.  This is a robust way to "fit"
    a periodic seam on a tube-like surface.
  Parameters:
    surface_side - [in]
      side that identifies the seam.
  Returns:
    True if the constraint was set.
  */
  bool SetSurfaceSeamFromEndsConstraint(
    ON_Surface::ISO surface_side
    );

  /*
  Description:
    Set a constraint so the surface returned by ON_BendBrepFace will
    be closed along the specified side.
  Parameters:
    surface_side - [in]
      ON_Surface::W_iso, ON_Surface::S_iso, 
      ON_Surface::E_iso or ON_Surface::W_iso
  Returns:
    True if the constraint was set.
  */
  bool SetClosedSurfaceConstraint(
    ON_Surface::ISO surface_side
    );

  /*
  Description:
    Set a singular constraint based on information in an ON_BrepTrim.
  Parameters:
    surface_side - [in]
      ON_Surface::W_iso, ON_Surface::S_iso, 
      ON_Surface::E_iso or ON_Surface::W_iso
    point - [in]
      Desired location of the singularity.
  Returns:
    True if the constraint was set.
  */
  bool SetSingularConstraint(
    ON_Surface::ISO surface_side,
    ON_3dPoint point
    );

  /*
  Description:
    Set a surface evaluation point constraint.
  Parameters:
    s - [in]
    t - [in]
      surface evaluation parameters
    point - [in]
      Desired location.
  Returns:
    True if the constraint was set.
  */
  bool SetPointConstraint(
    double s,
    double t,
    ON_3dPoint point
    );

  /*
  The value m_contraint_type determines what type of constraint
  to apply.
  
  0: Ignore this constraint.
  
  1: m_brep_trim_index identifies a brep trim.
     The surface along the trim will will be constrained to
     m_curve_constraint. m_curve_constraint must be oriented
     to take the trim's m_bRev3d flag into account.
  
  2: m_brep_trim_index identifies a brep trim.
     The surface along the trim will will be constrained to
     location of m_surface_constraint along the trim.
  
  3 - 6: Surface Singularity
     These constraints specify the surface returned by 
     ON_BendBrepFace will have a singularity on the side
     identified by the constraint value at the location specified
     by the value in m_point. The values of m_brep_trim_index, 
     m_curve_constraint and m_surface_constraint are ignored.
  3: west side singularity.
  4: south side singularity.
  5: east side singularity.
  6: north side singularity.
  
  7 - 8: Closed surface
     These constraints specify the surface returned by 
     ON_BendBrepFace will be closed in the parameter
     direction identified by the constraint value. The knot
     vector on the surface passed to ON_BendBrepFace
     determines if the the closure is clamped or periodic.
     The values of m_brep_trim_index, m_curve_constraint
     and m_surface_constraint are ignored.
  7: Closed surface the first parameter direction. (west = east)
  8: Closed surface in the second parameter direction. (south = north)
  
  13 - 16: Constrain a surface side to a curve.
     The specified side of the surface is constrained to
     the curve specified by m_curve_constraint.
     The domain and parameterization of m_curve_constaint
     must be set to agree with the portion of the side of
     the surface that is being constrained. The values of 
     m_brep_trim_index and m_surface_constraint are 
     ignored.
  13: west side lies on m_curve_constraint.
  14: south side lies on m_curve_constraint.
  15: east side lies on m_curve_constraint.
  16: north side lies on m_curve_constraint.
  
  23 - 26: Constrain a surface side to match another
     surface side.
     The specified side of the surface is constrained to
     match the same side of m_surface_constraint.
     The domain and parameterization of m_surface_constraint
     must be set to agree with the the side of the surface 
     that is being constrained. The values of 
     m_brep_trim_index and m_curve_constraint are
     ignored.
  23: west side matches west side of m_surface_constraint.
  24: south side matches west side of m_surface_constraint.
  25: east side matches west side of m_surface_constraint.
  26: north side matches west side of m_surface_constraint.

  27: Point constraint
      m_surface_parameters[2] = surface evaluation parameters
      m_point = 3d point.

  28: Set east-west seam from fixed north and south sides.      
  29: Set south-north seam from fixed west and east sides.      

 */
  unsigned int m_contraint_type; 

  // m_brep_trim_index identifies a trim on the face being bent
  int m_brep_trim_index;

private:
  unsigned char m_reserved1[7];
public:

  // If m_rev_curve_constraint is 0, the curve will be evaluated
  // as it is oriented. If m_rev_curve_constraint is 1, the curve
  // will be evaluated in a reverse orientation. In both cases, 
  // the trim's m_bRev3d settings into account. That is, the 
  // evaluated points on the curve with m_rev_curve_constraint
  // taken into account, must run in the direction indicated by 
  // by the trim's m_bRev3d value.
  unsigned char m_rev_curve_constraint;
  const ON_Curve* m_curve_constraint;

  // If the value of m_action is 2 or 23 - 26, then this
  // surface is used.
  const ON_Surface* m_surface_constraint;

  // If the value of m_action is 27, then a m_surface_parameters[]
  // specifies the evaluation parameters for the point constraint.
  double m_surface_parameters[2];

  // If the value of m_action is 3 through 6, then m_point
  // is a 3d location of the singular side.  If the value
  // of m_action is 27, then m_point is the 3d location
  // of the point constraint.
  ON_3dPoint m_point;

  // If the surface is being constrained to pass through 
  // m_curve_constraint or m_surface_constraint, then 
  // m_tolerance is the desired edge tolerance.  If the 
  // boundary conditions or tolerance are unreasonable,
  // then the fit may exceed this tolerance.
  double m_tolerance;

private:
  double m_reserved2[8];
public:
};

/*
Description:
  Calculate a surface that can be used to deform a brep
  face so that is has a new boundary.
Parameters:
  face0 - [in]
    existing valid face
  constraints - [in]
    Desired face boundary locations, surface singularities,
    and closed/periodic surface constraints.
  tolerance - [in]
    Desired tolerance
  time_limit - [in]
    If the deformation is extreme, it can take a long time to
    calculate the result. If time_limit > 0, then the value
    specifies the maximum amount of time in seconds you want
    to spend before giving up.
  bent_surface - [in]
    If bent_surface is not null, the result will be
    stored in this surface.  

    If bent_surface is null or is not a valid NURBS
    surface, then the face0->NurbForm() will be used
    as the starting surface. 

    If bent_surface is not null and a valid NURBS surface,
    then it will be used as the starting surface. In this
    case the knots must be set and the domain must include
    the range of all face0's trim curves.
  text_log - [in]
    optional parameter to use when debugging
Returns:
  A surface that can be used to update face0.
*/
ON_DECL
ON_NurbsSurface* ON_BendBrepFace( 
  const ON_BrepFace& face0,
  const ON_SimpleArray<ON_BendFaceConstraint>& constraints,
  double tolerance,
  double time_limit,
  ON_NurbsSurface* bent_surface,
  ON_TextLog* text_log = 0
  );

/*
Description:
  Transform a list of brep components, bend neighbors
  to match, and leave the rest fixed.
Parameters:
  brep - [in]
    brep to modify
  brep_component_count - [in]
    Length of brep_components[] array
  brep_components - [in]
    components to transform.
  xform - [in]
    transformation to apply to components
  tolerance - [in]
    Desired fitting tolerance to use when bending
    faces that share edges with both fixed and 
    transformed components.
  time_limit - [in]
    If the deformation is extreme, it can take a long time to
    calculate the result. If time_limit > 0, then the value
    specifies the maximum amount of time in seconds you want
    to spend before giving up.
  bUseMultipleThreads - [in]
    True if multiple threads can be used.
Returns:
  True if successful.
*/
ON_DECL
bool ON_TransformBrepComponents( 
  ON_Brep* brep,
  int brep_component_count,
  const ON_COMPONENT_INDEX* brep_components,
  ON_Xform xform,
  double tolerance,
  double time_limit,
  bool bUseMultipleThreads
  );

#endif


#if defined(OPENNURBS_PLUS)
class ON_CLASS ON_BrepQuadFacePackSide {
 public:
  ON_BrepQuadFacePackSide() : m_face_ptr{nullptr}, m_trim_dex{-1} {};
  ~ON_BrepQuadFacePackSide() = default;
  ON_BrepQuadFacePackSide(const ON_BrepQuadFacePackSide&) = default;
  ON_BrepQuadFacePackSide& operator=(const ON_BrepQuadFacePackSide&) = default;

  ON_BrepQuadFacePackSide(ON_BrepFace* face, int ti)
      : m_face_ptr{face}, m_trim_dex{ti} {};

 public:
  enum : unsigned { SeedCapacity = 8 };

  static const ON_BrepQuadFacePackSide CreateValid(const ON_BrepFace* face,
                                                   int trim_dex);

  inline bool operator==(const ON_BrepQuadFacePackSide& rhs) {
    return (IsNotNull() && rhs.IsNotNull() && m_face_ptr == rhs.m_face_ptr &&
            m_trim_dex == rhs.m_trim_dex);
  }

  inline bool operator!=(const ON_BrepQuadFacePackSide& rhs) {
    return !operator==(rhs);
  }

  inline ON_Brep* Brep() const {
    return (IsNull() ? nullptr : m_face_ptr->Brep());
  }

  inline ON_BrepFace* Face() const { return m_face_ptr; }

  inline ON_NurbsSurface* NurbsSurface(
      ON_NurbsSurface* pNurbsSurface = nullptr) const {
    return (IsNull() ? nullptr : m_face_ptr->NurbsSurface(pNurbsSurface));
  }

  bool ChangeSurface(ON_NurbsSurface* surface);

  inline int TrimDex() const { return m_trim_dex; }

  inline void SetPackId(unsigned int pack_id) {
    if (IsNotNull()) m_face_ptr->SetPackIdForExperts(pack_id);
  }

  inline unsigned PackId() const {
    return (IsNull() ? 0 : m_face_ptr->PackId());
  }

  inline const ON_BrepTrim* Trim() const {
    return ((m_face_ptr == nullptr || m_face_ptr->LoopCount() != 1 ||
             m_face_ptr->Loop(0) == nullptr)
                ? nullptr
                : m_face_ptr->Loop(0)->Trim(m_trim_dex));
  }

  inline bool IsNull() const { return nullptr == m_face_ptr; }

  inline bool IsNotNull() const { return nullptr != m_face_ptr; }

  const ON_BrepQuadFacePackSide NeighborFace(unsigned relative_side_dex,
                                             bool check_pack_id = true) const;

  const ON_BrepQuadFacePackSide RelativeSide(unsigned relative_side_dex) const;

  const ON_BrepTrim* RelativeTrim(unsigned relative_side_dex) const;

  inline bool IsPackable(int valence = 4, unsigned pack_id = 0) const {
    return (nullptr != m_face_ptr &&
            false == m_face_ptr->m_status.RuntimeMark() &&
            IsPackableFace(m_face_ptr, valence, pack_id));
  }

  static bool IsPackableFace(const ON_BrepFace* face, int valence = 4,
                             unsigned pack_id = 0);

  inline void SetMark(bool bMark) const {
    if (IsNotNull()) m_face_ptr->m_status.SetRuntimeMark(bMark);
  }

  inline bool Mark() const {
    return (IsNotNull() ? m_face_ptr->m_status.RuntimeMark() : false);
  }

  static unsigned GetStrip(ON_BrepQuadFacePackSide f,
                           ON_SimpleArray<ON_BrepQuadFacePackSide>& strip,
                           unsigned strip_capacity);
  static unsigned GetStrip(ON_BrepQuadFacePackSide f,
                           ON_BrepQuadFacePackSide* strip,
                           unsigned strip_capacity);

  static unsigned GetStripTagged(ON_BrepQuadFacePackSide f,
                                 ON_SimpleArray<ON_BrepQuadFacePackSide>& strip);

  static bool CheckStrip(const ON_SimpleArray<ON_BrepQuadFacePackSide>& strip,
                         int valence = 4, unsigned pack_id = 0);
  static bool CheckStrip(const ON_BrepQuadFacePackSide* strip,
                         unsigned strip_count, int valence = 4,
                         unsigned pack_id = 0);

  static unsigned GetStar(ON_BrepQuadFacePackSide f,
                          ON_SimpleArray<ON_BrepQuadFacePackSide>& star,
                          unsigned star_capacity);
  static unsigned GetStar(ON_BrepQuadFacePackSide f,
                          ON_BrepQuadFacePackSide* star,
                          unsigned star_capacity);

  static bool CheckStar(const ON_SimpleArray<ON_BrepQuadFacePackSide>& star,
                        unsigned pack_id = 0);
  static bool CheckStar(const ON_BrepQuadFacePackSide* star,
                        unsigned star_count, unsigned pack_id = 0);

  static bool StripsAreNeighbors(const ON_BrepQuadFacePackSide* strip0,
                                 unsigned strip0_side_dex,
                                 const ON_BrepQuadFacePackSide* strip1,
                                 unsigned strip1_side_dex, unsigned count,
                                 bool check_pack_id = true);

  static void SetStripMark(bool bMark, const ON_BrepQuadFacePackSide* strip,
                           unsigned count);

  static void SetStarMark(bool bMark, const ON_BrepQuadFacePackSide* star,
                          unsigned count);

  static bool GetPackSeed(
      const ON_BrepFace* face, unsigned count,
      ON_BrepQuadFacePackSide group[ON_BrepQuadFacePackSide::SeedCapacity]
                                   [ON_BrepQuadFacePackSide::SeedCapacity]);

  static bool MoveToStripEndTagged(ON_BrepQuadFacePackSide* f);

  static bool GetPackCornerTagged(const ON_BrepFace* face,
                                  ON_BrepQuadFacePackSide* corner);

  static void ReverseStrip(ON_BrepQuadFacePackSide* strip,
                           unsigned strip_count);

  static const bool RewindToIsoSideTrimStart(const ON_BrepFace* face, int* ti);

  static const bool MoveToNextIsoSideTrim(const ON_BrepFace* face, int* ti);

  static const bool CheckBrepFaceTrimsAreFullIsoSides(const ON_BrepFace* face);

  static const bool CheckCornersValence(const ON_BrepFace* face, int valence = 4);

 private:
  ON_BrepFace* m_face_ptr{nullptr};
  int m_trim_dex{-1};
};
#endif

#if defined(OPENNURBS_PLUS)
class ON_CLASS ON_BrepQuadFacePack {
 public:
  static const ON_BrepQuadFacePack Empty;

 public:
  ON_BrepQuadFacePack() = default;
  ~ON_BrepQuadFacePack() = default;
  ON_BrepQuadFacePack(const ON_BrepQuadFacePack&) = default;
  ON_BrepQuadFacePack& operator=(const ON_BrepQuadFacePack&) = default;

  ON_Brep* Brep() const;

  inline unsigned FaceCount() const { return m_size.i * m_size.j; }
  inline unsigned FaceCount(int dir) const { return dir ? m_size.j : m_size.i; }
  inline unsigned FaceCount(bool dir) const { return dir ? m_size.j : m_size.i; }

  inline bool IsEmpty() const { return m_size.i == 0 && m_size.j == 0; }

  inline bool IsNotEmpty() const { return !IsEmpty(); }

  const bool OrientPackFaces();

  const bool MatchPackFacesStructure();

  const ON_NurbsSurface* GetSurface();

  const ON_NurbsSurface* GetSurfaceSimpleKnots();

  const ON_NurbsSurface* GetSurfaceMixedKnots();
 
  const ON_NurbsSurface* GetSurfaceMultipleKnots();

  const bool GetTrims(const ON_NurbsSurface* srf,
                      ON_SimpleArray<int>* trim_origs_pack,
                      ON_ObjectArray<ON_NurbsCurve>* trim_curves_pack);

  /*
  Parameters:
    pack_seed_size - [in]
      >= 0
      The minimum size quad grid that will be returned is pack_seed_size X
      pack_seed_size.
    face - [in] The quad pack will be grown around this face.
    pack_id - [in]
      If a quad pack is found, this is the id that will be assigned to each
      ON_BrepFace.m_pack_id in the quad pack.
  Returns: If a pack is found, ON_BrepQuadFacePack has details. Otherwise,
    ON_BrepQuadFacePack::Empty is returned.
  */
  static const ON_BrepQuadFacePack GetPack(unsigned int pack_seed_size,
                                           const ON_BrepFace* face,
                                           unsigned int pack_id);

  static const ON_BrepQuadFacePack GetPackTagged(const ON_BrepFace* face,
                                                 unsigned count);

 public:
  ON_2udex m_size = ON_2udex::Zero;
  ON_BrepQuadFacePackSide m_corners[4]{};
};
#endif

#if defined(OPENNURBS_PLUS)
class ON_CLASS ON_BrepStarFacePack {
 public:
  static const ON_BrepStarFacePack Empty;

 public:
  ON_BrepStarFacePack() = default;
  ~ON_BrepStarFacePack() = default;
  ON_BrepStarFacePack(const ON_BrepStarFacePack&) = default;
  ON_BrepStarFacePack& operator=(const ON_BrepStarFacePack&) = default;

  ON_Brep* Brep() const;

  inline ON_BrepVertex* Center() const {
    return (m_start.Trim() == nullptr ? nullptr : m_start.Trim()->Vertex(1));
  };

  inline unsigned FaceCount() const { return m_size; }

  inline bool IsEmpty() const { return m_size == 0U; }

  inline bool IsNotEmpty() const { return !IsEmpty(); }

  /*
  Parameters:
    face - [in] The star pack will be grown around one of the vertices of this face.
    pack_id - [in]
      If a star pack is found, this is the id that will be assigned to each
      ON_BrepFace.m_pack_id in the quad pack.
  Returns: If a pack is found, ON_BrepStarFacePack has details. Otherwise,
    ON_BrepStarFacePack::Empty is returned.
  */
  static const ON_BrepStarFacePack GetPack(const ON_BrepFace* face,
                                           unsigned int pack_id);

  static const ON_BrepStarFacePack GetPackTagged(const ON_BrepFace* face,
                                                 unsigned count);

 public:
  unsigned m_size = 0U;
  ON_BrepQuadFacePackSide m_start{};
};
#endif

#if defined(ON_COMPILING_OPENNURBS)
//////////////////////////////////////////////////////////////////////////
//
// ON_BrepIncrementErrorCount()
//
//  Appears in places where the code traps error conditions.
//  Putting a breakpoint on the line indicated below is an easy way
//  to have the debugger break on all error conditions and inspect
//  the first place something goes wrong in a complex calculation.
//
void ON_BrepIncrementErrorCount();  // defined in opennurbs_error.cpp
#define ON_BREP_ERROR(msg) (ON_BrepIncrementErrorCount(), ON_ERROR(msg))
#define ON_BREP_RETURN_ERROR(rc) (ON_BrepIncrementErrorCount(), rc)
#define ON_BREP_RETURN_ERROR_MSG(msg, rc) \
  (ON_BrepIncrementErrorCount(), ON_ERROR(msg), rc)
#endif

#endif
