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

#if defined (RHINO_SDK_MFC)

class RHINO_SDK_CLASS CArgsRhinoPrintExportDlg
{
public:
  CArgsRhinoPrintExportDlg() = default;

  bool m_force_vector = false; //dialog should show previews using GDI
  bool m_display_page_viewscale = true;
  bool m_display_resolution_controls = true;
  bool m_display_page_objectscale = true;
  bool m_display_page_margins = true;
  bool m_display_page_show = true;
  //   purpose: reason the print dialog is being displayed
  //   info: initial print info settings for dialog
  //   mode:  0 = single page
  //          1 = multi-page
  //          2 = all pages
  //         -1 = single page with no multi-page controls
  int  m_mode = -1;

  ON_wString m_custom_page_title;
  UINT m_custom_page_id = 0;
  CRuntimeClass* m_pCustomPageClass = nullptr;
  AFX_MODULE_STATE* m_pCustomPageMS = nullptr;
};

// Description:
//   Show the Print Dialog for file export purposes
// Parameters:
//   info - [in/out] initial passed in print settings. These settings are
//          adjusted by the dialog
// Returns:
//   true if user clicked OK button, false if user clicked cancel
RHINO_SDK_FUNCTION bool RhinoPrintExportDialog( CRhinoPrintInfo& info,
                                                const CArgsRhinoPrintExportDlg& args);

// Description:
//   Show the Print Dialog for file export purposes
// Parameters:
//   info -            [in/out] initial passed in print settings. These settings are
//                     adjusted by the dialog
//   bDontShowDialog - [in/out] checkbox value for 
//                     "Always use these settings and do not show this dialog."
// Returns:
//   true if user clicked OK button, false if user clicked cancel
RHINO_SDK_FUNCTION bool RhinoPrintExportDialog( CRhinoPrintInfo& info,
                                                const CArgsRhinoPrintExportDlg& args,
                                                bool& bDontShowDialog);

// Description:
//   Show the Print Dialog for file export purposes
// Parameters:
//   info - [in/out] initial passed in print settings. These settings are
//          adjusted by the dialog
// Returns:
//   true if user clicked OK button, false if user clicked cancel
RHINO_SDK_FUNCTION bool RhinoPrintExportDialog(const CRhinoPrintInfo& initialinfo,
  const CArgsRhinoPrintExportDlg& args, ON_ClassArray<CRhinoPrintInfo>& settings);

class RHINO_SDK_CLASS CRhinoPrintControlsBase : public CRhinoUiExpandableTabPage_New
{
	DECLARE_DYNAMIC(CRhinoPrintControlsBase)
public:
  CRhinoPrintControlsBase(UINT nIDTemplate, CWnd* pParent);

protected:
  void AdjustRightEdge(int cx, int cy, CWnd& control);
  bool GetPrintInfo( CRhinoPrintInfo& info ) const;
  bool SetPrintInfo( const CRhinoPrintInfo& info );

  // Gets called when the print dialog gets a BroadcastPrintSettingsChanged call
  // Override to update controls on this page to match the new settings
  virtual void OnPrintSettingsChanged();

  void UpdatePreviewWindow();
  virtual BOOL32 OnInitDialog();
  virtual void DoDataExchange(CDataExchange* pDX);

  afx_msg LRESULT PrintSettingsChangeMesssage(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

protected:
  //Parent Dialog that contains all of the "meat" these controls make changes to
  class CRhPrintDialog* m_pPrintDialog;
};

#endif
