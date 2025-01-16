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

/////////////////////////////////////////////////////////////////////  
// The CRhinoPrintInfo class is used to hold the information required
// to generate high resolution output of a CRhinoViewport. This is
// used for generating paper prints or image files.
/////////////////////////////////////////////////////////////////////  
class RHINO_SDK_CLASS CRhinoPrintInfo
{
public:
  // color settings for generated output
  enum color_mode { display_color=0,
                    print_color=1,
                    black_white=2 };

  // how the CRhinoViewport is mapped to the output
  enum view_area { area_view = 0,     //best fit rectangle of what is displayed in a CRhinoView
                   area_extents = 1,  //zoom projection to all visible geometry in a viewport
                   area_window = 2 }; //use window rectangle defined by user

  // how the window is expanded off of the window anchor (location of m_anchor_location)
  enum window_target_location { wt_lowerleft  = 0,
                                wt_upperleft  = 1,
                                wt_lowerright = 2,
                                wt_upperright = 3,
                                wt_center     = 4};

  enum output_mode : unsigned int
  {
    om_vector = 0,
    om_raster = 1
  };

  enum class WireThicknessMode : unsigned char
  {
    UsePlotWeight,
    UseLinetypeWidth,
  };
                    
public:
  // Description:
  //   Create an invalid info that needs to be filled
  CRhinoPrintInfo();
  CRhinoPrintInfo(const CRhinoPrintInfo& other);
  CRhinoPrintInfo& operator=(const CRhinoPrintInfo& other);

  bool operator==(const CRhinoPrintInfo&) const;
  bool operator!=(const CRhinoPrintInfo&) const;

  virtual ~CRhinoPrintInfo();

  // Description:
  //   Determines if this has valid rectangles and a CRhinoViewport to work with
  // Return:
  //   true if this is a valid CRhinoPrintInfo
  bool IsValid() const;

  // Description:
  //   Set the CRhinoViewport that this info is based off of. This version only
  //   works with CRhinoViewports that are "attached" to CRhinoViews since it needs to 
  //   look up CRhinoViewports from the available view list.
  // Parameters:
  //   viewport_id: [in] unique ID of an existing CRhinoViewport. This can be
  //   extracted from a viewport through CRhinoViewport::ViewportId()
  void SetViewport(const ON_UUID& viewport_id);

  // Description:
  //   Set the CRhinoViewport that this layout info is based off of.
  //   Make sure that pViewport is valid during the entire scope of this info object
  // Parameters:
  //   pViewport: [in] the viewport to use
  void SetViewport(const CRhinoViewport* pViewport);

  // Description:
  //   Get the CRhinoViewport that this info is set up to print.
  const CRhinoViewport* GetRhinoViewport() const;

  // Description:
  //   Get the Id for the viewport this info is set up to print
  ON_UUID GetRhinoViewportId() const;

  // Description:
  //   Set the rhino doc associated with this print info
  void SetDocument(const CRhinoDoc* doc);

  // Description:
  //  Get the rhino doc associated with this print info
  CRhinoDoc* GetDocument() const;

  // Description:
  //   Set up the rectangles used to define the output image. This function
  //   is used for image file output because there typically is no "unprintable"
  //   portion for the output
  // Parameters:
  //   rect: [in] used for the paper rect, available rect, and draw rect
  //              all three rects are set to the same size. The rectangles are
  //              defined in dot positions on a printed page (no implied unit system)
  void SetLayoutRectangles(const ON_4iRect& rect);

  // Description:
  //   Set up the rectangles used to define the output image. Values are defined
  //   in dot positions on a printed page (no implied unit system)
  // Parameters:
  //   mediaSize: [in] used for the paper rect
  //   cropRectangle: [in] used for available rect, and draw rect
  void SetLayoutRectangles(const ON_2iSize& mediaSize, const ON_4iRect& cropRectangle);

  // Description:
  //   Sets the draw rect equal to the available rect. This sets the drawable area to the
  //   maximum available with the current settings.
  void MaximizeDrawRect();

  // Description:
  //   Sets the draw rect with values to match the original viewport's aspect ratio
  // Returns:
  //   true on success
  bool MatchViewportAspectRatio();

  // Description:
  //   Update the layout rectangles and dpi settings to match a printer dc
  // Parameters:
  //   printer_dc: [in] device context for the printer this layout will
  //              probably be eventually sent to
  //   maintain_margins: [in] set the m_draw_rect to keep the same margins as are currently set
  void UpdateFromPrinterDC(HDC printer_dc, bool maintain_margins);

  // Description:
  //   A CRhinoPrintInfo's display attributes are based on it's CRhinoViewport with
  //   a few possible overrides that are specific to this layout (background, grid, axis)
  // Return:
  //   display attributes specific to this CRhinoPrintInfo
  CDisplayPipelineAttributes DisplayAttributes() const;

  // Description:
  //  Set horizontal and vertical scales on the CDisplayPipelineAttributes that
  //  are returned by DisplayAttributes()
  void SetDisplayAttributesStretch(double horizontalStretch, double verticalStretch);

  // Description:
  //   Get factor used to scale tiles at the target DPI up to the device DPI
  //   Images may be generated at a lower resolution than the device DPI and then
  //   stretch blitted up to the device resolution.
  //   DeviceDPI / StretchFactor = the resolution that images are being generated at
  int StretchFactor() const;

  // Description:
  //   Margins are always measured from the edge of the page (m_paper_rect)
  // Parameters:
  //   units: [in] units to return margins in
  //   left,right,top,bottom: [out] the margins to return
  // Return
  //   true if successful
  //   false if unsuccessful (this could happen if there is no set device_dpi)
  bool GetMargins(ON::LengthUnitSystem units, double& left, double& right, double& top, double& bottom) const;
  bool SetMargins(ON::LengthUnitSystem units, double left, double right, double top, double bottom);

  // Description:
  //   Get minimum/maximum print margins available for this page.
  //   Margins are always measured from the edge of the page
  // Parameters:
  //   minimum: [in] return minimum margins if true (otherwise maximum margins)
  //   units: [in] units to return margins in
  //   left,right,top,bottom: [out] the margins to return
  // Return
  //   true if successful
  //   false if unsuccessful (this could happen if there is no set device_dpi)
  bool GetMarginLimits(bool minimum,
                       ON::LengthUnitSystem units,
                       double& left, double& right, double& top, double& bottom) const;

  // Description:
  //   Sets up an ON_Viewport that would be used for printing. This is the core function used to
  //   get all of the information stored in this class and generate a viewport mapped to an output device.
  // Parameters:
  //   viewport: [out] this will get set up with the projection information for an output device if the
  //             function is successful
  // Returns:
  //   true on success
  bool GetDeviceViewport(ON_Viewport& viewport) const;

  // Description:
  //   Creates a CRhinoViewport mapped to the output device with the appropriate view_area settings.
  //   NOTE: you MUST delete the returned CRhinoViewport when you are done with it.
  // Return:
  //   a CRhinoViewport on success
  //   NULL on failure
  CRhinoViewport* CreateDeviceViewport() const;

  // Description:
  //   returns the width/height of the currently defined paper area
  //   0.0 if the paper area is not defined or invalid
  double PaperAspectRatio() const;

  // Description:
  //   returns the width/height of the currently defined draw area
  //   0.0 if the draw area is not defined or invalid
  double DrawAspectRatio() const;

  // Description:
  //   Get the size of the printed output paper in a specified unit system. Custom
  //   units are not allowed.
  // Parameters:
  //   units: [in] unit system for the output sizes. custom units are not allowed
  //   width, height: [out] size of output paper on success
  // Return:
  //   true on success, false on failure
  bool GetPaperSize(ON::LengthUnitSystem units, double& width, double& height) const;
  bool GetAvailableSize(ON::LengthUnitSystem units, double& width, double& height) const;

  // Description:
  //   Gets the weight used for drawing an object with the current print info's settings
  // Parameters:
  //   pAttributes: [in] rhino object attributes to use for getting weight
  // Returns:
  //   -1 if the object has "No Plot" set weight
  //   otherwise, return a value greater than zero describing the weight used for draw functions
  int GetPrintWeight( const CRhinoObjectAttributes* pAttributes ) const;

  // Description:
  //   Gets the color used for drawing an object with the current print info's settings
  // Parameters:
  //   pAttributes: [in] rhino object attributes to use for getting color
  // Returns:
  //   The Color for the object
  ON_Color GetPrintColor( const CRhinoObjectAttributes* pAttributes  ) const;


  ON_4iRect GetScaledDrawRect() const;
  double ScaledDPI() const;
  double GetLineTypeScale() const;
  double ThinCurveWidthDots() const;
  double HairlineWidthDots() const;
  double DefaultWidthDots() const;
  void SetApplyDisplayModeWidthScales(bool on);
  bool ApplyDisplayModeWidthScales() const;

  void SetViewArea(view_area va);

  // WireThicknessMode is not used. Instead thickness is chosen by object based
  // on if it has a plot_weight or not. If no plot_weight is set (0), then the object's
  // linetype widths are used when printing.
  ON_DEPRECATED WireThicknessMode GetWireThicknessMode() const;
  ON_DEPRECATED void SetWireThicknessMode(WireThicknessMode mode);

  bool LinetypeWidthUnitsArePageLengths() const;
  void SetLinetypeWidthUnitAsPageLengths(bool on);
  
  // TODO: remove the deprecated tags. This should eventually work with m_print_width_default
  ON_DEPRECATED double PixelToMillimeterLinetypeScale() const;
  ON_DEPRECATED void SetPixelToMillimeterLinetypeScale(double millimetersRepresentingOnePixel);
public:
  //////////////////////////// Draw Routines //////////////////////////////////
  // Description:
  //   Draw the layout to a single DIB. This is all done in memory, so make sure that the resulting
  //   DIB is not too large. Tiled drawing may occur if the DIB is larger than the screen resolution,
  //   but this is handled internally by this function.
  //   Note that with display pipelines that are not bound by screen resolution there isn't necessarily
  //   any tiling occurring.
  // Parameters:
  //   full_dib: [out] the dib to draw to
  //   pAttributes: [in] display attributes to use for drawing
  //                     (typically from CRhinoPrintInfo::DisplayAttributes())
  //   pPipeline: [in] Display Pipeline to base drawing off of.
  //   force_GDI_pipeline: [in] if set to true, drawing is performed using a GDI display pipeline.
  // Return:
  //   true if successful
  bool DrawToSingleDib(CRhinoDib& full_dib,
                       const CDisplayPipelineAttributes* pAttributes,
                       const CRhinoDisplayPipeline* pPipeline,
                       bool force_GDI_pipeline) const;

  // Description:
  //   Draw the layout to a single DIB. This is all done in memory, so make sure that the resulting
  //   DIB is not too large. Tiled drawing may occur if the DIB is larger than the screen resolution,
  //   but this is handled internally by this function. This version of the function just "cooks" up
  //   the appropriate display pipelines and attributes and calls the other version of this function.
  // Parameters:
  //   full_dib: [out] the dib to draw to
  //   force_GDI_pipeline: [in] if set to true, drawing is performed using a GDI display pipeline and
  //                       not the pipeline attached to the viewport (which may or may not be GDI).
  // Return:
  //   true if successful
  ON_DEPRECATED_MSG("Use the DrawToSingleDib2")
  bool DrawToSingleDib(CRhinoDib& full_dib,
                       bool force_GDI_pipeline = false,
                       bool bForPrinting = true) const;

  bool DrawToSingleDib2(CRhinoDib& full_dib,
                       bool force_GDI_pipeline = false,
                       bool bForPrinting = true,
                       bool bPreview = false) const;

  // Description:
  //   Draws a sub-rect area of the full printable area rectangle. Tiled drawing is used for generating
  //   large images from hardware accelerated graphics. Graphics cards have a limited area that they
  //   can reasonably render to (typically screen resolution). This is called by the DrawToSingleDib
  //   function and directly from printing code to send smaller bitmaps to the printer.
  // Parameters:
  //   tile_dib:  [out] the dib to draw to
  //   tile_rect: [in] portion of printable area to draw
  //   full_vp:   [in] viewport that we are using to get geometry and projection information from
  //   pPipeline: [in] display pipeline to use for drawing
  // Return:
  //   true if successful
  bool DrawTileToDib( CRhinoDib& tile_dib,
                      const ON_4iRect& tile_rect,
                      const CRhinoViewport& full_vp,
                      CRhinoDisplayPipeline* pPipeline,
                      CDisplayPipelineAttributes* pAttributes) const;

  // Description:
  //   Draws a display pipeline to a device context using GDI
  // Parameters:
  //   draw_dc: [in] The device context to draw to. This could be a printer or a DIB.
  //   pAttributes: [in] display attributes to use for drawing. If NULL, the values from
  //                CRhinoPrintInfo::DisplayAttributes() are used
  //   printer_output: [in] Use styles in print info that draw to DC as if it was being sent to a printer
  // Return:
  //   true if successful
  bool DrawWithGDI(HDC draw_dc,
                    const CDisplayPipelineAttributes* pAttributes = nullptr,
                    bool printer_output = true ) const;

  bool DrawWithVectorEngine(class CRhVectorEngine* drawEngine, const CDisplayPipelineAttributes* pAttributes) const;

  // Description:
  //   Uses GDI to draw m_HeaderText and m_FooterText on top of the final image.
  //   This should be called after calling the above Draw routines
  // Return:
  //   true is successful
  bool DrawHeaderAndFooter(HDC pDC ) const;
  bool DrawHeaderAndFooter(class CRhVectorEngine* drawEngine) const;

  // Description:
  //   Gets the print information that is currently being used inside a CRhinoPrintInfo::Draw.... routine.
  // Returns:
  //   pointer to the active print info that is drawing.
  //   NULL if no CRhinoPrintInfo is currently drawing
  // Remarks:
  //   DO NOT hold onto this pointer outside the scope of your function as it can quickly change
  static const CRhinoPrintInfo* GetActiveDrawPrintInfo();
public:
  //////////////////////////// Tiling Functions //////////////////////////////////
  // Shaded/Rendered viewports are printed as a series of tileds bitmaps using the
  // display pipeline's graphics technology (i.e. OpenGL or DirectX).
  // These technologies are designed for drawing to an area the size of a screen,
  // so tiles are kept to these sizes and a final bitmap is constructed from the tiles.
  // Note that for display pipelines that support rendering to larger-than-screen
  // resolutions tiling isn't occurring.
  
  static void SetMaxTileResolution(int maxWidthOrHeight);
  static int  MaxTileResolution(void);

  // Description:
  //   Get the preferred tile size for tiled image generation. The width and height must
  //   be less than the desktop size in order to allow for accelerated graphics.
  // Parameters:
  //   width, height: [out] preferred tile size in pixels
  void GetPreferredTileSize(int& width, int& height) const;

  // Description:
  //   Get number of tiles required to create a tiled image at the current settings.
  //   Tile size is based on drawing size, target dpi, and preferred tile height/width
  //   For display pipelines not supporting tiling rows and cols will be both 1.
  // Parameters:
  //   rows:    [out] if not NULL, the number of tiles vertical
  //   columns: [out] if not NULL, the number of tiles across
  // Return:
  //   total number of tiles on success. 0 if an error occurs
  int GetTileCount(int* rows = nullptr, int* columns = nullptr) const;

  // Description:
  //   Get a rectangle that represents a subtile of the entire image. Tiles are
  //   in a coordinate system local to the m_draw_rect and scaled to the scale
  //   factor. This means Tile(0,0) will always have a top,left value of 0,0 and
  //   this maps to the top,left corner of the draw rectangle.
  // Parameters:
  //   row, column: [in] position of tile to retrieve
  // Returns:
  //   tile rectangle on success. empty rectangle on failure
  ON_4iRect GetTile(int row, int column) const;

  // Description:
  //   Same as GetTile(row,column), but addresses the tile by index number. This
  //   way you can do the following:
  //       for(int i=0; i<GetTileCount(); i++){
  //         CRect tile_rect = GetTile(i);
  //         ...
  //       }
  ON_4iRect GetTile(int index) const;

  // Description:
  //   Creates a rectangle that represents an image tile on the output device.
  //   This tile is at full resolution and positioned relative to the output
  //   device available print rectangle (m_available_print_rect)
  // Parameters:
  //   tile_rect: [in] tile relative to the draw rect and scaled down by the
  //              scale factor (typically from the GetTile functions.)
  ON_4iRect GetDeviceTile(const ON_4iRect& tile_rect) const;

public:
  ////// display attribute overrides //////
  //   When sending the image to a file or printer, it is sometimes best to override the display
  //   attributes that are associated with the CRhinoView. This is typical with printing where
  //   we may not want to fill in the background with a color or draw the display grid.
  bool m_bDrawBackground = false;
  bool m_bDrawGrid = false;
  bool m_bDrawAxis = false;
  bool m_bDrawLineWeights = true;
  bool m_bDrawBackgroundBitmap = false;
  bool m_bDrawWallpaper = false;
  bool m_bDrawLockedObjects = true;
  bool m_bDrawMargins = false;
public:
  double m_point_scale = 0.5; // size of point objects in millimeters
                              // if scale <= 0 the size is minimized so points are always drawn as small as possible
  double m_arrowhead_size_mm = 2.0; // arrowhead size in millimeters
  double m_textdot_size = 10.0;     // Font point size use for printing text dots

  color_mode m_ColorMode;
  output_mode m_OutputMode = output_mode::om_vector;

  RhinoLineCapStyle  m_endcap_style = RhinoLineCapStyle::None;  // Curve pen styles for thick lines
  RhinoLineJoinStyle m_join_style = RhinoLineJoinStyle::None;

  // default is false. Linetype scales are normally generated right before
  // printing in order to get linetypes to print to the same lengths as
  // defined. If true, the m_saved_linetype_scale is used. This is useful
  // if you want to print using the current display linetype scale or if
  // you want to print using some arbitrary linetype scale
  bool m_bUseSavedLineTypeScale;

  // scaling factor to apply to object print widths (typically 1.0). This is
  // helpful when printing something at 1/2 scale and having all of the curves
  // print 1/2 as thick
  double m_print_width_scale;

  // Line thickness used to print objects with no defined thickness (in mm)
  double m_print_width_default;

  //how to map the viewport to the output device
  view_area m_view_area; 

  //only display/print selected objects (default is false)
  bool m_bOnlySelectedObjects;

  bool DrawClippingPlanes() const;
  void SetDrawClippingPlanes(bool on);
  bool DrawLights() const;
  void SetDrawLights(bool on);
private:
  bool m_bDrawClippingPlanes;
  bool m_bDrawLights;
  bool m__reserved;
public:
  double m_saved_linetype_scale;

  // If we want to put text on top of the print assign it to the header and footer text
  // #NOTES# will be replaced with the document notes
  // #FILENAME# will be replaced with the document filename
  ON_wString m_HeaderText;
  ON_wString m_FooterText;

  static const wchar_t* FormatString_Notes();
  static const wchar_t* FormatString_Filename();

  ////// layout rectangles //////

  ON_4iRect m_paper_rect; // physical size of paper in dots (no margins - the actual paper)
  ON_4iRect m_available_print_rect; //maximum portion of paper that can be drawn to
  ON_4iRect m_draw_rect;  // location on m_paper_rect that image is drawn to in dots
                      // this rectangle holds information about where to draw and
                      // what the margins are.
                      // m_draw_rect must always be inside of m_available_print_rect

  ////// Output resolution settings //////

  // The actual dot per inch resolution of the final output. This is the resolution
  // of the selected printer when sending an image to a printer. For raster image files,
  // this is used to determine image space sizes (things like line widths)
  double m_device_dpi;

  // Resolution cap when generating rester output. Images are stretched to
  // m_device_dpi when m_device_dpi > m_maximum_dpi
  // Many photo printers have default resolutions of around 1200 dpi
  // which would demand enormous image size requirements if printing at full resolution
  // default DPI resolutions are set at 600 DPI. 
  // Personal Opinion (S. Baer):
  //   300DPI is a great setting for almost all prints
  //   600DPI looks only slightly better (but not worth 4x memory use)
  //   I can't see the difference between 600DPI and 1200 DPI
  double m_maximum_dpi = 600;

public:
  window_target_location GetAnchorLocation() const;
  void SetAnchorLocation(window_target_location anchor);

  void SetOffset( ON::LengthUnitSystem offset_units, bool from_margin, double x, double y);
  void GetOffset( ON::LengthUnitSystem offset_units, bool& from_margin, double& x, double& y) const;
  void GetCenterEquivalientOffset( ON::LengthUnitSystem offset_units, bool& from_margin, double& x, double& y) const;
private:
  const CRhinoViewport* m_pViewport = nullptr;  // the viewport that is used to generate drawings
  ON_UUID m_viewport_uuid;            // If m_pViewport==NULL, this can be used to look
                                      // up a viewport from the view list

  // window points are defined as u,v locations on plane parallel to the
  // camera frame with an origin at 0,0,0 when the viewport projection is parallel
  // When the viewport projection is perspective, the points are percent positions
  // on the screen port
  ON_2dPoint m_window_point1, m_window_point2;

  window_target_location m_anchor_location;

  bool   m_offset_from_margin;  // if true, offset is relative to margin edge
                                // if false, offset is relative to paper edge
  double m_offset_x; // horizontal offset in millimeters
  double m_offset_y; // vertical offset in millimeters
public:
  ////// scaling functions /////////
  double GetModelScale(ON::LengthUnitSystem paper_units, ON::LengthUnitSystem model_units) const;

  void SetModelScaleToValue(double scale);
  void SetModelScaleToFit(bool prompt_if_change);

  void SetModelScaleType(int type);
  int  GetModelScaleType() const;

  bool IsScaledToFit() const;

  static void GetPlotScaleNames(ON_ClassArray<ON_wString>& names, bool include_architectural);

private:
  static ON_ClassArray<ON_wString> m_plot_scale_names;
  static ON_SimpleArray<double> m_plot_scale_values;
  static int m_architectural_index; //start index of architectural scales in the plot_scale arrays
  // Description:
  //   Plot scale arrays are lazily created. ConstructPlotScaleArrays is always internally called
  //   before directly accessing the arrays.
  static void ConstructPlotScaleArrays();

  int m_scale_index;
  double m_custom_model_scale;  //if this is -1.0, use scale from list of hardcoded scales


public:
  // Functions used by specific classes
  // NOTE: For Internal Use

  // Description:
  //   Get the rectangles in screen space coordinates. This is used by the print dialog during
  //   window selection of the print area to show preview rectangles on the view.
  // Parameters:
  //   paper: [out] the full page rectangle
  //   available: [out] the portion of the paper that can drawn/printed to. Always inside paper rectangle
  //   draw: [out] defined area the will be drawn/printed to. Always inside available rectangle
  // Return:
  //   true if the layout is valid and was able to successfully return rectangles
  bool GetScreenRects(ON_4iRect& paper, ON_4iRect& available, ON_4iRect& draw) const;

  // Description:
  //   Used by DrawTileToDib to create tile viewports from a large single viewport
  // Parameters:
  //   full_viewport: [in] large single viewport that tiles are taken from
  //   tile_viewport: [out] sub portion of the full viewport
  //   tile_rect: [in] 
  // Returns:
  //   true on success
  bool GetTileViewport(const ON_Viewport& full_viewport, ON_Viewport& tile_viewport, const ON_4iRect& tile_rect) const;

  // Description:
  //   Create a layout info that is based on this layout info but sized to fit inside
  //   a preview area. Used by the print preview window to generate a preview image
  // Parameters:
  //   preview_area: [in] size to fit the layout info inside of
  //   preview: [out] sized layout for preview purposes
  // Return:
  //   true if successful
  bool GetPreviewLayout(const ON_2iSize& preview_area, CRhinoPrintInfo& preview) const;

  // Description:
  //   Defines the window print area rectangle.
  // Parameters:
  //   corner1: [in] world coordinate corner first pick for defining the window rectangle
  //   corner2: [in] world coordinate corner second pick for defining the window rectangle
  void SetWindowRect( ON_3dPoint corner1, ON_3dPoint corner2 );
  void SetWindowRectFromScreen( ON_2dPoint screen1, ON_2dPoint screen2 );

  void MoveWindowRect( ON_2dVector screen_delta );

  void InvalidateWindowRect();

  // Description:
  //   Return Window Area Size
  double GetWindowAreaWidth() const;

  void LoadSettings(CRhinoSettings& settings);
  void SaveSettings(CRhinoSettings& settings) const;

  static void SetCurrentLayerOnExport(const CRhinoLayer* layer);
private:
  class CRhPrintInfoPrivate* m_private = nullptr;
};

