
#pragma once // Public SDK header

#include "RhRdkDefinitions.h"
#include "RhRdkContent.h"
#include "IAutoUIExtraRequirements.h"

class IRhRdk_XMLSection;

/** \class CRhRdkContentField

	RDK content fields are essentially smart variants which have an internal name and local name
	and English name along with the usual variant data. By using fields to store your content's
	data items instead of using plain bools, floats and so on, you get automatic parameter undo
	support, support for transferring to and from IRhRdk_XMLSection and a few other features that
	should reduce the amount of work involved in implementing your content.

	If you want to use fields to store your content's data items, you must make sure it overrides
	BitFlags() and sets the bfFields flag.

	\see CRhRdkContent.
	\see CRhRdkContentFields.
	\see CRhRdkContent::BitFlags().
	\see CRhRdkContent::SetParameter().
	\see CRhRdkContent::GetParameter().
*/
class RHRDK_SDK CRhRdkContentField
{
public:
	enum Filter : unsigned int
	{
		None          = 0x00, // Field is never automatically accessed.
		Serialization = 0x01, // Field is only automatically accessed during serialization.
		Harvesting    = 0x02, // Field is only automatically accessed during harvesting.
		Rendering     = 0x04, // Field is only automatically accessed during rendering.
		All           = 0xFF, // Field is always automatically accessed.
	};

public:
	/** This constructor is deprecated in favor of the one below. */
	typedef Filter eFilter;
	/* DEPRECATED */ CRhRdkContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszFriendlyName, eFilter setFilter=All, eFilter getFilter=All, INT_PTR reserved=0);

	/** Fields are always constructed the same way. After construction they are null and must be
		initialized to a type/value before use by using an assignment operator or the Initialize() method.
		\param c is the content that the field belongs to.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is used for filtering fields between sections. Zero if not needed.
		\param bDynamic indicates that this field is dynamic. \see IsDynamic()
		\param reserved is reserved for future use. Must be zero. */
	CRhRdkContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                   Filter setFilter=All, Filter getFilter=All, int sectionId=0, bool bDynamic=false, INT_PTR reserved=0);

	virtual ~CRhRdkContentField();

	/** Return an exact copy of this field, with the same value and type. */
	virtual CRhRdkContentField* MakeCopy(CRhRdkContent&) const;

	/** Initialize the field's variant directly from a variant. */
	void Initialize(const CRhRdkVariant& v);

	/** Copy the field value. The type must be null or the same as the type of 'f'. */
	const CRhRdkContentField& operator = (const CRhRdkContentField& f);

	/** Assign from an integer. */
	const CRhRdkContentField& operator = (int i);

	/** Assign from a boolean. */
	const CRhRdkContentField& operator = (bool b);

	/** Assign from a float. */
	const CRhRdkContentField& operator = (float f);

	/** Assign from a double. */
	const CRhRdkContentField& operator = (double d);

	/** Assign from a string. */
	const CRhRdkContentField& operator = (const char* sz);

	/** Assign from a wide string. */
	const CRhRdkContentField& operator = (const wchar_t* wsz);

	/** Assign from a color. */
	const CRhRdkContentField& operator = (const ON_Color& c);

	/** Assign from a 2 dimensional vector. */
	const CRhRdkContentField& operator = (const ON_2dVector& v);

	/** Assign from a 3 dimensional vector. */
	const CRhRdkContentField& operator = (const ON_3dVector& v);

	/** Assign from a 4 dimensional point. */
	const CRhRdkContentField& operator = (const ON_4dPoint& p);

	/** Assign from a pointer. The field treats the pointer as an opaque 32-bit value. */
	const CRhRdkContentField& operator = (void* p);

	/** Assign from a UUID. */
	const CRhRdkContentField& operator = (const UUID& uuid);

	/** Assign from a matrix. */
	const CRhRdkContentField& operator = (const ON_Xform& xform);

	/** Assign from a time. */
	const CRhRdkContentField& operator = (const time_t& time);

	/** Assign from another variant. */
	const CRhRdkContentField& operator = (const CRhRdkVariant& v);

	/** Equality check. */
	bool operator == (const CRhRdkContentField& v) const;

	/** Inequality check. */
	bool operator != (const CRhRdkContentField& v) const;

	/** Greater-than check. */
	bool operator > (const CRhRdkContentField& v) const;

	/** Less-than check. */
	bool operator < (const CRhRdkContentField& v) const;

	/**	Retrieve the value as an integer. */
	operator int(void) const;

	/**	Retrieve the value as a boolean. */
	operator bool(void) const;

	/**	Retrieve the value as a float. */
	operator float(void) const;

	/**	Retrieve the value as a double. */
	operator double(void) const;

	/**	Retrieve the value as an ON_Color. */
	operator ON_Color(void) const;

	/**	Retrieve the value as a CRhRdkColor. */
	operator CRhRdkColor(void) const;

	/**	Retrieve the value as a 2 dimensional vector. */
	operator ON_2dVector(void) const;

	/**	Retrieve the value as a 3 dimensional vector. */
	operator ON_3dVector(void) const;

	/**	Retrieve the value as a 4 dimensional point.. */
	operator ON_4dPoint(void) const;

	/**	Retrieve the value as a wide string. */
	operator const wchar_t* (void) const;

	/**	Retrieve the value as a UUID. */
	operator UUID (void) const;

	/**	Retrieve the value as a matrix. */
	operator ON_Xform (void) const;

	/**	Retrieve the value as a time. Null or varying returns zero time. */
	operator time_t (void) const;

	/** Get the value of the field. */
	operator CRhRdkVariant (void) const;

	/**	Retrieve the value as a pointer. */
	void* AsPointer(void) const;

	/**	Retrieve the value as a buffer. */
	const void* AsBuffer(size_t& sizeOut) const;

public:
	/** \return The internal name of the field. */
	virtual ON_wString InternalName(void) const;

	/** \return The localized friendly name of the field. Deprecated in favor of LocalName(). */
	RDK_DEPRECATED virtual ON_wString FriendlyName(void) const;

	/** \return The English friendly name of the field. */
	virtual ON_wString EnglishName(void) const;

	/** \return The localized friendly name of the field. */
	virtual ON_wString LocalName(void) const;

	/** \return The string used when recording undo for the field. */
	virtual ON_wString UndoString(void) const;

	/** Get the limits that the field value should be within.
		\param vMin accepts the minimum value.
		\param vMax accepts the maximum value.
		\note If SetLimits() has not been called, both results will be null.
		\see SetLimits().
		\return \e true if successful, \e else false. */
	virtual bool GetLimits(CRhRdkVariant& vMin, CRhRdkVariant& vMax) const;

	/** Set the limits that the field value should be within.
		\param vMin is the minimum value.
		\param vMax is the maximum value.
		\return \e true if successful, \e else false. */
	virtual bool SetLimits(const CRhRdkVariant& vMin, const CRhRdkVariant& vMax);

	enum : unsigned int // Bit-flags for value conversion.
	{
		conv_none       = 0x0000, // The value should be displayed as it is.
		conv_percentile = 0x0001, // The value should be converted to (value * 100) for display.
		conv_one_minus  = 0x0002, // The value should be converted to (1 - value) for display.
		conv_radians    = 0x0004, // The value should be converted from radians to degrees for display.
	};

	/** Get the conversion that describes how the field should be displayed. */
	virtual UINT Conversion(void) const;

	/** Set the conversion that describes how the field should be displayed. */
	virtual void SetConversion(UINT uFlags);

	/** \return The value of the field. */
	virtual const CRhRdkVariant& Value(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool SetValue(const CRhRdkVariant& vNewValue, CRhRdkContent::eChangeContext cc);

	/** Set the value of the field. If the value changes, this may create an undo record and it will
		call CRhRdkContent::Changed() and return \e true. Otherwise it will return \e false.
		\param vNewValue is the value to set into the field.
		\return \e true if value was changed, \e false if not changed because it wasn't different. */
	virtual bool SetValue(const CRhRdkVariant& vNewValue);

	/** Get a value from a section into the field if the value, as specified by the field's internal name,
		is in the section. Does not create an undo record or call CRhRdkContent::Changed(). */
	virtual void GetValueFromSection(const IRhRdk_XMLSection& section);

	/** Set a value into a section using the field's internal name. */
	virtual void SetValueToSection(IRhRdk_XMLSection& section) const;

	/** Get the automatic UI section id for this field. */
	virtual int SectionId(void) const;
 
	/** The 'set filter' is a filter for use during CRhRdkContentFields::SetValuesToSection().
		The field is skipped if this filter does not match at least one item in the call's filter. */
	virtual Filter SetFilter(void) const;

	/** The 'get filter' is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		The field is skipped if this filter does not match at least one item in the call's filter. */
	virtual Filter GetFilter(void) const;

	/** Fields are traditionally static and embedded in the content as member variables. Static fields
		are created when the content is created and deleted when the content is deleted; they are
		owned by the content. Static fields are expected to exist when the content is created
		and they can load themselves from XML. Dynamic fields on the other hand do not exist until
		they are loaded, either from XML or by some other means (e.g., a content I/O plug-in), or
		until they are added by an override of Initialize().
		Dynamic fields are owned by the CRhRdkContentFields collection.
		\return \e true if the field is dynamic. */
	bool IsDynamic(void) const;

	/** When fields are used by the automatic UI, they can be hidden from it by calling SetIsHidden().
		This method returns the hidden state for when the field is used in the automatic UI. */
	bool IsHiddenInAutoUI(void) const;

	/** When fields are used by the automatic UI, they can be hidden from it by calling this method. */
	void SetIsHiddenInAutoUI(bool bHidden);

	/** \return The content that this field belongs to. */
	virtual CRhRdkContent& Content(void) const;

	/** Converts from a field value to a display value using the conversion. */
	virtual CRhRdkVariant ConvertToDisplay(const CRhRdkVariant& v) const;

	/** Converts to a field value from a display value using the conversion. */
	virtual CRhRdkVariant ConvertFromDisplay(const CRhRdkVariant& v) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*);

	bool AutomaticRegisteredProperty(void) const;
	void SetAutomaticRegisteredProperty(bool);

public:
	class CChangeInfo // Deprecated
	{
	public:
		CRhRdkVariant m_vOldValue;
		CRhRdkContentField* m_pField;
		RhRdkChangeContext m_ChangeContext;
		void* m_pReserved;
	};

private:
	class CImpl;
	CImpl* m_pImpl;
	friend class CRhRdkContentFields;

	CRhRdkContentField(const CRhRdkContentField& f); // Not implemented; should not be used.

protected:
	CRhRdkContentField* NewField(CRhRdkContent& c) const;
	CImpl& Impl(void) const;
	friend class CRhRdkDynamicContentFieldManager;
};

/** \class CRhRdkContentFields

	A collection of fields. This object is created by the content when its Fields() method is called.

*/
class RHRDK_SDK CRhRdkContentFields
{
public:
	CRhRdkContentFields(const CRhRdkContent& c);
	virtual ~CRhRdkContentFields();

	CRhRdkContentFields& operator = (const CRhRdkContentFields& f);

	/** \return The number of fields in the collection. */
	virtual int Count(void) const;

	/** Get a field in the collection.
		\param iIndex is the index of the field. This must be 0 <= iIndex < Count().
		\return The field at the required position or nullptr if the index is out of range. */
	virtual CRhRdkContentField* Field(int iIndex) const;

	/** \return A field in the collection found by its internal name.
		\param wszInternalName is the name of the field to find.
		\return The field with the specified name, or nullptr if the field does not exist. */
	virtual CRhRdkContentField* FindField(const wchar_t* wszInternalName) const;

	/** \return The content that this field collection belongs to. */
	virtual CRhRdkContent& Content(void) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool GetValuesFromSection(const IRhRdk_XMLSection& section, CRhRdkContentField::Filter filter, CRhRdkContent::eChangeContext cc);

	/** Get values from a section into fields with the same internal name.
		\param section is the section to read values from.
		\param filter is a filter to control which fields are modified.
		\return \e true if at least one field was actually changed, else \e false. */
	virtual bool GetValuesFromSection(const IRhRdk_XMLSection& section, CRhRdkContentField::Filter filter);

	/** Set values into a section from fields with the same internal name.
		\param section is the section to write values to.
		\param filter is a filter to control which fields are transferred to the section.
		\return \e true. The return value is for future use only. */
	virtual bool SetValuesToSection(IRhRdk_XMLSection& section, CRhRdkContentField::Filter filter) const;

	/** Calculate the CRC of all fields that match a filter.
		\param filter is a filter to control which fields are considered.
		\return The CRC. */
	/**virtual*/ ON__UINT32 GetFieldValueCRC(const CRhRdkContent::CRenderCRCFlags& rcrcFlags, CRhRdkContentField::Filter filter) const; // [SDK_UNFREEZE]

	/** Add values to a param block from fields with the same internal name. */
	virtual bool AddValuesToParamBlock(IRhRdkParamBlock& paramBlock, int sectionId, const wchar_t* wszExtraRequirementSet=nullptr) const;

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED virtual bool GetValuesFromParamBlock(const IRhRdkParamBlock& paramBlock, int sectionId, CRhRdkContent::eChangeContext cc);

	/** Get values from a param block into fields with the same internal name. */
	virtual bool GetValuesFromParamBlock(const IRhRdkParamBlock& paramBlock, int sectionId);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*);

public:	/**	Automatic Dynamic Field support.

		Dynamic fields are typically created in the override of CRhRdkContent::Initialize() and they are therefore created
		automatically whenever the content is created. However, some advanced users require the fields to be created in response
		to some user action which occurs much later than Initialize(). This creates the problem that the fields do not exist
		by default and therefore cannot be loaded when the document is loaded. These methods are provided to solve that
		problem by making it possible to automatically create the dynamic fields on loading if they don't already exist.
		Dynamic fields that have this auto-create-on-load behavior are referred to as automatic dynamic fields.

		\note Dynamic fields that do not require the advanced automatic feature can still be created by using
		 these methods (recommended), or they can be created manually (legacy usage). */

	/** You must call this before creating any dynamic fields. Calls to this method are counted; you must call
		EndCreateDynamicFields() once for every call to BeginCreateDynamicFields().
		\param bAutomatic specifies if the dynamic fields are automatic. If so, they will be created automatically
		 during loading of the document. */
	void BeginCreateDynamicFields(bool bAutomatic);

	/** Create a dynamic field with an initial value and min and max limits.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param vValue is the initial value of the field.
		\param vMinValue is the minimum value of the field. Must be the same type as vValue.
		\param vMaxValue is the maximum value of the field. Must be the same type as vValue.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is used for filtering fields between sections. Zero if not needed.
		\param reserved is reserved for future use. Must be zero. */
	bool CreateDynamicField(const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                        const CRhRdkVariant& vValue, const CRhRdkVariant& vMinValue=CRhRdkVariant(), const CRhRdkVariant& vMaxValue=CRhRdkVariant(),
	                        CRhRdkContentField::Filter setFilter=CRhRdkContentField::Filter::All,
	                        CRhRdkContentField::Filter getFilter=CRhRdkContentField::Filter::All,
	                        int sectionId=0, INT_PTR reserved=0);

	/** You must call this after creating dynamic fields. Calls to this method are counted; you must call
		BeginCreateDynamicFields() once for every call to EndCreateDynamicFields().*/
	void EndCreateDynamicFields(void);

protected:
	/** Add a field. Called automatically by the field constructor.
		\internal For RDK internal use only. */
	virtual void Add(CRhRdkContentField* pField);

	/** Remove a field. Called automatically by the field destructor.
		\internal For RDK internal use only. */
	virtual void Remove(CRhRdkContentField* pField);

	/** \internal For RDK internal use only. */
	void Initialize(void);

	/** \internal For RDK internal use only. */
	virtual void Uninitialize(void);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
	friend class CRhRdkContent;
	friend class CRhRdkContentField;
};

/** \class CRhRdkDynamicContentField

	This class makes it easier for clients to create dynamic fields in their Initialize() override
	or in a custom Content I/O plug-in. It is similar to a static field but can be added to the
	content at run-time. It is not strictly necessary to use this class to get a dynamic field,
	but it just makes it easier.

*/
class RHRDK_SDK CRhRdkDynamicContentField : public CRhRdkContentField
{
public:
	/** Constructs a dynamic field with an initial value and no limits.
		\param c is the content that the field belongs to.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param vValue is the initial value of the field.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is used for filtering fields between sections. Zero if not needed.
		\param reserved is reserved for future use. Must be zero. */
	CRhRdkDynamicContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                   const CRhRdkVariant& vValue, Filter setFilter=All, Filter getFilter=All, int sectionId=0, INT_PTR reserved=0);

	/** Constructs a dynamic field with an initial value and min and max limits.
		\param c is the content that the field belongs to.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param vValue is the initial value of the field.
		\param vMinValue is the minimum value of the field. Must be the same type as vValue.
		\param vMaxValue is the maximum value of the field. Must be the same type as vValue.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is used for filtering fields between sections. Zero if not needed.
		\param reserved is reserved for future use. Must be zero. */
	CRhRdkDynamicContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                   const CRhRdkVariant& vValue, const CRhRdkVariant& vMinValue, const CRhRdkVariant& vMaxValue,
	                   Filter setFilter=All, Filter getFilter=All, int sectionId=0, INT_PTR reserved=0);

	/** Return an exact copy of this field, with the same value and type. */
	virtual CRhRdkContentField* MakeCopy(CRhRdkContent&) const override;

	virtual void* EVF(const wchar_t*, void*) override;
};

/** \class CRhRdkTexturedContentField adds 'texturing' functionality to a field.

	This field is usually static but can be dynamic if necessary. It is useful
	when your content uses the Automatic UI because it enables the UI to give
	the field extra 'texturing' controls in the automatic UI. These controls allow
	assignment of a child content (usually a texture) with an on/off check box and
	an amount. The amount can be used when rendering to blend between the base color
	and the texture color at a certain point. The child's child-slot-name is derived
	from the field name (see CRhRdkContent::ChildSlotNameFromParamName()).

*/
class RHRDK_SDK CRhRdkTexturedContentField : public CRhRdkContentField
{
public:
	/** Textured fields are always constructed the same way. After construction they are null and
		must be initialized to a type/value before use by calling the Initialize() method.
		\param c is the content that the field belongs to.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is the id of an automatic section. This is optional and should be zero if not required.
		 It allows field UI controls to automatically be distributed among automatic UI sections.
		\param bDynamic indicates that this field is dynamic. \see IsDynamic()
		\param vTextureOn is the initial 'on' state. Use vtNull if not required.
		\param vTextureAmount is the initial amount. Use vtNull if not required.
		\param vTextureAmountMin is the amount lower limit. Use vtNull if not required.
		\param vTextureAmountMax is the amount upper limit. Use vtNull if not required.
		\param reserved is reserved for future use. Must be zero. */
	CRhRdkTexturedContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                           Filter setFilter=All, Filter getFilter=All, int sectionId=0, bool bDynamic=false,
	                           const CRhRdkVariant& vTextureOn=false,
	                           const CRhRdkVariant& vTextureAmount=100,
	                           const CRhRdkVariant& vTextureAmountMin=0,
	                           const CRhRdkVariant& vTextureAmountMax=100,
	                           INT_PTR reserved=0);

	CRhRdkTexturedContentField(const CRhRdkTexturedContentField& f); // Not implemented; should not be used.

	virtual ~CRhRdkTexturedContentField();

	virtual CRhRdkContentField* MakeCopy(CRhRdkContent&) const override;

	/** \return \e true if 'texture on' is in use, else \e false. */
	bool UseTextureOn(void) const;

	/** Turns the 'texture on' is in use on or off */
	void SetUseTextureOn(bool bOn);

	/** \return \e true if the texture is turned on, else \e false. */
	bool TextureOn(void) const;

	/** Turns the texture on or off. */
	void SetTextureOn(bool bOn);

	/** \return \e true if 'texture amount' is in use, else \e false. */
	bool UseTextureAmount(void) const;

	/** Turns 'texture amount is in use' on or off */
	void SetUseTextureAmount(bool bOn);

	/** \return The texture amount. The typical range is 0 to 100.
		This is deprecated in favor of TextureDoubleAmount() which provides more accuracy. */
	RDK_DEPRECATED int TextureAmount(void) const;

	/** Set the texture amount.
		This is deprecated in favor of SetTextureDoubleAmount() which provides more accuracy. */
	RDK_DEPRECATED void SetTextureAmount(int);

	/** \return The texture amount as a double. The typical range is 0.0 to 100.0 but the value
	            is allowed to have any range, positive or negative. */
	double TextureDoubleAmount(void) const;

	/** Set the texture amount. The typical range is 0.0 to 100.0 but the value is allowed to
		have any range, positive or negative. */
	void SetTextureDoubleAmount(double amount);

	/** \return \e true if 'texture amount min' is in use, else \e false. */
	bool UseTextureAmountMin(void) const;

	/** \return The minimum texture amount. The default is 0. */
	RDK_DEPRECATED int TextureAmountMin(void) const;

	/** Set the minimum texture amount. */
	RDK_DEPRECATED void SetTextureAmountMin(int amountMin);

	/** \return The minimum texture amount. The default is 0. */
	double TextureDoubleAmountMin(void) const;

	/** Set the minimum texture amount. The typical range is 0.0 to 100.0 but the value is allowed to
		have any range, positive or negative. */
	void SetTextureDoubleAmountMin(double amountMin);

	/** \return \e true if 'texture amount max' is in use, else \e false. */
	bool UseTextureAmountMax(void) const;

	/** \return The maximum texture amount. The default is 100. */
	RDK_DEPRECATED int TextureAmountMax(void) const;

	/** Set the maximum texture amount. */
	RDK_DEPRECATED void SetTextureAmountMax(int amountMax);

	/** \return The maximum texture amount. The default is 100. */
	double TextureDoubleAmountMax(void) const;

	/** Set the maximum texture amount. The typical range is 0.0 to 100.0 but the value is allowed to
		 ave any range, positive or negative. */
	void SetTextureDoubleAmountMax(double amountMax);

	virtual void* EVF(const wchar_t*, void*) override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkFilenameContentField is a 'filename' field.

	When contents need to store a filename, they should use this field subclass.
	It specializes certain operations for filenames.

*/
class RHRDK_SDK CRhRdkFilenameContentField : public CRhRdkContentField
{
public:
	/** Filename fields are always constructed the same way. After construction they
		are an empty string.
		\param c is the content that the field belongs to.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is the id of an automatic section. This is optional and should be zero if not required.
		 It allows field UI controls to automatically be distributed among automatic UI sections.
		\param bDynamic indicates that this field is dynamic. \see IsDynamic()
		\param reserved is reserved for future use. Must be zero. */
	CRhRdkFilenameContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                        Filter setFilter=All, Filter getFilter=All, int sectionId=0, bool bDynamic=false, INT_PTR reserved=0);

	CRhRdkFilenameContentField(const CRhRdkFilenameContentField& f); // Not implemented; should not be used.

	virtual ~CRhRdkFilenameContentField();

	/** Assign from a string. */
	const CRhRdkFilenameContentField& operator = (const char* sz);

	/** Assign from a wide string. */
	const CRhRdkFilenameContentField& operator = (const wchar_t* wsz);

	virtual void* EVF(const wchar_t*, void*) override;

public:
	virtual void SetValueToSection(IRhRdk_XMLSection& section) const override;
	virtual const CRhRdkVariant& Value(void) const override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \class CRhRdkSolarContentField is a 'solar' field.

	This is a 3D vector field which appears in the automatic UI as two edit boxes
	and a 'sun' button. The 3D vector is the vector from the sun to the observer.
	The edit boxes give the sun's horizon coordinates (azimuth and altitude) which
	are equivalent to the 3D vector. The sun button displays a dialog which allows
	the user to change these coordinates using a sun user interface. When OK is
	clicked, the coordinates will be updated from either manual settings, or the date,
	time and Earth location of a temporary 'sun'.

*/
class RHRDK_SDK CRhRdkSolarContentField : public CRhRdkContentField
{
public:
	/** Solar fields are always constructed the same way. After construction they
		are a zero 3D vector.
		\param c is the content that the field belongs to.
		\param wszInternalName is the internal name of the field. Not localized.
		\param wszLocalName is the localized user-friendly name of the field.
		\param wszEnglishName is the English user-friendly name of the field.
		\param setFilter is a filter for use during CRhRdkContentFields::SetValuesToSection().
		 The field is skipped if setFilter does not match at least one item in the call's filter.
		\param getFilter is a filter for use during CRhRdkContentFields::GetValuesFromSection().
		 The field is skipped if getFilter does not match at least one item in the call's filter.
		\param sectionId is the id of an automatic section. This is optional and should be zero if not required.
		 It allows field UI controls to automatically be distributed among automatic UI sections.
		\param bDynamic indicates that this field is dynamic. \see IsDynamic()
		\param reserved is reserved for future use. Must be zero. */
	CRhRdkSolarContentField(CRhRdkContent& c, const wchar_t* wszInternalName, const wchar_t* wszLocalName, const wchar_t* wszEnglishName,
	                        Filter setFilter=All, Filter getFilter=All, int sectionId=0, bool bDynamic=false, INT_PTR reserved=0);

	CRhRdkSolarContentField(const CRhRdkSolarContentField& f); // Not implemented; should not be used.

	virtual void* EVF(const wchar_t*, void*) override;

private:
	class CImpl;
	CImpl* m_pImpl;
};

/** \fn RhRdkGetTexturingExtraRequirement

	If your content uses CRhRdkTexturedContentField for its textured fields, you can call this function
	from your implementation of IAutoUIExtraRequirements::GetExtraRequirementParameter() to automatically
	handle requests for setting RDK_TEXTURING extra requirements. The function iterates over the contents
	in 'aSelection' in the specified document 'rdkDoc'. For each content, it calls GetExtraRequirementParameter().
	The default implementation of this function looks at the field named in 'wszParamName' and handles the
	'wszExtraReqName' request as follows:

	  - RDK_TEXTURING            - result is always true.
	  - RDK_TEXTURE_ON           - result is the TextureOn() state of the relevant field(s).
	  - RDK_TEXTURE_AMOUNT       - result is the TextureDoubleAmount() of the relevant field(s).
	  - RDK_TEXTURE_ON_SHOWN     - result is UseTextureOn() of the relevant field(s).
	  - RDK_TEXTURE_AMOUNT_SHOWN - result is UseTextureAmount() of the relevant field(s).
	  - RDK_TEXTURE_LOWER_BOUND  - result is TextureDoubleAmountMin() of the relevant field(s).
	  - RDK_TEXTURE_UPPER_BOUND  - result is TextureDoubleAmountMax() of the relevant field(s).
	  - RDK_TEXTURE_PARENT       - result is the instance id of the first content in the selection.

	If 'wszExtraReqName' is anything other than one of the above values, or the request cannot be satisfied
	for some other reason, the function returns \e false.

	If all the fields across all the contents have the same value for the above 'result', 'vValueOut' is
	set to that value. If the result differs across the contents, vValueOut is set to 'varies'.

*/
RHRDK_SDK bool RhRdkGetTexturingExtraRequirement(const CRhRdkContentArray& aSelection,
                                                 const wchar_t* wszParamName, const wchar_t* wszExtraReqName, CRhRdkVariant& vValueOut);

/** \fn RhRdkSetTexturingExtraRequirement

	If your content uses CRhRdkTexturedContentField for its textured fields, you can call this function
	from your implementation of IAutoUIExtraRequirements::SetExtraRequirementParameter() to automatically
	handle requests for setting RDK_TEXTURING extra requirements. The function iterates over the contents
	in 'aSelection' in the specified document 'rdkDoc'. For each content, it calls SetExtraRequirementParameter().
	The default implementation of this function looks at the field named in 'wszParamName' and handles the
	'wszExtraReqName' request as follows:

	  - RDK_TEXTURE_ON     - calls the SetTextureOn() method of the relevant field(s).
	  - RDK_TEXTURE_AMOUNT - calls the SetTextureDoubleAmount() method of the relevant field(s).

	If 'wszExtraReqName' is anything other than one of the above values, or the request cannot be satisfied
	for some other reason, the function returns \e false.

*/
RHRDK_SDK bool RhRdkSetTexturingExtraRequirement(const CRhRdkContentArray& aSelection,
                                                 const wchar_t* wszParamName, const wchar_t* wszExtraReqName, const CRhRdkVariant& vValue,
                                                 IAutoUIExtraRequirements::SetContext sc);
