
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

// STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkIORContextMenu final
{
public:
	CRhRdkIORContextMenu();
	~CRhRdkIORContextMenu();

	bool TrackMenu(HWND pWnd, const ON_2iPoint& point, double& dIOROut, ON_wString* pStringOut=nullptr) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};
