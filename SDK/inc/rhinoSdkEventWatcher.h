//
// Copyright (c) 1993-2023 Robert McNeel & Associates. All rights reserved.
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

#include "rhinoSdkApp.h"
#include "rhinoSdkDoc.h"

/*
Description:
  The CRhinoUuidCollection class provides a tool
  to efficiently maintain a collection of uuids
  and determine if a uuid is in the collection.

  This class is useful when an event watcher needs
  to watch for events happening to a specific object.
  Simply add a CRhinoUuidCollection member to your
  event watcher class and then 
*/
class RHINO_SDK_CLASS CRhinoUuidCollection : private ON_SimpleArray<ON_UUID>
{
public:
  CRhinoUuidCollection();
  CRhinoUuidCollection(int capacity);
  ~CRhinoUuidCollection();
  CRhinoUuidCollection(const CRhinoUuidCollection& src);
  CRhinoUuidCollection& operator=(CRhinoUuidCollection& src);

  /*
  Returns:
    Number of active uuids in the collection.
  */
  int Count() const;

  /*
  Description:
    Provides an efficient way to empty a collection so that it
    can be used again.
  */
  void Empty();

  /*
  Description:
    Adds a uuid to the collection.
  Parameters:
    uuid - [in] id to add.
    bCheckForDupicates - [in] if true, then the uuid
       is not added if it is already in the collection.
       If you know uuid is not in the collection or
       you don't care about dupicates, and you are
       going to have a large collection of uuids,
       then setting bCheckForDupicates=false will
       speed up the addition of uuids.
  Returns:
    True if uuid was added.  False if uuid was not added
    because it is already in the collection.
  */
  bool AddUuid(ON_UUID uuid, bool bCheckForDupicates=true);

  /*
  Description:
    Adds a uuid to the collection.
  Parameters:
    uuid - [in] id to remove
  Returns:
    True if uuid was in collection and was removed.
    False if uuid was not in the collection.
  */
  bool RemoveUuid(ON_UUID uuid);

  /*
  Description:
    Determine if a uuid is in the collection.
  Returns:
    True if uuid is in the collection.
  */
  bool FindUuid(ON_UUID uuid) const;

private:
  ON_UUID* SearchHelper(const ON_UUID*) const;
  int m_sorted_count;
  int m_removed_count;
};


/*
Description:
  This class is a virtual base class that defines the interface for watching
  events in Rhino.  If you want to watch one or more events, derive a class
  from CRhinoEventWatcher and override the virtual functions that correspond to
  the events you want to watch.

  You will only be notified if your event watcher is registered and enabled.
  You must explicitly register your event watcher by calling Register() and
  then enable your event watcher by calling Enable(true).
*/
class RHINO_SDK_CLASS CRhinoEventWatcher
{
public:
  CRhinoEventWatcher();

  /*
    Description:
      By default event watchers are not called for headless docs and headless app
      scenarios. Use this constructor if you want to make your event watcher be
      called for these headless cases. Note that there is a good chance that your
      event watcher can be called on a different thread than expected when dealing
      with "headless"
  */
  CRhinoEventWatcher(bool headlessDocAware, bool headlessAppAware);
  virtual ~CRhinoEventWatcher();

  /*
    Description:
      Adds event watcher to Rhino.    

    Example:
             class CMyEventWatcher : public CRhinoEventWatcher
             {
               ...
             }
             ...
             CMyEventWatcher* pEyes = new CMyEventWatcher();
             ...
             pEyes->Register();

             pEyes->Enable(TRUE);
             ...
             pEyes->Enable(FALSE);
             ...
             pEyes->Enable(TRUE);
             ...
             pEyes->Enable(FALSE);
             ...

             pEyes->UnRegister();
 
    Returns:
      TRUE if registration succeeds.

    Remarks:
      Registration is computationally expensive and should be 
      done sparingly. Enable is fast and can be used to quickly 
      enable/disable an event watcher.
  */ 
  void Register();

  /*
    Description:
      Removes event watcher from Rhino.

    Remarks:
      Registration is computationally expensive and should be 
      done sparingly. Enable is fast and can be used to quickly 
      enable/disable an event watcher.

    See Also: 
      CRhinoEventWatcher::Register();
  */
  void UnRegister();

  /*
    Description:
      You will only be notified if your event watcher
      is reistered and enabled.  You must explicitly
      register and then enable your even watcher.

    Returns:
      TRUE if the event manager is registered and enabled.

    See Also: 
      CRhinoEventWatcher::Register();
  */
  bool IsEnabled() const;

  /*
    Description:
      You will only be notified if your event watcher
      is reistered and enabled.  You must explicitly
      register and then enable your even watcher.
  
    Parameters:
      b - [in] TRUE enables the event watcher and FALSE
          disables the event watcher.
  
    Returns:
      TRUE if the event manager is registered and enabled.
  
    Remarks:
      Enable() is fast and efficient.  Use it when you need to
      repeatedly enable/disable your event watcher.  Call
      UnRegister() when you are done watching events.
    See Also: 
      CRhinoEventWatcher::Register();
  */
  void Enable( BOOL32 b = TRUE );

  /*
    Description:
      Called when this event watcher is enabled/disabled.
  
    Parameters:
      b - [in] TRUE means event watcher is being enabled.
  
    Remarks:
      For advanced users.
  */
  virtual
  void OnEnableEventWatcher( BOOL32 b );

  ///////////////////////////////////////////////////////////////////
  //
  // Application events
  //

  /*
  // Description:
  //   Called when Rhino starts.
  //
  // Parameters:
  //   app - [in] instance of Rhino begin started.
  WARNING: 
     Never modify the Rhino document in an OnInitRhino() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnInitRhino() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnInitRhino(class CRhinoApp& app);

  /*
  // Description:
  //   Called when Rhino stops.
  //
  // Parameters:
  //   app - [in] instance of Rhino begin closed.
  WARNING: 
     Never modify the Rhino document in an OnCloseRhino() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnCloseRhino() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnCloseRhino(class CRhinoApp& app);

  ///////////////////////////////////////////////////////////////////
  //
  // Document events
  //
  /*
  WARNING: 
     Never modify the Rhino document in an OnCloseDocument() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnCloseDocument() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnCloseDocument( CRhinoDoc& doc );

  /*
  WARNING: 
     Never modify the Rhino document in an OnNewDocument() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnNewDocument() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnNewDocument( CRhinoDoc& doc );

  // Description:
  //   Called when the active document changes.
  // Parameters:
  //   doc - [in] The new current document or null if no documents are
  //         currently open.
  virtual
  void OnActiveDocumentChanged(CRhinoDoc* doc);

  /*
  WARNING:
     Never modify the Rhino document in an OnBeginOpenDocument() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnBeginOpenDocument() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnBeginOpenDocument( 
      CRhinoDoc& doc,
      const wchar_t* filename,
      BOOL32 bMerge,
      BOOL32 bReference
      );

  /*
  WARNING: 
     Never modify the Rhino document in an OnEndOpenDocument() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnEndOpenDocument() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnEndOpenDocument( 
      CRhinoDoc& doc,
      const wchar_t* filename,
      BOOL32 bMerge,
      BOOL32 bReference
      );

  /*
  WARNING: 
     Never modify the Rhino document in an OnBeginSaveDocument() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnBeginSaveDocument() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnBeginSaveDocument( 
      CRhinoDoc& doc,
      const wchar_t* filename,
      BOOL32 bExportSelected
      );

  /*
  WARNING: 
     Never modify the Rhino document in an OnEndSaveDocument() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnEndSaveDocument() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnEndSaveDocument( 
      CRhinoDoc& doc,
      const wchar_t* filename,
      BOOL32 bExportSelected
      );

  /*
  WARNING: 
     Never modify the Rhino document in an OnDocumentPropertiesChanged() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnDocumentPropertiesChanged() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnDocumentPropertiesChanged( 
      CRhinoDoc& doc
      );


  /*
  Description:
    Called when a change in the model units results in a scaling operation on all of the objects in the document.
	This call is made before any of the objects are scaled.  A call to OnDocumentPropertiesChanged follows.
  */
  virtual
  void OnUnitsChangedWithScaling( 
      CRhinoDoc& doc, double scale
      );
  

  /*
  Description:
    Called after a value in CRhinoApp::AppSettings() changes.
  WARNING: 
     Never modify the Rhino document in a OnAppSettingsChanged() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnAppSettingsChanged() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnAppSettingsChanged( 
      const CRhinoAppSettings& new_app_settings
      );

  ///////////////////////////////////////////////////////////////////
  //
  // View events
  //

  /*
  WARNING: 
     Never modify the Rhino document in a view event override.
     If you need to change the Rhino document or post update messages
     to controls, then your view event override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual void OnCreateView( CRhinoView* rhino_view );
  virtual void OnDestroyView( CRhinoView* rhino_view );
  virtual void OnSetActiveView( CRhinoView* rhino_view );
  virtual void OnRenameView( CRhinoView* rhino_view );


  /*
  // Description:
  //   Called just before command.RunCommand().
  WARNING: 
     Never modify the Rhino document in a OnBeginCommand() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnBeginCommand() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnBeginCommand( 
        const CRhinoCommand& command,
        const CRhinoCommandContext& context
        );

  /*
  // Description:
  //   Called immediately after command.RunCommand().
  WARNING: 
     Never modify the Rhino document in a OnEndCommand() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnEndCommand() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnEndCommand( 
        const CRhinoCommand& command,
        const CRhinoCommandContext& context,
        CRhinoCommand::result rc
        );


  /*
  Description:
    Asks if a layers can be deleted.
  Parameters:
    layer_index_list - [in]
      indices of layers to be deleted.
    bQuiet - [in]
      If bQuiet is true, make a decision and set the return 
      value accordingly.

      If bQuiet is false and the plug-in cares, the 
      plug-in should do something like the example.
      
  Example:

          int CMyEventWatcher::DeleteLayerCheck(
                      CRhinoDoc& doc,
                      ON_SimpleArray<int>& layer_index_list, 
                      bool bQuiet)
          {
            const int layer_index_count = layer_index_list.Count();
            int delete_count = 0;
            for ( int i = 0; i < layer_index_count; i++ )
            {
              const int layer_index = layer_index_list[i];

              if ( MyPlugInUsesLayer(layer_index) )
              {
                const CRhinoLayer& layer = doc.m_layer_table[layer_index];
                ON_wString msg;
                msg.Format(L"My plug-in uses layer %s. Do you really want to delete it?",
                           layer.LayerName());
                const int yn = ::RhinoYesNoMessageBox( 
                            msg, 
                            L"Delete Layer",
                            (1|2|4|8),  // Yes, Yes to All, No, No to All,
                            MB_ICONEXCLAMATION | MB_DEFBUTTON3);

                if( 1 == yn )
                {
                  // yes
                  delete_count++;
                  continue;
                }
                if( 2 == yn )
                {
                  // yes to all
                  delete_count += (layer_index_count-i);
                  break;
                }
                if( 3 == yn )
                {
                  // no
                  layer_index_list[i] = -1;
                  continue;
                }
                if( 4 == yn )
                {
                  // no to all
                  layer_index_list.SetCount(i);
                  break;
                }
              }
            }

            if ( 0 == delete_count )
              return 0;
            if ( layer_index_count == delete_count )
              return 1;
            return 2;
          }

  Returns:
    0 - Delete no layers
    1 - Delete all layers in list
    2 - Delete layers in layer_index_list[]

  WARNING: 
     Never modify the Rhino document in a DeleteLayerCheck() override.
     If you need to change the Rhino document or post update messages
     to controls, then your DeleteLayerCheck() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  int DeleteLayerCheck(
         CRhinoDoc& doc,
         ON_SimpleArray<int>& layer_index_list,
         bool bQuiet
        );


  ///////////////////////////////////////////////////////////////////
  //
  // Object events
  //

  /*
  // Description:
  //   Called if a new object is added to the document.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] new object
  WARNING: 
     Never modify the Rhino document in a OnAddObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnAddObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnAddObject(
    CRhinoDoc& doc,
    CRhinoObject& object
    );

  /*
  // Description:
  //   Called if an object is deleted.  At some later point
  //   the object can be un-deleted.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object that was deleted.
  WARNING: 
     Never modify the Rhino document in a OnDeleteObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnDeleteObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnDeleteObject(
    CRhinoDoc& doc,
    CRhinoObject& object
    );
  
  /*
  Description:
    Called if an object is about to be replaced.

  Parameters:
    doc - [in] object's document
    old_object - [in] existing object - about to be deleted.
      At the time OnReplaceObject() is called, old_object has
      not been deleted.
    new_object - [in] new object - about to be added.  At
      the time OnReplaceObject() is called, new_object has
      not been added to doc.

  Remarks:
    If both CRhinoDoc::UndoActive() or CRhinoDoc::RedoActive() are false,
    then immediatedly after OnReplaceObject(doc,old_object,new_object)
    is called there will be a call to OnDeleteObject(doc,old_object)
    and then a call to OnAddObject(doc,new_object).

    If either CRhinoDoc::UndoActive() and CRhinoDoc::RedoActive() are 
    true, then immediatedly after OnReplaceObject(doc,old_object,new_object)
    is called there will be a call to OnDeleteObject(doc,old_object)
    and then a call to OnUnDeleteObject(doc,new_object).

  WARNING: 
     Never modify the Rhino document in a OnReplaceObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnReplaceObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnReplaceObject(
  CRhinoDoc& doc,
  CRhinoObject& old_object,
  CRhinoObject& new_object
  );

  /*
  Description:
    When an object is replaced by calling CRhinoDoc::ReplaceObject(), 
    the following CRhinoEventWatcher event notifications are sent:

    CRhinoEventWatcher::OnReplaceObject(doc,objA,objB)
    
    If the object being replace is selected, then a
    CRhinoEventWatcher::OnDeselectObject(objA)
    notification occurs.
    
    CRhinoEventWatcher::OnDeleteObject(objA)

    If a new object is being added, then a
    CRhinoEventWatcher::OnAddObject objB)
    notification occurs.
    If a previous relace object is being undone, then a
    CRhinoEventWatcher::OnUndeleteObject (objB)
    occurs.

    If the object being replace is selected, then a
    CRhinoEventWatcher::OnSelectObject(objB)
    notification occurs

    CRhinoEventWatcher::OnObjectReplaced(doc,objA,objB)

  Parameters:
    rhino_doc_serial_number - [in]
      Serial number of the document.
      Use CRhinoDoc::FromRuntimeSerialNumber(rhino_doc_serial_number) to get a pointer
      to the document. 
      Always check for nullptr before dereferencing the returned value.
      Never modify the document in the callback.

    deleted_object_sn - [in]
      The value of the deleted object's CRhinoObject.RuntimeSerialNumber().
      Depending on Rhino application state and settings, this object may 
      no longer exist or may continue to exist for an undetermined duration
      to permit a possible future undo operation.
      Use CRhinoObjec::FromRuntimeSerialNumber( rhino_doc_serial_number, deleted_object_sn ) to
      get a pointer to the object. 
      Always check for nullptr before dereferencing the returned value.
      Never modify this object.

    added_object_sn - [in]
      The value of the new object's CRhinoObject.RuntimeSerialNumber()
      Use CRhinoObjec::FromRuntimeSerialNumber( rhino_doc_serial_number, added_object_sn ) to
      get a pointer to the object. 
      Always check for nullptr before dereferencing the returned value.
      Never modify this object.

  static const CRhinoObject* FromRuntimeSerialNumber( 
    unsigned int rhino_doc_runtime_serial_number,
    unsigned int rhino_object_runtime_serial_number 
    );

  WARNING: 
     Never modify any object or other Rhino document values 
     in a OnObjectReplaced() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnObjectReplaced() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnObjectReplaced(
    unsigned int rhino_doc_serial_number,
    unsigned int deleted_object_sn,
    unsigned int added_object_sn
    );

  /*
  // Description:
  //   Called if an object is un-deleted.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object tha was deleted and is now active.
  WARNING: 
     Never modify the Rhino document in a OnUnDeleteObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnUnDeleteObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnUnDeleteObject(
    CRhinoDoc& doc,
    CRhinoObject& object
    );

  /*
  // Description:
  //   Called if an object is begin purged from a document.
  //   The object will cease to exist forever.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object to be purged.
  WARNING: 
     Never modify the Rhino document in a OnPurgeObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnPurgeObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnPurgeObject(
    CRhinoDoc& doc,
    CRhinoObject& object
    );

  /*
  // Description:
  //   Called if an object is being selected.
  //   Note, the seletion of grip objects will not trigger this event.
  //   Grips selection is ignored, as these objects are transient and 
  /    are often deleted shortly after selection and deselection.
  //   If you must be notified when grip objects are selected, use the 
  //   CRhinoOnChangeObjectSelectState class.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object to be purged.
  WARNING: 
     Never modify the Rhino document in a OnSelectObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnSelectObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnSelectObject(
    CRhinoDoc& doc,
    const CRhinoObject& object
    );

  /*
  // Description:
  //   Called when a collection of objects is selected.
  //   Note, the seletion of grip objects will not trigger this event.
  //   Grips selection is ignored, as these objects are transient and
  /    are often deleted shortly after selection and deselection.
  //   If you must be notified when grip objects are selected, use the
  //   CRhinoOnChangeObjectSelectState class.
  //
  // Parameters:
  //   doc - [in] object's document
  //   objects - [in] objects that were selected
  WARNING: 
     Never modify the Rhino document in a OnSelectObjects() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnSelectObjects() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnSelectObjects(
    CRhinoDoc& doc,
    const ON_SimpleArray<const CRhinoObject*>& objects
    );

  /*
  // Description:
  //   Called if an object is being deselected.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object that was deselected.
  //
  // Remarks:
  //   If all objects are being deselected, then
  //   a single notification is sent to 
  //   OnDeselectAllObjects.
  WARNING: 
     Never modify the Rhino document in a OnDeselectObject() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnDeselectObject() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnDeselectObject(
    CRhinoDoc& doc,
    const CRhinoObject& object
    );

  /*
  // Description:
  //   Called when a collection of objects is deselected.
  //
  // Parameters:
  //   doc - [in] object's document
  //   objects - [in] object that were deselected
  WARNING: 
     Never modify the Rhino document in a OnDeselectObjects() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnDeselectObjects() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnDeselectObjects(
    CRhinoDoc& doc,
    const ON_SimpleArray<const CRhinoObject*>& objects
    );

  /*
  // Description:
  //   Called if a all objects are begin deselected.
  //
  // Parameters:
  //   doc - [in] object's document
  //   count - [in] number of object's that were deselected.
  WARNING: 
     Never modify the Rhino document in a OnDeselectAllObjects() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnDeselectAllObjects() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnDeselectAllObjects(
    CRhinoDoc& doc,
    int count
    );


  /*
  // Description:
  //   Called if an object's attributes change.
  //
  // Parameters:
  //   doc - [in] object's document
  //   object - [in] object whose attributes were changed.  The new
  //       attributes are object.Attributes().
  //   old_attributes - [in] attributes before the change.
  WARNING: 
     Never modify the Rhino document in a OnModifyObjectAttributes() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnModifyObjectAttributes() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnModifyObjectAttributes(
  CRhinoDoc& doc,
  CRhinoObject& object,
  const CRhinoObjectAttributes& old_attributes
  );

  /*
  Description:
    Called if the object's render mesh or analysis mesh is updated.
  Parameters:
    doc - [in] object's document
    object - [in] object whose mesh was updated.  The new
        mesh is attached to the object.
    mesh_type - [in] type of mesh (ON::render_mesh or ON::analysis_mesh)

  WARNING: 
     Never modify the Rhino document in a OnUpdateObjectMesh() override.
     If you need to change the Rhino document or post update messages
     to controls, then your OnUpdateObjectMesh() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void OnUpdateObjectMesh(
  CRhinoDoc& doc,
  CRhinoObject& object,
  ON::mesh_type mesh_type
  );

  /*
  // Description:
  virtual
  void OnPreselectObjects();

  virtual
  void OnPreselectObjects();
  */
  
  
  ///////////////////////////////////////////////////////////////////
  //
  // Layer table events
  //
  enum layer_event
  {
    layer_added,
    layer_deleted,
    layer_undeleted,
    layer_modified,
    layer_sorted,    // doc.m_layer_table.Sort() potentially changed sort order
    layer_current    // current layer change
  };

  /*
  // Description:
  //   LayerTableEvent() is called whenever the layer table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   layer_table - [in] layer table
  //   layer_index - [in] index of layer
  //     If the event is layer_sorted, then layer_index is 0.
  //   old_settings - [in] If the event is layer_modified,
  //       then the old layer definition is 
  //       passed here. Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a LayerTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your LayerTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void LayerTableEvent( 
         CRhinoEventWatcher::layer_event event,
         const CRhinoLayerTable& layer_table,
         int layer_index,
         const ON_Layer* old_settings
         );

  ///////////////////////////////////////////////////////////////////
  //
  // Linetype table events
  //
  enum linetype_event
  {
    linetype_added,
    linetype_deleted,
    linetype_undeleted,
    linetype_modified,
    linetype_sorted,    // doc.m_linetype_table.Sort() potentially changed sort order
    linetype_current    // current linetype change
  };

  /*
  // Description:
  //   LinetypeTableEvent() is called whenever the linetype table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   linetype_table - [in] linetype table
  //   linetype_index - [in] index of linetype
  //     If the event is linetype_sorted, then linetype_index is 0.
  //   old_settings - [in] If the event is linetype_modified,
  //       then the old linetype definition is 
  //       passed here. Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a LinetypeTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your LinetypeTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void LinetypeTableEvent( 
         CRhinoEventWatcher::linetype_event event,
         const CRhinoLinetypeTable& linetype_table,
         int linetype_index,
         const ON_Linetype* old_settings
         );

  ///////////////////////////////////////////////////////////////////
  //
  // InstanceDefinition table events
  //
  enum idef_event
  {
    idef_added,
    idef_deleted,
    idef_undeleted,
    idef_modified,
    idef_sorted     // doc.m_instance_definition_table.Sort() potentially changed sort order
  };

  /*
  // Description:
  //   InstanceDefinitionTableEvent() is called whenever the idef table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   idef_table - [in] instance definition table
  //   idef_index - [in] index of idef
  //     If the event is idef_sorted, then idef_index is 0.
  //   old_settings - [in] If the event is idef_modified,
  //       then the old idef definition is 
  //       passed here. Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a InstanceDefinitionTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your InstanceDefinitionTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void InstanceDefinitionTableEvent( 
         CRhinoEventWatcher::idef_event event,
         const CRhinoInstanceDefinitionTable& idef_table,
         int idef_index,
         const ON_InstanceDefinition* old_settings
         );

  ///////////////////////////////////////////////////////////////////
  //
  // Light table events
  //
  enum light_event
  {
    light_added,
    light_deleted,
    light_undeleted,
    light_modified,
    light_sorted,    // doc.m_light_table.Sort() potentially changed sort order
  };

  /*
  // Description:
  //   LightTableEvent() is called whenever the light table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   light_table - [in] light table
  //   light_index - [in] light of light
  //     If the event is light_sorted, then light_index is 0.
  //   old_settings - [in] If the event is light_modified,
  //       then the old light definition is 
  //       passed here. Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a LightTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your LightTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void LightTableEvent( 
         CRhinoEventWatcher::light_event event,
         const CRhinoLightTable& light_table,
         int light_index,
         const ON_Light* old_settings
         );


  ///////////////////////////////////////////////////////////////////
  //
  // Material table events
  //
  enum material_event
  {
    material_added,
    material_deleted,
    material_undeleted,
    material_modified,
    material_sorted,     // doc.m_material_table.Sort() potentially changed sort order
    material_current     // current material change
  };

  /*
  // Description:
  //   MaterialTableEvent() is called whenever the material table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   material_table - [in] material table
  //   material_index - [in] index of material
  //     If the event is material_sorted, then material_index is 0.
  //   old_settings - [in] If the event is material_modified,
  //       then the old material definition is 
  //       passed here. Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a MaterialTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your MaterialTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void MaterialTableEvent( 
         CRhinoEventWatcher::material_event event,
         const CRhinoMaterialTable& material_table,
         int material_index,
         const ON_Material* old_settings
         );


  ///////////////////////////////////////////////////////////////////
  //
  // TextureMapping table events
  //
  enum texture_mapping_event
  {
    texture_mapping_added,
    texture_mapping_deleted,
    texture_mapping_undeleted,
    texture_mapping_modified,
    texture_mapping_sorted,     // doc.m_texture_mapping_table.Sort() potentially changed sort order
    texture_mapping_current     // current texture_mapping change
  };

  /*
  // Description:
  //   TextureMappingTableEvent() is called whenever the texture_mapping table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   texture_mapping_table - [in] texture_mapping table
  //   texture_mapping_index - [in] index of texture_mapping
  //     If the event is texture_mapping_sorted, then texture_mapping_index is 0.
  //   old_settings - [in] If the event is texture_mapping_modified,
  //       then the old texture_mapping definition is 
  //       passed here. Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a TextureMappingTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your TextureMappingTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void TextureMappingTableEvent( 
         CRhinoEventWatcher::texture_mapping_event event,
         const CRhinoTextureMappingTable& texture_mapping_table,
         int texture_mapping_index,
         const ON_TextureMapping* old_settings
         );

  ///////////////////////////////////////////////////////////////////
  //
  // Group table events
  //
  enum group_event
  {
    group_added,
    group_deleted,
    group_undeleted,
    group_modified,  // name, color, etc., change
    group_sorted     // doc.m_group_table.Sort() potentially changed sort order
  };

  /*
  // Description:
  //   GroupTableEvent() is called whenever the group table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   group_table - [in] group table
  //   group_index - [in] index of group
  //     If the event is group_sorted, then group_index is 0.
  //   old_settings - [in] If the event is group_modified,
  //       then the old group definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a GroupTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your GroupTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void GroupTableEvent( 
         CRhinoEventWatcher::group_event event,
         const CRhinoGroupTable& group_table,
         int group_index,
         const ON_Group* old_settings
         );

  ///////////////////////////////////////////////////////////////////
  //
  // Alias table events
  //
  enum alias_event
  {
    alias_added,
    alias_deleted,
    alias_undeleted,
    alias_modified,  // name, color, etc., change
    alias_sorted,     // doc.m_alias_table.Sort() potentially changed sort order
    alias_current
  };
  
  // Description:
  //   AliasTableEvent() is called whenever the alias table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   alias_table - [in] alias table
  //   alias_index - [in] index of alias
  //     If the event is alias_sorted, then alias_index is 0.
  //   old_settings - [in] If the event is alias_modified,
  //       then the old alias definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
  virtual
  void AliasTableEvent( 
                      CRhinoEventWatcher::alias_event event,
                      const CRhinoAppAliasList& alias_table,
                      int alias_index,
                      const CRhinoCommandAlias* old_settings
                      );
    
  ///////////////////////////////////////////////////////////////////
  //
  // Font table events
  //
  // OBSOLETE
  enum text_style_event : unsigned int
  {
    text_style_added,
    text_style_deleted,
    text_style_undeleted,
    text_style_modified,
    text_style_sorted,
    text_style_current
  };

  ///////////////////////////////////////////////////////////////////
  //
  //
  // OBSOLETE
  enum font_event
  {
    font_added,
    font_deleted,
    font_undeleted,
    font_modified,
    font_sorted,
    font_current
  };



  ///////////////////////////////////////////////////////////////////
  //
  // DimStyle table events
  //
  enum dimstyle_event
  {
    dimstyle_added,
    dimstyle_deleted,
    dimstyle_undeleted,
    dimstyle_modified,  // name, color, etc., change
    dimstyle_sorted,    // doc.m_dimstyle_table.Sort() potentially changed sort order
    dimstyle_current    // current dim style change
  };

  /*
  // Description:
  //   DimStyleTableEvent() is called whenever the dimstyle table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   dimstyle_table - [in] dimstyle table
  //   dimstyle_index - [in] index of dimstyle
  //     If the event is dimstyle_sorted, then dimstyle_index is 0.
  //   old_settings - [in] If the event is dimstyle_modified,
  //       then the old dimstyle definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a DimStyleTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your DimStyleTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void DimStyleTableEvent( 
         CRhinoEventWatcher::dimstyle_event event,
         const CRhinoDimStyleTable& dimstyle_table,
         int dimstyle_index,
         const ON_DimStyle* old_settings
         );

  ///////////////////////////////////////////////////////////////////
  //
  // HatchPattern table events
  //
  enum hatchpattern_event
  {
    hatchpattern_added,
    hatchpattern_deleted,
    hatchpattern_undeleted,
    hatchpattern_modified,  // name, color, etc., change
    hatchpattern_sorted,    // doc.m_hatchpattern_table.Sort() potentially changed sort order
    hatchpattern_current    // current hatch pattern change
  };

  /*
  // Description:
  //   HatchPatternTableEvent() is called whenever the hatchpattern table is changed.
  //
  // Parameters:
  //   event - [in] type of event
  //   hatchpattern_table - [in] hatch pattern table
  //   hatchpattern_index - [in] index of hatchpattern
  //     If the event is hatchpattern_sorted, then hatchpattern_index is 0.
  //   old_settings - [in] If the event is hatchpattern_modified,
  //       then the old hatchpattern definition is passed here.
  //       Otherwise, this parameter is NULL.
  //
   WARNING: 
     Never modify the Rhino document in a HatchPatternTableEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your HatchPatternTableEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void HatchPatternTableEvent( 
         CRhinoEventWatcher::hatchpattern_event event,
         const CRhinoHatchPatternTable& hatchpattern_table,
         int hatchpattern_index,
         const ON_HatchPattern* old_settings
         );

#if defined(INCLUDE_RHINO_SECTION_STYLE_TABLE)
  enum section_style_event
  {
    section_style_added,
    section_style_deleted,
    section_style_undeleted,
    section_style_modified,
  };
#endif

  enum undo_event
  {
    no_undo_event  = 0,

    begin_recording,  // Begin recording changes
    end_recording,

    begin_undo,       // Begin undoing a changes
    end_undo,

    begin_redo,       // Begin redoing a changes
    end_redo,

    before_begin_recording,
    before_end_recording,

    purge_record = 86
  };

  /*
  Description:
    Used to monitor Rhino's built in undo/redo support.

  Parameters:
    undo_event - [in]
      Reports what is happening:
    undo_record_serialnumber - [in]
      Runtime serial number of the undo event
    cmd - [in] (can be NULL)
      If not NULL, this is the command whose activity is
      being recorded for possible undo.
  Remarks:
    "86" is restaurant slang for "nix"
    http://www.restaurantreport.com/qa/86d.html

   WARNING: 
     Never modify the Rhino document in a UndoEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your UndoEvent() override
     should record what happened in an efficient way and then make
     any changes that are required in a CRhinoIsIdle.Notify() override.
  */
  virtual
  void UndoEvent( 
#ifdef ON_RUNTIME_APPLE
         const CRhinoDoc& doc,
#endif
         CRhinoEventWatcher::undo_event event,
         unsigned int undo_record_serialnumber,
         const CRhinoCommand* cmd
         );


  // OBSOLETE - NEVER USE THIS
  enum event_id
  {
    event_id_none = 0,

    // This line insures that adding events will not change the size of the event_id.
    event_id_force_4byte_size = 0xFFFFFFFF
  };

  // OBSOLETE - NEVER USE THIS
  virtual
  void Event( 
    event_id id,
    unsigned int iParam1,
    int iParam2,
    const void* ptrParam1,
    const void* ptrParam2
    );

  // Description:
  //   Each instance of a CRhinoEventWatcher gets a unique
  //   runtime serial number to prevent multiple registrations.
  unsigned int SerialNumber() const;

  bool IsHeadlessDocAware() const;
  bool IsHeadlessAppAware() const;
private:
  static unsigned int m_serial_number_count;
  unsigned int m_serial_number;
  bool m_bEnabled;
  bool m_headless_doc_aware = false;
  bool m_headless_app_aware = false;
  bool m_reserved = false;
  ON__UINT_PTR m_sdk_reserved = 0;
private:
  CRhinoEventWatcher(const CRhinoEventWatcher&);
  CRhinoEventWatcher& operator=(const CRhinoEventWatcher&);
};



class RHINO_SDK_CLASS CRhinoEventWatcherEx
{
public:

  enum event_type
  {
    event_type_none = 0,

    // Called before objects are transformed.
    event_type_transform_object = 1,

    // Object selection state changed.
    event_type_change_object_select_state = 2,

    // Called after objects are transformed.
    event_type_after_transform_object = 3,

    // Called after a page view's active detail object has changed.
    event_type_set_active_detail_object = 4,

    // Called after view's display mode has been changed.
    event_type_view_display_mode_changed = 5,

    // Called after a .3dm file is read to bracket the adjustments
    // to views and clipping planes.
    event_type_before_post_read_view_update = 6,
    event_type_after_post_read_view_update = 7,

    // Called when Rhino is idle. Do not do anything time
    // consuming when responding to this event.
    event_type_rhino_is_idle = 8,

    // Called when the Rhino document that modeless user interface
    // refers to has changed.  If you get this event, invalidate
    // your window and wait for something like a paint
    // message to come along.  At that time, call 
    // RhinoApp().ModelessUserInterfaceDocRuntimeSerialNumber()
    // and update your display accordingly.
    // Do not do extensive regeneration code when
    // this event happens.
    event_type_modeless_ui_doc_changed = 9,

    // Called when the Rhino document that a new command will
    // target has changed.  If you get this event, do something
    // fast and simple and return immediately.  Later, when
    // Rhino is idle, call RhinoApp().TargetDocRuntimeSerialNumber()
    // and update your information accordingly.
    // Do not do extensive regeneration code when
    // this event happens.
    event_type_target_doc_changed = 10,

    // Called when plug-in settings files have been over written
    event_type_plug_in_settings_changed = 11,

    // Called when a RUI file event is raised
    event_type_rui_file = 12,

    // Called every iteration of the Rhino main message loop.
    event_type_main_loop = 13,

    // Called after a view has been modified.
    event_type_view_modified = 14,

    // Called after document user text is set.
    event_type_doc_user_text_changed = 15,

    // Called after a display mode's setting(s) have been saved/changed.
    event_type_display_mode_settings_changed = 16,

    // Called when an object manager event is raised.
    event_type_object_manager_changed = 17,

    // Called when the properties of page layout view changed.
    event_type_pageview_properties_changed = 18,

    // Call when there is a CRhinoGetPoint::GetPoint event
    event_type_getpoint_event = 19,

    // Called when there is a CRhinoView::EnableDrawing
    event_type_enable_drawing_changed = 20,

    // Bound on event type value
    event_type_max,

    // This line insures that adding events will not 
    // change the size of event_type.
    event_type_force_4byte_size = 0xFFFFFFFF
  };

  /*
    Call this function to register your watcher. Your watcher will not be
    called until it is registered.
  */
  bool Register();

  /*
  Description:
    Call this function to unregister your watcher.
    If you need to disable and enable a watcher frequently,
    then use the Enable() function.
  */
  bool Unregister();

  /*
  Returns:
    True if this watcher is registered.
  */
  bool IsRegistered() const;

  /*
  Description:
    The constructor enables the watcher so that it will
    be called as soon as it is registered.
    If you need to disable/enable the watcher frequently,
    then the Enable() function is more efficient
    than calling Register()/Unregister().
  */
  void Enable(bool bEnable);
  
  /*
  Returns:
    True if this watcher is enabled.
  */
  bool IsEnabled() const;

  // Type of event being watched
  const event_type m_event_type;

  // unique runtime serial number for this watcher
  const unsigned int m_watcher_sn;

  // ID of plug-in that is watching
  const ON_UUID m_plugin_id;

  bool IsHeadlessDocAware() const;
  bool IsHeadlessAppAware() const;
public:
  // Insure the heap used by event watcher classes is
  // reliable and independent of plug-in heap choices.

  // new/delete
  void* operator new(size_t);
  void  operator delete(void*);

  // array new/delete
  void* operator new[] (size_t);
  void  operator delete[] (void*);

  // in place new/delete
  void* operator new(size_t,void*);
  void  operator delete(void*,void*);

protected:
  CRhinoEventWatcherEx(CRhinoEventWatcherEx::event_type et, ON_UUID plugin_id);
  /*
    Description:
      By default event watchers are not called for headless docs and headless app
      scenarios. Use this constructor if you want to make your event watcher be
      called for these headless cases. Note that there is a good chance that your
      event watcher can be called on a different thread than expected when dealing
      with "headless"
  */
  CRhinoEventWatcherEx(CRhinoEventWatcherEx::event_type et, ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);
  virtual ~CRhinoEventWatcherEx();

private:
  bool m_bIsRegistered;
  bool m_bIsEnabled;
  bool m_headless_doc_aware = false;
  bool m_headless_app_aware = false;
  ON__UINT_PTR m_dsk_reserved = 0;

private:
  // prohibit use
  CRhinoEventWatcherEx();
  CRhinoEventWatcherEx(const CRhinoEventWatcherEx&);
  CRhinoEventWatcherEx& operator=(const CRhinoEventWatcherEx&);
};

class RHINO_SDK_CLASS CRhinoOnTransformObject : public CRhinoEventWatcherEx
{
public:
  CRhinoOnTransformObject(ON_UUID plugin_id);
  CRhinoOnTransformObject(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // number of objects that will be transformed
    int m_object_count;

    // list of objects that will be transformed.
    // WARNING: 
    //   These objects may be deleted at any time
    //   after Notify is called.  Do not save these
    //   pointers and derference them later.  Use
    //   the runtime object serial number to safely
    //   save a reference to these objects that can
    //   be used at a later time.
    const CRhinoObject * const * m_object_list;

    // number of grips that will be transformed.
    int m_grip_count;

    // list of grips that will be transformed.
    // WARNING: 
    //   These grips may be deleted at any time
    //   after Notify is called.  Do not save these
    //   pointers and derference them later.  Use
    //   the runtime object serial number to safely
    //   save a reference to these objects that can
    //   be used at a later time.
    const CRhinoGripObject * const * m_grip_list;

    // number of grip owners that will be changed
    // when the grips move.
    int m_grip_owner_count;

    // list of grip owners that will be changed
    // when the grips move.
    // WARNING: 
    //   These grip owners may be deleted at any time
    //   after Notify is called.  Do not save these
    //   pointers and derference them later.  Use
    //   the runtime object serial number to safely
    //   save a reference to these objects that can
    //   be used at a later time.
    const CRhinoObject * const * m_grip_owner_list;

    // Transformation that will be applied
    ON_Xform m_xform;

    // True if objects will be copied.
    bool m_bCopy;

    // This serial number identifies the transform objects event
    // and will be passed to the AfterTransformObjects event
    // that marks the end of the transformation.
    unsigned int m_transform_event_id;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify before transforming the objects.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnTransformObject::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnTransformObject::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual 
  void Notify( const class CRhinoOnTransformObject::CParameters& params ) = 0;

private:
  // prohibit use
  CRhinoOnTransformObject();
  CRhinoOnTransformObject(const CRhinoOnTransformObject&);
  CRhinoOnTransformObject& operator=(const CRhinoOnTransformObject&);
};

class RHINO_SDK_CLASS CRhinoAfterTransformObject : public CRhinoEventWatcherEx
{
public:
  CRhinoAfterTransformObject(ON_UUID plugin_id);
  CRhinoAfterTransformObject(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // This serial number identifies the transform objects event and
    // is the same value passed to the CRhinoOnTransformObject::Notify().
    unsigned int m_transform_event_id;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify after transforming the objects.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoAfterTransformObject::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoAfterTransformObject::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual 
  void Notify( const class CRhinoAfterTransformObject::CParameters& params ) = 0;

private:
  // prohibit use
  CRhinoAfterTransformObject();
  CRhinoAfterTransformObject(const CRhinoAfterTransformObject&);
  CRhinoAfterTransformObject& operator=(const CRhinoAfterTransformObject&);
};

class RHINO_SDK_CLASS CRhinoOnChangeObjectSelectState : public CRhinoEventWatcherEx
{
public:
  CRhinoOnChangeObjectSelectState(ON_UUID plugin_id);
  CRhinoOnChangeObjectSelectState(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // number of objects that were selected.
    int m_object_selected_count;

    // list of objects that were selected
    // WARNING: 
    //   These objects may be deleted at any time
    //   after Notify is called.  Do not save these
    //   pointers and derference them later.  Use
    //   the runtime object serial number to safely
    //   save a reference to these objects.
    const CRhinoObject * const * m_object_selected_list;

    // number of objects that were deselected
    int m_object_deselected_count;

    // list of objects that were deselected
    // WARNING: 
    //   These objects may be deleted at any time
    //   after Notify is called.  Do not save these
    //   pointers and derference them later.  Use
    //   the runtime object serial number to safely
    //   save a reference to these objects.
    const CRhinoObject * const * m_object_deselected_list;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after changing the selection state.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnChangeObjectSelectState::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnChangeObjectSelectState::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual 
  void Notify( const class CRhinoOnChangeObjectSelectState::CParameters& params ) = 0;

private:
  // prohibit use
  CRhinoOnChangeObjectSelectState();
  CRhinoOnChangeObjectSelectState(const CRhinoOnChangeObjectSelectState&);
  CRhinoOnChangeObjectSelectState& operator=(const CRhinoOnChangeObjectSelectState&);
};

class RHINO_SDK_CLASS CRhinoOnSetActiveDetailObject : public CRhinoEventWatcherEx
{
public:
  CRhinoOnSetActiveDetailObject(ON_UUID plugin_id);
  CRhinoOnSetActiveDetailObject(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    unsigned int m_rhino_doc_sn;


    // The page view on which a different detail object was set active.
    unsigned int m_page_view_sn;

    class CRhinoPageView* m_page_view; //DALELEARV6// will be removed later

    // The id of the detail object was set active.
    // Note, if this id is equal to ON_nil_uuid, then the active detail object
    // is the page view itself.
    ON_UUID m_active_detail_object_id;

    // The id of the previously active detail object.
    // Note, if this id is equal to ON_nil_uuid, then the active detail object
    // was the page view itself.
    ON_UUID m_old_active_detail_object_id;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after changing the active detail.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnSetActiveDetailObject::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnSetActiveDetailObject::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual 
  void Notify(const class CRhinoOnSetActiveDetailObject::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoOnSetActiveDetailObject();
  CRhinoOnSetActiveDetailObject(const CRhinoOnSetActiveDetailObject&);
  CRhinoOnSetActiveDetailObject& operator=(const CRhinoOnSetActiveDetailObject&);
};

class RHINO_SDK_CLASS CRhinoDisplayModeChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoDisplayModeChanged(ON_UUID plugin_id);
  CRhinoDisplayModeChanged(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // The viewport representing specified mode changes...
    CRhinoViewport* m_vp;

    // The id of the display mode that is either changing or has been changed to (see below).
    ON_UUID m_changed_display_mode_id;

    // The id of the previously used display mode in the view
    ON_UUID m_old_display_mode_id;

    // See CRhinoDisplayAttrsMgr for more details on obtaining display mode
    // attributes given a display mode ID...

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after changing the view's display mode.
  // WARNING: Never modify the Rhino document or application state in a Notify override.
  // Never attempt to change the view's (or any other view) display mode inside
  // your Notify routine, or you will eventually crash Rhino.
  //
  // Rhino will also call Notify() whenever a display mode's attributes change, so do
  // NOT attempt to change any display attributes (ala CRhinoDisplayAttrsMgr) inside 
  // your Notify routine, or you may experience undefined behavior...
  //
  // There are three different states that can be observed using the notifier...
  //
  //    1) A view's display mode has been set.
  //    2) The attributes of the display mode used by the active view have changed.
  //    3) The attributes of a specific display mode have changed.
  //
  // ...all three can be determined using the CParameters:
  //
  //    1)  If ( (params.m_vp != NULL) && (params.m_changed_display_mode_id != params.m_old_display_mode_id) )
  //          then m_vp's display mode has just been set/changed.
  //
  //    2)  If ( (params.m_vp != NULL) && (params.m_changed_display_mode_id == params.m_old_display_mode_id) )
  //          then the attributes of the display mode used by the ACTIVE viewport have changed.
  //
  //    3)  If ( (params.m_vp == NULL) && (params.m_old_display_mode_id == ON_nil_uuid) )
  //          then the attributes of the display mode specified by params.m_changed_display_mode_id have changed
  //          (i.e. no viewport involved whatsoever).
  //
  // ...any other combinations of params are not yet determined. The differences are subtle, but each condition
  // can require completely different processing depending on the use of the notifier. 
  //
  // Note:  If you need to check against a specific CRhinoView (i.e. the active view), then you will need to
  //        get it from params.m_vp, like so:
  //
  //            CRhinoView*  view = params.m_vp ? params.m_vp->ParentView() : NULL;
  //
  //        ...there will be cases where viewports are not attached to views, so you'll need to handle those
  //        as well.
  //
  // Note2: You cannot just assume that if (view == RhinoApp().ActiveView()) then the active view's display
  //        mode changed. Likewise, you cannot assume that if (view != RhinoApp().ActiveView()) then only 
  //        the attributes have changed...you must look at each CParameters value in order to determine specific 
  //        conditions.
  //
  virtual 
  void Notify(const class CRhinoDisplayModeChanged::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoDisplayModeChanged();
  CRhinoDisplayModeChanged(const CRhinoDisplayModeChanged&);
  CRhinoDisplayModeChanged& operator=(const CRhinoDisplayModeChanged&);
};

class RHINO_SDK_CLASS CRhinoDisplayModeSettingsChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoDisplayModeSettingsChanged(ON_UUID plugin_id);
  CRhinoDisplayModeSettingsChanged(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // The id of the display mode whose settings have just changed.
    ON_UUID m_changed_display_mode_id;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after changing/saving a display mode's settings.
  virtual 
  void Notify(const class CRhinoDisplayModeSettingsChanged::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoDisplayModeSettingsChanged();
  CRhinoDisplayModeSettingsChanged(const CRhinoDisplayModeSettingsChanged&);
  CRhinoDisplayModeSettingsChanged& operator=(const CRhinoDisplayModeSettingsChanged&);
};

class RHINO_SDK_CLASS CRhinoOnBeforePostReadViewUpdate : public CRhinoEventWatcherEx
{
public:
  CRhinoOnBeforePostReadViewUpdate(ON_UUID plugin_id);
  CRhinoOnBeforePostReadViewUpdate(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // File reading information
    const CRhinoFileReadOptions* m_fro;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after OnEndOpenDocument()
  // before using the information that was read to adjust views
  // and clipping planes.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnBeforePostReadViewUpdate::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnBeforePostReadViewUpdate::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual 
  void Notify(const class CRhinoOnBeforePostReadViewUpdate::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoOnBeforePostReadViewUpdate();
  CRhinoOnBeforePostReadViewUpdate(const CRhinoOnBeforePostReadViewUpdate&);
  CRhinoOnBeforePostReadViewUpdate& operator=(const CRhinoOnBeforePostReadViewUpdate&);
};

class RHINO_SDK_CLASS CRhinoOnAfterPostReadViewUpdate : public CRhinoEventWatcherEx
{
public:
  CRhinoOnAfterPostReadViewUpdate(ON_UUID plugin_id);
  CRhinoOnAfterPostReadViewUpdate(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // File reading information
    const CRhinoFileReadOptions* m_fro;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after OnEndOpenDocument()
  // before using the information that was read to adjust views
  // and clipping planes.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnAfterPostReadViewUpdate::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnAfterPostReadViewUpdate::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual 
  void Notify(unsigned int document_runtime_serial_number, const class CRhinoOnAfterPostReadViewUpdate::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoOnAfterPostReadViewUpdate();
  CRhinoOnAfterPostReadViewUpdate(const CRhinoOnAfterPostReadViewUpdate&);
  CRhinoOnAfterPostReadViewUpdate& operator=(const CRhinoOnAfterPostReadViewUpdate&);
};

class RHINO_SDK_CLASS CRhinoIsIdle : public CRhinoEventWatcherEx
{
public:
  CRhinoIsIdle(ON_UUID plugin_id);
  CRhinoIsIdle(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {    
  public:
    // value of the CWinApp::OnIdle "lCount" parameter
    int m_count;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() when it is idling.  The Notify()
  // function of a CRhinoIsIdle event watcher is the only
  // place it is safe to modify a Rhino document.  All other
  // event watchers should simply watch and quickly record
  // what event happened for eventual processing in a
  // CRhinoIsIdle event watcher.
  virtual 
  void Notify(const class CRhinoIsIdle::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoIsIdle();
  CRhinoIsIdle(const CRhinoIsIdle&);
  CRhinoIsIdle& operator=(const CRhinoIsIdle&);
};

class RHINO_SDK_CLASS CRhinoModelessUserInterfaceDocChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoModelessUserInterfaceDocChanged(ON_UUID plugin_id);

  class CParameters
  { 
  public:
    unsigned int m_previous_modeless_ui_rhino_doc_sn;

    // Call CRhinoDoc::ModelessUserInterfaceRuntimeSerialNumber()
    // to get the current value.

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() when the document that  modeless
  // user interface should reference is changed.
  // When you receive this event do not immediately update your
  // user interface display. Instead, something that is fast and
  // efficient like invalidating a control's window and returning.
  // When you get a "paint" message, then call 
  // CRhinoDoc::ModelessUserInterfaceDocRuntimeSerialNumber()
  // to get the most up to date value and do whatever is appropriate.
  virtual 
  void Notify(const class CRhinoModelessUserInterfaceDocChanged::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoModelessUserInterfaceDocChanged();
  CRhinoModelessUserInterfaceDocChanged(const CRhinoModelessUserInterfaceDocChanged&);
  CRhinoModelessUserInterfaceDocChanged& operator=(const CRhinoModelessUserInterfaceDocChanged&);
};

class RHINO_SDK_CLASS CRhinoTargetDocChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoTargetDocChanged(ON_UUID plugin_id);
  CRhinoTargetDocChanged(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  { 
  public:
    unsigned int m_previous_target_rhino_doc_sn;

    // Call CRhinoDoc::TargetRuntimeSerialNumber()
    // to get the current value.

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() when the document that a new command
  // targets is changed.
  // When you receive this event do not immediately update your
  // user interface display. Instead, something that is fast and
  // efficient like invalidating a control's window and returning.
  // When you get a "paint" message, then call 
  // CRhinoDoc::TargetRuntimeSerialNumber()
  // to get the most up to date value and do whatever is appropriate.
  virtual 
  void Notify(const class CRhinoTargetDocChanged::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoTargetDocChanged();
  CRhinoTargetDocChanged(const CRhinoTargetDocChanged&);
  CRhinoTargetDocChanged& operator=(const CRhinoTargetDocChanged&);
};

class RHINO_SDK_CLASS CRhinoPlugInSettingsChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoPlugInSettingsChanged(ON_UUID plugInId);
  CRhinoPlugInSettingsChanged(ON_UUID plugInId, bool headlessDocAware, bool headlessAppAware);

  class RHINO_SDK_CLASS CParameters
  { 
  public:
    // Description:
    //    The plug-in associated with the CRhinoSettings being saved.
    ON_UUID PlugInId() const;
    // Description:
    //    The CRhinoSettings read from the current settings file, compare
    //    these values to the CRinoPlugIn::Settings() values to see if anything you
    //    care about changed.
    CRhinoSettings& Settings() const;
    bool ThisRhinoIsSaving() const;
  private:
    ON_UUID m_plug_in_id;
    unsigned int m_pointer_id;
    bool m_this_rhino_is_saving;
    class CRhPlugInSettings* m_settings;

  private:
    friend void RhCallWatcher_PlugInSettingsChanged(ON_UUID plugInId, unsigned int pointerId, bool thisRhinoIsSaving);
    CParameters(ON_UUID plugInId, unsigned int pointerId, bool thisRhinoIsSaving);
    // prohibit use
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() CRhinoPlugIn::SaveSettings() is called or when a
  // plug-in's settings are saved while Rhino is closing.  This event is useful
  // when you want to keep plug-in settings synchronized with multiple Rhino
  // sessions.  Rhino will only notify you if the plug-in Id passed to the
  // constructor is the plug-in writing the persistent settings file.
  virtual 
  void Notify(const class CRhinoPlugInSettingsChanged::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoPlugInSettingsChanged();
  CRhinoPlugInSettingsChanged(const CRhinoPlugInSettingsChanged&);
  CRhinoPlugInSettingsChanged& operator=(const CRhinoPlugInSettingsChanged&);
};

class RHINO_SDK_CLASS CRhinoUiFileEvent : public CRhinoEventWatcherEx
{
public:
  // Description:
  //  Constructor
  // Parameters:
  //  plugInId[in]
  //  eventId[in] See RhinoSdkUiFile.h for event type Id's
  CRhinoUiFileEvent(ON_UUID plugInId, ON_UUID eventId);
  virtual ~CRhinoUiFileEvent();
  // Description:
  //  Supported event Id
  ON_UUID EventId() const;

  class RHINO_SDK_CLASS CParameters
  {
  public:
    CParameters(class CRhUiFileEventPrivateParameters* privatePrarmeters);
    ~CParameters();

    ON_UUID FileId() const;
    ON_UUID ItemId() const;
    ON_UUID EventId() const;

  private:
    class CRhUiFileEventPrivateParameters* m_private = nullptr;

  protected:
    CParameters(const CParameters&) = delete;
    CParameters& operator=(const CParameters&) = delete;
  };

  virtual
    // Description:
    //  Rhino will call Notify when a sported RUI file event occurs, see
    //  RhinoSdkUiFile for supported event id's
    void Notify(const class CRhinoUiFileEvent::CParameters& params) = 0;

private:
  CRhinoUiFileEvent(const CRhinoUiFileEvent&) = delete;
  CRhinoUiFileEvent& operator=(const CRhinoUiFileEvent&) = delete;
  class CRUiFileEventPrivate* m_private = nullptr;
};

class RHINO_SDK_CLASS CRhinoOnMainLoopEvent : public CRhinoEventWatcherEx
{
public:
  CRhinoOnMainLoopEvent(ON_UUID plugin_id);

  class CParameters
  {
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Gets called every loop iteration inside Rhino's main message loop.
  virtual
    void Notify(const class CRhinoOnMainLoopEvent::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoOnMainLoopEvent();
  CRhinoOnMainLoopEvent(const CRhinoOnMainLoopEvent&);
  CRhinoOnMainLoopEvent& operator=(const CRhinoOnMainLoopEvent&);
};


// This function makes it so that Rhino's main loop is executed continuously.
// This is useful when Rhino needs to be doing something as often as possible, 
// such as rendering a view at interactive frame rates.
RHINO_SDK_FUNCTION
bool RhinoEnableContinuousMainLoop();

// This function makes it so that Rhino's main loop is not executed continuously.
// This is default behavior.
RHINO_SDK_FUNCTION
void RhinoDisableContinuousMainLoop();

class RHINO_SDK_CLASS CRhinoOnViewModifiedEvent : public CRhinoEventWatcherEx
{
public:
  CRhinoOnViewModifiedEvent(ON_UUID plugin_id);
  CRhinoOnViewModifiedEvent(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {
  public:
    unsigned int m_view_sn;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after changing the active detail.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnViewModifiedEvent::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnViewModifiedEvent::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual
    void Notify(const class CRhinoOnViewModifiedEvent::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoOnViewModifiedEvent();
  CRhinoOnViewModifiedEvent(const CRhinoOnViewModifiedEvent&);
  CRhinoOnViewModifiedEvent& operator=(const CRhinoOnViewModifiedEvent&);
};


class RHINO_SDK_CLASS CRhinoOnDocUserStringChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoOnDocUserStringChanged(ON_UUID plugInId);
  CRhinoOnDocUserStringChanged(ON_UUID plugInId, bool headlessDocAware, bool headlessAppAware);

  class RHINO_SDK_CLASS CParameters
  {
  public:
    CParameters(class CRhinoOnDocUserStringChangedPrivate* privatePrarmeters);
    ~CParameters();

    const wchar_t* Key() const;
    CRhinoDoc* Document() const;

  private:
    class CRhinoOnDocUserStringChangedPrivate* m_private = nullptr;

  protected:
    CParameters(const CParameters&) = delete;
    CParameters& operator=(const CParameters&) = delete;
  };

  virtual
    // Description:
    //  Rhino will call Notify when a sported RUI file event occurs, see
    //  RhinoSdkUiFile for supported event id's
    void Notify(const class CRhinoOnDocUserStringChanged::CParameters& params) = 0;

private:
  CRhinoOnDocUserStringChanged(const CRhinoOnDocUserStringChanged&) = delete;
  CRhinoOnDocUserStringChanged& operator=(const CRhinoOnDocUserStringChanged&) = delete;
};

class RHINO_SDK_CLASS CRhinoOnPageViewPropertiesChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoOnPageViewPropertiesChanged(ON_UUID plugin_id);
  CRhinoOnPageViewPropertiesChanged(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {
  public:
    // The document runtime serial number.
    unsigned int m_rhino_doc_sn;

    // The page view serial number.
    unsigned int m_page_view_sn;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after changing page view properties.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnPageViewPropertiesChanged::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnPageViewPropertiesChanged::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual
  void Notify(const class CRhinoOnPageViewPropertiesChanged::CParameters& params) = 0;

private:
  // prohibit use
  CRhinoOnPageViewPropertiesChanged(const CRhinoOnPageViewPropertiesChanged&) = delete;
  CRhinoOnPageViewPropertiesChanged& operator=(const CRhinoOnPageViewPropertiesChanged&) = delete;
};

class RHINO_SDK_CLASS CRhinoOnViewEnableDrawingChanged : public CRhinoEventWatcherEx
{
public:
  CRhinoOnViewEnableDrawingChanged(ON_UUID plugin_id);
  CRhinoOnViewEnableDrawingChanged(ON_UUID plugin_id, bool headlessDocAware, bool headlessAppAware);

  class CParameters
  {
  public:
    // The document runtime serial number.
    unsigned int m_rhino_doc_sn;

    // The view drawing enabled state.
    bool m_drawing_enabled;

  private:
    // prohibit use
    CParameters();
    CParameters(const CParameters&);
    CParameters& operator=(const CParameters&);
  };

  // Rhino will call Notify() immediately after view drawing enabled changes.
  // WARNING: 
  //   Never modify the Rhino document in a 
  //   CRhinoOnViewEnableDrawingChanged::Notify override.
  //   If you need to change the Rhino document or post update messages
  //   to controls, then your CRhinoOnViewEnableDrawingChanged::Notify
  //   should record what happened in an efficient way and then make
  //   any changes that are required in a CRhinoIsIdle.Notify() override.
  virtual
  void Notify(const class CRhinoOnViewEnableDrawingChanged::CParameters& params) = 0;
};
