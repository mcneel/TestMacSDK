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

class RHINO_SDK_CLASS CArgsRhinoGetSphere : public CArgsRhinoGetCircle
{
public:
  CArgsRhinoGetSphere();
  ~CArgsRhinoGetSphere();
  CArgsRhinoGetSphere(const CArgsRhinoGetSphere&);
  CArgsRhinoGetSphere& operator=(const CArgsRhinoGetSphere&);
};

RHINO_SDK_FUNCTION
CRhinoCommand::result RhinoGetSphere( 
    CArgsRhinoGetSphere& args,
    ON_Sphere& output_sphere
    );

class RHINO_SDK_CLASS CArgsRhinoGetSphereEx : public CArgsRhinoGetSphere
{
public:
  CArgsRhinoGetSphereEx();
  ~CArgsRhinoGetSphereEx();
  CArgsRhinoGetSphereEx(const CArgsRhinoGetSphereEx&);
  CArgsRhinoGetSphereEx& operator=(const CArgsRhinoGetSphereEx&);
};

