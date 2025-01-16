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

#include "rhinoSdkObject.h"
#include "rhinoSdkCommandOption.h"
#include "rhinoSdkCommand.h"

// TODO: Change this to a CRhinoApp::RhinoHistoryManager()
// member function on CRhinoApp as soon as John checks in
// the CRhinoApp header file.
RHINO_SDK_FUNCTION
class CRhinoHistoryManager* RhinoHistoryManager();

RHINO_SDK_FUNCTION
class CRhinoHistoryManager* RhinoHistoryManager(class CRhinoDoc* doc);


class RHINO_SDK_CLASS CRhinoHistoryManager
{
public:
  /*
  Description:
    Sets  switch for history recording.  
    If the master history recording switch is off, 
    then no history is recorded.  If the master history
    recording switch is on, the the command's history
    recording setting determines if a command records
    history.
  Parameters:
    bEnableHistoryRecording - [in]
  */
  void EnableHistoryRecording( bool bEnableHistoryRecording );

  /*
  Description:
    When history update is enabled, dependent objects are
    automatically updated when an antecedent is modified.
  Parameters:
    bEnableHistoryUpdate - [in] true to enable history update
  */
  void EnableHistoryUpdate( bool bEnableHistoryUpdate );

  /*
  Description:
    When history object locking is enabled, objects with
    history on them act as if they were locked and the only
    way to modify these objects is to edit their inputs.
  Parameters:
    bEnableHistoryObjectLocking - [in]
  */
  void EnableHistoryObjectLocking( bool bEnableHistoryObjectLocking );

  /*
  Description:
    When history recording is enabled, new objects keep
    a record of how they were constructed so that they can
    be updated if an input object changes.
  Returns: 
    The current history recording setting.  If a command is
    running, the current setting is true only when the
    master switch is on and the command has recording
    enabled.
  Remarks:
    This reports the CURRENT APPLICATION setting.  
    In order for history to be recorded during a 
    command, both the application setting and the command setting
    have to be true.    
  */
  bool HistoryRecordingEnabled() const;


  /*
  Description:
    When history update is enabled, dependent objects are
    automatically updated when an antecedent is modified.
  Returns: 
    true if history update is enabled.
  */
  bool HistoryUpdateEnabled() const;


  /*
  Description:
    When history update is enabled, dependent objects are
    automatically updated when an antecedent is modified.
  Returns: 
    true if history update is enabled.
  */
  bool HistoryObjectLockingEnabled() const;

  /*
  Description:
  Returns: 
    true if the history manager is in the process of updating
    objects because their antecedents were modified.
  */
  bool HistoryUpdateActive() const;


  // Expert user tools 
  //  - no support available 
  //  - can change without notice
  CRhinoCommand::result UpdateDescendants(class CRhinoDoc& doc);
  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  
  /*
  Description:
    Puts a the current value of the record setting on a private stack.
  */
  void PushRecordSetting();

  /*
  Description:
    Sets the current value of the record setting to the 
    value on the stack.
  Returns:
    Value of the record setting after the pop.
  */
  bool PopRecordSetting();

  /*
  Returns:
    Number of values in the record setting stack.
  */
  int RecordSettingStackCount() const;


  bool RecordNextCommand() const;
  void SetRecordNextCommand( bool bRecordNextCommand );

  bool BrokenRecordWarningEnabled() const;
  void EnableBrokenRecordWarning( bool bEnableBrokenRecordWarning );

  /*
  Description:
    These tools check for and alert users when history
    records are broken.
  Parameters
    action - [in]
      0 command
      1 object drag
    cmd - [in] (can be NULL)
      command
  */
  int BrokenRecordCount();
  void ZeroBrokenRecordCounter();
  void CheckForBrokenRecords( 
          int action, 
          unsigned int rhino_doc_sn,
          const CRhinoCommand* cmd 
          );

private:
  // Note: No data members of this class are public.  This class
  //      can be modified without breaking the SDK as long as existing
  //      public member functions are not changed.
  friend class CRhinoApp;
  friend class CRhinoDoc;
  friend class CRhinoInstanceDefinitionTable;

public:
  // TODO: Make these private when RhinoHistoryManager
  // turns into CRhionApp:HistoryManager()
  CRhinoHistoryManager();
  ~CRhinoHistoryManager();
private:

  CRhinoHistoryManager(const CRhinoHistoryManager&);
  CRhinoHistoryManager& operator=(const CRhinoHistoryManager&);

  /*
  Description:
    Called in CRhinoDoc::ReplaceObject to inform the history 
    manager that an object is about to be replaced.
  */
  bool ReplaceObjectHelper(const CRhinoObject* old_object,CRhinoObject* new_object);

  /*
  Description:
    Called in CRhinoInstanceDefinitionTable::ModifyInstanceDefinitionGeometry to
    inform the history manager that an objects instance definition has been updated
  */
  bool InstanceDefinitionUpdateHelper(const CRhinoObject* updatedObject);

public:
  /*
  Description:
    Called by Trim and Split when using replace object is not feasible.
  */
  bool DeleteObjectHelper(const CRhinoObject* old_object);

  /*
  Description:
    Called by CRhinoObject::ModifyAttributes to trigger history replay when specific parent attributes change.
  */
  bool ModifyObjectAttributesHelper(const CRhinoObject* object);

private:


  // The master switch must be on for history recording
  // to be enabled.  The current switch can be off if
  // a specific command has temporarily disabled history 
  // recording.
  bool m_bEnableHistoryRecordingMasterSwitch;

  bool m_bEnableHistoryRecording; // current "global" setting
  bool m_bEnableHistoryUpdate;
  bool m_bEnableHistoryObjectLocking;

  bool m_bRecordNextCommand;      // record setting for the next command

  bool m_bUpdateInProgress;

  //ON_SimpleArray<CRecordingSettings> m_recording_settings_stack;

  // During a command, when an antecedent object is replaced, 
  // the ids of its descendants are added to this array. When
  // the command ends, the objects listed in the m_dependant_objects[]
  // list are gone through to see if they can be updated.  
  // If history update is off, then m_dependant_objects[] is 
  // emptied at the end of the command.
  ON_SimpleArray<ON_UUID> m_dependant_objects;

  // m_cmd_settings is an array of commands with
  // non-default settings.
  //class CCmdSettings
  //{
  //public:
  //  ON_UUID m_cmd_id;
  //  unsigned int m_cmd_history_recording;
  //  static int CompareCmdId(const CCmdSettings*,const CCmdSettings*);
  //};
  ON_SimpleArray<bool> m_bRecordSettingStack;

  // number of times CRhinoDoc::ReplaceObject() has broken history
  int m_broken_record__counter;

  // number of times CRhinoDoc::ReplaceObject() has helped a captive
  // escape from its cage.
  int m_escaped_captive__counter;

  bool m_bEnableBrokenRecordWarning; // true if annoying "you broke history" 
                                     // dialog shows up when edit objects with
                                     // antecedents

  bool m_bEnableEscapedCaptiveWarning; // true if annoying "you changed a captive" 
                                     // dialog shows up when edit captive objects
};


/*
	Command instance specific history recording support.
	Description:
		This allows the global history recording option to be optionally turned on or off for this
		single instance of the command execution.  When command execution terminates the initial setting
		of History Recording is restored.  The option is implemented by
		a command line toggle option.  The option is allowed if and only if 
		command specific history recording is enabled.  	

*/
class RHINO_SDK_CLASS CRhinoRecordHistoryCommandOptionHelper 
{
public:
  // Value of RhinoHistoryManager()->HistoryRecordingEnabled()
  // when constructor was run.
  const bool m_bSavedEnableHistoryRecording;

  /*
  Description:
    The constructor saves the value of
    RhinoHistoryManager()->HistoryRecordingEnabled()
    in m_bSavedEnableHistoryRecording and sets
    m_bShowHistoryRecordOption = m_bDefaultShowHistoryRecordOption;

  */
	CRhinoRecordHistoryCommandOptionHelper(); 

  /*
  Description:
    The destructor calls
    RhinoHistoryManager()->EnableHistoryRecording(m_bSavedEnableHistoryRecording)
  */
	~CRhinoRecordHistoryCommandOptionHelper();

  /*
  Description:
    Adds a 
      RecordHistory=yes/no 
    command line option.  The value of the option is in
    m_bRecordHistoryOptionValue.
  Parameters:
    getter - [in]
  Returns:
    option index (-1 if option is not added)
  Remark:
    If m_bShowHistoryRecordOption is false, then
    not option is added.  This provides an easy way to
    get rid of the command option by changing a single
    bool.
  */
  int AddRecordHistoryCommandOption( CRhinoGet* getter );

  // Current value of RecordHistory command option.
  bool m_bRecordHistoryOptionValue;

private:
  CRhinoRecordHistoryCommandOptionHelper(const CRhinoRecordHistoryCommandOptionHelper&);
  CRhinoRecordHistoryCommandOptionHelper& operator=(const CRhinoRecordHistoryCommandOptionHelper&);
  // If CRhinoCommandHistorySettingsHelper::m_bUseCommandInstanceToggle
  // is true, then commands that support history get the 
  // RecordHistory=yes/no toggle command line switch
	static bool m_bShowHistoryRecordOption;
};

