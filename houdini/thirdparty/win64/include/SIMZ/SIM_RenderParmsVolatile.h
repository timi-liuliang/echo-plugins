/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RenderParmsVolatile_h__
#define __SIM_RenderParmsVolatile_h__

#include "SIMZ_API.h"

#include <SIM/SIM_RenderParms.h>

/// This class holds rendering options much like object rendering parameters.
/// To use this data type, attach it as subdata to a SIM_Geometry that is
/// being rendered.
/// This is a volatile version that reloads its value directly from
/// the opnode rather than using the data.
class SIMZ_API SIM_RenderParmsVolatile : SIM_RenderParms
{
public:
    /// These methods check the creatorNode to query its current
    /// parm value and will pass down to the actual options only
    /// if the creator node can't be found.
    virtual void	 getMaterialShop(const SIM_Object *obj, UT_String &str) const;
    virtual bool	 getForDisplay(const SIM_Object *obj) const;
    virtual bool	 getForRender(const SIM_Object *obj) const;
    

protected:
    explicit		 SIM_RenderParmsVolatile(const SIM_DataFactory *factory);
    virtual		~SIM_RenderParmsVolatile();

private:
    static const SIM_DopDescription	*getRenderParmsVolatileDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RenderParmsVolatile,
			SIM_RenderParms,
			"Rendering Parameters",
			getRenderParmsVolatileDopDescription());
};

#endif

