/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_SopScalarField.h ( SIMZ Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_SopScalarField_h__
#define __SIM_SopScalarField_h__

#include "SIMZ_API.h"
#include <OP/OP_Version.h>
#include <SIM/SIM_ScalarField.h>
#include <SIM/SIM_OptionsUser.h>
#include "SIMZ_Utils.h"

// This SIM_ScalarField implementation gets geometry from a SOP.
class SIMZ_API SIM_SopScalarField : public SIM_ScalarField
{
public:
    // Access functions for our specific data.
    GETSET_DATA_FUNCS_B("usesopdimensions", UseSopDimensions);
    GETSET_DATA_FUNCS_B("sopresizeonly", SopResizeOnly);

    GETSET_DATA_FUNCS_F(SIM_NAME_TIME, Time);
    GETSET_DATA_FUNCS_F("scalevalue", ScaleValue);
    GETSET_DATA_FUNCS_S(SIM_NAME_SOPPATH, SopPath);
    GETSET_DATA_FUNCS_I("primnum", PrimNumber);
    GETSET_DATA_FUNCS_B(SIM_NAME_USETRANSFORM, UseTransform);
    GETSET_DATA_FUNCS_B(SIM_NAME_LETSOPSINTERPOLATE, LetSopsInterpolate);

protected:
    explicit		 SIM_SopScalarField(const SIM_DataFactory *factory);
    virtual		~SIM_SopScalarField();

    // Method for accessing the geometry.
    virtual void		 handleModificationSubclass(int code);
    virtual void		 interpolateSubclass(const SIM_Data *source1,
						     const SIM_Data *source2,
						     fpreal interp);

private:
    void				 updateFieldFromOptions();

    static const SIM_DopDescription	*getSopScalarFieldDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SopScalarField,
			SIM_ScalarField,
			"SOP Scalar Field",
			getSopScalarFieldDopDescription());
};

#endif

