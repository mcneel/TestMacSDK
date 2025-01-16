
#pragma once // Public SDK header

/** \class IRhRdkContents

	This is the interface to document contents.

*/
class RHRDK_SDK IRhRdkContents
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkContents& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** Attach a content to the associated document.
		\param c is the content to attach.
		If successful, ownership of the content will be transferred to the document.
		\note If the content is a child or already attached to this or another document, the method will fail.
		\return \e true if successful, else \e false. */
	virtual bool Attach(CRhRdkContent& c) = 0;

	/** Detach a content from the document.
		\param c is the content to detach.
		 If successful, ownership of the content will be transferred to the caller.
		\note If the content is a child or not attached to this document, the method will fail.
		\return \e A pointer to the detached content if successful, else \e null. */
	virtual CRhRdkContent* Detach(const CRhRdkContent& c) = 0;

	/** \return \e true if content is filtered out according to document filter settings. */
	virtual bool IsFilteredOut(const CRhRdkContent& c) const = 0;

	/** Get a new content iterator. The caller shall delete the iterator after use.
		\param kinds specifies the content kinds to return.
		\param it_Flags specifies flags (see CRhRdkDocument::NewContentIterator). */
	virtual class IRhRdkContentIterator* NewIterator(const CRhRdkContentKindList& kinds, unsigned int it_Flags) const = 0;

	/** Replace a content in the document. Deprecated in favor of Replace(). */
	RDK_DEPRECATED virtual bool Change(const CRhRdkContent& oldContent, CRhRdkContent& newContent, bool bPreserve=true) = 0;

	/** Replace a content in the document.
		\param oldContent is the content to detach and delete.
		\param newContent is the content to attach in place of oldContent.
		 If successful, the old content will be deleted and ownership of the new content will
		 be transferred to the document.
		\param bPreserve determines whether or not the instance id and notes are preserved. If \e true,
		 the instance id of the old content is preserved in the new content. This means that any objects
		 or layers that were referencing the old content will reference the new content. The same applies to
		 the ground plane. The content's notes are also preserved. TODO: The name preserving is currently being reconsidered. [ANDY]
		\note The old content can be top-level or a child.
		\note If the old content is not attached to this document, the method will fail.
		\note If the new content is a child already attached to this or another document, the method will fail.
		\return \e true if successful, else \e false. */
	virtual_su bool Replace(const CRhRdkContent& oldContent, CRhRdkContent& newContent, bool bPreserve=true); // [SDK_UNFREEZE]

	/** Find a content by its instance id. All document-registered contents (including children) will be checked.
		\param uuidInstance is the instance id of the content to find.
		\return A const pointer to the content or null if not found. If you want to modify the content
		 you will need to call BeginChange() to get a non-const reference. \see BeginChange() */
	virtual const CRhRdkContent* Find(const UUID& uuidInstance) const = 0;

	/* [SDK_UNFREEZE] We need an extra param for this function.
		\param bCheckGlobal determines if global contents should also be checked. Global contents are non-document
		 contents that happen to be on the stack or the heap. If this is \e true, global contents will be checked if the
		 content was not found in the document. If \e false, the search is restricted to document contents only.
	virtual const CRhRdkContent* Find(const UUID& uuidInstance, bool bCheckGlobal=true) const = 0; */

	/** Searches for contents with a particular instance name.
		\param kinds specifies which top-level content kinds should be searched.
		\param wszName is the name to search for.
		\param bCaseSensitive specifies if the name check is case-sensitive or not.
		\param bRecurse specifies if children of top-level contents should be searched.
		\param aContent accepts all the contents that satisfy the search.
		 If you want to modify these contents you will need to call BeginChange() to
		 get a non-const reference. \see BeginChange() */
	virtual void Find(const CRhRdkContentKindList& kinds,
	                  const wchar_t* wszName, bool bCaseSensitive, bool bRecurse, OUT CRhRdkContentArray& aContent) const = 0;

	/** Searches for a content with the same name and appearance as another content.
		\note The name check is case-insensitive. The 'appearance' is the render CRC of the content.
		\param c is the content to match against.
		\return A pointer to a similar content if found, else null. */
	virtual const CRhRdkContent* FindSimilar(const CRhRdkContent& c) const = 0;

	/** Calls UpdateDocumentTables() on all of the contents. */
	virtual void UpdateDocumentTables(void) const = 0;

	/** Selects contents in all capable editors that are displaying those contents on behalf of the document.
		\param aInstance is the array of instance-ids of contents to select.
		\param bAppendToSelection specifies if the selection should be replaced (false) or modified (true).
		\return \e true if successful, else \e false. */
	virtual_su bool SelectContent(const CRhRdkInstanceIdArray& aInstance, bool bAppendToSelection) const; // = 0; [SDK_UNFREEZE] Implemented via EVF(L"SelectContent").

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkContents() { }
};
