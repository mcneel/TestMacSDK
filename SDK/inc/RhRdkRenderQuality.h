
#pragma once // Public SDK header

enum RDK_DEPRECATED eRhRdkRenderQuality { rcmQualNone, rcmQualLow, rcmQualMedium, rcmQualFull };
RDK_DEPRECATED bool IsRenderQualityHigher(eRhRdkRenderQuality a, eRhRdkRenderQuality b);

// This enum is used by CRhRdkContent::CreatePreview() and CRhRdkRenderPlugIn::CreatePreview().
// The 'Quick' option is only supported by the former, not the latter method.
enum class RhRdkPreviewQuality : unsigned int
{
	None                    = 0, // No quality set.
	Low                     = 1, // Low quality rendering for faster preview.
	Medium                  = 2, // Medium quality rendering for intermediate preview.
	IntermediateProgressive = 3, // Intermediate update, always considered better quality than the previous IntermediateProgressive, but not as high as Full.
	Full                    = 4, // Full quality rendering (quality comes from user settings).
	Quick                   = 5, // Quick preview. Uses a quick OpenGL function instead of the current renderer.
};

RHRDK_SDK bool IsPreviewQualityHigher(RhRdkPreviewQuality a, RhRdkPreviewQuality b);
