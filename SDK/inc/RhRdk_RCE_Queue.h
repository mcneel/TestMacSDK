
#pragma once // Public SDK header

namespace RhRdk
{
	namespace Realtime
	{

class CDocumentLogic;
class CInstanceAncestry;

/** \class RhRdk::Realtime::ChangeQueue

The ChangeQueue is a centralized mechanism for handling changes in a model.

All geometry is represented to clients as mesh data, and block instances are realized and handled properly, along with all material, environment
and lighting capabilities.

Once a ChangeQueue is constructed the CreateWorld() function should be called once, on the main thread.

The ChangeQueue communicates through NotifyBeginUpdates, NotifyEndUpdates and NotifyDynamicUpdatesAreAvailable when changes to the model exist,
including after the call to CreateWorld. Once updates are completede (NotifyEndUpdates) the client can call Flush(true) to receive all the collected
changes. All changes are passed to the client through the several Apply* funtions the ChangeQueue class provides. The custom ChangeQueue
implementation should implement those Apply* fnuctions that provide the data of interest.

Notes on double-sided material support.

	When using customized display attributes that specify a custom back-face material, the CRhRdkMaterial that you will retrieve from 
	MaterialFromId will be an internal display material with the type ID exported as uuidRealtimeDisplayMaterialType.  To support double sided materials
	you will need to query this material for its two children, both of which will also be derived from CRhRdkMaterial.  The two child materials will be
	in the child slots named "front" and "back".

	To support only single sided materials (should your renderer not support double sided materials), simply call "FindChild("front")" on the display material.
	Otherwise, use FindChild("front") and FindChild("back") to retrieve both.
*/

class RHRDK_SDK ChangeQueue
{
public:
	/**
	Construct a ChangeQueue for given parameters
	\param doc is the Rhino document for which the ChangeQueue is generated.
	\param uuidPlugInId is the UUID for the plug-in
	\param view is the ON_3dmView for which this Changeueue tracks viewport manipulations
	\param pAttributes is the display attributes if any
	\param bRespectDisplayAttributes true if changes to display attributes should be tracked
	\param bNotifyChanges true if changes should be notified
	*/
	ChangeQueue(const CRhinoDoc& doc, const UUID& uuidPlugInId, const ON_3dmView& view, const CDisplayPipelineAttributes* pAttributes, bool bRespectDisplayAttributes, bool bNotifyChanges);
	/**
	ChangeQueue constructed for a preview rendering.
	*/
	ChangeQueue(const IRhRdkPreviewSceneServer& ss, const UUID& uuidPlugInId);
	/**
	Deconstruct the ChangeQueue, clean up.
	*/
	virtual ~ChangeQueue(void);

	ChangeQueue(const ChangeQueue&) = delete;
	const ChangeQueue& operator=(const ChangeQueue&) = delete;

	/** Call this to update your renderer world and flash out the change queue.o
	The "Apply" functions will be called if bApplyChanges is true in order with the queue data, and then the data will be deleted.
	Once function exits, none of the data you were passed in the Apply functions is available.

	You should not call functions in the Rhino document during the Apply functions unless you are rendering on the
	main thread.

	\param bApplyChanges true if you want the Apply functions to be called.
	*/
	virtual void Flush(bool bApplyChanges = true);

	/** Call this to force the entire model through the queue. */
	virtual void CreateWorld(bool bFlushWhenFinished = true);	
	
	//These functions will be called on the main thread when the document is being updated.
	//You will probably want to flag your renderer to stop when BeginUpdates is called, and restart when EndUpdates is called.
	/** This function is called when the ChangeQueue receives a new change.
	*/
	virtual void NotifyBeginUpdates() const;
	/** This function is called when the ChangeQueue has completed handling changes. */
	virtual void NotifyEndUpdates() const;

	/** This function is called when an immediate updates is required (view changes, dynamic object changes.  Respond as quickly as you can). */
	virtual void NotifyDynamicUpdatesAreAvailable(void) const;

	/** Returns true if the related viewport has changed. */
	virtual bool HasViewChanged(void);
	/** Returns true if the viewport with UUID has changed.
	\param viewportId the UUID of the viewport.
	*/
	virtual bool HasViewChanged(const ON_UUID& viewportId);
	/** Returns true if a mesh addition has been queued.
	\param id the UUID of the mesh.
	*/
	virtual int  IsMeshAdditionQueued(const UUID& id) const;

	//For all data types that pass a content id, you can get at the queue cached version of the content by calling these functions.
	//For back-face material support (when setting custom display attributes, see the note at the top of this file).
	/**
	Return the CRhRdkMaterial for given hash.
	\param id the hash of the material.
	\return \e CRhRdkMaterial if found, else \e nullptr.
	*/
	virtual const CRhRdkMaterial* MaterialFromId(ON__UINT32 id) const;

	/**
	Return the CRhRdkTexture for given hash.
	\param id the hash of the texture.
	\return \e CRhRdkTexture if found, else \e nullptr.
	*/
	virtual_su const CRhRdkTexture* TextureFromId(ON__UINT32 id) const;

	/**
	Return the CRhRdkEnvironment for given hash.
	\param id the hash of the environment.
	\return \e CRhRdkEnvironment if found, else \e nullptr.
	*/
	virtual const CRhRdkEnvironment* EnvironmentFromId(ON__UINT32 id) const;

	/**
	Utility function to convert the default light to a more useful world-based light.  Only use
	if your renderer doesn't support camera-based lighting.
	You should only call this during a flush, where a camera based light has been posted.
	\param light the light to convert.
	\param vp the viewport to use as camera.
	\return \e true if successfull, else \e false
	*/
	virtual bool ConvertCameraBasedLightToWorld(ON_Light& light, const ON_Viewport& vp);

	//When changes are made to the environment, or items that might require the environment, it is possible
	//to get at all of the environments for any usage channel during the flush.
	/**
	Get the hash for environment based on usage. Can be used with EnvironmentFromId.
	\param usage the usage type
	\return the hash
	*/
	virtual ON__UINT32 EnvironmentIdForUsage(IRhRdkCurrentEnvironment::Usage usage) const;

public:
	class Mesh;
	class DynamicObject;
	class MeshInstance;
	class Light;
	class Material;
	class GroundPlane;
	class Environment;
	class Skylight;
	class ClippingPlane;

	//Special accessfor functions that return the state of the document at the time of the flush.  Use to query for
	//information about related data (for example, you may want skylight information when running environment changes)

	//These should be replaced with std::shared_ptr
	/** Get the skylight from the queue.
	\return Skylight*, or nullptr if no skylight.
	*/
	virtual const Skylight*						QueueSkylight(void) const;
	/** Get the ground plane data from the queue.
	\return GroundPlane* or nullptr if no ground plane.
	*/
	virtual const GroundPlane*					QueueGroundPlane(void) const;
	/** Get the ON_3dmRenderSettings.
	\return ON_3dmRenderSettings* or nullptr.
	*/
	virtual const ON_3dmRenderSettings*			QueueRenderSettings(void) const;
	/** Get the sun from the queue.
	\return ON_Light* or nullptr
	*/
	virtual const ON_Light*						QueueSun(void) const;
	/** Get the view from the queue.
	\return ON_3dmView* or nullptr
	*/
	virtual const ON_3dmView*					QueueView(void) const;
	/** Get the bounding box of the scene from the queue.
	\return ON_BoundingBox&
	*/
	virtual const ON_BoundingBox&				QueueSceneBoundingBox(void) const;
	/** Get the display pipeline attributes from the queue.
	\return CDisplayPipelineAttributes* or nullptr
	*/
	const CDisplayPipelineAttributes*			QueueDisplayAttributes(void) const;

	/**
	Available during a flush.  Specifically to be used if your renderer doesn't support instancing,
	and you need access to the concrete meshes during the ApplyMeshInstanceChanges call.
	\param uuid the UUID of the mesh
	\return Mesh* the mesh for the UUID
	*/
	virtual const Mesh* MeshFromId(const ON_UUID& uuid) const;

	// Counts for meshes and instances currently considered "delivered" - updated during the flush.
	/** Count of meshes considered delivered for given UUID.
	\param id instance id
	\return int count of meshes
	*/
	virtual int MeshCountForId(const UUID& id) const;
	/** Count of mesh intansces for given base id.
	Since block instances can be nested you can specifiy from which object to count recursively.
	\param groupId the object instance hash  from which the mesh count is given (recursive).
	*/
	virtual int MeshInstancesCountForBaseId(ON__UINT32 groupId) const;

protected:
	//Implement these functions to update your renderer during a "Flush" operation.
	/**
		Apply changes to the view. The given view has the new state.
		\param view the new view status.
	*/
	virtual void ApplyViewChange(const ON_3dmView& view) const;
	/**
	Apply the dynamic object transformations as given in the simple array (while dragging).
	\param dynamicTransforms array of dynamic object transforms.
	*/
	virtual void ApplyDynamicObjectTransforms(const ON_SimpleArray<const DynamicObject*>& dynamicTransforms) const; 
	/** Apply the dynamic light changes (while dragging).
	\param lights array of changed lights.
	*/
	virtual void ApplyDynamicLightChanges(const ON_SimpleArray<const ON_Light*>& lights) const;
	/** Apply the given mesh changes.
	\param deleted array of UUIDs of deleted meshes
	\param addedOrChanged array of meshes that have been added or changed.
	*/
	virtual void ApplyMeshChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const Mesh*>& addedOrChanged) const;
	/** Apply the given mesh instance changes.
	\param deleted array of instance ids of deleted mesh instances
	\param addedOrChanged array of MeshInstance that have been added or changed
	*/
	virtual void ApplyMeshInstanceChanges(const ON_SimpleArray<ON__UINT32>& deleted, const ON_SimpleArray<const MeshInstance*>& addedOrChanged) const;
	/** Apply the given sun changes.
	\param sun an ON_Light representing the sun.
	*/
	virtual void ApplySunChanges(const ON_Light& sun) const;
	/** Apply the given skylight changes.
	\param skylight the Skylight with the new state
	*/
	virtual void ApplySkylightChanges(const Skylight& skylight) const;
	/** Apply the given light changes.
	\param lights array of \e Light* that have been added, deleted or changed.
	*/
	virtual void ApplyLightChanges(const ON_SimpleArray<const Light*>& lights) const;
	/** Apply the given material changes.
	Note that initial material information always comes through the \e Mesh*  in ApplyMeshChanges.
	\param materials array of \e Material* that have been changed.
	*/
	virtual void ApplyMaterialChanges(const ON_SimpleArray<const Material*>& materials) const;
	/** Apply the changes for the given environment channel. 
	\param usage the environment type that changed.
	*/
	virtual void ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage usage) const;
	/** Apply the given ground plane changes.
	\param gp the \e GroundPlane& with the new state.
	*/
	virtual void ApplyGroundPlaneChanges(const GroundPlane& gp) const;
	/** Apply the linear workflow changes.
	\param lw the IRhRdkLinearWorkflow&
	*/
	virtual void ApplyLinearWorkflowChanges(const IRhRdkLinearWorkflow& lw) const;
	/** Apply the given render settings changes.
	\param rs the ON_3dmRenderSettings& with the new state.
	*/
	virtual void ApplyRenderSettingsChanges(const ON_3dmRenderSettings& rs) const;
	/** Apply the clipping plane changes.
	\param deleted array of UUID of the deleted clipping planes
	\param addedOrModifiedClippingPlanes array of ClippingPlane* that are added or changed
	*/
	virtual void ApplyClippingPlaneChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const ClippingPlane*>& addedOrModifiedClippingPlanes) const;
	/** Apply the dynamic clipping plane changes.
	\param adpc array of ClippingPlane* for dynamic changes (while dragging).
	*/
	virtual void ApplyDynamicClippingPlaneChanges(const ON_SimpleArray<const ClippingPlane*>& adpc) const;

public:
	/** Implement to handle the locking of the view that is running your display mode. */
	virtual void Lock() const;
	/** Implement to handle the unlocking of the view that is running your display model. */
	virtual void Unlock() const;
	/** Implement to handle the locking of the view that is running your display mode. */
	virtual void Lock();
	/** Implement to handle the unlocking of the view that is running your display model. */
	virtual void Unlock();

	bool TryLock(void) const;
	bool TryLock(void);

protected:
	//[EVF] L"ApplyDisplayAttributesChanges" - const CDisplayPipelineAttributes*
	virtual void* EVF(const wchar_t*, void*);

public:
	/** \class RhRdk::Realtime::ChangeQueue::MappingChannel
	Container for texture mapping information on an object (RhRdk::Realtime::ChangeQueue::Mesh)
	*/
	class RHRDK_SDK MappingChannel
	{
	public:
		MappingChannel();
		MappingChannel(int, const ON_TextureMapping*, const ON_Xform*);
		virtual ~MappingChannel();

		MappingChannel(const MappingChannel&) = delete;
		const MappingChannel& operator=(const MappingChannel&) = delete;

		/** The channel of the texturemapping. */
		int Channel(void) const;
		/** The ON_TextureMapping . */
		const ON_TextureMapping& Mapping(void) const;
		/** The local transform. */
		const ON_Xform& XformLocal(void) const;

		virtual void* EVF(const wchar_t*, void*);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::MappingChannels.
	Collection of mapping channels on a mesh.
	*/
	class RHRDK_SDK MappingChannels
	{
	public:
		MappingChannels();
		MappingChannels(const ON_MappingRef*, const CRhinoTextureMappingTable*);
		virtual ~MappingChannels();

		MappingChannels(const MappingChannels&) = delete;
		const MappingChannels& operator=(const MappingChannels&) = delete;

		/** Use if you don't support multiple mapping channels. */
		const MappingChannel& SingleMapping() const;

		/** Use this to access the channel list. */
		const ON_SimpleArray<MappingChannel*>& Channels() const;

		virtual void* EVF(const wchar_t*, void*);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::Mesh
		The meshed version of a CRhinoObject*
	*/
	class RHRDK_SDK Mesh
	{
	public:
		/** Get the mesh parts that make up the object.
		\return array of ON_Mesh*
		*/
		const ON_SimpleArray<ON_Mesh*>& Meshes() const;
		/** The original object. 
		\retu3n the original CRhinoObject*
		*/
		const CRhinoObject* Object(void) const;
		/** Mesh id. */
		const ON_UUID& UuidId(void) const;
		/** Mapping channels for this object. */
		const MappingChannels* Mapping(void) const;

		/** Total vertex count in this mesh. */
		const int TotalVertexCount(void) const;
		/** Total face count in this mesh. */
		const int TotalFaceCount(void) const;

	public:
		Mesh(const ON_UUID& id, MappingChannels* pMapping, const ON_SimpleArray<const ON_Mesh*>& meshes, const CRhinoObject* pObject);
		Mesh(const ON_UUID& id);
		virtual ~Mesh();

		Mesh(const Mesh&) = delete;
		const Mesh& operator=(const Mesh&) = delete;

		virtual void* EVF(const wchar_t*, void*);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::MeshInstance */
	class RHRDK_SDK MeshInstance
	{
	public:
		RDK_DEPRECATED MeshInstance(const CInstanceAncestry& a, const ON_Xform& xformInstance, ON__UINT32 materialCacheCRC, bool receiveShadows, bool castShadows);
		MeshInstance(const CInstanceAncestry& a, const ON_Xform& xformInstance, ON__UINT32 materialCacheCRC, bool receiveShadows, bool castShadows, CRhRdkDecalIterator*);
		virtual ~MeshInstance();

		MeshInstance(const MeshInstance&) = delete;
		const MeshInstance& operator=(const MeshInstance&) = delete;

		/** The instance id that will hold the mesh. */
		ON__UINT32 InstanceId(void) const;
		/** The id of the mesh to bind to this mesh instance. */
		ON_UUID MeshId(void) const;
		/** The mesh index of the mesh, in case the mesh is a multipart mesh. */
		int MeshIndex(void) const;
		/** The transform of the instance. */
		ON_Xform InstanceXform(void) const;
		/** The material that is to be used for the mesh in this instance. */
		ON__UINT32 MaterialId(void) const;
		/** Returns true if the object should receive shadows. */
		bool ReceiveShadows(void) const;
		/** Returns true if the object should cast shadows. */
		bool CastShadows(void) const;
		ON__UINT32 GroupId(void) const;
		/** Returns ON_UUID of the root of this mesh instance if it has one. Or ON_nil_uuid if this item is the first in its ancestry. */
		ON_UUID RootId(void) const;
		/** Returns ON_UUID of the parent of this mesh instance if it has one. Or ON_nil_uuid if this item is the first in its ancestry. */
		ON_UUID ParentId(void) const;

		/** Returns a pointer to the decal set for this object. Valid while ApplyChanges is in progress. */
		const CRhRdkDecalIterator* DecalIterator(void) const;

		virtual void* EVF(const wchar_t*, void*);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::Light */
	class RHRDK_SDK Light
	{
	public:
		/** Get the actual light data. */
		const ON_Light& LightData() const;
		/** The event that gives this light data, it can be delete, undelete, add or change 
		\see CRhinoEventWatcher::light_event
		*/
		ON__UINT32 MaterialId(void) const;

		const CRhinoEventWatcher::light_event Event() const;

		Light(const Light&) = delete;
		const Light& operator=(const Light&) = delete;

	public:
		ON_DEPRECATED Light(const ON_Light&, CRhinoEventWatcher::light_event);
		Light(const ON_Light&, ON__UINT32 material_id, CRhinoEventWatcher::light_event);
		
		virtual ~Light(void);

		virtual void* EVF(const wchar_t*, void*);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRDk::Realtime::ChangeQueue::DynamicObject.
	Holds the mesh instance that is being transformed during a drag.
	*/
	class RHRDK_SDK DynamicObject
	{
	public:
		/** mesh instance being transformed during a drag. */
		ON__UINT32 MeshInstanceId(void) const;
		/** the new transform. */
		const ON_Xform& Xform(void) const;

		virtual void* EVF(const wchar_t*, void*);

	public:
		DynamicObject(ON__UINT32 id, const ON_Xform& x);
		virtual ~DynamicObject();

		DynamicObject(const DynamicObject&) = delete;
		const DynamicObject& operator=(const DynamicObject&) = delete;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::Material. */
	class RHRDK_SDK Material
	{
	public:
		/** mesh instance for this material. */
		ON__UINT32 MeshInstanceId(void) const;
		/** the material hash. */
		ON__UINT32 MaterialId(void) const;
		/** mesh index on the mesh. */
		int MeshIndex(void) const;

		virtual void* EVF(const wchar_t*, void*);

	public:
		Material(ON__UINT32 iid, int midx, ON__UINT32 mid);
		virtual ~Material();

		Material(const Material&) = delete;
		const Material& operator=(const Material&) = delete;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::GroundPlane. */
	class RHRDK_SDK GroundPlane
	{
	public:
		/** Material hash  id used for this ground plane. */
		ON__UINT32 MaterialId(void) const;
		/** texture scale. */
		const ON_2dVector& TextureScale(void) const;
		/** texture offset. */
		const ON_2dVector& TextureOffset(void) const;
		/** Altitude of the ground plane in the world*/
		double Altitude(void) const;
		/** texture rotation. */
		double TextureRotation(void) const;
		/** true if the groundplane is enabled. */
		bool Enabled(void) const;
		/** true if should show through the underside. */
		bool ShowUnderside(void) const;
		/** true if groundplane is shadows only. */
		bool ShadowOnly(void) const;

		/** CRC of the groundplane for computed over all visible style affecting parameters. */
		ON__UINT32 CRC(void) const;

		virtual void* EVF(const wchar_t*, void*);

	public:
		GroundPlane(ON__UINT32 materialId, const ON_2dVector& vTextureScale, const ON_2dVector& vTextureOffset, double alt, double rot, bool on, bool showUnder, bool bShadowOnly);
		virtual ~GroundPlane();

		GroundPlane(const GroundPlane&) = delete;
		const GroundPlane& operator=(const GroundPlane&) = delete;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::Skylight */
	class RHRDK_SDK Skylight
	{
	public:
		/** true if skylight is active. */
		bool On(void) const;
		/** the shadow intensity for skylight. */
		double ShadowIntensity(void) const;
		/** true if skylight uses a custom environment. */
		bool CustomEnvironmentOn(void) const;

		virtual void* EVF(const wchar_t*, void*);

	public:
		Skylight(const IRhRdkSkylight& sl);
		Skylight(bool bOn, double dIntensity, bool bCustomEnvironment);
		virtual ~Skylight();

		Skylight(const Skylight&) = delete;
		const Skylight& operator=(const Skylight&) = delete;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

	/** \class RhRdk::Realtime::ChangeQueue::ClippingPlane. */
	class RHRDK_SDK ClippingPlane
	{
	public:
		/** Get the ON_ClippingPlane& */
		const ON_ClippingPlane& ClippingPlaneData() const;
		/** Get the CRhinoClippingPlaneObject*. (useful in case you have user data attached to it. )*/
		const CRhinoClippingPlaneObject* Object(void) const;

		ClippingPlane(const ClippingPlane&) = delete;
		const ClippingPlane& operator=(const ClippingPlane&) = delete;

	public:
		ClippingPlane(const ON_ClippingPlane& cp, const CRhinoClippingPlaneObject* po);
		virtual ~ClippingPlane(void);

		virtual void* EVF(const wchar_t*, void*);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		friend class Impl;
	};

public:
	class Impl;
	class PreviewImpl;
	class DocumentImpl;

protected:
	Impl* m_pImpl;

public:
	//Helpers
	ON__UINT32 AddMaterialReference(const CRhRdkMaterial& material);
	RDK_DEPRECATED ON__UINT32 AddMaterialReference(const ON_Material& pMaterial);
	ON__UINT32 AddMaterialReference(const CRhinoDoc* pDocAssoc, const ON_Material& pMaterial);
	ON__UINT32 AddContentReference(const CRhRdkContent& content);
	
public:
	//Customization
	/** Override if you want a custom application order. */
	virtual void ApplyChanges(void) const;

	//Deprecated - use NeedsBakingFor to specify which effects you want baked.
	/** Return bool if you need any of the mesh effects baked. deprecated. */
	virtual bool NeedsBakingOnAnyChannel(const CRhinoObject&, const CInstanceAncestry* pOptionalAncestry = nullptr, const CRhRdkMaterial* pOptionalMaterial = nullptr) const;

	//Default returns eBakingFunctions::kAll
	virtual eRhRdkBakingFunctions BakeFor(void) const;

	//Determine if views are considered equal - in other words, whether a specific different in views will trigger a view update.
	virtual bool AreViewsEqual(const ON_3dmView& oldView, const ON_3dmView& newView) const;

	//Default returns 2048
	virtual int BakingSize(const CRhinoObject& object, const CRhRdkMaterial& material, ON_Texture::TYPE channel) const;

	//Default is to return false.  Override and return true if you want the ApplyMeshChanges function to store a duplicate of the
	//original CRhinoObject.  This can take additional time and memory, but can be used to support NURBS returning or your own object userdata.

	/** Return true if you need the original geometry objects (mesh, light, clippingplane).*/
	virtual bool ProvideOriginalObject(void) const;
	/** Return true if you want the meshes aggregated for given object. */
	virtual bool ShouldAggregateMeshes(const CRhinoObject& obj) const;

	/** Ignore given object. */
	static void SetIgnoredObject(CRhinoObject&);
	/** true if given object is ignored. */
	static bool IsIgnoredObject(const CRhinoObject&);

protected:
	//Internal functions to set up the queue.
	//None of these functions should take a CRhinoDoc - they should all accept the information directly.
	virtual void PostViewChange(const ON_3dmView& v);
	RDK_DEPRECATED virtual void PostLightChange(const UUID& id, CRhinoEventWatcher::light_event ev, const ON_Light& light);
	virtual void PostMeshAdded(const UUID& id, const ON_SimpleArray<const ON_Mesh*>& incoming_meshes, const ON_MappingRef* pMappingRef, const CRhinoObject* pObject, const CRhinoTextureMappingTable* pTMT);
	virtual void PostMeshAdded(const UUID& id, const ON_Mesh& mesh, int iMeshIndex);
	virtual void PostMeshDeleted(const UUID& id);
	RDK_DEPRECATED virtual void PostMeshInstanceAdded(const CInstanceAncestry& uuidHistory, const ON__UINT32& materialId, const ON_Xform& instanceXForm, bool bCastShadows, bool bReceiveShadows);
	virtual void PostMeshInstanceDeleted(const CInstanceAncestry& uuidHistory);
	virtual void PostDynamicObjectChange(const CInstanceAncestry& uuidHistory, const ON_Xform& xform);
	virtual void PostDynamicLightChange(const CRhinoLight& light);
	virtual void PostSkylightChange(const IRhRdkSkylight& sl);
	virtual void PostSunChange(const IRhRdkSun& sun);
	virtual void PostMaterialChange(const CInstanceAncestry& uuidHistory, const ON__UINT32& dwdMaterialCRC);
	virtual void PostRenderSettingsChange(const ON_3dmRenderSettings& rs);
	virtual void PostGroundPlaneChange(const IRhRdkGroundPlane& gp);
	virtual void PostLinearWorkflowChange(const IRhRdkLinearWorkflow& lw);
	virtual void PostAddClippingPlane(const CRhinoClippingPlaneObject& cp);
	virtual void PostDeleteClippingPlane(const CRhinoClippingPlaneObject& cp);
	virtual void PostDynamicClippingPlaneChange(const CRhinoClippingPlaneObject& cpo);
	virtual void PostEnvironmentChange(IRhRdkCurrentEnvironment::Usage usage, const CRhRdkEnvironment* pEnv);
	virtual void SetSceneBoundingBox(const ON_BoundingBox& bb);
	virtual_su void PostDisplayAttributesChange(const CDisplayPipelineAttributes& da);
	RDK_DEPRECATED void PostMeshInstanceAdded(const CInstanceAncestry& uuidHistory, const ON__UINT32& materialId, const ON_Xform& instanceXForm, const ON_Xform& ocsXform, bool bCastShadows, bool bReceiveShadows);
	virtual_su void PostMeshInstanceAdded(const CInstanceAncestry& uuidHistory, const ON__UINT32& materialId, const ON_Xform& instanceXForm, const ON_Xform& ocsXform, bool bCastShadows, bool bReceiveShadows, CRhRdkDecalIterator*);
	virtual_su void PostLightChange(const UUID& id, CRhinoEventWatcher::light_event ev, const ON_Light& light, const ON__UINT32& dwdMaterialCRC);

	friend class QueuePoster;

public:
	ON_DEPRECATED_MSG("Use the ctor that takes display attributes.") ChangeQueue(const CRhinoDoc& doc, const UUID& uuidPlugInId, const ON_3dmView& view, bool bRespectDisplayAttributes = false);
};

}
}
