/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Utility Library (C++)
 *
 * COMMENTS:	Calculate an isosurface from a field.  The evaluation
 *		routine should return a positive value outside the surface
 *		and a negative value inside for the computed normals to
 *		point outward.
 */

#ifndef __GU_Iso__
#define __GU_Iso__

#include "GU_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Assert.h>

class GU_Detail;
class GEO_Primitive;

/// Function to determine inside/outside tests.
typedef float	(*GU_IsoCallback)(const UT_Vector3 &pos, void *data);

class GU_API GU_IsoSurface
{
public:
	     GU_IsoSurface(bool isthreadsafe = false);
    virtual ~GU_IsoSurface();

    void	setCallback(GU_IsoCallback eval, void *eval_data)
		    { myEval = eval; myEvalData = eval_data; }

    void	polygonize(GU_Detail *gdp, const UT_BoundingBox &box,
			   int xdivs, int ydivs, int zdivs,
                           bool makepolysoup = false);

    /// Evaluate the field at a position
    virtual float	evalEdge(const UT_Vector3 &pos)
			{ return myEval(pos, myEvalData); }

    /// Check whether gradients are available for surfacing
    virtual bool	hasGradient() const	{ return false; }

    /// Evaluate gradients if available
    virtual UT_Vector3	evalGrad(const UT_Vector3 &pos)
			{ return UT_Vector3(0, 0, 0); }

    /// This function can be used to fill attribute information.  The
    /// position of the point will be set to its final position when this
    /// function is called.  The eval() method will have just been used so
    /// that you can use any information that would have been set up for
    /// this.
    virtual void	fillPointVertexAttributes(GU_Detail &detail,
                                                  GA_Offset vtxoff) {}

    /// This function can be used to fill out primitive attribute information.
    virtual void	fillPrimitiveAttributes(GEO_Primitive *primitive) {}

    /// This indicates whether evalEdge and evalGrad
    /// can be called in multiple threads at the same time.
    bool                isThreadSafe() const {return myIsThreadSafe;}

private:
    GU_IsoCallback	myEval;
    void *		myEvalData;
    bool                myIsThreadSafe;
};

#endif // __GU_Iso__
