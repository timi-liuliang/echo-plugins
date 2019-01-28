/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Filter.h ( UT Library, C++)
 *
 * COMMENTS:	enum for filter types
 */

#ifndef __UT_FilterType__
#define __UT_FilterType__

// #define UT_ALL_FILTERS

typedef enum {
	UT_FILTER_POINT,	// Point filter must be first (for bkwd compat)
	UT_FILTER_BOX,		// Box filter must be second (for bkwd compat)

	UT_FILTER_GAUSS,
	UT_FILTER_CONE,		// Also known as a Bartlett
	UT_FILTER_SINC,

	UT_FILTER_CATROM,

	UT_FILTER_HANNING,
	UT_FILTER_BLACKMAN,

	UT_FILTER_MITCHELL,

#ifdef UT_ALL_FILTERS
	UT_FILTER_CUBIC,
	UT_FILTER_BESSEL,
	UT_FILTER_HAMMING,
#endif

	UT_FILTER_MAX_FILTERS
} UT_FilterType;

typedef enum {
    UT_WRAP_REPEAT = 0,	// Texture repeats
    UT_WRAP_CLAMP = 1,	// Texture coords are clamped
    UT_WRAP_BORDER = 2	// Texture is clamped with border color
} UT_FilterWrap;

#endif
