#pragma once

#if defined(ON_RUNTIME_WIN)

#if defined(RH_RISKY_AFX_MANAGE_STATE)
# define MFC_DIALOG_MODULE_STATE RH_RISKY_AFX_MANAGE_STATE(DialogModuleState())
#else
# define MFC_DIALOG_MODULE_STATE AFX_MANAGE_STATE(DialogModuleState());
#endif

RHINO_SDK_FUNCTION void RhDialogWindowRefCount(IRhinoWindow* window, bool increment);
///////////////////////////////////////////////////////////////////////////////
//
// template class TRhinoDialogWindow
//
// Description:
//  CDialog Implementation of IRhinoWindow, BASECLASS must be a class derived
//  from CDialog, if you compile Rhino with RHINO_SDK_MFC you should use 
//  CRhinoDialog as your base class.
//
//-----------------------------------------------------------------------------
template<class BASECLASS>
class TRhinoDialogWindow : public BASECLASS, public IRhinoWindow
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Constructor
  TRhinoDialogWindow(UINT dialogResourceId, bool deleteInRelease)
  : BASECLASS(dialogResourceId, nullptr)
  , m_dialog_resouce_id(dialogResourceId)
  , m_delete_in_release(deleteInRelease)
  {
    RhDialogWindowRefCount(this, true);
#if defined(RHINO_SDK_MFC)
    CRhinoUiDialog* rhino_dialog = dynamic_cast<CRhinoUiDialog*>(this);
    if (rhino_dialog != nullptr)
      rhino_dialog->SetAllowEscapeAndEnter(false);
#endif
  }
  virtual ~TRhinoDialogWindow()
  {
    RhDialogWindowRefCount(this, false);
  }
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to load a embedded icon resource from this module
  HICON LoadIconResource(UINT iconResourceId, const ON_2iSize& sizeInPixels)
  {
    if (iconResourceId == 0)
      return NULL;
    MFC_DIALOG_MODULE_STATE;
    return CRhinoDpi::LoadIcon(::AfxGetInstanceHandle(), iconResourceId, sizeInPixels.cx, sizeInPixels.cy);
  }
private:
  UINT m_dialog_resouce_id = 0;
  bool m_delete_in_release = false;
public:
  typedef BASECLASS __base_class;
  //---------------------------------------------------------------------------
  // Description:
  //  Gets the module state associated with this dialog object
  AFX_MODULE_STATE* DialogModuleState() const { return m_pModuleState; }
  //---------------------------------------------------------------------------
#pragma region IRhinoWindow overrides
  //---------------------------------------------------------------------------
  bool Create(const CRhinoWindowCreateOptions& options) override
  {
    MFC_DIALOG_MODULE_STATE;
    HWND parent = options.ParentWindowHandle();
    if (options.ParentWindow() != nullptr)
      parent = options.ParentWindow()->Handle();
    return BASECLASS::Create(m_dialog_resouce_id, CWnd::FromHandle(parent)) != false;
  }
  //---------------------------------------------------------------------------
  const wchar_t* EnglishCaption() const override { return L""; }
  //---------------------------------------------------------------------------
  const wchar_t* LocalCaption() const { return L""; }
  //---------------------------------------------------------------------------
  void Release() override
  {
    MFC_DIALOG_MODULE_STATE;
    Destroy();
    if (m_delete_in_release)
      delete this;
  }
  //---------------------------------------------------------------------------
  void SetParent(HWND parent) override
  {
    if (Created())
      ::SetParent(BASECLASS::GetSafeHwnd(), parent);
  }
  //---------------------------------------------------------------------------
  HWND Parent() const override
  {
    return ::GetParent(BASECLASS::GetSafeHwnd());
  }
  //---------------------------------------------------------------------------
  bool Created() const override
  {
    return ::IsWindow(BASECLASS::GetSafeHwnd());
  }
  //---------------------------------------------------------------------------
  ON_2iPoint Location() const override
  {
    if (!Created())
      return ON_2iPoint(0, 0);
    CRect r;
    BASECLASS::GetWindowRect(r);
    auto parent = ::GetParent(GetSafeHwnd());
    if (parent == NULL)
      return ON_2iPoint(r.left, r.top);
    POINT pt = r.TopLeft();
    ::ScreenToClient(parent, &pt);
    return ON_2iPoint(pt.x, pt.y);
  }
  //---------------------------------------------------------------------------
  void SetLocation(ON_2iPoint location) override
  {
    if (!Created())
      return;
    UINT flags = SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER;
    BASECLASS::SetWindowPos(nullptr, location.x, location.y, 0, 0, flags);
  }
  //---------------------------------------------------------------------------
  ON_2iSize Size() const override
  {
    if (!Created())
      return ON_2iSize(0, 0);
    CRect r;
    BASECLASS::GetWindowRect(r);
    return ON_2iSize(r.Width(), r.Height());
  }
  //---------------------------------------------------------------------------
  void SetSize(ON_2iSize size, bool redraw, bool redrawBorder) override
  {
    if (!Created())
      return;
    UINT flags = SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER;
    if (redrawBorder)
      flags |= SWP_DRAWFRAME;
    if (!redraw)
      flags |= SWP_NOREDRAW;
    BASECLASS::SetWindowPos(nullptr, 0, 0, size.cx, size.cy, flags);
  }
  //---------------------------------------------------------------------------
  void GetScreenCoords(ON_4iRect& rectOut) const override
  {
    if (!Created())
      rectOut.SetRect(0, 0, 0, 0);
    else
    {
      CRect r;
      BASECLASS::GetWindowRect(r);
      rectOut.SetRect(r.left, r.top, r.right, r.bottom);
    }
  }
  //---------------------------------------------------------------------------
  void SetIsVisible(bool visible) override
  {
    if (Created())
      BASECLASS::ShowWindow(visible ? SW_SHOWNA : SW_HIDE);
  }
  //---------------------------------------------------------------------------
  bool IsVisible() const override
  {
    return Created() && BASECLASS::IsWindowVisible() != false;
  }
  //---------------------------------------------------------------------------
  void SetIsEnabled(bool enable) override
  {
    if (Created())
      BASECLASS::EnableWindow(enable);
  }
  //---------------------------------------------------------------------------
  bool IsEnabled() const override
  {
    return Created() && BASECLASS::IsWindowEnabled();
  }
  //---------------------------------------------------------------------------
  void SetFocused() override
  {
    if (Created())
      BASECLASS::SetFocus();
  }
  //---------------------------------------------------------------------------
  void Refresh(bool immediate) override
  {
    if (!Created())
      return;
    BASECLASS::Invalidate();
    if (immediate)
      BASECLASS::UpdateWindow();
  }
  //---------------------------------------------------------------------------
  HWND Handle(void) override
  {
    return BASECLASS::GetSafeHwnd();
  }
  //---------------------------------------------------------------------------
  bool Destroy() override
  {
    MFC_DIALOG_MODULE_STATE;
    return ::IsWindow(GetSafeHwnd()) && DestroyWindow();
  }
  //---------------------------------------------------------------------------
#pragma endregion IRhinoWindow overrides
};
///////////////////////////////////////////////////////////////////////////////
//
// template class TRhinoPropertiesPanelPage
//
// Description:
//  CDialog Implementation of IRhinoOptionsPage, BASECLASS must be a class 
//  derived from CDialog, if you compile Rhino with RHINO_SDK_MFC you should 
//  use CRhinoDialog as your base class.  This is only a partial implementation
//  of IRhinoPropertiesPanelPage, there will be additional
//  methods that need to be implemented in your class.  See 
//  RHINO_OPTIONS_PAGE_DECLARE for minimum required methods.
//
//-----------------------------------------------------------------------------
template<class BASECLASS>
class TRhinoOptionsPage : public TRhinoDialogWindow<BASECLASS>, public  IRhinoOptionsPage
{
public:
  TRhinoOptionsPage(UINT dialogResourceId, UINT iconResourceId, bool deleteInRelease)
  : TRhinoDialogWindow<BASECLASS>(dialogResourceId, deleteInRelease)
  , m_icon_resource_id(iconResourceId)
  {
  }
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the host attached to this page.
  IRhinoOptionsPageHost* OptionsPageHost() const { return m_options_page_host; }
  //---------------------------------------------------------------------------
#pragma region IRhinoPage overrides
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the icon used by the host to identify this page.
  // Parameters:
  //  size[in] = Desired icon size.
  // Returns:
  //  The icon used by the host to identify this page. 
  HICON Image(ON_2iSize sizeInPixels) override
  {
    return LoadIconResource(m_icon_resource_id, sizeInPixels);
  }
  //---------------------------------------------------------------------------
  // Description:
  //  Window to display in this page.
  IRhinoWindow* Window() override { return dynamic_cast<TRhinoDialogWindow<BASECLASS>*>(this); }
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the object is no longer referenced by the framework, decrement
  //  your reference counter and delete this object when the counter is 0.
  void Release() override
  {
    TRhinoDialogWindow<BASECLASS>::Release();
  }
  //---------------------------------------------------------------------------
#pragma endregion IRhinoPage overrides
  //---------------------------------------------------------------------------
#pragma region IRhinoOptionsPage overrides
  //---------------------------------------------------------------------------
  // Description:
  //  This method gets called after the plug-in returns the pages to add to
  //  Rhino and the associated host objects are created.  Save this pointer
  //  and use it to communicate with the host or just ignore the call if you
  //  don't care.
  void HostAttached(IRhinoOptionsPageHost* host) override
  {
    m_options_page_host = host;
  }
  //---------------------------------------------------------------------------
#pragma endregion IRhinoOptionsPage overrides
  //---------------------------------------------------------------------------
protected:
  UINT m_icon_resource_id = 0;
  IRhinoOptionsPageHost* m_options_page_host = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
// Description:
//  CRhinoOptionsDialogPage has been replaced with IRhinoOptionsPage, this is
//  here to aid in porting from CRhinoOptionsDialogPage to IRhinoOptionsPage.
//  TRhinoOptionsPage implements the IRhinoWindow and IRhinoOptionsPage
//  interfaces and can be used as an example.  You might want to use the
//  RHINO_OPTIONS_PAGE_DECLARE macro in your class definition, it includes the
//  minimum required IRhinoOptionsPage overrides
//-----------------------------------------------------------------------------
typedef TRhinoOptionsPage<CRhinoDialog> CRhinoOptionsDialogPage;
///////////////////////////////////////////////////////////////////////////////
//
// template class TRhinoPropertiesPanelPage
//
// Description:
//  CDialog Implementation of IRhinoPropertiesPanelPage, BASECLASS must be a
//  class  derived from CDialog, if you compile Rhino with RHINO_SDK_MFC you
//  should  use CRhinoDialog as your base class.  This is only a partial
//  implementation of IRhinoPropertiesPanelPage, there will be additional
//  methods that need to be implemented in your class.  See 
//  RHINO_PROPERTIES_PANEL_PAGE_DECLARE for minimum required methods.
//
//-----------------------------------------------------------------------------
template<class BASECLASS>
class TRhinoPropertiesPanelPage : public TRhinoDialogWindow<BASECLASS>, public  IRhinoPropertiesPanelPage
{
protected:
  TRhinoPropertiesPanelPage(UINT dialogResourceId, UINT iconResourceId, bool deleteInRelease)
  : TRhinoDialogWindow<BASECLASS>(dialogResourceId, deleteInRelease)
  , m_icon_resource_id(iconResourceId)
  {
  }
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Get the IRhinoPropertiesPanelPageHost attached to this page.
  IRhinoPropertiesPanelPageHost* PropertiesPanelPageHost() const { return m_properties_panel_page_host; }
  //---------------------------------------------------------------------------
#pragma region IRhinoPage overrides
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the icon used by the host to identify this page.
  // Parameters:
  //  size[in] = Desired icon size.
  // Returns:
  //  The icon used by the host to identify this page. 
  HICON Image(ON_2iSize sizeInPixels) override
  {
    return LoadIconResource(m_icon_resource_id, sizeInPixels);
  }
  //---------------------------------------------------------------------------
  // Description:
  //  Window to display in this page.
  IRhinoWindow* Window() override { return dynamic_cast<TRhinoDialogWindow<BASECLASS>*>(this); }
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the object is no longer referenced by the framework, decrement
  //  your reference counter and delete this object when the counter is 0.
  void Release() override
  {
    TRhinoDialogWindow<BASECLASS>::Release();
  }
  //---------------------------------------------------------------------------
#pragma endregion IRhinoPage overrides
  //---------------------------------------------------------------------------
#pragma region IRhinoPropertiesPanelPage overrides
  //---------------------------------------------------------------------------
  // Description:
  //  This method gets called after the plug-in returns the pages to add to
  //  Rhino and the associated host objects are created.  Save this pointer
  //  and use it to communicate with the host or just ignore the call if you
  //  don't care.
  void HostAttached(IRhinoPropertiesPanelPageHost* host) override
  {
    m_properties_panel_page_host = host;
  }
  //---------------------------------------------------------------------------
#pragma endregion IRhinoPropertiesPanelPage overrides
  //---------------------------------------------------------------------------
protected:
  UINT m_icon_resource_id = 0;
  IRhinoPropertiesPanelPageHost* m_properties_panel_page_host = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
// Description:
//  CRhinoPropertiesDialogPage has been replaced with
//  IRhinoPropertiesPanelPage, this is here to aid in porting from
//  CRhinoPropertiesDialogPage to IRhinoPropertiesPanelPage.
//  TRhinoPropertiesPanelPage implements the IRhinoWindow and
//  IRhinoPropertiesPanelPage interfaces and can be used as an example.  You
//  might want to use the RHINO_PROPERTIES_PANEL_PAGE_DECLARE macro in your
//  class definition, it includes the minimum required IRhinoPropertiesPanelPage
//  overrides
//-----------------------------------------------------------------------------
typedef TRhinoPropertiesPanelPage<CRhinoDialog> CRhinoPropertiesDialogPage;
///////////////////////////////////////////////////////////////////////////////
// Description:
//  CRhinoObjectPropertiesDialogPage has been replaced with
//  IRhinoPropertiesPanelPage, this is here to aid in porting from
//  CRhinoObjectPropertiesDialogPage to IRhinoPropertiesPanelPage.
//  TRhinoPropertiesPanelPage implements the IRhinoWindow and
//  IRhinoPropertiesPanelPage interfaces and can be used as an example.  You
//  might want to use the RHINO_PROPERTIES_PANEL_PAGE_DECLARE macro in your
//  class definition, it includes the minimum required IRhinoPropertiesPanelPage
//  overrides
//-----------------------------------------------------------------------------
typedef TRhinoPropertiesPanelPage<CRhinoDialog> CRhinoObjectPropertiesDialogPage;
///////////////////////////////////////////////////////////////////////////////
// Description:
//  You can include this macro in your derived T...Page classes header file
//  to declare the required IRhinoPage method overrides.
//-----------------------------------------------------------------------------
#define RHINO_PAGE_DECLARE \
  const wchar_t* EnglishTitle() const override;\
  const wchar_t* LocalTitle() const override;
///////////////////////////////////////////////////////////////////////////////
// Description:
//  You can include this macro in your derived TRhinoOptionsPage classes 
//  header file to declare the required IRhinoOptionsPage method overrides.
//-----------------------------------------------------------------------------
#define RHINO_OPTIONS_PAGE_DECLARE \
  bool Apply(CRhinoOptionsPageEventArgs& e) override;\
  void UpdatePage(CRhinoOptionsPageEventArgs& e) override;\
  CRhinoCommand::result RunScript(CRhinoOptionsPageEventArgs& e) override;
///////////////////////////////////////////////////////////////////////////////
// Description:
//  You can include this macro in your derived TRhinoPropertiesPanelPage classes 
//  header file to declare the required IRhinoPropertiesPanelPage method 
//  overrides.
//-----------------------------------------------------------------------------
#define RHINO_PROPERTIES_PANEL_PAGE_DECLARE \
  bool IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs& e) const override;\
  CRhinoCommand::result RunScript(IRhinoPropertiesPanelPageEventArgs& e) override;\
  void UpdatePage(IRhinoPropertiesPanelPageEventArgs& e) override;

#endif
