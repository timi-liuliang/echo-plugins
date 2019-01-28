/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_SetParamScope.h (CH Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CH_SETPARAMSCOPE_H_INCLUDED__
#define __CH_SETPARAMSCOPE_H_INCLUDED__

#include "CH_API.h"
#include "CH_Manager.h"
#include <DEP/DEP_MicroNode.h>

class CH_API CH_SetGlobalParamScope : public DEP_KeepDependenciesScope
{
public:
    CH_SetGlobalParamScope(CH_Manager &mgr)
	: DEP_KeepDependenciesScope()
	, myMgr(mgr)
    {
	++myMgr.mySettingGlobalParam;
    }
    ~CH_SetGlobalParamScope()
    {
	--myMgr.mySettingGlobalParam;
    }

private:
    CH_Manager &myMgr;
};

class CH_API CH_SetGuideParamScope : public DEP_KeepDependenciesScope
{
public:
    CH_SetGuideParamScope(CH_Manager &mgr)
	: DEP_KeepDependenciesScope()
	, myMgr(mgr)
    {
	++myMgr.mySettingGuideParam;
    }
    ~CH_SetGuideParamScope()
    {
	--myMgr.mySettingGuideParam;
    }

private:
    CH_Manager &myMgr;
};

#endif // __CH_SETPARAMSCOPE_H_INCLUDED__
