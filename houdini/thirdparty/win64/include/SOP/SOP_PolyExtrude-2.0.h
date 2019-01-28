/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_PolyExtrude-2.0.h (C++)
 *
 */


#ifndef __SOP_PolyExtrude2__
#define __SOP_PolyExtrude2__

#include "SOP_API.h"
#include "SOP_Node.h"
#include <GU/GU_PolyExtrude2.h>

class SOP_API SOP_PolyExtrude2 : public SOP_Node
{
public:
			 SOP_PolyExtrude2(OP_Network *net,
				const char *name,
				OP_Operator *entry);
    virtual 		~SOP_PolyExtrude2();

    virtual OP_ERROR	 cookInputGroups(
				OP_Context &context, int alone = 0);

    virtual bool	 updateParmsFlags();

    virtual void	 syncNodeVersion(const char *old_version,
				const char *cur_version,
				bool *node_deleted);

    static OP_Node	*myConstructor(OP_Network *net,
				const char *name,
				OP_Operator *entry);

    static PRM_Template	 myTemplateList[];

    static const char	*theHandleCenterAttribName;
    static const char	*theHandleXformAttribName;
    static const char	*theDraggerStartAttribName;
    static const char	*theDraggerDirAttribName;

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    virtual const char  *inputLabel(unsigned idx) const;

private:

    typedef GU_PolyExtrude2::SpineSampleMethod 	SpineSampleMethod;

    void		 getGroupString(UT_String &str, fpreal t)
			    { evalString(str, 0, 0, t); }

    bool		 setExtrudeXform(GU_PolyExtrude2 &extrude, fpreal t);
    GA_Group		*setupOutputGroups(GU_PolyExtrude2 *extruder, fpreal t);
    void		 setExtruderParms(GU_PolyExtrude2 *extruder,
					  GEO_Face *external_spine, fpreal t,
					  UT_Ramp &thickness_ramp,
					  UT_Ramp &twist_ramp);
    void		 refreshHandleData(GU_PolyExtrude2 *extruder);

    void		 convertToPolys();
    GA_EdgeGroup	*switchGroupTypeForOpenCurves();

    const GA_Group 	*myGroup;
    const GA_EdgeGroup	*mySplitEdges;
    const GA_PointGroup	*mySplitPoints;

};

#endif
