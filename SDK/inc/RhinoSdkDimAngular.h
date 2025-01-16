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

class RHINO_SDK_CLASS CRhinoDimAngular : public CRhinoDimension
{
  ON_OBJECT_DECLARE(CRhinoDimAngular);
public:

  enum class definition_mode : unsigned char
  {
    None = 0,
    Lines = 1,  // picked lines or line segments
    Arc = 2,    // picked an arc
    Points = 3  // three 3d points
  };
  static CRhinoDimAngular::definition_mode DefinitionModeFromUnsigned(unsigned int definition_mode_as_unsigned);

  enum class point_type : unsigned char
  {
    Unset = 0,
    Def1 = 1,
    Def2 = 2,
    Arrow1 = 3,
    Arrow2 = 4,
    Text = 5,
    Center = 6,
    DimLine = 7
  };
  static CRhinoDimAngular::point_type PointTypeFromUnsigned(unsigned int grip_type_as_unsigned);

public:
  CRhinoDimAngular();
  CRhinoDimAngular(const CRhinoDimAngular& src);
  CRhinoDimAngular(const ON_3dmObjectAttributes& atts);

  const wchar_t* ShortDescription(bool bPlural) const override;

  void SetDimension(const ON_DimAngular* dim);
  void SetDimension(ON_DimAngular* dim);
  const ON_DimAngular* GetDimension() const;

  int Pick(
    const CRhinoPickContext& context,
    CRhinoObjRefArray& picklist) const override;

  bool SnapTo(
    const CRhinoSnapContext& snap_context,
    CRhinoSnapEvent& snap_event
    ) const override;

  void EnableGrips(bool bGripsOn) override;

  void UpdateBoundingBox(const CRhinoViewport* vp) override;

  const ON_TextContent* UpdateDimensionText(const CRhinoDoc* doc);

  void Draw(CRhinoDisplayPipeline& dp) const override;

  ON_DEPRECATED_MSG("Do not user this version of Draw")
  void Draw(CRhinoDisplayPipeline& dp, const CRhinoObjectAttributes attribs) const;

  definition_mode DefinitionMode() const;
  void SetDefinitionMode(definition_mode mode);

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;

private:
  definition_mode m_definition_mode = definition_mode::None;
};

// Class to interactively get user input for angular dimensions
class RHINO_SDK_CLASS CRhinoGetDimAngular
{
public:
  // Description:
  //   Gets an angular dimension from user input
  //   Will allocate dim_out memory which caller must delete
  // Parameters:
  //   doc_sn,          [in]     Serial no of the document for annotation styles
  //   output_dimension [out]    Resulting dimension
  //   objrefs[3],      [out]    ObjRefs of objects osnapped to or object picked to dimension
  //   bInteractive,    [in]     true: run in interactive mode
  //                             false - run in script mode
  //   def_mode,        [out]    Lines - picked lines or line segments
  //                             Arc - picked an arc
  //                             Points - picked three 3d points
  //   picked_ob[3],    [out]    If picked_ob[i] is true, objref[i] has information about the object picked
  //   lines[2]         [out]    If lines or line segments were picked, their endpoints are here
  //   pickpoints[2]    [out]    If lines or line segments were picked, this is where they were picked
  //   centerpoint      [out]    Center of the dimension
  //   arcpoint         [out]    Point on the dimension arc
  //
  // Returns:
  //   Failure:  No dimension gotten
  //   Success:  Got a dimension

  static CRhinoCommand::result GetDimAngular(
    const unsigned int doc_sn,
    ON_DimAngular*& output_dimension,
    bool bInteractive,
    CRhinoDimAngular::definition_mode& def_mode,
    bool picked_ob[3],        // Set true if objects are picked in point mode
    CRhinoObjRef objrefs[3],
    ON_Line lines[2],
    ON_3dPoint pickpoints[2], // Pick points on lines
    ON_3dPoint& centerpoint,  // Apex of the dimension
    ON_3dPoint& arcpoint);    // Pick point on arc

public:
  // Description:
  //   Drag the dimension arc of an angular dimension and 
  //   draw the dynamic feedback
  // Parameters:
  //   output_dimension [in/out]  - Pointer to an allocated dimension to drag and update.
  //                                The dimension must be created with the proper dimstyle and plane
  //                                If the dragging is successful, the dimension will be updated
  // Returns:          
  //   true - Dragging worked
  //   false - Dragging failed

  static bool DragDimAngleArc(
    ON_DimAngular*& output_dimension);

};

