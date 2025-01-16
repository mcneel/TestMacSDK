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

#include "rhinoSdkCoreClasses.h"
#include "rhinoSdkObjectAttributes.h"
#include "rhinoSdkAppSettings.h"
#include "RhinoSdkSpaceMorph.h"

#include <vector>
#include <memory>


namespace RhRdk
{
  namespace CustomRenderMeshes
  {
    class IRenderMeshes;
  }
}

class RHINO_SDK_CLASS CRhinoCacheHandle  
{
public:
  CRhinoCacheHandle() = default;

  std::shared_ptr<class CRhVboData> m_vbo_data;
  std::shared_ptr<class CRhVboCurveSetData> m_vbo_curve_data;
  std::shared_ptr<class CRhCacheData> m_cache_data;
  const CRhinoObject* ParentObject() const;
  void Reset();
private:
  friend class CRhinoObject;
  CRhinoObject* m_parent_object = nullptr;
  void DeleteFromDocNotification();
};

class CRhinoMaterial;
class CRhRdkMaterial;
class CRhinoClippingPlaneObject;

// Base class for all runtime Rhino objects.
class RHINO_SDK_CLASS CRhinoObject : public ON_Object
{
  ON_OBJECT_DECLARE(CRhinoObject);

private:
  // The m_rhino_object_sn field is private because nothing must 
  // ever pass this value by reference. It must be the first member
  // of CRhinoObject and is initialized before any other construction
  // code is called.
  //
  // Call CRhinoObject.RuntimeSerialNumber() when you want this value.
  //
  // Call CRhinoObject::FromRuntimeSerialNumber() when you need
  // to convert a rhino object runtime serial number into a 
  // CRhinoObject pointer.
  const unsigned int m_rhino_object_sn;

public:
  // virtual ON_Object override
  void DestroyRuntimeCache( bool bDelete = true ) override;

  // virtual ON_Object override
  void MemoryRelocate() override;

  ON_UUID ModelObjectId() const override;
public:
  CRhinoObject();                              // assigns a new object UUID
  CRhinoObject(const ON_3dmObjectAttributes&);
  virtual ~CRhinoObject();
  CRhinoObject( const CRhinoObject& );          // copies source object UUID
  CRhinoObject& operator=(const CRhinoObject&); // copies source object UUID
  
  /*
  Description:
    Duplicates the Rhino object.  If "this" is a CRhinoProxyObject,
    the duplicate is a "real" Rhino object.  If "this" is an
    ordinary Rhino object like a CRhinoCurveObject, CRhinoPointObject,
    and so on, then the duplicate is another rhino object of the
    same type.    
  Returns:
    NULL or a pointer to the duplicate.  The new object is
    not added to the Rhino document.
  */
  virtual
  CRhinoObject* DuplicateRhinoObject() const;

public:
  /*
  Description:
    The CRhinoObject runtime serial number is a value >= 1 that 
    and is unique for each instance of a CRhinoObject class in a
    given instance of the Rhino.exe application.
    The first CRhinoObject created has serial number 1.
    The serial number can be used to identify find and identify 
    Rhino objects and is a fast and robust way to specify a
    rhino object (as opposed to passing around a CRhinoObject
    pointer or reference).
    A value of 0 is unset and never identifies an object.
  See Also:
    CRhinoObject::FromSerialNumber();
  Remarks:
    The CRhinoObject runtime serial number value is not saved
    in 3DM archives and is not unique between instances of 
    Rhino.exe.
  */
  unsigned int RuntimeSerialNumber() const;

  /*
  Description:
    Get a pointer to a CRhinoObject from a runtime_serial number or id.
  Parameters:
    rhino_doc_runtime_serial_number - [in]
      If you want to search all documents, pass 0.  Specifying a document
      improves performance.
    rhino_object_runtime_serial_number - [in]
    rhino_object_id - [in]
  Returns:
    The Rhino object with the matching runtime serial number 
    or null if no object exists.
  Remarks:
    - The version that takes the document runtime serial number
      is faster and should be used when performance is critical.
    - The runtime serial number does not persist in 3dm archives
      and is not unique between instance of Rhino.exe.
    - Deleted objects can be found by object_runtime_serial_number.
    - Deleted objects cannot be found by id.
  See Also:
    CRhinoObject::RuntimeSerialNumber();
  */
  static const CRhinoObject* FromRuntimeSerialNumber( 
    unsigned int rhino_doc_runtime_serial_number,
    unsigned int rhino_object_runtime_serial_number 
    );

  static const CRhinoObject* FromId( 
    unsigned int rhino_doc_runtime_serial_number,
    ON_UUID rhino_object_id
    );

  /*
  Description:
    Get the runtime serial number that will be assigned to
    the next CRhinoObject that is created.
  Returns:
    The runtime serial number that will be assigned to
    the next CRhinoObject that is created.
  Example:

          unsigned int s0 = CRhinoObject::NextRuntimeSerialNumber();
          ... do a bunch of stuff
          unsigned int s1 = CRhinoObject::NextRuntimeSerialNumber();
          ...
          const CRhinoObject* pObject = ...;
          if (    s0 <= pObject->RuntimeSerialNumber() 
               && pObject->RuntimeSerialNumber() < s1 )
          {
           // pObject was created during "... do a bunch of stuff"
          }

  See Also:
    CRhinoObject::RuntimeSerialNumber()
  */
  static unsigned int NextRuntimeObjectSerialNumber();

  /*
  Description:
    Returns a constant that determines the type of
    CRhinoObject.
  Returns:
    @untitled table
    ON::point_object             CRhinoPointObject
    ON::pointset_object          CRhinoPointCloudObject
    ON::curve_object             CRhinoCurveObject
    ON::brep_object              CRhinoBrepObject
    ON::mesh_object              CRhinoMeshObject
    ON::annotation_object        CRhinoAnnotation
    ON::light_object             CRhinoLight
  Remarks:
    A brep object can be a surface or polysurface. An annotation
    object can be anything derived from CRhinoAnnotation.
  */
  virtual
  ON::object_type ObjectType() const override = 0;

  // Geometry types for CRhinoObjRef (same as the ON::object_type)
  //   In an ideal world, the CRhinoObject::GEOMETRY_TYPE would be removed 
  //   and ON::object_type would be used everywhere.
  enum GEOMETRY_TYPE
  {
    unknown_object_type  = ON::object_type::unknown_object_type,
    point_object         = ON::object_type::point_object,     // single point
    pointset_object      = ON::object_type::pointset_object,  // point cloud or point grid
    curve_object         = ON::object_type::curve_object,     // curve (can be 3d edge in a brep)
    surface_object       = ON::object_type::surface_object,   // surface (can be face in a brep)
    polysrf_object       = ON::object_type::polysrf_filter,   // brep with 2 or more faces
    edge_object          = ON::object_type::edge_filter,      // brep edge (with an associated ON_BrepTrim)
    polyedge_object      = ON::object_type::polyedge_filter,  // a chain of edges and/or curves
    loop_object          = ON::object_type::loop_object, // brep loop
    brepvertex_filter    = ON::object_type::brepvertex_filter, // brep vertex
    mesh_object          = ON::object_type::mesh_object,
    annotation_object    = ON::object_type::annotation_object,
    light_object         = ON::object_type::light_object,
    instance_reference   = ON::object_type::instance_reference,
    grip_object          = ON::object_type::grip_object,
    morph_control_object = ON::object_type::morph_control_object,
    subd_object          = ON::object_type::subd_object,
    meshvertex_filter    = ON::object_type::meshvertex_filter, // mesh vertex as an ON_MeshComponentRef
    meshedge_filter      = ON::object_type::meshedge_filter,   // mesh edge as an ON_MeshComponentRef
    meshface_filter      = ON::object_type::meshface_filter,   // mesh triangle, quad or ngon as an ON_MeshComponentRef
    meshcomponent_reference = ON::object_type::meshcomponent_reference, // mesh vertex, edge, face, ngon (ON_MeshComponentRef)
    cage_object          = ON::object_type::cage_object,
    hatch_object         = ON::object_type::hatch_object,
    phantom_object       = ON::object_type::phantom_object,
    extrusion_object     = ON::object_type::extrusion_object,
    any_object           = ON::object_type::any_object
  };

  ///////////////////////////////////////////////////////////////////
  //
  // ON_Object overrides
  //

  bool IsValid( ON_TextLog* text_log = nullptr ) const override;

  // Description:
  //   Gets document that owns the object.
  //   
  // Returns:
  //   Document that manages object.
  //
  // Remarks:
  //   When an object is created, it does not belong to a document.
  //   CRhinoDoc::AddObject() or CRhinoDoc::ReplaceObject() are used
  //   to add new objects to a document.  After the object is added
  //   to a document, it it managed by that document.  If the
  //   CRhinoObject::Document() returns NULL, then the object is
  //   not part of a document.
  unsigned int DocumentRuntimeSerialNumber() const;

  class CRhinoDoc* Document() const;

  /*
  Description:
    Some objects cannot be deleted, like grips on lights and
    annotation objects. 
  Returns:
    True if object can be deleted. 
  */
  virtual
  bool IsDeletable() const;

  /*
  Description:
    Returns true if the object is deleted.  
 
  Returns:
    true if object is deleted.
 
  Remarks:
    Use CRhinoDoc::UndeleteObject() to undelete an object.
  */
  bool IsDeleted() const;

  /*
  Description:
    Determine if an object is active in a document.  This is
    a required piece of information when deciding if a modification
    to an object should set the document modified flag.
  Returns:
    0: The object does not belong to a document or it is deleted.
    != 0: The object is active in a document and the serial number
          of that document is returned.
  */
  unsigned int IsActiveInDocument() const;

  /*
  Description:
    This call informs an object it is about to be deleted.
    Some objects, like clipping planes, need to do a little
    extra cleanup before they are deleted.
  */
  virtual
  void DeleteFromDocNotification();

  /*
  Description:
    This call informs an object it is about to be added to
    the list of active objects in the document.
    Some objects, like clipping planes, need to do a little
    extra cleanup before they are deleted.
  */
  virtual
  void AddToDocNotification();

  int IsProxy() const;

  /*
  Description:
    Returns true if the object is used as part of an instance
    definition.   
  Returns:
    true if object is is used as part of an instance
    definition.
  */
  bool IsInstanceDefinitionGeometry() const;

  /*
  Description:
    Returns the value of m_is_idef_geometry.
  */
  int IsInstanceDefinitionGeometryEx() const;

  /*
  Description:
    Test object to see if it is a proxy CRhinoGroupObject.
  Returns:
    true if this is a CRhinoGroupObject.
  */
  virtual
  bool IsGroupObject() const;
    

  ///////////////////////////////////////////////////////////////////

  // there are 3 mutually exclusive object states 
  // - normal (visible, can be snapped to, independent of selection state)
  // - locked (visible, can be snapped to, cannot be selected)
  // - hidden (not visible, cannot be snapped to, cannot be selected)
  bool IsNormal() const;         
  bool IsLocked() const;
  bool IsHidden() const;

  /*
  Description:
    See if at least one subobject has at least on of the states 
    specified by the status parameter.
  Parameters:
    status - [in]
      Set the states you want to test for.  
  Returns:
    If any subobject has a status state set to a non-default value,
    the corresponding state is set in the returned ON_ComponentStatus.
  Remarks:
    For the purpose of this function, 
    ON_ON_ComponentStatus::SELECTED_STATE::selected and
    ON_ON_ComponentStatus::SELECTED_STATE::selected_persistent
    are treated as equal states.
  */
  virtual
  ON_ComponentStatus SubObjectStatus() const;

  /*
  Description:
    Determine if an object is a reference object.
  Returns:
    True if the object is a reference object.
  Remarks:
    An object from a work session reference model is reference a
    reference object and cannot be modified.  An object is a reference
    object if, and only if, it is on a reference layer.
  */
  bool IsReference() const;

  /*
  Returns:
    Worksession reference model id.
         0: layer is not in a reference model.
         1: layer is in an unidentified worksession reference model.
     >1000:
       Serial number of the worksession reference model.       
  Remarks:
    The reference model serial number is a runtime value.
    It is not saved in files and it is generally different the
    next time a file is read.
  */
  unsigned int WorksessionReferenceModelSerialNumber() const;
  
  /*
  Returns:
    0: 
      The object is not part of a linked instance definition
    1-1000: 
      These numbers are reserved for future use.
    >1000:
      CRhinoDoc::m_instance_defintion_table.FindBySerialNumber() can
      be used to find the linked instance definition this object
      is part of.
  Remarks:
    The linked instance definition serial number is a runtime value.
    It is not saved in files and it is generally different the next
    time a file is read.
  */
  unsigned int LinkedInstanceDefinitionSerialNumber() const;

  /*
  Returns:
    True if the object is in model space.  
  */
  bool IsModelObject() const;

  /*
  Returns:
    True if the object is in page layout space.
  */
  bool IsPageObject() const;

  /*
  Returns:
    The space this object belongs to.
  */
  ON::active_space Space() const;

  //////////
  // Returns true if an object is visible and is on a layer
  // that is visible.
  bool IsVisible() const;

  /*
  Parameters:
    viewport_id - [in]
      If viewport_id is nil or does not identify an existing viewport, then
      the IsVisible() is returned.
  Return:
    true if the object is visible in the viewport.

  */
  bool PerViewportIsVisible(
    ON_UUID viewport_id
  ) const;

  /*
  Description:
    Query to determine if this object is active in a
    particular viewport.  
  Parameters:
    viewport - [in]
  Returns:
    True if the object is active in viewport.
  Remarks:
    The default implementation tests for space and viewport id.
    This handles things like testing if a page space object
    is visible in a modeling view.
  */
  virtual 
  bool IsActiveInViewport( const class CRhinoViewport& ) const;

  /*
  Description:
    Check selection state.
  Parameters:
    bCheckSubObjects - [in]
              If true and the entire object is not selected,
         and some subset of the object is selected, like some edges
         of a surface, then 3 is returned.
              If false and the entire object is not selected,
         then zero is returned.
  Returns:
    @untitled table
    0      object is not selected
    1      object is selected
    2      entire object is selected persistently
    3      one or more proper sub-objects are selected
  */
  int IsSelected( bool bCheckSubObjects = false ) const;

  /*
  Description:
    Check sub-object selection state.
  Parameters:
    component_index - [in] index of subobject to check
  Remarks:
    subobject cannot be persistently selected
  Returns:
    @untitled table
    false      sub-object is not selected
    true      sub-object is selected
  */
  virtual
  bool IsSubObjectSelected( ON_COMPONENT_INDEX component_index ) const;

  /*
  Description:
    Get a list of all selected sub-objects
  Parameters:
    component_indices - [out] component indices of selected sub-objects.
  Returns:
    number of selected subobjects
  */
  virtual
  int GetSelectedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const;

  /*
  Description:
    Reports if an object can be selected.
  Parameters:
    bIgnoreSelectionState - [in] (default=false)  If true, then
        selected objects are selectable.  If false, the
        selected objects are not selectable.
    bIgnoreGripsState - [in]  If true, then objects with grips on
        can be selected.  If false, then the value returned by
        the object's virtual IsSelectableWithGripsOn() function
        decides if the object can be selected.
  Remarks:
    Objects that are locked, hidden, or on locked or hidden layers
    cannot be selected. If IsSelectableWithGripsOn() returns false,
    then an that object is not selectable if it has grips turned on.

    Calls the virtual function InternalVirtual_IsSelectable(bIgnoreSelectionState,bIgnoreGripsState,false,false)
  */
  bool IsSelectable( // true if object is capable of being selected
         bool bIgnoreSelectionState = false,
         bool bIgnoreGripsState = false
         )
         const; 

  /*
  Description:
    Reports if an object can be selected.
  Parameters:
    bIgnoreSelectionState - [in] (default=false)  If true, then
        selected objects are selectable.  If false, the
        selected objects are not selectable.
    bIgnoreGripsState - [in]  If true, then objects with grips on
        can be selected.  If false, then the value returned by
        the object's virtual IsSelectableWithGripsOn() function
        decides if the object can be selected.
    bIgnoreLocking - [in] (default=false)  
        If true, then locked objects and objects on locked layers
        are selectable.  If false, then locked objects and objects
        on locked layers are not selectable.
    bIgnoreVisibility - [in] (default=false)  
        If true, then hidden objects and objects on hidden layers
        are selectable.  If false, then hidden objects and objects
        on hidden layers are not selectable.
  Returns:
    True if object is capable of being selected
  Remarks:
    Objects that are locked or hidden cannot be selected. 
    If IsSelectableWithGripsOn() returns false,
    then an that object is not selectable if it has grips turned on.

    Calls the virtual function InternalVirtual_IsSelectable(bIgnoreSelectionState,bIgnoreGripsState,bIgnoreLocking,bIgnoreVisibility)
  */
  bool IsSelectable(
         bool bIgnoreSelectionState ,
         bool bIgnoreGripsState,
         bool bIgnoreLocking,
         bool bIgnoreVisibility
         )
         const; 

protected:
  /*
  Description:
    Objects derived from CRhinoObject may override InternalVirtual_IsSelectable()
    when the default behavior needs to be modified.
  */
  virtual bool InternalVirtual_IsSelectable(
         bool bIgnoreSelectionState ,
         bool bIgnoreGripsState,
         bool bIgnoreLocking,
         bool bIgnoreVisibility
  ) const;

public:

  /*
  Description:
    Reports if a subobject can be selected.
  Parameters:
    component_index - [in] index of subobject to check
    bIgnoreSelectionState - [in] (default=false)  If true, then
        selected objects are selectable.  If false, the
        selected objects are not selectable.
  Remarks:
    Objects that are locked, hidden, or on locked or hidden layers
    cannot be selected. If IsSelectableWithGripsOn() returns false,
    then an that object is not selectable if it has grips turned on.
  */
  bool IsSubObjectSelectable( 
           ON_COMPONENT_INDEX component_index, 
           bool bIgnoreSelectionState = false 
           ) const;

  /*
  Description:
    Reports if an object can be selected.
  Parameters:
    bSelect - [in] (default=true)
    bSynchHighlight - [in] (default=true)
        If true, then the object is highlighted if it is selected
        and not highlighted if is is not selected.
    bPersistentSelect - [in] (default=true)
        Objects that are persistently selected stay selected when
        a command terminates.
    bIgnoreGripsState - [in]  If true, then objects with grips on
        can be selected.  If false, then the value returned by
        the object's virtual IsSelectableWithGripsOn() function
        decides if the object can be selected when it has grips
        turned on.
  Returns:
    0: object is not selected
    1: object is selected
    2: object is selected persistently
  Remarks:
    Objects that are locked, hidden, or on locked or hidden layers
    cannot be selected. If IsSelectableWithGripsOn() returns false,
    then an that object is not selectable if it has grips turned on.
  See Also:
    CRhinoDoc::UnselectAll
  */
  int Select(
        bool bSelect = true,
        bool bSynchHighlight = true,
        bool bPersistentSelect = true,
        bool bIgnoreGripsState = false
        ) const;

  /*
  Description:
    Reports if an object can be selected.
  Parameters:
    bSelect - [in] (default=true)
    bSynchHighlight - [in] (default=true)
        If true, then the object is highlighted if it is selected
        and not highlighted if is is not selected.
    bPersistentSelect - [in] (default=true)
        Objects that are persistently selected stay selected when
        a command terminates.
    bIgnoreGripsState - [in]  If true, then objects with grips on
        can be selected.  If false, then the value returned by
        the object's virtual IsSelectableWithGripsOn() function
        decides if the object can be selected when it has grips
        turned on.
    bIgnoreLayerLocking - [in] (default=false)  If true, then
        objects on locked layers can be selected.  If false, the
        objects on locked layers cannot be selected.
    bIgnoreLayerVisibility - [in] (default=false)  If true, then
        objects on hidden layers can be selectable.  If false, the
        objects on hidden layers cannot be selected.
  Returns:
    0: object is not selected
    1: object is selected
    2: object is selected persistently
  Remarks:
    Objects that are locked, hidden, or on locked or hidden layers
    cannot be selected. If IsSelectableWithGripsOn() returns false,
    then an that object is not selectable if it has grips turned on.
  See Also:
    CRhinoDoc::UnselectAll
  */
  int Select(
        bool bSelect,
        bool bSynchHighlight,
        bool bPersistentSelect,
        bool bIgnoreGripsState,
        bool bIgnoreLayerLocking,
        bool bIgnoreLayerVisibility
        ) const;

  /*
  Description:
    Reports if an object can be selected.
  Parameters:
    component_index - [in] index of subobject to check
    bSelect - [in] (default=true)
    bSynchHighlight - [in] (default=true)
        If true, then the object is highlighted if it is selected
        and not highlighted if is is not selected.
    bPersistentSelect - [in] (default=false)
        Subobjects that are persistently selected stay selected when
        a command terminates.
  Returns:
    0: object is not selected
    1: object is selected
    2: object is selected persistently
  Remarks:
    Objects that are locked, hidden, or on locked or hidden layers
    cannot be selected. If IsSelectableWithGripsOn() returns false,
    then an that object is not selectable if it has grips turned on.
  See Also:
    CRhinoDoc::UnselectAll
    CRhinoView::UnselectAllSubObjects
  */
  virtual
  int SelectSubObject(
        ON_COMPONENT_INDEX component_index,
        bool bSelect = true,
        bool bSynchHighlight = true,
        bool bPersistentSelect = false
        ) const;

  virtual
  int UnselectAllSubObjects(); // returns number of unselected subobjects

  // highlighted state (almost always = IsSelected() except in one or two commands)
  /*
  Description:
    Check highlight state.
  Parameters:
    bCheckSubObjects - [in]
              If true and the entire object is not highlighted,
         and some subset of the object is highlighted, like some edges
         of a surface, then 3 is returned.
              If false and the entire object is not highlighted,
         then zero is returned.
  Returns:
    @untitled table
    0      object is not highlighted
    1      entire object is highlighted
    3      one or more proper sub-objects are highlighted
  */
  int IsHighlighted(BOOL32 bCheckSubObjects = false ) const;

  bool Highlight(bool bHighlight=true) const;

  virtual
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX component_index) const;   // true if specified component is highlighted
  
  /*
  Description:
    Get a list of all highlighted sub-objects
  Parameters:
    component_indices - [out] component indices of highlighted sub-objects.
  Returns:
    number of highlighted subobjects
  */
  virtual
  int GetHighlightedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const;

  virtual
  bool HighlightSubObject(ON_COMPONENT_INDEX component_index,bool bHighlight=true) const;
  
  virtual
  int UnhighlightAllSubObjects(); // returns number of changed subobjects

  //////////
  // true if pixels used by highlighted object differ from pixels used
  // by highlighted object.
  virtual
  bool HighlightRequiresRedraw() const;


  // marking - no impact on display - not saved or copied
  // Use CRhinoDoc::ClearMarks() to set all marks to zero.
  int IsMarked() const;
  void Mark(int);

  /*
  Returns:
    CRhinoGripObject::GRIP_TYPE value
    0 = off
  See Also:
    CRhinoObject::EnableGrips
    CRhinoObject::GripsSelected
  */
  int GripsOn() const;


  /*
  Returns:
    If the object has grips on, this returns
    the enabler to turn on that type of grips.  If
    the object does not have grips on, this returns
    NULL.
  */
  const class CRhinoGripsEnabler* GripsEnabler() const;

  /*
  Returns:
    True if grips are turned on and at least one is selected.
  See Also:
    CRhinoObject::GripsOn
  */
  bool GripsSelected() const;

  /*
  Description:
    Turns on the object's default editing grips
  Parameters:
    bDefaultGripsOn - [in]
        True to turn on the object's default grips.
        False to turn off any grips.
  See Also:
    CRhinoObject::GripsOn
    CRhinoObject::GetGrips
  */
  virtual 
  void EnableGrips(
          bool bGripsOn
          );

  /*
  Description:
    Turns on/off the object's editing grips
  Parameters:
    grips - [in]
        This class must be on the heap.  The Rhino PointsOff
        command calls CRhinoObject::EnableGrips(0), which
        will delete grips.  If NULL, grips are turned off.
  Returns:
    True if the call succeeded.  If you attempt to add custom
    grips to an object that does not support custom grips,
    then false is returned and it is your responsibility to
    delete custom_grips.
  See Also:
    CRhinoObject::GripsOn
    CRhinoObject::GetGrips
  */
  virtual 
  bool EnableCustomGrips(
          class CRhinoObjectGrips* custom_grips
          );

  /*
  Description:
    If grips are enabled, then GetGrips appends the object's
    grips to the grip_list[] array.
  Returns:
    Number of grips appended to the array.
  See Also:
    CRhinoObject::GripsOn
    CRhinoObject::EnableGrips
  */
  virtual
  int GetGrips( ON_SimpleArray<class CRhinoGripObject*>& grip_list ) const;

  // managed by derived objects
  class CRhinoObjectGrips* m_grips;

  //////////////////////////////////////////////////////////////////////
  //
  // Rhino V6 object drawing function.
  //
  //  Ideally, DrawV6() will replace all the drawing functions below.
  //  The single parameter will indicate what should be drawn, where it
  //  is being drawn, and how it should appear.  If that happens, then
  //  this function will be renamed "Draw". 
  //
  //  For now it is called DrawV6() so it is easy to search for it.
  //
  //  The class CRhinoObjectDrawContext is defined in rhinoSdkDraw.h.
  //
  //  For now, you must include rhinoSdkDraw.h by hand.  Once the
  //  header file rhinoSdkDraw.h is being changed less than once
  //  a day, it will be included by rhinoSdk.h.
  //
  virtual
  void DrawV6(
    class CRhinoObjectDrawContext* draw_context
    ) const RHINO_NOEXCEPT;

    
  //////////
  // Draws the object's grips, control polygon, and dynamic
  // wireframe object.  Dynamic shaded objects are drawn in
  // the Draw
  virtual
  void DrawGrips(class CRhinoDisplayPipeline&) const;

  //////////
  // Draws object based on pipeline display attributes...
  virtual
  void Draw(class CRhinoDisplayPipeline&) const;

  virtual
  void PreDrawUpdate(CRhinoDisplayPipeline& dp) const;

  //////////
  // Draws wireframe representation of a sub object.
  virtual
  void DrawSubObject(class CRhinoDisplayPipeline&,ON_COMPONENT_INDEX) const;
  //////////
  // Draws wireframe representation of 
  // highlighted sub objects.
  // objects that support subobject highlighting must override 
  // this function
  virtual
  void DrawHighlightedSubObjects(class CRhinoDisplayPipeline&) const;

  /*
  Description:
    Low level tool for internal use only.  See definition
    of CRhinoObject::PickFilter for detailed information.
  Remarks:
    This function should not be used in Rhino plug-ins.
  */
  virtual
  bool PickFilter( const class CRhinoGetObject&, bool ) const;

  ON_DEPRECATED_MSG("use Draw function that takes a display pipeline")
  void Draw(CRhinoViewport&) const;

  /*
  Description:
    Expert level tool. Should not normally be used by Rhino plug-ins.
  Parameters:
    pick_list: [in/out] If object should be picked, it is appended to the pick_list
  Returns:
    Number of items added to pick_list.
  */
  virtual 
  int Pick( const CRhinoPickContext& pick_context, class CRhinoObjRefArray& pick_list ) const;

  /*
  Description:
    Low level tool for internal use only.  See definition
    of CRhinoObject::SnapTo for detailed information.
  Remarks:
    This function should not be used in Rhino plug-ins.
  */
  virtual
  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const;

  /*
  Description:
    Called when a user double clicks on this object. If the function returns a
    non-empty string, the string is interpreted as a command macro to execute.
  */
  virtual ON_wString OnDoubleClick(const CRhinoDoubleClickContext& double_click_context) const;

  //////////
  // returns a pointer to the object's geometry
  const ON_Geometry* Geometry() const;

  /*
  Description:
    Gets a guess at the initial object frame - this used to be the code that guesses
    the gumball location and orientation from the object geometry.

  Returns:
    A plane defining the object frame.

  Remarks:
    This is a "fake" virtual function - because I can't extend the SDK.  The implementation is to switch
    over the built in types and return a guessed frame.  If that doesn't work, it returns a CPlane aligned bounding box center
    assuming the cplane can be found for the current document view.  If that fails, a world plane BB center.
  */
  /*virtual*/
  ON_Plane InitialObjectFrame(const CRhinoDoc&) const;

  enum objectframe_flags : unsigned int
  {
    objectframe_standard = 0,                   //Never returns unset plane.  Will always calculate the plane even if nothing is stored.
    objectframe_include_scale_transforms = 1,   //By default, the plane is orientation and rotation only.  But scale transforms are stored if you need them.
    objectframe_return_unset = 2,               //The object frame may be stored unset by default.  If this is the case, and you want to know if it really is unset, this will tell you.
  };

  ON_Plane ObjectFrame(const ON_COMPONENT_INDEX& ci, unsigned int objectframe_flags) const;
  void SetObjectFrame(const ON_COMPONENT_INDEX& ci, const ON_Xform& wcs_to_ocs);
  void SetObjectFrame(const ON_COMPONENT_INDEX& ci, const ON_Plane& plane);

  /*
  Description:
    Gets 3d axis aligned bounding box that contains the object.

  Returns:
    3d bounding box

  Remarks:
    The default implementation uses m_geometry->BoundingBox(), 
    lazy evaluation, and caches the result in m_bbox.  
    If you override this function, then you must set m_bbox.
  */
  virtual
  ON_BoundingBox BoundingBox() const;

  /*
  Description:
    Gets 3d axis aligned world coordinate bounding box that 
    contains the geometry used to display the object.  
    For most objects, this is the same as BoundingBox().
    For objects like lights that use an icon, the world 
    coordinate bounding box depends on the view.

  Returns:
    3d bounding box

  Remarks:
    The default implementation calls BoundingBox().
  */
  virtual
  ON_BoundingBox BoundingBox( const class CRhinoViewport* pViewport ) const;


  /*
	Description:
    Get tight bounding box of this Rhino object.
	Parameters:
		tight_bbox - [in/out] tight bounding box
		bGrowBox -[in]	(default=false)			
      If true and the input tight_bbox is valid, then returned
      tight_bbox is the union of the input tight_bbox and the 
      tight bounding box of this Rhino object.
		xform -[in] (default=NULL)
      If not NULL, the tight bounding box of the transformed
      Rhino object is calculated.  The Rhino object is not modified.
	Returns:
    True if the returned tight_bbox is set to a valid 
    bounding box.
  */
  virtual
	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const;

  /*
  Description:
    Gets 3d frame for object.
  Parameters:
    plane - [out] 3d frame
  Returns:
    true if successful
  */
  virtual
  bool GetFrame( ON_Plane& plane ) const;

  /*
  Description:
    Used to change object attributes like name, layer, color,
    rendering material, etc.

  Parameters:
    new_attributes - [in] new object attributes. Values are copied.
    bEnableUndo - [in] if true, the attribute change can be undone
        by calling UndoModifyAttributes().
    bQuiet - [in] true to run quietly.

  Example:
    Change the layer of an object.

          int new_layer_index = ...;
          CRhinoObject& obj = ...;
          // copy attributes
          CRhinoObjectAttributes a = obj.Attributes();
          // change attribute settings
          a.SetLayerIndex( new_layer_index );
          // modify object's attributes
          obj.ModifyAttributes( a );

  Returns:
    true if successful. false if the layer index, material index, or
    any of the group indices are invalid.

  Remarks:
    ModifyAttributes() will not change the object's UUID.
    If doc.m_layer_table[new_attributes.LayerIndex()].IsDeleted() is 
    true, the layer is automatically undeleted.
    If doc.m_material_table[new_attributes.RenderMaterialIndex()].IsDeleted()
    is  true, the material is automatically undeleted.
  */
  virtual
  bool ModifyAttributes( 
         const ON_3dmObjectAttributes& new_attributes,
         bool bEnableUndo = true,
         bool bQuiet = false
         );

  bool UndoModifyAttributes( unsigned int ur_sn = 0 );

  /*
  Description:
    Object attributes include normal/locked/hidden status, name,
    layer, rendering attributes, display attributes, group membership, 
    and so on.

  Returns:
    Attributes

  Remarks:
    Some attributes like drawing color and rendering material
    can be specified per object or per layer.  The values of
    the attributes ColorSource() and MaterialSource() determine
    which definition should be used.

  See Also:
    CRhinoObject::ObjectLayer, CRhinoObject::ObjectMaterial,
    CRhinoObject::ObjectColor
  */
  virtual
  const CRhinoObjectAttributes& Attributes() const;

  CRhinoObjectAttributes& Attributes();

  /*
  Description:
    All objects are assigned to a layer.
  Returns:
    Layer this object is assigned to.

    NOTE WELL:
      This layer reference may become invalid if the
      layer table is modified in any way.  Do not use
      this pointer as a long term reference to a layer.
  See Also:
    CRhinoObject::Attributes
  */
  const class CRhinoLayer& ObjectLayer() const;
  const class CRhinoLayer* ObjectLayerPtr() const;

  /*
  Description:
    All objects reference a linetype.
  Returns:
    Linetype this object is references to. ( -1 == solid/continuous)

    NOTE WELL:
      This linetype reference may become invalid if the
      linetype table is modified in any way.  Do not use
      this pointer as a long term reference to a linetype.
  See Also:
    CRhinoObject::Attributes
  */
  const CRhinoLinetype& ObjectLinetype() const;
  const CRhinoLinetype* ObjectLinetypePtr() const;

  /*
  Description:
    Gets the material used to render the object's front side.
  Parameters:
    plugin_id - [in] (optional) If not present, the plugin_id
                     returned by RhinoApp().GetDefaultRenderApp()
                     is used.
  Returns:
    A reference to a rendering material.  When no material is
    explicitly specified, this is the default material 
    RhinoApp().AppSettings().DefaultMaterial().

    NOTE WELL: 
      If the material table is modified, this
      pointer will become invalid.  Do not use this pointer
      for saving long term reference to the material.  Use
      the material index or id instead.
  See Also
    CRhinoObjectAttributes::ObjectMaterialRef
    CRhinoMaterialTable::GetMaterials
    CRhinoObject::ObjectMaterialRef
  */
  virtual const CRhinoMaterial& ObjectMaterial() const;
  virtual const CRhinoMaterial& ObjectMaterial(const ON_UUID& plugin_id) const;
  virtual const CRhinoMaterial& ObjectMaterial(const ON_3dmObjectAttributes& attrs) const;

  //This is now the preferred version of the function.  This should be used to get materials component-wise in all cases.
  virtual const CRhinoMaterial& ObjectMaterial(const ON_COMPONENT_INDEX& ci, const ON_UUID& plugin_id = RhinoDefaultRendererId(), const ON_3dmObjectAttributes* attrs = NULL) const;

  //This version of ObjectMaterial returns the RDK material associated with an object if there is one.  It
  //directly corresponds to the function above that returns the material in the document material table.
  virtual const CRhRdkMaterial* ObjectRdkMaterial(const ON_COMPONENT_INDEX& ci, const ON_UUID& plugin_id = RhinoDefaultRendererId(), const ON_3dmObjectAttributes* attrs = NULL) const;


  virtual bool HasSubobjectMaterials(void) const;
  virtual int  GetSubobjectMaterialComponents(ON_SimpleArray<ON_COMPONENT_INDEX>& components_with_subobject_materials) const;
  //// OBSOLETE - REMOVE ALL CALLS TO THIS FUNCTION
  //ON_DEPRECATED bool SetTextureMapping() const;

  /// <summary>
  /// Determine if custom render meshes will be supplied for a particular object.
  /// </summary>
  /// <param name="vp">The viewport being rendered.  If nullptr, no support for view dependent meshes.</param>
  /// <param name="flags">See RhRdk::CustomRenderMeshes::IManager::Flags.  If nullptr, assumed to be Flags::None and no support for returning flags.</param>
  /// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.  Can be nullptr.</param>
  /// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.  Can be nullptr.</param>
  /// <returns>True if CustomMeshes will return a set of primitives, otherwise false.  CustomMeshes may still return an empty collection - in case of cancellation something similar.</returns>
  bool HasCustomRenderMeshes(
    ON::mesh_type mt,
    const ON_Viewport* vp = nullptr,
    ON__UINT32* flags = nullptr,
    const class CRhinoPlugIn* requestingPlugIn = nullptr,
    const class CDisplayPipelineAttributes* pAttributes = nullptr
  ) const;

  /// <summary>
  /// Returns a complete set of custom render meshes (primitives) for the object.
  /// </summary>
  /// <param name="vp">The viewport being rendered.  If nullptr, no support for view dependent meshes.</param>
  /// <param name="ancestry_only_for_by_parent_attributes">The ancestry of the object - ie, block references referring to this definition.  
  /// This is only used to handle ByParent attributes, and should not be used to transform geometry.  For objects in the document (ie - not block objects), it's fine to pass nullptr.</param>
  /// <param name="flags">See RhRdk::CustomRenderMeshes::IManager::Flags.  If nullptr, assumed to be Flags::None and no support for returning flags.</param>
  /// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
  /// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
  /// <returns>Returns a set of custom render meshes (primitives) for a given object.</returns>
  /// <param name="optional_initial_primitives">Optionally a custom set of initial primitives.  If this parameter is empty, the standard render meshes
  /// for the object will be used, or an empty set of primitives will be used where they are not available.  This is typically used in meshing exporters, which
  /// produce specific meshes for the output rather than using the render meshes.</param>
  /// <returns>Returns a complete set of custom render meshes (primitives) for a given ObjectId.</returns>
  std::shared_ptr<const RhRdk::CustomRenderMeshes::IRenderMeshes> RenderMeshes(
    ON::mesh_type mt,
    const ON_Viewport* vp = nullptr,
    const class CRhRdkObjectAncestry* ancestry_only_for_by_parent_attributes = nullptr,
    ON__UINT32* flags = nullptr,
    const CRhinoPlugIn* requestingPlugIn = nullptr,
    const CDisplayPipelineAttributes* pAttributes = nullptr,
    std::shared_ptr<const RhRdk::CustomRenderMeshes::IRenderMeshes> optional_initial_primitives = nullptr
  ) const;

  /// <summary>
  /// Returns a complete set of custom render meshes (primitives) for the object.  Simple version with no need for flags.
  /// </summary>
  /// <param name="vp">The viewport being rendered.  If nullptr, no support for view dependent meshes.</param>
  /// <param name="recursive">True if block instances should be unpacked.  Same as IManager::Flags::Recursive.</param>
  /// <param name="ancestry_only_for_by_parent_attributes">The ancestry of the object - ie, block references referring to this definition.  
  /// This is only used to handle ByParent attributes, and should not be used to transform geometry.  For objects in the document (ie - not block objects), it's fine to pass nullptr.</param>
  /// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
  /// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
  /// <returns>Returns a set of custom render meshes (primitives) for a given object.</returns>
  /// <param name="optional_initial_primitives">Optionally a custom set of initial primitives.  If this parameter is empty, the standard render meshes
  /// for the object will be used, or an empty set of primitives will be used where they are not available.  This is typically used in meshing exporters, which
  /// produce specific meshes for the output rather than using the render meshes.</param>
  /// <returns>Returns a complete set of custom render meshes (primitives) for a given ObjectId.</returns>
  std::shared_ptr<const RhRdk::CustomRenderMeshes::IRenderMeshes> RenderMeshes(
    ON::mesh_type mt,
    bool recursive,
    const ON_Viewport* vp = nullptr,
    const class CRhRdkObjectAncestry* ancestry_only_for_by_parent_attributes = nullptr,
    const CRhinoPlugIn* requestingPlugIn = nullptr,
    const CDisplayPipelineAttributes* pAttributes = nullptr,
    std::shared_ptr<const RhRdk::CustomRenderMeshes::IRenderMeshes> optional_initial_primitives = nullptr
  ) const;

  /*
  Description:
    If the object is a mesh or has existing render meshes,
    then this call uses mapping to set the texture 
    coordinates.
  Parameters:
    mapping - [in]
      Texture mapping function used to calculate the
      mesh's texture coordinates.
    object_xform - [in] (optional)
      Pass if the object has been transformed after the
      mapping was set.      
      For example, if a mapping M was used to calculate
      the object's texture coordinates, the object was
      subsequently transformed by T, and for some reason
      you needed to use M to recalculate the original texture 
      coordinates, then you would pass the T as object_xform.
    bLazy - [in]
      If true, then the calculation is performed only
      if the current mesh texture coordinates were
      calculated with a different mapping.
  Returns:
    True if the object's texture coordinates are set.
    False if the the coordinates cannot be set (the object
    may not have texture coordinates, like a point, or
    it may not have render mesh, like a new CRhinoBrepObject,
    or the mapping calculation may fail.

    The CRinoObject::Attributes()::m_rendering_attributes
    and CRhinoDoc::m_texture_mapping_table
    has tools to get texture mappings.  
  See Also:
    ON_TextureMapping::GetTextureCoordinates
    ON_Mesh::SetTextureCoordinates
    CRhinoObject::GetTextureMapping
  */
  virtual
  bool SetTextureCoordinates( 
         const ON_TextureMapping& mapping,
         const ON_Xform* object_xform,
         bool bLazy = true
         ) const;

  /*
  Description:
    If the object is a mesh or has existing render meshes,
    then this call uses mapping to set the cached texture 
    coordinates.
  Parameters:
    mapping - [in]
      Texture mapping function used to calculate the
      mesh's cached texture coordinates.
    object_xform - [in] (optional)
      Pass if the object has been transformed after the
      mapping was set.      
      For example, if a mapping M was used to calculate
      the object's texture coordinates, the object was
      subsequently transformed by T, and for some reason
      you needed to use M to recalculate the original texture 
      coordinates, then you would pass the T as object_xform.
    bLazy - [in]
      If true, then the calculation is performed only
      if the current mesh texture coordinates were
      calculated with a different mapping.
  Returns:
    True if the object's cached texture coordinates are set.
    False if the the cached coordinates cannot be set (the object
    may not have texture coordinates, like a point, or
    it may not have render mesh, like a new CRhinoBrepObject,
    or the mapping calculation may fail.

    The CRinoObject::Attributes()::m_rendering_attributes
    and CRhinoDoc::m_texture_mapping_table
    has tools to get texture mappings.  
  See Also:
    ON_TextureMapping::GetCachedTextureCoordinates
    ON_Mesh::SetCachedTextureCoordinates
    CRhinoObject::GetTextureMapping
  */
  bool SetCachedTextureCoordinates(
    const ON_TextureMapping& mapping,
    const ON_Xform* object_xform,
    bool bLazy = true
  ) const;

  virtual
  bool SetTextureCoordinatesFromPlugIn(
    const ON_UUID& plugin_id,
    int mapping_channel_id
    ) const; 

  /*
  Description:
   Set cached texture coordinates for all mappings that
   require per vertex texture texture coordinates.
   Includes mappings referenced by object material and
   sub-object materials.
  Parameters:
    plugin_id - [in]
      The id of the render plug-in to use to get the
      mapping ref.
  Returns:
    True if the object's cached texture coordinates are set.
    False if the the cached coordinates cannot be set (the object
    may not have texture coordinates, like a point, or
    it may not have render mesh, like a new CRhinoBrepObject,
    or the mapping calculation may fail.
  See Also:
    ON_TextureMapping::GetCachedTextureCoordinates
    ON_Mesh::SetCachedTextureCoordinates
    CRhinoObject::GetTextureMapping
  */
  bool SetCachedTextureCoordinatesFromPlugIn(
    const ON_UUID& plugin_id
  ) const;

  /*
  Description:
   Set cached texture coordinates for mappings listed
   in the array 'mappings_to_cache'.
   Includes mappings referenced by object material and
   sub-object materials.
  Parameters:
    plugin_id - [in]
      The id of the render plug-in to use to get the
      mapping ref.
    mappings_to_cache - [in]
      An array indicating which types of mappings that
      should be cached. If the array is empty, all
      found mappings will be cached.
  Returns:
    True if the object's cached texture coordinates are set.
    False if the the cached coordinates cannot be set (the object
    may not have texture coordinates, like a point, or
    it may not have render mesh, like a new CRhinoBrepObject,
    or the mapping calculation may fail.
  See Also:
    ON_TextureMapping::GetCachedTextureCoordinates
    ON_Mesh::SetCachedTextureCoordinates
    CRhinoObject::GetTextureMapping
  */
  bool SetCachedTextureCoordinatesFromPlugIn(
    const ON_UUID& plugin_id,
    const ON_SimpleArray<ON_TextureMapping::TYPE>& mappings_to_cache
  ) const;

  /*
  Description:
    Examines the attributes and gets the color that should be used
    to draw a wireframe image of this object.

  Parameters:
    bIgnoreObjectState - [in]
        Objects can be in one of 4 states: selected, normal, locked,
        and hidden. locked objects.  By default, selected objects are
        drawn in RhinoApp().AppSettings().SelectedObjectColor(), locked
        objects are drawn in RhinoApp().AppSettings().LockedObjectColor(),
        and normal objects are drawn in the color determined by the
        attributes settings.  If you want to ignore the object state and
        get the color determined by the attributes, the pass true.

  Returns:
    wireframe image drawing color

  Remarks:
    Objects and layers can have colors assigned to them
    The value of CRhinoObject::Attributes().ColorSource() 
    determines if the object's color or the object's layer
    color should be used to draw the object in a normal state.

  See Also:
  */
  ON_Color ObjectDrawColor( bool bIgnoreObjectState = false ) const;

  /*
  Description:
    Examines the attributes and gets the color that should be used
    to draw this object.

  Parameters:
    attributes - [in]
        Source for color information...normally color information
        is taken from the object's attributes, however, in this case
        all color info will be determined base on the passed in
        attributes object.
    bIgnoreObjectState - [in]
        Objects can be in one of 4 states: selected, normal, locked,
        and hidden. locked objects.  By default, selected objects are
        drawn in RhinoApp().AppSettings().SelectedObjectColor(), locked
        objects are drawn in RhinoApp().AppSettings().LockedObjectColor(),
        and normal objects are drawn in the color determined by the
        attributes settings.  If you want to ignore the object state and
        get the color determined by the attributes, the pass true.

  Returns:
    drawing color

  Remarks:
    Objects and layers can have colors assigned to them
    The value of attributes.ColorSource() 
    determines if the object's color or the object's layer
    color should be used to draw the object in a normal state.

  See Also:
  */
  ON_Color ObjectDrawColor( const CRhinoObjectAttributes& attributes, 
                            bool bIgnoreObjectState = false ) const;
  ON_Color ObjectDrawColor( 
          const CRhinoObjectAttributes& attributes, 
          const ON_UUID& viewport_id,
          bool bIgnoreObjectState 
          ) const;

  ///*
  //Description:
  //  Visual analysis modes that objects can support.
  //  edge - edges are shown in a user selected color
  //  curvature_graph - curve and surface curvature hair
  //  zebra - zebra stripe surface analysis.
  //  curvature - false color curvature surface analysis
  //              (Gaussian, mean, min, max)
  //  draftangle - false color draft angle analysis
  //  meshpreview - used internally when previewing analysis meshes
  //*/
  //enum ANALYSIS_MODE
  //{
  //  am_edge            =  1,
  //  am_curvature_graph =  2,
  //  am_zebra           =  4,
  //  am_emap            =  8,
  //  am_curvature       = 16,
  //  am_draftangle      = 32,
  //  am_meshpreview     = 64
  //};

  /*
  Description:
    Used to turn analysis modes on and off.
  Parameters:
    am - [in] specifies analysis mode
    bEnable - [in] true to turn mode on.
  Remarks:
    The zebra, Emap, curvature, and draft angle surface analysis
    modes are mutually exclusive.  Turning one on will 
    automatically turn off the  other three.  
    The edge and curvature_graph modes are independent
    of all other modes.
  Returns:
    True if this object supports the analysis mode.
  */
  //virtual
  //bool EnableAnalysisMode( ANALYSIS_MODE am, bool bEnable = true ) const; // old way
  bool EnableAnalysisMode( ON_UUID am_id, bool bEnable = true ) const; // new way (not virtual)

  /*
  Description:
    Reports if an analysis mode is currently enabled
    for an object
  Parameters:
    am - [in]
  Returns:
    True if the analysis display mode is currently enabled.
  */
  //bool AnalysisMode( ANALYSIS_MODE am ) const; // old way

  bool InAnalysisMode( const ON_UUID& am_id ) const;    // new way

  /*
  Returns:
    True if any visual analysis mode is active
  */
  bool InAnalysisMode() const;

  /*
  Description:
    An object can have at most one shaded analysis mode
    active at any time. It is possible to have multiple
    wireframe modes active.  For example, you can have
    zebra, edge, and curvature hair analysis on at the
    same time, but you cannot have zebra and draft angle
    on at the same time.
  Returns:
    If the object has a shaded analysis mode on, a pointer
    to the mode's controller is returned.  For any mode,
    this pointer is always the same and is valid for the
    entire life of the application.
  */
  const class CRhinoVisualAnalysisMode* InShadedAnalysisMode() const;

  // list of enabled analysis modes
  const class CRhinoObjectVisualAnalysisMode* m_analysis_mode_list;

  /*
  Description:
    Localized short description of object like "curve", "surface",
    "mesh", etc.).
  Parameters:
    bPlural - [in]
        If true, then a plural, like "curves", "surfaces", etc.,
        is returned.
  Returns:
    NULL terminated string.
  */
  virtual
  const wchar_t* ShortDescription( bool bPlural ) const = 0;

  /*
  Description:
    Gets the history record that describes how this object
    was created.  This information is used to update this
    object when Rhino history is enabled and an input
    object changes.
  Returns:
    NULL or a pointer to the history record for this object.
  */
  const CRhinoHistoryRecord* HistoryRecord() const;

  bool SetHistory( class CRhinoHistory& history );


  /*
  Description:
    Sets a flag on the object's history record (if one exists) that causes the history record to be
    copied and the copy applied to the new object when this object is replaced in the document.
    That allows some changes to be made to objects with history without breaking the history.
  */
  void SetCopyHistoryOnReplace(bool bCopy) const;

  /*
  Description:
    Returns whether the CopyHistoryOnReplace flag is set.
  Returns:
    true if the object has history and the flag is set
    false if there's no history or the flag is clear
  */
  bool CopyHistoryOnReplace() const;

private:
  unsigned int m__reserved0;
  unsigned int m__reserved1;

private:
  // m_linked_idef_sn
  //        0: Not from a linked idef layer table
  //   1-1000: reserved for future use
  //    >1000: instance definition serial number
  unsigned int m_linked_idef_sn;

  // m_worksession_ref_model_sn
  //        0: layer is in active model
  //        1: layer is in an unidentified reference model
  //   2-1000: reserved for future use
  //    >1000: worksession reference model serial number
  unsigned int m_worksession_ref_model_sn;

public:
  ///////////////////////////////////////////////////////////////////
  //
  // Render and Analysis mesh interface
  //

  // Returns true if the object is capable of having a mesh of the specified type.
  virtual
  bool IsMeshable( ON::mesh_type) const;

  void GetRenderMeshParameters( ON_MeshParameters& mp ) const;

  virtual
  bool SetRenderMeshParameters( const ON_MeshParameters& mp );

  /*
  Description:
    CRhinoHeaders can have several different types of meshes and 
    different numbers of meshes.  A b-rep can have a render and 
    an analysis mesh on each face.  A mesh object has a single 
    render mesh and no analysis mesh.  Curve, point, and annotation
    objects have no meshes.
  Parameters:
    mesh_type - [in] type of mesh to count
    mp - [in] if not NULL and if the object can change its mesh
              (like a brep), then only meshes that were created
              with these mesh parameters are counted.
  Returns:
    number of meshes.
  */
  virtual
  int MeshCount( 
        ON::mesh_type mesh_type,
        const ON_MeshParameters* mp = 0
        ) const;

  /*
  Description:
    Create meshes used to render and analyze surface and polysurface
    objects.
  Parameters:
    mesh_type - [in] type of meshes to create
    mp - [in] in parameters that control the quality of the meshes
              that are created.
    mapping - [in]
      If not NULL, the texture coordinates are calculated using the call
      ON_Mesh::SetTextureCoordinates(*mapping,mapping,true)
    mapping_xform - [in]
      See mapping parameter description.
  See Also:
    CRhinoObject::GetMeshes
    CRhinoObject::MeshCount
    CRhinoObject::IsMeshable
  */
  virtual
  int CreateMeshes(ON::mesh_type mesh_type, const ON_MeshParameters& mp, bool bIgnoreCustom=false );
  
  /*
  Description:
    Destroy meshes used to render and analyze surface and polysurface
    objects.
  Parameters:
    mesh_type - [in] type of mesh to destroy
    bDeleteMeshes - [in] if true, cached meshes are deleted.
      If false, pointers to cached meshes are just set to NULL.
  See Also:
    CRhinoObject::GetMeshes
    CRhinoObject::MeshCount
    CRhinoObject::IsMeshable
  */
  ON_DEPRECATED_MSG("bDeleteMeshes = false is no longer supported - override this function if you are implementing CRhinoObject, but ignore the bDeleteMeshes parameter.  Call the function below.") 
  virtual void DestroyMeshes( ON::mesh_type mesh_type, bool bDeleteMeshes );

  /*virtual*/ void DestroyMeshes(ON::mesh_type mesh_type);

  virtual
  bool PromotePreviewMeshes( ON::mesh_type );

  /*
  Description:
    Get existing meshes used to render and analyze surface and polysurface
    objects.
  Parameters:
    ON::mesh_type - [in] type of mesh to get
    meshes - [out] meshes are appended to this array.  The CRhinoObject
      owns these meshes so the meshes cannot be modified.
  Returns: 
    Number of meshes added to meshes array
  See Also:
    CRhinoObject::DestroyMeshes
    CRhinoObject::MeshCount
    CRhinoObject::IsMeshable
  */
  virtual
  int GetMeshes( 
          ON::mesh_type mesh_type, 
          ON_SimpleArray<const ON_Mesh *>& meshes 
          ) const;

  /*
  Description:
    Get existing meshes used to render and analyze surface and polysurface
    objects.
  Parameters:
    ON::mesh_type - [in] type of mesh to get
  Returns:
    Shared pointers to const meshes as stored on the objects.
  See Also:
    CRhinoObject::DestroyMeshes
    CRhinoObject::MeshCount
    CRhinoObject::IsMeshable
  */

  /*virtual*/
  std::vector<std::shared_ptr<const ON_Mesh>> GetMeshes(ON::mesh_type mesh_type) const;

  /*
  Description:
    When Rhino draws the meshes returned by GetMeshes(), it gives
    you the opportunity to modify the display attributes for that
    mesh.
  Parameters:
    mesh_index - [in]
      index of the mesh in the meshes[] array returned by the
      GetMeshes() function.
    mesh - [in]
      The mesh in meshes[mesh_index].
    display_attrs - [in/out]
      Display attributes that will be used to draw this mesh.
      The input values are what Rhino normally uses.
      You may modify them as needed.
  See Also:
    CRhinoObject::GetMeshes
  */
  virtual
  const class CDisplayPipelineAttributes* MeshDisplayAttributes( 
          int mesh_index, 
          const ON_Mesh& mesh,
          const class CDisplayPipelineAttributes& display_attrs
          ) const;

  /*
  Description:
    Get curves that are used to display the object's wireframe.
  Parameters:
    curve_list - [in] 3d world space wireframe curves are appended 
      to this list.  The caller is responsible for deleting the curves.
  Returns:
    Number of curves appended to the list.
  */
  virtual
  int GetWireframeCurves( ON_SimpleArray<ON_Curve*>& curve_list ) const;

  // Returns true if object is a closed solid otherwise false.  GetMeshes may
  // return more than one mesh to represent a solid object.
  virtual
  bool IsSolid() const;

  // updates CRhinoObject.m_bbox member.
  // This function calls BoundingBox() with no viewport so 
  // it will get the bounding box wrong on annotations
  void UpdateBoundingBox();
  
  /////////////////////////////////////////////////////////////////
  //
  // dynamic transformation commands
  //

  /*
  Description:
    While an object is being dynamically transformed (dragged, rotated, ...),
    the current transformation can be retrieved and used for creating
    dynamic display.
  Properties:
    xform - [out]
  Returns:
    True if the object is being edited and its transformation
    is available.  False if the object is not being edited,
    in which case the identity xform is returned.
  */
  bool GetDynamicTransform( ON_Xform& dynamic_xform ) const;

  /*
  Returns:
    True if the object has a dynamic transformation.
  */
  bool HasDynamicTransform() const;

  /*
  Description:
    Set the transformation currently being used.
  Properties:
    xform - [in]
  Returns:
    True if the xform is a valid and was set.
  */
  bool SetDynamicTransform( const ON_Xform& dynamic_xform );

  /*
  Description:
    Delete the dynamic transformation.
  */
  void DeleteDynamicTransform();


  /////////////////////////////////////////////////////////////////
  //
  // transformation callbacks
  //
  /*
  Description:
    Expert user tool 
    Called to determine if the object supports transformation in this case.
	The default implementation to returns true.  MappingWidgets return false.
  */
  virtual bool VetoTransformObject(CRhinoDoc& doc, bool bDeleteOriginal, bool bAddNewObjectToDoc, bool bAddTransformHistory) const;

  /*
  Description:
    Expert user tool 
    Called immediately after duplication of the transformed object, and before changes are made to it.
  */
  virtual void BeginTransform(CRhinoDoc& doc, bool bDeleteOriginal, bool bAddNewObjectToDoc, bool bAddTransformHistory);

  /*
  Description:
    Expert user tool 
    Called at the end of the transformation process. Default implementation doesn't do anything
  */
  virtual void EndTransform(CRhinoDoc& doc, bool bDeleteOriginal, bool bAddNewObjectToDoc, bool bAddTransformHistory);

  /////////////////////////////////////////////////////////////////
  //
  // user data

  /*
  Description:
    Attach user data to the Rhino object's geometry.  This
    user data will persist through file IO if the user data
    correctly implements file IO support.
  Parameters:
    pUserData - [in] user data to attach to object.
        The ON_UserData pointer passed to AttachUserData()
        must be created with new.
  Returns:
    If TRUE is returned, then ON_Object will delete the user
    data when appropriate.  If FALSE is returned, then data 
    could not be attached and caller must delete it.  
  Remarks:
    AttachGeometryUserData() will fail if the user data's 
    m_userdata_uuid field is nil or not unique.
  */
  bool AttachGeometryUserData( 
          ON_UserData* pUserData 
          );

  /*
  Description:
    Remove user data from a Rhino object's geometry.
  Parameters:
    pUserData - [in] user data to attach to object.
        The ON_UserData pointer passed to DetachUserData()
        must have been previously attached using 
        AttachUserData().  
  Returns:
    If TRUE is returned, then the user data was
    attached to this object and it was detached.  If FALSE
    is returned, then the user data was not attached to this
    object to begin with.  In all cases, you can be assured
    that the user data is no longer attached to "this".
  Remarks:
    Call delete pUserData if you want to destroy the user data.
  */
  bool DeleteGeometryUserData(
          ON_UUID userdata_uuid
          );

  /*
  Description:
    Get a pointer to user data on a Rhino object's geometry.
  Parameters:
    userdata_uuid - [in] value of the user data's 
       m_userdata_uuid field.
  Remarks:
    The returned user data is still attached to the object.
    Deleting the returned user data will automatically remove
    the user data from the object.
  */
  ON_UserData* GetGeometryUserData( 
          ON_UUID userdata_uuid
          ) const;

  /*
  Description:
    Attach user data to the Rhino object's attributes.
  Parameters:
    pUserData - [in] user data to attach to object's attributes.
        The ON_UserData pointer passed to AttachUserData()
        must be created with new.    If successful,
        the returned value of pUserData is NULL.
  Returns:
    If TRUE is returned, then ON_Object will delete the user
    data when appropriate.  If FALSE is returned, then data 
    could not be attached and caller must delete.  
  Remarks:
    AttachUserData() will fail if the user data's m_userdata_uuid
    field is nil or not unique.
  */
  BOOL32 AttachAttributeUserData( 
          ON_UserData* pUserData 
          );

  /*
  Description:
    Remove user data from a Rhino object's attributes.
  Parameters:
    userdata_uuid - [in] value of the user data's 
       m_userdata_uuid field.
  Returns:
    If TRUE is returned, then the user data was
    attached to this object and it was detached.  If FALSE
    is returned, then the user data was not attached to this
    object to begin with.  In all cases, you can be assured
    that the user data is no longer attached to "this".
  Remarks:
    Call delete pUserData if you want to destroy the user data.
  */
  BOOL32 DeleteAttributeUserData(
          ON_UUID userdata_uuid 
          );

  /*
  Description:
    Get a pointer to user data on a Rhino object's attributes.
  Parameters:
    userdata_uuid - [in] value of the user data's 
       m_userdata_uuid field.
  Remarks:
    The returned user data is still attached to the object.
    Deleting the returned user data will automatically remove
    the user data from the object.
  */
  ON_UserData* GetAttributeUserData( 
          ON_UUID userdata_uuid
          ) const;


  /*
  Description:
    Explode this object into subobjects.
  Parameters:
    subObjects - [out]
      subobjects are appended to this list.  The returned
      objects have not been added to the document and it
      is the caller's responsibility to manage them.
    pViewport - [in]
      Some objects' appearance is viewport dependent.  If
      not NULL, the object should be exploded with respect
      to its appearance in this viewport.
  Returns:
    Number of subobjects ( >= 2 ) or 0 if this
    object cannot be exploded.
  */
  virtual
  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const class CRhinoViewport* pViewport = NULL
          ) const;

  // returns number of bytes used by this particular instance
  virtual unsigned int SizeOf() const override;

  /*
  Description:
    Called before the object is saved to a 3dm archive
    so that the object pointed to by m_geometry can
    prepared for writing.
  Parameters:
    archive_3dm_version - [in]
      Version of the 3dm archive 2 = Rhino2, 3 = Rhino3, 4 = Rhino4.
  Returns:
    True if m_geometry should be saved.  False otherwise.
  */
  virtual
  bool PrepareToWrite( int archive_3dm_version );

#if defined (ON_RUNTIME_APPLE) && !defined(ON_RUNTIME_APPLE_IOS)
  id ObjectProxy() const;
#endif
  
public:
  mutable class CRhObjectSdkExtension* m__object_sdk_extension;

protected:
  friend class CRhinoRead3dmHelper;
  friend class CRhinoHistoryManager;
  friend class CRhinoHistoryRecordTable;
  friend class CRhObjectSdkExtension;

  virtual int SubObjectsAreSelected() const; // returns number of selected subobjects
  virtual int SubObjectsAreHighlighted() const; // returns number of highlighted subobjects
  virtual bool IsSelectableWithGripsOn() const; // true if can be selected while grips are on
  void Destroy();

  ON_Geometry* m_geometry;
  
  // world coordinate bounding box
  ON_BoundingBox m_bbox; // world coordinate bounding box

  // History record table entry for this object.  The
  // CRhinoDoc::m_history_record_table[] manages the
  // history record.  It is common for this pointer
  // to be NULL, in which case the object has no
  // construction history.
  const CRhinoHistoryRecord* m_history_record;

private:
  static std::atomic<unsigned int> Internal_CRhinoObjectRuntimeSerialNumberGenerator;

  void Create();
  void Copy(const CRhinoObject&);

  // used to determine if geometry is in document
  friend class CRhinoObjectGrips;
  friend class CRhinoGripObject;
  friend class CRhCameraIcon;
  friend class CRhinoDoc;
  friend class CRhinoLightTable;
  friend class CRhinoLight;
  friend class CRhObjectFinder;  
  friend class CRhinoObjectIterator;  
  friend class CRhinoProxyObject;  
  friend class CRhinoPhantomObject;  
  friend class CRhinoInstanceDefinitionTable;
  friend class CRhinoInstanceDefinition;
  friend class CRhSnapToVisible;
  friend class CRhRenderPreview;
  CRhinoObject* m_next;
  CRhinoObject* m_prev;
  class CRhinoDoc* m_doc;

private:
  // Ideally, CRhinoObject would be derived from
  // ON_ModelComponent and its name, id, serial number
  // would be managed by ON_ModelComponent like every
  // other persistent Rhino model component 
  // (layer, dimstyles, line patterns, render materials, ...)
  // However, the amount of work to move those core model
  // component properties from ON_3dmObjectAttributes to
  // this object itself and the number of 3rd party plug-ins
  // such a change would break is too much for Rhino V6.
  // So this mutable component is on as a private member
  // and its id and name are kept in sync with the
  // values on m_attributes whenever the 
  // ModelComponent() function is called;
  mutable ON_ModelComponent m_proxy_model_component;

public:
  const ON_ModelComponent& ModelComponent() const;

  // Code that uses CRhinoObject.Id() now will not need to be modified
  // when m_uuidis removed from ON_3dmObjectAttributes.
  const ON_UUID Id() const;

  // Code that uses CRhinoObject.Name() now will not need to be modified
  // when m_name is removed from ON_3dmObjectAttributes.
  const ON_wString Name() const;

private:
#if defined (ON_RUNTIME_APPLE) && !defined(ON_RUNTIME_APPLE_IOS)
  id m_object_proxy;       // RhinoObjectProxy object
  
  void DeleteObjectProxy();
  void CreateObjectProxy();
#endif
  
protected:
  // light objects are special because they are not in the geometry lists
  virtual void SynchAttributes();



public:
  // When an object like a CRhinoBrepObject is being meshed and
  // the display code should use the preview mesh instead of
  // the render or analysis mesh, then this flag is set to 1.
  // Otherwise it is set to zero.
  unsigned char m_use_preview_mesh;

  // if m_no_show_mark!= 0 and m_no_show_mark == m_mark,
  // then the IsVisible() returns false.  This is used
  // when inserting files.
  static int m_no_show_mark;

  mutable CRhinoCacheHandle m_cache;

protected:
  unsigned char m_geomem_type;    // 0 = m_geometry pointer managed with new/delete
                                  // 1 = m_geometry points to a class member.

private:
  // m_is_selected records if the entire object is selected,
  // and if it stays selected when commands end.  If m_is_selected
  // is not zero, the entire object is selected.  If m_is_selected==2,
  // then the object stays selected when commands end.
  // Being selected is independent of being highlighted 
  // and/or having grips on.  However, in general selected
  // objects are highlighted.  If m_is_selected is zero,
  // it is possible that some subset is selected.  Call
  // SubObjectsAreSelected() to find out if subobjects are selected.
  unsigned char m_is_selected;    // 0 = no, 1 = yes, 2 = persistent

  unsigned char m_is_deleted;     // 0 = no, 1 = yes
  unsigned char m_is_highlighted; // 0 = not highlighted, 1 = highlighted
  unsigned char m_is_proxy;       // 0 = normal (saved in files),
                                  // 1 = proxy object (not saved in files) for a render light, group, camera, etc.
                                  // 2 = phantom object (not saved in files)
  unsigned char m_runtime_flags;  // bits used to record various runtime states
                                  // bNoDeleteOnPurge = (0 != (0x01 & m_runtime_flags))
                                  //   If bNoDeleteOnPurge, then do not call delete in CRhinoDoc::PurgeObject()
                                  // bHasDynamicXform = (0 != (0x02 & m_runtime_flags))
                                  //  If bHasDynamicXform, then GetDynamicXform() will get an xform.  This bit is used
                                  //  to make GetDynamicXform() instantly return false.
                                  // bObjectIsBad = (0 != (0x04 & m_runtime_flags))
                                  //  If bObjectIsBad is true, the object did not pass validity
                                  //  check when it was added to document.

  unsigned char m_is_idef_geometry; // 0 = no, 
                                    // >=1: The object is in the CRhinoDoc::m_active_model_idef_geometry
                                    //      or the CRhinoDoc::m_active_model_idef_geometry list.
                                    // 1: means the object is not used by any CRhinoInstanceDefinition.
                                    // 2-254: means the object is used by (m_is_idef_geometry-1)
                                    //        CRhinoInstanceDefinition classes.
                                    // 255: means the object is used by more that 253
                                    //      CRhinoInstanceDefinition classes
                                    // Generally this number is 0 or 2.
  unsigned char m__reserved2;

  int m_mark;
  CRhinoObjectAttributes m_attributes;
  class CRhObjectAttributeUndo* m_attribute_undo; // used to undo attribute changes


public:
  /*
  Description:
  If you are writing a command or plug-in and and need
  a brep form of an object type that supports BrepForm, call Geometry()->BrepForm().

  The definition of CRhReferenceCountedBrep is not available
  in the SDK and this function is for use in the core Rhino
  code. The returned class must be deleted by the caller.
  */
  class CRhObjectReferenceCounter* BrepReferenceCounter(void) const;
  bool BrepReferenceCounterExists() const;
  unsigned int BrepReferenceCount() const;
  void BrepReferenceCounter_DestroyRuntimeCache(bool bDelete) const;

  virtual bool SupportsBrepProxyReferenceCounting() const;

private:
  // Used to manage a reference counted ON_Brep form of the
  // extrusion when one is needed for selection purposes.
  mutable class CRhObjectReferenceCounter* m_brep_reference_counter = nullptr;
  static void DeleteReferencedBrepCallback(const class ON_ObjectReferenceCounter* rco, void* ptr);

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

/*
Description:
  A picture frame object is an object that displays a texture
  map in all views.
Parameters:
  rhino_object - [in]
Returns:
  True if rhino_object is a picture frame object.
*/
RHINO_SDK_FUNCTION
bool RhinoObjectIsPictureFrame( const CRhinoObject* rhino_object );

/*
Description:
  When the brep behind a picture frame object is trimmed/split and the 
  underlying surface is shrunk, a new material needs to be assigned
  that will display the appropriate subset of the original picture image.
Parameters:
  doc - [in]
  original_picture_object - [in]
  shrunk_picture_brep - [in]
Returns:
  Index of new material or ON_UNSET_INT_INDEX if the input is not valid.
*/
RHINO_SDK_FUNCTION
int RhinoCreateSubPictureMaterialIndex(
  CRhinoDoc& doc,
  const CRhinoObject* original_picture_object,
  const ON_Brep* shrunk_picture_brep
);

/*
Description:
  Creates a PictureFrame object from a plane (or a mesh, if makemesh is true) and a path to an image file, 
   and adds it to the specified document. Note, a PictureFrame object is just a Plane surface or mesh that 
   has a material with a texture and assigned to it that displays in all display modes. If, for some reason, 
   you need access the PictureFrame's underlying geometry, then just cast the object's geometry as an 
   ON_Brep or an ON_Mesh. For example:
 
   const CRhinoObject* pPF = RhinoCreatePictureFrame(...);
   if (pPF)
   {
     const ON_Brep* pB = ON_Brep::Cast(pPF->Geometry());
     if (pB)
     {
       // TODO...
     }
   }
 
   You can also cast the returned CRhinoObject* as a CRhinoBrepObject* or a CRhinoMeshObject*.

Parameters:
  doc              - [in]  Document to which the PictureFrame will be added.
  plane            - [in]  Plane in which the PictureFrame will be created.  Bottom left 
                            corner of picture will be at plane's origin, width will be in 
                            the plane's x axis direction, height will be in the plane's 
                            y axis direction.
  texturepath      - [in]  Path to an image file.  File must exist if either width or height 
                            are 0.0 or return value will be null.  
  makemesh         - [in]  If true, the function will make a PictureFrame object from a mesh
                            rather than a plane.
                            The default is false.
  width            - [in]  Width of the resulting PictureFrame. If 0.0, the width of the 
                            picture frame is the width of the image if height is also 0.0 or 
                            calculated from the height and aspect ratio of the image if height 
                            is not 0.0.  
                            The default is 0.0.
  height           - [in]  Height of the resulting PictureFrame. If 0.0, the height of the 
                            picture frame is the height of the image if width is also 0.0 or 
                            calculated from the width and aspect ratio of the image if width 
                            is not 0.0.  
                            The default is 0.0.
  selfillumination - [in]  If true, the image mapped to the picture frame plane always displays 
                            at full intensity and is not affected by light or shadow.  
                            The default is true.
  embedbitmap      - [in]  If true, the function adds the the image to the bitmap table of the 
                            document to which the PictureFrame will be added. 
                            The default is false.
  addtodoc         - [in]  If false, the object will not be added to the document.  The material
                            for the PictureFrame will be added regardless and the material index
                            in the attributes will be an index into doc's material table.
                            The default is true.
  usealphaforobjecttransparency - [in]  If false, the picture frame is created without any transparency texture,
										as in all versions of Rhino previous to Rhino 5 SR2.  If true, a transparency texture
										is created with a "Mask Texture" set to alpha, and an instance of the diffuse texture in
										the source texture slot.  Not that when this flag is set, a full RDK material is created.

Returns:
  A pointer to a const CRhinoObject in the active document or null if the function fails for any
  reason.
*/
RHINO_SDK_FUNCTION
const CRhinoObject* RhinoCreatePictureFrame(
        class CRhinoDoc& doc,
        const ON_Plane& plane, 
        const ON_wString& texturepath,
        bool makemesh = false,
        bool usealphaforobjecttransparency = false,
        double width = 0.0, 
        double height = 0.0, 
        bool selfillumination = true, 
        bool embedbitmap = false,
        bool addtodoc = true
        );

/*
Description:
  This function is the predecessor to the function above and is the same in every regard except for these 
   things.  It does not have the argument "usealphaforobjecttransparency" to determine whether the function should create
   an automatic transparency texture with an alpha mask..
*/
RHINO_SDK_FUNCTION
const CRhinoObject* RhinoCreatePictureFrame(
        class CRhinoDoc& doc,
        const ON_Plane& plane, 
        const ON_wString& texturepath, 
        bool makemesh = false,
        double width = 0.0, 
        double height = 0.0, 
        bool selfillumination = true, 
        bool embedbitmap = false,
        bool addtodoc = true
        );

/*
Description:
  This function is the predecessor to the function above and is the same in every regard except for these 
   things.  It does not have the argument "makemesh" to determine whether the function should create
   a CRhinoMeshObject or a CRhinoBrepObject.  The CRhinoObject it returns may only be cast as a
   CRhinoBrepObject.
*/
RHINO_SDK_FUNCTION
const CRhinoObject* RhinoCreatePictureFrame(
        class CRhinoDoc& doc,
        const ON_Plane& plane, 
        const ON_wString& texturepath, 
        double width = 0.0, 
        double height = 0.0, 
        bool selfillumination = true, 
        bool embedbitmap = false,
        bool addtodoc = true
        );

/*
Description:
  A function to help CRhinoPhantomObject support snapping.
Parameters:
  object - [in]
  nurbs_curve - [in]
  snap_context - [in]
  snap_event - [in]
Example:
  Add snapping support to a widget.

          class CMyWidget : public CRhinoPhantomObject
          {
            ...
            bool SnapTo( 
              const CRhinoSnapContext& snap_context,
              CRhinoSnapEvent& snap_event
              ) const;
            ...
          };

          bool CMyWidget::SnapTo( 
              const CRhinoSnapContext& snap_context,
              CRhinoSnapEvent& snap_event
              ) const
          {
            // Get a NURBS curve representation of the wire objects
            // to snap to.
            ON_NurbsCurve nurbs_curve = ...; 
            return RhinoSnapToNurbsCurve(this,nurbs_curve,snap_context,snap_event);
          }
*/
RHINO_SDK_FUNCTION
        bool RhinoSnapToNurbsCurve(
        const CRhinoObject* object, 
        const ON_NurbsCurve& nurbs_curve,
        const CRhinoSnapContext& snap_context,
        CRhinoSnapEvent& snap_event 
        );


class RHINO_SDK_CLASS CRhinoPhantomObject : public CRhinoObject
{
  // CRhinoPhantomObject acts like an ordinary CRhinoObject
  // but is NEVER saved in file.
  ON_OBJECT_DECLARE(CRhinoPhantomObject);
public:

  // virtual CRhinoObject override returns ON::phantom_object
  ON::object_type ObjectType() const override;

  // virtual CRhinoObject override returns ON::phantom_object
  const wchar_t* ShortDescription( bool bPlural ) const override;

  CRhinoPhantomObject();                              // assigns a new object UUID
  CRhinoPhantomObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  virtual ~CRhinoPhantomObject();
  CRhinoPhantomObject( const CRhinoPhantomObject& );          // copies source object UUID
  CRhinoPhantomObject& operator=(const CRhinoPhantomObject&); // copies source object UUID
};


class RHINO_SDK_CLASS CRhinoProxyObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoProxyObject);
public:
  CRhinoProxyObject();                              // assigns a new object UUID
  CRhinoProxyObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoProxyObject();
  CRhinoProxyObject( const CRhinoProxyObject& );          // copies source object UUID
  CRhinoProxyObject& operator=(const CRhinoProxyObject&); // copies source object UUID

  /*
  Description:
    Overrides CRhinoObject::DuplicateRhinoObject() and returns
    a "real" Rhino object like a CRhinoCurveObject, CRhinoPointObject,
    and so on.  If no "real" object can be returned, then
    NULL is returned.
  Returns:
    NULL or a pointer to the duplicate.  The new object is
    not added to the Rhino document.
  */
  CRhinoObject* DuplicateRhinoObject() const override;

  // overrides CRhinoObject pure virtual and returns
  // m_geometry->ObjectType();
  ON::object_type ObjectType() const override;


  // overrides CRhinoObject pure virtual and returns
  // "polyedges", "polyedge", "proxies", or "proxy"
  // depending on what m_geometry points to.
  const wchar_t* ShortDescription( bool bPlural ) const override;

  /*
  Description:
    Use this to hang a piece of geometry off the rhino proxy object.
  Parameters:
    geometry - [in] geometry.  Pass a NULL to clear existing
                    geometry.  If geometry is NULL, any existing
                    m_geometry will not be deleted, even if
                    m_bDeleteGeometry is true. 
    bDeleteGeometry - [in]  If true, then ~CRhinoProxyObject()
                    will delete geometry.
  */
  void SetGeometry( ON_Geometry* geometry, bool bDeleteGeometry );

  /*
  Returns:
    The value of m_bDeleteGeometry.  If this is true, then
    ~CRhinoProxyObject() will delete the geometry.
  */
  bool DeleteGeometry() const;

private:
  bool m_bDeleteGeometry; // true if ~CRhinoProxyObject should delete the m_geometry
                        // object.

   
};

class CRhCamera;
class CRhNamedViewCamera;

class RHINO_SDK_CLASS CRhinoNamedViewCameraIcon : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoNamedViewCameraIcon);

public:
  CRhinoNamedViewCameraIcon(const wchar_t* wszAssociatedViewName = nullptr);
  virtual ~CRhinoNamedViewCameraIcon();

  CRhinoNamedViewCameraIcon(const CRhinoNamedViewCameraIcon& src);

  const CRhinoNamedViewCameraIcon& operator = (const CRhinoNamedViewCameraIcon&);

  CRhCamera& Camera(void);
  const CRhCamera& Camera(void) const;
  void SetCamera(const CRhCamera& c);

  CRhNamedViewCamera& NamedViewCamera(void);
  const CRhNamedViewCamera& NamedViewCamera(void) const;

  ON_wString AssociatedViewName(void) const;

  void Set3dmViewLocked(bool b);

  //Overrides
  virtual ON::object_type ObjectType() const override;
  virtual void EnableGrips(bool bEnable) override;
  virtual ON_BoundingBox BoundingBox() const override;
  virtual ON_BoundingBox BoundingBox(const CRhinoViewport*) const override;
  virtual void Draw(CRhinoDisplayPipeline& dp) const override;
  virtual bool PickFilter(const CRhinoGetObject& go, bool bCheckSubObjects) const override { return true; }
  virtual int Pick(const CRhinoPickContext&, CRhinoObjRefArray&) const override;
  virtual bool SnapTo(const CRhinoSnapContext& snap_context, CRhinoSnapEvent& snap_event) const override;
  virtual const wchar_t* ShortDescription(bool bPlural) const override;
  virtual void DeleteFromDocNotification() override;
  virtual bool VetoTransformObject(CRhinoDoc& /*doc*/, bool bDeleteOriginal, bool /*bAddNewObjectToDoc*/, bool /*bAddTransformHistory*/) const override;
  virtual void BeginTransform(CRhinoDoc& doc, bool bDeleteOriginal, bool bAddNewObjectToDoc, bool bAddTransformHistory) override;
  virtual void EndTransform(CRhinoDoc& doc, bool bDeleteOriginal, bool bAddNewObjectToDoc, bool bAddTransformHistory) override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

public:
  class Impl;
  std::unique_ptr<Impl> m_pImpl;
  friend class Impl;
};

//////////////////////////////////////////////////////////////////////////
//
// CRhinoObjRef is used to store references to objects returned by 
// CRhinoGetPoint and CRhinoGetObject
//
class RHINO_SDK_CLASS CRhinoObjRef : public ON_ObjRef
{
public:
  CRhinoObjRef();
  CRhinoObjRef(
            unsigned int doc_sn, // Document containing the object UUID
            const UUID& object_uuid // object UUID
            );
  ON_DEPRECATED_MSG("Use the form that takes a document serial number and object Id")
  CRhinoObjRef(
            const UUID& object_uuid // object UUID
            );
  CRhinoObjRef(
            const CRhinoObject* object // object in doc
            );
  CRhinoObjRef(
            const CRhinoObject* object,  // object in doc
            const ON_Geometry* geometry, 
            BOOL32 bProxyGeometry // true if geometry is a proxy object that needs to be deleted
            );
  CRhinoObjRef(
            const CRhinoObject* object,  // object in doc
            const CRhinoPickContext& pick_context
            );
  CRhinoObjRef(
            const CRhinoObject* object,  // object in doc
            const class CRhinoGetObject& go
            );
  CRhinoObjRef(
               unsigned int doc_sn, // Document containing the object ON_ObjRef
               const ON_ObjRef&
               );
  ON_DEPRECATED_MSG("Use the form that takes a document serial number and ON_ObjRef")
  CRhinoObjRef(const ON_ObjRef&);

  /*
    Creates an reference counted wrapper around the polyedge.
    When the last reference is destroyed, the polyedge is
    deleted.  A CRhinoProxyObject is automatically created
    for the polyedge.
  Parameters:
    doc - [in]
      Document containing actual geometry objects in the polyedge.
    polyedge - [in]  
      This CRhinoPolyEdge class must be on the heap.
      The resulting objref will use reference counting and
      delete the polyedge when the last reference is deleted.
    attributes - [in]  
      If null, then CRhinoDoc::GetDefaultObjectAttributes() will 
      be used to set the attributes on the CRhinoProxyObject().    
  */
  CRhinoObjRef(
      const class CRhinoDoc& doc,
      class CRhinoPolyEdge* polyedge,
      const ON_3dmObjectAttributes* attributes
    );

  ON_DEPRECATED_MSG("Use the form that takes a document serial number")
  CRhinoObjRef( // obsolete version - attributes should be const
      class CRhinoPolyEdge* polyedge,
      ON_3dmObjectAttributes* attributes
    );

private:
  // hack to fix error in obsolete constructor
  void PolyEdgeConstructorHelper(
          const class CRhinoDoc&,
          class CRhinoPolyEdge*,
          const ON_3dmObjectAttributes*
          );
public:

  /*
  Description:
    Determine if two CRhinoObjRefs refer to the same object.
  Returns:
    true if the object and object and geometry pointers are equal. 
  */
  bool operator==(const CRhinoObjRef& ) const;

  /*
  Description:
    Determine if two CRhinoObjRefs refer to the same object.
  Returns:
    true if the object or geometry pointers are not equal. 
  */
  bool operator!=(const CRhinoObjRef& ) const;

  /*
  Description:
    When an object is selected by picking a sub-object, SetSelectionComponent
    may be used to identify the sub-object.
  Parameters:
    component_index - [in]
  Remarks:
    Unconditionally sets the ON_ObjRef::m_component_index field. 
  */
  void SetSelectionComponent( ON_COMPONENT_INDEX component_index );

  /*
  Description:
    When an object is selected by picking a point on the object,
    SetSelectionPoint is used to save the location of the point.
  Parameters:
    point - [in]
  */
  void SetSelectionPoint( const ON_3dPoint& point );

  /*
  Description:
    When an annotation is selected by picking a point on the annotation,
    SetAnnotationSelectionPoint is used to save the annotation parameter
    and location of the point.
  Parameters:
    component_index - [in] component index of snap point
    point - [in]
  Remarks:
    Use CRhinoObjRef::AnnotationParameter to get the location of the pick.
  */
  void SetAnnotationSelectionPoint(ON_COMPONENT_INDEX component_index, const ON_3dPoint& point);

  /*
  Description:
    When an curve is selected by picking a point on the curve,
    SetCurveSelectionPoint is used to save the curve parameter
    and location of the point.
  Parameters:
    curve_parameter - [in] curve parameter
    point - [in]
  Remarks:
    Use CRhinoObjRef::CurveParameter to get the location of the pick.
  */
  void SetCurveSelectionPoint(double curve_parameter, const ON_3dPoint& point);

  /*
  Description:
    When an surface is selected by picking a point on the surface,
    SetSurfaceSelectionPoint is used to save the surface parameters
    and location of the point.
  Parameters:
    u - [in] first surface parameter
    v - [in] second surface parameter
    point - [in]
  Remarks:
    Use CRhinoObjRef::SurfaceParameter to get the location of the pick.
  */
  void SetSurfaceSelectionPoint( double u, double v, const ON_3dPoint& point );

  /*
  Description:
    When a point is picked on the edge of a brep, SetEdgeSelectionPoint
    is used to save the edge parameter, trim parameter, and the location
    of the point.
  Parameters:
    edge_parameter - [in] edge parameter
    point - [in]
    edge - [in]
    trim - [in]
  Remarks:
    Use CRhinoObjRef::TrimParameter, CRhinoObjRef::CurveParameter, 
    and CRhinoObjRef::SurfaceParameter to get the location of the pick.
  */
  void SetEdgeSelectionPoint( 
          double edge_parameter, 
          const ON_3dPoint& point,
          const ON_BrepEdge& edge,
          const ON_BrepTrim& trim
          );

  bool SetMeshVertexSelectionPoint(
          int vertex_index,
          const ON_Mesh& mesh
          );

  bool SetMeshEdgeSelectionPoint(
          int edge_index,
          double t,
          const ON_MeshTopology& meshtop
          );

  bool SetMeshFaceSelectionPoint(
          int face_index,
          const double t[4],
          const ON_Mesh& mesh
          );

  bool SetParentInstanceObject( 
    const class CRhinoInstanceObject* iref_object, 
    int idef_geometry_index 
    );

  void SetSelectionDepth( double );
  void SetSelectionDistance( double );
  void SetSelectionView( class CRhinoView* );
  void SetSelectionDetailView( class CRhinoView* page_view, unsigned int detail_object_sn );
  void SetDetails( int flags, CRhinoObject::GEOMETRY_TYPE geotype, ON_COMPONENT_INDEX ci );

  ~CRhinoObjRef();
  CRhinoObjRef(const CRhinoObjRef&);
  CRhinoObjRef& operator=(const CRhinoObjRef&);

  /*
  Description:
    Returns the runtime document serial number for the document containing the
    referenced object.
  Returns:
   Returns the runtime document serial number for the document containing the
   referenced object.
  */
  unsigned int DocumentRuntimeSerialNumber() const;

  /*
  Description:
    Returns the id of the referenced Rhino object.
  Returns:
    Returns the id of the referenced Rhino object.
  See Also:
    CRhinoObjRef::Object
  */
  ON_UUID ObjectUuid() const;


  /*
  Description:
    Returns the referenced Rhino object.
  Returns:
    Pointer to the referenced Rhino object.
  See Also:
    CRhinoObjRef::Geometry    
  */
  const CRhinoObject* Object() const;

  unsigned int RhinoObjectRuntimeSerialNumber() const;

  /*
  Description:
    Returns the type of referenced geometry.
  Returns:
    Type of referenced geometry.
  Remarks:
    The reference geometry may be a component of the
    parent object's geometry.  So, the type returned
    by this function may be different than the type
    parent object's geometry.
  See Also:
    CRhinoObjRef::Geometry
  */
  CRhinoObject::GEOMETRY_TYPE GeometryType() const;


  /*
  Description:
    Get the component index of the referenced (sub) geometry.
  Returns:
    @untitled table
    -1     Top level object is referenced.
    >0     component index of the referenced sub-geometry.
  Remarks:
    Some objects have subobjects that are valid pieces of geometry.
    For example, breps have edges and faces that are valid curves
    and surfaces.  Each subobject has a component index that is > 0.
    The parent geometry has a component index = -1.
  See Also:
    CRhinoObjRef::Geometry
  */
  ON_COMPONENT_INDEX GeometryComponentIndex() const;

  /*
  Description:
    Returns the referenced piece of geometry.  This piece of geometry
    may be a sub-part of the parent object geometry. 

    In some cases, like when pieces of instance references
    are selected in CRhinoGetObject::GetObjects(), the
    CRhinoObjRef contains reference counted proxy geometry. 
    You must keep at least one CRhinoObjRef in scope as long
    as you are using a pointer returned by Geometry().
    After the destruction of the last CRhinoObjRef,
    the proxy geometry is deleted.  At that time any
    pointers to the proxy geometry will be invalid.
  Example:

          // pReferencedGeometry may be a component of the pParentGeometry
          // For example, pParentGeometry may point to an ON_Brep and
          // pReferencedGeometry may point to an ON_BrepEdge in the brep.
          CRhinoObjRef objref = ...;
          const ON_Geometry* pReferencedGeometry = objref.Geometry();
          const CRhinoObject* pParentObject = objref.Object();
          const ON_Geometry* pParentGeometry = pParentObject->Geometry();

  Returns:
    Pointer to the referenced piece of geometry.
  Remarks:
    CRhinoObjRef::Geometry is a low level utility function and, in general, 
    it is more convenient to use GeometryType to determine the
    type of referenced geometry and the Point, Curve, ...,
    functions to get at the actual geometry.
  See Also:
    CRhinoObjRef::GeometryType
    CRhinoObjectRef::IsSubGeometry
    CRhinoObjRef::GeometryComponentIndex
    CRhinoObjectRef::Point
    CRhinoObjectRef::Curve
    CRhinoObjectRef::Surface
    CRhinoObjectRef::Edge
    CRhinoObjectRef::Trim
    CRhinoObjectRef::Face
    CRhinoObjectRef::Brep
    CRhinoObjectRef::Mesh
  */
  const ON_Geometry* Geometry() const;

  /*
  Description:
    If the referenced geometry is a point,
    this returns the point.
  Returns:
    point or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Point* Point() const;

  /*
  Description:
    If the referenced geometry is a point cloud,
    this returns the point cloud.
  Returns:
    point or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_PointCloud* PointCloud() const;

  /*
  Description:
    If the referenced geometry is a curve or edge,
    this returns the curve.
  Returns:
    curve or NULL.
  See Also:
    CRhinoObjectRef::Edge
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Curve* Curve() const;

  /*
  Description:
    If the referenced geometry is a surface, a brep with one face, or
    an edge of a surface, this returns the surface.
  Returns:
    surface or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Surface* Surface() const;


  /*
  Description:
    If the referenced geometry is a subd, this returns the subd.
  Returns:
    subd or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_SubD* SubD() const;

  ON_SubDRef SubDRef() const;
  const ON_SubDComponentPtr SubDComponent() const;
  const ON_SubDFace* SubDFace() const;
  const ON_SubDEdge* SubDEdge() const;
  const ON_SubDVertex* SubDVertex() const;
  const ON_SubDComponentRef* SubDComponentRef() const;

  const ON_SubDComponentRef SubDVertexRef() const;
  const ON_SubDComponentRef SubDEdgeRef() const;
  const ON_SubDComponentRef SubDFaceRef() const;

  /*
  Description:
    If the referenced geometry is an edge, this returns the edge.
  Parameters:
    bAllowProxy - if true and the object is really a subd or extrusion,
    then a proxy will be returned when possible.
    The version with no parameter acts like bAllowProxy = false.
  Returns:
    edge or NULL.
  See Also:
    CRhinoObjectRef::Curve
    CRhinoObjectRef::Trim
    CRhinoObjectRef::Brep
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_BrepEdge* Edge() const;
  const ON_BrepEdge* Edge(bool bAllowProxy) const;

  /*
  Description:
    If the referenced geometry is an edge of a surface,
    this returns the associated brep trim.
  Parameters:
    bAllowProxy - if true and the object is really a subd or extrusion,
    then a proxy will be returned when possible.
    The version with no parameter acts like bAllowProxy = false.
  Returns:
    trim or NULL.
  Remarks:
    The associated object is a CRhinoBrepObject.
  See Also:
    CRhinoObjectRef::Curve
    CRhinoObjectRef::Trim
    CRhinoObjectRef::Brep
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_BrepTrim* Trim() const;
  const ON_BrepTrim* Trim(bool bAllowProxy) const;

  /*
  Description:
    If the referenced geometry is an edge of a surface,
    this returns the associated brep trim.
  Returns:
    loop or NULL.
  Remarks:
    The associated object is a CRhinoBrepObject.
  See Also:
    CRhinoObjectRef::Curve
    CRhinoObjectRef::Trim
    CRhinoObjectRef::Brep
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_BrepLoop* Loop() const;

  /*
  Description:
    If the referenced geometry is a brep face, a brep with one face, or
    an edge of a surface, this returns the brep face.
  Parameters:
    bAllowProxy - if true and the object is really a subd or extrusion,
    then a proxy will be returned when possible.
    The version with no parameter acts like bAllowProxy = false.
  Returns:
    surface or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_BrepFace* Face() const;
  const ON_BrepFace* Face(bool bAllowProxy) const;

  /*
  Description:
    If the referenced geometry is a brep, a surface, or an edge,
    this returns the brep.
  Returns:
    brep or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  Remarks:
    Note that the brep may be a proxy brep from an extrusion or subds.
    This has been the case since 2010 for extrusions and always
    has happened for subds.
  */
  const ON_Brep* Brep() const;

  /*
  Description:
    If the referenced geometry is a extrusion,
    this returns the extrusion.
  Returns:
    extrusion or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Extrusion* Extrusion() const;

  /*
  Description:
    If the referenced geometry is a mesh,
    mesh vertex, mesh edge, or mesh face,
    this returns the mesh.
  Returns:
    mesh or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Mesh* Mesh() const;

  /*
  Description:
    If the referenced geometry is a mesh vertex, face, edge or ngon
    this returns the mesh component reference.
  Returns:
    A mesh component reference or null if there isn't one.
  */
  const ON_MeshComponentRef* MeshComponentRef() const;

  /*
  Description:
    If the referenced geometry is some type of annotation,
    this returns the annotation.
  Returns:
    mesh or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Annotation* Annotation() const;

  /*
  Description:
    If the referenced geometry is some type of render light,
    this returns the render light.
  Returns:
    mesh or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_Light* Light() const;

  /*
  Description:
    If the referenced geometry is ON_TextContent,
    this returns the text.
  Returns:
    Text or NULL.
  See Also:
    CRhinoObjectRef::Geometry
    CRhinoObjectRef::GeometryType
  */
  const ON_TextContent* RichText() const;

  //////////
  // returns true if the selected piece of geometry is a proper
  // sub-part of the parent object.
  bool IsSubGeometry() const;

  /*
  Description:
    Get the method used to select this object.
  Returns:
    0: selected by non-mouse method (SelAll, etc.)
    1: selected by mouse click on the object
    2: selected by being inside of a mouse window
    3: selected by intersecting a mouse crossing window
  */
  int SelectionMethod() const;


  /*
  Returns:
    0: not a shaded mode point pick
    1: shaded mode point pick
  */
  int SelectionStyle() const;

  /*
  Returns:
    Depth of selection point.
    -1 (far away) to 1 (near) or ON_UNSET_VALUE if not available.
  */
  double SelectionDepth() const;

  /*
  Returns:
    Relative visual distance from pick point to the object.
    0 (spot on) to 2 (outer corner of pick box)  0 is returned
    for objects that were not picked using a point pick.
  */
  double SelectionDistance() const;

  //////////
  // If the object was selected by picking a point on it, then
  // SelectionPoint() returns true and the point where the selection
  // occurred.
  bool SelectionPoint( ON_3dPoint& ) const;
  
  //////////
  // If the object was interactively selected in a particular viewport, then
  // SelectionView() returns the view where the object was selected.
  class CRhinoView* SelectionView() const;

  // If the object was interactively selected in a page space detail
  // view, then SelectionViewDetailSerialNumber() returns the CRhinoObject
  // serial number of the detail view object.  Use SelectionView()
  // to get the page view that contains the detail view object.
  // If SelectionViewDetailSerialNumber() returns 0, then the selection
  // did not happen in a detail view.
  unsigned int SelectionViewDetailSerialNumber() const;

  /*
  Description:
    If the reference geometry is an annotation with a selection
    point, then this gets the parameter of the selection point.
  Parameters:
    annotation_parameter - [out] If the selection point is on an annotation,
          then *annotation_parameter is the parameter of the selection point.
  Returns:
    If the selection point was on an annotation, a pointer
    to the annotation is returned.
  Remarks:
    If an annotation was selected and AnnotationParameter is called and the 
    ObjectRef::SelectionMethod() is not 1 (point pick on object), 
    the curve will be returned and curve_parameter will be set to 0.
    This can be misleading so it may be necessary to call SelectionMethod() first,
    before calling AnnotationParameter() to get the desired information.
  See Also:
    CRhinoObjRef::Annotation
  */
  const ON_Annotation* AnnotationParameter( 
            double* annotation_parameter 
            ) const;

  /*
  Description:
    If the reference geometry is a curve or edge with a selection
    point, then this gets the parameter of the selection point.
  Parameters:
    curve_parameter - [out] If the selection point is on a curve or edge,
    then *curve_parametert is the parameter of the selection point.
  Returns:
    If the selection point was on a curve or edge, a pointer
    to the curve/edge is returned.
  Remarks:
    If a curve was selected and CurveParameter is called and the
    ObjectRef::SelectionMethod() is not 1 (point pick on object),
    the curve will be returned and curve_parameter will be set to
    the start parameter of the picked curve.  This can be misleading
    so it may be necessary to call SelectionMethod() first, before
    calling CurveParameter() to get the desired information.
  See Also:
    CRhinoObjRef::Curve
    CRhinoObjRef::TrimParameter
  */
  const ON_Curve* CurveParameter(
    double* curve_parameter
  ) const;

  /*
  Description:
    If the reference geometry is a surface, brep with one face,
    or surface edge with a selection point, then this gets the 
    surface parameters of the selection point.
  Parameters:
    u - [out]
    v - [out] If the selection point was on surface, then (*u,*v)
          are the parameters of the selection point.
  Returns:
    If the selection point was on a surface, the
    the surface is returned.
  */
  const ON_Surface* SurfaceParameter( 
          double* u, 
          double* v 
          ) const;

  /*
  Description:
    If the reference geometry is an edge of a surface,
    then this gets the trim parameter of the selection point.
  Parameters:
    trim_parameter - [out] If non-null pointer is returned, this is then
              *trim_parameter is the parameter of the selection point.
  Returns:
    If the selection point was on the edge of a surface, a pointer
    to the associated trim is returned.
  See Also:
    CRhinoObjRef::CurveParameter
  */
  const ON_BrepTrim* TrimParameter( 
            double* trim_parameter 
            ) const;

  int Flags() const;

  class CNestedIRef
  {
  public:
    // instance reference
    const class CRhinoInstanceObject* m_iref;

    // Transformation
    ON_Xform m_xform;
  };

  /*
  Description:
    If the CRhinoObjRef geometry is in the list returned by 
    RhinoGetRenderMeshes or RhinoGetAnalysisMeshes and a 
    transformed version of a piece of geometry that is 
    ultimately part of an instance  definition, then this 
    function returns that top level instance reference and 
    the complete transformation that was applied to the 
    piece of geometry.
  Parameters:
    xform - [out]  transformation is returned here.
    nested_iref - [out] if not NULL a list of
      nested instance references is returned here.
      If there are no nested references the list is empty.
      The first item in the list is the non-nested
      instance.  The last item in a multi-item list will
      be the penultimate instance reference.
  Returns:
    NULL if this geometry did not come from an instance 
    definition.  Pointer to the top level instance 
    reference if the geometry did come from an instance
    definition.
  Example:
    Assume a CRhinoMeshObject M2 is the geometry for
    instance definition D1.  CRhinoInstanceObject R1 is a
    reference to D1 and the geometry for instance definition
    D2.  R2 is a reference to D2 and the geometry for instance
    definition D3.  R3 is a reference to D3.
    Then CRhinoObjRef::Object() CRhinoObjRef::Object() would
    return a pointer to M2 and CRhinoObjRef::Geometry() would
    return a pointer to a mesh that is already transformed
    to be in the correct location.
    CRhinoObjRef::GetInstanceTransformation() would return a 
    pointer to R3, xform would be the transformation that
    was used to move M2->Mesh() to the location of the returned
    mesh, and the and the nested_iref[] array would be (R2,R1).
  See Also:
    RhinoGetRenderMeshes
    RhinoGetAnalysisMeshes
  */
  const class CRhinoInstanceObject* GetInstanceTransformation( 
    ON_Xform& xform,
    ON_SimpleArray<CNestedIRef>* nested_iref= 0
    ) const;

  /*
  Description:
    Expert user tool to decrement reference counts.  Most
    users will never need to call this tool.  It is called
    by ~CRhinoObjRef and used in rare cases when a
    CRhinoObjRef needs to reference an object only by UUID
    and component index.
  */
  void DecrementReferenceCount();

  /*
  Description:
    Expert user tool to use the object UUID and component
    index to set m_object and m_geometry when they are NULL.
    Use DecrementReferenceCount() to unbind.
  */
  bool BindToRuntimeObject();

private:
  friend class CRhinoDoc;
  friend class CRhBrepDisplay; // for subobject picking
  friend class CRhinoPointCloudObject; // for subobject picking
  friend class CRhinoInstanceObject; // for subobject picking
  friend class CRhinoBrepObject; // for shaded flag picking
  friend class CRhinoSubDObject; // for shaded flag picking
  friend class CRhinoMeshObject; // for shaded flag picking
  friend class CRhinoExtrusionObject; // for shaded flag picking
  friend class CRhDropTargetManager; //For setting the pick flags during a drop operation
  void ConstructorHelper(const class CRhinoGetObject* go,
                         const CRhinoObject* object,
                         const ON_Geometry* geometry,
                         const ON_Geometry* proxy_geometry
                         );

  void SetDocumentRuntimeSerialNumber(unsigned int docSn);

  // In some cases, like polyedge picking, a proxy object
  // is created.  This object is not in the CRhinoDoc but
  // acts like an object so commands do not have to have
  // special case handling code.  The memory used by the
  // proxy object is managed by the base class (ON_ObjRef),
  // which reference counts and deletes the proxy object
  // when the last CRhinoObjRef that uses it is deleted.
  //
  // Some 3rd party plug-ins are using CRhinoObjRef as a general
  // parameter to pass around pointers to CRhinoObject classes
  // that are not in the document.  This practice is strongly
  // discouraged but is supported because it worked in earlier
  // versions of Rhino.
  const CRhinoObject* m__proxy_object;

  int  m_pick_flags; // if (m_flags&4) is set, object was "point selected"
                // if (m_flags&8) is set, object was "window selected"
                // if (m_flags&16) is set, object was "crossing selected"
                // if (m_flags&32) is set, object was "shaded selected", otherwise wireframe selected
                // if (m_flags&64) is set, object was part of a selected group
                // if (m_flags&128) is set, object was part of a selected group but not actually hit in the pick
                // if (m_flags&256) is set, brep (sub)object was"point selected" with a hit on an edge.
                // if (m_flags&512) is set, brep pick was a wire pick, but shaded part of the brep was also hit closer to the camera
                // if (m_flags&1024) is set, brep (sub)object was"point selected" with a hit on an isocurve.
                // if (m_flags&2048) is set, unjoined brep edge was single picked on the surface side.
                // (m_flags&1) reserved // obsolete settings -> if is set, m_pGeometry will be deleted when last ref is destroyed.
                // (m_flags&2) reserved // obsolete settings -> if is set, m_pHeader will be deleted when last ref is destroyed
  
  mutable class CRhObjRefPrivate* m_private = nullptr;

  // If m_detail_view_sn is nonzero, then the pick was in a detail view embedded
  // in a page space view. In this case, m_pick_view identifies the page space view
  // and m_detail_view_sn is the value of the detail's 
  // CRhinoObject::m_runtime_serial_number.
  unsigned int m_detail_view_sn;

  // relative depth of hit point on object
  // (-1 = far, 1 = near,non point picks are ON_UNSET_VALUE )
  double m_pick_depth; 

  // relative screen distance from pick point to hit point on object
  // (0 = direct hit, 2 = corner of pick box, non point picks are ON_UNSET_VALUE)
  double m_pick_dist;  
};

// obsolete symbol - use CRhinoObjRefArray
#define CRhinoPickList CRhinoObjRefArray

class RHINO_SDK_CLASS CRhinoObjRefArray : public ON_ClassArray<CRhinoObjRef>
{
public:
  // Description:
  //   Appends all selected objects to the array.
  //
  // Parameters:
  //   it - [in] iterator for objects to append.
  //
  // Returns:
  //   Number of objects appended to array.
  int AppendObjects( class CRhinoObjectIterator& it );
};


/*
Description:
  Returns the custom render mesh parameters of an object.
Parameters:
  obj   - [in]  A mesh-able object.
  mp    - [out] The object's mesh parameters.
Returns:
  true  - the object has custom render mesh parameters.
  false - the object does not have custom render mesh parameters.
See Also:
  RhinoObjectSetMeshParameters
  RhinoObjectIsMeshParametersEnabled
  RhinoObjectEnableMeshParameters
  RhinoObjectRemoveMeshParameters
*/
RHINO_SDK_FUNCTION
bool RhinoObjectGetMeshParameters( 
        const CRhinoObject* obj, 
        ON_MeshParameters& mp 
        );

/*
Description:
  Sets (adds or modifies) the custom render mesh parameters of an object.
Parameters:
  obj     - [in] A mesh-able object.
  mp      - [in] The mesh parameters.
  bEnable - [in] Enable the custom render mesh parameters.
Returns:
  true    - successful. 
  false   - unsuccessful.
See Also:
  RhinoObjectGetMeshParameters
  RhinoObjectIsMeshParametersEnabled
  RhinoObjectEnableMeshParameters
  RhinoObjectRemoveMeshParameters
Remarks:
  If bEnable=true, then the custom mesh parameters will be assigned to the object,
  and the object's render meshes will generated based on these mesh parameters.
  If bEnable=false, then the custom mesh parameters will be assigned to the object,
  but the object's render meshes will still be generated based on the document's 
  mesh parameters.
*/
RHINO_SDK_FUNCTION
bool RhinoObjectSetMeshParameters( 
        const CRhinoObject* obj, 
        const ON_MeshParameters& mp, 
        bool bEnable = true 
        );

/*
Description:
  Verifies that an object's custom render mesh parameters are enabled.
Parameters:
  obj      - [in]  A mesh-able object.
  bEnabled - [out] The current enabled state, either true or false.
Returns:
  true     - the object has custom render mesh parameters.
  false    - the object does not have custom render mesh parameters.
See Also:
  RhinoObjectGetMeshParameters
  RhinoObjectSetMeshParameters
  RhinoObjectEnableMeshParameters
  RhinoObjectRemoveMeshParameters
Remarks:
  If the object has custom render mesh parameters and they are enabled, then
  the object's render meshes will generated based on these mesh parameters.
  If the object does not have custom render mesh parameters, or if the object has 
  custom render mesh parameters but the are not enabled, then object's render meshes
  will be generated based on the document's mesh parameters.
*/
RHINO_SDK_FUNCTION
bool RhinoObjectIsMeshParametersEnabled( 
        const CRhinoObject* obj, 
        bool& bEnabled 
        );

/*
Description:
  Enables or disables an object's custom render mesh parameters.
Parameters:
  obj     - [in] A mesh-able object.
  bEnable - [in] true to enable, false to disable.
Returns:
  true    - the object has custom render mesh parameters.
  false   - the object does not have custom render mesh parameters.
See Also:
  RhinoObjectGetMeshParameters
  RhinoObjectSetMeshParameters
  RhinoObjectIsMeshParametersEnabled
  RhinoObjectRemoveMeshParameters
Remarks:
  If the object has custom render mesh parameters and they are enabled, then
  the object's render meshes will generated based on these mesh parameters.
  If the object does not have custom render mesh parameters, or if the object has 
  custom render mesh parameters but the are not enabled, then object's render meshes
  will be generated based on the document's mesh parameters.
*/
RHINO_SDK_FUNCTION
bool RhinoObjectEnableMeshParameters( 
        const CRhinoObject* obj, 
        bool bEnable = true 
        );

/*
Description:
  Deletes an object's custom render mesh parameters.
Parameters:
  obj     - [in] A mesh-able object.
Returns:
  true -  the object has custom render mesh parameters and they were deleted.
  false - the object does not have custom render mesh parameters, or
          deleting failed.
See Also:
  RhinoObjectGetMeshParameters
  RhinoObjectSetMeshParameters
  RhinoObjectIsMeshParametersEnabled
  RhinoObjectEnableMeshParameters
*/
RHINO_SDK_FUNCTION
bool RhinoObjectRemoveMeshParameters( 
        const CRhinoObject* obj 
        );

/*
Description:
  Create fill breps for given CRhinoObject with given clipping planes.
Parameters:
  obj             - [in] object to be clipped
  cps             - [in] clipping planes to use
  aBreps          - [out] list of breps as the fill geometry, if any
  bUnclippedFills - [in] set to true if the fills by a clipping plane shouldn't be
    clipped by the other clipping planes
Returns:
  true    - fills for the object were generated
  false   - no fills for the object were generated
*/
RHINO_SDK_FUNCTION
bool RhinoObjectClippingPlaneFills(
        const CRhinoObject& obj,
        ON_SimpleArray<const CRhinoClippingPlaneObject*>& cp,
        ON_SimpleArray<ON_Brep*>& aBreps,
        bool bUnclippedFills = false
        );

