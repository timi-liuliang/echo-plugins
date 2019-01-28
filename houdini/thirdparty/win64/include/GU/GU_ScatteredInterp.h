/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	GU_ScatteredInterp.h (GU Library, C++)
*
* COMMENTS:
*/

#ifndef __GU_SCATTEREDINTERP_H__
#define __GU_SCATTEREDINTERP_H__

#include "GU_API.h"
#include <SYS/SYS_Types.h>

// Scattered Interpolation formulation
enum GU_API GU_ScatteredInterpType
{
    GU_SCATTERED_INTERP_AUTO,
    GU_SCATTERED_INTERP_DOF,
    GU_SCATTERED_INTERP_EXAMPLES,
};

// Scattered interpolation kernel types
enum GU_API GU_RadialBasisKernelType
{
    GU_KERNEL_THINPLATE,
    GU_KERNEL_BIHARMONIC,
    GU_KERNEL_CAUCHY,
    GU_KERNEL_GAUSSIAN,
    GU_KERNEL_MULTIQUADRIC,
    GU_KERNEL_INVERSEMULTIQUADRIC,
    GU_KERNEL_EXPBUMP
};

// Least squares solver types
enum GU_API GU_LeastSquaresSolverType
{
    GU_FAST_LEAST_SQUARES,
    GU_ROBUST_LEAST_SQUARES
};

// Template must match the following matrix-like interface including:
//	accessors for:
//    virtual int rows() const = 0;
//    virtual int columns() const = 0;

//	accessors for row-wise data:
//    virtual fpreal *row(int i) = 0;
//    virtual const fpreal *row(int i) const = 0;

//	or accessors for column-wise data:
//    virtual fpreal *column(int i) = 0;
//    virtual const fpreal *column(int i) const = 0;

template <typename U>
struct GU_Matrix
{
    template <typename V, fpreal * (V::*)(int)> struct Accessor {};
    template <typename V, const fpreal * (V::*)(int) const> struct ConstAccessor {};

    template <typename V> static std::true_type row_test(Accessor <V, &V::row>*);
    template <typename V> static std::false_type row_test(...);

    template <typename V> static std::true_type const_row_test(ConstAccessor <V, &V::row>*);
    template <typename V> static std::false_type const_row_test(...);

    template <typename V> static std::true_type column_test(Accessor <V, &V::column>*);
    template <typename V> static std::false_type column_test(...);

    template <typename V> static std::true_type const_column_test(ConstAccessor <V, &V::column>*);
    template <typename V> static std::false_type const_column_test(...);

    // true if the type has member functions fpreal *row(int) and const fpreal *row(int) const
    static constexpr bool has_row = std::is_same<decltype(row_test<U>(0)), std::true_type>::value &&
	std::is_same<decltype(const_row_test<U>(0)), std::true_type>::value;

    // true if the type has member functions fpreal *column(int) and const fpreal *column(int) const
    static constexpr bool has_column = std::is_same<decltype(column_test<U>(0)), std::true_type>::value &&
	std::is_same<decltype(const_column_test<U>(0)), std::true_type>::value;
};

#endif // __GU_SCATTEREDINTERP_H__
