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

#if !defined(OPENNURBS_PLUS) || !defined(OPENNURBS_PLUS_INC_) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if defined(OPENNURBS_INC_IN_PROGRESS)
// This file is used by 
// opennurbs_plus_rectpack1.cpp and opennurbs_plus_rectpack2.cpp.
// to declare comman helper classes and untility functions.
// It should not be included in opennurbs.h.
#error See the comment above.
#endif

#if !defined(ON_PLUS_RECTPACK2_INC_)
#define ON_PLUS_RECTPACK2_INC_

// do not export this function or declare it in a header file
bool ON__PackTextureRectangles1(
  ON_PackedTextureRectangle *top, 
  const class ON_PackTextureRectanglesParameters* packing_parameters
  );

// do not export this function or declare it in a header file
bool ON__PackTextureRectangles2(
  ON_PackedTextureRectangle *top, 
  const class ON_PackTextureRectanglesParameters* packing_parameters
  );

#if defined(OPENNURBS_DECLARE_RECTPACK_HELPERS)

// do not export this function or declare it in a header file
bool ON__PackTextureRectangles_CountValidInput(
  ON_PackedTextureRectangle *first_input_rect, 
  unsigned int* count 
  );

// do not export this function or declare it in a header file
bool ON__PackTextureRectangles_PackOne(
  ON_PackedTextureRectangle& r,
  const class ON_PackTextureRectanglesParameters* packing_parameters
  );

class ON__PackedTextureInternalRect
{
public:
  // input rect
  const ON_PackedTextureRectangle *input_rect; // leaf nodes have a non-null input_rect

  //inputs:
	ON__PackedTextureInternalRect *nxt; // linked list

  // input rectangle
	double width;
	double height;

  //reserved:
  int   reserved0;

  //outputs:

  // rotated = 0 if "x" is the "height" direction.
  // rotated = 1 if "x" is the "width" direction.
	int rotated;

  // (x0,y0) and (x1,y1) will be set to the corners
  // of a rectangle containing the input rectangle.
  // After the packing is calculated,
  // (rotated?height:width) <= x1-x0
  // and (rotated?width:height) <= y1-y0
	double x0,y0,x1,y1;
  
  // total unused area
  double empty_area;

  //internal:
  // DPPackLogarithmic() builds a binary tree of rects
  // with the input rects as leaves.  The "left"
  // and "right" pointers are used to create 
  // and traverse the binary tree.
	ON__PackedTextureInternalRect *left;
	ON__PackedTextureInternalRect *right;

  // internal: 
  // The smaller and bigger pointers are used by
  // DPTreeSort() and its helpers.
  // (DPPackLogarithmic() sorts while using
  // the "left" and "right" pointers for its binary
  // tree, so DPTreeSort cannot use the left and right
  // pointers above.)
  ON__PackedTextureInternalRect *smaller;
  ON__PackedTextureInternalRect *bigger;
};

ON__PackedTextureInternalRect *ON__PackedTextureCopyInputToInternal(
  const ON_PackedTextureRectangle *first_input_rect, 
  ON_FixedSizePool& internal_rect_fsp 
  );

bool ON__PackedTextureCopyBack(
  ON__PackedTextureInternalRect *source,
  double tx0,
  double ty0,
  double tx1,
  double ty1,
  int parent_rect_rotated
  );

#endif

#endif

