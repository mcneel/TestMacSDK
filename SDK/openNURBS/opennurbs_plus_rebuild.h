#pragma once
//
// Copyright (c) 1993-2025 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_PLUS) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if defined(ON_OS_WINDOWS)

/// <summary>
/// A "kink" in a curve is a unit tangent discontinuity or a vector curvature discontinuity.
/// This class is used to determine which types and magnitudes of 
/// unit tangent discontinuities and vector curvature discontinuities
/// should are a "kink." It also provides functions for determining
/// if there is a kink at a specific curve parameter.
/// </summary>
class ON_WIP_CLASS ON_CurveKinkDefinition
{
public:
  ON_CurveKinkDefinition() = default;
  ~ON_CurveKinkDefinition() = default;
  ON_CurveKinkDefinition(const ON_CurveKinkDefinition&) = default;
  ON_CurveKinkDefinition& operator=(const ON_CurveKinkDefinition&) = default;

  /// <summary>
  /// Create a ON_CurveKinkDefinition with specified settings.
  /// </summary>
  /// <param name="polyline_tangent_kink_angle_degrees">
  /// 0 &lt;= polyline_tangent_kink_angle_degrees &lt;= 180.
  /// Values &gt; 180 are treated as 180 and 
  /// other values outside the [0,180] are treated as ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleDegrees.
  /// If a curve is a polyline and the angle (in degrees) between the tangents 
  /// at a tangent discontinuity is &gt; polyline_tangent_kink_angle_degrees, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing 180.0 disables tangent discontinuity checks for polyline curves.
  /// </param>
  /// <param name="curve_tangent_kink_angle_degrees">
  /// 0 &lt;= curve_tangent_kink_angle_degrees &lt;= 180.
  /// Values &gt; 180 are treated as 180 and 
  /// other values outside the [0,180] are treated as ON_CurveKinkDefinition::DefaultTangentKinkAngleDegrees.
  /// If a curve is not a polyline and the angle (in degrees) between the tangents 
  /// at a tangent discontinuity is &gt; curve_tangent_kink_angle_degrees, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing 180.0 disables tangent discontinuity checks for curves that are not polylines.
  /// </param>
  /// <param name="curvature_kink_angle_degrees">
  /// 0 &lt;= curvature_kink_angle_degrees &lt;= 180.
  /// Values &gt; 180 are treated as 180 and 
  /// other values outside the [0,180] are treated as ON_CurveKinkDefinition::DefaultCurvatureKinkAngleDegrees.
  /// If the angle (in degrees) between curvature vector directions
  /// at a curvature discontinuity is &gt; curvature_kink_angle_degrees, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing 180.0 disables curvature vector direction discontinuity checks.
  /// </param>
  /// <param name="curvature_kink_radius_ratio">
  /// 0 &lt;= curvature_kink_radius_ratio &lt;= 1.
  /// Values &gt; 1 are treated as 1 and 
  /// other values outside the [0,1] are treated as ON_CurveKinkDefinition::DefaultCurvatureKinkRadiusRatio.
  /// If the ratio (minimum radius of curvature)/(maximum radius of curvature) 
  /// at a curvature discontinuity is &lt; curvature_kink_radius_ratio, 
  /// then that discontinuity is treated as a kink. 
  /// In particular, passing 0.0 disables curvature radius discontinuity checks.
  /// </param>
  /// <param name="bKinkAtTangentChange">
  /// true if tangent discontinuities should be tested to determine if the tangent discontinuity is a kink.
  /// false to ingnore tangent discontinuities.
  /// </param>
  /// <param name="bKinkAtCurvatureChange">
  /// true if curvature discontinuities should be tested to determine if the curvature discontinuity is a kink.
  /// false to ingnore curvature discontinuities.
  /// </param>
  ON_CurveKinkDefinition(
    double polyline_tangent_kink_angle_degrees,
    double curve_tangent_kink_angle_degrees,
    double curvature_kink_angle_degrees,
    double curvature_kink_radius_ratio,
    bool bKinkAtTangentChange,
    bool bKinkAtCurvatureChange
  );

  static constexpr double DefaultTangentKinkAngleDegrees = 1.0;

  static constexpr double DefaultTangentKinkAngleRadians = 1.0 * ON_DEGREES_TO_RADIANS;

  static constexpr double DefaultPolylineTangentKinkAngleDegrees = 5.0;

  static constexpr double DefaultPolylineTangentKinkAngleRadians = 5.0 * ON_DEGREES_TO_RADIANS;

  static constexpr double DefaultCurvatureKinkAngleDegrees = 5.0;

  static constexpr double DefaultCurvatureKinkAngleRadians = 5.0 * ON_DEGREES_TO_RADIANS;

  static constexpr double DefaultCurvatureKinkRadiusRatio = 0.75;

  static const ON_CurveKinkDefinition Unset;

  /// <summary>
  /// DefaultTangentKink can be used to detect typical tangent discontinuties.
  /// The angles used for testing tangent discontinuities are 
  /// DefaultPolylineTangentKinkAngleDegrees for polylines and 
  /// DefaultTangentKinkAngleDegrees for all other curves.
  /// </summary>
  static const ON_CurveKinkDefinition DefaultTangentKink;

  /// <summary>
  /// DefaultCurvatureKink is used to detect typical unit tangent discontinuties and 
  /// typical vector curvature discontinuties.
  /// The angles used for testing tangent discontinuities are 
  /// DefaultPolylineTangentKinkAngleDegrees for polylines and 
  /// DefaultTangentKinkAngleDegrees for all other curves.
  /// The angle used for testing curvature vector direction discontinuties is DefaultCurvatureKinkAngleDegrees.
  /// The ratio used for testing curvature radius discontinuities is DefaultCurvatureKinkRadiusRatio.
  /// </summary>
  static const ON_CurveKinkDefinition DefaultCurvatureKink;

  /// <summary>
  /// Returns a hash of the settings used to determine if a discontinuity is a kink.
  /// This hash is useful for detecting changes in kink settings.
  /// If the hashes are equal, then the same kinks will be found.
  /// If the hashes are not equal, then curves exist where the different values
  /// of ON_CurveKinkDefinition will find different sets of kinks in the same curve.
  /// </summary>
  /// <returns>
  /// If this is set, a hash of the settings used to find a kink is returned.
  /// If this is unset, then ON_SHA1_Hash::EmptyContentHash is returned.
  /// </returns>
  const ON_SHA1_Hash Hash() const;

  /// <returns>
  /// Returns true if tangent discontinuities should be tested when finding kinks.
  /// The angle used for testing polylines is TangentKinkAngleDegrees(true).
  /// The angle used for testing curves that are not polylines is TangentKinkAngleDegrees(false).
  /// </returns>
  bool KinkAtTangentChange() const;

  /// <param name="bKinkAtTangentChange">
  /// If true, finding kinks at tangent discontinuities is enabled. 
  /// If the kink angles are not set, then 
  /// ON_CurveKinkDefinition::DefaultCurveTangentKinkAngleDegrees and 
  /// ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleDegrees are used.
  /// If false, then tangent discontinuities are ignored when finding kinks.
  /// The current values of TangentKinkAngleDegrees(true) and TangentKinkAngleDegrees(false) are not changed.
  /// </param>
  void SetKinkAtTangentChange(bool bKinkAtTangentChange);

  /// <summary>
  /// ClearKinkAtTangentChange() disables finding kinks at tangent discontinuities.
  /// The current values of TangentKinkAngleDegrees(true) and TangentKinkAngleDegrees(false) are not changed.
  /// </summary>
  void ClearKinkAtTangentChange();

  /// <returns>
  /// Returns true if curvature discontinuities should be tested when finding kinks.
  /// The angle used vector curvature direction discontinuities is CurvatureKinkAngleDegrees().
  /// The ratio used fo radius of curvature discontinuities is CurvatureKinkRadiusRatio().
  /// </returns>
  bool KinkAtCurvatureChange() const;

  /// <param name="bKinkAtTangentChange">
  /// If true, finding kinks at curvature discontinuities is enabled. 
  /// If the curvature parameters are not set, then 
  /// ON_CurveKinkDefinition::DefaultCurvatureKinkAngleDegrees and 
  /// ON_CurveKinkDefinition::DefaultCurvatureKinkRadiusRatio are used.
  /// If false, then curvature discontinuities are ignored when finding kinks.
  /// The current values of CurvatureKinkAngleDegrees() and CurvatureKinkRadiusRatio() are not changed.
  /// </param>
  void SetKinkAtCurvatureChange(bool bKinkAtCurvatureChange);

  /// <summary>
  /// ClearKinkAtCurvatureChange() disables finding kinks at curvature discontinuities.
  /// The current values of CurvatureKinkAngleDegrees() and CurvatureKinkRadiusRatio() are not changed.
  /// </summary>
  void ClearKinkAtCurvatureChange();

  /// <summary>
  /// Clear() disables finding kinks.
  /// The current values of TangentKinkAngleDegrees(true), TangentKinkAngleDegrees(false), 
  /// CurvatureKinkAngleDegrees() and CurvatureKinkRadiusRatio() are not changed.
  /// </summary>
  void Clear();

  /// <returns>
  /// True if testing tangent or curvature discontinutities is enabled.
  /// False if testing both tangent and curvature discontinuities is disabled.
  /// </returns>
  bool IsSet() const;

  /// <returns>
  /// False if testing tangent or curvature discontinutities is enabled.
  /// True if testing both tangent and curvature discontinuities is disabled.
  /// </returns>
  bool IsUnset() const;
    
  /// <returns>
  /// True if there is a kink at curve(t).
  /// </returns>
  bool IsKink(const ON_Curve& curve, double t) const;

  /// <returns>
  /// True if there is a tangent discontinuity at curve(t) and the agnle between the two tangents
  /// is &gt; TangentKinkAngleDegrees(curve.IsPolyline()).
  /// </returns>
  bool IsTangentKink(const ON_Curve& curve, double t) const;

  /// <returns>
  /// True if the angle between tangent_from_below and tangent_from_above is &gt; TangentKinkAngleDegrees(bCurveIsPolyline),
  /// </returns>
  bool IsTangentKink(ON_3dVector tangent_from_below, ON_3dVector tangent_from_above, bool bCurveIsPolyline) const;

  /// <returns>
  /// True if there is a curvature discontinuity at curve(t) that passes the curvature kink test.
  /// </returns>
  bool IsCurvatureKink(const ON_Curve& curve, double t) const;

  /// <returns>
  /// True if the angle between curvature_from_below and curvature_from_above is &gt; CurvatureKinkAngleDegrees()
  /// or the ratio min/max &lt; CurvatureKinkRadiusRatio(), where
  /// min and max are the minimum and maximum lengths of curvature_from_below and curvature_from_above.
  /// </returns>
  bool IsCurvatureKink(ON_3dVector curvature_from_below, ON_3dVector curvature_from_above) const;

  /// <summary>
  /// The angle, in degrees,  used to determine if a tangent discontinuity in a curve is a kink.
  /// Note that tangent kinks are only reported when KinkAtTangentChange() is true.
  /// If the tangent kink angles have not be explicitly set, then the values
  /// ON_CurveKinkDefinition::DefaultTangentKinkAngleDegrees and
  /// ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleDegrees are used.
  /// </summary>
  /// <param name="bDefaultToCurveAngle">
  /// If ture curve in quetion is a polyline and you want to use different angle definitions for tangent kinks in polylines,
  /// then pass bCurveIsPolyline = true.
  /// </param>
  /// <returns>
  /// The tangent kink angle in degrees. 
  /// </returns>
  double TangentKinkAngleDegrees(bool bCurveIsPolyline) const;

  /// <summary>
  /// The angle, in radians,  used to determine if a tangent discontinuity in a curve is a kink.
  /// Note that tangent kinks are only reported when KinkAtTangentChange() is true.
  /// If the tangent kink angles have not be explicitly set, then the values
  /// ON_CurveKinkDefinition::DefaultTangentKinkAngleRadians and
  /// ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleRadians are used.
  /// </summary>
  /// <param name="bDefaultToCurveAngle">
  /// If ture curve in quetion is a polyline and you want to use different angle definitions for tangent kinks in polylines,
  /// then pass bCurveIsPolyline = true.
  /// </param>
  /// <returns>
  /// The tangent kink angle in radians. 
  /// </returns>
  double TangentKinkAngleRadians(bool bCurveIsPolyline) const;

  /// <summary>
  /// Set the angles used to determine if a tangent discontinuity should be a kink.
  /// </summary>
  /// <param name="polyline_kink_angle_degrees">
  /// 0 &lt;= polyline_kink_angle_degrees &lt;= 180.
  /// Values &gt; 180 are treated as 180 and 
  /// other values outside the [0,180] are treated as ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleDegrees.
  /// If a curve is a polyline and the angle (in degrees) between the tangents 
  /// at a tangent discontinuity is &gt; polyline_kink_angle_degrees, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing 180.0 disables tangent discontinuity checks for polyline curves.
  /// </param>
  /// <param name="curve_kink_angle_degrees">
  /// 0 &lt;= curve_kink_angle_degrees &lt;= 180.
  /// Values &gt; 180 are treated as 180 and 
  /// other values outside the [0,180] are treated as ON_CurveKinkDefinition::DefaultTangentKinkAngleDegrees.
  /// If a curve is not a polyline and the angle (in degrees) between the tangents 
  /// at a tangent discontinuity is &gt; curve_kink_angle_degrees, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing 180.0 disables tangent discontinuity checks for curves that are not polylines.
  /// </param>
  void SetTangentKinkDefinitionDegrees(double polyline_kink_angle_degrees, double curve_kink_angle_degrees);

  /// <summary>
  /// Set the angles used to determine if a tangent discontinuity should be a kink.
  /// </summary>
  /// <param name="polyline_kink_angle_radians">
  /// 0 &lt;= polyline_kink_angle_radians &lt;= ON_PI.
  /// Values &gt; ON_PI are treated as ON_PI and 
  /// other values outside the [0,ON_PI] are treated as ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleRadians.
  /// If a curve is a polyline and the angle (in radians) between the tangents 
  /// at a tangent discontinuity is &gt; polyline_kink_angle_radians, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing ON_PI disables tangent discontinuity checks for polyline curves.
  /// </param>
  /// <param name="curve_kink_angle_radians">
  /// 0 &lt;= curve_kink_angle_radians &lt;= ON_PI.
  /// Values &gt; ON_PI are treated as ON_PI and 
  /// other values outside the [0,ON_PI] are treated as ON_CurveKinkDefinition::DefaultTangentKinkAngleRadians.
  /// If a curve is not a polyline and the angle (in degrees) between the tangents 
  /// at a tangent discontinuity is &gt; curve_kink_angle_radians, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing ON_PI.0 disables tangent discontinuity checks for curves that are not polylines.
  /// </param>
  void SetTangentKinkDefinitionRadians(double polyline_kink_angle_radians, double curve_kink_angle_radians);

  /// <summary>
  /// Clears any customized the curve and polyline tangent kink angles.
  /// ON_CurveKinkDefinition::DefaultPolylineTangentKinkAngleDegrees and
  /// ON_CurveKinkDefinition::DefaultTangentKinkAngleDegrees will be used.
  /// </summary>
  void ClearTangentKinkAngle();

  /// <summary>
  /// If the angle (in degrees) between curvature vector directions
  /// at a curvature discontinuity is &gt; CurvatureKinkAngleDegrees(), 
  /// then that curvature discontinuity is treated as a kink.
  /// In particular, if CurvatureKinkAngleDegrees() = 180.0, then curvature vector direction discontinuity checks are disabled.
  /// </summary>
  /// <returns>
  /// The angle used to test for curvature vector direction kinks.
  /// </returns>
  double CurvatureKinkAngleDegrees() const;

  /// <summary>
  /// If the angle (in radians) between curvature vector directions
  /// at a curvature discontinuity is &gt; CurvatureKinkAngleRadians(), 
  /// then that curvature discontinuity is treated as a kink.
  /// In particular, if CurvatureKinkAngleRadians() = ON_PI, then curvature vector direction discontinuity checks are disabled.
  /// </summary>
  /// <returns>
  /// The angle used to test for curvature vector direction kinks.
  /// </returns>
  double CurvatureKinkAngleRadians() const;

  /// <summary>
  /// If the ratio (minimum radius of curvature)/(maximum radius of curvature) 
  /// at a curvature discontinuity is &lt; CurvatureKinkRadiusRatio(), 
  /// then that curvature discontinuity is treated as a kink. 
  /// In particular, if CurvatureKinkRadiusRatio() = 0, then curvature radius discontinuity checks are disabled.
  /// </summary>
  /// <returns>
  /// The angle used to test for curvature vector direction kinks.
  /// </returns>
  double CurvatureKinkRadiusRatio() const;

  /// <param name="curvature_kink_angle_degrees">
  /// 0 &lt;= curvature_kink_angle_degrees &lt;= 180.
  /// Values &gt; 180 are treated as 180 and 
  /// other values outside the [0,180] are treated as ON_CurveKinkDefinition::DefaultCurvatureKinkAngleDegrees.
  /// If the angle (in degrees) between curvature vector directions
  /// at a curvature discontinuity is &gt; curvature_kink_angle_degrees, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing 180.0 disables curvature vector direction discontinuity checks.
  /// </param>
  /// <param name="curvature_kink_radius_ratio">
  /// 0 &lt;= curvature_kink_radius_ratio &lt;= 1.
  /// Values &gt; 1 are treated as 1 and 
  /// other values outside the [0,1] are treated as ON_CurveKinkDefinition::DefaultCurvatureKinkRadiusRatio.
  /// If the ratio (minimum radius of curvature)/(maximum radius of curvature) 
  /// at a curvature discontinuity is &lt; curvature_kink_radius_ratio, 
  /// then that discontinuity is treated as a kink. 
  /// In particular, passing 0.0 disables curvature radius discontinuity checks.
  /// </param>
  void SetCurvatureKinkDefinitionDegrees(
    double curvature_kink_angle_degrees,
    double curvature_kink_radius_ratio
  );

  /// <param name="curvature_kink_angle_radians">
  /// 0 &lt;= curvature_kink_angle_radians &lt;= ON_PI.
  /// Values &gt; ON_PI are treated as ON_PI and 
  /// other values outside the [0,ON_PI] are treated as ON_CurveKinkDefinition::DefaultCurvatureKinkAngleRadians.
  /// If the angle (in radians) between curvature vector directions
  /// at a curvature discontinuity is &gt; curvature_kink_angle_radians, 
  /// then that discontinuity is treated as a kink.
  /// In particular, passing ON_PI disables curvature vector direction discontinuity checks.
  /// </param>
  /// <param name="curvature_kink_radius_ratio">
  /// 0 &lt;= curvature_kink_radius_ratio &lt;= 1.
  /// Values &gt; 1 are treated as 1 and 
  /// other values outside the [0,1] are treated as ON_CurveKinkDefinition::DefaultCurvatureKinkRadiusRatio.
  /// If the ratio (minimum radius of curvature)/(maximum radius of curvature) 
  /// at a curvature discontinuity is &lt; curvature_kink_radius_ratio, 
  /// then that discontinuity is treated as a kink. 
  /// In particular, passing 0.0 disables curvature radius discontinuity checks.
  /// </param>
  void SetCurvatureKinkDefinitionRadians(
    double curvature_kink_angle_radians,
    double curvature_kink_radius_ratio
  );

  /// <summary>
  /// Clears any customized the curature kink settings.
  /// ON_CurveKinkDefinition::DefaultCurvatureKinkAngleDegrees and
  /// ON_CurveKinkDefinition::DefaultCurvatureKinkRadiusRatio will be used.
  /// </summary>
  void ClearCurvatureKinkDefinition();

private:

  // Optional custom angle to use instead of DefaultTangentKinkAngleDegrees.
  double m_tangent_kink_angle_degrees = ON_DBL_QNAN;

  // Optional custom angle to use instead of DefaultPolylineTangentKinkAngleDegrees.
  double m_polyline_tangent_kink_angle_degrees = ON_DBL_QNAN;

  /// <summary>
  /// If the angle between the curvature vectors is &gt; m_curvature_kink_angle_degrees,
  /// then the curvature change is a curvature kink.
  /// </summary>
  double m_curvature_kink_angle_degrees = ON_DBL_QNAN;

  /// <summary>
  /// Optional custom ratiou to use instead of DefaultCurvatureKinkRadiusRatio.
  /// If min(radius of curvature)/max(radius of curvature) &lt; m_curvature_kink_radius_ratio,
  /// then the curvature change is a curvature kink.
  /// </summary>
  double m_curvature_kink_radius_ratio = ON_DBL_QNAN;

  /// <summary>
  /// If false, tangent discontinuities cannot be kinks.
  /// If true, tangent discontinuities are tested to determine if they are kinks. 
  /// </summary>
  bool m_bKinkAtTangentChange = false;

  /// <summary>
  /// If false, curvature discontinuities cannot be kinks.
  /// If true, curvature discontinuities are tested to determine if they are kinks. 
  /// </summary>
  bool m_bKinkAtCurvatureChange = false;

  ON__UINT16 m_reserved2 = 0;
  ON__UINT32 m_reserved3 = 0;
  ON__UINT64 m_reserved4 = 0;
};

class ON_WIP_CLASS ON_RebuildCurveOptions
{
public:
  ON_RebuildCurveOptions() = default;
  ~ON_RebuildCurveOptions() = default;
  ON_RebuildCurveOptions(const ON_RebuildCurveOptions&) = default;
  ON_RebuildCurveOptions& operator=(const ON_RebuildCurveOptions&) = default;

public:
  enum : int
  {
    MinimumDegree = 1,
    MaximumDegree = 11,
    DefaultDegree = 3,

    MinimumClampedPointCount = 2,
    MinimumPeriodicPointCount = 3,
    MaximumPointCount = 3000,

    DefaultSampleCount = ON_PointPairing::DefaultPointCount,

    /// <summary>
    /// Minimum number of points to sample from input curves and
    /// pass to the fitter that creates the output NURBS curve.
    /// </summary>
    MinimumSampleCount = (ON_RebuildCurveOptions::DefaultSampleCount / 10),

    /// <summary>
    /// Maximum number of points to sample from input curves and
    /// pass to the fitter that creates the output NURBS curve.
    /// </summary>
    MaximumSampleCount = (10 * ON_RebuildCurveOptions::DefaultSampleCount),
  };

  /// <summary>
  /// Give a degree and periodic setting, determine the minimum point count.
  /// </summary>
  /// <param name="degree">
  /// Degree of output NURBS curve.
  /// </param>
  /// <param name="bPeriodic">
  /// True if the output NURBS curve will be periodic.
  /// </param>
  /// <returns>
  /// Minimum permitted point count.
  /// </returns>
  static int MinimumPointCountForDegree(int degree, bool bPeriodic);

  /// <summary>
  /// Given a point count and periodic setting, determine the maximum degree.
  /// </summary>
  /// <param name="point_count">
  /// Number of indpendent control points in the output NURBS curve.
  /// </param>
  /// <param name="bPeriodic">
  /// True if the output NURBS curve will be periodic.
  /// </param>
  /// <returns>
  /// Maximum permitted degree.
  /// </returns>
  static int MaximumDegreeForPointCount(int point_count, bool bPeriodic);

  static bool ValidInput(
    size_t sample_point_count,
    unsigned degree,
    unsigned control_point_count,
    bool bPeriodic,
    ON_Interval curve_domain
  );

  /// <summary>
  /// Returns a hash of the options that control the output curve geometry.
  /// These options are:
  /// degree (degree of the output curves),
  /// point_count (number of user control points in the output curves),
  /// sample_point_count (number of points sampled from the input curves).
  /// smoothing
  /// When bShowTangentOptions is true, end tangent matching settings are hashed.
  /// When bShowSplitAtKinksOption is true, kink parameters are hashed.
  /// This hash is useful for determing when previously calculated output curves
  /// need to be updated by comparing the hash current options and the hash
  /// of options used to calculate the output curves.
  /// </summary>
  /// <param name="bHashTangentOptions">
  /// If the set of input curves contain at least one that is open,
  /// then pass true to include end tangent matching settings in the hash.
  /// </param>
  /// <param name="bHashSplitAtKinksOption">
  /// If the set of input curves have kinks, then pass true to include 
  /// kink splitting settings in the hash.
  /// </param>
  /// <returns>
  /// A hash of the options that control the output curve geometry.
  /// </returns>
  const ON_SHA1_Hash Hash(
    bool bHashTangentOptions,
    bool bHashSplitAtKinksOption
  ) const;

  /// <returns>
  /// True if every curve created with these options will be periodic.
  /// </returns>
  bool Periodic() const;

  /// <summary>
  /// Generally, it is best to call SetPeriodic() once before setting
  /// any other options and to leave it unchanged while the other options
  /// are adjusted. This is because the relationship betweem point count and degree
  /// and the options that are used when rebuilding a curve are different
  /// for periodic and non-periodic curves.
  /// If every curve created with these options will be periodic, 
  /// then call SetPeriodic(true). Otherwise call SetPeriodic(false).
  /// </summary>
  /// <param name="bPeriodic">
  /// Pass true if every curve created with these options will be periodic.
  /// Otherwise, pass false.
  /// </param>
  void SetPeriodic(bool bPeriodic);


  int PointCount() const;

  /// <summary>
  /// Sets the point count and, if necessary, reduces the degree so that degree that 
  /// degree &lt;= ON_RebuildCurveOptions::MaximumDegree(point_count, PeriodicOutput()).
  /// </summary>
  /// <param name="point_count"></param>
  void SetPointCount(int point_count);

  int Degree() const;

  void SetDegree(int degree);

  int ClampedControlPointCount() const;

  int PeriodicControlPointCount() const;

  bool OptimizeCurve() const;

  void SetOptimizeCurve(bool bOptimizeCurve);

  bool MatchStartTangent() const;

  void SetMatchStartTangent(bool bMatchStartTangent);

  bool MatchEndTangent() const;

  void SetMatchEndTangent(bool bMatchEndTangent);

  void SetSplitAtKinks(
    bool bSplitAtKinks
  );

  bool SplitAtKinks() const;

  void SetKinkDefinition(ON_CurveKinkDefinition kink_definition);

  const ON_CurveKinkDefinition KinkDefinition() const;

  /// <summary>
  /// The Penalty enum is used to select a predefined or a custom coefficient
  /// for the smoothing and variance penalties.
  /// </summary>
  enum class Penalty : unsigned char
  {
    None = 0,
    Low = 1,
    Medium = 2,
    High = 3,
    Custom = 4
  };

  /// <summary>
  /// Smoothing coefficient that corresponds to ON_RebuildCurveOptions::Penalty::Low.
  /// </summary>
  static constexpr double LowSmoothingCoefficient = 0.01;

  /// <summary>
  /// Smoothing coefficient that corresponds to ON_RebuildCurveOptions::Penalty::Medium.
  /// </summary>
  static constexpr double MediumSmoothingCoefficient = 0.1;

  /// <summary>
  /// Smoothing coefficient that corresponds to ON_RebuildCurveOptions::Penalty::High.
  /// </summary>
  static constexpr double HighSmoothingCoefficient = 1.0;

  /// <summary>
  /// Maximum permitted smoothing coefficient.
  /// </summary>
  static constexpr double MaximumSmoothingCoefficient = 10.0;

  /// <summary>
  /// There are 5 types of smoothing: None, Low, Medium, High, and Custom.
  /// Use SmoothingCoefficient() to get the numerical coefficient used in 
  /// the optimization. Use SetSmoothingCoefficient() to set a custom smoothing
  /// coefficient.
  /// </summary>
  /// <returns>
  /// The type of smoothing (None, Low, Medium, High, Custom).
  /// </returns>
  ON_RebuildCurveOptions::Penalty SmoothingPenalty() const;

  /// <summary>
  /// Use one of the one of ON_RebuildCurveOptions::Penalty enum values
  /// to specify a smoothing coefficient. This method is provided to 
  /// make it easier to provide an interface that lets a user
  /// select reasonable values for the smoothing coefficient without
  /// having to understand the optimization calculation.
  /// </summary>
  /// <param name="smoothing">
  /// One of ON_RebuildCurveOptions::Penalty enum values.
  /// If ON_RebuildCurveOptions::Penalty::Custom is passed in
  /// and a preexisting custom smoothing penalty has not been specified
  /// using SetSmoothingCoefficient() then SmothingPenalty() is set to none.
  /// If you want to set a custom smoothing coefficient, use SetSmoothingCoefficient(x).
  /// </param>
  void SetSmoothingPenalty(ON_RebuildCurveOptions::Penalty smoothing_penalty);

  /// <summary>
  /// Specify a smoothing coefficient.
  /// </summary>
  /// <param name="smoothing_coefficient">
  /// Valid values are in the range 0 &lt;= smoothing_coefficient &lt;= ON_RebuildCurveOptions::MaximumCustomSmoothingCoefficient.
  /// If smoothing_coefficient is not valid, then smoothing is set to ON_RebuildCurveOptions::Smoothing::None.
  /// </param>
  void SetSmoothingCoefficient(double smoothing_coefficient);

  /// <returns>
  /// The numerical coefficient used in the optimization.
  /// 0 indicates no smoothing. 
  /// Values &gt; 0 and &lt;= ON_RebuildCurveOptions::MaximumCustomSmoothingCoefficient
  /// indicate smoothing will be applied.
  /// NaN indicates invalid settings and no smoothing will be applied.
  /// Note that smoothing only applies when ther are 3 or more control points
  /// in the rebuilt curve.
  /// </returns>
  double SmoothingCoefficient() const;


  /// <summary<coefficient>
  /// Variance coefficient that corresponds to ON_RebuildCurveOptions::Penalty::Low.
  /// </summary>
  static constexpr double LowVarianceCoefficient = 0.01;

  /// <summary>
  /// Variance coefficient that corresponds to ON_RebuildCurveOptions::Penalty::Medium.
  /// </summary>
  static constexpr double MediumVarianceCoefficient = 0.1;

  /// <summary>
  /// Variance coefficient that corresponds to ON_RebuildCurveOptions::Penalty::High.
  /// </summary>
  static constexpr double HighVarianceCoefficient = 1.0;

  /// <summary>
  /// Maximum permitted variance coefficient.
  /// </summary>
  static constexpr double MaximumVarianceCoefficient = 10.0;

  /// <summary>
  /// There are 5 types of variance: None, Low, Medium, High, and Custom.
  /// Use VarianceCoefficient() to get the numerical coefficient used in 
  /// the optimization. Use SetVarianceCoefficient() to set a custom variance
  /// coefficient.
  /// </summary>
  /// <returns>
  /// The type of variance (None, Low, Medium, High, Custom).
  /// </returns>
  ON_RebuildCurveOptions::Penalty VariancePenalty() const;

  /// <summary>
  /// Use one of the one of ON_RebuildCurveOptions::Penalty enum values
  /// to specify a variance coefficient. This method is provided to 
  /// make it easier to provide an interface that lets a user
  /// select reasonable values for the variance coefficient without
  /// having to understand the optimization calculation.
  /// </summary>
  /// <param name="variance">
  /// One of ON_RebuildCurveOptions::Penalty enum values.
  /// If ON_RebuildCurveOptions::Penalty::Custom is passed in
  /// and a preexisting custom variance penalty has not been specified
  /// using SetVarianceCoefficient() then SmothingPenalty() is set to none.
  /// If you want to set a custom variance coefficient, use SetVarianceCoefficient(x).
  /// </param>
  void SetVariancePenalty(ON_RebuildCurveOptions::Penalty variance_penalty);

  /// <summary>
  /// Specify a variance coefficient.
  /// </summary>
  /// <param name="variance_coefficient">
  /// Valid values are in the range 0 &lt;= variance_coefficient &lt;= ON_RebuildCurveOptions::MaximumCustomVarianceCoefficient.
  /// If variance_coefficient is not valid, then variance is set to ON_RebuildCurveOptions::Variance::None.
  /// </param>
  void SetVarianceCoefficient(double variance_coefficient);

  /// <returns>
  /// The numerical coefficient used in the optimization.
  /// 0 indicates no variance. 
  /// Values &gt; 0 and &lt;= ON_RebuildCurveOptions::MaximumCustomVarianceCoefficient
  /// indicate variance will be applied.
  /// NaN indicates invalid settings and no variance will be applied.
  /// Note that variance only applies when ther are 3 or more control points
  /// in the rebuilt curve.
  /// </returns>
  double VarianceCoefficient() const;


  int SampleCount() const;

  void SetSampleCount(int sample_count);


private:
  // Degree of the rebuild curve
  int m_degree = ON_RebuildCurveOptions::DefaultDegree;

  // Number of indpendent control points in the rebuilt curve.
  int m_point_count = (ON_RebuildCurveOptions::DefaultDegree + 1);

  // This is the number of points sampled from the input curves.
  int m_sample_count = ON_RebuildCurveOptions::DefaultSampleCount;

  /// <summary>
  /// True if the rebuilt curve will be periodic.
  /// </summary>
  bool m_bPeriodic = false;

  // If m_bOptimizeCurve = false, then all options below are ignored
  // and the Greville interpolant is returned. 
  // Rhino 1 - Rhino 8 Rebuild command created a result similar to what's
  // created when m_bOptimizeCurve = false.
  bool m_bOptimizeCurve = true;

  /// <summary>
  /// True if the start tangent of non-periodic rebuilt curves will be specified.
  /// </summary>
  bool m_bMatchStartTangent = false;

  /// <summary>
  /// True if the end tangent of non-periodic rebuilt curves will be specified.
  /// </summary>
  bool m_bMatchEndTangent = false;

  bool m_bSplitAtKinks = false;

  /// <summary>
  /// Smoothing penalty used to make triples of consecutive control polygon points colinear.
  /// Use SmoothingCoefficient() to convert this enum to the the value passed to the optimizer.
  /// </summary>
  ON_RebuildCurveOptions::Penalty m_smoothing_penalty = ON_RebuildCurveOptions::Penalty::None;

  /// <summary>
  /// Variance penalty used to make the distance between control points more equal.
  /// Use VarianceCoefficient() to convert this enum to the the value passed to the optimizer.
  /// </summary>
  ON_RebuildCurveOptions::Penalty m_variance_penalty = ON_RebuildCurveOptions::Penalty::None;

  ON__UINT8 m_reserved1 = 0;

  /// <summary>
  /// When m_smoothing_penalty = ON_RebuildCurveOptions::Penalty::Custom,
  /// SmoothingCoefficient() returns m_custom_smoothing_coefficient.
  /// </summary>
  double m_custom_smoothing_coefficient = ON_DBL_QNAN;

  /// <summary>
  /// When m_variance_penalty = ON_RebuildCurveOptions::Penalty::Custom,
  /// VarianceCoefficient() returns m_custom_variance_coefficient.
  /// </summary>
  double m_custom_variance_coefficient = ON_DBL_QNAN;

  double m_reserved2 = 0.0;

  ON_CurveKinkDefinition m_kink_definition = ON_CurveKinkDefinition::DefaultTangentKink;
};

class ON_WIP_CLASS ON_CurveFitToPointsSummary
{
public:
  enum class Results : unsigned char
  {
    Unset = 0,
    SuccessfulFit = 1,
    InvalidInput = 2
  };

  static const ON_CurveFitToPointsSummary Unset;
  static const ON_CurveFitToPointsSummary InvalidInput;

public:
  ON_CurveFitToPointsSummary(
    const ON_CurveFitToPointsSummary::Results result,
    unsigned evaluaton_count,
    double initial_objectiv_value,
    double final_objective_value
  );

  ON_CurveFitToPointsSummary() = default;
  ~ON_CurveFitToPointsSummary() = default;
  ON_CurveFitToPointsSummary(const ON_CurveFitToPointsSummary&) = default;
  ON_CurveFitToPointsSummary& operator=(const ON_CurveFitToPointsSummary&) = default;

  ON_CurveFitToPointsSummary::Results Result = ON_CurveFitToPointsSummary::Results::Unset;
  unsigned EvaluatonCount = 0u;
  double InitialObjectiveValue = ON_DBL_QNAN;
  double FinalObjectiveValue = ON_DBL_QNAN;

  const ON_wString ToString() const;

  /// <summary>
  /// The Rhino document runtime serial number of is typically used for debugging purposes.
  /// The calculations performed in opennurbs do not use this information.
  /// </summary>
  /// <param name="rhino_doc_runtime_serial_number"></param>
  void SetRhinoDocSerialNumber(unsigned rhino_doc_runtime_serial_number);

  /// <summary>
  /// The Rhino document runtime serial number of is typically used for debugging purposes.
  /// The calculations performed in opennurbs do not use this information.
  /// </summary>
  /// <returns>
  /// The Rhino document runtime serial number.
  /// </returns>
  unsigned RhinoDocSerialNumber() const;

private:
  unsigned m_rhino_doc_runtime_serial_number = 0;
};

class ON_WIP_CLASS ON_CurveFitToPointsSegment
{
public:
  ON_CurveFitToPointsSegment() = default;
  ~ON_CurveFitToPointsSegment() = default;
  ON_CurveFitToPointsSegment(const ON_CurveFitToPointsSegment&) = default;
  ON_CurveFitToPointsSegment& operator=(const ON_CurveFitToPointsSegment&) = default;

public:
  bool IsSet(const ON_Interval domain, const unsigned degree) const;

  // domain of the segment
  ON_Interval m_domain = ON_Interval::Nan;

  /// <summary>
  /// 0.0 &lt= m_relative_length &lt; 1.0 is the relative length of the segment (or an estimate) with respect to the entire curve
  /// and is used to determine how many control points are assigned to the segment.
  /// </summary>
  double m_relative_length = 0.0;

  // degree of the segment
  unsigned m_degree = 0;

  // number of control points assigned to the segment.
  unsigned m_control_point_count = 0;
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_SimpleArray<ON_CurveFitToPointsSegment>;
#endif


class ON_WIP_CLASS ON_CurveFitToPointsBuilder
{
public:
  static const ON_SHA1_Hash NurbsPropertiesHash(
    int degree,
    int cv_count,
    bool bIsRational,
    bool bIsPeriodic,
    const double* knots
  );

  static const ON_SHA1_Hash NurbsPropertiesHash(const ON_NurbsCurve& nurbs_curve);

  static const ON_CurveFitToPointsBuilder Unset;

public:
  ON_CurveFitToPointsBuilder() = default;
  ~ON_CurveFitToPointsBuilder() = default;
  ON_CurveFitToPointsBuilder(const ON_CurveFitToPointsBuilder&) = default;
  ON_CurveFitToPointsBuilder& operator=(const ON_CurveFitToPointsBuilder&) = default;

public:
  bool InitializeFromObjectRef(ON_ObjRef oref, const ON_Curve* oref_curve);

  bool InitializeFromInputCurve(const ON_Curve* input_curve, ON_Interval domain, bool bCheckForKinks);

private:
  static const bool Internal_RebuiltCurveIsPeriodic(const ON_Curve* input_curve, const ON_Interval domain);

  static const ON_SHA1_Hash Internal_InputCurveNurbFormHash(const ON_Curve* input_curve, const ON_Interval domain, ON_BoundingBox* nurbs_curve_bbox);

public:

  void ClearKinkSegments();

  unsigned SetKinkSegmentsIntervals(const ON_CurveKinkDefinition kink_definition);

  void ClearKinkSegmentsPointCounts();

  unsigned SetKinkSegmentsPointCounts(unsigned degree, unsigned desired_point_count);

  bool RebuildCurve(
    const int degree, const unsigned clamped_cv_count, const unsigned periodic_cv_count,
    bool bOptimizeCurve,
    const unsigned target_point_count,
    bool bMatchStartTangent, bool bMatchEndTangent,
    double smoothing_coefficient,
    const ON_CurveKinkDefinition kink_definition,
    bool bUseNurbFormWhenPossible,
    unsigned debug_rhino_doc_sn
  );

  void ClearRebuiltCurve();

public:
  /// <summary>
  /// The Rhino object used to select the input curve. 
  /// This object can be a curve object or something like a 
  /// brep, subd, or extrusion that has the curve as a component.
  /// </summary>
  ON_ObjRef m_input_oref;

  const ON_Curve* m_input_curve = nullptr;

  /// <summary>
  /// Domain of the portion of input_curve to rebuild. This is always inlcuded in m_input_curve->Domain().
  /// The rebuilt curve will have this domain as well and that is critical to make Rhino history work
  /// as expected.
  /// </summary>
  ON_Interval m_domain = ON_Interval::Nan;

  /// <summary>
  /// Set to true if it is possible that some setting of the "SplitAtKinks" option
  /// would find a kink in the input curve.
  /// NOTE WELL: 
  /// This bool indicates that if the user sets the strictest definition of a "kink,"
  /// then the curve will have a kink. If the sure uses larger angle tolerances or doesn't
  /// care about curvature changes, then the kink that caused m_InputCurveConatainsKink
  /// to be true may not be severe enought to quality as a kink in the rebuilt curve.
  /// </summary>
  bool m_SplitAtKinksCandidate = false;

  /// <summary>
  /// True if the input curve is closed and the rebuilt curve will be periodic
  /// </summary>
  bool m_bPeriodicRebuiltCurve = false;

  /// <summary>
  /// The hash in m_input_nurb_form_hash is from HashRebuildProperties(). When HashRebuildProperties() has the same values
  /// for the NURBS form and for the rebuild options, then Rebuild can use the NURBS form instead of using
  /// optimization to calculate a rebuilt curve.
  /// </summary>
  ON_SHA1_Hash m_input_nurb_form_hash = ON_SHA1_Hash::EmptyContentHash;

  ON_BoundingBox m_input_curve_bbox = ON_BoundingBox::NanBoundingBox;


  /// <summary>
  /// When m_kink_segments[] is not empty, it is a contiguous list of segments
  /// that contain no kinks and are separated by a kink.
  /// 
  /// Use SetKinkSegmentsIntervals() to set 
  /// m_kink_segments[].m_domain and m_kink_segments[].m_relative_length segments.
  /// 
  /// Use SetKinkSegmentsPointCounts() to set m_kink_segments[].m_degree and m_kink_segments[].m_control_point_count.
  /// </summary>
  ON_SimpleArray<ON_CurveFitToPointsSegment> m_kink_segments;

  /// <summary>
  /// Use SetKinkSegmentsIntervals() to set 
  /// m_kink_segments[].m_domain and m_kink_segments[].m_relative_length segments.
  /// 
  /// m_kink_parameters_hash is a SHA1 has of the paramters used to calculate 
  /// the m_kink_segments[].m_domain and m_kink_segments[].m_relative_length
  /// values. If m_kinks_parameters_hash = ON_SHA1_Hash::EmptyContentHash, then
  /// kinks in the input curve are ignored and m_kink_segments[] should be empty. 
  /// Otherwise m_kink_segments[] is either empty (no kinks) or is 
  /// a contiguous array of segments that contain no kinks in their interiors.
  /// </summary>
  ON_SHA1_Hash m_kink_definition_hash = ON_SHA1_Hash::EmptyContentHash;

  /// <summary>
  /// The degree assigned to each kink segment.
  /// Use SetKinkSegmentsPointCounts() to set this value.
  /// </summary>
  unsigned m_kink_segments_degree = 0;

  /// <summary>
  /// The number of points that will be in a kinky elmo curve.
  /// Use SetKinkSegmentsPointCount() to set this value.
  /// </summary>
  unsigned m_kink_segments_point_count = 0;


  unsigned KinkSegmentsTotalPointCount() const;

  /// <summary>
  /// m_point_pairing.Points(A) are points no the input curve and  m_point_pairing.Points(B) are pionts on the rebuilt curve.
  /// </summary>
  ON_PointPairing m_point_pairing;

  /// <summary>
  /// Initial value of the objective function when the rebuilt curve is optimized.
  /// </summary>
  double m_initial_objective_value = ON_DBL_QNAN;

  /// <summary>
  /// The minimum value of the objective function calculate from m_rebuilt_curve.
  /// </summary>
  double m_minimum_objective_value = ON_DBL_QNAN;

  /// <summary>
  /// Total number of interations in the optimization process.
  /// If m_initial_objective_value and m_minimum_objective_value are both 0.0
  /// and m_objective_evaluation_count = 0, it indicates that m_rebuilt_curve
  /// is the NURBS form of m_input_curve because it had the desire degree,
  /// control point count, and knot vector.
  /// Note that the minimum value may have been obtained before the final iternation.
  /// This is expected as the minimization algorothim does a final check to look for 
  /// a better result.
  /// </summary>
  unsigned m_objective_evaluation_count = 0;

  /// <summary>
  /// A NURBS curve fit to the input curve.
  /// </summary>
  ON_NurbsCurve m_rebuilt_curve;

  ON_BoundingBox m_rebuilt_curve_bbox = ON_BoundingBox::NanBoundingBox;
};

#endif
