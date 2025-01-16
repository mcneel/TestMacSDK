
#pragma once // Public SDK header

#include "RhRdkObject.h"
#include "RhRdkContent.h"

/** Although most of the RDK UI is now in the RDK Eto UI plug-in, there are a few
	dialogs that are required to remain in the RDK core as SDK services. These are:

	- A standard message box with 'Don't show this again' support. \see RhRdkMessageBox()

	- A file dialog for choosing content(s) to load. \see RhRdkShowLoadContentFileDialog()

	- A file dialog for choosing a content to save. \see RhRdkShowSaveContentFileDialog()

	- A previewing file dialog. \see CRhRdkFileDialogWithMoreTypesOption  **** NOT IN THE SDK ****

*/

/** Message box severity for RhRdkMessageBox() */
enum class RhRdkMbSeverity : unsigned int
{
	kError, kWarning, kQuestion, kInformation
};

/** Message box buttons for RhRdkMessageBox() */
enum class RhRdkMbButtons : unsigned int
{
	kOk, kOkCancel, kYesNo, kYesNoCancel, kYesToAllYesNoCancel
};

/** Message box default button for RhRdkMessageBox() */
enum class RhRdkMbDefaultButton : unsigned int
{
	kButton1, // Button 1 is 'OK' or 'Yes' (or 'Yes to All' when there are four buttons).
	kButton2, // Button 2 is 'No' or 'Cancel' (or 'Yes' when there are four buttons).
	kButton3, // Button 3 is 'Cancel' (or 'No' when there are four buttons).
	kButton4, // Button 4 is 'Cancel' when there are four buttons.

	// Prefer these when there are 2 or 3 buttons.
	kOkYes = kButton1,
	kNoCancel = kButton2,
	kCancelWhenThreeButtons = kButton3,
	kCancelWhenFourButtons = kButton4,
};

/** Message box result for RhRdkMessageBox() */
enum class RhRdkMbResult : unsigned int
{
	kOk, kCancel, kYes, kNo, kYesToAll
};

/** Simple information message box function. Shows message text, information icon and an OK button. */
RHRDK_SDK void RhRdkInfoBox(const wchar_t* wszMessage);

/** Simple error message box function. Shows message text, error icon and an OK button. */
RHRDK_SDK void RhRdkErrorBox(const wchar_t* wszMessage);

/** General message box function.
	\param wszMessage is the localized message to display.
	\param wszYesVerb Is the localized text to use on the 'OK' or 'Yes' button (Mac only).
	 If this string is empty or null, 'OK' or 'Yes' will be used by default.
	\param wszNoVerb Is the localized text to use on the 'No' button (Mac only).
	 If this string is empty or null, 'No' will be used by default.
	\param s is the severity of the message. This determines the icon used.
	\param b specifies the button(s) to display.
	\param wszDontShowAgainIdent is a unique identifier used as a key to record that the user asked to not show this message again.
	\param d specifies the button that will be activated if the user presses RETURN, or if the message box is not shown because of "don't show me again"
	\return the user's choice (\see RhRdkMbResult). */
RHRDK_SDK RhRdkMbResult RhRdkMessageBox(const wchar_t* wszMessage, RhRdkMbDefaultButton d, const wchar_t* wszYesVerb, const wchar_t* wszNoVerb,
                                        RhRdkMbSeverity s = RhRdkMbSeverity::kInformation, RhRdkMbButtons b = RhRdkMbButtons::kOk, const wchar_t* wszDontShowAgainIdent = nullptr);

RDK_DEPRECATED
RHRDK_SDK RhRdkMbResult RhRdkMessageBox(const wchar_t* wszMessage, const wchar_t* wszYesVerb, const wchar_t* wszNoVerb,
                                        RhRdkMbSeverity s=RhRdkMbSeverity::kInformation, RhRdkMbButtons b=RhRdkMbButtons::kOk,
                                        const wchar_t* wszDontShowAgainIdent = nullptr, RhRdkMbDefaultButton d = RhRdkMbDefaultButton::kButton1);

/** Show a file dialog for loading one or more contents.
	\param kind is the content kind.
	\param bMultiSelect is \e true to allow multiple file selections, else \e false.
	\param aFullPaths accepts one or more file names chosen by the user.
	\return \e true if the user chose a file, \e false if cancelled. */
RHRDK_SDK bool RhRdkShowLoadContentFileDialog(CRhRdkContent::Kinds kind, bool bMultiSelect, OUT ON_ClassArray<ON_wString>& aFullPaths);

/** Show a file dialog for saving a content.
	\param c is the content being saved.
	\param sFullPath accepts the full path chosen by the user.
	\return \e true if the user chose a file, \e false if cancelled. */
RHRDK_SDK bool RhRdkShowSaveContentFileDialog(const CRhRdkContent& c, OUT ON_wString& sFullPath);

/** Show a file dialog for choosing image file names for textures.
	\param pRdkDoc is an optional document.
	\param wszInitialFullPath is the initial path to the file and folder to display.
	\param bMultiSelect is \e true to allow multiple file selections, else \e false.
	\param aFullPaths accepts one or more file names chosen by the user.
	\return \e true if the user chose a file, \e false if cancelled. */
RHRDK_SDK bool RhRdkGetTextureImageFilenames(const CRhRdkDocument* pRdkDoc, const wchar_t* wszInitialFullPath,
                                             bool bMultiSelect, OUT ON_ClassArray<ON_wString>& aFullPaths);

