
#pragma once // Public SDK header

// This file is in the public SDK by mistake and should not be used by 3rd party plug-ins. [SDK_UNFREEZE]

#include "RhRdkDefinitions.h"

class CRhRdkContent;
class CRhRdkContentArray;
class CRhRdkDocument;

class RHRDK_SDK CRhRdkContentCreator : public CRhRdkObject
{
public:
	CRhRdkContentCreator(const CRhRdkDocument& rdkDoc);
	virtual ~CRhRdkContentCreator();

	const wchar_t* ErrorString(void) const;

	bool AddContent(OUT CRhRdkContentArray& aContent, const CRhRdkContent* pParentContent=nullptr, const wchar_t* wszChildSlotName=nullptr) const;

	const CRhRdkContent* AddContent(const CRhRdkContent* pParentContent=nullptr, const wchar_t* wszChildSlotName=nullptr) const;
	const CRhRdkContent* ChangeContentType(const CRhRdkContent*& pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName, bool bPreserveOldChild) const;
	const CRhRdkContent* FindChildAddOrChange(const CRhRdkContent& parentContent, const wchar_t* wszChildSlotName, bool bPreserveOldChild) const;
	const CRhRdkContent* ChangeContentTypeEx(const CRhRdkContent*& pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName, bool bPreserveOldChild, unsigned int ic_flags) const;

protected:
	virtual bool IsCopy(void) const = 0;
	virtual const CRhRdkContentArray* OriginalContent(void) const = 0;
	virtual bool CreateNewContent(CRhRdkContentArrayNC& aContentOut, const CRhRdkContent* pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName) const = 0;

public:
	virtual_su bool AutoEditChild(void) const; // = 0; [SDK_UNFREEZE]

protected:
	const CRhRdkDocument* RdkDocument(void) const;
	void SetErrorString(const wchar_t* wsz) const;

	virtual void* EVF(const wchar_t*, void*) override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkContentCreatorLoad : public CRhRdkContentCreator
{
public:
	CRhRdkContentCreatorLoad(const CRhRdkDocument& rdkDoc, CRhRdkContent::Kinds topLevelKind);
	virtual ~CRhRdkContentCreatorLoad();

	void SetFilenameOverride(const wchar_t* wsz);

protected:
	virtual bool IsCopy(void) const override { return false; } // [SDK_UNFREEZE] INLINE!
	virtual const CRhRdkContentArray* OriginalContent(void) const override { return nullptr; } // [SDK_UNFREEZE] INLINE!
	virtual bool CreateNewContent(CRhRdkContentArrayNC& aContentOut, const CRhRdkContent* pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName) const override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkContentCreatorNew : public CRhRdkContentCreator
{
public:
	CRhRdkContentCreatorNew(const CRhRdkDocument& rdkDoc, const ON_SimpleArray<UUID>& aUuidNewType, bool bHarvest);
	CRhRdkContentCreatorNew(const CRhRdkDocument& rdkDoc, const UUID& uuidNewType, bool bHarvest);
	virtual ~CRhRdkContentCreatorNew();

protected:
	virtual bool IsCopy(void) const override { return false; }
	virtual const CRhRdkContentArray* OriginalContent(void) const override { return nullptr; }
	virtual bool CreateNewContent(CRhRdkContentArrayNC& aContentOut, const CRhRdkContent* pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName) const override;
	virtual bool SetUpDefaults(CRhRdkContent* pContent, const CRhRdkContent* pOldContent) const;

	bool Harvest(void) const;
	void AppendType(const UUID uuidType);
	void AppendCreator(const class CCreator& c);

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkContentCreatorNewFileTexture : public CRhRdkContentCreatorNew
{
public:
	enum : unsigned int // Flags for uAcceptable
	{
		kAcceptBitmap       = 0x0001,
		kAcceptSimpleBitmap = 0x0002,
		kAcceptHDR          = 0x0004,
		kAcceptAll          = 0xFFFF
	};

	CRhRdkContentCreatorNewFileTexture(const CRhRdkDocument& rdkDoc, const wchar_t* wszFilename, unsigned int uAcceptable=kAcceptAll);
	virtual ~CRhRdkContentCreatorNewFileTexture();

	virtual bool SetUpDefaults(CRhRdkContent* pContent, const CRhRdkContent* pOldContent) const override;

protected:
	UUID TypeIdFromFilename(const wchar_t* wszFilename, unsigned int uAcceptable) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkContentCreatorExisting : public CRhRdkContentCreator
{
public:
	CRhRdkContentCreatorExisting(const CRhRdkDocument& rdkDoc, const CRhRdkContentArray& aContent);
	virtual ~CRhRdkContentCreatorExisting();

protected:
	virtual bool IsCopy(void) const override;
	virtual const CRhRdkContentArray* OriginalContent(void) const override;
	virtual void* EVF(const wchar_t*, void*) override;

protected:
	const CRhRdkContentArray& Content(void) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkContentCreatorExistingCopy : public CRhRdkContentCreatorExisting
{
public:
	CRhRdkContentCreatorExistingCopy(const CRhRdkDocument& rdkDoc, const CRhRdkContentArray& aContent);

protected:
	virtual bool CreateNewContent(CRhRdkContentArrayNC& aContentOut, const CRhRdkContent* pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName) const override;
};

class RHRDK_SDK CRhRdkContentCreatorExistingInstance : public CRhRdkContentCreatorExisting
{
public:
	CRhRdkContentCreatorExistingInstance(const CRhRdkDocument& rdkDoc, const CRhRdkContentArray& aContent);

protected:
	virtual bool CreateNewContent(CRhRdkContentArrayNC& aContentOut, const CRhRdkContent* pOldContent, const CRhRdkContent* pParentContent, const wchar_t* wszChildSlotName) const override;
};
