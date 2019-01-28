/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Constraint_h__
#define __SIM_Constraint_h__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"

class SIM_Relationship;
class SIM_ConAnchor;
class SIM_ConRel;

/// The intention of this class is to act as a flexible container for
/// constraint data. The individual components of the container (the
/// relationship, the anchors, etc.) can each be switched at need, without
/// affecting the other components.
class SIM_API SIM_Constraint : public SIM_RelationshipData
{
public:
    void		 initConstraint(const SIM_Relationship *relationship);
    SIM_ConRel		*getConRel();
    const SIM_ConAnchor	*getAnchor(int index) const;

protected:
    explicit		 SIM_Constraint(const SIM_DataFactory *factory);
    virtual		~SIM_Constraint();

    virtual long	 getGuideParmVersionSubclass() const;
    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    const SIM_ConRel			*getConstConRel() const;

    static const SIM_DopDescription	*getDopDescription();

    static bool				 theMakingStateTransitions;

    friend class			 SIM_ConstraintIterator;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_Constraint,
			SIM_RelationshipData,
			"Constraint",
			getDopDescription());
};

#endif

