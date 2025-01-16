//
// Copyright (c) 1993-2015 Robert McNeel & Associates. All rights reserved.
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

class RHINO_SDK_CLASS CRhinoUiResourceManager
{
public:
  CRhinoUiResourceManager() = default;
  virtual ~CRhinoUiResourceManager(void);

  enum cursors
  {
    cursor_tb_shift = 0,
    cursor_tb_ctrl,
    cursor_tb_shift_ctrl,
    cursor_tb_move,
    cursor_tb_link,
    cursor_tb_copy,
    cursor_tb_delete
  };
  virtual HCURSOR GetCursor( cursors) = 0;

  enum images
  {
    image_tb_tt_lr = 0, // Tool bar tool tip, left and right mouse images.
                        // left should be image index 0, right 1
    image_layer_manager_state,
    image_layer_manager_state_empty,
    image_layer_manager_image,
  };
  virtual bool GetImageList( images, CRhinoUiImageList&) = 0;
  virtual COLORREF GetImageMaskingColor() = 0;

  enum bitmaps
  {
    bmp_tb_drag_not = 0,
    bmp_tb_drag_del,
  };
  virtual bool GetBitmap( bitmaps, CRhinoDib&) = 0;

  HCURSOR CreateTextCursor( HCURSOR hCursorTemplate,
                            ON_wString wCustomText,
                            bool bBlackTextOnWhiteBkgrnd = true,
                            int iTextLeft = 4,
                            int iTextTop = 1,
                            LPRECT lpTextWrittenToRect = NULL
                          );
};
