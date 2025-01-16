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

//////////////////////////////////////////////////////////////////////////
//
// CRhinoGroupObject is a transient runtime proxy CRhinoObject that is 
// used during selection when a developer prefers to get a single group
// reference instead of a list of the members in a group.  
//
// A CRhinoGroupObject NEVER appears in the CRhinoDoc tables.
//
class RHINO_SDK_CLASS CRhinoGroupGeometry : public ON_Geometry
{
  ON_OBJECT_DECLARE(CRhinoGroupGeometry);
public:
  CRhinoGroupGeometry();
  ~CRhinoGroupGeometry();

  bool IsValid( ON_TextLog* text_log = nullptr ) const override;

  // virtual ON_Object::Dump override
  void Dump( ON_TextLog& ) const override;

  // virtual ON_Geometry::Dimension override
  int Dimension() const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  // virtual ON_Geometry::Transform override
  bool Transform( 
         const ON_Xform& xform
         ) override;


  const CRhinoGroup* m_group;
  ON_ClassArray<CRhinoObjRef> m_group_members;
  ON_BoundingBox m_bbox;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};


class RHINO_SDK_CLASS CRhinoGroupObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoGroupObject);

public:
  CRhinoGroupObject();
  ~CRhinoGroupObject();
  CRhinoGroupObject(const CRhinoGroupObject&);
  CRhinoGroupObject& operator=(const CRhinoGroupObject&);

  // virtual CRhinoObject::ObjectType() override
  ON::object_type ObjectType() const override;

  // virtual ON_Object::MemoryRelocate override
  void MemoryRelocate() override;

  // virtual CRhinoObject::IsDeletable() override (returns false)
  bool IsDeletable() const override;

  /*
  Description:
    Test object to see if it is a proxy CRhinoGroupObject.
    virtual CRhinoObject::IsGroupObject override
  Returns:
    TRUE if this is a CRhinoGroupObject.
  */
  bool IsGroupObject() const override;

  // virtual CRhinoObject::IsSubObjectSelected() override
  bool IsSubObjectSelected( ON_COMPONENT_INDEX component_index ) const override;

  // virtual CRhinoObject::IsSubObjectSelected() override
  int GetSelectedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const override;

  // virtual CRhinoObject::SelectSubObject() override
  int SelectSubObject(
        ON_COMPONENT_INDEX component_index,
        bool bSelect = true,
        bool bSynchHighlight = true,
        bool bPersistentSelect = false
        ) const override;

  // virtual CRhinoObject::UnselectAllSubObjects() override
  int UnselectAllSubObjects() override;

  // virtual CRhinoObject::IsSubObjectHighlighted() override
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX component_index) const override;   // true if specified component is highlighted

  // virtual CRhinoObject::GetHighlightedSubObjects() override
  int GetHighlightedSubObjects( ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices ) const override;

  // virtual CRhinoObject::HighlightSubObject() override
  bool HighlightSubObject(ON_COMPONENT_INDEX component_index,bool bHighlight=true) const override;
  
  // virtual CRhinoObject::UnhighlightAllSubObjects() override
  int UnhighlightAllSubObjects() override; // returns number of changed subobjects

  // virtual CRhinoObject::HighlightRequiresRedraw() override
  bool HighlightRequiresRedraw() const override;

  // virtual CRhinoObject::Draw() override
  void Draw(CRhinoDisplayPipeline&) const override;

  // virtual CRhinoObject::DrawSubObject() override
  void DrawSubObject(CRhinoDisplayPipeline&,ON_COMPONENT_INDEX) const override;

  // virtual CRhinoObject::DrawHighlightedSubObjects() override
  void DrawHighlightedSubObjects(CRhinoDisplayPipeline&) const override;

  /*
  Description:
    virtual CRhinoObject::ShortDescription override
  Returns:
    "group" or "groups"
  */
  const wchar_t* ShortDescription( bool bPlural ) const override;

  //////////
  // returns number of members in group
  int Create( 
    const CRhinoDoc&,
    int //  group_index 
    );

  const CRhinoGroup* Group() const;
  int MemberCount() const;
  CRhinoObject* Member(ON_COMPONENT_INDEX) const;
  CRhinoObject* Member(int) const;
  CRhinoObject* operator[](int) const;

protected:
  // virtual CRhinoObject::SubObjectsAreSelected() override
  int SubObjectsAreSelected() const override; // returns number of selected subobjects
  
  // virtual CRhinoObject::SubObjectsAreHighlighted() override
  int SubObjectsAreHighlighted() const override; // returns number of highlighted subobjects
  
private:
  friend class CRhinoDoc;
  CRhinoGroupGeometry m_group_geometry;
};

