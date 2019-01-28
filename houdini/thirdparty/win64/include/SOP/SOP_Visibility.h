/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *	Hides and exposes geometry by adding to/removing from a primitive
 *	group.
 */

#ifndef __SOP_Visibility_h__
#define __SOP_Visibility_h__

#include "SOP_API.h"
#include "SOP_Node.h"

// Constants for parameter indices:
#define SOP_VISIBILITY_GROUP_INDEX		0
#define SOP_VISIBILITY_HIDE_OR_EXPOSE_INDEX	1
#define SOP_VISIBILITY_SEL_OR_NONSEL_INDEX	2
#define SOP_VISIBILITY_3D_OR_2D_INDEX		3
#define SOP_VISIBILITY_CUMULATIVE		4

class SOP_API SOP_Visibility : public SOP_Node
{
public:
	     SOP_Visibility(OP_Network *net, const char *name, 
			    OP_Operator *entry);
    virtual ~SOP_Visibility();

    // These enumerated types are public so users can get or set
    // parameters.
    enum { HIDE, EXPOSE };
    enum { SELECTED, NON_SELECTED };
    enum { THREE_D, TWO_D, TWO_D_AND_THREE_D };

    virtual OP_ERROR	 cookInputGroups(OP_Context &context, 
					 int alone = 0);

    static OP_Node	*myConstructor(OP_Network *net, 
				       const char *name, 
				       OP_Operator *entry);

    static PRM_Template  myTemplateList[];

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    virtual const char 	*inputLabel(unsigned idx) const;

    void		 performVisibility(fpreal t);

private:
    void	 getGroups(UT_String &str, fpreal t) 
		{ evalString(str, SOP_VISIBILITY_GROUP_INDEX, 0, t); }
    int		 HIDE_OR_EXPOSE(fpreal t)
		{ return evalInt(SOP_VISIBILITY_HIDE_OR_EXPOSE_INDEX, 0, t); }
    int		 SEL_OR_NONSEL(fpreal t)
		{ return evalInt(SOP_VISIBILITY_SEL_OR_NONSEL_INDEX, 0, t); }
    int		 THREE_D_OR_TWO_D(fpreal t)
		{ return evalInt(SOP_VISIBILITY_3D_OR_2D_INDEX, 0, t); }
    int		 CUMULATIVE(fpreal t)
		{ return evalInt(SOP_VISIBILITY_CUMULATIVE, 0, t); }

    GA_PrimitiveGroup *myGroup;
};

#endif
