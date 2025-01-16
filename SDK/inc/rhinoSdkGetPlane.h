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

#include <functional>

// Input to the RhinoGetPlane() and RhinoGetRectangle() functions
class RHINO_SDK_CLASS CArgsRhinoGetPlane
{
public:
  enum {
    mode_corners,
    mode_3point,
    mode_vertical,
    mode_center,
    mode_3point_mid,
    mode_3point_center
  };

  enum {
    corner_arc = 0,
    corner_conic = 1,
    corner_chamfer = 2
  };

  enum {
    plane_type = 0,
    box_type = 1,
    plane_type_ex = 2,
    detail_type = 3,
  };

  CArgsRhinoGetPlane();
  virtual ~CArgsRhinoGetPlane();
  CArgsRhinoGetPlane(const CArgsRhinoGetPlane& src);
  CArgsRhinoGetPlane& operator=(const CArgsRhinoGetPlane& src);

  // prompt when getting first corner
  const wchar_t* FirstPointPromptCorners() const;
  void SetFirstPointPromptCorners( const wchar_t* prompt);

  // prompt when getting opposite corner
  const wchar_t* SecondPointPromptCorners() const;
  void SetSecondPointPromptCorners( const wchar_t* prompt);

  // prompt when getting first corner
  const wchar_t* FirstPointPrompt3Point() const;
  void SetFirstPointPrompt3Point( const wchar_t* prompt);

  // prompt when getting second point in 3 point mode
  const wchar_t* SecondPointPrompt3Point() const;
  void SetSecondPointPrompt3Point( const wchar_t* prompt);

  // prompt when getting opposite side in 3point mode
  const wchar_t* ThirdPointPrompt3Point() const;
  void SetThirdPointPrompt3Point( const wchar_t* prompt);

  // prompt when getting first corner
  const wchar_t* FirstPointPromptVertical() const;
  void SetFirstPointPromptVertical( const wchar_t* prompt);

  // prompt when getting second point in vertical mode
  const wchar_t* SecondPointPromptVertical() const;
  void SetSecondPointPromptVertical( const wchar_t* prompt);

  // prompt when getting opposite side in vertical mode
  const wchar_t* ThirdPointPromptVertical() const;
  void SetThirdPointPromptVertical( const wchar_t* prompt);

  // prompt when getting center in center mode
  const wchar_t* FirstPointPromptCenter() const;
  void SetFirstPointPromptCenter( const wchar_t* prompt);

  // prompt when getting corner in center mode
  const wchar_t* SecondPointPromptCenter() const;
  void SetSecondPointPromptCenter( const wchar_t* prompt);

  // color used to draw feedback line
  ON_Color FeedbackColor() const;
  void SetFeedbackColor( COLORREF color);

  // Like CRhinoGet::AcceptNothing().
  void AcceptNothing(bool bAcceptNothing = true);

  // Set or get a plane to which the rectangle is constrained
  // If none is set, the construction plane of the current
  // viewport is used
  // The base point is snapped to this plane
  const ON_Plane& Plane() const;
  void SetPlane( const ON_Plane& plane);
  BOOL32 HavePlane() const;
  void SetHavePlane( BOOL32 b = TRUE);

  // Set or get the basepoint used for the first corner
  // of the rectangle
  // If one isn't supplied, the user is prompted for one
  void SetFirstPoint( const ON_3dPoint& base);
  const ON_3dPoint& FirstPoint() const;
  BOOL32 HaveFirstPoint() const;
  void SetHaveFirstPoint( BOOL32 b = TRUE);

  // Set or get the second end of the first edge
  // If one isn't supplied, the user is prompted for one
  void SetSecondPoint( const ON_3dPoint& point);
  const ON_3dPoint& SecondPoint() const;
  BOOL32 HaveSecondPoint() const;
  void SetHaveSecondPoint( BOOL32 b = TRUE);

  // Set or get the third point in rect3pt mode
  // If one isn't supplied, the user is prompted for one
  void SetThirdPoint( const ON_3dPoint& point);
  const ON_3dPoint& ThirdPoint() const;
  BOOL32 HaveThirdPoint() const;
  void SetHaveThirdPoint( BOOL32 b = TRUE);

  // Set or get the starting mode for getting input
  // Default is start_corners
  void SetCurrentMode( int mode = mode_corners);
  int CurrentMode();

  // Set or Get whether to allow the respective interactive modes
  // Default is to allow them all
  void SetAllowCorners( BOOL32 allow = TRUE);
  BOOL32 AllowCorners();
  void SetAllowVertical( BOOL32 allow = TRUE);
  BOOL32 AllowVertical();
  void SetAllow3Point( BOOL32 allow = TRUE);
  BOOL32 Allow3Point();
  void SetAllowCenter( BOOL32 allow = TRUE);
  BOOL32 AllowCenter();

  void SetAllowRounded( BOOL32 allow = FALSE);
  BOOL32 AllowRounded();
  void SetRounded( BOOL32 allow = FALSE);
  BOOL32 Rounded();

  void SetCornerMode( int mode = corner_arc);
  int CornerMode();

  void SetCornerRho( double rho);
  double CornerRho();
  void SetCornerRadius( double radius);
  double CornerRadius();

  void SetLength( double length);
  double Length();
  void SetWidth( double width);
  double Width();

  void SetAllowForceAspectRatio(BOOL32 allow = TRUE);
  BOOL32 AllowForceAspectRatio();

  void SetForcedAspectRatio(double forced_aspect_ratio);
  double ForcedAspectRatio();

  void SetAllowDeformable( BOOL32 b = TRUE);
  BOOL32 AllowDeformable() const;
  void SetDeformable( BOOL32 bDeformable = TRUE);
  BOOL32 Deformable() const;
  void SetUDegree( int degree);
  int UDegree() const;
  void SetVDegree( int degree);
  int VDegree() const;
  void SetUPointCount( int count);
  int UPointCount() const;
  void SetVPointCount( int count);
  int VPointCount() const;


  //Interface to allow primitive mesh creation to use 
  //RhinoGetPlane, RhinoGetBox, etc.
  void SetPromptForMeshDensity( bool bMeshDensity = true);
  bool PromptForMeshDensity() const;

  // prompt for number of faces in first direction
  const CRhinoCommandOptionName& FirstMeshDensityDirectionPrompt() const;
  void SetFirstMeshDensityDirectionPrompt(const CRhinoCommandOptionName&);
  // number of faces in first direction
  int FirstDirectionCount() const;
  void SetFirstDirectionCount( int count);
  int FirstDirectionMinCount() const;
  void SetFirstDirectionMinCount( int count);

  // prompt for number of faces in second direction
  const CRhinoCommandOptionName& SecondMeshDensityDirectionPrompt() const;
  void SetSecondMeshDensityDirectionPrompt(const CRhinoCommandOptionName&);
  // number of faces in second direction
  int SecondDirectionCount() const;
  void SetSecondDirectionCount( int count);
  int SecondDirectionMinCount() const;
  void SetSecondDirectionMinCount( int count);

  // prompt for number of faces in third direction
  const CRhinoCommandOptionName& ThirdMeshDensityDirectionPrompt() const;
  void SetThirdMeshDensityDirectionPrompt(const CRhinoCommandOptionName&);
  // number of faces in third direction
  int ThirdDirectionCount() const;
  void SetThirdDirectionCount( int count);
  int ThirdDirectionMinCount() const;
  void SetThirdDirectionMinCount( int count);

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

  //return type of args class plane or derived box
  int ArgsType() const;

  //Interface for subclasses adding extra options. 
  // Return true if you want to add extra options to the start of the command option list
  //        false for options at the end
  // Default is false
  virtual bool ExtraOptionsAtStart() const;

  // Allows for adding extra command options during RhinoGetRectangle
  virtual void AddExtraOptions(CRhinoGetPoint& gp);
  virtual void HandleExtraOptions(const CRhinoGetPoint& gp);

  /*
  Description:
    Dynamically draws a picture or bitmap image.
  Parameters:
    filename [in] - Path to the picture or bitmap image file (e.g. png, jpg, bmp, etc.)
    width    [in] - Width of the bitmap image in pixels.
    height   [in] - Height of the bitmap image in pixels.
  Remarks:
    Picture drawing only works in 2Point (Default), 3Point, Center, and Vertical modes.
    The width and height arguments will be used to set the forced aspect ratio.
  See Also:
    CArgsRhinoGetPlane::SetAllowForceAspectRatio
    CArgsRhinoGetPlane::SetForcedAspectRatio
  */
  void SetPictureFileNameAndSize(const wchar_t* filename, int width, int height);
  const wchar_t* PictureFileName() const;
  bool DrawPicture();

  // input - simple options to add to the first prompt;
  ON_ClassArray<CRhinoCommandOptionName> m_simple_options;
  // output - if >= 0, one of the simpple options above
  // was picked.
  int m_simple_option_index;

  // When true the linetype of the active layer is used for dynamic drawing
  void SetUseActiveLayerLinetype(bool on);
  bool UseActiveLayerLinetype() const;

protected:
  // prompt when getting first corner in corners mode
  ON_wString m_prompt_first_point_corners;

  // prompt when getting opposite corner or second end of first edge
  ON_wString m_prompt_second_point_corners;

  // prompt when getting first corner in 3 point mode
  ON_wString m_prompt_first_point_3point;

  // prompt when getting opposite corner or second end of first edge
  ON_wString m_prompt_second_point_3point;

  // prompt when getting point on opposite side in 3 point mode
  ON_wString m_prompt_third_point_3point;

  // prompt when getting first corner in vertical mode
  ON_wString m_prompt_first_point_vertical;

  // prompt when getting the second point in vertical mode
  ON_wString m_prompt_second_point_vertical;

  // prompt when getting point on opposite side in 3 point mode
  ON_wString m_prompt_third_point_vertical;

  // prompt when getting center in center mode
  ON_wString m_prompt_first_point_center;

  // prompt when getting corner in center mode
  ON_wString m_prompt_second_point_center;


  // color to use when drawing dynamic line while getting second corner
  ON_Color m_dynamic_line_color;

  // plane on which the rectangle will lie
  ON_Plane m_plane;
  BOOL32 m_haveplane;

  // called-supplied first corner ot the rectangle
  ON_3dPoint m_firstpoint;
  BOOL32 m_havefirstpoint;

  // called-supplied second end of first edge
  ON_3dPoint m_secondpoint;
  BOOL32 m_havesecondpoint;

  // called-supplied third point in rect3pt mode
  ON_3dPoint m_thirdpoint;
  BOOL32 m_havethirdpoint;

  
  // starting mode - corners, 3pt, vertical
  int m_current_mode;

  // allow switching to respective modes for interactive input
  BOOL32 m_allow_corners;
  BOOL32 m_allow_3point;
  BOOL32 m_allow_vertical;
  BOOL32 m_allow_center;

  BOOL32 m_allow_deformable;
  BOOL32 m_deformable;

  int m_udegree;
  int m_vdegree;
  int m_ucount;
  int m_vcount;

  BOOL32 m_allow_rounded;
  BOOL32 m_rounded;
  int m_corner_mode;
  double m_corner_rho;
  double m_corner_radius;

  double m_length;
  double m_width;

  BOOL32 m_force_aspect_ratio;
  double m_forced_aspect_ratio;

  //Interface to allow primitive mesh creation to use 
  //RhinoGetPlane, RhinoGetBox, etc.
  bool m_PromptForMeshDensity;
  CRhinoCommandOptionName m_mesh_density_first_direction;
  int m_mesh_density_1stDirection;
  int m_mesh_density_Min1stDirection;
  CRhinoCommandOptionName m_mesh_density_second_direction;
  int m_mesh_density_2ndDirection;
  int m_mesh_density_Min2ndDirection;
  CRhinoCommandOptionName m_mesh_density_third_direction;
  int m_mesh_density_3rdDirection;
  int m_mesh_density_Min3rdDirection;

  //This variable is to distinguish whether this class is really
  //a CArgsRhinoGetPlane or the derived class CArgsRhinoGetBox.  It
  //is set to plane_type in the constructor for the plane, which is called first,
  //and box_type in the constructor for the box.  ArgsType() returns the value of 
  //m_ArgsType
  int m_ArgsType;

  // file used to draw picture or bitmap image dynamically
  ON_wString m_picture_filename;

  std::function<void(const ON_3dPoint[4])> m_dynamicRectangleCallback;

public:
  bool m_bAcceptNothing;
private:
  unsigned char m_pen_usage = 0;
  unsigned char m_reserved[6] = { 0 };
};



//////////
// Gets an oriented infinite plane
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetPlane(
    CArgsRhinoGetPlane& arg,
    ON_Plane& plane,
    CRhinoHistory* history = NULL
    );



/*
Description:
  Get a 3d rectangle.
Parameters:
  args - [in]
  rectangle_corners - [out] corners of rectangle in counter clockwise order
  history - [in/out]
Returns:
  CRhinoCommand::success if successful.
See Also:
  RhinoGetScreenRectangle
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetRectangle(
    CArgsRhinoGetPlane& args,
    ON_3dPoint rectangle_corners[4], // output ordered counter clockwise
    CRhinoHistory* history = NULL // optional history recorder
    );

/*
Description:
  Get a rectangle in view window coordinates.
Parameters:
  rect - [out] rectangle 
      0 <= left < right <= view width
      0 <= top < bottom <= view height
  pen_style - [in] (default = PS_SOLID) style of pen
      to use for drawing the dynamic 2d rectangle.
Returns:
  Pointer to the view the rectangle is in or NULL if the
  user canceled.
Remarks:
  The user can either click_down-drag-release or 
  click_down-release-drag-click_down to draw the rectangle.
See Also:
  CRhinoGetPoint::Get2dRectangle
*/
RHINO_SDK_FUNCTION
CRhinoView* RhinoGet2dRectangle( 
       ON_4iRect& rect,
       int pen_style = PS_SOLID
       );

// Gets a bounded plane that cuts through a boundingbox
// The output rectangle is perpendicular to the input plane and parallel to the input line
RHINO_SDK_FUNCTION
int RhinoCutPlane(
    const ON_Line& line_in,      // line included in the output rectangle
    const ON_Plane& plane,       // plane perpendicular to the output rectangle
    const ON_BoundingBox& bbox,  // bounding box through which to cut
    ON_3dPoint rect_out[4]);     // four 3d corners of the output rectangle



/*
  Description
    Make a plane that includes a line and a vector and goes through a bounding box
  Parameters:
    [in] line_in - line in the plane
    [in] vector_in - vector in the plane
    [in] bbox - box to cut through
  Returns:
    ON_PlaneSurface - a plane that goes thriugh the box
    NULL for error
  Remarks:
    Makes a plane from the line and vector and projects the corners of the box to it
*/
RHINO_SDK_FUNCTION
ON_PlaneSurface* RhinoPlaneThroughBox( const ON_Line& line_in,       // line contained it returned plane
                                       const ON_3dVector& vector_in, // vector contained in returned plane
                                       const ON_BoundingBox& bbox);  // bounding box through which to cut


/*
  Description
    Extend a plane so that it goes through a bounding box
  Parameters:
    [in] line_in - line in the plane
    [in] vector_in - vector in the plane
    [in] bbox - box to cut through
  Returns:
    ON_PlaneSurface - a plane that goes through the box
    NULL for error
  Remarks:
    Projects the corners of the box to the plane and sets the domain to include the projections
*/
RHINO_SDK_FUNCTION
ON_PlaneSurface* RhinoPlaneThroughBox( const ON_Plane& plane_in,      // plane to extend through box
                                       const ON_BoundingBox& bbox);   // box through which to extend


class RHINO_SDK_CLASS CArgsRhinoGetBox : public CArgsRhinoGetPlane
{
public:
  CArgsRhinoGetBox();
  virtual ~CArgsRhinoGetBox();
  CArgsRhinoGetBox(const CArgsRhinoGetBox&);
  CArgsRhinoGetBox& operator=(const CArgsRhinoGetBox&);

  const wchar_t* HeightPrompt() const;
  void SetHeightPrompt( const wchar_t* height_prompt );
  ON_wString m_height_prompt;
};

class RHINO_SDK_CLASS CArgsRhinoGetThickBox : public CArgsRhinoGetBox
{
public:
  CArgsRhinoGetThickBox();
  virtual ~CArgsRhinoGetThickBox();
  CArgsRhinoGetThickBox(const CArgsRhinoGetThickBox&);
  CArgsRhinoGetThickBox& operator=(const CArgsRhinoGetThickBox&);
  const wchar_t* ThicknessPrompt() const;
  void SetThicknessPrompt(const wchar_t*);
  const CRhinoCommandOptionName& ThicknessOption() const;
  void SetThicknessOption(const CRhinoCommandOptionName&);
  const bool AddThickness() const;
  void SetAddThickness(const bool);
  const double Thickness() const;
  void SetThickness(const double);

  // November 11, 2024 - Tim
  // Partial fix (box) for https://mcneel.myjetbrains.com/youtrack/issue/RH-63751
  // Add settings for bothsides.
  const bool AddBothSides() const;
  void SetAddBothSides(const bool);
  const CRhinoCommandOptionName& BothSidesOption() const;
  void SetBothSidesOption(const CRhinoCommandOptionName&);
  const bool BothSides() const;
  void SetBothSides(const bool);

protected:
  class CInternalArgsRhinoGetThickBox* m_internal;
};

// Description:
//   Get a 3d box
// Parameters:
//   args - [in] controls how the box is picked, prompts, etc.
//   box_corners - [out] corners of the box.
//
//            7______________6
//            |\             |\
//            | \            | \
//            |  \ _____________\
//            |   4          |   5
//            |   |          |   |
//            |   |          |   |
//            3---|----------2   |
//            \   |          \   |
//             \  |           \  |
//              \ |            \ |
//               \0_____________\1
//
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetBox(
    CArgsRhinoGetBox& args,
    ON_3dPoint box_corners[8],
    CRhinoHistory* history = NULL
    );

class RHINO_SDK_CLASS CArgsRhinoGetPicture : public CArgsRhinoGetPlane
{
public:
  CArgsRhinoGetPicture(int width, int height, double one_to_one_width, double one_to_one_height);
  virtual ~CArgsRhinoGetPicture();
  virtual void AddExtraOptions(CRhinoGetPoint& gp);
  virtual void HandleExtraOptions(const CRhinoGetPoint& gp);

public:
  bool m_one_to_one;
  int m_one_to_one_option;
  double m_width;
  double m_height;
  double m_one_to_one_width;
  double m_one_to_one_height;
};
