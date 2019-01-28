/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel library (C++)
 *
 * COMMENTS:	Local variable definition
 *
 */

#ifndef __CH_LocalVariable_h__
#define __CH_LocalVariable_h__

#include "CH_API.h"
#define CH_VARIABLE_NONE	0x0000		// 
#define CH_VARIABLE_TIME	0x0001		// $T, $F
#define CH_VARIABLE_VALUE	0x0002		// $IV, $OV
#define CH_VARIABLE_SLOPE	0x0004		// $IM, $OM
#define CH_VARIABLE_ACCEL	0x0008		// $IA, $OA
#define CH_VARIABLE_CHANNEL	0x0010		// another channel
#define CH_VARIABLE_INTVAL	0x0020		// By default, an integer value
#define CH_VARIABLE_STRVAL	0x1000		// By default, a string value
#define CH_VARIABLE_VOIDVAL	0x2000		// By default, a void * value
#define CH_VARIABLE_VECTORVAL	0x4000		// By default, a vector value
#define CH_VARIABLE_MATRIXVAL	0x8000		// By default, a matrix value

class CH_API CH_LocalVariable {
public:
    const char	*name;		// name of the variable
    unsigned	 id;		// unique ID in the table
    unsigned	 flag;		// dependency flags

    unsigned	isTimeDependent() const	{ return flag & CH_VARIABLE_TIME; }
    unsigned	isSegmentValue() const	{ return flag & CH_VARIABLE_VALUE; }
    unsigned	isSegmentSlope() const	{ return flag & CH_VARIABLE_SLOPE; }
    unsigned	isSegmentAccel() const	{ return flag & CH_VARIABLE_ACCEL; }
};

#endif
