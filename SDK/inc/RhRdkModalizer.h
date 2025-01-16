
#pragma once // Public SDK header

/** \class CRhRdkModalizer

	Embed one of these in any modal dialog that your renderer creates.
	This will ensure that RDK disables and enables any windows that could
	interfere with the dialog.
*/

#include "RhRdkDefinitions.h"

class RHRDK_SDK CRhRdkModalizer final
{
public:
	CRhRdkModalizer();
	~CRhRdkModalizer();

	/** \return \e true if a modalizer is active. */
	static bool IsActive(void);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
