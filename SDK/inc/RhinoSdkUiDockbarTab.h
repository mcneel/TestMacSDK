#pragma once

//RDK panels
RHINO_SDK_DATA extern UUID uuidPanelLightManager;
RHINO_SDK_DATA extern UUID uuidPanelLibraries;
RHINO_SDK_DATA extern UUID uuidPanelFileExplorer;
RHINO_SDK_DATA extern UUID uuidPanelGroundPlane;
RHINO_SDK_DATA extern UUID uuidPanelRenderSettings;
RHINO_SDK_DATA extern UUID uuidPanelSun;
RHINO_SDK_DATA extern UUID uuidPanelMaterials;
RHINO_SDK_DATA extern UUID uuidPanelEnvironments;
RHINO_SDK_DATA extern UUID uuidPanelTextures;
RHINO_SDK_DATA extern UUID uuidPanelLayers;
RHINO_SDK_DATA extern UUID uuidPanelObjectProps;
RHINO_SDK_DATA extern UUID uuidPanelDisplay;
RHINO_SDK_DATA extern UUID uuidPanelContextHelp;
RHINO_SDK_DATA extern UUID uuidPanelNotes;

//Deprecated
//RHINO_SDK_DATA extern UUID uuidPanelGroundPlaneEto;
//RHINO_SDK_DATA extern UUID uuidPanelRenderSettingsEto;
//RHINO_SDK_DATA extern UUID uuidPanelSunEto;

class RHINO_SDK_CLASS CRhinoUiDockBarTab
{
public:
  CRhinoUiDockBarTab();

  //NVM
  virtual void Delete(void);
  virtual bool Create(void);
  virtual void Move(const ON_4iRect& rect);
  virtual void Show(UINT uState);
  virtual void Destroy(void);
  virtual UUID Id(void) const;
  virtual bool Created(void) const;
  virtual void Switch(void);
  virtual void Help(void) const;
#if defined(ON_RUNTIME_APPLE)
  virtual RHINO_WINDOW_HANDLE WindowHandle() const;
#endif

protected:
  virtual void		DeleteThis(void) = 0;		//Implement as delete this;
  virtual bool		CreateWnd(void) = 0;		//Call the implementation specific PanelHost function to find the parent
  virtual void		MoveWnd(const ON_4iRect& rect) = 0;
  virtual void		ShowWnd(UINT uState) = 0;
  virtual void		DestroyWnd(void) = 0;
  virtual UUID		TabId(void) const = 0;
  virtual bool		IsCreated(void) const = 0;
  virtual void		SwitchDockBar(void) = 0;	//Call the implementation specific PanelHost function to find the parent
  virtual void		DoHelp(void) const = 0;

protected:
  virtual ~CRhinoUiDockBarTab();

public:
  virtual class   CRhinoUiDockBar* DockBar(void);

  virtual ON_UUID CurrentDockBarId(void) const;
  virtual void	SetCurrentDockBarId(const ON_UUID& uuid);

public:
  virtual class ITabbedDockBarEventWatcher* TabbedDockBarEventWatcher(void) const = 0;

public:
  virtual ON_wString	Caption(void) const = 0;
  virtual HICON Icon(const ON_2iSize& sizeInPixels) const = 0;

private:
  class Impl;
  Impl* m_pImpl;
  friend class Impl;
};

class RHINO_SDK_CLASS CRhinoUiPanel : public CRhinoUiDockBarTab
{
public:
  CRhinoUiPanel(unsigned int doc_sn);

public:
  virtual ON_UUID	PreviousDockBarId(void) const;
  virtual void	SetPreviousDockBarId(const ON_UUID& uuid);

  HWND PanelHost(void) const;

protected:
  //Implemented by searching for and calling the factory
  virtual ON_wString	Caption(void) const;
  virtual HICON		Icon(const ON_2iSize& sizeInPixels) const;

public:
  //Access functions
  virtual CRhinoDoc*		Document() const;							//Set by the constructor
  virtual unsigned int	DocumentRuntimeSerialNumber() const;		//Set by the constructor
  virtual bool			HasDocumentContext() const;						//Set by the factory

protected:
  virtual ~CRhinoUiPanel();

private:
  class Impl;
  Impl* m_pImpl;
  friend class Impl;
  friend class CRhinoUiPanelFactory;
};

class RHINO_SDK_CLASS CRhinoUiPanelFactory
{
protected:
  CRhinoUiPanelFactory(const ON_UUID& uuidPlugInId);
  virtual ~CRhinoUiPanelFactory();

public:
  CRhinoUiPanel*				NewDockbarTab(const CRhinoDoc&) const;
  CRhinoUiPanel*				NewNonDocDockbarTab(void) const;
  void Delete();

  virtual UUID				TabId(void) const = 0;
  virtual ON_wString			Caption(void) const = 0;
  virtual HICON				Icon(const ON_2iSize& sizeInPixels) const = 0;
#if defined(ON_RUNTIME_APPLE)
  virtual RHINO_WINDOW_IMAGE_HANDLE Image(const ON_2iSize& sizeInPixels) const;
#endif
  //Return true if you expect a new instance of the panel to be created when documents change.
  //This function returns false by default for C++ panels to retain compatibility with Rhino 5.
  virtual bool				HasDocumentContext(void) const; /* return false; */

protected:
  virtual CRhinoUiPanel*		NewTab(unsigned int document_sn) const = 0;
  virtual void				DeleteThis(void) = 0;

public:
  const ON_UUID&	PlugInId(void) const;
  bool			Enabled(void) const;
  void			SetEnabled(bool b);

private:
  class Impl;
  Impl* m_pImpl;
  friend class Impl;
};

// Description:
//   Creates a panel complete with a tab.  If no dockbar uuid is given, the framework will decide where to put it.
// Parameters:
//   pDoc [in]                    The document that this panel should display information for.
//   uuidTabId [in]               Panel to create.
//   makeSelectedTab[in]          Switch to this tab in the tab control that hosts this panel.
//   uuidDockBar [in, optional]   Specifies the host container id.
//                                If this is nullptr, the framework will determine the host itself, depending on the platform and previously saved state.
RHINO_SDK_FUNCTION CRhinoUiDockBar* RhinoUiOpenDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId, bool makeSelectedTab = true, const ON_UUID* uuidDockBar = nullptr);

// Description:
//   Creates a document context panel, or a document panel placeholder complete with a tab.  If no dockbar uuid is given, the framework will decide where to put it.
// Parameters:
//   uuidTabId [in]               Panel to create.
//   makeSelectedTab[in]          Switch to this tab in the tab control that hosts this panel.
//   uuidDockBar [in, optional]   Specifies the host container id.
//                                If this is nullptr, the framework will determine the host itself, depending on the platform and previously saved state.
RHINO_SDK_FUNCTION CRhinoUiDockBar* RhinoUiOpenDockbarTab(const ON_UUID& uuidTabId, bool makeSelectedTab = true, const ON_UUID* uuidDockBar = nullptr);

// Description:
//   Constructs a panel.  It is not attached to any dockbar.  This is generally used where the code inside the dockbar is required (Properties - I'm looking at you), but
//   it doesn't need to be shown.
// Parameters:
//   pDoc [in]                    The document that this panel should display information for.
//   uuidTabId [in]               Panel to create.
//                                If this is nullptr, the framework will determine the host itself, depending on the platform and previously saved state.
RHINO_SDK_FUNCTION void RhinoUiCreateTabQuietly(const CRhinoDoc& doc, const ON_UUID& uuidTabId);

// Description:
//   Hides the panel.  Does not destroy it.  If the panel is on a dockbar with a single tab, the dockbar will instead be hidden.
// Parameters:
//   pDoc [in]                    The document that this panel displays information for.
//   uuidTabId [in]               Panel to close.
RHINO_SDK_FUNCTION void RhinoUiCloseDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId, const ON_UUID* uuidDockBar = nullptr);

// Description:
//  This function is essentially a wrapper around RhinoUiOpenDockbarTab/RhinoUiCloseDockbarTab, depending on the bShow value.
// Parameters:
//   pDoc [in]                    The document that this panel displays information for.
//   uuidTabId [in]               Panel to close.
//   bShow [in]                   true for show, false for hide.
//   bMakeSelected [in]           true to select the tab that the panel is on, false otherwise. Ignored when bShow is false.
RHINO_SDK_FUNCTION void RhinoUiShowDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId, bool bShow, bool bMakeSelected, const ON_UUID* uuidDockBar = nullptr);

ON_DEPRECATED_MSG("Use the version with bMakeSelected")
RHINO_SDK_FUNCTION void RhinoUiShowDockbarTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId, bool bShow, const ON_UUID* uuidDockBar = nullptr);

// Description:
//  Determines if the panel is visible for a specific document.
// Parameters:
//   pDoc [in]                    The document that this panel displays information for.
//   uuidTabId [in]               Panel to check.
//   bIsSelectedTab [in]          Only return true if the panel is actually on the selected tab of a tabbed dockbar.
RHINO_SDK_FUNCTION bool             RhinoUiIsTabVisible(const CRhinoDoc& doc, const ON_UUID& uuidTabId, bool bIsSelectedTab = true);

RHINO_SDK_FUNCTION void             RhinoUiAddTabFactory(CRhinoUiPanelFactory* pFactory);

ON_DEPRECATED_MSG("RhinoUiDockBarForTab is obsolete. Use multi-doc capable RhinoUiDockBarsForTab (s after Bar).")
RHINO_SDK_FUNCTION CRhinoUiDockBar* RhinoUiDockBarForTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId);
ON_DEPRECATED_MSG("RhinoUiDockBarForTab is obsolete. Use the overloaded version that  returns an array of dock bar IDs.")
RHINO_SDK_FUNCTION void             RhinoUiDockBarsForTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId, ON_SimpleArray<CRhinoUiDockBar*>& dockbars);
RHINO_SDK_FUNCTION void             RhinoUiDockBarsForTab(const CRhinoDoc& doc, const ON_UUID& uuidTabId, ON_SimpleArray<ON_UUID>& dockbars);

//Use this function only when uuidTabId refers to a panel without document context.
RHINO_SDK_FUNCTION void             RhinoUiDockBarsForTab(const ON_UUID& uuidTabId, ON_SimpleArray<CRhinoUiDockBar*>& dockbars);

// Description:
//  Gets a list of host container Id's that include the specified panel.
// Parameters:
//   doc [in] Will only look for host containers associated with this docuument, on Mac there may be several per document.
//   panelId [in] Will check the container to see if it includes a panel with this Id.
//   hostIds [out] Will contain the Id of each appropriate container.
RHINO_SDK_FUNCTION void RhinoUiTabHostIds(const CRhinoDoc& doc, const ON_UUID& panelId, ON_SimpleArray<ON_UUID>& hostIds);

ON_DEPRECATED_MSG("Use RhinoUiPanelFactories")
RHINO_SDK_FUNCTION void             RhinoUiRegisteredDockbarTabs(ON_SimpleArray<ON_UUID>& aTabIds);

RHINO_SDK_FUNCTION void             RhinoUiPanelFactories(ON_SimpleArray<const CRhinoUiPanelFactory*>& factories);
RHINO_SDK_FUNCTION void             RhinoUiEnableTabFactory(const ON_UUID& uuidTabId, bool b);
RHINO_SDK_FUNCTION bool             RhinoUiIsTabFactoryEnabled(const ON_UUID& uuidTabId);
RHINO_SDK_FUNCTION COLORREF         RhGetDialogBackgroundColor(HWND ctrl);
RHINO_SDK_FUNCTION const CRhinoUiPanelFactory* RhinoUiPanelFactoryFromId(const ON_UUID& id);

#if defined (ON_RUNTIME_WIN)
RHINO_SDK_FUNCTION HWND             RhinoUiPanelHost(const ON_UUID& dockbarId);
#endif

#if defined (RHINO_SDK_MFC)
RHINO_SDK_FUNCTION bool RhinoUiIsDisabledTabbedDockBar(CControlBar*);
#endif

// Description:
//   Get a tab from a tab class Id.
// Parameters:
//   pDoc      [in] Document this panel displays.
//   uuidTabId [in] Tab to search for
// Returns:
//   Returns the tab specified by uuidTabId or nullptr if the tab has not been
//   created yet.
ON_DEPRECATED_MSG("RhinoUiDockBarTabFromId is obsolete. Use multi-doc capable RhinoUiDockBarTabsFromId (s after Tab).")
RHINO_SDK_FUNCTION CRhinoUiDockBarTab* RhinoUiDockBarTabFromId(const CRhinoDoc& doc, const ON_UUID& uuidTabId);
RHINO_SDK_FUNCTION void RhinoUiDockBarTabsFromId(const CRhinoDoc& doc, const ON_UUID& uuidTabId, ON_SimpleArray<CRhinoUiDockBarTab*>& dockbartabs);

// Description:
//   Update the icon for a specific tab.  If the tab is visible then
//   the CRhinoUiDockBarTab::Icon method will get called for each tab in the
//   container which includes the specified tab.  If the tab is not found or is
//   visible then nothing happens.
// Parameters:
//   doc       [in] Document to update the panel(s) for.
//   uuidTabId [in] Tab to update
RHINO_SDK_FUNCTION void RhinoUiDockBarTabUpdateIcon(const CRhinoDoc& doc, const ON_UUID& uuidTabId);

