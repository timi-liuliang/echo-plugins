/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RenderParms_h__
#define __SIM_RenderParms_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class SIM_Object;

/// This class holds rendering options much like object rendering parameters.
/// To use this data type, attach it as subdata to a SIM_Geometry that is
/// being rendered.
class SIM_API SIM_RenderParms : public SIM_Data,
				public SIM_OptionsUser
{
public:
    GET_DATA_FUNC_S(SIM_NAME_MATERIALSHOP, MaterialShopRaw);
    GET_DATA_FUNC_I(SIM_NAME_FORDISPLAY, ForDisplayRaw);
    GET_DATA_FUNC_I(SIM_NAME_FORRENDER, ForRenderRaw);

    /// These methods just pass down to the raw options version
    /// SIM_RenderParmsVolatile uses this hook to do a just in time
    /// parm evaluation.
    virtual void	 getMaterialShop(const SIM_Object *, UT_String &str) const
			 {
			     getMaterialShopRaw(str);
			 }
    virtual bool	 getForDisplay(const SIM_Object *) const
			 {
			     return getForDisplayRaw();
			 }
    virtual bool	 getForRender(const SIM_Object *) const
			 {
			     return getForRenderRaw();
			 }
    

protected:
    explicit		 SIM_RenderParms(const SIM_DataFactory *factory);
    virtual		~SIM_RenderParms();

private:
    static const SIM_DopDescription	*getRenderParmsDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RenderParms,
			SIM_Data,
			"Rendering Parameters",
			getRenderParmsDopDescription());
};

#endif

