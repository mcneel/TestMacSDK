
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkContent;

class RHRDK_SDK IRhRdkContentUndo
{
public:
	/** This method is implemented as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** Call this after creating a new content but before attaching it to a document.
		\param new_content is the new content being attached.
		\param parent is a pointer to the content that will become the parent of the new content,
		 or null if the new content is being attached at the top level (i.e., not a child).
		\return \e true if successful, else \e false. */
	virtual bool AddContent(const CRhRdkContent& new_content, const CRhRdkContent* parent) = 0;

	/** Call this before modifying a content in a significant way, for example changing a child.
		\note If you are just changing a simple parameter such as color, you should call TweakContent() instead.
		\param c is the content being modified. */
	virtual bool ModifyContent(const CRhRdkContent& c) = 0;

	/** Call this before tweaking a single content parameter. It is more efficient than calling ModifyContent()
		because it only saves and restores the single changed parameter instead of the whole content hierarchy.
		\param c is the content being tweaked.
		\param wszParamName is the name of the parameter being tweaked.
		\param vValueOld is the value of the parameter before the tweak is applied. */
	virtual bool TweakContent(const CRhRdkContent& c, const wchar_t* wszParamName, const class CRhRdkVariant& vValueOld) = 0;

	/** Call this before 'deleting' one or more top-level contents. This requires the caller to
		merely detach the contents from the document and pass ownership of them to this object.
		It eliminates the need to copy the contents and the need to actually delete them, thus
		saving time and memory. Because the contents are not actually deleted at all, the user
		still perceives them as deleted because they disappear from the UI when detached.
		\param aContent is an array of pointers to the contents being 'deleted'.
		 If any one of these contents is a child, the function will fail. */
	virtual bool DeleteTopLevelContent(const CRhRdkContentArrayNC& aContent) = 0;

	/** Call this before deleting one or more contents. This copies the content hierarchies
		into the undo object and is therefore somewhat inefficient. If all the contents are
		top-level, it is better to call DeleteTopLevelContent() instead.
		\param aContent is an array of pointers to the contents being deleted. */
	virtual bool DeleteContent(const CRhRdkContentArray& aContent) = 0;

	/** Call this before promoting a single content from reference to non-reference.
		\param c is the content being promoted. */
	/*virtual bool PromoteContent(const CRhRdkContent& c) = 0;*/ // [SDK_UNFREEZE]

	/** Emergency virtual function for future expansion.
		"PromoteContent" is supported. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkContentUndo() { }
};

/** \class CRhRdkContentUndo

	This is a stack object that makes it easy for clients to implement content undo.
	If you use the CRhRdkContentField object for storing your content's variables, you may not
	need to use this class since the field system takes care of parameter undo automatically.
	Use this class for handling parameter undo for data that is not stored in fields, or for
	handling custom content adds and deletes.

	\note This class can be instantiated more than once (in nested or recursive functions) and
	 will still work as if only one was instantiated. This removes the need to worry about an
	 instance already existing at a level above.

	\note For this to work outside of a command, you will need to start an undo record in the
	 relevant document.

	\see CRhRdkContentField
	\see CRhRdkUndoableEvent

*/
class RHRDK_SDK CRhRdkContentUndo : public IRhRdkContentUndo
{
public:
	/** This constructor is deprecated in favor of the one below. */
	RDK_DEPRECATED CRhRdkContentUndo(const wchar_t*, INT_PTR reserved=0);

	/** Use this constructor if you have a reference to a Rhino document.
		\note You must make sure an undo record is active in the given document
		 at the time this object is created or it will have no effect.
		\see CRhRdkUndoableEvent */
	CRhRdkContentUndo(const CRhinoDoc& doc);

	/** Use this constructor if you have a pointer to an RDK document.
		The pointer can be null. Undo will only be recorded if the RDK
		document is not null and is associated with a Rhino document.
		\note You must make sure an undo record is active in the given document
		 at the time this object is created or it will have no effect.
		\see CRhRdkUndoableEvent */
	CRhRdkContentUndo(const class CRhRdkDocument* pRdkDoc);

	virtual ~CRhRdkContentUndo();

	/** Call this after creating a new content but before attaching it to the document.
		\param new_content is the new content being attached.
		\param parent is a pointer to the content that will become the parent of the new content,
		 or null if the new content is being attached at the top level (i.e., not a child).
		\return \e true if successful, else \e false. */
	virtual bool AddContent(const CRhRdkContent& new_content, const CRhRdkContent* parent) override;

	/**	This method is obsolete because it does not support redo. It has only been retained
		for backward compatibility. Please call the other AddContent() method instead. */
	RDK_DEPRECATED virtual bool AddContent(const CRhRdkContent&);

	/** Call this before modifying a content in a significant way, for example changing a child.
		\note If you are just changing a simple parameter such as color, you should call TweakContent() instead.
		\param content is the content being modified. */
	virtual bool ModifyContent(const CRhRdkContent& c) override;

	/** Call this before tweaking a single content parameter. It is more efficient than calling ModifyContent()
		because it only saves and restores the single changed parameter instead of the whole content hierarchy.
		\param content is the content being tweaked.
		\param wszParamName is the name of the parameter being tweaked.
		\param vValueOld is the value of the parameter before the tweak is applied. */
	virtual bool TweakContent(const CRhRdkContent& c, const wchar_t* param_name, const class CRhRdkVariant& value_old) override;

	/** Call this before 'deleting' one or more top-level contents. This requires the caller to
		merely detach the contents from the document and pass ownership of them to this object.
		It eliminates the need to copy the contents and the need to actually delete them, thus
		saving time and memory. Although the contents are not actually deleted, the user
		still perceives them as deleted because they disappear from the UI when detached.
		\param aContent is an array of pointers to the contents being 'deleted'.
		 If any one of these contents is a child, the function will fail. */
	virtual bool DeleteTopLevelContent(const CRhRdkContentArrayNC& contents) override;

	/** Call this before deleting one or more contents. This copies the content hierarchies
		into the undo object and is therefore somewhat inefficient. If all the contents are
		top-level, it is better to call DeleteTopLevelContent() instead.
		\param aContent is an array of pointers to the contents being deleted. */
	virtual bool DeleteContent(const CRhRdkContentArray& contents) override;

	/** Call this before promoting a single content from reference to non-reference.
		\param content is the content being promoted. */
	virtual_su bool PromoteContent(const CRhRdkContent& c) /*override*/; // [SDK_UNFREEZE]

	virtual void DeleteThis(void) override { delete this; }

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) override;

	/** \return \e true if the given content has already been added to this content undo. */
	bool HasContent(const CRhRdkContent& c) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkContentUndoBlocker

	This is a stack object that prevents content undo records from being created. Sometimes it is
	necessary to change the state of a content without creating an undo record. This can be achieved
	by putting one of these on the stack just before making the change.

	\see CRhRdkContentUndo
*/

class RHRDK_SDK CRhRdkContentUndoBlocker final
{
public:
	CRhRdkContentUndoBlocker();
	~CRhRdkContentUndoBlocker();

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
