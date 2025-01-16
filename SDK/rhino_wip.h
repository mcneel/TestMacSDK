//
// Copyright (c) 1993-2016 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#if !defined(RHINO_WIP_INC_)
#define RHINO_WIP_INC_

// Exactly one of RHINO_COMMERCIAL_BUILD or RHINO_WIP_BUILD must be defined
// before including opennurbs_wip.h.

#if !defined(RHINO_WIP_BUILD) && !defined(RHINO_COMMERCIAL_BUILD) && !defined(RHINO_BETA_BUILD)
    #define RHINO_WIP_BUILD
#endif

// Initial sanity check
#if defined(RHINO_WIP_BUILD) && defined(RHINO_COMMERCIAL_BUILD)
#error Both RHINO_COMMERCIAL_BUILD and RHINO_WIP_BUILD are defined. Exactly one should be defined.
#endif
#if defined(RHINO_WIP_BUILD) && defined(RHINO_BETA_BUILD)
#error Both RHINO_WIP_BUILD and RHINO_BETA_BUILD are defined. Exactly one should be defined.
#endif
#if defined(RHINO_COMMERCIAL_BUILD) && defined(RHINO_BETA_BUILD)
#error Both RHINO_COMMERCIAL_BUILD and RHINO_BETA_BUILD are defined. Exactly one should be defined.
#endif
#if !defined(RHINO_WIP_BUILD) && !defined(RHINO_BETA_BUILD) && !defined(RHINO_COMMERCIAL_BUILD)
#error Neither RHINO_COMMERCIAL_BUILD nor RHINO_WIP_BUILD nor RHINO_BETA_BUILD are defined. Exactly one should be defined.
#endif

// opennurbs_wip.h controls what objects are in .3dm files
// and what geometry classes are available and how they are 
// exported for C++ SDK use. 
//
// Keep in mind that Rhino WIP, commercial Rhino 6 for Windows,
// and commercial Rhino 6 for Mac must be able to read
// .3dm archives made in all product versions.
#include "opennurbs/opennurbs_wip.h"

#if defined(ON_RUNTIME_WIN)
  #if defined(RHINO_COMMERCIAL_BUILD)
    // Nothing
  #elif defined(RHINO_WIP_BUILD) || defined(RHINO_BETA_BUILD)
    #define RHINO_LICENSE_BETA
  #endif
#endif

#if defined(ON_RUNTIME_APPLE)

  // A build system may define RHINO_WIP_DISABLE_ALL to
  // disable all Rhino work in progress features.
  #if !defined(RHINO_WIP_DISABLE_ALL)
    //FilletEdge editing project
    #define RHINO_WIP_FILLETEDGE_EDITING
  #endif

//
//  rhino_wip.h
//
//  Created by Daniel Rigdon-Bel on 12/19/14.
// ------------------------------------------------------------------------
// Platform differentiation (example):
// RHINO_WIN
// RHINO_MAC
//
// WIP Tag (example):
// RHINO_WIN_WIP
// RHINO_MAC_WIP
//
// Feature Tag (example):
// RHINO_WIN_WIP_LAYOUTS
// RHINO_MAC_WIP_LAYOUTS
//
// Sub-feature Tag (example):
// RHINO_MAC_WIP_SUBD_LIB  = Objects for feature are supported by openNURBS
// RHINO_MAC_WIP_SUBD_OBJ  = CRhino objects for feature are present
// RHINO_MAC_WIP_SUBD_UI   = UI for feature is present in some form
// ------------------------------------------------------------------------

#if defined RHINO_COMMERCIAL_BUILD
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   RELEASE build symbols defined below                                      //
//   Commenting out a line removes the feature from the build                 //
//                                                                            //

#define RHINO_MAC_ENABLE_LICENSE_VALIDATION
#define RHINO_MAC_ENFORCE_LICENSE_VALIDATION

//                                                                            //
//   RELEASE build symbols defined above                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#elif defined(RHINO_WIP_BUILD) || defined(RHINO_BETA_BUILD)
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   WIP build symbols defined below                                          //
//   Commenting out a line removes the feature from the build                 //
//                                                                            //

// Mac Features
// #define RHINO_MAC_WIP_NEWINV5
// #define RHINO_MAC_WIP_ALERTER
// #define RHINO_MAC_WIP_FINDTEXT
// #define RHINO_MAC_LICENSE_PREFERENCES
#define RHINO_MAC_ENABLE_LICENSE_VALIDATION
#define RHINO_MAC_ENFORCE_LICENSE_VALIDATION
// #define RHINO_MAC_WIP_TOUCAN
#define RHINO_LICENSE_BETA
#define RHINO_MAC_WIP_FIXED_EXPIRE_DATE
//                                                                            //
//   WIP build symbols defined above                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#else  // Sanity Check
#error "RHINO_COMMERCIAL_BUILD or RHINO_WIP_BUILD or RHINO_BETA_BUILD must be defined"
#endif

#if defined RHINO_WIP_BUILD && defined RHINO_COMMERCIAL_BUILD
#error "RHINO_COMMERCIAL_BUILD AND RHINO_WIP_BUILD are both defined. Define only one."
#endif

#endif // end ON_RUNTIME_APPLE

#endif // end RHINO_WIP_INC_

// Final cross-platform sanity check
#if defined(RHINO_WIP_BUILD) && defined(RHINO_COMMERCIAL_BUILD)
#error Both RHINO_COMMERCIAL_BUILD and RHINO_WIP_BUILD are defined. Exactly one should be defined.
#endif
#if defined(RHINO_WIP_BUILD) && defined(RHINO_BETA_BUILD)
#error Both RHINO_WIP_BUILD and RHINO_BETA_BUILD are defined. Exactly one should be defined.
#endif
#if defined(RHINO_COMMERCIAL_BUILD) && defined(RHINO_BETA_BUILD)
#error Both RHINO_COMMERCIAL_BUILD and RHINO_BETA_BUILD are defined. Exactly one should be defined.
#endif
#if !defined(RHINO_BETA_BUILD) && !defined(RHINO_WIP_BUILD) && !defined(RHINO_COMMERCIAL_BUILD)
#error Neither RHINO_COMMERCIAL_BUILD nor RHINO_WIP_BUILD nor RHINO_BETA_BUILD are defined. Exactly one should be defined.
#endif


#if defined(RHINO_WIP_BUILD)
#if !defined(RH_EXPERIMENTAL)
// items that should be defined in WIP, but not in commercial build
#define INCLUDE_RHINO_FLAIR
#define INCLUDE_RHINO_FLAIR_UI
#define RH_CONSTRAINTS
#endif
#endif

#if defined(RHINO_BETA_BUILD)
#if !defined(RH_EXPERIMENTAL)
#define INCLUDE_RHINO_FLAIR
#endif
#endif

#if defined(OPENNURBS_SECTION_STYLE_TABLE_WIP)
#define INCLUDE_RHINO_SECTION_STYLE_TABLE
#endif
