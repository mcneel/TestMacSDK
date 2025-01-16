
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"
#include "RhRdkUuidCollection.h"

class CRhRdkColor;
class IRhRdkColorPicker;

/** \class IRhRdkColorButton

	This interface allows RDK color buttons to be used without needing MFC.

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkColorButton : public IRhinoUiWithController
{
public:
	virtual bool UseAlpha(void) const = 0;
	virtual void SetUseAlpha(bool bUseAlpha) = 0;
	virtual CRhRdkColor GetColor(void) const = 0;
	virtual void SetColor(const CRhRdkColor& color, bool bNotify) = 0;
	virtual void SetColorEx(const CRhRdkColor& color, UINT uDelayMS) = 0;
	virtual void SetUndoString(const wchar_t* wszUndo) = 0;
	virtual void SetNewPicker(IRhRdkColorPicker* pPicker) = 0;
	virtual CRhRdkColor BackgroundColor(void) const = 0;
	virtual void SetBackgroundColor(const CRhRdkColor& color) = 0;
	virtual bool Varies(void) const = 0;
	virtual void SetVaries(bool bVaries) = 0;
	virtual void OnBeginPicker(void) = 0;
	virtual void OnEndPicker(void) = 0;
	virtual void OnFinalEndPicker(void) = 0;
	virtual void OnBeginDropper(void) = 0;
	virtual void OnEndDropper(void) = 0;
	virtual bool ShowColorWhenDisabled(void) const = 0;
	virtual void SetShowColorWhenDisabled(bool bShow) = 0;
	virtual const wchar_t* GetVariesText(void) const = 0;
	virtual void SetChildSlotName(const wchar_t* wszChildSlotName) = 0;

	/** Set a notification handler. RDK takes ownership of the object. */
	virtual void SetNotificationHandler(IRhRdkNotificationHandler* pNH) = 0;

	/** Used by SetClickAction(). */
	enum eClickAction { picker, dropper };

	virtual void SetClickAction(eClickAction action) = 0;

	/** \return a new instance of an implementation of this interface.
		\param f is the framework to use. The default, 'Auto', will automatically
		 choose the most suitable framework. */
	static IRhRdkColorButton* New(RhRdkUiFrameworks f);

protected:
	virtual ~IRhRdkColorButton() { }
};

#if defined ON_RUNTIME_WIN && defined RHINO_SDK_MFC

/** \class CRhRdkColorButton

	Provides a button which can be set to a color and supports color drag and drop.
	When clicked, the button displays a color picker. The default is the Rhino color picker
	but this can be changed by calling SetNewPicker(). The button sends a standard button notify
	message just before it displays the picker. You should not display your own color picker in
	the message handler. If you really need to display one then you should initially call SetNewPicker(NULL)
	to prevent the button from displaying the default color picker.

	\note This control is merely a window that hosts a color button in its client area.
	 The color button is itself an implementation of IRhRdkColorButton.

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkColorButton : public CWnd
{
public:
	CRhRdkColorButton();
	virtual ~CRhRdkColorButton();

	/** Set the controller. If a controller is not set, the color
		button will work as a basic color button, but it will not be
		able to do any child-slot (texture) functionality. */
	virtual void SetController(CRhRdkControllerPtr con);

	/** Create the window. */
	virtual bool CreateWnd(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParent, UINT nID);

	/** \return \e true if the button allows alpha in the color, else \e false. */
	virtual bool UseAlpha(void) const;

	/** Set whether or not the button allows alpha in the color. */
	virtual void SetUseAlpha(bool bUseAlpha);

	/** Get the color that the button is currently displaying. */
	virtual CRhRdkColor GetColor(void) const;

	/** Set the color that the button should display.
		\param color is the color to set.
		\param bNotify specifies whether or not a notification message should be sent to the parent. */
	virtual void SetColor(const CRhRdkColor& color, bool bNotify=true);

	/** Set the color that the button should display.
		\param color is the color to set.
		\param iDelayMS specifies if and when a notification message should be sent to the parent.
		 If the value is 0, no notification message is sent.
		 If the value is 1, the notification message is sent before the method returns.
		 If the value is greater than 1, it specifies the number of milliseconds to delay
		 after the method returns before sending the notification message to the parent. */
	virtual void SetColorEx(const CRhRdkColor& color, UINT uDelayMS=1);

	/** Set the undo string to be used for automatically handling undo.
		If you want to handle undo yourself, do not call this method.
		\param wszUndo is the string that will appear in Rhino's undo UI. */
	virtual void SetUndoString(const wchar_t* wszUndo);

	/** Set a new color picker. The button will subsequently display the new picker's
		user interface instead of the default one.
		\param pPicker is the new picker; the button will take ownership of this object. */
	virtual void SetNewPicker(IRhRdkColorPicker* pPicker);

	/** \return The button's background color. */
	virtual CRhRdkColor BackgroundColor(void) const;

	/** Set the button's background color. */
	virtual void SetBackgroundColor(const CRhRdkColor& color);

	/** \return \e true if the button is set to 'varies', else \e false. */
	virtual bool Varies(void) const;

	/** Set the button's 'varies' state. */
	virtual void SetVaries(bool bVaries);

	/** Called just before the color picker appears. */
	virtual void OnBeginPicker(void);

	/** Called just after the color picker has been dismissed but before the
		picked color has been set into the button (with optional undo). */
	virtual void OnEndPicker(void);

	/** Called after the color picker has been dismissed and the
		picked color has been set into the button (with optional undo). */
	virtual void OnFinalEndPicker(void);

	/** Called when the control enters dropper mode. */
	virtual void OnBeginDropper(void);

	/** Called when the control exits dropper mode. */
	virtual void OnEndDropper(void);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

	/** Set the action that occurs when the button is clicked. The default is to display the color picker.
		This also changes which context menu item is displayed in bold. */
	virtual void SetClickAction(IRhRdkColorButton::eClickAction action);

	/** \return \e true if the button always shows its color, even when disabled. */
	virtual bool ShowColorWhenDisabled(void) const;

	/** Set whether or not the button should always shows its color, even when disabled. */
	virtual void SetShowColorWhenDisabled(bool bShow);

	/** Set the child slot name which associates the button with a child slot in one or more parent contents. */
	virtual void SetChildSlotName(const wchar_t* wszCSN);

	/** Get the registered message number that will be sent when the button color is changed
		either by a call to SetColor() with bNotify=true, or when a color is dropped on the button.
		\return The message number to use in your ON_REGISTERED_MESSAGE handler. Example usage:
		\code
		const UINT uColorButtonChangedMsg = CRhRdkColorButton::ChangedMessageNumber();
		...
		BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
			ON_REGISTERED_MESSAGE(uColorButtonChangedMsg, OnColorChanged)
		END_MESSAGE_MAP()
		...
		LRESULT CMyDialog::OnColorChanged(WPARAM wParam, LPARAM lParam)
		{
			// wParam is the control id.
			CWnd* pWnd = GetDlgItem(int(wParam));
			...
			// lParam is a pointer to the control.
			CRhRdkColorButton* pButton = reinterpret_cast<CRhRdkColorButton*>(lParam);
			...
			return 1;
		}
		...
		\endcode */
	static UINT ChangedMessageNumber(void);

protected:
	/** Called just before the control is subclassed. */
	virtual void PreSubclassWindow() override;

	/** Override this to customize the text that the button displays when it is set
		to 'varies'. The default implementation returns L"(varies)". */
	virtual const wchar_t* GetVariesText(void) const;

	/** This override is no longer supported. */
	RDK_DEPRECATED virtual void BeginDrag(void);

	/** This override is no longer supported. */
	RDK_DEPRECATED virtual void DrawItem(LPDRAWITEMSTRUCT);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnDestroy();
	afx_msg BOOL32 OnEraseBkgnd(CDC*);
	afx_msg void OnEnable(BOOL32 bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnQueryFocus(WPARAM, LPARAM) { return 1; }
	afx_msg LRESULT OnChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	/** \internal For RDK internal use only. */
	void Initialize(void);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

// STARTING WITH RHINO 8, THIS FUNCTION IS DEPRECATED AND RETURNS NULL. See [DEPCTL]
RDK_DEPRECATED RHRDK_SDK IRhRdkColorButton* RhRdkGetActiveColorButton(void);

#endif
