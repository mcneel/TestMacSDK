//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
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
#if !defined(OPENNURBS_IMPLICITFN_INC_)
#define OPENNURBS_IMPLICITFN_INC_

/* An abstract base class for a signed distance function (SDF).

 * The SDF must be a non-manifold function that divides
 * R3 into closed (possibly at infinity) regions of
 * negative distance and positive distance, thereby defining
 * a set of implicit boundaries where the SDF is equal to zero.

 */
class ON_CLASS ON_SignedDistanceFunction
{
public:
  ON_SignedDistanceFunction()
    : nEval(0)
  {

  }
  // Evaluate the signed distance function
  // Parameters:
  //   p      [in]  - the point to evaluate the SDF at
  //   normal [out] - normal at the closest point on the SDF
  //                  such that closestPt = pt - dist*normal/normal.Length()
  // Returns:
  //   the distance between the closest point and the input point.
  // Remarks:
  //   1. The returned distance can be ON_UNSET_VALUE
  //       when the SDF is unable to evaluate the distance
  //   2. The returned normal is not unitized and can
  //      have (almost) zero length.
  //
  //   You should account for both cases when using the SDF.
  //
  virtual double Evaluate(const ON_3dPoint& p, ON_3dVector& normal) const = 0;
  int nEval;
};

class ON_CLASS ON_PlaneSDF : public ON_SignedDistanceFunction
{
public:
  ON_PlaneSDF(const ON_Plane& plane);
  double Evaluate(const ON_3dPoint& pt, ON_3dVector& normal) const override;
private:

  const ON_PlaneEquation& m_plane;
};

class ON_CLASS ON_SphereSDF : public ON_SignedDistanceFunction
{
public:
  ON_SphereSDF(ON_3dPoint center, double radius)
    : center(center)
    , radius(radius)
  {

  }
public:
  double Evaluate(const ON_3dPoint& pt, ON_3dVector& normal) const override;
  const ON_3dPoint center;
  const double radius;
};

class ON_CLASS ON_OffsetCurveSDF : public ON_SignedDistanceFunction
{
public:
  ON_OffsetCurveSDF(const ON_Curve& curve, double distance)
    : m_curve(curve)
    , m_distance(distance)
  {

  }
  double Evaluate(const ON_3dPoint& pt, ON_3dVector& normal) const override;
private:
  const ON_Curve& m_curve;
  double m_distance;
};




// Curve/ImplicitFunction intersection event
class ON_CLASS ON_CIXEvent
{
public:
  ON_CIXEvent()
    : m_type(Type::Unknown)
    , m_point(ON_3dPoint::UnsetPoint)
    , m_domain(ON_Interval::EmptyInterval)
  {

  }

  enum class Type : int
  {
    Unknown = 0, // unknown event type
    NoEvent = 1, // no point or overlap
    Point = 2,   // point intersection
    Overlap = 3  // overlap intersection
  };

  enum class AmbigType : int
  {
    Unknown = 0,
    Tangent = 1,
    TangentEnd = 2,
    Transverse = 3
  };
public:
  Type m_type;
  ON_3dPoint m_point;
  // the overlap interval [t0, t1] with t0 < t1 of the overlapping curve,
  // or the point parameter t as [t, t]
  ON_Interval m_domain;
};

// a parametric/space curve pair used in surface intersections
class ON_CLASS ON_CurvePair
{
public:
  ON_CurvePair()
    : ON_CurvePair(nullptr, nullptr)
  {

  }

  ON_CurvePair(const ON_Curve* curve2d, const ON_Curve* curve3d)
    : Curve2d(curve2d)
    , Curve3d(curve3d)
  {

  }

  const ON_Curve* Curve3d;
  const ON_Curve* Curve2d;
};

// Surface/ImplicitFunction Intersection event
class ON_CLASS ON_SIXEvent
{
public:
  ON_SIXEvent()
    : m_type(Type::Unknown)
    , m_uv(ON_2dPoint::UnsetPoint)
    , m_surface(nullptr)
  {
  }


  enum class Type : int
  {
    Unknown = 0, // unknown event type
    NoEvent = 1, // no point, curve or surface overlap
    Point = 2,   // the surface intersects the SDF at a point
    Curve = 3,   // the surface intersects the SDF with a curve
    Surface = 4  // the surface overlaps the SDF intersection
  };

  Type m_type;

  ON_2dPoint m_uv;
  ON_CurvePair m_curves;
  ON_Surface* m_surface;
};

#endif
