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

class RHINO_SDK_CLASS CRhinoMouseCallback
{
public:
  CRhinoMouseCallback() = default;
  virtual ~CRhinoMouseCallback();

  // When constructed, the mouse callback is not enabled.  After
  // Enable(true) is called, all mouse event pass through the callback.
  // Calling Enable(false) or destroying the class disables the callback.
  void Enable(bool enabled);

  // Returns TRUE if the mouse callback is enabled.
  bool IsEnabled() const;

  enum mouse_button
  {
    no_mouse_button     = 0,
    left_mouse_button   = 1,
    right_mouse_button  = 2,
    middle_mouse_button = 3,
    x_mouse_button      = 4
  };


  // Called at the beginning of CRhinoView processing a mouse down event
  virtual void OnBeginMouseDown(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool& executeDefaultHandler);

  // Called at the end of CRhinoView processing a mouse down event
  virtual void OnMouseDown(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted);

  // Called at the beginning of CRhinoView processing a mouse move event.
  virtual void OnBeginMouseMove(unsigned int viewSerialNumber, UINT flags, const ON_2iPoint& point, bool& executeDefaultHandler);

  // Called at the end of CRhinoView processing a mouse move event
  virtual void OnMouseMove(unsigned int viewSerialNumber, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted);

  // Called at the beginning of CRhinoView processing a mouse up event
  virtual void OnBeginMouseUp(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool& executeDefaultHandler);

  // Called at the end of CRhinoView processing a mouse up event
  virtual void OnMouseUp(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted);

  //////////
  // Override if you want to handle mouse double click events.
  // If you return TRUE, the default handler is not called.
  // If you return FALSE, the default OnMouseDblClick() handler 
  // is called.
  virtual BOOL32 OnMouseDblClk( 
          CRhinoView&,
          CRhinoMouseCallback::mouse_button, 
          UINT, // WM_*BUTTONDBLCLK message wParam flags
          const ON_2iPoint&
          );

protected:
  class CRhMouseCallbackManager* m_cbm = nullptr;

private:
  // no implementation
  CRhinoMouseCallback& operator=(const CRhinoMouseCallback&) = delete;
  CRhinoMouseCallback(const CRhinoMouseCallback&) = delete;
};

// Same as a CWaitCursor with the difference that no wait cursor
// is applied when Rhino is "headless"
class RHINO_SDK_CLASS CRhinoWaitCursor
{
public:
  CRhinoWaitCursor();
  ~CRhinoWaitCursor();
private:
  class CRhWaitCursor* m_private = nullptr;
};
