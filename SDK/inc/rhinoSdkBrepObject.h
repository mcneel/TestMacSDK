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


class RHINO_SDK_CLASS CRhinoBrepObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoBrepObject);

public:
  CRhinoBrepObject();                              // assigns a new object UUID
  CRhinoBrepObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoBrepObject();
  CRhinoBrepObject(const CRhinoBrepObject&);            // copies source object UUID
  CRhinoBrepObject& operator=(const CRhinoBrepObject&); // copies source object UUID

  /*
  Description:
    Returns a constant that determines the type of
    CRhinoObject.
  Returns:
    ON::brep_object
  Remarks:
    A brep object can be a surface or polysurface. An annotation
    object can be anything derived from CRhinoAnnotation.
  */
  ON::object_type ObjectType() const override;

  void SetBrep( ON_Brep* brep ); // ~CRhinoBrepObject() will delete the brep
  const ON_Brep* Brep() const;

  // virtual overrides to support subobject selection
protected:
  int SubObjectsAreSelected() const override;
  int SubObjectsAreHighlighted() const override;
  bool HasSubobjectMaterials() const override;
  int  GetSubobjectMaterialComponents(ON_SimpleArray<ON_COMPONENT_INDEX>& componentsWithSubobjectMaterials) const override;

  using CRhinoObject::ObjectMaterial;

  const CRhinoMaterial& ObjectMaterial(const ON_COMPONENT_INDEX& ci, const ON_UUID& plugin_id = RhinoDefaultRendererId(), const ON_3dmObjectAttributes* attrs = nullptr) const override;
  const CRhRdkMaterial* ObjectRdkMaterial(const ON_COMPONENT_INDEX& ci, const ON_UUID& plugin_id = RhinoDefaultRendererId(), const ON_3dmObjectAttributes* attrs = nullptr) const override;
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
        ON_COMPONENT_INDEX componentIndex,
        bool select = true,
        bool synchHighlight = true,
        bool persistentSelect = false
        ) const override;
  bool GetFrame(ON_Plane& plane) const override;


  // virtual CRhinoObject::EnableGrips override
  void EnableGrips(bool gripsOn) override;

  // virtual CRhinoObject::DrawV6 override
  virtual
  void DrawV6(
    class CRhinoObjectDrawContext* drawContext
      ) const RHINO_NOEXCEPT override;
               
  // virtual CRhinoObject::Draw override
  void Draw(CRhinoDisplayPipeline& dp) const override;
  // virtual CRhinoObject::DrawSubObject override
  void DrawSubObject(CRhinoDisplayPipeline& dp, ON_COMPONENT_INDEX componentIndex) const override;
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
  
  // virtual CRhinoObject::SnapTo override
  bool SnapTo(const CRhinoSnapContext& snapContext, CRhinoSnapEvent& snapEvent) const override;

  // virtual CRhinoObject::OnDoubleClick override
  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& double_click_context) const override;

  // virtual CRhinoObject::BoundingBox override
  ON_BoundingBox BoundingBox() const override;

  // virtual CRhinoObject::BoundingBox override
  ON_BoundingBox BoundingBox(const class CRhinoViewport* pViewport) const override;

  // virtual CRhinoObject::GetTightBoundingBox override
	bool GetTightBoundingBox( 
			ON_BoundingBox& tightBbox, 
      bool growBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  // virtual CRhinoObject::IsMeshable override
  bool IsMeshable(ON::mesh_type) const override;

  // virtual CRhinoObject::SetRenderMeshParameters override
  bool SetRenderMeshParameters(const ON_MeshParameters& mp) override;

  // virtual CRhinoObject::MeshCount override
  int MeshCount(ON::mesh_type meshType, const ON_MeshParameters* mp = nullptr) const override;

  // virtual CRhinoObject::CreateMeshes override
  int CreateMeshes(ON::mesh_type, const ON_MeshParameters&, bool bIgnoreCustom = false) override;
  
  // virtual CRhinoObject::DestroyMeshes override
  void DestroyMeshes(ON::mesh_type meshType, bool deleteMeshes = true) override;
  bool PromotePreviewMeshes(ON::mesh_type) override;

  // virtual CRhinoObject::GetMeshes override
  int GetMeshes(ON::mesh_type, ON_SimpleArray<const ON_Mesh *>&) const override;

  //Servicing the fake-virtual function in CRhinoObject
  std::vector<std::shared_ptr<const ON_Mesh>> GetMeshes(ON::mesh_type mesh_type) const /*override*/;

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
  Description:
    virtual CRhinoObject::ShortDescription override
  Returns:
    "surface", "surfaces", "polysurface", or "polysurfaces"
  */
  const wchar_t* ShortDescription(bool plural) const override;

  // virtual CRhinoObject::GetSubObjects override
  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const class CRhinoViewport* pViewport = nullptr
          ) const override;

  void DeleteFromDocNotification() override;
  void DestroyRuntimeCache(bool bDelete = true) override;

  class CRhBrepDisplay* BrepDisplay() const;
  bool BrepDisplayExists() const;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

class RHINO_SDK_CLASS CRhinoSurfaceObject : public CRhinoBrepObject
{
  ON_OBJECT_DECLARE(CRhinoSurfaceObject);
public:
  CRhinoSurfaceObject();                              // assigns a new object UUID
  CRhinoSurfaceObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoSurfaceObject();
  CRhinoSurfaceObject( const CRhinoSurfaceObject& );          // copies source object UUID
  CRhinoSurfaceObject& operator=(const CRhinoSurfaceObject&); // copies source object UUID

  void SetSurface( ON_Surface* ); // ~CRhinoBrepObject() will delete the surface
  const ON_Surface* Surface() const;
};
