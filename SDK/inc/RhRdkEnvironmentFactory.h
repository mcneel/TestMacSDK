
#pragma once // Public SDK header

#include "RhRdkContentFactory.h"

class RHRDK_SDK CRhRdkEnvironmentFactory : public CRhRdkContentFactory
{
public:
	CRhRdkEnvironmentFactory();
	virtual ~CRhRdkEnvironmentFactory();

	virtual CRhRdkContent::Kinds Kind(void) const;
	virtual CRhRdkContent* NewContent(void) const;

protected:
	virtual class CRhRdkEnvironment* NewEnvironment(void) const = 0;
};
