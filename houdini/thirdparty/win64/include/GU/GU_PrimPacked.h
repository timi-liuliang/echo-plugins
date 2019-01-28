/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimPacked.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PrimPacked__
#define __GU_PrimPacked__

#include "GU_API.h"
#include "GU_Detail.h"
#include "GU_PackedImpl.h"
#include <GEO/GEO_PrimPacked.h>
#include <GA/GA_PrimitiveDefinition.h>

class GU_PackedFactory;
class GU_PackedImpl;
class GU_PackedContext;
class GEO_ConvertParms;
class UT_MemoryCounter;

class GU_API GU_PrimPacked : public GEO_PrimPacked
{
public:
    // This constructor creates a new GU_PrimPacked but does
    // not append it to the detail
    GU_PrimPacked(GU_PackedImpl *proc,
		GU_Detail *gdp,
		GA_Offset offset=GA_INVALID_OFFSET);
    virtual ~GU_PrimPacked();

    virtual const GA_PrimitiveDefinition &getTypeDef() const;

    /// @{
    /// Find the type information for a given implementation.  May return NULL
    /// or invalid if the type isn't implemented.
    static const GA_PrimitiveDefinition	*lookupTypeDef(const UT_StringRef&type);
    static GA_PrimitiveTypeId		 lookupTypeId(const UT_StringRef &type);
    /// @}

    /// Build a packed primitive of the specified type name in the given detail.
    /// @{
    static GU_PrimPacked *build(GU_Detail &gdp, const UT_StringRef &type,
                                GA_Offset pt_off = GA_INVALID_OFFSET);
    static GU_PrimPacked *build(GU_Detail &gdp, const UT_StringRef &type,
                                const UT_Options &options,
                                GA_Offset pt_off = GA_INVALID_OFFSET);
    static GU_PrimPacked *build(GU_Detail &gdp, const GA_PrimitiveTypeId &type,
                                GA_Offset pt_off = GA_INVALID_OFFSET);
    static GU_PrimPacked *build(GU_Detail &gdp, const GA_PrimitiveTypeId &type,
                                const UT_Options &options,
                                GA_Offset pt_off = GA_INVALID_OFFSET);
    /// @}

    /// Return true if the primitive definition is implemented using the packed
    /// primitive interface.
    static bool		isPackedPrimitive(const GA_PrimitiveDefinition &pdef);
    static bool		isPackedPrimitive(const GA_PrimitiveTypeId &type);
    static bool		isPackedPrimitive(const GA_Primitive &prim)
			{ return isPackedPrimitive(prim.getTypeId()); }

    /// Convenience method to check whether a detail has any packed primitives
    static bool		hasPackedPrimitives(const GA_Detail &gdp);

    /// Convenience method to return the number of packed primitives
    static GA_Size	countPackedPrimitives(const GA_Detail &gdp);

    /// Returns all the packed primitives in the gdp. Unlike the counting
    /// functions above, this function requires iterating through all
    /// primitives.
    static bool		getPackedPrimitives(const GA_Detail &gdp,
				UT_Array<const GA_Primitive *> &prims);

    /// Iterate over all packed primitive types and clear any cached geometry
    /// for the type.  The method returns the number of cached items cleared.
    static exint	clearCachedGeometry();

    /// @{
    /// Implementation of methods from GEO level
    virtual void	stashed(bool beingstashed,
				GA_Offset offset=GA_INVALID_OFFSET);
    virtual bool  	isDegenerate() const;
    virtual void	transform(const UT_Matrix4 &xform);
    virtual fpreal	calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	calcArea() const;
    virtual fpreal	calcPerimeter() const;
    virtual bool	getUntransformedRenderingBounds(
				UT_BoundingBox &b) const;
    virtual void	getVelocityRange(UT_Vector3 &v0, UT_Vector3 &v1) const;
    virtual void	getWidthRange(fpreal &v0, fpreal &v1) const;
    virtual void	getFullTransform4(UT_Matrix4D &xform) const;
    virtual bool	saveOptions(UT_Options &options,
				const GA_SaveMap &map) const;
    virtual bool	loadOptions(const UT_Options &options,
				const GA_LoadMap &map);
    virtual bool	loadOptionsJSON(UT_JSONParser &p,
				const GA_LoadMap &map);
    virtual bool	loadOptionsJSONMap(UT_JSONValueMap &options,
				const GA_LoadMap &map);
    virtual bool	supportsJSONLoad() const;
    virtual bool	loadUnknownToken(const char *token,
				UT_JSONParser &p,
				const GA_LoadMap &map);
    virtual void	copyImplementation(const GEO_PrimPacked &src);
    virtual bool	saveSharedLoadData(UT_JSONWriter &w,
				GA_SaveMap &map, GA_GeometryIndex* geometryIndex) const;
    virtual bool	registerSharedLoadData(int dtype, GA_SharedDataHandlePtr item);
    /// @}
    /// Implementation of methods from GEO level.  If the boxCache() is valid,
    /// it will be used instead of calling the implementation to perform the
    /// computation.  The implementation should make sure to properly call the
    /// dirty() methods.
    virtual bool getUntransformedBounds(UT_BoundingBox &b) const;

    /// Set the value for the path attribute.  If the @c value is @c NULL, the
    /// default primitive "path" from the implementation will be used.
    /// This method may add the "path" attribute if it doesn't already exist.
    bool	setPathAttribute(const char *value=NULL,
			    const char *attrib="path");

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    /// Register a procedural factory.
    static void		registerPacked(GA_PrimitiveFactory *prim,
				    GU_PackedFactory *proc);

    // Conversion Methods
    virtual GEO_Primitive	*convert(GEO_ConvertParms &parms,
					 GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive	*convertNew(GEO_ConvertParms &parms);

    virtual void	normal(NormalComp &output) const;

    /// Return the implementation's packed detail.  This might return a NULL
    /// pointer.
    /// This packed detail should be @b untransformed (i.e. the primitive's
    /// transform shouldn't be included in this).
    GU_ConstDetailHandle	getPackedDetail() const
			    { return implementation()->getPackedDetail(); }
    GU_ConstDetailHandle	getPackedDetail(GU_PackedContext &context) const
			    { return implementation()->getPackedDetail(&context); }
    /// Unpack the packed geometry into the given detail.  This method should
    /// ensure that the unpacked geometry has the primitive's transform
    /// applied.
    bool		unpack(GU_Detail &dest) const
			    { return implementation()->unpack(dest, this); }
    bool		unpackWithContext(GU_Detail &dest, GU_PackedContext &context) const
			    { return implementation()->unpackWithContext(dest, context, this); }
    bool		unpackUsingPolygons(GU_Detail &dest) const
			    { return implementation()->unpackUsingPolygons(dest, this); }

    /// Unpack the packed geometry as with the unpack function. In addition,
    /// returns a STY_Styler for each unpacked primitive.
    bool                unpackWithStyler(
        GU_Detail &dest,
        STY_StylerGroup &prim_styler_group,
        const STY_Styler &parent_styler) const
    {
        return implementation()->unpackWithStyler(
            dest, prim_styler_group, parent_styler, this);
    }

    void		forceLoad() const
			    { return implementation()->forceLoad(); }

    bool		isLoaded() const
			    { return implementation()->isLoaded(); }

    const GU_PackedImpl	*implementation() const	{ return myImplementation; }
    GU_PackedImpl	*implementation()	{ return myImplementation; }

    /// The box cache can be used by the implementation to store the
    /// untransformed bounding box.  It should @b not store the transformed
    /// box.  The cache is automatically cleared when @c attributeDirty() or @c
    /// topologyDirty() are called.
    const UT_BoundingBox	&boxCache() const	{ return myBoxCache; }
    void	setBoxCache(const UT_BoundingBox &b) { myBoxCache=b; }
    void	clearBoxCache()		{ myBoxCache.makeInvalid(); }

    /// @{
    /// Implementation of dirty methods from GEO
    virtual void	 transformDirty();
    virtual void	 attributeDirty();
    virtual void	 topologyDirty();
    /// @}

    /// multiply xform by transform derived from pivot, local, P and instance
    /// attributes
    void	multiplyByPrimTransform(UT_Matrix4D &xform) const;
protected:
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE);
    bool		 doConvert(const GEO_ConvertParms &parms) const;

private:
    GU_PackedImpl	*myImplementation;
    mutable UT_BoundingBox myBoxCache;
    mutable UT_Matrix4D	 myCachedFullTransform;
    mutable int64	 myCachedFullTransformCount;
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
