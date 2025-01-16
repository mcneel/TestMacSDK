
#pragma once

#if defined RHINO_SDK_MFC

/* The EDIT control on the dialog can have the following styles which are
   supported by this control:

   ES_AUTOHSCROLL -- Automatically scroll when too much text is entered.
   ES_RIGHT       -- Right-align text
   ES_CENTER      -- Center text.
   ES_NUMBER      -- Prevent input of non-numeric characters.

   \see SetForcedStyles().
*/
class RHRDK_SDK CRhRdkSdkNumberEdit : public CWnd
{
public:
	/** Call from subclass constructor to force window styles that are not specified in the dialog. */
	virtual void SetForcedStyles(DWORD dwStyles);

	/** Create the control dynamically.
		\return \e true if successful, else \e false. */
	virtual bool CreateCtrl(DWORD dwStyle, const ON_4iRect& rect, CWnd* pParentWnd, UINT iID);

	/** \return \e true if the inner edit box is focused, else \e false. */
	virtual bool IsFocused(void) const;

	/** \return \e true if display is blank, else \e false. */
	virtual bool IsBlank(void) const;

	/** Set or clear display as blank. Useful for 'varies'. */
	virtual void SetBlank(bool bBlank=true);

	/** \return spinner width in pixels. */
	virtual int SpinnerWidth(void) const;

	/** Set spinner width in pixels. */
	virtual void SetSpinnerWidth(int iWidth);

	/** Show or hide the spinner. */
	virtual void SetSpinnerVisibility(bool bVisible);

	/** \return \e true if using a fixed spinner delta, \e false if using automatic delta. */
	virtual bool IsFixedSpinnerDelta(void) const;

	/** Set fixed spinner delta. Pass zero to use automatic delta. */
	virtual void SetFixedSpinnerDelta(double dDelta);

	/** Removes fixed spinner delta. Same as SetFixedSpinnerDelta(0.0). */
	virtual void RemoveFixedSpinnerDelta(void);

	/** Set lower bound only. */
	virtual void SetLowerBound(double dLowerBound);

	/** Set upper bound only. */
	virtual void SetUpperBound(double dUpperBound);

	/** Set both bounds. */
	virtual void SetBounds(double dLowerBound, double dUpperBound);

	/** Remove bounds; sets control as boundless. */
	virtual void RemoveBounds(void);

	/** \return true if user can edit the number by typing. */
	virtual bool EditByTyping(void) const;

	/** Enable or disable editing the number by typing. If disabled, the user can't
		type into the control. The default is \e true. */
	virtual void SetEditByTyping(bool bCanType);

	/** \return \e true if using hand cursor when mouse is over a spinner. */
	virtual bool UseHandCursor(void) const;

	/** Enable or disable use of hand cursor when mouse is over a spinner. The default is \e false. */
	virtual void SetUseHandCursor(bool bUseHand=true);

	/** Enable or disable hiding of mouse cursor when dragging a spinner. */
	virtual void SetCanHideCursor(bool bCanHide=true);

	/** \return \e true if change notifications will be sent during spinner dragging. */
	virtual bool SendChangeWhenDragging(void) const;

	/** Enable or disable sending of change notifications during spinner dragging. */
	virtual void SetSendChangeWhenDragging(bool bSend=true);

	/** Set the scaling for the amount to change by when the user presses arrow or page keys. */
	virtual void SetKeyScaling(double dArrow=1.0, double dPage=10.0);

	/** Set the scaling to be applied to all deltas used for arrow keys, page keys or spinners
		when the shift key is down. */
	virtual void SetShiftKeyScaling(double d);

	/** Set the scaling to be applied to all deltas used for arrow keys, page keys or spinners
		when the control key is down. */
	virtual void SetControlKeyScaling(double d);

	/** Get the delay, in milliseconds, before auto-repeat begins when the mouse is held down on the spinner. */
	virtual int AutoRepeatDelay(void) const;

	/** Set the delay, in milliseconds, before auto-repeat begins when the mouse is held down on the spinner. */
	virtual void SetAutoRepeatDelay(int iDelay);

	/** Set suffix to use when not percentile. Suffix is displayed after the number. */
	virtual void SetSuffixString(const TCHAR* szSuffix);

	/** Set the selection in the edit box. */
	virtual void SetSel(int iStartChar, int iEndChar, bool bNoScroll=false);

	/* Get the character at the given position in the edit box. */
	virtual int CharFromPos(const CPoint& pt) const;

	/* Limit the number of characters allowed in the edit box. */
	virtual void SetLimitText(int iMax);

	/** \return \e true if simple arithmetic is allowed, else \e false.
		Simple arithmetic allows the displayed value to be modified by using +=, -=, *=, /= operators.
		For example, if the control shows 25 and the user enters +=2 <RETURN>, the control
		will show 27. If the user then enters -=18 <RETURN>, the display will show 9. */
	virtual bool SimpleArithmeticOn(void) const;

	/** Enable or disable simple arithmetic. */
	virtual void SetSimpleArithmeticOn(bool bOn=true);

	/** \return Amount of linear rolling. If zero, acceleration is used instead (default). */
	virtual double LinearRollingDelta(void) const;

	/** Set amount of linear rolling. If zero, acceleration is used instead (default). */
	virtual void SetLinearRollingDelta(double dDelta);

	/** For use with a button that locks edit controls together.
		\return \e true if edit control is considered locked to one or more others.
		\note This class does not implement any actual locking, just the m_dLockedValue variable. */
	virtual bool IsLocked(void) const;

	/** For use with a button that locks edit controls together. Call this when the lock button state changes. */
	virtual bool SetLock(bool bLocked);

	/** Lock this control's value from another one.
		\return \e true if locking was performed, else \e false. */
	virtual bool LockFrom(const CRhRdkSdkNumberEdit& srce);

	/** Override this to specify if the spinner should be themed.
		The default implementation returns \e true. */
	virtual bool WantThemedSpinner(void) const;

	/** Get the threshold used for detecting a change. */
	virtual double ChangeThreshold(void) const;

	/** Set the threshold used for detecting a change. Default is 1e-6. */
	virtual void SetChangeThreshold(double dThresh);

	/** \return \e true if spinners are enabled when blank, else \e false. */
	virtual bool EnableSpinnerWhenBlank(void) const;

	/** Set or clear spinners enabled when blank. The default is \e false. */
	virtual void SetEnableSpinnerWhenBlank(bool bEnable=true);

public: // Notifications.

	/** Called after the user has changed the value in the edit box and focus has been lost.
		The default implementation sends the registered message ChangedMessageNumber(). */
	virtual void OnUserChangedValue(void);

	/** Called after the value in the edit box has updated for any reason. This is always called
		and cannot be disabled. It is designed for updating simple UI items immediately.
		If you override this method, do not use it for anything time-consuming. */
	virtual void OnValueChanged(void);

	/** Called when the user starts dragging a spinner or holds down the left mouse button
		so that the spinner starts to auto-repeat. */
	virtual void OnBeginRollSpinner(void);

	/** Called when the left mouse button is released if OnBeginDragSpinner() has been called. */
	virtual void OnEndRollSpinner(void);

	/** Override this to return a special cursor to use when dragging the spinners. */
	virtual HCURSOR SpinDragCursor(void) const;

	/** Get the registered message number that will be sent when the user changes the value in the edit box.
		\return The message number to use in your ON_REGISTERED_MESSAGE handler. Example usage:
		\code
		const UINT uEditChangedMsg = CRhRdkSdkNumberEdit::ChangedMessageNumber();
		...
		BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
			ON_REGISTERED_MESSAGE(uEditChangedMsg, OnEditChanged)
		END_MESSAGE_MAP()
		...
		LRESULT CMyDialog::OnEditChanged(WPARAM wParam, LPARAM lParam)
		{
			// wParam is the control id.
			CWnd* pWnd = GetDlgItem(wParam);
			...
			// lParam is a pointer to the control.
			CRhRdkSdkNumberEdit* pEdit = reinterpret_cast<CRhRdkSdkNumberEdit*>(lParam);
			...
			return 1;
		}
		...
		\endcode */
	static UINT ChangedMessageNumber(void);

public: // Advanced overrides.

	/** Override this to change the formatting procedure and return \e true.
		If this returns \e false then default formatting used. */
	virtual bool Format(const double dValue, CString& sText) const;

	/** Override this to parse the input string into a double and return \e true.
		If this returns \e false then default parsing is used. */
	virtual bool Parse(const TCHAR* tsz, double& dValueOut) const;

	/** This disables the 'Buggy UI' check -- for UIs that NEED to set the value from the change notification. */
	virtual void SetAllowSetDuringChange(bool bAllow=true);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

protected:
	virtual void PreSubclassWindow();

protected:
	afx_msg void OnKillfocusEdit();
	DECLARE_MESSAGE_MAP()

protected:
	class CImpl;
	virtual CImpl& Impl(void) = 0;
	virtual const CImpl& Impl(void) const = 0;
};

class RHRDK_SDK CRhRdkSdkRealNumberEdit : public CRhRdkSdkNumberEdit
{
public:
	CRhRdkSdkRealNumberEdit();
	virtual ~CRhRdkSdkRealNumberEdit();

	/** Initialize various properties. */
	virtual void Initialize(double dLowerBound=0.0, double dUpperBound=Unbounded(), double dDelta = 1.0, int iPlaces = 1,
	                        const TCHAR* tszSuffix=_T(""), bool bPercentile = false);

	/** Get the value. */
	virtual double GetValue(void) const;

	/** Set the value. */
	virtual bool SetValue(double dValue, bool bForceBlankOff=false);

	/** \return Number of decimal places being displayed (or -1 if not used). */
	virtual int DecimalPlaces(void) const;

	/** Set number of decimal places to display. -1 means not used (i.e., uses %g formatting). */
	virtual void SetDecimalPlaces(int iPlaces);

	/** \return \e true if control is set as percentile. */
	virtual bool IsPercentile(void) const;

	/** Set value as percentile. Value will be displayed times 100 with % suffix.
		e.g., a value of 0.5 will be displayed as 50% */
	virtual void SetAsPercentile(bool bPercentile=true);

	/** \return \e true if automatic percentile scaling is enabled. */
	virtual bool AutoPercentileScaling(void) const;

	/** Enable or disable automatic percentile scaling. If enabled, GetValue() returns
		an integer value from 0 to 100 and SetValue() accepts such a value. If disabled the
		functions use values between 0.0 to 1.0 when the control is set as percentile. */
	virtual void SetAutoPercentileScaling(bool bAuto=true);

	/** \return Whether or not trailing zeroes will be stripped after the decimal point. */
	virtual bool StripTrailingZeros(void) const;

	/** Set whether or not trailing zeroes will be stripped after the decimal point. */
	virtual void SetStripTrailingZeros(bool bStrip=true);

	/** \return Whether or not Format() will clamp near-zero values to zero. */
	virtual bool AlwaysClampToZero(void) const;

	/** Set whether or not Format() will clamp near-zero values to zero. */
	virtual void SetAlwaysClampToZero(bool bClamp=true);

	/** \return Whether or not super-smart rolling is active. */
	virtual bool SuperSmartRolling(void) const;

	/** Set whether or not super-smart rolling is active. This feature maintains a narrow field regardless
		of the decimal places setting. It also does smart acceleration with 'mouse-jerk' engaging 'fast-mode'
		when the number edit is unbounded.
		\note Calling SetLinearRollingDelta() overrides super-smart rolling and does linear rolling
		regardless of the setting of super-smart rolling. */
	virtual void SetSuperSmartRolling(bool bSuperSmart=true);

	/** This value can be passed to Set*Bound(s) to make the control boundless. */
	static double Unbounded(void);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL32 b);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual CImpl& Impl(void);
	virtual const CImpl& Impl(void) const;

protected:
	class CImpl;
	mutable CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkSdkIntegerNumberEdit : public CRhRdkSdkNumberEdit
{
public:
	CRhRdkSdkIntegerNumberEdit();
	virtual ~CRhRdkSdkIntegerNumberEdit();

	/** Initialize various properties. */
	virtual void Initialize(int iLowerBound, int iUpperBound, int iDelta, const TCHAR* tszSuffix=_T(""));

	/** Get the value. */
	virtual int GetValue(void) const;

	/** Set the value. */
	virtual bool SetValue(int iValue, bool bForceBlankOff=false);

	/** This value can be passed to Set*Bound(s) to make the control boundless. */
	static long Unbounded(void);

public: // Advanced overrides.

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL32 b);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	virtual CImpl& Impl(void);
	virtual const CImpl& Impl(void) const;

protected:
	class CImpl;
	mutable CImpl* m_pImpl;
};

#endif
