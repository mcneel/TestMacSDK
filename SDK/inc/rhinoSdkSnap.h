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

class CRhSnapEventExtension;

class RHINO_SDK_CLASS CRhinoSnapEvent
{
public:
  CRhinoSnapEvent();
  CRhinoSnapEvent(const CRhinoSnapEvent&);
  CRhinoSnapEvent& operator=(const CRhinoSnapEvent&);
  ~CRhinoSnapEvent();

  // the >, <, ==, != operators compare mode, depth and distance
  // and are used to determine which snaps take precedence
  BOOL32 operator>(const CRhinoSnapEvent&) const;
  BOOL32 operator<(const CRhinoSnapEvent&) const;
  BOOL32 operator==(const CRhinoSnapEvent&) const;
  BOOL32 operator!=(const CRhinoSnapEvent&) const;

  // Sets *this to src and returns true if src precedes this (src > *this). Also updates list of secondary snap modes with an optional xform needed when snapping to blocks or model space objects in layout space.
  bool CompareAndUpdate( const CRhinoSnapEvent& src, const ON_Xform* pXform = 0);

  // Returns secondary osnap modes if other osnaps were active at the same location as this event
  unsigned int SecondarySnapModes() const;

  // object and parameter corresponding to snap_point are in m_objref
  CRhinoObjRef m_objref;
  // Second object if snap mode is CRhinoAppSettings::os_intersection and two objects are involved in the snap event.
  // Set also when object-object snapping and snap mode is CRhinoAppSettings::os_tangent or CRhinoAppSettings::os_perpendicular.
  CRhinoObjRef m_second_objref;

  // type of snap
  CRhinoAppSettings::osnap_mode m_snap_mode;

  // The near point is the point on the curve nearest to
  // the mouse.  This point determines the snap depth
  // and snap distance.
  ON_3dPoint m_near_point;
  double m_near_t;
  
  // depth of near point.
  // CRhinoSnapContext::GetSnapDepth(m_near_point,..) sets m_near_depth.
  double m_near_depth;
  
  // shortest distance from snap ray to near point
  // CRhinoSnapContext::GetSnapDepth(m_near_point,..) sets m_near_distance.
  double m_near_distance;

  // depth of snap point.
  // CRhinoSnapContext::GetSnapDepth(m_snap___point,..) sets m_snap_depth.
  double m_snap_depth;

  // shortest distance from snap ray to snap point
  // CRhinoSnapContext::GetSnapDepth(m_snap___point,..) sets m_snap_distance.
  double m_snap_distance;

  // location of snap point.  When a snap point is an
  // end, midpoint, center, etc., the snap point may differ
  // from the near point.
  ON_3dPoint m_snap___point; // same as m_near_t for near snaps
  double m_snap_t; // same as m_near_t for center and near snaps

  // flag to see if an inferred line was involved in this snap event
  bool m_bInferred;
  
  // tells which smart point the snap event belongs to if any.
  int m_smart_point_index;
  // the inferred line type. 0=notset 1=ortho 2=tan 4=perp 8=tan_parallel 16=perp_parallel
  // can be a combination of the types
  int m_type; 

  // index that points to a snap_point in case one was hit. -1 if not set.
  int m_snap_point_index;
  
  bool m_bConstructionPointSnap;

private:
  friend class CRhSnapEventExtension;
  CRhSnapEventExtension* m_ex;
};

class RHINO_SDK_CLASS CRhinoIntSnapCandidate
{
public:
  CRhinoIntSnapCandidate();
  CRhinoSnapEvent m_event;

  // The portion of the curve that will be used to look for
  // intersection snaps is either a bezier segment in m_cd
  // or m_bez.  Use the BezierCurve() function to get the
  // right one.
  const class CRhCurveDisplay* m_cd;  // Curve display bezier list to intersect
  int m_cd_seg_index;  // seed index for finding the real intersection
  ON_BezierCurve m_bez; // Use for "phantom" smart-tracking lines

  /*
  Description:
    Use BezierCurve() to get the bezier to intersect.
  */
  const ON_BezierCurve& BezierCurve() const;

  // This transform maps the bezier into world coordinates.
  // For "oridinary" objects it is the identity.
  // For instance references, the bezier is instance definition
  // geometry and m_b2w maps it to the referenced location
  // in world space.
  ON_Xform m_b2w;

  // This transforms the bezier into snapping coordinates.
  ON_Xform m_near_snap_xform;

  // 9-22-03 Lowell Added flag so active curves can be recognized and 
  // excluded from overlap cases in int osnap testing
  enum eCase 
  {
    ccNormal = 0,
    ccActive,
    ccInferred,
  };
  int m_case;

  // 2 Oct 2003, Mikko added a setting that tells which smart point the candidate belongs to if any.
  int m_smart_point_index;
  int m_parallel_point_index; // index to possible associated smart point parallel lines come from. <0 if not set

  // 4 Oct 2003, Mikko added a setting that tells the inferred line type. 0=notset 1=ortho 2=tan 4=perp 8=tan_parallel 16=perp_parallel
  // can be a combination of the types
  int m_type;

  // 7 Oct 2003, Mikko added near point tangent vector.
  // If 2 lines overlap and third line intersects them, it's hard to snap to the intersection
  // because Rhino tends to find the overlapping lines as the 2 closest candidates.
  // Adding a tangent line to help sort out those cases.
  ON_Line m_tanline;

  // 18 Sept 2014, Mikko:
  // If true osnap will only snap to this candidate if the snap location is visible and not covered by shaded objects.
  bool m_bCheckVisibility; 
};

// 9-17-03 LW added in conjunction with inferred curve osnap testing
class RHINO_SDK_CLASS CRhinoInferredCurve : public ON_BezierCurve
{
public:
  CRhinoInferredCurve();
  ~CRhinoInferredCurve();
  CRhinoInferredCurve( int dim, BOOL32 israt, int order);
  CRhinoInferredCurve( const CRhinoInferredCurve& src);
  CRhinoInferredCurve& operator=( const CRhinoInferredCurve& src);
  BOOL32 Create( int dim, int israt, int order);
  CRhinoAppSettings::osnap_mode SnapTo( const CRhinoSnapContext& snap_context, CRhinoSnapEvent& snap_event) const;

  enum eCurveStyle
  {
    csSegment,    // use defined segment
    csInfinite,   // extend to edge of viewport each way
    csRay,        // extend to edge of viewport past last end
  };
  eCurveStyle m_style;

  int m_smart_point_index; // Index to possible associated smart point. <0 if not set.
  int m_parallel_point_index; // index to possible associated smart point parallel lines come from. <0 if not set

  // The type of inferred curve. 0=notset 1=ortho, 2=tan 4=perp, 8=tan_parallel 16=perp_parallel, 32=guide, 64=getpoint curve constraint
  // can be a combination of the types
  int m_type; 

  // 9-22-03 Lowell added flag
  bool m_bActive;  // curve currently being drawn
};

class RHINO_SDK_CLASS CRhinoInferredCurveArray: public ON_ClassArray<CRhinoInferredCurve>
{
public:
  CRhinoInferredCurveArray();
  ~CRhinoInferredCurveArray();

  // A special version of the array appender that checks if 
  // a line you are trying to add overlaps a line already on the array.
  // That way Int snapping will work right because parallel overlapping
  // lines will never end up being the ones that get tested for intersection.
  void Append( const CRhinoInferredCurve& c );
};

class CRhinoView;

class RHINO_SDK_CLASS CRhinoSnapContext
{
public:
  CRhinoSnapContext();
  ~CRhinoSnapContext();

  // C++ default operator= and copy constructor work
  // fine.  Do not override.
  //CRhinoSnapContext& operator=(const CRhinoSnapContext& src);
  //CRhinoSnapContext(const CRhinoSnapContext& src);

  /*
  Description:
    Get transformation that maps snapping frustum to clipping box.
    This transformation is stored in m_snap_xform and must be set
    before any of the SnapTo...() functions can be called.
  Parameters:
    view - [in] view where snap is happening
    screen_x - [in] view window x coordinate (0 = left side)
    screen_y - [in] view window y coordinate (0 = top side)
    near_snap_radius - [in] radius of snapping box in pixels
    point_snap_radius - [in] radius of snapping box in pixels
  Returns:
    TRUE if m_snap_xform is successfully set.
  */
  bool SetSnapXform( 
    const CRhinoView* view, 
    double screen_x, 
    double screen_y,
    int near_snap_radius = RhinoOsnapPickboxRadius(),
    int point_snap_radius = RhinoOsnapPickboxRadius() 
    );


  /*
  Description:
    When the an object's location is transformed before it is displayed,
    this function can be used to modify a the snap context's transforms
    and other picking information.
  Parameters:
    object_to_world - [in] 
      transformation that maps object into world coordinates
    world_to_object - [in] 
      inverse of object_to_world    
  Example:
         SnapTo( .., const CRhinoSnapContext& snap_context, ... )
         {
           CRhinoSnapContext object_sc = snap_context;
           object_sc.Transform( object_to_world, world_to_object );
           // now use object_sc for snapping calculation
           ...
         }
  */
  bool TransformSnapContext( const ON_Xform& object_to_world, const ON_Xform& world_to_object );

  ON_UUID m_snap_viewport_id; // id of viewport being snapped in
  const CRhinoView* m_view; // if not NULL, defines cplane for quad snaps
  ON_Plane m_cplane; // construction plane of the view. Used for quad osnap
  ON_2dPoint m_screen_point; // in m_view window coordinates
  ON_Xform m_w2s; // world to screen transformation
  unsigned int m_object_filter; // bits set from ON::object_type enum
  ON_Xform m_near_snap_xform;
  ON_Xform m_point_snap_xform;
  ON_Xform m_object_to_world;
  ON_Line m_snap_line; // mouse pick line from near to far

  // m_snap_region.m_xform = n_near_snap_xform.
  ON_ClippingRegion m_snap_region;

  // If m_bSetClippingPlanes is true (the default), then 
  // CRhinoSnapContext will use lazy evaluation to automatically
  // set m_snap_region.m_clip_planes[] when m_view is set.
  // If m_bSetClippingPlanes is false, then you have to set
  // m_snap_region.m_clip_planes[] if they are required.
  bool m_bSetClippingPlanes;

  /*
  Description:
    Updates the clipping plane information in m_snap_region.
    The m_bSetClippingPlanes and m_view fields must be 
    before calling UpdateClippingPlanes().
   Parameters:
     pViewport - [in] When set pViepwort is used to force update the clipping planes.
                      Otherwise active viewport is used.
  */
  void UpdateClippingPlanes( const CRhinoViewport* pViewport = 0);

  /*
  Description:
    Updates the clipping plane information in m_snap_region.
    The m_bSetClippingPlanes and m_view fields must be
    before calling UpdateClippingPlanes().
   Parameters:
     pViewport - [in] When set pViepwort is used to force update the clipping planes.
                      Otherwise active viewport is used.
     filter - [in] optional list of UUIDs to pay attention to when
      updating clipping planes. Only planes with ids in the filter
      list will be used if filter is not null and contains entries
  */
  void UpdateClippingPlanes(const CRhinoViewport* pViewport, const ON_UuidList* filter);


  const CRhinoObject* m_do_not_snap_to_me; // do not snap to this object

  // check for snaps in the following order
  unsigned int m_snap_mode; // CRhinoAppSettings::osnap_mode bits
  bool m_bSnapToCurves; // passed flag from CRhinoGetPoint
  
  // m_basepoint is typically the point provided at the
  // GetPoint() "From" prompt and is used for
  // tan and perp snapping.
  ON_3dPoint m_basepoint;


  // To snap to objects in page views set 
  // m_object_space = ON::page_space.
  // Default is to snap to objects in model views.
  ON::active_space m_object_space;

  /*
   Description:
     Utility for getting snap point depth and distance
   Parameters:
     bez - [in]
     snap_depth - [out] relative depth (world units)
     snap_distance - [out] relative distance (normalized units)
   Returns:
     TRUE if a near snap point is found.
  */
  void GetSnapDepth( const ON_3dPoint& snap_point,
                double* snap_depth,
                double* snap_distance
                ) const;

  /*
  Parameters:
    bCheckVisibility - If true only locations that are visible are snapped to.
  Returns:
     TRUE if point is in snap region
  */
  bool SnapToPoint( const ON_3dPoint& point, bool bCheckVisibility = false ) const;

  /*
   Description:
     Point snapper that ignores clpping coordinate Z.
     Makes snapping work on temporary geometry that is
     outside near and far clipping planes.
   Parameters:
     bCheckVisibility - If true only locations that are visible are snapped to.
   Returns:
     TRUE if point is in snap region
  */
  bool SnapToPointNoZ(const ON_3dPoint& point, bool bCheckVisibility = false ) const;

  /*
   Returns:
     TRUE if bbox intersects the snap region
  */
  bool SnapToBBox( const ON_BoundingBox& bbox ) const;

  /*
   Description:
     Utility for snapping to near point on bezier segments.
   Parameters:
     bez - [in]
     t - [out] parameter of near snap point if found
     bCheckVisibility - If true only locations that are visible are snapped to.
   Returns:
     TRUE if a near snap point is found.
  */
  bool NearSnapToBezier(
        const ON_BezierCurve& bez,
        double* t,
        bool bCheckVisibility = false
        ) const;

  bool NearSnapToLine(
        const ON_Line& line,
        double* t,
        bool bCheckVisibility = false
        ) const;

  /*
   Description:
     Utility for snapping to tan point on bezier segments.
   Parameters:
     bez - [in]
     point - [in] base point for tan snap
     seed_t - [in] start searching here
     t - [out] parameter of near snap point if found
   Returns:
     TRUE if a near snap point is found.
  */
  bool TanSnapToBezier(
        const ON_BezierCurve& bez,
        ON_3dPoint point,
        double seet_t,
        double* t
        ) const;

  /*
   Description:
     Utility for snapping to perp point on bezier segments.
   Parameters:
     bez - [in]
     point - [in] base point for tan snap
     seed_t - [in] start searching here
     t - [out] parameter of near snap point if found
   Returns:
     TRUE if a near snap point is found.
  */
  bool PerpSnapToBezier(
        const ON_BezierCurve& bez,
        ON_3dPoint point,
        double seet_t,
        double* t
        ) const;

  /*
   Description:
     Utility for snapping to quadrant point on bezier segments.
   Parameters:
     bez - [in]
     plane - [in] defines coordinate system for quad snapping
     seed_t - [in] start searching here
     t - [out] parameter of near snap point if found
   Returns:
     TRUE if a near snap point is found.
  */
  bool QuadSnapToBezier(
        const ON_BezierCurve& bez,
        const ON_Plane& plane,
        double seet_t,
        double* t
        ) const;

  // Snap to the end of a bezier if its within the pickbox
  bool TestOverlapSnap( const ON_BezierCurve* pBez, ON_3dPoint& pt);
  
  // Destroy the current active curve if there is one and create a new one
  // If there is an Active Curve and it matches the input dim, israt & order, reuse it
  bool CreateActiveCurve( int dim, int israt, int order);

  /*
  Description:
    Utility for snapping to intersection of bezier segments.
  Parameters:
    bApparentIntersections - [in] if true, the segments are 
        projected to a common plane before being intersected.
  Returns:
    True if an intersection is found. 
  */
  bool IntSnap( bool bApparentIntersections);

  // used to build a list of segments to test for intesection
  // snaps

  // true if eligible events should be added to the intersection
  // snap list.
  
  // volatile
  bool m_bIntSnapCheck;

  // volatile
  int m_int_snap_count;
  CRhinoIntSnapCandidate m_int_snap[2];
  ON_ClassArray<CRhinoIntSnapCandidate> m_int_snap_candidates;

  // 9-24-03 Lowell changed the way active curve is handled 
  // so it can be separate from smart-cursor inferred curves
  CRhinoInferredCurve m_active_curve;
  
  CRhinoInferredCurveArray m_inferred_curves;

  bool m_bUseSmartTracking;

  // the actual unmodified osnap mode that was passed to CRhinoDoc::SnapTo
  unsigned int m_input_snap_mode; // CRhinoAppSettings::osnap_mode bits

  // Set to true to make osnapping only snap to locations that are visible and not covered by shaded objects.
  // For example when snapping to blocks the low level bCheckVisibility parameter in SnapToPoint etc.
  // cannot be set, because the block members have no knowledge of the block selection status.
  // This check visibility setting makes it possible to override the parameter when necessary.
  void SetCheckVisibility(bool b) { m_bCheckVisibility = b; }
  bool m_bCheckVisibility;
};

// 12 April 2021, Mikko, RH-63759:
// Global clamp for max int snap candiadte count. For the last 15 or so years it was set to 1000, but it made block 
// snapping slow due to int snap candidate array duplication. Trying setting the clamp to 100.
int RhMaxIntSnapCandidateCount();
