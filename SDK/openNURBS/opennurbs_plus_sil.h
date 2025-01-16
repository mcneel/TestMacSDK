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
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_SILHOUETTE_INC_)
#define OPENNURBS_PLUS_SILHOUETTE_INC_

//	The ON_SIL_EVENT class is used to report silhouettes from functions
//  such as ON_Brep::GetSilhouette(..)
class ON_CLASS ON_SIL_EVENT
{
public:
	enum TYPE : unsigned int;
	enum END_TYPE : unsigned int;


  ON_SIL_EVENT() ON_NOEXCEPT;

	// This constructor leaves  m_side_fill[i]=unset but initializes all other fields.
	// The lifetimes of c3d and/or c2d are managed by the ON_SIL_EVENT class. 
	ON_SIL_EVENT(TYPE ttype,  ON_COMPONENT_INDEX cci,
								END_TYPE etype0, ON_COMPONENT_INDEX eci0, double p0,
								END_TYPE etype1, ON_COMPONENT_INDEX eci1, double p1,
								ON_Curve* c3d = nullptr, ON_Curve* c2d = nullptr) ON_NOEXCEPT;
		
  /*
  An ON_SIL_EVENT can own a pair of ON_Curves pointed to by m_curve2d and m_curve3d.
  So the copy constructor and operator= makes copies of these curves.
  And destructor ~ON_SIL_EVENT() deletes these curves.
  */
  ON_SIL_EVENT(const ON_SIL_EVENT& rhs);					
  ON_SIL_EVENT& operator= (const ON_SIL_EVENT& rhs);
	~ON_SIL_EVENT();

#if defined( ON_HAS_RVALUEREF)
	/*
	The move constructor and move assignment operator, moves curves (leaving nullptr's in rhs)
	and copies all other data members.
	*/
  ON_SIL_EVENT(ON_SIL_EVENT&& rhs);								
  ON_SIL_EVENT& operator= (ON_SIL_EVENT&& rhs);		
#endif

  void Dump(ON_TextLog& text_log) const;


	/*
	Description:
	The types are ordered.  The type of a ON_SIL_EVENT is reported as the lowest valued type that applies.
	For example, a boundary that is a tangent silhouette will be reported as kSilTangent type
	not a kSilBoundary.
	*/
	enum TYPE : unsigned int
	{
		kNoSilEvent = 0,
		kSilProjecting = 1,		// boundary of a region that projects to a curve.  
													// The view direction is tangent to the surface over the entire region.
		kSilTanProject = 2,		// tangent silhouette curve where the curve  projects to a point( within tolerance)
													// in this case side_fill[i] is meaningless so it's left unset.
		kSilTangent = 4,			// tangent silhouette curve. 
													// The view direction is tangent to the surface along the curve.

		kSilCrease = 8,				// crease on geometry that is a silhouette
		kSilBoundary = 16,		// boundary of geometry is always a silhouette

		// non_sil_features	
		kNonSilCrease = 256,	// A geometry crease that is not a silhouette
		kNonSilTangent = 512,	// A tangent edge that is not a silhouette
		kNonSilSeam = 1024,		// A surface seam that is not a silhouette

		kSectionCut = 4096,		// Intersection with a clipping plane, not produced by GetSilhouette(...) functions.
		kMiscFeature = 8192, // Miscellaneous curve feature, used for curve objects in TL_HiddenLineDrawing.

		// Draft curves are produced when SetDraftCurve(..)
		// is called on the ON_SilhouetteParameter.
		kDraftCurve = 32768,	// draft curve is a curve of constant draft angle.
	};

	/*
	END_TYPE describes the endpoint of an ON_SIL_EVENT
	*/
	enum END_TYPE : unsigned int
	{
		kEndUnknown = 0,
		kEndBoundary = 1, // end is on geometry boundary
		kEndEdge = 2,		 // end at a brep edge
		kEndLoop = 3,			// TODO?? ( why this and kEndProjected ) end in on a projecting region loop.
		kEndCusp = 4,			//  This end is a silhouette cusp point. (i.e. The projected silhouette has a kink)
		kEndProjected = 5, // ends on a projected region loop					
		kEndSection = 6,	// ends at a section cut
		kEndSngularity = 7, // ends at a point where more than two ON_SIL_EVENTs may join together.
	};


  TYPE m_type;

  /*
  IsSilhouetteType() returns true for types kSilProjecting  through kSilBoundary.
  */
  static bool IsSilhouetteType(TYPE);
  bool IsSilhouetteType() const;

  /*
  IsTangentSilhouetteType() returns true for types kSilProjecting  through kSilTangent.
  */
  static	bool IsTangentSilhouetteType(TYPE); // returns true for kSilProjecting  through kSilTangent
  bool  IsTangentSilhouetteType() const;


	/* m_end_type[0] is the END_TYPE  of the curve start and
		and  m_end_type[1] is the END_TYPE of the curve end.
	*/
  END_TYPE m_end_type[2];	

	// m_ci if valid, is the component index of geometry containing entire silhouette curve.
	// A typical example is ON_SIL_EVENT that starts and ends on ON_BrepEdges and goes across the interior of face.
	// In this case m_ci will be the component index of the face, m_end_ci[0] and  m_end_ci[1] will be the 
	// component indices of the edges on which the ON_SIL_EVENT terminates, m_end_type[0] and  m_end_type[1] will
	// be either END_TYPE::kEndBoundary or END_TYPE::kEndEdge.
  ON_COMPONENT_INDEX m_ci;

	// m_end_ci[0], if valid, is the component index of geometry containing the ON_SIL_EVENT start point.
	// Similarly  m_end_ci[0] applies to the ON_SIL_EVENT end point.
  ON_COMPONENT_INDEX m_end_ci[2];		

	/*
	m_end_parameter[0] is the ON_Brep::Edge parameter of the ON_SIL_EVENT start point when m_end_type[0] =  END_TYPE::kEndBoundary,
	or m_end_type[0] =  END_TYPE::kEndEdge.   m_end_parameter[0] similarly applies to the end point.  
	*/
  double m_end_parameter[2];	

  /*
  When a silhouette is projected on the image plane ( determined by the camera location or direction),
  and projects to a curve (not a point), the image area to the left or right of a projected silhouette
  curve is filled with either the surface or a void.
  */
  enum SIDE_FILL : unsigned int
  {
    kSideUnset = 0,
    kSideSurface = 1,
    kSideVoid = 2,
		kSideOtherSurface = 3			// Not used for silhouette results but for ....
  };

  /*
  m_side_fill[0] defines how the region in the  image plane to the left (or right for m_side_fill[1])
  of the projected silhouette curve is filled.

  Example: generally the surface locally lies to either the left or the right of the projected silhouette curve in the
  image plane.  The basic degenerate cases are that of a projecting surface or the case of a
  silhouette with the surface on both sides of the silhouette locally
						m_side_fill[0]			m_side_fill[1]
  Left Side   side_surface			side_void
  Right				side_void					side_surface

  projecting	 side_void				side_void
  degenerate  side_surface			side_surface
  */
  SIDE_FILL m_side_fill[2];

  // These are deleted by ~ON_SIL_EVENT()
  ON_Curve* m_curve3d;
  ON_Curve* m_curve2d;				// parameter space curve if available or nullptr


};

class ON_CLASS ON_SilhouetteParameters
{
public:
  ON_SilhouetteParameters();
  static const ON_SilhouetteParameters UnsetParameters;


  /* 
	Description:
	 Specify the parameters for calculating parallel view silhouettes.
  Parameters:
		cameraDirection -[in]		3d  camera direction, directed toward the scene.
		tolerance - [in]				A surface which projects in the image plane to within
														tolerance of a curve
														may be reported as a projecting surface.  When in doubt use
														CRhinoDoc::AbsoluteTolerance().
		angleToleranceRadians -[in]   A surface with unit normal N that satisfies
																N o cameraDirection < sin( angleToleranceRadians)
																may be reported as projecting surface.  When in doubt use
																CRhinoDoc::AngleToleranceRadians().
  */
  bool SetParallel(
    ON_3dVector cameraDirection,
    double tolerance,
    double angleToleranceRadians
    );

  /* 
	Description:
		Specify the parameters for calculating perspective view silhouettes.
  Parameters:
		cameraLocation -[in]  3d  camera location
		tolerance - [in]      A surface which projects in the image plane to within tolerance of a curve
													may be treated as projecting surface.  When in doubt use
													CRhinoDoc::AbsoluteTolerance().
		angleToleranceRadians -[in]   A surface with unit normal N that satisfies
													N o cameraDirection < sin( angleToleranceRadians)
													may be considered projecting.  When in doubt use
													CRhinoDoc::AngleToleranceRadians().
  */
  bool SetPerspective(
    ON_3dPoint cameraLocation,
    double tolerance,
    double angleToleranceRadians
    );

  /* 
	Description:
		Specify the parameters for calculating draft curves.  The draft curve separates
		the regions with draft at least equal to the draft angle
		from regions with draft less than the draft angle.
  Parameters:
		pullDirection -[in]      3d  direction for the mold to be pulled in, directed away from the object
		draftAngleRadians -[in]  draft angle can be positive or negative
		tolerance - [in]
		angleToleranceRadians -[in]
  */
  bool SetDraftCurve(
    ON_3dVector pullDirection,
    double draftAngleRadians,
    double tolerance,
    double angleToleranceRadians
    );

  /*
  Description:
		For parallel or perspective view silhouette calculations this type mask
		selects which of the optional ON_SIL_EVENT types are produced by calls
		to the silhouette finder.  Only the following types are optional

		kSilBoundary			- include kSilBoundary events in the results
		kNonSilCrease			- include kNonSilCrease events in the results
		kNonSilTangent  	- includekNonSilTangent events in the results
		kNonSilSeam     	- include kNonSilSeam events in the results

		The default value is	ON_SIL_EVENT::kSilBoundary
  Details:
		For example the Silhouette command in Rhino uses
		kSilBoundary | kNonSilCrease
		So that all boundaries and creased edges are shown.
  */
  unsigned int GetTypeMask() const;
  void SetTypeMask(unsigned int m_mask);
  
  const static unsigned int kAllOptionalTypes = 
    ON_SIL_EVENT::TYPE::kSilBoundary |
    ON_SIL_EVENT::TYPE::kNonSilCrease |
    ON_SIL_EVENT::TYPE::kNonSilTangent |
    ON_SIL_EVENT::TYPE::kNonSilSeam;


  enum SilhouetteTypes : unsigned int
  {
    kUnsetSilhouetteType = 0,
    kParallelProjection = 1,
    kPerspectiveProjection = 2,
    kDraftAngle = 3
  };

  /*
  Description:
	 Returns the silhouette type setting.
  */
  SilhouetteTypes SilhouetteType() const;

  double Tolerance() const;
  double AngleToleranceRadians() const;


  /*
  Returns:
		If SilhouetteType() is 	kParallelProjection then the function
		returns the unit camera direction (directed toward the scene),
		otherwise the function returns ON_3dVector::UnsetVector.
  */
  ON_3dVector ParallelCameraDirection() const;

  /*
  Returns:
		If SilhouetteType() is 	kPerspectiveProjection then
		the function returns the camera location
		otherwise the function returns ON_3dPoint::UnsetVector.
  */
  ON_3dPoint PerspectiveCameraLocation() const;

  /*
  Returns:
		If SilhouetteType() is 	kDraftAngle then the function
		returns the pull direction (directed away from the object),
		otherwise the function returns ON_3dVector::UnsetVector.
  */
  ON_3dVector DraftPullDirection() const;

  /*
  Returns:
		If SilhouetteType() is 	kDraftAngle then the function
		returns the draft angle setting,
		otherwise the function returns ON_UNSET_VALUE.
  */
  double DraftAngleRadians() const;


  /*
  Description:
		Find the unit view direction from the camera to the given
		3dPoint P0.
  Parameters:
		P0 - [in]
	Details:
		For parallel views ViewDirection does not depend on P0.
  */
  ON_3dVector ViewDirection(const ON_3dPoint& P0) const;

private:
  SilhouetteTypes m_type;
  ON_3dPoint m_CameraLocation;
  ON_3dVector m_CameraDirection;
  double m_DraftAngleRadians;
  double m_Tolerance;
  double m_AngleToleranceRadians;
  unsigned int m_type_mask;
};

#endif
