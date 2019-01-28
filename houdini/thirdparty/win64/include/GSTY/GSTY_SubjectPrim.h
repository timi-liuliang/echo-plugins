/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GSTY_SubjectPrim.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GSTY_SubjectPrim__
#define __GSTY_SubjectPrim__

#include "GSTY_API.h"
#include <STY/STY_SubjectNamed.h>
#include <GA/GA_Handle.h>

class GSTY_SubjectPrimGroup;
class GEO_Primitive;
class GEO_Detail;

/// Represents a primitive whose material properties are being overiden.
class GSTY_API GSTY_SubjectPrim : public STY_SubjectNamed
{
public:
    GSTY_SubjectPrim(const GEO_Primitive *prim,
	    int numeric_prim_group_offset,
	    int numeric_point_group_offset,
	    GSTY_SubjectPrimGroup *subject_group,
	    const GA_ROHandleS &detail_attrib,
	    const GA_ROHandleS &prim_attrib);

    // Note: this constructor will perform attribute lookups, which can be
    // expensive.
    GSTY_SubjectPrim(const GEO_Primitive *prim,
	    GSTY_SubjectPrimGroup *subject_group);

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

    const GEO_Primitive &getPrimitive() const;

    /// Returns true if the provided detail has any attributes that may
    /// contribute style information to this subject.
    static bool	 hasStyleAttrib(const GEO_Detail &gdp);

private:
    /// Base class overrides.
    virtual UT_StringHolder	getSubjectName() const;
    virtual int64		getSubjectID() const;

    /// Reference to the primitve represented by this styling subject.
    const GEO_Primitive		*myPrimitive;
    GSTY_SubjectPrimGroup	*mySubjectGroup;
    GA_ROHandleS		 myDetailAttrib;
    GA_ROHandleS		 myPrimAttrib;
    int				 myNumericPrimGroupOffset;
    int				 myNumericPointGroupOffset;
};

#endif

