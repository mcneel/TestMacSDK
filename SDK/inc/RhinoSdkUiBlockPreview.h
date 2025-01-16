/////////////////////////////////////////////////////////////////////////////
// RhinoSdkUiBlockPreview.h

#pragma once

/*
Description:
  The CRhinoUiBlockPreview control displays a "thumbnail" preview
  of a instance definition. To use this control:
    1. Add a picture control to your dialog or window.
    2. Set the control's type property to "owner draw".
    2. Add a CStatic variable to your dialog for this control.
    3. Replace the CStatic class with this one.
    4. Call the appropriate "Set" members.
*/

class CRhinoInstanceDefinition;


class RHINO_SDK_CLASS CRhinoUiBlockPreview : public CStatic
{
  DECLARE_DYNAMIC(CRhinoUiBlockPreview)

public:
  CRhinoUiBlockPreview();
  virtual ~CRhinoUiBlockPreview();

  /*
  Description:
    Returns the current instance definition (that is being previewed).
  */
  const CRhinoInstanceDefinition* InstanceDefinition() const;

  /*
  Description:
    Sets the current instance definition (to be previewed).
  Parameters:
    idef    - [in] The instance definition to preview.
    bRedraw - [in] Forces the control to redraw.
  */
  void SetInstanceDefinition(const CRhinoInstanceDefinition* idef, bool bRedraw = true);

  /*
  Description:
    Returns the current instance definition object (that is being previewed).
  */
  const CRhinoObject* InstanceDefinitionObject() const;

  /*
  Description:
    Sets the current instance definition object.
  Parameters:
    idef_obj - [in] The instance object to preview.
    idef_xf  - [in] The instance object transformation (as if the block were exploded).
    bRedraw  - [in] Forces the control to redraw.
  Remarks:
    Use this if you want to highlight objects inside of an instance definition.
  */
  void SetInstanceDefinitionObject(const CRhinoObject* idef_obj, const ON_Xform& idef_xf, bool bRedraw = true);

  enum view_type
  {
    top_view = 0,
    bottom_view = 1,
    left_view = 2,
    right_view = 3,
    front_view = 4,
    back_view = 5,
    perspective_view = 6,
    two_point_perspective = 7,
    // For internal use only
    view_type_force_32bit = 0xFFFFFFFF
  };
  static CRhinoUiBlockPreview::view_type GetViewType(int n);

  /*
  Description:
    Returns the current view type.
  */
  CRhinoUiBlockPreview::view_type ViewType() const;

  /*
  Description:
    Sets the current view type.
  Parameters:
    vt      - [in] The view type.
    bRedraw - [in] Forces the control to redraw.
  */
  void SetViewType(CRhinoUiBlockPreview::view_type vt, bool bRedraw = true);

  enum draw_decorations
  {
    draw_none = 0,
    draw_document_grid = 1,
    draw_grid = 2,
    draw_axes = 4,
    draw_world_axes = 8,
    draw_all = 0xFFFFFFFF
  };
  static CRhinoUiBlockPreview::draw_decorations GetDrawDecorations(unsigned int n);

  enum isometric_type
  {
    isometric_none = 0,
    north_east = 1,
    north_west = 2,
    south_east = 3,
    south_west = 4,
    // For internal use only
    isometric_type_force_32bit = 0xFFFFFFFF
  };
  static CRhinoUiBlockPreview::isometric_type GetIsometricType(int n);

  /*
  Description:
    Returns the current drawing decorations.
  */
  unsigned int DrawDecorations() const;

  /*
  Description:
    Sets the current drawing decorations.
  Parameters:
    dt      - [in] The drawing decorations made by "or-ing" ( | )
                   values from the draw_decorations enum. If you pass 0,
                   then no decorations will be drawn.
    bRedraw - [in] Forces the control to redraw.
  */
  void SetDrawDecorations(unsigned int dt, bool bRedraw = true);

  /*
  Description:
    Returns the current display mode.
  */
  ON_UUID DisplayMode() const;

  /*
  Description:
    Sets the current display mode.
  Parameters:
    dm      - [in] The display mode UUID.
    bRedraw - [in] Forces the control to redraw.
  */
  void SetDisplayMode(ON_UUID dm, bool bRedraw = true);

  /*
  Description:
    Redraws the control based on the current settings.
  */
  void Redraw();

  /*
  Description:
    Gets a CRhinoInstanceDefintion preview bitmap, see CRhinoUiBlockPreview for argument flag values
  Parameters:
    idef             - [in] Instance definition to draw
    view_type        - [in] Type of view to draw, see CRhinoUiBlockPreview::view_type for options
    display_mode     - [in] Display mode for block preview
    draw_decorations - [in] Bitwise file which specifies view decorations to draw, see CRhinoUiBlockPreview::draw_decorations for options
    size             - [in] Size of output preview image
    applyDpiScaling  - [in] Apply DPI scaling to the size.
    dib_out          - [out] Output bitmap
  Returns:
    Returns true if dib_out is created otherwise false
*/
  static bool GetInstanceDefinitionPreviewBitmap(
    const CRhinoInstanceDefinition& idef,
    view_type view_type,
    const CDisplayPipelineAttributes& da,
    unsigned int draw_decorations,
    CSize size,
    bool applyDpiScaling,
    CRhinoDib& dib_out
  );

  /*
  Description:
    Gets a CRhinoInstanceDefintion preview bitmap, see CRhinoUiBlockPreview for argument flag values
  Parameters:
    idef             - [in] Instance definition to draw
    idef_object_id   - [in] Id of one of the idef's definition objects you want to draw selected.
    view_type        - [in] Type of view to draw, see CRhinoUiBlockPreview::view_type for options
    display_mode     - [in] Display mode for block preview
    draw_decorations - [in] Bitwise file which specifies view decorations to draw, see CRhinoUiBlockPreview::draw_decorations for options
    size             - [in] Size of output preview image
    applyDpiScaling  - [in] Apply DPI scaling to the size.
    dib_out          - [out] Output bitmap
  Returns:
    Returns true if dib_out is created otherwise false
  */
  static bool GetInstanceDefinitionPreviewBitmap(
    const CRhinoInstanceDefinition& idef,
    ON_UUID idef_object_id,
    view_type view_type,
    const CDisplayPipelineAttributes& da,
    unsigned int draw_decorations,
    CSize size,
    bool applyDpiScaling,
    CRhinoDib& dib_out
  );

  static bool GetInstanceDefinitionPreviewBitmap(
    const CRhinoInstanceDefinition& idef,
    ON_UUID idef_object_id,
    view_type view_type,
    isometric_type iso_type,
    const CDisplayPipelineAttributes& da,
    unsigned int draw_decorations,
    CSize size,
    bool applyDpiScaling,
    CRhinoDib& dib_out
  );

  /*
   Description:
    Call this method to get the preview image associated with the specified
    instance definition.
   Returns:
   Returns the preview image associated with the specified instance definition.
   */
  CRhinoDib& PreviewDib();

protected:
  virtual void PreSubclassWindow();
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  DECLARE_MESSAGE_MAP()

private:
  const CRhinoInstanceDefinition* m_idef;
  const CRhinoObject* m_idef_obj;
  ON_Xform m_idef_xf;
  CRhinoUiBlockPreview::view_type m_view_type;
  ON_UUID m_display_mode;
  unsigned int m_draw_type;
  CRhinoDib m_dib;
};
