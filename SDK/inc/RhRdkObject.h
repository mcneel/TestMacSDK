
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

/** \class CRhRdkObject

	Base class for objects which are created by RDK but deleted by the client (or vice-versa).
	Overrides of new and delete allow you to delete such objects from your client DLL without any complications.

*/
class RHRDK_SDK CRhRdkObject
{
public:
	CRhRdkObject();
	virtual ~CRhRdkObject();

	//[SDK_UNFREEZE]
	//CRhRdkObject(const CRhRdkObject&);
	//const CRhRdkObject& operator=(const CRhRdkObject&);

	void* operator new(size_t size);
	void* operator new[] (size_t size);
	void  operator delete (void* buf);
	void  operator delete[] (void* buf);

	static void PopulateDebuggingInfo(void);
	static void CheckMemory(void);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

private:
	class CImpl;
	CImpl* m_pImpl;
};
