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

class RHINO_SDK_CLASS CRhinoHistoryRecord 
{
public:
  static 
  int CompareIndexAndId(const class CRhinoHistoryRecord&, const class CRhinoHistoryRecord& );

  /*
  Construction history information.
  Use the ON_HistoryRecord Get...Value() functions
  to retrieve the values from m_hr.
  */
  const ON_HistoryRecord& m_hr;

  /*
  Description:
    In ReplayHistory overrides, use GetRhinoObjRef
    to convert the information in m_hr into
    a CRhinoObjRef that has up to date CRhinoObject 
    pointers.
  Parameters:
    value_id - [in]
      History record value id
    or - [in]
      ON_ObjRef from m_hr.
    objref - [out]
    objref_list - [out]
  Returns
    True if successful
  */
  bool GetRhinoObjRef(     
          int value_id, 
          CRhinoObjRef& objref
          ) const;
  bool GetRhinoObjRef( 
          const ON_ObjRef& onref, 
          CRhinoObjRef& objref 
          ) const;

  /*
  Description:
    In ReplayHistory overrides, use GetRhinoObjRefList
    to convert the information in m_hr into
    an array CRhinoObjRefs that has up to date CRhinoObject 
    pointers.
  Parameters:
    value_id - [in]
      History record value id
    objref_list - [out]
      Object references are appended to this list.
  Returns
    Number of elements appended to objref_list[].
  */
  int GetRhinoObjRefList( 
          int value_id, 
          ON_ClassArray<CRhinoObjRef>& objref_list 
          ) const;

  /*
  Description:
    In ReplayHistory overrides, use GetRhinoPointOnObject
    to use the information in m_hr to find the CRhinoObject
    and evaluate it to get the point.
  Parameters:
    value_id - [in]
      History record value id
    or - [in]
      ON_ObjRef from m_hr.
    point - [out]
  Returns
    True if successful
  */
  bool GetRhinoPointOnObject( 
          int value_id, 
          ON_3dPoint& point
          ) const;
  bool GetRhinoPointOnObject( 
          const ON_ObjRef& onref, 
          ON_3dPoint& point 
          ) const;
  bool GetRhinoPointOnObject( 
          const CRhinoObjRef& objref, 
          ON_3dPoint& point 
          ) const;


  /*
  Description:
    Create a polyedges from information saved by SetPolyEdgeValue().
  Parameters:
    value_id - [in]
    polyedge - [out]
      New polyedge.
      The caller is responsible for deleting the returned polyedge.
  Returns:
    True if a polyedge is returned.
  */
  bool GetPolyEdgeValue( int value_id, class CRhinoPolyEdge*& polyedge ) const;

  /*
  Description:
    Creates polyedges from information saved by SetPolyEdgeValues().
  Parameters:
    value_id - [in]
    polyedges - [out]
      New polyedges are appended to this array.
      The caller is responsible for deleting the returned polyedges.
  Returns:
    Number of polyedges added to polyedges[]
  */
  int GetPolyEdgeValues( int value_id, ON_SimpleArray<class CRhinoPolyEdge*>& polyedges ) const;

  /*
  Description:
    Creates edge chain from information saved by SetSubDEdgeChainValue().
  Parameters:
    value_id - [in]
    edge_chain - [out]
      The returned edge_chain is updated to reference the current subd in the document.
  Returns:
    True if a nonempty edge chain is returned.
  */
  bool GetSubDEdgeChainValue( int value_id, ON_SubDEdgeChain& edge_chain) const;

  /*
  Description:
    Creates edge chains from information saved by SetSubDEdgeChainValues().
  Parameters:
    value_id - [in]
    edge_chains - [out]
      New edge chains are appended to this array.
  Returns:
    Number of edge chains added to edge_chains[]
  */
  int GetSubDEdgeChainValues( int value_id, ON_ClassArray<ON_SubDEdgeChain>& edge_chains) const;


  /*
  Returns:
    The command associated with this history record.
  */
  CRhinoCommand* Command() const;

  /*
  Returns:
    CRhinoDoc this record belongs to.
  */
  CRhinoDoc& Document() const;

  /*
  Description:
    ReplayHistory overrides check the version number
    to insure the information saved in the history
    record is compatible with the current implementation
    of ReplayHistory.
  Returns:
    History record version number.
  */
  int HistoryVersion() const;

  /*
  Description:
    Each history record has a unique id that Rhino
    assigns when it adds the history record to the
    history record table.
  Returns:
    History record id.
  */
  ON_UUID HistoryRecordId() const;

  /*
  Description:
    Determine if object is an antecedent (input) in this
    history record.
  Parameters:
    object_uuid - [in] 
  Returns:
    Returns true if object_uuid is the id of an input
    object.
  */
  bool IsAntecedent( ON_UUID object_uuid ) const;

  /*
  Description:
    Get a list of the antecedent (input) object ids in this
    history record.
  Parameters:
    uuid_list - [out] 
      Antecedent ids are appended to the uuid_list[] array
  Returns:
    Returns the number of uuids appended to uuid_list[] array.
  */
  int GetAntecedents( ON_SimpleArray<ON_UUID>& uuid_list ) const;
  int GetAntecedents( ON_UuidList& uuid_list ) const;

  /*
  Returns:
    True if this there are no antecedents or at least one
    is deleted or purged.
  */
  bool MissingAntecedents() const;


  /*
  Description:
    Get a list of the descendant (output) object ids in this
    history record.
  Parameters:
    descendant_ids - [out]
      Descendant ids are appended to the descendant_ids[] array
  Returns:
    Returns the number of uuids appended to descendant_ids[] array.
  */
  int GetDescendants(ON_SimpleArray<ON_UUID>& descendant_ids) const;
  int GetDescendants(ON_UuidList& descendant_ids) const;


  /*
  Description:
    Get a technical description of the values stored in the
    history record.  This list is not localized and never
    will be.  It is intended to be used by developers and
    to technical support staff.
  Parameters:
    text_log - [out]
  */
  void Report( ON_TextLog& text_log ) const;

  /*
  Description:
    Index of this record in the CRhinoDoc.m_history_record_table[]
    array.  This index will change during a session.  This
    function is here for creating reports used to study history
    bugs.
  Returns:
    Index of this record in the CRhinoDoc.m_history_record_table[]
    array.
  */
  int RecordIndex() const;


  // Runtime index used when marking records for import/export
  int m_remap_index;

private:
  friend class CRhinoHistoryRecordTable;

  CRhinoHistoryRecord( class CRhinoHistoryRecordTable& hr_table, ON_HistoryRecord*& );
  ~CRhinoHistoryRecord();

  // Number of CRhinoObject::m_history_record references
  // that point at this history record.
  // CRhinoHistoryRecordTable::CreateObjectHistory()
  // increments this count and
  // CRhinoHistoryRecordTable::DestoryObjectHistory()
  // decrements this count.  
  // Do NOT add any other
  int m__reference_count;

  // Current location in the CRhinoDoc::m_history_record_table[]
  // array.  This index can change.
  int m__history_record_index;

private:
  bool GetPointOnObjHelper( const ON_ObjRef&, ON_3dPoint& ) const;

  class CRhinoHistoryRecordTable& m_hr_table; // history record table this record belongs to
};


class RHINO_SDK_CLASS CRhinoHistoryRecordTable : private ON_SimpleArray<CRhinoHistoryRecord*>
{
public:
  /*
  Returns:
    CRhinoDoc that owns this history record table.
  */
  CRhinoDoc& Document() const;

  bool HistoryRecordingEnabled() const;

  /*
  Description:
    Command authors fill in a CRhinoHistory, create CRhinoObjects,
    and call SaveObjectHistory to put the information in
    the document.    
  */
  bool CreateObjectHistory( CRhinoObject* object, CRhinoHistory& history );

  void DestroyObjectHistory( CRhinoObject* object );

  /*
  Description:
    Object history tool that checks to see if an id
    is in the list of descendants.
  Parameters:
    descendant_uuid - [in] id to look for in the descendant
    list.
  Returns:
    True if descendant_uuid was found in the list of
    descendants.
  */
  bool IsHistoryDescendant( 
    const CRhinoObject* antecedent_object,
    ON_UUID descendant_uuid 
    ) const;

  /*
  Description:
    Object history tool that returns the list of
    objects that used this object during their
    construction.
  Parameters:
    antecedent_object - [in]
    uuid_list - [out]
  Returns:
    Number of ids appended to uuid_list[]
  */
  int GetHistoryDescendants(
    const CRhinoObject* antecedent_object,
    ON_SimpleArray<ON_UUID>& uuid_list
    ) const;


  /*
  Description:
    Purge a single history record. This cannot be undone.
  Parameters:
    history_record_id - [in]
      id of the record to purge.
  Returns:
    0: no changes
    >0: 1 + number of objects that had records purged.
  */
  int PurgeHistoryRecord(
    ON_UUID history_record_id
    );

  /*
  Description:
    Purge all history information.
  Returns:
    number of objects that had records purged.
  */
  int PurgeHistory();

  // Temporary - probably never needed.
  int HistoryRecordCount() const;

  const CRhinoHistoryRecord* operator[](int) const;

  void SetRemapIndex(
          int record_index,
          int remap_index
          );

  void ListTable( 
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

  // 26 May 2015, Mikko, RH-30481:
  /*
  Description:
    Get a copy of a history record with objects that no longer have history record attached to them purged from m_descendants list.
  Returns:
    History record copy. The caller is responsible for deleting the copy when done.
  */
  ON_HistoryRecord* GetPrunedHistoryRecord(int record_index);

private:
  friend class CRhinoDoc;

  // Constructs a layer table with one default layer.  This is private
  // because only CRhinoDoc is permitted to create or destroy layer
  // tables.
  CRhinoHistoryRecordTable(CRhinoDoc&);
  ~CRhinoHistoryRecordTable();
  CRhinoDoc& m_doc;
  int m__null_count;
  void Destroy();
  bool UpdateGenealogyHelper(ON_UUID,const CRhinoHistoryRecord*);
  bool RemoveGenealogyHelper(ON_UUID);
  void ClearUndoRecordsHelper();
  class CRhObjectGenealogy* m_og;
};
