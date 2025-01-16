//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
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

class RHINO_SDK_CLASS CRhinoDisplayPipeline_GDI : public CRhinoDisplayPipeline
{
  DECLARE_DYNAMIC_PIPELINE(CRhinoDisplayPipeline_GDI);
public:
  CRhinoDisplayPipeline_GDI();
#if defined (ON_RUNTIME_APPLE)
  CRhinoDisplayPipeline_GDI(CRhinoDoc* pDoc);
#endif
  CRhinoDisplayPipeline_GDI(class CRhVectorEngine*, CRhinoViewport&);
  CRhinoDisplayPipeline_GDI(CRhinoView&, CRhinoViewport&);
  CRhinoDisplayPipeline_GDI(HDC, CRhinoViewport&);

  virtual ~CRhinoDisplayPipeline_GDI();

public:  
  bool SetupFrustum(const ClippingInfo&) override;          
  bool DrawBackground(void) override;
  bool DrawMiddleground(void) override;
  bool DrawForeground(void) override;

  const ON_ClassId* EngineClass(void) override;
  void DrawTexturedSurface(const CRhinoObject* rhinoObject);
private:
  void DrawTexturedSurfaces( const DisplayBucket& bucket );
  ON__UINT_PTR m_gdi_reserved = 0;
};
