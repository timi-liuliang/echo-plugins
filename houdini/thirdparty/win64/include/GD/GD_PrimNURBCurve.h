/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class implements a non-uniform B-Spline curve defined by 
 *	a set of knots, basis function, and CVs.
 *
 */

#ifndef __GD_PrimNURBCurve_h__
#define __GD_PrimNURBCurve_h__

#include "GD_API.h"
#include "GD_Curve.h"
#include "GD_PrimType.h"
#include <GA/GA_NUBBasis.h>

class GD_API GD_PrimNURBCurve : public GD_Curve
{
public:
    // Constructor that attaches this curve to detail "d".
    GD_PrimNURBCurve(GD_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
	: GD_Curve(d, offset) {}

    // Trivial class destructor, virtual by inheritance. Please read the 
    // comments on the parent class d-tor.
    virtual ~GD_PrimNURBCurve() {}

    virtual const GA_PrimitiveDefinition &getTypeDef() const;

    // Build a new curve and insert it into the given detail.
    static GD_PrimNURBCurve	*build(GD_Detail *gdp, int npts,
				       int order = 4, int closed = 0,
				       int clamped = 1, int appendPts = 1);

    // Given a domain value (u), store all the basis derivatives (from 0 to du
    // inclusive) into bmatx. Return the first index of the CVs needed for the
    // linear combination in cvoffset.  The CV index may exceed the number of
    // vertices if the curve is wrapped, so remember to use modulus (%).
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluateBasisDerivs(fpreal u,
					    fpreal bmatx[][GA_MAXORDER],
					    int &cvoffset, unsigned du = 0,
					    int uoffset = -1) const;

    // Evaluate the basis at the given point in the domain and also return
    // the index of the first CV to linearly combine the basis with in cvoffset.
    // The CV index may exceed the number of vertices if the curve is wrapped,
    // so remember to use modulus (%). This method handles both rational and
    // non-rational curves.
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluateBasis(fpreal u, fpreal *ubvals,
				      int &cvoffset, unsigned du=0,
				      int uoffset=-1) const;

    // Curve evaluator that takes a start and a stop index
    // in the valid knot domain and an lod representing number of points to
    // be interpolated between every two breakpoints. The method ALWAYS
    // interpolates the encountered breakpoints (aka "edit points").
    // This returns true if successful, else false.  Please save
    // yourself headaches and pass VALID start and end indices (see the
    // method getValidInterval() in GA_Basis).
    virtual bool	evaluateBreakSegm(int uStartIdx, int uStopIdx, int lod,
				     UT_Vector3Array &pos, unsigned du=0) const;

    /// Implementation of NURBS evaluation
    virtual int		evaluateBreakSegment(int ustart, int uend,
			    int lod, GA_Offset *vertices,
			    GA_AttributeRefMap &h, uint du=0) const;

    // Given a CV index figure out the min/max indicies of the knots between
    // which the curve needs to be re-evaluated if the CV changes. If not
    // given a valid CV index, the method returns -1. Otherwise it returns 0.
    // If the curve wraps the domain we return will be rather large.
    virtual int		domainRangeOfCV(int cvidx, int &mink,int &maxk) const;

    // Convert a topologically open curve into a topologically closed one.
    // The rounded flag is mainly for surface end caps to indicate a half cap
    // or a full cap.
    virtual void	close(int rounded = 1, int preserveShape = 0);

    // Convert a topologically closed curve into a topologically open one. 
    virtual void	open(int preserveShape = 0, int = 0);

    // Check if the basis interpolates the endpoints, or change the flag.
    short		interpolatesEnds(void) const;
    void		toggleEndCondition(void);

    // Insert or delete vertices. The insertion methods return the index if
    // successful and -1 otherwise. The deletion methods return 0 if ok and
    // -1 otherwise. The insertion methods create the point if it does not
    // exist.
    virtual GA_Size	insertVertex(GA_Offset ptoff=GA_INVALID_OFFSET, GA_Size where=0);
    virtual GA_Size	appendVertex(GA_Offset ptoff=GA_INVALID_OFFSET);
    virtual int   	deleteVertex(GA_Size i);

    // Shift the array of vertices by an offset and wrap around.
    // Cycle a subrange if the curve is closed and cycle the basis
    // accordingly. The offset can be either negative or positive.
    // Optionally remap the new basis to the original length and origin
    virtual int         cycle(int amount, int keepSpan = 1);

    // Reverses the vertices of the curve.
    virtual void	reverse();

    // Build a trim loop (which will be open unless the face is closed). The
    // loop is build every time you call this method, so it's expensive. You
    // must free the loop yourself when you are done with it. The trim pieces
    // are generated in the [ustart,ustop] or [ustop,ustart] interval, where
    // ustart and ustop are parametric values.
    virtual GD_TrimLoop	*trimLoop(float ustart, float ustop) const;

protected:
    // Get a new basis of a type that matches our type:
    virtual GA_Basis *newBasis() const;

    /// All subclasses should call this method to register the NURBCurve
    /// intrinsics.
    /// @see GA_AttributeIntrinsic
    static GA_IntrinsicManager::Registrar
		rerIntrinsics(GA_PrimitiveDefinition &definition)
			{ return GD_Curve::registerIntrinsics(definition); }

private:
    // Fill the curve with data and build the basis. Return 0 if OK, and -1
    // if error.
    int		create(int nelems, int order=4, int closed = 0,
			    int interpEnds = 1, int appendPoints = 1);

    friend class GD_PrimitiveFactory;

    static GA_PrimitiveDefinition *theDefinition;
};

#endif
