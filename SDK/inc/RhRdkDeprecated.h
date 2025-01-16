
#pragma once // Public SDK header

#include "IRhRdkCustomRenderMeshManager.h"

class CRhRdkContent;
class CRhRdkContentArray;
class CRhRdkPostEffectPlugIn;
class IRhRdkRenderWindow;
class IRhRdkPreviewSceneServer;

RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicTextureType;           // DEPRECATED; use uuidBitmapTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicBitmapTextureType;     // DEPRECATED; use uuidSimpleBitmapTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicWrinkledTextureType;   // DEPRECATED; use uuidTurbulenceTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicTileTextureType;       // DEPRECATED; use uuidTileTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicFBmTextureType;        // DEPRECATED; use uuidFBmTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicTurbulenceTextureType; // DEPRECATED; use uuidTurbulenceTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicGradientTextureType;   // DEPRECATED; use uuidGradientTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidBasicCheckerTextureType;    // DEPRECATED; use uuid3DCheckerTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuid2dCheckerTextureType;       // DEPRECATED; use uuid2DCheckerTextureType instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidDDSTextureType;             // DEPRECATED; DDS textures are now supported by the Basic Texture.

RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentBrowserDockBarButtonInterface; // Obsolete.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentEditorCustomButtonInterface;   // Obsolete.

/** This custom event is obsolete and is no longer sent. */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidModalGraphChanged;

/** Custom event for signalling RDK commands in menus need updating. */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidMenuChecksDirty;

/** This event is obsolete (see CRhRdkEventWatcher::OnDisplayUpdateBegin) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidDisplayUpdateBegin;

/** This event is obsolete (see CRhRdkEventWatcher::OnDisplayUpdateEnded) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidDisplayUpdateEnded;

/** This event is obsolete (see CRhRdkEventWatcher::OnUndoRedoBegin) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidUndoRedoBegin;

/** This event is obsolete (see CRhRdkEventWatcher::OnUndoRedoEnded) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidUndoRedoEnded;

/** This event is obsolete. */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidUIComplexityChanged;

/** This event is obsolete (see CRhRdkEventWatcher::OnContentGroupIdChanged) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidGroupIdChanged;

/** This event is obsolete. */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidChildPanelVisibilityChanged;

/** This event is obsolete (see CRhRdkEventWatcher::OnContentAttached) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentAdded;

/** This event is obsolete (see CRhRdkEventWatcher::OnContentAddedByUser) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentAddedByUser;

/** This event is obsolete (see CRhRdkEventWatcher::OnContentSelection) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentMultipleSelection;

/** This event is obsolete (see CRhRdkEventWatcher::OnRenderingFinished) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidRenderingFinished;

/** This event is obsolete (see CRhRdkEventWatcher::OnContentRenaming) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentRenaming;

/** This event is obsolete. */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidUserDataTransformed;

/** This event is obsolete. */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidContentMoved;

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkRenderingFileInfo
{
public:
	CRhRdkRenderingFileInfo();
	ON_wString m_sFilename;
	ON_wString m_sRenderer;
	UUID m_uuidRenderEngine;
	UUID m_uuidRenderSession;
	int m_iElapsedSeconds;
	UINT_PTR m_Reserved;
};

/** This event is obsolete (see CRhRdkEventWatcher::OnRenderingLoaded) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidRenderingLoaded;

/** This event is obsolete (see CRhRdkEventWatcher::OnRenderingSaved) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidRenderingSaved;

/** This event is obsolete (see CRhRdkEventWatcher::OnRenderingDeleted) */
RDK_DEPRECATED RHRDK_SDK extern UUID uuidRenderingDeleted;

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkRenderWindowClonedInfo
{
public:
	CRhRdkRenderWindowClonedInfo();
	UUID m_uuidRenderSessionOld;
	UUID m_uuidRenderSessionNew;
	UINT_PTR m_Reserved;
};

/** \class IRhRdkCustomPlugIn

	\deprecated in favor of CRhRdkExtension
	\see CRhRdkExtension
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkCustomPlugIn() { }
	virtual void DeleteThis(void) = 0;             // Removed in CRhRdkExtension; obsolete.
	virtual UUID PlugInID(void) const = 0;         // Changed to PlugInId() in CRhRdkExtension.
	virtual UUID CustomPlugInType(void) const = 0; // Changed to InterfaceId() in CRhRdkExtension.
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
};

/** \class IRhRdkRenderWindowEventSink

	\deprecated in favor of CRhRdkRenderWindowEventSink
	\see CRhRdkRenderWindowEventSink
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkRenderWindowEventSink : public IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkRenderWindowEventSink() { }
	virtual void OnLButtonDown(const CPoint& pt) = 0;
	virtual void OnLButtonUp(const CPoint& pt) = 0;
	virtual void OnLButtonDblClick(const CPoint& pt) = 0;
	virtual void OnRButtonDown(const CPoint& pt) = 0;
	virtual void OnRButtonUp(const CPoint& pt) = 0;
	virtual void OnRButtonDblClick(const CPoint& pt) = 0;
};

/** \class IRhRdkRenderWindowCustomDlg

	\deprecated in favor of CRhRdkRenderWindowCustomDlg
	\see CRhRdkRenderWindowCustomDlg
*/
#if defined (RHINO_SDK_MFC)
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkRenderWindowCustomDlg : public IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkRenderWindowCustomDlg() { }
	virtual const wchar_t* DockBarTitle(const UUID&) const = 0;
	virtual bool Create(CWnd*, const UUID&) = 0;
	virtual void Destroy(const UUID&) = 0;
	virtual void MoveWindow(const ON_4iRect&, const UUID&) = 0;
	virtual UINT EnableDockingFlags(void) const = 0;
	virtual UINT DockControlBarFlags(void) const = 0;
	virtual bool InitialShowControlBar(void) const = 0;
	virtual bool AlwaysShowDialog(void) const = 0;
	virtual CDialog* Dialog(const UUID&) const = 0;
};
#endif

/** \class IRhRdkRenderFrameTabFactory

	\deprecated in favor of CRhRdkRenderFrameTabFactoryEx
	\see CRhRdkRenderFrameTabFactoryEx
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkRenderFrameTabFactory : public IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkRenderFrameTabFactory() { }
	virtual class CRhinoUiDockBarTab* NewTab(IRhRdkRenderWindow*) = 0;
	virtual UUID RenderEngineId(void) const = 0;
	virtual UUID TabId(void) const = 0;
};

/** \class IRhRdkContentFactory

	\deprecated in favor of CRhRdkContentFactory
	\see CRhRdkContentFactory
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkContentFactory
{
public:
	virtual void DeleteThis(void) = 0;
	virtual bool Initialize(void) = 0;
	virtual const wchar_t* Kind(void) const = 0;
	virtual bool IsKind(CRhRdkContent::Kinds kind) const = 0;
	virtual bool IsElevated(void) const = 0;
	virtual CRhRdkContent* NewContent(void) const = 0;
	virtual UUID ContentTypeId(void) const = 0;
	virtual ON_wString ContentInternalName(void) const = 0;
	virtual UUID RenderEngineId(void) const = 0;
	virtual UUID PlugInId(void) const = 0;
	virtual void RebuildCache(void) = 0;
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
	virtual bool IsBuiltIn(void) const = 0;
	virtual ~IRhRdkContentFactory() { }
};

/** \class CRhRdkContentMultipleSelection

	\deprecated OBSOLETE
	\see CRhRdkEventWatcher::OnContentSelection()

*/
class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkContentMultipleSelection
{
public:
	CRhRdkContentMultipleSelection(const CRhRdkContentArray& aContentSel, const CRhRdkContentArray& aContentEdit, int el, const UUID& e, void* p);

	const CRhRdkContentArray& ContentSel(void) const { return m_aContentSel; }
	const CRhRdkContentArray& ContentEdit(void) const { return m_aContentEdit; }
	int EditingLevel(void) const { return m_iEditingLevel; }
	UUID EditorId(void) const { return m_uuidEditor; }

private:
	void* m_pReserved;
	int m_iEditingLevel;
	UUID m_uuidEditor;
	const CRhRdkContentArray& m_aContentSel;
	const CRhRdkContentArray& m_aContentEdit;
};

/** \class IRhRdkPostEffectPlugInFactory

	\deprecated in favor of CRhRdkPostEffectPlugInFactory
	\see CRhRdkPostEffectPlugInFactory
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkPostEffectPlugInFactory
{
public:
	virtual ~IRhRdkPostEffectPlugInFactory() { }
	virtual UUID PlugInId() const = 0;
	virtual void DeleteThis(void) = 0;
	virtual CRhRdkPostEffectPlugIn* NewPlugIn(void) const = 0;
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
};

/** \class IRhRdkPostEffectPlugInFactories

	\deprecated in favor of CRhRdkPostEffectPlugInFactories
	\see CRhRdkPostEffectPlugInFactories
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkPostEffectPlugInFactories
{
public:
	virtual ~IRhRdkPostEffectPlugInFactories() { }
	virtual void Add(IRhRdkPostEffectPlugInFactory* pFactory) = 0;
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
};

/** \class IRhRdkContentEditorCustomButtonInterface

	\deprecated in favor of CRhRdkContentEditorCustomButtonProvider
	\see CRhRdkContentEditorCustomButtonProvider
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkContentEditorCustomButtonInterface : public IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkContentEditorCustomButtonInterface() { }
	enum RDK_DEPRECATED eStyle { styleNormal, styleFlat, styleFlatPopup, styleForce32   = 0xFFFFFFFF };
	virtual UUID RenderEngineID(void) const = 0;
	virtual int LeftMargin(void) const = 0;
	virtual int ButtonHeight(void) const = 0;
	virtual bool ButtonDetails(int iIndex, CRhRdkContent::Kinds kind, CRhinoDib& dibOut, ON_wString& sCaptionOut, ON_wString& sToolTipOut, int& iWidthOut, int& iGapOut, COLORREF& colBackOut, eStyle& styleOut, DWORD& dwFlagsOut, DWORD_PTR reserved) const = 0;
	virtual bool OnButtonClicked(int index) = 0;
};

/** \class IRhRdkCustomRenderFrameToolBar

	\deprecated in favor of CRhRdkCustomRenderFrameToolBar
	\see CRhRdkCustomRenderFrameToolBar
*/
#if defined (RHINO_SDK_MFC)
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCustomRenderFrameToolBar : public IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkCustomRenderFrameToolBar() { }
	virtual IRhRdkCustomRenderFrameToolBar* Clone(void) const = 0;
	virtual UUID ToolBarID(void) const = 0;
	virtual UUID RenderEngineID(void) const = 0;
	virtual int ButtonCount(void) const = 0;
	virtual const wchar_t* Caption(void) const = 0;
	virtual bool GetColdBitmap(CRhinoDib& dibOut) = 0;
	virtual bool GetHotBitmap(CRhinoDib& dibOut) = 0;
	virtual bool GetGrayBitmap(CRhinoDib& dibOut) = 0;
	virtual void SetFirstCommandId(UINT_PTR nID) = 0;
	virtual const wchar_t* GetToolTipText(UINT_PTR nID) const = 0;
	virtual const wchar_t* GetMessageString(UINT_PTR nID) const = 0;
	virtual bool IsSeparator(int iIndex) const = 0;
	virtual bool OnCommand(UINT_PTR nID) = 0;
	virtual void OnUpdateCommandUI(class CCmdUI& cmdUI) const = 0;
};
#endif

/** \class IRhRdkCustomRenderFrameMenu

	\deprecated in favor of CRhRdkCustomRenderFrameMenu
	\see CRhRdkCustomRenderFrameMenu
*/
#if defined (RHINO_SDK_MFC)
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCustomRenderFrameMenu : public IRhRdkCustomPlugIn
{
public:
	virtual ~IRhRdkCustomRenderFrameMenu() { }
	enum class RDK_DEPRECATED MenuType : unsigned int { kMain, kFile, kEdit, kView, kHelp };
	virtual bool AddMenuItems(HMENU menu, MenuType type, UINT& cmdId) = 0;
	virtual bool OnCommand(UINT cmdId) = 0;
	virtual bool OnUpdateCommandUI(class CCmdUI& cmdUI) const = 0;
};
#endif

/** \class IRhRdkCompoundContentImplementor

	\deprecated in favor of CRhRdkCompoundContentImplementor
	\see CRhRdkCompoundContentImplementor
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCompoundContentImplementor
{
public:
	virtual ~IRhRdkCompoundContentImplementor() { }
	virtual void DeleteThis(void) = 0;
	virtual UUID Kind(void) const = 0;
	virtual UUID PlugInId(void) const = 0;
	virtual UUID TargetRenderEngineId(void) const = 0;
	virtual IRhRdkCompoundContentImplementor* NewInstance(void) const = 0;
	virtual bool DisableLighting(void) const = 0;
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
	virtual bool IsBuiltIn(void) const = 0;
};

/** \class IRhRdkCompoundContentImplementors

  Obsolete.
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCompoundContentImplementors
{
public:
	virtual ~IRhRdkCompoundContentImplementors() { }
	virtual void Add(IRhRdkCompoundContentImplementor* pCCI) = 0;
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
};

#ifndef EXCLUDE_OLD_CRMS

class IRhRdkCustomRenderMeshProvider2;

/** \class IRhRdkCustomRenderMeshProvider

	\deprecated in favor of CRhRdkCustomRenderMeshProvider
	\see CRhRdkCustomRenderMeshProvider
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCustomRenderMeshProvider
{
public:
	virtual ~IRhRdkCustomRenderMeshProvider() { }
	virtual UUID ProviderId(void) const = 0;
	virtual UUID PlugInId(void) const = 0;
	virtual ON_wString Name(void) const = 0;
	virtual void DeleteThis(void) = 0;
	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshManager::eMeshType type) const = 0;
	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshManager::eMeshType type) const = 0;
	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshes& crmInOut, IRhRdkCustomRenderMeshManager::eMeshType type) const = 0;
	virtual bool IsViewDependent(void) const = 0;
	virtual bool IsPlugInDependent(void) const = 0;
	virtual bool IsPreviewAndStandardSameMesh(void) const = 0;
	virtual IRhRdkCustomRenderMeshProvider2* CustomRenderMeshProvider2(void);
	virtual CRhRdkVariant GetParameter(const CRhinoObject& object, const wchar_t* wszParamName) const = 0;
	virtual void SetParameter(const CRhinoObject& object, const wchar_t* wszParamName, const CRhRdkVariant& value) = 0;
	virtual void* EVF(const wchar_t*, void*) { return NULL; }
};

/** \class IRhRdkCustomRenderMeshProvider2

  Obsolete.
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkCustomRenderMeshProvider2 : public IRhRdkCustomRenderMeshProvider
{
};

#endif

/** \class IRhRdkContentIOPlugIn

	\deprecated in favor of CRhRdkContentIOPlugIn
	\see CRhRdkContentIOPlugIn
*/
class RDK_DEPRECATED_CLASS RHRDK_SDK IRhRdkContentIOPlugIn
{
public:
	virtual ~IRhRdkContentIOPlugIn() { }
	virtual UUID RdkPlugInId(void) const = 0;
	virtual void DeleteThis(void) = 0;
	virtual const wchar_t* FileExtension(void) const = 0;
	virtual const wchar_t* Description(void) const = 0;
	virtual bool CanLoad(void) const = 0;
	virtual bool CanSave(void) const = 0;
	RDK_DEPRECATED virtual CRhRdkContent* Load(const wchar_t* wszFilename) const;
	virtual CRhRdkContent* Load(const CRhinoDoc* pDoc, const wchar_t* wszFilename) const = 0;
	virtual bool Save(const wchar_t* wszFilename, const CRhRdkContent& content, const IRhRdkPreviewSceneServer* pSceneServer) const = 0;
	virtual const wchar_t* Kind(void) const = 0;
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkCustomRenderSaveInfo // Not needed in V6.
{
public:
	CRhRdkCustomRenderSaveFileType m_FileType;
	UUID m_uuidRenderSession;
	ON_wString m_sFilename;
	bool m_bSaveAlpha;
};

RDK_DEPRECATED extern const wchar_t* RDK_MAT_ALPHA_TRANSPARENCY                             /* use FS_MAT_ALPHA_TRANSPARENCY       */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_AMBIENT                                        /* use FS_MAT_AMBIENT                  */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_DIFFUSE                                        /* use FS_MAT_DIFFUSE                  */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_DISABLE_LIGHTING                               /* use FS_MAT_DISABLE_LIGHTING         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_EMISSION                                       /* use FS_MAT_EMISSION                 */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_FLAMINGO_LIBRARY                               /* use FS_MAT_FLAMINGO_LIBRARY         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_FRESNEL_ENABLED                                /* use FS_MAT_FRESNEL_ENABLED          */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_CLARITY_AMOUNT                                 /* use FS_MAT_CLARITY_AMOUNT           */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_IOR                                            /* use FS_MAT_IOR                      */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_POLISH_AMOUNT                                  /* use FS_MAT_POLISH_AMOUNT            */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_REFLECTION                                     /* use FS_MAT_REFLECTION               */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_REFLECTIVITY                                   /* use FS_MAT_REFLECTIVITY_AMOUNT      */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_REFLECTIVITY_COLOR                             /* use FS_MAT_REFLECTIVITY_COLOR       */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_SHINE                                          /* use FS_MAT_SHINE                    */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_SPECULAR                                       /* use FS_MAT_SPECULAR                 */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_TRANSPARENCY                                   /* use FS_MAT_TRANSPARENCY_AMOUNT      */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_TRANSPARENCY_COLOR                             /* use FS_MAT_TRANSPARENCY_COLOR       */ ;
RDK_DEPRECATED extern const wchar_t* RDK_MAT_TYPE;

RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND                                     /* use FS_ENV_BACKGROUND_COLOR         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_IMAGE                               /* use FS_ENV_BACKGROUND_IMAGE         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION                          /* use FS_ENV_BACKGROUND_PROJ          */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_AUTOMATIC                /* use FS_ENV_PROJ_AUTOMATIC           */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_BOX                      /* use FS_ENV_PROJ_BOX                 */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_CUBEMAP                  /* use FS_ENV_PROJ_CUBE_MAP            */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_EMAP                     /* use FS_ENV_PROJ_EMAP                */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_HEMISPHERICAL            /* use FS_ENV_PROJ_HEMISPHERICAL       */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_HORIZONTAL_CROSS_CUBEMAP /* use FS_ENV_PROJ_CUBE_MAP_HORZ       */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_LIGHTPROBE               /* use FS_ENV_PROJ_LIGHT_PROBE         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_PLANAR                   /* use FS_ENV_PROJ_PLANAR              */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_SPHERICAL                /* use FS_ENV_PROJ_SPHERICAL           */ ;
RDK_DEPRECATED extern const wchar_t* RDK_ENV_BACKGROUND_PROJECTION_VERTICAL_CROSS_CUBEMAP   /* use FS_ENV_PROJ_CUBE_MAP_VERT       */ ;

RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_OFFSET                                     /* use FS_TEX_OFFSET                   */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_REPEAT                                     /* use FS_TEX_REPEAT                   */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ROTATN                                     /* use FS_TEX_ROTATION                 */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_WCS_UNITS                                  /* ------------ OBSOLETE ------------- */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_PROJECTION_MODE                            /* use FS_TEX_PROJECTION_MODE          */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_WRAP_TYPE                                  /* use FS_TEX_WRAP_TYPE                */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_MAPPING_CHANNEL                            /* use FS_TEX_MAPPING_CHANNEL          */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ENVIRONMENT_MAPPING_MODE                   /* use FS_TEX_ENVIRONMENT_MAPPING_MODE */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_OFFSET_LOCKED                              /* use FS_TEX_OFFSET_LOCKED            */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_REPEAT_LOCKED                              /* use FS_TEX_REPEAT_LOCKED            */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_PREVIEW_LOCAL_MAPPING                      /* use FS_TEX_PREVIEW_LOCAL_MAPPING    */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_PREVIEW_IN_3D                              /* use FS_TEX_PREVIEW_IN_3D            */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_DISPLAY_IN_VIEWPORT                        /* use FS_TEX_DISPLAY_IN_VIEWPORT      */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_INVERT                              /* use FS_TEX_ADJUST_INVERT            */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_GRAYSCALE                           /* use FS_TEX_ADJUST_GRAYSCALE         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_CLAMP                               /* use FS_TEX_ADJUST_CLAMP             */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_SCALE_TO_CLAMP                      /* use FS_TEX_ADJUST_SCALE_TO_CLAMP    */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_MULTIPLIER                          /* use FS_TEX_ADJUST_MULTIPLIER        */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_CLAMP_MIN                           /* use FS_TEX_ADJUST_CLAMP_MIN         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_CLAMP_MAX                           /* use FS_TEX_ADJUST_CLAMP_MAX         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_GAIN                                /* use FS_TEX_ADJUST_GAIN              */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_SATURATION                          /* use FS_TEX_ADJUST_SATURATION        */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_HUE_SHIFT                           /* use FS_TEX_ADJUST_HUE_SHIFT         */ ;
RDK_DEPRECATED extern const wchar_t* RDK_TEXTURE_ADJUST_GAMMA                               /* use FS_TEX_ADJUST_GAMMA             */ ;

// The phrase 'Basic' has been removed from these classes.

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkBasicOneColorTextureBase : public CRhRdkOneColorTextureBase
{
};

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkBasicTwoColorTextureBase : public CRhRdkTwoColorTextureBase
{
};

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkBasicOneColorTextureBaseCSI : public CRhRdkOneColorTextureBaseCSI
{
public:
	CRhRdkBasicOneColorTextureBaseCSI(const CRhRdkContent* p, CRhRdkContent::CSIContext c) : CRhRdkOneColorTextureBaseCSI(p, c) { }
};

class RDK_DEPRECATED_CLASS RHRDK_SDK CRhRdkBasicTwoColorTextureBaseCSI : public CRhRdkTwoColorTextureBaseCSI
{
public:
	CRhRdkBasicTwoColorTextureBaseCSI(const CRhRdkContent* p, CRhRdkContent::CSIContext c) : CRhRdkTwoColorTextureBaseCSI(p, c) { }
};
