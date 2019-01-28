/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_SopVectorField.h ( SIMZ Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_SopVectorField_h__
#define __SIM_SopVectorField_h__

#include "SIMZ_API.h"
#include <OP/OP_Version.h>
#include <SIM/SIM_VectorField.h>
#include <SIM/SIM_OptionsUser.h>
#include "SIMZ_Utils.h"

// This SIM_VectorField implementation gets geometry from a SOP.
class SIMZ_API SIM_SopVectorField : public SIM_VectorField
{
public:
    // Access functions for our specific data.
    GETSET_DATA_FUNCS_B("usesopdimensions", UseSopDimensions);
    GETSET_DATA_FUNCS_B("sopresizeonly", SopResizeOnly);
    GETSET_DATA_FUNCS_F(SIM_NAME_TIME, Time);
    GETSET_DATA_FUNCS_S(SIM_NAME_SOPPATH, SopPath);
    GETSET_DATA_FUNCS_V3("primnum", PrimNumber);
    GETSET_DATA_FUNCS_V3("scalevalue", ScaleValues);
    GETSET_DATA_FUNCS_B(SIM_NAME_USETRANSFORM, UseTransform);
    GETSET_DATA_FUNCS_B(SIM_NAME_LETSOPSINTERPOLATE, LetSopsInterpolate);

protected:
    explicit		 SIM_SopVectorField(const SIM_DataFactory *factory);
    virtual		~SIM_SopVectorField();

    // Method for accessing the geometry.
    virtual void		 handleModificationSubclass(int code);
    virtual void		 interpolateSubclass(const SIM_Data *source1,
						     const SIM_Data *source2,
						     fpreal interp);

private:
    void				 updateFieldFromOptions();

    static const SIM_DopDescription	*getSopVectorFieldDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SopVectorField,
			SIM_VectorField,
			"SOP Vector Field",
			getSopVectorFieldDopDescription());
};

#endif

