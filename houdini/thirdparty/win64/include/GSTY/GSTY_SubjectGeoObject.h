/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GSTY_SubjectGeoObject.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GSTY_SubjectGeoObject__
#define __GSTY_SubjectGeoObject__

#include "GSTY_API.h"
#include <STY/STY_Subject.h>
#include <UT/UT_TagManager.h>

class GSTY_BundleMap;

/// Represents a geometry object whose primitives are being overriden.
class GSTY_API GSTY_SubjectGeoObject : public STY_Subject
{
public:
		GSTY_SubjectGeoObject(const UT_StringHolder &path,
			const UT_TagListPtr &tags,
			const GSTY_BundleMap &bundles,
			const UT_StringHolder &stylesheet);

    /// Base class overrides.
    virtual STY_TargetMatchStatus
		matchesStyleTarget(const STY_TargetHandle &target) const;
    virtual STY_StyleSheetHandle
		getPostFilterStyleSheet() const;
    virtual UT_StringHolder
		getSourceLocation() const;

private:
    bool	matchesTagOrBundleTarget(const STY_TargetHandle &target) const;
    STY_TargetMatchStatus
		matchesComponentTarget(const STY_TargetHandle &target) const;
    STY_TargetMatchStatus
		matchesGeoObjTarget(const STY_TargetHandle &target) const;

private:
    /// Full path and name of the geometry object. Store them both so we don't
    /// need to recreate the name from the path repeatedly.
    const UT_StringHolder	 myObjectPath;
    UT_StringHolder		 myObjectName;

    /// Tags set on the geometry object.
    const UT_TagListPtr		 myTags;

    /// Reference to bundle data.
    const GSTY_BundleMap	&myBundles;

    /// The style sheet provided by this object.
    STY_StyleSheetHandle	 myStyleSheet;
};
#endif

