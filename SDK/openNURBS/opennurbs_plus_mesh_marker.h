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

#if !defined(OPENNURBS_PLUS_MESH_MARKER_INC_)
#define OPENNURBS_PLUS_MESH_MARKER_INC_


class ON_CLASS ON_MeshMarker
{
public:
  ON_MeshMarker();
  ~ON_MeshMarker();

  // The copy constructor and operator=() use onmemdup to set 
  // the m_vertex_mark_value[] and m_face_mark_value[] arrays
  // and copy the values of all other fields.
  ON_MeshMarker(const ON_MeshMarker&);
  ON_MeshMarker& operator=(const ON_MeshMarker&);

  /*
  Description:
    Deallocates m_vertex_mark_value[] and m_face_mark_value[]
    arrays and zeros all fields.
  */
  void Destroy();


  /*
  Description:
    Specify the mesh to be marked.
  Parameters:
    mesh - [in]
    V_count - [in]
      number of vertices.
    V - [in]
      array of vertex points. This information is used only when
      MarkComponents() is called with bUseTopologicalComponent=true.
      If you will be passing bUseTopologicalComponent=false, then
      you can pass a nullptr V parameter.
    F_count - [in] (>=1)
      number of faces.
    F - [in]
      Face list.
      Triangular faces have F[i][2]==F[i][3] or F[i].vi[2]==F[i].vi[3].
  Returns:
    True if the mesh is valid.
  Remarks:
    This is the first step in calculating mesh component information.
    There are three versions of this function.  You only need to call
    one of them one time before proceeding.
  */
  bool SetMeshInfo(const ON_Mesh* mesh);
  bool SetMeshInfo( int V_count, const ON_3fPoint* V,
                   int F_count, const ON_MeshFace* F
                   );
  bool SetMeshInfo( int V_count, const float (*V)[3],
                   int F_count, const int (*F)[4]
                   );

  /*
  Description:
    Initially mark faces.
  Parameters:
    mesh_face_index - [in]
      index of face to mark
    mark_value - [in]
       0:  unmarked
      >0:  marked and mark will transfer to unmarked neighbors
      <0:  treat face as deleted when marking components.
  */
  bool MarkFace( int mesh_face_index, int mark_value );

  /*
  Description:
    Initially mark vertex.
  Parameters:
    mesh_vertex_index - [in]
      index of vertex to mark
    mark_value - [in]
       0:  unmarked
      >0:  marked and mark will transfer to unmarked neighbors
      <0:  treat vertex as deleted when marking components.
  */
  bool MarkVertex( int mesh_vertex_index, int mark_value );


  /*
  Description:
    Transfer initial marks to connected components.
  Parameters:
    bUseTopologicalComponent - [in]
      If true, then vertices that are at the same location
      are treated as one vertex.  Setting this value to
      true requires more time and memory to be used.  The
      additional time expense is quick sort of m_V_count
      items.  The additional memory expense is two integer
      heap arrays of length m_V_count.
      If false, then the vertex point information is ignored
      and each vertex is considered to be distinct.
    bProcessUnmarkedVertices - [in]
      A mesh may have isolated vertices (not used by a face) and
      some of these may not be marked by calls to MarkVertex().
      When bProcessUnmarkedVertices=true, if at the end of the
      component marking calculation there are unmarked vertices,
      then a new mark value is generated and assigned to each
      unmarked vertex.  If bProcessUnmarkedVertices=false, then
      vertices that are not marked by the calculation are left
      unmarked.  
    bProcessUnmarkedFaces - [in]
      Generally a mesh has several components and some may have 
      no marks in them.  When bProcessUnmarkedFaces=true, a
      new mark value is generated for one face in each unmarked
      component and then the entire component the face belongs to
      is marked with that value. When bProcessUnmarkedFaces=false,
      if at the end of the marking calculation there are unmarked
      components, then those components are left unmarked.  
      If you set bProcessUnmarkedFaces=false, then you must use
      MarkFace() or MarkVertex() to set a see mark in at least 
      one component before calling MarkComponents().
  Returns:
    True if mesh is successfully marked.  The mark information is
    is returned in the members

        m_vertex_mark_value[]
        m_face_mark_value[]
        m_marked_vertex_count
        m_marked_face_count
        m_deleted_vertex_count
        m_deleted_face_count
        m_max_mark_value

    in the m_face_mark_value[] and m_vertex_mark_value[] arrays.
    These arrays have length m_F_count and m_V_count, respectively,
    and are parallel to the m_F[] and m_V[] arrays.
    m_marked_vertex_count is the number of non-zero (positive or
    negative) elements m_vertex_mark_value[].
    m_marked_face_count is the number of non-zero (positive or
    negative) elements m_face_mark_value[].
    m_deleted_face_count is the number of negative
    elements in m_face_mark_value[].
    m_deleted_vertex_count is the number of negative
    elements in m_vertex_mark_value[].
    The maximum mark value used is in m_max_mark_value.

    MarkComponents() increments m_marking_error_count every time it 
    encounters an error condition that it can tolerate.  
    If MarkComponents() returns true and m_marking_error_count is
    greater than 0, then exercise caution using the results.
  */
  bool MarkComponents(
        bool bUseTopologicalComponent, 
        bool bProcessUnmarkedVertices,
        bool bProcessUnmarkedFaces 
        );

  /*
  Description:
    Validate results of the MeshComponents calculation.
  Parameters:
    bAllowUnmarkedVertices - [in]
    bAllowUnmarkedFaces - [in]
    vertex_index -[out]
    face_index - [out]
      If an error is found and the indices returned in
      vertex_index and face_index indicate where the error
      was found.

       vi   fi
       -1  >=0   m_face_mark_value[fi] = 0 and should be > 0
      >=0   -1   m_vertex_mark_value[vi] = 0 and should be > 0
      >=0  >=0   vertex vi is on face fi and the vertex and face
                 have different mark values.
  Returns:
    True if mark values are valid.  False if an error is found.
  */
  bool ValidateMarks(
        bool bAllowUnmarkedVertices,
        bool bAllowUnmarkedFaces,
        int* vertex_index, 
        int* face_index
        ) const;

  // mesh being marked
  int m_V_count; // length of m_vertex_info[] array
  int m_F_count; // length of m_face_info[] array
  const float (*m_V)[3]; // mesh vertices - used only when 
                         // bUseTopologicalComponent = true is passed
                         // to MarkComponents.
  const int (*m_F)[4]; // mesh vertices

  // The m_vertex_mark_value[] and m_face_mark_value[] arrays are 
  // allocated by onmalloc() when SetMeshInfo() is called
  // and are deallocated by onfree() in ~ON_MeshMarker().
  // If you want to keep these arrays and deallocate them yourself
  // using onfree(), then set the pointers to zero before 
  // ~ON_MeshMarker() is called.
  //
  // mark values 
  //
  //   0:  unmarked,
  // > 0:  vertex/face mark value
  // < 0:  treat vertex/face as deleted
  //
  int* m_vertex_mark_value; // array of m_V_count ints parallel to m_V[]
  int* m_face_mark_value;   // array of m_F_count ints parallel to m_F[]

  int m_marked_vertex_count;   // number of m_vertex_mark_value[] entries with nonzero m_mark_value
  int m_marked_face_count;     // number of m_face_mark_value[] entries with nonzero value
  int m_deleted_vertex_count;  // number of m_vertex_mark_value[] entries with negative m_mark_value
  int m_deleted_face_count;    // number of m_face_mark_value[] entries with negative m_mark_value
  int m_max_mark_value; // largest vertex mark value

  int m_marking_error_count;  // number of errors encountered in MarkComponents() call

private:
  int m_reserved1;
  int m_reserved2;
};

#endif
