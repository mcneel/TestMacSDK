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

class RHINO_SDK_CLASS CArgsRhinoGetTorus : public CArgsRhinoGetCircle
{
public:
  CArgsRhinoGetTorus();
  ~CArgsRhinoGetTorus();
  CArgsRhinoGetTorus(const CArgsRhinoGetTorus&);
  CArgsRhinoGetTorus& operator=(const CArgsRhinoGetTorus&);

public:
  int m_bFixInnerRadius;
  int m_bDiameter_Radius;
  double m_Second_Radius;
};

RHINO_SDK_CLASS
CRhinoCommand::result RhinoGetTorus(
  CArgsRhinoGetTorus& args,
  ON_Torus& output_torus
);

