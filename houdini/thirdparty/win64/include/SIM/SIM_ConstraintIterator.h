/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConstraintIterator_h__
#define __SIM_ConstraintIterator_h__

#include "SIM_API.h"
#include "SIM_Constraint.h"
#include <UT/UT_ValArray.h>

class SIM_DataFilter;
class SIM_Object;
class SIM_ConAnchor;

/// Iterator for constraints.
///
/// This class simplifies the process of scanning the constraints
/// associated with an object. Constraints are complicated to iterate: you
/// have to scan over the SIM_Constraints, then over each SIM_Constraint's
/// SIM_Relationships, then over the anchors within the relationship. This
/// class hides that complexity.
///
/// The iterator is similar in principal to an STL iterator, but somewhat
/// simpler. The classic STL loop
///   for ( it = begin(); it != end(); ++it )
/// is done using
///   for ( it.rewind(); !it.atEnd(); it.advance() )
/// 
/// The IterationStyle parameter in the SIM_ConstraintIterator's constructor
/// affects how constraint anchors of the type SIM_ConAnchorObjPointGroupPos
//  are traversed:
///
///     PRESENT_GROUP_AS_MULTIPLE:
///         This will present each point in a group as a separate
///         point anchor. That means that the iterator will point to the
///         same anchor multiple times during a traversal.
///         During each visit of this traversal, the anchor will pose as
//          a different point in the group.
///         SIM_ConstraintIterator implements this iteration
///         mode by using the members "reset", "nextPoint", and "atEnd"
///         to manipulate the group anchor's "current point.
///
///     PRESENT_GROUP_AS_SINGLE:
///         This will treat a group constraint as a single constraint.
///         It will only be visited once during an iteration traversal.
///         The client code will have to explicitly extract the individual
///         points.
///

class SIM_API SIM_ConstraintIterator
{
public:
    enum IterationStyle {
        PRESENT_GROUP_AS_MULTIPLE,
	PRESENT_GROUP_AS_SINGLE
    };

    /// Iterate over all constraint relationships on the given object,
    /// accepting only those whose relationships pass the given filter. (Note
    /// that relationships don't necessarily have any data "name", so filtering
    /// by name is meaningless.)
    /// The requirePinAnchor option forces the iteration to only include
    /// constraints that are pinned to this object.
			 SIM_ConstraintIterator(SIM_Object &object,
				    const SIM_DataFilter *relFilter,
				    const SIM_DataFilter *currentFilter,
				    const SIM_DataFilter *goalFilter,
				    const SIM_Time &time,
				    const IterationStyle style = 
				        PRESENT_GROUP_AS_MULTIPLE
				    );
			~SIM_ConstraintIterator();

    /// Test to see if the iterator is at the end.
    bool			 atEnd() const;
    /// Advance the iterator to the next item.
    void			 advance();
    /// Reset the iterator to the start of iteration.
    void			 rewind();

    /// Get the constraint associated with the current item.
    SIM_Constraint		*getConstraint() const;

    /// Get the relationship associated with the current constraint.
    SIM_ConRel			*getConRel() const;
    const SIM_ConAnchor		*getCurrentAnchor() const;
    const SIM_ConAnchor		*getGoalAnchor() const;

    /// Call all constraints to make state transitions based on their most
    /// recent state data updates.
    static void			 makeStateTransitions(SIM_Object &object,
						      const SIM_Time &time);
    /// Call all constraints to set the valid state on them.
    static void			 initConstraints(SIM_Object &object,
						 const SIM_Time &time);

private:
    /// Disallowed.
				 SIM_ConstraintIterator(
					const SIM_ConstraintIterator &);
    SIM_ConstraintIterator	&operator=(const SIM_ConstraintIterator &);

    /// Advance indexes until we point to a valid item.
    /// (i.e. a constraint with a non-zero number of relationships)
    void			 skipInvalid();

    int						 myObjectId;
    const SIM_Time				&myTime;
    const SIM_DataFilter			*myRelFilter;
    const SIM_DataFilter			*myCurrentFilter;
    const SIM_DataFilter			*myGoalFilter;
    UT_ValArray<SIM_Relationship *>		 myRelationships;
    UT_ValArray<SIM_Constraint *>		 myConstraints;
    SIM_ConRel					*myConRel;
    const SIM_ConAnchor				*myCurrentAnchor;
    const SIM_ConAnchor				*myGoalAnchor;
    int						 myConIdx;
    bool					 myIsMirrored;
    const IterationStyle                         myIterationStyle;
};

extern SIM_API void
SIMfindConstraints(
    SIM_Object &object,
    UT_ValArray<SIM_Relationship *> *rels,
    UT_ValArray<SIM_Constraint *> *cons);

#endif
