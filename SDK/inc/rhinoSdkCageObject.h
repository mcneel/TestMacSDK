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


class RHINO_SDK_CLASS CRhinoWireframeObject : public CRhinoObject
{
  // CRhinoWireframeObject is a pure virtual class for wireframe
  // objects that do not have to support subobject picking.
  ON_OBJECT_DECLARE(CRhinoWireframeObject);

public:
  CRhinoWireframeObject();                              // assigns a new object UUID
  CRhinoWireframeObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoWireframeObject();
  CRhinoWireframeObject( const CRhinoWireframeObject& src);          // copies source object UUID
  CRhinoWireframeObject& operator=(const CRhinoWireframeObject& src); // copies source object UUID

  // virtual CRhinoObject::Draw override
  void Draw(CRhinoDisplayPipeline& dp) const override;

  // virtual CRhinoObject::PickFilter override
  bool PickFilter( const CRhinoGetObject&, bool ) const override;

  // virtual CRhinoObject::Pick override
  int Pick( const CRhinoPickContext&, CRhinoObjRefArray& ) const override;
  
  // virtual CRhinoObject::SnapTo override
  bool SnapTo( 
     const CRhinoSnapContext& snap_context,
     CRhinoSnapEvent& snap_event
     ) const override;

  // virtual CRhinoObject::GetWireframeCurves override
  int GetWireframeCurves( ON_SimpleArray<ON_Curve*>& curve_list ) const override;

  virtual
  class CRhWireframeDisplay* WireframeDisplay() const = 0;

protected:
  class CRhWireframeDisplay* m_wireframe_display;
};


class RHINO_SDK_CLASS CRhinoCageObject : public CRhinoWireframeObject
{
  ON_OBJECT_DECLARE(CRhinoCageObject);

public:
  CRhinoCageObject();                              // assigns a new object UUID
  CRhinoCageObject(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoCageObject();

  /*
  Description:
    Sets the object's m_control to be a copy of the input parameter.
  Parameters:
    control - [in] copied.
  Returns:
    True if input is valid.
  */
  bool SetCage( const ON_NurbsCage& cage );

  /*
  Description:
    Sets the object's m_control to be the input parameter.
    ~CRhinoCageObject will delete the pointer.
  Parameters:
    control - [in]
  Returns:
    True if input is valid.
  */
  bool SetCage( ON_NurbsCage* cage );

  /*
  Description:
    Get a pointer to the object's NURBS cage.  
  Returns:
    Pointer to the object's ON_NurbsCage.
  Remarks:
   ~CRhinoCageObject deletes this pointer.
  */
  const ON_NurbsCage* Cage() const;

  /*
  Description:
    Override of CRhinoObject::ObjectType
  Returns:
    ON::cage_object
  */
  ON::object_type ObjectType() const override;

  // virtual CRhinoObject::EnableGrips override
  void EnableGrips(  bool bGripsOn ) override;

  /*
  Description:
    virtual CRhinoObject::ShortDescription override
  Returns:
    "cage" or "cages"
  */
  const wchar_t* ShortDescription( bool bPlural ) const override;

  class CRhWireframeDisplay* WireframeDisplay() const override;
};


class RHINO_SDK_CLASS CRhinoMorphControl : public CRhinoWireframeObject
{
  ON_OBJECT_DECLARE(CRhinoMorphControl);

public:
  CRhinoMorphControl();                              // assigns a new object UUID
  CRhinoMorphControl(const ON_3dmObjectAttributes&); // assigns a new object UUID
  ~CRhinoMorphControl();


  /*
  Description:
    Sets the object's m_control to be a copy of the input parameter.
  Parameters:
    control - [in] copied.
  Returns:
    True if input is valid.
  */
  bool SetControl( const ON_MorphControl& control );

  /*
  Description:
    Sets the object's m_control to be the input parameter.
    ~CRhinoMorphControl will delete the pointer.
  Parameters:
    control - [in]
  Returns:
    True if input is valid.
  */
  bool SetControl( ON_MorphControl* control );

  /*
  Description:
    Get a pointer to the object's NURBS control.  
  Returns:
    Pointer to the object's ON_NurbsControl.
  Remarks:
   ~CRhinoMorphControl deletes this pointer.
  */
  const ON_MorphControl* Control() const;

  /*
  Description:
    Override of CRhinoObject::ObjectType
  Returns:
    ON::morph_control_object
  */
  ON::object_type ObjectType() const override;

  // virtual CRhinoObject::EnableGrips override
  void EnableGrips( bool bGripsOn ) override;

  ON_Plane InitialObjectFrame(const CRhinoDoc&) const /*override*/;

  /*
  Description:
    virtual CRhinoObject::ShortDescription override
  Returns:
    "control" or "controls"
  */
  const wchar_t* ShortDescription( bool bPlural ) const override;

  // virtual CRhinoObject::GetSubObjects override
  int GetSubObjects(
          ON_SimpleArray<CRhinoObject*>& subObjects, 
          const CRhinoViewport* pViewport = nullptr
          ) const override;


  class CRhWireframeDisplay* WireframeDisplay() const override;
};

/*
Description:
  Sets up information that allows a control object to deform
  a captive.  The captive object cannot have grips on.
Parameters:
  control_object - [in]
  captive_object - [in]
Returns:
  True if capture is successful.
*/
RHINO_SDK_FUNCTION
bool RhinoCaptureObject( 
          CRhinoMorphControl* control_object, 
          CRhinoObject* captive_object 
          );

/*
Description:
  Turns on captive's grips. 
  1) The captive object must already be captured by RhinoCaptureObject()     
  2) The control object must have grips on.
  3) The captive object must not have grips on.
Parameters:
  control_object - [in]
  captive_object - [in]
Returns:
  Pointer to captive grips if successful.
*/
RHINO_SDK_FUNCTION
CRhinoObjectGrips* RhinoTurnOnCaptiveGrips( 
          const CRhinoMorphControl* control_object, 
          const CRhinoObject* captive_object
          );

/*
Description:
  Get a captive object's control object.
Parameters:
  captive_object - [in]
Returns:
  Point to control object if captive has one.
*/
RHINO_SDK_FUNCTION
CRhinoMorphControl* RhinoControlObject( const CRhinoObject* captive_object );

/*
Description:
  Get a list of captive objects.
Parameters:
  control_object - [in]
  captives - [out]
Returns:
  Number of objects appended to captives.
*/
RHINO_SDK_FUNCTION
int RhinoGetCaptiveObjects( 
          const CRhinoMorphControl* control_object,
          ON_SimpleArray<const CRhinoObject*>& captives 
          );
