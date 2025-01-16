
#pragma once // Public SDK header

#include "RhRdkContentFactory.h"

class RHRDK_SDK CRhRdkTextureFactory : public CRhRdkContentFactory
{
public:
	CRhRdkTextureFactory();
	virtual ~CRhRdkTextureFactory();

	virtual CRhRdkContent::Kinds Kind(void) const override;
	virtual CRhRdkContent* NewContent(void) const override;

	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

protected:
	virtual class CRhRdkTexture* NewTexture(void) const = 0;
};
