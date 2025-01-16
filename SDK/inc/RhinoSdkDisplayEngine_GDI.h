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

class RHINO_SDK_CLASS CRhinoDisplayEngine_GDI : public CRhinoDisplayEngine
{
  ON_OBJECT_DECLARE(CRhinoDisplayEngine_GDI);
public:
  CRhinoDisplayEngine_GDI();
  CRhinoDisplayEngine_GDI(class CRhVectorEngine*);
  virtual ~CRhinoDisplayEngine_GDI();

public:
  bool VectorEngineAttached() const;

public:
  int MaxClipPlanesSupported() const override;

  bool  StartEngine() override;
  bool  StopEngine() override;

  bool  ResetEngine() override;
  
  void  ShutdownEngine() override;

  void  ClearFrameBuffer(const ON_Color&, const ON_Color&, const ON_Color&, const ON_Color&) override;

  void  ClearFrameBuffer(const CRhinoDib*) override;
  
  // Used in printing code to set up clip regions
  const ON_4iRect SetClipRect(const ON_4iRect& clipRect);
  bool SetClipBoundary(const ON_Curve* boundary);

  void SaveGraphicsState();       // Mac only; does nothing on MS Windows
  void RestoreGraphicsState();    // Mac only; does nothing on MS Windows

  void DrawBezier(int dim, BOOL32 bIsRational, int order, int cv_stride, const double*  cv4d, const CRhCurveAttributes& attrs) override;

  void DrawPoints(int count, const class CRhPointData* points, CRhinoCacheHandle* cache) override;

  void DrawPolylines(const CRhinoClippingVertexCollection* lists, int count) override;

  void DrawMeshes(const ON_Mesh** pMeshes, int count, bool wires, bool shaded, const CDisplayPipelineAttributes*, CRhinoCacheHandle* cacheHandle) override;

  void DrawHatch(const ON_Hatch& hatch, const ON_HatchPattern* pattern, float strokeWidth, ON_Color hatchColor, ON_Color boundaryColor, CRhinoCacheHandle* cacheHandle) override;

  // Do not call - placeholder virtual function in order to improve hatch display without breaking SDK.
  void DrawHatch(const ON_Hatch& hatch, const class CRhinoDisplayBrush* fillBrush, const class CRhinoDisplayPen* boundaryPen, CRhinoCacheHandle* cacheHandle) override;

  void DrawCurve(const ON_Curve& curve, const class CRhinoDisplayPen& pen, CRhinoCacheHandle* cacheHandle) override;

  void Draw2dLines(const ON_2fPoint* points, int count,
    bool asPolyline,
    ON_Color strokeColor,
    float strokeWidth,
    RhinoDashStyle style,
    bool xormode) override;

  void DrawRoundedRectangle(ON_2fPoint screenCenter,
    float pixelWidth,
    float pixelHeight,
    float cornerRadius,
    ON_Color strokeColor,
    float strokeWidth,
    ON_Color fillColor
    ) override;

  void DrawDot(const ON_TextDot& dot,
    bool locationInWorld,
    ON_Color fillColor,
    ON_Color textColor,
    ON_Color borderColor,
    class CRhinoCacheHandle* cache
    ) override;

  void DrawBitmap(const CRhinoDib&, int  nX, int  nY) override;

  void DrawString(const ON_TextContent& text, const ON_Xform& textXform, ON_Color color,
    CRhinoCacheHandle* cache) override;

  void DrawString(const wchar_t* text, const ON_Xform& xf, const ON_Font* font, const ON_Color& color,
    ON::TextHorizontalAlignment horizontalAlignment, ON::TextVerticalAlignment verticalAlignment,
    CRhinoCacheHandle* cache) override;

  // Description:
  //   Draws text with specified color, font and height
  void DrawString(const wchar_t* text, const ON_Color& textColor, const ON_2dPoint& point,
    bool middle, float fontHeightPixels, const ON_Font* font ) override;

  void DrawPolygon(
        const ON_3dPoint*  points,
              int          count,
              ON_Color     color,
              bool         bFilled
              ) override;

  // Description:
  //   Draws wallpaper image into the back buffer.
  void DrawWallpaper(const class CRhWallpaper& wallpaper) override;

  // Description:
  //   Draws background bitmap image onto the construction plane.
  void DrawTraceImage(const class CRhTraceImage& traceimage) override;
  
  bool DrawTexturedPlane(const CRhinoDib& image, const ON_2dPoint& screenLowerLeft, const ON_2dPoint& screenLowerRight, const ON_2dPoint& screenUpperRight, const ON_2dPoint& screenUpperLeft) const;
  bool DrawTexturedPlane(const CRhinoDib& image, const ON_Plane& plane, ON_Interval domain_horz, ON_Interval domain_vert) const;

///////////////////////////////////////////////////////////////////////////
// Engine buffers support...
public:
  bool            GetBackBuffer(CRhinoDib&) override;
  
  bool            CopyBackBufferTo(HDC hDC) override;
  
  bool            CopyBackBufferTo(CRhinoDib&) override;
  
  bool            CopyBackBufferFrom(const CRhinoDib&) override;

  bool            SaveBackBuffer() override;
  
  bool            RestoreBackBuffer() override;
protected:
  bool            NotifyFrameSizeChanged(int width, int height) override;

private:
  bool DrawTexturedPlane(const CRhinoDib& image, 
    const ON_2dPoint& screenLowerLeft, const ON_2dPoint& screenLowerRight, const ON_2dPoint& screenUpperRight, const ON_2dPoint& screenUpperLeft,
    const ON_NurbsCurve* boundary,
    const ON_SimpleArray<ON_NurbsCurve*>* innerLoops
  ) const;
  friend class CRhinoDisplayPipeline_GDI;

  class CRhDisplayEnginePrivate_GDI* m_private_gdi = nullptr;
};
