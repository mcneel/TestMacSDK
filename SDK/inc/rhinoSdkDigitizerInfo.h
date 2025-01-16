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

/*
Description:
  The CRhinoDigitizerInfo class holds the current state
  of the active 3D digitizing device.
Also See:
  RhinoGetDigitizerInfo
*/
class RHINO_SDK_CLASS CRhinoDigitizerInfo
{
public:
  CRhinoDigitizerInfo();
  ~CRhinoDigitizerInfo();

  UUID m_uuid;          // uuid of the current digitizer

  bool m_bIsConnected;  // true if a digitizer is connected
  bool m_bIsCalibrated; // true if a digitizer is connected and calibrated
  bool m_bIsPaused;     // true if a digitizer is connected and paused
  bool m_bIsActive;     // true if GetPoint can accept digitizer input

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

  // This information is from the current digitizer
  ON::LengthUnitSystem m_dig_us;
  double m_dig_tol; // in m_dig_us units

  // This user defined scale factor is set by the "DigScale" command
  double m_user_scale;

  ON_Xform m_dig_point_xform;  // transforms point (has unit system scaling)
  ON_Xform m_dig_vector_xform; // transforms directions
  double m_scale;              // digitizer to rhino scale factor
};

/*
Description:
  Returns the current state of the active 3D digitizing device.
Parameters:
  none.
Returns:
  CRhinoDigitizerInfo - the current state of the active 3D digitizing device.
Also See:
  CRhinoDigitizerInfo
*/

RHINO_SDK_FUNCTION
CRhinoDigitizerInfo RhinoGetDigitizerInfo();