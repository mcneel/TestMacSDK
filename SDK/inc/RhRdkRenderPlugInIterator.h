
#pragma once // Public SDK header

#include "RhRdkObject.h"

class CRhRdkRenderPlugIn;

/** \class CRhRdkRenderPlugInIterator

	This class provides support for enumerating installed Rhino render plug-ins.
*/
class RHRDK_SDK CRhRdkRenderPlugInIterator : public CRhRdkObject
{
public:
	CRhRdkRenderPlugInIterator();
	virtual ~CRhRdkRenderPlugInIterator();

	class RHRDK_SDK Info final
	{
	public:
		Info();

		UUID m_uuidPlugIn;
		ON_wString m_sPlugInName;
		CRhRdkRenderPlugIn* m_pRdkRenderPlugIn;
		CRhinoRenderPlugIn* m_pRhinoRenderPlugIn;
		void* m_pReserved;
	};

	virtual bool Next(Info& infoOut);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
