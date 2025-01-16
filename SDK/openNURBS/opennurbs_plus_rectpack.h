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

#if !defined(ON_PLUS_RECTPACK_INC_)
#define ON_PLUS_RECTPACK_INC_


class ON_CLASS ON_PackedTextureRectangle
{
public:

  // The default constructor sets all fields to zero.
  ON_PackedTextureRectangle();

  // The default operator= and copy constructor work fine.
  // Note that the m_next field is copied and you must adjust it
  // as needed after making the copy.

public:

  // The input set of rectangles is managed as a linked list.
  // Note that the default copy constructor and operator= 
  // copy the m_next value. If you make a copy, be sure to
  // adjust the value m_next to something appropriate.
  class ON_PackedTextureRectangle *m_next;

  /////////////////////////////////////////////////////////////
  //
	// INPUT:
  //
  ON__INT_PTR m_id; //for later identification
	
  /////////////////////////////////////////////////////////////
  //
	// INPUT RECTANGLE:
  //
  // The input rectangle size is typically in world units,
  // but can be in any unit system.
  double m_width;	  // rectangle width
	double m_height;	// rectangle height

private:
  /////////////////////////////////////////////////////////////
  //
	// Reserved for future use:
  //
  int   m_reserved0;
  short m_reserved1;
  char  m_reserved2;

public:
  /////////////////////////////////////////////////////////////
  //
	// OUTPUT:
  //

  /*
  Description:
    Sets the m_tex_* output fields to use the entire
    normalized texture.
      m_rotate = 0;
      m_tex_x0 = 0.0;
      m_tex_y0 = 0.0;
      m_tex_x1 = 1.0;
      m_tex_y1 = 1.0;
      m_tex_dx = 1.0;
      m_tex_dy = 1.0;
  */
  void UseEntireTexture();

  // tex_rotated will be set to 0 or 1
  // 0 = not rotated
  //   The "width" direction of the rect corresponds to the 
  //   "x" direction of the normalized texture coordinates
  //   and the "height" direction of the rect corresponds to 
  //   the "y" direction of the normalized texture coordinates.
  // 1 = rotated
  //   The "width" direction of the rect corresponds to the 
  //   "y" direction of the normalized texture coordinates
  //   and the "height" direction of the rect corresponds to 
  //   the "x" direction of the normalized texture coordinates.
	char m_tex_rotated;

  // The normalized texture coordinate region this rectangle
  // can use has lower left corner at (m_tex_x0, m_tex_x1) and
  // upper right corner at (m_tex_y0, m_tex_y1)
	double m_tex_x0,m_tex_y0; // lower left normalized texture coordinate
  double m_tex_x1,m_tex_y1; // upper right normalized texture coordinate

  // If you want to preserve the aspect of the input rectangle
  // and have a uniform normalized texture coordinate scale
  // for all the rectangles in the packing, limit yourself to
  // using a normalized texture coordinate rectangle of size
  // m_tex_dx by m_tex_dy inside the texture coordinate region
  // specified by m_tex_x0,m_tex_y0 m_tex_x1,m_tex_y1.
  // (m_tex_dx <= m_tex_x1 - m_tex_x0)
  // (m_tex_dy <= m_tex_y1 - m_tex_y0)
  double m_tex_dx, m_tex_dy; 
};


class ON_CLASS ON_PackTextureRectanglesParameters
{
public:
  // The default constructor sets all fields to zero.
  ON_PackTextureRectanglesParameters();

  // The default operator= and copy constructor work fine.

public:
  bool IsValid( class ON_TextLog* text_log ) const;

public:
  // The m_method parameter specifies how the rectangles
  // are packed.  Generally, 0 is the best choice. The
  // other options are available so legacy packings can
  // be recalculated.
  //
  // 0 = latest
  // 1 = V4, V5 SR0, V5 SR1 
  // 2 = V5 SR2
  unsigned int m_method;

private:
  unsigned char m_reserved1[2];

public:
  // If m_bPreserveRectangleAspect is true, the aspect
  // of rectangle assigned to the rect will be the
  // same as the aspect of the original rectangle.
  bool m_bPreserveRectangleAspect;

  // If m_bm_width > 0 and m_bm_height > 0, then they
  // specify the size of the bitmap in pixels that the
  // packed texture will use.
  double m_bm_width;
  double m_bm_height;

private:
  unsigned char m_reserved2[64];
};

/*
Description:
  Pack a collection of rectangles into a rectangle.
Parameters:
  rectangle_list - [in]
    a linked list of rectangles defined by the m_next pointers.
    The last item in the list must have a null m_next pointer.

    or

  rectangle_count - [in]
  rectangle_array - [in]
    an array of rectangle_count rectangles to pack.
    The m_next pointers will be set so the array is a linked list.

    or

  rectangle_array - [in]
    A simple dynamic array of rectangles.  The m_next pointers
    will be set so the array is a linked list.

*/
ON_DECL
bool ON_PackTextureRectangles(
  class ON_PackedTextureRectangle *rectangle_list, 
  const class ON_PackTextureRectanglesParameters* packing_parameters
  );

ON_DECL
bool ON_PackTextureRectangles(
  size_t rectangle_count,
  class ON_PackedTextureRectangle* rectangle_array, 
  const class ON_PackTextureRectanglesParameters* packing_parameters
  );

ON_DECL
bool ON_PackTextureRectangles(
  ON_SimpleArray< ON_PackedTextureRectangle >& rectangle_array, 
  const class ON_PackTextureRectanglesParameters* packing_parameters
  );


#endif
