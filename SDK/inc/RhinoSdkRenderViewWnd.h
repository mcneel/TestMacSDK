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

#if defined (RHINO_SDK_MFC) && defined (ON_RUNTIME_WIN)

/////////////////////////////////////////////////////////////////////////////
// CRhinoSdkRenderViewWnd window
class RHINO_SDK_CLASS CRhinoSdkRenderViewWnd : public CWnd
{
// Construction
public:
	CRhinoSdkRenderViewWnd( CRhinoSdkRenderFrame* frame);

  /*
  Description:
    Called after window is created and just prior to showing window.
  */
  virtual void OnInitialUpdate();
  /*
  Description:
    Called by PostNcDestroy.
  Returns:
    If true is returned PostNcDestroy will call "delete this"
  */
  virtual bool DeleteOnPostNcDestroy() const;


// Attributes
public:
  const ON_3dmView& View() const;

// Operations
public:
  /*
  Description:
    Create DIBs used to transparently blit wires to output render DIB.  The DIB references
    passed to this function will contain bitmaps need when calling 
    CRhinoSdkRenderViewWnd::DrawWiresInDIB( alpha_dib, wires_dib, final_dib).  If alpha_dib
    or wires_dib contain an existing bitmap the existing bitmap will be destroyed and a new
    one created.  This all dibs will be created using the specified render output size.
  Parameters:
    alpha_dib [out] - Alpha map for all wireframe and curves.
    wires_dib [out] - Rendered wireframes and curves in their display colors.
    bGetRenderSettingsFromDoc [in] - Tells compositor to setup render engine using the settings
                                     from the Document (true) or Display (false) settings.
  Returns:
    true If DIBs are created successfully otherwise returns false.
  Comments:
    See: CRhinoSdkRenderViewWnd::DrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib)
  */
  bool GetWireframeCompositeDIBs( CRhinoDib& dib1, CRhinoDib& dib2, bool  bGetRenderSettingsFromDoc=true);
  /*
  Description:
    Composite bitmaps created in CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs with final_dib.
  Parameters:
    alpha_dib [in]  - Alpha map for all wireframe and curves.
    wires_dib [in]  - Rendered wireframes and curves in their display colors.
    final_dib [out] - alpha_dib and wires_dib will be transparently blit to this DIB.
  Returns:
    Success == true, failure == false.
  Comments:
    See: CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs
  */
  bool DrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib);
  /*
  Description:
    Same as calling CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs( alpha, wires) then
    CRhinoSdkRenderViewWnd::DrawWiresInDIB( alpha, wires, my_dib);
  Parameters:
    final_dib [out] - alpha_dib and wires_dib will be created, transparently blit to this DIB.
                      and destroyed.
    bGetRenderSettingsFromDoc [in] - Tells compositor to setup render engine using the settings
                                     from the Document (true) or Display (false) settings.
  Returns:
    Success == true, failure == false.
  Comments:
    See: CRhinoSdkRenderViewWnd::GetWireframeCompositeDIBs( CRhinoDib& dib1, CRhinoDib& dib2);
         CRhinoSdkRenderViewWnd::DrawWiresInDIB( CRhinoDib& alpha_dib, CRhinoDib& dib2, CRhinoDib& final_dib);
  */
  bool DrawWiresInDIB( CRhinoDib& final_dib, bool  bGetRenderSettingsFromDoc=true);

  // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRhinoSdkRenderViewWnd)
public:
	virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRhinoSdkRenderViewWnd();
  // for internal use only!
  void OnFrameDestruction();
	// Generated message map functions
protected:
  CRhinoSdkRenderFrame* m_wndFrame;
  HWND                  m_hWndFrame;

	//{{AFX_MSG(CRhinoSdkRenderViewWnd)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

#endif

