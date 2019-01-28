/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The UV Fuse SOP
 *
 */

#ifndef __SOP_UVFuse_h__
#define __SOP_UVFuse_h__

#include "SOP_API.h"
#include "SOP_Node.h"

enum {
    SOP_UVFUSE_UVATTRIB_IDX = 0,
    SOP_UVFUSE_GRP_IDX,
    SOP_UVFUSE_GRPTYPE_IDX,

    SOP_UVFUSE_MANPOS_IDX,

    SOP_UVFUSE_POSTYPE_IDX,
    SOP_UVFUSE_USEDIST_IDX,
    SOP_UVFUSE_DIST_IDX,
    SOP_UVFUSE_METRIC_IDX,

    SOP_UVFUSE_UVW_IDX
};

class SOP_API SOP_UVFuse : public SOP_Node
{
public:
	     SOP_UVFuse(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_UVFuse();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];

    virtual OP_ERROR		 cookInputGroups(OP_Context &context,
						 int alone = 0);

    virtual void getDescriptiveParmName(UT_String &name) const
    {
        name = "uvattrib";
    }

protected:
    virtual OP_ERROR		 cookMySop(OP_Context &context);
    virtual const char		*inputLabel(unsigned idx) const;

private:
    void	 getGroupString(UT_String &str)
			    { evalString(str, SOP_UVFUSE_GRP_IDX, 0, 0.0f); }
    
    int		 GRPTYPE()		{ return evalInt(SOP_UVFUSE_GRPTYPE_IDX, 0, 0); }

    int		 MANPOS()		{ return evalInt(SOP_UVFUSE_MANPOS_IDX, 0, 0); }
    
    // The Distance Folder
    // -------------------
    int		 POSTYPE()		{ return evalInt(SOP_UVFUSE_POSTYPE_IDX, 0, 0); }
    int		 USEDIST()		{ return evalInt(SOP_UVFUSE_USEDIST_IDX, 0, 0); }
    float	 DISTANCE(fpreal t)	{ return evalFloat(SOP_UVFUSE_DIST_IDX, 0, t); }
    int		 METRIC()		{ return evalInt(SOP_UVFUSE_METRIC_IDX, 0, 0); }

    // The Manual Folder
    // -----------------
    float	 U(fpreal t)		{ return evalFloat(SOP_UVFUSE_UVW_IDX, 0, t); }
    float	 V(fpreal t)		{ return evalFloat(SOP_UVFUSE_UVW_IDX, 1, t); }
    float	 W(fpreal t)		{ return evalFloat(SOP_UVFUSE_UVW_IDX, 2, t); }

    // The Grid Folder
    // ---------------
    int		LINETYPE()		 { return   evalInt(9, 0, 0); }
    float	SPACINGX(fpreal t)	 { return evalFloat(10, 0, t); }
    float	SPACINGY(fpreal t)	 { return evalFloat(10, 1, t); }
    float	SPACINGZ(fpreal t)	 { return evalFloat(10, 2, t); }
    float	NUMLINESX(fpreal t)	 { return evalFloat(11, 0, t); }
    float	NUMLINESY(fpreal t)	 { return evalFloat(11, 1, t); }
    float	NUMLINESZ(fpreal t)	 { return evalFloat(11, 2, t); }
    int 	POW2X(fpreal t)	 	 { return   evalInt(12, 0, t); }
    int 	POW2Y(fpreal t)	 	 { return   evalInt(12, 1, t); }
    int 	POW2Z(fpreal t)	 	 { return   evalInt(12, 2, t); }
    float	OFFSETX(fpreal t)	 { return evalFloat(13, 0, t); }
    float	OFFSETY(fpreal t)	 { return evalFloat(13, 1, t); }
    float	OFFSETZ(fpreal t)	 { return evalFloat(13, 2, t); }
    int		GRIDROUNDTYPE()		 { return   evalInt(14, 0, 0); }
    float	GRIDTOL(fpreal t)	 { return evalFloat(15, 0, t); }

    const GA_Group *myGroup;
};

#endif
