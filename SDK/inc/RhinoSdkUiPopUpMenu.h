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

#if defined (RHINO_MAC_APPLICATION)
@class MRUIPopupMenu;
#endif


#pragma once

// RhinoSdkPopUpMenu.h
//

//
// CRhinoUiPopupMenu
//
class RHINO_SDK_CLASS CRhinoUiPopupMenu
{
public:
  CRhinoUiPopupMenu( UINT nResourceID = 0, HINSTANCE hInst = NULL);
  CRhinoUiPopupMenu( LPCTSTR lpsResourceID, HINSTANCE hInst = NULL);

  virtual ~CRhinoUiPopupMenu(void);

public:
  // Add items in order you want them to display
  int AddItem( const char*);
  int AddItem( const wchar_t*);
  // Add seportaor (this will not have an index)
  void AddSeporator();
  // Call using index returned by AddItem to enable/disable a
  // menu item.  The default behavior is to enable everything.
  void EnableItem( int nItem, bool bEnable = true);
  // Called when the menu selection changes, returns index of selected item or -1 if nothing is selected
  virtual void OnMenuSelect( HWND hWnd, HMENU hMenu, int index);
  // Called when WM_INITMENU message received for context menu.
  virtual void OnInitMenu( HWND hWnd, HMENU hMenu);

  // Display popup menu, returns -1 on error or if escape pressed otherwise returns index of added item.
  virtual int PopUp( CWnd*,   // Window whos coord system the points are in
	  const ON_2iPoint&,  // Bottom point.  Top left corner of the popup.  This is used by default if there is enough
                              // screen space to display the popup menu on the bottom
	  const ON_2iPoint&); // Top point.  Bottom left corner of the poupup.  This is used if there is not enough space to display
                              // the menu below the window.

  // Display popup menu, returns -1 on error or if escape pressed otherwise returns index of added item.
  int PopUpEx( CWnd*,   // Window whos coord system the points are in
	  const ON_2iPoint&,  // Bottom point.  Top left corner of the popup.  This is used by default if there is enough
                        // screen space to display the popup menu on the bottom
	  const ON_2iPoint&,  // Top point.  Bottom left corner of the poupup.  This is used if there is not enough space to display
                        // the menu below the window.
               int);    // 0 = Not transparent, 1 to 100 = percentage of tranparency.

  bool LeftButtonPicked(); // returns true if menu dismissed with left button
  bool RightButtonPicked(); // returns true if menu dismissed with right button
  const ON_2iPoint* PointPicked(); // return NULL if LeftButtonPicked() and RightButtonPicked() are false otherwise returns point where mouse went down. 
  MSG* LastMessage(); // return NULL if left or right button picked is not true otherwise return pointer to message structure for message.

  // For internal use only
  void PopupWndProcOnInitMenu( HWND hWnd, HMENU hMenu);
  LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  HMENU GetMenuHandle();

public:
  int m_menu_select;
  bool m_menu_char;
protected:
  bool CheckDefaults( LPCTSTR nResourceID, HINSTANCE hInst);
  void Init();

  CStringArray m_item_array;
  HMENU        m_hMenu;
  HWND         m_hWndParent;
  MSG          m_msg;
  bool         m_bLeft;
  bool         m_bRight;
  CUIntArray   m_nEnabledList;
  ON_2iPoint   m_point;
#if defined (RHINO_MAC_APPLICATION)
  MRUIPopupMenu*   m_popupMenu;
#endif
public:
  // For internal use only
  class CRhinoUiPopupMenuExtension* m__RhinoUiPopupMenu_extension;
};
