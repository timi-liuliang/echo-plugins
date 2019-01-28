/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_POPShapeMatch_h__
#define __SIM_POPShapeMatch_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class SIM_API SIM_POPShapeMatch : public SIM_Data,
				 public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_F("restoration", Restoration);
    GETSET_DATA_FUNCS_F("squash", Squash);
    GETSET_DATA_FUNCS_F("framedrag", FrameDrag);

protected:
    explicit		 SIM_POPShapeMatch(const SIM_DataFactory *factory);
    virtual		~SIM_POPShapeMatch();

private:
    static const SIM_DopDescription	*getPOPShapeMatchDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_POPShapeMatch,
			SIM_Data,
			"POP Shape Match",
			getPOPShapeMatchDopDescription());
};

#endif

