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

namespace RhinoTabbedDockBar { class CCollection; }

class RHINO_SDK_CLASS CRhinoUiDockBarManager : private ON_SimpleArray<CRhinoUiDockBar*>
{
public:
  CRhinoUiDockBarManager(void);
  ~CRhinoUiDockBarManager(void);
  //
  // Description:
  //   Get number of dock bars associated with Rhino.
  //
  // Returns:
  //   Number of dock bars associated with Rhino.
  //
  // See Also:
  //   CRhinoUiDockBar* CRhinoUiDockBarManager::DockBar( int index)
  //   CRhinoUiDockBar* CRhinoUiDockBarManager::operator[]( int index)
  int DockBarCount() const;
  //
  // Description:
  //   The dock bar manager is a collection of CRhinoUiDockBars.  This
  //   function may be used to iterate the dock bar collection.
  //
  // Parameters:
  //   index [in] Zero based index for dock bar to return.
  //
  // Returns:
  //   Pointer to dock bar if index is valid otherwise NULL.
  //
  // See Also:
  //   int CRhinoUiDockBarManager::DockBarCount() const
  //   CRhinoUiDockBar* CRhinoUiDockBarManager::operator[]( int index)
  CRhinoUiDockBar* DockBar( int index) const;
  //
  // Description:
  //   The dock bar manager is a collection of CRhinoUiDockBars.  This
  //   function may be used to iterate the dock bar collection.
  //
  // Parameters:
  //   index [in] Zero based index for dock bar to return.
  //
  // Returns:
  //   Pointer to dock bar if index is valid otherwise NULL.
  //
  // See Also:
  //   int CRhinoUiDockBarManager::DockBarCount() const
  //   CRhinoUiDockBar* CRhinoUiDockBarManager::DockBar( int index)
  CRhinoUiDockBar* operator[]( int index) const;
  //
  // Description:
  //   The dock bar manager is a collection of CRhinoUiDockBars.  Each
  //   dock bar has a unique UUID.  This function may be used to see if
  //   a dock bar with the specified UUID is currently in the collection.
  //
  // Parameters:
  //   uuid [in] UUID of dock bar to search for
  //
  // Returns:
  //   Pointer to dock bar if UUID is found otherwise NULL.
  CRhinoUiDockBar* FindDockBar( UUID uuid) const;
  //
  // Description:
  //   The dock bar manager is a collection of CRhinoUiDockBars.  Each
  //   dock bar must have a name.  There is no requirement for names to
  //   be unique.  This function may be used to find a dock bar with the
  //   specified name.  To get a specific dock bar you should use 
  //   FindDockBar( UUID uuid).
  //
  // Parameters:
  //   control_bar_name [in] Name of dock bar to search for
  //   language_id [in] Localized language version control_bar_name is using
  //
  // Returns:
  //   Pointer to first dock bar with control_bar name or NULL if name is not found.
  CRhinoUiDockBar* FindDockBar( const wchar_t* control_bar_name, int language_id = 1033 /*English*/);
  // Description:
  //   Use this to set CRhinoUiDockBar::m_sort_index so that
  //   the values of m_sort_index run from 0 to DockBarCount()-1
  //   and compare(control_bar_list[i], control_bar_list[j]) < 0) implies 
  //   control_bar_list[i].m_sort_index < control_bar_list[j].m_sort_index.
  //
  // Parameters:
  //   compar - [in] compare function with prototype
  //       int compare(const CRhinoUiDockBar* arg1, const CRhinoUiDockBar* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       BOOL32 cull(const CRhinoUiDockBar* arg,void* p).
  //       that returns TRUE if the dock bar should be ignored
  //       when sorting.  Dock bars that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull the dock bar list.
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoUiDockBar pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the dock bar manager.  They are intended to be used to get
  //   sorted lists of dock bars for dialogs, etc.
  //
  // See Also:
  //   CRhinoUiDockBarManager::GetSortedList
  void Sort(  int (*compar)(CRhinoUiDockBar*const*,CRhinoUiDockBar*const*,void*),
              int (*cull)(const CRhinoUiDockBar*,void*),
              void* p=0
            );
  //
  // Description:
  //   This function is provided to simplify sorting by name. To use pass
  //   this function to CRhinoUiDockBarManager::Sort as the compar parameter and
  //   pass a CSortDockBarsByName pointer as p.
  //
  // Example:
  //   ::RhinoUiDockBarManager().Sort( CRhinoUiDockBarManager::SortDockBarsByName,
  //                                        NULL,
  //                                        &CRhinoUiDockBarManager::CSortDockBarsByName()
  //                                      );
  //   ON_SimpleArray<CRhinoUiDockBar*>sorted_list;
  //   ::RhinoUiDockBarManager().GetSortedList( sorted_list);
  //   
  // See Also:
  //   CRhinoUiDockBarManager::Sort
  //   CRhinoUiDockBarManager::GetSortedList
  //   CRhinoUiDockBarManager::CullNotCreatedDockBars
  class CSortDockBarsByName
  {
  public:
    CSortDockBarsByName( int language_id = 1033 /*English*/, bool ascending = true)
    : m_language_id( language_id), m_ascending( ascending)
    {}
    int m_language_id;
    bool m_ascending;
  };
  static int SortDockBarsByName( CRhinoUiDockBar* const*, CRhinoUiDockBar* const *, void*);
  //
  // Description:
  //   This function is provided to simplify culling dock bars that have not
  //   been created when calling CRhinoUiDockBarManager::Sort(). To use pass
  //   this function to CRhinoUiDockBarManager::Sort as the cull parameter.
  //
  // Example:
  //   ::RhinoUiDockBarManager().Sort( CRhinoUiDockBarManager::SortDockBarsByName,
  //                                        CRhinoUiDockBarManager::CullNotCreatedDockBars,
  //                                        &CRhinoUiDockBarManager::CSortDockBarsByName()
  //                                      );
  //   ON_SimpleArray<CRhinoUiDockBar*>sorted_list;
  //   ::RhinoUiDockBarManager().GetSortedList( sorted_list);
  //   
  // See Also:
  //   CRhinoUiDockBarManager::Sort
  //   CRhinoUiDockBarManager::GetSortedList
  //   CRhinoUiDockBarManager::SortDockBarsByName
  static int CullNotCreatedDockBars( const CRhinoUiDockBar*, void*);
  // Description:
  //   Gets an array of pointers to dock bars  that is sorted by
  //   the values of CRhinoUiDockBar::SortIndex().
  //
  // Parameters:
  //   sorted_list - [out] this array is returned sorted by the values of
  //            CRhinoUiDockBar::SortIndex().  This list may be a different length
  //            than the dock bar list if ignore_not_created is true and/or a cull
  //            function was used by Sort().
  //   ignore_not_created - [in] TRUE means don't include dock bars that have not
  //                             been created.
  //
  // Remarks:
  //   Use CRhinoUiDockBar::Sort() to set the values of m_sort_index.
  //
  // See Also:
  //   CRhinoUiDockBarManager::Sort();
  //   CRhinoUiDockBar::SortIndex();
  void GetSortedList( ON_SimpleArray<CRhinoUiDockBar*>& sorted_list,
                      bool ignore_not_created = false
                    ) const;
  //
  // Description:
  //   Get index for specified CRhinoUiDockBar object
  //
  // Parameters:
  //   pCB [in] Dock bar to search for
  //
  // Returns:
  //   -1 If dock bar not in list otherwise zero based index
  //   for dock bar.
  //
  int IndexFromPointer( CRhinoUiDockBar* pCB) const;
  //
  // Description:
  //   Show or hide specified dock bar.
  //   DO NOT call RhinoUiMainFrame().ShowDockBar(), use this method instead.
  //
  // Parameters:
  //   uuid [in] ID for dock bar to show or hide.
  //   show_control_bar [in] If true show dock bar otherwise hide it
  //   delay_recalc_layout [in] See MSDN help for CFrameWnd::ShowDockBar
  //
  // Returns:
  //   True if successful, false otherwise.
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   CFrameWnd::ShowDockBar()
  //
  bool ShowDockBar( UUID uuid, bool show_control_bar, bool delay_recalc_layout) const;
  //
  // Description:
  //   Show or hide specified dock bar.
  //   DO NOT call RhinoUiMainFrame().ShowDockBar(), use this method instead.
  //
  // Parameters:
  //   index [in] Index of dock bar to show or hide.
  //   show_control_bar [in] If true show dock bar otherwise hide it
  //   delay_recalc_layout [in] See MSDN help for CFrameWnd::ShowControlBar
  //
  // Returns:
  //   True if successful, false otherwise.
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   CFrameWnd::ShowControlBar()
  //
  bool ShowDockBar( int index, bool show_control_bar, bool delay_recalc_layout) const;
  //
  // Description:
  //   Show or hide specified dock bar.
  //   DO NOT call RhinoUiMainFrame().ShowDockBar(), use this method instead.
  //
  // Parameters:
  //   dock_bar [in] Dock bar to show or hide.
  //   show_dock_bar [in] If true show dock bar otherwise hide it
  //   delay_recalc_layout [in] See MSDN help for CFrameWnd::ShowControlBar
  //
  // Returns:
  //   True if successful, false otherwise.
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   CFrameWnd::ShowControlBar()
  //
  bool ShowDockBar( const CRhinoUiDockBar* dock_bar, bool show_dock_bar, bool delay_recalc_layout) const;
  //
  // Description:
  //   Call this method to cause a dock bar to not be docked to the frame window.
  //   DO NOT call RhinoUiMainFrame().FloatDockBar(), use this method instead.
  //
  // Parameters:
  //   uuid [in] ID for dock bar to float.
  //   point [in] The location, in screen coordinates, where the top left corner of the dock bar will be placed.
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   MSDN help for CFrameWnd::FloatControlBar()
  //   MSDN help for CFrameWnd::FloatControlBar
  //
	void FloatDockBar( UUID uuid, CPoint point);
  //
  // Description:
  //   Call this method to cause a dock bar to not be docked to the frame window.
  //   DO NOT call RhinoUiMainFrame().FloatDockBar(), use this method instead.
  //
  // Parameters:
  //   index [in] Index of dock bar to float.
  //   point [in] The location, in screen coordinates, where the top left corner of the dock bar will be placed.
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   MSDN help for CFrameWnd::FloatControlBar()
  //   MSDN help for CFrameWnd::FloatControlBar
  //
	void FloatDockBar( int index, CPoint point);
  //
  // Description:
  //   Call this method to cause a dock bar to not be docked to the frame window.
  //   DO NOT call RhinoUiMainFrame().FloatDockBar(), use this method instead.
  //
  // Parameters:
  //   dock_bar [in] Dock bar to show or hide.
  //   point [in] The location, in screen coordinates, where the top left corner of the dock bar will be placed.
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   MSDN help for CFrameWnd::FloatControlBar()
  //
	void FloatDockBar( const CRhinoUiDockBar* dock_bar, CPoint point);
  //
  // Description:
  //   Call this method to cause a dock bar to be docked to the main frame window.
  //   DO NOT call RhinoUiMainFrame().DockControlBar(), use this method instead.
  //
  // Parameters:
  //   uuid [in] ID for dock bar to dock.
  //   dock_location [in] Determines which sides of the frame window to consider for docking. It can be 0,
  //                      or one or more of the following: 
  //                        AFX_IDW_DOCKBAR_TOP   Dock to the top side of the frame window. 
  //                        AFX_IDW_DOCKBAR_BOTTOM   Dock to the bottom side of the frame window. 
  //                        AFX_IDW_DOCKBAR_LEFT   Dock to the left side of the frame window. 
  //                        AFX_IDW_DOCKBAR_RIGHT   Dock to the right side of the frame window. 
  //                      If 0, the dock bar can be docked to any side enabled for docking in the destination
  //                      frame window. 
  //   lpRect [in] Determines, in screen coordinates, where the dock bar will be docked in the nonclient area
  //               of the destination frame window. 
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   MSDN help for CFrameWnd::DockControlBar()
  //
  void DockDockBar( UUID uuid, UINT dock_location = 0, LPCRECT lpRect = NULL);
  //
  // Description:
  //   Call this method to cause a dock bar to be docked to the main frame window.
  //   DO NOT call RhinoUiMainFrame().DockControlBar(), use this method instead.
  //
  // Parameters:
  //   uuid [in] ID for dock bar to dock.
  //   dock_location [in] Determines which sides of the frame window to consider for docking. It can be 0,
  //                      or one or more of the following: 
  //                        AFX_IDW_DOCKBAR_TOP   Dock to the top side of the frame window. 
  //                        AFX_IDW_DOCKBAR_BOTTOM   Dock to the bottom side of the frame window. 
  //                        AFX_IDW_DOCKBAR_LEFT   Dock to the left side of the frame window. 
  //                        AFX_IDW_DOCKBAR_RIGHT   Dock to the right side of the frame window. 
  //                      If 0, the dock bar can be docked to any side enabled for docking in the destination
  //                      frame window. 
  //   lpRect [in] Determines, in screen coordinates, where the dock bar will be docked in the nonclient area
  //               of the destination frame window. 
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   MSDN help for CFrameWnd::DockControlBar()
  //
	void DockDockBar( int index, UINT dock_location = 0, LPCRECT lpRect = NULL);
  //
  // Description:
  //   Call this method to cause a dock bar to be docked to the main frame window.
  //   DO NOT call RhinoUiMainFrame().DockControlBar(), use this method instead.
  //
  // Parameters:
  //   dock_bar [in] Dock bar to show or hide.
  //   dock_location [in] Determines which sides of the frame window to consider for docking. It can be 0,
  //                      or one or more of the following: 
  //                        AFX_IDW_DOCKBAR_TOP   Dock to the top side of the frame window. 
  //                        AFX_IDW_DOCKBAR_BOTTOM   Dock to the bottom side of the frame window. 
  //                        AFX_IDW_DOCKBAR_LEFT   Dock to the left side of the frame window. 
  //                        AFX_IDW_DOCKBAR_RIGHT   Dock to the right side of the frame window. 
  //                      If 0, the dock bar can be docked to any side enabled for docking in the destination
  //                      frame window. 
  //   lpRect [in] Determines, in screen coordinates, where the dock bar will be docked in the nonclient area
  //               of the destination frame window. 
  //
  // See Also:
  //   CRhinoUiDockBarManager::FindDockBar()
  //   MSDN help for CFrameWnd::DockControlBar()
  //
	void DockDockBar( const CRhinoUiDockBar* dock_bar, UINT dock_location = 0, LPCRECT lpRect = NULL);
  //
  // Description:
  //   Call this method to determine if a dock bar is currently visible.
  //
  // Parameters:
  //   uuid [in] ID for dock bar to check visibility state.
  //
  // Returns:
  //   Returns true if dock bar is currently visible or false if it is not.
  //
  bool IsDockBarVisible( UUID uuid) const;
  //
  // Description:
  //   Call this method to determine if a dock bar is currently visible.
  //
  // Parameters:
  //   index [in] Index of dock bar to check visibility state.
  //
  // Returns:
  //   Returns true if dock bar is currently visible or false if it is not.
  //
  bool IsDockBarVisible( int index) const;
  //
  // Description:
  //   Call this method to determine if a dock bar is currently visible.
  //
  // Parameters:
  //   dock_bar [in] Dock bar to check visibility state.
  //
  // Returns:
  //   Returns true if dock bar is currently visible or false if it is not.
  //
  bool IsDockBarVisible( CRhinoUiDockBar* dock_bar) const;
  //
  // Description:
  //   Pointer to Rhino main frame window used by the dock bar manager.
  //
  // Returns:
  //   Pointer to Rhino main frame window used by the dock bar manager.
  //
  CFrameWnd* RhinoMainFrame() const;
  //
  // Description:
  //   Create pointer to derived CRhinoUiDockBar class assuring that it belongs
  //   to the correct module state so it can safely interact with the Rhino main
  //   frame window.  Your derived class definition must include the DECLARE_SERIAL
  //   macro and the implementation file must include DECLARE_SERIAL.
  //
  // For example:
  //    MyDockBar.h file
  //    class CMyDockBar : public CRhinoUiDockBar
  //    {
  //	    DECLARE_SERIAL( CMyDockBar)
  //    public:
  //      CMyDockBar(void);
  //      ~CMyDockBar(void);
  //      UUID DockBarID() const;
  //      const wchar_t* DockBarName( int language_id);
  //      bool CreateDockBarControls();
  //    };
  //
  //    MyDockBar.cpp file
  //    IMPLEMENT_DYNCREATE( CMyDockBar, CRhinoUiDockBar);
  //
  //    CMyDockBar::CMyDockBar(void)
  //    {
  //    }
  //
  //    CMyDockBar::~CMyDockBar(void)
  //    {
  //    }
  //
  //    UUID CMyDockBar::DockBarID() const
  //    {
  //      //TODO: This UUID must be unique.  If you copy this code you MUST change the UUID
  //      UUID uuid = { 0xf719d329, 0xbb9e, 0x4afd, { 0x84, 0x11, 0x7c, 0x95, 0x1, 0xfb, 0x2a, 0x7f } };
  //      return uuid;
  //    }
  //
  //    const wchar_t* CMyDockBar::DockBarName( int language_id)
  //    {
  //      return L"My Custom Dock Bar";
  //    }
  //
  //    bool CMyDockBar::CreateDockBarControls()
  //    {
  //      // create child windows here
  //      return true;
  //    }
  //
  // Parameters:
  //   pRC [in] Pointer to a CRuntimeClass object which describes the class to be
  //            created.
  //
  // Returns:
  //   Pointer to dock bar if successfully allocated.
  //
  // Note:
  //   Do NOT call delete on pointer returned by this method.  The dock bar manager will
  //   take care of that when appropriate.
  //
  // See Also:
  //   CRhinoUiDockBar::CreateRhinoDockBar()
  //   CRhinoUiDockBar::CreateDockBarControls();
  //   CRuntimeClass
  //   DECLARE_SERIAL
  //   IMPLEMENT_DYNCREATE
  //
  CRhinoUiDockBar* CreateDockBarObject( CRuntimeClass* pRC, UUID plug_in_id) const;
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   Call to create and display dock bar in default location.
  //
  // Parameters:
  //   show_control_bar [in] Display dock bar if created successfully.
  //   default_dock_location [in] Default display location of dock bar.
  //     Valid values are:
  //        AFX_IDW_DOCKBAR_FLOAT - float the dock bar
  //        AFX_IDW_DOCKBAR_TOP - dock the dock bar at the top of the Rhino main frame
  //        AFX_IDW_DOCKBAR_BOTTOM - dock the dock bar at the bottom of the Rhino main frame
  //        AFX_IDW_DOCKBAR_LEFT - dock the dock bar at the left side of the Rhino main frame
  //        AFX_IDW_DOCKBAR_RIGHT - dock the dock bar at the right side of the Rhino main frame
  //   dock_style [in] - Areas this dock bar is allowed to be docked.
  //   float_point [in] - Only used if default_dock_location==AFX_IDW_DOCKBAR_FLOAT.  If floating
  //     dock bar then this is the top left cornner of the dock bar.  If this is NULL then
  //     dock bar is cascaded.
  //   styles [in]
  //     Dock bar styles used when creating this dock bar.
  //
  // See Also:
  //   CControlBar::EnableDocking
  //   CControlBar::FloatControlBar
  //   
  CRhinoUiDockBar* CreateRhinoDockBar( CRuntimeClass* pRC
                                       , UUID plug_in_id
                                       , bool show_control_bar = true
                                       , DWORD default_dock_location = AFX_IDW_DOCKBAR_FLOAT
                                       , DWORD dock_style = CBRS_ALIGN_ANY
                                       , LPPOINT float_point = NULL
                                       , DWORD styles = CBRS_TOOLTIPS|CBRS_SIZE_DYNAMIC|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_LEFT
                                     );
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   Call to create and display dock bar in default location.
  //
  // Parameters:
  //   show_control_bar [in] Display dock bar if created successfully.
  //   default_dock_location [in] Default display location of dock bar.
  //     Valid values are:
  //        AFX_IDW_DOCKBAR_FLOAT - float the dock bar
  //        AFX_IDW_DOCKBAR_TOP - dock the dock bar at the top of the Rhino main frame
  //        AFX_IDW_DOCKBAR_BOTTOM - dock the dock bar at the bottom of the Rhino main frame
  //        AFX_IDW_DOCKBAR_LEFT - dock the dock bar at the left side of the Rhino main frame
  //        AFX_IDW_DOCKBAR_RIGHT - dock the dock bar at the right side of the Rhino main frame
  //   dock_style [in] - Areas this dock bar is allowed to be docked.
  //   float_point [in] - Only used if default_dock_location==AFX_IDW_DOCKBAR_FLOAT.  If floating
  //     dock bar then this is the top left cornner of the dock bar.  If this is NULL then
  //     dock bar is cascaded.
  //   styles [in]
  //     Dock bar styles used when creating this dock bar.
  //
  // See Also:
  //   CControlBar::EnableDocking
  //   CControlBar::FloatControlBar
  //   
  bool CreateRhinoDockBar( CRhinoUiDockBar* pCB
                           , UUID plug_in_id
                           , bool show_control_bar = true
                           , DWORD default_dock_location = AFX_IDW_DOCKBAR_FLOAT
                           , DWORD dock_style = CBRS_ALIGN_ANY
                           , LPPOINT float_point = NULL
                           , DWORD styles = CBRS_TOOLTIPS|CBRS_SIZE_DYNAMIC|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_LEFT
                         );
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   This should only be called one time from CMyWinApp::InitInstance() right
  //   after loading the main frame.  It will load the MRU tool bar collection
  //   list and restore dock and tool bar position and state.
  //
  void LoadToolBarsAndRestoreDockBarPositionAndStates();
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   This should only be called one time from CMyWinApp::InitInstance() right
  //   after loading the main frame.  It will load the MRU tool bar collection
  //   list and restore dock and tool bar position and state.
  //
  bool RestoreMainFrameWindowPos();

  // Description:
  //   Reads saved information about how to attempt to show the main frame when
  //   it is first created by using saved position information based on the current
  //   scheme
  // Returns:
  //   false if the information could not successfully be retrieved
  bool InitialMainFrameWindowPos(WINDOWPLACEMENT& wp) const;
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   This should only be called one time from CMyWinApp::OnSaveAllModified.  It
  //   will save current dock and tool bar positions and states.
  //
  // Returns:
  //   Return true if it is safe to continue closing application; otherwise false.
  //   
  bool OnCWinAppSaveAllModified( bool bSaveCollections = true);
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   The CRhinoUiDockBarManager calls this method to determine if it should 
  //   hide floating, visible CRhinoUiDockBars and floating Rhino views when
  //   the application is deactivated.  This should only be called by
  //   CWinApp::SaveAllModified() prior to hiding the application and saving values
  //   when shutting down.
  //
  // Parameters:
  //   b [in] If true then dock bars and views will not be hidden otherwise they will.
  //
  void DisableHideOnDeactivate( bool b);
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   The CRhinoUiDockBarManager calls this method to determine if it should 
  //   hide floating, visible CRhinoUiDockBars and floating Rhino views when
  //   the application is deactivated.
  //
  // Returns:
  //   Return true if it bars and views should be temporarily hidden.
  //   
  bool HideOnDeactivateEnabled() const;
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   This should only be called one time from CMyWinApp::ExitInstance.  It
  //   will clean up dock bar memory.
  //
  void OnExitInstance();
  //
  // Description:
  //   Get the next available dock bar ID.  Is used by CRhinoUiDockBar::CreateRhinoDockBar
  //   to get next available dock bar ID.
  //
  // Returns:
  //   ID to be used when creating a dock bar.
  //
  // Remarks:
  //   Call this cautiously, it inceremtns the ID counter each time it is called.
  //
  unsigned int GetNextDockBarID();
  //
  // Description:
  //   Called one time during initialization when it is time to create default
  //   dock and tool bars.
  //
  // Returns:
  //   Returns true if dock/tool bars were created successfully otherwise; false
  //
  bool CReateDefaultDockBars();
  //
  // Description:
  //   Docking location locked means any dock bar that is currently docked can not
  ///  be moved and any floating dock bar may be moved around but not docked.  This
  //   does not preclude a dock bars visibility state from changing.
  //
  // Returns:
  //   Returns true if dock bars current docked/floating state is locked otherwise false
  //
  // See Also:
  //   CRhinoUiDockBar::LockDockingLocation
  //
  bool DockingLocationLocked() const;
  //
  // Description:
  //   Docking location locked means any dock bar that is currently docked can not
  ///  be moved and any floating dock bar may be moved around but not docked.  This
  //   does not preclude a dock bars visibility state from changing.
  //
  // Parameters:
  //   b [in] New value for locked state.
  //
  // Returns:
  //   Returns previous locked state.
  //
  // See Also:
  //   CRhinoUiDockBar::DockingLocationLocked
  //
  bool LockDockingLocation( bool b);
  //
  // Description:
  //   FOR EXPERT USERS ONLY!
  //   Call this method if you want to unhook, destroy and delete a control bar at runtime.
  //
  // Parameters:
  //   dock_bar [in] Dock bar to unhook, destroy and delete
  //
  // Returns:
  //   Returns true if successful otherwise returns false.
  //
  bool UnhookDeleteAndDestroyDockBar( CRhinoUiDockBar* dock_bar);
  //
  // Description:
  //   FOR EXPERT USERS ONLY!
  //   Call this method if you want to unhook, destroy and delete a control bar at runtime.
  //
  // Parameters:
  //   dock_bar_id [in] ID of dock bar to unhook, destroy and delete
  //
  // Returns:
  //   Returns true if successful otherwise returns false.
  //
  bool UnhookDeleteAndDestroyDockBar( UUID dock_bar_id);
  //
  // Description:
  //   FOR EXPERT USERS ONLY!
  //   Call this method if you want to unhook, destroy and delete a control bar at runtime.
  //
  // Parameters:
  //   dock_bar_index [in] Index of dock bar to unhook, destroy and delete
  //
  // Returns:
  //   Returns true if successful otherwise returns false.
  //
  bool UnhookDeleteAndDestroyDockBar( int dock_bar_index);
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   FOR INTERNAL USE ONLY!!!
  //   Call this method when the applications main frame window gets WM_SIZE
  //   notification.
  //
  void MainFrameOnSize( UINT nType);
  /////////////////////////////////////////////////////////////////////////////
  //
  // Description:
  //   FOR INTERNAL USE ONLY!!!
  //   Call this method when the applications main frame window gets WM_SIZE
  //   notification.
  //
  void CallOnActivateApp( BOOL32 bActive, DWORD dwThreadID);
private:
  ///////////////////////////////////////////////////////////////////////////////////
  //
  // FOR INTERNAL USE ONLY
  //
  friend class CRhinoUiManager;
  friend class CRhinoUiDockBar;
  friend class CRhinoUiToolBarManager;
  friend class CRhinoUiToolBarCollection;
#if defined(ON_RUNTIME_WIN)
  friend CRhinoUiDockBar::CRhinoUiDockBar();
#endif
  friend void CRhinoUiDockBar::PostNcDestroy();
  friend void CRhinoUiDockBar::OnDestroy();
  friend bool CRhinoUiDockBar::CreateRhinoDockBar( UUID, bool, DWORD, DWORD, LPPOINT, DWORD);
  friend bool RhMainFrameOnCreate( CFrameWnd*);
  friend class CRhUiDockBarManagerWnd;
  //friend bool CRhinoUiToolBar::DestroyCascadeToolBar();
  friend class CRhinoUiToolBar;

  virtual bool CreateStatusBar( CFrameWnd* pMainFrame);
  virtual bool OnCReateDefaultDockBars();
  virtual void OnCleanUp();
  virtual void OnSetDockStateCompleted();

  void OnDestroyMainFrame();
  void CleanUp();

  bool SetMainFrame( CFrameWnd* pFrame);
  //
  // Description:
  //   Add dock bar to list when creating.
  //
  // Parameters:
  //   pCB [in] dock bar to add.
  //
  // Returns:
  //   0 Based index to dock bar if successfully added.
  //   -1 If pCB is NULL.
  //   -2 If pCB is already in the list.
  //
  int AddDockBar( CRhinoUiDockBar* pCB);
  //
  // Description:
  //   Remove dock bar pointer from manager.  Called by CRhinoUiDockBar::OnDestroy().
  //
  // Parameters:
  //   pCB [in] dock bar to remove.
  //
  void RemoveDockBar( CRhinoUiDockBar* pCB);
  bool CreatePrivateWindow();

  //
  // Description:
  //   For internal use
  // Returns:
  //   Returns true if this manager is in the process of restoring dock bar
  //   positions and states otherwise; returns false.
  bool IsRestoringDockBarPositions() const;

private:
  // Not implemented
  CRhinoUiDockBarManager( const CRhinoUiDockBarManager&);
  const CRhinoUiDockBarManager& operator = (const CRhinoUiDockBarManager&);

private:
  // Array of dock bar pointers to delete when cleaning up
  ON_SimpleArray<CRhinoUiDockBar*>m_auto_delete_list;
  // Next unassigned dock bar ID
  unsigned int m_next_control_bar_id;
  // Frame which owns Rhino control bars
  CFrameWnd* m_main_frame;
  // Dock/tool bar save file key name
  static class CRhUiDockBarManagerWnd* m_private_wnd;
  bool m_bCallCReateDefaultDockBars;
  bool m_bLockDockingLocation;
  bool m_bHideBarsOnDeactivate;
public:
  // Reserved for future use
  class CRhDockBarManagerExtension* m__control_bar_manager_sdk_extension;

  RhinoTabbedDockBar::CCollection* TabbedDockBarCollection(void) const;
  void DestroyDockBarCollection(void) const;
};


