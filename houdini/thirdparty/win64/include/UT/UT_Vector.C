/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Vector of arbitrary size (C++)
 *
 * COMMENTS:
 *
 */

#ifdef WIN32
    #include <memory.h>
#else
#ifdef LINUX
    #include <string.h>
#endif
#endif

#include <iostream>

#include <SYS/SYS_Align.h>
#include <VM/VM_Math.h>

#include <SYS/SYS_Math.h>
#include "UT_Vector.h"

#include "UT_Vector2.h"
#include "UT_Vector3.h"
#include "UT_Vector4.h"
#include "UT_EnvControl.h"
#include "UT_Format.h"
#include "UT_StringStream.h"

template <typename T>
UT_VectorT<T>::UT_VectorT(exint nl, exint nh)
{
    myVector = 0;
    myOwnData = 0;
    init(nl, nh);
}

template <typename T>
UT_VectorT<T>::UT_VectorT(const UT_VectorT<T> &v)
{
    exint i;

    myVector = 0;
    myOwnData = 0;
    init(v.myNL, v.myNH);
    for (i = v.myNL; i <= v.myNH; i++)
	myVector[i] = v.myVector[i];
}

template <typename T>
UT_VectorT<T>::~UT_VectorT()
{
    if (myOwnData && myVector)
	free(myVector + myNL);
}

template <typename T>
void
UT_VectorT<T>::subvector(const UT_VectorT<T> &v, exint nl, exint nh)
{
    exint	n = nh - nl + 1;
    
    if (myOwnData && myVector)
	free(myVector + myNL);
    myOwnData = 0;
    myNL = 1;
    myNH = n;
    myVector = &v.myVector[nl] - 1;
}

// Assign from a float array into the given subvector
template <typename T, typename S>
static void
utAssign(const T *data, S *vec, exint nl, exint nh)
{
    const exint	 n = nh - nl + 1;
    S		*dst;
    exint	 i;

    // assign the data, converting from single to double precision
    // NB: this may be vectorized into VM on x86 using the CVTPS2PD instr.
    dst = vec + nl;
    for (i = 0; i < n; i++)
	*dst++ = *data++;
}

template <typename T>
void
UT_VectorT<T>::assign(const fpreal32 *data, exint nl, exint nh)
{
    UT_ASSERT_P(nl >= myNL && nh <= myNH && (nh - nl + 1) >= 0);
    utAssign(data, myVector, nl, nh);
}

template <typename T>
void
UT_VectorT<T>::assign(const fpreal64 *data, exint nl, exint nh)
{
    UT_ASSERT_P(nl >= myNL && nh <= myNH && (nh - nl + 1) >= 0);
    utAssign(data, myVector, nl, nh);
}

template <typename T>
void
UT_VectorT<T>::init(exint nl, exint nh)
{
    UT_ASSERT((nh - nl + 1) >= 0);

    if (myVector && myOwnData)
    {
	if (nh-nl > myNH-myNL)
	{
	    myVector = (T *)realloc(myVector + myNL, (nh - nl + 1)*sizeof(T));
	    myVector = myVector - nl;
	}
	else
	{
	    myVector = myVector + myNL - nl; 
	}
    }
    else
    {
	myVector = (T *)malloc((nh - nl + 1)*sizeof(T));
	myVector = myVector - nl;
	myOwnData = 1;
    }

    myNL = nl;
    myNH = nh;
}

template <typename T>
void
UT_VectorT<T>::getPartialRange(exint &start, exint &end, const UT_JobInfo &info) const
{
    exint units = length();
    info.divideWork(units, start, end);
    start += getNL();
    end += getNL();
}

template <typename T>
void
UT_VectorT<T>::getPartialBlockRange(
    exint &start, exint &end, const exint blocksize, const UT_JobInfo &info) const
{
    exint units = (length() + blocksize-1)/blocksize;
    info.divideWork(units, start, end);
}

template <typename T>
void
UT_VectorT<T>::zero(exint nl, exint nh)
{
    UT_ASSERT_P(nl >= myNL && nh <= myNH && (nh - nl + 1) >= 0);
    memset(myVector + nl, 0, (nh - nl + 1)*sizeof(T));
}

template <typename T>
void
UT_VectorT<T>::getSubvector2(UT_Vector2 &v, exint idx) const
{
    v.x() = (*this)(idx);
    v.y() = (*this)(idx+1);
}

template <typename T>
void
UT_VectorT<T>::setSubvector2(exint idx, const UT_Vector2 &v)
{
    (*this)(idx) = v.x();
    (*this)(idx+1) = v.y();
}

template <typename T>
void
UT_VectorT<T>::getSubvector3(UT_Vector3 &v, exint idx) const
{
    v.x() = (*this)(idx);
    v.y() = (*this)(idx+1);
    v.z() = (*this)(idx+2);
}

template <typename T>
void
UT_VectorT<T>::setSubvector3(exint idx, const UT_Vector3 &v)
{
    (*this)(idx) = v.x();
    (*this)(idx+1) = v.y();
    (*this)(idx+2) = v.z();
}

template <typename T>
void
UT_VectorT<T>::getSubvector4(UT_Vector4 &v, exint idx) const
{
    v.x() = (*this)(idx);
    v.y() = (*this)(idx+1);
    v.z() = (*this)(idx+2);
    v.w() = (*this)(idx+3);
}

template <typename T>
void
UT_VectorT<T>::setSubvector4(exint idx, const UT_Vector4 &v)
{
    (*this)(idx) = v.x();
    (*this)(idx+1) = v.y();
    (*this)(idx+2) = v.z();
    (*this)(idx+3) = v.w();
}

template <typename T>
void
UT_VectorT<T>::changeNL(exint nl)
{
    exint 	diff = myNL-nl;

    myNL = nl;
    myNH -= diff;
    myVector += diff;
}

template <typename T>
T
UT_VectorT<T>::norm(int type) const
{
    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
        return 0;

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    normInternal((fpreal64*)accumulators, type);

    fpreal64 result = accumulators[0];

    if (type == 0)
    {
        for (exint i = 1; i < nblocks; ++i)
        {
            fpreal64 v = accumulators[i];
            if (v > result)
                result = v;
        }
    }
    else
    {
        for (exint i = 1; i < nblocks; ++i)
            result += accumulators[i];

        // normalize result.
        if (type == 2) // L2 norm
            result = SYSsqrt(result);
    }

    return result;
}


template <typename T>
T
UT_VectorT<T>::norm2() const 
{
    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
        return 0;

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    normInternal((fpreal64*)accumulators, 2);

    fpreal64 result = accumulators[0];
    for (exint i = 1; i < nblocks; ++i)
        result += accumulators[i];

    return result;
}

template <typename T>
void
UT_VectorT<T>::normInternalPartial(
    fpreal64 *output, int type, const UT_JobInfo &info) const
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;
    
    if (type == 0)
    {
        for (exint block = startblock; block < endblock; ++block)
        {
            exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
            fpreal64 result = SYSabs(myVector[i]);
            for (++i; i < blockend; ++i)
            {
                T v = SYSabs(myVector[i]);
                if (v > result)
                    result = v;
            }
            output[block] = result;
        }
    }
    else if (type == 1)
    {
        for (exint block = startblock; block < endblock; ++block)
        {
            exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
            fpreal64 result = SYSabs(myVector[i]);
            for (++i; i < blockend; ++i)
            {
                result += SYSabs(myVector[i]);
            }
            output[block] = result;
        }
    }
    else        // L2-norm
    {
        for (exint block = startblock; block < endblock; ++block)
        {
            exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
            T v = myVector[i];
            fpreal64 result = v*v;
            for (++i; i < blockend; ++i)
            {
                v = myVector[i];
                result += v*v;
            }
            output[block] = result;
        }
    }
}

template <typename T>
T
UT_VectorT<T>::distance2(const UT_VectorT<T> &v) const 
{
    UT_ASSERT_P(myNL == v.myNL && myNH == v.myNH);

    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
        return 0;

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    distance2Internal((fpreal64*)accumulators, v);

    fpreal64 result = accumulators[0];
    for (exint i = 1; i < nblocks; ++i)
        result += accumulators[i];

    return result;
}

template <typename T>
void
UT_VectorT<T>::distance2InternalPartial(
    fpreal64 *output, const UT_VectorT<T> &v, const UT_JobInfo &info) const
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
        fpreal64 diff = myVector[i] - v.myVector[i];
        fpreal64 result = diff*diff;
        for (++i; i < blockend; ++i)
        {
            diff = myVector[i] - v.myVector[i];
            result += diff*diff;
        }
        output[block] = result;
    }
}

template <typename T>
void
UT_VectorT<T>::negPartial(const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
        myVector[i] = -myVector[i];
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = -myVector[j];
#endif
}

template <typename T>
void
UT_VectorT<T>::negPlusPartial(const UT_VectorT<T> &v, const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
        myVector[i] = v.myVector[i] - myVector[i];
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = v.myVector[j] - myVector[j];
#endif
}

template <typename T>
void
UT_VectorT<T>::addScaledVecPartial(T s, const UT_VectorT<T> &v, 
				const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
        myVector[i] += s * v.myVector[i];
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance (2.4x slower for this function),
    // unless we make separate variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] += s * v.myVector[j];
#endif
}

template <typename T>
void
UT_VectorT<T>::addScaledVecNorm2(
    T s, const UT_VectorT<T> &v, fpreal64 *norm2)
{
    UT_ASSERT_P(myNL == v.myNL && myNH == v.myNH);

    // Just in case, handle null
    if (!norm2)
    {
        addScaledVec(s, v);
        return;
    }

    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
    {
        *norm2 = 0;
        return;
    }

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    addScaledVecNorm2Internal(s, v, (fpreal64*)accumulators);

    fpreal64 result = accumulators[0];
    for (exint i = 1; i < nblocks; ++i)
        result += accumulators[i];

    *norm2 = result;
}

template <typename T>
void
UT_VectorT<T>::addScaledVecNorm2InternalPartial(
    T s, const UT_VectorT<T> &v, fpreal64 *norm2, const UT_JobInfo &info)
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
        T val = myVector[i] + s*v.myVector[i];
        myVector[i] = val;
        fpreal64 result = val*val;
        for (++i; i < blockend; ++i)
        {
            val = myVector[i] + s*v.myVector[i];
            myVector[i] = val;
            result += val*val;
        }
        norm2[block] = result;
    }
}

template <typename T>
void
UT_VectorT<T>::addScaledVecNorm2UpTo(
    T s, const UT_VectorT<T> &v, fpreal64 *norm2, exint normlimit)
{
    UT_ASSERT_P(myNL == v.myNL && myNH == v.myNH);

    // Just in case, handle null
    if (!norm2 || normlimit <= 0)
    {
        addScaledVec(s, v);

        if (norm2)
            *norm2 = 0;

        return;
    }

    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
    {
        *norm2 = 0;
        return;
    }

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    addScaledVecNorm2UpToInternal(s, v, (fpreal64*)accumulators, normlimit);

    fpreal64 result = accumulators[0];
    for (exint i = 1; i < nblocks; ++i)
        result += accumulators[i];

    *norm2 = result;
}

template <typename T>
void
UT_VectorT<T>::addScaledVecNorm2UpToInternalPartial(
    T s, const UT_VectorT<T> &v, fpreal64 *norm2, exint normlimit, const UT_JobInfo &info)
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        const exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
        const exint normmax = SYSmin(blockend, normlimit);
        fpreal64 result = 0;
        for (; i < normmax; ++i)
        {
            T val = myVector[i] + s*v.myVector[i];
            myVector[i] = val;
            result += val*val;
        }
        for (; i < blockend; ++i)
        {
            myVector[i] += s*v.myVector[i];
        }
        norm2[block] = result;
    }
}

template <typename T>
void
UT_VectorT<T>::scaleAddVecPartial(T s, const UT_VectorT<T> &v,
				const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
        myVector[i] = s * myVector[i] + v.myVector[i];
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = s * myVector[j] + v.myVector[j];
#endif
}

template <typename T>
void
UT_VectorT<T>::multAndSetPartial(const UT_VectorT<T> &a, const UT_VectorT<T> &b,
				const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
        myVector[i] = a.myVector[i] * b.myVector[i];
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = a.myVector[j] * b.myVector[j];
#endif
}

template <typename T>
void
UT_VectorT<T>::multSetAndDotUpTo(
    const UT_VectorT<T> &a, const UT_VectorT<T> &b,
    fpreal64 *dot_aba, exint dotlimit)
{
    UT_ASSERT_P(myNL == a.myNL && myNH == a.myNH);
    UT_ASSERT_P(myNL == b.myNL && myNH == b.myNH);

    if (!dot_aba)
    {
        multAndSet(a, b);
        return;
    }

    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
    {
        *dot_aba = 0;
        return;
    }

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    multSetAndDotUpToInternal(a, b, (fpreal64*)accumulators, dotlimit);

    fpreal64 result = accumulators[0];
    for (exint i = 1; i < nblocks; ++i)
        result += accumulators[i];

    *dot_aba = result;
}

template <typename T>
void
UT_VectorT<T>::multSetAndDotUpToInternalPartial(
    const UT_VectorT<T> &a, const UT_VectorT<T> &b,
    fpreal64 *dot_aba, exint dotlimit,
    const UT_JobInfo &info)
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        const exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
        const exint dotmax = SYSmin(blockend, dotlimit);
        fpreal64 result = 0;
        for (; i < dotmax; ++i)
        {
            T av = a.myVector[i];
            T bv = b.myVector[i];
            T val = av*bv;
            myVector[i] = val;
            result += val*av;
        }
        for (; i < blockend; ++i)
        {
            myVector[i] += a.myVector[i]*b.myVector[i];
        }
        dot_aba[block] = result;
    }
}

template <typename T>
void
UT_VectorT<T>::divAndSetPartial(const UT_VectorT<T> &a, const UT_VectorT<T> &b,
				const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
        myVector[i] = a.myVector[i] / b.myVector[i];
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = a.myVector[j] / b.myVector[j];
#endif
}

template <typename T>
void
UT_VectorT<T>::safeInvertAndSetPartial(const UT_VectorT<T> &a,
				       const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
    {
	myVector[i] = SYSsafediv(T(1.0), a.myVector[i]);
    }
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = SYSsafediv(T(1.0), a.myVector[j]);
#endif
}

template <typename T>
void
UT_VectorT<T>::invertAndSetPartial(const UT_VectorT<T> &a,
				   const UT_JobInfo &info)
{
    exint	i, end;

    getPartialRange(i, end, info);

#if 0
    for (; i < end; i++)
    {
	myVector[i] = T(1.0) / a.myVector[i];
    }
#else
    // NOTE: Compilers get confused, thinking that i or end
    // might change underneath us, because we passed a non-const reference
    // to them to getPartialRange, so they don't make any assumptions
    // and have terrible performance, unless we make separate
    // variables j and jend that are entirely local.
    for (exint j = i, jend = end; j < jend; j++)
        myVector[j] = T(1.0) / a.myVector[j];
#endif
}

template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator= (const UT_VectorT<T> &v)
{
    UT_ASSERT_P(length() == v.length());
    copyFrom(v);
    return *this;
} 

template <typename T>
void
UT_VectorT<T>::copyFromPartial(const UT_VectorT<T> &v, 
				const UT_JobInfo &info)
{
    UT_ASSERT_P(length() == v.length());
    exint	i, end;

    getPartialRange(i, end, info);

    memcpy(&myVector[i], &v.myVector[i], 
	    (end - i) * sizeof(T));
}

template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator+= (const UT_VectorT<T> &v)
{
    exint	i;

    for (i=myNL; i<=myNH; i++)
        myVector[i] += v.myVector[i];
    return *this;
}

template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator-= (const UT_VectorT<T> &v)
{
    exint	i;

    for (i=myNL; i<=myNH; i++)
        myVector[i] -= v.myVector[i];
    return *this;
}

template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator*= (const UT_VectorT<T> &v)
{
    exint	i;

    for (i=myNL; i<=myNH; i++)
        myVector[i] *= v.myVector[i];
    return *this;
}

template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator/= (const UT_VectorT<T> &v)
{
    exint	i;

    for (i=myNL; i<=myNH; i++)
        myVector[i] /= v.myVector[i];
    return *this;
}

template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator*= (T scalar)
{
    exint	i;

    for (i=myNL; i<=myNH; i++)
        myVector[i] *= scalar;
    return *this;
}


template <typename T>
UT_VectorT<T> &
UT_VectorT<T>::operator/= (T scalar)
{
    exint	i;

    scalar = 1.0F / scalar;
    for (i=myNL; i<=myNH; i++)
        myVector[i] *= scalar;
    return *this;
}


template <typename T>
T
UT_VectorT<T>::dot(const UT_VectorT<T> &v) const
{
    UT_ASSERT_P(myNL == v.myNL && myNH == v.myNH);

    exint nblocks = (length()+PARALLEL_BLOCK_SIZE-1)/PARALLEL_BLOCK_SIZE;
    if (nblocks <= 0)
        return 0;

    UT_StackBuffer<fpreal64> accumulators(nblocks);

    dotInternal((fpreal64*)accumulators, v);

    fpreal64 result = accumulators[0];
    for (exint i = 1; i < nblocks; ++i)
        result += accumulators[i];

    return result;
}

template <typename T>
void
UT_VectorT<T>::dotInternalPartial(fpreal64 *output, const UT_VectorT<T> &v, const UT_JobInfo &info) const
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
        fpreal64 result = myVector[i]*v.myVector[i];
        for (++i; i < blockend; ++i)
        {
            result += myVector[i]*v.myVector[i];
        }
        output[block] = result;
    }
}

template <typename T>
std::ostream &
UT_VectorT<T>::save(std::ostream &os) const
{
    exint	i;

    for (i=myNL; i<=myNH; i++)
        os << i << ": " << myVector[i] << "\n";

    return os;
}

template <typename T>
bool
UT_VectorT<T>::isEqual(const UT_VectorT<T> &v, int64 ulps)
{
    if (getNL() != v.getNL() || getNH() != v.getNH())
	return false;
    for (exint i = getNL(); i < getNH(); i++)
    {
	if (!SYSalmostEqual((*this)(i), v(i), ulps))
	    return false;
    }
    return true;
}

template <typename T>
bool
UT_VectorT<T>::hasNan() const
{
    for (exint i = myNL; i <= myNH; i++)
    {
	if (SYSisNan(myVector[i]))
	    return true;
    }
    return false;
}

template <typename T>
void
UT_VectorT<T>::testForNan() const
{
    if (UT_EnvControl::getInt(ENV_HOUDINI_DOPVOLUME_NANTEST) && hasNan())
    {
	std::cerr << "NAN found in UT_VectorT\n";
	UT_ASSERT(0);
    }
}

//////////////////////////////
// UT_Vector Specializations
//////////////////////////////

#if 1

template <>
inline void
UT_VectorT<fpreal32>::negPartial(const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::negate(&myVector[i], &myVector[i], end-i);
}

template <>
inline void
UT_VectorT<fpreal32>::negPlusPartial(const UT_VectorT<fpreal32> &v, const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::scaleoffset(&myVector[i], -1.0F, &v.myVector[i], end-i);
}

template <>
inline void
UT_VectorT<fpreal32>::addScaledVecPartial(fpreal32 s, 
				const UT_VectorT<fpreal32> &v, 
				const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::madd(&myVector[i], &v.myVector[i], s, end-i);
}

template <>
inline void
UT_VectorT<fpreal32>::addScaledVecNorm2InternalPartial(
    fpreal32 s, 
    const UT_VectorT<fpreal32> &v, 
    fpreal64 *norm2,
    const UT_JobInfo &info)
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        const exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);

        fpreal64 result = VM_Math::maddAndNorm(&myVector[i], &v.myVector[i], s, blockend-i);

        norm2[block] = result;

        i = blockend;
    }
}

template <>
inline void
UT_VectorT<fpreal32>::addScaledVecNorm2UpToInternalPartial(
    fpreal32 s, 
    const UT_VectorT<fpreal32> &v, 
    fpreal64 *norm2,
    exint normlimit,
    const UT_JobInfo &info)
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        const exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);

        fpreal64 result = 0;
        if (i < normlimit)
        {
            const exint normmax = SYSmin(blockend, normlimit);
            result = VM_Math::maddAndNorm(&myVector[i], &v.myVector[i], s, normmax-i);

            i = normmax;
            if (i < blockend)
                VM_Math::madd(&myVector[i], &v.myVector[i], s, blockend-i);
        }
        else
            VM_Math::madd(&myVector[i], &v.myVector[i], s, blockend-i);

        norm2[block] = result;

        i = blockend;
    }
}

template <>
inline void
UT_VectorT<fpreal32>::scaleAddVecPartial(fpreal32 s, 
				const UT_VectorT<fpreal32> &v,
				const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::scaleoffset(&myVector[i], s, &v.myVector[i], end-i);
}


template <>
inline void
UT_VectorT<fpreal32>::multAndSetPartial(const UT_VectorT<fpreal32> &a, 
				const UT_VectorT<fpreal32> &b,
				const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::mul(&myVector[i], &a.myVector[i], &b.myVector[i], end-i);
}

template <>
inline void
UT_VectorT<fpreal32>::multSetAndDotUpToInternalPartial(
    const UT_VectorT<fpreal32> &a, 
    const UT_VectorT<fpreal32> &b,
    fpreal64 *dot_aba, exint dotlimit,
    const UT_JobInfo &info)
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        const exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);

        fpreal64 result = 0;
        if (i < dotlimit)
        {
            const exint dotmax = SYSmin(blockend, dotlimit);

            result = VM_Math::mulAndDotDA(&myVector[i], &a.myVector[i], &b.myVector[i], dotmax-i);

            i = dotmax;
            if (i < blockend)
                VM_Math::mul(&myVector[i], &a.myVector[i], &b.myVector[i], blockend-i);
        }
        else
            VM_Math::mul(&myVector[i], &a.myVector[i], &b.myVector[i], blockend-i);

        dot_aba[block] = result;

        i = blockend;
    }
}

template <>
inline void
UT_VectorT<fpreal32>::divAndSetPartial(const UT_VectorT<fpreal32> &a, 
				const UT_VectorT<fpreal32> &b,
				const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::div(&myVector[i], &a.myVector[i], &b.myVector[i], end-i);
}

template <>
inline void
UT_VectorT<fpreal32>::safeInvertAndSetPartial(const UT_VectorT<fpreal32> &a, 
				const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::safediv(&myVector[i], 1.0f, &a.myVector[i], end-i);
}

template <>
inline void
UT_VectorT<fpreal32>::invertAndSetPartial(const UT_VectorT<fpreal32> &a, 
				const UT_JobInfo &info)
{
    exint		i, end;

    getPartialRange(i, end, info);

    VM_Math::div(&myVector[i], 1.0f, &a.myVector[i], end-i);
}

template <>
inline UT_VectorT<fpreal32> &
UT_VectorT<fpreal32>::operator+= (const UT_VectorT<fpreal32> &v)
{
    VM_Math::add(&myVector[myNL], &myVector[myNL], &v.myVector[myNL], myNH-myNL+1);
    return *this;
}

template <>
inline UT_VectorT<fpreal32> &
UT_VectorT<fpreal32>::operator-= (const UT_VectorT<fpreal32> &v)
{
    VM_Math::sub(&myVector[myNL], &myVector[myNL], &v.myVector[myNL], myNH-myNL+1);
    return *this;
}

template <>
inline UT_VectorT<fpreal32> &
UT_VectorT<fpreal32>::operator*= (const UT_VectorT<fpreal32> &v)
{
    VM_Math::mul(&myVector[myNL], &myVector[myNL], &v.myVector[myNL], myNH-myNL+1);
    return *this;
}

template <>
inline UT_VectorT<fpreal32> &
UT_VectorT<fpreal32>::operator/= (const UT_VectorT<fpreal32> &v)
{
    VM_Math::div(&myVector[myNL], &myVector[myNL], &v.myVector[myNL], myNH-myNL+1);
    return *this;
}

template <>
inline UT_VectorT<fpreal32> &
UT_VectorT<fpreal32>::operator*= (fpreal32 scalar)
{
    VM_Math::mul(&myVector[myNL], &myVector[myNL], scalar, myNH-myNL+1);
    return *this;
}

template <>
inline UT_VectorT<fpreal32> &
UT_VectorT<fpreal32>::operator/= (fpreal32 scalar)
{
    VM_Math::div(&myVector[myNL], &myVector[myNL], scalar, myNH-myNL+1);
    return *this;
}

template <>
inline void
UT_VectorT<fpreal32>::dotInternalPartial(fpreal64 *output, const UT_VectorT<fpreal32> &v, const UT_JobInfo &info) const
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    for (exint block = startblock; block < endblock; ++block)
    {
        const exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);

        fpreal64 result = VM_Math::dot(&myVector[i], &v.myVector[i], blockend-i);

        output[block] = result;

        i = blockend;
    }
}

template <>
inline void
UT_VectorT<fpreal32>::normInternalPartial(
    fpreal64 *output, int type,
    const UT_JobInfo &info) const
{
    exint startblock;
    exint endblock;
    getPartialBlockRange(startblock, endblock, PARALLEL_BLOCK_SIZE, info);

    exint i = startblock*PARALLEL_BLOCK_SIZE + myNL;

    if (type == 0)
    {
        for (exint block = startblock; block < endblock; ++block)
        {
            exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
            fpreal32 result = SYSabs(myVector[i]);
            for (++i; i < blockend; ++i)
            {
                fpreal32 v = SYSabs(myVector[i]);
                if (v > result)
                    result = v;
            }
            output[block] = result;
        }
    }
    else if (type == 1)
    {
        for (exint block = startblock; block < endblock; ++block)
        {
            exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);
            fpreal64 result = SYSabs(myVector[i]);
            for (++i; i < blockend; ++i)
            {
                result += SYSabs(myVector[i]);
            }
            output[block] = result;
        }
    }
    else        // L2-norm
    {
        for (exint block = startblock; block < endblock; ++block)
        {
            exint blockend = SYSmin(i+PARALLEL_BLOCK_SIZE, myNH+1);

            fpreal64 result = VM_Math::dot(&myVector[i], &myVector[i], blockend-i);

            output[block] = result;

            i = blockend;
        }
    }
}
#endif

template <typename V> inline size_t
format(char *buffer, size_t bufsize, const UT_VectorT<V> &v)
{
    UT_OStringStream row;
    for (int c = v.getNL(); c <= v.getNH(); c++) {
	if (c != v.getNL())
	    row << ", ";
	UTformat(row, "{}", v(c));
    }

    UT::Format::Writer writer(buffer, bufsize);
    UT::Format::Formatter<> f;
    return f.format(writer, "[{}]", {row.str()});
}

// Explicit instantiation for format
#define VECTOR_INSTANTIATE_FMT(T) \
	template UT_API size_t format<T>(char*, size_t, const UT_VectorT<T>&);
VECTOR_INSTANTIATE_FMT(fpreal32)
VECTOR_INSTANTIATE_FMT(fpreal64)

//////////////////////////
// UT_Permutation class
//////////////////////////


template <typename T>
UT_PermutationT<T>::UT_PermutationT(exint nl, exint nh)
{
    myNL = nl;
    myNH = nh;
    myVector = (T *)malloc((nh - nl + 1)*sizeof(T));
    myVector = myVector - nl;
}

template <typename T>
UT_PermutationT<T>::~UT_PermutationT()
{
    free(myVector + myNL);
}

template <typename T>
UT_PermutationT<T>::UT_PermutationT(const UT_PermutationT<T> &p)
{
    clone(p);
}

template <typename T>
UT_PermutationT<T> &
UT_PermutationT<T>::operator=(const UT_PermutationT<T> &p)
{
    if (&p != this)
    {
	free(myVector + myNL);
	clone(p);
    }
    return *this;
}

template <typename T>
void
UT_PermutationT<T>::clone(const UT_PermutationT<T> &p)
{
    UT_ASSERT(&p != this);
    myNL = p.myNL;
    myNH = p.myNH;
    myVector = (T *)malloc((myNH - myNL + 1)*sizeof(T));
    myVector = myVector - myNL;

    memcpy(myVector + myNL, p.myVector + myNL, (myNH - myNL + 1)*sizeof(T));
}

template <typename T>
void
UT_PermutationT<T>::zero()
{
    memset(myVector + myNL, 0, (myNH - myNL + 1)*sizeof(T));
}

template <typename T>
void
UT_PermutationT<T>::changeNL(exint nl)
{
    exint 	diff = myNL-nl;

    myNL = nl;
    myNH -= diff;
    myVector += diff;
}


