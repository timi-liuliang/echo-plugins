/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEOWorkVertexBuffer.h ( GEO Library, C++)
 *
 * COMMENTS:	A work-buffer of temporary vertices.  These are allocated on
 *		demand and free'd when the class is destructed.  They are
 *		intended to be used as temporary stack storage, and shouldn't
 *		be persistent.
 *
 *		Each vertex is indexed by a unique integer (like an array).
 *		Accessing a vertex which hasn't been accessed already will
 *		cause the vertex to be created.
 *
 *		Each vertex will also have a point allocated for it, and the
 *		point associated with the vertex should NOT be changed.
 */

#ifndef __GEO_WorkVertexBuffer__
#define __GEO_WorkVertexBuffer__

#include "GEO_API.h"
#include <UT/UT_Array.h>
#include <GA/GA_WorkVertexBuffer.h>
#include "GEO_Vertex.h"
#include "GEO_Detail.h"

class GEO_Detail;
class GEO_Vertex;

#define GEO_WORK_VERTEX_ARRAY_SIZE	128

SYS_DEPRECATED_PUSH_DISABLE()

class GEO_API GEO_WorkVertexBuffer
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GEO_WorkVertexBuffer(
	    GA_VertexPool &pool,
	    GA_Size initial_size = 0,
	    GA_Size matrix_size = GA_MAXORDER);
    SYS_DEPRECATED_HDK(13.0)
    virtual ~GEO_WorkVertexBuffer();

    /// Clear all vertices out of the work vertex buffer.
    SYS_DEPRECATED_HDK(13.0)
    void	clear();

    /// @{
    /// If the vertex isn't already allocated at the index passed in, it will be
    /// created.  The const method may return a NULL pointer if the vertex
    /// hasn't been allocated.
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex	*getVertexElement(GA_Size idx);
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Vertex	*getVertexElement(GA_Size idx) const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex	*appendVertexElement();
    /// @}

    /// Return the number of vertices allocated in the buffer
    SYS_DEPRECATED_HDK(13.0)
    GA_Size	getVertexEntries() const { return myVertices.entries(); }

    /// Swap two entries in the vertex array.  This changes the vertex numbers
    /// associated with given entries in the vertex buffer.
    SYS_DEPRECATED_HDK(13.0)
    void	swapVertices(GA_Size i1, GA_Size i2);

private:
    GA_WorkVertexBuffer		myGAVertices;
    UT_Array<GEO_Vertex *>	myVertices;
};

class GEO_API GEO_WorkVertexArray
{
public:
    // Many of the curve/surface methods require an array of vertices.  Rather
    // than worrying about allocating/deleting dynamic arrays, the
    // work-vertex-array can be used.
    //
    // Vertices are allocated using the work-vertex-buffer, so it must remain
    // in scope as long as the array.
    //
    SYS_DEPRECATED_HDK(13.0)
    GEO_WorkVertexArray(GEO_WorkVertexBuffer &buffer, GA_Size size);
    SYS_DEPRECATED_HDK(13.0)
    virtual ~GEO_WorkVertexArray();

    // Get a pointer to the array of GEO_Vertex objects
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex	**getVertexElements() const	{ return myVertices; }

    /// Query size
    SYS_DEPRECATED_HDK(13.0)
    GA_Size	entries() const	{ return mySize; }

private:
    GEO_Vertex		**myVertices;
    GEO_Vertex		*myStorage[GEO_WORK_VERTEX_ARRAY_SIZE];
    GA_Size		 mySize;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
