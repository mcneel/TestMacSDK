
#pragma once

#include "RhRdkDefinitions.h"
#include "RhRdkObject.h"
#include "RhRdkExtension.h"
#include "RhRdkVariant.h"

class CRhRdkMaterial;
class CRhRdkObjectAncestry;
class CRhRdkCustomRenderMeshProvider;

namespace RhRdk
{
	/// <summary>
	/// The CustomRenderMeshes namespace includes everything needed to use the new custom render primitives subsystem starting
	/// with Rhino 8.  This replaces the deprecated code in IRhRdkCustomRenderMeshManager.h
	/// </summary>
	namespace CustomRenderMeshes
	{
		/// <summary>
		/// An ObjectId is either the UUID of a source object for which the standard render primitives are being replaced, or a non-object id
		/// which identifies another entity supplying custom renderer primitives and which should be rendered into the model as if it were an 
		/// object.  An example of a non-object id is a Grasshopper component ID.  A renderer will query the IManager for a list of non-object ids
		/// and call Primitves with each to get the complete set of meshes for the model (in addition to calling Primitives for each object in the scene)
		/// </summary>
		using ObjectId = ON_UUID;

		/// <summary>
		/// Each plug-in mesh provider as a unique MeshProviderId identifier.  The built in identifiers are listed in RhRdkUuids.h
		/// </summary>
		using MeshProviderId = ON_UUID;

		/// <summary>
		/// IRenderMeshes is a collection of geometry instances for a given ObjectId typically returned by the Custom Render Mesh system.
		/// It provides STL compatible iterators.
		/// As each set of primitives is returned by the PrimitiveProviders in turn, the running hash is updated with new information
		/// about the mesh modifications that have been made along the way.
		/// </summary>
		class RHRDK_SDK IRenderMeshes
		{
		public:
			/// <summary>
			/// Create a new render primitives collection.
			/// </summary>
			/// <param name="doc">The Rhino document with which this collection will be associated.</param>
			/// <param name="objectId">The ObjectId that this collection is for.</param>
			/// <param name="providerId">The MeshProvider that created this collection - if this is the first in the stack, typically this will be ON_nil_uuid.</param>
			/// <param name="runningHash">The running hash of this collection - each MeshProvider will update the hash with its own modification hash.</param>
			/// <returns></returns>
			ON_DEPRECATED_MSG("use the one below - adds flags")
			static std::shared_ptr<IRenderMeshes> New(
				const CRhinoDoc& doc, 
				const ObjectId& objectId, 
				const MeshProviderId& providerId,
				ON__UINT32 runningHash);

			static std::shared_ptr<IRenderMeshes> New(
				const CRhinoDoc& doc,
				const ObjectId& objectId,
				const MeshProviderId& providerId,
				ON__UINT32 runningHash,
				ON__UINT32 flags);

			virtual ~IRenderMeshes() { }

			/// <summary>
			/// The MeshProvider that created this collection - if this is the first in the stack, typically this will be ON_nil_uuid.
			/// </summary>
			/// <returns>The MeshProvider that created this collection - if this is the first in the stack, typically this will be ON_nil_uuid.</returns>
			virtual const MeshProviderId& ProviderId(void) const = 0;

			/// <summary>
			/// A handy helper class for implementing caching in MeshProviders.  The ProviderTracking class is a collection of per-document ITracker objects.
			/// Embed a ProviderTracking object in your MeshProvider to implement a simple cache.
			/// </summary>
			class RHRDK_SDK ProviderTracking final
			{
			public:
				ProviderTracking();
				~ProviderTracking();

				/// <summary>
				/// The per-document primitive cache for MeshProviders.  ITracker is basically a hash map of shared_ptr<const IRenderMeshes>
				/// with the key being the ObjectId.
				/// </summary>
				class RHRDK_SDK ITracker
				{
				public:
					virtual ~ITracker() {}

					/// <summary>
					/// Each tracker record is basically the entry in the map - so it's an IRenderMeshes object
					/// </summary>
					class RHRDK_SDK IRecord
					{
					public:
						virtual ~IRecord() {}

						/// <summary>
						/// Quick access to the hash for the primitives.
						/// </summary>
						/// <returns>The render hash</returns>
						virtual ON__UINT32 Hash(void) const = 0;

						/// <summary>
						/// A shared pointer to the IRenderMeshes stored in the map
						/// </summary>
						/// <returns>A shared pointer to the IRenderMeshes stored in the map</returns>
						ON_DEPRECATED_MSG("Use Primitives2")
						virtual std::shared_ptr<const IRenderMeshes> Primitives(void) const = 0;

						/// <summary>
						/// Emergency virtual function for future expansion
						/// </summary>
						/// <param name="wszFunc">The named function</param>
						/// <param name="pv">Private data</param>
						/// <returns>Private data in the form of a void*</returns>
						virtual void* EVF(const wchar_t* wszFunc, void* pv) = 0;
					};

					class RHRDK_SDK IRecord2 : public IRecord
					{
					public:
						virtual ~IRecord2() {}

						virtual bool RecordExists(ON__UINT32 flag_value) const = 0;

						virtual std::shared_ptr<const IRenderMeshes> Primitives2(ON__UINT32 flag_value) const = 0;

						virtual void AddPrimitives(ON__UINT32 flag_value, std::shared_ptr<const IRenderMeshes>) = 0;

						virtual void TransformAndSetNewHash(ON__UINT32 new_hash, const ON_Xform& transform) = 0;
					};

					/// <summary>
					/// Access the record for a specific ObjectId
					/// </summary>
					/// <param name="id">The ObjectId for these primitives.</param>
					/// <returns>The record in the cache containing the primitives.</returns>
					virtual const IRecord* Record(const ObjectId& id) const = 0;

					const IRecord2* Record2(const ObjectId& id) const; //[SDK_FREEZE]

					/// <summary>
					/// Adds a new set of primitives to the map.
					/// </summary>
					/// <param name="prims">The primitives to be added.  The prims must return a correct ObjectId and Hash for the cache to work correctly.</param>
					virtual void Set(std::shared_ptr<const IRenderMeshes> prims) = 0;

					//Named this way to enable the use of std:lock_guard.  Conforms to BasicLockable spec.
					/*virtual*/ void lock(void) const;	//Calls EVF(L"lock", nullptr);
					/*virtual*/ void unlock(void) const;  //Calls EVF(L"unlock", nullptr);

					/// <summary>
					/// Emergency virtual function for future expansion
					/// </summary>
					/// <param name="wszFunc">The named function</param>
					/// <param name="pv">Private data</param>
					/// <returns>Private data in the form of a void*</returns>
					virtual void* EVF(const wchar_t* wszFunc, void* pv) = 0;
				};

				/// <summary>
				/// Returns the document specific tracker for this cache object.
				/// </summary>
				/// <param name="doc"></param>
				/// <returns></returns>
				ITracker& Tracker(const CRhinoDoc& doc);
				ITracker& Tracker(unsigned int doc_serial);

			private:
				class Impl;
				std::unique_ptr<Impl> m_pImpl;
			};

			/// <summary>
			/// Replacement for ON_MappingChannel that actually carries the ON_TextureMapping around with it
			/// instead of storing it in a document table - meaning that it can be passed off to non-document
			/// threads and still access the data correctly.
			/// </summary>
			class IMappingChannel
			{
			public:
				virtual ~IMappingChannel() {}

				/// <summary>
				/// Corresponds to ON_MappingChannel::m_mapping_channel_id
				/// </summary>
				/// <returns>Channel number > 0 and <= 2147483647 (0x7FFFFFFF)</returns>
				virtual int Id(void) const = 0;

				/// <summary>
				/// The local mapping transform for this particular object - corresponds to ON_MappingChannel::m_object_xform
				/// The default value of m_object_xform is the identity. When an object that uses this mapping is transformed by "T", 
				/// m_object_xform is updated using the formula m_object_xform = T*m_object_xform. If texture coordinates are lost and 
				/// need to be recalculated and m_object_xform is not the identity, then m_object_xform should be passed to ON_TextureMapping::Evaluate() 
				/// as the mesh_xform parameter. When validating mapping coordinates, m_object_xform itself be passed to HasMatchingTextureCoordinates() 
				/// as the object_transform parameter.
				/// </summary>
				/// <returns>A transform</returns>
				virtual const ON_Xform& LocalXform(void) const = 0;

				/// <summary>
				/// The full mapping object that would usually be stored in the document mapping table.  See ON_TextureMapping
				/// </summary>
				/// <returns>The texture mapping</returns>
				virtual const ON_TextureMapping& Mapping(void) const = 0;

				/// <summary>
				/// Transforms the mapping channel
				/// </summary>
				/// <param name="xform"></param>
				virtual void Transform(const ON_Xform& xform) = 0;

				/// <summary>
				/// Deep copy
				/// </summary>
				/// <returns>A copy of the channel</returns>
				virtual std::shared_ptr<IMappingChannel> MakeCopy(void) const = 0;

				/// <summary>
				/// Create a new mapping channel
				/// </summary>
				/// <param name="channel">See IMappingChannel::</param>
				/// <param name="mapping">See IMappingChannel::Mapping</param>
				/// <param name="localXform">See IMappingChannel::LocalXform</param>
				/// <returns></returns>
				static std::shared_ptr<IMappingChannel> New(int channel, const ON_TextureMapping& mapping, const ON_Xform& localXform);
			};

			/// <summary>
			/// Defines a collection of mapping channels that will be carried around by instances or meshes.
			/// </summary>
			using MappingChannels = std::vector<std::shared_ptr<IMappingChannel>>;

			/// <summary>
			/// The core of the custom render mesh delivery system - and instance defines a single mesh or other primitive (sphere, plane, box or cone).
			/// Each instance has a shared mesh/primitive and a transform, along with a material, mapping channels (at the mesh and instance level).
			/// </summary>
			class RHRDK_SDK IInstance
			{
			public:
				virtual ~IInstance(void) {}

				/// <summary>
				/// Returns the object id this instance was generated from.
				/// </summary>
				/// <returns>The object id this instance was generated from.</returns>
				virtual ObjectId Object(void) const = 0;

				/// <summary>
				/// The IGeometry is class that collects all of the pure geometry information together for an instance.
				/// Essentially - the mesh/primitive along with the caching information for the display and an Id to differentiate meshes from each other
				/// without having to check their CRC.
				/// </summary>
				class IGeometry
				{
				public:
					virtual ~IGeometry(void) {}

					/// <summary>
					/// Returns the mesh associated with this geometry.
					/// </summary>
					/// <returns>A shared pointer to the mesh.</returns>
					virtual std::shared_ptr<const ON_Mesh> Mesh(void) const = 0;


					/// <summary>
					/// The cache handle for the mesh - used by the display to manage the VBOs.
					/// </summary>
					/// <param name=""></param>
					/// <returns></returns>
					virtual std::shared_ptr<CRhinoCacheHandle> CacheHandle(void) const = 0;

					/// <summary>
					/// An ID which uniquely identifies a shared mesh.  If the ID is the same for two separate
					/// geometry objects, the mesh or primitive produced by them will be considered the same - unless the ID is ON_nil_uuid
					/// in which case each one will be treated as though not shared.
					/// </summary>
					/// <param name=""></param>
					/// <returns></returns>
					virtual ON_UUID CacheHandleId(void) const = 0;

					/// <summary>
					/// Per mesh mapping channels.  If not supplied, then assume a single surface mapping channel.  May be overridden by the
					/// per-instance mapping channels.
					/// </summary>
					/// <returns>The mapping channels for this mesh.</returns>
					virtual std::shared_ptr<const MappingChannels> MappingChannels(void) const = 0;
				};

				/// <summary>
				/// Return the geometry for this instance.
				/// </summary>
				/// <returns>Geometry for this instance.</returns>
				virtual const IGeometry& Geometry(void) const = 0;

				/// <summary>
				/// Return the geometry for this instance.
				/// </summary>
				/// <returns>Geometry for this instance.</returns>
				virtual IGeometry& Geometry(void) = 0;

				/// <summary>
				/// Return the material for this instance.
				/// </summary>
				/// <returns>Material for this instance.</returns>
				virtual std::shared_ptr<const CRhRdkMaterial> Material(void) const = 0;

				/// <summary>
				/// The material for this instance should override any display mode materials.
				/// Implemented using EVF(L"IsForcedMaterial")
				/// </summary>
				/// <returns>true if the material should be forced..</returns>
				virtual_su bool IsForcedMaterial(void) const /*= 0*/; // [SDK_UNFREEZE]

				/// <summary>
				/// For per-instance mapping channels.  This overrides the mapping channels supplied by the geometry
				/// typically this will be nullptr.  If these channels are in use, they are specified in geometry space.
				/// </summary>
				/// <returns>Per instance mapping channel collection</returns>
				virtual std::shared_ptr<const MappingChannels> InstanceMappingChannels(void) const = 0;

				/// <summary>
				/// The transform for this instance.  Transform the geometry by this to set its final location in world space.
				/// </summary>
				/// <returns>The instance transform</returns>
				virtual const ON_Xform& Xform(void) const = 0;

				/// <summary>
				/// The bounding box for this instance.
				/// </summary>
				/// <param name="transformed">If true, the bounding box is returned in world space, if false, in geometry space.</param>
				/// <returns></returns>
				virtual ON_BoundingBox BoundingBox(bool transformed) const = 0;

				/// <summary>
				/// If this instance will change depending on the view direction.
				/// </summary>
				/// <returns>True if the instance is view dependent, otherwise false.</returns>
				virtual bool IsViewDependent(void) const = 0;

				/// <summary>
				/// If this instance will change depending on the requesting plug-in.
				/// </summary>
				/// <returns>True if the instance is dependent on the requesting plug-in, otherwise false.</returns>
				virtual bool IsRequestingPlugInDependent(void) const = 0;

				/// <summary>
				/// Transform this instance.  Note that if you're transforming an IRenderMeshes object, use the transform function
				/// there as it will update the hash.
				/// </summary>
				/// <param name=""></param>
				virtual void Transform(const ON_Xform&) = 0;

				/// <summary>
				/// Make a copy of the instance that can be modified - note, that the meshes/geometry themselves are still const and
				/// cannot be modified.  The modifyable parts are the transform and the mapping channels.
				/// </summary>
				/// <param name=""></param>
				/// <returns></returns>
				virtual std::shared_ptr<IInstance> MakeCopy(void) const = 0;

				

				/// <summary>
				/// Emergency virtual function for future expansion
				/// </summary>
				/// <param name="wszFunc">The named function</param>
				/// <param name="pv">Private data</param>
				/// <returns>Private data in the form of a void*</returns>
				virtual void* EVF(const wchar_t* wszFunc, void* pv) = 0;

			public:
				/// <summary>
				/// Use this function to create a ready made instance object
				/// </summary>
				/// <param name="id">The object Id this mesh came from.</param>
				/// <param name="mesh">The mesh - can be empty if primitive is not empty</param>
				/// <param name="cacheHandle">The cacheHandle to use for this mesh - should be unique to the object managed by the shared_ptr&lt;const ON_Mesh&gt;</param>
				/// <param name="cacheHandleId">An ID that is unique for any given mesh managed by the shared_ptr&lt;const ON_Mesh&gt;</param>
				/// <param name="primitive">Optionally the primitive for this instance - can be empty if the mesh is not empty.</param>
				/// <param name="meshMappingChannels">The mesh mapping channels - these must be the same for any given cacheHandleId.</param>
				/// <param name="material">The Material for this instance.</param>
				/// <param name="instanceMappingChannels">Per instance mapping channels - overrides the mesh mapping channels.  Can be empty (and usually is).</param>
				/// <param name="xform">The instance transformation.</param>
				/// <param name="IsViewDependent">True if the instance is view dependent, otherwise false.</param>
				/// <param name="IsRequestingPlugInDependent">True if the instance is dependent on the requesting plug-in, otherwise false.</param>
				/// <returns></returns>
				static std::shared_ptr<IInstance> New(
					const ObjectId& id,
					std::shared_ptr<const ON_Mesh> mesh,
					std::shared_ptr<CRhinoCacheHandle> cacheHandle,
					const ON_UUID& cacheHandleId,
					std::shared_ptr<const IRenderMeshes::MappingChannels> meshMappingChannels,
					std::shared_ptr<const CRhRdkMaterial> material,
					std::shared_ptr <const IRenderMeshes::MappingChannels> instanceMappingChannels,
					const ON_Xform& xform,
					bool IsViewDependent,
					bool IsRequestingPlugInDependent,
					bool ForceMaterial);

				ON_DEPRECATED_MSG("Use the one above") static std::shared_ptr<IInstance> New(
					const ObjectId& id,
					std::shared_ptr<const ON_Mesh> mesh,
					std::shared_ptr<CRhinoCacheHandle> cacheHandle,
					const ON_UUID& cacheHandleId,
					std::shared_ptr<const IRenderMeshes::MappingChannels> meshMappingChannels,
					std::shared_ptr<const CRhRdkMaterial> material,
					std::shared_ptr <const IRenderMeshes::MappingChannels> instanceMappingChannels,
					const ON_Xform& xform,
					bool IsViewDependent,
					bool IsRequestingPlugInDependent);

				//Simple version to just add a new mesh with the same materials, mapping and so on
				//automatically added a new cache handle.

				/// <summary>
				/// Use this function to create a ready made instance object - reusing the materials, mapping and so on from a previous instance
				/// object.  This is typically used inside a MeshProvider to replace the incoming instance with new geometry.
				/// </summary>
				/// <param name="mesh">The mesh</param>
				/// <param name="previous">The previous instance from which to copy.</param>
				/// <returns>A new instance object</returns>
				static std::shared_ptr<IInstance> New(
					std::shared_ptr<const ON_Mesh> mesh,
					const IInstance& previous);

				/// <summary>
				/// Extremely simple version for "use all object attributes".  Typically used
				/// when there are no previous meshes.
				/// </summary>
				/// <param name="mesh"></param>
				/// <returns>A new instance object</returns>
				static std::shared_ptr<IInstance> New(std::shared_ptr<const ON_Mesh> mesh, const CRhinoObject&);
			};

			/// <summary>
			/// Adds a new instance to the render primitives collection.
			/// </summary>
			/// <param name="instance">The instance to be added.</param>
			virtual void AddInstance(std::shared_ptr<IInstance> instance) = 0;

			struct Iterator;

			/// <summary>
			/// STL style constant iterator for iterating over the render primitives collection.
			/// </summary>
			struct RHRDK_SDK ConstantIterator
			{
				using iterator_category = std::random_access_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = std::shared_ptr<IInstance>;
				using pointer = const std::shared_ptr<IInstance>*;
				using reference = const std::shared_ptr<IInstance>&;

				ConstantIterator(const class CRenderMeshes&, size_t);
				ConstantIterator(const Iterator&);
				ConstantIterator(const ConstantIterator&);
				ConstantIterator& operator=(const ConstantIterator&);

				reference operator*() const;
				pointer operator->() const;
				reference operator[](size_t) const;
				ConstantIterator& operator++();
				ConstantIterator operator++(int);
				ConstantIterator& operator--();
				ConstantIterator operator--(int);
				bool operator== (const ConstantIterator& b);
				bool operator!= (const ConstantIterator& b);
				bool operator<(const ConstantIterator&) const;
				bool operator>(const ConstantIterator&) const;
				bool operator<=(const ConstantIterator&) const;
				bool operator>=(const ConstantIterator&) const;
				friend ConstantIterator operator+(size_t, const ConstantIterator&);
				ConstantIterator& operator-=(size_t);
				ConstantIterator operator-(size_t) const;
				difference_type operator-(ConstantIterator) const;
				ConstantIterator& operator+=(size_t);
				ConstantIterator operator+(size_t) const;

				const class CRenderMeshes* m_rp;
				mutable size_t m_index;
			};

			/// <summary>
			/// STL style iterator for iterating over the render primitives collection.
			/// </summary>
			struct RHRDK_SDK Iterator
			{
				using iterator_category = std::forward_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = std::shared_ptr<IInstance>;
				using pointer = std::shared_ptr<IInstance>*;
				using reference = std::shared_ptr<IInstance>&;

				Iterator(class CRenderMeshes& rp, size_t index);
				Iterator(const Iterator&);
				Iterator& operator=(const Iterator&);

				reference operator*() const;
				pointer operator->() const;
				reference operator[](size_t) const;
				Iterator& operator++();
				Iterator operator++(int);
				Iterator& operator--();
				Iterator operator--(int);
				bool operator== (const Iterator& b);
				bool operator!= (const Iterator& b);
				bool operator<(const Iterator&) const;
				bool operator>(const Iterator&) const;
				bool operator<=(const Iterator&) const;
				bool operator>=(const Iterator&) const;
				friend Iterator operator+(size_t, const Iterator&);
				Iterator& operator-=(size_t);
				Iterator operator-(size_t) const;
				difference_type operator-(Iterator) const;
				Iterator& operator+=(size_t);
				Iterator operator+(size_t) const;

				class CRenderMeshes* m_rp;
				mutable size_t m_index;
			};

			using ReverseIterator = std::reverse_iterator<Iterator>;
			using ConstantReverseIterator =  std::reverse_iterator<ConstantIterator>;

			friend Iterator;
			friend ConstantIterator;
			friend ReverseIterator;
			friend ConstantReverseIterator;

			virtual Iterator begin() = 0;
			virtual Iterator end() = 0;
			virtual ConstantIterator begin() const = 0;
			virtual ConstantIterator end() const = 0;
			virtual ConstantIterator cbegin() const = 0;
			virtual ConstantIterator cend() const = 0;
			virtual ReverseIterator rbegin() = 0;
			virtual ReverseIterator rend() = 0;
			virtual ConstantReverseIterator crbegin() const = 0;
			virtual ConstantReverseIterator crend() const = 0;

			/// <summary>
			/// The number of instance objects in this collection.
			/// </summary>
			/// <returns></returns>
			virtual size_t InstanceCount() const = 0;

			/// <summary>
			/// The ObjectId with which this IRenderMeshes object is associated.
			/// </summary>
			/// <returns>The objectId for which this collection of primitives is built.</returns>
			virtual ObjectId ObjectId(void) const = 0;

			/// <summary>
			/// Each IRenderMeshes is associated with a specific document.
			/// </summary>
			/// <returns>The document with which this IRenderMeshes object is associated.</returns>
			virtual CRhinoDoc* Document(void) const = 0;

			/// <summary>
			/// The bounding box for this IRenderMeshes, always in world space.
			/// </summary>
			/// <returns>The bounding box for this IRenderMeshes, always in world space.</returns>
			virtual ON_BoundingBox BoundingBox(void) const = 0;

			/// <summary>
			/// The running hash for this set of render primitives.  Usually calculated by a set of parameters in the MeshProvider and based on
			/// an initial hash delivered by the first set of primitives.
			/// </summary>
			/// <returns>The running hash for this IRenderMeshes.</returns>
			virtual ON__UINT32 Hash(void) const = 0;

			/// <summary>
			/// Sets the running hash for this set of render primitives.  Usually calculated by a set of parameters in the MeshProvider and based on
			/// an initial hash delivered by the first set of primitives.
			/// Note - implemented using EVF(L"SetHash", ON__UINT32*)
			/// </summary>
			virtual_su void SetHash(ON__UINT32); // [SDK_UNFREEZE]

			/// <summary>
			/// The flags with which these render primitives were created.
			/// </summary>
			/// Note - implemented using EVF(L"Flags", nullptr)
			virtual_su ON__UINT32 Flags(void) const; //[SDK_UNFREEZE]

			/// <summary>
			/// Transform this set of render primitives.
			/// </summary>
			/// <param name=""></param>
			virtual void Transform(const ON_Xform&) = 0;

			/// <summary>
			/// Create a copy of the render meshes that can be modified without risk of modifying the cached/document meshes.
			/// </summary>
			/// <param name=""></param>
			/// <returns></returns>
			virtual std::shared_ptr<IRenderMeshes> MakeCopy(void) const = 0;

			/// <summary>
			/// Emergency virtual function for future expansion
			/// </summary>
			/// <param name="wszFunc">The named function</param>
			/// <param name="pv">Private data</param>
			/// <returns>Private data in the form of a void*</returns>
			virtual void* EVF(const wchar_t*, void*) = 0;
            
            ///The function used to create the initial hash from the object when creating the initial set of primitives.
            static ON__UINT32 ObjectHash(const CRhinoObject&);
		};



		/// <summary>
		/// An IMeshProvider delivers custom render meshes (in the form of IRenderMeshes).  Typically the class will also derive from CRhRdkExtension
		/// and be registered with the RDK using RhRdkAddExtension, forming a collection of providers which the IManager will cycle through to produce the final render mesh.
		/// Each IMeshProvider::HasCustomMeshes will be called and if it returns true, a call to CustomMeshes will be made. It is up to the provider
		/// to cache its own primitives - the IRenderMeshes::ProviderTracking class is provided for that.
		/// A provider may optionally return a collection of non-object Ids that it will provide custom render meshes for.  An example of this is Grasshopper, which
		/// which will typically return a collection of the Ids of each CustomPreview component.
		/// Override this class if you are a plug-in developer intending to supply a custom set of meshes for a given object, or objectId.  Examples of IMeshProviders
		/// are CurvePiping, EdgeSoftening, Displacement, Shutlining, Grasshopper's CustomPreview component.
		/// </summary>
		class RHRDK_SDK IRenderMeshProvider
		{
		public:
			virtual ~IRenderMeshProvider() { }
			
			/// <summary>
			/// The ICustomPrimitiveProvider interface Id to return in CRhRdkExtension::InterfaceId.  Typically implemented as return ICustomPrimitiveProvider::InterfaceId();
			/// </summary>
			/// <returns>The IMeshProvider interface Id</returns>
			static ON_UUID InterfaceId(void);

			/// <summary>
			/// Each provider has a unique identifier.
			/// </summary>
			/// <returns>The uuid of this provider.</returns>
			virtual MeshProviderId ProviderId(void) const = 0;

			/// <summary>
			/// The localized name of the provider for UI display.
			/// </summary>
			/// <returns>The name of the provider for UI display.</returns>
			virtual ON_wString Name(void) const = 0;

			/// <summary>
			/// A provider may optionally return a collection of non-object Ids that it will provide custom render meshes for.  An example of this is Grasshopper, which
			/// which will typically return a collection of the Ids of each CustomPreview component.
			/// </summary>
			/// <param name=""></param>
			/// <returns>A collection of non-object Ids for which this provider may provide custom meshes.</returns>
			virtual std::vector<ObjectId> NonObjectIds(void) const = 0;

			/// <summary>
			/// Determine if custom primitives will be supplied for a particular object.  This function should return as quickly as possible.
			/// </summary>
			/// <param name="vp">The view being rendered.</param>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="objectId">The objectId for which the primitives should be supplied.</param>
			/// <param name="flags">See IManager::Flags</param>
			/// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
			/// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
			/// <returns>True if RenderMeshes will return a set of custom primitives, otherwise false.  RenderMeshes may still return an empty collection - in case of cancelation something similar.</returns>
			virtual bool HasCustomRenderMeshes(
				ON::mesh_type mt,
				const ON_Viewport& vp,
				const CRhinoDoc& doc,
				const ObjectId& objectId,
				ON__UINT32& flags,
				const CRhinoPlugIn* requestingPlugIn,
				const CDisplayPipelineAttributes* pAttributes
				) const = 0;

			//Return nullptr if you did not build meshes.  Don't worry about the nullptr being passed to the next provider - that is handled internally.

			/// <summary>
			/// Returns a set of render primitives for a given ObjectId.
			/// </summary>
			/// <param name="vp">The view being rendered.</param>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="objectId">The objectId for which the meshes should be supplied.</param>
			/// <param name="ancestry_only_for_by_parent_attributes">The ancestry of the object - ie, block references referring to this definition.  
			/// This is only used to handle ByParent attributes, and should not be used to transform geometry.</param>
			/// <param name="flags">See IManager::Flags</param>
			/// <param name="crmIn">The previous set of primitives.  These may be the original render meshes on the object.  They may also be empty - in
			/// the case of a non-object id - or non-meshable geometry.</param>
			/// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
			/// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
			/// <returns>Returns a set of custom render primitives for a given ObjectId.</returns>
			virtual std::shared_ptr<const IRenderMeshes> RenderMeshes(
				ON::mesh_type mt, 
				const ON_Viewport& vp,
				const CRhinoDoc& doc,
				const ObjectId& objectId,
				const CRhRdkObjectAncestry& ancestry_only_for_by_parent_attributes,
				ON__UINT32& flags,
				std::shared_ptr<const IRenderMeshes> crmIn,
				const CRhinoPlugIn* requestingPlugIn,
				const CDisplayPipelineAttributes* pAttributes
				) const = 0;

			/// <summary>
			/// Runtime access to specific parameters on for a given objectId relating to this provider.
			/// </summary>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="object">The objectId for which the parameter should be supplied.</param>
			/// <param name="wszParamName">The name of the parameter.</param>
			/// <returns>The value of the parameter for the specified ObjectId.</returns>
			virtual CRhRdkVariant GetParameter(const CRhinoDoc& doc, const ObjectId& object, const wchar_t* wszParamName) const = 0;

			/// <summary>
			/// Runtime access to set specific parameters on for a given objectId relating to this provider.
			/// </summary>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="object">The objectId for which the parameter should be supplied.</param>
			/// <param name="wszParamName">The value of the parameter for the specified ObjectId.</param>
			/// <param name="value">The value to set.</param>
			virtual void SetParameter(const CRhinoDoc& doc, const ObjectId& object, const wchar_t* wszParamName, const CRhRdkVariant& value) = 0;

			class IProgress
			{
			public:
				virtual ~IProgress() {}

				virtual ON_wString Text(void) const = 0;
				virtual double Amount(void) const = 0;
				virtual double Target(void) const = 0;
				virtual bool IsComplete(void) const = 0;
				virtual MeshProviderId ProviderId(void) const = 0;
			};
			
			virtual std::unique_ptr<IProgress> Progress(const CRhinoDoc&, const std::vector<ObjectId>* pOptionalObjects) const = 0;

			/// <summary>
			/// Emergency virtual function for future expansion
			/// 
			/// L"NonDocIdsOnly" = true (returns non nullptr) - HasCustomRenderMeshes will not be called on document resident objects.
			/// </summary>
			/// <param name="wszFunc">The named function</param>
			/// <param name="pv">Private data</param>
			/// <returns>Private data in the form of a void*</returns>
			virtual void* EVF(const wchar_t*, void*) = 0;
		};





		/// <summary>
		/// The interface to the RDK's custom render mesh manager.  Do not derive from this class - you can access this functionality using RhRdkCustomRenderMeshManager2
		/// Call the functions on this class to access the custom render meshes to be supplied for any given ObjectId (see ObjectId above).  You should call HasCustomMeshes
		/// before CustomMeshes to ensure the most efficient access - if HasCustomMeshes returns false, you can assume there are no custom render meshes.
		/// This class provides low level access to the custom render meshes - to just get the complete render meshes for an object, prefer IRhRdkSdkRenderMeshIterator / 
		/// CRhRdkSdkRender::NewRenderMeshIterator, or RhRdk::Realtime2::ChangeQueue.
		/// Use these functions if you are a renderer developer who wants to
		/// access the custom render meshes provided by IMeshProvider.  Note that this will include the built - in displacement
		/// edge softening, thickening, shut-lining and curve piping tools, as well as Grasshopper's CustomPreview component (which uses the NonObjectIds method of providing meshes associated
		/// with entities that are not actual objects in the document)
		/// </summary>
		class RHRDK_SDK IManager
		{
		public:
			virtual ~IManager() { }

			/// <summary>
			/// Determine if custom render meshes will be supplied for a particular object.
			/// </summary>
			/// <param name="vp">The viewport being rendered</param>
			/// <param name="doc">The Rhino document associated with the objectId</param>
			/// <param name="objectId">The objectId for which the meshes should be supplied.</param>
			/// <param name="flags">See IManager::Flags.</param>
			/// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
			/// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
			/// <returns>True if RenderMeshes will return a set of custom primitives, otherwise false.  
			/// RenderMeshes may still return an empty collection - in case of cancelation something similar, </returns>
			virtual bool HasCustomRenderMeshes(
				ON::mesh_type mt,
				const ON_Viewport& vp,
				const CRhinoDoc& doc,
				const ON_UUID& objectId,
				ON__UINT32& flags,
				const CRhinoPlugIn* requestingPlugIn,
				const CDisplayPipelineAttributes* pAttributes
				) const = 0;

			/// <summary>
			/// Flags to be passed into and returned from RenderMeshes and HasCustomRenderMeshes.  
			/// </summary>
			struct Flags
			{
				/// <summary>
				/// No flags
				/// </summary>
				static const ON__UINT32 None = 0;
				/// <summary>
				/// If this flag is set when returned, then CustomMeshes was canceled.
				/// </summary>
				static const ON__UINT32 Canceled = 1;

				/// <summary>
				/// Pass in this flag to bypass any primitive caching that the MeshProviders might do.
				/// </summary>
				static const ON__UINT32 DisableCaching = 2;

				/// <summary>
				/// Pass in this flag to get the render meshes for entire blocks.  Only has an effect if the ObjectId is a block reference, and if it will not produce CRMs of its own.
				/// </summary>
				static const ON__UINT32 Recursive = 4;

				/// <summary>
				/// Pass in this flag to indicate that the object id is a document resident object
				/// </summary>
				static const ON__UINT32 IsDocumentObject = 8;

				/// <summary>
				/// Pass in this flag to ensure that materials, environments and textures that refer to document objects are always copied.  This makes the RenderMeshes
				/// object safe to be stored outside of the lifetime of those objects - in caches, or for use in threaded situtations.
				/// </summary>
				static const ON__UINT32 AlwaysCopyDocumentContent = 16;

				/// <summary>
				/// Pass in this flag to request that nullptr is returned when the material on a mesh should be the standard material used - in otherwords, not 
				/// customized by this RenderMeshes object.  Note that this will require document access to resolve - do not use in threaded situations.
				/// </summary>
				static const ON__UINT32 ReturnNullForStandardMaterial = 32;
                
                /// <summary>
                /// This flag is returned when an asynchronous operation (like Displacement) has not yet completed.
                /// </summary>
                static const ON__UINT32 Incomplete = 64;
			};

			/// <summary>
			/// Returns a complete set of custom render meshes (primitives) for a given ObjectId.
			/// </summary>
			/// <param name="vp">The view being rendered.</param>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="objectId">The objectId for which the meshes should be supplied.</param>
			/// <param name="ancestry_only_for_by_parent_attributes">The ancestry of the object - ie, block references referring to this definition.  
			/// This is only used to handle ByParent attributes, and should not be used to transform geometry.</param>
			/// <param name="flags">See IManager::Flags</param>
			/// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
			/// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
			/// <returns>Returns a set of custom render meshes (primitives) for a given ObjectId.</returns>
			/// <param name="optional_initial_primitives">Optionally a custom set of initial primitives.  If this parameter is empty, the standard render meshes
			/// for the object will be used, or an empty set of primitives will be used where they are not available.  This is typically used in meshing exporters, which
			/// produce specific meshes for the output rather than using the render meshes.</param>
			/// <returns>Returns a complete set of custom render meshes (primitives) for a given ObjectId.</returns>
			virtual std::shared_ptr<const IRenderMeshes> RenderMeshes(
				ON::mesh_type mt,
				const ON_Viewport& vp,
				const CRhinoDoc& doc,
				const ObjectId& objectId,
				const CRhRdkObjectAncestry& ancestry_only_for_by_parent_attributes,
				ON__UINT32& flags,
				const CRhinoPlugIn* requestingPlugIn = nullptr,
				const CDisplayPipelineAttributes* pAttributes = nullptr,
				std::shared_ptr<const IRenderMeshes> optional_initial_primitives = nullptr
				) const = 0;

			/// <summary>
			/// Returns a Hash which identifies a particular state of the custom meshes for rendering purposes.  Note
			/// that this includes the hashing of material and mapping if they are returned as empty in IRenderMeshes.
			/// </summary>
			/// <param name="vp">The view being rendered.</param>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="objectId">The objectId for which the meshes should be supplied.</param>
			/// <param name="ancestry_only_for_by_parent_attributes">The ancestry of the object - ie, block references referring to this definition.  
			/// This is only used to handle ByParent attributes, and should not be used to transform geometry.</param>
			/// <param name="flags">See IManager::Flags</param>
			/// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
			/// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
			/// <returns>Returns a set of custom render meshes (primitives) for a given ObjectId.</returns>
			/// <param name="optional_initial_primitives">Optionally a custom set of initial primitives.  If this parameter is empty, the standard render meshes
			/// for the object will be used, or an empty set of primitives will be used where they are not available.  This is typically used in meshing exporters, which
			/// produce specific meshes for the output rather than using the render meshes.</param>
			/// <returns>A Hash which identifies a particular state of the custom meshes for rendering purposes</returns>
			virtual ON__UINT32 RenderMeshesHash(
				ON::mesh_type mt,
				const ON_Viewport& vp,
				const CRhinoDoc& doc,
				const ObjectId& objectId,
				const CRhRdkObjectAncestry& ancestry_only_for_by_parent_attributes,
				ON__UINT32& flags,
				const CRhinoPlugIn* requestingPlugIn = nullptr,
				const CDisplayPipelineAttributes* pAttributes = nullptr,
				std::shared_ptr<const IRenderMeshes> optional_initial_primitives = nullptr
			) const = 0;

			/// <summary>
			/// The bounding box for the complete set of custom render meshes for this object Id.  Note that the input parameters are the same as
			/// for CustomMeshes because typically this function is implemented by calling CustomMeshes and calculating the bounding box.  In that case, it
			/// is important to cache the finished primitives at this point rather than calcuating them again for the call to CustomMeshes.
			/// </summary>
			/// <param name="vp">The view being rendered.</param>
			/// <param name="doc">The Rhino document of interest.</param>
			/// <param name="objectId">The objectId for which the meshes should be supplied.</param>
			/// <param name="ancestry_only_for_by_parent_attributes">The ancestry of the object - ie, block references referring to this definition.  
			/// This is only used to handle ByParent attributes, and should not be used to transform geometry.</param>
			/// <param name="flags">See IManager::Flags</param>
			/// <param name="requestingPlugIn">The requesting plug-in - typically the render plug-in requesting the meshes.</param>
			/// <param name="pAttributes">The display attributes currently being used - nullptr if this is production rendering.</param>
			/// <returns>Returns a set of custom render meshes (primitives) for a given ObjectId.</returns>
			/// <param name="optional_initial_primitives">Optionally a custom set of initial primitives.  If this parameter is empty, the standard render meshes
			/// for the object will be used, or an empty set of primitives will be used where they are not available.  This is typically used in meshing exporters, which
			/// produce specific meshes for the output rather than using the render meshes.</param>
			/// <returns>The bounding box for the complete custom meshes.</returns>
			virtual ON_BoundingBox BoundingBox(
				ON::mesh_type mt, 
				const ON_Viewport& vp,
				const CRhinoDoc& doc,
				const ObjectId& objectId,
				const CRhRdkObjectAncestry& ancestry_only_for_by_parent_attributes,
				ON__UINT32& flags,
				const CRhinoPlugIn* requestingPlugIn = nullptr,
				const CDisplayPipelineAttributes* pAttributes = nullptr,
				std::shared_ptr<const IRenderMeshes> optional_initial_primitives = nullptr
			) const = 0;

			/// <summary>
			/// Get an interface to a specific Mesh provider previously registered with the RDK using RhRdkAddExtension.
			/// </summary>
			/// <param name="uuidProvider">Identifies the provider required.</param>
			/// <returns>A pointer to the interface of the specified provider, or nullptr if not found.</returns>
			virtual const IRenderMeshProvider* Provider(const MeshProviderId& uuidProvider) const = 0;

			/// <summary>
			/// Get the current progress for either the whole document, or for a collection of objects
			/// </summary>
			/// <param name="doc">The document for which you are requesting the progress.</param>
			/// <param name="pOptionalObjects">Optionally an array of objects - if nullptr, then the progress report is for the entire document.</param>
			/// <returns></returns>
			virtual std::vector<std::unique_ptr<IRenderMeshProvider::IProgress>> Progress(const CRhinoDoc& doc, const std::vector<ObjectId>* pOptionalObjects) const = 0;

			/// <summary>
			/// Providers may optionally return a collection of non-object Ids that they will provide custom render meshes for.  An example of this is Grasshopper, which
			/// which will typically return a collection of the Ids of each CustomPreview component.  This function returns a complete list of all of the NonObjectIds 
			/// returned from all registered MeshProviders.
			/// </summary>
			/// <returns>A collection of non-object Ids for which any provider may supply custom meshes.</returns>
			virtual std::vector<ObjectId> NonObjectIds(void) const = 0;

			/// <summary>
			/// Emergency virtual function for future expansion
			/// </summary>
			/// <param name="wszFunc">The named function</param>
			/// <param name="pv">Private data</param>
			/// <returns>Private data in the form of a void*</returns>
			virtual void* EVF(const wchar_t*, void*) = 0;
		};
	}
}



