
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

enum class RhRdkSmartMergeChoice : unsigned int
{
	DiscardIncoming,    // Keep the existing content and discard the incoming content.
	AutoRenameIncoming, // Automatically rename the incoming content with [imported] post-fix and keep both.
	ReplaceExisting,    // Replace the existing content with the incoming one.
	UseDialogToAsk,     // Ask the user what to do by displaying a dialog.
};

class RHRDK_SDK CRhRdkRendererSupportOptions
{
public:
	CRhRdkRendererSupportOptions();
	virtual ~CRhRdkRendererSupportOptions();

	void PopulateAdvancedOptions(void) const;

	virtual bool AlwaysShowSunPreview(void) const;
	virtual void SetAlwaysShowSunPreview(bool b);

	virtual bool PreviewCustomRenderMeshes(void) const;
	virtual void SetPreviewCustomRenderMeshes(bool b);

	virtual bool MultithreadedTextureEvaluation(void) const;
	virtual void SetMultithreadedTextureEvaluation(bool b);

	virtual bool HarvestContentParameters(void) const;
	virtual void SetHarvestContentParameters(bool b);

	enum { size1 = 128, size2 = 256, size3 = 512, size4 = 1024, size5 = 2048 };
	virtual int  TextureSize(void) const;

	RDK_DEPRECATED virtual void SetTextureSize(int size, bool bSendEvent=true);

	virtual bool CombineEditors(void) const;
	virtual void SetCombineEditors(bool b);

	// Libraries and File Explorer
	enum eInitialLocation
	{
		render_content,
		last_opened_folder,
		custom_folder
	};

	// Libraries - deprecated functions, please use the ones below.
	RDK_DEPRECATED virtual bool UseDefaultLibraryPath(void) const;
	RDK_DEPRECATED virtual void SetUseDefaultLibraryPath(bool bUseDefault);
	RDK_DEPRECATED virtual ON_wString CustomLibraryPath(void) const;
	RDK_DEPRECATED virtual void SetCustomLibraryPath(const wchar_t* wszPath);
	RDK_DEPRECATED virtual bool ShowRenderContent(void) const;
	RDK_DEPRECATED virtual bool ShowDocuments(void) const;
	RDK_DEPRECATED virtual bool ShowCustom(void) const;
	RDK_DEPRECATED virtual void SetShowRenderContent(bool b);
	RDK_DEPRECATED virtual void SetShowDocuments(bool b);
	RDK_DEPRECATED virtual void SetShowCustom(bool b);
	RDK_DEPRECATED virtual ON_wString CustomPaths(void) const; // Semicolon delimited.
	RDK_DEPRECATED virtual void SetCustomPaths(const wchar_t* wszPath);
	RDK_DEPRECATED virtual eInitialLocation LibrariesInitialLocation(void) const;
	RDK_DEPRECATED virtual void SetLibrariesInitialLocation(eInitialLocation l);
	RDK_DEPRECATED virtual ON_wString LibrariesInitialLocationCustomFolder(void) const;
	RDK_DEPRECATED virtual void SetLibrariesInitialLocationCustomFolder(const wchar_t* wszPath);

	// Libraries - expanded in May 2011
	bool             Libraries_UseDefaultLocation(void) const;
	void             Libraries_SetUseDefaultLocation(bool bUseDefault);
	ON_wString       Libraries_CustomPath(void) const;
	void             Libraries_SetCustomPath(const wchar_t* wszPath);
	bool             Libraries_ShowRenderContent(void) const;
	void             Libraries_SetShowRenderContent(bool b);
	bool             Libraries_ShowDocuments(void) const;
	void             Libraries_SetShowDocuments(bool b);
	bool             Libraries_ShowCustom(void) const;
	void             Libraries_SetShowCustom(bool b);
	ON_wString       Libraries_CustomPathList(void) const; // Semicolon delimited.
	void             Libraries_SetCustomPathList(const wchar_t* wszPath);
	eInitialLocation Libraries_InitialLocation(void) const;
	void             Libraries_SetInitialLocation(eInitialLocation l);
	ON_wString       Libraries_InitialLocationCustomFolder(void) const;
	void             Libraries_SetInitialLocationCustomFolder(const wchar_t* wszPath);
	ON_wString       Libraries_LastNavigatedFolder(void) const;
	void             Libraries_SetLastNavigatedFolder(const wchar_t* wszPath);

	// File Explorer.
	bool             FileExplorer_UseDefaultLocation(void) const;
	void             FileExplorer_SetUseDefaultLocation(bool bUseDefault);
	ON_wString       FileExplorer_CustomPath(void) const;
	void             FileExplorer_SetCustomPath(const wchar_t* wszPath);
	bool             FileExplorer_ShowRenderContent(void) const;
	void             FileExplorer_SetShowRenderContent(bool b);
	bool             FileExplorer_ShowDocuments(void) const;
	void             FileExplorer_SetShowDocuments(bool b);
	bool             FileExplorer_ShowCustom(void) const;
	void             FileExplorer_SetShowCustom(bool b);
	ON_wString       FileExplorer_CustomPathList(void) const; // Semicolon delimited.
	void             FileExplorer_SetCustomPathList(const wchar_t* wszPath);
	eInitialLocation FileExplorer_InitialLocation(void) const;
	void             FileExplorer_SetInitialLocation(eInitialLocation l);
	ON_wString       FileExplorer_InitialLocationCustomFolder(void) const;
	void             FileExplorer_SetInitialLocationCustomFolder(const wchar_t* wszPath);
	ON_wString       FileExplorer_LastNavigatedFolder(void) const;
	void             FileExplorer_SetLastNavigatedFolder(const wchar_t* wszPath);

	// Internal use only
#if defined RHINO_SDK_MFC && defined ON_RUNTIME_WIN
	void SetLastNavigatedLocation(LPCITEMIDLIST pidl);
	LPITEMIDLIST LastNavigatedLocation(void) const;
#endif

	virtual bool PreferNativeRenderer(void) const;
	virtual void SetPreferNativeRenderer(bool b);

	virtual bool UsePreviewCache(void) const;
	virtual void SetUsePreviewCache(bool b);

	virtual bool UseQuickInitialPreview(void) const;
	virtual void SetUseQuickInitialPreview(bool b);

	virtual bool UsePreview(void) const;

	virtual bool UseRenderedPreview(void) const;

	virtual bool ShowDetailsPanel(void) const;
	virtual void SetShowDetailsPanel(bool b);

	virtual ON_wString PreferredUnpackFolder(void) const;
	virtual void SetPreferredUnpackFolder(const wchar_t* wszPath);

	virtual bool CheckSupportFilesBeforeRendering(void) const;
	virtual void SetCheckSupportFilesBeforeRendering(bool b);

	// 14th Aug 2019 John Croudy, https://mcneel.myjetbrains.com/youtrack/issue/RH-54021
	// 19th Sep 2019 John Croudy, https://mcneel.myjetbrains.com/youtrack/issue/RH-54844
	RhRdkSmartMergeChoice SmartMergeChoice(bool bIsPaste) const;
	void SetSmartMergeChoice(bool bIsPaste, RhRdkSmartMergeChoice smc);

	// 15th Oct 2019 John Croudy, https://mcneel.myjetbrains.com/youtrack/issue/RH-54940
	bool CombineSimilarTextures(void) const;
	void SetCombineSimilarTextures(bool b);

	// 22nd June 2020 John Croudy, https://mcneel.myjetbrains.com/youtrack/issue/RH-58087
	ON_wString GetLastSavedRenderFileExtension(void) const;
	void SetLastSavedRenderFileExtension(const wchar_t* wszExt);

	virtual bool AutoSaveRenderings(void) const;
	virtual void SetAutoSaveRenderings(bool b);

	virtual int AutoSaveKeepAmount(void) const;
	virtual void SetAutoSaveKeepAmount(int value);

	virtual int MaxPreviewCacheMB(void) const;
	virtual int MaxPreviewSeconds(void) const;

	virtual_su int MaxDynamicIconCacheMB(void) const;

	virtual bool EnablePreviewJobLog(void) const;

	virtual COLORREF DarkPreviewCheckerColor(void) const;
	virtual COLORREF LightPreviewCheckerColor(void) const;

	bool EmbedFilesDocumentDefault(void) const;
	void SetEmbedFilesDocumentDefault(bool b);

	bool NeverEmbedFiles(void) const;
	void SetNeverEmbedFiles(bool b);

	bool AlwaysUnpackEmbeddedFilesToTemp(void) const;
	void SetAlwaysUnpackEmbeddedFilesToTemp(bool b);

	//-1 means "automatic" - typically # of processors - 1;
	int BakingQueueThreads(void) const;
	void SetBakingQueueThreads(int);

	bool SaveCompressedTextureFiles(void) const;
	void SetSaveCompressedTextureFiles(bool b);

	// 0 = adaptive baking quality disabled
	// 100 = bake to level of document tolerance
	// 200 = bake to level of 2.00 * document tolerance
	int AdaptiveBakingQuality(void) const;
	void SetAdaptiveBakingQuality(int);

	// If true, 'deleted' child textures are moved to the top-level document persistent texture list.
	// Otherwise they are really deleted.
	bool PreserveDeletedChildTextures(void) const;
	void SetPreserveDeletedChildTextures(bool);

public: // Sun dialog.
	virtual int LabelFormatLoc(void) const;
	virtual void SetLabelFormatLoc(int value);

	virtual int LabelFormatUTC(void) const;
	virtual void SetLabelFormatUTC(int value);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*);

private:
	enum { off, on, unset };
	static DWORD m_dwPreviewCustomRenderMeshes;
	static DWORD m_dwAlwaysShowSunPreview;
	static DWORD m_dwShowQuickInitialPreview;
};
