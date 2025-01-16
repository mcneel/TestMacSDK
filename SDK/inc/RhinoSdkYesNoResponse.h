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


// Deep, sigh. ... This file is included by Mac Rhino projects
// in a complicated, contrived, controversial, and complex, manner.  
// In MacRhino, in some projects, the enum and functions are
// declared at different points in time. In other projects, 
// everything simply includes RhinoSdk.h and life is, well, 
// simple. 
//
// In all Windows code, include RhinoSdk.h and move on. "There is nothing to see here."
//
// Too keep Mac Rhino building.
//
// NO NO NO NO #prag once

#if !defined(RhinoYesNoResponse_ENUM_INC_)
#define RhinoYesNoResponse_ENUM_INC_

// define RhinoYesNoResponse enum

#pragma region RH_C_SHARED_ENUM [RhinoYesNoResponse] [Rhino.RhinoYesNoResponse] [int]
/// <summary>
/// RhinoYesNoResponse is a list of possible responses to a question that can be answered yes or no.
/// </summary>
enum class RhinoYesNoResponse : unsigned int
{
  /// <summary>
  /// Response is unset.
  /// </summary>
  Unset = 0,

  /// <summary>
  /// User responded yes.
  /// </summary>
  Yes = 1,

  /// <summary>
  /// User responded yes and wants to automatically respond yes to all identical queries in the future.
  /// </summary>
  YesToAll = 2,

  /// <summary>
  /// User responded no.
  /// </summary>
  No = 3,

  /// <summary>
  /// User responded no and wants to automatically respond no to all identical queries in the future.
  /// </summary>
  NoToAll = 4,

  /// <summary>
  /// User selected cancel in a dialog or other interface.
  /// </summary>
  Cancel = 5,

  /// <summary>
  /// Response is not available. Possibly because an error occurred in code execution.
  /// </summary>
  Error = 86
};
#pragma endregion

#endif // RhinoYesNoResponse_ENUM_INC_

#if defined(RHINO_SDK_H_IN_PROGRESS_)

#if !defined(RhinoYesNoResponse_FUNC_INC_)
#define RhinoYesNoResponse_FUNC_INC_

// declare functions that use the RhinoYesNoResponse enum

/*
Returns:
  True if yes_no_response is RhinoYesNoResponse::Yes or RhinoYesNoResponse::YesToAll.
  False otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoResponseIsYesOrYesToAll(
  RhinoYesNoResponse response
);

/*
Returns:
  True if yes_no_response is RhinoYesNoResponse::No or RhinoYesNoResponse::NoToAll.
  False otherwise.
*/
RHINO_SDK_FUNCTION
bool RhinoResponseIsNoOrNoToAll(
  RhinoYesNoResponse response
);

RHINO_SDK_FUNCTION
RhinoYesNoResponse RhinoYesNoResponseFromUnsigned(
  unsigned int response_as_unsigned
);

#endif // RhinoYesNoResponse_FUNC_INC_

#endif //RHINO_SDK_H_IN_PROGRESS_


