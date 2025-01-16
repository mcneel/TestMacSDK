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

class RHINO_SDK_CLASS CRhinoAnnotation : public CRhinoObject
{
  ON_OBJECT_DECLARE(CRhinoAnnotation);

protected:
  CRhinoAnnotation() = default;
  CRhinoAnnotation(const CRhinoAnnotation& src);
  CRhinoAnnotation& operator=(const CRhinoAnnotation& src);
  CRhinoAnnotation(const ON_3dmObjectAttributes&);

public:
  ~CRhinoAnnotation() = default;

public:
  ON::object_type ObjectType(void) const override;
  
  /*
  Parameters:
    doc - [in]
      If doc is nullptr, then this->Document() is used. When the CRhinoAnnotation
      is managed by a CRhinoDoc, this is always the correct doc. In rare cases
      when a CRhinoAnnotation object is being created has not been added to a
      CRhinoDoc, the doc parameter must be non-null.
  Returns:
    If the annotation object has overrides, returns the override dimension style
    managed by the ON_Annotation object. Override styles always have nil ids,
    empty names, unset indices. Otherwise, returns the parent dimension style
    which is in the doc style table. The parent style
    always has a non nil id, a non-empty name, and an CRhinoDoc style index.
  */
  const ON_DimStyle& GetEffectiveDimensionStyle(const CRhinoDoc* doc) const;

  /*
  Parameters:
    doc - [in]
      If doc is nullptr, then this->Document() is used. When the CRhinoAnnotation
      is managed by a CRhinoDoc, this is always the correct doc. In rare cases
      when a CRhinoAnnotation object is being created has not been added to a
      CRhinoDoc, the doc parameter must be non-null.
  Returns:
    The parent dimension style which is always in the doc style table. The parent style
    always has a non nil id and a non-empty name. If there is not an override, then
    this is the style used by the object. If there is an override, then this 
    is the style used for settings that are not overridden.
  */
  const ON_DimStyle& ParentDimensionStyle(const CRhinoDoc* doc) const;

  /*
  Returns:
    Raw text that can contain rich text formatting insturctions.
  */
  const ON_wString RichText() const;

  /*
  Returns:
    Text information with rich text formatting insturctions removed.
  */
  const ON_wString PlainText() const;

  const ON_TextContent* TextContent() const;

  const ON_Plane& Plane() const;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;


private:
  const ON_DimStyle& Internal_DimensionStyle(const CRhinoDoc* doc,bool bParentStyle) const;

public:

  /*
  Returns:
    The id of the parent dimension style. If there is not an override, then
    this is the style used by the object. If there is an override, then this 
    is the style used for settings that are not overridden.
  */
  const ON_UUID ParentDimensionStyleId() const;

  ON::AnnotationType AnnotationType() const;

  /*
  Returns:
    A pointer to a const ON_Annotation that can be queried.
    You must always check for a nullptr before dreferencing.
  */
  const ON_Annotation* Annotation() const;

  /*
  Returns:
    A pointer to an  ON_Annotation that can be modified.
    The CRhinoAnnotation object must be non-const and must not
    be in a document.
    You must always check for a nullptr before dreferencing.
  */
  ON_Annotation* AnnotationToModify();
  
  // input: rtf_string - Rtf text string to search for the fonts in the font table
  // output: font_table_entries - An array of the names in the font table
  // returns true if there was a font table, false if there wasn't one
  static bool RtfFontTableEntries(
    const ON_wString rtf_string, 
    ON_ClassArray<ON_wString>& font_table_entries);

  // finds the fonts used by the input annotation
  // default_font - The font from the parent dimstyle - That font may or may not be used by the annotation
  // fonts_used - An array of the fonts actually used to display the text.  This list may or may not include the default_font.
  //              If there are fonts other than the default font listed in fonts_used, those are override fonts.
  // returns the number of override fonts found or -1 for error
  static int FontUse(
    const CRhinoAnnotation* annotation, 
    const ON_DimStyle* effective_dimstyle, 
    ON_Font* default_font, 
    ON_SimpleArray<const ON_Font*>& fonts_used);

  // Compare fonts by RichTextName and Quartet member
  static int CompareFonts(const ON_Font* const* lhs, const ON_Font* const* rhs);

  bool IsActiveInViewport(const CRhinoViewport& viewport) const override;
  bool IsVisibleInViewport(CRhinoDisplayPipeline& dp) const;
  bool IsVisibleInViewport(const CRhinoViewport& vp) const;
  bool IsVisibleInViewport(const CRhinoViewport& vp, const ON_Xform& xform) const;


  // Return a new object like this with properties of matchto
  CRhinoAnnotation* MatchAnnotationObject(const CRhinoAnnotation& matchto) const;

  // Annotative scale according to viewport
  // Returns viewport dependent scale multipler for annotative text
  // When main view is paperspace:
  //  1.0 when current viewport is the page
  //  != 1.0 when current viewport is a detail (except at 1:1 zoom)
  // When main view is modelspace:
  //  scale is only determined by "ModelSpaceTextScale" setting
  static double GetAnnotationScale(
    const CRhinoDoc* doc,
    const ON_DimStyle* dimstyle,
    const CRhinoViewport* pVP);

  static void UpdateTextFields(
    const CRhinoAnnotation* annotation,
    const CRhinoObject* top_object,
    bool forceupdate);


  class TextFieldUpdater
  {
  public:
    TextFieldUpdater() {};
    virtual void UpdateFields(void* args = nullptr) = 0;
  };

  //void UpdateTextFields(
  //  CRhinoDisplayPipeline& dp);

  static bool ParseFields(
    const ON_TextContent* text,
    const CRhinoAnnotation* annotation,
    const CRhinoObject* top_parent);

  static bool ParseFields(
    const ON_TextContent* text,
    const CRhinoAnnotation* annotation,
    const CRhinoObject* top_parent,
    bool forceUpdate);

  bool GetTightBoundingBox(
    ON_BoundingBox& tight_bbox,
    bool bGrowBox,
    const ON_Xform* xform
  ) const override; // CRhinoObject override

  bool GetTightBoundingBoxOld(
    ON_BoundingBox& tight_bbox,
    bool bGrowBox,
    const ON_Xform* xform
  ) const;

  ON_BoundingBox BoundingBox() const override; // CRhinoObject override

  ON_BoundingBox BoundingBox(
    const CRhinoViewport* pViewport) const override; // CRhinoObject override

  virtual ON_BoundingBox BoundingBox(
    const CRhinoViewport* vp,
    const ON_DimStyle* dimstyle) const;

  bool ContainsMeasurableTextFields() const;

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

class RHINO_SDK_CLASS CRhinoText : public CRhinoAnnotation
{
  ON_OBJECT_DECLARE(CRhinoText);

public:
  //Construction, destruction, assignment
  CRhinoText() = default;
  ~CRhinoText();
  CRhinoText(const CRhinoText& src);
  CRhinoText& operator=(const CRhinoText& src);
  CRhinoText(const ON_3dmObjectAttributes&);

  const wchar_t* ShortDescription(bool) const override;

  int Pick(const CRhinoPickContext&, CRhinoObjRefArray& objref) const override;

  // virtual CRhinoObject::SnapTo override
  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const override;

  void EnableGrips(bool bGripsOn) override;

  void SetTextObject(ON_Text* on_text);
  const ON_Text* TextObject(const ON_DimStyle* dimstyle) const;

  void Draw(CRhinoDisplayPipeline& dp) const override;

  ON_wString OnDoubleClick(const CRhinoDoubleClickContext& context) const override;

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

