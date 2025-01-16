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

struct RHINO_SDK_CLASS RhinoUiColorComboBoxItemData
{
  COLORREF m_cr;
  bool m_display_color_dialog;
};
void RHINO_SDK_FUNCTION RhinoUiColorComboBoxItemDataInit( RhinoUiColorComboBoxItemData& data);

struct RHINO_SDK_CLASS RhinoUiLayerComboBoxItemData
{
  COLORREF m_cr;
  UUID m_uuid;
};
void RHINO_SDK_FUNCTION RhinoUiLayerComboBoxItemDataInit( RhinoUiLayerComboBoxItemData& data);

struct RHINO_SDK_CLASS RhinoUiLinetypeComboBoxItemData
{
  UUID m_uuid; // Linetype ID
};
void RHINO_SDK_FUNCTION RhinoUiLinetypeComboBoxItemDataInit( RhinoUiLinetypeComboBoxItemData& data);

struct RHINO_SDK_CLASS RhinoUiPrintWidthComboBoxItemData
{
  double m_width;
};
void RHINO_SDK_FUNCTION RhinoUiPrintWidthComboBoxItemDataInit( RhinoUiPrintWidthComboBoxItemData& data);

struct RhinoUiFontComboBoxItemData
{
  LOGFONT m_lf;
};

struct RhinoUiUuidComboBoxItemData
{
  UUID m_uuid;
};

void RHINO_SDK_FUNCTION RhinoUiFontComboBoxItemDataInit( RhinoUiFontComboBoxItemData& data);

class RHINO_SDK_CLASS CRhinoUiComboBoxItemData
{
public:
  //
  // Valid CRhinoUiComboBox item data types
  //
  enum class_type
  {
    ct_font = 0, // Font item, m_data.m_font property will be valid
    ct_color,    // Color item, m_data.m_color property will be valid
    ct_layer,    // Uses m_data.m_color for drawing layer color swatch
    ct_linetype,
    ct_print_width,
    ct_strings,  // Generic item, neither font nor color items will be valid
    ct_uuid,     // An ON_UUID
    ct_invalid,  // For internal use, should always be last item in list
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_class_type = 0xFFFFFFFF
  };
  //
  // Description:
  //   Constructor
  //
  // Parameters:
  //   ct [in] Type of data this object will contain.  Tells constructor which property
  //           to initialize
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //
  CRhinoUiComboBoxItemData( class_type ct);
  //
  // Description:
  //   Destructor
  //
  virtual ~CRhinoUiComboBoxItemData();
  //
  // Description:
  //   Identifies what type of data this object contains.
  //
  // Returns:
  //   CRhinoUiComboBoxItemData::class_type which identifies type of data this object
  //   contains.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //
  class_type ClassType() const;
  //
  // Description:
  //   Display text associated with this item.  This text will be used when owner drawing
  //   items when the combo box does not include CBS_HASSTRINGS style.
  //
  // Returns:
  //   Text to owner draw.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::SetText()
  //
  LPCTSTR Text() const;
  //
  // Description:
  //   Set text associated with this item.  This text will be used when owner drawing
  //   items when the combo box does not include CBS_HASSTRINGS style.
  //
  // Parameters:
  //   lps [in] New text value.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::Text()
  //
  void SetText( LPCTSTR lps);
  //
  // Description:
  //   Get color data associated with this object.  This will only be valid if ClassType()
  //   returns ct_color.
  //
  // Returns:
  //   Pointer to RhinoUiColorComboBoxItemData struct if ClassType() returns ct_color otherwise
  //   NULL.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //   RhinoUiColorComboBoxItemData
  //
  RhinoUiColorComboBoxItemData* ColorData();
  //
  // Description:
  //   Get layer data associated with this object.  This will only be valid if ClassType()
  //   returns ct_layer.
  //
  // Returns:
  //   Pointer to RhinoUiLayerComboBoxItemData struct if ClassType() returns ct_layer otherwise
  //   NULL.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //   RhinoUiLayerComboBoxItemData
  //
  RhinoUiLayerComboBoxItemData* LayerData();
  //
  // Description:
  //   Get linetype data associated with this object.  This will only be valid if ClassType()
  //   returns ct_linetype.
  //
  // Returns:
  //   Pointer to RhinoUiLinetypeComboBoxItemData struct if ClassType() returns ct_linetype otherwise
  //   NULL.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //   RhinoUiLinetypeComboBoxItemData
  //
  RhinoUiLinetypeComboBoxItemData* LinetypeData();
  //
  // Description:
  //   Get print width data associated with this object.  This will only be valid if ClassType()
  //   returns ct_print_width.
  //
  // Returns:
  //   Pointer to RhinoUiPrintWithComboBoxItemData struct if ClassType() returns ct_print_width otherwise
  //   NULL.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //   RhinoUiPrintWidthComboBoxItemData
  //
  RhinoUiPrintWidthComboBoxItemData* PrintWidthData();
  //
  // Description:
  //   Get font data associated with this object.  This will only be valid if ClassType()
  //   returns ct_font.
  //
  // Returns:
  //   Pointer to RhinoUiFontComboBoxItemData struct if ClassType() returns ct_font otherwise
  //   NULL.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //   RhinoUiFontComboBoxItemData
  //
  RhinoUiFontComboBoxItemData* FontData();
  //
  // Description:
  //   Get UUID associated with this object.  This will only be valid if ClassType()
  //   returns ct_uuid.
  //
  // Returns:
  //   The stored uuid if ClassType() returns ct_uuid otherwise NULL.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //
  ON_UUID UuidData() const;
  //
  // Description:
  //   Store uuid on this item data.0
  //   Sets class type to ct_uuid.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //
  void SetUuidData(ON_UUID uuid);

public:
  //
  // Since this object is attached to combo box items using CComboBox::SetItemDataPtr()
  // the next to properties can be used to attach additional data to item.
  //
  void* m_pUserData;
  LONG m_lUserData;

private:
  friend class CRhinoUiComboBox;
#if defined (ON_RUNTIME_WIN)
  union
#else
  struct
#endif
  {
    RhinoUiColorComboBoxItemData m_color;
    RhinoUiLayerComboBoxItemData m_layer;
    RhinoUiFontComboBoxItemData m_font;
    RhinoUiLinetypeComboBoxItemData m_linetype;
    RhinoUiPrintWidthComboBoxItemData m_print_width;
    ON_UUID m_uuid;
  } m_data;
  CString m_sText;
  int m_ct;
};

class RHINO_SDK_CLASS CRhinoUiComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CRhinoUiComboBox)

public:
  //
  // Description:
  //   Constructor
  //
  CRhinoUiComboBox();
  //
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiComboBox();
  //   
  // Description:
  //   Flag which tells control to ensure that it is owner draw when it is created so that
  //   font and color previews are visible.
  //
  // Returns:
  //   If true then combo box style will be modified to include CBS_OWNERDRAW fixed if not
  //   owner draw when created.
  //
  // See Also:
  //   CRhinoUiComboBoxItem::SetCreateAsOwnerDraw()
  //   
  bool CreateAsOwnerDraw() const;
  //   
  // Description:
  //   Set flag which tells control to ensure that it is owner draw when it is created so that
  //   font and color previews are visible.
  //
  // Parameters:
  //   b [in] New value for create flag.
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiComboBoxItem::CreateAsOwnerDraw()
  //   
  bool SetCreateAsOwnerDraw( bool b);
  // These styles are only applied to owner draw versions of the combo box that include color swatches
  // such as Color combos and Layer combos
  enum color_swatch_draw_style
  {
    square_swatch = 0,
    diamond_swatch,
    circle_swatch,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_image_list = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Call this method to determine how color swatches are drawn in owner draw versions of the combo box
  //   that include a color swatch.
  //
  // Returns:
  //   Returns color swatch draw style, see CRhinoUiComboBoxItem::color_swatch_draw_style for options. 
  //
  // See Also:
  //   CRhinoUiComboBoxItem::color_swatch_draw_style
  //   CRhinoUiComboBoxItem::ColorSwatchDrawStyle()
  //   
  color_swatch_draw_style ColorSwatchDrawStyle() const;
  //   
  // Description:
  //   Call this method to control how color swatches are drawn in owner draw versions of the combo box
  //   that include a color swatch.
  //
  // Parameters:
  //   style [in] New swatch draw style, see CRhinoUiComboBoxItem::color_swatch_draw_style for options. 
  //
  // Returns:
  //   Returns previous value.
  //
  // See Also:
  //   CRhinoUiComboBoxItem::color_swatch_draw_style
  //   CRhinoUiComboBoxItem::ColorSwatchDrawStyle()
  //   
  color_swatch_draw_style SetColorSwatchDrawStyle( color_swatch_draw_style style);
  //   
  // Description:
  //   Get text associated with a combo box list item. First checks to see if list box 
  //   contains a string, if it does then then the string is returned.  If no string
  //   or an empty string is found at iIndex then checks associated CRhinoUiComboBoxItemData
  //   pointer associated with iIndex. If a CRhinoUiComboBoxItemData* item is associated with
  //   iIndex then CRhinoUiComboBoxItemData::Text() is returned.
  //
  // Parameters:
  //   iIndex [in] Zero based index of string in list box
  //   sResult [out] CString object to receive result
  //
  // Returns:
  //   If string is associated with item and value paced in sResult true is returned otherwerise
  //   false is returned.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::Text()
  //   
  bool GetTextFromListIndex( int iIndex, CString& sResult) const;
  //   
  // Description:
  //   Get item data pointer associated with list item.
  //
  // Parameters:
  //   iIndex [in] Zero based index of list item to check.
  //
  // Returns:
  //   Pointer to data item associated with iIndex.  Returns NULL if iIndex is invalid or there
  //   is no pointer associated with item at iIndex.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData
  //   CRhinoUiComboBox::ClassType()
  //   
  CRhinoUiComboBoxItemData* GetComboBoxItemData( int iIndex) const;
  //   
  // Description:
  //   Call this method to get the UUID returned when there is no current selection or and error condition
  //   was found.
  //
  // Returns:
  //   UUID associated with error condition or no selection
  //
  static UUID ListBoxErrorUUID();
  //-------------------------------------------------------------------------------------------------------
  // Color combo UI
  //-------------------------------------------------------------------------------------------------------
  //
  // Description:
  //   Populate combo box using base Rhino colors.  If bIncludeByLayerColor is true then the
  //   first item in the list is "By Layer".  If bIncludeCustomColor is true then the last
  //   item is "Other" which displays color dialog when selected.
  //
  //   Standard color list:
  //      By Layer
  //      Black
  //      White
  //      Red
  //      Yellow
  //      Green
  //      Cyan
  //      Blue
  //      Magenta
  //      Other
  //
  //   To see color swatches you must either use the  CBS_OWERDRAWFIXED style or set m_bMakeOwnerDraw prior 
  //   to creating this control.
  //
  // Parameters:
  //   bIncludeByLayerColor [in] Include "By Layer" option as first item in list.
  //   bIncludeByParentColor [in] Include "By Parent" option as second item in list.
  //   bIncludeCustomColor [in] Include "Other" option as last item in list.  If "Other" is
  //                            selected a color dialog appears.
  //
  // Returns:
  //   If list is filled correctly then true is returned otherwise false is returned indicating
  //   there was an error.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData::class_type
  //   CRhinoUiComboBox::ClassType()
  //
  int FillUsingStandardColorList( bool bIncludeByLayerColor = true,
                                  bool bIncludeCustomColor = true,
                                  bool bIncludeByParentColor = true,
                                  bool bIncludeByDisplayColor = false
                                );
  //   
  // Description:
  //   Used to add a color item to list. To see the font names you must either use the 
  //   CBS_OWERDRAWFIXED style or set m_bMakeOwnerDraw prior to creating this control.  //
  // Parameters:
  //   lpsColorName [in] Display text associated with color
  //   color [in] Color returned when item is selected and drawn when combo box is visible
  //   link_to_color_dialog [in] If this is true then color dialog will be displayed when
  //                             item is selected.
  // Returns:
  //   Zero or greater which is list index of added item or -1 on error.
  //
  // See Also:
  //   CRhinoUiComboBox::ColorFromListIndex()
  //
  int AddColorItem( LPCTSTR lpsColorName, COLORREF color, bool link_to_color_dialog = false);
  //   
  // Description:
  //   Get color associated with combo box item.
  //   CRhinoUiComboBoxItemData::ct_color.
  //
  // Parameters:
  //   iIndex [in] Index of item to get color for.
  //   crError = color to be returned in case of error.
  //
  // Returns:
  //   Color associated with list item or crError on error
  //
  // See Also:
  //   CRhinoUiComboBox::AddColorItem()
  //
  COLORREF ColorFromListIndex( int iIndex, COLORREF crError = CRhinoUiPaintManager::error_color) const;
  //   
  // Description:
  //   Get string associated with color value.
  //
  // Parameters:
  //   cr [in] Color to check for.
  //
  // Returns:
  //   Combo box string associated with RGB value
  //
  // See Also:
  //   CRhinoUiComboBox::AddColorItem()
  //
  ON_wString ColorNameFromColorRef( COLORREF cr);
  //   
  // Description:
  //   Check color to see if it is By Layer color.
  //
  // Parameters:
  //   cr [in] Color to check
  //
  // Returns:
  //   Returns true if cr is equal to CRhinoUiPaintManager::bylayer_color indicating that cr
  //   is an invalid RGB value.
  //
  // See Also:
  //   CRhinoUiPaintManager::bylayer_color
  //   CRhinoUiComboBox::ColorFromListIndex()
  //   
  bool ColorIsByLayer( COLORREF cr) const;
  //   
  // Description:
  //   Check color to see if it is By Parent color.
  //
  // Parameters:
  //   cr [in] Color to check
  //
  // Returns:
  //   Returns true if cr is equal to CRhinoUiPaintManager::byparent_color indicating that cr
  //   is an invalid RGB value.
  //
  // See Also:
  //   CRhinoUiPaintManager::byparent_color
  //   CRhinoUiComboBox::ColorFromListIndex()
  //   
  bool ColorIsByParent( COLORREF cr) const;
  //   
  // Description:
  //   Get color associated with current selection.
  //
  // Returns:
  //   Returns RGB value associated with currently selected item.  If no item is selected then
  //   CRhinoUiPaintManager::error_color is returned.
  //
  // See Also:
  //   CRhinoUiComboBox::ColorFromListIndex()
  //   CRhinoUiComboBox::ColorIsByLayer()
  //   
  COLORREF GetCurColor() const;
  //   
  // Description:
  //   Get color associated with current selection and return the current selection index.
  //
  // Returns:
  //   Returns LB_ERROR if nothing is selected otherwise; zero based index of the curret
  //   list item and sets color equal to the selected color.
  //
  // See Also:
  //   CRhinoUiComboBox::ColorFromListIndex()
  //   CRhinoUiComboBox::ColorIsByLayer()
  //   CRhinoUiComboBox::GetCurColor()
  //   
  int GetCurColor( COLORREF& color) const;
  //   
  // Description:
  //   Get color associated with current selection and return the current selection index.
  //
  // Returns:
  //   Returns LB_ERROR if nothing is selected otherwise; zero based index of the curret
  //   list item and sets color equal to the selected color.
  //
  // See Also:
  //   CRhinoUiComboBox::ColorFromListIndex()
  //   CRhinoUiComboBox::ColorIsByLayer()
  //   CRhinoUiComboBox::GetCurColor()
  //   
#if defined (ON_RUNTIME_WIN)
  // duplicate definition on OS X
  int GetCurColor( ON_Color& color) const;
#endif
  //   
  // Description:
  //   Find list item with specified color and set it as currently selected item.
  //
  // Parameters:
  //   cr [in] Check each item in list for color that matches and make the first
  //           one found current selection.
  //
  // Returns:
  //   Color of previously selected item or CRhinoUiPaintManager::error_color on error.
  //
  // See Also:
  //   CRhinoUiPaintManager::error_color
  //   CRhinoUiComboBox::ColorFromListIndex()
  //   CRhinoUiComboBox::ColorIsByLayer()
  //   
  COLORREF SetCurColor( COLORREF cr);
  bool SetCustomColor( COLORREF cr);
  //   
  // Description:
  //   Called when color dialog is closed by pressing the OK button.
  //
  // Parameters:
  //   cr [in] Color returned by color dialog.
  //   iItemIndex [in] List index for item that caused the color dialog to be displayed.
  //
  virtual void OnCustomColorChanged( COLORREF cr, int iItemIndex);
  //   
  // Description:
  //   This method is called when adding color items to the combo box list box.  Override
  //   this method if you want to derive a class from CRhinoUiComboBoxItemData and provide
  //   additional data members and methods.
  //
  // Parameters:
  //   ct [in] type of control data being requested.
  //
  // See Also:
  //   CRhinoUiComboBoxItemData
  //   CRhinoUiComboBoxItemData::class_type
  //
  virtual CRhinoUiComboBoxItemData* NewCRhinoUiComboBoxItemData( CRhinoUiComboBoxItemData::class_type ct) const;
  //-------------------------------------------------------------------------------------------------------
  // Layer combo is a modified color combo
  //-------------------------------------------------------------------------------------------------------
  //
  // Description:
  //   Populate list control with layers associated with active CRhinoDoc.  To see the layer color swatches
  //   you must either use the CBS_OWERDRAWFIXED style or set m_bMakeOwnerDraw prior to creating this control.
  //
  // Parameters:
  //   doc [in] Document containing layer table to read
  //   bUseSortedList [in] If true then use the default layer list returned by CRhinoLayerTable::GetSortedList()
  //   compare [in] Only valid if bUseSortedList is false. If so and compare is not NULL then
  //                CRhinoLayerTable::Sort() is called using the compare parameter prior to calling 
  //                CRhinoLayerTable::GetSortedList().
  //   cull [in] Only valid if bUseSortedList is false. If so and cull is not NULL then
  //                CRhinoLayerTable::Sort() is called using the cull parameter prior to calling 
  //                CRhinoLayerTable::GetSortedList().
  //   p [in] Passed to CRhinoLayerTable::Sort() if it is called
  // Returns:
  //   Returns number of layers added to list box.
  //
  // See Also:
  //    class CRhinoLayerTable
  //    CRhinoLayerTable::Sort()
  //    CRhinoLayerTable::GetSortedList()
  //   
  int FillLayerList( CRhinoDoc* doc,
                     bool bUseSortedList,
                     int (*compare)(const CRhinoLayer*,const CRhinoLayer*,void*) = NULL,
                     int (*cull)(const CRhinoLayer*,void*) = NULL,
                     void* p=0
                   );
  //   
  // Description:
  //   Add layer name and color to list.  Item is added as CRhinoUiComboBoxItemData::ct_color
  //   data type.
  //
  // Parameters:
  //   layer [in] Layer to add to list.
  //
  // Returns:
  //   Zero based index of new item in list or -1 on error.
  //
  // See Also:
  //   CRhinoUiCombo::FillLayerList()
  //   
  int AddLayer( const CRhinoLayer& layer);
  //   
  // Description:
  //   Get layer index associated with current selection.
  //
  // Returns:
  //   Returns a RhinoUiLayerComboBoxItemData pointer associated with current selection or
  //   NULL if none is found.
  //
  // See Also:
  //   CRhinoUiComboBox::FillLayerList()
  //   CRhinoUiComboBox::SetCurLayer()
  //   
  const RhinoUiLayerComboBoxItemData* GetCurLayer() const;
  //   
  // Description:
  //   Find list item with specified layer index and set it as currently selected item.
  //
  // Parameters:
  //   uuid [in] Check each item in list for a layer UUID that matches and make
  //   the first one found the current selection.
  //
  // Returns:
  //   Layer UUID of previously selected item or ListBoxErrorUUID() if there was
  //   no previous selection.
  //
  // See Also:
  //   CRhinoUiComboBox::GetCurLayer()
  //   CRhinoUiComboBox::FillLayerList()
  //   
  UUID SetCurLayer( UUID uuid, COLORREF cr);
  //   
  // Description:
  //   Get layer index associated with combo box item.
  //
  // Parameters:
  //   iIndex [in] Index of combo box item to get the layer index for.
  //
  // Returns:
  //   Layer index associated with comob box item or -1 on error
  //
  // See Also:
  //   CRhinoUiComboBox::AddLayer()
  //   CRhinoUiComboBox::LayerUUIDFromListIndex()
  //
  UUID LayerUUIDFromListIndex( int iIndex) const;
  //   
  // Description:
  //   Call this method to get the list box index associated with the specified layer UUID
  //
  // Parameters:
  //   uuid [in] UUID of layer to find
  //
  // Returns:
  //   List box index for matching item or -1 if not found
  //
  // See Also:
  //   CRhinoUiComboBox::AddLayer()
  //   CRhinoUiComboBox::LayerIndexFromListIndex()
  //
  int ListIndexFromLayerUUID( const UUID& uuid) const;
  //-------------------------------------------------------------------------------------------------------
  // Linetype combo UI
  //-------------------------------------------------------------------------------------------------------
  //   
  // Description:
  //   Call this method to get the UUID associated with the "Continuous" linetype item in this combo box.
  //
  // Returns:
  //   UUID associated with "Continuous" comob box item
  //
  static UUID ContinuousLinetypeID();
  //   
  // Description:
  //   Call this method to get the UUID associated with the "By Layer" linetype item in this combo box.
  //
  // Returns:
  //   UUID associated with "By Layer" comob box item
  //
  static UUID ByLayerLinetypeID();
  //   
  // Description:
  //   Call this method to get the UUID associated with the "By Parent" linetype item in this combo box.
  //
  // Returns:
  //   UUID associated with "By Parent" comob box item
  //
  static UUID ByParentLinetypeID();
  //
  // Description:
  //   Populate list control with linetypes associated with the specified CRhinoDoc.
  //
  // Parameters:
  //   doc [in] Document containing linetype table to read
  //   bUseSortedList [in] If true then use the default linetype returned by CRhinoLinetypeTable::GetSortedList();
  //                       otherwise iterate the linetype table and apply the compare and cull methods is specified.
  //   bIncludeByLayer [in] If true then "By Layer" option is added to the top of the list
  //   bIncludeByParent [in] If true then "By Parent" option is added to the top of the list
  //   bReadDefaultLinetypeFile [in] If true then the default linetype table is read and linetypes are added to the
  //                                 bottom of the list.
  //   compare [in] Only valid if bUseSortedList is false. If so and compare is not NULL then
  //                CRhinoLinetypeTable::Sort() is called using the compare parameter prior to calling 
  //                CRhinoLinetypeTable::GetSortedList().
  //   cull [in] Only valid if bUseSortedList is false. If so and cull is not NULL then
  //                CRhinoLinetypeTable::Sort() is called using the cull parameter prior to calling 
  //                CRhinoLinetypeTable::GetSortedList().
  //   p [in] Passed to CRhinoLinetypeTable::Sort() if it is called
  // Returns:
  //   Returns number of layers added to list box.
  //
  // See Also:
  //    class CRhinoLayerTable
  //    CRhinoLayerTable::Sort()
  //    CRhinoLayerTable::GetSortedList()
  //   
  int FillLinetypeList( CRhinoDoc* doc,
                        bool bUseSortedList,
                        bool bIncludeByLayer,
                        bool bIncludeByParent,
                        bool bReadDefaultLinetypeFile,
                        int (*compare)(const CRhinoLinetype*,const CRhinoLinetype*,void*) = NULL,
                        int (*cull)(const CRhinoLinetype*,void*) = NULL,
                        void* p=NULL
                      );
  //   
  // Description:
  //   Add linetype to the list box.  Item is added as CRhinoUiComboBoxItemData::ct_linetype
  //   data type.
  //
  // Parameters:
  //   linetype [in] Linetype to add to list.
  //   bReadFromFile [in] Linetype is not currently in the document linetype table, it is 
  //                      a default linetype or was read from a linetype definition file.
  //
  // Returns:
  //   Zero based index of new item in list or -1 on error.
  //
  // See Also:
  //   CRhinoUiCombo::FillLinetypeList()
  //   
  int AddLinetype( const ON_Linetype& linetype);
  //   
  // Description:
  //   Get linetype index associated with current selection.
  //
  // Returns:
  //   Returns linetype index for the currently selected linetype.  If no item is selected then
  //   -1 is returned.
  //
  // See Also:
  //   CRhinoUiComboBox::FillLinetypeList()
  //   CRhinoUiComboBox::SetCurLinetypeIndex()
  //   
  const RhinoUiLinetypeComboBoxItemData* GetCurLinetype() const;
  //   
  // Description:
  //   Find list item with specified linetype index and set it as currently selected item.
  //
  // Parameters:
  //   iLinetypeIndex [in] Check each item in list for a linetype index that matches and make
  //   the first one found the current selection.
  //
  // Returns:
  //   Linetype index of previously selected item or -1 on error.
  //
  // See Also:
  //   CRhinoUiComboBox::GetCurLinetypeIndex()
  //   CRhinoUiComboBox::FillLinetypeList()
  //   
  UUID SetCurLinetype( UUID uuid);
  //   
  // Description:
  //   Get linetype information associated with combo box item.
  //
  // Parameters:
  //   iIndex [in] Index of combo box item to get the linetype index for.
  //
  // Returns:
  //   Linetype data associated with comob box item or NULL on error
  //
  // See Also:
  //   CRhinoUiComboBox::AddLinetype()
  //   CRhinoUiComboBox::LinetypeUUIDFromListIndex()
  //
  RhinoUiLinetypeComboBoxItemData* LinetypeDataFromListIndex( int iIndex) const;
  //   
  // Description:
  //   Get linetype UUID associated with combo box item.
  //
  // Parameters:
  //   iIndex [in] Index of combo box item to get the linetype uuid for.
  //
  // Returns:
  //   Linetype UUID associated with comob box item or ON_nil_uuid on error
  //
  // See Also:
  //   CRhinoUiComboBox::AddLinetype()
  //   CRhinoUiComboBox::LinetypeIndexFromListIndex()
  //
  UUID LinetypeUUIDFromListIndex( int iIndex) const;
  //   
  // Description:
  //   Call this method to get the list box index associated with the specified linetype UUID
  //
  // Parameters:
  //   uuid [in] UUID of linetype to find
  //
  // Returns:
  //   List box index for matching item or -1 if not found
  //
  // See Also:
  //   CRhinoUiComboBox::AddLinetype()
  //   CRhinoUiComboBox::LinetypeIndexFromListIndex()
  //
  int ListIndexFromLinetypeUUID( const UUID& uuid) const;
  //   
  // Description:
  //   Call this method to get the linetype associated with a item which was read from a linetype
  //   definition file.  When the combo box fills the list with these items it generates unique 
  //   UUIDs for each item and addes them to an internal list.
  //
  // Parameters:
  //   uuid [in] UUID of linetype to find
  //
  // Returns:
  //   Returns a pointer to the ON_Linetype which was read from a file or NULL if not found.
  //
  // See Also:
  //   CRhinoUiComboBox::FillLinetypeList()
  //
  const ON_Linetype* LinetypeReadFromFile( const UUID& uuiid) const;
  //-------------------------------------------------------------------------------------------------------
  // Print width UI
  //-------------------------------------------------------------------------------------------------------
  enum print_width_options
  {
    default_width   =   0,
    no_print_width  = - 1,
    by_layer_width  = -10,
    by_parent_width = -15,
    list_box_error_width = -100,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_plot_width_option = 0xFFFFFFFF
  };
  //
  // Description:
  //   Populate list control with linetypes associated with the specified CRhinoDoc.
  //
  // Parameters:
  //   bIncludeByLayer [in] If true then "By Layer" option is added to top of list; otherwise it is not
  //   bIncludeDefault [in] If true then "Default" option is added to top of list; otherwise it is not
  //   bIncludeByParent [in] If true then "By Parent" option is added to top of list; otherwise it is not
  //   bIncludeNoPrint [in] If true then "By Print" option is added to top of list; otherwise it is not
  //   bIncludeHairline [in] If true then "Hairline" option is added to top of list; otherwise it is not
  //
  // Returns:
  //   Returns number of widths added to list box.
  //
  // See Also:
  //    CRhinoUiComboBox::print_width_options
  //    CRhinoUiComboBox::GetPrintWidthDisplayString()
  //   
  int FillUsingStandardPrintWidths( bool bIncludeByLayer,
                                    bool bIncludeDefault,
                                    bool bIncludeByParent,
                                    bool bIncludeNoPrint,
                                    bool bIncludeHairline
                                  );
  //
  // Description:
  //   Create display string from width value.
  //
  // Parameters:
  //   cx [in] If cx is equal to one of the CRhinoUiComboBox::print_width_options then the appropriate
  //           display string is created. If cx is not a CRhinoUiComboBox::print_width_options and is
  //           less than 0.0 then a error string is generated.  If cx is >= 0.0 then cx is formatted as
  //           a string and returned.
  //   result [out] Buffer to receive the display string.
  //
  // Returns:
  //   If cx is equal to one of the CRhinoUiComboBox::print_width_options then the appropriate display
  //   string is copied to result and true is returned. If cx is not a CRhinoUiComboBox::print_width_options
  //   and is less than 0.0 then result will be an empty string and false will be returned.  If cx is >= 0.0
  //  then cx is formatted as a string, placed in result and true is returned.
  //
  // See Also:
  //    CRhinoUiComboBox::print_width_options
  //    CRhinoUiComboBox::FillUsingStandardWidths()
  //
  static bool GetPrintWidthDisplayString( double cx, ON_wString& result);
  //   
  // Description:
  //   Add a print width value to list.
  //
  // Parameters:
  //   cx [in] Width value to add to the list
  //
  // Returns:
  //   Zero based index of new item in list or -1 on error.
  //
  // See Also:
  //    CRhinoUiComboBox::print_width_options
  //    CRhinoUiComboBox::FillUsingStandardWidths()
  //
  int AddPrintWidthItem( double cx);
  //   
  // Description:
  //   Read default print widths from specified file.
  //
  // Parameters:
  //   lpsFileName [in] Name of file to read
  //   widths [out] Width values will be added to this list
  //   bQueitly [in] If this is true then this function will fail quietly otherwise a message box will
  //                 appear on error.
  //
  // Returns:
  //   Returns true if file was successfully opened and read.
  //
  // See Also:
  //    CRhinoUiComboBox::FillUsingStandardWidths()
  //
  static bool ReadPrintWidthFile( const wchar_t* lpsFileName, ON_SimpleArray<double>& widths, bool bQuietly);
  //   
  // Description:
  //   Get print width data associated with current selection.
  //
  // Returns:
  //   Returns print width data for the currently selected linetype.  If no item is selected then
  //   NULL is returned.
  //
  // See Also:
  //   CRhinoUiComboBox::FillUsingStandardWidths()
  //   CRhinoUiComboBox::SetCurPrintWidth()
  //   
  const RhinoUiPrintWidthComboBoxItemData* GetCurPrintWidth() const;
  //   
  // Description:
  //   Find list item with specified print width set it as currently selected item.
  //
  // Parameters:
  //   cx [in] Check each item in list for a print width that matches and make
  //   the first one found the current selection.
  //
  // Returns:
  //   Print width of previously selected item or list_box_error_width on error.
  //
  // See Also:
  //   CRhinoUiComboBox::GetCurLinetypeIndex()
  //   CRhinoUiComboBox::list_box_error_width
  //   
  double SetCurPrintWidth( double cx);
  //   
  // Description:
  //   Get plot width information associated with combo box item.
  //
  // Parameters:
  //   iIndex [in] Index of combo box item to get the plot width for.
  //
  // Returns:
  //   Plot width data associated with comob box item or NULL on error
  //
  // See Also:
  //   CRhinoUiComboBox::AddPlotWidth()
  //
  RhinoUiPrintWidthComboBoxItemData* PrintWidthDataFromListIndex( int iIndex) const;
  //   
  // Description:
  //   Call this method to get the list box index associated with the specified print width
  //
  // Parameters:
  //   uuid [in] UUID of print width to find
  //
  // Returns:
  //   List box index for matching item or -1 if not found
  //
  // See Also:
  //   CRhinoUiComboBox::AddPrintWidth()
  //   CRhinoUiComboBox::PrintWidthIndexFromListIndex()
  //
  int ListIndexFromPrintWidth( double cx) const;
  //   
  // Description:
  //   Call this method to convert the current string in the combo box edit control to a
  //   width value.
  //
  // Returns:
  //   Returns a CRhinoUiComboBox::print_width_options or valid width value on success or
  //   list_box_error_width on failure.
  //
  // See Also:
  //   CRhinoUiComboBox::OnPrintWidthEditKillFocus()
  //   CRhinoUiComboBox::OnEditKillFocus()
  //
  double PrintWidthFromString();
  //   
  // Description:
  //   This method is called when a CBN_EDITCHANGE message is received and this combo box
  //   The current value in the edit control is validated and converted to a valid width
  //   if possible.
  //
  // See Also:
  //   CRhinoUiComboBox::PrintWidthFromString()
  //   CRhinoUiComboBox::OnEditKillFocus()
  //
  void OnPrintWidthEditKillFocus();
  //   
  // Description:
  //   This method is called when a CBN_EDITUPDATE message is received and this combo box
  //   The current value in the edit control is validated and converted to a valid width
  //   if possible.
  //
  // See Also:
  //   CRhinoUiComboBox::PrintWidthFromString()
  //   CRhinoUiComboBox::OnEditKillFocus()
  //
  void OnPrintWidthEditUpdate();
  //-------------------------------------------------------------------------------------------------------
  // Font combo UI
  //-------------------------------------------------------------------------------------------------------
  //
  // Description:
  //   Enumerate fonts installed on system and add them to the list.  To see the font sample you must either
  //   use the CBS_OWERDRAWFIXED style or set m_bMakeOwnerDraw prior to creating this control.
  //
  // Parameters:
  //   bIncludeTrueType [in] If true then true type fonts are added to the list.
  //   bIncludeNonTrueType [in] If true then non true type fonts are added to the list.
  //
  // Returns:
  //   Number of fonts added to the list.  To see the font names you must either use the 
  //   CBS_OWERDRAWFIXED style or set m_bMakeOwnerDraw prior to creating this control.
  //
  // See Also:
  //   CRhinoUiComboBox::AddFontItem()
  //   
  int FillUsingInstalledFonts( bool bIncludeTrueType = true, bool bIncludeNonTrueType = true);
  //   
  // Description:
  //   Add font description to list.  To see font sample you must either use the  CBS_OWERDRAWFIXED style or 
  //   set m_bMakeOwnerDraw prior to creating this control.
  //
  // Parameters:
  //   lf [in] Font to add to the list
  //
  // Returns:
  //   Zero based index of new item in list or -1 on error.
  //
  // See Also:
  //   CRhinoUiComboBox::GetFontIndex()
  //   CRhinoUiComboBox::GetCurFont()
  //   
  int AddFontItem( const LOGFONT& lf);
  //   
  // Description:
  //   Find item in list that has same font face and make it the current selection. Nothing happens
  //   if font face name is not found in the list.
  //
  // Parameters:
  //   lpsFontFace [in] Font face name.
  //
  // Returns:
  //   A return value greater than or equal to zero is the zero based index of previously selected item.  A value
  //   of LB_ERROR indicates that there was not previous selection.  A value of -2 indicates there was and error
  //   and the selection was not changed.
  //
  // See Also:
  //   LB_ERROR
  //   CComboBox::SetCurSel()
  //
  int SetCurFont( LPCTSTR lpsFontFace);
  //   
  // Description:
  //   Find item in list that has same font face and make it the current selection. Nothing happens
  //   if font face name is not found in the list.
  //
  // Parameters:
  //   lf [in] LOGFONT Structure containing font face name.
  //
  // Returns:
  //   A return value greater than or equal to zero is the zero based index of previously selected item.  A value
  //   of LB_ERROR indicates that there was not previous selection.  A value of -2 indicates there was and error
  //   and the selection was not changed.
  //
  // See Also:
  //   CRhinoUiComboBox::GetCurFont()
  //   LB_ERROR
  //   CComboBox::SetCurSel()
  //
  int SetCurFont( const LOGFONT& lf);
  //   
  // Description:
  //   Get LOGFONT associated with currently selected item.
  //
  // Parameters:
  //   lf_out [out] Currently selected LOGFONT is copied here.
  //
  // Returns:
  //   Zero based index of currently selected item whos LOGFONT was copied.
  //   A value of LB_ERROR indicates there is no current selection.  A value of -2
  //   indicates there was a current selection but it did not contain associated
  //   font data.
  //
  // See Also:
  //   CRhinoUiComboBox::SetCurFont()
  //   CComboBox::GetCurSel()
  //   LB_ERROR
  //   
  int GetCurFont( LOGFONT& lf_out);
  //   
  // Description:
  //   Find list item associated with a particular font face name.
  //
  // Parameters:
  //   lpsFontFace[in] Font face name to search for.  String comparison is
  //                   not case sensitive.
  //
  // Returns:
  //   Zero based index of list item that matched font name or -1 if not found.
  //
  // See Also:
  //   CRhinoUiComboBox::AddFontItem()
  //   
  int GetFontIndex( LPCTSTR lpsFontFace);
  //   
  // Description:
  //   Find list item associated with a particular font face name.
  //
  // Parameters:
  //   lf[in] Font face name to search for.  String comparison is not case sensitive.
  //
  // Returns:
  //   Zero based index of list item that matched font name or -1 if not found.
  //
  // See Also:
  //   CRhinoUiComboBox::AddFontItem()
  //   
  int GetFontIndex( const LOGFONT& lf);
private:
  int EnumFontProc( const LOGFONT* lpLF, const TEXTMETRIC* lpTM, DWORD dwType);
  // end - Font combo UI
  //-------------------------------------------------------------------------------------------------------
private:
  friend int CALLBACK RhinoUiComboBoxEnumFontProc( const LOGFONT*, const TEXTMETRIC*, DWORD, LPARAM);
  void DestroyDataList();
  void EnsureOwnerDraw();
protected:
  friend void CRhinoUiPopDownLayerWnd::HidePopUpWindow();
  virtual void ShowLayerPopDownWnd( HWND hWnd, bool bShow );
  void DestroyLayerPopDownWnd();
public:
  class CRhinoUiPopDownLayerWnd* LayerPopDownWindow() const;
protected:
  //CRhinoUiComboBoxItemData m_current;
  RhinoUiColorComboBoxItemData m_current_color;
  COLORREF m_crCustom;

  RhinoUiLayerComboBoxItemData m_current_layer;
  bool m_bShowReferenceLayers;
  class CRhinoUiPopDownLayerWnd* m_pWndLayerPopDown;

  RhinoUiLinetypeComboBoxItemData m_current_linetype;
  ON_ObjectArray<ON_Linetype>m_from_file_linetypes;

  RhinoUiPrintWidthComboBoxItemData m_current_print_width;
  bool m_bInOnPrintWidthEditKillFocus;

  RhinoUiFontComboBoxItemData m_current_font;
  bool m_bIncludeTrueType;
  bool m_bIncludeNonTrueType;
  color_swatch_draw_style m_swatch_style;

  CRhinoUiComboBoxItemData::class_type m_fill_class_type;

  enum change_mode
  {
    cm_selchange = 0,
    cm_editupdate,
    cm_nochange,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_change_mode = 0xFFFFFFFF
  };
  change_mode m_change_mode;
private:
  bool m_bMakeOwnerDraw;

protected:
	DECLARE_MESSAGE_MAP()
public:
  //
  // Standard CComboBox overrides
  //
  virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
  virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	virtual void DeleteItem(LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/);
  //
  // Message map methods
  //
  afx_msg void OnDestroy();
  afx_msg void OnCbnSelchange();
  afx_msg void OnEditKillFocus();
  afx_msg void OnEditUpdate();
  afx_msg void OnCbnDropDown();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
  afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );

protected:
  virtual void PreSubclassWindow();
  // virtual overrides
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual BOOL32 OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual void PostNcDestroy();
  virtual BOOL32 OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


/////////////////////////////////////////////////////////////////////////////

/*
Description:
  The CRhinoUiUnitSystemComboBox control displays a combo box with
  selectable unit system items. To use this control:
    1. Using the dialog box editor, add a combo box control to your dialog.
    2. Using ClassWizard, create a member variable of type CComboBox for
       the combo box.
    3. Go to the ClassWizard-generated code and change the type of the variable
       from CComboBox to CRhinoUiUnitSystemComboBox.
    4. In you dialog's OnInitDialog member, call CRhinoUiUnitSystemComboBox::FillComboBox
       to fill the combo box with unit system strings.
    5. Call CRhinoUiUnitSystemComboBox::SetUnitSystem to set the unit system.
    6. Call CRhinoUiUnitSystemComboBox::UnitSystem to get the unit system.
*/
class RHINO_SDK_CLASS CRhinoUiUnitSystemComboBox : public CComboBox
{
	DECLARE_DYNAMIC( CRhinoUiUnitSystemComboBox )

public:
	CRhinoUiUnitSystemComboBox();
	virtual ~CRhinoUiUnitSystemComboBox();

  void FillComboBox( bool bIncludeCustomUnits, bool bIncludeNoUnits );

  ON::LengthUnitSystem UnitSystem() const;
  bool SetUnitSystem( ON::LengthUnitSystem unit );

protected:
	DECLARE_MESSAGE_MAP()
};


