
#pragma once // Public SDK header

#include "RhRdkContentFactory.h"

class RHRDK_SDK CRhRdkMaterialFactory : public CRhRdkContentFactory
{
public:
	CRhRdkMaterialFactory();
	virtual ~CRhRdkMaterialFactory();

	virtual CRhRdkContent::Kinds Kind(void) const;
	virtual CRhRdkContent* NewContent(void) const;

protected:
	virtual class CRhRdkMaterial* NewMaterial(void) const = 0;
};
