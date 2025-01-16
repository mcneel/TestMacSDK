
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkColor;
class CRhRdkTexture;
class CRhRdkContent;

/** \class IRhRdkTextureEvaluator

	This is the interface to a lightweight object capable of evaluating texture color
	throughout uvw space.
*/

class RHRDK_SDK IRhRdkTextureEvaluator
{
public:
	/** You must implement this method as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** Get the color of the texture at a particular point in uvw space.
		May be called from within a rendering shade pipeline.
		\param uvw is the point for which to evaluate the texture.
		\param duvwdx is a ray differential.
		\param duvwdy is a ray differential.
		\param colOut receives the texture color.
		\note For ray differentials see Pharr Humphreys, "Physically Based Rendering", chapter 11.
		\return \e true if the color was retrieved successfully, else \e false.
		 It is important to only return \e false if the function fails catastrophically. A black
		 value outside some threshold is not a failure case. If the first call returns \e false,
		 the entire evaluation will fail. If you return \e false you must leave colOut untouched. */
	virtual bool GetColor(const ON_3dPoint& uvw, const ON_3dVector& duvwdx,
	                      const ON_3dVector& duvwdy, CRhRdkColor& colOut, void* pvData=NULL) const = 0;

	/** Call this function before calling GetColor for the first time. Ideally, this should
		be on the main thread, but you can also call it on a worker thread as long as you
		are sure that Initialize() or GetColor() cannot be called at the same time on another thread. */
	virtual bool Initialize(void) = 0;

	/** \class IRhRdkTextureEvaluator::IChildCallback

		This class allows a plug-in developer to handle the color requests for the children of this
		texture evaluator.  For example, if this is a texture evaluator for a blend texture, the developer will
		be given the opportunity to return the colors for each of the two blend textures instead of the standard method
		which would involve creating texture evaluators for those textures using pChild->NewTextureEvaluator.  This
		system is intended to be used by developers of textures that do not support texture evaluation.
	*/
	class RHRDK_SDK IChildCallback
	{
	public:
		/** You must implement this method as \code delete this; \endcode */
		virtual void DeleteThis(void) = 0;

		/* \return The render engine id of this child callback object. */
		virtual UUID RenderEngineId(void) const = 0;

		/* Called for each child during the call to IRhRdkTextureEvaluator::RegisterChildCallback.
			\return A unique index for each child, or -1 to indicate that this child should be handled in the standard way (NewTextureEvaluator). */
		virtual int InitializeChild(const CRhRdkContent* pChild, void* pvData) = 0;

		/* 	\param iIndex is the index you returned from InitializeChild().
			\param pvData is pvData that you returned from InitializeChild() .
			\param uvw    is the point for which to evaluate the texture..
			\param duvwdx is a ray differential. See IRhRdkTextureEvaluator::GetColor above.
			\param duvwdy is a ray differential. See IRhRdkTextureEvaluator::GetColor above.
			\param colOut receives the output color.
			\return \e true if successful, else \e false. */
		virtual bool GetChildColor(int iIndex, void* pvData, const ON_3dPoint& uvw, const ON_3dVector& duvwdx, 
							  const ON_3dVector& duvwdy, CRhRdkColor& colOut) const = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;

	protected:
		virtual ~IChildCallback() { }
	};

	/* Call this method to register a child callback object for a specific texture evaluator.
		\param texture is the texture for which this evaluator was created (required to enumerator the children)
		\param pvData is a pointer to your own private data.
		\return \e true if successful, else \e false. */
	virtual bool RegisterChildCallback(IChildCallback* pChildCallback, const CRhRdkTexture& texture, void* pvData) = 0;

	/** \return A pointer to the child callback object or NULL if not registered. */
	virtual const IChildCallback* ChildCallback(void) const = 0;

	class RHRDK_SDK CEvalFlags final
	{
	public:
		enum : unsigned int
		{
			efNormal                  = 0x0000,
			efDisableFiltering        = 0x0001, // Force the texture to be evaluated without filtering (the implementation gets to decide what that means).
			efDisableLocalMapping     = 0x0002, // Force the texture to be evaluated without local mapping - ie, Repeat(1.0, 1.0, 1.0), Offset(0.0, 0.0 0.0), Rotation(0.0, 0.0, 0.0).
			efDisableAdjustment       = 0x0004, // Force the texture to be evaluated without post-adjustment (greyscale, invert, clamp etc)
			efDisableProjectionChange = 0x0008, // Force the texture to be evaluated without any projection modification (ProjectionIn == ProjectionOut)
		};

		CEvalFlags(unsigned int f=efNormal);

		unsigned int Flags(void) const;
		void AddFlag(unsigned int f);
		bool HasFlag(unsigned int f) const;

	private:
		unsigned int m_Flags;
		void* m_pReserved;
	};

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	/* Call this method write out the 0.0 - 1.0 UV space of this evaluator to an array of bytes in the form RGBA (4 bytes per pixel).
		\param size is the pixel size for the output image.
		\param buffer is the pre-allocated buffer to receive the pixel data.  It must be at least 4 * size.cx * size.cy * sizeof(byte) bytes.  If nullptr, this function is a tester - see below.
		\return \e true if successful, or if the buffer is a nullptr, whether the function will succeed if a real buffer is passed in, else \e false. */
	bool WriteByteArray(const ON_2iSize& size, unsigned char* buffer) const;

	/* Call this method write out the 0.0 - 1.0 UV space of this evaluator to an array of floats in the form RGBA (16 bytes per pixel).
		\param size is the pixel size for the output image.
		\param buffer is the pre-allocated buffer to receive the pixel data.  It must be at least 4 * size.cx * size.cy * sizeof(float) bytes.  If nullptr, this function is a tester - see below.
		\return \e true if successful, or if the buffer is a nullptr, whether the function will succeed if a real buffer is passed in, else \e false. */
	bool WriteFloatArray(const ON_2iSize& size, float* buffer) const;

protected:
	virtual ~IRhRdkTextureEvaluator() { }
};

/** \class CRhRdkTextureEvaluator

	This is the default base class implementation of IRhRdkTextureEvaluator.
*/

class RHRDK_SDK CRhRdkTextureEvaluator : public CRhRdkObject, public IRhRdkTextureEvaluator
{
public:
	/** Implements IRhRdkTextureEvaluator::RegisterChildCallback(). */
	virtual bool RegisterChildCallback(IChildCallback* pChildCallback, const CRhRdkTexture& texture, void* pvData) override;

	/** Implements IRhRdkTextureEvaluator::ChildCallback(). */
	virtual const IChildCallback* ChildCallback(void) const override;

	/** Default implementation calls GetColor(ON_origin...). */
	virtual bool Initialize(void) override;

	struct WRITE_TO_BYTE_DATA
	{
		ON_2iSize size;
		unsigned char* buffer;
	};

	struct WRITE_TO_FLOAT_DATA
	{
		ON_2iSize size;
		float* buffer;
	};

	virtual void* EVF(const wchar_t*, void*) override;

	bool FilteringOn(void) const;
	bool LocalMappingOn(void) const;
	bool AdjustmentOn(void) const;
	bool ProjectionChangeOn(void) const;
	const CEvalFlags& EvaluatorFlags(void) const;

protected:
	virtual ~CRhRdkTextureEvaluator();

	/* Constructor.*/
	CRhRdkTextureEvaluator(IRhRdkTextureEvaluator::CEvalFlags ef);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
