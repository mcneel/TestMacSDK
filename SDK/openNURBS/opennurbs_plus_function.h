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

#if defined(OPENNURBS_PLUS) || defined(OPENNURBS_PLUS_INC_) || !defined(OPENNURBS_PUBLIC)

#if !defined(OPENNURBS_PLUS) || !defined(OPENNURBS_PLUS_INC_) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if defined(OPENNURBS_PLUS_FUNCTION_INC)
#error You probably should not be including opennurbs_plus_function.h in your code.
#define OPENNURBS_PLUS_FUNCTION_INC
#endif

#if !defined(OPENNURBS_PLUS_FUNCTION_BOZO_VACCINE)
#error Never include opennurbs_plus_function.h in your code.
#endif


////////////////////////////////////////////////////////////////////////
//
// Used by ON_HiddenLineDrawing constructors
//
typedef ON_HiddenLineDrawingImplBase* (*ON_PLUS_FUNCTION_GetNew_HiddenLineDrawing)(
	ON_SimpleArray<  class ON_HLDFullCurve*>&,
	ON_SimpleArray<  class ON_HLDCurve*>&,
	ON_SimpleArray< const class  ON_HLD_Object*>&
	);


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::CloseTrimGap()
//
typedef bool (*ON_PLUS_FUNCTION_BrepCloseTrimGap)( 
  class ON_Brep*,
  class ON_BrepTrim*,
  class ON_BrepTrim*
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_BrepFace::GetIsoIntervals()
//
typedef bool (*ON_PLUS_FUNCTION_BrepFaceGetIsoIntervals)( 
  const class ON_BrepFace*,
  int,
  double,
  ON_SimpleArray<ON_Interval>*,
  ON_SimpleArray<ON_Curve*>*,
  ON_SimpleArray<struct ON_BrepFaceIsoInterval>*
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_BendBrepFace()
//
typedef class ON_NurbsSurface* (*ON_PLUS_FUNCTION_BendBrepFace)( 
  const class ON_BrepFace* face0,
  const ON_SimpleArray<ON_BendFaceConstraint>*,
  double,
  double,
  class ON_NurbsSurface*,
  ON_TextLog*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_TransformBrepComponents()
//
typedef bool (*ON_PLUS_FUNCTION_TransformBrepComponents)( 
  ON_Brep*,
  int,
  const ON_COMPONENT_INDEX*,
  ON_Xform,
  double,
  double,
  bool
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_SquishMesh()
//

typedef class ON_Mesh* (*ON_PLUS_FUNCTION_SquishMesh)( 
  const class ON_Mesh*,
  const class ON_SquishParameters*,
  const ON_SimpleArray<const ON_Geometry*>*,
  ON_SimpleArray<ON_Geometry*>*,
  class ON_Mesh*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_SquishSurface()
//
typedef class ON_Brep* (*ON_PLUS_FUNCTION_SquishSurface)(
  const class ON_Surface*,
  const class ON_SquishParameters*,
  const ON_SimpleArray<const ON_Geometry*>*,
  ON_SimpleArray<ON_Geometry*>*,
  class ON_Brep*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_SquishBack2dMarks()
//
typedef bool (*ON_PLUS_FUNCTION_SquishBack2dMarks)(
  const ON_Geometry* squished_geometry, 
  const ON_SimpleArray<const ON_Geometry*>* marks,
  ON_SimpleArray<ON_Geometry*>* squishedback_marks);

////////////////////////////////////////////////////////////////////////
//
// Used by ON_MapBackPoint()
//
typedef bool (*ON_PLUS_FUNCTION_MapBackPoint)(
  const ON_Mesh& mesh0,
  const ON_3fPoint* V1,
  ON_3dPoint P0,
  ON_MESH_POINT& P1
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_MapBackCurve()
//
typedef class ON_PolylineCurve* (*ON_PLUS_FUNCTION_MapBackCurve)(
  const ON_Mesh& mesh0,
  double mesh0_edge_length,
  const ON_3fPoint* V1,
  const ON_Curve& curve0,
  ON_PolylineCurve* curve1
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Is2dPatternSquished()
//
typedef bool (*ON_PLUS_FUNCTION_Check2dPatternHasMapBackInfo)(const ON_Geometry* geom);

////////////////////////////////////////////////////////////////////////
//
// Used by ON_DumpSquishInfoText()
//
typedef void (*ON_PLUS_FUNCTION_DumpSquishInfoText)(const ON_Geometry* geom, ON_TextLog& text_log,
  const wchar_t* objid_locale_str, const wchar_t* meshcount_locale_str);

////////////////////////////////////////////////////////////////////////\
//
// Used by ON_Squisher::Create()
//
typedef ON_SquisherImpl* (*ON_PLUS_FUNCTION_CreateSquisherImpl)();

////////////////////////////////////////////////////////////////////////
//
// Used by ON_MeshClashSearch()
//
typedef void (*ON_PLUS_FUNCTION_MeshClashSearch)( 
  int,
  const class ON_Mesh* const *,
  int,
  const class ON_Mesh* const *,
  const class ON_Mesh*,
  const class ON_RTree*,
  const class ON_Mesh*,
  const class ON_RTree*,
  double,
  bool,
  int,
  ON_SimpleArray< class ON_ClashEvent >&
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Integrate() for one variable functions
//
typedef double (*ON_PLUS_FUNCTION_Integrate1)(
  double (*f)(ON__UINT_PTR context, int limit_direction, double t),
  ON__UINT_PTR f_context,
  const ON_SimpleArray<double>& limits,
  double relative_tolerance,
  double absolute_tolerance,
  double* error_bound
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Integrate() for two variable functions
//
typedef double (*ON_PLUS_FUNCTION_Integrate2)(
  double (*f)(ON__UINT_PTR context, int limit_direction, double s, double t),
  ON__UINT_PTR f_context,
  const ON_SimpleArray<double>& limits1,
  const ON_SimpleArray<double>& limits2,
  double relative_tolerance,
  double absolute_tolerance,
  double* error_bound
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Minimize() 
//
typedef double (*ON_PLUS_FUNCTION_Minimize)(
  unsigned n,
  double (*f)(ON__UINT_PTR context, const double* t, double* grad),
  ON__UINT_PTR context,
  const ON_Interval search_domain[],
  const double t0[],
  double terminate_value,
  double terminate_gradient,
  double relative_tolerance,
  double zero_tolerance,
  unsigned maximum_iterations,
  double t[],
  bool* bConverged
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_GetMatrixSVD()
//
typedef int (*ON_PLUS_FUNCTION_GetMatrixSVD)(
  int,
  int,
  int,
  double const * const *,
  double**&,
  double*&,
  double**&
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_IsPointListPlanar()
//
typedef int (*ON_PLUS_FUNCTION_IsPointListPlanar)(
  bool,
  int,
  int,
  const double*,
  const double*,
  const double*,
  double,
  ON_PlaneEquation*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Mesh2dPolygon()
//
typedef int (*ON_PLUS_FUNCTION_Mesh2dPolygon)(
  int,
  int,
  const double*,
  int,
  int*
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Triangulate2dRegion::
//
typedef int (*ON_PLUS_FUNCTION_Mesh2dRegion)(
  unsigned int,
  unsigned int,
  const double*,
  unsigned int,
  unsigned int,
  const unsigned int*,
  unsigned int,
  const unsigned char*,
  bool,
  bool,
  bool,
  ON_SimpleArray<ON_3dex>&,
  ON_SimpleArray<ON_2dPoint>*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by mesh intersections to tessellate
//
typedef int(*ON_PLUS_FUNCTION_SimpleCreate2dMesh)(
  const ON_SimpleArray<ON_2dPoint>& points,
  const ON_SimpleArray<ON_2udex>& edges,
  const ON_SimpleArray<unsigned char>& dirs,
  ON_SimpleArray<ON_3dex>& triangulation,
  ON_SimpleArray<ON_2dPoint>& new_pts,
  ON_TextLog* text_log
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_BrepFace::
//
typedef ON_Mesh* (*ON_PLUS_FUNCTION_BrepFace_Mesh)(
  const ON_BrepFace&,
  const ON_MeshParameters&,
  ON_Mesh*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::
//
typedef int (*ON_PLUS_FUNCTION_Brep_Mesh)(
  const ON_Brep&,
  const ON_MeshParameters&,
  ON_SimpleArray<ON_Mesh*>&
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::
//
typedef int (*ON_PLUS_FUNCTION_Brep_MassProperties)(
  const ON_Brep&,
  void*,
  int,
  ON_3dPoint,
  ON_MassProperties&,
  bool,
  bool,
  bool,
  bool,
  double,
  double
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::
//
typedef bool (*ON_PLUS_FUNCTION_Brep_SplitFaces)(
  ON_Brep&,
  bool,
  int,
  const int*,
  const int*,
  const double*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::
//
typedef bool (*ON_PLUS_FUNCTION_Brep_RegionTopologyHelper)(
  const ON_Brep&,
  ON_BrepRegionTopology&
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::
//
typedef ON_Brep* (*ON_PLUS_FUNCTION_Brep_MergeBrepsHelper)(
  const ON_Brep&,
  const ON_Brep&,
  double
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::
//
typedef bool(*ON_PLUS_FUNCTION_Brep_IsPointInsideHelper)(
	const ON_Brep&,
	ON_3dPoint,
	double,
	bool
	);

////////////////////////////////////////////////////////////////////////
//
// Used by ON_BrepFace::GetSilhouette()
//
typedef bool(*ON_PLUS_FUNCTION_BrepFace_Silhouette)(
	const ON_SilhouetteParameters ,
	const ON_BrepFace& ,
	const ON_PlaneEquation* ,
	size_t ,
	ON_ClassArray<ON_SIL_EVENT>& ,
	ON_ProgressReporter* ,
	ON_Terminator* 
	);


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::GetSilhouette()
//
typedef bool(*ON_PLUS_FUNCTION_Brep_Silhouette)(
	const ON_SilhouetteParameters ,
	const ON_Brep& ,
	const ON_PlaneEquation* ,
	size_t ,
	ON_ClassArray<ON_SIL_EVENT>& ,
	ON_ProgressReporter* ,
	ON_Terminator* 
	);


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Mesh::
//
typedef int (*ON_PLUS_FUNCTION_Mesh_MassProperties)(
  const ON_Mesh&,
  void*,
  int,
  ON_3dPoint,
  ON_MassProperties&,
  bool, 
  bool, 
  bool, 
  bool,
  double,
  double
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Mesh::GetSilhouette()
//
typedef bool(*ON_PLUS_FUNCTION_Mesh_Silhouette)(
	const ON_SilhouetteParameters ,
	const ON_Mesh& ,
	const ON_PlaneEquation* ,
	size_t ,
	ON_ClassArray<ON_SIL_EVENT>& ,
	ON_ProgressReporter* ,
	ON_Terminator*
	);


////////////////////////////////////////////////////////////////////////
//
// Used by ON_SurfaceTreeNode::
//
typedef const class ON_SurfaceTreeNode* (*ON_PLUS_FUNCTION_SurfaceTreeNode_GetClosestPoint)(
  const class ON_SurfaceTreeNode*,
  ON_3dPoint,
  double*,
  double*,
  ON_3dPoint*,
  double,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_SurfaceTreeNode::
//
typedef int (*ON_PLUS_FUNCTION_SurfaceTreeNode_IntersectSurfaceTree)(
  const class ON_SurfaceTreeNode*,
  const class ON_SurfaceTreeNode*,
  ON_ClassArray<ON_SSX_EVENT>&,
  double,
  double,
  double,
  const ON_Interval*,
  const ON_Interval*,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_CurveTreeNode::
//
typedef const class ON_CurveTreeNode* (*ON_PLUS_FUNCTION_CurveTreeNode_GetClosestPoint)(
  const class ON_CurveTreeNode*,
  ON_3dPoint,
  double*,
  ON_3dPoint*,
  double,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_CurveTreeNode::
//
typedef int (*ON_PLUS_FUNCTION_CurveTreeNode_IntersectCurveTree)(
  const class ON_CurveTreeNode*,
  const class ON_CurveTreeNode*,
  ON_SimpleArray<ON_X_EVENT>&,
  double,
  double,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_CurveTreeNode::
//
typedef int (*ON_PLUS_FUNCTION_CurveTreeNode_IntersectSurfaceTree)(
  const class ON_CurveTreeNode*,
  const class ON_SurfaceTreeNode*,
  ON_SimpleArray<ON_X_EVENT>&,
  double,
  double,
  const ON_Interval*,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_CurveTreeNode::
//
typedef int (*ON_PLUS_FUNCTION_CurveTreeNode_IntersectPlane)(
  const class ON_CurveTreeNode*,
  const class ON_PlaneEquation&,
  ON_SimpleArray<ON_X_EVENT>&,
  double,
  double,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_CurveTreeNode::
//
typedef int (*ON_PLUS_FUNCTION_CurveTreeNode_IntersectSelf)(
  const class ON_CurveTreeNode*,
  ON_SimpleArray<ON_X_EVENT>&,
  double,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_BezierCurve::
//
typedef bool (*ON_PLUS_FUNCTION_BezierCurve_GetLocalClosestPoint)(
  const class ON_BezierCurve*,
  ON_3dPoint,
  double,
  double*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_BezierCurve::
//
typedef bool (*ON_PLUS_FUNCTION_BezierCurve_LocalIntersectCurve)(
  const class ON_BezierCurve*,
  const class ON_BezierCurve*,
  double*,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_BezierCurve::
//
typedef bool (*ON_PLUS_FUNCTION_BezierCurve_LocalIntersectSurface)(
  const class ON_BezierCurve*,
  const class ON_BezierSurface*,
  double*,
  const ON_Interval*,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_BezierCurve::
//
typedef bool (*ON_PLUS_FUNCTION_BezierCurve_GetTightBoundingBox)(
  const class ON_BezierCurve*,
  ON_BoundingBox*,
  bool,
  const ON_Xform*
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_BezierSurface::GetLocalClosestPoint()
//
typedef bool (*ON_PLUS_FUNCTION_BezierSurface_GetClosestPoint)(
  const class ON_BezierSurface*,
  ON_3dPoint,
  double,
  double,
  double*,
  double*,
  const ON_Interval*,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Curve::MeshCurve().
//
typedef class ON_PolylineCurve* (*ON_PLUS_FUNCTION_Curve_Mesh)(
  const ON_Curve*,
  const ON_MeshCurveParameters*,
  class ON_PolylineCurve*,
  bool,
  const ON_Interval*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Curve::LengthMassProperties() 
// and ON_Curve::AreaMassProperties().
//
typedef int (*ON_PLUS_FUNCTION_Curve_MassProperties)(
  const ON_Curve&, 
  void*,
  int,
  ON_3dPoint,
  ON_3dVector,
  ON_MassProperties&,
  bool,
  bool,
  bool, 
  bool,
  double,
  double 
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Curve::CreateCubicLoft()
//
typedef ON_NurbsCurve* (*ON_PLUS_FUNCTION_Curve_CreateCubicLoft)(
  int, int, int, const double*,
  double,
  int,
  ON::cubic_loft_end_condition,
  ON::cubic_loft_end_condition,
  ON_NurbsCurve*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_NurbsCurve::RemoveKnots()
//
typedef bool (*ON_PLUS_FUNCTION_ON_NurbsCurve_RemoveKnots)(
  ON_NurbsCurve*,
  int, int, double
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_NurbsCurve::RemoveKnots()
//
typedef bool (*ON_PLUS_FUNCTION_ON_NurbsSurface_RemoveKnots)(
  ON_NurbsSurface*,
  int, int, int
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_NurbsCurve::GrevilleInterpolate()
//
typedef bool (*ON_PLUS_FUNCTION_ON_NurbsGrevilleInterpolate)(
  int, int, int, int,
  double*, const double*, int,
  int, 
  int, 
  const double*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Surface::CreateLinearLoft()
//
typedef ON_NurbsSurface* (*ON_PLUS_FUNCTION_Surface_CreateLinearLoft)(
  int,
  const ON_Curve* const*,
  double,
  int,
  ON_NurbsSurface*
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Surface::CreateCubicLoft()
//
typedef ON_NurbsSurface* (*ON_PLUS_FUNCTION_Surface_CreateCubicLoft)(
  int,
  const ON_Curve* const*,
  double,
  int,
  ON::cubic_loft_end_condition,
  ON::cubic_loft_end_condition,
  ON_NurbsSurface*
  );


////////////////////////////////////////////////////////////////////////
//
// Used by ON_Surface::AreaMassProperties() 
// and ON_Surface::VolumeMassProperties().
//
typedef int (*ON_PLUS_FUNCTION_Surface_MassProperties)(
  const ON_Surface&,
  void*,
  int, 
  ON_3dPoint,
  ON_MassProperties&,
  bool,
  bool,
  bool,
  bool,
  double,
  double
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Surface::GetSilhouette() 
//
typedef bool(*ON_PLUS_FUNCTION_Surface_Silhouette)(
	const ON_SilhouetteParameters,
	const ON_Surface& ,
	const ON_Interval* ,
	const ON_Interval* ,
	const ON_PlaneEquation* ,
	size_t ,
	ON_ClassArray<ON_SIL_EVENT>& ,
	ON_ProgressReporter* ,
	ON_Terminator* );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_NurbsSurface::CreateMesh().
//
typedef ON_Mesh* (*ON_PLUS_FUNCTION_NurbsSurface_Mesh)(
  const ON_NurbsSurface&,
  const ON_MeshParameters&,
  ON_Mesh*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_PlaneSurface::CreateMesh().
//
typedef ON_Mesh* (*ON_PLUS_FUNCTION_PlaneSurface_Mesh)(
  const ON_PlaneSurface&,
  const ON_MeshParameters&,
  ON_Mesh*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_RevSurface::CreateMesh().
//
typedef ON_Mesh* (*ON_PLUS_FUNCTION_RevSurface_Mesh)(
  const ON_RevSurface&,
  const ON_MeshParameters&,
  ON_Mesh*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_SumSurface::CreateMesh().
//
typedef ON_Mesh* (*ON_PLUS_FUNCTION_SumSurface_Mesh)(
  const ON_SumSurface&,
  const ON_MeshParameters&,
  ON_Mesh*
  );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Extrusion::Pushup().
//
typedef ON_Curve* (*ON_PLUS_FUNCTION_Extrusion_Pushup)(
          const ON_Extrusion&,
          const ON_Curve&,
          double,
          const ON_Interval*,
          ON_FitResult*
          );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Extrusion::Pullback().
//
typedef ON_Curve* (*ON_PLUS_FUNCTION_Extrusion_Pullback)( 
    const ON_Extrusion&,
    const ON_Curve&,
    double,
    const ON_Interval*,
    ON_3dPoint,
    ON_3dPoint,
    ON_FitResult*
    );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Mesh::IsPointInside().
//
typedef bool (*ON_PLUS_FUNCTION_Mesh_IsPointInside)(
    const ON_Mesh&,
    ON_3dPoint, 
    double,
    bool
    );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Mesh::SolidOrientation().
//
typedef int (*ON_PLUS_FUNCTION_Mesh_SolidOrientation)(
    const ON_Mesh&
    );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Brep::GetEdgeParameter().
//
typedef bool (*ON_PLUS_FUNCTION_Brep_GetEdgeOrTrimParameter)(
    const ON_Brep&,
    int,
    double,
    double*,
    bool
    );

////////////////////////////////////////////////////////////////////////
//
// Used by ON_Outline::GetOutlineMesh().
//
typedef unsigned int (*ON_PLUS_FUNCTION_Outline_GetOutlineMesh)(
  const class ON_Outline&,
  double,
  double,
  ON_SimpleArray<ON_2dPoint>&,
  ON_SimpleArray<ON_3udex>&,
  double*,
  double*,
  class ONX_ErrorCounter*
);

////////////////////////////////////////////////////////////////////////
//
// Used by New mesh booleans.
//
typedef bool (*ON_PLUS_FUNCTION_OldMeshBooleansPickTwoSides)(
  class ON_Mesh*,
  int,
  class ON_SimpleArray<int>*,
  class ON_Mesh*,
  bool
  );

////////////////////////////////////////////////////////////////////////
//
// static storage used by Rhino SDK to set function pointers used
// by opennurbs member functions.
//
class ON_CLASS ON_PlusFunction
{
public:
  static
  bool IsValid();

public:

	// Used by ON_BrepFace::
	static
		ON_PLUS_FUNCTION_BrepFace_Mesh f_ON_BrepFace_Mesh;

	// Used by ON_BrepFace::
	static
		ON_PLUS_FUNCTION_BrepFace_Silhouette f_ON_BrepFace_Silhouette;

  // Used by ON_Brep::
  static
  ON_PLUS_FUNCTION_Brep_Mesh f_ON_Brep_Mesh;

  // Used by ON_Brep::
  static
  ON_PLUS_FUNCTION_Brep_MassProperties f_ON_Brep_MassProperties;

  // Used by ON_Brep::
  static
  ON_PLUS_FUNCTION_Brep_SplitFaces f_ON_Brep_SplitFaces;

  // Used by ON_Brep::
  static
  ON_PLUS_FUNCTION_Brep_RegionTopologyHelper f_ON_Brep_RegionTopologyHelper;

  // Used by ON_Brep::
  static
  ON_PLUS_FUNCTION_Brep_MergeBrepsHelper f_ON_Brep_MergeBrepsHelper;

  // Used by ON_Brep::
  static
  ON_PLUS_FUNCTION_Brep_IsPointInsideHelper f_ON_Brep_IsPointInsideHelper;

	// Used by ON_Brep::
	static
	ON_PLUS_FUNCTION_Brep_Silhouette f_ON_Brep_Silhouette;

  // Used by ON_NurbsSurface::CreateMesh()
  static
  ON_PLUS_FUNCTION_NurbsSurface_Mesh f_ON_NurbsSurface_Mesh;

  // Used by ON_PlaneSurface::CreateMesh()
  static
  ON_PLUS_FUNCTION_PlaneSurface_Mesh f_ON_PlaneSurface_Mesh;

  // Used by ON_RevSurface::CreateMesh()
  static
  ON_PLUS_FUNCTION_RevSurface_Mesh f_ON_RevSurface_Mesh;

  // Used by ON_SumSurface::CreateMesh()
  static
  ON_PLUS_FUNCTION_SumSurface_Mesh f_ON_SumSurface_Mesh;

  // Used by ON_Mesh2dPolygon()
  static
  ON_PLUS_FUNCTION_Mesh2dPolygon f_ON_Mesh2dPolygon;

  // Used by ON_Mesh2dRegion()
  // Set by TL_PLUS_FUNCTION_Set_MeshingFunctionPointers().
  static
  ON_PLUS_FUNCTION_Mesh2dRegion f_ON_Mesh2dRegion;

  // Used by ON_MX_Split*
  static
    ON_PLUS_FUNCTION_SimpleCreate2dMesh f_ON_SimpleCreate2dMesh;

  // Used by ON_Mesh::
  static
  ON_PLUS_FUNCTION_Mesh_MassProperties f_ON_Mesh_MassProperties;

	// Used by ON_Mesh::GetSilhouette()
	static
		ON_PLUS_FUNCTION_Mesh_Silhouette f_ON_Mesh_Silhouette;

  // Used by ON_IsPointListPlanar()
  static
  ON_PLUS_FUNCTION_IsPointListPlanar f_ON_IsPointListPlanar;

  // Used by ON_GetMatrixSVD()
  static
  ON_PLUS_FUNCTION_GetMatrixSVD f_ON_GetMatrixSVD;

  // Used by ON_SurfaceTreeNode::
  static
  ON_PLUS_FUNCTION_SurfaceTreeNode_GetClosestPoint f_ON_SurfaceTreeNode_GetClosestPoint;

  // Used by ON_SurfaceTreeNode::
  static
  ON_PLUS_FUNCTION_SurfaceTreeNode_IntersectSurfaceTree f_ON_SurfaceTreeNode_IntersectSurfaceTree;

  // Used by ON_CurveTreeNode::
  static
  ON_PLUS_FUNCTION_CurveTreeNode_GetClosestPoint f_ON_CurveTreeNode_GetClosestPoint;

  // Used by ON_CurveTreeNode::
  static
  ON_PLUS_FUNCTION_CurveTreeNode_IntersectCurveTree f_ON_CurveTreeNode_IntersectCurveTree;

  // Used by ON_CurveTreeNode::
  static
  ON_PLUS_FUNCTION_CurveTreeNode_IntersectSurfaceTree f_ON_CurveTreeNode_IntersectSurfaceTree;

  // Used by ON_CurveTreeNode::
  static
  ON_PLUS_FUNCTION_CurveTreeNode_IntersectPlane f_ON_CurveTreeNode_IntersectPlane;

  // Used by ON_CurveTreeNode::
  static 
  ON_PLUS_FUNCTION_CurveTreeNode_IntersectSelf f_ON_CurveTreeNode_IntersectSelf;

  // Used by ON_BezierCurve::
  static
  ON_PLUS_FUNCTION_BezierCurve_GetLocalClosestPoint f_ON_BezierCurve_GetLocalClosestPoint;

  // Used by ON_BezierCurve::
  static
  ON_PLUS_FUNCTION_BezierCurve_LocalIntersectCurve f_ON_BezierCurve_LocalIntersectCurve;

  // Used by ON_BezierCurve::
  static
  ON_PLUS_FUNCTION_BezierCurve_LocalIntersectSurface f_ON_BezierCurve_LocalIntersectSurface;

  // Used by ON_BezierCurve::
  static
  ON_PLUS_FUNCTION_BezierCurve_GetTightBoundingBox f_ON_BezierCurve_GetTightBoundingBox;

  // Used by ON_BezierSurface::GetLocalClosestPoint()
  static
  ON_PLUS_FUNCTION_BezierSurface_GetClosestPoint f_ON_BezierSurface_GetClosestPoint;

  // Used by ON_Curve::MeshCurve().
  // Set by TL_PLUS_FUNCTION_Set_MeshingFunctionPointers().
  static
  ON_PLUS_FUNCTION_Curve_Mesh f_ON_Curve_Mesh;

  // Used by ON_Curve::LengthMassProperties() 
  // and ON_Curve::AreaMassProperties().
  // Set by TL_PLUS_FUNCTION_Set_MeshingFunctionPointers().
  static
  ON_PLUS_FUNCTION_Curve_MassProperties f_ON_Curve_MassProperties;

  // Used by ON_Curve::CreateCubicLoft().
  // Set by TL_PLUS_FUNCTION_SetCubicLoftFunctions().
  static
  ON_PLUS_FUNCTION_Curve_CreateCubicLoft f_ON_Curve_CreateCubicLoft;
  
  // Used by ON_NurbsCurve::RemoveKnots().
  // Set by TL_PLUS_FUNCTION_Set_Nurbs_RemoveKnots().
  static
  ON_PLUS_FUNCTION_ON_NurbsCurve_RemoveKnots f_ON_NurbsCurve_RemoveKnots;

  // Used by ON_NurbsCurve::RemoveKnots().
  // Set by TL_PLUS_FUNCTION_Set_Nurbs_RemoveKnots().
  static
  ON_PLUS_FUNCTION_ON_NurbsSurface_RemoveKnots f_ON_NurbsSurface_RemoveKnots;

  // Used by ON_NurbsCurve::GrevilleInterpolate().
  // Set by TL_PLUS_FUNCTION_Set_Nurbs_RemoveKnots().
  static
  ON_PLUS_FUNCTION_ON_NurbsGrevilleInterpolate f_ON_NurbsGrevilleInterpolate;

  // Used by ON_Surface::CreateLinearLoft().
  // Set by TL_PLUS_FUNCTION_SetCubicLoftFunctions().
  static
  ON_PLUS_FUNCTION_Surface_CreateLinearLoft f_ON_Surface_CreateLinearLoft;

  // Used by ON_Surface::CreateCubicLoft().
  // Set by TL_PLUS_FUNCTION_SetCubicLoftFunctions().
  static
  ON_PLUS_FUNCTION_Surface_CreateCubicLoft f_ON_Surface_CreateCubicLoft;

  // Used by ON_Surface::AreaMassProperties() and
  // ON_Surface::VolumeMassProperties()
  // Set by TL_PLUS_FUNCTION_Set_Surface_MassProperties().
  static
  ON_PLUS_FUNCTION_Surface_MassProperties f_ON_Surface_MassProperties;

	// Used by ON_Surface::GetSilhouettes() 
	static
	ON_PLUS_FUNCTION_Surface_Silhouette f_ON_Surface_Silhouette;

  // Used by ON_Extrusion::Pushup().
  static 
  ON_PLUS_FUNCTION_Extrusion_Pushup f_ON_Extrusion_Pushup;

  // Used by ON_Extrusion::Pullback().
  static 
  ON_PLUS_FUNCTION_Extrusion_Pullback f_ON_Extrusion_Pullback;

  // Used by ON_Mesh::IsPointInside
  static 
  ON_PLUS_FUNCTION_Mesh_IsPointInside f_ON_Mesh_IsPointInside;

  // Used by ON_Mesh::SolidOrientation
  static 
  ON_PLUS_FUNCTION_Mesh_SolidOrientation f_ON_Mesh_SolidOrientation;

  // Used by ON_Brep::GetEdgeParameter
  static
  ON_PLUS_FUNCTION_Brep_GetEdgeOrTrimParameter f_ON_Brep_GetTrimParameter;

  // Used by ON_Brep::GetEdgeParameter
  static
  ON_PLUS_FUNCTION_Brep_GetEdgeOrTrimParameter f_ON_Brep_GetEdgeParameter;

  // Used by ON_BendBrepFace
  static
  ON_PLUS_FUNCTION_BendBrepFace f_ON_BendBrepFace;

  // Used by ON_BendBrepFace
  static
  ON_PLUS_FUNCTION_TransformBrepComponents f_ON_TransformBrepComponents;

  // Used by ON_SquishMesh
  static
  ON_PLUS_FUNCTION_SquishMesh f_ON_SquishMesh;

  // Used by ON_SquishMesh
  static
  ON_PLUS_FUNCTION_SquishSurface f_ON_SquishSurface;

  // used by ON_SquishBack2dMarks
  static
    ON_PLUS_FUNCTION_SquishBack2dMarks f_ON_SquishBack2dMarks;

  // used by ON_MapBackPoint
  static
  ON_PLUS_FUNCTION_MapBackPoint f_ON_MapBackPoint;

  // used by ON_MapBackPoint
  static
  ON_PLUS_FUNCTION_MapBackCurve f_ON_MapBackCurve;

  // used by ON_Is2dPatternSquished
  static
    ON_PLUS_FUNCTION_Check2dPatternHasMapBackInfo f_ON_Check2dPatternHasMapBackInfo;

  // used by ON_DumpSquishInfoText
  static
    ON_PLUS_FUNCTION_DumpSquishInfoText f_ON_DumpSquishInfoText;

  // used by ON_Squisher::Create
  static
  ON_PLUS_FUNCTION_CreateSquisherImpl f_ON_CreateSquisherImpl;
  
  // Used by ON_MeshClashSearch and ON_MeshFaceClashSearch
  static
  ON_PLUS_FUNCTION_MeshClashSearch f_ON_MeshClashSearch;
  
  // Used by ON_BrepFace::GetIsoIntervals
  static
  ON_PLUS_FUNCTION_BrepFaceGetIsoIntervals f_ON_BrepFaceGetIsoIntervals;
  
  // Used by ON_Brep::CloseTrimGap
  static
  ON_PLUS_FUNCTION_BrepCloseTrimGap f_ON_BrepCloseTrimGap;

	// Used by ON_HiddenLineDrawing constructors
	// Set by TL_PLUS_FUNCTION_GetNew_HiddenLineDrawing().
	static
		ON_PLUS_FUNCTION_GetNew_HiddenLineDrawing f_ON_GetNew_HiddenLineDrawing;

	// Used by ON_Outline::GetOutlineMesh()
	static
		ON_PLUS_FUNCTION_Outline_GetOutlineMesh f_ON_Outline_GetOutlineMesh;

  // Used by new mesh booleans
  static
    ON_PLUS_FUNCTION_OldMeshBooleansPickTwoSides f_ON_OldMeshBooleansPickTwoSides;

  // Used by ON_Integrate() for 1 parameter functions
  static
    ON_PLUS_FUNCTION_Integrate1 f_ON_Integrate1;

  // Used by ON_Integrate() for 2 parameter functions
  static
    ON_PLUS_FUNCTION_Integrate2 f_ON_Integrate2;

  // Used by ON_Minimize() for to call TL_BoundedBFGSMin()
  static
    ON_PLUS_FUNCTION_Minimize f_ON_Minimize;  
};


#if defined(TL_COMPILING_TROUTLAKE)
////////////////////////////////////////////////////////////////////////
//
// Functions in TL used to set the ON_PlusFunction class's static 
// function pointers. NEVER put export macros on these TL function
// declarations because they have module scope and must not be called
// from outside of TL.
//
void TL_PLUS_FUNCTION_Set_Surface_CreateCubicLoft();
void TL_PLUS_FUNCTION_Set_Surface_MassProperties();
void TL_PLUS_FUNCTION_Set_MeshingFunctionPointers();
void TL_PLUS_FUNCTION_Set_Meshing2dRegionFunctionPointers();
void TL_PLUS_FUNCTION_Set_SurfaceClosestPointPointers();
void TL_PLUS_FUNCTION_Set_CurveSurfaceIntersectionPointers();
void TL_PLUS_FUNCTION_Set_CurveClosestPointPointers();
void TL_PLUS_FUNCTION_Set_CurveCurveIntersectionPointers();
void TL_PLUS_FUNCTION_Set_SurfaceSurfaceIntersectionPointers();
void TL_PLUS_FUNCTION_Set_BrepSpitFacePointers();
void TL_PLUS_FUNCTION_Set_BrepRegionTopologyHelper();
void TL_PLUS_FUNCTION_Set_GetSVDHelper();
void TL_PLUS_FUNCTION_Set_Extrusion_PushPull();
void TL_PLUS_FUNCTION_Set_Mesh_IsPointInsideEtCetera();
void TL_PLUS_FUNCTION_Set_Brep_GetEdgeParameter();
void TL_PLUS_FUNCTION_Set_BendBrepFaceHelper();
void TL_PLUS_FUNCTION_Set_TransformBrepComponentsHelper();
void TL_PLUS_FUNCTION_Set_SquishMeshHelper();
void TL_PLUS_FUNCTION_Set_SquishSurfaceHelper();
void TL_PLUS_FUNCTION_Set_SquisherCreate();
void TL_PLUS_FUNCTION_Set_SquishBack2dMarksHelper();
void TL_PLUS_FUNCTION_Set_MapBackPointHelper();
void TL_PLUS_FUNCTION_Set_MapBackCurveHelper();
void TL_PLUS_FUNCTION_Set_Check2dPatternHasMapBackInfoHelper();
void TL_PLUS_FUNCTION_Set_DumpSquishInfoText();
void TL_PLUS_FUNCTION_Set_MeshClashSearchHelper();
void TL_PLUS_FUNCTION_Set_BrepFaceIsoIntervalsHelper();
void TL_PLUS_FUNCTION_Set_GetSilhouettesPointers();
void TL_PLUS_FUNCTION_Set_GetNew_HiddenLineDrawing();
void TL_PLUS_FUNCTION_Set_Nurbs_RemoveKnots();
void TL_PLUS_FUNCTION_Set_Outline_GetOutlineMesh();
void TL_PLUS_FUNCTION_Set_TL_OldBooleans_PickTwoWays();
void TL_PLUS_FUNCTION_Set_IntegratePointers();
void TL_PLUS_FUNCTION_Set_MinimizePointers();
#endif

#endif
