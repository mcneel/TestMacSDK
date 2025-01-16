
#pragma once // Public SDK header - OBSOLETE - Not implemented.

// NOTE: These classes are obsolete and have been replaced by IRhRdkPostEffectPlugIn.
//       This file only exists for backward compatibility. These classes are not implemented.

#include "RhRdkObject.h"
#include "RhRdkColor.h"
#include "IRhRdkRenderWindow.h"

class CLBP_XMLNode;

enum eRhRdkPepByteOrder
{
	PPNO_NONE = 0,
	PPBO_RGB  = 1,
	PPBO_BGR  = 2,
	PPBO_RGBA = 3,
	PPBO_BGRA = 4,
	PPBO_ABGR = 5,
    PPBO_ARGB = 6,
	force32bit_pepbo=0xFFFFFFFF
};

#define PPID_RED					IRhRdkRenderWindow::chanRed
#define PPID_GREEN					IRhRdkRenderWindow::chanGreen
#define PPID_BLUE					IRhRdkRenderWindow::chanBlue
#define PPID_LUMINANCE_RED			IRhRdkRenderWindow::chanLuminanceRed
#define PPID_LUMINANCE_GREEN		IRhRdkRenderWindow::chanLuminanceGreen
#define PPID_LUMINANCE_BLUE			IRhRdkRenderWindow::chanLuminanceBlue
#define PPID_BK_LUM_RED				IRhRdkRenderWindow::chanBackgroundLuminanceRed
#define PPID_BK_LUM_GREEN			IRhRdkRenderWindow::chanBackgroundLuminanceGreen
#define PPID_BK_LUM_BLUE			IRhRdkRenderWindow::chanBackgroundLuminanceBlue
#define PPID_DISTANCE_FROM_CAMERA   IRhRdkRenderWindow::chanDistanceFromCamera
#define PPID_ALPHA_TRANSPARENCY     IRhRdkRenderWindow::chanAlpha
#define PPID_MATERIAL_IDS			IRhRdkRenderWindow::chanMaterialIds
#define PPID_OBJECT_IDS				IRhRdkRenderWindow::chanObjectIds
#define PPID_WIREFRAME				IRhRdkRenderWindow::chanWireframeRGBA

#define GetMonochromeValue(/*float* [3]*/ values) 0.3f * (values)[0] + 0.59f * (values)[1] + 0.11f * (values)[2]

/** \class CRhRdkPostEffectPlugIn

	This class is deprecated in favor of IRhRdkPostEffectPlugIn.

	Note: This is kept for backward compatibility. The New PEPs will not use this.

	\see IRhRdkPostEffectPlugIn

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkPostEffectPlugIn : public CRhRdkObject
{
public:
	CRhRdkPostEffectPlugIn();
	virtual ~CRhRdkPostEffectPlugIn();

	/** \return The maximum luminance found in the RGB channels of the current image. */
	virtual double GetMaxLuminance(void) const;

	/** Called when the plug-in is loaded. */
	virtual bool OnLoadPlugIn(void)	= 0;

	/** \return The localized name of the plug-in (e.g., "Fog"). */
	virtual ON_wString PlugInName(void) const = 0;

	/** \return The unique identifier of the plug-in. */
	virtual UUID PlugInID(void) const = 0;

	/* \return The state icon for the plug-in depending on whether it is turned on or off. */
	virtual HICON Icon(bool bOn) const;

	/** Query whether or not the plug-in can work with the current image.
		\note Use the channel query functions to determine if you can work on this image.
		\return \e true if the plug-in can work with the current image, else \e false. */
	virtual bool WorksWithCurrentImage(void) const = 0;

	/* \return The on/off state of the plug-in. */
	virtual bool On(void) const;

	/* Set the on/off state of the plug-in. */
	virtual void SetOn(bool bOn=true);

	/** Implement this method to display the plug-in's options (AKA Properties) dialog.
		\return \e true if the user clicked OK, else \e false. */
	virtual bool DisplayOptions(HWND hWndParent) = 0;

	/** Implement this method to have the plug-in process the image.
		Called just after PreProcessImage() is called, but \e only if it succeeded.
		\return \e true if successful, \e false if cancelled by the user. */
	virtual bool ProcessImage(void) = 0;

	/** Called just before ProcessImage() is called.
		If you override this method, <b>please be sure to call the base class</b>.
		\return \e true if successful, \e false if cancelled by the user. */
	virtual bool PreProcessImage(void);

	/** Called just after ProcessImage() is called, but \e only if it succeeded.
		If you override this method, <b>please be sure to call the base class</b>.
		\return \e true if successful, \e false if cancelled by the user. */
	virtual bool PostProcessImage(void);

	/** Implement this method to have the plug-in read its state from a buffer.
		\param pBuffer is the buffer containing data previously written by WriteState().
		\param size is the number of bytes in the buffer.
		\return \e true if successful, else \e false. */
	virtual bool ReadState(const void* pBuffer, size_t size) = 0;

	/** Implement this method to have the plug-in write its state to a buffer.
		\param pBuffer is the buffer to fill with state information. If pBuffer is NULL
		 the implementation must set \b size to the buffer size that would be required to
		 write the plug-in state.
		\param size is the physical number of bytes that the buffer can accomodate.
		 This parameter also accepts the logical number of bytes written to the buffer.
		 This will be more than \b size if the buffer was not big enough.
		 If pBuffer is NULL you can pass zero in \b size to get the number of bytes required
		 to write the plug-in state.
		\note The implementor is responsible for ensuring that the buffer is not overrun.
		\return \e true if successful, else \e false. */
	virtual bool WriteState(void* pBuffer, size_t& size) const = 0;

	/** Reset the plug-in's state to factory defaults. */
	virtual void SetToDefault(void) = 0;

	/** Read the state from the document defaults. */
	virtual bool ReadFromDocumentDefaults(const CRhinoDoc& doc);

	/** Write the state to the document defaults. */
	virtual bool WriteToDocumentDefaults(CRhinoDoc& doc) const;

protected:
	/** Called to update the image's progress display.
		\param fProgress is the amount of progress so far, 0.0f - 1.0f. */
	virtual void OnProcessImageProgress(float fProgress) const;

	/** Determine if the user cancelled the process.
		\return \e true if the user cancelled the process, else \e false. */
	virtual bool IsProcessInterrupted(void) const;

public:
	/** Query whether or not information is available in the image for a given channel.
		\param uuidChannel specifies the channel of interest.
		\return \e true if the image contains information for the specified channel, else \e false. */
	virtual bool ChannelInformationAvailable(const UUID& uuidChannel) const;

	/** Get minimum and maximum values from a channel in the image.
		\param uuidChannel specifies the channel of interest.
		\param fMinOut accepts the minimum value.
		\param fMaxOut accepts the maximum value.
		\return \e true if the image contains information for the specified channel, else \e false. */
	virtual bool GetMinMaxValuesFromChannel(const UUID& uuidChannel, float& fMinOut, float& fMaxOut) const;

	/** Get an RGB pixel from the image.
		\param x is the horizontal pixel position.
		\param y is the vertical pixel position.
		\param fRedOut accepts the red channel value.
		\param fGreenOut accepts the green channel value.
		\param fBlueOut accepts the blue channel value.
		\return \e true if successful, else \e false. */
	virtual bool GetDIBPixel(int x, int y, float& fRedOut, float& fGreenOut, float& fBlueOut) const;

	// Use these functions to get the image channel information.
	// The array based version may be quicker for getting several values depending on the implementation.

	/** Get a channel value from the image.
		\param uuidChannel specifies the channel of interest.
		\param x is the horizontal pixel position.  Not range checked.
		\param y is the vertical pixel position.  Not range checked.
		\param fValueOut accepts the channel value.
		\return \e true if successful, else \e false. */
	virtual bool GetChannelValue(const UUID& uuidChannel, int x, int y, float& fValueOut) const;
	virtual bool GetChannelValue(const UUID& uuidChannel, int x, int y, ON__UINT32& iValueOut) const;

	/** Get a new channel to pass into the fast version of GetChannelValue below.
		Remember, you must call Close() on this pointer when you are done with it in this case.
		\return \e true if the channel exists, else \e false. */
	virtual IRhRdkRenderWindow::IChannel* NewChannel(const UUID& uuidChannel) const;

	/** Much faster access to display channels than the UUID version.  However, this version does not
		automatically adjust the DISTANCE_FROM_CAMERA channel values, so if you are using that channel
		you will need to add fValueOut = (fValueOut < -1e20f || fValueOut > 1e20f) ? -1.f : std::abs(fValueOut);
		\param channel is the channel returned from NewChannel.
		\param x is the horizontal pixel position.  Not range checked.
		\param y is the vertical pixel position.  Not range checked.
		\param fValueOut accepts the channel value.*/
	void GetChannelValue(IRhRdkRenderWindow::IChannel& channel, int x, int y, float& fValueOut) const;
	void GetChannelValue(IRhRdkRenderWindow::IChannel& channel, int x, int y, ON__UINT32& iValueOut) const;

	/** Get multiple channel values from the image.
		\param aUuidChannels is an array specifying the channels of interest.
		\param x is the horizontal pixel position.
		\param y is the vertical pixel position.
		\param aValuesOut accepts the channel values in the same order as aUuidChannels.
		\param iNumberOfValues is the number of channels to retrieve. aUuidChannels and aValuesOut
		 must have this many elements.
		\return \e true if successful, else \e false. */
	virtual bool GetChannelValues(const UUID* aUuidChannels, int x, int y, float* aValuesOut, int iNumberOfValues) const;

	/** This exposes the raw bits of the DIB to which you are writing changes.
		Remember, previous plug-ins may have changed the DIB already, so don't "undo" their changes by writing
		color information to every pixel - make the changes to the information you already find there.
		\return A pointer to the bits in the image DIB. */
	virtual BYTE* Bits(void) const;

	/** \return The width of the image. */
	virtual DWORD Width(void) const;

	/** \return The height of the image. */
	virtual DWORD Height(void) const;

	/** \return The length in bytes (DWORD-aligned) of one image pixel line. */
	virtual DWORD LineLength(void) const;

	/** \return The number of bits in each image pixel. */
	virtual DWORD BitsPerPixel(void) const;

	/** \return \e true if the image is inverted (bottom-up DIB), else \e false. */
	virtual bool Inverted(void) const;

	/** \return The byte order of the dib pixels. */
	virtual eRhRdkPepByteOrder ByteOrder(void) const;

public:
	// Units support - use these functions in your interface to properly support units display.

	/** Parse a number in string form to get a double value. */
	virtual double ParseNumber(const wchar_t* wszNumber) const;

	/** Format a numeric value to get a string representation.
		\return \e true if successful, else \e false. */
	virtual bool FormatNumber(double dNumber, wchar_t* wszNumber, DWORD dwSize) const;

public:
	/** Display a color picker.
		\return \e true if successful, \e false if the user cancelled. */
	virtual bool ColorPickerUI(CRhRdkColor& colInOut, HWND pParent) const;

	/** Allows the user to pick a point on the image.
		\param x receives the picked horizontal pixel position.
		\param y receives the picked vertical pixel position.
		\param hCursor is the cursor to display during picking. If this is NULL, a default eye dropper will be used.
		\return \e true if successful, else \e false. */
	virtual bool PickPointOnImage(int& x, int& y, HCURSOR hCursor) const;

	/** Allows the user to pick a rectangle on the image by dragging a lasso.
		\param rectOut receives the picked rectangle.
		\param hCursor is the cursor to display during picking. If this is NULL, a default crosshair will be used.
		\return \e true if successful, else \e false. */
	virtual bool PickRectangleOnImage(ON_4iRect& rectOut, HCURSOR hCursor) const;

	/** Updates the image by applying the post-effects to it. */
	virtual bool UpdateImage(void);

	/** \return \e true if color picker is implemented, else \e false. */
	virtual bool IsColorPickerImplemented(void) const;

	/** \return \e true if picking a point on this image is implemented, else \e false. */
	virtual bool IsPickPointOnImageImplemented(void) const;

	/** \return \e true if picking a rectangle on this image is implemented, else \e false. */
	virtual bool IsPickRectangleOnImageImplemented(void) const;

	/** Get a localized string from the host application.
		The localized string should be placed into the buffer.
		The plug-in should supply the English version of the string in the buffer when calling
		this function. A string code can also be optionally passed.
		\return \e true if the string was localized, else \e false. */
	virtual bool GetLocalizedString(wchar_t* wszBuffer, DWORD dwNumChars, UINT uStringId = -1) const;

	/** \return Locale LCID for the current host (e.g., 1033 is US English). */
	virtual int GetHostLocale(void) const;

	/** Display help for the specified topic.
		\return \e true if successful, else \e false. */
	virtual bool ProvideHelp(const wchar_t* wszTopic);

	/** \internal For RDK internal use only. */
	class Manager;

	/** \internal For RDK internal use only. */
	virtual void SetManager(Manager* pManager);

	/** Get the offsets to R, G and B in the bits. For backward compatibility only. \see GetRGBAOffset(). */
	virtual void GetRGBOffset(int& iR, int& iG, int& iB) const;

	/** Get the offsets to R, G, B and A in the bits. */
	virtual void GetRGBAOffset(int& iR, int& iG, int& iB, int& iA) const;

	virtual_su bool CanDisplayOptions(void) const; // [SDK_UNFREEZE]

	// L"CanDisplayOptions" returns true by default.
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override; // [SDK_UNFREEZE]

public:
	/** \internal For RDK internal use only. */
	bool ReadFromNode(const CLBP_XMLNode& node);

	/** \internal For RDK internal use only. */
	bool WriteToNode(CLBP_XMLNode& node) const;

public:
	/** \internal For RDK UI internal use only. */
	virtual bool DisplayPreview(const class IRhRdkPEPDlg&);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkPostEffectPlugInEx : public CRhRdkPostEffectPlugIn
{
public:
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) = 0;
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const = 0;
};
