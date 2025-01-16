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

////////////////////////////////////////////////////////////////////////////////////////
//
// CRhinoDetailViewObject
//
// A detail view is a nested CRhinoViewport placed on a page view with a 2D closed curve
// boundary. Can be any type of modeling view.
class RHINO_SDK_CLASS CRhinoDetailViewObject : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoDetailViewObject);
public:
  // Attempt to get a detail object from a viewport. Only works if the viewport
  // is a viewport that is 'owned' by a detail.
  static const CRhinoDetailViewObject* FromViewport(const CRhinoViewport* viewport);
public:
  CRhinoDetailViewObject();                              // assigns a new object UUID
  CRhinoDetailViewObject(const ON_3dmObjectAttributes&); // assigns a new object UUID

  CRhinoDetailViewObject(const CRhinoPageView& parent_page, ON_2dPoint page_corner1, ON_2dPoint page_corner2);

  CRhinoDetailViewObject( const CRhinoDetailViewObject& );          // copies source object UUID
  CRhinoDetailViewObject& operator=(const CRhinoDetailViewObject&); // copies source object UUID
  ~CRhinoDetailViewObject();

  // virtual ON_Object override
  void MemoryRelocate() override;

  // virtual CRhinoObject override returns ON::detail_object
  ON::object_type ObjectType() const override;

  //Description:
  //  virtual CRhinoObject::ShortDescription override
  //Returns:
  //  "detail view" or "detail views"
  const wchar_t* ShortDescription( bool bPlural ) const override;

  // virtual CRhinoObject::EnableGrips override
  void EnableGrips( bool bGripsOn ) override;

  void Draw(CRhinoDisplayPipeline& dp) const override;

  bool PickFilter( const CRhinoGetObject&, bool ) const override;
  int Pick( const CRhinoPickContext&, CRhinoObjRefArray& ) const override;
  
  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const override;

  bool GetTightBoundingBox( 
    ON_BoundingBox& tight_bbox, 
    bool bGrowBox = false,
    const ON_Xform* xform = nullptr
    ) const override;

  bool PrepareToWrite( int archive_3dm_version ) override;

  void AddToDocNotification() override;

  bool SetDetailView( const ON_DetailView& detail_view );

  const ON_DetailView& GetDetailView() const;

  // No support for any boundary other than rectangles at this point
  bool SetRectangleBoundary( ON_2dPoint corner1, ON_2dPoint corner2 );

  //Description:
  // Set the boundary curve for this detail object
  //Parameters:
  // curve - [in] must be simple closed 2D curve on XY Plane
  bool SetBoundaryCurve( const ON_Curve& curve );

  //Description:
  // The 2d boundary for this detail in page coordinates
  const ON_NurbsCurve& BoundaryCurve() const;

  // The nested viewport
  // The viewport's ID should always match the attributes ID
  CRhinoViewport& Viewport();
  const CRhinoViewport& Viewport() const;

  // Description:
  //   Every detail view object belongs (displays) to one page view.
  //   Page views can be parent to multiple detail views.
  // Return:
  //   The CRhinoPageView that this detail view belongs to
  CRhinoPageView* ParentPageView() const;

  // Description:
  //   Set the detail viewport's projection so geometry is displayed at a certain scale
  // Parameters:
  //   model_length: [in] reference model length in current model units
  //   paper_length: [in] length on page that the model_length should equal in the current page units
  // Returns:
  //   true on success
  //   false if the viewport's projection is perspective or the input values do not make sense
  bool SetScale(double model_length, double paper_length);

  // Description:
  //   Set the detail viewport's projection so geometry is displayed at a certain scale
  // Parameters:
  //   model_length: [in] reference model length
  //   model_units:  [in] units for model length
  //   paper_length: [in] length on page that the model_length should equal
  //   paper_units:  [in] units for paper length
  // Returns:
  //   true on success
  //   false if the viewport's projection is perspective or the input values do not make sense
  bool SetScale(double model_length, const ON_UnitSystem& model_units,
                double paper_length, const ON_UnitSystem& paper_units);

  void InvalidateScale();

  // Description:
  //   Detail scale formats.
  enum class ScaleFormat : unsigned int
  {
    None,                         // No formattng
    PageLengthToOne,              // #:1
    OneToModelLength,             // 1:#
    OneInchToModelLengthFeet,     // 1" = #'
    ModelLengthInchToOneFoot,     // #" = 1'
    ModelLengthInchToOneFootInch  // #' = 1'-0"
  };
  static ScaleFormat ScaleFormatFromUnsigned(unsigned int scale_format_as_unsigned);

  // Description:
  //   Returns the detail's scale as a formatted string
  // Parameters:
  //   format: [in] The scale format.
  //   str:    [out] The formatted string.
  // Returns:
  //   true if successful
  bool GetFormattedScale(ScaleFormat format, ON_wString& str) const;

  bool GetModelLength( double paper_length, double& model_length ) const;
  bool GetPaperLength( double model_length, double& paper_length ) const;

  // Description:
  //   Return true if the screen point is inside of the detail's boundary curve.
  // Parameters:
  //   parent_vp: [in] Page view that this detail is shown in. This function is called
  //              inside of page view mouse events and passing the parent viewport is done
  //              purely for speed (don't have to look up the parent page)
  //   screen_pt: [in] point to test
  bool ScreenPointInBoundary(const CRhinoViewport& parent_vp, const ON_2iPoint& screen_pt);

  void DollyExtents();

  // Description:
  //   Get detail_world2page transformation.
  //   This transformation that maps world coordinates to page
  //   coordinates with depth from near (z=1) to far (z= -1).
  // Parameters:
  //   pWorldToPage [out]  Pointer to ON_Xform to fill in with world to page transform or NULL
  //   pPageToWorld [out]  Pointer to ON_Xform to fill in with page to world transform or NULL
  // Added 12/10/2009 Lowell
  bool GetPageXforms(ON_Xform* pWorldToPage, ON_Xform* pPageToWorld) const;

  static bool GetDetailXform(UINT sn, const CRhinoDoc* pDoc, ON_Xform* pWorldToPage, ON_Xform* pPageToWorld);

  // Description:
  //   Detail objects have two strings that can be used to describe the detail
  //   1. The m_name string that is part of the object's attributes
  //   2. The viewport projection title that is part of the ON_3dmView
  //   This function combines these two strings to create a single "description"
  //   string in the form of
  //   attribute_name - projection_title
  //   In the case where the attributes name is empty, then
  //   Detail - projection_title
  //   is returned.
  void DescriptiveTitle(ON_wString& str) const;
private:
  void DrawNestedViewport(CRhinoDisplayPipeline& parent_pipe, bool bActiveViewport, ON_Viewport* out_nestedViewport = 0) const;
  void DrawNestedViewportNew(CRhinoDisplayPipeline& parent_pipe, bool bActiveViewport) const;

  void UpdateViewport(const ON_2dPoint& screen_pt0, const ON_2dPoint& screen_pt1, bool active_vp);

  // Description:
  //   Gets linetype scale required to draw this detail view with proper
  //   linetypes on a page
  double GetLineTypeScale() const;

  void SetupDetailPipeline(CRhinoDisplayPipeline& src_pipe, bool bCopyBindings);
  void SetupDetailPipelineConst(const CRhinoDisplayPipeline& src_pipe, bool bCopyBindings);

  // m_detail_view.m_boundary is the bounding shape for the detail. 
  //   Current implementation assumes a rectangle.
  //   The boundary should always be a planar (on x-y plane) closed curve.
  // m_detail_view.m_view is ONLY used for saving the value in 3dm files.
  //   ALL RUNTIME CALCULATIONS USE m_pViewport->m_v.
  //   PrepareToWrite copies m_pViewport->m_v to m_detail_view.m_view.
  ON_DetailView m_detail_view;

  // Every Detail has a unique viewport and associated display pipeline
  // The viewport member variable is maintained as a pointer in order to maintain
  // display pipeline associations in the case of a "memory relocate"
  CRhinoViewport*        m_pViewport;
  // We use the parent viewport's display pipeline for drawing, but need a separate
  // detail viewport display pipeline, so users can define different display attributes
  // for details
  //CRhinoDisplayPipeline* m_pDisplayPipeline;

  void ValidateGeometry();
  friend class CRhinoDoc;
  friend class SkylightShadowsTilePrintHelper;
public:
  static bool s_bInPrintPreview;
  static void SetInPrintPreview(bool on);

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

/*
Description:
  Tool to go through the CRhinoDetailViewObject on a specified page.
*/
class RHINO_SDK_CLASS CRhinoDetailViewIterator
{
public:
  CRhinoDetailViewIterator(
    unsigned int rhino_page_view_serial_number
    );
  ~CRhinoDetailViewIterator();

  CRhinoDetailViewObject* First();
  CRhinoDetailViewObject* Next();

private:
  // Ideally, this implementation will change before the V6
  // SDK is frozen.
  CRhinoPageViewObjectIterator m_dit;

  CRhinoDetailViewIterator(const CRhinoDetailViewIterator&) = delete; // no implementation
  CRhinoDetailViewIterator& operator=(const CRhinoDetailViewIterator&) = delete; // no implementation
};
