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

#include "rhinoSdkGetPoint.h"

/*
CLASS
CArgsRhinoGetPlane
Input to the RhinoGetFrame() function
*/
class RHINO_SDK_CLASS CArgsRhinoGetFrame
{
public:
  CArgsRhinoGetFrame();
  ~CArgsRhinoGetFrame();
  CArgsRhinoGetFrame( const CArgsRhinoGetFrame& src);
  CArgsRhinoGetFrame& operator=( const CArgsRhinoGetFrame& src);

  // prompt when the object on which the frame is gotten
  const wchar_t* ObjectPrompt() const;
  void SetObjectPrompt( const wchar_t* prompt);

  // prompt when dragging the frame on the object
  const wchar_t* FramePrompt() const;
  void SetFramePrompt( const wchar_t* prompt);

  // Set or get the object on which the frame will be gotten
  ON_Object* ObjectOn() const;
  void SetObjectOn( ON_Object* pObj);

  BOOL32 HaveObjectOn() const;

  ON_Plane& Frame() const;

protected:
  // [in] prompt when getting object on which the frame will be
  ON_wString m_object_prompt;
  
  // [in] prompt when getting frame
  ON_wString m_frame_prompt;
  
  // [in] color to use when drawing dynamic lines while getting the frame
  ON_Color m_dynamic_line_color;

  // [in/out] Object on which the frame is gotten
  ON_Object* m_pObject;

  // [out] Return the frame
  ON_Plane m_frame;

};



/*
 CLASS
 CRhinoGetFrameOnObject
*/
class RHINO_SDK_CLASS CRhinoGetFrameOnObject : public CRhinoGetPoint
{
public:

  enum
  {
    nothing,
    curve,
    surface,
  };

  CRhinoGetFrameOnObject();
  ~CRhinoGetFrameOnObject();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: SetFramePrompt() to specify command prompt
  //         for dragging the frame
  void SetFramePrompt( const char* p1);
  void SetFramePrompt( const wchar_t* p1);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1a: Use SetObjectOnPrompt() to specify a prompt for picking 
  //          the object on which the frame will be gotten
  //
  void SetObjectOnPrompt( const char* p1);
  void SetObjectOnPrompt( const wchar_t* p1);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Set the object on which the frame will be gotten.
  //
  BOOL32 SetObjectOn( const ON_Object* pObject);
  void ClearObjectOn();
  BOOL32 HaveObjectOn() const;

  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Call GetFrame() to get the frame.
  //
  CRhinoGet::result GetFrame();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 6: If GetFrame() returned CRhinoGet::frame, 
  //         Call Frame() to get the frame chosen.
  //
  const ON_Plane& Frame() const;


protected:

  void OnMouseMove( 
          CRhinoViewport& vp,
          UINT nFlags,
          const ON_3dPoint& dragpoint,  // current 3d world location
          const ON_2iPoint* pScreenpoint     // current window 2d mouse location (NULL for 3d digitizer arms)
          );
  void DynamicDraw( CRhinoDisplayPipeline& dp,
                    const ON_3dPoint& dragpoint // current mouse/digitizer location
                  );

  
  CRhinoViewport* m_lastvp;
  int m_object_type;
  ON_Plane m_frame;
  BOOL32 m_drawframe;
  const ON_Object* m_pObject;
  ON_wString m_object_prompt;
  ON_wString m_frame_prompt;
};

