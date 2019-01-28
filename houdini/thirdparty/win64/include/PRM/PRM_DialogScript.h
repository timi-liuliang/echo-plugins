/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	This defines the mappings for the dialog scripts (C++)
 *
 * COMMENTS:
 */

#ifndef __PRM_DialogScript_h__
#define __PRM_DialogScript_h__

#include "PRM_API.h"

//
//  WARNING:  When you change this list, make sure to change PSI2_DialogPRM.C
//

enum PRM_DialogScript {
    PRM_DialogRManLight,
    PRM_DialogRManShader,
    PRM_DialogRManDisplace,
    PRM_DialogRManAtmosphere,
    PRM_DialogRManInterior,

    PRM_DialogGenericLight,
    PRM_DialogGenericShader,
    PRM_DialogGenericDisplace,
    PRM_DialogGenericAtmosphere,

    PRM_DialogLightShader,
    PRM_DialogLightAtmosphere,
    PRM_DialogAtmosphere,

    PRM_DialogObjectMacro,
    PRM_DialogSopMacro,
    PRM_DialogIceMacro,
    PRM_DialogShaderMacro,

    PRM_DialogRenderers,

    PRM_DialogCopVex,
    PRM_DialogCop2Vex,
    PRM_DialogChopVex,
    PRM_DialogSopVex,
    PRM_DialogPopVex,
    PRM_DialogObjVex,

    PRM_DialogVexSurface,
    PRM_DialogVexPhoton,
    PRM_DialogVexDisplace,
    PRM_DialogVexAtmosphere,
    PRM_DialogVexLight,
    PRM_DialogVexShadow,

    PRM_DialogCVex,

    PRM_DialogRIBProcedural,
    PRM_DialogMantraProcedural,

    PRM_DialogUser1,
    PRM_DialogUser2,
    PRM_DialogUser3,
    PRM_DialogUser4,
    PRM_DialogUser5,

    PRM_MAX_DIALOG_SCRIPTS		// Sentinal
};

PRM_API	extern PRM_DialogScript	 PRMlookupDialogScript(const char *name);
PRM_API extern const char	*PRMlookupDialogScript(PRM_DialogScript v);

#endif
