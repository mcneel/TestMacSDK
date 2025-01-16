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

class RHINO_SDK_CLASS CRhinoXformObjectList
{
public:
  CRhinoXformObjectList();
  ~CRhinoXformObjectList();
  CRhinoXformObjectList(const CRhinoXformObjectList&);
  CRhinoXformObjectList& operator=(const CRhinoXformObjectList&);

  // If an object is in the m_objects[] list, then it is not
  // in the m_grip_owners[] list, and vice verse.
  void Empty();

  /*
  Description:
    Empties the current list and then adds objects in go's object list.
  Parameters:
    go - [in] The list of objects in the getter is added.
    bAllowAllGrips - [in]
      If false and all of an objects grips are selected,
      then the object itself is added.  If true, then
      grips are added even when all of an object's grips
      are selected.
  Returns:
    Number of objects and grips added to the list.
  */
  int AddObjects( const CRhinoGetObject& go, bool bAllowAllGrips = false);

  /*
  Description:
    Empties the current list and then adds objects returned by the
    iterator.
  Parameters:
    go - [in] The list of objects in the getter is added.
    bAllowAllGrips - [in]
      If false and all of an objects grips are selected,
      then the object itself is added.  If true, then
      grips are added even when all of an object's grips
      are selected.
  Returns:
    Number of objects and grips added to the list.
  */
  int AddObjects( CRhinoObjectIterator& it, bool bAllowAllGrips );

  // returns 0 = not added, 1 = added as object, 2 = added as grip
  // Use this to add Polyedges so the references are properly counted.
  int AddObject( const CRhinoObjRef& objref, bool bAllowAllGrips = false);

  // returns 0 = not added, 1 = added as object, 2 = added as grip
  int AddObject( const CRhinoObject* object, bool bAllowAllGrips = false);

  // No duplicates in any list and no objects appear in more
  // than one list.  Use the AddObject() functions above to create
  // these lists.
  unsigned int m_rhino_doc_sn = 0; // Doc serial number set by add objects.
  ON_SimpleArray<CRhinoObject*> m_objects;       // regular objects 
  ON_SimpleArray<CRhinoGripObject*> m_grips;     // grips
  ON_SimpleArray<CRhinoObject*> m_grip_owners;   // objects the grips will change
  ON_SimpleArray<CRhinoObject*>* GripCaptives() const; // grip captives (objects in cages with grips on)

  /*
  Returns:
    m_objects.Count() + m_grip.Count();
  */
  int ObjectCount() const;
  
  //////////////////////////////////////////////////////////////
  //
  // Query tools
  //

  /*
  Returns:
    True if m_objects[] contains a polysurface (brep with 2 or more faces).
  */
  bool HasPolysurfaces() const;

  /*
  Returns:
    True if m_objects[] contains a surface (brep with 1 face).
  */
  bool HasSurfaces() const;

  /*
  Returns:
    True if m_objects[] contains a brep (any number of faces).
  */
  bool HasBreps() const;

  /*
  Returns:
    If objects are in the list, then a pointer to the document
    containing those objects is returned.  Otherwise NULL is
    returned.  If you call this function, you must check
    for a NULL pointer before dereferencing!
  */
  CRhinoDoc* Document() const;

  /*
  Returns:
    Space containing the objects;
  */
  ON::active_space ViewSpace() const;

  //////////////////////////////////////////////////////////////
  //
  // Display feedback tools
  //

  /*
  Description:
    Call EnableDisplayFeedback(true) if you want this class to 
    manage a CRhinoDisplayPipeline that provides display feedback.
  Parameters:
    bEnable - [in]
      true to enable display feedback.
      false to disable display feedback.
  */
  bool EnableDisplayFeedback(bool bEnable);

  /*
    Returns true if this class is using a CRhinoDisplayPipeline
    to provide display feedback.
  */
  bool DisplayFeedbackEnabled() const;

  /*
  Description:
    If display feedback is enabled, then call this function
    when the transformation changes.
  */
  bool UpdateDisplayFeedbackXform(const ON_Xform& xform);

  /*
  Description:
    If display feedback is enabled, then call this function
    when a drag changes the translation.
  */
  bool UpdateDisplayFeedbackXform(
    const ON_Xform& xform, 
    CRhinoViewport* vp, 
    UINT nFlags, 
    const ON_3dPoint* point, 
    const ON_2iPoint* screen_point,    
    const ON_3dVector* grip_frame_scale
    );

  /*
  Description:
    Returns the world coordinate axis-aligned tight bounding box of 
    the Rhino objects found in the CRhXformObjectList::m_objects array.
  */
  bool ObjectListTightBoundingBox(ON_BoundingBox& bbox);
  bool ObjectListTightBoundingBox(ON_BoundingBox& bbox, bool bIncludeGrips);


public:
  // For core Rhino use only.
  class CRhXformObjectListEx* Internal_Extension() const;

private:
  mutable class CRhXformObjectListEx* m__ex = nullptr; 

private:
  ON__UINT_PTR m__reserved = 0;
};

/*
Description:
  Base class for object transformation point getters.
  
  You will never directly create a CRhinoGetXform but you will use
  its member functions after calling CRhinoGetXform::GetXform(). 
*/
class RHINO_SDK_CLASS CRhinoGetXform : public CRhinoGetPoint
{
public:
  CRhinoGetXform();
  virtual ~CRhinoGetXform() = default;

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Add any objects you want transformed and grips you want
  //         transformed.  Make sure no duplicates are in the list
  //         and that no grip ownwers are passed in as objects.
  //
  void AppendObjects( const CRhinoGetObject& get );
  void AppendObjects( CRhinoObjectIterator& it );
  void AppendObjects( const CRhinoXformObjectList& list );
  void AppendObject( const CRhinoObject* object );

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Override this virtual function to provide your own custom
  //         transformation method.  Call this function to retrieve
  //         the final transformation.
  //
  virtual BOOL32 CalculateTransform( CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform ) = 0;

  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Override this virtual function to provide your own custom
  //         method get a transform object.
  //
  virtual CRhinoGet::result GetXform( CRhinoHistory* pHistory = nullptr );

  //////////////////////////////////////////////////////////////////
  //
  // Overridden members
  //
  void SetBasePoint( ON_3dPoint base_point, bool bShowDistanceInStatusBar = false );
  void OnMouseMove( CRhinoViewport& vp, UINT nFlags, const ON_3dPoint& pt, const ON_2iPoint* p ) override;
  void DynamicDraw( CRhinoDisplayPipeline& dp, const ON_3dPoint& pt ) override;

  BOOL32 m_bHaveXform;
  bool m_bMouseDrag; // true if transformation is from a mouse drag
  bool m_bReserved1;
  bool m_bReserved2;
  bool m_bReserved3;
  ON_Xform m_xform;
  ON_3dPoint m_basepoint;
  const CRhinoXformObjectList& ObjectList() const; // returns m_list.

  //////////////////////////////////////////////////////////////////
  //
  // Tools to support custom grip moving relative to the frame returned
  // by CRhinoGripObject::GetGripDirections()
  //

  /*
  Description:
    This is a utility function that can be called in CalculateTransform()
    if you want to transform grips relative to the frame returned by
    CRhinoGripObject::GetGripDirections().
  */
  void SetGripFrameTransform( double x_scale, double y_scale, double z_scale );

  /*
  Description:
    If GetGripFrameTransform() returns true, then grips should be
    transformed by moving them in the translation returned by
    GetGripTranslation().  If GetGripFrameTransform() returns false,
    then grips should be transformed by m_xform.
  */
  bool HasGripFrameTransform() const;
  bool GetGripFrameTransform( double* x_scale, double* y_scale, double* z_scale ) const;

  /*
  Description:
    Resets any grips that were moved.
  */
  void ResetGrips( bool bForceRedraw );

private:
  CRhinoXformObjectList m_list;
  ON__UINT_PTR m_sdk_reserved = 0;
};

