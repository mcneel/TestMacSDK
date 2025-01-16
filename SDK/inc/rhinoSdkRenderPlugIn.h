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
#include "rhinoSdkCommand.h"

////////////////////////////////////////////////////////////////////////////////////
//
//  class CRhinoRenderPlugIn
//
//  Description:
//    Control class usesd when creating Render plug-ins.  Render plug-ins have the 
//    ability to replace the Rhino render command and object properties render 
//    property page.
//
class RHINO_SDK_CLASS CRhinoRenderPlugIn : public CRhinoPlugIn
{
	ON_OBJECT_DECLARE( CRhinoRenderPlugIn);
public:
  CRhinoRenderPlugIn();
  ~CRhinoRenderPlugIn();

  plugin_type PlugInType() const override;
  ////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Render plug-in specific
  //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Required overrides
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Called by Render and RenderPreview commands if this application is a Render PlugIn and
  // is set as the default render engine.
  //
  // context [in] Command parameters passed to the render command.
  // render_preview [in] If true, lower quality faster render expected.
  //
  virtual CRhinoCommand::result Render( const CRhinoCommandContext& context, bool render_preview) = 0;
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Called by RenderWindow and RenderWindowPreview commands if this application is a Render
  // PlugIn and is set as the default render engine.
  //
  // context [in] Command parameters passed to the render command.
  // render_preview [in] If true, lower quality faster render expected.
  // view [in] View to render.
  // rect [in] Rectangle to render in viewport window coordinates.
  // bInWindow [in] If true the output destination is a standard CRhinoSdkRenderView otherwise
  //                output to the current CRhinoView.
  // bBlowUp [in] If true, the output size will be pixel-for-pixel the same as the full output (as if
  //              you'd called "Render") but only the window specified. In other words, if you took
  //              the image produced by this function and pasted it over an image from the same view,
  //              it would perfectly fit into the image without resizing.
  //
  virtual CRhinoCommand::result RenderWindow( const CRhinoCommandContext& context, bool render_preview,
                                               CRhinoView* view, const LPRECT rect, bool bInWindow, bool bBlowUp);
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Optional overrides
  //////////////////////////////////////////////////////////////////////////////////
  // This function is called by the Object Properties and Layer Control dialogs
  // when the "Material" button is pressed in the "Render" tab.  This is only called
  // if AppType returns RhinoApp::Render and EnableAssignMaterialButton return Rhino::rhTrue.
  virtual BOOL32 OnAssignMaterial( HWND hwndParent, ON_Material& material, CRhinoDoc& doc);
  //////////////////////////////////////////////////////////////////////////////////
  // Called to enable/disable the "Material" button located on the "Material" tab
  // in the Properties and Layer dialog boxes.  The default return value is false 
  // which will disable the button.  If the button is disabled then the OnAssignMaterial 
  // function is never called.
  virtual BOOL32 EnableAssignMaterialButton() const;
  ////////////////////////////////////////////////////////////////////////////////////////////
  // This function is called by the Object Properties and Layer Control dialogs
  // when the "Edit" button is pressed in the "Material" tab.  This is only called
  // if AppType returns RhinoApp::Render and EnableEditMaterialButton return Rhino::rhTrue.
  // A return value of Rhino::rhTrue means the material has been updated.
  virtual BOOL32 OnEditMaterial( HWND hwndParent, ON_Material &material, CRhinoDoc& doc);
  //////////////////////////////////////////////////////////////////////////////////
  // Called to enable/disable the "Edit" button located on the "Material" in the 
  // Properties and Layer dialog boxes.  The default return value is false 
  // which will disable the button.  If the button is disabled then the OnEditMaterial
  // function is never called.
  virtual BOOL32 EnableEditMaterialButton(ON_Material &material, CRhinoDoc& doc) const;
  ////////////////////////////////////////////////////////////////////////////////////////////
  // This function is called by the Object Properties and Layer Control dialogs
  // when the "New" button is pressed in the "Material" tab.  This is only called
  // if AppType returns RhinoApp::Render and EnableCreateMaterialButton return Rhino::rhTrue.
  virtual BOOL32 OnCreateMaterial(HWND hwndParent, ON_Material &material, CRhinoDoc& doc);
  //////////////////////////////////////////////////////////////////////////////////
  // Called to enable/disable the "New" button located on the "Material" in the 
  // Properties and Layer dialog boxes.  The default return value is false 
  // which will disable the button.  If the button is disabled then the OnEditMaterial
  // function is never called.
  virtual BOOL32 EnableCreateMaterialButton() const;
  //////////////////////////////////////////////////////////////////////////////////
  // Message sent from script to save file
  //////////////////////////////////////////////////////////////////////////////////
  virtual BOOL32 SaveRenderedImage( ON_wString wFileName);
  //////////////////////////////////////////////////////////////////////////////////
  // Close render window notification, called when rendering is done and render window
  // is no longer modal. (When RenderCommand returns if you leave the window up)
  //////////////////////////////////////////////////////////////////////////////////
  virtual BOOL32 CloseRenderWindow();
  //////////////////////////////////////////////////////////////////////////////////
  // This plug-in (has become)/(is no longer) the current renderer.
  //
  // If bCurrent then this plug-in has become the current renderer otherwise it
  // was, but is no longer, the current renderer.
  //////////////////////////////////////////////////////////////////////////////////
  virtual void OnSetCurrentRenderPlugIn( BOOL32 bCurrent);
  //////////////////////////////////////////////////////////////////////////////////
  // Called prior to adding the Rhino render tab to the FileProperites dialog.
  // This is only called when your render plug-in is the current render plug-in.
  // This function should return a pointer to a IRhinoOptionsPage derived 
  // object or NULL if you do not wish to replace the render tab.  You are 
  // responsible for deleting this pointers in your PostNcDestroy() override.
  //
  // Parameters:
  //   CRhinoDoc& [in] - Document to display properties for
  //   HWND       [in] - Main dialog HWND, parent for this page
  // Note:
  //   Make sure you include the following macro prior to calling new:
  //    AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //	 Please see MFC Technical Notes 33 and 58 for additional details.
  //////////////////////////////////////////////////////////////////////////////////
  virtual class IRhinoOptionsPage* RenderDialogPage( CRhinoDoc&, HWND);

public:
  // This will be used to extend this class without breaking the SDK
  virtual LRESULT RenderPlugInProc( UINT message, WPARAM wParam, LPARAM lParam);
private:
  ON__UINT_PTR m_sdk_reserved = 0;
};
