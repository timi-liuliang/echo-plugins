/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_VertexPool.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_VertexPool__
#define __GA_VertexPool__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_TempBuffer.h"

#define GA_VERTEX_POOL_SIZE	32

class GA_Detail;

#if defined(GA_STRICT_TYPES)
    #include <SYS/SYS_TypeDecorate.h>
    class GA_VertexPoolIndexTag {};
    typedef GA_OrdinalType<GA_VertexPoolIndexTag, GA_Size> GA_VertexPoolIndex;
    SYS_DECLARE_IS_POD(GA_VertexPoolIndex)
#else
    typedef GA_Size	GA_VertexPoolIndex;
#endif

/// @brief Manages allocation/deletion of temporary vertex/points for a detail.
///
/// This class stores a list of temporary points and vertices for a detail.
/// The class is used by GA_WorkVertexBuffer and is referenced by handle lists.
class GA_API GA_VertexPool
{
private:
    /// Objects are private to the attribute handle lists
     GA_VertexPool();
    ~GA_VertexPool();

    void	bindDetail(GA_Detail &gdp);
    void	bindDetail(const GA_Detail &gdp);
    void	unbindDetail();

    friend class GA_AttributeRefMap;
    friend class ga_VertexPoolProxy;
    friend class GEO_AttributeHandleList;

public:
    /// Return whether this pool is bound to a detail, and hence available
    /// for us.
    bool	 isBound() const { return myDetail != 0; }

    /// Return the detail which holds the temporary vertices/points.
    /// @note This may @b not be the same as the detail passed into the
    /// constructor.
    GA_Detail	&getDetail()
		{
		    return *myDetail;
		}

    /// If the vertex pool was constructed using a const detail, the pool will
    /// have made a copy of the detail and this method will return true.
    bool	isDetailCopied() const
		{
		    return myCopiedDetail;
		}

    /// Look up the GA_Offset at the given index
    GA_Offset	getVertex(GA_VertexPoolIndex index) const
		{
		    return myVertices(index);
		}
    GA_Offset	getPoint(GA_VertexPoolIndex index) const
		{
		    return myPoints(index);
		}

    /// Number of vertices in the vertex pool
    GA_Size	entries() const		{ return myVertices.entries(); }

    /// Reserve memory for the given amount of vertices without changing the
    /// number of entries. Only has an effect if num_elems > entries().
    void	reserve(GA_Size num_elems);

    /// Append a single vertex (with an optional point link)
    /// The method returns the offset in the pool (not the offset in the detail)
    GA_VertexPoolIndex	appendVertex(GA_Offset pt=GA_INVALID_OFFSET);

    /// Rewire a single vertex to a new point link.
    void		rewireVertexPoint(GA_VertexPoolIndex index, 
					  GA_Offset point);

    /// Append @c nvertex vertices to the pool.  The offsets are returned in
    /// the buffer provided.
    void	appendVertices(GA_VertexPoolIndex *buffer, GA_Size nvertex)
		{
		    for (GA_Size i = 0; i < nvertex; ++i)
			buffer[i] = appendVertex();
		}

    /// Append a single point
    /// The method returns the offset in the pool (not the offset in the detail)
    GA_VertexPoolIndex	appendPoint();

    /// Append @c nvertex vertices to the pool.  The offsets are returned in
    /// the buffer provided.
    void	appendPoints(GA_VertexPoolIndex *buffer, GA_Size nvertex)
		{
		    for (GA_Size i = 0; i < nvertex; ++i)
			buffer[i] = appendPoint();
		}


    /// Free a vertex.  If the index is invalid (i.e. < 0), the free is
    /// silently skipped.
    void	freeVertex(GA_VertexPoolIndex index);

    /// Free a list of vertices
    void	freeVertices(const GA_VertexPoolIndex *list, GA_Size nvertex)
		{
		    for (GA_Size i = 0; i < nvertex; ++i)
			freeVertex(list[i]);
		}

    /// Free a point.  If the index is invalid (i.e. < 0), the free is
    /// silently skipped.  Vertices should be free'd before points.
    void	freePoint(GA_VertexPoolIndex index);

    /// Free a list of points.  Vertices should be free'd before points.
    void	freePoints(const GA_VertexPoolIndex *list, GA_Size nvertex)
		{
		    for (GA_Size i = 0; i < nvertex; ++i)
			freePoint(list[i]);
		}

    const GA_Offset	*array() const		{ return myVertices.data(); }
    GA_Offset		*array()		{ return myVertices.data(0); }

private:
    void			 clear();
    GA_Offset			 validatePoint(GA_Offset point);

    GA_Detail			*myDetail;
    GA_TempBuffer<GA_Offset>	 myVertices;
    GA_TempBuffer<GA_Offset>	 myPoints;
    GA_Size			 myVCount, myPCount;
    bool			 myCopiedDetail;
};

#endif
