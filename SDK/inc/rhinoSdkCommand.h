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

class CRhinoObject;

class RHINO_SDK_CLASS CRhinoCommandContext
{
public:
  CRhinoCommandContext(CRhinoDoc&);
  CRhinoCommandContext(const unsigned int rhino_doc_sn);
  ~CRhinoCommandContext() = default;

  // The CRhinoCommandContext::m_doc field will be deleted ASAP
  CRhinoDoc& m_doc;

  // Document command is using
  const unsigned int m_rhino_doc_sn;

  // Check for null return
  CRhinoDoc* Document() const; 

  /*
  m_style determines what kind of user interface (ui) the command
  can use and how serious errors should be reported.

  interative_ui:
    Can use dialogs for input.  Must use message boxes to report
    serious error conditions.

  script_ui:
    All input must come from command line, GetPoint, GetObject,
    GetString, etc.  Must use message boxes to report serious
    error conditions.  Script mode gets used when a command is
    run with a hyphen (-) prefix.

  batch_ui:
    All input must come from command line, GetPoint, GetObject,
    GetString, etc.  Must use RhinoApp().Print() to report serious
    error conditions.  NO dialog boxes are permitted.
    Batch mode is enabled by calling the Rhino BatchModeOn 
    command in a command script.

  */
  enum COMMAND_STYLE
  {
    interactive_ui = 0,
    script_ui = 1,
    batch_ui = 2,
    force_32bit_size = 0xFFFFFFFF
  }
  m_style;

  /*
    Description:
      Tests if the command is being run in interactive mode 

    Returns:
      @untitled table
      true:
      Command is running in interactive mode. Commands are allowed
      to display dialog boxes for user input when running in interactive mode.

      false:
      Command is not running in interactive mode. Commands should
      get user input using command-line options, and should never
      display dialog boxes or other non-scriptable UI elements.
      The user indicates non-interactive mode by preceeding the command
      name with a "-" (eg. "-Layer") on the command line.
  */
  bool IsInteractive() const;

private:
  CRhinoCommandContext() = delete;
};

class RHINO_SDK_CLASS CRhinoObjectPair
{
public:
  CRhinoObjectPair();
  CRhinoObject* m_object[2];
};

// Every command is Rhino is defined by a class derived from CRhinoCommand.
class RHINO_SDK_CLASS CRhinoCommand
{
public:
  /*
  Description:
    For each class derived from CRhinoCommand, there is only a single
    static instance that exists in the .CPP file where the class is
    defined.  No other instances of a class derived from CRhinoCommand
    should ever be created.
  Parameters:
    bTransparent - [in] true if command is transparent and can be run
                   inside of other commands.
    bDoNotRepeat - [in] true if command should not be repeated.
    pPlugIn - [in] if command is part of a plug-in, the plug-in pointer must
        be passed here.  (This is automatically handled by the plug-in
        wizard.)
    bTestCommand - [in] true if command is a "test" or debugging tool
        that should not be part of the commercial release.
  */
  CRhinoCommand( 
    bool bTransparent = false,
    bool bDoNotRepeat = false,
    const class CRhinoPlugIn* pPlugIn = nullptr,
    bool bTestCommand = false
    );

	virtual ~CRhinoCommand();

  static bool IsValidCommandName(const wchar_t*);

  /////////////////////////////////////////////////////////////////////////
  // !! See cmdExample.cpp for instructions on the use of this class. !! //
  /////////////////////////////////////////////////////////////////////////

  //////////
  // Rhino command UUID
  //
  // this should have been a const function, but the mistake was made
  // years ago and now there are thousands of instances of this class.
  // The function CommandId() is const and calls CommandUUID().
	virtual 
  UUID CommandUUID() = 0;

  UUID CommandId() const;

  //////////
  // Rhino default english command name
  // TODO - When all the code is checked in,
  //        remove the non-const version and make
  //        the const version pure virtual.
	virtual 
  const wchar_t* EnglishCommandName() = 0;

	virtual 
  const wchar_t* EnglishCommandName() const;

  //////////
  // Localized command name.  The default implementation
  // uses the command name string table to translate the
  // string returned by EnglishCommandName().
	virtual 
  const wchar_t* LocalCommandName() const;

  enum result
  {
    success = 0, // command worked
    cancel  = 1, // user canceled command
    nothing = 2, // command did nothing but cancel was not pressed
    failure,     // command failed (bad input, computational problem, etc.)
    unknown_command, // command not found (user probably had a typo in command name)
    cancel_modeless_dialog,
    exit_rhino = 0x0FFFFFFF   // exit Rhino.
  };

  /*
  Description:
    Rhino calls this function to run the command.
  Returns:
    success  command worked
    cancel   user canceled command
    nothing  command did nothing but cancel was not pressed.
    failure  command failed (bad input, computational problem, etc.
  */
  virtual 
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& ) = 0;

  /*
  Description:
    Rhino calls uses the virtual ReplayHistory functions to
    to remake an objects when inputs have changed.  
    If a command supports history, it must override exactly
    one of the ReplayHistory functions.  Simple commands
    can use the simple version of replay history that just
    just generates a new object.  Complex commands, like
    Project and Intersect need to override the complex
    version of replay history that uses a results[] array
    to return multiple objects.
  Parameters:
    history - [in]
      History information the command record when it first 
      created the existing objects.

    results - [in/out] (complex version)
      results[i].m_objects[0] - [in]  existing object
      results[i].m_objects[1] - [out] updated object

      The input array has the objects that were
      created by the command in results[i].m_objects[0]. 

      ReplayHistory is responsible for creating replacements
      and putting them in results[i].m_objects[1].  The replacements
      must NOT be added to the document.
      The results[] array can be grown if there are new "updated" objects
      that do not replace an existing object.  In this case
      set the m_objects[0] pointer to NULL.  An input
      results[i].m_objects[0] can be set to NULL if no replacement
      can be calculated.

      If results[i].m_objects[0] and results[i].m_objects[1] are
      both not NULL, then m_objects[1] will replace m_objects[0].
      If results[i].m_objects[0] is not NULL and results[i].m_objects[1]
      is NULL, then results[i].m_objects[0] is deleted.
      If results[i].m_objects[0] is NULL and results[i].m_objects[1]
      is not NULL, then results[i].m_objects[1] is added.
  Returns:
    The simple version returns either NULL or an updated object.
    The complex version returns true if Rhino should process 
    the information in results[] and false otherwise.
  */
  virtual
  bool ReplayHistory( 
          const class CRhinoHistoryRecord& history_record,
          ON_SimpleArray<CRhinoObjectPair>& results
          );

  virtual
  CRhinoObject* ReplayHistory(
     const class CRhinoHistoryRecord& history_record
     );

  /*
  Description:
    The Rhino "Edit" command calls uses the virtual 
    EditObjects functions to interactively edit objects.
    If a command supports editing, it must override 
    the EditObjects function.
  Parameters:
    history - [in]
      History information the command record when it first 
      created the existing objects.

    results - [in/out]
      results[i].m_objects[0] - [in]  existing object
      results[i].m_objects[1] - [out] updated object

      The input array has the objects that were
      created by the command in results[i].m_objects[0]. 

      EditObjects is responsible for creating replacements
      and putting them in results[i].m_objects[1].  The 
      replacements must NOT be added to the document.
      An input results[i].m_objects[0] can be set to NULL 
      if no replacement can be calculated.

      If results[i].m_objects[0] and results[i].m_objects[1] are
      both not NULL, then m_objects[1] will replace m_objects[0].
      If results[i].m_objects[1] is NULL, then nothing is done
      to results[i].m_objects[0].
      If results[i].m_objects[0] is NULL and results[i].m_objects[1]
      is not NULL, then results[i].m_objects[1] is added.
  Returns:
    Return true if Rhino should process the information 
    in results[] and false otherwise.
  */
  virtual
  CRhinoObject* EditObjects(
     const class CRhinoHistoryRecord& history_record,
     ON_SimpleArray<CRhinoObjectPair>& results
     );

  // Description:
  //   Get persistent command settings
  CRhinoSettings& Settings();

  // Description:
  //   Loads persistenct command settings from the Rhino profile.
  // Parameters:
  //   lpszSection - [in] profile section to use for this command
  //   pc - [in] profile context to use to get at
  //             value saved in the Rhino profile.
  // Remarks:
  //   Most commands do not override LoadProfile.  Use LoadProfile
  //   to get command options that persist between Rhino sessions.
  //   The default does nothing.
  virtual
  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );

  // Description:
  //   Save persistent command settings to the Rhino profile.
  // Parameters:
  //   lpszSection - [in] profile section to use for this command
  //   pc - [in] profile context to use to get at
  //             value saved in the Rhino profile.
  // Remarks:
  //   Most commands do not override SaveProfile.  Use SaveProfile
  //   to save command options that persist between Rhino sessions.
  //   The default does nothing.
  virtual
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );

  //////////
  // Override this function to display help for this command.
  virtual 
  void DoHelp();

  //////////
  // If you need to draw visual cues during your command, then
  // override DynamicDraw().  
  // Call EnableDynamicDraw(true/false) to enable/disable
  // the use of your DynamicDraw() function.  
  // Call RhinoApp().UpdateViews()
  // when you need to force the view to be updated.
  virtual 
  void DynamicDraw( class CRhinoView& );

  //////////
  //Description:
  // The Command Context window will call this function when it is attempting
  // to show help for this command. Override this function if you have a help
  // topic that you can display in the Command Context window.
  //Parameters:
  // url - [out] URL to your command's help topic. The Command Context window is
  //       an embedded web browser control, so any URL that you can display in a
  //       web browser would be valid. The base class function returns a URL that
  //       is formatted to show a page inside the Rhino CHM help file.
  //       For example, Rhino's line command returns a string like the following:
  //       mk:@MSITStore:C:\Program%20Files\Rhinoceros%204.0\English\Help\rhinov4.chm::/Commands/Lines.htm#Line_command
  //Returns:
  // true if the Command Context should update to display the url
  // false if you don't want the Command Context window to change it's current url
  //Remarks:
  // The recommended approach is to create a URL that points to an HTML file that
  // is on the user's computer. This speeds up the display of your help topic and
  // doesn't make your command look like it is running slow when the Command Context
  // auto-update is on.
  virtual bool ContextHelpURL( ON_wString& url ) const;

  //////////
  // Returns true if a command is "transparent"; i.e., the command
  // does not modify the contents of the model's geometry in any way.
  // Examples of transparent commands include commands that change
  // views and toggle snap states.  Any command that adds or deletes,
  // a view cannot be transparent.
  bool IsTransparent() const;

  //////////
  // Returns true if a command is "repeatable"; i.e., the command
  // can be repeated by pressing ENTER immediately after the
  // command finishes.
  bool IsRepeatable() const;

  //////////
  // Returns true if a command is a system command.
  // (Most commands are not and SDK developers can ignore this field.)
  bool IsSystem() const;

  //////////
  // Returns true if a command is a "test" command that is
  // not available in the commercial release.  All commands
  // derived from CRhinoTestCommand are "test" commands.
  bool IsTestCommand() const;


  /*
  Description:
    Returns true if this command's actions can be
    undone by the Rhino "Undo" command.
  See Also:
    CRhinoCommand::EnableUndo
  */
  bool IsUndoable() const;

  /*
  Description:
    By default, all commands are undoable.  If you
    have a command that should not be undoable, then
    call EnableUndo(false) in the constructor for the
    command's class.
  Parameters:
    bUndoable - [in] 
  Remarks:
    It is a rare command that should call EnableUndo.
    Enable undo should only be called in the constructor
    for the command.  Under no circumstances should enable
    undo be called while RunCommand is active.
  See Also:
    CRhinoCommand::IsUndoable
  */
  void EnableUndo( bool bUndoable = true );

  //////////
  // Returns true if a command is an "alpha" command.
  bool IsAlphaCommand() const;

  /*
  Returns:
    True if the command is a script runner command like
    "ReadCommandFile" or the RhinoScript plug-in's "ReadScript".
  Remarks:
    In the depths of the Rhino core, "ordinary" commands
    and "script running" commands need to be treated a little
    differently so that commands run in a script behave exactly
    as if they were being run from the command line.
  */
  bool IsScriptRunnerCommand() const;

  void EnableRepeatable(BOOL32 bRepeatable=true);

  // If command is in a plug-in, this returns the pointer to the
  // plug-in.
  const class CRhinoPlugIn* PlugIn() const;
#if defined (ON_RUNTIME_APPLE)
  void SetPlugIn (class CRhinoPlugIn* );
#endif

  const unsigned int& CommandFlags() const;

  /*
  Description:
    By default, object attribute changes do not trigger
    history replay. If you want objects created with a command
    to trigger history replay when object attributes change, 
    call EnableHistoryReplayOnObjectAttributeChange(true) 
    in the constructor for the command's class.
  Parameters:
    bEnable - [in]
  See Also:
    CRhinoCommand::HistoryReplayOnObjectAttributeChange
  */
  void EnableHistoryReplayOnObjectAttributeChange(bool bEnable = true);

  /*
  Description:
    Returns true if objects created with this command should trigger
    history replay when object attributes change.
  See Also:
    CRhinoCommand::EnableHistoryReplayOnObjectAttributeChange
  */
  bool HistoryReplayOnObjectAttributeChange() const;

private:
  friend unsigned int RhCommandFlags_( const CRhinoCommand* );
  friend void RhSetCommandFlags_( CRhinoCommand*, unsigned int );
  friend class CRhCommandManager;
  friend class CRhCommand;
  friend class CRhMiniDumpHelper;
  friend class CRhinoHistoryManager;
  unsigned int m_cmd_flags; // 0x00000001 = transparent
                            // 0x00000002 = repeatable
                            // 0x00000004 = test command (not part of commercial release)
                            // 0x00000008 = system command
                            // 0x00000010 = alpha command
                            // 0x00000020 = not undoable
                            // 0x00000040 = script runner command (see IsScriptRunnerCommand)
                            // 0x00000080 = system or script runner command that should cull old undo information
                            // 0x00000100 = attribute change triggers history replay

  class CRhinoPlugIn* m_plugin;
  class CRhinoSettings* m_settings = nullptr;
  UUID m_plugin_uuid;
  UUID m_command_uuid; // the value returned by the virtual CommandUUID() is saved here
                       // when the command is registered.
  wchar_t m_command_name[32]; // The first 31 characters of the english command name 
                              // are saved here when the command is registered.
                              // This information is used for minidumps.
private:
  // prohibit by providing no implementation
	CRhinoCommand(const CRhinoCommand &) = delete;
	CRhinoCommand & operator=( const CRhinoCommand &) = delete;
};

/*
Description:
  For adding nestable selection commands that work like the 
  native Rhino SelCrv command, derive your command from 
  CRhinoSelCommand and override the virtual SelFilter
  function.
*/
class RHINO_SDK_CLASS CRhinoSelCommand : public CRhinoCommand
{
public:
  CRhinoSelCommand( 
    const class CRhinoPlugIn* pPlugIn = nullptr,
    bool bTestCommand = false
    );

  /*
  Description:
    Override this virtual function and
    Return true if object should be selected.
  Returns:
    true if the object should be selected.
  */
  virtual bool SelFilter(const CRhinoObject*) = 0;

  /*
  Description:
    Do not need to override CRhinoSelCommand::RunCommand().
  */
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );


  /*
  Description:
    By default, light objects are not passed to SelFilter().
    If your command wants to be able to test light objects
    for selection, then call TestLights(true) in your 
    constructor.
  Parameters:
    bTestLights - [in]
  */
  void TestLights(bool bTestLights);


  /*
  Description:
    By default, grip objects are not passed to SelFilter().
    If your command wants to be able to test grip objects
    for selection, then call TestGrips(true) in your 
    constructor.
  Parameters:
    bTestGrips - [in]
  */
  void TestGrips(bool bTestGrips);

  /*
  Description:
    By default, a summary message is printed in the command
    prompt that list the number and kind of objects that
    were selected.  If you want to suppress this message,
    then call BeQuiet(true) in your constructor.
  Parameters:
    bQuiet - [in]
  */
  void BeQuiet(bool bQuiet);

protected:
  bool m_bQuiet;
  bool m_bTestLights;
  bool m_bTestGrips;
  bool m_bIgnoreGripsState;
  bool m_bIgnoreLayerLocking;
  bool m_bIgnoreLayerVisibility;

  // These fields are fur use after the
  // SDK is frozen.  The defaults are zero.
  bool m_bReserved0;
  bool m_bReserved1;
  int m_reserved1;
  int m_reserved2;
  ON__UINT_PTR m_sdk_reserved = 0;
};

/*
Description:
  For adding nestable whole object & subobject selection commands, derive your command from
  CRhinoSelSubObjectCommand and override the virtual SelFilter and SelSubObjectFilter
  functions.
*/
class RHINO_SDK_CLASS CRhinoSelSubObjectCommand : public CRhinoSelCommand
{
public:
  CRhinoSelSubObjectCommand( const class CRhinoPlugIn* pPlugIn = nullptr, bool bTestCommand = false )
    : CRhinoSelCommand(pPlugIn, bTestCommand)
  {}

  /*
  Description:
    To select subobjects, override this virtual function, add component indices of the subobjects that 
    should get selected to indices_to_select array and return true.
    This is called only if the SelFilter returns false and the whole object does not get selected.
  Returns:
    true if components added to indices_to_select should get selected.
  */
  virtual bool SelSubObjectFilter(const CRhinoObject*, ON_SimpleArray<ON_COMPONENT_INDEX>& indices_to_select) = 0;

  /*
  Description:
    Do not need to override CRhinoSelSubObjectCommand::RunCommand().
  */
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};


/*
Description:
  For advanced users only.  Support is not available.
  Commands like the RhinoScript plug-in's RunScript() that want
  to run scripts as if they were typed at the command line,
  should be derived from CRhinoScriptCommand.
*/
class RHINO_SDK_CLASS CRhinoScriptCommand : public CRhinoCommand
{
public:
  CRhinoScriptCommand( 
    bool bTransparent = false,
    bool bDoNotRepeat = false,
    const class CRhinoPlugIn* pPlugIn = nullptr,
    bool bTestCommand = false
    );
};

class RHINO_SDK_CLASS CRhinoTestCommand : public CRhinoCommand
{
public:
  /*
  Description:
    For each class derived from CRhinoCommand, there is only a single
    static instance that exists in the .CPP file where the class is
    defined.  No other instances of a class derived from CRhinoCommand
    or CRhinoTestCommand should ever be created.
  Parameters:
    bTransparent - [in] true if command is transparent and can be run
                   inside of other commands.
    bDoNotRepeat - [in] true if command should not be repeated.
    pPlugIn - [in] if command is part of a plug-in, the plug-in pointer must
        be passed here.  (This is automatically handled by the plug-in
        wizard.)
  Remarks:
    All commands derived from CRhinoTestCommand are "test" commands and
    are not available in the commercial release.
  */
  CRhinoTestCommand( 
    bool bTransparent = false,
    bool bDoNotRepeat = false,
    const class CRhinoPlugIn* pPlugIn = nullptr
    );
};


class RHINO_SDK_CLASS CRhinoHiddenCommand : public CRhinoCommand
{
public:
  /*
  Description:
    Commands that should be localized by whose names should not autocomplete
    should be derived from CRhinoHiddenCommand.
    These are typically used when modeless UI changes the document
    and the change needs to be undoable by the undo command.
  Parameters:
    bTransparent - [in] true if command is transparent and can be run
                   inside of other commands.
    bDoNotRepeat - [in] true if command should not be repeated.
    pPlugIn - [in] if command is part of a plug-in, the plug-in pointer must
        be passed here.  (This is automatically handled by the plug-in
        wizard.)
  Remarks:
    All commands derived from CRhinoTestCommand are "test" commands and
    are not available in the commercial release.
  */
  CRhinoHiddenCommand(
    bool bTransparent = false,
    bool bDoNotRepeat = false,
    const class CRhinoPlugIn* pPlugIn = nullptr
  );
};

/*
Description:
  Base class for all transformation-type commands.
*/
class RHINO_SDK_CLASS CRhinoTransformCommand : public CRhinoCommand
{
public:
  CRhinoTransformCommand( 
        bool bTransparent = false,
        bool bDoNotRepeat = false,
        const class CRhinoPlugIn* pPlugIn = nullptr,
        bool bTestCommand = false
        );

  CRhinoCommand::result SelectObjects(const wchar_t* prompt, class CRhinoXformObjectList& list);
  CRhinoCommand::result SelectObjects(const wchar_t* prompt, class CRhinoXformObjectList& list, bool bAllowAllGrips);

  CRhinoCommand::result SelectObjects(const wchar_t* prompt, unsigned int geometry_filter, class CRhinoXformObjectList& list);
  CRhinoCommand::result SelectObjects(const wchar_t* prompt, unsigned int geometry_filter, class CRhinoXformObjectList& list, bool bAllowAllGrips);

  unsigned int DefaultGeometryFilter() const;

  void TransformObjects(CRhinoDoc& doc, class CRhinoXformObjectList& list, ON_Xform& xform, BOOL32 bCopy = FALSE, BOOL32 bAutoHistory = FALSE);
  void DuplicateObjects(CRhinoDoc& doc, class CRhinoXformObjectList& list);

  // Description:
  //  sets dynamic grip locations back to starting grip locations.
  //  This makes things like the Copy command work when grips are
  //  "copied".
  void ResetGrips(CRhinoDoc& doc, class CRhinoXformObjectList& list);

  CRhinoView* View() { return m_view; }
  bool ObjectsWerePreSelected() { return m_objects_were_preselected; }

private:
  CRhinoView* m_view = nullptr;
  bool m_objects_were_preselected = false;
};


//////////
// CRhinoHistory simply manages a pointer to an ON_HistoryRecord
// and frees command developers from having to carefully 
// create the CRhinoHistoryRecord pointer in a way that 
// never leaks memory.
class RHINO_SDK_CLASS CRhinoHistory
{
private:
  CRhinoHistory() = delete;

public:
  ~CRhinoHistory();
  CRhinoHistory(const CRhinoHistory&);
  CRhinoHistory(const CRhinoCommand&);
  CRhinoHistory(const class CRhinoHistoryRecord&);
  CRhinoHistory(ON_HistoryRecord*&);
  CRhinoHistory& operator=(const CRhinoHistory&);

  /*
  Description:
    Specify a non-zero integer that identifies the version of
    this history record. The virtual ReplayHistory() functions
    can check this version to avoid replaying history using
    information created by earlier versions of the command.
  Parameters:
    history_version - [in] any non-zero integer.  It is strongly
                           suggested that something like
                           YYYYMMDD be used.
  Returns:
    True if successful.
  */
  bool SetHistoryVersion( int history_version );

  /*
  Description:
    Set the command whos ReplayHistory override
    will be used to update the command's output objects
    if an input changes.
  Parameters:
    cmd - [in]
  Remarks:
    You can either set the command by passing it to the
    constructor or by using this function.
  */
  bool SetCommand( const CRhinoCommand& cmd );

  /*
  Description:
    For setting values when commands are creating history records.
  Parameters:
    value_id - [in]
      If there a value with the same input
      id exists, the old value is replaced.
    count - [in]
      Number of values
    b - [in]
      array of count bools
    i - [in]
      array of count ints
    x - [in]
      array of count doubles
    p - [in]
      array of count 3d points
    v - [in]
      array of count 3d vectors
    xform - [in]
      array of count xforms
    c - [in]
      array of count colors
    or - [in]
      array of count object references
    g - [in]
      array of count geometry pointers
    u - [in]
      array of uuids
    s - [in]
      string
  */
  bool SetBoolValue(     int value_id, bool b);
  bool SetIntValue(      int value_id, int i);
  bool SetDoubleValue(   int value_id, double x);
  bool SetPointValue(    int value_id, ON_3dPoint p);
  bool SetVectorValue(   int value_id, ON_3dVector v);
  bool SetXformValue(    int value_id, ON_Xform xform);
  bool SetColorValue(    int value_id, ON_Color c);
  bool SetObjRefValue(   int value_id, const class CRhinoObjRef& objref);
  bool SetPointOnObjectValue( int value_id, const class CRhinoObjRef& objref, ON_3dPoint point );
  bool SetUuidValue(     int value_id, ON_UUID uuid );
  bool SetStringValue(   int value_id, const wchar_t* s );
  bool SetGeometryValue( int value_id, ON_Geometry* g);
  bool SetCurveValue(    int value_id, const ON_Curve& curve );
  bool SetSurfaceValue(  int value_id, const ON_Surface& surface );
  bool SetBrepValue(     int value_id, const ON_Brep& brep );
  bool SetMeshValue(     int value_id, const ON_Mesh& mesh );
  bool SetSubDEdgeChainValue(int value_id, const ON_SubDEdgeChain& subd_edge_chain);
  bool SetPolyEdgeValue( CRhinoDoc& doc, int value_id, const class CRhinoPolyEdge& polyedge );

  /*
  Description:
    For setting values when commands are creating history records.
  Parameters:
    value_id - [in]
      If there a value with the same input
      id exists, the old value is replaced.
    count - [in]
      Number of values
    b - [in]
      array of count bools
    i - [in]
      array of count ints
    x - [in]
      array of count doubles
    P - [in]
      array of count 3d points
    V - [in]
      array of count 3d vectors
    xform - [in]
      array of count xforms
    c - [in]
      array of count colors
    or - [in]
      array of count object references
    g - [in]
      array of count geometry pointers
    u - [in]
      array of uuids
    s - [in]
      array of strings
  */
  bool SetBoolValues(     int value_id, int count, const bool* b);
  bool SetIntValues(      int value_id, int count, const int* i);
  bool SetDoubleValues(   int value_id, int count, const double* x);
  bool SetPointValues(    int value_id, int count, const ON_3dPoint* P);
  bool SetVectorValues(   int value_id, int count, const ON_3dVector* V);
  bool SetXformValues(    int value_id, int count, const ON_Xform* xform);
  bool SetColorValues(    int value_id, int count, const ON_Color* c);
  bool SetObjRefValues(   int value_id, int count, const class CRhinoObjRef* objref);
  bool SetUuidValues(     int value_id, int count, const ON_UUID* u );
  bool SetStringValues(   int value_id, int count, const wchar_t* const* s );
  bool SetStringValues(   int value_id, const ON_ClassArray<ON_wString>& s );
  bool SetGeometryValues( int value_id, const ON_SimpleArray<ON_Geometry*> a);
  bool SetSubDEdgeChainValues(int value_id, const ON_SimpleArray<const ON_SubDEdgeChain*>& a);
  bool SetPolyEdgeValues( CRhinoDoc& doc, int value_id, int count, const class CRhinoPolyEdge* const* polyedges );

  /*
  12 May, 2015 - Lowell
  When an object is replaced and the old object has a history record with
  this field set, the history record is copied and attached to the new object.
  That allows a descendant object to continue the history linkage after
  it is edited.  See http://mcneel.myjetbrains.com/youtrack/issue/RH-30399
  */
  bool CopyOnReplaceObject() const;

  /*
  Returns:
    True if successful.
    False if m_hr is nullptr and the value could not be set.
  */
  bool SetCopyOnReplaceObject(
    bool bCopyOnReplaceObject
    );

private:
  friend class CRhinoHistoryRecordTable;

  ON_HistoryRecord* m_hr;
  const class CRhinoHistoryRecord* m_history_record;
};


/*
Description:
  Public interface to lookup commands by name and uuid
*/
class RHINO_SDK_CLASS CRhinoCommandManager
{
public:

  /*
  Description:
    Lookup a command from it uuid.
  Parameters:
    command_uuid - [in]
  Returns:
    Pointer to the command or NULL if no command was found
  */
  CRhinoCommand* LookupCommand( const UUID& command_uuid ) const;

  /*
  Description:
    Lookup a command from a command name using the same algorithm
    that is used when a user types thae command name.
  Parameters:
    command_name - [in] command name with optional 
      RHINO_SCRIPT_COMMAND_PREFIX and/or RHINO_ENGLISH_COMMAND_PREFIX
      characters in any order.
  Returns:
    Pointer to the command or NULL if no command was found
  */
  CRhinoCommand* LookupCommandName( const wchar_t* command_name ) const;

  /*
  Description:
    Lookup a command from the english command name.
  Parameters:
    command_name - [in] english command name.
  Returns:
    Pointer to the command or NULL if no command was found
  */
  CRhinoCommand* LookupEnglishCommandName( const wchar_t* english_command_name ) const;

  /*
  Description:
    Lookup a command from the local command name.
  Parameters:
    command_name - [in] local command name.
  Returns:
    Pointer to the command or NULL if no command was found
  */
  CRhinoCommand* LookupLocalCommandName( const wchar_t* local_command_name ) const;

  //Description:
  //  Get list of command names in Rhino. This list does not include Test, Alpha, or System commands
  //Parameters:
  //  names   - [out] names for every command are appended to this list
  //  english - [in]  if true, retrieve the english name for every command
  //                  if false, retrieve the local name for every command
  //  loaded  - [in]  if true, only get names of currently loaded commands
  //                  if false, get names of all registered (may not be currently loaded) commands
  //Returns:
  //  Number of commands appended to the list.
  int GetCommandNames( ON_ClassArray<ON_wString>& names, bool english, bool loaded ) const;

private:
  friend class CRhCommandManager;
  CRhinoCommandManager() = default;
  ~CRhinoCommandManager() = default;
};

