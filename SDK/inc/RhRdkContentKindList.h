
#pragma once // Public SDK header

#include "RhRdkObject.h"

/** \class CRhRdkContentKindList

	This class is a collection of content kind designations.

	Supported kinds are listed in "RhRdkContentKinds.h"

*/
class RHRDK_SDK CRhRdkContentKindList : public CRhRdkObject
{
public:
	/** Construct with no kinds. */
	CRhRdkContentKindList();

	/** Construct from a single content kind. */
	CRhRdkContentKindList(CRhRdkContent::Kinds kind);

	/** Copy constructor. */
	CRhRdkContentKindList(const CRhRdkContentKindList&);

	virtual ~CRhRdkContentKindList();

	CRhRdkContent::Kinds operator [] (int index) const;

	const CRhRdkContentKindList& operator = (const CRhRdkContentKindList& other);

	/** Add all kinds to the list. */
	virtual void AddAllKinds(void);

	/** Add a kind to the list. */
	virtual void Add(CRhRdkContent::Kinds kind);

	/** \return The number of kinds in the list. */
	virtual int Count(void) const;

	/** Remove a kind from the list. */
	virtual void Remove(CRhRdkContent::Kinds kind);

	/** \return the single kind in the list. If the list does not contain exactly one kind, returns 'Unset'. */
	virtual CRhRdkContent::Kinds SingleKind(void) const;

	/** Query whether or not the collection contains a particular kind designation.
		\return \e true if the kind is in the collection, else \e false. This logic
		can be reversed by the SetInverse() method. */
	virtual bool Contains(CRhRdkContent::Kinds kind) const;

	/** Set from a semicolon-delimited string of kinds, e.g., L"material;texture".
		\return \e true if successful, \e false if the string is invalid. */
	virtual bool SetFromSemicolonDelimitedString(const wchar_t* wsz);

	/** Query whether or not a content kind is supported by RDK.
		\return \e true if kind is supported, else \e false. */
	static bool IsSupportedKind(CRhRdkContent::Kinds kind);

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkMaterialKindList

	This class makes it easy to get a content kind list for just materials.

*/
class RHRDK_SDK CRhRdkMaterialKindList : public CRhRdkContentKindList
{
public:
	CRhRdkMaterialKindList();
};

/** \class CRhRdkEnvironmentKindList

	This class makes it easy to get a content kind list for just environments.

*/
class RHRDK_SDK CRhRdkEnvironmentKindList : public CRhRdkContentKindList
{
public:
	CRhRdkEnvironmentKindList();
};

/** \class CRhRdkTextureKindList

	This class makes it easy to get a content kind list for just textures.

*/
class RHRDK_SDK CRhRdkTextureKindList : public CRhRdkContentKindList
{
public:
	CRhRdkTextureKindList();
};
