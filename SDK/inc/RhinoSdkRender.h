//
// Copyright (c) 1993-2017 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#pragma once

#if defined (ON_RUNTIME_WIN)
RHINO_SDK_FUNCTION UINT RhinoSdkRenderCloseRenderFrameMsg();
RHINO_SDK_FUNCTION UINT RhinoSdkRenderRenderFrameCancelMsg();

RHINO_SDK_FUNCTION void RhinoPostSdkRenderCloseRenderFrameMsg( WPARAM wParam = 0, LPARAM lParam = 0L);
RHINO_SDK_FUNCTION void RhinoPostSdkRenderRenderFrameCancelMsg( WPARAM wParam = 0, LPARAM lParam = 0L);
#endif

// This class has been modified, you should now use the CRhinoSdkRenderEx class defined at the bottom 
// of this file.
class RHINO_SDK_CLASS CRhinoSdkRender
{
public:
  ON_DEPRECATED_MSG("Use CRhRdkSdkRender instead") CRhinoSdkRender( CRhinoDoc& doc, CRhinoRenderPlugIn* plugin);
  virtual ~CRhinoSdkRender();

  CRhinoDoc& Document(){ return m_rhino_doc;};

  // Render command an helpers
  enum RenderReturnCodes
  {
    render_ok = 0,
    render_empty_scene,
    render_cancel,
    render_no_active_view,
    render_on_pre_create_window,
    render_no_frame_wnd_pointer,
    render_error_creating_window,
    render_error_starting_render,
    render_enter_modal_loop,
    render_exit_modal_loop,
    render_wm_quit,
    // add new codes to RenderReturnCodesToString()
  };

  RenderReturnCodes Render( const CRhinoCommandContext& context, LPSIZE sizeImage = 0, const wchar_t* lpsWindowTitle = 0, UINT nIDResource = 0);
  RenderReturnCodes RenderWindow( const CRhinoCommandContext& context, CRhinoView* view, const LPRECT rect, bool bRenderInWindow);

  void RenderReturnCodesToString( RenderReturnCodes code, ON_wString& out) const;

  int ProcessGeometryTable();
  int ProcessLightTable();
  CRhinoRenderPlugIn* PlugIn() const;
  
  DWORD StartTime() const { return m_start_time;}
  DWORD EndTime() const { return m_end_time;}
  double ElapsedTime( ON_wString& out) const;

  /////////////////////////////////////////////////////////////////////////////////
  //
  // Virtual interface
  //
  //-------------------------------------------------------------------------------
  // Description:
  //   Called by Render() to determine if ProcessGeometryTable() will get called.
  // Returns:
  //   TRUE  - Render() will call ProcessGeometryTable()
  //   FALSE - nothing changed so use plug-ins own mesh list
  virtual BOOL32 NeedToProcessGeometryTable();
  //-------------------------------------------------------------------------------
  // Description:
  //   Called by Render() to determine if ProcessLightTable() will get called.
  // Returns:
  //   TRUE  - Render() will call ProcessLightTable()
  //   FALSE - nothing changed so use plug-ins own light list
  virtual BOOL32 NeedToProcessLightTable();
  // Description:
  //   Called br Render() when no meshes are added to scene.
  // Returns:
  //   TRUE  - If plug-in will render scene with no meshes
  //   FALSE - abort, nothing to render
  virtual BOOL32 RenderSceneWithNoMeshes();
  // Description:
  //   Called br ProcessGeometryTable() when an object is encountered that does 
  //   not contain a render mesh.
  // Returns:
  //   TRUE  - object won't be rendered
  //   FALSE - If plug-in will render this object
  virtual BOOL32 IgnoreRhinoObject( const CRhinoObject* object);
  // Description:
  //   Called br ProcessGeometryTable() when an object is encountered that contains
  //   a render mesh.
  // Returns:
  //   TRUE  - If plug-in will render this object
  //   FALSE - object won't be rendered
  virtual BOOL32 AddRenderMeshToScene( const CRhinoObject* object, const CRhinoMaterial& material, const ON_Mesh*);
  // Description:
  //   Called br ProcessGeometryTable() when an light is encountered that should
  //   be included in the rendered scene.
  // Returns:
  //   TRUE  - If plug-in will use this light
  //   FALSE - ignore this light
  virtual BOOL32 AddLightToScene( const class CRhinoLight& light);
  /*
  Description:
    Called by Render or RenderWindow() when a key press or mouse down event occurs during rendering.  To interrupt
    rendering make ContinueModal() return false.
  */
  virtual void StopRendering();
  /*
  Description:
    Override this method and return true if your renderer supports pause / resume.
  */
  virtual bool SupportsPause() const;
  /*
  Description:
    If your renderer supports pause / resume, override this method to pause rendering.
  */
  virtual void PauseRendering();
  /*
  Description:
    If your renderer supports pause / resume, override this method to resume rendering.
  */
  virtual void ResumeRendering();
  /*
  Description:
    Called by RenderWindow when plug-in should start render processing. 
  Parameters:
    view - [in] View user selected 2d render rectangle in.
    rect - [in] Screen rectangle user selected.  Minimum size is 16x16 pixels.
  Returns:
    If false is returned then the Render() or RenderWindow() will abort.
  */
  virtual BOOL32 StartRenderingInWindow( CRhinoView* view, const LPCRECT rect);
  /*
  Description:
    Called by Render() just prior to calling NewRenderFrame(). Called by RenderWindow() just
    prior to calling StartRenderingInWindow();
  Returns:
    If false is returned then the Render() or RenderWindow() will abort.
  */
  virtual BOOL32 RenderPreCreateWindow();
  /*
  Description:
    Called by Render() just after RenderPreCreateWindow().  The frame returned by this 
    function will be used to display the rendered image in.
  Parameters:
    doc - [in] Active Rhino document;
  Returns:
    Pointer to CRhinoSdkRenderFrame object.  If NULL is returned then Render() will abort.
  */
  virtual class CRhinoSdkRenderFrame* NewRenderFrame( CRhinoDoc* doc) = 0;
  /*
  Description:
    Called by Render() and RenderWindow() just prior to entering modal loop.
  Returns:
    If false is returned Render() and RenderWindow() will abort.
  */
  virtual BOOL32 RenderEnterModalLoop();
  /*
  Description:
    Called by Render() and RenderWindow() during modal processing.  Modal loop will continue
    until a cancel event occurs until this function returns false.
  Returns:
    If false when rendering is complete.  This allows the Render and RenderWindow commands to
    return control to Rhino when rendering is complete.
  */
  virtual BOOL32 RenderContinueModal();
  /*
  Description:
    Called by Render() and RenderWindow() just after modal loop is exited.
  Returns:
    If false is returned Render() and and RenderWindow() will cancel.
  */
  virtual BOOL32 RenderExitModalLoop();
  /*
  Description:
    Determines if -Render or -RenderWindow was started command.
  Returns:
    A value of true means we are in batch scripting mode.
  */
  bool InBatchMode() const;
  /*
  Description:
    Returns code which caused Render() or RenderWindow() to terminate.
  Returns:
    Returns code which caused Render() or RenderWindow() to terminate.
  */
  int ErrorCode() const;

  // For internal use only
  void OnRenderFrameDestruction();

private:
  CRhinoSdkRender( const CRhinoSdkRender&) = delete;
  CRhinoSdkRender& operator=( const CRhinoSdkRender&) = delete;

protected:
  RenderReturnCodes ReturnRenderCode( RenderReturnCodes rc);
  CRhinoView* RhinoView() const { return m_rhino_view;}
  void SetRhinoView( CRhinoView* v) { m_rhino_view = v;}

  CRhinoDoc& m_rhino_doc;
  int m_mesh_count;
  int m_light_count;
  DWORD m_start_time;
  DWORD m_end_time;
  BOOL32 m_render_canceled;
  CRhinoRenderPlugIn* m_plugin;
  CRhinoView* m_rhino_view;
private:
  class CRhinoSdkRenderFrame* m_render_frame;		//Deprecated.  Don't use this.
  friend class CRhinoSdkRenderEx;
protected:
  bool m_batch_mode;
  int  m_error_code;
protected:
  class CImpl;
  CImpl* Impl();
  friend CImpl;
  ON__UINT_PTR m_sdk_reserved = 0;
};

// This class was added to fix a problem with blocks.  If a block is being rendered then you need a CRhinoObjRef& instead
// of a const CRhinoObject* so that you can get the block geometry insert transform.
class RHINO_SDK_CLASS CRhinoSdkRenderEx : public CRhinoSdkRender
{
public:
  ON_DEPRECATED_MSG("Use CRhRdkSdkRender instead") CRhinoSdkRenderEx( CRhinoDoc& doc, CRhinoRenderPlugIn* plugin);
  virtual ~CRhinoSdkRenderEx();
public:
  virtual RenderReturnCodes RenderEx( const CRhinoCommandContext& context, LPSIZE sizeImage = 0, const wchar_t* lpsWindowTitle = 0, UINT nIDResource = 0);
  virtual RenderReturnCodes RenderWindowEx( const CRhinoCommandContext& context, CRhinoView* view, const LPRECT rect, bool bRenderInWindow);
  virtual void RenderReturnCodesToStringEx( RenderReturnCodes code, ON_wString& out) const;
  virtual LRESULT QuerySdkRenderEx(UINT message, WPARAM wParam, LPARAM lParam);
public:
  // Description:
  //   Called br ProcessGeometryTableEx() when an object is encountered that does 
  //   not contain a render mesh.
  // Returns:
  //   TRUE  - object won't be rendered
  //   FALSE - If plug-in will render this object
  virtual BOOL32 IgnoreRhinoObjectEx( const CRhinoObjRef& ref);
  // Description:
  //   Called br ProcessGeometryTableEx() when an object is encountered that contains
  //   a render mesh.
  // Returns:
  //   TRUE  - If plug-in will render this object
  //   FALSE - object won't be rendered
  virtual BOOL32 AddRenderMeshToSceneEx( const CRhinoObjRef& ref, const CRhinoMaterial& material, const ON_Mesh*);
  // Description:
  //   Reserved for future use.  This may be used to extend this classed functionality in the
  //   future.
  virtual LRESULT CallBackProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
  virtual int ProcessGeometryTableEx();
  virtual int ProcessLightTableEx();
  virtual void OnRenderFrameDestructionEx();
};

class RHINO_SDK_CLASS CRhinoSdkRenderEx2 : public CRhinoSdkRenderEx
{
public:
	ON_DEPRECATED_MSG("Use CRhRdkSdkRender instead") CRhinoSdkRenderEx2(CRhinoDoc& doc, CRhinoRenderPlugIn* plugin);
	virtual class IRhinoSdkRenderFrame* NewRenderFrameEx(CRhinoDoc* pDocument) = 0;

protected:
	virtual class CRhinoSdkRenderFrame* NewRenderFrame(CRhinoDoc* doc) override;
};

class RHINO_SDK_CLASS CRhinoSdkRenderEx3 : public CRhinoSdkRenderEx2
{
public:
	CRhinoSdkRenderEx3(CRhinoDoc& doc, CRhinoRenderPlugIn* plugin);

	virtual void OnRenderWindowFinished(void) = 0;
	virtual class IRhinoSdkRenderFrame* NewRenderFrameEx3(CRhinoDoc* pDocument, bool bQuiet) = 0;
};
