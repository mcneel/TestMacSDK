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

#include "rhinoSdkGet.h"
#include "rhinoSdkObject.h"

/*
CLASS
CRhinoGetObject

Description:
  The CRhinoGetObject class is the tool commands use to interactively
  select objects.

Example:

          CRhinoGetObject go;
          go.GetObject();
          if ( go.Result() != CRhinoGet::object )
            ... use canceled or some other type of input was provided
          int object_count = go.ObjectCount();
          for ( i = 0; i < object_count; i++ ) 
          {
            CRhinoObjRef objref = go.Object(i);
            CRhinoObject* objref.Object();
            const ON_Geometry* geo = objref.Geometry();
            ...
          }

Remarks:
  The behavior of CRhinoGetObject::GetObject() can be highly customized
  to accept many kinds of alternative input, to restrict what kinds of
  object may be selected, to control how many object may be selected,
  etc.  See the detailed comments in the declarations of CRhinoGet
  and CRhinoGetObject for more information.
*/

class RHINO_SDK_CLASS CRhinoGetObject : public CRhinoGet
{
public:
  CRhinoGetObject();
  ~CRhinoGetObject();
  CRhinoGetObject( const CRhinoGetObject& );
  CRhinoGetObject& operator=(const CRhinoGetObject&);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //         and command options.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Specify object picking behavior.
  //

  // Description:
  //   The GEOMETRY_TYPE_FILTER enum values are bitwise or-ed
  //   together to create a filter used to specify acceptable
  //   geometry types.  The geometry type filter is set
  //   by calling CRhinoGetObject::SetGeometryFilter.
  //   values = ON::object_type enum values.
  //     In an ideal world, the CRhinoGetObject::GEOMETRY_TYPE_FILTER
  //     would be removed and ON::object_type would be used everywhere.
  enum GEOMETRY_TYPE_FILTER
  {
    point_object         = ON::object_type::point_object,     // single point
    grip_object          = ON::object_type::grip_object,
    pointset_object      = ON::object_type::pointset_object,  // point cloud or point grid
    curve_object         = ON::object_type::curve_object,     // curve (can be edge in a brep)
    surface_object       = ON::object_type::surface_object, // surface or brep face
    polysrf_object       = ON::object_type::polysrf_filter, // brep with 2 or more faces
    edge_object          = ON::object_type::edge_filter, // brep edge (with an associated ON_BrepTrim)
    polyedge_object      = ON::object_type::polyedge_filter, // a chain of edges and/or curves
    loop_object          = ON::object_type::loop_object, // brep loop
    brepvertex_filter    = ON::object_type::brepvertex_filter, // brep vertex
    mesh_object          = ON::object_type::mesh_object, // ON_Mesh
    annotation_object    = ON::object_type::annotation_object,
    light_object         = ON::object_type::light_object,
    instance_reference   = ON::object_type::instance_reference,

    // NOTE:
    // meshvertex_filter,meshedge_filter,meshface_filter,meshcomponent_reference
    // are used for selection filter that accept ON_Mesh and ON_SubD components.
    meshvertex_filter    = ON::object_type::meshvertex_filter, // ON_Mesh.m_V[] index, ON_MeshTopologyVertex, ON_SubDVertex
    meshedge_filter      = ON::object_type::meshedge_filter, // ON_MeshTopologyEdge, ON_SubDEdge
    meshface_filter      = ON::object_type::meshface_filter, // selection filter for ON_Mesh.m_F[] index, ON_MeshTopologyFace, ON_SubDFace
    meshcomponent_reference = ON::object_type::meshcomponent_reference, // ON_MeshComponentRef or ON_SubDComponentRef
    
    cage_object          = ON::object_type::cage_object,
    hatch_object         = ON::object_type::hatch_object,
    morph_control_object = ON::object_type::morph_control_object,
    subd_object          = ON::object_type::subd_object, // ON_SubD, ON_SubDRef, ON_SubDComponentRef, ON_SubD....
    phantom_object       = ON::object_type::phantom_object,
    extrusion_object     = ON::object_type::extrusion_object,
    any_object           = ON::object_type::any_object
  };

  // Description:
  //   If an object passes the geometry TYPE filter,then the
  //   geometry ATTRIBUTE filter is applied.
  //   The geometry attributes filter is set
  //   by calling CRhinoGetObject::SetGeometryFilter.
  enum GEOMETRY_ATTRIBUTE_FILTER
  {
    ////////////////////////////////////////////////////////////
    // attributes of acceptable curve objects.
    //

    //   If you want to accept only wire or edge curves, then
    //   specify wire_curve or edge_curve, otherwise both wire
    //   and edge curves will pass the attribute filter.
    wire_curve = 1<<0, // 3d wire curve
    edge_curve = 1<<1, // 3d curve of a surface edge

    //   If you want to accept only closed or open curves, then
    //   specify either closed_curve or open_curve.  Otherwise both
    //   closed and open curves will pass the attribute filter.
    closed_curve   = 1<<2, // closed curves and edges are acceptable
    open_curve     = 1<<3, // open curves and edges are acceptable

    ////////////////////////////////////////////////////////////
    // attributes of acceptable trimming edge objects
    // (associated with an ON_BrepTrim).
    //
    //  If none of these attributes are explicitly specified, then
    //  any kind of trimming edge will pass the attribute filter.
    seam_edge        = 1<<4, // seam edges are acceptable
    manifold_edge    = 1<<5, // edges with 2 difference surfaces pass
    nonmanifold_edge = 1<<6, // edges with 3 or more surfaces pass
    mated_edge       = (1<<4)|(1<<5)|(1<<6), // any mated edge passes
    surface_boundary_edge  = 1<<7, // boundary edges on surface sides pass
    trimming_boundary_edge = 1<<8, // boundary edges that trim a surface pass
    boundary_edge    = (1<<7)|(1<<8), // any boundary edge passes

    ////////////////////////////////////////////////////////////
    // attributes of acceptable surface objects.
    //

    //   If you want to accept only closed or open surfaces, then
    //   specify either closed_surface or open_surface.  Otherwise both
    //   closed and open surfaces will pass the attribute filter.
    closed_surface     = 1<<9,
    open_surface       = 1<<10,

    //   If you want to accept only trimmed or untrimmed surfaces, then
    //   specify either trimmed_surface or untrimmed_surface.  Otherwise
    //   both trimmed and untrimmed surfaces will pass the attribute 
    //   filter.
    trimmed_surface     = 1<<11,
    untrimmed_surface   = 1<<12,

    //   If you want to accept only sub-surfaces of (multi-surface)
    //   polysrf, then specify sub_surface.  If you do not want to
    //   accept sub-surfaces, then specify top_surface.  Otherwise
    //   sub-surfaces and top surfaces will pass the attribute filter.
    sub_surface = 1<<13,
    top_surface = 1<<14,

    ////////////////////////////////////////////////////////////
    // attributes of acceptable polysrf objects.
    //

    //   If you want to accept only manifold or nonmanifold polysrfs,
    //   then specify manifold_polysrf or nonmanifold_polysrf. Otherwise
    //   both manifold and nonmanifold polysrfs will pass the attribute
    //   filter.
    manifold_polysrf    = 1<<15,
    nonmanifold_polysrf = 1<<16,

    //   If you want to accept only closed or open polysrfs, then
    //   specify either closed_polysrf or open_polysrf.  Otherwise both
    //   closed and open polysrfs will pass the attribute filter.
    closed_polysrf     = 1<<17,
    open_polysrf       = 1<<18,

    ////////////////////////////////////////////////////////////
    // attributes of acceptable mesh objects.
    //

    //   If you want to accept only closed or open meshs, then
    //   specify either closed_mesh or open_mesh.  Otherwise both
    //   closed and open meshs will pass the attribute filter.
    closed_mesh     = 1<<19,
    open_mesh       = 1<<20,

    ////////////////////////////////////////////////////////////
    // attributes of acceptable loop objects.
    //
    boundary_inner_loop = 1<<21, // all trimming edges are boundary edges
    mated_inner_loop    = 1<<22, // all trimming edges are mated
    inner_loop = (1<<21)|(1<<22), // any inner loop is acceptable

    boundary_outer_loop = 1<<23, // all trimming edges are boundary edges
    mated_outer_loop    = 1<<24, // all trimming edges are mated
    outer_loop = (1<<23)|(1<<24), // any outer loop is acceptable

    special_loop = (1<<25),       // slit, crvonsrf, ptonsrf, etc.

    accept_all_attributes = 0xffffffff  // all attributes are acceptable
  }; 

  /*
    Description:
      The geometry type filter controls which types of geometry
      (points, curves, surfaces, meshes, etc.) can be selected.  
      The default geometry type filter permits selection of all 
      types of geometry.
  
    Parameters:
      geometry_type_filter - [in] an unsigned int made by or-ing ( | )
          values from the GEOMETRY_TYPE_FILTER enum.  If you pass 0,
          then all geometry will be acceptable.

    Example:
      If you want to be able to select curves and meshes, then
      you would make a call like

         unsigned int geometry_type_filter = 0;
         geometry_filter |= CRhinoGetObject::curve_object;
         geometry_filter |= CRhinoGetObject::mesh_object;
         SetGeometryFilter( geometry_type_filter );

    See Also:
      CRhinoGetObject::GeometryFilter
      CRhinoGetObject::SetGeometryAttributeFilter
      CRhinoGetObject::GeometryAttributeFilter
  */
  void SetGeometryFilter( 
           unsigned int geometry_type_filter
           );

  /*
    Description:
      The geometry type filter controls which types of geometry
      (points, curves, surfaces, meshes, etc.) can be selected.  
      The default geometry type filter permits selection of all 
      types of geometry.
  
    Returns:
      An unsigned int "object_filter".  If the bit from an
      GEOMETRY_TYPE_FILTER enum value is set in the geometry_filter,
      then GetObjects() will permit selection of object's
      that have that type of geometry.  If geometry_filter
      is 0xFFFFFFFF, then GetObjects() can select all types
      of geometry.

    Example:
      If (GeometryFilter() | curve_object) != 0, then GetObjects()
      will permit curves to be selected.

    See Also:
      CRhinoGetObject::SetGeometryFilter
      CRhinoGetObject::SetGeometryAttributeFilter
      CRhinoGetObject::GeometryAttributeFilter
  */
  unsigned int GeometryFilter() const;

  /*
    Description:
      The geometry attribute filter provides a secondary filter
      can be used to restrict which objects can be selected.  
      Control of the type of geometry (points, curves, surfaces,
      meshes, etc.) is provided by CRhinoGetObject::SetGeometryFilter.
      The geometry attribute filter is used to require the selected
      geometry to have certain attributes (open, closed, etc.).
      The default attribute filter permits selection of all types of 
      geometry.
  
    Parameters:
      geometry_attribute_filter - [in] an unsigned int made by 
          bitwise or-ing ( | ) values from the GEOMETRY_ATTRIBUTE_FILTER
          enum. If you pass 0, then all geometry attributes will 
          be acceptable.

    Example:
      If you want to be able to select open curves or solids, then
      you would make a call like

         CRhinoGetObject go;
         unsigned int geometry_type_filter = 0;
         unsigned int geometry_attribute_filter = 0;
         geometry_type_filter |= CRhinoGetObject::curve;
         geometry_type_filter |= CRhinoGetObject::surface_object;
         geometry_type_filter |= CRhinoGetObject::polysurface_object;
         geometry_attribute_filter |= CRhinoGetObject::closed_curve;
         geometry_attribute_filter |= CRhinoGetObject::solid_object;
         go.SetSetGeometryFilter( geometry_type_filter );
         go.SetGeometryAttributeFilter( geometry_attribute_filter );

    See Also:
      CRhinoGetObject::GeometryAttributeFilter.
      CRhinoGetObject::SetGeometryFilter.
      CRhinoGetObject::GeometryFilter.
  */
  void SetGeometryAttributeFilter( 
           unsigned int geometry_attribute_filter
           );

  /*
    Description:
      The geometry attribute filter provides a secondary filter
      can be used to restrict which objects can be selected.  
      Control of the type of geometry (points, curves, surfaces,
      meshes, etc.) is provided by CRhinoGetObject::SetGeometryFilter.
      The geometry attribute filter is used to require the selected
      geometry to have certain attributes (open, closed, etc.).
      The default attribute filter permits selection of all types of 
      geometry.
  
    Returns:
      An unsigned int "object_attribute_filter".  If the bit from an
      GEOMETRY_ATTRIBUTE_FILTER enum value is set in the geometry_attribute_filter,
      then GetObjects() will permit selection of object's
      that have that geometric attribute.  If geometry_attribute_filter
      is 0xFFFFFFFF, then GetObjects() can select all types
      of geometry.

    Example:
      If (GeometryAttributeFilter() | closed_curve) != 0, then 
      any curve selected by GetObjects() will be closed.

    See Also:
      CRhinoGetObject::SetGeometryAttributeFilter.
      CRhinoGetObject::SetGeometryFilter.
      CRhinoGetObject::GeometryFilter.
  */
  unsigned int GeometryAttributeFilter() const;

  /*
  Description:
    Checks geometry to see if it can be selected.
    The default always returns true.
    Override to provide fancy filtering.
  Parameters:
    object - [in] parent object being considered.
    geometry - [in] geometry being considered.
    component_index - [in] if >= 0, geometry is a proper
       sub-part of object->Geometry() with component_index.
  Remarks:
    The default simply returns true.  All obvious geometry
    filter checks have been performed by the time
    CustomGeometryFilter() is called.
  */
  virtual
  bool CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
    ) const;

  /*
  Description:
    Checks geometry to see if it passes the basic
    GeometryAttributeFilter.
  Parameters:
    object - [in] parent object being considered.
    geometry - [in] geometry being considered.
    component_index - [in] if >= 0, geometry is a proper
       sub-part of object->Geometry() with component_index.
  Remarks:
    The default simply checks that the geometry passes 
    the filter returned by GeometryAttributeFilter().
  See Also:
    CRhinoGetObject::EnablePostSelect
    CRhinoGetObject::PassesGeometryTypeFilter
  */
  bool PassesGeometryAttributeFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
    ) const;

  /*
  Description:
    Control the pre selection behavior GetObjects.
  Parameters:
    bEnablePreSelect - [in] if true, pre-selection is enabled.

    bIgnoreUnacceptablePreSelectedObjects - [in] If true and
         some acceptable objects are pre-selected, then any 
         unacceptable pre-selected objects are ignored.
         If false and any unacceptable are pre-selected,
         then the user is forced to post-select.

  Remarks:
    By default, if valid input is pre-selected when GetObjects()
    is called, then that input is returned and the user is not 
    given the opportunity to post-select.  If you want to force
    the user to post-select, then call EnablePreSelect(false).

  See Also:
    CRhinoGetObject::EnablePostSelect
    CRhinoGetObject::EnableDeselectAllBeforePostSelect
  */
  void EnablePreSelect(
          BOOL32 bEnablePreSelect = true,
          BOOL32 bIgnoreUnacceptablePreSelectedObjects = true
          );

  /*
  Description:
    Control the availability of post selection in GetObjects.

  Parameters:
    bEnablePostSelect - [in]

  Remarks:
    By default, if no valid input is pre-selected when GetObjects
    is called, then the user is given the chance to post select.
    If you want to force the user to pre-select, 
    then call EnablePostSelect(false).

  See Also:
    CRhinoGetObject::EnableOneByOnePostSelect
    CRhinoGetObject::EnablePreSelect
    CRhinoGetObject::EnableDeselectAllBeforePostSelect
  */
  void EnablePostSelect(
          BOOL32 bEnablePostSelect = true 
          );

  /*
  Description:
    Controls the deselection of preselected input when
    no valid input is pre-selected.

  Parameters:
    bDeselectAllBeforePostSelect - [in]

  Remarks:
    By default, if post selection is enabled and no valid input
    is pre-selected when GetObjects is called, then anything 
    that was preselected is deselected before post selection 
    begins.
    If there are objects selected and highlighted from a previous 
    operation that you want to have stay that way when GetObject()
    is called, Call EnableDeselectAllBeforePostSelect( false) to
    make that happen.  Otherwise the second GetObject will deselect 
    and unhighlight the already seelcted and highlighted objects.

  See Also:
    CRhinoGetObject::EnablePreSelect
    CRhinoGetObject::EnablePostSelect
  */
  void EnableDeselectAllBeforePostSelect(
          bool bDeselectAllBeforePostSelect = true 
          );

  /*
  Returns:
    True if pre-selected input will be deselected before
    post-selection begins when no pre-selected input is valid.
  Remarks:
    The default is true.
  See Also:
    CRhinoGetObject::EnablePreSelect
    CRhinoGetObject::EnableDeselectAllBeforePostSelect
  */
  bool DeselectAllBeforePostSelect() const;

  /*
  Description:
    If you want to force objects to be post selected one by one, 
    then enable one-by-one post selection by calling 
    EnableOneByOnePostSelect(true).
  Parameters:
    b - [in] if true, GetObjects() one-by-one post selection mode
             will be used.  If false, the selection mode specified
             by EnablePostSelect()/EnablePostSelect() will be used.
  Remarks:
    By default, CRhinoGetObject::GetObjects() permits multiple objects
    to be selected using any combination of single pick selection and
    window crossing selection.
  See Also:
    CRhinoGetObject::EnablePostSelect
  */
  void EnableOneByOnePostSelect(
    BOOL32 bEnableOneByOnePostSelect = true
    );

  /*
  Description:
    In one-by-one post selection, the user is forced
    to select objects by post picking them one at a time.
  Returns:
    true if one-by-one post selection mode is enabled.
  See Also:
    CRhinoGetObject::EnableOneByOnePostSelect
  */
  bool OneByOnePostSelect() const;

  /*
  Description:
    By default, CRhinoGetObject::GetObjects will permit a user
    to select sub-objects (like a curve in a b-rep or a curve 
    in a group, or member in a block instance). 
    If you only want the user to select "top" level
    objects, then call EnableSubObjectSelect(false).
  Parameters:
    bEnableSubObjectSelect - [in] 
       true to enable sub-object selection
       false to disable sub-object selection.
  See Also:
    CRhinoGetObject::SubObjectSelect, CRhinoGetObject::BlockSubObjectSelect
  */
  void EnableSubObjectSelect(
          BOOL32 bEnableSubObjectSelect=true
          );

  /*
  Description:
    By default, CRhinoGetObject::GetObjects will permit a user
    to select sub-objects (like a curve in a b-rep or a curve
    in a group). If you only want the user to select "top" level
    objects, then call EnableSubObjectSelect(false, false).
    This is the expert version that allows enabling/disabling
    normal object and block subobject selection separately, 
    used by transform commands.
  Parameters:
    bEnableSubObjectSelect - [in]
      true to enable sub-object selection of objects other than block instances
      false to disable sub-object selection.
    bEnableBlockSubObjectSelect - [in]
      true to enable sub-object selection of block instances
      false to disable block sub-object selection.
  See Also:
    CRhinoGetObject::SubObjectSelect, CRhinoGetObject::BlockSubObjectSelect
  */
  void EnableSubObjectSelect(BOOL32 bEnableSubObjectSelect, BOOL32 bEnableBlockSubObjectSelect);

  /*
  Description:
    By default, CRhinoGetObject::GetObjects will permit a user
    to select sub-objects (like a curve in a b-rep or a curve 
    in a group). If you only want the user to select "top" level
    objects, then call EnableSubObjectSelect(false) or EnableSubObjectSelect(false, true/false).
  Returns:
    true if selection of sub-objects is permitted.
  See Also:
    CRhinoGetObject::EnableSubObjectSelect
  */
  bool SubObjectSelect() const;

  /*
  Description:
    By default, CRhinoGetObject::GetObjects will permit a user
    to select block sub-objects (members in the top level block). 
    If you only want the user to select "top" level block instances,
    call EnableSubObjectSelect(false) or EnableSubObjectSelect(true/false, false).
  Returns:
    true if selection of block sub-objects is permitted.
  See Also:
    CRhinoGetObject::EnableSubObjectSelect
  */
  bool BlockSubObjectSelect() const;

  /*
  Description:
    By default, if a call to GetObject is permitted to select
    different parts fo the same object, like a polysurface and
    an edge of that polysurface, then the top-most object is 
    automatically selected.  If you want the choose-one-object
    mechanism to include pop up in these cases, then call 
    EnableChooseOneQuestion(true) before calling GetObjects().
  Parameters:
    bEnableChooseOneQuestion - [in]
        By default, if a call to GetObject is permitted to select
        different parts fo the same object, like a polysurface and
        an edge of that polysurface, then the top/bottom-most object
        is automatically selected.  If you want the choose-one-object
        mechanism to include pop up in these cases, then call 
        EnableChooseOneQuestion(true) before calling GetObjects().
  See Also:
    CRhinoGetObject::ChooseOneQuestion
  */
  void EnableChooseOneQuestion( BOOL32 bEnableChooseOneQuestion=true );

  bool ChooseOneQuestion() const;

  /*
  Description:
    By default, if a call to GetObject is permitted to select
    different parts fo the same object, like a polysurface,
    a surface and an edge, then the top-most object is 
    preferred.  (polysurface beats face beats edge).  If you
    want the bottom most object to be preferred, then call 
    EnableBottomObjectPreference(true) before calling GetObjects().
  Parameters:
    bEnableBottomObjectPreference - [in]
  See Also:
    CRhinoGetObject::ChooseOneQuestion
  */
  void EnableBottomObjectPreference( BOOL32 bEnableBottomObjectPreference=true );

  bool BottomObjectPreference() const;


  //////////
  // By default, groups are ignored in GetObject.  If you
  // want your call to GetObjects() to select every object 
  // in a group that has any objects selected, then enable
  // group selection.
  void EnableGroupSelect(BOOL32=true);

  bool GroupSelect() const;

  /*
  Parameters:
    bEnable - [in]
      true -
        if a subd (or a subd component) cannot be selected, but a brep (or brep component) can be selected,
        then automatically create and use a proxy brep.
      false
        Never create or use a proxy brep for a subd object.
  Remarks:
    Default = true;
  */
  void EnableProxyBrepFromSubD(bool bEnable);

  /*
  Returns:
    true -
      if a subd (or a subd component) cannot be selected, but a brep (or brep component) can be selected,
      then automatically create and use a proxy brep.
    false
      Never create or use a proxy brep for a subd object.
  Remarks:
    Default = true;
  */
  bool ProxyBrepFromSubD() const;


  /*
  Description:
    The getobject filters for mesh and SubD components are shared. In some commands that 
    deal exclusively with meshes and mesh components, it is necessary to prohibit SubD 
    and SubD component selections.
  Parameters:
    bEnable - [in]
      true - (default)
        Selection of SubD objects and SubD components is determined by the standard filters.
      false -
        Selection of SubD objects and SubD components is prohibited. This is useful in commands that
        use the mesh component filters and only want to select meshes and mesh components.
  Remarks:
    Default = true;
  */
  void EnableSubDSelections(bool bEnable);

  /*
  Returns:
    true - (default)
      Selection of SubD objects and SubD components is determined by the standard filters.
    false -
      Selection of SubD objects and SubD components is prohibited. This is useful in commands that
      use the mesh component filters and only want to select meshes and mesh components.
  Remarks:
    Default = true;
  */
  bool SubDSelections() const;



  /*
  Description:
    The getobject filters for mesh and SubD components are shared. In some commands that
    deal exclusively with meshes and mesh components, it is necessary to prohibit SubD
    and SubD component selections.
  Parameters:
    bEnable - [in]
      true - (default)
        Selection of mesh objects and mesh components is determined by the standard filters.
      false -
        Selection of mesh objects and mesh components is prohibited. This is useful in commands that
        use the mesh component filters and only want to select SubDs and SubD components.
  Remarks:
    Default = true;
  */
  void EnableMeshSelections(bool bEnable);

  /*
  Returns:
    true - (default)
      Selection of mesh objects and mesh components is determined by the standard filters.
    false -
      Selection of mesh objects and mesh components is prohibited. This is useful in commands that
      use the mesh component filters and only want to select SubDs and SubD components.
  Remarks:
    Default = true;
  */
  bool MeshSelections() const;
  
  
  //////////
  // By default, any object selected during a command becomes
  // part of the "previous selection set" and can be reselected
  // by the SelPrev command.  If you need to select objects
  // but do not want them to be selected by a subsequent call
  // to SelPrev, then call EnableSelPrev(false).
  void EnableSelPrev(BOOL32=true); // default is true


  //////////
  // By default, any object post-pick selected by GetObject() 
  // is highlighted.  If you want to post-pick objects and
  // not have them automatically highlight, then call
  // EnableHighlight(false).
  void EnableHighlight(BOOL32=true); // default is true

  bool Highlight() const;


  //////////
  // By default, reference objects can be selected.
  // If you do not want to be able to select reference
  // objects, then call EnableReferenceObjectSelect(false).
  void EnableReferenceObjectSelect(BOOL32=true);

  bool ReferenceObjectSelect() const;


  //////////
  // By default, post selection will select objects with
  // grips on.  If you do not want to be able to post select 
  // objects with grips on, then call EnableIgnoreGrips(false).
  // The ability to preselect an object with grips on is
  // determined by the value returned by the virtual
  // CRhinoObject::IsSelectableWithGripsOn.
  void EnableIgnoreGrips(bool=true);
    
  /*
  Description:
    By default, when CRhinoGetObject::GetObjects is 
    called with minimum_number > 0 and 
    maximum_number = 0, the command prompt automatically
    includes "Press Enter when done" after the user has 
    selected at least minimum_number of objects.  
    If you want to prohibit the addition of the
    "Press Enter when done", then call
    EnablePressEnterWhenDonePrompt(false).
  Parameters:
    bEnable - [in]
  Returns:
    Previous value.
  */
  bool EnablePressEnterWhenDonePrompt(bool bEnable=true);


  /*
  Description:
    The default prompt when EnablePressEnterWhenDonePrompt is
    enabled is "Press Enter when done". Use this function
    to specify a different string to be appended.
  Parameters:
    sSecondPrompt - [in]
  */
  void SetPressEnterWhenDonePrompt(const wchar_t* sSecondPrompt);

  /*
  Description:
    Allow selecting objects that are already selected.  By default,
    GetObjects() disallows selection of objects that are already 
    selected to avoid putting the same object in the selection set
    more than once.
    Calling EnableAlreadySelectedObjectSelect( true) overrides
    that restriction and allows selected objects to be selected and
    returned by GetObjects.
    This is useful because, coupled with the return immediately mode
    of GetObjects( 1, -1), it is possible to select a selected object
    to deselect when the selected objects are being managed outside
    GetObjects() as in the case of CRhinoPolyEdge::GetEdge().
  Parameters:
    bEnable - [in]
  */
  void EnableAlreadySelectedObjectSelect( bool bEnable = true);

  /*
  Description:
    Returns the current status of whether GetObjects() can select
    objects that are already selected, as set by 
    EnableAlreadySelectedObjectSelect()
  Returns:
    true if Selected objects can be selected and returned
    false if they can't.
  */
  bool AlreadySelectedObjectSelect() const;

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Call GetObjects() to get a list of objects.
  //

  /*
  Description:
    Call to select objects.
  Parameters:
    minimum_number - [in] minimum number of objects to select.
    maximum_number - [in] maximum number of objects to select.
       If 0, then the user must press enter to finish object selection.
       If -1, then object selection stops as soon as there are at
       least minimum_number of object selected.
       If >0, then the picking stops when there are maximum_number
       objects.  If a window pick, crossing pick, or Sel* command 
       attempts to add more than maximum_number, then the attempt
       is ignored.
  Example:
          // pick 3 or more curves and/or meshes
          CRhinoGetObject go;
          double x = 0.0;
          for (;;)
          {
            go.SetCommandPrompt("Select curves and meshes or type a number.\n");
            go.SetGeometryFilter( CRhinoGetObject::curve_object | CRhinoGetObject::mesh_object );
            go.AcceptNumber( true );
            go.GetObject(3,0);
            if ( go.Result() == CRhinoGet::number )
            {
              x = go.Number();
              continue;
            }
            break;
          }
          if ( go.Result() == CRhinoGet::object )
          {
            int i, object_count = go.ObjectCount();
            for ( i = 0; i < object_count; i++ )
            {
               CRhinoObjRef obj_ref = go.Object(i);
            }
          }
  See Also:
    CRhinGetObject::EnablePressEnterWhenDonePrompt
  Returns:
    CRhinoGet::success - objects selected.
    CRhinoGet::cancel - user pressed ESCAPE to cancel the get.
    See CRhinoGet::result for other possible values that may
    be returned when options, numbers, etc., are acceptable
    responses.
  */
  CRhinoGet::result GetObjects(
    int minimum_number = 1,
    int maximum_number = 1
    );


  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Use value returned by GetObject() to determine what
  //         function to call to get the input.
  //
  //         GetObject() return value   function
  //           CRhinoGet::option         CRhinoGet::Option()
  //           CRhinoGet::number         CRhinoGet::Number()
  //           CRhinoGet::object          CRhinoObject::Object()
  //
  // When pieces of instance references are selected, the
  // CRhinoObjRef contains reference counted proxy geometry. 
  // You need to keep either the CRhinoGetObject class or the
  // CRhinoObjRef in in scope until you have finished using 
  // the geometry it references.  After the destruction of 
  // the CRhinoGetObject class and the last CRhinoObjRef,
  // the proxy geometry is deleted.  At that point any
  // references or pointers to the proxy geometry will be 
  // invalid.
  int       ObjectCount() const; // returns number of objects selected
  CRhinoObjRef Object(int) const;   // 0 based index

  /*
  Returns:
    True if CRhinoGetObjects::GetObjects() returned preselected objects.
  */
  bool ObjectsWerePreSelected() const;


  //////////////////////////////////////////////////////////////////
  //
  // Implementation
  //

  /*
  Description:
    Each instanct of CRhinoGetObject has a unique runtime serial
    number that is used to identify object selection events associated
    with that instance.
  */
  unsigned int SerialNumber() const;

  /*
  Description:
    By default the picked object list is cleared when
    CRhinoGetObject::GetObjects() is called.  If you are
    reusing a CRhinoGetObject class and do not want the
    existing object list cleared when you call GetObjects,
    then call EnableClearObjectsOnEntry(false) before
    calling GetObjects().
  Parameters:
    bClearObjectsOnEntry - [in]
  See Also:
    CRhinoGetObject::EnableUnselectObjectsOnExit
    CRhinoGetObject::ClearObjects
  */
  void EnableClearObjectsOnEntry(bool bClearObjectsOnEntry=true);

  /*
  Description:
    By default any objects in the object list are
    unselected when CRhinoGetObject::GetObjects() exits with
    any return  code besides CRhinoGet::object.  If you want
    to leave the objects selected when non-object input
    is returned, then call EnableClearObjectsOnExit(false)
    before calling GetObjects().
  Parameters:
    bClearObjectsOnExit - [in]
  Remarks:
    If you pass false to this function, then you are
    responsible for making sure the selection state
    of the objects in the object list is correct.
  See Also:
    CRhinoGetObject::EnableClearObjectsOnEntry
    CRhinoGetObject::ClearObjects
  */
  void EnableUnselectObjectsOnExit(bool bUnselectObjectsOnExit=true);

  /*
  Description:
    This is a low level geek tool for emptying the selected object
    list.  Calling GetObjects() automatically clears this list, so
    the situations where this function is needed are very rare.
  See Also:
    CRhinoGetObject::EnableClearObjectsOnEntry
    CRhinoGetObject::EnableUnselectObjectsOnExit
  */
  void ClearObjects();

  /*
  Description:
    This is a low level geek tool for digging through the pick
    list.  The situations where this function is needed are rare
    and no support is available.
  */
  const CRhinoObjRefArray& PickList() const;

  // Added 30 June, 2010
  bool InactiveDetailPick() const;
  void EnableInactiveDetailPick(bool enable = true);

  bool IgnoreHistoryDependents() const;
  void EnableIgnoreHistoryDependents(bool bIgnoreHistoryDependents);

  // Posts an event to Rhino that object selection has changed.
  void PostObjectSelectionChangedEvent(const CRhinoView* view) const;

  /*
  Description:
    Call GetSubDComponents() after calling GetObjects(...) to get a list
    of the subd components that were selected.
  Parameters:
    subd_runtime_serial_number - [in]
      If subd_runtime_serial_number > 0 and < 0xFFFFFFFFFFFFFFFFllu,
      then only components from that subd will be added to 
    component_type - [in]
      If component_type is ON_COMPONENT_INDEX::TYPE::subd_vertex,
      ON_COMPONENT_INDEX::TYPE::subd_edge, or ON_COMPONENT_INDEX::TYPE::subd_face, 
      then only components of that type will be added.
  Returns:
    Number of components added to subd_component_list.
  */
  int GetSubDComponentList(
    ON__UINT64 subd_runtime_serial_number,
    ON_COMPONENT_INDEX::TYPE component_type,
    class ON_SubDComponentRefList& subd_component_list
  ) const;

private:
  friend class CRhinoPickContext;
  void SetSelectionInfo( CRhinoObjRef& );
  unsigned int m_ui_geometry_filter; // default = 0;
  int  m_geometry_filter; // GEOMETRY_TYPE_FILTER bits (default is 0)
  int  m_geometry_attribute_filter; // GEOMETRY_ATTRIBUTE_FILTER bits (default is 0)
  bool m_bEnablePreSelect;       // default is true;
  bool m_bIgnoreUnacceptablePreSelectedObjects; // default is true;
  bool m_bEnablePostSelect;      // default is true;
  bool m_bEnableOneByOnePostSelect; // default is false;
  bool m_bEnableSubObjectSelect; // default is true;
  bool m_bEnableBlockSubObjectSelect; // default is true;
  bool m_bEnableGroupSelect;     // default is true;
  bool m_bEnableSelPrev;         // default is true;
  bool m_bEnableHightlight;      // default is true;
  bool m_bEnableReferenceObjectSelect; // default is true;
  bool m_bDeselectAllBeforePostSelect; // default is true;
  bool m_bEnableChooseOneQuestion; // default is false;
  bool m_bEnableBottomObjectPreference; // default is false;
  bool m_bEnableIgnoreGrips; // default is true (post select only)
  bool m_bEnablePressEnterWhenDonePrompt; // default is true
  bool m_bClearObjectsOnEntry; // default is true
  bool m_bUnselectObjectsOnExit; // default is true
  bool m_bAlreadySelectedObjectSelect; // default is false
  // set to true if GetObjects() returns objects that were prepicked.
  bool m_bObjectsWerePrepicked;

  // Added 30 June, 2010
  bool m_bInactiveDetailPick;
  
  // Added 6 April 2011 to make it easy to avoid picking
  // objects with history records.
  bool m_bIgnoreHistoryDependents;

private:
  bool m_bProhibitProxyBrepFromSubD = false;

  // Because we ran out of bits on a 32 bit filter enum, SubDs and meshes
  // both use ON::object_type::meshvertex_filter, ON::object_type::meshedge_filter, ON::object_type::meshface_filter
  // for specifying subobjet selection of vertices, faces and edges.
  // When a command wants only SubD/mesh components, it may use
  // EnableMeshSelections(false)/EnableSubDSelections(false) to prevent
  // selection of the unwanted type of vertices,edges,faces.
  bool m_bProhibitSubDSelections = false;
  bool m_bProhibitMeshSelections = false;

private:
  // 1 Dec 2016, Mikko, RH-36345:
  // Maximum number of objects allowed in the pending GetObjects() call, otherwise 0. 
  int m_max_object_count;

private:
  ON_wString m_sPressEnterWhenDonePrompt; // default is "Press Enter when done"
  ON_ClassArray<CRhinoObjRef> m_objects; // selected objects

private:
  // "raw" pick list
  friend class CRhinoView;
  friend class CRhViewBase;
  friend int RhSelCmdHelper(unsigned int, bool(*)(const CRhinoObject*), bool, bool, bool, bool, bool, bool, bool);
  friend int RhSelCmdHelper(unsigned int, bool(*)(const CRhinoObject*), bool, bool, bool, bool, bool, bool, bool, bool(*)(const CRhinoObject*, ON_SimpleArray<ON_COMPONENT_INDEX>&));
  friend CRhinoCommand::result RhSelSetHelper(CRhinoDoc&,bool,int);
  friend class CRhRegionSelectHelper;

  // Added 8-27-2003 by Dale Fugier (TRR 11353)
  friend const CRhinoObjRefArray* RhinoCurrentGetObjectPickList(void);

  // 8 May 2018, Mikko, RH-45657:
  friend int CRhinoCurveObject::Pick(const CRhinoPickContext&, CRhinoObjRefArray&) const;

  CRhinoObjRefArray m_pick_list;
  void AddBuiltInOptions(void*);
  int ProcessBuiltInOptions(void*);

  // Set to true if SetGeometryFilter() or EnableSubObjectSelect()
  // is called before GetObject().  If m_bChangedSetFilterOrSubSelect
  // is false, then GetObject() will only pick top level objects
  // unless one of the hidden "crv", "srf", etc. options is used.
  int  m_bChangedSetFilterOrSubSelect; 

private:
  // runtime values used by GetObject to respond to proper rhino get event
  int m_serial_number;
  class CGetObjectDynamicDraw* m_pDynamicDraw;
  ON__UINT_PTR m_sdk_reserved = 0;
};

/*
Description:
  Visual user interface tool to choose one object from a list.
Parameters:
  view       - [in] view where ambiguous pick occurred
  view_point - [in] window coordinates where ambiguous pick occurred.
  objref_count - [in] number of object references in objref_array[]
  objref_array - [in]
  selected_object_index - [out] index of selected object
  default_object_index - [in] if >= 0 and < objref_count, then this
     object is the default.
  visibility_hint - [in] By default, in a shaded view point pick,
     RhinoChooseOneObject() will ask the user show only visible
     candidates.  If objects are were visible when the pick occurred
     but are no longer visible, then pass them in here.  For example,
     if a wire frame object was selected when the pick occurred but
     was deselected before the call to RhinoChooseOneObject(), then
     pass its point in this array.
  bShowAllOption - [in] When true, "All" option is added to the list
     and when selected the return code is 4.

Returns:
  @untitle table
  0     none selected
  1     one selected
  2     user canceled
  3     one object selected - selection was automatically reduced to one by culling
  4     "All" option selected

*/
RHINO_SDK_FUNCTION
int RhinoChooseOneObject(
      CRhinoView* view, 
      POINT view_point,
      int objref_count,
      const CRhinoObjRef* objref_array,
      int* selected_object_index,
      int default_object_index = -1,
      ON_SimpleArray<CRhinoObject*> * visibility_hint = 0,
      bool bShowAllOption = false
      );


//////////////////////////////////////////////////////////////////////////
//
// CRhinoMeshRef is used to return mesh objects
//
/*
CLASS
CRhinoMeshRef
*/
class RHINO_SDK_CLASS CRhinoMeshRef : public CRhinoObjRef
{
public:
  CRhinoMeshRef();
  CRhinoMeshRef(CRhinoObject*, ON_Mesh*, 
            BOOL32 // true if geometry is a proxy object that needs to be deleted
            );
  ~CRhinoMeshRef();
  CRhinoMeshRef(const CRhinoMeshRef&);
  CRhinoMeshRef& operator=(const CRhinoMeshRef&);

  const ON_Mesh* Mesh() const; // mesh object
};

/*-------------------------------------------------------------------
CLASS
  CRhinoGetExportMeshes

OVERVIEW TEXT
  Interactively gets meshes for commands that export meshes to
  other file formats.
  
KEYWORDS
  mesh, export, get_object
-------------------------------------------------------------------*/
class RHINO_SDK_CLASS CRhinoGetMeshes : public CRhinoGet
{
public:
  CRhinoGetMeshes();
  ~CRhinoGetMeshes();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Interactively get meshes.
  //
  BOOL32 GetExportMeshes(
         BOOL32=false // set to true to mesh only selected objects
         );

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: call MeshCount() to see how many meshes you have
  //
  int MeshCount() const;

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Mesh(i) (0 <= i < MeshCount() to get the meshes.  Note 
  //         that the meshes will automatically be destroyed by
  //         ~CRhinoGetMeshes(), so you must copy them if you need
  //         them after ~CRhinoGetMeshes().
  //
  CRhinoMeshRef Mesh(int);

private:
  CRhinoGetMeshes(const CRhinoGetMeshes&);
  CRhinoGetMeshes& operator=(const CRhinoGetMeshes&);
  void Cleanup();
  void* m_p;
  int m_num_created;
  int m_mesh_count;
};

