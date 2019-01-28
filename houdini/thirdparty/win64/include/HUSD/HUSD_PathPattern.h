/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_PathPattern_h__
#define __HUSD_PathPattern_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_Utils.h"
#include <UT/UT_PathPattern.h>

class HUSD_API HUSD_PathPattern : public UT_PathPattern
{
public:
			 HUSD_PathPattern(const UT_StringArray &pattern_tokens,
				HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands);
			 HUSD_PathPattern(const UT_StringRef &pattern,
				HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands);
			~HUSD_PathPattern();

protected:
    virtual bool	 matchSpecialToken(
				const UT_StringRef &path,
				const Token &token) const;

private:
    void		 initializeSpecialTokens(HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands);
};

#endif

