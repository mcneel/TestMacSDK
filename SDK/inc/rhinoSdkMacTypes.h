//
//  rhinoSdkMacTypes.h
//  TestMacCPPPlugin
//
//  Created by Andrew Le Bihan on 31.10.2024.
//

#ifndef rhinoSdkMacTypes_h
#define rhinoSdkMacTypes_h

// Define some Windows types and structures
#ifndef CONST
#define CONST           const
#endif

typedef unsigned int    DWORD;
typedef unsigned short  USHORT;
typedef unsigned long long    ULONGLONG;
typedef unsigned long long    DWORD64;
typedef long            LONG;
typedef long long       LONGLONG;
typedef long long       LARGE_INTEGER;
// Mac Objective-C definition of BOOL
//typedef signed char     BOOL;
typedef signed char BOOL32;
// Windows definition of BOOL
//typedef int             BOOL;
typedef unsigned char   BYTE;
typedef unsigned char   UCHAR;
typedef unsigned short  WORD;
typedef char            CHAR;
typedef short           SHORT;
typedef int             INT;
typedef unsigned int    UINT;
typedef float           FLOAT;
typedef FLOAT*          PFLOAT;
typedef BOOL32*           PBOOL;
typedef BOOL32*           LPBOOL;
typedef BYTE*           PBYTE;
typedef BYTE*           LPBYTE;
typedef int*            PINT;
typedef int*            LPINT;
typedef WORD*           PWORD;
typedef WORD*           LPWORD;
typedef long*           LPLONG;
typedef DWORD*          PDWORD;
typedef DWORD*          LPDWORD;
typedef void            VOID;
typedef void*           PVOID;
typedef void*           LPVOID;
typedef CONST void*     LPCVOID;

typedef long            INT_PTR, *PINT_PTR;
typedef unsigned long   UINT_PTR, *PUINT_PTR;

typedef long            LONG_PTR, *PLONG_PTR;
typedef unsigned long   ULONG_PTR, *PULONG_PTR;
typedef unsigned long   DWORD_PTR, *PDWORD_PTR;

typedef long long       __int64;

typedef unsigned int*   PUINT;

typedef UINT_PTR        WPARAM;
typedef LONG_PTR        LPARAM;
typedef long            LRESULT;
typedef int             HRESULT;
typedef unsigned long   LCID;

typedef const char*     LPCSTR;
typedef const wchar_t*  LPCTSTR;
typedef const wchar_t*  LPCWSTR;
typedef wchar_t*        LPTSTR;
typedef wchar_t*        LPWSTR;
typedef char*           LPSTR;
typedef char*           PSTR;

typedef wchar_t         TCHAR;
typedef wchar_t         WCHAR;

typedef LPWSTR BSTR;

typedef double DATE;

//
// The HCURSOR handle type is special on OS X.
// It contains a valid Objective-C object pointer, so
// it declared as id so ARC will handle it correctly.
//
typedef id      HCURSOR;

#if TARGET_OS_IOS
typedef unsigned long   ULONG;
class COleDataObject;
typedef int DROPEFFECT;
typedef void* HGLRC;
#else
typedef UInt32          ULONG;
typedef NSDragOperation DROPEFFECT;
class NSPasteBoard;
typedef NSPasteBoard COleDataObject;
typedef NSOpenGLContext* HGLRC;
#endif

class CWnd;

typedef void* LPDISPATCH;
typedef void*    LPUNKNOWN;
typedef void*    WNDPROC;

typedef void*    CRITICAL_SECTION, *LPCRITICAL_SECTION;
typedef void*    HANDLE;
typedef void*    HINSTANCE;
typedef void*    HMODULE;
typedef void*    HKEY;
typedef void*    HMENU;
typedef void*    MENUITEMINFO;
typedef CWnd* HWND;
typedef void* HDC;
typedef void* HPEN;
typedef void* HGLOBAL;
typedef void* HHOOK;
typedef void* HPALETTE;
typedef void* HBRUSH;
typedef void* HTREEITEM;
typedef void* HMONITOR;
typedef void* HACCEL;
typedef void* HTHEME;
typedef void* HFONT;
typedef void* HDWP;
typedef void* HRGN;
typedef void* HIMAGELIST;
typedef void* HGDIOBJ;
typedef void* MSG;

typedef void* LPHELPINFO;
typedef void* LPDRAWITEMSTRUCT;
typedef void* LPCOMPAREITEMSTRUCT;
typedef void* LPMEASUREITEMSTRUCT;
typedef void* LPDELETEITEMSTRUCT;
typedef void* LPCDLGTEMPLATE;
typedef void* LPTOP_LEVEL_EXCEPTION_FILTER;
typedef void* LPNMUPDOWN;
typedef void* LPDEVMODE;
typedef void* LPPRINTER_INFO_2;

typedef void* TEXTMETRIC;

typedef struct ON_WindowsRGBQUAD RGBQUAD;

typedef struct ON_WindowsBITMAPINFOHEADER BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct ON_WindowsBITMAPFILEHEADER BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct ON_WindowsBITMAPINFO BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

typedef LPBITMAPINFO HBITMAP;
typedef LPBITMAPINFO HICON;

typedef ON_UUID GUID;
typedef ON_UUID UUID;
typedef ON_Color COLORREF;
typedef ON_Font LOGFONT;
typedef ON_Font LOGFONTW;
typedef ON_Font CFont;

typedef struct tagSIZE
{
  long        cx;
  long        cy;
} SIZE, *PSIZE, *LPSIZE;

// abstract iteration position
struct __POSITION {};
typedef __POSITION* POSITION;

typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;
typedef const RECT* LPCRECT;

typedef struct tagPOINT
{
  long  x;
  long  y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;

typedef struct tagPOINTFLOAT
{
  float  x;
  float  y;
} POINTFLOAT, *PPOINTFLOAT, *NPPOINTFLOAT, *LPPOINTFLOAT;

typedef struct tagWINDOWPOS {
  HWND    hwnd;
  HWND    hwndInsertAfter;
  int     x;
  int     y;
  int     cx;
  int     cy;
  UINT    flags;
} WINDOWPOS, *LPWINDOWPOS, *PWINDOWPOS;

typedef struct tagLOGBRUSH {
  UINT      lbStyle;
  COLORREF  lbColor;
  ULONG_PTR lbHatch;
} LOGBRUSH, *PLOGBRUSH;

typedef struct _charrange {
  LONG cpMin;
  LONG cpMax;
} CHARRANGE;


#define RGB(r,g,b)    (ON_Color(r,g,b))
#define RGBA(r,g,b,a)    (ON_Color(r,g,b,a))
#define GetRValue(c)  ((c).Red())
#define GetGValue(c)  ((c).Green())
#define GetBValue(c)  ((c).Blue())
#define GetAValue(c)  ((c).Alpha())

#define LOBYTE(w)  ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)  ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

typedef struct tagBROWSEINFO {
  // contents never used
} BROWSEINFO;

typedef struct tagTCITEM {
  // contents never used
} TCITEM;

typedef struct tagNCCALCSIZE_PARAMS {
  // contents never used
} NCCALCSIZE_PARAMS;

typedef struct tagWINDOWPLACEMENT {
  UINT  length;
  UINT  flags;
  UINT  showCmd;
  POINT ptMinPosition;
  POINT ptMaxPosition;
  RECT  rcNormalPosition;
#ifdef _MAC
  RECT  rcDevice;
#endif
} WINDOWPLACEMENT;
typedef WINDOWPLACEMENT *PWINDOWPLACEMENT, *LPWINDOWPLACEMENT;

typedef struct tagTOOLINFOW {
  UINT cbSize;
  UINT uFlags;
  HWND hwnd;
  UINT_PTR uId;
  RECT rect;
  HINSTANCE hinst;
  LPWSTR lpszText;
#if (_WIN32_IE >= 0x0300)
  LPARAM lParam;
#endif
#if (_WIN32_WINNT >= 0x0501)
  void *lpReserved;
#endif
} TOOLINFOW, *PTOOLINFOW, *LPTTTOOLINFOW;


typedef struct _GLYPHMETRICSFLOAT { // gmf
  FLOAT      gmfBlackBoxX;
  FLOAT      gmfBlackBoxY;
  POINTFLOAT gmfptGlyphOrigin;
  FLOAT      gmfCellIncX;
  FLOAT      gmfCellIncY;
} GLYPHMETRICSFLOAT, *LPGLYPHMETRICSFLOAT;

typedef struct tagPIXELFORMATDESCRIPTOR { // pfd
  WORD  nSize;
  WORD  nVersion;
  DWORD dwFlags;
  BYTE  iPixelType;
  BYTE  cColorBits;
  BYTE  cRedBits;
  BYTE  cRedShift;
  BYTE  cGreenBits;
  BYTE  cGreenShift;
  BYTE  cBlueBits;
  BYTE  cBlueShift;
  BYTE  cAlphaBits;
  BYTE  cAlphaShift;
  BYTE  cAccumBits;
  BYTE  cAccumRedBits;
  BYTE  cAccumGreenBits;
  BYTE  cAccumBlueBits;
  BYTE  cAccumAlphaBits;
  BYTE  cDepthBits;
  BYTE  cStencilBits;
  BYTE  cAuxBuffers;
  BYTE  iLayerType;
  BYTE  bReserved;
  DWORD dwLayerMask;
  DWORD dwVisibleMask;
  DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR;


typedef struct tagCREATESTRUCT {
  LPVOID lpCreateParams;
  HINSTANCE hInstance;
  HMENU hMenu;
  HWND hwndParent;
  int cy;
  int cx;
  int y;
  int x;
  LONG style;
  LPCTSTR lpszName;
  LPCTSTR lpszClass;
  DWORD dwExStyle;
} CREATESTRUCT, *LPCREATESTRUCT;

typedef struct tagPAINTSTRUCT {
  HDC hdc;
  BOOL32 fErase;
  RECT rcPaint;
  BOOL32 fRestore;
  BOOL32 fIncUpdate;
  BYTE rgbReserved[32];
} PAINTSTRUCT, *LPPAINTSTRUCT;

typedef struct tagPALETTEENTRY
{
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
} PALETTEENTRY;

typedef struct tagPALETTEENTRY *LPPALETTEENTRY;

typedef struct tagSYSTEMTIME
{
    time_t tm;
} SYSTEMTIME;

typedef struct tagSTGMEDIUM
{
} STGMEDIUM;

typedef struct tagCLIPFORMAT
{
} CLIPFORMAT;

/* Pen Styles */
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8

#define PS_COSMETIC         0x00000000
#define PS_ENDCAP_ROUND     0x00000000
#define PS_JOIN_ROUND       0x00000000
#define PS_ENDCAP_SQUARE    0x00000100
#define PS_ENDCAP_FLAT      0x00000200
#define PS_JOIN_BEVEL       0x00001000
#define PS_JOIN_MITER       0x00002000
#define PS_GEOMETRIC        0x00010000

#define PS_ENDCAP_STYLE     0x00000F00
#define PS_JOIN_STYLE       0x0000F000

#define SWP_NOSIZE            0x0001
#define SWP_NOMOVE            0x0002
#define SWP_NOZORDER          0x0004
#define SWP_NOACTIVATE        0x0010
#define SWP_SHOWWINDOW        0x0040

/* Windows ShowWindow() parameter values */
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11

#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L

#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND

#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND

#define MB_DEFBUTTON1               0x00000000L
#define MB_DEFBUTTON2               0x00000100L
#define MB_DEFBUTTON3               0x00000200L

#define MB_APPLMODAL                0x00000000L
#define MB_SYSTEMMODAL              0x00001000L
#define MB_TASKMODAL                0x00002000L
#define MB_HELP                     0x00004000L // Help Button

#define MB_NOFOCUS                  0x00008000L
#define MB_SETFOREGROUND            0x00010000L
#define MB_DEFAULT_DESKTOP_ONLY     0x00020000L

#define MB_TOPMOST                  0x00040000L
#define MB_RIGHT                    0x00080000L
#define MB_RTLREADING               0x00100000L

#define MB_SERVICE_NOTIFICATION          0x00200000L
#define MB_SERVICE_NOTIFICATION_NT3X     0x00040000L

#define MB_TYPEMASK                 0x0000000FL
#define MB_ICONMASK                 0x000000F0L
#define MB_DEFMASK                  0x00000F00L
#define MB_MODEMASK                 0x00003000L
#define MB_MISCMASK                 0x0000C000L

#define MF_STRING                   0x00000000L
#define MF_SEPARATOR                0x00000800L
#define MF_CHECKED                  0x00000008L
#define MF_UNCHECKED                0x00000000L
#define MF_BYCOMMAND                0x00000000L
#define MF_BYPOSITION               0x00000000L

#define TPM_BOTTOMALIGN             0x0020L
#define TPM_LEFTALIGN               0x0000L
#define TPM_RETURNCMD               0x0100L

#define ERROR_SUCCESS               0L
#define ERROR_INVALID_FUNCTION      1L
#define ERROR_FILE_NOT_FOUND        2L
#define ERROR_ACCESS_DENIED         5L
#define ERROR_INVALID_DATA          13L
#define ERROR_MORE_DATA             234L
#define ERROR_NO_MORE_ITEMS         259L

/* fake error numbers */
#define ERROR_MOD_NOT_FOUND         -23345
#define ERROR_PROC_NOT_FOUND        -23346
#define ERROR_INVALID_PIXEL_FORMAT  -23347


#endif /* rhinoSdkMacTypes_h */
