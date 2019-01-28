/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	The className method is used for saving/loading so there should
 *	  be no spaces in the name.
 *	This class is actually a system of particles instead of a single
 *	  primitive.  This is for efficiency.
 * 
 */

#ifndef __GEO_PrimParticle_H__
#define __GEO_PrimParticle_H__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include "GEO_PrimType.h"
#include "GEO_PartRender.h"

class GA_Detail;
class UT_MemoryCounter;

typedef GA_Size	GEO_ParticleVertexIndex;

#define PART_STATE_PRIMARY              0x0001
#define PART_STATE_DYING                0x0002
#define PART_STATE_STOPPED              0x0004
#define PART_STATE_COLLIDE              0x0008
#define PART_STATE_STUCK                0x0010

// don't move point: dying or stuck or stopped
#define PART_STATE_DONTMOVE		0x0016

// supress default rules
#define PART_STATE_SUPPALL              0x7f00
#define PART_STATE_SUPPPOS              0x0100
#define PART_STATE_SUPPVEL              0x0200
#define PART_STATE_SUPPUP               0x0400
#define PART_STATE_SUPPAGE              0x0800
#define PART_STATE_SUPPREAP             0x1000
#define PART_STATE_SUPPROT              0x2000
#define PART_STATE_SUPPANGVEL           0x4000

// New state flags added in Version 6.0.
#define PART_STATE_SLIDING              0x8000


/// A GEO_PrimParticle represents a particle system, though a simulation may
/// consist of more than a single primitive.  Each particle in the system is
/// a vertex in the primitive.
class GEO_API GEO_PrimParticle : public GEO_Primitive
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimParticle(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimParticle()
    {
#if !GA_PRIMITIVE_VERTEXLIST
        deleteLists();
#endif
    }

public:
    virtual bool	 saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
                            const;
    virtual bool	 loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    /// @{
    /// Save/Load vertex list to a JSON stream
    bool		saveVertexArray(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadVertexArray(UT_JSONParser &p,
				const GA_LoadMap &map);
    /// @}
    /// @{
    /// Save/load render properties to a JSON stream
    bool		saveRenderProperties(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		saveRenderProperties(UT_JSONValue &v,
				const GA_SaveMap &map) const;
    bool		loadRenderProperties(UT_JSONParser &p,
				const GA_LoadMap &map);
    bool		loadRenderProperties(UT_JSONParser &p,
				const UT_JSONValue &v,
				const GA_LoadMap &map);
    bool		areRenderPropertiesEqual(
				const GEO_PrimParticle &src) const;
    /// @}
 
    virtual int		 getBBox(UT_BoundingBox *bbox) const;
    virtual void	 addToBSphere(UT_BoundingSphere *bsphere) const;
    /// @{
    /// Enlarge a bounding box by the bounding box of the primitive.  A
    /// return value of false indicates an error in the operation, most
    /// likely an invalid P.  Unlike getBBox(), velocity is ignored.
    virtual bool	 enlargeBoundingBox(UT_BoundingRect &b,
					const GA_Attribute *P) const;
    virtual bool	 enlargeBoundingBox(UT_BoundingBox &b,
					const GA_Attribute *P) const;
    /// @}
    /// Enlarge a bounding sphere to encompass the primitive.  A return value
    /// of false indicates an error in the operation, most likely an invalid
    /// P.  Unlike getBBox(), velocity is ignored.
    virtual bool	 enlargeBoundingSphere(UT_BoundingSphere &b,
					const GA_Attribute *P) const;
    virtual UT_Vector3	 computeNormal() const;
    virtual UT_Vector3	 baryCenter() const;
    virtual void	 reverse();
    virtual void	 copyPrimitive(const GEO_Primitive *src);
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;
    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);
    virtual bool	 isDegenerate() const;
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	 addPointRefToGroup(GA_PointGroup &grp) const;
#endif

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints (GA_PointGroup &grp);

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

    // The following method simply computes the bounding box.  If no
    // particles exist, returns 0, else return 1.
    int			 getPointBBox(UT_BoundingBox &bbox) const;

    /// @{
    /// Retrieve the object specifying how this particle system should be
    /// rendered.
    GEO_PartRender	 &getRenderAttribs() { return myRenderAttribs; }
    const GEO_PartRender &getRenderAttribs() const { return myRenderAttribs; }
    /// @}

    /// Append a new particle.  @sa giveBirth()
    GA_Offset		 appendParticle(GA_Offset pt);

    /// Delete all particles not on the dead list that reference the supplied
    /// point.
    void		 deleteParticleByPoint(GA_Offset pt);

    /// Delete the specified particle.
    /// @pre vtx belongs to this particle primitive and is not currently on
    /// the dead list.
    /// @sa deadParticle()
    void		 deleteParticle(GA_Offset vtxoff);

    /// Resurrect a particle or give birth to a new one.
    GA_Offset		 giveBirth();

    /// Move the specified particle to the dead list.
    /// @pre vtx belongs to this particle primitive
    /// Does not actually remove from our vertex list!
    /// This builds a list of all vertices that should be deleted
    /// so deleteDead() can do a single pass over them.
    /// vtxindex is the index into our vertex list, not a vertex offset.
    void		 deadParticle(GEO_ParticleVertexIndex vtxindex);

    /// Delete all dead particles.
    void		 deleteDead();

    /// Returns the number of live particles.
    int			 getNumParticles() const { return myVertexList.entries(); }

#if !GA_PRIMITIVE_VERTEXLIST
    ///
    // Methods to handle vertex attributes for the attribute dictionary
    ///
    virtual bool	vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;

    /// @note getVertexCount() includes dead particles.
    virtual GA_Size	getVertexCount() const;

    /// @warning Traverses the linked list of vertices.
    virtual GA_Offset	getVertexOffset(GA_Size index) const;

    void		setVertexPoint(unsigned int i, GA_Offset pt)
			{
			    if (i < myVertexList.entries())
				wireVertex(myVertexList(i), pt);
			}

    virtual void	beginVertex(const_iterator &i) const;
    virtual void	nextVertex(const_iterator &i) const;
#endif

    void			 clearAndDestroy()
				 { deleteLists(); }

    // Have we been deactivated and stashed?
    virtual void	stashed(bool beingstashed,
					 GA_Offset offset = GA_INVALID_OFFSET);

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// @warning vertexPoint() doesn't check the bounds.  Use with caution.
    GA_Offset		vertexPoint(GA_Size i) const
			    { return getDetail().vertexPoint(myVertexList(i)); }

    const GA_OffsetList &getVertexList() const
			    { return myVertexList; }

protected:
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	clearForDeletion();
#endif

    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GA_FAMILY_NONE; }

    // Declare intrinsic attribute methods
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);
#endif

    /// Resize our vertex array
    void		setSize(GA_Size sz);

    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored here. Return 0 if
    // OK and -1 otherwise.
    virtual bool	 evaluatePointRefMap(GA_Offset result_vtx,
					GA_AttributeRefMap &hlist,
					fpreal u, fpreal v,
					unsigned du, unsigned dv) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos,
					float u_unit, float=0,
					unsigned du=0, unsigned = 0) const;
    virtual bool	 evaluateBaryCenterRefMap(GA_Offset result_vertex,
				GA_AttributeRefMap &hlist) const;

    // This is called by the subclass to get the
    // memory used by myVertexList and myDeadList
    int64 getBaseMemoryUsage() const;

    // This is called by the subclass to count the
    // memory used by myVertexList and myDeadList
    void countBaseMemory(UT_MemoryCounter &counter) const;

private:
    void		 deleteLists();
    void		 deleteParticleVertex(GA_Offset vtxoff)
			 {
			    destroyVertex(vtxoff);
			 }

    GEO_PartRender				myRenderAttribs;
#if !GA_PRIMITIVE_VERTEXLIST
    GA_OffsetList				myVertexList;
#endif
    UT_ValArray<GEO_ParticleVertexIndex>	myDeadList;

    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimParticle &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
