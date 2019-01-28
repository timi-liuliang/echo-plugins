/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedImpl.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedImpl__
#define __GU_PackedImpl__

#include "GU_API.h"
#include <GEO/GEO_PackedTypes.h>
#include <GA/GA_Types.h>
#include "GU_Detail.h"
#include "GU_DetailHandle.h"

class UT_MemoryCounter;
class UT_Options;
class GU_PackedFactory;
class GU_PrimPacked;
class GA_LoadMap;
class GA_Primitive;
class GA_SaveMap;
class STY_Styler;
class STY_StylerGroup;
class GU_PackedContext;

/// This class is used by the deferred load primitives to provide methods to
/// handle the deferred geometry.
///
/// <b>Load/save of shared primitive data</b>
/// If multiple instances of the packed primitive share data, it's possible to
/// save this data just once (instead of per primitive).
///
/// There are two parts to saving shared data.  Saving the actual data, but
/// also saving a "key" used to resolve the shared data.  The shared data
/// itself is saved using @c saveSharedData().  In addition, the "key"
/// associated with the shared data is saved per-primitive instance.
/// During loading, the "key" is used to resolve the shared data.
///
/// When generating a key, the key must be unique within all shared data blocks
/// for a single geometry.  A simple way to do this would be to use the address
/// of the shared data item.  More complicated ways might include using a GUID,
/// etc.  For example, with a @c UT_SharedPtr<GU_Detail>, you might have
/// generate a key using: @code
/// @code
/// UT_StringHolder     makeKey(const UT_SharedPtr<GU_Detail> &detail) {
///     UT_WorkBuffer   key;
///     key.sprintf("sharedgeo:%p", detail.get());
///     return UT_StringHolder(key);
/// }
/// @endcode
///
/// The @c saveSharedData() method writes a single JSON object to the save
/// stream.  During loading, the loader expects to load an array containing
/// four items. @code
/// [
///     "string Primitive_Name",
///     "string Shared_Data_Type",
///     "string Shared_Data_Identifier",
///     shared_data
/// ]
/// @endcode
/// The @c Primitive_Name ensures that shared data resolution is tied to the
/// specific primitive type.  Kind of like a namespace.
///
/// The @c Shared_Data_Type is an identifier to allow primitives to store
/// multiple types of shared data.  For example, if you have a material
/// definition and a geometry definition, these can be stored as separate
/// shared blocks rather than creating separate shared blocks for each pair.
///
/// The @c Shared_Data_Identifier is a unique identifier used to specify a
/// particular instance of a shared data block.  This is the "key" that can be
/// used to resolve the data at load time. See the @c makeKey() example above.
///
/// Example code might be something like @code
///   UT_StringHolder     key = makeKey(mySharedObject);
///   if (save_map.hasSavedSharedData(key))
///       return true;    // Another primitive has saved the shared data
///   map.setSavedSharedData(key);
///   json.jsonStringToken(getFactory()->name());
///   // Used for seeking (see section on loading)
///   geometryIndex->addEntry(key, json.getNumBytesWritten());
///
///   json.beginArray();
///   json.jsonString("Shared_Data_Type");
///   json.jsonString(key);
///   json.jsonBeginMap();
///       ...
///   json.jsonEndMap();
///
///   // Provide a seek point used for loading
///   UT_WorkBuffer       endkey(key);
///   endkey.append(GA_SAVEMAP_END_TOKEN);
///   geometryIndex->addEntry(endkey.buffer(), w.getNumBytesWritten());
///   ok = ok && w.jsonEndArray();
/// @endcode
///
/// The @c save() method needs to store the key so the shared data can be
/// resolved at load time.  The identifier you use in the save method doesn't
/// have to be the @c "Shared_Data_Type" string, but it does have to match the
/// value in the load method.  For example: @code
///     PrimClass::save(UT_Options &options, const GA_SaveMap &map) const {
///          options.setOptionS("shareddata", makeKey());
///     }
/// @endcode
///
/// Shared data loading may be deferred until the shared data is actually
/// required.  This is done using a "handle" to the shared data (@c
/// GA_SharedDataHandlePtr).  During the load() method, you can ask for a
/// handle to any shared data. @code
///  bool load(const UT_Options &options, const GA_LoadMap &loadmap) {
///     UT_StringHolder key;
///     if (import(options, "shareddata", key)) {
///         mySharedHandle = loadmap.needSharedData(key,
///                                     getFactory()->name(), nullptr);
///  }
/// @endcode
///
/// When loading primitives, the shared data is not actually available until
/// later in the loading process.  The primitive itself needs to hold a
/// reference to the shared data handle so the shared data can be resolved at a
/// later time.
///
/// Resolving the handle is done by asking the handle to resolve the shared
/// data, and then extracting information from a sub-class of a
/// @c GA_SharedLoadData object.  If the shared data is never resolved, it's
/// possible the @c GA_SharedLoadData may never read the data from the file
/// (if the geometry format supports seeking, the loader may be able to use the
/// offsets stored in the @c geometryIndex during writing).  The code to
/// resolve the data might look like: @code
///    if (mySharedHandle)
///    {
///	const GA_SharedLoadData		*item;
///	const CustomSharedLoadData	*data;
///        item = mySharedHandle->resolveSharedData(packed_context);
///        data = dynamic_cast<const CustomSharedLoadData *>(item);
///        if (!data)
///            return false;       // This really shouldn't ever happen
///        copySharedData(data);
///
///        /// Clear the reference to the @c GA_SharedDataHandlePtr.  When all
///        /// references to the shared handle are cleared, the handle will be
///        /// deleted.
///        mySharedHandle.clear();
///    }
/// @endcode
///
/// The last piece to loading, is the actual loading of the shared data block.
/// Since shared data blocks must be able to be loaded without a primitive
/// object, the loading is handled by the @c GU_PackedFactory subclass (the @c
/// loadSharedData()) method.  This method should create the sub-class of the
/// @c GA_SharedLoadData object that's used when resolving shared data.  For
/// example @code
///    class CustomFactory : public GU_PackedFactory
///    {
///        // The type matches the "Shared_Data_Type" described above
///        // The key matches the "Shared_Data_Identifier" described above
///        virtual GA_SharedLoadData   *loadSharedData(UT_JSONParser &p,
///                                        const char *type,
///                                        const char *key,
///                                        bool is_delayed_load) const
///        {
///            CustomSharedLoadData    *data = new CustomSharedLoadData();
///    
///            // This just load the data.  For example the JSON Map saved in
///            // the example above (not the entire array of shared data).
///            if (!data->load(p))
///            {
///                delete data;
///                return nullptr;     // Unable to load the shared data
///            }
///            return data;
///        }
///    };
/// @endcode
class GU_API GU_PackedImpl
{
public:
    /// @{
    /// Typedefs for intrinsic evaluation
    typedef fpreal	(GU_PackedImpl::*FloatGetter)(const GU_PrimPacked *) const;
    typedef void	(GU_PackedImpl::*FloatSetter)(const GU_PrimPacked *, fpreal);
    typedef GA_Size	(GU_PackedImpl::*IntGetter)(const GU_PrimPacked *) const;
    typedef void	(GU_PackedImpl::*IntSetter)(GU_PrimPacked *, GA_Size);
    typedef bool	(GU_PackedImpl::*BoolGetter)(const GU_PrimPacked *) const;
    typedef void	(GU_PackedImpl::*BoolSetter)(GU_PrimPacked *, bool);
    typedef const char	*(GU_PackedImpl::*StringGetter)(const GU_PrimPacked *) const;
    typedef void	(GU_PackedImpl::*StringSetter)(GU_PrimPacked *, const char *);
    typedef std::string	(GU_PackedImpl::*StdStringGetter)(const GU_PrimPacked *) const;
    typedef void	(GU_PackedImpl::*StdStringSetter)(GU_PrimPacked *, const std::string &);
    typedef UT_StringHolder (GU_PackedImpl::*StringHolderGetter)(const GU_PrimPacked *) const;
    typedef void	(GU_PackedImpl::*StringHolderSetter)(GU_PrimPacked *, const UT_StringHolder &);

    typedef fpreal	(GU_PackedImpl::*FloatTupleGetter)(const GU_PrimPacked *, exint) const;
    typedef void	(GU_PackedImpl::*FloatTupleSetter)(GU_PrimPacked *, exint, fpreal);
    typedef GA_Size	(GU_PackedImpl::*IntTupleGetter)(const GU_PrimPacked *, exint) const;
    typedef void	(GU_PackedImpl::*IntTupleSetter)(GU_PrimPacked *, exint,GA_Size);
    typedef bool	(GU_PackedImpl::*BoolTupleGetter)(const GU_PrimPacked *, exint) const;
    typedef void	(GU_PackedImpl::*BoolTupleSetter)(GU_PrimPacked *, exint,bool);
    typedef const char	*(GU_PackedImpl::*StringTupleGetter)(const GU_PrimPacked *, exint) const;
    typedef void	(GU_PackedImpl::*StringTupleSetter)(GU_PrimPacked *, exint,const char *);
    typedef std::string	(GU_PackedImpl::*StdStringTupleGetter)(const GU_PrimPacked *, exint) const;
    typedef void	(GU_PackedImpl::*StdStringTupleSetter)(GU_PrimPacked *, exint,const std::string &);
    typedef UT_StringHolder (GU_PackedImpl::*StringHolderTupleGetter)(const GU_PrimPacked *, exint) const;
    typedef void	(GU_PackedImpl::*StringHolderTupleSetter)(GU_PrimPacked *, exint,const UT_StringHolder &);

    typedef void  (GU_PackedImpl::*F32VectorGetter)(const GU_PrimPacked *, fpreal32*, exint) const;
    typedef void  (GU_PackedImpl::*F32VectorSetter)(GU_PrimPacked *, const fpreal32*, exint);
    typedef void  (GU_PackedImpl::*F64VectorGetter)(const GU_PrimPacked *, fpreal64*, exint) const;
    typedef void  (GU_PackedImpl::*F64VectorSetter)(GU_PrimPacked *, const fpreal64*, exint);
    typedef void  (GU_PackedImpl::*I32VectorGetter)(const GU_PrimPacked *, int32 *, exint) const;
    typedef void  (GU_PackedImpl::*I32VectorSetter)(GU_PrimPacked *, const int32 *, exint);
    typedef void  (GU_PackedImpl::*I64VectorGetter)(const GU_PrimPacked *, int64 *, exint) const;
    typedef void  (GU_PackedImpl::*I64VectorSetter)(GU_PrimPacked *, const int64 *, exint);
    typedef void  (GU_PackedImpl::*BVectorGetter)(const GU_PrimPacked *, bool *, exint) const;
    typedef void  (GU_PackedImpl::*BVectorSetter)(GU_PrimPacked *, const bool *, exint);

    typedef void  (GU_PackedImpl::*StringArrayGetter)(const GU_PrimPacked *, UT_StringArray &) const;
    typedef void  (GU_PackedImpl::*StringArraySetter)(GU_PrimPacked *, const UT_StringArray &);
    /// @}

    GU_PackedImpl();
    GU_PackedImpl(const GU_PackedImpl &src);
    virtual ~GU_PackedImpl();

    /// Get the factory associated with this procedural
    virtual GU_PackedFactory	*getFactory() const = 0;

    /// Create a copy of this resolver
    virtual GU_PackedImpl	*copy() const = 0;

    /// Test whether the deferred load primitive data is valid
    virtual bool	isValid() const = 0;

    /// The clearData() method is called when the primitives are stashed during
    /// the cook of a SOP.  See GA_Primitive::stashed().  This gives the
    /// primitive to optionally clear some data during the stashing process.
    virtual void	clearData() = 0;

    /// Give a UT_Options of load data, create resolver data for the primitive
    virtual bool        load(GU_PrimPacked *prim, const UT_Options &options,
				const GA_LoadMap &map) = 0;

    /// Determines if we support loading from a JSONParser directly
    /// rather than just from a UT_Options.  Loading from JSON is typically
    /// more efficient than loading from a UT_Options since the file loader
    /// doesn't have to convert the JSON map to a UT_Options.
    virtual bool	supportsJSONLoad() const { return false; }

    /// The JSONValueMap represents a UT_Options.  The options may be saved in
    /// compact form, but also may be saved in expanded form.  If in expanded
    /// form, the value for each key's value is a UT_JSONValueMap storing the
    /// value in the "value" key.  It's highly suggested you just use the
    /// protected import() convenience method which deals with this for you.
    virtual bool loadFromJSON(
        GU_PrimPacked *prim,
        const UT_JSONValueMap &options,
        const GA_LoadMap &map)
    { UT_ASSERT(!"JSON direct loading not supported"); return false; }

    /// Depending on the update, the procedural should call one of:
    /// - prim->transformDirty()
    /// - prim->attributeDirty()
    /// - prim->topologyDirty()
    virtual void	update(GU_PrimPacked *prim, const UT_Options &options) = 0;

    /// Save shared data objects to the JSON stream.  This is called directly
    /// from GA_Primitive::saveSharedLoadData().  Please see GU_PackedFactory
    /// for the interface to create the shared data loader.
    virtual bool	saveSharedData(UT_JSONWriter &w,
				GA_SaveMap &map,
				GA_GeometryIndex *geometryIndex) const;

    /// This method is called to resolve the shared data on load.  The @c
    /// load_data_type given is the integer passed when you called
    /// "GA_LoadMap::needSharedData()" -- which was done in load().
    virtual bool	loadSharedData(int load_data_type,
				const GA_SharedLoadData *item);

    /// Copy the resolver data into the UT_Options for saving
    virtual bool	save(UT_Options &options,
				const GA_SaveMap &map) const = 0;

    /// Handle unknown token/value pairs when loading the primitive.  By
    /// default, this adds a warning and skips the next object.  Return false
    /// if there was a critical error.
    virtual bool	loadUnknownToken(const char *token, UT_JSONParser &p,
				const GA_LoadMap &map);

    /// Get the bounding box for the geometry (not including transforms)
    virtual bool	getBounds(UT_BoundingBox &box) const = 0;

    /// Get the rendering bounding box for the geometry (not including
    /// transforms).  For curve and point geometry, this needs to include any
    /// "width" attributes.
    virtual bool	getRenderingBounds(UT_BoundingBox &box) const = 0;

    /// When rendering with velocity blur, the renderer needs to know the
    /// bounds on velocity to accurately compute the bounding box.
    virtual void	getVelocityRange(UT_Vector3 &min,
				UT_Vector3 &max) const = 0;

    /// When rendering points or curves, the renderer needs to know the bounds
    /// on the width attribute to accurately compute the bounding box.
    virtual void	getWidthRange(fpreal &wmin, fpreal &wmax) const = 0;

    /// Return the primitive's "description".  This should be a unique
    /// identifier for the primitive and defaults to:
    ///	  <tt>"%s.%d" % (getFactory()->name(), getPrim()->getNum()) </tt>
    virtual void	getPrimitiveName(const GU_PrimPacked *prim, UT_WorkBuffer &wbuf) const;

    /// @{
    /// Intrinsic functions
    int64		 intrinsic3Tuple(const GU_PrimPacked *prim) const   { return 3; }
    int64		 intrinsic6Tuple(const GU_PrimPacked *prim) const   { return 6; }
    int64		 intrinsic9Tuple(const GU_PrimPacked *prim) const   { return 9; }
    int64		 intrinsic16Tuple(const GU_PrimPacked *prim) const  { return 16; }
    std::string		 intrinsicPrimitiveName(const GU_PrimPacked *prim) const;
    const char		*intrinsicFactoryName(const GU_PrimPacked *prim) const;
    const char		*intrinsicFactoryLabel(const GU_PrimPacked *prim) const;
    void		 intrinsicPackedBox(const GU_PrimPacked *prim, fpreal64 *vals, exint sz) const;
    void		 intrinsicPackedRenderBox(const GU_PrimPacked *prim, fpreal64 *vals, exint sz) const;
    void		 intrinsicPivot(const GU_PrimPacked *prim, fpreal64 *vals, exint sz) const;
    void		 intrinsicSetPivot(GU_PrimPacked *prim, const fpreal64 *vals, exint sz);
    void		 intrinsicPrimTransform(const GU_PrimPacked *prim, fpreal64 *vals, exint sz) const;
    void		 intrinsicSetPrimTransform(GU_PrimPacked *prim, const fpreal64 *v, exint sz);
    void		 intrinsicPackedTransform(const GU_PrimPacked *prim, fpreal64 *v, exint sz) const;
    void		 intrinsicFullTransform(const GU_PrimPacked *prim, fpreal64 *v, exint sz) const;
    const char		*intrinsicViewportLOD(const GU_PrimPacked *prim) const;
    void		 intrinsicSetViewportLOD(GU_PrimPacked *prim, const char *lod);
    /// @}

    /// Some procedurals have an "intrinsic" transform.  These are combined
    /// with the local transform on the geometry primitive.
    ///
    /// The default method returns false and leaves the transform unchanged.
    virtual bool	getLocalTransform(UT_Matrix4D &m) const;

    /// Unpack the procedural into a GU_Detail.  By default, this calls
    /// getGTFull() and converts the GT geometry to a GU_Detail.
    virtual bool        unpackWithContext(
        GU_Detail &destgdp,
        GU_PackedContext &context,
        const GU_PrimPacked *prim) const
    {  return unpack(destgdp, prim); }

    /// Unpacks this into destgdp, transforming the new geometry by
    /// the transform of prim.  If prim is nullptr, no transform will be applied.
    bool                unpack(GU_Detail &destgdp, const GU_PrimPacked *prim) const;

    /// Unpacks this into destgdp, transforming the new geometry by
    /// the given transform.  If transform is nullptr, no transform will be applied.
    virtual bool        unpack(GU_Detail &destgdp, const UT_Matrix4D *transform) const = 0;

    /// Unpack into a GU_Detail, and generate stylers for each primitive.
    /// By default calls unpack, prunes the supplied styler for each generated
    /// primitive, and adds the styles in the material_stylesheet attribute
    /// for each primitive.
    virtual bool        unpackWithStyler(
        GU_Detail &destgdp,
        STY_StylerGroup &prim_styler_group,
        const STY_Styler &parent_styler,
        const GU_PrimPacked *prim) const;

    /// Forces a load of all shared data the primitive has access to
    /// this should leave no data on disk
    virtual void	forceLoad() const {}

    /// Checks if this packed implementation has loaded its shared data
    /// Default implementation returns true.
    virtual bool	isLoaded() const { return true; }

    /// Unpack the procedural into a GU_Detail using polygons instead of
    /// polygon soups.  The default method just unpacks, then converts polygon
    /// soups to polygons.
    virtual bool        unpackUsingPolygons(
        GU_Detail &destgdp,
        const GU_PrimPacked *prim) const;

    /// Get a reference to a const GU_Detail for the packed geometry.  Not all
    /// implementations need to provide this.  If this isn't implemented, the
    /// method should return an empty detail handle.  This is meant as a quick
    /// short-cut instead of having to call unpack() which performs a full
    /// merge.
    virtual GU_ConstDetailHandle	getPackedDetail(GU_PackedContext *context = 0) const;

    /// If @c pointInstanceTransform() returns false, @c getFullTransform4()
    /// method will only use the point position as a translate.  If true, @c
    /// getFullTransform4() will perform the standard "houdini instancing"
    /// (i.e. use the "N", "v", "rot" and other attributes to compute an
    /// orientation and translation).
    ///
    /// The default method returns false, so only "P" is used to apply
    /// translates.
    bool		pointInstanceTransform() const
			    { return myPointInstanceTransform; }
    bool                intrinsicPointInstanceTransform(const GU_PrimPacked *prim) const
    { return pointInstanceTransform(); }

    /// Set whether point instancing should be used.
    void		setPointInstanceTransform(GU_PrimPacked *prim, bool b);

    /// Indicate that topology and attributes are dirty
    void		topologyDirty();

    SYS_DEPRECATED_HDK_REPLACE(17.0, "passing the primitive separately, since this pointer will be removed soon")
    GU_PrimPacked	*getPrim()		{ return myOwner; }
    SYS_DEPRECATED_HDK_REPLACE(17.0, "passing the primitive separately, since this pointer will be removed soon")
    const GU_PrimPacked	*getPrim() const	{ return myOwner; }

    /// Convenience method to compute the velocity bounds for a given geometry.
    /// The geometry may be a NULL pointer.
    static bool	computeVelocityRange(const GU_Detail *gdp,
			    UT_Vector3 &vmin, UT_Vector3 &vmax);
    static bool	computeWidthRange(const GU_Detail *gdp,
			    fpreal &wmin, fpreal &wmax);

    /// Report memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const = 0;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: There's nothing outside of sizeof(*this) to count in the
    ///       base class, so it can be pure virtual.
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const = 0;

    /// Determines if we should save the bounding box to the metadata, this
    /// makes sense for fully embedded primitives, but if the primitive
    /// can be swapped out without the owner knowing, it should be
    /// avoided.
    virtual bool saveCachedBBox() const { return false; }

protected:
    /// For all the groups that this primitive belongs to, create groups on the
    /// destination detail and place all primitives in the given range into the
    /// groups.  If the group already exists on the destination detail, then
    /// nothing is done (unless @c force is set).
    void	copyPrimitiveGroups(GU_Detail &dest, bool force=false) const;

    /// Unpack the source detail into the destination detail.  The source
    /// detail *may* be modified (attributes may be promoted)
    ///
    /// The @c src detail is not deleted by unpackToDetail.
    bool	unpackToDetail(GU_Detail &destgdp,
				GU_Detail *src,
                                const UT_Matrix4D *transform) const;
    /// Unpack the source detail into the destination detail.  The source
    /// detail is const and will not be modified.  However, this may be more
    /// expensive than the non-const version.
    bool	unpackToDetail(GU_Detail &destgdp,
				const GU_Detail *src,
                                const UT_Matrix4D *transform) const;
    /// Unpack the source detail handle
    bool	unpackToDetail(GU_Detail &destgdp,
				const GU_ConstDetailHandle &gdh,
                                const UT_Matrix4D *transform) const
    {
	GU_DetailHandleAutoReadLock rlock(gdh);
	return unpackToDetail(destgdp, rlock.getGdp(), transform);
    }
    /// Optional method to compute centroid (default uses bounding box)
    virtual UT_Vector3	getBaryCenter() const;
    /// Optional method to calculate volume (default uses bounding box)
    virtual fpreal	computeVolume(const UT_Vector3 &refpt) const;
    /// Optional method to calculate surface area (default uses bounding box)
    virtual fpreal	computeArea() const;
    /// Optional method to calculate perimeter (default uses bounding box)
    virtual fpreal	computePerimeter() const;

    /// @{
    /// Import a value from a UT_JSONValueMap or a UT_Options.  This handles
    /// both the compact and expanded options and is valid for the same types
    /// as UT_JSONValueMap::import()
    ///
    /// These import() methods can be used in the @c load() and @c
    /// loadFromJSON() methods to simplify importing code.  For example, you
    /// should be able to implement a generic @c loadFrom() method, templated
    /// on either @c UT_Options or @c UT_JSONValueMap.  You can then just @code
    ///    virtual bool load(const UT_Options &options, const GA_LoadMap &map)
    ///        { return loadFrom<UT_Options>(options, map); }
    ///    virtual bool loadFromJSONValueMap(const UT_JSONValueMap &options,
    ///               const GA_LoadMap &map)
    ///        { return loadFrom<UT_Options>(options, map); }
    /// @endcode
    template <typename T>
    static bool	import(const UT_JSONValueMap &options,
			    const UT_StringRef &key,
			    T &value);
    template <typename T>
    static bool	import(const UT_Options &options,
			    const UT_StringRef &key,
			    T &value);
    /// @}

private:
    GU_PrimPacked	*myOwner;
    bool		 myPointInstanceTransform;

    friend class	 GU_PrimPacked;
};

#endif
