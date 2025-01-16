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

/*
Description:
  Interactively picks a point on a mesh object
Parameters:
  mesh_object    - [in]  mesh object
  sPrompt        - [in]  command line prompt
  bAcceptNothing - [in]  allows the user to be able to press enter in order
                         to skip selecting the point.
  output_point   - [out] the selected ON_MESH_POINT see opennuurbs_meshtree.h
                         for details on ON_MESH_POINT

Returns: 0 - success
         1 - nothing
         2 - cancel
         3 - failure
*/
RHINO_SDK_FUNCTION
int RhinoGetPointOnMesh(
        const CRhinoMeshObject* mesh_object,
        const wchar_t* sPrompt,
        BOOL32 bAcceptNothing,
        ON_MESH_POINT& output_point,
        CRhinoHistory* history = NULL
        );

RHINO_SDK_FUNCTION
bool RhinoRepairMesh( 
          ON_Mesh* pMesh, 
          double repair_tolerance
          );

/*
Description:
  Function that attempts to fix inconsistenties in the directions of meshfaces for a mesh.
Parameters:
  origmesh      - Input.
  pNewMesh      - Output.  If pNewMesh is not null the the return will be pNewMesh also.
  bGetCountOnly - Input.  Default is false.  If set to true no mesh is modified and the return is null.
  pCount        - Output.  If pCount is not null and bGetCountOnly is true then pCount is the number of faces that would be modified.
Returns:
  If bGetCountOnly is false then the return is a pointer to a copy of origmesh with the appropriate faces flipped.  If pNewMesh is not null
  then the return is pNewMesh.  If pNewMesh is null and bGetCountOnly is true then the return will be null.
Remarks:
  bGetCountOnly and pCount were added so the function could be used to examine the origmesh without the need to create a new modified mesh.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoUnifyMeshNormals(const ON_Mesh& origmesh, 
                               ON_Mesh* pNewMesh = 0, 
                               bool bGetCountOnly = false, 
                               int* pCount = 0);


class RHINO_SDK_CLASS CRhinoPolylineOnMeshUserData : public ON_UserData
{
  ON_OBJECT_DECLARE(CRhinoPolylineOnMeshUserData);

public:
  static GUID PolylineOnMeshUserData_uuid;

  CRhinoPolylineOnMeshUserData();
  CRhinoPolylineOnMeshUserData(const CRhinoPolylineOnMeshUserData&);
  CRhinoPolylineOnMeshUserData& operator=(const CRhinoPolylineOnMeshUserData&);
  ~CRhinoPolylineOnMeshUserData();

  bool Archive() const override;
  bool Write(ON_BinaryArchive&) const override;
  bool Read(ON_BinaryArchive&) override;
  bool GetDescription(ON_wString&) override;

  UUID m_mesh_uuid;
  ON_SimpleArray<ON_MESH_POINT> m_P; //see opennurbs_meshtree.h for details on ON_MESH_POINT
};

/*
Description:
  Makes a polyline approximation of the input curve and gets the closest point on the mesh for each point on the mesh.  Then it "connects the points" so 
  that you have a polyline on the mesh.
Parameters:
  pCrv      - Input curve.
  pMesh      - Input mesh.
  tolerance - Input tolerance.
  YDData    - Optional userdata.  If YDData is not null then it will be filled in.
Returns:
  Returns a ON_PolylineCurve on success.  Returns null on failure.
*/
RHINO_SDK_FUNCTION
ON_PolylineCurve* RhinoPullCurveToMesh(const ON_Curve* pCrv, 
                                       const ON_Mesh* pMesh, 
                                       const double tolerance, 
                                       CRhinoPolylineOnMeshUserData* YDData = 0);
/*
Description:
  Makes a new mesh with vertices offset at distance d in the opposite direction of the existing vertex normals.
  
Parameters:
  d          - Input offset distance.
  pMesh      - Input mesh.
Returns:
  Returns a ON_Mesh pointer on success.  Returns null on failure.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshOffset(double d, const ON_Mesh* pMesh);

/*
Description:
  Makes a new mesh with vertices offset at distance d in the opposite direction of the existing vertex normals.
  Optionally, based on the value of bSolidify, adds the input mesh and a ribbon of faces along anhy naked edges.
  If bSolidy if false it acts exactly as the function above.

  Essentially this function calls the function above to get an offset output mesh and then the function below
  to create the ribbon mesh and combine the input, ribbon and output into a single output.
  
Parameters:
  d          - Input offset distance.
  pMesh      - Input mesh.
  bSolidify  - Input bool to determine whether the output mesh should be turned into a closed mesh, see description above.
Returns:
  Returns a ON_Mesh pointer on success.  Returns null on failure.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshOffset(double d, const ON_Mesh* pMesh, bool bSolidify);

/*
Description:
  Makes a new mesh with vertices offset at distance d along the direction vector parameter.
  Optionally, based on the value of bSolidify, adds the input mesh and a ribbon of faces along any naked edges.
  If direction is ON_3dVector::UnsetVector it acts exactly as the function above.

Parameters:
  d          - Input offset distance.
  pMesh      - Input mesh.
  bSolidify  - Input bool to determine whether the output mesh should be turned into a closed mesh, see description above.
  direction  - Input vector for offset direction for all vertices or ON_3dVector::UnsetVector to use vertex normals.
Returns:
  Returns a ON_Mesh pointer on success.  Returns null on failure.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshOffset(double d, const ON_Mesh* pMesh, bool bSolidify, const ON_3dVector& direction);

/*
Description:
  Makes a new mesh with vertices offset at distance d along the direction vector parameter.
  Optionally, based on the value of bSolidify, adds the input mesh and a ribbon of faces along any naked edges.
  If direction is ON_3dVector::UnsetVector it acts exactly as the function above. Optionally returns list of wall faces,
  i.e. the faces that connect original and offset mesh when solidified.

Parameters:
  d             - Input offset distance.
  pMesh         - Input mesh.
  bSolidify     - Input bool to determine whether the output mesh should be turned into a closed mesh, see description above.
  direction     - Input vector for offset direction for all vertices or ON_3dVector::UnsetVector to use vertex normals.
  pWallFacesOut - If not null then gets filled with indices of the wall faces
Returns:
  Returns a ON_Mesh pointer on success.  Returns null on failure.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshOffset(double d, const ON_Mesh* pMesh, bool bSolidify, const ON_3dVector& direction, ON_SimpleArray<int>* pWallFacesOut);

/*
Description:
  Sort of an expert function meant to be called from the RhinoMeshOffset function above.  It assumes, and checks for, 
  the input and output meshes are topologically identical.  What is meant by this is that they have the same m_V count 
  and the same m_F count.  In fact, the m_F arrays of both meshes must be identical.

  This function creates the ribbon of faces along the associated naked edges of the input and output meshes.  Once it 
  makes those faces it add them and the input mesh to the output mesh to create a closed mesh.  The Offset variable is 
  only used for it's sign to determine the orientation of the ribbon faces.
  
Parameters:
  pInputMesh      - Input mesh.
  pOutputMesh     - Output mesh.
  Offset          - Input only used for sign.

Returns:
  Returns a true on success. False on failure.  Potentially modifies pOutputMesh even on failure.
*/
RHINO_SDK_FUNCTION
bool CapOffSettedMesh(const ON_Mesh* pInputMesh, ON_Mesh* pOutputMesh, double Offset);

/*
Description:
  Splits up the input mesh into its unconnected pieces.  If bJustCount is true then it returns the number of meshes that the input
  mesh would be split into.
  
Parameters:
  pMesh      - Input mesh.
  MeshArray  - Output array of meshes.
  bJustCount - Input flag to determine whether to actually split the input mesh
Returns:
  Returns number of meshes the input mesh would be split into, 0 on failure.
*/
RHINO_SDK_FUNCTION
int RhinoSplitDisjointMesh(const ON_Mesh* pMesh, ON_SimpleArray<ON_Mesh*>& MeshArray, bool bJustCount = false);

/*
Description:
  Triangulate a 3D simple closed polygon that projects onto a plane.
  The polygon must project to a plane and the projected polygon must be a simple
  closed curve.  Do NOT duplicate the start/end point; i.e., a triangle will have
  a point count of 3 and P will specify 3 distinct non-collinear points.
Parameters:
  point_count  - [in] number of points in polygon ( >= 3 )
  point_stride - [in] the point stride
  P            - [in] an array of points, where:
                      i-th point = {P[i*point_stride], P[i*point_stride+1], P[i*point_stride+2]}
  tri_stride   - [in] the triangle stride
  triangle     - [out] an array of (point_count-2)*tri_stride integers
Returns:
  0 if successful
  <0 on failure
Remarks:
  The polygon is trianglulated into point_count-2 triangles.  The indices of the
  3 points that are the corner of the i-th (0<= i < point_count-2) triangle are
    {triangle[i*tri_stride], triangle[i*tri_stride+1], triangle[i*tri_stride+2]}.
*/
RHINO_SDK_FUNCTION
int RhinoTriangulate3dPolygon( 
        int point_count, 
        int point_stride, 
        const double* P,
        int tri_stride, 
        int* triangle 
        );

/*
Description:
  RhinoMeshBooleanUnion Finds the union of the meshes in InMeshes.  If the attribute arrays are non null then the first attribute
  of each union will be used.

Parameters:
  InMeshes               - [In]  Meshes to be unioned
  Intersection_Tolerance - [In]  General tolerance to be used in mesh-mesh intersection   
  Overlap_Tolerance      - [In]  Tolerance used to determine overlaps in mesh-mesh intersection 
  something_happened     - [Out] Set to true if something actually happened, independent of function return
  OutMeshes              - [Out] Result of the union of the input meshes
  InputAttributeArray    - [In]  Optional array of attributes.  Must be not be null and have count equal to InMeshes to be used.
  OuputAttributeArray    - [Out] Optional array of attributes.  Must be not be null to be used.

Returns:
  True or false based on whether the function was able to complete successfully.  It does not indicate whether the function actually 
  unioned the input meshes.  The argument, something_happened, is used for that purpose. 

See Also:
  RhionMeshBooleanIntersection, RhinoMeshBooleanDifference, RhinoMeshBooleanSplit
*/
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanUnion(
        const ON_SimpleArray<const ON_Mesh*>& InMeshes,
        double Intersection_Tolerance, 
        double Overlap_Tolerance, 
        bool* something_happened, 
        ON_SimpleArray<ON_Mesh*>& OutMeshes,
        const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray = 0,
        ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray = 0
        );

class CRhMeshBooleanOptionsPrivate
{
public:
  ON_SimpleArray<unsigned>* m_firstNewlyAddedFace;
};

/// <summary>
/// Contains additional information about mesh booleans. This class can have more methods added.
/// </summary>
class RHINO_SDK_CLASS CRhinoMeshBooleanOptions
{
public:
  ON_TextLog* m_text_log = nullptr;
  ON_Terminator* m_cancel = nullptr;
  ON_ProgressReporter* m_reporter = nullptr;
  bool m_ngons = true;

  ~CRhinoMeshBooleanOptions();
  CRhinoMeshBooleanOptions();

  /// <summary>
  /// Retuns a list with the first index, the first index of the first newly added mesh face,
  /// and as secondo index, the count of new faces after that index.
  /// </summary>
  /// <param name="list">The indices of the new faces, as addessed in the summary.</param>
  void RequestFirstNewlyAddedFacesList(ON_SimpleArray<unsigned>* list);

private:
  class CRhMeshBooleanOptionsPrivate* m_private = nullptr;
};

RHINO_SDK_FUNCTION
bool RhinoMeshBooleanUnion(
  const ON_SimpleArray<const ON_Mesh*>& InMeshes,
  double Intersection_Tolerance,
  double Overlap_Tolerance,
  bool* something_happened,
  ON_SimpleArray<ON_Mesh*>& OutMeshes,
  const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray,
  ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray,
  CRhinoMeshBooleanOptions* options
);


/*
Description:
  RhinoMeshBooleanDifference subtracts the meshes in InMeshes1 from InMeshes0.  Meshes from InMeshes1 are unioned together first 
  and the result is differenced with each mesh in InMeshes0.  If the attribute arrays are non null then the attributes for 
  InMeshes0 will be used.
  
Parameters:
  InMeshes0              - [In]  Meshes to be from which to have InMeshes1 subtracted
  InMeshes1              - [In]  Meshes to subtract from InMeshes0
  Intersection_Tolerance - [In]  General tolerance to be used in mesh-mesh intersection   
  Overlap_Tolerance      - [In]  Tolerance used to determine overlaps in mesh-mesh intersection 
  something_happened     - [Out] Set to true if something actually happened, independent of function return
  OutMeshes              - [Out] Result of the difference of the input meshes
  InputAttributeArray    - [In]  Optional array of attributes.  Must be not be null and have count equal to InMeshes to be used.
  OuputAttributeArray    - [Out] Optional array of attributes.  Must be not be null to be used.

Returns:
  True or false based on whether the function was able to complete successfully.  It does not indicate whether the function actually 
  unioned the input meshes.  The argument, something_happened, is used for that purpose. 

See Also:
  RhinoMeshBooleanIntersection, RhinoMeshBooleanUnion, RhinoMeshBooleanSplit
*/
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanDifference(
        const ON_SimpleArray<const ON_Mesh*>& InMeshes0, 
        const ON_SimpleArray<const ON_Mesh*>& InMeshes1, 
        double Intersection_Tolerance, 
        double Overlap_Tolerance, 
        bool* something_happened, 
        ON_SimpleArray<ON_Mesh*>& OutMeshes, 
        const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray = 0, 
        ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray = 0
        );
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanDifference(
  const ON_SimpleArray<const ON_Mesh*>& InMeshes0,
  const ON_SimpleArray<const ON_Mesh*>& InMeshes1,
  double Intersection_Tolerance,
  double Overlap_Tolerance,
  bool* something_happened,
  ON_SimpleArray<ON_Mesh*>& OutMeshes,
  const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray,
  ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray,
  CRhinoMeshBooleanOptions* options
);

/*
Description:
  RhinoMeshBooleanIntersection intersects the meshes in InMeshes0 with InMeshes1.  Meshes from InMeshes0 and InMeshes1 are 
  unioned together first and the results are intersected.  If the attribute arrays are non null then the attributes for 
  InMeshes0 will be used.
  
Parameters:
  InMeshes0              - [In]  First input mesh set
  InMeshes1              - [In]  Second input mesh set
  Intersection_Tolerance - [In]  General tolerance to be used in mesh-mesh intersection   
  Overlap_Tolerance      - [In]  Tolerance used to determine overlaps in mesh-mesh intersection 
  something_happened     - [Out] Set to true if something actually happened, independent of function return
  OutMeshes              - [Out] Result of the intersection of the input meshes
  InputAttributeArray    - [In]  Optional array of attributes.  Must be not be null and have count equal to InMeshes to be used.
  OuputAttributeArray    - [Out] Optional array of attributes.  Must be not be null to be used.

Returns:
  True or false based on whether the function was able to complete successfully.  It does not indicate whether the function actually 
  unioned the input meshes.  The argument, something_happened, is used for that purpose. 

See Also:
  RhinoMeshBooleanDifference, RhinoMeshBooleanUnion, RhinoMeshBooleanSplit
*/
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanIntersection(
        const ON_SimpleArray<const ON_Mesh*>& InMeshes0, 
        const ON_SimpleArray<const ON_Mesh*>& InMeshes1, 
        double Intersection_Tolerance, 
        double Overlap_Tolerance, 
        bool* something_happened, 
        ON_SimpleArray<ON_Mesh*>& OutMeshes, 
        const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray = 0, 
        ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray = 0
        );
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanIntersection(
  const ON_SimpleArray<const ON_Mesh*>& InMeshes0,
  const ON_SimpleArray<const ON_Mesh*>& InMeshes1,
  double Intersection_Tolerance,
  double Overlap_Tolerance,
  bool* something_happened,
  ON_SimpleArray<ON_Mesh*>& OutMeshes,
  const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray,
  ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray,
  CRhinoMeshBooleanOptions* options
);

/*
Description:
  RhinoMeshBooleanSplit "splits" the meshes in MeshesToSplit with MeshSplitters.  That is, each mesh in MeshesToSplit is 
  split with every mesh in MeshSplitters and for every subsequent splitter mesh all of the previous results ar split.  
  If the attribute arrays are non null then the attributes for MeshesToSplit will be used.
  
Parameters:
  MeshesToSplit          - [In]  Meshes to be split
  MeshSplitters          - [In]  Meshes to split with
  Intersection_Tolerance - [In]  General tolerance to be used in mesh-mesh intersection   
  Overlap_Tolerance      - [In]  Tolerance used to determine overlaps in mesh-mesh intersection 
  something_happened     - [Out] Set to true if something actually happened, independent of function return
  OutMeshes              - [Out] Result of the split of the input meshes
  InputAttributeArray    - [In]  Optional array of attributes.  Must be not be null and have count equal to InMeshes to be used.
  OuputAttributeArray    - [Out] Optional array of attributes.  Must be not be null to be used.

Returns:
  True or false based on whether the function was able to complete successfully.  It does not indicate whether the function actually 
  unioned the input meshes.  The argument, something_happened, is used for that purpose. 

See Also:
  RhinoMeshBooleanDifference, RhinoMeshBooleanUnion, RhinoMeshBooleanIntersection
*/
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanSplit(
        const ON_SimpleArray<const ON_Mesh*>& MeshesToSplit, 
        const ON_SimpleArray<const ON_Mesh*>& MeshSplitters, 
        double Intersection_Tolerance, 
        double Overlap_Tolerance, 
        bool* something_happened, 
        ON_SimpleArray<ON_Mesh*>& OutMeshes,
        bool bMakeBooleanSplits = true,
        const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray = 0, 
        ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray = 0
        );
RHINO_SDK_FUNCTION
bool RhinoMeshBooleanSplit(
        const ON_SimpleArray<const ON_Mesh*>& MeshesToSplit, 
        const ON_SimpleArray<const ON_Mesh*>& MeshSplitters, 
        double Intersection_Tolerance, 
        double Overlap_Tolerance, 
        bool* something_happened, 
        ON_SimpleArray<ON_Mesh*>& OutMeshes,
        bool bMakeBooleanSplits,
        const ON_SimpleArray<const ON_3dmObjectAttributes*>* InputAttributeArray, 
        ON_SimpleArray<const ON_3dmObjectAttributes*>* OuputAttributeArray,
        CRhinoMeshBooleanOptions* options
        );

/*
Description:
  RhinoExplodeMesh "explodes" the input mesh into submeshes where a submesh is a collection of faces that are contained within a closed loop
  of "unwelded" edges.  Unwelded edges are edges where the faces that share the edge have unique mesh vertices (not mesh topology vertices) at both 
  ends of the edge.  See the definition of ON_MeshTopologyEdge and ON_MeshFace for more details.
  
Parameters:
  mesh_in      - [In]  Mesh to be exploded
  meshlist_out - [Out] Array of submeshes that result from exploding the input

Returns:
  The number of meshes that the input mesh was divided into.  If the count is 1 then nothing happened and the output is essentially a
  copy of the input.  Returns 0 on error, -1 if the input mesh has only 1 face, and -2 on user cancel.
*/
RHINO_SDK_FUNCTION
int RhinoExplodeMesh(
        const ON_Mesh* mesh_in,         
        ON_SimpleArray<ON_Mesh*>& meshlist_out
        );

/*
Description:
  RhinoUnWeldMesh will make sure that faces sharing an edge and having a difference of normal greater
  than or equal to angle_tol have unique vertices along that edge, adding vertices if necessary.
  
Parameters:
  OldMesh      - [In]  Mesh to be unwelded
  angle_tol    - [In]  Angle at which to make unique vertices as described above
  pOutputMesh  - [Out] If pOutputMesh is not null then the return will be pOutputMesh;

Returns:
  The an unwelded version of the OldMesh.  
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoUnWeldMesh(const ON_Mesh& OldMesh, 
                         double angle_tol, 
                         ON_Mesh* pOutputMesh = 0);

/*
Description:
  RhinoUnWeldMeshVertexes will make sure that faces sharing a common topological vertex have unique indexes
  into the output meshes m_V array.

Parameters:
  input          - [In]  Mesh to be unwelded
  indexes        - [In]  Topological vertex indices to be unwelded 
                         (see ON_MeshTopology::m_topv, use ON_MeshTopology::m_topv_map to convert from mesh vertex indices)
  ModifyNormals  - [In]  If true, the new vertex normals will be calculated from the face normal.

Returns:
  The an unwelded version of the input mesh.  Should not happen, but calling function confirm that the 
  pointer returned is not null.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoUnWeldMeshVertexes(const ON_Mesh& input, 
                                 ON_SimpleArray<int> indexes, 
                                 bool ModifyNormals);

/*
Description:
  RhinoUnWeldMesh will make sure that faces sharing an edge and having a difference of normal greater
  than or equal to angle_tol have unique vertices along that edge, adding vertices if necessary.
  
Parameters:
  OldMesh       - [In]  Mesh to be unwelded
  angle_tol     - [In]  Angle at which to make unique vertices as described above
  ModifyNormals - [In]  Determines whether new vertex normals will have the same vertex normal as the original (false) 
                        or vertex normals made from the corresponding face normals (true)
  pOutputMesh   - [Out] If pOutputMesh is not null then the return will be pOutputMesh;

Returns:
  The an unwelded version of the OldMesh.  
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoUnWeldMesh(const ON_Mesh& OldMesh, 
                         double angle_tol,
                         bool ModifyNormals,
                         ON_Mesh* pOutputMesh = 0);

/*
Description:
  RhinoWeldMesh will make sure that faces sharing an edge and having a difference of normal greater
  than or equal to angle_tol share vertices along that edge, vertex normals are averaged.
  
Parameters:
  MeshIn    - [In]  Mesh to be welded
  angle_tol - [In]  Angle at which to make unique vertices as described above
  pMeshOut  - [Out] If pMeshOut is not null then the return will be pMeshOut;

Returns:
  The a welded version of the MeshIn.  
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoWeldMesh(const ON_Mesh& MeshIn, 
                       double angle_tol, 
                       ON_Mesh* pMeshOut = 0);

/*
Description:
  RhinoMakeConnectedMeshFaceList uses StartFaceIDX and finds all connected face indexes where
  adjacent face normals meet the criteria of AngleInRadians and bGreaterThan
Parameters:
  pMesh          - [In]  Mesh 
  StartFaceIDX   - [In]  Initial face index
  AngleInRadians - [In]  Angle between adjacent face normals
  bGreaterThan   - [In]  If true angles greater than or equal to are consider, if false, less than or equal to
  FaceIDXArray   - [Out] Array of connected face indexes
Returns:
  The number of face indexes added to FaceIDXArray or 0 on failure.  
*/
RHINO_SDK_FUNCTION
int RhinoMakeConnectedMeshFaceList(
    const ON_Mesh* pMesh,
    int StartFaceIDX, 
    double AngleInRadians, 
    bool bGreaterThan,
    ON_SimpleArray<int>& FaceIDXArray
    );


/*
Description:
  RhinoMakeMeshPartFaceList uses StartFaceIDX and finds all connected face indexes up to unwelded or naked edges.
  If bExtractToNonManifoldEdges is true then non-manifold edges will be considered as unwelded or naked.
Parameters:
  pMesh                      - [In]  Mesh 
  StartFaceIDX               - [In]  Initial face index
  bExtractToNonManifoldEdges - [In]  True means non-manifold edges will be handled like unwelded edges, 
                                     False means they aren't considered.
  FaceIDXArray               - [Out] Array of connected face indexes
Returns:
  The number of face indexes added to FaceIDXArray or 0 on failure.  
*/
RHINO_SDK_FUNCTION
int RhinoMakeMeshPartFaceList(
  const ON_Mesh* pMesh, 
  int StartFaceIDX, 
  bool bExtractToNonManifoldEdges,
  ON_SimpleArray<int>& FaceIDXArray
  );



/*
Description:
Moves mesh vertices that belong to naked edges to neighboring vertices within the specified distance.

Parameters:
MeshArray - [in/out] Input array of meshes.  These meshes are modified.
VertexFlags - [in] Array of arrays of flags to determine which vertices may be modified.
distance - [in] Distance to not exceed when modifying the meshes.

Returns:
Count of all vertices that were modified if successful. -1 for error.
*/
RHINO_SDK_FUNCTION
int RhinoAlignMeshVertices(ON_SimpleArray<ON_Mesh*>& MeshArray, const ON_ClassArray<ON_SimpleArray<bool>>& VertexFlags, double distance);

/*
Description:
Moves mesh vertices that belong to naked edges to neighboring vertices within the specified distance.

Parameters:
meshesSubdsAndPointclouds - [in/out] Input array of meshes, meshes pointclouds and subds.  These inputs are modified.
distance - [in] Distance to not exceed when modifying the meshes.
onlyNaked - [in] Only target naked edges for modification and snapping to.
vertexFlags - [in] Array of arrays of flags to determine which vertices may be modified. Can be null
averagePoints - [in] Instructs Rhino to move candidate points near each other, rather than priviledging one.
Returns:
Count of all vertices that were modified if successful. -1 for error.
*/
RHINO_SDK_FUNCTION
int RhinoAlignVerticesInPlace(ON_SimpleArray<ON_Geometry*>& meshesSubdsAndPointclouds, double distance, bool onlyNaked, const ON_SimpleArray<const ON_SimpleArray<bool>*>* vertexFlags, bool averagePoints);

/*
Description:
Moves mesh vertices that belong to naked edges to neighboring vertices within the specified distance. Does not modify the input array.
Parameters:
results - [out] The returned results. Nullptr is inserted if the input needs no adjusting.
[same as above]
*/
RHINO_SDK_FUNCTION
int RhinoAlignVertices(ON_SimpleArray<const ON_Geometry*>& meshesSubdsAndPointclouds, ON_SimpleArray<ON_Geometry*>& results, double distance, bool onlyNaked, const ON_SimpleArray<const ON_SimpleArray<bool>*>* vertexFlags, bool averagePoints);

/*
Description:
  RhinoCalcTriangleArea calculates the area of a triangle using the Heron formula.

  Heron Formula
  Area=SQRT(S(S-A)(S-B)(S-C)), 
  where S=(A+B+C)/2 or perimeter/2. 

Parameters:
  A - [In]  First point of the triangle
  B - [In]  Second point of the triangle
  C - [In]  Third point of the triangle
Returns:
  The area of the triangle.  
*/
RHINO_SDK_FUNCTION
double RhinoCalcTriangleArea(ON_3dPoint A, ON_3dPoint B, ON_3dPoint C);

/*
Description:
  RhinoGetDuplicateMeshFaces creates a list of duplicate faces in a mesh.
Parameters:
  FaceIDXArray - [Out] List of indexes that are duplicates of other indexes (does not contain pairs)
  pMesh        - [In]  Mesh to examine
Returns:
  Count of duplicate faces in mesh.  
*/
RHINO_SDK_FUNCTION
int RhinoGetDuplicateMeshFaces(ON_SimpleArray<int>& FaceIDXArray, const ON_Mesh* pMesh);

/*
Description:
RhinoGetDuplicateMeshFaces creates a list of duplicate faces in a mesh.
Parameters:
FaceIDXArray - [Out] List of indexes that are duplicates of other indexes (does not contain pairs)
pMesh        - [In]  Mesh to examine
bGetAll      - [In]  If true, all duplicates will be added, false only one.
Returns:
Count of duplicate faces in mesh.
*/
RHINO_SDK_FUNCTION
int RhinoGetDuplicateMeshFaces(ON_SimpleArray<int>& FaceIDXArray, const ON_Mesh* pMesh, bool bGetAll);

/*
Description:
  RhinoGetZeroAreaMeshFaces examines pMesh and adds face indexes to WhollyDegenerateFaces if the face
  is a triangle with zero area or a quad both triangles have zero area.  Face indexes are added to PartiallyDegenerateFaces
  when a quad has one triangle with zero area.
Parameters:
  WhollyDegenerateFaces    - [Out] List of indexes for faces, both triangles and quads, that have zero area.
  PartiallyDegenerateFaces - [Out] List of indexes for quad faces, that have one triangle with zero area.
  pMesh                    - [In]  Mesh to examine
Returns:
  Returns false if pMesh is null or has zero for a face or vertex count, otherwise returns true.  
*/
RHINO_SDK_FUNCTION
bool RhinoGetZeroAreaMeshFaces(ON_SimpleArray<int>& WhollyDegenerateFaces, ON_SimpleArray<int>& PartiallyDegenerateFaces, const ON_Mesh* pMesh);

class RHINO_SDK_CLASS RhinoCheckMeshOutput
{
public:
  RhinoCheckMeshOutput();

  void ZeroCounts();

  // If m_bAbbreviatedVersion is set to true then the feedback from ON_Mesh::IsValid is all that will be added to the ON_TextLog.
  bool m_bAbbreviatedVersion; // Default is false.

  // If m_bCheckForDegenerateFaces is true then the function will look for triangles and quads that have zero area and quads where one sub triangle
  // has zero area.  m_totalDegenerateFaceCt will contain the total degenerate face count for all meshes.
  bool m_bCheckForDegenerateFaces; // Default is true.
  int m_totalDegenerateFaceCt;


  // If m_bCheckForDegenerateNgons is true then the function will look for ngons that have invalid information.
  // m_totalInvalidNgonCt will contain the total degenerate face count for all meshes.
  bool m_bCheckForDegenerateNgons; // Default is true.
  int m_totalInvalidNgonCt;
  
  // If m_bCheckForNakeEdges is true then the function will look for topological edges in the mesh topology where the face count is 1.  
  // m_totalNakedEdgeCt will contain the total naked edge count for all meshes.
  // Note: This check is also made if m_bCheckForDuplicateFaces but the output is not shown if m_bCheckForNakeEdges is false
  bool m_bCheckForNakeEdges; // Default is true.
  int m_totalNakedEdgeCt;

  // If m_bCheckForNonManifoldEdges is true then the function will look for topological edges in the mesh topology where the face count is greater than 2.  
  // m_totalNonManifoldEdgeCt will contain the total naked edge count for all meshes.
  // Note: This check is also made if m_bCheckForDuplicateFaces but the output is not shown if m_bCheckForNonManifoldEdges is false
  bool m_bCheckForNonManifoldEdges; // Default is true.
  int m_totalNonManifoldEdgeCt;

  // If m_bCheckForExtremelyShortEdges is true then the function will look for topological edges where the distance between the endpoints is less than m_DistanceToCheck.  
  // m_totalExtremelyShortEdgeCt will contain the total extremely short edge count for all meshes.
  bool m_bCheckForExtremelyShortEdges; // Default is true.
  double m_DistanceToCheck; // Default is 1.0e-12
  int m_totalExtremelyShortEdgeCt;

  // If m_bCheckForBogusNormals is true then the function will look for vertex normals that are not unit vectors, are zero length or vary greatly from the general 
  // direction of the face normals.
  // m_totalNotUnitVectorNormalCt will contain the total count of normals that are not unit vectors for all meshes.
  // m_totalZeroLengthNormalCt will contain the total count of zero length normals for all meshes.
  // m_totalVertexFaceNormalDifferCt will contain the total count of vertex normals that differ greatly from face normals for all meshes.
  bool m_bCheckForBogusNormals; // Default is true.
  int m_totalNotUnitVectorNormalCt;
  int m_totalZeroLengthNormalCt;
  int m_totalVertexFaceNormalDifferCt;

  // If m_bCheckForDuplicateFaces is true then the function will look for faces that have vertices (not necessarily vertex indexes) that are identical.
  // m_totalDuplicateFaceCt will contain the total count of duplicate faces for all meshes.
  bool m_bCheckForDuplicateFaces;
  int m_totalDuplicateFaceCt; 

  // If m_bCheckForRandomFaceNormals is true then the function will look for adjacent faces that have the same edge orientation.  This typically means that
  // clockwise/counterclockwise orientation, and face normal are screwed up.  
  // m_totalRandomFaceNormalCt will contain the total count of random face normals for all meshes.  
  // Note: UnifyMeshNormals is a command that may be able to resolve this issue
  bool m_bCheckForRandomFaceNormals; // Default is true.
  int m_totalRandomFaceNormalCt;

  // If m_bCheckForDisjointMeshes is true then the function will look for faces in the mesh that are totally isolated from other faces in the mesh.  
  // ie. an island of faces (or many islands) that have a boundary of naked edges
  // m_totalDisjointMeshCt will contain the total count of disjoint pieces for all meshes.  
  bool m_bCheckForDisjointMeshes; // Default is true.
  int m_totalDisjointMeshCt; 

  // If m_bCheckForUnusedVertices is true then the function will look for vertices that are not referenced by any face in the mesh.  
  // m_totalDisjointMeshCt will contain the total count of unused vertices for all meshes.  
  bool m_bCheckForUnusedVertices; // Default is true.
  int m_totalUnusedVertexCt;

  bool m_bCheckForSelfIntersection;
  int m_totalClashingPairsCt;

private:

  // Values below are reserved for future use.
  bool m_bCheckForReserved1;
  int m_totalReserved1Ct;

  bool m_bCheckForReserved2;
  int m_totalReserved2Ct;
};

/*
Description:
  RhinoCheckMesh examines the input meshes contained in pMeshObjArray.  It creates a description of what it finds wrong with the input in text_log.
  The various things the function checks for are described below in the output parameters.
Parameters:
  pMeshObjArray - [In]      Mesh(es) to examine.  Numbers in output make most sense when one mesh is examined at a time though.
  text_log      - [Out]     ON_TextLog with printed output of what was discovered based on the things set to check in the output parameter, 
                            include description of the CRhinoMeshObject.
  output        - [In/Out]  Flags to determine what to check and counts for those things that were checked.  See RhinoCheckMeshOutput above.
Returns:
  Returns true if the function determined there was anything wrong with any of the input meshes, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCheckMesh(const ON_SimpleArray<const CRhinoMeshObject*>& pMeshObjArray, ON_TextLog& text_log, RhinoCheckMeshOutput& output);

/*
Description:
  RhinoCheckMesh examines the input meshes contained in pMeshObjArray.  It creates a description of what it finds wrong with the input in text_log.
  The various things the function checks for are described below in the output parameters.
Parameters:
  pMeshObjArray - [In]      Mesh(es) to examine.  Numbers in output make most sense when one mesh is examined at a time though.
  text_log      - [Out]     ON_TextLog with printed output of what was discovered based on the things set to check in the output parameter
  output        - [In/Out]  Flags to determine what to check and counts for those things that were checked.  See RhinoCheckMeshOutput above.
Returns:
  Returns true if the function determined there was anything wrong with any of the input meshes, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCheckMesh(const ON_SimpleArray<const ON_Mesh*>& pMeshArray, ON_TextLog& text_log, RhinoCheckMeshOutput& output);

/*
Description:
  RhinoCheckMesh examines the input meshand creates a description of what it finds wrong with the input in text_log.
  The various things the function checks for are described below in the output parameters.
Parameters:
  pMesh         - [In]      Mesh to examine.
  text_log      - [Out]     ON_TextLog with printed output of what was discovered based on the things set to check in the output parameter
  output        - [In/Out]  Flags to determine what to check and counts for those things that were checked.  See RhinoCheckMeshOutput above.
Returns:
  Returns true if the function determined there was anything wrong with any of the input meshes, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCheckMesh(const ON_Mesh* pMesh, ON_TextLog& text_log, RhinoCheckMeshOutput& output);

/*
Description:
  RhinoCullZeroAreaMeshFaces Deletes or fixes mesh faces that have 0 area.
  
Parameters:
  MeshIn    - [In]  Mesh to be culled
  removed_faces_count - [In]  Number of removed wholly degenerate faces
  fixed_faces_count - [In]  Number of fixed partially degenerate faces 
  pMeshOut  - [Out] If pMeshOut is not null then the return will be pMeshOut

Returns:
  The a culled version of the MeshIn.  

Notes:
  Reference ON_Mesh::CullDegenerateFaces for alternate implementation to cull 
  degenerate faces using vertices
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoCullZeroAreaMeshFaces(const ON_Mesh& MeshIn, 
                       int& removed_faces_count, 
                       int& fixed_faces_count,
                       ON_Mesh* pMeshOut = 0);


/*
Description:
RhinoExtractNonManifoldMeshEdges extracts non manifold edges.
Parameters:
MeshIn - [In]  Input mesh
bSelective - [In] if true, then extract hanging faces only
minFaceCtAroundEdge - [In] NME must have greater than or equal to be included.
MeshOut - [Out] output mesh
pMeshExtractionOut  - [Out] output extracted non-manifold part of the mesh
Returns:
The number of extracted faces.
*/
RHINO_SDK_FUNCTION
int RhinoExtractNonManifoldMeshEdges(
  const ON_Mesh& MeshIn,
  bool bSelective,
  int minFaceCtAroundEdge,
  ON_Mesh& MeshOut,
  ON_Mesh& MeshExtractionOut);

/*
Description:
  RhinoExtractNonManifoldMeshEdges extracts non manifold edges.
Parameters:
  MeshIn - [In]  Input mesh
  bSelective - [In] if true, then extract hanging faces only
  MeshOut - [Out] output mesh
  pMeshExtractionOut  - [Out] output extracted non-manifold part of the mesh
Returns:
  The number of extracted faces.  
*/
RHINO_SDK_FUNCTION
int RhinoExtractNonManifoldMeshEdges(const ON_Mesh& MeshIn, 
                    bool bSelective,
                    ON_Mesh& MeshOut,
                    ON_Mesh& MeshExtractionOut);
/*
Description:
  RhinoExtractDuplicateMeshFaces extractsduplicate mesh faces.
Parameters:
  MeshIn - [In]  Input mesh
  MeshOut - [Out] output mesh without duplicate faces
  MeshExtractionOut - [Out] output extracted duplicate faces
Returns:
  The number of extracted duplicate faces.  
*/
RHINO_SDK_FUNCTION
int RhinoExtractDuplicateMeshFaces(const ON_Mesh& MeshIn, 
              ON_Mesh& MeshOut,
              ON_Mesh& MeshExtractionOut);

/*
Description:
RhinoExtractDuplicateMeshFaces extractsduplicate mesh faces.
Parameters:
MeshIn - [In]  Input mesh
bBoth, [In] set to true if you want the pair of duplicate faces extracted.
MeshOut - [Out] output mesh without duplicate faces
MeshExtractionOut - [Out] output extracted duplicate faces
Returns:
The number of extracted duplicate faces.
*/
RHINO_SDK_FUNCTION
int RhinoExtractDuplicateMeshFaces(const ON_Mesh& MeshIn,
  bool bBoth,
  ON_Mesh& MeshOut,
  ON_Mesh& MeshExtractionOut);
/*
Description:
  RebuildMeshNormals Removes mesh normals and reconstructs the face and 
  vertex normals based on the orientation of the faces.
  
Parameters:
  MeshIn    - [In]  Input mesh

Returns:
  New mesh.  
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoRebuildMeshNormals(const ON_Mesh* MeshIn);


/*
Description:
  RhinoExtractMeshFaces extracts mesh faces using list of indices
  and return two meshes.
  
Parameters:
  FaceIDXArray - [in] input list of face indices to be extracted
  MeshIn    - [In/out]  Input mesh. Output with extracted faces removed
  MeshExtracted - [out] extracted faces

Returns:
  true if at least one face was extracted.  
*/
RHINO_SDK_FUNCTION
bool RhinoExtractMeshFaces(	ON_SimpleArray<int>& FaceIDXArray, 
              ON_Mesh& MeshIn, 
              ON_Mesh& MeshExtracted);

/*
Description:
  RhinoReduceMesh reduces mesh polygon count.

Parameters:
  mesh                 - Triangular mesh to be reduced
  iDesiredPolygonCount - Target polygon count
  bAllowDistortion     - If true mesh appearance is not changed even if the target polygon count is not reached
  iAccuracy            - Integer from 1 to 10 telling how accurate reduction algorithm to use. Greater number gives
                         more accurate results.
  bNormalizeMeshSize   - If true mesh is fitted to an axis aligned unit cube until reduction is complete
  pProgressReporter    - Null or a pointer to an ON_ProgressReporter object. Provides custom progress information to caller.
  pCancelHandler       - Null or a pointer to an ON_Terminator object. Provides a custom way to handle cancelling.
  pStrProblemOut       - Null or a pointer to an ON_wString object. Reports possible problems.
  pFaceTagArray        - Null or a pointer to an ON_SimpleArray<int> object with element count equal to mesh face count.
                         Each element is a user defined tag for the mesh face with the same index. Can be used to identify
                         mesh faces before and after calling RhinoReduceMesh.
  pLockedComponents    - Null or a pointer to an array of ON_COMPONENT_INDEX objects containing mesh vertex components that
                         must be kept untouched.

Returns:
  True if mesh is successfully reduced and false if mesh could not be reduced for some reason. If return value is false
  then pStrProblemOut may have information what went wrong.
*/
RHINO_SDK_FUNCTION
bool RhinoReduceMesh(ON_Mesh & mesh,
                     int iDesiredPolygonCount,
                     bool bAllowDistortion = true,
                     int iAccuracy = 10,
                     bool bNormalizeMeshSize = true,
                     ON_ProgressReporter * pProgressReporter = 0,
                     ON_Terminator * pCancelHandler = 0,
                     ON_wString * pStrProblemOut = 0,
                     ON_SimpleArray<int> * pFaceTagArray = 0,
                     ON_SimpleArray<ON_COMPONENT_INDEX>* pLockedComponents = 0);
/*
Description:
  Set of parameters for RhinoReduceMesh function.
*/
class RHINO_SDK_CLASS RhinoReduceMeshParameters
{
public:
  // Constructs new reduce mesh parameters with default values
  RhinoReduceMeshParameters();

  // Target polygon count
  int m_iDesiredPolygonCount;

  // If true mesh appearance is not changed even if the target polygon count is not reached
  bool m_bAllowDistortion;

  // Integer from 1 to 10 telling how accurate reduction algorithm to use. Greater number gives
  // more accurate results.
  int m_iAccuracy;

  // If true mesh is fitted to an axis aligned unit cube until reduction is complete
  bool m_bNormalizeMeshSize;

  // Reports possible problems
  ON_wString m_sProblemOut;
 
  // Null or a pointer to an ON_SimpleArray<int> object with element count equal to mesh face count.
  // Each element is a user defined tag for the mesh face with the same index.Can be used to identify
  // mesh faces before and after calling RhinoReduceMesh.
  ON_SimpleArray<int>* m_pFaceTagArray;

  // Null or a pointer to an ON_SimpleArray<ON_COMPONENT_INDEX> object containing components of type
  // ON_COMPONENT_INDEX::meshtop_vertex and ON_COMPONENT_INDEX::mesh_vertex.
  // Vertices listed here are not moved or deleted in reduction process.
  // Each mesh_vertex will lock the corresponding meshtop_vertex. In other words it is not possible to have a
  // locked and non-locked mesh vertex at the same location. 
  ON_SimpleArray<ON_COMPONENT_INDEX>* m_pLockedComponents;

  // Null or a pointer to an ON_ProgressReporter object. Provides custom progress information to caller
  ON_ProgressReporter* m_pProgressReporter;

  // Null or a pointer to an ON_Terminator object. Provides a custom way to handle cancelling.
  ON_Terminator* m_pCancelHandler;
private:

  // Reserved space for future modifications
  ON__UINT_PTR m_reserved;
  char __padding[64]; 
};

/*
Description:
  RhinoReduceMesh reduces mesh polygon count.

Parameters:
  mesh                 - Triangular mesh to be reduced
  parameters           - Parameters for reduction

Returns:
  True if mesh is successfully reduced and false if mesh could not be reduced for some reason. If return value is false
  then parameters.m_sProblemOut may have information what went wrong.
*/
RHINO_SDK_FUNCTION
bool RhinoReduceMesh(ON_Mesh& mesh, RhinoReduceMeshParameters& parameters);

#pragma region RH_C_SHARED_ENUM [RhPipeCapType] [Rhino.Geometry.MeshPipeCapStyle]
///<summary>Enumerates methods to end a mesh pipe.</summary>
// DEPRECATED -- This is now a duplicate of ON_CurvePiping::CapTypes.
enum class RhPipeCapType
{
  ///<summary>Capping is skipped. The resulting object will be simpler.</summary>
  None = 0,

  ///<summary>A flat surface will cap the pipe.</summary>
  Flat = 1,

  ///<summary>A simple construction will cap the pipe.</summary>
  Box = 2,

  ///<summary>A meridians-and-parallels hemisphere construction will cap the pipe.</summary>
  Dome = 3
};
#pragma endregion

/*
Description:
  Contains the arguments to the RhinoMeshPipeFromCurve function.
*/
class RHINO_SDK_CLASS RhMeshPipeArgs
{
public:
  // The constructor sets the default arguments.
  RhMeshPipeArgs();
  ~RhMeshPipeArgs();
  RhMeshPipeArgs(const RhMeshPipeArgs&);
  RhMeshPipeArgs& operator=(const RhMeshPipeArgs&);

  const ON_Linetype* GetLinetype() const;
  void SetLinetype(const ON_Linetype* linetype);

  void SetDocument(unsigned int serialNumber);
  unsigned int DocumentSerialNumber() const;
public:
  double m_dRadius;        // The radius of the pipe (minimum value 0.0001).
  int m_iSegments;         // The number of segments in the pipe (minimum value 2).
  int m_iAccuracy;         // The accuracy of the pipe.
  RhPipeCapType m_capType; // The type of cap to be created at the ends of the pipe.
  bool m_bFaceted;         // Specifies whether the pipe is faceted or not.

  // An array of intervals specifying which parts of the curve
  // should be piped. Leave empty to pipe the whole curve.
  ON_SimpleArray<ON_Interval> m_aVisibleIntervals;

private:
  ON_Linetype* m_linetype = nullptr;
  unsigned int m_doc_sn = 0;
  char __padding[52]; // Reserved space for future modifications.
};

/*
Description:
  Create a mesh pipe from a curve.
Parameters:
  curve          - [in] The curve to create a mesh pipe from.
  args           - [in] The arguments used to create the mesh pipe.
  pCancelHandler - [in] Pass an ON_Terminator to provide ability to cancel the operation, use NULL otherwise.
Returns:
  The created mesh pipe. NULL if failed or canceled.
See Also:
  RhinoDisplaceMesh, RhinoEdgeSoftenMesh, RhinoAddShutLinesToMesh
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshPipeFromCurve( const ON_Curve& curve, const RhMeshPipeArgs& args, ON_Terminator* pCancelHandler = 0 );


///<summary>Enumerates sweep resolution formulas</summary>
// DEPRECATED -- This is now a duplicate of ON_Displacement::SweepResolutionFormulas.
enum class RhDisplacementSweepResolutionFormula : int
{
  ///<summary>Default formula.</summary>
  Default = 0,

  ///<summary>Formula used in Rhino 5. Dependent on absolute tolerance.</summary>
  AbsoluteToleranceDependent = 1,
};

// A class containing all displacement options.
class RHINO_SDK_CLASS RhDisplacementArgs
{
public:
  double m_dWhite;             // The amount of displacement for the white color in the texture.
  double m_dBlack;             // The amount of displacement for the black color in the texture.
  double m_dSweepPitch;        // Specifies how densely the object is initially subdivided.
                               // The lower the value, the higher the resolution of the displaced mesh.
  int m_iRefineStepCount;      // Specifies the number of refinement passes.
  double m_dRefineSensitivity; // Specifies how sensitive the divider for contrasts is on the displacement texture.
                               // Specify 1 to split all mesh edges on each refine step.
                               // Specify 0.99 to make even slight contrasts on the displacement texture cause edges to be split.
                               // Specifying 0.01 only splits edges where heavy contrast exists.
  double m_dPostWeldAngle;     // Specifies the maximum angle between face normals of adjacent faces
                               // that will get welded together.
  int m_iFairingAmount;        // Straightens rough feature edges. The value specifies the number of passes.
  int m_iFaceLimit;            // Runs a mesh reduction as a post process to simplify the result of displacement.
                               // to meet the specified number of faces.
  int m_iChannelNumber;        // Mapping channel number for the displacement mapping.
  int m_meshMemoryLimit;       // Specifies in megabytes how much memory can be allocated for use by the displacement mesh.

  RhDisplacementArgs(); // The constructor sets the default option values.

  RhDisplacementSweepResolutionFormula m_sweepResolutionFormula; // Specifies which formula is used to calculate sweep resolution from sweep pitch.
  double m_dAbsoluteTolerance; // Absolute tolerance used for sweep resolution when calculation formula needs it.

  bool operator == (const RhDisplacementArgs& other) const;
  bool operator != (const RhDisplacementArgs& other) const;

private:
  char __padding[52]; // Reserved space for future modifications.
};

class CRhRdkTexture;
class RhDisplacementImpl;

// Represents surface displacement on an object.
class RHINO_SDK_CLASS RhDisplacement
{

public:
  // Description:
  //   The constructor.
  // Parameters:
  //   pRdkTexture       - [in] The texture used as displacement
  //   dBlack            - [in] Value considered lowest point of the displacement texture.
  //   dWhite            - [in] Value considered highest point of the displacement texture.
  //   textureMapping    - [in] The texture mapping of the mesh.
  //   mappingTransform  - [in] Texture mapping transform.
  //   instanceTransform - [in] Instance transformation of the mesh.
  RhDisplacement(
    const CRhRdkTexture* pRdkTexture,
    double dBlack,
    double dWhite,
    const ON_TextureMapping& textureMapping,
    const ON_Xform& mappingTransform,
    const ON_Xform& instanceTransform);

  virtual ~RhDisplacement();

  RhDisplacementImpl* m_pImpl;
};

/*
Description:
  Create a mesh displaced by a texture. This operation can be canceled by pressing Esc.
Parameters:
  originalMesh      - [in] The mesh which we want to displace.
  displacement      - [in] The texture and transformation data representing the displacement.
  args              - [in] The arguments passed to the displacement algorithm.
  problemString     - [out] If the algorithm fails, this string contains a description of what went wrong.
  pCancelHandler    - [in] Pass an ON_Terminator to provide ability to cancel the operation, use NULL otherwise.
  pProgressReporter - [in] Used to display algorithm progress information.
                           Pass in NULL to use default progress reporting functionality.
Returns:
  A displaced mesh. NULL if failed or canceled.
See Also:
  RhinoMeshPipeFromCurve, RhinoEdgeSoftenMesh, RhinoAddShutLinesToMesh
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoDisplaceMesh(
  const ON_Mesh& originalMesh,
  const RhDisplacement& displacement,
  const RhDisplacementArgs& args,
  ON_wString& problemString,
  ON_Terminator* pCancelHandler = 0,
  ON_ProgressReporter* pProgressReporter = 0);


/*
Description:
  Contains the arguments to the RhinoEdgeSoftenMesh function.
*/
class RHINO_SDK_CLASS RhEdgeSofteningArgs
{
public:
  double m_dSoftening;    // The softening radius.
  bool m_bChamfer;        // Specifies whether to chamfer the edges.
  bool m_bFaceted;        // Specifies whether the edges are faceted.
  bool m_bForceSoftening; // Specifies whether to soften edges despite too large a radius.

  // Threshold angle (in degrees) which controls whether an edge is softened or not.
  // The angle refers to the angles between the adjacent faces of an edge.
  double m_dEdgeAngleThreshold;

  // The constructor sets the default values of the arguments.
  RhEdgeSofteningArgs();

private:
  char __padding[64]; // Reserved space for future modifications.
};

/*
Description:
  Soften the edges of a mesh.
Parameters:
  originalMesh   - [in] The mesh whose edges we want to soften.
  args           - [in] The arguments used in edge softening.
  pCancelHandler - [in] Pass an ON_Terminator to provide ability to cancel the operation, use NULL otherwise.
Returns:
  A new mesh with softened edges. NULL if failed.
See Also:
  RhinoMeshPipeFromCurve, RhinoDisplaceMesh, RhinoAddShutLinesToMesh
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoEdgeSoftenMesh( const ON_Mesh& originalMesh, const RhEdgeSofteningArgs& args, ON_Terminator* pCancelHandler = 0 );


// Shut-lining curve arguments
class RHINO_SDK_CLASS RhShutLiningCurveArgs
{
public:
  const ON_Curve* m_curve; // The curve used as a shutline.
  bool m_bEnabled;         // Specifies whether this curve should be shutlined.
  double m_dRadius;        // The radius of the shutline.
  int m_iProfile;          // The profile of the shutline.
  bool m_bPull;            // Specifies whether the curve should be pulled to the mesh.
  bool m_bIsBump;          // Specifies whether the shutline is coming out of the mesh.

  // An array of intervals specifying which parts of the curve
  // should be shutlined. Leave empty to pipe the whole curve.
  ON_SimpleArray<ON_Interval> m_aCurveIntervals;

  // The constructor sets the default arguments.
  RhShutLiningCurveArgs();

private:
  char __padding[64]; // Reserved space for future modifications.
};

// Shut-lining arguments
class RHINO_SDK_CLASS RhShutLiningArgs
{
public:
  bool m_bFaceted;     // Specifies whether the shutline is faceted.
  double m_dTolerance; // The tolerance of the shutline.
  ON_ClassArray<RhShutLiningCurveArgs> m_curveArgs; // A collection of curve arguments.

  // The constructor sets the default arguments.
  RhShutLiningArgs();

private:
  char __padding[64]; // Reserved space for future modifications.
};

/*
Description:
  Use curves to add shutlines to a mesh.
Parameters:
  originalMesh   - [in] The mesh to apply shutlining to.
  args           - [in] The arguments used in shutlining.
  pCancelHandler - [in] Pass an ON_Terminator to provide ability to cancel the operation, use NULL otherwise.
Returns:
  A new mesh with shutlining. NULL if failed.
See Also:
  RhinoMeshPipeFromCurve, RhinoEdgeSoftenMesh, RhinoDisplaceMesh
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoAddShutLinesToMesh(const ON_Mesh& originalMesh, const RhShutLiningArgs& args, ON_Terminator* pCancelHandler = 0);

/*
Description:
Collapses a mesh edge based on criteria found in Stan Melax's polygon reduction,
see http://pomax.nihongoresources.com/downloads/PolygonReduction.pdf

Parameters:
mesh          - [In/Out] Input mesh
edgeIndex     - [In]     Edge index to collapse

Returns:
True on success.
*/
RHINO_SDK_FUNCTION
bool RhinoCollapseMeshEdge(ON_Mesh& mesh, int edgeIndex);

/*
Description:
Collapses a mesh vertex based on criteria found in Stan Melax's polygon reduction,
see http://pomax.nihongoresources.com/downloads/PolygonReduction.pdf

Parameters:
mesh            - [In/Out] Input mesh
topVertexIndex1 - [In]     1st vertex index to collapse
topVertexIndex2 - [In]     2nd vertex index to collapse (set to -1, if it doesn't matter what vertex topVertexIndex1 is collapsed to)

Returns:
True on success.
*/
RHINO_SDK_FUNCTION
bool RhinoCollapseMeshVertex(ON_Mesh& mesh, int topVertexIndex1, int topVertexIndex2);

/*
Description:
Collapses a mesh face based on criteria found in Stan Melax's polygon reduction,
see http://pomax.nihongoresources.com/downloads/PolygonReduction.pdf

Parameters:
mesh          - [In/Out] Input mesh
faceIndex     - [In]     Face index to collapse

Returns:
True on success.
*/
RHINO_SDK_FUNCTION
bool RhinoCollapseMeshFace(ON_Mesh& mesh, int faceIndex);

/*
Description:
Collapses a multiple mesh edges(faces), determined by edge length, based on criteria 
found in Stan Melax's polygon reduction,
see http://pomax.nihongoresources.com/downloads/PolygonReduction.pdf

Parameters:
mesh         - [In / Out] Input mesh
bGreaterThan - [In]       Determines whether edges with lengths greater than or equal to edgeLength are considered candidates.
edgeLength   - [In]       Length of edge to consider as candidates.

Returns:
Number of edges that were collapsed in the process.  This number may differ from the initial number of edges that meet
the input criteria because the lengths of some initial edges may be altered as other edges are collapsed.  Returns -1 for 
general failure (like bad topology or index out of range) or -2 if all of the edges would be collapsed and the resulting mesh would be invalid.
*/
RHINO_SDK_FUNCTION
int RhinoCollapseMeshEdges(ON_Mesh& Mesh, bool bGreaterThan, double edgeLength);

/*
Description:
Collapses a multiple mesh faces, determined by face area, based on criteria 
found in Stan Melax's polygon reduction,
see http://pomax.nihongoresources.com/downloads/PolygonReduction.pdf

Parameters:
mesh                   - [In / Out] Input mesh
double lessThanArea    - [In]       Area in which faces are selected if their area is less than or equal to
double greaterThanArea - [In]       Area in which faces are selected if their area is greater than or equal to

Returns:
Number of faces that were collapsed in the process.  This number may differ from the initial number of faces that meet
the input criteria because the areas of some initial faces may be altered as other faces are collapsed.

Remarks:
The face area must be both <= LessThanArea and >= GreaterThanArea in order to be considered.  Use large numbers
for lessThanArea or zero for greaterThanArea to simulate an or.
*/
RHINO_SDK_FUNCTION
int RhinoCollapseMeshFacesByArea(ON_Mesh& Mesh, double lessThanArea, double greaterThanArea);

/*
Description:
Collapses a multiple mesh faces, determined by face aspect ratio, based on criteria found in Stan Melax's polygon reduction,
see http://pomax.nihongoresources.com/downloads/PolygonReduction.pdf

Parameters:
mesh         - [In / Out] Input mesh
aspectRatio  - [In]       Faces with an aspect ratio less than aspectRatio are considered as candidates.

Returns:
Number of faces that were collapsed in the process.  This number may differ from the initial number of faces that meet
the input criteria because the aspect ratios of some initial faces may be altered as other faces are collapsed.
*/
RHINO_SDK_FUNCTION
int RhinoCollapseMeshFacesByAspectRatio(ON_Mesh& Mesh, double aspectRatio);

/*
Description:
Worker function to determine aspect ratios for the faces in the function above;
Parameters:
A, B, C, D - [In] 4 vertices of a mesh face, face_points[2] and face_points[3] are identical for triangles

Returns:
Aspect ratio of face.

Remarks:
This function was is really meant for public consumption outside of the context of  
RhinoCollapseMeshFacesByAspectRatio.  It's handy if you'd like to determine what faces
might be considered as candidates prior to calling RhinoCollapseMeshFacesByAspectRatio.
*/
RHINO_SDK_FUNCTION
double RhinoCalculateAspectRatio(ON_3dPoint A, ON_3dPoint B, ON_3dPoint C, ON_3dPoint D);

/*
Description:
Given a starting "naked" edge index, this function attempts to determine a "hole" by chaining additional
naked edges together until if returns to the start index. Then it triangulates the closed polygon and either
adds the faces to pMesh or optionally to pMeshHolePatch.

Parameters:
StartEdgeIdx   - [In]        Starting naked edge index
pMesh          - [In / Out]  Input mesh
pMeshHolePatch - [Out]       Optional output.  New mesh is created rather than adding to pMesh.

Returns:
true on success

Remarks:
"naked" in the context of RhinoFillMeshHole, RhinoFillMeshHoles and RhinoDupMeshBorder, refers to
edges that border a single face.
*/
RHINO_SDK_FUNCTION
bool RhinoFillMeshHole(const int StartEdgeIdx, ON_Mesh* pMesh, ON_Mesh** pMeshHolePatch = nullptr);

/*
Description:
This function attempts to determine "holes" in the input mesh by chaining naked edges together. 
Then it triangulates the closed polygons adds the faces to pMesh.

Parameters:
pMesh          - [In / Out]  Input mesh

Returns:
true on success

Remarks:
This function does not differentiate between inner and outer naked edges.  If you need that,
it would be better to use RhinoFillMeshHole.  You should be able to use ON_MeshTopologyFace::m_reve 
to determine what ought to be inner and outer.
*/
RHINO_SDK_FUNCTION
bool RhinoFillMeshHoles(ON_Mesh* pMesh);

/*
Description:
Given a starting (naked) edge ref, this function attempts to determine a hole/border by chaining additional
naked edges together until if returns to the start index. 

Parameters:
pMeshEdgeRef   - [In]        Starting naked edge reference

Returns:
pointer to curve on success, nullptr on failure
*/
RHINO_SDK_FUNCTION
ON_PolylineCurve* RhinoDupMeshBorder(const ON_MeshComponentRef* pMeshEdgeRef);

/*
Description:
This function will swap the orientation of an edge shared by two (only) triangles.
-----      -----
|  /|      |\  |
| / |  ->  | \ |
|/  |      |  \|
-----      -----

Parameters:
mesh         - [In]  Input mesh
topei        - [In]  Topological edge index to be swapped
pOutputMesh  - [In]  Optional output mesh.  If null, the return indicates whether the edge could be swapped.
                     If not null, and success, *pOutputMesh will be a copy of mesh with the edge swapped.

Returns:
true on success.
*/
RHINO_SDK_FUNCTION
bool RhinoSwapMeshEdge(const ON_Mesh* pInputMesh, int topei, ON_Mesh** pOutputMesh = nullptr);

/*
Description:
This function will split up a mesh edge at the given parameters.

Parameters:
pMesh        - [In / Out]  Mesh to be altered
topei        - [In]        Topological edge index to be swapped
params       - [In]        Parameters at which to split up the edge

Returns:
true on success. 

Remarks:
All parameters must be 0.0 <= t >= 1.0 or the function will bail early and return false;
*/
RHINO_SDK_FUNCTION
bool RhinoSplitMeshEdge(ON_Mesh* pMesh, const int edgeidx, const ON_SimpleArray<double>& params);

/*
Description:
This function will split up a mesh edge at the given points.

Parameters:
pMesh        - [In / Out]  Mesh to be altered
topei        - [In]        Topological edge index to be swapped
points       - [In]        Points at which to split up the edge

Returns:
true on success.

Remarks:
Points must be on the line, created by the edge at edgeidx, on or between the start and end points.
If any point does not fit that criteria the function will bail early and return false.
*/
RHINO_SDK_FUNCTION
bool RhinoSplitMeshEdge(ON_Mesh* pMesh, const int edgeidx, const ON_SimpleArray<ON_3dPoint>& points);

/*
Description:
  Function attempts to "heal" naked edges in a mesh based on a given distance.  
  First attempts to move vertices to neighboring vertices that are within that
  distance away. Then it finds edges that have a closest point to the vertex within
  the distance and splits the edge.  When it finds one it splits the edge and 
  makes two new edges using that point.
Parameters:
  pMeshIn - [In]  Input mesh
  MeshOut - [Out] Output Mesh
  distance - [In] Distance to not exceed when modifying the mesh.
Returns:
  True or false depending on the success of the modification.
*/
RHINO_SDK_FUNCTION
bool RhinoHealMesh(const ON_Mesh* pMeshIn, ON_Mesh& MeshOut, const double distance);

/* Description:
Function that will fill in the result array with the per-vertex Voronoi areas of a triangle mesh.
Will fail on a quad mesh.

Parameters:
mesh - [in] Input mesh
result - [out] result, per vertex or per topology vertex
useMeshTopology - optional, boolean toggle to indicate if mesh topology vertices or mesh vertices are used.

Returns:
  True or false depending on the success of the computation.
*/
RHINO_SDK_FUNCTION
bool MeshVoronoiAreas(const ON_Mesh& mesh, ON_SimpleArray<double>& result, bool useMeshTopology=true);


/* Description:
Function that will apply the (pure, not area-weighted) cotangent-weighted Laplacian operator to a scalar field, specified
at the topology vertices of the mesh.
Parameters:
mesh - [in] Input mesh
scalarValues  - [in, out] the scalar values to be Laplaced on input, and the result on output.
Their count must be the number of the mesh's topology vertices.
Returns:
  True or false depending on the success of the operation.
*/
RHINO_SDK_FUNCTION
bool RhinoApplyTopologyLaplacianToScalar(const ON_Mesh& mesh, ON_SimpleArray<double>& scalarValues);


/*
Description:
  Function that computes the discrete Gaussian curvatures, by taking the angle defects and dividing them
  by the face areas.
  That definition has the desired property that its integral gives ON_2PI times the Euler characteristic,
  at least on a closed mesh. Thus, it satisfies the Gauss-Bonnet theorem which arguably characterizes the
  Gaussian curvature fully.
  Note that this function will triangulate internally along the short diagonal, but leave the mesh untouched.
  The function takes care of mesh topology and quads.
  Use at your own risk on nonmanifold meshes.
Parameters:
  MeshIn - [In] Input mesh
  result - [out] array to be filled with the curvatures. On success, its Count() will be MeshIn.VertexCount().
  globalAngleDefect - [out] optional, the sum of all angle defects before dividing by the vertex areas
Returns:
  True or false depending on the success of the computation.
*/
RHINO_SDK_FUNCTION
bool RhinoDiscreteGaussianMeshCurvature(const ON_Mesh& MeshIn, ON_SimpleArray<double>& result, double* globalAngleDefect = nullptr);


/*
Description:
  Function that computes the discrete Gaussian curvatures like the preceding one, but trying to give a meaningful result on naked edges, too.
  Moreover, it tries to find each "unwelded" vertex and treat it like such a naked edge.
Parameters:
  MeshIn - [In] Input mesh
  result - [out] array to be filled with the curvatures. On success, its Count() will be MeshIn.VertexCount().
Returns:
  True or false depending on the success of the computation.
*/
RHINO_SDK_FUNCTION
bool RhinoDiscreteGaussianMeshCurvatureIncludingNaked(const ON_Mesh& MeshIn, ON_SimpleArray<double>& result);



/*
Description:
  Function that computes the discrete mean curvatures, by taking the dihedral angles of the topology edges and dividing them
  by the edge lengths.
  The function takes care of mesh topology and quads by internal triangulation.
  Use at your own risk on nonmanifold meshes.

Parameters:
  MeshIn - [In] Input mesh
  result - [out] array to be filled with the curvatures. On success, its Count() will be MeshIn.VertexCount().
Returns:
  True or false depending on the success of the computation.
*/
RHINO_SDK_FUNCTION
bool RhinoDihedralMeanMeshCurvature(const ON_Mesh& MeshIn, ON_SimpleArray<double>& result);

/*
Description:
  Function that computes the discrete mean curvatures like the preceding one, but trying to give a meaningful result on naked edges, too.
  Moreover, it tries to find each "unwelded" vertex and treat it like such a naked edge.
Parameters:
  MeshIn - [In] Input mesh
  result - [out] array to be filled with the curvatures. On success, its Count() will be MeshIn.VertexCount().
Returns:
  True or false depending on the success of the computation.
*/
RHINO_SDK_FUNCTION
bool RhinoDihedralMeanMeshCurvatureIncludingNaked(const ON_Mesh& MeshIn, ON_SimpleArray<double>& result);


/*
Description:
  Function that computes the discrete mean curvatures, by using the cotangent-weighted Laplacian of the coordianate functions.
  The function takes care of mesh topology and quads by internal triangulation.
  Use at your own risk on nonmanifold meshes.

Parameters:
  MeshIn - [In] Input mesh
  result - [out] array to be filled with the curvatures. On success, its Count() will be MeshIn.VertexCount().
Returns:
  True or false depending on the success of the computation.
*/
RHINO_SDK_FUNCTION
bool RhinoLaplacianMeanMeshCurvature(const ON_Mesh& MeshIn, ON_SimpleArray<double>& result);

/*
Description:
  Function that computes the discrete mesh curvatures.
  On success, the m_K member of the mesh will be filled in.
  On all naked vertices, the value will be unset.
  This function triangulates the mesh using the default method.
  Parameters:
  mesh - [In, Out] Scrutinee
  useLaplacian - [In] optional boolean toggle. If false (default), mean curvature is estimated in a robust but simple way. 
    If true, the cotangent-weighted Laplacian is used.
  Returns:
  True or false depending on the success of the computation.
 */
RHINO_SDK_FUNCTION
bool RhinoComputePrincipalMeshCurvatures(ON_Mesh& mesh, bool useLaplacian = true);


/*
Description:
  Creates a icospherical mesh. A mesh icosphere differs from a standard UV mesh
  sphere in that it's vertices are evenly distributed. A mesh icosphere starts
  from an icosahedron (a regular polyhedron with 20 equilateral triangles).
  It is then refined by splitting each triangle into 4 smaller triangles.
  This splitting can be done several times.
Parameters:
  sphere       - [in] The input sphere provides the orienting plane and radius.
  subdivisions - [in] The number of times you want the faces split, where
                      0 <= subdivisions <= 7. Note, the total number of mesh
                      faces produces is: 20 * (4 ^ subdivisions)
Returns:
  A pointer to a welded mesh icosphere if successful, or nullptr on failure.
Remarks:
  NOTE: Memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshIcoSphere(const ON_Sphere& sphere, int subdivisions);

/*
Description:
  Creates a quad mesh sphere. A quad mesh sphere differs from a standard UV mesh
  sphere in that it's vertices are evenly distributed. A quad mesh sphere starts
  from an cube (a regular polyhedron with 6 faces). It is then refined by splitting
  each quad into 4 smaller quads. This splitting can be done several times.
Parameters:
  sphere       - [in] The input sphere provides the orienting plane and radius.
  subdivisions - [in] The number of times you want the faces split, where
                      0 <= subdivisions <= 8. Note, the total number of mesh
                      faces produces is: 6 * (4 ^ subdivisions)
Returns:
  A pointer to a welded quad mesh sphere if successful, or nullptr on failure.
Remarks:
  NOTE: Memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoMeshQuadSphere(const ON_Sphere& sphere, int subdivisions);

/*
Description:
  Adds creases to a smooth mesh by creating coincident vertices along selected edges.
Parameters:
  mesh - [in] The mesh to unweld edges on.
  edges - [in] An array of mesh topology edge indices.
  bModifyNormals - [in] If true, the vertex normals on each side of the edge take the same value 
                        as the face to which they belong, giving the mesh a hard edge look.
                        If false, each of the vertex normals on either side of the edge is assigned
                        the same value as the original normal that the pair is replacing, 
                        keeping a smooth look.
Returns:
  A pointer to the modified mesh if successful, or nullptr on failure.
Remarks:
  NOTE: Memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoUnweldMeshEdge(const ON_Mesh& mesh, const ON_SimpleArray<int>& edges, bool bModifyNormal);

RHINO_SDK_FUNCTION
ON_Mesh* RhinoWeldMeshEdges(const ON_Mesh& mesh, const ON_SimpleArray<int>& edges);


// Interface to preview and perform mesh extrusions
class RHINO_SDK_CLASS IRhinoMeshExtruder
{
protected:
  IRhinoMeshExtruder() {}
  virtual ~IRhinoMeshExtruder() {}
  IRhinoMeshExtruder(const IRhinoMeshExtruder&) = delete;
  IRhinoMeshExtruder& operator=(const IRhinoMeshExtruder&) = delete;

public:
  // Description:
  //   Returns a new instance of interface implementation.
  //   Caller must delete returned instance by calling DeleteThis().
  // Parameters:
  //   inputMesh         - [in] Mesh to use as starting point
  //   componentIndices  - [in] Mesh faces, edges and ngons to extrude
  static IRhinoMeshExtruder* NewMeshExtruder(const ON_Mesh& inputMesh, const ON_SimpleArray<ON_COMPONENT_INDEX>& componentIndices);

  // Description:
  //   Sets whether or not to perform extrude in UVN basis
  // Parameters:
  //   bEnable           - [in] True to perform extrude in UVN basis,
  //                            false to perform in world coordinate system
  virtual void SetUVN(bool bEnable) = 0;
  // Description:
  //   Gets whether or not to perform extrude in UVN basis
  virtual bool UVN(void) const = 0;

  // Description:
  //   Sets transform of extrusion.
  // Parameters:
  //   transform         - [in] Transform to use
  virtual void SetTransform(const ON_Xform& transform) = 0;
  // Description:
  //   Sets transform of extrusion by translation vector.
  // Parameters:
  //   translation       - [in] Translation vector
  virtual void SetTransform(const ON_3dVector& translation) = 0;
  // Description:
  //   Gets transform of extrusion.
  virtual ON_Xform Transform(void) const = 0;

  // Description:
  //   Sets whether to keep original faces. By default they will be deleted.
  // Parameters:
  //   bEnable           - [in] True to keep original faces and false to delete them
  virtual void SetKeepOriginalFaces(bool bEnable) = 0;
  // Description:
  //   True if original faces are deleted and false if not
  virtual bool KeepOriginalFaces(void) const = 0;

#pragma region RH_C_SHARED_ENUM [IRhinoMeshExtruder::ParameterMode] [Rhino.Geometry.MeshExtruderParameterMode] [int]
  /// <summary>
  /// Parameter mode for creating texture coordinates and surface parameters
  /// </summary>
  enum class ParameterMode : int
  {
    /// <summary>
    /// Stretch parameters slightly on extruded faces to cover walls
    /// </summary>
    CoverWalls = 0,
    /// <summary>
    /// Keep parameters on extruded faces and stretch on walls
    /// </summary>
    KeepAndStretch = 1
  };
#pragma endregion

  // Description:
  //   Sets parameter mode for texture coordinates
  // Parameters:
  //   mode              - [in] mode to use
  virtual void SetTextureCoordinateMode(ParameterMode mode) = 0;
  // Description:
  //   Gets parameter mode for texture coordinates
  virtual ParameterMode TextureCoordinateMode(void) const = 0;

  // Description:
  //   Sets parameter mode for surface parameters
  // Parameters:
  //   mode              - [in] mode to use
  virtual void SetSurfaceParameterMode(ParameterMode mode) = 0;
  // Description:
  //   Gets parameter mode for surface parameters
  virtual ParameterMode SurfaceParameterMode(void) const = 0;

#pragma region RH_C_SHARED_ENUM [IRhinoMeshExtruder::FaceDirectionMode] [Rhino.Geometry.MeshExtruderFaceDirectionMode] [int]
  /// <summary>
  /// Defines how mesh extruder decides orients faces
  /// </summary>
  enum class FaceDirectionMode : int
  {
    /// <summary>
    /// Maintain all face directions
    /// </summary>
    Keep = 0,
    /// <summary>
    /// Orient closed meshes front face out
    /// </summary>
    OrientClosedFrontOut = 1
  };
#pragma endregion

  // Description:
  //   Sets face direction mode
  // Parameters:
  //   mode              - [in] mode to use
  virtual void SetFaceMode(FaceDirectionMode mode) = 0;
  // Description:
  //   Gets face direction mode
  virtual FaceDirectionMode FaceMode(void) const = 0;

  // Description:
  //   Creates a set of lines to preview extruded mesh.
  // Parameters:
  //   linesOut               - [out] Lines to preview extruded mesh
  virtual void PreviewLines(ON_SimpleArray<ON_Line>& linesOut) = 0;

  // Description:
  //   Applies extrude
  //   Return true if any faces or edges were extruded.
  // Parameters:
  //   meshOut               - [out] This mesh will be the result of applying extrude
  //                                 to the input mesh passed to NewMeshExtruder.
  virtual bool ExtrudedMesh(ON_Mesh& meshOut) = 0;
  // Description:
  //   Applies extrude and gives list of components that should be selected in extruded mesh.
  //   Return true if any faces or edges were extruded.
  // Parameters:
  //   meshOut               - [out] This mesh will be the result of applying extrude
  //                                 to the input mesh passed to NewMeshExtruder.
  //   extrudedComponentsOut - [out] List of components that should be selected in extruded mesh.
  virtual bool ExtrudedMesh(ON_Mesh& meshOut, ON_SimpleArray<ON_COMPONENT_INDEX>& extrudedComponentsOut) = 0;

  // Description:
  //   Deletes this instance
  virtual void DeleteThis(void) = 0;

  // Description:
  //   Emergency virtual function
  virtual void* EVF(const wchar_t*, void*) { return nullptr; };
};

RHINO_SDK_FUNCTION int RhinoInsetMeshFace(
  double d,
  ON_Mesh* pMesh,
  ON_COMPONENT_INDEX::TYPE ci_type,
  int ci_index,
  bool bModify,
  ON_3dPointArray& basepoints,
  ON_3dPointArray& corners);

RHINO_SDK_FUNCTION int RhinoInsetSubDFace(
  double d,
  ON_SubD* pSubD,
  ON_COMPONENT_INDEX::TYPE ci_type,
  int ci_index,
  bool bModify,
  ON_3dPointArray& basepoints,
  ON_3dPointArray& corners);

class RhinoMeshEdgeSliderImpl;

// Class for sliding edges of a mesh
class RHINO_SDK_CLASS RhinoMeshEdgeSlider
{
public:
  // Description:
  //   Constructs an object that can slide mesh edges
  // Parameters:
  //   mesh              - [in] Mesh to use as starting point
  //   edgeIndices       - [in] Mesh edge indices to slide
  //   minEdgeLength     - [in] Minimum allowed edge length
  RhinoMeshEdgeSlider(const ON_Mesh& mesh, const ON_SimpleArray<int>& edgeIndices, double minEdgeLength);

  // Description:
  //   Constructs an object that can slide mesh edges
  // Parameters:
  //   mesh              - [in] Mesh to use as starting point
  //   edgeIndices       - [in] Mesh edge indices to slide
  //   topVertexIndices  - [in] Mesh topology vertex indices to slide
  //   minEdgeLength     - [in] Minimum allowed edge length
  RhinoMeshEdgeSlider(const ON_Mesh& mesh, const ON_SimpleArray<int>& edgeIndices, const ON_SimpleArray<int>& topVertexIndices, double minEdgeLength);

  virtual ~RhinoMeshEdgeSlider();

  // Description:
  //   Slides mesh edges in absolute units
  //   Returns true on success
  // Parameters:
  //   meshOut           - [in] Output mesh
  //   amount            - [in] Slide amount in absolute units
  //   smoothness        - [in] Smoothness if the path each vertex is slid along
  bool SlideAbsolute(ON_Mesh& meshOut, double amount, double smoothness);

  // Description:
  //   Slides mesh edges in proportional units. 
  //   Returns true on success
  // Parameters:
  //   meshOut           - [in] Output mesh
  //   amount            - [in] Slide amount in proportion of the crossing edge
  //   smoothness        - [in] Smoothness if the path each vertex is slid along
  bool SlideProportional(ON_Mesh& meshOut, double amount, double smoothness);

  // Description:
  //   Slides mesh edges. 
  //   Returns true on success
  // Parameters:
  //   meshOut           - [in] Output mesh
  //   bAcross           - [in] True to slide sideways across the selected edges
  //                            False to slide along the selected edges
  //   bAbsolute         - [in] True to use amount as absolute units
  //                            False to use amount as proportion of edge length
  //   amount            - [in] Slide amount in either absolute units or proportion of the edge length
  //                            depending on the value of bAbsolute
  //   smoothness        - [in] Smoothness if the path each vertex is slid along
  bool Slide(ON_Mesh& meshOut, bool bAcross, bool bAbsolute, double amount, double smoothness);

  // Description:
  //   Returns closest point on edges crossing the edges being slid. 
  // Parameters:
  //   pt                    - [in]  Point anywhere near the mesh
  //   bPositiveDirectionOut - [out] True if point is on an edge in positive slide direction
  //   pickEdgeLength        - [out] Length of the crossing edge that the closest point is on
  //   pickOffset            - [out] Offset from the loop
  ON_3dPoint ClosestPickPoint(const ON_3dPoint& pt, bool& bPositiveDirectionOut, double& pickEdgeLength, double& pickOffset) const;

  // Description:
  //   Returns closest point on edges in given direction from the edges being slid.
  // Parameters:
  //   pt                    - [in]  Point anywhere near the mesh
  //   bAcross               - [in]  True to find closest point on edges crossing the edges being slid
  //                                 False to find closest point on edges along the edges being slid
  //   bPositiveDirectionOut - [out] True if point is on an edge in positive slide direction
  //   pickEdgeLength        - [out] Length of the edge that the closest point is on
  //   pickOffset            - [out] Offset from the component being slid
  ON_3dPoint ClosestPickPoint(const ON_3dPoint& pt, bool bAcross, bool& bPositiveDirectionOut, double& pickEdgeLength, double& pickOffset) const;


  // Description:
  //   Emergency virtual function
  virtual void* EVF(const wchar_t*, void*) { return nullptr; };

private:
  RhinoMeshEdgeSliderImpl* m_pImpl;
  bool m_padding_bool[4];
  unsigned short int m_padding_short[4];
  unsigned long int m_padding_long[4];
};

class RhinoSubDEdgeSliderImpl;

// Class for sliding edges of a SubD
class RHINO_SDK_CLASS RhinoSubDEdgeSlider
{
public:
  // Description:
  //   Constructs an object that can slide SubD edges
  // Parameters:
  //   subD              - [in] SubD to use as starting point
  //   edgeIds           - [in] SubD edge ids to slide
  //   minEdgeLength     - [in] Minimum allowed edge length
  RhinoSubDEdgeSlider(const ON_SubD& subD, const ON_SimpleArray<int>& edgeIds, double minEdgeLength);

  // Description:
  //   Constructs an object that can slide SubD edges and vertices
  // Parameters:
  //   subD              - [in] SubD to use as starting point
  //   edgeIds           - [in] SubD edge ids to slide
  //   vertexIds         - [in] SubD vertex ids to slide
  //   minEdgeLength     - [in] Minimum allowed edge length
  RhinoSubDEdgeSlider(const ON_SubD& subD, const ON_SimpleArray<int>& edgeIds, const ON_SimpleArray<int>& vertexIds, double minEdgeLength);

  virtual ~RhinoSubDEdgeSlider();

  // Description:
  //   Slides SubD edges in absolute units
  //   Returns true on success
  // Parameters:
  //   subDOut           - [in] Output SubD
  //   amount            - [in] Slide amount in absolute units
  //   smoothness        - [in] Smoothness of the path each vertex is slid along
  bool SlideAbsolute(ON_SubD& subDOut, double amount, double smoothness);

  // Description:
  //   Slides SubD edges in proportional units. 
  //   Returns true on success
  // Parameters:
  //   subDOut           - [in] Output SubD
  //   amount            - [in] Slide amount in proportion of the crossing edge
  //   smoothness        - [in] Smoothness of the path each vertex is slid along
  bool SlideProportional(ON_SubD& subDOut, double amount, double smoothness);


  // Description:
  //   Slides SubD edges. 
  //   Returns true on success
  // Parameters:
  //   subDOut           - [in] Output SubD
  //   bAcross           - [in] True to slide sideways across the selected edges
  //                            False to slide along the selected edges
  //   bAbsolute         - [in] True to use amount as absolute units
  //                            False to use amount as proportion of edge length
  //   amount            - [in] Slide amount in either absolute units or proportion of the edge length
  //                            depending on the value of bAbsolute
  //   smoothness        - [in] Smoothness of the path each vertex is slid along
  bool Slide(ON_SubD& subDOut, bool bAcross, bool bAbsolute, double amount, double smoothness);

  // Description:
  //   Returns closest point on edges crossing the edges being slid. 
  // Parameters:
  //   pt                    - [in]  Point anywhere near the SubD
  //   bPositiveDirectionOut - [out] True if point is on an edge in positive slide direction
  //   pickEdgeLength        - [out] Length of the crossing edge that the closest point is on
  //   pickOffset            - [out] Offset from the loop
  ON_3dPoint ClosestPickPoint(const ON_3dPoint& pt, bool& bPositiveDirectionOut, double& pickEdgeLength, double& pickOffset) const;

  // Description:
  //   Returns closest point on edges in given direction from the edges being slid.
  // Parameters:
  //   pt                    - [in]  Point anywhere near the mesh
  //   bAcross               - [in]  True to find closest point on edges crossing the edges being slid
  //                                 False to find closest point on edges along the edges being slid
  //   bPositiveDirectionOut - [out] True if point is on an edge in positive slide direction
  //   pickEdgeLength        - [out] Length of the edge that the closest point is on
  //   pickOffset            - [out] Offset from the component being slid
  ON_3dPoint ClosestPickPoint(const ON_3dPoint& pt, bool bAcross, bool& bPositiveDirectionOut, double& pickEdgeLength, double& pickOffset) const;

  // Description:
  //   Emergency virtual function
  virtual void* EVF(const wchar_t*, void*) { return nullptr; };

private:
  RhinoSubDEdgeSliderImpl* m_pImpl;
  bool m_padding_bool[4];
  unsigned short int m_padding_short[4];
  unsigned long int m_padding_long[4];
};

/*
Description:
  Creates a single mesh face from the given input.
Parameters:
  mesh    - [in] Vertices and edges from this mesh are used to generate the new mesh face
  indexes - [in] An array of ON_COMPONENT_INDEXs.  Can be 1 of the following combinations.
                 1 vertex (mesh_vertex or meshtop_vertex) and 1 edge
                 2 edges
                 3 vertices
  bJoin   - [in] If bJoin is true then the return, on success, will be the input mesh with the 
                 new mesh face added.  If bJoin is false the return, on success, will only be
                 the new mesh face.
Returns:
  A pointer to the new mesh if successful, or nullptr on failure.
Remarks:
  NOTE: Memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoPatchSingleMeshFace(const ON_Mesh* pInputMesh, const ON_SimpleArray<ON_COMPONENT_INDEX>& indexes, bool bJoin);


class RhinoMeshUnwrapperImpl;

// Class for unwrapping meshes
// Unwraps and packs the meshes and stores the result to m_T array.
class RHINO_SDK_CLASS RhinoMeshUnwrapper
{
public:
  /// <summary>
  /// Creates an unwrapper object for a single mesh.
  /// </summary>
  /// <param name="mesh">Mesh to unwrap</param>
  RhinoMeshUnwrapper(ON_Mesh& mesh);
  /// <summary>
  /// Creates an unwrapper object for an array of meshes
  /// </summary>
  /// <param name="meshes">Meshes to be uwrapped</param>
  RhinoMeshUnwrapper(const ON_SimpleArray<ON_Mesh*>& meshes);

  virtual ~RhinoMeshUnwrapper();

  /// <summary>
  /// Sets symmetry tip plane. Unwrapper tries to keep unwrap result symmetric to that plane.
  /// </summary>
  /// <param name="symmetryTip">Symmetry tip plane</param>
  void SetSymmetryTip(const ON_Plane& symmetryTip);

#pragma region RH_C_SHARED_ENUM [RhinoMeshUnwrapper::UnwrapMethod] [Rhino.Geometry.MeshUnwrapMethod] [int]
  /// <summary>
  /// Unwrapping method used
  /// </summary>
  enum class UnwrapMethod : int
  {
    /// <summary>
    /// Least squares conformal map, Rhino 7 standard method
    /// </summary>
    LSCM = 0,
    /// <summary>
    /// ABF++, angle based flattening
    /// </summary>
    ABFPP = 1,
    /// <summary>
    /// ARAP, as rigid as possible
    /// </summary>
    ARAP = 2,
  };
#pragma endregion

  /// <summary>
  /// Unwraps the meshes and stores the results to the m_T arrays.
  /// </summary>
  /// <param name="method">Unwrap method to be used</param>
  /// <returns>True on success</returns>
  bool Unwrap(UnwrapMethod method);

  /// <summary>
  /// Emergency virtual function
  /// </summary>
  /// <param name=""></param>
  /// <param name=""></param>
  /// <returns></returns>
  virtual void* EVF(const wchar_t*, void*) { return nullptr; };

private:
  RhinoMeshUnwrapperImpl* m_pImpl;
  bool m_padding_bool[4];
  unsigned short int m_padding_short[4];
  unsigned long int m_padding_long[4];
};

/// <summary>
/// Subdivides the faces of a mesh.
/// </summary>
RHINO_SDK_FUNCTION
bool RhinoMeshSubdivide(
  const ON_Mesh& inMesh,
  ON_Mesh& outMesh
);

/// <summary>
/// Subdivides specific faces of a mesh.
/// </summary>
RHINO_SDK_FUNCTION
bool RhinoMeshSubdivideFaces(
  const ON_Mesh& inMesh,
  const ON_SimpleArray<ON_COMPONENT_INDEX> face_list,
  ON_Mesh& outMesh
);
