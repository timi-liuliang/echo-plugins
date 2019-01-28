/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Interleave.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Functions to do quick interleaving and deinterleaving of 1-4 component
 *	vector arrays. (a vector of 1 component is a degenerate case; however,
 *      since this class also does strides and components steps, it can be used
 *	for other purposes).
 */
#ifndef UT_Interleave_h
#define UT_Interleave_h

#include "UT_API.h"

// Typesize        The number of bytes per element. The array should be aligned
//		   to at least that size (ie, for floats, the array must be at
//		   least word aligned). It may be 1,2,4 or 8.

// Vectorsize	   The size of the destination vector. Not all the sources need
//		   to be present (non-NULL) to meet the vectorsize (however, at
//                 least one must be in src[1,vectorsize]).

// Width/Height    The size of the array. For 1D arrays, leave height at 1.

// Dest/Src Stride Used only when height is not 1, this specifies the number of
//		   bytes per scanline if the scanlines are padded. This must be
//		   evenly divisible by 'typesize'

// Thread	   Enables threading of the op. Will still only thread if the 
//		   total size is big enough to overcome the thread cost.


// INTERLEAVE
// interleave flat arrays src[N] into dest.
UT_API void UTinterleave(void *dest,
			 const void *src1,
			 const void *src2,
			 const void *src3,
			 const void *src4,
			 int typesize,
			 int vectorsize,
			 int width,
			 int height = 1,
			 int deststride = 0,
			 bool thread = true);

// Similar to the above, but the dest and source arrays don't have to be flat.
// They can have a larger jump (dinc,sinc) between elements. The jumps are in
// elements, not bytes.
UT_API void UTinterleave(void *dest, int dinc,
			 const void *src1, int sinc1,
			 const void *src2, int sinc2,
			 const void *src3, int sinc3,
			 const void *src4, int sinc4,
			 int typesize,
			 int vectorsize,
			 int width,
			 int height = 1,
			 int deststride = 0,
			 bool thread = true);

// Assuming data is deinterleaved(rrr..rgggg...gbbbb...b), interleave it.
// For 2D arrays, if by_scanline is true, only deinterleave per scanline.
// Otherwise, completely separate the 2D arrays. 
UT_API void UTinterleaveSelf(void *data,
			     int typesize,
			     int vectorsize,
			     int width,
			     int height = 1,
			     bool by_scanline = true,
			     bool thread = true);


// DEINTERLEAVE
// Deinterleaving does the opposite - it extracts vector components out of 'src'
// and into flat arrays dest[N]. 
UT_API void UTdeinterleave(void *dest1,
			   void *dest2,
			   void *dest3,
			   void *dest4,
			   const void *src,
			   int typesize,
			   int vectorsize,
			   int width,
			   int height = 1,
			   int srcstride = 0,
			   bool thread = true);

UT_API void UTdeinterleave(void *dest1, int dinc1,
			   void *dest2, int dinc2,
			   void *dest3, int dinc3,
			   void *dest4, int dinc4,
			   const void *src, int sinc,
			   int typesize,
			   int vectorsize,
			   int width,
			   int height = 1,
			   int srcstride = 0,
			   bool thread = true);

// assume data is interleaved, write it back deinterleaved to data.
// For 2D arrays, if by_scanline is true, only scanlines are deinterleaved.
// Otherwise, the full 2D arrays are considered separate.
UT_API void UTdeinterleaveSelf(void *data,
			       int typesize,
			       int vectorsize,
			       int width,
			       int height = 1,
			       bool by_scanline = true,
			       bool thread = true);


#endif


