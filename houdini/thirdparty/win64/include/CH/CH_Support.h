/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel Library (C++)
 *
 * COMMENTS:	Support glue between channel and expression libraries
 *
*/

#ifndef __CH_Support_h__
#define __CH_Support_h__

#include "CH_API.h"
#include <SYS/SYS_Types.h>

//
//  Some defines for expressions.  These defines may be used by anyone
//	who needs them.  They are the lower bits in the EV_Expression
//	flag and are accessible by the method for getting flags in the
//	CH_Expression class. Some (CH_SYM...) are flags for the EV_SYMBOL,
//	which are or'ed into the expression flags and so must use the
//	same bit-range.
//

#define CH_EXPRTIME	 0x00008000	// Expression uses time
#define CH_EXPRCHAN	 0x00010000	// Expression uses channel
#define CH_EXPRIVALUE	 0x00020000	// Expression uses in value
#define CH_EXPROVALUE	 0x00040000	// Expression uses out value
#define CH_EXPRSLOPE	 0x00080000	// Expression uses in slope/out slope
//#define CH_EXPRPARAM	 0x00100000	// Now Unused, free for use
#define CH_EXPRACCEL	 0x00200000	// Expression uses in accel/out accel
#define CH_EXPRKNOTS	 0x00400000	// Expression uses spline knots
#define CH_EXPRMATCH	 0x00800000	// Expression has a match() function
//#define CH_SYMHARDWIRED 0x01000000	// Now unused, free for use
#define CH_EXPRDATA      0x02000000	// Expression sets a data interest
#define CH_EXPRVALUE	 (CH_EXPRIVALUE | CH_EXPROVALUE)

// Flags above 0x00008000 are reserved for the channel library
// Flags above 0x02000000 are reserved by the expression library
//	Applications may use any flags between 0x0001 and 0x4000
//	for their own custom flags

//
// It is possible to define constants for the user to use.  For each
//	define, there is a name for the label and a corresponding value.
//	for example:  "XMIN", 0.  This would allow the user to type
//	XMIN instead of having to remember that XMIN is always 0.

class CH_API CH_ExprDefine {
public:
    const char	*label;
    fpreal	 value;
};

class CH_SegmentValues;
class CH_Segment;
class CH_Cubic;

class CH_API CH_Bezier
{
public:
    fpreal	x[4];
    fpreal	y[4];

    CH_Bezier() {}

    CH_Bezier( CH_SegmentValues const& sv ) { init( sv ); }

    CH_Bezier( CH_Segment const& seg );
    CH_Bezier( CH_Segment const& seg, fpreal vscale, fpreal voff );

    void	init( CH_SegmentValues const& sv );

    void	getValues( CH_SegmentValues &v );

    static void	getCubicFromCVs( fpreal x[4], CH_Cubic &c );

    static void	splitInternal( fpreal t, fpreal xin[4],
				    fpreal xout1[4], fpreal xout2[4] );

    void	splitP( fpreal u, CH_Bezier &a, CH_Bezier &b );

    void	split( fpreal t, CH_Bezier &a, CH_Bezier &b );

    void	splitAndFindSlopeLengths( fpreal t,
		    fpreal &left_in, fpreal &left_out,
		    fpreal &right_in, fpreal &right_out );

    fpreal	timeToParametric( fpreal t );

    fpreal	evalP( fpreal u );

    fpreal 	eval( fpreal t );	// t is local

    void	display();
};

class CH_API CH_Cubic
{
public:
    fpreal	t0, t1;
    fpreal	x0, x1, x2, x3;

    fpreal	timeToParametric( fpreal t );	// t -> u

    fpreal	reverseEvalP( fpreal y );	// y -> u

    fpreal	evalP( fpreal u )		// u -> y
    {
	return x0 + u*(x1 + u*(x2 + u*x3));
    }

    fpreal	eval( fpreal t );		// t -> y

    fpreal	calculateSlopeP( fpreal u );

    fpreal	calculateAccelP( fpreal u );

    void	display();
};

#endif
