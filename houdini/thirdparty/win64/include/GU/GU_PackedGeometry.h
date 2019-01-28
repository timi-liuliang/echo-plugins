/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedGeometry.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedGeometry__
#define __GU_PackedGeometry__

#include "GU_PackedImpl.h"
#include "GU_DetailHandle.h"
#include <GA/GA_Stat.h>

class GA_PrimitiveFactory;

/// Geometry Embedded procedural
class GU_API GU_PackedGeometry : public GU_PackedImpl
{
public:
	     GU_PackedGeometry();
	     GU_PackedGeometry(const GU_PackedGeometry &src);
    virtual ~GU_PackedGeometry();

    /// Convenience method to create a packed primitive in the destination
    /// detail which embeds the source detail.  The source detail will be
    /// copied into the embedded detail.
    /// The source detail's bounding box will be computed.  (But then
    /// cached in the source detail, so multple packs should only
    /// compute the first time)
    ///
    /// Returns a NULL pointer if the process fails.
    static GU_PrimPacked	*packGeometry(GU_Detail &dest,
					const GU_ConstDetailHandle &gdh);
    /// Provide a bounding box of the gdh, compatible with gdh.getBBox()
    /// useful if you already had to compute it while building gdh.
    /// An invalid bounding box will generate a recompute later,
    /// so can be used if you must delay computation, but this is
    /// not recommended as many readers may demand it simultanesously
    /// leading to performance problems.
    static GU_PrimPacked	*packGeometry(GU_Detail &dest,
					const GU_ConstDetailHandle &gdh,
					const UT_BoundingBox &bbox);

    /// Install the procedural
    static void	install(GA_PrimitiveFactory *prim);

    /// Get the type ID for the GU_PackedGeometry primitive type.
    static GA_PrimitiveTypeId typeId()
    {
        return theTypeId;
    }

    /// @{
    /// Implementation of GU_PackedImpl interface
    virtual GU_PackedFactory	*getFactory() const;
    virtual GU_PackedImpl	*copy() const;

    virtual bool	isValid() const;
    virtual void	clearData();
    virtual bool	load(GU_PrimPacked *prim, const UT_Options &options, const GA_LoadMap &map)
			    { return loadFrom(options, map); }
    virtual bool	supportsJSONLoad() const { return true; }
    virtual bool	loadFromJSON(GU_PrimPacked *prim, const UT_JSONValueMap &options, const GA_LoadMap &map)
			    { return loadFrom(options, map); }
    virtual void	update(GU_PrimPacked *prim, const UT_Options &options);
    virtual bool	save(UT_Options &options, const GA_SaveMap &map) const;
    virtual bool	getBounds(UT_BoundingBox &box) const;
    virtual bool	getRenderingBounds(UT_BoundingBox &box) const;
    virtual void	getVelocityRange(UT_Vector3 &min, UT_Vector3 &max)const;
    virtual void	getWidthRange(fpreal &min, fpreal &max) const;

    virtual bool	unpackWithContext(GU_Detail &destgdp, GU_PackedContext &context, const GU_PrimPacked *prim) const;
    virtual bool	unpack(GU_Detail &destgdp, const UT_Matrix4D *transform) const;
    virtual void	forceLoad() const;
    virtual bool	isLoaded() const;
    virtual GU_ConstDetailHandle	getPackedDetail(GU_PackedContext *context = 0) const;
    virtual bool	loadSharedData(int dtype, const GA_SharedLoadData *it) { UT_ASSERT(0 && "Should not be called!"); return false; }
    virtual bool	saveSharedData(UT_JSONWriter &w,
				GA_SaveMap &map, GA_GeometryIndex* geometryIndex) const;

    /// Report memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// We will be saving the contents in the same file so we don't
    /// have to worry about them changing.  This avoids having
    /// to seek & stat to restore as bbox.
    virtual bool saveCachedBBox() const { return true; }

    /// @}

    /// @{
    /// Member data accessors for intrinsics
    exint geometryId() const
    {
	GU_DetailHandleAutoReadLock gdl(myDetail);
	const GU_Detail *gdp = gdl.getGdp();
	return gdp ? gdp->getUniqueId() : -1;
    }
    exint intrinsicGeometryId(const GU_PrimPacked *prim) const
    {
        return geometryId();
    }
    /// @}

    const GU_ConstDetailHandle	&detailPtr() const;

    void	setDetailPtr(const GU_DetailHandle &d);
    void	setDetailPtr(const GU_ConstDetailHandle &d);
protected:
    template <typename T>
    bool	loadFrom(const T &options, const GA_LoadMap &map);

    bool loadPackedGeometry(GU_PackedContext *context = 0) const;

    bool statPackedGeometry() const;

    GU_ConstDetailHandle		myDetail;
    mutable UT_BoundingBox		myStatBounds;
    mutable GA_SharedDataHandlePtr	mySharedData;
    mutable UT_Lock			mySharedDataLock;

    static GA_PrimitiveTypeId theTypeId;
};

#endif
