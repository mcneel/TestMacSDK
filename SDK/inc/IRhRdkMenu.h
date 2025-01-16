
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

/** \class IRhRdkMenu

    This interface allows access to menus created or managed by the RDK.
    It is used by the RDK UI plug-in to get menus from the core RDK.

*/
class RHRDK_SDK IRhRdkMenu
{
public:
	virtual ~IRhRdkMenu() { }

	/** Add a sub-menu to the menu.
		\param wszCaption is the sub-menu's caption.
		\return A reference to the new sub-menu. */
	virtual IRhRdkMenu& AddSubMenu(const wchar_t* wszCaption) = 0;

	/** Add a menu item to the menu.
		\param wszCaption is the menu item's caption.
		\param cmd is the menu item's command id 
		\param bEnabled is the menu item's enable state.
		\return \e true if successful, else \e false. */
	virtual bool AddItem(const wchar_t* wszCaption, unsigned short cmd, bool bEnabled) = 0;

	/** This method is obsolete. */
	/*RDK_DEPRECATED*/ virtual bool AddItem(const UUID& uuidAction) = 0;

	/** Add a separator to the menu. */
	virtual void AddSeparator(void) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};
