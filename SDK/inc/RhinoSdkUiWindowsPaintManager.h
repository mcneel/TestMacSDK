/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

RHINO_SDK_FUNCTION bool RhinoUiScaleLocalizedFonts();

class RHINO_SDK_CLASS CRhinoUiPaintManager
{
public:
  CRhinoUiPaintManager();
  void Initialize();
  ~CRhinoUiPaintManager();

  enum
  {
    unset_color = 0xFF000000,
    error_color = 0xFE000000,
    bylayer_color = 0xFD000000,
    byparent_color = 0xFC000000,
    bydisplay_color = 0xFB000000,
  };

  COLORREF ColorButtonFace( bool inactive_color = false) const;
  COLORREF ColorButtonEdge() const;
  COLORREF ColorButtonEdgeShadow() const;
  COLORREF ColorButtonEdgeHighlight() const;
  COLORREF ColorButtonText( bool inactive_color = false) const;
  COLORREF ColorCaptonText( bool inactive_color = false) const;
  COLORREF ColorButtonHot() const;
  COLORREF ColorButtonPushed() const;
  COLORREF ColorNCBackground() const;
  COLORREF ColorClientBackground() const;
  COLORREF ColorButtonBitmapMask() const;
  COLORREF ColorGripperBars() const;
  COLORREF ColorDockBarClientBackground() const;
  COLORREF ColorExpandableTab( bool inactive_color = false) const;
  COLORREF ColorExpandableTabText( bool inactive_color = false) const;
  COLORREF ColorItem( bool selected, bool inactive_color = false, bool separator = false) const;
  COLORREF ColorItemText( bool selected, bool inactive_color = false, bool separator = false) const;
  COLORREF ColorItemValue( bool selected, bool inactive_color = false) const;
  COLORREF ColorItemValueText( bool selected, bool inactive_color = false) const;
  COLORREF ColorTransparentNCDraw() const;

  bool DrawNcWindowFrame( CWnd& wnd, CDC& dc, bool bSmallCaption, bool bActive, LPRECT lprWin = NULL, LPRECT lprClient = NULL, LPRECT lprCaption = NULL);
  void DrawDockBarGripper( CDC& dc, CRect rGripper, int dock_location);
  int DrawExpandIcon(CDC& dc, int x, int y, bool expanded);
  void DrawDividerLine(
    CDC& dc,
    const CRect& dividerRect,
    bool enabled,
    bool horizontal = true,
    COLORREF lineColor = ON_UNSET_COLOR,
    COLORREF disabledLineColor = ON_UNSET_COLOR
  );

  enum class DrawExpanderHeaderOptions : unsigned int
  {
    None              = 0x00000000,
    DrawBoldText      = 0x00000001,
    UseLargeText      = 0x00000002,
    Collapsible       = 0x00000004,
    Opened            = 0x00000008,
    Disabled          = 0x00000010,
    DrawSeparatorLine = 0x00000020,
    UsePathElipses    = 0x00000040,
  };

  DrawExpanderHeaderOptions DefaultDrawExpanderHeaderOptions() const;

  void DrawExpanderHeader(
    CDC& dc,
    const CRect& headerRect,
    int leftMargin,
    int rightMargin,
    const wchar_t* text,
    DrawExpanderHeaderOptions options,
    COLORREF bkgrndColor = ON_UNSET_COLOR,
    COLORREF textColor = ON_UNSET_COLOR,
    COLORREF disabledTextColor = ON_UNSET_COLOR,
    COLORREF separatorLineColor = ON_UNSET_COLOR,
    COLORREF disabledseparatorLineColor = ON_UNSET_COLOR
  );

  enum class ExpanderHeaderDefaultColorPart : unsigned int
  {
    Background,
    Text,
    DisabledText,
    SeparatorLine,
    DisabledSeparatorLine
  };

#pragma region RH_C_SHARED_ENUM [CRhinoUiPaintManager::FontSize] [Rhino.UI.Fonts.Size] [nested:int]
  /// <summary>
  /// Size for UI font.
  /// </summary>
  enum class FontSize : int {
    /// <summary>0.8 x Normal</summary>
    Small,
    /// <summary>1.0 x Normal</summary>
    Normal,
    /// <summary>1.2 x Normal</summary>
    Large,
    /// <summary>2.0 x Normal</summary>
    Title,
  };
#pragma endregion

#pragma region RH_C_SHARED_ENUM [CRhinoUiPaintManager::FontStyle] [Rhino.UI.Fonts.Style] [nested:int]
  /// <summary>
  /// Style for UI font.
  /// </summary>
  enum class FontStyle : int {
    /// <summary>Normal</summary>
    Regular = 0,
    /// <summary>Bold</summary>
    Bold = 1,
    /// <summary>Italic</summary>
    Italic = 2,
    /// <summary>Underline</summary>
    Underline = 4,
    /// <summary>Strikeout</summary>
    Strikeout = 8
  };
#pragma endregion

  COLORREF DrawExpanderHeaderDefaultColor(ExpanderHeaderDefaultColorPart part) const;

  void PaintDockBarClientArea( CControlBar& cb, CDC& dc);

  CSize SmallCaptionButtonSize() const;

  CFont* GetUiFont(FontSize size = FontSize::Normal, FontStyle style = FontStyle::Regular);

  bool DrawExpandableTabCtrlBkgnd( HWND hWnd, CDC* pDC, CRhinoUiThemeDraw* pThemeDraw);
  bool DrawExpandableTabPageBkgnd( HWND hWnd, CDC* pDC, CRhinoUiThemeDraw* pThemeDraw, bool bSpecial);
  CBrush* GetExpandableTabPageBkgndBrush( CRhinoUiThemeDraw* pThemeDraw, bool bSpecial);
  bool DrawExpandableTabHeaderBkgnd( CDC& dc, const CRect& rect, CRhinoUiThemeDraw* pThemeDraw, bool bSpecial);
  bool DrawExpandableTabHeaderForegnd( CDC& dc, const CRect& rect, CRhinoUiThemeDraw* pThemeDraw, bool bSpecial, bool bHot, bool bExpanded, const CString& text);

  bool DrawGradientTitleRect( CDC* pDC, LPRECT lpRect, const wchar_t* lpsCaption, bool bActiveWindow, bool bVerticalRect = false);

  bool DrawTitleText( CDC* pDC, LPRECT lpRect, const wchar_t* lpsCaption, bool bActiveWindow, bool bVerticalRect = false);

private:
  class CRhUiWindowsPaintManagerColors* m_pColors = nullptr;
};

RHINO_SDK_FUNCTION CRhinoUiPaintManager::DrawExpanderHeaderOptions operator&(CRhinoUiPaintManager::DrawExpanderHeaderOptions a, CRhinoUiPaintManager::DrawExpanderHeaderOptions b);
RHINO_SDK_FUNCTION CRhinoUiPaintManager::DrawExpanderHeaderOptions operator|(CRhinoUiPaintManager::DrawExpanderHeaderOptions a, CRhinoUiPaintManager::DrawExpanderHeaderOptions b);
RHINO_SDK_FUNCTION CRhinoUiPaintManager::DrawExpanderHeaderOptions& operator|=(CRhinoUiPaintManager::DrawExpanderHeaderOptions& a, CRhinoUiPaintManager::DrawExpanderHeaderOptions b);
RHINO_SDK_FUNCTION CRhinoUiPaintManager::DrawExpanderHeaderOptions& operator&=(CRhinoUiPaintManager::DrawExpanderHeaderOptions& a, CRhinoUiPaintManager::DrawExpanderHeaderOptions b);
RHINO_SDK_FUNCTION CRhinoUiPaintManager::DrawExpanderHeaderOptions operator~(CRhinoUiPaintManager::DrawExpanderHeaderOptions a);
