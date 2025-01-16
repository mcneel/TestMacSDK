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


// CRhinoUiEditInt

// THIS IS PROVIDED FOR BACKWARDS COMPATIBILITY ONLY!!
// This class has been replaced by CRhinoUiEdit and will be removed in
// the next version of Rhino

class RHINO_SDK_CLASS CRhinoUiEditInt : public CRhinoUiEdit
{
	DECLARE_DYNAMIC(CRhinoUiEditInt)

public:
	CRhinoUiEditInt();
	virtual ~CRhinoUiEditInt();

protected:
	DECLARE_MESSAGE_MAP()
};
