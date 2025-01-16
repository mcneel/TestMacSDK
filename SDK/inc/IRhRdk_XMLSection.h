
#pragma once // Public SDK header

#include "RhRdkVariant.h"

/** \class IRhRdk_XMLSection

	This interface represents one section of an XML stream. For example:

	\verbatim

	<section>
		<color type="color">0.5,0.8,0.9</color>
		<depth type="float">1.0</depth>
		<radius type="double" special="true" >5.0</radius>
	</section>

	\endverbatim

	the interface can be used to access the color, depth and radius values.
	'special' is a marker-string ('beacon') which can be used for searching.
*/
class RHRDK_SDK IRhRdk_XMLSection
{
public:
	class RHRDK_SDK IExtraRequirements
	{
	public:
		virtual ~IExtraRequirements() { }

		virtual int Count(void) const = 0;
		virtual ON_wString Name(int iIndex) const = 0;
		virtual CRhRdkVariant Value(int iIndex) const = 0;
		virtual bool FindValue(const wchar_t* wszName, CRhRdkVariant& vValueOut) const = 0;
		virtual void* EVF(const wchar_t*, void*) =0;
	};

	virtual ~IRhRdk_XMLSection() { }

	/** Get the section data as an XML string. */
	virtual ON_wString AsString(void) const = 0;

	/** Set the section data from an XML string. */
	virtual void SetAsString(const wchar_t* wsz) = 0;

	/** Set a parameter. The parameter is created if it doesn't exist.
		\param wszParamName is the name that will appear in the XML output.
		\param vParamValue is the value to assign to the name.
		\param pExtra is an optional pointer to an object containing data for extra requirements.
		\param wszBeacon is an optional marker string which will appear in the XML output
		 and can be used for special purpose searching. */
	virtual void SetParam(const wchar_t* wszParamName, const CRhRdkVariant& vParamValue,
	                      const IExtraRequirements* pExtra=NULL, const wchar_t* wszBeacon=NULL) = 0;

	/** Get a parameter by its internal name.
		\param wszParamName is the name of the parameter to get.
		\param vParamValueOut receives the value of the parameter if it exists.
		\param ppExtraOut is an optional pointer that, if not NULL, will receive a pointer
		 to an object containing the data of the parameter's extra requirements.
		\return \e true if successful or \e false if the parameter does not exist. */
	virtual bool GetParam(const wchar_t* wszParamName, CRhRdkVariant& vParamValueOut,
	                      const IExtraRequirements ** ppExtraOut=NULL) const = 0;

	class RHRDK_SDK Iterator
	{
	public:
		virtual ~Iterator() { }

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t*, void*) = 0;
	};

	/** Get an iterator to use when calling NextParam().
		The caller shall delete the iterator. */
	virtual Iterator* NewIterator(void) const = 0;

	/** Get the next parameter name and value from the section.
		\return \e true on success, \e false if no more parameters. */
	virtual bool NextParam(Iterator* pIterator, ON_wString& sParamNameOut,
	                       CRhRdkVariant& vParamValueOut, const IExtraRequirements** ppExtraOut=NULL) const = 0;

	/** \return \e true if the state of this object is equal to another one. */
	virtual bool IsEqual(const IRhRdk_XMLSection& s) const = 0;

	/** Dump the parameters to the debugger output pane. */
	virtual void DebugDump(void) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;
};


class RHRDK_SDK IRhRdk_XMLSection2 : public IRhRdk_XMLSection
{
public:
	virtual ~IRhRdk_XMLSection2() {}

	virtual const CRhinoFileReadOptions* FileReadOptions(void) const = 0;
	virtual const CRhinoFileWriteOptions* FileWriteOptions(void) const = 0;
};
