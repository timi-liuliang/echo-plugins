/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP library (C++)
 *
 * COMMENTS:	UVFlatten SOP
 */


#ifndef __SOP_UVFlatten_h__
#define __SOP_UVFlatten_h__

#include "SOP_API.h"
#include <GU/GU_Flatten.h>

#include "SOP_Node.h"

class DEP_MicroNode;

namespace sop_uvflatten_private {
enum
{
    SOP_FLATTEN_PRIMGROUP_INDEX = 0,
    SOP_FLATTEN_SEAMGROUP_INDEX,
    SOP_FLATTEN_EXTRASEAMGROUP_INDEX,
    SOP_FLATTEN_AXISALIGNISLANDS_INDEX,
    SOP_FLATTEN_OPTIMIZE_WASTED_SPACE,
    SOP_FLATTEN_USEEXISTINGUV_INDEX,
    SOP_FLATTEN_ISLANDPRIM_INDEX,
    SOP_FLATTEN_CREATEPINGROUPS_INDEX,
    SOP_FLATTEN_INITIALPINGROUP_INDEX,
    SOP_FLATTEN_ACTIVEPINGROUP_INDEX,
    SOP_FLATTEN_PINGROUP_INDEX,
    SOP_FLATTEN_USEPINS_INDEX,
    SOP_FLATTEN_PINNEDPOINTS_INDEX
};
}

using namespace sop_uvflatten_private;

class SOP_API SOP_UVFlatten : public SOP_Node
{
public:
			 SOP_UVFlatten(OP_Network *net, const char *name,
				 OP_Operator *op);
			~SOP_UVFlatten();

    virtual int		 isRefInput(unsigned int i) const;

    // This method is created so that it can be called by handles.  It only
    // cooks the input group of this SOP.  The geometry in this group is
    // the only geometry manipulated by this SOP.
    virtual OP_ERROR	 cookInputGroups(OP_Context &context, int alone = 0);
    OP_ERROR		 cookExtraSeamsGroup(OP_Context &context,
				int alone = 0);
    virtual bool	 updateParmsFlags();

    virtual void syncNodeVersion(const char *old_version,
        const char *cur_version, bool *node_deleted);

    static PRM_Template	 myTemplateList[];
    static OP_Node	*myConstructor(OP_Network*,const char *, OP_Operator *);

protected:
    virtual const char	*inputLabel(unsigned idx) const;

    // Method to cook geometry for the SOP
    virtual OP_ERROR	 cookMySop(OP_Context &context);

    bool 		 mustReflattenAll(fpreal now);
    bool		 extraCutsChanged(fpreal now);

private:

    bool	UESEXISTINGUV()
		    { return evalInt(SOP_FLATTEN_USEEXISTINGUV_INDEX, 0, 0); }
    bool	ISLANDTOGGLE()
		    { return evalInt(SOP_FLATTEN_ISLANDPRIM_INDEX, 0, 0); }
    bool	CREATEPINGROUPS()
		    { return evalInt(SOP_FLATTEN_CREATEPINGROUPS_INDEX, 0, 0); }
    bool	USEPINS()
    		    { return evalInt(SOP_FLATTEN_USEPINS_INDEX, 0, 0); }
    void	INITIALPINGROUP(UT_String &s)
		    { evalString(s, SOP_FLATTEN_INITIALPINGROUP_INDEX, 0, 0); }
    void	ACTIVEPINGROUP(UT_String &s)
		    { evalString(s, SOP_FLATTEN_ACTIVEPINGROUP_INDEX, 0, 0); }
    int		NUMPINNED()
		    { return evalInt("pins", 0, 0.0f); }
    int		USEPOINT(int i)
		    { return evalIntInst("usepin#", &i, 0, 0.0f); }
    fpreal	POINTVAL(int i, int vi, fpreal t)
		    { return evalFloatInst("pinuv#", &i, vi, t); }
    int		PRIM(int i)
    		    { return evalIntInst("primvert#", &i, 0, 0.0f); }
    int		VERT(int i)
    		    { return evalIntInst("primvert#", &i, 1, 0.0f); }
    fpreal	U(int i, fpreal t)
		    { return POINTVAL(i, 0, t); }
    fpreal	V(int i, fpreal t)
		    { return POINTVAL(i, 1, t); }

    const
    GA_PrimitiveGroup	*myGroup;

    const GA_EdgeGroup	*mySeams;
    const GA_EdgeGroup	*myExtraSeams;

    GU_Flatten		*myFlatten;
    DEP_MicroNode	myReflattenAllMicroNode;
    DEP_MicroNode	myExtraCutsMicroNode;
    bool		myCookInterrupted;
};

#endif
