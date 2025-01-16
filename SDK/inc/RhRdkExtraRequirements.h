
#pragma once // Public SDK header

#include "IRhRdk_XMLSection.h"

class RHRDK_SDK CRhRdkExtraRequirements : public IRhRdk_XMLSection::IExtraRequirements
{
public:
	CRhRdkExtraRequirements();
	virtual ~CRhRdkExtraRequirements();

	virtual int Count(void) const;
	virtual ON_wString Name(int iIndex) const;
	virtual CRhRdkVariant Value(int iIndex) const;
	virtual bool FindValue(const wchar_t* wszName, CRhRdkVariant& vValueOut) const;
	virtual void AddItem(const ON_wString& sName, const CRhRdkVariant& vValue);
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
