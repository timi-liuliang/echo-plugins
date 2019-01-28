/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RestPropertiesBase_h__
#define __SIM_RestPropertiesBase_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"
#include "SIM_DopDescription.h"

#include <GU/GU_DetailHandle.h>

#define SIM_DATA_NAME_REST_PROPERTIES "RestProperties"

#define SIM_NAME_IMPORT_REST_GEOMETRY "importrestgeometry"
#define SIM_NAME_REST_GEOMETRY_PATH   "restgeometrypath"

class OP_Context;

class SIM_API SIM_RestPropertiesBase : public SIM_Data,
                                       public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_B(SIM_NAME_IMPORT_REST_GEOMETRY, ImportGeometry);
    GETSET_DATA_FUNCS_S(SIM_NAME_REST_GEOMETRY_PATH, GeometryPath);

    virtual GU_ConstDetailHandle getGeoHandle(
        OP_Context& context
    ) const = 0;

protected:
    explicit SIM_RestPropertiesBase(const SIM_DataFactory* factory);
    virtual ~SIM_RestPropertiesBase();

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_RestPropertiesBase, SIM_Data);
};

#endif
