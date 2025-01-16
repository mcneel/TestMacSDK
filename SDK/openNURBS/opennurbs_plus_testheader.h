//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////
#if !defined(OPENNURBS_TESTHEADER_INC_)
#define OPENNURBS_TESTHEADER_INC_

/* WARNING: DO NOT INCLUDE THIS FILE WITH THE DISTRIBUTION OF OPENNURBS OR IN THE RHINO SDK */

/* PURPOSE: this file is used to expose functions that are normally not part of the
            API of opennurbs, with the purpose of being able to unit tests such
            functions as part of the build.

   USAGE:   Include the function signatures prepended with ON_DECL to export the
            function in the opennurbs library. This will allow the tests to call
            the function directly.

   DEFINES: This file defines the symbol ON_TESTS_BUILD which you can use to fine
            tune your code.

*/

#define ON_TESTS_BUILD

#endif
