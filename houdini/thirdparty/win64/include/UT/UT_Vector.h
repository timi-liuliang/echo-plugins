/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Vector of arbitrary size (C++)
 *
 * COMMENTS:	From Numerical Recipes
 *		And contains a permutation vector class.
 *
 */

#ifndef	__UT_Vector_H__
#define	__UT_Vector_H__

#include "UT_API.h"
#include <stdlib.h>
#include <iosfwd>
#include <SYS/SYS_Types.h>
#include "UT_Assert.h"
#include "UT_ThreadedAlgorithm.h"

#include "UT_VectorTypes.h"

//////////////////////////////////////////////////////////////////////////////
//
// UT_VectorT
//

template <typename T>
class UT_VectorT
{
public:

    typedef T value_type;

    /// Input the index range [nl..nh].
    UT_VectorT() { myVector = 0; myOwnData = 0; }
    UT_VectorT(exint nl, exint nh);
    UT_VectorT(const UT_VectorT<T> &v);
    ~UT_VectorT();

    /// Initialize nl, nh and allocate space.
    void 	 init(exint nl, exint nh);

    /// Steal from another vector, resulting vector has origin at 1.
    void	 subvector(const UT_VectorT<T> &v, exint nl, exint nh);

    /// Assign from a float array into the designated subvector, performing a
    /// deep copy.
    void	 assign(const fpreal32 *data, exint nl, exint nh);
    void	 assign(const fpreal64 *data, exint nl, exint nh);

    int		 isInit() const { return myVector ? 1 : 0; }

    /// Initialize to zeros.
    void	 zero(exint nl, exint nh);
    void	 zero()
		    { zero(myNL, myNH); }

    /// These methods allow one to read out and write into subvectors
    /// of UT_Vector using our other vector classes.
    /// Keep in mind that the usual UT_Vector? methods are 0 based,
    /// while this class is usually 1 based.
    void	 getSubvector2(UT_Vector2 &v, exint idx) const;
    void	 setSubvector2(exint idx, const UT_Vector2 &v);
    void	 getSubvector3(UT_Vector3 &v, exint idx) const;
    void	 setSubvector3(exint idx, const UT_Vector3 &v);
    void	 getSubvector4(UT_Vector4 &v, exint idx) const;
    void	 setSubvector4(exint idx, const UT_Vector4 &v);

    /// Get the low index.
    exint 	 getNL() const { return myNL; }

    /// Get the high index.
    exint	 getNH() const { return myNH; }

    /// Get dimension of this vector
    exint	 length() const	{ return myNH - myNL + 1; }

    /// Determines if we have a large enough vector to make micro
    /// threading worthwhile.
    /// Experimentation shows with 4 procs on NT, 5000 is a cut off for good
    /// behaviour on the simplistic addScaledVec method.
    bool	 shouldMultiThread() const
		 {
#ifdef CELLBE
		     return false;
#else
		     return (myNH - myNL) > 4999;
#endif
		 }

    /// Determines the [start,end) interval that should be worked on
    void	 getPartialRange(exint &start, exint &end, const UT_JobInfo &info) const;

    /// Determines the [startblock,endblock) interval that should be worked on,
    /// where block 0 starts at myNL and all blocks are blocksize long, except
    /// possibly the last.
    void	 getPartialBlockRange(exint &startblock, exint &endblock,
                    const exint blocksize, const UT_JobInfo &info) const;

    /// Change the low index, and the high index will adjust itself.
    void	 changeNL(exint nl);

    /// Change the indices in a very shallow manner, i.e. simply change
    /// the index without shifting the data. This is highly dangerous, yet
    /// very useful if used with care.
    void	 setShallowNL(exint nl)	{ myNL = nl; }
    void	 setShallowNH(exint nh)	{ myNH = nh; }

    /// For retrieving data
    T		&operator()(exint i)
		 {
		     UT_ASSERT_P(i >= myNL && i <= myNH);
		     return myVector[i];
		 }
    T		 operator()(exint i) const
		 {
		     UT_ASSERT_P(i >= myNL && i <= myNH);
		     return myVector[i];
		 }

    /// Lp-norm
    /// type: 0  L-infinity norm    (ie. max abs)
    ///       1  L1-norm            (ie. sum of abs)
    ///       2  L2-norm            (ie. Euclidean distance)
    T		 norm(int type=2) const;

    /// Square of L2-norm
    T		 norm2() const;

    T		 distance2(const UT_VectorT<T> &v) const;

    /// Negate
    THREADED_METHOD(UT_VectorT, shouldMultiThread(), neg)
    void	 negPartial(const UT_JobInfo &info);
    THREADED_METHOD1(UT_VectorT, shouldMultiThread(), negPlus,
		    const UT_VectorT<T> &, v)
    void	 negPlusPartial(const UT_VectorT<T> &v, const UT_JobInfo &info);

    /// Add scaled vector
    THREADED_METHOD2(UT_VectorT, shouldMultiThread(), addScaledVec,
			T, s,
			const UT_VectorT<T> &, v)
    void	 addScaledVecPartial(T s, const UT_VectorT<T> &v,
				    const UT_JobInfo &info);

    /// Add scaled vector and compute the squared L2 norm
    void addScaledVecNorm2(T s, const UT_VectorT<T> &v, fpreal64 *norm2);

    /// Add scaled vector and compute the squared L2 norm,
    /// but only using components up to (but excluding) index normlimit
    /// for the norm computation.
    void addScaledVecNorm2UpTo(T s, const UT_VectorT<T> &v, fpreal64 *norm2, exint normlimit);

    /// Scale itself then add vector
    THREADED_METHOD2(UT_VectorT<T>, shouldMultiThread(), scaleAddVec,
			T, s,
			const UT_VectorT<T> &, v)
    void	 scaleAddVecPartial(T s, const UT_VectorT<T> &v,
				    const UT_JobInfo &info);

    /// Multiply two sources together, save to this.  Requires
    /// that three vectors already have the same size.
    THREADED_METHOD2(UT_VectorT<T>, shouldMultiThread(), multAndSet,
			const UT_VectorT<T> &, a,
			const UT_VectorT<T> &, b)
    void	multAndSetPartial(const UT_VectorT<T> &a,
			    const UT_VectorT<T> &b,
			    const UT_JobInfo &info);

    /// Multiply two sources together, save to this.  Requires
    /// that three vectors already have the same size.
    /// Computes dot(a*b, a)
    /// This strange seeming pair is useful for jacobian preconditioners.
    void multSetAndDotUpTo(const UT_VectorT<T> &a, const UT_VectorT<T> &b,
        fpreal64 *dot_aba, exint dotlimit);


    /// Divides two sources together, save to this.  Requires
    /// that three vectors already have the same size.
    THREADED_METHOD2(UT_VectorT<T>, shouldMultiThread(), divAndSet,
			const UT_VectorT<T> &, a,
			const UT_VectorT<T> &, b)
    void	divAndSetPartial(const UT_VectorT<T> &a,
			    const UT_VectorT<T> &b,
			    const UT_JobInfo &info);

    /// Inverts the source and saves to this.
    /// Requires that the vectors match.
    THREADED_METHOD1(UT_VectorT<T>, shouldMultiThread(), safeInvertAndSet,
			const UT_VectorT<T> &, a)
    void	safeInvertAndSetPartial(const UT_VectorT<T> &a,
			    const UT_JobInfo &info);

    THREADED_METHOD1(UT_VectorT<T>, shouldMultiThread(), invertAndSet,
			const UT_VectorT<T> &, a)
    void	invertAndSetPartial(const UT_VectorT<T> &a,
			    const UT_JobInfo &info);

    T		 dot(const UT_VectorT<T> &v) const;

    /// Multithreaded copy.  operator= uses this.
    THREADED_METHOD1(UT_VectorT<T>, shouldMultiThread(), copyFrom,
			const UT_VectorT<T> &, v)
    void	copyFromPartial(const UT_VectorT<T> &v,
			    const UT_JobInfo &info);

    /// Operators
    /// NOTE: operator= requires the destination be a matching size and
    /// layout!
    UT_VectorT	&operator= (const UT_VectorT<T> &v);
    UT_VectorT	&operator+= (const UT_VectorT<T> &v);
    UT_VectorT	&operator-= (const UT_VectorT<T> &v);

    /// Componentwise multiplication & division.
    UT_VectorT	&operator*= (const UT_VectorT<T> &v);
    UT_VectorT	&operator/= (const UT_VectorT<T> &v);

    /// Scalar multiplication and division.
    UT_VectorT	&operator*= (T scalar);
    UT_VectorT	&operator/= (T scalar);

    /// Equals
    ///   upls is the number of representable fpreal64's to accept between
    ///   each individual component. If you deal in fpreal32's and assign into
    ///   this fpreal64 class, then you will want a ulps that is scaled by
    ///   2^32. eg. for 50upls at fpreal32 precision, you will need 50*2^32
    ///   or approximately 1e+11.
    bool	 isEqual(const UT_VectorT<T> &v, int64 ulps);

    /// Output
    std::ostream        &save(std::ostream &os) const;
    friend std::ostream &operator<<(std::ostream &os, const UT_VectorT<T> &v)
                         { v.save(os); return os; }

    T		*getData() const	{ return &myVector[myNL]; }

    bool	 hasNan() const;
    void	 testForNan() const;

protected:
    /// A multithreaded norm function that returns the non-normalized
    /// norm (Ie, squared in the L2 case.) and is mulithreaded.
    THREADED_METHOD2_CONST(UT_VectorT, shouldMultiThread(), normInternal,
		    fpreal64 *, result,
		    int, type)
    void normInternalPartial(fpreal64 *result, int type, const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(UT_VectorT, shouldMultiThread(), distance2Internal,
		    fpreal64 *, result,
		    const UT_VectorT<T> &, v)
    void distance2InternalPartial(fpreal64 *result, const UT_VectorT<T> &v, const UT_JobInfo &info) const;

    /// A multithreaded dot method
    THREADED_METHOD2_CONST(UT_VectorT, shouldMultiThread(), dotInternal,
		    fpreal64 *, result,
		    const UT_VectorT<T> &, v)
    void dotInternalPartial(fpreal64 *result,
			    const UT_VectorT<T> &v, 
			    const UT_JobInfo &info) const;	

    THREADED_METHOD3(UT_VectorT, shouldMultiThread(), addScaledVecNorm2Internal,
        T, s,
        const UT_VectorT<T> &, v,
        fpreal64 *, norm2)
    void addScaledVecNorm2InternalPartial(T s, const UT_VectorT<T> &v,
        fpreal64 *norm2, const UT_JobInfo &info);

    THREADED_METHOD4(UT_VectorT, shouldMultiThread(), addScaledVecNorm2UpToInternal,
        T, s,
        const UT_VectorT<T> &, v,
        fpreal64 *, norm2,
        exint, normlimit)
    void addScaledVecNorm2UpToInternalPartial(T s, const UT_VectorT<T> &v,
        fpreal64 *norm2, exint normlimit, const UT_JobInfo &info);

    THREADED_METHOD4(UT_VectorT<T>, shouldMultiThread(), multSetAndDotUpToInternal,
        const UT_VectorT<T> &, a,
        const UT_VectorT<T> &, b,
        fpreal64 *, dot_aba,
        exint, dotlimit)
    void multSetAndDotUpToInternalPartial(
        const UT_VectorT<T> &a, const UT_VectorT<T> &b,
        fpreal64 *dot_aba, exint dotlimit,
        const UT_JobInfo &info);

    static const exint PARALLEL_BLOCK_SIZE = 1024;

private:
    exint 	 myNL, myNH;
    int		 myOwnData;
    T		*myVector;
};


//////////////////////////////////////////////////////////////////////////////
//
// UT_Permutation
//


template <typename T>
class UT_PermutationT
{
public:
     // Input the index range [nl..nh].
     UT_PermutationT(exint nl, exint nh);
    ~UT_PermutationT();

    // Assignment
    UT_PermutationT(const UT_PermutationT<T> &p);
    UT_PermutationT<T> &operator=(const UT_PermutationT<T> &p);

    // Initialize to zeros.
    void	 zero();

    // Get the low index.
    exint 	 getNL() const { return myNL; }

    // Get the high index.
    exint 	 getNH() const { return myNH; }

    exint	 length() const	{ return myNH - myNL + 1; }

    // Change the low index, and the high index will adjust itself.
    void	 changeNL(exint nl);

    // Change the indices in a very shallow manner, i.e. simply change
    // the index without shifting the data. This is highly dangerous, yet
    // very useful if used with care.
    void	 setShallowNL(exint nl)	{ myNL = nl; }
    void	 setShallowNH(exint nh)	{ myNH = nh; }

    // For retrieve data.
    T		&operator()(exint i)
		 {
		     UT_ASSERT_P(i >= myNL && i <= myNH);
		     return myVector[i];
		 }
    T	 	 operator()(exint i) const
		 {
		     UT_ASSERT_P(i >= myNL && i <= myNH);
		     return myVector[i];
		 }

private:
    void	 clone(const UT_PermutationT<T> &p);
    exint	 myNL, myNH;
    T		*myVector;
};

#if defined( WIN32 ) || defined( LINUX ) || defined( MBSD ) || defined(GAMEOS)
    #include "UT_Vector.C"
#endif


//////////////////////////////////////////////////////////////////////////////
//
// UT_Vector typedefs
//

typedef UT_PermutationT<int>		UT_Permutation;
typedef UT_VectorT<fpreal>		UT_VectorR;
typedef UT_VectorT<fpreal32>		UT_VectorF;
typedef UT_VectorT<fpreal64>		UT_VectorD;
typedef UT_VectorT<fpreal64>		UT_Vector;

//////////////////////////////////////////////////////////////////////////////
//
// UT_Vector inline binary operators 
//

// Free floating functions
inline fpreal64	dot(const UT_VectorD &v1, const UT_VectorD &v2);
inline fpreal64	distance2(const UT_VectorD &v1, const UT_VectorD &v2);
inline fpreal32	dot(const UT_VectorF &v1, const UT_VectorF &v2);
inline fpreal32	distance2(const UT_VectorF &v1, const UT_VectorF &v2);

/// Dot product
inline fpreal64	 
dot(const UT_VectorD &v1, const UT_VectorD &v2)
{
    return v1.dot(v2);
}

/// Distance squared (L2) aka quadrance
inline fpreal64
distance2(const UT_VectorD &v1, const UT_VectorD &v2)
{
    return v1.distance2(v2);
}

inline fpreal32	 
dot(const UT_VectorF &v1, const UT_VectorF &v2)
{
    return v1.dot(v2);
}

/// Distance squared (L2) aka quadrance
inline fpreal32
distance2(const UT_VectorF &v1, const UT_VectorF &v2)
{
    return v1.distance2(v2);
}

// Overload for custom formatting of UT_VectorT<T> with UTformat.
template<typename T> inline size_t
format(char *buffer, size_t buffer_size, const UT_VectorT<T> &v);

#endif

