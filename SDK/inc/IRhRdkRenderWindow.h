
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

#if defined(ON_RUNTIME_APPLE)
#import <Metal/Metal.h>
#endif

class CRhRdkImageAdjust; // OBSOLETE -- not implemented.
class CRhRdkRenderPlugIn;
class CRhRdkRenderingInfo;
class IRhRdkRenderUI;

using GPU_SURFACE = ON__UINT_PTR;

/** Deprecated in favor of IRhRdkRenderWindow::RenderResult */
enum RDK_DEPRECATED eRenderSuccessCode { rsc_completed, rsc_failed };

enum class ComponentOrder : int // [SDK_UNFREEZE] Rename to RhRdkComponentOrder.
{
	Irrelevant = 0, // Used in single-value channels.

	// These can be used with the RGB or the RGBA channel. Both channels are identical,
	// the only difference being the channel id. They can also be used with the AlbedoRGB
	// channel, but A will be ignored because there is no alpha in the albedo channel.

	RGBA = 1, // This is the default (to match Rhino 5).
	ARGB = 2,
	RGB  = 3, // This will only access 3 components, even in the case of the RGBA channel.
	BGR  = 4, // This will only access 3 components, even in the case of the RGBA channel.
	ABGR = 5,
	BGRA = 6,

	// These are just for readability when using the Normal XYZ channel.
	XYZ = RGB,
	ZYX = BGR,
};

/** \class IRhRdkRenderSession

	This is the interface to a render session.

	\note This interface is implemented in RDK in a thread-safe manner so that you can
	 call the methods from any thread your renderer happens to be running in.

	\see CRhRdkRenderSession   \see CRhRdkSdkRenderImpl    \see IRhRdkAsyncRenderContext
	\see IRhRdkRenderWindow    \see IRhRdkRenderUI
	\see RhRdkFindRenderWindow \see RhRdkFindRenderSession
*/
class RHRDK_SDK IRhRdkRenderSession
{
public:
	virtual ~IRhRdkRenderSession() { }

	/** \return The render session id of this render session. */
	virtual UUID RenderSessionId(void) const = 0;

	/** \return The render engine id of this render session. */
	virtual UUID RenderEngineId(void) const = 0;

	/** \return The document serial number associated with this render session. */
	virtual unsigned int RhinoDocSerialNumber(void) const = 0;

	/** \return The SDK Render stack object associated with this render session.
		\note For async renderers, this method may return null because the SDK Render
		 object goes off the stack right after rendering begins. It is not safe to
		 call this method from your async rendering thread because the object may
		 go off the stack and get deleted after the thread gets a pointer to it. */
	virtual class CRhRdkSdkRender* SdkRender(void) const = 0;

	/** \return The async render context if the render session is asynchronous, else null. */
	virtual class IRhRdkAsyncRenderContext* AsyncRenderContext(void) const = 0;

	/** Set the async render context if the render session is asynchronous.
		RDK will take ownership of the async render context. */
	virtual void SetAsyncRenderContext(class IRhRdkAsyncRenderContext* pARC) = 0;

	/** \return The render window associated with this render session. */
	virtual class IRhRdkRenderWindow& RenderWindow(void) const = 0;

	enum class Status : unsigned int
	{
		Quiescent,    // The render session has not started rendering and in the case of an inactive render frame, never will.
		Initializing, // The render session is initializing; it is in this state between the start of Render() and the end of StartRendering().
		Rendering,    // Rendering has begun.
		Paused,       // Rendering is paused.
		Completed,    // Rendering has completed successfully.
		Canceled,     // Rendering was canceled. If the user canceled by closing the frame window, this status is never set.
		Aborted,      // Rendering was aborted. This happens when an async render must abort because the document is closed.
		Failed,       // Rendering failed (but not because it was canceled).
		Reusing,      // OBSOLETE. Not used.
		Disposed,     // The render session has been disposed of and is awaiting deletion.
		Deleted,      // The render session is being deleted.
		Waiting,      // The render session is waiting for user input (used by the RenderWindow command).
	};

	virtual Status GetStatus(void) const = 0;
	virtual void SetStatus(Status s) = 0;

	/** Stop rendering as soon as possible. Should not return until rendering has stopped. */
	virtual void StopRendering(void) = 0;

	/** \return The number of seconds taken by rendering (so far if not finished). */
	virtual int ElapsedSeconds(void) const = 0;

	/** \return \e true if the last rendering is considered 'valuable', else \e false.
		When closing valuable renderings, the user will be asked if s/he wants to save. */
	virtual bool IsLastRenderingValuable(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

public: // Internal expert methods for the RDK / RDK UI.
	virtual void SetQuiet(void) = 0;

	/** Create a clone of the render session. All important state is preserved
		in the clone except that it gets a new unique render session id.
		This call \e must be followed by a call to EndClone(). */
	virtual IRhRdkRenderSession* BeginClone(void) const = 0;

	/** Finish creating a clone of the render session. */
	virtual void EndClone(void) const = 0;

	/** OBSOLETE. Not called. */
	virtual void SetElapsedSeconds(int elapsed) = 0;

	/** Get 'rendering was canceled' flag. */
	virtual bool WasCanceled(void) const = 0;

	/** Set 'rendering was canceled' flag. */
	virtual void SetWasCanceled(void) = 0;

	/** Call this to dispose of the render session. */
	virtual void Dispose(void) = 0;

	virtual Status GetPrevStatus(void) const = 0;
};

/** \class IRhRdkRenderWindow

	This is the interface to the render window and frame buffer system used in the RDK render pipeline.

	\note This interface is implemented in RDK in a thread-safe manner so that you can call the methods
	 from any thread your renderer happens to be running in.

	To the client, this interface represents the window into which rendered pixels are written. It is
	associated with a render session and also usually with a render frame. The render session manages
	the render window object (including its smart frame buffer, frame buffer and channels), and in the
	case of asynchronous rendering, an async render context. The SdkRender stack object creates the session
	with a unique identifier. Anyone with the identifier can find the render session and therefore also the
	associated render window and optional async render context.

	\see CRhRdkRenderSession    \see CRhRdkSdkRenderImpl     \see IRhRdkAsyncRenderContext
	\see IRhRdkRenderSession    \see IRhRdkRenderUI
	\see RhRdkFindRenderSession \see RhRdkFindRenderWindow
*/
class RHRDK_SDK IRhRdkRenderWindow
{
public:
	virtual ~IRhRdkRenderWindow() { }

	/** Allocate all memory associated with a channel in advance. If your application will
		access channels on multiple threads during the rendering process, you are strongly advised
		to call this function first for each channel you will access on the main thread. Ensure that
		SetSize() has been called on the IRhRdkRenderWindow object first.
		\return \e true if successful or \e false if the channel cannot be opened. */
	ON_DEPRECATED_MSG("Use virtual PreAllocateChannel below") 
	static bool PreAllocateChannel(IRhRdkRenderWindow& rw, const UUID& uuidChannel);

	/** Allocate all memory associated with a channel in advance.  If your application will
		access channels on multiple threads during the rendering process, you are strongly advised
		to call this function first for each channel you will access on the main thread.  Ensure that
		SetSize has been called on the IRhRdkRenderWindow object first.
		\return \e true if successful or \e false if the channel cannot be opened. */
	virtual bool PreAllocateChannel(const UUID& uuidChannel) = 0;

	/** Renderers that support the feature uuidFeatureRenderChannels can call this to get the channels to render.
		The render settings has a user option for 'automatic' or 'custom'. When set to automatic, this returns the channels
		required by the current set of post effects. When set to custom, this returns the user's choice of channels.
		\note The returned channels are not added to the render window by this method. This just reports which channels
		 the RDK recommends you add based on the user's settings and which post effects are in use. */
	virtual_su bool GetRenderChannels(OUT ON_SimpleArray<UUID>& aChan) const; // = 0; [SDK_UNFREEZE]

	/** \return The pixel size of the frame buffer. */
	virtual ON_2iSize Size(void) const = 0;

	/** Set the pixel size of the frame buffer.
		\return \e true if successful, else \e false. */
	virtual bool SetSize(const ON_2iSize& size) = 0;

	/** Lock the dib for exclusive access. You \e must call UnlockDib() later even if this returns null. */
	virtual CRhinoDib* LockDib(void) const = 0;

	/** Unlock the dib. Must be called after \e any call to LockDib(). */
	virtual void UnlockDib(void) const = 0;

	/** OBSOLETE -- not implemented

		CRhRdkImageAdjust is no longer used and is not implemented. To access the settings that this
		used to provide, you will need access to the document being rendered.

		- For dithering settings, please use doc.Properties().RenderSettings().Dithering().

		- For gamma settings, please use doc.Properties().RenderSettings().LinearWorkflow().

		- For tone mapping settings, please use doc.Properties().RenderSettings().PostEffects().

	*/
	/*RDK_DEPRECATED*/ virtual CRhRdkImageAdjust Adjust(void) const = 0;

	/** \return The render session id of the associated render session. */
	virtual UUID RenderSessionId(void) const;

	/** \return The render session associated with this render window. */
	virtual IRhRdkRenderSession* RenderSession(void) const = 0;

	/** \return The SDK Render stack object associated with this render window.
		\note For async renderers, this method may return null because the SDK Render
		 object goes off the stack right after rendering begins. It is not safe to
		 call this method from your async rendering thread because the object may
		 go off the stack and get deleted after the thread gets a pointer to it. */
	virtual class CRhRdkSdkRender* SdkRender(void) const = 0;

	/** Set progress text and optional percentage.
		\param sText is the text to display in the progress area.
		\param iPercent is the amount of progress as a percentage. If this parameter is
		 set to -1 it will be ignored. */
	virtual void SetProgress(const ON_wString& sText, int iPercent) = 0;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual void EndAsyncRender(eRenderSuccessCode rsc);

	enum class RenderResult : unsigned int
	{
		Completed,
		Failed,
	};

	/** This function must be called when an asynchronous render process has ended.
		Failing to do so will result in crashes when render window junk
		is cleared (either on EndCommand or exit of program).
		\param res is rsc_completed for a successful render or canceled render,
		 or rsc_failed for a render with errors. */
	virtual void EndAsyncRender(RenderResult res) = 0;

	enum class FileResult : unsigned int
	{
		Success,
		OpenReadFail,
		OpenWriteFail,
		ReadFail,
		WriteFail,
		ReadException,
		WriteException,
		WrongRenderer,
	};

	virtual FileResult ReadFrameBufferFile(const wchar_t* wszFilename, CRhRdkRenderingInfo& infoOut) = 0;
	virtual FileResult WriteFrameBufferFile(const wchar_t* wszFilename) const = 0;
	virtual FileResult SaveRenderImageAs(const wchar_t* wszFilename, const UUID& uuidRenderEngine, bool bSaveAlpha) const = 0;

	/** \class IPostEffectExecutionControl

		This is an extension interface that can be optionally implemented by renderers to control
		the execution of post effects.

		To use this, implement the interface and pass a new instance of the implementation to
		RegisterPostEffectExecutionControl().

	*/
	class RHRDK_SDK IPostEffectExecutionControl
	{
	public:
		/** This method is implemented as \code delete this; \endcode */
		virtual void DeleteThis(void) = 0;

		/** Allows the renderer to decide when a post effect should be executed.
			\return \e true if the renderer is ready to execute the specified post effect, else \e false. */
		virtual bool ReadyToExecutePostEffect(const ON_UUID& post_effect_id) const = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;

	protected:
		virtual ~IPostEffectExecutionControl() { }
	};

	/** Register an instance of IPostEffectExecutionControl to be used for controlling post effect execution.
		The RDK takes ownership of the 'ec' object. */
	virtual_su void RegisterPostEffectExecutionControl(IPostEffectExecutionControl* ec); // = 0; [SDK_UNFREEZE]

	class RHRDK_SDK IChannel // Channel interface; returned by OpenChannel(). AKA 'CPU' channel.
	{
	public:
		virtual ~IChannel() { }

		/** \return The size of one pixel's data in the channel. For RDK standard channels, this
			 value is always sizeof(float). For the special chanRGBA composite channel,
			 this value is 4 * sizeof(float). */
		virtual UINT PixelSize(void) const = 0;

		/** Get a value from the channel. This function is deprecated in favor of IChannelEx::GetValueEx().
			\param x is the horizontal pixel position. No validation is done on this value.
			 The caller is responsible for ensuring that it is within the frame buffer.
			\param y is the vertical pixel position. No validation is done on this value.
			 The caller is responsible for ensuring that it is within the frame buffer.
			\return A pointer to the data in the channel at the specified position.
			\warning You must know how much data the buffer contains and be ready to read the correct
			 amount. If you try to read more data than the buffer contains, you may crash Rhino. */
		/* DEPRECATED */ virtual const void* GetValue(int x, int y) const = 0;

		/** Set a value into the channel.
			\param x is the horizontal pixel position. No validation is done on this value.
			 The caller is responsible for ensuring that it is within the frame buffer.
			\param y is the vertical pixel position. No validation is done on this value.
			 The caller is responsible for ensuring that it is within the frame buffer.
			\param co is the component order of the source data.
			\param pData contains the data to store in the channel at the specified position.
			\warning If x or y are out of range or pData does not point to a big enough buffer,
			 the function will fail and may crash Rhino. */
		virtual void SetValue(int x, int y, ComponentOrder co, const void* pData) = 0;

		/** Set a rectangle of values into the channel. The input parameters are checked for validity.
			If the rectangle is not fully inside the frame buffer, the function will fail.
			\param x is the horizontal pixel position of the left edge.
			\param y is the vertical pixel position of the top edge.
			\param width is the width of the rectangle in pixels.
			\param height is the height of the rectangle in pixels.
			\param stride is the number of bytes between pixel rows in the source buffer.
			\param co is the component order of the source data.
			\param pData contains the data to store in the channel starting at the specified position. There must
			 be the correct amount of data in the correct layout to match the pixel size, the shape of the rectangle
			 and the stride. NOTE: RDK standard channels expect float data; the size of each pixel is always
			 sizeof(float). For the special chanRGBA composite channel, the size of a pixel is 4 * sizeof(float).
			\return \e true if successful, else \e false. */
		virtual bool SetValueRect(int x, int y, int width, int height, int stride, ComponentOrder co, const void* pData) = 0;

		/** Set a rectangle of values into the channel with the ability to use x and y as offsets into pData.
			The input parameters are checked for validity. If the rectangle is not fully inside the frame buffer,
			the function will fail.
			\param x is the horizontal pixel position of the left edge.
			\param y is the vertical pixel position of the top edge.
			\param width is the width of the rectangle in pixels.
			\param height is the height of the rectangle in pixels.
			\param stride is the number of bytes between pixel rows in the source buffer.
			\param co is the component order of the source data.
			\param pData contains the data to store in the channel starting at the specified position. There must
			 be the correct amount of data in the correct layout to match the pixel size, the shape of the rectangle
			 and the stride. NOTE: RDK standard channels expect float data; the size of each pixel is always
			 sizeof(float). For the special chanRGBA composite channel, the size of a pixel is 4 * sizeof(float).
			\param bUseOffset is \e true if x and y should also be used as offsets into pData, else \e false.
			\return \e true if successful, else \e false. */
		virtual bool SetValueRect(int x, int y, int width, int height, int stride, ComponentOrder co, const void* pData, bool bUseOffset) = 0;

		/** Close the channel. */
		virtual void Close(void) = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;
	};

	class RHRDK_SDK IChannelEx : public IChannel // Extended 'CPU' channel interface.
	{
	public:
		virtual ~IChannelEx() { }

		/** \return the channel id. */
		virtual UUID Id(void) const = 0;

		/** \return the channel width. */
		virtual int Width(void) const = 0;

		/** \return the channel height. */
		virtual int Height(void) const = 0;

		/** Get value(s) from the channel.
			\param x is the horizontal pixel position. No validation is done on this value.
			 The caller is responsible for ensuring that it is within the frame buffer.
			\param y is the vertical pixel position. No validation is done on this value.
			 The caller is responsible for ensuring that it is within the frame buffer.
			\param co is the component order of the destination data.
			\param pData accepts the data from the channel at the specified position and it must have enough room
			 to accomodate the correct amount of data in the correct layout to match the pixel size.
			 NOTE: RDK standard channels contain float data; the size of each pixel is always sizeof(float).
			 For the special chanRGBA composite channel, the size of a pixel is 4 * sizeof(float).
			\return \e true if successful, else \e false. */
		virtual bool GetValueEx(int x, int y, ComponentOrder co, OUT void* pData) const = 0;

		/** Get a rectangle of values from the channel. The input parameters are checked for validity.
			If the rectangle is not fully inside the frame buffer, the function will fail.
			\param x is the horizontal pixel position of the left edge.
			\param y is the vertical pixel position of the top edge.
			\param width is the width of the rectangle in pixels.
			\param height is the height of the rectangle in pixels.
			\param stride is the number of bytes between pixel rows in the destination buffer.
			\param co is the component order of the destination data.
			\param pData accepts the data from the channel starting at the specified position and it must have
			 enough room to accomodate the correct amount of data in the correct layout to match the pixel size,
			 the shape of the rectangle and the stride. NOTE: RDK standard channels contain float data; the size
			 of each pixel is always sizeof(float). For the special chanRGBA composite channel, the size of a pixel
			 is 4 * sizeof(float).
			\return \e true if successful, else \e false. */
		virtual bool GetValueRect(int x, int y, int width, int height, int stride, ComponentOrder co, OUT void* pData) const = 0;

		/** Makes a clone of the channel. */
		virtual IChannelEx* Clone(void) const = 0;

		/** Get the minimum and maximum values in the channel */
		virtual bool GetMinMaxValues(OUT float& fMin, OUT float& fMax) const = 0;
	};

	class RHRDK_SDK IGPUChannel // 'GPU' channel.
	{
	public:
		virtual ~IGPUChannel() { }

		/** \return the type of API used for this channel's texture handle. */
		virtual CRhinoDisplayPipeline::DisplayTechnologies DisplayTechnology(void) const = 0;

		/** \return the channel id. */
		virtual UUID Id(void) const = 0;

		/** \return the channel width. */
		virtual int Width(void) const = 0;

		/** \return the channel height. */
		virtual int Height(void) const = 0;

		/** \return The size of one pixel's data in the channel. */
		virtual unsigned int PixelSize(void) const = 0;

		/** This method has been deprecated.
		    In order to access the texture handle, dynamic cast your object
		    to IOpenGLChannel or IMetalChannel and use their respective methods.
		    Use this class' DisplayTechnology() method to know which class to
		    cast to. */
		ON_DEPRECATED virtual unsigned int TextureHandle(void) const = 0;

		/** Close the channel. Implemented as \code delete this; \endcode */
		virtual void Close(void) = 0;

		/** \return a clone of the channel. */
		virtual IGPUChannel* Clone(void) const = 0;

		/** Copy the channel to a 'CPU' channel. */
		virtual void CopyTo(IChannelEx& chan) const = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;
	};
	
	class RHRDK_SDK IOpenGLChannel
	{
	public:
		virtual ~IOpenGLChannel() { }

		/** \return the channel's OpenGL texture. */
		virtual unsigned int OpenGLTexture(void) const = 0;
	};

#ifdef ON_RUNTIME_APPLE
	class RHRDK_SDK IMetalChannel
	{
	public:
		virtual ~IMetalChannel() { }

		/** \return the channel's Metal texture. */
		virtual id<MTLTexture> MetalTexture(void) const = 0;
	};
#endif

	/** Remove all channels except Red, Green, Blue and Alpha. Prepares the system for calling AddChannel(). */
	virtual void ClearChannels(void) = 0;

	/** Add a channel to the frame buffer in addition to the fixed Red, Green, Blue and Alpha channels.
		\param uuidChannel is the channel identifier. This can be one of the standard
		 RDK channel UUIDs (see below) or a custom-defined UUID. Standard RDK channels
		 appear on the toolbar and menu. Custom ones do not, but are still available
		 for use by software that knows how to use them (e.g., a custom post-effect plug-in).
		\param uPixelSize is the number of bytes required to store channel data for a single pixel.
		 In the case of the standard RDK channels, this value must be sizeof(float) because
		 these channels store a single float for each pixel. In the case of custom channels,
		 this value can be anything you like since RDK does not understand or use it.
		\note You can use the compound channel identifiers chanNormalXYZ etc, to automatically
		 add the individual underlying channels. In this case, the uPixelSize parameter is ignored.
		\return \e true if the channel was added successfully, else \e false. */
	virtual bool AddChannel(const UUID& uuidChannel, UINT uPixelSize) = 0;

	/** Use this utility method to automatically add wireframe channels to the rendered image.
		This method sets the frame buffer size and adds all the necessary wireframe channels automatically.
		It also creates the wireframe channel data automatically so that your renderer doesn't have to.
		You typically call this method only when your renderer does not support wireframe rendering itself.
		If you call this method, then you should not add any wireframe channels returned by GetRenderChannels().
		If your renderer \e is capable of rendering the wireframe channels itself, you should not call this method.
		Instead, you must make sure you add the wireframe channels if GetRenderChannels() requests them.
		\see IRhRdkRenderWindow::GetRenderChannels().
		After the wires are rendered, the wireframe post effects will composite them into the final rendered image.
		\param doc is the document to display.
		\param view is the view to display.
		\param size is the size of the image without clipping (i.e., if you have a region, it is the
		 size of the image before you cut the region out.
		\param region is the area of the rendering you want to display. This is usually a rect which is
		 from (0, 0) to the width and height in the 'size' parameter, but it's different during RenderWindow
		 and RenderInWindow where it is the region selected by the user in the viewport. This should match the
		 size of the render window itself (i.e., the size set using SetSize).

		\return \e true if all of the wireframe channels were added successfully.
		\note This method should really be called AddWireframeChannels(). [SDK_UNFREEZE]

		+---------------------+
		|                     |
		|         +-------+   |
		|         |       | <------- 'region' is the area being rendered. It's different to the full
		|         |       |   |       rendering area during RenderWindow and RenderInWindow.
		|         +-------+   |
		|                     | <--- 'size' is the size of the unclipped rendering.
		+---------------------+
	*/
	virtual bool AddWireframeChannel(CRhinoDoc& doc, const ON_3dmView& view, const ON_2iSize& size, const ON_4iRect& region) = 0;

	/** Open a channel. If the channel is already open, the function will fail and return null.
		If successful, an IChannel interface will be returned and you must call Close() on this
		interface after use. The returned interface allows direct access to the underlying
		channel (or, in the case of chanRGB, chanRGBA, chanAlbedoRGB and NormalXYZ, collective channels).
		It is the responsibility of the caller to exercise caution when calling IChannel::GetValue() and
		IChannel::SetValue() since no range checking or other validation is performed by these methods.
		\warning Incorrect use of the IChannel interface may crash Rhino.
		\param uuidChannel is the channel identifier.
		\note IChannel* is returned but this can be cast to IChannelEx* if needed.
		\return A pointer to the channel interface if the channel was opened successfully, else null. */
	virtual IChannel* OpenChannel(const UUID& uuidChannel) = 0;

	/** Query if a channel is available.
		\return \e true if the channel is available, else \e false. */
	virtual bool IsChannelAvailable(const UUID& uuidChannel) const = 0;

	/** Query if a channel is being shown.
		\return \e true if the channel is being shown, else \e false. */
	virtual bool IsChannelShown(const UUID& uuidChannel) const = 0;

	/** Show a channel.
		\param uuidChannel is the identifier of the channel to show.
		\return \e true if the channel was shown, else \e false. */
	virtual bool ShowChannel(const UUID& uuidChannel) = 0;

	/** Get the channel that is being shown. */
	virtual UUID ShowingChannel(void) const = 0;

	/** Invalidate the entire view window so that the pixels get painted. */
	virtual void Invalidate(void) = 0;

	/** Invalidate an area of the view window so that the pixels get painted. */
	virtual void InvalidateArea(const ON_4iRect& rect) = 0;

	virtual ON_3dmView View(void) const = 0;

	virtual void SetView(const ON_3dmView& view) = 0;

	/** Call this to set a watermark string which will be displayed in the rendering as a post effect.
		\param wsz is the string to set. */
	virtual_su void SetWatermarkText(const wchar_t* wsz); // = 0; [SDK_UNFREEZE] Uses EVF "SetWatermarkText" for now.

	/** Call this to clear the end time. Used when extending a rendering (e.g., in Cycles). */
	virtual_su void ClearEndTime(void); // = 0; [SDK_UNFREEZE] Uses EVF "ClearEndTime" for now.

	/** This function is here to support raytraced mode. */
	virtual_su void SetIsRendering(bool b); // = 0; [SDK_UNFREEZE] Uses EVF "SetIsRendering" for now.

	/** Emergency virtual function for future expansion. Currently uses:
		"SetWatermarkText" - pvData is pointer to wide string.
		"ClearEndTime"
		"SetIsRendering"   - pvData is pointer to bool. */
	virtual void* EVF(const wchar_t*, void*) = 0;

public: // Events.
	class RHRDK_SDK IEventWatcher
	{
	public:
		virtual ~IEventWatcher() { }

		enum class Hints : unsigned int
		{
			Adjustment,         // #### OBSOLETE
			Channel,            // Any channel change requiring a refresh in the UI.
			Progress,           // #### OBSOLETE
			SessionState,       // Render session state changed.
			Other,              // #### OBSOLETE
			Dib,                // #### OBSOLETE
			PostEffectProgress, // IMPORTANT: This event is sent from a worker thread. Do not directly update UI in response to this event.
			Histogram,          // Histogram changed and requires a refresh in the UI.
			PostEffectOnOff,    // A post effect was turned on or off.
		};
		virtual void OnRenderWindowEvent(Hints h) = 0;
		virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
	};

	/** Call this to register an event watcher. This enables the watcher to
		find out when data in the render window has changed. */
	virtual_su void RegisterEventWatcher(IEventWatcher& ew); /* = 0 */ // [SDK_UNFREEZE] Uses EVF "RegisterEventWatcher" for now.

	/** Call this to unregister an event watcher. */
	virtual_su void UnregisterEventWatcher(IEventWatcher& ew); /* = 0 */ // [SDK_UNFREEZE] Uses EVF "UnregisterEventWatcher" for now.

public:
	// Standard channel identifiers.

	// Red, Green, Blue and Alpha channels are fixed and always available.
	static const UUID chanRed;   // Represents the Red channel.
	static const UUID chanGreen; // Represents the Green channel.
	static const UUID chanBlue;  // Represents the Blue channel.
	static const UUID chanAlpha; // Represents the Alpha channel.
	static const UUID chanRGB;   // Represents the Red, Green and Blue channels collectively.
	static const UUID chanRGBA;  // Represents the Red, Green, Blue and Alpha channels collectively.

	// These channels can be added by calling AddChannel() if required.
	static const UUID chanDistanceFromCamera;       // Represents the distance-from-camera, AKA Z-buffer channel.
	static const UUID chanNormalX;                  // Represents the Normal X channel (world-space, range -1..+1).
	static const UUID chanNormalY;                  // Represents the Normal Y channel (world-space, range -1..+1).
	static const UUID chanNormalZ;                  // Represents the Normal Z channel (world-space, range -1..+1).
	static const UUID chanNormalXYZ;                // Represents the Normal X, Y, and Z channels collectively (world-space, range -1..+1).
	static const UUID chanAlbedoRed;                // Represents the Albedo red channel.
	static const UUID chanAlbedoGreen;              // Represents the Albedo green channel.
	static const UUID chanAlbedoBlue;               // Represents the Albedo blue channel.
	static const UUID chanAlbedoRGB;                // Represents the Albedo red, green, and blue channels collectively.
	static const UUID chanLuminanceRed;             // Represents the Luminance red channel.
	static const UUID chanLuminanceGreen;           // Represents the Luminance green channel.
	static const UUID chanLuminanceBlue;            // Represents the Luminance blue channel.
	static const UUID chanBackgroundLuminanceRed;   // Represents the Background Luminance red channel.
	static const UUID chanBackgroundLuminanceGreen; // Represents the Background Luminance green channel.
	static const UUID chanBackgroundLuminanceBlue;  // Represents the Background Luminance blue channel.
	static const UUID chanMaterialIds;              // Represents the Material Identifier channel.
	static const UUID chanObjectIds;                // Represents the Object Identifier channel.
	static const UUID chanWireframe;                // OBSOLETE - do not use.
	static const UUID chanWireframeRGBA;            // OBSOLETE - do not use.
	static const UUID chanWireframePointsRGBA;      // Points and point clouds.
	static const UUID chanWireframeIsocurvesRGBA;   // Surface edges and isocurves.
	static const UUID chanWireframeCurvesRGBA;      // Curves.
	static const UUID chanWireframeAnnotationsRGBA; // Dimensions and text.

public:
	/** Each of these methods creates a render window which is not associated with an actual window or
		render session but is otherwise fully functional. You can use it to write to channels, create DIBs,
		run post effects, tone-mapping, gamma, etc.
		\return a pointer to a standalone render window. The caller shall delete the returned object after use.
		\note If you want to do actual rendering using the render pipeline without a physical window on the screen,
		 you should not use these methods. Instead, you should call RhRdkNewRenderSession(). */

	// This method is obsolete. Please call the methods that pass a document instead.
	static IRhRdkRenderWindow* New(void);

	// This method is obsolete because it has no view access. It can load post effects only from the document.
	static IRhRdkRenderWindow* New(const CRhinoDoc& doc);

	// Loads post effects from the document and also from view-specific settings if available.
	static IRhRdkRenderWindow* New(const CRhinoDoc& doc, const ON_3dmView& view);

public: // Internal expert methods for the RDK UI.
	virtual IRhRdkRenderUI* UI(void) const = 0;
	virtual ON_wString ChannelDescription(const UUID&) const = 0;
	virtual void ApplyUserInterfaceUpdates(bool) = 0; // OBSOLETE
	virtual void ClearRendering(void) = 0;
	virtual void OnTimer(void) = 0; // OBSOLETE
	virtual void OnSynchronousRenderFinished(bool) = 0;
	virtual void SetUI(IRhRdkRenderUI*) = 0;
	virtual bool EnsureDib(void) = 0; // OBSOLETE
	virtual bool DeleteDib(void) = 0; // OBSOLETE
	virtual bool SetAdjust(const class IRhRdkImageAdjust&) = 0; // OBSOLETE
	virtual bool SetDib(const CRhinoDib&) = 0;                  // OBSOLETE
	virtual bool IsInPlace(void) const = 0;
	virtual void UseInPlaceView(const CRhinoDoc& doc, const ON_4iRect& rect, CRhinoView& view) = 0;
	virtual class CRhRdkPostEffectPlugInManager* PEPManager(void) const = 0;
	virtual IChannel* NewClientChannel(const UUID& uuidChannel, bool bAllowMultipleAccess) const = 0;
	virtual_su bool OnClose(void) const; // = 0; [SDK_UNFREEZE]
};
