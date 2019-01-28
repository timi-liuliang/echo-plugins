/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class implements a non-uniform B-Spline surface defined by 
 *	a set of knots, basis functions, and CVs.
 *
 */

#ifndef __GEO_PrimNURBSurf_h__
#define __GEO_PrimNURBSurf_h__

#include "GEO_API.h"
#include "GEO_TPSurf.h"
#include "GEO_PrimType.h"

class GA_Detail;
class GA_NUBBasis;

class GEO_API GEO_PrimNURBSurf : public GEO_TPSurf
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimNURBSurf(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_TPSurf(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    /// Please read the comments on the parent class destructor.
    virtual ~GEO_PrimNURBSurf() {}

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

    // Reparameterize the surface by changing its basis. All the knots up to
    // and including the first valid knot remain unchanged. This type of 
    // reparameterization is generally NOT shape preserving:
    virtual void	reparameterizeU(GA_ParameterizationType ptype);
    virtual void	reparameterizeV(GA_ParameterizationType ptype);

    // Change the multiplicity of the knot by inserting it "r" times after
    // checking its current multiplicity. The maximum valid r for a knot is
    // equal to the degree of the basis (ie order-1). Return -1 if k is outside
    // the valid interval or greater than the number of CVs. Otherwise, return
    // the index of the inserted knot.

    virtual int		refineU(float k, GA_AttributeRefMap &hl, int r=1);
    virtual int		refineU	      (float k, int r=1);

    virtual int		refineV(float k, GA_AttributeRefMap &hl, int r=1);
    virtual int		refineV	      (float k, int r=1);

    // To gain access to the GEO_AttributeHandleList versions of refine()
    using GEO_Hull::refineU;
    using GEO_Hull::refineV;
    // Warp the nurb at u,v by the given delta. Change 1 or 4 Cvs and possibly
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

    // Set the wrap or open flag, and make sure the basis knot vectors are
    // consistent with the change. These functions are virtual by inheritance.
    virtual void	wrapU(int rounded = 1, int preserveShape = 0);
    virtual void	openU(int preserveShape = 0, int = 0);
    virtual void	wrapV(int rounded = 1, int preserveShape = 0);
    virtual void	openV(int preserveShape = 0, int = 0);

    virtual int         unrollU(int append_pts = 1);
    virtual int         unrollV(int append_pts = 1);

    // Check if the basis interpolates the endpoints, or change the flag.
    bool		interpolatesEndsU() const;
    void		toggleEndConditionU(void);
    bool		interpolatesEndsV() const;
    void		toggleEndConditionV(void);

    // Insert or remove rows and columns. The insertion returns the index if
    // successful. The deletion methods return 0 if ok. If failure, return -1.
    virtual int		insertRow(unsigned int beforeWhich, bool appendPts=true);
    virtual int		insertCol(unsigned int beforeWhich, bool appendPts=true);
    virtual int 	deleteRow(unsigned int which);
    virtual int 	deleteCol(unsigned int which);

    // If the surface is wrapped in U and/or V, explicitly add the wrapped 
    // vertex (or vertices) and open the surface in the direction(s) 
    // it's wrapped in.
    virtual void	fixSeamsU(void);
    virtual void	fixSeamsV(void);

    // Find the valid min and max u and v indices of the bases:
    virtual int		uMinValidIndex(void) const;
    virtual int		uMaxValidIndex(void) const;
    virtual int		vMinValidIndex(void) const;
    virtual int		vMaxValidIndex(void) const;

    // Raise the number of CVs to match the newcount. The shape of the curve
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int		loftU(int newcount, int start=-1, int stop=-1);
    virtual int		loftV(int newcount, int start=-1, int stop=-1);

    // Merge a bunch of NURB surfaces together. The surfaces are assumed to 
    // be all NURBS, all of the same order and characteristics such as
    // non/end-interp, open/wrapped.
    static int 		mergeGroupU(GEO_Detail *gdp, GA_PrimitiveGroup *nurbs);
    static int 		mergeGroupV(GEO_Detail *gdp, GA_PrimitiveGroup *nurbs);

    // Shift the array of vertices by an offset and wrap around.
    // Cycle a subrange if the curve is closed and cycle the basis
    // accordingly. The offset can be either negative or positive.
    // Optionally remap the new basis to the original length and origin
    virtual int		cycleU(int amount, int keepSpan = 1);
    virtual int		cycleV(int amount, int keepSpan = 1);

    // Remove the interior knot at kidx once in udir if possible, where mult is
    // the multiplicity of the knot, and U/V[kidx] != U/V[kidx+1].
    // If multiplicity mult is not given, the procedure will compute it.
    // The tol specifies the tolerance of distance between the knot removable
    // position and its actual position. To force knot removal, set tol = -1.0F.
    // The deletegroup is used to gather unused cv geo points to be deleted.
    // (It is more efficient to delete points in a group.)
    // Output: 1 if the knot got removed, else 0.
    // Note: During the process of knot removal, interior cvs geo points
    //       may changed and/or deleted.
    int			 unrefine(int udir, int kidx, 
				   GA_AttributeRefMap &hlist,
				   int mult=0, float tol=1e-4F,
				   GA_PointGroup *delgroup=0);
    int			 unrefine (int udir, int kidx, int mult=0,
				   float tol=1e-4F,GA_PointGroup *delgroup=0);

    // Get error bound for removing of a knot once.
    float		 getKnotRemovalBound(int uDir, int curveIdx, 
					     int knotIdx, int mult=0) const;

    // If this surface is open and non-end-interpolating, insert knots such that
    // it becomes end-interpolating without changing its shape. Return 0 if
    // successful and -1 othewise.
    // If the delPoints group is passed in, the points to be deleted will be
    // added to the group.  Otherwise, the points will be deleted from the gdp.
    int                 clampU(GA_PointGroup *delPoints = 0);
    int                 clampV(GA_PointGroup *delPoints = 0);
    void		unclampU(void); 
    void		unclampV(void); 

    virtual bool	isClampedU() const;
    virtual bool	isClampedV() const;

    // Translate the CVs such that the given breakpoint change positions by
    // the given delta.  Return -1 if something goes wrong, 0 if translation
    // was successful.
    // NOTE: Cannot contain any duplicates.  If the surface is wrapped,
    //       the first and last breakpoint are considered the same.
    virtual int		translateBreakpoints(const UT_IntArray &uindices,
                                             const UT_IntArray &vindices,
                                             const UT_Vector3 &delta,
					     int fixbkpts = 1,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);

    virtual int		transformBreakpoints(const UT_IntArray &uindices,
					     const UT_IntArray &vindices,
					     const UT_Matrix4 &matx,
					     int fixbkpts = 1,
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
			        int unrefine = 1,GA_PointGroup *ptgroup=0);
    virtual int		attachV(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int unrefine = 1,GA_PointGroup *ptgroup=0);

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

    // Reverse rows (V) or columns (U). Reversing in either direction will
    // flip the normal.
    virtual void	reverseU(void);
    virtual void	reverseV(void);


    // Parameter correction by Newton iteration.
    void		 correctParam(const UT_Vector4 &p, 
				      float &u, float &v,
				      float distTol=1e-10F, float angTol=1e-2F, 
				      int maxIter=50) const;

    // Protected methods to translate/transform selected breakpoints and
    // perform an interpolation.
    virtual int          translateBkptsFixed(const UT_IntArray &uindices,
                                             const UT_IntArray &vindices,
                                             const UT_Vector3 &delta,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);
    virtual int		 transformBkptsFixed(const UT_IntArray &uindices,
					     const UT_IntArray &vindices,
					     const UT_Matrix4 &matx,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);

    // Methods to translate/transform selected breakpoints which do not
    // perform an interpolation.
    virtual int          translateBkptsNonFixed(const UT_IntArray &uindices,
                                             const UT_IntArray &vindices,
                                             const UT_Vector3 &delta,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);
    virtual int		 transformBkptsNonFixed(const UT_IntArray &uindices,
					     const UT_IntArray &vindices,
					     const UT_Matrix4 &matx,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);
 
    // Given a parameter in the domain, insert as many CVs as necessary to
    // create a discontinuity at the corresponding point on the curve.The shape
    // of the curve should NOT change. Return u's index upon success and -1
    // otherwise.
    virtual int		subdivideURefMap (float u, GA_AttributeRefMap &map);
    virtual int		subdivideUFloat	 (float u);

    virtual int		subdivideVRefMap (float u, GA_AttributeRefMap &map);
    virtual int		subdivideVFloat	 (float u);

    virtual int		 unrefineURefMap(int kidx, 
				   GA_AttributeRefMap &hlist,
				   int mult=0, float tol=1e-4F,
				   GA_PointGroup *delgroup=0);
    virtual int		 unrefineUFloat(int kidx, int mult=0, float tol=1e-4F,
				   GA_PointGroup *delgroup=0);

    virtual int		 unrefineVRefMap(int kidx, 
				   GA_AttributeRefMap &hlist,
				   int mult=0, float tol=1e-4F,
				   GA_PointGroup *delgroup=0);
    virtual int		 unrefineVFloat(int kidx, int mult=0, float tol=1e-4F,
				   GA_PointGroup *delgroup=0);

    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    virtual int		raiseOrderURefMap(int neworder, GA_AttributeRefMap &h);
    virtual int		raiseOrderUInt       (int neworder);
    virtual int		raiseOrderVRefMap(int neworder, GA_AttributeRefMap &h);
    virtual int		raiseOrderVInt       (int neworder);

private:
    int			 xformBreakpoints(const UT_IntArray &uindices,
					  const UT_IntArray &vindices,
					  const UT_Vector3Array &trans,
					  GA_PointGroup *ptgroup=NULL,
					  GEO_Delta *geodelta = 0);

    // Merge the given basis into ours and refine the surface accordingly.
    // It is assumed that both bases have the same order and that "basis"
    // contains all our knots plus possibly others. Also, it is assumed that
    // "basis" has been GA_Basis::map()-ed onto our knot interval. The 
    // method returns 0 if OK and -1 if problems.
    // Finally, it is assumed that the basis has no knots of higher
    // multiplicity than degree.  This is not checked for here, so one
    // must ensure the resulting basis is of proper length.
    int			mergeU(const GA_NUBBasis &basis);
    int			mergeV(const GA_NUBBasis &basis);

    // Return the multiplicity of knot k, cycling the curve first and/or
    // unsharing the wrapped points if necessary.

    int			alignKnotU(float k, int &kidx, 
					int &cycFix, int &interpFix);
    int			alignKnotV(float k, int &kidx,
					int &cycFix, int &interpFix);

    void		unAlignKnotU(int &kidx, int cycFix, int interpFix);
    void		unAlignKnotV(int &kidx, int cycFix, int interpFix);

};

#endif
