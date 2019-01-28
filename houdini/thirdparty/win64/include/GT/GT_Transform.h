/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Transform.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_Transform__
#define __GT_Transform__

#include "GT_API.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Matrix4.h>
#include "GT_Handles.h"

class UT_JSONWriter;

/// A simple interface to provide multiple-segments of transforms.
class GT_API GT_Transform
    : public UT_IntrusiveRefCounter<GT_Transform>
{
public:
     /// Default c-tor
     GT_Transform();
     /// Copy c-tor
     GT_Transform(const GT_Transform &src);
     /// Initialize with single precision transforms
     GT_Transform(const UT_Matrix4F *xforms, int nsegments);
     /// Initialize with double precision transforms
     GT_Transform(const UT_Matrix4D *xforms, int nsegments);
    ~GT_Transform();

    /// Assignment operator
    GT_Transform	&operator=(const GT_Transform &m);

    /// Equality operation
    bool	operator==(const GT_Transform &other) const;
    bool	operator!=(const GT_Transform &other) const
		    { return !(*this == other); }

    /// Get the identity transform (as a handle)
    static const GT_TransformHandle &identity();

    /// @{
    /// Initialize the matrix
    void	alloc(int nsegments);
    void	setMatrix(const UT_Matrix4F &xform, int segment);
    void	setMatrix(const UT_Matrix4D &xform, int segment);
    /// @}

    /// @{
    /// Multiply this transform by a single matrix.  This applies the matrix to
    /// all the embedded transforms.
    GT_Transform	*preMultiply(const UT_Matrix4D &xform) const;
    GT_Transform	*multiply(const UT_Matrix4D &xform) const;
    GT_Transform	*preMultiply(const UT_Matrix4F &xform) const;
    GT_Transform	*multiply(const UT_Matrix4F &xform) const;
    GT_TransformHandle	 preMultiply(const GT_Transform &x) const;
    GT_TransformHandle	 multiply(const GT_Transform &x) const;
    /// @}

    /// @{
    /// Multiply by the 3x3 transform (not including translates)
    GT_Transform	*preMultiply(const UT_Matrix3D &xform) const;
    GT_Transform	*multiply(const UT_Matrix3D &xform) const;
    GT_Transform	*preMultiply(const UT_Matrix3F &xform) const;
    GT_Transform	*multiply(const UT_Matrix3F &xform) const;
    /// @}

    /// Compute inverse transforms.  This will be done lazily when getting
    /// inverse transforms.
    void	computeInverse();

    /// Return approximate memory usage.
    int64	getMemoryUsage() const;

    /// Return motion segments
    int		getSegments() const	{ return myMotionSegments; }

    /// @{
    /// Extract a matrix
    void	getMatrix(UT_Matrix4F &xform, int segment=0) const;
    void	getMatrix(UT_Matrix4D &xform, int segment=0) const;
    /// @}
    /// @{
    /// Extract a matrix
    void	getInverse(UT_Matrix4F &xform, int segment=0) const;
    void	getInverse(UT_Matrix4D &xform, int segment=0) const;
    /// @}

    /// @{
    /// Transform a data array (using the type info on the array)
    GT_DataArrayHandle	transform(const GT_DataArrayHandle &src,
				    int segment=0) const;
    /// @}

    /// Return the inverse transform
    GT_TransformHandle	invert() const;

    /// Test whether the transform is an identity matrix
    bool	isIdentity() const	{ return myIdentity; }

    /// Save to a JSON stream/value
    bool	save(UT_JSONWriter &w) const;

    /// Load from a JSON stream/value
    static bool	load(GT_TransformHandle &x, UT_JSONParser &p);

    /// Save the transform to stdout
    void	dump(const char *msg="") const;

private:
    void		 clear();
    void		 clearInverse();
    void		 checkIdentity();

    // Keep the 32 bit int first in the structure.  Since we inherit from
    // GA_IntrusiveRefCounter, this alignment makes the object smaller.
    int			 myMotionSegments;
    UT_Matrix4D		*myMatrix;
    UT_Matrix4D		*myInverse;
    UT_Matrix4D		 mySingleMatrix;
    bool		 myIdentity;
};

#endif
