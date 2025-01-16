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

class RHINO_SDK_CLASS CRhinoCurveObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoCurveObject);

public:
  // Returns ON::curve_object
  ON::object_type ObjectType() const override;

public:
  CRhinoCurveObject();                                          // assigns a new object UUID
  CRhinoCurveObject(const ON_3dmObjectAttributes& attributes);  // assigns a new object UUID
  ~CRhinoCurveObject();
  CRhinoCurveObject(const CRhinoCurveObject& other);            // copies source object UUID
  CRhinoCurveObject& operator=(const CRhinoCurveObject& other); // copies source object UUID

  void EnableGrips(bool gripsOn) override;

  void DrawV6(class CRhinoObjectDrawContext* draw_context) const RHINO_NOEXCEPT override;

  void Draw(CRhinoDisplayPipeline& dp) const override;

  bool PickFilter(const CRhinoGetObject&, bool ) const override;

  int Pick(const CRhinoPickContext&, CRhinoObjRefArray& ) const override;
  
  bool SnapTo(const CRhinoSnapContext& snapContext, CRhinoSnapEvent& snapEvent) const override;

  ON_BoundingBox BoundingBox() const override;
  ON_BoundingBox BoundingBox( const CRhinoViewport* pViewport) const override;

	bool GetTightBoundingBox( 
			ON_BoundingBox& tightBbox, 
      bool growBox = false,
			const ON_Xform* xform = nullptr
      ) const override;

  int GetWireframeCurves( ON_SimpleArray<ON_Curve*>& curveList ) const override;

  bool IsSolid() const override;

  // Returns "curve" or "curves"
  const wchar_t* ShortDescription( bool plural ) const override;

  int GetSubObjects(ON_SimpleArray<CRhinoObject*>& subObjects, const CRhinoViewport* pViewport = nullptr) const override;

  /*
  Description:
    Gets curve subobjects or displayed subobjects if bUseCurveDisplay is set to true.
  Returns:
    Count of subobjects created. 
  */
  int GetSubObjects(ON_SimpleArray<CRhinoObject*>& subObjects, bool bUseCurveDisplay, const CRhinoViewport* pViewport = nullptr) const;

  /*
  Description:
    virtual CRhinoObject::GetFrame override
    Calls GetFrameAt(domain[0])
  Parameters:
    plane - [out] frame for curve
  Returns:
    true if successful
  */
  bool GetFrame( ON_Plane& plane ) const override;

  void DeleteFromDocNotification() override;

  // Description:
  //   Specify the curve geometry.
  // Parameters:
  //   curve - [in] curve geometry copied to object
  // Returns:
  //   Pointer to curve geometry on CRhinoCurveObject.
  ON_Curve* SetCurve( const ON_Curve& curve );

  // Description:
  //   Specify the curve geometry.
  // Parameters:
  //   curve - [in] curve geometry copied to object
  // Returns:
  //   Pointer to curve geometry on CRhinoCurveObject.
  ON_Curve* SetCurve( const ON_BezierCurve& curve );

  // Description:
  //   Specify the curve geometry.
  // Parameters:
  //   pCurve - [in] ~CRhinoCurveObject() will delete this curve
  void SetCurve(ON_Curve* pCurve);

  const ON_Curve* Curve() const;

  class CRhCurveDisplay* CurveDisplay(bool createIfNull=true) const;

  /*
  Description:
    If you change a setting that requires the curve
    segments to be updated, then call this function.
  */
  void DeleteCurveDisplay() const;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

  /*
  Description:
    Fetches the visible curve intervals (dashes and/or dots of a curve) for a given linetype.
    In the case that the curve is continuous (no line styling), intervals contains a single
    interval representing the whole curve.
  */
  bool GetVisibleIntervals(ON_SimpleArray<ON_Interval>& intervals) const;
protected:
  int SubObjectsAreSelected() const override;
  int SubObjectsAreHighlighted() const override;
public:
  int UnselectAllSubObjects() override;
  int UnhighlightAllSubObjects() override;
  bool HighlightRequiresRedraw() const override;
  bool IsSubObjectSelected(ON_COMPONENT_INDEX component_index) const override;
  int GetSelectedSubObjects(ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices) const override;
  bool IsSubObjectHighlighted(ON_COMPONENT_INDEX component_index) const override;
  int GetHighlightedSubObjects(ON_SimpleArray<ON_COMPONENT_INDEX>& component_indices) const override;
  bool HighlightSubObject(ON_COMPONENT_INDEX component_index, bool bHighlight = true) const override;
  int SelectSubObject(ON_COMPONENT_INDEX component_index,
                      bool bSelect = true,
                      bool bSynchHighlight = true,
                      bool bPersistentSelect = false
                      ) const override;
  void DrawSubObject(CRhinoDisplayPipeline&, ON_COMPONENT_INDEX) const override;
  void DrawHighlightedSubObjects(CRhinoDisplayPipeline&) const override;

private:
  class CRhCacheDataCurve* GetRuntimeCache() const;

#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4251 ) 
  // C4251: ... needs to have dll-interface to be used by clients of class ...
  // m__curve_runtime_data is private and all code that manages m__curve_runtime_data is explicitly implemented in the DLL.
private:
  mutable std::shared_ptr<class CRhCacheDataCurve> m__curve_runtime_data;
#pragma ON_PRAGMA_WARNING_POP
};
