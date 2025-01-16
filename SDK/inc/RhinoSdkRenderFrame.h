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

#include "RhinoSdkRenderViewWnd.h"


#pragma once
// RhinoSdkRenderFrame.h : header file
//

// This is the new, modern, MFC free version of the render window class. The old CRhinoSdkRenderFrame
// class can access this interface through the RenderFrame() functions. This will provide the encapsulation
// that allows non-MFC windows to be used without hacks in Rhino 6.8 onwards.
class RHINO_SDK_CLASS IRhinoSdkRenderFrame : public IRhinoWindow
{
protected:
	IRhinoSdkRenderFrame();
	virtual ~IRhinoSdkRenderFrame();

	//This is for implementation only, to ensure that the correct runtime is used for
	//memory deallocation.  This function cannot be called from anywhere else than the implementation for IRhinoWindow::Release.
	virtual void Delete(void) = 0;

public:
	virtual const ON_3dmView& View() const = 0;
	virtual void SetView(const ON_3dmView& view) = 0;

	virtual ON_2iSize ImageSize() const = 0;
	virtual void SetImageSize(const ON_2iSize&) = 0;

	virtual void SetPlugIn(CRhinoRenderPlugIn* pPlugIn) = 0;
	virtual void SetRenderCommand(CRhinoSdkRender* pRenderCommand) = 0;

	virtual bool StartRendering() = 0;
	virtual bool IsRendering() const = 0;
	virtual void StopRendering() = 0;

	virtual void SetSaveOnClose(bool bSave) = 0;

	virtual void MakeModal(bool bModal) = 0;

	virtual bool CopyRenderWindowToClipboard() const = 0;
	virtual bool SaveRenderImageAs(const wchar_t* lpsFileName) const = 0;

	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

public:
	static class IRhinoSdkRenderFrame* TopRenderFrame(const CRhinoDoc&);
	static ON_SimpleArray<class IRhinoSdkRenderFrame*> RenderFrames(const CRhinoDoc&);
};

//Provides direct access to the RDK render window and session.  Naturally, these functions will
//only return valid pointers if the render frame is actually an RDK render window, or if it is a 
//complete implementation that exposes these two objects.
class RHINO_SDK_CLASS IRhinoSdkRenderFrameEx : public IRhinoSdkRenderFrame
{
public:
	virtual class IRhRdkRenderWindow* RenderWindow(void) = 0;
	virtual const class IRhRdkRenderWindow* RenderWindow(void) const = 0;

	virtual class IRhRdkRenderSession* RenderSession(void) = 0;
	virtual const class IRhRdkRenderSession* RenderSession(void) const = 0;
};

//Alpha support added when saving rendered image.
class RHINO_SDK_CLASS IRhinoSdkRenderFrameEx2 : public IRhinoSdkRenderFrameEx
{
public:
  using IRhinoSdkRenderFrame::SaveRenderImageAs;
  virtual bool SaveRenderImageAs(const wchar_t* lpsFileName, bool bUseAlpha) const = 0;
};

// Added support for picking.
class RHINO_SDK_CLASS IRhinoSdkRenderFrameEx3 : public IRhinoSdkRenderFrameEx2
{
public:
  enum class PickTypes // Used by PickPointOnImage()
  {
    Color,    // Picking a color.
    Distance, // Picking a distance.
  };

  virtual bool PickPointOnImage(PickTypes type, int& xOut, int& yOut) = 0;
  virtual bool PickRectangleOnImage(ON_4iRect& rectOut) = 0;
};

class CRhinoSdkRender;

/////////////////////////////////////////////////////////////////////////////
// CRhinoSdkRenderFrame frame

#if defined (RHINO_SDK_MFC) && defined (ON_RUNTIME_WIN)

class RHINO_SDK_CLASS CRhinoSdkRenderFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CRhinoSdkRenderFrame)
	CRhinoSdkRenderFrame();           // protected constructor used by dynamic creation
public:
	CRhinoSdkRenderFrame( CRhinoDoc* doc);

protected:
	virtual ~CRhinoSdkRenderFrame();

public:
	// Access to the new IRhinoSdkRenderFrame interface.
	      IRhinoSdkRenderFrame& RenderFrame(void);
	const IRhinoSdkRenderFrame& RenderFrame(void) const;

// Attributes
public:
  const ON_3dmView& View() const{ return m_view;}
  void SetView(const ON_3dmView& view) { m_view = view; }
  ON_2iSize ImageSize() const { return m_sizeImage;}
  void SetImageSize(const ON_2iSize& size) { m_sizeImage = size; }
  CRhinoSdkRender* RhinoRender() const { return m_render_command;}
  CRhinoRenderPlugIn* RhinoRenderPlugIn() const { return m_render_plug_in;}
  HWND RhinoMainFrame(){ return m_hWndRhinoMainFrame;}
  void InternalSetIcon(HICON icon) { m_sdk_window_icon = icon;  }
  void InternalSetPlugIn(CRhinoRenderPlugIn* pPlugIn) { m_render_plug_in = pPlugIn; }
  void InternalSetRenderCommand(CRhinoSdkRender* p) { m_render_command = p; }

// Operations
public:
  /*
  Description:
    Set status bar message text.
  Parameters:
    lpszText [in] - Text to display in status bar.  If NULL then status bar
                    will be blanked out.
  */
	void RhinoSetMessageText( const wchar_t* lpszText);
  // for internal use only
  void OnViewWndDestruction();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRhinoSdkRenderFrame)
public:
	virtual BOOL32 OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

  // Must override
public:
  virtual CRhinoSdkRenderViewWnd* NewRenderViewWnd( CRhinoSdkRenderFrame* frame);
  virtual BOOL32 StartRendering();
  virtual BOOL32 IsRendering();
  virtual void StopRendering();
  virtual BOOL32 CopyRenderWindowToClipboard();
  virtual BOOL32 SaveRenderImageAs( const wchar_t* lpsFileName);

  CRhinoDoc& Document();

public:
  class CRhSdkRenderFrameSdkExtension* m__render_frame_sdk_extension;

// Implementation
protected:
  friend class CRhinoSdkRender;
  friend class CRhinoSdkRenderEx;
  friend class CRhRenderWindowManager;
  friend class IRhinoSdkRenderFrame;

  void Construct();

  CRhinoSdkRenderViewWnd* m_wndView;
  HWND                    m_hWndView;
  CRhinoSdkRender*        m_render_command;
  CRhinoRenderPlugIn*     m_render_plug_in;
  CStatusBar            m_wndStatusBar;
  ON_2iSize               m_sizeImage;
  CRhinoDoc*              m_pDoc;
  ON_3dmView              m_view;
  BOOL32                    m_do_not_prompt_for_save;
  HWND                    m_hWndRhinoMainFrame;
  BOOL32                    m_minimize_enable;
  HICON                   m_sdk_window_icon;
  
	// Generated message map functions
	//{{AFX_MSG(CRhinoSdkRenderFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL32 bShow, UINT nStatus);
	//}}AFX_MSG
  afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
#if _MSC_VER < 1300
  afx_msg void RhinoSdkRenderFrameOnWmUser( WPARAM wParam, LPARAM lParam);
#else
  afx_msg LRESULT RhinoSdkRenderFrameOnWmUser( WPARAM wParam, LPARAM lParam);
#endif

	DECLARE_MESSAGE_MAP()

	// No longer used.
	virtual bool IsRDKRenderFrame(void) const;
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

class RHINO_SDK_CLASS CRhinoSdkRenderFrame2 : public CRhinoSdkRenderFrame
{
public:
  CRhinoSdkRenderFrame2(CRhinoDoc* pDoc);
  virtual ~CRhinoSdkRenderFrame2();

public:
  using CRhinoSdkRenderFrame::SaveRenderImageAs;
  virtual bool SaveRenderImageAs(const wchar_t* lpsFileName, bool bUseAlpha);
};

class RHINO_SDK_CLASS CRhinoSdkRenderFrame3 : public CRhinoSdkRenderFrame2
{
public:
  CRhinoSdkRenderFrame3(CRhinoDoc* pDoc);
  virtual ~CRhinoSdkRenderFrame3();

public:
  virtual bool PickPointOnImage(IRhinoSdkRenderFrameEx3::PickTypes type, int& x, int& y);
  virtual bool PickRectangleOnImage(ON_4iRect& rectOut);
};

#endif
