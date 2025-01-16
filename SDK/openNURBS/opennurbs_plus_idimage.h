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

#if !defined(OPENNURBS_PLUS_IDIMAGE_INC_)
#define OPENNURBS_PLUS_IDIMAGE_INC_

/*
Description:
  ON_DepthImage is a tool for geometry calculations that want to
  use depth buffer rendering techniques. The depth buffer is a 
  double precision depth buffer and the calculations are all
  performed in double precision.
*/

#define ON_DepthImage_DEPTH_TYPE double

class ON_CLASS ON_DepthImage
{
 public:
   ON_DepthImage();
   ~ON_DepthImage();

  /*
  Parameters:
    vp - [in]
      A valid view projection.  
      - The frustum near and far distances must satisfy
          frust_left != frus_right
          frust_bottom != frus_top
          frus_near > 0
          frus_far > frus_near

    image_width - [in]
      number of horizontal elements in the image's id and depth planes.
      > 0
      Commonly set to vp.ScreenPortWidth()

    image_height - [in]
      number of vertical elements in the image's id and depth planes.
      > 0
      Commonly set to vp.ScreenPortHeight()

    projection_id - [in]
      When not nil, the projection id is used by drawing functions
      to eliminate duplicating calculations during mult-pass rendering
      algorithms.  When the projection is changed, either change
      the projection id or set it to nil.
  Remarks:
    The image's lower left corner has coordinates (0,0).
    The image's upper right corner has coordinates (image_width,image_height).
    The indices of the image's upper right id and depth elements are
    [image_width-1][image_height-1].
  */
  bool SetProjection( 
    const ON_Viewport& vp,
    unsigned int image_width,
    unsigned int image_height,
    ON_UUID projection_id
    );

  ON_UUID ProjectionId() const;

  const ON_Viewport& ProjectionViewport() const;

  /*
  Description:
    Push a copy of the current model transformation onto
    the model transformation stack.
  Returns:
    > 0: 
      The returned value is the number of transformations 
      that are in the stack after the push.
  */
  unsigned int PushModelTransformation();

  /*
  Description:
    Remove the most recently pushed model transformation from
    the model transformation stack.
  Returns:
    > 0: 
      The returned value is the number of transformations 
      that were in the stack before the pop.
    0:
      The model transformation stack was empty and nothing
      was changed.
  */
  unsigned int PopModelTransformation();

  /*
  Parameters:
    cumulative_model_transformation - [in]
      The cumulative model transformation that will be applied
      to world coordinate objects.  Model transformations
      act on the left.
  Returns:
    > 0: 
      The returned value is the number of transformations 
      one the stack.
    0:
      Invalid input.
  */
  unsigned int SetModelTransformation(
    ON_Xform cumulative_model_transformation
    );

  /*
  Description:
    Push a copy of the current model transformation onto
    the model transformation stack and set the cumulative
    transformation.
  Example:
    Add a translation to the cumulative model transformation.
       ON_Xform translation = ...;
       PushModelTransformation(ModelTransformation()*translation);
  Returns:
    > 0: 
      The returned value is the number of transformations 
      that are in the stack after the push.
  */
  unsigned int PushModelTransformation(
    ON_Xform cumulative_model_transformation
    );

  /*
  Returns:
    Cumulative model transformation.
  */
  ON_Xform ModelTransformation() const;

  /*
  Returns:
    Inverse of the cumulative model transformation.
  */
  ON_Xform ModelTransformationInverse() const;

  /*
  Returns:
    The number of transformations in model transformation
    stack. If no transformations have been pushed, then
    zero is returned.
  */
  unsigned int ModelTransformationStackCount() const;

  /*
  Description:
    Empty the model transformation stack.  This leaves
    the model transformation as the identity.
  */
  void PopAllModelTransformations();


  unsigned int Width() const;
  unsigned int Height() const;

  void DestroyImage();

  void EraseImageDepth(
    double erased_depth
    );

  void EraseImageId(
    ON__UINT_PTR erased_id
    );

  enum class DEPTH_TEST : unsigned int
  {
    unset = 0,
    never_passes,            // pass = false
    always_passes,           // pass = true
    less_passes,             // pass = (incoming depth < current depth)
    less_or_equal_passes,    // pass = (incoming depth <= current depth)
    equal_passes,            // pass = (incoming depth == current depth)
    notequal_passes,         // pass = (incoming depth != current depth)
    greater_or_equal_passes, // pass = (incoming depth > current depth)
    greater_passes,          // pass = (incoming depth >= current depth)
  };

  // Clipping status bits.
  // (Not an enum class because these values
  // are used to initize bits in unsigned ints.)
  enum CLIP_STATUS : unsigned int
  {
    unset      = 0,
    
    // view frustum clipping status.
    xmin_clip    = 0x01, // x < frustum left plane
    xmax_clip    = 0x02, // x > frustum right plane
    ymin_clip    = 0x04, // y < frustum bottom plane
    ymax_clip    = 0x08, // y > frustum top plane
    zfar_clip    = 0x10, // depth > frustum far distance
    znear_clip   = 0x20, // depth < frustum near distance
    zbehind_clip = 0x40, // depth <= 0 (on or behind camera plane)
    xyz_mask     = 0x7F,
    degenerate   = 0x80, // unable to project or clip - generally a calculation failed
    xyzd_mask    = 0xFF, // (degenerate | xyz_mask)

    // view frustum visibility
    infrus      = 0x100, // completely inside view frustum
    liminal     = 0x200, // may be partially inside frustum - more testing and clipping is required
    
    ignored     = 0x400, // outside frustum, outside custom clip region, not visible, calculation failed, degenerate, ...

    frustum_clip_mask = 0x1FF, // (degenerate | infrus | xyz_mask )
    visibility_status_mask = 0x700, // (infrus | liminal | ignored)

    // Triangle vertex order
    tridir_unset = 0x1000, // cannot compute triangle dir because a portion is behind the perspective viewpoint
    // after clipping, triangles with this status have the direction calculated when needed.
    tridir_none = 0x2000, // projected triangle has negligible area (side facing or degenerate)
    tridir_ccw  = 0x4000, // projected triangle vertices run counterclockwise (front facing)
    tridir_cw   = 0x8000, // projected triangle vertices run clockwise (back facing)
    tridir_status_mask = 0xF000,

    // a maximum of 16 custom clipping planes can
    // be use to define up to 16 convex regions.
    // The union of the regions is the active space.
    // This permits non-convex section views.
    custom_clip_1 = 0x00010000,
    custom_clip_2 = 0x00020000,
    custom_clip_3 = 0x00040000,
    custom_clip_4 = 0x00080000,
    custom_clip_5 = 0x00100000,
    custom_clip_6 = 0x00200000,
    custom_clip_7 = 0x00400000,
    custom_clip_8 = 0x00800000,
    custom_clip_9 = 0x01000000,
    custom_clip_10 = 0x02000000,
    custom_clip_11 = 0x04000000,
    custom_clip_12 = 0x08000000,
    custom_clip_13 = 0x10000000,
    custom_clip_14 = 0x20000000,
    custom_clip_15 = 0x40000000,
    custom_clip_16 = 0x80000000,

    custom_clip_mask = 0xFFFF0000,
    not_custom_clip_mask = 0x0000FFFF,

    all_bits_mask = 0xFFFFFFFF
  };

  enum class VISIBILITY : unsigned int
  {
    unset = 0,
    all_visible = 1,
    partially_visible = 2, // known to be partially visible
    not_visible = 3,
    unknown = 4            // more testing is required
  };

  /*
  Description:
    Determine visibility of the convex hull of a point set based on the
    clip status bits of a collection of points in the set. The clip status
    bits for a point are set by bitwise | of ON_DepthImage::CLIP_STATUS enum values.
    For a mesh, the set of vertices works.
    For a bezier or NURBS curve or surface with positive weights, the
    set of control points works.
  Parameters:
    ccr - [in]
      nullptr or a pointer to the custom clipping region used to
      set the clip status bits.
    bitwise_and_point_set_clip_status - [in]
      a bitwise & of the individual point statuses.
    bitwise_or_point_set_clip_status - [in]
      a bitwise | of the individual point statuses.
  Returns:
    ON_DepthImage::VISIBILITY::unset
      Invalid input
    ON_DepthImage::VISIBILITY::all_visible
      The entire convex hull is in a visible region
    ON_DepthImage::VISIBILITY::not_visible
      The entire convex hull is outside all visible regions.
      ON_DepthImage::VISIBILITY::partially_visible
      The convex hull has portions that are visible and not visible.
      Note the object (mesh, curve, surface) could be completely visible
      or completely hidden in this case.
    ON_DepthImage::VISIBILITY::unknown
      Unable to determine visibility based on simple plane tests.
      At least one point is degenerate or is not visible, but the chord 
      from this point to another in the set has the potential for being
      visible.
  */
  static ON_DepthImage::VISIBILITY Visibility(
    const class ON_DepthImageCustomClippingRegion* ccr,
    unsigned int bitwise_and_point_set_clip_status,
    unsigned int bitwise_or_point_set_clip_status
    );

  static ON_DepthImage::VISIBILITY FrustumVisibility(
    unsigned int bitwise_and_point_set_clip_status,
    unsigned int bitwise_or_point_set_clip_status
    );

  static ON_DepthImage::VISIBILITY CustomClippingRegionVisibility(
    const class ON_DepthImageCustomClippingRegion* ccr,
    unsigned int bitwise_and_point_set_clip_status,
    unsigned int bitwise_or_point_set_clip_status
    );



  /*
  Parameters:
    depth_test - [in]
      specifies the type of depth test.
    incoming_depth - [in]
      depth value to test.
    current_image_depth - [in]
      deth value in the current image.
  Returns:
    true: 
      incoming_depth passes the depth test when compared with current_image_depth.
    false: 
      incoming_depth fails the depth test when compared with current_image_depth.
  */
  static bool PassesDepthTest(
    ON_DepthImage::DEPTH_TEST depth_test,
    double incoming_depth,
    double current_image_depth
    );

  unsigned int ConvexHullClipStatus(
    size_t point_count,
    size_t point_stride,
    const double* points
    ) const;

  unsigned int BoundingBoxClipStatus(
    const class ON_BoundingBox* bbox
    ) const;

  /*
  Description:
    Draw world_point into the current image.
  Parameters:
    world_point - [in]
      world coordinate point to draw
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
      intentionally terminated rendering by returning false.
    true
      Otherwise.
  */
  bool Draw3dPoint(
    const double* world_point,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  /*
  Description:
    Draw world_point into the current image.
  Parameters:
    world_point0 - [in]
    world_point1 - [in]
      world coordinate points to draw
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
      intentionally terminated rendering by returning false.
    true
      Otherwise.
  */
  bool Draw3dLineSegment(
    const double* world_point0,
    const double* world_point1,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  bool Draw3dBezierCurve(
    bool bIsRational,
    int order,
    size_t cv_stride,
    const double* cv,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  bool DrawBezierCurve(
    const ON_BezierCurve& bezier_curve,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  bool DrawNurbsCurve(
    const ON_NurbsCurve& nurbs_curve,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  bool DrawCurve(
    const ON_Curve* curve,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  /*
  Description:
    Draw triangle_mesh into the current image.
  Parameters:
    triangle_mesh - [in]
      triangle mesh to draw.
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
      intentionally terminated rendering by returning false.
    true
      Otherwise.
  */
  bool DrawTriangleMesh(
    const class ON_DepthImageTriangleMesh& triangle_mesh,
    ON__UINT_PTR id,
    const class ON_DepthImageTriangleShader* shader
    );


  /*
  Description:
    Draw triangle_mesh into the current image.
  Parameters:
    world_point1 - [in]
    world_point2 - [in]
    world_point3 - [in]
      corners of the triangle
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
       intentionally terminated rendering by returning false.
    true
      Otherwise.
  */
  bool DrawTriangle(
    const double* world_point1,
    const double* world_point2,
    const double* world_point3,
    ON__UINT_PTR id,
    const class ON_DepthImageTriangleShader* shader
    );

  bool DrawTriangle(
    const float* world_point1,
    const float* world_point2,
    const float* world_point3,
    ON__UINT_PTR id,
    const class ON_DepthImageTriangleShader* shader
    );


  /*
  Description:
    Draw mesh into the current image.
  Parameters:
    mesh - [in]
      mesh to draw
    triangle_mesh - [in]
      null or the triangle_mesh that will be used to draw mesh.
      This parameter is useful when many objects are being
      drawn to minimize calls to heap functions.
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
      intentionally terminated rendering by returning false.
    true
      Otherwise.
  */
  bool DrawMesh(
    const class ON_Mesh& mesh,
    class ON_DepthImageTriangleMesh* triangle_mesh,
    ON__UINT_PTR id,
    const class ON_DepthImageTriangleShader* shader
    );

  /*
  Description:
    Draw extrusion into the current image.
  Parameters:
    extrusion - [in]
      extrusion to draw
    triangle_mesh - [in]
      null or the triangle_mesh that will be used to draw extrusion.
      This parameter is useful when many objects are being
      drawn to minimize calls to heap functions.
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
      intentionally terminated rendering by returning false.
    true
      Otherwise.
  */
  bool DrawExtrusion(
    const class ON_Extrusion& extrusion,
    class ON_DepthImageTriangleMesh* triangle_mesh,
    ON::mesh_type mesh_type,
    ON__UINT_PTR id,
    const class ON_DepthImageTriangleShader* shader
    );

  /*
  Description:
    Draw brep into the current image.
  Parameters:
    brep - [in]
      brep to draw
    triangle_mesh - [in]
      null or the triangle_mesh that will be used to draw brep.
      This parameter is useful when many objects are being
      drawn to minimize calls to heap functions.
    id - [in]
      id passed to shader.
    shader - [in]
      null or a custom shader.
  Returns:
    false
      The image is not ready for drawing or the shader
      intentional terminated rendering by returning false.
    true
      Otherwise.
  */
  bool DrawBrep(
    const class ON_Brep& brep,
    class ON_DepthImageTriangleMesh* triangle_mesh,
    ON::mesh_type mesh_type,
    ON__UINT_PTR id,
    const class ON_DepthImageTriangleShader* shader
    );

  /*
  Description:
    Test a bounding box to see if some portion is visible.
  Parameters:
    bbox - [in]
    expansion_distance - [in]
      Before testing, expand the bounding box by adding this distance 
      to each of the bbox.m_max coordinates and subtracting this
      distance from each of the bbox.m_min coordinates.
    depth_test - [in]
    depth_bias - [in]
      Before testing, move the bounding box a distance of depth_bias
      parallel to the view direction.  
      A positive depth_bias moves the deeper (away from the camera).
      A a negative depth_bias moves the box nearer (towards the camera).
  */
  bool IsVisibleBoundingBox(
    ON_BoundingBox bbox,
    double expansion_distance,
    const ON_DepthImage::DEPTH_TEST depth_test,
    double depth_bias
    );


  /*
  Description:
    Test a triangle mesh to see if some portion is visible.

  Parameters:
    triangle_mesh - [in]

    triangle_status_mask - [in]
      If triangle_status_mask is not zero, then only triangles
      satisfying 0 != (triangle_status & triangle_status_mask)
      will be tested.  Specifically, setting triangle_status_mask
      to ON_DepthImage::tridir_ccw or ON_DepthImage::tridir_cw
      can be used to limit testing to front or back facing
      triangles.

    depth_test - [in]
      To test for ordinary visibility, set depth_test to
      the same value used to create the image, typically
      ON_DepthImage::DEPTH_TEST::less_passes or ON_DepthImage::less_or_equal_passes.
      You can use other values to test if some portion of 
      triangle_mesh is in the frustum and "behind" a set pixel
      by using ON_DepthImage::greater_passes.

    depth_bias - [in]
      Before testing, move the bounding box a distance of depth_bias
      parallel to the view direction.  
      A positive depth_bias moves the deeper (away from the camera).
      A a negative depth_bias moves the box nearer (towards the camera).

    pixel - [out]
      If not null, image pixel information for the first pixel 
      discovered where the triangle_mesh is visible.

    visible_triangle - [out]
      If not null, the triangle where the visible point was discovered.

    triangle_depth - [out]
      If not null, the depth of the point on the visible triangle.
  */
  bool IsVisibleTriangleMesh(
    class ON_DepthImageTriangleMesh& triangle_mesh,
    unsigned int triangle_status_mask,
    const ON_DepthImage::DEPTH_TEST depth_test,
    double depth_bias,
    class ON_DepthImagePixel* pixel,
    class ON_DepthImageTriangle* visible_triangle,
    double* visible_triangle_depth
    );

  bool GetPixel(
    unsigned int x,
    unsigned int y,
    ON__UINT_PTR* id,
    double* depth
    ) const;

  bool GetPixel(
    int x,
    int y,
    ON__UINT_PTR* id,
    double* depth
    ) const;

  bool SetPixel(
    unsigned int x,
    unsigned int y,
    ON__UINT_PTR id,
    double depth
    );

  bool SetPixel(
    int x,
    int y,
    ON__UINT_PTR id,
    double depth
    );

  /*
  Parameters:
    image_x - [in]
      >= 0.0 and <= image.Width().
    image_y - [in]
      >= 0.0 and <= image.Height().
  Remarks:
    The lower left corner of the image has image coordinates (0,0)
    The upper right corner of the image has image coordinates (Width(),Height())
  */
  bool GetWorldPoint(
    double image_x,
    double image_y,
    double depth,
    double* world_point
    ) const;

  /*
  Description:
    Gets a world point at the center of each pixel
    whose depth is greater than zero and less than
    ON_DBL_MAX and not equal to ErasedDepthValue().
  Parameters:
    points  - [out]
      Output world coordinate points are appended to this array.
  Returns:
    number of points append to points[].
  */
  unsigned int GetWorldPoints(
    ON_SimpleArray<ON_3dPoint>& points
    ) const;

  /*
  Description:
    Gets a world point at the center of each pixel
    whose id equals id depth is greater than zero and less than
    ON_DBL_MAX and not equal to ErasedDepthValue().
   Parameters:
      id_filter - [in]
        All pixels whose id value is not equal to
        id_filter are skipped.
      depth_filter - [in]
        All pixels whos depth value does not satisfy
        depth >= depth_filter.m_min and depth <= depth_filter.m_max
        are skipped.
    points  - [out]
      Output world coordinate points are appended to this array.
  Returns:
    number of points append to points[].
  */
  unsigned int GetWorldPoints(
    ON__UINT_PTR id_filter,
    ON_Interval depth_filter,
    ON_SimpleArray<ON_3dPoint>& points
    ) const;

  ON__UINT_PTR ErasedIdValue() const;

  double ErasedDepthValue() const;

  /*
  Parameters:
    point - [in]
      world coordinate 3d point
    image_point - [out]
      x,y = image point
      z = signed distance from the point to the image plane through
          the camera location.          
  Returns:
    A value set by bitwise or of ON_DepthImage::CLIP_STATUS values.
  */
  unsigned int ProjectPoint(
    const double point[3],
    double image_point[3]
    ) const;


  /*
  Parameters:
    point - [in]
      world coordinate 3d point
    image_depth - [out]
      signed distance from the point to the image plane
      through the camera location.          
  Returns:
    ON_DepthImage::CLIP_STATUS::zfar_clip
      depth > frustum far depth
    ON_DepthImage::CLIP_STATUS::infrus
      depth >= frustum near depth and depth <= frustum far depth
    ON_DepthImage::CLIP_STATUS::znear_clip
      depth < frustum near depth
    ON_DepthImage::CLIP_STATUS::degenerate
      depth cannot be calculated
    A value set by bitwise or of ON_DepthImage::CLIP_STATUS values.
  */
  unsigned int GetPointDepth(
    const double point[3],
    double* image_depth
    ) const;

  /*
  Parameters:
    plane_equations - [in]
      world coordinate custom clipping planes.
      A point is in the visible portion of the region if and only if
      the value of every plane equation is >= 0.0 at the point.
    plane_equation_count - [in]
      <= MaximumCustomClippingPlaneCount()
  */
  bool SetCustomClippingConvexRegion(
    const ON_PlaneEquation* plane_equations,
    size_t plane_equation_count
    );

  /*
  Parameters:
    plane_equations - [in]
      world coordinate custom clipping planes.
      A point is in the visible portion of the region if and only if
      the value of every plane equation is >= 0.0 at the point.
  plane_equation_count - [in]
    <= MaximumCustomClippingPlaneCount() - CustomClippingPlaneCount()
  */
  bool AddCustomClippingConvexRegion(
    const ON_PlaneEquation* plane_equations,
    size_t plane_equation_count
    );

  void DeleteCustomClippingRegions();

  /*
  Returns:
    Number of convex clipping regions.
  Remarks:
    The entire custom clipping region is the union of the convex clipping regions.
  */
  unsigned int CustomClippingConvexRegionCount() const;

  /*
  Returns:
    Total number of planes in the custom clipping region.
  Remarks:
    These planes are grouped into subsets that bound convex subsets.
    The entire custom clipping region is the union of the convex subsets.
  */
  unsigned int CustomClippingPlaneCount() const;

  static unsigned int MaximumCustomClippingPlaneCount();


  const class ON_DepthImageCustomClippingRegion* WorldCoordinateCustomClippingRegion() const;
	const class ON_DepthImageCustomClippingRegion* ClipCoordinateCustomClippingRegion() const;

	// GBA 12/19/16 RH-32565 Added cached Object coordinate version of custom clipping region
	const class ON_DepthImageCustomClippingRegion* ObjCoordinateCustomClippingRegion() const;


private:
  friend struct INTERP_LINE_SEGMENT;

  bool AllocateImage();
  void DeallocateImage();
  bool BeforeDraw();  

  ON_UUID m_projection_id = ON_nil_uuid;

  ON_Viewport m_vp;
  bool m_bInterpInverseDepth = false; // true for perspective views, false for parallel views
  ON_Plane m_vp_camera_plane; // origin = camera, zaxis points out of frustum
  ON_PlaneEquation m_vp_depth_plane_eq; // origin = camera, normal points into frustum
  // m_vp frustum near and far distances used to calculate the view projection
  // transformations. Whenever possible, the 
  double m_vp_frus_near = 0.0;
  double m_vp_frus_far = 0.0;

  // frustum extents at a camera depth of 1 unit
  double m_vp_frus1_left = 0.0;
  double m_vp_frus1_right = 0.0;
  double m_vp_frus1_bottom = 0.0;
  double m_vp_frus1_top = 0.0;

  // view projection transformations from m_vp.
  ON_Xform m_world2clip = ON_Xform::ZeroTransformation;  // world to clip = ModelTransformation()*m_vp_world2clip
  ON_Xform m_vp_world2clip = ON_Xform::ZeroTransformation;  // view projection world to clip
  ON_Xform m_clip2image = ON_Xform::ZeroTransformation; // clip to image
  ON_Xform m_world2image = ON_Xform::ZeroTransformation; // world to image transformation = ModelTransformation()*m_vp_world2image
  ON_Xform m_vp_world2image = ON_Xform::ZeroTransformation; // view projection world to image transformation
  ON_Xform m_clip2camera = ON_Xform::ZeroTransformation; // clip to camera 
  ON_Xform m_camera2clip = ON_Xform::ZeroTransformation; // camera to clip

  // m_vp_clip_far_limit < 0 and is the smallest valid value for
  // a clipping z coordinate. In orthographic projections, this
  // value is ON_DBL_MIN.  In perspective projections, this value
  // is -(1.0-ON_EPSILON)*((frus_far + frus_near)/(frus_far - frus_near)).
  // Points in the semi-infinite view frustum beyond the far clipping
  // plane will have a -1.0 < z clipping coordinate <= m_vp_clip_far_limit.
  double m_vp_clip_far_limit = 0.0;

  // m_clip_z_lft[] are the coefficients of the linear fractional transformation
  // that removes clipping coordinate z depth distortion. For orthographic projections,
  // m_clip_z_lft[] = (1.0,0.0).  This linear fractional transformation is used
  // to convert clipping z coordinates into world depth values.  Clipping coordinates
  // are the result of applying the transformation m_w2c to a world coordinate point.
  // Usage:
  //  double clip_z = ...; // clipping coordinate depth (+1.0 = near, -1.0 = far)
  //  double lft_clip_z = (m_clip_z_lft[0]*clip_z + m_clip_z_lft[1])
  //                    / (m_clip_z_lft[0] + m_clip_z_lft[1]*clip_z);
  //  double s = 0.5*(lft_clip_z+1.0);
  //  double world_depth = s*m_vp_near_distance + (1.0-s)*m_vp_frus_far;
  double m_clip_z_lft[2]; 


  // Parameters needed in projection functions are stored in a single
  // array so they may be passed as a simple const double*.
  // m_point_projection_parameters[0,...,3]
  //  plane equation (camera location, normal points into frustum)
  //  If a model transform is present, this plane is transformed by it.
  // m_point_projection_parameters[4]
  //  frustum near distance
  // m_point_projection_parameters[5,...,20]
  //  4x4 world to image transformation
  //  If a model transform is present, it is included in this transformation.
  // m_point_projection_parameters[21,22]
  //  image width, image height
  double m_point_projection_parameters[23];

  ON_DepthImage_DEPTH_TYPE** m_depth_buffer = nullptr;
  ON__UINT_PTR** m_id_buffer = nullptr;
  size_t m_buffer_width = 0;
  size_t m_buffer_height = 0;

  unsigned int m_width = 0;
  unsigned int m_height = 0;

  unsigned char m_vp_status = 0;    // 0 uninitialized, 1 valid
  unsigned char m_depth_status = 0; // 0 uninitialized, 1 erased, 2 depths vary
  unsigned char m_id_status = 0;    // 0 uninitialized, 1 erased, 2 ids vary
  unsigned char m_image_status = 0; // 0 uninitialized, 1 ready for drawing

  ON__UINT_PTR m_erased_id = 0;
  ON_DepthImage_DEPTH_TYPE m_erased_depth = ON_DBL_MAX;

  double m_degenerate_image_x = ON_DBL_QNAN;
  double m_degenerate_image_y = ON_DBL_QNAN;
  double m_degenerate_image_z = ON_DBL_QNAN;

  ON_FixedSizePool m_depth_fsp;
  ON_FixedSizePool m_id_fsp;

  class ON_DepthImageModelTransformLink* m_model_transform_stack = nullptr;

  // regions in the world a clip coordinate systems.
  class ON_DepthImageCustomClippingRegion* m_world_coord_custom_clipping_region = nullptr;

	// m_clip_coord_custom_clipping_region and m_obj_coord_custom_clipping_region are lazy evaluated cached 
	// versions of clipping region in other coordinate systems.
  // m_clip_coord_custom_clipping_region = m_vp_world2clip*m_world_coord_custom_clipping_region;
	class ON_DepthImageCustomClippingRegion* m_clip_coord_custom_clipping_region = nullptr;
	// m_obj_coord_custom_clipping_region = ModelTransformationInverse()* m_world_coord_custom_clipping_region;
	class ON_DepthImageCustomClippingRegion* m_obj_coord_custom_clipping_region = nullptr;


private:
  void UpdateModelTransformation();

  unsigned int ProjectPointXY(
    const double* w2i, // = ON_DepthImage::m_w2i
    const double point[3],
    double image_xy[2]
    ) const;

  /*
  Parameters:
    image_point_count - [in] >= 0
    image_point_stride - [in] >= 3
      The first coordinate of the i-th image point is 
      image_points + i*image_point_stride
    image_points - [in]
      list of image points to connect with line segments.  
      Each point has three image coordinate values, 
      (x,y,depth), where depth > 0.
    bClose - [in]
      true if a segment should be rendered between the initial
      and final image points.
    id - [in]
    shader - [in]
  Returns:
    true - continue rendering
    false - shader terminated rendering.
  */
  bool RenderPolyline(
    size_t image_point_count,
    size_t image_point_stride,
    const double* image_points,
    bool bClose,
    ON__UINT_PTR id,
    const class ON_DepthImagePointShader* shader
    );

  /*
  Description:
    This function assumes triangle->m_triangle_status is set correctly
    and simple visibility tests base on clipping status have been 
    correctly performed.
  */
  bool RenderTriangle( 
    class ON_DepthImageRenderTriangleContext* rtc
    );

  /*
  Parameters:
    rtc - [in]
    points - [in/out]
      The m_pixel_x and m_pixel_y values are modified.
  */
  bool RenderConvexPolygon( 
    class ON_DepthImageRenderTriangleContext* rtc,
    unsigned int point_count,
    const struct RENDER_VERTEX *points
    );

  bool RenderScanline(
    class ON_DepthImageRenderTriangleContext* rtc,
    const struct INTERP* iA,
    const struct INTERP* iB
    );

  
  bool CalculateZClipIntersection( 
    const class ON_DepthImageCustomClippingRegion* ccr,
    const double world_point0[3],
    const double world_point1[3],
    double image_z0,
    double image_z1,
    struct RENDER_VERTEX *out
    ) const;

  double ClippingZToWorldDepth(
    double clip_z
    ) const;

  /*
  Returns:
     >= 3: successfully clipped.
           The returned number is the number of elements of points_out[]
           that are the corners of a visible convex polygon.
     0: successfully clipped and nothing is visible.
     1: clipping calculation failed.
  */
  unsigned int SutherlandHodgmanViewFrustumClip( 
    class ON_DepthImageRenderTriangleContext* rtc,
    unsigned int clip_status,
    size_t points_in_count,
    const struct RENDER_VERTEX *points_in, 
    size_t points_out_capacity,
    struct RENDER_VERTEX *points_out
    );

  unsigned int SutherlandHodgmanConvexRegionClip(
    class ON_DepthImageRenderTriangleContext* rtc,
    unsigned int plane_index0,
    unsigned int plane_index1,
    size_t points_in_count,
    const struct RENDER_VERTEX *points_in,
    size_t points_out_capacity,
    struct RENDER_VERTEX *points_out
    );

    bool CalculateClipIntersection(
    class ON_DepthImageRenderTriangleContext* rtc,
      const struct RENDER_VERTEX *from,
      const struct RENDER_VERTEX *to,
      unsigned int clip_status,
    struct RENDER_VERTEX *out
      );

    bool CalculateCustomClippingPlaneIntersection(
    class ON_DepthImageRenderTriangleContext* rtc,
      const struct RENDER_VERTEX *from,
      const struct RENDER_VERTEX *to,
      unsigned int plane_index,
      unsigned int cp_bit,
      struct RENDER_VERTEX *out
      );

  static bool Draw3dBezierClippedPoints(
    void* context,
    size_t point_count,
    size_t point_stride,
    double* points
    );

  unsigned int GetWorldPointsHelper(
    bool bIdFilterEnabled,
    ON__UINT_PTR id_filter,
    bool bDepthFilterEnabled,
    ON_Interval depth_filter,
    ON_SimpleArray<ON_3dPoint>& points
    ) const;

  void InitializeDrawBezier(
    class Draw3dBezierClippedPointsContext& context,
    class ON_DrawBezierQQ& db
    );
};

class ON_CLASS ON_DepthImageTriangleMesh
{
public:
  ON_DepthImageTriangleMesh();
  ~ON_DepthImageTriangleMesh();

  /*
  Parameters:
    mesh - [in]
    bAsReference [ in]
      true:
        When possible, this triangle mesh will reference mesh 
        vertex and face information.  In this case, the caller
        must insure mesh stays in scope and is not modified 
        while this triangle mesh is in use.
      false:
        mesh vertex and face information is copied and this triangle
        mesh is independent from mesh.
  */
  bool Set(
    const class ON_Mesh* mesh,
    bool bAsReference
    );

  /*
  Parameters:
    bbox - [in]
    expansion_distance - [in]
      Before creating the triangle mesh, expand the bounding box by
      adding this distance to each of the bbox.m_max coordinates and
      subtracting this distance from each of the bbox.m_min coordinates.
  */
  bool Set(
    ON_BoundingBox bbox,
    double expansion_distance
    );

  void Unset();
           
  void Destroy();

  bool Grow(
    size_t vertex_capacity,
    size_t triangle_capacity
    );

  // vertices
  size_t m_V_count;
  size_t m_V_stride;
  const double* m_V;  // vertex locations

  // triangles (specified as three vertex indices)
  size_t m_T_count;
  size_t m_T_stride;
  const unsigned int* m_T;

  /*
  Returns:
    True if the triangle is visible.  "Visible" means
    - The triangle is valid.
    - Either false = m_bSkipBackfaces or the triangle is front facing.
    - A bitwise and of the corner vertex clipflags is zero.
    - No corner vertex is invalid.
  */
  bool TriangleIsVisible(
    unsigned int triangle_index
    ) const;

  unsigned int TriangleStatus(
    unsigned int triangle_index
    ) const;

  bool GetVertexLocation( 
    unsigned int vertex_index,
    ON_3dPoint* location
    ) const;

  bool GetTriangleLocation( 
    unsigned int triangle_index,
    ON_3dPoint location[3]
    ) const;

  void GetRenderVertex( 
    unsigned int vertex_index,
    struct RENDER_VERTEX* render_vertex
    ) const;

  void GetTriangleRenderVertices( 
    unsigned int triangle_index,
    struct RENDER_VERTEX render_vertex[3]
    ) const;

private:
  // When m_image_id is nil and it is not equal to 
  // ON_DepthImage::ImageId(), ProjectMesh() calculates
  // the information needed to project the mesh onto
  // a depth image.  When m_image_id is not nil and it
  // is equal to ON_DepthImage::ImageId(), ProjectMesh()
  // assumes the current projection information is valid
  // for the image.  This speeds up muli-pass rendering
  // algorithms.
  ON_UUID m_image_projection_id;

  size_t m_triangle_capacity;
  size_t m_vertex_capacity;

  double* m_V_buffer;
  unsigned int* m_T_buffer;

public:
  unsigned int *m_vertex_status;   // [m_vertex_capacity] 
                                   // & 0xFF = CLIP_STATUS, 
                                   // & 0xFF00 = VERTEX_STATUS
                                   // & 0xFFFF0000 = custom clipping region status

  unsigned int *m_triangle_status; // [m_triangle_capacity]
                                   // (0xFFFF00FF & (vertex[0].m_vertex_status | ... | vertex[2].m_vertex_status))
                                   // | (0xFF00 & (triangle CLIP_STATUS bits))

  // m_mesh_vertex_status is a bitwise or of every m_vertex_status[] value
  // that belongs to a triangle that may be visible.
  unsigned int m_and_vertex_status;
  unsigned int m_or_vertex_status;
  unsigned int m_and_triangle_status;
  unsigned int m_or_triangle_status;
  ON_DepthImage::VISIBILITY m_mesh_visibility;

private:
  // imagePoint coordinates:
  //  x,y = image point
  //  z = signed distance from image plane through the camera to the vertex
  //      >= frustum near distance is visible
  ON_3dPoint *m_image_points; // [m_vertex_capacity]

  // custom clipping region clipping plane equation values
  unsigned int m_cp_count;
  double* m_cp_values; // [m_vertex_capacity * m_cp_count]


public:
  /*
  Returns:
    true if some portion of the mesh may be visible.
    false if no portion of the mesh is visible.
  */
  ON_DepthImage::VISIBILITY ProjectMesh(
    const class ON_DepthImage& image
    );

private:
  /*
  Returns:
    true if some portion of the mesh may be visible.
    false if no portion of the mesh is visible.
  */
  void ProjectVertices(
    const class ON_DepthImage& image
    );

  void ProjectTriangles(
    const class ON_DepthImage& image
    );

};

/*
Description:
  ON_DepthImageTriangle is used to pass image triangle information to
  ON_DepthImage shaders.
*/
class ON_CLASS ON_DepthImageTriangle
{
public:
  static const ON_DepthImageTriangle Unset;
  const class ON_DepthImage* m_image;
  const class ON_DepthImageTriangleMesh* m_triangle_mesh;
  unsigned int  m_triangle_index;
  unsigned int  m_triangle_status; // ON_DepthImageTriangleMesh.m_triangle_status value
};


/*
Description:
  ON_DepthImagePixel is used to pass id image pixel information to ON_DepthImage
  shaders.
*/
class ON_CLASS ON_DepthImagePixel
{
public:
  static const ON_DepthImagePixel Unset;

  const class ON_DepthImage* m_image;

  // pixel coordinates
  unsigned int m_i; // 0 <= m_i < m_id_image->m_width
  unsigned int m_j; // 0 <= m_j < m_id_image->m_height

  // pointer to the id value in the image
  ON__UINT_PTR* m_id; // = &m_id_image->m_id[m_j][m_i]

  // pointer to the depth value in the image
  ON_DepthImage_DEPTH_TYPE* m_depth; // = &m_id_image->m_depth[m_j][m_i]
};

/*
Description:
  ON_DepthImageShader is used to pass a shader function and context
  to ON_DepthImage drawing tools.
*/
class ON_CLASS ON_DepthImagePointShader
{
public:
  ON_DepthImagePointShader();

  static const ON_DepthImagePointShader Default;

  void* m_context; // first parameter passed to m_function

  bool (*m_shade_pixel_function)(
    void*,                           // m_context pointer
    ON__UINT_PTR,                    // drawing id
    double,                          // incoming depth value
    const class ON_DepthImagePixel*     // pixel being shaded
    );

  /*
  Description:
    When m_context points to a ON_DepthImageDefaultShaderContext
    class, the DefaultShadePixel function can be used as the 
    m_shade_pixel_function.
  */
  static bool DefaultShadePixel(
    void* context, 
    ON__UINT_PTR id,
    double depth,
    const class ON_DepthImagePixel* pixel
    );
};

/*
Description:
  ON_DepthImageShader is used to pass a shader function and context
  to ON_DepthImage drawing tools.
*/
class ON_CLASS ON_DepthImageTriangleShader
{
public:
  ON_DepthImageTriangleShader();

  static const ON_DepthImageTriangleShader Default;

  void* m_context; // first parameter passed to m_function

  /*
  Returns:
    true if the triangle should be included in the image.
  */
  bool (*m_test_triangle_function)(
    void*,                           // m_context pointer
    const class ON_DepthImageTriangle*  // triangle being shaded
    );

  bool (*m_shade_pixel_function)(
    void*,                           // m_context pointer
    const class ON_DepthImageTriangle*, // triangle being shaded
    ON__UINT_PTR,                    // drawing id
    double,                          // incoming depth value
    const class ON_DepthImagePixel*     // pixel being shaded
    );

  /*
  Description:
    When m_context points to a ON_DepthImageDefaultShaderContext
    class, the DefaultTestTriangle function can be used as the 
    m_test_triangle_function.
  Parameters:
    context - [in]
      This must point to a ON_DepthImageDefaultShaderContext class.
    triangle - [in]
      Triangle information about the triangle ON_DepthImage::DrawTriangleMesh()
      will draw if this function returns true.
  Returns:
    0 == (triangle->m_triangle_status & ON_DepthImageDefaultShaderContext::m_test_triangle_status).
  Example:
    To create a triangle shader that will cull back faces:
        ON_DepthImageDefaultShaderContext shader_context;
        shader_context.m_test_triangle_status = ON_DepthImage::back_triangle;
        ON_DepthImageTriangleShader shader(ON_DepthImageTriangleShader::Default);
        shader.m_context = &shader_context;
  */
  static bool DefaultTestTriangle(
    void* context, 
    const class ON_DepthImageTriangle* triangle
    );
  
  /*
  Description:
    When m_context points to a ON_DepthImageDefaultShaderContext
    class, the DefaultShadePixel function can be used as the 
    m_shade_pixel_function.
  */
  static bool DefaultShadePixel(
    void* context, 
    const class ON_DepthImageTriangle* triangle,
    ON__UINT_PTR id,
    double depth,
    const class ON_DepthImagePixel* pixel
    );
};

class ON_CLASS ON_DepthImageDefaultShaderContext
{
public:
  ON_DepthImageDefaultShaderContext();

  static const ON_DepthImageDefaultShaderContext Default;

  // depth buffer controls
  ON_DepthImage::DEPTH_TEST m_depth_test; // default = ON_DepthImage::DEPTH_TEST::less_passes
  bool m_bTestDepth;                   // default = true
  bool m_bWriteDepth;                  // default = true

  // id buffer controls
  bool m_bWriteId;                     // default = true

  // test triangle status controls
  unsigned int m_test_triangle_status; // default = 0
};

#endif
