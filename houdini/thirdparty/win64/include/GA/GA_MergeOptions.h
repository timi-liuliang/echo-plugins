/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_MergeOptions.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_MergeOptions__
#define __GA_MergeOptions__

#include "GA_API.h"
#include "GA_Range.h"
#include "GA_Types.h"
#include "GA_ReuseStrategy.h"

#include <SYS/SYS_Math.h>


class GA_Detail;
class GA_GBPointRedirectArray;
class GA_Primitive;

template <typename T> class UT_Array;


/// @brief Provide options when performing a merge operation.
///
/// This class stores generic options which can be set/accessed when merging.
///
/// Some common options are implemented using the @c IMPL_ENUM define.
///
/// - TYPE = the name of the enum
/// - DEF  = the default enum (used if no option is found)
/// - GENERAL = the name of the general option (i.e. "storagestrategy")
/// - VERTEX = speicific name for vertex attribs (i.e. "vertex:storagestrategy)
/// - POINT = specific name for point (i.e. "point:storagestrategy)
/// - PRIMITIVE = specifid name for primitive (i.e. "primitive:storagestrategy)
/// - GLOBAL = specific name for global (i.e. "global:storagestrategy)
///
/// The methods provided are:
/// @code
/// TypeStrategy	getTypeStrategy()
/// TypeStrategy	getTypeStrategy(GA_AttributeOwner owner)
/// void setTypeStrategy(TypeStrategy value)
/// void setTypeStrategy(TypeStrategy value, GA_AttributeOwner owner)
///
/// StorageStrategy	getStorageStrategy()
/// StorageStrategy	getStorageStrategy(GA_AttributeOwner owner)
/// void setStorageStrategy(StorageStrategy value)
/// void setStorageStrategy(StorageStrategy value, GA_AttributeOwner owner)
///
/// TupleSizeStrategy	getTupleSizeStrategy()
/// TupleSizeStrategy	getTupleSizeStrategy(GA_AttributeOwner owner)
/// void setTupleSizeStrategy(TupleSizeStrategy value)
/// void setTupleSizeStrategy(TupleSizeStrategy value, GA_AttributeOwner owner)
///
/// MergeStrategy	getMergeStrategy()
/// MergeStrategy	getMergeStrategy(GA_AttributeOwner owner)
/// void setMergeStrategy(MergeStrategy value)
/// void setMergeStrategy(MergeStrategy value, GA_AttributeOwner owner)
/// @endcode
///
/// List of all well defined options:
///  - [point|vertex|primitive|global:]typestrategy @n
///	What to do when the attribute type on the source & destination of the
///	merge don't match.
///  - [point|vertex|primitive|global:]storagestrategy @n
///	What to do when the attribute storage on the source & destination of the
///	merge don't match.
///  - [point|vertex|primitive|global:]tuplesizestrategy @n
///	What to do when the attribute tuple size on the source & destination of
///	the merge don't match.
///  - [point|vertex|primitive|global:]mergestrategy @n
///	What approach should be taken when laying out data in the index maps
///	when merging data.  Optimize for either space or speed.
///  - point|primitive:insert_at_head @n
///	Insert primitives/points at head of ordered list rather than appending
///  - point|primitive|vertex|edge|breakpoint:mergegroups
///	Whether to merge groups from source detail.  Defaults to true.
///  - point|primitive|vertex|edge|breakpoint:mergeinternalgroups
///	Whether to merge internal groups from source detail.  Defaults to true.
///  - point:mergeallpoints @n
///	When specifying a primitive group, by default only the points
///	referenced by the primitives will be merged.  Setting this option will
///	cause @b all points to be merged.
///  - merge:threaded
///	Perform threaded merging of data
///
class GA_API GA_MergeOptions
{
public:
    GA_MergeOptions();

    /// The @c TypeStrategy is used to give a hint to the AIFMerge if it
    /// encounters a type difference on the attributes.  For example,
    /// if the destination is a string attribute, but the source has a boolean
    /// attribute, the attribute can either keep or change the destination to
    /// match the source.
    enum TypeStrategy
    {
	/// Only process exact matches on types
	TYPE_DISALLOW,
	/// Keep destination unchanged if possible
	TYPE_KEEP_DEST,
	/// Change destination attribute to match source
	TYPE_CHANGE_DEST,

	TYPE_DEFAULT = TYPE_KEEP_DEST
    };
    /// For attributes which have storage, this enum gives a hint of what to do
    /// if there's a mis-match on storage types.
    enum StorageStrategy
    {
	/// Disallow mis-matched storage
	STORAGE_DISALLOW,
	/// Keep destination storage (casting source)
	STORAGE_KEEP_DEST,
	/// Change destination to match storage
	STORAGE_CHANGE_DEST,
	/// Use storage with greatest precision
	STORAGE_USE_BEST,

	STORAGE_DEFAULT = STORAGE_USE_BEST
    };

    /// For attributes which have a tuple size, this enum gives a hint of what
    /// to do if there's a mis-match on storage types.
    enum TupleSizeStrategy
    {
	/// Disallow mis-matched tuple sizes
	TUPLE_SIZE_DISALLOW,
	/// Use the destination tuple size
	TUPLE_SIZE_KEEP_DEST,
	/// Change destination to match source
	TUPLE_SIZE_CHANGE_DEST,
	/// Use the largest tuple size
	TUPLE_SIZE_USE_BEST,

	TUPLE_SIZE_DEFAULT = TUPLE_SIZE_USE_BEST
    };

    /// The @c MergeStrategy is used to give a hint as to how the merge should
    /// append data after the existing data or whether the source data should
    /// be interleaved with the existing data.
    /// The merge operation may change this "hint" for the actual operation.
    enum MergeStrategy
    {
	/// The optimal mode will use simple heuristics to determine whether
	/// elements should be appended or interleaved.
	MERGE_OPTIMAL,
	/// MERGE_COPY is very similar to MERGE_APPEND.  However, this is done
	/// when the original destination list has no elements allocated before
	/// the merge (i.e. the merge acts as a copy).
	MERGE_COPY,
	/// All data is appended to the end of the existing destination data.
	/// This offers attributes the opportunity to share data.
	MERGE_APPEND,
	/// In this mode, the source data is merged and mixed in with the
	/// destination data.  This will result in the most compact storage,
	/// but may result in non-shared data after the merge.
	MERGE_INTERLEAVE,

	MERGE_DEFAULT = MERGE_OPTIMAL
    };

#define IMPL_ENUM(TYPE, DEF) \
    TYPE	get##TYPE() const { \
		    if (my##TYPE[GA_ATTRIB_OWNER_N] >= 0) \
			return my##TYPE[GA_ATTRIB_OWNER_N]; \
		    return DEF; \
		} \
    TYPE	get##TYPE(GA_AttributeOwner owner) const { \
		    if (my##TYPE[owner] >= 0) return my##TYPE[owner]; \
		    return get##TYPE(); \
		} \
    void	set##TYPE(TYPE v) { my##TYPE[GA_ATTRIB_OWNER_N] = v; } \
    void	set##TYPE(TYPE v, GA_AttributeOwner owner) { \
		    my##TYPE[owner] = v; \
		}

    IMPL_ENUM(TypeStrategy, TYPE_DEFAULT)
    IMPL_ENUM(StorageStrategy, STORAGE_DEFAULT)
    IMPL_ENUM(TupleSizeStrategy, TUPLE_SIZE_DEFAULT)
    IMPL_ENUM(MergeStrategy, MERGE_DEFAULT)

#undef IMPL_ENUM

    const GA_Range	*getSourceRange(GA_AttributeOwner owner)
			 {
			     switch (owner)
			     {
				case GA_ATTRIB_POINT:
				    return getSourcePointRange();
				case GA_ATTRIB_PRIMITIVE:
				    return getSourcePrimitiveRange();
				default:
				    break;
			     }
			     return 0;
			 }

    /// @{
    /// Set whether the merge should copy over group membership for the given
    /// attribute type.  By default, all groups are copied.
    ///
    /// Note that only a single type should be passed in (don't use bit-masks)
    void	setMergeGroups(GA_GroupType type, bool state)
		    { myGroupMerge[type] = state; }
    void	setAllMergeGroups(bool state)
		    {
			for (int i = 0; i < GA_GROUP_N; ++i)
			{
			    myGroupMerge[i] = state;
			}
		    }
    bool	getMergeGroups(GA_GroupType type) const
		    { return myGroupMerge[type]; }
    void	setMergeInternalGroups(GA_GroupType type, bool state)
		    { myInternalGroupMerge[type] = state; }
    void	setAllMergeInternalGroups(bool state)
		    {
			for (int i = 0; i < GA_GROUP_N; ++i)
			{
			    myInternalGroupMerge[i] = state;
			}
		    }
    bool	getMergeInternalGroups(GA_GroupType type) const
		    { return myInternalGroupMerge[type]; }
    /// @}

    /// @{
    /// There are two iterators that can be specified in the options.  This
    /// allows the merge operation to operate on a subset of primitives &
    /// points.
    /// - If neither range is set, all source primitives, vertices and points
    ///   will be merged.
    /// - If the primitive range is set, only the primitives in the range will
    ///   be merged.  Only the points used by the primitives will be merged
    ///   unless the bool option: "point:mergeallpoints" is set.
    /// - If the point range is set, no primitives will be merge, and only the
    ///   points in the range will be merged.
    const GA_Range	*getSourcePointRange() const
			    { return myPointRange.getRTI()
				? &myPointRange : (const GA_Range *)NULL; }
    const GA_Range	*getSourcePrimitiveRange() const
			    { return myPrimitiveRange.getRTI()
				? &myPrimitiveRange : (const GA_Range *)NULL; }

    void		 setSourcePrimitiveRange(const GA_Range &range)
			    { myPrimitiveRange = range; }
    void		 setSourcePointRange(const GA_Range &range)
			    { myPointRange = range; }
    /// @}

    /// @{
    /// For backward compatibility with the GEO library, we provide a method to
    /// control the "mapping" from source primitives to the newly
    /// created merged primitives.
    void		  setPrimitiveMap(UT_Array<GA_Primitive*> *prims);
    UT_Array<GA_Primitive*> *getPrimitiveMap() const 
			    { return myPrimitiveMap; }
    /// @}

    /// @{
    /// For backward compatibility with the GEO library, we provide a method to
    /// control the "mapping" from source points to the newly created merged
    /// points.
    void			 setPointRedirect(GA_GBPointRedirectArray *map);
    GA_GBPointRedirectArray	*getPointRedirect() const
					{ return myPointRedirect; }
    /// @}

    /// @{
    /// Whether to perform threaded merging of attributes
    /// Default: true
    bool	mergeThreaded() const { return myMergeThreaded; }
    void	setMergeThreaded(bool v) { myMergeThreaded = v; }
    /// @}

    /// @{
    /// When merging a range of primitives, check whether to merge @b all
    /// points (or just the referenced points).
    /// Default: false
    bool	mergeAllPoints() const { return myMergeAllPoints; }
    void	setMergeAllPoints(bool v) { myMergeAllPoints = v; }
    /// @}

    /// @{
    /// Whether to insert points at the head when merging.
    /// Default: false
    bool	mergePointsAtHead() const { return myMergePointsAtHead; }
    void	setMergePointsAtHead(bool v) { myMergePointsAtHead = v; }
    /// @}

    /// @{
    /// Whether to insert primitives at the head when merging.
    /// Default: false
    bool	mergePrimitivesAtHead() const { return myMergePrimitivesAtHead;}
    void	setMergePrimitivesAtHead(bool v) { myMergePrimitivesAtHead=v; }
    /// @}

    /// @{
    /// Merge the input detail multiple times.
    GA_Size	mergeCount() const		{ return myMergeCount; }
    void	setMergeCount(GA_Size nmerges)
		{
		    myMergeCount = SYSmax(1, nmerges);
		    if (myMergeCount > 1)
			myForceInterleaved = true;
		}
    /// @}

    /// @{
    /// Whether the "copy" style of merging should force interleaving instead
    /// of verbatim copying.  This is often useful when merging multiple
    /// different details.
    bool	forceInterleaved() const	{ return myForceInterleaved; }
    void	setForceInterleaved(bool v)	{ myForceInterleaved = v; }
    /// @}
 
    /// @{
    /// Strategy to use for attribute data ids during the merge, defaulting
    /// to GA_DATA_ID_BUMP.  GA_DATA_ID_CLONE is useful when copying a source
    /// detail in order to perform a partial update without always blindly
    /// invalidating any external caches tied to data ids.  The data id must
    /// be manually bumped when an attribute is later modified.
    GA_DataIdStrategy	 getAttributeDataIdStrategy() const
					{ return myAttributeDataIdStrategy; }
    void		 setAttributeDataIdStrategy(GA_DataIdStrategy s)
					{ myAttributeDataIdStrategy = s; }
    /// @}

    /// @{
    /// Strategy to use for handling point attributes and vertex attributes
    /// of the same name, defaulting to override a previous point/vertex
    /// attribute with the requested vertex/point attribute.
    const GA_ReuseStrategy &getReuseStrategy() const
    { return myReuseStrategy; }
    void setReuseStrategy(const GA_ReuseStrategy &reuse)
    { myReuseStrategy = reuse; }
    /// }

private:
    TypeStrategy	 myTypeStrategy[GA_ATTRIB_OWNER_N + 1];
    StorageStrategy	 myStorageStrategy[GA_ATTRIB_OWNER_N + 1];
    TupleSizeStrategy	 myTupleSizeStrategy[GA_ATTRIB_OWNER_N + 1];
    MergeStrategy	 myMergeStrategy[GA_ATTRIB_OWNER_N + 1];

    UT_Array<GA_Primitive*>	 *myPrimitiveMap;
    GA_Range			  myPrimitiveRange;
    GA_GBPointRedirectArray	 *myPointRedirect;
    GA_Range			  myPointRange;
    GA_Size			  myMergeCount;
    GA_DataIdStrategy		  myAttributeDataIdStrategy;
    GA_ReuseStrategy              myReuseStrategy;
    bool			  myGroupMerge[GA_GROUP_N];
    bool			  myInternalGroupMerge[GA_GROUP_N];
    bool			  myForceInterleaved;
    bool			  myMergeThreaded;
    bool			  myMergeAllPoints;
    bool			  myMergePointsAtHead;
    bool			  myMergePrimitivesAtHead;
};

#endif
