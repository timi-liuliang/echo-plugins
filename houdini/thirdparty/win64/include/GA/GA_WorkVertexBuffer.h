/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_WorkVertexBuffer.h ( GA Library, C++)
 *
 * COMMENTS:	Container to allocate/store temporary vertices
 */

#ifndef __GA_WorkVertexBuffer__
#define __GA_WorkVertexBuffer__

#include "GA_API.h"
#include "GA_Defines.h"
#include "GA_TempBuffer.h"
#include "GA_Types.h"
#include "GA_VertexPool.h"


class GA_API GA_WorkVertexBuffer
{
public:
    /// Construct a buffer of temporary vertices/points for a given detail.
    /// Thes vertices can be used for intermediate computations.
    /// The @c size parameter specifies the initial allocation size of the
    /// buffer.
    ///
    /// When the vertex buffer represents a 2D array of vertices, the @c
    /// matrix_number_columns variable specifies the number of columns in the
    /// array (i.e. nu).
    GA_WorkVertexBuffer(GA_VertexPool &pool, GA_Size size=0,
			GA_Size matrix_number_colums = GA_MAXORDER);
    /// Virtual solely for GEO backward compatibility
    virtual ~GA_WorkVertexBuffer();

    /// Release all the vertices
    void	clear();

    /// @{
    /// Look up the vertex at the index specified.  If the index requested is
    /// greater than the allocated number of vertices, new vertices will be
    /// allocated.  The const method will not append new vertices, and will
    /// return GA_INVALID_INDEX if the index is out of range.
    GA_Offset	getVertexOffset(GA_Size idx, GA_Offset pt=GA_INVALID_OFFSET);
    GA_Offset	getVertexOffset(GA_Size idx) const;
    /// @}

    /// Grow the buffer to the size specified.  Temporary points will be
    /// created for each temporary vertex
    void	growBuffer(GA_Size newsize);

    /// Reserve memory for the given amount of vertices/points without growing
    /// the the buffer. Only has an effect when num_elems is larger than the
    /// current number of entries.
    void	reserve(GA_Size num_elems);

    /// Append a single vertex.  If a point is not specified, a temporary point
    /// will be created to handle point attributes.
    GA_Offset	appendVertexOffset(GA_Offset point=GA_INVALID_OFFSET);

    /// Append a single temporary point.
    GA_Offset	appendPointOffset();


    /// Allocate @c N vertices in the provided buffer
    void	appendVertices(GA_Offset *buffer, GA_Size n)
		{
		    reserve(n);
		    for (GA_Size i = 0; i < n; i++)
			buffer[i] = appendVertexOffset();
		}

    /// Allocate @c N points into the provided buffer
    void	appendPoints(GA_Offset *buffer, GA_Size n)
		{
		    reserve(n);
		    for (GA_Size i = 0; i < n; ++i)
			buffer[i] = appendPointOffset();
		}

    GA_VertexPool	&getVertexPool() const	{ return myVertexPool; }

    /// @{
    /// Query 
    GA_Size	getVertexEntries() const	{ return myVertices.entries(); }
    /// @}

    /// Get a vertex in a mesh (rather than a linear array).  This uses the
    /// MatrixSize to determine the number of columns in the matrix.
    GA_Offset		 getBasisVertexOffset(GA_Size r, GA_Size c,
					GA_Offset pt=GA_INVALID_OFFSET)
			    { return getVertexOffset(r*myMatrixSize+c, pt); }
    /// The matrix size is the number of columns in the 2D array of vertices
    GA_Size		 getMatrixSize() const { return myMatrixSize; }


    /// Swap two entries in the vertex array.  This changes the vertex order in
    /// the buffer, but has no effect on the geometry.
    void	swapVertices(GA_Size i1, GA_Size i2)
		    { myVertices.swap(i1, i2); }

private:
    GA_VertexPoolIndex	appendVertex(GA_Offset point)
			{
			    GA_VertexPoolIndex	vertex;
			    vertex = myVertexPool.appendVertex(point);
			    myVertices.append(vertex);
			    return vertex;
			}

    GA_VertexPool			&myVertexPool;
    GA_TempBuffer<GA_VertexPoolIndex>	 myVertices;
    GA_Size				 myMatrixSize;
};

#endif
