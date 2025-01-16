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


// CRhinoUiExpandableTabbedDockBar

class RHINO_SDK_CLASS CRhinoUiExpandableTabbedDockBar : public CRhinoUiDockBar
{
	DECLARE_DYNAMIC(CRhinoUiExpandableTabbedDockBar)

public:
	CRhinoUiExpandableTabbedDockBar();
	virtual ~CRhinoUiExpandableTabbedDockBar();

protected:
	DECLARE_MESSAGE_MAP()

protected:
  UUID m_dock_bar_uuid;
};
