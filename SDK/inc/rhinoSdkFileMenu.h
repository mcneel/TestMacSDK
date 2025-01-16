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

#include "rhinoSdkFileOptions.h"

class RHINO_SDK_CLASS CRhinoFileMenu  
{
public:
	CRhinoFileMenu();
	virtual ~CRhinoFileMenu();

protected:
  friend class CRhinoApp;
  friend class CMainFrame;

  void Start(bool bUseDialogs);
  bool Finish(bool result);
  void Defaults();

  /*
  Description:
    Create a new document and read a file into that new document.
  Parameters:
    file_name_to_read - [in]
      name of file to read. 
      If null, the user is asked to select a file.
    bUseDialogs - [in]
      true if user dialogs can be used.  Otherwise a predictable
      scripting interface is used.
    bCreateEmptyDocOnFailure - [in]
      true if you want to create an empty document if the
      file readding fails.
    file_name_read - [out]
      If not null, the name of the file that is actually read
      is returned here.
  Returns:
    Runtime serial number of the new document or 0 if fails.
  */
  unsigned int OpenFile(
    const wchar_t* file_name_to_read,
    bool bUseDialogs, 
    bool bCreateEmptyDocOnFailure,
    ON_wString* file_name_read
    );

  /*
  Description:
    Import the contents of a file into the specified document.
  Parameters:
    rhino_doc_sn - [in]
      document to import into
    file_name_to_read - [in]
      name of file to read. 
      If null, the user is asked to select a file.
    bUseDialogs - [in]
      true if user dialogs can be used.  Otherwise a predictable
      scripting interface is used.
    file_name_read - [out]
      If not null, the name of the file that is actually read
      is returned here.
  Returns:
    True if the file was successfully read.
  */
  bool ImportFile(
    unsigned int rhino_doc_sn,
    const wchar_t* file_name_to_read,
    bool bUseDialogs, 
    bool bSelectNewObjects,
    ON_wString* file_name_read
    );

  unsigned int FileNew( bool bUseDialogs);
  bool OnOpenRecentFile(UINT nID);

  bool FileReference(ON_wString& wFileName, bool bUseDialogs, HWND hWndParent = NULL);

  CRhinoFileReadOptions  m_SaveFileReadOptions;
};

