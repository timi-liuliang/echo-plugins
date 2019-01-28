/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Polygonal knitting filter. (C++)
 *
 * COMMENTS:
 */


#ifndef __SOP_PolyKnit_h__
#define __SOP_PolyKnit_h__

#include "SOP_API.h"

#include <UT/UT_IntArray.h>

#include "SOP_Node.h"

class SOP_API SOP_PolyKnit : public SOP_Node
{
public:
	     SOP_PolyKnit(OP_Network *net, const char *, OP_Operator *entry);
    virtual ~SOP_PolyKnit();

//    virtual OP_ERROR	 cookInputGroups(OP_Context &context, int alone = 0);

    virtual bool	 updateParmsFlags();

    static OP_Node	*myConstructor(OP_Network  *net, const char *name,
				       OP_Operator *entry);
    static PRM_Template	 myTemplateList[];

    void		 setPointGroup(UT_String &str, CH_StringMeaning meaning,
				       fpreal t)
					{ setString(str, meaning, 0, 0, t); }
    void		 getPointGroup(UT_String &str, fpreal t)
					{ evalString(str, 0, 0, t); }

protected:
    virtual OP_ERROR	 cookMySop (OP_Context &context);
    virtual const char 	*inputLabel(unsigned idx) const;

private:
    //		getPointGroup is in public because state needs it
    int		COLLAPSEQUADS(void)	{ return evalInt(1, 0, 0); }
    int		REMOVEDEGENERATE(void)	{ return evalInt(2, 0, 0); }
    int		FLIP(void)		{ return evalInt(3, 0, 0); }
    int		UNIQUEPOINTS(void)	{ return evalInt(4, 0, 0); }
    int		UPDATENORMALS(void)	{ return evalInt(5, 0, 0); }
    int		KEEPORIGINAL(void)	{ return evalInt(6, 0, 0); }

    GU_Detail			*mySource;
};

#endif
