#pragma once

#if defined(ON_RUNTIME_WIN)
#define RHINO_WINDOW_HANDLE HWND
#define RHINO_WINDOW_IMAGE_HANDLE HICON
#endif

#if defined(ON_RUNTIME_APPLE)
#if defined(RHINO_MAC_APPLICATION)
#define RHINO_WINDOW_HANDLE NSView*
#define RHINO_WINDOW_IMAGE_HANDLE NSImage*
#else
#define RHINO_WINDOW_HANDLE UIView*
#define RHINO_WINDOW_IMAGE_HANDLE UIImage*
#endif
#endif

#if defined(ON_RUNTIME_LINUX)
#define RHINO_WINDOW_HANDLE void*
#define RHINO_WINDOW_IMAGE_HANDLE void*
#endif

///////////////////////////////////////////////////////////////////////////////
/// </summary>
/// Create window options
/// </summary>
enum class RhinoWindowCreateOptions : int
{
  /// <summary>
  /// No options set.
  /// </summary>
  None = 0x00000000,
  /// <summary>
  /// The window is a child page being hosted by a core Rhino interface
  /// component.
  /// </summary>
  ChildPage = 0x00000001,
  /// <summary>
  /// Window should be made visible when creating
  /// component.
  /// </summary>
  Visible = 0x00000002,
  /// <summary>
  /// Window should be resizable (WS_THICKFRAME on Windows)
  /// component.
  /// </summary>
  Sizable = 0x4,
  /// <summary>
  /// Window should have a title bar and border (WS_OVERLAPPED on Windows)
  /// component.
  /// </summary>
  Frame = 0x8,
  /// <summary>
  /// Window should have a minimize button (WS_MINIMIZEBOX on Windows)
  /// component.
  /// </summary>
  Minimizable = 0x10,
  /// <summary>
  /// Window should have a maximize button (WS_MAXIMIZEBOX on Windows)
  /// component.
  /// </summary>
  Maximizable = 0x20,
};
///////////////////////////////////////////////////////////////////////////////
//
// class CRhinoWindowCreateOptions
//
// Description:
//  Generic window representation.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoWindowCreateOptions
{
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  CRhinoWindowCreateOptions();
  //---------------------------------------------------------------------------
  // Description:
  //  Default destructor.
  ~CRhinoWindowCreateOptions();
  //---------------------------------------------------------------------------
  // Description:
  //  Create options, defaults to RhinoWindowCreateOptions::ChildPage
  RhinoWindowCreateOptions Options() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Create options, defaults to RhinoWindowCreateOptions::ChildPage
  void Options(RhinoWindowCreateOptions options);
  //---------------------------------------------------------------------------
  // Description:
  //  Optional parent window
  class IRhinoWindow* ParentWindow() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Optional parent window
  void ParentWindow(class IRhinoWindow* parent) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Optional parent window
  RHINO_WINDOW_HANDLE ParentWindowHandle() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Optional parent window
  void ParentWindowHandle(RHINO_WINDOW_HANDLE windowHandle) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Optional english caption
  void EnglishCaption(const wchar_t* caption);
  ON_wString EnglishCaption(void) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Optional localized caption
  void LocalizedCaption(const wchar_t* caption);
  ON_wString LocalizedCaption(void) const;

private:
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the copy constructor.
  CRhinoWindowCreateOptions(const CRhinoWindowCreateOptions&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the operator=.
  const CRhinoWindowCreateOptions& operator=(const CRhinoWindowCreateOptions&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Internal, private data.
  class CRhPrivateWindowCreateOptions* m_private_create_options = nullptr;
};

RHINO_SDK_FUNCTION RhinoWindowCreateOptions operator&(RhinoWindowCreateOptions a, RhinoWindowCreateOptions b);
RHINO_SDK_FUNCTION RhinoWindowCreateOptions operator|(RhinoWindowCreateOptions a, RhinoWindowCreateOptions b);
RHINO_SDK_FUNCTION RhinoWindowCreateOptions& operator|=(RhinoWindowCreateOptions& a, RhinoWindowCreateOptions b);
RHINO_SDK_FUNCTION RhinoWindowCreateOptions& operator&=(RhinoWindowCreateOptions& a, RhinoWindowCreateOptions b);
RHINO_SDK_FUNCTION RhinoWindowCreateOptions operator~(RhinoWindowCreateOptions a);
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoWindow
//
// Description:
//  Generic window representation.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoWindow
{
public:
  virtual ~IRhinoWindow() = default;
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoWindow() = default;

  // Description:
  //  Delete the default copy constructor
  IRhinoWindow(const IRhinoWindow&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoWindow& operator=(const IRhinoWindow&) = delete;

public:
  //---------------------------------------------------------------------------
  // Description:
  //  Create window
  // Parameters:
  //  options - Create window options
  // Returns:
  //  Return true if the window was created else return false.
  virtual bool Create(const CRhinoWindowCreateOptions& options) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Destroy the window if it has been created otherwise don't do anything.
  virtual bool Destroy() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the English caption for this window.
  // Returns:
  //  The English name for this page.  
  virtual const wchar_t* EnglishCaption() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the localized caption for this window using the current Rhino
  //  language Id.
  // Returns:
  //  The localized name for this page using the current Rhino language Id.
  virtual const wchar_t* LocalCaption() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Set the parent (host) window.
  // Parameters:
  //  parent is the parent to host the window on.
  // Note:
  //  When the object uses a controller, the controller
  //  must be set before calling SetParent() otherwise creation
  //  of the underlying control may fail. 
  virtual void SetParent(RHINO_WINDOW_HANDLE parent) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Get the parent (host) window. This will return null unless a parent has been set in SetParent. 
  virtual RHINO_WINDOW_HANDLE Parent() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Return true if the underlying window has been created, else false.
  // Returns:
  //  Return true if the underlying window has been created, else false.
  virtual bool Created() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Destroys the underlying window and deletes the object.
  virtual void Release() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Gets the top left corner of this window.
  virtual ON_2iPoint Location() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Sets the top left corner of this window.
  // Parameters:
  //  location is the point to move to within the parent. 
  virtual void SetLocation(ON_2iPoint location) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Gets the current window size.
  virtual ON_2iSize Size() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Sets the window size.
  // Parameters:
  //  size - is the new size for the window. 
  //  redraw - if true then repaint the window after resizing else just resize
  //           it without redrawing.
  //  redrawBorder - if true then redraw the window border else do not. 
  virtual void SetSize(ON_2iSize size, bool redraw = false, bool redrawBorder = false) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Shows or hides the window.
  // Parameters:
  //  visible is true to show the window or false to hide it. 
  virtual void SetIsVisible(bool visible) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Return true if the window is visible, else false.
  // Returns:
  //  Return true if the window is visible, else false. 
  virtual bool IsVisible() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Enables or disables the window.
  // Parameters:
  //  enable is true to enable the window or false to disable it.
  virtual void SetIsEnabled(bool enable) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Return true if the window is enabled, else false.
  // Returns:
  //  Return true if the window is enabled, else false.
  virtual bool IsEnabled() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Sets the keyboard focus to the window. 
  virtual void SetFocused() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Refreshes the window by repainting it.
  // Parameters:
  //  immediate is true if the window should be repainted immediately, else false.
  // Note:
  //  On Microsoft Windows, Refresh(true) is equivalent to RedrawWindow() and Refresh(false)
  //  is equivalent to Invalidate().
  virtual void Refresh(bool immediate = false) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Get the screen coordinates of the underlying window.
  // Parameters:
  //  rectOut accepts the coordinates.
  virtual void GetScreenCoords(ON_4iRect& rectOut) const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  If this is a Windows window the window handle, trying not to need this
  virtual RHINO_WINDOW_HANDLE Handle(void) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Reserved for future use to extend class without breaking the SDK
  virtual LPARAM WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam);
};
///////////////////////////////////////////////////////////////////////////////
//
// class CRhinoWindow
//
// Description:
//  Generic window helpers.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoWindow
{
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Make sure window styles are appropriate when running in Windows, does
  //  nothing on Mac.
  static void MakeChildPage(RHINO_WINDOW_HANDLE hwnd, RHINO_WINDOW_HANDLE hwndParent);
  //---------------------------------------------------------------------------
  // Description:
  //  Make sure window styles are appropriate when running in Windows, does
  //  nothing on Mac.
  static void MakeChildPage(IRhinoWindow* window, RHINO_WINDOW_HANDLE hwndParent);
  //---------------------------------------------------------------------------
  // Description:
  //  Make sure window styles are appropriate when running in Windows, does
  //  nothing on Mac.
  static void MakeChildPage(IRhinoWindow* window, IRhinoWindow* parent);
};
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPage
//
// Description:
//  Generic page interface, requires minimum, common interface.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoPage
{
public:
  virtual ~IRhinoPage() = default;
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoPage() = default;

  // Description:
  //  Delete the default copy constructor
  IRhinoPage(const IRhinoPage&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoPage& operator=(const IRhinoPage&) = delete;

public:
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the English name for this page.
  // Returns:
  //  The English name for this page.  
  virtual const wchar_t* EnglishTitle() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the localized name for this page using the current Rhino
  //  language Id.
  // Returns:
  //  The localized name for this page using the current Rhino language Id.
  virtual const wchar_t* LocalTitle() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Window to display in this page.
  virtual IRhinoWindow* Window() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the icon used by the host to identify this page.
  // Parameters:
  //  size[in] = Desired icon size.
  // Returns:
  //  The icon used by the host to identify this page.
  virtual RHINO_WINDOW_IMAGE_HANDLE Image(ON_2iSize size) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the default size of the page, used by the host to calculate
  //  the vertical scroll bar.  If 0x0 is returned then the default window
  //  size is used when the page is initially created.
  virtual ON_2iSize DefaultSize() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Called to get the minimum size of the page to be used by the host.
  // Returns:
  //  The minimum size of the page to be used by the host.
  virtual ON_2iSize MinimumSize() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the object is no longer referenced by the framework, decrement
  //  your reference counter and delete this object when the counter is 0.
  virtual void Release() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the host window has been created
  virtual void HostWindowCreated(const class IRhinoPageHost* host) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the host window becomes the active page or when another page
  //  is about to be made the active page.
  // Parameters:
  //  activated[in] If true then the page has become the active page otherwise;
  //    another page is about to be made the active page.
  // Returns:
  //  The return value is only significant if activated is false, in that case
  //  returning false will prevent the page from being deactivated and 
  //  returning true will allow it to be deactivated.  Return false if there
  //  is an error on the page you want the user to correct before applying 
  //  changes.
  virtual bool Activated(const class IRhinoPageHost* host, bool activated) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host to display help for the page
  // Returns:
  //  If true it returned the host will assume the page handled the help request
  //  otherwise; the host will display the default help.
  virtual bool ShowHelp(const class IRhinoPageHost* host) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Reserved for future use to extend class without breaking the SDK
  virtual LPARAM PageProc(unsigned int message, WPARAM wParam, LPARAM lParam);
};
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPageHost
//
// Description:
//  Common host interface.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoPageHost
{
public:
  virtual ~IRhinoPageHost() = default;
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoPageHost() = default;

  // Description:
  //  Delete the default copy constructor
  IRhinoPageHost(const IRhinoPageHost&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoPageHost& operator=(const IRhinoPageHost&) = delete;

public:
  //---------------------------------------------------------------------------
  // Description:
  //  The page this host is responsible for
  virtual IRhinoPage* Page() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  The runtime serial number of the document associated with this page.
  virtual unsigned int DocumentRuntimeSerialNumber() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Return the native host window
  // Returns:
  //  Return the native host window
  virtual RHINO_WINDOW_HANDLE Window() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the native window has been created, tells the host it is time
  //  to create the page window.
  virtual void Created() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the native window size changes, tells the host it is time to
  //  resize the page.
  virtual void SizedChanged(ON_2iSize size) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the native window visibility state changes, the pages
  //  visibility state should be synchronized with the host.
  virtual void VisibleChanged(bool visible) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to update the original size for the page.  This is
  //  useful when a page contains expandable sections that change the default
  //  page height.  Changing this value will update the page host scroll bars
  //  if necessary.
  virtual void ChangeDefaultSize(ON_2iSize newSize) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Reserved for future use to extend class without breaking the SDK
  virtual LPARAM PageHostProc(unsigned int message, WPARAM wParam, LPARAM lParam);
};
#pragma region OptionsPage classes
///////////////////////////////////////////////////////////////////////////////
//
// class CRhinoOptionsPageEventArgs
//
// Description:
//  Arguments class passed to IRhinoOptionsPage notification methods.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoOptionsPageEventArgs
{
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  // Parameters:
  //  documentRuntimeSerailNumber [in] = The document associated with this event
  CRhinoOptionsPageEventArgs(class IRhinoOptionsPageHost* host);
  //---------------------------------------------------------------------------
  // Description:
  //  Destructor.
  ~CRhinoOptionsPageEventArgs();
  //---------------------------------------------------------------------------
  // Description:
  //  Copy constructor.
  CRhinoOptionsPageEventArgs(const CRhinoOptionsPageEventArgs& src);
  //---------------------------------------------------------------------------
  // Description:
  //  Operator =
  const CRhinoOptionsPageEventArgs& operator=(const CRhinoOptionsPageEventArgs& src);
  //---------------------------------------------------------------------------
  // Description:
  //  The Options page host.
  class IRhinoOptionsPageHost* Host() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the associated host documents runtime serial
  //  number.
  // Returns:
  //  The associated host documents runtime serial number.
  unsigned int DocumentRuntimeSerialNumber() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the associated host document.
  // Returns:
  //  The associated host document.
  CRhinoDoc* Document() const;
  
private:
  class CPrivateRhOptionsPageArgs* m_options_page_args = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
// class RhinoOptionPageButtons
//
// Description:
//  The standard buttons that can be optionally displayed by IRhinoOptionsPage
//
///////////////////////////////////////////////////////////////////////////////
#pragma region RH_C_SHARED_ENUM [RhinoOptionPageButtons] [Rhino.UI.OptionPageButtons] [int]
/// <summary>
/// Standard IRhinoOptionsPageButton
/// </summary>
enum class RhinoOptionPageButtons : int
{
  /// <summary>
  /// Don't display any of the standard buttons.
  /// </summary>
  None = 0x00000000,
  /// <summary>
  /// The "Restore Defaults" button located at the bottom of the host.
  /// </summary>
  DefaultButton = 0x00000001,
  /// <summary>
  /// The "Apply" button located at the bottom of the host.
  /// </summary>
  ApplyButton   = 0x00000002,
};
#pragma endregion

RHINO_SDK_FUNCTION RhinoOptionPageButtons operator&(RhinoOptionPageButtons a, RhinoOptionPageButtons b);
RHINO_SDK_FUNCTION RhinoOptionPageButtons operator|(RhinoOptionPageButtons a, RhinoOptionPageButtons b);
RHINO_SDK_FUNCTION RhinoOptionPageButtons& operator|=(RhinoOptionPageButtons& a, RhinoOptionPageButtons b);
RHINO_SDK_FUNCTION RhinoOptionPageButtons& operator&=(RhinoOptionPageButtons& a, RhinoOptionPageButtons b);
RHINO_SDK_FUNCTION RhinoOptionPageButtons operator~(RhinoOptionPageButtons a);

///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPropertiesPanelPageHost
//
// Description:
//  Provides access the object properties panel page host.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoOptionsPageHost : public IRhinoPageHost
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoOptionsPageHost() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default copy constructor
  IRhinoOptionsPageHost(const IRhinoOptionsPageHost&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoOptionsPageHost& operator=(const IRhinoOptionsPageHost&) = delete;
public:
  //---------------------------------------------------------------------------
  // Description:
  //  This must be implemented and may be used to store child pages if
  //  supported.
  // Parameters:
  //  child[in] The new host object to add as a child.
  //  documentRuntimeSerialNumber[in] The runtime serial number for the
  //    document associated with the child page.
  // Returns:
  //  Returns the index for the new child item or -1 if something went wrong.
  virtual int AddChild(class IRhinoOptionsPage* child, unsigned int documentRuntimeSerialNumber) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  This must be implemented and may be used to store child pages if
  //  supported.
  // Parameters:
  //  child[in] The new host object to add as a child.
  // Returns:
  //  Returns the index for the new child item or -1 if something went wrong.
  virtual int AddChild(IRhinoOptionsPageHost* child) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  This must be implemented and may be used to remove a child page if child
  //  pages are supported.
  // Parameters:
  //  child[in] The child page to remove.
  //  releaseChildPage[in] If true and the child page is successfully removed
  //    then Release will be called on the page.
  // Returns:
  //  Returns the index for the new child item or -1 if something went wrong.
  virtual bool RemoveChild(IRhinoOptionsPageHost* childToRemove, bool releaseChildPage) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  This must be implemented and may be used to set the active page.
  // Parameters:
  //  page[in] The page to set as the active page
  virtual void MakeActivePage() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to update the host dialog reference to this pages tile
  //  string.
  virtual void RefreshPageTitle() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the parent host for this page host.
  // Returns:
  //  Returns the parent or null if there is none.
  virtual IRhinoOptionsPageHost* Parent() = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoOptionsPage
//
// Description:
//  Interface used to extend the Rhino options or document properties dialogs.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoOptionsPage : public IRhinoPage
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoOptionsPage() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default copy constructor
  IRhinoOptionsPage(const IRhinoOptionsPage&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoOptionsPage& operator=(const IRhinoOptionsPage&) = delete;

public:
  //---------------------------------------------------------------------------
  // Description:
  //  This method gets called after the plug-in returns the pages to add to
  //  Rhino and the associated host objects are created.  Save this pointer
  //  and use it to communicate with the host or just ignore the call if you
  //  don't care.
  virtual void HostAttached(IRhinoOptionsPageHost* host);
  //---------------------------------------------------------------------------
  // Description:
  //  Called when the page has been added to the appropriate Rhino user
  //  interface.  This is a good time to set attributes like bold or title text
  //  color.
  virtual void AttachedToUi(CRhinoOptionsPageEventArgs& e);
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the script-able version of the host command when this page is
  //  selected.  You should use CRhinoGet... methods to interact with users when
  //  overriding this method.
  // Returns:
  //  The return value notifies the calling command to continue or cancel.
  virtual CRhinoCommand::result RunScript(CRhinoOptionsPageEventArgs& e) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when it is time to update the page contents reflecting
  //  the current options states.
  // Parameters:
  //  e[in] - Event arguments.
  virtual void UpdatePage(CRhinoOptionsPageEventArgs& e) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when deciding which standard buttons to display.
  // Returns:
  //  The standard controls to display, returns RhinoOptionPageButtons::None
  //  by default.
  virtual RhinoOptionPageButtons ButtonsToDisplay() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when deciding if a visible standard control should be
  //  enabled or not.
  // Parameters:
  //  control[in] = The control to enable.
  // Returns:
  //  Return true to enable the control or false to disable it, returns false
  //  by default.
  virtual bool IsButtonEnabled(RhinoOptionPageButtons control) const;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when it is time to apply changes to the application or
  //  document.
  // Parameters:
  //  e[in] - Event arguments.
  // Returns:
  //  Returns true if it is safe to close the page and or host or false if
  // there was an error on the page.
  virtual bool Apply(CRhinoOptionsPageEventArgs& e);
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when the page modifications have been canceled.
  virtual void Cancel(CRhinoOptionsPageEventArgs& e);
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when the "Restore Defaults" button is clicked, will be
  //  followed by a UpdatePage call to update the page contents.
  virtual void OnRestoreDefaultsClick(CRhinoOptionsPageEventArgs& e);
};
///////////////////////////////////////////////////////////////////////////////
//
// class CRhinoOptionsPageCollectionItem
//
// Description:
//  A CRhinoOptionsPageCollectionItem representing a item added to a
//  CRhinoOptionsPageCollection.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoOptionsPageCollectionItem
{
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Constructor
  // Parameters:
  //  page[in] Page to manage
  CRhinoOptionsPageCollectionItem(IRhinoOptionsPage* page);
  //---------------------------------------------------------------------------
  // Description:
  //  Destructor
  ~CRhinoOptionsPageCollectionItem();
  //---------------------------------------------------------------------------
  // Description:
  //  Page to manage
  IRhinoOptionsPage* Page() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Add child page to the managed page
  // Parameters:
  //  pageToAdd[in] The child page to add
  // Returns:
  //  The new collection item for the child page, use it to nest child pages.
  CRhinoOptionsPageCollectionItem* AddPage(IRhinoOptionsPage* pageToAdd);
  //---------------------------------------------------------------------------
  // Description:
  //  Number of child pages directly owned by this page
  // Returns:
  //  Returns the number of child pages directly owned by this page
  int Count() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Child page from index
  // Parameters:
  //  index[in] = Zero based index of item to get
  // Returns:
  //  Returns the child item if the index is valid otherwise; returns null.
  CRhinoOptionsPageCollectionItem* operator[](int index) const;

private:
  //---------------------------------------------------------------------------
  CRhinoOptionsPageCollectionItem(const CRhinoOptionsPageCollectionItem&) = delete;
  //---------------------------------------------------------------------------
  const CRhinoOptionsPageCollectionItem& operator=(const CRhinoOptionsPageCollectionItem&) = delete;
  //---------------------------------------------------------------------------
  // For internal use only
  class CPrivateRhOptionsPageCollectionItem* m_private_page_collection_item = nullptr;
};
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoOptionsPageCollection
//
// Description:
//  A collection of IRhinoOptionsPage objects, used to add pages.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoOptionsPageCollection
{
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  CRhinoOptionsPageCollection(unsigned int documentRuntimeSerialNumber, HWND parent);
  //---------------------------------------------------------------------------
  // Description:
  //  Destructor
  ~CRhinoOptionsPageCollection();
  //---------------------------------------------------------------------------
  // Description:
  //  Runtime serial for document displaying the options dialog.
  unsigned int DocumentRuntimeSerialNumber() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Document displaying the options dialog.
  CRhinoDoc* Document() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Document properties window handle.
  HWND Parent() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Add top level page
  // Parameters:
  //  pageToAdd[in] The page to add
  // Returns:
  //  The new collection item for the page, use it to add child pages.
  CRhinoOptionsPageCollectionItem* AddPage(IRhinoOptionsPage* pageToAdd);
  //---------------------------------------------------------------------------
  // Description:
  //  Number of top level pages in this collection
  // Returns:
  //  Returns the top level pages in this collection
  int Count() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Top level page to add from index
  // Parameters:
  //  index[in] = Zero based index of item to get
  // Returns:
  //  Returns the top level item if the index is valid otherwise; returns null.
  CRhinoOptionsPageCollectionItem* operator[](int index) const;
private:
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default copy constructor
  CRhinoOptionsPageCollection(const CRhinoOptionsPageCollection&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  CRhinoOptionsPageCollection& operator=(const CRhinoOptionsPageCollection&) = delete;
  //---------------------------------------------------------------------------
  // For internal use only
  class CRhPrivateOptionsPageCollection* m_private_collection = nullptr;
};

#pragma endregion OptionsPage classes

#pragma region PropertiesPanelPage classes

///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPropertiesPanelPageHost
//
// Description:
//  Provides access the object properties panel page host.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoPropertiesPanelPageHost : public IRhinoPageHost
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoPropertiesPanelPageHost() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Default destructor.
  ~IRhinoPropertiesPanelPageHost() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default copy constructor
  IRhinoPropertiesPanelPageHost(const IRhinoPropertiesPanelPageHost&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoPropertiesPanelPageHost& operator=(const IRhinoPropertiesPanelPageHost&) = delete;
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Call this to get a list of all view pages that can appear on the
  //  properties panel.
  static void GetViewPages(ON_SimpleArray<IRhinoPropertiesPanelPageHost*>& viewPages);
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to modify objects or views associated with this page.
  //  Rhino will put the properties page manager in edit mode and call your 
  //  OnModifypge with a IRhinoPropertiesPanelPageEventArgs when it is safe
  //  to modify objects or views.
  virtual void ModifyPage() = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  The unique runtime Id for this page.
  virtual const ON_UUID& RuntimeId() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to post a UpdatePage call which will occur on a on idle
  //  event when this page is visible.
  virtual void PostUpdatePageCall() const = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
//  class RhinoPropertiesPanelPageType
//
// Description:
//  The type of properties page this is.
//
///////////////////////////////////////////////////////////////////////////////
#pragma region RH_C_SHARED_ENUM [RhinoPropertiesPanelPageType] [Rhino.UI.PropertyPageType] [int]
/// <summary>
/// IRhinoProperties page type
/// </summary>
enum class RhinoPropertiesPanelPageType : int
{
  /// <summary>
  /// Replace the material page with this page; only works for the current render plug - in.
  /// </summary>
  Material = 0,
  /// <summary>
  /// Replace the light page with this page; only works for the current render plug-in.
  /// </summary>
  Light,
  /// <summary>
  /// User-defined custom object page.
  /// </summary>
  Custom,
  /// <summary>
  /// For internal use only.
  /// </summary>
  ObjectProperties,
  /// <summary>
  /// If page is provided by Rhino and only dimensions are selected, activate this page.
  /// </summary>
  Dimension,
  /// <summary>
  /// If page is provided by Rhino and leaders are selected, activate this page.
  /// </summary>
  Leader,
  /// <summary>
  /// If page is provided by Rhino and only text objects are selected, activate this page.
  /// </summary>
  Text,
  /// <summary>
  /// If page is provided by Rhino and only hatch objects are selected, activate this page.
  /// </summary>
  Hatch,
  /// <summary>
  /// If page is provided by Rhino and only dot objects are selected, activate this page.
  /// </summary>
  Dot,
  /// <summary>
  /// Replace the texture mapping page with this page; only works for the current render plug-in.
  /// </summary>
  TextureMapping,
  /// <summary>
  /// If page is provided by Rhino and only detail objects are selected, activate this page.
  /// </summary>
  Detail,
  /// <summary>
  /// If page is provided by Rhino and only clipping plane objects are selected, activate this page.
  /// </summary>
  ClippingPlane,
  /// <summary>
  /// If page is provided by Rhino and only named view widget objects are selected, activate this page.
  /// </summary>
  NamedView,
  /// <summary>
  /// Decals user interface
  /// </summary>
  Decal,
  /// <summary>
  ///  Page is a view properties page and uses view properties methods. Does not use object properties methods.
  /// </summary>
  View,
  /// <summary>
  ///  Reserved, do not use.
  /// </summary>
  PageCount,
};
#pragma endregion
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPropertiesPanelPageEventArgs
//
// Description:
//  Interface used to extend the Rhino object properties panel.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoPropertiesPanelPageEventArgs
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoPropertiesPanelPageEventArgs() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Default destructor.
  ~IRhinoPropertiesPanelPageEventArgs() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default copy constructor
  IRhinoPropertiesPanelPageEventArgs(const IRhinoPropertiesPanelPageEventArgs&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoPropertiesPanelPageEventArgs& operator=(const IRhinoPropertiesPanelPageEventArgs&) = delete;
public:
  //---------------------------------------------------------------------------
  // Description:
  //  This is being provided temporarily to allow access to the selected object
  //  list from the IRhinoPropertiesPanelPage.
  // Returns:
  //  If the page is valid then a pointer to the pages 
  // IRhinoPropertiesPanelPageEventArgs interface is returned otherwise; null
  // is returned.  DO NOT delete this pointer.
  static IRhinoPropertiesPanelPageEventArgs* FromPage(const class IRhinoPropertiesPanelPage* page);
  //---------------------------------------------------------------------------
  // Description:
  //  This is being provided temporarily to allow access to the selected object
  //  list from the IRhinoPropertiesPanelPageHost.
  // Returns:
  //  If the page is valid then a pointer to the host page 
  // IRhinoPropertiesPanelPageEventArgs interface is returned otherwise; null
  // is returned.  DO NOT delete this pointer.
  static IRhinoPropertiesPanelPageEventArgs* FromHost(const class IRhinoPropertiesPanelPageHost* host);
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the number of selected objects.
  // Returns:
  //  Returns the number of selected objects.
  virtual int ObjectCount() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method when iterating the selected object list.
  // Returns:
  //  Returns the object at the specified index or null if the index is not
  //  valid.
  virtual const CRhinoObject* ObjectAt(int index) const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the combined CRhinoObject::ObjectType in the
  //  selection list.  This is the fastest way of determining if a specific
  //  object type exist in the current selection.
  // Returns:
  //  Returns the combined CRhinoObject::ObjectType types of the currently 
  //  selected objects.
  virtual unsigned int ObjectTypes() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to determine if the list contains any objects of the
  //  specified type.
  // Parameters:
  //  objectTypes [in] - Type(s) of objects to filter for, see CRhinoObject::ObjectType
  // Returns:
  //  Returns true if there is one or more objects of objectType in currently 
  //  selected or false if there are none.
  virtual bool AnySelected(unsigned int objectTypes) const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get a list of the currently selected objects
  //  of a specified type.
  // Parameters:
  //  objectTypes [in] - Type(s) of objects to filter for, see
  //                     ON::object_type
  //  objectList [out] - Selected objects of objectType are added to this list.
  //                     This method does not empty the list.
  // Returns:
  //  Returns the number of items added to objectList.
  virtual int GetObjects(unsigned int objectTypes, ON_SimpleArray<const CRhinoObject*>& objectList) const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get a list of the currently selected objects.
  //  This method calls GetSelectedObjects(ON::object_type::any_object, objectList).
  // Parameters:
  //  objectList [out] - Selected objects of objectType are added to this list.
  //                     This method does not empty the list.
  // Returns:
  //  Returns the number of items added to objectList.
  virtual int GetObjects(ON_SimpleArray<const CRhinoObject*>& objectList) const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the view that was active when the message was
  //  generated.
  // Returns:
  //  Returns the view that was active when the message was generated.
  virtual CRhinoView* View() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the runtime serial number for the view that was
  //  active when the message was generated.
  // Returns:
  //  Returns the runtime serial number for the view that was active when the
  //  message was generated.
  virtual unsigned int ViewRuntimeSerialNumber() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the viewport that was active when the message was
  //  generated.
  // Returns:
  //  Returns the viewport that was active when the message was generated.
  virtual class CRhinoViewport* Viewport() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the runtime serial number for the viewport that was
  //  active when the message was generated.
  // Returns:
  //  Returns the runtime serial number for the viewport that was active when the
  //  message was generated.
  virtual ON_UUID ViewportId() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the runtime serial number for the document that
  //  was active when the message was generated.
  // Returns:
  //  Returns the document that was active or null if there was none.
  virtual CRhinoDoc* Document() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to get the runtime serial number for the document that
  //  was active when the message was generated.
  // Returns:
  //  Returns the runtime serial number for the document that was active.
  virtual unsigned int DocumentRuntimeSerialNumber() const = 0;
};
///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoPropertiesPanelPage
//
// Description:
//  Interface used to extend the Rhino object properties panel.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoPropertiesPanelPage : public IRhinoPage
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoPropertiesPanelPage() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Default destructor.
  ~IRhinoPropertiesPanelPage() = default;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default copy constructor
  IRhinoPropertiesPanelPage(const IRhinoPropertiesPanelPage&) = delete;
  //---------------------------------------------------------------------------
  // Description:
  //  Delete the default operator=
  IRhinoPropertiesPanelPage& operator=(const IRhinoPropertiesPanelPage&) = delete;
public:
  //---------------------------------------------------------------------------
  // Description:
  //  This method gets called after the plug-in returns the pages to add to
  //  Rhino and the associated host objects are created.  Save this pointer
  //  and use it to communicate with the host or just ignore the call if you
  //  don't care.
  virtual void HostAttached(IRhinoPropertiesPanelPageHost* host);
  //---------------------------------------------------------------------------
  // Description:
  //  This method is called when updating the contents of the view or
  //  object properties navigation controls.
  // Parameters:
  //  args - [in] Information about the currently selected view or currently
  //              selected object list.
  // Returns:
  //  Return true to include the page in the navigation panel or false to exclude it.
  virtual bool IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs& args) const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the script-able version of the host command when this page is
  //  selected.  You should use CRhinoGet... methods to interact with users when
  //  overriding this method.
  // Returns:
  //  The return value notifies the calling command to continue or cancel.
  virtual CRhinoCommand::result RunScript(IRhinoPropertiesPanelPageEventArgs& args) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  Called by the host when it is time to update the page contents reflecting
  //  the current options states.
  // Parameters:
  //  e[in] - Event arguments.
  virtual void UpdatePage(IRhinoPropertiesPanelPageEventArgs& e) = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  This method gets called by ModifyPage when it is safe to modify the
  //  selected objects or view.
  virtual void OnModifyPage(IRhinoPropertiesPanelPageEventArgs& args);// = 0;

  //---------------------------------------------------------------------------
  // Description:
  //   This defaults to custom_page.  Override and return the system page you want to replace.  Values of material_page and
  //   light_page are only meaningful if this page was added by a CRhinoRenderPlugIn and the plug-in is the current renderer.
  virtual RhinoPropertiesPanelPageType PageType() const;
  //---------------------------------------------------------------------------
  // Description:
  //  The page navigation control adds buttons in the order the pages are 
  //  processed, override this method and return a sort index to move the button
  //  to the beginning of the list.  By default this returns -1 which puts the
  //  button at the end of the list.
  virtual int Index(void) const;
  //---------------------------------------------------------------------------
  // Description:
  //  This method is called in response to a specific event, see
  //  CRhinoPropertiesDialogPageEvent for event descriptions.  This method has
  //  been provided to allow extending the class in the future without breaking
  //  the SDK.
  // Parameters:
  //  args  - [in] Describes this event and provides access to the selected
  //               object and/or active view list.
  // Returns:
  //  Return value is dependent on the event reason provided in the args
  //  parameter.
  virtual bool PropertiesDialogPageEvent(IRhinoPropertiesPanelPageEventArgs& args);
  //---------------------------------------------------------------------------
  // Description:
  //  Override and return true if this is a object properties and not view
  //  properties page and it supports sub object selection.  Returns false
  //  by default.
  // Returns:
  //  Returns false by default.
  virtual bool SupportsSubObjectSelection(void) const;
  //---------------------------------------------------------------------------
  // Description:
  //  If you want the page to appear in the modal Named View properties dialog,
  //  override this method and return a new instance of this page.
  virtual IRhinoPropertiesPanelPage* NewNamedViewPage() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Returns the IRhinoPropertiesPanelPageHost containing the specified page.
  // Returns:
  //  Returns the IRhinoPropertiesPanelPageHost containing the specified page.
  static IRhinoPropertiesPanelPageHost* GetHost(IRhinoPropertiesPanelPage* page);
};
///////////////////////////////////////////////////////////////////////////////
//
// class CRhinoPropertiesPanelPageCollection
//
// Description:
//  Passed to CRhinoPlugIn::AddObjectPropertiesPages and is used to add
//  plug-in provided object properties pages.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS CRhinoPropertiesPanelPageCollection
{
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Constructor
  CRhinoPropertiesPanelPageCollection(unsigned int documentRuntimeSerialNumber);
  //---------------------------------------------------------------------------
  // Description:
  //  Destructor
  ~CRhinoPropertiesPanelPageCollection();
  //---------------------------------------------------------------------------
  // Description:
  //  Runtime serial for document displaying the options dialog.
  unsigned int DocumentRuntimeSerialNumber() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Document displaying the options dialog.
  CRhinoDoc* Document() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Call this method to add a plug-in provided page to the collection
  void Add(IRhinoPropertiesPanelPage* page);
  //---------------------------------------------------------------------------
  // Description:
  //  Number of items in this collection
  int Count() const;
  //---------------------------------------------------------------------------
  // Description:
  //  Provide operator[] access to collection array
  IRhinoPropertiesPanelPage* operator[](int index) const;

private:
  CRhinoPropertiesPanelPageCollection(const CRhinoPropertiesPanelPageCollection&) = delete;
  const CRhinoPropertiesPanelPageCollection& operator=(const CRhinoPropertiesPanelPageCollection&) = delete;
  class CRhPrivatePropertiesPanelPageCollection* m_private_collection = nullptr;
};
#pragma endregion PropertiesPanelPage classes
