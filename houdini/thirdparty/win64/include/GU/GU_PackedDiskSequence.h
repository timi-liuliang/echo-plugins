/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedDiskSequence.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedDiskSequence__
#define __GU_PackedDiskSequence__

#include "GU_PackedSequence.h"
#include "GU_PackedDisk.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringArray.h>

class GA_PrimitiveFactory;
class UT_MemoryCounter;

/// Geometry file procedural
class GU_API GU_PackedDiskSequence : public GU_PackedSequence
{
public:
    using this_type = GU_PackedDiskSequence;
    using parent_type = GU_PackedSequence;

	     GU_PackedDiskSequence();
	     GU_PackedDiskSequence(const GU_PackedDiskSequence &src);
    virtual ~GU_PackedDiskSequence();

    /// Convenience method to create a packed primitive in the destination
    /// detail.
    ///
    /// Returns a NULL pointer if the process fails.
    static GU_PrimPacked	*packedDiskSequence(GU_Detail &dest,
					const UT_StringArray &filenames,
					fpreal index,
					WrapMode wrap);

    /// Install the procedural
    static void	install(GA_PrimitiveFactory *prim);

    /// Get the type ID for the GU_PackedDiskSequence primitive type.
    static GA_PrimitiveTypeId	typeId() { return theTypeId; }

    /// @{
    /// Implementation of GU_PackedImpl interface
    virtual GU_PackedFactory	*getFactory() const;
    virtual GU_PackedImpl	*copy() const;
    virtual void		 clearData();

    virtual bool	isValid() const;
    virtual bool	load(GU_PrimPacked *prim, const UT_Options &options, const GA_LoadMap &map)
			    { return loadFrom(prim, options, map); }
    virtual bool	supportsJSONLoad() const { return true; }
    virtual bool	loadFromJSON(GU_PrimPacked *prim, const UT_JSONValueMap &options,
				const GA_LoadMap &map)
			    { return loadFrom(prim, options, map); }
    virtual void	update(GU_PrimPacked *prim, const UT_Options &options);
    virtual bool	save(UT_Options &options, const GA_SaveMap &map) const;
    virtual bool	isLoaded() const;

    /// Report memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;
    /// @}

    /// @{
    /// Member data accessors for intrinsics
    void filenames(UT_StringArray &files) const;
    void intrinsicFilenames(const GU_PrimPacked *prim, UT_StringArray &files) const
    { filenames(files); }
    void setFilenames(GU_PrimPacked *prim, const UT_StringArray &files);
    /// @}

    UT_StringHolder  filename(exint index) const;
	
    using CacheItem = GU_PackedDisk::CacheItem;

    GU_ConstDetailHandle	getPointCloud() const;

    virtual InstanceKey	getInstanceKey(exint quanize_steps) const;

    GU_ConstDetailHandle getClosestGeometry(fpreal t) const;

protected:
    template <typename T>
    bool	loadFrom(GU_PrimPacked *prim, const T &options, const GA_LoadMap &map);

    virtual exint	sequenceSize() const;
    virtual bool	sequenceBounds(exint index,
				UT_BoundingBox &box) const;
    virtual void	sequenceVelocityRange(exint index,
				UT_Vector3 &min, UT_Vector3 &max) const;
    virtual void	sequenceWidthRange(exint index,
				fpreal &min, fpreal &max) const;

    virtual GU_ConstDetailHandle	sequencePointCloud(exint index) const;
    virtual GU_ConstDetailHandle	sequenceGeometry(exint index) const;

    UT_Array<CacheItem>	myCache;

private:
    static GA_PrimitiveTypeId theTypeId;
};

#endif
