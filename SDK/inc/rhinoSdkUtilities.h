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
Parameters:
  rhino_app_version_quartet - [out]
    rhino_app_version_quartet[0] = major version
    rhino_app_version_quartet[1] = minor version (service release)
    rhino_app_version_quartet[2] = (year - 1000)*1000 + (day of year)
    rhino_app_version_quartet[3] = (hour*1000 + minute*10 + branch)
Returns:
  Rhino application version number = RHINO_VERSION_NUMBER.
Remarks:
  The application version is not the same as the RhinoSdkVersion()
  or the RhinoSdkServiceRelease().
*/
extern "C" RHINO_SDK_FUNCTION unsigned int RhinoAppVersionGetQuartet(
  unsigned int rhino_app_version_quartet[4]
);

/*
Returns:
  Rhino application version quartet as a string = RHINO_VERSION_NUMBER_STRING.
Remarks:
  The application version is not the same as the RhinoSdkVersion()
  or the RhinoSdkServiceRelease().
*/
extern "C" RHINO_SDK_FUNCTION const char* RhinoAppVersionQuartetAsString();

/*
Returns:
  Rhino application version quartet as a wide string = RHINO_VERSION_NUMBER_WSTRING.
Remarks:
  The application version is not the same as the RhinoSdkVersion()
  or the RhinoSdkServiceRelease().
*/
extern "C" RHINO_SDK_FUNCTION const wchar_t* RhinoAppVersionQuartetAsWideString();

/*
Returns:
  Rhino application version build description.
*/
extern "C" RHINO_SDK_FUNCTION const char* RhinoAppVersionBuildDescription();

/*
Description:
  Base version of the Rhino SDK supported by this Rhino executable.
  Rhino will only load plug-ins that were build with exactly the
  same version of the SDK.
  Same as RhinoApp().SdkVersion()
Returns:
  Rhino SDK version number created by ON_VersionNumberConstruct().
*/
extern "C" RHINO_SDK_FUNCTION unsigned int RhinoSdkVersion();

/*
Description:
  Service release of the Rhino SDK supported by this
  executable. Rhino will only load plug-ins that require a
  service release of <= this release number.

  For example, SR1 will load all plug-ins made with any SDK released
  up through and including the SR1 SDK.  But, SR1 will not load a
  plug-in built using the SR2 SDK.  If an "old" Rhino tries to load
  a "new" plug-in, the user is told that they have to get a free
  Rhino.exe update in order for the plug-in to load.  Rhino.exe
  updates are available from http://www.rhino3d.com.

  Same as RhinoApp().SdkServiceRelease()
Returns:
  Rhino service release number created by ON_VersionNumberConstruct().
*/
extern "C" RHINO_SDK_FUNCTION unsigned int RhinoSdkServiceRelease();

/*
Description:
  Calls Win32 ::GetLastError() to see if a Windows error has
  occurred.  If so, it invokes Rhino's standard error reporting
  code that is useful for debugging and then calls
  ::SetLastError(0) so the error is not reported multiple times.
  Many Windows system calls unconditionally call ::SetLastError(0),
  so you need to call RhinoGetLastWindowsError() liberally if you
  want to catch Windows errors.
Returns:
  Value reported by the call to ::GetLastError().
*/
RHINO_SDK_FUNCTION
DWORD RhinoGetLastWindowsError();

/*
Description:
  Allows you to ignore a instance of a Windows errors
  in order to prevent Rhino from reporting the error as
  a serious problem.
Parameters:
  winerr - [in]
    code of error to mask.
Example:

      RhinoGetLastWindowsError();
      ...
      call windows open file code in a situation
      where a missing file is a valid situation.
      ..
      // in this case, it is OK if file does not exist
      RhinoMaskLastWindowsError(ERROR_FILE_NOT_FOUND);
      RhinoGetLastWindowsError();
      ....

Returns:
  True if the value of ::GetLastError() matched winerr
  and the error was masked by calling ::SetLastError(0).
  True if the value of ::GetLastError() was zero.
  False if the value of ::GetLastError() was non-zero
  and did not match winerr.
*/
RHINO_SDK_FUNCTION
bool RhinoMaskLastWindowsError(DWORD winerr);

/*
Description:
  Returns true if the Rhino pug-in SDK is installed.
  Several commands, including "CheckNewObjects" and
  "TestErrorCheck" turn on full debugging dialogs
  when the Rhino SDK is installed.
*/
RHINO_SDK_FUNCTION
bool RhinoPlugInSDKInstalled();

/*
Description:
  Use a CRhinoDump object when you want text
  sent to an ON_TextLog to appear in the Rhino
  command window.
*/
class RHINO_SDK_CLASS CRhinoDump : public ON_TextLog
{
public:
  CRhinoDump() = default;
  ~CRhinoDump() = default;
  // appends text to log
  void AppendText(const char*) override;
  // appends UNICODE text to log
  void AppendText(const wchar_t*) override;
};

/*
Description:
  Display Rhino's color selection dialog.
Parameters:
  hwndParent - [in] Window handle of dialog's parent window.
  color - [in/out] Default color for dialog, and will receive
                   new color if function returns TRUE.
  include_button_colors - [in] Display button face and text options at
                               top of named color list.
  dialog_title - [in] The title of the dialog.
  use_alpha - [in] Display an alpha slider and use alpha value in color.
  hwndMessage - [in] Optional window that receives notifications when the color has changed.
                     Uses the registered window message "RhinoColorPickerChangedMessage".
                     The LPARAM of this message is a pointer to an ON_Color object.
Example:
          ON_Color color = RGB(0,0,0);
          if( RhinoColorDialogEx(::RhinoApp().MainWnd(), color) )
          {
            // do something
          }
Returns:
   TRUE - color changed
   FALSE - color not changed or dialog canceled
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoColorDialogEx(
  HWND hwndParent,
  ON_Color& color,
  bool include_button_colors = false,
  const wchar_t* dialog_title = nullptr,
  bool use_alpha = false,
  HWND hwndMessage = nullptr,
  const ON_ClassArray<CRhinoNamedColor>* pCustomNamedColors = nullptr
);

ON_DEPRECATED_MSG("use RhinoColorDialogEx")
RHINO_SDK_FUNCTION
BOOL32 RhinoColorDialog(
  HWND hwndParent,
  ON_Color& color,
  bool include_button_colors = false,
  const wchar_t* dialog_title = nullptr
);

/*
Description:
  Display Rhino's linetype selection dialog.
Parameters:
  hwndParent     - [in] Window handle of dialog's parent window.
  linetype_index - [in/out] Default linetype index for dialog, and will receive
                            new linetype index if function returns TRUE.
  bDisplayByLayer - [in] Displays the "ByLayer" linetype in the list
Returns:
  TRUE - Dialog completed successfully.
  FALSE - Dialog was canceled.
Remarks:
  If the user selects the "Continuous" linetype, then linetype_index will
  equal -1. If bDisplayByLayer = true and the user selects the "ByLayer"
  linetype, then linetype_index will equal -2.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoLinetypeDialog(
  HWND hwndParent,
  int& linetype_index,
  bool bDisplayByLayer = false
);

/*
Description:
  Display Rhino's print width selection dialog.
Parameters:
  hwndParent - [in] Window handle of dialog's parent window.
  print_width - [in/out] Default print width for dialog, and will receive
                         new print width if function returns TRUE.
Returns:
   TRUE - Dialog completed successfully.
   FALSE - Dialog was canceled.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoPrintWidthDialog(
  HWND hwndParent,
  double& print_width
);

/*
Description:
  Display Rhino's single layer selection dialog.
Parameters:
  hwndParent     - [in] Window handle of dialog's parent window.
  lpsPrompt      - [in] The prompt, or title, of the dialog window.
  layer_index    - [in/out] Default layer for the dialog, and will receive
                            new layer if function returns TRUE.
  bDisplayDialog - [in] If TRUE, a dialog box is displayed.
                        Otherwise, a command line version is displayed.
  bEnableNewLayerButton- [in] Enable the "New" layer button.
  bEnableSetLayerCurrentButton - [in] Enable the "Set layer current" checkbox.
  pbSetLayerCurrentButtonValue - [in/out] Default value for "Set layer current checkbox,
                                          and will receive dialog value if function
                                          return TRUE.

Returns:
   TRUE - Dialog or command completed successfully.
   FALSE - Dialog, or command was canceled.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoSelectLayerDialog(
  HWND hwndParent,
  const wchar_t* lpsPrompt,
  int& layer_index,
  BOOL32 bDisplayDialog = TRUE,
  BOOL32 bEnableNewLayerButton = FALSE,
  BOOL32 bEnableSetLayerCurrentButton = FALSE,
  BOOL32* pbSetLayerCurrentButtonValue = nullptr
);

/*
Description:
  Display Rhino's multiple layer selection dialog.
Parameters:
  hwndParent     - [in] Window handle of dialog's parent window.
  lpsPrompt      - [in] The prompt, or title, of the dialog window.
  layer_indices  - [in/out] Default layers for the dialog, and will receive
                         new layers if function returns TRUE.
  bDisplayDialog - [in] If TRUE, a dialog box is displayed.
                        Otherwise, a command line version is displayed.
  bEnableNewLayerButton- [in] Enable the "New" layer button.
Returns:
   TRUE - Dialog or command completed successfully.
   FALSE - Dialog, or command was canceled.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoSelectMultipleLayersDialog(
  HWND hwndParent,
  const wchar_t* lpsPrompt,
  ON_SimpleArray<int>& layer_indices,
  BOOL32 bDisplayDialog = TRUE,
  BOOL32 bEnableNewLayerButton = FALSE
);

/*
Description:
  Sets the display cursor.
Parameters:
  hCursor - [in] Handle to a cursor.
Returns:
  HCURSOR - Handle to the previous cursor.
*/
RHINO_SDK_FUNCTION
HCURSOR RhinoSetCursor(HCURSOR hCursor);

#if defined (ON_RUNTIME_APPLE) || defined(ON_RUNTIME_LINUX)
/*
Description:
  Gets the display cursor.
Returns:
  HCURSOR - Handle to the current cursor.
*/
RHINO_SDK_FUNCTION
HCURSOR RhinoGetCursor();
#endif

/*
Description:
  Parse a string to see if it is a number.
Parameters:
  string - [in] string to parse
  number_value - [out] value of number
Returns:
   0:   string does not parse to a number
  >0:   number of characters parsed
See Also:
  RhinoParsePoint
  RhinoFormatPoint
  RhinoFormatNumber
*/
RHINO_SDK_FUNCTION
int RhinoParseNumber(
  const wchar_t* string,
  double* number_value
);

/*
Description:
  Parse a string to see if it is an angle.
Parameters:
  string - [in] string to parse
  angle_radians_value - [out] value of angle in radians
Remarks:
  A "plain" number is assumed to be in degrees.
Returns:
   0:   string does not parse to an angle
  >0:   number of characters parsed
See Also:
  RhinoParsePoint
  RhinoFormatPoint
  RhinoFormatNumber
*/
RHINO_SDK_FUNCTION
int RhinoParseAngle(
  const wchar_t* string,
  double* angle_radians_value
);

RHINO_SDK_FUNCTION
int RhinoParseAngleDegrees(
  const wchar_t* string,
  double* angle_degrees_value
);

RHINO_SDK_FUNCTION
bool RhinoParsePythonFormula(
  const wchar_t* sformula,
  unsigned int sformula_count,
  double* value
);

/*
Returns:
  The angle unit system that is used when and angles with
  no explicitly or implicitly specified angle unit system
  are encountered while parsing formulas, lengths and points.
*/
RHINO_SDK_FUNCTION
ON::AngleUnitSystem RhinoParseDefaultAngleUnitSystem();

/*
Returns:
  0x0B0 - the ASCII and Unicode degrees symbol
*/
RHINO_SDK_FUNCTION
wchar_t RhinoDegreesSymbol();

/*
Description:
  Parse a string to see if it is an angle.
Parameters:
  angle_decimal_degrees - [in] angle in degrees
  str - [out]
    angle in deg min sec format.  When needed,
    the seconds have up to 4 decimal points.  This
    provides a linear precision of better than
    1 cm on the surface of the earth.
  bWithSpaces - [in]
    If true, there will be a space between the
    degrees, minutes, and seconds.  If false,
    there will not be a space.
Returns:
  Fractional seconds.  If this number is not zero,
  then the number of fractional seconds in the
  decimal angle.  This value can be used to
  make UI decisions about what format to use.
*/
RHINO_SDK_FUNCTION
double RhinoFormatDegreesInDMS(
  double angle_decimal_degrees,
  ON_wString& str,
  bool bWithSpaces = false
);

/*
Description:
  Convert a double to a latitude string.
Parameters:
  latitude_decimal_degrees - [in] latitude in degrees
  str - [out]
    latitude in deg min secN/S format
    If the decimal latitude is not exactly dms, then
    the decimal format is appended.
  format - [in]
    0 = degrees minutes seconds
    1 = decimal degrees
Returns:
  Fractional seconds.  If this number is not zero,
  then the number of fractional seconds in the
  decimal angle.  This value can be used to
  make UI decisions about what format to use.
*/
RHINO_SDK_FUNCTION
double RhinoFormatDegreesLatitude(
  double latitude_decimal_degrees,
  ON_wString& str,
  int format = 0
);

/*
Description:
  Convert a double to a longitude string.
Parameters:
  longitude_decimal_degrees - [in] longitude in degrees
  str - [out]
    Longitude in deg min secE/W format
    If the decimal latitude is not exactly dms, then
    the decimal format is appended.
  format - [in]
    0 = degrees minutes seconds
    1 = decimal degrees
    2 = degrees minutes seconds (decimal degrees)
Returns:
  Fractional seconds.  If this number is not zero,
  then the number of fractional seconds in the
  decimal angle.  This value can be used to
  make UI decisions about what format to use.
*/
RHINO_SDK_FUNCTION
double RhinoFormatDegreesLongitude(
  double longitude_decimal_degrees,
  ON_wString& str,
  int format = 0
);

/*
Description:
  Parse a string to see if it is an RGB color.
Parameters:
  string - [in] string to parse
  color_value - [out] value of number
Returns:
   0:   string does not parse to a color
  >0:   number of characters parsed
Remarks:
  An RGB string can have two formats.

    r,g,b where r, g, and b are integers in the range 0 to 255
    and no spaces are around the commas.

    A name from the color table RhinoApp().AppSettings().NamedColorList().

  Since the formats of r,g,b colors and points are the same,
  care must be taken to avoid confusing points and colors.
See Also:
  RhinoParsePoint
  RhinoFormatPoint
  RhinoFormatNumber
*/
RHINO_SDK_FUNCTION
int RhinoParseColor(
  const wchar_t* string,
  ON_Color* color_value
);

/*
Description:
  Parse a string to see if it is an RGB color.
Parameters:
  string - [in] string to parse
  color_value - [out] value of number
  bUseAlpha - [i] set to true if you want to parse for an alpha value
Returns:
   0:   string does not parse to a color
  >0:   number of characters parsed
Remarks:
  An RGB string can have two formats.

    r,g,b,a where r, g, b, and a are integers in the range 0 to 255
    and no spaces are around the commas.

    A name from the color table RhinoApp().AppSettings().NamedColorList().

  Since the formats of r,g,b colors and points are the same,
  care must be taken to avoid confusing points and colors.
See Also:
  RhinoParsePoint
  RhinoFormatPoint
  RhinoFormatNumber
*/
RHINO_SDK_FUNCTION
int RhinoParseColor(
  const wchar_t* string,
  ON_Color* color_value,
  bool bUseAlpha
);

/*
Description:
  Parse a string to see if it is a point.
Parameters:
  string - [in] string to parse
  point_value - [out] value of point
Returns:
   0:   string does not parse to a point
  >0:   number of characters parsed
Example:
  Parse a string that contains an unknown number of space
  delimited points like "1,2,3 4,5,6 7,8,9".

      ON_3dPointArray points;
      ON_3dPoint point;
      int i;
      const wchar_t* string = ...;
      for(;;)
      {
        i = RhinoParsePoint(string,point);
        if ( !i )
          break;
        points.Append(point);
        string += i;
      }

Remarks:
  RhinoParsePoint will parse strings of the form
  _x,y,z~ where "x", "y", and "z" are numbers,
  "_" can be zero or more white space characters,
  and "~" is any non-digit value.
See Also:
  RhinoParseNumber
  RhinoFormatPoint
  RhinoFormatNumber
*/
RHINO_SDK_FUNCTION
int RhinoParsePoint(
  const wchar_t* string,
  ON_3dPoint* point_value
);

/*
Description:
  Format a point for printing.
Parameters:
  point - [in]
  string - [in/out] string representation of point is
    appended to this string
  number_format - [in] (default=NULL) if not NULL, this
     string overrides the automatic format used by Rhino.
Example:

      ON_3dPoint point(1,2,3);
      ON_wString s = L"The point is (";
      RhinoFormatPoint( point, s, L"%0.2f" );
      s += ')';
      // The string "The point is (1.00,2.00,3.00)"

See Also:
  RhinoFormatNumber
  RhinoParseNumber
  RhinoParsePoint
*/
RHINO_SDK_FUNCTION
void RhinoFormatPoint(
  ON_3dPoint point,
  ON_wString& string,
  const wchar_t* number_format = nullptr
);

/*
Description:
  Format a color for printing.
Parameters:
  color - [in]
  string - [in/out] string representation of color is
    appended to this string
See Also:
  RhinoFormatNumber
  RhinoParseNumber
  RhinoParsePoint
*/
RHINO_SDK_FUNCTION
void RhinoFormatColor(
  ON_Color color,
  ON_wString& string
);

/*
Description:
  Format a color for printing.
Parameters:
  color - [in]
  string - [in/out] string representation of color is
    appended to this string
  bUseAlpha - [in] set to true to set alpha value
See Also:
  RhinoFormatNumber
  RhinoParseNumber
  RhinoParsePoint
*/
RHINO_SDK_FUNCTION
void RhinoFormatColor(
  ON_Color color,
  ON_wString& string,
  bool bUseAlpha
);

/*
Description:
  Format a number for printing.
Parameters:
  x - [in]
  string - [in/out] string representation of x is
    appended to this string
  number_format - [in] (default=NULL) if not NULL, this
     string overrides the automatic format used by Rhino.
  unit_system - [in]
     If unit_system != ON::LengthUnitSystem::None, then the name
     of the units is append to string.
  units - [in]
    x is assumed to be in units.m_unit_system.  The settings
    on units are used to control the display format and
    decimal precision.
  input_units - [in]
  output_units - [in]
    If output_units.m_unit_system != input_units.m_unit_system,
    then x is scaled.
    If output_units.m_display_mode is decimal or fractional and
    output_units.m_unit_system is not ON::LengthUnitSystem::None,
    then a localized unit system name is appended to string.
  bAppendUnitsName - [in]
Example:

        // plain old number
        double x = ...;
        ON_wString s1 = L"x = ";
        RhinoFormatNumber( x, s1 );

        // length in current unit system
        double length = ...;
        ON_wString s2 = L"length = ";
        CRhinoDoc* doc = CRhinoDoc::FromRuntimeSerialNumber(...);
        if ( doc )
        {
          RhinoFormatNumber( length, doc->UnitSystem(), s2, true );
        }
        else
        {
          RhinoFormatNumber( length, s2 );
        }

See Also:
  RhinoFormatPoint
  RhinoParseNumber
  RhinoParsePoint
*/
RHINO_SDK_FUNCTION
void RhinoFormatNumber(
  double x,
  ON_wString& string
);

RHINO_SDK_FUNCTION
void RhinoFormatNumber(
  double x,
  const ON_3dmUnitsAndTolerances& units,
  ON_wString& string,
  bool bAppendUnitSystemName = false
);

RHINO_SDK_FUNCTION
void RhinoFormatNumber(
  double x,
  const ON_3dmUnitsAndTolerances& input_units,
  const ON_3dmUnitsAndTolerances& output_units,
  ON_wString& string,
  bool bAppendUnitSystemName = false
);

// Returns true if formatting worked without fallbacks - no invalid input, overruns, etc
bool RhFormatNumber(
  double x,
  const ON_3dmUnitsAndTolerances& input_units,
  const ON_3dmUnitsAndTolerances& output_units,
  ON_wString& string,
  bool bAppendUnitSystemName,
  bool bBracketFractions
);

void RhFormatNumber(
  double x,
  ON_wString& string,
  bool bAppendUnitSystemName,
  bool bAbbreviate);

/*
Description:
  Returns a unit system name based on Rhino's current locale setting.
Parameters:
  bCapitalize - [in] Capitalize the first character (e.g. "Millimeter" vs "millimeter").
  bSingular - [in] Return the name in singular form (e.g. "millimeter" vs "millimeters").
  bAbbreviate - [in] Return the abbreviated name  (e.g. "mm" vs "millimeter").
Returns:
  The localized unit system name.
*/
RHINO_SDK_FUNCTION
const wchar_t* RhinoLocalizedUnitSystemName(
  bool bCapitalize,
  bool bSingular,
  bool bAbbreviate,
  ON::LengthUnitSystem length_unit_system
);

RHINO_SDK_FUNCTION
const wchar_t* RhinoLocalizedUnitSystemName(
  bool bCapitalize,
  bool bSingular,
  bool bAbbreviate,
  const ON_UnitSystem& unit_system
);

/*
Description:
  Converts a UUID to a string.
*/
RHINO_SDK_FUNCTION
void RhinoUuidToString(const UUID& uuid, wchar_t s[37]);

RHINO_SDK_FUNCTION
void RhinoUuidToString(const UUID& uuid, char s[37]);

/*
Description:
  Tests a string to see if it can be used as a name for
  an object, layer, material, group, font, etc.
Parameters:
  name - [in] name to test.
Remarks:
  Valid names do not have leading or trailing white space,
  do not contain DEL characters (127), and do not contain
  characters with values < 32.
Returns:
  TRUE if the string is valid name for an object, layer, material,
  group, font, etc.
See Also: CRhinoCommand::IsValidCommandName
*/
RHINO_SDK_FUNCTION
int RhinoIsValidName(const wchar_t* name);

RHINO_SDK_FUNCTION
int RhinoMessageBox( // The main application window is used as HWND
  const char* message, // text in message box
  const char* title, // title of message box
  UINT type          // style of message box
);

RHINO_SDK_FUNCTION
int RhinoMessageBox( // The main application window is used as HWND
  const wchar_t* message, // text in message box
  const wchar_t* title, // title of message box
  UINT type          // style of message box
);

RHINO_SDK_FUNCTION
int RhinoMessageBox(HWND hParent,
  const char* message, // text in message box
  const char* title, // title of message box
  UINT type   // style of message box
);

RHINO_SDK_FUNCTION
int RhinoMessageBox(HWND hParent,
  const wchar_t* message, // text in message box
  const wchar_t* title, // title of message box
  UINT type     // style of message box
#if defined (ON_RUNTIME_APPLE)
  , bool* suppressAlert = 0   // suppress additional alerts
#endif
);

/*
This class is for use in the common case where a calculation or process
may generate lots of error messages that need to be printed on
the Rhino command output window.  In addition, if the calculation is
happening in an interactive situation, a modal dialog box displays
the message as well.
*/
class RHINO_SDK_CLASS CRhinoDisplayErrorMessage
{
public:
  CRhinoDisplayErrorMessage();

  /*
  Parameters:
    bUseBatchMode - [in]
      false:
        PrintErrorMessage() will print the error message on the command line
        and display it in a modal message box requiring the user to
        click a button to continue.
      true:
        PrintErrorMessage() will print the error message on the command line.
        No user input will be required.
     sMessageBoxTitle - [in]
       Title for the modal message box.
  */
  CRhinoDisplayErrorMessage(
    bool bUseBatchMode,
    const wchar_t* sMessageBoxTitle
  );

  /*
  Displays the error message.  When a message box is used, it has an OK button.
  */
  void DisplayErrorMessage(
    const wchar_t* sErrorMessage
  ) const;

  /*
  Displays the error message.  When a message box is used, it has an OK and Cancel button.
  Returns:
    ID_OK if no message box was displayed or the user clicked the OK button.
    ID_CANCEL otherwise.
  */
  int DisplayOkCancelErrorMessage(
    const wchar_t* sErrorMessage
  ) const;

  /*
  Parameters:
    sMessageBoxTitle - [in]
      Title for the modal message box.
  */
  void SetMessageBoxTitle(
    const wchar_t* sErrorMessageBoxTitle
  );

  /*
  Parameters:
    bUseBatchMode - [in]
      false:
        PrintErrorMessage() will print the error message on the command line
        and display it in a modal message box requiring the user to
        click a button to continue.
      true:
        PrintErrorMessage() will print the error message on the command line.
        No user input will be required.
  */
  void SetUseBatchMode(
    bool bUseBatchMode
  );

private:
  ON_wString m_sMessageBoxTitle;
  bool m_bPrint;
  bool m_bMessageBox;
};

/*
Description:
  Display Rhino question message box.  This is a special Rhino dialog which
  may be used to ask Yes/No questions.  If you are going to ask the same
  question more than once you can include "Yes to All" and "No to All" buttons,
  see the nIncludeButtons parameter for details.
Parameters:
  hWndParent      - [in] Parent for message box.
  lpsText         - [in] Message to display
  lpsCaption      - [in] Optional caption to display as dialog title.  If this is
                         NULL or "" then default application title is used.
  nIncludeButtons - [in] Buttons to display, you may or the following:
                            1  = yes button
                            2  = yes to all button
                            4  = no button
                            8  = no to all button
                            16 = cancel_button
  nMessageBoxTypeCodes - [in] Optional message box codes.  Currently supports MB_ICON...
                              and MB_DEFBUTTON? arguments.  See documentation
                              for MessageBox()
Returns:
  0 = error displaying dialog
  1 = yes button
  2 = yes to all button
  3 = no button
  4 = no to all button
  5 = cancel button
*/
RHINO_SDK_FUNCTION
RhinoYesNoResponse RhinoYesNoMessageBox(
  HWND hWndParent,
  const wchar_t* lpsText,
  const wchar_t* lpsCaption = nullptr,
  int nIncludeButtons = 1 | 4, // Yes and No buttons
  UINT nMessageBoxTypeCodes = MB_ICONQUESTION | MB_DEFBUTTON1
);

/*
Description:
  Display Rhino question message box.  This is a special Rhino dialog which
  may be used to ask Yes/No questions.  If you are going to ask the same
  question more than once you can include "Yes to All" and "No to All" buttons,
  see the nIncludeButtons parameter for details.
Parameters:
  lpsText         - [in] Message to display
  lpsCaption      - [in] Optional caption to display as dialog title.  If this is
                         NULL or "" then default application title is used.
  nIncludeButtons - [in] Buttons to display, you may or the following:
                            1  = yes button
                            2  = yes to all button
                            4  = no button
                            8  = no to all button
                            16 = cancel_button
                            0xffffffff = all buttons
  nMessageBoxTypeCodes - [in] Optional message box codes.  Currently supports MB_ICON...
                              and MB_DEFBUTTON? arguments.  See documentation
                              for MessageBox()
Returns:
  0 = error displaying dialog
  1 = yes button
  2 = yes to all button
  3 = no button
  4 = no to all button
  5 = cancel button
*/
RHINO_SDK_FUNCTION
RhinoYesNoResponse RhinoYesNoMessageBox(
  const wchar_t* lpsText,
  const wchar_t* lpsCaption = nullptr,
  int nIncludeButtons = 1 | 4, // Yes and No buttons
  UINT nMessageBoxTypeCodes = MB_ICONQUESTION | MB_DEFBUTTON1
);

/*
Description:
  Rename an existing file for temporary backup purposes.
Parameters:
  filename - [in] name of existing file
  backup_filename - [out] new file name created by
    RhinoGetUnusedFilename(drive+path+$$rhino_temp$$+ext,...)
    Drive drive, path, and ext are from filename.
Returns:
  TRUE if candidate_filename is successfully renamed unused_filename.
  FALSE if filename did not exist or could not be renamed.  If FALSE
  is returned, then unused_filename is not changed.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoBackupFile(
  const wchar_t* filename,
  ON_wString& backup_filename
);

/*
Description:
  Make a Brep with 1 face from 3 or 4 points
Parameters:
  ob - [out] pointer to the finished Brep
  corners -  [in] 3 or 4 corner points for the face
  count - [in] number of corners
  tolerance - [in] min edge length without collapsing to a singularity
Returns:
  ON_Brep* - The resulting surface or NULL
Remarks:
  Calls RhinoCreateSurfaceFromCorners() to make the surface and trims it to a triangle for 3-corners
  Makes a singular surface for 4 corners with 2 coincident
  Memory for the Brep is allocated and becomes the responsibility
  of the caller.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoCreate1FaceBrepFromPoints(
  const ON_3dPoint corners[4],
  int count,
  double tolerance = 0
);

/*
Description:
  Make a CRhinoNurbsSurface from 4 corner points
Parameters:
  srf - [out] pointer to the finished surface
  a, b, c, d -  [in] 3 or 4 corner points for the face
  dtol - [in] min edge length without collapsing to a singularity
Returns:
  ON_NurbsSurface* - The resulting surface or NULL
Remarks:
  Memory for the surface is allocated and becomes the responsibility
  of the caller.
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoCreateSurfaceFromCorners(
  const ON_3dPoint& a,
  const ON_3dPoint& b,
  ON_3dPoint& c,
  ON_3dPoint& d,
  double dtol = 0
);

/*
Description:
  Creates a coons patch from 2, 3, or 4 curves
Parameters:
  curve_count [in] number of input curves
  pCrvs [in] Array of pointers to input curves
Returns:
  ON_Brep* - The resulting surface or NULL
  Memory for the Brep is allocated and becomes the responsibility
  of the caller.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoCreateEdgeSrf(
  int curve_count,
  const ON_Curve** pCrvs
);

/*
Description:
  Joins the SubDs in the input array to form as few as possible resulting SubDs.
  There may be more than one SubD in the result array.
Parameters:
  input_subds                - [in] Array of SubDs to join.
  join_tolerance             - [in] The join tolerance.
  bSubDJoinedEdgesAreCreases - [in] If true, merged boundary edges will be creases.
                                    If false, merged boundary edges will be smooth
  bPermitNonmanifoldSubD     - [in] - Permit non-manifold SubDs to be joined.
  output_subds               - [out] Array of output SubDs.
Returns:
  The number of SubDs added to the output array.
Remarks:
  NOTE: Memory for the output SubDs is allocated and becomes the responsibility of the caller.
  All of the geometry is copied and added to output_subds in one form or another.
  NOTE: Symmetry information is removed from newly joined SubDs. See also comments in
  RhinoJoinSubDs with the bPreserveSymmetry parameter.
*/
RHINO_SDK_FUNCTION
int RhinoJoinSubDs(
  const ON_SimpleArray<const ON_SubD*>& input_subds,
  double join_tolerance,
  bool bSubDJoinedEdgesAreCreases,
  bool bPermitNonmanifoldSubD,
  ON_SimpleArray<ON_SubD*>& output_subds
);

/*
Description:
  Joins the SubDs in the input array to form as few as possible resulting SubDs.
  There may be more than one SubD in the result array.
Parameters:
  input_subds                - [in] Array of SubDs to join.
  join_tolerance             - [in] The join tolerance.
  bSubDJoinedEdgesAreCreases - [in] If true, merged boundary edges will be creases.
                                    If false, merged boundary edges will be smooth
  bPermitNonmanifoldSubD     - [in] - Permit non-manifold SubDs to be joined.
  bPreserveSymmetry          - [in] If true, and if all inputs share the same symmetry, the output will also be symmetrical wrt. that symmetry.
                                    If If false, or true but no common symmetry exists, symmetry information is removed from all newly joined SubDs.
  output_subds               - [out] Array of output SubDs.
Returns:
  The number of SubDs added to the output array.
Remarks:
  NOTE: Memory for the output SubDs is allocated and becomes the responsibility of the caller.
  All of the geometry is copied and added to output_subds in one form or another.
*/
RHINO_SDK_FUNCTION
int RhinoJoinSubDs(
  const ON_SimpleArray<const ON_SubD*>& input_subds,
  double join_tolerance,
  bool bSubDJoinedEdgesAreCreases,
  bool bPermitNonmanifoldSubD,
  bool bPreserveSymmetry,
  ON_SimpleArray<ON_SubD*>& output_subds
);


/*
Description:
  Joins the Breps in the input array at any overlapping edges to form
  as few as possible resulting Breps.
  There may be more than one Brep in the output array.
Parameters:
  input_breps     [in] Array of Breps to join.
  tolerance       [in] 3d distance tolerance for detecting overlapping edges.
                       When in doubt, use the document's model absolute toleance.
  angle_tolerance [in] Angle tolerance, in radians, used for merging edges.
                       When in doubt, use the document's model angle toleance.
  output_breps    [out] Empty array to return results of the join.
Returns:
  true if any edges were joined.
Remarks:
  Memory for the output Breps is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoJoinBreps(
  const ON_SimpleArray<const ON_Brep*>& input_breps,
  double tolerance,
  double angle_tolerance,
  ON_SimpleArray<ON_Brep*>& output_breps
);

/*
Description:
  Joins the Breps in the input array at any overlapping edges to form
  as few as possible resulting Breps.
  There may be more than one Brep in the output array.
Parameters:
  input_breps     [in] Array of Breps to join.
  tolerance       [in] 3d distance tolerance for detecting overlapping edges.
                       When in doubt, use the document's model absolute toleance.
  angle_tolerance [in] Angle tolerance, in radians, used for merging edges.
                       When in doubt, use the document's model angle toleance.
  output_breps    [out] Empty array to return results of the join.
  output_key      [out] Array of integer arrays, one for each joined result, 
                        containing all input Brep indices that went into making it.
Returns:
  true if any edges were joined.
Remarks:
  Memory for the output Breps is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoJoinBreps(
  const ON_SimpleArray<const ON_Brep*>& input_breps,
  double tolerance,
  double angle_tolerance,
  ON_SimpleArray<ON_Brep*>& output_breps,
  ON_ClassArray<ON_SimpleArray<int>>& output_key
);

/*
Description:
  OBSOLETE - USE THE VERSION ABOVE
  Joins the Breps in the input array at any overlapping edges to form
  as few as possible resulting Breps.
  There may be more than one Brep in the result array
Parameters:
  input_breps  [in] Array of Breps to join
  output_breps [in] Empty array to return results of the join
  tolerance    [in] 3d distance tolerance for detecting overlapping edges
Returns:
  true if any edges were joined.
Remarks:
  The Breps sent in to join should be deleted after this call
  All of the geometry is copied and added to joined_breps in one form or another.
  ON_SimpleArray<ON_Brep*> joined_breps;
  RhinoJoinBreps(in_breps, joined_breps, tolerance);
  for (i=0; i<in_breps.Count(); i++)
    delete in_breps[i];
*/
RHINO_SDK_FUNCTION
bool RhinoJoinBreps(
  ON_SimpleArray<ON_Brep*>& input_breps,
  ON_SimpleArray<ON_Brep*>& output_breps,
  double tolerance
);

/*
Description:
  If any edges of B overlap edges of B1, merge a copy of B1 into B
  joining all edges that overlap within tolerance
Parameters:
  B         [in] Base Brep to which faces will be added.
  B1        [in] Brep to be added to B,
  tolerance [in] 3d distance tolerance for detecting overlapping edges
  bCompact  [in] if true, set Brep flags and tolerances, remove unused faces and edges
Returns:
  true if any edges were joined.
Remarks:
  if no edges overlap, B is unchanged.

  B1 is copied if it is merged with B, and B1 is always unchanged

  Use this to join a list of Breps in a series.

  given ON_SimpleArray<const ON_Brep*> BList, of manifold , non-solid Breps,

  ON_Brep* B = ON_Brep::New(BList[0]); //be sure to copy this one, not cast.
  bool joined_something = false;
  for (int i=1; i<BList.Count(); i++){
    bool joined_something_here = RhinoJoinBreps(*B, *BList[i], tolerance));
    if (joined_something_here){
      joined_something = true;
      if (B->IsSolid()) break;
    }
  }

  When joining multiple Breps in series, bCompact should be set to false.
  If bCompact is false, the caller should do something like this afterwards:

  B.Compact();
  B.SetEdgeTolerances(TRUE);
  B.SetVertexTolerances(TRUE);
*/
RHINO_SDK_FUNCTION
bool RhinoJoinBreps(
  ON_Brep& B,
  const ON_Brep& B1,
  double tolerance,
  bool bCompact = true
);

/*
Description:
  Separates, or splits, a disjoint Brep into separate Breps.
Parameters:
  in_brep     - [in] The disjoint Brep to separate.
  out_breps   - [out] If input is disjoint, the output pieces.
  out_keys    - [out] Optional array of int arrays,
                      where out_keys->At(i) contains all in_brep.m_face_user.j[1] that contribute to out_breps[i].
                      With this, the original input can be reconstructed and history can be recorded.
Returns:
  true if the input was separated into multiple pieces.
  false if error or if the input was not disjoint.
Remarks:
  Memory for the output Breps is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoSeparateBreps(
  const ON_Brep& in_brep,
  ON_SimpleArray<ON_Brep*>& out_breps,
  ON_ClassArray<ON_SimpleArray<int>>* out_keys
);

/*
Description:
  Combine vertices with same m_vertex_user.i, Combine edges with same m_edge_user.i that are
  within tolerance
Parameters:
  B         [in] Brep with edges and vertices to be combined.
  tol       [in] 3d distance tolerance for detecting overlapping edges
  bCompact  [in] if true, set Brep flags and tolerances, remove unused faces and edges
Returns:
  true if all indicated edges and vertices were combined.
  false if some pair could not be combined
Remarks:
  The vertex and edge user fields will be changed by this routine.
  If bCompact is false, B will have unused edges and vertices.
  This routine is intended for use in file translation applications for which the topology
  information is available but not easily accessible at a useful time.
*/
RHINO_SDK_FUNCTION
bool RhinoBrepCombineMarkedVerticesAndEdges(
  ON_Brep& B,
  double tol,
  bool bCompact = true
);

/*
Description:
  Join a bunch of ON_Curves into one or more ON_Curves
Parameters:
  input_curves [in] Array of pointers to ON_Curves to be joined
  output [out] Array of pointers to joined results
  join_tol [in] max distance between endpoints to be joined.
           If join_tol < ON_EPSILON, use CRhinoDoc::AbsoluteTolerance()
  bPreserveDir [in] if TRUE, don't reverse input curves to get them to join
  key     [out] if non-null, curves[i] is part of output[key[i]]
                WARNING - key[i] may be -1 for some i, in particular if curves[i] is
                extremely short, key[i] will be -1 and curves[i] will not contribute to the
                joined results.
Returns:
  @untitled table
  TRUE     Success
  False    Failure
Remarks:
  Join as many of the input curves as have matching endpoints. If the input
  curve is a NURBS curve or a line, the endpoints within the specified
  tolerance are trued up to meet exactly. All of the input curves are copied
  and the caller must free the results.  When curves are joined they are made
  into polycurves. Memory for the curves is allocated and becomes the
  responsibility of the caller.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoMergeCurves(
  ON_SimpleArray<const ON_Curve*>& input_curves,
  ON_SimpleArray<ON_Curve*>& output,
  double join_tol = 0.0,
  BOOL32 bPreserveDir = FALSE,
  ON_SimpleArray<int>* key = nullptr
);

/*
Description:
  Join a bunch of ON_Curves into one or more ON_Curves.
  This is simple version of RhinoMergeCurves that just uses ON_JoinCurve.
*/
RHINO_SDK_FUNCTION
int RhinoJoinCurves(
  ON_SimpleArray<const ON_Curve*>& in_curves,
  ON_SimpleArray<ON_Curve*>& out_curves,
  double join_tol,
  bool bPreserveDir,
  ON_SimpleArray<int>* key
);

/*
Description:
  Removes any nesting of polycurves.  If PC has just a single segment, the segment is returned.
  If, after nest removal, there are adjacent segments which are polylines, they are combined into
  a single polyline.
Parameters:
  PC     [in] Polycurve to clean up.
Returns:
  NULL if PC is unchanged
  Otherwise, a new curve that is not necessarily a polycurve.
Remarks:
  The new curve may have a different domain from PC.
  If the start and end segments of a closed input are polylines,
    the result may have a different seam location since the start and end segments will be combined.
  The caller is responsible for destroying the returned curve.
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoCleanUpPolyCurve(const ON_PolyCurve& PC);

/*
Description:
  If pCrv->IsClosed(), just return TRUE.  Otherwise, decide if
  pCrv can be closed as follows: Linear curves polylinear curves
  with 2 segments, NURBS with 3 or less control points cannot
  be made closed, Also, if tolerance > 0 and the gap between
  start and end is larger than tolerance, curve cannot be made
  closed.Adjust the curve's endpoint to match its start point.
Parameters:
  pCrv      [in] curve to adjust
  tolerance [in] if nonzero, and the gap is more than tolerance, curve cannot
                 be made closed.
Returns:
  @untitled table
  TRUE     Success
  False    Curve could not be made closed.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoMakeCurveClosed(
  ON_Curve* pCrv,
  double tolerance = 0.0
);

/*
Description:
  Make adjustments to the ends of one or both input curves
  so that they meet at a point
Parameters:
  pCrv0 [in] 1st curve to adjust
  end0  [in] end of the 1st curve to adjust 0: start, 1: end
  pCrv1 [in] 2nd curve to adjust
  end1  [in] end of the 2nd curve to adjust 0: start, 1: end
Returns:
  @untitled table
  TRUE     Success
  False    Failure
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoMakeCurveEndsMeet(
  ON_Curve* pCrv0,
  int end0,
  ON_Curve* pCrv1,
  int end1
);

/*
Description:
  Remove short segments from the input curve.
Parameters:
  curve - [in] curve to check
  tolerance - [in] if < 0 (default), then an appropriate tolerance
                   is automatically calculated.
                   if == 0, then ON_ZERO_TOLERANCE is used.
                   if > 0, then the input value is used.
Returns:
  @untitled table
  True    short segments were removed.
  False   no changes to curve.
See Also:
  ON_Curve::RemoveShortSegments
*/
RHINO_SDK_FUNCTION
bool RhinoRemoveShortSegments(
  ON_Curve* curve,
  double tolerance = ON_UNSET_VALUE
);

/*
Description:
  Creates planar Breps of 1 face from array of curves
Parameters:
  input_loops [in] loops to sort and use as boundaries
  breps [out] array of 1 face Breps
  notused - optional array to hold indices of the input curves that got used
Returns:
  @untitled table
  TRUE     Success
  FALSE    Failure
Remarks:
  calls RhinoMakePlanarFaces() to sort the input curves into faces
See Also:
  RhinoMakePlanarBrep()
*/
ON_DEPRECATED_MSG("use overload that takes a tolerance")
RHINO_SDK_FUNCTION
BOOL32 RhinoMakePlanarBreps(
  ON_SimpleArray<const ON_Curve*>& input_loops,
  ON_SimpleArray<ON_Brep*>& breps,
  ON_SimpleArray<int>* used = nullptr
);

RHINO_SDK_FUNCTION
BOOL32 RhinoMakePlanarBreps(
  ON_SimpleArray<const ON_Curve*>& input_loops,
  ON_SimpleArray<ON_Brep*>& breps,
  double tolerance,
  ON_SimpleArray<int>* used = nullptr
);

/*
Description:
  Returns V, clamped to lie between L & H, Make L <= V <= H
Parameters:
    V [in] value to be clamped
    L [in] low bound
    H [in] high bound
*/
#define RHINO_CLAMP(V,L,H) ( (V) < (L) ? (L) : ( (V) > (H) ? (H) : (V) ) )

#define RHINO_CLAMP_UI_SUBD_DISPLAY_DENSITY(D) (D < ON_SubDDisplayParameters::MinimumUserInterfaceDensity ? ON_SubDDisplayParameters::MinimumUserInterfaceDensity :  (D > ON_SubDDisplayParameters::MaximumUserInterfaceDensity ? ON_SubDDisplayParameters::MaximumUserInterfaceDensity : D))

/*
Description:
  Fit a plane through an array of points
Parameters:
  points [in] - The points through which to fit
  plane [out] - The resulting plane
  box [out]   - The 2d bounding box of the corners of the plane
Returns:
  @untitled table
  1      Results were inconclusive, as in collinear points, but some plane was found
  0      Success
 -1      Couldn't find a plane
Remarks:
  Finds a best-fit plane through the points
*/
RHINO_SDK_FUNCTION
int RhinoFitPlaneToPoints(
  const ON_SimpleArray<ON_3dPoint>& point_array,
  ON_Plane& plane,
  ON_BoundingBox& box
);

RHINO_SDK_FUNCTION
int RhinoFitPlaneToPoints(
  int point_count,
  const ON_3dPoint* points,
  ON_Plane& plane,
  ON_BoundingBox& box
);

/*
  Description:
    Fit a plane through an array of points
  Parameters:
    points [in] - The points through which to fit
    plane [out] - The resulting plane
    box [out]   - The 3d bounding box of the corners of the plane
  Returns:
    @untitled table
    1      Results were inconclusive, as in collinear points, but some plane was found
    0      Success
   -1      Couldn't find a plane
  Remarks:
    Finds a best-fit plane through the points
*/
RHINO_SDK_FUNCTION
bool RhinoFitLineToPoints(
  const ON_SimpleArray<ON_3dPoint>& point_array,
  ON_Line& line
);

RHINO_SDK_FUNCTION
bool RhinoFitLineToPoints(int point_count, const ON_3dPoint* points, ON_Line& line);

/*
Description:
  Fits a circle through an array of points.
Parameters:
  points [in] - The points through which to fit.
  circle [out] - The resulting circle.
Returns:
  true if successful, false otherwise.
Remarks:
  Finds a best-fit circle through the points.
*/
RHINO_SDK_FUNCTION
bool RhinoFitCircleToPoints(
  const ON_SimpleArray<ON_3dPoint>& point_array,
  ON_Circle& circle
);

RHINO_SDK_FUNCTION
bool RhinoFitCircleToPoints(
  int point_count,
  const ON_3dPoint* points,
  ON_Circle& circle
);

/*
Description:
  Fits a sphere through an array of points.
Parameters:
  points [in] - The points through which to fit.
  sphere [out] - The resulting sphere.
Returns:
  true if successful, false otherwise.
Remarks:
  Finds a best-fit sphere through the points.
*/
RHINO_SDK_FUNCTION
bool RhinoFitSphereToPoints(
  const ON_SimpleArray<ON_3dPoint>& point_array,
  ON_Sphere& sphere
);

RHINO_SDK_FUNCTION
bool RhinoFitSphereToPoints(
  int point_count,
  const ON_3dPoint* points,
  ON_Sphere& sphere
);

/*
Description:
  Fits an ellipse through an array of points.
Parameters:
  points [in] - The points through which to fit.
  ellipse [out] - The resulting ellipse.
Returns:
  true if successful, false otherwise.
Remarks:
  Finds a best-fit ellipse through the points.
*/
RHINO_SDK_FUNCTION
bool RhinoFitEllipseToPoints(
  const ON_SimpleArray<ON_3dPoint>& point_array, 
  ON_Ellipse& out_ellipse
);

RHINO_SDK_FUNCTION
bool RhinoFitEllipseToPoints(
  int point_count, 
  const ON_3dPoint* points, 
  ON_Ellipse& out_ellipse
);

/*
Description:
  Tests if the points in an array are coplanar
Parameters:
  points [in] - the array of points to test
  pPlane [in] - Optional pointer to return the plane if the points are coplanar
  tolerance [in] - Optional tolerance for distance from the plane
Returns:
  @untitled table
  TRUE     Points are Coplanar
  False    Points are not Coplanar
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoArePointsCoplanar(
  ON_SimpleArray<ON_3dPoint>& points,
  ON_3dPoint* pPoint = nullptr,
  ON_3dVector* pNormal = nullptr,
  double tolerance = ON_ZERO_TOLERANCE
);

RHINO_SDK_FUNCTION
BOOL32 RhinoArePointsCoplanar(
  ON_SimpleArray<ON_4dPoint>& points,
  ON_3dPoint* pPoint = nullptr,
  ON_3dVector* pNormal = nullptr,
  double tolerance = ON_ZERO_TOLERANCE
);

/*
Description:
  Return a pointer to the active cplane
Returns:
  pointer to cplane
  or NULL if there is none.
*/
RHINO_SDK_FUNCTION
const ON_Plane& RhinoActiveCPlane();

/*
Description:
  Project a NURBS curve to a plane
Parameters:
  curve [in] the curve to project.
        [out] the modified curve
Returns:
  @untitled table
  TRUE     Success
  False    Failure
Remarks:
  Curve is flattered in place
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoProjectToPlane(
  ON_NurbsCurve& curve,
  ON_Plane& plane
);

/*
Description:
  Project a curve to a plane.
Parameters:
  curve [in] - Curve to project.
  plane [in] - Projection plane.
  shortSegmentTolerance [in] - Short curve segment tolerance.
Returns:
  Pointer to the projected curve if successful, false otherwise.
  Memory for the output curve is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoProjectToPlane(
  const ON_Curve& curve,
  const ON_Plane& plane,
  double shortSegmentTolerance
);

/*
Description:
  Project an ON_BoundingBox to a plane
Parameters:
  box [in] the box to project.
      [out] the modified box
Returns:
  @untitled table
  TRUE     Success
  False    Failure
Remarks:
  Bounding box is flattered in place
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoProjectToPlane(
  ON_BoundingBox& curve,
  ON_Plane& plane
);

/*
Description:
  Find a line collinear with line_in that goes completely through the box
Parameters:
  line_in [in] - The original line ( not modified)
  bbox [in] - The box to extend through
  line_out [out] - The extended line
Returns:
  @untitled table
  TRUE     Success
  False    Failure
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoExtendLineThroughBox(
  const ON_Line& line_in,
  const ON_BoundingBox& bbox,
  ON_Line& line_out
);

/*
Description:
  Converts a string to a name string valid in AutoCAD
Parameters:
  [in/out] name - a string to convert / the converted string
  [in] max_len - maximum length of output name
  [in] bAllowSpaces - optional to allow ' ' in output
Remarks:
  Replaces characters not valid in AutoCAD strings with '_'
  and truncates to max_length (default == 31 characters)
*/
RHINO_SDK_FUNCTION
void RhinoAcadNameString(
  char* name,
  int max_length = 31,
  bool bAllowSpace = false
);

/*
Description:
  Find the extreme geometric end points of a curve that may be
  doubled back on itself as when a circle is collapsed edge-wise
  Such a curve may have endpoints on the interior of the geometric line
Parameters:
  [in] curve - the curve to examine
  [out] line - the geometric line representing the curve
  [in] - optional tolerance for testing if the curve is a line
Returns:
  @untitled table
  TRUE     Success
  FALSE    Failure
Remarks -
  Does not check if the curve is linear or not - just returns
  the max distance between kinks
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoGetLineExtremes(
  const ON_Curve& curve,
  ON_Line& line
);

/*
Description:
  find points at which to cut a pair of curves so that a fillet of radius r fits.
Parameters:
  c0,c1 - [in] curves
  radius - [in] fillet radius
  t0_base -[in] parameter value for base point on curve 0
  t1_base -[in] parameter value for base point on curve 1
  t0 - [out] parameter value of fillet point on curve 0
  t1 - [out] parameter value of fillet point on curve 1
  FilletPlane -[out] The fillet is contained in this plane
                      with center at the plane origin.
Returns:
  true - solution found
  false - no solution found
Remarks:
  A fillet point is a pair of curve parameters (t0,t1) such that there is a
  circle of radius r tangent to curve c0 at t0 and tangent to curve c1 at t1.
  Of all possible fillet points this function returns the one which is the closest
  to the base point t0_base, t1_base.  Distance from the base point is
  measured by  the sum of arc lengths along the two curves.
*/
RHINO_SDK_FUNCTION
BOOL32 RhinoGetFilletPoints(
  const ON_Curve& c0,
  const ON_Curve& c1,
  double radius,
  double t0_base,
  double  t1_base,
  double& t0,
  double& t1,
  ON_Plane& FilletPlane
);

/*
Description:
  Input class for MakeRhinoContours functions below.
*/
class RHINO_SDK_CLASS CRhinoContourInput
{
public:
  CRhinoContourInput();
  // input geometry
  ON_SimpleArray<const ON_Geometry*> m_geom;
  // input Attributes
  ON_SimpleArray<const ON_3dmObjectAttributes*> m_attribs;
  //base point for vector that is normal to contour planes and used as contour limit if m_limit_range is true
  ON_3dPoint m_basept;
  //end point for vector that is normal to contour planes and used as contour limit if m_limit_range is true
  ON_3dPoint m_endpt;
  //distance between contours
  double m_interval;
  //true if contours are only to be made between m_basept and m_endpt
  BOOL32 m_limit_range;
  //This parameter is now used to assign attributes (not just layer - treat like Assign Attributes: ByLayer/ByInput)
  int m_AssignLayerBy; //0 = CurrentLayer, 1 = InputObject
  int m_JoinCurves; //0 = NoJoin, 1 = ByPolySurface, 2 = ByContourPlane
  //To add output directly to the document set m_bAddOutputToDocument to true.
  //If m_bAddOutputToDocument is true, group output by cut plane by setting
  //m_bGroupByContourPlane to true.
  bool m_bAddOutputToDocument; //default is false
  bool m_bGroupByContourPlane; //default is false
  bool m_bShowCount; //default is false
  int m_CountIncrement; //default is 1
};

/*
Description:
  Create contour curves for Breps and contour polylines for meshes that are
  input.m_interval distance apart and fill the output arrays with the results.
  If m_bAddOutputToDocument==true the output will be added to the current
  Rhino document instead of populating the output arrays.
Parameters:
  input - [in]
  output_PlineArray - [out] //output from mesh and point cloud intersection
  output_CurveArray - [out] //output from surface and polysurface intersection
  output_PointArray = [out] //output from curve intersection
Returns:
  true or false depending on success or failure
Remarks:
  It is the calling functions responsibility to clean up the memory used by these arrays.
*/
RHINO_SDK_FUNCTION
bool MakeRhinoContours(
#if defined (ON_RUNTIME_APPLE)
  CRhinoDoc& doc,
#endif
  const CRhinoContourInput& input,
  ON_SimpleArray<ON_Polyline*>& output_PlineArray,
  ON_SimpleArray<ON_Curve*>& output_CurveArray,
  ON_SimpleArray<ON_3dPoint>& output_PointArray
);

/*
Description:
  Create contour curves for Breps and contour polylines for meshes that are
  input.m_interval distance apart and fill the output arrays with the results.
  If m_bAddOutputToDocument==true, then doc must NOT be NULL
  If m_bAddOutputToDocument==false, doc can be NULL and is ignored
Parameters:
  input - [in]
  output_PlineArray - [out] //output from mesh and point cloud intersection
  output_CurveArray - [out] //output from surface and polysurface intersection
  output_PointArray = [out] //output from curve intersection
Returns:
  true or false depending on success or failure
Remarks:
  It is the calling functions responsibility to clean up the memory used by these arrays.
*/
RHINO_SDK_FUNCTION
bool MakeRhinoContours(
  const CRhinoContourInput& input,
  ON_SimpleArray<ON_Polyline*>& output_PlineArray,
  ON_SimpleArray<ON_Curve*>& output_CurveArray,
  ON_SimpleArray<ON_3dPoint>& output_PointArray,
  double tolerance,
  CRhinoDoc* doc
);


/*
Description:
  Create contour curves for Breps and contour polylines for meshes that are
  input.m_interval distance apart and fill the output arrays with the results.
  If m_bAddOutputToDocument==true, then doc must NOT be NULL
  If m_bAddOutputToDocument==false, doc can be NULL and is ignored
Parameters:
  input - [in]
  output_PlineArray = [out] //output from mesh and point cloud intersection
  output_CurveArray = [out] //output from surface and polysurface intersection
  output_PointArray = [out] //output from curve intersection
  tolerance = tolerance
  output = false outputs curves only, true outputs hatches if set
  layer_index = input document
  doc = input document
Returns:
  true or false depending on success or failure
Remarks:
  It is the calling functions responsibility to clean up the memory used by these arrays.
*/
RHINO_SDK_FUNCTION
bool RhinoMakeContours(
  const CRhinoContourInput& input,
  ON_SimpleArray<ON_Polyline*>& output_PlineArray,
  ON_SimpleArray<ON_Curve*>& output_CurveArray,
  ON_SimpleArray<ON_3dPoint>& output_PointArray,
  double tolerance,
  bool output,
  int layer_index,
  CRhinoDoc* doc
);
/*
Description:
  Interpolates a sequence of points.  Used by InterpCurve Command
Parameters:
  degree - [in] The degree of the curve >=1. Note: Even degree > 3 periodic interpolation
                results in a non-periodic closed curve.
  Pt -[in]			Array of points to interpolate.  For periodic curves if the final point is
                a duplicate of the initial point it is  ignored.
                Pt.Count()>=2 and
  start_tan  - [in]		If not NULL this is the unit tangent at the start of the curve.
                      For periodic style curves start_tan must be NULL.
  end_tan - [in]	If not NULL this is unit tangent at the end of the curve.
                      For periodic style curves end_tan must be NULL.
  knot_style-[in]		Knot-style to use  and specifies if the curve should be periodic.

                    @table
                    value		meaning
                    0				uniform knots. Parameter spacing between consecutive knots is 1.0
                    1				chord length spacing,  requires degree=3 with CV1 and CVn1 specified.
                    2				sqrt(chord length),  requires degree=3 with CV1 and CVn1 specified.
                    3				periodic with uniform spacing
                    4				periodic with chord length spacing.
                    5				periodic with sqrt(chord length) spacing.
  nurbs_curve-[in]  If not NULL, this curve will be used and returned.
Returns:
  Pointer to interpolating curve.  Returns NULL if failure.
Details:
  This routine works best when degree=3.
  This is used by the Rhino InterpCrv command.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoInterpCurve(
  int degree,
  const ON_SimpleArray<ON_3dPoint>& Pt,
  const ON_3dVector* start_tan,
  const ON_3dVector* end_tan,
  int knot_style,
  ON_NurbsCurve* nc = nullptr
);

ON_DEPRECATED_MSG("use RhinoInterpCurve")
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoNurbsCurveInterpolant(
  int degree,
  const ON_SimpleArray<ON_3dPoint>& Pt,
  ON_3dPoint* CV1,
  ON_3dPoint* CVn1,
  int knot_style = 0,
  ON_NurbsCurve* nurbs_curve = nullptr
);

/*
Description:
  returns a curve that interpolates points on a surface.  The interpolant lies on the surface.
Parameters:
 srf	-[in]   Surface containing the points.
 PtIn -[in]		Array of at least 2 parameter points on the surface
 is_closed -[in] 0 - not closed. The interpolating curve is not closed
                 1 - closed.    The interpolating curve is closed.
                                The last point and first point of
                                PtIn should generally not be equal.
 tol	-[in]				tolerance used for the fit of the pushup curve.  Generally, the resulting interpolating
                  curve will be within tol of the surface.
 ClosedSrfHandling - [in]
                  0 -   All points in PtIn must be in the rectangular domain of srf.
                            If srf is closed in some direction then this routine will
                            interpret each point and place it at an appropriate location in the
                            the covering space.  This is the simplest option and should give
                            good results.
                  1 -   Use this option for more control of handling curves going across seams.
                        If the surface is closed in some direction then the points in PtIn
                            are taken as points in the covering space.
                            Example: Suppose srf.IsClosed(0)=TRUE and srf.IsClosed(1)=FALSE
                            and srf.Domain(0)=srf.Domain(1)=ON_Interval(0,1) then if
                            ClosedSrfHandling = 1  a point (u,v) in PtIn can have any
                            value for the u coordinate, but must have 0<=v<=1.  In particular
                            if PtIn = { (0.0,0.5), (2.0,0.5) } then the interpolating curve will
                            wrap around the surface 2 times in the closed direction before ending
                            at start of the curve.  If is_closed=1 the last point should equal
                            the first point plus an integer multiple of the period on a closed
                            direction.
Returns:
  An interpolating curve lying on the surface.
Details:
  This code doesn't make periodic curves, but will make closed curves.
*/

RHINO_SDK_FUNCTION
ON_Curve* RhinoInterpolatePointsOnSurface(
  const ON_Surface& srf,
  const ON_SimpleArray<ON_2dPoint> PtIn,
  int is_closed,
  double tol,
  int ClosedSrfHandling
);

/*
Description:
  Constructs a geodesic between 2 points, used by ShortPath command in Rhino
Parameters:
  Srf   - [in] Surface
  start - [in] endpoints of curve in parameter space.  Points must be distinct
  end   - [in]   in the domain of Srf
  tol   - [in] tolerance used in fitting discrete solution
Return:
  Returns a geodesic curve on the surface
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoShortPath(
  const ON_Surface& Srf,
  ON_2dPoint start,
  ON_2dPoint end,
  double tol
);

/*
Description:
  Tests if two curves are generally in the same direction,
  or if they would be more in the same direction if they
  were flipped.
Parameters:
  c0 - [in] the first curve to test.
  c1 - [in] the second curve to test.
Returns:
  true - generally in the same direction
  false - would be more in the same direction if they were flipped
*/
RHINO_SDK_FUNCTION
bool RhinoDoCurveDirectionsMatch(
  const ON_Curve* c0,
  const ON_Curve* c1
);

/*
Description:
  Creates a NURBS surface from a 2D grid of points
Parameters:
  point_count   - [in] The number of points in the U and V directions.
  degree        - [in] The surface degree in the U and V directions.
  is_closed     - [in] Whether or not the surface is closed in the U and V directions.
  point_array   - [in] The grid (array) of points.
  nurbs_surface - [in] If not NULL, this surface will be used and returned.
Returns:
  Pointer to a NURBS surface curve.  Returns NULL if failure. If
  nurbs_surface == NULL, the default, the user must delete the curve when
  done with it.
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoSrfPtGrid(
  int point_count[2],
  int degree[2],
  BOOL32 is_closed[2],
  const ON_3dPointArray& point_array,
  ON_NurbsSurface* nurbs_surface = nullptr
);

/*
Description:
  Creates a NURBS surface from a 2D grid of control points
Parameters:
  point_count   - [in] The number of points in the U and V directions.
  degree        - [in] The surface degree in the U and V directions.
  point_array   - [in] The grid (array) of control points.
  nurbs_surface - [in] If not NULL, this surface will be used and returned.
Returns:
  Pointer to a NURBS surface curve.  Returns NULL if failure. If
  nurbs_surface == NULL, the default, the user must delete the curve when
  done with it.
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoSrfControlPtGrid(
  int point_count[2],
  int degree[2],
  const ON_3dPointArray& point_array,
  ON_NurbsSurface* nurbs_surface = nullptr
);

class RHINO_SDK_CLASS CRhinoExtend
{
public:
  enum Type
  {
    Line,
    Arc,
    Smooth,
    Natural // Line for linear curves, Arc for arcs, and Smooth for others
  };
};

/*
Description:
  Extend a curve by a line, arc or smooth extension for a specified distance
Parameters:
  crv - [in/out] curve to extend.  Note either *crv can change or the
                 pointer crv can be changed by the routine.
  type - [in]		type of extension, line, arc or smooth
  side - [in]   end to extend. 0- beginning, 1, end, 2 - both
  length - [in] length of extension ( >= 0)
Returns:
  true if desired extension achieved.
Details:
  The parametric curve is extended, i.e. if c' is the extension of c then
      c(t) = c'(t)   for any t in the domain of c.
  The extension is generally C1-continuous at the point of the extension,
  and may be smoother depending on the type of extension.

  When type==RhinoExtend::Smooth the parametric curve  is extended by the analytic
  extension.  The curve *crv is modified, in particular the type is unchanged.
  If the curve is	a NURBS curve, or a polycurve with a NURBS curve at the end to be
  extended, then the extension length is only approximate.  Also, refer to the comments
  in TL_ExtendNurb for warnings about smooth extensions of NURBS curves.

  When type==RhinoExtend::Line or RhinoExtend::Arc the result is an ON_PolyCurve with
  the extensions being the first and/or last segments.
  The user must delete the returned polycurve.

  When type=RhinoExtend::Arc the result is G2-continuous, but generally not C2-continuous.

  Even if the function returns false the curve may be extended as far as possible, e.g.,
  an arc may be extended to 360 degrees but not more.

  Returns false on closed curves.
*/
RHINO_SDK_FUNCTION
bool RhinoExtendCurve(
  ON_Curve*& crv,
  CRhinoExtend::Type type,
  int side,
  double length
);

/*
Description:
  Extend a curve on a face
Parameters:
  face - [in] a Brep face to extend the curve on
  crv  - [in/out] pointer to the  input curve can be either a 2d parameter space curve or a 3d-curve.
                  At output this is a pointer to the output curve, which contains the input curve as a sub-curve.
  side - [in] end to extend. 0- beginning, 1, end, 2 - both
Returns:
  true the function succeeded
  false the function failed the crv is left unchanged
Details:
  If crv is a 2d parameter space curve and either end is in the active
  region of the face the curve is extended at this end,in a C^1 fashion.
  the extension may not end at the boundary of the face. The resulting curve is a ON_PolyCurve
  with the original curve as one of the polycurve segments.

  If crv is a 3d model space curve and either end projects, via the closest
  point projection, to the interior of the active  region of the face then
  the curve is extended at this end, in a C^1 fashion.
*/
RHINO_SDK_FUNCTION
bool RhinoExtendCrvOnSrf(
  const ON_BrepFace& face,
  ON_Curve*& crv,
  int side = 2
);

/*
Description:
  Extend a curve by a line, arc or smooth extension until it intersects a collection of objects
Parameters:
  crv -[in/out] curve to extend.  Note either *crv can change or the
                pointer crv can be changed by the routine.
  type -[in]		type of extension, line, arc or smooth
  side -[in]    end to extend. 0- beginning, 1, end, 2 - both
  geom -[in]    collection of objects.  Allowable object types are ON_Curve, ON_Surface, ON_Brep,
Return:
  Returns true if the extension hit an object.
*/
RHINO_SDK_FUNCTION
bool RhinoExtendCurve(
  ON_Curve*& crv,
  CRhinoExtend::Type type,
  int side,
  const ON_SimpleArray<const ON_Geometry*>& geom
);

/*
Description:
  Extend a curve by a line, arc or smooth extension.
  The extension endpoint is specified.
Parameters:
  crv -[in/out] curve to extend.  Note either *crv can change or the
                pointer crv can be changed by the routine.
  type -[in]		type of extension, line, arc or smooth
  side -[in]    end to extend. 0- beginning, 1- end
  end  -[in]    end point of extension
Return:
  Returns true if the extension hit an object.
*/
RHINO_SDK_FUNCTION
bool RhinoExtendCurve(
  ON_Curve*& crv,
  CRhinoExtend::Type type,
  int side,
  ON_3dPoint end
);

/*
Description
  Simplify curve replaces the curve with a geometrically equivalent curve.
  The result is a simple curve or a ON_PolyCurve of at least two simple curve segments.
    1. Simple curves are either  ON_LineCurve, ON_PolylineCurve ON_ArcCurve, or ON_NurbsCurve.
    2. Simple NURBS curves  do not have fully multiple interior knots.
    3. Simple rational NURBS curves do not have constant weights.
    4. A curve for which IsLinear() is true is simplified to an ON_Line.
    4. A curve for which IsArc() is true and IsLinear() is false  is simplified to an  ON_Arc.
    5. Adjacent Collinear or Co-circular segments are combined.
    6. Segments that meet with G1-continuity have there ends tuned up so that they meet
       with G1-continuity to within machine precision
    *NOTE* Unlike V3, V4 RhinoSimplifyCurve can  return a simple curve,
           instead of a one-segment polycurve.
Parameters:
  crv  [in/out]  - input curve can be of any ON_Curve type.  output curve
                    is an ON_PolyCurve.
  flags -   [in] - flags specifying which function to use to simplify curve.
  dist_tol  [in] -
  angle_tol [in] -
Details:
  The default of flags=0 implies all of the simplification functions will be used.
    DONT_SPLIT_FMK				Don't split NurbsCurves at fully multiple knots
    DONT_REBUILD_LINES	 	Don't replace segments with IsLinear()==true with ON_LineCurves
    DONT_REBUILD_ARCS		 	Don't replace segments with IsArc()==true with ON_Arc
    DONT_REBUILD_RATIONALS	Don't replace rational NURBS curves with constant denominator
                            with an equivalent non-rational ON_NurbsCurve
    DONT_ADJUST_G1				Don't adjust Curves at G1-joins.
    DONT_MERGE            Don't merge adjacent co-linear lines or co-circular arc
                          or combine consecutive line segments into a polyline

  The distance_tolerance is used in the following stages
    Detecting lines and arcs

  Both distance_tolerance and angle_tolerance are used in the following stages
    Detecting G1 continuity
    Detecting co-linearity of adjacent line segments and co-circularity of adjacent arcs

  If in doubt about what to use for the tolerance settings use
    dist_tol  = CRhinoDoc.AbsoluteTolerance(), and
    angle_tol = CRhinoDoc.AngleToleranceRadians()

Returns:
  true if the curve is modified
*/
RHINO_SDK_FUNCTION
bool RhinoSimplifyCurve(
  ON_Curve*& crv,
  int flags,
  double dist_tol,
  double angle_tol
);

RHINO_SDK_FUNCTION
bool RhinoSimplifyCurve(
  ON_Curve*& crv,
  double dist_tol,
  double angle_tol
);

/*
Description:
  Get sub-curves from a curve.
Parameters:
  curve - [in]
  bSkipSingleSegmentCurves - [in]
    This parameter controls what happens with curve does
    not "explode" into two or more segments.
    true = append a duplicate to the curve_segments[] array.
    false = do nothing.
  curve_segments - [out]
    segments of curve are appended to this array.
Returns:
  true:
    The input curve made sense and the function worked
    as expected.
  false:
    Some portion of the input curve was invalid and
    the function did the best it could.
Comment:
  This function is here because at least for the time being,
   it matches the way Explode makes sub-curve segments.
*/
RHINO_SDK_FUNCTION
bool RhinoGetSubCurves(
  const ON_Curve* pCurve,
  const ON_Interval* pDomain,
  bool bSkipSingleSegments,
  ON_SimpleArray<ON_Curve*>& curve_segments
);

/*
Description:
  Same as RhinoSimplifyCurve, but simplifies only the last two segments at "side" end.
Parameters:
  side -   [in] - specifies end to simplify. 0=start, 1=end
See Also
  RhinoSimplifyCurve
*/
RHINO_SDK_FUNCTION
bool RhinoSimplifyCurveEnd(
  ON_Curve*& pC,
  int side,
  int flags,
  double dist_tol,
  double angle_tol
);

/*
Description
  Enumeration describing option to RhinoSimplifyCurve(...)
See Also
  RhinoSimplifyCurve
*/
class RHINO_SDK_CLASS RhSimplifyCurveFlags {
public:
  enum Flags {
    DONT_SPLIT_FMK = (1 << 0),
    DONT_REBUILD_LINES = (1 << 1),
    DONT_REBUILD_ARCS = (1 << 2),
    DONT_REBUILD_RATIONALS = (1 << 3),
    DONT_ADJUST_G1 = (1 << 4),
    DONT_MERGE = (1 << 5)
  };
};

/*
Description:
  Extend an untrimmed surface along one edge.
Parameters:
  srf   - [in/out]   surface to modify/ or replace.  The surface must be on the heap since
                     it may be deleted and a new object of the appropriate type is returned.
  edge   -[in] edge to extend must be N_iso, E_iso, S_iso, or W_iso
  length -[in]>0
  Smooth -[in] true for smooth (C-infinity) extension
               false  for a C1- ruled extension
Returns:
  true if extension is successfully computed
  false no changes
*/
RHINO_SDK_FUNCTION
bool RhinoExtendSurface(
  ON_Surface*& srf,
  ON_Surface::ISO edge_index,
  double ExtensionLength,
  bool SmoothToggle
);

/*
  Description
     Get objects  tight axis aligned bounding box or the
     union of the input box with the object's bounding box.
     The alignment of the box may be specified by a parameter.
  Parameters
    bbox - [in/out]		3d axis aligned bounding box
    bGrowBox -[in]	(default=false)
           If true, then the union of the input bbox and the
                    object's bounding box is returned.
           If false, the object's bounding box is returned.
    onb -[in]				If null the box is computed in world coordinates.
                    If not null must *onb must be a valid ON_Plane.
                    onb's origin, x-axis, y-axis and z-axis specify
                    an oriented orthonormal frame.
    MakeMeshes-[in] Make render meshes for all meshless Breps
                    in a cancelable thread
  Returns
    true if success
  Details
    If onb points to a valid ON_Plane the geometry is bounded by  the
    plane  aligned box  defined by the point set:
      onb->Origin() + x*onb->Xaxis() + y*onb->YAxis() + z*onb->zaxis
    for all (x,y,z) in bbox.
    If MakeMeshes is false the Brep surface wireframe is used for
    Breps without meshes
*/
RHINO_SDK_FUNCTION
bool RhinoGetTightBoundingBox(
  const ON_SimpleArray<const class CRhinoObject*>& geom,
  ON_BoundingBox& box,
  bool Grow = false,
  ON_Plane* onb = nullptr,
  bool MakeMeshes = true
);

/*
Description:
  Get intervals where the isocurve exists on an ON_BrepFace
  (trimmed surface)
Parameters:
  face - [in] (trim curves will be promoted to TL_NurbsCurves)
  iso_dir - [in]
      0 intervals are "u" intervals, c = constant "v" value
      1 intervals are "v" intervals, c = constant "u" value
  iso_constant - [in]
    If iso_dir is 0, then iso_constant is the constant "v" parameter.
    If iso_dir is 1, then iso_constant is the constant "u" parameter.
  intervals - [out] intervals are append to this list
     The isocurve(s) exist on the domain of the intervals.
Returns:
  number of intervals appended to the intervals[] array.
Remarks:
  If iso_dir = 0, the parameter space iso interval connects the
  2d points

    (intervals[i][0],iso_constant) and (intervals[i][1],iso_constant).

  If iso_dir = 0, the parameter space iso interval connects the
  2d points

    (iso_constant,intervals[i][0]) and (iso_constant,intervals[i][1]).
See Also:
  RhinoGetIsoCurves
*/
RHINO_SDK_FUNCTION
int RhinoGetBrepFaceIsoIntervals(
  const ON_BrepFace& face,
  int iso_dir,
  double iso_constant,
  ON_SimpleArray<ON_Interval>& intervals
);

/*
Description:
  Get isocurves on an ON_BrepFace (trimmed surface)
Parameters:
  face - [in] Brep face
  iso_dir - [in]
    0: intervals are "u" intervals at constant "v" values
    1: intervals are "v" intervals at constant "u" values
  iso_constanct - [in]
    If iso_dir is 0, then iso_constant is a constant "v" parameter.
    If iso_dir is 1, then iso_constant is a constant "u" parameter.
  iso_curves - [out] 3d isocurves are appended to this list
Returns:
  number of curves appended to the iso_curves[] array.
See Also:
  RhinoGetIsoIntervals
*/
RHINO_SDK_FUNCTION
int RhinoGetBrepFaceIsoCurves(
  const ON_BrepFace& face,
  int iso_dir,
  double iso_constant,
  ON_SimpleArray<ON_Curve*>& iso_curves
);

/*
Description:
  Finds a point inside of a solid Brep.
Parameters:
  brep      - [in] The Brep to search.
  tolerance - [in] Used for intersecting rays and is not necessarily related to the distance from the Brep to the found point.
                   When in doubt, use the document's model absolute tolerance.
  point    - [out] A point inside of the solid Brep.
Returns:
  Returns false if the input is not solid and manifold, if the Brep's bounding box is less than 2.0 * tolerance wide,
  or if no point could be found due to ray shooting or other errors.
See Also:
  RhinoIsPointInBrep
*/
RHINO_SDK_FUNCTION
bool RhinoGetPointInSolidBrep(
  const ON_Brep& brep,
  double tolerance,
  ON_3dPoint& point
);

/*
Description:
  Used by RhinoBrepRepair (see below)
*/
RHINO_SDK_FUNCTION
bool RhinoRepairCurve(
  ON_Curve* pCurve,
  double repair_tolerance,
  int dim
);

/*
Description:
  Used by RhinoBrepRepair (see below)
*/
RHINO_SDK_FUNCTION
bool RhinoRepairSurface(
  ON_Surface* pSurface,
  double repair_tolerance
);

/*
Description:
  Fills in missing or fixes incorrect component information from a Brep object.
  Useful when reading Brep information from other file formats
  that do not provide as complete of a Brep definition
  as required by Rhino.
Parameters:
  pBrep - [in] The Brep to repair
  repair_tolerance - [in] The repair tolerance. When in doubt,
                          use the document's model absolute tolerance.
Returns:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoRepairBrep(
  ON_Brep* pBrep,
  double repair_tolerance
);

/*
Description:
Fills in missing or fixes incorrect component information from a Brep object.
Useful when reading Brep information from other file formats
that do not provide as complete of a Brep definition
as required by Rhino.
Parameters:
pBrep - [in] The Brep to repair
repair_tolerance - [in] The repair tolerance. When in doubt,
use the document's model absolute tolerance.
bSpliKinkyEdges - [in] Split up kinky edges if true.  Use true
if in doubt.  Some IGES files have edges that are polylines. reading those files
in takes a long time if the edges are split up.  If the only purpose is
visualization, there is no need to splt the edges.
Returns:
True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoRepairBrep(
  ON_Brep* pBrep,
  double repair_tolerance,
  bool bSpliKinkyEdges);


/*
Description:
  If a surface is very nearly G1-continuous, move control points
  to make it exactly G1.
Parameters:
  srf - [in] surface to be made G1
  tol - [in] tolerance.  Control points will be moved if they do not
             need to be moved any more than tol.  If tol <= 0,
             the Rhino document tolerance will be used.
Returns:
  true if srf was changed
See Also:
  RhinoStraightenBrep
*/
RHINO_SDK_FUNCTION
bool RhinoMakeG1Surface(
  ON_Surface& srf,
  double tol = 0.0
);

/*
Description:
  If any surface in a Brep is very nearly G1-continuous, move control points
  to make it exactly G1.
Parameters:
  B - [in] Brep to straighten.
  tol - [in] tolerance.  Control points will be moved if they do not
             need to be moved any more than tol.  If tol <= 0,
             the Rhino document tolerance will be used.
Returns:
  number of surfaces that were changed
See Also:
  RhinoMakeG1Surface
*/
RHINO_SDK_FUNCTION
int RhinoStraightenBrep(
  ON_Brep& B,
  double tol = 0.0
);

/*
Description:
  Determine the relationship between the regions
  bounded by two coplanar simple closed curves.
Parameters:
  closed_curveA - [in] simple closed curve
  closed_curveB - [in] simple closed curve
  plane - [in] plane containing the closed curves
  tolerance - [in] intersection tolerance (> 0.0)
Returns:
 -1: invalid input
  0:  regions bounded by the curves are disjoint
  1:  curves intersect
  2:  region bounded by curveA is inside of curveB
  3:  region bounded by curveB is inside of curveA
See Also:
  RhinoPlanarCurveCollisionTest
  RhinoPointInPlanarClosedCurve
*/
RHINO_SDK_FUNCTION
int RhinoPlanarClosedCurveContainmentTest(
  const ON_Curve& closed_curveA,
  const ON_Curve& closed_curveB,
  ON_Plane plane,
  double tolerance
);

/*
Description:
  Determine if two coplanar curves intersect.
Parameters:
  curveA - [in]
  curveB - [in]
  plane - [in] plane containing the curves
  tolerance - [in] intersection tolerance (> 0.0)
Returns:
  false:  curves do not intersect or input is not valid
  true:   curves intersect
See Also:
  RhinoPlanarClosedCurveContainmentTest
  RhinoPointInPlanarClosedCurve
*/
RHINO_SDK_FUNCTION
bool RhinoPlanarCurveCollisionTest(
  const ON_Curve& curveA,
  const ON_Curve& curveB,
  ON_Plane plane,
  double tolerance
);

/*
Description:
  Determine if a point is inside of a closed curve, on
  a closed curve, or outside of a closed curve.
Parameters:
  point - [in]
  closed_curve - [in] simple closed curve
  plane - [in] plane containing the closed curve and point
  tolerance - [in] intersection tolerance (> 0)
Returns:
 -1:  invalid input
  0:  point is outside of the closed curve
  1:  point is inside of the closed curve
  2:  point is on the closed curve
See Also:
  RhinoPlanarClosedCurveContainmentTest
  RhinoPlanarCurveCollisionTest
*/
RHINO_SDK_FUNCTION
int RhinoPointInPlanarClosedCurve(
  ON_3dPoint point,
  const ON_Curve& closed_curve,
  ON_Plane plane,
  double tolerance
);

ON_DEPRECATED_MSG("use overload that accepts tolerance parameter")
RHINO_SDK_FUNCTION
int RhinoPointInPlanarClosedCurve(
  ON_3dPoint point,
  const ON_Curve& closed_curve,
  ON_Plane plane
);

/*
Description:
  Join naked edge pairs within the same Brep that
  overlap within tolerance.
Parameters:
  B - [in] Brep
  tolerance - [in] overlap tolerance for joining edges
              Uses document tolerance if <= 0.0
Returns:
  number of joins made.
See Also:
*/
RHINO_SDK_FUNCTION
int RhinoJoinBrepNakedEdges(
  ON_Brep& brep,
  double tolerance = 0.0
);

/*
Description:
  Joins two naked edges, or edges that are coincident or close together.
Parameters:
  brep        - [in] The Brep to modify.
  edge_index0 - [in] The first edge index on the first Brep.
  edge_index1 - [in] The second edge index on the second Brep.
  tolerance   - [in] The join tolerance.
  bCompact    - [in] If you are joining more than one edge pair and want the edge indices of
                     subsequent pairs to remain valid, set bCompact = false. But then call
                     ON_Brep::Compact() on the final result when done.
Returns:
  true if successful, false on failure.
*/
RHINO_SDK_FUNCTION
bool RhinoJoinEdges(
  ON_Brep& brep,
  int edge_index0,
  int edge_index1,
  double tolerance,
  bool bCompact
);

/*
Description:
  Joins two naked edges, or edges that are coincident or close together, from two Breps.
Parameters:
  brep0       - [in] The first Brep.
  edge_index0 - [in] The edge index on the first Brep.
  brep1       - [in] The second Brep.
  edge_index1 - [in] The edge index on the second Brep.
  tolerance   - [in] The join tolerance.
Returns:
  The resulting Brep if successful, null on failure.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoJoinEdges(
  const ON_Brep& brep0, int edge_index0,
  const ON_Brep& brep1, int edge_index1,
  double tolerance
);

/*
Description:
  Unjoins, or separates, edges within a Brep.
  Note, seams in closed surfaces will not separate.
Parameters:
  InBrep   - [in] The input Brep
  eids     - [in] An array of one or more Brep edge indices.
  OutBreps - [out] The results of the unjoin operation.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoUnjoinEdges(
  const ON_Brep& InBrep,
  const ON_SimpleArray<int>& eids,
  ON_SimpleArray<ON_Brep*>& OutBreps
);

/*
Description:
  Low level Brep joining utility designed for use in file reading.
  Use this when joining a list of Breps for which it is know which
  pairs are supposed to be joined.
  Let Ei and Ej be naked edges of Breps[i] and Breps[j], i < j.
  This routine will attempt to join Ei and Ej if they have an overlap region
  and j is on the list JoinData[i].

  Once this routine has been made more efficient, it will replace TL_JoinBreps() when
  the JoinData information is known
Parameters:
  Breps - [in] List of Breps to be joined.
  JoinData - [in] Attempt to join edges from Breps[i] with edges of Breps[JoinData[i][j]]
  tolerance - [in] overlap tolerance for joining edges
              Uses document tolerance if <= 0.0
Returns:
  Joined Brep, or 0 if no edges are joined
Process:
  Append copies of all Breps to JoinedBrep.
  Attempt to join any appropriate naked edges.
  Return result.
Note:
  The returned Brep may have several connected components.
  Caller must separate the components.
  Returns 0 if no edges are joined.
See Also:
  Rhino3JoinBreps, RhinoJoinBrepNakedEdges(B, tol)
*/
RHINO_SDK_FUNCTION
ON_Brep* Rhino3JoinBreps(
  const ON_SimpleArray<ON_Brep*>& Breps,
  ON_ClassArray< ON_SimpleArray<int> >& JoinData,
  double tolerance = 0.0
);

/*
Description:
  Compares the geometry of two objects, similarly to the SelDup command.
Parameters:
  a - [in] The first object to compare
  b - [in] The second object to compare
Returns:
  true:  The two objects are geometrically equivalent
  false:  The two objects are not geometrically equivalent
*/
RHINO_SDK_FUNCTION
bool RhinoCompareGeometry(const ON_Geometry* a, const ON_Geometry* b);

/*
Description:
  Compares the geometry of two objects, similarly to the SelDup command.
Parameters:
  a - [in] The first object to compare
  b - [in] The second object to compare
Returns:
  true:  The two objects are geometrically equivalent
  false:  The two objects are not geometrically equivalent
*/
RHINO_SDK_FUNCTION
bool RhinoCompareGeometry(const CRhinoObject* a, const CRhinoObject* b);

/*
Description:
  Dolly the camera location and so that the view frustum contains
  camcoord_bbox and the volume of camcoord_bbox fills the frustum.
  If the projection is perspective, the camera angle is not changed.
Parameters:
  current_vp - [in]
    current projection - must be valid.
  camcoord_bbox - [in]
    valid bounding box in current_vp camera coordinates
  zoomed_vp - [out] (can be the same as current_vp
    projection
Returns:
  True if successful.
*/
RHINO_SDK_FUNCTION
bool RhinoDollyExtents(
  const ON_Viewport& current_vp,
  ON_BoundingBox camcoord_bbox,
  ON_Viewport& zoomed_vp
);

/*
Description:
  Dolly the camera location and so that the view frustum contains
  the list of objects and the objects are as large as possible.
  If the projection is perspective, the camera angle is not changed.
Parameters:
  it - [in]
    list of objects that need to be seen in the view.
  current_vp - [in]
    current projection - must be valid.
  border - [in]
    If border > 1.0, then the frustum in enlarged by this factor
    to provide a border around the view.  1.1 works well for
    parallel projections; 0.0 is suggested for perspective projections.
  zoomed_vp - [out] (can be the same as current_vp)
    projection with a view frustum that contains the list
    of objects.
  extra_points - [in] (default = NULL)
    optional list of world 3d points that need to be included
    in the view.
  camcoord_bbox - [out] (default = NULL)
    will be set to the camera coordinate bounding used to compute the
    extents with respect to the viewport
Returns:
  True if successful.
Remarks:
  This version of RhinoDollyExtents calculates a tight camera
  coordinate bounding box of the list of objects and calls

     RhinoDollyExtents( current_vp, camcoord_bbox, zoomed_vp )

  to calculate the new projection.
*/
RHINO_SDK_FUNCTION
bool RhinoDollyExtents(
  class CRhinoObjectIterator& it,
  const CRhinoViewport& rhino_vp,
  double border,
  ON_Viewport& zoomed_vp,
  const ON_SimpleArray<ON_3dPoint>* extra_points = nullptr,
  ON_BoundingBox* camcoord_box = nullptr
);

/*
Description:
  Dolly the camera location and so that the view frustum contains
  all of the document objects that can be seen in view.
  If the projection is perspective, the camera angle is not changed.
Parameters:
  viewport - [in/out]
  border - [in]
    If border > 1.0, then the frustum in enlarged by this factor
    to provide a border around the view.  1.1 works well for
    parallel projections; 0.0 is suggested for perspective projections.
Returns:
  True if successful.
Remarks:
  This version of RhinoDollyExtents sets up an appropriate
  iterator and uses:

     RhinoDollyExtents( iterator, viewport.VP(), border, viewport.VP() )

  to calculate the new projection.
*/
RHINO_SDK_FUNCTION
bool RhinoDollyExtents(
  class CRhinoViewport& viewport,
  double border
);

/*
Description:
  Dolly the camera location and so that the view frustum contains
  all of the selected document objects that can be seen in view.
  If the projection is perspective, the camera angle is not changed.
Parameters:
  viewport - [in/out]
  border - [in]
    If border > 1.0, then the frustum in enlarged by this factor
    to provide a border around the view.  1.1 works well for
    parallel projections; 0.0 is suggested for perspective projections.
Returns:
  True if successful.
Remarks:
  This version of RhinoDollyExtentsSelected sets up an appropriate
  iterator and uses

     RhinoDollyExtents( iterator, viewport.VP(), border, viewport.VP() )

  to calculate the new projection.
*/
RHINO_SDK_FUNCTION
bool RhinoDollyExtentsSelected(
  class CRhinoViewport& viewport,
  double border
);

/*
Description:
  Dolly the camera location and so that the view frustum contains
  all of the selected document objects that can be seen in view.
  If the projection is perspective, the camera angle is not changed.
Parameters:
  view - [in/out] The view to adjust
  active_viewport - [in] if true, dolly is performed on the ActiveViewport of the view
                         if false, dolly is performed on the MainViewport of the view
Returns:
  True if successful.
Remarks:
  This version of RhinoDollyExtents determines what border factor
  would be used by the Rhino Zoom command and calls
  RhinoDollyExtents(view,border).
*/
RHINO_SDK_FUNCTION
bool RhinoDollyExtents(
  class CRhinoView* view,
  bool active_viewport = true
);

/*
Description:
  Dolly the camera location and so that the view frustum contains
  all of the selected document objects that can be seen in view.
  If the projection is perspective, the camera angle is not changed.
Parameters:
  view - [in/out]
  active_viewport - [in] if true, dolly is performed on the ActiveViewport of the view
                         if false, dolly is performed on the MainViewport of the view
Returns:
  True if successful.
Remarks:
  This version of RhinoDollyExtentsSelected determines what border factor
  would be used by the Rhino Zoom command and calls
  RhinoDollyExtentsSelected(view,border).
*/
RHINO_SDK_FUNCTION
bool RhinoDollyExtentsSelected(
  class CRhinoView* view,
  bool active_viewport = true
);

/*
Description:
  Test math library functions in the TL library.
Parameters:
  function_index - [in]  Determines which math library function is called.

           1:    z = x+y
           2:    z = x-y
           3:    z = x*y
           4:    z = x/y
           5:    z = fabs(x)
           6:    z = exp(x)
           7:    z = log(x)
           8:    z = logb(x)
           9:    z = log10(x)
          10:    z = pow(x,y)
          11:    z = sqrt(x)
          12:    z = sin(x)
          13:    z = cos(x)
          14:    z = tan(x)
          15:    z = sinh(x)
          16:    z = cosh(x)
          17:    z = tanh(x)
          18:    z = asin(x)
          19:    z = acos(x)
          20:    z = atan(x)
          21:    z = atan2(y,x)
          22:    z = fmod(x,y)
          23:    z = modf(x,&y)
          24:    z = frexp(x,&y)

  double x - [in]
  double y - [in]
Returns:
  Returns the "z" value listed in the function_index parameter
  description.
Remarks:
  This function is used to test the results of class floating
  point functions.  It is primarily used to see what happens
  when Rhino performs illegal math library calls.
*/
RHINO_SDK_FUNCTION
double Rhino_TestMathFunction(
  int function_index,
  double x,
  double y
);

/*
Description:
  Surface intersector.
Parameters:
  B0        - [in]  The first input surface.
  B1        - [in]  The second input surface.
  tolerance - [in]  The fitting and near miss tolerance.
  curves    - [out] The output 3d curves. NOTE, the caller is responsible for destroying these curves.
  points    - [out] If non-NULL, the 3d intersection points.
Returns:
  TRUE if successful.
  FALSE if B0 == B1, or on error.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectSurfaces(
  const ON_Surface& srf0,
  const ON_Surface& srf1,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray* points = nullptr
);

/*
Description:
  Brep intersector.
Parameters:
  B0        - [in]  The first input Brep.
  B1        - [in]  The second input Brep.
  tolerance - [in]  The fitting and near miss tolerance.
  curves    - [out] The output 3d curves. NOTE, the caller is responsible for destroying these curves.
  points    - [out] If non-NULL, the 3d intersection points.
Returns:
  TRUE if successful.
  FALSE if B0 == B1, or on error.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectBreps(
  const ON_Brep& brep0,
  const ON_Brep& brep1,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray* points = nullptr
);

/*
Description:
  Brep - curve intersector. Gets 3d points of intersection
  and 3d overlap curves.  Returns FALSE if there is an error processing an
  overlap curve.  In that case, partial results will be returned.
Parameters:
  crv    - [in] input curve
  brep   - [in] input Brep
  tol    - [in] fitting and near miss tolerance
  curves - [out] output 3d overlap curves
  points - [out] output 3d intersection points
Returns:
  TRUE - successful
  FALSE - failure. Partial results will be returned
*/
RHINO_SDK_FUNCTION
bool RhinoCurveBrepIntersect(
  const ON_Curve& crv,
  const ON_Brep& brep,
  double tol,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray& points
);

/*
Description:
  Brep intersector.
Parameters:
  B0          - [in]  The first input Brep.
  B1          - [in]  The second input Brep.
  tolerance   - [in]  The fitting and near miss tolerance.
  bJoinCurves - [in]  If true, join the resulting curves where possible
  curves      - [out] The output 3d curves. NOTE, the caller is responsible for destroying these curves.
  points      - [out] If non-NULL, the 3d intersection points.
Returns:
  TRUE if successful.
  FALSE if B0 == B1, or on error.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectBreps(
  const ON_Brep& B0,
  const ON_Brep& B1,
  double tolerance,
  bool bJoinCurves,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray* points
);

/*
Description:
  Plane - Brep intersector.
Parameters:
  plan        - [in]  The plane.
  brep        - [in]  The Brep.
  tolerance   - [in]  The fitting and near miss tolerance.
  curves      - [out] The output 3d curves. NOTE, the caller is responsible for destroying these curves.
  points      - [out] If non-NULL, the 3d intersection points.
Returns:
  TRUE if successful.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectPlaneBrep(
  const ON_Plane& plane,
  const ON_Brep& brep,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray* points
);

/*
Description:
  Plane - Brep intersector.
Parameters:
  plan        - [in]  The plane.
  brep        - [in]  The Brep.
  tolerance   - [in]  The fitting and near miss tolerance.
  bJoinCurves - [in]  If true, join the resulting curves where possible
  curves      - [out] The output 3d curves. NOTE, the caller is responsible for destroying these curves.
  points      - [out] If non-NULL, the 3d intersection points.
Returns:
  TRUE if successful.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectPlaneBrep(
  const ON_Plane& plane,
  const ON_Brep& brep,
  double tolerance,
  bool bJoinCurves,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray* points
);

/*
Description:
  Brep face - curve intersector. Gets 3d points of intersection
  and 3d overlap curves.  Returns FALSE if there is an error processing an
  overlap curve.  In that case, partial results will be returned.
Parameters:
  crv    - [in] input curve
  face   - [in] input face
  tol    - [in] fitting and near miss tolerance
  curves - [out] output 3d overlap curves
  points - [out] output 3d intersection points
Returns:
  TRUE - successful
  FALSE - failure. Partial results will be returned
*/
RHINO_SDK_FUNCTION
bool RhinoCurveFaceIntersect(
  const ON_Curve& crv,
  const ON_BrepFace& face,
  double tol,
  ON_SimpleArray<ON_Curve*>& curves,
  ON_3dPointArray& points
);

/*
Description:
  Finds a point on a Brep that is closest to a test point.
Parameters:
  brep - [in] input Brep
  pt   - [in] base point to project to surface
  ci   - [out] Component index of the Brep component that
               contains the closest point. Possible types are
               brep_face, brep_edge or brep_vertex
  s    - [out] If the component_index type is brep_edge, then s is the parameter
               of the closest edge point.
  t    - [out] If the component_index type is brep_face, then (s,t) is the parameter
               of the closest edge point.
  brep_point - [out] if not NULL and true is returned, this is the location of
                     the closest Brep point.
  maximum_distance - [in].  If maximum_distance > 0, then only points whose distance to p0
                            is <= maximum_distance will be returned.  Using a positive
                            value of maximum_distance can substantially speed up the search.
Returns:
  TRUE - successful
  FALSE - failure - No point exists within maximum_distance
See Also:
  ON_Surface::GetClosestPoint
  ON_Brep::ComponentIndexType
*/
RHINO_SDK_FUNCTION
bool RhinoBrepClosestPoint(
  const ON_Brep& brep,
  ON_3dPoint pt,
  ON_COMPONENT_INDEX* ci,
  double* s,
  double* t,
  ON_3dPoint* brep_point = nullptr,
  double maximum_distance = 0.0
);

/*
Description:
  Get the parameters of the point on the Brep face that is closest to P.
Parameters:
  face - [in] face to test
  P    - [in] test point
  s    - [out]
  t    - [out]
            (*s,*t) = parameters of the surface point that
            is closest to P.
  maximum_distance = 0.0 - [in]
          optional upper bound on the distance from P to
          the surface.  If you are only interested in
          finding a point Q on the surface when
          P.DistanceTo(Q) < maximum_distance, then set
          maximum_distance to that value.
  sdomain = 0 - [in] optional domain restriction
  tdomain = 0 - [in] optional domain restriction
Returns:
  True if successful.  If false, the values of *s and *t are undefined.
See Also:
  ON_Surface::GetClosestPoint
  RhinoBrepClosestPoint
*/
RHINO_SDK_FUNCTION
bool RhinoBrepFaceClosestPoint(
  const ON_BrepFace& face,
  ON_3dPoint P,
  double* s,
  double* t,
  double maximum_distance = 0.0,
  const ON_Interval* sdomain = nullptr,
  const ON_Interval* tdomain = nullptr
);

/*
Description:
  Surface splitter.
Parameters:
  srf          - [in]  The surface to be split
  bFlip        - [in]  If true, input curves are oriented clockwise.
  EdgeCurves   - [in]  The new edge curves (with consistent orientation).
  ftol         - [in]  The fitting tolerance.
  ctol         - [in]  Used to decide which face to keep. For best results, should be at least 2*ftol.
  Breps        - [out] The results of the split.
  max_tol_used - [in]  Default is NULL. ftol is the desired trimming tolerance.
                          If the curves are not within ftol of the surface,
                          the distance to the surface is used for trimming.
                          max_ftol_used returns the maximum of the tolerances used.
                          Always at least ftol.
*/
RHINO_SDK_FUNCTION
void RhinoCutUpSurface(
  const ON_Surface& srf,
  BOOL32 bFlip,
  const ON_SimpleArray<ON_Curve*>& EdgeCurves,
  double ftol,
  double ctol,
  ON_SimpleArray<ON_Brep*>& Breps,
  double* max_tol_used = nullptr
);

/*
Description:
  Join Breps into a single Brep.
Parameters:
  breps     - [in] Array of Breps to join.
                   (THESE ARE MODIFIED AND SHOULD BE DELETED BY CALLER!)
  tolerance - [in] 3d join tolerance.
Returns:
  Brep made by joining all the inputs together.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoJoinBreps(
  ON_SimpleArray<ON_Brep*>& Breps,
  double tolerance
);

/*
Description:
  Find the union of the Breps in InBreps. Separate into disjoint pieces.
Parameters:
  InBreps       - [in]  Breps to be unioned.
  tolerance     - [in]  distance tolerance
  bResult       - [out] True if any of the Breps had intersecting faces.
                        If false and OutBreps.Count() is less than InBreps.Count(),
                        then some closed Breps were inside others,
                        or bManifoldOnly == true and some were non-manifold.
  OutBreps      - [out] Resulting Breps.
  bManifoldOnly - [in]  If true, leave out any non-manifold input Breps.
  bRaisedTol    - [out] If non-null and Boolean fails at tolerance,
                        try at twice tolerance and indicate success.
Returns:
  false if error
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanUnion(
  const ON_SimpleArray<const ON_Brep*>& InBreps,
  double tolerance,
  bool* bResult,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  bool bManifoldOnly = true,
  bool* bRaisedTol = nullptr
);

/*
Description:
  Find the union of the Breps in InBreps. Separate into disjoint pieces.
Parameters:
  InBreps               - [in]  Breps to be unioned.
  tolerance             - [in]  Distance tolerance
  bResult               - [out] True if any of the Breps had intersecting faces.
                                If false and OutBreps.Count() is less than InBreps.Count(),
                                then some closed Breps were inside others,
                                or bManifoldOnly == true and some were non-manifold.
  OutBreps              - [out] Resulting Breps. The caller is reponsible for the allocated memory.
  NakedEdgePoints       - [out] If Boolean failed because the intersection hit a naked edge, a
                                point will be added where the intersection hits the edge.
  BadIntersectionPoints - [out] If Boolean failed because an intersection ends on the interior
                                of both surfaces, a point is added. This happens when the surface
                                intersector fails.
  NonmanifoldEdgePoints - [out] If Boolean failed because the intersection hit a non-manifold edge, a
                                point will be added where the intersection hits the edge.
  bManifoldOnly         - [in]  If true, leave out any non-manifold input Breps.
  bRaisedTol            - [out] If non-null and Boolean fails at tolerance,
                                try at twice tolerance and indicate success.
Returns:
  false if error
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanUnion(
  const ON_SimpleArray<const ON_Brep*>& InBreps,
  double tolerance,
  bool* bResult,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  ON_SimpleArray<ON_3dPoint>& NakedEdgePoints,
  ON_SimpleArray<ON_3dPoint>& BadIntersectionPoints,
  ON_SimpleArray<ON_3dPoint>& NonmanifoldEdgePoints,
  bool bManifoldOnly = true,
  bool* bRaisedTol = nullptr
);

/*
Description:
  Intersect each Brep of InBreps0 with the Breps of InBreps1, Separate into disjoint pieces.
Parameters:
  InBreps0      - [in]  First set of input Breps.
  InBreps1      - [in]  Second set of input Breps.
  tolerance     - [in]  Distance tolerance.
  bResult       - [out] True if any of the Breps had intersecting faces,
                        If false, and OutBreps.Count() is not empty,
                        then some closed Breps were inside others,
                        or bManifoldOnly == true and some were non-manifold.
  OutBreps      - [out] Resulting Breps.
  bManifoldOnly - [in]  If true, leave out any non-manifold input Breps.
  bRaisedTol    - [out] If non-null and Boolean fails at tolerance,
                        try at twice tolerance and indicate success.
Returns:
  false if error
Remarks:
  Think of this as an operation on InBreps0. Each Brep of InBreps1 may remove part of a Brep of
  InBreps0.  If a member of InBreps0 does not intersect any member of InBreps1, then it is copied to
  OutBreps.
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanIntersection(
  const ON_SimpleArray<const ON_Brep*>& InBreps0,
  const ON_SimpleArray<const ON_Brep*>& InBreps1,
  double tolerance,
  bool* bResult,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  bool bManifoldOnly = true,
  bool* bRaisedTol = nullptr
);

/*
Description:
  Intersect each Brep of InBreps0 with the Breps of InBreps1, Separate into disjoint pieces.
Parameters:
  InBreps0              - [in]  First set of input Breps.
  InBreps1              - [in]  Second set of input Breps.
  tolerance             - [in]  Distance tolerance.
  bResult               - [out] True if any of the Breps had intersecting faces.
                                If false, and OutBreps.Count() is not empty,
                                then some closed Breps were inside others,
                                or bManifoldOnly == true and some were non-manifold.
  OutBreps              - [out] Resulting Breps.
  NakedEdgePoints       - [out] If Boolean failed because the intersection hit a naked edge, a
                                point will be added where the intersection hits the edge.
  BadIntersectionPoints - [out] If Boolean failed because an intersection ends on the interior
                                of both surfaces, a point is added. This happens when the surface
                                intersector fails.
  NonmanifoldEdgePoints - [out] If Boolean failed because the intersection hit a non-manifold edge, a
                                point will be added where the intersection hits the edge.
  bManifoldOnly         - [in]  If true, leave out any non-manifold input Breps.
  bRaisedTol            - [out] If non-null and Boolean fails at tolerance,
                                try at twice tolerance and indicate success.
Returns:
  false if error
Remarks:
  Think of this as an operation on InBreps0. Each Brep of InBreps1 may remove part of a Brep of
  InBreps0. If a member of InBreps0 does not intersect any member of InBreps1, then it is copied to
  OutBreps.
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanIntersection(
  const ON_SimpleArray<const ON_Brep*>& InBreps0,
  const ON_SimpleArray<const ON_Brep*>& InBreps1,
  double tolerance,
  bool* bResult,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  ON_SimpleArray<ON_3dPoint>& NakedEdgePoints,
  ON_SimpleArray<ON_3dPoint>& BadIntersectionPoints,
  ON_SimpleArray<ON_3dPoint>& NonmanifoldEdgePoints,
  bool bManifoldOnly = true,
  bool* bRaisedTol = nullptr
);

/*
Description:
  Subtract each Brep of Inbreps1 from each Brep of InBreps0.
Parameters:
  InBreps0       - [in]  First set of input Breps.
  InBreps1       - [in]  Second set of input Breps.
  tolerance      - [in]  Distance tolerance.
  bResult        - [out] True if any of the Breps had intersecting faces.
                         If false, and OutBreps.Count() is less than InBreps0.Count(),
                         then some closed Breps were inside others,
                         or bManifoldOnly == true and some were non-manifold.
  OutBreps       - [out] Resulting Breps.
  IndexForInputs - [out] OutBrep[i] is the result if subtracting something from InBreps0[InputIndexForOutPut[i]].
  bManifoldOnly  - [in]  If true, leave out any non-manifold input Breps.
  bRaisedTol     - [out] If non-null and Boolean fails at tolerance,
                         try at twice tolerance and indicate success.
Returns:
  false if error
Remarks:
  Think of this as an operation on InBreps0. Each Brep of InBreps1 may remove part of a Brep of
  InBreps0. If a member of InBreps0 does not intersect any member of InBreps1, then it is copied to
  OutBreps.
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanDifference(
  const ON_SimpleArray<const ON_Brep*>& InBreps0,
  const ON_SimpleArray<const ON_Brep*>& InBreps1,
  double tolerance,
  bool* bResult,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  ON_SimpleArray<int>& InputIndexForOutput,
  bool bManifoldOnly = true,
  bool* bRaisedTol = nullptr
);

/*
Description:
  Subtract each Brep of Inbreps1 from each Brep of InBreps0.
Parameters:
  InBreps0              - [in]  First set of input Breps.
  InBreps1              - [in]  Second set of input Breps.
  tolerance             - [in]  Distance tolerance.
  bResult               - [out] True if any of the Breps had intersecting faces.
                                If false, and OutBreps.Count() is less than InBreps0.Count(),
                                then some closed Breps were inside others,
                                or bManifoldOnly == true and some were non-manifold.
  OutBreps              - [out] Resulting Breps.
  NakedEdgePoints       - [out] If Boolean failed because the intersection hit a naked edge, a
                                point will be added where the intersection hits the edge.
  BadIntersectionPoints - [out] If Boolean failed because an intersection ends on the interior
                                of both surfaces, a point is added. This happens when the surface
                                intersector fails.
  NonmanifoldEdgePoints - [out] If Boolean failed because the intersection hit a non-manifold edge, a
                                point will be added where the intersection hits the edge.
  IndexForInputs        - [out] OutBrep[i] is the result if subtracting something from InBreps0[InputIndexForOutPut[i]].
  bManifoldOnly         - [in]  If true, leave out any non-manifold input Breps.
  bRaisedTol            - [out] If non-null and Boolean fails at tolerance,
                                try at twice tolerance and indicate success.
Returns:
  false if error
Remarks:
  Think of this as an operation on InBreps0. Each Brep of InBreps1 may remove part of a Brep of
  InBreps0. If a member of InBreps0 does not intersect any member of InBreps1, then it is copied to
  OutBreps.
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanDifference(
  const ON_SimpleArray<const ON_Brep*>& InBreps0,
  const ON_SimpleArray<const ON_Brep*>& InBreps1,
  double tolerance,
  bool* bResult,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  ON_SimpleArray<ON_3dPoint>& NakedEdgePoints,
  ON_SimpleArray<ON_3dPoint>& BadIntersectionPoints,
  ON_SimpleArray<ON_3dPoint>& NonmanifoldEdgePoints,
  ON_SimpleArray<int>& InputIndexForOutput,
  bool bManifoldOnly = true,
  bool* bRaisedTol = nullptr
);

/*
Description:
  Splits shared areas of Breps and creates separate Breps from the shared and unshared parts.
Parameters:
  InBreps0  - [in]  The Breps to split.
  InBreps1  - [in]  The cutting Breps.
  tolerance - [in]  distance tolerance
  OutBreps  - [out] Resulting Breps
Returns:
  true if successful
Remarks:
  NOTE, memory for OutBreps is allocated and becomes the responsibility of the caller.
 */
RHINO_SDK_FUNCTION
bool RhinoBooleanSplit(
  const ON_SimpleArray<const ON_Brep*>& InBreps0,
  const ON_SimpleArray<const ON_Brep*>& InBreps1,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutBreps
);

/*
Description:
  Projects curves onto Breps
Parameters:
  Breps        - [in]  The Breps to project onto
  Curves       - [in]  The curves to project
  ProjDir      - [in]  The direction to project
  OutCurves    - [out] The output, or projected, curves.
                       NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE CURVES.
  CurveIndices - [out] OutCurves[i] is a projection of Curves[CurveIndices[i]]
  BrepIndices  - [out] OutCurves[i] was projected onto Breps[BrepIndices[i]]
  tolerance    - [in]  The projection tolerance.
Returns:
  true if at least one of the input curves was projected, otherwise false.
See Also:
  RhinoProjectCurvesToMeshes
  RhinoProjectPointsToBreps
  RhinoProjectPointsToMeshes
*/
RHINO_SDK_FUNCTION
bool RhinoProjectCurvesToBreps(
  ON_SimpleArray<const ON_Brep*>& Breps,
  ON_SimpleArray<const ON_Curve*>& Curves,
  const ON_3dVector& ProjDir,
  ON_SimpleArray<ON_Curve*>& OutCurves,
  ON_SimpleArray<int>& CurveIndices,
  ON_SimpleArray<int>& BrepIndices,
  double tolerance
);

/*
Description:
  Projects points onto Breps
Parameters:
  Breps        - [in]  The Breps to project onto
  Points       - [in]  The points to project
  ProjDir      - [in]  The direction to project
  OutPoints    - [out] The output, or projected, points.
  Indices      - [out] OutPoints[i] is a projection of Points[Indices[i]]
  tolerance    - [in]  The projection tolerance - used for culling close
                       points and for line-Brep intersection.
Returns:
  true if at least one of the input points was projected, otherwise false.
See Also:
  RhinoProjectCurvesToBreps
  RhinoProjectCurvesToMeshes
  RhinoProjectPointsToMeshes
*/
RHINO_SDK_FUNCTION
bool RhinoProjectPointsToBreps(
  ON_SimpleArray<const ON_Brep*>& Breps,
  const ON_3dPointArray& Points,
  const ON_3dVector& ProjDir,
  ON_3dPointArray& OutPoints,
  ON_SimpleArray<int>& Indices,
  double tolerance
);

/*
Description:
  Get a polyline approximation of a curve
Parameters:
  curve - [in]
  main_seg_count - [in]
  sub_seg_count - [in]
    If main_seg_count <= 0, then both these parameters are ignored.
    If main_seg_count > 0, then sub_seg_count must be >= 1.  In this
    case the NURB will be broken into main_seg_count equally spaced
    chords.  If needed, each of these chords can be split into as many
    sub_seg_count sub-parts if the subdivision is necessary for the
    mesh to meet the other meshing constraints.  In particular, if
    sub_seg_count = 0, then the curve is broken into main_seg_count
    pieces and no further testing is performed.
  max_ang_radians - [in] ( 0 to pi )
    Maximum angle (in radians) between unit tangents at adjacent
    vertices.
  max_chr - [in]
    Maximum permitted value of
      (distance chord midpoint to curve) / (length of chord)
  max_aspect - [in]
    If max_aspect < 1.0, the parameter is ignored.  If
    1 <= max_aspect < sqrt(2), it is treated as if max_aspect = sqrt(2).
    This parameter controls the maximum permitted value of
      (length of longest chord) / (length of shortest chord)
  tolerance - [in]
    If tolerance = 0, the parameter is ignored.
    This parameter controls the maximum permitted value of the
    distance from the curve to the polyline.
  max_edge_length - [in]
    If max_edge_length = 0, the parameter is ignored.
    This parameter controls the maximum permitted edge length.
  polyline - [out] The polyline approximation of the NURBS curve is appended
      to this polyline.  If bKeepStartPoint is FALSE, the starting point is
      not appended.
  bKeepStartPoint - [in]
    TRUE  The starting point of the curve is added to the polyline
    FALSE The starting point of the curve is not added to the polyline
  curve_domain - [in]
    if not NULL, this sub-domain of the NURBS curve is approximated.
Returns:
  TRUE if successful.
*/
RHINO_SDK_FUNCTION
bool RhinoConvertCurveToPolyline(
  const ON_Curve& curve,
  int main_seg_count,
  int sub_seg_count,
  double max_ang_radians,
  double max_chr,
  double max_aspect,
  double tolerance,
  double min_edge_length,
  double max_edge_length,
  ON_PolylineCurve& polyline,
  bool bKeepStartPoint = true,
  ON_Interval* curve_domain = nullptr
);

/*
Description:
  Calculation tool for the Rhino DivideCurve command.
Parameters:
  curve - [in];
  seg_count - [in]
    If > 0, the curve will be divided into this many segments.
    If > 0, then seg_length must be 0.
  seg_length -[in]
    If > 0, the curve will be divided into segments of this length.
    If > 0, then seg_count must be 0.
  bReverse - [in]
    If true and seg_length is used to divide the curve,
    then the divisions start from the end.
  bIncludeEnd - [in]
    If true, then the point at the start of the first division
  segment is returned.
  curve_P - [out]
    If not NULL, the division points are returned here.
  curve_t - [out]
    If not NULL, the curve parameters at the division points
    are returned here.
*/
RHINO_SDK_FUNCTION
bool RhinoDivideCurve(
  const ON_Curve& curve,
  double seg_count,
  double seg_length,
  bool bReverse,
  bool bIncludeEnd,
  ON_SimpleArray<ON_3dPoint>* curve_P,
  ON_SimpleArray<double>* curve_t
);

/*
Description:
  OBSOLETE - Use the version of RhinoOffsetCurve declared below.
*/
ON_DEPRECATED_MSG("use other version of RhinoOffsetCurve")
RHINO_SDK_FUNCTION
bool RhinoOffsetCurve(
  const ON_Curve& curve,
  const ON_3dVector& normal,
  const ON_3dPoint& origin,
  double distance,
  ON_SimpleArray<ON_Curve*>& offset_curves,
  double tolerance = 0.0,
  double angle_tolerance = 0.0,
  int corner_style = 1
);

/*
Description:
  Offsets a curve.
Parameters:
  curve_in        - [in] The curve to offset.
  distance        - [in] The distance to offset the curve.
  direction_point - [in] A point that indicates the direction of the offset.
  normal          - [in] A vector that indicates the normal of the plane in which the offset will occur.
  corner_style    - [in] The corner style, where: no_corner = 0, sharp = 1, round = 2, smooth = 3, chamfer = 4
  tolerance       - [in] The offset tolerance.
  angle_tolerance - [in] The angle tolerance in radians, used to decide whether to split at kinks.
  bLoose          - [in] If false, offset within tolerance. If true, offset by moving edit points.
  end_style       - [in] Closes the gap at the curve ends, where: none = 0, flat = 1, 2 = round.
  curves_out      - [out] The offset curves.
Returns:
  Returns number of curves that are appended to array.
Remarks:
  The caller is responsible for deleting these curves when finished.
*/
RHINO_SDK_FUNCTION
int RhinoOffsetCurve(
  const ON_Curve& curve_in,
  double distance,
  ON_3dPoint direction_point,
  ON_3dVector normal,
  int corner_style,
  double tolerance,
  double angle_tolerance,
  bool bLoose,
  int end_style,
  ON_SimpleArray<ON_Curve*>& curves_out
);

RHINO_SDK_FUNCTION
int RhinoOffsetCurve(
  const ON_Curve& curve_in,
  double distance,
  ON_3dPoint direction_point,
  ON_3dVector normal,
  int corner_style,
  double tolerance,
  double angle_tolerance,
  // bLoose = false,
  // end_style = 0,
  ON_SimpleArray<ON_Curve*>& curves_out
);

RHINO_SDK_FUNCTION
int RhinoOffsetCurve(
  const ON_Curve& curve_in,
  double distance,
  ON_3dPoint direction_point,
  ON_3dVector normal,
  int corner_style,
  double tolerance,
  // angle_tolerance = ON_DEFAULT_ANGLE_TOLERANCE,
  // bLoose = false,
  // end_style = 0,
  ON_SimpleArray<ON_Curve*>& curves_out
);

/*
Description:
  Offsets a closed curve in the following way:
  Project the curve to a plane with given normal.  Loose Offset the projection
  by distance + blend_radius and trim off self-intersection. Offset the remaining curve
  back in the opposite direction by blend_radius, filling gaps with blends.
  Finally, use the elevations of the input curve to get the correct elevations of the result.
Parameters:
  curve_in        - [in] the curve to offset.
  distance        - [in] the positive distance to offset the curve.
  blend_radius    - [in] Positive, typically the same as distance.
                         When the offset results in a self-intersection that
                         gets trimmed off at a kink, the kink will be blended
                         out using this radius
  direction_point - [in] a point that indicates the direction of the offset.
                         If the offset is inward, the point's projection to the plane should be
                         well within the curve.  It will be used to decide which part of the offset
                         to keep if there are self-intersections.
  normal          - [in] a vector that indicates the normal of the plane in which the offset will occur.
  tolerance       - [in] Used to determine self-intersections, not offset error.
  CrossSections   - [out] If not NULL, should be a pointer to valid array.  It will
                          contain lines between the input and the offset that might
                          be good for use in sweep2 or another surface creation tool.
  RuledSrfs       - [out] If not NULL, should be a pointer to valid array.  It will
                          contain ruled surfaces between the input and the parts of the offset
                          that correspond exactly.  There will be gaps between these at blends.
Returns:
  the result or nullptr.
Remarks:
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoRibbonOffsetCurve(
  const ON_Curve& curve_in,
  double distance,
  double blend_radius,
  ON_3dPoint direction_point,
  ON_3dVector normal,
  double tolerance,
  ON_SimpleArray<ON_Curve*>* CrossSections,
  ON_SimpleArray<ON_Surface*>* RuledSrfs
);

/*
Description:
  Offsets a closed curve in the following way:
  Project the curve to a plane with given normal.  Loose Offset the projection
  by distance + blend_radius and trim off self-intersection. Offset the remaining curve
  back in the opposite direction by blend_radius, filling gaps with blends.
  Finally, use the elevations of the input curve to get the correct elevations of the result.
Parameters:
  curve_in        - [in] the curve to offset.
  distance        - [in] the positive distance to offset the curve.
  blend_radius    - [in] Positive, typically the same as distance.
                         When the offset results in a self-intersection that
                         gets trimmed off at a kink, the kink will be blended
                         out using this radius
  direction_point - [in] a point that indicates the direction of the offset.
                         If the offset is inward, the point's projection to the plane should be
                         well within the curve.  It will be used to decide which part of the offset
                         to keep if there are self-intersections.
  normal          - [in] a vector that indicates the normal of the plane in which the offset will occur.
  tolerance       - [in] Used to determine self-intersections, not offset error.
  CrossParams     - [out] A list of parameter pairs for creating cross sections.
                          CP[i].x is for the resulting offset curve.  CP[i].y is for curve_in.
Returns:
  the result or nullptr.
Remarks:
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoRibbonOffsetCurve(
  const ON_Curve& curve_in,
  double distance,
  double blend_radius,
  ON_3dPoint direction_point,
  ON_3dVector normal,
  double tolerance,
  ON_SimpleArray<ON_2dPoint>& CrossParams
);

/*
Description:
  Determine if a point is in the active region of a face.
Parameters:
  face       - [in] The Brep face to test.
  tolerance  - [in] 3d tolerance
  bOkToWrap  - [in] If true and if surface the face is built on is closed,
                    the the test parameter is permitted to wrap across the
                    surface seam.  If the (s,t)parameters are the result
                    of a closest point or intersection query, then you
                    generally want bOkToWrap = true.
  s          - [in/out] (s,t) is the parameter to be tested.
  t          - [in/out] (s,t) is the parameter to be tested.
Returns:
  True if (s,t) is on the active part of the face.
Remarks:
  If true is returned, then the returned values of
  (s,t) are the parameters of the active point.  When
  bOkToWrap is true and the surface is closed, the
  return values may differ from the input values.
*/
RHINO_SDK_FUNCTION
bool RhinoIsPointOnFace(
  const ON_BrepFace& face,
  double tolerance,
  bool bOkToWrap,
  double& s,
  double& t
);

/*
Description:
  Merge adjacent edges to this edge recursively using tolerance
Parameters:
  edge      - [in] on edge in the Brep to be modified
  angtol    - [in] The maximum allowable difference of angle in radian between
                   adjacent edges that can be merged.
  merge_cnt - [out] Number of edges merged into final edge
Returns:
  A pointer to the resulting merged edge. or NULL if nothing was merged.
Details
  A pair of adjacent edges in a Brep are mergeable if the angle between them is less than
  tolerance and the valence of the shared vertex is 2.
*/
RHINO_SDK_FUNCTION
ON_BrepEdge* RhinoMergeAdjoiningEdges(
  ON_BrepEdge& edge,
  double angtol,
  int* merge_cnt = nullptr
);

/*
Description:
  Determine if a 3D point is inside of a Brep.
  This function only makes sense for closed manifold Breps.
Parameters:
  pBrep       - [in] the Brep.
  pt          - [in] 3d point to test.
  tolerance   - [in] 3d distance tolerance used for intersection
                     and determining strict inclusion.
  bStrictlyIn - [in] if true, pt is in if inside pBrep by at least tolerance.
                     if false, pt is in if truly in or within tolerance of boundary.
Returns:
  true if pt is in, false if not.  See parameter bStrictlyIn.
*/
RHINO_SDK_FUNCTION
bool RhinoIsPointInBrep(
  const ON_Brep* pBrep,
  ON_3dPoint pt,
  double tolerance = ON_SQRT_EPSILON,
  bool bStrictlyIn = false
);

/*
Description:
  Calculates the minimum and maximum distances between two curves.
Parameters:
  crv_a [in] - curve A
  dom_a [in] - optional domain for curve A
  crv_b [in] - curve B
  dom_b [in] - optional domain for curve B
  tol   [in] - tolerance for common normal detection
  lim   [in] - limits acceptable overlap distance
  cnt   [out] - overlap interval count (0 to 3)
  int_a [out] - curve A interval(s)
  int_b [out] - curve B interval(s)
  max_a [out] - curve A parameter at maximum overlap distance point
  max_b [out] - curve B parameter at maximum overlap distance point
  max_d [out] - maximum overlap distance
  min_a [out] - curve A parameter at minimum distance point
  min_b [out] - curve B parameter at minimum distance point
  min_d [out] - minimum distance between curves
Returns:
  true if successful, false on failure or if no no overlap found
*/
RHINO_SDK_FUNCTION
bool RhinoGetOverlapDistance(
  const ON_Curve* crv_a,
  const ON_Interval* dom_a,
  const ON_Curve* crv_b,
  const ON_Interval* dom_b,
  double tol,
  double lim,
  int* cnt,
  double int_a[3][2],
  double int_b[3][2],
  double* max_a,
  double* max_b,
  double* max_d,
  double* min_a,
  double* min_b,
  double* min_d
);

/*
Description:
  Make planar faces out of planar closed boundaries.
Parameters:
  pInputBrep  - [in] the Brep.
  tol         - [in] tolerance to determine planarity and for joining
Returns:
  A joined, capped Brep if successful. Null if it failed.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepCapPlanarHoles(
  const ON_Brep* pInputBrep,
  const double& tol
);

/*
Description:
  Make planar faces out of planar closed boundaries.
Parameters:
  pInputBrep      - [in] the Brep.
  tol             - [in] tolerance to determine planarity and for joining
  bDisplayErrors  - [in] write error conditions to the command line
Returns:
  A joined, capped Brep if successful. Null if it failed.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepCapPlanarHoles(
  const ON_Brep* pInputBrep,
  const double& tol,
  const bool& bDisplayErrors
);

RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepCapPlanarHolesOld(
  const ON_Brep* pInputBrep,
  const double& tol,
  const bool& bDisplayErrors
);

/*
Description:
  Derive from CRhinoReplaceColorDialog and override ColorDialog(...) to replace the standard
  Rhino color picker with a custom version.  The last CRhinoReplaceColorDialog dialog created
  will be the current replacement.
*/
#if defined (RHINO_SDK_MFC)
class RHINO_SDK_CLASS CRhinoReplaceColorDialog
{
public:
  /*
  Description:
    Constructor for color dialog replacement object.  Do not construct it until it is safe to use
    ::AfxGetStaticModuleState().
  Parameters:
    pPlugInModuleState [in] Is the module state that will be made active prior to calling ColorDialog().
                            If this is NULL ColorDialog() will never get called.  See MSDN for AFX_MANAGE_STATE
  */
  CRhinoReplaceColorDialog(AFX_MODULE_STATE* pPlugInModuleState);
  /*
  Description:
    Destructor
  */
  virtual ~CRhinoReplaceColorDialog();
  /*
  Description:
    Method which gets called by Rhino when it is ready to display a color picker dialog.
  Parameters:
    hWndParent [in] Handle to window that is the parent for the color dialog.  If this is
                    NULL the main Rhino window handle is used.
    color [in/out] Used initially as the default color for the dialog.  If ColorDialog() returns
                   true then it should be set to selected color.
    bIncludeButtonColors [in] If true then the color dialog should include standard button face and
                              button text color options, these are used by the tool bar button bitmap
                              editor.
    lpsDialogTitle [in] Dialog title
  Returns:
    Returns true if OK button was pressed in dialog or false if dialog was canceled.
  */
  virtual bool ColorDialog(HWND hWndParent, ON_Color& color, bool bIncludeButtonColors, const wchar_t* lpsDialogTitle) = 0;
  /*
  Description:
    Call this method to get the AFX_MODULE_STATE* passed to the constructor.  If this returns NULL
    then ColorDialog() will not get called.
  Returns:
    Returns the AFX_MODULE_STATE* passed to the constructor.  If this returns NULL then ColorDialog()
    will not get called.
  */
  AFX_MODULE_STATE* PlugInModuleState() const;
  /*
  Description:
    Call this method to get the color dialog replacement that was active prior to this one.
  Returns:
    Returns the color dialog replacement that was active prior to this one.
  */
  CRhinoReplaceColorDialog* PreviousColorDialog() const;
  /*
  Description:
    Call this method to get the current color dialog replacement.
  Returns:
    Returns the current color dialog replacement.
  */
  static CRhinoReplaceColorDialog* CurrentColorDialog();

private:
  AFX_MODULE_STATE* m_pPlugInModuleState;
  CRhinoReplaceColorDialog* m_pNext;
  CRhinoReplaceColorDialog* m_pPrev;
  static CRhinoReplaceColorDialog* m_pCurrent;
};
#endif

/*
Description:
  Gets a list of commands associated with a specific plug-in
Parameters:
  plug_in  - [in]  Plug-in to get command list for
  cmd_list - [out] Array to add command list to
*/
RHINO_SDK_FUNCTION
void RhinoGetPlugInCommandList(
  const class CRhinoPlugIn* plug_in,
  ON_SimpleArray<const class CRhinoCommand*>& cmd_list
);

/*
Description:
  Splits a Brep into pieces.
Parameters:
  brep       - [in] Brep to split.
  splitter   - [in] Splitter is a single possibly non-manifold brep.
  tolerance  - [in] Intersection tolerance
  pieces     - [out] Output array that contains all the split components.
               Note, the caller is responsible for destroying these objects.
  bRaisedTol - [in/out]	If not null then *bRaisedTol is set to true if the
               split failed at tolerance but succeeded when the tolerance
               was increased to twice tolerance.
Returns:
  true if successful.
*/
RHINO_SDK_FUNCTION
bool RhinoBrepSplit(
  const ON_Brep& brep,
  const ON_Brep& splitter,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& pieces,
  bool* bRaisedTol = nullptr
);

/*
Description:
   Trim a Brep B with an oriented cutter.   The parts of B that lie to inside
   ( opposite the normal) of the cutter are retained while the parts to the
   outside ( in the direction of the normal ) are discarded.  If the Cutter is
   closed, or is an ON_Plane, then a connected component of B that does not
   intersect the Cutter is kept if it is contained in the inside of Cutter.
   That is the region bounded by Cutter opposite from the normal of Cutter,
   or in the case of an ON_Plane cutter the half space opposite from the plan normal
   If Cutter is not closed all these components are kept.
Parameters:
  B -(in) Brep to trim
  Cutter-(in) cutter is either a Brep or an ON_Plane object
  tol -(in) intersection tolerance
  Trim -(out) Results are appended to Trim, user is responsible for deleting
              these objects.
  fmap - (out) Optional parameter.  fmap[i][fi] is the index of the face in B
                corresponding to Trim[i].Face(fi).

  emap - (out) Optional parameter.  emap[i][ei] is the index of the edge in B
                corresponding to Trim[i].Edge(ei) or -1 it this edge lies on
                the cutter.

Returns:
    The number, cnt,  of connected components of what is retained from B. If cnt >1
    or cnt=1 and the result is different from  B then cnt many Breps are appended to Trim.
    If cnt=0 or the object doesn't get trimmed by the cutter, in which case cnt=1, then no objects
    are appended to Trim.
Details:
    Trim, *fmap and *emap are parallel arrays so they must have the same count
    at entry and will have the same count at exit.

    The special handling of cnt==1 is provided so that the function does not make extra copies of
    Breps that are not modified by the trim operation.   This could be important for example if this
    function is applied to many Breps most of which don't actually get trimmed
*/
RHINO_SDK_FUNCTION
int RhinoBrepTrim(
  const ON_Brep& B,
  const ON_Brep& Cutter,
  double tol,
  ON_SimpleArray<ON_Brep*>& Trim,
  ON_ClassArray< ON_SimpleArray<int> >* fmap = nullptr,
  ON_ClassArray< ON_SimpleArray<int> >* emap = nullptr
);

RHINO_SDK_FUNCTION
int RhinoBrepTrim(
  const ON_Brep& B,
  const ON_Plane& Cutter,
  double tol,
  ON_SimpleArray<ON_Brep*>& Trim,
  ON_ClassArray< ON_SimpleArray<int> >* fmap = nullptr,
  ON_ClassArray< ON_SimpleArray<int> >* emap = nullptr
);

/*
Description:
  Merges planar faces.
Parameters:
  InRegions  - [in]  Array of Breps, each with a single face with a planar surface.
                     Will fail if any do not satisfy this requirement.
  plane      - [in]  The planar faces should be on or parallel to this plane.  The results
                     will be on this plane.
  tolerance  - [in]  Used to detect overlapping and near-missing edges.
  OutRegion  - [out] OutRegions will consist of the union of the projected regions,
                     each member will have a single face with a planar surface
                     based on plane.
  pbRev      - [in]  if non-null, pbRev must have the same size as InRegions.
                     if null, the array will be assumed to have all elements = false
                     if pbRev[i] = true, the complement of InRegions[i] will be used in the
                     boolean. if pbRev is non-null, the complement of the union of the
                     regions or complements will be returned.  For Union, no  array is necessary.
                     For intersection of 2 regions, bRev[0] = bRev[1] = true.
                     For A-B bRev[0] = true, bRev[1] = false
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhPlanarRegionBoolean(
  const ON_SimpleArray<const ON_Brep*>& InRegions,
  const ON_Plane& plane,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutRegions,
  ON_SimpleArray<bool>* pbRev = nullptr
);

/*
Description:
  Find Union of multiple planar faces.
Parameters:
  InRegions  - [in]  Array of Breps, each with a single face with a planar surface.
                     Will fail if any do not satisfy this requirement.
  plane      - [in] The planar faces should be on or parallel to this plane. The results will be on this plane.
  tolerance  - [in]  Used to decide if edges overlap.
  OutRegions - [out] OutRegions will consist of the union of the projected regions,
                     each member will have a single face with a planar surface based on plane.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhPlanarRegionUnion(
  const ON_Brep* B0,
  const ON_Brep* B1,
  const ON_Plane& plane,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutRegions
);

/*
Description:
  Find Union of two planar faces.
Parameters:
  B0, B1     - [in] Breps to be unioned, each with a single face with a planar surface.
  plane      - [in] The planar faces should be on or parallel to this plane. The results will be on this plane.
  tolerance  - [in]  Used to decide if edges overlap.
  OutRegions - [out] OutRegions will consist of the union of the projected regions,
                     each member will have a single face with a planar surface based on plane.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhPlanarRegionUnion(
  const ON_SimpleArray<const ON_Brep*>& InRegions,
  const ON_Plane& plane,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutRegions
);

/*
Description:
  Find intersection of two planar faces.
Parameters:
  B0, B1     - [in] Breps to be intersected, each with a single face with a planar surface.
  plane      - [in] The planar faces should be on or parallel to this plane. The results will be on this plane.
  tolerance  - [in]  Used to decide if edges overlap.
  OutRegions - [out] OutRegions will consist of the union of the projected regions,
                     each member will have a single face with a planar surface based on plane.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhPlanarRegionIntersection(
  const ON_Brep* B0,
  const ON_Brep* B1,
  const ON_Plane& plane,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutRegions
);

/*
Description:
  Find difference of two planar faces, B0 - B1.
Parameters:
  B0, B1     - [in] Breps to be subtracted, each with a single face with a planar surface.
  plane      - [in] The planar faces should be on or parallel to this plane.  The results will be on this plane.
  OutRegions - [out] OutRegions will consist of the union of the projected regions,
                     each member will have a single face with a planar surface based on plane.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhPlanarRegionDifference(
  const ON_Brep* B0,
  const ON_Brep* B1,
  const ON_Plane& plane,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutRegions
);

/*
Description:
  Trims and splits curves based on their overlapping regions.
Parameters:
  InCurves        - [in] An array of curves.
  plane           - [in] Regions will be found in the projection of the curves to this plane.
  InPoints        - [in] These points will be projected to plane. All regions that contain at least one of these points will be found.
  bAllRegions     - [in] InPoints will be ignored.  All regions will be found.
  bCombineRegions - [in] If true, then adjacent regions will be combined.
  tolerance       - [in] Function tolerance. When in doubt, use the document's model absolute tolerance.
  PlanarCurves    - [out] The region curves. Note, memory for these curves is allocated and is the responsibility of the caller.
  OutRegions      - [out] The indices and parameters for the members of PlanarCurves.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCurveBoolean(
  const ON_SimpleArray<const ON_Curve*>& InCurves,
  const ON_Plane& plane,
  const ON_SimpleArray<ON_3dPoint>& InPoints,
  bool bAllRegions,
  bool bCombineRegions,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& PlanarCurves,
  ON_ClassArray<ON_CurveRegion>& OutRegions
);

/*
Description:
  Trims and splits curves based on their overlapping regions.
Parameters:
  InCurves        - [in] An array of curves.
  plane           - [in] Regions will be found in the projection of the curves to this plane.
  InPoints        - [in] These points will be projected to plane. All regions that contain at least one of these points will be found.
  bAllRegions     - [in] InPoints will be ignored.  All regions will be found.
  bCombineRegions - [in] If true, then adjacent regions will be combined.
  tolerance       - [in] Function tolerance. When in doubt, use the document's model absolute tolerance.
  PlanarCurves    - [out] The region curves. Note, memory for these curves is allocated and is the responsibility of the caller.
  OutRegions      - [out] The indices and parameters for the members of PlanarCurves.
  PointRegionIds  - [out] Only used if bAllRegions is false. Parallel to InPoints. Index into OutRegions.
                          PointRegionIds[i] = n means InPoints[i] is contained in OutRegions[n].
                          If InPoints[i] is not in any region, then PointRegionIds[i] = -1. Also, if
                          InPoints[i] is within tolerance on any of the input curves, not just the final region boundaries,
                          then PointRegionIds[i] = -1.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCurveBoolean(
  const ON_SimpleArray<const ON_Curve*>& InCurves,
  const ON_Plane& plane,
  const ON_SimpleArray<ON_3dPoint>& InPoints,
  bool bAllRegions,
  bool bCombineRegions,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& PlanarCurves,
  ON_ClassArray<ON_CurveRegion>& OutRegions,
  ON_SimpleArray<int>& PointRegionIds
);

/*
Returns:
  True if backfaces of surface and mesh control polygons are culled.
*/
RHINO_SDK_FUNCTION
bool RhinoCullControlPolygon();

/*
Description:
  Merges adjacent coplanar faces into single faces.
Parameters:
  brep            - [in/out] The Brep.
  tolerance       - [in] 3d tolerance for determining when edges are adjacent.
                         If <= 0, then the current document tolerance is used.
  angle_tolerance - [in] 3d angle tolerance for determining when faces are parallel.
                         If <= 0, then the current document angle tolerance is used.
Returns:
  true if faces were merged, false if no faces were merged.
*/
RHINO_SDK_FUNCTION
bool RhinoMergeCoplanarFaces(
  ON_Brep* brep,
  double tolerance = ON_UNSET_VALUE,
  double angle_tolerance = ON_UNSET_VALUE
);

/*
Description:
  Merges adjacent coplanar faces into single faces.
Parameters:
  geometry        - [in/out] Geometry to modify. Breps, SubDs or meshes.
  tolerance       - [in] 3d tolerance for determining when edges are adjacent.
                         If in doubt, use document tolerance.
  angle_tolerance - [in] 3d angle tolerance for determining when faces are parallel.
                         If in doubt, use current document angle tolerance.
Returns:
  true if faces were merged, false if no faces were merged.
*/
RHINO_SDK_FUNCTION
bool RhinoMergeCoplanarFaces(
  ON_Geometry* geometry,
  double tolerance,
  double angle_tolerance
);

/*
Description:
  Merges coplanar faces adjacent to index into a single face.
Parameters:
  geometry        - [in/out] Geometry to modify. Breps, SubDs or meshes.
  face_index      - [in] face index to use as the input (use overload that doesn't have this argument
                         to merge all applicable faces of  the entire object)
  tolerance       - [in] 3d tolerance for determining when edges are adjacent.
                         If in doubt, use document tolerance.
  angle_tolerance - [in] 3d angle tolerance for determining when faces are parallel.
                         If in doubt, use current document angle tolerance.
Returns:
  true if faces were merged, false if no faces were merged.
*/
RHINO_SDK_FUNCTION
bool RhinoMergeCoplanarFaces(
  ON_Geometry* geometry,
  ON_COMPONENT_INDEX face_index,
  double tolerance,
  double angle_tolerance
);

/*
Description:
  Merges 2 coplanar, adjacent faces into a single face.
Parameters:
  geometry        - [in/out] Geometry to modify. Breps, SubDs or meshes.
  face_index1     - [in] first face index
  face_index2     - [in] second face index
  tolerance       - [in] 3d tolerance for determining when edges are adjacent.
                         If in doubt, use document tolerance.
  angle_tolerance - [in] 3d angle tolerance for determining when faces are parallel.
                         If in doubt, use current document angle tolerance.
Returns:
  true if faces were merged, false if no faces were merged.
*/
RHINO_SDK_FUNCTION
bool RhinoMergeCoplanarFaces(
  ON_Geometry* geometry,
  ON_COMPONENT_INDEX face_index1,
  ON_COMPONENT_INDEX face_index2,
  double tolerance = ON_UNSET_VALUE,
  double angle_tolerance = ON_UNSET_VALUE
);

/*
Description:
  Merges adjacent coplanar faces into single faces.
Parameters:
  brep      - [in/out] The Brep.
  tolerance - [in] 3d tolerance for determining when edges are adjacent.
                   If <= 0, then the current document tolerance is used.
Returns:
  True if faces were merged. False if no faces were merged.
Notes:
  This function will be deprecated in Rhino 7. Use RhinoMergeCoplanarFaces instead
*/
RHINO_SDK_FUNCTION
bool RhinoMergeBrepCoplanarFaces(
  ON_Brep* brep,
  double tolerance = ON_UNSET_VALUE
);

/*
Description:
  Makes an array of cubic, non-rational Beziers that fit a curve to a tolerance
Parameters:
  Curve    - [in]
  BezArray - [out] List of Beziers
  dist_tol - [in] max fitting error. Will use ON_SQRT_EPSILON as a minimum.
  kink_tol - [in] If the input curve has a g1-discontinuity with angle radian measure greater
                  than kink_tol at some point P, the list of Beziers will also have a kink at P.
Returns:
  Returns 0 if input is bogus. 1 if fit to tolerance, 2 if not fit to tolerance.
*/
RHINO_SDK_FUNCTION
int RhinoMakeCubicBeziers(
  const ON_Curve& Curve,
  ON_SimpleArray<ON_BezierCurve*>& BezArray,
  double dist_tol,
  double kink_tol
);

/*
Description:
  Change the seam of a closed trimmed surface.
Parameters:
  Face - [in] A face Brep with a closed underlying surface.
  di   - [in] the parameter direction to 0=u, 1 = v
  t    - [in] the parameter at which to place the seam.
  tol  - [in] tolerance used to cut up surface
Returns:
  a pointer to a new surface or polysurface that has the same geometry as Face
  with the relocated seam.
Notes:
  Must have the following conditions where:
    Srf = Face.SurfaceOf()
    Srf->IsClosed(di),
    Srf->Domain(di).Includes(t,true)
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoChangeSeam(
  const ON_BrepFace& Face,
  int di,
  double t,
  double tol
);

/*
Description:
  RhinoBrepGet2dProjection generates a 2D projection of a list of Breps.
Parameters:
  brepObjects            - [in] List of Brep objects to be projected.
  projectionPlane        - [in] Plane used to get 2D projection.
  projectedCurves        - [out] Curves that are in the positive part of the
                                 plane and are visible will be appended.
  pProjectedHiddenCurves - [out] If not NULL, curves that are in the
                                 positive part of the plane and are
                                 hidden will be appended.
  pDistTolerance         - [in] Tolerance used for distance comparisons.
                                Use CRhinoDoc.AbsoluteTolerance() when in doubt.
  pAngTolerance          - [in] Tolerance used for angular comparisons.
                                Use CRhinoDoc.AngleToleranceRadians() when in doubt.
Returns:
  true if success.
*/
ON_DEPRECATED_MSG(
  "This function will be removed in Rhino Version 7. \n"
  "For assistance replacing this function post on discourse.mcneel.com/c/rhino-developer.")
  RHINO_SDK_FUNCTION
  bool RhinoBrepGet2dProjection(
    unsigned int rhino_doc_sn,
    const ON_SimpleArray<const CRhinoBrepObject*>& brepObjects,
    const ON_Plane& projectionPlane,
    ON_SimpleArray<CRhinoCurveObject*>& projectedCurves,
    ON_SimpleArray<CRhinoCurveObject*>* pProjectedHiddenCurves = nullptr,
    double distTolerance = ON_UNSET_VALUE,
    double angTolerance = ON_UNSET_VALUE
  );

/*
Description:
  Generates a 2D section of a list of Breps. Splits all Breps using
  the specified Brep splitter, then generates the 2D projection of
  resulting pieces, and finally returns all output projection curves
  separated depending in curve position (back or front) and visibility.
Parameters:
  brepObjects             - [in] List of Brep objects to be sectioned.
  brepSplitter            - [in] Brep used to split objects. Splitter is a single
                                 possibly non-manifold Brep. (See RhinoBrepSplit)
  projectionPlane         - [in] Plane used to get 2D projection.
  pSectionCurves          - [out] Curve objects that lies in brepSplitter
  pForegroundCurves       - [out] Curves that are in the positive part of the
                                  splitter and are visible.
  pBackgroundCurves       - [out] Curves that are in the negative part of the
                                  splitter and are visible.
  pForegroundHiddenCurves - [out] Curves that are in the positive part
                                  of the splitter and are hidden.
  pBackgroundHiddenCurves - [out] Curves that are in the negative part
                                  of the splitter and are hidden.
  pDistTolerance          - [in] Tolerance used for distance comparisons.
                                 Use CRhinoDoc.AbsoluteTolerance() when in doubt.
  pAngTolerance           - [in] Tolerance used for angular comparisons.
                                 Use CRhinoDoc.AngleToleranceRadians() when in doubt.
Returns:
  true if success.
*/
ON_DEPRECATED_MSG(
  "This function will be removed in Rhino Version 7. \n"
  "For assistance replacing this function post on discourse.mcneel.com/c/rhino-developer.")
  RHINO_SDK_FUNCTION
  bool RhinoBrepGet2dSection(
    unsigned int rhino_doc_sn,
    const ON_SimpleArray<const CRhinoBrepObject*>& brepObjects,
    const ON_Brep& brepSplitter,
    const ON_Plane& projectionPlane,
    ON_SimpleArray<CRhinoCurveObject*>* pSectionCurves = nullptr,
    ON_SimpleArray<CRhinoCurveObject*>* pForegroundCurves = nullptr,
    ON_SimpleArray<CRhinoCurveObject*>* pBackgroundCurves = nullptr,
    ON_SimpleArray<CRhinoCurveObject*>* pForegroundHiddenCurves = nullptr,
    ON_SimpleArray<CRhinoCurveObject*>* pBackgroundHiddenCurves = nullptr,
    double distTolerance = ON_UNSET_VALUE,
    double angTolerance = ON_UNSET_VALUE
  );

ON_DEPRECATED_MSG("use overload that has an ON_ObjectArray parameter")
RHINO_SDK_FUNCTION
bool RhinoReadHatchPatternFile(
  const wchar_t* pFilename,
  ON_ClassArray<ON_HatchPattern>& hatchPatterns,
  bool bAllowDuplicates,
  bool bQuiet
);

/*
Description:
  Read hatch pattern definitions from a file.
Parameters:
  pFilename        - [in] name of existing file.
  hatchPatterns    - [in/out] hatch patterns are appended to this list.
¨ bAllowDuplicates - [in] if false, the function checks before append new patterns that no
                          other hatch pattern exists in the array with the same name.
  bQuiet           - [in] If file doesn't exist, and bQuiet is false, an error message box is shown.
Returns:
  true if success.
Notes:
  If pFilename is NULL, default hatch pattern filename is used. In this case, if file doesn't
  exist, default hatch patterns will be appended to hatchPatterns.
*/
RHINO_SDK_FUNCTION
bool RhinoReadHatchPatternFile(
  const wchar_t* pFilename,
  ON_ObjectArray<ON_HatchPattern>& hatchPatterns,
  bool bAllowDuplicates,
  bool bQuiet
);

/*
Description:
  Write hatch pattern definitions to a file.
Parameters:
  filename - [in] Name of file to create. If file exists, it will be overwritten.
  patterns - [in] Hatch patterns to be written to the file.
Returns:
  true if success, false otherwise.
Notes:
  File will be in AutoCAD .PAT format.
*/
RHINO_SDK_FUNCTION
bool RhinoWriteHatchPatternFile(
  const wchar_t* filename,
  const ON_ObjectArray<ON_HatchPattern>& patterns
);

/*
Description:
  Compare 2 curves to see if the geometry is the same.
Parameters:
  Pointers to the curves to compare
Returns:
  1 or -1 if the curves are different
  0 if they are the same
Notes:
  This is a fast compare function that can be used for sorting
*/
RHINO_SDK_FUNCTION
int Rhino_dup_cmp_curve(const ON_Curve* crva, const ON_Curve* crvb);

/*
Description:
  Compare 2 surfaces to see if the geometry is the same.
Parameters:
  Pointers to the surfaces to compare
Returns:
  1 or -1 if the surfaces are different
  0 if they are the same
Notes:
  This is a fast compare function that can be used for sorting
*/
RHINO_SDK_FUNCTION
int Rhino_dup_cmp_surface(const ON_Surface* srfa, const ON_Surface* srfb);

/*
Description:
  Combines two or more Breps into one.
Parameters:
  InBreps    - [in] the Breps to merge.
  tolerance  - [in] the tolerance to use when merging.
  MergedBrep - [out] the resulting Brep.
Returns:
  true if success.
  false if unsuccessful.
Notes:
  Faces are split by intersection curves.
*/
RHINO_SDK_FUNCTION
bool RhinoMergeBreps(
  const ON_SimpleArray<const ON_Brep*>& InBreps,
  double tolerance,
  ON_Brep& MergedBrep
);

/*
Description:
  Fits a new curve through an existing curve.
Parameters:
  curve_in  - [in] the curve to fit.
  degree    - [in] the output curve degree. Must be bigger than 1.
  dFitTol   - [in] the fitting tolerance.
                   If dFitTol is ON_UNSET_VALUE or <=0.0, the document absolute tolerance is used.
  dAngleTol - [in] the kink smoothing tolerance in radians.
                   If dAngleTol is 0.0, all kinks are smoothed.
                   If dAngleTol is >0.0, kinks smaller than dAngleTol are smoothed.
                   If dAngleTol is ON_UNSET_VALUE or <0.0, the document angle tolerance is used for the kink smoothing.
  curve_out - [in] if not NULL this curve will be used for output.
Returns:
  pointer to curve if success.
  NULL if unsuccessful.
Notes:
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoFitCurve(
  const ON_Curve& curve_in,
  int degree = 3,
  double dFitTol = ON_UNSET_VALUE,
  double dAngleTol = ON_UNSET_VALUE,
  ON_NurbsCurve* curve_out = nullptr
);

/*
Description:
  Fits a new surface through an existing surface.
Parameters:
  surface_in   - [in] the surface to fit.
  udegree      - [in] the output surface U degree. Must be bigger than 1.
  vdegree      - [in] the output surface V degree. Must be bigger than 1.
  dFitTol      - [in] the fitting tolerance.
                      If dFitTol is ON_UNSET_VALUE or <=0.0, the document absolute tolerance is used.
  pAchievedTol - [out] if not NULL this will be used for the achieved tolerance. pAchievedTol is always <= dFitTol.
Returns:
  pointer to surface if success.
  NULL if unsuccessful.
Notes:
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoFitSurface(
  const ON_Surface& surface_in,
  int udegree = 3,
  int vdegree = 3,
  double dFitTol = ON_UNSET_VALUE,
  double* pAchievedTol = nullptr
);

/*
Description:
  Gets parameter intervals for the active part of Brep face.
Parameters:
  face         - [in] the Brep face to evaluate.
  iso_dir      - [in] 0 = intervals are "u" intervals, c = constant "v" value
                      1 = intervals are "v" intervals, c = constant "u" value
  iso_constant - [in] constant "c" above
  intervals    - [out] intervals are appended to this array.
Returns:
  Returns number of intervals that are appended to array.
See Also:
  RhinoGetIsoCurves
Remarks:
  The isoparametric curve(s) exist on the domain of the intervals.
*/
RHINO_SDK_FUNCTION
int RhinoGetIsoIntervals(
  const ON_BrepFace& face,
  int iso_dir,
  double iso_constant,
  ON_SimpleArray<ON_Interval>& intervals
);

/*
Description:
  Gets isoparametric curve(s) from a Brep face.
Parameters:
  face         - [in] the Brep face to evaluate.
  iso_dir      - [in] 0 = intervals are "u" intervals, c = constant "v" value
                      1 = intervals are "v" intervals, c = constant "u" value
  iso_constant - [in] constant "c" above
  curves       - [out] curves are appended to this array.
Returns:
  Returns number of curves that are appended to array.
See Also:
  RhinoGetIsoIntervals
Remarks:
  The caller is responsible for deleting these curves when finished.
*/
RHINO_SDK_FUNCTION
int RhinoGetIsoCurves(
  const ON_BrepFace& face,
  int iso_dir,
  double iso_constant,
  ON_SimpleArray<ON_Curve*>& curves
);

/*
Description:
  Rebuilds an existing curve with a new curve to given point count.
Parameters:
  curve_in      - [in] the curve to rebuild.
  degree        - [in] the output curve degree.
  pointcount    - [in] the number of points in the output curve.
                       Must be bigger than the degree.
                       With closed curves the minimum point count is three.
                       The highest accepted point count is 1000.
  bKeepTangents - [in] if set, maintain start and end tangent directions
                       The minimum number of points to do this is degree + 2
  curve_out     - [in] if not NULL this curve will be used for output.
Returns:
  pointer to curve if success.
  NULL if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoRebuildCurve(
  const ON_Curve& curve_in,
  int degree,
  int pointcount,
  bool bKeepTangents,
  ON_NurbsCurve* curve_out = nullptr
);

/*
Description:
  Rebuilds an existing curve with a new curve to given point count.
Parameters:
  curve_in   - [in] the curve to rebuild.
  degree     - [in] the output curve degree.
  pointcount - [in] the number of points in the output curve.
                    Must be bigger than the degree.
                    With closed curves the minimum point count is three.
                    The highest accepted point count is 1000.
  curve_out  - [in] if not NULL this curve will be used for output.
Returns:
  pointer to curve if success.
  NULL if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoRebuildCurve(
  const ON_Curve& curve_in,
  int degree = 3,
  int pointcount = 10,
  ON_NurbsCurve* curve_out = nullptr
);

/*
Description:
  Rebuilds an existing surface with a new surface to a given point count.
Parameters:
  surface_in  - [in] the surface to rebuild.
  udegree     - [in] the output surface u degree.
  vdegree     - [in] the output surface v degree.
  upointcount - [in] the number of points in the output surface u direction.
  vpointcount - [in] the number of points in the output surface v direction.
                     Must be bigger than the degree.
                     The highest accepted point count is 1000.
  surface_out - [in] if not NULL this surface will be used for output.
Returns:
  pointer to surface if success.
  NULL if unsuccessful.
Notes:
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoRebuildSurface(
  const ON_Surface& surface_in,
  int udegree = 3,
  int vdegree = 3,
  int upointcount = 10,
  int vpointcount = 10,
  ON_NurbsSurface* surface_out = nullptr
);

/*
Description:
  Rebuilds an existing surface with a new surface to a given point count
  in either the u or v directions independently.
Parameters:
  surface_in  - [in] the surface to rebuild.
  direction   - [in] the direction (0 = U, 1 = V).
  point_count - [in] the number of points in the output surface in the "direction" direction.
  loft_type   - [in] the loft type (0 = Normal, 1 = Loose, 2 = Tight, 3 = Straight, 4 = Uniform).
  tolerance   - [in] the refit tolerance. When in doubt, use the document's model absolute tolerance.
  surface_out - [in] if not nullptr this surface will be used for output.
Returns:
  pointer to surface if success.
  nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoRebuildSurfaceOneDirection(
  const ON_Surface& surface_in,
  int direction,
  int point_count,
  int loft_type,
  double tolerance,
  ON_NurbsSurface* surface_out = nullptr
);

/*
Description:
  Transfers trims from a Brep face to another surface roughly the same shape and in the same location
  as the original face, and returns the result in a single surface Brep.
  This is useful for re-trimming a trimmed surface after rebuild or refit.
Parameters:
  trim_source - [in] the original face to transfer the trims from.
  trim_target - [in] the surface to transfer the trims to.
  tol         - [in] tolerance used to rebuild edges.
Returns:
  Pointer to single face Brep if success. The target surface is duplicated.
  nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoRetrimSurface(
  const ON_BrepFace& trim_source,
  const ON_Surface& trim_target,
  double tol
);

/*
Description:
  Finds split points on a curve using a cutter Brep
Parameters:
  curve      - [in] curve to find the split points on.
  cutter     - [in] Brep to split curve with.
  tolerance  - [in] intersection tolerance
  angtolrads - [in] angle tolerance in radians
  tsplit     - [out] array of split parameters on curve
Returns:
  True if successful
Notes:
*/
RHINO_SDK_FUNCTION
bool RhinoGetSplitPoints(
  const ON_Curve& curve,
  const ON_Brep& cutter,
  double tolerance,
  double angtolrads,
  ON_SimpleArray<double>& tsplit
);

/*
Description:
  Splits a face into pieces using 3d trimming curves.
Parameters:
  brep         - [in] the base Brep.
  face_index   - [in] index of face in m_F[] array to split.
  curves       - [in] array of 3d curves used for trimming.
  tolerance    - [in] fitting and intersecting tolerance.  When in doubt,
                      use the document's absolute tolerance.
  bCopySurface - [in] if TRUE and the face is split, then the
                      surface(s) used by the new face(s) will be copied.
Returns
  The resulting Brep or nullptr.
  Note, memory for the Brep is allocated and becomes the responsibility of the caller.
Remarks:
  If the face is split, the portion of the face to the left of the
  trim at the start of the trim remains in m_F[face_index].  Any
  additional faces are appended to the m_F[] array.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoSplitBrepFace(
  const ON_Brep& brep,
  int face_index,
  const ON_SimpleArray<const ON_Curve*>& curves,
  double tolerance,
  bool bCopySurface = false
);

/*
Description:
  Splits a Brep into pieces using Breps as cutters
Parameters:
  brep      - [in] The Brep to split.
  cutters   - [in] An array of one or more Breps to use as cutters.
  tolerance - [in] The intersection tolerance
  pieces    - [out] Output array that contains the split components.
Returns:
  The number of Breps added to the output array.
Remarks:
  NOTE, memory for the resulting Breps is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
int RhinoBrepSplit(
  const ON_Brep& brep,
  const ON_SimpleArray<const ON_Brep*>& cutters,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& pieces
);

/*
Description:
  Splits a Brep into pieces using curves (at least partially) on the Brep
Parameters:
  brep      - [in] The Brep to split.
  cutters   - [in] An array of curves. Only the portion of the curve on the Brep surface will be used for cutting.
  tolerance - [in] The intersection tolerance
  pieces    - [out] Output array that contains the split components.
Returns:
  The number of Breps added to the output array.
Remarks:
  This is one of the splitting functions used to split Breps in the Rhino Split command.
  NOTE, memory for the resulting Breps is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
int RhinoBrepSplit(
  const ON_Brep& brep,
  const ON_SimpleArray<const ON_Curve*>& cutters,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& pieces
);

/*
Description:
  Splits a Brep into pieces using a combination of curves to be extruded and Breps.
Parameters:
  brep         - [in] The Brep to split.
  cutters      - [in] An array of curves, surfaces, faces and Breps, any other geometry is ignored.
  CPlaneNormal - Construction Plane Normal and PlanView  are used in deciding how to extrude a curve into a cutter ( see Remark below)
  PlanView     - true if this view is plan.
  tolerance    - [in] The intersection tolerance
  pieces       - [out] Output array that contains the split components.
Returns:
  The number of Breps added to the output array.
Remarks:
  This is one of the splitting functions used to split Breps in the Rhino Split command.

  Extrusion of Cutting Curves
    A Curve in cutters is extruded to produce a surface to use as a cutter.
    The extrusion direction is choose as in the Rhino Split command based on the properties of the active view.
    In particular the Construction Plane Normal and whether the active view is a PlanView ( A parallel projection
    with construction plane normal as the view direction).

    If PlanView is false and the curve is planar then the curve is extruded perpendicular to the curve,
    otherwise the curve is extruded in the CPlaneNormal direction.

  NOTE, memory for the resulting Breps is allocated and becomes
  the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
int RhinoBrepSplit(
  const ON_Brep& brep,
  const ON_SimpleArray<const ON_Geometry*>& cutters,
  ON_3dVector CPlaneNormal,
  bool PlanView,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& pieces
);

/*
Description:
  Get relatively parallel frames at selected points along a curve.
Parameters:
  curve       - [in] the input curve
  count       - [in] number of values in t[] array
  t           - [in] increasing array of curve parameters
  frames      - [out] frames are appended to this array.  The plane's
                      origin is on the curve and the z-axis is the curve's tangent.
  starting_x0 - [in] optional vector that specifies the x direction
                     to use for the plane at t[0].  If starting_x0 is ON_unset_point,
                     then the curve's curvature vector at t[0] is used.
*/
RHINO_SDK_FUNCTION
bool RhinoGet1RailFrames(
  const ON_Curve& curve,
  int count,
  const double* t,
  ON_SimpleArray<ON_Plane>& frames,
  ON_3dVector starting_x0
);

/*
Description:
  Calculates the boolean union of two or more closed, planar curves.
  Note, curves must be co-planar.
Parameters:
  InCurves  - [in]  The curves co-planar curves to union.
  tolerance - [in]  3d fitting and intersecting tolerance.
  OutCurves - [out] The results.
                    NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE CURVES.
Returns:
  The number of curves added to OutCurves if successful.
  0 if not successful.
See Also:
  RhinoCurveBooleanIntersection
  RhinoCurveBooleanDifference
*/
RHINO_SDK_FUNCTION
int RhinoCurveBooleanUnion(
  const ON_SimpleArray<const ON_Curve*>& InCurves,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& OutCurves
);

/*
Description:
  Calculates the boolean intersection of two closed, planar curves.
  Note, curves must be co-planar.
Parameters:
  crvA      - [in]  The first closed, planar curve.
  crvB      - [in]  The second closed, planar curve.
  tolerance - [in]  3d fitting and intersecting tolerance.
  OutCurves - [out] The results.
                    NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE CURVES.
Returns:
  The number of curves added to OutCurves if successful.
  0 if not successful.
See Also:
  RhinoCurveBooleanUnion
  RhinoCurveBooleanDifference
*/
RHINO_SDK_FUNCTION
int RhinoCurveBooleanIntersection(
  const ON_Curve* crvA,
  const ON_Curve* crvB,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& OutCurves
);

/*
Description:
  Calculates the boolean intersection of multiple closed, planar curves.
  Note, curves must be co-planar.
Parameters:
  InCurves  - [in]  The curves co-planar curves to union.
  tolerance - [in]  3d fitting and intersecting tolerance.
  OutCurves - [out] The results. May be several or none.
                    NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE CURVES.
Returns:
  true if successful, false otherwise.
See Also:
  RhinoCurveBooleanUnion
  RhinoCurveBooleanDifference
*/
RHINO_SDK_FUNCTION
bool RhinoCurveBooleanIntersection(
  const ON_SimpleArray<const ON_Curve*>& InCurves,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& OutCurves
);

/*
Description:
  Calculates the boolean difference between two closed, planar curves.
  Note, curves must be co-planar.
Parameters:
  crvA      - [in]  The first closed, planar curve.
  crvB      - [in]  The second closed, planar curve.
  tolerance - [in]  3d fitting and intersecting tolerance.
  OutCurves - [out] The results.
                    NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE CURVES.
Returns:
  The number of curves added to OutCurves if successful.
  0 if not successful.
See Also:
  RhinoCurveBooleanUnion
  RhinoCurveBooleanIntersection
*/
RHINO_SDK_FUNCTION
int RhinoCurveBooleanDifference(
  const ON_Curve* crvA,
  const ON_Curve* crvB,
  double tolerance,
  ON_SimpleArray<ON_Curve*>& OutCurves
);

RHINO_SDK_FUNCTION
int RhinoCurveBooleanDifference(
  const ON_Curve* crvA,
  const ON_SimpleArray<const ON_Curve*>& crvB,
  double cbtol,
  ON_SimpleArray<ON_Curve*>& OutCurves
);

/*
Description:
  Calculates 3d points on a curve where the linear distance between the points is equal.
Parameters:
  curve    - [in] The input curve.
  distance - [in] The distance between division points.
  points   - [out] The results of the division.
Returns:
  The number of points added to the output array if successful.
  0 if not successful.
See Also:
  RhinoDivideCurve
Remarks:
  Unlike the RhinoDivideCurve SDK function, which divides a curve based on arc length,
  or the distance along the curve between two points, this function divides a curve
  based on the linear distance between points.
*/
RHINO_SDK_FUNCTION
int RhinoDivideCurveEquidistant(
  const ON_Curve* curve,
  double distance,
  ON_SimpleArray<ON_3dPoint>& points
);

/*
Description:
  Calculates 3d points on a curve where the linear distance between the points is equal.
Parameters:
  curve      - [in] The input curve.
  distance   - [in] The distance between division points.
  points     - [out] The results of the division.
  parameters - [out] The curve parameters at the point locations.
Returns:
  The number of points added to the output array if successful.
  0 if not successful.
See Also:
  RhinoDivideCurve
Remarks:
  Unlike the RhinoDivideCurve SDK function, which divides a curve based on arc length,
  or the distance along the curve between two points, this function divides a curve
  based on the linear distance between points.
*/
RHINO_SDK_FUNCTION
int RhinoDivideCurveEquidistant(
  const ON_Curve* curve,
  double distance,
  ON_SimpleArray<ON_3dPoint>& points,
  ON_SimpleArray<double>* parameters
);


/*
Description:
  Projects curves onto meshes
Parameters:
  Meshes       - [in]  The meshes to project onto
  Curves       - [in]  The curves to project
  ProjDir      - [in]  The direction to project
  OutCurves    - [out] The output, or projected, curves.
                       NOTE: THE CALLER IS RESPONSIBLE FOR DESTROYING THESE CURVES.
  CurveIndices - [out] OutCurves[i] is a projection of Curves[CurveIndices[i]]
  MeshIndices  - [out] OutCurves[i] was projected onto Meshes[MeshIndices[i]]
  tolerance    - [in]  The projection tolerance.
Returns:
  true if at least one of the input curves was projected, otherwise false.
See Also:
  RhinoProjectCurvesToBreps
  RhinoProjectPointsToBreps
  RhinoProjectPointsToMeshes
*/
RHINO_SDK_FUNCTION
bool RhinoProjectCurvesToMeshes(
  ON_SimpleArray<const ON_Mesh*>& Meshes,
  ON_SimpleArray<const ON_Curve*>& Curves,
  const ON_3dVector& ProjDir,
  ON_SimpleArray<ON_Curve*>& OutCurves,
  ON_SimpleArray<int>& CurveIndices,
  ON_SimpleArray<int>& MeshIndices,
  double tolerance
);

/*
Description:
  Projects points onto meshes
Parameters:
  Meshes    - [in]  The meshes to project onto
  Points    - [in]  The points to project
  ProjDir   - [in]  The direction to project
  OutPoints - [out] The output, or projected, points.
  Indices   - [out] OutPoints[i] is a projection of Points[Indices[i]]
  tolerance - [in]  The projection tolerance - used for culling close
                    points and for line-mesh intersection.
Returns:
  true if at least one of the input points was projected, otherwise false.
See Also:
  RhinoProjectCurvesToBreps
  RhinoProjectCurvesToMeshes
  RhinoProjectPointsToBreps
*/
RHINO_SDK_FUNCTION
bool RhinoProjectPointsToMeshes(
  ON_SimpleArray<const ON_Mesh*>& Meshes,
  const ON_3dPointArray& Points,
  const ON_3dVector& ProjDir,
  ON_3dPointArray& OutPoints,
  ON_SimpleArray<int>& Indices,
  double tolerance
);

/*
Description:
  Fill in a 2d region with triangles.
Parameters:
  point_count  - [in] number of 2d points.
  point_stride - [in] i-th point = (point[j],point[j+1]), where
                      j = i*point_stride.
  point        - [in] 2d point locations. It is OK to include points that are inside the
                      region but not at the end of an edge.  Duplicate points are not permitted.
  edge_count   - [in] number of edges (if 0, then the input list of point
                      is treated as a counterclockwise closed polyline.
  edge_stride  - [in] i-th edge connects points (edge[j],edge[j+1]) where
                      j = i*edge_stride.
  edge         - [in] indices of edge ends.  Edges can intersect only at shared end points.
  edge_side    - [in] if NULL, the triangles are built on the left side
                      of the edges.  If not NULL, then
                      edge[i] determines which side(s) of the edge need
                      triangles.  1 = left side only, 2 = right side only, 3 = both sides
  triangles    - [out] triangles are appended to this list.  The (i,j,k) are vertex indices.
Returns:
  Number of triangles appended to triangles[] array.
*/
RHINO_SDK_FUNCTION
int RhinoTriangulate2dRegion(
  int point_count,
  int point_stride,
  const double* point,
  int edge_count,
  int edge_stride,
  const int* edge,
  const int* edge_side,
  ON_SimpleArray<ON_3dex>& triangles
);

/*
Description:
  Create a triangulated mesh from the given input.  This is a higher level function that ultimately calls
  RhinoTriangulate2dRegion above.  Any polyline input will become edges as described above and the edge_side will
  depend on the input variety, i.e. InputInnerBoundaryCurves will have edge_sides of 1, pInputOuterBoundaryCurves
  will have edge_sides of 2 and InputInnerBothsideCurves will have edge_sides of 3.
Parameters:
  InputInnerPoints         - [in] Points to be used to generate the mesh.
                                  Note: If pInputOuterBoundaryCurve is not null, points outside of that boundary
                                  after it has been pulled to pInputPullBackSurface (or the best plane through the
                                  input if pInputPullBackSurface is null) will be ignored.
                                  Optional, can be empty.
  InputInnerBoundaryCurves - [in] These polylines will create holes in the output mesh.
                                  Note: If InputInnerBoundaryCurves are the only input then the result may be null
                                  if bTrimback is set to false (see comments for bTrimback) because the resulting
                                  mesh could be invalid (all faces created contained vertices from the perimeter
                                  boundary).
                                  Optional, can be empty.
  InputInnerBothsideCurves - [in] These polylines will create faces on both sides of the edge.
                                  Note: If there are only input points (InputInnerPoints) there is no way to
                                  guarantee a triangulation that will create an edge between two particular points.
                                  Adding a line, or polyline, to InputInnerBothsideCurves that includes points from
                                  InputInnerPoints will help guide the triangulation.
                                  Optional, can be empty.
  pInputOuterBoundaryCurve - [in] Outer boundary polyline, if provided this will become the outer boundary of the
                                  resulting mesh.
                                  Note: Any of the above input that is completely outside the outer boundary will be
                                  ignored and have no impact on the result.  If any of the above input intersects the
                                  outer boundary the result will be unpredictable and is likely to not include the
                                  entire outer boundary.
                                  Optional, can be null.
  pInputPullBackSurface    - [in] Initial surface where, if provided, 3d input will be pulled to make a 2d
                                  representation used by the function that generates the mesh.  Providing a
                                  pInputPullBackSurface can be helpful when it is similar in shape to the pattern of
                                  the input, the pulled 2d points will be a better representation of the 3d points.
                                  If all of the input is more or less coplanar to start with providing
                                  pInputPullBackSurface has no real benefit.
                                  Optional, can be null.
                                  If not provided the best plane through all of the given input is used.
  bTrimback                - [in] bTrimback is only used when a pInputOuterBoundaryCurve has not been provided.
                                  When that is the case, the function uses the perimeter of the surface as the outer
                                  boundary instead.  If bTrimback is true, any face of the resulting triangulated mesh
                                  that contains a vertex of the perimeter boundary will be removed.
                                  Optional, the default is true.
  divisions                - [in] divisions is also only used when a pInputOuterBoundaryCurve has not been provided.
                                  When that is the case, division becomes the number of divisions each side of the
                                  surface's perimeter will be divided into to create an outer boundary to work with.
                                  Optional, the default is 10.
Returns:
  The result will be a mesh that has the given input as vertices and faces that are the triangulation of those vertices.
  Null will be returned on error, such as would be the case if no valid input was given being that all arguments are
  optional.
Remarks:
  The calling function should check that the returned ON_Mesh pointer is not null and is responsible for destroying the
  returned ON_Mesh.  Edges that intersect (polyline inputs that intersect) are not allowed.  They will generate an error
  in the function used to triangulate the input.  A result may be returned but not all input will have been used to
  generate the triangulation.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoGetTriangulation(
  const ON_SimpleArray<ON_3dPoint>& InputInnerPoints,
  const ON_SimpleArray<const ON_PolylineCurve*>& InputInnerBoundaryCurves,
  const ON_SimpleArray<const ON_PolylineCurve*>& InputInnerBothsideCurves,
  const ON_PolylineCurve* pInputOuterBoundaryCurve = nullptr,
  const ON_Surface* pInputPullBackSurface = nullptr,
  bool bTrimback = true,
  int divisions = 10
);

/*
Description:
  Merges two surfaces into one surface at untrimmed edges.
  Both surfaces must be untrimmed and share an edge.
Parameters:
  in_brep0    - [in] The first single-face Brep to merge.
  in_brep1    - [in] The second single-face Brep to merge.
  tolerance   - [in] Surface edges must be within this tolerance for the two surfaces to merge.
  angtolrads  - [in] Edge must be within this angle tolerance in order for contiguous
                     edges to be combined into a single edge.
  pick_point0 - [in] 2-D pick point on the first single-face Brep (optional).
  pick_point1 - [in] 2-D pick point on the second single-face Brep (optional).
  roundness   - [in] Defines the roundness of the merge. The default is 1.0 (full
                     smoothing). Acceptable values are between 0.0 (sharp) and 1.0 (smooth).
  smooth      - [in] The surface will be smooth. This makes the surface behave better
                     for control point editing, but may alter the shape of both surfaces.
Returns:
  The merged Brep is successful, 0 if not successful.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE RETURNED BREP.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoMergeSrf(
  const ON_Brep* in_brep0,
  const ON_Brep* in_brep1,
  double tolerance,
  double angtolrads,
  const ON_2dPoint* pick_point0 = nullptr,
  const ON_2dPoint* pick_point1 = nullptr,
  double roundness = 1.0,
  bool smooth = true
);

/*
Description:
  Creates closed polysurfaces from surfaces and polysurfaces
  that bound a region in space.
Parameters:
  InBreps   - [in]  The intersecting surfaces and polysurfaces to automatically
                    trim and join into closed polysurfaces.
  OutBreps  - [out] The resulting polysurfaces.
  tolerance - [in]  The trim and join tolerance.
Returns:
  The results of the operation, where:
    0 - Success.
    1 - InBreps has no non-NULL Breps.
    2 - Merge failed.
    3 - All faces removed.
    4 - Non-manifold after fins removed.
    5 - Could not determine orientation.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE RETURNED BREPS.
*/
RHINO_SDK_FUNCTION
int RhinoCreateSolid(
  const ON_SimpleArray<const ON_Brep*>& InBreps,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  double tolerance
);

/*
Description:
  Splits a curve into pieces using a surface or a polysurface
Parameters:
  curve      - [in] Curve to split.
  splitter   - [in] Splitter is a single, non-manifold Brep.
  tolerance  - [in] intersection tolerance
  angtolrads - [in] angle tolerance in radians
  pieces     - [out] Curves of all the split components.
Returns:
  true if successful, false otherwise.
Remarks:
  The caller is responsible for deleting the pieces when finished.
*/
RHINO_SDK_FUNCTION
bool RhinoCurveSplit(
  const ON_Curve& curve,
  const ON_Brep& splitter,
  double tolerance,
  double angtolrads,
  ON_SimpleArray<ON_Curve*>& pieces
);

/*
Description:
  Finds the object, and the closest point in that object, that is
  closest to a given test curve. Allowable objects to test the curve
  against include ON_Brep, ON_Surface, ON_Curve, and ON_PointCloud objects.
Parameters:
  crv        - [in]  The curve to test.
  t_param    - [out] The parameter of closest point on curve.
  geom       - [in]  The geometry to test the curve against.
  obj_index  - [out] index of the geometry object containing the closest point.
  comp_index - [out] The component_index (see remarks below).
  u_param    - [out] The u parameter of local closest point (see remarks below).
  v_param    - [out] The v parameter of local closest point (see remarks below).
  MaxDist    - [in]  The maximum allowable distance.
Returns:
  true if successful, false otherwise.
Remarks:
  The geom array can only contain ON_Brep, ON_Surface, ON_Curve,
    and ON_PointCloud pointers.
  If geom[obj_index] is a Brep or face, comp_index is valid.
    Otherwise it is undefined.
  If geom[obj_index] is a curve, u_param is the curve parameter,
    and v_param is undefined.
*/
RHINO_SDK_FUNCTION
bool RhinoGetClosestPoint(
  const ON_NurbsCurve& crv,
  double& t_param,
  const ON_SimpleArray<const ON_Geometry*>& geom,
  int& obj_index,
  ON_COMPONENT_INDEX& comp_index,
  double& u_param,
  double& v_param,
  double MaxDist = 0.0
);

/*
Description:
  Creates one or more planar meshes from one or more planar closed curves.
Parameters:
  curves - [in]  The planar closed curves.
  mp     - [in]  Mesh parameters to be used in the creation of the meshes
  meshes - [out] The resulting planar meshes.
  tol    - [in]  tolerance used, use document tolerance if you're unsure
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoMakePlanarMeshes(
  ON_SimpleArray<const ON_Curve*>& curves,
  const ON_MeshParameters& mp,
  ON_SimpleArray<ON_Mesh*>& meshes,
  double tol
);

/*
Description:
  Pulls one or more points to a Brep face.
Parameters:
  face      - [in]  The Brep face that pulls.
  InPoints  - [in]  The points to pull.
  OutPoints - [out] The results of the pull.
  tolerance - [in]  The tolerance.
Returns:
  The number of points added to OutPoints if successful, 0 otherwise.
*/
RHINO_SDK_FUNCTION
int RhinoPullPointsToFace(
  const ON_BrepFace& face,
  const ON_SimpleArray<ON_3dPoint>& InPoints,
  ON_SimpleArray<ON_3dPoint>& OutPoints,
  double tolerance
);

/*
Description:
  Pulls a curve to a Brep face.
Parameters:
  face      - [in]  The Brep face that pulls.
  curve     - [in]  The curve to pull.
  OutCurves - [out] The results of the pull.
  tolerance - [in]  The tolerance.
Returns:
  The number of curves added to OutCurves if successful, 0 otherwise.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVES.
*/
RHINO_SDK_FUNCTION
int RhinoPullCurveToFace(
  const ON_BrepFace& face,
  const ON_Curve& curve,
  ON_SimpleArray<ON_Curve*>& OutCurves,
  double tolerance
);

/*
Description:
  Curve segment duplicator (exploder).
  Explodes polylines, polycurves and G1 discontinuous NURBS curves.
Parameters:
  curve     - [in]  The curve whose segments you want duplicated.
  OutCurves - [out] The curve segments that were duplicated.
Returns:
  The number of curves added to OutCurves if successful, 0 otherwise.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVES.
*/
RHINO_SDK_FUNCTION
int RhinoDuplicateCurveSegments(
  const ON_Curve* curve,
  ON_SimpleArray<ON_Curve*>& OutCurves
);

/*
Description:
  Create constant radius rolling ball fillets between two faces.
Parameters:
  FaceA -  [in] First face.
  bFlipA - [in] If true, offset FaceA in direction opposite the surface normal.
                Note that FaceA.m_bRev is not taken into consideration.
  FaceB -  [in] Second face.
  bFlipB - [in] If true, offset FaceB in direction opposite the surface normal.
                Note that FaceB.m_bRev is not taken into consideration.
  radius - [in] Radius of fillet.  If negative, absolute value will be used.
  tol -    [in] Tolerance used for approximating and intersecting offset srfs
  Fillets -[out] Fillet surfaces.  Results will be appended without emptying first.
Returns:
  true if all fillets were created.  False otherwise.  If Fillets.Count() is larger on
  exit than on entrance, and the return is false, then some but not all fillets were created.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT SURFACE.
*/
RHINO_SDK_FUNCTION
bool RhinoSimpleRollingBallFillet(
  const ON_BrepFace& FaceA,
  bool bFlipA,
  const ON_BrepFace& FaceB,
  bool bFlipB,
  double radius,//positive
  double tol,
  ON_SimpleArray<ON_Surface*>& Fillets
);

/*
Description:
  Create constant radius rolling ball fillets between two faces.
Parameters:
  FaceA -  [in] First face.
  uvA -    [in] A point in the parameter space of FaceA near where the
                fillet is expected to hit the surface.
  FaceB -  [in] Second face.
  uvA -    [in] A point in the parameter space of FaceA near where the
                fillet is expected to hit the surface.
  radius - [in] Radius of fillet.  If negative, absolute value will be used.
  tol -    [in] Tolerance used for approximating and intersecting offset srfs
  Fillets -[out] Fillet surfaces.  Results will be appended without emptying first.
Returns:
  true if all fillets were created.  False otherwise.  If Fillets.Count() is larger on
  exit than on entrance, and the return is false, then some but not all fillets were created.
Remarks:
  uvA and uvB determine the directions of the offsets.  The surfaces will be offset towards
  the middle of the segment from FaceA(uvA) to FaceB(uvB).
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT SURFACE.
*/
RHINO_SDK_FUNCTION
bool RhinoSimpleRollingBallFillet(
  const ON_BrepFace& FaceA,
  const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB,
  const ON_2dPoint& uvB,
  double radius,//positive
  double tol,
  ON_SimpleArray<ON_Surface*>& Fillets
);

/*
Description:
  Asks a question about whether to change the size of the geometry or the unit system
  for use when importing a file with a unit system different than the current model
Parameters:
   current      - [in] Unit system in the current model
   file         - [in] Unit system in the file being imported
   bInteractive - [in] true: use a dialog, false: use command line
Returns:
  IDYES:    Scale the geometry
  IDNO:     Don't scale the geometry
  IDCANCEL: No answer was provided - user canceled
Remarks:
*/
RHINO_SDK_FUNCTION
int RhinoQueryImportScaleGeometry(
  ON::LengthUnitSystem current,
  ON::LengthUnitSystem file,
  bool bInteractive
);

/*
Description:
  Offsets a Brep face and returns the offset in a Brep.
Parameters:
  face            - [in] The Brep face to offset.
  dOffsetDistance - [in] The offset distance.
  dTolerance      - [in] The offset tolerance. Use 0.0 to make a loose offset.
                         Otherwise, the document's absolute tolerance is usually sufficient.
  bBothSides      - [in] When true, offset to both sides of the input face.
  bSolid          - [in] When true, make a solid object.
  pAchievedTol    - [out] The achieved offset tolerance.
Returns:
  A pointer to a new Brep if success. The Brep can be disjoint if bBothSides is true and
  bSolid is false, or if bSolid is true and connecting the offsets with side surfaces fails.
  NULL if unsuccessful.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT BREP.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoOffsetSurface(
  const ON_BrepFace& face,
  const double dOffsetDistance,
  const double dTolerance = 0.0,
  const bool bBothSides = false,
  const bool bSolid = false,
  double* pAchievedTol = nullptr
);

/*
Description:
  Creates a single anaglyph image from the two passed in images.
Parameters:
  LeftEye  - [in] The image that represents pixels rendered from the "left" eye/camera
  RightEye - [in] The image that represents pixels rendered from the "right" eye/camera
  nType    - [in] The type of anaglyph being requested... acceptable values are:
                   0 = red/cyan
                   1 = red/blue
                   2 = red/green
                   3 = amber/blue
Returns:
  A pointer to a new CRhinoDib object containing the anaglyph. NULL if the
  required criteria are not met (see remarks below).
Remarks:
  Both left and right images MUST have the same dimensions
  The resulting anaglyph will have the same dimensions as the left and right images.
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CRhinoDib.
*/
RHINO_SDK_FUNCTION
CRhinoDib* RhinoCreateAnaglyph(
  const CRhinoDib& LeftEye,
  const CRhinoDib& RightEye,
  const int nType = 0
);

/*
Description:
  Creates a mean, or average, curve from two curves.
Parameters:
  pC0             - [in] The first curve.
  pC1             - [in] The second curve.
  angle_tolerance - [in] The angle tolerance, in radians, used to match kinks between curves.
                         If you are unsure how to set this parameter, then either use the
                         document's angle tolerance (CRhinoDoc::AngleToleranceRadians(),
                         or the default value.
Returns:
  A pointer to a new curve if success.
  NULL if unsuccessful.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoMeanCurve(
  const ON_Curve* pC0,
  const ON_Curve* pC1,
  double angle_tolerance = ON_UNSET_VALUE
);

/*
Description:
  Finds a curve by offsetting an existing curve normal to a surface.
  The caller is responsible for ensuring that the input curve lies
  on the input surface.
Parameters:
  pSrf   [in] - Surface from which normals are calculated.
  pCrv   [in] - 3d curve lying on the surface along which normals are calculated.
  height [in] - offset distance (distance from surface to result curve).
Return:
  ON_NurbsCurve* pointing to offset curve at distance height from the surface.
Remarks:
  The offset curve is interpolated through a small number of points so if the
  surface is irregular or complicated, the result will not be a very accurate offset.
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoOffsetCurveNormal(
  const ON_Surface* pSrf,
  const ON_NurbsCurve* pNC,
  double height
);

/*
Description:
  Finds a curve by offsetting an existing curve normal to a surface.
  The caller is responsible for ensuring that the input curve lies
  on the input surface.
Parameters:
  pSrf     [in] - Surface from which normals are calculated.
  pCrv     [in] - 3d curve lying on the surface along which normals are calculated.
  pHeights [in] - offset distance (distance from surface to result curve).
                  pHeights is a NURBS curve of dimension 1 with the same domain as
                  pCrv.  It is evaluated at points corresponding to the locations where
                  offsets from the surface are calculated to find the height of the offset
                  at those points.
Return:
  ON_NurbsCurve* pointing to offset curve at distance height from the surface.
Remarks:
  Same as the function above, but the offset height is specified by a function spline
  instead of a constant value.
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
See Also:
  RhinoMakeRadiusSpline
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoOffsetCurveNormal(
  const ON_Surface* pSrf,
  const ON_NurbsCurve* pNC,
  const ON_NurbsCurve* pHeights
);

/*
Description:
  Finds a curve by offsetting an existing curve normal to a surface.
  The caller is responsible for ensuring that the input curve lies
  on the input surface.
Parameters:
  pSrf     [in] - Surface from which normals are calculated.
  pCrv     [in] - 3d curve lying on the surface along which normals are calculated.
  params   [in] - parameters in the domain of pCrv for offset points to interpolate
  pHeights [in] - offset distance (distance from surface to result curve).
                  pHeights is a NURBS curve of dimension 1 with the same domain as
                  pCrv.  It is evaluated at points corresponding to the locations where
                  offsets from the surface are calculated to find the height of the offset
                  at those points.
Return:
  ON_NurbsCurve* pointing to offset curve at distance height from the surface.
Remarks:
  Same as the function above, but the offset point locations are specified.
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
See Also:
  RhinoMakeRadiusSpline
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoOffsetCurveNormal(
  const ON_Surface* pSrf,
  const ON_NurbsCurve* pNC,
  const ON_SimpleArray<double>& spine_parameters,
  const ON_NurbsCurve* pHeights
);

/*
Description:
  Makes a dimension = 1, degree = 3 spline curve that interpolates input values
  and doesn't overshoot extreme explicitly set in the input values. The output
  spline will have double knots at each interpolant and 0 derivative at extremes.
  The domain will be from dist_data[0].t to dist_data[dist_data.Count()-1].t.
  This makes a good smooth function for interpolating input values along a curve
  or for use with the RhinoOffsetCurveNormal() function.
Parameters:
  pSrf     [in] - Surface from which normals are calculated.
  pCrv     [in] - 3d curve lying on the surface along which normals are calculated.
  params   [in] - parameters in the domain of pCrv for offset points to interpolate
  pHeights [in] - offset distance (distance from surface to result curve).
                  pHeights is a NURBS curve of dimension 1 with the same domain as
                  pCrv.  It is evaluated at points corresponding to the locations where
                  offsets from the surface are calculated to find the height of the offset
                  at those points.
Return:
  ON_NurbsCurve* pointing to offset curve at distance height from the surface.
Remarks:
  Same as the function above, but the offset point locations are specified.
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
See Also:
  RhinoOffsetCurveNormal
*/
typedef struct tagMakeRadiusSplineData
{
  double m_t;        // curve parameter
  double m_distance; // distance at m_t
} MakeRadiusSplineData;

RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoMakeRadiusSpline(
  ON_Interval domain,
  ON_SimpleArray<MakeRadiusSplineData> distance_data,
  bool bClosed = false
);

/*
Description:
  Finds a curve by offsetting an existing curve tangent to a surface.
  The caller is responsible for ensuring that the input curve lies
  on the input surface.
Parameters:
  pSrf   [in] - Surface from which tangents are calculated.
  pCrv   [in] - 3d curve lying on the surface along which tangents are calculated.
  height [in] - offset distance (distance from surface to result curve).
Return:
  ON_NurbsCurve* pointing to offset curve at distance height from the surface.
Remarks:
  The offset curve is interpolated through a small number of points so if the
  surface is irregular or complicated, the result will not be a very accurate offset.
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoOffsetCurveTangent(
  const ON_Surface* pSrf,
  const ON_NurbsCurve* pNC,
  double height
);

/*
Description:
  Creates polyline curve outlines from meshes.
Parameters:
  InMeshes   [in] - The meshes to outline.
  view       [in] - The view that provides the outline direction.
  tolerance  [in] - The tolerance used in the calculation (can be tiny).
  OutRegions [out] - The output curves
Return:
  True if successful, false otherwise.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
*/
RHINO_SDK_FUNCTION
bool RhinoGetMeshOutline(
  const ON_SimpleArray<const ON_Mesh*>& InMeshes,
  const CRhinoView* view,
  double tolerance,
  ON_ClassArray<ON_SimpleArray<ON_PolylineCurve*> >& OutRegions
);

/*
Description:
  Creates polyline curve outlines from meshes.
Parameters:
  InMeshes   [in] - The meshes to outline.
  view       [in] - The view that provides the outline direction.
  plane      [in] - Usually the view's construction plane. If parallel projection
                    and view plane is parallel to this one, project results to plane.
  tolerance  [in] - The tolerance used in the calculation (can be tiny).
  OutRegions [out] - The output curves
Return:
  True if successful, false otherwise.
Remarks:
  THE CALLER IS RESPONSIBLE FOR DESTROYING THE OUTPUT CURVE.
*/
RHINO_SDK_FUNCTION
bool RhinoGetMeshOutline(
  const ON_SimpleArray<const ON_Mesh*>& InMeshes,
  const ON_Viewport& view,
  const ON_Plane& plane,
  double tolerance,
  ON_ClassArray<ON_SimpleArray<ON_PolylineCurve*>>& OutRegions
);

/*
Description:
  Destroy temporary meshes used for displaying shaded views of
  objects that are not mesh objects (like surfaces, polysurfaces
  and extrusions).
Parameters:
  mesh_type     - [in] Type of mesh to destroy.
  it            - [in] Objects to test
  bRedrawNeeded - [out] If a mesh is destroyed and a viewport is not in wireframe display mode,
                        the *bRedrawNeeded is set to true and the caller should eventually
                        call doc.Redraw() to update Rhino's display.
Return:
  Number of objects that had meshes that were destroyed.
*/
RHINO_SDK_FUNCTION
unsigned int RhinoDestroyDisplayMeshes(
  ON::mesh_type mesh_type,
  CRhinoObjectIterator& it,
  bool* bRedrawNeeded
);

/*
Description:
  Automatically adjusts camera target depth based on objects within the rectangular focus area in the viewport.
Parameters:
  pViewport   - [in/out] Viewport to adjust.
  pMousePoint - [in] Center of focus. Default is viewport center.
  pFocusSize  - [in] Focus rectangle size relative to viewport size. Default is 0.5.
  pWeight     - [in] Weight of adjustment. 0.0 means use old target (=no change),
                     1.0 means use the new adjusted target. Default is 1.0.
Return:
  True if target was adjusted.
*/
RHINO_SDK_FUNCTION
bool RhinoAutoAdjustTargetDepth(
  CRhinoViewport* pViewport,
  const ON_2iPoint* pMousePoint = nullptr,
  const double* pFocusSize = nullptr,
  const double* pWeight = nullptr
);

/*
Description:
  Destroy the splash screen if it is being displayed
*/
RHINO_SDK_FUNCTION
void RhinoKillSplash();

/*
Description:
  Builds a surface from ordered network of curves/edges.
Parameters:
  ucurves      - [in] array of U curves/edges
  ucontinuity  - [in] continuity at first and last U segment, 0 = loose, 1 = position, 2 = tangent, 3 = curvature
  vcurves      - [in] array of V curves
  vcontinuity  - [in] continuity at first and last V segment, 0 = loose, 1 = position, 2 = tangent, 3 = curvature
  surface_out  - [out] the output surface
  dEdgeTol     - [in] tolerance to use along network surface edge
  dInteriorTol - [in] tolerance to use for the interior curves
  dAngleTol    - [in] angle tolerance to use
Returns:
  0 = success
  1 = curve sorter failed
  2 = network initializing failed
  3 = failed to build surface
  4 = network surface is not valid
*/
RHINO_SDK_FUNCTION
int RhinoNetworkSurface(
  const ON_SimpleArray<const ON_Curve*>& ucurves,
  const int ucontinuity[2],
  const ON_SimpleArray<const ON_Curve*>& vcurves,
  const int vcontinuity[2],
  ON_NurbsSurface& surface_out,
  const double dEdgeTol = 0.0,
  const double dInteriorTol = 0.0,
  const double dAngleTol = 0.0
);

/*
Description:
  Builds a surface from auto-sorted network of curves/edges.
Parameters:
  curves       - [in] array of curves/edges, sorted automatically into U and V curves
  continuity   - [in] continuity along edges, 0 = loose, 1 = position, 2 = tangent, 3 = curvature
  surface_out  - [out] the output surface
  dEdgeTol     - [in] tolerance to use along network surface edge
  dInteriorTol - [in] tolerance to use for the interior curves
  dAngleTol    - [in] angle tolerance to use
Returns:
  0 = success
  1 = curve sorter failed
  2 = network initializing failed
  3 = failed to build surface
  4 = network surface is not valid
*/
RHINO_SDK_FUNCTION
int RhinoNetworkSurface(
  const ON_SimpleArray<const ON_Curve*>& curves,
  const int continuity,
  ON_NurbsSurface& surface_out,
  const double dEdgeTol = 0.0,
  const double dInteriorTol = 0.0,
  const double dAngleTol = 0.0
);

/*
Description:
  Selects objects bounded by a polyline region
Parameters:
  vp              - [in] viewport to use for selection
  region          - [in] list of points that define the selection region
  selectionmode   - [in] the selection mode, 0=window, 1=crossing, 2=outside window, 3=outside crossing window
  geometry_filter - [in] geometry filter, same as in CRhinoGetObject
  output          - [out] optional output array. If NULL, the objects are selected.
Returns:
  true if success
*/
RHINO_SDK_FUNCTION
bool RhinoRegionSelect(
  CRhinoViewport& vp,
  const ON_SimpleArray<ON_3dPoint>& region,
  const int selectionmode = 0,
  unsigned int geometry_filter = ON::any_object,
  ON_ClassArray<CRhinoObjRef>* output = nullptr
);

/*
Description:
  Creates a single-walled pipe.
Parameters:
  rail        - [in]  The rail, or path, curve.
  rail_params - [in]  An array of one or more normalized curve parameters where changes in radius occur.
                      Important: curve parameters must be normalized - ranging between 0.0 and 1.0.
                      Use ON_Interval::NormalizedParameterAt to calculate these.
  radii       - [in]  An array of radii - one at each normalized curve parameter in rail_params.
  results     - [out] The results of the pipe creation. If rail is a line segment, rail_params contains two
                      normalized curve parameters, radii contains two identical values, and and cap_mode is either
                      0 (None) or 1 (Flat), then an ON_Extrusion object will be returned. Otherwise, an ON_Brep
                      object will be returned. Note, it is possible for more than one ON_Brep to be returned
                      (when it is not possible to miter sharp corners, for example).
  abstol      - [in]  The sweeping and fitting tolerance. If you are unsure whaat to use, then use the document's
                      absolute tolerance - CRhinoDoc::AbsoluteTolerance().
  angtol      - [in]  The angle tolerance. If you are unsure what to use, then either use the document's
                      angle tolerance in radians - CRhinoDoc::AngleToleranceRadians().
  blend_type  - [in]  The shape blending, where:
                      0: Local (pipe radius stays constant at the ends and changes more rapidly in the middle)
                      1: Global (radius is linearly blended from one end to the other, creating pipes that taper from one radius to the other)
  cap_mode    - [in]  The end cap mode, where:
                      0: None
                      1: Flat (cap with planar surface)
                      2: Round (cap with hemispherical surface)
  fit_rail    - [in]  If the curve is a polycurve of lines and arcs, the curve is fit and a single surface is created;
                      otherwise the result is a polysurface with joined surfaces created from the polycurve segments.
Returns:
  true if success, false otherwise.
Remarks:
  CRITICAL: Memory for the resulting ON_Extrusion or ON_Brep object is allocated. It is the calling functions
  responsibility to clean up the memory.
*/
RHINO_SDK_FUNCTION
bool RhinoPipe(
  const ON_Curve& rail,
  const ON_SimpleArray<double>& rail_params,
  const ON_SimpleArray<double>& radii,
  ON_SimpleArray<ON_Object*>& results,
  double abstol,
  double angtol,
  int blend_type = 0,
  int cap_mode = 0,
  bool fit_rail = false
);

/*
Description:
  Creates a double-walled pipe.
Parameters:
  rail        - [in]  The rail, or path, curve.
  rail_params - [in]  An array of one or more normalized curve parameters where changes in radius occur.
                      Important: curve parameters must be normalized - ranging between 0.0 and 1.0.
                      Use ON_Interval::NormalizedParameterAt to calculate these.
  radii0      - [in]  An array of radii for the first wall - one at each normalized curve parameter in rail_params.
  radii1      - [in]  An array of radii for the second wall - one at each normalized curve parameter in rail_params.
  results     - [out] The results of the pipe creation. If rail is a line segment, rail_params contains two
                      normalized curve parameters, radii0 contains two identical values, radii1 contains two identical values,
                      and and cap_mode is 1 (Flat), then a single ON_Extrusion object will be returned.
                      If cap_mode is 0 (None), then two ON_Extrusion objects will be returned. Otherwise, an ON_Brep
                      object will be returned. Note, it is possible for more than one ON_Brep to be returned
                      (if cap_mode = 0, or when it is not possible to miter sharp corners, for example).
  abstol      - [in]  The sweeping and fitting tolerance. If you are unsure what to use, then use the document's
                      absolute tolerance - CRhinoDoc::AbsoluteTolerance().
  angtol      - [in]  The angle tolerance. If you are unsure what to use, then either use the document's
                      angle tolerance in radians - CRhinoDoc::AngleToleranceRadians().
  blend_type  - [in]  The shape blending, where:
                      0: Local (pipe radius stays constant at the ends and changes more rapidly in the middle)
                      1: Global (radius is linearly blended from one end to the other, creating pipes that taper from one radius to the other)
  cap_mode    - [in]  The end cap mode, where:
                      0: None
                      1: Flat (cap with planar surface)
                      2: Round (cap with hemispherical surface)
  fit_rail    - [in]  If the curve is a polycurve of lines and arcs, the curve is fit and a single surface is created;
                      otherwise the result is a polysurface with joined surfaces created from the polycurve segments.
Returns:
  true if success, false otherwise.
Remarks:
  CRITICAL: Memory for the resulting ON_Extrusion or ON_Brep object is allocated. It is the calling functions
  responsibility to clean up the memory.
*/
RHINO_SDK_FUNCTION
bool RhinoPipe(
  const ON_Curve& rail,
  const ON_SimpleArray<double>& rail_params,
  const ON_SimpleArray<double>& radii0,
  const ON_SimpleArray<double>& radii1,
  ON_SimpleArray<ON_Object*>& results,
  double abstol,
  double angtol,
  int blend_type = 0,
  int cap_mode = 0,
  bool fit_rail = false
);

/*
Description:
  Finds a "zero-twisting" plane that is perpendicular to a curve at a curve parameter.
  The result plane is "zero-twisting" in that it does not flip based on the curvature
  of the curve at the parameter.
Parameters:
  crv   [in]  - The curve to evaluate.
  crv_t [in]  - The evaluation parameter.
  plane [out] - A perpendicular plane.
Return:
  True or false indicating success or failure.
*/
RHINO_SDK_FUNCTION
bool RhinoGetPerpendicularCurvePlane(
  const ON_Curve* crv,
  double crv_t,
  ON_Plane& plane
);

/*
Description:
  Tries to convert a curve to a piecewise linear curve, by checking for stacked control points
  and fully multiple knots. Works on curves of dim >= 2, also high-dimensional curves.
Parameters:
  crv          [in]  - the input curve
  tolerance    [in]  - the tolerance for linearity
  linear       [out] - if successful, the linearized degree-1 curve

Return:
  True or false indicating success or failure.
  If false is returned, the state of the curve 'linear' is undetermined.
*/
RHINO_SDK_FUNCTION
bool RhinoMakePiecewiseLinearCurve(
  const ON_NurbsCurve& crv,
  double tolerance,
  ON_NurbsCurve& linear
);

/*
Description:
  Tries to convert a surface to a piecewise linear surface, by checking for stacked control points
  and fully multiple knots in both directions.
Parameters:
  srf          [in]  - the input surface
  tolerance    [in]  - the tolerance for linearity
  linear       [out] - if successful, the linearized degree-1 surface

Return:
  True or false indicating success or failure.
  If false is returned, the state of the surface 'linear' is undetermined.
*/
RHINO_SDK_FUNCTION
bool RhinoMakePiecewiseLinearSurface(
  const ON_NurbsSurface& srf,
  double tolerance,
  ON_NurbsSurface& linear
);


/*
Description:
  The data in this struct is used for variable radius fillet edge. 
  The parameter t gives the position of a variable radius,
  and the parameter distance gives the value of the radius.
*/
struct RHINO_SDK_CLASS CRhinoBrepEdgeFilletDistance
{
  double t;         // the curve parameter on which to apply the distance
  double distance;  // the distance to apply
};

class RHINO_SDK_CLASS CRhinoFilletEdge
{
public:
  enum BlendType
  {
    Chamfer, // Creates a ruled surface between Brep edges with varying chamfer distances.
             // The Brep is trimmed and joined to the chamfer surfaces.
    Fillet,  // Creates a tangent surface between Brep edges with varying radius values.
             // The Brep is trimmed and joined to the fillet surfaces.
    Blend,   // Creates a curvature-continuous blend surface between Brep edges with varying radius values.
             // The Brep is trimmed and joined to the blend surfaces.
  };

  enum RailType
  {
    DistFromEdge,     // The distance from the edge curves determines the intersection.
    RollingBall,      // The radius of a rolling ball determines the intersection.
    DistBetweenRails, // The distance between the edge rails determines the intersection.
  };



  /*
  Description:
    Fillets, chamfers, or blends the edges of a Brep with support for variable radii along each edge.
  Parameters:
    brep                  [in]  - The Brep to fillet, chamfer, or blend edges.
    edges                 [in]  - An array of one or more edge indices where the fillet, chamfer, or blend will occur.
    distances             [in]  - An array of arrays of distance data, one array for each edge.
    blend_type            [in]  - The blend type (see above).
    rail_type             [in]  - The rail type (see above).
    bool setback_fillets  [in] -  Use setback fillets (only used for blend_type CRhinoFilletEdge::BlendType::Blend)
    tolerance             [in]  - The tolerance to be used to perform calculations.
    angle_tolerance       [in]  - The angle tolerance to be used to perform calculations.
    out_breps             [out] - The results of the operation. It is the responsibility of the caller to clean up the memory.
  Return:
     True if successful, False otherwise
  Remarks:
    CRITICAL: Memory for the resulting Breps is allocated. It is the calling functions responsibility to clean up the memory.
  */
  static bool FilletEdgesVariableRadius(
    const ON_Brep& brep,
    const ON_SimpleArray<int>& edges,
    const ON_ClassArray<ON_SimpleArray<CRhinoBrepEdgeFilletDistance>>& distances,
    CRhinoFilletEdge::BlendType blend_type,
    CRhinoFilletEdge::RailType rail_type,
    bool setback_fillets,
    double tolerance,
    double angle_tolerance,
    ON_SimpleArray<ON_Brep*>& out_breps
  );


  /*
  Description:
    Fillets, chamfers, or blends the edges of a Brep with support for a different start and end radius for each edge.
  Parameters:
    brep          [in]  - The Brep to fillet, chamfer, or blend edges.
    edges         [in]  - An array of one or more edge indices where the fillet, chamfer, or blend will occur.
    radii0        [in]  - An array of starting fillet, chamfer, or blend radii, one for each edge index.
    radii1        [in]  - An array of end fillet, chamfer, or blend radii, one for each edge index.
    blend_type    [in]  - The blend type (see above).
    rail_type     [in]  - The rail type (see above).
    tolerance     [in]  - The tolerance to be used to perform calculations.
    out_breps     [out] - The results of the operation. It is the responsibility of the caller to clean up the memory.
  Return:
     True if successful, False otherwise
  Remarks:
    CRITICAL: Memory for the resulting Breps is allocated. It is the calling functions responsibility to clean up the memory.
  */
  static bool FilletEdges(
    const ON_Brep& brep,
    const ON_SimpleArray<int>& edges,
    const ON_SimpleArray<double>& radii0,
    const ON_SimpleArray<double>& radii1,
    CRhinoFilletEdge::BlendType blend_type,
    CRhinoFilletEdge::RailType rail_type,
    double tolerance,
    ON_SimpleArray<ON_Brep*>& out_breps
  );


  /*
  Description:
    Fillets, chamfers, or blends the edges of a Brep with support for a different start and end radius for each edge.
  Parameters:
    brep                  [in]  - The Brep to fillet, chamfer, or blend edges.
    edges                 [in]  - An array of one or more edge indices where the fillet, chamfer, or blend will occur.
    radii0                [in]  - An array of starting fillet, chamfer, or blend radii, one for each edge index.
    radii1                [in]  - An array of end fillet, chamfer, or blend radii, one for each edge index.
    blend_type            [in]  - The blend type (see above).
    rail_type             [in]  - The rail type (see above).
    bool setback_fillets  [in] -  Use setback fillets (only used for blend_type CRhinoFilletEdge::BlendType::Blend)
    tolerance             [in]  - The tolerance to be used to perform calculations.
    angle_tolerance       [in]  - The angle tolerance to be used to perform calculations.
    out_breps             [out] - The results of the operation. It is the responsibility of the caller to clean up the memory.
  Return:
     True if successful, False otherwise
  Remarks:
    CRITICAL: Memory for the resulting Breps is allocated. It is the calling functions responsibility to clean up the memory.
  */
  static bool FilletEdges(
    const ON_Brep& brep,
    const ON_SimpleArray<int>& edges,
    const ON_SimpleArray<double>& radii0,
    const ON_SimpleArray<double>& radii1,
    CRhinoFilletEdge::BlendType blend_type,
    CRhinoFilletEdge::RailType rail_type,
    bool setback_fillets,
    double tolerance,
    double angle_tolerance,
    ON_SimpleArray<ON_Brep*>& out_breps
  );

};

/*
Description:
  Creates one or more Breps with a single planar face from an array of curves.
Parameters:
  InCurves  [in]  - Input loops to sort and use as boundaries.
  OutBreps  [out] - Output array of Breps
  tolerance [in]  - The tolerance to be used to perform calculations.
Return:
  True if successful, false otherwise.
Remarks:
  Note, if a input  curve is self-intersecting, then curve boolean processing
  will be used.
  CRITICAL: Memory for the resulting Breps is allocated. It is the calling
  functions responsibility to clean up the memory.
*/
RHINO_SDK_FUNCTION
bool RhinoPlanarSrf(
  const ON_SimpleArray<const ON_Curve*>& InCurves,
  ON_SimpleArray<ON_Brep*>& OutBreps,
  double tolerance
);

/*
Description:
  Given 6 or more pairs of screen (2d) and world (3d) points, together with
  the viewport, this function determines where the perspective camera is positioned.
  The image created by the screen points must be centered in the viewport; i.e., non
  symmetric cropping is not permitted.
Parameters:
  rhino_view [in]  - The Rhino view.
  points2d   [in]  - The 2d screen points
  points3d   [in]  - The 3d world points
  view       [out] - A modified version of the view, maintained by the input Rhino view,
                     containing the calculated camera information.
Return:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoPerspectiveMatch(
  CRhinoView* rhino_view,
  const ON_SimpleArray<POINT>& points2d,
  const ON_SimpleArray<ON_3dPoint>& points3d,
  ON_3dmView& view
);

/*
Description:
  Sets external texture packing routine.
Parameters:
  routine [in] - Pointer to a texture packing function. This function takes in two parameters:
                  const ON_SimpleArray<const ON_Mesh*>& meshes
                    Array of Brep meshes. Never modify meshes in this array.
                  ON_SimpleArray<ON_Mesh*>& packed_tex_out
                    Array of empty meshes to transfer packing settings.
                  Routine is supposed to fill in m_packed_tex_domain and m_packed_tex_rotate.
                  Return true if the packing settings in packed_tex_out are good to apply, or
                  false if they are to be discarded.
Return:
  True if the routine is successfully hooked up and otherwise false.
*/
RHINO_SDK_FUNCTION
bool RhinoSetExternalPackBrepTexturesRoutine(
  bool (*routine)(const ON_SimpleArray<const ON_Mesh*>& meshes, ON_SimpleArray<ON_Mesh*>& packed_tex_out)
);

/*
Description:
  Creates curves between two open or closed input curves.
Parameters:
  start_crv   [in] - Start curve.
  end_crv     [in] - End curve.
  num_of_crvs [in] - Number of tween curves.
  tol         [in] - Tolerance to use, if unsure use document tolerance.
  tween_crvs  [out] - Output array of tween curves
Return:
  True if successful, false otherwise.
Remarks:
  Uses the control points of the curves for finding tween
  curves. That means the first control point of first curve is
  matched to first control point of the second curve and so on.
  There is no matching of curves direction.  Caller must match
  input curves direction before calling the function.
*/
RHINO_SDK_FUNCTION
bool RhinoTweenCurves(
  const ON_Curve* start_crv,
  const ON_Curve* end_crv,
  int num_of_crvs,
  double tol,
  ON_SimpleArray<ON_Curve*>& tween_crvs
);

/*
Description:
  Creates curves between two open or closed input curves.
  Make the structure of input curves compatible if needed.
Parameters:
  start_crv   [in] - Start curve.
  end_crv     [in] - End curve.
  num_of_crvs [in] - Number of tween curves.
  tol         [in] - Tolerance to use, if unsure use document tolerance.
  tween_crvs  [out] - Output array of tween curves
Return:
  True if successful, false otherwise.
Remarks:
  Refits the input curves to have the same structure. The
  resulting curves are usually more complex than input unless
  input curves are compatible and no refit is needed.
  There is no matching of curves direction.  Caller must match
  input curves direction before calling the function.
*/
RHINO_SDK_FUNCTION
bool RhinoTweenCurvesWithMatching(
  const ON_Curve* start_crv,
  const ON_Curve* end_crv,
  int num_of_crvs,
  double tol,
  ON_SimpleArray<ON_Curve*>& tween_crvs
);

/*
Description:
  Creates curves between two open or closed input curves.
  Use sample points method to make curves compatible.
Parameters:
  start_crv   [in] - Start curve.
  end_crv     [in] - End curve.
  num_of_crvs [in] - Number of tween curves.
  sample_num  [in] - Number of sample points along input curves.
  tol         [in] - Tolerance to use, if unsure use document tolerance.
  tween_crvs  [out] - Output array of tween curves.
Return:
  True if successful, false otherwise.
Remarks:
  This is how the algorithm works: Divides the two curves into
  an equal number of points, finds the midpoint between the
  corresponding points on the curves and interpolates the tween
  curve through those points.
  There is no matching of curves direction. Caller must match
  input curves direction before calling the function.
*/
RHINO_SDK_FUNCTION
bool RhinoTweenCurveWithSampling(
  const ON_Curve* start_crv,
  const ON_Curve* end_crv,
  int num_of_crvs,
  int sample_num,
  double tol,
  ON_SimpleArray<ON_Curve*>& tween_crvs
);

/*
Description:
  Returns the status of a linked block.
Parameters:
  idef [in] - The instance definition to query.
Return:
  RHINO_IDEF_STATUS - the status.
*/
enum RHINO_IDEF_STATUS
{
  RHINO_IDEF_NOT_A_LINKED_IDEF = -3,
  RHINO_IDEF_LINKED_FILE_NOT_READABLE = -2,
  RHINO_IDEF_LINKED_FILE_NOT_FOUND = -1,
  RHINO_IDEF_LINKED_FILE_IS_TO_DATE = 0,
  RHINO_IDEF_LINKED_FILE_IS_NEWER = 1,
  RHINO_IDEF_LINKED_FILE_IS_OLDER = 2,
  RHINO_IDEF_LINKED_FILE_IS_DIFFERENT = 3,
};

RHINO_SDK_FUNCTION
RHINO_IDEF_STATUS RhinoInstanceArchiveFileStatus(const CRhinoInstanceDefinition* idef);

/*
Description:
  Creates a hollowed out shell from a solid Brep.
  Function only operates on simple, solid, manifold Breps.
Parameters:
  brep            [in] - The solid Brep to shell.
  faces_to_remove [in] - The indices of the Brep faces to remove. These surfaces
                         are removed and the remainder is offset inward, using
                         the outer parts of the removed surfaces to join the inner
                         and outer parts.
  distance        [in] - The distance, or thickness, for the shell.
                         This is a signed distance value with respect to face normals
                         and flipped faces.
  tolerance       [in] - The offset tolerance. When in doubt, use the document's
                         absolute tolerance.
  shells          [out] - The results of the shell calculation.
Return:
  The number of Breps added to the output array.
*/
RHINO_SDK_FUNCTION
int RhinoShellBrep(
  const ON_Brep& brep,
  const ON_SimpleArray<int>& faces_to_remove,
  double distance,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& shells
);

/*
Description:
  Offsets a Brep.
Parameters:
  brep            [in] - The Brep to offset.
  distance        [in] - The distance to offset. This is a signed distance value
                         with respect to face normals and flipped faces.
  tolerance       [in] - The offset tolerance. When in doubt, use the document's
                         absolute tolerance.
  bSolid          [in] - If true, then the function makes a closed solid from the
                         input and offset surfaces by lofting a ruled surface between
                         all of the matching edges.
  bExtend         [in] - If true, then the function maintains the sharp corners when
                         the original surfaces have sharps corner.
                         If False, then the function creates fillets at sharp corners
                         in the original surfaces.
  Offsets         [out] - The results of the calculation (see Remarks).
  Blends          [out] - The results of the calculation (see Remarks).
  Walls           [out] - The results of the calculation (see Remarks).
Return:
  True if the function succeeds (see Remarks).
  False if the function did not succeed  (see Remarks).
  NOTE, In either case, caller is responsible for destroying the Breps in the
        Offsets, Blends, and Walls arrays.
Remarks:
  If the function succeeds in offsetting the Brep, the single result should be Offsets[0].
  Otherwise, Offsets will contain the offset surfaces. Blends will contain the set of blends
  used to fill in gaps if bExtend is false. Walls will contain the set of wall surfaces that
  was supposed to join the offset to the original if bSolid is true.
*/
RHINO_SDK_FUNCTION
bool RhinoOffsetBrep(
  const ON_Brep& B,
  double distance,
  double tolerance,
  bool bSolid,
  bool bExtend,
  ON_SimpleArray<ON_Brep*>& Offsets,
  ON_SimpleArray<ON_Brep*>& Blends,
  ON_SimpleArray<ON_Brep*>& Walls
);

/*
Description:
  Offsets a Brep.
Parameters:
  brep            [in] - The Brep to offset.
  distance        [in] - The distance to offset. This is a signed distance value
                         with respect to face normals and flipped faces.
  tolerance       [in] - The offset tolerance. When in doubt, use the document's
                         absolute tolerance.
  bSolid          [in] - If true, then the function makes a closed solid from the
                         input and offset surfaces by lofting a ruled surface between
                         all of the matching edges.
  bExtend         [in] - If true, then the function maintains the sharp corners when
                         the original surfaces have sharps corner.
                         If False, then the function creates fillets at sharp corners
                         in the original surfaces.
  bShrinkInputSrfs [in] - If true, then shrink all surfaces to outer loop pboxes.
  Offsets         [out] - The results of the calculation (see Remarks).
  Blends          [out] - The results of the calculation (see Remarks).
  Walls           [out] - The results of the calculation (see Remarks).
Return:
  True if the function succeeds (see Remarks).
  False if the function did not succeed  (see Remarks).
  NOTE, In either case, caller is responsible for destroying the Breps in the
        Offsets, Blends, and Walls arrays.
Remarks:
  If the function succeeds in offsetting the Brep, the single result should be Offsets[0].
  Otherwise, Offsets will contain the offset surfaces. Blends will contain the set of blends
  used to fill in gaps if bExtend is false. Walls will contain the set of wall surfaces that
  was supposed to join the offset to the original if bSolid is true.
*/
RHINO_SDK_FUNCTION
bool RhinoOffsetBrep(
  const ON_Brep& B,
  double distance,
  double tolerance,
  bool bSolid,
  bool bExtend,
  bool bShrinkInputSrfs,
  ON_SimpleArray<ON_Brep*>& Offsets,
  ON_SimpleArray<ON_Brep*>& Blends,
  ON_SimpleArray<ON_Brep*>& Walls
);


/*
Description:
  Removes kinks from a curve. Periodic curves deform smoothly without kinks.
Parameters:
  curve   [in] - The curve to make periodic. Curve must have degree >= 2.
  bSmooth [in] - Controls kink removal.
                 If true, smooths any kinks in the curve and moves control points
                 to make a smooth curve.
                 If false, control point locations are not changed or changed minimally
                 (only one point may move) and only the knot vector is altered.
Return:
  ON_NurbsCurve* - The resulting curve or NULL.
  NOTE, memory for the curve is allocated and becomes the responsibility of the caller.
Remarks:
  A periodic curve is a smooth closed curve. Periodic curves stay smooth when you edit them.
  If a joined curve is made periodic, it becomes a single-span curve and can no longer explode.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoMakeCurvePeriodic(
  const ON_Curve& curve,
  bool bSmooth
);

/*
Description:
  Removes kinks from a surface. Periodic surface deform smoothly without kinks.
Parameters:
  surface [in] - The surface to make periodic.
  dir     [in] - The direction to make periodic, where u = 0, v = 1.
                 Surface in dir direction must have degree >= 2.
  bSmooth [in] - Controls kink removal.
                 If true, smooths any kinks in the surface and moves control points
                 to make a smooth surface.
                 If false, control point locations are not changed or changed minimally
                 (only one point may move) and only the knot vector is altered.
Return:
  ON_NurbsSurface* - The resulting surface or NULL.
  NOTE, memory for the surface is allocated and becomes the responsibility of the caller.
Remarks:
  Periodic surfaces are closed surfaces (such as a cylindrical surface) that can be deformed
  without developing kinks. Periodic surfaces are automatically created when the input curves
  are periodic.
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoMakeSurfacePeriodic(
  const ON_Surface& surface,
  int dir,
  bool bSmooth
);

ON_DEPRECATED_MSG("use the version of RhinoSmoothCurve that accepts a plane parameter")
RHINO_SDK_FUNCTION
ON_Curve* RhinoSmoothCurve(
  const ON_Curve* pInput,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem
);

/*
Description:
  Smooths curves. The parameters are the same as in Smooth command.
Parameters:
  pInput [in] - Curve to smooth
  dSmoothFactor [in] - The smoothing factor. Controls how much control points move towards the
                       average of the neighboring points.
  bXSmooth [in] - When true control points move in X axis direction
  bYSmooth [in] - When true control points move in Y axis direction
  bZSmooth [in] - When true control points move in Z axis direction
  bFixBoundaries [in] - When true the curve ends don't move
  nCoordinateSystem [in] - Current coordinate system. 0=World, 1=CPlane, 2=Object.
  pPlane [in] = If nCoordinateSystem == 1, then the construction plane. Otherwise pass nullptr.
Return:
  ON_Curve* - The resulting curve or nullptr.
  NOTE, memory for the curve is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoSmoothCurve(
  const ON_Curve* pInput,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem,
  const ON_Plane* pPlane
);

ON_DEPRECATED_MSG("use the version of RhinoSmoothSurface that accepts a plane parameter")
RHINO_SDK_FUNCTION
ON_Surface* RhinoSmoothSurface(
  const ON_Surface* pInput,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem
);

/*
Description:
  Smooths surfaces. The parameters are the same as in Smooth command.
Parameters:
  pInput [in] - Surface to smooth
  dSmoothFactor [in] - The smoothing factor. Controls how much control points move towards
                       the average of the neighboring points.
  bXSmooth [in] - When true control points move in X axis direction
  bYSmooth [in] - When true control points move in Y axis direction
  bZSmooth [in] - When true control points move in Z axis direction
  bFixBoundaries [in] - When true the surface edges don't move
  nCoordinateSystem [in] - Current coordinate system. 0=World, 1=CPlane, 2=Object.
  pPlane [in] = If nCoordinateSystem == 1, then the construction plane. Otherwise pass nullptr.
Return:
  ON_Surface* - The resulting surface or nullptr.
  NOTE, memory for the surface is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoSmoothSurface(
  const ON_Surface* pInput,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem,
  const ON_Plane* pPlane
);


ON_DEPRECATED_MSG("use the version of RhinoSmoothMesh that accepts a plane parameter")
RHINO_SDK_FUNCTION
ON_Mesh* RhinoSmoothMesh(
  const ON_Mesh* pInput,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem
);

/*
Description:
  Smooths meshes. The parameters are the same as in Smooth command.
Parameters:
  pInput [in] - Mesh to smooth
  dSmoothFactor [in] - The smoothing factor. Controls how much control points move towards the average of the neighboring points.
  bXSmooth [in] - When true control points move in X axis direction
  bYSmooth [in] - When true control points move in Y axis direction
  bZSmooth [in] - When true control points move in Z axis direction
  bFixBoundaries [in] - When true the mesh edges don't move
  nCoordinateSystem [in] - Current coordinate system. 0=World, 1=CPlane, 2=Object.
  pPlane [in] = If nCoordinateSystem == 1, then the construction plane. Otherwise pass nullptr.
Return:
  ON_Mesh* - The resulting mesh or nullptr.
  NOTE, memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoSmoothMesh(
  const ON_Mesh* pInput,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem,
  const ON_Plane* pPlane
);

/*
Description:
  Smooths meshes. The parameters are the same as in Smooth command.
Parameters:
  pInput [in] - Mesh to smooth
  dSmoothFactor [in] - The smoothing factor. Controls how much control points move towards the average of the neighboring points.
  numSteps [in] - The number of smoothing iterations.
  bXSmooth [in] - When true control points move in X axis direction
  bYSmooth [in] - When true control points move in Y axis direction
  bZSmooth [in] - When true control points move in Z axis direction
  bFixBoundaries [in] - When true the mesh edges don't move
  nCoordinateSystem [in] - Current coordinate system. 0=World, 1=CPlane, 2=Object.
  pPlane [in] = If nCoordinateSystem == 1, then the construction plane. Otherwise pass nullptr.
Return:
  ON_Mesh* - The resulting mesh or nullptr.
  NOTE, memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoSmoothMesh(
  const ON_Mesh* pInput,
  double dSmoothFactor,
  int numSteps,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem,
  const ON_Plane* pPlane
);

/*
Description:
  Smooths part of a mesh. The parameters are the same as in Smooth command.
Parameters:
  pInput [in] - Mesh to smooth
  InVertices [in] - The mesh vertex indices that specify the part of the mesh to smooth.
  dSmoothFactor [in] - The smoothing factor. Controls how much control points move towards the average of the neighboring points.
  bXSmooth [in] - When true control points move in X axis direction
  bYSmooth [in] - When true control points move in Y axis direction
  bZSmooth [in] - When true control points move in Z axis direction
  bFixBoundaries [in] - When true the mesh edges don't move
  nCoordinateSystem [in] - Current coordinate system. 0=World, 1=CPlane, 2=Object.
  pPlane [in] = If nCoordinateSystem == 1, then the construction plane. Otherwise pass nullptr.
Return:
  ON_Mesh* - The resulting mesh or nullptr.
  NOTE, memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoSmoothMesh(
  const ON_Mesh* pInput,
  const ON_SimpleArray<int>& InVertices,
  double dSmoothFactor,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem,
  const ON_Plane* pPlane
);

/*
Description:
  Smooths part of a mesh. The parameters are the same as in Smooth command.
Parameters:
  pInput [in] - Mesh to smooth
  InVertices [in] - The mesh vertex indices that specify the part of the mesh to smooth.
  dSmoothFactor [in] - The smoothing factor. Controls how much control points move towards the average of the neighboring points.
  numSteps [in] - The number of smoothing iterations.
  bXSmooth [in] - When true control points move in X axis direction
  bYSmooth [in] - When true control points move in Y axis direction
  bZSmooth [in] - When true control points move in Z axis direction
  bFixBoundaries [in] - When true the mesh edges don't move
  nCoordinateSystem [in] - Current coordinate system. 0=World, 1=CPlane, 2=Object.
  pPlane [in] = If nCoordinateSystem == 1, then the construction plane. Otherwise pass nullptr.
Return:
  ON_Mesh* - The resulting mesh or nullptr.
  NOTE, memory for the mesh is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Mesh* RhinoSmoothMesh(
  const ON_Mesh* pInput,
  const ON_SimpleArray<int>& InVertices,
  double dSmoothFactor,
  int numSteps,
  bool bXSmooth,
  bool bYSmooth,
  bool bZSmooth,
  bool bFixBoundaries,
  int nCoordinateSystem,
  const ON_Plane* pPlane
);

/*
Description:
  Creates curve outlines from the specified text string using the glyphs from
  the specified font. Output curves will lie on in World X-Y plane with the
  lower-left corner of output curves bounding box at (0,0,0).
Parameters:
  str              [in]  - The text string to create outlines from.
  font             [in]  - The font that provides the glyphs.
  text_height      [in]  - The height of the outlines.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tolerance   [in]  - The tolerance used to join contours
  out_glyphs       [out] - A nested array of output curves that are organized in the
                           following manner:
                               glyphs         contours        curves
                            ON_ClassArray< ON_ClassArray < ON_SimpleArray< ON_Curve* > > >
Return:
  true or false indicating success or failure.
  NOTE, memory for the output is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoGetTextOutlines(
  const wchar_t* str,
  const ON_Font* font,
  double text_height,
  bool close_contours,
  double join_tolerance,
  ON_ClassArray<ON_ClassArray<ON_SimpleArray<ON_Curve*>>>& out_glyphs
);

/*
Description:
  Creates curve outlines from the specified text string using the glyphs from
  the specified font. Output curves will lie on in World X-Y plane with the
  lower-left corner of output curves bounding box at (0,0,0).
Parameters:
  str              [in]  - The text string to create outlines from.
  font             [in]  - The font that provides the glyphs.
  text_height      [in]  - The height of the outlines.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tolerance   [in]  - The tolerance used to join contours
  small_caps_scale [in]  - Scale for small caps, set to 1.0 for no small caps
  out_glyphs       [out] - A nested array of output curves that are organized in the
  following manner:
                           glyphs         contours        curves
                           ON_ClassArray< ON_ClassArray < ON_SimpleArray< ON_Curve* > > >
Return:
  true or false indicating success or failure.
  NOTE, memory for the output is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoGetTextOutlinesWithSmallCaps(
  const wchar_t* str,
  const ON_Font* font,
  double text_height,
  bool close_contours,
  double join_tolerance,
  double small_caps_scale,
  ON_ClassArray<ON_ClassArray<ON_SimpleArray<ON_Curve*>>>& out_glyphs
);

/*
Description:
  Creates curve outlines for all of the runs in the ON_Text object.  Output
  is ordered by glyphs, then contours, then curves making up each contour.  There
  is nothing in place to distinguish glyphs from any particular run but they will
  be in the same order as they are in ON_Text and transformed to the proper run
  offset.
Parameters:
  textobj          [in]  - The text object to create outlines from.
  viewport         [in]  - The viewport from which the object was selected.  Can be null.
  dimstyle         [in]  - The text object's dimension style.  Can be null.
                           If null, then ON_DimStyle::Default is used.
  text_scale       [in]  - Additional scaling factor. Use 1.0, for no scaling.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tolerance   [in]  - The tolerance used to join contours
  small_caps_scale [in]  - Scale for small caps, set to 1.0 for no small caps
  out_glyphs       [out] - A nested array of output curves that are organized in the
                           following manner:
  glyphs         contours        curves
  ON_ClassArray< ON_ClassArray < ON_SimpleArray< ON_Curve* > > >
Return:
  true or false indicating success or failure.
  NOTE, memory for the output is allocated and becomes the responsibility of the caller.
Remarks:
  A transformation is calculated from the viewport, dimstyle and text_scale arguments for
  proper placement of the output. See ON_Annotation::GetTextXform().
*/
RHINO_SDK_FUNCTION
bool RhinoGetTextOutlinesWithSmallCaps(
  const ON_Text* pText,
  const ON_Viewport* viewport,
  const ON_DimStyle* dimstyle,
  double text_scale,
  bool close_contours,
  double join_tol,
  double small_caps_scale,
  ON_ClassArray<ON_ClassArray<ON_SimpleArray<ON_Curve*>>>& out_glyphs
);

/*
Description:
  Creates various "TextObject" type objects from the text of an annotation object.
Parameters:
  pAnno            [in]  - The text object to create outlines from.
  viewport         [in]  - The viewport from which the object was selected.  Can be null.
  dimstyle         [in]  - The text object's dimension style.  Can be null.
                           If null, then ON_DimStyle::Default is used.
  text_scale       [in]  - Additional scaling factor. Use 1.0, for no scaling.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tol         [in]  - The tolerance used to join contours
  variety          [in]  - Determines type of objects the output will be.
  height           [in]  - Height of polysurfaces or extrusions.  Set to 0.0 and variety
                           to Brep to get trimmed planes as the output.
  spacing          [in]  - Additional spacing to add between glyphs. Set to 0.0 to disable.
  output           [out] - An array of output objects.
Return:
  true or false indicating success or failure.
  NOTE, memory for the output is allocated and becomes the responsibility of the caller.
Remarks:
  A transformation is calculated from the viewport, dimstyle and text_scale arguments for
  proper placement of the output. See ON_Annotation::GetTextXform().
*/
RHINO_SDK_FUNCTION
bool RhinoCreateTextObjectGeometry(
  const ON_Annotation* pAnno,
  const ON_Viewport* vp,
  const ON_DimStyle* dimstyle,
  double text_scale,
  bool close_contours,
  double join_tol,
  double small_caps_scale,
  ON::object_type variety,
  double height,
  double spacing,
  ON_SimpleArray<ON_Object*>& output
);

/*
Description:
  Creates various "TextObject" type objects from the text of an annotation object.
Parameters:
  pAnno            [in]  - The text object to create outlines from.
  viewport         [in]  - The viewport from which the object was selected.  Can be null.
  dimstyle         [in]  - The text object's dimension style.  Can be null.
                           If null, then ON_DimStyle::Default is used.
  text_scale       [in]  - Additional scaling factor. Use 1.0, for no scaling.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tol         [in]  - The tolerance used to join contours
  make_small_caps  [in]  - Set to true to make small caps, even if scale is 1.0
  small_caps_scale [in]  - Scale of small caps compared to regular caps 
  variety          [in]  - Determines type of objects the output will be.
  height           [in]  - Height of polysurfaces or extrusions.  Set to 0.0 and variety
                           to Brep to get trimmed planes as the output.
  spacing          [in]  - Additional spacing to add between glyphs. Set to 0.0 to disable.
  letter_output           [out] - An array of contour arrays, one contour array per letter.
Return:
  true or false indicating success or failure.
  NOTE, memory for the output is allocated and becomes the responsibility of the caller.
Remarks:
  A transformation is calculated from the viewport, dimstyle and text_scale arguments for
  proper placement of the output. See ON_Annotation::GetTextXform().
*/
RHINO_SDK_FUNCTION
bool RhinoCreateTextObjectGeometryByLetter(
  const ON_Annotation* pAnno,
  const ON_Viewport* vp,
  const ON_DimStyle* dimstyle,
  double text_scale,
  bool close_contours,
  double join_tol,
  bool make_small_caps,
  double small_caps_scale,
  ON::object_type variety,
  double height,
  double spacing,
  ON_ClassArray<ON_SimpleArray<ON_Object*>>& letter_output
);

/*
Description:
  Creates various "TextObject" type objects from the text of an annotation object.
Parameters:
  pAnno            [in]  - The text object to create outlines from.
  viewport         [in]  - The viewport from which the object was selected.  Can be null.
  dimstyle         [in]  - The text object's dimension style.  Can be null.
                           If null, then ON_DimStyle::Default is used.
  text_scale       [in]  - Additional scaling factor. Use 1.0, for no scaling.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tol         [in]  - The tolerance used to join contours
  small_caps_scale [in]  - Scale of small caps compared to regular caps, 1.0 disables
  variety          [in]  - Determines type of objects the output will be.
  height           [in]  - Height of polysurfaces or extrusions.  Set to 0.0 and variety
                           to Brep to get trimmed planes as the output.
  spacing          [in]  - Additional spacing to add between glyphs. Set to 0.0 to disable.
  letter_output           [out] - An array of contour arrays, one contour array per letter.
Return:
  true or false indicating success or failure.
  NOTE, memory for the output is allocated and becomes the responsibility of the caller.
Remarks:
  A transformation is calculated from the viewport, dimstyle and text_scale arguments for
  proper placement of the output. See ON_Annotation::GetTextXform().
*/
RHINO_SDK_FUNCTION
bool RhinoCreateTextObjectGeometryByLetter(
  const ON_Annotation* pAnno,
  const ON_Viewport* vp,
  const ON_DimStyle* dimstyle,
  double text_scale,
  bool close_contours,
  double join_tol,
  double small_caps_scale,
  ON::object_type variety,
  double height,
  double spacing,
  ON_ClassArray<ON_SimpleArray<ON_Object*>>& letter_output
);

/*
Description:
  Creates curve outlines from the specified ON_Text object.
Parameters:
  textobj          [in]  - The text object to create outlines from.
  dimstyle         [in]  - The text object's dimensions style.
                           If nullptr, then ON_DimStyle::Default is used.
  close_contours   [in]  - Set this value to true when dealing with normal fonts and
                           when you expect closed loops. You may want to set this to
                           false when specifying a single-stroke font where you don't
                           want closed loops.
  join_tolerance   [in]  - The tolerance used to join contours.
  out_curves       [out] - An array of output curves.
Return:
  true or false indicating success or failure.
NOTE, memory for the output is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoGetTextOutlines(
  const ON_Text* textobj,
  const ON_DimStyle* dimstyle,
  bool close_contours,
  double join_tol,
  ON_SimpleArray<ON_Curve*>& out_curves
);

/*
Description:
  Returns the parameter values of all local extrema.
  Parameter values are in increasing order so consecutive extrema
  define an interval on which each component of the curve is monotone.
  Note, non-periodic curves always return the end points.
Parameters:
  curve          [in]  - The curve to calculate all local extrema.
  dir            [in]  - The direction in which to perform the calculation.
  out_parameters [out] - The parameter values of all local extrema.
Return:
  The number of parameter values added to out_parameters.
*/
RHINO_SDK_FUNCTION
int RhinoCurveExtremeParameters(
  const ON_Curve& curve,
  const ON_3dVector& dir,
  ON_SimpleArray<double>& out_parameters
);

/*
Description:
  Creates a third curve from two curves that are planar in different construction planes.
    The new curve looks the same as each of the original curves when viewed in each plane.
Parameter:
  C0            [in]  - The first curve.
  C1            [in]  - The second curve.
  D0			      [in]  - A vector defining the normal direction of the plane which the first curve is drawn upon.
  D1			      [in]  - A vector defining the normal direction of the plane which the second curve is drawn upon.
  tol			      [in]  - The tolerance for the operation.
  angle_tol	    [in]  - The angle tolerance for the operation.
  output_curves [out] - An array containing one or more curves if successful.
Return:
*/
RHINO_SDK_FUNCTION
bool RhinoCurve2View(
  const ON_Curve& C0,
  const ON_Curve& C1,
  ON_3dVector D0,
  ON_3dVector D1,
  ON_SimpleArray <ON_Curve*>& output_curves,
  double tol,
  double angle_tol
);


enum class RhinoFilletSurfaceType : int
{
  RollingBall = 0,
  Chamfer = 1,
  Rails = 2
};

#pragma region RH_C_SHARED_ENUM [RhinoFilletSurfaceSplitType] [Rhino.Geometry.FilletSurfaceSplitType] [int]
/// <summary>
/// RhinoFilletSurfaceSplitType
/// </summary>
enum class RhinoFilletSurfaceSplitType : int
{
  /// <summary>
  /// Nothing
  /// </summary>
  Nothing = 0,
  /// <summary>
  /// Trim
  /// </summary>
  Trim = 1,
  /// <summary>
  /// Split
  /// </summary>
  Split = 2
};
#pragma endregion

#pragma region RH_C_SHARED_ENUM [RhinoRefitTrimSectionMode] [Rhino.Geometry.RefitTrimSectionMode] [int]
/// <summary>
/// The mode for dividing the surface into sections
/// </summary>
enum class RhinoRefitTrimSectionMode : int
{
  /// <summary>
  /// just output one "upper" and one "lower" surface
  /// </summary>
  NoSections = 0,
  /// <summary>
  /// divide into sections at each of the input surface's existing knots
  /// </summary>
  DivideAtInputKnots = 1,
  /// <summary>
  /// Yes, divide into sections at each of the "nKnot" added knot values
  /// </summary>
  DivideAtAddedKnots = 2,
  /// <summary>
  /// Yes, divide into sections at ALL knot values (i.e. both 1 and 2)
  /// </summary>
  DivideAtAllKnots = 3
};
#pragma endregion


#pragma region RH_C_SHARED_ENUM [RhinoRefitTrimKnotMode] [Rhino.Geometry.RefitTrimKnotMode] [int]
/// <summary>
/// The mode for adding kinots to the surface for fitting
/// </summary>
enum class RhinoRefitTrimKnotMode : int
{
  /// <summary>
  /// No knots are added
  /// </summary>
  None = 0,
  /// <summary>
  /// The entire surface is fit using uniform spacing with the specified number of knots
  /// </summary>
  UniformEntireSurface = 1,
  /// <summary>
  /// Each existing span is uniformly spaced with the specified number of knots
  /// </summary>
  UniformPerSpan = 2,
  /// <summary>
  /// The provided knot vector will be used
  /// </summary>
  Custom = 3
};
#pragma endregion

/*
Description:
  Creates a surface between two surfaces.
Parameters:
  pFace0      - [in] A pointer to the first face.
  u0          - [in] A parameter in the u direction of the first face at the side
                     you want to keep after filleting.
  v0          - [in] A parameter in the v direction of the first face at the side
                     you want to keep after filleting.
  radius0     - [in] The radius, or distance, from the intersection of pFace0 to
                     the edge of the fillet. Note, radius0 should equal radius1
                     unless fillet_type == RhinoFilletSurfaceType::Chamfer, in
                     which case the values can differ.
  pFace1      - [in] A pointer to the second face.
  u1          - [in] A parameter in the u direction of the second face at the side
                     you want to keep after filleting.
  v1          - [in] A parameter in the v direction of the second face at the side
                     you want to keep after filleting.
  radius1     - [in] The radius, or distance, from the intersection of pFace1 to
                     the edge of the fillet. Note, radius0 should equal radius1
                     unless fillet_type == RhinoFilletSurfaceType::Chamfer, in
                     which case the values can differ.
  fillet_type - [in] The type of fillet
  bExtend     - [in] If true, then when one input surface is longer than the other,
                     the fillet surface is extended to the input surface edges.
  split_type  - [in] The split type
  tolerance   - [in] The tolerance. In in doubt, the the document's absolute tolerance.
  OutFillets  - [out] The results of the fillet calculation. NOTE: Memory for the output
                      is allocated and is the responsibility of the caller.
  OutBreps0   - [out] The trim or split results of the Brep owned by pFace0. NOTE: Memory
                      for the output is allocated and is the responsibility of the caller.
  OutBreps1   - [out] The trim or split results of the Brep owned by pFace1. NOTE: Memory
                      for the output is allocated and is the responsibility of the caller.
Returns:
  true if successful, false otherwise.
Remarks:
  The trim or split input Breps are in OutBreps0, and OutBreps1. If the input faces are
  from the same Brep, nothing will be added to OutBreps1. If you specified a split type
  of RhinoFilletSurfaceSplitType::Nothing, then nothing will be added to either OutBreps0
  or OutBreps1.
*/
RHINO_SDK_FUNCTION
bool RhinoFilletSurface(
  const ON_BrepFace* pFace0,
  double u0, double v0,
  double radius0,
  const ON_BrepFace* pFace1,
  double u1, double v1,
  double radius1,
  RhinoFilletSurfaceType fillet_type,
  bool bExtend,
  RhinoFilletSurfaceSplitType split_type,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& OutFillets,
  ON_SimpleArray<ON_Brep*>& OutBreps0,
  ON_SimpleArray<ON_Brep*>& OutBreps1
);

/*
Description:
  Creates a non-rational approximation of an arc as a single bezier segment
Parameters:
  degree      - [in] The degree of the non-rational approximation, can be either 3, 4, or 5
  ptCenter    - [in] The arc center
  ptStart     - [in] A point in the direction of the start point of the arc
  ptEnd       - [in] A point in the direction of the end point of the arc
  radius      - [in] the radius of the arc
  tanSlider   - [in] a number between zero and one which moves the tangent control point toward the midpoint of an equivalent rational arc
  midSlider   - [in] a number between zero and one which moves the mid control points toward the midpoint of an equivalent rational arc
Returns:
  A bezier approximation of a rational arc if successful, null if not
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoNonRationalArcBezier(int degree, ON_3dPoint ptCenter, ON_3dPoint ptStart, ON_3dPoint ptEnd, double radius,
  double tanSlider, double midSlider);

/*
Description:
  Creates a surface between two surfaces, with a fixed rail curve on the first surface.
Parameters:
  pFace0      - [in] A pointer to the first face.
  pRailCurve0 - [in] A pointer to the desired fillet rail curve (3D) on the first face
  pFace1      - [in] A pointer to the second face.
  u1          - [in] A parameter in the u direction of the second face at the side
                     you want to keep after filleting.
  v1          - [in] A parameter in the v direction of the second face at the side
                     you want to keep after filleting.
  railDegree  - [in] Desired fillet degree (3 or 5) in the u-direction, along the rails
  arcDegree   - [in] Desired fillet degree (2, 3, 4, or 5) in the v-direction, along the
                     fillet arcs.  If 2, then the surface is rational in v
  arcSliders  - [in] Array of 2 sliders to shape the fillet in the arc direction,
                     used for arcDegree = 3, 4, or 5; input { 0.0, 0.0 } to ignore
                      [0] (-1 to 1) slides tangent arms from base (-1) to theoretical (1)
                      [1] (-1 to 1) slides inner CV(s) from base (-1) to theoretical (1)
  nBezierSrfs - [in] If >0, this indicates the number of equally-spaced fillet surfaces
                     to be output in the rail direction, each surface Bézier in u.
  bExtend     - [in] If true, then when one input surface is longer than the other,
                     the fillet surface is extended to the input surface edges.
  split_type  - [in] The split type
  tolerance   - [in] The tolerance. In in doubt, the the document's absolute tolerance.
  OutFillets  - [out] The results of the fillet calculation. NOTE: Memory for the output
                      is allocated and is the responsibility of the caller.
  OutBreps0   - [out] The trim or split results of the Brep owned by pFace0. NOTE: Memory
                      for the output is allocated and is the responsibility of the caller.
  OutBreps1   - [out] The trim or split results of the Brep owned by pFace1. NOTE: Memory
                      for the output is allocated and is the responsibility of the caller.
  fitResults  - [out] array of doubles indicating fitting results:
                      [0] max 3d point deviation along surface 0
                      [1] max 3d point deviation along surface 1
                      [2] max angle deviation along surface 0 (in degrees)
                      [3] max angle deviation along surface 1 (in degrees)
                      [4] max angle deviation between Bézier surfaces (in degrees)
                      [5] max  curvature difference between Bézier surfaces
Returns:
  true if successful, false otherwise.
Remarks:
  The trim or split input Breps are in OutBreps0, and OutBreps1. If the input faces are
  from the same Brep, nothing will be added to OutBreps1. If you specified a split type
  of RhinoFilletSurfaceSplitType::Nothing, then nothing will be added to either OutBreps0
  or OutBreps1.
*/
RHINO_SDK_FUNCTION
bool RhinoFilletSurfaceToRail(
  const ON_BrepFace* pFace0,
  const ON_Curve* pRailCurve0,
  const ON_BrepFace* pFace1,
  double u1, double v1,
  int railDegree,
  int arcDegree,
  double* arcSliders,
  int nBezierSrfs,
  bool bExtend,
  RhinoFilletSurfaceSplitType split_type,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& out_fillets,
  ON_SimpleArray<ON_Brep*>& out_breps0,
  ON_SimpleArray<ON_Brep*>& out_breps1,
  double* fitResults
);

/*
Description:
  Creates a non-rational constant-radius fillet surface between two surfaces.
Parameters:
  pFace0      - [in] A pointer to the first face.
  u0          - [in] A parameter in the u direction of the first face at the side
                     you want to keep after filleting.
  v0          - [in] A parameter in the v direction of the first face at the side
                     you want to keep after filleting.
  pFace1      - [in] A pointer to the second face.
  u1          - [in] A parameter in the u direction of the second face at the side
                     you want to keep after filleting.
  v1          - [in] A parameter in the v direction of the second face at the side
                     you want to keep after filleting.
  radius      - [in] The radius of the constant-radius fillet desired;
                     NOTE: using arcSliders will change the shape of the arcs themselves
  railDegree  - [in] Desired fillet degree (3 or 5) in the u-direction, along the rails
  arcDegree   - [in] Desired fillet degree (3, 4, or 5) in the v-direction, along arcs
  arcSliders  - [in] array of 2 sliders to shape the fillet in the arc direction;
                     input { 0.0, 0.0 } to ignore
                      [0] (-1 to 1) slides tangent arms from base (-1) to theoretical (1)
                      [1] (-1 to 1) slides inner CV(s) from base (-1) to theoretical (1)
  nBezierSrfs - [in] If >0, this indicates the number of equally-spaced fillet surfaces
                     to be output in the rail direction, each surface Bézier in u.
  fillet_type - [in] The type of fillet
  bExtend     - [in] If true, then when one input surface is longer than the other,
                     the fillet surface is extended to the input surface edges.
  split_type  - [in] The split type
  tolerance   - [in] The tolerance. In in doubt, the the document's absolute tolerance.
  OutFillets  - [out] The results of the fillet calculation. NOTE: Memory for the output
                      is allocated and is the responsibility of the caller.
  OutBreps0   - [out] The trim or split results of the Brep owned by pFace0. NOTE: Memory
                      for the output is allocated and is the responsibility of the caller.
  OutBreps1   - [out] The trim or split results of the Brep owned by pFace1. NOTE: Memory
                      for the output is allocated and is the responsibility of the caller.
  fitResults  - [out] array of doubles indicating fitting results:
                      [0] max 3d point deviation along surface 0
                      [1] max 3d point deviation along surface 1
                      [2] max angle deviation along surface 0 (in degrees)
                      [3] max angle deviation along surface 1 (in degrees)
                      [4] max angle deviation between Bézier surfaces (in degrees)
                      [5] max  curvature difference between Bézier surfaces
Returns:
  true if successful, false otherwise.
Remarks:
  The trim or split input Breps are in OutBreps0, and OutBreps1. If the input faces are
  from the same Brep, nothing will be added to OutBreps1. If you specified a split type
  of RhinoFilletSurfaceSplitType::Nothing, then nothing will be added to either OutBreps0
  or OutBreps1.
*/
RHINO_SDK_FUNCTION
bool RhinoFilletSurfaceNonRational(
  const ON_BrepFace* pFace0,
  double u0, double v0,
  const ON_BrepFace* pFace1,
  double u1, double v1,
  double radius,
  int railDegree,
  int arcDegree,
  double* arcSliders,
  int nBezierSrfs,
  bool bExtend,
  RhinoFilletSurfaceSplitType split_type,
  double tolerance,
  ON_SimpleArray<ON_Brep*>& out_fillets,
  ON_SimpleArray<ON_Brep*>& out_breps0,
  ON_SimpleArray<ON_Brep*>& out_breps1,
  double* fitResults
);

/*
Description:
  Creates a constant-radius fillet surface between a surface and a curve.
Parameters:
  pCurve      - [in] A pointer to the free-standing 3D curve being filleted.
  t           - [in] A parameter on the curve, indicating region of fillet.
  pFace       - [in] A pointer to the face being filleted.
  u           - [in] A parameter in the u direction of the face indicating which
                     side of the curve to fillet.
  v           - [in] A parameter in the v direction of the face indicating which
                     side of the curve to fillet.
  radius      - [in] The radius of the constant-radius fillet desired;
                     NOTE: using arcSliders will change the shape of the arcs themselves
  alignToCrv  - [in] Does the user want the fillet to align to the curve?
                      0 - No, ignore the curve's b-spline structure
                      1 - Yes, match the curves's degree, spans, CVs as much as possible
                      2 - Same as 1, but iterate to fit to tolerance
                     Note that a value of 1 or 2 will cause nBezierSrfs to be ignored
  railDegree  - [in] Desired fillet degree (3 or 5) in the u-direction, along the curve
  arcDegree   - [in] Desired fillet degree (2, 3, 4, or 5) in the v-direction, along the
                     fillet arcs.  If 2, then the surface is rational in v
  arcSliders  - [in] Array of 2 sliders to shape the fillet in the arc direction,
                     used for arcDegree = 3, 4, or 5; input { 0.0, 0.0 } to ignore
                      [0] (-1 to 1) slides tangent arms from base (-1) to theoretical (1)
                      [1] (-1 to 1) slides inner CV(s) from base (-1) to theoretical (1)
  nBezierSrfs - [in] If >0, this indicates the number of equally-spaced fillet surfaces
                     to be output in the rail direction, each surface Bézier in u.
  tolerance   - [in] The tolerance. In in doubt, the the document's absolute tolerance.
  OutFillets  - [out] The results of the fillet calculation. NOTE: Memory for the output
                      is allocated and is the responsibility of the caller.
  fitResults  - [out] array of doubles indicating fitting results:
                      [0] max 3d point deviation along curve
                      [1] max 3d point deviation along face
                      [2] max angle deviation along face (in degrees)
                      [3] max angle deviation between Bézier surfaces (in degrees)
                      [4] max  curvature difference between Bézier surfaces
Returns:
true if successful, false otherwise.
Remarks:
Face trimming, splitting not yet implemented
*/
RHINO_SDK_FUNCTION
bool RhinoFilletSurfaceCurve(
  const ON_Curve* pCurve,
  double t,
  const ON_BrepFace* pFace,
  double u, double v,
  double radius,
  int alignToCurve,
  int railDegree,
  int arcDegree,
  double* arcSliders,
  int nBezierSrfs,
  double tolerance,
  ON_SimpleArray<ON_NurbsSurface*>& out_fillets,
  double* fitResults
);

/*
Description:
  Rounds the corners of a kinked curve with arcs of a single, specified radius.
Parameters:
  curve   - [in] The curve to fillet.
  radius  - [in] The fillet radius.
  tol     - [in] The tolerance. When in doubt, use the document's model space absolute tolerance.
  ang_tol - [in] The angle tolerance in radians. When in doubt, use the document's model space angle tolerance.
Returns:
  The modified curve if successful, NULL otherwise.
Remarks:
  Memory for the result curve is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoFilletCurveCorners(
  const ON_Curve* curve,
  double radius,
  double tol,
  double ang_tol
);

/*
Description: Creates a standard surface fillet using rational arc sections
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */
RHINO_SDK_FUNCTION bool RhinoCreateRationalArcsFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);

/*
Description: Creates a surface fillet using Non-rational Quintic arc approximations as sections
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */
RHINO_SDK_FUNCTION bool RhinoCreateNonRationalQuinticArcsFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);//Arc approximation

/*
Description: Creates a fillet using non-rational Quintic sections with a tangent and inner slider
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  TanSlider - [in] A number between -0.95 and 0.95 indicating how far to push the tangent control points toward or away from the theoretical
    quadratic middle control point
  InnerSlider - [in] A number between -0.95 and 0.95 indicating how far to push the inner control points toward or away from the theoretical
    quadratic middle control point
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */

RHINO_SDK_FUNCTION bool RhinoCreateNonRationalQuinticFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, double TanSlider, double InnerSlider, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);

/*
Description: Creates a surface fillet using Non-rational Quartic arc approximations as sections
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */
RHINO_SDK_FUNCTION bool RhinoCreateNonRationalQuarticArcsFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);//Arc approximation

/*
Description: Creates a fillet using non-rational Quartic sections with a tangent and inner slider
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  TanSlider - [in] A number between -0.95 and 0.95 indicating how far to push the tangent control points toward or away from the theoretical
    quadratic middle control point
  InnerSlider - [in] A number between -0.95 and 0.95 indicating how far to push the inner control point toward or away from the theoretical
    quadratic middle control point
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */

RHINO_SDK_FUNCTION bool RhinoCreateNonRationalQuarticFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, double TanSlider, double InnerSlider, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);

/*
Description: Creates a surface fillet using Non-rational Cubic arc approximations as sections
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */
RHINO_SDK_FUNCTION bool RhinoCreateNonRationalCubicArcsFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);//Arc approximation

/*
Description: Creates a fillet using non-rational Cubic sections with a tangent slider
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  TanSlider - [in] A number between -0.95 and 0.95 indicating how far to push the tangent control points toward or away from the theoretical
    quadratic middle control point
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */
RHINO_SDK_FUNCTION bool RhinoCreateNonRationalCubicFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, double TanSlider, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);

/*
Description: Creates a surface fillet using G2 chordal Quintic arc approximations as sections
Parameters :
  FaceA - [in] the first face to use constructing the fillet
  uvA - [in] The parametric u,v selection point on FaceA close to the edge to fillet
  FaceB - [in] the second face to use constructing the fillet
  uvB - [in] The parametric u,v selection point on FaceB close to the edge to fillet
  radius - [in] The radius of the fillet
  tol - [in] Tolerance to use in fitting a solution
  resultsA - [out] if bTrim = true, returns the remains of FaceA trimmed to the fillet
  resultsB - [out] if bTrim = true, returns the remains of FaceB trimmed to the fillet
  rail_degree - [in] the degree of the rail curve
  bTrim - [in] if True, trim the faces and retuen those results in resultsA and resultsB
  bExtend - [in] if True and if one input surface is longer than the other, the fillet surface is extended to the input surface edges
  Fillets - [out] returns the resulting fillet surfaces
  Returns :
  True if successful.
  Remarks :
  */
RHINO_SDK_FUNCTION bool RhinoCreateG2ChordalQuinticFilletSrf(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
  const ON_BrepFace& FaceB, const ON_2dPoint& uvB, double radius, double tolerance,
  ON_SimpleArray<ON_Brep*>& resultsA, ON_SimpleArray<ON_Brep*>& resultsB,
  int rail_degree, bool bTrim, bool bExtend,ON_SimpleArray<ON_Brep*>& Fillets);

/*
Description:
  Soft edit of a curve from a parameter on curve using smooth field of influence.
Parameters:
  curve    - [in] The curve to soft edit.
  curve_t  - [in] A parameter on the curve to move from. This location on the curve is
                  moved, and the move is smoothly tapered off with increasing distance
                  along the curve from this parameter.
  delta    - [in] The direction and magnitude, or maximum distance, of the move.
  length   - [in] The distance along the curve from the editing point over which the
                  strength of the editing falls off smoothly.
  bFixEnds - [in] Keeps the position of the curve ends fixed.
Returns:
  The modified curve if successful, NULL otherwise.
Remarks:
  Memory for the result curve is allocated and becomes the responsibility of the caller.
  */
RHINO_SDK_FUNCTION
ON_Curve* RhinoSoftEditCrv(
  const ON_Curve* curve,
  double curve_t,
  const ON_3dVector& delta,
  double length,
  bool bFixEnds
);

/*
Description:
  Soft edit of a surface from a uv parameter using smooth field of influence
Parameters:
  surface   - [in] The surface to soft edit.
  uv        - [in] A point in the parameter space to move from. This location on the
                   surface is moved, and the move is smoothly tapered off with increasing
                   distance along the surface from this parameter.
  delta     - [in] The direction and magnitude, or maximum distance, of the move.
  u_length  - [in] The distance along the surface's u-direction from the editing point over
                   which the strength of the editing falls off smoothly.
  v_length  - [in] The distance along the surface's v-direction from the editing point over
                   which the strength of the editing falls off smoothly.
  tolerance - [in] The active document's model absolute tolerance.
  bFixEnds  - [in] Keeps edge locations fixed.
Returns:
  The modified surface if successful, NULL otherwise.
Remarks:
  Memory for the result surface is allocated and becomes the responsibility of the caller.
  */
RHINO_SDK_FUNCTION
ON_Surface* RhinoSoftEditSrf(
  const ON_Surface* surface,
  const ON_2dPoint& uv,
  const ON_3dVector& delta,
  double u_length,
  double v_length,
  double tolerance,
  bool bFixEnds
);


enum class RhinoConicSectionType : int
{
  Unknown = 0,
  Circle = 1,
  Ellipse = 2,
  Hyperbola = 3,
  Parabola = 4
};

/*
Description:
  Determines if a curve is a conic section based on the curve's shape.
Parameters:
  curve   - [in] The curve to evaluate.
  focus1  - [out] The point at the conic's first focus.
  focus2  - [out] The point at the conic's second focus.
  center  - [out] The point at the center of the conic.
Returns:
  The type of conic section.
*/
RHINO_SDK_FUNCTION
RhinoConicSectionType RhinoIsCurveConicSection(
  const ON_Curve* curve,
  ON_3dPoint* focus1,
  ON_3dPoint* focus2,
  ON_3dPoint* center
);

RHINO_SDK_FUNCTION void RhResetAllMessageBoxes();

/*
Description:
  Returns the surface draft angle and point at a parameter.
Parameters:
  face        - [in] The Brep face to evaluate.
  uv          - [in] The u,v parameter on the face to evaluate.
  test_angle  - [in] The angle in radians to test.
  direction   - [in] The pull direction.
  bEdge       - [in] Restricts the point placement to an edge.
  draft_point - [out] The draft angle point.
  draft_angle - [out] The draft angle in radians.
Returns:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoSurfaceDraftAnglePoint(
  const ON_BrepFace& face,
  const ON_2dPoint& test_point,
  double test_angle,
  const ON_3dVector& direction,
  bool bEdge,
  ON_3dPoint& draft_point,
  double* draft_angle
);

/*
Description:
  Splits all of the faces of a Brep at tangent locations.
Parameters:
  brep - [in/out] The Brep.
Returns:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoSplitFacesAtTangents(ON_Brep& brep);

/*
Description:
  Splits a face of a Brep at tangent locations.
Parameters:
  brep       - [in/out] The Brep.
  face_index - [in] The index of the Brep face to split.
Returns:
  True if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoSplitFaceAtTangents(ON_Brep& brep, int face_index);

/*
Description:
  Returns the serial number of the computer running Rhino
Returns:
  ON_wString containing computer serial number
*/
RHINO_SDK_FUNCTION
ON_wString RhinoComputerSerialNumber();

/*
Description:
  Transform a list of Brep components, bend neighbors to match, and leave the rest fixed.
Parameters:
  brep             - [in] The Brep to modify
  component_count  - [in] Length of brep_components[] array
  brep_components  - [in] Components to transform.
  xform            - [in] Transformation to apply to components
  tolerance        - [in] Desired fitting tolerance to use when bending
                          faces that share edges with both fixed and
                          transformed components.
  time_limit       - [in] If the deformation is extreme, it can take a long time to
                          calculate the result. If time_limit > 0, then the value
                          specifies the maximum amount of time in seconds you want
                          to spend before giving up.
  bMultipleThreads - [in] True if multiple threads can be used.
Returns:
  True if successful.
*/
RHINO_SDK_FUNCTION
bool RhinoTransformBrepComponents(
  ON_Brep* brep,
  int brep_component_count,
  const ON_COMPONENT_INDEX* brep_components,
  ON_Xform xform,
  double tolerance,
  double time_limit,
  bool bUseMultipleThreads
);

/*
Description:
  Converts a curve into polycurve consisting of arc segments.
  Sections of the input curves that are nearly straight are
  converted to straight-line segments.
Parameters:
  pCurve     - [in] the curve to convert.
  dTol       - [in] maximum deviation from arc midpoints to curve.
  dAngleTol  - [in] maximum deviation of the arc end directions from the curve direction in radians
  dMinLength - [in] minimum segment length
  dMaxLength - [in] maximum segment length
Returns:
  Pointer to curve if success, nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_PolyCurve* RhinoConvertCurveToArcs(
  const ON_Curve* pCurve,
  const double dTol,
  const double dAngleTol,
  const double dMinLength,
  const double dMaxLength
);

/*
Description:
  Converts a curve into polycurve consisting of line segments.
Parameters:
  pCurve     - [in] the curve to convert.
  dTol       - [in] maximum deviation from line midpoints to curve.
  dAngleTol  - [in] maximum deviation of the line directions in radians
  dMinLength - [in] minimum segment length
  dMaxLength - [in] maximum segment length
Returns:
  Pointer to curve if success, nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_PolyCurve* RhinoConvertCurveToLines(
  const ON_Curve* pCurve,
  const double dTol,
  const double dAngleTol,
  const double dMinLength,
  const double dMaxLength
);

/*
Description:
  Remove all inner loops (holes) in a Brep.
Parameters:
  brep      - [in] the Brep to convert.
  tolerance - [in] tolerance.
Returns:
  Pointer to Brep if success, nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepRemoveAllInnerLoops(
  const ON_Brep& brep,
  double tolerance
);

/*
Description:
  Remove all inner loops (holes) in a Brep face.
Parameters:
  brep      - [in] the Brep to convert.
  face_ci   - [in] face component index
  tolerance - [in] tolerance.
Returns:
  Pointer to Brep if success, nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepFaceRemoveInnerLoops(
  const ON_Brep& brep,
  ON_COMPONENT_INDEX face_ci,
  double tolerance);

/*
Description:
  Remove a list of inner loops.
Parameters:
  brep      - [in] the Brep to convert.
  loops_ci  - [in] list of loops to remove
  tolerance - [in] tolerance.
Returns:
  Pointer to Brep if success, nullptr if unsuccessful.
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepRemoveInnerLoops(
  const ON_Brep& brep,
  ON_SimpleArray<ON_COMPONENT_INDEX> loops_ci,
  double tolerance
);

/*
Description:
  Create parabola from vertex and ends.
Parameters:
  vertex - [in] parabola vertex
  start  - [in] parabola start
  end    - [in] parabola end
  parabola_out - [out] degree 2 Bézier curve defining the parabola.
Returns:
  true if success
*/
RHINO_SDK_FUNCTION
bool RhinoCreateParabolaFromVertex(
  const ON_3dPoint& vertex,
  const ON_3dPoint& start,
  const ON_3dPoint& end,
  ON_BezierCurve& parabola_out
);

/*
Description:
  Create parabola from focus and ends.
Parameters:
  focus - [in] parabola focus
  start - [in] parabola start
  end   - [in] parabola end
  parabola_out - [out] degree 2 Bézier curve defining the parabola.
Returns:
 true if success
*/
RHINO_SDK_FUNCTION
bool RhinoCreateParabolaFromFocus(
  const ON_3dPoint& focus,
  const ON_3dPoint& start,
  const ON_3dPoint& end,
  ON_BezierCurve& parabola_out
);

/*
Description:
  Create parabola from three points.
Parameters:
  focus - [in] parabola start
  start - [in] parabola interior point
  end   - [in] parabola end
  curve_out - [out] degree 2 NURBS curve defining the parabola.
Returns:
 true if success
*/
RHINO_SDK_FUNCTION
bool RhinoCreateParabolaFromPoints(
  const ON_3dPoint& P0,
  const ON_3dPoint& P1,
  const ON_3dPoint& P2,
  ON_NurbsCurve& curve_out
);

/*
Description:
  Get a Brep definition of a sphere.
Parameters:
  center    - [in] Center of sphere
  radius    - [in] Radius of sphere
  tolerance - [in] Used in computing 2d trimming curves.
                   If <= 0.0, then max of 0.0001 * radius and ON_ZERO_TOLERANCE will be used.
  pBrep     - [in] If not nullptr, this Brep will be used and returned.
                   If not nullptr and creation failed, nullptr will be
                   returned and *pBrep will be an empty Brep.
Returns:
  A Brep representation of the sphere with two similar trimmed NURBS surfaces
  and no singularities. or nullptr if failure.
Remarks:
  NOTE: Memory for the Brep is allocated and becomes the responsibility of the caller.
  */
RHINO_SDK_FUNCTION
ON_Brep* RhinoBrepBaseballSphere(
  const ON_3dPoint& center,
  double radius,
  double tolerance,
  ON_Brep* pBrep = nullptr
);

/*
Description:
  For expert use only.
  From the input curves, make an array of compatible NURBS curves.
  Useful as input shapes for Loft, Sweep, etc.
*/
RHINO_SDK_FUNCTION
bool RhinoMakeCompatibleNurbs(
  ON_SimpleArray<const ON_Curve*>& curves,
  ON_3dPoint* startpt,
  ON_3dPoint* endpt,
  int simplify_method,
  int num_elmo_points,
  double refit_tolerance,
  double angle_tolerance_radians,
  ON_SimpleArray<ON_NurbsCurve*>& compatiblecurves
);

/*
Description:
  For expert use only.
  Makes a pair of compatible NURBS surfaces based on two input surfaces.
*/
RHINO_SDK_FUNCTION
bool RhinoMakeCompatibleNurbsSurfaces(
  const ON_Surface& S0,
  const ON_Surface& S1,
  ON_NurbsSurface& C0,
  ON_NurbsSurface& C1
);

/*
Description:
  Sort a list of points so they will be connected in "reasonable polyline" order.
Parameters:
  points  - [in/out] list of 3d points that is sorted to be in reasonable polyline order.
  mindist - [in]  minimum distance between points to use. Throw out ones closer than this.
Returns:
  true if it works.
*/
RHINO_SDK_FUNCTION
bool RhinoSortPointList(
  ON_SimpleArray<ON_3dPoint>& points,
  double mindist = ON_ZERO_TOLERANCE
);

/*
Description:
  Sort a list of points so they will be connected in "reasonable polyline" order.
Parameters:
  point_count - [in] number of points
  points      - [in] array of point_count 3d points
  order       - [out] sorted list of indices so that
                      points[order[0]], points[order[1]], , points[order[n]]
                      will be a reasonable polyline.
  mindist     - [in]  minimum distance between points to use. Throw out ones closer than this.
Returns:
  true if it works.
*/
RHINO_SDK_FUNCTION
bool RhinoSortPointList(
  int point_count,
  const ON_3dPoint* points,
  int* order,
  double mindist
);

/*
Description:
  Sort a list of points so they will be connected in "reasonable polyline" order.
Parameters:
  point_count - [in] number of points
  points      - [in] array of point_count 2d points
  order       - [out] sorted list of indices so that
                      points[order[0]], points[order[1]], , points[order[n]]
                      will be a reasonable polyline.
  mindist     - [in]  minimum distance between points to use. Throw out ones closer than this.
Returns:
  true if it works.
*/
RHINO_SDK_FUNCTION
bool RhinoSortPointList(
  int point_count,
  const ON_2dPoint* points,
  int* order,
  double mindist
);

/*
Description:
  Creates a polycurve consisting of two tangent arc segments that connect two points and two directions.
Parameters:
  startpt   - [in] start of the arc blend curve
  startdir  - [in] start direction of the arc blend curve
  endpt     - [in] end of the arc blend curve
  enddir    - [in] end direction of the arc blend curve
  controlpolygonlengthratio - [in] the ratio of the control polygon lengths of the two arcs
  curve_out - [out] optional curve out. If nullptr a new curve is created.
Returns:
  polycurve consisting of two arc segments if success, nullptr otherwise
*/
RHINO_SDK_FUNCTION
ON_PolyCurve* RhinoCreateArcBlend(
  const ON_3dPoint& startpt,
  const ON_3dVector& startdir,
  const ON_3dPoint& endpt,
  const ON_3dVector& enddir,
  const double controlpolygonlengthratio = 1.0,
  ON_PolyCurve* curve_out = nullptr
);

/*
Description:
  Detailed tool to compute 'splines' on a surface.
  This function creates a discrete 'spline' curve on the surface, i.e. a sequence
  of point on the surface each with a corresponding parameter value.
  The curve interpolates a sequence of points on the surface.
Parameters:
  Srf        - [in] The surface on which the curve is constructed.
  Fixed2     - [in] Surface points to interpolate given by parameters. These must be distinct.
  tol        - [in] Relative tolerance used in solver.  Use 0 for default value.
  Periodic   - [in] Construct a smoothly closed curve when true.
                    Fixed2[0]==Fixed2.Last() (at least up to addition of a period)
  init_count - [in] Maximum number of points to insert between fixed points on first level.
  nlevels    - [in] Number of levels to be used in multilevel solver.
  out        - [out] Sequence of surface points (given by surface parameters).
  t          - [out] Curve parameter associated with each out value, where: out.Count()==t.Count()
  pIndex     - [out] If not null, then out[(*pIndex)[i]]==Fixed2[i]
Remarks:
  Srf should be G1 continuous. If Srf is closed in the u or the v direction and
  is G1 at the seam then the function can construct point sequences that
  cross over the seam. In this case the surface parameter space points in Fixed2 and
  out are in the covering space of Srf.Domain, i.e they extend across the domain
  boundaries of Srf.Domain(0) in the the sense of a natural periodic extension.
  (See ON_PeriodicDomain for more information).
  Also, the number of out points is approximately 2^(nlevel-1)*init_count*Fixed2.Count()
*/
RHINO_SDK_FUNCTION bool
RhinoCurveOnSurfacePoints(
  const ON_Surface& Srf,
  const ON_SimpleArray<ON_2dPoint>& Fixed2,
  double tol,
  bool Periodic,
  int init_count,
  int nlevels,
  ON_SimpleArray<ON_2dPoint>& out,
  ON_SimpleArray<double>& t,
  ON_SimpleArray<int>* pIndex = nullptr
);


RHINO_SDK_FUNCTION bool
RhinoCurveOnSurfacePoints(
  const ON_Surface& Srf,
  const ON_SimpleArray<ON_2dPoint>& Fixed2,
  const ON_SimpleArray<double>& Dist,
  const ON_ClassArray<ON_SimpleArray<ON_2dPoint> >& Geo,
  double tol,
  bool Periodic,
  int init_count,
  int nlevels,
  ON_SimpleArray<ON_2dPoint>& out,
  ON_SimpleArray<double>& t,
  ON_SimpleArray<int>* pIndex
);

/*
Description:
  Fit a sequence of 3d points on a surface to make a curve on the surface.
Parameters:
  srf      - [in] Surface on which to construct curve.
  points   - [in] Parameter space coordinates of the points to interpolate. If srf is closed
                  the points can be in the domain covering space of srf (See ON_PeriodicDomain).
  tol      - [in] Curve should be within tolerance of surface and points. (srf.Evaluate(points[i])
  Periodic - [in] When true make a periodic curve.
Returns:
  A curve interpolating the points.
*/
RHINO_SDK_FUNCTION ON_NurbsCurve*
RhinoFitPointsOnSurface(
  const ON_Surface& srf,
  const ON_SimpleArray<ON_2dPoint>& points,
  double tol,
  bool Periodic
);

/*
Description:
  Expert function to compute CurveOnSurfacePoints using a multilevel solver.
  On input out, index and t provide a rough approximation of the curve.
Parameters:
  Srf -[in]				Surface on which curve lies
  out- [in/out]		on input out is a rough approximation of the curve including all interpolation points.
                  On output out is a discrete version of the curve on surface. On output
                  out.Count() = 2^Num_Multilevels*( n_0 - 1) + 1 where n0 = out<.Count() at input.
  index -[in/out] The fixed interpolation values are identified by index.  On input
                  index.Count()<=out.Count() and the count of index is the same on output as on input.
                  on both input and output index is an increasing list of indices in the range 0 to out.Count()-1.
                  Furthermore index[0]=0 and index.Last()==out.Count()-1.
                  Srf.Evaluate( out[index[j]] ) is a surface point to be interpolated and this value is the same
                  on input as on output.
  t -[in/out] -		on input t is an increasing list of parameters. t[i] is the parameter corresponding
                  to the surface point Srf.Evaluate( out[i] ).  t.Count()==out.Count().
                  on output t is an increasing list of parameters t.Count()=out.Count().
                  The first and last value in t are the same as they were in the input list.
                  t is a refinement of the list at input.
  tol -[in] -			An array of Num_level tolerance values. tol[0] is used for the top coarsest level etc..
                  set all tol values to 0 for default values.
  Periodic -[in]  Set to true to generate a periodic curve instead of an open curve.
                  If true then out.First() == out.Last() mod domain.
  Num_Levels -[in] Number of levels in multilevel solver.  Use Num_Level=1 for single level solve, in this
                  case out.Count() doesn't change.  Use sparingly probably should have Num_Level<4.
                  Remarks:
  If Srf is G1-closed in one or both directions the points in out may be taken in a covering space of Srf.Domain.
  See ON_PeriodicDomain.
*/
RHINO_SDK_FUNCTION bool
RhinoCurveOnSurfaceSolver(
  const ON_Surface& Srf,
  ON_SimpleArray<ON_2dPoint>& out,
  ON_SimpleArray<int>& Index,
  ON_SimpleArray<double>& t,
  ON_SimpleArray<double>& tol,
  bool Periodic,
  int Num_Levels
);

/*
Description:
  Copies a surface so that all locations at the corners of the copied surface
  are specified distances from the original surface. Optionally, you can specify
  interior locations and their offset distances. If the uv and dist arrays are
  empty, then the distance at any interior point will be found by doing a bilinear
  interpolation of the corner distances.
Parameters:
  InputSrf     - [in] The surface to offset.
  corner_dists - [in] The offset distances as the corners.
  uv           - [in] An array of surface interior uv parameters to offset at.
                      The count of this array much match that of the dists array.
  dists        - [in] An array of offset distances at the interior uv parameters.
                      The count of this array much match that of the uv array.
  tolerance    - [in] The tolerance used when offsetting.
Returns:
  A pointer to the offset surface if successful.
Remarks:
  NOTE: Memory for the surface is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoVariableOffsetSurface(
  const ON_Surface& InputSrf,
  const double corner_dists[2][2], // [umin,umax][vmin,vmax]
  const ON_SimpleArray<ON_2dPoint>& uv, // Not at the corners, all different 
  const ON_SimpleArray<double>& dists,
  double tolerance
);


typedef long long RhTimestamp;

/*
Description:
  Returns a high-resolution timestamp that can be used for time-interval measurements.
Returns:
  A timestamp object.
*/
RHINO_SDK_FUNCTION
RhTimestamp RhinoGetTimestamp();

/*
Description:
  Calculates the time in seconds between the two provided timestamps.
Returns:
  Time in seconds between 'start_time' and 'stop_time'.
Remarks:
  Use RhinoGetTimestamp() to get a timestamp.
*/
RHINO_SDK_FUNCTION
double RhinoGetTimeInSecondsBetween(const RhTimestamp& start_time, const RhTimestamp& stop_time);

/*
Description:
  Calculates the time in seconds between now and the provided timestamp.
Returns:
  Time in seconds between now and 'timestamp'.
Remarks:
  Use RhinoGetTimestamp() to get a timestamp.
*/
RHINO_SDK_FUNCTION
double RhinoGetTimeInSecondsSince(const RhTimestamp& timestamp);


/*
Description:
  Returns a NURBS curve's edit, or Greville, points.
Parameters:
  in_curve   - [in] The input NURBS curve.
  out_points - [out] The edit points.
Returns:
  The number of points added to out_points.
*/
RHINO_SDK_FUNCTION
int RhinoGetGrevillePoints(
  const ON_NurbsCurve& in_curve,
  ON_SimpleArray<ON_3dPoint>& out_points
);

/*
Description:
  Modifies a NURBS curve's edit, or Greville, points.
Parameters:
  pNurbsCurve - [in] The input NURBS curve.
  point_count - [in] The number of points in the array. The number of points should match
                     the number of point returned by RhinoGetGrevillePoints(in_curve).
  in_points   - [in] The array of points.
  out_curve   - [in] If not nullptr, this curve will be used and returned.
Returns:
  Pointer to the modified curve if successful, or nullptr on failure
  Note: if out_curve == nullptr, then memory will be allocated for the
  returned curve and the caller is responsible for deleting it when finished.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoSetGrevillePoints(
  const ON_NurbsCurve& in_curve,
  int point_count,
  const ON_3dPoint* in_points,
  ON_NurbsCurve* out_curve = nullptr
);

/*
Description:
  Create a surface by extruding a smooth, G1 curve a distance along an axis with
  draft angle. The end of the extrusion will be on a plane parallel to
  ReferenceDir, a distance of dist from ReferencePoint.
Parameters:
  Crv            - [in] The smooth, G1 curve to extrude.
  ExtrudeDir     - [in] The extrusion direction.
  dist           - [in] The extrusion distance.
  draft          - [in] The extrusion draft angle.
  ReferencePoint - [in] The reference point, generally be a point on the curve.
  ReferenceDir   - [in] The reference direction. For planar curves, a natural choice
                        for ReferenceDir will be the plane's normal. In any case,
                        ReferenceDir = ExtrudeDir may make sense.
Returns:
  A surface if successful, nullptr otherwise.
Remarks;
  If Crv is not G1, nullptr will be returned. Use RhinoTaperedExtrudeCurveWithRef instead.
  NOTE: Memory for the surface is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_Surface* RhinoTaperedExtrudeSmoothCurveWithRef(
  const ON_Curve& Crv,
  const ON_3dVector& ExtrudeDir,
  double dist,
  double draft,
  const ON_3dPoint& ReferencePoint,
  const ON_3dVector& ReferenceDir
);

/*
Description:
  Create a polysurface by extruding a curve a distance along an axis with
  draft angle. The end of the extrusion will be on a plane parallel to
  ReferenceDir, a distance of dist from ReferencePoint.
Parameters:
  Crv            - [in] The curve to extrude.
  ExtrudeDir     - [in] The extrusion direction.
  dist           - [in] The extrusion distance.
  draft          - [in] The extrusion draft angle.
  ReferencePoint - [in] The reference point, generally be a point on the curve.
  ReferenceDir   - [in] The reference direction. For planar curves, a natural choice
                        for ReferenceDir will be the plane's normal. In any case,
                        ReferenceDir = ExtrudeDir may make sense.
  ExtrusionSrfs  - [out] The results of the extrusion.
Returns:
  true if successful, false otherwise.
Remarks;
  The result will have a face for each G1 section of Crv, and may be disjoint.
  NOTE: Memory for the surfaces is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
bool RhinoTaperedExtrudeCurveWithRef(
  const ON_Curve& Crv,
  const ON_3dVector& ExtrudeDir,
  double dist,
  double draft,
  const ON_3dPoint& ReferencePoint,
  const ON_3dVector& ReferenceDir,
  ON_SimpleArray<ON_Surface*>& ExtrusionSrfs
);

RHINO_SDK_FUNCTION
bool RhinoTaperedExtrudeCurveWithRef(
  const ON_Curve& Crv,
  const ON_3dVector& ExtrudeDir,
  double dist,
  double draft,
  const ON_3dPoint& ReferencePoint,
  const ON_3dVector& ReferenceDir,
  double tolerance,//For intersecting and trimming
  ON_SimpleArray<ON_Brep*>& Extrusions
);

/*
Description:
  Contracts the underlying untrimmed surface of a Brep face right to the to the trimming boundaries.
  Note, shrinking the trimmed surface can sometimes cause problems later since having
  the edges so close to the trimming boundaries can cause commands that use the surface
  edges as input to fail.
Parameters:
  brep - [in] The Brep.
  face_index- [in] The index of the Brep face.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoBrepShrinkSurfaceToEdge(
  ON_Brep& brep,
  int face_index
);


/*
Description:
  Calculates the U, V, and N directions of a NURBS curve at a parameter
  similar to the method used by the MoveUVN command.
Parameters:
  nc    - [in] The NURBS curve to evaluate.
  t     - [in] The evaluation parameter.
  u_dir - [out] The u direction.
  v_dir - [out] The v direction.
  n_dir - [out] The n direction.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoNurbsCurveDirectionsAt(
  const ON_NurbsCurve& nc,
  double t,
  ON_3dVector& u_dir,
  ON_3dVector& v_dir,
  ON_3dVector& n_dir
);

/*
Description:
  Calculates the U, V, and N directions of a NURBS surface at a u,v parameter
  similar to the method used by the MoveUVN command.
Parameters:
  ns    - [in] The NURBS surface to evaluate.
  u     - [in] The u evaluation parameter.
  v     - [in] The v evaluation parameter.
  u_dir - [out] The u direction.
  v_dir - [out] The v direction.
  n_dir - [out] The n direction.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoNurbsSurfaceDirectionsAt(
  const ON_NurbsSurface& ns,
  double u,
  double v,
  ON_3dVector& u_dir,
  ON_3dVector& v_dir,
  ON_3dVector& n_dir
);

/*
Description:
  Calculates a curve's inflection points. An inflection point is a location
  on a curve at which the sign of the curvature (i.e., the concavity) changes.
  The curvature at these locations is always 0.
Parameters:
  curve  - [in] The curve to evaluate.
  points - [out] The inflection points.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCurveInflectionPoints(
  const ON_Curve& curve,
  ON_SimpleArray<ON_3dPoint>& points
);

/*
Description:
  Calculates a curve's inflection points. An inflection point is a location
  on a curve at which the sign of the curvature (i.e., the concavity) changes.
  The curvature at these locations is always 0.
Parameters:
  curve      - [in] The curve to evaluate.
  points     - [out] The inflection points.
  parameters - [out] The inflection parameters.
Returns:
  true if successful, false otherwise.
*/

RHINO_SDK_FUNCTION
bool RhinoCurveInflectionPoints(
  const ON_Curve& curve,
  ON_SimpleArray<ON_3dPoint>& points,
  ON_SimpleArray<double>& parameters
);

/*
Description:
  Calculates a curve's maximum curvature points. The maximum curvature points
  identify where the curvature starts to decrease in both directions
  from the points.
Parameters:
  curve  - [in] The curve to evaluate.
  points - [out] The minimum curvature radius points.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCurveMaxCurvaturePoints(
  const ON_Curve& curve,
  ON_SimpleArray<ON_3dPoint>& points
);

/*
Description:
  Calculates a curve's maximum curvature points. The maximum curvature points
  identify where the curvature starts to decrease in both directions
  from the points.
Parameters:
  curve      - [in] The curve to evaluate.
  points     - [out] The minimum curvature radius points.
  parameters - [out] TThe minimum curvature parameters.
Returns:
  true if successful, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoCurveMaxCurvaturePoints(
  const ON_Curve& curve,
  ON_SimpleArray<ON_3dPoint>& points,
  ON_SimpleArray<double>& parameters
);

/*
Description:
  Construct an H-spline from a sequence of interpolation points and optional
  start and end derivative information.
Parameters:
  Pts      - [in] Points to interpolate.
  PntCount - [in] Number of points. PntCount > 1
  StartTan - [in] Unit tangent vector or nullptr.
  EndTan   - [in] Unit tangent vector or nullptr.

 Returns:
  The H-spline curve, or nullptr if there was an error.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoCreateHSpline(
  const ON_3dPoint* Pts,
  int PntCount,
  const ON_3dVector* StartTan,
  const ON_3dVector* EndTan
);

/*
Description:
  Verifies a Brep is in the form of a solid box.
Parameters:
  brep      - [in] The Brep to test.
  tolerance - [in] The tolerance used to determine if faces are planar
                   and to compare face normals.
 Returns:
  True if the Brep is in the form of a solid box.
*/
RHINO_SDK_FUNCTION
bool RhinoIsBrepBox(
  const ON_Brep& brep,
  double tolerance
);

/*
Description:
  Creates an arc-line-arc blend curve between two curves.
Parameters:
  P0 - [in] Point on the first curve.
  D0 - [in] Tangent direction at P0.
  P1 - [in] Point on the second curve.
  D1 - [in] Tangent direction a P1.
  radius - [in] The radius of the arc segments.
Returns:
  A pointer to a polycurve if successful, nullptr on failure.
  Note, memory for the output curve is allocated and becomes the responsibility of the caller.
Remarks:
  Output is generally a polycurve with three segments: Arc, Line, Arc.
  The first arc segment will start at P0, with starting tangent D0.
  The second arc segment will end at P1 with end tangent D1.
  The line segment will start from the end of the first arc segment and end at start of the second arc segment,
  and it will be tangent to both arcs at those points.
  In some cases, one or more of those segments will be absent because they would have 0 length.
  If there is only a single segment, the result will not be a polycurve.
*/
RHINO_SDK_FUNCTION
ON_Curve* RhinoArcLineArcBlend(
  ON_3dPoint P0,
  ON_3dVector D0,
  ON_3dPoint P1,
  ON_3dVector D1,
  double radius
);

/*
Description:
  Recursively removes any face with a naked edge.
  This function is only useful for non-manifold Breps.
Parameters:
  brep - [in] The Brep.
Returns:
  true if successful, false otherwise.
See Also:
  ON_Brep::IsManifold
*/
RHINO_SDK_FUNCTION
bool RhinoRemoveBrepFins(ON_Brep& brep);

/*
Description:
  Find any naked edges with same start and end vertex, and arclength less than tolerance,
  and attempt to remove them by removing trims and extending the adjacents to meet.
Parameters:
  brep      - [in] The Brep.
  tolerance - [in] The tolerance.
  bCleanUp  - [in] If true, sets tolerances, boxes, etc., and compacts the Brep.
                   If false, then the caller should at some point.
Returns:
  The number of edges removed.
*/
RHINO_SDK_FUNCTION
int RhinoRemoveAllNakedMicroEdges(
  ON_Brep& brep,
  double tolerance,
  bool bCleanUp
);

/*
Description:
  Find any naked edges with same start and end vertex, and arclength less than tolerance,
  and attempt to remove them by removing trims and extending the adjacents to meet.
Parameters:
  brep         - [in] The Brep.
  edge_indices - [in] The indices of the edges to process.
  tolerance    - [in] The tolerance.
  bCleanUp     - [in] If true, sets tolerances, boxes, etc., and compacts the Brep.
                      If false, then the caller should at some point.
Returns:
  The number of edges removed.
*/
RHINO_SDK_FUNCTION
int RhinoRemoveNakedMicroEdges(
  ON_Brep& brep,
  const ON_SimpleArray<int>& edge_indices,
  double tolerance,
  bool bCleanUp
);

/*
Description:
  Find a nake edges with same start and end vertex, and arclength less than tolerance,
  and attempt to remove it by removing trims and extending the adjacents to meet.
Parameters:
  brep         - [in] The Brep.
  edge_indices - [in] The edge to process.
  tolerance    - [in] The tolerance.
  bCleanUp     - [in] If true, sets tolerances, boxes, etc., and compacts the Brep.
                      If false, then the caller should at some point.
Returns:
  True if the edge was removed, false otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoRemoveNakedMicroEdge(
  ON_Brep& brep,
  int edge_index,
  double tolerance,
  bool bCleanUp
);


RHINO_SDK_FUNCTION
int RhinoFitTrimSurface(
  const ON_BrepFace* face,
  const ON_BrepEdge* edge,
  ON_SimpleArray<double>& knots,
  double tolerance,
  bool bSections,
  ON_SimpleArray<ON_NurbsSurface*>& FitSurfaces,
  double* FitMeasurement
);


RHINO_SDK_FUNCTION
int RhinoFitTrimSurface2(
  const ON_BrepFace* face,
  const ON_BrepEdge* edge,
  ON_SimpleArray<double>& knots,
  double tolerance,
  bool bSections,
  ON_SimpleArray<ON_NurbsSurface*>& FitSurfaces,
  ON_SimpleArray<ON_NurbsCurve*>& EdgeCurves,
  double* FitMeasurement,
  ON_Curve** trimCurveOnSurface,
  ON_Curve** splitCurve
);

RHINO_SDK_FUNCTION
bool RhinoIsRefitTrimValid(
  // Determines whether the input surface, curve, and projection are valid for a RefitTrim or FitCurveToSurface
  // returns the split surface and (possibly extended) trim curve, adjusted to flow in U
  // returns flags to indicate whether the surface is transposed (i.e. knot vector in V, trim curve is North/South)
  // or reversed (flipped)

  // INPUT:
  const ON_Surface* surface, // untrimmed surface 
  const ON_Curve* trimCurve3d, // curve that will trim from one edge to the opposite

  // How is the trim curve projected onto the surface?
  const ON_3dVector trimProjectionDir, // ... if non-zero, vector for projection
  double tolerance,    // 3d tolerance for projection, splitting, fitting...

  // Inputting an "active" point means you only want one side of the "split" to be refit -
  // In other words, you want a "trim refit".  Results will be returned in "srfLower"
  bool oneSideOnly,
  const ON_3dPoint PtActive,

  //OUTPUT:
  ON_NurbsCurve** trimCrvU, // trim curve adjusted to flow in u
  ON_NurbsSurface** surfaceU,   // input surface adjusted to flow in u, North edge trimmed
  bool* isTransposed,           // true it transposed
  bool* isReversed                  // true if reversed
);

RHINO_SDK_FUNCTION
bool RhinoRefitCrvCalculateMaxDeviation(
  // This function calculates the maximum deviation between the original curve and the resulting fit curve
  // returns true if successful
  // INPUT:
  const ON_Curve* input_curve,  // the original curve 
  const ON_Curve* refit_curve,  // the fit result
  double tolerance,             // tolerance to use, typically document tolerance
  // OUTPUT:
  ON_Line* deviation            // A line segment representing the maximum deviation, from the refit curve to the original curve
);

RHINO_SDK_FUNCTION
bool RhinoRefitSimplySplitSurface(

  // This routine accepts an untrimmed surface (srf) and a 3-d trim curve (trimCrv3d).
  // The trim curve is assumed to run from one surface edge to the opposite edge;
  // this is referred to as a "simple" trim curve, roughly parallel to one of the
  // srf directions (either u or v).  We refer to that parameter as the "trim parameter".
  //
  // The routine splits the surface via the trim curve, and then refits either side
  // ("upper" = "above the trim", and "lower" = "below the trim") as a set of
  // untrimmed Nurbs surfaces.  The idea is to retain, as much as possible, the 
  // Nurbs structure of srf, especially in the trim parameter.
  //
  // INPUT:
  const ON_Surface* surface, // untrimmed surface 
  const ON_Curve* trimCurve3d, // curve that will trim from one edge to the opposite

  // How is the trim curve projected onto the surface?
  const ON_3dVector trimProjectionDir, // ... if non-zero, vector for projection
  double tolerance,    // 3d tolerance for projection, splitting, fitting...

  // Adding any knots to the surface in the trim parameter?
  RhinoRefitTrimKnotMode knotAdditionMode,
  // 0: no, don't add any
  // 1: Yes, add nKnots knots, spaced regularly over the entire surface
  // 2: Yes, add nKnots knots, spaced regularly PER EXISTING SPAN
  // 3: Yes, add nKnots knots, as provided in the array Knots
  int numKnots,     // if addTrimParamKnots != 3, the number of knots to add
  ON_SimpleArray<double>& Knots,

  // Output surface divided into sections?
  RhinoRefitTrimSectionMode trimParamSections,
  // 0: no sections, just output one "upper" and one "lower" surface
  // 1: Yes, divide into sections at each of the input surface's existing knots
  // 2: Yes, divide into sections at each of the "nKnot" added knot values
  // 3: Yes, divide into sections at ALL knot values (i.e. both 1 and 2)
  int numNonTrimSpans,   // number of spans in the non-trim parameter.

  // The same number applies to both the upper and lower output surfaces
  // The routine will calculate a reasonable minimum, so "1" is an OK default
  bool meetCurve,  // Drag the refit surfaces out to meet the original trim curve

  // Inputting an "active" point means you only want one side of the "split" to be refit -
  // In other words, you want a "trim refit".  Results will be returned in "srfLower"
  bool oneSideOnly,
  const ON_3dPoint PtActive,

  // What type of output?
  bool outputSurface,  // if true, output fit surfaces: srfLower and, if !bActivePt, srfUpper
  bool outputCurve,    // if true, output fit curve:  nurbsEdge

  // OUTPUT:
  int* numSections, // Calculated based on trimParamSections
  double* FitMeasurement,    // 3-d fit to trim curve (projected, or trimCrv3d if bMeetCurve)
  ON_SimpleArray<ON_NurbsSurface*>& lowerSurface, // Surfaces (# = nSections) below trim curve in srf's (u,v) domain
  ON_SimpleArray<ON_NurbsSurface*>& upperSurface, // Surfaces (# = nSections) above trim curve in srf's (u,v) domain
  ON_SimpleArray<ON_NurbsCurve*>& edgeCurves,  // Curves (# = nSections) fit to trim curve with srf's u/v parameter
  ON_Curve** trimCurveOnSurface,// if not null, returns the actual projected or pulled trim curve used for the split/fit
  ON_Curve** splitCurve // if not null, returns the curve fit splitting the two halves

  // Junctions between surfaces in srfLower should be G1; similarly for srfUpper

  // integer return value:
  //  true - ok
  //  false - error, surfaces not returned
);

RHINO_SDK_FUNCTION
int RhinoFitCurveToSurface(
  const ON_NurbsSurface* surface,
  const ON_Curve* curve,
  ON_3dVector projDir,
  ON_SimpleArray<double> knots,
  double tolerance,
  bool bBezierSections,
  ON_SimpleArray<ON_NurbsCurve*>& FitCurves,
  double* FitMeasurement
);

RHINO_SDK_FUNCTION
int RhinoFitCurveToSurface2(
  const ON_NurbsSurface* surface,
  const ON_Curve* curve,
  ON_3dVector projDir,
  ON_SimpleArray<double> knots,
  double tolerance,
  bool bBezierSections,
  ON_SimpleArray<ON_NurbsCurve*>& FitCurves,
  ON_Curve** trimCurveOnSurface, // if not null, returns the actual projected or pulled trim curve used for the split/fit
  ON_Curve** splitCurve,
  double* FitMeasurement
);

RHINO_SDK_FUNCTION
int RhinoSplitRefitSurface(
  const ON_Surface* surface,
  const ON_Curve* curve,
  ON_3dVector projDir,
  bool bMeetCurve,
  ON_SimpleArray<double> knots,
  double tolerance,
  bool bBezierSections,
  ON_SimpleArray<ON_NurbsSurface*>& FitSurfacesLower,
  ON_SimpleArray<ON_NurbsSurface*>& FitSurfacesUpper,
  ON_SimpleArray<ON_NurbsCurve*>& FitCurves,
  ON_Curve** trimCurveOnSurface, // if not null, returns the actual projected or pulled trim curve used for the split/fit
  ON_Curve** splitCurve,
  double* FitMeasurement
);

/*
Description:
  Creates a NURBS surface from a plane and additional parameters.
Parameters:
  plane     - [in] The plane.
  u_extents - [in] The interval of the output surface in the U direction.
  v_extents - [in] The interval of the output surface in the V direction.
  u_degree  - [in] The degree of the output surface in the U direction.
  v_degree  - [in] The degree of the output surface in the V direction.
  u_points  - [in] The number of control points of the output surface in the U direction.
  v_points  - [in] The number of control points of the output surface in the V direction.
  pOutSrf   - [in/out] If not nullptr, this surface will be used for output.
Returns:
  The output surface or nullptr.
  Note, if nullptr is passed as pOutSrf, then memory for the output surface is allocated
  and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_NurbsSurface* RhinoSurfaceFromPlane(
  const ON_Plane& plane,
  ON_Interval u_interval,
  ON_Interval v_interval,
  int u_degree,
  int v_degree,
  int u_points,
  int v_points,
  ON_NurbsSurface* pOutSrf = nullptr
);

/*
Description:
  Fits a NURBS curve to a dense, ordered set of points.
Parameters:
  point_count        - [in] The number of points.
  pPoints            - [in] The ordered list of points.
  pStartTan          - [in] The tangent direction at the start of the curve. This value can be nullptr.
  pEndTan            - [in] The tangent direction at the end of the curve. This value can be nullptr.
  degree             - [in] The desired degree of the output curve.
  bPeriodic          - [in] Set true if the output curve should be periodic.
  tolerance          - [in] The fitting tolerance. When in doubt, use the document's absolute tolerance.
  bToleranceAchieved - [out] true if all points are within tolerance of the result.
                             Most likely reason for a value of false is insufficient density of the point list
                             relative to the complexity of the shape of the output.
Returns:
  The output curve if successful, or nullptr on failure.
  Note, memory for the output curve is allocated and becomes the responsibility of the caller.
*/
RHINO_SDK_FUNCTION
ON_NurbsCurve* RhinoFitNurbsCurveToPoints(
  int point_count,
  const ON_3dPoint* pPoints,
  ON_3dVector* pStartTan,
  ON_3dVector* pEndTan,
  int degree,
  bool bPeriodic,
  double tolerance,
  bool& bToleranceAchieved
);

/*
Description:
  Local minimization for point on a curve with tangent perpendicular to N.
Parameters:
  curve       - [in] A valid curve to evaluate.
  N           - [in] N and the curve tangent define a plane, must not be zero.
                     In this plane, there is a vector V perpendicular to the tangent.
  subdomain   - [in] Subdomain to evaluate, must not be empty.
  seed        - [in] A seed parameter, must be contained in the non-empty interval subdomain.
  curve_param - [out] The parameter on the curve.
  angle_error - [out] The measure in radians of the angle between N and V.
                      The angle will be zero when the result is an inflection.
Returns:
  Return is true if the minimization succeeds, regardless of angle_error.
Remarks:
  The algorithm minimizes the square of the dot product of N with the curve tangent.
  It is possible that the result will not be close to zero.
*/
RHINO_SDK_FUNCTION
bool RhinoFindLocalInflection(
  const ON_Curve& curve,
  ON_3dVector N,
  ON_Interval subdomain,
  double seed,
  double* curve_param,
  double* angle_error
);

/*
Description:
  Fast intersection of plane with bounding box. 
 Parameters:
  origin       [in] - The plane's origin.
  normal       [in] - The plane's normal direction.
  bounding_box [in] - The bounding box.
  bAccurate    [in] - If false, returns the four segment polyline of the intersection 
                        with the box extended infinitely in the max coordinate direction,
                      If true, incorporates the intersection with the Max and Min bounding
                        planes if the four point polyline sticks out a significant distance
                        (0.001*the length of the box in the max normal coordinate direction).
  polyline     [out] - The output polyline if successful. 
Returns:
  Returns false if all intersections are out of the same side of the bounding box.
  If bAccurate is false, returns the four segment polyline of the intersection
  with the box extended infinitely in the max coord direction
Remarks:
  Intersect the four bounding box infinite lines in the direction of the max
  coordinate with the plane. Point and single line intersections are ignored.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectPlaneWithBoundingBox(
  ON_3dPoint origin, 
  ON_3dVector normal,
  const ON_BoundingBox& bounding_box, 
  bool bAccurate, 
  ON_Polyline& polyline
);

class CRhSurfaceFillet;

//New interactive attempt at FilletSrf. 
class RHINO_SDK_CLASS CRhinoSurfaceFillet
{
public:
  //FilletSrfSrf
  CRhinoSurfaceFillet(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
    const ON_BrepFace& FaceB, const ON_2dPoint& uvB,
    double radius, bool bAllowExtend, double tolerance);

  CRhinoSurfaceFillet(const ON_BrepFace& FaceA, const ON_2dPoint& uvA,
    const ON_BrepFace& FaceB, const ON_2dPoint& uvB,
    double radius, double tolerance);

  //FilletSrfToRail Result will have v-min along Rail3d, v-max along the calculated
  //rail on Face.
  /*
  CRhinoSurfaceFillet(const ON_BrepFace& RailFace, const ON_Curve& Rail3d, //On RailFace
    const ON_BrepFace& Face, const ON_2dPoint& uv,//In Face's pspace
    bool bAllowExtend, double tolerance);
    */
  CRhinoSurfaceFillet(const ON_BrepFace& RailFace, const ON_Curve& Rail3d, //On RailFace
    const ON_BrepFace& Face, const ON_2dPoint& uv,//In Face's pspace
    double tolerance);

  
  //FilletSrfCrv
  /*
  CRhinoSurfaceFillet(const ON_BrepFace& Face, const ON_2dPoint& uv,
    const ON_Curve& Curve,
    double t,
    double radius, bool bAllowExtend, double tolerance);
    */

  CRhinoSurfaceFillet(const ON_BrepFace& Face, const ON_2dPoint& uv,
    const ON_Curve& Curve,
    double t,
    double radius, double tolerance);


  ~CRhinoSurfaceFillet();

  //Call this before calling TrimBreps or any of the fillet creation functions. 
  //If it returns false, none of them will work.
  bool IsInitialized();
                 

  //If the two input breps are the same, TrimmedBreps[1] will be empty.
  //The constructor idoes everything necessary for this to be called.
  //Calles to the various fillet creation members will not change the result.
  bool TrimBreps(bool bExtend, ON_SimpleArray<ON_Brep*> TrimmedBreps[2]);//Trim input breps with the rails.

  //These make fillet surfaces and hack them up to active pieces. 
  //Each adjusts cross sections and calls FitCrossSections().

  bool RationalArcs(int rail_degree, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);
  bool NonRationalQuinticArcs(int rail_degree, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);//Arc approximation
  bool NonRationalQuintic(int rail_degree, double TanSlider, double InnerSlider, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);
  bool NonRationalQuarticArcs(int rail_degree, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);//Arc approximation
  bool NonRationalQuartic(int rail_degree, double TanSlider, double InnerSlider, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);
  bool NonRationalCubicArcs(int rail_degree, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);//Arc approximation
  bool NonRationalCubic(int rail_degree, double TanSlider, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);
  bool G2ChordalQuintic(int rail_degree, bool bExtend, ON_SimpleArray<ON_Brep*>& Fillets);
  //bool WhateverElse();

  //This will clear out the existing rails and fillet surfaces, 
  //and redo the offset intersections, etc. DO NOT use this for SrfToRail - there is no radius..  
  //The trimmed brep must be redone.
  bool ChangeRadius(double new_radius);

  void DebugPlines(int which_one, ON_SimpleArray<ON_Curve*>& Crvs) const;//0,1 for rails, 2 for center, 3-apex
  void DebugArcs(ON_SimpleArray<ON_Curve*>& Crvs) const;

  //Center,start, apex, end
  void DebugArcData(ON_SimpleArray<ON_Curve*>& Crvs) const;

private:
  CRhSurfaceFillet* _pFillet;
  CRhinoSurfaceFillet();//Not called
};

/*
Description:
  Parses a text field string.
Parameters:
  pParseResults [in/out] - The text formula to parse.
  pRhinoObject [in] - The Rhino object. Value can be nullptr.
  pTopParent [in] - The parent Rhino object. Value can be nullptr.
  pImmediateParent [in] - The immediate parent instance object. Value can be nullptr.
Returns:
  true on success, the parsed text field is returned in pParseResults.
  false on failure.
Since:
  8.12
*/
RHINO_SDK_FUNCTION 
bool RhinoParseTextFields(
  ON_wString* pParseResults, 
  const CRhinoObject* pRhinoObject,
  const CRhinoObject* pTopParent,
  const CRhinoInstanceObject* pImmediateParent
);

/*
Description:
  Finds a line segment between a pair of curves such that the line segment
  is either tangent or perpendicular to each of the curves.
 Parameters:
  curve0  [in] - The first curve.
  curve1  [in] - The second curve.
  bPerp0  [in] - Find line perpendicular to (true) or tangent to (false) curve0.
  bPerp1  [in] - Find line Perpendicular to (true) or tangent to (false) curve1.
  abs_tol [in] - The active document's model absolute tolerance.
  t0      [in/out] - Parameter value of point on curve0. Seed value at input and solution at output.
  t1      [in/out] - Parameter value of point on curve1. Seed value at input and solution at output.
Returns:
  true on success, false on failure.
*/
RHINO_SDK_FUNCTION
bool RhinoGetTanPerpPoint(
  const ON_Curve& curve0,
  const ON_Curve& curve1,
  bool bPerp0,
  bool bPerp1,
  double abs_tol,
  double& t0,
  double& t1
);

/*
Description:
  Duplicates the edges of a brep
 Parameters:
  tol           [in] - tolerance used to merge the output curves. see RhinoMergeCurves
  pBrep         [in] - input brep
  output_array  [out] - output curves
  bPerp1  [in] - Find line Perpendicular to (true) or tangent to (false) curve1.
  abs_tol [in] - The active document's model absolute tolerance.
  t0      [in/out] - Parameter value of point on curve0. Seed value at input and solution at output.
  t1      [in/out] - Parameter value of point on curve1. Seed value at input and solution at output.
Returns:
  number of curves added to output_array;
Remarks:
  The returned curves are on the heap and the caller is
  responsible for deleting them when they are no longer
  needed.
*/
RHINO_SDK_FUNCTION
int RhinoDuplicateEdgeCurves(
  const double tol,
  const ON_Brep* pBrep,
  ON_SimpleArray<ON_Curve*>& output_array);

/*
Description:
  Duplicates the borders of a brep (ie. naked edges)
 Parameters:
  tol           [in] - tolerance used to merge the output curves. see RhinoMergeCurves
  pBrep         [in] - input brep
  output_array  [out] - output curves
  bPerp1  [in] - Find line Perpendicular to (true) or tangent to (false) curve1.
  abs_tol [in] - The active document's model absolute tolerance.
  t0      [in/out] - Parameter value of point on curve0. Seed value at input and solution at output.
  t1      [in/out] - Parameter value of point on curve1. Seed value at input and solution at output.
Returns:
  number of curves added to output_array;
Remarks:
  The returned curves are on the heap and the caller is
  responsible for deleting them when they are no longer
  needed.
*/
RHINO_SDK_FUNCTION
int RhinoDuplicateBorderCurves(
  const double tol,
  const ON_Brep* pBrep,
  ON_SimpleArray<ON_Curve*>& output_array);

/*
Description:
  Individually insets faces of a brep by offsetting the faces outer edges inward and inner edges out then splitting the face
  with the offset curves.
Parameters:
  brep - the brep to inset
  aFaceIndices - the indices of the faces to inset
  dDistance - The distance to offset along the face
  bLoose - If true, offset by moving edit points otherwise offset within tolerance.
  bIgnoreSeams - If true, the seam edges are not offset and adjacent edges are extended to meet the seam. Otherwise offset normally.
  bCreaseCorners - If true, splitting curves will be made between the creases on edge curves and creases on the inset curves.
  dTolerance - The fitting tolerance for the offset. When in doubt, use the document's absolute tolerance.
  dAngleTolerance - The angle tolerance in radians for identifying creases when creasing corners
Returns:
  A brep with the faces inset or nullptr on failure
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoInsetBrepFaces(
  const ON_Brep& brep,
  const ON_SimpleArray<int>& aFaceIndices,
  double dDistance,
  bool bLoose,
  bool bIgnoreSeams,
  bool bCreaseCorners,
  double dTolerance,
  double dAngleTolerance
);

/*
Description:
  If the brep.m_F[iFaceIdx] is planar, and all the adjacent faces are planar, this function transforms brep.m_F[iFaceIdx]
  with the passed in xform and recomputes the adjacent faces. See the PushPull commands extend mode.
Parameters:
  brep - The brep to extend
  iFaceIdx - The face being transformed
  xform - The transformation
  dTolerance - The tolerance for planarity checks
Returns:
  A brep with the transformed face or nullptr on failure
*/
RHINO_SDK_FUNCTION
ON_Brep* RhinoPushPullExtend(
  const ON_Brep& brep,
  int iFaceIdx,
  const ON_Xform& xform,
  double dTolerance
);

/*
Description:
  Extends faces to meet and trims the faces to each other. If input faces intersect without extending, the selection point indicates the side to keep.
Parameters :
  pFace0 - [in] first face to connect
  f0SelPt - [in] selection point on first face near the edge to extend.
  pFace1 - [in] second surface
  f1SelPt - [in] selection point on second face near the edge to extend.
  tol - [in] tolerance used for intersecting faces and for simplifying intersection curve.
  angleTol - [in] angle tolerance in radians used for simplifying intersection curve.
  outBrep0 - [out] first face extended and trimmed to connect with the second face.
  outBrep1 - [out] second face extended and trimmed to connect with the first face.
Returns :
  true if valid connection found.
*/
RHINO_SDK_FUNCTION
bool RhinoExtendBrepFacesToConnect(
  const ON_BrepFace* pFace0,
  ON_3dPoint f0SelPt,
  const ON_BrepFace* pFace1,
  ON_3dPoint f1SelPt,
  double tol,
  double angleTol,
  ON_Brep*& outBrep0,
  ON_Brep*& outBrep1);

/*
Description:
  Extends faces to meet and trims the faces to each other. If input faces intersect without extending, the edge indicates the side to keep.
Parameters :
  pFace0 - [in] first face to connect
  edgeIndex0 - [in] edge to extend.
  pFace1 - [in] second surface
  edgeIndex1 - [in] edge to extend.
  tol - [in] tolerance used for intersecting faces and for simplifying intersection curve.
  angleTol - [in] angle tolerance in radians used for simplifying intersection curve.
  outBrep0 - [out] first face extended and trimmed to connect with the second face.
  outBrep1 - [out] second face extended and trimmed to connect with the first face.
Returns :
  true if valid connection found.
*/
RHINO_SDK_FUNCTION
bool RhinoExtendBrepFacesToConnect(
  const ON_BrepFace* pFace0,
  int edgeIndex0,
  const ON_BrepFace* pFace1,
  int edgeIndex1,
  double tol,
  double angleTol,
  ON_Brep*& outBrep0,
  ON_Brep*& outBrep1);


/*
Description:
  Intersects a curve with an implicit function.
Parameters:
  curve            - [in] the curve to intersect
  implicitFunction - [in] the implicit function to intersect the curve with
  tolerance        - [in] the intersection tolerance
  results          - [out] the list of intersection events (points and overlaps)
  error            - [out] if the return value is false, the error will contain an error message.
Returns:
  true if no errors were encountered.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectCurveImplicitFunction(
  const ON_Curve* curve,
  const ON_SignedDistanceFunction& implicitFunction,
  double tolerance,
  ON_SimpleArray<ON_CIXEvent>& results,
  ON_wString& error);


/*
Description:
  Intersects a surface with an implicit function.
Parameters:
  surface          - [in] the surface to intersect
  implicitFunction - [in] the implicit function to intersect the curve with
  tolerance        - [in] the intersection tolerance
  overlapTolerance - [in] the overlap tolerance, give <= 0 to use a sensible value
  fittingTolerance - [in] the fitting tolerance, give <= 0 to use a sensible value
  results          - [out] the list of intersection events
  error            - [out] if the return value is false, the error will contain an error message.
Returns:
  true if no errors were encountered.
*/
RHINO_SDK_FUNCTION
bool RhinoIntersectSurfaceImplicitFunction(
  const ON_Surface* surface,
  const ON_SignedDistanceFunction& implicitFunction,
  double tolerance,
  double overlapTolerance,
  double fittingTolerance,
  ON_SimpleArray<ON_SIXEvent>& results, 
  ON_wString& error);

/*
Description:
  Create tween surfaces that gradually transition between 2 bounding surfaces using point sampling
Parameters :
  srf0 - [in] first surface.
  srf1 - [in] second surface.
  num_of_tween_srfs - [in] number of tween surfaces.
  num_of_sample_pts - [in] number of sample points.
  tol - [in] tolerance.
  out_srfs - [out] output tween surfaces
Returns :
  true if valid tween surfaces found.
*/
RHINO_SDK_FUNCTION 
bool RhinoTweenSurfacesSamplePoints(
  const ON_Surface* srf0,
  const ON_Surface* srf1,
  int num_of_tween_srfs,
  int num_of_sample_pts,
  double tol,
  ON_SimpleArray<ON_Surface*>& out_srfs);

enum RhVariationalDomain : int
{
  Projected = 0,
  Molded = 1,
  Regular = 2,
  RegularMolded = 3,
  Untrimmed = 4
};


/*
Description:
  Create a variational patch for input edges, internal curves, points and normal lines
Parameters:
 edges [in]                  - constraint edges. These curves will get a positional, normal or curvature constraint
                               depending on the edgeContinuities array content. These curves will be used to trim the patch.
 internalCurves [in]         - constraint internal curves. These curves will get positional constraint.
 points [in]                 - positional constraint points
 normals [in]                - normal constraint points. The surface will be constrained to the start of the line with a surface normal
                               parallel to the line.
 edgeContinuities [in]       - the required continuity for each edge in the edges array
 order [in]                  - the surface order to be used (default: 4, 4)
 cvCount [in]                - the initial surface cv count (default: 12, 12)
 domainOption [in]           - the method of generating a UV domain for the input edges, curves and points (default: Projected)
                               o TL_VariationalDomain::Projected will project to a best fitted plane
                               o TL_VariationalDomain::Molded will use the parameter curves of Projected as a starting point and gradually
                                 increase the constraint weights. This will pull the surface towards the constraints. At each step in the
                                 pulling, the parameter curves and points are obtained by a pullback to the current shape of the surface.
                               o TL_VariationalDomain::Untrimmed - only usable with 4 edges and no other constraints. Will create an
                                 untrimmed patch
 stretching [in]             - the fraction of first derivatives' contribution to the stiffness matrix (default: 0.0)
 bending [in]                - the fraction of second derivatives' contribution to the stiffness matrix (default: 0.3)
 rocBending [in]             - the fraction of third derivatives' contribution to the stiffness  matrix (default: 0.7)
 maxRefinements [in]         - the maximum number of refinements performed to the surface where constraints are not met (default: 5)
 tolerance [in]              - positional constraint tolerance
 angleToleranceRadians [in]  - normal constraint angle tolerance
 curvatureTolerance [in]     - curvature tolerance
 result [out]                - the resulting trimmed patch
 toleranceMet [out]          - true if all constraints are within positional tolerance
 angleToleranceMet [out]     - true if all constraints are within normal angle tolerance
 curvatureToleranceMet [out] - true if all constraints are within curvature tolerance
 warning [out]               - if successful, this string may contain wwarning(s)
 error [out]                 - if not successful, this will contain a description of what went wrong.
 */

RHINO_SDK_FUNCTION
bool RhinoCreateVariationalPatch
(
  const ON_SimpleArray<const ON_Curve*>& edges,
  const ON_SimpleArray<const ON_Curve*>& internalCurves,
  const ON_3dPointArray& points,
  const ON_SimpleArray<ON_Line>& normals,
  const ON_SimpleArray<ON::continuity>& edgeContinuities,
  const int order[2], const int cvCount[2],
  RhVariationalDomain domainOption,
  double stretching, double bending, double rocBending,
  double tolerance, double angleToleranceRadians, double curvatureTolerance,
  int maxRefinements,
  TL_Brep& result, 
  bool& toleranceMet, bool& angleToleranceMet, bool& curvatureToleranceMet,
  ON_wString& warning,
  ON_wString& error
);

