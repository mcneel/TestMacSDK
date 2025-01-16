
#pragma once // Public SDK header

#include "RhRdkObject.h"
#include "IRhRdkDecal.h"

class CRhRdkMaterial;

/** \class CRhRdkObjectAncestry

	This class describes the parent chain for an object so that the "By Parent" material source can be properly implemented.
	When a reference to this class is required, and the object is inside a block or worksession, supply the full parent chain starting
	from the object that is in the document itself (usually the block reference), followed by the nested block reference or concrete
	geometry object...until you reach the object itself.

*/
class RHRDK_SDK CRhRdkObjectAncestry : public CRhRdkObject
{
public:
	CRhRdkObjectAncestry();
	CRhRdkObjectAncestry(const CRhRdkObjectAncestry& src);
	CRhRdkObjectAncestry(const CRhRdkObjectAncestry* pSrc);
	CRhRdkObjectAncestry(const ON_SimpleArray<const CRhinoInstanceObject*>& a);
	virtual ~CRhRdkObjectAncestry();

	static const CRhRdkObjectAncestry empty;

	const CRhRdkObjectAncestry& operator=(const CRhRdkObjectAncestry&);

	const CRhRdkObjectAncestry& operator += (const CRhinoInstanceObject* pObject);

	const CRhinoInstanceObject* TopLevelParent(void) const;

	bool IsTopLevel(void) const;

	const ON_SimpleArray<const CRhinoInstanceObject*>& Ancestry(void) const;

	ON_Xform CompleteTransform(void) const;

private:
	ON_SimpleArray<const CRhinoInstanceObject*> m_Objects;
	void* m_pReserved;
};

/** \class CRhRdkDecalIterator

	This class provides a means of iterating over decals. An instance of this object can be obtained
	by calling CRhRdkObjectDataAccess::NewDecalIterator(). The implementation of this class is thread-safe
	so that it can be used from multiple threads during rendering.

*/
class RHRDK_SDK CRhRdkDecalIterator : public CRhRdkObject
{
public:
	virtual ~CRhRdkDecalIterator() { }

	/** Reset the iterator to get the first decal again. */
	virtual void Reset(void) = 0;

	/** Retrieve the next decal.
		\return A pointer to the next decal or null if no more decals. */
	virtual const IRhRdkDecal* NextDecal(void) = 0;

	/** \return A CRC representing the combined, order-dependent CRC of this iterator's decals. */
	virtual RDK_DECAL_CRC CRC(void) const = 0;
};

/** \class CRhRdkObjectDataAccess

	This class provides a means of accessing data associated with a Rhino object or layer.
*/
class RHRDK_SDK CRhRdkObjectDataAccess
{
public:
	virtual ~CRhRdkObjectDataAccess();

	/** Construct to access data of Rhino object by object uuid. */
	CRhRdkObjectDataAccess(const CRhinoDoc& doc, const UUID& uuidRhinoObject);

	/** Deprecated; not document-aware. */
	RDK_DEPRECATED CRhRdkObjectDataAccess(const UUID& uuidRhinoObject);

	/** Construct to access data of Rhino object by object pointer. */
	CRhRdkObjectDataAccess(const CRhinoObject* pRhinoObject);

	/** Construct to access data of Rhino layer by layer pointer. */
	CRhRdkObjectDataAccess(const CRhinoLayer* pRhinoLayer);

	//-------- Material. Works only for object.
	
	/** \return the material associated with this object, depending on the material source.
		\note Currently, 'By Parent' is not implemented. */
	virtual UUID ObjectMaterial(const CRhRdkObjectAncestry*) const;

	//-------- Material. Works for object or layer.

	/** \return Instance id of RDK material associated with the object. 
		Internal use - consider using ObjectMaterial. */
	virtual UUID MaterialInstanceId(void) const;

	/** Set an RDK material to be associated with the object or layer.
		\param uuidMaterialInstance is the instance id of the material.
		\return \e true if successful, else \e false. */
	virtual bool SetMaterialInstanceId(const UUID& uuidMaterialInstance) const;

	/** Set an RDK material to be associated with the object or layer.
		\param pMaterial is a pointer to the material.
		\return \e true if successful, else \e false. */
	virtual bool SetMaterialInstanceId(const CRhRdkMaterial* pMaterial) const;

	/** Set an RDK material to be associated with the object.
		\param pMaterial is a pointer to the material.
		\param bClearPerFace if \e true will cause per-face materials to be cleared.
		 Otherwise they will be retained. Only makes sense for objects, not layers.
		\return \e true if successful, else \e false. */
	bool SetMaterialInstanceId(const CRhRdkMaterial* pMaterial, bool bClearPerFace) const;

	/** By default, content instance ids are verified as existing in the document.
		In certain cases this verification may not be required. It can be disabled
		and re-enabled by calling this method. */
	void SetVerifyInstanceId(bool b);

	//-------- Mapping.

	/** Get an array of used mapping channel ids.
		\param arrayOut is filled with 1-based channel identifiers.
		\return \e true if successful, else \e false. */
	virtual bool GetUsedMappingChannels(ON_SimpleArray<int>& arrayOut) const;

	/** Get the mapping and local xform for a channel.
		\param iChannelId is the channel identifier (1-based).
		\param mappingOut receives a copy of the mapping.
		\param xformLocalOut receives the local transform associated with the channel.
		\return \e true if successful, else \e false. */
	virtual bool Mapping(int iChannelId, ON_TextureMapping& mappingOut, ON_Xform& xformLocalOut) const;

	//-------- Decals. Works only for object.

	/** \return An iterator for accessing decals or null if this object is not connected to a valid Rhino object.
		\note This is designed to be called by renderers so it only returns visible decals.
		The caller shall delete the iterator. */
	virtual CRhRdkDecalIterator* NewDecalIterator(const UUID& uuidRenderEngine = RhinoApp().GetDefaultRenderApp()) const;

	/** Remove all decals from the object.
		\return \e true if this object is connected to a valid Rhino object, else \e false. */
	virtual bool RemoveAllDecals(void) const;

	/** Remove the specified decal from the object.
		\return \e true if the decal was removed, \e false if the decal does not exist,
		 or if this object is not connected to a valid Rhino object. */
	virtual bool RemoveDecal(RDK_DECAL_CRC id) const;

	/** Add a new decal to the object.
		\return RDK_NIL_DECAL_CRC if the decal was not added, or any other valid crc if successful.
		\note Your implementation of IRhRdkDecal does not need to support CRC() or Color(). The data you
		 supply will be copied from your object and stored in the RDK's own implementation of IRhRdkDecal.
		 You will be passed a valid decal crc that you can use to find that decal later. */
	RDK_DECAL_CRC AddDecal(const IRhRdkDecal& decal) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

// The following global functions were requested by John Morse to allow decal access to raw object attributes.

/** Add decal information to object attributes. */
RHRDK_SDK RDK_DECAL_CRC RhRdkAddDecalToObjectAttributes(ON_3dmObjectAttributes& attr, const IRhRdkDecal& d);

/** Remove decal information from object attributes by its decal crc. */
RHRDK_SDK bool RhRdkRemoveDecalFromObjectAttributes(ON_3dmObjectAttributes& attr, RDK_DECAL_CRC decalCRC);

/** Remove all decal information from object attributes. */
RHRDK_SDK void RhRdkRemoveAllDecalsFromObjectAttributes(ON_3dmObjectAttributes& attr);

/** Find decal information on object attributes by its decal crc.
	\return A pointer to a decal if found, else null. The caller shall delete the decal after use. */
RHRDK_SDK IRhRdkDecal* RhRdkFindDecalOnObjectAttributes(ON_3dmObjectAttributes& attr, RDK_DECAL_CRC decalCRC);

/** \return An iterator for accessing decals on object attributes. The caller shall delete the iterator.
	\note The returned iterator is not thread safe. Any decals it provides will be owned by the caller
	 and must be deleted after use. Note that this is different to when you use the object data access
	 version of the iterator which is thread-safe and does not pass decal ownership to you. */
RHRDK_SDK CRhRdkDecalIterator* RhRdkNewDecalIteratorForObjectAttributes(const ON_3dmObjectAttributes& attr);
