
#pragma once // Public SDK header

#include "RhRdkTypes.h"
#include "RhRdkCoreDialogs.h"
#include "RhRdkDataSource.h"

class RHRDK_SDK IRhinoUiWindow
{
public:
	/** Implement this method to return the text caption that should be displayed in the window.
		\param bAlwaysEnglish controls localization. If \e true the method must return the English caption.
		 Otherwise, it must return the localized caption. If no caption is required, the method should
		 return an empty string. */
	virtual ON_wString Caption(bool bAlwaysEnglish=false) const = 0;

	/** Set the parent (host) window.
		\param parent is the parent to host the window on.
		\note When the object uses a controller, the controller must be set before calling
		 SetParent() otherwise creation of the underlying control may fail. */
	virtual void SetParent(HWND parent) = 0;

	/** Get the parent (host) window. This will return null unless a parent has been set in SetParent(). */
	virtual HWND Parent(void) const = 0;

	/** \return \e true if the underlying window has been created, else \e false. */
	virtual bool IsCreated(void) const = 0;

	/** Destroys the underlying window and deletes the object. */
	virtual void Delete(void) = 0;

	/** Moves the underlying window.
		\param rect is the rectangle to move to within the parent. */
	virtual void Move(const ON_4iRect& rect, bool bRepaint=false, bool bRepaintBorder=false) = 0;

	/** Shows or hides the window.
		\param bShow is \e true to show the window or \e false to hide it. */
	virtual void Show(bool bShow) = 0;

	/** \return \e true if the window is being shown, else \e false. */
	virtual bool IsShown(void) const = 0;

	/** Enables or disables the window.
		\param bEnable is \e true to enable the window or \e false to disable it. */
	virtual void Enable(bool bEnable) = 0;

	/** \return \e true if the window is enabled, else \e false. */
	virtual bool IsEnabled(void) const = 0;

	/** Sets the keyboard focus to the window. */
	virtual void SetFocus(void) = 0;

	/** Refreshes the window by repainting it.
		\param bImmediate is \e true if the window should be repainted immediately, else \e false.
		\note On Microsoft Windows, Refresh(true) is equivalent to RedrawWindow() and Refresh(false)
		 is equivalent to Invalidate(). */
	virtual void Refresh(bool bImmediate=false) = 0;

	/** Get the screen coordinates of the underlying window.
		\param rectOut accepts the coordinates. */
	virtual void GetScreenCoords(ON_4iRect& rectOut) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

	/** Do not use */
	virtual HWND GetWindow(void) = 0;

protected:
	virtual ~IRhinoUiWindow() { }
};

class RHRDK_SDK IRhRdkNotificationHandler
{
public:
	/** This method must be implemented as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** This method is called when multiple notifications begin.
		\param w is the window sending the notification.
		\param context is a control-specific value. */
	virtual void OnBeginNotify(IRhinoUiWindow& w, unsigned int context) = 0;

	/** This method is called when multiple notifications end.
		\param w is the window sending the notification.
		\param context is a control-specific value. */
	virtual void OnEndNotify(IRhinoUiWindow& w, unsigned int context) = 0;

	/** This method is called when a notification occurs.
		\param w is the window sending the notification.
		\param vOld is the old value. The type depends on the sending control.
		\param vNew is the new value. The type depends on the sending control.
		\param context is a control-specific value. */
	virtual void OnNotify(IRhinoUiWindow& w, const CRhRdkVariant& vOld, const CRhRdkVariant& vNew, unsigned int context) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkNotificationHandler() { }
};

class RHRDK_SDK IRhinoUiHolder : public IRhinoUiWindow
{
public:
	enum class Styles : unsigned int { Flat };

	/** Create a new holder.
		\param f is the framework to use.
		\param rect is the initial rect for the holder.
		\param colBackground obsolete. The background color is now automatically set based on the current Rhino color scheme.
		\param hParent is the holder's parent window.
		\param style is the holder style (for future use).
		\return A pointer to the holder if successful, or null on failure.
		 The caller shall delete the holder when it is no longer needed. \see IRhinoUiWindow::Delete(). */
	static IRhinoUiHolder* New(RhRdkUiFrameworks f, const ON_4iRect& rect, const ON_Color& colBackground,
	                           HWND hParent=NULL, IRhinoUiHolder::Styles style=IRhinoUiHolder::Styles::Flat);

	enum class Margins : unsigned int { Top, Bottom, Left, Right };
	virtual int Margin(Margins m) const = 0;
	virtual void SetMargin(Margins m, int pixels) = 0;

	/** \return The unique identifier of the holder. */
	virtual UUID Uuid(void) const = 0;

	/** Calls UpdateView() on all sections.
		\param flags specifies what to update. See the enum at IRhinoUiSection::UpdateView() */
	virtual void UpdateAllViews(unsigned int flags) = 0;

	/** Call this before attaching or detaching more than one section.
		This optimizes the process and reduces the amount of flicker when
		attaching or detaching multiple sections. */
	virtual void BeginAttachDetachSections(void) = 0;

	/** Call this after attaching or detaching more than one section. */
	virtual void EndAttachDetachSections(void) = 0;

	/** Attaches a section to the holder. The holder takes ownership of the section.
		\param pSection is a pointer to a new section.
		\return \e true if successful, else \e false. */
	virtual bool AttachSection(IRhinoUiSection* pSection) = 0;

	/** Detaches a section from the holder. The section is hidden, unparented from
		the holder and returned to the ownership of the caller. If you are not
		planning to reuse the section, you must call Delete() on it. */
	virtual bool DetachSection(IRhinoUiSection& s) = 0;

	/** Sets a section as 'full height'. This means it expands to use the available space below it. */
	virtual_su void SetFullHeightSection(IRhinoUiSection& section); // = 0; [SDK_UNFREEZE] Implemented via EVF(L"SetFullHeightSection").

	/** Sets text to be displayed in the holder when there are no sections. */
	virtual void SetEmptyText(const wchar_t* wsz) = 0;

	virtual int GetSectionCount(void) const = 0;

	class RHRDK_SDK IIterator
	{
	public:
#if defined(ON_RUNTIME_APPLE)
		// This is OSX only because adding it for Windows would break the SDK. [SDK_UNFREEZE]
		virtual ~IIterator() { }
#endif
		virtual IRhinoUiSection* FirstSection(void) = 0;
		virtual IRhinoUiSection* NextSection(void) = 0;
		virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
	};

	/** Get a section iterator. The caller shall delete the iterator when no longer needed. */
	virtual IIterator* NewIterator(void) const = 0;

	/** \return \e true if a section is expanded, else \e false.
		\param s is the section to check. */
	virtual bool IsSectionExpanded(const IRhinoUiSection& s) const = 0;

	/** Expands or collapses a section.
		\param s is the section.
		\param bExpand is \e true to expand the section, \e false to collapse it.
		\param bEnsureVisible is \e true to scroll the section into view, else \e false.
		\return \e true if successful, else \e false. */
	virtual bool ExpandSection(IRhinoUiSection& s, bool bExpand, bool bEnsureVisible) = 0;

	/** OBSOLETE. Always uses Rhino compliant colors. */
	virtual void SetBackgroundColor(const ON_Color& col) = 0;

	/** Enable or disable a section's header button.
		\param s is the section.
		\param index is the index of the button.
		\param bEnable is the enable state. */
	virtual bool EnableHeaderButton(IRhinoUiSection& s, int index, bool bEnable) = 0;

	/** Show or hide a section's header button.
		\param s is the section.
		\param index is the index of the button.
		\param bShow is the show state. */
	virtual bool ShowHeaderButton(IRhinoUiSection& s, int index, bool bShow) = 0;

	/** Optionally call this to specify a sub-key for storing section settings. */
	virtual void SetSettingsSubKey(const wchar_t* wsz) = 0;

	/** return The vertical scroll bar position. */
	virtual int GetScrollPos(void) const = 0;

	/** Sets the vertical scroll bar position. */
	virtual void SetScrollPos(int pos, bool bRedraw) = 0;

protected:
	virtual ~IRhinoUiHolder() { }
};

class RHRDK_SDK IRhinoUiWithController : public IRhinoUiWindow, public IRhinoUiControllerEventWatcher
{
public:
	/** Get the controller associated with this window. */
	virtual std::shared_ptr<IRhinoUiController> Controller(void) const = 0;

	/** Set the controller associated with this window. */
	virtual void SetController(std::shared_ptr<IRhinoUiController> con) = 0;

	/** If your UI requires a specific registered controller, return its ID here - otherwise return ON_nil_uuid. */
	virtual ON_UUID ControllerId(void) const = 0;

protected:
	virtual ~IRhinoUiWithController() { }
};

class RHRDK_SDK IRhinoUiSection : public IRhinoUiWithController
{
public:
	/** Implement this method to return the section's unique identifier. */
	virtual UUID Uuid(void) const = 0;

	/** Implement this method to return a unique string used to identify the section when
		loading and saving the section's state. This can usually be implemented to return
		a string version of the section's Uuid() unless something special is needed. */
	virtual ON_wString SettingsTag(void) const = 0;

	/** \return the background color of the section. */
	virtual ON_Color GetBackgroundColor(void) const = 0;

	/** OBSOLETE. Always uses Rhino compliant colors.*/
	virtual void SetBackgroundColor(const ON_Color& col) = 0;

	/** \return the height of the section in pixels.
		This is usually the same as the default height but can be overridden to enable
		variable-height sections.
		\note This method is called continuously by the holder and should be implemented to
		 be as fast as possible. */
	virtual int GetHeight(void) const = 0;

	/** \return \e true if the section should be hidden, else \e false.
		\note This is not to be confused with IRhinoUiWindow::IsShown().
		 IsHidden() tells the holder whether or not to completely hide the section and its header.
		 IsShown() reports the current visible state of the section window which is invisible
		 even if the section is only collapsed.
		\note This method is called continuously by the holder and should be implemented to
		 be as fast as possible. */
	virtual bool IsHidden(void) const = 0;

	/** If this method returns \e false, the holder will not allow the user to collapse the section.
		\return \e true if the section is collapsible, else \e false. */
	virtual bool IsCollapsible(void) const = 0;

	/** \return \e true if the section is initially expanded. This is used
		to determine the section's default expanded state. Thereafter, the
		expanded state is remembered by the RDK in the persistent settings. */
	virtual bool IsInitiallyExpanded(void) const = 0;

	/** Enable or disable a header button.
		\param index is the index of the button.
		\param bEnable is the enable state. */
	virtual bool EnableHeaderButton(int index, bool bEnable) = 0;

	/** Show or hide a header button.
		\param index is the index of the button.
		\param bShow is the show state. */
	virtual bool ShowHeaderButton(int index, bool bShow) = 0;

	/** If you want to create buttons on the header, you should override this method to
		return a pointer to a new instance of an object that implements IRhRdkHeaderButtonHandler.
		\note RDK takes ownership of this object. If you previously multiple-inherited your section
		 from IRhRdkHeaderButtonHandler, you must change your code so that your header button handler
		 is an independent lightweight object.
		\see IRhRdkHeaderButtonHandler */
	virtual IRhRdkHeaderButtonHandler* NewHeaderButtonHandler(void) = 0;

	enum : unsigned int // Flags for UpdateView().
	{
		updv_DisplayData   = 0x0001, // Transfers data to elements of the UI.
		updv_EnableDisable = 0x0002, // Enables or disables elements of the UI.
		updv_UpdateLayout  = 0x0004, // Lays out the UI elements in the correct places (when resizing section, etc).
	};

	/** Updates the 'view' of the section according to the specified flags.
		\param flags specifies what to update. See the enum above. */
	virtual void UpdateView(unsigned int flags) = 0;

	/** Optionally implement this method to handle when the section is expanded or closed-up.
		This happens when the user clicks the section header.
		If you don't want to handle this, just implement the method to do nothing. */
	virtual void OnExpandSection(bool bExpanding) = 0;

	/** Optionally implement this method to handle just before the section is attached to a holder.
		If you don't want to handle this, just implement the method to do nothing. */
	virtual void OnAttachingToHolder(IRhinoUiHolder& h) = 0;

	/** Optionally implement this method to handle just after the section is attached to a holder.
		If you don't want to handle this, just implement the method to do nothing. */
	virtual void OnAttachedToHolder(IRhinoUiHolder& h) = 0;

	/** Optionally implement this method to handle just before the section is detached from a holder.
		If you don't want to handle this, just implement the method to do nothing. */
	virtual void OnDetachingFromHolder(IRhinoUiHolder& h) = 0;

	/** Optionally implement this method to handle just after the section is detached from a holder.
		If you don't want to handle this, just implement the method to do nothing. */
	virtual void OnDetachedFromHolder(IRhinoUiHolder& h) = 0;

	/** Implement this method to return your plug-in id.  Determines visibility for Sun and RenderSettings sections based on the current renderer. */
	virtual UUID PlugInId(void) const = 0;

	/** Implement this method to return the command option name for scripting. Used by Sun and RenderSettings sections. Should return L"" if no-scripting support. */
	virtual CRhinoCommandOptionName CommandOptionName(void) const = 0;

	/** Implement this method to do scripting.
		\param con is the controller to use to get and set your data items. Used by Sun and RenderSettings sections. */
	virtual CRhinoCommand::result RunScript(CRhRdkControllerPtr con) = 0;

protected:
	virtual ~IRhinoUiSection() { }
};

class RHRDK_SDK IRhRdkPostEffectSection
{
public:
	/** Implement this method to return the id of your post effect. */
	virtual UUID PostEffectId(void) const = 0;

	/** Implement this method to return a named post effect parameter. */
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName, const CRhRdkVariant& vDefault) const = 0;

	/** Implement this method to set a named post effect parameter. */
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) = 0;

	/** Get the post effects using this section. Currently, this returns only one item. */
	virtual void GetPostEffects(IRhRdkPostEffect::Types type, ON_SimpleArray<const IRhRdkPostEffect*>& aPostEffects) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkPostEffectSection() { }

public: // Temporary implementation. [SDK_UNFREEZE]
	static CRhRdkVariant GetParameter(const IRhinoUiWithController& ui, const UUID& uuidPostEffect, const wchar_t* wszName, const CRhRdkVariant& vDefault);
	static bool SetParameter(const IRhinoUiWithController& ui, const UUID& uuidPostEffect, const wchar_t* wszName, const CRhRdkVariant& vValue);
	static void GetPostEffects(const IRhinoUiWithController& ui, IRhRdkPostEffect::Types type, ON_SimpleArray<const IRhRdkPostEffect*>& aPostEffects);
	/*RDK_DEPRECATED*/ static void GetPostEffects(const IRhinoUiWithController& ui, const UUID& uuidPostEffect, ON_SimpleArray<const IRhRdkPostEffect*>& aPostEffects);
};

class RHRDK_SDK IRhRdkSunSection
{
public:
	/** Get the sun. */
	virtual const IRhRdkSun* Sun(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkSunSection() { }

public: // Temporary implementation. [SDK_UNFREEZE]
	static const IRhRdkSun* Sun(const IRhinoUiWithController& ui);
};

RDK_DEPRECATED class RHRDK_SDK IRhRdkRenderSettingsSection
{
public:
	/** Get the current environment settings. */
	virtual const class IRhRdkCurrentEnvironment* CurrentEnvironment(void) const = 0;

	/** Get the dithering settings. */
	virtual const class IRhRdkDithering* Dithering(void) const = 0;

	/** Get the ground plane settings. */
	virtual const class IRhRdkGroundPlane* GroundPlane(void) const = 0;

	/** Get the linear workflow settings. */
	virtual const class IRhRdkLinearWorkflow* LinearWorkflow(void) const = 0;

	/** Get the Rhino settings. */
	virtual const class IRhRdkRhinoSettings* RhinoSettings(void) const = 0;

	/** Get the skylight settings. */
	virtual const class IRhRdkSkylight* Skylight(void) const = 0;

	/** Get the sun settings. */
	virtual const class IRhRdkSun* Sun(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkRenderSettingsSection() { }

public: // Temporary implementation. [SDK_UNFREEZE]
	static const IRhRdkCurrentEnvironment* CurrentEnvironment(const IRhinoUiWithController& ui);
	static const IRhRdkDithering* Dithering(const IRhinoUiWithController& ui);
	static const IRhRdkGroundPlane* GroundPlane(const IRhinoUiWithController& ui);
	static const IRhRdkLinearWorkflow* LinearWorkflow(const IRhinoUiWithController& ui);
	static const IRhRdkRhinoSettings* RhinoSettings(const IRhinoUiWithController& ui);
	static const IRhRdkSkylight* Skylight(const IRhinoUiWithController& ui);
	static const IRhRdkSun* Sun(const IRhinoUiWithController& ui);
};

class RHRDK_SDK IRhRdkContentUISection
{
public:
	/** Get the currently selected content. */
	virtual bool GetSelection(OUT CRhRdkContentArray& aContent) const = 0;

	/** Set the content selection. */
	virtual bool SetSelection(const CRhRdkContentArray& aContent) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkContentUISection() { }

public: // Temporary implementation. [SDK_UNFREEZE]
	static bool GetSelection(const IRhinoUiWithController& ui, OUT CRhRdkContentArray& aContent);
	static bool SetSelection(const IRhinoUiWithController& ui, const CRhRdkContentArray& aContent);
};

RHRDK_SDK IRhinoUiSection* RhRdkFindSection(const IRhinoUiHolder& holder, const UUID& uuidSection);

RHRDK_SDK void RhRdkRemoveAllSections(IRhinoUiHolder& h);
