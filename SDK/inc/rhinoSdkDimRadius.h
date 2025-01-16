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

class RHINO_SDK_CLASS CRhinoDimRadial : public CRhinoDimension
{
  ON_OBJECT_DECLARE(CRhinoDimRadial);
public:
  enum class point_type : unsigned char
  {
    Unset = 0,
    Center = 1,
    Arrow = 2,
    Text = 3
  };
  static CRhinoDimRadial::point_type PointTypeFromUnsigned(unsigned int grip_type_as_unsigned);

  CRhinoDimRadial();
  CRhinoDimRadial(const CRhinoDimRadial& src);
  CRhinoDimRadial(const ON_3dmObjectAttributes& atts);

  const wchar_t* ShortDescription(bool bPlural) const override;

  void SetDimension(const ON_DimRadial* dim);
  void SetDimension(ON_DimRadial* dim);
  const ON_DimRadial* GetDimension() const;

  void UpdateBoundingBox(const CRhinoViewport* vp) override;

  int Pick(
    const CRhinoPickContext& context,
    CRhinoObjRefArray& picklist) const override;

  bool SnapTo(
    const CRhinoSnapContext& snap_context,
    CRhinoSnapEvent& snap_event
    ) const override;

  void EnableGrips(bool bGripsOn) override;

  const ON_TextContent* UpdateDimensionText(
    const CRhinoDoc* doc,
    bool in_page_space);

  void Draw(CRhinoDisplayPipeline& dp) const override;

  ON_DEPRECATED_MSG("Do not user this version of Draw")
  void Draw(CRhinoDisplayPipeline& dp, const CRhinoObjectAttributes attribs) const;

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;


};




// Class to interactively get user input for radial dimensions
class RHINO_SDK_CLASS CRhinoGetDimRadial
{
public:
  // Description:
  //   Gets a radial dimension from user input
  //   Will allocate dim_out memory which caller must delete
  // Parameters:
  //   doc_sn,          [in]     Serial no of the document for annotation styles
  //   output_dimension [out]    Resulting dimension
  //   point_ref,       [out]    ObjRef of object selected to dimension
  //   bInteractive,    [in]     true: run in interactive mode
  //                             false - run in script mode
  //   bGetDiameter     [in]     true - Get a diameter dimension
  //                             false - Get a radius dimension
  // Returns:
  //   Failure:  No dimension gotten
  //   Success:  Got a dimension

  static CRhinoCommand::result GetDimRadial(
    unsigned int doc_sn,
    ON_DimRadial*& dim_out,
    CRhinoObjRef& point_ref,
    bool bInteractive,
    bool GetDiameter);

};




