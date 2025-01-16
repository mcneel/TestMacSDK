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
#include "rhinoSdkFont.h"

class RHINO_SDK_CLASS CRhinoTextDot : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoTextDot);
public:
  // Constructors
  CRhinoTextDot();                                    // assigns a new object UUID
  CRhinoTextDot(const ON_3dmObjectAttributes& src);   // assigns a new object UUID
  CRhinoTextDot(const CRhinoTextDot& src);            // copies source object UUID
  CRhinoTextDot& operator=(const CRhinoTextDot& src); // copies source object UUID

  ~CRhinoTextDot() override;

  // Returns ON::text_dot
  ON::object_type ObjectType() const override;

  // Create a CRhinoTextDot from a text string and a point
  void Create( const wchar_t* string, ON_3dPoint& point);

  // copies dot and moves user data from argument to m_textdot
  void SetDot( ON_TextDot& dot);

  bool IsVisibleInView( CRhinoView& view) const;

  void Draw( CRhinoDisplayPipeline& dp ) const override;

  /*
    Description:
      Test if the object was hit by the pick event described in the PickContext
      and add it to the picklist if it was
    Parameters:
      [in] CRhinoPickContext& - Information about the current pick event
      [out] CRhinoObjRefArray& - Array for collecting objects that are picked
    Returns:
      1     Object was hit
      2     Object was not hit
  */
  int Pick(const CRhinoPickContext&, CRhinoObjRefArray& objref) const override;

  /*
    Description:
      virtual CRhinoObject::SnapTo override
    Remarks:
      This function should not be used in Rhino plug-ins.
  */
  bool SnapTo(const CRhinoSnapContext& snap_context, CRhinoSnapEvent& snap_event) const override;

  // Returns "text dot"
  const wchar_t* ShortDescription( bool bPlural) const override;

  void DeleteFromDocNotification() override;

  /*
    Description:
      Set or get a 3d definition point for the dot
    Parameters:
      [in] point   the point to set
  */
  void SetPoint( const ON_3dPoint& point);
  ON_3dPoint Point() const;

  /*
  Description:
  Set or Get the text string for the object
  Parameters:
  [in] const wchar_t*  - the text to assign to the object
  Returns:
  const wchar_t*  - the object's text
  */
  void SetTextString(const wchar_t* string);
  const wchar_t* TextString() const;

  /*
  Description:
  Set or Get the secondary text string for the object
  which is typically used for additional info and only 
  displayed when the dot is selected or under the mouse 
  or something
  Parameters:
  [in] const wchar_t*  - the secondary text to assign to the object
  Returns:
  const wchar_t*  - the object's secondary text
  */
  void SetSecondaryTextString(const wchar_t* string);
  const wchar_t* SecondaryTextString() const;

  // Set or Get the font name for the object
  void SetFontFace( const wchar_t* string);
  const wchar_t* FontFace() const;

  // Set or Get the height of the text in pixels
  void SetTextHeight( int height);
  int TextHeight() const;

  /*
    Description:
      Get the size of the bounding rectangle of the text in pixels
    Parameters:
      [out] rect - the bounding rectangle
    Returns:
      TRUE - success
      FALSE - failure
  */
  bool GetScreenRectangleSize( ON_2dPoint& rect) const;

  ON_BoundingBox BoundingBox() const override;
  ON_BoundingBox BoundingBox( const CRhinoViewport* ) const override;

  // not virtual
  ON_BoundingBox BoundingBox( const ON_Viewport& vp) const;

  /*
    Description:
      Get or Set whether the dot is drawn "On Top" of other geometry
    Parameters:
      [in] bTop  bool - It is or isn't on top
    Returns:
      true - on top
      false - not on top
  */
  void SetAlwaysOnTop(bool bTop);
  bool AlwaysOnTop() const;

  /*
    Description:
      Get or Set whether the dot is drawn with a transparent background
    Parameters:
      [in] bTop  bool - It is or isn't on transparent
    Returns:
      true - transparent
      false - not transparent
  */
  void SetTransparent(bool bTransparent);
  bool Transparent() const;

  /*
    Description:
      Get or Set whether the dot is drawn with Bold text
    Parameters:
      [in] bBold  bool - It is or isn't Bold
    Returns:
      true - Bold
      false - not Bold
  */
  void SetBold(bool bBold);
  bool Bold() const;

  /*
    Description:
      Get or Set whether the dot is drawn with Italic text
    Parameters:
      [in] bItalic  bool - It is or isn't Italic
    Returns:
      true - Italic
      false - not Italic
  */
  void SetItalic(bool bItalic);
  bool Italic() const;

  static void TextDotScreenSize(const ON_TextDot& dot, float& cornerRadius, ON_2fPoint& dotSize, ON_2fPoint& textSize);

  void DestroyRuntimeCache(bool bDelete = true) override;

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;

protected:
  // Cached information
  class CRhCacheDataTextDot* TextDotDisplay() const;

  ON_TextDot m_textdot; // m_geometry points to this object
};

/*
  Description:
    Create a text dot at the location of an error. Primary text will be "!".
    Secondary text is a description of the error.
  Parameters:
    Location   [in] - the location of the text dot.
    s          [in] - the error message
    attr       [in] - attributes for the dot.  If NULL, then the default is used
    RhinoDocSN [in] - if non-0, add the dot to the document with this runtime serial number.
  Returns:
    A pointer to the CRhinoTextDot.  
    if RhinoDocSN = 0, then it is the caller's responsibility to manage the memory for
    the returned CRhinoTextDot. Otherwise, if RhinoDocSN is not the serial number for a document, the return will be 0.
*/
RHINO_SDK_FUNCTION
CRhinoTextDot* RhinoCreateErrorMarker(ON_3dPoint Location, 
                                      const wchar_t* msg, 
                                      ON_3dmObjectAttributes* attr, 
                                      unsigned int RhinoDocSN
                                      );
