/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Quad Edge Library (C++)
 *
 * COMMENTS:	Code for a quad edge face
 *
 */

#ifndef _GQ_Face_h_
#define _GQ_Face_h_

#include "GQ_API.h"
#include "GQ_Point.h"
#include <GA/GA_Types.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_SmallObject.h>

class GQ_Edge;
class GQ_Point;
class GEO_PrimPoly;

class GQ_Face;

class GQ_API GQ_Face: public UT_SmallObject<GQ_Face>
{
public:
    explicit		 GQ_Face(GEO_PrimPoly *p)
			 {
			    myPoly = p;
			    myEdge = 0;
			    mySpare = 0;
			    myNormal = 0;
			    myFlags = 0;
			    myIndex = -1;
			    myGQpt = 0;
			 }
			~GQ_Face()		 { delete myNormal; }
    GEO_PrimPoly	*getPoly() const	{ return myPoly; }
    void		 setPoly(GEO_PrimPoly *p) { myPoly = p; }

    // Assume no intersection of edges with plane 
    int                  aboveOrBelow(UT_Vector3 &normal, float distance);

    void		 setEdge(GQ_Edge *e) 	{ myEdge = e; }
    void		 adjustToSafeEdge();
    GQ_Edge		*getEdge() const	{ return myEdge; }
    int			 getNumEdges() const;

    void                 setFlags(unsigned mask)        { myFlags = mask; }
    unsigned		 getFlags(unsigned mask=~0) const
						      { return myFlags & mask; }
    void                 addFlags(unsigned mask)        { myFlags |= mask; }
    void                 clearFlags(unsigned mask=~0)   { myFlags &= ~mask; }

    // Face point operators, face points are temporarily created:
    void		 setGQPoint(GQ_Point *pt)	{ myGQpt = pt; }
    GQ_Point		*getGQPoint() const	{ return myGQpt; }

    GA_Offset		 ptOff() const
			    { return myGQpt->ptOff(); }

    // Public data use it with care please
    union {
	int		 mySpareInt;
	exint		 mySpareExint;
	void 		*mySpare;
    };

    UT_Vector3		*myNormal;
    int			 myIndex;

private:

    GEO_PrimPoly        *myPoly;
    GQ_Point		*myGQpt;
    GQ_Edge             *myEdge;       // an edge that share this face on left() 
    unsigned             myFlags;
};
#endif

