
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class CRhRdkRenderPlugIn;

/* Because this is strictly for file dialogs and might even be Microsoft-only, it should probably
   be in the UI plug-in. I need it here while I get everything else that uses it under control.
   Later it might move into the UI plug-in.
*/
class RHRDK_SDK CRhRdkFileDialogFilter : public CRhRdkObject
{
public:
	CRhRdkFileDialogFilter();
	virtual ~CRhRdkFileDialogFilter();

	/** Add a filter to the collection.
		\param sName is the localized name. e.g., "JPEG File"
		\param sFriendlyPattern is the human-readable pattern. e.g., "*.jpg, *.jpeg"
		 Parentheses are added automatically.
		\param wszPattern is the optional machine-readable pattern. e.g., "*.jpg;*.jpeg"
		 If null, uses sFriendlyPattern. */
	virtual void Add(const ON_wString& sName, const ON_wString& sFriendlyPattern, const wchar_t* wszPattern=nullptr);

	/** Get output for use in CFileDialog() or CFileDialog::m_ofn.lpstrFilter.
		\param bConvertBars is \e true to convert bars (|) to zeroes, \e false to leave bars intact.
		 Typically you will pass \e false to use the result as the constructor param for CFileDialog,
		 and \e true when using CFileDialog::m_ofn.lpstrFilter. */
	virtual const wchar_t* Buffer(bool bConvertBars=true) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

enum // Flags for CRhRdkImageFileDialogFilter uFlags parameter.
{
	rdk_fdf_AllSupported = 0x00000001, // Include 'All supported image files'
	rdk_fdf_AllowBasic   = 0x00000002, // Include basic types (*.bmp, *.jpg, *.png, *.tif, *.tga, *.dds)
	rdk_fdf_AllowRimage  = 0x00000004, // Include *.rimage
	rdk_fdf_AllowHDR     = 0x00000008, // Include *.hdr
	rdk_fdf_AllowEXR     = 0x00000010, // Include *.exr
	rdk_fdf_AllowCustom  = 0x00000020, // Include plug-in custom types.
	rdk_fdf_AllowRTEX    = 0x00000040, // Include *.rtex
	rdk_fdf_StarDotStar  = 0x00008000, // Include *.*

	rdk_fdf_Force32bit   = 0xFFFFFFFF
};

class RHRDK_SDK CRhRdkImageFileDialogFilter : public CRhRdkFileDialogFilter
{
public:
	CRhRdkImageFileDialogFilter(unsigned int uFlags, CRhRdkRenderPlugIn* pPlugIn=nullptr);
	virtual ~CRhRdkImageFileDialogFilter();

	// Returns one-based position for use in CFileDialog::m_ofn.nFilterIndex.
	int PositionOfType(const wchar_t* wszType) const;

	ON_wString TypeAtPosition(int) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

class RHRDK_SDK CRhRdkHDRFileDialogFilter : public CRhRdkImageFileDialogFilter
{
public:
	CRhRdkHDRFileDialogFilter();
};

class RHRDK_SDK CRhRdkEXRFileDialogFilter : public CRhRdkImageFileDialogFilter
{
public:
	CRhRdkEXRFileDialogFilter();
};
