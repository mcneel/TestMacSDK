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

#include "rhinoSdkGet.h"

class RHINO_SDK_CLASS CRhinoGetString : public CRhinoGet
{
public:
  CRhinoGetString();
  ~CRhinoGetString();
  CRhinoGetString(const CRhinoGet&); // prompt and options inherited
  CRhinoGetString& operator=(const CRhinoGetString&); // prompt and options inherited

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //         and command options.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Call GetString() or GetLiteralString() to get a string.
  //         GetString() is the standard and you should almost always
  //         use this.  GetLiteralString() is used to get the entire
  //         line (up to an Enter key press or a new-line in a script
  //         file.).  For example, GetLiteralString() is used to get
  //         input for lisp, python, etc. scripts. 
  //
  //
  CRhinoGet::result GetString();

  CRhinoGet::result GetLiteralString();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Use result code returned by GetString() to determine what
  //         CRhinoGet function to call to get the input.
  //
  //         result               function
  //           CRhinoGet::cancel    user pressed ESCAPE
  //           CRhinoGet::nothing   user pressed ENTER
  //           CRhinoGet::undo      user wants to UNDO
  //           CRhinoGet::option    CRhinoGet::Option()
  //           CRhinoGet::number    CRhinoGet::Number()
  //           CRhinoGet::string    CRhinoGet::String()
  //
protected:
  // used by CRhinoGetColor, CRhinoGetNumber, and CRhinoGetOption
  CRhinoGetString(TYPE);
  CRhinoGetString(TYPE,const CRhinoGet&);
};

class RHINO_SDK_CLASS CRhinoGetOption : public CRhinoGetString
{
public:
  CRhinoGetOption();
  ~CRhinoGetOption();
  CRhinoGetOption(const CRhinoGet&);                  // prompt and options inherited
  CRhinoGetOption& operator=(const CRhinoGetOption&); // prompt and options inherited

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //         and command options.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Call GetOption() to get a option.
  //
  // A return value of CRhinoGet::option means the user selected a
  // valid option.  Use CRhinoGet::Option() to determine which option
  // was selected.
  CRhinoGet::result GetOption();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Use result code returned by GetOption() to determine what
  //         CRhinoGet function to call to get the input.
  //
  //         result               function
  //           CRhinoGet::cancel    user pressed ESCAPE
  //           CRhinoGet::nothing   user pressed ENTER
  //           CRhinoGet::undo      user wants to UNDO
  //           CRhinoGet::option    CRhinoGet::Option()
  //           CRhinoGet::number    CRhinoGet::Number()
  //           CRhinoGet::string    CRhinoGet::String()
  //

private:
  // If you want to explicitly get string input, then 
  // use CRhinoGetString with options.
  // If you want to only get options, then use CRhinoGetOption,
  CRhinoGet::result GetString();
};


//////////////////////////////////////////////////////////////////////////
//
// CRhinoGetNumber is used to get floating point numbers
//
class RHINO_SDK_CLASS CRhinoGetNumber : public CRhinoGetString
{
public:
  CRhinoGetNumber();
  ~CRhinoGetNumber();
  CRhinoGetNumber(const CRhinoGet&);                  // prompt and options inherited
  CRhinoGetNumber(const CRhinoGetNumber&);
  CRhinoGetNumber& operator=(const CRhinoGetNumber&);


  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  /*
  Description:
    Set a lower limit on the number that can be returned.  
    By default there is no lower limit.
  Parameters:
    lower_limit - [in] smallest acceptable number.  Pass ON_UNSET_VALUE
        if you want to disable a previously set lower_limit.
    bStrictlyGreaterThan - [in] (default=FALSE) If TRUE, then
        the returned number will be > lower_limit.
  See Also:
    CRhinoGetNumber::SetUpperLimit
  */
  void SetLowerLimit( 
    double lower_limit,
    BOOL32 bStrictlyGreaterThan = FALSE
    );


  /*
  Description:
    Set a upper limit on the number that can be returned.
    By default there is no upper limit.
  Parameters:
    upper_limit - [in] largest acceptable number.  Pass ON_UNSET_VALUE
        if you want to disable a previously set upper_limit.
    bStrictlyLessThan - [in] (default=FALSE) If TRUE, then
        the returned number will be < upper_limit.
  See Also:
    CRhinoGetNumber::SetLowerLimit
  */
  void SetUpperLimit( 
    double upper_limit,
    BOOL32 bStrictlyLessThan = FALSE
    );

  //////////////////////////////////////////////////////////////////
  //
  // STEP 4: Call GetNumber() to get an integer
  //
  //
  CRhinoGet::result GetNumber();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Use result code returned by GetNumber() to determine what
  //         CRhinoGet function to call to get the input.
  //
  //         result               function
  //           CRhinoGet::cancel    user pressed ESCAPE
  //           CRhinoGet::nothing   user pressed ENTER
  //           CRhinoGet::undo      user wants to UNDO
  //           CRhinoGet::option    CRhinoGet::Option()
  //           CRhinoGet::number    CRhinoGet::Number()
  //           CRhinoGet::string    CRhinoGet::String()
  //

protected:
  int m_lower_limit_flag; // 0 = none, 1 = m_lower_limit <= result, 2 = m_lower_limit <= result;
  int m_upper_limit_flag; // 0 = none, 1 = result <= m_upper_limit, 2 = result < m_upper_limit
  double m_lower_limit; // default is ON_UNSET_VALUE
  double m_upper_limit; // default is ON_UNSET_VALUE

private:
  // If you want to get a string, then use CRhinoGetString.
  // If you want to get a number, then use CRhinoGetNumber.
  CRhinoGet::result GetString();
};


//////////////////////////////////////////////////////////////////////////
//
// CRhinoGetInteger is used to get integer numbers
//
class RHINO_SDK_CLASS CRhinoGetInteger : public CRhinoGetNumber
{
public:
  CRhinoGetInteger();
  ~CRhinoGetInteger();
  CRhinoGetInteger(const CRhinoGet&);                  // prompt and options inherited
  CRhinoGetInteger& operator=(const CRhinoGetInteger&); // prompt and options inherited


  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: Use CRhinoGet member functions to specify command prompt
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: Use CRhinoGet member functions to specify what types of
  //         alternate input can be accepted.
  //

  //////////////////////////////////////////////////////////////////
  //
  // STEP 3: Call GetNumber() to get an integer
  //
  //
  CRhinoGet::result GetInteger();

  //////////////////////////////////////////////////////////////////
  //
  // STEP 5: Use result code returned by GetString() to determine what
  //         CRhinoGet function to call to get the input.
  //
  //         result               function
  //           CRhinoGet::cancel    user pressed ESCAPE
  //           CRhinoGet::nothing   user pressed ENTER
  //           CRhinoGet::undo      user wants to UNDO
  //           CRhinoGet::option    CRhinoGet::Option()
  //           CRhinoGet::number    CRhinoGet::Number()
  //           CRhinoGet::string    CRhinoGet::String()
  //
  int Number();
};

class RHINO_SDK_CLASS CRhinoGetColor : public CRhinoGetString
{
public:
  CRhinoGetColor();
  ~CRhinoGetColor();
  CRhinoGetColor(const CRhinoGet&);
  CRhinoGetColor& operator=(const CRhinoGetColor&);

  CRhinoGet::result GetColor();
  CRhinoGet::result GetColor(bool bUseAlpha);

private:
  // If you want to get a string, then use CRhinoGetString.
  // If you want to get a color, then use CRhinoGetColor.
  CRhinoGet::result GetString();
};

