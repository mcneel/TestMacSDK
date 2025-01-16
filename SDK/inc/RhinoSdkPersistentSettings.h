#pragma once

class RHINO_SDK_CLASS CRhinoSettings
{
protected:
  friend class CRhSettingsHooks;
  friend class CRhPlugInSettings;
  friend class CRhinoCommand;

  CRhinoSettings(ON_UUID plugInId, unsigned int pointerId, const ON_UUID& commandId, const wchar_t* englishCommandName);

public:
  ~CRhinoSettings();

  /*
  Description:
    Get settings associated with a plug-in
  Parameters:
    plugInId [in] Id of the plug-in to get settings from
    forcePlugInToLoad [in] Only used if the plug-in is not currently loaded, if it is not
      currently loaded and this is set to true then the plug-in will load and its settings
      returned, if this is false and the plug-in is not loaded NULL will be returned.
  Returns:
    Returns a pointer to a settings class if the plug-in is loaded otherwise returns NULL.
  */
  static CRhinoSettings* GetPlugInSettings(const ON_UUID& plugInId, bool forcePlugInToLoad);

  const ON_UUID& PlugInId() const { return m_plug_in_id; }
  const ON_UUID& CommandId() const { return m_command_id; }
  const wchar_t* Name() const { return m_english_command_name; }

  CRhinoSettings* GetChild(const wchar_t* key) const;
  CRhinoSettings* AddChild(const wchar_t* key);
  bool DeleteChild(const wchar_t* key);

  int GetChildKeys(ON_ClassArray<ON_wString>& keys) const;
  int GetKeys(ON_ClassArray<ON_wString>& keys) const;

  bool HiddenFromUserInterface() const;
  bool HideFromUserInterface(bool hide);

  bool DeleteItem(const wchar_t* key);

  bool SetDefaultString(const wchar_t* key, const wchar_t* value);
  bool SetDefaultColor(const wchar_t* key, const ON_Color& value);
#if defined(ON_RUNTIME_WIN)
  bool SetDefaultColor(const wchar_t* key, COLORREF value);
#endif
  bool SetDefaultRect(const wchar_t* key, const RECT& value);
  bool SetDefaultPoint(const wchar_t* key, const POINT& value);
  bool SetDefaultSize(const wchar_t* key, const SIZE& value);
  bool SetDefaultStringList(const wchar_t* key, const ON_ClassArray<ON_wString>& value);
  bool SetDefaultStringDictionary(const wchar_t* key, const CRhinoStringDictionary& value);
  bool SetDefaultUuid(const wchar_t* key, const ON_UUID& value);
  bool SetDefaultInteger(const wchar_t* key, int value);
  bool SetDefaultInteger(const wchar_t* key, unsigned int value);
  bool SetDefaultFloat(const wchar_t* key, float value);
  bool SetDefaultDouble(const wchar_t* key, double value);
  bool SetDefaultBool(const wchar_t* key, bool value);

  bool SetString(const wchar_t* key, const wchar_t* value);
  bool SetColor(const wchar_t* key, const ON_Color& value);
#if defined(ON_RUNTIME_WIN)
  bool SetColor(const wchar_t* key, COLORREF value);
#endif
  bool SetRect(const wchar_t* key, const ON_4iRect& value);
  bool SetRect(const wchar_t* key, const RECT& value);
  bool SetPoint(const wchar_t* key, const POINT& value);
  bool SetSize(const wchar_t* key, const SIZE& value);
  bool SetStringList(const wchar_t* key, const ON_ClassArray<ON_wString>& value);
  bool SetStringDictionary(const wchar_t* key, const CRhinoStringDictionary& value);
  bool SetUuid(const wchar_t* key, const ON_UUID& value);
  bool Set3dPoint(const wchar_t* key, const ON_3dPoint& value);
  bool Set3dVector(const wchar_t* key, const ON_3dVector& value);
  bool SetInteger(const wchar_t* key, int value);
  bool SetInteger(const wchar_t* key, unsigned int value);
  bool SetFloat(const wchar_t* key, float value);
  bool SetDouble(const wchar_t* key, double value);
  bool SetBool(const wchar_t* key, bool value);
  bool SetHideFromUserInterface(const wchar_t* key, bool value);

  bool GetString(const wchar_t* key, ON_wString& value) const;
  bool GetString(const wchar_t* key, ON_wString& value, const wchar_t* defaultValue) const;
  bool GetColor(const wchar_t* key, ON_Color& value) const;
  bool GetColor(const wchar_t* key, ON_Color& value, const ON_Color& defaultValue) const;
  bool GetRect(const wchar_t* key, ON_4iRect& value) const;
  bool GetRect(const wchar_t* key, ON_4iRect& value, const ON_4iRect& defaultValue) const;
  bool GetRect(const wchar_t* key, RECT& value) const;
  bool GetRect(const wchar_t* key, RECT& value, const RECT& defaultValue) const;
  bool GetPoint(const wchar_t* key, POINT& value) const;
  bool GetPoint(const wchar_t* key, POINT& value, const POINT& defaultValue) const;
  bool GetSize(const wchar_t* key, SIZE& value) const;
  bool GetSize(const wchar_t* key, SIZE& value, const SIZE& defaultValue) const;
#if defined(ON_RUNTIME_WIN)
  bool GetColor(const wchar_t* key, COLORREF& value) const;
  bool GetColor(const wchar_t* key, COLORREF& value, COLORREF defaultValue) const;
#endif
  bool GetStringList(const wchar_t* key, ON_ClassArray<ON_wString>& value) const;
  bool GetStringList(const wchar_t* key, ON_ClassArray<ON_wString>& value, const ON_ClassArray<ON_wString>& defaultValue) const;
  bool GetStringDictionary(const wchar_t* key, CRhinoStringDictionary& value) const;
  bool GetStringDictionary(const wchar_t* key, CRhinoStringDictionary& value, const CRhinoStringDictionary& defaultValue) const;
  bool GetUuid(const wchar_t* key, ON_UUID& value) const;
  bool GetUuid(const wchar_t* key, ON_UUID& value, const ON_UUID& defaultValue) const;
  bool Get3dPoint(const wchar_t* key, ON_3dPoint& value) const;
  bool Get3dPoint(const wchar_t* key, ON_3dPoint& value, const ON_3dPoint& defaultValue) const;
  bool Get3dVector(const wchar_t* key, ON_3dVector& value) const;
  bool Get3dVector(const wchar_t* key, ON_3dVector& value, const ON_3dVector& defaultValue) const;
  bool GetInteger(const wchar_t* key, int& value, const wchar_t*const* legacyKeyList = NULL, int count = 0) const;
  bool GetInteger(const wchar_t* key, int& value, int defaultValue, const wchar_t*const* legacyKeyList = NULL, int count = 0) const;
  bool GetInteger(const wchar_t* key, int& value, int defaultValue, int bound, bool boundIsLower) const;
  bool GetInteger(const wchar_t* key, int& value, int defaultValue, int lowerBound, int upperBound) const;
  bool GetInteger(const wchar_t* key, unsigned int& value) const;
  bool GetInteger(const wchar_t* key, unsigned int& value, unsigned int defaultValue) const;
  bool GetFloat(const wchar_t* key, float& value) const;
  bool GetFloat(const wchar_t* key, float& value, float defaultValue) const;
  bool GetDouble(const wchar_t* key, double& value) const;
  bool GetDouble(const wchar_t* key, double& value, double defaultValue) const;
  bool GetBool(const wchar_t* key, bool& value) const;
  bool GetBool(const wchar_t* key, bool& value, bool defaultValue) const;
  bool GetHideFromUserInterface(const wchar_t* key, bool& value) const;

protected:
  ON_UUID m_plug_in_id;
  unsigned int m_pointer_id;
  ON_UUID m_command_id;
  ON_wString m_english_command_name;
  ON_SimpleArray<CRhinoSettings*>m_children;
};
