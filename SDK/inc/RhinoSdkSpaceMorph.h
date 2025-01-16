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

class RHINO_SDK_CLASS CRhinoSpaceMorph : public ON_SpaceMorph
{
public:
  /*
  Description:
    The default constructor sets the morph tolerance
    to 0.001.  You should adjust this to something
    that makes sense in your context.
  */
  CRhinoSpaceMorph();

  virtual ~CRhinoSpaceMorph();
};

// Twist morph
class RHINO_SDK_CLASS CRhinoTwistSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoTwistSpaceMorph();

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const override;

  // virtual ON_SpaceMorph override
  bool IsIdentity( const ON_BoundingBox& bbox ) const override;

  ON_Line m_twist_axis; // Axis about which to twist
  double m_twist_angle; // radians
  bool m_bInfiniteTwist;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Taper morph
class RHINO_SDK_CLASS CRhinoTaperSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoTaperSpaceMorph();
  ~CRhinoTaperSpaceMorph();

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint point ) const override;

  // virtual ON_SpaceMorph override
  bool IsIdentity( const ON_BoundingBox& bbox ) const override;

  /*
  Description:
    Validates and sets the end radius
  Parameters:
    double - [in] end radius
  Returns:
    true if end radius is successfully set
  */
  bool SetEndDistance( double );

  /*
  Description:
    Takes a point and calculates the end radius based on the 
    distance from the origin of the taper plane
  Parameters:
    ON_3dPoint - [in] point from the plane origin defining an end radius
  Returns:
    true if end radius is successfully set
  */
  bool SetEndDistance( ON_3dPoint );

  ON_Line m_taper_axis; 
  double m_start_radius;
  double m_end_radius;

  ON_Plane m_plane;

  // Create a one-directional, one-dimensional taper.
  bool m_bFlat;
  double m_length;

  /* 
    If true the deformation happens throughout the object, even if 
    the axis is shorter. Otherwise, the deformation takes place only
    the length of the axis. The length of the axis is important. If
    the axis is shorter than the object, the taper applies only to
    that part of the object.
  */
  bool m_bInfiniteTaper;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Bend morph
// Deforms objects by bending along a spine arc.
class RHINO_SDK_CLASS CRhinoBendSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoBendSpaceMorph();
  ~CRhinoBendSpaceMorph();

  CRhinoBendSpaceMorph(const CRhinoBendSpaceMorph&);
  CRhinoBendSpaceMorph& operator=(const CRhinoBendSpaceMorph&);

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const;

  // virtual ON_SpaceMorph override
  bool IsIdentity(const ON_BoundingBox& bbox) const;

  /*
  Description:
    Checks if the bend spine line is valid, then sets 
    the spine parameter
  Parameters:
    ON_Line spine - [in]
  Returns:
    true if successfully set spine
  */
  bool SetSpine( ON_Line spine );

  /*
  Description:
    Defines the arc about which to bend using a point and angle
  Parameters:
    ON_3dPoint bend_pt - [in]
    double bend_angle_degrees - [in]
  Returns:
    true if the arc has been successfully created from the
    supplied point and angle
  */
  bool SetArc( ON_3dPoint bend_pt, double bend_angle_degrees );

  /*
  Description:
    Checks whether arc and spine values have been set
  Returns:
    true if both arc and spine values are set
  */
  bool IsValid() const;

  /*
  Description:
    Checks to see if a bend point and bend angle have been updated
    and are valid
  Parameters:
    ON_3dPoint bend_point - [in]
    double bend_angle_degrees - [in]
  Returns:
    true if bend point and angle are valid and new, otherwise false
  */
  bool NewBendPoint( ON_3dPoint bend_point, double bend_angle_degrees ) const;

  /*
  Description:
    Checks whether a supplied angle in degrees is valid
  Parameters:
    double bend_angle_degrees - [in] angle in degrees to verify
  Returns:
    true if the angle is valid
  */
  static bool ValidBendAngle(double bend_angle_degrees);

  // bend morph options

  // If m_bStraightEnd is true, then the length of the 
  // bend arc = length of the spine. If m_bStraightEnd is false, 
  // then the bend arc ends at the bend point.  The option
  // is poorly named, because even when m_bStraightEnd=false, 
  // the bend is straight after the bend point.
  bool m_bStraightEnd;

  // If m_bSymmetric is true, then the bend arc is mirrored.
  bool m_bSymmetric;

  // If false (default), the morph will be attenuated at arc ends, making it suitable
  // for morphing parts of objects.
  // If true, the morph will uniformly affect the entire region along the arc. Most suitable for morphing
  // whole objects.
  void SetNonAttenuated(bool bNonAttenuated);
  bool NonAttenuated() const;

protected:
  bool m_bHaveSpine;
  ON_Line m_spine; // line
  double m_spine_length;

  double m_bend_angle;
  ON_3dPoint m_bend_point;

  // m_arc starts at m_spine.from, is tangent to m_spine.Direction,
  // and ends at m_arc_endpt.  The length of this arc is generally
  // different that the length of the spine.  This is the raw
  // input information.  The DrawSpineAndArc() and MorphPoint()
  // functions extend/shorten this arc based on the value
  // m_bStraightEnd.
  bool m_bHaveArc;
  ON_Arc m_arc;
  ON_3dPoint m_arc_endpt;
  double m_arc_length;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};


// Flow morph
// Re-aligns an object or group of objects from a base curve to a target curve.
class RHINO_SDK_CLASS CRhinoFlowSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoFlowSpaceMorph();
  ~CRhinoFlowSpaceMorph();

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const;
  ON_3dVector MorphVector(ON_3dPoint tail_point, ON_3dVector vector) const;

  bool SetRadii(double r0, double r1);

public:

  // 30 June 2004 Dale Lear:
  //     Stuff that FastMorphPoint() needs to work.
  void FlowInit(
    const ON_Curve* crv_from, bool bReverseFrom, 
    const ON_Curve* crv_to, bool bReverseTo,
    bool bPreventStretching,
    const ON_Surface* srf_to = nullptr // optional target surface. If supplied crv_to should be on srf_to for predictable results.
    );

  void FlowCleanup();

  ON_Curve* m_fastflow_crv_from;
  ON_Curve* m_fastflow_crv_to;
  ON_Surface* m_fastflow_srf_to;
  double m_fastflow_length_from;
  double m_fastflow_length_to;
  ON_3dVector m_fastflow_from_up;
  ON_3dVector m_fastflow_to_up;
  ON_SimpleArray< double > m_fastflow_from_t;
  ON_SimpleArray< double > m_fastflow_to_t;
  
  // if true, no stretching happens along the flow curve
  bool m_bPreventStretching;
  // If true, the flow "from" and "to" frame z axes are always m_fastflow_from_up and m_fastflow_to_up
  bool m_bStairlike;

private:
  double m_fastflow_max_radius;
  ON_Interval m_fastflow_radius; // ignored unless 0 < m_fastflow_radius[0] < m_fastflow_radius[1]
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Flow along surface morph
class RHINO_SDK_CLASS CRhinoSporphSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoSporphSpaceMorph();
  ~CRhinoSporphSpaceMorph();

  ON_Surface* m_srf[2]; // The surface planes to flow along surface
  ON_Interval m_s[2]; // Length of the surface plane
  ON_Interval m_t[2]; // Width of the surface plane
  ON_3dVector m_N1; // If set defines a normal to use instead of target surface normal

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const;

private:
  ON_3dPoint m_pt0;
  ON_3dPoint m_pt1;
  double m_s0, m_t0, m_s1, m_t1;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Maelstrom
// Deforms objects in a spiral as if they were caught in a whirlpool.
class RHINO_SDK_CLASS CRhinoMaelstromSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoMaelstromSpaceMorph();
  ~CRhinoMaelstromSpaceMorph();

  /*
  Description:
    virtual ON_SpaceMorph override

    If m_r0 = m_r1 > 0, then the morph is a rotation where
    the angle of rotation is proportional to the radius.

    If m_r0 < m_r1, then everything inside of the circle
    of radius m_r0 if fixed, the rotation angle increases
    smoothly from 0 at m_r0 to m_a at m_r1, and everything
    outside of the circle of radius m_r1 is rotated by m_a.

    If m_r0 > m_r1, then everything outside of the circle
    of radius m_r0 if fixed, the rotation angle increases
    smoothly from 0 at m_r0 to m_a at m_r1, and everything
    inside of the circle of radius m_r1 is rotated by m_a.

  */
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const;

  bool IsIdentity(const ON_BoundingBox& bbox ) const;

  ON_Circle m_circle;

  // fixed radius
  double m_r0;

  // rotated radius
  double m_r1;

  // angle in radians
  double m_a;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Stretch morph
// Deforms an object by scaling selected portions in one direction.
class RHINO_SDK_CLASS CRhinoStretchSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoStretchSpaceMorph();

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const;

  // virtual ON_SpaceMorph override
  bool IsIdentity( const ON_BoundingBox& bbox ) const;

  /*
  Description:
    Takes a point to stretch to and converts it to a stretch factor value
  Parameters:
    ON_3dPoint stretch_point - [in]
  Returns:
    A stretch factor if a valid point, otherwise returns ON_UNSET_VALUE 
  */
  double GetStretchFactor( ON_3dPoint stretch_point );

  /*
  Description:
    Takes a number that represents a new stretch axis length and converts it to a stretch factor value
  Parameters:
    double new_length - [in]
  Returns:
    A stretch factor, or ON_UNSET_VALUE if m_stretch_axis.Length() is zero.
  */
  double GetStretchFactor( double new_length );

  /*
  Description:
    If the stretch_factor is valid, it sets the m_stretch_factor and
    builds the ON_BezierCurve m_stretch_function
  Parameters:
    double stretch_factor - [in]
  Returns:
    True if stretch_factor is valid
  */
  bool SetStretchFactor( double stretch_factor );

  /*
  Description:
    Identifies whether the m_stretch_axis and m_stretch_factor
    are set and valid. 
  Returns:
    true if m_stretch_axis and m_stretch_factor are valid
  */
  bool IsValid() const;

  static bool IsValidStretchFactor(double stretch_factor);

  // Line along which to perform the stretch operation
  ON_Line m_stretch_axis;
  
  /*
  NOTE:
   The m_stretch_function and m_stretch_factor are for expert users only.
   It is preferred to use the GetStretchFactor and SetStretchFactor functions
   instead. 

   The m_stretch_function is a monotone increasing function f() such that:
   f(0) = 0, f(1) = 1, f'(0) = 0, f'(1) = 1
   */
  ON_BezierCurve m_stretch_function;

  double m_stretch_factor;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

// Splop morph
// Copies, rotates, scales, and wraps objects on a surface, 
// like pottery sprigging or appliqué.
class RHINO_SDK_CLASS CRhinoSplopSpaceMorph : public CRhinoSpaceMorph
{
public:
  CRhinoSplopSpaceMorph();
  ~CRhinoSplopSpaceMorph();

  // virtual ON_SpaceMorph override
  ON_3dPoint MorphPoint( ON_3dPoint pt ) const;

  ON_3dPoint m_P0;
  ON_3dVector m_X, m_Y, m_N;
  double m_M[2][2];
  double m_u0;
  double m_v0;
  const ON_Surface* m_srf;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

