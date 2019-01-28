/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class implements a piecewise RBezier defined by 
 *	a set of breakpoints, basis functions, and CVs.
 *
 * 
 */

#ifndef __GEO_PrimRBezSurf_h__
#define __GEO_PrimRBezSurf_h__

#include "GEO_API.h"
#include "GEO_TPSurf.h"
#include "GEO_PrimType.h"

class GEO_API GEO_PrimRBezSurf : public GEO_TPSurf
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimRBezSurf(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_TPSurf(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    /// Please read the comments on the parent class destructor.
    virtual ~GEO_PrimRBezSurf() {}

public:
    // Given a domain value (u,v), store all the basis derivatives 
    // (from 0 to du and from 0 to dv inclusive) into ubmatx and vbmatx
    // respectively. Return the min indices of the CVs needed
    // for the linear combination. The indices may exceed the number of
    // vertices if the surface is wrapped, so remember to use modulus (%).
    virtual int		evaluateBasisDerivs(float u, float v,
				     float ubmatx[][GA_MAXORDER],
				     float vbmatx[][GA_MAXORDER],
				     int &rowoffset, int &coloffset,
				     unsigned du=0, unsigned dv=0,
				     int uoffset=-1, int voffset=-1) const;

    // Evaluate the basis at the given point in the domain and also return
    // the index of the first CV to linearly combine the basis with. The CV
    // indices may exceed the number of vertices if the surface is wrapped,
    // so remember to use modulus (%).
    virtual int		evaluateBasis(float u, float v, float *ubvals,
				     float *vbvals, int &rowoffset,
				     int &coloffset,
				     unsigned du=0, unsigned dv=0,
				     int uoffset=-1, int voffset=-1) const;
    virtual int		evaluateUBasis(float u, float *ubvals, int &coloffset,
				     unsigned du=0, int uoffset=-1) const;
    virtual int		evaluateVBasis(float v, float *vbvals, int &rowoffset,
				     unsigned dv=0, int voffset=-1) const;

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		evaluateBreakpoint(int uidx, int vidx,
					   UT_Vector4 &pos,
					   int du=0, int dv=0) const;

    // Given a CV index figure out the min/max indicies of the knots between
    // which the curve needs to be re-evaluated if the CV changes. If not
    // given a valid CV index, the method returns -1. Otherwise it returns 0.
    virtual int		domainRangeOfCV(int i, int j, int &minuk,int &maxuk,
					int &minvk,int &maxvk) const;

    // Given a CV index figure out the min/max breakpoints which are
    // affected if the CV changes.  If not given a valid CV index, the
    // method returns -1.  Otherwise it returns 0.  Also returns -1 if
    // no breakpoints are affected.
    // NOTE: use % breakCount since maxbkp may be >= breakCount
    virtual int         breakpointRangeOfCV(int i, int j, int &minbkpu,
                                            int &maxbkpu, int &minbkpv,
					    int &maxbkpv) const;

    // Reparameterize the surface by changing its basis. The first breakpoint
    // is not changed.
    virtual void	reparameterizeU(GA_ParameterizationType ptype);
    virtual void	reparameterizeV(GA_ParameterizationType ptype);

    // Insert a breakpoint at the given point in the domain and return its
    // index if successful. Return -1 in case of failure (eg. if outside the
    // valid domain or equal to an existing breakpoint).
    virtual int		refineU(float k, GA_AttributeRefMap &hlist, int=1);
    virtual int		refineU		(float k, int=1);

    virtual int		refineV(float k, GA_AttributeRefMap &hlist, int=1);
    virtual int		refineV		(float k, int=1);

    // To gain access to the GEO_AttributeHandleList versions of refine()
    using GEO_Hull::refineU;
    using GEO_Hull::refineV;

    // Set the wrap or open flag, and make sure the basis knot vectors are
    // consistent with the change. These functions are virtual by inheritance.
    // TODO-CB: increase the size of the knot vector if necessary.

    // Wrap in the specified U/V direction.
    //
    // If preserveShape = 1:
    //          add extra row/col cvs at the end to define the last span.
    //          if rounded,
    //                  the new cvs just added will be arranged to give a 
    //			rounded appearance in the specified U/V direction.
    //          else
    //                  the new cvs just added will form a plane
    //                  between the first and last row/col endpoint.
    // If preserveShape = 0:
    //          just grow the U/V basis by one span, without adding any 
    //		new cvs and ignore the rounded flag.
    virtual void	wrapU(int rounded = 1, int preserveShape = 0);
    virtual void	wrapV(int rounded = 1, int preserveShape = 0);

    // Open in the specified U/V direction.
    //
    // If preserveShape = 1:
    //          the open surface will have exactly the same geometry
    //          (ie. still look closed) with the first row/col point 
    //		duplicated.
    // If preserveShape = 0:
    //          the open surface will remove the last span in the basis,
    //          thus open up a gap in the surface.
    //          last few row/col cvs are dangling which do not form a valid
    //          surface patch, if the safe flag = 0.
    // The safe flag is to remove unneed cvs for ensuring the resulting
    // surface is a valid bezier surface.
    virtual void	openU(int preserveShape = 0, int safe = 0);
    virtual void	openV(int preserveShape = 0, int safe = 0);

    // Insert or remove rows and columns. The insertion returns the index if
    // successful. The deletion methods return 0 if ok. If failure, return -1.
    virtual int		insertRow(unsigned int beforeWhich, bool appendPts=true);
    virtual int		insertCol(unsigned int beforeWhich, bool appendPts=true);
    virtual int 	deleteRow(unsigned int which);
    virtual int 	deleteCol(unsigned int which);

    // Find the max u and v indices of the bases:
    virtual int		uMaxValidIndex(void) const;
    virtual int		vMaxValidIndex(void) const;

    // Raise the number of CVs to match the newcount. The shape of the curve
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int		loftU(int newcount, int start=-1, int stop=-1);
    virtual int		loftV(int newcount, int start=-1, int stop=-1);

    // Warp the mesh at u,v by the given delta. Change 1 or 4 Cvs and possibly
    // insert a knot once or more as well. If a knot is inserted or we happen
    // to land exactly on a knot, we change only one CV. The bias makes sense
    // only when changing 4 CVs, and will be ignored altogether if < 0.
    // We return the CV index in warpU/V and 0 in warp() if OK; -1 otherwise.
    virtual int		 warpU(float u, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float sharpness = 0.0f, float bias = -1.0f);
    virtual int		 warpV(float v, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float sharpness = 0.0f, float bias = -1.0f);
    virtual int		 warp (float u, float v, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float usharpness = 0.f, float vsharpness = 0.f,
			       float ubias = -1.0f, float vbias = -1.0f);

    virtual int         unrollU(int append_pts = 1);
    virtual int         unrollV(int append_pts = 1);

    // Reverse rows (V) or columns (U). Reversing in either direction will
    // flip the normal.
    virtual void	reverseU(void);
    virtual void	reverseV(void);

    // Translate the CVs such that the given breakpoint change positions by
    // the given delta.  Return -1 if something goes wrong, 0 if translation
    // was successful.
    // NOTE: Cannot contain any duplicates.  If the surface is wrapped,
    //       the first and last breakpoint are considered the same.
    virtual int         translateBreakpoints(const UT_IntArray &uindices,
                                             const UT_IntArray &vindices,
                                             const UT_Vector3 &delta,
					     int fixbpkts = 1,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);

    virtual int		transformBreakpoints(const UT_IntArray &uindices,
					     const UT_IntArray &vindices,
					     const UT_Matrix4 &matx,
					     int fixbpkts = 1,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);

    // Append another hull to us in one of two ways: blend the two endpoints
    // or connect them straight or rounded. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert knots into the hulls to minimize the affected areas; if 1,
    // no refinement is done. For the non-blend case, the tolerance will
    // generate a span whose shape goes from round to straight; 0 tolerance
    // means straight connection. If unrefine is on, we'll try to reduce the
    // complexity of the hull if we're connecting rounded. We return 0 if OK
    // and -1 if error. Both hulls must be open and have the same order.
    virtual int		attachU(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int = 1, GA_PointGroup *ptgroup=0);
    virtual int		attachV(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int = 1, GA_PointGroup *ptgroup=0);

    // If [ustart,ustop] and [vstart,vstop] are in the valid interval, return
    // the part of the surface delimited by those values in a new primitive.
    // Return 0 if a problem is encountered.
    virtual GEO_TPSurf	*extract(float ustart, float ustop,
				 float vstart, float vstop) const;

    // Reverse the roles of rows and columns
    virtual void	transpose();

    virtual const GA_PrimitiveJSON	*getJSON() const;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_TPSurf::buildFamilyMask(); }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_TPSurf::registerIntrinsics(defn); }

    // Get a new basis of a type that matches our type:
    virtual GA_Basis	*newBasis(void) const;

    // increase order by 1.
    void		incrementOrderU(void);
    void		incrementOrderU(GA_AttributeRefMap &map);

    void		incrementOrderV(void);
    void		incrementOrderV(GA_AttributeRefMap &map);
    
    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    virtual int		raiseOrderURefMap(int neworder, GA_AttributeRefMap &m);
    virtual int		raiseOrderUInt       (int neworder);
    virtual int		raiseOrderVRefMap(int neworder, GA_AttributeRefMap &m);
    virtual int		raiseOrderVInt       (int neworder);
    
private:
    // Nothing.
};
#endif
