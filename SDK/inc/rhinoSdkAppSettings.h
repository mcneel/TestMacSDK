//
// Copyright (c) 1993-2024 Robert McNeel & Associates. All rights reserved.
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

///////////////////////////////////////////////////////////////////////////////
//
#pragma region class CRhinoCurvatureGraphSettings
//
// Description:
//  When curves, surfaces, and polysurfaces have their curvature graph turned
//  on, these settings control the appearance of the curvature graph.
class RHINO_SDK_CLASS CRhinoCurvatureGraphSettings
{
public:
  CRhinoCurvatureGraphSettings();
  CRhinoCurvatureGraphSettings(const CRhinoCurvatureGraphSettings&);
  ~CRhinoCurvatureGraphSettings();
  CRhinoCurvatureGraphSettings& operator=(const CRhinoCurvatureGraphSettings&);


  bool operator==(const CRhinoCurvatureGraphSettings& other) const;
  bool operator!=(const CRhinoCurvatureGraphSettings& other) const;

  static const CRhinoCurvatureGraphSettings Default;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate(); // clamp settings to valid values
  void Defaults();
  const wchar_t* ProfileKey() const;

  ON_Color m_curve_hair_color;
  ON_Color m_srfu_hair_color;
  ON_Color m_srfv_hair_color;

  bool m_bSrfuHair = true; // true if surface u isocurve hair is on
  bool m_bSrfvHair = true; // true if surface v isocurve hair is on
  int  m_hair_scale = 100; // 100 = true length, > 100 magnified, < 100 shortened
  int  m_hair_density = 1; // >= 0 larger numbers = more hairs
  int  m_sample_density = 2; // between 1 and 10.  Higher numbers draw smoother outer curves.

  int WireDensity() const;
  void SetWireDensity(int wireDensity);
private:
  class CRhCurvatureGraphSettingsPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoCurvatureGraphSettings

#pragma region class CRhinoZebraAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoZebraAnalysisSettings
{
public:
  CRhinoZebraAnalysisSettings();
  ~CRhinoZebraAnalysisSettings();

  CRhinoZebraAnalysisSettings(const CRhinoZebraAnalysisSettings& other);
  CRhinoZebraAnalysisSettings& operator=(const CRhinoZebraAnalysisSettings& other);
  
  bool operator==(const CRhinoZebraAnalysisSettings& other) const;
  bool operator!=(const CRhinoZebraAnalysisSettings& other) const;

  static const CRhinoZebraAnalysisSettings Default;
  void Defaults();

  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from settings
  void SaveProfile( CRhinoProfileContext& ) const; // save to settings
  void Validate(); // clamp settings to valid values

  BYTE m_stripe_direction; // 0 = horizontal, 1 = vertical
  BYTE m_show_isoparms;
  BYTE reserved[2];

  int m_stripe_thickness; // 0 = thinnest ... 6 = thickest
  ON_Color m_stripe_color; 

  bool UseIsoDrawEffectSettings() const;
  void SetUseIsoDrawEffectSettings(bool enable);
  const CRhinoIsoDrawEffect& IsoDrawEffectSettings() const;
  void SetIsoDrawEffectSettings(const CRhinoIsoDrawEffect& settings);
private:
  mutable class CRhZebraAnalysisSettingsPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoZebraAnalysisSettings

#pragma region class CRhinoEmapAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoEmapAnalysisSettings
{
public:
  CRhinoEmapAnalysisSettings();
  
  bool operator==(const CRhinoEmapAnalysisSettings& other) const;
  bool operator!=(const CRhinoEmapAnalysisSettings& other) const;

  static const CRhinoEmapAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry

  bool m_bBlendWithObjectColor = false;
  bool m_show_isoparms = false;

  //  Return file name to be mapped to objects
  const wchar_t* FileName() const;
  /*
  Description:
    Set file name.
  Arguments:
    name [in] - Name to make current.  The previous name will get moved
                down the MRU file name list.
  Returns:
    If name added successfully true is returned otherwise false.
  */
  bool SetFileName( const wchar_t* name);
  /*
  Description:
    Get number of items in most recently used(MRU) file name list.
  */
  int MruFileNameCount() const;
  /*
  Description:
    Get MRU file name at specified index.
  Arguments:
    index [in] - Index of name to return, 0 for oldest, MruFleNameCount()-1
                 for current.
  Returns:
    If index is valid then a pointer to the name is returned otherwise NULL.
  */
  const wchar_t* MruFileName( int index) const;
  /*
  Description:
    Remove MRU file name at specified index.
  Arguments:
    index [in] - Index of name to remove from list.
  */
  void MruRemoveFileName( int index);

  enum { max_mru_count = 20};

protected:
  void FormatEntryName( int i, ON_wString& s) const;
  ON_ClassArray<ON_wString> m_mru_emap_files;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoEmapAnalysisSettings

#pragma region class CRhinoCurvatureAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoCurvatureAnalysisSettings
{
public:
  CRhinoCurvatureAnalysisSettings();
  
  bool operator==(const CRhinoCurvatureAnalysisSettings& other) const;
  bool operator!=(const CRhinoCurvatureAnalysisSettings& other) const;

  static const CRhinoCurvatureAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate(); // clamp settings to valid values

  /// <returns>
  /// The 32 bit CRC of these curvature analysis settings.
  /// </returns>
  ON__UINT32 ColorMappingCRC() const;

  /// <summary>
  /// The curvature vertex color mapping tag is used to mark vertex colors
  /// when they are set by curvature color analysis.
  /// </summary>
  /// <returns>
  /// The vertex color mapping tag for these curvature analysis settings.
  /// </returns>
  const ON_MappingTag MappingTag() const;


  /// <returns>Style of curvature being analyzed (Gaussian, mean, ...).</returns>
  ON::curvature_style KappaStyle() const;

  /// <returns>Range of curvatures to map to the hues from red to purple.</returns>
  const ON_Interval KappaRange() const;

  const ON_SurfaceCurvatureColorMapping KappaColors() const;

  const ON_Interval HueRangeRadians() const;

  enum 
  {
    kappa_gaussian,
    kappa_mean,
    kappa_minradius,
    kappa_maxradius
  } m_style;

  ON_Interval m_gauss_range;
  ON_Interval m_mean_range;
  ON_Interval m_minradius_range;
  ON_Interval m_maxradius_range;
  bool m_show_isoparms;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoCurvatureAnalysisSettings

#pragma region class CRhinoDraftAngleAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoDraftAngleAnalysisSettings
{
public:
  CRhinoDraftAngleAnalysisSettings();
  
  bool operator==(const CRhinoDraftAngleAnalysisSettings& other) const;
  bool operator!=(const CRhinoDraftAngleAnalysisSettings& other) const;

  static const CRhinoDraftAngleAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate(); // clamp settings to valid values

  const ON_SurfaceDraftAngleColorMapping DraftAngleColors() const;

  /// <returns>
  /// The 32 bit CRC of these draft angle analysis settings.
  /// </returns>
  ON__UINT32 ColorMappingCRC() const;

  /// <summary>
  /// The draft angle vertex color mapping tag is used to mark vertex colors
  /// when they are set by draft angle color analysis.
  /// </summary>
  /// <returns>
  /// The vertex color mapping tag for these draft angle analysis settings.
  /// </returns>
  const ON_MappingTag MappingTag() const;

  const ON_Interval AngleRangeDegrees() const;

  const ON_Interval AngleRangeRadians() const;

  void SetAngleRangeRadians(double a0_radians, double a1_radians);
  void SetAngleRangeRadians(ON_Interval angle_range_radians);

  void SetAngleRangeDegrees(double a0_degrees, double a1_degrees);
  void SetAngleRangeDegrees(ON_Interval angle_range_degrees);
  const ON_Interval HueRangeRadians() const;

  // angles in degrees
  ON_Interval m_angle_range;

  ON_3dVector m_up;

  bool m_show_isoparms;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoDraftAngleAnalysisSettings

#pragma region class CRhinoThicknessAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoThicknessAnalysisSettings
{
public:
  CRhinoThicknessAnalysisSettings();
  
  bool operator==(const CRhinoThicknessAnalysisSettings& other) const;
  bool operator!=(const CRhinoThicknessAnalysisSettings& other) const;

  static const CRhinoThicknessAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate(); // clamp settings to valid values

  ON__UINT32 ColorMappingCRC() const;

  ON_Interval m_distance_range;
  bool m_bShowIsoparams;
public:
  // maximum number of seconds a user is willing to wait to
  // calculate the analysis results on a single object.
  // An unsigned short allows for a maximum limit of 18.6 hours.
  // If m_max_update_time_seconds is zero, then the calculation
  // is allowed to run forever.
  unsigned short m_max_update_time_seconds;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoThicknessAnalysisSettings

#pragma region class CRhinoEdgeAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoEdgeAnalysisSettings
{
public:
  CRhinoEdgeAnalysisSettings();
  
  bool operator==(const CRhinoEdgeAnalysisSettings& other) const;
  bool operator!=(const CRhinoEdgeAnalysisSettings& other) const;

  static const CRhinoEdgeAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate(); // clamp settings to valid values

  // color used to enhance display edges in commands like ShowEdges
  // and ShowNakedEdges.
  ON_Color m_show_edge_color;
  int m_show_edges; // 0 = all, 1 = naked, 2 = non-manifold

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoEdgeAnalysisSettings


#pragma region class CRhinoEndAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoEndAnalysisSettings
{
public:
  CRhinoEndAnalysisSettings();
  
  bool operator==(const CRhinoEndAnalysisSettings& other) const;
  bool operator!=(const CRhinoEndAnalysisSettings& other) const;
  
  static const CRhinoEndAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate();
  
  ON_Color m_color; // color used to enhance display
  bool m_bOpenStart;
  bool m_bOpenEnd;
  bool m_bSeams;
  bool m_bJoints;

private:
  ON__UINT_PTR m_private = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
#pragma region class CRhinoEdgeContinuitySettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoEdgeContinuitySettings
{
public:
#pragma region RH_C_SHARED_ENUM [CRhinoEdgeContinuitySettings::EdgeContinuityMode] [Rhino.Display.VisualAnalysisMode.EdgeContinuityMode] [nested:int]
  /// <summary>
  /// Continuity mode to report
  /// </summary>

  enum class EdgeContinuityMode : int
  {
    /// <summary> G0 Continuity </summary>
    Distance = 0,
    /// <summary> G1 Continuity </summary>
    Tangency = 1,
    /// <summary> G2 Continuity </summary>
    Curvature = 2
  };
#pragma endregion

  static EdgeContinuityMode EdgeContinuityModeFromUnsigned(
    unsigned int edge_mode_as_unsigned);

  CRhinoEdgeContinuitySettings();

  bool operator==(const CRhinoEdgeContinuitySettings& other) const;
  bool operator!=(const CRhinoEdgeContinuitySettings& other) const;

  static const CRhinoEdgeContinuitySettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile(CRhinoProfileContext&) const; // save in registry
  void Validate();

  void SetDensity(int density);
  int Density() const;
  void SetDistanceScale(double scale);
  double DistanceScale() const;
  void SetTangencyScale(double scale);
  double TangencyScale() const;
  void SetCurvatureScale(double scale);
  double CurvatureScale() const;
  void SetCurvatureTolerancePercent(double percent);
  double CurvatureTolerancePercent() const;
  void SetTangentToleranceRadians(double radians);
  double TangentToleranceRadians() const;
  void SetTangentToleranceDegrees(double degrees);
  double TangentToleranceDegrees() const;
  void SetDistanceTolerance(double distance);
  double DistanceTolerance() const;
  void SetGoodHairColor(ON_Color color);
  ON_Color GoodHairColor() const;
  void SetBadHairColor(ON_Color color);
  ON_Color BadHairColor() const;
  void SetMaxColor(ON_Color color);
  ON_Color MaxColor() const;
  void SetTextColor(ON_Color color);
  ON_Color TextColor() const;
  void SetShowDistance(bool show);
  bool ShowDistance() const;
  void SetShowCurvature(bool show);
  bool ShowCurvature() const;
  void SetShowTangency(bool show);
  bool ShowTangency() const;
  void SetContinuity(EdgeContinuityMode continuity);
  EdgeContinuityMode Continuity() const;

private:
  int      m_density                     = 50;
  ON_Color m_good_color                  = ON_Color::SaturatedGreen;
  ON_Color m_bad_color                   = ON_Color::SaturatedRed;
  ON_Color m_max_color                   = ON_Color::SaturatedBlue;
  ON_Color m_text_color                  = ON_Color::Black;
  double   m_distance_scale              = 10.0;
  double   m_tangency_scale              = 10.0;
  double   m_curvature_scale             = 100.0;
  double   m_curvature_tolerance_percent = 5.0;
  double   m_tangent_tolerance_radians   = ON_DEGREES_TO_RADIANS;
  double   m_distance_tolerance          = 0.01;
  bool     m_show_distance_number        = true;
  bool     m_show_curvature_number       = true;
  bool     m_show_tangency_number        = true;
  EdgeContinuityMode m_continuity        = EdgeContinuityMode::Tangency;
  double   m_reserved_double_0           = 0.0;
  double   m_reserved_double_1           = 0.0;
  double   m_reserved_double_2           = 0.0;

  ON__UINT_PTR m_private = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoEndAnalysisSettings


#pragma region class CRhinoDirectionAnalysisSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoDirectionAnalysisSettings
{
public:
  CRhinoDirectionAnalysisSettings();
  
  bool operator==(const CRhinoDirectionAnalysisSettings& other) const;
  bool operator!=(const CRhinoDirectionAnalysisSettings& other) const;
  
  static const CRhinoDirectionAnalysisSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate();
  
  bool m_bDirection;
  bool m_bU;
  bool m_bV;
  ON_Color m_color;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoDirectionAnalysisSettings

#pragma region class CRhinoDisplaySettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoDisplaySettings
{
public:
  CRhinoDisplaySettings();
  CRhinoDisplaySettings(const CRhinoDisplaySettings&);
  ~CRhinoDisplaySettings();
  CRhinoDisplaySettings& operator=(const CRhinoDisplaySettings&);

  bool m_bRenderedViewsShowLights;
  
#if defined(ON_RUNTIME_APPLE)
  bool m_bMetalUseMaxSampleCount;
#endif

  bool m_get_rendered_view_settings_from_doc;
  bool m_display_curves_and_isocurves_in_rendered_views;
  bool m_display_text_and_dimensions_in_rendered_views;
  bool m_use_hidden_lights_in_rendered_views;
  ON_Color m_ambient_light_color_in_rendered_views;
  ON_Color m_background_color_in_rendered_views;
  int m_advanced_texture_preview_min_resolution;
  int m_advanced_texture_preview_max_resolution;

  bool operator==(const CRhinoDisplaySettings& other) const;
  bool operator!=(const CRhinoDisplaySettings& other) const;

  static const CRhinoDisplaySettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Validate();

  double SolidHatchMeshingTolerance() const;

private:
  class CRhDisplaySettingsPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoDisplaySettings

#pragma region class CRhinoOpenGLSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoOpenGLSettings
{
public:
  CRhinoOpenGLSettings();
  CRhinoOpenGLSettings(const CRhinoOpenGLSettings&);
  ~CRhinoOpenGLSettings();
  CRhinoOpenGLSettings& operator=(const CRhinoOpenGLSettings&);

  bool operator==(const CRhinoOpenGLSettings& other) const;
  bool operator!=(const CRhinoOpenGLSettings& other) const;

  static const CRhinoOpenGLSettings Default;
  void Defaults();
  const wchar_t* ProfileKey() const;
  void LoadProfile(CRhinoProfileContext& pc);
  void SaveProfile( CRhinoProfileContext& ) const;
  void Validate(); // clamp settings to valid values

public:
  bool m_bUseHardwareDriverModes    = true; // Try to use non-generic modes...

  // 18 July 2020 S. Baer (RH-58768)
  // Mac drivers still have issues with tessellation shaders. Turning them off
  // by default on Macand users can opt in to this feature in case it works
  // well for a specific set up
#if defined (ON_RUNTIME_APPLE)
  bool m_bUseTessellationShaders    = false;
#else
  bool m_bUseTessellationShaders    = true; // use tessellation shaders when available
#endif
  bool m_bRegenBufferOnHardwareSwap = true; // Some cards can't reproduce the backbuffer in certain situations.
  bool m_bUseTextureCompression     = false;

  int  m_max_gl_level = 45;     // used to cap the OpenGL operating level
                                // (40 would mean only use OpenGL 4.0 functionality and below)
  int  m_nAntialiasMode = 2;    // 0 = none, 1 = Low, 2 = Medium, 3 = High
  int  m_nMipMapMode = 2;       // 0 = none, 1 = Nearest, 2 = Linear
  int  m_nAnisotropicMode = 3;  // 0 = none, 1 = Low, 2 = Medium, 3 = High

  bool m_bLineSmoothing = true; // alpha blend wires out from the edge of the curve. No line smoothing
                                // will have higher performance, but lower quality
  
#if defined (ON_RUNTIME_APPLE)
  bool m_use_core_profile = true;
#else
  bool m_use_core_profile = false;
#endif
  
#if defined (ON_RUNTIME_APPLE)
  int   m_nLineSmoothingMode;       // 0 = none, 1 = Add thickness adjustment, 2 = Multiply thickness adjustment
  double m_dLineSmoothingThickener;  // typically -9 to + 20
  int   m_nBackBufferMode;   // 0 = none, 1 = use DIB, 2 = use texture
#endif
  
  float WireThicknessScale() const;
  void SetWireThicknessScale(float scale);

  unsigned int SkylightShadowResolutionScale() const;
  void SetSkylightShadowResolutionScale(int scale);
  unsigned int SkylightShadowResolutionScaleDynamic() const;
  void SetSkylightShadowResolutionScaleDynamic(int scale);
  bool AllowGpuProcedurals() const;
  void SetAllowGpuProcedurals(bool allow_gpu_procedurals);

  void SetQuadBuffering(bool);
  bool QuadBuffering() const;
private:
  class CRhOpenGLSettingsPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoOpenGLSettings

#pragma region class CRhinoAppAppearanceSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppAppearanceSettings
{
public:
  CRhinoAppAppearanceSettings();
  CRhinoAppAppearanceSettings( const CRhinoAppAppearanceSettings& );
  ~CRhinoAppAppearanceSettings();
  CRhinoAppAppearanceSettings& operator=( const CRhinoAppAppearanceSettings& );

  bool operator==( const CRhinoAppAppearanceSettings& src) const;
  bool operator!=( const CRhinoAppAppearanceSettings& src) const;

  void LoadProfile(CRhinoProfileContext& pc, unsigned int nDefaultLanguage = 0);
  void SaveProfile( CRhinoProfileContext& ) const;
  void Defaults();
  static const CRhinoAppAppearanceSettings Default;
  void SynchWithGUI();
  void GetDefaultFontFaceName(ON_wString &font) const;
#if defined(ON_RUNTIME_WIN)
  const LOGFONT& GetUiFont() const;
#endif

  enum command_prompt_position
  {
    command_prompt_top,
    command_prompt_bottom,
    command_prompt_floating,
    command_prompt_hidden
  };

public:
  ON_Color m_default_layer_color; // default layer color

  /*
  Description:
    Get the color used to draw selected objects (default is yellow)
  Parameters:
    bIsReferenceObject - [in] if true, the color used for selected
                        reference model objects is returned.
  */
  ON_Color SelectedObjectColor( bool isReferenceObject = false ) const;

  /*
  Description:
    Makes sure the font is appropriate for the locale
    that is selected.
  */
  void ValidateFontSetting();

  void SetSelectedObjectColor( ON_Color );

  /*
  Description:
    Get the color used to draw selected objects (default is yellow)
  Parameters:
    bIsReferenceObject - [in] if true, the color used for selected
                        reference model objects is returned.
  */
  ON_Color LockedObjectColor( bool isReferenceObject = false ) const;

  void SetLockedObjectColor( ON_Color );

  ON_Color SelectionWindowStrokeColor() const;
  ON_Color SelectionWindowFillColor() const;
  ON_Color SelectionWindowCrossingStrokeColor() const;
  ON_Color SelectionWindowCrossingFillColor() const;

  void SetSelectionWindowStrokeColor(const ON_Color& c);
  void SetSelectionWindowFillColor(const ON_Color& c);
  void SetSelectionWindowCrossingStrokeColor(const ON_Color& c);
  void SetSelectionWindowCrossingFillColor(const ON_Color& c);

  // world coordinate sprite axes attributes
  ON_Color m_world_coord_icon_xaxis_color;
  ON_Color m_world_coord_icon_yaxis_color;
  ON_Color m_world_coord_icon_zaxis_color;
  int m_world_coord_icon_axis_size; // length of world coordinate sprite axis in pixels
  int m_world_coord_icon_label_size; // "radius" of letter in pixels
  bool m_world_coord_icon_bMoveLabels; // true to move axis letters as sprite rotates

  // size of arrow icon used to indicate direction
  int m_direction_arrow_icon_shaft_size; // length of direction arrow shaft icon in pixels
  int m_direction_arrow_icon_head_size;  // length of direction arrowhead icon in pixels

  int DirectionArrowIconThickness() const;
  void SetDirectionArrowIconThickness(int thickness);

  ON_Color DirectionArrowIconColorU() const;
  ON_Color DirectionArrowIconColorV() const;
  ON_Color DirectionArrowIconColorW() const;
  void SetDirectionArrowIconColorU(const ON_Color& color);
  void SetDirectionArrowIconColorV(const ON_Color& color);
  void SetDirectionArrowIconColorW(const ON_Color& color);
public:
  bool ViewportTabsVisibleAtStart() const;
  void SetViewportTabsVisibleAtStart(bool on);

  // Display PDF printers in the print dialog that are not the PDF driver built into Rhino
  bool ShowExternalPdfPrinters() const;
  void SetShowExternalPdfPrinters(bool show);

  // Display drop shadow on layout viewports
  bool ShowLayoutDropShadow() const;
  void SetShowLayoutDropShadow(bool show);
private:
  // Used to determine main frame viewport tab settings at startup
  unsigned char m_viewport_tabs_flags;

public:
  // object creation and modification colors
  ON_Color m_tracking_color;
  ON_Color m_feedback_color;
  ON_Color m_default_color;  // default object color
  // view colors
  ON_Color m_viewport_background_color;
  // frame window color attributes
  ON_Color m_frame_background_color;
  // command prompt window colors
  ON_Color m_cmdprompt_text_color;
  ON_Color m_cmdprompt_active_hypertext_color;
  ON_Color m_cmdprompt_background_color;
  ON_wString m_cmdprompt_font_name;

  //ON_DEPRECATED_MSG("no longer used")
  int m_cmdprompt_font_height;
  
  ON_DEPRECATED_MSG("no longer used")
  int m_cmdprompt_height_in_lines;

  ON_DEPRECATED_MSG("no longer used")
  command_prompt_position m_cmdprompt_position;

  bool m_show_statusbar;
  int m_statusbar_info_pane_mode;

  bool m_show_osnap_dialog; // display persistent Osnaps dialog

  bool m_echo_prompts_to_history;
  bool m_echo_commands_to_history;

  bool m_show_full_path_in_title_bar;

  // Cross hair color and state
  bool m_show_crosshairs;     // lines that extend across the view
  ON_Color m_crosshair_color; // cross hair line color
  
  // a screen length value of zero is interpreted as using the entire screen
  double CrosshairScreenLength() const;
  void SetCrosshairScreenLength(double length);

// TODO: should these be included?  Used in V 2.0
  bool m_show_viewport_title;       // viewport name in top left cornner
  bool m_main_window_title;         // main frame window title
  bool m_show_menu;
  unsigned int m_language_identifier;

  // CRhinoPageView paper background. A rectangle is drawn into the background
  // of page views to represent the printed area. The alpha portion of the color
  // is used to draw the paper blended into the background
  ON_Color m_pageview_paper_color;

  // This color is used by the layer manager dialog 
  // as the background color for the current layer.
  ON_Color m_current_layer_bkcolor;

  bool m_show_side_bar;

  // Color of objects that are eligible to be edited
  ON_Color m_edit_candidate_color;
private:
  ON_Color m_selected_object_color;
  ON_Color m_locked_object_color;

private:
  class CRhAppearanceSettingsPrivate* m_private = nullptr;
  friend class CRhDefaultColors;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppAppearanceSettings

#pragma region class CRhinoAppFileSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppFileSettings
{
public:
  CRhinoAppFileSettings();
  ~CRhinoAppFileSettings();
  CRhinoAppFileSettings(const CRhinoAppFileSettings&);
  CRhinoAppFileSettings& operator=(const CRhinoAppFileSettings&);

  bool operator==(const CRhinoAppFileSettings& src) const;
  bool operator!=(const CRhinoAppFileSettings& src) const;

  void LoadProfile(CRhinoProfileContext& pc);
  void SaveProfile( CRhinoProfileContext& ) const;
  void Defaults();

  ON_wString m_template_files; // default template file folder (RhinoInstallFolder\Templates)
  ON_wString m_default_template_file; // template file loaded when Rhino starts and the default file name for OpenTemplateFile dialog
  
  // AutoSave options
  ON_wString                m_autosave_file;       // name of the default autosave file (<RhinoInstallFolder>\Autosave.3dm)
  bool                      m_autosave = true;            // enable autosave feature
  bool                      m_autosave_meshes = false;      // save render and display meshes
  int                       m_autosave_increment = 20;  // how many minutes between autosaves
  ON_ClassArray<ON_wString> m_always_autosave_before_commands; // List of commands that force AutoSave prior to running

  // true for users who consider view changes a document change
  bool m_save_view_changes = false;

  bool m_enable_file_locking = true; // Ensure that only one person at a time can have a file open for saving
  bool m_enable_file_locking_open_warning = true; // Display information dialog which identifies computer file is open on

  // Clipboard options
  bool m_copy_to_previous_rhino_version = true; // Copy both objects to the clipboard in both the current and previous Rhino clipboard formats.  This
                                         // means you will double the size of what is saved in the clipboard but will be able to copy from 
                                         // the current to the previous version using the clipboard.

  // By default, any preexisting .3dm files are renamed .3dm.back
  // when Rhino saves.  If this m_bCreate3dmBakFiles is false,
  // then Rhino does not create .3dmbak files.
  bool m_bCreate3dmBakFiles = true; // default = true

  bool CreateOtherBakFiles() const;
  void SetCreateOtherBakFiles(bool on);

  ON_wString BackupFileFolder() const;
  void SetBackupFileFolder(ON_wString);

  // Move autosave files to recycle bin
  bool m_recycle_autosave_files = true;

  // Keep multiple copies of autosave files in recycle bin
  //ON_DEPRECATED_MSG("No longer supported")
  bool m_keep_multiple_recycled_autosave_files = true;

  enum clipboard_on_exit : unsigned int
  {
    keep_clipboard_data = 0,        // Always keep clipboard data, regardless of size and never prompt the user
    delete_clipboard_data,          // Always delete clipboard data, regardless of size and never prompt the user
    prompt_user_when_clipboard_big  // Prompt user when clipboard memory is large
  };
  clipboard_on_exit m_clipboard_on_exit = CRhinoAppFileSettings::clipboard_on_exit::prompt_user_when_clipboard_big;

  // The import file layer matching setting belongs on CRhinoAppFileSettings
  // but that class cannot be modified without breaking the SDK.
  // This setting controls what happens to layers when the contents of an
  // external source file are imported into an existing model.  
  // The Import, Paste and Insert commands are a few
  // of the ways a file can be imported into an existing model.
  //   0 = import layer names using the full path found in
  //       the source file.
  //   1 = if the source file has a layer with named 
  //       "source_file_parent_layer_path/X"
  //       and the current model has a layer named 
  //       "current_model_parent_layer_path/X",
  //       then change all references from "source_file_parent_layer_path/X"
  //       to "current_model_parent_layer_path/X"
  //       when importing items from the source file.
  //       This mimics Rhino 4.0 behavior, where all short layer names had
  //       to be unique and the parent path was ignored.
  unsigned int ImportFileLayerMatchingOption() const;

  void SetImportFileLayerMatchingOption(
    unsigned int import_file_layer_matching_option
  );

  enum : unsigned int
  {
    ImportFileLayerMatchingOptionDefault = 0
  };

  bool WriteLocalTempFileWhenSaving() const;
  void SetWriteLocalTempFileWhenSaving(bool on);

  unsigned int FileSearchRecursiveDepth() const;
  void SetFileSearchRecursiveDepth(unsigned int);

  const ON_ClassArray<ON_wString>& ExcludedFiles() const;
  void SetExcludedFiles(const ON_ClassArray<ON_wString>&);

  bool AlwaysDisplayAllFiletypes() const;
  void SetAlwaysDisplayAllFiletypes(bool on);

  bool RemoveDotFromRhlFiles() const;
  void RemoveDotFromRhlFiles(bool on);


private:
  int m_import_file_layer_matching_option = CRhinoAppFileSettings::ImportFileLayerMatchingOptionDefault;

  class CRhAppFileSettingsPrivate* m_private = nullptr;
};


///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppFileSettings

#pragma region class CRhinoAppDontRepeatCommandSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppDontRepeatCommandSettings
{
public:
  CRhinoAppDontRepeatCommandSettings();

  bool operator==(const CRhinoAppDontRepeatCommandSettings& other) const;
  bool operator!=(const CRhinoAppDontRepeatCommandSettings& other) const;

  /*
  Description:
    Only use the m_dont_repeat_list if somebody modifies it via CRhinoAppSettings::SetDontRepeatCommands()
  Returns:
    A return value of true means CRhinoCommand don't repeat flags will be ignored and the m_dont_repeat_list
    will be used instead.  False means the individual CRhinoCommands will determine if they are repeatable.
  */
  bool UseDontRepeatList() const;
  /*
  Description:
    Parse input string and put command name tokens in m_dont_repeat_list.
  Parameters:
    lps [in] - String to parse.  Will use " ", "," and ";" as delimiters.
  Returns:
    Number of items added to m_dont_repeat_list.
  */
  int SetDontRepeatList( const wchar_t* lps);
  /*
  Description:
    Convert m_dont_repeat_list to space delited string
  Parameters:
    list [out] Receives delimited string
  */
  void GetDontRepeatList( ON_wString& list) const;

  ON_ClassArray<ON_wString>m_dont_repeat_list; // List of commands that will not repeat

  // For internal use only
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  static const CRhinoAppDontRepeatCommandSettings Default;
  void Defaults();
  bool CommandInDontRepeatList(const class CRhinoCommand* cmd) const;
private:
  friend class CRhinoAppSettings;
  friend class CRhAppSettingsData;
  void PopulateFromCommandList();
  const wchar_t* ProfileSection() const;
  void FormatEntryString(ON_wString& s, int i) const;
  bool m_loaded_profile_called = false;
  bool m_dont_repeat_found; // If true, CRhinoAppSettings::SetDontRepeatCommands() was called and values saved.

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppDontRepeatCommandSettings

#pragma region class CRhinoAppGeneralSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppGeneralSettings
{
public:
  CRhinoAppGeneralSettings();

  bool operator==(const CRhinoAppGeneralSettings& src) const;
  bool operator!=(const CRhinoAppGeneralSettings& src) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  static const CRhinoAppGeneralSettings Default;
  void Defaults();

  enum mouse_select_mode
  {
    crossing,
    window,
    combo
  };

  enum middle_mouse_mode
  {
    popup_menu,
    popup_toolbar,
    run_macro
  };

  int ParseStartUpCommandList( ON_ClassArray<ON_wString> &result) const;

  mouse_select_mode m_mouse_select_mode;

  // Popup menu
  int                       m_max_popup_menu_lines;
  ON_ClassArray<ON_wString> m_popup_favorites;
  // Commands
  ON_wString m_startup_commands;

  // Undo
  //   Undo records will be purged if 
  //   there are more than m_min_undo_steps 
  //     AND 
  //  they use more than m_max_undo_memory_kb.
  //  When the records are purged there will
  //  always be at least m_min_undo_steps.
  //  saved.
  int m_min_undo_steps;
  int m_max_undo_memory_mb; // in MB megabytes

  // Surface isoparm density
  int        m_new_object_isoparm_count;
  bool       m_show_surface_isoparms;
  // Middle mouse button
  middle_mouse_mode m_middle_mouse_mode;
  /*
  Description:
    Call this method to get the tool bar that will be displayed if
    popup_toolbar == m_middle_mouse_mode
  Returns:
    Returns pointer to tool bar to pop-up if found otherwise NULL
  */
  const class CRhinoUiToolBar* MiddleMouseToolBar() const;
  ON_wString        m_middle_mouse_toolbar_name;
  ON_wString        m_middle_mouse_macro;
  
  // If true middle mouse button manipulates the view, emulating other CAD apps.
  bool m_bMiddleMouseViewManipulation;
  
  // If true plain middle mouse button rotates the view instead of pans.
  bool m_bMiddleMousePlainButtonRotate;
  
  // If true middle mouse button zoom is activated with Shift key instead of Control.
  bool m_bMiddleMouseShiftControlSwap;

  // Right mouse button

  // True if right mouse down + delay will pop up
  // context menu on a mouse up if no move happens.
  bool m_bEnableContextMenu;

  // number of milliseconds to wait before 
  // permitting context menu display
  int  m_context_menu_delay;

#if defined (ON_RUNTIME_APPLE)
  // True if two finger trackpad scrolling on Mac laptops is enabled
  bool m_bEnableTrackpadScrolling;
  
  // macros for Mac trackpad swipes
  ON_wString m_trackpad_swipe_3_left_macro;
  ON_wString m_trackpad_swipe_3_right_macro;
  ON_wString m_trackpad_swipe_3_up_macro;
  ON_wString m_trackpad_swipe_3_down_macro;
  
  ON_wString m_trackpad_swipe_4_left_macro;
  ON_wString m_trackpad_swipe_4_right_macro;
  ON_wString m_trackpad_swipe_4_up_macro;
  ON_wString m_trackpad_swipe_4_down_macro;
  
  // macros for main screen edges
  ON_wString m_screen_touch_left_edge_macro;
  ON_wString m_screen_touch_right_edge_macro;
  ON_wString m_screen_touch_top_edge_macro;
  ON_wString m_screen_touch_bottom_edge_macro;
  
  // macros for second screen edges
  ON_wString m_screen2_touch_left_edge_macro;
  ON_wString m_screen2_touch_right_edge_macro;
  ON_wString m_screen2_touch_top_edge_macro;
  ON_wString m_screen2_touch_bottom_edge_macro;

#endif
  
  // Material persistence

  // If true, the "Save" command will save every material
  // including the ones that are not used by any object
  // or layer.
  bool m_bSaveUnreferencedMaterials;

  // DEPRECATED - materials are no longer shared in Rhino.  This value will always be false.
  bool m_bShareMaterials; //DEPRECATED

  // If m_bSplitCreasedSurfaces is true, then 
  // surfaces are automatically split into
  // polysurfaces with smooth faces when they are added
  // to the CRhinoDoc.  Never perminantly change the
  // value of this setting.  It was a mistake to
  // put this setting in the public SDK.
  //
  // To temporarily set m_bSplitCreasedSurfaces to false,
  // create a CRhinoKeepKinkySurfaces on the stack
  // like this:
  // {  
  //   CRhinoKeepKinkySurfaces keep_kinky_surfaces;
  //   ... code that adds kinky surfaces to CRhinoDoc ...
  // }
  bool m_bSplitCreasedSurfaces;

  // CommandContext dialog auto-update feature
  bool m_bAutoUpdateCommandContext;

  // If true, then parent layers control the visible
  // and locked modes of sublayers. Otherwise, layers
  // operate independently.
  bool m_bEnableParentLayerControl;

  // If true, objects with texture mappings that are
  // copied from other objects will get the same 
  // texture mapping.  Otherwise the new object gets
  // a duplicate of the original texture mapping so
  // that the object's mappings can be independently
  // modified.  The default is false.
  bool m_bShareTextureMappings;

  // when true Control+Shift+click/window selects subobjects in addition to group members
  bool m_bControlShiftSubObjectSelect;

  // When true object can be selected even if control points are on.
  bool m_bPointsOnObjectSelect;

  // When true single selecting objects (currently curves and annotations) automatically turns on control points.
  bool m_bAutomaticPointsOn;

  // when true commands that create objects use extrusions instead of breps whenever possible.
  bool m_bUseExtrusions;

  // When true control points can be selected by picking the connecting control polygon
  bool m_bPickableControlPolygon;

private:
  int ParseCmdList( const ON_wString& in, ON_ClassArray<ON_wString> &out) const;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppGeneralSettings

#pragma region class CRhinoKeepKinkySurfaces
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoKeepKinkySurfaces
{
public:

  // This constructor saves the current value of
  //
  //   RhinoApp().AppSettings().GeneralSettings().m_bSplitCreasedSurfaces
  // 
  // in m_bSavedSetting and then sets the app setting to false.
  CRhinoKeepKinkySurfaces();

  // This constructor saves the current value of
  //
  //   RhinoApp().AppSettings().GeneralSettings().m_bSplitCreasedSurfaces
  // 
  // in m_bSavedSetting and then sets the app setting to !bKeep.
  CRhinoKeepKinkySurfaces(bool bKeep);

  // The destructor sets
  //
  //   RhinoApp().AppSettings().GeneralSettings().m_bSplitCreasedSurfaces
  //
  // to the value saved in m_bSavedSetting.
  ~CRhinoKeepKinkySurfaces();

private:
  bool m_bSavedSetting;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoKeepKinkySurfaces

#pragma region class CRhinoAppGridSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppGridSettings
{
public:
  CRhinoAppGridSettings();

  bool operator==(const CRhinoAppGridSettings& src) const;
  bool operator!=(const CRhinoAppGridSettings& src) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  static const CRhinoAppGridSettings Default;
  void Defaults();

  // construction plane grid line properties
  int          m_thick_line_width; // 1 or 2
  int          m_axis_line_width;  // 1 or 2
  unsigned int m_line_stipple_pattern; 
  bool         m_show_zaxis;
  
  // grid colors
  ON_Color m_thin_line_color;
  ON_Color m_thick_line_color;
  ON_Color m_xaxis_color;
  ON_Color m_yaxis_color;
  ON_Color m_zaxis_color;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppGridSettings

#pragma region class CRhinoAppModelAidSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppModelAidSettings
{
public:
  CRhinoAppModelAidSettings();

  bool operator==(const CRhinoAppModelAidSettings& src) const;
  bool operator!=(const CRhinoAppModelAidSettings& src) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppModelAidSettings Default;

  // Grid snap
  bool   m_grid_snap_on;
  // ortho
  bool   m_ortho_on;
  double m_ortho_angle;
  // planar mode
  bool   m_planar_on;
  // object snaps
  double m_digitizer_osnap_pickbox_radius;
  int    m_osnap_pickbox_radius;
  bool   m_project_snap_to_cplane;
  bool   m_use_horizontal_dialog;
  bool   m_center_snap_to_approximations;
  // trim / extend
  bool   m_extend_trim_lines;
  bool   m_extend_to_apparent_int;

  // nudge steps in units
  double m_nudge_key_step;
  double m_ctrl_nudge_key_step;
  double m_shift_nudge_key_step;

  // nudge keys and direction
  bool   m_alt_plus_arrows; // true mean Alt+arrow is used for nudging.
  int    m_nudge_mode; // 0 = world, 1 = cplane, 2 = view, 3 = uvn, -1 = not set
  // obsolete, replaced by m_nudge_mode
  //bool   m_use_cplane_axes; // True means use cplane, false means use world
  
  // control polygon display
  bool   m_display_control_polygon;
  bool   m_highlight_control_polygon;
  int    m_control_polygon_display_density;

  // osnap
  bool   m_suspend_osnap;
  bool   m_snap_to_locked;
  bool   m_snap_to_occluded;
  bool   m_snap_to_filtered;
  bool   m_only_snap_to_selected;
  enum cursor_mode
  {
    no_osnap_cursor = 0,
    black_on_white_osnap_cursor,
    white_on_black_osnap_cursor
  };
  int    m_cursor_mode; // See enum cursor_mode

  unsigned int m_osnap_mode; // bits set using osnap_mode enum

  /////////
  // radius of mouse pick box in pixels
  int m_mouse_pickbox_radius;

  enum point_display_mode
  {
    world_point = 0, // points are displayed in world coordinates
    cplane_point,    // points are displayed in cplane coordinates
    invalid_point_display_mode // invalid, unset value.
  };

  point_display_mode m_point_display_mode;

  // Universal construction plane mode
  bool m_uplane_mode;

  // Projecting osnaps in plan parallel views
  bool m_project_snap_to_cplane_in_plan_parallel_views;

  // osnapping to mesh objects
  bool m_nearmidintperp_snap_to_meshes;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppModelAidSettings

#pragma region class CRhinoAppSpaceBallSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppSpaceBallSettings
{
public:
  CRhinoAppSpaceBallSettings();
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppSpaceBallSettings Default;

  bool operator==(const CRhinoAppSpaceBallSettings& src) const;
  bool operator!=(const CRhinoAppSpaceBallSettings& src) const;

  // spacemouse/spaceball settings
  int    m_rotation_scale;
  int    m_translation_scale;
  bool   m_rotate_parallel_view;
  bool   m_swap_pan_and_zoom;
  bool   m_allow_tilt;
  bool   m_swap_zoom_with_dolly;
  bool   m_rotate_about_camera;

#if defined (ON_RUNTIME_APPLE)
  ON_wString m_button_1_macro;
  ON_wString m_button_2_macro;
  ON_wString m_button_3_macro;
  ON_wString m_button_4_macro;
  ON_wString m_button_5_macro;
  ON_wString m_button_6_macro;
  ON_wString m_button_7_macro;
  ON_wString m_button_8_macro;
#endif

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppSpaceBallSettings

#pragma region class CRhinoAppViewSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppViewSettings
{
public:
  CRhinoAppViewSettings();
  CRhinoAppViewSettings(const CRhinoAppViewSettings&);
  ~CRhinoAppViewSettings();
  CRhinoAppViewSettings& operator=(const CRhinoAppViewSettings&);

  bool operator==(const CRhinoAppViewSettings& src) const;
  bool operator!=(const CRhinoAppViewSettings& src) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppViewSettings Default;

  // pan
  double m_pan_increment;
  bool   m_pan_reverse_keyboard;
  bool   m_always_pan_parallel_views;

  // zoom
  double m_zoom_scale;

  double ZoomExtentsParallelViewBorder() const;
  void   SetZoomExtentsParallelViewBorder(double val);
  double ZoomExtentsPerspectiveViewBorder() const;
  void   SetZoomExtentsPerspectiveViewBorder(double val);

  // rotate
  int    m_rotate_circle_increment;
  bool   m_rotate_reverse_keyboard;

  enum rotate_to_view
  {
    rotate_around_world_axes = 0,
    rotate_relative_to_view = 1,
    rotate_relative_to_view_v2_style = 2,
    rotate_around_cplane_zaxis = 3,
  };
  rotate_to_view m_rotate_to_view;
  void SetRotateToView(int);
  bool RotateRelativeToView() const;
  bool RotateAroundCPlaneZaxis() const;

  // Andy le Bihan - 2010.12.09 - added 4 bytes to fill structure padding.  Reserved bytes are initialized to 0.
  // m_bits is used for 0x01: NamedViewsSetClippingPlanes (true by default)
  //					0x02: NamedViewsSetDisplayMode (true by default)
  //          0x04: AutoAdjustTargetDepth (true by default) 1 Feb 2011, Mikko
  //          0x08: RotateViewAroundAutogumball (false by default) 14 Feb 2011, Mikko
  //          0x10: Pan plan parallel views with Control+Shift+RMB
  //          0x20: RotateViewAroundObjectAtMouseCursor (false by default) 23 May 2023, Mikko
  unsigned char	 m_bits;

  bool NamedViewsSetClippingPlanes() const;
  void SetNamedViewsSetClippingPlanes(bool);

  bool NamedViewsSetDisplayMode() const;
  void SetNamedViewsSetDisplayMode(bool);

  bool AutoAdjustTargetDepth() const;
  void SetAutoAdjustTargetDepth(bool);

  bool RotateViewAroundAutogumball() const;
  void SetRotateViewAroundAutogumball(bool);

  bool PanPlanParallelViewsWithControlShiftRMB() const;
  void SetPanPlanParallelViewsWithControlShiftRMB(bool);

  bool RotateViewAroundObjectAtMouseCursor() const;
  void SetRotateViewAroundObjectAtMouseCursor(bool);

  // speedy redraw settings
  double m_fast_regen_frames_per_second; // must be a number >= 0

  // what happens when defined view are loaded
  bool   m_defined_view_set_cplane;
  bool   m_defined_view_set_projection;

  // view-port
  bool   m_single_click_maximize;
  bool   m_linked_viewports;
  bool   m_viewport_cursor_wrap;

  double TwoPointPerspectiveLensLength() const;   // default is 20 mm
  double ThreePointPerspectiveLensLength() const; // default is 50 mm

  double m_camera_lense_length;

private:
  class CRhAppViewSettingsPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppViewSettings

#pragma region class CRhinoAppShortcutKeys

enum class RhinoKeyboardModifierKey : unsigned char
{
  None = 0,
  Ctrl = 1,
  MacCommand = 1, // same as Ctrl
  Shift = 2,
  Alt = 4,
  MacControl = 8
};

RHINO_SDK_FUNCTION
RhinoKeyboardModifierKey RhinoKeyboardModifierKeyFromUnsigned(unsigned int val);

class RHINO_SDK_CLASS CRhinoKeyboardShortcut
{
public:
  RhinoKeyboardModifierKey m_modifier = RhinoKeyboardModifierKey::None;
  unsigned int m_keycode = 0;
  ON_wString m_macro;
private:
  class CRhKeyboardShortcutPrivate* m_private = nullptr;
};


//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppShortcutKeys
{
public:
  CRhinoAppShortcutKeys();
  CRhinoAppShortcutKeys( const CRhinoAppShortcutKeys& src);
  ~CRhinoAppShortcutKeys();

  CRhinoAppShortcutKeys& operator=( const CRhinoAppShortcutKeys& src);

  bool operator==(const CRhinoAppShortcutKeys& src) const;
  bool operator!=(const CRhinoAppShortcutKeys& src) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();

  enum shortcut_key
  {
    f1 = 0,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,
    ctrl_f1,
    ctrl_f2,
    ctrl_f3,
    ctrl_f4,
    ctrl_f5,
    ctrl_f6,
    ctrl_f7,
    ctrl_f8,
    ctrl_f9,
    ctrl_f10,
    ctrl_f11,
    ctrl_f12,
    shift_ctrl_f1,
    shift_ctrl_f2,
    shift_ctrl_f3,
    shift_ctrl_f4,
    shift_ctrl_f5,
    shift_ctrl_f6,
    shift_ctrl_f7,
    shift_ctrl_f8,
    shift_ctrl_f9,
    shift_ctrl_f10,
    shift_ctrl_f11,
    shift_ctrl_f12,
    alt_ctrl_f1,
    alt_ctrl_f2,
    alt_ctrl_f3,
    alt_ctrl_f4,
    alt_ctrl_f5,
    alt_ctrl_f6,
    alt_ctrl_f7,
    alt_ctrl_f8,
    alt_ctrl_f9,
    alt_ctrl_f10,
    alt_ctrl_f11,
    alt_ctrl_f12,

    ctrl_a,
    ctrl_b,
    ctrl_c,
    ctrl_d,
    ctrl_e,
    ctrl_f,
    ctrl_g,
    ctrl_h,
    ctrl_i,
    ctrl_j,
    ctrl_k,
    ctrl_l,
    ctrl_m,
    ctrl_n,
    ctrl_o,
    ctrl_p,
    ctrl_q,
    ctrl_r,
    ctrl_s,
    ctrl_t,
    ctrl_u,
    ctrl_v,
    ctrl_w,
    ctrl_x,
    ctrl_y,
    ctrl_z,
    shift_ctrl_a,
    shift_ctrl_b,
    shift_ctrl_c,
    shift_ctrl_d,
    shift_ctrl_e,
    shift_ctrl_f,
    shift_ctrl_g,
    shift_ctrl_h,
    shift_ctrl_i,
    shift_ctrl_j,
    shift_ctrl_k,
    shift_ctrl_l,
    shift_ctrl_m,
    shift_ctrl_n,
    shift_ctrl_o,
    shift_ctrl_p,
    shift_ctrl_q,
    shift_ctrl_r,
    shift_ctrl_s,
    shift_ctrl_t,
    shift_ctrl_u,
    shift_ctrl_v,
    shift_ctrl_w,
    shift_ctrl_x,
    shift_ctrl_y,
    shift_ctrl_z,
    alt_ctrl_a,
    alt_ctrl_b,
    alt_ctrl_c,
    alt_ctrl_d,
    alt_ctrl_e,
    alt_ctrl_f,
    alt_ctrl_g,
    alt_ctrl_h,
    alt_ctrl_i,
    alt_ctrl_j,
    alt_ctrl_k,
    alt_ctrl_l,
    alt_ctrl_m,
    alt_ctrl_n,
    alt_ctrl_o,
    alt_ctrl_p,
    alt_ctrl_q,
    alt_ctrl_r,
    alt_ctrl_s,
    alt_ctrl_t,
    alt_ctrl_u,
    alt_ctrl_v,
    alt_ctrl_w,
    alt_ctrl_x,
    alt_ctrl_y,
    alt_ctrl_z,
 
    ctrl_0,
    ctrl_1,
    ctrl_2,
    ctrl_3,
    ctrl_4,
    ctrl_5,
    ctrl_6,
    ctrl_7,
    ctrl_8,
    ctrl_9,
    shift_ctrl_0,
    shift_ctrl_1,
    shift_ctrl_2,
    shift_ctrl_3,
    shift_ctrl_4,
    shift_ctrl_5,
    shift_ctrl_6,
    shift_ctrl_7,
    shift_ctrl_8,
    shift_ctrl_9,
    alt_ctrl_0,
    alt_ctrl_1,
    alt_ctrl_2,
    alt_ctrl_3,
    alt_ctrl_4,
    alt_ctrl_5,
    alt_ctrl_6,
    alt_ctrl_7,
    alt_ctrl_8,
    alt_ctrl_9,

    home,
    end,
    ctrl_home,
    ctrl_end,
    shift_home,
    shift_end,
    shift_ctrl_home,
    shift_ctrl_end,
    alt_ctrl_home,
    alt_ctrl_end,

    // 24 March 2020, Mikko, RH-57554
    // Added "tab". PageUp/Down combos haven't been exposed since 2003, 
    // hope it's ok to inject a new item above them.
    tab,

    page_up,
    page_down,
    shift_page_up,
    shift_page_down,
    ctrl_page_up,
    ctrl_page_down,
    shift_ctrl_page_up,
    shift_ctrl_page_down,
    alt_ctrl_page_up,
    alt_ctrl_page_down,

    none,

    // 29 September 2023, John Morse
    // https://mcneel.myjetbrains.com/youtrack/issue/RH-77352
    // Mac only Alt+XX shortcuts
    alt_home,
    alt_end,
    alt_a,
    alt_b,
    alt_c,
    alt_d,
    alt_e,
    alt_f,
    alt_g,
    alt_h,
    alt_i,
    alt_j,
    alt_k,
    alt_l,
    alt_m,
    alt_n,
    alt_o,
    alt_p,
    alt_q,
    alt_r,
    alt_s,
    alt_t,
    alt_u,
    alt_v,
    alt_w,
    alt_x,
    alt_y,
    alt_z,
    alt_0,
    alt_1,
    alt_2,
    alt_3,
    alt_4,
    alt_5,
    alt_6,
    alt_7,
    alt_8,
    alt_9,
    alt_f1,
    alt_f2,
    alt_f3,
    alt_f4,
    alt_f5,
    alt_f6,
    alt_f7,
    alt_f8,
    alt_f9,
    alt_f10,
    alt_f11,
    alt_f12,

    alt_shift_home,
    alt_shift_end,
    alt_shift_a,
    alt_shift_b,
    alt_shift_c,
    alt_shift_d,
    alt_shift_e,
    alt_shift_f,
    alt_shift_g,
    alt_shift_h,
    alt_shift_i,
    alt_shift_j,
    alt_shift_k,
    alt_shift_l,
    alt_shift_m,
    alt_shift_n,
    alt_shift_o,
    alt_shift_p,
    alt_shift_q,
    alt_shift_r,
    alt_shift_s,
    alt_shift_t,
    alt_shift_u,
    alt_shift_v,
    alt_shift_w,
    alt_shift_x,
    alt_shift_y,
    alt_shift_z,
    alt_shift_0,
    alt_shift_1,
    alt_shift_2,
    alt_shift_3,
    alt_shift_4,
    alt_shift_5,
    alt_shift_6,
    alt_shift_7,
    alt_shift_8,
    alt_shift_9,
    alt_shift_f1,
    alt_shift_f2,
    alt_shift_f3,
    alt_shift_f4,
    alt_shift_f5,
    alt_shift_f6,
    alt_shift_f7,
    alt_shift_f8,
    alt_shift_f9,
    alt_shift_f10,
    alt_shift_f11,
    alt_shift_f12,

    maccontrol_home,
    maccontrol_end,
    maccontrol_a,
    maccontrol_b,
    maccontrol_c,
    maccontrol_d,
    maccontrol_e,
    maccontrol_f,
    maccontrol_g,
    maccontrol_h,
    maccontrol_i,
    maccontrol_j,
    maccontrol_k,
    maccontrol_l,
    maccontrol_m,
    maccontrol_n,
    maccontrol_o,
    maccontrol_p,
    maccontrol_q,
    maccontrol_r,
    maccontrol_s,
    maccontrol_t,
    maccontrol_u,
    maccontrol_v,
    maccontrol_w,
    maccontrol_x,
    maccontrol_y,
    maccontrol_z,
    maccontrol_0,
    maccontrol_1,
    maccontrol_2,
    maccontrol_3,
    maccontrol_4,
    maccontrol_5,
    maccontrol_6,
    maccontrol_7,
    maccontrol_8,
    maccontrol_9,
    maccontrol_f1,
    maccontrol_f2,
    maccontrol_f3,
    maccontrol_f4,
    maccontrol_f5,
    maccontrol_f6,
    maccontrol_f7,
    maccontrol_f8,
    maccontrol_f9,
    maccontrol_f10,
    maccontrol_f11,
    maccontrol_f12,

    maccontrol_alt_home,
    maccontrol_alt_end,
    maccontrol_alt_a,
    maccontrol_alt_b,
    maccontrol_alt_c,
    maccontrol_alt_d,
    maccontrol_alt_e,
    maccontrol_alt_f,
    maccontrol_alt_g,
    maccontrol_alt_h,
    maccontrol_alt_i,
    maccontrol_alt_j,
    maccontrol_alt_k,
    maccontrol_alt_l,
    maccontrol_alt_m,
    maccontrol_alt_n,
    maccontrol_alt_o,
    maccontrol_alt_p,
    maccontrol_alt_q,
    maccontrol_alt_r,
    maccontrol_alt_s,
    maccontrol_alt_t,
    maccontrol_alt_u,
    maccontrol_alt_v,
    maccontrol_alt_w,
    maccontrol_alt_x,
    maccontrol_alt_y,
    maccontrol_alt_z,
    maccontrol_alt_0,
    maccontrol_alt_1,
    maccontrol_alt_2,
    maccontrol_alt_3,
    maccontrol_alt_4,
    maccontrol_alt_5,
    maccontrol_alt_6,
    maccontrol_alt_7,
    maccontrol_alt_8,
    maccontrol_alt_9,
    maccontrol_alt_f1,
    maccontrol_alt_f2,
    maccontrol_alt_f3,
    maccontrol_alt_f4,
    maccontrol_alt_f5,
    maccontrol_alt_f6,
    maccontrol_alt_f7,
    maccontrol_alt_f8,
    maccontrol_alt_f9,
    maccontrol_alt_f10,
    maccontrol_alt_f11,
    maccontrol_alt_f12,

    maccontrol_ctrl_home,
    maccontrol_ctrl_end,
    maccontrol_ctrl_a,
    maccontrol_ctrl_b,
    maccontrol_ctrl_c,
    maccontrol_ctrl_d,
    maccontrol_ctrl_e,
    maccontrol_ctrl_f,
    maccontrol_ctrl_g,
    maccontrol_ctrl_h,
    maccontrol_ctrl_i,
    maccontrol_ctrl_j,
    maccontrol_ctrl_k,
    maccontrol_ctrl_l,
    maccontrol_ctrl_m,
    maccontrol_ctrl_n,
    maccontrol_ctrl_o,
    maccontrol_ctrl_p,
    maccontrol_ctrl_q,
    maccontrol_ctrl_r,
    maccontrol_ctrl_s,
    maccontrol_ctrl_t,
    maccontrol_ctrl_u,
    maccontrol_ctrl_v,
    maccontrol_ctrl_w,
    maccontrol_ctrl_x,
    maccontrol_ctrl_y,
    maccontrol_ctrl_z,
    maccontrol_ctrl_0,
    maccontrol_ctrl_1,
    maccontrol_ctrl_2,
    maccontrol_ctrl_3,
    maccontrol_ctrl_4,
    maccontrol_ctrl_5,
    maccontrol_ctrl_6,
    maccontrol_ctrl_7,
    maccontrol_ctrl_8,
    maccontrol_ctrl_9,
    maccontrol_ctrl_f1,
    maccontrol_ctrl_f2,
    maccontrol_ctrl_f3,
    maccontrol_ctrl_f4,
    maccontrol_ctrl_f5,
    maccontrol_ctrl_f6,
    maccontrol_ctrl_f7,
    maccontrol_ctrl_f8,
    maccontrol_ctrl_f9,
    maccontrol_ctrl_f10,
    maccontrol_ctrl_f11,
    maccontrol_ctrl_f12,

    maccontrol_shift_home,
    maccontrol_shift_end,
    maccontrol_shift_a,
    maccontrol_shift_b,
    maccontrol_shift_c,
    maccontrol_shift_d,
    maccontrol_shift_e,
    maccontrol_shift_f,
    maccontrol_shift_g,
    maccontrol_shift_h,
    maccontrol_shift_i,
    maccontrol_shift_j,
    maccontrol_shift_k,
    maccontrol_shift_l,
    maccontrol_shift_m,
    maccontrol_shift_n,
    maccontrol_shift_o,
    maccontrol_shift_p,
    maccontrol_shift_q,
    maccontrol_shift_r,
    maccontrol_shift_s,
    maccontrol_shift_t,
    maccontrol_shift_u,
    maccontrol_shift_v,
    maccontrol_shift_w,
    maccontrol_shift_x,
    maccontrol_shift_y,
    maccontrol_shift_z,
    maccontrol_shift_0,
    maccontrol_shift_1,
    maccontrol_shift_2,
    maccontrol_shift_3,
    maccontrol_shift_4,
    maccontrol_shift_5,
    maccontrol_shift_6,
    maccontrol_shift_7,
    maccontrol_shift_8,
    maccontrol_shift_9,
    maccontrol_shift_f1,
    maccontrol_shift_f2,
    maccontrol_shift_f3,
    maccontrol_shift_f4,
    maccontrol_shift_f5,
    maccontrol_shift_f6,
    maccontrol_shift_f7,
    maccontrol_shift_f8,
    maccontrol_shift_f9,
    maccontrol_shift_f10,
    maccontrol_shift_f11,
    maccontrol_shift_f12,

    // 12 Aug 2024, Mikko, RH-82951
    // Added Control/Command Plus/Minus for zoom in/out.
    ctrl_plus,
    ctrl_minus
  };

  static bool IsAcceptableKeyCombo(unsigned int keycode, RhinoKeyboardModifierKey modifier);
  static shortcut_key FromKeyCombo(bool control, bool shift, bool alt, bool macControlKeyDown, unsigned int nChar);
  static void Label(int key, ON_wString& label);
  static void Label(bool control, bool shift, bool alt, bool macControlKeyDown, unsigned int keycode, ON_wString& label);

  const wchar_t* Macro( shortcut_key) const;
  const wchar_t* Macro(bool control, bool shift, bool alt, bool macControlKeyDown, unsigned int keycode) const;
  void SetMacro( shortcut_key, const wchar_t*);
  void SetMacro(unsigned int keycode, RhinoKeyboardModifierKey modifier, const wchar_t* macro);
  void ClearAllMacros();

  int GetAllShortcuts(ON_ClassArray<CRhinoKeyboardShortcut>& shortcuts) const;
protected:
  ON_DEPRECATED_MSG("This function should not be used.")
  bool KeyInRange( shortcut_key) const;

  ON_wString m_macros[177]; // keep at 177 in order to maintain same class size as Rhino 7

private:
  class CRhAppShortcutKeysPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppShortcutKeys

#pragma region class CRhinoCommandAlias
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoCommandAlias
{
public:
  CRhinoCommandAlias();
  CRhinoCommandAlias(const wchar_t* alias, const wchar_t* macro);
  CRhinoCommandAlias(const wchar_t* alias, const wchar_t* macro, bool instant);
  CRhinoCommandAlias(const CRhinoCommandAlias& other);
  CRhinoCommandAlias& operator=(const CRhinoCommandAlias& other);

  // 17 Jan 2006 S.Baer
  // Default C++ copy constructor, operator=, and destructor work fine

  ON_wString m_alias;
  ON_wString m_macro;

  bool IsInstant() const;
  void SetInstant(bool instant);
private:
  class CRhCommandAliasPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoCommandAlias

#pragma region class CRhinoAppAliasList
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppAliasList
{
public:
  CRhinoAppAliasList();
  CRhinoAppAliasList( const CRhinoAppAliasList& src);
  ~CRhinoAppAliasList();

  CRhinoAppAliasList& operator=( const CRhinoAppAliasList& src);

  bool operator==( const CRhinoAppAliasList&) const;
  bool operator!=( const CRhinoAppAliasList&) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppAliasList Default;

  void DestroyAliasList();  // remove all aliases from the list
  
  bool AddAlias( const wchar_t* alias, const wchar_t* macro); // return true if added, false if
                                                              // already in the list.
  bool AddAlias(const wchar_t* alias, const wchar_t* macro, bool instant);

  bool ModifyAlias( int index, const wchar_t* alias, const wchar_t* macro);
  bool ModifyAliasName( int index, const wchar_t* alias);
  bool ModifyAliasMacro( int index, const wchar_t* macro);
  bool ModifyAliasInstant(int index, bool instant);

  const CRhinoCommandAlias* Alias( int index) const;          // start at 0, return NULL when index is out of range
  const CRhinoCommandAlias* FindAlias( const wchar_t*) const;
  int AliasIndex( const wchar_t* alias) const; // returns -1 if not found
  bool GetMatchingAliases(const wchar_t* sPartialName, ON_SimpleArray<CRhinoCommandAlias*> &alias_list);

  bool DeleteAlias( int index);
  bool DeleteAlias( const wchar_t*);

  int Count() const;

  int InstantAliasDelayMilliseconds() const;
  void SetInstantAliasDelayMilliseconds(int delay);
protected:
  bool InRange( int) const;
  CRhinoCommandAlias* A( int);

  ON_ClassArray<CRhinoCommandAlias> m_alias_list;

private:
  class CRhAppAliasListPrivate* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppAliasList

#pragma region class CRhinoAppContextMenuSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppContextMenuSettings
{
public:
  CRhinoAppContextMenuSettings();

  bool operator==(const CRhinoAppContextMenuSettings&) const;
  bool operator!=(const CRhinoAppContextMenuSettings&) const;

  void LoadProfile(CRhinoProfileContext& pc);
  void SaveProfile( CRhinoProfileContext& ) const;

  CRhinoStringDictionary m_object_context_menu_items;
  CRhinoStringDictionary m_grip_context_menu_items;
  CRhinoStringDictionary m_view_context_menu_items;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppContextMenuSettings

#pragma region class CRhinoAppMouseSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppMouseSettings
{
public:
  CRhinoAppMouseSettings();

  bool operator==(const CRhinoAppMouseSettings&) const;
  bool operator!=(const CRhinoAppMouseSettings&) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  void Validate();

  static const CRhinoAppMouseSettings Default;

  // mouse event remap
  int m_left_mouse_down_remap;
  int m_right_mouse_down_remap;
  int m_middle_mouse_down_remap;

  int m_left_mouse_move_remap;
  int m_right_mouse_move_remap;
  int m_middle_mouse_move_remap;

  int m_left_mouse_up_remap;
  int m_right_mouse_up_remap;
  int m_middle_mouse_up_remap;

  int m_left_mouse_dblclk_remap;
  int m_right_mouse_dblclk_remap;
  int m_middle_mouse_dblclk_remap;

  // number of pixels the mouse must move to 
  // trigger any mouse move UI event
  int m_mouse_move_tol;

  // number of pixels the mouse must move to 
  // trigger a control point drag (>=m_mouse_move_tol)
  int m_mouse_move_grip_tol;

  // number of pixels the mouse must move to 
  // trigger an object drag event (>=m_mouse_move_tol)
  int m_mouse_move_object_tol;

  // true if mouse can drag selected objects
  bool m_bEnableSelectedObjectDrag;

  // true if mouse can drag unselected objects
  bool m_bEnableUnselectedObjectDrag;

  // true if mouse can drag selected grips
  bool m_bEnableSelectedGripDrag;

  // true if mouse can drag unselected grips
  bool m_bEnableUnselectedGripDrag;
  
#if defined (ON_RUNTIME_APPLE)
  // true if rotating with Mighty Mouse scroll ball is enabled
  bool m_bEnableMouseScrollBallRotation;
  
  // true if Magic Mouse gestures are enabled
  bool m_bEnableMagicMouseGestures;
  
  // true if rotating with Magic Mouse is enabled
  bool m_bEnableMagicMouseRotation;

  // macros for additional mouse buttons
  ON_wString m_mouse_button_4_macro;
  ON_wString m_mouse_button_5_macro;
  ON_wString m_mouse_button_6_macro;
  ON_wString m_mouse_button_7_macro;
  ON_wString m_mouse_button_8_macro;
  ON_wString m_mouse_button_9_macro;
  ON_wString m_mouse_button_10_macro;
  ON_wString m_mouse_button_11_macro;
  ON_wString m_mouse_button_12_macro;
#endif

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppMouseSettings

#pragma region class CRhinoAppChooseOneObjectSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppChooseOneObjectSettings
{
public:
  CRhinoAppChooseOneObjectSettings();

  bool operator==(const CRhinoAppChooseOneObjectSettings&) const;
  bool operator!=(const CRhinoAppChooseOneObjectSettings&) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppChooseOneObjectSettings Default;
  // Choose One Object dialog settings
  bool m_bFollowCursor;
  int m_xoffset;
  int m_yoffset;

  bool m_bAutomaticResize;
  int m_max_autoresize_items;

  bool m_bShowTitlebarAndBorder;

  bool m_bShowObjectName;
  bool m_bShowObjectType;
  bool m_bShowObjectColor;
  bool m_bShowObjectLayer;

  bool m_bDynamicHighlight;
  bool m_bUseCustomColor;
  ON_Color m_highlight_color;

  bool m_bShowAllOption;
  bool m_bShowObjectTypeDetails;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppChooseOneObjectSettings

#pragma region class CRhinoAppSmartTrackSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppSmartTrackSettings
{
  public:
  CRhinoAppSmartTrackSettings();
  bool operator==(const CRhinoAppSmartTrackSettings& src) const;
  bool operator!=(const CRhinoAppSmartTrackSettings& src) const;
  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppSmartTrackSettings Default;

  // SmartTrack settings
  bool m_bUseSmartTrack;
  
  int m_smartpoint_wait_ms;
  int m_max_smart_points;
  
  ON_Color m_line_color;
  ON_Color m_tanperp_line_color;
  ON_Color m_point_color;
  ON_Color m_active_point_color;
  ON_Color m_guide_color;
  
  bool m_bDottedLines;
  bool m_bSmartOrtho;
  bool m_bMarkerSmartPoint;
  bool m_bSmartTangents;
  bool m_bSmartSuppress;
  bool m_bStrongOrtho;
  bool m_bSemiPermanentPoints;
  bool m_bShowMultipleTypes;
  bool m_bParallels;
  bool m_bSmartBasePoint;

  bool m_bSmartPointAtCPlaneOrigin;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppSmartTrackSettings

#pragma region Mac Rhino specific
//
//-----------------------------------------------------------------------------
#if defined (ON_RUNTIME_APPLE)
///////////////////////////////////////////////////////////////////////////////
//
#pragma region class CRhinoAppToolPaletteSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppToolPaletteSettings
{
public:
  CRhinoAppToolPaletteSettings();
  bool operator==(const CRhinoAppToolPaletteSettings&) const;
  bool operator!=(const CRhinoAppToolPaletteSettings&) const;
  void LoadProfile( CRhinoProfileContext& ); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  
  int m_menu_delay;       // milliseconds before additional commands menu is displayed
  
  BOOL32 m_hide_other_tool_palettes;    // hide other active tool palettes when starting a command
  
  int m_tool_button_size;             // size of each tool button in pixels
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppToolPaletteSettings

#pragma region class CRhinoAppExperimentalSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppExperimentalSettings
{
public:
  CRhinoAppExperimentalSettings();
  bool operator==(const CRhinoAppExperimentalSettings&) const;
  bool operator!=(const CRhinoAppExperimentalSettings&) const;
  void LoadProfile( CRhinoProfileContext& ); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  
  // do not delete deprecated settings
  bool m_core_dumps_enabled;
  bool m_layer_backed_views;
  bool m_show_bitmap_icons;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppExperimentalSettings

#pragma region class CRhinoAppThemeSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppThemeSettings
{
public:
  CRhinoAppThemeSettings();
  bool operator==(const CRhinoAppThemeSettings&) const;
  bool operator!=(const CRhinoAppThemeSettings&) const;
  void LoadProfile( CRhinoProfileContext& ); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  
  // command options presentation style
  enum {
    command_options_dialog,
    command_options_in_sidebar,
  };
  
  // application visual theme
  enum {
    application_visual_light_theme = 0,
    application_visual_dark_theme,
  };
  
  int m_theme_index;
  bool m_use_ribbon_bar;
  bool m_use_top_palette;
  bool m_command_sidebar_has_osnaps_panel;
  bool m_layout_command_sidebar_has_osnaps_panel;
  int m_application_visual_theme;
  int m_command_options_presentation_style;
  int m_inspector_sidebar_sections;
  int m_layout_inspector_sidebar_sections;
  bool m_use_vibrancy;
  ON_UUID m_top_palette_id;
  ON_UUID m_command_palette_id;
  ON_UUID m_layout_utilities_palette_id;

  // modeling window sidebars
  bool m_left_sidebar_is_open;
  bool m_right_sidebar_is_open;
  double m_left_sidebar_width;
  double m_right_sidebar_width;

  // layout window sidebars
  bool m_layout_left_sidebar_is_open;
  bool m_layout_right_sidebar_is_open;
  double m_layout_left_sidebar_width;
  double m_layout_right_sidebar_width;

  // materials window sidebars
  bool m_materials_left_sidebar_is_open;
  bool m_materials_right_sidebar_is_open;
  double m_materials_left_sidebar_width;
  double m_materials_right_sidebar_width;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppThemeSettings
//
///////////////////////////////////////////////////////////////////////////////

#endif
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion Mac Rhino specific
//
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//
#pragma region class CRhinoAppCursorToolTipSettings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoAppCursorToolTipSettings
{
public:
  CRhinoAppCursorToolTipSettings();

  bool operator==(const CRhinoAppCursorToolTipSettings&) const;
  bool operator!=(const CRhinoAppCursorToolTipSettings&) const;

  void LoadProfile(CRhinoProfileContext& pc); // load from registry
  void SaveProfile( CRhinoProfileContext& ) const; // save in registry
  void Defaults();
  static const CRhinoAppCursorToolTipSettings Default;

  // Cursor ToolTip settings
  bool m_bEnableCursorToolTips;
  
  int m_xoffset;
  int m_yoffset;
  
  ON_Color m_background_color;
  ON_Color m_text_color;
  
  bool m_bOsnapPane;
  bool m_bDistancePane;
  bool m_bPointPane;
  bool m_bRelativePointPane;
  bool m_bCommandPromptPane;
  bool m_bAutoSuppress;

  bool EnableGumballToolTips() const;
  void SetEnableGumballToolTips(bool bEnable);

private:
  bool m_bEnableGumballTooltips;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppCursorToolTipSettings

#pragma region class CRhinoNamedColor
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoNamedColor
{
public:
  CRhinoNamedColor();
  ON_Color m_color;
  ON_wString m_color_name;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoNamedColor

#pragma region class CRhinoGumballColors - gumball color settings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoGumballColors
{
public:
  CRhinoGumballColors();

  static int Compare(const CRhinoGumballColors& a, const CRhinoGumballColors& b);
  bool operator==(const CRhinoGumballColors&) const;
  bool operator!=(const CRhinoGumballColors&) const;

  ON_Color m_xcolor; // default is red
  ON_Color m_ycolor; // default is green
  ON_Color m_zcolor; // default is blue
  ON_Color m_menubuttoncolor;
  
  /*
   Description:
   Set members to factory gumball colors.
   */
  ON_DEPRECATED_MSG("use CRhinoGumballColors::Reset")
  void SetToFactoryColors();

  /*
  Description:
    Set members to default gumball colors. These are the current settings, not the built in defaults.
  */
  void SetToDefaultColors();

  /*
  Description:
    Set members to true built in original default gumball colors.
  */
  void Reset();

  /*
  Description:
    Saves the CRhinoGumballColors in an ON_BinaryArchive.
    This Write function includes a versioned chunk wrapper.
  Parameters:
    binary_archive - [in/out]
      The CRhinoGumballColors information is appended to
      this archive.
  Returns:
    True if the write was successful.
  */
  bool Write(ON_BinaryArchive& binary_archive) const;

  /*
  Description:
    Reads the CRhinoGumballColors from an ON_BinaryArchive.
  Parameters:
    binary_archive - [in]
      The CRhinoGumballColors information is read from
      the archive.  Reading begins at the current
      location in the archive.
  Returns:
    True if the read was successful.
  */
  bool Read(ON_BinaryArchive& binary_archive);

  /*
  Description:
    Saves the CRhinoGumballColors settings to a CRhinoProfileContext.
  Parameters:
    lpszSection - [in]
    pc - [in];
  Returns:
    True if the write was successful.
  */
  bool Write( LPCTSTR lpszSection, CRhinoProfileContext& pc ) const;

  /*
  Description:
    Reads the CRhinoGumballColors settings from a CRhinoProfileContext.
    Any settings that are not in the profile context are
    set to the default values.
  Parameters:
    lpszSection - [in]
    pc - [in];
  Returns:
    True if the read was successful.
  */
  bool Read( LPCTSTR lpszSection, CRhinoProfileContext& pc );

  /*
    These static values are used by the constructor.  They
    are set by CRhinoApp.AppSettings().SetGumballColors().
  */
  static ON_Color default_xcolor; // default is red
  static ON_Color default_ycolor; // default is green
  static ON_Color default_zcolor; // default is blue
  static ON_Color default_menubuttoncolor;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoGumballColors - gumball color settings

#pragma region class CRhinoGumballSizes - gumball size settings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoGumballSizes
{
public:
  CRhinoGumballSizes();

  static int Compare(const CRhinoGumballSizes& a, const CRhinoGumballSizes& b);
  bool operator==(const CRhinoGumballSizes&) const;
  bool operator!=(const CRhinoGumballSizes&) const;

  // Size of the translation and rotation
  // gumball controls in pixels
  int m_gumball_radius;     // in pixels
  int m_gumball_tip_length; // length of arrow head in pixels
  int m_gumball_tip_width;  // width of arrow head in pixels
  int m_gumball_tail_size;  // scale grip size
  int m_gumball_ptran_dist; // planar translation grip corner
  int m_gumball_ptran_size; // planar translation grip size
  int m_axis_thickness;     // in pixels
  int m_arc_thickness;      // in pixels
  int m_menu_dist;          // distance of menu ball from center
  int m_menu_size;          // radius of menu circle
  double m_gumball_ptran_angle; // planar translation grip visibility angle
  int m_extrude_ball_size;
  
  /*
  Description:
    Set members to factory gumball sizes.
  */
  ON_DEPRECATED_MSG("use CRhinoGumballSizes::Reset")
  void SetToFactorySizes();

  /*
  Description:
    Set members to default gumball sizes. These are the current settings, not the built in defaults.
  */
  void SetToDefaultSizes();

  /*
  Description:
    Set members to true built in original default gumball sizes.
  */
  void Reset();

  /*
  Description:
    Gets the world coordinate gumball radius.
  Parameters:
    viewport - [in]
    center - [in] center of gumball
    world_radius - [out]
  Returns:
    True if a valid world_radius is returned.
  */
  bool GetWorldRadius( 
          const ON_Viewport& viewport,
          const ON_3dPoint& center,
          double* world_radius 
          ) const;


  /*
  Description:
    Saves the CRhinoGumballAppearance in an ON_BinaryArchive.
    This Write function includes a versioned chunk wrapper.
  Parameters:
    binary_archive - [in/out]
      The CRhinoGumballAppearance information is appended to
      this archive.
  Returns:
    True if the write was successful.
  */
  bool Write(ON_BinaryArchive& binary_archive) const;

  /*
  Description:
    Reads the CRhinoGumballAppearance from an ON_BinaryArchive.
  Parameters:
    binary_archive - [in]
      The CRhinoGumballAppearance information is read from
      the archive.  Reading begins at the current
      location in the archive.
  Returns:
    True if the read was successful.
  */
  bool Read(ON_BinaryArchive& binary_archive);

  /*
  Description:
    Saves the CRhinoGumballAppearance settings to a CRhinoProfileContext.
  Parameters:
    lpszSection - [in]
    pc - [in];
  Returns:
    True if the write was successful.
  */
  bool Write( LPCTSTR lpszSection, CRhinoProfileContext& pc ) const;

  /*
  Description:
    Reads the CRhinoGumballAppearance settings from a CRhinoProfileContext.
    Any settings that are not in the profile context are
    set to the default values.
  Parameters:
    lpszSection - [in]
    pc - [in];
  Returns:
    True if the read was successful.
  */
  bool Read( LPCTSTR lpszSection, CRhinoProfileContext& pc );

  /*
    These static values are used by the constructor.  They
    are set by CRhinoApp.AppSettings().SetGumballSizes().
  */
  static int default_gumball_radius;     // in pixels
  static int default_gumball_tip_length; // length of arrow head in pixels
  static int default_gumball_tip_width;  // length of arrow head in pixels
  static int default_gumball_tail_size;  // scale grip size
  static int default_gumball_ptran_dist; // location of planar translation grip size
  static int default_gumball_ptran_size; // planar translation grip size
  static int default_axis_thickness;     // in pixels
  static int default_arc_thickness;      // in pixels
  static int default_menu_dist;
  static int default_menu_size;
  static double default_gumball_ptran_angle;  // planar translation grip visibility angle
  static int default_extrude_ball_size;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoGumballSizes - gumball size settings

#pragma region class CRhinoWidgetColorSettings - widget (mapping, decal) color settings
//
//-----------------------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoWidgetColorSettings
{
public:
  // Initializes to factory defaults.
  CRhinoWidgetColorSettings();

  bool operator == (const CRhinoWidgetColorSettings&) const;
  bool operator != (const CRhinoWidgetColorSettings&) const;

  // Sets the members from the current color cache.
  void SetFromCurrent(void);

  // Copies the members to the current color cache.
  void SetToCurrent(void) const;

  // Sets the members from the factory defaults.
  void Defaults(void);

  // Factory defaults.
  static CRhinoWidgetColorSettings Default;

  LPCTSTR ProfileKey(void) const;
  void LoadProfile(CRhinoProfileContext& pc);
  void SaveProfile( CRhinoProfileContext& context) const;

public:
  ON_Color m_u_color;
  ON_Color m_v_color;
  ON_Color m_w_color;

  // These static members are a cache to optimize the retrieval
  // of the current system widget colors. This optimization is
  // needed because these values are used during view-port drawing.
  // These are the colors set in the color options dialog.
  static ON_Color current_u_color;
  static ON_Color current_v_color;
  static ON_Color current_w_color;

private:
  ON__UINT_PTR m_private = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoWidgetColorSettings - widget (mapping, decal) color settings

#pragma region class CRhinoUiPaintColorSettings

// Colors for the general windows user interface elements
class RHINO_SDK_CLASS CRhinoUiPaintColorSettings
{
public:
  CRhinoUiPaintColorSettings();
  ~CRhinoUiPaintColorSettings();

  CRhinoUiPaintColorSettings(const CRhinoUiPaintColorSettings&);
  CRhinoUiPaintColorSettings& operator=(const CRhinoUiPaintColorSettings&);

  bool operator==(const CRhinoUiPaintColorSettings&) const;
  bool operator!=(const CRhinoUiPaintColorSettings&) const;

  // Gradient start for active toolbar tab and non-client area of Rhino.
  ON_Color m_normal_start;
  // Gradient end for active toolbar tab and non-client area of Rhino.
  ON_Color m_normal_end;
  // Edge color used for grippers, toolbar border, resize bars, status bar pane borders.
  ON_Color m_normal_border;
  // Color for hovering gradient start.
  ON_Color m_mouse_overcontrol_start;
  // Color for hovering gradient end.
  ON_Color m_mouse_overcontrol_end;
  // Active floating window non-client area.
  ON_Color m_mouse_overcontrol_border;
  //Gradient start for inactive toolbar tab.
  ON_Color m_hot_start;
  // Gradient end for inactive toolbar tab.
  ON_Color m_hot_end;
  //Inactive toolbar tab border.
  ON_Color m_hot_border;
  // Pressed gradient start.
  ON_Color m_pressed_start;
  // Pressed gradient end.
  ON_Color m_pressed_end;
  // Pressed border.
  ON_Color m_pressed_border;
  // Toolbar tab text and status bar text.
  ON_Color m_text_enabled;
  // Disabled text color.
  ON_Color m_text_disabled;
private:
  // Made private in V7. Use the TabBackground() function instead
  ON_Color m_tab_background__obsolete; //no longer used. Renamed with __obsolete
public:
  // Active viewport title.
  ON_Color m_active_viewport_title;
  // Inactive viewport title.
  ON_Color m_inactive_viewport_title;
  // Active floating window non-client area.
  ON_Color m_active_caption;
  // Inactive floating window non-client area.
  ON_Color m_inactive_caption;

  // Tab background color.
  ON_Color TabBackground() const;
  ON_Color TabBackground(bool activeTab, bool computeIfUnset) const;

  void SetTabBackground(const ON_Color& color);
  void SetTabBackground(const ON_Color& color, bool activeTab);

  // Modified property value label text color
  ON_Color ModifiedValueControlColor() const;
  
  //Background fill for input elements (edit box, checkbox).
  ON_Color EditBoxBackground() const;
  ON_Color EditBoxBackground(bool computeIfUnset) const;

  void SetEditBoxBackground(const ON_Color& color);

  ON_Color GridLineColor(bool onEditBackground) const;

  // Returns true if Rhino is running in dark-theme mode.
  bool RunningInDarkMode() const;

  void Defaults();
  static const CRhinoUiPaintColorSettings Default;

  static const wchar_t* ProfileKey();
  void LoadProfile(CRhinoProfileContext& pc);
  void SaveProfile( CRhinoProfileContext& ) const;
  void Validate();

private:
  class CRhUiPaintColorSettingsPrivate* m_private = nullptr;
  friend class CRhDefaultColors;
};

#pragma endregion class CRhinoUiPaintColorSettings

#pragma region class CRhinoAppSelectionFilterSettings
class RHINO_SDK_CLASS CRhinoAppSelectionFilterSettings
{
public:
  CRhinoAppSelectionFilterSettings();

  bool operator==(const CRhinoAppSelectionFilterSettings&) const;
  bool operator!=(const CRhinoAppSelectionFilterSettings&) const;

  void LoadProfile(CRhinoProfileContext& pc);
  void SaveProfile(CRhinoProfileContext&) const;
  void Defaults();
  void Validate();

  static const CRhinoAppSelectionFilterSettings Default;

  bool GetGlobalSelectionFilter(unsigned int* pFilter, const unsigned int rhino_doc_sn = 0) const;
  bool PlainClickSubObjectSelect() const;

  unsigned int m_nGlobalGeometryFilter;
  unsigned int m_nOneShotGeometryFilter;
  bool m_bEnableGlobalGeometryFilter;
  bool m_bPlainClickSubObjectSelect;

private:
  ON__UINT_PTR m_private = 0;
};
#pragma endregion class CRhinoAppSelectionFilterSettings

#pragma region class CRhinoAppSettings
//
// CRhinoAppSettings class - Rhino application settings
//-----------------------------------------------------------------------------

class RHINO_SDK_CLASS CRhinoAppSettings
{
private:
  CRhinoAppSettings();
  ~CRhinoAppSettings();
  CRhinoAppSettings(const CRhinoAppSettings&) = delete;
  CRhinoAppSettings& operator=(const CRhinoAppSettings&) = delete;

public:
  // This function WILL call OnAppSettingsChanged event watchers if any
  // settings change.
  void LoadProfile(CRhinoProfileContext& context);
  void SaveProfile(CRhinoProfileContext& context) const;

  void Defaults();
  
  // The CRhinoAppSettings::osnap_mode enum is OBSOLETE.
  // Use the ON::osnap_mode values.  The reason for the
  // change is that history records are saving these
  // values in ON_ObjRefs.
  //
  // Currently lots of files are checked out and I can't convert
  // Rhino's use CRhinoAppSettings::osnap_mode to ON::osnap_mode. 
  enum osnap_mode
  {
    os_none          = ON::os_none,
    os_near          = ON::os_near,
    os_focus         = ON::os_focus,
    os_center        = ON::os_center,
    os_vertex        = ON::os_vertex,
    os_knot          = ON::os_knot,
    os_quadrant      = ON::os_quadrant,
    os_midpoint      = ON::os_midpoint,
    os_intersection  = ON::os_intersection,
    os_end           = ON::os_end,
    os_perpendicular = ON::os_perpendicular,
    os_tangent       = ON::os_tangent,
    os_point         = ON::os_point,
  };

  enum osnap_command
  {
    osc_none               = 0x00000000,
    osc_between            = 0x00000010,
    osc_from_point         = 0x00000100,
    osc_perpendicular_from = 0x00000200,
    osc_tangent_from       = 0x00000400,
    osc_along_line         = 0x00001000,
    osc_along_parallel     = 0x00002000,
    osc_on_curve           = 0x00010000,
    osc_on_surface         = 0x00020000,
    osc_on_polysurface     = 0x00040000,
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // Show and Hide the Osnap dialog
  void ShowOsnapDialog(bool=true);
  bool OsnapDialogVisible();

  // Returns:
  //   An unsigned int with CRhinoAppSettings::osnap_mode bits set
  //   if an osnap is enabled.
  //   
  unsigned int OsnapMode() const;
  void SetOsnapMode(osnap_mode mode, bool b);

  bool SetEnableOsnaps(bool bEnable = true);
  bool EnableOsnaps() const;

  bool SetProjectOsnaps(bool bEnable = true);
  bool ProjectOsnaps() const;

  bool SetProjectOsnapsInPlanParallelViews(bool bEnable = true);
  bool ProjectOsnapsInPlanParallelViews() const;

  // Grid snap setting
  bool GridSnap() const;
  void EnableGridSnap(bool=true);

  // Ortho setting
  bool Ortho() const;
  void EnableOrtho(bool=true);

  // Planar setting
  bool Planar() const;
  void EnablePlanar(bool=true);

  // Persistent Osnaps
  bool OsnapMode( osnap_mode) const;

  // Returns: true if any osnaps are enabled.
  bool OsnapEnabled() const;

  //////////
  // The "original" mouse event is passed in and can be modified
  // by users who want to change one action to another.
  void RemapMouse(
    int,  // 0 = mouse down, 1 = mouse move, 2 = mouse up, 3 = mouse dbl click
    int&, // button 0 = none, 1 = left, 2 = right, 3 = middle
    UINT& // nFlags
    ) const;


  // viewport options
  bool DisplayViewportTitles() const;

  // true if changing a view should mark a document as modified
  bool SaveViewChanges() const;

  // construction plane grid colors
  ON_Color GridThinLineColor() const;
  ON_Color GridThickLineColor() const;
  ON_Color GridXAxisColor() const;
  ON_Color GridYAxisColor() const;
  ON_Color GridZAxisColor() const;

  // construction plane grid line properties
  int GridThickLineWidth() const;
  int GridAxisLineWidth() const;

  bool ShowZAxis(void) const;
  
  // world coordinate icon axes colors
  ON_Color WorldCoordIconXAxisColor() const;
  ON_Color WorldCoordIconYAxisColor() const;
  ON_Color WorldCoordIconZAxisColor() const;

  // length of the axes lines in pixels
  int WorldCoordIconAxisSize() const;
  
  // radius of the axes labels in pixels
  int WorldCoordIconLabelSize() const;

  // If WorldCoordIconMoveLabels() returns true, the
  // world axes icon labels position themselves so that
  // the axes lines are not drawn on top of the
  // axes labels.  If false, the labels are always drawn
  // to the right of the ends of the axes.
  bool WorldCoordIconMoveLabels() const;
  bool DefaultWorldCoordIconMoveLabels() const;

  // Length of direction arrow icon shaft in pixels
  int DirectionArrowIconShaftSize() const; 
  // Length of direction arrow icon head in pixels
  int DirectionArrowIconHeadSize() const;

  // viewport colors
  ON_Color ViewportBackgroundColor() const;
  ON_Color FrameBackgroundColor() const;

  // object state colors
  ON_Color SelectedObjectColor( bool bIsReferenceObject = false ) const;
  ON_Color SelectedGripColor() const;
  ON_Color LockedObjectColor( bool bIsReferenceObject = false ) const;
  ON_Color ActiveLayerColor() const;
  ON_Color FeedbackColor() const;
  ON_Color TrackingColor() const;
  ON_Color ShowEdgeColor() const;

  // command prompt visibility and position
  bool CommandPromptIsVisible() const;
  ON_DEPRECATED_MSG("Obsolete method, no longer used and does not reflect the actual command control state")
  CRhinoAppAppearanceSettings::command_prompt_position CommandPromptPosition() const;

  // status bar state
  bool StatusBarIsVisisble() const;
  bool ShowStatusBar(bool bShow = true);

  // mouse pick box size
  int MousePickboxRadius() const;

  // object snap pick box size
  int OsnapPickboxRadius() const;

  // 3D digitizer object snap pick box size
  double DigitizerOsnapPickboxRadius() const;

  CRhinoAppModelAidSettings::point_display_mode PointDisplayMode() const;

  //////////
  // rendered views options
  bool GetRenderedViewSettingsFromDoc() const;
  bool SetGetRenderedViewSettingsFromDoc( bool b);
  // If GetRenderedViewSettingsFromDoc() returns true then values for the
  // following are returned from CRhinoDoc::Properties().RenderSettings()
  // otherwise they are returned from CRhinoDisplaySettings
  bool DisplayCurvesAndIsocurvesInRenderedViews( const CRhinoDoc*) const;
  bool DisplayTextAndDimensionsInRenderedViews( const CRhinoDoc*) const;
  bool UseHiddenLightsInRenderedViews( const CRhinoDoc*) const;
  ON_Color AmbientLightColor( const CRhinoDoc*) const;
  ON_Color BackgroundColor( const CRhinoDoc*) const;

  // default properties and light
  ON_Color DefaultObjectColor() const;
  ON_DEPRECATED_MSG("use CRhinoMaterialTable::DefaultMaterial")
  const class CRhinoMaterial& DefaultMaterial() const;
  ON_DEPRECATED_MSG("use CRhinoMaterialTable::DefaultLockedMaterial")
  const class CRhinoMaterial& LockedObjectMaterial() const; // for RenderPreview
  const ON_Light& DefaultLight() const;

  bool UseDontRepeatList() const;

  void DefaultFontFaceName(ON_wString &font) const;

  //Returns:
  //  A managed font with facename = DefaultFontFaceName().
  const ON_Font& DefaultFont();

  bool EchoPromptsToHistory() const;
  bool EchoCommandsToHistory() const;
  
  const ON_ClassArray<CRhinoNamedColor>& NamedColorList();

  const CRhinoAppAppearanceSettings& AppearanceSettings( bool synch_with_ui = false) const;
  const CRhinoAppFileSettings& FileSettings() const;

  // The import file layer matching setting belongs on CRhinoAppFileSettings
  // but that class cannot be modified without breaking the SDK.
  // This setting controls what happens to layers when the contents of an
  // external source file are imported into an existing model.  
  // The Import, Paste and Insert commands are a few
  // of the ways a file can be imported into an existing model.
  //   0 = import layer names using the full path found in
  //       the source file.
  //   1 = if the source file has a layer with named 
  //       "source_file_parent_layer_path/X"
  //       and the current model has a layer named 
  //       "current_model_parent_layer_path/X",
  //       then change all references from "source_file_parent_layer_path/X"
  //       to "current_model_parent_layer_path/X"
  //       when importing items from the source file.
  //       This mimics Rhino 4.0 behavior, where all short layer names had
  //       to be unique and the parent path was ignored.
  unsigned int ImportFileLayerMatchingOption() const;

  const CRhinoAppDontRepeatCommandSettings& DontRepeatCommands() const;
  const CRhinoAppGeneralSettings& GeneralSettings() const;
  const CRhinoAppGridSettings& GridSettings() const;
  const CRhinoAppModelAidSettings& ModelAidSettings() const;
  const CRhinoAppViewSettings& ViewSettings() const;
  const CRhinoAppSpaceBallSettings& SpaceBallSettings() const;
  const CRhinoAppShortcutKeys& ShortCutKeys() const;
  const CRhinoAppAliasList& Aliases() const;
  const CRhinoAppMouseSettings& MouseSettings() const;


  const CRhinoEdgeAnalysisSettings& EdgeAnalysisSettings() const;
  const CRhinoEndAnalysisSettings& EndAnalysisSettings() const;
  const CRhinoDirectionAnalysisSettings& DirectionAnalysisSettings() const;
  const CRhinoEdgeContinuitySettings& EdgeContinuitySettings() const;
  const CRhinoZebraAnalysisSettings& ZebraAnalysisSettings() const;
  const CRhinoCurvatureGraphSettings& CurvatureGraphSettings() const;
  const CRhinoAppChooseOneObjectSettings& ChooseOneObjectSettings() const;
  const CRhinoAppSmartTrackSettings& SmartTrackSettings() const;
  const CRhinoAppCursorToolTipSettings& CursorToolTipSettings() const;
  const CRhinoAppContextMenuSettings& ContextMenuSettings() const;
#if defined (ON_RUNTIME_APPLE)
  const CRhinoAppToolPaletteSettings& ToolPaletteSettings() const;
  const CRhinoAppExperimentalSettings& ExperimentalSettings() const;
  const CRhinoAppThemeSettings& ThemeSettings() const;
#endif

  const CRhinoOpenGLSettings& OpenGLSettings() const;
  void  SetOpenGLSettings(const CRhinoOpenGLSettings&);

  const CRhinoDisplaySettings& DisplaySettings() const;
  const CRhinoEmapAnalysisSettings& EmapAnalysisSettings() const;
  const CRhinoDraftAngleAnalysisSettings& DraftAngleAnalysisSettings() const;
  const CRhinoThicknessAnalysisSettings& ThicknessAnalysisSettings() const;
  const CRhinoCurvatureAnalysisSettings& CurvatureAnalysisSettings() const;

#if defined RH_CONSTRAINTS
  const class CRhinoConstraintsSettings& ConstraintsSettings() const;
  void SetConstraintsSettings(const class CRhinoConstraintsSettings&);
#endif

  void SetAppearanceSettings( const CRhinoAppAppearanceSettings& settings);
  void SetFileSettings( const CRhinoAppFileSettings& settings);

  // See the comment above the declaration of CRhinoApp::ImportFileLayerMatchingOption()
  // for a description of the values import_file_layer_matching_option can have.
  void SetImportFileLayerMatchingOption( unsigned int import_file_layer_matching_option);

  void SetDontRepeatCommands( const CRhinoAppDontRepeatCommandSettings& settings);
  void SetGeneralSettings( const CRhinoAppGeneralSettings& settings);
  void SetGridSettings( const CRhinoAppGridSettings& settings);
  void SetModelAidSettings( const CRhinoAppModelAidSettings& settings);
  void SetViewSettings( const CRhinoAppViewSettings& settings);
  void SetSpaceBallSettings( const CRhinoAppSpaceBallSettings& settings);
  void SetShortCutKeys( const CRhinoAppShortcutKeys& settings);
  void SetAliases( const CRhinoAppAliasList& settings);
  void SetMouseSettings( const CRhinoAppMouseSettings& settings);

  void SetEdgeAnalysisSettings( const CRhinoEdgeAnalysisSettings& settings );
  void SetEndAnalysisSettings( const CRhinoEndAnalysisSettings& settings );
  void SetDirectionAnalysisSettings( const CRhinoDirectionAnalysisSettings& settings );
  void SetZebraAnalysisSettings( const CRhinoZebraAnalysisSettings& settings );
  void SetCurvatureGraphSettings( const CRhinoCurvatureGraphSettings& settings );
  void SetChooseOneObjectSettings( const CRhinoAppChooseOneObjectSettings& settings);
  void SetSmartTrackSettings( const CRhinoAppSmartTrackSettings& settings);
  void SetCursorToolTipSettings( const CRhinoAppCursorToolTipSettings& settings);
  void SetContextMenuSettings(const CRhinoAppContextMenuSettings& settings);
#if defined (ON_RUNTIME_APPLE)
  void SetToolPaletteSettings( const CRhinoAppToolPaletteSettings& settings);
  void SetExperimentalSettings( const CRhinoAppExperimentalSettings& settings);
  void SetThemeSettings( const CRhinoAppThemeSettings& settings);
#endif

  void SetEmapAnalysisSettings( const CRhinoEmapAnalysisSettings& settings);
  void SetEdgeContinuitySettings(const CRhinoEdgeContinuitySettings& settings);
  void SetDraftAngleAnalysisSettings( const CRhinoDraftAngleAnalysisSettings& settings);
  void SetThicknessAnalysisSettings( const CRhinoThicknessAnalysisSettings& settings);
  void SetCurvatureAnalysisSettings( const CRhinoCurvatureAnalysisSettings& settings);
  void SetDisplaySettings( const CRhinoDisplaySettings& settings);

  const CRhinoUiPaintColorSettings& UiPaintColorSettings() const;
  void SetUiPaintColorSettings( const CRhinoUiPaintColorSettings& settings );

  const CRhinoWidgetColorSettings& WidgetColorSettings() const;
  void SetWidgetColorSettings( const CRhinoWidgetColorSettings& settings );

  const CRhinoAppSelectionFilterSettings& SelectionFilterSettings() const;
  void  SetSelectionFilterSettings(const CRhinoAppSelectionFilterSettings&);

  ///////////////////////////////////////////////////////////////////
  //
  // BEGIN Gumball settings
  //

  // NOTE: When we are closer to shipping a finished gumball,
  //       There will be a CRhinoGumballSettings class that
  //       can be used to set/get all gumball related settings.
  //       While we are still testing the look and feel of
  //       the gumball, individual settings will be added here.

  // Rhino default gumball colors
  // The CRhinoGumballColors() constructor uses these values.
  void SetGumballColors(CRhinoGumballColors gumball_colors);

  // Set Rhino gumball sizes.
  // The CRhinoGumballSizes() constructor uses these values.
  void SetGumballSizes(CRhinoGumballSizes gumball_sizes);

  // When AutoGumball is on, a gumball automatically appears
  // when objects are prepicked.
  bool ShowAutoGumball() const;
  void SetShowAutoGumball( bool bShowAutoGumball );

  // When SnappyGumball is on, a dragging a gumball moves the
  // center point.  When snappy gumball is off, dragging a
  // gumball moves the mouse down point.
  bool SnappyGumball() const;
  void SetSnappyGumball( bool bSnappyGumball );

  //For the sake of not breaking the SDK, you will have to cast back and forward from int to CRhinoView::drag_plane
  int AutoGumballAlignment() const;
  void SetAutoGumballAlignment(int);

  //When the gumball extrudes a face it will
  //merge faces along the extrusion if possible
  bool AutoGumballExtrudeMergeFaces() const;
  void SetAutoGumballExtrudeMergeFaces(bool bMerge);

  bool GumballAutoReset() const;
  void SetGumballAutoReset(bool bAutoReset);

  //
  // END Gumball settings
  //
  ///////////////////////////////////////////////////////////////////

  bool AutoAlignCPlane() const;
  void SetAutoAlignCPlane(bool bAutoCPlane);

  //0 = object, 1 = world, 2 = view
  int AutoCPlaneAlignment() const;
  void SetAutoCPlaneAlignment(int iAlignment);

  bool StickyAutoCPlane() const;
  void SetStickyAutoCPlane(bool bSticky);

  bool OrientAutoCPlaneToView() const;
  void SetOrientAutoCPlaneToView(bool bAlign);

  // By default active osnap is indicated by thickening the wireframe of the osnapped object.
  // Disable osnap highlight to remove the effect.
  bool EnableOsnapHighlight() const;
  void SetEnableOsnapHighlight( bool bEnable);

  // If true, commands are in-line autocompleted and exact matches are shown in a popup menu.
  bool AutocompleteCommands() const;
  bool DefaultAutocompleteCommands() const;
  void SetAutocompleteCommands( bool bEnable);

  // If true, in-line auto-complete uses popularity sorting, and pop-up menu includes closest inexact matches.
  bool FuzzyAutocomplete() const;
  bool DefaultFuzzyAutocomplete() const;
  void SetFuzzyAutocomplete( bool bEnable);

  // If true, a dialog will be shown when load protected plug-ins are being loaded. If false, load protected
  // plug-ins will silently not load
  bool AskOnLoadProtection() const;
  void SetAskOnLoadProtection( bool bAsk );

  void LoadRepeatCommandSettings(CRhinoProfileContext& context);

  ON_wString GetPackageManagerSources() const;
  void SetPackageManagerSources(const wchar_t* s);

  // Relative width of the status bar layer pane. The default has been 2 in the past,
  // and will be changed to 3 from V8 onwards
  int StatusBarLayerPaneWidth() const;
  void SetStatusBarLayerPaneWidth(int value);

  // If true, objects are highlighted with mouse over
  bool MouseOverHighlight() const;
  void SetMouseOverHighlight(bool bEnable);

  // If true, sub-objects are highlighted with mouse over
  bool MouseOverHighlightSubObjects() const;
  void SetMouseOverHighlightSubObjects(bool bEnable);

  bool SilhouetteHighlighting(void) const;
  void SetSilhouetteHighlighting(bool bEnable);

  bool ShowViewportXEdges(void) const;
  void SetShowViewportXEdges(bool);

  int  ViewportXEdgeThickness(void) const;
  void SetViewportXEdgeThickness(int);

  int  ViewportXEdgeStyle(int) const;
  void SetViewportXEdgeStyle();

  float ViewportXEdgeLength(void) const;
  void  SetViewportXEdgeLength(float);

  float ViewportXEdgeSensitivity(void) const;
  void  SetViewportXEdgeSensitivity(float);

  int  SilhouetteThickness(void) const;
  void SetSilhouetteThickness(int);

  int  SilhouetteMethod(void) const;
  void SetSilhouetteMethod(int);

  int  SilhouetteHighlightOuterScale(void) const;
  void SetSilhouetteHighlightOuterScale(int);

  float SilhouetteHighlightInnerFactor(void) const;
  void  SetSilhouetteHighlightInnerFactor();

  float SilhouetteHighlightInnerColorScale(void) const;
  void  SetSilhouetteHighlightInnerColorScale(float);

  bool ShowSelectedObjectCorners(void) const;
  void SetShowSelectedObjectCorners();

  int  SelectedObjectCornerThickness(void) const;
  void SetSelectedObjectCornerThickness(bool);

  float SelectedObjectCornerLength(void) const;
  void  SetSelectedObjectCornerLength(float);

  ON_Color SelectedObjectCornerColor(void) const;
  void     SetSelectedObjectCornerColor(ON_Color);

  bool HiddenLineEnabled(void) const;
  void SetHiddenLineEnabled(bool);

  bool ShowHiddenLines(void) const;
  void SetShowHiddenLines(bool);

  int  HiddenLineMode(void) const;
  void SetHiddenLineMode(int);

  ON_Color HiddenLineColor(void) const;
  void     SetHiddenLineColor(ON_Color);

  float HiddenLineColorMixingFactor(void) const;
  void  SetHiddenLineColorMixingFactor(float);

  float HiddenLineThicknessScale(void) const;
  void  SetHiddenLineThicknessScale(float);

  bool OrthoUseZ() const;
  void SetOrthoUseZ(bool bUse);

  bool ShowOrthoHashMarks() const;
  void SetShowOrthoHashMarks(bool bShow);

  bool DisableRightClickAsEnter() const;
  void SetDisableRightClickAsEnter(bool);

  bool ControlClickSubObjectDeselect() const;
  void SetControlClickSubObjectDeselect(bool bEnable);

  bool NudgeAutoUpdateMesh() const;
  void SetNudgeAutoUpdateMesh(bool bEnable);

  UINT NudgeAutoUpdateDelay() const;
  void SetNudgeAutoUpdateDelay(UINT delay);

  bool PickOccluded() const;
  void SetPickOccluded(bool bEnable);

  bool NumericKeypadDecimalKeyPeriod() const;
  void SetNumericKeypadDecimalKeyPeriod(bool bEnable);

  bool TransparentOccluders() const;
  void SetTransparentOccluders(bool bEnable);

  bool DominantOrtho() const;
  void SetDominantOrtho(bool bEnable);

  // For internal use only
  class CRhAppSettingsData* Private() const;

private:
  friend class CRhinoApp;
  friend class CRhLinuxApp;
  class CRhAppSettingsData* m_private = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
#pragma endregion class CRhinoAppSettings
//
///////////////////////////////////////////////////////////////////////////////
