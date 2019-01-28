/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GU_ElementGroup_h__
#define __GU_ElementGroup_h__

#include "GU_Group.h"

class GU_Detail;
class GA_ElementGroup;
class GA_Group;

class GU_API GU_ElementGroup : public GU_Group
{
public:
    GU_ElementGroup(GU_Detail *gdp);
    virtual ~GU_ElementGroup();
    
    // Select items based on a boolean operation done between one or two
    // different groups (e.g. intersect, union, subtraction, negation).
    // Returns false if any error was encountered and true otherwise.
    virtual bool	 boolean(GU_GroupBoolOp boolop, 
				 const UT_String &group1, bool negate1,
				 const UT_String &group2, bool negate2) const;

    virtual bool	 boolean(GU_GroupBoolOp boolop, 
				 GA_Group *group1, bool negate1,
				 GA_Group *group2, bool negate2) const;

    GA_ElementGroup	* elementGroup() const;


};

#endif
