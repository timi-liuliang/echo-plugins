/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedFragment.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedFragment__
#define __GU_PackedFragment__

#include "GU_PackedImpl.h"
#include "GU_DetailHandle.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>

class GA_PrimitiveFactory;

/// Geometry fragment procedural
class GU_API GU_PackedFragment : public GU_PackedImpl
{
public:
	     GU_PackedFragment();
	     GU_PackedFragment(const GU_PackedFragment &src);
    virtual ~GU_PackedFragment();

    /// Convenience method to create a packed fragment primitive in the
    /// destination detail.  The packed fragment primitive will embed the
    /// entire source detail but represent just the subset of the source detail
    /// with primitive attribute "ref" having a value of "val".  The source
    /// detail will be copied into the embedded detail.
    ///
    /// The @c dest detail is the detail in which the packed fragment will be
    /// created.  The @c GU_ConstDetailHandle for the @c src will add a
    /// reference to the detail handle (sharing it between multiple fragments).
    /// The @c attrib refers to a primitive string attribute (i.e. "name")
    /// which is used to specify the houdini primitives which compose the
    /// fragment.  The @c val is used to match the attribute to determine
    /// selection of the primitives of the fragment.  For example: @code
    ///   GU_DetailHandleAutoReadLock rlock(src);
    ///   const GU_Detail *srcgdp = rlock.getGdp();
    ///   GA_ROAttributeRef aref = srcgdp->findStringTuple(
    ///                                 GA_ATTRIB_PRIMITIVE, "name");
    ///   int nstrings = srcgdp->getUniqueValueCount(aref);
    ///   for (int i = 0; i < nstrings; ++i)
    ///   {
    ///       GU_PrimFragment *prim;
    ///       const char *val = srcgdp->getUniqueStringValue(aref, i);
    ///       // Create a primitive in the @c dest detail
    ///       prim = GU_PackedFragment::createFragment(dest, src, aref, val);
    ///   }
    /// @endcode
    ///
    /// Returns a NULL pointer if the process fails.
    ///
    /// NOTE: The const char* overload is to try to avoid ambiguity if some code
    ///       passes nullptr.
    /// @{
    static GU_PrimPacked *createFragment(
        GU_Detail &dest,
        const GU_ConstDetailHandle &src,
        const GA_Attribute *attrib,
        const UT_StringHolder &val);
    static GU_PrimPacked *createFragment(
        GU_Detail &dest,
        const GU_ConstDetailHandle &src,
        const GA_Attribute *attrib,
        const char *val);
    static GU_PrimPacked *createFragment(
        GU_Detail &dest,
        const GU_ConstDetailHandle &src,
        const GA_Attribute *attrib,
        exint val);
    /// @}

    /// Install the procedural
    static void	install(GA_PrimitiveFactory *prim);

    /// Get the type ID for the GU_PackedFragment primitive type.
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
			    { return loadFrom(prim, options, map); }
    virtual bool	supportsJSONLoad() const { return true; }
    virtual bool	loadFromJSON(GU_PrimPacked *prim, const UT_JSONValueMap &options,
				const GA_LoadMap &map)
			    { return loadFrom(prim, options, map); }
    virtual void	update(GU_PrimPacked *prim, const UT_Options &options)
			    { updateFrom(prim, options); }
    virtual bool	save(UT_Options &options, const GA_SaveMap &map) const;
    virtual bool	getBounds(UT_BoundingBox &box) const;
    virtual bool	getRenderingBounds(UT_BoundingBox &box) const;
    virtual void	getVelocityRange(UT_Vector3 &min, UT_Vector3 &max)const;
    virtual void	getWidthRange(fpreal &min, fpreal &max)const;

    virtual bool        unpack(GU_Detail &destgdp, const UT_Matrix4D *transform) const;
    virtual void	forceLoad() const;
    virtual bool	isLoaded() const;
    virtual GU_ConstDetailHandle	getPackedDetail(GU_PackedContext *context = 0) const;
    virtual bool	loadSharedData(int dtype, const GA_SharedLoadData *it)
    {
	UT_ASSERT(false && "Should not be called!");
	return false;
    }
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
    UT_StringHolder attribute() const { return myAttribName; }
    UT_StringHolder intrinsicAttribute(const GU_PrimPacked *prim) const { return attribute(); }
    UT_StringHolder name() const { return myAttribValue; }
    UT_StringHolder intrinsicName(const GU_PrimPacked *prim) const { return name(); }
    void	setAttribute(GU_PrimPacked *prim, const UT_StringHolder &f) { myAttribName = f; }
    void	setName(GU_PrimPacked *prim, const UT_StringHolder &f)
    {
        myIsIntegerAttrib = false;
        myAttribValue = f;
    }
    exint geometryId() const
    {
	loadPackedGeometry();
	GU_DetailHandleAutoReadLock gdl(myDetail);
	const GU_Detail *gdp = gdl.getGdp();
	return gdp ? gdp->getUniqueId() : -1;
    }
    exint intrinsicGeometryId(const GU_PrimPacked *prim) const
    {
        return geometryId();
    }
    /// @}

    const GU_ConstDetailHandle	&detailPtr() const {
	loadPackedGeometry();
	return myDetail;
    }
    GA_Range	getPrimitiveRange() const;
    void	setDetails(GU_PrimPacked *prim, const GU_ConstDetailHandle &d, const GA_Attribute *attrib, const UT_StringHolder &name);
    void	setDetails(GU_PrimPacked *prim, const GU_ConstDetailHandle &d, const GA_Attribute *attrib, const char *name);
    void	setDetails(GU_PrimPacked *prim, const GU_ConstDetailHandle &d, const GA_Attribute *attrib, exint id);
protected:
    /// Returns if the filename has changed
    void		 makeEmbeddedname();

    template <typename T>
    bool	loadFrom(GU_PrimPacked *prim, const T &options, const GA_LoadMap &map);

    template <typename T>
    void	updateFrom(GU_PrimPacked *prim, const T &options);

    virtual bool loadPackedGeometry() const;

    UT_StringHolder			myAttribName;
    UT_StringHolder			myAttribValue;
    exint                               myAttribIntValue;
    GA_AttributeOwner                   myAttribOwner;
    bool                                myIsIntegerAttrib;
    mutable GU_ConstDetailHandle	myDetail;
    mutable const GA_Attribute         *myCachedAttrib;
    mutable GA_SharedDataHandlePtr	mySharedData;
    mutable UT_Lock			mySharedDataLock;
    UT_UniquePtr<fpreal64[]>            myBoundingBox;

    static GA_PrimitiveTypeId theTypeId;
};

#endif
