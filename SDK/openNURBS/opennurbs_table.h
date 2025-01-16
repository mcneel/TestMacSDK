// ON_Table class
#ifndef OPENNURBS_TABLE_H_INCLUDED
#define OPENNURBS_TABLE_H_INCLUDED

#if defined(OPENNURBS_ANNOTATION_TABLE_WIP)

namespace ON_TableDef
{
    enum eRowType
  {
    unset_row  = 0x00,
    title_row  = 0x01,
    header_row = 0x02,
    data_row   = 0x04,
    all_row    = title_row | header_row | data_row
  };

  enum eGridlineType
  { 
    invalid_gridline  = 0x00,
    horz_top          = 0x01,
    horz_inside       = 0x02,
    horz_bottom       = 0x04,
    vert_left         = 0x08,
    vert_inside       = 0x10,
    vert_right        = 0x20,
    horz_gridlines    = horz_top | horz_bottom | horz_inside,
    vert_gridlines    = vert_left | vert_right | vert_inside,
    outer_gridlines   = horz_top | horz_bottom | vert_left | vert_right,
    inner_gridlines   = horz_inside | vert_inside,
    all_grid_lines    = outer_gridlines | inner_gridlines
  };

  enum eDataType
  {
    data_unknown      = 0x00,
    data_int          = 0x01,
    data_uint         = 0x02,
    data_double       = 0x04,
    data_string       = 0x08,
    data_date         = 0x10,
    data_2dpoint      = 0x20,
    data_3dpoint      = 0x40,
  };                  
                      
  enum eUnitType      
  {                   
    unit_unitless     = 0x00,
    unit_distance     = 0x01,
    unit_angle        = 0x02,
    unit_area         = 0x04,
    unit_volume       = 0x08,
    unit_currency     = 0x10,
    unit_percentage   = 0x20
  };

  enum eCellEdge
  { 
    top_edge          = 0x1,
    right_edge        = 0x2,
    bottom_edge       = 0x4,
    left_edge         = 0x8,
    all_edges         = top_edge | right_edge | bottom_edge | left_edge
  };

  enum eCellAlignment
  { 
    top_left          = 1,
    top_center        = 2,
    top_right         = 3,
    middle_left       = 4,
    middle_center     = 5,
    middle_right      = 6,
    bottom_left       = 7,
    bottom_center     = 8,
    bottom_right      = 9 
  };

  enum eStyleType
  {
    cell_style = 1,
    row_style = 2,
    column_style = 3,
    table_style = 4,
  };

  enum eSelectionType
  {
    not_selected   = 0,
    row_selected   = 1,
    col_selected   = 2,
    cell_selected  = 3,
    range_selected = 4,
  };
};

using namespace ON_TableDef;

class ON_CLASS ON_Table;

class ON_CLASS ON_TableRange
{
public:
  ON_TableRange()
    : m_min_row(-1)
    , m_min_col(-1)
    , m_max_row(-1)
    , m_max_col(-1) 
  {}

  ON_TableRange(ON__INT32 top_row, ON__INT32 left_column, ON__INT32 bottom_row, ON__INT32 right_column)
    : m_min_row(top_row)
    , m_min_col(left_column)
    , m_max_row(bottom_row)
    , m_max_col(right_column) 
  {}

  bool operator==(const ON_TableRange src)
  {
    return m_min_row == src.m_min_row && 
           m_min_col == src.m_min_col &&
           m_max_row == src.m_max_row &&
           m_max_col == src.m_max_col;
  }

  bool operator!=(const ON_TableRange src)
  {
    return !operator==(src);
  }

  ON__INT32  m_min_row;
  ON__INT32  m_min_col;
  ON__INT32  m_max_row;
  ON__INT32  m_max_col;
};

class ON_CLASS  ON_TableGridline
{
public:  
  ON_TableGridline();

  ON__UINT32                   m_property_flag;
  bool                         m_override_flag;

  bool                         m_visible;
  ON::object_color_source      m_color_source;
  ON::plot_color_source        m_plot_color_source;
  ON::plot_weight_source       m_plot_weight_source;
  eGridlineType                m_type;
  double                       m_line_spacing;
  ON_Color                     m_color;
  ON_Color                     m_plot_color;
  double                       m_plot_weight_mm;
};

class ON_CLASS ON_TableCellStyle
{
public:
  ON_TableCellStyle(eStyleType style_type = table_style);

  static ON__INT32 NextId() 
  {
    static ON__INT32 next_id = 0;
    return next_id++;
  }

  ON__INT32              m_style_id;
  eStyleType             m_style_type;
  ON__UINT32             m_overrides;
  ON_Font                m_font;
  double                 m_text_height;
  eCellAlignment         m_alignment;
  ON_Color               m_text_color;
  ON_Color               m_fill_color;
  bool                   m_fill_enabled;
  eDataType              m_data_type;
  eUnitType              m_unit_type;
  double                 m_grid_margins[4]; // left, right, top, bottom
  ON__UINT32             m_margin_flag;
  ON_TableGridline       m_gridlines[4];

  const wchar_t*         DoubleFormat() const;
  const wchar_t*         IntFormat() const;
  const wchar_t*         DateFormat() const;
  void                   SetDoubleFormat(const wchar_t* format);
  void                   SetIntFormat(const wchar_t* format);
  void                   SetDateFormat(const wchar_t* format);

  static const wchar_t*  DefaultDoubleFormat();
  static const wchar_t*  DefaultIntFormat();
  static const wchar_t*  DefaultDateFormat();
private:
  wchar_t                m_double_format_string[16];
  wchar_t                m_int_format_string[16];
  wchar_t                m_date_format_string[32];

};

class ON_CLASS ON_TableCell
{
public:
  ON_TableCell(ON__INT32 id = -1);
  ~ON_TableCell();

  eDataType         ContentType() const;
  void              SetContent(const ON_TableCell& src);

  void              SetStringContent(const ON_Table& table, const wchar_t* string);
  void              SetIntContent(const ON_Table& table, int number);
  void              SetDoubleContent(const ON_Table& table, double number);
  void              EmptyContent();
  const wchar_t*    StringValue() const;
  bool              DoubleValue(double& double_out) const;
  bool              IntValue(ON__INT64& int_out) const;
  const wchar_t*    GetContentString() const;

  ON__INT32         StyleId() const;
  void              SetStyleId(ON__INT32 id);
  double            TextHeight() const;

  static
  const wchar_t*    FormatContent(const ON_Table& table, ON__INT32 style_id, double number);
  static
  const wchar_t*    FormatContent(const ON_Table& table, ON__INT32 style_id, ON__INT64 number);
private:
  void              FormatContent(const ON_Table& table);
  void              FormatContent(const ON_Table& table, ON__INT32 style_id);
  eDataType         m_content_type;
  ON__INT32         m_style_id;
  ON_wString        m_string_content;
  ON_U              m_content;
};

class ON_CLASS ON_TableColumn;
class ON_CLASS ON_TableRow;

class ON_CLASS ON_TableSelection : public ON_TableRange
{
public:
  ON_TableSelection() 
    : m_selection_type(not_selected)
  {};

  eSelectionType m_selection_type;
};

class ON_CLASS ON_TableData : public ON_ClassArray< ON_TableRow >
{
public:
  ON_TableData()
    : m_cell_style(row_style)
  {};
  ON_TableCellStyle   m_cell_style;
};

class ON_CLASS ON_TableColumnData : public ON_ClassArray< ON_TableColumn >
{
public: 
  ON_TableColumnData() {};
};

class ON_CLASS ON_TableStyleArray : public ON_ClassArray< ON_TableCellStyle >
{
public: 
  ON_TableStyleArray() {};
};

class ON_CLASS ON_TableRangeArray : public ON_ClassArray< ON_TableRange >
{
public: 
  ON_TableRangeArray() {};
};

#if defined(ON_DLL_TEMPLATE)
// This stuff is here because of a limitation in the way Microsoft
// handles templates and DLLs.  See Microsoft's knowledge base 
// article ID Q168958 for details.
#pragma ON_PRAGMA_WARNING_PUSH
#pragma ON_PRAGMA_WARNING_DISABLE_MSC( 4231 )
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableRange >;
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableCell >;
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableRow >;
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableCellStyle >;
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableColumn >;
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableSelection >;
ON_DLL_TEMPLATE template ON_ClassArray< ON_TableRange >;
#pragma ON_PRAGMA_WARNING_POP
#endif


class ON_CLASS ON_Table : public ON_Geometry
{
  ON_OBJECT_DECLARE(ON_Table);

public:
  // Default constructor
  ON_Table() = default;
  ~ON_Table() = default;
  ON_Table(const ON_Table& src) = default;
  ON_Table& operator=(const ON_Table& src) = default;

  bool Create(
    ON__UINT32 rows,
    ON__UINT32 cols, 
    bool has_title,
    bool has_header);

  void Destroy();

#pragma region OnFunctions

  virtual ON_Table* DuplicateTable() const;

  // ON_Object overrides
  /////////////////////////////////////////////////////////////////
  bool IsValid( class ON_TextLog* text_log = nullptr ) const override;
  void Dump(ON_TextLog& text_log) const override; // for debugging
  bool Write( ON_BinaryArchive& archive) const override;
  bool Read( ON_BinaryArchive& archive) override;
  ON::object_type ObjectType() const override;

  // ON_Geometry overrides
  /////////////////////////////////////////////////////////////////
  /*
    Returns the geometric dimension of the object ( usually 3)
  */
  int Dimension() const override;

  // virtual ON_Geometry GetBBox override		
  bool GetBBox( double* boxmin, double* boxmax, bool bGrowBox = false ) const override;

  // virtual ON_Geometry GetTightBoundingBox override		
  bool GetTightBoundingBox( class ON_BoundingBox& tight_bbox, bool bGrowBox = false, const class ON_Xform* xform = nullptr ) const override;

  ON_BoundingBox BoundingBox() const;

  /*
    Description:
      Transform the object by a 4x4 xform matrix
    Parameters:
      [in] xform  - An ON_Xform with the transformation information
    Returns:
      true = Success
      false = Failure
    Remarks:
      The object has been transformed when the function returns.
  */
  bool Transform(const ON_Xform&) override;
#pragma endregion

#pragma region interface

  const ON_Plane& Plane() const;

  ON__UINT32 RowCount() const;
  void SetRowCount(ON__UINT32 rowcount);
  
  ON__UINT32 ColCount() const;
  void SetColCount(ON__UINT32 colcount);

   void SetSize(ON__UINT32 nrows, ON__UINT32 ncols);
   
   ON_TableCell* GetCell(ON__UINT32 row, ON__UINT32 col);
   bool GetCell(ON__UINT32 row, ON__UINT32 col, ON_TableCell& cell) const;

  // Total 3d width of the table
  double TotalWidth() const;
  bool SetTotalWidth(double width);
  
  // Total 3d height of the table
  double TotalHeight() const;
  bool SetTotalHeight(double height);
  
  double ColWidth(ON__UINT32 column) const;
  bool SetColWidth(ON__UINT32 column, double width);
  bool SetColWidth(double width);  // Sets width for all columns
  
  double RowHeight(ON__UINT32 row) const;
  bool SetRowHeight(ON__UINT32 row, double height);
  bool SetRowHeight(double height);  // Sets Height for all rows
  
  double CellMarginHorz() const;
  void SetCellMarginHorz(double margin);
  double CellMarginVert() const;
  void SetCellMarginVert(double margin);
  
  bool HasTitle() const;
  void SetHasTitle(bool title);
  bool HasHeader() const;
  void SetHasHeader(bool header);

  eRowType RowType(ON__UINT32 row) const;

  eCellAlignment Alignment(eRowType rowtype = data_row) const;
  eCellAlignment Alignment(ON__UINT32 row, ON__UINT32 col) const;
  bool SetAlignment(eCellAlignment alignment, ON__UINT32 row_type = data_row);
  bool SetAlignment(eCellAlignment alignment, ON__UINT32 row, ON__UINT32 col);
  
  bool IsBackgroundColorNone(eRowType rowtype = all_row) const;
  bool IsBackgroundColorNone(ON__UINT32 row, ON__UINT32 col) const;
  bool SetBackgroundColorNone(bool none, eRowType rowtype = all_row);
  bool SetBackgroundColorNone(bool none, ON__UINT32 row, ON__UINT32 col);
  
  ON_Color BackgroundColor(eRowType rowtype = all_row) const;
  ON_Color BackgroundColor(ON__UINT32 row, ON__UINT32 col) const;
  bool SetBackgroundColor(ON_Color color, eRowType rowtype = all_row);
  bool SetBackgroundColor(ON_Color color, ON__UINT32 row, ON__UINT32 col);
  
  ON_Color ContentColor(eRowType rowtype = all_row) const;
  ON_Color ContentColor(ON__UINT32 row, ON__UINT32 col) const;
  bool SetContentColor(ON_Color color, eRowType rowtype = all_row);
  bool SetContentColor(ON_Color color, ON__UINT32 row, ON__UINT32 col);

  ON_Font Font(eRowType rowtype) const;
  ON_Font Font(ON__UINT32 row, ON__UINT32 col) const;
  bool SetFont(ON_Font font, ON__UINT32 row_type = data_row);
  bool SetFont(ON_Font font, ON__UINT32 row, ON__UINT32 col);
  
  double TextHeight(eRowType rowtype= data_row) const;
  double TextHeight(ON__UINT32 row, ON__UINT32 col) const;
  bool SetTextHeight(double height,ON__UINT32 row_type = data_row);
  bool SetTextHeight(double height, ON__UINT32 row, ON__UINT32 col);

  double GridLineWeight_mm(eRowType row_type = all_row) const; 
  double GridLineWeight_mm(ON__UINT32 row, ON__UINT32 col, eCellEdge edge_type) const;
  bool SetGridLineWeight_mm(double weight_mm, eRowType row_type, eGridlineType gridline_type);
  bool SetGridLineWeight_mm(double weight_mm, ON__UINT32 row, ON__UINT32 col, eCellEdge edge_type);

  ON_Color GridColor(eRowType row_type, eGridlineType gridline_type) const; 
  ON_Color GridColor(ON__UINT32 row, ON__UINT32 col, eCellEdge edge_type) const;
  void SetGridColor(ON_Color color, eRowType row_type, eGridlineType gridline_type);
  void SetGridColor(ON_Color color, ON__UINT32 row, ON__UINT32 col, eCellEdge edge_type);

  bool GridVisible(eRowType row_type, eGridlineType gridline_type) const; 
  bool GridVisible(ON__UINT32 row, ON__UINT32 col, eCellEdge edge_type) const;
  void SetGridVisible(bool visible, eRowType row_type, eGridlineType gridline_type);
  void SetGridVisible(bool visible, ON__UINT32 row, ON__UINT32 col, eCellEdge edge_type);

  bool CellExtents(ON__UINT32 row, ON__UINT32 col, bool ignore_margins,
                   ON_2dPoint& ll, ON_2dPoint& ur) const;
  bool CellExtents(ON__UINT32 row, ON__UINT32 col, bool ignore_margins, 
                   ON_3dPoint& ll, ON_3dPoint& lr, ON_3dPoint& ur, ON_3dPoint& ul) const;

  bool AttachPoint(ON__UINT32 row, ON__UINT32 col, ON_2dPoint& pt2d) const;
  bool AttachPoint(ON__UINT32 row, ON__UINT32 col, ON_3dPoint& pt3d) const;

  bool DeleteCellContent(ON__UINT32 row, ON__UINT32 col);

  bool TextString(ON__UINT32 row, ON__UINT32 col, ON_wString& string) const;
  const wchar_t* TextString(ON__UINT32 row, ON__UINT32 col) const;
  bool SetTextString(const wchar_t* string, ON__UINT32 row, ON__UINT32 col);

  bool InsertColumns(ON__UINT32 before_col, double width, ON__UINT32 num_cols = 1);
  bool DeleteColumns(ON__UINT32 first_col, ON__UINT32 num_cols = 1);
  bool InsertRows(ON__UINT32 before_row, double height, ON__UINT32 num_rows = 1);
  bool DeleteRows(ON__UINT32 first_row, ON__UINT32 num_rows = 1);
  bool AppendColumns(double width, ON__UINT32 num_cols = 1); 
  bool AppendRows(double height, ON__UINT32 num_rows = 1); 

  const ON_TableRow* Row(ON__UINT32 row) const;
  const ON_TableColumn* Column(ON__UINT32 col) const;

  bool GetCorners(ON_3dPoint& upper_left, ON_3dPoint& lower_left, ON_3dPoint& lower_right, ON_3dPoint& upper_right) const;
  void GetCellCorners(ON_3dPoint& ul, ON_3dPoint& ll, ON_3dPoint& lr, ON_3dPoint& ur, ON__UINT32 row, ON__UINT32 col) const; 

  const ON_TableCellStyle& CellStyle(ON__UINT32 row, ON__UINT32 col) const;
  const ON_TableCellStyle& CellStyle(ON__INT32 id) const;
  bool AppendCellStyle(const ON_TableCellStyle& style);  // will not append if id already exists
  bool ReplaceCellStyle(const ON_TableCellStyle& style); // replaces style with same id

  static const double MinColWidth;    // = 1.0
  static const double MinRowHeight;   // = 0.1
  static const double MinTableWidth;  // = 1.0
  static const double MinTableHeight; // = 0.1
    
#pragma endregion

  bool SetSelection(ON__UINT32 row, ON__UINT32 col);
  bool SelectCell(bool add, ON__UINT32 row, ON__UINT32 col);
  bool SelectRow(bool add, ON__UINT32 row);
  bool SelectCol(bool add, ON__UINT32 col);
  bool SelectRange(bool add, ON__UINT32 min_row, ON__UINT32 min_col, ON__UINT32 max_row, ON__UINT32 max_col);
  bool HasSelection();
  void ClearSelection();

protected:
  int CellStyleIndex(ON__INT32 id) const;

  ON_Plane               m_plane;
  ON_TableData           m_rows;
  ON_TableColumnData     m_columns;
                        
  ON_TableStyleArray     m_styles;
  ON__INT32              m_style_id = -1;

  ON_TableSelection      m_selection;

  bool                   m_has_title = false;
  bool                   m_has_header = false;
};

class ON_CLASS ON_TableColumn
{
public:
  ON_TableColumn() = default;
  ~ON_TableColumn() = default;
  ON_TableColumn(const ON_TableColumn&) = default;
  ON_TableColumn& operator=(const ON_TableColumn&) = default;

  ON_TableColumn(
    const wchar_t* name, 
    double width = ON_Table::MinColWidth
  );

  const wchar_t* Name() const;
  void SetName(const wchar_t* name);
  double Width() const;
  void SetWidth(double width);
  ON__INT32 StyleId() const;
  void SetStyleId(ON__INT32 id);

private:
  ON_wString        m_name;
  ON__INT32         m_style_id = -1;
  double            m_width = ON_Table::MinColWidth;
};

class ON_CLASS ON_TableRow : public ON_ClassArray< ON_TableCell >
{
public:
  ON_TableRow() ;
  ON_TableRow(double height);
  double Height() const;
  void SetHeight(double height);
  bool IsTitle() const;
  void SetTitle(bool title);
  bool IsHeader() const;
  void SetHeader(bool header);

private:
  bool   m_is_header;
  bool   m_is_title;
  double m_height;
};

#endif

#endif
