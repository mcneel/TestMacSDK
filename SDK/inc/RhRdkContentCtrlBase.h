
#pragma once // Public SDK header

#if (defined ON_RUNTIME_WIN) && (defined RHINO_SDK_MFC)

#include "RhRdkDefinitions.h"

/** \class CRhRdkContentCtrlBase

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkContentCtrlBase : public CWnd
{
public:
	CRhRdkContentCtrlBase();
	virtual ~CRhRdkContentCtrlBase();

	CRhRdkControllerPtr Controller(void) const;
	void SetController(CRhRdkControllerPtr con);
	virtual bool CreateWnd(DWORD dwStyle, const RECT& rect, CWnd* pParent, UINT uID);
	virtual bool IsEmpty(void) const;
	virtual void ShowIcon(bool b);
	virtual CRhRdkVariant GetChannelMode(void) const;
	virtual CRhRdkVariant GetChannelCustom(void) const;
	virtual bool SetChannelMode(const CRhRdkVariant& vMode);
	virtual void SetChannelCustom(const CRhRdkVariant& vString);
	virtual void DisplayData(void);
	virtual void Change(void);
	virtual void OnChange(void) { }
	virtual bool ReadOnly(void) const;
	virtual void SetReadOnly(bool b);
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);
	virtual void ShowCheck(bool b);
	virtual UINT GetCheck(void) const;
	virtual void SetCheck(UINT u);
	virtual bool CheckEnabled(void) const;
	virtual void SetCheckEnabled(bool b);
	RDK_DEPRECATED static UINT CheckMessageNumber(void);
	virtual void OnCheckChanged(void) { }
	virtual void ShowAmount(bool b);
	virtual double GetAmount(void) const;
	virtual void SetAmount(double d);
	virtual void SetAmountBounds(double dMin, double dMax);
	virtual bool AmountVaries(void) const;
	virtual void SetAmountVaries(bool b=true);
	virtual bool AmountEnabled(void) const;
	virtual void SetAmountEnabled(bool b);
	RDK_DEPRECATED static UINT AmountMessageNumber(void);
	virtual void OnAmountChanged(void) { }

protected:
	virtual void PreSubclassWindow() override;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnable(BOOL32 bEnable);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	RDK_DEPRECATED afx_msg LRESULT OnCheck(WPARAM, LPARAM);
	RDK_DEPRECATED afx_msg LRESULT OnAmount(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	virtual void Initialize(void);
	virtual class IRhRdkContentCtrlBase* GetBaseInterface(void) const = 0;

private:
	class CImpl;
	CImpl* m_pBaseImpl;
};

#endif
