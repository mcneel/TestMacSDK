
#pragma once // Public SDK header - OBSOLETE - Not implemented.

// NOTE: These classes are obsolete. This file only exists for backward compatibility. These classes are not implemented.

#include "RhRdkObject.h"

class IRhRdkPostEffectImage;
class CRhRdkPostEffectPlugIn;

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkPostEffectPlugInManager : public CRhRdkObject
{
public:
	CRhRdkPostEffectPlugInManager();
	virtual ~CRhRdkPostEffectPlugInManager();

	/** Copy the manager. */
	const CRhRdkPostEffectPlugInManager& operator = (const CRhRdkPostEffectPlugInManager& srce);

	/** Initialize the manager.
		\param pImage is a pointer to a post-effect image that will be modified by the plug-ins.
		This pointer can be NUILL in which case RunSingleProcess() will have no effect. */
	virtual bool Initialize(IRhRdkPostEffectImage* pImage) const;

	/** \return A pointer to the post-effect image associated with this manager. */
	virtual IRhRdkPostEffectImage* Image(void) const;

	/** Reset the iterator for GetNextPlugIn(). */
	virtual void ResetPlugInIterator(void) const;

	/** Get the next plug-in.
		\param uuidOut accepts the UUID of the next plug-in.
		\param sNameOut accepts the name of the next plug-in.
		\return \e true if plug-in was retrieved or \e false if there are no more plug-ins. */
	virtual bool GetNextPlugIn(UUID& uuidOut, ON_wString& sNameOut) const;

	/** Find a plug-in by its UUID.
		\param uuidPlugIn is the UUID of the plug-in to find.
		\return A pointer to the plug-in or NULL if the specified plug-in does not exist. */
	virtual CRhRdkPostEffectPlugIn* PlugIn(const UUID& uuidPlugIn) const;

	/** Reset all post-effect plug-ins to their original factory defaults. */
	virtual void SetAllPlugInsToFactoryDefaults(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void SetAllPlugInsToDocumentDefaults(void) const;

	/** Reset all post-effect plug-ins to document defaults. */
	virtual void SetAllPlugInsToDocumentDefaults(const CRhinoDoc& doc) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void WriteAllPlugInsToDocumentDefaults(void) const;

	/** Copy all post-effect plug-in state to document defaults. */
	virtual void WriteAllPlugInsToDocumentDefaults(CRhinoDoc& doc) const;

	/** \return \e true if a plug-in works with the current image.
		\param uuidPlugIn is the UUID of the plug-in. */
	virtual bool PlugInWorksWithCurrentImage(const UUID& uuidPlugIn) const;

	/** Enable a plug-in.
		\param uuidPlugIn is the UUID of the plug-in.
		\return \e true if successful, \e false if the plug-in does not exist. */
	virtual bool EnablePlugIn(const UUID& uuidPlugIn, bool bEnable=true) const;

	/** Query if a plug-in is enabled.
		\param uuidPlugIn is the UUID of the plug-in.
		\return \e true if enabled, \e false if disabled or the plug-in does not exist. */
	virtual bool IsPlugInEnabled(const UUID& uuidPlugIn) const;

	/** Query if a plug-in can display a properties dialog.
		\param uuidPlugIn is the UUID of the plug-in.
		\return \e true if it can, \e false if it can't or the plug-in does not exist. */
	virtual_su bool CanDisplayOptions(const UUID& uuidPlugIn) const; // [SDK_UNFREEZE]

	/** Display plug-in options to the user.
		\param uuidPlugIn is the UUID of the plug-in.
		\return \e true if successful, \e false if the plug-in does not exist or user cancelled the dialog. */
	virtual bool DisplayOptions(const UUID& uuidPlugIn, HWND hParent) const;

	/** Process the image by calling RunSingleProcess() on each enabled plug-in in order,
		stopping if any one plug-in fails.
		\return \e true if all plug-ins were successful, \e false if a plug-in failed. */
	virtual bool ProcessImage(void) const;

	/** Process the image with a single plug-in.
		\param uuidPlugIn is the UUID of the plug-in.
		\return \e true if successful, \e false if plug-in failed. */
	virtual bool RunSingleProcess(const UUID& uuidPlugIn) const;

	/** Read a plug-in's state from a buffer.
		\param uuidPlugIn is the UUID of the plug-in.
		\param pBuffer is the buffer containing data previously written by WriteState().
		\param size is the number of bytes in the buffer.
		\return \e true if successful, else \e false. */
	virtual bool ReadState(const UUID& uuidPlugIn, const void* pBuffer, size_t size) const;

	/** Write a plug-in's state to a buffer.
		\param uuidPlugIn is the UUID of the plug-in.
		\param pBuffer is the buffer to fill with state information.
		\param size is the physical number of bytes that the buffer can accomodate.
		This parameter also accepts the logical number of bytes written to the buffer.
		This will be more than \b size if the buffer was not big enough.
		If pBuffer is NULL you can pass zero in \b size to get the number of bytes required
		to write the plug-in state.
		\return \e true if successful, else \e false. */
	virtual bool WriteState(const UUID& uuidPlugIn, void* pBuffer, size_t& size) const;

	/** Get the CRC of all plug-in state. */
	virtual DWORD StateCRC(void) const;

	/** Move a plug-in down in the list.
		\return \e true if the plug-in was moved.
		\param uuidPlugIn is the UUID of the plug-in to move. */
	virtual bool MoveDown(const UUID& uuidPlugIn) const;

	/** Move a plug-in up in the list.
		\return \e true if the plug-in was moved.
		\param uuidPlugIn is the UUID of the plug-in to move. */
	virtual bool MoveUp(const UUID& uuidPlugIn) const;

	/** \return \e true if a plug-in can be moved down in the list.
		\param uuidPlugIn is the UUID of the plug-in. */
	virtual bool MoveDownPossible(const UUID& uuidPlugIn) const;

	/** \return \e true if a plug-in can be moved up in the list.
		\param uuidPlugIn is the UUID of the plug-in. */
	virtual bool MoveUpPossible(const UUID& uuidPlugIn) const;

	/** \internal For RDK internal use only. */
	void SetAllowWireframeEffects(bool bAllow);
	bool AllowWireframeEffects(void) const;

protected:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;

public:
	/** \internal For RDK internal use only. */
	CImpl& GetImpl(void) const;
};
