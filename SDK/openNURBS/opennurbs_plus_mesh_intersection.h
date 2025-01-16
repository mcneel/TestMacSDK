#pragma once
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

#if !defined(ON_PLUS_MESH_INTERSECTION_INC_)
#define ON_PLUS_MESH_INTERSECTION_INC_

class ON_XMeshPlaneEquation;
class ON_XMeshComponent;
class ON_XMeshVertex;
class ON_XMeshEdge;
class ON_XMeshFace;
class ON_XMesh;

class ON_XMeshEvent;
class ON_XMeshPolyline;
class ON_XMeshFF;
class ON_XMeshFaceXData;
class ON_XMeshIntersector;

class ON_XMeshVertexIterator;
class ON_XMeshEdgeIterator;
class ON_XMeshFaceIterator;

typedef ON_wString ON_XMeshEventComponentString[2];


#if defined(ON_DLL_TEMPLATE)
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4231 )
//ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_XMeshEventComponentString>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class ON_XMeshFF*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class ON_XMeshPolyline*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class ON_XMeshEdge*>;
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<class ON_XMeshFace*>;
#pragma ON_PRAGMA_WARNING_POP
#endif

enum class eXMeshPoolStatus : unsigned int {
  unknown = 0,
  active = 1,
  returned = 2
};

class ON_CLASS ON_XMeshFFPool

{
public:
  ON_XMeshFFPool(bool bMultiThread=false);
  ~ON_XMeshFFPool();
  ON_XMeshFF* NewFF(ON_XMeshFace* pF0, ON_XMeshFace* pF1);
  bool ReturnFF(ON_XMeshFF*& pFF);//pFF comes back 0.  False if status is not active
  void ReturnAll();
  size_t ActiveCount() const;
private:
  friend class ON_XMeshFFIterator;

  ON_XMeshFF* NewFF_MP(ON_XMeshFace* pF0, ON_XMeshFace* pF1);
  bool ReturnFF_MP(ON_XMeshFF*& pFF);//pFF comes back 0.  False if status is not active

  ON_FixedSizePool m_Pool;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshEventPool

{
public:
  ON_XMeshEventPool(bool bMultiThread=false);
  ~ON_XMeshEventPool();
  ON_XMeshEvent* NewEvent();
  bool ReturnEvent(ON_XMeshEvent*& pX);//pX comes back 0. False if status is not active
  void ReturnAll();
  size_t ActiveCount() const;
private:
  friend class ON_XMeshEventIterator;

  ON_XMeshEvent* NewEvent_MP();
  bool ReturnEvent_MP(ON_XMeshEvent*& pFF);//pFF comes back 0.  False if status is not active

  ON_FixedSizePool m_Pool;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshVertexPool

{
public:
  ON_XMeshVertexPool(bool bMultiThread=false);
  ~ON_XMeshVertexPool();
  ON_XMeshVertex* NewVertex(const ON_3dPoint& P, const ON_XMesh* pM);
  bool ReturnVertex(ON_XMeshVertex*& pV);//pV comes back 0. False if status is not active
  void ReturnAll();
  size_t ActiveCount() const;
private:
  friend class ON_XMeshVertexIterator;
  ON_XMeshVertex* NewVertex_MP(const ON_3dPoint& P, const ON_XMesh* pM);
  bool ReturnVertex_MP(ON_XMeshVertex*& pV);//pV comes back 0. False if status is not active
  ON_FixedSizePool m_Pool;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshEdgePool

{
public:
  ON_XMeshEdgePool(bool bMultiThread=false);
  ~ON_XMeshEdgePool();
  ON_XMeshEdge* NewEdge(ON_XMeshVertex* pV0, 
                        ON_XMeshVertex* pV1, 
                        const ON_XMesh* pM);
  bool ReturnEdge(ON_XMeshEdge*& pE);//pE comes back 0. False if status is not active
  void ReturnAll();
  size_t ActiveCount() const;
private:
  friend class ON_XMeshEdgeIterator;
  ON_XMeshEdge* NewEdge_MP(ON_XMeshVertex* pV0, 
                        ON_XMeshVertex* pV1, 
                        const ON_XMesh* pM);
  bool ReturnEdge_MP(ON_XMeshEdge*& pE);//pE comes back 0. False if status is not active
  ON_FixedSizePool m_Pool;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshFacePool

{
public:
  ON_XMeshFacePool(bool bMultiThread=false);
  ~ON_XMeshFacePool();
  ON_XMeshFace* NewFace(ON_XMeshVertex* pV0,
                        ON_XMeshVertex* pV1, 
                        ON_XMeshVertex* pV2,
                        ON_XMeshEdge* pE0, bool bRev0,
                        ON_XMeshEdge* pE1, bool bRev1,
                        ON_XMeshEdge* pE2, bool bRev2,
                        const ON_XMesh* pM);
  bool ReturnFace(ON_XMeshFace*& pF);//pF comes back 0. False if status is not active
  void ReturnAll();
  size_t ActiveCount() const;
private:
  friend class ON_XMeshFaceIterator;
  ON_XMeshFace* NewFace_MP(ON_XMeshVertex* pV0,
                        ON_XMeshVertex* pV1, 
                        ON_XMeshVertex* pV2,
                        ON_XMeshEdge* pE0, bool bRev0,
                        ON_XMeshEdge* pE1, bool bRev1,
                        ON_XMeshEdge* pE2, bool bRev2,
                        const ON_XMesh* pM);
  bool ReturnFace_MP(ON_XMeshFace*& pF);//pF comes back 0. False if status is not active
  ON_FixedSizePool m_Pool;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshFFIterator

{
public:
  ON_XMeshFFIterator(bool bMultiThread=false);
  ON_XMeshFFIterator(const ON_XMeshFFPool& Pool, bool bMultiThread=false);
  ON_XMeshFFIterator(const ON_XMeshFFIterator& src);
  ~ON_XMeshFFIterator();
  ON_XMeshFFIterator& operator=(const ON_XMeshFFIterator& src);
  void Create(const ON_XMeshFFPool& Pool, bool bMultiThread=false);
  void Reset();//To be used on the same pool with same mesh
  int Count() const;//Active count of the pool.  
  ON_XMeshFF* Current();
  ON_XMeshFF* First();
  ON_XMeshFF* Next();

private:
  ON_XMeshFF* Next_MP();
  bool IsActive(const ON_XMeshFF*) const;
  const ON_XMeshFFPool* m_pPool;
  ON_FixedSizePoolIterator m_It;//Must have m_pPool as its pool
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshEventIterator

{
public:
  ON_XMeshEventIterator(bool bMultiThread=false);
  ON_XMeshEventIterator(const ON_XMeshEventPool& Pool, bool bMultiThread=false);
  ON_XMeshEventIterator(const ON_XMeshEventIterator& src);
  ~ON_XMeshEventIterator();
  ON_XMeshEventIterator& operator=(const ON_XMeshEventIterator& src);
  void Create(const ON_XMeshEventPool& Pool, bool bMultiThread=false);
  void Reset();//To be used on the same pool with same mesh
  int Count() const;//Active count of the pool.  
  ON_XMeshEvent* Current();
  ON_XMeshEvent* First();
  ON_XMeshEvent* Next();

private:
  ON_XMeshEvent* Next_MP();
  bool IsActive(const ON_XMeshEvent*) const;
  const ON_XMeshEventPool* m_pPool;
  ON_FixedSizePoolIterator m_It;//Must have m_pPool as its pool
  ON_SleepLock m_lock;
  bool m_bMP;
};

/* 
  In the three mesh component iterators, if the mesh is non-NULL, 
  then only elements from that mesh will be considered.  
  */

class ON_CLASS ON_XMeshVertexIterator

{
public:
  ON_XMeshVertexIterator(bool bMultiThread=false);
  ON_XMeshVertexIterator(const ON_XMeshVertexPool& Pool, bool bCheckX, ON_XMesh* pM, bool bMultiThread=false);
  ON_XMeshVertexIterator(const ON_XMeshVertexIterator& src);
  ~ON_XMeshVertexIterator();
  ON_XMeshVertexIterator& operator=(const ON_XMeshVertexIterator& src);
  void Create(const ON_XMeshVertexPool& Pool, bool bCheckX, ON_XMesh* pM, bool bMultiThread=false);
  void Reset();//To be used on the same pool with same mesh
  int Count() const;//Active count of the pool.  If m_pM == 0 or m_bCheckX is false, this may be more
  //than the number of vertices from this mesh that are active, but 
  //it does give a bound on the iteration.
  ON_XMeshVertex* Current();
  ON_XMeshVertex* First();
  ON_XMeshVertex* Next();

private:
  ON_XMeshVertex* Next_MP();
  //Only use this if Return has not been used on the pool. It is specifically
  //for use while creating an ON_XMesh from an ON_MeshTopology.
  friend class ON_XMesh;
  ON_XMeshVertex* Vertex(int id);

  bool IsActive(const ON_XMeshVertex*) const;
  const ON_XMeshVertexPool* m_pPool;
  ON_FixedSizePoolIterator m_It;//Must have m_pPool as its pool
  bool m_bCheckX;//If true, only give results that have intersection data.
  ON_XMesh* m_pM;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshEdgeIterator

{
public:
  ON_XMeshEdgeIterator(bool bMultiThread=false);
  ON_XMeshEdgeIterator(const ON_XMeshEdgePool& Pool, bool bCheckX, ON_XMesh* pM, bool bMultiThread=false);
  ON_XMeshEdgeIterator(const ON_XMeshEdgeIterator& src);
  ~ON_XMeshEdgeIterator();
  ON_XMeshEdgeIterator& operator=(const ON_XMeshEdgeIterator& src);
  void Create(const ON_XMeshEdgePool& Pool, bool bCheckX, ON_XMesh* pM, bool bMultiThread=false);
  void Reset();//To be used on the same pool with same mesh
  int Count() const;//Active count of the pool.  If m_pM == 0 or m_bCheckX is false, this may be more
  //than the number of edges from this mesh that are active, but 
  //it does give a bound on the iteration.
  ON_XMeshEdge* Current();
  ON_XMeshEdge* First();
  ON_XMeshEdge* Next();
private:
  ON_XMeshEdge* Next_MP();
  bool IsActive(const ON_XMeshEdge*) const;
  const ON_XMeshEdgePool* m_pPool;
  ON_FixedSizePoolIterator m_It;//Must have m_pPool as its pool
  bool m_bCheckX;//If true, only give results that have intersection data.
  ON_XMesh* m_pM;
  ON_SleepLock m_lock;
  bool m_bMP;
};

class ON_CLASS ON_XMeshFaceIterator

{
public:
  ON_XMeshFaceIterator(bool bMultiThread=false);
  ON_XMeshFaceIterator(const ON_XMeshFacePool& Pool, bool bCheckX, ON_XMesh* pM, bool bMultiThread=false);
  ON_XMeshFaceIterator(const ON_XMeshFaceIterator& src);
  ~ON_XMeshFaceIterator();
  ON_XMeshFaceIterator& operator=(const ON_XMeshFaceIterator& src);
  void Create(const ON_XMeshFacePool& Pool, bool bCheckX, ON_XMesh* pM, bool bMultiThread=false);
  void Reset();//To be used on the same pool with same mesh
  int Count() const;//Active count of the pool.  If m_pM == 0 or m_bCheckX is false, this may be more
  //than the number of faces from this mesh that are active, but 
  //it does give a bound on the iteration.
  ON_XMeshFace* Current();
  ON_XMeshFace* First();
  ON_XMeshFace* Next();
private:
  ON_XMeshFace* Next_MP();
  bool IsActive(const ON_XMeshFace*) const;
  const ON_XMeshFacePool* m_pPool;
  ON_FixedSizePoolIterator m_It;//Must have m_pPool as its pool
  bool m_bCheckX;//If true, only give results that have intersection data.
  ON_XMesh* m_pM;
  ON_SleepLock m_lock;
  bool m_bMP;
};

//NOTE: This class should not manage the memory of the ON_XMeshEvents
//If closed, then first and last are the same.
class ON_CLASS ON_XMeshPolyline : public ON_SimpleArray<ON_XMeshEvent*>

{
public:
  //The default constructors, destructor, etc, for the ON_SimpleArray are
  //correct for this class.
  static void Merge(
    const ON_XMeshPolyline& lhs,
    const ON_XMeshPolyline& rhs,
    ON_XMeshPolyline& dest);
  bool IsClosed();
};

class ON_CLASS ON_XMeshEvent

{
public:
  ON_XMeshEvent();
  ~ON_XMeshEvent();
  ON_XMeshVertex* Vertex(int side);
  ON_XMeshEdge* Edge(int side, double& t);
  ON_XMeshEdge* Edge(int side);
  ON_XMeshFace* Face(int side, ON_3dPoint& abc);
  ON_XMeshFace* Face(int side);
  const ON_XMeshVertex* Vertex(int side) const;
  const ON_XMeshEdge* Edge(int side, double& t) const;
  const ON_XMeshEdge* Edge(int side) const;
  const ON_XMeshFace* Face(int side, ON_3dPoint& abc) const;
  const ON_XMeshFace* Face(int side) const;

  ON_3dPoint Point(int side) const;//Evaluate the component

  //Get all faces or edges that involve component[side]. Side value not checked
  void AllVertices(int side, ON_SimpleArray<ON_XMeshVertex*>& Vertices);
  void AllEdges(int side, ON_SimpleArray<ON_XMeshEdge*>& Edges);
  void AllFaces(int side, ON_SimpleArray<ON_XMeshFace*>& Faces);
  void AllComponents(int side, ON_SimpleArray<ON_XMeshComponent*>& Components);
  void AllVertices(int side, ON_SimpleArray<const ON_XMeshVertex*>& Vertices) const;
  void AllEdges(int side, ON_SimpleArray<const ON_XMeshEdge*>& Edges) const;
  void AllFaces(int side, ON_SimpleArray<const ON_XMeshFace*>& Faces) const;
  void AllComponents(int side, ON_SimpleArray<const ON_XMeshComponent*>& Components) const;

  void Add(ON_XMeshVertex* pV, int side);
  void Add(ON_XMeshEdge* pE, double t, int side);
  void Add(ON_XMeshFace* pF, const ON_3dPoint& abc, int side);//3 doubles.

  //true if m_component[side] = pC
  bool ComponentMatches(const ON_XMeshComponent* pC, int side) const;

  //true if m_component[0]=pA and m_component[1] = pB;
  bool ComponentsMatch(const ON_XMeshComponent* pA, const ON_XMeshComponent* pB) const;

  //Find all the faces that include the component on side.
  bool GetFaces(int side, ON_SimpleArray<ON_XMeshFace*>& Faces);
  bool UsesFace(const ON_XMeshFace* pF, int side) const;
  bool UsesEdge(const ON_XMeshEdge* pE, int side) const;

  void Ignore(); //Sets m_bIgnore to true.
  bool IsActive() const;//Just checks m_bIgnore.
  bool AddToComponent(int side);//Adds this to it's component's event list.
  void RemoveFromComponent(int side);//Removes this from its component's event list.
  void CreateComponentString(ON_XMeshEventComponentString& S) const;//For debug purposes.
private:
  ON_XMeshEvent(const ON_XMeshEvent& X);//Not implemented
  ON_XMeshEvent& operator=(const ON_XMeshEvent& src);//Not implemented

  friend class ON_XMeshEventPool;
  friend class ON_XMeshEventIterator;
  friend class ON_XMeshIntersector;

  void Destroy();

  double m_param[2][3];//If component[i] is 
                                //vertex, the, m_param[i] is not used.  All 0.
                                //edge, then m_pam[i][0] is the edge parameter,
                                //face. then barycentric coords.
  ON_XMeshComponent* m_component[2];//Face, Vertex, or Edge
  bool m_bIgnore;//Will be true if it has been decided that this event is inactive.
  //Typically because either it is not within the active region or because 
  //it was combined with another event.
  eXMeshPoolStatus m_status;
};

class ON_CLASS ON_XMeshEventList : public ON_SimpleArray<ON_XMeshEvent*>

{
public:
  void RemoveEvent(const ON_XMeshEvent* pX);
  bool HasEvent(const ON_XMeshEvent* pX) const;
  bool HasComponentEvent(const ON_XMeshComponent* pC, int side) const;
  ON_XMeshEvent* Match(ON_XMeshComponent* component, int side);
  const ON_XMeshEvent* Match(const ON_XMeshComponent* component, int side) const;
  ON_XMeshEvent* Match(ON_XMeshComponent* comp0, ON_XMeshComponent* comp1);
  const ON_XMeshEvent* Match(const ON_XMeshComponent* comp0, const ON_XMeshComponent* comp1) const;
};

enum class eXMeshOverlapType : unsigned int {
  unknown = 0,
  not_overlap = 1,
  same_normal = 2,
  opposite_normal = 3,
  overlap = 4
};

class ON_CLASS ON_XMeshFF

{
public:
  ON_XMeshFF(ON_XMeshFace* pF0, ON_XMeshFace* pF1);
  ~ON_XMeshFF();
  int EventCount() const;
  ON_XMeshEvent* Event(int id);//Gives m_X[id].  No range checking.
  void AddEvent(ON_XMeshEvent* pX);
  void RemoveEvent(const ON_XMeshEvent* pX);
  bool HasEvent(const ON_XMeshEvent* pX) const;
  bool HasComponentEvent(const ON_XMeshComponent* pC, int side) const;
  bool FacesMatch(ON_XMeshFace* pF0, const ON_XMeshFace* pF1) const;  
private:
  ON_XMeshFF(const ON_XMeshFF& src);//Not implemented
  ON_XMeshFF& operator=(const ON_XMeshFF& src);//Not implemented
  ON_XMeshFF();//Not implemented

  friend class ON_XMeshFFPool;
  friend class ON_XMeshFFIterator;
  friend class ON_XMeshFace;
  friend class ON_XMeshIntersector;

  void Destroy();

  //Only check  Vi on Fj if Fi is Vi's first face.
  //Coords of V on F are 0 if F is degenerate
  bool CheckVerticesOnPlanes(ON_XMeshEventPool& XPool);

  //Only intersect  Ei with Fj if Fi is Ei's first face.
  //Coords of E on F are 0 if F is degenerate. 
  //If intersection is at a vertex, nothing is added.
  bool IntersectEdgesWithPlanes(ON_XMeshEventPool& XPool);
  bool IntersectEdgeWithFacePlane(int face_side, ON_XMeshEdge* pE, ON_XMeshEventPool& XPool);


  ON_XMeshFace* m_pF[2];
  ON_XMeshEventList m_X;
  eXMeshOverlapType m_bOverlap;
  eXMeshPoolStatus m_status;
};

class ON_CLASS ON_XMeshFaceXData

{
public:
  ON_XMeshFaceXData();
  ~ON_XMeshFaceXData();

  void AddFF(ON_XMeshFF* pFF);
  ON_XMeshFF* FindFF(ON_XMeshFace* pF0, ON_XMeshFace* pF1);
private:
  ON_XMeshFaceXData(const ON_XMeshFaceXData& src);//Not implemented
  ON_XMeshFaceXData& operator=(const ON_XMeshFaceXData& src);//Not implemented
  
  friend class ON_XMeshFace;

  ON_SimpleArray<ON_XMeshFF*> m_FF;//These are shared and are managed by an ON_XMeshIntersector.
  ON_SimpleArray<ON_XMeshPolyline*> m_plines;
};

class ON_CLASS ON_XMeshPlaneEquation

{
private://This should only be accessible from a face.
  ON_XMeshPlaneEquation();
  ~ON_XMeshPlaneEquation();
  bool Create(const ON_3dPoint& A, const ON_3dPoint& B, const ON_3dPoint& C);
  bool Create(const ON_PlaneEquation& pe, double fuzz);
  int ClassifyPoint(const ON_3dPoint& P, double& dist) const;//-1 below, 1 above, 0 on.
  double Fuzz() const;
  bool IsValid() const;
  const ON_PlaneEquation& PlaneEquation() const;

  ON_XMeshPlaneEquation(const ON_XMeshPlaneEquation& src);//Not implemented
  ON_XMeshPlaneEquation& operator=(const ON_XMeshPlaneEquation& src);//Not implemented

  friend class ON_XMeshFace;
  ON_PlaneEquation m_eq;
  double m_fuzz;
  bool m_bIsValid;
};

class ON_CLASS ON_XMeshComponent

{
public:
  virtual ~ON_XMeshComponent();

  virtual bool IsValid() const;

  //Look through the event list
  int EventCount() const;
  ON_XMeshEvent* Event(int xid);
  const ON_XMeshEvent* Event(int xid) const;
  bool AddEvent(ON_XMeshEvent* pX);
  void RemoveEvent(ON_XMeshEvent* pX);
  ON_XMeshEvent* FindEvent(ON_XMeshComponent* pC);
  const ON_XMeshEvent* FindEvent(const ON_XMeshComponent* pC) const;
  bool HasEvent(const ON_XMeshComponent* pC) const;//Does m_X have a member with component?

  //Cast
  ON_XMeshVertex* Vertex();
  const ON_XMeshVertex* Vertex() const;
  ON_XMeshEdge* Edge();
  const ON_XMeshEdge* Edge() const;
  ON_XMeshFace* Face();
  const ON_XMeshFace* Face() const;

  //What is this thing?
  bool IsVertex() const;
  bool IsEdge() const;
  bool IsFace() const;

  virtual void CreateComponentString(ON_wString& S) const;

  //Whatever it is, what are the components that involve it?
  virtual void Vertices(ON_SimpleArray<ON_XMeshVertex*>& Vertices)=0;
  virtual void Vertices(ON_SimpleArray<const ON_XMeshVertex*>& Vertices) const=0;
  virtual void Edges(ON_SimpleArray<ON_XMeshEdge*>& Edges)=0;
  virtual void Edges(ON_SimpleArray<const ON_XMeshEdge*>& Edges) const=0;
  virtual void Faces(ON_SimpleArray<ON_XMeshFace*>& Faces)=0;//Appends to array.
  virtual void Faces(ON_SimpleArray<const ON_XMeshFace*>& Faces) const=0;//Appends to array.
protected:
  ON_XMeshComponent();
  ON_XMeshComponent(const ON_XMeshComponent& C);//Not implemented
  ON_XMeshComponent& operator=(const ON_XMeshComponent& src);//Not implemented

  virtual void Destroy();

  ON_XMeshEventList *m_X;//events not managed here
  bool m_bIsValid; //false if any allocation failures for m_X or other
  //used by fixed pool stuff
  const ON_XMesh* m_pM;
  eXMeshPoolStatus m_status;
};

class ON_CLASS ON_XMeshVertex : public ON_XMeshComponent

{
public:
  ON_XMeshVertex(const ON_3dPoint& P);
  ~ON_XMeshVertex();
  //bool IsValid() const; Use base

  //This does not change the edge's vertices.
  void AddEdge(ON_XMeshEdge* pE);

  ON_3dPoint& Point();//I guess we should allow changes to the location
  const ON_3dPoint& Point() const;
  int EdgeCount() const;
  ON_XMeshEdge* Edge(int eid);
  const ON_XMeshEdge* Edge(int eid) const;

  void Vertices(ON_SimpleArray<ON_XMeshVertex*>& V) override;
  void Vertices(ON_SimpleArray<const ON_XMeshVertex*>& V) const override;
  void Edges(ON_SimpleArray<ON_XMeshEdge*>& E) override;
  void Edges(ON_SimpleArray<const ON_XMeshEdge*>& E) const override;
  void Faces(ON_SimpleArray<ON_XMeshFace*>& F) override;
  void Faces(ON_SimpleArray<const ON_XMeshFace*>& F) const override;
  const ON_XMeshFace* FirstFace() const;
  void CreateComponentString(ON_wString& S) const override;
private:  
  ON_XMeshVertex(); //Not implemented.
  ON_XMeshVertex(const ON_XMeshVertex& src);//Not implemented
  ON_XMeshVertex& operator=(const ON_XMeshVertex& src);//Not implemented
  
  friend class ON_XMeshVertexPool;
  friend class ON_XMeshVertexIterator;
  friend class ON_XMesh;
  friend class ON_XMeshIntersector;

  void Destroy() override;

  ON_3dPoint m_P;
  ON_SimpleArray<ON_XMeshEdge*> m_E;//edges not managed here
  int m_vid;//This is for temporary use.  In particular, when converting to an ON_Mesh
};

class ON_CLASS ON_XMeshEdge : public ON_XMeshComponent

{
public:
  ON_XMeshEdge(ON_XMeshVertex* pV0, ON_XMeshVertex* pV1);
  ~ON_XMeshEdge(); 
  bool IsValid() const override;

  const ON_3dPoint& StartPoint() const;//No checking for valid vertex
  const ON_3dPoint& EndPoint() const;//No checking for valid vertex
  ON_3dPoint MidPoint() const;//No checking for valid vertex
  ON_3dPoint PointAt(double t) const;
  ON_XMeshVertex* StartVertex();
  ON_XMeshVertex* EndVertex();
  const ON_XMeshVertex* StartVertex() const;
  const ON_XMeshVertex* EndVertex() const;

  //Finds parameter on the infinite line. Lexorders points to guarantee
  //answer independent of line direction.
  bool ClosestPointToLine(const ON_3dPoint& P, double* t) const;


  bool IsDegenerate() const;

  int FaceCount() const;
  ON_XMeshFace* Face(int fid);
  const ON_XMeshFace* Face(int fid) const;

  void Vertices(ON_SimpleArray<ON_XMeshVertex*>& V) override;
  void Vertices(ON_SimpleArray<const ON_XMeshVertex*>& V) const override;
  void Edges(ON_SimpleArray<ON_XMeshEdge*>& E) override;
  void Edges(ON_SimpleArray<const ON_XMeshEdge*>& E) const override;
  void Faces(ON_SimpleArray<ON_XMeshFace*>& F) override;
  void Faces(ON_SimpleArray<const ON_XMeshFace*>& F) const override;
  void CreateComponentString(ON_wString& S) const override;
private:
  ON_XMeshEdge(); //Not implemented.
  ON_XMeshEdge(const ON_XMeshEdge& src);//Not implemented
  ON_XMeshEdge& operator=(const ON_XMeshEdge& src);//Not implemented

  friend class ON_XMeshEdgePool;
  friend class ON_XMeshEdgeIterator;
  friend class ON_XMesh;
  friend class ON_XMeshIntersector;

  void Destroy() override;


  bool GetEvalLine(ON_Line& L, bool& bRev) const;
  void SetDegeneracy();//true if can't unitize?

  ON_XMeshVertex *m_V[2];
  ON_SimpleArray<ON_XMeshFace*> m_F;//Faces not managed here
  bool m_bIsDegenerate;
  int m_eid;
};

class ON_CLASS ON_XMeshFace : public ON_XMeshComponent

{
public:
  ON_XMeshFace(ON_XMeshVertex* pV0,
               ON_XMeshVertex* pV1, 
               ON_XMeshVertex* pV2,
               ON_XMeshEdge* pE0, bool bRev0,
               ON_XMeshEdge* pE1, bool bRev1,
               ON_XMeshEdge* pE2, bool bRev2);
  ~ON_XMeshFace();

  bool IsValid() const override;//Just checks m_bOK and for null verts, edges
  bool IsDegenerate() const;//Just look at m_bIsDegenerate
  void Box(double fmin[3], double fmax[3]) const;

  const ON_3dPoint& Point(int vid) const;//i = 0,1,2. 
  ON_3dPoint PointAt(const ON_3dPoint& abc) const;
  ON_XMeshVertex* Vertex(int vid);
  const ON_XMeshVertex* Vertex(int vid) const;
  ON_XMeshEdge* Edge(int eid, bool& bRev);
  ON_XMeshEdge* Edge(int eid);
  const ON_XMeshEdge* Edge(int eid) const;
  const ON_XMeshEdge* Edge(int eid, bool& bRev) const;
  //-1 if not found. 
  int VertexId(const ON_XMeshVertex* pV) const;//returns index in F.m_V. 
  int EdgeId(const ON_XMeshEdge* pE) const;//returns index in F.m_E.
  bool EdgeIsReversed(int eid) const;

  void Vertices(ON_SimpleArray<ON_XMeshVertex*>& V) override;
  void Vertices(ON_SimpleArray<const ON_XMeshVertex*>& V) const override;
  void Edges(ON_SimpleArray<ON_XMeshEdge*>& E) override;
  void Edges(ON_SimpleArray<const ON_XMeshEdge*>& E) const override;
  void Faces(ON_SimpleArray<ON_XMeshFace*>& F) override;
  void Faces(ON_SimpleArray<const ON_XMeshFace*>& F) const override;

  const ON_PlaneEquation& PlaneEquation() const;
  double PlaneFuzz() const;

  //Where is P in relation to the face's plane. 
  //-1 below. 1 above. 0 on. dist will be negative if below.  
  //Will return 0 if fabs(dist) < fuzz
  int ClassifyPoint(const ON_3dPoint& P, double& dist); 

  //Get barycentric coords.  Returns false if face is degenerate of algorithm fails
  bool ClosestPoint(const ON_3dPoint& P, ON_3dPoint& abc);

  //If face isn't degenerate, get the vector perp to edge pointing to the
  //interior of the face from the edge midpt
  bool GetVectorIntoFace(int eid, ON_3dVector& V) const;
  bool GetVectorIntoFace(const ON_XMeshEdge* pE, ON_3dVector& V) const;

  //Returns index in F.m_V or -1.
  int VertexNotOnEdge(const ON_XMeshEdge* pE) const;//if pE is not an edge of face, returns -1
  int VertexNotOnEdge(int eid) const;//input index into F.m_E. 0, 1, 2.

  bool AddFF(ON_XMeshFF* pFF);
  ON_XMeshFF* FindFF(int this_side, ON_XMeshFace* pFOther);
  bool CreatePlane();//Only fails if there's an allocation problem.
  //If degenerate, will not make a plane but will flag the face as degenerate.
  bool HasXData() const;
  void CreateComponentString(ON_wString& S) const override;
private:
  ON_XMeshFace(const ON_XMeshFace& src);//Not implemented
  ON_XMeshFace& operator=(const ON_XMeshFace& src);//Not implemented
  
  friend class ON_XMeshFacePool;
  friend class ON_XMeshFaceIterator;
  friend class ON_XMesh;
  friend class ON_XMeshIntersector;
  ON_XMeshFace();
  void Create(ON_XMeshVertex* pV0,
         ON_XMeshVertex* pV1, 
         ON_XMeshVertex* pV2,
         ON_XMeshEdge* pE0, bool bRev0,
         ON_XMeshEdge* pE1, bool bRev1,
         ON_XMeshEdge* pE2, bool bRev2);
  void Destroy() override;

  bool SetPlaneEquation();//Sets plane equation and plane fuzz.
  bool SetDegeneracy();//Figure out if degenerate by checking if edges are.  Only called during mesh creation
  int FFCount() const;
  ON_XMeshFF* FF(int id);
  const ON_XMeshFF* FF(int id) const;
  bool IntersectEdgesWithPlanes(ON_XMeshEventPool& XPool);

  bool m_bIsDegenerate;
  ON_XMeshVertex* m_V[3];
  ON_XMeshEdge* m_E[3];//edge i ends at vertex i, when taking reve into account
  bool m_bRev[3];
  ON_XMeshPlaneEquation* m_plane_eq; 

  ON_XMeshFaceXData *m_XData;
  bool m_bOK;//Verts, edges all there and are consistent with bRev
  int m_fid;
};

class ON_CLASS ON_XMesh

{
public:
  ON_XMesh();
  ~ON_XMesh();

  bool IsValid() const;

  int VertexCount() const;
  int EdgeCount() const;
  int FaceCount() const;

  bool Create(const ON_Mesh& M, 
              ON_XMeshVertexPool& VPool, 
              ON_XMeshEdgePool& EPool, 
              ON_XMeshFacePool& FPool
              );
  bool Create(const ON_Mesh& M);
  ON_XMeshEdge* FindEdge(ON_XMeshVertex* pV0, ON_XMeshVertex* pV1, bool& bRev);
  const ON_XMeshEdge* FindEdge(const ON_XMeshVertex* pV0, const ON_XMeshVertex* pV1, bool& bRev) const;
  ON_Mesh* ConvertToOnMesh();
private:
  ON_XMesh(const ON_XMesh& src);//Not implemented.
  ON_XMesh& operator=(const ON_XMesh& src);//Not implemented.
  //Private since it is assumes that MT comes from a triangle mesh.
  //Called from within ON_XMesh(const ON_Mesh& M). Also, it is assumed that
  //nothing has been returned to any of the component pools.
  bool Create(const ON_MeshTopology& MT);

  //Allocation. Increments m_*count
  ON_XMeshVertex* NewVertex(const ON_3dPoint& P);
  ON_XMeshEdge* NewEdge(ON_XMeshVertex* pV0, ON_XMeshVertex* pV1);
  ON_XMeshFace* NewFace(ON_XMeshVertex* pV0, ON_XMeshVertex* pV1, ON_XMeshVertex* pV2);

  //Deletion. Decrements m_*count.
  void ReturnVertex(ON_XMeshVertex*& pV);
  void ReturnEdge(ON_XMeshEdge*& pE);
  void ReturnFace(ON_XMeshFace*& pF);

  bool m_bDeletePools;
  ON_XMeshVertexPool* m_pVPool;
  ON_XMeshEdgePool* m_pEPool;
  ON_XMeshFacePool* m_pFPool;

  int m_vcount;
  int m_ecount; 
  int m_fcount;

  bool m_bIsValid;
};

class ON_CLASS ON_XMeshIntersector

{
public:
  ON_XMeshIntersector(const ON_Mesh& M0, const ON_Mesh& M1, bool bMultiThread);
  ~ON_XMeshIntersector();
  bool IsValid() const;

  bool IntersectFaces(double test_tolerance);
  ON_Mesh* ConvertToOnMesh(int i);

  //For debugging.
  void GetEventPoints(ON_SimpleArray<ON_3dPoint>& pts) const;
  void GetLabels(int mi, ON_ClassArray<ON_TextDot>& Dots) const;
  void GetComponents(ON_SimpleArray<ON_XMeshVertex*> V[2], 
                     ON_SimpleArray<ON_XMeshEdge*> E[2], 
                     ON_SimpleArray<ON_XMeshFace*> F[2], 
                     ON_SimpleArray<ON_XMeshFF*>& FF, 
                     ON_SimpleArray<ON_XMeshEvent*>& X,
                     ON_SimpleArray<ON_XMeshEventComponentString>& S
                     );
     

private:
  ON_XMeshIntersector();//Not implemented
  ON_XMeshIntersector(const ON_XMeshIntersector& src);//Not implemented
  ON_XMeshIntersector& operator=(const ON_XMeshIntersector& src);//Not implemented.
  
  bool MultiThread() const;
  ON_XMeshFF* NewFF(ON_XMeshFace* pF0, ON_XMeshFace* pF1);
  void ReturnFF(ON_XMeshFF*& pFF);//Does not remove from m_XData->m_FF for each face
  ON_XMeshEvent* NewEvent();
  void ReturnEvent(ON_XMeshEvent*& pX);//Removes from all m_X and m_FF lists
  void SetMeshOrder(ON_XMesh& M0, ON_XMesh& M1);
  //pF0 is a mesh from m_Mesh[0].  Find all m_Mesh[1] faces that might intersect
  bool CreateFaceTree();
  bool SetUpFFs(ON_XMeshFace* pF0, double test_tolerance);
  bool SetUpFFs(double test_tolerance);//Puts FFs on faces of M0 and M1 for every collision
  //Also makes planes for non-degenerate pfaces and flags others as degenerate
  bool SetUpDegenerateFacePlanes(ON_SimpleArray<ON_XMeshFace*> Degen[2]);//Figure out planes for degenerate faces using planes of adjacents.
  bool CheckVerticesOnPlanes();
  bool IntersectEdgesWithPlanes();//Assumes CheckVerticesOnPlanes already called
    
  bool DistributeEvents();
  bool GetHitList(const ON_XMeshFace* pF0, double tolerance,
                  ON_SimpleArray<ON_XMeshFace*>& F1List);
  bool AdjustEvents();
  //XRet will be the list of events that should be returned. 
  bool AdjustVertexEdge(int vside, 
                        ON_SimpleArray<ON_XMeshEvent*>& EdgeEvents,
                        ON_SimpleArray<ON_XMeshEvent*>& FaceEvents);
  bool AdjustVertexEdge(ON_XMeshVertex* pV, int vside, 
                        ON_SimpleArray<ON_XMeshEvent*>& EdgeEvents,
                        ON_SimpleArray<ON_XMeshEvent*>& FaceEvents);
  //XArray is list of events for V on faces of E
  ON_XMeshEvent* AdjustVertexEdge(int vside, ON_XMeshEdge* pE, 
                                  ON_SimpleArray<ON_XMeshEvent*>& XArray);
  ON_XMeshEvent* AdjustVertexNakedEdge(int vside, ON_XMeshEdge* pE, ON_XMeshEvent* pX);
  ON_XMeshEvent* CreateVertexOnEdgeEvent(int vside, ON_XMeshVertex* pV, ON_XMeshEdge* pE);
  bool AdjustVertexEdge();
  bool AdjustVertexVertex();
  bool AdjustEdgeEdge();
  bool CollectFaceEvents();
  bool LinkEvents();

  //For iteration limits.
  int FFCount() const;
  int EventCount() const;

  bool m_bIsValid;
  bool m_bSwap;//If true, then m_Mesh[0] is M1;
  ON_XMesh* m_Mesh[2];//only m_Mesh[1] will have a a spacial decomposition. 
  ON_XMeshVertexPool m_VPool;
  ON_XMeshEdgePool m_EPool;
  ON_XMeshFacePool m_FPool;
  ON_RTree m_FaceTree;//For M[1].
  ON_XMeshFFPool m_FFPool;
  ON_XMeshEventPool m_XPool;

  bool m_bUseMultipleThreads;

  /* Just a thought.  Don't bother until deemed worthwhile.
  ON_SimpleArray<ON_XMeshFace*> m_FList[2];//List of faces that have xdata.
  //If m_ffcount > 0 and m_Flist[0] is empty, then not used.  Only use if there
  //are a lot of faces and only a few intersections.
  */

};
#endif
