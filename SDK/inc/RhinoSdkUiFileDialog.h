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

// 2 Jan. 2014 - S. Baer
// No longer used. If you were using this class, just use a CFileDialog
// or use the SDK CRhinoGetFileDialog class

/*
class RHINO_SDK_CLASS CRhinoUiFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CRhinoUiFileDialog)

public:
	CRhinoUiFileDialog( BOOL32 bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		                  LPCTSTR lpszDefExt = NULL,
		                  LPCTSTR lpszFileName = NULL,
		                  DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                  LPCTSTR lpszFilter = NULL,
		                  CWnd* pParentWnd = NULL
                    );
  virtual ~CRhinoUiFileDialog();

protected:
	DECLARE_MESSAGE_MAP()
};
*/
