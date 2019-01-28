/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Cdf.h ( UT Library, C++)
 *
 * COMMENTS:	An N-Dimensional cumulative distribution function (CDF)
 *		for sampling.
 */

#ifndef __UT_Cdf__
#define __UT_Cdf__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include "UT_Assert.h"
#include "UT_Algorithm.h"
#include "UT_RootFinder.h"
#include "UT_VoxelArray.h"

struct UT_CdfLinearSearch
{
    template<class InputIt, typename T>
    static inline int eval(
        InputIt cdf_begin,
        const int &res,
        const T &u)
    {
        int i = 0;
        for ( ; i < res; ++i)
        {
            if (cdf_begin[i] > u) { break; }
        }

        return i;
    }
};

struct UT_CdfStdBinarySearch
{
    template<class InputIt, typename T>
    static inline int eval(
        InputIt cdf_begin,
        const int &res,
        const T &u)
    {
        InputIt u_i = std::upper_bound(cdf_begin, cdf_begin+res, u);

        return SYSmax(int(0), int(u_i-cdf_begin));
    }
};

struct UT_CdfFastBinarySearch
{
    template<class InputIt, typename T>
    static inline int eval(
        InputIt cdf_begin,
        const int &res,
        const T &u)
    {
        InputIt u_i = UTfastUpperBound(cdf_begin, cdf_begin+res, u);

        return SYSmax(int(0), int(u_i-cdf_begin));
    }
};

typedef UT_CdfStdBinarySearch UT_Cdf_default_search_t;

template<int N>
struct UT_CdfIndexer
{
    static inline int eval(const int *res,
                           const int *i)
    {
        return 0;
    }
};

template<>
struct UT_CdfIndexer<1>
{
    static inline int eval(const int *res,
                           const int *i)
    {
        return i[0];
    }
};

template<>
struct UT_CdfIndexer<2>
{
    static inline int eval(const int *res,
                           const int *i)
    {
        return i[1] * res[0] + i[0];
    }
};

template<>
struct UT_CdfIndexer<3>
{
    static inline int eval(const int *res,
                           const int *i)
    {
        return (i[0] * res[1] + i[1]) * res[2] + i[2];
    }
};

template<int N>
struct UT_CdfValueFill
{
    template<class F, typename T>
    static inline void eval(
        T *cdf,
        const int *res,
        const F &ftor)
    {
    }
};

template<>
struct UT_CdfValueFill<1>
{
    enum { N = 1 };

    template<class F, typename T>
    static inline void eval(
        T *cdf,
        const int *res,
        const F &ftor)
    {
        int i[N] = {0};
        int j = 0;
        for ( ; i[0] < res[0]; ++i[0])
        {
            cdf[j++] = ftor(i);
        }
    }
};

template<>
struct UT_CdfValueFill<2>
{
    enum { N = 2 };

    template<class F, typename T>
    static inline void eval(
        T *cdf,
        const int *res,
        const F &ftor)
    {
        int i[N] = {0, 0};
        int j = 0;
        for (i[0] = 0; i[0] < res[0]; ++i[0])
        {
            for (i[1] = 0; i[1] < res[1]; ++i[1])
            {
                cdf[j++] = ftor(i);
            }
        }
    }
};

template<>
struct UT_CdfValueFill<3>
{
    enum { N = 3 };

    template<class F, typename T>
    static inline void eval(
        T *cdf,
        const int *res,
        const F &ftor)
    {
        int i[N] = {0, 0, 0};
        int j = 0;
        for (i[0] = 0; i[0] < res[0]; ++i[0])
        {
            for (i[1] = 0; i[1] < res[1]; ++i[1])
            {
                for (i[2] = 0; i[2] < res[2]; ++i[2])
                {
                    cdf[j++] = ftor(i);
                }
            }
        }
    }
};

struct UT_CdfValueVoxelArrayF
{
    enum { N = 3 };

    UT_CdfValueVoxelArrayF(const UT_VoxelArrayF *voxels)
        : myVoxels(voxels) {}

    inline float operator()(const int *i) const
    {
        return myVoxels->operator()(i[0], i[1], i[2]);
    }

    const UT_VoxelArrayF *myVoxels;
};

/// Fill a linear cdf with res entries of a linear gradient.
template<class OutputIt>
inline void
UTfillCdfIdentity(OutputIt cdf, const int res)
{
    if (res == 0) { return; }
    typedef typename std::iterator_traits<OutputIt>::value_type output_t;
    output_t s = 1.0F / res;
    for (int i = 0; i < res; ++i)
    {
        *cdf++ = s * i;
    }
}

/// Sample from a linear cdf with res entries.  The return value is the
/// sampled array index.
template<class S, class InputIt, typename T>
inline int
UTsampleCdf(InputIt cdf_begin, const int &res, const T &u)
{
    return SYSclamp(S::eval(cdf_begin, res, u), 0, res-1);
}

/// Sample from a linear cdf with res entries.  The return value is the
/// sampled array index and dval is the offset.
template<class S, class InputIt, typename T>
inline int
UTsampleCdf(InputIt cdf_begin, const int &res, const T &u,
            T &dval, T &pdf)
{
    fpreal	range;
    int		idx;

    dval = u;
    dval *= cdf_begin[res-1];
    idx = UTsampleCdf<S>(cdf_begin, res, dval);

    range = cdf_begin[idx];
    pdf = 0;
    if (idx > 0)
    {
        dval -= cdf_begin[idx-1];
        range -= cdf_begin[idx-1];
    }
    if (range > 0)
    {
        dval /= range;
        pdf = res*range / cdf_begin[res-1];
    }

    return idx;
}

/// Sample the pdf from a linear cdf at index i with res entries.
template<class InputIt, typename T>
inline void
UTsampleCdf(InputIt cdf_begin, const int &res, const int &i, T &pdf)
{
    fpreal range = cdf_begin[i];
    pdf = 0;
    if (i > 0)
    {
        range -= cdf_begin[i-1];
    }
    if (range > 0)
    {
        pdf = res*range / cdf_begin[res-1];
    }
}

/// Find the value x from 0 to 1 where the area from 0 to x under the
/// trapezoid (0,0),(1,0),(1,d1),(0,d0) is portion u of the total area.
template<typename T>
inline T
UTsampleTrapezoidPdf(T u, T d0, T d1)
{
    // Solve for x:
    // 0 = (d1-d0)*x^2 + 2*d0*x - (d0+d1)*u
    T x0; T x1;
    int nroots = UT_RootFinder::quadratic(d1-d0, 2*d0, -(d0+d1)*u, x0, x1);
    // If there are multiple roots, pick the one that is in
    // or just outside of the [0,1] interval.
    T x = (nroots == 1 || SYSabs(x0-0.5f) <= SYSabs(x1-0.5f)) ? x0 : x1;
    return x;
}

/// Create a PDF from a range of values.
template<class InputIt, class OutputIt>
inline void
UTcreatePdf(InputIt values_begin,
            InputIt values_end,
            OutputIt pdf_begin)
{
    typedef typename std::iterator_traits<OutputIt>::value_type output_t;

    UTnormalizeArray(values_begin, values_end, pdf_begin);

    output_t sum = 0;
    std::for_each(pdf_begin, pdf_begin + (values_end-values_begin),
        [&sum](const output_t &v)
        {
            sum += v;
        });

    output_t area = output_t(1.0) / sum;
    std::transform(pdf_begin, pdf_begin + (values_end-values_begin), pdf_begin,
                   [&area](const output_t &v) { return v * area; });
}

/// Create a CDF from a range of values.
template<class InputIt, class OutputIt>
inline void
UTcreateCdf(InputIt values_begin,
            InputIt values_end,
            OutputIt cdf_begin)
{
    int i = 0;
    while (values_begin != values_end)
    {
        *cdf_begin = *values_begin;
        if (i > 0) { *cdf_begin += *(cdf_begin-1); }
        ++values_begin;
        ++cdf_begin;
        ++i;
    }
}

template<
    int N = 1,
    typename T = float,
    class S = UT_Cdf_default_search_t>
class UT_API_TMPL UT_Cdf
{
    static_assert((N > 0 && N < 4), "unsupported number of dimensions");

public:
    enum { DIM = N };

    inline UT_Cdf();

    /// Construct from an array
    ///		data	= row-major pdf/cdf data
    ///		res	= resolution for each dimension
    /// data values less than zero will be treated as zero.
    inline UT_Cdf(T *data,
                  const int *res,
                  const bool &copydata);

    /// Construct from a value functor
    ///		ftor	= value functor
    ///		res	= resolution for each dimension
    /// data values less than zero will be treated as zero.
    template<class F>
    inline UT_Cdf(const F &ftor,
                  const int *res);

    /// Create a cdf without data.  You should then use getRawArray()
    /// or setRawArray() functions and build() to construct the cdf.
    /// This method avoids duplication of memory for the pdf and cdf
    /// when only the cdf is needed.
    ///		res	= resolution for each dimension
    inline UT_Cdf(const int *res);

    inline ~UT_Cdf();

    /// Clear associated storage.
    inline void         clear();

    /// Resize storage according to resolution.
    ///		res	= resolution for each dimension
    inline void         resize(const int *res);

    /// Set the data array from an array.
    ///		data	 = row-major pdf/cdf data
    ///		copydata = should the data be copied?
    inline void         setRawArray(T *data,
                                    const bool &copydata);

    /// Set the data array from a value functor.
    ///		ftor	 = value functor
    template<class F>
    inline void         setRawArray(const F &ftor);

    /// Retrieve the raw data array in row-major order.  There will be
    /// product(res) entries in the array to fill out.
    inline T            *getRawArray() { return myCdf[N-1]; }

    /// Build the cdf from the row-major data array.  If you provided data
    /// to the constructor, you don't need to call this method unless
    /// you've modified the data through getRawArray().
    inline void         build();

    /// Draw an importance sample from the cdf.  u and dval must have
    /// 'N' entries.  In this version, dval is the resulting sample value
    /// for each dimension in the range [0, 1).
    inline void         sample(const T *u, T *dval) const;
    inline void         sample(const T *u, T *dval, T &pdf) const;

    /// Same as above but produces an exact index.
    inline void         sample(const T *u, int *didx) const;

    /// Same as above but produces an exact index.  (dval) above relates to
    /// (didx, doff) using the following identity:
    ///     dval[dim] = (didx[dim] + doff[dim]) / res[dim]
    inline void         sample(const T *u, int *didx, T *doff, T &pdf) const;

    /// Evaluate the pdf at the given index.
    inline void         evaluatePdf(const T *u, T &pdf) const;

    /// Get the sum over all image values
    inline const T      &getSum() const         { return myCdf[0][myRes[0]-1]; }
    inline const T      &getISum() const	{ return myISum; }

    /// Get the average image value
    inline const T      &getScaling() const	{ return myScale; }
    inline const T      &getIScaling() const	{ return myIScale; }

    inline int		getDim() const		{ return N; }
    inline const int	*getRes() const         { return myRes; }
    inline const int	&getRes(int dim) const	{ return myRes[dim]; }

    inline void         dump() const;

private:
    inline void         init();
    inline void         realloc(const int *res);

    T  *myCdf[N];
    int myRes[N];
    T   myIRes[N];
    T   myISum;
    T   myScale;
    T   myIScale;
    int myCapacity;
    int mySize;
    bool myOwnData;
};

template<int N, typename T, class S>
inline
UT_Cdf<N, T, S>::UT_Cdf()
{
    init();
}

template<int N, typename T, class S>
inline
UT_Cdf<N, T, S>::UT_Cdf(
    T *data,
    const int *res,
    const bool &copydata)
{
    init();
    myOwnData = copydata;
    resize(res);
    setRawArray(data, copydata);
    build();
}

template<int N, typename T, class S>
template<class F>
inline
UT_Cdf<N, T, S>::UT_Cdf(const F &ftor,
                     const int *res)
{
    init();
    resize(res);
    setRawArray(ftor);
    build();
}

template<int N, typename T, class S>
inline
UT_Cdf<N, T, S>::UT_Cdf(const int *res)
{
    init();
    resize(res);
}

template<int N, typename T, class S>
inline
UT_Cdf<N, T, S>::~UT_Cdf()
{
    clear();
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::init()
{
    for (int i = 0; i < N; ++i)
    {
        myCdf[i] = nullptr;
        myRes[i] = 0;
        myIRes[i] = 0;
    }
    myISum = 0;
    myScale = 0;
    myIScale = 0;
    myCapacity = 0;
    mySize = 0;
    myOwnData = true;
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::clear()
{
    if (!myOwnData) { return; }
    for (int i = 0; i < N; ++i)
    {
        if (myCdf[i])
        {
            delete[] myCdf[i];
            myCdf[i] = nullptr;
        }
    }
    myCapacity = 0;
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::realloc(const int *res)
{
    if (!myOwnData) { return; }
    clear();
    myCapacity = 1;
    for (int i = 0; i < N; ++i)
    {
        myCapacity *= res[i];
        myCdf[i] = new T[myCapacity];
    }
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::resize(const int *res)
{
    mySize = 1;
    for (int i = 0; i < N; ++i)
    {
        myRes[i] = res[i];
        myIRes[i] = 1.0F / myRes[i];
        mySize *= myRes[i];
    }
    if (mySize > myCapacity) { realloc(res); }
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::setRawArray(
    T *data,
    const bool &copydata)
{
    if (myOwnData && !copydata)
    {
        clear();
    }
    myOwnData = copydata;
    if (copydata)
    {
        memcpy(myCdf[N-1], data, mySize*sizeof(T));
    }
    else
    {
        myCdf[N-1] = data;
    }
}

template<int N, typename T, class S>
template<class F>
inline void
UT_Cdf<N, T, S>::setRawArray(const F &ftor)
{
    static_assert(std::is_class<F>::value, "invalid functor object");
    static_assert(F::N == N, "unsupported number of dimensions");

    myOwnData = true;
    UT_CdfValueFill<N>::eval(myCdf[N-1], myRes, ftor);
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::build()
{
    // Compute the sums for efficient sampling
    exint size = mySize;
    for (int i = N; i-- > 0; )
    {
        size /= myRes[i];

        exint idx = 0;
        for (exint j = 0; j < size; ++j)
        {
            // We need non-negative, so we clamp negative to zero.
            T value = SYSmax(myCdf[i][idx], 0.0F);
            myCdf[i][idx] = value;
            // Always sum using double-precision, to reduce the
            // impact of catastrophic roundoff error.
            double sum = value;
            ++idx;
            for (exint k = 1; k < myRes[i]; ++k, ++idx)
            {
                value = SYSmax(myCdf[i][idx], 0.0F);
                sum += value;
                myCdf[i][idx] = sum;
            }

            if (i > 0)
                myCdf[i-1][j] = myCdf[i][idx-1];
        }
    }

    UT_ASSERT(size == 1);

    myScale = getSum();
    for (int i = 0; i < N; ++i)
        myScale /= (T)myRes[i];

    myISum = 1.0F / getSum();
    myIScale = 1.0F / getScaling();
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::sample(const T *u, T *dval) const
{
    T pdf = 0;

    sample(u, dval, pdf);
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::sample(const T *u, T *dval, T &pdf) const
{
    int didx[N];

    sample(u, didx, dval, pdf);
    for (int i = 0; i < N; ++i)
    {
        dval[i] += didx[i];
        dval[i] *= myIRes[i];
    }
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::sample(const T *u, int *didx) const
{
    int off = 0;
    int idx = 0;
    for (int i = 0; i < N; ++i)
    {
        off += idx;
        off *= myRes[i];
        const T *cdf = myCdf[i] + off;

        idx = didx[i] = UTsampleCdf<S>(
                cdf, myRes[i], u[i] * cdf[myRes[i]-1]);
    }
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::sample(const T *u, int *didx, T *doff, T &pdf) const
{
    int off = 0;
    int idx = 0;
    T onepdf = 0;
    pdf = 1;
    for (int i = 0; i < N; ++i)
    {
        off += idx;
        off *= myRes[i];
        const T *cdf = myCdf[i] + off;

        idx = didx[i] = UTsampleCdf<S>(
                cdf, myRes[i], u[i], doff[i], onepdf);
        pdf *= onepdf;
    }
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::evaluatePdf(const T *u, T &pdf) const
{
    int off = 0;
    int idx = 0;
    for (int i = 0; i < N; ++i)
    {
        idx = (int)(u[i]*myRes[i]);
        idx = SYSclamp(idx, 0, myRes[i]-1);

        off *= myRes[i];
        off += idx;
    }

    pdf = myCdf[N-1][off];
    if (off > 0)
    {
        pdf -= myCdf[N-1][off-1];
    }

    // Divide by the average value to normalize the pdf to 1
    pdf *= getIScaling();
}

template<int N, typename T, class S>
inline void
UT_Cdf<N, T, S>::dump() const
{
    for (int dim = 0; dim < N; ++dim)
    {
        fprintf(stderr, "dim %d:\n", dim);
        for (int i = 0; i < myRes[dim]; ++i)
            fprintf(stderr, "%d: %f\n", i, myCdf[dim][i]);
    }
}

#endif
