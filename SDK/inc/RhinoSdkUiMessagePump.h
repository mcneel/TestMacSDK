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

class RHINO_SDK_CLASS CRhinoUiMessagePump  
{
public:
	CRhinoUiMessagePump( CWinApp& win_app);
	virtual ~CRhinoUiMessagePump();

  enum modal_result
  {
    continue_modal = 0,
    continue_modal_msg,
    continue_modal_wm_quit,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_modal_result = 0xFFFFFFFF
  };
  modal_result DoModal();
  virtual BOOL32 ContinueModal() = 0;
  virtual BOOL32 ContinueModal( const MSG& msg) = 0;

private:
  CWinApp& m_win_app;
};

