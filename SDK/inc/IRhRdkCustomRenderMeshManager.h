
#pragma once

#ifndef EXCLUDE_OLD_CRMS

#include "RhRdkDefinitions.h"
#include "RhRdkObject.h"
#include "RhRdkExtension.h"
#include "RhRdkVariant.h"

/** \enum RhRdkRenderMeshPrimitiveTypes
	\see IRhRdkCustomRenderMeshes::PrimitiveType.
*/
enum class RhRdkRenderMeshPrimitiveTypes : unsigned int
{
	None   = 0,
	Mesh   = 1,
	Sphere = 2,
	Plane  = 3,
	Box    = 4,
	Cone   = 5,
};

class CRhRdkMaterial;
class CRhRdkObjectAncestry;
class CRhRdkCustomRenderMeshProvider;

/** \class IRhRdkCustomRenderMeshes

	This interface is a collection of custom render meshes for a particular Rhino object.
	It is used by the custom render mesh provider platform. \see IRhRdkCustomRenderMeshProvider.

*/
class RHRDK_SDK IRhRdkCustomRenderMeshes
{
public:
	virtual ~IRhRdkCustomRenderMeshes() { }

	/** The provider should call this to make the object automatically delete all meshes 
		when they are no longer needed. If called, it must be called before calling Add(). */
	virtual void SetAutoDeleteMeshesOn(void) = 0;

	/** The provider should call this to make the object automatically delete all materials 
		when they are no longer needed. If called, it must be called before calling Add(). */
	virtual void SetAutoDeleteMaterialsOn(void) = 0;

	/** The provider should call this to identify itself. It must be called before calling Add(). */
	virtual void SetProviderId(const UUID& uuidRMP) = 0;

	/** \return The id of the provider that called SetProviderId(). */
	virtual UUID ProviderId(void) const = 0;

	/** Add mesh and material.
		\param pMesh is the mesh to add.
		\param pMaterial is the material to add. Can be NULL if not needed.
		 If SetAutoDeleteMaterialsOn has been called prior to calling Add, the material will be copied. */
	virtual void Add(ON_Mesh* pMesh, const CRhRdkMaterial* pMaterial) = 0;

	/** Add primitive sphere and material.
		\param sphere is the sphere to add.
		\param pMaterial is the material to add. Can be NULL if not needed.
		 If SetAutoDeleteMaterialsOn has been called prior to calling Add, the material will be copied. */
	virtual void Add(const ON_Sphere& sphere, const CRhRdkMaterial* pMaterial) = 0;

	/** Add primitive (possibly truncated) cone and material.
		\param cone is the cone to add as defined in ON_Cone.
		\param truncation is the plane used to cut the cone (the non-apex end is kept).
		 Should be equal to cone.plane if not truncated.
		\param pMaterial is the material to add. Can be NULL if not needed.
		 If SetAutoDeleteMaterialsOn has been called prior to calling Add, the material will be copied. */
	virtual void Add(const ON_Cone& cone, const ON_Plane& truncation, const CRhRdkMaterial* pMaterial) = 0;

	/** Add primitive finite plane and material.
		\param plane is the plane to add.
		\param pMaterial is the material to add. Can be NULL if not needed.
		 If SetAutoDeleteMaterialsOn has been called prior to calling Add, the material will be copied. */
	virtual void Add(const ON_PlaneSurface& plane, const CRhRdkMaterial* pMaterial) = 0;

	/** Add primitive box and material.
		\param plane is the plane of the box to add.
		\param cz is the z-interval of the box.
		\param pMaterial is the material to add. Can be NULL if not needed.
		 If SetAutoDeleteMaterialsOn has been called prior to calling Add, the material will be copied. */
	virtual void Add(const ON_PlaneSurface& plane, const ON_Interval& cz, const CRhRdkMaterial* pMaterial) = 0;

	/** \return Number of mesh/material pairs that are in the collection. */
	virtual int Count(void) const = 0;

	/** \return \e true if the texture mapping will be taken from the Rhino object.
		Otherwise the texture mapping will use the TCs on the mesh only. */
	virtual bool UseObjectsMappingChannels(void) const = 0;

	/** The provider calls this with \e true if the texture mapping will be taken from
		the Rhino object. Otherwise it calls this with \e false. Then the texture mapping
		will use the TCs on the mesh only. */
	virtual void SetUseObjectsMappingChannels(bool bSet) = 0;

	/** \return The primitive type supported by the mesh. Usually just \e mesh. */
	virtual RhRdkRenderMeshPrimitiveTypes PrimitiveType(int iIndex) const = 0;

	/** Get a mesh by its index.
		\param iIndex is the mesh index which must be >= 0 and < Count().
		\return A pointer to the mesh or NULL if iIndex is out of range. 
		\note This always returns a mesh even if this is a primitive type other than mesh;
		 i.e., a suitable mesh is created on the fly. */
	virtual const ON_Mesh* Mesh(int iIndex) const = 0;

	/** Get a sphere by its index.
		\param iIndex is the geometry index which must be >= 0 and < Count().
		\param sphere is the spherical geometry if the function returns \e true.
		\return \e true if the geometry at the index is a sphere.  \e false if not, or iIndex is out of range. */
	virtual bool Sphere(int iIndex, ON_Sphere& sphere) const = 0;

	/** Get a box by its index.
		\param iIndex is the geometry index which must be >= 0 and < Count().
		\param plane is the base of the box geometry if the function returns \e true.
		\param z_interval is the height of the box geometry if the function returns \e true.
		\return \e true if the geometry at the index is a box.  \e false if not, or iIndex is out of range. */
	virtual bool Box(int iIndex, ON_PlaneSurface& plane, ON_Interval& z_interval) const = 0;

	/** Get a plane by its index.
		\param iIndex is the geometry index which must be >= 0 and < Count().
		\param plane is the plane geometry if the function returns \e true.
		\return \e true if the geometry at the index is a plane.  \e false if not, or iIndex is out of range. */
	virtual bool Plane(int iIndex, ON_PlaneSurface& plane) const = 0;

	/** Get a truncated cone by its index.
		\param iIndex is the geometry index which must be >= 0 and < Count().
		\param cone is the untruncated conical geometry if the function returns \e true.
		\param truncation is the planar truncation of the cone if the function returns \e true.
		\return \e true if the geometry at the index is a cone.  \e false if not, or iIndex is out of range. */
	virtual bool Cone(int iIndex, ON_Cone& cone, ON_Plane& truncation) const = 0;
	
	/** Get a material by its index.
		\param iIndex is the material index which must be >= 0 and < Count().
		\return A pointer to the material or NULL if iIndex is out of range. */
	virtual const CRhRdkMaterial* Material(int iIndex) const = 0;

	/** \return A pointer to the Rhino object. */
	virtual const CRhinoObject* Object(void) const = 0;

	/** \return The transformation that the meshes have undergone from their original
		location in instance space. i.e., The mesh should not be transformed by this, but
		other transformable data (such as primitive mapping types) should have this applied
		before using them. */
	virtual const ON_Xform& GetInstanceTransform(void) const = 0;

	/** Providers should set this information - \see GetInstanceTransform() for more information. */
	virtual void SetInstanceTransform(const ON_Xform& xformInstance) = 0;

	/** Convert all meshes to triangles if they contain quads. */
	virtual void ConvertMeshesToTriangles(void) = 0;

	/* Returns the "parent chain" for the object for use in implementing ByParent materials etc. */
	virtual const CRhRdkObjectAncestry* Ancestry(void) const = 0;

	/** Emergency virtual function for future expansion. 
		"AddMesh" - new implementation of AddMesh that also supoprts instancing.   pv = struct { ON_Mesh*; const CRhRdkMaterial*; ON_Xform*; }
	*/
	virtual void* EVF(const wchar_t*, void*) = 0;

public:
	/** Create a new render mesh collection. Use this instead of creating a CRhRdkCustomRenderMeshes object
		(that class is now deprecated).
		\param pObject is the object to which these custom render meshes apply or null if the render meshes
		 are for the document based custom render mesh.
		\return a new object that supports the IRhRdkCustomRenderMeshes interface.
		 Call IRhRdkCustomRenderMeshes2::Promote to get IRhRdkCustomRenderMeshes2 */
	static IRhRdkCustomRenderMeshes* New(const CRhinoObject* pObject);

	/** Delete a IRhRdkCustomRenderMeshes object. */
	static void Delete(IRhRdkCustomRenderMeshes*);
};

/** \class IRhRdkCustomRenderMeshes2

	This interface is a collection of custom render meshes for a particular Rhino object.
	It extends IRhRdkCustomRenderMeshes and can be exposed from an IRhRdkCustomRenderMeshes pointer
	using IRhRdkCustomRenderMeshes2::Promote(). Do not up-cast; always use Promote.
	It is used by the custom render mesh provider platform. \see IRhRdkCustomRenderMeshProvider.

*/
class RHRDK_SDK IRhRdkCustomRenderMeshes2 : public IRhRdkCustomRenderMeshes
{
public:
	virtual ~IRhRdkCustomRenderMeshes2() { }

	/** Clear the collection of custom render meshes. */
	virtual void Clear(void) = 0;

	/** Gets the ownership of meshes in the collection. */
	virtual bool AutoDeleteMeshesOn(void) const = 0;

	/** Gets the ownership of materials in the collection. */
	virtual bool AutoDeleteMaterialsOn(void) const = 0;

	/** Sets the ancenstry of meshes in the collection.  This is used to identify which instance chain the meshes refer to. */
	virtual void SetAncestry(const class CRhRdkObjectAncestry& pAncestry) = 0;

	/** Gets the ancenstry of meshes in the collection. */
	virtual const CRhRdkObjectAncestry* Ancestry(void) const = 0;

	/** Adds a mesh/transform to the collection.
		\param pMesh pointer to the mesh.  Note that you can add the same mesh over and over with different transforms.
		 If SetAutoDeleteMeshesOn has been called prior to calling Add, the mesh will be copied.
		\param pMaterial is the material to add. Can be NULL if not needed.
		 If SetAutoDeleteMaterialsOn() has been called prior to calling Add(), the material will be copied.
		\param instance_transform is the transformation of this mesh.
	*/
	virtual void AddMeshInstance(ON_Mesh* pMesh, const CRhRdkMaterial* pMaterial, const ON_Xform& instance_transform) = 0;

	/** Get a mesh by its index with support for instanced, transformed meshes.
		\param iIndex is the mesh index which must be >= 0 and < Count().
		\param instance_transform is the instance transform if this mesh has one.  Otherwise this will be identity.
		\return A pointer to the mesh or NULL if iIndex is out of range.
		\note This always returns a mesh even if this is a primitive type other than mesh;
		 i.e., a suitable mesh is created on the fly. */
	virtual const ON_Mesh* MeshInstance(int iIndex, ON_Xform& instance_transform) const = 0;

	/** Transform all of the meshes in the collection. 
		\param xform is the transformation to apply.
	*/
	virtual void ApplyTransformToAllMeshes(const ON_Xform& xform) = 0;

	/** Sets cache usages for the collection. */
	virtual void SetUseCache(bool bUseCache) = 0;

	/** Gets cache usages for the collection. */
	virtual bool UseCache(void) const = 0;

	/** Gets the instance id of the material for this index.  Since materials are generally copied,
		they will have different instance ids to the ones in the document. */
	virtual const UUID& OriginalMaterialId(int iIndex) const = 0;

public:
	/** Return a IRhRdkCustomRenderMeshes2 interface pointer for a given IRhRdkCustomRenderMeshes interface pointer if one exists. */
	static IRhRdkCustomRenderMeshes2* Promote(IRhRdkCustomRenderMeshes*);

	/** Return a IRhRdkCustomRenderMeshes2 interface pointer for a given IRhRdkCustomRenderMeshes interface pointer if one exists. */
	static const IRhRdkCustomRenderMeshes2* Promote(const IRhRdkCustomRenderMeshes*);
};

class RHRDK_SDK IRhRdkCustomRenderMeshes3 : public IRhRdkCustomRenderMeshes2
{
public:
	virtual ~IRhRdkCustomRenderMeshes3() { }

	virtual void OriginalTextureMappingIds(ON_SimpleArray<UUID>& ids) const = 0;

public:
	/** Return a IRhRdkCustomRenderMeshes2 interface pointer for a given IRhRdkCustomRenderMeshes interface pointer if one exists. */
	static IRhRdkCustomRenderMeshes3* Promote(IRhRdkCustomRenderMeshes*);

	/** Return a IRhRdkCustomRenderMeshes2 interface pointer for a given IRhRdkCustomRenderMeshes interface pointer if one exists. */
	static const IRhRdkCustomRenderMeshes3* Promote(const IRhRdkCustomRenderMeshes*);
};

/** \class CRhRdkCustomRenderMeshes

	Built-in implementation of IRhRdkCustomRenderMeshes.  This is now DEPRECATED.
	Use the creator functions in the interface classes.

*/
class RHRDK_SDK CRhRdkCustomRenderMeshes : public IRhRdkCustomRenderMeshes
{
public:
	//Use IRhRdkCustomRenderMeshes::New
	RDK_DEPRECATED CRhRdkCustomRenderMeshes(const CRhinoObject* pObject);
	virtual ~CRhRdkCustomRenderMeshes();

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void SetAutoDeleteMeshesOn(void);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void SetAutoDeleteMaterialsOn(void);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void SetProviderId(const UUID& uuidRMP);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual UUID ProviderId(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool UseObjectsMappingChannels(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void SetUseObjectsMappingChannels(bool bSet);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void Add(ON_Mesh* pMesh, const CRhRdkMaterial* pMaterial);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void Add(const ON_Sphere& sphere, const CRhRdkMaterial* pMaterial);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void Add(const ON_PlaneSurface& plane, const CRhRdkMaterial* pMaterial);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void Add(const ON_PlaneSurface& plane, const ON_Interval& cz, const CRhRdkMaterial* pMaterial);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void Add(const ON_Cone& cone, const ON_Plane& truncation, const CRhRdkMaterial* pMaterial);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual int  Count(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual const ON_Mesh* Mesh(int iIndex) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual RhRdkRenderMeshPrimitiveTypes PrimitiveType(int iIndex) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool Sphere(int iIndex, ON_Sphere& sphere) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool Box(int iIndex, ON_PlaneSurface& plane, ON_Interval& z_interval) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool Plane(int iIndex, ON_PlaneSurface& plane) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool Cone(int iIndex, ON_Cone& cone, ON_Plane& truncation) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual const CRhRdkMaterial* Material(int iIndex) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual const UUID& OriginalMaterialId(int iIndex) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes3 */
	void OriginalTextureMappingIds(ON_SimpleArray<UUID>& ids) const;

	std::shared_ptr<CRhinoCacheHandle> DisplayCacheHandle(int iIndex) const;

	void AddEx(ON_Mesh* pMesh, const CRhRdkMaterial* pMaterial, std::shared_ptr<CRhinoCacheHandle> handle);
	void AddEx(const ON_Sphere& sphere, const CRhRdkMaterial* pMaterial, std::shared_ptr<CRhinoCacheHandle> handle);
	void AddEx(const ON_Cone& cone, const ON_Plane& truncation, const CRhRdkMaterial* pMaterial, std::shared_ptr<CRhinoCacheHandle> handle);
	void AddEx(const ON_PlaneSurface& plane, const CRhRdkMaterial* pMaterial, std::shared_ptr<CRhinoCacheHandle> handle);
	void AddEx(const ON_PlaneSurface& plane, const ON_Interval& cz, const CRhRdkMaterial* pMaterial, std::shared_ptr<CRhinoCacheHandle> handle);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual const CRhinoObject* Object(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void ConvertMeshesToTriangles(void);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void PrepareRenderMeshesForMapping(void);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual const ON_Xform& GetInstanceTransform(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void SetInstanceTransform(const ON_Xform& xformInstance);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void Clear(void);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool AutoDeleteMeshesOn(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual bool AutoDeleteMaterialsOn(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void SetAncestry(const class CRhRdkObjectAncestry& pAncestry);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual const CRhRdkObjectAncestry* Ancestry(void) const;

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	virtual void AddOnTheFlyMeshFromCache(int index, const ON_Mesh* pMesh);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	void Add(ON_Mesh* pMesh, const CRhRdkMaterial* pMaterial, const ON_Xform* xform);
	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	void AddEx(ON_Mesh* pMesh, const CRhRdkMaterial* pMaterial, const ON_Xform* xform, std::shared_ptr<CRhinoCacheHandle> handle);

	const ON_Mesh* MeshInstance(int iIndex, ON_Xform& instance_transform) const;

public:
	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	IRhRdkCustomRenderMeshes2& I2(void);

	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	const IRhRdkCustomRenderMeshes2& I2(void) const;

public:
	class CImpl;
	CImpl* m_pImpl;

private:
	/** Deprecated.  See IRhRdkCustomRenderMeshes */
	const ON_Mesh* MeshInstanceImpl(int iIndex, ON_Xform* instance_transform) const;
};

/** \class IRhRdkCustomRenderMeshManager

	This is the interface to RDK's custom render mesh manager.  Use these functions if you are a renderer developer who wants to
	access the custom render meshes provided by CRhRdkCustomRenderMeshProvider.  Note that this will include the built-in displacement
	edge softening, thickening, shut-lining and curve piping tools.

*/
class RHRDK_SDK IRhRdkCustomRenderMeshManager
{
public:
	virtual ~IRhRdkCustomRenderMeshManager() { }

	/** Register your custom render mesh provider with RDK.
		\note Ownership of the object is passed to RDK. */
	virtual bool Add(CRhRdkCustomRenderMeshProvider* pProvider) = 0;

	/** Call this method if any of your render meshes change. */
	//RDK_DEPRECATED virtual void Changed(const CRhinoDoc& doc) const = 0;

	enum RDK_DEPRECATED eMeshType { standard, preview, force32bit_type = 0xFFFFFFFF };

	/** Determine if custom render meshes will be built for a particular object.
		\param vp is the viewport being rendered.
		\param pObject is the Rhino object of interest.
		\param doc is the Rhino document this mesh will be used in.
		\param uuidRequestingPlugIn is the UUID of the RDK plug-in requesting the meshes.
		\param pAttributes is the current display attributes for the view we are drawing.  nullptr means this is a rendering, not a viewport.
		\param pUuidRMP is a pointer to the UUID of the sole provider to call. Pass NULL for all providers.
		\return \e true if BuildCustomMeshes() will build custom render mesh(es) for the given object. */
	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                 const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes, const UUID* pUuidRMP=NULL) const = 0;

	/** Returns a bounding box for the custom render meshes for the given object.
		\param vp is the viewport being rendered.
		\param uuidRequestingPlugIn is the UUID of the RDK plug-in requesting the meshes.
		\param pObject is the Rhino object of interest.
		\param doc is the Rhino document this mesh will be used in.
		\param pAttributes is the current display attributes for the view we are drawing.  nullptr means this is a rendering, not a viewport.
		\param pUuidRMP is a pointer to the UUID of the sole provider to call. Pass NULL for all providers.
		\return ON_BoundingBox for the meshes. */
	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                   const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes, const UUID* pUuidRMP=NULL) const = 0;

	/** Build custom render mesh(es) for the given object.
		\param vp is the viewport being rendered.
		\param uuidRequestingPlugIn is the UUID of the RDK plug-in requesting the meshes.
		\param crmInOut is the collection to fill with the custom meshes.
		\param doc is the Rhino document this mesh will be used in.
		\param pAttributes is the current display attributes for the view we are drawing.  nullptr means this is a rendering, not a viewport.
		\param pUuidRMP is a pointer to the UUID of the sole provider to call. Pass NULL for all providers.
		\return \e true if successful, else \e false. */
	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
	                               IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes, const UUID* pUuidRMP=NULL) const = 0;

	/** Get an interface to a custom render mesh provider.
		\param uuidProvider identifies the provider required.
		\return A pointer to the interface of the specified provider, or NULL if not found. */
	virtual CRhRdkCustomRenderMeshProvider* Provider(const UUID& uuidProvider) const = 0;

	/** Ask if a custom render mesh provider is on.
		\param uuidProvider identifies the provider to check.
		\return \e true if the specified provider is on, \e false if not on or not found. */
	virtual bool IsProviderOn(const UUID& uuidProvider) const = 0;

	/* Implementation detail.  Do not call. */
	virtual void OnRhinoObjectChanged(const CRhinoDoc& doc, const CRhinoObject* obj) = 0;

	/* Implementation detail.  Do not call. */
	virtual void OnRhinoDocumentChanged(const CRhinoDoc& doc) = 0;

	/* Forces the object's custom render mesh into the preview cache. */
	virtual bool ForceObjectIntoPreviewCache(const CRhinoObject& obj, const CRhinoDoc& doc) = 0;

	/* Implementation detail.  Do not call. */
	virtual IRhRdkCustomRenderMeshes* PreviousMeshes(const IRhRdkCustomRenderMeshes& crm) const = 0;

	/* Functions to enable and disable the CRM mesh cache. */
	virtual void SetEnableCache(bool) = 0;
	virtual bool CacheEnabled(void) const = 0;

	/* Implementation detail.  Do not call. */
	virtual void SetUserCanceled(bool) = 0;
	virtual bool UserCanceled(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

/** \class CRhRdkCustomRenderMeshProvider

	This class allows clients to provide custom render meshes for individual objects in the scene, or per document.

	This is the way that displacement works (per object) and the custom preview component in Grasshopper (per document) work to supply
	render meshes that can be accessed by all rendering engines, using IRhRdkCustomRenderMeshManager.

	To supply custom render meshes, derive a class from CRhRdkCustomRenderMeshProvider and register it during plug-in startup in
	CRhRdkPlugIn::RegisterExtensions.

*/
class RHRDK_SDK CRhRdkCustomRenderMeshProvider : public CRhRdkExtension
{
public:
	virtual UUID InterfaceId(void) const final override;

	/** Each provider has a unique identifier.
		\return The uuid of this provider. */
	virtual UUID ProviderId(void) const = 0;

	/** \return The name of the provider for UI display. */
	virtual ON_wString Name(void) const = 0;

	/** Determine if custom render meshes will be built for a particular object.
		\param vp is the viewport being rendered.
		\param pObject is the Rhino object of interest.
		\param doc is the Rhino document of interest.
		\param uuidRequestingPlugIn is the UUID of the RDK plug-in requesting the meshes.
		\param pAttributes is the current display attributes for the view we are drawing.  nullptr means this is a rendering, not a viewport.
		\return \e true if BuildCustomMeshes() will build custom render mesh(es) for the given object. */
	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                 const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const = 0;

	/** Returns a bounding box for the custom render meshes for the given object.
		\param vp is the viewport being rendered.
		\param uuidRequestingPlugIn is the UUID of the RDK plug-in requesting the meshes.
		\param pObject is the Rhino object of interest.
		\param doc is the Rhino document of interest.
		\param pAttributes is the current display attributes for the view we are drawing.  nullptr means this is a rendering, not a viewport.
		\return ON_BoundingBox for the meshes. */
	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                   const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const = 0;

	/** Build custom render mesh(es).
		\param vp is the viewport being rendered.
		\param doc is the Rhino document of interest.
		\param uuidRequestingPlugIn is the UUID of the RDK plug-in requesting the meshes.
		\param crmInOut is the collection to fill with the custom meshes.
		\param pAttributes is the current display attributes for the view we are drawing.  nullptr means this is a rendering, not a viewport.
		\param bWillBuildCustomMeshCheck determines whether this function will call the WillBuildCustomMesh method.
		\return \e true if successful, else \e false. */
	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
	                               IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
	                               bool bWillBuildCustomMeshCheck = true) const = 0;

	/* Return true if your custom meshes depend on the view parameter. For example, if your custom render meshes are different when viewed from
	   above than from the side, return true here. */
	virtual bool IsViewDependent(void) const = 0;

	/* Return true if your custom render meshes will be different depending on which plug-in is calling (the uuidRequestingPlugIn parameter) */
	virtual bool IsRequestingPlugInDependent(void) const = 0;

	/* Return true if there will not be a difference between the mesh requested for preview (pAttributes != nullptr) and the mesh requested for a full rendering. */
	virtual bool IsPreviewAndStandardSameMesh(void) const = 0;

	/** Get a parameter by name. */
	virtual CRhRdkVariant GetParameter(const CRhinoObject& object, const wchar_t* wszParamName) const = 0;

	/** Set a parameter by name. */
	virtual void SetParameter(const CRhinoObject& object, const wchar_t* wszParamName, const CRhRdkVariant& value) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) override { return NULL; }
};

/** Helper function to calculate bounding box for custom render meshes. Generally used by custom render mesh provider implementers to easily
    calculate the bounding box of their meshes.  Not the quickest solution though - just extracts the meshes and unions the bounding boxes of the meshes. */
RHRDK_SDK ON_BoundingBox RMPBoundingBoxImpl(const CRhRdkCustomRenderMeshProvider& rmp, const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
                                            const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes);


#endif
