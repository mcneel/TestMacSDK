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

class RHINO_SDK_CLASS CRhinoUiImageList : public CImageList
{
public:
  CRhinoUiImageList(void);
  ~CRhinoUiImageList(void);

public:
  bool LoadTrueColorImageList( UINT nID, int cx, bool grow, COLORREF crMask, AFX_MODULE_STATE* pPlugInModuleState = NULL);
  bool LoadTrueColorImageList( LPCTSTR lpszBitmapID, int cx, bool grow, COLORREF crMask, AFX_MODULE_STATE* pPlugInModuleState = NULL);
	bool DeleteTrueColorImageList(); // DO NOT call DeleteImageList() directly!!!!!

protected:
  AFX_MODULE_STATE* m_pPlugInModuleState;
};
