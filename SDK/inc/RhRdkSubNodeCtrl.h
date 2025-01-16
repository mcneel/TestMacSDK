
#pragma once // Public SDK header

#if (defined ON_RUNTIME_WIN) && (defined RHINO_SDK_MFC)

#include "RhRdkContentCtrlBase.h"

/** \class CRhRdkSubNodeCtrl

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkSubNodeCtrl : public CRhRdkContentCtrlBase
{
public:
	CRhRdkSubNodeCtrl();
	virtual ~CRhRdkSubNodeCtrl();

	RDK_DEPRECATED virtual UUID ParentInstanceId(void) const;
	RDK_DEPRECATED virtual bool SetParentInstanceId(const UUID&);
	virtual bool ParentInstanceIdList(CRhRdkUuidCollection& ucOut) const;
	RDK_DEPRECATED virtual bool SetParentInstanceIdList(const CRhRdkUuidCollection& uc);
	virtual ON_wString ChildSlotName(void) const;
	virtual void SetChildSlotName(const ON_wString& sChildSlotName);
	virtual bool Varies(void) const;
	RDK_DEPRECATED static UINT ChangingMessageNumber(void);
	RDK_DEPRECATED static UINT ChangedMessageNumber(void);

protected:
	RDK_DEPRECATED afx_msg LRESULT OnChanging(WPARAM, LPARAM);
	RDK_DEPRECATED afx_msg LRESULT OnChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

protected:
	virtual IRhRdkContentCtrlBase* GetBaseInterface(void) const override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

#endif
