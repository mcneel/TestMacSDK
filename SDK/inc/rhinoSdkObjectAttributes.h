//
// Copyright (c) 1993-2021 Robert McNeel & Associates. All rights reserved.
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


// RHINO_COMPONENT_INDEX is OBSOLETE as of 9/30/2004 - user ON_COMPONENT_INDEX
#define RHINO_COMPONENT_INDEX OBSOLETE__USE__ON_COMPONENT_INDEX

class RHINO_SDK_CLASS CRhinoObjectAttributes : public ON_3dmObjectAttributes
{
public:
  CRhinoObjectAttributes();
  ~CRhinoObjectAttributes();
  CRhinoObjectAttributes(const CRhinoObjectAttributes&);
  CRhinoObjectAttributes(const ON_3dmObjectAttributes&);
  CRhinoObjectAttributes& operator=(const CRhinoObjectAttributes&);
  CRhinoObjectAttributes& operator=(const ON_3dmObjectAttributes&);

  ON_Color DrawColor(const CRhinoDoc* = NULL ) const;
  ON_Color DrawColor(const CRhinoDoc* doc, const ON_UUID& viewport_id ) const;
  ON_Color PlotColor(const CRhinoDoc* = NULL ) const;
  ON_Color PlotColor(const CRhinoDoc* doc, const ON_UUID& viewport_id ) const;
  double PlotWeight(const CRhinoDoc* = NULL ) const;
  double PlotWeight(const CRhinoDoc* doc, const ON_UUID& viewport_id ) const;

  // Description:
  //  Get section attributes
  ON_SectionStyle ComputedSectionStyle(const CRhinoDoc* doc, 
    const CRhinoObjectAttributes* sectionerAttributes,
    bool computeColors) const;

  // Description:
  //   Returns the effective linetype index to be used to find the 
  //     linetype definition to draw an object
  //   If an object's linetype source is ON::linetype_from_object,
  //     the linetype index in the object's attributes is used.
  //   If an object's linetype source is ON::linetyper_from_layer,
  //     the linetype index from the object's layer is used.
  //
  //  Returns:
  //    Zero based linetype table index of the effective linetype.
  int EffectiveLinetypeIndex(const CRhinoDoc* doc = nullptr) const;

  // prints localized summary of attributes
  void Dump( ON_TextLog& ) const;
};

