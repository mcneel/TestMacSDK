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

class RHINO_SDK_CLASS CRhinoFitPatch
{
public:
  //Input geometry for all Fit functions can consist of any 
  // combination of ON_Curves, ON_BrepTrims, ON_Points, ON_PointClouds or ON_Meshes
  // Curves and trims are sampled to get points
  // Trims are sampled for points and normals

  // Simple version of fit
  // Uses a specified starting surface
  // Sample points from the input are pulled to the starting surface to get uv
  //  locations on the surface to try to fit to the point locations.
  static
  ON_Brep* Fit(
    ON_SimpleArray<const ON_Geometry*>& input_geometry,
    const ON_Surface* pSrf,
    double tolerance
    );

  // Simple version of fit
  // Uses a plane with u x v spans
  // Makes a plane by fitting to the points from the input geometry
  //  to use as the starting surface.  The surface has the specified 
  //  u and v span count.
  static
  ON_Brep* Fit(
    ON_SimpleArray<const ON_Geometry*>& input_geometry,
    int u_spans,
    int v_spans,
    double tolerance
    );

  // This version of fit exposes all of the controls
  // that the rhino patch command uses.
  // input_geometry: Combination of ON_Curves, ON_BrepTrims, ON_Points, ON_PointClouds or ON_Meshes
  // pSrf:           Starting surface or NULL to use a plane fit through the input points
  // u_spans, 
  // v_spans:        Number of surface spans used when a plane is fit through points to start
  // bTrim:          If true, try to find an outer loop from among the input curves and trim the result to that loop
  // bTangency:      If true, try to find brep trims in the outer loop of curves and try to fit to the
  //                  normal direction of the trim's surface at those locations.
  // point_spacing:  Basic distance between points sampled from input curves.
  // flexibility:    Determines the behavior of the surface in areas where its not otherwise controlled
  //                  by the input.  Lower numbers make the surface behave more like a stiff material,
  //                  higher, more like a stiff material
  //                  That is, each span is made to more closely match the spans adjacent to it if there 
  //                  is no input geometry mapping to that area of the surface when the flexibility value
  //                  is low.  The scale is logrithmic. Numbers around 0.001 or 0.1 make the patch pretty stiff
  //                  and numbers around 10 or 100 make the surface flexible.
  // surface_pull:   Tends to keep the result surface where it was before the fit in areas where
  //                  there is on influence from the input geometry
  // fix_edges[]:    Flags to keep the edges of a starting (untrimmed) surface in place while fitting
  //                  the interior of the surface.  Order of flags is left, bottom, right, top
  // tolerance:      Tolerance used by input analysis functions for loop finding, trimming, etc.
  static
  ON_Brep* Fit(
    ON_SimpleArray<const ON_Geometry*>& input_geometry,
    const ON_Surface* pSrf,
    int u_spans,
    int v_spans,
    bool bTrim,
    bool bTangency,
    double point_spacing,
    double flexibility,
    double surface_pull,
    int fix_edges[4],
    double tolerance
  );
    
private:
  CRhinoFitPatch();
  CRhinoFitPatch(const CRhinoFitPatch&);
};

