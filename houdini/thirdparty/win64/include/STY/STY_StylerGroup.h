/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_StylerGroup.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_StylerGroup__
#define __STY_StylerGroup__

#include "STY_API.h"
#include "STY_Styler.h"
#include <UT/UT_StringArray.h>
#include <UT/UT_NonCopyable.h>

class STY_SubjectGroup;
class STY_Results;
class STY_ResultsFilter;

/// Represents a collection of STY_Stylers in a single class. This allows for
/// more efficient processing of multiple stylers.
class STY_API STY_StylerGroup : public UT_NonCopyable
{
public:
			 STY_StylerGroup();
    virtual		~STY_StylerGroup();

    /// Returns the number of stylers in this group.
    int			 getNumStylers() const;

    /// Get a single styler corresponding to the subject with the same index
    /// in the STY_SubjectGroup that was used to create this STY_StylerGroup.
    const STY_Styler	&getStyler(int idx) const;

    /// Returns style sheets for all stylers in this group.
    void		 saveStyleSheets(UT_StringArray &stylesheets,
					 bool verbose=false,
					 bool path=false) const;

    /// Returns results for every styler in the group.
    /// This method returns results from entries whose targets
    /// have been previously fully matched by the subjects in the subject
    /// hierarchy.
    /// 
    /// @param filter		An object that decides what kind of override
    ///	    values the caller is interested in. It filters out results
    ///	    that are not needed, and allows in the ones that the caller
    ///	    wants to obtain. Eg, the caller may want only the results
    ///	    in the 'materialParameters' category.
    void		 getResults(UT_Array<STY_Results> &results,
				const STY_ResultsFilter &filter) const;

    /// Adds stylers to this group from a parent styler and a subject group.
    void		 append(const STY_Styler &parent_styler,
				const STY_SubjectGroup &subjects);
    /// Add a single styler that has been fully pruned. As much as possible
    /// the group functions should be used, but sometimes we just care about
    /// a single styler.
    void		 append(const STY_Styler &styler);

private:
    // A block of stylers created by a call to append.
    class STY_StylerGroupData :
	public UT_IntrusiveRefCounter<STY_StylerGroupData>
    {
    public:
	UT_Array<STY_Styler>			 myStylers;
	UT_Array<STY_StylerGroupMatchHandle>	 myMatches;
	exint					 myNumParentMatches;
	exint					 myNumParentStyleSheets;
    };
    typedef UT_IntrusivePtr<STY_StylerGroupData> STY_StylerGroupDataHandle;

    // Our array of group data blocks.
    UT_Array<STY_StylerGroupDataHandle>		 myDataArray;
    exint					 myTotalNumStylers;
};

#endif

