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

#if !defined(ON_PLUS_MASSPROP_INC_)
#define ON_PLUS_MASSPROP_INC_

/*
Description:
  This class is used to returned results of 
  mass properties calculations.

Note:
  The constructor for this class is meaningful and
  initializes m_mass_type to ON_UNSET_INT_INDEX.
  Users of arrays of this class should use ON_ClassArray.
  If you insist on using ON_SimpleArray<ON_MassProperties>,
  be sure to properly initialize all elements using the
  explicit constructor or Create(). Elements created with
  AppendNew() need a call to Create()!
  See RH-68376 for details.
*/
class ON_CLASS ON_MassProperties
{
public:
  ON_MassProperties();
  ~ON_MassProperties();

  // Set values to constructor defaults.
  void Create();

  /*
  Description:
    Prints a text description of the mass properties
    information.
  Parameters:
    dump - [in] target for text dump information.
  */
  void Dump( ON_TextLog& dump ) const;


  /*
  Description:
    Sum a list of mass properties together to get
    an aggregate mass.
  Parameters:
    count - [in] number of elements in the summands array.
    summands - [in] array of mass properties to add
    bAddTo - [in] if true, then the summands are added to
                  the existing mass.
  Returns:
    True if successful.
  */
  bool Sum(
    int count,
    const ON_MassProperties* summands,
    bool bAddTo = false
    );

	/*
	Description:
		On input *this contains the mass properties for some geometry G,
		On exit  *this contains the mass properties for the transformed 
		geometry X(G).   
	Parameters:
		X - [in]  When m_mass_type==3 ( for a solid)  X must be an Affine transformation.	
							When m_mass_type==2 ( for a surface)  X must be a similarity transformation  
						  (see ON_Xform.IsAffine(), and ON_Xform::IsSimilarity())
						For example perspective transformations are not allowed. 
	Returns:
		True if successful.
	Details:
		The Area of a transformed object can not be calculated from the area of the original object
		if the transform is not a similarity, like a non-uniform scaling.
	*/
	bool Transform(const ON_Xform& X);

  // Determines what type of "mass" is returned.
  // ON_UNSET_INT_INDEX = unset, 0 = point masses, 1 = length, 2 = area, 3 = volume
  int m_mass_type;
  int m__reserved;

  // These flags are set to true if the calcluation was
  // successful.  If a flag is false, the corresponding
  // values should not be used.
  bool m_bValidMass;
  bool m_bValidCentroid;
  bool m_bValidFirstMoments;
  bool m_bValidSecondMoments;
  bool m_bValidProductMoments;
  bool m__bReserved1;
  bool m__bReserved2;
  bool m__bReserved3;



  // MASS - count, length, area, or volume of object
  //      = integral of "dm" over the object
  double m_mass;
  double m_mass_err; // uncertainty in calculation

  // CENTROID - in world coordinate system
  double m_x0;      // = m_world_x / m_mass
  double m_x0_err;
  double m_y0;      // = m_world_y / m_mass
  double m_y0_err;
  double m_z0;      // = m_world_z / m_mass
  double m_z0_err;




  // FIRST MOMENTS - with respect to world coordinate system
  // integral of "x dm" over the length/area/volume
  double m_world_x;
  double m_world_x_err; // uncertainty in calculation

  // integral of "y dm" over the length/area/volume
  double m_world_y;
  double m_world_y_err; // uncertainty in calculation

  // integral of "z dm" over the length/area/volume
  double m_world_z;
  double m_world_z_err; // uncertainty in calculation

  // SECOND MOMENTS - with respect to world coordinate system
  // integral of "xx dm" over the length/area/volume
  double m_world_xx;
  double m_world_xx_err; // uncertainty in calculation
  // integral of "yy dm" over the length/area/volume
  double m_world_yy;
  double m_world_yy_err; // uncertainty in calculation
  // integral of "zz dm" over the length/area/volume
  double m_world_zz;
  double m_world_zz_err; // uncertainty in calculation

  // PRODUCT MOMENTS - with respect to world coordinate system
  // integral of "xy dm" over the length/area/volume
  double m_world_xy;
  double m_world_xy_err; // uncertainty in calculation
  // integral of "yz dm" over the length/area/volume
  double m_world_yz;
  double m_world_yz_err; // uncertainty in calculation
  // integral of "zx dm" over the length/area/volume
  double m_world_zx;
  double m_world_zx_err; // uncertainty in calculation



  
  // The "centroid coordinate system" (ccs) is the
  // coordinate system with origin at the centroid and
  // axes parallel to the world coordinate axes.

  // SECOND MOMENTS - with respect to centroid coordinate system
  // integral of "xx dm" over the length/area/volume
  double m_ccs_xx;
  double m_ccs_xx_err; // uncertainty in calculation
  // integral of "yy dm" over the length/area/volume
  double m_ccs_yy;
  double m_ccs_yy_err; // uncertainty in calculation
  // integral of "zz dm" over the length/area/volume
  double m_ccs_zz;
  double m_ccs_zz_err; // uncertainty in calculation

  // PRODUCT MOMENTS - with respect to centroid coordinate system
  // integral of "xy dm" over the length/area/volume
  double m_ccs_xy;
  double m_ccs_xy_err; // uncertainty in calculation
  // integral of "yz dm" over the length/area/volume
  double m_ccs_yz;
  double m_ccs_yz_err; // uncertainty in calculation
  // integral of "zx dm" over the length/area/volume
  double m_ccs_zx;
  double m_ccs_zx_err; // uncertainty in calculation


  double m__reserved1;
  double m__reserved2;
  double m__reserved3;
  double m__reserved4;
  double m__reserved5;
  double m__reserved6;
  double m__reserved7;
  double m__reserved8;


  // FUNCTIONAL INTERFACE for those who wish to ignore error terms.

  /*
  Returns:
    Centroid of object.
  */
  ON_3dPoint Centroid() const;

  /*
  Description:
    Returns count of points if the mass properties
    calculation was a pointmass calculation.
  Returns:
    Count of points if m_mass_type = 0 and zero
    otherwise.
  */
  double Count() const;

  /*
  Description:
    Returns length of the curve if the mass properties
    calculation was a length calculation.
  Returns:
    Length of the curve if m_mass_type = 1 and zero
    otherwise.
  */
  double Length() const;

  /*
  Description:
    Returns area of the surface or mesh if the mass properties
    calculation was an area calculation.
  Returns:
    Area of the surface or mesh if m_mass_type = 2 and zero
    otherwise.
  */
  double Area() const;

  /*
  Description:
    Returns volume of the solid if the mass properties
    calculation was a volume calculation.
  Returns:
    Volume of the solid if m_mass_type = 3 and zero
    otherwise.
  */
  double Volume() const;

  /*
  Description:
    If m_bValidFirstMoments is true, then the world
    coordinate first moments are returned in a vector
    (m_world_x,m_world_y,m_world_z)
    If m_bValidFirstMoments is false, then (0,0,0) is
    returned.
  Returns:
    First moments with respect to world coordinate system.
    WorldCoordFirstMoments().x = integral of x dm
    WorldCoordFirstMoments().y = integral of y dm
    WorldCoordFirstMoments().z = integral of z dm
  */
  ON_3dVector WorldCoordFirstMoments() const;

  /*
  Description:
    If m_bValidSecondMoments is true, then the world
    coordinate second moments are returned in a vector
    (m_world_xx,m_world_yy,m_world_zz)
    If m_bValidSecondMoments is false, then (0,0,0) is
    returned.
  Returns:
    Second moments with respect to world coordinate system.
    WorldCoordSecondMoments().x = integral of x^2 dm
    WorldCoordSecondMoments().y = integral of y^2 dm
    WorldCoordSecondMoments().z = integral of z^2 dm
  See Also:
    CentroidCoordSecondMoments
  */
  ON_3dVector WorldCoordSecondMoments() const;

  /*
  Description:
    Calculates the moments of inertia about world
    coordinate axes.  
  Returns:
    Moments of inertia with respect to world coordinate system.
    WorldCoordMomentsOfInertia().x = integral of (y^2 + z^2) dm
    WorldCoordMomentsOfInertia().y = integral of (z^2 + x^2) dm
    WorldCoordMomentsOfInertia().z = integral of (x^2 + y^2) dm
  Remarks:
    What is meant by "moments of inertia" varies widely in 
    textbooks and papers.  The values returned here
    are the integrals listed in the Returns section.
    Some applications may want the values from
    WorldCoordSecondMoments() instead of the values
    returned here.
  */
  ON_3dVector WorldCoordMomentsOfInertia() const;

  /*
  Description:
    Calculates the radii of gyration about world
    coordinate axes.
  Returns:
    Radii of gyration with respect to world coordinate system.
    WorldCoordRadiiOfGyration().x = sqrt(integral of (y^2 + z^2) dm/M)
    WorldCoordRadiiOfGyration().y = sqrt(integral of (z^2 + x^2) dm/M)
    WorldCoordRadiiOfGyration().z = sqrt(integral of (z^2 + y^2) dm/M)
  Remarks:
    What is meant by "radii of gyration" varies widely in 
    textbooks and papers.  The values returned here
    are the integrals listed in the Returns section.
  */
  ON_3dVector WorldCoordRadiiOfGyration() const;

  /*
  WARNING
    The matrix returned by this function is not an Inertia Matrix.
    So this function is being made obsolete 
  Description:
    If moments are valid, then the 3x3 inertia matrix with respect
    to world coordinates is returned.  This matrix is sometimes
    called the "inertia tensor".
  Parameters:
    martix - [in] If you want to fill in an existing matrix, 
                  pass a pointer to that matrix.  Otherwise
                  a matrix will be created and returned.
    returned.
  Returns:
    The inertia matrix or nullptr if the moments are not valid.

 
  See Also:
    CentroidCoordIntertiaMatrix
  */
  ON_DEPRECATED
  ON_Matrix* WorldCoordIntertiaMatrix( 
                ON_Matrix* matrix = nullptr 
                ) const;

  /*
   WARNING
     NOT PRINCIPAL MOMENTS of INERTIA!!!
     consider using  WorldCoordPrincipalMomentsOfInertia.
  Description:
    Calculates the eigenvalues and eigenvectors of moments matrix
    in world coordinates. 
          m_world_xx  m_world_xy  m_world_xz
          m_world_xy  m_world_yy  m_world_yz
          m_world_xz  m_world_yz  m_world_zz

  Parameters:
    pxx - [out] principal moment
    Ax - [out] principal axis for pxx
    pyy - [out] principal moment
    Ay - [out] principal axis for pyy
    pzz - [out] principal moment
    Az - [out] principal axis for pzz
  Returns:
    True if successful.
  See Also:
    CentroidCoordPrincipalMoments
  */
  bool WorldCoordPrincipalMoments( 
                double* pxx, ON_3dVector& Ax,
                double* pyy, ON_3dVector& Ay,
                double* pzz, ON_3dVector& Az
                ) const;


  /*
  Description:
    Calculates the principal moments of inertia and principal
    axes with respect to world coordinates.
  Parameters:
    pxx - [out] principal moment
    Ax - [out] principal axis for pxx
    pyy - [out] principal moment
    Ay - [out] principal axis for pyy
    pzz - [out] principal moment
    Az - [out] principal axis for pzz
  Returns:
    True if successful.
  See Also:
    WorldCoordPrincipalMoments
  */
  bool WorldCoordPrincipalMomentsOfInertia(
    double* pxx, ON_3dVector& Ax,
    double* pyy, ON_3dVector& Ay,
    double* pzz, ON_3dVector& Az
  ) const;

  /*
  Description:
    If m_bValidSecondMoments is true, then the centroid
    coordinate second moments are returned in a vector
    (m_ccs_xx,m_ccs_yy,m_ccs_zz)
    If m_bValidSecondMoments is false, then (0,0,0) is
    returned.
  Returns:
    Second moments with respect to centroid coordinate system.
    CentroidCoordSecondMoments().x = integral of (x-x0)^2 dm
    CentroidCoordSecondMoments().y = integral of (y-y0)^2 dm
    CentroidCoordSecondMoments().z = integral of (z-z0)^2 dm
    where (x0,y0,z0) = centroid.
  See Also:
    WorldCoordSecondMoments
  */
  ON_3dVector CentroidCoordSecondMoments() const;

  /*
  Description:
    Calculates the moments of inertia about centroid
    coordinate axes.  
  Returns:
    Moments of inertia with respect to centroid coordinate system.
    WorldCoordMomentsOfInertia().x = integral of ((y-y0)^2 + (z-z0)^2) dm
    WorldCoordMomentsOfInertia().y = integral of ((z-z0)^2 + (x-x0)^2) dm
    WorldCoordMomentsOfInertia().z = integral of ((x-x0)^2 + (y-y0)^2) dm
    where (x0,y0,z0) = centroid.
  Remarks:
    What is meant by "moments of inertia" varies widely in 
    textbooks and papers.  The values returned here
    are the integrals listed in the Returns section.
    Some applications may want the values from
    WorldCoordSecondMoments() instead of the values
    returned here.
  */
  ON_3dVector CentroidCoordMomentsOfInertia() const;

  /*
  Description:
    Calculates the radii of gyration about centroid
    coordinate axes.
  Returns:
    Radii of gyration with respect to centroid coordinate system.
    CentroidCoordRadiiOfGyration().x 
      = sqrt(integral of ((y-y0)^2 + (z-z0)^2) dm/M)
    CentroidCoordRadiiOfGyration().y 
      = sqrt(integral of ((z-z0)^2 + (x-x0)^2) dm/M)
    CentroidCoordRadiiOfGyration().z 
      = sqrt(integral of ((z-z0)^2 + (y-y0)^2) dm/M)
    where (x0,y0,z0) = centroid.
  Remarks:
    What is meant by "radii of gyration" varies widely in 
    textbooks and papers.  The values returned here
    are the integrals listed in the Returns section.
  */
  ON_3dVector CentroidCoordRadiiOfGyration() const;

  /*
    WARNING
      The function returned by this function is not an
      inertial matrix. So its been deprecated.
  Description:
    If moments are valid, then the 3x3 moment matrix with respect
    to centroid coordinates is returned.  
  Parameters:
    martix - [in] If you want to fill in an existing matrix, 
                  pass a pointer to that matrix.  Otherwise
                  a matrix will be created and returned.
    returned.
  Returns:
    The moment matrix or nullptr if the moments are not valid.

          m_ccs_xx  m_ccs_xy  m_ccs_xz
          m_ccs_xy  m_ccs_yy  m_ccs_yz
          m_ccs_xz  m_ccs_yz  m_ccs_zz
  See Also:
    WorldCoordIntertiaMatrix
  */
  ON_DEPRECATED
  ON_Matrix* CentroidCoordIntertiaMatrix( 
                ON_Matrix* matrix = nullptr 
                ) const;


  /*
  *  WARNING
     NOT PRINCIPAL MOMENTS of INERTIA!!!
     consider using  CentroidCoordPrincipalMomentsOfInertia.
   Description:
     Calculates the eigenvalues and eigenvectors of the moments
     matrix with respect to centroid coordinates.
          m_ccs_xx  m_ccs_xy  m_ccs_xz
          m_ccs_xy  m_ccs_yy  m_ccs_yz
          m_ccs_xz  m_ccs_yz  m_ccs_zz
   Parameters:
     pxx - [out] principal moment
     Ax - [out] principal axis for pxx
     pyy - [out] principal moment
     Ay - [out] principal axis for pyy
     pzz - [out] principal moment
     Az - [out] principal axis for pzz
   Returns:
     True if successful.
   See Also:
     WorldCoordPrincipalMoments
   */
  bool CentroidCoordPrincipalMoments(
    double* pxx, ON_3dVector& Ax,
    double* pyy, ON_3dVector& Ay,
    double* pzz, ON_3dVector& Az
  ) const;

  /*
  Description:
    Calculates the principal moments of inertia and principal
    axes with respect to centroid coordinates.  
  Parameters:
    pxx - [out] principal moment
    Ax - [out] principal axis for pxx
    pyy - [out] principal moment
    Ay - [out] principal axis for pyy
    pzz - [out] principal moment
    Az - [out] principal axis for pzz
  Returns:
    True if successful.
  See Also:
    WorldCoordPrincipalMoments
  */
  bool CentroidCoordPrincipalMomentsOfInertia( 
                double* pxx, ON_3dVector& Ax,
                double* pyy, ON_3dVector& Ay,
                double* pzz, ON_3dVector& Az
                ) const;

private:
	/*
	Description
		Calculates centroid ( m_x0, m_y0, m_z0) and centroid error ( m_x0_err, m_y0_err, m_z0_err)
		from mass and first moment information.
	Returns
		True if mass and first moment are valid
	*/
	bool SetCentroid();				

};

#endif
