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

////////////////////////////////////////////////////////////////
// In-place control events:
// - VK_RETURN, VK_ESCAPE -> IPEK_KEY
// - Button click -> IPEK_ACTION
// - WM_KILLFOCUS ->IPEK_FOCUS

#if defined(ON_RUNTIME_WIN)
////////////////////////////////////////////////////////////////
//
// CRhinoUiInPlaceEdit
//
class RHINO_SDK_CLASS CRhinoUiInPlace
{
public:
  static LPCTSTR MsgString();
  enum
  {
    ipek_key    = 1,
    ipek_action = 2,
    ipek_focus  = 3,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_in_place = 0xFFFFFFFF
  };

  typedef struct tagIPCENDEDITINFO
  {
    HWND  hwndSrc;    // Event source
    UINT  uKind;      // Event (key press, focus change, etc )
#if defined (ON_RUNTIME_WIN)
// this union produces an error with the clang compiler on OS X, and is not needed in Mac Rhino.
    union 
    {
      UINT  uID;      // Control ID (for actions)
      UINT  nVirtKey; // Virtual key
      HWND  hNewWnd;  // New window with focus
    };
#endif
    POINT pt;         // Event point, in screen coordinates
  } IPCENDEDITINFO, FAR *LPIPCENDEDITINFO;

  static UINT const m_IPCMsg;
};
#endif

////////////////////////////////////////////////////////////////
//
// CRhinoUiInPlaceEdit
//
class RHINO_SDK_CLASS CRhinoUiInPlaceEdit: public CEdit
{
  DECLARE_DYNCREATE( CRhinoUiInPlaceEdit )

public:
  CRhinoUiInPlaceEdit();
  virtual ~CRhinoUiInPlaceEdit();
  
  void Show( CRect rcEdit );
  void Hide();
  
  void SetParentMessageWnd(HWND hWnd);
private:
  HWND m_hParent;
  
public:
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  afx_msg void OnKillFocus( CWnd *pNewWnd );
  afx_msg UINT OnGetDlgCode();
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
};

////////////////////////////////////////////////////////////////
//
// CRhinoUiInPlaceButton
//
class RHINO_SDK_CLASS CRhinoUiInPlaceButton: public CButton
{
   DECLARE_DYNCREATE( CRhinoUiInPlaceButton )

public:
   CRhinoUiInPlaceButton();
   virtual ~CRhinoUiInPlaceButton();

   void Show(CRect rcButton);
   void Hide();

private:
   HWND m_hParent;
   BOOL32 m_bInAction;

private:
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnClick();
protected:
   DECLARE_MESSAGE_MAP()
public:
   virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
   virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
};
