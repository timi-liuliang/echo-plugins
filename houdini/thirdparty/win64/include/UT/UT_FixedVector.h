/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_FixedVector.h (UT Library, C++)
 *
 * COMMENTS:    A vector class templated on its size and data type.
 */

#pragma once

#ifndef __UT_FixedVector__
#define __UT_FixedVector__

#include "UT_Assert.h"
#include "UT_Storage.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>

template<typename T,exint SIZE,bool INSTANTIATED=false>
class UT_FixedVector
{
public:
    typedef UT_FixedVector<T,SIZE,INSTANTIATED> ThisType;
    typedef T value_type;
    typedef T theType;
    static const exint theSize = SIZE;

    typedef UT_StorageNum<T> Storage;

    SYS_FORCE_INLINE UT_FixedVector() = default;

    /// Initializes every component to the same value
    SYS_FORCE_INLINE explicit UT_FixedVector(T that) noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] = that;
    }

    SYS_FORCE_INLINE UT_FixedVector(const ThisType &that) = default;
    SYS_FORCE_INLINE UT_FixedVector(ThisType &&that) = default;

    /// Converts vector of S into vector of T,
    /// or just copies if same type.
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_FixedVector(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] = that[i];
    }

    template<typename S>
    SYS_FORCE_INLINE UT_FixedVector(const S that[SIZE]) noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] = that[i];
    }

    SYS_FORCE_INLINE const T &operator[](exint i) const noexcept
    {
        UT_ASSERT_P(i >= 0 && i < SIZE);
        return vec[i];
    }
    SYS_FORCE_INLINE T &operator[](exint i) noexcept
    {
        UT_ASSERT_P(i >= 0 && i < SIZE);
        return vec[i];
    }

    SYS_FORCE_INLINE constexpr const T *data() const noexcept
    {
        return vec;
    }
    SYS_FORCE_INLINE T *data() noexcept
    {
        return vec;
    }

    SYS_FORCE_INLINE ThisType &operator=(const ThisType &that) = default;
    SYS_FORCE_INLINE ThisType &operator=(ThisType &&that) = default;

    template <typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE ThisType &operator=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] = that[i];
        return *this;
    }
    SYS_FORCE_INLINE const ThisType &operator=(T that) noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] = that;
        return *this;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE void operator+=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] += that[i];
    }
    SYS_FORCE_INLINE void operator+=(T that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] += that;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_FixedVector<typename UT_StorageBetter<T,S>::Type,SIZE> operator+(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typedef typename UT_StorageBetter<T,S>::Type Type;
        UT_FixedVector<Type,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = vec[i] + that[i];
        return result;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE void operator-=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] -= that[i];
    }
    SYS_FORCE_INLINE void operator-=(T that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] -= that;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_FixedVector<typename UT_StorageBetter<T,S>::Type,SIZE> operator-(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typedef typename UT_StorageBetter<T,S>::Type Type;
        UT_FixedVector<Type,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = vec[i] - that[i];
        return result;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE void operator*=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] *= that[i];
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_FixedVector<typename UT_StorageBetter<T,S>::Type,SIZE> operator*(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typedef typename UT_StorageBetter<T,S>::Type Type;
        UT_FixedVector<Type,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = vec[i] * that[i];
        return result;
    }
    SYS_FORCE_INLINE void operator*=(T that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] *= that;
    }
    /// NOTE: Strictly speaking, this should use UT_StorageBetter<T,S>::Type,
    ///       but in the interests of avoiding accidental precision escalation,
    ///       it uses T.
    SYS_FORCE_INLINE UT_FixedVector<T,SIZE> operator*(T that) const
    {
        UT_FixedVector<T,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = vec[i] * that;
        return result;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE void operator/=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that)
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] /= that[i];
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_FixedVector<typename UT_StorageBetter<T,S>::Type,SIZE> operator/(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typedef typename UT_StorageBetter<T,S>::Type Type;
        UT_FixedVector<Type,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = vec[i] / that[i];
        return result;
    }

    SYS_FORCE_INLINE void operator/=(T that)
    {
        typename Storage::MathFloat t(that);
        t = typename Storage::MathFloat(1)/t;
        for (exint i = 0; i < SIZE; ++i)
            vec[i] *= t;
    }
    /// NOTE: Strictly speaking, this should use UT_StorageBetter<T,S>::Type,
    ///       but in the interests of avoiding accidental precision escalation,
    ///       it uses T.
    SYS_FORCE_INLINE UT_FixedVector<T,SIZE> operator/(T that) const
    {
        typename Storage::MathFloat t(that);
        t = typename Storage::MathFloat(1)/t;
        UT_FixedVector<T,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = vec[i] * t;
        return result;
    }
    SYS_FORCE_INLINE void negate()
    {
        for (exint i = 0; i < SIZE; ++i)
            vec[i] = -vec[i];
    }

    SYS_FORCE_INLINE UT_FixedVector<T,SIZE> operator-() const
    {
        UT_FixedVector<T,SIZE> result;
        for (exint i = 0; i < SIZE; ++i)
            result[i] = -vec[i];
        return result;
    }

    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool operator==(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
        {
            if (vec[i] != T(that[i]))
                return false;
        }
        return true;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool operator!=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const noexcept
    {
        return !(*this==that);
    }
    /// Lexicographic order comparison operators
    /// @{
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool operator<(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const noexcept
    {
        for (exint i = 0; i < SIZE-1; ++i)
        {
            if (vec[i] != that[i])
                return (vec[i] < that[i]);
        }
        return (vec[SIZE-1] < that[SIZE-1]);
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool operator<=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const noexcept
    {
        for (exint i = 0; i < SIZE-1; ++i)
        {
            if (vec[i] != that[i])
                return (vec[i] < that[i]);
        }
        return (vec[SIZE-1] <= that[SIZE-1]);
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool operator>(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const noexcept
    {
        // NOTE: Because of NaNs, this can't just be !(*this <= that)
        for (exint i = 0; i < SIZE-1; ++i)
        {
            if (vec[i] != that[i])
                return (vec[i] > that[i]);
        }
        return (vec[SIZE-1] > that[SIZE-1]);
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool operator>=(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const noexcept
    {
        // NOTE: Because of NaNs, this can't just be !(*this < that)
        for (exint i = 0; i < SIZE-1; ++i)
        {
            if (vec[i] != that[i])
                return (vec[i] > that[i]);
        }
        return (vec[SIZE-1] >= that[SIZE-1]);
    }
    /// @}
    SYS_FORCE_INLINE bool isNan() const
    {
        for (exint i = 0; i < SIZE; ++i)
        {
            if (SYSisNan(vec[i]))
                return true;
        }
        return false;
    }
    SYS_FORCE_INLINE bool isZero() const noexcept
    {
        for (exint i = 0; i < SIZE; ++i)
        {
            if (vec[i] != T(0))
                return false;
        }
        return true;
    }
    SYS_FORCE_INLINE bool equalZero(T tol = T(SYS_FTOLERANCE)) const
    {
        for (exint i = 0; i < SIZE; ++i)
        {
            if (vec[i] < -tol || vec[i] > tol)
                return false;
        }
        return true;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE bool isEqual(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that, S tol = S(SYS_FTOLERANCE)) const
    {
        for (exint i = 0; i < SIZE; ++i)
        {
            if (vec[i] < that[i]-tol || vec[i] > that[i]+tol)
                return false;
        }
        return true;
    }
    SYS_FORCE_INLINE T maxComponent() const
    {
        T v = vec[0];
        for (exint i = 1; i < SIZE; ++i)
            v = (vec[i] > v) ? vec[i] : v;
        return v;
    }
    SYS_FORCE_INLINE T minComponent() const
    {
        T v = vec[0];
        for (exint i = 1; i < SIZE; ++i)
            v = (vec[i] < v) ? vec[i] : v;
        return v;
    }
    SYS_FORCE_INLINE T avgComponent() const
    {
        T v = vec[0];
        for (exint i = 1; i < SIZE; ++i)
            v += vec[i];
        return v / SIZE;
    }

    SYS_FORCE_INLINE typename UT_StorageAtLeast32Bit<T,T>::Type length2() const noexcept
    {
        typedef typename UT_StorageAtLeast32Bit<T,T>::Type TheType;
        TheType a0(vec[0]);
        TheType result(a0*a0);
        for (exint i = 1; i < SIZE; ++i)
        {
            TheType ai(vec[i]);
            result += ai*ai;
        }
        return result;
    }
    SYS_FORCE_INLINE typename Storage::MathFloat length() const
    {
        typename Storage::MathFloat len2(length2());
        return SYSsqrt(len2);
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE typename UT_StorageAtLeast32Bit<T,S>::Type dot(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typedef typename UT_StorageAtLeast32Bit<T,S>::Type TheType;
        TheType result(TheType(vec[0])*TheType(that[0]));
        for (exint i = 1; i < SIZE; ++i)
            result += TheType(vec[i])*TheType(that[i]);
        return result;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE typename UT_StorageAtLeast32Bit<T,S>::Type distance2(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typedef typename UT_StorageAtLeast32Bit<T,S>::Type TheType;
        TheType v(TheType(vec[0]) - TheType(that[0]));
        TheType result(v*v);
        for (exint i = 1; i < SIZE; ++i)
        {
            v = TheType(vec[i]) - TheType(that[i]);
            result += v*v;
        }
        return result;
    }
    template<typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE typename UT_StorageNum<typename UT_StorageBetter<T,S>::Type>::MathFloat distance(const UT_FixedVector<S,SIZE,S_INSTANTIATED> &that) const
    {
        typename UT_StorageNum<typename UT_StorageBetter<T,S>::Type>::MathFloat dist2(distance2(that));
        return SYSsqrt(dist2);
    }

    SYS_FORCE_INLINE typename Storage::MathFloat normalize()
    {
        typename Storage::AtLeast32Bit len2 = length2();
        if (len2 <= std::numeric_limits<typename Storage::MathFloat>::min())
            return typename Storage::MathFloat(0);
        if (len2 == typename Storage::AtLeast32Bit(1))
            return typename Storage::MathFloat(1);
        typename Storage::MathFloat len = SYSsqrt(typename Storage::MathFloat(len2));
        // Check if the square root is equal 1.  sqrt(1+dx) ~ 1+dx/2,
        // so it may get rounded to 1 when it wasn't 1 before.
        if (len != typename Storage::MathFloat(1))
            (*this) /= len;
        return len;
    }

public:
    T vec[SIZE];
};

/// NOTE: Strictly speaking, this should use UT_StorageBetter<T,S>::Type,
///       but in the interests of avoiding accidental precision escalation,
///       it uses T.
template<typename T,exint SIZE,bool INSTANTIATED,typename S>
SYS_FORCE_INLINE UT_FixedVector<T,SIZE> operator*(const S &that,const UT_FixedVector<T,SIZE,INSTANTIATED> &a)
{
    T t(that);
    UT_FixedVector<T,SIZE> result;
    for (exint i = 0; i < SIZE; ++i)
        result[i] = t * a[i];
    return result;
}

template<typename T, exint SIZE, bool INSTANTIATED, typename S, bool S_INSTANTIATED>
SYS_FORCE_INLINE typename UT_StorageNum<typename UT_StorageBetter<T,S>::Type>::AtLeast32Bit
dot(const UT_FixedVector<T,SIZE,INSTANTIATED> &a, const UT_FixedVector<S,SIZE,S_INSTANTIATED> &b)
{
    return a.dot(b);
}

template<typename T>
struct UT_FixedVectorTraits
{
    typedef UT_FixedVector<T,1> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 1;
    static const bool isVectorType = false;
};

template<typename T,exint SIZE,bool INSTANTIATED>
struct UT_FixedVectorTraits<UT_FixedVector<T,SIZE,INSTANTIATED> >
{
    typedef UT_FixedVector<T,SIZE,INSTANTIATED> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = SIZE;
    static const bool isVectorType = true;
};

#endif
