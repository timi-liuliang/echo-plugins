/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_ResultsFilter.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_ResultsFilter__
#define __STY_ResultsFilter__

#include "STY_API.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_NonCopyable.h>

class STY_Style;

/// Filter that decides which results should be retrieved with
/// a call to getResults() in a styler. 
/// Since virtually all calls to getResults() will want results from 
/// a specific category, the constructor takes category mask for convenience.
/// Any custom filtering can be done by deriving from this class.
class STY_API STY_ResultsFilter : public UT_NonCopyable
{
public:
    /// Constructor, which for convenience takes a mask for accepted
    /// override categories. The argument can be NULL for all categories.
		 STY_ResultsFilter(const char *category_mask = nullptr);
    virtual	~STY_ResultsFilter();

    /// Returns true if the filter allows the overrides from the given category.
    virtual bool acceptsCategory( const UT_StringHolder &category_name ) const;

    /// Returns true if the filter allows the overrides from style sheet entries
    /// of lower precedence level than the given style entry.
    /// Usually the overrides from all matching entries are allowed, but
    /// sometimes an entery may block any further overrides, eg, if an style
    /// contains 'material' override, andy lower level overrides for
    /// 'materialParameters' need to be disregarded, since they were intended
    /// for the old material.
    virtual bool acceptsFurtherOverrides(const STY_Style &style)const;

private:
    /// Mask (pattern) for accepted override categories.
    UT_StringHolder	myCategoryMask;
};

/// This predefined filter eliminates all results of lower precedence if
/// it detects a result setting the current material. This is because lower
/// precedence results are most likely directed at the lower precedence
/// material assignment, and so should only affect that material, not the
/// newly assigned material.
class STY_API STY_MaterialResultsFilter : public STY_ResultsFilter
{
public:
    STY_MaterialResultsFilter() :
	// TODO: ask only for material and material parameter overrides
	STY_ResultsFilter(nullptr)
    {}

    virtual bool acceptsFurtherOverrides(const STY_Style &style) const;
};

#endif

