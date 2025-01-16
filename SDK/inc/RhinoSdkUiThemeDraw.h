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

#if !defined (ON_RUNTIME_WIN)
class CRhinoUiThemeDraw;
#endif


#if defined (ON_RUNTIME_WIN)

#ifndef THEMEAPI
typedef HANDLE HTHEME;
#endif

class RHINO_SDK_CLASS CRhinoUiThemeDraw
{
public:
  CRhinoUiThemeDraw( HWND hWnd, LPCWSTR pszClassList);
  ~CRhinoUiThemeDraw();

  bool IsInitialized();

  bool LoadShellStyleDLL();
  HMODULE ShellStyleDLL() const;
  bool ExtractShellStyleResource( LPCTSTR lpsName, LPCTSTR lpsType, CStringA& result);
  bool ExtractShellStyleBitmap( LPCTSTR lpsName, CRhinoDib& result);

  bool DrawThemeBackground( HDC hdc, int iPartId, int iStateId, const RECT *pRect,const RECT *pClipRect);
  bool DrawThemeText( HDC hdc, int iPartId, int iStateId, LPCTSTR pszText, int iCount, DWORD dwTextFlags, BOOL32 bGrayedOut, const RECT *pRect);
  bool DrawThemeText( HDC hdc, int iPartId, int iStateId, const CString& str, DWORD dwTextFlags, BOOL32 bGrayedOut, const RECT *pRect);
  bool DrawThemeText( HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT *pRect);

  bool DrawThemeEdge( HDC hdc, int iPartId, int iStateId, const RECT *pDestRect, UINT uEdge, UINT uFlags, RECT *pContentRect);
  bool DrawThemeIcon( HDC hdc, int iPartId, int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex);

  bool GetThemeBackgroundContentRect( HDC hdc, int iPartId, int iStateId, const RECT *pBoundingRect, RECT *pContentRect);

  bool GetThemeColor( int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor);
  bool GetThemeMetric( OPTIONAL HDC hdc, int iPartId, int iStateId, int iPropId, OUT int *piVal);
  bool GetThemeString( int iPartId, int iStateId, int iPropId, OUT LPWSTR pszBuff, int cchMaxBuffChars);
  bool GetThemeBool( int iPartId, int iStateId, int iPropId, OUT BOOL32 *pfVal);
  bool GetThemeInt( int iPartId, int iStateId, int iPropId, OUT int *piVal);
  bool GetThemeEnumValue( int iPartId, int iStateId, int iPropId, OUT int *piVal);
  bool GetThemeFont( OPTIONAL HDC hdc, int iPartId, int iStateId, int iPropId, OUT LOGFONT *pFont);
  bool GetThemeRect( int iPartId, int iStateId, int iPropId, OUT RECT *pRect);
  bool SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
  bool GetThemeFilename( int iPartId, int iStateId, int iPropId, OUT LPWSTR pszThemeFileName, int cchMaxBuffChars);
  COLORREF GetThemeSysColor( int iColorId);
  HBRUSH GetThemeSysColorBrush( int iColorId);
  bool GetThemeSysFont( int iFontId, OUT LOGFONT *plf);
  bool GetThemeSysString( int iStringId, OUT LPWSTR pszStringBuff, int cchMaxStringChars);
  bool IsAppThemed();
  HTHEME GetWindowTheme( HWND hwnd);
  bool GetCurrentThemeName( CStringW& sThemeFileName, CStringW& sColorBuff, CStringW& sSizeBuff);

  bool IsThemeBackgroundPartiallyTransparent( int iPartId, int iStateId);
  bool DrawThemeParentBackground( HWND hwnd, HDC hdc, OPTIONAL RECT* prc);

  operator HTHEME()
  {
    if( m_hThemeData)
      return m_hThemeData;
    return NULL;
  }

private:
  void Initialize();
  HTHEME OpenThemeData( HWND hwnd, LPCWSTR pszClassList);
  HRESULT CloseThemeData();

  HWND m_hWnd;
  HTHEME m_hThemeData;
  LPCWSTR m_pszClassList;
  HMODULE m_hShellStyleDLL;

  typedef HRESULT (__stdcall *PFNCLOSETHEMEDATA)(HTHEME hTheme);
  typedef HRESULT (__stdcall *PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect,  const RECT *pClipRect);
  typedef HTHEME  (__stdcall *PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
  typedef HRESULT (__stdcall *PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
                                                 DWORD dwTextFlags2, const RECT *pRect);
  typedef HRESULT (__stdcall *PFNGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME hTheme,  HDC hdc, int iPartId, int iStateId,  const RECT *pBoundingRect, 
                                                                 RECT *pContentRect);
  typedef HRESULT (__stdcall *PFNDRAWTHEMEEDGE)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pDestRect, UINT uEdge, UINT uFlags, OPTIONAL OUT RECT *pContentRect);
  typedef HRESULT (__stdcall *PFNDRAWTHEMEICON)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex);

  typedef HRESULT (__stdcall *PFNGETTHEMECOLOR)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT COLORREF *pColor);
  typedef HRESULT (__stdcall *PFNGETTHEMEMETRIC)(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, int iStateId, int iPropId, OUT int *piVal);
  typedef HRESULT (__stdcall *PFNGETTHEMESTRING)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT LPWSTR pszBuff, int cchMaxBuffChars);
  typedef HRESULT (__stdcall *PFNGETTHEMEBOOL)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT BOOL32 *pfVal);
  typedef HRESULT (__stdcall *PFNGETTHEMEINT)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT int *piVal);
  typedef HRESULT (__stdcall *PFNGETTHEMEENUMVALUE)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT int *piVal);
  typedef HRESULT (__stdcall *PFNGETTHEMEFONT)(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, int iStateId, int iPropId, OUT LOGFONT *pFont);
  typedef HRESULT (__stdcall *PFNGETTHEMERECT)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT RECT *pRect);
  typedef HRESULT (__stdcall *PFNSETWINDOWTHEME)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
  typedef HRESULT (__stdcall *PFNGETTHEMEFILENAME)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, OUT LPWSTR pszThemeFileName, int cchMaxBuffChars);
  typedef COLORREF(__stdcall *PFNGETTHEMESYSCOLOR)(HTHEME hTheme, int iColorId);
  typedef HBRUSH (__stdcall *PFNGETTHEMESYSCOLORBRUSH)(HTHEME hTheme, int iColorId);
  typedef HRESULT (__stdcall *PFNGETTHEMESYSFONT)(HTHEME hTheme, int iFontId, OUT LOGFONT *plf);
  typedef HRESULT (__stdcall *PFNGETTHEMESYSSTRING)(HTHEME hTheme, int iStringId, OUT LPWSTR pszStringBuff, int cchMaxStringChars);
  typedef BOOL32 (__stdcall *PFNISAPPTHEMED)();
  typedef HTHEME (__stdcall *PFNGETWINDOWTHEME)(HWND hwnd);
  typedef HRESULT (__stdcall *PFGETCURRENTTHEMENAME)( LPWSTR pszThemeFileName, int dwMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);
  typedef BOOL32 (__stdcall *PISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)( HTHEME hTheme, int iPartId, int iStateId);
  typedef HRESULT (__stdcall *PDRAWTHEMEPARENTBACKGROUND)( HWND hwnd, HDC hdc, OPTIONAL RECT* prc);

  static HMODULE m_hModThemes;
  static PFNOPENTHEMEDATA m_OpenThemeData;
  static PFNDRAWTHEMEBACKGROUND m_DrawThemeBackground;
  static PFNCLOSETHEMEDATA m_CloseThemeData;
  static PFNDRAWTHEMETEXT m_DrawThemeText;
  static PFNGETTHEMEBACKGROUNDCONTENTRECT m_GetThemeBackgroundContentRect;
  static PFNDRAWTHEMEEDGE m_DrawThemeEdge;
  static PFNDRAWTHEMEICON m_DrawThemeIcon;

  static PFNGETTHEMECOLOR m_GetThemeColor;
  static PFNGETTHEMEMETRIC m_GetThemeMetric;
  static PFNGETTHEMESTRING m_GetThemeString;
  static PFNGETTHEMEBOOL m_GetThemeBool;
  static PFNGETTHEMEINT m_GetThemeInt;
  static PFNGETTHEMEENUMVALUE m_GetThemeEnumValue;
  static PFNGETTHEMEFONT m_GetThemeFont;
  static PFNGETTHEMERECT m_GetThemeRect;
  static PFNSETWINDOWTHEME m_SetWindowTheme;
  static PFNGETTHEMEFILENAME m_GetThemeFilename;
  static PFNGETTHEMESYSCOLOR m_GetThemeSysColor;
  static PFNGETTHEMESYSCOLORBRUSH m_GetThemeSysColorBrush;
  static PFNGETTHEMESYSFONT m_GetThemeSysFont;
  static PFNGETTHEMESYSSTRING m_GetThemeSysString;
  static PFNISAPPTHEMED m_IsAppThemed;
  static PFNGETWINDOWTHEME m_GetWindowTheme;
  static PFGETCURRENTTHEMENAME m_GetCurrentThemeName;
  static PISTHEMEBACKGROUNDPARTIALLYTRANSPARENT m_IsThemeBackgroundPartiallyTransparent;
  static PDRAWTHEMEPARENTBACKGROUND m_DrawThemeParentBackground;
};

#endif // defined (ON_RUNTIME_WIN)
