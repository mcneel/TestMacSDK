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

class RHINO_SDK_CLASS CRhinoUiDialogItemResizer
{
public:
  CRhinoUiDialogItemResizer( CWnd*);
  ~CRhinoUiDialogItemResizer(void);

  enum Modes
  {
    resize_lockleft        = 0x0001, // Distance to left is fixed
    resize_lockright       = 0x0002, // Distance to right is fixed
    resize_locktop         = 0x0004, // Distance to top is fixed
    resize_lockbottom      = 0x0008, // Distance to bottom is fixed
    resize_showhide        = 0x0010, // Show/hide if not fully visible

    resize_lockall         = resize_lockleft|resize_lockright|resize_locktop|resize_lockbottom,
    resize_locktopleft     = resize_lockleft|resize_locktop,
    resize_locktopright    = resize_lockright|resize_locktop,
    resize_lockbottomleft  = resize_lockbottom|resize_lockleft,
    resize_lockbottomright = resize_lockbottom|resize_lockright,
    resize_lockwidth       = resize_lockleft|resize_lockright,
    resize_lockheight      = resize_locktop|resize_lockbottom,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_modes = 0xFFFFFFFF
  };

	// Adds a control to the resize list
	void Add(CWnd *pControl, unsigned int uFlags);

	void Add( int nCtrlID, unsigned int uFlags);

	// Resizes the controls in the window
	void Resize( CWnd *pWnd);

	// Sets the original size of the parent
	void SetInitialSize( int cx, int cy);

	// Sets the original size of the parent
	void SetInitialSize(const CSize &size);

	// Returns the initial size
	CSize GetInitialSize() const;

protected:
  CWnd*     m_pOwner;
	ON_SimpleArray<void*> m_Controls;
	CSize     m_szInitial;
};
