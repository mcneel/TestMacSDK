
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkContent;
class CRhRdkContentArray;
class CRhRdkContentKindList;

/** \class IRhRdkContentList

	This interface is used for accessing lists of content. Any kind of content can coexist
	in such a list, although some actual lists in RDK allow only a single kind.
*/
class RHRDK_SDK IRhRdkContentList
{
public:
	virtual ~IRhRdkContentList() { }

	/** \return The number of items in the list. */
	virtual int Count(void) const = 0;

	/** \return The list's unique identifier. */
	virtual UUID Uuid(void) const = 0;

	/** Find a content by its instance id.
		\param uuidInstance is the instance id of the content to find.
		\param bRecurse specifies if children should be searched as well as top-level content.
		\return A pointer to the content or NULL if not found. */
	virtual const CRhRdkContent* FindInstance(const UUID& uuidInstance, bool bRecurse) const = 0;

	/** Find contents with the specified instance name.
		\param wszName is the name to search for.
		\param bCaseSensitive specifies if the name check is case-sensitive or not.
		\param bRecurse specifies if the content's children should be searched.
		\param aContent accepts all the contents that satisfy the search. */
	virtual void FindInstanceByName(const wchar_t* wszName, bool bCaseSensitive, bool bRecurse, OUT CRhRdkContentArray& aContent) const = 0;

	class Iterator
	{
	public:
		virtual ~Iterator() { }

		/** \return A pointer to the next content in the list or NULL if no more exist. */
		virtual const CRhRdkContent* Next(void) = 0;
	};

	/** Get an iterator to use when calling Next().
		The caller shall delete the iterator. */
	virtual Iterator* NewIterator(void) const = 0;

	/** \return A pointer to the next content in the list or NULL if no more exist. */
	virtual const CRhRdkContent* Next(Iterator* pIterator) const = 0;

	/** Get the content kinds that the list contains. */
	virtual void GetKinds(CRhRdkContentKindList& kindsOut) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
