/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_TransformArray.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_TransformArray__
#define __GT_TransformArray__

#include "GT_API.h"
#include "GT_Handles.h"
#include "GT_Types.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>

class UT_JSONWriter;

class GT_API GT_TransformArray
    : public UT_IntrusiveRefCounter<GT_TransformArray>
{
public:
	     GT_TransformArray() {}
    virtual ~GT_TransformArray() { setEntries(0); }

    /// Return the number of entries
    GT_Size		entries() const { return myTransforms.entries(); }

    /// Return approximate memory usage
    int64		getMemoryUsage() const;

    /// Return the number of motion segments.  We assume that all transforms
    /// have the same number of segments.
    int			getMotionSegments() const;

    /// Set the number of entries
    void		setEntries(GT_Size n);

    /// Return a transform handle
    const GT_TransformHandle	&get(GT_Size i) const
				    { return myTransforms(i); }
    /// Set a transform handle
    void			 set(GT_Size i, const GT_TransformHandle &h)
				    { myTransforms(i) = h; }

    /// Append a transform handle
    void			 append(const GT_TransformHandle &h);

    /// Multiply all the transforms by a single transform
    GT_TransformArrayHandle	multiply(const GT_TransformHandle &h) const;

    /// Pre-multiply all the transforms by a single transform
    GT_TransformArrayHandle	preMultiply(const GT_TransformHandle &h) const;

    /// @{
    /// For memory tracking, we override the new/delete operators
    static void	*operator	new(size_t size);
    static void	*operator	new(size_t size, void *p);
    static void	 operator	delete(void *p, size_t size);
    /// @}

    /// Save to JSON
    bool	save(UT_JSONWriter &w) const;

    void	setTransforms(const UT_Matrix4FArray &transforms);
    void	setTransforms(const UT_Matrix4DArray &transforms);

    void	getTransforms(UT_Matrix4FArray &transforms);
    void	getTransforms(UT_Matrix4DArray &transforms);

    /// Data id support for derived classes (-1 means always re-update).
    virtual int64 getDataId() const { return -1; }

    /// Dump array
    void	dump(const char *msg="") const;

private:
    UT_Array<GT_TransformHandle>	myTransforms;
};

#endif
