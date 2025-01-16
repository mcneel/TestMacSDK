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

#pragma once

///////////////////////////////////////////////////////////////////
//
// class CRhinoUiPageDockBar
//
//-----------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoUiPageDockBar : public CRhinoUiDockBar
{
	DECLARE_SERIAL(CRhinoUiPageDockBar)

public:
  //---------------------------------------------------------------
  // Begin CRhinoUiDockBar overrides
  //
  UUID DockBarID() const;
  const wchar_t* DockBarName( int language_id = 1033 /*English*/) const;
  bool CreateDockBarControls();
  //
  // End CRhinoUiDockBar overrides
  //---------------------------------------------------------------

public:
  class CRhUiPageDockBarTabCtrl& GetTabCtrl() const;

protected:
  class CRhUiPageDockBarTabCtrl* m_tab_ctrl;
  UUID m_page_id;
  ON_wString m_page_name;

protected:
  friend class CRhinoUiPageDockBarManager;
  friend class CRhinoUiModlessDockingDialogManager;

	CRhinoUiPageDockBar();
  CRhinoUiPageDockBar( const CRhinoUiPageDockBar&);
  const CRhinoUiPageDockBar& operator=( const CRhinoUiPageDockBar&);
  virtual ~CRhinoUiPageDockBar();
};

///////////////////////////////////////////////////////////////////
//
// class CRhinoUiModelessDockingDialog
//
//-----------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoUiModelessDockingDialog : public CRhinoUiDockBarClientExpandableTabPage
{
  DECLARE_DYNCREATE( CRhinoUiModelessDockingDialog)

protected:
  CRhinoUiModelessDockingDialog( LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
  CRhinoUiModelessDockingDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL);
  virtual ~CRhinoUiModelessDockingDialog();
  void Construct();

public:
  //---------------------------------------------------------------
  // Begin CRhinoUiExpandableTabPage overrides
  //
  virtual void OnHideTab( int iTabIndex, bool bHide);
  virtual void OnExpandTab( int iTabIndex, bool bExpand);
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL32 OnInitDialog();
  //
  // End CRhinoUiExpandableTabPage overrides
  //---------------------------------------------------------------
  // Begin CRhinoUiDialog overrides
  //
  virtual void OnUnhandledEscapeAndEnter( UINT nChar );
  virtual bool ForwardLMouseDownMessagesToParent( CPoint point);
  virtual bool ForwardLMouseDblClickMessagesToParent( CPoint point);
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  //
  // End CRhinoUiDialog overrides
  //---------------------------------------------------------------
protected:
  friend class CRhinoUiModlessDockingDialogManager;
  UUID m_uuid;
  ON_wString m_wName;
};
///////////////////////////////////////////////////////////////////
//
// class CRhinoUiModelessDockingDialog
//
//-----------------------------------------------------------------
class CRhinoUiModlessDockingDialogRecord
{
public:
  CRhinoUiModlessDockingDialogRecord() = default;
  virtual ~CRhinoUiModlessDockingDialogRecord() = default;

public:
  const wchar_t* Name() const;
  CDialog* Dialog() const;
  CRhinoUiModelessDockingDialog* DockingDialog() const;
  CRhinoUiPageDockBar* PageDockBar() const;

protected:
  friend class CRhinoUiModlessDockingDialogManager;

protected:
  CRhinoUiPageDockBar* m_pPDB = nullptr;
  CRhinoUiModelessDockingDialog* m_dlg_page = nullptr;
};
///////////////////////////////////////////////////////////////////
//
// class CRhinoUiModelessDockingDialog
//
//-----------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoUiModlessDockingDialogManager
{
public:
  CRhinoUiModlessDockingDialogManager();
  virtual ~CRhinoUiModlessDockingDialogManager();

public:
  //   
  // Description:
  //   Add new page to bottom of tab stack.
  //
  // Parameters:
  //   lpsCaption [in] Tab caption
  //   nImage [in] Index of image to display to left of caption
  //   lpszTemplateName [in] The null-terminated string that is the name of a dialog-box
  //                         template resource. 
  //   szHtmlResID [in] The null-terminated string that is the name of an HTML resource. 
  //   nIDTemplate [in] Contains the ID number of a dialog-box template resource. 
  //   nHtmlResID [in] Contains the ID number of an HTML resource. 
  //   pRC [in] Class to create and add to control.  Use RUNTIME_CLASS( CMyClass) macro to format this parameter.
  //            Class decloration must include 	DECLARE_DYNCREATE(CMyClass) macro.  Class implementation must
  //            include IMPLEMENT_DYNCREATE(CMyClass, CRhinoUiDialog) macro.
  //   pMS [in] This must be your DLL application module state.
  //
  // Returns:
  //   Returns record index if successful; otherwise -1
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::InsertItem()
  //   AfxGetStaticModuleState()
  //   
  int AddDockingDialog( UUID uuidDockBar, const wchar_t* lpsCaption, UUID uuid, int nImage, LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  int AddDockingDialog( UUID uuidDockBar, const wchar_t* lpsCaption, UUID uuid, int nImage, UINT nIDTemplate, UINT nHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);
  //   
  // Description:
  //   Number of tabs currently added to this control.  Includes hidden tabs.
  //
  // Returns:
  //   Returns number of items in tab control including hidden items.
  //
  int GetRecordCount() const;
  //   
  // Description:
  //   Get the tab control item associated with a particular index.
  //
  // Parameters:
  //   nItem [in] Index of item to get.
  //
  // Returns:
  //   Return valid pointer if nItem is valid or NULL if not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetItemCount()
  //   
  const CRhinoUiModlessDockingDialogRecord* GetRecord( int nItem) const;
  //   
  // Description:
  //   Get the tab control item associated with a particular UUID.
  //
  // Parameters:
  //   uuid [in] UUID of of item to get.
  //
  // Returns:
  //   Return valid pointer if a record with the specified UUID is found or NULL if not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetItemCount()
  //   
  const CRhinoUiModlessDockingDialogRecord* GetRecord( UUID uuid) const;
  //   
  // Description:
  //   Get the tab control item associated with a particular page.
  //
  // Parameters:
  //   pPage [in] Page to search for.
  //
  // Returns:
  //   Return valid pointer if the dialog page is found or NULL if not.
  //
  // See Also:
  //   CRhinoUiExpandableTabCtrl::GetItemCount()
  //   
  const CRhinoUiModlessDockingDialogRecord* GetRecord( const CDialog* pPage) const;
  CRhinoUiPageDockBar* GetPageDockBarFromRecordIndex( int iRecIndex) const;

protected:
  int AddDockingDialog( UUID uuidDockBar, const wchar_t* lpsCaption, UUID uuid, int nImage, UINT nIDTemplate, LPCTSTR lpszTemplateName, UINT nHtmlResID, LPCTSTR szHtmlResID, CRuntimeClass* pRC, AFX_MODULE_STATE* pMS);

protected:
  ON_ClassArray<CRhinoUiModlessDockingDialogRecord>m_item_list;

protected:
  // Not implemented
  CRhinoUiModlessDockingDialogManager( const CRhinoUiModlessDockingDialogManager&);
  CRhinoUiModlessDockingDialogManager& operator= ( const CRhinoUiModlessDockingDialogManager&);
};
///////////////////////////////////////////////////////////////////
//
// class CRhinoUiPageDockBarManager
//
//-----------------------------------------------------------------
class RHINO_SDK_CLASS CRhinoUiPageDockBarManager : private ON_SimpleArray<CRhinoUiPageDockBar*>
{
public:
  CRhinoUiPageDockBarManager();
  ~CRhinoUiPageDockBarManager();

  //
  // Description:
  //   Get number of page dock bars associated with Rhino.
  //
  // Returns:
  //   Number of page dock bars associated with Rhino.
  //
  // See Also:
  //   CRhinoUiPageDockBar* CRhinoUiPageDockBarManager::DockBar( int index)
  //   CRhinoUiPageDockBar* CRhinoUiPageDockBarManager::operator[]( int index)
  int DockBarCount() const;
  //
  // Description:
  //   The page dock bar manager is a collection of CRhinoUiPageDockBars.  This
  //   function may be used to iterate the page dock bar collection.
  //
  // Parameters:
  //   index [in] Zero based index for page dock bar to return.
  //
  // Returns:
  //   Pointer to dock bar if index is valid otherwise NULL.
  //
  // See Also:
  //   int CRhinoUiPageDockBarManager::DockBarCount() const
  //   CRhinoUiPageDockBar* CRhinoUiPageDockBarManager::operator[]( int index)
  CRhinoUiPageDockBar* DockBar( int index) const;
  //
  // Description:
  //   The page dock bar manager is a collection of CRhinoUiPageDockBars.  This
  //   function may be used to iterate the dock bar collection.
  //
  // Parameters:
  //   index [in] Zero based index for page dock bar to return.
  //
  // Returns:
  //   Pointer to dock bar if index is valid otherwise NULL.
  //
  // See Also:
  //   int CRhinoUiPageDockBarManager::DockBarCount() const
  //   CRhinoUiPageDockBar* CRhinoUiPageDockBarManager::DockBar( int index)
  CRhinoUiPageDockBar* operator[]( int index) const;
  //
  // Description:
  //   The dock bar manager is a collection of CRhinoUiPageDockBars.  Each
  //   dock bar has a unique UUID.  This function may be used to see if
  //   a dock bar with the specified UUID is currently in the collection.
  //
  // Parameters:
  //   uuid [in] UUID of dock bar to search for
  //
  // Returns:
  //   Pointer to dock bar if UUID is found otherwise NULL.
  CRhinoUiPageDockBar* FindDockBar( UUID uuid) const;
  //
  // Description:
  //   The dock bar manager is a collection of CRhinoUiPageDockBars.  Each
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
  CRhinoUiPageDockBar* FindDockBar( const wchar_t* control_bar_name, int language_id = 1033 /*English*/);
  // Description:
  //   Use this to set CRhinoUiPageDockBar::m_sort_index so that
  //   the values of m_sort_index run from 0 to DockBarCount()-1
  //   and compare(control_bar_list[i], control_bar_list[j]) < 0) implies 
  //   control_bar_list[i].m_sort_index < control_bar_list[j].m_sort_index.
  //
  // Parameters:
  //   compar - [in] compare function with prototype
  //       int compare(const CRhinoUiPageDockBar* arg1, const CRhinoUiPageDockBar* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       BOOL32 cull(const CRhinoUiPageDockBar* arg,void* p).
  //       that returns TRUE if the dock bar should be ignored
  //       when sorting.  Dock bars that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull the dock bar list.
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoUiPageDockBar pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the dock bar manager.  They are intended to be used to get
  //   sorted lists of dock bars for dialogs, etc.
  //
  // See Also:
  //   CRhinoUiPageDockBarManager::GetSortedList
  void Sort(  int (*compar)(CRhinoUiPageDockBar*const*,CRhinoUiPageDockBar*const*,void*),
              int (*cull)(const CRhinoUiPageDockBar*,void*),
              void* p=0
            );
  // Description:
  //   Gets an array of pointers to dock bars  that is sorted by
  //   the values of CRhinoUiPageDockBar::SortIndex().
  //
  // Parameters:
  //   sorted_list - [out] this array is returned sorted by the values of
  //            CRhinoUiPageDockBar::SortIndex().  This list may be a different length
  //            than the dock bar list if ignore_not_created is true and/or a cull
  //            function was used by Sort().
  //   ignore_not_created - [in] TRUE means don't include dock bars that have not
  //                             been created.
  //
  // Remarks:
  //   Use CRhinoUiPageDockBar::Sort() to set the values of m_sort_index.
  //
  // See Also:
  //   CRhinoUiPageDockBarManager::Sort();
  //   CRhinoUiPageDockBar::SortIndex();
  void GetSortedList( ON_SimpleArray<CRhinoUiPageDockBar*>& sorted_list,
                      bool ignore_not_created = false
                    ) const;
  //
  // Description:
  //   Get index for specified CRhinoUiPageDockBar object
  //
  // Parameters:
  //   pCB [in] Dock bar to search for
  //
  // Returns:
  //   -1 If dock bar not in list otherwise zero based index
  //   for dock bar.
  //
  int IndexFromPointer( CRhinoUiPageDockBar* pCB) const;

protected:
  friend CRhinoUiPageDockBar::CRhinoUiPageDockBar();
  friend CRhinoUiPageDockBar::~CRhinoUiPageDockBar();

protected:
  void PageConstructor( CRhinoUiPageDockBar*);
  void PageDestructor( CRhinoUiPageDockBar*);

protected:
  // Not implemented
  CRhinoUiPageDockBarManager( const CRhinoUiPageDockBarManager&);
  CRhinoUiPageDockBarManager& operator=( const CRhinoUiPageDockBarManager&);

protected:
  ON_SimpleArray<CRhinoUiPageDockBar*>m_page_list;
};