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

class RHINO_SDK_CLASS CRhinoLeader : public CRhinoAnnotation
{
  ON_OBJECT_DECLARE(CRhinoLeader);

public:
  //Construction, destruction, assignment
  CRhinoLeader() = default;
  ~CRhinoLeader();
  CRhinoLeader(const CRhinoLeader& src);
  CRhinoLeader& operator=(const CRhinoLeader& src);
  CRhinoLeader(const ON_3dmObjectAttributes&);
  ON::object_type ObjectType(void) const override;
  const wchar_t* ShortDescription(bool) const override;

  int Pick(const CRhinoPickContext&, CRhinoObjRefArray& objref) const override;

  // virtual CRhinoObject::SnapTo override
  bool SnapTo(const CRhinoSnapContext& snap_context, CRhinoSnapEvent& snap_event) const override;

  bool IsActiveInViewport(const CRhinoViewport& viewport) const override;
  bool IsVisibleInViewport(CRhinoDisplayPipeline& dp) const;
  bool IsVisibleInViewport(const CRhinoViewport& vp) const;
  bool IsVisibleInViewport(const CRhinoViewport& vp, const ON_Xform& xform) const;
  void EnableGrips(bool bGripsOn) override;
  
  ON_Leader* SetLeader(const ON_Leader& on_leader);
  void SetLeader(ON_Leader* on_leader);

  const ON_Leader* LeaderObject(const ON_DimStyle* dimstyle) const;

  void Draw(CRhinoDisplayPipeline& dp) const override;

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;
};

/*
Description:
  Explodes a leader into its component parts.
Parameters:
  leader - [in] The leader to explode.
  dimstyle - [in] The leader's effective dimension style.
  out_curves - [out] The output curves.
  out_objects - [out] The output text and instance references (if the leader is using user-defined arrowheadsrowheads)
Returns:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoExplodeLeader(
  const ON_Leader* leader,
  const ON_DimStyle* dimstyle,
  ON_SimpleArray<ON_Curve*>& out_curves,
  ON_SimpleArray<ON_Geometry*>& out_objects
);
