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

/*
Description:
  Composite bitmaps created in CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs with final_dib.
Parameters:
  alpha_dib [in]  - Alpha map for all wireframe and curves.
  wires_dib [in]  - Rendered wireframes and curves in their display colors.
  final_dib [out] - alpha_dib and wires_dib will be transparently blit to this DIB.
Returns:
  Success == true, failure == false.
Comments:
  See: CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs
       CRhinoSdkRenderViewWnd::DrawWiresInDIB
       RhinoRenderDrawWiresInDIB( CRhinoDoc& doc, ON_3dmView view, CSize size, CRhinoDib& final_dib, bool bGetRenderSettingsFromDoc=true)
*/
RHINO_SDK_FUNCTION bool RhinoRenderDrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib);
/*
Description:
  Same as calling CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs( alpha, wires) then
  CRhinoSdkRenderViewWnd::DrawWiresInDIB( alpha, wires, my_dib);
Parameters:
  doc  [in] - Document to iterate
  view [in] - view to draw
  size [in] - size of image
  final_dib [out] - alpha_dib and wires_dib will be created, transparently blit to this DIB.
                    and destroyed.
  bGetRenderSettingsFromDoc [in] - Tells compositor to setup render engine using the settings
                                   from the Document (true) or Display (false) settings.
Returns:
  Success == true, failure == false.
Comments:
  See: CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs( CRhinoDib& dib1, CRhinoDib& dib2);
       CRhinoSdkRenderViewWnd::DrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib);
       RhinoRenderDrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib)
*/
RHINO_SDK_FUNCTION bool RhinoRenderDrawWiresInDIB( CRhinoDoc& doc, ON_3dmView view, const ON_2iSize& size, CRhinoDib& final_dib, bool bGetRenderSettingsFromDoc=true);
/*
Description:
  Get bitmaps containing curve and annotation which may be used to composite with a rendered bitmap
Parameters:
  doc  [in] - Document to iterate
  view [in] - view to draw
  size [in] - size of image
  alpha_dib[out] - Bitmap which defines which pixels contain wires
  wires_dib [out] - Wire/annotation pixel color
  bGetRenderSettingsFromDoc [in] - Tells compositor to setup render engine using the settings
                                   from the Document (true) or Display (false) settings.
Returns:
  Success == true, failure == false.
Comments:
  See: CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs( CRhinoDib& dib1, CRhinoDib& dib2);
       CRhinoSdkRenderViewWnd::DrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib);
       RhinoRenderDrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib)
*/
RHINO_SDK_FUNCTION bool RhinoRenderGetWireframeCompositeDIBs( CRhinoDoc& doc, ON_3dmView view, const ON_2iSize& size, CRhinoDib& alpha_dib, CRhinoDib& wires_dib, bool bGetRenderSettingsFromDoc);
