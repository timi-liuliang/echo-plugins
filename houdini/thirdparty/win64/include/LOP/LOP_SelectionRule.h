/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LOP Library (C++)
 *
 * COMMENTS:	Flat list of primitive paths, or rule-based description of a
 *		collection of USD primitives.
 *
 */

#ifndef __LOP_SelectionRule_h__
#define __LOP_SelectionRule_h__

#include "LOP_API.h"
#include <HUSD/HUSD_DataHandle.h>
#include <HUSD/HUSD_Utils.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>

class UT_JSONWriter;
class UT_JSONParser;

class LOP_API LOP_SelectionRule
{
public:
			 LOP_SelectionRule();
			~LOP_SelectionRule();

    bool		 operator==(const LOP_SelectionRule &other) const;
    bool		 operator!=(const LOP_SelectionRule &other) const;
    int			 hash() const;

    void		 save(UT_JSONWriter &writer) const;
    bool		 load(UT_JSONParser &parser);

    void		 getExpandedPaths(HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands,
				UT_StringArray &paths) const;
    void		 getCollectionAwarePaths(HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands,
				UT_StringArray &paths) const;

    const UT_StringHolder &pathPattern() const
			 { return myPathPattern; }
    void		 setPathPattern(const UT_StringHolder &pattern);

private:
    UT_StringHolder	 myPathPattern;
};

typedef UT_StringMap<LOP_SelectionRule> LOP_SelectionRuleMap;

#endif
