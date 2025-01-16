//
// Copyright (c) 1993-2016 Robert McNeel & Associates. All rights reserved.
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

class RHINO_SDK_CLASS CRhinoTextOutTextLog : public ON_TextLog
{
public:
  virtual void DumpText( const wchar_t* lps);
};

class RHINO_SDK_CLASS CRhinoTextOut
{
public:
  CRhinoTextOut();
  virtual ~CRhinoTextOut();

  // Description:
  //  This will eventually call CRhinoTextOut::PrintText().  If is_interactive is true then
  //  the ON_TextLog passed to CRhinoTextOut::PrintText() will output text to a RichEditCtrl
  //  embedded in a text dialog otherwise the user will be prompted for a text file name to
  //  open and CRhinoTextOut::PrintText() will append the text file.
  // Parameters:
  //  is_interactive [in] - If true then a text dialog is displayed otherwise text is output
  //                        to a file.
  //  lpsWindowTitle [in] - Only valid if is_interactive == true, if so then the text dialog
  //                        title is set to this string.
  //  pData          [in] - Passed to CRhinoTextOut::PrintText as pData argument.
  //  hWndParent     [in] - Optional argument which is only meaningful if is_interactive is
  //                        true.  If so then it is used as the parent window for the text dialog.
  // Returns:
  //  CRhinoCommand::success = Text output successfully.
  //  CRhinoCommand::cancel  = User pressed escape when promted for a file name to write to.
  //  CRhinoCommand::failure = Error opening text file.
  virtual CRhinoCommand::result DisplayText( bool is_interactive, const wchar_t* lpsWindowTitle, LPVOID pData = nullptr, HWND hWndParent = nullptr);

  // Description:
  //  You must override this function to actually display your text.
  // Parameters:
  //  text_log [in] - Log attached to text file, history window or text dialog.
  //  pData    [in] - Data passed to CRhinoTextOut::DisplayText method. 
  virtual void PrintText( ON_TextLog& text_log, LPVOID pData) = 0;

  enum output_mode
  {
    text_dialog = 0, // Display text in dialog box
    history_window,  // Display text in Rhino command history window
    text_file,       // Write text to file
    clipboard        // Write text to Windows clipboard
  };

  // Description:
  //  Direct output to a specific context.
  // Parameters:
  //  mode [in] - Place to direct output
  void SetTextOutputMode( output_mode mode);

  // Description:
  //  Place to direct output.
  output_mode TextOutputMode() const;

  // Description:
  //  Specify whether or not to use the file dialog when specifying a text file to write to.
  // Parameters:
  //  b [in] If true then file dialog will be displayed when choosing the file option otherwise
  //         users will be prompted for a file name via the command line.
  // Returns:
  //  Returns previous value
  bool SetUseFileDialog( bool b);

  // Description:
  //  Specify whether or not to use the file dialog when specifying a text file to write to.
  // Returns:
  //  Returns true if the file dialog when specifying a text file to write to.
  bool UseFileDialog() const;


  ////////////////////////////////////////////////////////
  // For internal use
  //
  // Description:
  //  String to use as dialog box title.
  // Returns:
  //  Returns string to use as dialog box title.
  //
  const wchar_t* WindowTitle() const;

  // Description:
  //  Data pointer passed to CRhinoTextOut::DisplayText().
  // Returns:
  //  Returns pointer passed to CRhinoTextOut::DisplayText()
  // See also:
  //  CRhinoTextOut::DisplayText()
  LPVOID Data() const;

  // Description:
  //  Dialog box used to display text.
  // Returns:
  //  Returns pointer to dialog box used to display text.
  class CRhRichTextDialog* Dialog() const;

  // Description:
  //  If this is true and a text dialog is displayed then the edit control in the dialog
  //  will be scrolled to the bottom.
  // Returns:
  //  Returns true if edit control should be scrolled to the bottom
  bool ScrollTextDialogToBottom() const;

  // Description:
  //  If this is true and a text dialog is displayed then the edit control in the dialog
  //  will be scrolled to the bottom.
  // Parameters:
  //  b [in] If true then edit control is scrolled to the bottom in the text dialog.
  // Returns:
  //  Returns true if edit control should be scrolled to the bottom
  void ScrollTextDialogToBottom( bool b);

public:
  class CRhTextOutWStringTextLog* m_wbuffer;
protected:
  class CRhRichTextDialog* m_dialog;
  ON_wString         m_window_title;
  LPVOID             m_pData;
  HWND               m_hWnd;
  output_mode        m_output_mode;
  bool               m_use_file_dialog;
  bool               m_scroll_dialog_to_bottom;

  bool PrintBuffer( CRhinoTextOutTextLog& text_log);
  CRhinoCommand::result PrintToHistory();
  CRhinoCommand::result PrintToDialog();
  CRhinoCommand::result PrintToFile();
  CRhinoCommand::result PrintToClipboard();
};

class RHINO_SDK_CLASS CRhinoTextDump : public CRhinoTextOut
{
public:
  CRhinoTextDump();
  ON_TextLog& TextLog();
  operator ON_TextLog&();

protected:
  void PrintText( ON_TextLog& text_log, LPVOID pData) override;
};
