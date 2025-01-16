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

class RHINO_SDK_CLASS CRhinoPolyEdge : public ON_PolyCurve
{
  ON_OBJECT_DECLARE(CRhinoPolyEdge);
public:
  CRhinoPolyEdge();
  ~CRhinoPolyEdge();
  // default copy constructor and operator= are fine.
public:

  // virtual ON_Object::DestroyRuntimeCache override
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Curve::IsClosed override
  bool IsClosed() const override;

  // virtual ON_Curve::DuplicateCurve override
  //   This insures that the duplicate is a CRhinoPolyEdge class
  //   rather than the ON_PolyCurve class that is created
  //   by the base class.
  ON_Curve* DuplicateCurve() const override;

  /*
  Description:
    Create a one segment CRhinoPolyEdge curve that uses a 
    single edge.
  */
  bool Create(
           const ON_BrepTrim* trim,
           const CRhinoObject* rhino_object=0  
           );

  /*
  Description:
    Create a one segment CRhinoPolyEdge curve that uses a 
    single curve.
  */
  bool Create(
           const ON_Curve* curve,
           const CRhinoObject* rhino_object=0  
           );

  /*
  Description:
    Create a one segment CRhinoPolyEdge curve.
  */
  bool Create( const CRhinoObjRef& );

  bool Create( CRhinoDoc& doc, const ON_PolyEdgeHistory& history_value );

  void Highlight(bool bHighlight=true) const;
  bool IsHighlighted() const;
  void Select(bool bSelect=true,bool bSynchHighlight=true) const;
  bool IsSelected() const;

  int SegmentCount() const;

  class CRhinoPolyEdgeSegment* SegmentCurve(
    int segment_index
    ) const;

  class CRhinoPolyEdgeSegment* operator[](int) const;

  BOOL32 Prepend( class CRhinoPolyEdgeSegment* new_segment ); // Prepend curve.
  BOOL32 Append( class CRhinoPolyEdgeSegment* new_segment );  // Append curve.
  BOOL32 Insert( 
           int segment_index,
           class CRhinoPolyEdgeSegment* new_segment
           );

  // if the segment is an edge, the following
  // return non-NULL pointers.
  const ON_BrepEdge* EdgeAt(double t) const;
  const ON_BrepTrim* TrimAt(double t) const;
  const ON_Brep*     BrepAt(double t) const;
  const ON_BrepFace* FaceAt(double t) const;
  const ON_Surface*  SurfaceAt(double t) const;
  ON_Surface::ISO    IsoType( double t) const;

  double EdgeParameter(double t) const;
  double TrimParameter(double t) const;
  ON_2dPoint SurfaceParameter(double t) const;

  // Test if there are any surface edges in the polyedge
  bool ContainsAnyEdges() const;
  // Test if all segments of the polyedge are surface edges
  bool ContainsAllEdges() const;
  
  /*
  Description:
    The CRhinoPolyEdge EvSrf... tools can evaluate the 
    "binormal" surface tangent direction in several different
    ways.  The binormal surface tangent always points "out".
    @untitled table
    tmAuto     if trim is a srf iso, then tm_IsoDir, else tmPerp
    tmIsoDir   srf tangent will be parallel to du or dv
    tmPerp     srf tangent will be perpendicular to edge
  See Also:
    CRhinoPolyEdge::SrfTangentMode
    CRhinoPolyEdge::SetSrfTangentMode
  */
  enum eTanMode
  {
    tmAuto = 0, 
    tmIsoDir = 1,
    tmPerp = 2,
    tmUser = 3,
  };

  /*
  Description:
    The CRhinoPolyEdge EvSrf... tools can evaluate the 
    "binormal" surface tangent direction in several different
    ways.
  Returns:
    Current value of mode by CRhinoPolyEdge::EvSrf... tools
    when computing value of srftangent parameter.
  See Also:
    CRhinoPolyEdge::SrfTangentMode
  */
  eTanMode SrfTangentMode() const;

  /*
  Description:
    The CRhinoPolyEdge EvSrf... tools can evaluate the 
    "binormal" surface tangent direction in several different
    ways.
  Parameters:
    tm - [in] mode for CRhinoPolyEdge::EvSrf... tools to use
              when computing value of srftangent parameter.
  See Also:
    CRhinoPolyEdge::SrfTangentMode
  */
  void SetSrfTangentMode( eTanMode tm );


  /*
  Description:
    Evaluate surface binormal and normal.
  Parameters:
    t - [in] CRhinoPolyEdge curve parameter
    srfpoint - [out] location on surface
    srftangent - [out] "binormal" tangent to the surface.
         The direction of this tangent is controlled by
         the surface tangent mode setting.
    srfnormal - [out] normal to surface
  See Also:
    CRhinoPolyEdge::SetSrfTangentMode    
  */
  bool EvSrfTangent( 
          double t,
          ON_3dPoint& srfpoint,
          ON_3dVector& srftangent,
          ON_3dVector& srfnormal
          ) const;

  /*
  Description:
    Evaluate surface binormal and normal.
  Parameters:
    t - [in] CRhinoPolyEdge curve parameter
    srfpoint - [out] location on surface
    edgetangent - [out] tangent to the edge
    srfbinormal - [out] "binormal" tangent to the surface, perpendicular to the edge
    srfisodir - [out] tangent to the surface, parallel to the isocurve closest to binormal direction
    srfnormal - [out] normal to surface
  */
  bool EvaluateTangents( 
          double t,
          ON_3dPoint& srfpoint,
          ON_3dVector& edgetangent,
          ON_3dVector& srfbinormal,
          ON_3dVector& srfisodir,
          ON_3dVector& srfnormal
          ) const;

  /*
  Description:
    Evaluate surface normal curvature
  Parameters:
    t - [in] CRhinoPolyEdge curve parameter
    srftangent - [in] tangent to the surface
    srfnormalcurvature - [out] normal curvature
    srfnormal - [out] normal to surface
  See Also:
    CRhinoPolyEdge::EvSrfTangent    
  */
  bool EvSrfNormalCurvature( 
            double t,
            ON_3dVector srftangent,
            ON_3dVector& srfnormalcurvature,
            ON_3dVector& srfnormal
            ) const;

  /*
  Description:
    Evaluate surface derivatives.
  Parameters:
    t - [in] CRhinoPolyEdge curve parameter
    srfpoint - [out] location on surface
    du - [out]
    dv - [out]
    duu - [out]
    duv - [out]
    dvv - [out]
  */
  bool EvSrfDerivatives(
          double t,
          ON_3dPoint& srfpoint,
          ON_3dVector& du,
          ON_3dVector& dv,
          ON_3dVector& duu,
          ON_3dVector& duv,
          ON_3dVector& dvv
          ) const;

  /*
  Description:
    See if this polyedge has an edge as one of its segments
  Parameters:
    edge - [in] the edge to look for
  Returns:
     -1: edge is not in the polyedge
    >=0: index of first segment that uses the edge
  */
  int FindEdge( const ON_BrepEdge* edge) const;

  /*
  Description:
    See if this polyedge has a trim as one of its segments
  Parameters:
    trim - [in] the trim to look for
  Returns:
     -1: trim is not in the polyedge
    >=0: index of first segment that uses the trim
  */
  int FindTrim( const ON_BrepTrim* trim) const;

  /*
  Description:
    See if this polyedge has a wire curve as one of its segments
  Parameters:
    curve - [in] the curve to look for
  Returns:
     -1: trim is not in the polyedge
    >=0: index of first segment that uses the curve
  */
  int FindCurve( const ON_Curve* curve) const;


  // OBSOLETE and unreliable.  Use FindCurve, FindEdge, or FindTrim
  //bool Contains( const ON_Curve* curve) const;

  // virtual ON_Curve overrides do nothing
  // to prevent changing edge
  bool SetStartPoint(ON_3dPoint start_point) override;
  bool SetEndPoint(ON_3dPoint end_point) override;
  bool ChangeClosedCurveSeam( double t ) override;
  BOOL32 PrependAndMatch(ON_Curve*);
  BOOL32 AppendAndMatch(ON_Curve*);

  // 7-1-03 lw added override to unset cached closed flag
  // when a segment is removed
  BOOL32 Remove(); // remove last
  BOOL32 Remove( int index);

  bool GetHistoryValue( CRhinoDoc& doc, ON_PolyEdgeHistory& polyedge_history ) const;

private:
  eTanMode m_ev_srf_tan_mode;
  int m_is_closed_helper; // 0 = unset, 1 = closed, 2 = not closed
};

class RHINO_SDK_CLASS CRhinoPolyEdgeSegment : public ON_CurveProxy
{
  ON_OBJECT_DECLARE(CRhinoPolyEdgeSegment);
public:
  // construction

  CRhinoPolyEdgeSegment();
  ~CRhinoPolyEdgeSegment();

  // The custom copy constructor and operator= are required
  // to manage m_or_copy.
  CRhinoPolyEdgeSegment(const CRhinoPolyEdgeSegment& );
  CRhinoPolyEdgeSegment& operator=(const CRhinoPolyEdgeSegment& );
  // default copy constructor and operator= are fine.

  // virtual ON_Object::DestroyRuntimeCache override
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Curve::IsClosed override
  bool IsClosed() const override;

  // virtual ON_Geometry::ComponentIndex override
  ON_COMPONENT_INDEX ComponentIndex() const override;

  // virtual ON_Curve::DuplicateCurve override
  //   This insures that the duplicate is a CRhinoPolyEdgeSegment
  //   class rather than a duplicate of the "real" curve
  //   the the ON_CurveProxy base class referse to.
  ON_Curve* DuplicateCurve() const override;

public:
  /*
  Description:
    Creates a polyedge segment that uses the entire edge
    and has the same domain as the edge.
  Parameters:
    trim - [in] 
  Returns:
    true if successful (edge was valid and trim_index was valid)
  Remarks:
    Use ON_Curve::SetDomain, ON_Curve::Trim, ON_Curve::Reverse,
    etc., to tweak the domain, support, direction etc.
  */
  bool Create( 
          const ON_BrepTrim* trim,
          const CRhinoObject* rhino_object=0
          );

  /*
  Description:
    Creates a polyedge segment that uses the entire curve
    and has the same domain as the curve.
  Parameters:
    curve - [in] 
  Remarks:
    Use ON_Curve::SetDomain, ON_Curve::Trim, ON_Curve::Reverse,
    etc., to tweak the domain, support, direction etc.
  */
  bool Create( 
          const ON_Curve* curve,
          const CRhinoObject* rhino_object=0
          );

  /*
  Description:
    Creates a polyedge segment from a curve and component index. This is needed for mesh objects, 
    because the curve is a proxy and does not carry the component index of the mesh edge.
  Parameters:
    curve - [in]
    component_index - [in]
  */
  bool Create(
    const ON_Curve* curve,
    const ON_COMPONENT_INDEX& component_index,
    const CRhinoObject* rhino_object = 0
  );

  /*
  Description:
    Creates a polyedge segment that uses the entire curve
    and has the same domain as the curve.
  Parameters:
    ref - [in] 
      ref.Curve() and ref.Object() are used to create a segment.
      This function does NOT store a copy of the ref on
      the segment.  Use SetSegmentObjRef() if you need to save
      a copy of a particular CRhinoObjRef.
  Remarks:
    Use ON_Curve::SetDomain, ON_Curve::Trim, ON_Curve::Reverse,
    etc., to tweak the domain, support, direction etc.
  */
  bool Create( const CRhinoObjRef& ref );

  bool Create( CRhinoDoc& doc, const ON_CurveProxyHistory& curveproxy_history );

  /*
  Description:
    Store a copy of a CRhinoObjRef on this polysegment class.
    The copy will be deleted when this polysegment is deleted.
  Parameters:
    ref - [in]
  Returns:
    True if it was possible to save a copy of ref.
    False if it was not possible to save a copy of ref because
    the internal storage facility was full.
  */
  void AddSegmentObjRef( const CRhinoObjRef& ref );

  /*
  Returns:
    Number of segment obj refs added by AddSegmentObjRef().
  */
  int SegmentObjRefCount() const;

  /*
  Returns:
    A pointer to any objref being stored by SetSegmentObjRef().
  */
  const CRhinoObjRef* SegmentObjRef( int segment_objref_index ) const;

  bool GetHistoryValue( CRhinoDoc& doc, ON_CurveProxyHistory& curveproxy_history ) const;

  void Highlight(bool bHighlight=true) const;
  bool IsHighlighted() const;
  void Select(bool bSelect=true,bool bSynchHighlight=true) const;
  bool IsSelected() const;


  const CRhinoObject* RhinoObject() const;
  const ON_BrepEdge* Edge() const;
  const ON_BrepTrim* Trim() const;
  const ON_Brep*     Brep() const;
  const ON_BrepFace* Face() const;
  const ON_Surface*  Surface() const;
  ON_Surface::ISO    IsoType() const;

  double EdgeParameter(double t) const;
  double TrimParameter(double t) const;
  ON_2dPoint SurfaceParameter(double t) const;

  /*
  Returns:
    True if this segment has an ON_BrepEdge and the direction of
    the ON_BrepEdge is the reverse of the direction of the segment.
  */
  bool ReversedEdgeDir() const;

  /*
  Returns:
    True if this segment has an ON_BrepTrim and the direction of
    the ON_BrepTrime is the reverse of the direction of the segment.
  */
  bool ReversedTrimDir() const;

  /*
  Returns:
    subdomain of the edge that this segment uses.  This can
    be different than the domain returned by this->Domain().
  */
  ON_Interval EdgeDomain() const;

  /*
  Returns:
    subdomain of the trim that this segment uses.  This can
    be different than the domain returned by this->Domain().
  */
  ON_Interval TrimDomain() const;

public:
  bool Trim(
    const ON_Interval& domain
    ) override;

  bool Split(
      double t,
      ON_Curve*& left_side,
      ON_Curve*& right_side
    ) const override;

private:
  friend class CRhinoPolyEdge;

  // domain of the CRhinoPolyEdgeSegment = this->Domain();
  // portion of the 3d curve used by this segment
  // = this->ProxyCurveDomain

  // Serial number of a rhino object
  unsigned int m_rhino_doc_sn = 0;
  unsigned int m_rhino_object_sn = 0;

  unsigned int m_reserved;

  ON_SimpleArray< CRhinoObjRef*> * m_or_copies; 
  // copies of the obj refs returned by GetObjects()
  // It is critical that these obj refs persist in
  // the cases like when the "brep" is a proxy from an
  // extrusion or instance reference.

  ON_COMPONENT_INDEX m_component_index;
  const class TL_Brep* m_brep;
  const ON_BrepTrim* m_trim;  // 2d trim in m_brep
  const ON_BrepEdge* m_edge;  // 3d edge in m_brep
  const ON_BrepFace* m_face;
  const ON_Surface* m_surface;
  ON_Surface::ISO m_iso_type;
  ON_Interval m_edge_domain;  // corresponding domain of the edge 
  ON_Interval m_trim_domain;  // corresponding domain of the trim 

  void SetTrimDomainFromEdgeDomain();

private:
  void ClearEvalCacheHelper();

  // parameter evaluation cache
  double m_t;
  double m_edge_t;
  double m_trim_t;
  double m_srf_uv[2];
  int m_trim_hint;
  int m_edge_hint;

  // surface evaluation cache
  int m_evsrf_hint[2];
  double m_evsrf_uv[2];
  ON_3dPoint m_evsrf_pt;
  ON_3dVector m_evsrf_du;
  ON_3dVector m_evsrf_dv;
  ON_3dVector m_evsrf_duu;
  ON_3dVector m_evsrf_duv;
  ON_3dVector m_evsrf_dvv;
  ON_3dVector m_evsrf_tan;

  void InitHelper( bool bInitProxyCurve );

  void CopyHelper(const CRhinoPolyEdgeSegment&);
};

