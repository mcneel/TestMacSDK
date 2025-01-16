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
class CRhinoUiSliderCtrl;
//
// CRhinoUiEdit
//
// Description:
//    Standard edit control to be used by CRhinoUiDialog classes.  This edit control
//    should provide all the standard edit control behavior for Rhino.  See edit_type
//    enum for currently supported edit modes.
//
class RHINO_SDK_CLASS CRhinoUiEdit : public CEdit
{
	DECLARE_DYNAMIC(CRhinoUiEdit)
public:
  //   
  // Description:
  //   Constructor
  //
	CRhinoUiEdit();
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiEdit();
  //   
  // Description:
  //   Available edit control modes
  //     et_text   - Standard text type edit control.
  //     et_int    - Only allow integer values.  Make sure you replace the standard data exchange
  //                 macro with the following to allow data validation:
  //                    m_MyEditControl.DDX_Text( pDX, nIDC, m_nMyIntProperty);
  //     et_double - Only allow double values1.  Make sure you replace the standard data exchange
  //                 macro with the following to allow data validation:
  //                    m_MyEditControl.DDX_Text( pDX, nIDC, m_nMyIntProperty);
  //     et_ip,    - Windows IP input control
  //   
  enum edit_type
  {
    et_text = 0, // Default type
    et_int,
    et_double,
    et_ip,
    et_invalid,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_edit_type = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Current mode for this edit control.
  //
  // Returns:
  //   Return current edit mode for this control.
  //
  // See Also:
  //   CRhinoUiEdit::edit_type
  //   
  int GetEditType() const;
  //   
  // Description:
  //   Set current mode for this edit control.  You can use GetSpinn
  //
  // Parameters:
  //   [et] New edit mode for this control.
  //
  // Returns:
  //   Previous edit mode
  //
  // See Also:
  //   CRhinoUiEdit::edit_type
  //   
  int SetEditType( edit_type et);
  //   
  // Description:
  //   Returns the current decimal display precision when the
  //   control is displaying double values (CRhinoUiEdit::et_double).
  //
  // Returns:
  //   Returns the current display precision
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::SetDisplayPrecison()
  //   
  int DisplayPrecision() const;
  //   
  // Description:
  //   Sets the current decimal display precision when the
  //   control is displaying double values (CRhinoUiEdit::et_double).
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::DisplayPrecison()
  //   
  void SetDisplayPrecision( int precision );
  //   
  // Description:
  //   If this method returns true a spinner control will be created and embedded
  //   in this edit control.  Use the GetSpinButtonCtrl() method to gain access to
  //   the embedded control.
  //
  // Returns:
  //   Returns true if Spinner will be created
  //
  // See Also:
  //   CRhinoUiEdit::GetSpinButtonCtrl()
  //   
  bool IncludeSpinner() const;
  //   
  // Description:
  //   This function will set the create spinner control flag.  If this is set to
  //   true then a spinner control will be created and embedded in this edit control.
  //   Use the GetSpinButtonCtrl() method to gain access to the embedded control.
  //
  // Parameters:
  //   b [in] If true create spin control when this control is created.
  //
  // Returns:
  //   Return previous value.
  //
  // See Also:
  //   CRhinoUiEdit::IncludeSpinner()
  //   CRhinoUiEdit::GetSpinButtonCtrl()
  //   
  bool SetIncludeSpinner( bool b);
  //
  // DoDataExchange methods
  //   
  // Description:
  //    Replace the DDX_Text(....) function call in your dialog class
  //    with m_?my_control?.DDX_Text( ...)
  //
  // Example:
  //    Header file contains:
  //       CRhinoUiEdit m_IntCtrl;
  //       int          m_nIntValue;
  //   CPP file would contain:
  //    void CTestDialog::DoDataExchange( CDataExchange* pDX)
  //    {
	//        CRhinoUiDialog::DoDataExchange(pDX);
	//        //{{AFX_DATA_MAP(CTestDialog)
	//        DDX_Control(pDX, IDC_EDIT1, m_IntCtrl);
	//        //}}AFX_DATA_MAP
	//        m_IntCtrl.DDX_Text(pDX, IDC_EDIT1, m_nIntValue);
  //     }
  //
  // Parameters:
  //   pDX [in] A pointer to a CDataExchange object. The framework supplies this object to establish
  //            the context of the data exchange, including its direction. 
  //   nIDC [in] The ID of an edit control in the dialog box, form view, or control view object. 
  //   nValue [in] A reference to a data member in the dialog box, form view, or control view object.
  //               The data type of value depends on which of the overloaded versions of DDX_Text you use.
  //               Make sure SetEditType() is called with the correct data type also.
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   DDX_Text MFC macro
  //   
  //
  void DDX_Text( CDataExchange* pDX, int nIDC, int& nValue);
  void DDX_Text( CDataExchange* pDX, int nIDC, double& nValue);
  void DDX_Text( CDataExchange* pDX, int nIDC, ON_wString& wValue);
  //   
  // Description:
  //    Format and verify current edit control value.  If the value is invalid then it is set to the last
  //    good value and the control is updated.
  //
  void Format();
  //   
  // Description:
  //   If this is set to true then zero value is not allowed otherwise zero is okay.
  //
  // Parameters:
  //   bSet [in] If true then values must be non zero.
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::DDX_Text
  //   
  void SetNonZero( bool bSet = TRUE);
  //   
  // Description:
  //   Set or clear minimum numeric value.  This is only relevant if the EditType() is
  //   set to et_double or et_int.
  //
  // Parameters:
  //   bSset [in] If true then check minimum value flag is set and the minimum allowable
  //              value is set to min.  If false then check minimum value flag is unset.
  //   nMin  = Minimum allowable value, only used if set is true otherwise is ignored.
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::DDX_Text
  //   CRhinoUiEdit::SetMax()
  //   
  // Notes:
  //   Constructor sets has minimum flag to FALSE
  void SetMin( bool bSet, double nMin = 0.0 );
  //   
  // Description:
  //   Set or clear minimum numeric value.  This is only relevant if the EditType() is
  //   set to et_double or et_int.
  //
  // Parameters:
  //   bSset [in] If true then check minimum value flag is set and the minimum allowable
  //              value is set to min.  If false then check minimum value flag is unset.
  //   nMin  = Minimum allowable value, only used if set is true otherwise is ignored.
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::DDX_Text
  //   CRhinoUiEdit::SetMax()
  //   
  // Notes:
  //   Constructor sets has minimum flag to FALSE
  void SetMin( bool bSet, int nMin = 0 );
  //   
  // Description:
  //   Set or clear maximum numeric value.  This is only relevant if the EditType() is
  //   set to et_double or et_int.
  //
  // Parameters:
  //   bSset [in] If true then check maximum value flag is set and the maximum allowable
  //              value is set to min.  If false then check maximum value flag is unset.
  //   nMax  = Maximum allowable value, only used if set is true otherwise is ignored.
  //
  // Returns:
  //   
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::DDX_Text
  //   CRhinoUiEdit::SetMax()
  //   
  void SetMax( bool bSet, double nMax = 0.0 );
  //   
  // Description:
  //   Set or clear maximum numeric value.  This is only relevant if the EditType() is
  //   set to et_double or et_int.
  //
  // Parameters:
  //   bSset [in] If true then check maximum value flag is set and the maximum allowable
  //              value is set to min.  If false then check maximum value flag is unset.
  //   nMax  = Maximum allowable value, only used if set is true otherwise is ignored.
  //
  // Returns:
  //   
  //
  // See Also:
  //   CRhinoUiEdit::SetEditType()
  //   CRhinoUiEdit::DDX_Text
  //   CRhinoUiEdit::SetMax()
  //   
  void SetMax( bool bSet, int nMax = 0 );
  //   
  // Description:
  //   If this is set to true then NULL values are allowed in control.
  //
  // Parameters:
  //   bSet [in] If set to true then NULL values are allowed otherwise this control
  //             must contain at least one character.
  //
  void SetAllowEmpty( bool bSet = true);
  //   
  // Description:
  //   If nValue is out of range then clamps it to minimum or maximum
  //   value and updates the edit control.  Will only clamp if minimum
  //   and or maximum values have been set.
  //
  // Parameters:
  //   nValue [in/out] Value to validate.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void VerifyValue( int& nValue);
  //   
  // Description:
  //   If nValue is out of range then clamps it to minimum or maximum
  //   value and updates the edit control.  Will only clamp if minimum
  //   and or maximum values have been set.
  //
  // Parameters:
  //   nValue [in/out] Value to validate.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void VerifyValue( double& nValue);
  //   
  // Description:
  //   Uses VerifyValue() to clamp value to minimu or maximum if appropriate and update
  //   edit control.
  //
  // Parameters:
  //   nValue [in] New value for control.  If this value is out of range then it will
  //               be clamped at minimum and/or maximum values as necessary.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   CRhinoUiEdit::VerifyValue()
  //   
  void SetValue( int nValue);
  //   
  // Description:
  //   Uses VerifyValue() to clamp value to minimu or maximum if appropriate and update
  //   edit control.
  //
  // Parameters:
  //   nValue [in] New value for control.  If this value is out of range then it will
  //               be clamped at minimum and/or maximum values as necessary.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   CRhinoUiEdit::VerifyValue()
  //   
  void SetValue( double nValue);
  //   
  // Description:
  //   Allow keyword as value in edit control.
  //
  // Parameters:
  //   lps [in] String to non-case sensitive match
  //
  // See Also:
  //   CRhinoUiEdit::FormatKeyword()
  //   
  void SetFormatKeyword( const char* lps);
  //   
  // Description:
  //   Allow keyword as value in edit control.
  //
  // Parameters:
  //   lps [in] String to non-case sensitive match
  //
  // See Also:
  //   CRhinoUiEdit::FormatKeyword()
  //   
  void SetFormatKeyword( const wchar_t* lps);
  //   
  // Description:
  //   Keyword allowed as value in edit control.
  //
  // Returns:
  //   If a non NULL string the string is used to non-case sensitive match
  //   edit control value when validating.
  // See Also:
  //   CRhinoUiEdit::SetFormatKeyword()
  //   
  const CString& FormatKeyword() const;
  //   
  // Description:
  //   Synchronize slider control which is companion to this edit control.
  //
  void SynchSlider();
  //   
  // Description:
  //   Get last good numeric value entered in control.
  //
  // Parameters:
  //   n [out] Last good number entered
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void GetNumericValue( int& n);
  //   
  // Description:
  //   Get last good numeric value entered in control.
  //
  // Parameters:
  //   n [out] Last good number entered
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void GetNumericValue( double& d);
  //   
  // Description:
  //   Get string from edit control.
  //
  // Parameters:
  //   w [out] Current edit control string.
  //
  // See Also:
  //   CRhinoUiEdit::GetNumericValue()
  //   CRhinoUiEdit::StringValue()
  //   
  void GetStringValue( ON_wString& w);
  //   
  // Description:
  //   Get string from edit control.
  //
  // Returns:
  //   Returns current edit control string.
  //
  // See Also:
  //   CRhinoUiEdit::GetNumericValue()
  //   CRhinoUiEdit::GetStringValue()
  //   
  ON_wString StringValue();
  //   
  // Description:
  //   Set string from edit control.
  //
  // Parameters:
  //   w [in] New value for edit control.
  //   bAppend [in] If true then value is appended to the end of the current
  //                edit control string otherwise; the edit control string is
  //                replaced with w.
  //
  // See Also:
  //   CRhinoUiEdit::GetStringValue()
  //   CRhinoUiEdit::StringValue()
  //   
  void SetStringValue( const wchar_t* w, bool bAppend = false);
  //   
  // Description:
  //   Set string value for edit control.
  //
  // Parameters:
  //   s [in] New value for edit control.
  //   bAppend [in] If true then value is appended to the end of the current
  //                edit control string otherwise; the edit control string is
  //                replaced with s.
  //
  // See Also:
  //   CRhinoUiEdit::GetStringValue()
  //   CRhinoUiEdit::StringValue()
  //   
  void SetStringValue( const char* s, bool bAppend = false);
  //   
  // Description:
  //   Convert double to string and put string in edit control.
  //
  // Parameters:
  //   d [in] Value to place in edit control
  //
  // See Also:
  //   CRhinoUiEdit::SetStringValue()
  //   
  void SetNumber( double d);
  //   
  // Description:
  //   Convert int to string and put string in edit control.
  //
  // Parameters:
  //   i [in] Value to place in edit control
  //
  // See Also:
  //   CRhinoUiEdit::SetStringValue()
  //   
  void SetNumber( int i);
  enum on_number_error
  {
    one_use_last_good_number = 0,
    one_clamp_to_min_max_value,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_number_error = 0xFFFFFFFF
  };
  //   
  // Description:
  //   Call this method to deterimine what happens when a numeric value is less then
  //   the specified minimum or greater than the specified maximum value.
  //
  // Returns:
  //   one_use_last_good_number = If a value is specified that is out of range then the
  //                              last valid value is used.
  //   one_clamp_to_min_max_value = If a value is specified that is out of range then the
  //                                minimum or maximum values are used if specified.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  on_number_error OnNumberErrorMode() const;
  //   
  // Description:
  //   Call this method to specify what happens when a numeric value is less then
  //   the specified minimum or greater than the specified maximum value.
  //
  // Parameters:
  //   mode [in]
  //       one_use_last_good_number = If a value is specified that is out of range then the
  //                                  last valid value is used.
  //       one_clamp_to_min_max_value = If a value is specified that is out of range then the
  //                                    minimum or maximum values are used if specified.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void SetOnNumberErrorMode( on_number_error mode);
  //   
  // Description:
  //   Get spin button control embedded in this edit control.
  //
  // Returns:
  //   Returns pointer to spin button control if one was created otherwise NULL.
  //
  // See Also:
  //   CRhinoUiEdit::IncludeSpinner()
  //   CRhinoUiEdit::SetIncludeSpinner()
  //   
  CSpinButtonCtrl* GetSpinButtonCtrl() const;
  void LinkToSliderCtrl( CRhinoUiSliderCtrl*);
protected:
  //   
  // Description:
  //   Get text from edit control, convert to integer and clamp using minimum and/or maximum 
  //   values as appropriate.  If value changed (clamped) then edit control will reflect the
  //   modified value.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void FormatInt();
  //   
  // Description:
  //   Get text from edit control, convert to double and clamp using minimum and/or maximum 
  //   values as appropriate.  If value changed (clamped) then edit control will reflect the
  //   modified value.
  //
  // See Also:
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void FormatReal();
  //   
  // Description:
  //   This method is used to validate character input.  If the character can not be used
  //   to create an integer then a MessageBeep() is sounded and the character is discarded.
  //   If possible the new value is clamped to minimum and/or maximum values as appropriate.
  //
  // Parameters:
  //   nChar [in] Specifies the virtual key code of the given key. For a list of standard 
  //              virtual key codes, see Winuser.h 
  //   nRepCnt [in] Specifies the repeat count, that is, the number of times the keystroke
  //                is repeated as a result of the user holding down the key. 
  //   nFlags  [in] Specifies the scan code, key-transition code, previous key state, and
  //                context code.  See CWnd::OnChar() for table of possible state and codes.
  // See Also:
  //   CWnd::OnChar()
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //   
  void OnCharInt(UINT nChar, UINT nRepCnt, UINT nFlags);
  //   
  // Description:
  //   This method is used to validate character input.  If the character can not be used
  //   to create an double then a MessageBeep() is sounded and the character is discarded.
  //   If possible the new value is clamped to minimum and/or maximum values as appropriate.
  //
  // Parameters:
  //   nChar [in] Specifies the virtual key code of the given key. For a list of standard 
  //              virtual key codes, see Winuser.h 
  //   nRepCnt [in] Specifies the repeat count, that is, the number of times the keystroke
  //                is repeated as a result of the user holding down the key. 
  //   nFlags  [in] Specifies the scan code, key-transition code, previous key state, and
  //                context code.  See CWnd::OnChar() for table of possible state and codes.
  // See Also:
  //   CWnd::OnChar()
  //   CRhinoUiEdit::SetMin()
  //   CRhinoUiEdit::SetMax()
  //
  void OnCharReal(UINT nChar, UINT nRepCnt, UINT nFlags);
  //   
  // Description:
  //   If GetEditType() is equal to et_int or et_double then create a CSpinButtonControl and
  //   attach it to the edit box.
  //
  // Returns:
  //   If spin button control is created then true is returned.
  //
  // See Also:
  //   CRhinoUiEdit::IncludeSpinner()
  //   CRhinoUiEdit::SetIncludeSpinner()
  //   
  bool CreateBuddy();
private:
  class CRUiSpinButtonCtrl* m_spinner;
protected:
  int m_edit_type;
  bool m_nonzero;
  bool m_hasmin;
  bool m_hasmax;
  double m_min;
  double m_max;
  bool m_allowemtpy;
  on_number_error m_on_number_error;
  CString m_key_word;
  int m_displayprecision;
  double m_lastgoodnumber;
  bool m_include_buddy;
  CRhinoUiSliderCtrl* m_linked_slider;
  CString m_last_good_string;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
  virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs);
  virtual void PreSubclassWindow();
};


