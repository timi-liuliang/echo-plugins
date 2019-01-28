/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Relationship_h__
#define __SIM_Relationship_h__

#include "SIM_API.h"
#include "SIM_RootData.h"
#include "SIM_ObjectArray.h"
#include <iosfwd>

class UT_IStream;
class SIM_SimulationState;

/// This class defines a relationship between two groups of objects.
/// The first group of objects, called simply the "group", is the set
/// of objects that are modified or controlled by the relationship. The
/// second group of objects, the "affector group", is the set of objects
/// that control the "group" objects. Note that these sets of objects
/// can overlap in any way. It is also possible to have a relationship
/// without any affector group. A DOP Object Group is an example of this
/// kind of affector-less relationship. Mutual affector relationships
/// occur when one or more objects are in both the object group and the
/// affector group. This also implies a self-affecting relationship.
///
/// This data type is not meant to be subclassed, and does not contain any
/// information about the nature or meaning of the relationship. That
/// semantic information is held in the subdata of the SIM_Relationship.
/// Only one subdata can be attached to this data at a time, and it must
/// be a subclass of SIM_RelationshipData. Any other kind or number of
/// subdata are ignored and invalidate the relationship.
///
/// Any data that refers to other data should be implemented as a
/// SIM_Relationship. Doing so avoids problems of having to lookup objects
/// based on name, enforces proper solve order (so affector objects
/// are always solved before the group objects), and allows reverse
/// lookups (what objects are in a relationship, and what relationships
/// refer to a particular object).
///
/// Like SIM_Objects, SIM_Relationships are created and controlled by a
/// SIM_SimulationState. They cannot be created and attached as subdata
/// on existing data. Unlike SIM_Objects, SIM_Relationships do not have
/// a unique integer identifier. Relationships must be given unique names.
class SIM_API SIM_Relationship : public SIM_RootData
{
public:
    /// Returns the single subdata that defines the relationship type.
    SIM_Data		*getRelationshipTypeData();
    /// Returns the single const subdata that defines the relationship type.
    const SIM_Data	*getConstRelationshipTypeData() const;

    /// Sets the object group equal to the set of supplied objects.
    void		 setGroup(const SIM_ObjectArray &objects);
    /// Sets the object group to contain only the single specified object.
    void		 setGroup(SIM_Object *object);
    /// Adds a number of objects to the object group.
    void		 addGroup(const SIM_ObjectArray &objects);
    /// Adds a single object to the object group.
    void		 addGroup(SIM_Object *object);
    /// Removes a number of objects from the object group.
    void		 removeGroup(const SIM_ObjectArray &objects);
    /// Removes a single object from the object group.
    void		 removeGroup(SIM_Object *object);
    /// Removes all objects from the object group.
    void		 clearGroup();
    /// Gets the number of objects in the object group.
    int			 getGroupEntries() const;
    /// Gets the object at position index within the object group.
    const SIM_Object	*getGroupObject(int index) const;
    /// Tests whether the supplied object is in the object group.
    bool		 getGroupHasObject(const SIM_Object *object) const;
    /// Sets the affector group equal to the set of supplied objects.
    void		 setAffGroup(const SIM_ObjectArray &objects);
    /// Sets the affector group to contain only the single specified object.
    void		 setAffGroup(SIM_Object *object);
    /// Adds a number of objects to the affector group.
    void		 addAffGroup(const SIM_ObjectArray &objects);
    /// Adds a single object to the affector group.
    void		 addAffGroup(SIM_Object *object);
    /// Removes a number of objects from the affector group.
    void		 removeAffGroup(const SIM_ObjectArray &objects);
    /// Removes a single object from the affector group.
    void		 removeAffGroup(SIM_Object *object);
    /// Removes all objects from the affector group.
    void		 clearAffGroup();
    /// Gets the number of objects in the affector group.
    int			 getAffGroupEntries() const;
    /// Gets the object at position index within the affector group.
    const SIM_Object	*getAffGroupObject(int index) const;
    /// Tests whether the supplied object is in the affector group.
    bool		 getAffGroupHasObject(const SIM_Object *object) const;

protected:
			 SIM_Relationship(const SIM_SimulationState *factory);
    virtual		~SIM_Relationship();

    /// Clears all attached objects.
    virtual void	 initializeSubclass();
    /// Makes the relationship groups equal, but doesn't set the relationship
    /// name, which must be kept unique.
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    /// Saves a relationship to a stream. The object groups are saved to
    /// the stream as arrays of object id numbers.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads a relationship from a stream.
    virtual bool	 loadSubclass(UT_IStream &is);
    /// Creates a query object specific to SIM_Relationship.
    virtual SIM_Query	*createQueryObjectSubclass() const;
    /// Returns the memory usage of our group data.
    virtual int64	 getMemorySizeSubclass() const;

    /// Override the SIM_RootData function for matching us to a string.
    virtual bool	 getMatchesStringSubclass(const char *pattern) const;
    /// Override the SIM_RootData function for getting our identifier.
    virtual void	 getRootDataIdSubclass(SIM_RootDataId &id) const;

private:
    /// The function to set the name is private because it should only be
    /// called by the load function or by SIM_SimulationState.
    void		 setName(const char *name);
    /// Clear ourselves from the relationship arrays of our group objects.
    void		 clearAllGroupReferences();
    /// Add ourselves to the relationship arrays of our group objects.
    void		 setAllGroupReferences();
    /// Clear ourselves from the relationship arrays of our affector objects.
    void		 clearAllAffGroupReferences();
    /// Add ourselves to the relationship arrays of our affector objects.
    void		 setAllAffGroupReferences();

    const SIM_SimulationState	*mySimulationState;
    SIM_ObjectArray		 myGroup;
    SIM_ObjectArray		 myAffGroup;

    friend class		 SIM_SimulationState;
    friend class		 SIM_Object;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Relationship, SIM_RootData);
};

#endif

