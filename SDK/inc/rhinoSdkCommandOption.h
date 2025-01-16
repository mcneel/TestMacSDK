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

class RHINO_SDK_CLASS CRhinoCommandOption
{
public:
  CRhinoCommandOption();
  ~CRhinoCommandOption();
  CRhinoCommandOption( const CRhinoCommandOption&);
  CRhinoCommandOption& operator=( const CRhinoCommandOption&);

  void Clear();

  // Index assigned by CRhinoGet::AddOption();
  int m_option_index;

  bool UpdateValue( const wchar_t* );

  // Assigned by CRhinoGet::Get*() if an option value specified in script
  // or by a fancy command window control
  ON_wString m_option_value;

  enum COMMAND_OPTION_TYPE
  {
    simple_option = 0,
    number_option = 1,
    toggle_option = 2,
    color_option  = 3,
    list_option   = 4,
    hidden_option = 5, // full name required to activate - does not appear on command line
    string_option = 6,
    force_32bit_enum = 0xFFFFFFFF
  } m_type;

  // option names
  ON_wString m_local_option_name;  // <- this one shows up on command window prompt
  ON_wString m_english_option_name; // this is for _english script processing

  ////////////////////////////////////////////////////
  //
  // option accelerator characters are automatically
  // set in CRhinoGet::SetOptionAccelerators()
  //
  // If m_option_accelerator_char is not zero, this is the single char 
  // accelerator.  If m_option_accelerator_char is zero, then the option
  // does not have a single char accelerator.
  // If m_option_accelerator_char is not zero and 
  // m_option_accelerator_index >= 0,
  // then m_option_accelerator_index is the index of the
  // accelerator character in the m_local_option_name.  The
  // m_option_accelerator_index value is used for formatting
  // m_local_option_name display.
  // For languages that are far from ASCII (Asian languages), 
  // m_option_accelerator is typically a letter from the 
  // m_english_option_name and  m_option_accelerator_index will be -1.
  // For languages that are (nearly) ASCII, m_option_accelerator_index>=0
  // and m_option_accelerator is a character from m_local_option_name
  wchar_t m_option_accelerator_char; 
  int m_option_accelerator_index;

  // option aliases (for hidden options)
  ON_wString m_local_option_alias[4];

  // option prompts used when a built-in number/color/string/toggle/list get happens
  ON_wString m_option_prompt;  // <- this one shows up then user types the number/toggle/list option name and presses Enter

  // simple option value
  // if empty string prompt looks like
  //   " local_option_name "
  // if not empty string prompt looks like
  //   " local_option_name=value "
  ON_wString m_local_simple_option_value;
  ON_wString m_english_simple_option_value;

  /////////////////////////////////////////////////////////////////
  // number options

  // number option value
  double m_number_option_value;
  double* m_number_option_value_ptr; // used for "automatic" number options
  int*    m_integer_option_value_ptr; // used for "automatic" integer and string options
  char    m_number_option_format;     // format used to display the number option
                                      // 0 = general
                                      // 1 = distance
                                      // 2 = angle in degrees
                                      // 3 = angle in radians
  char m_flags; // 0x01 = option value is displayed as "Varies"
                // 0x02 - empty string allowed in string mode
  char m_reserved[2];

  ON_Color m_color_option_value;
  ON_Color* m_color_option_value_ptr; // used for "automatic" color options

  // if TRUE, number must be an integer
  bool m_bIntegerNumberValue;

  // if != ON_UNSET_VALUE, number value must be >= lower_limit
  double m_number_lower_limit;

  // if != ON_UNSET_VALUE, number value must be <= upper_limit
  double m_number_upper_limit;

  // Returns:
  //   TRUE if this is a number option and the value passes
  //   the m_bIntegerNumberValue, m_number_lower_limit, and
  //   m_number_upper_limit tests.
  bool ValidNumber(double value) const;

  /////////////////////////////////////////////////////////////////
  // toggle options

  // prompt looks like
  //   " local_option_name=m_toggle_option_OnOff[m_toggle_option_value?1:0] "
  int m_toggle_option_value; // -1 = unset, 1 = true, 0 = false
  BOOL32* m_toggle_BOOL_option_value_ptr; // used for "automatic" toggle options
  bool* m_toggle_bool_option_value_ptr; // used for "automatic" toggle options
  ON_wString m_local_toggle_option_OffOn[2];  // <- this one shows up on command window prompt
  ON_wString m_english_toggle_option_OffOn[2]; // this is for _english script processing

  /////////////////////////////////////////////////////////////////
  // list options

  // list option stuff
  // prompt looks like
  //   " local_option_name=m_list_option_values[m_list_option_current] "
  ON_ClassArray<ON_wString> m_local_list_option_values; // <- this one shows up on command window prompt
  ON_ClassArray<ON_wString> m_english_list_option_values; // this is for _english script processing
  int m_list_option_current; // 0 based index into m_list_option_values[] array

  /////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
  // command line display information - set/used by the command line formatter
  void SetDisplayString( int nUniqueChars ) const; // sets m_display_string
  ON_wString m_display_string; // (volatile) contains formatting instructions
  RECT m_display_rect;         // (volatile) for hit testing mouse picks

  // If true the list/number/color/toggle option value is displayed as "Varies"
  void SetVaries(bool bVaries);
  bool Varies() const;

  // If true an empty string is allowed when type is string_option
  void SetAllowEmptyString(bool bAccept);
  bool AllowEmptyString() const;
};

