
#pragma once // Public SDK header

#include "RhRdkUuidCollection.h"

/** \class CRhRdkMarkedObjects

	This class provides a collection of objects that are marked in some way. The existence of the
	object's UUID in the collection makes the object 'marked'. This can be used for exclusion
	lists and so on.

*/
class RHRDK_SDK CRhRdkMarkedObjects : public CRhRdkUuidCollection
{
public:
	/** DEPRECATED: DO NOT USE. No document context. */
	CRhRdkMarkedObjects();

	/** Uses supplied Rhino doc. */
	CRhRdkMarkedObjects(CRhinoDoc& doc);

	virtual ~CRhRdkMarkedObjects();

	/** Edit the collection using a modal dialog.
		\param sIncludeListCaption is the caption used when bIsExcludeList is \e false.
		\param sExcludeListCaption is the caption used when bIsExcludeList is \e true.
		\param bIsExcludeListInOut is \e true if editing a list of excluded objects or \e false if
		 editing a list of included objects. After the user has finished editing the list, this
		 parameter is updated to reflect any change.
		\param bAllowChangeIncExc is \e true to allow the user to change from 'exclude' to 'include'.

		STARTING WITH RHINO 8, THIS METHOD IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

		\note This function requires the RDK UI plug-in to be loaded.
		\return \e true if the user clicked OK, else \e false. */
	virtual bool Edit(const ON_wString& sIncludeListCaption, const ON_wString& sExcludeListCaption,
	                  bool& bIsExcludeListInOut, bool bAllowChangeIncExc);

	/** Pick objects in the Rhino viewport.
		\param sPrompt is the prompt to display on the Rhino command line.
		\return \e true if the pick succeeded, else \e false. */
	virtual bool Pick(bool bPreserveSelection, const ON_wString& sPrompt);

	/** Set the object types for the Rhino object iterator used in Edit() and Pick(). By default,
		all types except ON::detail_object, ON::phantom_object and ON::clipplane_object are considered.
		Call this method to allow other kinds of objects to be considered for picking and display.
		\param uTypes is a combination of bit flags from the ON::object_type enum. */
	virtual void SetIteratorObjectTypes(UINT uTypes);

	/** Set the object state for the Rhino object iterator used in Edit() and Pick().
		The default is CRhinoObjectIterator::undeleted_objects. */
	virtual void SetObjectState(CRhinoObjectIterator::object_state s);

	/** Set the object category for the Rhino object iterator used in Edit() and Pick().
		The default is CRhinoObjectIterator::active_and_reference_objects. */
	virtual void SetObjectCategory(CRhinoObjectIterator::object_category c);

	/** Gets the document, if the object was constructed with one, or null if not. */
	CRhinoDoc* RhinoDoc(void) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
