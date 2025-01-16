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
// CRhinoPageView view
//
// A Page view represents a single sheet of paper with embedded detail views.
// Rules:
// - Always a "Top" 2D View
// - Geometry in page view is always drawn on top of it's embedded detail views
// - Geometry defined in a page view is ONLY drawn in this one page view.
class RHINO_SDK_CLASS CRhinoPageView : public CRhinoView
{
protected:
  CRhinoPageView(class CRhViewBase& view);           // protected constructor used by dynamic creation
  CRhinoPageView(class CRhViewBase* view);
  virtual ~CRhinoPageView();

  virtual bool IsPageView(void) const override;

public:
  /*
  Description:
    Get a page view from a runtime serial number
  Parameters:
    rhino_view_sn - [in]
      View runtime serial number.
  Return:
    A pointer to the page view or null if there is not
    a page view with the specified serial number.
  */  
  static CRhinoPageView* FromRuntimeSerialNumber(
    unsigned int rhino_view_sn
    );

  /*
  Description:
    Get a page view from a main viewport id.
  Parameters:
    main_viewport_id - [in]
      Model views have a single viewport which
      is both the main viewport and
      Page views, this is the id of the large
      view that may contain multiple details.

      main_viewport id      
  Return:
    A pointer to the pate view or null if there is not
    a page view with the specified main viewport id.
  */
  static CRhinoPageView* FromMainViewportId(
    ON_UUID main_viewport_id
    );
  
  /*
  Parameters:
    view - [in]
      pointer value
  Returns:
    A pointer to a CRhinoPageView if view points to a CRhinoPageView class.
  */
  static CRhinoPageView* FromPointer(
    void* view
    );

  bool SetActiveDetailObject(const class CRhinoDetailViewObject* pDVO);
  bool SetActiveDetailObject(ON_UUID object_id);
  ON_UUID ActiveDetailObject() const;
  class CRhinoDetailViewObject* FindActiveDetailObject();

  CRhinoViewport& ActiveViewport() const override;
  ON_UUID ActiveViewportID() const override;

  // Description:
  //  Create a detail view object that is displayed on this page and adds it to the doc
  // Parameters:
  //  corner0, corner1: [in] corners of the detail view in world coordinates
  //  corner_units: [in] unit system corners are supplied in. If NULL, the document's page units are used
  // Returns:
  //  NULL on failure
  // Example:
  //  The corner_units are supplied as a parameter so you can define the details position
  //  on a page. The page always has it's lower left corner at 0,0. If you wanted to add
  //  a detail that is 1/2 inch from the bottom and left edges, and is 6"wide x 3"tall
  //  ON_2dPoint bottom_left(0.5,0.5);
  //  ON_2dPoint top_right(6.5,3.5);
  //  ON_UnitSystem units(ON::LengthUnitSystem::Inches);
  //  int index = AddDetailView(bottom_left, top_right, &units);
  class CRhinoDetailViewObject* AddDetailView(const ON_2dPoint& corner0,
                                     const ON_2dPoint& corner1,
                                     const ON_UnitSystem* corner_units = NULL);

  /*
  Description:
    Get list of detail view objects that are displayed on this page
  Parameters:
    details: [out] list to fill
  Returns:
    Number of detail view objects in list
  Remarks:
    If you are going to use a for() loop to go throught details[],
    consider using a CRhinoDetailViewObjectIterator instead.
  */
  int GetDetailViewObjects(ON_SimpleArray<class CRhinoDetailViewObject*>& details) const;

  // Description:
  //   Each page has a paper size used for WYSIWYG printing of page layouts. This
  //   function gets the paper size for this page.
  // Parameters:
  //   units: [in] units system for getting the page size. If units = ON::LengthUnitSystem::Inches,
  //          the width and height are distances in inches
  //   width, height: [out] paper size in provided units
  void GetPaperSize(ON::LengthUnitSystem units, double& width, double& height) const;

  // Description:
  //   Each page has a paper size used for WYSIWYG printing of page layouts. This
  //   function sets the paper size for this page.
  // Parameters:
  //   units: [in] units system for setting the page size. If units = ON::LengthUnitSystem::Inches,
  //          the width and height are distances in inches
  //   width, height: [in] paper size in provided units
  // Returns:
  //   true if valid input parameters are provided (width and height are > 0)
  bool SetPaperSize(ON::LengthUnitSystem units, double width, double height);

  // Description:
  //   Get the number of pixels per inch of page for the current display viewport
  // Returns:
  //   pixels of screen display per inch page
  double GetPixelsPerInch() const;

  // Description:
  //   Get the runtime page number. The first page returns a value of 0
  int GetPageNumber() const;
  // Description:
  //   Sets the runtime page number and updates the page number for all of the other pages
  void SetPageNumber(int page);

  // Description:
  //   Creates a preview image of the page.
  // Parameters:
  //    image_size - [in] The size of the preview image to create.
  //    bGrayScale - [in] Set true to produce a grayscale image, false to produce a color image.
  //    dib - [out] The output image as a dib.
  //  Returns:
  //    true if successful, false otherwise.
  bool GetPreviewImage(ON_2iSize image_size, bool bGrayScale, CRhinoDib& dib);
  
  // Description:
  //   A page view overrides the default implementation of this function because it uses a
  //   custom display pipeline.
  // See:
  //   CRhinoView::GetDefaultDisplayPipelineClass
  const ON_ClassId* GetDefaultDisplayPipelineClass() const override;

  // scale == 0, don't perform any special linetype scaling during draw
  // scale == 1, linetypes show on page and details at scale pattern defined
  // For other values scale is multiplied against the actual linetype scale.
  // This is useful when print at 1/2 size and displaying the linetypes at 1/2 size
  void SetLinetypeDrawScaleFactor(double scale);
  double GetLinetypeDrawScaleFactor() const;

  // 21 June, 2010 - Lowell - Adding a function to update the page space dimensions that refer 
  // to geometry in the detail view that just got changed
  //void UpdatePageViewDimensions();
protected:
  //void GetPageViewDimensions(const CRhinoViewport& vp, ON_SimpleArray<CRhinoObject*>& dimensions);
  //CRhinoObject* ReplayDimensionHistory(CRhinoObject* pDim);

  int RhinoViewType() const override;


protected:
  // This is normally ON_nil_uuid. In cases where nested detail viewports are the active
  // viewport for this view, this equals the object id of the CRhinoDetailViewObject that
  // holds the viewport information. Standard CRhinoViews do not support nested viewports
  ON_UUID m_active_detail_object_id;

  // Runtime scaling factor used for display of linetypes on a page
  double m_dLineTypeScaleFactor = 0;
  
  // static list used to define the page order (page1, page2, page3, ...)
  static ON_SimpleArray<CRhinoPageView*> m_page_order;
private:
  ON__UINT_PTR m_reserved = 0;

  friend class CRhinoApp;
  friend class CRhPageView;
};
