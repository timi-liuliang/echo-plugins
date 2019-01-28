/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchor_h__
#define __SIM_ConAnchor_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

class SIM_Object;
class SIM_Relationship;

/// Definition of the relationship between a constraint and an anchor
/// (typically part of a SIM_Object).
///
/// It knows
/// 1) which object the anchor is associated with (may be none)
/// 2) which part of that object (e.g., point, primitive, etc.)
/// 3) the position, orientation and linear/angular velocities of the
///    anchor
class SIM_API SIM_ConAnchor : public SIM_Data,
			      public SIM_OptionsUser
{
public:
    /// Get the object pointer stored by setReferencedObject(). This is
    /// the object that defines this anchor, if there is one. The object
    /// is interpolated to the specified simulation time so that the
    /// returned anchor values are properly interpolated.
    const SIM_Object	*getReferencedObject(const SIM_Time &time) const;

    /// Returns true if the anchor has to be initialized before it is used.
    /// Initialization involves calling initAnchor() on the anchor, passing
    /// in the object we are attached to, so the anchor can customize itself
    /// to a particular object. This function calls getNeedsInitSubclass().
    bool		 getNeedsInit() const;
    /// This function performs any initialization that may be required
    /// to make a constraint specific to a particular object. For example,
    /// in spatial constraints, this function may convert a world space
    /// position parameter to an object space position. This function
    /// calls initAnchorSubclass().
    void		 initAnchor(const SIM_Object *object);
    /// This function will return true if this anchor class requires an
    /// object specification otherwise it returns false.  The returned string
    /// value can be used in an object filter expression.
    bool		 getObjectSpecification(UT_String &str) const;
    /// This function is responsible for calling the initAnchor function
    /// which initialize this anchor based on the owner relationship.
    void		 initConstraint(const SIM_Relationship *rel);

    /// Slight variation on buildGuideGeometry. It's just like
    /// buildGuideGeometry, but it will be called by the constraint using
    /// the anchor, and will add some extra options about the status of
    /// that constraint.
    void		 buildAnchorGuideGeometry(const SIM_Options &options,
						  const GU_DetailHandle &gdh,
						  const SIM_Relationship &rel,
						  const SIM_Time &t) const;

protected:
    explicit		 SIM_ConAnchor(const SIM_DataFactory *factory);
    virtual		~SIM_ConAnchor();

    /// Sets the object that this anchor is defined by. Calling this
    /// function is the responsibility of the subclasses that are defined
    /// by objects. This function is generally called from
    /// initAnchorSubclass().
    void		 setReferencedObject(const SIM_Object *object);

    /// Returns true if the anchor needs to have initAnchor() called. The
    /// default implementation returns false.
    virtual bool	 getNeedsInitSubclass() const;
    /// Initializes the anchor for use by a solver. The default implementation
    /// does nothing.
    virtual void	 initAnchorSubclass(const SIM_Object *object);
    /// Implements the real functionality of getObjectSpecification.
    virtual bool	 getObjectSpecificationSubclass(UT_String &str) const;
    /// Implements the real functionality of initConstraint. The default
    /// implementation calls initAnchor on the object from the relationship
    /// that we are interested in.
    virtual void	 initConstraintSubclass(const SIM_Relationship *rel);

    /// Default implementation does nothing.
    virtual void	 buildAnchorGuideGeometrySubclass(
					const SIM_Options &options,
					const GU_DetailHandle &gdh,
					const SIM_Relationship &rel,
					const SIM_Time &t) const;

private:
    const SIM_Object	*myReferencedObject;
    SIM_Time		 myReferencedObjectTime;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchor, SIM_Data);
};

#endif
