
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkContent;
class IRhRdkPreviewSceneServer;

/** \class CRhRdkContentIOPlugIn

	This class allows a plug-in to provide custom file formats for loading and saving content.

	Please use CRhRdkContentIOPlugInEx instead of CRhRdkContentIOPlugIn.

*/
class RHRDK_SDK CRhRdkContentIOPlugIn : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const override;

	/** \return The unique id of the client's RDK plug-in. */
	virtual UUID RdkPlugInId(void) const = 0;

	/** \return The file extension used by the plug-in (e.g., "jpg") */
	virtual const wchar_t* FileExtension(void) const = 0;

	/** \return The description of the file format (e.g., "JPEG Files" */
	virtual const wchar_t* Description(void) const = 0;

	/** \return \e true if the plug-in is capable of loading content. */
	virtual bool CanLoad(void) const = 0;

	/** \return \e true if the plug-in is capable of saving content. */
	virtual bool CanSave(void) const = 0;

	/** Create a new content and load it from a custom content file.
		\param pDoc is a pointer to a Rhino document. May be nullptr.
		\param wszFullPath is the filename to load from.
		\param kind is only used by I/O plug-ins that support multiple kinds.
		 It tells the plug-in which content kind to create. If the plug-in only
		 supports a single content kind, it can ignore this parameter.
		\note The content is not actually attached to any document, it is just loaded.
		 The pDoc parameter is only used for unpacking embedded files, if necessary.
		\note This method is deprecated in favor of CRhRdkContentIOPlugInEx::LoadMultiple().
		\return A pointer to the new content if loaded successfully, else nullptr. */
	/*RDK_DEPRECATED*/ virtual CRhRdkContent* Load(const CRhinoDoc* pDoc, const wchar_t* wszFullPath, CRhRdkContent::Kinds kind) const = 0;

	/** Save an existing content to a custom content file.
		\param wszFullPath is the full paths to the filename to save to.
		\param content is the content to save.
		\param pSceneServer is a pointer to a scene server that can be used to render a preview image.
		 for storing in the file. \note This parameter is nullptr when \e content is a texture.
		\return \e true if saved successfully, else \e false. */
	virtual bool Save(const wchar_t* wszFullPath, const CRhRdkContent& content, const IRhRdkPreviewSceneServer* pSceneServer) const = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual const wchar_t* Kind(void) const { return L""; }

	/** \return The kind(s) of content that this plug-in can load/save. */
	virtual CRhRdkContentKindList SupportedKinds(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) override { return nullptr; } // [SDK_UNFREEZE] Should not be inline.
};

/** \class CRhRdkContentIOPlugInEx

	This class is an extension of CRhRdkContentIOPlugIn. It adds support for loading multiple contents from a file.

*/
class RHRDK_SDK CRhRdkContentIOPlugInEx : public CRhRdkContentIOPlugIn
{
public:
	CRhRdkContentIOPlugInEx();
	virtual ~CRhRdkContentIOPlugInEx();

	virtual UUID InterfaceId(void) const override final;

	/** \return \e true if the plug-in is capable of loading multiple contents. */
	virtual bool CanLoadMultiple(void) const = 0;

	enum : unsigned int // Flags for LoadMultiple().
	{
		LoadMultiple_Normal  = 0x00,
		LoadMultiple_Preload = 0x01,
	};

	/** Create any number of new contents and load them from a custom content file.
		\param pDoc is a pointer to a Rhino document. May be nullptr.
		\param aFullPath is a list of full paths of files to load from. Each file can contain any number of contents.
		\param kind is only used by I/O plug-ins that support multiple kinds. It tells the plug-in which
		 content kind to create. If the plug-in only supports a single content kind, it can ignore this parameter.
		\param flags is a set of flags from the enum above.
		\note The contents are not actually attached to any document, they are just loaded.
		 The pDoc parameter is only used for unpacking embedded files, if necessary.
		\note After loading each content from a file, you should call ReportContentAndFile() to report the content and the
		 file it was loaded from. However, during file drag and drop, the LoadMultiple_Preload flag is specified, and if your
		 loading process is time-consuming or displays a dialog, then for each file, you must create a 'deferred' content instead
		 of loading any real contents. A deferred content is a lightweight content of the same kind which is fast to create. It
		 represents all the contents that will eventually be loaded from the file. This is needed because during drag and drop,
		 the contents are created when the drag enters a window. On dropping, any deferred contents will be properly loaded by
		 calling LoadMultiple() again for each individual file extension that was dragged. In this case, instead of calling
		 ReportContentAndFile() you must call ReportDeferredContentAndFile().
		\return \e true if all contents loaded successfully, \e false if any one failed. */
	virtual bool LoadMultiple(const CRhinoDoc* pDoc, const ON_ClassArray<ON_wString>& aFullPath, CRhRdkContent::Kinds kind, unsigned int flags, void* pReserved) const = 0;

	/** This is called from your implementation of LoadMultiple() to add a content and the file it was loaded from
		when the LoadMultiple_Preload flag is \e not set.
		\see CRhRdkContentIOPlugInEx::LoadMultiple() for an explanation of this method's use.
		\param c is the content that was loaded.
		\param wszFullPath is the full path to the file that 'c' was loaded from.
		\param flags is reserved for future use; you should pass zero.
		\param pReserved is reserved for future use; you should pass nullptr. */
	virtual void ReportContentAndFile(CRhRdkContent& c, const wchar_t* wszFullPath, unsigned int flags, void* pReserved) const;

	/** This is called from your implementation of LoadMultiple() to add a 'deferred' content and the file it will be loaded from,
		when the LoadMultiple_Preload flag is set.
		\see CRhRdkContentIOPlugInEx::LoadMultiple() for an explanation of this method's use.
		\param c is the deferred content.
		\param wszFullPath is the full path to the file that 'c' will be loaded from.
		\param flags is reserved for future use; you should pass zero.
		\param pReserved is reserved for future use; you should pass nullptr. */
	virtual void ReportDeferredContentAndFile(CRhRdkContent& c, const wchar_t* wszFullPath, unsigned int flags, void* pReserved) const;

public:
	/** \internal For RDK internal use only. */
	void SetReport(class CRhRdkContentIOLoadMultipleReport*) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};
