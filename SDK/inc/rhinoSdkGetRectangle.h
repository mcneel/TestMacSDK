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



// This works a lot like CRhinoGetPoint.
// but instead of a line, it draws a rectangle for dynamic feedback
// Caller can use
// CRhinoGetPoint::Constrain( ON_Plane),
// CRhinoGetPoint::ConstrainToConstructionPlane(),
// CRhinoGetPoint::SetBasePoint( ON_3dPoint)
// to supply a context for the rectangle

// Use CRhinoGetPoint::GetPoint() to get the far corner of a
// rectangle based at the BasePoint or construction plane origin
// Use Corners( ON_3dPoint rect[4]) to get the results

class RHINO_SDK_CLASS CRhinoGetRectangle : public CRhinoGetPoint
{
public:
  CRhinoGetRectangle( CArgsRhinoGetPlane& );
  ~CRhinoGetRectangle();

 /*
    Description:
      Use GetCorners() to fill in rect with the 3d corners of the rectangle
    Parameters:
      rect [in] an unused array to store the output corners if GetRectangle() returns point
    Returns:
      @untitled table
      TRUE     Success
      False    Failure
  */
  BOOL32 Corners( ON_3dPoint rect[4]) const;

  /*
    Description:
      Set or get the length or width of the rectangle if they were
      typed in instead of picking a corner
    Parameters:
      length [in] - length to set
      width [in] - width to set
    Returns:
      the typed length or width
  */
  /*
  void SetLength( double length);
  double Length() const;
  void SetWidth( double width);
  */

  /*
  Description:
  Get callback function for doing custom dynamic drawing
  Returns:
  Callback function
  Remarks:
  Good for custom drawing when calling ::RhinoGetRectangle
  */
  std::function<void(const ON_3dPoint[4])> DynamicRectangleCallback() const;

  /*
  Description:
  Set callback function for doing custom dynamic drawing
  Parameters:
  callback [in] Callback function for dynamic drawing
  Remarks:
  Good for custom drawing when calling ::RhinoGetRectangle
  */
  void SetDynamicRectangleCallback(std::function<void(const ON_3dPoint[4])> callback);

  /*
    Description:
      Do the interactive point getting
    Parameters:
      history [in] pointer to a CRhinoHistory record
    Returns:
      CRhinoGet::result
    Remarks:
      Derived from CRhinoGetPoint.
      Use any CRhinoGetPoint options/set-up functions
      See CRhinoGetPoint for return values
  */
  CRhinoGet::result GetRectangle( CRhinoHistory* history);

protected:
  BOOL32 CalcCorners( CRhinoViewport& vp, const ON_3dPoint& point, ON_3dPoint corners[4]) const;

  void OnMouseMove(
          CRhinoViewport&,
          UINT nFlags,
          const ON_3dPoint&, // current 3d world location
          const ON_2iPoint*      // current window 2d mouse location (NULL for 3d digitizer arms)
          ) override;
  void DynamicDraw( CRhinoDisplayPipeline&, const ON_3dPoint& dragpoint) override;

  // [i/o] access to arguments
  CArgsRhinoGetPlane& m_args;

  // [in] prompt when getting end point
  ON_wString m_prompt_second_point;

  // [in] color to use when drawing dynamic line while getting end point
  ON_Color m_dynamic_line_color;

  // for dynamic drawing
  ON_3dPoint m_corners[4];

  // callback for dynamic drawing
  std::function<void(const ON_3dPoint[4])> m_dynamicRectangleCallback;

  // not implemented
  CRhinoGetRectangle( const CRhinoGetRectangle& src);
  CRhinoGetRectangle& operator=( const CRhinoGetRectangle& src);

private:
  class CRhPictureDrawHelper* m_picture_draw_helper;
};


//------------------------------------------------------------------
// class CRhinoGetRectangle3Point : public CRhinoGetPoint

// This works a lot like CRhinoGetPoint.
// but instead of a line, it draws a rectangle for dynamic feedback
// Use SetBaseEdge( ON_Line) for setting the first edge of the rectangle
// and GetPoint() to drag out a point on the other side of the rectangle
// If GetPoint() returns rectangle,
//   Use Corners( ON_3dPoint rect[4]) to get the results

class RHINO_SDK_CLASS CRhinoGetRectangle3Point : public CRhinoGetPoint
{
public:
  CRhinoGetRectangle3Point( CArgsRhinoGetPlane& args);
  ~CRhinoGetRectangle3Point();

  void SetBase( const ON_Line& line);

  // Use GetCorners() to fill in rect with the 3d corners of the rectangle
  BOOL32 Corners( ON_3dPoint rect[4]) const;

protected:
  BOOL32 CalcCorners( CRhinoViewport& vp, const ON_3dPoint& point, ON_3dPoint corners[4]) const;

  void OnMouseMove(
          CRhinoViewport&,
          UINT nFlags,
          const ON_3dPoint&, // current 3d world location
          const ON_2iPoint*      // current window 2d mouse location (NULL for 3d digitizer arms)
          );
  void DynamicDraw( CRhinoDisplayPipeline&, const ON_3dPoint& dragpoint);

  // [i/o] access to arguments
  CArgsRhinoGetPlane& m_args;

  // [in] prompt when getting end point
  ON_wString m_prompt;

  // [in] color to use when drawing dynamic line while getting end point
  ON_Color m_dynamic_line_color;

  // [in]  The first edgs of the rectangle
  ON_Line m_base;

  // for dynamic drawing
  ON_3dPoint m_corners[4];

  // not implemented
  CRhinoGetRectangle3Point( const CRhinoGetRectangle3Point& src);
  CRhinoGetRectangle3Point& operator=( const CRhinoGetRectangle3Point& src);

private:
  CRhPictureDrawHelper* m_picture_draw_helper;
};


//------------------------------------------------------------------
// class CRhinoGetRectangleRounded : public CRhinoGetPoint

class RHINO_SDK_CLASS CRhinoGetRectangleRounded : public CRhinoGetPoint
{
public:
  CRhinoGetRectangleRounded( CArgsRhinoGetPlane& args, ON_3dPoint rect[4]);
  ~CRhinoGetRectangleRounded();

  BOOL32 CalcCorner( const ON_3dPoint& dragpoint, double& radius, double& rho);
  double MaxRadius( void);

protected:
  void OnMouseMove( CRhinoViewport& vp, UINT nFlags, const ON_3dPoint& dragpoint, const ON_2iPoint* point2d);
  void DynamicDraw( CRhinoDisplayPipeline&, const ON_3dPoint& dragpoint);

  CArgsRhinoGetPlane& m_args;
  double m_radius, m_maxradius, m_rho;
  BOOL32 m_bHaveCorner;
  ON_3dPoint* m_rect; // points to an array of 4 points
};

