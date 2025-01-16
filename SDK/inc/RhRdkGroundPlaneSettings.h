
#pragma once // UI SDK header

#if 0

class RHRDK_UI_SDK CRhRdkDocumentGroundPlaneSettings : protected CRhRdkEventWatcher
{
public:
	CRhRdkDocumentGroundPlaneSettings(const CRhinoDoc& doc);
	CRhRdkDocumentGroundPlaneSettings(CRhRdkDocument& rdkDoc);
	virtual ~CRhRdkDocumentGroundPlaneSettings();

	bool On(void) const;
	void SetOn(bool b);

	bool ShowUnderside(void) const;
	void SetShowUnderside(bool);

	bool AutoAltitude(void) const;
	void SetAutoAltitude(bool b);

	bool ShadowOnly(void) const;
	void SetShadowOnly(bool b);

	double Altitude(void) const;
	void SetAltitude(double d);

	UUID MaterialInstanceId(void) const;
	void SetMaterialInstanceId(const UUID& uuidInstance);

	ON_2dVector TextureOffset(void) const;
	void SetTextureOffset(const ON_2dVector&);

	ON_2dVector TextureSize(void) const;
	void SetTextureSize(const ON_2dVector&);

	double TextureRotation(void) const;
	void SetTextureRotation(double d);

	bool OffsetLocked(void) const;
	void SetOffsetLocked(bool b);

	bool RepeatLocked(void) const;
	void SetRepeatLocked(bool b);

public:
	void BeginNotifyBracket(void);
	void EndNotifyBracket(void);

	CRhRdkDocument* RdkDocument(void) const;

protected: // RDK event watcher overrides.
	virtual void OnGroundPlaneSettingsChanged(const CRhRdkDocument& rdkDoc, const IRhRdkGroundPlaneEventInfo& newGP, const IRhRdkGroundPlaneEventInfo& oldGP, const void* pSender) override;
	virtual void OnNewRdkDocument(const CRhRdkDocument& rdkDoc) override;

private:
	CRhRdkVariant Setting(const wchar_t* wszName, CRhRdkVariant& vCache) const;
	void SetSetting(const wchar_t* wszName, const CRhRdkVariant& vValue, CRhRdkVariant& vCache);

public:
	void HandleNotify(void);
	void CopyToGroundPlaneEventInfo(class CRhRdkGroundPlaneEventInfo& info) const;

private:
	class CImpl;
	CImpl* m_pImpl;
};

#endif
