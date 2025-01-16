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

class RHINO_SDK_CLASS CRhinoDimLinear : public CRhinoDimension
{
  ON_OBJECT_DECLARE(CRhinoDimLinear);
public:
  enum class continue_mode : unsigned char
  {
    None = 0,
    Continue = 1,
    Baseline = 2,
  };

  static CRhinoDimLinear::continue_mode ContinueModeFromUnsigned(unsigned int continue_as_unsigned);

  // This is not quite the same thing as ON_Dimension::DimensionType.
  // This is only for the interface mode for how the dimension angle is set
  enum class rotation_mode : unsigned char
  {
    Ortho = 0,   // Horizontal or vertical
    Rotated = 1,
    Aligned = 2
  };

  static CRhinoDimLinear::rotation_mode RotationModeFromUnsigned(unsigned int rotation_as_unsigned);

  enum class point_type : unsigned char
  {
    Unset = 0,
    Def1 = 1,
    Def2 = 2,
    Arrow1 = 3,
    Arrow2 = 4,
    Text = 5
  };

  static CRhinoDimLinear::point_type PointTypeFromUnsigned(unsigned int grip_type_as_unsigned);

  CRhinoDimLinear();
  CRhinoDimLinear(const CRhinoDimLinear& src);
  CRhinoDimLinear(const ON_3dmObjectAttributes& atts);

  const wchar_t* ShortDescription(bool bPlural) const override;

  void SetDimension(const ON_DimLinear* dim);
  void SetDimension(ON_DimLinear* dim);
  const ON_DimLinear* GetDimension() const;

  void UpdateBoundingBox(const CRhinoViewport* vp) override;

  const ON_TextContent* UpdateDimensionText(
    const CRhinoDoc* doc,
    bool in_page_space);

  int Pick(
    const CRhinoPickContext& context,
    CRhinoObjRefArray& picklist) const override;

  bool SnapTo(
    const CRhinoSnapContext& snap_context,
    CRhinoSnapEvent& snap_event
  ) const override;

  void EnableGrips(bool bGripsOn) override;

  void Draw(CRhinoDisplayPipeline& dp) const override;

  ON_DEPRECATED_MSG("Do not user this version of Draw")
  void Draw(CRhinoDisplayPipeline& dp, const CRhinoObjectAttributes attribs) const;

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;
};


// Class to interactively get user input for linear dimensions
class RHINO_SDK_CLASS CRhinoGetDimLinear
{
public:

  // Description:
  //   Gets a dimension rotated to either horizontal or vertical in the cplane
  //   Will allocate dim_out memory which caller must delete
  // Parameters:
  //   doc_sn,          [in]     Serial no of the document for annotation styles and continued dimensions
  //   output_dimension [out]    Resulting dimension
  //   objrefs[2],      [out]    ObjRefs of objects osnapped to or object picked to dimension
  //   bInteractive,    [in]     true: run in interactive mode
  //                             false - run in script mode
  //   rotation_mode,   [in]     Ortho -   dimension line is horizontal or vertical
  //                             Rotated - dimension line is rotated to a specified angle
  //                             Aligned - dimension line is parallel to line between definition points
  //   rotation_angle,  [in]     Angle in radians if RotationMode == Rotated
  //   continue_mode,   [in/out] None - get a single dimension
  //                             Continue - run in continue mode
  //                             Baseline - run in baseline mode
  // Returns:
  //   Failure:  No dimension gotten
  //   Success:  Got a dimension
  //   Undo:     User requested an undo in continue or baseline mode
  static
    CRhinoCommand::result GetDimLinear(
      const unsigned int doc_sn,
      ON_DimLinear*& output_dimension,
      CRhinoObjRef objrefs[2],
      bool bInteractive,
      CRhinoDimLinear::rotation_mode& rotation_mode,
      double& rotation_angle,
      CRhinoDimLinear::continue_mode& continue_mode);

protected:
  // Undo record for GetDimLinear baseline and continue modes
  class RhDocDimension
  {
  public:
    unsigned int m_doc_sn = 0;
    ON_UUID m_dim_id = ON_nil_uuid;
    int m_direction = 0;
  };
  // Functions used by GetDimLinear for baseline and continue modes
  static const RhDocDimension* GetDimRecToContinue(unsigned int doc_sn);
  static const CRhinoDimLinear* DimToContinue(unsigned int doc_sn, int& direction);
  static void SetDimToContinue(unsigned int doc_sn, ON_UUID dim_id, int direction);
  static void ClearDimToContinue(unsigned int doc_sn);
  static const CRhinoDimLinear* SelectDimToContinue(unsigned int doc_sn);

private:
  static ON_ClassArray<RhDocDimension> m_to_be_continued;
};

