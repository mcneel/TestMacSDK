//
// Copyright (c) 1993-2015 Robert McNeel & Associates. All rights reserved.
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
  CRhinoSubDDisplay stores information necessary to display and pick
  ON_SubD objects.
  A CRhinoSubDDisplay can be created from a CRhinoSubDObject or a stand alone
  ON_SubD.  
  It is a reference counted object and the information it can be 
  efficiently and safely passed as a parameter and saved.
*/
class RHINO_SDK_CLASS CRhinoSubDDisplay
{
public:
  static const CRhinoSubDDisplay Empty;

  CRhinoSubDDisplay() = default;
  ~CRhinoSubDDisplay() = default;
  CRhinoSubDDisplay(const CRhinoSubDDisplay&) = default;
  CRhinoSubDDisplay& operator=(const CRhinoSubDDisplay&) = default;
  CRhinoSubDDisplay( CRhinoSubDDisplay&& ) ON_NOEXCEPT;
  CRhinoSubDDisplay& operator=( CRhinoSubDDisplay&& );


  ////////////////////////////////////////////////////////////////
  //
  // Creation
  //
  static CRhinoSubDDisplay Create( const class CRhinoSubDObject& subd_object );
  static CRhinoSubDDisplay Create( 
    const class CRhinoSubDObject& subd_object,
    ON_SubDRef display_subd_ref
  );
  static CRhinoSubDDisplay Create( ON_SubDRef subd_ref );
  static CRhinoSubDDisplay Create( const ON_SubD& subd );

  bool IsEmpty() const;

  /*
  Description:
    Removes all contents and references.
  */
  void Clear();
  
  /*
  Returns:
    If this CRhinoSubDDisplay was created from a CRhinoSubDObject or an ON_SubDRef,
    then a nonempty ON_SubDRef is returned.
    Otherwise ON_SubDRef::Empty is returned. 
  */
  ON_SubDRef SubDRef() const;

  /*
  Returns:
    If this CRhinoSubDDisplay was created from a CRhinoSubDObject or an ON_SubDRef,
    then a reference to the ON_SubD is returned.
    Otherwise ON_SubD::Empty is returned. 
  */
  const ON_SubD& SubD() const;

  /*
  Returns:
    If this CRhinoSubDDisplay is for a CRhinoSubDObject, then
    the runtime serial number of the CRhinoSubDObject is returned.
    Otherwise, 0 is returned.
  */
  unsigned int RhinoSubDObjectSerialNumber() const;

  /*
  Returns:
    If this CRhinoSubDDisplay is for a CRhinoSubDObject and that
    object is part of a CRhinoDoc, then id of the CRhinoSubDObject
    is returned.
    Otherwise, ON_nil_uuid is returned.
  */
  ON_UUID RhinoSubDObjectId() const;

  /*
  Returns:
    If this CRhinoSubDDisplay is for a CRhinoSubDObject and that
    object is part of a CRhinoDoc, then runtime serial number of
    the CRhinoDoc is returned.
    Otherwise, 0 is returned.
  */
  unsigned int RhinoDocSerialNumber() const;

  /*
  Returns:
    CRhinoSubDObject::Cast(
      CRhinoObject::FromRuntimeSerialNumber(
        RhinoSubDObjectSerialNumber(),
        RhinoDocSerialNumber()
        ));
  Remarks:
    The pointer returned by SubDObject() may be nullptr.
    If not, the pointer can safely be used in the scope of a function
    that does not allow other Rhino commands to run. For 
    a persistent reference, use  RhinoSubDObjectSerialNumber()
    and RhinoDocSerialNumber().
  */
  const class CRhinoSubDObject* SubDObject() const;

  /*
  Description:
    Get an ON_SubDMeshFragmentIterator for displaying the subd object. 
    This fragment iterator is used for both "wireframe" and "shaded" 
    display and selection. It can produce both the smooth surface and
    control polygon meshes. 
  Parameters:
    bCreateIfEmpty - [in]
      If the limit mesh fragments do not exist and bCreateIfEmpty is true,
      then they will be created.
  Returns:
    An ON_SubDMeshFragmentIterator. 
    The fragments are managed by the subd and have information for both
    surface and control polygon display. Internal shared pointers insure
    that the subd will stay in scope for the lifetime of the fragment iterator.
    The value of frit.SubDAppearance() (surface/control polygon) is set from
    the subd. It can be changed by calling frit.SetSubDAppearanceOveride().
  Example:
    ON_SubDMeshFragmentIterator frit = subddisplay.FragmentIterator()
    for ( frag = frit.FirstFragment(); nullptr != frag; frag = frit.NextFragment()) ...
  */
  ON_SubDMeshFragmentIterator MeshFragmentIterator(bool bCreateIfEmpty) const;

  /*
  Description:
    Update the SubD's mesh fragments if the SubD has changed since
    the last call to Update().
  */
  void Update();

  /*
  Description:
    This function is used when mutable appearance is chaged. There are two
    current situations. Toggling (flat/smooth a.k.a crunchy/creamy) and changing
    default texture coordinates.
  */
  void ForceUpdate();

  ON_DEPRECATED_MSG("OBSOLETE. ContentSerialNumber() always returns 0")
  unsigned int ContentSerialNumber() const;


  /// <summary>
  /// This function determines how the display code treats an 
  /// interior SubD edge. (Interior edges with 2 faces).
  /// 
  /// Rhino display code has to partition the set of interior edges into
  /// 2 disjoint sets, "smooth interior edges" and "other interior edges."
  /// 
  /// Smooth SubD edges with zero sharpness belong in the "smooth interior edges" set.
  /// 
  /// Crease SUbD edges belong in the "other interior edges" set.
  /// 
  /// Sharp SubD edges (which smooth edges with nonzero sharpness) go into 
  /// "other interior edges" as of Feb 21, 2023.
  /// This is done so the ShowEdges command and SubD object display settings can be used to
  /// find sharp edges.
  /// </summary>  
  /// <param name="edge"></param>
  /// <returns>True if the display code, ShowEdge command, etc., consider this edge to be a smooth interior edge.</returns>
  static bool DrawAsSmoothInteriorEdge(const ON_SubDEdge* edge);

private:
  // NO friends!
  //
  // Information used to get safe runtime access to the CRhinoSubDObject
  // when this class is used to display one.
  unsigned int m_subd_object_sn = 0;
  mutable unsigned int m_doc_sn = 0;
  mutable ON_UUID m_subd_object_id = ON_nil_uuid;

  // Safe access to the ON_SubD and insure the core subd exists
  // until no process are using it.
  ON_SubDRef m_subd_ref;

#pragma warning( push )
#pragma warning( disable : 4251 ) 
  // disable C4235 for private m_sp
private:
  // Safe access to CRhSubDDisplay implementation and insure it
  // exists until no processes are using it.
  std::shared_ptr < class CRhSubDDisplay > m_sp;
#pragma warning( pop )

public:
  // CRhSubDDisplay is used internally.  
  // The definition of CRhSubDDisplay is intentionally not part of the SDK.
  // Plug-ins should ignore these functions.
  const class CRhSubDDisplay& Display() const;
  class CRhSubDDisplay* Display( bool bCreateIfNeeded );
};


class RHINO_SDK_CLASS CRhinoSubDDisplayAttributes
{
public:
  // CRhinoSubDDisplayAttributes::Default draws the default presentation of
  // subdivision objects in Rhino.
  static const CRhinoSubDDisplayAttributes Default;

  CRhinoSubDDisplayAttributes() = default;
  CRhinoSubDDisplayAttributes(const CRhinoSubDDisplayAttributes&);
  CRhinoSubDDisplayAttributes& operator=(const CRhinoSubDDisplayAttributes&);
  ~CRhinoSubDDisplayAttributes();

public:
  // Specify what to draw.

  // The edges are curves on the surface based on subd appearance (smooth/flat) settings
  bool m_bDrawEdges = true;

  // The surface is a shaded mesh based on subd appearance (smooth/flat) settings
  bool m_bDrawSurface = false;

  // The control net is made up of lines between edge control net points.
  // This setting ignores subd appearance settings so that a 
  // control net can be used to contrast with a smooth surface appearance.
  bool m_bDrawControlNet = false;

  // The surface net is made up of lines between edge surface points.
  // This setting ignores subd appearance settings so that a 
  // surface net can be used to contrast with a flat surface appearance.
  bool m_bDrawSurfaceNet = false;

private:
  unsigned m_reserved = 0U;

public:
  /////////////////////////////////////////////////////////
  //
  // Override default behavior defined by the display pipeline attributes
  // at drawing time.
  //
  void SetMaterialOverride(const class CDisplayPipelineMaterial*);

  void SetComponentToDraw(const ON_SubDComponentPtr& component);
  ON_SubDComponentPtr ComponentToDraw() const;

  void SetReflectionPlane(const class ON_PlaneEquation* pe);

  const class CRhSubDDisplayAttributesPrivate* PrivateData() const;
private:
  class CRhSubDDisplayAttributesPrivate* m_private = nullptr;
};

class RHINO_SDK_CLASS CRhinoSubDObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoSubDObject);

  /////////////////////////////////////////////////////////////////////////////
  //
  // Add a SubD object to a Rhino document
  //

  /*
  Description:
    This is the best way to add a Sub-D object with default object attributes
    to a Rhino document.

  Parameters:
    subd_ref - [in]
    rhino_doc_sn - [in]

  Returns:
    0: The input was not valid and the subd object was not added to the document.
    != 0: The runtime object serial number of the new CRhinoSubDObject.

  Example:

        ON_SubDRef subd_ref;
        ON_SubD& subd = subd_ref.NewSubD();

        // fill in subd
        ...

        unsigned int rhino_doc_sn = ...;
        unsigned int rhino_subd_object_sn = CRhinoSubDObject::AddToDocument(subd_ref,rhino_doc_sn);
  */
public:
  /*
  Return:
    true if SubD is visible.
    false in commercial Rhino 6, unless test commands have been turned on with the "T_E_S_T" command.
  */
  static bool SubDIsVisible();

public:
  static unsigned int AddToDocument(
    ON_SubDRef subd_ref,
    unsigned int rhino_doc_sn
    );
  

  /// <summary>
  /// Replace an existing Rhino object with a new Rhino subd object.
  /// </summary>
  /// <param name="subd_ref"></param>
  /// <param name="rhino_object_id">
  /// Id of the object to replace. If null, the SubD is added as a new Rhino document object.</param>
  /// <param name="bPreserveNwComponentSelectionState">
  /// If true, the compononent selection states in new_subd are used for the new object.
  /// If false, the compononent selection states of the object being replaced are used
  /// to set the component selection states of the new SubD object.
  /// </param>
  /// <param name="rhino_doc_sn"></param>
  /// <returns>
  /// The runtime Rhino document object serial number of the updated CRhinoSubDObject.
  /// </returns>
  static unsigned int AddToDocument(
    ON_SubDRef subd_ref,
    ON_UUID rhino_object_id,
    bool bUseNewSubDComponentSelectionState,
    unsigned int rhino_doc_sn
  );

  /*
  Description:
    This is the best way to add a Sub-D object with custom object attributes
    to a Rhino document.

  Parameters:
    subd_ref - [in]
    subd_attributes - [in]
    rhino_doc_sn - [in]

  Returns:
    0: The input was not valid and the subd object was not added to the document.
    != 0: The runtime object serial number of the new CRhinoSubDObject.
  */
public:
  static unsigned int AddToDocument(
    ON_SubDRef subd_ref,
    const ON_3dmObjectAttributes& subd_attributes,
    unsigned int rhino_doc_sn
    );
  
  /*
  Description:
    This is the best way to add a Sub-D object with custom object attributes
    to a Rhino document.

  Parameters:
    subd_ref - [in]
    subd_attributes - [in]
    rhino_doc_sn - [in]

  Returns:
    0: The input was not valid and the subd object was not added to the document.
    != 0: The runtime object serial number of the new CRhinoSubDObject.
  */
public:
  static unsigned int AddToDocument(
    ON_SubDRef subd_ref,
    const ON_3dmObjectAttributes* subd_attributes,
    CRhinoHistory* pHistory,
    bool bReference,
    unsigned int rhino_doc_sn
    );

public:
  /////////////////////////////////////////////////////////////////////////////
  //
  // C++ construction, etc.
  //

  CRhinoSubDObject();                              // assigns a new object UUID
  CRhinoSubDObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoSubDObject();
  CRhinoSubDObject(const CRhinoSubDObject&);            // copies source object UUID
  CRhinoSubDObject& operator=(const CRhinoSubDObject&); // copies source object UUID

  /////////////////////////////////////////////////////////////////////////////
  //
  // ON_SubD object access
  //
  
  /*
  Description:
    This is the best way to set the CRhinoSubDObject's subd.

  Remarks:
    This is a safe and reliable function.

  Example:
    Commands that create ON_SubD objects should do something like the following:

      ON_SubDRef subd_ref;

      ON_SubD& subd = subd_ref.NewSubD();

      // add information to the subd
      subd.Add(...)
    
      CRhinoSubDObject* subd_object = new CRhinoSubDObject();
      subd_object->SetSubD(subd_ref);

  */
  bool SetSubD( ON_SubDRef subd_ref );
  
  /*
  Description:
    Set the subd this CRhinoSubDObject references to a managed copy of src_subd. 
    Userdata is not copied from src_subd to the managed subd.
  Remarks:
    This is a safe and reliable function.
  */
  bool SetSubD( const ON_SubD& src_subd );

  /*
  Description:
    For expert users.  Avoid.
  Parameters:
    subd - [in]
      Points to a subd that was created on the default Rhino heap by a call to
      new ON_Sub(...).
      If this function returns true, then ~CRhinoSubDObject() will delete subd. 
  Remarks:
    If subd is on the stack, then Rhino will crash when ~CRhinoSubDObject() deletes the subd.
    If subd is on the heap, and some other code deletes subd, then Rhino will crash on the
    second attempt to delete subd.
    This function is provided so expert users can avoid copying large ON_SubD objects
    and precisely control the management of userdata attached to the ON_SubD.
  */
  bool SetSubDForExperts( ON_SubD* subd );

  /*
  Description:
    When you need to reference the ON_SubD geometry, the best
    way is to save an ON_SubDRef for the scope of your use. 
  Returns:
    An ON_SubDRef you can use to reference the ON_SubD geometry.
    As long as an ON_SubDRef exists, you can be assured the referenced
    ON_SubD exists.
  Example:
    ON_SubDRef subd_ref = subd_object->SubDRef();
    ...
    ... anything can happen including delete subd_object.
    ...
    subd_ref.SubD().WhatEver() reliably exists and is safe to use.
  */
  ON_SubDRef SubDRef() const;

  /*
  Description:
    For expert users.  Avoid and use SubDRef() instead.
    You must insure this CRhinoSubDObject is not deleted while
    you use the returned reference.  It better to use an ON_SubDRef.
  Returns:
    reference to the ON_SubD this CRhinoSubDObject uses for its geometry.
  */
  const ON_SubD& SubD() const;

  /*
  Description:
    Controls the appearance of all possible SubD displays
    (this object's SubD(), the dynamic display SubD, and the
    the static display SubD()).
  Returns:
    True if some display appearance changed.

   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts      
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  bool SetDisplayAppearanceForExperts(
    ON_SubDComponentLocation subd_appearance
  ) const;

  /*
  Description:
    For expert users who need to temporarily modify the display.
    For example, when the SubD object is being dynamically edited.

   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  void SetDynamicDisplayForExperts(
    ON_SubDRef dynamic_display_subd_ref
  ) const;

  /*
  Description:
    For expert users who are finished with a dynamic editing display.

   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  void ClearDynamicDisplayForExperts() const;

  /*
  Description:
    Controls the appearance of the dynamic display.

   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  void SetDynamicDisplayAppearanceForExperts(
    ON_SubDComponentLocation subd_appearance
  ) const;

  /*
  Description:
    Controls the appearance of the static display.

   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  void SetStaticDisplayAppearanceForExperts(
    ON_SubDComponentLocation subd_appearance
  ) const;

  /*
  Description:
    For expert users who need to temporarily modify the display.
    For example, when the SubD object is being dynamically edited.

   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  void SetStaticDisplayForExperts(
    ON_SubDRef static_display_subd_ref
  ) const;

  /*
   Remarks:
      NOTE WELL:
      When using the CRhinoSubDObject expert display functions like
        SetDisplayAppearanceForExperts
        ...DynamicDisplayForExperts
        ...StaticDisplayForExperts
      you must use CRhinoDoc::Regen() to update the views.
      (CRhinoDoc::Redraw() is not powerful enough to consistently
      force display buffers to update.)
  */
  void ClearStaticDisplayForExperts() const;

  /////////////////////////////////////////////////////////////////////////////
  //
  // CRhinoObject overrides
  //

  /*
  Returns:
    ON::subd_object
  */
  ON::object_type ObjectType() const override;

  virtual const CRhinoMaterial& ObjectMaterial() const override;

  virtual const CRhinoMaterial& ObjectMaterial(const ON_COMPONENT_INDEX& ci,
                                               const ON_UUID& plugin_id,
                                               const ON_3dmObjectAttributes* attrs) const override;

  // virtual overrides to support subobject selection
protected:
  int SubObjectsAreSelected() const override;
  int SubObjectsAreHighlighted() const override;
  bool HasSubobjectMaterials(void) const override;
  int  GetSubobjectMaterialComponents(ON_SimpleArray<ON_COMPONENT_INDEX>& componentsWithSubobjectMaterials) const override;

public:
  int UnselectAllSubObjects() override;
  int UnhighlightAllSubObjects() override;
  bool HighlightRequiresRedraw() const override;
  bool IsSubObjectSelected(ON_COMPONENT_INDEX componentIndex) const override;
  int GetSelectedSubObjects(ON_SimpleArray<ON_COMPONENT_INDEX>& componentIndices) const override;
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX componentIndex) const override;
  int GetHighlightedSubObjects(ON_SimpleArray<ON_COMPONENT_INDEX>& componentIndices) const override;
  bool HighlightSubObject(ON_COMPONENT_INDEX componentIndex, bool highlight = true) const override;

  int SelectSubObject(
        ON_COMPONENT_INDEX component_index,
        bool bSelect = true,
        bool bSynchHighlight = true,
        bool bPersistentSelect = false
        ) const override;
  

  int SelectSubDComponent(
    ON_SubDComponentPtr component_ptr,
    bool bSelect = true,
    bool bSynchHighlight = true,
    bool bPersistentSelect = false
    ) const;

  //bool GetFrame(ON_Plane& plane) const override;


  // virtual CRhinoObject::EnableGrips override
  void EnableGrips(bool gripsOn) override;

  //// virtual CRhinoObject::DrawV6 override
  //void DrawV6(
  //  class CRhinoObjectDrawContext* drawContext
  //    ) const RHINO_NOEXCEPT override;
               
  // virtual CRhinoObject::Draw override
  void Draw(CRhinoDisplayPipeline& dp) const override;
  // virtual CRhinoObject::DrawSubObject override
  void DrawSubObject( CRhinoDisplayPipeline& dp, ON_COMPONENT_INDEX component_index) const override;
  //virtual CRhinoObject::DrawHighlightedSubObjects override
  void DrawHighlightedSubObjects(CRhinoDisplayPipeline& dp) const override;

  // virtual CRhinoObject::SetTextureCoordinates override
  bool SetTextureCoordinates( 
         const ON_TextureMapping& mapping,
         const ON_Xform* objectXform,
         bool lazy = true
         ) const override;

  // virtual CRhinoObject::PickFilter override
  bool PickFilter(const CRhinoGetObject& go, bool) const override;

  // virtual CRhinoObject::Pick override
  int Pick(const CRhinoPickContext& pickContext, CRhinoObjRefArray& objRefs) const override;

private:
  bool Internal_CreateSubObjectRef(
    const class CRhinoPickContext& pick_context,
    const class CRhinoSubDPickSettings& settings,
    const class ON_SubDComponentRef& r,
    const class ON_SubDComponentPoint& p,
    ON_COMPONENT_INDEX ci,
    CRhinoObjRefArray& pick_list
  ) const;

public:
  
  // virtual CRhinoObject::SnapTo override
  bool SnapTo(const CRhinoSnapContext& snapContext, CRhinoSnapEvent& snapEvent) const override;

  // virtual CRhinoObject::BoundingBox override
  ON_BoundingBox BoundingBox() const override;

  // virtual CRhinoObject::BoundingBox override
  ON_BoundingBox BoundingBox(const class CRhinoViewport* pViewport) const override;

  // virtual CRhinoObject::GetTightBoundingBox override
	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  // virtual CRhinoObject::IsMeshable override
  bool IsMeshable(ON::mesh_type) const override;

  // virtual CRhinoObject::SetRenderMeshParameters override
  bool SetRenderMeshParameters(const ON_MeshParameters& mp) override;

  // virtual CRhinoObject::MeshCount override
  int MeshCount(ON::mesh_type meshType, const ON_MeshParameters* mp = 0) const override;

  // virtual CRhinoObject::CreateMeshes override
  int CreateMeshes(ON::mesh_type, const ON_MeshParameters&, bool bIgnoreCustom = false) override;
  
  // virtual CRhinoObject::DestroyMeshes override
  void DestroyMeshes(ON::mesh_type meshType, bool deleteMeshes = true) override;
  
  bool PromotePreviewMeshes(ON::mesh_type) override;

  // virtual CRhinoObject::GetMeshes override
  int GetMeshes(ON::mesh_type, ON_SimpleArray<const ON_Mesh *>&) const override;

  //Servicing the fake-virtual function in CRhinoObject
  std::vector<std::shared_ptr<const ON_Mesh>> GetMeshes(ON::mesh_type mesh_type) const /*override*/;

  /*
  Returns:
    Current render/analysis/preview mesh.
  */
  ON_MeshRef MeshRef(ON::mesh_type mesh_type) const;

  /*
  Parameters:
    adaptive_display_density - [in] 
      When in doubt, pass ON_SubDDisplayParameters::DefaultDensity.
      Valid adaptive_display_density values are >= ON_SubDDisplayParameters::MinimumDensity and <= ON_SubDDisplayParameters::MaximumDensity.
      See details under bCreateIfMissing for more information.

    bCreateIfMissing - [in]

      If a valid adaptive_display_density is passed:
        If the existing mesh has the requested density, then it is returned.
        If bCreateIfMissing is true, the a mesh with the requested density is created and returned.
        If bCreateIfMissing is false, and empty mesh ref is returned.

      If an invalid display_density is passed:
        If any mesh exists, then it is returned.
        If bCreateIfMissing is true, the a mesh with ON_SubDDisplayParameters::DefaultDensity is created and returned.
        If bCreateIfMissing is false, and empty mesh ref is returned.

    mp - [in]
      If bCreateIfMissing is true and a mesh is created, these will be set as the meshes creation parameters.      

  Returns:
    A render mesh with the specified properties. 
    When bCreateIfMissing, this mesh may be empty.
  */
  ON_MeshRef MeshRef(
    ON::mesh_type mesh_type,
    unsigned int adaptive_display_density,
    bool bCreateIfMissing,
    const ON_MeshParameters* mp
    );

  // virtual CRhinoObject::MeshDisplayAttributes override
  const class CDisplayPipelineAttributes* MeshDisplayAttributes( 
          int meshIndex, 
          const ON_Mesh& mesh,
          const class CDisplayPipelineAttributes& displayAttrs
          ) const override;

  // virtual CRhinoObject::GetWireframeCurves override
  int GetWireframeCurves(ON_SimpleArray<ON_Curve*>& curveList) const override;

  // virtual CRhinoObject::IsSolid override
  bool IsSolid() const override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

  /*
  Returns:
    "subd"
  */
  const wchar_t* ShortDescription(bool plural) const override;

  // virtual CRhinoObject::GetSubObjects override
  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const class CRhinoViewport* pViewport = NULL
          ) const override;

  // virtual CRhinoObject::SizeOf override
  unsigned int SizeOf() const override;

  void DeleteFromDocNotification() override;
  void DestroyRuntimeCache(bool bDelete = true) override;

  /*
  Description:
    Get the SubD appearance (surface or control net);
  Returns:
    ON_SubDComponentLocation::Surface (creamy/smooth) 
    or ON_SubDComponentLocation::ControlNet (cruncy/box).
  */
  ON_SubDComponentLocation SubDAppearance() const;

  /*
  Parameters:
    bDynamicDisplay - [in]
      If bEnableDynamicDisplay is true and a dynamic display is available, then it is returned.
      Otherwise the static display is returned.
  */
  CRhinoSubDDisplay SubDDisplay(
    bool bDynamicDisplay
  ) const;

  ON_DEPRECATED_MSG("OBSOLETE: Always returns 0.")
  unsigned int DynamicDisplayContentSerialNumber() const;

  ON_DEPRECATED_MSG("OBSOLETE: Always returns 0.")
  unsigned int StaticDisplayContentSerialNumber() const;

  /*
  Returns:
    True if the dynamic display from this->SubDisplay(true) is
    different than the static display from this->SubDDisplay(false).
  */
  bool DynamicDisplayAvailable() const;

  /*
    bIgnoreObjectState - [in]
        Objects can be in one of 4 states: selected, normal, locked,
        and hidden. locked objects.  By default, selected objects are
        drawn in RhinoApp().AppSettings().SelectedObjectColor(), locked
        objects are drawn in RhinoApp().AppSettings().LockedObjectColor(),
        and normal objects are drawn in the color determined by the
        attributes settings.  If you want to ignore the object state and
        get the color determined by the attributes, the pass true.
  */
  CRhinoSubDDisplayAttributes SubDDisplayAttributes(
    const class CRhinoDisplayPipeline* display_pipeline,
    bool bIgnoreObjectState
    ) const;

  // virtual CRhinoObject::SupportsBrepProxyReferenceCounting override
  bool SupportsBrepProxyReferenceCounting() const override;

  // Debugging tools

  /*
  Description:
    This tool will create text dots and a mesh object that can be used
    to debug the current state of a subd.
  Parameters:
    rhino_doc_sn - [in]
      destination document
    layer_index - [in]
      index of destination layer
      (-1) to use default layer.
    subd - [in]
      subd to label
    bVertexLabels - [in]
      true to label vertex locations
    bEdgeLabels - [in]
      true to label edges near the midpoint
    bFaceLabels - [in]
      true to label faces with their id near their centroid
    bFacePackIdLabels - [in]
      true to include face pack id near the face centroids
    bFacePackIdColors- [in]
      true to show face pack groups using per face colors.
    bSectorCoefficients - [in]
      true to label sector coefficients
    bEdgeSharpnesses - [in]
      true to label edge sharpnesses for sharp edges
    bAllEdgesSharpnesses - [in]
      true to label edge sharpnesses for all edges
    bNetAsLines - [in]
      true to add a rhino line curve object for each edge.
  Returns:
    Number of objects added to the document.
  */
  static unsigned int LabelSubDNet(
    unsigned int rhino_doc_sn,
    int layer_index,
    const ON_SubD* subd,
    bool bVertexLabels,
    bool bEdgeLabels,
    bool bFaceLabels,
    bool bFacePackIdLabels,
    bool bFacePackIdColors,
    bool bSectorCoefficients,
    bool bNetAsLines
  );

  static unsigned int LabelSubDNet(
    unsigned int rhino_doc_sn,
    int layer_index,
    const ON_SubD* subd,
    bool bVertexLabels,
    bool bEdgeLabels,
    bool bFaceLabels,
    bool bFacePackIdLabels,
    bool bFacePackIdColors,
    bool bSymmetryMotifLabels,
    bool bSymmetryMotifColors,
    bool bSectorCoefficients,
    bool bNetAsLines
  );

  static unsigned int LabelSubDNet(
    unsigned int rhino_doc_sn,
    int layer_index,
    const ON_SubD* subd,
    bool bVertexLabels,
    bool bEdgeLabels,
    bool bFaceLabels,
    bool bFacePackIdLabels,
    bool bFacePackIdColors,
    bool bSymmetryMotifLabels,
    bool bSymmetryMotifColors,
    bool bSectorCoefficients,
    bool bEdgeSharpnesses,
    bool bAllEdgesSharpnesses,
    bool bNetAsLines
  );

  /*
  Parameters:
    text_log - [in]
      If nullptr, then the output appears in a Rhino text output dialog
      and can be easily copied to the clipboard.
  */
  static unsigned int PrintSubDTopology(
    const ON_SubD* subd,
    bool bUseRhinoTextOutTool
  );

private:
  // NO friends!
  void DeleteSubDGeometryHelper();

  // Manages the ON_SubD object that CRhinoObject.m_geometry points at.
  ON_SubDRef m_subd_ref;

  // Reference counted ON_Mesh classe used by legacy rendering code that calls CRhinoSubDObject::CreateMeshes()
  // As of June 2020, "legacy" means V6 rendering plugins and V7 Cycles.
  // All core code for wireframe, shaded and rendered viewports uses modern
  // ON_SubDMeshFragments and  OpenGL.
  // All core code for Zebra, Draft, Curvature, Environment analysis uses modern
  // ON_SubDMeshFragments and  OpenGL.
  // Raytraced viewports and the Render command call Cycles which is currently "legacy."
  ON_MeshRef m_render_mesh_ref;
  ON_MeshRef m_analysis_mesh_ref; // for legacy V6 plugins that want an analysis mesh
  ON_MeshRef m_preview_mesh_ref; // for legacy V6 plugins that want a preview mesh

  // Manages all the information necessary for picking and display.
  // 
  mutable CRhinoSubDDisplay m_subd_static_display;

  // Manages all the information necessary for dynamic display.
  // For example, when control points are being moved, 
  // m_subd_static_display is used to draw the starting position 
  // and m_subd_dynamic_display is used to draw the moving subd.
  mutable CRhinoSubDDisplay m_subd_dynamic_display;

  ON__UINT_PTR m_reserved = 0;
};


/*
Description:
  Makes a new SubD with vertices offset at distance in the direction of the control net vertex normals.
  Optionally, based on the value of bSolidify, adds the input SubD and a ribbon of faces along any naked edges.
Parameters:
  InSubD    - [in] Input SubD.
  distance  - [in] Input offset distance.
  bSolidify - [in] Input bool to determine whether the output SubD should be turned into a closed SubD.
Returns:
  Returns a ON_SubD pointer on success, nullptr on failure.
  Note, memory for the new SubD is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_SubD* RhinoOffsetSubD(
  const ON_SubD& InSubD,
  double distance,
  bool bSolidify
);
