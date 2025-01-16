//
// Copyright (c) 1993-2017 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#pragma once

class RHINO_SDK_CLASS CRhinoDigitizerManager
{
  friend CRhinoDigitizerManager& CRhinoApp::DigitizerManager(); // only function that can create this class

  CRhinoDigitizerManager();
public:
  ~CRhinoDigitizerManager();

  void LoadProfile(CRhinoProfileContext& pc);

  // called in CRhinoApp::ExitInstance
  void SaveProfile(CRhinoProfileContext& pc) const;

  void Default();

  /*
  Returns:
    UUID of the digitizer plugin or ON_nil_uuid if none 
  */
  CRhinoCommand::result GetDigitizer( 
                  const CRhinoCommandContext& context,
                  ON_UUID& digitizer_plugin_uuid
                  );

  bool ConnectDigitizer( 
    ON_UUID digitizer_plugin_uuid
    );
  
  bool CalibrateDigitizer();

  void DisconnectDigitizer();

  bool UpdateTransform( 
    bool=false // bForceScaleUpdate=true forces m_scale to be set even if unit system is unchanged
    );

  /*
  Description:
    Commands that need a digitizer can call CheckDigitizer
    to make sure everything is ready.  It takes care
    of starting a digitizer if one is not already running.
  */
  CRhinoCommand::result CheckDigitizer( const CRhinoCommandContext& context, bool bRecalibrate = false );

  /*
  Description:
    When the digitizer is "paused", it cannot be
    used for getting points.
  Parameters:
    bPause - [in]
  Returns:
    Previous pause state
  */
  bool PauseDigitizer(bool bPause = true);

  /*
  Description:
    Mimic a digitizer "foot pedal" click.
  Returns:
    True if successful.
  */
  bool MimicDigClick();

  bool IsConnected() const; // true if a digitizer is connected
  bool IsCalibrated() const; // true if a digitizer is connected and calibrated
  bool IsPaused() const; // true if a digitizer is connected and paused
  bool IsActive() const; // true if GetPoint can accept digitizer input

  double UserScale() const;

  // returns previous value
  double SetUserScale(double user_scale);

  // Return the UUID for the current digitizer plug-in.
  UUID CurrentPlugInID() const;

  // get a raw dig point during calibration
  CRhinoGet::result GetDigClickPoint( const wchar_t* prompt, bool bAcceptEnter, ON_3dPoint& pt);
  void GetTransformedRay(const ON_3dRay& raw_ray, ON_3dRay& transformed_ray);
  const CRhinoDigitizerPlugIn* CurrentDigitizerPlugIn() const;

  // These points are in digitizer coordinates and are 
  // set by the "DigCalibrate" command
  ON_3dPoint m_dig_origin;
  ON_3dVector m_dig_xaxis;
  ON_3dVector m_dig_yaxis;

  // These points are in rhino world coordinates and are 
  // set by the "DigCalibrate" command
  ON_3dPoint m_rhino_origin;
  ON_3dVector m_rhino_xaxis;
  ON_3dVector m_rhino_yaxis;

  // This information is from m_current_plugin
  ON::LengthUnitSystem m_dig_us;
  double m_dig_tol; // in m_dig_us units


  // true when we are getting 3 raw coord dig calibration pts
  bool m_bGetDigClickPoint; 
  bool m_bDigCalibrated;
  bool m_bDigPaused;

  // This value is set by carefully placed calls to 
  // GetCurrentModel()->Settings().m_ModelUnitsAndTolerances.m_unit_system;
  // bool m_bRhinoRunning; // true after CRhinoApp::InitInstance()
  ON::LengthUnitSystem m_rhino_us;

private:
  // This user defined scale factor is set by the "DigScale" command
  double m_user_scale;

  friend void InitDigitizer();
  friend void CleanupDigitizer();

  CRhinoDigitizerPlugIn* m_current_plugin;


  bool CheckUnitSystem( // returns true if Rhino unit system has changed
    bool=false // bForceScaleUpdate=true forces m_scale to be set even if unit system is unchanged
    );  


  // The information above is used to compute this transformation.
  // This transformation is used to convert the (x,y,z) values sent
  // from the digitizer device into the (z,y,z) values pushed onto
  // get point.
public:
  ON_Xform m_dig_point_xform;  // transforms point (has unit system scaling)
  ON_Xform m_dig_vector_xform; // transforms directions
  ON_Plane m_RhinoOriginPlane;
  double m_scale; // Digitizer to Rhino scale factor

  LPARAM m_nFlags; // value of nFlags on last point sent to PostDigitizerPointEvent
private:
#if !defined ON_RUNTIME_APPLE
  friend LRESULT CALLBACK SendPointWndProc(HWND,UINT,WPARAM,LPARAM);
#endif

  void ReceivePoint( ON_3dRay raw_ray, LPARAM nFlags);
  ON_3dRay m_most_recent_raw_ray;

  ON_3dRay m_dig_ray;   // in digitizer coordinates
  ON_3dRay m_rhino_ray; // in rhino coordinates

private:
  ON_UUID m_mru_dig_plugin; // uuid of last active dig plugin
};
