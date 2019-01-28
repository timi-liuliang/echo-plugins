/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Duplicate.h ( GU Library, C++) (based on GU_Copy.h)
 *
 * COMMENTS:	Duplicate (copy) subset of a gdp with a transform.
 * 		Retain source geometry.
 */

#ifndef __GU_Duplicate__
#define __GU_Duplicate__

#include "GU_API.h"

#include <GA/GA_Types.h>
#include <UT/UT_VectorTypes.h>
class UT_String;
class UT_Interrupt;
class GA_AttributeRefMap;
class GA_PrimitiveGroup;
class GA_PointGroup;
class GEO_Detail;

typedef int (*GU_DuplicateTransformCallback)(UT_Matrix4&,int copynum,void*data);

typedef int (*GU_DuplicateGroupNameCallback)(UT_String&,void*data);

class GU_API GU_DuplicateParms
{
public:
    GU_DuplicateParms();
    ~GU_DuplicateParms() {}

    void	setKeepVectorLength(bool onoff) { myKeepFlag = onoff; }
    void	setPreserveGroups(bool onoff) { myPreserveGroups = onoff; }
    void	setTransformCallback(GU_DuplicateTransformCallback callback)
		{
		    myXformCallback = callback;
		}
    void	setGroupNameCallback(GU_DuplicateGroupNameCallback callback)
		{
		    myGNameCallback = callback;
		}

    // If a setSource is used, the callback is ignored.

    void	setSource(const GEO_Detail *inputgdp, const GEO_Detail *dupgdp) 
		{
		    myInputSource = inputgdp;
		    myDuplicateSource = dupgdp;
		}

    void	duplicate(GEO_Detail *gdp, int ncopies,
				bool changed, void *data = nullptr);

private:

    void transform(
        GEO_Detail *gdp, bool changed,
        const UT_Matrix4 &xform, GA_PrimitiveGroup *grp,
        GA_Index primstart, GA_Index primend,
        GA_Index ptstart, GA_Index ptend,
        GA_AttributeRefMap &ptattribs,
        GA_AttributeRefMap &vtxattribs,
        GA_AttributeRefMap &primattribs);

    GU_DuplicateTransformCallback myXformCallback;
    GU_DuplicateGroupNameCallback myGNameCallback;
    
    const GEO_Detail *myInputSource;
    const GEO_Detail *myDuplicateSource;

    bool myKeepFlag;
    bool myPreserveGroups;
};

#endif

