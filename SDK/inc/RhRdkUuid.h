
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"

class RHRDK_SDK CRhRdkUuid final
{
public:
	CRhRdkUuid();
	CRhRdkUuid(const CRhRdkUuid& uuid);
	CRhRdkUuid(const UUID& uuid);
	CRhRdkUuid(const char* sz);
	CRhRdkUuid(const wchar_t* wsz);
	~CRhRdkUuid();

public: // UUID operations.
	void Create(void);
	bool IsNil(void) const;
	operator UUID (void) const;
	CRhRdkUuid& operator = (const CRhRdkUuid& uuid);
	CRhRdkUuid& operator = (const UUID& uuid);
	bool operator == (const UUID& uuid) const;
	bool operator != (const UUID& uuid) const;

public: // String operations.
	static const int ciNumChars = 36;
	ON_wString String(int numChars=ciNumChars) const;
	ON_wString RegistryFormat(int numChars=ciNumChars) const;

	/** \return \e true if string is a valid UUID, else \e false. */
	static bool IsValidUuid(const wchar_t* wsz);

public:
	DWORD Hash(void) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
