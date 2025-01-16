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

//----------------------------------------------------------------
// Map old clas names to new ones
#define CRmaUiBitmapButton CRhinoUiBitmapButton
#define CRmaUiCheckBox CRhinoUiCheckBox
#define CRmaUiColorButton CRhinoUiColorButton
#define CRmaUiColorComboBox CRhinoUiColorComboBox
#define CRmaUiColorComboBoxItem CRhinoUiColorComboBoxItem
#define CRmaUiColorListBox CRhinoUiColorListBox
#define CRmaUiDialog CRhinoUiDialog
#define CRmaUiDialogItemResizer CRhinoUiDialogItemResizer
#define CRmaUiDialogSub CRhinoUiDialogSub
#define CRmaUiDirDialog CRhinoUiDirDialog
#define CRmaUiEditInt CRhinoUiEditInt
#define CRmaUiEditReal CRhinoUiEditReal
#define CRmaUiFontComboBox CRhinoUiFontComboBox
#define CRmaUiGradientSlider CRhinoUiGradientSliderCtrl
#define CRmaUiGridListCombo CRhinoUiGridListCombo
#define CRmaUiGridListCtrl CRhinoUiGridListCtrl
#define CRmaUiGridListEdit CRhinoUiGridListEdit
#define CRmaUiGridListHeaderCtrl CRhinoUiGridListHeaderCtrl
#define CRmaUiHyperlink CRhinoUiHyperlink
#define CRmaUiImageList CRhinoUiImageList
#define CRmaUiInPlace CRhinoUiInPlace
#define CRmaUiInPlaceButton CRhinoUiInPlaceButton
#define CRmaUiInPlaceEdit CRhinoUiInPlaceEdit
#define CRmaUiListBoxEx CRhinoUiListBoxEx
#define CRmaUiListBoxExBuddy CRhinoUiListBoxExBuddy
#define CRmaUiMemDC CRhinoUiMemDC
#define CRmaUiPopupMenu CRhinoUiPopupMenu
#define CRmaUiPopuUpListBox CRhinoUiPopuUpListBox
#define CRmaUiPopUpListWnd CRhinoUiPopUpListWnd
#define CRmaUiProgressBar CRhinoUiProgressBar
#define CRmaUiRichEditCtrl CRhinoUiRichEditCtrl
#define CRmaUiSeparator CRhinoUiSeparator
#define CRmaUiStatic CRhinoUiStatic
#define CRmaUiTextProgressCtrl CRhinoUiProgressCtrl
#define CRmaUiThemeDraw CRhinoUiThemeDraw
//----------------------------------------------------------------
// RmaDib specific
#define CRmaDib CRhinoDib
#define RmaDibInstallMemManager RhinoUiDibInstallMemManager
#define RMADIB_CALLOC RHINOUI_DIB_CALLOC
#define RMADIB_MALLOC RHINOUI_DIB_MALLOC
#define RMADIB_FREE   RHINOUI_DIB_FREE
#define DIBCopy RhinoUiDIBCopy
#define DIBColorDepth RhinoUiDIBColorDepth
#define DIBNumColors RhinoUiDIBNumColors
#define DIBPaletteSize RhinoUiDIBPaletteSize
#define DIBWidth RhinoUiDIBWidth
#define DIBHeight RhinoUiDIBHeight
#define DIBLineSize RhinoUiDIBLineSize
#define DIBSizeBits RhinoUiDIBSizeBits
#define DIBSize RhinoUiDIBSize
#define DIBCreate RhinoUiDIBCreate
#define DIBFindBits RhinoUiDIBFindBits
#define DIBCreateHBitmap RhinoUiDIBCreateHBitmap
#define DIBScanWidth RhinoUiDIBScanWidth
#define DIBGet256Palette RhinoUiDIBGet256Palette
#define DIBSet256PalColors RhinoUiDIBSet256PalColors
#define DIBSet256IdentityPalette RhinoUiDIBSet256IdentityPalette
#define DIBCopyToHandle RhinoUiDIBCopyToHandle
#define DIBCopyToClipboard RhinoUiDIBCopyToClipboard
