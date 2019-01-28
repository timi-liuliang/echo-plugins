/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Target.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Target__
#define __STY_Target__

#include "STY_API.h"
#include "STY_Types.h"
#include "STY_TargetType.h"
#include <UT/UT_Array.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class UT_JSONValue;
class UT_JSONValueMap;

/// The style target represents an abstract description of a style subject 
/// to which the overrides should be applied. When a style sheet entry is
/// tested whether its overrides should be applied to a style subject,
/// it asks the given style subject to test its target for a match.
/// Thus, it is up to the style subject to interpret the target's requirement
/// data and decide if it satisfies them or not.
///
/// The style target is purposely generic, because if it were specific then
/// each usecase of the targets would need to implement own specific target
/// meeting its needs, and it's hard to write own custom target class because
/// it needs to load from JSON into our style sheet class. In the future,
/// we may provide style target factory registration that uses targetType
/// to choose specific implementation class, but now there is no such mechanism.
///
/// The style subject, on the other hand, is easy to subclass from, and thus
/// it is style subject's responsibility to perform the matching and processing
/// of style targets.
class STY_API STY_Target :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_Target>
{
public:
    /// Constructor of a target based on JSON encoding.
    /// The 'is_selftarget' flag is true for targets that refer to the subjects 
    /// that provided the style sheet definition (it's false for most cases).
			 STY_Target(const UT_JSONValue *json,
				bool is_selftarget);
    /// Construct from requirements and a subtarget.
			 STY_Target(const UT_StringHolder &label,
				const STY_TargetType &type,
				const STY_TargetHandle &sub_target,
				const STY_ConstantMap &requirements,
				const UT_Array<STY_BindingHandle> *pre_bind);
			~STY_Target();

    /// Returns true if this target is a completely default target
    /// specification (equivalent to no target specification at all in JSON).
    /// This means a self target with no conditions, no bindings, and no
    /// sub-target.
    bool		 isEquivalentToNoTarget() const;

    /// Returns the label given to this target.
    const UT_StringHolder &getLabel() const
			{ return myLabel; }

    /// Returns the target type. It's a basic constraint that many style
    /// subjects test before proceeding with further tests.
    const STY_TargetType &getType() const
			{ return myType; }

    /// Returns the target requirements in the form of key-value dictionary.
    /// Subjects query data in this dictionary to retrieve the values they
    /// deem to necessary to match. 
    const STY_ConstantMap &getTargetRequirements() const
			{ return myTargetRequirements; }

    /// Convenience getter for requirement string. Returns NULL if the
    /// key is not in the requirements. See the STY_REQUIREMENT constants
    /// defined in STY_Types.h for the most common cases.
    const char		*getRequirement(const UT_StringHolder &key) const;

    /// Returns an array of pre-binding, which contain names of the data
    /// that should be retrieved from the subject that match this target.
    /// The data (source) is usually stored in a match closure object, 
    /// and is used for input arguments of override scripts.
    const UT_Array<STY_BindingHandle> *
			getPreBindings() const
			{ return myPreBindings.get(); }

    /// Returns this target's sub-target than needs to be matched by subsequent
    /// styling subject. Usually, styling subjects are arranged in a hiearchy 
    /// (eg, crowd geometry object > agent primitive > agent's shape), and
    /// it is useful for the style target to mimic this hierarchy so that
    /// each subject in the hierarchy has a corresponding target, with its
    /// own set of requirements.
    const STY_TargetHandle &getSubTarget() const
			{ return mySubTargetHandle; }

    /// Creates a JSON value map for saving to a style sheet.
    void		saveStyleSheet(UT_JSONValueMap *map,
			    const STY_TargetMatchClosureHandle &closure) const;

private:
    void		 loadStyleTarget(const UT_JSONValue *json);
    bool		 loadStyleTargetPart(const UT_StringHolder &key,
				const UT_JSONValue *value);

    /// A user-defined label for the target.
    UT_StringHolder				 myLabel;

    /// Target type indended to serve for the basic identification of target.
    STY_TargetType				 myType;

    /// Target's requirements specifications. 
    STY_ConstantMap				 myTargetRequirements;

    /// Target's pre-bindings for override script's input arguments.
    /// These are the names of the data that should be retrieved from subjects 
    /// matching this target (usually a component of a compound target ). 
    UT_UniquePtr<UT_Array<STY_BindingHandle> >	 myPreBindings;

    /// The next target that needs to be matched by the next styling subject.
    STY_TargetHandle				 mySubTargetHandle;
};

#endif

