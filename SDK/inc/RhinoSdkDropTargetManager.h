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

#if defined (RHINO_SDK_MFC)
class RHINO_SDK_CLASS CRhinoDropTarget
{
public:
  CRhinoDropTarget();
  virtual ~CRhinoDropTarget();

  // Required overrides
  //
  /*
    Description:
      This method is called by Rhino to determine if this drop data object is associated
      with this drop target.  Rhino will forward messages to the first enabled drop target
      whos SupportDataObject() returns true.
    Parameters:
      pDataObject [in] Points to the data object containing the data that can be dropped.
    Returns:
      Return true if this drop target supports the specified data object.
  */
  virtual bool SupportDataObject( COleDataObject* pDataObject) = 0;
  //
  // Optional, sometimes overridden
  //
  // See MFC help for class COleDropTarget for description of these methods
  //
  /*
    Description:
      Called by the framework when the cursor is first dragged into the window
    Parameters:
      pWnd        [in] Points to the window the cursor is entering.
      pDataObject [in] Points to the data object containing the data that can be dropped.
      dwKeyState  [in] Contains the state of the modifier keys. This is a combination
                       of any number of the following: MK_CONTROL, MK_SHIFT, MK_ALT,
                       MK_LBUTTON, MK_MBUTTON, and MK_RBUTTON.
      point       [in] Contains the current location of the cursor in client coordinates.
    Returns:
      The effect that would result if a drop were attempted at the location specified by point.
      It can be one or more of the following: 
        DROPEFFECT_NONE   A drop would not be allowed.
        DROPEFFECT_COPY   A copy operation would be performed.
        DROPEFFECT_MOVE   A move operation would be performed.
        DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
        DROPEFFECT_SCROLL   A drag scroll operation is about to occur or is occurring in the target.
    See Also:
      See MFC help for class COleDropTarget for description
  */
  virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, const ON_2iPoint& point);
  /*
    Description:
      Called by the framework when the cursor is dragged over the window.
    Parameters:
      pWnd        [in] Points to the window that the cursor is over.
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dwKeyState  [in] Contains the state of the modifier keys. This is a combination of any number of
                       the following: MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON, MK_MBUTTON, and MK_RBUTTON.
      point       [in] Contains the current location of the cursor in client coordinates.
    Returns:
      The effect that would result if a drop were attempted at the location specified by point. It can
      be one or more of the following: 
        DROPEFFECT_NONE   A drop would not be allowed.
        DROPEFFECT_COPY   A copy operation would be performed.
        DROPEFFECT_MOVE   A move operation would be performed.
        DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
        DROPEFFECT_SCROLL Indicates that a drag scroll operation is about to occur or is occurring in the target.
    See Also:
      See MFC help for class COleDropTarget for description
  */
  virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, const ON_2iPoint& point);
  /*
    Description:
      Called by the framework when the cursor is dragged over the window.
    Parameters:
      pWnd        [in] Points to the window that the cursor is over.
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dwKeyState  [in] Contains the state of the modifier keys. This is a combination of any number
                       of the following: MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON, MK_MBUTTON,
                       and MK_RBUTTON.
      point       [in] Contains the current location of the cursor in client coordinates.
    Returns:
      The effect that would result if a drop were attempted at the location specified by point.
      It can be one or more of the following: 
        DROPEFFECT_NONE   A drop would not be allowed.
        DROPEFFECT_COPY   A copy operation would be performed.
        DROPEFFECT_MOVE   A move operation would be performed.
        DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
        DROPEFFECT_SCROLL A drag scroll operation is about to occur or is occurring in the target.
    See Also:
      See MFC help for class COleDropTarget for description
  */
  virtual BOOL32 OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, const ON_2iPoint& point);
  /*
    Description:
      Called by the framework when a drop operation is to occur.
    Parameters:
      pWnd        [in] Points to the window the cursor is currently over.
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dropEffect  [in] The effect that the user chose for the drop operation. It can be one or more
                       of the following:
                         DROPEFFECT_COPY   A copy operation would be performed.
                         DROPEFFECT_MOVE   A move operation would be performed.
                         DROPEFFECT_LINK   A link from the dropped data to the original data would
                                           be established.
      point       [in] Contains the location of the cursor, in pixels, relative to the screen.
    Returns:
      Nonzero if the drop is successful; otherwise 0.
    See Also:
      See MFC help for class COleDropTarget for description
  */
  virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, const ON_2iPoint& point);
  /*
    Description:
      Called by the framework when a drop operation is to occur.
    Parameters:
      pWnd        [in] Points to the window the cursor is currently over.
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dropDefault [in] The effect that the user chose for the default drop operation based on the current
                       key state. It can be DROPEFFECT_NONE. Drop effects are discussed in the Comments section.
      dropList    [in] A list of the drop effects that the drop source supports. Drop effect values can be
                       combined using the bitwise OR (|) operation. Drop effects are discussed in the Comments
                       section.
      point       [in] Contains the location of the cursor, in pixels, relative to the screen.
    Returns:
      The drop effect that resulted from the drop attempt at the location specified by point. Drop effects are
      discussed in the Comments section.
    Comments:
      The framework first calls this function. If it does not handle the drop, the framework then calls OnDrop.
      Typically, you will override OnDropEx in the view class to support right mouse-button drag and drop. Typically,
      the view class OnDrop is used to handle the case of support for simple drag and drop.

      The default implementation of COleDropTarget::OnDropEx calls CView::OnDropEx. By default, CView::OnDropEx
      simply returns a dummy value to indicate the OnDrop member function should be called.

      Drop effects describe the action associated with a drop operation. See the following list of drop effects: 
        DROPEFFECT_NONE   A drop would not be allowed.
        DROPEFFECT_COPY   A copy operation would be performed.
        DROPEFFECT_MOVE   A move operation would be performed.
        DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
        DROPEFFECT_SCROLL Indicates that a drag scroll operation is about to occur or is occurring in the target.

      For more information, see IDropTarget::Drop in the Platform SDK.
    See Also:
      See MFC help for class COleDropTarget for description
  */
  virtual void OnDragLeave(CWnd* pWnd);
  /*
    Description:
      Called by frame work when data object is dropped on an object.
    Parameters:
      obj_ref     [in] Object data was dropped on (note that component index(m_component_index) and group membership (Flags()&64) are supported from Rhino 6 onwards)
      pRhinoView  [in] View the drop action occurred in
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dropEffect  [in] The desired drop affect, can be one or more of the following: 
                         DROPEFFECT_NONE   A drop would not be allowed.
                         DROPEFFECT_COPY   A copy operation would be performed.
                         DROPEFFECT_MOVE   A move operation would be performed.
                         DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
                         DROPEFFECT_SCROLL A drag scroll operation is about to occur or is occurring in the target.
      point       [in] Contains the current location of the cursor in client coordinates.
    Returns:
      Returns true if data object was successfully dropped on the specified object      
    See Also:
      CRhinoDropTarget::OnDropOnLayerListCtrl
      CRhinoDropTarget::OnDropOnRhinoView
      CRhinoDropTarget::OnDrop
  */
  virtual bool OnDropOnObject( CRhinoObjRef obj_ref, CRhinoView* pRhinoView, COleDataObject* pDataObject, DROPEFFECT dropEffect, const ON_2iPoint& point);

  /*
    Description:
      Called by frame work when data object is dropped on the layer list control
    Parameters:
      pLayerListCtrl [in] The layer list control window the object was dropped on.
      nLayerIndex    [in] The zero base layer table index the data object was dropped on or -1 if it was dropped
                          on the control but not on a layer.
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dropEffect  [in] The desired drop affect, can be one or more of the following: 
                         DROPEFFECT_NONE   A drop would not be allowed.
                         DROPEFFECT_COPY   A copy operation would be performed.
                         DROPEFFECT_MOVE   A move operation would be performed.
                         DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
                         DROPEFFECT_SCROLL A drag scroll operation is about to occur or is occurring in the target.
      point       [in] Contains the current location of the cursor in client coordinates.
    Returns:
      Returns true if data object was successfully dropped.
    See Also:
      CRhinoDropTarget::OnDropOnObject
      CRhinoDropTarget::OnDropOnRhinoView
      CRhinoDropTarget::OnDrop
  */
  virtual bool OnDropOnLayerListCtrl( HWND pLayerListCtrl, int nLayerIndex, COleDataObject* pDataObject, DROPEFFECT dropEffect, const ON_2iPoint& point);
  /*
    Description:
      Called by frame work when data object is dropped on a Rhino view and does not land on a selectable object.
    Parameters:
      pRhinoView  [in] View the drop action occurred in
      pDataObject [in] Points to the data object that contains the data to be dropped.
      dropEffect  [in] The desired drop affect, can be one or more of the following: 
                         DROPEFFECT_NONE   A drop would not be allowed.
                         DROPEFFECT_COPY   A copy operation would be performed.
                         DROPEFFECT_MOVE   A move operation would be performed.
                         DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
                         DROPEFFECT_SCROLL A drag scroll operation is about to occur or is occurring in the target.
      point       [in] Contains the current location of the cursor in client coordinates.
    Returns:
      Returns true if data object was successfully dropped.
    See Also:
      CRhinoDropTarget::OnDropOnObject
      CRhinoDropTarget::OnDropOnLayerListCtrl
      CRhinoDropTarget::OnDrop
  */
  virtual bool OnDropOnRhinoView( CRhinoView* pRhinoView, COleDataObject* pDataObject, DROPEFFECT dropEffect, const ON_2iPoint& point);
  /*
    Description:
      Called by the frame work at the very beginning and end of a drag event.
  */
  virtual void ResetDropFlags();
  /*
    Description:
      Called at the end of the drop event.
    Parameters:
      pWnd        [in] Points to the window the cursor is entering.
      pDataObject [in] Points to the data object containing the data that can be dropped.
      dropEffect  [in] The effect that the user chose for the drop operation. It can be one or more
                       of the following:
                         DROPEFFECT_COPY   A copy operation would be performed.
                         DROPEFFECT_MOVE   A move operation would be performed.
                         DROPEFFECT_LINK   A link from the dropped data to the original data would
                                           be established.
      point       [in] Contains the current location of the cursor in client coordinates.
  */
  virtual void OnDropDone( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, const ON_2iPoint& point);
  /*
    Description:
      Call this method to enable or disable the drop target.  If the drop target is disabled then
      it will not get called by the frame work during drop events.
    Parameters:
      bEnable [in] If true then the drop target will be notified of drop events otherwise it will not.
    See Also:
      CRhinoDropTarget::IsRhinoDropTargetEnabled
  */
  void EnableRhinoDropTarget( bool bEnable);
  /*
    Description:
      Call this method to find out if this drop target is enabled or not.
    Returns:
      Returns true if the drop target will be notified of drop events otherwise it returns false.
    See Also:
      CRhinoDropTarget::EnableRhinoDropTarget
     
  */
  bool IsRhinoDropTargetEnabled() const;
  /*
    Description:
      Call this method to find out if the user pressed the Escape key to cancel the drop event.
    Returns:
      Returns true if the drop event was canceled otherwise returns false
    See Also:
      CRhinoDropTarget::OnDropExWasCalled()
  */
  bool OnDropWasCalled() const;
  /*
    Description:
      Call this method to find out if the user pressed the Escape key to cancel the drop event.
    Returns:
      Returns true if the drop event was canceled otherwise returns false
    See Also:
      CRhinoDropTarget::OnDropWasCalled()
  */
  bool OnDropExWasCalled() const;

#if defined(ON_RUNTIME_WIN)
  /*
    Description:
      Call this method to get the CImageList associated with this drop target.
    Returns:
      Returns a pointer to the CImageList which will be used when dragging or NULL if none will
      be used.
    See Also:
      CRhinoDropTarget::SetDragImageList()
      CRhinoDropTarget::DragImageIndex()
  */
  CImageList* GetDragImageList() const;
  /*
    Description:
      Call this method to set the CImageList associated with this drop target.
    Parameters:
      pImageList [in] Pointer to the CImageList to use when dragging or NULL to use none
    See Also:
      CRhinoDropTarget::GetDragImageList()
      CRhinoDropTarget::DragImageIndex()
  */
  void SetDragImageList( CImageList* pImageList);
  /*
    Description:
      Zero based index into image list associated with this drop target.  It will only
      be used if SetDragImageList() was called and an image list was specified.
    Returns:
      Zero based index into image list associated with this drop target.  It will only
      be used if SetDragImageList() was called and an image list was specified.
    See Also:
      CRhinoDropTarget::DragImageList()
      CRhinoDropTarget::SetDragImageList()
      CRhinoDropTarget::SetDragImageIndex()
  */
  int DragImageIndex() const;
  /*
    Description:
      Call this method to set the zero based index into the DragImageList().  Setting this
      to -1 will disable the drag image.
    Parameters:
      iIndex [in] Zero based drag image index or -1 to not display a drag image.
    See Also:
      CRhinoDropTarget::DragImageList()
      CRhinoDropTarget::SetDragImageList()
      CRhinoDropTarget::DragImageIndex()
  */
  void SetDragImageIndex( int iIndex);
  /*
    Description:
      Coordinates of the starting drag position (typically, the cursor position). The coordinates
      are relative to the upper left corner of the drag image.
    See Also:
      See CImageList::BeginDrag() in MSDN
      CRhinoDropTarget::SetDragImageHotSpot()
      CRhinoDropTarget::DragImageList()
      CRhinoDropTarget::SetDragImageList()
      CRhinoDropTarget::SetDragImageIndex()
  */
  ON_2iPoint DragImageHotSpot() const;
  /*
    Description:
      Coordinates of the starting drag position (typically, the cursor position). The coordinates
      are relative to the upper left corner of the drag image.
    Parameters:
      Coordinates of the starting drag position relative to the upper left corner of the drag image.
    See Also:
      See CImageList::BeginDrag() in MSDN
      CRhinoDropTarget::DragImageHotSpot()
      CRhinoDropTarget::DragImageList()
      CRhinoDropTarget::SetDragImageList()
      CRhinoDropTarget::SetDragImageIndex()
  */
  void SetDragImageHotSpot(const ON_2iPoint& pt);
#endif
  /*
    Description:
    Parameters:
    Returns:
    See Also:
  */
  bool RightMouseDrag() const;
  /*
    Description:
      Call this method to find out if the control key was pressed when the drag event started.
    Returns:
      Returns true if the control key was pressed when the drag event started.
  */
  bool CtrlKeyDownWhenDragStarted() const;
  /*
    Description:
      Call this method to find out if the shift key was pressed when the drag event started.
    Returns:
      Returns true if the shift key was pressed when the drag event started.
  */
  bool ShiftKeyDownWhenDragStarted() const;
  /*
    Description:
      Call this method to find out if a command was running when the drag event started.
    Returns:
      Returns true if a command was running when the drag event started.
  */
  bool DragStartedWhileInCommand() const;
  /*
    Description:
      Call this method to unselect any currently selected objects.
  */
  void DeselectAllObjects() const;
  /*
    Description:
      Call this method to enable/disable calling of CRhinoDropTarget::OnDropEx()
    Parameters:
      b [in] If true then OnDropEx() will be called otherwise it will not
    Returns:
      Returns previous state
    See Also:
      CRhinoDropTarget::AllowOnDropEx()
  */
  bool EnableAllowOnDropEx( bool b = true);
  /*
    Description:
      Call this method to find out if CRhinoDropTarget::OnDropEx() should
      get called.
    Returns:
      Returns true if CRhinoDropTarget::OnDropEx() shold be called otherwise
      returns false
    See Also:
      CRhinoDropTarget::EnableAllowOnDropEx()
  */
  bool AllowOnDropEx() const;
  /*
    Description:
      Call this method to enable or disable dropping while a command
      is running.  The default behavior is to not allow dropping while
      a command is running.
    Parameters:
      b [in] If true then droppgin will be allowed while commands are
             running otherwise it will not.
    Returns:
      Returns previous value for this setting.
    See Also:
      CRhinoDropTarget::AllowDropWhileInRhinoCommand
  */
  bool EnableAllowDropWhileInRhinoCommand( bool b = true);
  /*
    Description:
      Call this method to find out if droppging while a Rhino command
      is running is allowed.
    Returns:
      Returns true if dropping on objects is allowed while a command
      is running otherwise false.
    See Also:
      CRhinoDropTarget::EnableAllowDropWhileInRhinoCommand
  */
  bool AllowDropWhileInRhinoCommand() const;
  /*
    Description:
      Allow drag over to select objects.  If this is enabled
      and something is dropped on a valid Rhino object then
      the object is selected and CRhinoDropTarget::OnDropOnObject
      is called.
    Parameters:
      b - [in] If true then objects will be selected when dragged
      over.
    Returns:
      Previous value.
    See Also:
      CRhinoDropTarget::AllowDropOnObject
      CRhinoDropTarget::SetGeometryFilter
      CRhinoDropTarget::SetGeometryAttributeFilter
  */
  bool EnableAllowDropOnObject( bool b = true);
  /*
    Description:
      Allow drag over to select objects.  If this is enabled
      and something is dropped on a valid Rhino object then
      the object is selected and CRhinoDropTarget::OnDropOnObject
      is called.
    Returns:
      true then objects will be selected when dragged
      over.
    See Also:
      CRhinoDropTarget::EnableAllowDropOnObject
      CRhinoDropTarget::SetGeometryFilter
      CRhinoDropTarget::SetGeometryAttributeFilter
  */
  bool AllowDropOnObject() const;
  /*
    Description:
      The geometry type filter controls which types of geometry
      (points, curves, surfaces, meshes, etc.) can be selected.  
      The default geometry type filter permits selection of all 
      types of geometry.  This is only valid if EnableDropOnObject()
      is set to true.
  
    Parameters:
      geometry_type_filter - [in] an unsigned int made by or-ing ( | )
          values from the CRhinoGetObject::GEOMETRY_TYPE_FILTER enum.
          If you pass 0, then all geometry will be acceptable.

    Example:
      If you want to be able to select curves and meshes, then
      you would make a call like

         unsigned int geometry_type_filter = 0;
         geometry_filter |= CRhinoGetObject::curve_object;
         geometry_filter |= CRhinoGetObject::mesh_object;
         SetGeometryFilter( geometry_type_filter );

    See Also:
      CRhinoDropTarget::GeometryFilter
      CRhinoDropTarget::SetGeometryAttributeFilter
      CRhinoDropTarget::GeometryAttributeFilter
  */
  void SetGeometryFilter( unsigned int geometry_type_filter);
  /*
    Description:
      The geometry type filter controls which types of geometry
      (points, curves, surfaces, meshes, etc.) can be selected.  
      The default geometry type filter permits selection of all 
      types of geometry.  This is only valid if EnableDropOnObject()
      is set to true.
  
    Returns:
      An unsigned int "object_filter".  If the bit from an
      CRhinoGetObject::GEOMETRY_TYPE_FILTER enum value is set
      in the geometry_filter, then OnDragOver() will permit 
      selection of object's that have that type of geometry.
      If geometry_filter is 0xFFFFFFFF, then OnDragOver()
      can select all types of geometry.

    Example:
      If (GeometryFilter() | curve_object) != 0, then GetObjects()
      will permit curves to be selected.

    See Also:
      CRhinoGetObject::GEOMETRY_TYPE_FILTER
      CRhinoGetObject::GeometryFilter()
      CRhinoDropTarget::EnableDropOnObject
      CRhinoDropTarget::DropOnObject
      CRhinoDropTarget::SetGeometryFilter
      CRhinoDropTarget::SetGeometryAttributeFilter
      CRhinoDropTarget::GeometryAttributeFilter
  */
  unsigned int GeometryFilter() const;
  /*
    Description:
      The geometry attribute filter provides a secondary filter
      can be used to restrict which objects can be selected.  
      Control of the type of geometry (points, curves, surfaces,
      meshes, etc.) is provided by CRhinoGetObject::SetGeometryFilter.
      The geometry attribute filter is used to require the selected
      geometry to have certain attributes (open, closed, etc.).
      The default attribute filter permits selection of all types of 
      geometry.  This is only valid if EnableDropOnObject() is set
      to true.
  
    Parameters:
      geometry_attribute_filter - [in] an unsigned int made by 
          bitwise or-ing ( | ) values from the 
          CRhinoGetObject::GEOMETRY_ATTRIBUTE_FILTER enum.
          If you pass 0, then all geometry attributes will be acceptable.

    Example:
      If you want to be able to select open curves or solids, then
      you would make a call like

         unsigned int geometry_type_filter = 0;
         unsigned int geometry_attribute_filter = 0;
         geometry_type_filter |= CRhinoGetObject::curve;
         geometry_type_filter |= CRhinoGetObject::surface_object;
         geometry_type_filter |= CRhinoGetObject::polysurface_object;
         geometry_attribute_filter |= CRhinoGetObject::closed_curve;
         geometry_attribute_filter |= CRhinoGetObject::solid_object;
         SetSetGeometryFilter( geometry_type_filter );
         SetGeometryAttributeFilter( geometry_attribute_filter );

    See Also:
      CRhinoDropTarget::EnableDropOnObject
      CRhinoDropTarget::DropOnObject
      CRhinoDropTarget::GeometryAttributeFilter.
      CRhinoDropTarget::SetGeometryFilter.
      CRhinoDropTarget::GeometryFilter.
  */
  void SetGeometryAttributeFilter( unsigned int geometry_attribute_filter);
  /*
    Description:
      The geometry attribute filter provides a secondary filter
      can be used to restrict which objects can be selected.  
      Control of the type of geometry (points, curves, surfaces,
      meshes, etc.) is provided by CRhinoDropTarget::SetGeometryFilter.
      The geometry attribute filter is used to require the selected
      geometry to have certain attributes (open, closed, etc.).
      The default attribute filter permits selection of all types of 
      geometry.  This is only valid if EnableDropOnObject() is set
      to true.
  
    Returns:
      An unsigned int "object_attribute_filter".  If the bit from an
      CRhinoGetObject::GEOMETRY_ATTRIBUTE_FILTER enum value is set in
      the geometry_attribute_filter, then OnDragOver() will permit 
      selection of object's that have that geometric attribute.  If 
      geometry_attribute_filter is 0xFFFFFFFF, then OnDragOver() can
      select all types of geometry.

    Example:
      If (GeometryAttributeFilter() | closed_curve) != 0, then 
      any curve selected by OnDragOver() will be closed.

    See Also:
      CRhinoDropTarget::EnableDropOnObject
      CRhinoDropTarget::DropOnObject
      CRhinoDropTarget::SetGeometryAttributeFilter.
      CRhinoDropTarget::SetGeometryFilter.
      CRhinoDropTarget::GeometryFilter.
  */
  unsigned int GeometryAttributeFilter() const;
  /*
  Description:
    Checks geometry to see if it passes the basic
    GeometryAttributeFilter.  This is only valid if
    EnableDropOnObject() is set to true.
  Parameters:
    object - [in] parent object being considered.
    geometry - [in] geometry being considered.
    component_index - [in] if >= 0, geometry is a proper
       sub-part of object->Geometry() with component_index.
  Remarks:
    The default simply checks that the geometry passes 
    the filter returned by GeometryAttributeFilter().
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::EnablePostSelect
    CRhinoDropTarget::PassesGeometryTypeFilter
  */
  virtual bool CustomGeometryFilter( const CRhinoObject* object, const ON_Geometry* geometry,
                                              ON_COMPONENT_INDEX component_index) const;
  /*
  Description:
    Checks geometry to see if it passes the basic
    GeometryAttributeFilter.
  Parameters:
    object - [in] parent object being considered.
    geometry - [in] geometry being considered.
    component_index - [in] if >= 0, geometry is a proper
       sub-part of object->Geometry() with component_index.
  Remarks:
    The default simply checks that the geometry passes 
    the filter returned by GeometryAttributeFilter().
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::EnablePostSelect
    CRhinoDropTarget::PassesGeometryTypeFilter
  */
  bool PassesGeometryAttributeFilter( const CRhinoObject* object, const ON_Geometry* geometry, 
                                               ON_COMPONENT_INDEX component_index) const;
  /*
  Description:
    By default, CRhinoDropTarget::OnDragOver will permit a user
    to select only top level objects.  To select sub-objects
    (like a curve in a b-rep or a curve in a group) call
    EnableSubObjectSelect( true).
  Parameters:
    b - [in] 
       true to enable sub-object selection
       false to disable sub-object selection.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::SubObjectSelect
  */
  bool EnableAllowDropOnSubObject( bool b = true);
  /*
  Description:
    By default, CRhinoDropTarget::OnDragOver will permit a user
    to select only top level objects.  To select sub-objects
    (like a curve in a b-rep or a curve in a group) call
    EnableSubObjectSelect( true).
  Returns:
    true if selection of sub-objects is permitted.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::EnableSubObjectSelect
  */
  bool AllowDropOnSubObject() const;
  /*
  Description:
    By default, groups are ignored in OnDragOver.  If you
    want the drag event to select every object  in a group 
    then enable group selection.
  Parameters:
    b - [in]
      true to enable selecting all objects in a group with a
        single drop event
      false to only select the object dropped on ignoring the
        other objects in the group it belongs to.
  Returns:
    Returns previous value.
  See Also:
    CRhinoDropTarget::GroupSelect
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::GroupSelect
  */
  bool EnableGroupSelect( bool b = true);
  /*
  Description:
    By default, groups are ignored in OnDragOver.  If you
    want the drag event to select every object  in a group 
    then enable group selection.
  Returns:
    true, When dropping on an object in a group the entire
          group will be selected.
    true, When dropping on an object in a group only the
          object dropped on gets selected.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::GroupSelect
  */
  bool GroupSelect() const;
  /*
  Description:
    By default, reference objects can not be selected.
    If you do not want to be able to select reference
    objects, then call EnableReferenceObjectSelect( true).
  Parameters:
    b - [in]
      true to enable selection of referenced objects
      false reference objects will not be selected
  Returns:
    Returns previous value.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::ReferenceObjectSelect
  */
  bool EnableReferenceObjectSelect( bool b = true);
  /*
  Description:
    By default, reference objects can not be selected.
    If you do not want to be able to select reference
    objects, then call EnableReferenceObjectSelect( true).
  Returns:
    true, then reference objects may be selected by drag
          and drop.
    false, then reference objects are ignored by drag
          and drop.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
    CRhinoDropTarget::EnableReferenceObjectSelect
  */
  bool ReferenceObjectSelect() const;
  /*
  Description:
    Check to see if mouse is currently over an object.
    This is only valid if CRhinoDropTarget::DropOnObject()
    is true.
  Returns:
    true when the mouse is over object that is selected and 
      it passes all selection filter tests.
    false when nothing is selected.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
  */
  bool OverObject() const;
  /*
  Description:
    Check to see if mouse is currently over an object.
    This is only valid if CRhinoDropTarget::DropOnObject()
    is true.
  Returns:
    Object the mouse is currently over.  If CRhinoObjRef.Object()
    returns NULL then the mouse is not over a valid object.
  See Also:
    CRhinoDropTarget::EnableDropOnObject
    CRhinoDropTarget::DropOnObject
  */
  CRhinoObjRef RhinoObject() const;
  /*
  Description:
    Allow drop action to happen on a valid CRhinoView without
    landing on an object.
  Parameters:
    b - [in]
      true to enable.
      false to disable.
  Returns:
    Previous state.
  See Also:
    CRhinoDropTarget::AllowDropOnRhinoView
  */
  bool EnableAllowDropOnRhinoView( bool b = true);
  /*
  Description:
    Allow drop action to happen on a valid CRhinoView without
    landing on an object.
  Returns:
    true means the drop event can terminate on a CRhinoView
  See Also:
    CRhinoDropTarget::EnableAllowDropOnRhinoView
  */
  bool AllowDropOnRhinoView() const;
  /*
  Description:
    Allow drop action to happen on the layer list control.  If
    this is set to true and the drop event is terminated over
    the layer list control then CRhinoDropTarget::OnDropOnLayerListCtrl
    will be called.
  Parameters:
    b - [in]
      true to enable.
      false to disable.
  Returns:
    Previous state.
  See Also:
    CRhinoDropTarget::OnDropOnLayerListCtrl
    CRhinoDropTarget::AllowDropOnRhinoLayerListControl
  */
  bool EnableAllowDropOnRhinoLayerListControl( bool b = true);
  /*
  Description:
    Allow drop action to happen on the layer list control.  If
    this is set to true and the drop event is terminated over
    the layer list control then CRhinoDropTarget::OnDropOnLayerListCtrl
    will be called.
  Returns:
    true to allow dropping on layer list control.
  See Also:
    CRhinoDropTarget::OnDropOnLayerListCtrl
    CRhinoDropTarget::EnableAllowDropOnRhinoLayerListControl
  */
  bool AllowDropOnRhinoLayerListControl() const;
  /*
  Description:
    Default value returned by CRhinoDropTarget::OnDrop... methods.
  Parameters:
    rc - [in]
      Value to be returned.
  Returns:
    Previous value.
  See Also:
    CRhinoDropTarget::DefaultOnEventReturnCode
  */
  DROPEFFECT SetDefaultOnEventReturnCode( DROPEFFECT rc = DROPEFFECT_COPY);
  /*
  Description:
    Default value returned by CRhinoDropTarget::OnDrop... methods.
  Returns:
    Default value returned by CRhinoDropTarget::OnDrop... methods.
  See Also:
    CRhinoDropTarget::DefaultOnEventReturnCode
  */
  DROPEFFECT DefaultOnEventReturnCode() const;
  /*
  Description:
    Controls the deselection of preselected input when
    drag event starts.
  Parameters:
    b - [in]
      If true then any objects currently selected will be
      deselected at beginig of drop event.
  See Also:
    CRhinoDropTarget::DeselectAllOnDrag
  */
  bool EnableDeselectAllOnDrag( bool b = true);
  /*
  Description:
    Controls the deselection of preselected input when
    drag event starts.
  Returns:
    True if pre-selected input will be deselected when drop
    event starts.
  Remarks:
    The default is false.
  See Also:
    CRhinoDropTarget::EnableDeselectAllOnDrag
  */
  bool DeselectAllOnDrag() const;
  /*
  Description:
    CRhinoView that the drag cursor is currently over.
  Returns:
    Valid point if over a view otherwise NULL.
  */
  CRhinoView* RhinoViewFromDragPoint( CWnd* pWnd, const ON_2iPoint& point, ON_2iPoint* ptView) const;
  CRhinoView* RhinoView() const;
  
#if defined(ON_RUNTIME_WIN)
  ON_DEPRECATED_MSG("Obsolete - do not use")
  HWND RhinoLayerListCtrlFromDragPoint(HWND pWnd, const ON_2iPoint& point, ON_2iPoint* ptList) const;

  ON_DEPRECATED_MSG("Obsolete - do not use")
  HWND RhinoLayerListCtrl() const;

  ON_DEPRECATED_MSG("Obsolete - do not use")
  int RhinoLayerIndex() const;

protected:
  void EndDragImageList();
#endif

private:
  CRhinoDropTarget( const CRhinoDropTarget&);// Not implemented
  const CRhinoDropTarget& operator=( const CRhinoDropTarget&);// Not implemented

protected:
  friend class CRhDropTargetManager;

  bool m_bEnabled;

  int m_drag_image_index;
  bool m_end_drag_image;
  ON_2iPoint m_drag_image_hot_spot;
  class CImageList* m_drag_image_list;

  class CRhDropTargetManager* m_pRhDropTargetManager;
  class CRhinoDropTarget__helper* m_helper;
};
#endif
