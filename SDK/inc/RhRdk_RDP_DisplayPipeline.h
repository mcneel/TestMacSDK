
#pragma once // Public SDK header

#include <vector>

namespace RhRdk
{
	namespace Realtime
	{
		// Provides a thread-safe way to signal to an object that updates will be needed
		class RHRDK_SDK ISignalUpdate
		{
		public:
			virtual ~ISignalUpdate() { }
			virtual void SignalUpdate(void) = 0;
			virtual void* EVF(const wchar_t*, void*) = 0;
		};

		/** \class RhRdk::Realtime::DisplayMode

		RhRdk::Realtime::DisplayMode provides a way to integrate a new rendering method in
		the Rhino viewport.

		\see https://github.com/mcneel/rhino-developer-samples/tree/6/cpp/SampleRealtimeRenderer
		*/
		class RHRDK_SDK DisplayMode : public CRhinoMouseCallback, public ISignalUpdate
		{
		public:
			/** \class RhRdk::Realtime::DisplayMode::Factory

			Factory class needed to register a display mode with the system. This enables
			the display system to new up a custom implementation when needed.
			*/
			class Factory
			{
			public:
#if defined(ON_RUNTIME_APPLE)
                //This is OSX only because adding it for Windows would break the SDK. [SDK_UNFREEZE]
                virtual ~Factory() {}
#endif
				/**
				Implementation should return the name of the display mode.
				*/
				virtual ON_wString Name(void) const = 0;
				/**
				Implementation should return the UUID of the display mode.
				*/
				virtual const UUID&  ClassId(void) const = 0;
				/**
				Implementation should return a new instance of its DisplayMode implementation as a
				shared_ptr.
				*/
				virtual std::shared_ptr<DisplayMode> MakeDisplayEngine(const CRhinoDisplayPipeline& pipeline) const = 0;
				virtual void* EVF(const wchar_t*, void*) = 0;

			public:
				/**
				Registration function for the custom Display mode factory implementation.
				*/
				RHRDK_SDK static void Register(std::unique_ptr<Factory>&& newFactory);
			};

			class Factories
			{
			private:
				class Impl;
				Impl* m_pImpl;
				friend class Impl;

			public:
				Factories();
				virtual ~Factories();

				std::vector<std::unique_ptr<DisplayMode::Factory> >::const_iterator begin(void) const;
				std::vector<std::unique_ptr<DisplayMode::Factory> >::const_iterator end(void) const;
				std::vector<std::unique_ptr<DisplayMode::Factory> >::iterator erase(std::vector<std::unique_ptr<DisplayMode::Factory> >::const_iterator factory);

				std::shared_ptr<DisplayMode> Make(const UUID& realtime_id, const CRhinoDisplayPipeline& pipeline) const;
				void Add(std::unique_ptr<DisplayMode::Factory>&& newFactory);

				void Destroy(void);

				virtual void* EVF(const wchar_t*, void*);
			};

		public:
			/**
			Constructor.
			*/
			DisplayMode(const CRhinoDisplayPipeline& pipeline);
			/**
			Virtual deconstructor.
			*/
			virtual ~DisplayMode();

			/** Returns the runtime serial number of the CRhinoDisplayPipeline that is associated with this display mode.
			*/
			unsigned int PipelineRuntimeSerialNumber(void) const;

			/** Returns the runtime serial number of the CRhinoView that is associated with this display mode.
			*/
			unsigned int ViewRuntimeSerialNumber(void) const;

			/** Returns the display mode associated with a particular display pipline.
			*/
			static std::shared_ptr<DisplayMode> FromPipeline(const CRhinoDisplayPipeline& pipe);

			/** Returns the size of the viewport or renderframe related to the running session.
			*/
			ON_2iSize	RenderSize(void) const;

			/** You must override this with a generated UUID - and the one in your factory must match it.
			*/
			virtual const UUID&  ClassId(void) const = 0;

			///////////////////////////////////////////////////////////////////////////
			//	Fundamental Renderer state queries
			//
		public:
			/** The display pipeline has successfully requested a lock on the renderer
				and we can use this to render the viewport.
				pParent points to the original engine if this rendering is being used for capture (render an existing view at a new size)
				*/
			virtual bool StartRenderer(const ON_2iSize& frameSize,
				const CRhinoDoc& doc,
				const ON_3dmView& view,
				const ON_Viewport& vp,
				const DisplayMode* pParent) = 0;

			/** Scene has changed, don't start from scratch, just get changes and continue.
			*/
			virtual bool OnRenderSizeChanged(const ON_2iSize& newFrameSize) = 0;
			/**
			The display mode is switched out, you need to clean up your display mode integration.
			*/
			virtual void ShutdownRenderer() = 0;

			/** The system's best guess as to whether StartRenderer will succeed.
			If your renderer is available return true, if it isn't ready yet return false.
			*/
			virtual bool RendererIsAvailable(void) const = 0;

			/** If this display mode has a parent, then it is an offline capture started from another pipeline.  Either a view capture or a print. */
			const DisplayMode* Parent(void) const;

			/** Create the world for this display mode instance. This sets up the changequeue. */
			virtual void CreateWorld(const CRhinoDoc& doc, const ON_3dmView& view, const CDisplayPipelineAttributes& attributes) = 0;
			//virtual void UpdateWorld(void) = 0;

			/** Used to pass information about which pass to render to for cloned (view capture) renderers. */
			virtual int LastRenderedPass(void) const = 0;

			/** Return true if custom display mode supports for the built-in view capture progress dialog . */
			virtual bool ShowCaptureProgress(void) const = 0;
			/** Return the progress of an ongoing capture. */
			virtual double Progress(void) const = 0;

			/**	We have started the renderer and set it up successfully. */
			virtual bool	IsRendererStarted(void) const = 0;
			
			/** Return true if the renderer is done. */
			virtual bool	IsCompleted(void) const = 0;

			/** Called during the frame draw to decide whether there is relevant data
			 when "LockRendererFrameBuffer" is called.  Return false if the image is still
			 being rendered.
			*/
			virtual bool	IsFrameBufferAvailable(const ON_3dmView& vp) const = 0;

			/** Default behaviour is to return nullptr if the view dimensions are the same, or construct a new one
			if not.  Relies on "RenderSize" to determine if something has changed.
			*/
			virtual std::shared_ptr<DisplayMode> Clone(const CRhinoDisplayPipeline& pipeline) const;

			// Renderer frame buffer access
			struct FRAME_BUFFER_INFO_INPUTS
			{
				bool enable_opengl_drawing = true;
				unsigned int minimum_gl_version_minor = 0;
				unsigned int minimum_gl_version_major = 2;
				CRhinoDisplayPipeline* pipeline = nullptr;
				CRhinoDisplayEngine* engine = nullptr;
				ON__INT_PTR padding0 = 0;
				ON__INT_PTR padding1 = 0;
				ON__INT_PTR padding2 = 0;
				ON__INT_PTR padding3 = 0;
			};

			struct FRAME_BUFFER_INFO_OUTPUTS
			{
				//Set this member to true if you successfully drew to the frame buffer on your own.
				//This tells Rhino to not attempt to do any drawing on its own.
				bool client_render_success = false;

				//Set this member to tell the display to draw the rendered image from the supplied DIB.  Alpha values will not be used.
				const CRhinoDib* pointer_to_dib = nullptr;

				//Set this member to tell the display to draw the image from the supplied buffer.  Alpha values will not be used.  You must set
				//the buffer_component_order, buffer_size_width, buffer_size_height  members in this case.
				const unsigned char* pointer_to_buffer = nullptr;

				//Set this to the component order of the buffer;
				ComponentOrder buffer_component_order = ComponentOrder::Irrelevant;

				//Set this value to the width of the buffer.
				unsigned buffer_size_width = 0;

				//Set this value to the height of the buffer.
				unsigned buffer_size_height = 0;

				//Set this to true if you want the framework to automatically flip the image before drawing.  Supported by all image types.
				bool flip_y = false;

				void* m_post_effect_plugin_ptr = nullptr;

				// The output region in the buffer where the render output data was stored.
				ON_4iRect render_output_rect = ON_4iRect::Unset;

			private:
				class Impl;
				Impl* m_pImpl = nullptr;
				friend Impl;
			};

			/** Return true if you have drawn the frame into the current OpenGL view, or you have supplied one of the other
			 methods for drawing in outputs.  Return false if the frame was not available, and the framework should wait.
			 If you're using IRhRdkRenderWindow as your framebuffer (which is recommended), you can implement DrawOrLockRendererFrameBuffer
			 simply by calling DrawOrLockRendererFrameBufferImpl.
			 */
			virtual bool DrawOrLockRendererFrameBuffer(const FRAME_BUFFER_INFO_INPUTS& input, FRAME_BUFFER_INFO_OUTPUTS& outputs) = 0;
			
			/** 
			 If you're calling DrawOrLockRendererFrameBufferImpl, you should call UnlockRendererFrameBufferImpl in your override
			 of this function.
			 */
			virtual void UnlockRendererFrameBuffer() = 0;

			/** See comment for DrawOrLockRendererFrameBuffer.
			*/
			static bool DrawOrLockRendererFrameBufferImpl(const DisplayMode& dm, IRhRdkRenderWindow& rw, const FRAME_BUFFER_INFO_INPUTS& input, FRAME_BUFFER_INFO_OUTPUTS& outputs);

			/** See comment for UnlockRendererFrameBuffer.
			*/
			static void UnlockRendererFrameBufferImpl(const DisplayMode& dm, IRhRdkRenderWindow& rw);

			/** Return true for fast draw mechanism. Default is old behavior where the pipeline would wait for
			 a short period on several occasions for the implementor to get a framebuffer ready.
			 This function is called just before StartRenderer();
			 */
			virtual bool UseFastDraw() = 0;

			/** Allows render engines to signal when they want the display pipeline to redraw.
			*/
			virtual void SignalUpdate(void) override;

			/** Calculate the CRC of ON_Viewport. Does not include properties that have no effect on the views appearance like the viewport ID . */
			static ON__UINT32 ComputeViewportCRC(const ON_Viewport& vp);

		public:
			/** \class IHUDCallbacks
			Support for built in HUD
			*/
			class IHUDCallbacks
			{
			public:
				virtual ~IHUDCallbacks() {}

				/** Return the name of the renderer
				*/
				virtual ON_wString	ProductName() const = 0;
				/** Return the requested maximum passes. */
				virtual int			MaximumPasses(void) const = 0;
				/** The user has changed the amount of maximum passes through the HUD. */
				virtual void		SetMaximumPasses(int) const = 0;
				/** Return the pass that has been rendered at the time of calling this function. */
				virtual int			LastRenderedPass() const = 0;
				/** Return true if the renderer is paused. */
				virtual bool 		RendererPaused(void) const = 0;
				/** Return true if the renderer is locked. */
				virtual bool 		RendererLocked(void) const = 0;
				/** Return true if you want to show the maximum pass count in the HUD. */
				virtual bool		ShowMaximumPasses(void) const = 0;
				/** Return true if you want to show the current pass count in the HUD. */
				virtual bool		ShowPasses(void) const = 0;
				/** Return true if you want to show custom status text in the HUD. */
				virtual bool		ShowCustomStatusText(void) const = 0;
				/** Return true if you want to show HUD controls. */
				virtual bool		ShowControls(void) const = 0;
				/** Return true if you want the HUD to show. */
				virtual bool		Show(void) const = 0;
				/** Return the start time of the rendering. */
				virtual time_t		RenderStartTime(void) const = 0;
				/** Return the custom status text to be shown if ShowCustomStatusText() returns true. */
				virtual ON_wString	CustomStatusText() const = 0;
				/** Return true if you want to allow the user to edit the maximum passes count from the HUD. */
				virtual bool		AllowMaximumPassesEdit() const = 0;
				virtual void* EVF(const wchar_t*, void*) = 0;
			};

			class IHUDFunctionality : public IHUDCallbacks
			{
			public:
				/** Pause (true) or resume (false) rendering. Once a display mode is paused any change from the changequeue should automatically unpause. */
				virtual void SetPaused(bool) const = 0;
				/** Lock (true) or unlock (false) the viewport. Changes from the changequeue are ignored. */
				virtual void SetLocked(bool) const = 0;
			};

			enum class StatusBarComponent : int
			{
				All,
				Play,
				Pause,
				Stop,
				Lock,
				Unlock,
				ProductName,
				StatusText,
				Time,
				PassAmount,
				PostEffectsOn,	//New in V7
				PostEffectsOff	//New in V7
			};


			//New in V7
			class IHUDCallbacks3 : public IHUDFunctionality
			{
			public:
				/** Return the start time of the rendering in milliseconds.
					Supersedes RenderStartTime(). */
				virtual ON__UINT64 RenderStartTimeInMilliseconds(void) const = 0;

				virtual void SetPostEffectsOn(bool) const = 0;
				virtual bool PostEffectsOn(void) const = 0;
				virtual bool ShowPostEffectsOnControl(void) const = 0;

				enum class MouseClickType
				{
					LeftClick = 0,
					RightClick = 1,
					DoubleClick = 2
				};

				virtual void OnStatusBarMouseEvent(StatusBarComponent, MouseClickType, int reserved) const = 0;
			};

			/** Default implementation stops built in HUD from being displayed.
				Implementation may optionally return IHUDFunctionality or IHUDCallbacks3 - dynamic cast to access that functionality. */
			virtual const IHUDCallbacks* HUDCallbacks() const;

			virtual void PushDynamicMeshes(void);
			virtual void UpdateDynamicMesh(const ON_Mesh&, const ON_Material&);

			//Events

			/** triggered when display pipeline attributes have changed. */
			virtual void OnDisplayPipelineSettingsChanged(const CDisplayPipelineAttributes&);

			/** Triggered when a component on the HUD has been pressed. Deprecated - use IHUDCallbacks3::OnStatusBarMouseEvent*/
			virtual void OnStatusBarComponentPressed(StatusBarComponent);
			/** Triggered when the user has changed the maximum passes count in the HUD. */
			virtual void OnMaxPassesChanged(int);

		protected:
			/** Triggered when OnPreDrawObjects conduit stage is invoked. */
			virtual void OnPreDrawObjects(CRhinoDisplayPipeline& pipeline);
			/** Triggered when OnPostDrawObjects conduit stage is invoked. */
			virtual void OnPostDrawObjects(CRhinoDisplayPipeline& pipeline);
			/** Triggered when OnInitFrameBuffer conduit stage is invoked. */
			virtual void OnInitFrameBuffer(CRhinoDisplayPipeline& pipeline);

		private:
			void SetParentDisplayMode(const DisplayMode* pParent);

		protected:
			virtual void InternalShutdown(void);
			virtual bool InternalStartRenderer(const CRhinoDisplayPipeline& pipe);
			virtual bool SetupSceneServer(const CRhinoDisplayPipeline& pipe);
			virtual bool PrepareRenderer(const CRhinoDisplayPipeline& pipe);

		public:
			virtual bool ExecConduit(CRhinoDisplayPipeline& pipeline, UINT channel, bool& terminate);
			void DrawShadedMesh(const CRhinoDisplayPipeline& pipeline, const ON_Mesh& mesh, const CDisplayPipelineMaterial* pMat);

			virtual_su IRhRdkRenderWindow* RenderWindow(void); // Implmented with EVF(L"RenderWindow", 0);

			virtual void* EVF(const wchar_t*, void*) override;

		protected:
			//CRhinoMouseCallback overrides
			virtual void OnMouseDown(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted) override;
			virtual void OnBeginMouseDown(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool& defaultHandlerExecuted) override;
			virtual void OnBeginMouseUp(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool& executeDefaultHandler) override;
			virtual void OnMouseUp(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted) override;
			virtual void OnMouseMove(unsigned int viewSerialNumber, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted) override;
			virtual BOOL32 OnMouseDblClk(CRhinoView& view, CRhinoMouseCallback::mouse_button, UINT flags, const ON_2iPoint& pt) override;
		
		public:
			void OnMouseLeave(); 

		private:
			class HUD;
			
			class Impl;
			Impl* m_pImpl;
			friend class Impl;
		};





		class DisplayModes
		{
		public:
			DisplayModes(); 
			virtual ~DisplayModes();

			const DisplayMode::Factories& Factories();

			void AddFactory(std::unique_ptr<DisplayMode::Factory>&& factory);
			void RemoveFactory(const UUID& uuidFactory);
			void RemoveAllFactories(void);

			//DisplayMode::Conduit& Conduit();

			void Add(std::shared_ptr<DisplayMode> mode);
			bool Remove(unsigned int pipeline_serial_number);

			std::shared_ptr<DisplayMode> FromPipelineSerialNumber(unsigned int);

			std::vector<std::shared_ptr<DisplayMode> >::const_iterator begin(void) const;
			std::vector<std::shared_ptr<DisplayMode> >::const_iterator end(void) const;

			virtual void* EVF(const wchar_t*, void*);

		private:
			class Impl;
			Impl* m_pImpl;
			friend class Impl;

			class Conduit;
		};


	}
}
