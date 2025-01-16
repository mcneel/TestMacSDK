
#pragma once // Public SDK header

#include "RhRdkExtension.h"

/** \class IRhRdkCommandUpdate

	This interface is used to update custom menu or tool bar UI elements.
	It is similar to the MFC CCmdUI object but does not need MFC to work.

*/
class RHRDK_SDK IRhRdkCommandUpdate
{
public:
	enum class Checks : unsigned int { Unchecked, Checked, Indeterminate };

	/** Call this method to set the checked state of the UI item. */
	virtual void SetCheck(Checks c) = 0;

	/** Call this method to enable or disable the UI item. */
	virtual void SetEnable(bool bEnable=true) = 0;

	/** Call this method to check or uncheck the UI item radio state. */
	virtual void SetRadio(bool bOn=true) = 0;

	/** Call this method to set the text of the UI item. */
	virtual void SetText(const wchar_t* wszText) = 0;

	/** \return the command id of the UI element. */
	virtual unsigned int CommandId(void) const = 0;

	/** \return the UI element's position. */
	virtual unsigned int Position(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkCommandUpdate() { }
};

/** \class CRhRdkContentEditorCustomButtonProvider

	This class allows a client to add custom buttons to the bottom of content editor windows.
	You can implement the abstract interface to provide custom buttons on the content editors.

	\note This class replaces IRhRdkContentBrowserDockBarButtonInterface in Rhino 5
	 and IRhRdkContentEditorCustomButtonInterface in earlier versions of Rhino 6 WIP.

*/
class RHRDK_SDK CRhRdkContentEditorCustomButtonProvider : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	/** Implement this method to return your render engine's id. */
	virtual UUID RenderEngineID(void) const = 0;

	/** Implement this method to return the left margin before the first button. */
	virtual int LeftMargin(void) const = 0;

	/** Implement this method to return the height of your custom buttons. */
	virtual int ButtonHeight(void) const = 0;

	enum class Style : unsigned int
	{
		Simple    = 0, // Normal old-style solid button.
		Flat      = 1, // Flat button that does not pop up.
		FlatPopup = 2, // Flat button that pops up when the mouse is over it (default).
	};

	/** Implement this method to describe your custom buttons.
		For each content kind, RDK sets iIndex to zero and calls this method continually increasing
		iIndex until it returns \e false.
		\param iIndex is the button index starting at zero and increasing with each call.
		\param kind is the kind of content being displayed in the associated editor.
		\param dibOut accepts the button's imagery (or 'icon'). If an icon is not required, do not set this parameter.
		\param sCaptionOut accepts the button's caption. If a caption is not required, do not set this parameter.
		\param sToolTipOut accepts the button's tool-tip. If a tool-tip is not required, do not set this parameter.
		\param iWidthOut accepts the button's width. If you do not set this parameter, the width will be 23 pixels.
		\param iGapOut accepts the gap after the button. If you do not set this parameter, the gap will be 2 pixels.
		\param colBackOut is the button's background color. If a special color is not required, do not set this parameter.
		\param styleOut specifies the button style. \see enum class Style.
		\param dwFlagsOut is currently unused.
		\param reserved is reserved for future use.
		\return \e true if button is required, \e false to stop. */
	virtual bool ButtonDetails(int iIndex, CRhRdkContent::Kinds kind, CRhinoDib& dibOut,
	                           ON_wString& sCaptionOut, ON_wString& sToolTipOut,
	                           int& iWidthOut, int& iGapOut, COLORREF& colBackOut,
	                           Style& styleOut, DWORD& dwFlagsOut, DWORD_PTR reserved) const = 0;

	/** Implement this method to handle a button click.
		\param index is the index of the button that was clicked.
		\return \e true if handled successfully, else \e false. */
	virtual bool OnButtonClicked(int index) = 0;
};

/** \class CRhRdkCustomRenderFrameToolBar

	This interface allows a client to add custom tool bars to the render frame.

	The custom tool bar system is based on the MFC tool bars and uses the same bitmap format.
	The image for each button is 24 by 24 pixels, and the bitmap is a 'strip' of images. This
	strip is 24 pixels high and its width must be 24 times the number of buttons on the tool bar.
	This is the same as the value you return from ButtonCount() minus any separators.
	There can be up to four bitmaps for each tool bar, one for each of normal, hot, pressed and gray.

*/
class RHRDK_SDK CRhRdkCustomRenderFrameToolBar : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	/** Implement this method to return a clone of the tool bar.
		The tool bars that are registered by CRhRdkPlugIn::RegisterCustomPlugIns() are only prototypes.
		Each render frame clones the prototypes to create per-frame tool bars. */
	virtual CRhRdkCustomRenderFrameToolBar* Clone(void) const = 0;

	/** Implement this method to return the tool bar's id.
		\return The unique identifier of this tool bar. */
	virtual UUID ToolBarID(void) const = 0;

	/** Implement this method to return your render engine's id.
		\return The unique identifier of your render engine. */
	virtual UUID RenderEngineID(void) const = 0;

	/** Implement this method to return the number of buttons you need on the tool bar.
		\return The number of buttons, including separators. \note Separators are not included
		in the tool bar's bitmap. */
	virtual int ButtonCount(void) const = 0;

	/** Implement this method to return the tool bar's caption.
		\return The caption that appears when the tool bar is floating. */
	virtual const wchar_t* Caption(void) const = 0;

	/** Implement this method to return the tool bar's normal bitmap. This is the bitmap
		of images used for buttons that are not pressed or being hovered over.
		\param dibOut accepts the bitmap.
		The width of the bitmap must be 24 times the number of buttons. The height
		must be 24 pixels and the bit depth can be anything up to 24 bits.
		\return The \e true if successful, else \e false. */
	virtual bool GetNormalBitmap(CRhinoDib& dibOut) = 0;

	/** Implement this method to return the tool bar's hot (hover) bitmap. This is the bitmap
		of images used for buttons that are pressed or being hovered over.
		\param dibOut accepts the bitmap.
		The width of the bitmap must be 24 times the number of buttons. The height
		must be 24 pixels and the bit depth can be anything up to 24 bits.
		\return The \e true if successful, else \e false. */
	virtual bool GetHotBitmap(CRhinoDib& dibOut) = 0;

	/** Implement this method to return the tool bar's pressed bitmap. This is the bitmap
		of images used for buttons that are pressed.
		\param dibOut accepts the bitmap.
		The width of the bitmap must be 24 times the number of buttons. The height
		must be 24 pixels and the bit depth can be anything up to 24 bits.
		\return The \e true if successful, else \e false. */
	virtual bool GetPressedBitmap(CRhinoDib& dibOut) = 0;

	/** Implement this method to return the tool bar's gray (disabled) bitmap. This is the bitmap
		of images used for buttons that are disabled (grayed-out).
		\param dibOut accepts the bitmap.
		The width of the bitmap must be 24 times the number of buttons. The height
		must be 24 pixels and the bit depth can be anything up to 24 bits.
		\return The \e true if successful, else \e false. */
	virtual bool GetGrayBitmap(CRhinoDib& dibOut) = 0;

	/** OBSOLETE Never called. */
	RDK_DEPRECATED virtual void SetRenderWindow(class IRhRdkRenderWindow*) { }

	/** Implement this to store the render session id. */
	virtual void SetRenderSessionId(const UUID& uuidRenderSession) = 0;

	/** RDK assigns a range of contiguous command ids to the tool buttons and then notifies
		the tool bar of the first command id. Implement this method to store this first id.
		You can then use it as a base in OnCommand() and OnUpdateCommandUI().
		\param nID is the command id of the first command on this tool bar. */
	virtual void SetFirstCommandId(UINT_PTR nID) = 0;

	/** Implement this method to return a tool tip string for a command. If you do not recognise the command id, return NULL.
		\param nID is the command id of the command for which a string is required.
		\return The tool-tip for the tool button that executes the given command. */
	virtual const wchar_t* GetToolTipText(UINT_PTR nID) const = 0;

	/** Implement this method to return a hint string for a command. This string is typically displayed on the status bar
		when the user hovers over a tool button. If you do not recognise the command id, return NULL.
		\param nID is the command id of the command for which a string is required.
		\return The status bar message for the tool button that executes the given command. */
	virtual const wchar_t* GetMessageString(UINT_PTR nID) const = 0;

	/** Implement this method to specify if the tool button at a given index is a separator.
		\param iIndex is the index of the button which goes from zero to ButtonCount()-1.
		\return \e true if the button is a separator, else \e false. */
	virtual bool IsSeparator(int iIndex) const = 0;

	/** Implement this method to execute a tool bar command.
		\param nID is the command id of the command to execute.
		\return \e true if successful, else \e false. */
	virtual bool OnCommand(UINT_PTR nID) = 0;

	/** Implement this method to update a tool bar button. */
	virtual void OnUpdateCommandUI(IRhRdkCommandUpdate& cu) const = 0;
};

/** \class CRhRdkCustomRenderFrameMenu

	This interface allows a client to add custom menu items to the render frame.

*/
class RHRDK_SDK CRhRdkCustomRenderFrameMenu : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	/** Implement this method to return your render engine's id.
		\return The unique identifier of your render engine. */
	virtual UUID RenderEngineID(void) const = 0;

	/** Used by AddMenuItem() */
	enum class MenuType : unsigned int { Main, File, Edit, View, Render, Help };

	/** Implement this method to add one or more menu items to a menu.
		RDK calls this method once for each of the types in enum MenuType.
		\param menu is the menu to append the item(s) to.
		\param type is the type of menu that 'menu' refers to.
		\param cmdId is the first command id to use. You should increment this value for each item
		 so that on returning, the next available command id is passed back to the RDK.
		\return \e true if you appended an item, else \e false. */
	virtual bool AddMenuItems(HMENU menu, MenuType type, UINT& cmdId) = 0;

	/** Implement this method to execute a menu command. If you recognize the command id,
		you should execute the command and return \e true. If not, just return \e false.
		\param cmdId is the command id of the command to execute.
		\return \e true if successful, else \e false. */
	virtual bool OnCommand(UINT cmdId) = 0;

	/** Implement this method to update a menu item. If you recognize the command id, you
		should update the menu item's state and return \e true. If not, just return \e false.
		\return \e true if successful, else \e false. */
	virtual bool OnUpdateCommandUI(IRhRdkCommandUpdate& cu) const = 0;
};
