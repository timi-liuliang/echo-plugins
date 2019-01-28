/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_VertexGroupIterator.h (GA Library, C++)
 *
 * COMMENTS:    Allows iteration through a vertex group in primitive order,
 *              with fast access to the number of the current vertex
 *              within the current primitive.
 *
 */

#ifndef __GA_VertexGroupIterator_h__
#define __GA_VertexGroupIterator_h__

#include "GA_API.h"

#include "GA_Types.h"
#include <UT/UT_Array.h>

class GA_Detail;
class GA_Primitive;
class GA_VertexGroup;

class GA_API GA_VertexGroupIterator
{
public:
    GA_VertexGroupIterator(const GA_VertexGroup *grp)
    {
        setGroup(grp);
    }
    GA_VertexGroupIterator() {}

    void setGroup(const GA_VertexGroup *grp);

    /// @{
    /// Interface to iteration
    void rewind()
    {
        myCurrentIndex = 0;
    }
    bool atEnd() const
    {
        return myCurrentIndex >= myPrimitives.size();
    }
    void advance()
    {
        ++myCurrentIndex;
    }
    GA_VertexGroupIterator &operator++() { advance(); return *this; }
    // No post inc as it is harmful.
    /// @}

    /// Accessor to get the current primitive
    const GA_Primitive *getPrimitive() const;

    /// Accessor to get the current primitive offset
    GA_Offset getPrimitiveOffset() const
    {
        return myPrimitives(myCurrentIndex);
    }

    /// The number for the current vertex within the current primitive.
    /// For example, the vertex offset is, @code
    ///     it.getPrimitive()->getVertexOffset(it.getPrimVertexNum());
    /// @endcode
    GA_Size getPrimVertexNum() const
    {
        return myPrimVertexNums(myCurrentIndex);
    }

private:
    const GA_Detail    *myDetail;
    UT_Array<GA_Offset> myPrimitives;
    UT_Array<GA_Size>   myPrimVertexNums;
    exint               myCurrentIndex;
};

#endif
