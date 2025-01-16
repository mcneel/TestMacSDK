
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

RHRDK_SDK extern UUID uuidUniversalRenderEngine;

// Data UUIDs for IRhinoUiDataSource.
RHRDK_SDK extern UUID uuidData_RdkColorData;                     // Return value must be cast to IRhRdkColorData*
RHRDK_SDK extern UUID uuidData_RdkContentChildSlot;              // Return value must be cast to IRhRdkContentChildSlot*
RHRDK_SDK extern UUID uuidData_RdkContentCtrlInfo;               // Return value must be cast to IRhRdkContentCtrlInfo*
RHRDK_SDK extern UUID uuidData_RdkContentDatabase;               // Return value must be cast to IRhRdkContentCollection*
RHRDK_SDK extern UUID uuidData_RdkContentDisplayCollection;      // Return value must be cast to IRhRdkContentCollection*
RHRDK_SDK extern UUID uuidData_RdkContentEditorSettings;         // Return value must be cast to IRhRdkContentEditorSettings*
RHRDK_SDK extern UUID uuidData_RdkContentFilters;                // Return value must be cast to IRhRdkContentFilters*
RHRDK_SDK extern UUID uuidData_RdkContentLookup;                 // Return value must be cast to IRhRdkContentLookup*
RHRDK_SDK extern UUID uuidData_RdkContentTarget;                 // Return value must be cast to IRhRdkContentTarget*
RHRDK_SDK extern UUID uuidData_RdkContentUIs;                    // Return value must be cast to IRhRdkContentUIs*
RHRDK_SDK extern UUID uuidData_RdkContentUndo;                   // Return value must be cast to IRhRdkContentUndoDS*
RHRDK_SDK extern UUID uuidData_RdkCurrentEnvironment;            // Return value must be cast to IRhRdkCurrentEnvironment*
RHRDK_SDK extern UUID uuidData_RdkCustomCurve;                   // Return value must be cast to IRhRdkCustomCurve*
RHRDK_SDK extern UUID uuidData_RdkDataSourceFactory;             // Return value must be cast to IRhRdkDataSourceFactory*
RHRDK_SDK extern UUID uuidData_RdkDecals;                        // Return value must be cast to IRhRdkDecals*
RHRDK_SDK extern UUID uuidData_RdkDithering;                     // Return value must be cast to ON_Dithering*
RHRDK_SDK extern UUID uuidData_RdkDragDropTopLevel;              // Return value must be cast to IRhRdkDragDrop*
RHRDK_SDK extern UUID uuidData_RdkDragDropSubNode;               // Return value must be cast to IRhRdkDragDrop*
RHRDK_SDK extern UUID uuidData_RdkDragDropColor;                 // Return value must be cast to IRhRdkDragDrop*
RHRDK_SDK extern UUID uuidData_RdkDragDropLibraries;             // Return value must be cast to IRhRdkDragDrop*
RHRDK_SDK extern UUID uuidData_RdkDragDropNamedItems;            // Return value must be cast to IRhRdkDragDrop*
RHRDK_SDK extern UUID uuidData_RdkDragDropPostEffects;           // Return value must be cast to IRhRdkDragDrop*
RHRDK_SDK extern UUID uuidData_RdkEdit;                          // Return value must be cast to IRhRdkEdit*
RHRDK_SDK extern UUID uuidData_RdkEditor;                        // Return value must be cast to IRhRdkEditor*
RHRDK_SDK extern UUID uuidData_RdkFindFile;                      // Return value must be cast to IRhRdkFindFile*
RHRDK_SDK extern UUID uuidData_RdkFolderNavigator;               // Return value must be cast to IRhRdkFolderNavigator*
RHRDK_SDK extern UUID uuidData_RdkGroundPlane;                   // Return value must be cast to ON_GroundPlane*
RHRDK_SDK extern UUID uuidData_RdkImageFile_V8;                  // Return value must be cast to IRhRdkImageFile_V8*
RHRDK_SDK extern UUID uuidData_RdkIORMenuData;                   // Return value must be cast to IRhRdkIORMenuData*
RHRDK_SDK extern UUID uuidData_RdkLibraries;                     // Return value must be cast to IRhRdkLibraries*
RHRDK_SDK extern UUID uuidData_RdkLinearWorkflow;                // Return value must be cast to ON_LinearWorkflow*
RHRDK_SDK extern UUID uuidData_RdkNamedItems;                    // Return value must be cast to IRhRdkNamedItems*
RHRDK_SDK extern UUID uuidData_RdkObjectMaterialAssignBy;        // Return value must be cast to IRhRdkNewContentCtrl::IAssignBy.
RHRDK_SDK extern UUID uuidData_RdkParamBlock;                    // Return value must be cast to IRhRdkParamBlock*
RHRDK_SDK extern UUID uuidData_RdkPEP_Fog;                       // Return value must be cast to IRhRdkPEP_Fog*
RHRDK_SDK extern UUID uuidData_RdkPEP_DOF;                       // Return value must be cast to IRhRdkPEP_DOF*
RHRDK_SDK extern UUID uuidData_RdkPEP_Glare;                     // Return value must be cast to IRhRdkPEP_Glare*
RHRDK_SDK extern UUID uuidData_RdkPEP_Glow;                      // Return value must be cast to IRhRdkPEP_Glow*
RHRDK_SDK extern UUID uuidData_RdkPreviewProperties;             // Return value must be cast to IRhRdkPreviewProperties*
RHRDK_SDK extern UUID uuidData_RdkPreviewRendered;               // Return value must be cast to IRhRdkPreviewRendered*
RHRDK_SDK extern UUID uuidData_RdkPreviewSettings;               // Return value must be cast to IRhRdkPreviewSettings*
RHRDK_SDK extern UUID uuidData_RdkRenderChannels;                // Return value must be cast to ON_RenderChannels*
RHRDK_SDK extern UUID uuidData_RdkRendering;                     // Return value must be cast to IRhRdkRendering*
RHRDK_SDK extern UUID uuidData_RdkRenderingProgress;             // Return value must be cast to IRhRdkRenderingProgress*
RHRDK_SDK extern UUID uuidData_RdkRenderingPostEffects;          // Return value must be cast to IRhRdkRenderingPostEffects*
RHRDK_SDK extern UUID uuidData_RdkRenderingPostEffectProgress;   // Return value must be cast to IRhRdkRenderingPostEffectProgress*
RHRDK_SDK extern UUID uuidData_RdkRenderingHistogram;            // Return value must be cast to IRhRdkRenderingHistogram*
RHRDK_SDK extern UUID uuidData_RdkRenderWindowSidePaneUI;        // Return value must be cast to IRhRdkRenderWindowSidePaneUI*
RHRDK_SDK extern UUID uuidData_RdkRhinoSettings;                 // Return value must be cast to IRhRdkRhinoSettings*
RHRDK_SDK extern UUID uuidData_RdkSelectionNavigator;            // Return value must be cast to IRhRdkSelectionNavigator*
RHRDK_SDK extern UUID uuidData_RdkSkylight;                      // Return value must be cast to ON_Skylight*
RHRDK_SDK extern UUID uuidData_RdkSun;                           // Return value must be cast to ON_Sun*
RHRDK_SDK extern UUID uuidData_RdkTextureMapping;                // Return value must be cast to IRhRdkTextureMapping*
RHRDK_SDK extern UUID uuidData_RdkTextureMapping_Channels;       // Return value must be cast to IRhRdkTextureMapping_Channels*
RHRDK_SDK extern UUID uuidData_RdkTextureMapping_Mappings;       // Return value must be cast to IRhRdkTextureMapping_Mappings*
RHRDK_SDK extern UUID uuidData_RdkTinyIcons;                     // Return value must be cast to IRhRdkTinyIcons_V8*
RHRDK_SDK extern UUID uuidData_RdkUndo;                          // Return value must be cast to IRhRdkUndo*.

// Deprecated.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkImageAdjust;          // OBSOLETE.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkImageFile;            // OBSOLETE.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkNewContentUndo;       // Deprecated; please use uuidData_RdkContentUndo instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkNewUndoRecord;        // Deprecated; please use uuidData_RdkUndo instead.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkRenderingGamma;       // OBSOLETE.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkRenderingToneMapping; // OBSOLETE.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkToneMapping;          // OBSOLETE.
RDK_DEPRECATED RHRDK_SDK extern UUID uuidData_RdkSafeFrame;            // OBSOLETE.

// Content selection data UUIDs.

// Use to get and set the selection but NOT to change the parameters or type of the contents in that selection.
RHRDK_SDK extern UUID uuidData_RdkContentSelection; // Return value must be cast to IRhRdkContentCollection*.

// Use to get the selection for thumbnail displays. Returns Texture Proxies intact (not their members).
RHRDK_SDK extern UUID uuidData_RdkContentSelectionForPreview; // Return value must be cast to IRhRdkContentCollection*.

// Use to get the selection so you can change the parameters (but NOT the type) of the contents in that selection.
RHRDK_SDK extern UUID uuidData_RdkContentSelectionForSetParams; // Return value must be cast to IRhRdkContentCollection*.

// Use to get the selection so you can change the type (but NOT the parameters) of the contents in that selection.
RHRDK_SDK extern UUID uuidData_RdkContentSelectionForChangeType; // Return value must be cast to IRhRdkContentCollection*

// Materials.
RHRDK_SDK extern UUID uuidBasicMaterialType;
RHRDK_SDK extern UUID uuidBlendMaterialType;
RHRDK_SDK extern UUID uuidCompositeMaterialType;
RHRDK_SDK extern UUID uuidPlasterMaterialType;
RHRDK_SDK extern UUID uuidMetalMaterialType;
RHRDK_SDK extern UUID uuidPaintMaterialType;
RHRDK_SDK extern UUID uuidPlasticMaterialType;
RHRDK_SDK extern UUID uuidGemMaterialType;
RHRDK_SDK extern UUID uuidGlassMaterialType;
RHRDK_SDK extern UUID uuidPictureMaterialType;
RHRDK_SDK extern UUID uuidDefaultMaterialInstance;
RHRDK_SDK extern UUID uuidPhysicallyBasedMaterialType;
RHRDK_SDK extern UUID uuidDoubleSidedMaterialType;
RHRDK_SDK extern UUID uuidEmissionMaterialType;
RHRDK_SDK extern UUID uuidDisplayAttributeMaterialType;
RHRDK_SDK extern UUID uuidV8BlendMaterialType;

// Display material used to support front and back materials for realtime rendering in the viewport.
RHRDK_SDK extern UUID uuidRealtimeDisplayMaterialType;

// Environments.
RHRDK_SDK extern UUID uuidBasicEnvironmentType;
RHRDK_SDK extern UUID uuidDefaultEnvironmentInstance;

// Textures.
RHRDK_SDK extern UUID uuid2DCheckerTextureType;
RHRDK_SDK extern UUID uuid3DCheckerTextureType;
RHRDK_SDK extern UUID uuidAdvancedDotTextureType;
RHRDK_SDK extern UUID uuidBitmapTextureType;
RHRDK_SDK extern UUID uuidBlendTextureType;
RHRDK_SDK extern UUID uuidCubeMapTextureType;
RHRDK_SDK extern UUID uuidExposureTextureType;
RHRDK_SDK extern UUID uuidFBmTextureType;
RHRDK_SDK extern UUID uuidGradientTextureType;
RHRDK_SDK extern UUID uuidGraniteTextureType;
RHRDK_SDK extern UUID uuidGridTextureType;
RHRDK_SDK extern UUID uuidAddTextureType;
RHRDK_SDK extern UUID uuidMultiplyTextureType;
RHRDK_SDK extern UUID uuidHDRTextureType;
RHRDK_SDK extern UUID uuidEXRTextureType; // Deprecated in favor of uuidHDRTextureType.
RHRDK_SDK extern UUID uuidMarbleTextureType;
RHRDK_SDK extern UUID uuidMaskTextureType;
RHRDK_SDK extern UUID uuidNoiseTextureType;
RHRDK_SDK extern UUID uuidPerlinMarbleTextureType;
RHRDK_SDK extern UUID uuidPerturbingTextureType;
RHRDK_SDK extern UUID uuidProjectionChangerTextureType;
RHRDK_SDK extern UUID uuidResampleTextureType;
RHRDK_SDK extern UUID uuidSingleColorTextureType;
RHRDK_SDK extern UUID uuidSimpleBitmapTextureType;
RHRDK_SDK extern UUID uuidStuccoTextureType;
RHRDK_SDK extern UUID uuidTextureAdjustmentTextureType;
RHRDK_SDK extern UUID uuidTileTextureType;
RHRDK_SDK extern UUID uuidTurbulenceTextureType;
RHRDK_SDK extern UUID uuidWavesTextureType;
RHRDK_SDK extern UUID uuidWoodTextureType;
RHRDK_SDK extern UUID uuidPhysicalSkyTextureType;

// Simple bump textures
RHRDK_SDK extern UUID uuidHatchBumpTexture;
RHRDK_SDK extern UUID uuidCrossHatchBumpTexture;
RHRDK_SDK extern UUID uuidLeatherBumpTexture;
RHRDK_SDK extern UUID uuidWoodBumpTexture;
RHRDK_SDK extern UUID uuidSpeckleBumpTexture;
RHRDK_SDK extern UUID uuidGritBumpTexture;
RHRDK_SDK extern UUID uuidDotBumpTexture;

// Compound content implementors.
RHRDK_SDK extern UUID uuidBasicMaterialCCI;
RHRDK_SDK extern UUID uuidBlendMaterialCCI;
RHRDK_SDK extern UUID uuidCompositeMaterialCCI;
RHRDK_SDK extern UUID uuidBasicEnvironmentCCI;

RHRDK_SDK extern UUID uuidV8BlendMaterialCCI;

// Post-effect plug-ins.
RHRDK_SDK extern UUID uuidDepthOfFieldPEP;
RHRDK_SDK extern UUID uuidFogPEP;
RHRDK_SDK extern UUID uuidGlarePEP;
RHRDK_SDK extern UUID uuidGlowPEP;

// Custom render mesh providers.
RHRDK_SDK extern UUID uuidEdgeSofteningCRMProvider;
RHRDK_SDK extern UUID uuidDisplacementCRMProvider;
RHRDK_SDK extern UUID uuidCurvePipingCRMProvider;
RHRDK_SDK extern UUID uuidShutLiningCRMProvider;
RHRDK_SDK extern UUID uuidThickeningCRMProvider;
RHRDK_SDK extern UUID uuidSubObjectMaterialSplitterCRMProvider;

// Extension interfaces. For internal use only.
/*RDK_DEPRECATED*/ RHRDK_SDK extern UUID uuidRenderWindowEventSinkInterface;
RHRDK_SDK extern UUID uuidRenderWindowMouseEventWatcherInterface;
RHRDK_SDK extern UUID uuidRenderWindowCustomDlgInterface;
RHRDK_SDK extern UUID uuidCustomUserDataFactoryInterface;
RHRDK_SDK extern UUID uuidContentEditorCustomButtonProvider;
RHRDK_SDK extern UUID uuidCustomRenderFrameTabFactoryInterface;
RHRDK_SDK extern UUID uuidCustomRenderFrameMenuInterface;
RHRDK_SDK extern UUID uuidCustomRenderFrameToolBarInterface;
/*RDK_DEPRECATED*/ RHRDK_SDK extern UUID uuidPostEffectPlugInFactoryInterface;
RHRDK_SDK extern UUID uuidPostEffectFactoryInterface;
RHRDK_SDK extern UUID uuidLightManagerSupportInterface;
RHRDK_SDK extern UUID uuidTaskInterface;

// Features - See CRhinoRenderPlugIn::RenderWindow / CRhinoRenderPlugIn::Render

RHRDK_SDK extern UUID uuidFeatureMaterials;     // }
RHRDK_SDK extern UUID uuidFeatureEnvironments;  // } These 2 features determine if the relevant content editor is enabled.
/*RDK_DEPRECATED*/ RHRDK_SDK extern UUID uuidFeatureTextures; // DEPRECATED. Textures cannot be disabled.

RHRDK_SDK extern UUID uuidFeatureCustomReflectionEnvironment;
RHRDK_SDK extern UUID uuidFeatureCustomRenderMeshes;
RHRDK_SDK extern UUID uuidFeatureCustomSkylightEnvironment;
RHRDK_SDK extern UUID uuidFeatureDecals;
RHRDK_SDK extern UUID uuidFeatureExposure;
RHRDK_SDK extern UUID uuidFeatureFocalBlur;
RHRDK_SDK extern UUID uuidFeatureGroundPlane;
RHRDK_SDK extern UUID uuidFeatureLightMaterials;
//RDK_SDK extern UUID uuidFeatureLinearWorkflow; // See below.
RHRDK_SDK extern UUID uuidFeaturePostEffects;
RHRDK_SDK extern UUID uuidFeatureRenderArcticCommand;
RHRDK_SDK extern UUID uuidFeatureRenderBlowupCommand;
RHRDK_SDK extern UUID uuidFeatureRenderChannels;
RHRDK_SDK extern UUID uuidFeatureRenderInWindowCommand;
RHRDK_SDK extern UUID uuidFeatureRenderViewSource;
RHRDK_SDK extern UUID uuidFeatureRenderWindowCommand;
RHRDK_SDK extern UUID uuidFeatureShadowOnlyGroundPlane;
RHRDK_SDK extern UUID uuidFeatureSkyLight;
RHRDK_SDK extern UUID uuidFeatureSun;

/** Note that if you want to support the Linear Workflow settings in Rhino, you will need to do
	the following in your renderer:

	1) Return true for uuidFeatureLinearWorkflow in your CRhRdkPlugIn::SupportsFeature() override.

	2) Check that Linear Workflow is on using

	         doc.Properties().RenderSettings().LinearWorkflow().PreProcessGammaOn()

	   and if so:

	3) Convert all colors that you get from Rhino to their linear equivalent using
	   CRhRdkColor::ApplyLinearWorkflowGamma() / ON_LinearWorkflow::ApplyPreProcessGamma()

	4) Convert all texture colors EXCLUDING HDR TEXTURES (those that return true for
	   CRhRdkTexture::IsLinear), or textures that are used in a bump or normal map, using 
	   CRhRdkColor::ApplyLinearWorkflowGamma().
	   NOTE: You can call CRhRdkMaterial::TextureRequiresLinearWorkflowAdjustment() to determine if
	   the texture requires it.

	5) If you are not using the built-in render window, apply the gamma value in
	   ON_LinearWorkflow::PostProcessGamma(). NOTE: while Rhino 5 automatically did some of the gamma
	   correction work in SimulateMaterial(), this is not the case in Rhino 6 upwards. Everything now
	   has to be done explicitly by the client.
*/
RHRDK_SDK extern UUID uuidFeatureLinearWorkflow;

/** Varies. */
RHRDK_SDK extern UUID uuidVaries;

/** Custom Curve UI section. */
RHRDK_SDK extern UUID uuidCustomCurveSection;

/** The following three UUIDs are the identifiers of the three main content editors.
	By 'main content editor' we mean the modeless RDK content editor, containing a thumbnail list and a UI.
	\note Each UUID uniquely identifies a concrete content editor of a certain kind. However, with
	 multiple-document support, there can potentially be more than one editor of a given kind open
	 at the same time, especially on the Mac. These UUIDs therefore only identify the first editor
	 of a certain kind. Other editors of the same kind will have modified versions of these UUIDs.
*/
RHRDK_SDK extern UUID uuidMaterialEditor;
RHRDK_SDK extern UUID uuidEnvironmentEditor;
RHRDK_SDK extern UUID uuidTextureEditor; // AKA 'Texture Palette'.

// Tasks.
RHRDK_SDK extern UUID uuidTaskAssignToLayers;
RHRDK_SDK extern UUID uuidTaskAssignToLayersOfObjects;
RHRDK_SDK extern UUID uuidTaskAssignToObjects;
RHRDK_SDK extern UUID uuidTaskAssignToGroundPlane;
RHRDK_SDK extern UUID uuidTaskChange_HarvestAlways;
RDK_DEPRECATED RHRDK_SDK extern UUID uuidTaskChange_HarvestNever;
RDK_DEPRECATED RHRDK_SDK extern UUID uuidTaskChange_HarvestUseOption;
RHRDK_SDK extern UUID uuidTaskChoose;
RHRDK_SDK extern UUID uuidTaskChooseExisting;
RHRDK_SDK extern UUID uuidTaskClose;
RHRDK_SDK extern UUID uuidTaskContentCtrlChange;
RHRDK_SDK extern UUID uuidTaskContentCtrlProperties;
RHRDK_SDK extern UUID uuidTaskContentCtrlRemove;
RHRDK_SDK extern UUID uuidTaskContentFilter;
RHRDK_SDK extern UUID uuidTaskCopy;
RDK_DEPRECATED RHRDK_SDK extern UUID uuidTaskCreateNew;
RHRDK_SDK extern UUID uuidTaskCreatePBRFromFiles;
RHRDK_SDK extern UUID uuidTaskDelete;
RHRDK_SDK extern UUID uuidTaskDuplicate;
RHRDK_SDK extern UUID uuidTaskFilterByContentUsage_None;
RHRDK_SDK extern UUID uuidTaskFilterByContentUsage_Unused;
RHRDK_SDK extern UUID uuidTaskFilterByContentUsage_Used;
RHRDK_SDK extern UUID uuidTaskFilterByContentUsage_UsedSelected;
RHRDK_SDK extern UUID uuidTaskFilterByContentTags;
RDK_DEPRECATED RHRDK_SDK extern UUID uuidTaskFloatingPreview; // OBSOLETE
RHRDK_SDK extern UUID uuidTaskFloatingEditor;
RHRDK_SDK extern UUID uuidTaskFloatThumbPane;
RHRDK_SDK extern UUID uuidTaskFloatTreePane;
RHRDK_SDK extern UUID uuidTaskImportFromFile;
RHRDK_SDK extern UUID uuidTaskLayoutHorz;
RHRDK_SDK extern UUID uuidTaskMergeIdenticalMaterials;
RHRDK_SDK extern UUID uuidTaskModalEdit;
RHRDK_SDK extern UUID uuidTaskModeGrid;
RHRDK_SDK extern UUID uuidTaskModeList;
RHRDK_SDK extern UUID uuidTaskModeTree;
RHRDK_SDK extern UUID uuidTaskMoreTypes;
RHRDK_SDK extern UUID uuidTaskAlphaSort;
RHRDK_SDK extern UUID uuidTaskPaste;
RHRDK_SDK extern UUID uuidTaskPasteAsInstance;
RHRDK_SDK extern UUID uuidTaskPreviewAutoUpdate;
RHRDK_SDK extern UUID uuidTaskPreviewGeomCone;
RHRDK_SDK extern UUID uuidTaskPreviewGeomCube;
RHRDK_SDK extern UUID uuidTaskPreviewGeomPlane;
RHRDK_SDK extern UUID uuidTaskPreviewGeomPyramid;
RHRDK_SDK extern UUID uuidTaskPreviewGeomScene;
RHRDK_SDK extern UUID uuidTaskPreviewGeomSphere;
RHRDK_SDK extern UUID uuidTaskPreviewGeomTorus;
RHRDK_SDK extern UUID uuidTaskPreviewProperties;
RHRDK_SDK extern UUID uuidTaskPreviewShowLabels;
RHRDK_SDK extern UUID uuidTaskPreviewShowUnits;
RHRDK_SDK extern UUID uuidTaskPreviewSizeLarge;
RHRDK_SDK extern UUID uuidTaskPreviewSizeMedium;
RHRDK_SDK extern UUID uuidTaskPreviewSizeSmall;
RHRDK_SDK extern UUID uuidTaskPreviewSizeTiny;
RHRDK_SDK extern UUID uuidTaskPreviewUpdateAll;
RHRDK_SDK extern UUID uuidTaskPreviewUpdateNow;
RHRDK_SDK extern UUID uuidTaskRemoveCurrent_Lighting;
RHRDK_SDK extern UUID uuidTaskRemoveCurrent_ReflRefr;
RHRDK_SDK extern UUID uuidTaskRemoveInstancing;
RHRDK_SDK extern UUID uuidTaskReplace;
RHRDK_SDK extern UUID uuidTaskResetToDefaults;
RHRDK_SDK extern UUID uuidTaskSaveAsImage;
RHRDK_SDK extern UUID uuidTaskSaveToFile;
RHRDK_SDK extern UUID uuidTaskSelectAll;
RHRDK_SDK extern UUID uuidTaskSelectObjects;
RHRDK_SDK extern UUID uuidTaskSetAsCurrent_All;
RHRDK_SDK extern UUID uuidTaskSetAsCurrent_OverrideChannels;
RHRDK_SDK extern UUID uuidTaskSetAsCurrent_Background;
RHRDK_SDK extern UUID uuidTaskSetAsCurrent_Lighting;
RHRDK_SDK extern UUID uuidTaskSetAsCurrent_ReflRefr;
RHRDK_SDK extern UUID uuidTaskShowPreviewPane;
RHRDK_SDK extern UUID uuidTaskSubNodeCtrlChange;
RHRDK_SDK extern UUID uuidTaskSubNodeCtrlEdit;
RHRDK_SDK extern UUID uuidTaskSubNodeCtrlProperties;
RHRDK_SDK extern UUID uuidTaskSubNodeCtrlRemove;
RHRDK_SDK extern UUID uuidTaskTags;
RHRDK_SDK extern UUID uuidTaskTreeCollapseAll;
RHRDK_SDK extern UUID uuidTaskTreeExpandAll;

// Special values for CRhRdkRenderPlugIn::UiContentTypes().
RHRDK_SDK extern UUID uuidUiContentType_Separator; // Specifies a separator item is desired.

// Panels are now in RhinoSdkDialog.h
//RHRDK_SDK extern UUID uuidPanelLightManager;
//RHRDK_SDK extern UUID uuidPanelLibraries;
//RHRDK_SDK extern UUID uuidPanelFileExplorer;
//RHRDK_SDK extern UUID uuidPanelGroundPlane;
//RHRDK_SDK extern UUID uuidPanelRenderSettings;
//RHRDK_SDK extern UUID uuidPanelSun;
//RHRDK_SDK extern UUID uuidPanelMaterials;
//RHRDK_SDK extern UUID uuidPanelEnvironments;
//RHRDK_SDK extern UUID uuidPanelTextures;

// OBSOLETE Render window tabs.
RHRDK_SDK RDK_DEPRECATED extern UUID uuidExposureRenderWindowTab;
RHRDK_SDK RDK_DEPRECATED extern UUID uuidPostEffectRenderWindowTab;

// Post effect tab ids.
RHRDK_SDK extern UUID uuidTabPostEffect_Early;
RHRDK_SDK extern UUID uuidTabPostEffect_ToneMapping;
RHRDK_SDK extern UUID uuidTabPostEffect_Late;

// Render presets.
RHRDK_SDK extern UUID uuidRenderPreset_Studio;
RHRDK_SDK extern UUID uuidRenderPreset_Custom;
RHRDK_SDK extern UUID uuidRenderPreset_Exterior;
RHRDK_SDK extern UUID uuidRenderPreset_Interior;

// Render Settings sections.
RHRDK_SDK extern UUID uuidRenderSettingsSection_CurrentRenderer;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Resolution;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Background;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Lighting;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Wireframe;
RHRDK_SDK extern UUID uuidRenderSettingsSection_ColorAdjustment;
RHRDK_SDK extern UUID uuidRenderSettingsSection_View;

// Render Settings sections ETO.
RHRDK_SDK extern UUID uuidRenderSettingsSection_CurrentRenderer_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Resolution_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Background_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Lighting_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Wireframe_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_ColorAdjustment_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_View_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_RenderChannels_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Presets_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Studio_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Exterior_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_Interior_Eto;
RHRDK_SDK extern UUID uuidRenderSettingsSection_GroundPlane_Eto;

///////////////////////////////
// C u s t o m   e v e n t s
///////////////////////////////

// OBSOLETE.
RHRDK_SDK extern UUID uuidRenderWindowCloned;

// Custom event for custom UI sections added.
RHRDK_SDK extern UUID uuidEventAddCustomUISections;

// Custom event for asynchronous baking completed.
RHRDK_SDK extern UUID uuidEventAsynchronousBakingCompleted;

// Custom event for 'Combine similar textures' check box.
RHRDK_SDK extern UUID uuidEventCombineSimilarTexturesCheck;

// Custom event for 'Content hidden (or un-hidden)'.
RHRDK_SDK extern UUID uuidEventContentHidden;

// Custom event for document post effects change.
RHRDK_SDK extern UUID uuidEventDocumentPostEffectsChanged;

// Custom event for preview meta data saved.
RHRDK_SDK extern UUID uuidEventPreviewMetaDataSaved;

// Custom event for linear workflow change.
RHRDK_SDK extern UUID uuidEventLinearWorkflowChanged;

// Custom event for render channels change.
RHRDK_SDK extern UUID uuidEventRenderChannelsChanged;

// Custom event for post effect parameter change.
RHRDK_SDK extern UUID uuidEventPostEffectParamChanged;

// Custom event for external view change.
RHRDK_SDK extern UUID uuidEventExternalViewChange;

// Custom event for decal user data transformation.
RHRDK_SDK extern UUID uuidEventDecalUserDataTransformed;

// Early post effects.
RHRDK_SDK extern UUID uuidPostEffect_Glare;
RHRDK_SDK extern UUID uuidPostEffect_Bloom;
RHRDK_SDK extern UUID uuidPostEffect_Glow;
RHRDK_SDK extern UUID uuidPostEffect_Fog;
RHRDK_SDK extern UUID uuidPostEffect_DepthOfField;
RHRDK_SDK extern UUID uuidPostEffect_Multiplier;
RHRDK_SDK extern UUID uuidPostEffect_Noise;
RHRDK_SDK extern UUID uuidPostEffect_GaussianBlur;
RHRDK_SDK extern UUID uuidPostEffect_WireframePointsRGBA;
RHRDK_SDK extern UUID uuidPostEffect_WireframeCurvesRGBA;
RHRDK_SDK extern UUID uuidPostEffect_WireframeIsocurvesRGBA;
RHRDK_SDK extern UUID uuidPostEffect_WireframeAnnotationsRGBA;

// Tone mapping post effects.
RHRDK_SDK extern UUID uuidPostEffect_ToneMapper_Clamp;
RHRDK_SDK extern UUID uuidPostEffect_ToneMapper_BlackWhitePoint;
RHRDK_SDK extern UUID uuidPostEffect_ToneMapper_Logarithmic;
RHRDK_SDK extern UUID uuidPostEffect_ToneMapper_FalseColor;
RHRDK_SDK extern UUID uuidPostEffect_ToneMapper_Filmic;

// Late post effects.
RHRDK_SDK extern UUID uuidPostEffect_Gamma;
RHRDK_SDK extern UUID uuidPostEffect_Dithering;
RHRDK_SDK extern UUID uuidPostEffect_Watermark;
RHRDK_SDK extern UUID uuidPostEffect_HueSatLum;
RHRDK_SDK extern UUID uuidPostEffect_BriCon;
