/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_NSidedSubdivPatch.h (GU Library, C++)
 *
 * COMMENTS: This class is based off the subdivision scheme detailed in the
 * following paper:
 *      Filling an N-sided hole using subdivision schemes
 *      Adi Levin
 *      adilev@math.tau.ac.il, http://www.math.tau.ac.il/~adilev
 *      Tel Aviv university
 *      June 3, 1999
 *      http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.16.4131&rep=rep1&type=pdf
 *
 * Note that this is different from GU_SubDivPatch, this takes some extra work
 * to ensure that the sides of the hole are smooth with the surface surrounding
 * the hole. An example of it's use can be seen in GU_PolyBevel.C in
 * BlendPositioner::positionPointsCubicSubdiv().
 */


#ifndef __GU_GU_NSidedSubdivPatch_h__
#define __GU_GU_NSidedSubdivPatch_h__

#include "GU_API.h"

#include <UT/UT_Vector.h>

class GU_API GU_NSidedSubdivPatch
{
public:
    GU_NSidedSubdivPatch(int sides, bool cubic = false) :
	mySides(sides), myCubic(cubic), myDivs(1)
    {
        setupMesh();
    }

    GU_NSidedSubdivPatch(const GU_NSidedSubdivPatch *src) :
	mySides(src->sides()), myCubic(src->myCubic), myDivs(2 * src->divs())
    {
        setupMesh();
    }

    int			 divs() const { return myDivs; }
    int			 sides() const { return mySides; }

    const UT_Vector3	&pt(int m, int i, int j) const
			    { return myPoints(pointIndex(m, i, j)); }

    UT_Vector3		&pt(int m, int i, int j)
			    { return myPoints(pointIndex(m, i, j)); }

    UT_Vector3		 interpolate(int m, fpreal u, fpreal v);

    void		 subdivide(const GU_NSidedSubdivPatch *src);

    int		 pointIndex(int m, int i, int j) const;
    
private:
    
    void                 setupMesh();

    
    void		 subdivideQuadratic(const GU_NSidedSubdivPatch *src);
    void		 subdivideCubic(const GU_NSidedSubdivPatch *src);

    int			 mySides;
    int			 myDivs;
    int			 myMeshSize;

    fpreal		 myMeshUnit;

    bool		 myCubic;
    UT_Vector3Array	 myPoints;
};

#endif
