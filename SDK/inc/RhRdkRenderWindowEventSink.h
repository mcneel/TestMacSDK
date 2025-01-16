
#pragma once // Public SDK header

#include "RhRdkExtension.h"

/** \class CRhRdkRenderWindowEventSink

	This interface is deprecated in favor of CRhRdkRenderWindowMouseEventWatcher.

*/
RDK_DEPRECATED class RHRDK_SDK CRhRdkRenderWindowEventSink : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	virtual void OnLButtonDown(const CPoint& pt) = 0;

	// These were never implemented and are not called.
	virtual void OnLButtonUp(const CPoint& pt) = 0;
	virtual void OnLButtonDblClick(const CPoint& pt) = 0;
	virtual void OnRButtonDown(const CPoint& pt) = 0;
	virtual void OnRButtonUp(const CPoint& pt) = 0;
	virtual void OnRButtonDblClick(const CPoint& pt) = 0;
};

/** \class CRhRdkRenderWindowMouseEventWatcher

	This object is an RDK extension that allows a plug-in to find out about mouse activity
	in the render window. To use it, derive a class that implements each abstract method
	to respond to mouse events. Then register your subclass as follows:

	\code
	void CMyRdkPlugIn::RegisterExtensions(void) const
	{
		...
		AddExtension(new CMyRenderWindowEventWatcher);
		...
	}
	\endcode

	The mouse event methods will be called when the user operates the mouse inside the view
	area (rendering) of any render window that was opened by your plug-in. You can use the
	supplied render window reference to determine which render window it is.

*/
class RHRDK_SDK CRhRdkRenderWindowMouseEventWatcher : public CRhRdkExtension
{
public:
	/** Implement this method to respond to mouse move events. */
	virtual void OnMouseMove(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

	/** Implement this method to respond to left mouse button down events. */
	virtual void OnLButtonDown(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

	/** Implement this method to respond to left mouse button up events. */
	virtual void OnLButtonUp(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

	/** Implement this method to respond to left mouse button double-click events. */
	virtual void OnLButtonDblClk(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

	/** Implement this method to respond to right mouse button down events. */
	virtual void OnRButtonDown(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

	/** Implement this method to respond to right mouse button up events. */
	virtual void OnRButtonUp(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

	/** Implement this method to respond to right mouse button double-click events. */
	virtual void OnRButtonDblClk(IRhRdkRenderWindow& rw, const ON_2iPoint& point) = 0;

public:
	virtual UUID InterfaceId(void) const final override;
};

/** \class CRhRdkRenderWindowCustomDlg

	This custom plug-in interface allows you to create a custom dialog to be hosted on an 'extra side pane'
	inside the render window. This is in addition to the 'main side pane' which is the pane that contains the
	post effect tabs (Early, Tone Mapping, Late) and any custom tabs that may have been added. Since it is
	possible to have more than one open render window, it is possible for this interface to access more than
	one hosted dialog. This is made possible by use of the render session id which also identifies the dialog.
	Your implementation must ensure that it is possible to have multiple dialogs identified by this UUID.

*/
class RHRDK_SDK CRhRdkRenderWindowCustomDlg : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	/** Implement this method to return the title that will appear on the extra side pane when it is floating.
		\note This is currently not used since there is no 'dock bar' in the cross-platform UI and it
		 therefore can never 'float'. It might be used again in the future if we ever decide to implement
		 cross-platform floating side panes.
		\param uuidSidePaneUI is the unique identifier of the side-pane UI. This is typically the same as
		 the render session id, but in some cases (e.g., Rhino Options), it is not. */
	virtual const wchar_t* DockBarTitle(const UUID& uuidSidePaneUI) const = 0;

	/** Implement this method to construct and create your dialog.
		\param hParent is the parent window.
		\param uuidSidePaneUI is the unique identifier of the side-pane UI. This is typically the same as
		 the render session id, but in some cases (e.g., Rhino Options), it is not. When it is, you can get
		 a pointer to the render window interface by calling RhRdkFindRenderWindow().
		\see RhRdkFindRenderWindow() */
	virtual bool Create(HWND hParent, const UUID& uuidSidePaneUI) = 0;

	/** Implement this method to destroy and delete your dialog.
		\param uuidSidePaneUI is the unique identifier of the side-pane UI. This is typically the same as
		 the render session id, but in some cases (e.g., Rhino Options), it is not. */
	virtual void Destroy(const UUID& uuidSidePaneUI) = 0;

	/** Implement this method to move your dialog to the position specified by 'rect'.
		\param rect specifies the position to move the dialog to.
		\param uuidSidePaneUI is the unique identifier of the side-pane UI. This is typically the same as
		 the render session id, but in some cases (e.g., Rhino Options), it is not. */
	virtual void MoveWindow(const ON_4iRect& rect, const UUID& uuidSidePaneUI) = 0;

	/** Implement this method to return the flags to be passed to CControlBar::EnableDocking() (e.g., CBRS_ALIGN_ANY)
		\note This is currently not used. */
	virtual UINT EnableDockingFlags(void) const = 0;

	/** Implement this method to return the ids to use for positioning the side pane.
		For backward compatibility, this uses the old MFC dock-bar ids. Supported ids are:
			AFX_IDW_DOCKBAR_LEFT
			AFX_IDW_DOCKBAR_TOP
			AFX_IDW_DOCKBAR_RIGHT
			AFX_IDW_DOCKBAR_BOTTOM
		\note The term 'Flags' here is a typo -- it should be 'Ids'. */
	virtual UINT DockControlBarFlags(void) const = 0;

	/** Implement this method to return the initial visibility of the side pane. */
	virtual bool InitialShowControlBar(void) const = 0;

	/** Implement this method to return \e true if you want this dialog to always appear on the render window
		even when the render window is for a different renderer. If you return \e false, the dialog and side pane
		will only be created when the render window's renderer id matches the render engine id.
		\note It should be \e very rare to implement this as anything other than \code return false; \endcode. */
	virtual bool AlwaysShowDialog(void) const = 0;

	/** Implement this method to return the window handle of your dialog.
		\param uuidSidePaneUI is the unique identifier of the side-pane UI. This is typically the same as
		 the render session id, but in some cases (e.g., Rhino Options), it is not. */
	virtual HWND WindowHandle(const UUID& uuidSidePaneUI) const = 0;
};

/** \class CRhRdkRenderFrameTabFactory

	RDK provides 3 tabs on the Render Window's main side pane for 'Early', 'Tone Mapping' and 'Late'
	Post-Effects. This class is an RDK extension which is supposed to allow users to add their own tabs
	to this pane. This worked in the MFC UI but it's now deprecated (from Rhino 8) due to it not working
	in the cross-platform (Eto) UI.

*/
class RHRDK_SDK RDK_DEPRECATED CRhRdkRenderFrameTabFactory : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	virtual class CRhinoUiDockBarTab* NewTab(IRhRdkRenderWindow&) const = 0;
	//            ^^^^^^^^^^^^^^^^^^ This class is MFC-bound and incomplete. It can't be hosted in Eto.

	virtual UUID RenderEngineId(void) const = 0;
	virtual UUID TabId(void) const = 0;
};

#pragma warning(push)
#pragma warning(disable : 4996)

/** \class CRhRdkRenderFrameTabFactoryEx

	This was an extension for use on the Mac although I'm not sure it was ever used there.
	It's now deprecated (from Rhino 8) due to it not working in the cross-platform (Eto) UI.

*/
class RHRDK_SDK RDK_DEPRECATED CRhRdkRenderFrameTabFactoryEx : public CRhRdkRenderFrameTabFactory
{
public:
	virtual class CRhinoUiDockBarTab* NewTabEx(CRhRdkControllerPtr con, HWND parent) const = 0;
	//            ^^^^^^^^^^^^^^^^^^ This class is MFC-bound and incomplete. It can't be hosted in Eto.
};

#pragma warning(pop)

/** \class IRhinoWindowOnTab

	This is an extended IRhinoWindow for windows (panes) which are hosted on a tab control.

*/
class IRhinoWindowOnTab : public IRhinoWindow
{
public:
	/** Called when the tab is activated or deactivated. A tab is activated when the user
		clicks on it and deactivated when the user clicks on a different tab. */
	virtual void OnTabActivateChange(bool active) = 0;
};

/** \class CRhRdkRenderWindowMainSidePaneTabFactory

	RDK provides 3 tabs on the Render Window's main side pane for 'Early', 'Tone Mapping' and 'Late'
	Post-Effects. This class is an RDK extension which allows users to add their own tabs to this pane.
	It supersedes CRhRdkRenderFrameTabFactory and CRhRdkRenderFrameTabFactoryEx in Rhino 8 upwards.

*/
class RHRDK_SDK CRhRdkRenderWindowMainSidePaneTabFactory : public CRhRdkExtension
{
public:
	virtual ON_UUID InterfaceId(void) const final override;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

public:
	/** \return the tab id. This identifies the tab in the row of tabs. */
	virtual ON_UUID TabId(void) const = 0;

	/** \return the render engine id of your renderer. */
	virtual ON_UUID RenderEngineId(void) const = 0;

	/** Get an icon to be drawn on the tab.
		\param size is the required size of the icon. This is typically 16x16.
		\param dib accepts the icon imagery. */
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dib, void* reserved) const = 0;

	/** Create a new tab page. This must create the window object but not the actual window handle.
		\param con is a controller. TODO: Elaborate.
		\param uuidSidePaneUI is the unique identifier of the side-pane UI. This is typically the same as
		 the render session id, but in some cases (e.g., Rhino Options), it is not.
		\param reserved is reserved for future use.
		\return The new tab page or null on failure. */
	virtual IRhinoWindowOnTab* NewTabPage(CRhRdkControllerPtr con, const ON_UUID& uuidSidePaneUI, void* reserved) const = 0;
};
