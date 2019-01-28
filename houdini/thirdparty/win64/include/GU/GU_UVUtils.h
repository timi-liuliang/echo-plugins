/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 */

#ifndef __GU_UVUtils_h__
#define __GU_UVUtils_h__

#include "GU_API.h"

#include <UT/UT_Matrix.h>
#include <UT/UT_SparseMatrix.h>

#include <Eigen/Sparse>
#include <functional>

namespace GU_UVUtils
{

// Get and set functor for items with integer index.
template <typename T>
using GetFunctor = std::function<T(int)>;

template <typename T>
using SetFunctor = std::function<void(int, T)>;

// Our native sparse matrix, matrix, etc.
template <typename T>
using VectorT = UT_VectorT<T>;
using VectorR = VectorT<fpreal>;

template <typename T>
using MatrixT = UT_MatrixT<T>;
using MatrixR = MatrixT<fpreal>;

template <typename T>
using SparseMatrixT = UT_SparseMatrixCSRT<T>;
using SparseMatrixR = SparseMatrixT<fpreal>;

using TripletR = SparseMatrixR::Triplet;
using TripletRArray = UT_Array<TripletR>;

// Eigen's sparse matrix, vector, etc.
template<typename T>
using EVectorT = Eigen::Matrix<T, Eigen::Dynamic, 1>;
using EVectorR = EVectorT<fpreal>;

template<typename T>
using EMatrixT = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
using EMatrixR = EMatrixT<fpreal>;

template<typename T>
using ESparseMatrixT = Eigen::SparseMatrix<T>;

template<typename T>
using ETripletT = Eigen::Triplet<T>;
using ESparseMatrixR = ESparseMatrixT<fpreal>;

using ETripletR = ETripletT<fpreal>;
using ETripletRArray = UT_Array<ETripletR>;

// Returns a vector representing the most prominent symmetry axis of a set
// of num_tris triangles in the plane. The get_pt_pos is queried for three
// times the number of triangles, ie for points numbers will be in the range
// 0 .. 3 * num_tris - 1.
// The  returned vector will have unit length upon success and will be zero
// otherwise. The centroid of the triangle set can be optionally returned.
// max_eigen_ratio sets the maximum ratio between the smaller and larger
// eigenvalues of the covariance matrix above which the results will be
// discarded and zero vector is returned.

template <typename T>
GU_API UT_Vector2T<T>	 trianglesSymmetryAxis(int num_tris,
				const GetFunctor< UT_Vector2T<T> > &get_pt_pos,
				UT_Vector2T<T> *centroid = nullptr,
				T max_eigen_ratio = T(0.99));

// Returns a vector representing the most prominent symmetry axis of a set
// of num_segs segments in the plane. The get_pt_pos is queried for twice
// the number of segments, ie for points numbers 0 .. 2 * num_segs - 1. The
// returned vector will have unit length upon success and will be zero
// otherwise.

template <typename T>
GU_API UT_Vector2T<T>	 segmentsSymmetryAxis(int num_segs,
				const GetFunctor< UT_Vector2T<T> > &get_pt_pos,
				UT_Vector2T<T> *centroid = nullptr,
				T max_eigen_ratio = T(0.99));

// Returns a vector representing the tangent side of the minimum area bounding
// box of a set of points in the plane. The get_pt_pos is queried each point.
// The returned vector will have unit length upon success and will be zero
// otherwise.

template <typename T>
GU_API UT_Vector2T<T>	 pointsMinAreaBoundingBoxAxis(int num_points,
				const GetFunctor< UT_Vector2T<T> > &get_pt_pos);

// Returns a for a set of vectors a vector representing a direction which
// minimizes (in least squares sense) the angles between the each vector
// and the returned axis direction *or* the direction orthogonal to it. The
// magnitudes of vectors determines their weights. The returned vector will
// have unit length upon success and will be zero otherwise.

template <typename T>
GU_API UT_Vector2T<T>	 vectorsOrthoAxis(int num_vectors,
				const GetFunctor< UT_Vector2T<T> > &get_vector);

} // namespace GU_UVFlatten;


#endif
