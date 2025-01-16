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

class RHINO_SDK_CLASS CDisplayAttributeMaterial : public ON_Material
{
private:
  ON_OBJECT_DECLARE( CDisplayAttributeMaterial );


public:
  CDisplayAttributeMaterial();
  CDisplayAttributeMaterial(const ON_Material&);
  CDisplayAttributeMaterial(const CDisplayAttributeMaterial&);

  ~CDisplayAttributeMaterial();
            
  void      Default(); 

  bool AppearanceEquals(const CDisplayAttributeMaterial& other) const;
  bool                       operator==(const CDisplayAttributeMaterial&) const;
  CDisplayAttributeMaterial&  operator=(const CDisplayAttributeMaterial&);
  CDisplayAttributeMaterial&  operator=(const ON_Material& src);
  
public:
  bool            m_bOverrideObjectColor;
  bool            m_bOverrideObjectTransparency;
  bool            m_bOverrideObjectReflectivity;

  bool            m_bFlatShaded;
  
  int             m_nShineIntensity;
  int             m_nLuminosity;

  mutable ON_SimpleArray<int>   m_nTextureUnit;
  mutable int                   m_nEmapUnit;
  mutable int                   m_nBumpUnit;
  mutable int                   m_nTransUnit;
  
  void EnableMipMap(bool bEnable);
  bool MipMappingEnabled() const;

  void SetIsSelectionMapColor(bool b);
  bool IsSelectionMapColor() const;

  void SetIsBakedMaterial(bool b);
  bool IsBakedMaterial() const;

  //These perform a similar task to CRhRdkContent::DocumentAssoc - when
  //a material is being rendered in the null document scope (ie - the geometry source is nullptr)
  //these functions are there to let the material know which document they come from for the purposes
  //of looking for files and so on.
  void SetDocumentAssoc(const CRhinoDoc* pDoc);
  const CRhinoDoc* DocumentAssoc(void) const;

  void SetDocumentAssoc(unsigned int documentRuntimeSerialNumber);
  unsigned int  DocumentAssocRSN(void) const;

  ON_Color        m_selectShadeHighlight;
private:
  bool            m_bUseTextureIndirect;
  bool            m_bUseBumpIndirect;
  bool            m_bUseEmapIndirect;
  
  bool            m_bAllowMipMapping;

  bool            m_bIsSelectionMapColor;
  bool            m_bIsBakedMaterial;
  
  std::unique_ptr<class CDisplayAttributeMaterial_Private> m_pPrivate;

public:
  const CDisplayAttributeMaterial_Private& Private() const;
  CDisplayAttributeMaterial_Private& Private();

private:
  friend class CDisplayMaterialMgr;
  ON__UINT_PTR m_sdk_reserved = 0;
};


class RHINO_SDK_CLASS CDisplayPipelineMaterial
{
public:
  CDisplayPipelineMaterial();
  CDisplayPipelineMaterial(const ON_Material&);
  CDisplayPipelineMaterial(const CDisplayAttributeMaterial&);
  CDisplayPipelineMaterial(const CDisplayAttributeMaterial&, const CDisplayAttributeMaterial&);

  bool AppearanceEquals(const CDisplayPipelineMaterial& other) const;
  bool        operator==(const CDisplayPipelineMaterial&) const;
  void        Initialize(bool bUseBackMaterial=false);
  
  ON_SHA1_Hash ComputeHash() const;

public:
  bool                          m_bUseBackMaterial = false;
  bool                          m_bFrontIsCustom = false;
  bool                          m_bBackIsCustom = false;
  CDisplayAttributeMaterial     m_FrontMaterial;
  CDisplayAttributeMaterial     m_BackMaterial;
};
