/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SopGeometry_h__
#define __SIM_SopGeometry_h__

#include "SIMZ_API.h"
#include <SIM/SIM_Geometry.h>
#include "SIMZ_Utils.h"
#include <UT/UT_WorkBuffer.h>

#define SIM_SOPGEOMETRY_USESOPPATH		"usesoppath"
// This SIM_Geometry implementation gets geometry from a SOP.
class SIMZ_API SIM_SopGeometry : public SIM_Geometry
{
public:
    // Access functions for our specific data.
    GETSET_DATA_FUNCS_F(SIM_NAME_TIME, Time);
    GETSET_DATA_FUNCS_B(SIM_SOPGEOMETRY_USESOPPATH, UseSopPath);
    GETSET_DATA_FUNCS_S(SIM_NAME_SOPPATH, SopPath);
    GETSET_DATA_FUNCS_S(SIM_NAME_PRIMGROUP, PrimGroup);
    GETSET_DATA_FUNCS_B(SIM_NAME_USETRANSFORM, UseTransform);
    GETSET_DATA_FUNCS_F("transformtime", TransformTime);
    GETSET_DATA_FUNCS_B(SIM_NAME_LETSOPSINTERPOLATE, LetSopsInterpolate);

    GETSET_DATA_FUNCS_I("numstamps", NumberOfStamps);

    fpreal64		getStampValue(int stampno) const
    { UT_WorkBuffer buf;  buf.sprintf("stampvalue%d", stampno); return getOptions().getOptionF(buf.buffer()); }

    void 		getStampParam(UT_String &value, int stampno) const
    { UT_WorkBuffer buf;  buf.sprintf("stampname%d", stampno); return getOptions().getOptionS(buf.buffer(), value); }


protected:
    explicit		 SIM_SopGeometry(const SIM_DataFactory *factory);
    virtual		~SIM_SopGeometry();

    // Method for accessing the geometry.
    virtual void		 handleModificationSubclass(int code);
    virtual void		 interpolateSubclass(const SIM_Data *source1,
						     const SIM_Data *source2,
						     fpreal interp);

private:
    void				 updateGeometryFromOptions();

    static const SIM_DopDescription	*getSopGeometryDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SopGeometry,
			SIM_Geometry,
			"SOP Geometry",
			getSopGeometryDopDescription());
};

#endif

