/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	
 *
 * COMMENTS:
 *
 */

#ifndef __GU_CAP_H__
#define __GU_CAP_H__

#include "GU_API.h"
enum GU_CapType
{
    GU_CAP_NONE    	= 0,
    GU_CAP_FACETED	= 1,
    GU_CAP_SHARED 	= 2,
    GU_CAP_ROUNDED	= 3,
    GU_CAP_TANGENT	= 4
};

enum GU_CapKind
{
    GU_FIRST_UCAP	= 0,
    GU_LAST_UCAP	= 1,
    GU_FIRST_VCAP	= 2,
    GU_LAST_VCAP	= 3
};

class GU_API GU_CapOptions
{
public:
    GU_CapOptions(void);

    GU_CapType	firstUCap, lastUCap;
    GU_CapType  firstVCap, lastVCap;

    int		pshapeU;
    int         numfirstUCaps, numlastUCaps;
    int         numfirstVCaps, numlastVCaps;

    int		pshapeV;
    float	firstUScale, lastUScale;
    float	firstVScale, lastVScale;

    bool        sharePoints;
};

class GU_API GU_CapParms 
{
public:
    GU_CapParms(GU_CapType itype, GU_CapKind ikind, int inum=0, 
		float iscale=1.0F, int ipshape=1) 
    { 
	type = itype; 
	kind = ikind;
        numcaps = inum; 
	scale = iscale;
	pshape = ipshape;
    }

    GU_CapType	 type;		// type of cap -> rounded, flat, etc.
    GU_CapKind	 kind;		// first, last, in U or V
    int		 numcaps;	// number of division in the rounded cap
    float	 scale;		// scaling of rounded cap height
    int		 pshape;	// preserve nurb shape
};

#endif
