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

#if !defined(OPENNURBS_PLUS_SECTIONS_INC_)
#define OPENNURBS_PLUS_SECTIONS_INC_


/*
  The class ON_SectionAnalysisPlanes defines a set
  of parallel planes that are commonly used to 
  analyze the geometry of a surface by viewing the
  intersections and sometimes the sectional curvature
  of the sections.
*/
class ON_CLASS ON_SectionAnalysisPlane
{
public:
  static const ON_SectionAnalysisPlane Default;

  ON_SectionAnalysisPlane();
  // The default copy constructor, operator=, and destructor
  // work fine.
  
  // Applications can use m_section_plane_id to track section
  // plane appearance settings. The default constructor sets 
  // m_section_plane_id = ON_nil_id and nothing in opennurbs
  // changes this value.
  //
  // Rhino uses this value to detect when 
  // ON_PlanarSection information needs to be updated.
  ON_UUID m_section_plane_id;

  // Applications can use m_section_plane_mark for any purpose.
  // The default constructor sets this value to 0 and nothing
  // in opennurbs changes this value.
  //
  // Rhino saves a time() value in m_section_plane_mark and
  // uses it to detect when ON_PlanarSection information needs to 
  // be updated.
  ON__UINT64 m_section_plane_mark;

  // Compares all field values except m_section_plane_id and m_section_plane_mark.
  bool operator==(const ON_SectionAnalysisPlane& other) const;

  // Compares all field values except m_section_plane_id and m_section_plane_mark.
  bool operator!=(const ON_SectionAnalysisPlane& other) const;
  
  
  // The "base plane" equation is stored in m_plane_equation.
  //
  // If m_offset_spacing != 0, then sections are
  // calculated at the planes
  //   a*x + b*y + c*z + (d - n*m_plane_delta),
  // where n is an integer value. (The value n*m_plane_delta
  // is subtracted so that positive values of n*m_plane_delta
  // move the plane in the direction of the plane's normal
  // vector and programmers who are not familiar with translating
  // plane equations are more likely to get what they expect.)
  //
  // If m_plane_delta_limits[0] < m_plane_delta_limits[1], then
  // n is restricted to the range,
  // m_plane_delta_limits[0] <= n < m_plane_delta_limits[1].  
  // Otherwise all values of n are used and m_max_plane_count
  // is used to limit the maximum number of section planes.
  //
  // Observe that m_plane_delta is a 3d distance between
  // the planes only when (x,y,z) is a unit vector.
  //
  // If m_plane_delta_limits[0] <= m_plane_delta_limits[1]
  // and m_plane_delta > 0, then m_max_plane_count is used
  // to limit the number of section planes. 
  // If m_max_plane_count = 0, then at most 100 section planes
  // will be calculated for any one object. 
  // If m_max_plane_count > 0, then the value of m_max_plane_count
  // will be used to limit the number of section planes for
  // any one object.
  //
  // Use ON_SectionAnalysisPlane::GetPlaneIndices() to easily
  // get a finite list of section planes that intersect a 
  // bounding box.
  ON_PlaneEquation m_plane_equation;
  double m_plane_delta;
  int m_plane_delta_limits[2];
  int m_max_plane_count;
private:
  int m_reserved1;
public:

  /*
  Description:
    This function gets the plane indices for planes in this
    set of sectional analysis planes that will intersect 
    the input bounding box.

  Example:
        int plane_index0 = 0;
        int plane_index1 = 0;
        GetPlaneIndices(bbox,tolerance,&plane_index0,&plane_index1);
        for ( int i = plane_index0; i < plane_index1; i++ )
        {
          ON_PlaneEquation pe(m_plane_equation);
          pe.d += i*m_plane_delta;
        }
  */
  bool GetPlaneIndices( 
    ON_BoundingBox bbox, 
    double tolerance, 
    int* plane_index0,
    int* plane_index1 
    ) const;

  bool GetPlaneEquation(
    int plane_index,
    ON_PlaneEquation& plane_equation
    ) const;

public:

  // The appearance of the contours in the Rhino user interface
  // is controlled by the values in a CRhinoSectionAnalysisAppearanceSettings with
  // matching id.
  ON_UUID m_section_appearance_id;

  // Fields may be be added to this class during the V5 lifecycle.
  // If that happens, then m_class_version will be incremented.
  unsigned char m_class_version; // constructor sets m_class_version = 0;
private:
  unsigned char m_reserved2[3];
  unsigned int m_reserved3;
  double m_reserved4[4];
};

class ON_CLASS ON_SectionAnalysisAppearance
{
public:
  static const ON_SectionAnalysisAppearance Default;

  ON_SectionAnalysisAppearance();
  // The default copy constructor, operator=, and destructor
  // work fine.
  
  // Applications can use m_section_appearance_id to track section
  // plane appearance settings. The default constructor sets 
  // m_section_appearance_id = ON_nil_id and nothing in opennurbs
  // changes this value.
  //
  // Rhino uses this value to detect when 
  // ON_PlanarSection.m_section_appearance needs to
  // be updated.
  ON_UUID m_section_appearance_id;

  // Applications can use m_update_mark for any purpose.
  // The default constructor sets this value to 0 and nothing
  // in opennurbs changes this value.
  //
  // Rhino saves a time() value in m_section_appearance_mark and
  // uses it to detect when ON_PlanarSection information needs to 
  // be updated.
  ON__UINT64 m_section_appearance_mark;

  // Compares every value except m_section_appearance_id and m_section_appearance_mark.
  bool operator==(const ON_SectionAnalysisAppearance& other) const;

  // Compares every value except m_section_appearance_id and m_section_appearance_mark.
  bool operator!=(const ON_SectionAnalysisAppearance& other) const;

  /*
  Returns:
    !m_bHideIsoCurvesWhenSectionsAreVisible 
    || (!m_bShowSectionCurve && !m_bShowSectionHair)
  */
  bool ShowIsoCurves() const;

  // true to hide the iso-curves that are commonly
  // drawn on surfaces when section curves are
  // visible.
  bool m_bHideIsoCurvesWhenSectionsAreVisible;

  // true to show the curve on the surface
  bool m_bShowSectionCurve;

  // Sectional curvature hair is visible when m_bShowSectionHair
  // is true and m_hair_spacing > 0.0
  bool m_bShowSectionHair;

private:
  bool m_reserved1[5];
public:

  /*
  Description:
    Sets both m_curve_color and  m_hair_color to the
    specified color.
  */
  void SetAllColors( ON_Color color );

  // The color of the section curve
  ON_Color m_curve_color;

  // The sectional curvature "hair" color
  ON_Color m_hair_color;

  // The frequency of curvature hair in a sectional curvature
  // display is controlled by two parameters,
  // m_hair_spacing and m_hair_angle_radians.
  // If m_hair_spacing > 0.0, then the maximum distance
  // between hairs along a sectional curve will be at
  // most m_hair_spacing.
  // If m_hair_angle_radians > 0.0, then the maximum angle
  // between adjacent curvature hairs will be around
  // m_hair_angle_radians.
  double m_hair_spacing;
  double m_hair_angle_radians;

  /*
  Returns:
    (m_hair_scale > 0.0) ? m_hair_scale : 1.0.
  */
  double HairScale() const;
  // If m_hair_scale > 0.0, when curvature hair is drawn, 
  // its length is scaled by this factor.

  /*
  Description:
    This function provides a good way to use a linear
    user interface control to set m_hair_scale.
  Parameter:
    ui_value - [in]
      values from -10 to +10 make the most sense.
       0: true length
      >0 lengthen hair
      <0 shorten hair
  Returns:
    2^(ui_value/2.0)
  See Also:
    ON_SectionAnalysisAppearance::HairScaleUserInterfaceValue()
  */
  static double HairScaleFromUserInterfaceValue( int ui_value );

  /*
  Returns:
    User interface value corresponding to the current value
    of HairScale();
  See Also:
    ON_SectionAnalysisAppearance::HairScaleFromUserInterfaceValue()
  */
  static int HairScaleUserInterfaceValue(double hair_scale);

  double m_hair_scale;

  // Fields may be be added to this class during the V5 lifecycle.
  // If that happens, then m_class_version will be incremented.
  unsigned char m_class_version; // constructor sets m_class_version = 0;
private:
  unsigned char m_reserved2[3];
  unsigned int m_reserved3;
  double m_reserved4[4];
};

class ON_CLASS ON_PlanarSection
{
public:
  ON_PlanarSection();
  ~ON_PlanarSection();
  ON_PlanarSection(const ON_PlanarSection&src);
  ON_PlanarSection& operator=(const ON_PlanarSection&src);

  // A copy of the appearance settings is stored here so
  // the values can be quickly found.  Use
  // ON_SectionAnalysisAppearance.m_update_mark to efficiently
  // detect changes and update these cached settings.
  ON_SectionAnalysisAppearance m_appearance;

  // ON_SectionAnalysisPlane identification.
  ON_UUID m_section_plane_id;
  ON__UINT64 m_section_plane_mark;

  // Equation of the plane used to calculate the section
  // information below.
  ON_PlaneEquation m_plane_eqn;

  // 3d points on the section curve
  ON_SimpleArray<ON_3dPoint> m_points;

  // Surface sectional curvature (when available)
  ON_SimpleArray<ON_3dVector> m_curvatures;

  // Surface parameters (when available)
  ON_SimpleArray<ON_2dPoint> m_2dpoints;

  // A 3d curve that approximates the intersection
  // In general, the curvature of m_3dcurve is a
  // poor approximation of the surface's sectional
  // curvature. (Often null)
  ON_Curve* m_3dcurve;

  // A 2d curve that approximates the intersection
  // in the surface's parameter space. (Often null)
  ON_Curve* m_2dcurve;

  /*
  Description:
    Evaluate the surface at the m_2dpoints[] values to set
    the m_points[] and m_curvatures[] values.
  Parameters:
    surface - [in]
      The input values of m_appearance, m_plane_eqn 
      and m_m2d_points are used to calculate the 
      3d planar section values and these are returned in
      m_points[] and m_curvatures.  If the appearance
      values indicate the 2d segments need to be refined,
      then m_2dpoints[] is updated as well.
  */
  void EvaluateSurface( 
    const class ON_Surface* surface
    );
  
  // Fields may be be added to this class during the V5 lifecycle.
  // If that happens, then m_class_version will be incremented.
  const unsigned char m_class_version; // constructor sets m_class_version = 0;
private:
  unsigned char m_reserved1[3];
  unsigned int m_reserved2;
  double m_reserved3[4];
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray< const class ON_PlanarSection* >;
#endif


class ON_CLASS ON_PlanarSections
{
public:

  /*
  Description:
    Calculate and attach planar sections to object.
  Parameters:
    object - [in]
      The section information will be attached to object.
      Typically, object will be mxp.m_mesh or the surface, brep
      face or brep used to create the mesh, but you can attach
      the sections to any class derived from ON_Object.
    mxp - [in]
      a mesh and a plane to intersect.
    srf - [in]
      If srf is not null, then msp.m_mesh must be an approximation
      of srf and mxp.m_mesh.m_S[] must be parameters from srf.  srf
      will be used to calculate sectional curvature information.
      Often, mxp.m_mesh is a mesh of a brep face and srf is the
      underlying surface.
    appearance - [in]
    plane - [in]
  */
  static bool AttachToObject( 
    const class ON_Object& object,
    class ON_MeshXPlane& mxp,
    const class ON_Surface* srf,
    const class ON_SectionAnalysisAppearance& appearance,
    const class ON_SectionAnalysisPlane& plane
    );
  static bool AttachToObject( 
    const class ON_Object* object,
    class ON_MeshXPlane& mxp,
    const class ON_Surface* srf,
    const class ON_SectionAnalysisAppearance& appearance,
    const class ON_SectionAnalysisPlane& plane
    );

  /*
  Description:
    Gets any planar sections that are attached to obj.
  Parameters:
    obj - [in]
  */
  static const ON_PlanarSections* Get( const class ON_Object& obj );
  static const ON_PlanarSections* Get( const class ON_Object* obj );

  /*
  Description:
    Destroys all planar sections that are attached to obj.
  Parameters:
    obj - [in]
  */
  static void RemoveFromObject( const class ON_Object& obj );
  static void RemoveFromObject( const class ON_Object* obj );

  void DeleteSectionsWithSectionPlaneId( ON_UUID m_section_plane_id );
  void DeleteSectionsWithSectionAppearanceId( ON_UUID m_section_appearance_id );

  ON_PlanarSections();
  ~ON_PlanarSections();
  ON_PlanarSections(const ON_PlanarSections& src);
  ON_PlanarSections& operator=(const ON_PlanarSections& src);

  /*
  Description:
    Calculate and append planar sections to m_sections[].
  Parameters:
    mxp - [in]
    srf - [in]
      If srf is not null, then msp.m_mesh must be an approximation
      of srf and mxp.m_mesh.m_S[] must be parameters from srf.
    appearance - [in]
    plane - [in]
  */
  bool Append(
    class ON_MeshXPlane& mxp,
    const class ON_Surface* srf,
    const class ON_SectionAnalysisAppearance& appearance,
    const class ON_SectionAnalysisPlane& plane
    );

  ON__UINT64 m_section_appearances_mark;
  ON__UINT64 m_section_planes_mark;
  ON_SimpleArray< const class ON_PlanarSection* > m_sections;

  // Fields may be be added to this class during the V5 lifecycle.
  // If that happens, then m_class_version will be incremented.
  const unsigned char m_class_version; // constructor sets m_class_version = 0;
private:
   unsigned char m_reserved1[3];
   unsigned int m_reserved2;
   double m_reserved3[4];
};

/*
Description:
  Evaluate the surface to set the ps.m_points[] and ps.m_curvatures[]
  values.
*/
ON_DECL
void ON_EvaluateSurfaceSectionalCurvature( 
  const class ON_Surface* surface,
  const class ON_SectionAnalysisAppearance& appearance,
  ON_PlaneEquation plane_eqn,
  unsigned int uv_point_count,
  size_t uv_point_stride,
  const double* uv_points,
  ON_SimpleArray<ON_3dPoint>& points,
  ON_SimpleArray<ON_3dVector>& curvatures,
  ON_SimpleArray<ON_2dPoint>* refined_uv_points
  );

#endif
