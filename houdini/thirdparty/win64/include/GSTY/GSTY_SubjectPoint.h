/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GSTY_SubjectPoint.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GSTY_SubjectPoint__
#define __GSTY_SubjectPoint__

#include "GSTY_API.h"
#include <GA/GA_Types.h>
#include <GA/GA_Handle.h>
#include <STY/STY_SubjectNamed.h>

class GSTY_SubjectPointGroup;
class GEO_Detail;

/// Represents a primitive whose material properties are being overiden.
class GSTY_API GSTY_SubjectPoint : public STY_SubjectNamed
{
public:
    GSTY_SubjectPoint(const GEO_Detail &gdp,
		GA_Offset offset,
		int numeric_point_group_offset,
		GSTY_SubjectPointGroup *subject_group,
		const GA_ROHandleS &detail_attrib,
		const GA_ROHandleS &point_attrib);

    // Note: this constructor will perform attribute lookups, which can be
    // expensive.
    GSTY_SubjectPoint(const GEO_Detail &gdp,
		GA_Offset offset);

    /// Base class overrides.
    virtual STY_TargetMatchStatus 
		 matchesStyleTarget(const STY_TargetHandle &target) const;
    virtual STY_BindingDataSourceHandle
		 getOverrideBindingDataSource(
			 const UT_StringHolder &data_name) const;
    virtual STY_StyleSheetHandle
		 getPreFilterStyleSheet() const;
    virtual STY_StyleSheetHandle
		 getPostFilterStyleSheet() const;
    virtual UT_StringHolder
		 getSourceLocation() const;

    const GEO_Detail	&getGdp() const;
    GA_Offset		 getOffset() const;

    /// Returns true if the provided detail has any attributes that may
    /// contribute style information to this subject.
    static bool	 hasStyleAttrib(const GEO_Detail &gdp);

private:
    /// Base class overrides.
    virtual UT_StringHolder getSubjectName() const;
    virtual int64	 getSubjectID() const;

    /// Reference to the primitve represented by this styling subject.
    const GEO_Detail		*myGdp;
    GSTY_SubjectPointGroup	*mySubjectGroup;
    GA_ROHandleS		 myDetailAttrib;
    GA_ROHandleS		 myPointAttrib;
    GA_Offset			 myOffset;
    int				 myNumericPointGroupOffset;
};

#endif

