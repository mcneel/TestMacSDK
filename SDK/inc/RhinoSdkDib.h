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

#if defined(ON_RUNTIME_WIN)
#define RHINODIB_HDC_SUPPORT
#if defined RHINO_SDK_MFC
#define RHINODIB_CDC_SUPPORT
#endif
#endif

#if defined(ON_RUNTIME_COCOA_AVAILABLE) && !defined (RHINO_THIRD_PARTY_OSX_PLUGIN_COMPILE)
#define RHINODIB_CDC_SUPPORT
#define RHINODIB_HDC_SUPPORT
#endif

#if defined(ON_RUNTIME_LINUX)
#define RHINODIB_CDC_SUPPORT
#endif

void RhinoUiDibInstallMemManager(void* (*my_calloc)(size_t num, size_t size),
  void* (*my_malloc)(size_t size),
  void(*my_free)(void* memblock)
  );

void* RHINOUI_DIB_CALLOC(size_t num, size_t size);
void* RHINOUI_DIB_MALLOC(size_t size);
void RHINOUI_DIB_FREE(void* memblock);

RHINO_SDK_FUNCTION bool RhinoUiDIBGetPixel(BITMAPINFO& bmi, unsigned char *base, long x, unsigned char *r, unsigned char *g, unsigned char *b);
RHINO_SDK_FUNCTION bool RhinoUiDIBGetPixel(BITMAPINFO& bmi, const LPBYTE dib_bits, int c_depth, long x, long y, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a = nullptr);
RHINO_SDK_FUNCTION bool RhinoUiDIBGetPixel(BITMAPINFO& bmi, long x, long y, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a = nullptr);
RHINO_SDK_FUNCTION bool RhinoUiDIBSetPixel(BITMAPINFO& bmi, LPBYTE dib_bits, int c_depth, long x, long y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
RHINO_SDK_FUNCTION bool RhinoUiDIBSetPixel(BITMAPINFO& bmi, long x, long y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

// Class to manage DIB's. This class will create a DIB, a HBITMAP and associated memory
// DC.  The HBITMAP and CRhinoDib::FindBits() point to the same memory.  You can modify the
// bits directly or use GDI with the associated memory DC to modify the bitmap. The HBITMAP
// does not get selected by the memory devidce context due to the fact that the DIB can 
// only be selected in one device context at a time.  To select or remove the Bitmap() from 
// its memory DC call DCSelectBitmap( bool).  If you call DCSelectBitmap( true) you may
// cast a CRhinoDib as a CDC* or HDC and use it directly.
class RHINO_SDK_CLASS CRhinoDib
{
private:
  CRhinoDib(class CRhDibPrivate*);
public:
  // Construction
  CRhinoDib();
  // create bitmap with specified width, height and color depth
  CRhinoDib(long width, long height, int color_depth, bool select_into_mem_dc = false, HPALETTE = nullptr);
  // copy constructor, create bitmap that matches another
  CRhinoDib(const CRhinoDib&);
    
#if !defined ON_RUNTIME_APPLE
  // set Dib to specified BITMAPINFO pointer
  CRhinoDib(LPBITMAPINFO, bool free_lpbmi = true);
#endif
    
  // create Dib from hbitmap
  CRhinoDib(HBITMAP);

  // Destruction
  virtual ~CRhinoDib();
  // Destroy dib && hbitmap and zero out pointers
  void DestroyDib();

  unsigned int RuntimeSerialNumber() const;
  CRhinoCacheHandle* Cache() const;

  // Copy source bitmap, if m_hBitmap is non NULL then create destroy current and 
  // create new hBitmap
  CRhinoDib& operator =(const CRhinoDib& src);
    
#if defined(ON_RUNTIME_WIN)
  CRhinoDib& operator =(LPBITMAPINFO lpbmi);
#endif
    
  CRhinoDib& operator =(HBITMAP hBitmap);

#if defined ON_RUNTIME_WIN && defined RHINODIB_CDC_SUPPORT
  operator HDC() const;
#endif

#if defined RHINODIB_CDC_SUPPORT
  operator CDC*();
  operator CDC*() const;
#endif

  operator HBITMAP();
  operator HBITMAP() const;
    
  //Work-alike for CDC::BitBlt or ::BitBlt where both drawing surfaces are actually CRhinoDib.
  //Note that the coordinate system is the same as Windows DC - 0,0 is at the top left. (For CRhinoDib itself, 0,0 is bottom left)
  //Copys the pixels from srcDib to this.
  bool BitBlt(const ON_4iRect& destRect,
              const CRhinoDib& srcDib,
              const ON_2iPoint& srcPoint);

#if defined(ON_RUNTIME_COCOA_AVAILABLE)
  
  CRhinoDib(const NSImage*);
  CRhinoDib& operator=(const NSImage*);
  void SetDib(const NSImage*);
  NSImage* ToNSImage(bool upside_down=false) const;
  operator NSImage*() const;

  CRhinoDib(const NSBitmapImageRep*);
  CRhinoDib& operator=(const NSBitmapImageRep*);
  void SetDib(const NSBitmapImageRep*);
  NSBitmapImageRep* ToNSBitmapImageRep(bool upside_down=false) const;
  operator NSBitmapImageRep* () const;

  CRhinoDib(const CGImage*);
  CRhinoDib& operator=(const CGImage*);
  void SetDib(const CGImage*);
  void SetBitmap(const CGImage*);
  CGImage* ToCGImage() const;
  operator CGImage* () const;

#endif

  ON_WindowsBitmap ToON_WindowsBitmap(void) const;

  /*
  Description:
    Call this method to convert this bitmap to a ON_WindowsBitmap pointer, you
    should NEVER call ON_WindowsBitmap(BitmapInfo()) as it assumes the
    BITMAPINFO is a contiguous block of memory for the bitmap and its color
    table.  CRhinoDib will rarely represent a contiguous block of memory and
    will cause the ON_WindowsBitamp constructor to crash when attempting to copy
    the bitmap bits.
  Returns:
    A pointer to a new ON_WindowsBitmap, the caller is responsible for deleting
    the returned pointer.
  */
  ON_WindowsBitmap* ToOnWindowsBitmap() const;

  // Clears (fills) the entire dib with the passed in color...
  void Clear(COLORREF);

  enum class ResampleAlgorithm
  {
    Nearest,  // Samples a single pixel in the source. Fastest.
    Bilinear, // Bilinear interpolation of 4 neighboring pixels.
    Box       // Accurate brute-force resample of source area to pixels. Slowest.
  };

  // Legacy resize method; uses Bilinear resampling.
  bool Resize(const ON_2iSize& newSize);
 
  // New resize method; uses chosen resampling algorithm.
  bool Resize(const ON_2iSize& newSize, ResampleAlgorithm alg);

  // Legacy resized method; uses Bilinear resampling.
  bool Resized(const CRhinoDib& input, const ON_2iSize& newSize);

  // New resized method; uses chosen resampling algorithm.
  bool Resized(const CRhinoDib& input, const ON_2iSize& newSize, ResampleAlgorithm alg);

  // If color depth is less than the requested color depth create a new
  // bitmap of the desired color depth and copy old bits to it
  // Change of behavior in V6 - if the new color depth is 32, the alpha pixels will be filled with 255.
  bool IncreaseColorDepth(int nNewColorDepth);

  // Converts the dib into a grayscale image...and promotes it to a
  // 24 bit dib if need be...
  // If bSetAlpha is true, and the bitmap is a 32-bit image, the alpha byte is set to the same as the other three components
  // otherwise, the alpha byte is preserved from the original image.
  bool ConvertToGrayScale(bool bSetAlpha = false);

  // DIB or HBITMAP creation functions
  LPBITMAPINFO CreateDib(long width, long height, int color_depth, bool select_into_mem_dc = false, HPALETTE = nullptr);
  bool ReuseDib(long width, long height, int color_depth, bool select_into_mem_dc);
  bool CreatePalette(LPBITMAPINFO);
  // Create HBITMAP and set m_hBitmap 
  HBITMAP CreateHBitmap(HPALETTE hpal = nullptr);
  // Destroy existing m_bitmapinfo and m_hBitmap, set them to specified dib.
  // If Bitmap() was non NULL then a new HBITMAP is created from the new dib
  bool SetDib(LPBITMAPINFO lpbmi, bool free_lpbmi = true);
  // Create dib from hBitmap, if dib created successfully m_hBitmap is set
  // to a new HBITMAP created from Dib()
  bool SetBitmap(HBITMAP hBitmap);
  // Return copy of existing dib, you are responsible for destroying this object
  LPBITMAPINFO CopyDib() const;
  // Copy bmi to m_bitmapinfo
  bool CopyDib(const BITMAPINFO& bmi);
  // Return copy of the current Dib().  If Bitmap() is NULL then one is created
  // from Dib() and returned.  m_hBitmap is not set if Bitmap was NULL.
  // You are responsible for destroying this object.
  HBITMAP CopyHBitmap() const;
  // Copy dib bits from Dib to destination
  bool BltToDib(CRhinoDib& destination, long x_dest, long y_dest, long cx, long cy, long x_src, long y_src);
  bool BltToDib(BITMAPINFO& destination, long x_dest, long y_dest, long cx, long cy, long x_src, long y_src);

  // Clipboard operations
  bool CopyToClipboard(HWND hwnd = nullptr) const;
  bool PasteFromClipboard(HWND hwnd, bool bMustBeSameSize = true, bool bMustHaveSameColorDepth = true) const;

    
#if defined RHINODIB_HDC_SUPPORT
  // Blit functions functions
  bool CalcStretchRect(const ON_4iRect& rect, bool bMaintainRatio = true, bool bCenter = true,
    LPRECT lpRcSource = nullptr, LPRECT lpRcStretch = nullptr, HDC pDC = nullptr);
  bool Stretch(HDC pDC, const ON_4iRect& rect, bool bMaintainRatio = true, bool bCenter = true,
    LPRECT lpRcSource = nullptr, LPRECT lpRcStretch = nullptr);


  bool StretchMasked(HDC pDC, COLORREF crMask, const ON_4iRect& rect, bool bMaintainRatio = true, bool bCenter = true,
    LPRECT lpRcSource = nullptr, LPRECT lpRcStretch = nullptr);


  bool Draw(HDC pDC, long x = 0L, long y = 0L, long cx = 0L, long cy = 0L, long xSrc = 0L, long ySrc = 0L);
  bool Draw(HDC pDC, const ON_4iRect& rect, long xSrc = 0L, long ySrc = 0L);
  bool DrawMasked(HDC pDC, COLORREF crMask, long x = 0L, long y = 0L, long cx = 0L, long cy = 0L, long xSrc = 0L, long ySrc = 0L);
  bool DrawMasked(HDC pDC, COLORREF crMask, const ON_4iRect& rect, long xSrc = 0L, long ySrc = 0L);
    
#endif
    
  // Bitmap information
  DWORD Size() const;
  long SizeHeader() const;
  DWORD SizeBits() const;
  int  ColorDepth() const;
  int NumColors() const;
  long PaletteSize() const;
  HPALETTE Palette() const;
  long Width() const;
  DWORD WidthInBytes() const;
  long Height() const;
  DWORD ScanWidth() const;
  UINT PelsPerMeter() const;
  void SetPelsPerMeter(UINT);
  double DPI() const;
  void SetDPI(double);

  /*
  Description:
    Get Windows bitmap bits.
  Returns:
    A pointer to the bits.  Look but do not change.
  See Also:
    CRhinoDib::BitmapInfo
  */
  LPBYTE FindDIBBits() const;

  bool IsAllocated() const;

  enum class ComponentOrders : int
  {
	  None	= 0,
	  L		= 1,
	  BGR	= 2,
	  RGB	= 3,
	  BGRA	= 4,
	  RGBA	= 5,
	  ABGR	= 6,
	  ARGB	= 7,
	  RUNTIME_24BIT = 2,
	  RUNTIME_32BIT = 4
  };

  ComponentOrders ComponentOrder(void) const;

  static ComponentOrders GLFormatToComponentOrder(unsigned int);

  LPBYTE FindDIBBits(ComponentOrders) const;

  class Pixel
  {
  public:
    inline void Set(unsigned char in_r, unsigned char in_g, unsigned char in_b, unsigned char in_alpha)
    {
      ::RhinoUiDIBSetPixel(bmi, dib_bits, color_depth, x, y, in_r, in_g, in_b, in_alpha);
    }

    BITMAPINFO& bmi;
    LPBYTE dib_bits;
    const int color_depth;
    const long x;
    const long y;
    const unsigned char r, g, b;
    const unsigned char alpha;

    //To support DIBs where a 32-bit float type is stored in the 32 bits of each pixel.  Does not work on non-32-bit images.
    float Float(void) const;
    void Set(float);

    Pixel(BITMAPINFO& in_bmi, LPBYTE in_bits, long in_cd, long in_x, long in_y, unsigned char in_r, unsigned char in_g, unsigned char in_b, unsigned char in_alpha)
      :
      bmi(in_bmi), dib_bits(in_bits), color_depth((int)in_cd), x(in_x), y(in_y), r(in_r), g(in_g), b(in_b), alpha(in_alpha)
    {}
  };

  // Fast pixel iterators.  Quick access to the image data for each pixel, multi-threaded.
  // Use the pixel setter provided to quickly set pixel values.
  void ProcessPixels(void(*f)(Pixel&, void* pvData), void* pvData = nullptr);
  void ProcessPixels(void(*f)(Pixel&, const void* pvData), const void* pvData = nullptr);
  void ProcessPixels(void(*f)(const Pixel&, void* pvData), void* pvData = nullptr) const;

  void ProcessPixels_SingleThreaded(void(*f)(Pixel&, void* pvData), void* pvData = nullptr);
  void ProcessPixels_SingleThreaded(void(*f)(Pixel&, const void* pvData), const void* pvData = nullptr);
  void ProcessPixels_SingleThreaded(void(*f)(const Pixel&, void* pvData), void* pvData = nullptr) const;

  // Pixel iterators with an interrupt.  Return false in the callback function to interrupt the loops.  
  // Current implementation is not multithreaded, but this may change.
  bool ProcessPixels(bool(*f)(Pixel&, void* pvData), void* pvData = nullptr);
  bool ProcessPixels(bool(*f)(const Pixel&, void* pvData), void* pvData = nullptr) const;

  bool GetPixel(unsigned char *base, long x, unsigned char *r, unsigned char *g, unsigned char *b) const;

#ifdef ON_RUNTIME_WIN
  inline HBITMAP Bitmap() const;
#else
  HBITMAP Bitmap() const;
#endif

  /*
    These "safe" functions call CRhinoDib::FindDIBBits and validate x and y
  */

  // Sets the color of a pixel at an x,y location given a fractional RGB color components (between 0.0 and 1.0)
  bool SetPixel(long x, long y, float r, float g, float b, float alpha = -1.0);
  // Sets the color of a pixel at an x,y location given an RGB color components (between 0 and 255)
  bool SetPixel(long x, long y, int r, int g, int b, int alpha = -1);
  // Sets the color of a pixel at an x,y location given an RGB color
  bool SetPixel(long x, long y, COLORREF cr, int alpha = -1);

  // Gets the RGB color of a pixel at an x,y location 
  bool GetPixel(long x, long y, COLORREF& cr) const;
  bool GetPixel(long x, long y, COLORREF& cr, int& alpha) const;
  // Gets the RGB color components (between 0 and 255) of a pixel at an x,y location 
  bool GetPixel(long x, long y, int& r, int& g, int& b) const;
  bool GetPixel(long x, long y, int& r, int& g, int& b, int& alpha) const;
  // Gets the fractional RGB color components (between 0.0 and 1.0) of a pixel at an x,y location 
  bool GetPixel(long x, long y, float& r, float& g, float& b) const;
  bool GetPixel(long x, long y, float& r, float& g, float& b, float& alpha) const;

  class RHINO_SDK_CLASS CFastPixelAccess
  {
  public:
    CFastPixelAccess(CRhinoDib& dib);
    ~CFastPixelAccess();

    bool IsValid(void) const;

    //Note that these functions will call CRhinoDib::GetPixel or SetPixel if IsValid returns false (so they will also be slow)
    //but in most cases, these functions will be way, way faster than the raw functions on CRhinoDib, as long as the object is
    //retained during the pixel loop
    BOOL32 GetPixel(long x, long y, int& r, int& g, int& b, int& alpha) const;
    BOOL32 SetPixel(long x, long y, int r, int g, int b, int alpha = 255);

    int Width(void) const;
    int Height(void) const;
    CRhinoDib& Dib(void);
  
  private:
    class Impl;
    Impl* m_pImpl;
    friend class Impl;

  public:
    //Copying
    CFastPixelAccess(const CFastPixelAccess&) = delete;
    const CFastPixelAccess& operator=(const CFastPixelAccess&) = delete;
  };

  const CFastPixelAccess* FastPixelAccess(void) const;
  CFastPixelAccess* FastPixelAccess(void);

  /*
    These "unsafe" functions assume the DIB bits are valid and assume x and y are within range
  */

  // Sets the color of a pixel at an x,y location given a fractional RGB color components (between 0.0 and 1.0)
  bool SetPixel(LPBYTE dib_bits, int color_depth, long x, long y, float r, float g, float b, float alpha = -1.0);
  // Sets the color of a pixel at an x,y location given an RGB color components (between 0 and 255)
  bool SetPixel(LPBYTE dib_bits, int color_depth, long x, long y, int r, int g, int b, int alpha = -1);
  // Sets the color of a pixel at an x,y location given an RGB color
  bool SetPixel(LPBYTE dib_bits, int color_depth, long x, long y, COLORREF cr, int alpha = -1);

  // Gets the RGB color of a pixel at an x,y location 
  bool GetPixel(const LPBYTE dib_bits, int color_depth, long x, long y, COLORREF& cr, int& alpha) const;
  // Gets the RGB color components (between 0 and 255) of a pixel at an x,y location 
  bool GetPixel(const LPBYTE dib_bits, int color_depth, long x, long y, int& r, int& g, int& b) const;
  bool GetPixel(const LPBYTE dib_bits, int color_depth, long x, long y, int& r, int& g, int& b, int& alpha) const;
  // Gets the fractional RGB color components (between 0.0 and 1.0) of a pixel at an x,y location 
  bool GetPixel(const LPBYTE dib_bits, int color_depth, long x, long y, float& r, float& g, float& b) const;
  bool GetPixel(const LPBYTE dib_bits, int color_depth, long x, long y, float& r, float& g, float& b, float& alpha) const;

  bool BlendDibsUsingAlphaChannel(long xDest, long yDest, long cx, long cy, CRhinoDib& src, long xSrc, long ySrc);

  // Goofy edit functions
  bool FloodFill(long x, long y, COLORREF cr, int traget_alpha = -1);

  // Fills the entire dib with the specified color...Note: if it's a 32
  // bit dib, then the high-order byte of the passed in color is considered
  // to be an alpha value, and thus will also be included in the fill
  // operation (normal GDI operations will strip this out).
  bool FillSolid(COLORREF);

  bool GradientFill(COLORREF topLeft, COLORREF topRight, COLORREF bottomLeft, COLORREF bottomRight);

  //Points are in DIB space - ie, the same coordinate system as GetPixel.  Use FlipPoint/Flipped points to switch
  bool DrawFilledPolygon(const ON_SimpleArray<ON_2iPoint>& points, ON_Color cr);
  bool DrawFilledTriangle(const ON_2iPoint& p1, const ON_2iPoint& p2, const ON_2iPoint& p3, ON_Color);
  bool DrawFilledCircle(const ON_2iPoint& center, int radius, ON_Color);

  bool DrawLine(const ON_2iPoint& p1, const ON_2iPoint& p2, ON_Color color, int thickness = 0, bool circle_caps = false);
  bool DrawPolyline(const ON_SimpleArray<ON_2iPoint>& points, ON_Color color, int thickness = 0, bool circle_caps = false, bool close = false);

  //Use these functions to transform points and arrays of points between DIB space (bottom up) and CDC space (top down)
  void FlipPoint(ON_2iPoint&) const;
  void FlipPoints(ON_SimpleArray<ON_2iPoint>&) const;

  ON_2iPoint FlippedPoint(const ON_2iPoint&) const;
  ON_SimpleArray<ON_2iPoint> FlippedPoints(const ON_SimpleArray<ON_2iPoint>&) const;

  // shift a dib up by one pixel - wraps other side around
  // only works on a 24 and 32 bit dibs right now.
  // note that since a DIB is flipped in y from what you'd
  // expect, the guts of this does what you'd usually think to do to
  // shift it down by one pixel
  CRhinoDib* ShiftUp();
  // shift a dib down by one pixel - wraps other side around
  // only works on a 24 and 32 bit dibs right now.
  // note that since a DIB is flipped in y from what you'd
  // expect, the guts of this does what you'd usually think to do to
  // shift it up by one pixel
  CRhinoDib* ShiftDown();
  // shift a dib left by one pixel - wraps other side around
  // only works on a 24 and 32 bit dibs right now.
  CRhinoDib* ShiftLeft();
  // shift a dib right by one pixel - wraps other side around
  // only works on a 24 and 32 bit dibs right now.f
  CRhinoDib* ShiftRight();
  // Convert Y value from window display Y to bitmap Y
  long WindowYtoDibY(long y) const;

  void FlipVertical(void);
  void MakeOpaque(void);

#if defined ON_RUNTIME_WIN || defined ON_RUNTIME_COCOA_AVAILABLE
  bool DCSelectBitmap(bool bSelect);
  bool BitmapSelectedIntoDC() const;
#endif

#if defined (ON_RUNTIME_WIN)
  // Read and write *.bmp Windows bitmap files.
  virtual bool SaveBmp(HANDLE hFile);
  virtual bool SaveBmp(LPCTSTR pszStringName);
  virtual bool LoadBmp(UINT nResID);
  virtual bool LoadBmp(HANDLE hFile);
  virtual bool LoadBmp(LPCTSTR pszStringName);
#endif
    
  DWORD GetStorageWidth() const;

#if defined ON_RUNTIME_WIN
    inline UINT GetDepth() const;
    inline DWORD GetStorageSize() const;
#else
    UINT GetDepth() const;
    DWORD GetStorageSize() const;
#endif

  /*
  Description:
    Get Windows bitmap info.
  Remarks:
    NOTE WELL: The bitmap bits are generally NOT located after
    BITMAPINFO and bmiColors[] array.
  Returns:
    A pointer to the bitmap info.  Look but do not change.
  See Also:
    CRhinoDib::CRhinoDib::FindDIBBits
  */
  const BITMAPINFO* BitmapInfo() const;

  enum class Quality : int
  {
    kHigh,
    kMedium,
    kLow
  };

  /*
  Description:
    Prompt user for the name of the bitmap file to save then write the bitmap
    file.
  Parameters:
    hWnd - [in] Parent window of the file dialog.
    bUseDialogs - [in] If true then the file dialog is displayed otherwise; the
                       user is prompted on the command line for a file name.
    q [in]
  Returns:
    Returns true if a file name was supplied and the file was written successfully.
  */
  bool WriteToFile(HWND hWnd = nullptr, bool bUseDialogs = true, Quality q = Quality::kHigh) const;

  /*
  Description:
    Prompt user for the name of the bitmap file to save then write the bitmap
    file.
  Parameters:
    hWnd - [in] Parent window of the file dialog.
    bUseDialogs - [in] If true then the file dialog is displayed otherwise; the
    user is prompted on the command line for a file name.
    q [in]
  Returns:
    CRhinoCommand::result::cancel - The file dialog or script was canceled.
    CRhinoCommand::result::failure - Error writing the bitmap file.
    CRhinoCommand::result::success - The file was written successfully.
  */
  CRhinoCommand::result WriteFile(HWND hWnd = nullptr, bool bUseDialogs = true, Quality q = Quality::kHigh) const;

  /*
  Description:
    Write bitmap to disk.
  Parameters:
    lpsFileName - [in] Name of file to write.
  Returns:
    true  - Specified File name written to disk.
    false - Error writing file or invalid file type.
  */
  bool WriteToFile(const wchar_t* lpsFileName, Quality q = Quality::kHigh) const;

  /*
  Description:
    Display Rhino bitmap open dialog prompting for name then read
    bitmap from disk.
  Parameters:
    hWnd - [in] Parent window for file dialog.  If NULL will use
    Rhino frame window.
    bUseDialogs - [in] If true then display file dialog otherwise
    prompt for name on command line.
  Returns:
    true  - File name specified and written to disk.
    false - File dialog canceled or error reading file.
  */
  ON_DEPRECATED_MSG("Use the version that takes doc_serial_number")
  bool ReadFromFile(HWND hWnd = nullptr, bool bUseDialogs = true);

  /*
  Description:
  Display Rhino bitmap open dialog prompting for name then read
  bitmap from disk.
  Parameters:
  doc_serial_number - [in] CRhinoDoc::RuntimeSerialNumber to use for searches.
  hWnd - [in] Parent window for file dialog.  If NULL will use
  Rhino frame window.
  bUseDialogs - [in] If true then display file dialog otherwise
  prompt for name on command line.
  Returns:
  true  - File name specified and written to disk.
  false - File dialog canceled or error reading file.
  */
  bool ReadFromFile(unsigned int doc_serial_number, HWND hWnd = nullptr, bool bUseDialogs = true);

  /*
  Description:
    Display Rhino bitmap open dialog prompting for name then read
    bitmap from disk.
  Parameters:
    hWnd - [in] Parent window for file dialog.  If NULL will use
    Rhino frame window.
    bUseDialogs - [in] If true then display file dialog otherwise
    prompt for name on command line.
  Returns:
    CRhinoCommand::result::cancel - The file dialog or script was canceled.
    CRhinoCommand::result::failure - Error reading the file.
    CRhinoCommand::result::success - The file was read successfully
  */
  ON_DEPRECATED_MSG("Use the version that takes doc_serial_number")
  CRhinoCommand::result ReadFile(HWND hWnd = nullptr, bool bUseDialogs = true);

  /*
  Description:
  Display Rhino bitmap open dialog prompting for name then read
  bitmap from disk.
  Parameters:
  doc_serial_number - [in] CRhinoDoc runtime serial number for searches.
  hWnd - [in] Parent window for file dialog.  If NULL will use
  Rhino frame window.
  bUseDialogs - [in] If true then display file dialog otherwise
  prompt for name on command line.
  Returns:
  CRhinoCommand::result::cancel - The file dialog or script was canceled.
  CRhinoCommand::result::failure - Error reading the file.
  CRhinoCommand::result::success - The file was read successfully
  */
  CRhinoCommand::result ReadFile(unsigned int doc_serial_number, HWND hWnd = nullptr, bool bUseDialogs = true);

  /*
  Description:
    Read bitmap from disk.
  Parameters:
    lpsFileName - [in] Name of file to read.  Uses
    CRhinoFileUtilities::FindFile(...) to validate file name.
  Returns:
    true  - File was read from disk.
    false - Error reading file or invalid file type.
  */
  ON_DEPRECATED_MSG("Use the version that takes doc_serial_number")
  bool ReadFromFile(const wchar_t* lpsFileName);

  /*
  Description:
  Read bitmap from disk.
  Parameters:
  doc_serial_number - [in] Runtime serial number of the CRhinoDoc to be used for image searches.
  lpsFileName - [in] Name of file to read. Uses CRhinoFileUtilities::FindFile(...) to validate the file name.
  Returns:
  true  - File was read from disk.
  false - Error reading file or invalid file type.
  */
  bool ReadFromFile(unsigned int doc_serial_number, const wchar_t* lpsFileName);

  /*
  Description:
    Read image size and dots per inch from disk.
  Parameters:
    lpsFileName - [in]
    Name of file to read.  Uses
    CRhinoFileUtilities::FindFile(...) to validate file name.

    image_width_pixels - [out]
    image_height_pixels - [out]
    The dimensions of the image are returned in the pixels parameters.

    image_width_dpi - [out]
    image_height_dpi - [out]
    The dots per inch values saved in the image file are
    returned in the dpi parameters.

    You can pass null for any [out] parameters that are not required.
  Returns:
    true  - File was read from disk.
    false - Error reading file or invalid file type.
  */
  static bool ReadFromFileImageSize(
    const wchar_t* lpsFileName,
    unsigned int* image_width_pixels,
    unsigned int* image_height_pixels,
    unsigned int* image_width_dpi,
    unsigned int* image_height_dpi
    );

  /*
  Description:
    Reads image size, dots per inch and bit depth from a file.
  Parameters:
    file_name - [in]
    Name of file to read. Uses CRhinoFileUtilities::FindFile(...) to validate file name.

    image_width_pixels - [out]
    image_height_pixels - [out]
    The dimensions of the image are returned in the pixels parameters.

    image_width_dpi - [out]
    image_height_dpi - [out]
    The dots per inch values saved in the image file are returned in the dpi parameters.

    image_depth_bits - [out]
    The bit depth of the image is returned here (e.g., 24 for RGB, 32 for RGBA).

    You can pass null for any [out] parameters that are not required.
  Returns:
    true  - File was read from disk.
    false - Error reading file or invalid file type.
  */
  static bool ReadFromFileImageSize(
    const wchar_t* file_name,
    unsigned int* image_width_pixels,
    unsigned int* image_height_pixels,
    unsigned int* image_width_dpi,
    unsigned int* image_height_dpi,
    unsigned int* image_depth_bits
    );

  /*
  Description:
    Read bitmap from a memory buffer.
  Parameters:
    buffer - [in]
    sizeof_buffer - [in] number of bytes in the buffer
  Returns:
    true  - Image was read from buffer
    false - Error reading buffer
  */
  bool ReadFromMemory(
    const void* buffer,
    size_t sizeof_buffer
    );


  /*
  Description:
  Converts an SVG definition to a bitmap.

  Parameters:
  svg - [in] The textual definition of the SVG image.
  width - [in] The required width of the bitmap.
  height - [in] The required height of the bitmap.
  convert_to_darkmode - [in] If true, the image will be processed to make it suitable for display in dark mode.

  Returns:
  True if successful, else false.
  */
  bool ReadFromSvgData(const ON_wString& svg, int width, int height, bool convert_to_darkmode = false);

  /*
  Description:
    Display print dialog
  Parameters:
    hWnd - [in] Parent window for print dialog.  If NULL will use
    Rhino frame window.
  Returns:
    true  - DIB sent to printer
    false - Print dialog canceled or error writing to printer.
  */
  ON_DEPRECATED bool Print(HWND hWnd = nullptr, bool bFitToPage = true);
  const wchar_t* LastSaveName() const;
  void SetLastSaveName(const wchar_t*);

  enum bitmap_file_type
  {
    bmp_file_type = 0, // "*.bmp"
    jpg_file_type,     // "*.jpg;*.jpeg"
    pcx_file_type,     // "*.pcx"
    png_file_type,     // "*.png"
    tga_file_type,     // "*.tga"
    tif_file_type,     // "*.tif;*.tiff"
	dds_file_type,     // "*.dds"
    // Do NOT change this value, this allows adding additional values without
    // breaking the SDK in the future
    unsupported_file_type = 0xFFFFFFFF
  };
  /*
  Description:
    Call this method to get supported file type from file name.
  Parameters:
    lpsFileName - [in] File name, extension will be extracted from the end of this string
    and compared to supported bitmap file types.
  Returns:
    Returns a valid bitmap_file_type on success or unsupported_file_type if the file type
    is not supported.
  */
  static bitmap_file_type FileTypeFromFileName(const wchar_t* lpsFileName);
  /*
  Description:
    Call this method to see if a specified file type supports 32 bit bitmaps with alpha channel.
  Parameters:
    fileType - [in] Bitmap file type to check
  Returns:
    Returns a true if the bitmap can be saved as a 32 bit DIB with alpha channel.
  */
  static bool FileTypeSupportsAlphaChannel(bitmap_file_type fileType);
  /*
  Description:
    Call this method to see if a specified file (gets the type from the file extension)
    supports 32 bit bitmaps with alpha channel.
  Parameters:
    fileName - [in] Name of the file to extract the extension from.
  Returns:
    Returns a true if the bitmap can be saved as a 32 bit DIB with alpha channel.
  */
  static bool FileTypeSupportsAlphaChannel(const wchar_t* fileName);

  /*
  Description:
    Call this method to see if the DIB appears to be a normal map.
  Returns:
    Returns true if the bitmap appears to be a normal map.  False otherwise.
  */
  ON_DEPRECATED_MSG("Use the version that takes parameters bLossyCompressionSource and bPositiveZComponent")
  bool IsNormalMap(void) const;

  /*
  Description:
    Call this method to see if the DIB appears to be a normal map.
  Parameters:
    bLossyCompressionSource - [in] True if the source of the bitmap is an image with lossy compression (e.g. jpg). False otherwise.
      The check will be less strict if the image can contain errors due to lossy compression.
    bPositiveZComponent - [out]
      True if the image is a normal map with the z-component mapped to the range 0 .. +1.
      False if the image is a normal map with the z-component mapped to the range -1 .. +1.
  Returns:
    Returns true if the bitmap appears to be a normal map. False otherwise.
  */
  bool IsNormalMap(bool bLossyCompressionSource, bool& bPositiveZComponent) const;

  void ConvertToNormalMap(bool bLossyCompressionSource, bool& bPositiveZComponent);

  const class CRhDibPrivate& Private(void) const;
  class CRhDibPrivate& Private(void);

protected:
  static int GetBitmapType(const wchar_t *lpsFileName);
  class CRhDibPrivate* m_dib_section = nullptr;
    
#if defined (ON_RUNTIME_COCOA_AVAILABLE) && defined(RHINODIB_CDC_SUPPORT)
    friend CDC::ProxyGraphicsContext;
    friend CDC;
#endif
};

RHINO_SDK_FUNCTION LPBITMAPINFO RhinoUiDIBCopy(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION int RhinoUiDIBColorDepth(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION int RhinoUiDIBColorDepth(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION int RhinoUiDIBNumColors(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION int RhinoUiDIBNumColors(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION long RhinoUiDIBPaletteSize(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION long RhinoUiDIBPaletteSize(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION long RhinoUiDIBWidth(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION long RhinoUiDIBWidth(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION long RhinoUiDIBHeight(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION long RhinoUiDIBHeight(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION DWORD RhinoUiDIBLineSize(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION DWORD RhinoUiDIBLineSize(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION DWORD RhinoUiDIBSizeBits(const BITMAPINFO bmi);
RHINO_SDK_FUNCTION DWORD RhinoUiDIBSize(const BITMAPINFO bmi);
RHINO_SDK_FUNCTION LPBITMAPINFO RhinoUiDIBCreate(long width, long height, int color_depth);
RHINO_SDK_FUNCTION LPBYTE RhinoUiDIBFindBits(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION HBITMAP RhinoUiDIBCreateHBitmap(const BITMAPINFO& bmi, HPALETTE hpal = nullptr);
RHINO_SDK_FUNCTION DWORD RhinoUiDIBScanWidth(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION bool RhinoUiDIBGet256Palette(const BITMAPINFO& bmi, RGBQUAD *rgb);
RHINO_SDK_FUNCTION bool RhinoUiDIBSet256Palette(BITMAPINFO& bmi, RGBQUAD *rgb);
RHINO_SDK_FUNCTION bool RhinoUiDIBSet256PalColors(BITMAPINFO& bmi);
RHINO_SDK_FUNCTION bool RhinoUiDIBSet256IdentityPalette(BITMAPINFO& bmi, HPALETTE hpal);
RHINO_SDK_FUNCTION bool RhinoUiSetDIBGray256Palette(BITMAPINFO& bmi, HPALETTE hpal, COLORREF base_color = RGB(0, 0, 255));
RHINO_SDK_FUNCTION HANDLE RhinoUiDIBCopyToHandle(const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION bool RhinoUiDIBCopyToClipboard(HWND hwnd, const BITMAPINFO& bmi);
RHINO_SDK_FUNCTION LPBITMAPINFO RhinoUiDIBCopyFromClipboard(HWND hwnd);
RHINO_SDK_FUNCTION bool RhinoUiDIBBltToDib(
  BITMAPINFO& bmi_target,
  LPBYTE dest_bits,
  long x_dest,
  long y_dest,
  long cx,
  long cy,
  BITMAPINFO& bmi_source,
  LPBYTE src_bits,
  long x_src,
  long y_src);

RHINO_SDK_FUNCTION long RhinoUiDIBXPixelsPerMeter(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION long RhinoUiDIBXPixelsPerMeter(const BITMAPINFO& bmi);

RHINO_SDK_FUNCTION long RhinoUiDIBYPixelsPerMeter(const BITMAPINFOHEADER& bmih);
RHINO_SDK_FUNCTION long RhinoUiDIBYPixelsPerMeter(const BITMAPINFO& bmi);

RHINO_SDK_FUNCTION HBITMAP RhinoUiCopyHBitmap(HBITMAP hBitmapSrc);
BITMAPINFO *RhinoUiDIBHBitmapToDIB(HBITMAP hBitmap, HPALETTE hpal = nullptr);

#if defined (ON_RUNTIME_WIN)
typedef CRhinoDib ON_DEPRECATED_MSG("use CRhinoDib") CRhinoUiDib;
#endif

  /*
  Description:
    Inserts dib into Rhino's texture manager and returns an ON_FileReference
    for use in e.g. ON_Texture::m_image_file_reference. This way it's possible
    to use a dib with an ON_Texture without having to read the dib from file.
  Parameters:
    dib - [in] The dib which will be referenced by the ON_FileReference.
    crc - [in] The crc of the dib. This should be a unique number which
               changes if the contents of the dib changes.
               NOTE: if a different dib is provided using the same crc as
               a previous dib, then the previous dib will be overwritten
               in the texture manager and both previously returned
               ON_FileReferences will reference the newly provided dib.
  Returns:
    Returns an ON_FileReference which, when used together with e.g.
    ON_Texture::m_image_file_reference, is used by Rhino's display to find 
    the texture from the texture manager.
  */
RHINO_SDK_FUNCTION
ON_FileReference RhinoGetDibAsTextureFileReference(const CRhinoDib& dib, ON__UINT32 crc);


