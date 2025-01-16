
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class IRhRdk_XMLSection;
class CRhRdkColor;

/** \class IRhRdkDecal

	This interface allows RDK clients to access decals that are attached to Rhino objects.
	\see CRhRdkObjectDataAccess
*/

RDK_DEPRECATED typedef DWORD RDK_DECALID; // Please use RDK_DECAL_CRC instead.

typedef DWORD RDK_DECAL_CRC;

#define RDK_NIL_DECAL_CRC RDK_DECAL_CRC(0)

class RHRDK_SDK IRhRdkDecal
{
public:
	virtual ~IRhRdkDecal() { }

	enum eMapping
	{
		mapUnset       =-1,
		mapPlanar      = 0, // Planar mapping. Uses projection, origin, up and across vectors (not unitized).
		mapCylindrical = 1, // Cylindrical mapping. Uses origin, up, across, height, radius, latitude start and stop.
		mapSpherical   = 2, // Spherical mapping. Uses origin, up, across, radius, latitude/longitude start and stop.
		mapUV          = 3, // UV mapping.

		mapForce32bit  = 0xFFFFFFFF
	};

	enum eProjection
	{
		projUnset    =-1,
		projForward  = 0, // Project forward.
		projBackward = 1, // Project backward.
		projBoth     = 2, // Project forward and backward.

		projForce32bit = 0xFFFFFFFF
	};

	/** This method is deprecated because the name is misleading. The decal 'id' is not
		an identifier at all, it's a CRC which changes when the decal changes.
		Please change your code to call CRC() instead. */
	RDK_DEPRECATED virtual RDK_DECAL_CRC Id(void) const { return CRC(); }

	/** The decal CRC identifies a decal by its state. Multiple decals which would be exactly the same would
		have the same CRC and are culled from the system. If you store this value with the intention of using
		it to find the decal again later, you must update your stored value whenever the decal state changes.
		You can detect when a decal changes by watching for the legacy OnUserDataTransformed event or the newer
		custom event uuidEventDecalUserDataTransformed. \see CRhRdkEventWatcher */
	virtual RDK_DECAL_CRC CRC(void) const = 0;

	/** \return The decal texture's instance id. */
	virtual UUID TextureInstanceId(void) const;

	/** \return The decal's texture or null on failure.
		\note The returned texture is temporary and only exists as long as
		 the decal object, so you should not store this pointer. You should
		 also not store the instance id. If you need the texture again, you
		 should construct a new decal object and call Texture() each time. */
	virtual const CRhRdkTexture* Texture(void) const = 0;

	/** Set the decal's texture. The decal copies the texture and this
		copy will subsequently be returned by Texture(). */
	virtual void SetTexture(const CRhRdkTexture* pTexture) = 0;

	/** \return The decal's mapping. */
	virtual eMapping Mapping(void) const = 0;

	/** \return The decal's projection. Used only when mapping is planar. */
	virtual eProjection Projection(void) const = 0;

	/** \return \e true if texture is mapped to inside of sphere or cylinder, else \e false.
		 Used only when mapping is cylindrical or spherical. */
	virtual bool MapToInside(void) const = 0;

	/** \return The decal's transparency in the range 0 to 1. */
	virtual double Transparency(void) const = 0;

	/** \return The origin of the decal in world space.
		 Not used when the mapping is UV. */
	virtual ON_3dPoint Origin(void) const = 0;

	/** \return The 'up' vector of the decal. For planar mapping the length of the vector is relevant.
		 Not used when the mapping is UV.
		 For cylindrical and spherical mapping, the vector is unitized. */
	virtual ON_3dVector VectorUp(void) const = 0;

	/** \return The 'across' vector of the decal. For planar mapping the length of the vector is relevant.
		 Not used when the mapping is UV.
		 For cylindrical and spherical mapping, the vector is unitized. */
	virtual ON_3dVector VectorAcross(void) const = 0;

	/** \return The height of the decal. Only used when mapping is cylindrical. */
	virtual double Height(void) const = 0;

	/** \return The radius of the decal. Only used when mapping is cylindrical or spherical. */
	virtual double Radius(void) const = 0;

	/** This method is deprecated in favor of GetHorzSweep(). \see GetHorzSweep(). */
	RDK_DEPRECATED virtual double LatStart(void) const;

	/** This method is deprecated in favor of GetHorzSweep(). \see GetHorzSweep(). */
	RDK_DEPRECATED virtual double LatEnd(void) const;

	/** This method is deprecated in favor of GetVertSweep(). \see GetVertSweep(). */
	RDK_DEPRECATED virtual double LonStart(void) const;

	/** This method is deprecated in favor of GetVertSweep(). \see GetVertSweep(). */
	RDK_DEPRECATED virtual double LonEnd(void) const;

	/** Gets the start and end angles of the decal's 'horizontal sweep' (these are angles of longitude in radians).
		\note These correspond to the old methods LatStart() and LatEnd() which were badly named.
		Only used when mapping is cylindrical or spherical. */
	virtual void GetHorzSweep(double& sta, double& end) const = 0;

	/** Gets the start and end angles of the decal's 'vertical sweep' (these are angles of latitude in radians).
		\note These correspond to the old methods LonStart() and LonEnd() which were badly named.
		Only used when mapping is spherical. */
	virtual void GetVertSweep(double& sta, double& end) const = 0;

	/** The UV bounds of the decal. Only used when mapping is UV. */
	virtual void UVBounds(double& dMinUOut, double& dMinVOut, double& dMaxUOut, double& dMaxVOut) const = 0;

	/** Get custom data associated with this decal.
		\return A pointer to an interface from which you can retrieve custom data,
		 or NULL if no custom data exists on the decal for the current Rhino renderer. */
	virtual const IRhRdk_XMLSection* CustomData(void) const = 0;

	/** Blend color with the decal color at a given point.
		\param ptPoint is the point in space or, if the decal is uv-mapped, the uv-coordinate of that point.
		\param vecNormal is the face normal of the given point.
		\param colInOut is the color to blend the decal color to.
		\param uvOut is the UV on the texture that the color point was read from
		\return \e true if the given point hits the decal, else \e false. */
	virtual bool Color(const ON_3dPoint& ptPoint, const ON_3dVector& vecNormal, CRhRdkColor& colInOut, ON_2dPoint& uvOut) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	static bool GetTextureMapping(const IRhRdkDecal& decal, ON_TextureMapping& mappingOut);
};

class RHRDK_SDK IRhRdkDecal2 : public IRhRdkDecal
{
public:
	//This is mainly for decal access from the change queue
	virtual ON__UINT32 TextureRenderCRC(const CRhRdkContent::CRenderCRCFlags& rcrcFlags) const = 0;
	virtual ON__UINT32 TextureRenderCRC(const CRhRdkContent::CRenderCRCFlags& rcrcFlags, const IRhRdkLinearWorkflow& lw) const = 0;
};

RHRDK_SDK unsigned int RhRdkGetTextureRenderCRC(const CRhinoDoc& doc, const ON_Decal& decal, ON__UINT64 rcf, const ON_LinearWorkflow* lw);
RHRDK_SDK bool RhRdkGetDecalColor(const CRhinoDoc& doc, const ON_Decal& decal, const ON_3dPoint& ptPoint, const ON_3dVector& vecNormal, ON_4fColor& colInOut, ON_2dPoint& uvOut);
