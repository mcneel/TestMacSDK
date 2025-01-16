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

class RHINO_SDK_CLASS CArgsRhinoHatch
{
public:
	CArgsRhinoHatch() = default;

  // Sets
  void SetPatternIndex( int index);
  void SetPatternRotation( double rotation);
  void SetPatternScale( double scale);
  void SetView( const CRhinoView* view);

  // Gets
  int PatternIndex() const;
  double PatternRotation() const;
  double PatternScale() const;
  const CRhinoView* View() const;

  
  ON_SimpleArray<const ON_Curve*> m_loops;

protected:
  int m_pattern_index = -1;
  double m_pattern_rotation = 0.0;
  double m_pattern_scale = 1.0;
  const CRhinoView* m_view = nullptr;
};



class RHINO_SDK_CLASS CRhinoHatch : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoHatch);
public:

  // Constructors
  CRhinoHatch();                              // assigns a new object UUID
  CRhinoHatch(const ON_3dmObjectAttributes& src); // assigns a new object UUID
  
  // Destructor
  ~CRhinoHatch();
  
  CRhinoHatch( const CRhinoHatch& src);          // copies source object UUID
  CRhinoHatch& operator=(const CRhinoHatch& src); // copies source object UUID

   /*
  Description:
    Returns a constant that determines the type of
    CRhinoObject.
  Returns:
    ON::hatch_object
  */
  ON::object_type ObjectType() const override;

  /*
  Description:
    Returns the index of the hatch's fill pattern
    in the HatchPatternTable
  Returns:
    The index
  */
  int PatternIndex() const;

  /*
  Description:
    Gets the rotation applied to the hatch pattern 
    when it is mapped to the hatch's plane
  Returns:
    The rotation in radians
  Remarks:
    The pattern is rotated counter-clockwise around
    the hatch's plane origin by this value
  */
  double PatternRotation() const;

/*
  Description:
    Sets the rotation applied to the hatch pattern 
    when it is mapped to the hatch's plane
  Parameters:
    rotation (I) - The rotation in radians
  Remarks:
    The pattern is rotated counter-clockwise around
    the hatch's plane origin by this value
  */
  void SetPatternRotation( double rotation);
  
  /*
  Description:
    Gets the scale applied to the hatch pattern 
    when it is mapped to the hatch's plane
  Returns:
    The scale
  Remarks:
    The pattern is scaled around
    the hatch's plane origin by this value
  */
  double PatternScale() const;
  // 11 Nov, 2010 - Lowell - Added to support paper space scaling of hatches
  double PatternScale(const CRhinoViewport* pVP) const;

/*
  Description:
    Sets the scale applied to the hatch pattern 
    when it is mapped to the hatch's plane
  Parameters:
    scale (I) - The scale
  Remarks:
    The pattern is scaled around
    the hatch's plane origin by this value
  */
  void SetPatternScale( double scale);

   /*
  Description:
    Returns the fill type of the hatch's pattern
  Returns:
    The fill type
  */
  ON_HatchPattern::HatchFillType FillType() const;

  /*
    Description:
      Specify the ON_Hatch geometry to be used by the Rhino Object
    Parameters:
      hatch - [in] hatch geometry copied to object
    Returns:
      Pointer to hatch geometry on CRhinoHatchObject.
  */
  ON_Hatch* SetHatch( const ON_Hatch& hatch);

  /*
     Description:
       Specify the hatch geometry.
    Parameters:
       pHatch - [in] ~CRhinoHatchObject() will delete this hatch
  */
  void SetHatch( ON_Hatch* hatch );

  /*
    Description:
      Get a pointer to the hatch geometry
    Parameters:
  */
  const ON_Hatch* Hatch() const;
  
  /*
    Description:
      Draw the object in a viewport
    Parameters:
      [in] CRhinoViewport& vport - The viewport in which to draw
    Remarks:
      The viewport has projection information and tools for drawing 
      primitive objects on the screen that are display mode independent
  */
  void Draw( CRhinoDisplayPipeline& dp) const override;

  /*
    Description:
      Test if the object was hit by the pick event described in the PickContext
      and add it to the picklist if it was
    Parameters:
      [in] CRhinoPickContext& - Information about the current pick event
      [out] CRhinoObjRefArray& - Array for collecting objects that are picked
    Returns:
      @untitled table
      1     Object was hit
      2     Object was not hit
  */
  int Pick( const CRhinoPickContext&, CRhinoObjRefArray& objref) const override;

  //bool PickFilter(const CRhinoGetObject& go, bool bCheckSubObjects) const;


  /*
  Description:
    virtual CRhinoObject::SnapTo override
  Remarks:
    Returns false
  */
  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const override;

  /*
  Description:
    virtual CRhinoObject::ShortDescription override
  Returns:
    "hatch"
  */
  const wchar_t* ShortDescription( bool bPlural) const override;

  // virtual CRhinoObject::GetSubObjects override
  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const CRhinoViewport* pViewport = nullptr
          ) const override;

  ON_BoundingBox BoundingBox() const override;

  bool GetTightBoundingBox( 
		ON_BoundingBox& tight_bbox, 
    bool bGrowBox,
		const ON_Xform* xform
    ) const override;

  /*
  Description:
    Remove all of the loops on the hatch and add the curves in 'loops' as new loops
  Parameters:
    loops - [in] An array of pointers to 2d or 3d curves
                 If the curves are 2d, add them to the hatch directly
                 If they are 3d, project them to the hatch's plane first
  Returns:
    true  - success
    false - no loops in input array or an error adding them
  */
  bool ReplaceLoops(ON_SimpleArray<ON_Curve*>& loops);
  bool ReplaceLoops(ON_SimpleArray<const ON_Curve*>& loops);

  // Delete cached display information with this object is placed on the undo list
  // virtual
  void DeleteFromDocNotification() override;
  void EnableGrips(bool bGripsOn) override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

private:
  friend class CRhHatchObjectPrivate;
  class CRhHatchObjectPrivate* m_private_hatch = nullptr;
};


/*
Description:
  Creates one or more hatches.
Parameters:
  args      - [in] The hatching arguments.
  tolerance - [in] The tolerance used for detecting planar loops.
                   When in doubt, use the document's absolute tolerance.
Returns:
  True if one or more hatches were created, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCreateHatches(
  CArgsRhinoHatch& args,
  double tolerance,
  ON_SimpleArray<ON_Hatch*>& results
  );

/*
Description:
  Creates one or more hatches.
Parameters:
  args      - [in] The hatching arguments.
  tolerance - [in] The tolerance used for detecting planar loops.
                    When in doubt, use the document's absolute tolerance.
Returns:
  True if one or more hatches were created, false otherwise.
*/
ON_DEPRECATED_MSG("use overload that accepts tolerance parameter")
RHINO_SDK_FUNCTION
bool RhinoCreateHatches(
  CArgsRhinoHatch& args,
  ON_SimpleArray<ON_Hatch*>& results
  );

