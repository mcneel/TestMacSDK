
#pragma once // Public SDK header - OBSOLETE - Not implemented.

// NOTE: These classes are obsolete. This file only exists for backward compatibility. These classes are not implemented.

#include "RhRdkPostEffectPlugIn.h"

/** \class IRhRdkPostEffectImage

	Obsolete.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkPostEffectImage
{
public:
	virtual ~IRhRdkPostEffectImage() { }

	/** Query whether or not information is available for a given channel.
		\param uuidChannel specifies the channel of interest.
		\return \e true if the image contains information for the specified channel, else \e false. */
	virtual bool ChannelInformationAvailable(const UUID& uuidChannel) const = 0;

	/** Get minimum and maximum values from a channel in the image.
		\param uuidChannel specifies the channel of interest.
		\param fMinOut accepts the minimum value.
		\param fMaxOut accepts the maximum value.
		\return \e true if the image contains information for the specified channel, else \e false. */
	virtual bool GetMinMaxValuesFromChannel(const UUID& uuidChannel, float& fMinOut, float& fMaxOut) const = 0;

	/** Get an RGB pixel from the image.
		\param x is the horizontal pixel position.
		\param y is the vertical pixel position.
		\param fRedOut accepts the red channel value.
		\param fGreenOut accepts the green channel value.
		\param fBlueOut accepts the blue channel value.
		\return \e true if successful, else \e false. */
	virtual bool GetDIBPixel(int x, int y, float& fRedOut, float& fGreenOut, float& fBlueOut) const = 0;

	/** Get a channel value from the image.
		\param uuidChannel specifies the channel of interest.
		\param x is the horizontal pixel position.
		\param y is the vertical pixel position.
		\param fValueOut accepts the channel value.
		\return \e true if successful, else \e false. */
	virtual bool GetChannelValue(const UUID& uuidChannel, int x, int y, float& fValueOut) const = 0;

	/** Get multiple channel values from the image.
		\param aUuidChannels is an array specifying the channels of interest.
		\param x is the horizontal pixel position.
		\param y is the vertical pixel position.
		\param aValuesOut accepts the channel values in the same order as aUuidChannels.
		\param iNumberOfValues is the number of channels to retrieve. aUuidChannels and aValuesOut
		 must have this many elements.
		\return \e true if successful, else \e false. */
	virtual bool GetChannelValues(const UUID* aUuidChannels, int x, int y, float* aValuesOut, int iNumberOfValues) const = 0;

	/** \return A pointer to the bits in the image DIB. */
	virtual BYTE* Bits(void) const = 0;	//Deprecated, do not use.

	/** \return The width of the image. */
	virtual DWORD Width(void) const = 0;

	/** \return The height of the image. */
	virtual DWORD Height(void) const = 0;

	/** \return The length in bytes (DWORD-aligned) of one image pixel line. */
	virtual DWORD LineLength(void) const = 0;

	/** \return The number of bits in each image pixel. */
	virtual DWORD BitsPerPixel(void) const = 0;

	/** \return \e true if the image is inverted (bottom-up DIB), else \e false. */
	virtual bool Inverted(void) const = 0;

	/** \return The length in bytes of one image pixel line. */
	virtual eRhRdkPepByteOrder ByteOrder(void) const = 0; // See enum in RhRdkPostEffectPlugIn.h	

	/** Called at the start of post-processing the image.
		\param wszPlugIn is the name of the post-effect plug-in. */
	virtual void OnProcessImageBegin(const wchar_t* wszPlugIn) const = 0;

	/** Called at the end of post-processing the image.
		\param wszPlugIn is the name of the post-effect plug-in. */
	virtual void OnProcessImageEnd(const wchar_t* wszPlugIn) const = 0;

	/** Called periodically during post-processing of the image.
		\param wszPlugIn is the name of the post-effect plug-in.
		\param fProgress is the amount of progress made so far (0.0 to 1.0). */
	virtual void OnProcessImageProgress(const wchar_t* wszPlugIn, float fProgress) const = 0;

	/** Called periodically during post-processing the image to determine if the user cancelled the process.
		\return \e true if the user cancelled the process, else \e false. */
	virtual bool IsProcessInterrupted(void) const = 0;

	/** Allows the user to pick a point on the image.
		\param x receives the picked horizontal pixel position.
		\param y receives the picked vertical pixel position.
		\param hCursor is obsolete and is not used.
		\return \e true if successful, else \e false. */
	virtual bool PickPointOnImage(int& x, int& y, HCURSOR hCursor) const = 0;

	/** Allows the user to pick a rectangle on the image.
		\param rectOut receives the picked rectangle.
		\param hCursor is obsolete and is not used.
		\return \e true if successful, else \e false. */
	virtual bool PickRectangleOnImage(ON_4iRect& rectOut, HCURSOR hCursor) const = 0;

	/** Updates the image by applying the post-effects to it. */
	virtual bool UpdateImage(void) = 0;

	/** \return \e true if picking a point on this image is implemented, else \e false. */
	virtual bool IsPickPointOnImageImplemented(void) const = 0;

	/** \return \e true if picking a rectangle on this image is implemented, else \e false. */
	virtual bool IsPickRectangleOnImageImplemented(void) const = 0;

	virtual IRhRdkRenderWindow::IChannel* NewChannel(const UUID& uuidChannel) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};

RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkPostEffectImageEx : public IRhRdkPostEffectImage
{
public:
	/** \return A pointer to the bits in the image DIB. */
	virtual BYTE* BitsEx(eRhRdkPepByteOrder) const = 0;
};
