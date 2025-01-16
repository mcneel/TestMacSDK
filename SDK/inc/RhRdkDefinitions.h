
#pragma once // Public SDK header

#undef IN
#define IN

#undef OUT
#define OUT

#if defined ON_RUNTIME_WIN
	#ifdef _RHRDK
		#define RHRDK_SDK __declspec(dllexport)
	#else
		#define RHRDK_SDK __declspec(dllimport)
	#endif

	////////////////////////////////////////////////////////////////////////////////
	// This should only be commented in when testing the Eto UI or the MFC Test UI on Windows.
	//#define USE_RENDERING_TEST_DS
	////////////////////////////////////////////////////////////////////////////////
#else
	#define RHRDK_SDK __attribute__ ((visibility ("default")))
#endif

#define RDK_DEPRECATED ON_DEPRECATED

// Public SDK items with this marker are for RDK internal use only and should not be used by 3rd-party plug-ins.
#define RDK_INTERNAL

// Deprecating a class creates too many warnings which I don't know how to fix so for now it's just a no-op.
#define RDK_DEPRECATED_CLASS

// Classes and functions marked with RHRDK_UI_SDK were originally included in the RDK SDK for use by
// the RDK (MFC) UI, but they should really have been put in the internal SDK. Anything marked with
// this prefix should be considered to be in the public SDK by mistake and should not be used by
// public 3rd-party clients.
#define RHRDK_UI_SDK RHRDK_SDK

// Classes and functions marked with RHRDK_INTERNAL_SDK are in the RDK internal SDK
// which is not exposed to the public. This SDK can be changed in any way at any time.
#define RHRDK_INTERNAL_SDK RHRDK_SDK

// When the SDK can be unfrozen, functions marked with this will become real virtual functions.
#define virtual_su

// When the SDK can be unfrozen, functions marked with this will become real overrides.
#define override_su

enum class RHRDK_SDK RhRdkMiiOption : unsigned int
{
	None,
	AllowDefaultMaterial,
	ForMaterialCtrl,
	ForLayerMatch,
};

enum class RHRDK_SDK RhRdkMaterialSource : unsigned int
{
	None,
	Layer,
	Parent,
	Object,
};

enum class RHRDK_SDK RhRdkUiModalities : unsigned int
{
	Modeless, // Modeless UIs are non-blocking and usually operate directly on the modeless UI document.
	Modal,    // Modal UIs are blocking. Some operate on temporary ('sandbox') data and others ('semi-modal') operate directly on the modeless UI document.
};

enum class RHRDK_SDK RhRdkUserControl : unsigned int // Indicates which control the user is interacting with.
{
	Unknown,                    // Unknown control.
	MainThumb,                  // The main content editor thumbnail list.
	MainTree,                   // The main content editor tree control.
	EditorPreview,              // The content editor preview thumbnail list.
	SubNodeCtrl,                // A sub-node control.
	ColorButton,                // A color button.
	CreateNewButton,            // A 'Create New' button - AKA [+] button.
	ContentCtrl,                // An old-style content control.
	NewContentCtrl,             // A new-style content control.
	NewContentCtrlDropDown,     // A new-style content control's drop-down thumbnail list.
	BreadcrumbCtrl,             // A breadcrumb control.
	FloatingPreview,            // OBSOLETE
	Spanner,                    // Spanner menu.
	ContentTypeSection,         // Content type section.
	ContentTypeBrowserNew,      // Content New / Existing Browser 'new' page.
	ContentTypeBrowserExisting, // Content New / Existing Browser 'existing' page.
	ContentInstanceBrowser,     // Content instance browser.
	ToolTipPreview,             // Tool-tip preview.
	AddDecalButton,             // The 'Add decal' button on the decals panel.
};

#pragma region RH_C_SHARED_ENUM [RhRdkUiFrameworks] [ANDYENUM] [nested]
	/// <summary>Framework to use for UI elements.</summary>
	enum class RHRDK_SDK RhRdkUiFrameworks : unsigned int
	{
		/// <summary>Win32.</summary>
		Win32,
		/// <summary>Cocoa.</summary>
		Cocoa,
		/// <summary>Eto.</summary>
		Eto,
		/// <summary>WinForms.</summary>
		WinForms,
		/// <summary>WPF.</summary>
		WPF,
	};
#pragma endregion

enum /** Bit-flags for CRhRdkDocument::ShowContentInstanceBrowser() rdkcce_Flags. */
{
	rdkcce_NewButton  = 0x0001,
	rdkcce_EditButton = 0x0002,
};

enum class RHRDK_SDK RhRdkImageDropChoice : unsigned int
{
	None,
	PictureFrame,
	Wallpaper,
	Material,
	Environment,
	Texture,
};

enum class RHRDK_SDK RhRdkChangeContext : int // Context of a change to document or content parameters.
{
	Unset      = -1, // Value when BeginChange() has not been called.
	UI         =  0, // Change occurred as a result of user activity in the UI.
	Drop       =  1, // Change occurred as a result of drag and drop.
	Program    =  2, // Change occurred as a result of internal program activity. Clients should prefer Command as automatic content undo is not recorded for this change.
	Ignore     =  3, // Change can be disregarded.
	Tree       =  4, // Change occurred within the content tree (e.g., nodes reordered).
	Undo       =  5, // Change occurred as a result of an undo or redo operation.
	Reserved   =  6, // Reserved for RDK internal use only [SDK_UNFREEZE]
	Serialize  =  7, // Change occurred during serialization (loading).
	RealTimeUI =  8, // Change occurred as a result of 'real-time' user activity in the (content) UI.
	                 // The content's preview, UI, group members and registerable properties are not updated.
	Script     =  9, // Change occurred during execution of a script. Automatic content undo can be recorded for this change.
	Command    = 10, // Change occurred during execution of a command. Automatic content undo can be recorded for this change.
};

enum class RHRDK_SDK RhRdkFilterContentByUsage : unsigned int
{
	None,         // No filter.
	Used,         // Filter to show only contents used by objects, layers or ground plane.
	Unused,       // Filter to show only contents NOT used by objects, layers or ground plane.
	UsedSelected, // Filter to show only contents used by selected objects.
};

enum class RHRDK_SDK RhRdkAssignToSubFaceChoices : unsigned int
{
	Keep,   // Keep any existing sub-face assignments.
	Remove, // Remove any existing sub-face assignments first.
	Ask,    // Ask the user what to do.
};

enum class RHRDK_SDK RhRdkAssignToBlockChoices : unsigned int
{
	Always, // Always assign to blocks recursively.
	Never,  // Never assign to blocks recursively.
	Ask,    // Ask the user what to do.
};
