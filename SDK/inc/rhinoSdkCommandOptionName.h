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

/*
Description:
  CRhinoCommandOptionName classes are used to pass command 
  option names, list option values, and toggle option values 
  to CRhinoGet::AddCommandOption...().  Use the RHCMDOPTNAME()
  macro to quickly create these.
*/
class RHINO_SDK_CLASS CRhinoCommandOptionName
{
public:

  // Sets both string pointers to NULL.
  CRhinoCommandOptionName();

  // Sets m_english_option_name = english_option_name
  // and m_local_option_name = local_option_name.
  CRhinoCommandOptionName( 
    const wchar_t* english_option_name,
    const wchar_t* local_option_name 
    );

  // The default copy constructor, destructor, and operator= are sufficient.

  // english command option name (can be NULL if m_local_string is set)
  const wchar_t* m_english_option_name;

  // localized command option name (can be NULL if m_english_string is set)
  const wchar_t* m_local_option_name;
};

#if !defined(RHCMDOPTNAME)
#define RHCMDOPTNAME(s) CRhinoCommandOptionName(s,s)
#endif

/*
Description:
  CRhinoCommandOptionValue classes are used to pass command 
  toggle option and list option values to 
  CRhinoGet::AddCommandOptionToggle() and 
  CRhinoGet::AddCommandOptionList().  Use the RHCMDOPTVALUE()
  macro to quickly create these.
*/
class RHINO_SDK_CLASS CRhinoCommandOptionValue
{
public:

  // Sets both string pointers to NULL.
  CRhinoCommandOptionValue();

  // Sets m_english_option_value = english_option_value 
  // and m_local_option_value = local__option_value.
  CRhinoCommandOptionValue( 
    const wchar_t* english_option_value,
    const wchar_t* local_option_value 
    );

  // The default copy constructor, destructor, and operator= are sufficient.

  // english command option value (can be NULL if m_local_option_value is set)
  const wchar_t* m_english_option_value;

  // localized command option value (can be NULL if m_english_option_value is set)
  const wchar_t* m_local_option_value;
};

#if !defined(RHCMDOPTVALUE)
#define RHCMDOPTVALUE(s) CRhinoCommandOptionValue(s,s)
#endif

