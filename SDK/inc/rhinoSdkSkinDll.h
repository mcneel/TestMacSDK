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

class RHINO_SDK_CLASS CRhinoSkinDLL
{
public:
  //
  // Description:
  //    CRhinoSkinDLL constructor
  //
  CRhinoSkinDLL();
  //
  // Description:
  //    CRhinoSkinDLL destructor
  //
  virtual ~CRhinoSkinDLL();
  /////////////////////////////////////////////////////////////////////////////
  //
  // Begin - Required overrides
  //
  //
  // Description:
  //    This method is called by Rhino to get the skin icon to be associated
  //    with the Rhino main frame and dialog boxes.
  //
  // Returns:
  //    The Skin DLL is responsible for loading and deleting the HICON.  Make
  //    sure you use the AFX_MANAGE_STATE( AfxGetStaticModuleState()) prior
  //    to loading the icon.
  //
  // See Also:
  //    AFX_MANAGE_STATE In Microsoft platform SDK help
  //
  virtual HICON MainRhinoIcon() = 0;
  //
  // Description:
  //    String to be placed in the main frame title bar and message boxes displayed
  //    by Rhino.
  //
  // Returns:
  //    String to be placed in the main frame title bar and message boxes displayed
  //    by Rhino.
  //
  virtual const wchar_t* ApplicationName() = 0;
  //
  // Description:
  //    A skin DLL is required to provide a valid Rhino plug-in UUID.  This plug-in
  //    will be used to manage the MainRhinoMenu() and proides a place for the developer
  //    to add commands to Rhino.
  //
  // Returns:
  //    Returns ID of plug-in associated with this skin.
  //
  // See Also:
  //    CRhinoSkinDLL::MainRhinoMenu()
  //
  virtual UUID SkinPlugInID() = 0;
  //
  // Description:
  //    This method is called when Rhino is loading.  Use this method to replace
  //    the Rhino splash screen with the skin version.  The skin is responsible for
  //    destroying the splash window when this method is called with bShow=false.
  //
  // Parameters:
  //    bShow [in] If true then display the skin splash screen if it is not already displayed
  //               otherwise destroy the splash window.
  //
  virtual void ShowSplash( bool bShow) = 0;
  //
  // End- Required overrides
  //
  /////////////////////////////////////////////////////////////////////////////
  //
  // Optional overrides
  //
  //
  // Description:
  //    If this method returns a valid HMENU then it is used to replace the menu associated
  //    with the Rhino main frame window.  If this method returns NULL then the default
  //    Rhino menu is used.  Make sure you use the AFX_MANAGE_STATE( AfxGetStaticModuleState())
  //    prior to loading the skin menu.
  //
  // Returns:
  //    If this method returns a valid HMENU then it is used to replace the menu associated
  //    with the Rhino main frame window.  If this method returns NULL then the default
  //    Rhino menu is used.
  //
  // See Also:
  //    AFX_MANAGE_STATE In Microsoft platform SDK help
  //
  virtual HMENU MainRhinoMenu();
protected:
  CRhinoSkinDLL( const CRhinoSkinDLL&); // Not implemented, do not use
  const CRhinoSkinDLL& operator= ( const CRhinoSkinDLL&); // Not implemented, do not use
};


class RHINO_SDK_CLASS CRhinoSkinDLLEx : public CRhinoSkinDLL
{
public:
  CRhinoSkinDLLEx();
  // 1000 = License manager check complete
  // 2000 = MainWindowCreated
  // 3000 = Builtin Commands Registered
  // 4000 = Begin Load Plug-In
  // 5000 = End Load Plug-In
  virtual void OnInitIntanceEvent(int event_id, const wchar_t* description) = 0;
  virtual void OnBeginLoadPlugins(int expected_count) = 0;
  // Default implementation calls ShowSplash(true)
  virtual void OnRhinoAbout();
protected:
  CRhinoSkinDLLEx( const CRhinoSkinDLLEx&); // Not implemented, do not use
  const CRhinoSkinDLLEx& operator= ( const CRhinoSkinDLLEx&); // Not implemented, do not use
};
