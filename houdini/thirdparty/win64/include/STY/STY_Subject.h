/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Subject.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Subject__
#define __STY_Subject__

#include "STY_API.h"
#include "STY_Types.h"
#include "STY_BindingDataSource.h"
#include <UT/UT_IntrusivePtr.h>

class STY_TargetMatchStatus;

/// Describes the entity subjected to stylilng, ie, entity whose properties can 
/// be overriden by style sheets (eg, a specific primitive in the whole scene,
/// whose material diffuse color is being overriden from blue to red).
/// Subjects matching a given target may be asked to provide pre binding
/// data associated with the target.
/// Also, subject may optionally provide additional style sheet, which contains
/// overrides that are intended to itself (self-targeted entries) or that are
/// intended for subject's children (non self-target entries), if subjects
/// are structured hierarchically (eg, in the case of packed primitives).
class STY_API STY_Subject :
	public UT_IntrusiveRefCounter<STY_Subject>
{
public:
    virtual	~STY_Subject()
		 { }

    /// Returns the information wheter the subject matches the given target.
    virtual STY_TargetMatchStatus   
		matchesStyleTarget(const STY_TargetHandle &target) const = 0;

    /// Returns a source object that can provide data for the given data name.
    /// Eg, style subject representing a primitive can provide primitive
    /// attribute data.
    virtual STY_BindingDataSourceHandle
		getOverrideBindingDataSource(
			const UT_StringHolder &data_name) const;

    /// Returns the style sheet that should be applied 
    /// to this subject. This style sheet is applied _before_ this subject
    /// is used to filter out non-matching styles. This subject is the top
    /// level target for these styles.
    /// Returns an empty string, if no style sheet is provided.
    virtual STY_StyleSheetHandle
		getPreFilterStyleSheet() const;

    /// Returns the style sheet that should be applied 
    /// to this subject and its children. This style sheet is applied _after_
    /// this subject is used to filter out non-matching styles. The children
    /// of this subject are the top level targets for these styles.
    /// Returns an empty string, if no style sheet is provided.
    virtual STY_StyleSheetHandle
		getPostFilterStyleSheet() const;

    /// Returns the location where the above subject-specific style sheet
    /// comes from. For example the name of the object, prim number, etc.
    /// Returns a default value of "Unknown Subject Source".
    virtual UT_StringHolder
		getSourceLocation() const;

protected:
    /// Helper for handling path requirement.
    STY_TargetMatchStatus
		matchesPathHelper(const UT_StringRef &path,
			const STY_TargetHandle &target,
			char separator) const;
};

#endif

