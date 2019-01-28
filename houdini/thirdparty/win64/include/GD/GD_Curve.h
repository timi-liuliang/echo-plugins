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

#ifndef __GD_Curve_h__
#define __GD_Curve_h__

#include "GD_API.h"
#include <GA/GA_Basis.h>
#include "GD_Face.h"
#include "GD_PrimType.h"
#include <iosfwd>

class UT_MemoryCounter;

class GD_API GD_Curve : public GD_Face
{
public:
    // Constructor that attaches this curve to detail "d". 
    GD_Curve(GD_Detail *d, GA_Offset offset = GA_INVALID_OFFSET);

    // Class destructor, virtual by inheritance. It assumes it is OK to
    // delete the basis, so make sure you always set the basis with an object
    // allocated from the heap.
    virtual ~GD_Curve();

    /// Report memory usage
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// @{
    /// Return the basis for the face (or @c NULL if there is no basis)
    virtual GA_Basis		*getFaceBasis();
    virtual const GA_Basis	*getFaceBasis() const;
    /// @}

    /// @{
    /// Load basis from a JSON value (from GD_Face)
    virtual bool		 jsonLoadBasis(UT_JSONParser &p);
    virtual bool		 jsonSaveBasis(UT_JSONWriter &w) const;
    virtual bool		 jsonLoadBasis(UT_JSONParser &p,
						const UT_JSONValue &v);
    virtual bool		 jsonSaveBasis(UT_JSONValue &v) const;
    /// @}

    // Overwrite this curve with the data from src. Virtual by inheritance.
    virtual GD_Primitive *copy(int preserve_shared_pts = 0) const;
    virtual int		  copy(const GD_Primitive &src, int ptoffset);

    virtual void	  copyUnwiredForMerge(const GA_Primitive *src,
					      const GA_MergeMap &map);

    // Return the bounds of the valid evaluation interval in domain space:
    virtual void	validInterval(int   &a,  int   &b ) const;
private:
    virtual void	validRangeImpl(fpreal64 &ua, fpreal64 &ub) const;
public:

    // Evaluate one point (when du=0), or the du-th derivative.
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluate(float u, UT_Vector3 &pos,
				 unsigned du=0, int uOffset=-1) const;


    // Stub for now
    virtual bool	evaluate(GA_Offset vtx, GA_AttributeRefMap &h,
				fpreal u, uint du=0, int uoffset=-1) const;

    // Given a domain value (u), store all the basis derivatives (from 0 to du
    // inclusive) into bmatx. Return the first index of the CVs needed for the
    // linear combination in cvoffset.  The CV index may exceed the number of
    // vertices if the curve is wrapped, so remember to use modulus (%).
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluateBasisDerivs(fpreal u,
					    fpreal bmatx[][GA_MAXORDER],
					    int &cvoffset, unsigned du = 0,
					    int uoffset = -1) const = 0;

    // Evaluate the basis at the given point in the domain and also return
    // the index of the first CV to linearly combine the basis with in cvoffset.
    // The CV index may exceed the number of vertices if the curve is wrapped,
    // so remember to use modulus (%). This method handles both rational and
    // non-rational curves.
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluateBasis(fpreal u, fpreal *ubvals,
				      int &cvoffset, unsigned du=0,
				      int uoffset=-1) const = 0;

    // Return the value of the i'th basis at parameter u. This method handles
    // both rational an non-rational curves.
    fpreal		computeBValue(float u, int i) const;

    // Evaluate the curvature at (u). Return 0 if successful and -1 otherwise.
    int 		curvature(float u, UT_Vector2 &curv) const;

    // Evaluate the curve betw. breakpoints by taking a start and a stop index
    // in the valid knot domain and an lod representing number of points to
    // be interpolated between every two breakpoints. The method ALWAYS
    // interpolates the encountered breakpoints (aka "edit points").
    // This returns true if successful, else false.  Please save
    // yourself headaches and pass VALID start and end indices (see the
    // method getValidInterval() in GA_Basis).
    virtual bool	evaluateBreakSegm(int uStartIdx, int uStopIdx,
			    int lod, UT_Vector3Array &pos, unsigned du=0) const;

    /// Implementation for curves
    virtual int		evaluateBreakSegment(int ustart, int uend,
			    int lod, GA_Offset *vertices,
			    GA_AttributeRefMap &h, uint du=0) const;

    // Given a CV index figure out the min/max indicies of the knots between
    // which the curve needs to be re-evaluated if the CV changes. If not
    // given a valid CV index, the method returns -1. Otherwise it returns 0.
    virtual int		domainRangeOfCV(int cvidx, int &mink,
					int &maxk) const = 0;

    // Take the weights into consideration or don't. If you do, the curve
    // becomes a rational, and possibly different algorithms apply. If 'onOff'
    // is true, this function first checks if any two weights are different
    // before setting the rational flag to TRUE; if they are all the same,
    // the curve is set to non-rational (=> faster evaluation). weights()
    // calls normalizeWeights() to bring the weights to standard form, and
    // invokes recordChanges() if necessary. Weights that are <= 0 are bumped
    // up to FLT_EPSILON.
    virtual void	weights(unsigned short onOff);

    // Normalize the weights of the control mesh so that edge weights are 1,
    // and all weights are >= 0 (those that are <= 0 are bumped up to 
    // FLT_EPSILON).
    void		normalizeWeights();

    // Normalize the domain and optionally shift it to a new origin. Use the 
    // given length if greater than 0.
    void		normalizeDomain(fpreal len=0, fpreal *neworigin=0)
			{
			    myBasis->normalize(len, neworigin);
			}

    // Set or query the basis. The "set" function  overwrites the current
    // basis pointer, so use with care; it returns -1 if the basis is invalid.
    int			setBasis(GA_Basis *ub)
			{
			    if (ub && ub->checkValid((int)getVertexCount(),
					      (int)isClosed()))
			    {
				delete myBasis;
				myBasis = ub;
				return 0;
			    }
			    else return -1;
			}
    const GA_Basis	*getBasis(void) const	{ return myBasis; }
    GA_Basis		*getBasis(void) 	{ return myBasis; }

    // Query the order and the dimension of the basis.
    virtual unsigned	getOrder(void) const
			    { return myBasis->getOrder(); }
    unsigned 		getDim(void) const
			    { return myBasis->getDimension(); }

    // Inherited from the base class: just checks the validity of the basis.
    virtual bool	isDegenerate(void) const;

    // Reverses the vertices of a given face. It's virtual because some faces,
    // such as rational splines, need to be aware of it.
    virtual void	reverse();

    // Find out whether the curve is currently computed as a rational:
    bool		isRational(void) const { return myRational; }

protected:
    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_AttributeIntrinsic
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveDefinition &definition)
			{ return GD_Face::registerIntrinsics(definition); }

    // Load and save functions redefined from the parent class.
    virtual bool	 savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool	 loadPrivateH9(UT_IStream &is);

    // Get a new basis of a type that matches our type:
    virtual GA_Basis	*newBasis() const = 0;

    // Check the validity of the data. Meant to be called especially at loading
    // time, since it also checks the weights. The method returns 1 if OK and 
    // 0 if trouble.
    virtual bool	 validate(void) const;

    // Set the order of the basis:
    void		 setOrder(unsigned ord)	{ myBasis->setOrder(ord); }

    // Build the vextex list, and optionally append points to the detail.
    // Return 0 if OK, and -1 if error.
    int 	 	create(GD_Curve *crv, int nelems, int closed,
					      int appendPoints = 1);
    virtual int		breakCount() const;

protected:
    /// Curve's basis
    GA_Basis		*myBasis;

private:
    // Flag that indicates whether the CV weights should be taken into account
    // when evaluating the curve or otherwise affecting its geometry.
    bool		myRational;

    friend std::ostream	&operator<<(std::ostream &os, const GD_Curve &d)
			{
			    d.saveH9(os, 0,
				     GD_Primitive::theEmptySaveAttribs,
				     GD_Primitive::theEmptySaveAttribs);
			    return os;
			}
};

#endif
