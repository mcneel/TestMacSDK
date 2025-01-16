
#pragma once // Public SDK header

/** \class CRhRdkLinearWorkflow

	This helper class provides simple gamma and linear workflow processing.

	If you want to get the linear workflow or gamma settings being
	used by a document, please see CRhRdkDocument::LinearWorkflow().

	Note that this does not support the EVF functions "PostProcessGammaOn" or "SetPostProcessGammaOn"
	mentioned in IRhRdkLinearWorkflow. Please use CRhRdkLinearWorkflowEx to solve this.

*/
class RHRDK_SDK CRhRdkLinearWorkflow : public IRhRdkLinearWorkflow
{
public:
	CRhRdkLinearWorkflow();
	CRhRdkLinearWorkflow(const IRhRdkLinearWorkflow& lw);

	const CRhRdkLinearWorkflow& operator = (const IRhRdkLinearWorkflow& lw);

	bool operator == (const IRhRdkLinearWorkflow& lw) const;
	bool operator != (const IRhRdkLinearWorkflow& lw) const;

public: // Implement IRhRdkLinearWorkflow.
	virtual IRhRdkLinearWorkflow& BeginChange(RhRdkChangeContext cc) const override;
	virtual bool EndChange(void) override;

	virtual bool PreProcessTextures(void) const override;
	virtual bool PreProcessColors(void) const override;
	virtual bool PostProcessFrameBuffer(void) const override;

	virtual void SetPreProcessTextures(bool b) override;
	virtual void SetPreProcessColors(bool b) override;
	virtual void SetPostProcessFrameBuffer(bool b) override;

	virtual float PreProcessGamma(void) const override;
	virtual float PostProcessGamma(void) const override;
	virtual float PostProcessGammaReciprocal(void) const override;

	virtual void SetPreProcessGamma(float fGamma) override;
	virtual void SetPostProcessGamma(float fGamma) override;

	virtual void ApplyPreProcessGamma(CRhRdkColor& col, bool bForTexture) const override;
	virtual void ApplyPreProcessGamma(CRhinoDib& dibInOut) const override;
	virtual void ApplyPostProcessGamma(CRhinoDib& dibInOut) const override;

	virtual ON__UINT32 ComputeCRC(void) const override;

	virtual bool IsEqual(const IRhRdkLinearWorkflow& lw) const override;
	virtual void CopyFrom(const IRhRdkLinearWorkflow& lw) override;

	virtual void* EVF(const wchar_t*, void*) override { return nullptr; }

	void CopyFrom(const ON_LinearWorkflow& lw);

private:
	mutable int m_iBeginChange = 0;

	float m_fPreProcessGamma = 1.f;
	float m_fPostProcessGamma = 1.f;
	float m_fPostProcessGammaReciprocal = 1.f;

	bool m_bPreProcessTextures = false;
	bool m_bPreProcessColors= false;
	bool m_bPostProcessFrameBuffer = false;

	void* m_pReserved = nullptr;
};

/** \class CRhRdkLinearWorkflowEx

	This helper class extends CRhRdkLinearWorkflow to add support for
	turning post-process gamma on and off.

	If you want to get the linear workflow or gamma settings being
	used by a document, please see CRhRdkDocument::LinearWorkflow().

*/
class RHRDK_SDK CRhRdkLinearWorkflowEx : public CRhRdkLinearWorkflow
{
public:
	CRhRdkLinearWorkflowEx();
	CRhRdkLinearWorkflowEx(const IRhRdkLinearWorkflow& lw);
	CRhRdkLinearWorkflowEx(const ON_LinearWorkflow& lw);

	const CRhRdkLinearWorkflowEx& operator = (const IRhRdkLinearWorkflow& lw);

	bool operator == (const IRhRdkLinearWorkflow& lw) const;
	bool operator != (const IRhRdkLinearWorkflow& lw) const;

public: // Implement IRhRdkLinearWorkflow.
	virtual void ApplyPostProcessGamma(CRhinoDib& dibInOut) const override;

	virtual void* EVF(const wchar_t*, void*) override;

public:
	// Get the linear workflow as an XML string.
	ON_wString XML(void) const;

	// Set the linear workflow from an XML string.
	void SetXML(const wchar_t* wszXML);

	// Updated version of ComputeCRC() for internal use.
	ON__UINT32 CRC(ON__UINT32 current_remainder) const;

private:
	bool m_bOn = false;
	void* m_pReserved = nullptr;
};
