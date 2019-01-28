/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_TargetMatchClosure.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_TargetMatchClosure__
#define __STY_TargetMatchClosure__

#include "STY_API.h"
#include "STY_Types.h"
#include "STY_BindingDataSource.h"
#include <UT/UT_StringMap.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class UT_StringArray;

/// Class containing information about data associated with a style subject
/// match to a style target. It contains concrete data provided by the subject,
/// such as data for script input argument binding, etc.
class STY_API STY_TargetMatchClosure :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_TargetMatchClosure>
{
public:
    /// Constructor.
		STY_TargetMatchClosure(
		    const STY_TargetMatchClosureHandle &parent_closure );

    /// Registers an override binding data source for a given data name.
    void	addOverrideBindingDataSource(const UT_StringHolder &entry_name,
		    const STY_BindingDataSourceHandle &data_source);

    /// Returns a data source given the data name for override script 
    /// input argument binding.
    STY_BindingDataSourceHandle
		findOverrideBindingDataSource(
		    const UT_StringHolder &entry_name) const;

    /// Returns all bindings in the closure (or any parent closures).
    void	getBindingDataSourceNames(UT_StringArray &entry_names) const;

private:
    /// A table of data sources for a given data name.
    UT_StringMap<STY_BindingDataSourceHandle>	myBindingDataSources;

    /// Parent closure constructed from previous partial matches of
    /// sub-targets, if any.
    STY_TargetMatchClosureHandle		myParentClosure;
};

#endif

