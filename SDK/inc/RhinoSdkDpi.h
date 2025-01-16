/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2016 Robert McNeel & Associates. All rights reserved.
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

/*
Description:
  A set of static utility functions to help with high-DPI displays
*/

class RHINO_SDK_FUNCTION CRhinoDpi
{
public:
  /*
  Returns:
    The display DPI scale factor when Rhino started.
  */
  static int DpiScale();
  
  /*
  Returns:
    The display DPI value when Rhino started.
  */
  static int ScreenDpi();

  /*
  Returns:
    True if the DPI scale factor was set to 100% when Rhino started, false otherwise.
  */
  static bool DpiScaleIsOne();

  /*
  Description:
    Scales a value by the current DPI scale factor.
  Parameters:
    x [in] - The value to scale
  Returns:
    The value multiplied by the DPI scale.
  */
  static int Scale(int x);
    
  /*
     Description:
     Standard icon types.
     */
    enum class IconType : unsigned int
    {
        NormalIcon,
        SmallIcon
    };

    /*
     Description:
     Returns the preferred sizes of icons. The measurement values that are returned are scaled to the current DPI setting.
     Parameters:
     iconType [in] - The icon type.
     Returns:
     The size in pixels.
     */
    static int IconSize(IconType iconType);

#if defined(ON_OS_WINDOWS)

  /*
  Description:
    Scales a SIZE by the current DPI scale factor.
  Parameters:
    lpSize [in/out] - The size
  Returns:
    Nothing
  */
  static void ScaleSize(LPSIZE lpSize);

  /*
  Description:
    Scales a RECT by the current DPI scale factor.
  Parameters:
    lpRect [in/out] - The rectangle
  Returns:
    Nothing
  */
  static void ScaleRect(LPRECT lpRect);

  /*
  Description:
    Creates an image list from an icon.
  Parameters:
    imageList [in/out] - The image list to create.
    hInstance [in]     - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconId    [in]     - The resource id of the icon to load.
    nSize     [in]     - The desired size, in pixels, of the icon.
  Returns:
    True if successful, false otherwise.
  */
#if defined (RHINO_SDK_MFC)
  static bool CreateImageList(CImageList& imageList, HINSTANCE hInstance, unsigned int iconId, int nSize);
#endif

  /*
  Description:
    Creates an image list from an icon.
  Parameters:
    imageList [in/out] - The image list to create.
    hInstance [in]     - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconId    [in]     - The resource id of the icon to load.
    cxDesired [in]     - The desired width, in pixels, of the icon.
    cyDesired [in]     - The desired height, in pixels, of the icon.
    Returns:
    True if successful, false otherwise.
  */
#if defined (RHINO_SDK_MFC)
  static bool CreateImageList(CImageList& imageList, HINSTANCE hInstance, unsigned int iconId, int cxDesired, int cyDesired);
#endif

  /*
  Description:
    Creates an image list from an array of icons.
  Parameters:
    imageList [in/out] - The image list to create.
    hInstance [in]     - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconIds   [in]     - The resource ids of the icons to load.
    nSize     [in]     - The desired size, in pixels, of the icon.
  Returns:
    The number of icons added to the imagelist.
  */
#if defined (RHINO_SDK_MFC)
  static int CreateImageList(CImageList& imageList, HINSTANCE hInstance, ON_SimpleArray<unsigned int> iconIds, int nSize);
#endif

  /*
  Description:
    Creates an image list from an array of icons.
  Parameters:
    imageList [in/out] - The image list to create.
    hInstance [in]     - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconCount [in]     - The number of icons to load.
    iconIds   [in]     - The resource ids of the icons to load.
    nSize     [in]     - The desired size, in pixels, of the icon.
  Returns:
    The number of icons added to the imagelist.
  */
#if defined (RHINO_SDK_MFC)
  static int CreateImageList(CImageList& imageList, HINSTANCE hInstance, int iconCount, const unsigned int* iconIds, int nSize);
#endif

  /*
  Description:
    Loads an icon. If the icon is not a standard size, this function scales down a larger image.
  Parameters:
    hInstance [in] - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconId    [in] - The resource id of the icon to load.
    iconType  [in] - The icon type
  Returns:
    A handle to the icon if successful.
  Remarks:
    Do not call the Win32 DestroyIcon() function on the returned value.
    Rhino will destroy the icon for you.
  */
  static HICON LoadIcon(HINSTANCE hInstance, unsigned int iconId, IconType iconType);

  /*
  Description:
    Loads an icon. If the icon is not a standard size, this function scales down a larger image.
  Parameters:
    hInstance [in] - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconId    [in] - The resource id of the icon to load.
    nSize     [in] - The desired size, in pixels, of the icon.
  Returns:
    A handle to the icon if successful.
  Remarks:
    Do not call the Win32 DestroyIcon() function on the returned value.
    Rhino will destroy the icon for you.
  */
  static HICON LoadIcon(HINSTANCE hInstance, unsigned int iconId, int nSize);
  
  /*
  Description:
    Loads an icon. If the icon is not a standard size, this function scales down a larger image.
  Parameters:
    hInstance [in] - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    iconId    [in] - The resource id of the icon to load.
    cxDesired [in] - The desired width, in pixels, of the icon.
    cyDesired [in] - The desired height, in pixels, of the icon.
  Returns:
    A handle to the icon if successful.
  Remarks:
    Do not call the Win32 DestroyIcon() function on the returned value.
    Rhino will destroy the icon for you.
  */
  static HICON LoadIcon(HINSTANCE hInstance, unsigned int iconId, int cxDesired, int cyDesired);

  /*
  Description:
    Loads an icon from an unloaded Rhino C++ plug-in DLL. 
    If the icon is not a standard size, this function scales down a larger image.
  Parameters:
    hInstance [in] - A handle to the module of either a DLL or EXE file that contains the icon to be loaded.
    cxDesired [in] - The desired width, in pixels, of the icon.
    cyDesired [in] - The desired height, in pixels, of the icon.
  Returns:
    A handle to the icon if successful.
  Remarks:
    Do not call the Win32 DestroyIcon() function on the returned value.
    Rhino will destroy the icon for you.
  */
  static HICON LoadIcon(const wchar_t* pszPath, int cxDesired, int cyDesired);

  /*
  Description:
    Returns the item height of a CListBox. Good for setting item heights for owner-drawn list boxes,
    or check list boxes (which are owner-drawn).
  Parameters:
    listbox [in] - An instance of a list box.
  Returns:
    A useful item height in pixels.
  */
#if defined (RHINO_SDK_MFC)
  static int ListBoxItemHeight(CListBox& listbox);
#endif

  /*
  Description:
    Returns the item height of a CComboBox. Good for setting item heights for owner-drawn combo boxes.
  Parameters:
    listbox [in] - An instance of a list box.
  Returns:
    A useful item height in pixels.
  */
#if defined (RHINO_SDK_MFC)
  static int ComboBoxItemHeight(CComboBox& combobox);
#endif

  /*
  Description:
    Returns the size of a checkbox in pixels. Useful for those owner-drawing or
    theme drawing checkbox buttons.
  Parameters:
    hwnd [in] - The handle of the window doing the drawing.
  Returns:
    A useful size in pixels.
  */
  static ON_2iSize CheckBoxSize(HWND hWnd);

  /*
  Description:
    Returns the size of a menu check in pixels. Useful for those owner-drawing or
    theme drawing menus.
  Parameters:
    hwnd [in] - The handle of the window doing the drawing.
  Returns:
    A useful size in pixels.
  */
  static ON_2iSize MenuCheckSize(HWND hWnd);

  /*
  Description:
    Returns the size of a tree view +/- icons in pixels. Useful for those owner-drawing or
    theme drawing tree like controls.
  Parameters:
    hwnd [in] - The handle of the window doing the drawing.
    bHot [in] - return either a 'normal' or a 'hot' image size.
    Returns:
    A useful size in pixels.
  */
  static ON_2iSize TreeViewExpandIconSize(HWND hWnd, bool bHot = false);

  /*
  Description:
    Gets the tree view +/- icons. Useful for those owner-drawing or
    theme drawing tree like controls.
  Parameters:
    hwnd [in] - The handle of the window doing the drawing. If NULL,
                then Explorer-themed images will be returned. If
                a valid window handle, then images based on the windows's
                thheme will be returned.
    opened [out] - opened icon
    closed [out] - collapsed icon
    bHot   [in]  - return either a 'normal' or a 'hot' image.
  Returns:
    Returns true if the bitmaps were successfully created otherwise false.
  */
  static bool TreeViewExpandIcons(HWND hWnd, CRhinoDib& opened, CRhinoDib& closed, bool bHot = false);

  /*
  Description:
    Calculate height of font selected in the specified device context and
    add the padding * DPI scale
  Returns:
    The height in pixels.
  */
  static int HeightFromDeviceContextFont(HDC dc, int padding);

  /*
  Description:
    Calculate height of window font in the specified window and
    add the padding * DPI scale
  Returns:
    The height in pixels.
  */
  static int HeightFromWindowFont(HWND wnd, int padding);

#endif // ON_OS_WINDOWS
};

