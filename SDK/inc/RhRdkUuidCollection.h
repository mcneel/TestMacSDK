
#pragma once // Public SDK header

#include "RhRdkObject.h"

/** \class CRhRdkUuidCollection

	This class provides a collection of UUIDs. It can be used for collecting Rhino objects, Render Contents
	or anything else that is identified by a UUID.

*/
class RHRDK_SDK CRhRdkUuidCollection : public CRhRdkObject
{
public:
	CRhRdkUuidCollection();
	CRhRdkUuidCollection(const CRhRdkUuidCollection& c);
	virtual ~CRhRdkUuidCollection();

	/** Copy the collection. */
	const CRhRdkUuidCollection& operator = (const CRhRdkUuidCollection& c);

	/** Clear the collection. */
	virtual void Clear(void);

	/** \return The number of items in the collection. */
	virtual int Count(void) const;

	/** Add a UUID to the collection. */
	virtual void Add(const UUID& uuid);

	/** Remove a UUID from the collection. */
	virtual void Remove(const UUID& uuid);

	/** Reset the iterator for use with Next(). */
	virtual void ResetIterator(void);

	/** Get the next UUID from the collection. \note The UUIDs are returned in a random order,
		not in the order they were added. \note The const on this method is incorrect.
		\return \e true if a UUID was retrieved, \e false if there are no more uuids. */
	virtual bool Next(UUID& uuidOut) const;

	/** Get the collection as a semicolon-delimited string of UUIDs. */
	virtual ON_wString GetSemicolonDelimitedString(void) const;

	/** Build the collection from a semicolon-delimited string of UUIDs.
		\return \e true if successful, \e false if string is invalid. */
	virtual bool SetSemicolonDelimitedString(const ON_wString& sUuidList);

	/** \return \e true if the UUID is present in the collection, else \e false. */
	virtual bool Contains(const UUID& uuid) const;

	/** \return \e true if the collection is empty, else \e false. */
	virtual bool IsEmpty(void) const;

	/** \internal For RDK internal use only. */
	virtual void DebugDump(void);

private:
	class CImpl;
	CImpl* m_pImpl;
};
