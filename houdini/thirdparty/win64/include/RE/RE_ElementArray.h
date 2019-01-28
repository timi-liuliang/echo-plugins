/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_ElementArray.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Abstracts a vertex element array, which is used for indexing which
 *	points make up a primitive. This class provides several important
 *	abstractions:
 *
 *	- allows primitives to be specified one at a time or in batches
 *	- dices up long element arrays into sizes the HW can support
 *	- uses a primitive restart index automatically for variable-sized prims
 *	- builds an edge array so that geometry shaders can determine false
 *	  edges (the array is a 2-comp array of {index,maxindex}).
 */
#ifndef RE_ElementArray_h
#define RE_ElementArray_h

class re_ElementChunk;

#include <UT/UT_ValArray.h>
#include "RE_VertexArray.h"
#include "RE_Render.h"

#include <iosfwd>

class RE_API RE_ElementArray 
{
public:
    explicit	RE_ElementArray(bool use_buffers = true);
	       ~RE_ElementArray();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_ElementArray.
    int64 getMemoryUsage(bool inclusive) const;

    // Establish the base cache name for the element arrays created.
    void	setCacheName(const char *cachename);

    // Sets the version of this particular element array.
    void		setCacheVersion(RE_CacheVersion version);
    RE_CacheVersion	getCacheVersion() const;

    // Returns true if all element buffers were found in the cache, with
    // the appropriate version.
    bool	findCachedVersion(RE_CacheVersion version);
    
    // Set up the element buffer type. These cannot be called while within a
    // begin/endPrims(), however you can switch types and start a new
    // begin/endPrims pair (don't do this often, however, as the buffers will
    // be very small chunks and the #draw calls will increase substantially)
    void	setElementType(RE_GPUType type);

    // Set the primitive type that the element array represents. This cannot
    // be changed between begin/endPrims().
    void	setPrimitiveType(RE_PrimType type, int vertices_per_patch = 0);
    
    // Alternate way to set the type, by the length of the vertex array being
    // indexed. 
    void	setNumPoints(int points);

    // If true, quads and polygons are tessellated to triangles. If primitive
    // restart is not supported, polygons are always tessellated to triangles,
    // otherwise triangle tessellation is disabled by default.
    void	tessellateToTriangles(bool enable);

    // enables or disables the generation of a primitive info buffer. It is
    // disabled by default. This also enables triangle tessellation.
    void	requirePrimInfo(bool enable);

    // enables or disables the generation of a vertex info buffer. It is
    // disabled by default. This also enables triangle tessellation.
    void	requireVertexInfo(bool enable);

    // Adds primitive connectivity information to the arrays. beginPrims()
    // must be called before any addPrim() calls are done, and endPrims()
    // before a draw() is issued.

    // beginPrims() may fail if a unbounded primitive is used without the
    // RE_EXT_PRIMITIVE_RESTART extension. 'num_vertex_hint' gives the element
    // array an initial hint as to how many vertices will be in the array.
    bool	beginPrims(RE_Render *r,
			   bool clear_old = false,
			   int num_vertex_hint = 0);

    // Adds a single primitive. May fail if npts doesn't fit the current
    // primitive (4, TRIANGLES). 'real_npnts' is used if mixed size polygons
    // up to 'npnts' is size are being output through a fixed GL size type.
    bool	addPrim(RE_Render *r, int npnts, int *pnt_indices,
			int *prim_info = NULL,
			uint8 *vert_info = NULL,
			int base_prim = -1,
			int base_vert = -1);

    // Specifies indicies for multiple primitives.  
    bool	addPrims(RE_Render *r, int npnts, int *prim_indices,
			 int *prim_info = NULL,
			 uint8 *vert_info = NULL,
			 bool simple_pindex = false,
			 int pindex_start = 0);

    // Special case for pre-convexed models. 
    bool	addTriangle(RE_Render *r, int *pnt_indices, int *prim_info,
			    uint8 *vert_info );
    
    // Must be called after a beginPrims() and before a draw().
    bool	endPrims(RE_Render *r);

    // Upload all information at once without doing begin/end.
    bool	addTriangleChunk(RE_Render *r, int num_triangles,
				 const int *pnt_indices,         // num*3
				 const int *prim_info = NULL,    // num*4
				 const uint8 *vert_info = NULL); // num*4

    // Draws the primitives using the element arrays. Other vertex arrays must
    // already be enabled. Instanced drawing is only supported if
    // RE_EXT_DRAW_INSTANCED is supported.
    // if 'prim' is not RE_PRIM_AS_IS, it will override the primitive type
    // specified by setPrimitiveType.
    void	draw(RE_Render *r,
		     RE_Geometry *geo = NULL,
		     RE_PrimType prim = RE_PRIM_AS_IS);
    void	drawInstanced(RE_Render *r, int num_instances,
			      RE_Geometry *geo = NULL,
			      RE_PrimType prim = RE_PRIM_AS_IS);

    // Returns the minimum and maximum number edges in the primitives added.
    // These are not defined for element arrays fetched from the cache, only
    // newly created ones.
    int		getMinEdges() const;
    int		getMaxEdges() const;

    // Number of arrays created, equal to the number of GL draw calls draw()
    // performs. This is not defined for element arrays fetched from the
    // cache, only newly created ones.
    int		getNumArrays() const;

    int		getNumVertices() const;
    int		getNumOrigVertices() const;
    int		getNumPrimitives() const { return myNumPrims; }
    RE_PrimType getPrimitiveType() const { return myPrimType; }
    int		getVerticesPerPatch() const { return myVerticesPerPatch; }

    /// Return the size of all underlying arrays
    int64	getSizeBytes() const;

    // Only used by RE_Geometry.
    void	clearCachedData() { myHasGeoPrimInfo = -1; }

    // debug print method
    void	print(std::ostream &os); 

private:
    void	completeElementBuffer(RE_Render *r);
    RE_VertexArray *completePrimInfoBuffer(RE_Render *r, const char *cachename);
    RE_VertexArray *completeVertInfoBuffer(RE_Render *r, const char *cachename);
    void	drawPrivate(RE_Render *r, int num_instances,
			    RE_Geometry *geo, RE_PrimType prim);

    void	bumpBufferSizes(int nprims);
    
    RE_GPUType				myElementType;
    RE_PrimType				myPrimType;
    UT_String				myCacheName;
    RE_CacheVersion			myCacheVersion;
    bool				myUseBaseVertex;
    bool				myAddingPrims;
    bool				myUsePrimRestart;
    bool				myLineSegmentMode;
    bool				myFilledMode;
    bool				myRequirePrimInfo;
    bool				myRequireVertInfo;
    bool				myTessellateToTriangles;

    // Global
    int					myMinPrimEdges;
    int					myMaxPrimEdges;
    int					myNumPrims;
    int					myNumOrigVertices;
    int					myPrimIndex;
    int					myVerticesPerPatch;

    // Per chunk
    int					myChunkMinEdges;
    int					myChunkMaxEdges;
    int					myChunkNumPrims;
    int					myChunkMinIndex;
    int					myChunkMaxIndex;

    // Current element chunk
    int					myElementSize;
    int					myElementIndex;
    int				       *myElementBuffer;

    int					myHasGeoPrimInfo;
    int					myPrimInfoSize;
    int					myPrimInfoIndex;
    int				       *myPrimInfoBuffer;
    
    int					myHasGeoVertInfo;
    int					myVertInfoSize;
    int					myVertInfoIndex;
    uint32			       *myVertInfoBuffer;

    // Previously processed chunks
    UT_ValArray<re_ElementChunk *>	myChunks;

    // cached data for drawing
    void			       *myLastShader;
    int					myLastShaderSerial;
    int					myLastShaderPrimInfoIndex;
    int					myLastShaderVertInfoIndex;
};

// Inlines
inline int
RE_ElementArray::getMinEdges() const
{
    return myMinPrimEdges;
}

inline int
RE_ElementArray::getMaxEdges() const
{
    return myMaxPrimEdges;
}

inline int
RE_ElementArray::getNumArrays() const
{
    return myChunks.entries();
}

inline void
RE_ElementArray::draw(RE_Render *r, RE_Geometry *geo, RE_PrimType prim)
{
    drawPrivate(r, 0, geo, prim);
}

inline void
RE_ElementArray::drawInstanced(RE_Render *r, int num_instances,
			       RE_Geometry *geo,
			       RE_PrimType prim)
{
    UT_ASSERT(num_instances>0);
    UT_ASSERT(r->hasGLExtension(RE_EXT_DRAW_INSTANCED));
    if(r->hasGLExtension(RE_EXT_DRAW_INSTANCED))
	drawPrivate(r, num_instances, geo, prim);
}

#endif
