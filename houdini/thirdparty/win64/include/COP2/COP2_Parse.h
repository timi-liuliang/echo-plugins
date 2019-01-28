/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Parse.h
 *
 * COMMENTS:
 *	Parses a plane creation expression.
 */
#ifndef COP2_PARSE_H
#define COP2_PARSE_H

#include "COP2_API.h"
#include <TIL/TIL_Defines.h>
class TIL_Sequence;
class UT_String;

class COP2_API COP2_Parse
{
public:
		COP2_Parse(TIL_Sequence *seq);
		~COP2_Parse();

    // creates planes from the passed string.
    // syntax <Name> [(N)] [{C1,C2[,C3[,C4]]}] [:i8|i16|i32|f32[(B,W)]
    // Name		= vector name
    // N		= array size (int)
    // C1/C2/C3/C4	= component name
    // B/W		= Black/white points (int)
    int		parse(UT_String &create_string, TIL_DataFormat default_type,
		      int use_bw, int b, int w);

    // Scopes the channels in the sequence with the passed string
    // (sets the plane masks).
    // returns 0 if the scope string is invalid.
    // If allow_partially_scoped_planes is false, then entire planes are
    // scoped.
    int		scope(UT_String &scope_string, 
	              bool allow_partially_scoped_planes);

    void	frameScope(UT_String &frame_scope, bool scope,
			   int start, int length = 1);

    const char *getError() const;
    
private:
    int		parseSingle(char *create, int length);
    int		parseVector(char *comps, int length);
    int		parsePoints(char *pnts, int length);
    
    int		scopeSingle(char *scope, int length, 
			    bool allow_partially_scoped_planes);
    int		scopeArray(char *bounds, int length);

    void	createPlanes(const char *name);
    void        addError(const char *error);
    
    TIL_Sequence	*mySequence;

    // parsing stuff...
    TIL_DataFormat	myDefaultType;
    TIL_DataFormat	myPlaneType;
    int			myPlaneArraySize;
    int			myPlaneBlack;
    int			myPlaneWhite;
    int			myPlaneUsePoints;
    int			myDefaultBlack;
    int			myDefaultWhite;

    // scoping stuff...
    int			myCompScope;
    int			myCompCount;
    char		myComp[PLANE_MAX_VECTOR_SIZE][256];
    int			myArrayScope;
    int			myArrayStart, myArrayEnd, myArrayStep;

    // error reporting.
    const char		*myError;
};




#endif
