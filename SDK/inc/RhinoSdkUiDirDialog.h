//
// Copyright (c) 1993-2015 Robert McNeel & Associates. All rights reserved.
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

/////////////////////////////////////////////////////////////////////////////
// CRhinoUiDialogSub

class RHINO_SDK_CLASS CRhinoUiDialogSub : public CWnd
{
  DECLARE_DYNAMIC(CRhinoUiDialogSub)

public:
  CRhinoUiDialogSub();
  virtual ~CRhinoUiDialogSub();

protected:
  friend class CRhinoUiDirDialog;
  class CRhinoUiDirDialog* m_pPathDialog;

protected:
  afx_msg void OnOK();              // OK button clicked
  afx_msg void OnChangeEditPath();

protected:
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CRhinoUiDirDialog dialog

class RHINO_SDK_CLASS CRhinoUiDirDialog
{
  friend class CRhinoUiDialogSub;

  // Construction
public:
  CRhinoUiDirDialog( LPCTSTR lpszCaption = NULL, LPCTSTR lpszTitle = NULL,
                     LPCTSTR lpszInitialPath = NULL, CWnd* pParent = NULL);

  virtual ~CRhinoUiDirDialog();

  CString GetPathName();

  enum ErrorCodes
  {
    eFolderDoesNotExist,
    eInvalidPath,
    eCreatePath, // return IDYES to create folder
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_error_codes = 0xFFFFFFFF
  };

  // Override the following to localize error messages
  virtual int ErrorMessage( ErrorCodes ec, LPCTSTR lpsFileName) const;
  virtual int DoModal();


private:
  static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData);

  CString m_sCaption;
  CString m_sTitle;
  CString m_sInitialPath;

#if defined (ON_RUNTIME_WIN)
  TCHAR m_szPathName[MAX_PATH];

  BROWSEINFO m_bi;
  HWND m_hWnd;
  CWnd*	m_pParentWnd;
  bool m_bParentDisabled;
  bool m_bGetSuccess;

  CRhinoUiDialogSub m_PathDialogSub;
#endif
#if defined (ON_RUNTIME_APPLE)
  CString m_szPathName;
#endif
};
