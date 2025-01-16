
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkContent;
class CRhRdkContentFactory;

/** \class IRhRdkContentFactories

	This interface represents the collection of all content factories that are registered with the RDK.

	Content factories are RDK extensions. You usually add new content factories to the system when you
	are called on your implementation of CRhRdkPlugIn::RegisterExtensions(). If necessary, you can also
	add new factories later by calling RhRdkAddExtension(). Factories so added can be found again later
	by calling RhRdkContentFactories() which returns this interface.

	\see CRhRdkPlugIn::RegisterExtensions()
*/
class RHRDK_SDK IRhRdkContentFactories
{
public:
	virtual ~IRhRdkContentFactories() { }

	// If you are looking for the Add() method, please read the comments above.

	/** Create a new content from the specified content type.
		\note This method calls Initialize() on the new content. If this fails, the
		 new content is deleted and this method returns nullptr.
		\return A pointer to a new instance of the content that this factory produces. */
	/*ON_DEPRECATED*/ virtual CRhRdkContent* NewContentFromType(const UUID& uuidType) const = 0;

	/** Get a pointer to the first factory in the collection.
		\return A pointer to the first factory or nullptr if there are none. */
	virtual CRhRdkContentFactory* FirstFactory(void) const = 0;

	/** Get a pointer to the next factory in the collection.
		\return A pointer to the next factory or nullptr if no more. */
	virtual CRhRdkContentFactory* NextFactory(void) const = 0;

	/** Find the factory that creates content of the specified type.
		\return A pointer to the factory or nullptr if not found. */
	virtual CRhRdkContentFactory* FindFactory(const UUID& uuidType) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RHRDK_SDK IRhRdkContentFactoriesEx : public IRhRdkContentFactories
{
public:
	/** Create a new content from the specified content type.
	\note This method calls Initialize() on the new content. If this fails, the
	new content is deleted and this method returns nullptr.
	Use this function in preference to NewContentFromType(const UUID& uuidType)
	\return A pointer to a new instance of the content that this factory produces. */
	virtual CRhRdkContent* NewContentFromTypeEx(const UUID& uuidType, const CRhinoDoc* pDocAssoc) const = 0;
};
