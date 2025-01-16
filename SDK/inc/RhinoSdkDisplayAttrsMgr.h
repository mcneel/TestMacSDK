/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

/////////////////////////////////////////////////////////////////////////////
//
// File: RhinoDisplayAttrsMgr.h   -- Interface file
//
/////////////////////////////////////////////////////////////////////////////

class RHINO_SDK_CLASS DisplayAttrsMgrListDesc
{
public:
            DisplayAttrsMgrListDesc();
            DisplayAttrsMgrListDesc(const  CDisplayPipelineAttributes&,
                                    bool   bSupportsShadeCmd, 
                                    bool   bSupportsShading, 
                                    bool   bAddToMenu);
            DisplayAttrsMgrListDesc(const DisplayAttrsMgrListDesc&);
            
  virtual  ~DisplayAttrsMgrListDesc();                                    
                                
public:
  bool                          m_bSupportsShadeCmd;
  bool                          m_bSupportsShading;
  bool                          m_bAddToMenu;
  bool                          m_bAllowObjectAssignment;
  
  bool                          m_bShadedPipelineRequired;
  bool                          m_bWireframePipelineRequired;
  bool                          m_bPipelineLocked;
  bool                          m_bSupportsStereo;

  bool                          m_bProfilePersistent;
  
  CDisplayPipelineAttributes*   m_pAttrs;
  
  bool       GetBuiltInDefaults(DisplayAttrsMgrListDesc&);
  

private:
  BYTE        m_pad01[128];

public:
  int                           Order(void) const { return m_nSortId; };
  const ON_UUID&                DerivedFromId(void) const { return m_DerivedFrom; }

  DisplayAttrsMgrListDesc&      operator=(const DisplayAttrsMgrListDesc&);
  
  bool                          operator==(const DisplayAttrsMgrListDesc&) const;
  
  bool                          ReplaceAttributes(const ON_UUID& uuidAttributeClassId, bool bCopyExisting=false);
  
  bool SaveProfile( const wchar_t* lpsSection, CRhinoProfileContext& ) const; // save in registry
  void LoadProfile(const wchar_t* lpsSection, CRhinoProfileContext&, const DisplayAttrsMgrListDesc& defaults); // load from registry
  void LoadProfile(const wchar_t* lpsSection, CRhinoProfileContext&); // load from registry

private:
  // maintenance fields...
  int         m_nSortId;
  bool        m_bDeleted;
  bool        m_bBuiltIn;
  bool        m_bNewlyAdded;
  ON_UUID     m_DerivedFrom;

  BYTE        m_pad02[110];
public:
  static bool BuiltInModeDefaults(int mode, DisplayAttrsMgrListDesc& dal);
private:
  friend class CRhinoDisplayAttrsMgr;
  friend class DisplayAttrsMgrList;
  friend class COptionsAdvancedDisplay;
};


class RHINO_SDK_CLASS DisplayAttrsMgrList : public ON_ClassArray<DisplayAttrsMgrListDesc>
{
public: 
       DisplayAttrsMgrList();
       
  void SaveProfile( const wchar_t* lpsSection, CRhinoProfileContext& ) const; // save in registry
  // LoadProfile will zero the array out and build a new list with all the keys under lpsSection
  // whos name can be converted to a valid UUID.
  bool LoadProfile( const wchar_t* lpsSection, CRhinoProfileContext& ); // load from registry
};

class RHINO_SDK_CLASS CRhinoDisplayAttrsMgr
{
public:
  virtual  ~CRhinoDisplayAttrsMgr(void);

public:
  static int   m_nMajor;
  static int   m_nMinor;

public:  
  static const CDisplayPipelineAttributes*   StdWireframeAttrs(void);
  static const CDisplayPipelineAttributes*   StdShadedAttrs(void);
  static const CDisplayPipelineAttributes*   StdRenderedAttrs(void);
  static const CDisplayPipelineAttributes*   StdRenderedShadowsAttrs(void);
  static const CDisplayPipelineAttributes*   StdGhostedAttrs(void);
  static const CDisplayPipelineAttributes*   StdXrayAttrs(void);
  static const CDisplayPipelineAttributes*   StdTechAttrs(void);
  static const CDisplayPipelineAttributes*   StdArtisticAttrs(void);
  static const CDisplayPipelineAttributes*   StdPenAttrs(void);
  static const CDisplayPipelineAttributes*   StdMonochromeAttrs(void);
  static const CDisplayPipelineAttributes*   StdArcticAttrs(void);
  static const CDisplayPipelineAttributes*   StdRaytracedAttrs(void);
 
  ON_DEPRECATED_MSG("CRhinoDisplayAttrsMgr::IntOGLWireframeAttrs is obsolete, please remove all references. Use CRhinoDisplayAttrsMgr::StdWireframeAttrs instead.")
  static const CDisplayPipelineAttributes*   IntOGLWireframeAttrs(void);
  ON_DEPRECATED_MSG("CRhinoDisplayAttrsMgr::IntGDIWireframeAttrs is obsolete, please remove all references. Use CRhinoDisplayAttrsMgr::StdWireframeAttrs instead.")
  static const CDisplayPipelineAttributes*   IntGDIWireframeAttrs(void);
  ON_DEPRECATED_MSG("CRhinoDisplayAttrsMgr::IntShadedAttrs is obsolete, please remove all references. Use CRhinoDisplayAttrsMgr::StdShadedAttrs instead.")
  static const CDisplayPipelineAttributes*   IntShadedAttrs(void);

  static const ON_UUID                       WireframeModeId();
  static const ON_UUID                       ShadedModeId();
  static const ON_UUID                       RenderedModeId();
  static const ON_UUID                       RenderedShadowsModeId();
  static const ON_UUID                       GhostedModeId();
  static const ON_UUID                       XRayModeId();
  static const ON_UUID                       TechModeId();
  static const ON_UUID                       ArtisticModeId();
  static const ON_UUID                       PenModeId();
  static const ON_UUID                       MonochromeModeId();
  static const ON_UUID                       AmbientOcclusionModeId();
  static const ON_UUID                       RaytracedModeId();

  static const CDisplayPipelineAttributes*   FindDisplayAttrs(const ON_UUID&);
  static DisplayAttrsMgrListDesc*      FindDisplayAttrsDesc(const ON_UUID&);
  
  static bool   AddDisplayAttrs(const  CDisplayPipelineAttributes&,
                                bool   bSupportsShadeCmd, 
                                bool   bSupportsShading, 
                                bool   bAddToMenu);
  
  static DisplayAttrsMgrListDesc*    AppendNewEntry(void);
  static DisplayAttrsMgrListDesc*    AppendNewEntry(bool  bSupportsShadeCmd, 
                                                    bool  bSupportsShading,
                                                    bool  bAddToMenu);
  static DisplayAttrsMgrListDesc*    CreateNewAttrs(const ON_wString& Name, const ON_UUID& DerivedFrom);
  static DisplayAttrsMgrListDesc*    CreateDuplicateAttrs(const DisplayAttrsMgrListDesc&);

  static bool  RemoveEntry(const ON_UUID&);
  
  static int   GetDisplayAttrsList(DisplayAttrsMgrList&);
  static int   GetShadedDisplayAttrsList(DisplayAttrsMgrList&);
  
  static void  UpdateReferencedPipelines(void);
  
  static bool  IsModified(void);
  static void  SetIsModified(bool);

  static void LoadProfile( CRhinoProfileContext& ); // load from registry
  static void SaveProfile( CRhinoProfileContext& ); // save in registry
  static const wchar_t* ProfileSection();

  static bool UpdateAttributes(const DisplayAttrsMgrListDesc&);
  
  ON_DEPRECATED_MSG("CRhinoDisplayAttrsMgr::IsInternalAttributes is obsolete, please remove all references.")
  static bool IsInternalAttributes(const CDisplayPipelineAttributes*);
  static bool IsBuiltInAttributes(const CDisplayPipelineAttributes*);
  static bool IsTechnicalMode(const CDisplayPipelineAttributes*);

  static void FormatValidDisplayName(ON_wString& sName);

  static double GetWireZBias(void);
  static void   SetWireZBias(double);

  static DisplayAttrsMgrListDesc* AddNewEntry(const wchar_t* local_name);
  static DisplayAttrsMgrListDesc* CopyExistingEntry(ON_UUID id, const wchar_t* local_name);
  static DisplayAttrsMgrListDesc* FindDisplayAttrsDesc(const wchar_t* local_name);

private:
  CRhinoDisplayAttrsMgr(void) = default;
  
  static bool  m_bIsModified;             
  static void  Destroy(void);
  
  static CDisplayPipelineAttributes* FindBuiltInMode(int Mode);

  static DisplayAttrsMgrList   m_DisplayAttrs;

  static CRhinoDisplayAttrsMgr __TheDisplayAttrsMgr;
};
