
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkMaterial;
class CRhRdkEnvironment;

/** \class IRhRdkPreviewSceneServer

	This is the interface to RDK's scene server which provides the information necessary to
	render a preview of a material or an environment.

*/
class RHRDK_SDK IRhRdkPreviewSceneServer
{
public:
	virtual ~IRhRdkPreviewSceneServer() { }

	class IObject;

public:
	// Interface to set up the preview scene. For methods, see IRhRdkPreviewSceneServerEx
	enum eGeometry
	{
		geomSphere  = 0,
		geomCuboid  = 1,
		geomCone    = 2,
		geomPyramid = 3,
		geomTorus   = 4,
		geomPlane   = 5,
		geomSelectedObjects = 6, // Not implemented.
		geomMesh    = 7,
		geomForce32bit = 0xFFFFFFFF
	};

public:
	// Interface for sucking the scene details out.

	/** \return A signature value which changes when the scene changes in any way
		that would affect the rendered image. */
	virtual DWORD Signature(void) const = 0;

	/** Set the image used to cache the thumbnail for this scene. */
	virtual void SetCacheImage(const CRhinoDib& image, const UUID& uuidRenderEngine) const = 0;

	/** Return the type id of the content being previewed. */
	virtual const UUID& ContentType(void) const = 0;

	/** Return the kind of the content being previewed. */
	virtual CRhRdkContent::Kinds ContentKind(void) const = 0;

	/** Return the plug-in id of the content being previewed. */
	virtual const UUID& ContentPluginId(void) const = 0;

	/** \return A pointer to the environment that the previewed object is rendered in. */
	virtual const CRhRdkEnvironment* Environment(void) const = 0;

	/** Get information about the view. */
	virtual bool GetView(ON_Viewport& view) const = 0;

	virtual int LightCount(void) const = 0;
	virtual void ResetLightEnumerator(void) const = 0;
	virtual const ON_Light* NextLight(void) const = 0;
	virtual bool Skylighting(void) const = 0;

	/** Support for linear workflow in the preview scene. */
	virtual const IRhRdkLinearWorkflow& LinearWorkflow(void) const = 0;

	RDK_DEPRECATED const class CRhRdkLinearWorkflow& LinearWorkfow(void) const; // Not implemented.

	// Geometry.
	class IGeometry
	{
	public:
		virtual eGeometry Type(void) const = 0;

		/** \return Radius of sphere, radius of base of cone or cylinder. */
		virtual double Radius(void) const = 0;

		/** \return Width of cuboid or base of pyramid. */
		virtual double Width(void) const = 0;

		/** \return Height of cuboid, cone, cylinder or pyramid. */
		virtual double Height(void) const = 0;

		/** Create a mesh of the geometry centered at the origin. */
		virtual void CreateLocalMesh(ON_Mesh& meshOut) const = 0;

		/** Apply UV mapping to the geometry. */
		virtual void ApplyUVMapping(ON_Mesh& meshOut) const = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;
	};

	// Objects.
	class IObject
	{
	public:
		virtual void CreateMesh(ON_Mesh& meshOut) const = 0;
		virtual IGeometry* Geometry(void) const = 0;
		virtual const CRhRdkMaterial* Material(void) const = 0;
		virtual ON_3dPoint Location(void) const = 0;
		virtual bool Normals(void) const = 0;
		virtual void* EVF(const wchar_t*, void*) = 0;
	};

	class ICancellation
	{
	public:
		virtual ~ICancellation() { }

		/** Set the timeout for cancellation in milliseconds. */
		virtual void SetTimeout(int ms) = 0;

		/** Set whether or not the preview job will be auto-cancelled.
			Jobs are auto-cancelled after 20 seconds by default. */
		virtual void SetSupportCancelation(bool) = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;
	};

	virtual ICancellation* CancellationToken() const = 0;
	virtual void SetCancellationToken(ICancellation*) = 0;

	/** Set the scale of the scene. */
	virtual void SetSceneScale(double dScale) = 0;

	enum eRotationType { camera, object, unset };
	virtual void ApplyRotation(double dRotationX, double dRotationY, eRotationType type) = 0;

	/** \return The number of objects in the scene. */
	virtual int ObjectCount(void) const = 0;

	/** Reset the enumerator used by NextObject(). */
	virtual void ResetObjectEnumerator(void) const = 0;

	/** \return A pointer to the next object in the scene, or NULL if no more. */
	virtual const IObject* NextObject(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

class RHRDK_SDK IRhRdkPreviewSceneServerEx : public IRhRdkPreviewSceneServer
{
public: // Interface to set up the preview scene.

	class IObjectEx : public IObject
	{
	public:
		/** Set the location of the object.
			\param loc is the location in world coordinates. */
		virtual void SetLocation(const ON_3dPoint& loc) = 0;
	};

	/** Add an object to the scene.
		\param geom is the geometry to add.
		\param m is the material to use for the object.
		\param bCopy determines if the material should be copied.
		\return A pointer to the added object. */
	virtual IObject* AddObject(eGeometry geom, const CRhRdkMaterial& m, bool bCopy) = 0;

	/** Add a light to the scene.
		\param light is the light to add -- this is copied into a collection.
		\return A pointer to the added light in the collection. */
	virtual ON_Light* AddLight(const ON_Light& light) = 0;

	/*	\param pEnv is a pointer to the environment to set (can be null).
		\param bCopy determines if the environment should be copied.
		\param uuidRdkDocMat is the document the material is associated with when the scene server
		 is used for previewing a material and this call is for the background environment.
		 It is used to provide a document when the default preview environment is used. */
	virtual void SetEnvironment(const CRhRdkEnvironment* pEnv, bool bCopy, const UUID& uuidRdkDocMat=ON_nil_uuid) = 0;

	/** Set the view to use in the preview. */
	virtual void SetView(const ON_Viewport& view) = 0;

	/** Set skylighting on or off. */
	virtual void SetSkylighting(bool b) = 0;

	/** Get the linear workflow; used for setting its parameters. */
	virtual class CRhRdkLinearWorkflowEx& GetLinearWorkflow(void) = 0;

	/** \return \e true if the scene server owns the material, else \e false. */
	virtual bool OwnsMaterial(void) const = 0;

	/** \return \e true if the scene server owns the environment, else \e false.  */
	virtual bool OwnsEnvironment(void) const = 0;
};
