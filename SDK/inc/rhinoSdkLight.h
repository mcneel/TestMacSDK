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

class RHINO_SDK_CLASS CRhinoLight : public CRhinoObject  
{
  ON_OBJECT_DECLARE(CRhinoLight);

public:
  /*
  Returns:
    CRhinoLight returns ON::light_object.
  */
  ON::object_type ObjectType() const override;

  // virtual ON_Object override
  void MemoryRelocate() override;

public:
  // All CRhinoLights are in CRhinoDoc::m_light_table[].
  // Use CRhinoLightTable::AddLight() to add new lights to
  // the document and CRhinoLightTable::ModifyLight() to
  // change light settings.  Use CRhinoObject::ModifyAttributes
  // to change CRhinoLight attributes like layer, etc.

  const ON_Light& Light() const;
  int LightIndex() const;
  ON_BoundingBox BoundingBox() const override;
  ON_BoundingBox BoundingBox( const CRhinoViewport* pViewport ) const override;

  // virtual CRhinoObject::GetTightBoundingBox override
  bool GetTightBoundingBox(
    ON_BoundingBox& tight_bbox,
    bool bGrowBox = false,
    const ON_Xform* xform = nullptr
  ) const override;
  
  // virtual CRhinoObject::EnableGrips override
  void EnableGrips( bool bGripsOn ) override;

  // virtual CRhinoObject::Draw override
  void Draw(CRhinoDisplayPipeline&) const override;

  // virtual CRhinoObject::Pick override
  int Pick( const CRhinoPickContext&, CRhinoObjRefArray& ) const override;

  // virtual CRhinoObject::SnapTo override
  bool SnapTo( const CRhinoSnapContext& snap_context, CRhinoSnapEvent& snap_event) const override;

  // virtual CRhinoObject::ShortDescription override
  const wchar_t* ShortDescription( bool bPlural) const override;

  BOOL32 IsDeleted() const;
  BOOL32 IsReference() const;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

  // runtime index used to determine how lights are sorted in light dialog
  int m_sort_index;  
  
  // runtime index used when remapping lights for import/export
  int m_remap_index; 

  /*
  Description:
    Get the location of a light that is being dynamically 
    edited by grip dragging or a transformation command.
  Properties:
    light - [out]
  Returns:
    True if the light is being edited and its location
    is available.  False if light is not being edited,
    in which case the fixed location is returned.
  */
  bool GetDynamicLightLocation( ON_Light& light ) const;

  /*
  Description:
    Set the location of a light that is being dynamically
    edited by grip dragging or a transformation command.
  Properties:
    light - [out]
  Returns:
    True if the light is a valid setting for a dynamic light.
  */
  bool SetDynamicLightLocation( const ON_Light& light );

  /*
  Description:
    Delete the dynamic light.
  */
  void DeleteDynamicLightLocation();

private:
  friend class CRhLightGrips;
  friend class CRhinoDoc;
  friend class CRhinoLightTable;
  friend class ON_ClassArray<CRhinoLight>;
  CRhinoLight(CRhinoDoc& doc);
  ~CRhinoLight();
  CRhinoLight& operator=(const CRhinoLight&);
  CRhinoLight( const CRhinoLight& );

  // m_light.m_name is kept in synch with m_attributes.m_name.
  // m_attributes.m_uuid is the object uuid for the light.
  // m_light.m_uuid is the uuid of the rendering plug-in app that
  // created the light.
  ON_Light m_light; // if light index >= 0, then this light is in m_light_table[]
  class CRhLightExtension* m_ex;
  void SynchAttributes() override;
};

class RHINO_SDK_CLASS CRhinoLightTable : private ON_SimpleArray<CRhinoLight*>
{
public:
  ~CRhinoLightTable();

  // Description:
  //   Light tables store the list of lights in a
  //   Rhino document.
  //
  //  Returns:
  //    CRhinoDocument that owns this light table.
  CRhinoDoc& Document() const;

  const class CRhinoDocTableReference& TableReference() const;

  // Returns:
  //   Number of lights in the light table, including deleted
  //   lights.
  int LightCount() const;

  // Description:
  //   Conceptually, the light table is an array of LightCount()
  //   lights.  The operator[] can be used to get individual
  //   lights.  A light is either active or deleted and this
  //   state is reported by CRhinoLight::IsDeleted().
  //
  // Parameters:
  //   light_index - zero based array index
  //
  // Returns:
  //   Reference to the light.  If light_index is out of range,
  //   the current light is returned. Note that this reference
  //   may become invalid after AddLight() is called.
  const CRhinoLight& operator[](
    int light_index
    ) const;

  /*
  Description:
    Finds a light with the given name. (There may be more
    than one light with the same name)
  Parameters:
    light_name - [in] name of light to search for.  The
        search ignores case.
    bIgnoreDeletedLights - [in] TRUE means don't search
        deleted lights.
    first_light_index - [in] searching begins at this
        index.
  Example:

          // Find all undeleted lights named "Fred Smith"

          const wchar_t* sFredSmith = L"Fred Smith";
          ON_SimpleArray<int> index_list;
          BOOL32 bIgnoreDeletedLights = TRUE;
          for(int light_index = 0; 
              light_index >= 0 && light_index < doc->m_light_table.LightCount(); 
              light_index++)
          {
            light_index = doc->m_light_table.Find(sFredSmith,bIgnoreDeletedLights,light_index);
            if ( light_index >= 0 )
              index_list.Append(light_index);
          }
          RhinoApp.Print(L"Found %d lights named \"%s\"\n",index_list.Count(), sFredSmith );

  Returns:
    @untitle table
    >=0     index of the light with the given name
    -1      no light has the given name
  */
  int FindLight( 
        const wchar_t* light_name,
        BOOL32  bIgnoreDeletedLights = TRUE,
        int first_light_index = 0
        ) const;

  int FindLight( 
          ON_UUID light_id, 
          bool bIgnoreDeleted = true
          ) const;


  /*
  Description:
    Adds a new light with specified definition to the light table.
 
  Parameters:
    light - [in] definition of new light.  The information
        in light is copied.  If light.LightName() is empty
        the a unique name of the form "Light 01" will be
        automatically created.
    attributes - [in] NULL or attributes (layer, etc.) for light
        See remarks for details about name.
 
  Returns:
    @untitled table
    >=0     index of new light
    -1      light not added because a light with that name
            already exists.
 
  Remarks:
    If attributes is not NULL, attributes->m_name is ignored. 
    light.LightName() specifies the light's name.

    In some cases, calling AddLight() can cause the memory 
    references previously returned by operator[] and 
    CurrentLight() to become invalid.
  */
  int AddLight( 
     const ON_Light& light,
     const ON_3dmObjectAttributes* attributes = 0
     );

  /*
  Description:
    Modify light settings
 
  Parameters:
    light - [in] new settings.  This information is copied.
    light_index - [in] zero based index of light to set.  This
         must be in the range 0 <= light_index < LightCount().
    bEnableUndo - [in] if TRUE, previous settings are saved for
        possible undo.
    bQuiet - [in] if TRUE, information message boxes pop up
        when illegal changes are attempted.
 
  Returns:
    TRUE if successful. FALSE if light_index is out of range.
  */
  bool ModifyLight( 
     const ON_Light& light,
     int light_index,
     BOOL32 bEnableUndo = TRUE,
     BOOL32 bQuiet = FALSE
     );

  // Description:
  //   If the light has been modified and the modification can be undone,
  //   then UndoModify() will restore the light to its previous state.
  //
  // Returns:
  //   TRUE if this light had been modified and the modifications were
  //   undone.
  //
  // See Also: IsModified.
  bool UndoModifyLight(
     int light_index,
     unsigned int ur_sn = 0
     );

  // Description:
  //   Deletes light
  //
  // Parameters:
  //   light_index - [in] zero based index of light to delete.
  //       This must be in the range 0 <= light_index < LightCount().
  //   bQuiet - [in] If TRUE, no warning message box appears if a light
  //       the light cannot be deleted because it is the current light
  //       or it contains active geometry.
  //
  // Returns:
  //   TRUE if successful. FALSE if light_index is out of range or the
  //   the light cannot be deleted because it is the current light or
  //   because it light contains active geometry.
  bool DeleteLight(
     int light_index,
     bool bQuiet
     );

  // Description:
  //   Deletes multiple lights
  //
  // Parameters:
  //   light_index_count - [in] length of light_index[] array.
  //   light_index - [in] array zero based indices of lights to delete.
  //       The indices must be in the range 0 <= light_index < LightCount().
  //   bQuiet - [in] If TRUE, no warning message box appears if a light
  //       the light cannot be deleted because it is the current light
  //       or it contains active geometry.
  //
  // Returns:
  //   TRUE if successful. FALSE if light_index is out of range or the
  //   the light cannot be deleted because it is the current light or
  //   because it light contains active geometry.
  bool DeleteLights(
     int light_index_count,
     const int* light_index,
     bool bQuiet
     );

  // Description:
  //   Undeletes a light that has been deleted by DeleteLight().
  //
  // Parameters:
  //   light_index - [in] zero based index of light to undelete.
  //       This must be in the range 0 <= light_index < LightCount().
  //
  // Returns:
  //   TRUE if successful.
  bool UndeleteLight(
     int light_index
     );

  // Description:
  //   Use this to set CRhinoLight::m_sort_index so that
  //   the values of m_sort_index run from 0 to LightCount()-1
  //   and compare(light_table[i],light_table[j]) < 0 ) implies 
  //   light_table[i].m_sort_index < light_table[j].m_sort_index.
  //
  // Parameters:
  //   compare - [in] compare function with prototype
  //       int compare(const CRhinoLight* arg1,const CRhinoLight* arg2,void* p).
  //       that returns <0 if arg1<arg2, 0 if arg1=arg2, 
  //       and >0 if arg1>arg2.
  //   cull - [in] optional filter function with prototype
  //       BOOL32 cull(const CRhinoLight* arg,void* p).
  //       that returns TRUE if the light should be ignored
  //       when sorting.  Lights that are ignored are not included
  //       in the list returned by GetSortedList(). Pass 0
  //       if you do not need to cull lights
  //   p - [in] pointer passed as last argument to compare() and cull().
  //
  // Remarks:
  //   After calling Sort(), you can repeatedly call GetSortedList()
  //   to get a sorted list of CRhinoLight pointers.  Sort() and
  //   GetSortedList() do not modify the order or persistent information
  //   in the light table.  They are intended to be used to get
  //   sorted lists of lights for dialogs, etc.
  void Sort( 
    int (*compare)(const CRhinoLight*,const CRhinoLight*,void*),
    int (*cull)(const CRhinoLight*,void*),
    void* p=0
    );

  // Description:
  //   Gets an array of pointers to lights that is sorted by
  //   the values of CRhinoLight::m_sort_index.
  //
  // Parameters:
  //   sorted_list - [out] this array is returned with length
  //       LightCount() and is sorted by the values of
  //       CRhinoLight::m_sort_index.
  //   bIgnoreDeleted - [in] TRUE means don't include
  //       deleted lights.
  //
  // Remarks:
  //   Use Sort() to set the values of m_sort_index.
  void GetSortedList(
    ON_SimpleArray<const CRhinoLight*>&, // sorted_list
    bool = false // bIgnoreDeleted
  ) const;

  // Description:
  //   Gets unsed light name used as default when creating new lights
  //
  // Parameters:
  //   result - [out] this is the wString which receives new name
  //
  void GetUnusedLightName( ON_wString& result, const wchar_t* lpsPrefix = NULL) const;


  /*
  Description:
    Draw wireframe icons for lights that are visible and not deleted.
  Parameters:
    vp - [in]
  */
  void DrawWireframeIcons( CRhinoDisplayPipeline& vp ) const;

  /*
  Description:
    Get bounding box of wireframe icons for lights that are visible
    and not deleted.
  Returns:
    Bounding box of wireframe icons for lights that are visible
    and not deleted.
  */
  ON_BoundingBox WireframeIconsBoundingBox( const CRhinoViewport* viewport ) const;

  const CRhinoLight& DefaultLight() const;

  /*
  Description:
    Gets lighting model defined by the light table.
  Parameters:
    lights - [out] world coordinate lighting model returned here.
    model - [in] model used for the lighting
         0 - lighting model for
               wireframe display mode is returned
         1 - default lighting model for a shaded
               display mode is returned
         2 - lighting model for lighting the Scene
               the lights returned are the ones 
               defined and placed in the scene
  Returns:
    number of lights.  
  See Also:
    CRhinoView::GetLightingModel
  */
  int GetLightingModel(
        ON_ObjectArray<ON_Light>& lights,
        int model
        ) const;


  void SetRemapIndex(
    int, // light_index
    int  // remap_index
    );

  void ListTable( 
    ON_TextLog& text_log, 
    unsigned int level_of_detail 
    ) const;

private:
  friend class CRhinoDoc;
  CRhinoLightTable(CRhinoDoc&);
  CRhinoDoc& m_doc;
  CRhinoDocTableReference m_table_reference;
  ON_ComponentManifest& Internal_Manifest();

  CRhinoLight m_default_light;

  // used to quickly find lights by uuid;
  ON_UuidIndexList m_light_id_list;

  BOOL32 IndexInRange( int light_index, const wchar_t* lpsMessageTitle = 0);
  
  void Destroy();

private:
  // no default constructor
  CRhinoLightTable();
  // no copy constructor
  CRhinoLightTable(const CRhinoLightTable&);
  // no operator=
  CRhinoLightTable& operator=(const CRhinoLightTable&);
};

