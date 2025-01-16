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

#include "rhinoSdkGetPoint.h"

class RHINO_SDK_CLASS CArgsRhinoGetCone : public CArgsRhinoGetCircle
{
public:
  CArgsRhinoGetCone();
  ~CArgsRhinoGetCone();
  CArgsRhinoGetCone(const CArgsRhinoGetCone&);
  CArgsRhinoGetCone& operator=(const CArgsRhinoGetCone&);

  const wchar_t* ApexPrompt() const;
  void SetApexPrompt( const wchar_t* prompt );

  bool m_bAllowAngleInput; //If this is true Angle options are exposed in the end point prompt.
  double m_BaseAngle; //Angle between the base circle and the side of the cone in degrees.
  double m_ApexAngle; //Full angle of the apex, not the half angle, in degrees.

private:
  ON_wString m_apex_prompt;
};

class RHINO_SDK_CLASS CArgsRhinoGetConeExtra : public CArgsRhinoGetCone
{
public:
  CArgsRhinoGetConeExtra();
  ~CArgsRhinoGetConeExtra();
  CArgsRhinoGetConeExtra(const CArgsRhinoGetConeExtra&);
  CArgsRhinoGetConeExtra& operator=(const CArgsRhinoGetConeExtra&);
  CArgsRhinoGetConeExtra(const CArgsRhinoGetCone&);
  CArgsRhinoGetConeExtra& operator=(const CArgsRhinoGetCone&);
  
  const bool ShowHeight() const;              // Get the BothSides prompt
  const double Height() const;                // Get the m_Height value
  void SetHeight( double );                   // Set the m_Height value

  void SetCapFaceStyle(unsigned int);         // 0 means do not prompt, 3 for triangle, 4 for quads
  unsigned int CapFaceStyle();

  // November 12, 2024 - Tim
  // Partial fix for https://mcneel.myjetbrains.com/youtrack/issue/RH-63751
  // Add "BothSides" functionality to Cone.
  const bool ShowBothSides() const;          
  const bool BothSides() const;              
  void SetBothSides(bool);                   


private:
  // m_bShowHeight is only ever set in constructors so a function to set it is not included
  unsigned char m_bShowHeight; 
  double m_Height;

  // Reserved variables for future use without SDK breakage
  ON_wString m_ReservedPrompt1; 
  ON_wString m_ReservedPrompt2; 
  ON_wString m_ReservedPrompt3; 
  ON_wString m_ReservedPrompt4; 

  double m_ReservedDouble1;
  double m_ReservedDouble2;
  double m_ReservedDouble3;
  double m_ReservedDouble4;

  unsigned char m_ShowBothSides;
  unsigned char m_BothSides;
  unsigned char m_ReservedUChar3;
  unsigned char m_ReservedUChar4;

  unsigned int m_CapFaceStyle;
  unsigned int m_ReservedUInt2;
  unsigned int m_ReservedUInt3;
  unsigned int m_ReservedUInt4;
};

// Description:
//   UI tool to get a cone.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   cone - [out] cone returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetCone( 
    CArgsRhinoGetCone& args,
    ON_Cone& cone
    );

// Description:
//   New and improved UI tool to get a cone.
//   Prompts for and maintains height. 
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   cone - [out] cone returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetCone( 
    CArgsRhinoGetConeExtra& args,
    ON_Cone& cone
    );

