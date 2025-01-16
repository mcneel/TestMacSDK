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
  Worksessions lets more than one user work on a large project.
  By breaking the project down into many files, one user can edit
  a part of the project while another user edits a different file.
  Only one user can have a file open for editing, but many users
  can see it.
*/
class RHINO_SDK_CLASS CRhinoWorkSession
{
public:

  /*
  Description:
    Returns a worksession's runtime serial number given a document's runtime serial number.
  Parameters:
    doc_serialno [in] - The document's runtime serial number.
  Return:
    > 0 if successful.
    0 if not successful.
  */
  static unsigned int GetWorksessionFromDocument(unsigned int doc_serialno);

  /*
  Description:
    Returns the serial numbers of the active worksessions. 
  Parameters:
    ws_serialnos [out] - The serial numbers of the active worksessions.
  Return: 
    The number of serial numbers added to the output array.
    -1 on error.
  */
  ON_DEPRECATED_MSG("use CRhinoWorkSession::GetWorksessionFromDocument")
  static int GetActiveWorksessionRuntimeSerialNumbers(ON_SimpleArray<unsigned int>& ws_serialnos);

  /*
  Description:
    Returns the path to the open worksession .rws file. 
    If false is returned, then either there is no worksession .rws file open,
    or the active worksession has not yet been saved.
  Parameters:
    ws_serialno  [in]  - The worksession's runtime serial number.
    ws_file_name [out] - The path to the open worksession .rws file.
  Return: 
    True if successful.
    False if not successful.
  */
  static bool FileName(unsigned int ws_serialno, ON_wString& ws_file_name);

  /*
  Description:
    Returns the number of models in the worksession, including the active model.
  Parameters:
    ws_serialno [in] - The worksession's runtime serial number.
  Return: 
    >= 0 if successful.
    -1 on error.
  */
  static int ModelCount( unsigned int ws_serialno );

  /*
  Description:
    Returns the paths to the models used by the worksession.
  Parameters:
    ws_serialno    [in]  - The worksession's runtime serial number.
    ws_model_names [out] - The paths to the models.
  Return: 
    The number of model names added to the output array if successful.
    -1 on error.
  */
  static int ModelNames(unsigned int ws_serialno, ON_ClassArray<ON_wString>& ws_model_names);

  /*
  Description:
    Returns the aliases of the models used by the worksession,
    including the active model which might not be saved.
  Parameters:
    ws_serialno      [in]  - The worksession's runtime serial number.
    ws_model_aliases [out] - The aliases of the models.
  Return: 
    The number of model aliases added to the output array if successful.
    -1 on error.
  */
  static int ModelAliases(unsigned int ws_serialno, ON_ClassArray<ON_wString>& ws_model_aliases);

  /*
  Description:
    Returns the path to a model used by the worksession, given the reference serial number.
  Parameters:
    ws_serialno    [in]  - The worksession's runtime serial number.
    model_serialno [in]  - The reference model's runtime serial number.
    model_name     [out] - The path to the model.
  Return:
    True if successful.
    False if not successful.
  */
  static bool ModelNameFromSerialNumber(unsigned int doc_serialno, unsigned int model_serialno, ON_wString& model_name);
};
 
