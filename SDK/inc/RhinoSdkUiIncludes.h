/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

class CRhinoUiDockBar;

// These classes do not use MFC
#include "rhinoSdkUiProfileContext.h"
#include "RhinoSdkUiProfileContextIterator.h"

#if defined (RHINO_SDK_MFC)
#include "RhinoSdkDib.h"
#include "RhinoSdkUiThemeDraw.h"
//#include "rhinoSdkUiBitmapButton.h"
//#include "rhinoSdkUiColorButton.h"
#include "RhinoSdkUiCheckBox.h"
#include "RhinoSdkUiImageList.h"
#include "rhinoSdkUiLayerListCtrl.h"
#include "RhinoSdkUiLayerComboBox.h"
#include "RhinoSdkUiColorComboBox.h"
#include "RhinoSdkUiColorListBox.h"
#include "RhinoSdkUiDirDialog.h"
#include "RhinoSdkUiFileDialog.h"
#include "RhinoSkdUiFontComboBox.h"
#include "RhinoSdkUiSliderCtrl.h"
#include "RhinoSdkUiGradientSliderCtrl.h"
#include "RhinoSdkUiHyperLink.h"
#include "RhinoSdkUiInPlaceCtrls.h"
#include "RhinoSdkUiListBoxEx.h"
#include "RhinoSdkUiMemDC.h"
#include "RhinoSdkUiPopUpMenu.h"
#include "RhinoSdkUiPopuUpListBox.h"
#include "RhinoSdkUiPopUpListWnd.h"
#include "RhinoSdkUiProgressCtrl.h"
#include "RhinoSdkUiProgressBar.h"
#include "RhinoSdkUiRichEditCtrl.h"
#include "RhinoSdkUiStatic.h"
#include "RhinoSdkUiSeparator.h"
#include "RhinoSdkUiGridListCombo.h"
#include "RhinoSdkUiGridListEdit.h"
#include "RhinoSdkUiGridListHeaderCtrl.h"
#include "RhinoSdkUiGridListCtrl.h"
#include "RhinoSdkUiMultiMonitor.h"
#include "RhinoSdkUiRegistry.h"
#include "RhinoSdkUiResourceManager.h"
#include "RhinoSdkUiWindowsPaintManager.h"
#include "RhinoSdkUiComboBox.h"
#include "RhinoSdkUiDragBitmapWnd.h"
#include "RhinoSdkUiEdit.h"
#include "RhinoSdkUiPointEdit.h"
#include "RhinoSdkUiDialogItemResizer.h"
#include "RhinoSdkUiDialog.h"
#include "RhinoSdkUiTabCtrlPage.h"
#include "RhinoSdkUiTabCtrl.h"
#include "rhinoSdkUiTabPageCtrl.h"
#include "RhinoSdkUiDHtmlDialog.h"
#include "RhinoSdkUiExpandableTabCtrl.h"
#include "RhinoSdkUiExpandableTabPage.h"
#include "RhinoSdkUiDockBarCaptionButton.h"
#include "RhinoSdkUiDockBarClientWnd.h"
#include "RhinoSdkUiDockBarDialog.h"
#include "RhinoSdkUiTabbedSheetCtrl.h"
#include "RhinoSdkUiToolTip.h"
#include "RhinoSdkUiButton.h"
#include "RhinoSdkUiBitmapButton.h"
#include "RhinoSdkUiColorButton.h"
#include "RhinoSdkUiDockBar.h"
#include "RhinoSdkUiDockBarManager.h"
#include "RhinoSdkUiDockBarExpandableTabCtrl.h"
#include "RhinoSdkUiExpandableTabbedDockBar.h"
#include "RhinoSdkUiPageDockBarManager.h"
#include "RhinoSdkUiManager.h"
#include "RhinoSdkUiUtilities.h"
#include "RhinoSdkUiLineTypeListCtrl.h"
#include "RhinoSdkUiPrintWidthListCtrl.h"
#include "RhinoSdkUiAutocomplete.h"
#include "RhinoSdkUiOptionsListCtrlItem.h"
#include "RhinoSdkUiOptionsListCtrl.h"
// The next 2 files are provided for backwards compatibility only
// New code should use CRhinoSdkUiEdit
#include "RhinoSdkUiEditInt.h"
#include "RhinoSdkUiEditReal.h"
#include "RhinoSdkUiBlockPreview.h"
#include "RhinoSdkUiCheckListBox.h"
#endif

// END - From RMA MFC UI Lib
