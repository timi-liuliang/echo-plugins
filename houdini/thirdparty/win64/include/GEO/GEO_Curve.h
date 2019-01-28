/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class handles curves of arbitrary degree.
 *      The Face class maintains the control vertices for the curve.
 *
 */

#ifndef __GEO_Curve_h__
#define __GEO_Curve_h__

#include "GEO_API.h"
#include "GEO_Face.h"
#include "GEO_Greville.h"
#include "GEO_PrimType.h"

#include <GA/GA_Basis.h>
#include <GA/GA_GenericHandle.h>

#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Interval.h>
#include <UT/UT_Matrix.h>

class GEO_Delta;
template<bool isconst> class GA_PwHandle;
typedef GA_PwHandle<true> GA_PwHandleRO;

class GEO_API GEO_Curve : public GEO_Face
{
public:
    using GEO_Face::evaluate;
    using GEO_Face::evaluatePoint;
    using GEO_Face::evaluateMesh;

protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    inline GEO_Curve(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Face(d, offset), myBasis(NULL)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    /// It assumes it is OK to delete the basis, so make sure you always
    /// set the basis with an object allocated from the heap.
    virtual ~GEO_Curve()
    {
        delete myBasis;
    }

public:
    // Overwrite this curve with the data from src. Virtual by inheritance.
    virtual void		 copyPrimitive(const GEO_Primitive *src);
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;

    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);

    virtual bool	changePointRef(GA_Offset from, GA_Offset to);

    // Evaluate one point (when du=0), or the du-th derivative.
    // Return 0 if successful, and -1 otherwise.
    int		evaluateHomogeneous(float u, UT_Vector4 &pos,
				 unsigned du=0, int uOffset=-1) const;
    template <typename T>
    bool	evaluateHomogeneous(fpreal u,
				    const GA_ROGenericHandleVertex<T> &h,
				    T &result,
				    int du=0, int uOffset=-1) const;

    // Given a domain value (u), store all the basis derivatives (from 0 to du
    // inclusive) into bmatx. Return the min and max index of the CVs needed
    // for the linear combination. The indices may exceed the number of
    // vertices if the curve is wrapped, so remember to use % getVertexCount().
    virtual int		evaluateBasisDerivs(float u, float bmatx[][GA_MAXORDER],
					    int &cvoffset, unsigned du = 0,
					    int uoffset = -1) const = 0;

    // Evaluate the basis at the given point in the domain and also return
    // the index of the first CV to linearly combine the basis with. The CV
    // index may exceed the number of vertices if the curve is wrapped,
    // so remember to use modulus (%). This method handles both rational and
    // non-rational curves.
    virtual int		evaluateBasis(float u, float *ubvals, int &cvoffset,
				      unsigned du=0, int uoffset=-1) const = 0;

    // Return the value of the i'th basis at parameter u. This method handles
    // both rational an non-rational curves.
    float		computeBValue(float u, int i) const;

    virtual bool	evaluateMesh(const UT_FloatArray &uvals,
			    GA_Offset *results,
			    GA_AttributeRefMap &hlist, unsigned du=0) const
			{ return evaluateMesh(uvals,
				GA_ROGenericHandleVertex<GA_Offset>(hlist),
				results, du); }

    // Explicit instantiations are provided for float, UT_Vector3,
    // UT_Vector4, UT_Matrix3, UT_Matrix4
    template <typename T>
    bool		evaluateMesh(const UT_FloatArray &uvals,
			    const GA_ROGenericHandleVertex<T> &h, T *pos,
			    unsigned du=0) const;

    virtual bool	evaluateMesh(const UT_FloatArray &uvals,
			    UT_Vector4 *pos, unsigned du=0) const;

    // This method takes a start and a stop index in the valid domain, a lod
    // representing number of points between every two breakpoints, an integer
    // index, and return the value of the DOMAIN of point index after the
    // domain has lod points inserted between breakpoints.
    virtual float       breakSegmIndexToDomain(int ustartidx, int ustopidx,
                            int lod, int index) const;

    // Evaluate the unit normal at (u). Return 0 if successful, and -1 
    // otherwise.
    int 		evaluateNormal(float u, UT_Vector3 &nml) const;

    // Evaluate the curvature at (u). Return 0 if successful and -1 otherwise.
    int 		curvature(float u, UT_Vector3 &curv) const;

    // Evaluate the arc length of the curve within a range. If the
    // values are invalid, return -1. divs means divisions per span.
    // If use_frwd_diff is true, forward differencing is used to speed
    // up the computation of the arc length; if false, a more precise
    // but slower solution is computed.
    float		arcLength(float u0, float u1, bool use_frwd_diff = true,
				  int divs = 10) const;

    // compute the un-normalized lengths corresponding to points on the curve
    // evaluated at the valid knots parameter values
    // (ones with which lie within curve paramer domain) and store them
    // in the 'lengths' array. Returns the curve "length" (same as last knot).
    // The meaning of "length" depends on ptype, and can mean chord length,
    // square chord length, approximate arc length.
    virtual float	getKnotLengths(GA_ParameterizationType ptype, 
				       UT_FloatArray &lengths) const=0;

    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    int		 raiseOrder(int neworder, GA_AttributeRefMap &map)
		    { return raiseOrderRefMap(neworder, map); }
    int		 raiseOrder	  (int neworder)
		    { return raiseOrderInt(neworder); }

    // Translate the CVs such that the given breakpoint change positions by
    // the given delta.  Return -1 if something goes wrong, 0 if translation
    // was successful.
    // NOTE: uindices cannot contain any duplicates.  If the curve is closed,
    //       the first and last breakpoint are considered the same.
    virtual int          translateBreakpoints(const UT_IntArray &uindices,
                                              const UT_Vector3 &delta,
					      int fixbkpts = 1,
					      GA_PointGroup *ptgroup = NULL,
					      GEO_Delta *geodelta = 0)= 0;

    virtual int		 transformBreakpoints(const UT_IntArray &uindices,
					      const UT_Matrix4 &matx,
					      int fixbkpts = 1,
					      GA_PointGroup *ptgroup = NULL,
					      GEO_Delta *geodelta = 0)= 0;

    // Append another face to us in one of two ways: blend the two endpoints
    // or connect them straight or rounded. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert knots into the curves to minimize the affected areas; if 1,
    // no refinement is done. For the non-blend case, the tolerance will
    // generate a span whose shape goes from round to straight; 0 tolerance
    // means straight connection. If unrefine is on, we'll try to reduce the
    // complexity of the face if we're connecting rounded. We return 0 if OK
    // and -1 if error. Both curves must be open and have the same order.
    virtual int		attach(const GEO_Face &face, int blend = 1,
			       float bias = 0.5f, float tolerance = 1.0f,
			       int unrefine = 1, GA_PointGroup *ptgroup=0);

    // Given a CV index figure out the min/max indicies of the knots between
    // which the curve needs to be re-evaluated if the CV changes. If not
    // given a valid CV index, the method returns -1. Otherwise it returns 0.
    virtual int		domainRangeOfCV(int cvidx, int &mink,
					int &maxk) const = 0;

    // Given a CV index figure out the min/max breakpoints which are
    // affected if the CV changes.  If not given a valid CV index, the
    // method returns -1.  Otherwise it returns 0.  Also returns -1 if
    // no breakpoints are affected.
    // NOTE: use % breakCount since maxbkp may be >= breakCount
    virtual int		breakpointRangeOfCV(int cvidx, int &minbkp,
					    int &maxbkp) const = 0;

    // Reparameterize the curve by changing its basis. This type of 
    // reparameterization is generally NOT shape preserving:
    virtual void	reparameterize(GA_ParameterizationType ptype) = 0;

    // Compute the UV and CV images of each Greville point.
    // resize is expensive: try to avoid it if possible.
    // Returns true if success
    bool                buildGrevilles(UT_Array<GEO_Greville> &dest) const;

    // Set the coordinates of cv[r]
    // The method returns 0 if OK and -1 if the indices are wrong.
    int			setCV(unsigned r, const UT_Vector4 &v);

    // Take the weights into consideration or don't. If you do, the curve
    // becomes a rational, and possibly different algorithms apply. If 'onOff'
    // is true, this function first checks if any two weights are different
    // before setting the rational flag to TRUE; if they are all the same,
    // the curve is set to non-rational (=> faster evaluation). weights()
    // calls normalizeWeights() to bring the weights to standard form.
    // Weights that are <= 0 are bumped up to FLT_EPSILON.
    void		weights(unsigned short onOff);

    // Set the weight of cv[r] to w. If the indices are out of bounds, it
    // returns -1; otherwise it returns 0. Weights that are <= 0 are bumped
    // up to FLT_EPSILON.
    int			setWeight(unsigned int r, float w);

    // Get the weight of cv[r]. Return -1 if indices are wrong.
    float		getWeight(unsigned int r) const;

    // Find out whether the curve is currently computed as a rational:
    int			isRational() const;

    // Normalize the weights of the control mesh so that edge weights are 1,
    // and all weights are >= 0 (those that are <= 0 are bumped up to 
    // FLT_EPSILON).
    void		normalizeWeights();

    // Normalize the domain and optionally shift it to a new origin. Use the 
    // given length if greater than 0.
    void		normalizeDomain(float len = 0.0F, fpreal *neworigin=0)
			{
			    myBasis->normalize(len, neworigin);
			}

    // Go from a normalized domain ([0,1]) to the real domain or vice versa.
    // We look only at the valid interval.
    virtual float	unitToRealDomain(float u_unit) const;
    virtual float	realToUnitDomain(float u_real) const;

    // Calculate the real domains for n consecutive operations on the domain
    // given n normalized domains and the operation
    // Here the real domain ranges from 0 to vertexcount - 1 + isClosed().

    virtual void        unitToRealSequence(float *uunit, float *ureal, 
				int ulen) const;
    
    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve. The tolerance specifies max
    // error of the returned value (cannot be 0.0f)

    virtual float	 unitLengthToUnitDomain(float ulength,
						float tolerance = 1e-05F) const;
    virtual float	 unitToUnitLengthDomain(float  uparm) const;


    // Return the bounds of the valid evaluation interval in domain space:
    virtual void	validInterval(int &a, int &b) const;
    virtual void	validRange(float &ua, float &ub) const;

    /// Compute the texture coordinates either uniformly of chord length.
    /// If ptattrib is true, we deal with points, else with vertices.
    /// Returns false iff problems.
    bool		uniformTexture (const GA_RWHandleV3 &txth, bool ptattrib);
    bool		grevilleTexture(const GA_RWHandleV3 &txth, bool ptattrib);
    bool		chordLenTexture(const GA_RWHandleV3 &txth, bool ptattrib);

    // Reverses the vertices of a given face. It's virtual because some faces,
    // such as rational splines, need to be aware of it.
    virtual void	reverse();

    // Set or query the basis. The "set" function  overwrites the current
    // basis pointer, so use with care; it returns -1 if the basis is invalid.
    int			setBasis(GA_Basis *ub)
			{
			    if (ub && ub->checkValid((int)getVertexCount(),
						     (int)isClosed()))
			    {
				delete myBasis; myBasis = ub;
				return 0;
			    }
			    else return -1;
			}
    /// Set the basis to a copy of the passed-in basis.
    /// NOTE: basis *must* be valid for this curve!
    virtual void        setBasisCopy(const GA_Basis *basis) = 0;
    const GA_Basis	*getBasis(void) const	{ return myBasis; }
    GA_Basis		*getBasis(void)		{ return myBasis; }
    void		setAnyBasis(GA_Basis *ub) // Use with care!
			{
			    delete myBasis; myBasis = ub;
			}

    // Query the order and the dimension of the basis.
    virtual unsigned 	getOrder(void) const;
    unsigned 		getDim(void) const  { return (unsigned)myBasis->getDimension(); }

    // Inherited from the base class: just checks the validity of the basis.
    virtual bool	isDegenerate(void) const;

    // Get the bounding box for a specific range of u and v
    virtual void	getRangeBBox(const UT_Interval &u,
				     UT_BoundingBox &bbox,
				     const GA_PwHandleRO &h) const = 0;

    // Dehomogenize data (with attributes):
    static void		dehomogenizeData(GA_Offset *vertices,
					GA_AttributeRefMap &hlist,
					int count);
    static void		dehomogenizeData(UT_Vector4 *pos, int count);

    // Return the surrounding values of the real-space u,v parameters.
    // Returns 1 if succesful, 0 if out-of-range.
    virtual int		parametricBBox(float u, float v,
				       float *u0, float *u1,
				       float *v0, float *v1);

    // If ustart and ustop are two values in the valid interval,ustart < ustop,
    // return the part of curve defined by ustart and ustop in a new primitive.
    // Return 0 if a problem is encountered.
    virtual GEO_Curve		*extract(float ustart,float ustop) const = 0;
    virtual int         breakCount() const;

    /// @{
    /// Method to load/save basis values.  Defined from TriMesh
    virtual bool	jsonSaveBasis(UT_JSONWriter &w) const;
    virtual bool	jsonSaveBasis(UT_JSONValue &v) const;
    virtual bool	jsonLoadBasis(UT_JSONParser &p);
    virtual bool	jsonLoadBasis(UT_JSONParser &p, const UT_JSONValue &v);
    /// @}

    // Contraint based manipulation.
    // If the parameterization and derivative constraints do not change and
    // the curve's basis and order don't change, then the system only needs to
    // be solved once.  The solution can then be applied multiple times,
    // with different changes at the given parameters.

    // Return 0 if the resulting system is overdetermined or the dimensions
    //		are incorrect.
    //        1 if ok.
    int			 solveConstraints(const UT_Vector &param,
					  const UT_IntArray &dervs,
					  UT_MatrixF &soln,
					  UT_IntArray &cv_index);
    // Return 0 if the given data is invalid (incorrect dimensions or indices).
    //        1 if ok.
    // If the curve is rational, the changes matrix will be modified.
    int			 applyConstraints(UT_MatrixF &changes,
					  const UT_Vector &param,
					  const UT_IntArray &dervs,
					  const UT_IntArray &cv_index,
					  const UT_MatrixF &soln);

    // Return 0 if the resulting system is overdetermined.
    //        1 if ok.
    // Only call this method when the solution cannot be reused.
    // If the curve is rational, the changes matrix will be modified.
    int			 solveAndApplyConstraints(const UT_Vector &param,
						  const UT_IntArray &dervs,
						  UT_MatrixF &changes);

    /// Elevate the curve to the given order. Return 0 if OK and -1 otherwise.
    int                  elevateOrder(int order);

    /// This is more powerful than convertNew. It always returns a NEW
    /// object, so free it when you're done with it. It may return
    /// a NURB curve or a Bezier curve depending on the type.
    virtual GEO_Face	*reconfigure(unsigned type, int order, bool open,
				    bool interpends, bool nonrational) const=0;

    /// Cut a wedge of the primitive given a domain range
    /// ind1 and ind2 are indices to the refined values
    /// They are updated if negative on input, else used as is.
    /// If keep is zero the curve is only refined and the indices
    /// updated.
    virtual GEO_Curve	*cut(float u1, float u2,
			     int &ind1, int &ind2, int keep) = 0;

    /// Open the primitive at the given domain value
    virtual void	 openAt(float u) = 0;

    /// Puts the location of the breakpoint at index idx on the curve in p.
    /// Returns 1 if idx is valid, 0 otherwise.
    bool		 interpretBreakpoint(int idx, UT_Vector3 &p) const;

    /// Method for helping curve fitting on a set of data points.
    /// Given a set of data points dataPts[0..n][0..2].
    /// Compute the unit tangent vectors for local interpolation methods,
    /// and indicate possible corner points if needed.
    /// To specify that you want to detect corners, set the column
    /// dimensions of tangents to [0..5] (ie. tangents[0..n][0..5],
    /// to allow for 2 tangents (in/out) per point) 
    /// else set tangents[0..n][0..2].
    /// Output: tangents[0..n][0..2||5]. 
    static void		 computeDataPtsUnitTangents(const UT_MatrixF &dataPts,
						    UT_MatrixF &tangents);

    virtual void computeInteriorPointWeights(
		    UT_Array<GA_Offset> &vtxlist, UT_Array<float> &weightlist,
		    fpreal u, fpreal v, fpreal w) const;

protected:
    // The basis. Not const because we want to be able to (re)set it at
    // initialization time or elsewhere. It must be assigned an object
    // allocated from the heap so that we can delete it when calling this
    // class's destructor.
    GA_Basis	       *myBasis;
      
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    {
				return static_cast<GA_PrimitiveFamilyMask>(
					    GEO_Face::buildFamilyMask() |
					    GEO_FAMILY_CURVE
					);
			    }
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)

    // Load and save functions redefined from the parent class.
    virtual bool	savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool	loadPrivateH9(UT_IStream &is);
    virtual void	copyBasis(const GEO_Curve *src);

    // Get a new basis of a type that matches our type:
    virtual GA_Basis	*newBasis(void) const = 0;

    // Check the validity of the data. Meant to be called especially at loading
    // time, since it also checks the weights. The method returns 1 if OK and 
    // 0 if trouble.
    virtual bool	validate(void) const;

    // Set the order of the basis:
    void		 setOrder(unsigned ord)	{ myBasis->setOrder(ord); }

    // Compute an evalution stepsize that is surely valid.
    float 		stepSize(float start, float stop, int count) const
    			{
			    return (count > 0) ? ((stop - start)/(float)count)
					       : 0.0F;
			}
 
    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored here. Return 0 if
    // OK and -1 otherwise.
    virtual bool	 evaluatePointRefMap( GA_Offset result_vtx,
					GA_AttributeRefMap &hlist,
					fpreal u_unit, fpreal=0,
					uint du=0, uint = 0) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos,
					float u_unit, float=0,
					unsigned du=0, unsigned = 0) const;

    virtual bool	evaluateRefMap(fpreal u, GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				int du=0, int uOffset=-1) const;
    virtual int		evaluateV4(float u, UT_Vector4 &pos,
				 unsigned du=0, int uOffset=-1) const;

    virtual int		 raiseOrderRefMap(int neworder, 
					  GA_AttributeRefMap &map)=0;
    virtual int		 raiseOrderInt(int neworder)=0;

    /// Report approximate memory usage for myBasis, grevArray, myVertexList,
    /// since the subclasses don't have access to grevArray.
    int64 getBaseMemoryUsage() const;

    // This is called by the subclasses to count the
    // memory used by myBasis, grevArray, myVertexList
    void countBaseMemory(UT_MemoryCounter &counter) const;

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_Curve &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
