/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ReuseStrategy.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_ReuseStrategy__
#define __GA_ReuseStrategy__

#include "GA_API.h"
#include "GA_Types.h"

class GA_Attribute;
class GA_Defaults;
class GA_AttributeOptions;

class UT_Options;
class UT_StringRef;


/// @brief Specify when and how to reuse an existing attribute.
///
/// When attempting to create a new attribute, we may encounter an existing
/// attribute with the same name.  The @c GA_ReuseStrategy settings specify
/// when and how to reuse the existing attribute instead of creating the new
/// one.
///
class GA_API GA_ReuseStrategy
{
public:
    /// The high level strategy for resolving a name collision with an
    /// existing attribute.
    enum CollisionStrategy
    {
	/// Fail immediately on the collision, and resolve using the set
	/// FailureStrategy.
	COLLISION_FAIL,

	/// Attempt to recycle the existing attribute, failing only if the
	/// attempt fails, at which point, resolve using the FailureStrategy.
	/// The existing attribute may be partially changed on failure, so
	/// be careful when combining with FAILURE_RETURN_MISMATCH or even
	/// FAILURE_RETURN_INVALID.
	COLLISION_RECYCLE,

	COLLISION_DEFAULT = COLLISION_RECYCLE
    };

    /// This enum specifies how to deal with a failure when applying the
    /// CollisionStrategy.
    enum FailureStrategy
    {
	/// Return the conflicting attribute
	FAILURE_RETURN_MISMATCH,
	/// Return an invalid result (NULL pointer or invalid reference)
	FAILURE_RETURN_INVALID,
	/// Replace the conflicting attribute
	FAILURE_REPLACE,
	/// Replace the conflicting attribute and attempt to copy data
	/// values to the new attribute.
	FAILURE_REPLACE_AND_COPY,

	FAILURE_DEFAULT = FAILURE_REPLACE_AND_COPY
    };

    /// Typically a type mismatch will result in the existing attribute being
    /// replaced when an attribute type is explicitly requested, but when the
    /// only criteria is that the existing attribute support some expected
    /// interface, we can be less restrictive.
    enum TypeStrategy
    {
	/// Disallow mis-matched type
	TYPE_DISALLOW,
	/// Allow mis-matched type if compatible (only possible in generic
	/// methods like addFloatTuple(), addStringTuple(), etc where type
	/// is only implied).
	TYPE_KEEP_COMPATIBLE,

	TYPE_DEFAULT = TYPE_KEEP_COMPATIBLE
    };

    /// For attributes which have storage, this enum specifies what to do if
    /// there is a mismatch.
    enum StorageStrategy
    {
	/// Disallow mis-matched storage
	STORAGE_DISALLOW,
	/// Keep existing storage (if compatible)
	STORAGE_KEEP,
	/// Reduce existing storage to that desired
	STORAGE_CLAMP,
	/// Use storage with greatest precision (float > int)
	STORAGE_USE_BEST,

	STORAGE_DEFAULT = STORAGE_USE_BEST
    };

    /// For attributes which have a tuple size, this enum specifies what to do
    /// if there is a mis-match.
    enum TupleSizeStrategy
    {
	/// Disallow mis-matched tuple sizes
	TUPLE_SIZE_DISALLOW,
	/// Set the new tuple size to be no larger than the old size.
	TUPLE_SIZE_CLAMP,
	/// Use the larger of the two tuple sizes
	TUPLE_SIZE_USE_BEST,

	TUPLE_SIZE_DEFAULT = TUPLE_SIZE_USE_BEST
    };

    /// This enum specifies what to do with the attribute options passed to the
    /// creation method in the event that we're going to reuse an attribute.
    enum OptionsStrategy
    {
	/// Keep existing attribute's options
	OPTIONS_KEEP,
	/// Replace existing attribute's options entirely
	OPTIONS_REPLACE,
	/// Overwrite existing attribute's options so that options unique to
	/// the existing attribute are not lost
	OPTIONS_OVERWRITE,
	/// Keep any existing options, but merge in any missing options
	OPTIONS_MERGE_MISSING,

	OPTIONS_DEFAULT = OPTIONS_MERGE_MISSING
    };

    /// This enum specifies whether or not we attempt to copy values from the
    /// existing attribute in the event that we fail to reuse it.
    enum DataCopyStrategy
    {
	DATACOPY_FALSE,
	DATACOPY_TRUE,

	DATACOPY_DEFAULT = DATACOPY_TRUE
    };

    /// This enum specifies how we deal with name collisions across the point
    /// and vertex dictionaries.
    enum PointVertexCollisionStrategy
    {
	/// Ignore the attribute in the other dictionary.
	PT_VTX_COLLISION_IGNORE,
	/// Destroy the attribute in the other dictionary.
	PT_VTX_COLLISION_DESTROY,
	/// Inherit the values from the attribute in the other dictionary, and
	/// then destroy it.
	PT_VTX_COLLISION_INHERIT_VALUES_AND_DESTROY,

	PT_VTX_COLLISION_DEFAULT = PT_VTX_COLLISION_INHERIT_VALUES_AND_DESTROY
    };


     GA_ReuseStrategy(FailureStrategy fail_strategy = FAILURE_DEFAULT,
		      TypeStrategy type_strategy = TYPE_DEFAULT,
		      StorageStrategy storage_strategy = STORAGE_DEFAULT,
		      TupleSizeStrategy size_strategy = TUPLE_SIZE_DEFAULT,
		      OptionsStrategy opts_strategy = OPTIONS_DEFAULT)
	 : myCollisionStrategy(COLLISION_RECYCLE),
	   myFailureStrategy(fail_strategy),
	   myTypeStrategy(type_strategy), myStorageStrategy(storage_strategy),
	   myTupleSizeStrategy(size_strategy), myOptionsStrategy(opts_strategy),
	   myPointVertexCollisionStrategy(PT_VTX_COLLISION_DEFAULT)
	{ }
    ~GA_ReuseStrategy() {}

    CollisionStrategy	 getCollisionStrategy() const
						{ return myCollisionStrategy; }
    void		 setCollisionStrategy(CollisionStrategy s)
						{ myCollisionStrategy = s; }

    FailureStrategy	 getFailureStrategy() const
						{ return myFailureStrategy; }
    void		 setFailureStrategy(FailureStrategy s)
						{ myFailureStrategy = s; }

    TypeStrategy	 getTypeStrategy() const
						{ return myTypeStrategy; }
    void		 setTypeStrategy(TypeStrategy s)
						{ myTypeStrategy = s; }

    StorageStrategy	 getStorageStrategy() const
						{ return myStorageStrategy; }
    void		 setStorageStrategy(StorageStrategy s)
						{ myStorageStrategy = s; }

    TupleSizeStrategy	 getTupleSizeStrategy() const
						{ return myTupleSizeStrategy; }
    void		 setTupleSizeStrategy(TupleSizeStrategy s)
						{ myTupleSizeStrategy = s; }

    OptionsStrategy	 getOptionsStrategy() const
						{ return myOptionsStrategy; }
    void		 setOptionsStrategy(OptionsStrategy s)
						{ myOptionsStrategy = s; }


    bool		 getRecycleOnCollision() const
			 { return myCollisionStrategy == COLLISION_RECYCLE; }
    bool		 getReplaceOnFailure() const
			 {
			     return myFailureStrategy == FAILURE_REPLACE ||
				  myFailureStrategy == FAILURE_REPLACE_AND_COPY;
			 }

    PointVertexCollisionStrategy getPointVertexCollisionStrategy() const
			 { return myPointVertexCollisionStrategy; }
    void		 setPointVertexCollisionStrategy(
					PointVertexCollisionStrategy s)
			 {
			     myPointVertexCollisionStrategy = s;
			 }


    /// Attempt to bring the attribute to compliance as per the strategy.  Our
    /// TypeStrategy determines whether we care about an exact type match or
    /// can work with a tuple AIF.
    ///
    /// Returns true on success and false on failure.  It is expected that the
    /// attribute will be replaced on failure as it may be partially modified
    /// by this method.
    bool		 applyStrategiesForTuples(
			    GA_Attribute &attrib, GA_Storage requested_storage,
			    int requested_tuple_size,
			    const GA_Defaults *requested_defaults,
			    const UT_Options *requested_creation_options,
			    const GA_AttributeOptions *requested_attribute_options,
			    const UT_StringRef &requested_type) const;

    /// Attempt to bring the attribute to compliance as per the strategy.  Our
    /// TypeStrategy determines whether we care about an exact type match
    ///
    /// Returns true on success and false on failure.  It is expected that the
    /// attribute will be replaced on failure as it may be partially modified
    /// by this method.
    bool		 applyStrategiesForArrays(
			    GA_Attribute &attrib, GA_Storage requested_storage,
			    int requested_tuple_size,
			    const UT_Options *requested_creation_options,
			    const GA_AttributeOptions *requested_attribute_options,
			    const UT_StringRef &requested_type) const;

    /// Only call this method if getReplaceOnFailure() returns true.  Returns
    /// true if the copy was successful, or was not needed.  False otherwise.
    bool		 applyDataCopyStrategy(
					GA_Attribute *dest,
					const GA_Attribute *src) const;

    /// @{
    /// Helper methods for COLLISION_RECYCLE
    ///
    /// Applies getTupleSizeStrategy() to return the new tuple size for the
    /// attribute, or -1 if no resolution is possible.
    int			 computeTupleSize(int old_size,
					  int requested_size) const;

    /// Applies getStorageStrategy() to return the new numeric storage for the
    /// attribute or GA_STORE_INVALID if no resolution is possible.  Note that
    /// GA_STORE_BOOL is considered a numeric storage type for these purposes.
    GA_Storage		 computeNumericStorage(GA_Storage old_storage,
					       GA_Storage requested_storage)
									const;

    /// Compare two defaults objects for equality.
    static bool		 areEqual(const GA_Defaults &a_defaults,
				  const GA_Defaults &b_defaults,
				  GA_Storage storage, int size);

    /// Apply getOptionsStrategy().
    void		 applyOptionsStrategy(
					const GA_AttributeOptions *requested_options,
					GA_AttributeOptions &current_options) const;

    /// @}
    //
    /// Returns the dictionary that should be checked for a cross dictionary
    /// collision (GA_ATTRIB_VERTEX for GA_ATTRIB_POINT and vice versa) and
    /// GA_ATTRIB_INVALID when no check is needed.
    GA_AttributeOwner	 getPointVertexCollisionDict(
						GA_AttributeOwner dict) const
    {
	if (myPointVertexCollisionStrategy != PT_VTX_COLLISION_IGNORE)
	{
	    if (dict == GA_ATTRIB_POINT)
		return GA_ATTRIB_VERTEX;
	    if (dict == GA_ATTRIB_VERTEX)
		return GA_ATTRIB_POINT;
	}
	return GA_ATTRIB_INVALID;
    }

    /// Only call this method if getPointVertexCollisionDict() does not return
    /// GA_ATTRIB_GLOBAL.  Returns true if the copy was successful, or was not
    /// needed.  False otherwise.
    bool		 applyPointVertexCollisionDataCopyStrategy(
					GA_Attribute *dest, bool replacement,
					const GA_Attribute *src) const;

private:
    CollisionStrategy		 myCollisionStrategy;
    FailureStrategy		 myFailureStrategy;
    TypeStrategy		 myTypeStrategy;
    StorageStrategy		 myStorageStrategy;
    TupleSizeStrategy		 myTupleSizeStrategy;
    OptionsStrategy		 myOptionsStrategy;
    PointVertexCollisionStrategy myPointVertexCollisionStrategy;
};

#endif
