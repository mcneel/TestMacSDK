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

// CRhinoUiProgressBar
#if defined(_MSC_VER)
#pragma warning( push )
// C4263: 'function' : member function does not override any base class virtual member function
// C4264: 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
// C4265: 'class' : class has virtual functions, but destructor is not virtual
// C4266: 'class' : class has virtual functions, but destructor is not virtual
#pragma warning( disable : 4263 4264 4265 4266 )
#endif
class RHINO_SDK_CLASS CRhinoUiProgressBar : public CRhinoUiProgressCtrl
{
	DECLARE_DYNAMIC(CRhinoUiProgressBar)

public:
	CRhinoUiProgressBar( HWND hMainWnd = NULL);
	CRhinoUiProgressBar( HWND hMainWnd, const char* lpMessage, int nSize=100, 
                        int nMaxValue=100, bool bShow=false, int nPane=0);
	CRhinoUiProgressBar( HWND hMainWnd, const wchar_t* lpMessage, int nSize=100, 
                        int nMaxValue=100, bool bShow=false, int nPane=0);
	virtual ~CRhinoUiProgressBar();

public:
  bool Create( const char* lpMessage, int nSize=100, int nMaxValue=100, bool bShow=false, int nPane=0);
  bool Create( const wchar_t* lpMessage, int nSize=100, int nMaxValue=100, bool bShow=false, int nPane=0);
  // Sets the message to display to left of control
  bool SetMessage(const char* lpMessage);
  bool SetMessage(const wchar_t* lpMessage);
  // Sets the size of the progress bar control
  bool SetSize(int nSize);
  // Clears the progress bar control and restores the status bar.
  void Clear();
protected:
  bool _Create( LPCTSTR lpMessage, int nSize=100, int nMaxValue=100, bool bShow=false, int nPane=0);
  // Called when the progress bar needs to be resized
	bool Resize();
  // Retrieves a pointer to the status bar
  CStatusBar* GetStatusBar();
protected:
	int		  m_nSize;       // Percentage size of control
	int		  m_nPane;       // ID of status bar pane progress bar will appear in
	CString	m_strMessage;  // Message to display to left of control
  CString m_strPrevText; // Previous text in status bar
	CRect   m_Rect;        // Dimensions of the whole thing
  CWnd*   m_pMainWnd;    // Pointer to main window

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
};
#if defined(_MSC_VER)
#pragma warning( pop )
#endif


