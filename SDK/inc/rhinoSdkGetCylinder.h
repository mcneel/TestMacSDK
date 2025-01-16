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
#include <functional>

class RHINO_SDK_CLASS CArgsRhinoGetCylinder : public CArgsRhinoGetCircle
{
public:
  CArgsRhinoGetCylinder();
  ~CArgsRhinoGetCylinder();
  CArgsRhinoGetCylinder(const CArgsRhinoGetCylinder&);
  CArgsRhinoGetCylinder& operator=(const CArgsRhinoGetCylinder&);

  const wchar_t* HeightPrompt() const;
  void SetHeightPrompt( const wchar_t* prompt );

private:
  ON_wString m_height_prompt;
};

class RHINO_SDK_CLASS CArgsRhinoGetCylinderExtra : public CArgsRhinoGetCylinder
{
public:
  CArgsRhinoGetCylinderExtra();
  ~CArgsRhinoGetCylinderExtra();
  CArgsRhinoGetCylinderExtra(const CArgsRhinoGetCylinderExtra&);
  CArgsRhinoGetCylinderExtra& operator=(const CArgsRhinoGetCylinderExtra&);
  CArgsRhinoGetCylinderExtra(const CArgsRhinoGetCylinder&);
  CArgsRhinoGetCylinderExtra& operator=(const CArgsRhinoGetCylinder&);
  
  const CRhinoCommandOptionName& BothSidesPrompt() const;          // Get the BothSides prompt
  void SetBothSidesPrompt(const CRhinoCommandOptionName& prompt);  // Set the BothSides prompt
  const bool BothSides() const;                                    // Get the m_bBothSides value
  void SetBothSides( bool );                                       // Set the m_bBothSides value

  const bool ShowHeight() const;              // Get the ShowHeight value
  const double Height() const;                // Get the m_Height value
  void SetHeight( double );                   // Set the m_Height value

  void SetCapFaceStyle(unsigned int);         // 0 means do not prompt, 3 for triangle, 4 for quads
  unsigned int CapFaceStyle();

  /*
  Description:
  Set callback function for doing custom dynamic drawing
  Parameters:
  callback [in] Callback function for dynamic drawing
  Remarks:
  Good for custom drawing when calling ::RhinoGetCylinder
  */
  void SetDynamicCylinderCallback(std::function<void(const ON_Cylinder&)> callback);

  /*
  Description:
  Get callback function for doing custom dynamic drawing
  Returns:
  Callback function
  Remarks:
  Good for custom drawing when calling ::RhinoGetCylinder
  */
  std::function<void(const ON_Cylinder&)> DynamicCylinderCallback() const;

private:
  CRhinoCommandOptionName m_BothSidesPrompt; // String to Display as the BothSides prompt
  unsigned char m_bBothSides; 

  // m_bShowHeight is only ever set in constructors so a function to set it is not included
  unsigned char m_bShowHeight; 
  double m_Height;

  std::function<void(const ON_Cylinder&)> m_dynamicCylinderCallback;

  // Reserved variables for future use without SDK breakage
  ON_wString m_ReservedPrompt1; 
  ON_wString m_ReservedPrompt2; 
  ON_wString m_ReservedPrompt3; 
  ON_wString m_ReservedPrompt4; 

  double m_ReservedDouble1;
  double m_ReservedDouble2;
  double m_ReservedDouble3;
  double m_ReservedDouble4;

  unsigned char m_ReservedUChar1;
  unsigned char m_ReservedUChar2;
  unsigned char m_ReservedUChar3;
  unsigned char m_ReservedUChar4;

  unsigned int m_CapFaceStyle;
  unsigned int m_ReservedUInt2;
  unsigned int m_ReservedUInt3;
  unsigned int m_ReservedUInt4;
};

class RHINO_SDK_CLASS CArgsRhinoGetCylinderEx : public CArgsRhinoGetCylinder
{
public:
  CArgsRhinoGetCylinderEx();
  ~CArgsRhinoGetCylinderEx();
  CArgsRhinoGetCylinderEx(const CArgsRhinoGetCylinderEx&);
  CArgsRhinoGetCylinderEx& operator=(const CArgsRhinoGetCylinderEx&);
};

class RHINO_SDK_CLASS CArgsRhinoGetTube : public CArgsRhinoGetCylinder
{
public:
  CArgsRhinoGetTube();
  ~CArgsRhinoGetTube();
  CArgsRhinoGetTube(const CArgsRhinoGetTube&);
  CArgsRhinoGetTube& operator=(const CArgsRhinoGetTube&);

  void SetTConeDefaultPrompts();
  
  double m_dSecondRadius;
  double m_dHeight;

private:
  ON_wString m_height_prompt;
};

class RHINO_SDK_CLASS CArgsRhinoGetTubeExtra : public CArgsRhinoGetCylinderExtra
{
public:
  CArgsRhinoGetTubeExtra();
  ~CArgsRhinoGetTubeExtra();
  CArgsRhinoGetTubeExtra(const CArgsRhinoGetTubeExtra&);
  CArgsRhinoGetTubeExtra& operator=(const CArgsRhinoGetTubeExtra&);
  CArgsRhinoGetTubeExtra(const CArgsRhinoGetTube&);

  void SetTConeDefaultPrompts();

  const wchar_t* ThicknessPrompt() const;        // Get the Thickness prompt
  void SetThicknessPrompt( const wchar_t* );     // Set the Thickness prompt
  const bool PromptForThickness() const;         // Get the m_bPromptForThickness value
  void SetPromptForThickness( bool );            // Set the m_bPromptForThickness value
  const double Thickness() const;                // Get the m_Thickness value
  void SetThickness( double );                   // Set the m_Thickness value

  const wchar_t* SecondRadiusPrompt() const;     // Get the SecondRadius prompt
  void SetSecondRadiusPrompt( const wchar_t* );  // Set the SecondRadius prompt
  const double SecondRadius() const;             // Get the m_dSecondRadius value
  void SetSecondRadius( double );                // Set the m_dSecondRadius value

private:
  ON_wString m_ThicknessPrompt;
  unsigned int m_bPromptForThickness;
  double m_Thickness;
  ON_wString m_SecondRadiusPrompt;
  double m_dSecondRadius;

  // Reserved variables for future use without SDK breakage
  ON_wString m_TubeReservedPrompt1; 
  ON_wString m_TubeReservedPrompt2; 

  double m_TubeReservedDouble1;
  double m_TubeReservedDouble2;

  unsigned char m_TubeReservedUChar1;
  unsigned char m_TubeReservedUChar2;

  unsigned int m_TubeReservedUInt1;
  unsigned int m_TubeReservedUInt2;
};


// Description:
//   UI tool to get a cylinder.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   cylinder - [out] cylinder returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetCylinder( 
    CArgsRhinoGetCylinder& args,
    ON_Cylinder& cylinder
    );

// Description:
//   New and improved UI tool to get a cylinder.
//   Prompts for and maintains height and also
//   has a BothSides option.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   cylinder - [out] cylinder returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetCylinder( 
    CArgsRhinoGetCylinderExtra& args,
    ON_Cylinder& cylinder);

// Description:
//   New and improved UI tool to get a cylinder shaped extrusion.
//   Prompts for and maintains height and also
//   has a BothSides option.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   cylinder - [out] cylinder returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetCylinderExtrusion(
  CArgsRhinoGetCylinderExtra& args,
  ON_Extrusion& cylinder);

// Description:
//   UI tool to get a tube.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   tube - [out] tube returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetTube( 
    CArgsRhinoGetTube& args,
    ON_Brep& Tube
    );

// Description:
//   UI tool to get a tube.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   tube - [out] tube returned here
//   bPromptForThickness - [in] determines whether to prompt for WallThickness
//   WallThickness - [in] initial value for WallThickness, used if bPromptForThickness is true
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetTube( 
    CArgsRhinoGetTube& args,
    ON_Brep& Tube,
    bool bPromptForThickness,
    double& WallThickness
    );

// Description:
//   New and improved UI tool to get a tube.
//   Wraps Thickness stuff into the args class and
//   also has a BothSides option.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   tube - [out] tube returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetTube( 
    CArgsRhinoGetTubeExtra& args,
    ON_Brep& Tube
    );


// Description:
//   UI tool to get a tcone.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   tcone - [out] tcone returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetTcone( 
    CArgsRhinoGetTube& args,
    ON_Brep& Tcone
    );

// Description:
//   UI tool to get a tcone.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   tcone - [out] tcone returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetTcone(
  CArgsRhinoGetTubeExtra& args,
  ON_Brep& Tcone
);

// Description:
//   UI tool to get a tcone.  Old style.  Circle, circle, height.
// Parameters:
//   args - [in] controls get point prompts, options, etc.
//   tcone - [out] tcone returned here
// Returns:
//   CRhinoCommand::success if successful.
RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetCCHTcone( 
    CArgsRhinoGetTube& args,
    ON_Brep& Tcone
    );

