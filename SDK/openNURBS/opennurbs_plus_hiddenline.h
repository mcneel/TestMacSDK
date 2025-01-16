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

#if !defined(OPENNURBS_PLUS_HIDDENLINE_INC_)
#define OPENNURBS_PLUS_HIDDENLINE_INC_

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class ON_HLDFullCurve*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<const class ON_HLDFullCurve*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class ON_HLDCurve*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<const class  ON_HLDCurve*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class  ON_HLDPoint*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<const class  ON_HLDPoint*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray< const class ON_HLD_Object*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray < ON__UINT_PTR > ;
#endif

class  ON_CLASS ON_HiddenLineDrawing
{
public:
	//  Step I. Construct an empty hidden line drawing
	//		Absolute tolerance is used to decide if curves
	//    overlap or intersect.  
	//    A suggested value is to use 
	//       CRhinoDoc::AbsoluteTolerance
	ON_HiddenLineDrawing(double absoluteTolerance);
	ON_HiddenLineDrawing();
	~ON_HiddenLineDrawing();

	ON_HiddenLineDrawing(const ON_HiddenLineDrawing&);
	ON_HiddenLineDrawing& operator=(const ON_HiddenLineDrawing&);

	bool SetAbsoluteTolerance(double absolute_tolerance);
	double AbsoluteTolerance() const;
	bool IsValid(ON_TextLog* text_log = nullptr) const;
	void Dump(ON_TextLog&) const;

	// Step II.  Specify viewport
	/*
	Description:
		Set the viewport for the hidden line drawing.  	The viewport supplies the projection that determines
		the visibility of curves, and the HiddenLineDrawing coordinate system in which the resulting
		ON_HLDCurve's are represented.   The HLD-coordinate system is a right handed system
		for 3-d model space, in which the 3rd coordinate direction is the projection
		direction.  In particular the z-coordinate direction points from the scene toward the camera.
	Parameters:
		v	-[in]	viewport.  A copy of the viewport is made inside of ON_HiddenLineDrawing.
	Returns:
		true if viewport has been set
	Details:
		Only allowed before calling Draw(..).

		For perspective views the "target depth" is used to determine the scale of the results.
		The target_depth, v.TargetDistance(false), defines the target plane that is the plane
		perpendicular to the camera axis at this distance from the camera location.
		The HLD-Coordinate system is scaled 1:1 to world coordinates on the target plane.
		The HLD_coordinate system is translated in the z-direction so that the target plane
		has z=0 in HLD-coordinates.
		If you have a viewport and don't want to think about the choice of target_depth
		you might want to try:
			double near = v.FrustumNear();
			double far = v.FrustumFar();
			double target_distance = .5*(near + far);
			bool rc = v.SetFrustumNearFar( near, far, 0.0, 0.0, target_distance);
			if( rc)
				HLD.SetViewport(v);

		Another option is to leave the target_depth unset, or more explicitly call
		  v.SetTargetPoint( ON_UNSET_POINT)
		When the target point is unset, a target_depth will be computed from the geometry and used to define
		the HLD coordinate system.  

	Perspective distortion ( on perspective views only) will scale objects in front (back) of the target plane
	to be larger (smaller) in HLD-coordinates than in world coordinates.  The absolute tolerance is applied in
	HLD-coordinates so its effect depends on the target_depth setting.

	If v is a parallel projection viewport the HLD-coordinate system is the camera coordinate system of v.

	For perspective views the transformation from world coordinates to HLD-coordinates is
	a diagonal scaling of the view transformation from world to clip coordinates.
	Specifically
															[ W/2                     ]
	World_to_HLD-coordinates =	[			H/2                 ]  x World_to_clip_coordinates
															[					(f - n)/2    d  ]
															[											1.0 ]

	where W and H are the width and depth of the view frustum at the target_depth and f and  n are the
	distances to the far and near clipping plane in the viewport, d is chosen so that the target plane
	is represented as the plane z=0 in HLD-coordinates.
	*/
	bool SetViewport(const ON_Viewport& v);

	/*
	Description:
		SetParallelView will set the viewport to a parallel projection.  
	Parameters:
		CameraDirection -[in] Camera direction ( pointing toward the scene) in world coordinates.
		CameraUp -[in]        Direction, in world coordinates,  that corresponds to increasing y-direction 
													in the hidden line drawing.
	Returns:
	  true if the viewport has been set.  false if the vectors are linearly dependent.
	*/
	bool SetParallelViewport(ON_3dVector CameraDirection, ON_3dVector CameraUp);
	const ON_Viewport& Viewport() const;

	// Step 3. Optionally, Specify clipping planes or other options.
	/*
	Description:
		Specify clipping planes that are active for this view.
	Parameters:
		clip -[in] clipping plane or planes 
		clip_id -[in]   This is an optional parameter the user can use to identify the clipping plane.
	Returns:
		true if clipping planes have been set.
	Remarks:
		A point x in model space is visible if p(x)<=0 for each clipping plane p.

		Only allowed before calling Draw.
		The clipping planes are append to the ON_HiddenLineDrawing::m_clipping_plane array.
		For perspective views SetViewport() will add PerspectiveViewClipCount many clipping planes from the viewport,
		and the the first clipping plane is the near plane of the view frustum.
	*/

	bool AddClippingPlanes(const ON_SimpleArray<ON_PlaneEquation>& clip, 
												 const ON_SimpleArray<ON__UINT_PTR>* clip_id = nullptr );
	bool AddClippingPlanes(const ON_PlaneEquation*  clip, const  ON__UINT_PTR* clip_id, size_t count);
	bool AddClippingPlane(ON_PlaneEquation clip, ON__UINT_PTR clip_id = 0 );

  static const int PerspectiveViewClipCount = 5;
	/*
	Description:
		Get the array of clipping planes used in this hiddenline drawing,  includes both user specified 
		planes and frustum clipping planes for perspective views. 
	Parameters:
		clip_id  -[out] Optional parameter to get array of clipping plane ids.
	Returns:
		The array of clipping planes.
	Remarks:
		Frustum clipping planes, if they are present, are in positions 0 to 5 and have id==0.
		To get the associated clip
	*/
	const ON_SimpleArray< ON_PlaneEquation >& GetClippingPlanes() const;
	const ON_SimpleArray< ON_PlaneEquation >& GetClippingPlanes(ON_SimpleArray<ON__UINT_PTR>* clip_id ) const;

  /*
  	Description:
        Enable Occluding Sections option.
    Parameters:
      bEnable - true enables Occluding Sections option, false disables
  */
  void EnableOccludingSection(bool bEnable = true);

  /*
      Returns true iff the OccludingSectionOption is enabled.
  */
  bool OccludingSectionOption() const;

	/*
	Include tangent edges in hidden line drawing
	Parameters:
		include -[in]  true to include tangent edges
	Note:
		By default tangent edges are included.  To exclude tangent
		edges use  IncludeTangentEdges( false )
	*/
	void IncludeTangentEdges(bool include);

	/*
	Include tangent seams in hidden line drawing
	Parameters:
		include -[in]  true (default )to include tangent seams
	Returns:
		false if tangent seams are not included
	Note:
		This option is only valid if tangent edges are included.
		By default seams are not included.  To include seams
		use IncludeTangentSeams( true );
	*/
	bool IncludeTangentSeams(bool include);

	/*
	Include hidden curves in hidden line drawing
	Parameters:
	include -[in]  true (default )to include hidden curves
	Returns:
	false if hidden curves are not included
	Note:
	  Hidden curves are included by default.
		When hidden curves are not included curves can 
		still be marked with visibility of hidden.

		When hidden curves are included, all hidden curves are 
		included unless they are duplicates of other hidden or 
		visible curves.  

		If hidden curves are not needed in the output 
		IncludeHiddenCurves(false) will improve performance.
	*/
	bool IncludeHiddenCurves(bool include);


	/*
	Description:
		Set the HiddenLineDrawing context from another HiddenLine Drawing
	Parameters:
		Source - (in)		Source of context information
	Details:
		copies, tolerance, view, clipping planes, tangent edges and seams options from source.
		This should be used on an empty drawing only.
	*/
	void SetContext(const ON_HiddenLineDrawing& source);

	// Step IV - Add Objects that are to be drawn.  
	/*	Each call to AddObjects with valid geometry will add a corresponding ON_HLD_Object to the m_object array.
			Results in the ON_HiddenLineDrawing  will then refer back to these ON_HLD_Object as source objects.

	Parameters:
		geom - [in]  geometry to be drawn.  returns -1 if geometry type is not supported by ON_HiddenLineDrawing.
		xform - [in] If not null this transformation is applied to geom to place it in the world coordinate system.
									The transform *xform is copied.
		uuid -[in]   This parameter is copied into the ON_HLD_Object.
		m_id -[in]   This parameter is copied into the ON_HLD_Object.
	Returns:
		Index into m_object array of the corresponding ON_HLD_Object, or -1 if the geometry type is not supported.
	Details:
		The only supported geometry types are ON_Brep, ON_Curve, ON_Mesh, ON_Point and  ON_PointCloud.
		The uuid and m_id are fields for the
		user to cross-reference the geometry to other information in their application.
		When using the simple geom pointer it is the users responsibility to ensure the geometry exists for the lifetime
		of the ON_HiddenLineDrawing.  Alternatively, the shr_geom_ptr parameter is a "smart pointer" that will ensure
		geometry persists until the  ON_HiddenLineDrawing is destroyed.
	*/
	int AddObject(const ON_Geometry* geom, const ON_Xform* xform, ON_UUID uuid, ON__UINT_PTR m_id);
	int AddObject(std::shared_ptr<const ON_Geometry> shr_geom_ptr, const ON_Xform* xform, ON_UUID uuid, ON__UINT_PTR m_id);

	// m_object is the array of objects that that have been added and will be used in the Draw() operation.
	const ON_SimpleArray<const class ON_HLD_Object*>&	m_object;

  /*
  Description:
      Enable/Disable the Selective Clipping option for an object
  Parameters:
    obj   - an object that been added to the drawing
    active_clip_ids - list ( possibly empty) of active clip_ids for this object.
  Returns
    true if the the list is a subset of those added with AddClippingPlane methods
*/
  bool EnableSelectiveClipping(class ON_HLD_Object&  obj, const ON_SimpleArray< ON__UINT_PTR>& active_clip_ids);

  void DisableSelectiveClipping(class ON_HLD_Object& obj);

  /*
  Parameters:
    obj   - an object that been added to the drawing
  Returns :
    true if the Selective Clipping Option is enabled.
  */
  bool SelectiveClippingOption(const class ON_HLD_Object& obj ) const;

  /*
   Returns:
       Array of active clipping planes for this object.
       If Selective Clipping option is enabled for this object
       then the array returned may be a proper subset of GetClippingPlanes. 
 */
  ON_SimpleArray< ON__UINT_PTR> ActiveClipIds(const class ON_HLD_Object& obj ) const;


	// Step V.  Draw objects  hidden line drawings
	/*
	Description:
		Makes a hidden line drawing for the the objects in m_obj
	Parameters:
		bAllowUseMP -[in]  If true multiprocessors may be used to speed up the calculation.
		progress - [in]		If not null this class can be used to see how much progress has been made while the computing.
		terminator  - [in] Optional object to allow caller to terminate computation.
	Returns:
		true when drawing completes without error
	Remarks:
		If this ON_HiddenLineDrawing already contains a drawing of objects  the result includes a composite drawing of
		old and new objects.  However for best performance add all the objects then call Draw only once.
	*/
	bool Draw(bool bAllowUseMP,
		ON_ProgressReporter* progress,
		ON_Terminator* terminator);

	// Return number of objects present when the last Draw() operation was performed.
	int NumberDrawn() const;


	/*
	Description:
		If OtherHLD and *this are hidden line drawings of different objects with the same context they can be be merged
		into a single hidden line drawing.
	Parameters:
		OtherHLD - [in]
		terminator -[in]  Optional object to allow caller to terminate computation.
	Returns:
		true if *this is the merger of the two input hidden line drawings.
	Remarks:
		Hidden line drawings may be merged only if they have the same tolerances, viewports, clipping planes and
		options.  When true is returned Other is left as an empty hidden line drawing.
	*/
	bool Merge(ON_HiddenLineDrawing& OtherHLD, ON_Terminator* terminator);

	// Step VI.  After Draw() the results are available as arrays of ON_HLDFullCurve, ON_HLDCurve and ON_HLDPoint.
	// m_curve, m_full_curve and GetHLDPoints() are the results of the draw operation.
	const ON_SimpleArray<  const class ON_HLDFullCurve*>& m_full_curve;
	const ON_SimpleArray<  const class ON_HLDCurve*>&			m_curve;
	const ON_SimpleArray< const class ON_HLDPoint*>& GetHLDPoints();

	// Flatten()  projects all m_full_curve to the x-y plane in HLD-coordinates.
	// Since HLD_Curves are ProxyCurves with references in the m_full_curve array these are also flattened. 
	bool Flatten();
	bool HasBeenFlattened() const;

	// Get tight bounding box of the hidden line drawing
	ON_BoundingBox GetBoundingBox(bool include_hidden) const;

	// The ON_HiddenLineDrawing HLD-coordinate system is a right handed system in which the 3rd coordinate is the projection
	// direction.  In particular the z coordinate direction points from the sceene to the camera.

	const ON_Xform& World2HiddenLine() const;			// World to HLD-coordinate System.  
	const ON_Xform& HiddenLine2World() const;			// HLD to World coordinate System.  
	ON_3dVector CameraDirection(ON_3dPoint wp)const;	// world camera direction ( increasing toward scene) at a world point
	ON_3dVector CamLocDir() const;								//returns camera location, if perspective, or else camera direction.


	bool IsPerspective() const;

  void RejoinCompatibleVisible();   // Rejoin consecutive visible curves of a fullcurve.

	// Call EmergencyDestroy() if the memory used by the results of the any Draw operations, 
	// in particular any of the contents of m_full_curve and m_curve, has become invalid.
	void EmergencyDestroy();
private:
	class ON_HiddenLineDrawingImplBase* pImpl;
	ON_SimpleArray< class ON_HLDFullCurve*>	m_FullCrv;		// These are base curves that may be part of the solution. 																						
	ON_SimpleArray< class ON_HLDCurve*>			m_Crv;						// These curves are referenced by the HLD_Curves in m_Curve
	ON_SimpleArray< const ON_HLD_Object*>		m_Obj;
	friend ON_HLDFullCurve;
};

class ON_CLASS ON_HLD_Object
{
public:
	// These user specified identifiers are available for the caller to identify the source of objects 
	// in the hidden line drawing.  These fields are not used by the ON_HiddenLineDrawing class.  
	ON_UUID m_obj_UUID = ON_nil_uuid;
	ON__UINT_PTR m_obj_id = 0;

	// Geometry() is in world coordinates if UseXform() is false, otherwise geometry is in object space 
	//  coordinates (see GetXform() below).
	virtual	const ON_Geometry* Geometry() const = 0;
	virtual void DestroyRuntimeCache(bool bDelete) = 0;

	// Description
	//	Returns true if there is an xform that must be applied to transform geom 
	//  into the world coordinate system
	bool UseXform() const;

	// Description
	//	GetXform returns the mapping from object space 
	//  to world coordinates. If UseXform() is false then 
	//  GetXform() returns the identity transform.
	const ON_Xform& GetXform() const;

/*
  Description:
      Enable Occluding Sections option for this object
  Parameters:
    bEnable - true enables Occluding Sections option, false disables
*/
  void EnableOccludingSection(bool bEnable = true);

  /*
      Returns true iff the OccludingSectionOption is enabled.
  */
  bool OccludingSectionOption() const;

  bool SelectiveClippingOption() const;

	ON_HLD_Object& operator=(const ON_HLD_Object&) = default;
protected:
	ON_HLD_Object() = default;
	ON_HLD_Object(const ON_HLD_Object&) = delete;


	virtual ~ON_HLD_Object() {};

	void* m_Reserved = nullptr;

	ON_Xform m_xform;			// the xform is applied to the geometry, *geom, before it is located in the 3d world coordinates of the scene 
	bool m_bUseXform = false;	// if true use xform.  This allows a simple test instead always testing for th

  // This function is only called by  This function is only called by ON_HiddenLineDrawingImplBase::SetSelectiveClipping() 
  void EnableSelectiveClipping(bool bEnable);

  bool m_bOccludingSections = false;						// If true clip sections occlude
  bool m_bSelectiveClipping = false;					// If true clip sections o

  ON_SimpleArray<bool> m_Active_Clip;           // When Selective Clipping is enabled Active_Clip[cpi] 
                                              // refers to m_Clip[cpi]
  friend class ON_HiddenLineDrawingImplBase;
  friend class TL_HiddenLineDrawingImpl;
};



// A hidden line drawing consists of curves generated from source objects in the m_object array. 
// The curves correspond to edges, and silhouettes of  source objects and intersections with cutting planes.
// These curves are represented as ON_HLDFullCurve's.
// An ON_HLDFullCurve is partitioned into hidden  and visible segments called TL_HLDCurves.  
class ON_CLASS ON_HLDFullCurve
{
public:
	void Dump(ON_TextLog& log) const;
	bool IsValid(const ON_HiddenLineDrawing& hld, ON_TextLog* = nullptr) const;

	//  FullCurve() is a curve in HLD-coordinates. So that we can freely apply the 
	//  World2HLD transformation and its inverse the curve is a nurbs curve.
	//  If this ON_HLDFullCurve IsProjecting() and the containing ON_HiddenLineDrawing 
	//  HasBeenFlattened then this curve will not be valid, since flattening projects 
	//  this curve to a point.  i.e Make sure to check for nullptr.
	const ON_NurbsCurve* FullCurve() const;
	ON_NurbsCurve* FullCurve();
	void SetFullCurve(ON_NurbsCurve*);

	//  If  m_type sil_tangent *m_Pcrv may be the surface parameter space curve of the silhouette.
	//  In other cases it is nullptr.
	const ON_Curve* PSpaceCurve() const;
	ON_Curve* PSpaceCurve();
	void SetPSpaceCurve(ON_Curve*);

	//  pointer into source object in ON_HiddenLineDrawing::m_Object.
	const ON_HLD_Object* 	 m_SourceObject;

	// component of source object part that generated this curve.
	ON_COMPONENT_INDEX	m_CompInd;

	// index into  ON_HiddenLineDrawing::m_clipping_plane when m_type=section cut.
	int m_clipping_plane_index;

	int m_fci;								// index of this FullCurve in ON_HiddenLineDrawing::m_full_curve. 

	ON_SIL_EVENT::TYPE m_type;

	// ON_HLDCurve containing parameter t 
	// side - optional - determines which side to return at breakpoints
	//         0 = default
	//      <  0  curve that contains an interval [ t-, t ] , for some t- < t.
	//      >  0 curve that contains an interval  [ t, t+ ], for some t+ > t.
	ON_HLDCurve* Curve(double t, int side = 0);
	const ON_HLDCurve* Curve(double t, int side = 0)const;

	// Initialized to ON_UNSET_VALUE.  If the FullCurve is a closed curve.
	// Rejoin can reparmeterize the curve by moving the seam.  
	// When this has been done the original domain start is stored here.
	// So to compute the source curve parameter from the ON_HLDFullCurve parameter
	//		double Source_t = Curve_t;
	//    if( m_OriginalDomainStart != ON_UNSET_VALUE)
	//			Source_t += m_OriginalDomainStart - Crv->Domain[0];
	double m_OriginalDomainStart;

	// Increasing,  partition of m_Crv->Domain()== ( m_t[0], *m_t.Last() )
	ON_SimpleArray<double> m_t;

	// HLD_Curves that make up this full curve.
	// C.Count() = t.Count()-1
	// C[i]->Domain() = [ t[i], t[i+1] ]
	ON_SimpleArray<ON_HLDCurve*> m_C;

	// returns true if all the non clipped portions of this curve are projecting.	
	// That is each curve segment projects to a point in the 2d hidden line drawing.
	bool IsProjecting() const;

	// returns true if the curve is a BrepEdge of the source geometry.
	bool IsEdgeCurve() const; 
private:
	ON_HLDFullCurve();				// makes an uninitialized ON_HLDFullCurve
	~ON_HLDFullCurve();
	void Initialize();

	bool IsValid(const class ON_HiddenLineDrawingImplBase& hld, ON_TextLog* = nullptr) const;

	// These makes an partially initialized ON_HLDFullCurve
	ON_HLDFullCurve(const ON_HLDFullCurve&);
	ON_HLDFullCurve& operator=(const ON_HLDFullCurve&);			// makes an partially initialized ON_HLDFullCurve

	ON_NurbsCurve* m_Crv;
	ON_Curve* m_Pcrv;
	int NearEnd(double s, double tol);

	void* m_Reserved = nullptr;				

	friend class ON_HiddenLineDrawingImplBase;
	friend class TL_HiddenLineDrawingImpl;
	friend ON_HLDCurve;
	friend class TL_HLDBrepObj;
};

/*
The results of HiddenLineDrawing calculation are a collection of TL_HLDCurves.  An ON_HLDCurve is a
subcurve of an ON_HLDFullCurve, represented as an ON_CurveProxy of the containing ON_HLDFullCurve.
*/
class  ON_CLASS ON_HLDCurve : public ON_CurveProxy
{
public:
	void Dump(ON_TextLog&) const; // for debugging

	// Index of this curve in ON_HiddenLineDrawing::m_curve
	int m_ci = -1;

	// This curve is a subcurve of *FullCurve().
	const ON_HLDFullCurve* FullCurve() const;
	ON_HLDFullCurve* FullCurve();

	enum class VISIBILITY : unsigned int
	{
		kUnset = 0,
		kVisible = 1,
		kHidden = 2,
		kDuplicate = 3,
		kProjecting = 4,			// projects to a point ( smaller than tol )
		/* Deprecated : kClipped is not actually generated by HiddenLineDrawings and should not be used*/
		kClipped = 5					// clipped by clipping planes
	};

	VISIBILITY m_vis = VISIBILITY::kUnset;

	bool IsSceneSilhouette() const;		// Is this Curve a scene silhouette.

  // True if this curve on the boundary of the visible region, as defined by the clipping planes.
	bool IsOnSectionCut() const;

	// The m_SilSide fields are only valid for visible curves.
	// With respect to the make2d image the region to the left ( or right respectively) of this curve is
	// described by m_SilSide[0] ( or m_SideSil[1] respectively).  If exactly one of these regions is empty 
	// this is a scene silhouette.  If this region contains a surface it is either a surface which in 3d is adjacent 
	// to this edge or it an surface that is further away from the camera, we call this a shadow surface.  
	// unknown is used for unset values and for projecting curves
	ON_SIL_EVENT::SIDE_FILL m_SilSide[2];

private:
	ON_HLDCurve(ON_HLDFullCurve& C);
	ON_HLDCurve(ON_HLDFullCurve& C, ON_Interval dom);
	ON_HLDCurve(const ON_HLDCurve&) = delete;
	void operator=(const ON_HLDCurve&) = delete;
	~ON_HLDCurve() = default;

	ON_HLDFullCurve* m_FullCrv;					// the containing ON_HLDFullCurve

	/* Merge SideFill data into m_SilSide[side_ind] */
	void MergeSilSideData(int side_ind, ON_SIL_EVENT::SIDE_FILL);
	int m_temp = 0;				// this is a temporary variable that is used only during HiddenLine Drawing construction. 

	void * m_Reserved; 

	friend class ON_HiddenLineDrawingImplBase;
	friend class TL_HiddenLineDrawingImpl;
	friend class TL_HLDBrepObj;
	friend class ON_HLDFullCurve;
};

/*
The results of HiddenLineDrawing calculation are a collection of TL_HLDCurves and ON_HLDPoints.  An ON_HLDPoint 
is a point in HLDCoordinates, that is generated by a ON_Point Object or an ON_Point_Cloud.
*/
class  ON_CLASS ON_HLDPoint : public ON_3dPoint
{
public:
	void Dump(ON_TextLog&) const; //  for debugging

	//  pointer into source object in ON_HiddenLineDrawing::m_Object.
	const ON_HLD_Object* 	 m_SourceObject = nullptr;

	// component of source object part that generated this curve.
	ON_COMPONENT_INDEX	m_CompInd = ON_COMPONENT_INDEX::UnsetComponentIndex;

	// True if this curve on the boundary of the visible region, as defined by the clipping planes.
	bool IsOnSectionCut() const;

	// If point is on a section_cut then m_clipping_plane_index>=0 is the index into
	// ON_HiddenLineDrawing::m_clipping_plane otherwise m_clipping_plane_index=-1
	int m_clipping_plane_index = -1;

	// Index of this point  in ON_HiddenLineDrawing::m_Pnt
	int m_pi = -1;

	// The values of this type are a subset of values of ON_HLDCurve::VISIBILITY
	enum class VISIBILITY : unsigned int
	{
		kUnset = 0,
		kVisible = 1,
		kHidden = 2,
		kDuplicate = 3,
	};

	VISIBILITY m_vis = VISIBILITY::kUnset;

private:
	ON_HLDPoint(const ON_3dPoint& p) : ON_3dPoint(p) {}
	ON_HLDPoint(const ON_HLDPoint&) = delete;
	void operator=(const ON_HLDPoint&) = delete;
	~ON_HLDPoint() = default;

  ON__UINT_PTR m_reserved = 0;

	friend class TL_HLDPointObj;
	friend class TL_HLDPointCloudObj;
	friend class TL_HiddenLineDrawingImpl;
};

#endif
