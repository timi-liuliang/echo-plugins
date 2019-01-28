/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Quad Edge Library (C++)
 *
 * COMMENTS:
 */

#ifndef _GQ_Point_h_
#define _GQ_Point_h_

#include "GQ_API.h"
#include <GEO/GEO_Detail.h>
#include <GA/GA_Types.h>
#include <SYS/SYS_Math.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_SmallObject.h>
#include <SYS/SYS_Types.h>

class GQ_Edge;

class GQ_API GQ_Point : public UT_SmallObject<UT_NonCopyable>
{
public:
    explicit		 GQ_Point(GEO_Detail *gdp, GA_Offset ptoff)
			    : myGdp(gdp)
			    , myPtOff(ptoff)
			    , mySpare(0)
			    , myEdge(0)
			    , myFlags(0)
			 {
			 }

    bool		 isValid() const    { return GAisValid(myPtOff); }

    GEO_Detail		*geoDetail() const  { return myGdp; }

    GA_Offset		 ptOff() const	    { return myPtOff; }
    GA_Index		 ptNum() const	    { return myGdp->pointIndex(myPtOff);}

    UT_Vector3		 getPos3() const    { return myGdp->getPos3(myPtOff); }
    UT_Vector4		 getPos4() const    { return myGdp->getPos4(myPtOff); }

    void		 setPos3(const UT_Vector3 &pos)
			 {
			     myGdp->setPos3(myPtOff, pos);
			 }

    int			 aboveOrBelow(UT_Vector3 &normal, float distance) const
			 {
			     float dist = dot(getPos3(), normal) + distance;
			     if (SYSequalZero(dist)) return 0;
			     else return (dist > 0.0F) ? 1 : -1;
			 }
    float		 distToPlane(UT_Vector3 &normal, float distance) const
			 {
			     return dot(getPos3(), normal) + distance;
			 }

    void		 setEdge(GQ_Edge *e) 		{ myEdge = e; }
    GQ_Edge		*getEdge() const 		{ return myEdge; }

    void		 setFlags(unsigned mask) 	{ myFlags = mask; }
    unsigned             getFlags(unsigned mask=~0) const
						     { return myFlags & mask; }
    void 		 addFlags(unsigned mask) 	{ myFlags |= mask; }
    void 		 clearFlags(unsigned mask=~0)	{ myFlags &= ~mask; }

private:
    GEO_Detail		*myGdp;
    GA_Offset		 myPtOff;

public:
    void		*mySpare;   // public for legacy reasons

private:
    GQ_Edge		*myEdge;    // an edge that share this point at org()
    unsigned		 myFlags;
};
#endif
