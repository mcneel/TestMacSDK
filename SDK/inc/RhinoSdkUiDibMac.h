//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#if defined(ON_RUNTIME_APPLE_IOS)
#include <UiKit/UIKit.h>
#endif

#pragma once

void RhinoUiDibInstallMemManager( void* (*my_calloc)( size_t num, size_t size),
                                  void* (*my_malloc)( size_t size),
                                  void  (*my_free)( void* memblock)
                                 );

void* RHINOUI_DIB_CALLOC( size_t num, size_t size);
void* RHINOUI_DIB_MALLOC( size_t size);
void RHINOUI_DIB_FREE( void* memblock);

// Class to manage DIB's. This class will create a DIB, a HBITMAP and associated memory
// DC.  The HBITMAP and CRhinoDib::FindBits() point to the same memory.  You can modify the
// bits directly or use GDI with the associated memory DC to modify the bitmap. The HBITMAP
// does not get selected by the memory devidce context due to the fact that the DIB can
// only be selected in one device context at a time.  To select or remove the Bitmap() from
// its memory DC call DCSelectBitmap( bool).  If you call DCSelectBitmap( true) you may
// cast a CRhinoDib as a CDC* or HDC and use it directly.
class RHINO_SDK_CLASS CRhinoDib
{
public:
  // Construction
  CRhinoDib();
  // create bitmap with specified width, height and color depth
  CRhinoDib( int width, int height, int color_depth, bool select_into_mem_dc = false, CPalette* = NULL);
  // copy constructor, create bitmap that matches another
  CRhinoDib( const CRhinoDib&);
  // create Dib from hbitmap
  CRhinoDib( HBITMAP);

  CRhinoDib( CGImageRef);
  void SetBitmap( CGImageRef );

  void SetBitmap(HBITMAP);

  // Destruction
  virtual ~CRhinoDib();
  // Destroy dib && hbitmap and zero out pointers
  void DestroyDib();

  unsigned int RuntimeSerialNumber() const;
  CRhinoCacheHandle* Cache() const;

  // Copy source bitmap, if m_hBitmap is non NULL then create destroy current and
  // create new hBitmap
  CRhinoDib& operator =( const CRhinoDib& src);

  operator CDC*();
  operator CDC*() const;

  // Clears (fills) the entire dib with the passed in color...
  void Clear(COLORREF);

  // If color depth is less than the requested color depth create a new
  // bitmap of the desired color depth and copy old bits to it
  bool IncreaseColorDepth( int nNewColorDepth);

  // Converts the dib into a grayscale image...and promotes it to a
  // 24 bit dib if need be...
  // If bSetAlpha is true, and the bitmap is a 32-bit image, the alpha byte is set to the same as the other three components
  // otherwise, the alpha byte is preserved from the original image.
  bool ConvertToGrayScale(bool bSetAlpha = false);

  // DIB or HBITMAP creation functions
  bool CreateDib( int width, int height, int color_depth, bool select_into_mem_dc = false, CPalette* = NULL);

  // Destroy existing m_bitmapinfo and m_hBitmap, set them to specified dib.
  // If Bitmap() was non NULL then a new HBITMAP is created from the new dib
  BOOL32 SetDib( LPBITMAPINFO lpbmi, bool free_lpbmi = true);

  // Return copy of the current Dib().  If Bitmap() is NULL then one is created
  // from Dib() and returned.  m_hBitmap is not set if Bitmap was NULL.
  // You are responsible for destroying this object.
  HBITMAP CopyHBitmap();

  // Clipboard operations
  BOOL32 CopyToClipboard( HWND hwnd) const;

  // Blit functions functions
  BOOL32 Draw( CDC* pDC, int x = 0, int y = 0, int cx = 0, int cy = 0, int xSrc = 0, int ySrc = 0);
  BOOL32 Draw( CDC* pDC, CRect rect, int xSrc = 0, int ySrc = 0);
  BOOL32 DrawMasked( CDC* pDC, COLORREF crMask, int x = 0, int y = 0, int cx = 0, int cy = 0, int xSrc = 0, int ySrc = 0);
  BOOL32 DrawMasked( CDC* pDC, COLORREF crMask, CRect rect, int xSrc = 0, int ySrc = 0);

  // Bitmap information
  long Size() const;
  long SizeHeader() const;
  long SizeBits() const;
  int  ColorDepth() const;
  long NumColors() const;
  long PaletteSize() const;
  CPalette* Palette() const;
  int Width() const;
  long WidthInBytes() const;
  int Height() const;
  long ScanWidth() const;

  // OpenGL attributes for this image
  UINT GLPixelFormat() const;       // e.g. GL_RGBA
  UINT GLDataType() const;          // e.g. GL_UNSIGNED_BYTE

  void Resize (int newWidth, int newHeight);
  bool ReuseDib( int width, int height, int depth, bool select_into_mem_dc);

  //Call this to ensure there is memory allocated.  Replace calls to check FindDIBBits() for nullptr with this.
  bool IsAllocated() const;

  enum class ComponentOrders : int
  {
    None = 0,
    L    = 1,
    BGR  = 2,
    RGB  = 3,
    BGRA = 4,
    RGBA = 5,
    ABGR = 6,
    ARGB = 7,
    FLOAT = 8,
    RUNTIME_24BIT = 3,
    RUNTIME_32BIT = 5
  };

  ComponentOrders ComponentOrder(void) const;

  static ComponentOrders GLFormatToComponentOrder(GLenum);

  /*
  Description:
    Get bitmap's bits.  Note that on the Mac, this function is not to be used, because byte order cannot be guaranteed.
  Returns:
    A pointer to the bits.  Look but do not change.
  See Also:
    CRhinoDib::BitmapInfo
  */
private:
  LPBYTE FindDIBBits(CRhinoDib::ComponentOrders) const;
  BITMAPINFO DibInfo() const;

  //This is a list of the badly behaved
  friend class CRhRdkPostEffectImage;
  friend class CXXXXRender;
  friend class CRhCmnDisplayBitmap;
  friend class CRhEngine_GL11;
  friend class CRhOpenGLTexture2D;
  friend class CRhinoDisplayEngine_OGL;
  friend class CRhinoDisplayEngine_GDI;
  friend class CRhinoDisplayPipeline;
  friend class CRhOpenGLTextureCube;
  //friend class CRhinoZBuffer;
  //friend class CRhinoRenderOverlays;
  friend class CRhOpenGLRenderer;
  friend class CRhRegionSelectHelper;
  friend class CRhResourceManagerMetal;

  //friend CRhinoDib* RhinoCreateAnaglyph(const CRhinoDib&, const CRhinoDib&,const int);
  //friend void ConvertDepthToImage(CRhinoDib&, bool);
  friend GLuint GetGlTextureForDib(class CRhinoDisplayEngine_OGL&, const CRhinoDib&);
  friend unsigned int CreateGlAlphaTexture(const CRhinoDib&, bool, unsigned int);
  friend bool RhinoUpdateCachedTextureFromDib(unsigned int, const CRhinoDib&);
#if defined (RHINO_MAC_APPLICATION)
  friend void ApplyBufferImageAttributes(class CRhOpenGLTexture2D*, const wchar_t* name, const class CRhTextureAttributes&, class CRhinoDisplayEngine_OGL*, HGLRC);
#endif
  //friend void Combine(const CRhinoDib&, CRhinoDib&, const float*, float*);
	friend class CRhEngine_PEP_GL33;
	friend class TestDavidEBitmapFlip;

public:
  class Pixel
  {
  public:
    void Set(unsigned char in_r, unsigned char in_g, unsigned char in_b, int in_alpha);

    BITMAPINFO& bmi;
    const CRhinoDib* pDib;

    //Only set this value when the exact format is known (8-bit RGBA or RGB)
    //Note that this is Mac only.
    unsigned char* pRGBAPixel8;
    ON__UINT_PTR reserved;

    int color_depth;
    long x;
    long y;
    unsigned char r, g, b;
    int alpha;

	//To support DIBs where a 32-bit float type is stored in the 32 bits of each pixel.  Does not work on non-32-bit images.
	float Float(void) const;
	void Set(float);

    Pixel(BITMAPINFO& in_bmi, const CRhinoDib* in_dib, int in_cd, long in_x, long in_y, unsigned char in_r, unsigned char in_g, unsigned char in_b, int in_alpha, unsigned char* in_pRGBAPixel8=nullptr)
		  :
		  bmi(in_bmi), pDib(in_dib), color_depth(in_cd), x(in_x), y(in_y), r(in_r), g(in_g), b(in_b), alpha(in_alpha), pRGBAPixel8(in_pRGBAPixel8)
    {}
  };

  //Fast pixel iterators.  Quick access to the image data for each pixel, multi-threaded.
  //Use the pixel setter provided to quickly set pixel values.
  void ProcessPixels(void (*f)(Pixel&, void* pvData), void* pvData=nullptr);
  void ProcessPixels(void (*f)(Pixel&, const void* pvData), const void* pvData=nullptr);
  void ProcessPixels(void (*f)(const Pixel&, void* pvData), void* pvData=nullptr) const;

  void ProcessPixels_SingleThreaded(void (*f)(Pixel&, void* pvData), void* pvData=nullptr);
  void ProcessPixels_SingleThreaded(void (*f)(Pixel&, const void* pvData), const void* pvData = nullptr);
  void ProcessPixels_SingleThreaded(void (*f)(const Pixel&, void* pvData), void* pvData=nullptr) const;

  //Pixel iterators with an interrupt.  Return false in the callback function to interrupt the loops.
  //Current implementation is not multithreaded, but this may change.
  bool ProcessPixels(bool (*f)(Pixel&, void* pvData), void* pvData=nullptr);
  bool ProcessPixels(bool (*f)(const Pixel&, void* pvData), void* pvData=nullptr) const;

  bool GetPixel( unsigned char *base, int x, unsigned char *r, unsigned char *g, unsigned char *b);

  MAC_DEPRECATED HBITMAP Bitmap() const;   // returns this, which is not very useful

#if defined(ON_RUNTIME_COCOA_AVAILABLE)
  NSImage* Image() const;
  NSBitmapImageRep* BitmapImageRep() const;
  void BitmapImageRep (NSBitmapImageRep* newBitmapRep);
  static void FlipVertical (NSBitmapImageRep* imageRep);
#endif
    
  // exchange the red and blue color bytes in the NSBitmapImageRep
  void SwapRedAndBlueSamples();

  // convert all partially transparent pixels to fully opaque
  void MakeOpaque();

  // From RhinoSdkDib.h
  // Fills the entire dib with the specified color...Note: if it's a 32
  // bit dib, then the high-order byte of the passed in color is considered
  // to be an alpha value, and thus will also be included in the fill
  // operation (normal GDI operations will strip this out).
  BOOL32 FillSolidRect (int x, int y, int nWidth, int nHeight, ON_Color color );
  BOOL32 FillSolidRect (CRect& rect, ON_Color color );
  BOOL32 FillSolid (ON_Color color);

  void FlipVertical ();

  // Functions call FindDIBBits and validate x and y
  BOOL32 SetPixel( long x, long y, float r, float g, float b, float alpha = -1.0);
  BOOL32 SetPixel( long x, long y, int r, int g, int b, int alpha = -1);
  BOOL32 SetPixel( long x, long y, COLORREF cr, int alpha = -1);

  BOOL32 GetPixel( long x, long y, COLORREF& cr) const;
  BOOL32 GetPixel( long x, long y, COLORREF& cr, int& alpha) const;
  BOOL32 GetPixel( long x, long y, int& r, int& g, int& b) const;
  BOOL32 GetPixel( long x, long y, int& r, int& g, int& b, int& alpha) const;
  BOOL32 GetPixel( long x, long y, float& r, float& g, float& b) const;
  BOOL32 GetPixel( long x, long y, float& r, float& g, float& b, float& alpha) const;
      
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

  BOOL32 BitBlt (int x, int y, int nWidth, int nHeight,
               const CRhinoDib* srcDC,
               int srcLeft, int srcTop,
               int mode);

  BOOL32 StretchBlt (int x, int y, int nWidth, int nHeight,
                   const CRhinoDib* srcDC,
                   int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
                   int mode);

  BOOL32 DCSelectBitmap( bool );  // don't know what this might do

  UINT GetDepth() const;
  UINT GetStorageWidth() const;
  UINT GetStorageSize() const { return GetStorageWidth() * Height();}
  UINT PelsPerMeter() const;
  void SetPelsPerMeter(UINT);
  double DPI() const;
  void SetDPI(double);

  void GaussianBlurWithRadius (double radius);

protected:
  void construct();

  void MakeNSImage();
#if defined (ON_RUNTIME_COCOA_AVAILABLE)
  // Native OS X NSBitmapImageRep data is stored top row first.  Windows bitmaps
  // are stored bottom row first.  Rhino code expects to access the raw pixels in a
  // CRhinoDib, so the pixels stored in m_image are stored bottom row first.
  NSBitmapImageRep* m_image_rep = nullptr;
  void SetImageRep (NSImageRep*);

  // NSImage that contains the above NSBitmapImageRep
  NSImage* m_nsimage = nullptr;
#endif
  
#if defined(ON_RUNTIME_APPLE_IOS)
  mutable ON_SimpleArray<unsigned char> m_raw_data;
#endif

  // This Mac-only CDC points at ourself.  We deliberately use an instantiated CDC object rather than
  // using the Windows m_pDC so the two very different CDC implementations (Mac and Windows)
  // do not collide and the Mac CDC cannot be deleted independently of this DIB.
  CDC m_dc;

  int m_width;
  int m_height;
  unsigned int m_color_depth;
  bool m_is_gray_scale;
  double m_dpi;

  /////////////////////////////////////////////////////////////////////////////
  // From RhinoSdkDib.h
  //---------------------------------------------------------------------------
public:
  enum class Quality : int
  {
    kHigh,
    kMedium,
    kLow
  };

  bool WriteToFile(HWND hWnd = 0, bool bUseDialogs = true, Quality q = Quality::kHigh) const;
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
   false - File dialog canceled or error writing file.
   */
  bool ReadFromFile(HWND hWnd = 0, bool bUseDialogs = true);

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
  lpsFileName - [in] Name of file to read.  Uses
  CRhinoFileUtilities::FindFile(...) to validate file name.
  doc_serial_number  - [in] Runtime serial number of the CRhinoDoc to be used for image searches
  Returns:
  true  - File was read from disk.
  false - Error reading file or invalid file type.
  */
  bool ReadFromFile(unsigned int doc_serial_number, const wchar_t* lpsFileName);

  /*
   Description:
   Read bitmap image size from disk.
   Parameters:
   lpsFileName - [in]
   Name of file to read.  Uses
   CRhinoFileUtilities::FindFile(...) to validate file name.

   image_width_pixels - [out]
   image_height_pixels - [out]
   The sizes of the image are returned in the pixels parameters.

   image_width_dpi - [out]
   image_height_dpi - [out]
   The dots per inch values saved in the image file are
   returned in the dpi parameters.
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
                      int sizeof_buffer
                      );

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
  bool Print(HWND hWnd = 0, bool bFitToPage = true);
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

protected:
  static int GetBitmapType(const wchar_t *lpsFileName);

  mutable ON_wString m_wLastSaveName;
  virtual void NonConstOperation();
private:
  unsigned int m_runtime_serial_number = 0;
  mutable CRhinoCacheHandle m_display_cache;
  ON__UINT_PTR m_sdk_reserved = 0;

  unsigned char* InternalDibBits(void) const;
};

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
