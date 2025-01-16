
#pragma once

#include "RhRdkDefinitions.h"
#include "RhRdkExtension.h"
#include "RhRdkUuids.h"
#include "RhRdkPlugIn.h"

// Support for "third party" lights in the light manager.  These functions are called by the light manager panel.  You can
// override them to modify the behaviour of the light manager by providing additional lights, or vetoing the listing of
// existing Rhino lights.  You can also modify the command behaviour, including the edit, on-off and intensity/color controls.
class RHRDK_SDK CRhRdkLightManagerSupport : public CRhRdkExtension, public CRhinoEventWatcher
{
public:
	CRhRdkLightManagerSupport();
	virtual ~CRhRdkLightManagerSupport();

	virtual UUID RenderEngineId(void) const = 0;

	//Determines if this object is the currently active light manager support object
	bool IsActive(void) const;

	virtual UUID InterfaceId(void) const override;

	/** Return the light list - default behaviour is to retrieve the lights from CRhinoDoc::m_light_table.  Custom lights should have unique m_light_id. */
	virtual void GetLights(const CRhinoDoc& doc, ON_ObjectArray<ON_Light>& lights) const;
	
	/* Create a new group containing the lights */
	virtual void GroupLights(CRhinoDoc& doc, ON_ObjectArray<ON_Light>& lights) const;

	/* UnGroup the lights */
	virtual void UnGroup(CRhinoDoc& doc, ON_ObjectArray<ON_Light>& lights) const;

	//
	//Conversions between CRhinoLight and ON_Light (and lookup from m_light_id)
	//

	/** Default implementation searches the CRhinoDoc::m_light_table */
	virtual bool LightFromId(const CRhinoDoc& doc, const ON_UUID& uuid, ON_Light& light) const;

	/** Default implementation searchs the CRhinoDoc::m_light_table and compares ON_Light::m_light_id */
	virtual unsigned int ObjectSerialNumberFromLight(const CRhinoDoc& doc, const ON_Light& light) const;

	/** The default implementation is to check if the object is a CRhinoLight, and if it is, return the ON_Light. */
	virtual bool LightFromObject(const CRhinoObject& object, ON_Light& light) const;	

	//Queries
	virtual ON_wString LightDescription(const CRhinoDoc& doc, const ON_Light& light) const;

	//Return true to have the framework call ModifyLight
	virtual bool OnEditLight(CRhinoDoc& doc, ON_ObjectArray<ON_Light>& light) const;
	
	//Operations
	virtual bool DeleteLight(CRhinoDoc& doc, const ON_Light& light, bool bUndelete = false) const;
	virtual void ModifyLight(CRhinoDoc& doc, const ON_Light& light) const;

	virtual bool SaveToSoloStorage(CRhinoDoc& doc, const ON_UUID& lightId) const;	//Returns false if the light was already in solo storage, otherwise saves the on-ness of the light to solo-storage.
	virtual bool IsInSoloStorage(const CRhinoDoc& doc, const ON_UUID& lightId) const;		//Returns true if the light is in solo storage
	virtual bool RestoreFromSolo(CRhinoDoc& doc, const ON_UUID& lightId) const;		//Restores the light's on-ness from solo storage.  Returns false if the light was not in solo storage.

	//The default implementation raises the CRhinoEventWatcher::OnLightTableEvent
	virtual void OnCustomLightEvent(const CRhinoDoc& doc, CRhinoEventWatcher::light_event, const ON_Light& light) const;

	virtual void* EVF(const wchar_t*, void*) override;

protected:
	//The default implementations of these functions determine if the object that is modified is a light (using LightFromObject), and if so,
	//calls OnCustomLightChanged.  If the object is already a CRhinoLight, nothing is done
	virtual void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object) override;
	virtual void OnObjectReplaced(unsigned int rhino_doc_serial_number, unsigned int deleted_object_sn, unsigned int added_object_sn) override;
	virtual void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes) override;
	virtual void OnAddObject(CRhinoDoc& doc, CRhinoObject& object) override;
	virtual void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
	virtual void OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
	virtual const CRhinoObject* ObjectFromLight(const CRhinoDoc & doc, const ON_Light & light) const;

	virtual void LightTableEvent(CRhinoEventWatcher::light_event event, const CRhinoLightTable& light_table, int light_index, const ON_Light* old_settings) override;

private:
	bool m_bIsReplacing = false;

public:
	//Solo helper functions
	//First checks to see if we are in "solo mode" - which means that there are any lights that respond "true" to IsInSoloStorage.
	//If in solo mode:
	//	If bSolo = true
	//		Sets this light on.
	//  If bSolo = false
	//		If this is the last light "on", forces all lights out of solo mode.
	//		If there are other lights on, turns this light off.
	//If not in solo mode:
	//	If bSolo = true
	//		Forces all lights into solo storage and sets this light on.
	//  If bSolo = false
	//		This shouldn't happen.  Will cause an ASSERT
	virtual bool SetLightSolo(CRhinoDoc& doc, const ON_UUID& id, bool bSolo) const;

	//Returns the value of "ON_LIght::m_bOn" if the light is in solo storage, or false if not in solo storage (ie - this is the checkbox state on the light manager dialog)
	virtual bool GetLightSolo(CRhinoDoc& doc, const ON_UUID& id) const;

	//Returns the number of lights in solo storage - any number other than 0 means "in solo mode"
	virtual int  LightsInSoloStorage(const CRhinoDoc& doc) const;
};

class CRhRdkLightManagerSupport_BuiltIn : public CRhRdkLightManagerSupport
{
	virtual UUID PlugInId(void) const override;
	virtual UUID RenderEngineId(void) const override { return uuidUniversalRenderEngine; }
};


//Get the light manager support for a specific plug-in.  If the plug-in has no specific support
//class registered as an extension, then the built in functionality will be used.
RHRDK_SDK const CRhRdkLightManagerSupport* RhRdkLightManagerSupport(const UUID& uuidRenderPlugInId = RhinoApp().GetDefaultRenderApp());
