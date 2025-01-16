
#pragma once // Public SDK header

#include "IRhRdkRenderWindow.h"

class RHRDK_SDK IRhRdkListablePostEffect
{
public:
	/** \return \e true if the post effect is shown in a list. Only post effects that are shown will execute. */
	virtual bool Shown(void) const = 0;

	/** Show or hide the post effect.
		\return \e true if the 'shown' state changed, else \e false. */
	virtual bool SetShown(bool bOn) = 0;

	/** \return \e true if the post effect is turned on. Only post effects that are on will execute. */
	virtual bool On(void) const = 0;

	/** Turn the post effect on or off.
		\return \e true if the 'on' state changed, else \e false. */
	virtual bool SetOn(bool bOn) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkListablePostEffect() { }
};

/** \class IRhRdkPostEffect

	This is the new post effect interface. It is implemented by both the RDK for built-in post effects 
	and (partly) by 3rd-party plug-ins for custom post effects.

	In the new system, post effects consist of:
	- 'Early' post effects like Glare and Glow which operate on HDR (float) image data.
	- 'Tone mapping' post effects like Filmic which convert HDR image data to LDR (8-bit) image data.
	- 'Late' post effects like Gamma which operate on LDR image data.

	Post effects can be implemented to run on the CPU or the GPU at the discretion of the implementer.

	The post-processing runs in a 'pipeline' as follows:

	- The renderer produces a rendering in a frame buffer composed of channels.
	  The channels are HDR capable.
	- The early post effect manager's post effects are executed to produce modified versions of some of
	  these channels.
	- One of the tone mapping post effect manager's post effects is executed to perform tone mapping.
	- The RGBA channels are clamped to be between 0 and 1, converting the image from HDR to LDR.
	- The late post effect manager's post effects are executed to perform gamma correction and so on.
	- Finally, the RGBA channels left behind by these processes are converted to a displayable form.

	Old system (V5, V6)                  New system (V7+)
	-------------------                  ----------------
	- Post Effect Plug-ins (PEPs)        Early post effects (run before tone mapping).
	- Exposure
	  * Tone Mapping                     Tone mapping post effects.
	  * Gamma, Dithering, De-noising     Late post effects (run after tone mapping).

	Notes for 3rd-party post effect developers
	------------------------------------------
	- You should not try to implement IRhRdkPostEffect directly. Rather, you should subclass one of
	  CRhRdkEarlyPostEffect, CRhRdkToneMappingPostEffect or CRhRdkLatePostEffect.
	- If you want your post effect to be run before tone mapping, it should be an early post effect
	  implemented by subclassing CRhRdkEarlyPostEffect.
	- If your post effect performs Tone Mapping, it should be a Tone Mapping post effect implemented
	  by subclassing CRhRdkToneMappingPostEffect.
	- If you want your post effect to be run after tone mapping, it should be a late post effect
	  implemented by subclassing CRhRdkLatePostEffect.

*/
class RHRDK_SDK IRhRdkPostEffect
{
public:
	virtual ~IRhRdkPostEffect() { }

	enum class Types : int
	{
		Early,
		ToneMapping,
		Late
	};

	/** \return the Type of this post effect. */
	virtual Types Type(void) const = 0;

	/** \return The unique identifier of this post effect. */
	virtual UUID Id(void) const = 0;

	/** \return A CRC of the state of this post effect. */
	virtual ON__UINT32 CRC(void) const = 0;

	/** \return The localized name of this post effect. */
	virtual ON_wString LocalName(void) const = 0;

	/** \return \e true if the post effect is visible to the user. For early and late post effects, this is equivalent to 'shown'.
		 For tone-mapping post effects, this is equivalent to 'selected'. */
	virtual bool IsVisible(void) const = 0;

	/** \return \e true if the post effect is active. For early and late post effects, this is equivalent to 'shown' and 'on'.
		 For tone-mapping post effects, this is equivalent to 'selected'. */
	virtual bool IsActive(void) const = 0;

	/** \return \e true if the post effect is selected in the user interface. */
	virtual bool IsSelected(void) const = 0;

	enum : unsigned int // Flags for BitFlags()
	{
		uf_ExecuteForProductionRendering = 0x0001, // Post effect can execute during production rendering.
		uf_ExecuteForRealtimeRendering   = 0x0002, // Post effect can execute during realtime rendering.
		uf_ExecuteForViewportDisplay     = 0x0004, // Post effect can execute in the OpenGL viewport display (for future use only).
		uf_Fixed                         = 0x0100, // Post effect is always shown and cannot be removed by the user.
		uf_DefaultShown                  = 0x0200, // Post effect is shown by default.
		uf_DefaultOn                     = 0x0400, // Post effect is turned on by default.
		uf_UsesRenderingSettings         = 0x0800, // Post effect uses rendering settings rather than plain document settings.
	};

	/** Flags describing when the post effect executes and its attributes. See enum above. */
	virtual unsigned int BitFlags(void) const = 0;

	enum class ExecuteWhileRenderingOptions
	{
		// The post effect does not support execution while rendering.
		Never,

		// The post effect supports execution while rendering and it should be run every time the dib
		// is updated.
		Always,

		// The post effect supports execution while rendering but only after a delay the first time.
		UseDelay,

		// The post effect supports execution while rendering but requires an
		// IRhRdkRenderWindow::IPostEffectExecutionControl object to be registered with the render window.
		// The ReadyToExecutePostEffect() function on this object will then be called to decide when to run
		// the post effect. If an execution control is not registered, the post effect will always execute.
		UseExecutionControl
	};

	/** As rendering proceeds, in order for progress to be seen on the screen, small areas of the frame buffer
		that have been rendered are post-processed and written to an area of the dib which is then shown on the
		screen. Some post effects will not be able to support this as they need the whole frame buffer in order to operate.
		This method allows a post effect to report whether or not it is able to support execution during rendering, and
		if so, when it should be done. \see enum ExecuteWhileRenderingOptions above. */
	virtual ExecuteWhileRenderingOptions GetExecuteWhileRenderingOption(void) const = 0;

	/** If GetExecuteWhileRenderingOption() returns UseDelay then this method returns the delay in milliseconds before the
		post effect should execute for the first time. After the first time, the post effect will execute every time the dib is updated
		during rendering. \see enum ExecuteWhileRenderingOptions above. */
	virtual int ExecuteWhileRenderingDelayMS(void) const = 0;

	/** \return \e true if the post effect can execute, else \e false.
		The default implementation checks if the post effect is 'on' and 'shown'.
		Post effect authors can override this to include other criteria but please be sure to call the base class. */
	virtual bool CanExecute(const class IRhRdkPostEffectPipeline& pepl) const = 0;

	/** The RDK calls this method to determine which channels a post effect requires. If a required channel is not
		available, the RDK will hide the post effect's UI and display explanatory text instead.
		\param aChan accepts the channels. The post effect should add all channels used by its implementation to this array.
		 The post effect should \e not clear the array as other post effects may have previously added their channels to it.
		\note As a convenience, the default implementation adds IRhRdkRenderWindow::chanRGBA to the output array.
		 Most post effects should be able to use this default with no need to override the method. */
	virtual void RequiredChannels(OUT ON_SimpleArray<UUID>& aChan) const = 0;

	/** Execute the post effect.
		\param pepl provides access to the post-effect pipeline.
		\param rect is the pixel area to process.
		\see IRhRdkPostEffectPipeline
		\return \e true if successful, else \e false. */
	virtual bool Execute(IRhRdkPostEffectPipeline& pepl, const ON_4iRect& rect) const = 0;

	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkPostEffect& BeginChange(RhRdkChangeContext cc) const = 0;

	/** Called when the state of the post effect is changed. If your post effect subclass implements a member variable
		that affects rendering and/or appears in the post effect's user interface, then you should call this method
		from your Set... method when the Set... method detects a change to the value of the member. */
	virtual void Changed(void) = 0;

	/** Call this method after finishing a batch of changes. You must call EndChange() once for every call to BeginChange().
		\return \e true if successful, else \e false. */
	virtual bool EndChange(void) = 0;

	/** Get a parameter.
		\param wszName is the name of the parameter to get.
		\param vValue accepts the parameter value.
		\return \e true if successful or false if the parameter was not found. */
	virtual bool GetParameter(const wchar_t* wszName, OUT CRhRdkVariant& vValue) const = 0;

	/** Set a parameter.
		\param wszName is the name of the parameter to get.
		\param vValue specifies the type and value to set.
		\return \e true if successful or false if the parameter could not be set. */
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) = 0;

	class RHRDK_SDK IState // Arbitrary parameters.
	{
	public:
		virtual bool GetParam(const wchar_t* wszName, OUT CRhRdkVariant& vValue) const = 0;
		virtual bool SetParam(const wchar_t* wszName, const CRhRdkVariant& vValue) = 0;
		virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

	protected:
		virtual ~IState() { }
	};

	/** Read the state. If your post effect has no state, you must still return \e true for success.
		\return \e true if successful, else \e false. */
	virtual bool ReadState(const IState& state) = 0;

	/** Write the state. If your post effect has no state, you must still return \e true for success.
		\return \e true if successful, else \e false. */
	virtual bool WriteState(IState& state) const = 0;

	/** Because post effects are now in the render settings, this function can no longer be called.
		ReadFromRenderSettings() is called instead. */
	RDK_DEPRECATED virtual bool ReadFromDocumentDefaults(const CRhinoDoc& doc) = 0;

	/** Because post effects are now in the render settings, this function can no longer be called.
		WriteToRenderSettings() is called instead. */
	RDK_DEPRECATED virtual bool WriteToDocumentDefaults(CRhinoDoc& doc) const = 0;

	virtual_su bool ReadFromRenderSettings(const ON_3dmRenderSettings& rs); // = 0; [SDK_UNFREEZE] Uses EVF(L"ReadFromRenderSettings", &rs)
	virtual_su bool WriteToRenderSettings(ON_3dmRenderSettings& rs) const;  // = 0; [SDK_UNFREEZE] Uses EVF(L"WriteToRenderSettings", &rs)

	/** Reset the state to factory defaults. */
	virtual void ResetToFactoryDefaults(void) = 0;

	/** Create each of your UI sections using 'new' and then call ui.AddSection() on them.
		RDK takes ownership of the sections. If your post effect does not need a UI, then
		your implementation of this method can be a no-op. */
	virtual void AddUISections(class IRhRdkPostEffectUI& ui) = 0;

	/** Displays the post effect's help page, if any.
		\return \e true if successful, else \e false. */
	virtual bool DisplayHelp(void) const = 0;

	/** \return \e true if the post effect can display a help page, else \e false. */
	virtual bool CanDisplayHelp(void) const = 0;

	/** Get the listable interface. Returns null if the post effect is not listable.
		Therefore, it only returns an object if it's an early or late post effect. */
	virtual IRhRdkListablePostEffect* Listable(void) = 0;

	/** Get the listable interface (const version). Returns null if the post effect is not listable.
		Therefore, it only returns an object if it's an early or late post effect. */
	virtual const IRhRdkListablePostEffect* Listable(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

public:
	/** \internal For RDK internal use only. */
	virtual void SetManager(class CRhRdkPostEffectManager&) = 0;
};

class RHRDK_SDK CRhRdkPostEffect : public CRhRdkObject, public IRhRdkPostEffect
{
public:
	CRhRdkPostEffect(IRhRdkPostEffect::Types type);
	virtual ~CRhRdkPostEffect();

	virtual Types Type(void) const final override;
	virtual bool IsVisible(void) const override;
	virtual bool IsActive(void) const final override;
	virtual bool IsSelected(void) const final override;
	virtual bool CanExecute(const IRhRdkPostEffectPipeline&) const override;
	virtual void RequiredChannels(OUT ON_SimpleArray<UUID>& aChan) const override;
	virtual int  ExecuteWhileRenderingDelayMS(void) const override;
	virtual bool ReadFromDocumentDefaults(const CRhinoDoc& doc) override;
	virtual bool WriteToDocumentDefaults(CRhinoDoc& doc) const override;
	virtual ON__UINT32 CRC(void) const final override;
	virtual IRhRdkPostEffect& BeginChange(RhRdkChangeContext cc) const override;
	virtual void Changed(void) override;
	virtual bool EndChange(void) override;
	virtual IRhRdkListablePostEffect* Listable(void) override;
	virtual const IRhRdkListablePostEffect* Listable(void) const override;
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;
	virtual_su bool ReadFromRenderSettings(const ON_3dmRenderSettings& rs) override_su; // [SDK_UNFREEZE]
	virtual_su bool WriteToRenderSettings(ON_3dmRenderSettings& rs) const  override_su; // [SDK_UNFREEZE]

public:
	virtual void SetManager(class CRhRdkPostEffectManager& man) final override;

	virtual_su void CopyParamsFrom(const ON_3dmRenderSettings& rs); // [SDK_UNFREEZE] Uses EVF(L"CopyParamsFrom", &rs)
	virtual_su void CopyParamsTo(ON_3dmRenderSettings& rs) const;   // [SDK_UNFREEZE] Uses EVF(L"CopyParamsTo", &rs)

public:
	class CImpl;
	CImpl* m_pImpl;

protected:
	CImpl* Impl(void);
	const CImpl* Impl(void) const;
};

/** \class CRhRdkEarlyPostEffect

	3rd-party early post effects should subclass this class.

	Early post effects must expect the channel data to possibly be greater than 1
	(i.e., high dynamic range) and they must preserve the high dynamic range (HDR);
	it is the job of the following tone mapping stage to convert HDR to LDR.

*/
class RHRDK_SDK CRhRdkEarlyPostEffect : public CRhRdkPostEffect
{
public:
	CRhRdkEarlyPostEffect();
	virtual ~CRhRdkEarlyPostEffect();
};

/** \class CRhRdkToneMappingPostEffect

	3rd-party tone mapping post effects should subclass this class.

	Tone mapping post effects must expect the channel data to possibly be greater than 1
	(i.e., high dynamic range). Their job is to convert this high dynamic range (HDR) data
	to low dynamic range (LDR) data in the range 0..1

*/
class RHRDK_SDK CRhRdkToneMappingPostEffect : public CRhRdkPostEffect
{
public:
	CRhRdkToneMappingPostEffect();
	virtual ~CRhRdkToneMappingPostEffect();
};

/** \class CRhRdkLatePostEffect

	3rd-party late post effects should subclass this class.

	Late post effects can assume that the channel data was normalized to 0..1 by the previous tone mapping
	or clamp operation. If no tone mapping or clamping was done (such as when creating a HDR result), then
	the late post effects are not executed.

*/
class RHRDK_SDK CRhRdkLatePostEffect : public CRhRdkPostEffect
{
public:
	CRhRdkLatePostEffect();
	virtual ~CRhRdkLatePostEffect();
};

/** \class IRhRdkPostEffectCollection

	This interface provides access to all the post effects that are available from a data source.
	Post effects are classified by having a type of 'early', 'tone mapping', or 'late'.
	Each of these sets can be accessed separately as each has slightly different specializations.

*/
class RHRDK_SDK IRhRdkPostEffectCollection
{
public:
	virtual ~IRhRdkPostEffectCollection() { }

	virtual IRhRdkPostEffect* PostEffectFromId(const UUID& uuidPostEffect) const = 0;
	virtual void GetPostEffects(IRhRdkPostEffect::Types type, OUT ON_SimpleArray<IRhRdkPostEffect*>& a) const = 0;

	// Emergency virtual function for future expansion.
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
};

/** \class IRhRdkPostEffectPipeline

	This object provides a way for post effects to access the frame buffer channels from a rendering and create
	new channels containing post-processed information which can be passed to the next post effect in the chain.

	Consider a simple post effect that just modifies the red component of a rendering. It will call GetChannelForRead()
	to get the RGBA channel as its input, and it will call GetChannelForWrite() to get a new RGBA channel for its output.
	It will then read the input channel, do calculations and write to the output channel. When finished, it will
	call IChannel::Commit() passing on the output channel. Because both channels have the same identifier, this will
	replace the old channel with the new one so that subsequent post effects in the chain will use the new channel instead
	of the original. Note that this will only replace the channel used by the pipeline. The original channel will still
	exist in the frame buffer. This system allows any post effect to access any number of channels for reading and
	create any number of new channels which may or may not replace existing channels depending on the channel id.
	The final stage (convert to 8-bit) operates on the channels left in the pipeline by the post effect chain to
	produce the final 32-bit RGBA image.

	It is also possible for a post effect to create and use any number of 'scratch' channels. If a post effect needs
	a temporary pixel buffer for some intermediate results, it can call GetChannelForWrite() with a custom (random) id.
	Once it is finished with this scratch channel, it will automatically be discarded by the pipeline.

	\see IRhRdkPostEffect

*/
class RHRDK_SDK IRhRdkPostEffectPipeline
{
public:
	virtual ~IRhRdkPostEffectPipeline() { }

	enum : int // creationFlags for IRhRdkPostEffectPipeline::New().
	{
		CreationFlags_None                          = 0x00,
		CreationFlags_DisableEarlyPostEffects       = 0x01,
		CreationFlags_DisableToneMappingPostEffects = 0x02,
		CreationFlags_DisableLatePostEffects        = 0x04,
		CreationFlags_DisableGamma                  = 0x08,

		CreationFlags_DisableAllPostEffects = CreationFlags_DisableEarlyPostEffects       |
		                                      CreationFlags_DisableToneMappingPostEffects |
		                                      CreationFlags_DisableLatePostEffects
	};

	static IRhRdkPostEffectPipeline* New(IRhRdkRenderWindow& rw, const UUID& uuidShowingChannel, int creationFlags);

	/** \return a UUID that uniquely identifies the rendering being processed. */
	virtual UUID RenderingId(void) const = 0;

	/** \return \e true if the pipeline is allowed to use the GPU, else \e false.
		post effect authors should check that GPU use is allowed before using the GPU in a post effect. */
	virtual bool AllowGPU(void) const = 0;

	/** Get the dimensions of the frame buffer. All channels in the frame buffer have the same dimensions. */
	virtual ON_2iSize Dimensions(void) const = 0;

	/** \return a pointer to the specified post effect or null if not found. */
	virtual IRhRdkPostEffect* FindPostEffect(const UUID& uuidPostEffect) const = 0;

	/** Get a list of the post effects to be executed by this pipeline in order. */
	virtual void GetPostEffects(OUT ON_SimpleArray<ON_UUID>& uuidPostEffects) const = 0;

	enum Histograms
	{
		eHistogram_None               = 0x00,
		eHistogram_BeforeEarlyEffects = 0x01,
		eHistogram_BeforeToneMapping  = 0x02,
		eHistogram_AfterToneMapping   = 0x04,
		eHistogram_AfterLateEffects   = 0x08,

		eHistogram_All = eHistogram_BeforeEarlyEffects | eHistogram_BeforeToneMapping | eHistogram_AfterToneMapping | eHistogram_AfterLateEffects,

		eHistogram_ToneMappingDisplay = eHistogram_BeforeToneMapping | eHistogram_AfterToneMapping,

		eHistogram_AfterEarlyEffects = eHistogram_BeforeToneMapping,
		eHistogram_BeforeLateEffects = eHistogram_AfterToneMapping,
	};

	enum class ExecuteContexts
	{
		ProductionRendering = 0,
		RealtimeRendering   = 1,
		ViewportDisplay     = 2,
		ThumbnailCreation   = 3,
		ConvertingToHDR     = 4,
	};

	/** Execute the pipeline. This executes all the post effects in order.
		\param rect is a rectangle within the frame buffer. Only this rectangle needs be modified by the post effects.
		\param bRenderingIsInProgress is \e true if rendering is in progress (i.e., not yet completed).
		\param uc provides the context that this pipeline is running in.
		\param flagsHistogramsToUpdate is a set of flags specifying when to update the frame buffer's histograms.
		\return \e true if successful, else \e false. */
	virtual bool Execute(const ON_4iRect& rect, bool bRenderingIsInProgress, ExecuteContexts uc, Histograms flagsHistogramsToUpdate, unsigned int reserved=0, void* pReserved=nullptr) = 0;

	/** Post effects should call this during execution to report progress.
		A good strategy is to call this once per pixel row (or several rows).
		If the function returns \e false, your post effect should exit its pixel loop
		as the user has requested that the operation be canceled.
		\return \e true if the process should continue, else \e false. */
	virtual bool ReportProgress(int rowsCompleted) = 0;

	/** Get the max luminance in the rendering. */
	virtual float GetMaxLuminance(void) const = 0;

	/** Set the start time of the rendering in milliseconds since some unspecified epoch. */
	virtual void SetStartTimeInMilliseconds(ON__UINT64 ms) = 0;

	/** Get the start time of the rendering expressed in milliseconds since some unspecified epoch.
		Do not make assumptions about what the epoch is; it might be different on different platforms. */
	virtual ON__UINT64 GetStartTimeInMilliseconds(void) const = 0;

	/** Get the end time of the rendering expressed in milliseconds since some unspecified epoch.
		Do not make assumptions about what the epoch is; it might be different on different platforms. */
	virtual ON__UINT64 GetEndTimeInMilliseconds(void) const = 0;

	/** \return \e true if rendering is active, else \e false. */
	virtual bool IsRendering(void) const = 0;

	/** Provides access to a thread engine that allows post effects to run off of the main thread.
		\see IRhRdkPostEffectThreadEngine */
	virtual class IRhRdkPostEffectThreadEngine& ThreadEngine(void) const = 0;

	class RHRDK_SDK IChannel
	{
	public:
		/** Return the channel id which indicates what type of data is used in this channel. */
		virtual UUID Id(void) const = 0;

		/** Return the pixel size in bytes for this channel. */
		virtual unsigned int PixelSize(void) const = 0;

		/** Return an interface to this channel for doing read/write operations on the channel using the CPU. */
		virtual IRhRdkRenderWindow::IChannelEx* CPU(void) = 0;

		/** Return an interface to this channel for doing read-only operations on the channel using the CPU. */
		virtual const IRhRdkRenderWindow::IChannelEx* CPU(void) const = 0;

		/** Return an interface to this channel for doing read/write operations on the channel using the GPU. */
		virtual IRhRdkRenderWindow::IGPUChannel* GPU(void) = 0;

		/** Return an interface to this channel for doing read-only operations  on the channel using the GPU. */
		virtual const IRhRdkRenderWindow::IGPUChannel* GPU(void) const = 0;

		/** Return a clone of this channel. */
		virtual IChannel* Clone(void) const = 0;

		/** Close the channel. Implemented as \code delete this; \endcode */
		virtual void Close(void) = 0;

		/** Commit changes to the channel so that those changes can be used by subsequent post effects in the chain.
			Only valid for channels that were obtained by calling GetChannelForWrite().
			If the channel has the same id as an existing channel, the existing channel will be replaced by
			the new one. If the existing channel was created by a previous post effect in the chain, it will be deleted.
			Changes to channels that are not commited simply get ignored.
			\note This call merely sets a flag. The process is deferred until after the post effect has finished executing. */
		virtual void Commit(void) = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t* wsFunc, void* pvData) = 0;

	protected:
		virtual ~IChannel() { }
	};

	/** Get a channel for reading. A post effect will use this to get channel data as input to
		its process. Output will be written to new channel(s). \see GetChannelForWrite()
		This method returns the current state of the channel at this stage in the pipeline.
		If the first post effect calls this, it will get the actual frame buffer channel.
		Subsequent post effects will get the data left behind by the previous post effect.
		A post effect calls GetChannelForRead() to get its input and GetChannelForWrite()
		to get the object to which it will write its output. Even when the same channel id
		is specified, these are separate, unconnected objects.
		\param uuidChannel is the channel identifier.
		\param flags is for future use; you should pass zero.
		\return A pointer to the channel or null if the channel is not available. */
	virtual const IChannel* GetChannelForRead(const UUID& uuidChannel, unsigned int flags) const = 0;

	/** Get a channel for writing. A post effect will use this to get channel(s) to write the output of its
		processing to. Input will usually come from existing channels, although a post effect is free to read
		its own output channels if needed. \see GetChannelForRead()
		\param uuidChannel is the channel identifier.
		\param flags is for future use; you should pass zero.
		\return A pointer to the new channel or null if the channel could not be created.
		\note You are allowed to create one new channel with the same identifier as an existing channel,
		 in which case IChannel::Commit() will replace the existing channel with the new one in the pipeline. */
	virtual IChannel* GetChannelForWrite(const UUID& uuidChannel, unsigned int flags) = 0;

	// Emergency virtual function for future expansion.
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
};

class RHRDK_SDK IRhRdkPostEffectUI
{
public:
	/** Add a section to the UI. */
	virtual void AddSection(IRhinoUiSection* pSection) = 0;

	// Emergency virtual function for future expansion.
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkPostEffectUI() { }
};

/** \class IRhRdkPostEffectThreadEngine

	This class allows post effects to run asynchronously (off of the main thread) by queuing jobs that run
	on a worker thread. It is implemented by the RDK. Example usage:

	bool CMyPostEffect::Execute(IRhRdkPostEffectPipeline& pepl, const ON_4iRect& rect) const
	{
		CMyPostEffectJob job(*this); // Implements IRhRdkPostEffectThreadEngine::IJob
		ON_SimpleArray<UUID> aChanId;
		aChanId.Append(IRhRdkRenderWindow::chanRGBA);
		aChanId.Append(...);
		return pepl.ThreadEngine().RunPostEffect(job, pepl, *this, rect, aChanId, nullptr);
	}

*/
class RHRDK_SDK IRhRdkPostEffectThreadEngine
{
public:
	/** \class IJob

		Represents a job queued in the thread engine system. A post effect implements this interface
		to do its processing.

	*/
	class RHRDK_SDK IJob
	{
	public:
		/** \class IChannels

			This is a collection of channels that will be processed by the post effect. The channels in this
			collection are copies of some of the channels being passed through the post effect pipeline. These
			are the channels that were specified by id during the call to RunPostEffect(). The post effect will
			read from one or more of these channels, do calculations, and write the results to one or more of the channels.

		*/
		class RHRDK_SDK IChannels
		{
		public:
			/** Get a channel interface. This interface can be used to get the channel data.
				\param chanId is the channel identifier.
				\return a pointer to the channel or null if the channel is not in the collection. */
			virtual IRhRdkPostEffectPipeline::IChannel* GetChannel(const UUID& chanId) const = 0;

			/** Emergency virtual function for future expansion. */
			virtual void* EVF(const wchar_t*, void*) = 0;

		protected:
			virtual ~IChannels() { }
		};

		/** Implement this to execute your post effect. It runs in a worker thread.
			\param rect is the rectangle to be processed.
			\param chans is the collection of channels to process.
			\return \e true if successful, else \e false. */
		virtual bool Execute(const ON_4iRect& rect, IChannels& chans) = 0;

		/** This method must be implemented as \code return new CMyJob(...); \endcode.
			\return a clone of the job object. */
		virtual IJob* Clone(void) const = 0;

		/** You must implement this method as \code delete this; \endcode */
		virtual void DeleteThis(void) = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;

	protected:
		virtual ~IJob() { }
	};

	/** Called from a post effect's Execute() method to run the post effect asynchronously.
		Post effects are queued as 'jobs' and run in sequence on a worker thread.
		\param job is the job that will be executed asynchronously (on a thread). The job is copied
		 so that ownership of 'job' remains with the caller. Note that IJob& job should be const and
		 this will be corrected the next time we can break the SDK.
		\param pepl is the post effect pipeline that was passed to the post effect's Execute() method.
		\param pe is the post effect being executed.
		\param rect is the rectangle that was passed to the post effect's Execute() method.
		\param aChanId is an array of channel identifiers to the channels the post effect will process.
		\param pExtension is for future expansion and should be passed as nullptr.
		\return \e true if successful, else \e false. */
	virtual bool RunPostEffect(/*const*/ IJob& job, IRhRdkPostEffectPipeline& pepl, const IRhRdkPostEffect& pe,
	                           const ON_4iRect& rect, const ON_SimpleArray<UUID>& aChanId, void* pExtension) = 0; // [SDK_UNFREEZE] const IJob&

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkPostEffectThreadEngine() { }
};
