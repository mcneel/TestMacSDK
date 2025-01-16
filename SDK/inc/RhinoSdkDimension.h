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

// Putting one of these on the stack before checking if history is on
// and writing a history record will turn history on temporarily if it's not
// already on, and then turn it back off if it was off when this is destructed
// It's here so history can easily be turned on whenever dimensions are made
class RHINO_SDK_CLASS CRhinoDimensionHistoryEnabler
{
public:
  CRhinoDimensionHistoryEnabler(unsigned int doc_sn);
  ~CRhinoDimensionHistoryEnabler();

private:
  static bool m_force_dimension_history;
public:
  bool m_history_on = false;
  unsigned int m_doc_sn = 0;

  static void EnableRecordAnnotationHistory(bool enable);
  static bool RecordAnnotationHistory();
};



class RHINO_SDK_CLASS CRhinoDimension : public CRhinoAnnotation
{
  ON_OBJECT_DECLARE(CRhinoDimension);
protected:
  CRhinoDimension() = default;
  CRhinoDimension(const CRhinoDimension& src);
  CRhinoDimension(const ON_3dmObjectAttributes& atts);

public:
  ON::object_type ObjectType() const override;

  static bool TextPlaneIsVisible(const ON_Viewport& vp, const ON_Plane& plane);
  bool IsVisibleInViewport(CRhinoDisplayPipeline& dp) const;
  bool IsVisibleInViewport(const CRhinoViewport& vp) const;
  bool IsVisibleInViewport(const CRhinoViewport& vp, const ON_Xform& xform) const;

  static
    bool PlaneIsVisibleInViewport(const ON_Viewport& vp, const ON_Plane& plane);

  static
    int GetFrontDrawNumber(const CRhinoDoc& doc);

  void ClearBoundingBox() const;
  
  // Destroy and remake bounding box
  virtual void UpdateBoundingBox(const CRhinoViewport* vp);

  bool  SetPlane(const ON_Plane& plane);
  const ON_Plane& Plane() const;

  // Added this function to force updating text on dimensions added to a 
  // CRhinoDoc, but not drawn yet.
  // Somebody wrote a script to add a dimension and query it's length
  // with document drawing turned off
  // Normally text is generated on demand when dimensions are drawn
  void ForceTextUpdate() const;

};

class RHINO_SDK_CLASS CRhinoCentermark : public CRhinoDimension
{
  ON_OBJECT_DECLARE(CRhinoCentermark);
public:
  enum class point_type : unsigned char
  {
    Unset = 0,
    Center = 1
  };
  static CRhinoCentermark::point_type GripTypeFromUnsigned(unsigned int grip_type_as_unsigned);

  CRhinoCentermark();
  CRhinoCentermark(const CRhinoCentermark& src);
  CRhinoCentermark(const ON_3dmObjectAttributes& atts);

  const wchar_t* ShortDescription(bool bPlural) const override;

  void SetDimension(const ON_Centermark* dim);
  void SetDimension(ON_Centermark* dim);
  const ON_Centermark* GetDimension() const;

  void EnableGrips(bool bGripsOn) override;

  int Pick(
    const CRhinoPickContext& context,
    CRhinoObjRefArray& picklist) const override;

  bool SnapTo(
    const CRhinoSnapContext& snap_context,
    CRhinoSnapEvent& snap_event
    ) const override;

  void Draw(
    CRhinoDisplayPipeline& dp) const override;

  void Draw(
    CRhinoDisplayPipeline& dp,
    const CRhinoObjectAttributes attribs) const;

};


class RHINO_SDK_CLASS CRhinoDimOrdinate : public CRhinoDimension
{
  ON_OBJECT_DECLARE(CRhinoDimOrdinate);
public:
  enum class point_type : unsigned char
  {
    Unset = 0,
    Base = 1,
    Def = 2,
    Ldr = 3,
    Kink1 = 4,
    Kink2 = 5
  };
  static CRhinoDimOrdinate::point_type GripTypeFromUnsigned(unsigned int grip_type_as_unsigned);

  CRhinoDimOrdinate();
  CRhinoDimOrdinate(const CRhinoDimOrdinate& src);
  CRhinoDimOrdinate(const ON_3dmObjectAttributes& atts);

  const wchar_t* ShortDescription(bool bPlural) const override;

  //void EnableGrips(bool bGripsOn) override;

  void SetDimension(const ON_DimOrdinate* dim);
  void SetDimension(ON_DimOrdinate* dim);
  const ON_DimOrdinate* GetDimension() const;

  void UpdateBoundingBox(const CRhinoViewport* vp) override;

  const ON_TextContent* UpdateDimensionText(
    const CRhinoDoc* doc,
    bool in_page_space);

  void EnableGrips(bool bGripsOn) override;

  int Pick(
    const CRhinoPickContext& context,
    CRhinoObjRefArray& picklist) const override;

  bool SnapTo(
    const CRhinoSnapContext& snap_context,
    CRhinoSnapEvent& snap_event
    ) const override;

  void Draw(
    CRhinoDisplayPipeline& dp) const override;

  void Draw(
    CRhinoDisplayPipeline& dp,
    const CRhinoObjectAttributes attribs) const;

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;

};


// Class to interactively get user input for radial dimensions
class RHINO_SDK_CLASS CRhinoGetDimOrdinate
{
public:
  // Description:
  //   Gets an ordinate dimension from user input
  //   Will allocate dim_out memory which caller must delete
  // Parameters:
  //   doc_sn,          [in]     Serial no of the document for annotation styles
  //   output_dimension [out]    Resulting dimension
  //   point_ref,       [out]    ObjRef of object selected to dimension
  //   basepoint,       [in/out] Reference point for dimensions
  //   dimension_plane  [in]     Plane where dimension is made
  //   kink_offset      [in/out] Distance perpendicular to measured direction between kinks
  //                             when dimenson is offset from point being dimensioned
  //   direction        [in/out] Xaxis - dimension meadures parallel to x axis
  //                             yaxis - dimension meadures parallel to y axis
  //   bBaseOverride    [out]    true if basepoint has been changed
  //   bInteractive,    [in]     true: run in interactive mode
  //                             false - run in script mode
  // Returns:
  //   Failure:  No dimension gotten
  //   Success:  Got a dimension

  static
    CRhinoCommand::result GetDimOrdinate(
      unsigned int doc_sn,
      ON_DimOrdinate*& output_dimension,
      CRhinoObjRef& point_ref,
      ON_3dPoint& basepoint,
      ON_Plane dimension_plane,
      double& kink_offset,
      ON_DimOrdinate::MeasuredDirection& direction,
      bool& bBaseOverride,
      bool bInteractive);
};


/*
Description:
  Explodes a dimension into its component parts.
Parameters:
  leader - [in] The dimension to explode.
  dimstyle - [in] The dimension's effective dimension style.
  out_curves - [out] The output curves.
  out_objects - [out] The output text and instance references (if the dimension is using user-defined arrowheadsrowheads)
Returns:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoExplodeDimension(
  const ON_Dimension* dimension,
  const ON_DimStyle* dimstyle,
  ON_SimpleArray<ON_Curve*>& out_curves,
  ON_SimpleArray<ON_Geometry*>& out_objects
);
