
#pragma once // Public SDK header

#if defined ON_RUNTIME_WIN && defined RHINO_SDK_MFC

#include "RhRdkContentCtrlBase.h"

/** \class CRhRdkContentCtrl

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkContentCtrl : public CRhRdkContentCtrlBase
{
public:
	CRhRdkContentCtrl();
	virtual ~CRhRdkContentCtrl();

	/** This method is deprecated in favor of the one below. */
	RDK_DEPRECATED static void NewDataSources(OUT ON_SimpleArray<IRhinoUiDataSource*>& aDS, const CRhRdkDocument* pRdkDoc);

	/** Get a collection of data sources that are needed to make a content control work.
		\param aDS accepts all the new data sources that the control needs in order to function.
		\param rdkDoc specifies the document within which the contents reside.
		The caller should add these to a controller and set the controller to the control.
		The caller must supply an additional data source to this controller that knows how
		to get and set the actual data used by IRhRdkContentCtrlInfo.
		The caller shall delete the data sources when they are no longer needed. */
	static void NewDataSources(OUT ON_SimpleArray<IRhinoUiDataSource*>& aDS, const CRhRdkDocument& rdkDoc);

	/** This method is deprecated in favor of GetContentArray(). */
	RDK_DEPRECATED virtual CRhRdkVariant InstanceId(void) const;

	/** This method is deprecated in favor of SetContentArray(). */
	RDK_DEPRECATED virtual bool SetInstanceId(const CRhRdkVariant& vInstance, bool bNotify);

	/** Get the content(s) referenced by the control.
		\return \e true if successful, else \e false. */
	virtual bool GetContentArray(CRhRdkContentArray& aOut) const;

	/** Set the content(s) referenced by the control.
		\return \e true if successful, else \e false. */
	virtual bool SetContentArray(const CRhRdkContentArray& a);

	// These methods are deprecated because the control is now data-aware
	// and is able to handle changes automatically.
	RDK_DEPRECATED static UINT ChangingMessageNumber(void);
	RDK_DEPRECATED static UINT ChangedMessageNumber(void);

	/** \return The kinds of content associated with the control.
		This can be a single kind or a semicolon-delimited string of multiple kinds. */
	virtual CRhRdkContentKindList AllowedKinds(void) const = 0;

protected:
	// These methods are deprecated because the control is now data-aware
	// and is able to handle changes automatically.
	RDK_DEPRECATED afx_msg LRESULT OnChanging(WPARAM, LPARAM);
	RDK_DEPRECATED afx_msg LRESULT OnChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

protected:
	/** \internal For RDK internal use only. */
	virtual IRhRdkContentCtrlBase* GetBaseInterface(void) const override;

protected:
	class CImpl;
	CImpl* m_pImpl;
};

//---- CRhRdkMaterialCtrl ----

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkMaterialCtrl : public CRhRdkContentCtrl
{
public:
	CRhRdkMaterialCtrl();

	virtual CRhRdkContentKindList AllowedKinds(void) const;
};

//---- CRhRdkEnvironmentCtrl ----

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkEnvironmentCtrl : public CRhRdkContentCtrl
{
public:
	CRhRdkEnvironmentCtrl();

	virtual CRhRdkContentKindList AllowedKinds(void) const;
};

//---- CRhRdkTextureCtrl ----

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkTextureCtrl : public CRhRdkContentCtrl
{
public:
	CRhRdkTextureCtrl();

	virtual CRhRdkContentKindList AllowedKinds(void) const;
};

#endif
