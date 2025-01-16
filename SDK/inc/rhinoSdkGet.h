//
// Copyright (c) 1993-2021 Robert McNeel & Associates. All rights reserved.
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

#include "rhinoSdkCommandOptionName.h"
#include "rhinoSdkCommandOption.h"
#include "rhinoSdkCommand.h"


#if defined (ON_RUNTIME_APPLE)
class CRhGetPointHelper
{
  // The only instance of this class is the
  // static s_GET_POINT_HELPER.
  //
  // s_GET_POINT_HERLPER remembers status of what
  // we last sent to get point so repeated key
  // down events don't get reported zillions of
  // times.
public:
  CRhGetPointHelper() {Init();}
  
  // state of shift, control, and alt key on last mouse move
  bool m_bShiftDown;
  bool m_bCtrlDown;
  bool m_bAltDown;
  
  // each TAB key press toggles TAB mode.
  bool m_bTabMode;
  
  // true if next TAB down event should toggle m_bTabMode
  bool m_bEnableTabToggle;
  
  // true if a keypress was short enough to count as a toggle
  bool m_bCountAsToggle;
  
  unsigned int m_get__sn;
  ON_2iPoint m_alt_down_point;
  
  void Init();
};
#endif

/*
Description:
  Easy to use point getter.
Parameters:
  prompt - [in] command prompt
  bAcceptNothing - [in] if true, the user can press enter
  point - [out] point value returned here
  history_value_id - [in]
     optional history value id.  If history_value_id > 0 and 
     history != NULL, and a snap is used to create the point,
     then a PointOnObject value is added.
  history - [in/out] optional history record
Returns:
  CRhinoCommand::success - got point
  CRhinoCommand::nothing - user pressed enter
  CRhinoCommand::cancel - user cancel point getting
Remarks:
  If you need options or more advanced user interface, then
  use CRhinoGetPoint.
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetPoint( 
        const wchar_t* prompt,
        BOOL32 bAcceptNothing,
        ON_3dPoint& point
        );

RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetPoint( 
        const wchar_t* prompt,
        BOOL32 bAcceptNothing,
        ON_3dPoint& point,
        int history_value_id,
        CRhinoHistory* history
        );

/*
Description:
  Easy to use color getter.
Parameters:
  prompt - [in] command prompt
  bAcceptNothing - [in] if true, the user can press enter
  output_color - [out] color value returned here
Returns:
  CRhinoCommand::success - got color
  CRhinoCommand::nothing - user pressed enter
  CRhinoCommand::cancel - user cancel color getting
Remarks:
  If you need options or more advanced user interface, then
  use CRhinoGetColor.
See Also:
  CRhinoGetColor
  CRhinoGet::AddCommandOptionColor
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetColor( 
  const wchar_t* prompt, 
  BOOL32 bAcceptNothing,
  ON_Color& output_color
  );

RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetColor(
  const wchar_t* prompt,
  BOOL32 bAcceptNothing,
  BOOL32 bUseAlpha,
  ON_Color& output_color
);

/*
Description:
  Easy to use string getter.
Parameters:
  prompt - [in] command prompt
  bAcceptNothing - [in] if true, the user can press enter
  output_string - [in/out] default string set to this value and 
                           string value returned here
Returns:
  CRhinoCommand::success - got string
  CRhinoCommand::nothing - user pressed enter
  CRhinoCommand::cancel - user cancel string getting
Remarks:
  If you need options or more advanced user interface, then
  use CRhinoGetString.
See Also:
  CRhinoGetString
  CRhinoGet::AddCommandOption
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetString( const wchar_t* prompt, 
                     BOOL32 bAcceptNothing,
                     ON_wString& output_string);

/*
Description:
  Easy to use number getter.
Parameters:
  prompt - [in] command prompt
  bAcceptNothing - [in] if true, the user can press enter
  is_integer - [in] if true then value constrainded to integer value
  output_number - [in/out] default number to this value and 
                           number value returned here
  lower_limit - [in] 
Returns:
  CRhinoCommand::success - got number
  CRhinoCommand::nothing - user pressed enter
  CRhinoCommand::cancel - user cancel number getting
Remarks:
  If you need options or more advanced user interface, then
  use CRhinoGetNumber.
See Also:
  CRhinoGetInteger
  CRhinoGet::AddCommandOptionNumber
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetNumber( const wchar_t* prompt, 
                     BOOL32 bAcceptNothing,
                     BOOL32   is_integer,
                     double& output_number,
                     double lower_limit = ON_UNSET_VALUE,
                     double upper_limit = ON_UNSET_VALUE
                     );

/*
Description:
  Easy to use integer getter.
Parameters:
  prompt - [in] command prompt
  bAcceptNothing - [in] if true, the user can press enter
  output_integer - [in/out] default integer to this value and 
                           integer value returned here
  lower_limit - [in] 
Returns:
  CRhinoCommand::success - got integer
  CRhinoCommand::nothing - user pressed enter
  CRhinoCommand::cancel - user cancel integer getting
Remarks:
  If you need options or more advanced user interface, then
  use CRhinoGetinteger.
See Also:
  CRhinoGetInteger
  CRhinoGet::AddCommandOptioninteger
*/
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetInteger( const wchar_t* prompt, 
                     BOOL32 bAcceptNothing,
                     int& output_number,
                     double lower_limit = ON_UNSET_VALUE,
                     double upper_limit = ON_UNSET_VALUE
                     );

 
// used by CRhinoGet::Line2d() to return 2d screen line.
typedef struct tagRH_LINE
{
  ON_2iPoint from;
  ON_2iPoint to;
} RH_LINE;
 
/*
  Base class for CRhinoGetObject, CRhinoGetPoint, CRhinoGetSphere, etc.
  
  You will never directly create a CRhinoGet but you will use its member
  functions after calling CRhinoGetObject::GetObject(), 
  CRhinoGetPoint::GetPoint(), and so on.

  Provides tools to set command prompt, set command options, and specify
  if the "get" can optionally accept numbers, nothing (pressing enter),
  and undo.
*/
class RHINO_SDK_CLASS CRhinoGet
{
public:
  /*
  Description:
    Test a string to see if it can be used as an option name in
    any of the CRhinoGet::AddCommandOption...() functions.
  Returns:
    true if string can be used as an option name.
  */
  static BOOL32 IsValidOptionName( const wchar_t* option_name );

  /*
  Description:
    Test a string to see if it can be used as an option value in
    CRhinoGet::AddCommandOption, CRhinoGet::AddCommandOptionToggle,
    or CRhinoGet::AddCommandOptionList.
  Returns:
    true if string can be used as an option value.
  */
  static BOOL32 IsValidOptionValueName( const wchar_t* option_value );
  
  enum TYPE
  {
    get_string   = 1,
    get_point    = 2,
    get_object   = 3,
    get_meshes   = 4,
    get_option   = 5,
    get_number   = 6,
    get_color    = 7,
    force_32bit_enum = 0xFFFFFFFF
  };

  CRhinoGet(TYPE);
  virtual ~CRhinoGet();
  CRhinoGet(TYPE,const CRhinoGet&);

  TYPE Type() const;

  /*
  Description:
    Set the document targeted by this get.
  Parameters:
    rhino_doc_sn - [in]
  */
  void SetDocument( unsigned int rhino_doc_sn );

  /*
  Returns;
    0 or the specific document for this get.
  Remarks:
    If no document is specified and no event has happened
    in something specifically associated with a document,
    then 0 is returned.
  */
  unsigned int DocumentRuntimeSerialNumber() const;

  /*
  Returns:
    true if the get is or will be a CRhinoGetPoint::GetPoint()
  */
  bool IsGetPoint() const;

  /*
  Returns:
    true if the get is or will be a CRhinoGetObject::GetObject()
  */
  bool IsGetObject() const;

  /*
  Returns:
    true if the get is or will be a CRhinoGetString::GetString()
  */
  bool IsGetString() const;

  /*
  Returns:
    true if the get is or will be getting meshes.
  */
  bool IsGetMeshes() const;

  /*
  Returns:
    true if the get is or will be a CRhinoGetString::GetNumber()
  */
  bool IsGetNumber() const;

  /*
  Returns:
    true if the get is or will be a CRhinoGetString::GetOption()
  */
  bool IsGetOption() const;

  /*
  Returns:
    true if the get is or will be a CRhinoGetString::GetColor()
  */
  bool IsGetColor() const;

  //////////////////////////////////////////////////////////////////
  //
  // Possible results from CRhinoGetObject::GetObject(),
  // CRhinoGetPoint::GetPoint(), etc.
  enum result 
  {
    no_result = 0,
    cancel    = 1, // user wants to cancel current command
    nothing   = 2,  // user pressed enter - typically used to accpt defaults
    option,   // user specified an option - call Option() to get option index
    number,   // user entered a real number - call Number() to get value
    color,    // user entered a color - call Color() to get value
    undo,     // user pressed undo
    miss,     // user clicked and missed
    point,    // user picked 3d point - call Point() to get 3d point
    point2d,  // user picked 2d window point in CRhinoGetPoint::Get2dPoint()
              //   call Point2d() to get the point and View() to get the view.
    line2d,   // user picked a 2d line in CRhinoGetPoint::Get2dLine()
              //   call Line2d() to get the line and View() to get the view.
    rect2d,   // user picked a 2d rectangle in CRhinoGetPoint::Get2dRectangle()
              //   call Rectangle2d() to get the rectangle and View() to get the view.
    object,   // user selected an object - call Object() to get object
    string,   // user typed a string - call String() to get the string
    winmsg,   // Windows posted a message id that was in the list passed
              // to CRhinoGet::AcceptWindowsMessage().  Call CRhinoGet::WndMsg()
              // to get the message.
    timeout,  // The getter waited for the amount of time specified
              // in CRhinoGet::SetWaitDuration() and then gave up.

    /////////////
    // higher level stuff
    //
    circle,   // call CRhinoGetCircle::GetCircle() to get the circle
    plane,    // call CRhinoGetPlane::GetPlane() to get the plane
    cylinder, // call CRhinoGetCylinder::GetCylinder() to get the cylinder
    sphere,   // call CRhinoGetSphere::GetSphere() to get the sphere
    angle,    // call CRhinoGetAngle::Angle() to get the angle in radians (CRhinoGetAngle() returns this for typed number, too)
    distance, // call CRhinoGetDistance::Distance() to get the distance value
    direction,// call CRhinoGetDirection::Direction() to get the direction vector
    frame,    // call CRhinoGetFrame::Frame() to get the frame that was picked

    number_preview, // internal, user is typing a number - call Number() to get value
    point_preview, // internal, user is typing a 3d point - call Point() to get 3d point

    user1 = 0xFFFFFFFF,
    user2 = 0xFFFFFFFE,
    user3 = 0xFFFFFFFD,
    user4 = 0xFFFFFFFC,
    user5 = 0xFFFFFFFB,

    exit_rhino = 0x0FFFFFFF // stop now, do not cleanup, just return ASAP.
  };

  //////////////////////////////////////////////////////////////////
  //
  // STEP 1: specify optional command prompt and command options
  //

  // Description:
  //   Set prompt message that appears in the command prompt
  //   window.
  // Parameters:
  //   prompt - [in] command prompt message.  Use the RHSTR
  //       macro if you want the message to be automatically
  //       localized.
  // Example:
  //   CRhinoGetPoint gp;
  //   gp.SetCommandPrompt( RhLocalizeString( L"Center point", 43543) );
  void SetCommandPrompt(const wchar_t* prompt);

  /*
  Description:
    Set message that describes what default value will
    be used if the user presses enter.  This description
    appears in angle brackets <> in the command prompt
    window.  You do not need to provide a default value
    description unless you explicitly enable AcceptNothing.
  Parameters:
    default_value - [in] description of default value.
  Example:
    ON_3dPoint default_center(2,3,4);
    CRhinoGetPoint gp;
    gp.SetCommandPrompt( RhLocalizeString( L"Center point", 43544) );
    gp.SetCommandPromptDefault( L"(2,3,4)" );
    gp.AcceptNothing(true);
    gp.GetPoint();
    if ( gp.Result() == CRhinoGet::nothing )
      point = default_center;
  Remarks:
    If you have a simple default point, number, or string, it is
    easier to use SetDefaultPoint, SetDefaultNumber, or SetDefaultString.
    SetCommandPromptDefault and AcceptNothing can be used for providing
    more advanced UI behavior.
  */
  void SetCommandPromptDefault(const wchar_t* default_value);

  /*
  Description:
    Set a point as default value that will be returned
    if the user presses the ENTER key during the get.
  Parameters:
    default_point - [in] value for default point
  Remarks:
    Calling SetDefaultPoint will automatically handle
    setting the command prompt default and reacting to
    the user pressing ENTER.  If the user presses enter
    to accept the default point, CRhinoGet::point is
    returned and CRhinoGet::GotDefault() will return
    true.  Calling SetDefaultPoint will clear any
    previous calls to SetDefaultString or SetDefaultNumber.
  */
  void SetDefaultPoint(ON_3dPoint default_point);

  /*
  Description:
    Set a number as default value that will be returned
    if the user presses ENTER key during the get.
  Parameters:
    default_number - [in] value for default number
    format - [in] If null, RhinoFormatNumber() is
       used to format the number.  If not null,
       format is used to format the number.  For
       example, if you want the default to always
       have "%g" format, then you would pass L"%g"
       as the format string.  A non-null format string
       must have EXACTLY one %...e, %...f, or %...g
       in it.
  Remarks:
    Calling SetDefaultNumber will automatically handle
    setting the command prompt default and reacting to
    the user pressing ENTER.  If the user presses ENTER
    to accept the default number, CRhinoGet::number is
    returned and CRhinoGet::GotDefault() will return
    true.  Calling SetDefaultNumber will clear any
    previous calls to SetDefaultString or SetDefaultPoint.
  */
  void SetDefaultNumber(double default_number, const wchar_t* format = nullptr);

  /*
  Returns:
    Get the default number value that will be returned
    if the user presses ENTER key during the get.
  */
  double DefaultNumber() const;

  /*
  Description:
    Set a number as default value that will be returned
    if the user presses ENTER key during the get.
  Parameters:
    default_integer - [in] value for default number
  Remarks:
    Calling SetDefaultInteger will automatically handle
    setting the command prompt default and reacting to
    the user pressing ENTER.  If the user presses ENTER
    to accept the default integer, CRhinoGet::number is
    returned and CRhinoGet::GotDefault() will return
    true.  Calling SetDefaultNumber will clear any
    previous calls to SetDefaultString or SetDefaultPoint.
  */
  void SetDefaultInteger(int default_integer);

  /*
  Description:
    Set a string as default value that will be returned
    if the user presses ENTER key during the get.
  Parameters:
    default_string - [in] value for default string
  Remarks:
    Calling SetDefaultString will automatically handle
    setting the command prompt default and reacting to
    the user pressing ENTER.  If the user presses ENTER
    to accept the default string, CRhinoGet::string is
    returned and CRhinoGet::GotDefault() will return
    true.  Calling SetDefaultString will clear any
    previous calls to SetDefaultNumber or SetDefaultPoint.
  */
  void SetDefaultString(const wchar_t* default_string);

  /*
  Description:
    Set a color as default value that will be returned
    if the user presses ENTER key during the get.
  Parameters:
    default_color - [in] value for default color
  Remarks:
    Calling SetDefaultColor will automatically handle
    setting the command prompt default and reacting to
    the user pressing ENTER.  If the user presses ENTER
    to accept the default color, CRhinoGet::color is
    returned and CRhinoGet::GotDefault() will return
    true.  Calling SetDefaultColor will clear any
    previous calls to SetDefaultString or SetDefaultPoint.
  */
  void SetDefaultColor(ON_Color default_color);

  /*
  Description:
    Set a color as default value that will be returned
    if the user presses ENTER key during the get.
  Parameters:
    default_color - [in] value for default color
    bUseAlpha     - [in] set to true to use the alpha value
  Remarks:
    Calling SetDefaultColor will automatically handle
    setting the command prompt default and reacting to
    the user pressing ENTER.  If the user presses ENTER
    to accept the default color, CRhinoGet::color is
    returned and CRhinoGet::GotDefault() will return
    true.  Calling SetDefaultColor will clear any
    previous calls to SetDefaultString or SetDefaultPoint.
  */
  void SetDefaultColor(ON_Color default_color, bool bUseAlpha);

  /*
  Returns:
    Get the default color value that will be returned
    if the user presses ENTER key during the get.
  */
  const ON_Color& DefaultColor() const;

  /*
  Description:
    Clears any defaults se using SetDefaultPoint, SetDefaultNumber,
    SetDefaultString, or SetCommandPromptDefault
  */
  void ClearDefault();

  /*
  Returns:
    true if user pressed ENTER to accept a default
    point, number, or string set using SetDefaultPoint,
    SetDefaultNumber, or SetDefaultString.
  */
  BOOL32 GotDefault() const;

  /*
  Description:
    Add a command line option.  If you are adding an option
    to get a number, string, toggle, or list value, then use
    the tools listed in the see also section.
  Parameters:
    option_name - [in] 
    option_value - [in] optional description of current value
                        (not automaically localized - use RHSTR()
                         when appropriate.)
    bHiddenOption - [in] if true, the option is not displayed on
        the command line and the full option name must be typed
        in order to activate the option.  
  Returns:
    option index value (>0) or 0 if option cannot be added.
  Example:
          CRhinoGetPoint gp;
          int grain_option_index = gp.AddCommandOption( 
                        RhLocalizeCommandOptionName( L"Grain", 43545), 
                        RhLocalizeCommandOptionValue( L"rice", 43546) );
          int fruit_option_index = gp.AddCommandOption( 
                        RhLocalizeCommandOptionName( L"Fruit", 43547), 
                        RhLocalizeCommandOptionValue( L"banana", 43548) 
                        );
          gp.GetPoint();
          if ( gp.Result() == CRhinoGet::option ) 
          {
            CRhinoCommandOption* opt = gp.Option();
            if ( opt->m_option_index == grain_option_index )
            {
              ... ask about grains 
            }
            else if ( opt->m_option_index == grain_option_index )
            {
              ... ask about fruits 
            }
          }
  */
  int AddCommandOption( 
        CRhinoCommandOptionName option_name,
        CRhinoCommandOptionValue option_value = CRhinoCommandOptionValue(nullptr,nullptr),
        BOOL32 bHiddenOption = false
        );

  /*
  Description:
    Add a simple command line option with a number as a value.
  Parameters:
    option_name - [in] english option description 
                       automatic localization uses command option
                       string table
    number_value - [in] current value
  Returns:
    option index value (>0) or 0 if option cannot be added.
  Remarks:
    In most cases it is easier to use AddCommandOptionNumber().
  Example:
          ON_3dPoint center;
          static radius r = 1.0;
          for(;;)
          {
            CRhinoGetPoint gp;
            gp.SetCommandPrompt(RhLocalizeString( L"Center point", 43549));
            int radius_option_index = gp.AddCommandOption( RhLocalizeCommandOptionName( L"Radius", 43550), r );
            gp.GetPoint();
            if ( gp.Result() == CRhinoGet::option ) 
            {
              CRhinoCommandOption* opt = gp.Option();
              if ( opt->m_option_index == radius_option_index )
              {
                //user typed "Radius" - manually prompt for new number
                CRhinoGetNumber gn;
                gn.SetCommandPrompt(RhLocalizeString( "Radius", 56330));
                gn.SetDefault(r);
                gn.GetNumber();
                if ( gn.Result() == CRhinoGet::number )
                  r = gn.Number();
              }
              continue;
            }
            if ( gp.Result() == CRhinoGet::point ) 
            {
              center = gp.Point();
              break;
            }
          }
  */
  int AddCommandOption(CRhinoCommandOptionName option_name, double number_value);

  int AddCommandOption(CRhinoCommandOptionName option_name, int number_value);

  /*
  Description:
    Add a command line option to get numbers and automatically
    save the value.
  Parameters:
    option_name - [in] english option description 
        Automatic localization uses command option string table.
    number_value - [in/out] pointer to current value of the number.
        If the user changes the nubmer's value, the value of 
        *number_value is changed but the current call to 
        GetPoint/GetString/... does not return.
    option_prompt - [in] option prompt shown if the user selects 
        this option.  If NULL, then option_name.m_local_option_name
        is used as the get number prompt.
    bIntegerValue - [in] (default=false) if true, number must be an integer
    lower_limit - [in] (default=ON_UNSET_VALUE which means no lower limit)
                       if set, the returned number  must be >= lower_limit
    upper_limit - [in] (default=ON_UNSET_VALUE which means no upper limit)
                       if set, the returned number  must be <= upper_limit
    number_format - [in]  This controls how strings are parsed to get a number
              and how the number is displayed in the command prompt.

              0 = general - RhinoParseNumber - decimal display
              1 = distance - RhinoParseNumber - decimal display with doc units
              2 = angle in degrees - RhinoParseAngleDegrees - decimal degrees display
              3 = angle in radians - RhinoParseAngle - decimal radians display
  Returns:
    option index value (>0) or 0 if option cannot be added.
  Remarks:
    If you need to explicitly react to a changed number, then use 
    override of CRhinoGet::AddCommandOption that takes a number
    as an option value.  See the CRhinoGet::AddCommandOption examples.
  Example:
          ON_3dPoint center;
          static radius r = 1.0;
          double min_radius = 1.0e-12;
          for(;;)
          {
            CRhinoGetPoint gp;
            gp.SetCommandPrompt(RhLocalizeString( L"Center point", 43551));
            int radius_option_index = gp.AddCommandOptionNumber( 
                                              RhLocalizeCommandOptionName( L"Radius", 43552), 
                                              &r, 
                                              RhLocalizeString( L"New radius", 43553), 
                                              false, 
                                              min_radius 
                                              );
            gp.GetPoint();
            if ( gp.Result() == CRhinoGet::point ) 
            {
              center = gp.Point();
              break;
            }
            if ( gp.Result() == CRhinoGet::option )
            {
              const CRhinoCommandOption* opt = gp.Option();
              if ( opt->m_option_index == radius_option_index )
              {
                // r has been changed to a number >= min_radius.  Do
                // any additional validation or changes here.
              }
            }
            if ( gp.Result() == CRhinoGet::cancel ) 
              break; // user pressed ESCAPE to cancel get point
            ...
          }
  */
  int AddCommandOptionNumber( 
        CRhinoCommandOptionName option_name,
        double* number_value,
        const wchar_t* option_prompt = nullptr,
        BOOL32 bIntegerValue = false,
        double lower_limit = ON_UNSET_VALUE,
        double upper_limit = ON_UNSET_VALUE,
        int number_format = 0
        );

  /*
  Description:
    Add a command line option to get integers and automatically
    save the value.
  Parameters:
    option_name - [in] english option description 
        Automatic localization uses command option string table.
    integer_value - [in/out] pointer to current value of the number.
        If the user changes the integer's value, the value of 
        *integer_value is changed but the current call to 
        GetPoint/GetString/... does not return.
    option_prompt - [in] option prompt shown if the user selects
        this option.  If NULL, then option_name.m_local_option_name
        is used as the get number prompt.
    lower_limit - [in] (default=ON_UNSET_VALUE which means no lower limit)
                       if set, the returned integer  must be >= lower_limit
    upper_limit - [in] (default=ON_UNSET_VALUE which means no upper limit)
                       if set, the returned integer  must be <= upper_limit
  Returns:
    option index value (>0) or 0 if option cannot be added.

  Example:
       CRhinoGetPoint gp;
       ...
       int favorite_integer = 8;
       gp.AddCommandOptionInteger(
           RhLocalizeCommandOptionName( L"FavoriteInteger", 43554),
           &favorite_integer,
           RhLocalizeString( L"What is your favorite integer?", 43555)
           );
       ...
  Remarks:
    If you need to explicitly react to a changed number, then use 
    override of CRhinoGet::AddCommandOption that takes a number
    as an option value.  See the CRhinoGet::AddCommandOption examples.
  */
  int AddCommandOptionInteger( 
        CRhinoCommandOptionName option_name,
        int* integer_value,
        const wchar_t* option_prompt = nullptr,
        double lower_limit = ON_UNSET_VALUE,
        double upper_limit = ON_UNSET_VALUE
        );

  /*
  Description:
    Add a command line option to get colors and automatically
    save the value.
  Parameters:
    option_name - [in] english option description 
        Automatic localization uses command option string table.
    color_value - [in/out] pointer to current value of the number.
        If the user changes the integer's value, the value of 
        *integer_value is changed but the current call to 
        GetPoint/GetString/... does not return.
    option_prompt - [in] option prompt shown if the user selects
        this option.  If NULL, then option_name.m_local_option_name
        is used as the get number prompt.

  Example:
       CRhinoGetPoint gp;
       ...
       ON_Color favorite_color(0,0,0);
       gp.AddCommandOptionColor(
           RhLocalizeCommandOptionName( L"FavoriteColor", 43556),
           &favorite_color,
           RhLocalizeString( L"What is your favorite color?", 43557)
           );
       ...
  Returns:
    option index value (>0) or 0 if option cannot be added.
  */
  int AddCommandOptionColor( 
        CRhinoCommandOptionName option_name,
        ON_Color* color_value,
        const wchar_t* option_prompt = nullptr
        );

  /*
  Description:
    Add a command line option to toggle a setting.
  Parameters:
    option_name - [in] english option description 
                       automatic localization uses command option
                       string table
    option_off_value - [in] english "off" value
                       automatic localization uses command option
                       string table
    option_on_value - [in] english "on" value
                       automatic localization uses command option
                       string table
    bToggleState - [in] current toggle state (true = on)
    pbCurrentState - [in/out] if not NULL, then the value of 
        *pbCurrentState is kept set to the current state.
  Returns:
    option index value (>0) or 0 if option cannot be added.
  Example:
          ON_3dPoint center;
          static bLights = true;
          for(;;)
          {
            CRhinoGetPoint gp;
            gp.SetCommandPrompt(RhLocalizeString( L"Center point", 43558));
            int togglelights_option_index = gp.AddCommandOptionToggle( 
                                                RhLocalizeCommandOptionName( L"Lights", 43559), 
                                                RhLocalizeCommandOptionValue( L"Off", 43560), 
                                                RhLocalizeCommandOptionValue( L"On", 43561), 
                                                bLights, 
                                                &bLights );
            gp.GetPoint();
            if ( gp.Result() == CRhinoGet::option ) 
            {
              CRhinoCommandOption* opt = gp.Option();
              if ( opt->m_option_index == togglelights_option_index )
              {
                // opt->m_toggle_option_value = current state of toggle
                // because we passed &bLights as the last argument, bLights
                // is automatically set to opt->m_toggle_option_value.
                // Do whatever else needs to be done here.
                ...
              }
              continue;
            }
            if ( gp.Result() == CRhinoGet::point ) 
            {
              center = gp.Point();
              break;
            }
          }
  */
  int AddCommandOptionToggle(
        CRhinoCommandOptionName option_name,
        CRhinoCommandOptionValue option_off_value,
        CRhinoCommandOptionValue option_on_value,
        BOOL32 bToggleState,
        BOOL32* bCurrentToggleState = nullptr
        );

  int AddCommandOptionToggle(
        CRhinoCommandOptionName option_name,
        CRhinoCommandOptionValue option_off_value,
        CRhinoCommandOptionValue option_on_value,
        bool bToggleState,
        bool* bCurrentToggleState = nullptr
        );

  /*
  Description:
    Add a command line list option
  Parameters:
    option_name - [in] english option description 
                       automatic localization uses command option
                       string table
    list_values_count - [in] number of values option_values[] array
    list_values - [in] array of english option values
                       automatic localization uses command option
                       string table
    list_current_index - [in] zero based index of current option
  Returns:
    option index value (>0) or 0 if option cannot be added.
  Example:
          ON_3dPoint center;
          int fruit_index = 0;
          CRhinoCommandOptionValue fruits[3];
          fruits[0] = RhLocalizeCommandOptionValue( L"banana", 43562);
          fruits[1] = RhLocalizeCommandOptionValue( L"apple", 43563);
          fruits[2] = RhLocalizeCommandOptionValue( L"orange", 43564);
          for(;;)
          {
            CRhinoGetPoint gp;
            gp.SetCommandPrompt(RhLocalizeString( L"Center point", 43565));
            int fruit_option_index = gp.AddCommandOptionList( RhLocalizeCommandOptionName( L"Fruit", 43566), 3, fruits, fruit_index );
            gp.GetPoint();
            if ( gp.Result() == CRhinoGet::option ) 
            {
              const CRhinoCommandOption* opt = gp.Option();
              if ( opt->m_option_index == fruit_option_index )
              {
                // .. get new fruit from list
                fruit_index = opt->m_list_option_current;
              }
              continue;
            }
            if ( gp.Result() == CRhinoGet::point ) 
            {
              center = gp.Point();
              break;
            }
          }
  */
  int AddCommandOptionList( 
        CRhinoCommandOptionName option_name,
        int list_values_count,
        const CRhinoCommandOptionValue* list_values,
        int list_current_index
        );

  int AddCommandOptionList( 
        CRhinoCommandOptionName option_name,
        const ON_SimpleArray<CRhinoCommandOptionValue>& list_values,
        int list_current_index
        );

  /*
  Description:
    Add a command line list option.  This version of AddCommandOptionList
    simply makes the appropriate array out of list0_value, ..., list9_value
    and 
  Parameters:
    option_name - [in] english option description 
                       automatic localization uses command option
                       string table
    list_current_index - [in] zero based index of current option
                All the listN_value parameters for N <= list_current_index
                must be provided.
    list0_value - [in] NULL or value for list_values[0]
    list1_value - [in] NULL or value for list_values[1]
    list2_value - [in] NULL or value for list_values[2]
    list3_value - [in] NULL or value for list_values[3]
    list4_value - [in] NULL or value for list_values[4]
    list5_value - [in] NULL or value for list_values[5]
    list6_value - [in] NULL or value for list_values[6]
    list7_value - [in] NULL or value for list_values[7]
    list8_value - [in] NULL or value for list_values[8]
    list9_value - [in] NULL or value for list_values[9]
  Returns:
    option index value (>0) or 0 if option cannot be added.
  Example:
          ON_3dPoint center;
          int fruit_index = 0;
          const wchar_t* fruits[3] = {L"banana",L"apple",L"orange"};
          for(;;)
          {
            CRhinoGetPoint gp;
            gp.SetCommandPrompt(RhLocalizeString( L"Center point", 43567));
            int fruit_option_index = gp.AddCommandOptionList(
                                          RhLocalizeCommandOptionName( L"Fruit", 43568),
                                          fruit_index,
                                          RhLocalizeCommandOptionValue( L"banana", 43569),
                                          RhLocalizeCommandOptionValue( L"apple", 43570),
                                          RhLocalizeCommandOptionValue( L"orange", 43571)
                                          );
            gp.GetPoint();
            if ( gp.Result() == CRhinoGet::option ) 
            {
              CRhinoCommandOption* opt = gp.Option();
              if ( opt->m_option_index == fruit_option_index )
              {
                .. get new fruit from list
              }
              continue;
            }
            if ( gp.Result() == CRhinoGet::point ) 
            {
              center = gp.Point();
              break;
            }
          }
  */
  int AddCommandOptionList( 
        CRhinoCommandOptionName option_name,
        int list_current_index,
        CRhinoCommandOptionValue list0_value = CRhinoCommandOptionValue(nullptr,nullptr),
        CRhinoCommandOptionValue list1_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list2_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list3_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list4_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list5_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list6_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list7_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list8_value = CRhinoCommandOptionValue(nullptr, nullptr),
        CRhinoCommandOptionValue list9_value = CRhinoCommandOptionValue(nullptr, nullptr)
        );

  /*
  Description:
    Add a command line option to get a string and automatically
    save the value.
  Parameters:
    option_name - [in] english option description
        Automatic localization uses command option string table.
    string_value - [in/out] pointer to current value of the string.
        If the user changes the string's value, the value of
        *string_value is changed.
    option_prompt - [in] option prompt shown if the user selects
        this option.  If NULL, then option_name.m_local_option_name
        is used as the get string prompt.
    bAllowEmptyString - [in] (default=false) if true, empty string is allowed

  Returns:
    option index value (>0) or 0 if option cannot be added.
  Remarks:
    If you need to explicitly react to a changed number, then use
    override of CRhinoGet::AddCommandOption that takes a number
    as an option value.  See the CRhinoGet::AddCommandOption examples.
  */
  int AddCommandOptionString(
    CRhinoCommandOptionName option_name,
    ON_wString* string_value,
    const wchar_t* get_string_prompt = nullptr, // localized string passed in
    bool bAllowEmptyString = false
  );

  //////////
  // Clear all command line options.
  void ClearCommandOptions(); // clear all command options 

  //////////
  // Set option that uses "option_name=value" format to show the value as "Varies"
  void SetOptionVaries(int option_index, bool bVaries);

  //////////////////////////////////////////////////////////////////
  //
  // STEP 2: specify types of keyboard input can be accepted
  //

  /*
  Description:
    Control the availability of transparent commands during
    the get.
  Parameters:
    bEnableTransparentCommands - [in] If true, then transparent
      commands can be run during the get.  If false, then
      transparent commands cannot be run during the get.
  Remarks:
    Some Rhino commands are "transparent" and can be run inside
    of other commands.  Examples of transparent commands include
    the view manipulation commands like ZoomExtents, Top, etc.,
    and the selection commands like SelAll, SelPoint, etc.
    By default transparent commands can be run during any get.
    If you want to disable this feature, then call
    EnableTransparentCommands(false) before calling GetString, GetPoint,
    GetObject, etc.
  */
  void EnableTransparentCommands(BOOL32 bEnableTransparentCommands);

  //////////
  // If you want to allow the user to be able to press enter in order
  // to skip selecting a something in CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), etc., then call AcceptNothing() before
  // calling GetPoint()/GetObject().
  void AcceptNothing(BOOL32 b = true);

  //////////
  // If you want to allow the user to have an undo option in
  // CRhinoGetPoint::GetPoint(), CRhinoGetObject::GetObject(), etc.,
  // then call AcceptUndo() before calling GetPoint()/GetObject().
  void AcceptUndo(BOOL32 b = true);    // call to allow user undo previous selection.

  //////////
  // If you want to allow the user to be able to type in a number
  // during CRhinoGetPoint::GetPoint(), CRhinoGetObject::GetObject(),
  // etc., then call AcceptNumber() before calling GetPoint()/GetObject().
  // If the user chooses to type in a number, then the result code
  // CRhinoGet::number is returned and you can use CRhinoGet::Number()
  // to get the value of the number.  If you are using GetPoint and
  // you want "0" to return (0,0,0) instead of the number zero, then
  // set bAcceptZero = false.
  void AcceptNumber(bool b = true, bool bAcceptZero = true);

  //////////
  // If you want to allow the user to be able to type in a point
  // then call AcceptNumber() before calling GetPoint()/GetObject().
  // If the user chooses to type in a number, then the result code
  // CRhinoGet::point is returned and you can use CRhinoGet::Point()
  // to get the value of the point.
  void AcceptPoint(BOOL32 b = true);

  //////////
  // If you want to allow the user to be able to type in a color
  // r,g,b or name during CRhinoGetPoint::GetPoint(), 
  // CRhinoGetObject::GetObject(), etc.,
  // then call AcceptNumber() before calling GetPoint()/GetObject().
  // If the user chooses to type in a color, then the result code
  // CRhinoGet::color is returned and you can use CRhinoGet::Color()
  // to get the value of the color.  If the get accepts points,
  // then the user will not be able to type in r,g,b colors but will
  // be able to type color names.
  void AcceptColor(BOOL32 b = true);

  //////////
  // If you want to allow the user to be able to type in a string
  // during CRhinoGetPoint::GetPoint(), CRhinoGetObject::GetObject(), etc.,
  // then call AcceptString() before calling GetPoint()/GetObject().
  // If the user chooses to type in a string, then the result code
  // CRhinoGet::string is returned and you can use CRhinoGet::String()
  // to get the value of the string.
  void AcceptString(BOOL32 b = true);

  /*
  Description:
    Expert user tool that allows GetPoint, GetObject,
    GetString, etc., to return if a registered Windows
    message is posted by the static function
    CRhinoGet::PostCustomWindowsMessage
  Parameters:
    winmsg_id - [in] Windows message id in the range
                   0xC000 throught 0xFFFF.
  Returns:
    true if the message id is in the permitted range.
  Remarks:
    Study the Windows documentation for RegisterWindowMessage.
    AcceptWindowsMessage is intended to be used by experts.
    Support is not available.  If Windows posts a message with
    matching id while GetPoint, etc., is waiting for input,
    the the getter returns CRhinoGet::winmsg.  
    Call CRhinoGet::WndMsg() to get the message.
  */
  bool AcceptCustomWindowsMessage(UINT winmsg_id);

  /*
  Description:
    Posts a Windows message that can terminate a call
    tooGetPoint, GetObject, GetString, etc.
  Parameters:
    Msg - [in] Windows message id in the range
                   0xC000 throught 0xFFFF.
    wParam - [in]
    lParam - [in]
  */
  static void PostCustomWindowsMessage( 
          UINT Msg,
          WPARAM wParam,
          LPARAM lParam
          );

  /*
  Description:
    Expert user function to limit the amount of time
    GetPoint, GetObjects, GetString, etc., will wait
    for input.
  Parameters:
    seconds - [in] number of seconds to wait.  By default
           the getters will wait indefinitely.
  Remarks:
    If a getter times out, then CRhinoGet::timeout 
    is returned by GetPoint, GetObjects, GetString, etc.
  */
  void SetWaitDuration( double seconds );

  //////////////////////////////////////////////////////////////////
  //
  // STEP N: Get options/numbers if user specified on of the alternate
  //         objects.
  //

  //////////
  // Used to get result code returned by CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), etc.
  CRhinoGet::result Result() const; // returns result of the Get*() call

  /*
  Description:
    Handy tool for getting command result value from getter results.
  Returns:
    @untitled table
    CRhinoCommand::cancel         if CRhinoGet::Result() == CRhinoGet::cancel
    CRhinoCommand::exit_rhino     if CRhinoGet::Result() == CRhinoGet::exit_rhino
    CRhinoCommand::success        otherwise
  */
  CRhinoCommand::result CommandResult() const;


  //////////
  // Used to get option if CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), etc., returns CRhinoGet::option.
  const CRhinoCommandOption* Option() const; 

  /*
  Returns:
    If this->Result() = CRhinoGet::option, this->Option() is not nullptr,
    and this->Option()->m_option_index is greater than 0,  
    then this->Option()->m_option_index is returned.
    Otherwise 0 is returned.
  */
  int OptionIndex() const; 

  /// <summary>
  /// Get the number of available command line options. This information is useful
  /// when command flow depends on the existence of command line options.
  /// </summary>
  /// <returns>Number of available options.</returns>
  int OptionCount() const;

  /*
  Returns:
    If this->Result() = CRhinoGet::option, this->Option() is not nullptr, 
    and this->Option()->m_option_index is greater than zero,
    then this->Option()->m_type is returned.
    Otherwise CRhinoCommandOption::COMMAND_OPTION_TYPE::force_32bit_enum is returned.
  */
  CRhinoCommandOption::COMMAND_OPTION_TYPE OptionType() const;

  /*
  Returns:
    If this->Result() = CRhinoGet::option, this->Option() is not nullptr,
    this->Option()->m_option_index is greater than zero,
    and CRhinoCommandOption::COMMAND_OPTION_TYPE::list_option = this->Option()->m_type,
    then this->Option()->m_list_option_current is returned.
    Otherwise -1 is returned.
  */
  int OptionCurrentListOptionIndex() const;
  
  //////////
  // Used to get number if CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), etc., returns CRhinoGet::number.
  double Number() const;

  //////////
  // Used to get string if CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), etc., returns CRhinoGet::string.
  const wchar_t* String() const;

  //////////
  // Used to get point if Get*() returns CRhinoGet::point.
  ON_3dPoint Point() const;

  // Used to get direction if Get*() returns CRhinoGet::point.
  // (Set by some digitizers, but in general it's (0,0,0);
  ON_3dVector Vector() const; 

  //////////
  // Used to get color if Get*() returns CRhinoGet::color.
  ON_Color Color() const;

  //////////
  // Used to get view user clicked in during CRhinoGetPoint::GetPoint(),
  // CRhinoGetObject::GetObject(), etc.
  CRhinoView* View() const;

  /*
  Returns:
    If the get was a GetObjects() and the mouse was used
    to select the objects, then the returned rect has
    left < right and top < bottom.  This rect is the Windows
    GDI screen coordinates of the picking rectangle.
    View()->Viewport().GetPickXform( m_pick_rect, pick_xform )
    will calculate the picking transformation that was used.
    In all other cases, left=right=top=bottom=0;
  */
  ON_4iRect PickRect() const;

  /*
  Returns:
    Location in view of point in selected in CRhinoGetPoint::GetPoint()
    or CRhinoGetPoint::Get2dPoint().
    (0,0) = upper left corner of window
  Remarks:
    Call CRhinoGet::View() to get the view window.
  */
  ON_2iPoint Point2d() const;
  
  /*
  Returns:
    Value of first 3d world point typed in the command line during 
    a Get2dPoint()....if not set, value will be ON_unset_point
  */
  ON_3dPoint  WorldPoint1() const;
      
  /*
  Returns:
    Value of second 3d world point typed in the command line during 
    a Get2dPoint()....if not set, value will be ON_unset_point
  */
  ON_3dPoint  WorldPoint2() const;
  
  /*
  Returns:
    Location in view of 2d rectangle selected in
    CRhinoGetPoint::Get2dRectangle().
    rect.left < rect.right and rect.top < rect.bottom
    (0,0) = upper left corner of window
  Remarks:
    Call CRhinoGet::View() to get the view window.
  */
  ON_4iRect Rectangle2d() const;

  /*
  Returns:
    Location in view window of 2d line selected 
    in CRhinoGetPoint::Get2dLine().
    (0,0) = upper left corner of window
  Remarks:
    Call CRhinoGet::View() to get the view window.
  */
  RH_LINE Line2d() const;


  /*
  Description:
    Used to get Windows message if Get*() returns CRhinoGet::winmsg.
  Returns:
    Windows message.
  */
  MSG WindowsMessage() const;

  // low level tool used to identify the get
  unsigned int SerialNumber() const;

public:
  class CRhGetSdkExtension* m__rhino_get_sdk_extension;

private:
  unsigned int m_rhino_doc_sn = 0;

protected:
  CRhinoGet& operator=(const CRhinoGet&);

  void PreGet();
  virtual void SetCommandWndPrompt() const;

  /*
  Parameters:
    token - [in] event token to test
    bAcceptNumber - [in] if true, then a number is acceptable,
        even when m_bAcceptNumber is false.  This happens with
        GetPoint ortho distance constraints and automatic number
        options.
    bUpdatedOptionValue - [out] if an option with a valid value
        is specified, then *bUpdatedOptionValue is set to true.
    history - [out] (can be NULL) updated as needed.
  returns 
   * CRhinoGet::no_result if token is not acceptable input.
   * CRhinoGet::option and sets m_option_index if token is a
     valid option.  Use CRhinoGet::Option() to get option index.
   * CRhinoGet::number if user typed a number and a number
     is acceptable input.
   * CRhinoGet::string if user typed a string and a string
     is acceptable input.
   * CRhinoGet::undo if user typed ^Z or "Undo" and undo is an
     acceptable option.
  */
  CRhinoGet::result IsAcceptableInput( 
    const class CRhEventToken&, 
    BOOL32 bAcceptNumber,
    BOOL32* bUpdatedOptionValue,
    CRhinoHistory* history
    );

  // Helper for used by IsAcceptableInput for 
  // autoprocessing number/toggle/list command options
  CRhinoGet::result AutoProcessOption();

  // Helper used by IsAcceptableInput when parsing points
  virtual
  ON_Plane GetCS( bool bWorldCoordinates, bool bRelativeCoordinates );

public:
  // low level tool
  void AddHiddenCommandOptionAlias( 
    int option_index, 
    const wchar_t* alias1, 
    const wchar_t* alias2 = nullptr,
    const wchar_t* alias3 = nullptr,
    const wchar_t* alias4 = nullptr
    );

  /*
  Description:
    Set m_vector field.
  Parameters:
    v - [in];
  */
  void SetVector(const ON_3dVector& v);

protected:
  unsigned int m_rhino_view_sn = 0;

  // if left < right and top < bottom, this is the Windows
  // GDI screen coordinates of the picking rectangle.
  // m_view->Viewport().GetPickXform( m_pick_rect, pick_xform )
  // will calculate the picking transformation that was used.
  ON_4iRect m_pick_rect;

  BOOL32 m_bEnableTransparentCommands;
  BOOL32 m_bAcceptNothing;
  BOOL32 m_bAcceptUndo;
  BOOL32 m_bAcceptPoint; // 3d world point
  bool m_bAccept2dPoint; // 2d screen point (used by CRhinoGet2dPoint)
  bool m_bGetPointOnMouseUp; // true if getting point on mouse up
  bool m_bAcceptNumber;
  bool m_bAcceptNumberZero;
  BOOL32 m_bAcceptColor;
  BOOL32 m_bAcceptString;
  BOOL32 m_bAcceptString_0;
  CRhinoGet::result m_result;
  double m_number;
  ON_3dPoint m_point;   // 3d world point
  ON_3dVector m_vector; // 3d world vector (from some digitizers)
  ON_2iPoint  m_2dpoint; // 2d window point (used by CRhinoGetPoint::Get2dPoint)
  ON_2iPoint  m_2dget_point1; // first 2d rect corner-center / 2d line end
  ON_2iPoint  m_2dget_point2; // second 2d rect corner / 2d line end
  bool       m_2dClamp;    // true to clamp 2d coordinates to window
  int        m_2dget_step; // 0 = none, 1 = get first point, 2 get second point, 3 = done
  int        m_2dget_mode; // 0 = none, 
                           // 1 = corner-to-corner rect, 2 = center-to-corner rect, 
                           // 3 = start-to-end line, 4 = middle-to-end line
  ON_3dPoint m_2dget_wpoint1; // first 3d world point input during 2d get point
  ON_3dPoint m_2dget_wpoint2; // second 3d world point input during 2d get point
  int        m_2dget_pen_style;
  ON_wString m_2dget_second_prompt;
  CRhinoView* m_p2dPointView; // if not NULL, Get2dPoint()/Get2dRectangle() will use this view

  ON_wString m_string;
  ON_Color   m_color;

  ON_wString m_prompt;
  ON_wString m_prompt_default;

  const TYPE m_type;

#if defined (ON_RUNTIME_APPLE)
public:
  // owning document
  CRhinoDoc* Document() const;

  const ON_wString CommandPrompt() const;
  const ON_wString CommandPromptDefault() const;
  int HasDefaultValue() const;
  
  bool m_bShowManualEntryField;
  bool m_bAcceptEnterWhenDone;

  bool EnableTransparentCommandsValue() const;
  bool AcceptNothingValue() const;
  bool AcceptUndoValue() const;
  bool AcceptPointValue() const; // 3d world point
  bool Accept2dPointValue() const; // 2d screen point (used by CRhinoGet2dPoint)
  bool GetPointOnMouseUpValue() const; // true if getting point on mouse up
  bool AcceptNumberValue() const;
  bool AcceptNumberZeroValue() const;
  bool AcceptColorValue() const;
  bool AcceptStringValue() const;
  bool AcceptString_0Value() const;

  // There are instances of CRhinoGet that prompt with "Press Enter when Done." yet do not call AcceptNothing().
  // On the Mac, these instances need an additional call to AcceptEnterWhenDone() so the GetPointOptions dialog
  // can correctly enable the Done button.
  void AcceptEnterWhenDone(BOOL32 b = true);
  bool AcceptEnterWhenDoneValue() const;
#endif
  const CRhinoCommandOption* CommandOptionAtIndex(int option_index) const; 
  
  int CommandOptionCount() const; // see source for comments
  void RemoveCommandOptions( int option_index ); // see source for comments

protected:
  // list of windows message ids (in the range 0xC000 - 0xFFFF)
  // to wait for.  These are set by calling CRhinoGet::AcceptWindowsMessage()
  // before calling GetPoint/GetObjects/GetOption/GetString/...
  ON_SimpleArray<UINT> m_winmsg_id_list;
private:
  MSG m_winmsg; // 

protected:
  UINT m_wait_duration; // milliseconds

private:
  int m_have_default_value; // 0 = none, 1 = point, 2 = number, 3 = string, 4 = color
  ON_3dPoint m_default_point_value;
  double     m_default_number_value;
  ON_wString m_default_string_value;
  ON_Color   m_default_color_value;
  BOOL32 m_bGotDefaultValue;

protected:
  void SetOptionAccelerators();

private:
  BOOL32 IsValidOptionFirstChar( wchar_t ) const;
  int m_option_index;
  int m_auto_accept_number; // see CRhinoGet::PreGet()
  unsigned int m_get__sn;

  ON_ClassArray<CRhinoCommandOption> m_options;

  // no implementation to prohibit use
  CRhinoGet();
  CRhinoGet(const CRhinoGet&);

public:
  // these functions are used by CRhinoView::OnMouseDown/Move/Up - pretend they don't exist
  bool In2dGet() const; // true if getting 2d point

private:
  ON__UINT_PTR m_sdk_reserved = 0;
};

/*
  Description:
    Used to perform background calculations on different threads while waiting
    for completion or the user to press escape to cancel.
*/
class RHINO_SDK_CLASS CRhinoGetCancel
{
public:
  CRhinoGetCancel();
  CRhinoGetCancel(const CRhinoGetCancel&) = delete;
  CRhinoGetCancel& operator=(const CRhinoGetCancel&) = delete;
  virtual ~CRhinoGetCancel();

  class Task
  {
  public:
    /*
    Description:
      Each task is run using the overridden Run function. Override this
      function to do the actual work. Note that this function will be called
      on different threads than the main UI thread and it is likely that
      multiple Run calls will be occurring at the same time on different
      thread.
    Parameters:
      terminator [in] - It is advised to check TerminationRequested for the
        passed in terminator and gracefully exit Run as fast as possible.
        This is how you should check if a user has pressed the escape key.
    Returns:
      true if this task completed successfully and wants to have its
      OnTaskCompleted function called on the main thread
    */
    virtual bool Run(ON_Terminator* terminator) = 0;

    /*
    Description:
      Called on the main thread after a Run for a given Task has completed
    */
    virtual void OnTaskCompleted(CRhinoDoc* doc) {};
  };

  /*
    Description:
      Runs a number of asynchronous tasks and waits for either
      all of the tasks to complete or for the user to cancel by pressing escape
    Parameters:
      prompt - [in] prompt to display at the command prompt
      tasks - [in] the tasks to run
      doc - [in] document this run is associated with
    Returns:
      true if all tasks ran to completion
      false if the tasks were cancelled
  */
  bool Run(const wchar_t* prompt, const std::vector<std::shared_ptr<Task>>& tasks, const CRhinoDoc* pDoc) const;

  /*
    Description:
      When escape is pressed the ON_Terminator passed to all tasks is set to
      to a state where termination has been requested. The Run function will
      wait until all of the tasks have finished unless this function is called.
  */
  void DoNotWaitForTasksToFinishWhenCancelled(void);

  /*
    Description:
      Max number of concurrent tasks to run. By default, this is the number of processors - 2.
  */
  int MaxConcurrentTasks() const;

  /*
    Description:
      Set the max number of concurrent tasks to run.
  */
  void SetMaxConcurrentTasks(int);

  /*
    Description:
      Sets whether escape cancels the task while another application has focus.
  */
  void SetAllowCancelWhileAnotherApplicationHasFocus(bool b);

private:
  class Private;
  std::unique_ptr<Private> m_private;
  friend Private;
};
