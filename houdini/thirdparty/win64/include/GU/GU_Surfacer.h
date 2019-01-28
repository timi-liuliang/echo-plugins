/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Surfacer.h (C++)
 *
 * COMMENTS:
 * 
 */

#ifndef __GU_Surfacer_h__
#define __GU_Surfacer_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <UT/UT_Map.h>

class GEO_PrimPolySoup;
class guSurfacerCellEvaluator;

class GU_API GU_Surfacer
{
public:
    GU_Surfacer(GU_Detail &gdp,
		const UT_Vector3 &pos,
		const UT_Vector3 &size,
		int xdivs, int ydivs, int zdivs,
		bool do_normals,
                bool makepolysoup = false);
    virtual ~GU_Surfacer();

    /// Return the size of a cell
    const UT_Vector3	&getStepSize() const	{ return myStepSize; }

    /// create polygons for cell given density values at the corners and
    /// optionally gradient values
    void addCell(int x, int y, int z,
		 const fpreal density[8],
		 const UT_Vector3 gradient[8]);

    /// If using addCell directly, instead of polygonize, call done to finish.
    void done();

    class Evaluator {
    public:
	Evaluator(bool hasgradient, bool isthreadsafe=false, bool subsample=false)
	    : myHasGradient(hasgradient)
            , myIsThreadSafe(isthreadsafe)
	    , mySubSample(subsample)
        {}
	virtual ~Evaluator() {}

	virtual fpreal		density(const UT_Vector3 &pos) const = 0;

	bool			hasGradient() const { return myHasGradient; }
	virtual UT_Vector3	gradient(const UT_Vector3 &pos) const
				{ return UT_Vector3(0, 0, 0); }
        /// This indicates whether density and gradient
        /// can be called in multiple threads at the same time.
        bool                    isThreadSafe() const {return myIsThreadSafe;}
	/// This indicates whether the density function should be sampled
	/// along a cell edge to find the zero crossing.
	bool			subsample() const { return mySubSample; }

    private:
	bool	myHasGradient;
        bool    myIsThreadSafe;
	bool	mySubSample;
    };

    /// Surface the entire range, using the Evaluator to calculate density
    /// and possibly gradient values.
    void polygonize(Evaluator &eval);
    
protected:
    /// called after adding a new point to the detail
    virtual void pointAdded(GEO_Detail *gdp, GA_Offset ptoff, int idx0, int idx1, fpreal u) {}
    /// called after adding a complete polygon or polysoup to the detail
    virtual void primitiveAdded(GEO_Primitive *primitive) {}

private:
    void addCell(int x, int y, int z, const guSurfacerCellEvaluator *e);

    GU_Detail		&myGdp;
    UT_Vector3		 myP;
    UT_Vector3		 mySize;
    UT_Vector3		 myStepSize;
    int			 myXDivs;
    int			 myYDivs;
    int			 myZDivs;
    GA_RWHandleV3	 myNormalAttrib;
    UT_Map<int64, GA_Offset> myPoints;
    GEO_PrimPolySoup	*myPolySoup;
};

#endif
