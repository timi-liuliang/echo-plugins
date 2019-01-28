/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class handles tensor product (TP) surfaces of arbitrary degree.
 *      The Hull class maintains the hull for the surface.
 *
 * 
 */

#ifndef __GEO_TPSurf_h__
#define __GEO_TPSurf_h__

#include "GEO_API.h"
#include <UT/UT_BoundingRect.h>
#include <UT/UT_RefMatrix.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Interval.h>
#include <UT/UT_Matrix.h>
#include <GA/GA_Basis.h>
#include <GA/GA_GenericHandle.h>
#include "GEO_Hull.h"
#include "GEO_PrimType.h"

class GU_IsectCurveSet;
class GEO_Curve;
class GEO_Delta;
class GEO_Face;
class GEO_Greville;
class GEO_PasteSurf;
class GEO_Profiles;
class GEO_WorkVertexArray;
class GA_Detail;
template<bool isconst> class GA_PwHandle;
typedef GA_PwHandle<true> GA_PwHandleRO;
class GA_WorkVertexBuffer;
class UT_MemoryCounter;

class GEO_API GEO_TPSurf : public GEO_Hull
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    // You can optionally specify the number of rows and columns.
    GEO_TPSurf(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Hull(d, offset)
        , myUBasis(NULL)
        , myVBasis(NULL)
        , theProfiles(NULL)
        , thePasteSurf(NULL)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    // It assumes it is OK to delete the bases, so make sure you always
    // set the bases with objects allocated from the heap.
    virtual ~GEO_TPSurf();

public:
    // Return any dependent details (GEO_Profiles)
    virtual unsigned		 getNumSecondaryDetails() const;
    virtual const GA_Detail	*getSecondaryDetail(GA_Index i) const;
    virtual       GA_Detail	*getSecondaryDetail(GA_Index i);

    // Overwrite this surface with the data from src. Virtual by inheritance.
    virtual void		 copyPrimitive( const GEO_Primitive *src);
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;

    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);

    virtual void	 clearForDeletion();

    // Merge tpsurf's profiles into ours. Skin invisible ones if you want.
    void			 mergeProfiles(const GEO_TPSurf &tpsurf,
					       int onlyvisible = 0);


    // Evaluate one point (when du=dv=0), or the du-th dv-th derivative.
    // Return 0 if successful, and -1 otherwise.
    bool 		evaluate(fpreal u, fpreal v, GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				unsigned du=0, unsigned dv=0,
				int uoffset=-1, int voffset=-1) const;
    int 		evaluateHomogeneous(float u, float v, UT_Vector4 &pos,
				 unsigned du=0, unsigned dv=0,
				 int uoffset=-1, int voffset=-1) const;
    int 		evaluate(float u, float v, UT_Vector4 &pos,
				 unsigned du=0, unsigned dv=0,
				 int uoffset=-1, int voffset=-1) const;
    virtual void	computeInteriorPointWeights(
				UT_Array<GA_Offset> &vtxlist,
				UT_Array<float> &weightlist,
				fpreal u, fpreal v, fpreal w) const;

    template <typename T>
    bool 		evaluateHomogeneous(fpreal u, fpreal v,
				 const GA_ROGenericHandleVertex<T> &h, T &pos,
				 unsigned du=0, unsigned dv=0,
				 int uoffset=-1, int voffset=-1) const;

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
				     int uoffset=-1, int voffset=-1) const = 0;

    // Evaluate the basis at the given point in the domain and also return
    // the index of the first CV to linearly combine the basis with. The CV
    // indices may exceed the number of vertices if the surface is wrapped,
    // so remember to use modulus (%).
    virtual int		evaluateBasis(float u, float v, float *ubvals,
				     float *vbvals, int &rowoffset,
				     int &coloffset,
				     unsigned du=0, unsigned dv=0,
				     int uoffset=-1, int voffset=-1) const = 0;
    virtual int		evaluateUBasis(float u, float *ubvals, int &coloffset,
				     unsigned du=0, int uoffset=-1) const = 0;
    virtual int		evaluateVBasis(float v, float *vbvals, int &rowoffset,
				     unsigned dv=0, int voffset=-1) const = 0;

    // Return the value of the i'th u/vbasis at parameter u or v, or both.
    // This method handles both rational an non-rational surfaces. You need
    // to specify the row and column index only if the surface is rational,
    // so that we know whose weights to use.
    float		computeUBValue(float u, int uoff) const;
    float		computeVBValue(float v, int voff) const;
    float		computeBValue (float u,float v,int uoff,int voff)const;

    // Fill in parameter values that interpolate breakpoints.  The start
    // and stop indices are breakpoint indices.  lod specifies the number
    // of isoparms to insert between breakpoints in each dimension.
    void		fillBreakParameters(UT_FloatArray &uvals,
					    UT_FloatArray &vvals,
					    int ustartidx, int ustopidx,
					    int vstartidx, int vstopidx,
					    int ulod, int vlod) const;

    // Evaluate the surface for the given parameter arrays.  The provided
    // result array should provide storage for
    // uvals.entries()*vvals.entries() values.
    bool		evaluateMesh(const UT_FloatArray &uvals,
				     const UT_FloatArray &vvals,
				     UT_Vector4 *pos,
				     bool transpose = false,
				     unsigned du = 0, unsigned dv = 0) const;
    bool		evaluateMesh(const UT_FloatArray &uvals,
				     const UT_FloatArray &vvals,
				     GEO_WorkVertexArray &verts,
				     GEO_AttributeHandleList &hlist,
				     bool transpose = false,
				     unsigned du = 0, unsigned dv = 0) const;
    bool		evaluateMesh(const UT_FloatArray &uvals,
				     const UT_FloatArray &vvals,
				     GA_WorkVertexBuffer &verts,
				     GA_AttributeRefMap &map,
				     bool transpose = false,
				     unsigned du = 0, unsigned dv = 0) const;
    
    // Explicit instantiations are provided for float, UT_Vector3,
    // UT_Vector4, UT_Matrix3, UT_Matrix4, GA_Offset
    template <typename T>
    bool		evaluateMesh(const UT_FloatArray &uvals,
				     const UT_FloatArray &vvals,
				     const GA_ROGenericHandleVertex<T> &h,
				     T *pos,
				     bool transpose = false,
				     unsigned du = 0, unsigned dv = 0) const;

    // Compute the CVs of the U or V isoparametric curve running through
    // the given parametric value. The number of resulting CVs equals the
    // number or rows (if v = const.) or cols (if u = const.) respectively.
    // We return 0 if successful and -1 otherwise. Use these methods to
    // extract isoparms.
    int                 computeUConstCVs(float u, GEO_Face *results,
                                         const GA_AttributeRefMap &map,
                                         int uoffset=-1) const;

    int 		computeUConstCVs(float u, UT_Vector4 *cvs, 
				         int uoffset=-1) const;

    int                 computeVConstCVs(float v, GEO_Face *results,
                                         const GA_AttributeRefMap &map,
                                         int voffset=-1) const;
    int 		computeVConstCVs(float v, UT_Vector4 *cvs, 
				         int voffset=-1) const;

    // Create a profile curve (curve on surface) at the given isopam value:
    // Return 0 if successful and -1 otherwise. growtip is a value by which
    // we grow the tips of the iso line. It's useful to have it non-zero
    // when trimming.
    int			createUConstProfile(float u, float growtip = 0.0f);
    int			createVConstProfile(float v, float growtip = 0.0f);

    // Map the given value, defined in our domain, to a value in a chord/arc
    // length domain computed from our columns (u) or rows (v). Return -1 if
    // failed and 0 if successful. u and v must live in the valid interval.
    // Compute the chord-length parameterized basis in U or V for a given
    // location in our valid interval. Return 0 if OK and -1 otherwise. ubasis
    // and vbasis will be resized inside these methods if necessary.
    int			chordUBasis(float v, GA_Basis &ub, int voff=-1) const;
    int			chordVBasis(float u, GA_Basis &vb, int uoff=-1) const;

    // Evaluate the unit normal at (u,v) or at basis indices (iu,iv).
    // Return 0 if successful, and -1 otherwise.
    virtual int 	evaluateNormal(float u,float v, UT_Vector3 &nml) const;
    virtual int 	normalIndex(float iu, float iv, UT_Vector3 &nml) const;

    // Evaluate the curvature at (u,v).
    // Return 0 if successful, and -1 otherwise.
    int 		curvature(float u, float v, float &curv) const;

    // Evaluate the arc length of the surface within a range. If the
    // values are invalid, return -1. divs means divisions per span.
    float		arcLength(float u0, float v0, float u1, float v1,
				  int divs = 10) const;

    // Given a CV index figure out the min/max indicies of the knots between
    // which the curve needs to be re-evaluated if the CV changes. If not
    // given a valid CV index, the method returns -1. Otherwise it returns 0.
    virtual int		domainRangeOfCV(int i, int j, int &minuk,int &maxuk,
					int &minvk,int &maxvk) const=0;

    // Given a CV index figure out the min/max breakpoints which are
    // affected if the CV changes.  If not given a valid CV index, the
    // method returns -1.  Otherwise it returns 0.  Also returns -1 if
    // no breakpoints are affected.
    // NOTE: use % breakCount since maxbkp may be >= breakCount
    virtual int         breakpointRangeOfCV(int i, int j, int &minbkpu,
                                            int &maxbkpu, int &minbkpv,
					    int &maxbkpv) const = 0;

    // Reparameterize the surface by changing its basis. This type of 
    // reparameterization is generally NOT shape preserving:
    virtual void	reparameterizeU(GA_ParameterizationType ptype) = 0;
    virtual void	reparameterizeV(GA_ParameterizationType ptype) = 0;

    // Reverse rows (V) or columns (U). Reversing in either direction will
    // flip the normal.
    virtual void	reverseU(void);
    virtual void	reverseV(void);

    // Remap the basis and possibly the profiles too. The origin and length
    // apply to the whole length of the basis, not just to the valid interval.
    void		mapUBasis(float orig, float len);
    void		mapVBasis(float orig, float len);

    // Return the bounds of the valid evaluation interval in domain space:
    // For meshes this refers to cv indices.
    virtual void	validURange(float &ua, float &ub) const;
    virtual void	validVRange(float &va, float &vb) const;
    virtual void	validUInterval(int &a, int &b) const;
    virtual void	validVInterval(int &a, int &b) const;

    /// Compute the texture coordinates either uniformly of chord length.
    /// If ptattrib is true we deal with points, else with vertices.
    /// Returns false iff problems.
    bool		uniformTexture (const GA_RWHandleV3 &txth, bool ptattrib);
    bool		grevilleTexture(const GA_RWHandleV3 &txth, bool ptattrib);
    bool		chordLenTexture(const GA_RWHandleV3 &txth, bool ptattrib);

    // Compute the UV anc CV images of each Greville point.
    // resize is expensive: try to avoid it if possible.
    // Returns true if success
    bool                buildGrevilles(UT_RefMatrix<GEO_Greville> &dest) const;

    // Compute the _approximate_ first order derivative at
    // greville[uIdx,vIdx]. None of these methods checks the boundary
    // conditions on the two indices, so be careful.
    // They return true iff successful.
    bool	duApprox(const UT_RefMatrix<GEO_Greville> &grevilles,
                         int uIdx, int vIdx, UT_Vector3 &du,
                         int normalize=1, float precision=0.008F) const;
    bool	dvApprox(const UT_RefMatrix<GEO_Greville> &grevilles,
                         int uIdx, int vIdx, UT_Vector3 &du,
                         int normalize=1, float precision=0.008F) const;

    // Set the coordinates of cv[r,c].
    // The method returns 0 if OK and -1 if the indices are wrong.
    int			setCV(unsigned r, unsigned c, const UT_Vector4 &v);

    // Take the weights into consideration or don't. If you do, the surface
    // becomes a rational, and possibly different algorithms apply. If 'onOff'
    // is true, this function first checks if any two weights are different
    // before setting the rational flag to TRUE; if they are all the same,
    // the surface is set to non-rational (=> faster evaluation). weights()
    // calls normalizeWeights() to bring the weights to standard form.
    // Weights that are <= 0 are bumped up to FLT_EPSILON.
    void		weights(unsigned short onOff);

    // Set the weight of cv[r,c] to w. If the indices are out of bounds, it
    // returns -1; otherwise it returns 0. Weights that are <= 0 are bumped
    // up to FLT_EPSILON.
    int			setWeight(unsigned int r, unsigned int c, float w);

    // Get the weight of cv[r,c]. Return -1 if indices are wrong.
    float		getWeight(unsigned int r, unsigned int c) const;

    // Find out whether the surface is currently computed as a rational:
    int			isRational() const;

    // Go from a normalized domain ([0,1]) to the real domain or vice versa.
    // We look only at the valid interval.
    virtual void	unitToRealDomain(float  u_unit, float  v_unit,
					 float &u_real, float &v_real) const;
    virtual void	realToUnitDomain(float  u_real, float  v_real,
					 float &u_unit, float &v_unit) const;

    // Calculate the real domains for n consecutive operations on the domain
    // given n normalized domains and the operation
    virtual void	unitToRealSequenceU(float *uunit, float *ureal, 
				int ulen) const;

    virtual void	unitToRealSequenceV(float *vunit, float *vreal, 
				int vlen) const;

    // Normalize the weights of the control mesh so that edge weights are 1,
    // and all weights are >= 0 (those that are <= 0 are bumped up to 
    // FLT_EPSILON).
    void		normalizeWeights();

    // Normalize the domain and optionally shift it to a new origin. Use the 
    // given length if greater than 0.
    void		normalizeDomain(float len = 0.F,float *newuorigin = 0,
							float *newvorigin = 0);

    // This function will refine the area bounded by "rect" to have divisions
    // at least the size of "tol". "rect" passes in the boundary of the 
    // surface that contains the curve to be deformed, and it passes out the 
    // boundary that was actually defined (the latter is slightly bigger).
    // It returns (via reference parameters uunits, vunits, udivs, vdivs) the
    // float arrays and the size of the float array.  Please delete the
    // array pointed to by uunits and vunits after use.  It also returns an
    // array of original breakpoints (via uunrefineranges and vunrefineranges)
    // where all the original breakpoints with some newly inserted nodes between
    // them are returned.
    void		refineRange(const UT_BoundingRect &rect, float tol, 
				    float *&uunits, float *&vunits, 
				    int &udivs, int &vdivs,
				    UT_FloatArray &uunrefineranges,
				    UT_FloatArray &vunrefineranges);

    // Refine an area around the boundary of the surface, called the "belt".
    void		refineBelt(float ubwidth = 0.4F, float vbwidth = 0.4F,
				   int   ubdivs  = 2,   int   vbdivs  = 2);

    // Set or query the U and V bases respectively. The "set" functions 
    // overwrite the current basis pointer, so use with care; they return
    // -1 if the bases are invalid.
    int			setUBasis(GA_Basis *ub)
			{
			    if (ub && ub->checkValid((int)getNumCols(), (int)isWrappedU()))
			    {
				delete myUBasis; myUBasis = ub;
				return 0;
			    }
			    else return -1;
			}
    int			setVBasis(GA_Basis *vb)
			{
			    if (vb && vb->checkValid((int)getNumRows(), (int)isWrappedV()))
			    {
				delete myVBasis; myVBasis = vb;
				return 0;
			    }
			    else return -1;
			}
    GA_Basis*		getUBasis(void) const	{ return myUBasis; }
    GA_Basis*		getVBasis(void) const	{ return myVBasis; }

    // Query the order of the u and respectively v bases.
    unsigned 		getUOrder(void) const	{ return (unsigned)myUBasis->getOrder(); }
    unsigned 		getVOrder(void) const	{ return (unsigned)myVBasis->getOrder(); }

    // Query the dimension of the u and respectively v bases.
    unsigned 		getUDim(void) const { return (unsigned)myUBasis->getDimension(); }
    unsigned 		getVDim(void) const { return (unsigned)myVBasis->getDimension(); }

    // Gets the current U and V grevilles of the surface
    // User Beware: If the surface is subsequently changed, i.e. if the basis
    //		    changed, refined, etc., the results from this method
    //		    will be useless.
    void		getUGrevilles(UT_FloatArray &ugrevilles) const;
    void		getVGrevilles(UT_FloatArray &vgrevilles) const;

    // Inherited from the base class: just checks the validity of the bases.
    virtual bool	isDegenerate(void) const;

    // Get the bounding box for a specific range of u and v
    virtual void	getRangeBBox(const UT_Interval &u,
					const UT_Interval &v,
					UT_BoundingBox &bbox,
					const GA_PwHandleRO &h) const = 0;

    // Dehomogenize data (with attributes):
    static void		dehomogenizeData(GA_Offset *vertices,
				    GA_AttributeRefMap &hlist,
				    int count);
    static void		dehomogenizeData(UT_Vector4 *pos, int count);

    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    int		raiseOrderU	  (int neworder)
				    { return raiseOrderUInt(neworder); }
    int		raiseOrderV	  (int neworder)
				    { return raiseOrderVInt(neworder); }
    int		raiseOrderU(int neworder,
				GA_AttributeRefMap &map)
				    { return raiseOrderURefMap(neworder, map); }
    int		raiseOrderV(int neworder,
				GA_AttributeRefMap &map)
				    { return raiseOrderVRefMap(neworder, map); }


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
					     GEO_Delta *geodelta = 0) = 0;

    virtual int		transformBreakpoints(const UT_IntArray &uindices,
					     const UT_IntArray &vindices,
					     const UT_Matrix4 &matx,
					     int fixbkpts = 1,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0) = 0;

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
			        int unrefine = 1, GA_PointGroup *ptgroup=0);
    virtual int		attachV(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int unrefine = 1, GA_PointGroup *ptgroup=0);

    // If [ustart,ustop] and [vstart,vstop] are in the valid interval, return
    // the part of the surface delimited by those values in a new primitive.
    // Return 0 if a problem is encountered.
    virtual GEO_TPSurf		*extract(float ustart, float ustop,
					 float vstart, float vstop) const = 0;

    // Derived from the base class. Here we just want to preserve our basis.
    virtual void	subdivide(int numdivs, GA_PointGroup *ptgroup=0);

    // Return the surrounding values of the real-space u,v parameters.
    // Returns 1 if succesful, 0 if out-of-range.
    virtual int         parametricBBox(float u, float v,
				       float *u0, float *u1,
				       float *v0, float *v1);
    
    // Find out if the surface has any profiles in its domain:
    int			hasProfiles(void) const;

    // Get the profiles detail. Create the detail if not there if the create
    // option is true.
    GEO_Profiles	*profiles(int create = 1); 
    GEO_Profiles	*profiles(void) const	{ return theProfiles; }

    // find out if the surface is pasted:
    int			isPasted(void) const	{ return thePasteSurf != 0; }

    // Get or set the pasted image of this surface:
    GEO_PasteSurf	*pastedSurface(void) const { return thePasteSurf; }
    void		 pastedSurface(GEO_PasteSurf *p) { thePasteSurf = p; }

    // Update the displacement of the CV(s) whose point is ptoff if the
    // surface is pasted. This method wil not update the pasted dependents
    // unless you want it to. Return 1 of found and 0 otherwise.
    int			 updateDisplacement(GA_Offset ptoff,
					    int updatekids = 1);

    virtual void	 isolate(void);

    /// Stash (deactivate) or unstash (reactivate) the primitive.
    virtual void	stashed(bool beingstashed,
				GA_Offset offset = GA_INVALID_OFFSET);

    /// @{
    /// Method to load/save basis values.
    bool		jsonSaveUBasis(UT_JSONWriter &w) const;
    bool		jsonSaveUBasis(UT_JSONValue &v) const;
    bool		jsonSaveVBasis(UT_JSONWriter &w) const;
    bool		jsonSaveVBasis(UT_JSONValue &v) const;
    bool		jsonLoadUBasis(UT_JSONParser &p);
    bool		jsonLoadUBasis(UT_JSONParser &p, const UT_JSONValue &v);
    bool		jsonLoadVBasis(UT_JSONParser &p);
    bool		jsonLoadVBasis(UT_JSONParser &p, const UT_JSONValue &v);
    /// @}

    // Constraint based manipulation.
    // If the parameterization and derivative constraints do not change and
    // the curve's basis and order don't change, then the system only needs to
    // be solved once.  The solution can then be applied multiple times,
    // with different changes at the given parameters.

    // Return 0 if the resulting system is overdetermined (or otherwise
    //          unsolvable).
    //        1 if ok.
    int			 solveConstraints(const UT_Vector &uparam,
					  const UT_Vector &vparam,
					  const UT_IntArray &udervs,
					  const UT_IntArray &vdervs,
					  UT_MatrixF &soln,
					  UT_IntArray &cv_idx);

    // Return 0 if the given data is invalid (incorrect dimensions or indices).
    //        1 if ok.
    // The changes matrix is an n x 3 matrix, with each of the columns
    // corresponding to the x, y, and z coordinates.
    // If the surface is rational, the changes matrix will be modified.
    int			 applyConstraints(UT_MatrixF &changes,
					  const UT_Vector &uparam,
					  const UT_Vector &vparam,
					  const UT_IntArray &udervs,
					  const UT_IntArray &vdervs,
					  const UT_IntArray &cv_index,
					  const UT_MatrixF &soln);

    // Return 0 if the resulting system is overdetermined.
    //        1 if ok.
    // Only call this method when the solution cannot be reused.
    // If the surface is rational, the changes matrix will be modified.
    int			 solveAndApplyConstraints(const UT_Vector &uparam,
						  const UT_Vector &vparam,
						  const UT_IntArray &udervs,
						  const UT_IntArray &vdervs,
						  UT_MatrixF &changes);

    /// Elevate the curve to the given order. Return 0 if OK and -1 otherwise.
    int			elevateOrderU(int order);
    int			elevateOrderV(int order);

    virtual int  intersectSurf(GEO_TPSurf &surf2,
                               GU_IsectCurveSet &curveset,
                               float worldtol = 1e-4F,
                               float domaintol = 1e-2F,
                               int steps = 100,
                               bool docompact = true) = 0;
    virtual int  doesIntersect(const GEO_Primitive &prim,
                               float worldtol = 1e-4F, int nontrivial = 0) const = 0;

    /// This is more powerful than convertNew. It always returns a NEW
    /// object, so free it when you're done with it. It may return a mesh,
    /// a NURB surface or a Bezier surface depending on the type.
    virtual GEO_Hull	*reconfigure(unsigned type, int orderu, int orderv,
				     bool openu, bool openv,
				     bool endsu, bool endsv) const = 0;

    /// Construct a curve that has our characteristics in u or v. The
    /// CVs of the new curve are not set.
    /// @{
    virtual GEO_Curve   *buildRowCurve(bool appendPoints,
	    			       GEO_Detail *parent) const = 0;
    virtual GEO_Curve   *buildColCurve(bool appendPoints,
	    			       GEO_Detail *parent) const = 0;
    /// @}

    /// Cut a wedge of the primitive given a domain range
    /// ind1 and ind2 are indices to the refined values
    /// They are updated if negative on input, else used as is.
    /// If keep is zero the curve is only refined and the indices
    /// updated.
    virtual GEO_TPSurf	*cutU(float u1, float u2, int &ind1, int &ind2,
				int keep) = 0;

    virtual GEO_TPSurf	*cutV(float v1, float v2, int &ind1, int &ind2,
				int keep) = 0;

    /// Open the primitive at the given domain value
    virtual void	 openAtU(float u) = 0;
    virtual void	 openAtV(float v) = 0;

    /// Puts the position of the breakpoint at (uidx, vidx) in p
    /// Returns 1 if uidx and vidx are both valid, 0 otherwise.
    bool		 interpretBreakpoint(int uidx, int vidx, UT_Vector3 &p) const;

    /// Compute the CVs of the U or V isoparametric curve running through
    /// the given parametric value. The number of resulting CVs equals the
    /// number or rows (if v = const.) or cols (if u = const.) respectively.
    /// We return the extracted curve if successful and 0 otherwise.
    /// @{
    GEO_Curve           *extractUIsoparm(float u, int uoffset=-1)
    { return extractUIsoparm(u, uoffset, getParent()); }
    GEO_Curve           *extractUIsoparm(float u, int uoffset,
                                         GEO_Detail *parent) const;
    GEO_Curve           *extractUIsoparm(float u, const GA_AttributeRefMap &map,
                                         int uoffset=-1)
    { return extractUIsoparm(u, map, uoffset, getParent()); }
    GEO_Curve           *extractUIsoparm(float u, const GA_AttributeRefMap &map,
                                         int uoffset, GEO_Detail *parent) const;
    GEO_Curve           *extractVIsoparm(float v, int voffset=-1)
    { return extractVIsoparm(v, voffset, getParent()); }
    GEO_Curve           *extractVIsoparm(float v, int voffset,
                                         GEO_Detail *parent) const;
    GEO_Curve           *extractVIsoparm(float v, const GA_AttributeRefMap &map,
                                         int voffset=-1)
    { return extractUIsoparm(v, map, voffset, getParent()); }
    GEO_Curve           *extractVIsoparm(float v, const GA_AttributeRefMap &map,
                                         int voffset, GEO_Detail *parent) const;
    /// @}

    /// remove a single curve in the domain of the hull
    /// @{
    GEO_Curve           *extractU(float unitu)
    { return extractU(unitu, getParent()); }
    GEO_Curve           *extractU(float unitu, GEO_Detail *parent) const;
    GEO_Curve           *extractV(float unitv)
    { return extractV(unitv, getParent()); }
    GEO_Curve           *extractV(float unitv, GEO_Detail *parent) const;
    /// @}

protected:
    // The bases in each parametric direction. Not const because we want
    // to be able to (re)set them at initialization time or elsewhere.
    // They must be set with objects allocated from the heap so that we can
    // delete them when calling this class's destructor.
    GA_Basis	       *myUBasis;
    GA_Basis	       *myVBasis;

    // PROTECTED METHODS:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    {
				return static_cast<GA_PrimitiveFamilyMask>(
					    GEO_Hull::buildFamilyMask() |
					    GEO_FAMILY_TPSURF
					);
			    }

    // Declare intrinsic attribute functions
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE);

    // Load and save functions redefined from the parent class.
    virtual bool	savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool	saveExtraH9  (std::ostream &, bool binary) const;
    virtual bool	loadPrivateH9(UT_IStream &is);
    virtual bool	loadExtraH9  (UT_IStream &);

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    virtual bool	validate(void) const;

    // Used by the derived classes in their attach() methods.
    void		mergeAttachProfilesU(const GEO_TPSurf &tpsurf);
    void		mergeAttachProfilesV(const GEO_TPSurf &tpsurf);

    // Compute an evalution stepsize that is surely valid.
    float 		stepSize(float start, float stop, int count) const
    			{
			    return (count > 0) ? ((stop - start)/(float)count)
					       : 0.0F;
			}

    // Get a new basis of a type that matches our type:
    virtual GA_Basis	*newBasis(void) const = 0;
 
    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored when dealing
    // with one-dimensional types such as circles and polygons. Return 0 if
    // OK and -1 otherwise.
    virtual bool	 evaluatePointRefMap( GA_Offset result_vtx,
				    GA_AttributeRefMap &hlist,
				    fpreal u, fpreal v, uint du, uint dv) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const;

    // Evaluate one point (when du=dv=0), or the du-th dv-th derivative.
    // Return 0 if successful, and -1 otherwise.
    // The difference between this method and evaluate() is that this one
    // treats u and v as values between 2 knot indices, while evaluate()
    // sees them as actual domain values.
    virtual bool	evaluateIndexRefMap(fpreal u, fpreal v, GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				unsigned du, unsigned dv) const;
    virtual int 	evaluateIndexV4(float iu, float iv, UT_Vector4 &pos, 
				 unsigned du=0, unsigned dv=0) const;
    
    // Refine for all spans with specified number of divisions per span.
    virtual void	spanRefineURefMap(GA_AttributeRefMap &map, int ndivs=1);
    virtual void	spanRefineUInt(int numdivs=1);
    virtual void	spanRefineVRefMap(GA_AttributeRefMap &map, int ndivs=1);
    virtual void	spanRefineVInt(int numdivs=1);
 
    virtual int		raiseOrderUInt	  (int neworder) = 0;
    virtual int		raiseOrderVInt	  (int neworder) = 0;
    virtual int		raiseOrderURefMap(int neworder,
				GA_AttributeRefMap &map) = 0;
    virtual int		raiseOrderVRefMap(int neworder,
				GA_AttributeRefMap &map) = 0;

    /// Report approximate memory usage for myUBasis, myVBasis,
    /// theProfiles, since the subclasses don't have access to some.
    int64 getBaseMemoryUsage() const;

    // This is called by the subclasses to count the
    // memory used by myUBasis, myVBasis, theProfiles
    void countBaseMemory(UT_MemoryCounter &counter) const;

private:
    // Method to copy the basis of the source to me
    void			 copyBasisAndProfiles(const GEO_TPSurf *src);

    // Detail of (trimming) profiles:
    GEO_Profiles		*theProfiles;

    // The pasted image:
    GEO_PasteSurf		*thePasteSurf;

    friend std::ostream	&operator<<(std::ostream &os, const GEO_TPSurf &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};

#endif
