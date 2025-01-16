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


// CRhinoUiRichEditCtrl

class RHINO_SDK_CLASS CRhinoUiRichEditCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRhinoUiRichEditCtrl)

public:
	CRhinoUiRichEditCtrl();
	virtual ~CRhinoUiRichEditCtrl();

// DO NOT USE THESE FUNCTIONS
//
// These functions use TCHAR, and char string types,
// which means that Unicode text will get garbled.
//
// Instead, use the corresponding RmaUnicode... functions in the public section
// below
//
private:
  // Use RmaSetUnicodeText(...);
  void SetWindowText(LPCTSTR lpszString);
  // Use RmaGetUnicodeText(...);
	int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
  // Use RmaGetUnicodeTextLength(...);
  void GetWindowText(CString& rString) const;

public:
  int RmaGetUnicodeText( wchar_t* psText, int nSize) const;
  int RmaGetUnicodeText( ON_wString& wText) const;
  ON_wString RmaGetUnicodeText() const;
  int RmaGetUnicodeTextLength() const;
	void RmaSetUnicodeText( const wchar_t* sText, bool bAppend = false);
  int RmaUnicodeGetSelText( wchar_t* sText, int nSize) const;
  int RmaUnicodeGetSelText( ON_wString& wText) const;
  void RmaUnicodeReplaceSel( const wchar_t* sText );

  //
  // Description:
  //    DoDataExchange method
  //    Replace the DDX_Text(....) function call in your dialog class
  //    with m_?my_control?.DDX_Text( ...)
  //
  // Example:
  //    Header file contains:
  //       CRhinoUiRichEditCtrl m_wCtrl;
  //       ON_wString           m_wValue;
  //   CPP file would contain:
  //    void CTestDialog::DoDataExchange( CDataExchange* pDX)
  //    {
	//        CRhinoUiDialog::DoDataExchange(pDX);
	//        //{{AFX_DATA_MAP(CTestDialog)
	//        DDX_Control(pDX, IDC_EDIT1, m_wCtrl);
	//        //}}AFX_DATA_MAP
	//        m_wCtrl.DDX_Text(pDX, IDC_EDIT1, m_wValue);
  //     }
  //
  // Parameters:
  //   pDX [in] A pointer to a CDataExchange object. The framework supplies this object to establish
  //            the context of the data exchange, including its direction. 
  //   nIDC [in] The ID of an edit control in the dialog box, form view, or control view object. 
  //   wValue [in] A reference to a data member in the dialog box, form view, or control view object.
  //
  // See Also:
  //   DDX_Text MFC macro
  //   
  //
  void DDX_Text( CDataExchange* pDX, int nIDC, ON_wString& wValue);
  // For backwards compatibility
  ON_wString GetText() const;

  virtual int printf( const wchar_t* format, ...);

  int CreateFrom( CEdit *CopyControl);

  bool EnableContextMenu( bool b);
  bool ContextMenuEnabled() const;

protected:
	CHARRANGE m_SelRange;
  bool m_bContextMenu;
private:
  CFont m_font;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnContextMenu( CWnd* pWnd, CPoint pt);
  LRESULT OnPaste( WPARAM wParam, LPARAM lParam );
  virtual BOOL32 PreTranslateMessage( MSG* pMsg );
};


class RHINO_SDK_CLASS CRhinoUiRichTextEditCtrl : public CRhinoUiRichEditCtrl
{
	DECLARE_DYNAMIC(CRhinoUiRichTextEditCtrl)

public:
	CRhinoUiRichTextEditCtrl();
	virtual ~CRhinoUiRichTextEditCtrl();

  //  Description:
  //    DoDataExchange method
  //    Replace the DDX_Text(....) function call in your dialog class
  //  Use as CRhinoUiRichEditCtrl::DDX_Text above but for Rhino Rich Text strings.
  void DDX_RhinoRichText( CDataExchange* pDX, int nIDC, ON_wString& wValue);

  void RmaSetRichText( const wchar_t* sText, bool bAppend = false);
  void RmaGetRichText( ON_wString& str);
  ON_wString RmaGetRichText();

  // Set the default properties to use for RichText display
  // The height and width factors are relative to the parent object's text height
  // and since text has only one basic height in the dialog, that doesn't get 
  // considered here.
  void InitializeProperties( const LOGFONT& logfont);

  CHARFORMAT m_charformat;
};