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

#include "rhinoSdkObject.h"

#include <vector>


class RHINO_SDK_CLASS CRhinoMeshObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoMeshObject);

public:

  /*
  Returns:
    ON::mesh_object
  */
  ON::object_type ObjectType() const override;

  CRhinoMeshObject();                              // assigns a new object UUID
  CRhinoMeshObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoMeshObject();
  CRhinoMeshObject( const CRhinoMeshObject& );          // copies source object UUID
  CRhinoMeshObject& operator=(const CRhinoMeshObject&); // copies source object UUID

  void SetMesh( ON_Mesh* ); // ~CRhinoMeshObject() will delete this mesh
  void SetMesh(std::shared_ptr<const ON_Mesh>);
  const ON_Mesh* Mesh() const;

  //Prefer this to Mesh()
  std::shared_ptr<const ON_Mesh> SharedMesh() const;

	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  // virtual overrides to support subobject selection
protected:
  int SubObjectsAreSelected() const override;
  int SubObjectsAreHighlighted() const override;

public:
  int UnselectAllSubObjects() override;
  int UnhighlightAllSubObjects() override;
  bool HighlightRequiresRedraw() const override;
  bool IsSubObjectSelected( ON_COMPONENT_INDEX component_index ) const override;
  int GetSelectedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const override;
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX component_index) const override;
  int GetHighlightedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const override;
  bool HighlightSubObject(ON_COMPONENT_INDEX component_index, bool bHighlight=true) const override;
  int SelectSubObject(
        ON_COMPONENT_INDEX component_index,
        bool bSelect = true,
        bool bSynchHighlight = true,
        bool bPersistentSelect = false
        ) const override;

  int GetWireframeCurves( ON_SimpleArray<ON_Curve*>& curve_list ) const override;

  void EnableGrips( bool bGripsOn ) override;

  // virtual CRhinoObject::DrawV6 override
  virtual
  void DrawV6(
    class CRhinoObjectDrawContext* draw_context
    ) const RHINO_NOEXCEPT override;

  void Draw(CRhinoDisplayPipeline& dp) const override;
  
  void DrawSubObject(CRhinoDisplayPipeline&,ON_COMPONENT_INDEX) const override;

  void DrawHighlightedSubObjects(CRhinoDisplayPipeline&) const override;

  bool SetTextureCoordinates( 
         const ON_TextureMapping& mapping,
         const ON_Xform* object_xform,
         bool bLazy = true
         ) const override;

  bool PickFilter( const CRhinoGetObject&, bool ) const override;

  int Pick( const CRhinoPickContext&, CRhinoObjRefArray& ) const override;

  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const override;

  bool IsMeshable( ON::mesh_type) const override;
  int MeshCount( ON::mesh_type mesh_type, const ON_MeshParameters* mp = 0 ) const override;
  int CreateMeshes( ON::mesh_type, const ON_MeshParameters&, bool bIgnoreCustom=false ) override;
  void DestroyMeshes( ON::mesh_type mesh_type, bool bDeleteMeshes = true ) override;
  int GetMeshes( ON::mesh_type, ON_SimpleArray<const ON_Mesh *>& ) const override;

  //Servicing the fake-virtual function in CRhinoObject
  std::vector<std::shared_ptr<const ON_Mesh>> GetMeshes(ON::mesh_type mesh_type) const /*override*/;

  bool IsSolid() const override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

  //// virtual CRhinoObject override
  //bool EnableAnalysisMode( ANALYSIS_MODE am, bool bEnable = true ) const;

  //// virtual CRhinoObject override
  //bool SupportsAnalysisMode( ANALYSIS_MODE ) const;

  /*
  Returns:
    "mesh" or "meshes"
  */
  const wchar_t* ShortDescription( bool bPlural ) const override;

  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const CRhinoViewport* pViewport = nullptr
          ) const override;

};

// Description:
//   CRhinoMeshDensity determines the number of quads created
//   by the RhinoMeshPlane, RhinoMeshBox, and RhinoMeshSphere
//   commands.
class RHINO_SDK_CLASS CRhinoMeshDensity
{
public:
  CRhinoMeshDensity();

  // Returns:
  //   true if settings are valid
  BOOL32 IsValid() const;
  // boxes and planes.
  int m_x; // (>=1) number of quads in "x" direction
  int m_y; // (>=1) number of quads in "y" direction
  int m_z; // (>=1) number of quads in "z" direction
  // spheres, cylinders, and cones
  int m_around;   // (>=3) number of quads in latitude direction
  int m_vertical; // (>=2) number of quads in longitude direction

  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
};

// Description:
//   Create a mesh face from 3 or 4 points.
RHINO_SDK_FUNCTION
bool RhinoMeshFace( 
						ON_Mesh& mesh, 
						const ON_SimpleArray <ON_3dPoint>& points );

// Description:
//   Create a mesh sphere.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshSphere( 
            const ON_Sphere& sphere, 
            const CRhinoMeshDensity& mesh_density );

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshSphere( 
            const ON_Sphere& sphere, 
            int vertical, int around );

// Description:
//   Create a mesh sphere.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCylinder( 
            const ON_Cylinder& cylinder, 
            const CRhinoMeshDensity& mesh_density );

// Remarks: bQuadCaps is ignored if around is not an even number.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCylinder(
  const ON_Cylinder& cylinder,
  const CRhinoMeshDensity& mesh_density, bool bQuadCaps);

// Remarks: bQuadCaps is ignored if around is not an even number.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCylinder(
  const ON_Cylinder& cylinder,
  const CRhinoMeshDensity& mesh_density, bool CapTop, bool CapBottom, bool bCircumscribe, bool bQuadCaps);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCylinder( 
            const ON_Cylinder& cylinder, 
            int vertical, int around );

// Remarks: bQuadCaps is ignored if around is not an even number.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCylinder(
  const ON_Cylinder& cylinder,
  int vertical, int around, bool bQuadCaps);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCylinder(
  const ON_Cylinder& cylinder,
  int vertical, int around, bool CapTop, bool CapBottom, bool bCircumscribe, bool bQuadCaps);

// Description:
//   Create a mesh cone.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCone( 
            const ON_Cone& cone, 
            const CRhinoMeshDensity& mesh_density);

// Remarks: bQuadCaps is ignored if around is not an even number.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCone(
            const ON_Cone& cone,
            const CRhinoMeshDensity& mesh_density, 
            bool bQuadCaps);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCone( 
            const ON_Cone& cone, 
            int vertical, int around );

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshConeBothSides(
  const ON_Cone& cone,
  int vertical, int around);


// Description:
//   Create a mesh truncated cone.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshTCone(const ON_Brep& TCone, const CRhinoMeshDensity& md);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshTCone(const ON_Brep& TCone, const CRhinoMeshDensity& md, bool bQuadCaps);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshTCone(const ON_Brep& TCone, int vertical, int around );

// Remarks: bQuadCaps is ignored if around is not an even number.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshTCone(const ON_Brep& TCone, int vertical, int around, bool bQuadCaps);

// Description:
//   Create a mesh Cone.
// Parameters:
//   cone - [in]
//   vertical - [in]
//   around - [in]
//   bSolid - [in]  If bSolid is false the mesh will be open with no faces on the circular planar portion.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCone( 
            const ON_Cone& cone, 
            int vertical, int around,
            bool bSolid);

// Remarks: bQuadCaps is ignored if around is not an even number.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshCone(
            const ON_Cone& cone,
            int vertical, int around,
            bool bSolid, bool bQuadCaps);


// Description:
//   Create a mesh plane.
// Parameters:
//   plane - [in]
//   x_interval - [in]
//   y_interval - [in]
// Returns:
//   A mesh of the rectangle with corners
//   plane.PointAt(x_interval[0],x_interval[0]), 
//   plane.PointAt(x_interval[1],x_interval[1]).
//   The number of quads is determined by md.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshPlane( 
            const ON_Plane& plane, 
            ON_Interval x_interval, 
            ON_Interval y_interval, 
            const CRhinoMeshDensity& mesh_density 
            );

// Description:
//   Create a mesh plane.
// Parameters:
//   plane - [in]
//   x_interval - [in]
//   y_interval - [in]
//   X number of faces in X direction - [in]
//   Y number of faces in Y direction - [in]
// Returns:
//   A mesh of the rectangle with corners
//   plane.PointAt(x_interval[0],x_interval[0]), 
//   plane.PointAt(x_interval[1],x_interval[1]).
//   The number of quads is determined by md.
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshPlane( 
            const ON_Plane& plane, 
            ON_Interval x_interval, 
            ON_Interval y_interval,
            int X,
            int Y
            );

// Description:
//   Get a 3d box
// Parameters:
//   args - [in] controls how the box is picked, prompts, etc.
//   box_corners - [out] corners of the box.
//
//            7______________6
//            |\             |\
//            | \            | \
//            |  \ _____________\
//            |   4          |   5
//            |   |          |   |
//            |   |          |   |
//            3---|----------2   |
//            \   |          \   |
//             \  |           \  |
//              \ |            \ |
//               \0_____________\1
//
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshBox(ON_3dPoint box_corners[8], const CRhinoMeshDensity& md );

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshBox( ON_3dPoint box_corners[8], int X, int Y, int Z);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshTorus(const ON_RevSurface* torus, const CRhinoMeshDensity& md);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshTorus(const ON_RevSurface* torus, int vertical, int around);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshEllipsoid(const ON_Surface* pSrf, const CRhinoMeshDensity& md);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshEllipsoid(const ON_Surface* pSrf, const CRhinoMeshDensity& md, const bool bQuadCaps);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshEllipsoid(const ON_Surface* pSrf, int vertical, int around);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshEllipsoid(const ON_Surface* pSrf, int vertical, int around, const bool bQuadCaps);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshSphere(const ON_Sphere& sphere, const CRhinoMeshDensity& md);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshSphere(const ON_Sphere& sphere, int vertical, int around);


class CRhinoBrepObject;
class CRhinoInstanceObject;
/*
Description:
  Used to return meshes created by RhinoMeshObjects.
*/
class RHINO_SDK_CLASS CRhinoObjectMesh
{
public:
  CRhinoObjectMesh();

  // deletes m_mesh if it is not NULL
  ~CRhinoObjectMesh();

  // mesh is returned here. If you want to keep the mesh, then set
  // m_mesh to NULL. Otherwise ~CRhinoObjectMesh will delete m_mesh.
  ON_Mesh* m_mesh;

  // object attributes associated with this mesh
  ON_3dmObjectAttributes m_mesh_attributes;

  // object that was meshed to create m_mesh
  const CRhinoObject* m_parent_object;

  // If m_iref_object is not NULL when the top level object
  // was an instance reference.  Otherwise, m_iref_object
  // is NULL and the m_brep_object or m_mesh_object is the
  // top level object.
  const CRhinoInstanceObject* m_iref_object;

  // At most one of m_mesh_object or m_brep_object is not NULL.
  // If both are null, then inspect m_parent_object to determine
  // what object was meshed, often a CRhinoExtrusionObject.
  // If m_iref_object is NULL, then m_brep_object or m_mesh_object
  // is an object in the doc's geometry  or reference geometry tables.
  // If m_iref_object is not NULL, then m_brep_object or m_mesh_object
  // is in the doc's instance definition geometry list.
  const CRhinoMeshObject* m_mesh_object;
  const CRhinoBrepObject* m_brep_object;

  // If m_brep_object is not NULL, then m_brep is the object that
  // was meshed
  const ON_Brep* m_brep;
};


/*
Description:
  Displays Rhino mesh object user interface.
Parameters:
  mp - [in/out] meshing parameters. The meshing parameters may change.
  ui_style - [in/out]
       0 = simple dialog
       1 = details dialog
       2 = script or batch mode
       If the input value is 0 or 1, the user can switch to the other
       dialog and the style of dialog that was used is returned.
Returns:
  CRhinoCommand::success - user finished input
  CRhinoCommand::cancel - user canceled input
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetMeshParameters(
  CRhinoDoc& doc,
  ON_MeshParameters& mp,
  int& ui_style
);

/*
Description:
  Mesh Rhino objects.
Parameters:
  objects - [in] objects to mesh

  mp - [in/out] meshing parameters. The meshing parameters may change.
                The parameters used to create the meshes are returned.

  ui_style - [in/out]
       0 = simple dialog 
       1 = details dialog
       2 = script or batch mode
       If the input value is 0 or 1, the user can switch to the other
       dialog and the style of dialog that was used is returned.

  meshes - [out] the created meshes are appended to this array.
       If an input object is a CRhinoBrepObject, then there will be
       one mesh in the output list for that brep.  If an input object
       is a CRhinoInstanceObject, then there will be one object in
       the list for each piece of instance definition geometry that
       can be meshed.
       If an input object is not a brep and not an instance reference
       that has meshable parts, then nothing is added to the output list.
Returns:
  @untitled table
  CRhinoCommand::success         meshes created
  CRhinoCommand::cancel          user canceled meshing
  CRhinoCommand::exit_rhino      user closed Rhino
  CRhinoCommand::failure         unable to create meshes
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoMeshObjects(
  const ON_SimpleArray<const CRhinoObject*>& objects,
  ON_MeshParameters& mp,
  int& ui_style,
  ON_ClassArray<CRhinoObjectMesh>& meshes
);

/*
Description:
  Mesh Rhino objects.
Parameters:
  objects - [in] objects to mesh

  mp - [in/out] meshing parameters. The meshing parameters may change.
                The parameters used to create the meshes are returned.

  xform - [in]
    If xform is valid, not zero and not the identity, then the transformation is
    applied to all the returned meshes.

  ui_style - [in/out]
       0 = simple dialog 
       1 = details dialog
       2 = script or batch mode
       If the input value is 0 or 1, the user can switch to the other
       dialog and the style of dialog that was used is returned.

  meshes - [out] the created meshes are appended to this array.
       If an input object is a CRhinoBrepObject, then there will be
       one mesh in the output list for that brep.  If an input object
       is a CRhinoInstanceObject, then there will be one object in
       the list for each piece of instance definition geometry that
       can be meshed.
       If an input object is not a brep and not an instance reference
       that has meshable parts, then nothing is added to the output list.
Returns:
  @untitled table
  CRhinoCommand::success         meshes created
  CRhinoCommand::cancel          user canceled meshing
  CRhinoCommand::exit_rhino      user closed Rhino
  CRhinoCommand::failure         unable to create meshes
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoMeshObjects(
  const ON_SimpleArray<const CRhinoObject*>& objects,
  ON_MeshParameters& mp,
  const ON_Xform& xform,
  int& ui_style,
  ON_ClassArray<CRhinoObjectMesh>& meshes
);

RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoMeshObjects(
  const ON_SimpleArray<const CRhinoObject*>& objects,
  ON_MeshParameters& mp,
  const ON_Xform& xform,
  int& ui_style,
  ON_ClassArray<CRhinoObjectMesh>& meshes,
  bool bUseWorkerThread
);


typedef bool (*CUSTOM_MESH_HANDLER_FUNC)(const CRhinoObject&, ON_SimpleArray<ON_Mesh*>& aMeshes);
ON_DEPRECATED_MSG("Use RhinoRegisterCustomMeshHandler2") RHINO_SDK_FUNCTION
void RhinoRegisterCustomMeshHandler(CUSTOM_MESH_HANDLER_FUNC);

typedef bool (*CUSTOM_MESH_HANDLER_FUNC2)( const CRhinoObject&, std::vector<std::shared_ptr<const ON_Mesh>>& aMeshes);
RHINO_SDK_FUNCTION
void RhinoRegisterCustomMeshHandler2(CUSTOM_MESH_HANDLER_FUNC2);

class RHINO_SDK_CLASS CRhinoMeshObjectsUI
{
public:
  CRhinoMeshObjectsUI();
  
  virtual
  ~CRhinoMeshObjectsUI();

  /*
  Description:
    Provides the user interface that gets meshing parameters.
  Parameters:
    objects - [in] objects being meshed
    mp - [out] meshing parameters.
  Returns:
    CRhinoCommand::success if the mesh parameters
    returned in mp should be used to create meshes.

    CRhinoCommand::cancel if the user canceled.    
  */
  virtual 
  CRhinoCommand::result GetMeshParameters( 
    const ON_SimpleArray< const CRhinoObject* >& objects,
    ON_MeshParameters& mp
    ) = 0;

  /*
  Description:
    Call this function if you want a visual preview of
    the meshes created from the settings in mp.
  Returns:
    0: OnCancelPreview() canceled the mesh calculation.
    1: successful preview mesh creation.
    2: User pressed ESC key.  
       Return CRhinoCommand::cancel immediately.
  */
  int OnPreview( const ON_MeshParameters& mp );

  /*
  Description:
    Call to cancel mesh calculations happening during a
    call to OnPreview().
  Returns:
    True if preview meshes were begin calculated and the
    calculation was canceled.
  */
  bool OnCancelPreview();

public:
  // Pretend these are private.  Don't look and don't touch.
  class CRhinoMeshWorkerThread* m__meshworker;
 	AFX_MODULE_STATE* m__pModuleState;
};


/*
Description:
  Creates the course/medium/fine + optional tolerance dialog
  used for STL export.
*/
class RHINO_SDK_CLASS CRhinoMeshStlUI : public CRhinoMeshObjectsUI
{
public:
  CRhinoMeshStlUI();
  
  virtual
  ~CRhinoMeshStlUI();

  // Description:
  //   Loads settings from the Rhino profile.
  // Parameters:
  //   lpszSection - [in] profile section to use for this command
  //   pc - [in] profile context to use to get at
  //             value saved in the Rhino profile.
  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );

  // Description:
  //   Save settings to the Rhino profile.
  // Parameters:
  //   lpszSection - [in] profile section to use for this command
  //   pc - [in] profile context to use to get at
  //             value saved in the Rhino profile.
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );


  // Override of pure virtual CRhinoMeshObjectsUI
  CRhinoCommand::result GetMeshParameters( 
    const ON_SimpleArray< const CRhinoObject* >& objects,
    ON_MeshParameters& mp
    );

  /*
  Description:
    Sets meshing parameters from simple quality and tolerance settings.
  Parameters:
    quality - [in] 0 = course, 1 = medium, 2 = fine
    tolerance - [in] mesh tolerance (ignored if <= 0.0)
    mp - [out]
  */
  static 
  void SetMeshParameters( 
    int quality, 
    double tolerance,
    ON_MeshParameters& mp
    );


  void SetValuesFromMeshParameters(const ON_MeshParameters&, bool bDetailed);

  /*
  Description:
    Get quality setting from mesh parameters.
  Parameters:
    mp - [in]
  Returns
    Quality setting:

    0 = course, 1 = medium, 2 = fine
  */
  static 
  int MeshQuality( const ON_MeshParameters& mp );

  /*
  Description:
    Get tolerance setting from mesh parameters.
  Parameters:
    mp - [in]
  Returns
    Tolerance
  */
  static 
  double MeshTolerance( const ON_MeshParameters& mp );

  /*
  Specifies style of command using this UI
    CRhinoCommandContext::interactive_ui
    CRhinoCommandContext::script_ui
    CRhinoCommandContext::batch_ui
    (default=interactive_ui)
  */
  int m_command_style;

  // 0 = course, 1 = medium, 2 = fine (default=1)
  int m_mesh_quality;

  // mesh tolerance is ignored if <= 0.0 (default=0.0)
  double m_mesh_tolerance;

  //default = 0.0
  double m_max_edge_length;

  //default = 6.0
  double m_grid_aspect_ratio;

  //16;
  int m_grid_min_count;

  //20.0*ON_PI/180.0;
  double m_refine_angle; 

  //The new Rhino 4 mesh Density setting.  Initially set to 0
  double m_relative_tolerance;

  // starting dialog type
  int m_detailed_dlg; //0 = stl dialog, 1 = detailed dialog

  // tolerance description string
  ON_wString m_tolerance_str; 

  // dialog title
  ON_wString m_title_str; 
};

/*
Description:
  Mesh Rhino objects.

Parameters:
  objects - [in] objects to mesh

  mp - [in/out] meshing parameters. The meshing parameters may change.
                The parameters used to create the meshes are returned.

  xform - [in]
          If xform is valid, not zero and not the identity, then the transformation is
          applied to all the returned meshes.

  ui - [in]
       Defines a custom ui for setting meshing parameters.

  meshes - [out] the created meshes are appended to this array.
       If an input object is a CRhinoBrepObject, then there will be
       one mesh in the output list for that brep.  If an input object
       is a CRhinoInstanceObject, then there will be one object in
       the list for each piece of instance definition geometry that
       can be meshed.
       If an input object is not a brep and not an instance reference
       that has meshable parts, then nothing is added to the output list.
Remarks:
  If you want to create meshes using the UI that is found in the
  Rhino Mesh command, then you can use the version of
  RhinoMeshObjects() that takes an int instead of a
  CRhinoMeshObjectsUI.

Returns:
  @untitled table
  CRhinoCommand::success         meshes created
  CRhinoCommand::cancel          user canceled meshing
  CRhinoCommand::exit_rhino      user closed Rhino
  CRhinoCommand::failure         unable to create meshes
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoMeshObjects( 
  const ON_SimpleArray<const CRhinoObject*>& objects,
  ON_MeshParameters& mp,
  const ON_Xform& xform,
  class CRhinoMeshObjectsUI& ui,
  ON_ClassArray<CRhinoObjectMesh>& meshes
  );

RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoMeshObjects(
  const ON_SimpleArray<const CRhinoObject*>& objects,
  ON_MeshParameters& mp,
  const ON_Xform& xform,
  class CRhinoMeshObjectsUI& ui,
  ON_ClassArray<CRhinoObjectMesh>& meshes,
  bool bUseWorkerThread
);


/*
Description:
Mesh Rhino objects.

Parameters:
objects - [in] objects to mesh

mp - [in/out] meshing parameters. The meshing parameters may change.
The parameters used to create the meshes are returned.

ui - [in]
Defines a custom ui for setting meshing parameters.

meshes - [out] the created meshes are appended to this array.
If an input object is a CRhinoBrepObject, then there will be
one mesh in the output list for that brep.  If an input object
is a CRhinoInstanceObject, then there will be one object in
the list for each piece of instance definition geometry that
can be meshed.
If an input object is not a brep and not an instance reference
that has meshable parts, then nothing is added to the output list.
Remarks:
If you want to create meshes using the UI that is found in the
Rhino Mesh command, then you can use the version of
RhinoMeshObjects() that takes an int instead of a
CRhinoMeshObjectsUI.

Returns:
@untitled table
CRhinoCommand::success         meshes created
CRhinoCommand::cancel          user canceled meshing
CRhinoCommand::exit_rhino      user closed Rhino
CRhinoCommand::failure         unable to create meshes
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoMeshObjects(
  const ON_SimpleArray<const CRhinoObject*>& objects,
  ON_MeshParameters& mp,
  class CRhinoMeshObjectsUI& ui,
  ON_ClassArray<CRhinoObjectMesh>& meshes
  );

/*
Description:
  Mesh Rhino objects.
Parameters:
  objects - [in] objects to get render meshes for.
  render_meshes - [out]
       render_meshes[i].Object() = input object
       render_meshes[i].Mesh() = render mesh
  bOkToCreate - [in] if true and the render meshes do not
       exist, then they will be created.
  bReturnAllObjects - [in] curves, points, etc., do not
       have render meshes.  If you want these objects to
       appear in the output list with a NULL mesh, then
       set this flag to true.
  bSkipHiddenObjects - [in] will not create render meshes for
       hidden objects if set to true.
  bUpdateMeshTCs - [in] updates texture coordinates on the output mesh
       using the texture mapping if set to true.
Returns:
  Number of meshes appended to render_meshes[].
Remarks:
  References to instance definitions that contain multiple
  objects will appear multiple times in the output list.

  When the input object is a CRhinoInstanceObject, one or
  more CRhinoObjRefs may be returned in the render_meshes[]
  array.  Each atomic object the instance reference ultimately
  refers to gets a CRhinoObjRef.  The Object() returned by
  the CRhinoObjRef points to the atomic object in the
  instance definition table.  The Geometry() returned by the
  CRhinoObjRef is a transformed copy of the mesh.  The mesh
  is correctly located and does not require further
  transformations.  In this case,
  CRhinoObjRef::GetInstanceTransformation() will
  return a pointer to the top level instance reference
  (the input CRhinoInstanceObject) and report the
  total transformation that was applied to the mesh.
  In the case of nested instance references, this
  transformation may be different from the transformation
  associated with the input CRhinoInstanceObject.
*/
ON_DEPRECATED_MSG("Does not support custom render meshes, per-face materials or delivery of the shared pointer meshes.  Prefer IRhRdkSdkRenderMeshIterator / CRhRdkSdkRender::NewRenderMeshIterator")
RHINO_SDK_FUNCTION
int RhinoGetRenderMeshes(
  const ON_SimpleArray< const CRhinoObject* >& objects,
  ON_ClassArray< CRhinoObjRef >& render_meshes,
  bool bOkToCreate,
  bool bReturnAllObjects,
  bool bSkipHiddenObjects,
  bool bUpdateMeshTCs
);

/*
Description:
  Mesh Rhino objects.
Parameters:
  objects - [in] objects to get render meshes for.
  render_meshes - [out] 
       render_meshes[i].Object() = input object
       render_meshes[i].Mesh() = render mesh
  bOkToCreate - [in] if true and the render meshes do not
       exist, then they will be created.
  bReturnAllObjects - [in] curves, points, etc., do not
       have render meshes.  If you want these objects to
       appear in the output list with a NULL mesh, then
       set this flag to true.
  bSkipHiddenObjects - [in] will not create render meshes for
       hidden objects if set to true.
Returns:
  Number of meshes appended to render_meshes[].
Remarks:
  References to instance definitions that contain multiple
  objects will appear multiple times in the output list.

  When the input object is a CRhinoInstanceObject, one or 
  more CRhinoObjRefs may be returned in the render_meshes[]
  array.  Each atomic object the instance reference ultimately
  refers to gets a CRhinoObjRef.  The Object() returned by
  the CRhinoObjRef points to the atomic object in the
  instance definition table.  The Geometry() returned by the
  CRhinoObjRef is a transformed copy of the mesh.  The mesh
  is correctly located and does not require further 
  transformations.  In this case,
  CRhinoObjRef::GetInstanceTransformation() will
  return a pointer to the top level instance reference
  (the input CRhinoInstanceObject) and report the 
  total transformation that was applied to the mesh.  
  In the case of nested instance references, this
  transformation may be different from the transformation
  associated with the input CRhinoInstanceObject.  
*/
ON_DEPRECATED_MSG("Does not support custom render meshes, per-face materials or delivery of the shared pointer meshes.  Prefer IRhRdkSdkRenderMeshIterator / CRhRdkSdkRender::NewRenderMeshIterator")
RHINO_SDK_FUNCTION
int RhinoGetRenderMeshes(
          const ON_SimpleArray< const CRhinoObject* >& objects,
          ON_ClassArray< CRhinoObjRef >& render_meshes,
          bool bOkToCreate,
          bool bReturnAllObjects,
          bool bSkipHiddenObjects
          );

ON_DEPRECATED_MSG("Does not support custom render meshes, per-face materials or delivery of the shared pointer meshes.  Prefer IRhRdkSdkRenderMeshIterator / CRhRdkSdkRender::NewRenderMeshIterator")
RHINO_SDK_FUNCTION
int RhinoGetRenderMeshes(
          const ON_SimpleArray< const CRhinoObject* >& objects,
          ON_ClassArray< CRhinoObjRef >& render_meshes,
          bool bOkToCreate = true,
          bool bReturnAllObjects = false
          );

/*
Description:
  Mesh Rhino objects.
Parameters:
  objects - [in] objects to get analysis meshes for.
  analysis_meshes - [out] 
       analysis_meshes[i].Object() = input object
       analysis_meshes[i].Mesh() = analysis mesh
  bOkToCreate - [in] if true and the analysis meshes do not
       exist, then they will be created.
  bReturnAllObjects - [in] curves, points, etc., do not
       have analysis meshes.  If you want these objects to
       appear in the output list with a NULL mesh, then
       set this flag to true.
  bSkipHiddenObjects - [in] will not create analysis meshes for
       hidden objects if set to true.
Returns:
  Number of meshes appended to analysis_meshes[].
Remarks:
  References to instance definitions that contain multiple
  objects will appear multiple times in the output list.

  When the input object is a CRhinoInstanceObject, one or 
  more CRhinoObjRefs may be returned in the render_meshes[]
  array.  Each atomic object the instance reference ultimately
  refers to gets a CRhinoObjRef.  The Object() returned by
  the CRhinoObjRef points to a the atomic object in the
  instance definition table.  The Geometry() returned by the
  CRhinoObjRef is a transformed copy of the mesh.  The mesh
  is correctly located and does not require further 
  transformations.  In this case,
  CRhinoObjRef::GetInstanceTransformation() will
  return a pointer to the top level instance reference
  (the input CRhinoInstanceObject) and report the 
  total transformation that was applied to the mesh.  
  In the case of nested instance references, this
  transformation may be different from the transformation
  associated with the input CRhinoInstanceObject.  
*/
RHINO_SDK_FUNCTION
int RhinoGetAnalysisMeshes(
          const ON_SimpleArray< const CRhinoObject* >& objects,
          ON_ClassArray< CRhinoObjRef >& analysis_meshes,
          bool bOkToCreate,
          bool bReturnAllObjects,
          bool bSkipHiddenObjects
          );

RHINO_SDK_FUNCTION
int RhinoGetAnalysisMeshes(
          const ON_SimpleArray< const CRhinoObject* >& objects,
          ON_ClassArray< CRhinoObjRef >& analysis_meshes,
          bool bOkToCreate = true,
          bool bReturnAllObjects = false
          );

/*
Description:
  Get mesh parameters in a scriptable manner.
Parameters:
  mp - [in/out] in = current settings
Returns:
  CRhinoCommand::result code
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoScriptGetMeshParameters( ON_MeshParameters& mp );

/*
Description:
  Performs same procedure as the MatchMeshEdge command on a single mesh.
Parameters:
  input - [in] Original const mesh.
  output - [out] If return value is true output will be the modified copy of input.
  distance - [in] Distance to not exceed when modifying the mesh.
  ratchet - [in] Divides the initial distance by 1000 and goes through the code 4 times, each time increasing the distance
                  by a factor of 10.
Returns:
  True on success.  If false, output has not been modified.
*/
RHINO_SDK_FUNCTION
bool RhinoMatchMeshEdge(
  const ON_Mesh* input,
  ON_Mesh& output,
  double distance,
  bool rachet);
