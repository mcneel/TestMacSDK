
#pragma once // Public SDK header

#include "RhRdkObject.h"

/** \class CRhRdkAction

	This class is deprecated in favor of CRhRdkCustomTask.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkAction : public CRhRdkObject
{
public:
	CRhRdkAction(const UUID& uuidClient);
	virtual ~CRhRdkAction();
	virtual bool Enabled(void) const;
	virtual void SetEnabled(bool bEnable);
	virtual bool Checked(void) const;
	virtual void SetChecked(bool bCheck);
	virtual int Group(void) const { return 0; }
	virtual UUID Uuid(void) const = 0;
	virtual ON_wString Caption(void) const = 0;
	virtual ON_wString ToolTip(void) const = 0;
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const = 0;
	virtual bool Execute(void) = 0;
	virtual UUID Client(void) const;
	virtual WORD CommandId(void) const;
	virtual void SetActions(class CRhRdkActions*);

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class IRhRdkActions

	This class is deprecated in favor of CRhRdkTask

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkActions
{
public:
	virtual ~IRhRdkActions() { }
	virtual int Count(void) const = 0;
	virtual bool Add(CRhRdkAction* pAction) = 0;
	virtual CRhRdkAction* Action(int iIndex) const = 0;
	virtual CRhRdkAction* Find(const UUID& uuidAction) const = 0;
	virtual CRhRdkAction* Find(unsigned short cmd) const = 0;
	virtual void* EVF(const wchar_t*, void*) = 0;
};
