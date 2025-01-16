
#pragma once // Public SDK header

#if defined RHINO_SDK_MFC && defined ON_RUNTIME_WIN

#include "RhRdkResizingDialog.h"

/** \class CRhRdkSectionBase_MFC

	This class represents one XP-style section within a holder.
	This class is typically used in non-content UIs, but can also be used for content UIs
	if you don't want to use content UI sharing.

	\see IRhinoUiSection

*/
class RHRDK_SDK CRhRdkSectionBase_MFC : public CRhRdkResizingDialog
{
	typedef CRhRdkResizingDialog _super;
public:
	CRhRdkSectionBase_MFC(unsigned int idd, CWnd* pParent=nullptr);

	bool IsBlocking(void) const;

protected:
	virtual const IRhinoUiSection& Interface(void) const = 0;
	virtual IRhinoUiSection& Interface(void) = 0;

	/** This method is obsolete (never called). */
	virtual unsigned int GetIDD(void) const = 0;

	/** Implement this method to return the module state for the section.
		Your override will typically return AfxGetStaticModuleState(); */
	virtual AFX_MODULE_STATE* GetModuleState(void) const = 0;

protected:
	/** Refresh the section. The default implementation calls DisplayData() followed by EnableDisableControls(). */
	virtual void Refresh(void);

	/** \returns the background color of the section. */
	virtual COLORREF BackgroundColor(void) const;

	/** OBSOLETE. Always uses Rhino compliant colors. */
	RDK_DEPRECATED virtual void SetBackgroundColor(COLORREF cr);

	virtual void SetControlPositionAndSize(void);

protected: // Helpers for moving a window to the position of another window (e.g., a placeholder).
	virtual void SetWindowPositionAndSize(CWnd& destWnd, CWnd& srceWnd) const;
	virtual void DeferWindowPositionAndSize(HDWP& hdwp, CWnd& destWnd, CWnd& srceWnd) const;

protected:
	/** \internal For RDK internal use only */
	virtual BOOL32 OnInitDialog() override;
	virtual BOOL32 PreCreateWindow(CREATESTRUCT& cs) override;
	virtual void PreSubclassWindow() override;
	virtual int GetDefaultHeight(void) const;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL32 bEnable);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nTimerId);
	afx_msg LRESULT OnRhinoOptionsSearchShowControlParent(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	virtual ~CRhRdkSectionBase_MFC();

	virtual UUID _InternalUuid(void) const = 0;

private:
	friend class CRhRdkSection_MFC;
	friend class CRhRdkContentUISection_MFC;
	friend class CRhRdkRenderSettingsSection_MFC;
	friend class CRhRdkSunSection_MFC;
	friend class CRhRdkPostEffectSection_MFC;
	void _SetParent(CWnd* pParent);
	CWnd* _Parent(void) const;
	void _SetFocus(void);
	void _Delete(void);
	void _Show(bool bShow);
	bool _IsShown(void) const;
	bool _IsCreated(void) const;
	void _Enable(bool bEnable);
	bool _IsEnabled(void) const;
	bool _IsHidden(void) const;
	void _SetIsHidden(bool);
	void _UpdateView(unsigned int flags);
	int _GetHeight(void) const;
	void _GetScreenCoords(ON_4iRect& rectOut) const;
	void _Refresh(bool bImmediate);
	ON_wString _SettingsTag(void) const;
	void _OnExpandSection(bool bExpanding);
	void _OnAttachingToHolder(IRhinoUiHolder& h);
	void _OnAttachedToHolder(IRhinoUiHolder& h);
	void _OnDetachingFromHolder(IRhinoUiHolder& h);
	void _OnDetachedFromHolder(IRhinoUiHolder& h);
	void _Move(const ON_4iRect& rect, bool bRepaint, bool bRepaintBorder);
	void _SetController(std::shared_ptr<IRhinoUiController>);
	std::shared_ptr<IRhinoUiController> _Controller(void) const;
	bool _EnableHeaderButton(int index, bool bEnable);
	bool _ShowHeaderButton(int index, bool bShow);
	void* _EVF(const wchar_t* wszFunc, void* pvData);
	virtual void InternalOnTimer(UINT_PTR) { }

public:
	// Helper function to get data from the controller.
	void* GetData(const UUID& uuidData, bool bForWrite) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkSection_MFC : public CRhRdkSectionBase_MFC, public IRhinoUiSection
{
	typedef CRhRdkSectionBase_MFC _super;
protected:
	CRhRdkSection_MFC(unsigned int idd, CWnd* pParent=nullptr);

	virtual IRhinoUiSection& Interface(void) override;
	virtual const IRhinoUiSection& Interface(void) const override;

public: // Implement IRhinoUiSection.
	virtual void SetParent(HWND pParent) override;
	virtual HWND Parent(void) const override;
	virtual void Delete(void) override;
	virtual ON_wString SettingsTag(void) const override;
	virtual void SetController(std::shared_ptr<IRhinoUiController>) override;
	virtual std::shared_ptr<IRhinoUiController> Controller(void) const override;
	virtual ON_UUID ControllerId(void) const override { return ON_nil_uuid; }

	virtual HWND GetWindow(void) override;

	/** If you want to create buttons on the section's header, you should override
		this method to return a pointer to a new instance of an object that implements
		IRhRdkHeaderButtonHandler.
		\note RDK takes ownership of this object. If you previously multiple-inherited
		 your section from IRhRdkHeaderButtonHandler, you must change your code so that
		 your header button handler is an independent lightweight object.
		\see IRhRdkHeaderButtonHandler */
	virtual IRhRdkHeaderButtonHandler* NewHeaderButtonHandler(void);

	virtual bool IsCreated(void) const override;
	virtual bool IsCollapsible(void) const override;
	virtual bool IsInitiallyExpanded(void) const override;
	virtual bool EnableHeaderButton(int index, bool bEnable) override;
	virtual bool ShowHeaderButton(int index, bool bShow) override;
	virtual void Move(const ON_4iRect& rect, bool bRepaint=false, bool bRepaintBorder=false) override;
	virtual void Show(bool bShow) override;
	virtual bool IsShown(void) const override;
	virtual bool IsHidden(void) const override;
	virtual void Enable(bool bEnable) override;
	virtual bool IsEnabled(void) const override;
	virtual void SetFocus(void) override;
	virtual int GetHeight(void) const override;
	virtual void GetScreenCoords(ON_4iRect& rectOut) const override;
	virtual void Refresh(bool bImmediate=false) override;
	virtual void OnExpandSection(bool bExpanding) override;
	virtual void UpdateView(unsigned int flags) override;
	virtual ON_Color GetBackgroundColor(void) const override;
	virtual void SetBackgroundColor(const ON_Color& col) override; // OBSOLETE.
	virtual void OnAttachingToHolder(IRhinoUiHolder& h) override;
	virtual void OnAttachedToHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachingFromHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachedFromHolder(IRhinoUiHolder& h) override;
	virtual void OnControllerActivatedEvent(IRhinoUiController&) override { }
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;

protected:
	virtual ~CRhRdkSection_MFC();

private:
	virtual UUID _InternalUuid(void) const { return Uuid(); }
	virtual UUID PlugInId(void) const override { return ON_nil_uuid; }
	virtual CRhinoCommandOptionName CommandOptionName(void) const override { return RHCMDOPTNAME(L""); }
	virtual CRhinoCommand::result RunScript(CRhRdkControllerPtr) override { return CRhinoCommand::failure; }
};

/** \class CRhRdkContentUISection_MFC

	This class represents one shared XP-style section within a IRhinoUiHolder.

	Expanding sections that appear in content UIs should be derived from this class instead of
	CRhRdkSectionBase_MFC. You can then call Content() to access the content that is currently
	using the UI. Typically your content UI section will be a subclass of CRhRdkMaterialUISection_MFC,
	CRhRdkEnvironmentUISection_MFC, or CRhRdkTextureUISection_MFC. Your UI will then be able to access the
	relevant material, environment or texture by calling either the Material(), Environment() or
	Texture() methods. Contents that do not support UI sharing can still be accessed by a stored
	pointer inside the section. This is the default to enable legacy code to continue working.

	\see CreateUI()
	\see DestroyUI()
	\see CRhRdkMaterialUISection_MFC
	\see CRhRdkEnvironmentUISection_MFC
	\see CRhRdkTextureUISection_MFC

*/
class RHRDK_SDK CRhRdkContentUISection_MFC : public CRhRdkSectionBase_MFC, public IRhinoUiSection //, public IRhRdkContentUISection - This may be added to a future version of the SDK. [SDK_UNFREEZE]
{
	typedef CRhRdkSectionBase_MFC _super;
public:
	CRhRdkContentUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

	/** Get the currently selected content.
		\return \e true if successful, else \e false.
		[SDK_UNFREEZE] In the future, this might be an override of IRhRdkContentUISection::GetSelection(). */
	bool GetSelection(OUT CRhRdkContentArray& aContent) const;

	/** Set the selection to a certain array of contents. This can be used, for example,
		to implement a button that selects a child or children of the currently selected content.
		\return \e true if successful, else \e false.
		[SDK_UNFREEZE] In the future, this might be an override of IRhRdkContentUISection::SetSelection(). */
	bool SetSelection(const CRhRdkContentArray& aContent);

protected:
	virtual IRhinoUiSection& Interface(void) override;
	virtual const IRhinoUiSection& Interface(void) const override;

	/** Query if the section 'supports varies'. Some sections can only display information
		for a single content at a time. Such sections do not 'support varies'. Other sections
		can display information for multiple contents at the same time. If the data in the
		contents is conflicting, the display shows 'varies' instead of a single value. Sections
		that support this feature are said to 'support varies'. Such sections will implement
		this method to return \e true when the contents are favorable.
		\param aContent is the array of contents to check.
		\return \e true if 'varies' is supported for the given contents, else \e false. */
	virtual bool SupportsVaries(const CRhRdkContentArray& aContent) const;

protected: // Implement IRhinoUiSection.
	virtual void SetParent(HWND pParent) override;
	virtual HWND Parent(void) const override;
	virtual void Delete(void) override;
	virtual bool IsCreated(void) const override;
	virtual void Move(const ON_4iRect& rect, bool bRepaint=false, bool bRepaintBorder=false) override;
	virtual void Show(bool bShow) override;
	virtual bool IsShown(void) const override;
	virtual void Enable(bool bEnable) override;
	virtual bool IsEnabled(void) const override;
	virtual void SetFocus(void);
	virtual void Refresh(bool bImmediate=false) override;
	virtual void GetScreenCoords(ON_4iRect& rectOut) const override;
	virtual ON_wString SettingsTag(void) const override;
	virtual ON_Color GetBackgroundColor(void) const override;
	virtual void SetBackgroundColor(const ON_Color& col) override; // OBSOLETE.
	virtual int GetHeight(void) const override;
	virtual bool IsHidden(void) const override;
	virtual bool IsCollapsible(void) const override;
	virtual bool IsInitiallyExpanded(void) const override;
	virtual bool EnableHeaderButton(int index, bool bEnable) override;
	virtual bool ShowHeaderButton(int index, bool bShow) override;
	virtual void UpdateView(unsigned int flags) override;
	virtual void OnExpandSection(bool bExpanding) override;
	virtual void OnAttachingToHolder(IRhinoUiHolder& h) override;
	virtual void OnAttachedToHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachingFromHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachedFromHolder(IRhinoUiHolder& h) override;
	virtual IRhRdkHeaderButtonHandler* NewHeaderButtonHandler(void) override;
	virtual void SetController(std::shared_ptr<IRhinoUiController>) override;
	virtual std::shared_ptr<IRhinoUiController> Controller(void) const override;
	virtual ON_UUID ControllerId(void) const override { return ON_nil_uuid; }
	virtual void OnEvent(IRhinoUiController& con, const UUID&, IRhinoUiController::EventPriority, const IRhinoUiEventInfo*) override;
	virtual void OnControllerActivatedEvent(IRhinoUiController& con) override;
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override;
	virtual HWND GetWindow(void) override;

	virtual UUID PlugInId(void) const override { return ON_nil_uuid; }
	virtual CRhinoCommandOptionName CommandOptionName(void) const override { return RHCMDOPTNAME(L""); }
	virtual CRhinoCommand::result RunScript(CRhRdkControllerPtr) override { return CRhinoCommand::failure; }

protected:
	virtual ~CRhRdkContentUISection_MFC();
	virtual void DisplayData(void) override;
	virtual void EnableDisableControls(void) override;
	virtual void InternalOnTimer(UINT_PTR nTimerId) override;

protected:
	void DeferredUpdateUI(void);
	void UpdateParameter(const wchar_t* wszParam, const CRhRdkVariant& vValue);

private:
	virtual UUID _InternalUuid(void) const { return Uuid(); }
};

class RHRDK_SDK CRhRdkMaterialUISection_MFC : public CRhRdkContentUISection_MFC
{
public:
	CRhRdkMaterialUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

protected:
	/** This method is deprecated because it doesn't support the concept of writing to the materials.
		Please use CRhRdkEditableContentArray instead.
		\see CRhRdkEditableContentArray. */
	RDK_DEPRECATED virtual void GetMaterialList(CRhRdkMaterialArray& aOut) const;

	/** This method is deprecated because it doesn't support the concept of writing to the materials
		or multiple selections. Please use CRhRdkEditableContentArray instead.
		\see CRhRdkEditableContentArray. */
	RDK_DEPRECATED CRhRdkMaterial* Material(void) const;
};

class RHRDK_SDK CRhRdkEnvironmentUISection_MFC : public CRhRdkContentUISection_MFC
{
public:
	CRhRdkEnvironmentUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

protected:
	/** This method is deprecated because it doesn't support the concept of writing to the environments.
		Please use CRhRdkEditableContentArray instead.
		\see CRhRdkEditableContentArray. */
	RDK_DEPRECATED virtual void GetEnvironmentList(CRhRdkEnvironmentArray& aOut) const;

	/** This method is deprecated because it doesn't support the concept of writing to the environments
		or multiple selections. Please use CRhRdkEditableContentArray instead.
		\see CRhRdkEditableContentArray. */
	RDK_DEPRECATED CRhRdkEnvironment* Environment(void) const;
};

class RHRDK_SDK CRhRdkTextureUISection_MFC : public CRhRdkContentUISection_MFC
{
public:
	CRhRdkTextureUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

protected:
	/** This method is deprecated because it doesn't support the concept of writing to the textures.
		Please use CRhRdkEditableContentArray instead.
		\see CRhRdkEditableContentArray. */
	RDK_DEPRECATED virtual void GetTextureList(CRhRdkTextureArray& aOut) const;

	/** This method is deprecated because it doesn't support the concept of writing to the textures
		or multiple selections. Please use CRhRdkEditableContentArray instead.
		\see CRhRdkEditableContentArray. */
	RDK_DEPRECATED CRhRdkTexture* Texture(void) const;
};

class RHRDK_SDK CRhRdkVariesMaterialUISection_MFC : public CRhRdkMaterialUISection_MFC
{
public:
	CRhRdkVariesMaterialUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

protected:
	virtual bool SupportsVaries(const CRhRdkContentArray& aContent) const override;
};

class RHRDK_SDK CRhRdkVariesEnvironmentUISection_MFC : public CRhRdkEnvironmentUISection_MFC
{
public:
	CRhRdkVariesEnvironmentUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

protected:
	virtual bool SupportsVaries(const CRhRdkContentArray& aContent) const override;
};

class RHRDK_SDK CRhRdkVariesTextureUISection_MFC : public CRhRdkTextureUISection_MFC
{
public:
	CRhRdkVariesTextureUISection_MFC(unsigned long idd, CWnd* pParent=nullptr);

protected:
	virtual bool SupportsVaries(const CRhRdkContentArray& aContent) const override;
};

/** \class CRhRdkRenderSettingsSection_MFC

	If you want to put your own custom dialogs in the Render Settings dialog, you must
	derive your dialog from this base class and override CRhRdkRenderPlugIn::AddCustomRenderSettingsSections().
	This dialog can be shown in both a modal and modeless context. \see IsModal().

	\see CRhRdkRenderPlugIn::AddCustomRenderSettingsSections()

*/
class RHRDK_SDK CRhRdkRenderSettingsSection_MFC : public CRhRdkSectionBase_MFC, public IRhinoUiSection //, public IRhRdkRenderSettingsSection - This may be added to a future version of the SDK. [SDK_UNFREEZE]
{
	typedef CRhRdkSectionBase_MFC _super;
public:
	CRhRdkRenderSettingsSection_MFC(RhRdkUiModalities m, unsigned int idd, CWnd* pParent=nullptr);

protected:
	virtual ~CRhRdkRenderSettingsSection_MFC();

protected: // Implement IRhinoUiSection.
	virtual void SetParent(HWND pParent) override;
	virtual HWND Parent(void) const override;
	virtual void Delete(void) override;
	virtual void Move(const ON_4iRect& r, bool b, bool rb) override;
	virtual void Show(bool b) override;
	virtual bool IsShown(void) const override;
	virtual bool IsCreated(void) const override;
	virtual bool IsHidden(void) const override;
	virtual void Enable(bool b) override;
	virtual bool IsEnabled(void) const override;
	virtual void SetFocus(void) override;
	virtual void Refresh(bool b) override;
	virtual void GetScreenCoords(ON_4iRect& r) const override;
	virtual ON_wString SettingsTag(void) const override;
	virtual ON_Color GetBackgroundColor(void) const override;
	virtual void SetBackgroundColor(const ON_Color& col) override; // OBSOLETE.
	virtual int GetHeight(void) const override;
	virtual bool IsCollapsible(void) const override;
	virtual bool IsInitiallyExpanded(void) const override;
	virtual bool EnableHeaderButton(int index, bool bEnable) override;
	virtual bool ShowHeaderButton(int index, bool bShow) override;
	virtual void UpdateView(unsigned int f) override;
	virtual void OnExpandSection(bool b) override;
	virtual IRhinoUiSection& Interface(void) override;
	virtual const IRhinoUiSection& Interface(void) const override;
	virtual void OnAttachingToHolder(IRhinoUiHolder& h) override;
	virtual void OnAttachedToHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachingFromHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachedFromHolder(IRhinoUiHolder& h) override;
	virtual IRhRdkHeaderButtonHandler* NewHeaderButtonHandler(void) override;
	virtual void SetController(std::shared_ptr<IRhinoUiController>) override;
	virtual std::shared_ptr<IRhinoUiController> Controller(void) const override;
	virtual ON_UUID ControllerId(void) const override { return ON_nil_uuid; }
	virtual void OnControllerActivatedEvent(IRhinoUiController&) override { }
	virtual HWND GetWindow(void) override;
	virtual void* EVF(const wchar_t*, void*) override;
	virtual UUID PlugInId(void) const override { return ON_nil_uuid; }
	virtual CRhinoCommandOptionName CommandOptionName(void) const override { return RHCMDOPTNAME(L""); }
	virtual CRhinoCommand::result RunScript(CRhRdkControllerPtr) override { return CRhinoCommand::failure; }

private:
	virtual UUID _InternalUuid(void) const { return Uuid(); }
};

/** \class CRhRdkSunSection_MFC

	If you want to put your own custom dialogs in the Sun dialog, you must
	derive your dialog from this base class and override CRhRdkRenderPlugIn::AddCustomSunSections().
	This dialog can be shown in modal, semi-modal and modeless contexts. \see IsModal().

	\see CRhRdkRenderPlugIn::AddCustomSunSections()

*/
class RHRDK_SDK CRhRdkSunSection_MFC : public CRhRdkSectionBase_MFC, public IRhinoUiSection //, public IRhRdkSunSection - This may be added to a future version of the SDK. [SDK_UNFREEZE]
{
	typedef CRhRdkSectionBase_MFC _super;
public:
	CRhRdkSunSection_MFC(RhRdkUiModalities m, unsigned int idd, CWnd* pParent=nullptr);

	// [SDK_UNFREEZE] In the future, this might be an override of IRhRdkSunSection::Sun().
	virtual_su const IRhRdkSun* Sun(void) const;

protected:
	virtual ~CRhRdkSunSection_MFC();

protected: // Implement IRhinoUiSection.
	virtual void SetParent(HWND pParent) override;
	virtual HWND Parent(void) const override;
	virtual void Delete(void) override;
	virtual void Move(const ON_4iRect& r, bool b, bool rb) override;
	virtual void Show(bool b) override;
	virtual bool IsShown(void) const override;
	virtual bool IsCreated(void) const override;
	virtual bool IsHidden(void) const override;
	virtual void Enable(bool b) override;
	virtual bool IsEnabled(void) const override;
	virtual void SetFocus(void) override;
	virtual void Refresh(bool b) override;
	virtual void GetScreenCoords(ON_4iRect& r) const override;
	virtual ON_wString SettingsTag(void) const override;
	virtual ON_Color GetBackgroundColor(void) const override;
	virtual void SetBackgroundColor(const ON_Color& col) override; // OBSOLETE.
	virtual int GetHeight(void) const override;
	virtual bool IsCollapsible(void) const override;
	virtual bool IsInitiallyExpanded(void) const override;
	virtual bool EnableHeaderButton(int index, bool bEnable) override;
	virtual bool ShowHeaderButton(int index, bool bShow) override;
	virtual void UpdateView(unsigned int f) override;
	virtual void OnExpandSection(bool b) override;
	virtual IRhinoUiSection& Interface(void) override;
	virtual const IRhinoUiSection& Interface(void) const override;
	virtual void OnAttachingToHolder(IRhinoUiHolder& h) override;
	virtual void OnAttachedToHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachingFromHolder(IRhinoUiHolder& h) override;
	virtual void OnDetachedFromHolder(IRhinoUiHolder& h) override;
	virtual IRhRdkHeaderButtonHandler* NewHeaderButtonHandler(void) override;
	virtual void SetController(std::shared_ptr<IRhinoUiController>) override;
	virtual std::shared_ptr<IRhinoUiController> Controller(void) const override;
	virtual ON_UUID ControllerId(void) const override { return ON_nil_uuid; }
	virtual void OnControllerActivatedEvent(IRhinoUiController&) override { }
	virtual HWND GetWindow(void) override;
	virtual void* EVF(const wchar_t*, void*) override;
	virtual UUID PlugInId(void) const override { return ON_nil_uuid; }
	virtual CRhinoCommandOptionName CommandOptionName(void) const override { return RHCMDOPTNAME(L""); }
	virtual CRhinoCommand::result RunScript(CRhRdkControllerPtr) override { return CRhinoCommand::failure; }

private:
	virtual UUID _InternalUuid(void) const { return Uuid(); }
};

class RHRDK_SDK CRhRdkPostEffectSection_MFC : public CRhRdkSection_MFC //, public IRhRdkPostEffectSection - This may be added to a future version of the SDK. [SDK_UNFREEZE]
{
	typedef CRhRdkSection_MFC _super;
public:
	CRhRdkPostEffectSection_MFC(unsigned int idd, CWnd* pParent=nullptr);

	// [SDK_UNFREEZE] In the future, this might be an override of IRhRdkPostEffectSection::GetParameter().
	virtual CRhRdkVariant GetParam(const wchar_t* wszName, const CRhRdkVariant& vDefault);

	// [SDK_UNFREEZE] In the future, this might be an override of IRhRdkPostEffectSection::SetParameter().
	virtual bool SetParam(const wchar_t* wszName, const CRhRdkVariant& vValue);

protected:
	// [SDK_UNFREEZE] In the future, this might be an override of IRhRdkPostEffectSection::PostEffectId().
	virtual UUID PostEffectId(void) const = 0;

	// [SDK_UNFREEZE] In the future, this will probably be deleted.
	enum class Type { Early, ToneMapping, Late };
	virtual Type PostEffectType(void) const = 0;

protected:
	virtual void DisplayData(void) override;
};

#endif
