
#pragma once // Public SDK header

#include "RhRdkColor.h"

class CRhRdkVariantData;

/** \class CRhRdkVariant

	The RDK variant is an object that can hold data of different types. It is primarily
	used by the automatic user interface system to provide a unified way of passing
	parameters to and from the user interface.

	A null variant has a type of vtNull and no value.

	Use the 'varying' feature to represent multiple different values of the
	same type. This is useful for property pages etc. If you use this feature,
	you \e must check Varies() before retrieving the value. If you never call
	SetVaries() then you never have to call Varies().

	When reading the value, conversion will be performed if necessary.

	\note It is not possible to retrieve the value if the variant
	is null or varying. An assert will occur if this is attempted.

	This class has no virtual functions and should not be subclassed.
*/
class RHRDK_SDK CRhRdkVariant final
{
public:
	/** Variant type. */
	enum VariantType
	{
		vtNull     = 0,
		vtBool     = 1,
		vtInteger  = 2,
		vtFloat    = 3,
		vtDouble   = 4,
		vtColor    = 5,
		vtVector2d = 6,
		vtVector3d = 7,
		vtString   = 8,
		vtPointer  = 9,
		vtUuid     = 10,
		vtMatrix   = 11,
		vtTime     = 12,
		vtBuffer   = 13,
		vtPoint4d  = 14,

		vtForce32bit = 0xFFFFFFFF
	};

public:
	~CRhRdkVariant();

	/** Construct as null. */
	CRhRdkVariant();

	/** Construct from an integer. */
	CRhRdkVariant(int i);

	/** Construct from a boolean. */
	CRhRdkVariant(bool b);

	/** Construct from a float. */
	CRhRdkVariant(float f);

	/** Construct from a double. */
	CRhRdkVariant(double d);

	/** Construct from a string. */
	CRhRdkVariant(const char* sz);

	/** Construct from a wide string. */
	CRhRdkVariant(const wchar_t* wsz);

	/** Construct from an ON_wString. */
	explicit CRhRdkVariant(const ON_wString& s);

	/** Construct from a color. */
	CRhRdkVariant(const ON_Color& c);

	/** Construct from an RDK color. */
	CRhRdkVariant(const CRhRdkColor& c);

	/** Construct from a float color. */
	CRhRdkVariant(const ON_4fColor& c);

	/** Construct from a 2 dimensional vector. */
	CRhRdkVariant(const ON_2dVector& v);

	/** Construct from a 3 dimensional vector. */
	CRhRdkVariant(const ON_3dVector& v);

	/** Construct from a 4 dimensional point. */
	CRhRdkVariant(const ON_4dPoint& p);

	/** Construct from a pointer. The variant treats the pointer as an opaque 32-bit value. */
	CRhRdkVariant(void* p);

	/** Construct from a UUID. */
	CRhRdkVariant(const UUID& uuid);

	/** Construct from a matrix. */
	CRhRdkVariant(const ON_Xform& xform);

	/** Construct from a time. */
	CRhRdkVariant(const time_t& time);

	/** Construct from a buffer */
	CRhRdkVariant(const ON_Buffer& buf);

	/** Construct from a raw buffer. */
	CRhRdkVariant(const void* pData, size_t size);

	/** Construct from an ON_XMLVariant. */
	explicit CRhRdkVariant(const ON_XMLVariant& v);

	/** Construct from another RDK variant. */
	CRhRdkVariant(const CRhRdkVariant& v);

	/** Retrieve the units associated with numeric values. */
	ON::LengthUnitSystem Units(void) const;

	/** Set the units associated with numeric values.
	\see AsModelFloat(). \see AsModelDouble(). \see SetAsModelFloat(). \see SetAsModelDouble(). */
	void SetUnits(ON::LengthUnitSystem units);

	/** Check if the variant is null. */
	bool IsNull(void) const;

	/** Set the variant to be null. Can be called at any time. */
	void SetNull(void);

	/**	Check if the value is varying. */
	bool Varies(void) const;

	/**	Set the value to varying. The type is retained. Calling this method on a null variant is not supported. */
	void SetVaries(void);

	/* Check the type of the variant. */
	VariantType Type(void) const;

	/* Set the value to an integer. */
	void SetValue(int v);

	/* Set the value to a boolean. */
	void SetValue(bool b);

	/* Set the value to a float. */
	void SetValue(float v);

	/* Set the value to a double. */
	void SetValue(double v);

	/* Set the value to a string. */
	void SetValue(const char* sz);

	/* Set the value to a wide string. */
	void SetValue(const wchar_t* wsz);

	/* Set the value to a color. */
	void SetValue(const ON_Color& c);

	/* Set the value to a float color. */
	void SetValue(const ON_4fColor& c);

	/** Set the value to an RDK color. */
	void SetValue(const CRhRdkColor& c);

	/* Set the value to a 2 dimensional vector. */
	void SetValue(const ON_2dVector& v);

	/* Set the value to a 3 dimensional vector. */
	void SetValue(const ON_3dVector& v);

	/* Set the value to a 4 dimensional point. */
	void SetValue(const ON_4dPoint& p);

	/** Set the value to a pointer. The variant treats the pointer as an opaque 64-bit value. */
	void SetValue(void* p);

	/** Set the value to a UUID. */
	void SetValue(const UUID& uuid);

	/** Set the value to a matrix. */
	void SetValue(const ON_Xform& xform);

	/** Set the value to a time. */
	void SetValue(const time_t& time);

	/** Set the value to a buffer. */
	void SetValue(const ON_Buffer& buf);

	/** Set the value to an ON_wString. */
	void SetValue(const ON_wString& s);

	/** Set the value to an ON_XMLVariant. */
	void SetValue(const ON_XMLVariant& v);

	/** Set the value to a raw buffer. */
	void SetValue(const void* pBuffer, size_t size);

	/** Assign from an integer. */
	const CRhRdkVariant& operator = (int i);

	/** Assign from a boolean. */
	const CRhRdkVariant& operator = (bool b);

	/** Assign from a float. */
	const CRhRdkVariant& operator = (float f);

	/** Assign from a double. */
	const CRhRdkVariant& operator = (double d);

	/** Assign from a string. */
	const CRhRdkVariant& operator = (const char* sz);

	/** Assign from a wide string. */
	const CRhRdkVariant& operator = (const wchar_t* wsz);

	/** Assign from an ON_wString. */
	const CRhRdkVariant& operator = (const ON_wString& s);

	/** Assign from a color. */
	const CRhRdkVariant& operator = (const ON_Color& c);

	/** Assign from a float color. */
	const CRhRdkVariant& operator = (const ON_4fColor& c);

	/** Assign from a 2 dimensional vector. */
	const CRhRdkVariant& operator = (const ON_2dVector& v);

	/** Assign from a 3 dimensional vector. */
	const CRhRdkVariant& operator = (const ON_3dVector& v);

	/** Assign from a 4 dimensional point. */
	const CRhRdkVariant& operator = (const ON_4dPoint& p);

	/** Assign from a pointer. The variant treats the pointer as an opaque 32-bit value. */
	const CRhRdkVariant& operator = (void* p);

	/** Assign from a UUID. */
	const CRhRdkVariant& operator = (const UUID& uuid);

	/** Assign from a matrix. */
	const CRhRdkVariant& operator = (const ON_Xform& xform);

	/** Assign from a time. */
	const CRhRdkVariant& operator = (const time_t& time);

	/** Assign from a buffer. */
	const CRhRdkVariant& operator = (const ON_Buffer& buf);

	/** Assign from an ON_XMLVariant. */
	const CRhRdkVariant& operator = (const ON_XMLVariant& v);

	/** Assign from another variant. Null and varying can be copied. */
	const CRhRdkVariant& operator = (const CRhRdkVariant& v);

	/** Equality check. */
	bool operator == (const CRhRdkVariant& v) const;

	/** Inequality check. */
	bool operator != (const CRhRdkVariant& v) const;

	/** Greater-than check. */
	bool operator > (const CRhRdkVariant& v) const;

	/** Less-than check. */
	bool operator < (const CRhRdkVariant& v) const;

	/**	Retrieve the value as an integer. Null or varying returns 0. */
	int AsInteger(void) const;
	operator int(void) const;

	/**	Retrieve the value as a boolean. Null or varying returns false. */
	bool AsBool(void) const;
	operator bool(void) const;

	/**	Retrieve the value as a float. Null or varying returns 0.0f. */
	float AsFloat(void) const;
	operator float(void) const;

	/**	Retrieve the value as a double. Null or varying returns 0.0. */
	double AsDouble(void) const;
	operator double(void) const;

	/**	Retrieve the value as an ON_Color. Null or varying returns black. */
	ON_Color AsOnColor(void) const;
	operator ON_Color(void) const;

	/**	Retrieve the value as an ON_4fColor. Null or varying returns black. */
	RDK_DEPRECATED ON_Color AsOn4fColor(void) const; // Deprecated due to typo. [SDK_UNFREEZE]
	ON_4fColor AsOn4fColorEx(void) const;
	operator ON_4fColor(void) const;

	/**	Retrieve the value as a CRhRdkColor. Null or varying returns black. */
	CRhRdkColor AsRdkColor(void) const;
	operator CRhRdkColor(void) const;

	/**	Retrieve the value as a 2 dimensional vector. Null or varying returns (0, 0). */
	ON_2dVector AsVector2d(void) const;
	operator ON_2dVector(void) const;

	/**	Retrieve the value as a 3 dimensional vector. Null or varying returns (0, 0, 0). */
	ON_3dVector AsVector3d(void) const;
	operator ON_3dVector(void) const;

	/**	Retrieve the value as a 4 dimensional point. Null or varying returns (0, 0, 0, 0). */
	ON_4dPoint AsPoint4d(void) const;
	operator ON_4dPoint(void) const;

	/**	Retrieve the value as a wide string. Null returns "NULL", varying returns "VARIES". */
	const wchar_t* AsString(void) const;
	operator const wchar_t* (void) const;

	/**	Retrieve the value as a pointer. Null or varying returns NULL. */
	void* AsPointer(void) const;
	operator void* (void) const;

	/**	Retrieve the value as a UUID. Null or varying returns ON_nil_uuid. */
	UUID AsUuid(void) const;
	operator UUID (void) const;

	/**	Retrieve the value as a matrix. Null or varying returns zero matrix. */
	ON_Xform AsMatrix(void) const;
	operator ON_Xform (void) const;

	/**	Retrieve the value as a time. Null or varying returns zero time. */
	time_t AsTime(void) const;
	operator time_t (void) const;

	/**	Retrieve the value as an ON_XMLVariant. */
	ON_XMLVariant AsOnXMLVariant(void) const;
	operator ON_XMLVariant (void) const;

	/**	Retrieve the value as a buffer. Null or varying returns an empty buffer. */
	ON_Buffer AsBuffer(void) const;
	operator ON_Buffer (void) const;

	/**	Retrieve the value as a raw buffer. Null or varying returns null pointer and zero length. */
	const void* AsBuffer(size_t& sizeOut) const;

	/**	It is often necessary to get a variant as a boolean, especially in dialogs with lots
		of check boxes. This helper retrieves the value as a boolean ensuring that null and
		varying are handled correctly. Null returns bNullResult, varying returns bVariesResult. */
	bool AsSmartBool(bool bNullResult=false, bool bVariesResult=true) const;

	/**	It is often necessary to get a variant as a double, especially in dialogs with lots
		of numeric edit boxes. This helper retrieves the value as a double ensuring that null and
		varying are handled correctly. Null returns dNullResult, varying returns dVariesResult. */
	double AsSmartDouble(double dNullResult=0.0, double dVariesResult=0.0) const;

	/**	It is often necessary to get a variant as a string, especially in dialogs with lots
		of combo boxes. This helper retrieves the value as a string ensuring that null and
		varying are handled correctly. Null returns wszNullResult, varying returns wszVariesResult. */
	const wchar_t* AsSmartString(const wchar_t* wszNullResult=L"", const wchar_t* wszVariesResult=L"") const;

	/**	Retrieve the value as a float in model units. Null or varying returns 0.0.
		The value will be converted from the variant's units to model units if necessary.
		\see Units(). \see SetUnits(). */
	float AsModelFloat(const CRhinoDoc* pDo) const;

	/** Set the value to a float in model units.
		The value will be converted from model units to the variant's units if necessary. 
		\see Units(). \see SetUnits(). */
	void SetAsModelFloat(float f, const CRhinoDoc* pDoc);

	/**	Retrieve the value as a double in model units. Null or varying returns 0.0.
		The value will be converted from the variant's units to model units if necessary.
		\see Units(). \see SetUnits(). */
	double AsModelDouble(const CRhinoDoc* pDoc) const;

	/** Set the value to a double in model units.
		The value will be converted from model units to the variant's units if necessary. 
		\see Units(). \see SetUnits(). */
	void SetAsModelDouble(double d, const CRhinoDoc* pDoc);

	/** Format string as type and value. Useful for debugging. */
	void Format(ON_wString& sOut) const;

	/** Calculates a CRC for the variant's state. If the variant is null or 'varies' this will be zero.
		\return CRC value which is never zero for a valid variant. */
	DWORD CalculateCRC(void) const;

	/** Emergency function for future expansion. */
	void* EF(const wchar_t* wszFunc, void* pvData);

	/** General purpose null variant for use in return values where a null variant means "nothing". */
	static const CRhRdkVariant& Null(void);

	/** Change the type of the variant while retaining the data as close as possible. */
	void ChangeType(VariantType);

	/** Disallows calls which usually result from a typo; force use of object or reference. */
	explicit CRhRdkVariant(const ON_XMLVariant* v) = delete;

private:
	/** \internal For RDK internal use only. */
	void Construct(void);

	/** \internal For RDK internal use only. */
	void DeleteData(void);

	/** \internal For RDK internal use only. */
	void SetType(VariantType) const;

	/** \internal For RDK internal use only. */
	CRhRdkVariantData& Data(void) const;

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};
