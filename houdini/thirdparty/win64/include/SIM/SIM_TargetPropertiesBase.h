/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_TargetPropertiesBase_h__
#define __SIM_TargetPropertiesBase_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"
#include "SIM_DopDescription.h"
#include "SIM_Names.h"

#include <GU/GU_DetailHandle.h>

#define SIM_DATA_NAME_TARGET_PROPERTIES "TargetProperties"

#define SIM_NAME_IMPORT_TARGET_GEOMETRY "importtargetgeometry"
#define SIM_NAME_TARGET_GEOMETRY_PATH   "targetgeometrypath"
#define SIM_NAME_TARGET_STIFFNESS       "targetstiffness"
#define SIM_NAME_TARGET_DAMPING         "targetdamping"

class OP_Context;

class SIM_API SIM_TargetPropertiesBase : public SIM_Data,
                                         public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_B(SIM_NAME_IMPORT_TARGET_GEOMETRY, ImportGeometry);
    GETSET_DATA_FUNCS_S(SIM_NAME_TARGET_GEOMETRY_PATH, GeometryPath);

    GETSET_DATA_FUNCS_F(SIM_NAME_TARGET_STIFFNESS, Stiffness)
    GETSET_DATA_FUNCS_F(SIM_NAME_TARGET_DAMPING, Damping)

    virtual GU_ConstDetailHandle getGeoHandle(
        OP_Context& context
    ) const = 0;

protected:
    explicit SIM_TargetPropertiesBase(const SIM_DataFactory* factory);
    virtual ~SIM_TargetPropertiesBase();

private:
    static const SIM_DopDescription* getTargetPropertiesDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_TargetPropertiesBase, SIM_Data);
};

#endif
