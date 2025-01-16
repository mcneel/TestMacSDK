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

// Use this class when you need to have the user interactively
// pick a viewport.
class RHINO_SDK_CLASS CRhinoGetView : public CRhinoGetPoint
{
public:
  CRhinoGetView() = default;
  ~CRhinoGetView() = default;

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Call GetView() to get a View.
  CRhinoGet::result GetView( );

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: If GetView() returned CRhinoGet::Point, 
  //         Call View() to get the View.
  //
  CRhinoView* View();

  //////////////////////////////////////////////////////////////////
  //
  // Overrides
  //
  void OnMouseMove( CRhinoViewport&, UINT, const ON_3dPoint&, const ON_2iPoint* ) override {}
  void DynamicDraw( CRhinoDisplayPipeline&, const ON_3dPoint& ) override {}

protected:
  CRhinoView* m_view = nullptr;
};

