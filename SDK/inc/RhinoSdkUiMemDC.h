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

class RHINO_SDK_CLASS CRhinoUiMemDC : public CDC
{
public:
  CRhinoUiMemDC( CDC* pDC, const CRect* pRect = NULL);
  ~CRhinoUiMemDC(void);

  CRhinoUiMemDC* operator->();
  operator CRhinoUiMemDC*();

  COLORREF MaskingColor() const;
  void SetMaskingColor( COLORREF cr);

private:       
  CBitmap m_bitmap;     // Offscreen bitmap
  CBitmap* m_oldBitmap; // bitmap originally found in CMemDC
  CDC* m_pDC;           // Saves CDC passed in constructor
  CRect m_rect;         // Rectangle of drawing area.
  bool m_bMemDC;        // TRUE if CDC really is a Memory DC.
  COLORREF m_crMask;    // If set to ON_UNSET_COLOR the destructor will bit blit the bitmap
                        // to m_pDC.  If set to a color other than ON_UNSET_COLOR then the
                        // bitmap will transparent blit to m_pDC using m_crMask as the
                        // masking color
};
