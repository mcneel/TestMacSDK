
#pragma once // Public SDK header

#include "RhRdkContentKinds.h"

// Many of these defines are deprecated in favor of the OpenNURBS defines.

#define SS_DOCUMENT                         ON_RDK_DOCUMENT

	#define SS_CONTENT                          ON_RDK_CURRENT_CONTENT

	#define SS_DEFAULT_CONTENT_SECTION          ON_RDK_DEFAULT_CONTENT_SECTION

	#define SS_SETTINGS                             ON_RDK_SETTINGS

		#define SS_NAMED_VIEWS                         ON_RDK_NAMED_VIEWS
		#define SS_NAMED_CPLANES                       ON_RDK_NAMED_CPLANES
		#define SS_NAMED_POSITIONS                     ON_RDK_NAMED_POSITIONS
		#define SS_NAMED_SNAPSHOTS                     ON_RDK_NAMED_SNAPSHOTS
			#define SS_SORT_MODE                           ON_RDK_SORT_MODE
				#define SS_SORT_MODE_ASCENDING                 ON_RDK_SORT_MODE_ASCENDING
				#define SS_SORT_MODE_DESCENDING                ON_RDK_SORT_MODE_DESCENDING
				#define SS_SORT_MODE_CUSTOM                    ON_RDK_SORT_MODE_CUSTOM

		#define SS_MISCELLANEOUS                       ON_RDK_MISCELLANEOUS

			#define SS_CUSTOM_IMAGE_SIZE_IS_PRESET        ON_RDK_CUSTOM_IMAGE_SIZE_IS_PRESET

			#define SS_NAME_COLLISION_PREFIX              L"smart-merge-name-collision-prefix" // OBSOLETE; Not used.

			#define SS_NAME_COLLISION_SUPPRESS            ON_RDK_NAME_COLLISION_SUPPRESS
				#define SS_IMPORT                           ON_RDK_IMPORT
				#define SS_PASTE                            ON_RDK_PASTE

		#define SS_EXCLUDED_RENDER_ENGINES            ON_RDK_EXCLUDED_RENDER_ENGINES

			#define SS_UUIDS                            ON_RDK_UUIDS

		#define SS_FILTERS                           ON_RDK_FILTERS

			#define SS_NAME_FILTER                      ON_RDK_NAME_FILTER
			#define SS_NAME_FILTER_INVERT               ON_RDK_NAME_FILTER_INVERT
			#define SS_SHOW_UNASSIGNED                  ON_RDK_SHOW_UNASSIGNED
			#define SS_SHOW_V4                          ON_RDK_SHOW_V4
			#define SS_SHOW_HIDDEN                      ON_RDK_SHOW_HIDDEN
			#define SS_SHOW_REFERENCE                   ON_RDK_SHOW_REFERENCE

		#define SS_RENDERING                          ON_RDK_RENDERING

			#define SS_RENDER_CHANNELS                ON_RDK_RENDER_CHANNELS

				#define SS_RCH_MODE                     ON_RDK_RCH_MODE
				//----- +++++++++++++
				#define SS_RCH_MODE_AUTOMATIC             ON_RDK_RCH_MODE_AUTOMATIC
				#define SS_RCH_MODE_CUSTOM                ON_RDK_RCH_MODE_CUSTOM

				#define SS_RCH_LIST                     ON_RDK_RCH_LIST

			#define SS_EMBED_SUPPORT_FILES_ON           ON_RDK_EMBED_SUPPORT_FILES_ON
			#define SS_GAMMA                            ON_RDK_POST_PROCESS_GAMMA
			#define SS_USE_LINEAR_WORKFLOW              ON_RDK_PRE_PROCESS_GAMMA_ON
			#define SS_USE_POST_PROCESS_GAMMA           ON_RDK_POST_PROCESS_GAMMA_ON
			#define SS_USE_DITHERING                    ON_RDK_DITHERING_ENABLED
			#define SS_CUSTOM_REFLECTIVE_ENVIRONMENT_ON	ON_RDK_CUSTOM_REFLECTIVE_ENVIRONMENT_ON
			#define SS_CUSTOM_REFLECTIVE_ENVIRONMENT    ON_RDK_CUSTOM_REFLECTIVE_ENVIRONMENT
			#define SS_INCLUDE_TEX_MAT_ON               L"include-texmat-on" // OBSOLETE; Not used.

			// Early post effects.
			#define SS_PEP_GLARE                        L"glare"
			#define SS_PEP_GLARE_GAIN                     L"gain"
			#define SS_PEP_GLARE_WHITE_POINT_BOUND        L"white-point-bound"
			#define SS_PEP_GLARE_AREA_MULTIPLIER          L"area-multiplier"
			#define SS_PEP_GLARE_SAMPLES                  L"samples"
			#define SS_PEP_GLARE_USE_LOG_SCALE            L"use-log-scale"

			#define SS_PEP_BLOOM                        L"bloom"
			#define SS_PEP_BLOOM_BRIGHTNESS_THRESHOLD     L"brightness-threshold"
			#define SS_PEP_BLOOM_RADIUS                   L"radius"
			#define SS_PEP_BLOOM_INTENSITY                L"intensity"

			#define SS_PEP_GLOW                         L"glow"
			#define SS_PEP_GLOW_GAIN                      L"gain"
			#define SS_PEP_GLOW_SENSITIVITY               L"sensitivity"
			#define SS_PEP_GLOW_AREA_VALUE                L"area-value"
			#define SS_PEP_GLOW_ITEM_NUMOF                L"num-colors"
			#define SS_PEP_GLOW_ITEM_FORMAT               L"-%u"
			#define SS_PEP_GLOW_ITEM_COLOR                L"color"
			#define SS_PEP_GLOW_ITEM_ENABLED              L"enabled"
			#define SS_PEP_GLOW_ITEM_SENSITIVITY          L"sensitivity"

			#define SS_PEP_FOG                          L"fog"
			#define SS_PEP_FOG_LEFT                       L"left"
			#define SS_PEP_FOG_TOP                        L"top"
			#define SS_PEP_FOG_RIGHT                      L"right"
			#define SS_PEP_FOG_BOTTOM                     L"bottom"
			#define SS_PEP_FOG_START_DISTANCE             L"start-distance"
			#define SS_PEP_FOG_END_DISTANCE               L"end-distance"
			#define SS_PEP_FOG_STRENGTH                   L"strength"
			#define SS_PEP_FOG_FEATHERING                 L"feathering"
			#define SS_PEP_FOG_NOISE                      L"noise"
			#define SS_PEP_FOG_BACKGROUND                 L"background"
			#define SS_PEP_FOG_COLOR                      L"color"

			#define SS_PEP_DOF                          L"depth-of-field"
			#define SS_PEP_DOF_BLUR_BACKGROUND            L"blur-background"
			#define SS_PEP_DOF_BLUR_FOCAL_DISTANCE        L"focal-distance"
			#define SS_PEP_DOF_BLUR_BLUR_AMOUNT           L"blur-amount"
			#define SS_PEP_DOF_BLUR_MAX_BLURRING          L"max-blurring"

			#define SS_PEP_DITHERING                    L"dithering"

			/**-------------------- Old tone mapping strings ------------------------------------------*/
			/*                                                                                         */
			/*  These are all deprecated. Tone mapping is now a post effect.                           */
			/*                                                                                         */
			/**/ #define SS_TONE_MAPPING                       L"tone-mapping"                       /**/
			/**/ #define SS_TONE_MAPPING_NONE                  L"none"                               /**/
			/**/ #define SS_TONE_MAPPING_BLACK_WHITE_POINT     L"black-white-point"                  /**/
			/**/ #define SS_TONE_MAPPING_LOGARITHMIC           L"logarithmic"                        /**/
			/**/ #define SS_TONE_MAPPING_FILMIC                L"filmic"                             /**/
			/**/ #define SS_TONE_MAPPING_REINHARD_2001         L"reinhard-2001"                      /**/
			/**/ #define SS_TONE_MAPPING_REINHARD_2005         L"reinhard-2005"                      /**/
			/*                                                                                         */
			/**----------------------------------------------------------------------------------------*/

			/*DEPRECATED*/ #define SS_TONE_MAPPING_METHOD L"tone-mapping-method"

			// Tone mapping post effects.
			#define SS_TONEMAP_BLACK_POINT                L"tm-black-point"
			#define SS_TONEMAP_WHITE_POINT                L"tm-white-point"

			#define SS_TONEMAP_LOG_BIAS                   L"bias"
			#define SS_TONEMAP_LOG_EXPOSURE               L"exposure"
			#define SS_TONEMAP_LOG_CONTRAST               L"contrast"
			#define SS_TONEMAP_LOG_MANUAL_LEVEL           L"manual-level"
			#define SS_TONEMAP_LOG_MANUAL_LEVEL_ON        L"manual-level-on"

			#define SS_TONEMAP_FILMIC_PRESET              L"preset"
			#define SS_TONEMAP_FILMIC_PRESET_LOW_CONTRAST    L"low-contrast"
			#define SS_TONEMAP_FILMIC_PRESET_MEDIUM_CONTRAST L"medium-contrast"
			#define SS_TONEMAP_FILMIC_PRESET_HIGH_CONTRAST   L"high-contrast"

			#define SS_TONEMAP_FILMIC_CONTRAST               L"contrast"
			#define SS_TONEMAP_FILMIC_SATURATION             L"saturation"
			#define SS_TONEMAP_FILMIC_EXPOSURE_BIAS          L"exposure-bias"
			#define SS_TONEMAP_FILMIC_SHADOW_OFFSET          L"shadow-offset"
			#define SS_TONEMAP_FILMIC_MIDTONE_OFFSET         L"midtone-offset"
			#define SS_TONEMAP_FILMIC_HIGHLIGHT_OFFSET       L"highlight-offset"

			// Late post effects.
			#define SS_PEP_GAMMA                          L"gamma"

			#define SS_DITHERING                          L"dithering"
			//----- ++++++++++++
			#define SS_DITHERING_NONE                       L"none" // OBSOLETE
			#define SS_DITHERING_FLOYD_STEINBERG            L"floyd-steinberg"
			#define SS_DITHERING_SIMPLE_NOISE               L"simple-noise"

			// Watermark.
			#define SS_PEP_WATERMARK                        L"watermark"
			#define SS_PEP_WATERMARK_FONT_FACE              L"font-face"
			#define SS_PEP_WATERMARK_FONT_COLOR             L"font-color"
			#define SS_PEP_WATERMARK_FONT_POINTS            L"font-points"
			#define SS_PEP_WATERMARK_BACK_COLOR             L"back-color"
			#define SS_PEP_WATERMARK_TRANSPARENCY           L"transparency"
			#define SS_PEP_WATERMARK_CUSTOM_TEXT            L"custom-text"
			#define SS_PEP_WATERMARK_RENDERER_ON            L"renderer-on"
			#define SS_PEP_WATERMARK_START_TIME_ON          L"start-time-on"
			#define SS_PEP_WATERMARK_END_TIME_ON            L"end-time-on"
			#define SS_PEP_WATERMARK_ELAPSED_TIME_ON        L"elapsed-time-on"
			#define SS_PEP_WATERMARK_CUSTOM_TEXT_ON         L"custom-text-on"

			// Hue/Sat/Lum.
			#define SS_PEP_HUESATLUM                        L"hue-sat-lum"
			#define SS_PEP_HUESATLUM_HUE                    L"hue"
			#define SS_PEP_HUESATLUM_SAT                    L"saturation"
			#define SS_PEP_HUESATLUM_LUM                    L"luminance"

			// Bri/Con.
			#define SS_PEP_BRICON                           L"bri-con"
			#define SS_PEP_BRICON_BRI                       L"brightness"
			#define SS_PEP_BRICON_CON                       L"contrast"

			// Multiplier
			#define SS_PEP_MULTIPLIER                       L"multiplier"
			#define SS_PEP_MULTIPLIER_VALUE                 L"multiplier-value"

			// Noise
			#define SS_PEP_NOISE                            L"noise"
			#define SS_PEP_NOISE_AMOUNT                     L"noise-amount"
			#define SS_PEP_NOISE_MONOCHROME                 L"noise-monochrome"

			// Gaussian blur
			#define SS_PEP_GAUSSIAN_BLUR                    L"gaussian-blur"
			#define SS_PEP_GAUSSIAN_BLUR_HORZ               L"gaussian-blur-horz"
			#define SS_PEP_GAUSSIAN_BLUR_VERT               L"gaussian-blur-vert"
			#define SS_PEP_GAUSSIAN_BLUR_AMOUNT             L"gaussian-blur-amount"

			// If you add more, remember to update the CRhRdkDocument SDK comments and XML validation.

			#define SS_TONE_MAPPER_PARAMS                  L"tone-mapper-params" // OBSOLETE; Not used.

		#define SS_SUN                                    ON_RDK_SUN
			#define SS_SUN_ENABLE_ALLOWED                   ON_RDK_SUN_ENABLE_ALLOWED
			#define SS_SUN_ENABLE_ON                        ON_RDK_SUN_ENABLE_ON
			#define SS_SUN_MANUAL_CONTROL_ALLOWED           ON_RDK_SUN_MANUAL_CONTROL_ALLOWED
			#define SS_SUN_MANUAL_CONTROL_ON                ON_RDK_SUN_MANUAL_CONTROL_ON
			#define SS_SUN_NORTH                            ON_RDK_SUN_NORTH
			#define SS_SUN_AZIMUTH                          ON_RDK_SUN_AZIMUTH
			#define SS_SUN_ALTITUDE                         ON_RDK_SUN_ALTITUDE
			#define SS_SUN_DATE_YEAR                        ON_RDK_SUN_DATE_YEAR
			#define SS_SUN_DATE_MONTH                       ON_RDK_SUN_DATE_MONTH
			#define SS_SUN_DATE_DAY                         ON_RDK_SUN_DATE_DAY
			#define SS_SUN_TIME_HOURS                       ON_RDK_SUN_TIME_HOURS
			#define SS_SUN_DAYLIGHT_SAVING_ON               ON_RDK_SUN_DAYLIGHT_SAVING_ON
			#define SS_SUN_DAYLIGHT_SAVING_MINUTES          ON_RDK_SUN_DAYLIGHT_SAVING_MINUTES
			#define SS_SUN_OBSERVER_LATITUDE                ON_RDK_SUN_OBSERVER_LATITUDE
			#define SS_SUN_OBSERVER_LONGITUDE               ON_RDK_SUN_OBSERVER_LONGITUDE
			#define SS_SUN_OBSERVER_TIMEZONE                ON_RDK_SUN_OBSERVER_TIMEZONE

			// Additions for V5.
			#define SS_SUN_SKYLIGHT_ON                      ON_RDK_SUN_SKYLIGHT_ENABLED
			#define SS_SUN_SKYLIGHT_SHADOW_INTENSITY        ON_RDK_SUN_SKYLIGHT_SHADOW_INTENSITY
			#define SS_SUN_SKYLIGHT_CUSTOM_ENVIRONMENT_ON   ON_RDK_SUN_SKYLIGHT_ENVIRONMENT_OVERRIDE
			#define SS_SUN_SKYLIGHT_CUSTOM_ENVIRONMENT      ON_RDK_SUN_SKYLIGHT_ENVIRONMENT_ID

			// Additions for V6.
			#define SS_SUN_SHADOW_INTENSITY                 ON_RDK_SUN_SHADOW_INTENSITY

			// Additions for V7.
			#define SS_SUN_INTENSITY                        ON_RDK_SUN_INTENSITY

		#define SS_SAFE_FRAME                       ON_RDK_SAFE_FRAME
			#define SS_SF_ON                            ON_RDK_SF_ENABLED
			#define SS_SF_PERSPECTIVE_ONLY              ON_RDK_SF_PERSPECTIVE_ONLY
			#define SS_SF_4x3_FIELD_GRID_ON             ON_RDK_SF_4x3_FIELD_GRID_ON
			#define SS_SF_LIVE_FRAME                    ON_RDK_SF_LIVE_FRAME
			#define SS_SF_ACTION_FRAME                  ON_RDK_SF_ACTION_FRAME
			#define SS_SF_TITLE_FRAME                   ON_RDK_SF_TITLE_FRAME
				#define SS_SFF_ON                           ON_RDK_SFF_ON
				#define SS_SFF_XSCALE                       ON_RDK_SFF_XSCALE
				#define SS_SFF_YSCALE                       ON_RDK_SFF_YSCALE
				#define SS_SFF_LINK                         ON_RDK_SFF_LINK

		#define SS_GROUND_PLANE                     ON_RDK_GROUND_PLANE
			#define SS_GP_ON                            ON_RDK_GP_ENABLED
			#define SS_GP_ALTITUDE                      ON_RDK_GP_ALTITUDE
			#define SS_GP_MATERIAL                      ON_RDK_GP_MATERIAL_ID
			#define SS_GP_TEXTURE_OFFSET                ON_RDK_GP_TEXTURE_OFFSET
			#define SS_GP_TEXTURE_SIZE                  ON_RDK_GP_TEXTURE_SIZE
			#define SS_GP_TEXTURE_ROTATION              ON_RDK_GP_TEXTURE_ROTATION
			#define SS_GP_OFFSET_LOCK                   ON_RDK_GP_TEXTURE_OFFSET_LOCKED
			#define SS_GP_REPEAT_LOCK                   ON_RDK_GP_TEXTURE_SIZE_LOCKED
			#define SS_GP_SHOW_UNDERSIDE                ON_RDK_GP_SHOW_UNDERSIDE
			#define SS_GP_AUTO_ALTITUDE                 ON_RDK_GP_AUTO_ALTITUDE
			#define SS_GP_SHADOW_ONLY                   ON_RDK_GP_SHADOW_ONLY

		#define SS_POST_EFFECTS                     L"post-effects" // OBSOLETE; Not used.
			#define SS_PE_PLUG_INS                      L"plug-ins" // OBSOLETE; Not used.

		#define SS_POST_EFFECTS_NEW                 ON_RDK_POST_EFFECTS

			#define SS_APPLY_TONE_MAPPING_WHILE_RENDERING L"apply-tone-mapping-while-rendering" // OBSOLETE

			#define SS_PEP_TYPE_EARLY            ON_RDK_PEP_TYPE_EARLY
			#define SS_PEP_TYPE_TONE             ON_RDK_PEP_TYPE_TONE_MAPPING
			#define SS_PEP_TYPE_LATE             ON_RDK_PEP_TYPE_LATE

			#define SS_PEP_SELECTION                ON_RDK_PEP_SELECTION

			#define SS_PEP_SELECTION_POSTFIX        L"-" SS_PEP_SELECTION

			#define SS_PEP_EARLY_SELECTION          SS_PEP_TYPE_EARLY SS_PEP_SELECTION_POSTFIX
			#define SS_PEP_TONE_SELECTION           SS_PEP_TYPE_TONE  SS_PEP_SELECTION_POSTFIX
			#define SS_PEP_LATE_SELECTION           SS_PEP_TYPE_LATE  SS_PEP_SELECTION_POSTFIX

					#define SS_PEP_PLUG_IN             ON_RDK_PEP_PLUG_IN
					#define SS_PEP_LOCAL_NAME          ON_RDK_PEP_LOCAL_NAME
					#define SS_PEP_ID                  ON_RDK_PEP_ID
					#define SS_PEP_SHOWN               ON_RDK_PEP_SHOWN
					#define SS_PEP_ON                  ON_RDK_PEP_ON
					#define SS_PEP_STATE               ON_RDK_PEP_PARAMS

		#define SS_CUSTOM_RENDER_MESH               L"custom-render-mesh"
			#define SS_PROVIDERS                      L"providers"
				#define SS_PROVIDER                     L"provider"
					#define SS_PROVIDER_ID              L"id"
					#define SS_PROVIDER_ON              L"on"

	#define SS_PREVIEW_SECTION                      L"preview-section"
		#define SS_ITEM                               L"item"
			#define SS_CONTENT_ID                     L"content-id"
			#define SS_SCENE_SCALE                    L"scene-scale"
			#define SS_ANGLE_X                        L"angle-x"
			#define SS_ANGLE_Y                        L"angle-y"
			#define SS_GEOMETRY                       L"geometry"
			#define SS_BACKGROUND                     L"background"
			#define SS_LIGHTING                       L"lighting"
			#define SS_ENVIRONMENT_UUID               L"environment-uuid"

			#define SS_ROTATION_TYPE                  L"rotation-type"
				#define SS_RT_OBJECT                    L"object"
				#define SS_RT_CAMERA                    L"camera"

	#define SS_POSTFIX_SECTION                      L"-section"
		#define SS_NAME                               L"name"
		#define SS_TYPE                               L"type"
		#define SS_UNITS                              L"units"
		#define SS_PREVIEW                            L"preview"
		#define SS_CUSTOM_DATA                        L"custom-data"
		#define SS_PARAMETER                          L"parameter"

		#define SS_TYPE_NAME                          ON_RENDER_CONTENT_TYPE_NAME
		#define SS_TYPE_ID                            ON_RENDER_CONTENT_TYPE_ID
		#define SS_INSTANCE_ID                        ON_RENDER_CONTENT_INSTANCE_ID
		#define SS_RENDER_ENGINE_ID                   ON_RENDER_CONTENT_RENDER_ENGINE_ID
		#define SS_PLUG_IN_ID                         ON_RENDER_CONTENT_PLUG_IN_ID
		#define SS_GROUP_ID                           ON_RENDER_CONTENT_GROUP_ID
		#define SS_INSTANCE_NAME                      ON_RENDER_CONTENT_INSTANCE_NAME
		#define SS_CHILD_SLOT_NAME                    ON_RENDER_CONTENT_CHILD_SLOT_NAME 
		#define SS_NOTES                              ON_RENDER_CONTENT_NOTES
		#define SS_TAGS                               ON_RENDER_CONTENT_TAGS
		#define SS_HIDDEN                             ON_RENDER_CONTENT_HIDDEN
		#define SS_REFERENCE                          ON_RENDER_CONTENT_REFERENCE
		#define SS_AUTO_DELETE                        ON_RENDER_CONTENT_AUTO_DELETE
		#define SS_PARAMETERS                         ON_RENDER_CONTENT_PARAMETERS
		#define SS_PARAMETERS_V8                      ON_RENDER_CONTENT_PARAMETERS_V8
		#define SS_SIMULATION                         ON_RENDER_CONTENT_SIMULATION

		#define SS_REFERENCE_MARKER                   L"reference-marker" // For passing ImportReferenceMode flag from loading to content creation.

		#define SS_ENV_BACKGROUND_COLOR               L"background-color"

	// Starting with V6 we stopped storing the embedded files as Base64 and started storing them
	// as binary blobs. Therefore, these are only for backward compatibility.
	#define SS_EMBEDDED_FILES                   L"embedded-files"
		#define SS_FILE                           L"file"

	// Additions for V8. Texture proxy information is now persistent so that preview meta-data can work.
	#define SS_TP_TEXTURE_PROXY_SECTION         L"texture-proxy-section"
	#define SS_TP_COMBINED                        L"combined"
	#define SS_TP_UNCOMBINED                      L"uncombined"
	#define SS_TP_TEXTURE_PROXY                     L"texture-proxy"
	#define SS_TP_ID                                L"id"
	#define SS_TP_MEMBERS                             L"members"
	#define SS_TP_MEMBER                                L"member"

#define SS_UD_ROOT                              L"render-content-manager-data"

	#define SS_UD_MATERIAL                          L"material"      // }
		#define SS_UD_NAME                            L"name"        // } OBSOLETE; Not used.
		#define SS_UD_INSTANCE_ID                     L"instance-id" // }

	#define SS_UD_DECALS                            ON_RDK_DECALS

		#define SS_UD_DECAL                           ON_RDK_DECAL

			#define SS_UD_DECAL_CUSTOM                  ON_RDK_DECAL_CUSTOM
			#define SS_UD_DECAL_CUSTOM_RENDERER         ON_RDK_DECAL_CUSTOM_RENDERER
			#define SS_UD_DECAL_CUSTOM_PARAMS           ON_RDK_DECAL_CUSTOM_PARAMS

			#define SS_UD_DECAL_INSTANCE_ID               L"instance-id" // OBSOLETE; Not used.

			#define SS_UD_DECAL_MAPPING                   ON_RDK_DECAL_MAPPING
			//----- +++++++++++++++++++
			#define SS_UD_DECAL_MAPPING_PLANAR            ON_RDK_DECAL_MAPPING_PLANAR
			#define SS_UD_DECAL_MAPPING_CYLINDRICAL       ON_RDK_DECAL_MAPPING_CYLINDRICAL
			#define SS_UD_DECAL_MAPPING_SPHERICAL         ON_RDK_DECAL_MAPPING_SPHERICAL
			#define SS_UD_DECAL_MAPPING_UV                ON_RDK_DECAL_MAPPING_UV
			#define SS_UD_DECAL_MAPPING_NONE              ON_RDK_DECAL_MAPPING_NONE

			#define SS_UD_DECAL_PROJECTION                ON_RDK_DECAL_PROJECTION
			//----- ++++++++++++++++++++++
			#define SS_UD_DECAL_PROJECTION_FORWARD        ON_RDK_DECAL_PROJECTION_FORWARD
			#define SS_UD_DECAL_PROJECTION_BACKWARD       ON_RDK_DECAL_PROJECTION_BACKWARD
			#define SS_UD_DECAL_PROJECTION_BOTH           ON_RDK_DECAL_PROJECTION_BOTH
			#define SS_UD_DECAL_PROJECTION_NONE           ON_RDK_DECAL_PROJECTION_NONE

			#define SS_UD_DECAL_MAP_TO_INSIDE_ON          ON_RDK_DECAL_MAP_TO_INSIDE_ON
			#define SS_UD_DECAL_TRANSPARENCY              ON_RDK_DECAL_TRANSPARENCY
			#define SS_UD_DECAL_TEXTURE_INSTANCE          ON_RDK_DECAL_TEXTURE_INSTANCE
			#define SS_UD_DECAL_HEIGHT                    ON_RDK_DECAL_HEIGHT
			#define SS_UD_DECAL_ORIGIN                    ON_RDK_DECAL_ORIGIN
			#define SS_UD_DECAL_RADIUS                    ON_RDK_DECAL_RADIUS
			#define SS_UD_DECAL_HORZ_SWEEP_STA            ON_RDK_DECAL_HORZ_SWEEP_STA
			#define SS_UD_DECAL_HORZ_SWEEP_END            ON_RDK_DECAL_HORZ_SWEEP_END
			#define SS_UD_DECAL_VERT_SWEEP_STA            ON_RDK_DECAL_VERT_SWEEP_STA
			#define SS_UD_DECAL_VERT_SWEEP_END            ON_RDK_DECAL_VERT_SWEEP_END
			#define SS_UD_DECAL_VECTOR_UP                 ON_RDK_DECAL_VECTOR_UP
			#define SS_UD_DECAL_VECTOR_ACROSS             ON_RDK_DECAL_VECTOR_ACROSS
			#define SS_UD_DECAL_MIN_U                     ON_RDK_DECAL_MIN_U
			#define SS_UD_DECAL_MIN_V                     ON_RDK_DECAL_MIN_V
			#define SS_UD_DECAL_MAX_U                     ON_RDK_DECAL_MAX_U
			#define SS_UD_DECAL_MAX_V                     ON_RDK_DECAL_MAX_V
			#define SS_UD_DECAL_IS_TEMPORARY              ON_RDK_DECAL_IS_TEMPORARY
			#define SS_UD_DECAL_IS_VISIBLE                ON_RDK_DECAL_IS_VISIBLE

// Content meta-data
#define SS_CONTENT_META_DATA                    L"meta-data"
	#define SS_CONTENT_META_DATA_RENDERER_NAME    L"renderer-name"
	#define SS_CONTENT_META_DATA_TYPE_NAME        L"type-name"

// Texture simulation.
#define RDK_TEX_SIM_FILENAME         ON_TEXTURE_SIMULATION_FILENAME
#define RDK_TEX_SIM_REPEAT           ON_TEXTURE_SIMULATION_REPEAT
#define RDK_TEX_SIM_OFFSET           ON_TEXTURE_SIMULATION_OFFSET
#define RDK_TEX_SIM_ROTATION         ON_TEXTURE_SIMULATION_ROTATION
#define RDK_TEX_SIM_WRAP_TYPE        ON_TEXTURE_SIMULATION_WRAP_TYPE
#define RDK_TEX_SIM_MAPPING_CHANNEL  ON_TEXTURE_SIMULATION_MAPPING_CHANNEL
#define RDK_TEX_SIM_PROJECTION_MODE  ON_TEXTURE_SIMULATION_PROJECTION_MODE

// Miscellaneous.
#define SS_HDR_INTENSITY         L"intensity" // This is an alias for FS_TEX_HDR_MULTIPLIER.
#define SS_MAX_SIMULATED_HEIGHT  L"max-simulated-height"
