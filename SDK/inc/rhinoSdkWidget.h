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

class CRhWidgetGeometryCustomObjectRef;

class RHINO_SDK_CLASS CRhWidgetDrawAttr
{
public:
  CRhWidgetDrawAttr() : m_col(0), m_thickness(1), m_pattern(0xFFFFFFFF), m_uuid_object(ON_nil_uuid) { }
  ON_Color m_col;
  int m_thickness;
  UINT m_pattern;
  UUID m_uuid_object;
};

class RHINO_SDK_CLASS CRhWidgetThickerDottedDrawAttr : public CRhWidgetDrawAttr
{
public:
  CRhWidgetThickerDottedDrawAttr() { m_thickness = 2; m_pattern = 0xCCCC; }
};

/*
Description:

 This is the interface through which clients can draw widget geometry.
 See CRhinoWidgetGeometry::AddToDrawCache()

*/
class RHINO_SDK_CLASS IRhWidgetDrawCache
{
public:
  virtual ~IRhWidgetDrawCache() { }

  virtual CRhWidgetDrawAttr DrawAttr(void) const = 0;
  virtual CRhWidgetDrawAttr SetDrawAttr(const CRhWidgetDrawAttr& da) = 0;

  virtual UINT SetPattern(UINT p) = 0;
  virtual COLORREF SetColor(COLORREF col) = 0;

  virtual bool UseAxisColors(void) const = 0;
  virtual bool SetUseAxisColors(bool b) = 0;

  virtual void SetInterestObject(const UUID& uuidIO) = 0;

  virtual bool SnapTo(const CRhinoSnapContext& sc, CRhinoSnapEvent& se, const ON_Xform& xformLocal) const = 0;

  virtual void AddArc(const ON_Arc& a, const UUID& uuidObject) = 0;
  virtual void AddCircle(const ON_Circle& c, const UUID& uuidObject) = 0;
  virtual void AddEllipse(const ON_Ellipse& e, const UUID& uuidObject) = 0;
  virtual void AddLine(const ON_3dPoint& pt1, const ON_3dPoint& pt2, const UUID& uuidObject) = 0;
  virtual void AddArrow(const ON_3dPoint& pt1, const ON_3dPoint& pt2, const UUID& uuidObject) = 0;
  virtual void AddObject(CRhWidgetGeometryCustomObjectRef& ref, const UUID& uuidObject) = 0;
  virtual void AddDottedLine(const ON_3dPoint& pt1, const ON_3dPoint& pt2, const UUID& uuidObject) = 0;
  virtual void AddClippingPlane(ON_3dPoint cornerA, ON_3dPoint cornerB, ON_3dPoint cornerC, ON_3dPoint cornerD, const UUID& uuidObject) = 0;
};

class RHINO_SDK_CLASS IRhWidgetDrawCacheEx : public IRhWidgetDrawCache
{
public:
  virtual ~IRhWidgetDrawCacheEx() { }

  virtual void AddText(const wchar_t* text,
                        const ON_Plane& plane,
                        double height,
                        const ON_Font* font,
                        const ON_UUID& uuidObject) = 0;

  virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
};

/*
Description:

 This is an abstract base class for widget geometry.

*/
class RHINO_SDK_CLASS CRhinoWidgetGeometry : public ON_Geometry
{
  ON_OBJECT_DECLARE(CRhinoWidgetGeometry);

public:
  CRhinoWidgetGeometry();
  CRhinoWidgetGeometry(const CRhinoWidgetGeometry& g);
  virtual ~CRhinoWidgetGeometry();

  const CRhinoWidgetGeometry& operator = (const CRhinoWidgetGeometry&);

  CRhinoDoc* Document(void) const;
  void SetDocument(const CRhinoDoc& doc);

  void CleanupGeometryCache(void) const;

public:
  virtual bool Transform(const ON_Xform& xform) override;

  void Start(const CRhinoDoc* doc);
  void Stop();
  void SetMapEntry(int iChannelId, const UUID& uuidObject, const ON_TextureMapping& m, const ON_Xform& xformLocal, const CRhinoDoc* pDoc);

  bool Transforming(ON_Xform& xformToApply) const;

public:
  virtual bool AddToDrawCache(IRhWidgetDrawCache& wdc, const ON_Xform& xformLocal, const UUID& uuidObject) = 0;

public:
  class RHINO_SDK_CLASS CSweep // Sweep info for ellipsoid and cylindrical drawing.
  {
  public:
    CSweep();

    struct CAngles // In radians.
    {
      double dHorzSweepSta; // Start of 'horizontal sweep'.
      double dHorzSweepEnd; // End   of 'horizontal sweep'.
      double dVertSweepSta; // Start of 'vertical sweep'.
      double dVertSweepEnd; // End   of 'vertical sweep'.
    }
    angles;

    int iNumLat;           // Number of lines of latitude.
    int iNumLon;           // Number of lines of longitude.
    int iHighlightLatMod;  // Modulus for highlighting lines of latitude  (e.g., 'off'=0, 'every'=1, 'every other'=2).
    int iHighlightLonMod;  // Modulus for highlighting lines of longitude (e.g., 'off'=0, 'every'=1, 'every other'=2).
  };

  // Direction for AddPlaneToDrawCache. Draws 'normal' arrow(s).
  enum eDrawNormal { dn_none, dn_forward, dn_backward, dn_both };

  /* Note on xform: in V5 it was the inverse of the real xform. In V6 it is the real xform (transforms unit object into the world).
     This was changed because it was confusing me and would probably have confused future clients. */
  virtual void AddBoxToDrawCache(IRhWidgetDrawCache& wdc, const ON_Xform& xform, const UUID& uuidObject) const;
  virtual void AddPlaneToDrawCache(IRhWidgetDrawCache& wdc, const ON_Xform& xform, const UUID& uuidObject, bool bCapped, eDrawNormal dn) const;
  virtual void AddCylinderToDrawCache(IRhWidgetDrawCache& wdc, const ON_Xform& xform, const UUID& uuidObject, bool bCapped, const CSweep* pSweep = NULL) const;
  virtual void AddEllipsoidToDrawCache(IRhWidgetDrawCache& wdc, const ON_Xform& xform, const UUID& uuidObject, const CSweep* pSweep = NULL) const;

  // This used to be a test function but I'm reusing it for OCS mapping because I can't add a new virtual function and this one is not being used.
  virtual void AddAxesToDrawCache(IRhWidgetDrawCache& wdc, const ON_Xform& xform, const UUID& uuidObject) const;

protected: // Implement ON_Geometry.
  virtual int Dimension(void) const override;

  virtual bool IsValid(ON_TextLog* text_log = nullptr) const override;

  // virtual ON_Geometry GetBBox override		
  virtual bool GetBBox(double* boxmin, double* boxmax, bool bGrowBox = false) const override;

private:
  class CImpl;
  CImpl* m_pImpl;

protected:
  CImpl& Impl(void) const;
};

/*
Description:

  This is the widget grip base class. You can subclass it to create special kinds of grips
  or just use the base class (default). See CRhinoWidget::NewGrip().

*/
class RHINO_SDK_CLASS CRhinoWidgetGrip : public CRhinoGripObject
{
  ON_OBJECT_DECLARE(CRhinoWidgetGrip);

public:
  CRhinoWidgetGrip();
  virtual ~CRhinoWidgetGrip();

public:
  class CImpl;
  CImpl* m_pImpl;
};

/*
Description:

  CRhinoWidget is a special kind of 'phantom' Rhino object which is used for widgets.
  A widget is a tool the user can use to manipulate data such as texture mappings or decals.
  Because widgets are also Rhino objects, they work with the gumball.

  The widget base class is intentionally 'dumb'. A widget doesn't know exactly what it is being
  used for. That knowledge is delegated to subclasses created by widget clients.

  NOTE: This object evolved from the 'mapping widget' but it is really nothing to do
  with 'mapping', 'texture mapping' or ON_TextureMapping. Mapping-specific code must not
  appear in this base class or its implementation.

  To create your own widget, derive a class from CRhinoWidget and create the widget as follows:

    ::RhCreateWidget(doc, new CMyWidget(...));

  Rhino will create a document-resident widget from your instance and take ownership of it; you
  must not store a pointer to it. Widgets, like other document-resident Rhino objects, are managed
  by the document.

*/
class RHINO_SDK_CLASS CRhinoWidget : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoWidget);

public:
  CRhinoWidget();

  const CRhinoWidget& operator = (const CRhinoWidget& w);

  CRhinoWidgetGeometry& Geometry(void) const;

  IRhWidgetDrawCache& DrawCache(void) const;

public:
  /** Optionally implement this method to record which Rhino object is being picked
    or otherwise manipulated by the user. */
  virtual void SetInterestObject(const UUID& uuidObject) const;

  /** Called when the user deletes the associated widget by using the 'Delete' command.
    Perform any extra actions here; for example, you may want to delete the data
    associated with the widget. Note that this is called BEFORE the widget is deleted
    so you cannot rely on calling CRhinoObject::IsDeleted(). */
  virtual void OnWidgetDeleted(void);

  /** Optionally override this method to return a new special grip.
    The default implementation returns a new standard grip.
    \param index is the index of the grip (in point order). */
  virtual CRhinoWidgetGrip* NewGrip(int index) const;

  /** The first grip is usually the 'origin'.
    Optionally override this method to return a different index. */
  virtual int GripOriginIndex(void) const { return 0; } // [SDK_UNFREEZE] Remove inline implementation.

  /** The grip at (count - index) is usually the opposite one.
    Optionally override this method to return a different index. */
  virtual int GripOppositeIndex(int index, int count) const { return count - index; } // [SDK_UNFREEZE] Remove inline implementation.

public:
  /** Return true if the widget is associated with the specified object.
    Widgets can optionally be associated with objects if they happen to have
    something in common with them. They can also optionally move with these objects. */
  virtual bool IsAssociatedWithObject(const CRhinoObject& obj) const = 0;

  /** If the widget automatically moves with an associated object, it should return true. */
  virtual bool MovesWithAssociatedObjects(void) const = 0;

  /** Compute feedback-related data when the widget is being moved. */
  virtual void ComputeFeedback(const ON_Xform& /*xformDynamic*/) const { } // [SDK_UNFREEZE] Remove inline implementation.

  /** Draw feedback when the widget is being moved. */
  virtual void DrawFeedback(const ON_Xform& xformDynamic, CRhinoDisplayPipeline& dp) const = 0;

  /** Get the widget's transformation.
    \param xformTransOut accepts the xform that transforms the unit widget into world space.
    Return true if successful, else false. */
  virtual bool GetXformTrans(ON_Xform& xformTransOut) const = 0;

  /** Apply a transformation to the widget's data. */
  virtual void ApplyTransformation(const ON_Xform& xform) = 0;

  virtual void GetGeometryTransformData(const CRhinoWidgetGeometry& newGeom, ON_Xform& xform) const;

  /** When grips are dragged and dropped, new geometry is automatically created.
    Implement this method to use the new geometry to update your data. */
  virtual void SetGeometryWhenGripDropped(CRhinoWidgetGeometry& newGeom, bool bApplyToDynamicDecal) = 0;

  /** Get the widget's grip points. */
  virtual void GetGripPoints(ON_3dPointArray& aOut) const = 0;

  /** Add the widget's geometry to the draw cache. */
  virtual void AddToDrawCache(CRhinoWidgetGeometry& geom) = 0;

  /** Return a new geometry object for the widget. */
  virtual CRhinoWidgetGeometry* NewGeometry(void) const = 0;

  /** Return a new geometry object to use when grips are being manipulated. */
  virtual CRhinoWidgetGeometry* NewGeometryForMovedGrip(const CRhinoWidgetGrip& movedGrip) const = 0;

  /** \param xformLocalOut accepts an xform that makes a widget stay with an object when the object moves.
    \note This is here to support mapping widgets specifically and should not be needed by other clients. */
  virtual bool GetXformLocal(ON_Xform& xformLocalOut) const;

  virtual void UpdateFeedbackManager(const CRhinoWidgetGeometry& geom, const ON_Xform& xformLocal) const = 0;

public:
  virtual ON::object_type ObjectType() const override;
  virtual ON_BoundingBox BoundingBox() const override;
  virtual ON_BoundingBox BoundingBox(const CRhinoViewport*) const override;
  virtual bool IsDeletable() const override;
  virtual void EnableGrips(bool bEnable) override;
  virtual bool IsSelectableWithGripsOn() const override;
  virtual bool PickFilter(const CRhinoGetObject& go, bool bCheckSubObjects) const override;
  virtual int  Pick(const CRhinoPickContext&, CRhinoObjRefArray& a) const override;
  virtual bool CanDrawMarquee(void) const;
  virtual void Draw(CRhinoDisplayPipeline& dp) const override;
  virtual void PreDrawUpdate(CRhinoDisplayPipeline& dp) const override;
/*virtual*/ ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/; // [SDK_UNFREEZE]

protected:
  virtual void AddToDocNotification() override;
  virtual void DeleteFromDocNotification() override;

protected:
  virtual ~CRhinoWidget();
  void CheckUpdateGrips(void);
  void RefreshGrips(void);
  ON_BoundingBox BoundingBoxImpl(void);

  friend class CRhinoWidgetGrips;
  void CalculateXformForMovedGrip(const CRhinoWidgetGrip& movedGrip, ON_Xform& xformOut, bool constrain_axis[3] = {}) const;

public:
  // Added to support 'ALB 2011.6.3 - partial fix for TRR#34533, #34544'
  bool IsAssociatedWithListObject(ON_SimpleArray<CRhinoObject*>& list) const;

  // Added to support '15 March 2012, Mikko: Hack to get planar mapping widget gumball to behave the way it used to.'
  virtual bool GumballHack(const CRhinoDoc& doc, CRhinoGumball& gumball, bool bAlignToObject) const;

  // For internal use only.
  void SetInitialGeometry(ON_Geometry*);

private:
  CRhinoWidget(const CRhinoWidget&); // Not implemented.

private:
  class CImpl;
  CImpl* m_pImpl;
};

/*
Description:
  Creates a document-resident widget from a new instance of a CRhinoWidget subclass.

Parameters:
  doc     - [in] The document to create the widget in.
  pWidget - [in] A pointer to a new instance of your widget class.
  pAttr   - [in] A pointer to optional object attributes to use for the new widget.

Returns:
  The Rhino object UUID of the widget if successful, else ON_nil_uuid.

Remarks:
  Rhino will create the widget and take ownership of your instance; you must not store a pointer to it.

*/
RHINO_SDK_FUNCTION UUID RhCreateWidget(CRhinoDoc& doc, CRhinoWidget* pWidget, const CRhinoObjectAttributes* pAttr=NULL);

RHINO_SDK_FUNCTION void RhFindWidgetsAssociatedWithObject(const CRhinoObject& obj, ON_SimpleArray<CRhinoWidget*>& aWidgetsOut);

RHINO_SDK_FUNCTION void RhGetGripPoints_Box(ON_3dPointArray& aOut);
RHINO_SDK_FUNCTION void RhGetGripPoints_Sphere(ON_3dPointArray& aOut);
RHINO_SDK_FUNCTION void RhGetGripPoints_Cylinder(ON_3dPointArray& aOut);
RHINO_SDK_FUNCTION void RhGetGripPoints_Plane_UV(ON_3dPointArray& aOut);
RHINO_SDK_FUNCTION void RhGetGripPoints_Plane_UVW(ON_3dPointArray& aOut);

/* 	The geometry that can be checked by RhCheckObjectSnapPoints().
*/
enum eRhSnapGeom
{
  sng_plane_uv,  // Check a plane (uv only).
  sng_plane_uvw, // Check a plane (uvw).
  sng_box,       // Check a box.
  sng_sphere,    // Check a sphere.
  sng_cylinder,  // Check a cylinder.
  sng_axes,      // Check axes (e.g., x,y,z arrows / frame).
};

/*
Description:
  Checks 'osnap' points for a particular geometry. Checks ON::os_end, ON::os_quadrant,
  ON::os_midpoint and ON::os_center in that order.

Parameters:
  sng   - [in]     The geometry to check (see eRhSnapGeom above).
  sc    - [in]     The snap context.
  se    - [in/out] The snap event.
  xform - [in]     The xform that transforms the object into world space.

Returns:
  True if any snap check was successful, else false.
*/
RHINO_SDK_FUNCTION bool RhCheckObjectSnapPoints(eRhSnapGeom sng, const CRhinoSnapContext& sc,
                                                CRhinoSnapEvent& se, const ON_Xform& xform);
