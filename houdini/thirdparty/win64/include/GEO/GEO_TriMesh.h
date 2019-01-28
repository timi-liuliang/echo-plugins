/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimTriMesh.h (GEO Library, C++)
 *
 * COMMENTS:	This is the base class for all triangle mesh types.
 */

#pragma once

#ifndef __GEO_TriMesh__
#define __GEO_TriMesh__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include "GEO_Vertex.h"
#include <UT/UT_Array.h>
#include <SYS/SYS_Inline.h>
#include <iosfwd>

class GA_Detail;
class UT_JSONWriter;
class UT_JSONParser;
class UT_JSONValue;
class GA_SaveMap;
class GA_LoadMap;

SYS_DEPRECATED_PUSH_DISABLE()

class GEO_API GEO_TriMesh : public GEO_Primitive
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    SYS_FORCE_INLINE
    GEO_TriMesh(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
    {}

#if !GA_PRIMITIVE_VERTEXLIST
    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_TriMesh();
#endif
public:
    virtual int		getBBox(UT_BoundingBox *bbox) const;
    virtual void	addToBSphere(UT_BoundingSphere *bsphere) const;
    virtual UT_Vector3	baryCenter() const;
    virtual bool	saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;
    virtual bool	loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    /// @{
    /// Method to load/save basis values.  Default methods return false (fail)
    virtual bool	jsonSaveBasis(UT_JSONWriter &w) const;
    virtual bool	jsonSaveBasis(UT_JSONValue &v) const;
    virtual bool	jsonLoadBasis(UT_JSONParser &p);
    virtual bool	jsonLoadBasis(UT_JSONParser &p, const UT_JSONValue &v);
    /// @}

    /// @{
    /// Save/Load vertex list to a JSON stream
    bool		saveVertexArray(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadVertexArray(UT_JSONParser &p,
				const GA_LoadMap &map);
    /// @}

#if !GA_PRIMITIVE_VERTEXLIST
    SYS_DEPRECATED_HDK(13.0)
    virtual void	copyPrimitive(const GEO_Primitive *src);
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;

    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);

    virtual void	addPointRefToGroup(GA_PointGroup &grp) const final;
#endif
    virtual bool	changePointRef(GA_Offset from, GA_Offset to);

    virtual bool  	isDegenerate() const;

    // Adjust the size of the vertex array.
#if GA_PRIMITIVE_VERTEXLIST
    SYS_FORCE_INLINE
    void setSize(GA_Size nvertices)
    { setNumVertices(nvertices); }
#else
    void		 setSize(GA_Size sz);
#endif

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints(GA_PointGroup &grp);

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

    // Insert or delete vertices. The insertion methods return the index if
    // successful and -1 otherwise. The deletion methods return 0 if ok and
    // -1 otherwise.
    virtual GA_Size	insertVertex(GA_Offset ppt, GA_Size where=0);
    virtual GA_Size	appendVertex(GA_Offset ppt);
    virtual int		deleteVertex(GA_Size num);

    /// Remove all vertices listed in the array.
    /// The array must be in order, contain no duplicates, and all
    /// numbers must be in the range [0, getVertexCount()).
    virtual void	deleteVertices(const UT_Array<GA_Size> &nums);

    /// return the index of a vertex within our vertex list
    GA_Size		findVertex(GA_Offset vtx) const { return myVertexList.find(vtx); }

    /// Steal a vertex from its current primitive and insert it into our vertex list.
    ///
    /// @param vtx Vertex to be stolen
    /// @param insert_before_vtx (Optional) the vertex before which to insert the stolen vertex
    ///        If unspecified (negative) or if the vertex doesn't exist, the stolen vertex is
    ///        appended at the end of the vertex list.
    /// @return The position at which vtx is inserted if successful, or -1 otherwise.
    virtual GA_Size	stealVertex(GA_Offset vtx,
                            GA_Offset insert_before_vtx = GA_INVALID_OFFSET);

    // Query the number of vertices in the array. This number may be smaller
    // than the actual size of the array.
#if !GA_PRIMITIVE_VERTEXLIST
    virtual GA_Size getVertexCount() const final
    { return myVertexList.size(); }
#endif
    SYS_FORCE_INLINE GA_Size getFastVertexCount() const
    { return myVertexList.size(); }

#if !GA_PRIMITIVE_VERTEXLIST
    virtual GA_Offset getVertexOffset(GA_Size index) const final
    {
        UT_ASSERT_P(index >= 0 && index < myVertexList.size());
        return myVertexList(index);
    }
#endif
    SYS_FORCE_INLINE GA_Offset getFastVertexOffset(GA_Size index) const
    {
        UT_ASSERT_P(index >= 0 && index < myVertexList.size());
        return myVertexList(index);
    }

#if !GA_PRIMITIVE_VERTEXLIST
    SYS_FORCE_INLINE GA_Index getVertexIndex(GA_Size i) const
    { return getDetail().vertexIndex(getFastVertexOffset(i)); }
    SYS_FORCE_INLINE GA_Offset getPointOffset(GA_Size i) const
    { return getDetail().vertexPoint(getFastVertexOffset(i)); }
    SYS_FORCE_INLINE GA_Index getPointIndex(GA_Size i) const
    { return getDetail().pointIndex(getPointOffset(i)); }
    SYS_FORCE_INLINE UT_Vector3 getPos3(GA_Size i) const
    { return getDetail().getPos3(getPointOffset(i)); }
    SYS_FORCE_INLINE void setPos3(GA_Size i, const UT_Vector3 &pos) const
    { getDetail().setPos3(getPointOffset(i), pos); }
    SYS_FORCE_INLINE UT_Vector4 getPos4(GA_Size i) const
    { return getDetail().getPos4(getPointOffset(i)); }
    SYS_FORCE_INLINE void setPos4(GA_Size i, const UT_Vector4 &pos) const
    { getDetail().setPos4(getPointOffset(i), pos); }

    /// A trivial vertex list is a uniform ascending list, so
    /// getFastVertexOffset(i) == getFastVertexOffset(0) + i
    bool		isVertexListTrivial() const { return myVertexList.isTrivial(); }
    /// A trivial point list is doubly-trivial: 
    /// getPointOffset(i) == getPointOffset(0) + i
    /// This requires testing every point so is slower than isVertexListTrivial.
    bool		isPointListTrivial() const;

    virtual void	beginVertex(const_iterator &i) const final;
    virtual void	nextVertex(const_iterator &i) const final;
#endif

    // Find the index of the given vertex that refers to pt.
    // Return -1 if not found.
    GA_Size	find(GA_Offset pt) const;

    // Subscript operators. The () operator does not check the boundaries.
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Vertex	operator()(GA_Size i) const
					    { return getVertexElement(i); }
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex		operator()(GA_Size i)
					    { return getVertexElement(i); }
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Vertex	operator[](GA_Size i) const
					    { return getVertexElement(i); }
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex		operator[](GA_Size i)
					    { return getVertexElement(i); }

    void setVertexPoint(GA_Size i, GA_Offset pt)
    {
        if (i < myVertexList.entries())
            wireVertex(myVertexList(i), pt);
    }

#if !GA_PRIMITIVE_VERTEXLIST
    // Have we been deactivated and stashed?
    virtual void	 stashed(bool beingstashed,
					GA_Offset offset = GA_INVALID_OFFSET);
#endif

    // Methods to handle vertex attributes for the attribute dictionary
    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const final;

    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm)const;
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm, float tolerance)const;

    virtual void	 unitToUnitLengthPair(float  uparm,   float  vparm,
					      float &ulength, float &vlength)
									const;

    /// Replaces the entire vertex list.
    /// Use with *utmost* caution. It probably won't work with Nurbs or
    /// Polysoups, and possibly other primitives and is only tested for
    /// Polygons.
    ///
    /// @param destroy_existing
    ///     If set, existing vertex offsets are destroyed.
    /// @param update_topology
    ///     If set, new vertices are wired to the primitive.
    void                 assignVertexList(const GA_OffsetList &list,
                                          bool destroy_existing = true,
                                          bool update_topology = true);

protected:
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	clearForDeletion();
#endif

    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GA_FAMILY_NONE; }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Primitive::registerIntrinsics(defn); }

    virtual bool savePrivateH9(std::ostream &os, bool binary) const = 0;
    virtual bool loadPrivateH9(UT_IStream &is) = 0;

    // Check the validity of the data. Meant to be called especially at loading
    // time.
    virtual bool	 validate(void) const;

    // Allow derived classes to override the minimum number of vertices needed
    // for a non-degenerate primitive.
    virtual GA_Size	 getMinVertexCount() const { return 3; }

    // Insert a vertex multiple times in the list. Return the index.
    GA_Size		 multipleInsert(GA_Size where,
				        GA_Size count,
				        bool appendPoints = true);

    /// @warning vertexPoint() doesn't check the bounds.  Use with caution.
    SYS_FORCE_INLINE GA_Offset vertexPoint(GA_Size i) const
			    { return getDetail().vertexPoint(myVertexList(i)); }

    /// @warning swapVertices() doesn't check the bounds, nor that j != i.  Use
    /// with caution.
    void		swapVertices(GA_Size i, GA_Size j)
			{
			    GA_Offset	tmp = myVertexList(i);
			    myVertexList.set(i, myVertexList(j));
			    myVertexList.set(j, tmp);
			}

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);
#endif

#if !GA_PRIMITIVE_VERTEXLIST
    GA_OffsetList	myVertexList;
#endif

    virtual bool	evaluatePointRefMap(GA_Offset result_vertex,
				GA_AttributeRefMap &hlist,
				fpreal u, fpreal v, uint du, uint dv) const;
    virtual int		evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const
			    { return GEO_Primitive::evaluatePointV4(pos, u, v, du, dv); }

    virtual bool	evaluateBaryCenterRefMap(GA_Offset result_vertex,
				GA_AttributeRefMap &hlist) const;

    /// Release the vertex vtx, i.e. deletes it from myVertexList but doesn't delete
    /// the offset itself. Consequently, topology information and attributes are preserved.
    /// It's meant to be used by stealVertex
    /// @see stealVertex
    /// @note overriding from GA_Primitive
    virtual GA_Offset	releaseVertex(GA_Offset vtx);

#if !GA_PRIMITIVE_VERTEXLIST
    /// Report approximate memory usage for myVertexList for subclasses.
    int64 getBaseMemoryUsage() const;

    /// This is called by the subclasses to count the
    /// memory used by myVertexList
    void countBaseMemory(UT_MemoryCounter &counter) const;
#endif

private:
    

    friend std::ostream	&operator<<(std::ostream &os, const GEO_TriMesh &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};

SYS_DEPRECATED_POP_DISABLE()

#endif

