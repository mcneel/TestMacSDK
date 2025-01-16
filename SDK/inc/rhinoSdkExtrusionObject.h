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

class RHINO_SDK_CLASS CRhinoExtrusionObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoExtrusionObject);

public:
  // override virtual ObjectType() - returns ON::extrusion_object
  ON::object_type ObjectType() const override;

  // override virtual ON_Object::DestroyRuntimeCache()
  void DestroyRuntimeCache( bool bDelete = true ) override;

public:
  CRhinoExtrusionObject();                              // assigns a new object UUID
  CRhinoExtrusionObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoExtrusionObject();
  CRhinoExtrusionObject( const CRhinoExtrusionObject& );          // copies source object UUID
  CRhinoExtrusionObject& operator=(const CRhinoExtrusionObject&); // copies source object UUID

  void SetExtrusion( ON_Extrusion* ); // ~CRhinoExtrusionObject() will delete the ON_Extrusion
  const ON_Extrusion* Extrusion() const;

  const wchar_t* ShortDescription( bool bPlural ) const override;
  void EnableGrips( bool bGripsOn ) override;

  void DrawV6(class CRhinoObjectDrawContext* draw_context) const RHINO_NOEXCEPT override;

  void Draw(CRhinoDisplayPipeline&) const override;

  void DrawSubObject(CRhinoDisplayPipeline&,ON_COMPONENT_INDEX) const override;

  void DrawHighlightedSubObjects(CRhinoDisplayPipeline&) const override;

  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const CRhinoViewport* pViewport
          ) const override;

  /*
  Description:
    When an CRhinoExtrusionObject is replaced with a 
    CRhinoBrepObject is commands like explode, bend,
    and so on, the attributes' wire density may need
    to be changed.  
    This function calculates the wire density to use.
  */
  int BrepFormAttributesWireDensity() const;


  // virtual CRhinoObject overrides to support subobject selection
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
  /*
  bool GetFrame( ON_Plane& plane ) const;


  // virtual CRhinoObject::EnableGrips override
  void EnableGrips(  bool bGripsOn );

               
  // virtual CRhinoObject::Draw override
  void Draw(CRhinoDisplayPipeline&) const;
  // virtual CRhinoObject::DrawSubObject override
  void DrawSubObject(CRhinoDisplayPipeline&,ON_COMPONENT_INDEX) const;
  //virtual CRhinoObject::DrawHighlightedSubObjects override
  void DrawHighlightedSubObjects(CRhinoDisplayPipeline&) const;

  */
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

  // // virtual CRhinoObject::BoundingBox override
  // ON_BoundingBox BoundingBox() const;

  // // virtual CRhinoObject::BoundingBox override
  // ON_BoundingBox BoundingBox( const CRhinoView* ) const;

	bool GetTightBoundingBox( 
			ON_BoundingBox& tight_bbox, 
      bool bGrowBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  bool IsMeshable( ON::mesh_type) const override;

  //// virtual CRhinoObject::SetRenderMeshParameters override
  //bool SetRenderMeshParameters( const ON_MeshParameters& mp );

  int MeshCount( ON::mesh_type mesh_type, const ON_MeshParameters* mp = 0 ) const override;

  int CreateMeshes( ON::mesh_type, const ON_MeshParameters&, bool bIgnoreCustom=false ) override;
  
  void DestroyMeshes( ON::mesh_type mesh_type, bool bDeleteMeshes = true ) override;

  bool PromotePreviewMeshes( ON::mesh_type ) override;

  int GetMeshes( ON::mesh_type, ON_SimpleArray<const ON_Mesh *>& ) const override;

  //Servicing the fake-virtual function in CRhinoObject
  std::vector<std::shared_ptr<const ON_Mesh>> GetMeshes(ON::mesh_type mesh_type) const /*override*/;

  //// virtual CRhinoObject::MeshDisplayAttributes override
  //const class CDisplayPipelineAttributes* MeshDisplayAttributes( 
  //        int mesh_index, 
  //        const ON_Mesh& mesh,
  //        const class CDisplayPipelineAttributes& display_attrs
  //        ) const;

  int GetWireframeCurves( ON_SimpleArray<ON_Curve*>& curve_list ) const override;

  bool IsSolid() const override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

  //// virtual CRhinoObject override
  //bool EnableAnalysisMode( ANALYSIS_MODE am, bool bEnable = true ) const;

  //// virtual CRhinoObject override
  //bool SupportsAnalysisMode( ANALYSIS_MODE ) const;

  //// virtual CRhinoObject::GetSubObjects override
  //int GetSubObjects(
  //        ON_SimpleArray<CRhinoObject*>& subObjects, 
  //        const CRhinoViewport* pViewport = NULL
  //        ) const;

  class CRhExtrusionDisplay* ExtrusionDisplay(void) const;

  virtual bool SupportsBrepProxyReferenceCounting() const override;

  ON_DEPRECATED_MSG("use BrepReferenceCounter")
  class CRhObjectReferenceCounter* BrepProxy(void) const;
  ON_DEPRECATED
  bool BrepProxyExists() const;
};
