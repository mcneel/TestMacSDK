
#pragma once // Public SDK header

/** Field strings.

	This file contains strings for the UI plug-in to use with GetParameter() and SetParameter().

	This is needed because most of the contents with UIs are not in the RDK SDK.

	######## NOTE WELL: The strings on the right side are stored in documents and must not be changed.
	########            Similarly, do not change the order of enums. Append new ones to the end.

	Child slot names are included in this file. They start with CS_ instead of FS_
*/

// General parameters used by materials that simulate ON_Material.
#define FS_MAT_ALPHA_TRANSPARENCY                 ON_MATERIAL_ALPHA_TRANSPARENCY
#define FS_MAT_AMBIENT                            ON_MATERIAL_AMBIENT
#define FS_MAT_DIFFUSE                            ON_MATERIAL_DIFFUSE
#define FS_MAT_DISABLE_LIGHTING                   ON_MATERIAL_DISABLE_LIGHTING
#define FS_MAT_EMISSION                           ON_MATERIAL_EMISSION
#define FS_MAT_FLAMINGO_LIBRARY                   ON_MATERIAL_FLAMINGO_LIBRARY
#define FS_MAT_FRESNEL_ENABLED                    ON_MATERIAL_FRESNEL_ENABLED
//#define FS_MAT_FROST_AMOUNT                     "frost-amount" // This is the inverse of "polish" - used in glass
#define FS_MAT_CLARITY_AMOUNT                     ON_MATERIAL_CLARITY_AMOUNT
#define FS_MAT_IOR                                ON_MATERIAL_IOR
#define FS_MAT_POLISH_AMOUNT                      ON_MATERIAL_POLISH_AMOUNT
#define FS_MAT_REFLECTION                         ON_MATERIAL_SIM_REFLECTION_COLOR
#define FS_MAT_REFLECTIVITY_AMOUNT                ON_MATERIAL_REFLECTIVITY_AMOUNT
#define FS_MAT_REFLECTIVITY_COLOR                 ON_MATERIAL_REFLECTIVITY_COLOR
#define FS_MAT_SHINE                              ON_MATERIAL_SHINE // Value goes from 0.0 to 1.0, NOT ON_Material::MaxShine.
#define FS_MAT_SPECULAR                           ON_MATERIAL_SPECULAR
#define FS_MAT_TRANSPARENCY_AMOUNT                ON_MATERIAL_TRANSPARENCY_AMOUNT
#define FS_MAT_TRANSPARENCY_COLOR                 ON_MATERIAL_TRANSPARENCY_COLOR

#define FS_MAT_IS_PHYSICALLY_BASED                ON_MATERIAL_IS_PHYSICALLY_BASED

#define FS_MAT_PBR_BRDF                           ON_PBR_MATERIAL_BRDF
#define   FS_MAT_PBR_BRDF_GGX                       ON_PBR_MATERIAL_BRDF_GGX
#define   FS_MAT_PBR_BRDF_WARD                      ON_PBR_MATERIAL_BRDF_WARD

#define FS_MAT_PBR_ALPHA                          ON_PBR_MATERIAL_ALPHA
#define FS_MAT_PBR_ANISOTROPIC                    ON_PBR_MATERIAL_ANISOTROPIC
#define FS_MAT_PBR_ANISOTROPIC_ROTATION           ON_PBR_MATERIAL_ANISOTROPIC_ROTATION
#define FS_MAT_PBR_BASE_COLOR                     ON_PBR_MATERIAL_BASE_COLOR
#define FS_MAT_PBR_CLEARCOAT                      ON_PBR_MATERIAL_CLEARCOAT
#define FS_MAT_PBR_CLEARCOAT_ROUGHNESS            ON_PBR_MATERIAL_CLEARCOAT_ROUGHNESS
#define FS_MAT_PBR_EMISSION_COLOR                 ON_PBR_MATERIAL_EMISSION_COLOR
#define FS_MAT_PBR_METALLIC                       ON_PBR_MATERIAL_METALLIC
#define FS_MAT_PBR_OPACITY                        ON_PBR_MATERIAL_OPACITY
#define FS_MAT_PBR_OPACITY_IOR                    ON_PBR_MATERIAL_OPACITY_IOR
#define FS_MAT_PBR_OPACITY_ROUGHNESS              ON_PBR_MATERIAL_OPACITY_ROUGHNESS
#define FS_MAT_PBR_ROUGHNESS                      ON_PBR_MATERIAL_ROUGHNESS
#define FS_MAT_PBR_SHEEN                          ON_PBR_MATERIAL_SHEEN
#define FS_MAT_PBR_SHEEN_TINT                     ON_PBR_MATERIAL_SHEEN_TINT
#define FS_MAT_PBR_SPECULAR                       ON_PBR_MATERIAL_SPECULAR
#define FS_MAT_PBR_SPECULAR_TINT                  ON_PBR_MATERIAL_SPECULAR_TINT
#define FS_MAT_PBR_SUBSURFACE                     ON_PBR_MATERIAL_SUBSURFACE
#define FS_MAT_PBR_SUBSURFACE_SCATTERING_COLOR    ON_PBR_MATERIAL_SUBSURFACE_SCATTERING_COLOR
#define FS_MAT_PBR_SUBSURFACE_SCATTERING_RADIUS   ON_PBR_MATERIAL_SUBSURFACE_SCATTERING_RADIUS
#define FS_MAT_PBR_USE_BASE_COLOR_TEXTURE_ALPHA   ON_PBR_MATERIAL_USE_BASE_COLOR_TEXTURE_ALPHA

#define CS_MAT_PBR_ALPHA                          FS_MAT_PBR_ALPHA
#define CS_MAT_PBR_AMBIENT_OCCLUSION              L"pbr-ambient-occlusion"
#define CS_MAT_PBR_ANISOTROPIC                    FS_MAT_PBR_ANISOTROPIC
#define CS_MAT_PBR_ANISOTROPIC_ROTATION           FS_MAT_PBR_ANISOTROPIC_ROTATION
#define CS_MAT_PBR_BASE_COLOR                     FS_MAT_PBR_BASE_COLOR
#define CS_MAT_PBR_BUMP                           L"pbr-bump"
#define CS_MAT_PBR_CLEARCOAT                      FS_MAT_PBR_CLEARCOAT
#define CS_MAT_PBR_CLEARCOAT_BUMP                 L"pbr-clearcoat-bump"
#define CS_MAT_PBR_CLEARCOAT_ROUGHNESS            FS_MAT_PBR_CLEARCOAT_ROUGHNESS
#define CS_MAT_PBR_DISPLACEMENT                   L"pbr-displacement"
#define CS_MAT_PBR_EMISSION_COLOR                 FS_MAT_PBR_EMISSION_COLOR
#define CS_MAT_PBR_METALLIC                       FS_MAT_PBR_METALLIC
#define CS_MAT_PBR_OPACITY                        FS_MAT_PBR_OPACITY
#define CS_MAT_PBR_OPACITY_IOR                    FS_MAT_PBR_OPACITY_IOR
#define CS_MAT_PBR_OPACITY_ROUGHNESS              FS_MAT_PBR_OPACITY_ROUGHNESS
#define CS_MAT_PBR_ROUGHNESS                      FS_MAT_PBR_ROUGHNESS
#define CS_MAT_PBR_SHEEN                          FS_MAT_PBR_SHEEN
#define CS_MAT_PBR_SHEEN_TINT                     FS_MAT_PBR_SHEEN_TINT
#define CS_MAT_PBR_SPECULAR                       FS_MAT_PBR_SPECULAR
#define CS_MAT_PBR_SPECULAR_TINT                  FS_MAT_PBR_SPECULAR_TINT
#define CS_MAT_PBR_SUBSURFACE                     FS_MAT_PBR_SUBSURFACE
#define CS_MAT_PBR_SUBSURFACE_SCATTERING_COLOR    FS_MAT_PBR_SUBSURFACE_SCATTERING_COLOR
#define CS_MAT_PBR_SUBSURFACE_SCATTERING_RADIUS   FS_MAT_PBR_SUBSURFACE_SCATTERING_RADIUS

#define FS_MAT_POSTFIX_ON                         L"on"
#define FS_MAT_POSTFIX_AMOUNT                     L"amount"
#define FS_MAT_POSTFIX_FILTER_ON                  L"filter-on"

#define FS_MAT_BITMAP_TEXTURE                     L"bitmap-texture"
#define FS_MAT_BUMP_TEXTURE                       L"bump-texture"
#define FS_MAT_TRANSPARENCY_TEXTURE               L"transparency-texture"
#define FS_MAT_ENVIRONMENT_TEXTURE                L"environment-texture"

#define CS_MAT_BITMAP_TEXTURE                     L"bitmap-texture"
#define CS_MAT_BUMP_TEXTURE                       L"bump-texture"
#define CS_MAT_TRANSPARENCY_TEXTURE               L"transparency-texture"
#define CS_MAT_ENVIRONMENT_TEXTURE                L"environment-texture"

// Basic material color property.

#define FS_MAT_COLOR                              L"color"

// Gem material.
#define FS_MAT_GEM_TYPE                           L"type"
  #define FS_MAT_GEM_TYPE_AMBER                     L"amber"
  #define FS_MAT_GEM_TYPE_AMETHYST                  L"amethyst"
  #define FS_MAT_GEM_TYPE_AQUAMARINE                L"aquamarine"
  #define FS_MAT_GEM_TYPE_DIAMOND                   L"diamond"
  #define FS_MAT_GEM_TYPE_EMERALD                   L"emerald"
  #define FS_MAT_GEM_TYPE_RUBY                      L"ruby"
  #define FS_MAT_GEM_TYPE_SAPPHIRE                  L"sapphire"

// Picture material.
#define FS_MAT_PIC_ALPHA_TRANSPARENCY_ON          L"alpha-transparency"
#define FS_MAT_PIC_OBJECT_TRANSPARENCY_ON         L"object-transparency"
#define FS_MAT_PIC_SELF_ILLUMINATED               L"self-illuminated"
#define FS_MAT_PIC_GRAYSCALE_ON                   L"grayscale"
#define FS_MAT_PIC_TRANSPARENCY_AMOUNT            L"transparency"
#define FS_MAT_PIC_HAS_TRANSPARENT_COLOR          L"has-transparent-color"
#define FS_MAT_PIC_TRANSPARENT_COLOR              L"transparent-color"
#define FS_MAT_PIC_TRANSPARENT_COLOR_SENS         L"transparent-color-sensitivity"

// Emission material
#define FS_MAT_EMISSION_INTENSITY_AMOUNT          L"intensity"

// Textured bump materials
#define FS_MAT_BUMP_TYPE                          L"bump-type"
#define FS_MAT_BUMP_SCALE                         L"bump-scale"

// Composite material.
#define FS_COMPOSITE_MATERIAL                     L"material-"   // 'base' or slot number (1 to 9) is appended.
#define FS_COMPOSITE_ON                           L"on-"         //                  "
#define FS_COMPOSITE_MIX_AMOUNT                   L"mix-amount-" //                  "
#define FS_COMPOSITE_MIX_TYPE                     L"mix-type-"   //                  "
  #define FS_COMPOSITE_MIX_TYPE_ADD                 L"add"
  #define FS_COMPOSITE_MIX_TYPE_SUBTRACT            L"subtract"
  #define FS_COMPOSITE_MIX_TYPE_MULTIPLY            L"multiply"
  #define FS_COMPOSITE_MIX_TYPE_BLEND               L"blend"

// Blend material.
#define FS_MAT_BLEND_MIX_AMOUNT                   L"mix-amount"
#define CS_MAT_BLEND_MATERIAL_1                   L"material-1"
#define CS_MAT_BLEND_MATERIAL_2                   L"material-2"

// Blend texture.
#define FS_TEX_BLEND_FACTOR                       L"blend-factor"
#define FS_TEX_BLEND_TEXTURE_ON                   L"texture-on"
  #define CS_TEX_BLEND_TEXTURE                      L"blend-texture"

// Basic environment.
#define FS_ENV_BACKGROUND_COLOR                   L"background-color"
#define FS_ENV_BACKGROUND_IMAGE                   L"background-image"
#define FS_ENV_BACKGROUND_PROJ                    L"background-projection"
  #define FS_ENV_PROJ_AUTOMATIC                     L"automatic"
  #define FS_ENV_PROJ_BOX                           L"box"
  #define FS_ENV_PROJ_CUBE_MAP                      L"cubemap"
  #define FS_ENV_PROJ_EMAP                          L"emap"
  #define FS_ENV_PROJ_CUBE_MAP_HORZ                 L"horizontal-cross-cubemap"
  #define FS_ENV_PROJ_CUBE_MAP_VERT                 L"vertical-cross-cubemap"
  #define FS_ENV_PROJ_HEMISPHERICAL                 L"hemispherical"
  #define FS_ENV_PROJ_LIGHT_PROBE                   L"lightprobe"
  #define FS_ENV_PROJ_PLANAR                        L"planar"
  #define FS_ENV_PROJ_SPHERICAL                     L"spherical"

// Bitmap texture.
#define FS_TEX_FILTER_ON                          L"filter"
#define FS_TEX_HAS_TRANSPARENT_COLOR              L"has-transparent-color"
#define FS_TEX_MIRROR_ALTERNATE_TILES             L"mirror-alternate-tiles"
#define FS_TEX_TREAT_AS_LINEAR                    L"treat-as-linear"
#define FS_TEX_TRANSPARENT_COLOR                  L"transparent-color"
#define FS_TEX_TRANSPARENT_COLOR_SENSITIVITY      L"transparent-color-sensitivity"
#define FS_TEX_USE_ALPHA                          L"use-alpha-channel"

// Two color texture common params.
#define FS_TEX_TWO_COLOR_COLOR1                   L"color-one"
#define FS_TEX_TWO_COLOR_COLOR2                   L"color-two"
#define FS_TEX_TWO_COLOR_SWAP_COLORS              L"swap-colors"
#define FS_TEX_TWO_COLOR_SUPERSAMPLE              L"super-sample"
#define FS_TEX_TWO_COLOR_TEXTURE_ON1              L"texture-on-one"
#define FS_TEX_TWO_COLOR_TEXTURE_ON2              L"texture-on-two"
#define FS_TEX_TWO_COLOR_TEXTURE_AMOUNT1          L"texture-amount-one"
#define FS_TEX_TWO_COLOR_TEXTURE_AMOUNT2          L"texture-amount-two"
#define FS_TEX_TWO_COLOR_TILE                     L"tile"
#define CS_TEX_TWO_COLOR_COLOR_1                  L"color-one"
#define CS_TEX_TWO_COLOR_COLOR_2                  L"color-two"

// FBM texture.
#define FS_TEX_FBM_MAX_OCTAVES                    L"max-octaves"
#define FS_TEX_FBM_ROUGHNESS                      L"roughness"
#define FS_TEX_FBM_GAIN                           L"gain"

// Granite texture.
#define FS_TEX_GRANITE_SIZE                       L"size"
#define FS_TEX_GRANITE_SPOT_SIZE                  L"spot-size"
#define FS_TEX_GRANITE_BLENDING                   L"blending"

// Mask texture.
#define FS_TEX_MASK_TYPE                          L"mask-type"
  #define FS_TEX_MASK_TYPE_LUMINANCE                L"luminance"
  #define FS_TEX_MASK_TYPE_RED                      L"red"
  #define FS_TEX_MASK_TYPE_GREEN                    L"green"
  #define FS_TEX_MASK_TYPE_BLUE                     L"blue"
  #define FS_TEX_MASK_TYPE_ALPHA                    L"alpha"
  #define CS_TEX_MASK_INPUT_TEXTURE                 L"source-texture"

// Gradient texture.
#define FS_TEX_GRADIENT_TYPE                      L"gradient-type" // This is an int, not a string :-(
  enum class RhRdkGradientType : unsigned int { Linear, Box, Radial, Tartan, Sweep, Pong, Spiral };

#define FS_TEX_GRADIENT_FLIP_ALTERNATE            L"flip-alternate"
#define FS_TEX_GRADIENT_CUSTOM_CURVE_ON           L"custom-curve"
#define FS_TEX_GRADIENT_CUSTOM_CURVE_DATA         L"custom-curve-data"

// Grid texture.
#define FS_TEX_GRID_CELLS                         L"cells"
#define FS_TEX_GRID_FONT_THICKNESS                L"font-thickness"

// Exposure texture.
#define FS_TEX_EXPOSURE_EXPOSURE                  L"exposure"
#define FS_TEX_EXPOSURE_MULTIPLIER                L"multiplier"
#define CS_TEX_EXPOSURE_INPUT_TEXTURE             L"input-texture"
#define FS_TEX_EXPOSURE_WORLD_LUMINANCE           L"world-luminance" // Lazily computed from input texture.
#define FS_TEX_EXPOSURE_MAX_LUMINANCE             L"max-luminance"   // Lazily computed from input texture.

// Perlin marble texture.
#define FS_TEX_PERLIN_MARBLE_SIZE                 L"size"
#define FS_TEX_PERLIN_MARBLE_LEVELS               L"levels"
#define FS_TEX_PERLIN_MARBLE_NOISE                L"noise"
#define FS_TEX_PERLIN_MARBLE_BLUR                 L"blur"
#define FS_TEX_PERLIN_MARBLE_SAT_COLOR_1          L"color-1-saturation"
#define FS_TEX_PERLIN_MARBLE_SAT_COLOR_2          L"color-2-saturation"

// Checker textures.
#define FS_TEX_CHECKER_REMAP_TEXTURES             L"remap-textures"

// Dot texture.
#define FS_TEX_DOT_SAMPLER_TYPE                   L"sampler-type"
  enum class RhRdkDotSamplerType : unsigned int { BestCandidate, Stratified };

#define FS_TEX_DOT_FALL_OFF_TYPE                  L"fall-off-type"
  enum class RhRdkDotFallOffType : unsigned int { Flat, Linear, Cubic, Elliptic };

#define FS_TEX_DOT_COMPOSITION_TYPE               L"composition"
  enum class RhRdkDotCompositionType : unsigned int { Maximum, Addition, Subtraction, Multiplication, Average, Standard };

#define FS_TEX_DOT_SAMPLE_AREA_SIZE               L"sample-area-size"
#define FS_TEX_DOT_BEST_CANDIDATE_NUMBER_OF_DOTS  L"best-candidate-number-of-dots"
#define FS_TEX_DOT_STRATIFIED_JITTER              L"stratified-jitter"
#define FS_TEX_DOT_STRATIFIED_GRID_SIZE           L"stratified-grid-size"
#define FS_TEX_DOT_STRATIFIED_DOT_PROBABILITY     L"stratified-dot-probability"
#define FS_TEX_DOT_RADIUS                         L"radius"
#define FS_TEX_DOT_RADIUS_VARY                    L"radius-vary"
#define FS_TEX_DOT_AMPLITUDE                      L"amplitude"
#define FS_TEX_DOT_AMPLITUDE_VARY                 L"amplitude-vary"
#define FS_TEX_DOT_HUE_VARY                       L"hue-vary"
#define FS_TEX_DOT_SATURATION_VARY                L"saturation-vary"
#define FS_TEX_DOT_RINGS                          L"rings"
#define FS_TEX_DOT_RING_RADIUS                    L"ring-radius"
#define FS_TEX_DOT_RING_RADIUS_VARY               L"ring-radius-vary"

// Waves texture.
#define FS_TEX_WAVES_CONTRAST1                    L"contrast1"
#define FS_TEX_WAVES_CONTRAST2                    L"contrast2"
#define FS_TEX_WAVES_WAVE_TYPE                    L"wave-type" // This is an int, not a string :-(
  enum class RhRdkWaveType : unsigned int { Linear, Radial };

#define FS_TEX_WAVES_WAVE_WIDTH                   L"wave-width"
#define FS_TEX_WAVES_WAVE_WIDTH_TEX_ON            L"wave-width-tex-on"
#define CS_WAVES_WAVE_WIDTH                       L"wave-width-tex"

// Wood texture.
#define FS_TEX_WOOD_GRAIN_THICKNESS               L"grain-thickness"
#define FS_TEX_WOOD_RADIAL_NOISE                  L"radial-noise"
#define FS_TEX_WOOD_AXIAL_NOISE                   L"axial-noise"
#define FS_TEX_WOOD_BLUR_1                        L"blur-1"
#define FS_TEX_WOOD_BLUR_2                        L"blur-2"

// Stucco texture.
#define FS_TEX_STUCCO_SIZE                        L"size"
#define FS_TEX_STUCCO_THICKNESS                   L"thickness"
#define FS_TEX_STUCCO_THRESHOLD                   L"threshold"

// Marble texture.
#define FS_TEX_MARBLE_SIZE                        L"size"
#define FS_TEX_MARBLE_VEIN_WIDTH                  L"vein-width"
#define FS_TEX_MARBLE_BLUR                        L"blue" // [sic] 'blue' is a typo but it's now in old documents...
#define FS_TEX_MARBLE_NOISE                       L"noise"

// Single color texture.
#define FS_TEX_SINGLE_COLOR_USE_OBJECT_COLOR      L"use-object-color"

// Perturbing texture.
#define FS_TEX_PERTURBING_AMOUNT                  L"amount"
#define CS_TEX_PERTURBING_SOURCE_TEXTURE          L"source"
#define CS_TEX_PERTURBING_PERTURB_TEXTURE         L"perturb"

// Resample texture.
#define FS_TEX_RESAMPLE_U_DIVISIONS               L"u-division-count"
#define FS_TEX_RESAMPLE_V_DIVISIONS               L"v-division-count"
#define FS_TEX_RESAMPLE_MIN_SAMPLES               L"min-sample-set-count"
#define FS_TEX_RESAMPLE_MAX_SAMPLES               L"max-sample-set-count"
#define FS_TEX_RESAMPLE_INTERPOLATE               L"interpolate"
#define FS_TEX_RESAMPLE_BLUR_ON                   L"blur-on"
#define FS_TEX_RESAMPLE_BLUR_RADIUS               L"blur-radius"
#define FS_TEX_RESAMPLE_BLUR_WRAP_U               L"blur-u-tiled"
#define FS_TEX_RESAMPLE_BLUR_WRAP_V               L"blur-v-tiled"
#define FS_TEX_RESAMPLE_BLUR_TYPE                 L"blur-type"
  #define FS_TEX_RESAMPLE_BLUR_TYPE_BOX             L"box"
  #define FS_TEX_RESAMPLE_BLUR_TYPE_TRIANGLE        L"triangle"
  #define FS_TEX_RESAMPLE_BLUR_TYPE_GAUSSIAN        L"gaussian"
  #define FS_TEX_RESAMPLE_BLUR_TYPE_MITNET          L"mitnet"
#define CS_TEX_RESAMPLE_TEXTURE                   L"texture"

// Projection changer texture (see also FS_PROJECTION_*).
#define FS_TEX_PROJ_CHANGE_AZIMUTH                L"azimuth"
#define FS_TEX_PROJ_CHANGE_ALTITUDE               L"altitude"
#define FS_TEX_PROJ_CHANGE_PROJECTION_IN          L"input-projection"
#define FS_TEX_PROJ_CHANGE_PROJECTION_OUT         L"output-projection"
#define CS_TEX_PROJ_CHANGE_INPUT_TEXTURE          L"input-texture"

// High dynamic range textures (see also FS_PROJECTION_*).
#define FS_TEX_HDR_MULTIPLIER                     L"multiplier"
#define FS_TEX_HDR_AZIMUTH                        L"azimuth"
#define FS_TEX_HDR_ALTITUDE                       L"altitude"
#define FS_TEX_HDR_TONE_MAPPING                   L"tone-mapping"
  #define FS_TEX_HDR_TONE_MAPPING_NONE              L"none"
  #define FS_TEX_HDR_TONE_MAPPING_BLACK_WHITE_POINT L"black-white-point"
  #define FS_TEX_HDR_TONE_MAPPING_LOGARITHMIC       L"logarithmic"
  #define FS_TEX_HDR_TONE_MAPPING_REINHARD_2001     L"reinhard-2001"
  #define FS_TEX_HDR_TONE_MAPPING_REINHARD_2005     L"reinhard-2005"

#define FS_TEX_HDR_FILTER                         L"filter"

// Texture adjustment texture.
#define FS_TEX_ADJUSTMENT_TEXTURE_INVERT          L"invert"
#define FS_TEX_ADJUSTMENT_TEXTURE_GRAYSCALE       L"grayscale"
#define FS_TEX_ADJUSTMENT_TEXTURE_MULTIPLIER      L"multiplier"
#define FS_TEX_ADJUSTMENT_TEXTURE_CLAMP           L"clamp"
#define FS_TEX_ADJUSTMENT_TEXTURE_CLAMP_MIN       L"clamp-min"
#define FS_TEX_ADJUSTMENT_TEXTURE_CLAMP_MAX       L"clamp-max"
#define FS_TEX_ADJUSTMENT_TEXTURE_SCALE_TO_CLAMP  L"scale-to-clamp"
#define FS_TEX_ADJUSTMENT_TEXTURE_GAIN            L"gain"
#define FS_TEX_ADJUSTMENT_TEXTURE_GAMMA           L"gamma"
#define FS_TEX_ADJUSTMENT_TEXTURE_SATURATION      L"saturation"
#define FS_TEX_ADJUSTMENT_TEXTURE_HUE_SHIFT       L"hue-shift"
#define FS_TEX_ADJUSTMENT_TEXTURE_FLIP_VERTICAL   L"flip-vertically"
#define FS_TEX_ADJUSTMENT_TEXTURE_FLIP_HORIZONTAL L"flip-horizontally"
#define CS_TEX_ADJUSTMENT_TEXTURE_INPUT_TEXTURE   L"input-texture"

// Noise texture.
#define FS_TEX_NOISE_TYPE                         L"noise-type"
  #define FS_TEX_NOISE_TYPE_PERLIN                  L"perlin"
  #define FS_TEX_NOISE_TYPE_VALUE_NOISE             L"valuenoise"
  #define FS_TEX_NOISE_TYPE_PERLIN_PLUS_VALUE       L"perlin_plus_value"
  #define FS_TEX_NOISE_TYPE_SIMPLEX                 L"simplex"
  #define FS_TEX_NOISE_TYPE_SPARSE_CONVOLUTION      L"sparseconvolution"
  #define FS_TEX_NOISE_TYPE_LATTICE_CONVOLUTION     L"latticeconvolution"
  #define FS_TEX_NOISE_TYPE_WARDS_HERMITE           L"wardshermite"
  #define FS_TEX_NOISE_TYPE_AALTONEN                L"aaltonen"
#define FS_TEX_NOISE_SPECSYNTH_TYPE               L"spectral-synthesis-type"
  #define FS_TEX_NOISE_SPECSYNTH_TYPE_FRACTAL_SUM   L"fractalsum"
  #define FS_TEX_NOISE_SPECSYNTH_TYPE_TURBULENCE    L"turbulence"
#define FS_TEX_NOISE_OCTAVE_COUNT                 L"octave-count"
#define FS_TEX_NOISE_FREQUENCY_MULT               L"frequency-multiplier"
#define FS_TEX_NOISE_AMPLITUDE_MULT               L"amplitude-multiplier"
#define FS_TEX_NOISE_CLAMP_MIN                    L"clamp-min"
#define FS_TEX_NOISE_CLAMP_MAX                    L"clamp-max"
#define FS_TEX_NOISE_SCALE_TO_CLAMP               L"scale-to-clamp"
#define FS_TEX_NOISE_INVERSE                      L"inverse" // This should be 'invert' but I'm keeping 'inverse' because of old documents.
#define FS_TEX_NOISE_GAIN                         L"gain"

// Tile texture.
#define FS_TEX_TILE_TYPE                          L"tile-type"
  #define FS_TEX_TILE_TYPE_3D_RECTANGULAR           L"3d-rectangular"
  #define FS_TEX_TILE_TYPE_2D_RECTANGULAR           L"2d-rectangular"
  #define FS_TEX_TILE_TYPE_2D_TRIANGULAR            L"2d-triangular"
  #define FS_TEX_TILE_TYPE_2D_HEXAGONAL             L"2d_hexagonal"
  #define FS_TEX_TILE_TYPE_2D_OCTAGONAL             L"2d_octagonal"
#define FS_TEX_TILE_WIDTH_X                       L"width-x"
#define FS_TEX_TILE_WIDTH_Y                       L"width-y"
#define FS_TEX_TILE_WIDTH_Z                       L"width-z"
#define FS_TEX_TILE_PHASE_X                       L"phase-x"
#define FS_TEX_TILE_PHASE_Y                       L"phase-y"
#define FS_TEX_TILE_PHASE_Z                       L"phase-z"

// General parameters used by most textures.
#define FS_TEX_OFFSET                             L"rdk-texture-offset"
#define FS_TEX_REPEAT                             L"rdk-texture-repeat"
#define FS_TEX_SIZE                               L"rdk-texture-size"
#define FS_TEX_ROTATION                           L"rdk-texture-rotation"
#define FS_TEX_WRAP_TYPE                          L"rdk-texture-wrap-type"
#define FS_TEX_MAPPING_CHANNEL                    L"rdk-texture-mapping-channel"
#define FS_TEX_PROJECTION_MODE                    L"rdk-texture-projection-mode"
#define FS_TEX_ENVIRONMENT_MAPPING_MODE           L"rdk-texture-environment-mapping-mode"
#define FS_TEX_OFFSET_LOCKED                      L"rdk-texture-offset-locked"
#define FS_TEX_REPEAT_LOCKED                      L"rdk-texture-repeat-locked"
#define FS_TEX_PREVIEW_LOCAL_MAPPING              L"rdk-texture-preview-local-mapping"
#define FS_TEX_PREVIEW_IN_3D                      L"rdk-texture-preview-in-3D"
#define FS_TEX_DISPLAY_IN_VIEWPORT                L"rdk-texture-display-in-viewport"
#define FS_TEX_ADJUST_INVERT                      L"rdk-texture-adjust-invert"
#define FS_TEX_ADJUST_GRAYSCALE                   L"rdk-texture-adjust-grayscale"
#define FS_TEX_ADJUST_CLAMP                       L"rdk-texture-adjust-clamp"
#define FS_TEX_ADJUST_SCALE_TO_CLAMP              L"rdk-texture-adjust-scale-to-clamp"
#define FS_TEX_ADJUST_MULTIPLIER                  L"rdk-texture-adjust-multiplier"
#define FS_TEX_ADJUST_CLAMP_MIN                   L"rdk-texture-adjust-clamp-min"
#define FS_TEX_ADJUST_CLAMP_MAX                   L"rdk-texture-adjust-clamp-max"
#define FS_TEX_ADJUST_GAIN                        L"rdk-texture-adjust-gain"
#define FS_TEX_ADJUST_SATURATION                  L"rdk-texture-adjust-saturation"
#define FS_TEX_ADJUST_HUE_SHIFT                   L"rdk-texture-adjust-hue-shift"
#define FS_TEX_ADJUST_GAMMA                       L"rdk-texture-adjust-gamma"

// File name used by file textures.
#define FS_TEX_FILENAME                           ON_RENDER_TEXTURE_FILENAME

// Projections used by projection changer texture and high dynamic range textures.
// NOT the same as ON_ENVIRONMENT_PROJECTION_*
#define FS_PROJECTION_CUBE_MAP                    L"cube-map"
#define FS_PROJECTION_EMAP                        L"emap"
#define FS_PROJECTION_EQUIRECT                    L"equirect"
#define FS_PROJECTION_CUBE_MAP_HORZ               L"horizontal-cross-cube-map"
#define FS_PROJECTION_CUBE_MAP_VERT               L"vertical-cross-cube-map"
#define FS_PROJECTION_HEMISPHERICAL               L"hemispherical"
#define FS_PROJECTION_LIGHT_PROBE                 L"light-probe"
#define FS_PROJECTION_PLANAR                      L"planar"
#define FS_PROJECTION_SAME_AS_INPUT               L"same-as-input"
