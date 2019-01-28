/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_SubjectSelf.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_SubjectSelf__
#define __STY_SubjectSelf__

#include "STY_API.h"
#include "STY_Subject.h"
#include <UT/UT_StringHolder.h>

/// ===========================================================================
/// Subject that matches self targets and leaves all other targets intact
/// for its children to match.
class STY_API STY_SubjectSelf : public STY_Subject
{
public:
		STY_SubjectSelf();
		STY_SubjectSelf(const char *pre_filter_style_sheet,
			const char *post_filter_style_sheet);
		STY_SubjectSelf(const STY_Subject &underlying_subject);

    virtual STY_StyleSheetHandle
		getPreFilterStyleSheet() const;

    virtual STY_StyleSheetHandle
		getPostFilterStyleSheet() const;

    virtual STY_TargetMatchStatus
		matchesStyleTarget(const STY_TargetHandle &target) const;

    virtual STY_BindingDataSourceHandle
		getOverrideBindingDataSource(
			const UT_StringHolder &data_name) const;

private: 
    const STY_Subject		*mySelfSubject; 
    STY_StyleSheetHandle	 myPreFilterStyleSheet;
    STY_StyleSheetHandle	 myPostFilterStyleSheet;
};

#endif

