/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveDefinition.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_PrimitiveDefinition__
#define __GA_PrimitiveDefinition__

#include "GA_API.h"
#include "GA_IntrinsicManager.h"
#include "GA_PrimitiveFamilyMask.h"
#include "GA_PrimitiveTypeId.h"
#include "GA_Types.h"

#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Inline.h>

class UT_Options;
class GA_Detail;
class GA_MergeMap;
class GA_Primitive;
class GA_PrimitiveDefinition;
class GA_PrimitiveFactory;
class GA_PrimitiveList;
class GA_LoadMap;
class GA_SharedLoadData;
class GA_SharedLoadDataStat;

typedef void (*GA_PrimitiveBlockConstructor)(
    GA_Primitive **new_prims,
    GA_Size nprimitives,
    GA_Detail &detail,
    GA_Offset start_offset,
    const GA_PrimitiveDefinition &def,
    bool allowed_to_parallelize);

/// @brief Definition of a geometric primitive
///
/// Every primitive type is registered with a GA_PrimitiveFactory which creates
/// a GA_PrimitiveDefinition.  Each GA_PrimitiveDefinition in a given
/// GA_PrimitiveFactory is assigned a unique identifier.  In addition, an
/// optional family mask can be registered with the type to specify from which
/// standard primitive types (managed by that factory) the new GA_Primitive
/// subclass derives.
///
/// The GA_PrimitiveDefinition for the new type registers a constructor
/// function that will be used to allocate new instances of that type.
///
/// Finally, the definition also contains a list of "intrinsics" that can
/// be queried from primitives of that type.
class GA_API GA_PrimitiveDefinition 
{
public:
    /// The name of the primitive
    const UT_StringHolder	&getToken() const	{ return myToken; }
    /// The label (user friendly & readable token) of the primitive
    const UT_StringHolder	&getLabel() const	{ return myLabel; }
    /// The icon associated with the primitive type (if any).
    const UT_StringHolder	&getIcon() const 	{ return myIcon; }

    /// The unique ID assigned by the GA_PrimitiveFactory
    SYS_FORCE_INLINE
    const GA_PrimitiveTypeId &getId() const	{ return myId; }
    /// The optional family mask
    GA_PrimitiveFamilyMask	getFamilyMask() const	{ return myFamilyMask; }

    /// Primitive intrinsic attributes defined for this primitive
    const GA_IntrinsicManager	&getIntrinsicManager() const
				    { return myIntrinsicManager; }
    GA_IntrinsicManager		&getIntrinsicManager()
				    { return myIntrinsicManager; }

    /// Set the label for the primitive
    void			setLabel(const UT_StringHolder &label)
				    { myLabel = label; }

    /// Set the icon for the primitive
    void			setIcon(const UT_StringHolder &icon)
				    { myIcon = icon; }

    /// NOTE: This used to set the merge constructor, but merge constructors are
    /// no longer a thing.  Please make sure that the regular constructor of
    /// your primitive types don't add any vertices to the detail or anything
    /// else thread-unsafe like that.  You can always add vertices and
    /// initialize them later.  This change was required in order for all
    /// primitive types to be able to be constructed in parallel.
    SYS_DEPRECATED_REPLACE(16.0, a threadsafe primitive constructor that doesnt add any vertices to the detail)
    void	setMergeConstructor(GA_Primitive *(*ctor)(const GA_MergeMap &map, GA_Detail &dest_detail, GA_Offset dest_offset, const GA_Primitive &src_prim))
    { UT_ASSERT_MSG(0, "This should never be called anymore!  All regular constructors must be threadsafe.  This function is only here for the deprecation warning message."); }

    /// Return whether the primitive has a transform associated with it
    bool	hasLocalTransform() const	{ return myHasLocalTransform; }
    /// Set whether the primitive is has a transform associated with it
    void	setHasLocalTransform(bool x)	{ myHasLocalTransform = x; }

    /// Return whether the primitive is able to be cached on the GPU.
    bool	hasCECaches() const		{ return myHasCECaches; }
    void	setHasCECaches(bool x)		{ myHasCECaches = x; }

    /// Returns true if this type is for use by GEO_Detail and false
    /// if this type is for use by GD_Detail.
    bool        isForPrimaryDetail() const { return myForPrimaryDetail; }

    /// Class to load shared data.
    class SharedDataLoader
    {
    public:
	SharedDataLoader() {}
	virtual ~SharedDataLoader() {}
	/// Loads shared data from the parser given returning the shared load data object
	///
	/// The loader also takes one of two things:
	///
	/// 1. A random primitive (for now the first that requests shared) is given to the loader
	/// because voxel array shared data loader needs one because of incorrect splitting of
	/// data across the primitive and the shared voxels.
	///
	/// 2. The load map from IOJSON. This is used when we are loading without delay. To get a primitive
	/// the loader must ask the load map for one using its key.
	///
	/// It takes these things when it really should not because volume shared data was designed
	/// poorly and some of the data that should be shared is kept with the primitive and is
	/// needed to load the shared data. (Examples: Size, Border of voxel array, there may be more)
	///
	/// IF YOU ARE WRITING A NEW SHARED DATA LOADER DO NOT USE THE SECOND TWO PARAMETERS!!
	virtual GA_SharedLoadData* load(UT_JSONParser &p, GA_LoadMap* loadMap, GA_Primitive* randomPrimitive) const = 0;

	/// Stats the shared data from the parser given returning a shared load data stat object
	/// The default implementation for stat is to return a NULL pointer because most
	/// shared data does not have stat. (mostly used for packed geometry)
	/// This should only be called when delayed loading
	virtual GA_SharedLoadDataStat* stat(UT_JSONParser& parser) const { return NULL; }
    };
    /// Return whether the primitive type has a shared data loader.
    bool	hasSharedLoadData() const
			{ return mySharedDataLoader != 0; }
    /// Associate the primitive type with a shared data loader.
    void	setSharedDataLoader(const SharedDataLoader *l)
			{ mySharedDataLoader = l; }
    const SharedDataLoader	*sharedDataLoader() const
					{ return mySharedDataLoader; }

private:
    /// Get the primitive block constructor.
    /// NOTE: This should only be called via GA_PrimitiveList.
    GA_PrimitiveBlockConstructor getConstructor() const
			{ return myConstructor; }

    /// Constructor
    GA_PrimitiveDefinition(
        GA_PrimitiveFactory &factory,
        const UT_StringHolder &name,
        const GA_PrimitiveTypeId &id,
        GA_PrimitiveFamilyMask family_mask,
        GA_PrimitiveBlockConstructor construct,
        bool isforprimarydetail);

    /// Destructor
    ~GA_PrimitiveDefinition() {}

    /// Back pointer to the factory which owns this
    GA_PrimitiveFactory		&myFactory;
    
    /// Intrinsics defined for this primitive type
    GA_IntrinsicManager		myIntrinsicManager;

    /// Name of the primitive
    UT_StringHolder		 myToken;

    /// Label for the primitive (a nice name)
    UT_StringHolder		 myLabel;

    /// Icon for the primitive (to show in the data tree and elsewhere)
    UT_StringHolder		 myIcon;

    /// Unique ID (assigned at run-time)
    GA_PrimitiveTypeId		 myId;
    /// Optional family mask
    GA_PrimitiveFamilyMask	 myFamilyMask;

    /// This creates a block of primitives, so that only one call
    /// needs to be done via a function pointer.
    GA_PrimitiveBlockConstructor myConstructor;

    const SharedDataLoader      *mySharedDataLoader;

    /// Whether the primitive has a transform (like a quadric)
    bool		myHasLocalTransform;

    /// Whether the primitive can be cached on the GPU
    bool		myHasCECaches;

    /// true iff this primitive type is for use in GEO_Detail,
    /// and not GD_Detail.
    const bool myForPrimaryDetail;

    friend class GA_PrimitiveFactory;
    friend class GA_PrimitiveList;
};

#endif
