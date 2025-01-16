
#pragma once // Public SDK header

#include "RhRdkObject.h"
#include "IRhRdkRenderWindow.h"
#include "IRhRdkCustomRenderMeshManager.h"
#include "IRhRdkCustomRenderMeshes.h"

class CRhRdkSdkRenderImpl;
class CRhRdkCustomRenderMeshes;
class CRhRdkObjectAncestry;
class CRhRdkMaterial;

class RHRDK_SDK CRhRdkRenderMesh : public CRhRdkObject
{
public:
	CRhRdkRenderMesh();
	virtual ~CRhRdkRenderMesh();

	// Call this before extracting meshes. Although the Mesh() method
	// will always work correctly no matter what the primitive type is,
	// if your renderer supports rendering any of these primitives, you can
	// extract the data using the Sphere(), Plane(), Cone() and Box() methods.
	enum class RhRdkRenderMeshPrimitiveTypes PrimitiveType(void) const;

	virtual void Clear(void);

	virtual const CRhinoObject* Object(void) const;
	virtual void SetObject(const CRhinoObject* pObject);

	virtual void SetGeometry(const CRhRdkCustomRenderMeshes& crm, int iIndex);

	virtual_su void SetGeometryEx(const IRhRdkCustomRenderMeshes& crm, int iIndex);
	virtual_su void SetGeometryEx(const RhRdk::CustomRenderMeshes::IRenderMeshes::IInstance& instance);

	virtual const ON_Mesh* Mesh(void) const;

	virtual_su std::shared_ptr<const ON_Mesh> MeshEx(void) const;

	virtual void SetMesh(const ON_Mesh* pMesh);
	void SetMesh(std::shared_ptr<const ON_Mesh> pMesh);

	virtual const ON_Sphere* Sphere(void) const;
	virtual void SetSphere(const ON_Sphere& sphere);

	virtual bool Box(ON_PlaneSurface& plane, ON_Interval& z_interval) const;
	virtual void SetBox(const ON_PlaneSurface& plane, const ON_Interval& z_interval);

	virtual bool Plane(ON_PlaneSurface& plane) const;
	virtual void SetPlane(const ON_PlaneSurface& plane);	

	virtual bool Cone(ON_Cone& cone, ON_Plane& truncation) const;
	virtual void SetCone(const ON_Cone& cone, const ON_Plane& plane);

	virtual ON_Xform XformInstance(void) const;
	virtual void SetXformInstance(const ON_Xform& xform);

	// Note: Check RdkMaterial() first - if it is null, use ONMaterial().
	virtual bool IsRdkMaterial(void) const;
	virtual bool IsONMaterial(void) const;

	virtual const CRhRdkMaterial* RdkMaterial(void) const;
	virtual_su std::shared_ptr<const CRhRdkMaterial> RdkMaterialEx(void) const;

	virtual void SetRdkMaterial(const CRhRdkMaterial* pRdkMaterial);
	virtual_su void SetRdkMaterial(std::shared_ptr<const CRhRdkMaterial> pRdkMaterial);

	virtual const ON_Material* ONMaterial(void) const;
	virtual void SetONMaterial(const ON_Material* pONMaterial);

	virtual ON_BoundingBox BoundingBox(void) const;

	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

	/** \internal For RDK internal use only. */
	virtual const CRhRdkObjectAncestry* Ancestry(void) const;

	/** \internal For RDK internal use only. */
	virtual void SetAncestry(const CRhRdkObjectAncestry& rma);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK IRhRdkSdkRenderMeshIterator
{
public:
	virtual ~IRhRdkSdkRenderMeshIterator() { }

	virtual void EnsureRenderMeshesCreated(void) = 0;
	virtual void Reset(void) = 0;
	virtual bool Next(CRhRdkRenderMesh& rmOut) = 0;
	virtual bool SupportsAutomaticInstancing(void) const = 0;
	virtual void SetSupportsAutomaticInstancing(bool b) = 0;
	virtual ON_BoundingBox SceneBoundingBox(void) = 0;

	virtual CRhinoObjectIterator& ObjectIterator(void) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RHRDK_SDK IRhRdkAsyncRenderContext
{
public:
	virtual ~IRhRdkAsyncRenderContext() { }

	/** Called when RDK wants rendering to stop, for example when the user closes the
		window during rendering. You should stop rendering as soon as possible and only
		return when you are certain that your renderer will not attempt to access the
		render window again. */
	virtual void StopRendering(void) = 0;

	/** Implement this method to return \e true if your renderer supports pause / resume, else \e false. */
	virtual bool SupportsPause(void) const = 0;

	/** Implement this method to pause rendering. */
	virtual void PauseRendering(void) = 0;

	/** Implement this method to resume (i.e., un-pause) rendering. */
	virtual void ResumeRendering(void) = 0;

	/** You must implement this method as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** Implement this to be notified when a quiet rendering is finished. */
	virtual void OnQuietRenderFinished(const IRhRdkRenderSession& session) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RHRDK_SDK CRhRdkSdkRender : public CRhRdkObject
{
public:
	RDK_DEPRECATED CRhRdkSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn* pPlugIn,
	                               const ON_wString& sCaption, UINT uIconId);

	/** Construct a subclass of this object on the stack in your Rhino plug-in's
		Render() or RenderWindow() implementation.
	    \param context is the command context passed to the render command in Rhino.
	    \param plugIn is a reference to your Rhino plug-in.
		\param sRenderer is the name of the renderer.
		\param uIconId is the resource id of the icon to use for the frame window. */
	CRhRdkSdkRender(const CRhinoCommandContext& context, CRhinoRenderPlugIn& plugIn,
	                const ON_wString& sRenderer, UINT uIconId);

	virtual ~CRhRdkSdkRender();

	/** Get a pointer to the object that was passed to SetAsyncRenderContext().
		If SetAsyncRenderContext() has not been called, this method will return null.
		The returned pointer is managed by RDK and should not be stored. */
	virtual IRhRdkAsyncRenderContext* AsyncRenderContext(void) const;

	/** Set the rendering to be executed asynchronously. After this is called, Render() and RenderWindow()
		will return success immediately.
		\param pRC is a pointer to an instance of your implementation of IRhRdkAsyncRenderContext.
		Ownership of this object will be passed to RDK. */
	virtual void SetAsyncRenderContext(IRhRdkAsyncRenderContext* pRC);

	/** Render the scene normally. For synchronous renders, the function returns when rendering is complete (or canceled).
		\param sizeImage is the size of the image to be rendered. If you want RDK to provide this
		 value, you can call RenderSize(). */
	virtual CRhinoSdkRender::RenderReturnCodes Render(const ON_2iSize& sizeImage);

	/** Render the scene in a view window. The function returns when rendering is complete (or cancelled).
		\param pView is a pointer to the view that the user selected a rectangle in.
		\param pRect is the rectangle that the user selected.
		\param bInWindow is \e true to render directly into the view window (Not used in Rhino 3). */
	virtual CRhinoSdkRender::RenderReturnCodes RenderWindow(CRhinoView* pView, const LPRECT pRect, bool bInWindow);

	/** \return the command context that was passed to the constructor. */
	virtual const CRhinoCommandContext& CommandContext(void) const;

#if defined (RHINO_SDK_MFC)
	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED static CSize RenderSize(void);
#endif

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED static ON_2iSize RenderSize(const CRhinoDoc& doc);

	/** Get the render size as specified in the ON_3dmRenderSettings.
		\param doc is the document to use for the render settings.
		\param bUseRenderingSources is \e false to get the active viewport size
		 (the same as in previous versions of Rhino), \e true to use rendering sources:
		 It will automatically return the correct size based on rendering sources or
		 custom settings. \see ON_3dmRenderSettings::RenderingSources() */
 	static ON_2iSize RenderSize(const CRhinoDoc& doc, bool bUseRenderingSources);
 
	/** Returns the right view depending on ON_3dmRenderSettings::RenderingSource(). */
	class RHRDK_SDK CRenderingSourcesView final
	{
	public:
		CRenderingSourcesView(CRhinoDoc& doc);
		~CRenderingSourcesView();

	public:
		const ON_3dmView& View(void) const;
		bool HasError(void) const;

	private:
		class CImpl;
		CImpl* m_pImpl;
	};

	/** Get the render window interface. This interface is used to access the frame buffer to which
		computed pixels are written. Although this returns a reference, it is safe to store
		the address of the returned object for later use. The object is guaranteed to exist for the
		lifetime of the associated frame window. This is at least as long as the duration of your
		rendering procedure (render thread). */
	virtual IRhRdkRenderWindow& GetRenderWindow(void) const;

	/** \return The render session id associated with this render instance. */
	virtual UUID RenderSessionId(void) const;

	/** Saves the rendered image to a file.
		\note This is a legacy method which does not work unless there is rendering UI
		 on the screen. As a result, this
		 method has been deprecated.
		\param wszFilename is the full path to the file name to save to. If this
		 parameter is null, the function prompts the user for a file name.
		\return \e true if successful, else \e false. */
	RDK_DEPRECATED virtual bool SaveImage(const wchar_t* wszFilename) const;

	/** Saves the rendered image to a file. Does not prompt the user in any way.
		\param wszFilename is the full path to the file name to save to.
		\param bSaveAlpha determines if alpha will be saved in files that support it (e.g., PNG). */
	virtual IRhRdkRenderWindow::FileResult SaveImage(const wchar_t* wszFilename, bool bSaveAlpha) const;

	/** Closes the render window associated with this render instance.
	    \return \e true if successful, else \e false. */
	virtual bool CloseWindow(void) const;

	/** Set the number of seconds that need to elapse during rendering before the user
		is asked if the rendered image should be saved. */
	virtual void SetConfirmationSeconds(int iSeconds);

	/** \return A new render mesh iterator. The caller shall delete the iterator.
		Any meshes created by the iterator persist in memory for the lifetime of the iterator.
		\param bQuiet indicates that the render mesh iterator does not display any progress UI. */
	virtual class IRhRdkSdkRenderMeshIterator* NewRenderMeshIterator(const ON_Viewport& vp, bool bForceTriMesh=true, bool bQuiet=false) const;

	virtual bool IsRenderInitializing(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED static class IRhRdkSdkRenderMeshIterator* NewRenderMeshIterator(const UUID& uuidRenderPlugIn, const ON_Viewport& vp, bool bForceTriMesh=true);

	/** \return A new render mesh iterator. The caller shall delete the iterator.
		Any meshes created by the iterator persist in memory for the lifetime of the iterator. 
		Use this version if you are not using the CRhRdkSdkRender class to manage your render pipeline. */
	static class IRhRdkSdkRenderMeshIterator* NewRenderMeshIterator(const CRhinoDoc& doc, const UUID& uuidRenderPlugIn, const ON_Viewport& vp, bool bForceTriMesh=true, bool bQuiet=false);

public:
	/** Implement this method to start your rendering thread. */
	virtual void StartRendering(void) = 0;

	/** Override this method and return \e true if your renderer supports pause / resume. */
	virtual bool SupportsPause(void) const;

	/** Override this method to pause rendering. */
	virtual void PauseRendering(void);

	/** Override this method to resume (i.e., un-pause) rendering. */
	virtual void ResumeRendering(void);

	/** OBSOLETE. Not called. */
	virtual bool ReuseRenderWindow(void) const;

	/** \return \e true if the render window should be cleared at the start of rendering, else \e false. */
	virtual bool ClearLastRendering(void) const;

public: // Same interface as CRhinoSdkRender.
	CRhinoRenderPlugIn* PlugIn() const;
	virtual void StopRendering();
	virtual BOOL32 NeedToProcessGeometryTable();
	virtual BOOL32 NeedToProcessLightTable();
	virtual BOOL32 IgnoreRhinoObject(const CRhinoObject* pObject);
	virtual BOOL32 AddRenderMeshToScene(const CRhinoObject* pObject, const CRhinoMaterial& material, const ON_Mesh* pMesh);
	virtual BOOL32 AddLightToScene(const CRhinoLight& light);
	virtual BOOL32 RenderSceneWithNoMeshes();
	virtual BOOL32 StartRenderingInWindow(CRhinoView* pView, const LPCRECT rect);
	virtual BOOL32 StartRenderingQuiet(LPSIZE sizeImage);
	virtual BOOL32 RenderPreCreateWindow();
	virtual BOOL32 RenderEnterModalLoop();
	virtual BOOL32 RenderExitModalLoop();
	virtual BOOL32 RenderContinueModal();

private:
	friend class CRhRdkSdkRenderImpl;
	friend class CRhRdkRenderSession;
	void RenderSessionDeleting(void);
	CRhRdkSdkRenderImpl* m_pImpl;
};
