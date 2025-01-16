
#pragma once

#include "RhRdkObject.h"

/** \class CRhRdkTextureCache

	This object gives access to RDK's texture cache. Textures are cached on disk using
	file names that are derived from a combination of the texture size and a CRC of
	the parameters that were used to render the texture.
*/

class RHRDK_SDK CRhRdkTextureCache : public CRhRdkObject
{
public:
	/** Instantiate this object on the stack.
		\param size is the size of the texture in pixels.
		\param dwCRC is a CRC of all parameters that were used to create the texture imagery. */
	ON_DEPRECATED CRhRdkTextureCache(const ON_2iSize& size, DWORD dwCRC);

	CRhRdkTextureCache(const ON_2iSize& size, DWORD dwCRC, const wchar_t* wszTag);

	virtual ~CRhRdkTextureCache();

	class Location
	{
	public:
		static Location MemoryOnly;
		static Location FilesOnly;
		static Location Both;

		bool IncludeFiles(void) const;
		bool IncludeMemory(void) const;

	private:
		Location(unsigned int f);
		const unsigned int m_flags;
	};

	/** Get the full path to the texture file on disk. */
	ON_DEPRECATED virtual ON_wString Filename(void) const;

	virtual_su ON_wString Filename(Location locationFileIsExpected) const;

	/** \return \e true if there is a cached texture on disk for this texture size and CRC. */
	ON_DEPRECATED virtual bool HasCachedTexture(void) const;

	virtual_su bool HasCachedTexture(Location locationsToCheck) const;

	/** Add a texture to the cache for this texture size and CRC.
		If the texture file already exists it will be overwritten. */
	ON_DEPRECATED virtual void AddTexture(const CRhinoDib& dib);

	virtual_su ON_wString AddTexture(const CRhinoDib& dib, Location locationsToAddTo);

	//For avoiding race conditionsa between the check and the population of the cache,
	void Lock() const;
	void Unlock() const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
